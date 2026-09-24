//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`ifndef READER_CONFIG__SV                        
    `define READER_CONFIG__SV                    
                                                            
    class reader_config extends uvm_object;            
                                                            
        int check_ena;                                      
        int cover_ena;                                      
        svr_pkg::svr_config port_in_r_cfg;
        axi_pkg::axi_cfg gmem_cfg;
        axi_pkg::axi_cfg control_cfg;

        `uvm_object_utils_begin(reader_config)         
        `uvm_field_object(port_in_r_cfg, UVM_DEFAULT)
        `uvm_field_object(gmem_cfg, UVM_DEFAULT);
        `uvm_field_object(control_cfg, UVM_DEFAULT);
        `uvm_field_int   (check_ena , UVM_DEFAULT)          
        `uvm_field_int   (cover_ena , UVM_DEFAULT)          
        `uvm_object_utils_end                               

        function new (string name = "reader_config");
            super.new(name);                                
            port_in_r_cfg = svr_pkg::svr_config::type_id::create("port_in_r_cfg");
            gmem_cfg = new("gmem_cfg", 1);
            control_cfg = axi_pkg::axi_cfg::type_id::create("control_cfg");
        endfunction                                         
                                                            
    endclass                                                
                                                            
`endif                                                      
