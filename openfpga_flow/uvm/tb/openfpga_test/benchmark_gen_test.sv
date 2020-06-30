`ifndef BENCHMARK_GEN_TEST
`define BENCHMARK_GEN_TEST

class benchmark_gen_test extends openfpga_base_test;


   `uvm_component_utils(benchmark_gen_test)  

   /**
    * Default constructor.
    */
   extern function new(string name="benchmark_gen_test", uvm_component parent=null);
   
   /**
    * Launch sequences
    */
   extern task run_phase(uvm_phase phase);

endclass : benchmark_gen_test

function benchmark_gen_test::new(string name = "benchmark_gen_test",uvm_component parent=null);

      super.new(name,parent);
      uvm_config_db #(string) 	:: set (null, "uvm_test_top", "test_name", "benchmark"); // Used to configure the reference model
											 // And disable scoreboarding operations

endfunction : new

task benchmark_gen_test::run_phase(uvm_phase phase);
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
			m_prog_seq_gen.start(m_openfpga_env.m_bs_agent.m_bs_sqr);
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
  
    
`endif // benchmark_gen_test
