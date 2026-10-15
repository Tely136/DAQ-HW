#include "stream_generator.h"
#include <stdio.h>

int main() {
    generator_trigger_t trigger;
    stream_out output;
    int data_in[128] = {};
    static_assert(N_TOTAL > 0 && N_TOTAL <= 128, "Test exceeds m_axi depth");
    int errors = 0;

    // Two separate functional invocations; these queued values model reads,
    // not clock timing. RTL testing must check independently timed pulses.
    for (int shot = 0; shot < 2; ++shot) {
        for (int i = 0; i < N_TOTAL; ++i)
            data_in[i] = ((i + shot) % 3 == 0);

        trigger.write(0);
        trigger.write(0);
        trigger.write(1); // Release wait_for_trigger.
        trigger.write(1); // Still high when wait_for_release reads it.
        trigger.write(0); // Rearm.

        stream_generator(trigger, output, N_TOTAL, data_in);

        if ((int)output.size() != N_TOTAL) {
            printf("FAIL: shot %d has %d samples, expected %d\n",
                   shot, (int)output.size(), N_TOTAL);
            return 1;
        }
        for (int i = 0; i < N_TOTAL; ++i) {
            int actual = (int)output.read();
            if (actual != data_in[i]) {
                printf("FAIL: shot %d sample %d expected %d, got %d\n",
                       shot, i, data_in[i], actual);
                ++errors;
            }
        }
        if (trigger.size() != 0) {
            printf("FAIL: trigger inputs not fully consumed\n");
            ++errors;
        }
    }
    printf("%s: two triggered shots\n", errors ? "FAIL" : "PASS");
    return errors ? 1 : 0;
}
