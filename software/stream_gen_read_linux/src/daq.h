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

    
    return 0;
}

void daq_close(struct daq_device* daq) {
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
}

int daq_configure(struct daq_device* daq, struct daq_config* config) {
    XStream_reader_Set_avg_shot(&daq->reader,  config->avg_shot);
    XStream_reader_Set_n_bin(&daq->reader,  config->n_bins);
    XStream_reader_Set_n_cycle(&daq->reader,  config->n_clk);
    XStream_reader_Set_data_out(&daq->reader, DATA_READ_ADDR);

    XStream_generator_Set_n_total(&daq->generator, config->n_bins * config->n_clk);
    XStream_generator_Set_data(&daq->generator, BRAM_ADDRESS);

    return 0;
}