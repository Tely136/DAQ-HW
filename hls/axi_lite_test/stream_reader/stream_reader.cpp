#include "stream_reader.h"


void reader(stream_in& in, dout_t out[N]) {
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=axis port=in
#pragma HLS INTERFACE mode=m_axi port=out
    int tmp;
    
    for (int i=0; i<N; i++) {
        tmp = in.read();

        out[i] = tmp;
    }
}