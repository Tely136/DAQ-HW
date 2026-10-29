#include <stdint.h>
#include <math.h>

#include "params.h"

#include "xtrigger.h"
#include "xstream_generator.h"
#include "xmulti_channel_scaler.h"

struct daq_device {
    int configured;
    volatile uint32_t* counts;

    void* bram_mapping;
    void* ddr_mapping;
    void* trig_mapping;
    void* gen_mapping;
    void* mcs_mapping;

    int irq_fd;

    XTrigger trigger;
    XStream_generator generator;
    XMulti_channel_scaler mcs;
};

struct daq_config {
    uint32_t n_bins;
    uint32_t n_clk;
    uint32_t n_shot;
};

static double elapsed(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
} 

void clear_memory(void* mapping, size_t size) {
    volatile uint32_t* mem = mapping;
    for (size_t i=0; i<size/sizeof(uint32_t); i++) {
        mem[i] = 0;
    }
}

void print_status(u32 status) {
    printf("Control register: 0x%08" PRIx32 "\n", status);
    printf("start:        %u\n", (unsigned)((status >> 0) & 1));
    printf("done:         %u\n", (unsigned)((status >> 1) & 1));
    printf("idle:         %u\n", (unsigned)((status >> 2) & 1));
    printf("ready:        %u\n", (unsigned)((status >> 3) & 1));
    printf("auto-restart: %u\n", (unsigned)((status >> 7) & 1));
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

void init_bram(void* bram_mapping, int* data, int n_total) {
    volatile uint32_t* bram = bram_mapping;
    for (int i=0; i<n_total; i++) {
        bram[i] = data[i];
    }
}

void initialize_testdata(void* memory_map, int n_bins, int n_clk) {
    int data[n_bins * n_clk];
    int id = 0;
    for (int i=0; i<n_bins; i++) {
        for (int j=0; j<n_clk; j++) {
            data[id] = (j % (i + 1) == 0);
            id++;
        }
    }
    init_bram(memory_map, data, n_bins * n_clk);
}

int daq_open(struct daq_device* daq) {

    // map bram
    daq->bram_mapping = map_device(BRAM_ADDRESS, BRAM_SIZE);
    if (daq->bram_mapping == NULL) {
        return 1;
    }

    // map trigger
    daq->trig_mapping = map_device(TRIG_ADDRESS, TRIG_SIZE);
    if (daq->trig_mapping == NULL) {
        return 1;
    }

    // trigger instance
    XTrigger trigger = {
        .Control_BaseAddress = (u64)(uintptr_t) daq->trig_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };
    daq->trigger = trigger;

    // map generator
    daq->gen_mapping = map_device(GEN_ADDRESS, GEN_SIZE);
    if (daq->gen_mapping == NULL) {
        return 1;
    }

    // generator instance
    XStream_generator generator = {
        .Control_BaseAddress = (u64)(uintptr_t) daq->gen_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };
    daq->generator = generator;

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

    // map ddr if needed
#if DATA_READ_ADDR == DDR_ADDRESS
    void* ddr_mapping = map_device(DDR_ADDRESS, DDR_SIZE);
    if (ddr_mapping == NULL) {
        return 1;
    }
    volatile uint32_t* counts = ddr_mapping;
#else
    volatile uint32_t* counts = (volatile uint32_t*)bram_mapping
                              + (DATA_READ_ADDR - BRAM_ADDRESS) / sizeof(uint32_t);
#endif
    daq->counts = counts;


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

    // unmap memory regions
    if (daq->bram_mapping) {
        munmap(daq->bram_mapping, BRAM_SIZE);
    }
    if (daq->trig_mapping) {
        munmap(daq->trig_mapping, TRIG_SIZE);
    }
    if (daq->gen_mapping) {
        munmap(daq->gen_mapping, GEN_SIZE);
    }
    if (daq->mcs_mapping) {
        munmap(daq->mcs_mapping, MCS_SIZE);
    }
#if DATA_READ_ADDR == DDR_ADDRESS
    if (daq->ddr_mapping) {
        munmap(daq->ddr_mapping, DDR_SIZE);
    }
#endif  

    return 0;
}

int daq_configure(struct daq_device* daq, struct daq_config* config, char* message) {

    // parse config message
    int matched = sscanf(message, "CONFIG %u %u %u", &config->n_shot, &config->n_bins, &config->n_clk);
    if (matched != 3) {
        fprintf(stderr, "Invalid CONFIG message: %s\n", message);
        return 1;
    }   

    int n_shot = config->n_shot;
    int n_bins = config->n_bins;
    int n_clk = config->n_clk;
    int n_total = n_bins * n_clk;

    // set number of clock periods per trigger output
    XTrigger_Set_clk_periods(&daq->trigger, CLK_PERIODS);

    // set total number of clock cycles for generator, and bram address for input data
    XStream_generator_Set_n_bin(&daq->generator, n_bins);
    XStream_generator_Set_n_cycle(&daq->generator, n_clk);

    // set number of shots, bins, cycles, and output address for mcs
    XMulti_channel_scaler_Set_n_shot(&daq->mcs,  config->n_shot);
    XMulti_channel_scaler_Set_n_bin(&daq->mcs,  config->n_bins);
    XMulti_channel_scaler_Set_n_cycle(&daq->mcs,  config->n_clk);
    XMulti_channel_scaler_Set_data_out(&daq->mcs, DATA_READ_ADDR);

    // enable mcs interrupts
    XMulti_channel_scaler_InterruptEnable(&daq->mcs, 0x1);
    XMulti_channel_scaler_InterruptGlobalEnable(&daq->mcs);

    // verify configuration by reading back registers
    u32 periods = XTrigger_Get_clk_periods(&daq->trigger);
    u32 shots = XMulti_channel_scaler_Get_n_shot(&daq->mcs);

    printf("Readback: periods=%" PRIu32 ", shots=%" PRIu32", bins=%" PRIu32 ", cycles=%" PRIu32 ", output=0x%08" PRIx64 "\n",
           periods, shots, XMulti_channel_scaler_Get_n_bin(&daq->mcs), XMulti_channel_scaler_Get_n_cycle(&daq->mcs), XMulti_channel_scaler_Get_data_out(&daq->mcs));

    if (periods != CLK_PERIODS || shots != config->n_shot ||
        XMulti_channel_scaler_Get_n_bin(&daq->mcs) != config->n_bins ||
        XMulti_channel_scaler_Get_n_cycle(&daq->mcs) != config->n_clk ||
        XMulti_channel_scaler_Get_data_out(&daq->mcs) != DATA_READ_ADDR ||

        XStream_generator_Get_n_bin(&daq->generator) != n_bins ||
        XStream_generator_Get_n_cycle(&daq->generator) != n_clk)     
    {
        fprintf(stderr, "Register readback does not match requested configuration.\n");
        return 1;
    }

    printf("Assuming a %d Hz trigger clock: nominal acquisition %.6f s\n", CLK_FREQ, (double)config->n_shot * CLK_PERIODS / CLK_FREQ);

    daq->configured = 1;

    return 0;
}

int aquire_test(struct daq_device* daq, struct daq_config* config, uint32_t* counts_out) {
    printf("Entered aquire_test function\n");

    struct timespec start, started, end, cpu_start, cpu_end;
    uint32_t enable = 1;
    uint32_t interrupt_count;
    ssize_t bytes;
    int exit_status = 0;

    int n_shot = config->n_shot;
    int n_bins = config->n_bins;
    int n_clk = config->n_clk;

    // initialize test data
    // initialize_testdata(daq->bram_mapping, n_bins, n_clk);
    
    // Disable auto-restart for MCS and wait for it to become idle before starting a new acquisition.
    XMulti_channel_scaler_DisableAutoRestart(&daq->mcs);
    while (!XMulti_channel_scaler_IsIdle(&daq->mcs)) { }

    // enable auto restart for trigger and mcs
    // XTrigger_EnableAutoRestart(&daq->trigger);
    // XMulti_channel_scaler_EnableAutoRestart(&daq->mcs);

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

    // start the acquisition
    // XTrigger_Start(&daq->trigger);
    XTrigger_Set_enable(&daq->trigger, 1);
    XMulti_channel_scaler_Start(&daq->mcs);
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
    // XMulti_channel_scaler_InterruptGlobalDisable(&daq->mcs);
    if (!(pending & 0x1)) {
        fprintf(stderr, "UIO notification without MCS completion.\n");

        goto cleanup;
    }

    // calculate elapsed time and print results
    double time_taken = elapsed(start, end);
    printf("Event completed in %f seconds\n", time_taken);
    printf("Start calls: %.6f s; interrupt wait: %.6f s; process CPU: %.6f s\n", elapsed(start, started), elapsed(started, end), elapsed(cpu_start, cpu_end));
    printf("Effective accepted-shot rate: %.3f shots/s\n", n_shot / time_taken);

    // verify counts against expected values
    volatile uint32_t* bram = daq->bram_mapping;

    int counts_test;
    int id=0;
    for (int i=0; i<n_bins; i++) {
        counts_test = 0;

        counts_test = n_shot * (1 + (n_clk - 1) / (i + 1));
        printf("Bin %d: Expected %d, Read %" PRIu32 "\n", i, counts_test, daq->counts[i]);

        if ((uint32_t)counts_test != daq->counts[i]) {
            exit_status = 1;
        }
    }

    for (int i = 0; i < n_bins; i++) {
        counts_out[i] = daq->counts[i];
    }


cleanup:
    // disable auto-restart for trigger and mcs
    // XTrigger_DisableAutoRestart(&daq->trigger);
    XTrigger_Set_enable(&daq->trigger, 0);
    XMulti_channel_scaler_DisableAutoRestart(&daq->mcs);

    // clear any pending interrupts
    XMulti_channel_scaler_InterruptClear(&daq->mcs,
    XMulti_channel_scaler_InterruptGetStatus(&daq->mcs));
        
    printf("%s\n", exit_status == 0 ? "Run Finished Successfully." : "Run failed.");
    return exit_status;
}