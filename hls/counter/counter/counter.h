#include "hls_stream.h"
#include "ap_int.h"
#include <iostream>

#define NUM_BINS 10
#define CYCLE_PER_BIN 100;
#define TOTAL_CYCLES NUM_BINS*CYCLE_PER_BIN

typedef int packet;
typedef hls::stream<ap_uint<1>> bitstream;


void example(bitstream& A, ap_uint<16> counts[NUM_BINS]);
