#include "stream_reader.h"
#include <stdio.h>

int main() {
    int i,j;
    int tmp;

    stream_in A;
    int count_test[N_BINS];
    dout_t counts[N_TOTAL];

    int count;
    for (i=0; i<N_BINS; i++) {
        
        count = 0;
        for (j=0; j<N_CLK; j++) {            
            tmp = (j%(i+1)==0);
            count += tmp;
            
            A.write(tmp);
        }
        
        // printf("count test: %d\r\n",count);
        count_test[i] = count;
    }

    stream_reader(A,counts,N_BINS,N_CLK);

    for (i=0; i<N_BINS; i++) {
        printf("count test: %d\tcount real: %d\r\n", count_test[i], counts[i]);
        
        if (counts[i] != count_test[i]) {
            printf("failure\r\n");

            return 1;
        }
    }

    return 0;
}
