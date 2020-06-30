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

`ifndef BS_CNTXT
`define BS_CNTXT


/**
 * Object encapsulating all state variables for all Bitstream agent
 * (bs_agent) components.
 */
class bs_cntxt extends uvm_object;
   
   // Handle to agent interface
   virtual bs_if  vif;
   
   // Clock Monitoring

   int unsigned  prog_clock_cycles; ///< Number of good clock cycles
   
   // Reset Monitoring
   logic    mon_reset_state           ; ///< Last reset edge
   realtime mon_reset_assert_timestamp; ///< Reset assertion edge timestamp
   
   // Bitstream loaded indicator
   logic    config_done;
   
   `uvm_object_utils_begin(bs_cntxt)
      
      `uvm_field_int (prog_clock_cycles	, UVM_DEFAULT + UVM_DEC)
      `uvm_field_int (config_done	, UVM_DEFAULT)
      `uvm_field_int (mon_reset_state           , UVM_DEFAULT)
      `uvm_field_real(mon_reset_assert_timestamp, UVM_DEFAULT)
   `uvm_object_utils_end
   
   
   /**
    * Builds events.
    */
   extern function new(string name="bs_cntxt");
   
   /**
    * Resets integrals to their default values.
    */
   extern function void reset();
   /**
    * Resets integrals to their default values.
    */
   extern function void set_config_done();
   
endclass : bs_cntxt


function bs_cntxt::new(string name="bs_cntxt");
   
   super.new(name);   
   reset();
   
endfunction : new


function void bs_cntxt::reset();
   
   prog_clock_cycles	    =  0;
   config_done		    =  0;
   
   mon_reset_state            = 0;
   mon_reset_assert_timestamp = 0;
   
endfunction : reset

function void bs_cntxt::set_config_done();

   config_done	= 0;

endfunction : set_config_done


`endif // BS_CNTXT
