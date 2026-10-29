#include "pulse_source.h"
#include <stdio.h>


struct test_case {
    unsigned period;
    int cycles;
};

int main() {
    source_period_t period;
    source_enable_t enable;
    source_out_t output;
    int errors = 0;

    const test_case tests[] = {
        {1, 20},           // all ones
        {2, 21},           // alternating zeros and ones
        {5, 23},           // stop partway through a period
        {5, 15},           // reenable starts a full period
        {0, 10},           // all zeros
        {3, 30},           // several complete periods
        {0xffffffffu, 10}  // large unsigned period
    };

    // queued values represent C reads, not actual clock cycles
    for (unsigned test=0; test<sizeof(tests)/sizeof(tests[0]); test++) {
        unsigned p = tests[test].period;
        int cycles = tests[test].cycles;

        // begin disabled
        period.write(p);
        enable.write(0);

        for (int i=0; i<cycles; i++) {
            period.write(p);
            enable.write(1);
        }

        // finish disabled
        period.write(p);
        enable.write(0);

        pulse_source(period, enable, output);

        if (period.size() != 0 || enable.size() != 0 ||
            output.size() != (unsigned)(cycles + 2)) {
            printf("FAIL: test %u has incorrect input/output lengths\n", test);
            return 1;
        }

        if (output.read() != 0) {
            printf("FAIL: test %u output high while disabled\n", test);
            errors++;
        }

        int count = 0;

        for (int i=0; i<cycles; i++) {
            // modulo is only used in the testbench
            int expected = (p != 0 && ((unsigned)i + 1) % p == 0);
            int actual = output.read();
            count += actual;

            if (actual != expected) {
                printf("FAIL: test %u sample %d expected %d, read %d\n",
                       test, i, expected, actual);
                errors++;
            }
        }

        int expected_count = p == 0 ? 0 : (unsigned)cycles / p;

        if (count != expected_count) {
            printf("FAIL: test %u expected %d counts, read %d\n",
                   test, expected_count, count);
            errors++;
        }

        if (output.read() != 0) {
            printf("FAIL: test %u output high after disabling\n", test);
            errors++;
        }
    }

    printf("%s: seven source tests, %d errors\n",
           errors ? "FAIL" : "PASS", errors);

    return errors ? 1 : 0;
}