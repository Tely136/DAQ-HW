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

#define DDR_ADDRESS 0x1F000000
#define DDR_SIZE    0x10000

#define DATA_READ_ADDR   DDR_ADDRESS

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

void init_bram(void* bram_mapping, int* data) {
    volatile uint32_t* bram = bram_mapping;
    for (int i=0; i<N_TOTAL; i++) {
        bram[i] = data[i];
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

    void* ddr_mapping = map_device(DDR_ADDRESS, DDR_SIZE);
    if (ddr_mapping == NULL) {
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


    volatile uint32_t* bram = bram_mapping;
    volatile uint32_t* ddr = ddr_mapping;

    int counts_test;
    id=0;
    for (int i=0; i<N_BINS; i++) {
        counts_test = 0;

        for(int j=0; j<N_CLK; j++) {
            counts_test += bram[id];
            id++;
        }

        if (counts_test != ddr[i]) {
            printf("Comparison Failed");
            return 1;
        }
    }

    munmap(bram_mapping, BRAM_SIZE);
    munmap(gen_mapping, GEN_SIZE);
    munmap(reader_mapping, READER_SIZE);
    munmap(ddr_mapping, DDR_SIZE);

    printf("Run Finished Successfully.\n");
    return 0;
}