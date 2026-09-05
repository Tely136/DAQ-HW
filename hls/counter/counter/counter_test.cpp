using namespace std;

#include <fstream>
#include "counter.h"

int main() {
    bitstream in;
    ap_uint<16> out[NUM_BINS];
    ap_uint<16> gold[NUM_BINS];

    std::ifstream photon_stream_file("photon_stream.dat");
    std::ifstream golden_data_file("golden.dat");

    if (!photon_stream_file || !golden_data_file) { 
        cerr << "File error!" << endl;
        return 1;
    }

    int x;
    while (photon_stream_file >> x) {
        in.write((ap_uint<1>)x);
    }

    int tmp_cnt = 0;
    while (golden_data_file >> x) {
        gold[tmp_cnt]=x;
        
        tmp_cnt++;
    }
    
    example(in, out);

    for (int i=0; i<NUM_BINS; i++) {
        if (gold[i] != out[i]) {
           cerr << "Results don't match" << endl;
           return 1; 
        }
    }

    return 0;
}
