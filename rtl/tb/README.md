# Reader RTL testbench

`reader_tb.sv` tests the existing `design_1` block design with two laser shots,
two bins per shot, and four samples per bin. It uses the actual exported reader
and all three VIPs. The HLS algorithm is not modified.

## Run in Vivado

Open `DAQ-BUILD/simulation/simulation.xpr`. Save any block-design edits first.
In the Tcl console:

```tcl
source C:/Users/tely1/PhD/DAQ-HW/rtl/tb/setup_reader_tb.tcl
launch_simulation
add_wave /reader_tb/*
run all
```

If a simulation is already open, close it first. The setup script generates BD
output products, refreshes the wrapper, adds the testbench to `sim_1`, and selects
`reader_tb` as the simulation top. No synthesis or bitstream is required.

Expected output:

```text
Shot 1: expect partial sums [2, 3]
Bin 0 = 2 (correct)
Bin 1 = 3 (correct)
Shot 2: add [1, 2], expect final sums [3, 5]
Bin 0 = 3 (correct)
Bin 1 = 5 (correct)
PASS: two triggered shots accumulated correctly.
```

## Read the testbench in this order

1. **Signals and DUT:** `dut` is the block-design wrapper. `always #5` creates a
   100 MHz clock. `rst` is a local active-low signal connected to `aresetn`.
2. **VIP agents:** the three class objects control the corresponding simulated
   interfaces. Their package names retain Vivado's original component names even
   though the diagram labels are `control_vip`, `memory_vip`, and `samples_vip`.
3. **Helper tasks:** `write_register` configures AXI-Lite; `trigger_shot` emits
   one clock of trigger; `send_sample` queues a sample to the stream driver;
   `check_bin` reads the memory model and compares a count.
4. **Main initial block:** executes reset, configuration, start, both shots and
   checks. The samples differ between shots so accumulation is observable.
5. **fork/join:** runs start-handshake handling and stimulus concurrently. For
   this reader, waiting for `ap_ready` before sending the shots can deadlock.
6. **Monitors and timeout:** count accepted stream transfers, remember completion,
   and fail after 100 us rather than hanging forever.

The memory starts with 100 in each result bin. Shot 1 must overwrite that value.
The first shot is `[1,0,1,0] [1,1,1,0]`; the second is
`[0,1,0,0] [1,0,0,1]`. Each output int occupies four bytes.

The verified control offsets are data_out low/high at 0x10/0x14, avg_shot at
0x1c, n_bin at 0x24, and n_cycle at 0x2c. Control base is zero and the memory
segment starts at 0x44a00000. Recheck these after re-exporting changed HLS IP.

This is a functional teaching test, not a minimum-spacing or throughput test.
It deliberately waits 100 clocks before the first trigger and 500 clocks after
the first shot's final accepted sample. Those delays are not a ready handshake
or a general guarantee for different configurations. The sample VIP respects
backpressure; an external physical pulse source would need separate capture.

For waveforms, begin with `aclk`, `rst`, `shot_trigger`, `ap_start`, `ap_ready`,
`ap_done`, `accepted_samples`, and `completed`. Expand
`dut/design_1_i/stream_reader_0` to add `data_in_TVALID`, `data_in_TREADY`, and
`data_in_TDATA`. `restart` followed by `run all` reruns the simulation.
