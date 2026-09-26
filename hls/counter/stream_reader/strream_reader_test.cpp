#include "stream_reader.h"

int main() {
    int i,j;
    int tmp;

    stream_in A;
    dout_t B[N_TOTAL];

    for (i=0; i<N_OUTER; i++) {
        for (j=0; j<N_INNER; j++) {            
            A.write((i+1)*100 + j);
        }
    }

    stream_reader(A,B,N_OUTER,N_INNER);

    int id = 0;
    for (i=0; i<N_OUTER; i++) {
        for (j=0; j<N_INNER; j++) {            
            if (B[id] != ((i+1)*100 + j)) {
                printf("failure\r\n");


                return 1;
            }
            
            id++;
        }
    }

    return 0;
}
