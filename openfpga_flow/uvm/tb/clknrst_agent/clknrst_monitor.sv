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

`ifndef CLKNRST_MONITOR
`define CLKNRST_MONITOR

class clknrst_monitor extends uvm_monitor;

   clknrst_cfg    m_clknrst_cfg;
   clknrst_cntxt  m_clknrst_cntxt;
   
   `uvm_component_utils_begin(clknrst_monitor)
      `uvm_field_object(m_clknrst_cfg  , UVM_DEFAULT)
      `uvm_field_object(m_clknrst_cntxt, UVM_DEFAULT)
   `uvm_component_utils_end
  
   /**
    * Default constructor.
    */
   extern function new(string name="clknrst_monitor", uvm_component parent=null);
   
   /**
    * Builds scoreboard port.
    */
   extern virtual function void build_phase(uvm_phase phase);
   
   /**
    * Monitor signals for scoreboard to be send to scoreboard.
    */
   extern virtual task run_phase(uvm_phase phase);

endclass : clknrst_monitor 
 
function clknrst_monitor::new(string name="clknrst_monitor",uvm_component parent);
   
	super.new(name,parent);
//	trans_collected =new();
//   	item_collected_port =new("item_collected_port",this);
       
endfunction 

function void clknrst_monitor::build_phase(uvm_phase phase);
	
	super.build_phase(phase);
   	void'(uvm_config_db#(clknrst_cfg)::get(this, "", "cfg", m_clknrst_cfg));
	if (!m_clknrst_cfg) begin
      	   `uvm_fatal("CFG", "Configuration handle is null")
	end
   
	void'(uvm_config_db#(clknrst_cntxt)::get(this, "", "cntxt", m_clknrst_cntxt));
   	if (!m_clknrst_cntxt) begin
	   `uvm_fatal("CNTXT", "Context handle is null")
   	end

endfunction: build_phase

task clknrst_monitor::run_phase(uvm_phase phase);


endtask: run_phase
      
`endif // CLKNRST_MONITOR
