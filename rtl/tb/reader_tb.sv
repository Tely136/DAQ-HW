`timescale 1ns/1ps
// A small, self-checking test of the complete reader block design.
module reader_tb;
    import axi_vip_pkg::*;
    import axi4stream_vip_pkg::*;
    // These names come from the VIP component names, not their BD labels.
    import design_1_axi_vip_0_0_pkg::*;
    import design_1_axi_vip_1_0_pkg::*;
    import design_1_axi4stream_vip_0_0_pkg::*;

    logic aclk = 0;
    logic rst = 0; // Active-low reset: 0 resets the reader and all VIPs.
    logic shot_trigger = 0;
    logic ap_start = 0;
    wire ap_done, ap_idle, ap_ready;
    always #5 aclk = ~aclk; // 100 MHz

    design_1_wrapper dut (
        .aclk(aclk), .aresetn(rst), .shot_trigger(shot_trigger),
        .ap_ctrl_start(ap_start), .ap_ctrl_done(ap_done),
        .ap_ctrl_idle(ap_idle), .ap_ctrl_ready(ap_ready)
    );

    design_1_axi_vip_0_0_mst_t control;
    design_1_axi_vip_1_0_slv_mem_t memory;
    design_1_axi4stream_vip_0_0_mst_t samples;

    // Addresses verified against design_1.bd and xstream_reader_hw.h.
    localparam bit [63:0] BUFFER = 64'h44A0_0000;
    localparam int REG_BUFFER_LO = 'h10, REG_BUFFER_HI = 'h14;
    localparam int REG_SHOTS = 'h1c, REG_BINS = 'h24, REG_CYCLES = 'h2c;
    int accepted_samples = 0;
    bit completed = 0;

    // Observe actual handshakes, not merely transactions queued to the VIP.
    always @(posedge aclk) begin
        if (rst && dut.design_1_i.stream_reader_0.data_in_TVALID &&
                   dut.design_1_i.stream_reader_0.data_in_TREADY)
            accepted_samples <= accepted_samples + 1;
        if (rst && ap_done)
            completed <= 1;
    end

    task automatic write_register(input int address, input bit [31:0] value);
        xil_axi_resp_t response;
        control.AXI4LITE_WRITE_BURST(address, 0, value, response);
        if (response != XIL_AXI_RESP_OKAY)
            $fatal(1, "AXI-Lite write failed at %h", address);
    endtask

    task automatic trigger_shot();
        @(negedge aclk); shot_trigger = 1;
        @(negedge aclk); shot_trigger = 0;
    endtask

    task automatic send_sample(input bit value);
        axi4stream_transaction transaction;
        xil_axi4stream_data_byte payload[1];
        payload[0] = value;
        transaction = samples.driver.create_transaction("sample");
        transaction.set_data(payload);
        transaction.set_id(0);   // Unused by reader; avoid VIP default tags.
        transaction.set_dest(0);
        transaction.set_delay(0); // No intentional idle clocks between samples.
        samples.driver.send(transaction); // VIP holds VALID until READY.
    endtask

    task automatic check_bin(input int bin_index, input int expected);
        bit [31:0] actual;
        // Backdoor reads inspect simulated memory without adding an AXI master.
        actual = memory.mem_model.backdoor_memory_read_4byte(BUFFER + 4*bin_index);
        if (actual != expected)
            $fatal(1, "Bin %0d: expected %0d, got %0d", bin_index, expected, actual);
        $display("Bin %0d = %0d (correct)", bin_index, actual);
    endtask

    initial begin
        // 1. Attach software agents to the three simulated VIP interfaces.
        control = new("control", dut.design_1_i.control_vip.inst.IF);
        memory  = new("memory",  dut.design_1_i.memory_vip.inst.IF);
        samples = new("samples", dut.design_1_i.samples_vip.inst.IF);
        control.start_master();
        memory.start_slave();
        samples.start_master();

        // 2. Reset, then program two shots, two bins, four samples per bin.
        repeat (20) @(negedge aclk); // VIP requires at least 16 reset clocks.
        rst = 1;
        repeat (5) @(negedge aclk);
        write_register(REG_BUFFER_LO, BUFFER[31:0]);
        write_register(REG_BUFFER_HI, BUFFER[63:32]);
        write_register(REG_SHOTS, 2);
        write_register(REG_BINS, 2);
        write_register(REG_CYCLES, 4);

        // Poison initial memory: shot 1 must overwrite, not add to old data.
        memory.mem_model.backdoor_memory_write_4byte(BUFFER,   32'd100);
        memory.mem_model.backdoor_memory_write_4byte(BUFFER+4, 32'd100);

        // 3. Start ONE acquisition. Hold START until READY, concurrently with
        // the stimulus: this reader may only assert READY near completion.
        @(negedge aclk); ap_start = 1;
        fork
            begin
                do @(posedge aclk); while (!ap_ready);
                @(negedge aclk); ap_start = 0;
            end
            begin
                // Generous initial wait; this tests functionality, not minimum
                // trigger spacing. No trigger-ready output exists in this IP.
                repeat (100) @(negedge aclk);
                if (completed) $fatal(1, "Completed before any shot trigger");
                $display("Shot 1: expect partial sums [2, 3]");
                trigger_shot();
                send_sample(1); send_sample(0); send_sample(1); send_sample(0);
                send_sample(1); send_sample(1); send_sample(1); send_sample(0);
                wait (accepted_samples == 8);

                // Allow outstanding memory writes and transition to wait state.
                repeat (500) @(negedge aclk);
                if (completed) $fatal(1, "Completed after only one of two shots");
                check_bin(0, 2); check_bin(1, 3);

                $display("Shot 2: add [1, 2], expect final sums [3, 5]");
                trigger_shot();
                send_sample(0); send_sample(1); send_sample(0); send_sample(0);
                send_sample(1); send_sample(0); send_sample(0); send_sample(1);
                wait (completed);
            end
        join

        // 4. Check the final results after the acquisition completes.
        check_bin(0, 3); check_bin(1, 5);
        if (accepted_samples != 16)
            $fatal(1, "Expected 16 accepted samples, got %0d", accepted_samples);
        $display("PASS: two triggered shots accumulated correctly.");
        $finish;
    end

    // A separate concurrent process catches missed triggers or AXI deadlocks.
    initial begin
        #100000;
        $fatal(1, "Timeout after 100 us: inspect reset, trigger, AXI and ap_ctrl");
    end
endmodule
