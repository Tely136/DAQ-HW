#include <stdint.h>

#define TRIG_ADDRESS 0x40020000
#define TRIG_SIZE   0x10000 /* 64K */

#define GEN_ADDRESS 0x40010000 /* confirm with files, look for way to get this from the hardware description */
#define GEN_SIZE   0x10000 /* 64K */

#define MCS_ADDRESS 0x40000000
#define MCS_SIZE   0x10000 /* 64K */

#define BRAM_ADDRESS 0x42000000
#define BRAM_SIZE    0x2000 /* 8 KiB, from the hardware address map. */

#define DDR_ADDRESS 0x1F000000
#define DDR_SIZE    0x10000

#define DATA_READ_ADDR   DDR_ADDRESS
// #define DATA_READ_ADDR (BRAM_ADDRESS + 4 * N_TOTAL)

#define CLK_FREQ    50000000 // Hz
#define SHOT_FREQ   20
#define CLK_PERIODS (CLK_FREQ / SHOT_FREQ) // number of clock cycles per trigger

#define N_SHOT 20

#define N_BINS   5
#define N_CLK   10 // number of clock cycles per bin
#define N_TOTAL   (N_BINS * N_CLK)  // total number of clock cycles in single shot acquisition

_Static_assert(N_SHOT > 0 && N_BINS > 0 && N_CLK > 0, "Acquisition sizes must be positive");
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