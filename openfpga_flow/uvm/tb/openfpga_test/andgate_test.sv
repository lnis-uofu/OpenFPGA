`ifndef ANDGATE_TEST
`define ANDGATE_TEST

class andgate_test extends openfpga_base_test;

    
   `uvm_component_utils(andgate_test)  



   /**
    * Default constructor.
    */
   extern function new(string name="andgate_test", uvm_component parent=null);
   
   /**
    * Launch sequences
    */
   extern task run_phase(uvm_phase phase);

endclass : andgate_test

function andgate_test::new(string name = "andgate_test",uvm_component parent=null);

      super.new(name,parent);
      uvm_config_db #(string) :: set (null, "uvm_test_top", "test_name", "andgate"); // Used to configure the reference model.
endfunction : new

task andgate_test::run_phase(uvm_phase phase);
      phase.raise_objection(this);

// Enable pReset for few cycle, and Reset until programming phase is done
      	begin
      		m_reset_at_start.start(m_openfpga_env.m_clknrst_agent.m_clknrst_sqr);
      	end
// Launch programming phase clock & programming sequence
	fork
		begin
			m_openfpga_env_cntxt.m_clknrst_cntxt.vif.start_bs_clk();
		end
		begin
			m_bitstream_andgate.start(m_openfpga_env.m_bs_agent.m_bs_sqr);
		end
	join
// Stop Prog_Clock
	begin
	        m_openfpga_env_cntxt.m_clknrst_cntxt.vif.stop_bs_clk();
	end
// Reset before operating phase
        begin
     		m_reset_after_bs.start(m_openfpga_env.m_clknrst_agent.m_clknrst_sqr);
        end
// Launch operating clock & operating sequence
	begin
		m_openfpga_env_cntxt.m_clknrst_cntxt.vif.start_clk();
	end
      	begin              
    		m_stimuli_standard_seq.start(m_openfpga_env.m_stimuli_agent.m_stimuli_sqr); 
      	end
        $display("Simulation Succeed");
        uvm_config_db#(bit)::set(null, "", "sim_finished", 1);
      phase.drop_objection(this);
endtask:run_phase
  
    
`endif // OPENFPGA_CC_TEST
