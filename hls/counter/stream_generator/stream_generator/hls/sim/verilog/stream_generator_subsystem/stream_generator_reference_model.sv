//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================

`ifndef STREAM_GENERATOR_REFERENCE_MODEL_SV
`define STREAM_GENERATOR_REFERENCE_MODEL_SV
typedef class stream_generator_reference_model;
class memaccess_axi_state_cbs extends axi_pkg::axi_state_cbs;
    stream_generator_reference_model refm;
    string memid;
    //function new(string name="memaccess_axi_state_cbs");
    //    super.new(name);
    //endfunction
    virtual function void memmodel_read_fromar(ref logic[7:0] data[$], input longint addr, input longint len);
        if(memid=="gmem") refm.mem_blk_pages_gmem.read_elems_pipepage(data, addr, len);
    endfunction
endclass

class stream_generator_reference_model extends uvm_component;
`define TV_IN_gmem "../tv/cdatafile/c.stream_generator.autotvin_gmem.dat"
`define TV_OUT_gmem "../tv/rtldatafile/rtl.stream_generator.autotvout_gmem.dat"
`define TV_IN_OFFSET_data "../tv/cdatafile/c.stream_generator.autotvin_data.dat"
`define TV_IN_n_total "../tv/cdatafile/c.stream_generator.autotvin_n_total.dat"
`define TV_OUT_n_total ""
`define TV_IN_data "../tv/cdatafile/c.stream_generator.autotvin_data.dat"
`define TV_OUT_data ""
    bit  write_data_finish_control;
    event allaxilite_write_data_finish;
    event allsvr_input_done;
    event allsvr_output_done;
    event write_start_finish;
    int trans_num_total = 1;
    int trans_num_idx;
    int ap_done_cnt=1;
    event dut2tb_ap_ready;
    event dut2tb_ap_done;
    event ap_ready_for_nexttrans;
    event ap_done_for_nexttrans;
    event finish;
    stream_generator_config stream_generator_cfg;
    virtual interface misc_interface misc_if;

    mem_model_pages_with_diffofst#(32,8) mem_blk_pages_gmem;
    int blk_id_gmem = 0;
    memaccess_axi_state_cbs axi_memaccess_cb_gmem;

    mem_model_pages#(32,8) mem_blk_pages_control_n_total;
    int svr_out_r_delay;
    covergroup svr_out_r_cov;
        delay: coverpoint svr_out_r_delay
        {
            bins norm[2] = { [0 : 1] };
        }
    endgroup
    
    `uvm_component_utils_begin(stream_generator_reference_model)
        `uvm_field_int (trans_num_idx, UVM_DEFAULT)
    `uvm_component_utils_end

    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        if(!uvm_config_db#(virtual misc_interface)::get(this, "", "misc_if", misc_if))
            `uvm_fatal(this.get_full_name(), "No misc_if from high level")
        axi_memaccess_cb_gmem = new;
        axi_memaccess_cb_gmem.refm = this;
        axi_memaccess_cb_gmem.memid = "gmem";
    endfunction

    function new (string name = "", uvm_component parent = null);
        super.new (name, parent);
        svr_out_r_cov = new;
        trans_num_idx= 0;
    endfunction

    virtual task run_phase(uvm_phase phase);
        string fpath[$];
misc_if.dut2tb_ap_done = 0;

        fpath.push_back(`TV_IN_n_total);
        mem_blk_pages_control_n_total = mem_model_pages#(32,8)::type_id::create("mem_blk_pages_control_n_total");
        mem_blk_pages_control_n_total.tvinload_pagechk_atinit(fpath, 1*((32+7)/8), 0, 16);
        fpath.delete;

        fpath.push_back(`TV_IN_gmem);
        mem_blk_pages_gmem = mem_model_pages_with_diffofst#(32,8)::type_id::create("mem_blk_pages_gmem");
        mem_blk_pages_gmem.whole_page_size=576;
        mem_blk_pages_gmem.maxi_bundlevar_fpath["data"]=`TV_IN_OFFSET_data;
        mem_blk_pages_gmem.set_binary(1);
        mem_blk_pages_gmem.tvinload_pagechk_atinit(fpath, 128*((32+7)/8), 0, 0);
        fpath.delete();

        fork
            forever begin
                fork
                    begin
                        wait(write_data_finish_control);
                    end
                join
                `uvm_info("", "trigger_allaxilite_write_data_finish", UVM_LOW)
                @(posedge misc_if.clock);
                write_data_finish_control = 0;
                -> allaxilite_write_data_finish;
            end
            forever begin
                //this is non-pipeline case
                forever begin
                    @(negedge misc_if.clock);
                    if(misc_if.dut2tb_ap_done===1) break;
                end
                @(posedge misc_if.clock);
                @allaxilite_write_data_finish;
                @(posedge misc_if.clock);
                -> ap_ready_for_nexttrans;
                `uvm_info(this.get_full_name(), "trigger event ap_ready_for_nexttrans", UVM_LOW)
                fork
                    begin
                        misc_if.ap_ready_for_nexttrans = 1;
                        @(posedge misc_if.clock);
                        misc_if.ap_ready_for_nexttrans = 0;
                    end
                join_none
            end
            forever begin
                forever begin
                    @(negedge misc_if.clock);
                    if(misc_if.dut2tb_ap_done===1) break;
                end
                @(posedge misc_if.clock);
                fork
                    begin
                        @(negedge misc_if.clock);
                        -> misc_if.dut2tb_ap_done_evt;
                        #0;
                        -> misc_if.dut2tb_ap_ready_evt;
                    end
                join_none
                -> ap_done_for_nexttrans;
                `uvm_info(this.get_full_name(), "trigger event ap_done_for_nexttrans", UVM_LOW)
                fork
                    begin
                        misc_if.ap_done_for_nexttrans = 1;
                        @(posedge misc_if.clock);
                        misc_if.ap_done_for_nexttrans = 0;
                    end
                join_none
            end

            for(int i=1; i<1; i++) begin
                @dut2tb_ap_ready;
                mem_blk_pages_gmem.incr_rd_page_idx() ;
            end
            forever begin
                forever begin
                    @(negedge misc_if.clock);
                    if (misc_if.dut2tb_ap_ready === 1)   break;
                end
                @(posedge misc_if.clock);
                `uvm_info(this.get_full_name(), "trigger event DUT2TB_AP_READY", UVM_LOW)
                -> dut2tb_ap_ready;
                 misc_if.tb2dut_ap_start = 0;
            end
            forever begin
                forever begin
                    @(negedge misc_if.clock);
                    if (misc_if.dut2tb_ap_done_kernel === 1)   break;
                end
                @(posedge misc_if.clock);
                fork
                    begin
                        @(negedge misc_if.clock);
                        `uvm_info(this.get_full_name(), "trigger event dut2tb_ap_done_kernel_evt", UVM_LOW)
                        -> misc_if.dut2tb_ap_done_kernel_evt;
                    end
                join_none
            end
        join
    endtask

    virtual function void write_svr_slave_out_r(svr_transfer#(32) tr);
    //  trans_size++;
        svr_out_r_delay = tr.delay;
        svr_out_r_cov.sample();
        `uvm_info(this.get_full_name(), "port a collected one pkt", UVM_DEBUG);
    endfunction

    virtual function void write_axi_wtr_gmem(axi_pkg::axi_transfer tr);
        mem_blk_pages_gmem.write_elems_pipepage(tr.data,tr.byte_addr);
    endfunction

    virtual function void write_axi_rtr_gmem(axi_pkg::axi_transfer tr);
    endfunction

    virtual function void write_axi_wtr_control(axi_pkg::axi_transfer tr);
        if(tr.addr == 0 && tr.len == 0 && tr.data[0][0]==1) begin //addr 0 and bit 0 are parameter
            -> write_start_finish;
            misc_if.tb2dut_ap_start = 1;
        end
    endfunction
    virtual function void write_axi_rtr_control(axi_pkg::axi_transfer tr);
            `uvm_info("receive axi read data", tr.sprint(), UVM_HIGH)
        if(tr.addr == 0 && tr.len == 0) begin
            if(tr.data[0][1]==1) begin  //bit 1 is parameter
                `uvm_info("status polling", "ap_done is polled", UVM_LOW);
                fork
                    begin
                        misc_if.dut2tb_ap_done = 1;
                        @(posedge misc_if.clock);
                        #0;
                        misc_if.dut2tb_ap_done = 0;
                        misc_if.tb2dut_ap_continue = 0;
                        -> dut2tb_ap_done;
                    end
                join_none
            end
            begin
                misc_if.dut2tb_ap_idle = tr.data[0][2];
            end
        end else begin
        end
    endfunction
endclass
`endif
