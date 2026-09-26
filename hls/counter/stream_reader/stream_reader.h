#include "hls_stream.h"
// #include "ap_int.h"

#include <stdio.h>

#define N_OUTER 5
#define N_INNER 10

#define N_TOTAL N_OUTER*N_INNER

typedef int din_t;
typedef hls::stream<din_t> stream_in;

typedef int dout_t;


void stream_reader(stream_in& data_in, dout_t* data_out, int n_outer, int n_inner);