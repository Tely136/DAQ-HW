#include "stream_generator.h"
#include <stdio.h>

int main () {
    stream_out A;
    
    dout_t tmp;
    int data_in[N_TOTAL];

    int i,j,tmp_in;

    int id=0;
    for (i=0; i<N_BINS; i++) {
        for (j=0; j<N_CLK; j++) {

            tmp_in = (j%(i+1)==0);
            data_in[id] = tmp_in;

            printf("in: %d\r\n",tmp_in);

            id++;
        }
    }

    stream_generator(A,N_TOTAL,data_in);

    for (i=0; i<N_TOTAL; i++) {
        tmp = A.read();
        printf("out: %d\r\n",(int)tmp);

        if(tmp != data_in[i]) {
            printf("results don't match\r\n");

            // return 1;
        }
    }

    return 0;
}