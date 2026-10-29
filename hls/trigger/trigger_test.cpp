#include "trigger.h"
#include <stdio.h>
#include <vector>

struct Sample {
    unsigned period;
    bool enabled;
    bool expected;
};

static void append(std::vector<Sample>& samples, unsigned period,
                   bool enabled, int iterations, const int* highs = 0,
                   int high_count = 0) {
    for (int i = 0; i < iterations; ++i) {
        bool expected = false;
        for (int j = 0; j < high_count; ++j)
            expected |= (i == highs[j]);
        samples.push_back({period, enabled, expected});
    }
}

int main() {
    std::vector<Sample> samples;
    // Each explicitly listed high index is relative to its enabled segment.
    append(samples, 5, false, 3);             // Disabled startup.
    const int period5[] = {4, 9, 14};
    append(samples, 5, true, 15, period5, 3);
    append(samples, 5, false, 2);             // Disable immediately after high.
    append(samples, 5, true, 3);              // Interrupt a partial period.
    append(samples, 5, false, 1);             // Must clear the partial count.
    append(samples, 5, true, 10, period5, 2);  // Full period after reenable.
    append(samples, 3, false, 2);             // Reconfigure while disabled.
    const int period3[] = {2, 5, 8};
    append(samples, 3, true, 9, period3, 3);
    append(samples, 3, false, 1);
    append(samples, 0, true, 4);              // Invalid periods stay low.
    append(samples, 1, true, 4);
    append(samples, 2, false, 1);
    const int period2[] = {1, 3, 5, 7};
    append(samples, 2, true, 8, period2, 4);   // Minimum supported period.
    append(samples, 2, false, 2);
    append(samples, 0xffffffffu, false, 1);
    append(samples, 0xffffffffu, true, 8);    // Unsigned maximum: no early pulse.
    append(samples, 5, false, 2);

    period_port_t periods;
    enable_port_t enables;
    trigger_out_t output;
    for (unsigned i = 0; i < samples.size(); ++i) {
        periods.write(samples[i].period);
        enables.write(samples[i].enabled);
    }

    trigger(periods, enables, output);
    if (periods.size() != 0 || enables.size() != 0 ||
        output.size() != samples.size()) {
        printf("FAIL: input consumption or output length mismatch\n");
        return 1;
    }

    int errors = 0;
    int pulses = 0;
    bool previous = false;
    for (unsigned i = 0; i < samples.size(); ++i) {
        const bool actual = output.read();
        pulses += actual;
        if (actual != samples[i].expected) {
            printf("FAIL: iteration %u period=%u enable=%d expected=%d actual=%d\n",
                   i, samples[i].period, (int)samples[i].enabled,
                   (int)samples[i].expected, (int)actual);
            ++errors;
        }
        if (actual && previous) {
            printf("FAIL: pulse wider than one iteration at %u\n", i);
            ++errors;
        }
        previous = actual;
    }
    printf("%s: %u iterations, %d pulses, %d errors\n",
           errors ? "FAIL" : "PASS", (unsigned)samples.size(), pulses, errors);
    return errors ? 1 : 0;
}
