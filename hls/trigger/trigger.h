#include "ap_int.h"

#define F_CLK 100
#define F_OUT 20
#define CLK_PERIODS F_CLK/F_OUT

typedef ap_uint<1> out_t ;

void trigger(int clk_periods, volatile out_t* out);