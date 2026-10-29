#include "trigger.h"

void trigger(period_port_t& clk_periods, enable_port_t& enable, trigger_out_t& out) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=s_axilite port=clk_periods bundle=control
#pragma HLS INTERFACE mode=s_axilite port=enable bundle=control

    static ap_uint<32> count = 0;
#pragma HLS RESET variable=count

    // Configure the period while disabled. Periods 0 and 1 disable output.
    // In hardware this loop never exits: there is no start/autorestart control.
    run:
    while (true) {
#pragma HLS PIPELINE II=1
#ifndef __SYNTHESIS__
        // C simulation only: consume the finite sequence queued by the test.
        // These queues model I/O operations, not independently timed clocks.
        // RTL verification needs a clocked testbench for this free-running IP.
        if (clk_periods.size() == 0 || enable.size() == 0)
            break;
#endif
        const ap_uint<32> period = clk_periods.read();
        const bool enabled = enable.read();
        out_t pulse = 0;

        if (!enabled || period < 2) {
            count = 0;
        } else if (count == period - 1) {
            count = 0;
            pulse = 1;
        } else {
            ++count;
        }

        // Write zero explicitly on every non-pulse iteration.
        out.write(pulse);
    }
}
