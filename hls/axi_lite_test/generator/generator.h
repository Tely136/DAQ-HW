#include "hls_stream.h"

#define N 50


typedef int dout_t;
typedef hls::stream<dout_t> out_stream;

void generator(out_stream& out);
