#ifndef PULSE_SOURCE_H
#define PULSE_SOURCE_H

#include "ap_int.h"
#include "hls_directio.h"

typedef hls::ap_none<ap_uint<32>> source_period_t;
typedef hls::ap_none<ap_uint<1>> source_enable_t;
typedef hls::ap_none<ap_uint<1>> source_out_t;

void pulse_source(source_period_t& clk_periods,
                  source_enable_t& enable, source_out_t& out);

#endif