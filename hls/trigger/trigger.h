#ifndef SHOT_TRIGGER_H
#define SHOT_TRIGGER_H

#include "ap_int.h"
#include "hls_directio.h"

typedef ap_uint<1> out_t;
typedef hls::ap_none<ap_uint<32>> period_port_t;
typedef hls::ap_none<ap_uint<1>> enable_port_t;
typedef hls::ap_none<out_t> trigger_out_t;

void trigger(period_port_t& clk_periods, enable_port_t& enable, trigger_out_t& out);

#endif
