#include "stream_reader.h"

void stream_reader(stream_in& data_in, dout_t* data_out, int n_outer, int n_inner) {
#pragma HLS INTERFACE mode=s_axilite port=n_inner
#pragma HLS INTERFACE mode=s_axilite port=n_outer
#pragma HLS INTERFACE mode=m_axi port=data_out depth=128
#pragma HLS INTERFACE mode=axis port=data_in
#pragma HLS INTERFACE mode=s_axilite port=return
    
    din_t tmp;
    int count;

    int id = 0;
    
    for (int i=0; i<n_outer; i++) {
        count = 0;
        
        for (int j=0; j<n_inner; j++) {
            count += data_in.read();
            
            // data_out[id] = tmp;

            // id++;
        }

        data_out[i] = count;
    }
}
