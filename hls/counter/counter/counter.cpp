#include "counter.h"

void counter(stream_in& data_in, dout_t* data_out, int n_outer, int n_inner) {
#pragma HLS INTERFACE mode=s_axilite port=n_inner
#pragma HLS INTERFACE mode=s_axilite port=n_outer
#pragma HLS INTERFACE mode=m_axi port=data_out
#pragma HLS INTERFACE mode=axis port=data_in
#pragma HLS INTERFACE mode=s_axilite port=return
    
    int i,j;
    din_t tmp;

    int id = 0;
    for (i=0; i<n_outer; i++) {
        for (j=0; j<n_inner; j++) {
            tmp = data_in.read();
            
            data_out[id] = tmp;

            id++;
        }
    }
}
