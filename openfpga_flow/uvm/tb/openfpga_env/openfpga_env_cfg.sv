// Copyright 2020 OpenHW Group
// Copyright 2020 Datum Technology Corporation
// 
// Licensed under the Solderpad Hardware Licence, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     https://solderpad.org/licenses/
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


`ifndef OPENFPGA_ENV_CFG
`define OPENFPGA_ENV_CFG


/**
 * Object encapsulating all parameters for creating, connecting and running
 * OpenFPGA verification openfpga_environment (openfpga_env) components.
 */
class openfpga_env_cfg extends uvm_object;
   
   // Integrals
 

   rand bit			 enabled;
   rand uvm_active_passive_enum  is_active;
   rand bit                      scoreboarding_enabled ;
   rand bit                      cov_model_enabled;
   rand bit                      trn_log_enabled;
   rand int unsigned             sys_clk_period;
   rand which_bitstream		 m_which_bitstream;
   
   
   // Agent cfg handles
   rand clknrst_cfg  m_clknrst_cfg;
   rand bs_cfg	     m_bs_cfg;
   rand stimuli_cfg  m_stimuli_cfg;
   
   // Agent cntxt handles
   rand clknrst_cntxt m_clknrst_cntxt;
   rand bs_cntxt      m_bs_cntxt;
   rand stimuli_cntxt m_stimuli_cntxt;

   rand openfpga_ral  m_openfpga_ral;
   
   // Test_case to enable/disable scoreboard
   string test_name;
   // Objects
   // TODO Add scoreboard configuration handles
   //      Ex: rand uvml_sb_cfg_c  sb_egress_cfg;
   //          rand uvml_sb_cfg_c  sb_ingress_cfg;
   
   
   `uvm_object_utils_begin(openfpga_env_cfg)
      `uvm_field_int (                         enabled                     , UVM_DEFAULT          )
      `uvm_field_enum(uvm_active_passive_enum, is_active                   , UVM_DEFAULT          )
      `uvm_field_enum(which_bitstream	     , m_which_bitstream           , UVM_DEFAULT          )
      `uvm_field_int (                         scoreboarding_enabled       , UVM_DEFAULT          )
      `uvm_field_int (                         cov_model_enabled           , UVM_DEFAULT          )
      `uvm_field_int (                         trn_log_enabled             , UVM_DEFAULT          )
      `uvm_field_int (                         sys_clk_period            , UVM_DEFAULT + UVM_DEC)
      
      `uvm_field_object(m_clknrst_cfg	, UVM_DEFAULT)
      `uvm_field_object(m_bs_cfg  	, UVM_DEFAULT)
      `uvm_field_object(m_stimuli_cfg   , UVM_DEFAULT)
      
   `uvm_object_utils_end
   
   constraint defaults_cons {
      soft enabled                == 1;
      soft is_active              == UVM_ACTIVE;
      soft scoreboarding_enabled  == 1;
      soft cov_model_enabled      == 0;
      soft trn_log_enabled        == 0;
      soft sys_clk_period         == uvme_cv32_sys_default_clk_period; // see openfpga_env_constants.sv
   }
   
   constraint scoreboard_control {
      if (test_name == "benchmark"){
	scoreboarding_enabled == 0;
	}
   }

   constraint agent_cfg_cons {
      if (enabled) {
         m_clknrst_cfg.enabled == 0;
         m_bs_cfg.enabled      == 1;
         m_stimuli_cfg.enabled == 1;
      }
      
      if (is_active == UVM_ACTIVE) {
         m_clknrst_cfg.is_active == UVM_ACTIVE;
         m_bs_cfg.is_active 	 == UVM_ACTIVE;
         m_stimuli_cfg.is_active == UVM_ACTIVE;
      }
      
      if (trn_log_enabled) {
         m_clknrst_cfg.trn_log_enabled == 1;
         m_bs_cfg.trn_log_enabled      == 1;
         m_stimuli_cfg.trn_log_enabled == 1;
      }
   }
   
   
   /**
    * Creates sub-configuration objects.
    */
   extern function new(string name="openfpga_env_cfg");
   /**
    * Adapt reference model to bitstream
    */
   //extern function which_bitstream();
endclass : openfpga_env_cfg


function openfpga_env_cfg::new(string name="openfpga_env_cfg");
   
   super.new(name);
   
   m_clknrst_cfg   = clknrst_cfg::type_id::create("clknrst_cfg");
   m_clknrst_cntxt = clknrst_cntxt::type_id::create("clknrst_cntxt");
   m_bs_cfg        = bs_cfg::type_id::create("clknrst_cfg");
   m_bs_cntxt      = bs_cntxt::type_id::create("clknrst_cntxt");
   m_stimuli_cfg   = stimuli_cfg::type_id::create("stimuli_cfg");
   m_stimuli_cntxt = stimuli_cntxt::type_id::create("stimuli_cntxt");

   m_openfpga_ral = openfpga_ral::type_id::create("ral");
   m_openfpga_ral.build();
   m_openfpga_ral.lock_model();

    if (uvm_config_db #(string) :: get (null, "uvm_test_top", "test_name", test_name))
      `uvm_info ("ENV", $sformatf ("Found %s", test_name), UVM_MEDIUM)

endfunction : new

//function openfpga_env_cfg::which_bitstream();
	



`endif // OPENFPGA_ENV_CFG
