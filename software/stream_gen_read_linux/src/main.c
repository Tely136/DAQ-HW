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

#include "params.h"
#include "daq.h"
#include "daq_tcpip.h"


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

int main() {
    struct daq_device daq;
    struct daq_config config = {
        .n_bins = N_BINS,
        .n_clk = N_CLK,
        .avg_shot = AVG_SHOT
    };  

    // setup code here
    daq_open(&daq);

    int id;
    uint32_t interrupt_count;
    struct timespec start, started, end, cpu_start, cpu_end;

    /* Autorestart persists after the previous application exits. */
    int trigger_was_running = !XTrigger_IsIdle(&daq.trigger);

    /* Set input parameters. This does not start an idle generator. */
    XTrigger_Set_clk_periods(&daq.trigger, CLK_PERIODS);
    XTrigger_EnableAutoRestart(&daq.trigger);
    // XTrigger_DisableAutoRestart(&daq.trigger);

    /* Do not reconfigure an acquisition left running by an earlier process. */
    if (!XStream_reader_IsIdle(&daq.reader)) {
        fprintf(stderr, "Reader is still running; wait for it to finish before retrying.\n");
        return 1;
    }
    XStream_reader_DisableAutoRestart(&daq.reader);

    /* ISR is toggle-on-write: clear only bits which are already set.
     * Opening UIO afterwards starts with a fresh notification count.
     */
    XStream_reader_InterruptGlobalDisable(&daq.reader);
    XStream_reader_InterruptDisable(&daq.reader, 0x3);
    XStream_reader_InterruptClear(&daq.reader, XStream_reader_InterruptGetStatus(&daq.reader));
    (void)XStream_reader_InterruptGetStatus(&daq.reader); /* Flush the clear. */

    int irq_fd = open("/dev/uio0", O_RDWR);
    if (irq_fd < 0) {
        perror("open /dev/uio0");
        return 1;
    }
    int exit_status = 1;


    // wait for client to connect and return socket file descriptor
    int sockfd = tcp_listen(atoi(PORT));
    if (sockfd < 0) {
        goto cleanup; // need to look at this
    }


    daq_configure(&daq, &config);
    
    /* Verify the actual AXI-Lite settings before timing the acquisition. */
    u32 periods = XTrigger_Get_clk_periods(&daq.trigger);
    u32 shots = XStream_reader_Get_avg_shot(&daq.reader);
    printf("Build: %s %s; trigger was %s\n", __DATE__, __TIME__,
           trigger_was_running ? "already running" : "idle");
    printf("Readback: periods=%" PRIu32 ", shots=%" PRIu32
           ", bins=%" PRIu32 ", cycles=%" PRIu32 ", output=0x%08" PRIx64 "\n",
           periods, shots, XStream_reader_Get_n_bin(&daq.reader),
           XStream_reader_Get_n_cycle(&daq.reader), XStream_reader_Get_data_out(&daq.reader));
    if (periods != CLK_PERIODS || shots != AVG_SHOT ||
        XStream_reader_Get_n_bin(&daq.reader) != N_BINS ||
        XStream_reader_Get_n_cycle(&daq.reader) != N_CLK ||
        XStream_reader_Get_data_out(&daq.reader) != DATA_READ_ADDR ||
        XStream_generator_Get_n_total(&daq.generator) != N_TOTAL ||
        XStream_generator_Get_data(&daq.generator) != BRAM_ADDRESS) {
        fprintf(stderr, "Register readback does not match requested configuration.\n");
        goto cleanup;
    }

    printf("Assuming a %d Hz trigger clock: nominal acquisition %.6f s\n",
           CLK_FREQ, (double)AVG_SHOT * CLK_PERIODS / CLK_FREQ);
    fflush(stdout); /* Console output is outside the timed interval. */

    XStream_reader_InterruptEnable(&daq.reader, 0x1);  // ap_done
    XStream_reader_InterruptGlobalEnable(&daq.reader);

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

    XStream_reader_Start(&daq.reader);
    XTrigger_Start(&daq.trigger);
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

    u32 pending = XStream_reader_InterruptGetStatus(&daq.reader);
    XStream_reader_InterruptClear(&daq.reader, pending);
    XStream_reader_InterruptGlobalDisable(&daq.reader);
    if (!(pending & 0x1)) {
        fprintf(stderr, "UIO notification without reader completion.\n");
        goto cleanup;
    }

    double time_taken = elapsed(start, end);
    printf("Event completed in %f seconds\n", time_taken);
    printf("Start calls: %.6f s; interrupt wait: %.6f s; process CPU: %.6f s\n",
           elapsed(start, started), elapsed(started, end), elapsed(cpu_start, cpu_end));
    printf("Effective accepted-shot rate: %.3f shots/s\n", AVG_SHOT / time_taken);

    volatile uint32_t* bram = daq.bram_mapping;

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
        printf("Bin %d: Expected %d, Read %" PRIu32 "\n", i, counts_test, daq.counts[i]);

        if ((uint32_t)counts_test != daq.counts[i]) {
            exit_status = 1;
        }
    }

    uint32_t counts_copy[N_BINS];
    for (int i = 0; i < N_BINS; i++) {
        counts_copy[i] = daq.counts[i];
    }

cleanup:
    /* This program performs one acquisition. The next run re-arms UIO. */
    XStream_reader_InterruptGlobalDisable(&daq.reader);
    XStream_reader_InterruptDisable(&daq.reader, 0x3);
    XStream_reader_InterruptClear(&daq.reader,
        XStream_reader_InterruptGetStatus(&daq.reader));
        
    close(irq_fd);

    daq_close(&daq);

    printf("%s\n", exit_status == 0 ? "Run Finished Successfully." : "Run failed.");
    return exit_status;
}
