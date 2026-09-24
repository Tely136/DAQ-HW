//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================

`ifndef READER_SUBSYSTEM_MONITOR_SV
`define READER_SUBSYSTEM_MONITOR_SV

`uvm_analysis_imp_decl(_svr_master_in_r)
`uvm_analysis_imp_decl(_axi_wtr_gmem)
`uvm_analysis_imp_decl(_axi_rtr_gmem)
`uvm_analysis_imp_decl(_axi_wtr_control)
`uvm_analysis_imp_decl(_axi_rtr_control)

class reader_subsystem_monitor extends uvm_component;

    reader_reference_model refm;
    reader_scoreboard scbd;

    `uvm_component_utils_begin(reader_subsystem_monitor)
    `uvm_component_utils_end

    uvm_analysis_imp_svr_master_in_r#(svr_pkg::svr_transfer#(32), reader_subsystem_monitor) svr_master_in_r_imp;
    uvm_analysis_imp_axi_wtr_gmem#(axi_pkg::axi_transfer, reader_subsystem_monitor) gmem_wtr_imp;
    uvm_analysis_imp_axi_rtr_gmem#(axi_pkg::axi_transfer, reader_subsystem_monitor) gmem_rtr_imp;
    uvm_analysis_imp_axi_wtr_control#(axi_pkg::axi_transfer, reader_subsystem_monitor) control_wtr_imp;
    uvm_analysis_imp_axi_rtr_control#(axi_pkg::axi_transfer, reader_subsystem_monitor) control_rtr_imp;

    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        if (!uvm_config_db#(reader_reference_model)::get(this, "", "refm", refm))
            `uvm_fatal(this.get_full_name(), "No refm from high level")
        `uvm_info(this.get_full_name(), "get reference model by uvm_config_db", UVM_MEDIUM)
        scbd = reader_scoreboard::type_id::create("scbd", this);
    endfunction

    virtual function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
    endfunction

    function new (string name = "", uvm_component parent = null);
        super.new(name, parent);
        svr_master_in_r_imp = new("svr_master_in_r_imp", this);
        gmem_wtr_imp = new("gmem_wtr_imp", this);
        gmem_rtr_imp = new("gmem_rtr_imp", this);
        control_wtr_imp = new("control_wtr_imp", this);
        control_rtr_imp = new("control_rtr_imp", this);
    endfunction

    virtual function void write_svr_master_in_r(svr_transfer#(32) tr);
        refm.write_svr_master_in_r(tr);
        scbd.write_svr_master_in_r(tr);
    endfunction

    virtual function void write_axi_wtr_gmem(axi_transfer tr);
        refm.write_axi_wtr_gmem(tr);
        scbd.write_axi_wtr_gmem(tr);
    endfunction

    virtual function void write_axi_rtr_gmem(axi_transfer tr);
        refm.write_axi_rtr_gmem(tr);
        scbd.write_axi_rtr_gmem(tr);
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
