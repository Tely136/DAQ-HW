// host-only tests: generated drivers access arrays instead of physical registers
#define main daq_server_main
#include "../src/main.c"
#undef main
#include <assert.h>

int main(void) {
    struct daq_config config = { .n_shots=1200, .n_bins=10, .cycles_per_bin=10 };
    const uint32_t periods[] = {0, 1, 2, 4, 20, UINT32_MAX};
    const uint64_t lower[] = {0, 12000, 6000, 2400, 0, 0};
    const uint64_t upper[] = {0, 12000, 6000, 3600, 1200, 1200};
    for (unsigned i=0; i<sizeof(periods)/sizeof(periods[0]); i++) {
        uint64_t min_counts, max_counts;
        config.source_period_cycles = periods[i];
        daq_count_bounds(&config, &min_counts, &max_counts);
        assert(min_counts == lower[i]);
        assert(max_counts == upper[i]);
    }

    uint32_t trigger_regs[64] = {0};
    uint32_t source_regs[64] = {0};
    uint32_t mcs_regs[64] = {0};
    struct daq_device daq = {0};
    daq.trigger.Control_BaseAddress = (uintptr_t)trigger_regs;
    daq.trigger.IsReady = XIL_COMPONENT_IS_READY;
    daq.source.Control_BaseAddress = (uintptr_t)source_regs;
    daq.source.IsReady = XIL_COMPONENT_IS_READY;
    daq.mcs.Control_BaseAddress = (uintptr_t)mcs_regs;
    daq.mcs.IsReady = XIL_COMPONENT_IS_READY;

    assert(daq_configure(&daq, &config, "CONFIG 1200 10 10 4 20") == 0);
    assert(config.n_shots == 1200 && config.n_bins == 10);
    assert(config.cycles_per_bin == 10 && config.source_period_cycles == 4);
    assert(XPulse_source_Get_clk_periods(&daq.source) == 4);
    assert(XTrigger_Get_enable(&daq.trigger) == 0);
    assert(XPulse_source_Get_enable(&daq.source) == 0);

    const char* invalid[] = {
        "CONFIG 0 10 10 4 20", "CONFIG 1 129 10 4 20", "CONFIG 1 10 0 4 20",
        "CONFIG 1 10 10 4294967296 20", "CONFIG -1 10 10 4 20",
        "CONFIG 2147483647 10 2 4 20", "CONFIG 1 10 10 20", "CONFIG 1 10 10 4 extra 20"
    };
    for (unsigned i=0; i<sizeof(invalid)/sizeof(invalid[0]); i++) {
        assert(daq_configure(&daq, &config, (char*)invalid[i]) != 0);
        assert(!daq.configured);
    }
    assert(daq_configure(&daq, &config, "CONFIG 1 128 10 0 20") == 0);

    assert(XTrigger_Get_clk_periods(&daq.trigger) == 2500000);
    assert(daq_configure(&daq, &config, "CONFIG 1 10 10 4 12.5") == 0);
    assert(config.trigger_period_cycles == 4000000);
    assert(XTrigger_Get_clk_periods(&daq.trigger) == 4000000);
    assert(daq_configure(&daq, &config, "CONFIG 1 10 10 4 3") == 0);
    assert(config.trigger_period_cycles == 16666667);
    const char* invalid_rates[] = {
        "CONFIG 1 10 10 4", "CONFIG 1 10 10 4 0", "CONFIG 1 10 10 4 -1",
        "CONFIG 1 10 10 4 nan", "CONFIG 1 10 10 4 inf",
        "CONFIG 1 10 10 4 0.001", "CONFIG 1 10 10 4 25000001"
    };
    for (unsigned i=0; i<sizeof(invalid_rates)/sizeof(invalid_rates[0]); i++) {
        assert(daq_configure(&daq, &config, (char*)invalid_rates[i]) != 0);
    }

    // command and binary framing must stay compatible with the MATLAB client
    int sockets[2];
    char line[128];
    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0);
    const char commands[] = "CONFIG 1 10 10 4 20\nSTART\nEND\n";
    assert(send_all(sockets[0], commands, sizeof(commands)-1) == 0);
    assert(recv_line(sockets[1], line, sizeof(line)) == 1);
    assert(strcmp(line, "CONFIG 1 10 10 4 20") == 0);
    assert(recv_line(sockets[1], line, sizeof(line)) == 1 && strcmp(line, "START") == 0);
    assert(recv_line(sockets[1], line, sizeof(line)) == 1 && strcmp(line, "END") == 0);
    uint32_t sent[] = {2, 3, 0, 12000};
    uint32_t received[4];
    assert(send_all(sockets[0], sent, sizeof(sent)) == 0);
    assert(recv_exact(sockets[1], received, sizeof(received)) == 1);
    assert(memcmp(sent, received, sizeof(sent)) == 0);
    close(sockets[0]);
    close(sockets[1]);
    puts("PASS: count bounds, configuration validation/readback, and TCP framing");
    return 0;
}