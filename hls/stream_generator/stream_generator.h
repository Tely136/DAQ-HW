#include "hls_stream.h"
#include "ap_int.h"
#include "hls_directio.h"

#define N_BINS 5
#define N_CLK 10
#define N_TOTAL N_BINS*N_CLK

typedef ap_uint<1> dout_t;
typedef hls::ap_none<dout_t> stream_out;

typedef hls::ap_none<ap_uint<1>> generator_trigger_t;

void stream_generator(generator_trigger_t& shot_trigger, stream_out& out, int n_bin, int n_cycle);