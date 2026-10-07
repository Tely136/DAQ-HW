#include "stream_reader.h"
#include <stdio.h>

static int shot_counter = 0;

void stream_reader(stream_in& data_in, dout_t* data_out, int avg_shot, int n_bin, int n_cycle) {
#pragma HLS INTERFACE mode=s_axilite port=avg_shot
#pragma HLS INTERFACE mode=s_axilite port=n_bin
#pragma HLS INTERFACE mode=s_axilite port=n_cycle
#pragma HLS INTERFACE mode=m_axi port=data_out depth=128
#pragma HLS INTERFACE mode=axis port=data_in
#pragma HLS INTERFACE mode=ap_ctrl_hs port=return
    
    din_t tmp;
    int count;

    int id = 0;

    // printf("shot: %d\r\n", shot_counter);

    for (int i=0; i<n_bin; i++) {
    count = 0;
        
        for (int j=0; j<n_cycle; j++) {
            #pragma HLS PIPELINE II=1
            count += data_in.read();
        }

        // look into how tasks or dataflow could improve this 
        data_out[i] = (shot_counter==0) ? count : data_out[i] + count;
    }
    shot_counter = (shot_counter==avg_shot-1) ? 0 : shot_counter+1;

}

