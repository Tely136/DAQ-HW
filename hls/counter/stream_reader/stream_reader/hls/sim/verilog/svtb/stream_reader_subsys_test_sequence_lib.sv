//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`ifndef STREAM_READER_SUBSYS_TEST_SEQUENCE_LIB__SV                                              
    `define STREAM_READER_SUBSYS_TEST_SEQUENCE_LIB__SV                                          
                                                                                                    
    `define AUTOTB_TVIN_data_in_data_in_TDATA  "../tv/cdatafile/c.stream_reader.autotvin_data_in.dat" 
                                                                                                    
    `include "uvm_macros.svh"                                                                     
                                                                                                    
    // Delay kinds used in this test sequence                                                       
    typedef enum int {                                                                              
        TB_DELAY_NONE,                                                                              
        DELAY_DIRECTIO_BEFORE_FORCE,  // delay before force used in directio on s_axilite           
        DELAY_MAXI                    // delay for m_axi to make sure mem incr_rd_page_idx is called first
    } tb_delay_e;                                                                                   
                                                                                                    
    // Delay application task                                                                       
    task automatic tb_apply_delay(tb_delay_e kind);                                                 
        case (kind)                                                                                 
            TB_DELAY_NONE:                     ;         // no delay                                
            DELAY_DIRECTIO_BEFORE_FORCE: #0.001;                                                    
            DELAY_MAXI:                  #0.001;                                                    
            default:                           ;                                                    
        endcase                                                                                     
    endtask                                                                                         
                                                                                                    
    class stream_reader_subsys_test_sequence_lib extends uvm_sequence;                                
                                                                                                    
        function new (string name = "stream_reader_subsys_test_sequence_lib");                      
            super.new(name);                                                                        
            `uvm_info(this.get_full_name(), "new is called", UVM_LOW)                             
        endfunction                                                                                 
                                                                                                    
        `uvm_object_utils(stream_reader_subsys_test_sequence_lib)                                     
        `uvm_declare_p_sequencer(stream_reader_virtual_sequencer)                                     
                                                                                                    
        virtual task body();                                                                        
            uvm_phase starting_phase;                                                               
            virtual interface misc_interface misc_if;                                               
            stream_reader_reference_model refm;                                                       
                                                                                                    
            string file_queue_data_in [$];                                                         
            integer bitwidth_queue_data_in [$];                                                    
                                                                                                               
            svr_pkg::svr_master_sequence#(32) svr_port_data_in_seq;            
            svr_pkg::svr_random_sequence#(32) svr_port_random_port_data_in_seq;

            axi_pkg::axi_slave_sequence#(64,4,8,3,1) axi_slave_gmem_seq;
            axi_pkg::axi_busdatas_master_sequence#(6, 32) axi_master_wr_control_seq;
            axi_pkg::axi_busdatas_master_sequence#(6, 32) axi_master_poll_control_seq;

            if (!uvm_config_db#(stream_reader_reference_model)::get(p_sequencer,"", "refm", refm))
                `uvm_fatal(this.get_full_name(), "No reference model")
            `uvm_info(this.get_full_name(), "get reference model by uvm_config_db", UVM_LOW)

            `uvm_info(this.get_full_name(), "body is called", UVM_LOW)
            starting_phase = this.get_starting_phase();
            if (starting_phase != null) begin
                `uvm_info(this.get_full_name(), "starting_phase not null", UVM_LOW)
                starting_phase.raise_objection(this);
            end
            else
                `uvm_info(this.get_full_name(), "starting_phase null" , UVM_LOW)

            misc_if = refm.misc_if;


            //phase_done.set_drain_time(this, 0ns);
            wait(refm.misc_if.reset === 1);
            ->refm.misc_if.initialed_evt;

            fork
                begin
                    fork
                        begin
                            string keystr_delay;
                            file_queue_data_in.push_back(`AUTOTB_TVIN_data_in_data_in_TDATA);
                            bitwidth_queue_data_in.push_back(32);

                            `uvm_create_on(svr_port_data_in_seq, p_sequencer.svr_port_data_in_sqr);
                            svr_port_data_in_seq.misc_if = refm.misc_if;
                            svr_port_data_in_seq.ap_done  = refm.ap_done_for_nexttrans ;
                            svr_port_data_in_seq.ap_ready = refm.ap_ready_for_nexttrans;
                            svr_port_data_in_seq.finish   = refm.finish;
                            svr_port_data_in_seq.file_rd.config_file(file_queue_data_in, bitwidth_queue_data_in);
                            if( refm.stream_reader_cfg.port_data_in_cfg.prt_type == AP_VLD ) wait(refm.misc_if.tb2dut_ap_start === 1'b1);
                            svr_port_data_in_seq.isusr_delay = svr_pkg::NO_DELAY;
                            `uvm_send(svr_port_data_in_seq);     
                        end                                               
                        begin //axi slave sequence. loop delays
                            `uvm_create_on(axi_slave_gmem_seq, p_sequencer.gmem_sqr);
                            axi_slave_gmem_seq.misc_if = refm.misc_if;
                            axi_slave_gmem_seq.ap_done    = refm.ap_done_for_nexttrans   ;
                            axi_slave_gmem_seq.ap_ready   = refm.ap_ready_for_nexttrans  ;
                            axi_slave_gmem_seq.finish     = refm.finish ;
                            axi_slave_gmem_seq.isusr_delay = axi_pkg::NO_DELAY;
                            `uvm_send(axi_slave_gmem_seq);
                        end
                        begin
                            int control_page_idx_bak;
                            `uvm_create_on(axi_master_wr_control_seq, p_sequencer.control_sqr);
                            axi_master_wr_control_seq.misc_if = refm.misc_if;
                            axi_master_wr_control_seq.ap_done    = refm.ap_done_for_nexttrans   ;
                            axi_master_wr_control_seq.ap_ready   = refm.ap_ready_for_nexttrans  ;
                            axi_master_wr_control_seq.finish     = refm.finish ;
                            axi_master_wr_control_seq.isusr_delay = axi_pkg::NO_DELAY;
                            for(int i=0; i<1; i++) begin
                                logic[63:0] data64bit_data_out[$];
                                logic[32-1:0] databusbit_data_out[$];
                                logic[63:0] data64bit_n_outer[$];
                                logic[32-1:0] databusbit_n_outer[$];
                                logic[63:0] data64bit_n_inner[$];
                                logic[32-1:0] databusbit_n_inner[$];
                                data64bit_data_out.delete(); databusbit_data_out.delete();
                                axi_master_wr_control_seq.StableAxiliteNoUpdate=0;
                                for(int j=0; j < (64+32-1)/32; j++) begin
                                    data64bit_data_out.push_back( ((refm.mem_blk_pages_gmem.maxi_bundlevar_offset["data_out"]+refm.mem_blk_pages_gmem.page_ofst[refm.mem_blk_pages_gmem.rd_page_idx])>>(j*32)) & (2**32-1) );
                                end
                                foreach(data64bit_data_out[s]) databusbit_data_out[s]=data64bit_data_out[s][32-1:0];
                                axi_master_wr_control_seq.StableAxiliteNoUpdate=1;
                                axi_master_wr_control_seq.datamerge_inavg(databusbit_data_out, 0, 16, 1);
                                data64bit_n_outer.delete(); databusbit_n_outer.delete();
                                axi_master_wr_control_seq.StableAxiliteNoUpdate=0;
                                refm.mem_blk_pages_control_n_outer.tobusdata(data64bit_n_outer, refm.mem_blk_pages_control_n_outer.rd_page_idx, 32);
                                foreach(data64bit_n_outer[s]) databusbit_n_outer[s]=data64bit_n_outer[s][32-1:0];
                                axi_master_wr_control_seq.StableAxiliteNoUpdate=1;
                                axi_master_wr_control_seq.datamerge_inavg(databusbit_n_outer, 0, 28, 1);
                                data64bit_n_inner.delete(); databusbit_n_inner.delete();
                                axi_master_wr_control_seq.StableAxiliteNoUpdate=0;
                                refm.mem_blk_pages_control_n_inner.tobusdata(data64bit_n_inner, refm.mem_blk_pages_control_n_inner.rd_page_idx, 32);
                                foreach(data64bit_n_inner[s]) databusbit_n_inner[s]=data64bit_n_inner[s][32-1:0];
                                axi_master_wr_control_seq.StableAxiliteNoUpdate=1;
                                axi_master_wr_control_seq.datamerge_inavg(databusbit_n_inner, 0, 36, 1);
                                `uvm_send(axi_master_wr_control_seq);
                                refm.write_data_finish_control = 1;
                                `uvm_info("control data writting thread", $sformatf("%0dth(total 1): waiting for all write data finish event",i), UVM_LOW)
                                wait(refm.allaxilite_write_data_finish.triggered);
                                refm.write_data_finish_control = 0;
                                fork
                                    begin // configure start to enable DUT
                                        axi_master_wr_control_seq.wr_addr_data.push_back( (1<<0)+(0<<32) );
                                        `uvm_info("control start dut by axilite", $sformatf("%0dth(total 1): begin to set start bit",i), UVM_LOW)
                                        `uvm_send(axi_master_wr_control_seq);
                                    end // end of configuration loop
                                    begin
                                        `uvm_info("control wait for ap_ready for next trans", $sformatf("%0dth(total 1): begin to wait",i), UVM_LOW)
                                        wait(refm.dut2tb_ap_ready.triggered);
                                        wait(refm.ap_done_for_nexttrans.triggered);
                                        tb_apply_delay(DELAY_MAXI); //make sure mem incr_rd_page_idx is called first
                                    end
                                join
                            end
                        end
                        begin
                            for(int j=0; j<1; j=j+refm.ap_done_cnt) begin
                                wait(misc_if.dut2tb_ap_done_kernel == 1);
                                `uvm_info("test finish control", $sformatf("ap_done of kernel is triggered"), UVM_LOW)
                                @(posedge misc_if.clock);
                                fork
                                    forever begin
                                        `uvm_create_on(axi_master_poll_control_seq, p_sequencer.control_sqr);
                                        axi_master_poll_control_seq.isusr_delay = axi_pkg::NO_DELAY;
                                        axi_master_poll_control_seq.misc_if = refm.misc_if;
                                        axi_master_poll_control_seq.rd_addr.push_back(0);
                                        `uvm_send(axi_master_poll_control_seq)
                                        repeat(2) @(posedge misc_if.clock);
                                    end
                                    begin
                                        `uvm_info("test finish control", $sformatf("%0dth(total 1) ap_done_for_nexttrans begin to wait",j), UVM_LOW)
                                        @refm.dut2tb_ap_done;
                                    end
                                join_any
                                disable fork;
                                wait(refm.ap_ready_for_nexttrans.triggered);
                            end
                        end
                        begin
                            wait(svr_port_data_in_seq);
                            forever begin
                                wait(svr_port_data_in_seq.one_sect_read);
                                svr_port_data_in_seq.one_sect_read = 0;
                                -> refm.allsvr_input_done;
                            end
                        end
                    join
                end

                begin
                    for(int j=0; j<1; j=j+refm.ap_done_cnt) @refm.ap_done_for_nexttrans;
                    `uvm_info(this.get_full_name(), "autotb finished", UVM_LOW)
                    -> refm.finish;
                    refm.misc_if.finished = 1;
                    @(posedge refm.misc_if.clock);
                    refm.misc_if.finished = 0;
                    @(posedge refm.misc_if.clock);
                    -> refm.misc_if.finished_evt;
                end
            join_any
            repeat(5) @(posedge refm.misc_if.clock); //5 cycles delay for finish stuff. 5 is haphazard value

            p_sequencer.svr_port_data_in_sqr.stop_sequences();
            p_sequencer.gmem_sqr.stop_sequences();
            p_sequencer.control_sqr.stop_sequences();
            disable fork;
                                                                                                    
            starting_phase.drop_objection(this);                                                    
                                                                                                    
        endtask                                                                                     
    endclass                                                                                        
                                                                                                    
`endif                                                                                              
