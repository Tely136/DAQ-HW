#include "stream_generator.h"

void stream_generator(stream_out& out, int n_total, dout_t* data) {
#pragma HLS INTERFACE mode=m_axi port=data depth=128
#pragma HLS INTERFACE mode=s_axilite port=n_total
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=axis port=out
    int i,j;
    int tmp;
    
    for (i=0; i<n_total; i++) {
        out.write(data[i]);
    }
}