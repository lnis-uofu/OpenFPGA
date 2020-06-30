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


`ifndef OPENFPGA_ENV
`define OPENFPGA_ENV


/**
 * Top-level component that encapsulates, builds and connects all other
 * CV32 openfpga_environment components.
 */
class openfpga_env extends uvm_env;
   
   // Objects
   openfpga_env_cfg    	m_openfpga_env_cfg;
   openfpga_env_cntxt  	m_openfpga_env_cntxt;

   // RAL
   openfpga_ral		m_openfpga_ral; 

   // Components
 //  uvme_cv32_cov_model_c  cov_model;
     reference_model	m_reference_model;
     my_scoreboard      m_my_scoreboard;
 //  uvme_cv32_vsqr_c       vsequencer;
   
   // Agents
   clknrst_agent  	m_clknrst_agent;
   bs_agent		m_bs_agent;
   stimuli_agent	m_stimuli_agent;

   // Analysis port
//   uvm_analysis_port #(bs_seq_item) to_refmod;
  
   `uvm_component_utils_begin(openfpga_env)
      `uvm_field_object(m_openfpga_env_cfg  , UVM_DEFAULT)
      `uvm_field_object(m_openfpga_env_cntxt, UVM_DEFAULT)
   `uvm_component_utils_end
   
   
   /**
    * Default constructor.
    */
   extern function new(string name="openfpga_env", uvm_component parent=null);
   
   /**
    * 1. Ensures cfg & cntxt handles are not null
    * 2. Assigns cfg and cntxt handles via assign_cfg() & assign_cntxt()
    * 3. Builds all components via create_<x>()
    */
   extern virtual function void build_phase(uvm_phase phase);
   
   /**
    * 1. Connects agents to predictor via connect_predictor()
    * 2. Connects ral to predictor & provisioning agent via connect_ral()
    * 3. Connects predictor & agents to scoreboard via connect_scoreboard()
    * 4. Assembles virtual sequencer handles via assemble_vsequencer()
    * 5. Connects agents to coverage model via connect_coverage_model()
    */
   extern virtual function void connect_phase(uvm_phase phase);
   
   /**
    * Assigns configuration handles to components using UVM Configuration Database.
    */
   extern virtual function void assign_cfg();
   
   /**
    * Assigns context handles to components using UVM Configuration Database.
    */
   extern virtual function void assign_cntxt();
   
   /**
    * Creates agent components.
    */
   extern virtual function void create_agents();
   /**
    * Creates reference model.
    */
   extern virtual function void create_reference_model();
   
   /**
    * Creates ral_adapter which translates to/from ral to debug_agent.
    */
   extern virtual function void create_ral_adapter();
   
   /**
    * Creates additional (non-agent) openfpga_environment components (and objects).
    */
   extern virtual function void create_openfpga_env_components();
   
   /**
    * Creates openfpga_environment's virtual sequencer.
    */
   extern virtual function void create_vsequencer();
   
   /**
    * Creates openfpga_environment's coverage model.
    */
   extern virtual function void create_cov_model();
   
   /**
    * Connects agents to predictor.
    */
   extern virtual function void connect_predictor();
   
   /**
    * Connects scoreboards components to agents/predictor.
    */
   extern virtual function void connect_scoreboard();
   
   /**
    * Connects RAL to provisioning agent (debug_agent).
    */
   extern virtual function void connect_ral();
   
   /**
    * Connects openfpga_environment coverage model to agents/scoreboards/predictor.
    */
   extern virtual function void connect_coverage_model();
   
   /**
    * Assembles virtual sequencer from agent sequencers.
    */
   extern virtual function void assemble_vsequencer();
   
endclass : openfpga_env


function openfpga_env::new(string name="openfpga_env", uvm_component parent=null);
   
   super.new(name, parent);

endfunction : new


function void openfpga_env::build_phase(uvm_phase phase);
   
   super.build_phase(phase);
   
   void'(uvm_config_db#(openfpga_env_cfg)::get(this, "", "cfg", m_openfpga_env_cfg));
   if (!m_openfpga_env_cfg) begin
      `uvm_fatal("CFG", "Configuration handle is null")
   end
   else begin
      `uvm_info("CFG", $sformatf("Found configuration handle:\n%s", m_openfpga_env_cfg.sprint()), UVM_DEBUG)
   end

   randomize(m_openfpga_env_cfg); // Could be placed somewhere else
   
   if (m_openfpga_env_cfg.enabled) begin
      void'(uvm_config_db#(openfpga_env_cntxt)::get(this, "", "cntxt", m_openfpga_env_cntxt));
      if (!m_openfpga_env_cntxt) begin
         `uvm_info("CNTXT", "Context handle is null; creating.", UVM_DEBUG)
         m_openfpga_env_cntxt = openfpga_env_cntxt::type_id::create("cntxt");
      end
      
      assign_cfg           		();
      assign_cntxt         		();
      create_agents        		();
      create_reference_model		();
      create_ral_adapter   		();
      create_openfpga_env_components	();
      if (m_openfpga_env_cfg.is_active) begin
         create_vsequencer();
      end
      
      if (m_openfpga_env_cfg.cov_model_enabled) begin
         create_cov_model();
      end
   end
   
endfunction : build_phase


function void openfpga_env::connect_phase(uvm_phase phase);
   
   super.connect_phase(phase);
   
   if (m_openfpga_env_cfg.enabled) begin
      if (m_openfpga_env_cfg.scoreboarding_enabled) begin
         connect_predictor ();
         connect_scoreboard();
      end
      
      if (m_openfpga_env_cfg.is_active) begin
         connect_ral();
//         assemble_vsequencer();
      end
      
      if (m_openfpga_env_cfg.cov_model_enabled) begin
 //        connect_coverage_model();
      end
   end
   
endfunction: connect_phase


function void openfpga_env::assign_cfg();
   
   uvm_config_db#(openfpga_env_cfg)	::set(this, "*", "cfg",			m_openfpga_env_cfg);
   uvm_config_db#(clknrst_cfg)		::set(this, "*clknrst_agent", "cfg",	m_openfpga_env_cfg.m_clknrst_cfg);
   uvm_config_db#(bs_cfg)		::set(this, "*bs_agent", "cfg",		m_openfpga_env_cfg.m_bs_cfg);
   uvm_config_db#(stimuli_cfg)		::set(this, "*stimuli_agent", "cfg",	m_openfpga_env_cfg.m_stimuli_cfg);


endfunction: assign_cfg


function void openfpga_env::assign_cntxt();
   
   uvm_config_db#(openfpga_env_cntxt)	::set(this, "*", "cntxt"   ,		m_openfpga_env_cntxt);
   uvm_config_db#(clknrst_cntxt)	::set(this, "clknrst_agent", "cntxt", 	m_openfpga_env_cntxt.m_clknrst_cntxt);
   uvm_config_db#(bs_cntxt)		::set(this, "bs_agent", "cntxt", 	m_openfpga_env_cntxt.m_bs_cntxt);
   uvm_config_db#(stimuli_cntxt)	::set(this, "stimuli_agent", "cntxt", 	m_openfpga_env_cntxt.m_stimuli_cntxt);
   
endfunction: assign_cntxt


function void openfpga_env::create_agents();
   
   m_clknrst_agent = clknrst_agent::type_id::create("clknrst_agent",	this);
   m_stimuli_agent = stimuli_agent::type_id::create("stimuli_agent",	this);
   m_bs_agent      = bs_agent	  ::type_id::create("bs_agent",		this);
      
endfunction: create_agents

function void openfpga_env::create_reference_model();
   
   m_reference_model = reference_model::type_id::create("reference_model",this);
      
endfunction: create_reference_model

function void openfpga_env::create_openfpga_env_components();
   
   if (m_openfpga_env_cfg.scoreboarding_enabled) begin
      m_my_scoreboard        = my_scoreboard ::type_id::create("scoreboard", this);
   end
   
endfunction: create_openfpga_env_components


function void openfpga_env::create_ral_adapter();
   
   //reg_adapter = uvma_debug_reg_adapter_c::type_id::create("reg_adapter");
   m_openfpga_ral = m_openfpga_env_cfg.m_openfpga_ral;
   
endfunction: create_ral_adapter


function void openfpga_env::create_vsequencer();
   
 //  vsequencer = uvme_cv32_vsqr_c::type_id::create("vsequencer", this);
   
endfunction: create_vsequencer


function void openfpga_env::create_cov_model();
   
//   cov_model = uvme_cv32_cov_model_c::type_id::create("cov_model", this);
   
endfunction: create_cov_model


function void openfpga_env::connect_predictor();
   	// Connecting driver to predictor
	m_stimuli_agent.ap.connect(m_reference_model.port);
   
endfunction: connect_predictor



function void openfpga_env::connect_scoreboard();
   
      m_stimuli_agent.m_stimuli_monitor.stimuli_collected_port.connect(m_my_scoreboard.dut_collected_export);
      m_reference_model.ap.connect(m_my_scoreboard.ref_collected_export)				    ;
   
endfunction: connect_scoreboard


function void openfpga_env::connect_ral();
   
   //ral.default_map.set_sequencer(debug_agent.sequencer, reg_adapter);
   //ral.default_map.set_auto_predict(1);
   
endfunction: connect_ral


function void openfpga_env::connect_coverage_model();
   
   // TODO Implement uvme_cv32_openfpga_env_c::connect_coverage_model()
   //      Ex: debug_agent.mon_ap.connect(cov_model.debug_export);
   
endfunction: connect_coverage_model


function void openfpga_env::assemble_vsequencer();
   
//   vsequencer.clknrst_sqr = clknrst_agent.sequencer;
   //vsequencer.debug_sequencer   = debug_agent.sequencer;
   
endfunction: assemble_vsequencer


`endif // OPENFPGA_ENV
