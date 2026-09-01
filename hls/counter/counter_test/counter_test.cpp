// remember to look into controlling custom axi ip from linux

#include "ap_int.h"
// #include "hls_stream.h"

#include "counter_test.h"

// typedef ap_uint<1> uint128_t;  // 1-bit user defined type
// typedef  hls::stream<uint128_t> my_stream;  // A stream declaration

// static ap_uint<32> counts[NUM_BINS];

void test(ap_uint<1> pulse, ap_uint<1> next_bin, ap_uint<32> counts[NUM_BINS]) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=bram port=counts latency=1 storage_type=ram_1p

    if(pulse) {
        counts[0] = counts[0] + 1;
    }

}