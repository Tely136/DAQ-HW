#include "generator.h"
#include <stdio.h>


int main() {
    dout_t tmp;
    out_stream A;

    generator(A);

    for (int i=0; i<N; i++) {
        tmp = A.read();

        if (tmp!=i) {
            printf("failure\r\n");

            return 1;
        }
    }
    
    return 0;
}