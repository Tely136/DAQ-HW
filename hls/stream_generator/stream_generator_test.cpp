#include "stream_generator.h"
#include <stdio.h>

struct TestCase {
    int bins;
    int cycles;
    int low_reads_before;
    int high_reads_after;
};

int main() {
    generator_trigger_t trigger;
    stream_out output;
    int errors = 0;
    const TestCase cases[] = {
        {N_BINS, N_CLK, 2, 1}, // Default pattern, including idle writes.
        {N_BINS, N_CLK, 0, 0}, // Repeat with immediate trigger and release.
        {7, 3, 1, 3},          // Periods longer than a bin; held trigger.
        {1, 1, 0, 0}           // Minimum positive dimensions.
    };

    // Direct-I/O queues model successive C reads/writes, NOT clock cycles.
    // No trigger reads occur during send_samples. Do not enqueue a trigger
    // value per generated sample. RTL simulation must check actual timing,
    // bin-boundary gaps, and output level on cycles without an out.write().
    for (unsigned shot = 0; shot < sizeof(cases) / sizeof(cases[0]); ++shot) {
        const TestCase& test = cases[shot];
        for (int i = 0; i < test.low_reads_before; ++i)
            trigger.write(0);
        trigger.write(1); // Exit wait_for_trigger; this read writes no output.
        for (int i = 0; i < test.high_reads_after; ++i)
            trigger.write(1);
        trigger.write(0); // Exit wait_for_release; this read writes no output.

        stream_generator(trigger, output, test.bins, test.cycles);

        const int expected_size = test.low_reads_before
                                + test.bins * test.cycles + 1
                                + test.high_reads_after;
        if ((int)output.size() != expected_size) {
            printf("FAIL: shot %u output size %d, expected %d\n",
                   shot, (int)output.size(), expected_size);
            return 1; // Avoid blocking on missing output below.
        }

        for (int i = 0; i < test.low_reads_before; ++i) {
            if ((int)output.read() != 0) {
                printf("FAIL: shot %u nonzero output before trigger\n", shot);
                ++errors;
            }
        }

        for (int bin = 0; bin < test.bins; ++bin) {
            int next_pulse = 0;
            int count = 0;
            for (int cycle = 0; cycle < test.cycles; ++cycle) {
                // Independent reference: schedule pulses instead of using %.
                const int expected = (cycle == next_pulse);
                if (expected)
                    next_pulse += bin + 1;
                const int actual = (int)output.read();
                count += actual;
                if (actual != expected) {
                    printf("FAIL: shot %u bin %d sample %d expected %d, got %d\n",
                           shot, bin, cycle, expected, actual);
                    ++errors;
                }
            }
            const int expected_count = 1 + (test.cycles - 1) / (bin + 1);
            if (count != expected_count) {
                printf("FAIL: shot %u bin %d count %d, expected %d\n",
                       shot, bin, count, expected_count);
                ++errors;
            }
        }

        // output must go low after the samples, even if trigger is already low
        if ((int)output.read() != 0) {
            printf("FAIL: shot %u nonzero output after samples\n", shot);
            ++errors;
        }

        for (int i = 0; i < test.high_reads_after; ++i) {
            if ((int)output.read() != 0) {
                printf("FAIL: shot %u nonzero output while waiting for release\n", shot);
                ++errors;
            }
        }
        if (trigger.size() != 0 || output.size() != 0) {
            printf("FAIL: shot %u has unconsumed I/O\n", shot);
            return 1;
        }
    }

    printf("%s: four functional shots (%d errors)\n", errors ? "FAIL" : "PASS", errors);
    return errors ? 1 : 0;
}
