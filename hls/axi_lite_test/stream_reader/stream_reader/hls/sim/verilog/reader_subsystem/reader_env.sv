//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`ifndef READER_ENV__SV                                                                                   
    `define READER_ENV__SV                                                                               
                                                                                                                    
                                                                                                                    
    class reader_env extends uvm_env;                                                                          
                                                                                                                    
        reader_virtual_sequencer reader_virtual_sqr;                                                      
        reader_config reader_cfg;                                                                         
                                                                                                                    
        svr_pkg::svr_env#(32) env_master_svr_in_r;
        axi_pkg::axi_env#(64,4,8,3,1) axi_master_gmem;
        axi_pkg::axi_env#(5,4,4,3,1) axi_lite_control;
                                                                                                                    
        reader_reference_model   refm;                                                                         
                                                                                                                    
        reader_subsystem_monitor subsys_mon;                                                                   
                                                                                                                    
        `uvm_component_utils_begin(reader_env)                                                                 
        `uvm_field_object (env_master_svr_in_r,  UVM_DEFAULT | UVM_REFERENCE)
        `uvm_field_object (refm, UVM_DEFAULT | UVM_REFERENCE)                                                       
        `uvm_field_object (reader_virtual_sqr, UVM_DEFAULT | UVM_REFERENCE)                                    
        `uvm_field_object (reader_cfg        , UVM_DEFAULT)                                                    
        `uvm_component_utils_end                                                                                    
                                                                                                                    
        function new (string name = "reader_env", uvm_component parent = null);                              
            super.new(name, parent);                                                                                
        endfunction                                                                                                 
                                                                                                                    
        extern virtual function void build_phase(uvm_phase phase);                                                  
        extern virtual function void connect_phase(uvm_phase phase);                                                
        extern virtual task          run_phase(uvm_phase phase);                                                    
                                                                                                                    
    endclass                                                                                                        
                                                                                                                    
    function void reader_env::build_phase(uvm_phase phase);                                                    
        super.build_phase(phase);                                                                                   
        reader_cfg = reader_config::type_id::create("reader_cfg", this);                           
                                                                                                                    
        reader_cfg.port_in_r_cfg.svr_type = svr_pkg::SVR_MASTER ;
        env_master_svr_in_r  = svr_env#(32)::type_id::create("env_master_svr_in_r", this);
        uvm_config_db#(svr_pkg::svr_config)::set(this, "env_master_svr_in_r*", "cfg", reader_cfg.port_in_r_cfg);
        reader_cfg.port_in_r_cfg.prt_type = svr_pkg::AXIS;
        reader_cfg.port_in_r_cfg.is_active = svr_pkg::SVR_ACTIVE;
        reader_cfg.port_in_r_cfg.spec_cfg = svr_pkg::NORMAL;
        reader_cfg.port_in_r_cfg.reset_level = svr_pkg::RESET_LEVEL_LOW;
 

        reader_cfg.gmem_cfg.set_default();
        reader_cfg.gmem_cfg.drv_type = axi_pkg::SLAVE;
        reader_cfg.gmem_cfg.reset_level = axi_pkg::RESET_LEVEL_LOW;
        reader_cfg.gmem_cfg.write_latency_mode = TRANSACTION_FIRST;
        reader_cfg.gmem_cfg.read_latency_mode = TRANSACTION_FIRST;
        uvm_config_db#(axi_pkg::axi_cfg)::set(this, "axi_master_gmem*", "cfg", reader_cfg.gmem_cfg);
        axi_master_gmem = axi_pkg::axi_env#(64,4,8,3,1)::type_id::create("axi_master_gmem", this);

        reader_cfg.control_cfg.set_default();
        reader_cfg.control_cfg.drv_type = axi_pkg::MASTER;
        reader_cfg.control_cfg.reset_level = axi_pkg::RESET_LEVEL_LOW;
        uvm_config_db#(axi_pkg::axi_cfg)::set(this, "axi_lite_control*", "cfg", reader_cfg.control_cfg);
        axi_lite_control = axi_pkg::axi_env#(5,4,4,3,1)::type_id::create("axi_lite_control", this);



        refm = reader_reference_model::type_id::create("refm", this);


        uvm_config_db#(reader_reference_model)::set(this, "*", "refm", refm);


        `uvm_info(this.get_full_name(), "set reference model by uvm_config_db", UVM_LOW)


        subsys_mon = reader_subsystem_monitor::type_id::create("subsys_mon", this);


        reader_virtual_sqr = reader_virtual_sequencer::type_id::create("reader_virtual_sqr", this);
        `uvm_info(this.get_full_name(), "build_phase done", UVM_LOW)
    endfunction


    function void reader_env::connect_phase(uvm_phase phase);
        super.connect_phase(phase);


        reader_virtual_sqr.svr_port_in_r_sqr = env_master_svr_in_r.m_agt.sqr;
        env_master_svr_in_r.m_agt.mon.item_collect_port.connect(subsys_mon.svr_master_in_r_imp);
 
        if(reader_cfg.gmem_cfg.drv_type==axi_pkg::MASTER ||reader_cfg.gmem_cfg.drv_type==axi_pkg::SLAVE)
            reader_virtual_sqr.gmem_sqr = axi_master_gmem.vsqr;
        axi_master_gmem.item_wtr_port.connect(subsys_mon.gmem_wtr_imp);
        axi_master_gmem.item_rtr_port.connect(subsys_mon.gmem_rtr_imp);
        uvm_callbacks#(axi_pkg::axi_state, axi_pkg::axi_state_cbs)::add(axi_master_gmem.state, refm.axi_memaccess_cb_gmem);
        if(reader_cfg.control_cfg.drv_type==axi_pkg::MASTER ||reader_cfg.control_cfg.drv_type==axi_pkg::SLAVE)
            reader_virtual_sqr.control_sqr = axi_lite_control.vsqr;
        axi_lite_control.item_wtr_port.connect(subsys_mon.control_wtr_imp);
        axi_lite_control.item_rtr_port.connect(subsys_mon.control_rtr_imp);
        refm.reader_cfg = reader_cfg;
        `uvm_info(this.get_full_name(), "connect phase done", UVM_LOW)
    endfunction


    task reader_env::run_phase(uvm_phase phase);
        `uvm_info(this.get_full_name(), "reader_env is running", UVM_LOW)
    endtask


`endif
