#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include "xtrigger.h"
#include "xstream_generator.h"
#include "xstream_reader.h"

#define TRIG_ADDRESS 0x40040000
#define TRIG_SIZE   0x1000

#define GEN_ADDRESS 0x40010000 /* confirm with files, look for way to get this from the hardware description */
#define GEN_SIZE   0x1000 /* 4 KiB, from the hardware address map. check the hardware description for the correct size */

#define READER_ADDRESS 0x40030000
#define READER_SIZE   0x1000

#define BRAM_ADDRESS 0x40002000
#define BRAM_SIZE    0x2000 /* 8 KiB, from the hardware address map. */

#define DDR_ADDRESS 0x1F000000
#define DDR_SIZE    0x10000

#define DATA_READ_ADDR   DDR_ADDRESS
// #define DATA_READ_ADDR (BRAM_ADDRESS + 4 * N_TOTAL)

#define CLK_FREQ    50000000 // Hz
#define SHOT_FREQ   20
#define CLK_PERIODS (CLK_FREQ / SHOT_FREQ)

#define AVG_SHOT 1200

#define N_BINS   5
#define N_CLK   10
#define N_TOTAL   (N_BINS * N_CLK)

_Static_assert(AVG_SHOT > 0 && N_BINS > 0 && N_CLK > 0, "Acquisition sizes must be positive");
_Static_assert(SHOT_FREQ > 0 && CLK_PERIODS > 0, "Invalid trigger frequency");
_Static_assert(N_TOTAL * sizeof(uint32_t) <= BRAM_SIZE, "Input exceeds BRAM");
#if DATA_READ_ADDR == DDR_ADDRESS
_Static_assert(N_BINS * sizeof(uint32_t) <= DDR_SIZE, "Output exceeds DDR buffer");
#else
_Static_assert(DATA_READ_ADDR >= BRAM_ADDRESS + N_TOTAL * sizeof(uint32_t),
               "Output overlaps generator input");
_Static_assert(DATA_READ_ADDR + N_BINS * sizeof(uint32_t) <= BRAM_ADDRESS + BRAM_SIZE,
               "Output exceeds BRAM");
#endif

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

void init_bram(void* bram_mapping, int* data) {
    volatile uint32_t* bram = bram_mapping;
    for (int i=0; i<N_TOTAL; i++) {
        bram[i] = data[i];
    }
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

int main() {
    uint32_t interrupt_count;
    struct timespec start, started, end, cpu_start, cpu_end;

    int data[N_TOTAL];
    int id = 0;
    for (int i=0; i<N_BINS; i++) {
        for (int j=0; j<N_CLK; j++) {
            data[id] = (j % (i + 1) == 0);
            id++;
        }
    }


    void* bram_mapping = map_device(BRAM_ADDRESS, BRAM_SIZE);
    if (bram_mapping == NULL) {
        return 1;
    }
    init_bram(bram_mapping, data);

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

    /* Map the trigger's AXI-Lite control registers. */
    void* trig_mapping = map_device(TRIG_ADDRESS, TRIG_SIZE);
    if (trig_mapping == NULL) {
        return 1;
    }

    /* Map the generator's AXI-Lite control registers. */
    void* gen_mapping = map_device(GEN_ADDRESS, GEN_SIZE);
    if (gen_mapping == NULL) {
        return 1;
    }

    /* Map the reader's AXI-Lite control registers. */
    void* reader_mapping = map_device(READER_ADDRESS, READER_SIZE);
    if (reader_mapping == NULL) {
        return 1;
    }

    XTrigger trig = {
        .Control_BaseAddress = (u64)(uintptr_t) trig_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };

    XStream_generator gen = {
        .Control_BaseAddress = (u64)(uintptr_t) gen_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };

    XStream_reader reader = {
        .Control_BaseAddress = (u64)(uintptr_t) reader_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };


    /* Do not reconfigure an acquisition left running by an earlier process. */
    if (!XStream_reader_IsIdle(&reader)) {
        fprintf(stderr, "Reader is still running; wait for it to finish before retrying.\n");
        return 1;
    }
    XStream_reader_DisableAutoRestart(&reader);

    /* ISR is toggle-on-write: clear only bits which are already set.
     * Opening UIO afterwards starts with a fresh notification count.
     */
    XStream_reader_InterruptGlobalDisable(&reader);
    XStream_reader_InterruptDisable(&reader, 0x3);
    XStream_reader_InterruptClear(&reader,
        XStream_reader_InterruptGetStatus(&reader));
    (void)XStream_reader_InterruptGetStatus(&reader); /* Flush the clear. */

    int irq_fd = open("/dev/uio0", O_RDWR);
    if (irq_fd < 0) {
        perror("open /dev/uio0");
        return 1;
    }
    int exit_status = 1;

    /* Autorestart persists after the previous application exits. */
    int trigger_was_running = !XTrigger_IsIdle(&trig);

    /* Set input parameters. This does not start an idle generator. */
    XTrigger_Set_clk_periods(&trig, CLK_PERIODS);
    XTrigger_EnableAutoRestart(&trig);
    // XTrigger_DisableAutoRestart(&trig);

    XStream_generator_Set_n_total(&gen, N_TOTAL);
    XStream_generator_Set_data(&gen,    BRAM_ADDRESS);

    XStream_reader_Set_avg_shot(&reader,  AVG_SHOT);
    XStream_reader_Set_n_bin(&reader,  N_BINS);
    XStream_reader_Set_n_cycle(&reader,  N_CLK);
    XStream_reader_Set_data_out(&reader, DATA_READ_ADDR);

    /* Verify the actual AXI-Lite settings before timing the acquisition. */
    u32 periods = XTrigger_Get_clk_periods(&trig);
    u32 shots = XStream_reader_Get_avg_shot(&reader);
    printf("Build: %s %s; trigger was %s\n", __DATE__, __TIME__,
           trigger_was_running ? "already running" : "idle");
    printf("Readback: periods=%" PRIu32 ", shots=%" PRIu32
           ", bins=%" PRIu32 ", cycles=%" PRIu32 ", output=0x%08" PRIx64 "\n",
           periods, shots, XStream_reader_Get_n_bin(&reader),
           XStream_reader_Get_n_cycle(&reader), XStream_reader_Get_data_out(&reader));
    if (periods != CLK_PERIODS || shots != AVG_SHOT ||
        XStream_reader_Get_n_bin(&reader) != N_BINS ||
        XStream_reader_Get_n_cycle(&reader) != N_CLK ||
        XStream_reader_Get_data_out(&reader) != DATA_READ_ADDR ||
        XStream_generator_Get_n_total(&gen) != N_TOTAL ||
        XStream_generator_Get_data(&gen) != BRAM_ADDRESS) {
        fprintf(stderr, "Register readback does not match requested configuration.\n");
        goto cleanup;
    }

    printf("Assuming a %d Hz trigger clock: nominal acquisition %.6f s\n",
           CLK_FREQ, (double)AVG_SHOT * CLK_PERIODS / CLK_FREQ);
    fflush(stdout); /* Console output is outside the timed interval. */

    XStream_reader_InterruptEnable(&reader, 0x1);  // ap_done
    XStream_reader_InterruptGlobalEnable(&reader);

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

    XStream_reader_Start(&reader);
    XTrigger_Start(&trig);
    if (clock_gettime(CLOCK_MONOTONIC, &started) != 0) {
        perror("clock_gettime");
        goto cleanup;
    }

    /* Blocking read sleeps until UIO delivers the completion interrupt.
     * No register polling and no timeout; EINTR means a Linux signal arrived.
     */
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

    u32 pending = XStream_reader_InterruptGetStatus(&reader);
    XStream_reader_InterruptClear(&reader, pending);
    XStream_reader_InterruptGlobalDisable(&reader);
    if (!(pending & 0x1)) {
        fprintf(stderr, "UIO notification without reader completion.\n");
        goto cleanup;
    }

    double time_taken = elapsed(start, end);
    printf("Event completed in %f seconds\n", time_taken);
    printf("Start calls: %.6f s; interrupt wait: %.6f s; process CPU: %.6f s\n",
           elapsed(start, started), elapsed(started, end), elapsed(cpu_start, cpu_end));
    printf("Effective accepted-shot rate: %.3f shots/s\n", AVG_SHOT / time_taken);

    volatile uint32_t* bram = bram_mapping;

    exit_status = 0;
    int counts_test;
    id=0;
    for (int i=0; i<N_BINS; i++) {
        counts_test = 0;

        for(int j=0; j<N_CLK; j++) {
            counts_test += bram[id];
            id++;
        }

        counts_test *= AVG_SHOT;
        printf("Bin %d: Expected %d, Read %" PRIu32 "\n", i, counts_test, counts[i]);

        if ((uint32_t)counts_test != counts[i]) {
            exit_status = 1;
        }
    }

cleanup:
    /* This program performs one acquisition. The next run re-arms UIO. */
    XStream_reader_InterruptGlobalDisable(&reader);
    XStream_reader_InterruptDisable(&reader, 0x3);
    XStream_reader_InterruptClear(&reader,
        XStream_reader_InterruptGetStatus(&reader));
    close(irq_fd);

    munmap(bram_mapping, BRAM_SIZE);
    munmap(trig_mapping, TRIG_SIZE);
    munmap(gen_mapping, GEN_SIZE);
    munmap(reader_mapping, READER_SIZE);
#if DATA_READ_ADDR == DDR_ADDRESS
    munmap(ddr_mapping, DDR_SIZE);
#endif

    printf("%s\n", exit_status == 0 ? "Run Finished Successfully." : "Run failed.");
    return exit_status;
}
