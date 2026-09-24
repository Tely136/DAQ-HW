//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`ifndef GENERATOR_CONFIG__SV                        
    `define GENERATOR_CONFIG__SV                    
                                                            
    class generator_config extends uvm_object;            
                                                            
        int check_ena;                                      
        int cover_ena;                                      
        svr_pkg::svr_config port_out_r_cfg;
        axi_pkg::axi_cfg control_cfg;

        `uvm_object_utils_begin(generator_config)         
        `uvm_field_object(port_out_r_cfg, UVM_DEFAULT)
        `uvm_field_object(control_cfg, UVM_DEFAULT);
        `uvm_field_int   (check_ena , UVM_DEFAULT)          
        `uvm_field_int   (cover_ena , UVM_DEFAULT)          
        `uvm_object_utils_end                               

        function new (string name = "generator_config");
            super.new(name);                                
            port_out_r_cfg = svr_pkg::svr_config::type_id::create("port_out_r_cfg");
            control_cfg = axi_pkg::axi_cfg::type_id::create("control_cfg");
        endfunction                                         
                                                            
    endclass                                                
                                                            
`endif                                                      
