//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`timescale 1ns/1ps 

`ifndef GENERATOR_SUBSYSTEM_PKG__SV          
    `define GENERATOR_SUBSYSTEM_PKG__SV      
                                                     
    package generator_subsystem_pkg;               
                                                     
        import uvm_pkg::*;                           
        import file_agent_pkg::*;                    
        import svr_pkg::*;
        import axi_pkg::*;
                                                     
        `include "uvm_macros.svh"                  
                                                     
        `include "generator_config.sv"           
        `include "generator_reference_model.sv"  
        `include "generator_scoreboard.sv"       
        `include "generator_subsystem_monitor.sv"
        `include "generator_virtual_sequencer.sv"
        `include "generator_pkg_sequence_lib.sv" 
        `include "generator_env.sv"              
                                                     
    endpackage                                       
                                                     
`endif                                               
