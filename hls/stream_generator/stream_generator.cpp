#include "stream_generator.h"

// change to use direct IO over streams, and make testbench work, if simple

void stream_generator(generator_trigger_t& shot_trigger, stream_out& out, int n_total, int* data) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=m_axi port=data offset=slave bundle=gmem depth=128
#pragma HLS INTERFACE mode=s_axilite port=data bundle=control
#pragma HLS INTERFACE mode=s_axilite port=n_total bundle=control

    // Configure data/n_total before triggering; keep them stable during a shot.
    // The ap_none type supplies a plain trigger input, without handshake ports.
    wait_for_trigger:
    while (!shot_trigger.read()) {
        #pragma HLS PIPELINE II=1
    }

    send_samples:
    for (int i = 0; i < n_total; ++i) {
        #pragma HLS PIPELINE II=1
        out.write(data[i]);
    }

    // Rearm only after low is observed. Pulses wholly inside send_samples
    // are not queued. ap_ctrl_none runs the function again automatically.
    wait_for_release:
    while (shot_trigger.read()) {
        #pragma HLS PIPELINE II=1
    }
}
