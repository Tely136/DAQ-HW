#include "generator.h"
#include <stdio.h>


void generator(out_stream& out) {
#pragma HLS INTERFACE mode=axis port=out
#pragma HLS INTERFACE mode=s_axilite port=return
    for (int i=0; i<N; i++) {
        out.write(i);

        // printf("val: %d\r\n",out);
    }
}