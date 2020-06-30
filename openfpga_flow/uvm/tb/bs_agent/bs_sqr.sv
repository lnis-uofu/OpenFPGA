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

`ifndef BS_SEQUENCER
`define BS_SEQUENCER

class bs_sqr extends uvm_sequencer#(
   .REQ(bs_seq_item),
   .RSP(bs_seq_item)
);
  
   bs_cfg    m_bs_cfg;
   bs_cntxt  m_bs_cntxt;
   
   
   `uvm_component_utils_begin(bs_sqr)
      `uvm_field_object(m_bs_cfg  , UVM_DEFAULT)
      `uvm_field_object(m_bs_cntxt, UVM_DEFAULT)
   `uvm_component_utils_end
    
   /**
    * Default constructor.
    */
   extern function new(string name="bs_sqr", uvm_component parent=null);
   /**
    * Ensures cfg & cntxt handles are not null
    */
   extern virtual function void build_phase(uvm_phase phase);

endclass : bs_sqr

function bs_sqr::new(string name="bs_sqr",uvm_component parent);

      super.new(name,parent);

endfunction

function void bs_sqr::build_phase(uvm_phase phase);
   
   super.build_phase(phase);
   
   void'(uvm_config_db#(bs_cfg)::get(this, "", "cfg", m_bs_cfg));
   if (!m_bs_cfg) begin
      `uvm_fatal("CFG", "Configuration handle is null")
   end
   
   void'(uvm_config_db#(bs_cntxt)::get(this, "", "cntxt", m_bs_cntxt));
   if (!m_bs_cntxt) begin
      `uvm_fatal("CNTXT", "Context handle is null")
   end
   
endfunction : build_phase

  
   
`endif // BS_SEQUENCER
