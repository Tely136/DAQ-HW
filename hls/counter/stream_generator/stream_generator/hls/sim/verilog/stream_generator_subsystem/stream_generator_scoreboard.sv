//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`ifndef STREAM_GENERATOR_SCOREBOARD__SV                                                       
    `define STREAM_GENERATOR_SCOREBOARD__SV                                                   
                                                                                               
    `define AUTOTB_TVOUT_out_r_out_r_TDATA_wrapc  "../tv/rtldatafile/rtl.stream_generator.autotvout_out_r.dat"
    `define STREAM_SIZE_OUT_out_r_out_r_TDATA  "../tv/stream_size/stream_size_out_out_r.dat"
                                                                                               
    class stream_generator_scoreboard extends uvm_component;                                        
                                                                                               
        stream_generator_reference_model refm;                                                      
                                                                                               
        typedef integer TRANS_SIZE_QUEUE_TYPE [$];                                      
        TRANS_SIZE_QUEUE_TYPE TVOUT_transaction_size_queue;                                
        int write_file_done_out_r_out_r_TDATA;                                                          
        int write_section_done_out_r_out_r_TDATA = 0;                                                   
                                                                                           
        file_agent_pkg::file_read_agent#(32) file_rd_TVOUT_transaction_size;               
        file_agent_pkg::file_write_agent#(32) file_wr_port_out_r_out_r_TDATA;
                                                                                               
        `uvm_component_utils_begin(stream_generator_scoreboard)                                     
        `uvm_field_object(refm  , UVM_DEFAULT)                                                 
        `uvm_field_object(file_rd_TVOUT_transaction_size, UVM_DEFAULT)
        `uvm_field_queue_int(TVOUT_transaction_size_queue, UVM_DEFAULT)                    
        `uvm_field_object(file_wr_port_out_r_out_r_TDATA, UVM_DEFAULT)
        `uvm_field_int(write_file_done_out_r_out_r_TDATA, UVM_DEFAULT)
        `uvm_field_int(write_section_done_out_r_out_r_TDATA, UVM_DEFAULT)
        `uvm_component_utils_end                                                               
                                                                                               
        virtual function void build_phase(uvm_phase phase);                                    
            if (!uvm_config_db #(stream_generator_reference_model)::get(this, "", "refm", refm))
                `uvm_fatal(this.get_full_name(), "No refm from high level")                  
            `uvm_info(this.get_full_name(), "get reference model by uvm_config_db", UVM_MEDIUM) 
            file_rd_TVOUT_transaction_size = file_agent_pkg::file_read_agent#(32)::type_id::create("file_rd_TVOUT_transaction_size", this);
                                                                                               
            file_wr_port_out_r_out_r_TDATA = file_agent_pkg::file_write_agent#(32)::type_id::create("file_wr_port_out_r_out_r_TDATA", this);
        endfunction                                                                            
                                                                                               
        function new (string name = "", uvm_component parent = null);                        
            super.new(name, parent);                                                           
            write_file_done_out_r_out_r_TDATA = 0;                                                          
        endfunction                                                                            
                                                                                               
        virtual task run_phase(uvm_phase phase);                                               
            create_TVOUT_transaction_size_queue_by_file(`STREAM_SIZE_OUT_out_r_out_r_TDATA);
            file_wr_port_out_r_out_r_TDATA.config_file(   
                    `AUTOTB_TVOUT_out_r_out_r_TDATA_wrapc,
                    TVOUT_transaction_size_queue                            
                );                                                          
                                                                            

            fork                                                                               
                forever begin
                    @refm.allaxilite_write_data_finish;
                    `uvm_info(this.get_full_name(), "receive allaxilite_write_finish axilite write_mem_page_process", UVM_LOW)
                    void'(refm.mem_blk_pages_control_n_total.pages.pop_front());
                end
                                                                                               
                forever begin
                    @refm.dut2tb_ap_done;
                    `uvm_info(this.get_full_name(), "receive dut2tb_ap_done and do axim dump", UVM_LOW)
                            refm.mem_blk_pages_gmem.tvout_dump_frontpage(0);
                    file_wr_port_out_r_out_r_TDATA.receive_ap_done();
                end                                                                            
                begin                                                                          
                    @refm.finish;                                                              
                    `uvm_info(this.get_full_name(), "receive FINISH", UVM_LOW)               
                    file_wr_port_out_r_out_r_TDATA.wait_write_file_done();
                end                                                                            
                begin                                                                      
                    forever begin                                                              
                        wait(write_section_done_out_r_out_r_TDATA);                          
                        write_section_done_out_r_out_r_TDATA = 0;                                               
                        -> refm.allsvr_output_done;                                         
                    end                                                                        
                end                                                                        
            join                                                                               
        endtask                                                                                
                                                                                               
         virtual function void create_TVOUT_transaction_size_queue_by_file(string TVOUT_file); 
             typedef bit [31: 0] DATA_QUEUE_TYPE [$];                                     
             DATA_QUEUE_TYPE TV_Queue [$];                                                  
             DATA_QUEUE_TYPE TV;                                                               
             string file_queue [$];                                                         
             integer bitwidth_queue [$];                                                    
             file_queue.push_back(TVOUT_file);                                                 
             bitwidth_queue.push_back(32);                                                     
                                                                                               
             file_rd_TVOUT_transaction_size.config_file(                                       
                 file_queue,                                                                   
                 bitwidth_queue                                                                
             );                                                                                
                                                                                               
             file_rd_TVOUT_transaction_size.read_TVIN_file();                                  
                                                                                               
             TV_Queue = file_rd_TVOUT_transaction_size.TV_Queue;                               
             TVOUT_transaction_size_queue.delete();                                            
             while (TV_Queue.size() > 0) begin                                                 
                 TV = TV_Queue.pop_front();                                                    
                 if (TV.size() != 1)                                                           
                         `uvm_fatal(this.get_full_name(), $sformatf("number of each transaction size should be 1, read %0d in file %0s   ", TV.size(), TVOUT_file))
                 `uvm_info(this.get_full_name(), $sformatf("get transaction size %0d", TV[0]), UVM_MEDIUM)
                 TVOUT_transaction_size_queue.push_back(TV.pop_front());                       
             end                                                                               
         endfunction                                                                           
                                                                                               
        virtual function void write_svr_slave_out_r(svr_transfer#(32) tr);
            `uvm_info(this.get_full_name(), "port out_r collected one pkt", UVM_DEBUG);          
            file_wr_port_out_r_out_r_TDATA.write_TVOUT_data(tr.data[31: 0]);
            write_file_done_out_r_out_r_TDATA = file_wr_port_out_r_out_r_TDATA.is_write_file_done();
            write_section_done_out_r_out_r_TDATA = file_wr_port_out_r_out_r_TDATA.is_write_section_done();
            if(write_section_done_out_r_out_r_TDATA) 
                `uvm_info("out_r rx data done", "signal name:out_r_TDATA", UVM_MEDIUM)
        endfunction
                   
        virtual function void write_axi_wtr_gmem(axi_pkg::axi_transfer tr);
        endfunction

        virtual function void write_axi_rtr_gmem(axi_pkg::axi_transfer tr);
        endfunction

        virtual function void write_axi_wtr_control(axi_pkg::axi_transfer tr);
        endfunction

        virtual function void write_axi_rtr_control(axi_pkg::axi_transfer tr);
        endfunction

    endclass                                                                                   
                                                                                               
`endif                                                                                         
