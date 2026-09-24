//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`ifndef GENERATOR_VIRTUAL_SEQUENCER__SV                        
    `define GENERATOR_VIRTUAL_SEQUENCER__SV                    
                                                                       
    class generator_virtual_sequencer extends uvm_sequencer;         
        svr_slave_sequencer#(32) svr_port_out_r_sqr;
        axi_pkg::axi_virtual_sequencer control_sqr; 
 
        function new (string name, uvm_component parent);              
            super.new(name, parent);                                   
            //`uvm_info(this.get_full_name(), "new is called", UVM_LOW)
        endfunction                                                    
                                                                       
        `uvm_component_utils_begin(generator_virtual_sequencer)      
        `uvm_component_utils_end                                       
                                                                       
    endclass

`endif
