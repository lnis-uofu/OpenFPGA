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

`ifndef stimuli_MONITOR
`define stimuli_MONITOR

class stimuli_monitor extends uvm_monitor;


   // TLM
   uvm_analysis_port #(stimuli_seq_item) stimuli_collected_port;
   stimuli_seq_item trans_collected;

   // Objects handle
   stimuli_cfg    m_stimuli_cfg;
   stimuli_cntxt  m_stimuli_cntxt;

   // Phasing with reference model

   `uvm_component_utils_begin(stimuli_monitor)
      `uvm_field_object(m_stimuli_cfg  , UVM_DEFAULT)
      `uvm_field_object(m_stimuli_cntxt, UVM_DEFAULT)
   `uvm_component_utils_end
 
   /**
    * Default constructor.
    */
   extern function new(string name="stimuli_monitor", uvm_component parent=null);
   
   /**
    * Builds scoreboard port.
    */
   extern virtual function void build_phase(uvm_phase phase);
   
   /**
    * Monitor signals for scoreboard to be send to scoreboard.
    */
   extern virtual task run_phase(uvm_phase phase);

endclass : stimuli_monitor
   
function stimuli_monitor::new(string name="stimuli_monitor",uvm_component parent);
 
      	super.new(name,parent);
      	trans_collected =new();
      	stimuli_collected_port =new("stimuli_collected_port",this);
       
endfunction 

function void stimuli_monitor::build_phase(uvm_phase phase);

        super.build_phase(phase);
	void'(uvm_config_db#(stimuli_cfg)::get(this, "", "cfg", m_stimuli_cfg));
	if (!m_stimuli_cfg) begin
		`uvm_fatal("CFG", "Configuration handle is null")
	end
   
	void'(uvm_config_db#(stimuli_cntxt)::get(this, "", "cntxt", m_stimuli_cntxt));
	if (!m_stimuli_cntxt) begin
		`uvm_fatal("CNTXT", "Context handle is null")
	end

endfunction: build_phase

task stimuli_monitor::run_phase(uvm_phase phase);
	   @(posedge m_stimuli_cntxt.vif.clk); 
      	forever begin
	   @(posedge m_stimuli_cntxt.vif.clk); // driver on posedge
	   trans_collected.gfpga_pad_GPIO = m_stimuli_cntxt.vif.gfpga_pad_GPIO;

	   	stimuli_collected_port.write(trans_collected);


      	end
endtask: run_phase
   
`endif // stimuli_MONITOR
