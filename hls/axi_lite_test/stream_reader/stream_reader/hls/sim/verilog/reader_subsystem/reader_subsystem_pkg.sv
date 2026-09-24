//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`timescale 1ns/1ps 

`ifndef READER_SUBSYSTEM_PKG__SV          
    `define READER_SUBSYSTEM_PKG__SV      
                                                     
    package reader_subsystem_pkg;               
                                                     
        import uvm_pkg::*;                           
        import file_agent_pkg::*;                    
        import svr_pkg::*;
        import axi_pkg::*;
                                                     
        `include "uvm_macros.svh"                  
                                                     
        `include "reader_config.sv"           
        `include "reader_reference_model.sv"  
        `include "reader_scoreboard.sv"       
        `include "reader_subsystem_monitor.sv"
        `include "reader_virtual_sequencer.sv"
        `include "reader_pkg_sequence_lib.sv" 
        `include "reader_env.sv"              
                                                     
    endpackage                                       
                                                     
`endif                                               
