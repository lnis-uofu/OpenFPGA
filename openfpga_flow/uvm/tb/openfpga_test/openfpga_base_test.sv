`ifndef OPENFPGA_BASE_TEST
`define OPENFPGA_BASE_TEST

class openfpga_base_test extends uvm_test;

  rand test_cfg			m_test_cfg 		;
  rand openfpga_env_cfg		m_openfpga_env_cfg  	;
  rand openfpga_env_cntxt	m_openfpga_env_cntxt	;

  openfpga_env      		m_openfpga_env		;
  reset_at_start 		m_reset_at_start	;
  reset_after_bs 		m_reset_after_bs	;
  bitstream_andgate		m_bitstream_andgate	;
  prog_seq_gen			m_prog_seq_gen		;
  stimuli_standard_seq  	m_stimuli_standard_seq  ;

 
   `uvm_component_utils_begin(openfpga_base_test)
      `uvm_field_object(m_test_cfg , UVM_DEFAULT)
      `uvm_field_object(m_openfpga_env_cfg  , UVM_DEFAULT)
      `uvm_field_object(m_openfpga_env_cntxt, UVM_DEFAULT)
   `uvm_component_utils_end  

  constraint openfpga_env_cfg_cons {
      m_openfpga_env_cfg.enabled         == 1;
     // m_openfpga_env_cfg.is_active       == UVM_ACTIVE;
     // m_openfpga_env_cfg.trn_log_enabled == 1;
   }
   
   constraint test_type_default_cons {
     soft m_test_cfg.tpt == NO_TEST_PROGRAM;
   }

   /**
    * Default constructor.
    */
   extern function new(string name="openfpga_base_test", uvm_component parent=null);
   
   /**
    * Builds all components via create_<x>()
    */
   extern virtual function void build_phase(uvm_phase phase);
    /**
    * Indicates to the test bench (uvmt_cv32_tb) that the test has completed.
    * This is done by checking the properties of the phase argument.
    */
   extern virtual function void phase_ended(uvm_phase phase);  


endclass : openfpga_base_test
 
function openfpga_base_test::new(string name = "openfpga_base_test",uvm_component parent=null);

      super.new(name,parent);

endfunction : new

function void openfpga_base_test::build_phase(uvm_phase phase);

      super.build_phase(phase);

      m_test_cfg = test_cfg ::type_id::create("test_cfg");
      m_openfpga_env_cfg  = openfpga_env_cfg  ::type_id::create("openfpga_env_cfg" );
      m_openfpga_env_cntxt= openfpga_env_cntxt::type_id::create("openfpga_env_cntxt");

  m_test_cfg.process_cli_args();

      uvm_config_db#(test_cfg)	::set(this, "*", "cfg",			m_test_cfg);
      uvm_config_db#(openfpga_env_cfg)  ::set(this, "openfpga_env", "cfg"  , m_openfpga_env_cfg);
      uvm_config_db#(openfpga_env_cntxt)::set(this, "openfpga_env", "cntxt", m_openfpga_env_cntxt);

      m_openfpga_env		=openfpga_env		::type_id::create("openfpga_env",this);
      m_reset_at_start		=reset_at_start		::type_id::create("Reset at start");
      m_reset_after_bs		=reset_after_bs		::type_id::create("Reset after bitstream is loaded");
      m_bitstream_andgate	=bitstream_andgate	::type_id::create("Bitstream sequence for andgate configuration");
      m_prog_seq_gen		=prog_seq_gen		::type_id::create("Configuration sequence for generated benchmark");
      m_stimuli_standard_seq    =stimuli_standard_seq	::type_id::create("stimuli");

endfunction:build_phase

function void openfpga_base_test::phase_ended(uvm_phase phase);

   super.phase_ended(phase);
   
   if (phase.is(uvm_final_phase::get())) begin // change phase to the one after run_phase
     // Set sim_finished (otherwise tb will flag that sim was aborted)
     uvm_config_db#(bit)::set(null, "", "sim_finished", 1);
   end
   
endfunction : phase_ended
    
`endif // OPENFPGA_BASE_TEST
