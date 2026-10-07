#include "stream_reader.h"
#include <stdio.h>

int main() {
    int i,j;
    int tmp;

    stream_in A;
    int count_test[N_BINS][N_SHOT];
    dout_t counts[N_TOTAL];

    for (i=0; i<N_BINS; i++) {
        for (j=0; j<N_SHOT; j++) {
        count_test[i][j] = 0;
        }
    }

    int count;
    for (int k=0; k<N_SHOT; k++) {
        for (i=0; i<N_BINS; i++) {
            
            count = 0;
            for (j=0; j<N_CLK; j++) {            
                tmp = (j%(i+1)==0);
                count += tmp;
                
                A.write(tmp);
            }
            
            count_test[i][k] = (k==0 | k%AVG_SHOT==0) ? count : count_test[i][k-1] + count;
        }
    }


    for (int k=0; k<N_SHOT; k++) {
        stream_reader(A,counts,AVG_SHOT,N_BINS,N_CLK);

        // printf("Shot %d\n",k+1);
        for (i=0; i<N_BINS; i++) {
            // printf("count test: %d\tcount IP: %d\n", count_test[i][k], counts[i]);
            
            if (counts[i] != count_test[i][k]) {
                printf("failure\r\n");
                return 1;
            }
        }
    }

    return 0;
}
