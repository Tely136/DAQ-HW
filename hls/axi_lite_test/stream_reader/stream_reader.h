#include "hls_stream.h"

#define N 50

typedef int din_t;
typedef int dout_t;
typedef hls::stream<din_t> stream_in;

void reader(stream_in& in, dout_t out[N]);