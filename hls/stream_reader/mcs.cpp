#include <stdio.h>

#include "mcs.h"


void multi_channel_scaler(trigger_t& shot_trigger, stream_in& data_in, dout_t* data_out, int avg_shot, int n_bin, int n_cycle) {
    // #pragma HLS INTERFACE mode=ap_none port=shot_trigger
    #pragma HLS INTERFACE mode=s_axilite port=avg_shot
    #pragma HLS INTERFACE mode=s_axilite port=n_bin
    #pragma HLS INTERFACE mode=s_axilite port=n_cycle
    #pragma HLS INTERFACE mode=m_axi port=data_out depth=128
    #pragma HLS INTERFACE mode=axis port=data_in
    #pragma HLS INTERFACE mode=s_axilite port=return
    
    din_t tmp;
    dout_t accumulated[MAX_BINS];
    int count;

    int id = 0;

    for (int shot=0; shot<avg_shot; shot++) {

        wait_for_shot:
        while(!shot_trigger.read()) {
            #pragma HLS PIPELINE II=1
        }
        

        bin_loop:
        for (int bin=0; bin<n_bin; bin++) {
            count = 0;
            
            cycle_loop:
            for (int cycle=0; cycle<n_cycle; cycle++) {
                #pragma HLS PIPELINE II=1
                count += data_in.read();
            }

            // look into how tasks or dataflow could improve this 
            accumulated[bin] = (shot==0) ? count : accumulated[bin] + count;
        }
    }

    for (int bin=0; bin<n_bin; bin++) {
        #pragma HLS PIPELINE II=1
        data_out[bin] = accumulated[bin];
    }
}

