#include "stream_generator.h"
#include <stdio.h>

int main () {
    stream_out A;
    dout_t data_in[N_TOTAL];

    for (int i=0; i<N_TOTAL; i++) {
        data_in[i] = i;
    }

    stream_generator(A,N_TOTAL,data_in);

    int i,j;
    dout_t tmp;
    
    for (i=0; i<N_TOTAL; i++) {
        tmp = A.read();
            
        if(tmp != i) {
            printf("results don't match\r\n");

            return 1;
        }
    }

    return 0;
}