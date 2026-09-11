#include "pulse_generator.h"

static int count;

void pulse_generator(int div[N_BINS], int n_reps, ap_uint<1>& out) {
#pragma HLS INTERFACE mode=bram port=div storage_type=rom_1p
#pragma HLS INTERFACE mode=ap_none port=out
#pragma HLS INTERFACE mode=ap_none port=n_reps

    for(int j=0; j<N_BINS; j++) {
        count = 1;
        for(int i=0; i<n_reps; i++) {
            if (count==div[j]) {
                out=1;
                count=1;
            }
            else {
                out=0;
                count++;
            }

            // printf("%d \n",out);
        }
    }
}