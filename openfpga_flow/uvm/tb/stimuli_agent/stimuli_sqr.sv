// 
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
// 

`ifndef stimuli_SQR
`define stimuli_SQR

class stimuli_sqr extends uvm_sequencer#(
   .REQ(stimuli_seq_item),
   .RSP(stimuli_seq_item)
);
  
   stimuli_cfg    m_stimuli_cfg;
   stimuli_cntxt  m_stimuli_cntxt;
   
   
   `uvm_component_utils_begin(stimuli_sqr)
      `uvm_field_object(m_stimuli_cfg  , UVM_DEFAULT)
      `uvm_field_object(m_stimuli_cntxt, UVM_DEFAULT)
   `uvm_component_utils_end
    
   /**
    * Default constructor.
    */
   extern function new(string name="stimuli_sqr", uvm_component parent=null);
   /**
    * Ensures cfg & cntxt handles are not null
    */
   extern virtual function void build_phase(uvm_phase phase);

endclass : stimuli_sqr

function stimuli_sqr::new(string name="stimuli_sqr",uvm_component parent);

      super.new(name,parent);

endfunction

function void stimuli_sqr::build_phase(uvm_phase phase);
   
   super.build_phase(phase);
   
   void'(uvm_config_db#(stimuli_cfg)::get(this, "", "cfg", m_stimuli_cfg));
   if (!m_stimuli_cfg) begin
      `uvm_fatal("CFG", "Configuration handle is null")
   end
   
   void'(uvm_config_db#(stimuli_cntxt)::get(this, "", "cntxt", m_stimuli_cntxt));
   if (!m_stimuli_cntxt) begin
      `uvm_fatal("CNTXT", "Context handle is null")
   end
   
endfunction : build_phase

  
   
`endif // stimuli_SQR
