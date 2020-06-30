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

`ifndef BS_AGENT
`define BS_AGENT

class bs_agent extends uvm_agent;
  
   // Objects
   bs_cfg       m_bs_cfg;
   bs_cntxt     m_bs_cntxt;
  
   bs_driver 	m_bs_driver;
   bs_sqr  	m_bs_sqr;
   bs_monitor	m_bs_monitor;
    
   `uvm_component_utils_begin(bs_agent)
      `uvm_field_object(m_bs_cfg  , UVM_DEFAULT)
      `uvm_field_object(m_bs_cntxt, UVM_DEFAULT)
   `uvm_component_utils_end 

   /**
    * Default constructor.
    */
   extern function new(string name="bs_agent", uvm_component parent=null);
   
   /**
    * 1. Ensures cfg & cntxt handles are not null
    * 2. Builds all components
    */
   extern virtual function void build_phase(uvm_phase phase);
   
   /**
    * Links agent's analysis ports to sub-components'
    */
   extern virtual function void connect_phase(uvm_phase phase);
   /**
    * Uses uvm_config_db to retrieve cfg and hand out to sub-components.
    */
   extern function void get_and_set_cfg();
   
   /**
    * Uses uvm_config_db to retrieve cntxt and hand out to sub-components.
    */
   extern function void get_and_set_cntxt();
   
   /**
    * Uses uvm_config_db to retrieve the Virtual Interface (vif) associated with this
    * agent.
    */
   extern function void retrieve_vif();
   
   /**
    * Creates sub-components.
    */
   extern function void create_components();

endclass

function bs_agent::new(string name="bs_agent",uvm_component parent);

	super.new(name,parent);

endfunction
      
function void bs_agent::build_phase(uvm_phase phase);

	super.build_phase(phase);
   	get_and_set_cfg  ();
   	get_and_set_cntxt();
   	retrieve_vif     ();
   	create_components();

endfunction:build_phase
        
function void bs_agent::connect_phase(uvm_phase phase);

	if(m_bs_cfg.is_active ==UVM_ACTIVE) begin
	   m_bs_driver.seq_item_port.connect(m_bs_sqr.seq_item_export);
	end
endfunction:connect_phase

function void bs_agent::get_and_set_cfg();
   
   void'(uvm_config_db#(bs_cfg)::get(this, "", "cfg", m_bs_cfg));
   if (!m_bs_cfg) begin
      `uvm_fatal("CFG", "Configuration handle is null")
   end
   else begin
      `uvm_info("CFG", $sformatf("Found configuration handle:\n%s", m_bs_cfg.sprint()), UVM_DEBUG)
      uvm_config_db#(bs_cfg)::set(this, "*", "cfg", m_bs_cfg);
   end
   
endfunction : get_and_set_cfg


function void bs_agent::get_and_set_cntxt();
   
   void'(uvm_config_db#(bs_cntxt)::get(this, "", "cntxt", m_bs_cntxt));
   if (!m_bs_cntxt) begin
     `uvm_info("CNTXT", "Context handle is null; creating.", UVM_DEBUG)
      m_bs_cntxt = bs_cntxt::type_id::create("cntxt");
   end
   uvm_config_db#(bs_cntxt)::set(this, "*", "cntxt", m_bs_cntxt);
   
endfunction : get_and_set_cntxt


function void bs_agent::retrieve_vif();
   
   if (!uvm_config_db#(virtual bs_if)::get(this, "", "vif", m_bs_cntxt.vif)) begin
      `uvm_fatal("VIF", $sformatf("Could not find vif handle of type %s in uvm_config_db", $typename(m_bs_cntxt.vif)))
   end
   else begin
      `uvm_info("VIF", $sformatf("Found vif handle of type %s in uvm_config_db", $typename(m_bs_cntxt.vif)), UVM_DEBUG)
   end
   
endfunction : retrieve_vif


function void bs_agent::create_components();
   
   m_bs_monitor = bs_monitor::type_id::create("monitor", this);
   if(m_bs_cfg.is_active ==UVM_ACTIVE) begin
	m_bs_sqr 	= bs_sqr     ::type_id::create("sequencer"      , this);
	m_bs_driver     = bs_driver  ::type_id::create("driver"         , this);
   end

endfunction : create_components

   
`endif // BS_AGENT
