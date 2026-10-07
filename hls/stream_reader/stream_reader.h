#include "hls_stream.h"
#include "ap_int.h"

#include <stdio.h>

#define AVG_SHOT 5

#define N_SHOT 15
#define N_BINS 5
#define N_CLK 10

#define N_TOTAL N_BINS*N_CLK

typedef ap_uint<1> din_t;
typedef hls::stream<din_t> stream_in;

typedef int dout_t;

void stream_reader(stream_in& data_in, dout_t* data_out, int avg_shot, int n_bin, int n_cycle);