#include "stream_reader.h"

static void count_bins(stream_in& data_in, counts_stream& bins, int n_outer, int n_inner) {
#pragma HLS INLINE off
    int count=0;
    
    for (int i=0; i<n_outer; i++) {
        for (int j=0; j<n_inner; j++) {
#pragma HLS PIPELINE II=1
            count += data_in.read();

            if (j==n_inner-1) {
                bins.write(count);
                count = 0;
            }
        }
    }
}

static void write_bins(counts_stream& bins, dout_t* out, int n_outer) {
#pragma HLS INLINE off
    for (int i=0; i<n_outer; i++) {
#pragma HLS PIPELINE II=1
        out[i] = bins.read();
    }
}

void stream_reader(stream_in& data_in, dout_t* data_out, int n_bin, int n_clk) {
#pragma HLS DATAFLOW
#pragma HLS INTERFACE mode=s_axilite port=n_clk
#pragma HLS INTERFACE mode=s_axilite port=n_bin
#pragma HLS INTERFACE mode=m_axi port=data_out depth=n_bin*n_clk
#pragma HLS INTERFACE mode=axis port=data_in
#pragma HLS INTERFACE mode=s_axilite port=return
    
    counts_stream bins("bins");
    
    count_bins(data_in, bins, n_bin, n_clk);
    write_bins(bins, data_out, n_bin);
}