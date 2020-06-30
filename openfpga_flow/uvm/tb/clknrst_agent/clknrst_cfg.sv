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

`ifndef CLKNRST_CFG
`define CLKNRST_CFG


/**
 * Object encapsulating all parameters for creating, connecting and running all
 * Debug agent (clknrst_cfg) components.
 */
class clknrst_cfg extends uvm_object;
   
   rand bit                      enabled;
   rand uvm_active_passive_enum  is_active = UVM_ACTIVE;
   rand uvm_sequencer_arb_mode   sqr_arb_mode;
   rand bit                      cov_model_enabled;
   rand bit                      trn_log_enabled;

   rand initial_interface_value  clknrst_initial_itf_value           ; ///< The value that will be driven onto interface pin at time 0   
   
   `uvm_object_utils_begin(clknrst_cfg)
      `uvm_field_int (                         enabled          	, UVM_DEFAULT)
      `uvm_field_enum(uvm_active_passive_enum, is_active        	, UVM_DEFAULT)
      `uvm_field_enum(uvm_sequencer_arb_mode , sqr_arb_mode     	, UVM_DEFAULT)
      `uvm_field_int (                         cov_model_enabled	, UVM_DEFAULT)
      `uvm_field_int (                         trn_log_enabled  	, UVM_DEFAULT)
      `uvm_field_enum(initial_interface_value, clknrst_initial_itf_value, UVM_DEFAULT)
   `uvm_object_utils_end
   
// Constraint aren't working   
   constraint defaults_cons {
      soft enabled           == 1;
      soft is_active         == UVM_ACTIVE;
      soft sqr_arb_mode      == UVM_SEQ_ARB_FIFO;
      soft cov_model_enabled == 0;
      soft trn_log_enabled   == 0;
   }
   
   
   /**
    * Default constructor.
    */
   extern function new(string name="clknrst_cfg");
   
endclass : clknrst_cfg


`pragma protect begin


function clknrst_cfg::new(string name="clknrst_cfg");
   
   super.new(name);
   
endfunction : new


`pragma protect end


`endif // CLKNRST_CFG
