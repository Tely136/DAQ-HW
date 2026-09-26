//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`timescale 1ns/1ps 

`ifndef STREAM_GENERATOR_SUBSYSTEM_PKG__SV          
    `define STREAM_GENERATOR_SUBSYSTEM_PKG__SV      
                                                     
    package stream_generator_subsystem_pkg;               
                                                     
        import uvm_pkg::*;                           
        import file_agent_pkg::*;                    
        import svr_pkg::*;
        import axi_pkg::*;
                                                     
        `include "uvm_macros.svh"                  
                                                     
        `include "stream_generator_config.sv"           
        `include "stream_generator_reference_model.sv"  
        `include "stream_generator_scoreboard.sv"       
        `include "stream_generator_subsystem_monitor.sv"
        `include "stream_generator_virtual_sequencer.sv"
        `include "stream_generator_pkg_sequence_lib.sv" 
        `include "stream_generator_env.sv"              
                                                     
    endpackage                                       
                                                     
`endif                                               
