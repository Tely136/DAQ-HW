static int tmp_count;
static int N;


#include "counter.h"

void example(bitstream& A, ap_uint<16> counts[NUM_BINS]) {
    for (int i=0; i<NUM_BINS; i++) {
        N=0;
        tmp_count=0;
        
        int l = CYCLE_PER_BIN;
        while (N<l) {
            if(A.read()) {
                tmp_count = tmp_count + 1;
            }

            N++;
        }

        counts[i] = tmp_count;
    }
}
