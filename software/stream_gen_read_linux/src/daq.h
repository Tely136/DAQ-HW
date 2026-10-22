#include <stdint.h>


#include "params.h"

#include "xtrigger.h"
#include "xstream_generator.h"
#include "xstream_reader.h"

struct daq_device {
    volatile uint32_t* counts;

    void* bram_mapping;
    void* ddr_mapping;
    void* trig_mapping;
    void* gen_mapping;
    void* reader_mapping;

    XTrigger trigger;
    XStream_generator generator;
    XStream_reader reader;
};

struct daq_config {
    uint32_t n_bins;
    uint32_t n_clk;
    uint32_t avg_shot;
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

void init_bram(void* bram_mapping, int* data) {
    volatile uint32_t* bram = bram_mapping;
    for (int i=0; i<N_TOTAL; i++) {
        bram[i] = data[i];
    }
}

void initialize_testdata(void* memory_map) {
    int data[N_TOTAL];
    int id = 0;
    for (int i=0; i<N_BINS; i++) {
        for (int j=0; j<N_CLK; j++) {
            data[id] = (j % (i + 1) == 0);
            id++;
        }
    }
    init_bram(memory_map, data);
}

int daq_open(struct daq_device* daq) {

    daq->bram_mapping = map_device(BRAM_ADDRESS, BRAM_SIZE);
    if (daq->bram_mapping == NULL) {
        return 1;
    }

    /* Map the trigger's AXI-Lite control registers. */
    daq->trig_mapping = map_device(TRIG_ADDRESS, TRIG_SIZE);
    if (daq->trig_mapping == NULL) {
        return 1;
    }

    XTrigger trigger = {
        .Control_BaseAddress = (u64)(uintptr_t) daq->trig_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };
    daq->trigger = trigger;

    /* Map the generator's AXI-Lite control registers. */
    daq->gen_mapping = map_device(GEN_ADDRESS, GEN_SIZE);
    if (daq->gen_mapping == NULL) {
        return 1;
    }

    XStream_generator generator = {
        .Control_BaseAddress = (u64)(uintptr_t) daq->gen_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };
    daq->generator = generator;

    /* Map the reader's AXI-Lite control registers. */
    daq->reader_mapping = map_device(READER_ADDRESS, READER_SIZE);
    if (daq->reader_mapping == NULL) {
        return 1;
    }

    XStream_reader reader = {
        .Control_BaseAddress = (u64)(uintptr_t) daq->reader_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };
    daq->reader = reader;

    initialize_testdata(daq->bram_mapping);

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

    /* ISR is toggle-on-write: clear only bits which are already set.
     * Opening UIO afterwards starts with a fresh notification count.
     */
    XStream_reader_InterruptGlobalDisable(&daq->reader);
    XStream_reader_InterruptDisable(&daq->reader, 0x3);
    XStream_reader_InterruptClear(&daq->reader, XStream_reader_InterruptGetStatus(&daq->reader));
    (void)XStream_reader_InterruptGetStatus(&daq->reader); /* Flush the clear. */

    int irq_fd = open("/dev/uio0", O_RDWR);
    if (irq_fd < 0) {
        perror("open /dev/uio0");
        return 1;
    }

    
    return 0;
}

int daq_close(struct daq_device* daq) {
    if (daq->bram_mapping) {
        munmap(daq->bram_mapping, BRAM_SIZE);
    }
    if (daq->trig_mapping) {
        munmap(daq->trig_mapping, TRIG_SIZE);
    }
    if (daq->gen_mapping) {
        munmap(daq->gen_mapping, GEN_SIZE);
    }
    if (daq->reader_mapping) {
        munmap(daq->reader_mapping, READER_SIZE);
    }
#if DATA_READ_ADDR == DDR_ADDRESS
    if (daq->ddr_mapping) {
        munmap(daq->ddr_mapping, DDR_SIZE);
    }
#endif  

    return 0;
}

int daq_configure(struct daq_device* daq, struct daq_config* config) {
    XTrigger_Set_clk_periods(&daq->trigger, CLK_PERIODS);

    XStream_generator_Set_n_total(&daq->generator, config->n_bins * config->n_clk);
    XStream_generator_Set_data(&daq->generator, BRAM_ADDRESS);

    XStream_reader_Set_avg_shot(&daq->reader,  config->avg_shot);
    XStream_reader_Set_n_bin(&daq->reader,  config->n_bins);
    XStream_reader_Set_n_cycle(&daq->reader,  config->n_clk);
    XStream_reader_Set_data_out(&daq->reader, DATA_READ_ADDR);

    /* Verify the actual AXI-Lite settings before timing the acquisition. */
    u32 periods = XTrigger_Get_clk_periods(&daq->trigger);
    u32 shots = XStream_reader_Get_avg_shot(&daq->reader);

    printf("Readback: periods=%" PRIu32 ", shots=%" PRIu32", bins=%" PRIu32 ", cycles=%" PRIu32 ", output=0x%08" PRIx64 "\n",
           periods, shots, XStream_reader_Get_n_bin(&daq->reader), XStream_reader_Get_n_cycle(&daq->reader), XStream_reader_Get_data_out(&daq->reader));

    if (periods != CLK_PERIODS || shots != AVG_SHOT ||
        XStream_reader_Get_n_bin(&daq->reader) != N_BINS ||
        XStream_reader_Get_n_cycle(&daq->reader) != N_CLK ||
        XStream_reader_Get_data_out(&daq->reader) != DATA_READ_ADDR ||
        XStream_generator_Get_n_total(&daq->generator) != N_TOTAL ||
        XStream_generator_Get_data(&daq->generator) != BRAM_ADDRESS) {
        fprintf(stderr, "Register readback does not match requested configuration.\n");
        return 1;
    }

    printf("Assuming a %d Hz trigger clock: nominal acquisition %.6f s\n", CLK_FREQ, (double)AVG_SHOT * CLK_PERIODS / CLK_FREQ);

    return 0;
}


int aquire_test(struct daq_device* daq) {

    XTrigger_EnableAutoRestart(&daq->trigger);

    struct timespec start, started, end, cpu_start, cpu_end;
    uint32_t enable = 1;
    if (write(irq_fd, &enable, sizeof(enable)) != sizeof(enable)) {
        perror("enable UIO interrupt");
        goto cleanup;
    }

    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_start) != 0 ||
        clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        perror("clock_gettime");
        goto cleanup;
    }

    XTrigger_Start(&daq->trigger);
    XStream_reader_Start(&daq->reader);
    if (clock_gettime(CLOCK_MONOTONIC, &started) != 0) {
        perror("clock_gettime");
        goto cleanup;
    }

    /* Blocking read sleeps until UIO delivers the completion interrupt.
     * No register polling and no timeout; EINTR means a Linux signal arrived.
     */
    uint32_t interrupt_count;
    ssize_t bytes;
    do {
        bytes = read(irq_fd, &interrupt_count, sizeof(interrupt_count));
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


    /* Timestamp immediately after waking, before reading any IP registers. */
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0 ||
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_end) != 0) {
        perror("clock_gettime");
        goto cleanup;
    }

    u32 pending = XStream_reader_InterruptGetStatus(&daq->reader);
    XStream_reader_InterruptClear(&daq->reader, pending);
    XStream_reader_InterruptGlobalDisable(&daq->reader);
    if (!(pending & 0x1)) {
        fprintf(stderr, "UIO notification without reader completion.\n");
        goto cleanup;
    }

    double time_taken = elapsed(start, end);
    printf("Event completed in %f seconds\n", time_taken);
    printf("Start calls: %.6f s; interrupt wait: %.6f s; process CPU: %.6f s\n",
           elapsed(start, started), elapsed(started, end), elapsed(cpu_start, cpu_end));
    printf("Effective accepted-shot rate: %.3f shots/s\n", AVG_SHOT / time_taken);

    volatile uint32_t* bram = daq->bram_mapping;

    exit_status = 0;
    int counts_test;
    int id=0;
    for (int i=0; i<N_BINS; i++) {
        counts_test = 0;

        for(int j=0; j<N_CLK; j++) {
            counts_test += bram[id];
            id++;
        }

        counts_test *= AVG_SHOT;
        printf("Bin %d: Expected %d, Read %" PRIu32 "\n", i, counts_test, daq->counts[i]);

        if ((uint32_t)counts_test != daq->counts[i]) {
            exit_status = 1;
        }
    }

    uint32_t counts_copy[N_BINS];
    for (int i = 0; i < N_BINS; i++) {
        counts_copy[i] = daq->counts[i];
    }

    return 0;
}