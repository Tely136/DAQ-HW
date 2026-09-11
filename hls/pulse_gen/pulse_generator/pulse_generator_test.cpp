#include "pulse_generator.h"

int main() {

    int d[N_BINS];
    ap_uint<1> out;

    for(int i=0;i<N_BINS; i++) {
        d[i] = i+1;
    }
    
    pulse_generator(d, N_REPS, out);

    return 0;
}