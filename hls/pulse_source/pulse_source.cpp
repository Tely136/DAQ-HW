#include "pulse_source.h"


void pulse_source(source_period_t& clk_periods,
                  source_enable_t& enable, source_out_t& out) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=s_axilite port=clk_periods bundle=control
#pragma HLS INTERFACE mode=s_axilite port=enable bundle=control

    static ap_uint<32> count = 0;
#pragma HLS RESET variable=count

    // change the period while disabled
    // period 0 gives all zeros, period 1 gives all ones
    run:
    while (true) {
        #pragma HLS PIPELINE II=1

#ifndef __SYNTHESIS__
        // stop when the queued C test data has been consumed
        // hardware runs continuously
        if (clk_periods.size() == 0 || enable.size() == 0) {
            break;
        }
#endif

        ap_uint<32> period = clk_periods.read();
        bool enabled = enable.read();
        ap_uint<1> pulse = 0;

        if (!enabled || period == 0) {
            count = 0;
        }
        else if (count == period - 1) {
            count = 0;
            pulse = 1;
        }
        else {
            count++;
        }

        out.write(pulse);
    }
}