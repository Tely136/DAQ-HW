#include "hls_stream.h"
#include "ap_int.h"
#include "hls_directio.h"

#include <stdio.h>

#define MAX_BINS 128

#define N_TRIG 100  /* number of clock pulses between triggers */

#define N_SHOT 12 /* total number of shots in simulation */
#define AVG_SHOT 10 /* number of shots to average */

#define N_BINS 5 /* number of bins to acquire */
#define N_CLK 10 /* number of clock pulses within each bin */

#define N_TOTAL N_SHOT*N_TRIG /* total number of clock pulses in simulation */

typedef hls::ap_none<ap_uint<1>> trigger_t;

typedef ap_uint<1> din_t;
typedef hls::ap_none<din_t> stream_in;

typedef int dout_t;

void multi_channel_scaler(trigger_t& shot_trigger, stream_in& data_in, dout_t* data_out, int avg_shot, int n_bin, int n_cycle);