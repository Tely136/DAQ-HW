#include <stdint.h>
#include <limits.h>
#include <math.h>

#include "params.h"

#include "xtrigger.h"
#include "xpulse_source.h"
#include "xmulti_channel_scaler.h"

struct daq_device {
    int configured;
    volatile uint32_t* counts;

    void* ddr_mapping;
    void* trigger_mapping;
    void* source_mapping;
    void* mcs_mapping;

    int irq_fd;

    XTrigger trigger;
    XPulse_source source;
    XMulti_channel_scaler mcs;
};

struct daq_config {
    uint32_t n_shots;
    uint32_t n_bins;
    uint32_t cycles_per_bin;
    uint32_t source_period_cycles;
    double trigger_rate_hz;
    uint32_t trigger_period_cycles;
};

void daq_count_bounds(const struct daq_config* config, uint64_t* min_counts, uint64_t* max_counts) {
    uint32_t period = config->source_period_cycles;
    uint32_t cycles = config->cycles_per_bin;
    *min_counts = 0;
    *max_counts = 0;
    if (period != 0) {
        *min_counts = (uint64_t)config->n_shots * (cycles / period);
        *max_counts = (uint64_t)config->n_shots *
                      (cycles / period + (cycles % period != 0));
    }
}
static double elapsed(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
} 

void* map_device(uint64_t address, size_t size) {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("failed to open /dev/mem");
        return NULL;
    }

    void* mapping = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, address);
    if (mapping == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return NULL;
    }
    close(fd);
    return mapping;
}

int daq_open(struct daq_device* daq) {

    // map trigger
    daq->trigger_mapping = map_device(TRIGGER_ADDRESS, TRIGGER_SIZE);
    if (daq->trigger_mapping == NULL) {
        return 1;
    }

    // trigger instance
    XTrigger trigger = {
        .Control_BaseAddress = (u64)(uintptr_t) daq->trigger_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };
    daq->trigger = trigger;

    // map source
    daq->source_mapping = map_device(SOURCE_ADDRESS, SOURCE_SIZE);
    if (daq->source_mapping == NULL) {
        return 1;
    }

    // source instance
    XPulse_source source = {
        .Control_BaseAddress = (u64)(uintptr_t) daq->source_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };
    daq->source = source;

    // map mcs
    daq->mcs_mapping = map_device(MCS_ADDRESS, MCS_SIZE);
    if (daq->mcs_mapping == NULL) {
        return 1;
    }

    // mcs instance
    XMulti_channel_scaler mcs = {
        .Control_BaseAddress = (u64)(uintptr_t) daq->mcs_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };
    daq->mcs = mcs;

    // map the output buffer reserved for MCS
    daq->ddr_mapping = map_device(COUNTS_ADDRESS, COUNTS_SIZE);
    if (daq->ddr_mapping == NULL) {
        return 1;
    }
    daq->counts = daq->ddr_mapping;

    XTrigger_Set_enable(&daq->trigger, 0);
    XPulse_source_Set_enable(&daq->source, 0);

    // disable mcs interrupts and clear any pending ones
    XMulti_channel_scaler_InterruptGlobalDisable(&daq->mcs);
    XMulti_channel_scaler_InterruptDisable(&daq->mcs, 0x3);
    XMulti_channel_scaler_InterruptClear(&daq->mcs, XMulti_channel_scaler_InterruptGetStatus(&daq->mcs));
    (void)XMulti_channel_scaler_InterruptGetStatus(&daq->mcs);

    // open UIO device for interrupt handling
    int irq_fd = open("/dev/uio0", O_RDWR);
    if (irq_fd < 0) {
        perror("open /dev/uio0");
        return 1;
    }
    daq->irq_fd = irq_fd;

    return 0;
}

int daq_close(struct daq_device* daq) {

    if (daq->trigger_mapping) {
        XTrigger_Set_enable(&daq->trigger, 0);
    }
    if (daq->source_mapping) {
        XPulse_source_Set_enable(&daq->source, 0);
    }
    if (daq->irq_fd >= 0) {
        close(daq->irq_fd);
    }

    // unmap memory regions
    if (daq->trigger_mapping) {
        munmap(daq->trigger_mapping, TRIGGER_SIZE);
    }
    if (daq->source_mapping) {
        munmap(daq->source_mapping, SOURCE_SIZE);
    }
    if (daq->mcs_mapping) {
        munmap(daq->mcs_mapping, MCS_SIZE);
    }

    if (daq->ddr_mapping) {
        munmap(daq->ddr_mapping, COUNTS_SIZE);
    }

    return 0;
}

int daq_configure(struct daq_device* daq, struct daq_config* config, char* message) {

    // CONFIG n_shots n_bins cycles_per_bin source_period_cycles trigger_rate_hz
    daq->configured = 0;
    unsigned long long shots, bins, cycles, period;
    double trigger_rate_hz;
    char extra;
    int matched = sscanf(message, "CONFIG %llu %llu %llu %llu %lf %c",
                         &shots, &bins, &cycles, &period, &trigger_rate_hz, &extra);
    if (matched != 5 || shots == 0 || shots > INT_MAX ||
        bins == 0 || bins > MAX_BINS || cycles == 0 || cycles > INT_MAX ||
        period > UINT32_MAX) {
        fprintf(stderr, "Invalid CONFIG message: %s\n", message);
        return 1;
    }
    // the trigger requires at least two clocks between rising edges
    if (!isfinite(trigger_rate_hz) || trigger_rate_hz < (double)CLOCK_HZ / UINT32_MAX ||
        trigger_rate_hz > CLOCK_HZ / 2.0) {
        fprintf(stderr, "Trigger rate must be between %.9g and %.9g Hz\n",
                (double)CLOCK_HZ / UINT32_MAX, CLOCK_HZ / 2.0);
        return 1;
    }
    uint32_t trigger_period_cycles = (uint32_t)((double)CLOCK_HZ / trigger_rate_hz + 0.5);

    // MCS currently accumulates into signed 32-bit integers
    if (shots * cycles > INT_MAX) {
        fprintf(stderr, "Requested counts could overflow MCS accumulation\n");
        return 1;
    }

    config->n_shots = shots;
    config->n_bins = bins;
    config->cycles_per_bin = cycles;
    config->source_period_cycles = period;
    config->trigger_rate_hz = trigger_rate_hz;
    config->trigger_period_cycles = trigger_period_cycles;

    // stop the source and trigger before changing their periods
    XTrigger_Set_enable(&daq->trigger, 0);
    XPulse_source_Set_enable(&daq->source, 0);
    usleep(1000);
    XTrigger_Set_clk_periods(&daq->trigger, config->trigger_period_cycles);
    XPulse_source_Set_clk_periods(&daq->source, config->source_period_cycles);
    // set number of shots, bins, cycles, and output address for mcs
    XMulti_channel_scaler_Set_n_shot(&daq->mcs,  config->n_shots);
    XMulti_channel_scaler_Set_n_bin(&daq->mcs,  config->n_bins);
    XMulti_channel_scaler_Set_n_cycle(&daq->mcs,  config->cycles_per_bin);
    XMulti_channel_scaler_Set_data_out(&daq->mcs, COUNTS_ADDRESS);

    // enable mcs interrupts
    XMulti_channel_scaler_InterruptEnable(&daq->mcs, 0x1);
    XMulti_channel_scaler_InterruptGlobalEnable(&daq->mcs);

    // verify configuration by reading back registers
    u32 periods = XTrigger_Get_clk_periods(&daq->trigger);
    u32 readback_shots = XMulti_channel_scaler_Get_n_shot(&daq->mcs);

    printf("Readback: periods=%" PRIu32 ", shots=%" PRIu32", bins=%" PRIu32 ", cycles=%" PRIu32 ", output=0x%08" PRIx64 "\n",
           periods, readback_shots, XMulti_channel_scaler_Get_n_bin(&daq->mcs), XMulti_channel_scaler_Get_n_cycle(&daq->mcs), XMulti_channel_scaler_Get_data_out(&daq->mcs));

    if (periods != config->trigger_period_cycles || readback_shots != config->n_shots ||
        XMulti_channel_scaler_Get_n_bin(&daq->mcs) != config->n_bins ||
        XMulti_channel_scaler_Get_n_cycle(&daq->mcs) != config->cycles_per_bin ||
        XMulti_channel_scaler_Get_data_out(&daq->mcs) != COUNTS_ADDRESS ||
        XPulse_source_Get_clk_periods(&daq->source) != config->source_period_cycles)
    {
        fprintf(stderr, "Register readback does not match requested configuration.\n");
        return 1;
    }

    printf("Assuming a %d Hz trigger clock: nominal acquisition %.6f s\n", CLOCK_HZ, (double)config->n_shots * config->trigger_period_cycles / CLOCK_HZ);

    printf("Trigger rate: requested %.9g Hz, programmed %.9g Hz (%u clocks)\n",
           config->trigger_rate_hz, (double)CLOCK_HZ / config->trigger_period_cycles,
           config->trigger_period_cycles);

    printf("Source period: %u clocks; source rate: %.3f events/s; bin width: %.3f us\n",
           config->source_period_cycles,
           config->source_period_cycles ? (double)CLOCK_HZ / config->source_period_cycles : 0.0,
           1e6 * config->cycles_per_bin / CLOCK_HZ);

    daq->configured = 1;

    return 0;
}

int daq_acquire(struct daq_device* daq, struct daq_config* config, uint32_t* counts_out) {

    struct timespec start, started, end, cpu_start, cpu_end;
    uint32_t enable = 1;
    uint32_t interrupt_count;
    ssize_t bytes;
    int exit_status = -1;

    int n_shots = config->n_shots;
    int n_bins = config->n_bins;
    int cycles_per_bin = config->cycles_per_bin;

    // wait for MCS to become idle before starting a new acquisition
    XMulti_channel_scaler_DisableAutoRestart(&daq->mcs);
    while (!XMulti_channel_scaler_IsIdle(&daq->mcs)) { }

    // start the continuous source before arming MCS
    XTrigger_Set_enable(&daq->trigger, 0);
    XPulse_source_Set_enable(&daq->source, 1);
    usleep(1000);
    XMulti_channel_scaler_InterruptClear(&daq->mcs,
        XMulti_channel_scaler_InterruptGetStatus(&daq->mcs));
    (void)XMulti_channel_scaler_InterruptGetStatus(&daq->mcs);
    // enable UIO interrupt
    if (write(daq->irq_fd, &enable, sizeof(enable)) != sizeof(enable)) {
        perror("enable UIO interrupt");

        goto cleanup;
    }

    // timestamp before starting the acquisition
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_start) != 0 || clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        perror("clock_gettime");

        goto cleanup;
    }

    // arm MCS before enabling the shot trigger
    XMulti_channel_scaler_Start(&daq->mcs);
    XTrigger_Set_enable(&daq->trigger, 1);
    if (clock_gettime(CLOCK_MONOTONIC, &started) != 0) {
        perror("clock_gettime");

        goto cleanup;
    }

    // wait for interrupt from UIO
    do {
        bytes = read(daq->irq_fd, &interrupt_count, sizeof(interrupt_count));
    } while (bytes < 0 && errno == EINTR);

    if (bytes != sizeof(interrupt_count)) {
        if (bytes < 0 ) {
            perror("UIO read");
        }
        else {
            fprintf(stderr, "Incomplete UIO interrupt count\n");
        }

        goto cleanup;
    }

    // timestamp after acquisition completes
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0 ||
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_end) != 0) {
        perror("clock_gettime");

        goto cleanup;
    }

    // verify the mcs interrupt status and clear it
    u32 pending = XMulti_channel_scaler_InterruptGetStatus(&daq->mcs);
    XMulti_channel_scaler_InterruptClear(&daq->mcs, pending);

    if (!(pending & 0x1)) {
        fprintf(stderr, "UIO notification without MCS completion.\n");

        goto cleanup;
    }

    // calculate elapsed time and print results
    double time_taken = elapsed(start, end);
    printf("Event completed in %f seconds\n", time_taken);
    printf("Start calls: %.6f s; interrupt wait: %.6f s; process CPU: %.6f s\n", elapsed(start, started), elapsed(started, end), elapsed(cpu_start, cpu_end));
    printf("Configured shots / elapsed time: %.3f shots/s\n", n_shots / time_taken);

    // unknown source phase allows floor or ceil counts per shot in each bin
    uint64_t min_counts, max_counts;
    daq_count_bounds(config, &min_counts, &max_counts);
    double exposure = (double)n_shots * cycles_per_bin / CLOCK_HZ;
    exit_status = 0;
    printf("Bin   Expected counts   Read counts   Counts/shot   Rate (events/s)   Check\n");
    for (int i=0; i<n_bins; i++) {
        uint32_t count = daq->counts[i];
        int passed = count >= min_counts && count <= max_counts;
        counts_out[i] = count;
        printf("%3d   %" PRIu64 "-%" PRIu64 "   %10" PRIu32 "   %11.3f   %15.3f   %s\n",
               i, min_counts, max_counts, count, (double)count / n_shots,
               count / exposure, passed ? "PASS" : "FAIL");
        if (!passed) {
            exit_status = 1;
        }
    }
    
cleanup:
    // stop the test source and shot trigger

    XPulse_source_Set_enable(&daq->source, 0);
    XTrigger_Set_enable(&daq->trigger, 0);
    XMulti_channel_scaler_DisableAutoRestart(&daq->mcs);

    // clear any pending interrupts
    XMulti_channel_scaler_InterruptClear(&daq->mcs,
    XMulti_channel_scaler_InterruptGetStatus(&daq->mcs));
        
    printf("%s\n", exit_status < 0 ? "Acquisition failed." : (exit_status == 0 ? "Self-test passed." : "Self-test failed."));
    return exit_status;
}