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


`ifndef CLKNRST_DRIVER
`define CLKNRST_DRIVER

class clknrst_driver extends uvm_driver#(
   .REQ(clknrst_seq_item),
   .RSP(clknrst_seq_item)
);

   clknrst_cfg    m_clknrst_cfg;
   clknrst_cntxt  m_clknrst_cntxt;

   clknrst_seq_item req;

   `uvm_component_utils_begin(clknrst_driver)
      `uvm_field_object(m_clknrst_cfg  , UVM_DEFAULT)
      `uvm_field_object(m_clknrst_cntxt, UVM_DEFAULT)
   `uvm_component_utils_end

   /**
    * Default constructor.
    */
   extern function new(string name="clknrst_driver", uvm_component parent=null);
   
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

endclass : clknrst_driver
  
function clknrst_driver::new(string name="clknrst_driver",uvm_component parent=null);

	super.new(name,parent);

endfunction : new

function void clknrst_driver::build_phase(uvm_phase phase);

	super.build_phase(phase);

	void'(uvm_config_db#(clknrst_cfg)::get(this, "", "cfg", m_clknrst_cfg));
	if (!m_clknrst_cfg) begin
      	   `uvm_fatal("CFG", "Configuration handle is null")
	end
   
	void'(uvm_config_db#(clknrst_cntxt)::get(this, "", "cntxt", m_clknrst_cntxt));
	if (!m_clknrst_cntxt) begin
	   `uvm_fatal("CNTXT", "Context handle is null")
   	end

endfunction : build_phase
    
   // run phase
task clknrst_driver::run_phase(uvm_phase phase);

   case (m_clknrst_cfg.clknrst_initial_itf_value)
	STANDARD:
		begin
		 m_clknrst_cntxt.vif.pReset  = 0;
		 m_clknrst_cntxt.vif.Reset   = 0; 
		 m_clknrst_cntxt.vif.clk     = 0;
		end
	 NOVALUE:
		begin
		 m_clknrst_cntxt.vif.pReset  	= 0;
	  	 m_clknrst_cntxt.vif.clk    	= 0;
	  	 m_clknrst_cntxt.vif.bs_clock_reg  = 0;
		end 
   endcase
	forever begin
       		`uvm_info("driver", $sformatf("debug purpose"), UVM_LOW);      
    		seq_item_port.get_next_item(req);
		drive();
		seq_item_port.item_done();
	end
endtask : run_phase
 
// drive
task clknrst_driver::drive();
	req.print();
	$display("-----------------------------------------");
	@(negedge m_clknrst_cntxt.vif.clk_uvm);
		m_clknrst_cntxt.vif.pReset  = req.pReset;
		m_clknrst_cntxt.vif.Reset   = req.Reset;
     
endtask : drive
   
    
`endif // CLKNRST_DRIVER
