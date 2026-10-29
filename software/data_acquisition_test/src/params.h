#ifndef DAQ_PARAMS_H
#define DAQ_PARAMS_H

#include <stdint.h>

#define TRIGGER_ADDRESS 0x40020000
#define TRIGGER_SIZE    0x10000

#define SOURCE_ADDRESS  0x40010000
#define SOURCE_SIZE     0x10000

#define MCS_ADDRESS     0x40000000
#define MCS_SIZE        0x10000

// reserved DDR output buffer
#define COUNTS_ADDRESS  0x1F000000
#define COUNTS_SIZE     0x10000

#define CLOCK_HZ        50000000

// match the accumulation array in mcs.h
#define MAX_BINS 128

_Static_assert(MAX_BINS * sizeof(uint32_t) <= COUNTS_SIZE, "Output exceeds buffer");

#endif