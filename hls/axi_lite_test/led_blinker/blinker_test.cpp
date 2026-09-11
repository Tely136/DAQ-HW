#include "blinker.h"

int main() {
    int A = 3;
    int B = 4;
    ap_uint<1> C;
    ap_uint<1> D;

    blinker(A,B,C,D);

    return 0;
}