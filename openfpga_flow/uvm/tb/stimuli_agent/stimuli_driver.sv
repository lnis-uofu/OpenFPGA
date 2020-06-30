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


`ifndef stimuli_DRIVER
`define stimuli_DRIVER

class stimuli_driver extends uvm_driver#(
   .REQ(stimuli_seq_item),
   .RSP(stimuli_seq_item)
);

   uvm_analysis_port #(stimuli_seq_item) stimuli_driver_port;
   stimuli_seq_item trans_collected;

   stimuli_cfg    m_stimuli_cfg;
   stimuli_cntxt  m_stimuli_cntxt;

   stimuli_seq_item req;

   `uvm_component_utils_begin(stimuli_driver)
      `uvm_field_object(m_stimuli_cfg  , UVM_DEFAULT)
      `uvm_field_object(m_stimuli_cntxt, UVM_DEFAULT)
   `uvm_component_utils_end

   /**
    * Default constructor.
    */
   extern function new(string name="stimuli_driver", uvm_component parent=null);
   
   /**
    * Builds ap.
    */
   extern virtual function void build_phase(uvm_phase phase);
   
   /**
    * Obtains the reqs from the sequence item port and calls drv_req()
    */
   extern virtual task run_phase(uvm_phase phase);
   
   /**
    * Drives the virtual interface's (cntxt.vif) signals using req's contents.
    */
   extern task drive();

endclass : stimuli_driver
  
function stimuli_driver::new(string name="stimuli_driver",uvm_component parent=null);

	super.new(name,parent);
      	trans_collected 	=new();
      	stimuli_driver_port 	=new("bs_collected_port",this);
endfunction : new

function void stimuli_driver::build_phase(uvm_phase phase);

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
    
   // run phase
task stimuli_driver::run_phase(uvm_phase phase);

   case (m_stimuli_cfg.stimuli_initial_itf_value)
	STANDARD:
		begin

		 m_stimuli_cntxt.vif.gfpga_pad_GPIO_IN_drv  = 0; 

		end
	 NOVALUE:
		begin
	  	 m_stimuli_cntxt.vif.gfpga_pad_GPIO_IN_drv  = 0; 

		end

   endcase
	forever begin  
    		seq_item_port.get_next_item(req);
		drive();
		seq_item_port.item_done();
	end
endtask : run_phase
 
// drive
task stimuli_driver::drive();
	req.print();
	$display("-----------------------------------------");
	@(negedge m_stimuli_cntxt.vif.clk);
		m_stimuli_cntxt.vif.DRIVER.gfpga_pad_GPIO_IN_drv   = req.gfpga_pad_GPIO_IN_drv;
		trans_collected.gfpga_pad_GPIO_IN_drv		 = req.gfpga_pad_GPIO_IN_drv;
     	   	stimuli_driver_port.write(trans_collected);
endtask : drive
   
    
`endif // stimuli_DRIVER
