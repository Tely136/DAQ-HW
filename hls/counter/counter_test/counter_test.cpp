// remember to look into controlling custom axi ip from linux

#include "ap_int.h"
#include "hls_stream.h"

#include "counter_test.h"

typedef ap_uint<1> uint128_t;  // 1-bit user defined type
typedef  hls::stream<uint128_t> my_stream;  // A stream declaration

static ap_uint<32> counts[NUM_BINS];

void test(my_stream& A, ap_uint<1> next_bin) {
#pragma HLS BIND_STORAGE variable=counts type=ram_2p impl=bram

for (int i = 0; i<NUM_BINS; i++) {
    int count_tmp = 0;
    
    do {
        count_tmp = count_tmp + A.read();
    } while (next_bin == 0);

    counts[i] = count_tmp;
}

}