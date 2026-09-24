//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`ifndef GENERATOR_ENV__SV                                                                                   
    `define GENERATOR_ENV__SV                                                                               
                                                                                                                    
                                                                                                                    
    class generator_env extends uvm_env;                                                                          
                                                                                                                    
        generator_virtual_sequencer generator_virtual_sqr;                                                      
        generator_config generator_cfg;                                                                         
                                                                                                                    
        svr_pkg::svr_env#(32) env_slave_svr_out_r;
        axi_pkg::axi_env#(4,4,4,3,1) axi_lite_control;
                                                                                                                    
        generator_reference_model   refm;                                                                         
                                                                                                                    
        generator_subsystem_monitor subsys_mon;                                                                   
                                                                                                                    
        `uvm_component_utils_begin(generator_env)                                                                 
        `uvm_field_object (env_slave_svr_out_r,  UVM_DEFAULT | UVM_REFERENCE)
        `uvm_field_object (refm, UVM_DEFAULT | UVM_REFERENCE)                                                       
        `uvm_field_object (generator_virtual_sqr, UVM_DEFAULT | UVM_REFERENCE)                                    
        `uvm_field_object (generator_cfg        , UVM_DEFAULT)                                                    
        `uvm_component_utils_end                                                                                    
                                                                                                                    
        function new (string name = "generator_env", uvm_component parent = null);                              
            super.new(name, parent);                                                                                
        endfunction                                                                                                 
                                                                                                                    
        extern virtual function void build_phase(uvm_phase phase);                                                  
        extern virtual function void connect_phase(uvm_phase phase);                                                
        extern virtual task          run_phase(uvm_phase phase);                                                    
                                                                                                                    
    endclass                                                                                                        
                                                                                                                    
    function void generator_env::build_phase(uvm_phase phase);                                                    
        super.build_phase(phase);                                                                                   
        generator_cfg = generator_config::type_id::create("generator_cfg", this);                           
                                                                                                                    
        generator_cfg.port_out_r_cfg.svr_type = svr_pkg::SVR_SLAVE ;
        env_slave_svr_out_r  = svr_env#(32)::type_id::create("env_slave_svr_out_r", this);
        uvm_config_db#(svr_pkg::svr_config)::set(this, "env_slave_svr_out_r*", "cfg", generator_cfg.port_out_r_cfg);
        generator_cfg.port_out_r_cfg.prt_type = svr_pkg::AXIS;
        generator_cfg.port_out_r_cfg.is_active = svr_pkg::SVR_ACTIVE;
        generator_cfg.port_out_r_cfg.spec_cfg = svr_pkg::NORMAL;
        generator_cfg.port_out_r_cfg.reset_level = svr_pkg::RESET_LEVEL_LOW;
 

        generator_cfg.control_cfg.set_default();
        generator_cfg.control_cfg.drv_type = axi_pkg::MASTER;
        generator_cfg.control_cfg.reset_level = axi_pkg::RESET_LEVEL_LOW;
        uvm_config_db#(axi_pkg::axi_cfg)::set(this, "axi_lite_control*", "cfg", generator_cfg.control_cfg);
        axi_lite_control = axi_pkg::axi_env#(4,4,4,3,1)::type_id::create("axi_lite_control", this);



        refm = generator_reference_model::type_id::create("refm", this);


        uvm_config_db#(generator_reference_model)::set(this, "*", "refm", refm);


        `uvm_info(this.get_full_name(), "set reference model by uvm_config_db", UVM_LOW)


        subsys_mon = generator_subsystem_monitor::type_id::create("subsys_mon", this);


        generator_virtual_sqr = generator_virtual_sequencer::type_id::create("generator_virtual_sqr", this);
        `uvm_info(this.get_full_name(), "build_phase done", UVM_LOW)
    endfunction


    function void generator_env::connect_phase(uvm_phase phase);
        super.connect_phase(phase);


        generator_virtual_sqr.svr_port_out_r_sqr = env_slave_svr_out_r.s_agt.sqr;
        env_slave_svr_out_r.s_agt.mon.item_collect_port.connect(subsys_mon.svr_slave_out_r_imp);
 
        if(generator_cfg.control_cfg.drv_type==axi_pkg::MASTER ||generator_cfg.control_cfg.drv_type==axi_pkg::SLAVE)
            generator_virtual_sqr.control_sqr = axi_lite_control.vsqr;
        axi_lite_control.item_wtr_port.connect(subsys_mon.control_wtr_imp);
        axi_lite_control.item_rtr_port.connect(subsys_mon.control_rtr_imp);
        refm.generator_cfg = generator_cfg;
        `uvm_info(this.get_full_name(), "connect phase done", UVM_LOW)
    endfunction


    task generator_env::run_phase(uvm_phase phase);
        `uvm_info(this.get_full_name(), "generator_env is running", UVM_LOW)
    endtask


`endif
