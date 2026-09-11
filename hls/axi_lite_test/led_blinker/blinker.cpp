#include <stdio.h>
#include "blinker.h"

void blinker(int& A, int& B, ap_uint<1>& C, ap_uint<1>& D) {
#pragma HLS INTERFACE mode=ap_none port=D
#pragma HLS INTERFACE mode=ap_none port=C
#pragma HLS INTERFACE mode=s_axilite port=B bundle=test
#pragma HLS INTERFACE mode=s_axilite port=A bundle=test
    C = 0;
    D = 0;

    for(int i=0; i<A; i++) {
        C=~C;
        printf("%d\n",(int)C);
    }

    for(int i=0; i<B; i++) {
        D=~D;
        printf("%d\n",(int)D);
    }
}