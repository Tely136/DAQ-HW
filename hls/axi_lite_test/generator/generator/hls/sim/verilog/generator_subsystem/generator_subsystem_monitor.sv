//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================

`ifndef GENERATOR_SUBSYSTEM_MONITOR_SV
`define GENERATOR_SUBSYSTEM_MONITOR_SV

`uvm_analysis_imp_decl(_svr_slave_out_r)
`uvm_analysis_imp_decl(_axi_wtr_control)
`uvm_analysis_imp_decl(_axi_rtr_control)

class generator_subsystem_monitor extends uvm_component;

    generator_reference_model refm;
    generator_scoreboard scbd;

    `uvm_component_utils_begin(generator_subsystem_monitor)
    `uvm_component_utils_end

    uvm_analysis_imp_svr_slave_out_r#(svr_pkg::svr_transfer#(32), generator_subsystem_monitor) svr_slave_out_r_imp;
    uvm_analysis_imp_axi_wtr_control#(axi_pkg::axi_transfer, generator_subsystem_monitor) control_wtr_imp;
    uvm_analysis_imp_axi_rtr_control#(axi_pkg::axi_transfer, generator_subsystem_monitor) control_rtr_imp;

    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        if (!uvm_config_db#(generator_reference_model)::get(this, "", "refm", refm))
            `uvm_fatal(this.get_full_name(), "No refm from high level")
        `uvm_info(this.get_full_name(), "get reference model by uvm_config_db", UVM_MEDIUM)
        scbd = generator_scoreboard::type_id::create("scbd", this);
    endfunction

    virtual function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
    endfunction

    function new (string name = "", uvm_component parent = null);
        super.new(name, parent);
        svr_slave_out_r_imp = new("svr_slave_out_r_imp", this);
        control_wtr_imp = new("control_wtr_imp", this);
        control_rtr_imp = new("control_rtr_imp", this);
    endfunction

    virtual function void write_svr_slave_out_r(svr_transfer#(32) tr);
        refm.write_svr_slave_out_r(tr);
        scbd.write_svr_slave_out_r(tr);
    endfunction

    virtual function void write_axi_wtr_control(axi_transfer tr);
        refm.write_axi_wtr_control(tr);
        scbd.write_axi_wtr_control(tr);
    endfunction

    virtual function void write_axi_rtr_control(axi_transfer tr);
        refm.write_axi_rtr_control(tr);
        scbd.write_axi_rtr_control(tr);
    endfunction
endclass
`endif
