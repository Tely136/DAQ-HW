#include "hls_stream.h"
#include "ap_int.h"

#define N_BINS 5
#define N_CLK 10
#define N_TOTAL N_BINS*N_CLK

typedef ap_uint<1> dout_t;
typedef hls::stream<dout_t> stream_out;

void stream_generator(stream_out& out, int n_total, int* data);