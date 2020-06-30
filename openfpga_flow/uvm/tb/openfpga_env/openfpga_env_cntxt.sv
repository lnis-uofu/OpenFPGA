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


`ifndef OPENFPGA_ENV_CNTXT
`define OPENFPGA_ENV_CNTXT


/**
 * Object encapsulating all state variables OpenFPGA verification
 * openfpga_environment.
 */
class openfpga_env_cntxt extends uvm_object;
   
   // Agent context handles
   clknrst_cntxt  m_clknrst_cntxt;
   bs_cntxt	  m_bs_cntxt;
   stimuli_cntxt  m_stimuli_cntxt;
   
   // TODO Add scoreboard context handles
   //      Ex: uvme_cv32_sb_cntxt_c  sb_egress_cntxt;
   //          uvme_cv32_sb_cntxt_c  sb_ingress_cntxt;
   
   // Events
   uvm_event  config_done;
   uvm_event  stim_done;
   
   
   `uvm_object_utils_begin(openfpga_env_cntxt)
      `uvm_field_object(m_clknrst_cntxt ,UVM_DEFAULT)
      `uvm_field_object(m_bs_cntxt	,UVM_DEFAULT)
      `uvm_field_object(m_stimuli_cntxt ,UVM_DEFAULT)
      
      // TODO Add scoreboard context field macros
      //      Ex: `uvm_field_object(sb_egress_cntxt , UVM_DEFAULT)
      //          `uvm_field_object(sb_ingress_cntxt, UVM_DEFAULT)
      
      `uvm_field_event(config_done  , UVM_DEFAULT)
      `uvm_field_event(stim_done, UVM_DEFAULT)
   `uvm_object_utils_end
   
   
   /**
    * Builds events and sub-context objects.
    */
   extern function new(string name="uvme_cv32_cntxt");
   
endclass : openfpga_env_cntxt


function openfpga_env_cntxt::new(string name="uvme_cv32_cntxt");
   
   super.new(name);
   
   m_clknrst_cntxt = clknrst_cntxt::type_id::create("clknrst_cntxt");
   m_bs_cntxt	   = bs_cntxt     ::type_id::create("bs_cntxt");
   m_stimuli_cntxt = stimuli_cntxt::type_id::create("stimuli_cntxt");
   // TODO Create uvme_cv32_cntxt_c scoreboard context objects
   //      Ex: sb_egress_cntxt  = uvma_cv32_sb_cntxt_c::type_id::create("sb_egress_cntxt" );
   //          sb_ingress_cntxt = uvma_cv32_sb_cntxt_c::type_id::create("sb_ingress_cntxt");
   
   config_done   = new("config_done"  );
   stim_done	 = new("stim_done");
   
endfunction : new


`endif // OPENFPGA_ENV_CNTXT
