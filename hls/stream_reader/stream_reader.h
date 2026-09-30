#include "hls_stream.h"
#include "ap_int.h"

#include <stdio.h>

#define N_BINS 100
#define N_CLK 10

#define N_TOTAL N_BINS*N_CLK

typedef ap_uint<1> din_t;
typedef hls::stream<din_t> stream_in;

typedef int dout_t;
typedef hls::stream<dout_t> counts_stream;

void stream_reader(stream_in& data_in, dout_t* data_out, int n_bin, int n_clk);