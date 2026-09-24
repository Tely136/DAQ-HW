#include "stream_reader.h"


int main() {
    stream_in A;
    dout_t B[N];
    
    for (int i=0; i<N; i++) {
        A.write(i);
    }

    reader(A,B);

    for (int i=0; i<N; i++) {
        if(B[i] != i) {
            printf("failure\r\n");
            return 1;
        }
    }

    return 0;
}