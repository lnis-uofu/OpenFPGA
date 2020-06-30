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

`ifndef BS_DRIVER
`define BS_DRIVER

class bs_driver extends uvm_driver #(
   .REQ(bs_seq_item),
   .RSP(bs_seq_item)
);

   bs_cfg    m_bs_cfg;
   bs_cntxt  m_bs_cntxt;

   bs_seq_item req; 

   `uvm_component_utils_begin(bs_driver)
      `uvm_field_object(m_bs_cfg  , UVM_DEFAULT)
      `uvm_field_object(m_bs_cntxt, UVM_DEFAULT)
   `uvm_component_utils_end

 
  /**
    * Default constructor.
    */
   extern function new(string name="bs_driver", uvm_component parent=null);
   
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
   
 
endclass : bs_driver 
  
function bs_driver::new(string name="bs_driver",uvm_component parent);

	super.new(name,parent);

endfunction : new

function void bs_driver::build_phase(uvm_phase phase);

        super.build_phase(phase);
	void'(uvm_config_db#(bs_cfg)::get(this, "", "cfg", m_bs_cfg));
	if (!m_bs_cfg) begin
		`uvm_fatal("CFG", "Configuration handle is null")
	end
   
	void'(uvm_config_db#(bs_cntxt)::get(this, "", "cntxt", m_bs_cntxt));
	if (!m_bs_cntxt) begin
		`uvm_fatal("CNTXT", "Context handle is null")
	end

endfunction:build_phase
    
   // run phase
task bs_driver::run_phase(uvm_phase phase);

// Case currently useless, could be useful later if we speed up bitstream load somehow
   case (m_bs_cfg.bs_initial_itf_value)
	STANDARD:
		begin
		 m_bs_cntxt.vif.address 			 = 0;
		 m_bs_cntxt.vif.data_in				 = 0;
		 m_bs_cntxt.vif.IE				 = 0;	 
		end
	 NOVALUE:
		begin
	  	 m_bs_cntxt.vif.address		     		 = 'X;
		 m_bs_cntxt.vif.data_in				 = 'X;
		 m_bs_cntxt.vif.IE				 = 'X;
		end
  endcase

	forever begin
	   seq_item_port.get_next_item(req);
	   drive();
	   seq_item_port.item_done();
    	end
endtask : run_phase
 
task bs_driver::drive();
	req.print();
	@(negedge m_bs_cntxt.vif.clk_uvm);
	m_bs_cntxt.vif.address   = req.address;
	m_bs_cntxt.vif.data_in   = req.data_in;
	m_bs_cntxt.vif.IE	 = req.IE;
	$display("-----------------------------------------");
endtask : drive
    
`endif // BS_DRIVER
