using namespace std;

#include <fstream>
#include "counter.h"

int main() {
    bitstream in;
    ap_uint<32> out[MAX_BINS];
    ap_uint<32> gold[MAX_BINS];

    int bins = NUM_BINS;
    int cycles = CYCLE_PER_BIN;

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
    
    MCS(in, out, bins, cycles);

    for (int i=0; i<NUM_BINS; i++) {
        if (gold[i] != out[i]) {
           cerr << "Results don't match" << endl;
           return 1; 
        }
    }

    return 0;
}
