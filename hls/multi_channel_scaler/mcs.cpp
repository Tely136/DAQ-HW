#include "mcs.h"
#include <stdio.h>


void multi_channel_scaler(trigger_t& shot_trigger, stream_in& data_in, dout_t* data_out, int n_shot, int n_bin, int n_cycle) {
    #pragma HLS INTERFACE mode=s_axilite port=n_shot
    #pragma HLS INTERFACE mode=s_axilite port=n_bin
    #pragma HLS INTERFACE mode=s_axilite port=n_cycle
    #pragma HLS INTERFACE mode=m_axi port=data_out depth=128
    #pragma HLS INTERFACE mode=s_axilite port=return

    const int N = n_bin * n_cycle;
    
    din_t tmp;
    dout_t accumulated[MAX_BINS];
    int bin, bin_cycle, count;

    for (int shot=0; shot<n_shot; shot++) {

        wait_for_shot:
        while(!shot_trigger.read()) {
            #pragma HLS PIPELINE II=1
        }

        bin = 0;
        bin_cycle = 0;
        count = 0;

        main_loop:
        for (int i=0; i<N; i++) {
            #pragma HLS DEPENDENCE variable=accumulated inter false
            #pragma HLS PIPELINE II=1

            count += data_in.read();

            if (bin_cycle == n_cycle-1) {
                accumulated[bin] = (shot==0) ? count : accumulated[bin] + count;

                bin++;
                bin_cycle = 0;
                count = 0;
            }
            else {
                bin_cycle++;
            }
        }
    }

    for (int bin=0; bin<n_bin; bin++) {
        #pragma HLS PIPELINE II=1
        data_out[bin] = accumulated[bin];
    }
}

