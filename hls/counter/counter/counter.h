#include "hls_stream.h"
#include "ap_int.h"
#include <iostream>

#define MAX_BINS 10000

#define NUM_BINS 100
#define CYCLE_PER_BIN 50
// #define TOTAL_CYCLES NUM_BINS*CYCLE_PER_BIN

typedef int packet;
typedef hls::stream<ap_uint<1>> bitstream;


void MCS(bitstream& A, ap_uint<32> counts[MAX_BINS], int num_bins, int cycle_per_bin);
