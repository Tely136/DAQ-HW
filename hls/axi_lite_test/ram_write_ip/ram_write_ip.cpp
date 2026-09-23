#include <stdio.h>


void example(int* a, int N) {
#pragma HLS INTERFACE mode=s_axilite port=N
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=m_axi port=a
    
    for (int i=0; i<N; i++) {
        a[i] = i;
    }
    
}

