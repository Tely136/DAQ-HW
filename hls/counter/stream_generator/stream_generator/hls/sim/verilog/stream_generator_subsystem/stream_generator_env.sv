//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`ifndef STREAM_GENERATOR_ENV__SV                                                                                   
    `define STREAM_GENERATOR_ENV__SV                                                                               
                                                                                                                    
                                                                                                                    
    class stream_generator_env extends uvm_env;                                                                          
                                                                                                                    
        stream_generator_virtual_sequencer stream_generator_virtual_sqr;                                                      
        stream_generator_config stream_generator_cfg;                                                                         
                                                                                                                    
        svr_pkg::svr_env#(32) env_slave_svr_out_r;
        axi_pkg::axi_env#(64,4,8,3,1) axi_master_gmem;
        axi_pkg::axi_env#(6,4,4,3,1) axi_lite_control;
                                                                                                                    
        stream_generator_reference_model   refm;                                                                         
                                                                                                                    
        stream_generator_subsystem_monitor subsys_mon;                                                                   
                                                                                                                    
        `uvm_component_utils_begin(stream_generator_env)                                                                 
        `uvm_field_object (env_slave_svr_out_r,  UVM_DEFAULT | UVM_REFERENCE)
        `uvm_field_object (refm, UVM_DEFAULT | UVM_REFERENCE)                                                       
        `uvm_field_object (stream_generator_virtual_sqr, UVM_DEFAULT | UVM_REFERENCE)                                    
        `uvm_field_object (stream_generator_cfg        , UVM_DEFAULT)                                                    
        `uvm_component_utils_end                                                                                    
                                                                                                                    
        function new (string name = "stream_generator_env", uvm_component parent = null);                              
            super.new(name, parent);                                                                                
        endfunction                                                                                                 
                                                                                                                    
        extern virtual function void build_phase(uvm_phase phase);                                                  
        extern virtual function void connect_phase(uvm_phase phase);                                                
        extern virtual task          run_phase(uvm_phase phase);                                                    
                                                                                                                    
    endclass                                                                                                        
                                                                                                                    
    function void stream_generator_env::build_phase(uvm_phase phase);                                                    
        super.build_phase(phase);                                                                                   
        stream_generator_cfg = stream_generator_config::type_id::create("stream_generator_cfg", this);                           
                                                                                                                    
        stream_generator_cfg.port_out_r_cfg.svr_type = svr_pkg::SVR_SLAVE ;
        env_slave_svr_out_r  = svr_env#(32)::type_id::create("env_slave_svr_out_r", this);
        uvm_config_db#(svr_pkg::svr_config)::set(this, "env_slave_svr_out_r*", "cfg", stream_generator_cfg.port_out_r_cfg);
        stream_generator_cfg.port_out_r_cfg.prt_type = svr_pkg::AXIS;
        stream_generator_cfg.port_out_r_cfg.is_active = svr_pkg::SVR_ACTIVE;
        stream_generator_cfg.port_out_r_cfg.spec_cfg = svr_pkg::NORMAL;
        stream_generator_cfg.port_out_r_cfg.reset_level = svr_pkg::RESET_LEVEL_LOW;
 

        stream_generator_cfg.gmem_cfg.set_default();
        stream_generator_cfg.gmem_cfg.drv_type = axi_pkg::SLAVE;
        stream_generator_cfg.gmem_cfg.reset_level = axi_pkg::RESET_LEVEL_LOW;
        stream_generator_cfg.gmem_cfg.write_latency_mode = TRANSACTION_FIRST;
        stream_generator_cfg.gmem_cfg.read_latency_mode = TRANSACTION_FIRST;
        uvm_config_db#(axi_pkg::axi_cfg)::set(this, "axi_master_gmem*", "cfg", stream_generator_cfg.gmem_cfg);
        axi_master_gmem = axi_pkg::axi_env#(64,4,8,3,1)::type_id::create("axi_master_gmem", this);

        stream_generator_cfg.control_cfg.set_default();
        stream_generator_cfg.control_cfg.drv_type = axi_pkg::MASTER;
        stream_generator_cfg.control_cfg.reset_level = axi_pkg::RESET_LEVEL_LOW;
        uvm_config_db#(axi_pkg::axi_cfg)::set(this, "axi_lite_control*", "cfg", stream_generator_cfg.control_cfg);
        axi_lite_control = axi_pkg::axi_env#(6,4,4,3,1)::type_id::create("axi_lite_control", this);



        refm = stream_generator_reference_model::type_id::create("refm", this);


        uvm_config_db#(stream_generator_reference_model)::set(this, "*", "refm", refm);


        `uvm_info(this.get_full_name(), "set reference model by uvm_config_db", UVM_LOW)


        subsys_mon = stream_generator_subsystem_monitor::type_id::create("subsys_mon", this);


        stream_generator_virtual_sqr = stream_generator_virtual_sequencer::type_id::create("stream_generator_virtual_sqr", this);
        `uvm_info(this.get_full_name(), "build_phase done", UVM_LOW)
    endfunction


    function void stream_generator_env::connect_phase(uvm_phase phase);
        super.connect_phase(phase);


        stream_generator_virtual_sqr.svr_port_out_r_sqr = env_slave_svr_out_r.s_agt.sqr;
        env_slave_svr_out_r.s_agt.mon.item_collect_port.connect(subsys_mon.svr_slave_out_r_imp);
 
        if(stream_generator_cfg.gmem_cfg.drv_type==axi_pkg::MASTER ||stream_generator_cfg.gmem_cfg.drv_type==axi_pkg::SLAVE)
            stream_generator_virtual_sqr.gmem_sqr = axi_master_gmem.vsqr;
        axi_master_gmem.item_wtr_port.connect(subsys_mon.gmem_wtr_imp);
        axi_master_gmem.item_rtr_port.connect(subsys_mon.gmem_rtr_imp);
        uvm_callbacks#(axi_pkg::axi_state, axi_pkg::axi_state_cbs)::add(axi_master_gmem.state, refm.axi_memaccess_cb_gmem);
        if(stream_generator_cfg.control_cfg.drv_type==axi_pkg::MASTER ||stream_generator_cfg.control_cfg.drv_type==axi_pkg::SLAVE)
            stream_generator_virtual_sqr.control_sqr = axi_lite_control.vsqr;
        axi_lite_control.item_wtr_port.connect(subsys_mon.control_wtr_imp);
        axi_lite_control.item_rtr_port.connect(subsys_mon.control_rtr_imp);
        refm.stream_generator_cfg = stream_generator_cfg;
        `uvm_info(this.get_full_name(), "connect phase done", UVM_LOW)
    endfunction


    task stream_generator_env::run_phase(uvm_phase phase);
        `uvm_info(this.get_full_name(), "stream_generator_env is running", UVM_LOW)
    endtask


`endif
