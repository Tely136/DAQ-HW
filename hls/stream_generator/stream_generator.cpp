#include "stream_generator.h"


void stream_generator(generator_trigger_t& shot_trigger, stream_out& out, int n_bin, int n_cycle) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=s_axilite port=n_bin bundle=control
#pragma HLS INTERFACE mode=s_axilite port=n_cycle bundle=control

    // Configure n_bin/n_cycle before triggering; keep them stable during a shot.
    // The ap_none type supplies a plain trigger input, without handshake ports.
    wait_for_trigger:
    while (!shot_trigger.read()) {
        #pragma HLS PIPELINE II=1
        out.write(0);
    }

    int remaining = 0;

    send_samples:
    for (int i = 0; i < n_bin; i++) {
        for (int j = 0; j < n_cycle; j++) {
            #pragma HLS PIPELINE II=1
            #pragma HLS LOOP_FLATTEN

            // restart the pattern at the beginning of each bin
            int current = (j == 0) ? 0 : remaining;
            bool pulse = (current == 0);

            out.write(pulse);
            remaining = pulse ? i : current - 1;
        }
    }

    out.write(0);

    // Rearm only after low is observed. Pulses wholly inside send_samples
    // are not queued. ap_ctrl_none runs the function again automatically.
    wait_for_release:
    while (shot_trigger.read()) {
        #pragma HLS PIPELINE II=1
        out.write(0);
    }
}
