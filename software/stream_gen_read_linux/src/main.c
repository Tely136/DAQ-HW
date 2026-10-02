#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include "xstream_generator.h"
#include "xstream_reader.h"

#define GEN_ADDRESS 0x40010000
#define GEN_SIZE   0x1000 /* 4 KiB, from the hardware address map. */

#define READER_ADDRESS 0x40030000
#define READER_SIZE   0x1000

#define BRAM_ADDRESS 0x40002000
#define BRAM_SIZE    0x2000 /* 8 KiB, from the hardware address map. */

#define DATA_READ_ADDR   (BRAM_ADDRESS + N_BINS * sizeof(uint32_t))

#define N_BINS   5
#define N_CLK   10
#define N_TOTAL   (N_BINS * N_CLK)

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

void init_bram(void* bram_mapping) {
    volatile uint32_t* bram = bram_mapping;
    for (int i=0; i<N_BINS; i++) {
        bram[i] = i+1;
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

    void* bram_mapping = map_device(BRAM_ADDRESS, BRAM_SIZE);
    if (bram_mapping == NULL) {
        return 1;
    }
    init_bram(bram_mapping);

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

    XStream_generator gen = {
        .Control_BaseAddress = (u64)(uintptr_t) gen_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };

    XStream_reader reader = {
        .Control_BaseAddress = (u64)(uintptr_t) reader_mapping,
        .IsReady = XIL_COMPONENT_IS_READY
    };


    /* Read the control/status register once, then decode it. */
    u32 gen_status = XStream_generator_ReadReg(
        gen.Control_BaseAddress, 
        XSTREAM_GENERATOR_CONTROL_ADDR_AP_CTRL
    );

    u32 reader_status = XStream_reader_ReadReg(
        reader.Control_BaseAddress,
        XSTREAM_READER_CONTROL_ADDR_AP_CTRL
    );

    /* Print the status of both components. */
    // print_status(gen_status);
    // print_status(reader_status);

    /* Set input parameters. This does not start the generator. */
    XStream_generator_Set_n_total(&gen, N_TOTAL);
    XStream_generator_Set_data(&gen,    BRAM_ADDRESS);

    XStream_reader_Set_n_outer(&reader,  N_BINS);
    XStream_reader_Set_n_inner(&reader,  N_CLK);
    XStream_reader_Set_data_out(&reader, DATA_READ_ADDR);

    XStream_generator_Start(&gen);
    XStream_reader_Start(&reader);

    while (!XStream_generator_IsDone(&gen)) {}
    while (!XStream_reader_IsDone(&reader)) {}

    for (int i=0; i<N_BINS; i++) {
        volatile uint32_t* bram = bram_mapping;
        uint32_t value = bram[i]; // Read from the output location in BRAM
        printf("BRAM[%d] = %u\n", i, value);
    }

    munmap(bram_mapping, BRAM_SIZE);
    munmap(gen_mapping, GEN_SIZE);
    munmap(reader_mapping, READER_SIZE);

    printf("Run Finished.\n");
    return 0;
}