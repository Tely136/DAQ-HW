#include <iostream>
#include "ap_int.h"

#define N_BINS 10
#define N_REPS 100

void pulse_generator(int div[N_BINS], int n_reps, ap_uint<1>& out);
