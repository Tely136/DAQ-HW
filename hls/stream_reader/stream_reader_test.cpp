#include "stream_reader.h"
#include <stdio.h>

int main() {
    // Match the reader's m_axi depth so this buffer is also suitable for cosim.
    const int OUTPUT_DEPTH = 128;
    const int WAIT_READS = 3;
    static_assert(AVG_SHOT > 0 && N_BINS > 0 && N_CLK > 0,
                  "Test dimensions must be positive");
    static_assert(N_BINS <= OUTPUT_DEPTH, "Bins exceed the reader's m_axi depth");

    trigger_t trigger;
    stream_in data;
    dout_t counts[OUTPUT_DEPTH];
    for (int i = 0; i < OUTPUT_DEPTH; ++i)
        counts[i] = -1; // The first shot must overwrite previous contents.

    // One acquisition for now: provide exactly AVG_SHOT shots.
    // N_SHOT is reserved for a future multiple-acquisition test.
    for (int shot = 0; shot < AVG_SHOT; ++shot) {
        // These are values for successive trigger reads, not clock delays.
        for (int check = 0; check < WAIT_READS; ++check)
            trigger.write(0);
        trigger.write(1);

        for (int bin = 0; bin < N_BINS; ++bin) {
            for (int sample = 0; sample < N_CLK; ++sample)
                data.write(sample % (bin + 1) == 0);
        }
    }

    stream_reader(trigger, data, counts, AVG_SHOT, N_BINS, N_CLK);

    int errors = 0;
    for (int bin = 0; bin < N_BINS; ++bin) {
        // Independently count multiples of (bin + 1) in [0, N_CLK).
        const int per_shot = 1 + (N_CLK - 1) / (bin + 1);
        const int expected = AVG_SHOT * per_shot;
        printf("Bin %d: expected %d, got %d\n", bin, expected, counts[bin]);
        if (counts[bin] != expected)
            ++errors;
    }

    // Detect writes outside the requested bins, within the declared buffer.
    for (int bin = N_BINS; bin < OUTPUT_DEPTH; ++bin) {
        if (counts[bin] != -1) {
            printf("Unexpected write to output bin %d\n", bin);
            ++errors;
        }
    }

    if (data.size() != 0 || trigger.size() != 0) {
        printf("Unconsumed inputs: %d samples, %d trigger values\n",
               (int)data.size(), (int)trigger.size());
        ++errors;
    }

    printf("%s: single acquisition (%d shots)\n", errors ? "FAIL" : "PASS", AVG_SHOT);
    return errors ? 1 : 0;
}
