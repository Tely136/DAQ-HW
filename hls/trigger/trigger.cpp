#include "trigger.h"

#include <stdio.h>


static int count=0;

// look at initialization and reset behavior for potentially having default inputs
// without needing to use axi lite

void trigger(int clk_periods, volatile out_t* out) {
#pragma HLS INTERFACE mode=ap_none port=out
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=s_axilite port=clk_periods

    
    for (int i=0; i<clk_periods; i++) {
#pragma HLS PIPELINE II=1
        bool fire = (count == clk_periods - 1);
        if (fire) {
            *out = 1;
        }
        else {
            *out = 0;
        }
        count = fire ? 0 : count + 1;

        // printf("out: %d\r\n",*out);
    }    
}