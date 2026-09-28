#include "stream_reader.h"

static void count_bins(stream_in& data_in, hls::stream<dout_t>& bins, int n_outer, int n_inner) {
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

static void write_bins(hls::stream<dout_t>& bins, dout_t* out, int n_outer) {
#pragma HLS INLINE off
    for (int i=0; i<n_outer; i++) {
#pragma HLS PIPELINE II=1
        out[i] = bins.read();
    }
}

void stream_reader(stream_in& data_in, dout_t* data_out, int n_outer, int n_inner) {
#pragma HLS DATAFLOW
#pragma HLS INTERFACE mode=s_axilite port=n_inner
#pragma HLS INTERFACE mode=s_axilite port=n_outer
#pragma HLS INTERFACE mode=m_axi port=data_out depth=128
#pragma HLS INTERFACE mode=axis port=data_in
#pragma HLS INTERFACE mode=s_axilite port=return
    
    hls::stream<dout_t> bins("bins");
    
    count_bins(data_in, bins, n_outer, n_inner);
    write_bins(bins, data_out, n_outer);

}
