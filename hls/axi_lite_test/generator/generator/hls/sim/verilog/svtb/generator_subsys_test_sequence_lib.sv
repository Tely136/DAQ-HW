//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2026.1 (64-bit)
//Tool Version Limit: 2026.06
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2026 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`ifndef GENERATOR_SUBSYS_TEST_SEQUENCE_LIB__SV                                              
    `define GENERATOR_SUBSYS_TEST_SEQUENCE_LIB__SV                                          
                                                                                                    
                                                                                                    
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
                                                                                                    
    class generator_subsys_test_sequence_lib extends uvm_sequence;                                
                                                                                                    
        function new (string name = "generator_subsys_test_sequence_lib");                      
            super.new(name);                                                                        
            `uvm_info(this.get_full_name(), "new is called", UVM_LOW)                             
        endfunction                                                                                 
                                                                                                    
        `uvm_object_utils(generator_subsys_test_sequence_lib)                                     
        `uvm_declare_p_sequencer(generator_virtual_sequencer)                                     
                                                                                                    
        virtual task body();                                                                        
            uvm_phase starting_phase;                                                               
            virtual interface misc_interface misc_if;                                               
            generator_reference_model refm;                                                       
                                                                                                    
            svr_pkg::svr_slave_sequence #(32) svr_port_out_r_seq;            

            axi_pkg::axi_busdatas_master_sequence#(4, 32) axi_master_wr_control_seq;
            axi_pkg::axi_busdatas_master_sequence#(4, 32) axi_master_poll_control_seq;

            if (!uvm_config_db#(generator_reference_model)::get(p_sequencer,"", "refm", refm))
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
                            `uvm_create_on(svr_port_out_r_seq, p_sequencer.svr_port_out_r_sqr);
                            svr_port_out_r_seq.misc_if = refm.misc_if;
                            svr_port_out_r_seq.ap_done  = refm.ap_done_for_nexttrans ;
                            svr_port_out_r_seq.ap_ready = refm.ap_ready_for_nexttrans;
                            svr_port_out_r_seq.finish   = refm.finish;
                            svr_port_out_r_seq.isusr_delay = svr_pkg::NO_DELAY;
                            `uvm_send(svr_port_out_r_seq);     
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

            p_sequencer.svr_port_out_r_sqr.stop_sequences();
            p_sequencer.control_sqr.stop_sequences();
            disable fork;
                                                                                                    
            starting_phase.drop_objection(this);                                                    
                                                                                                    
        endtask                                                                                     
    endclass                                                                                        
                                                                                                    
`endif                                                                                              
