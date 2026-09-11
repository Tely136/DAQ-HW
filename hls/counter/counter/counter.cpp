static int tmp_count;
static int N;

#include "counter.h"

void MCS(bitstream& A, ap_uint<32> counts[MAX_BINS], int num_bins, int cycle_per_bin) {
#pragma HLS INTERFACE mode=m_axi port=counts
    
    for (int i=0; i<num_bins; i++) {
        N=0;
        tmp_count=0;
        
        int l = cycle_per_bin;
        while (N<l) {
            if(A.read()) {
                tmp_count = tmp_count + 1;
            }

            N++;
        }

        counts[i] = tmp_count;
    }
}
