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

`ifndef STIMULI_CNTXT
`define STIMULI_CNTXT


/**
 * Object encapsulating all state variables for all Clock & Reset agent
 * (uvma_stimuli_agent_c) components.
 */
class stimuli_cntxt extends uvm_object;
   
   // Handle to agent interface
   virtual bs_if vif;
   
   // Clock Monitoring
   bit           mon_stimuli_lock       ; ///< Indicates that we have achieved clock lock
   realtime      mon_stimuli_period     ; ///< Sampled clock period
   logic         mon_stimuli_last_val   ; ///< Last clock value sampled
   realtime      mon_stimuli_last_edge  ; ///< Timestamp of last clock edge
   int unsigned  mon_stimuli_cycle_count; ///< Number of good clock cycles
   
   // Reset Monitoring
   logic    mon_reset_state           ; ///< Last reset edge
   realtime mon_reset_assert_timestamp; ///< Reset assertion edge timestamp
   
   
   `uvm_object_utils_begin(stimuli_cntxt)
     
      `uvm_field_int (mon_stimuli_lock       , UVM_DEFAULT          )
      `uvm_field_real(mon_stimuli_period     , UVM_DEFAULT          )
      `uvm_field_int (mon_stimuli_last_val   , UVM_DEFAULT          )
      `uvm_field_real(mon_stimuli_last_edge  , UVM_DEFAULT          )
      `uvm_field_int (mon_stimuli_cycle_count, UVM_DEFAULT + UVM_DEC)
      
      `uvm_field_int (mon_reset_state           , UVM_DEFAULT)
      `uvm_field_real(mon_reset_assert_timestamp, UVM_DEFAULT)
   `uvm_object_utils_end
   
   
   /**
    * Builds events.
    */
   extern function new(string name="stimuli_cntxt");
   
   /**
    * Resets integrals to their default values.
    */
   extern function void reset();
   
endclass : stimuli_cntxt


function stimuli_cntxt::new(string name="stimuli_cntxt");
   
   super.new(name);
   
   reset();
   
endfunction : new


function void stimuli_cntxt::reset();
   
   mon_stimuli_lock         =  0;
   mon_stimuli_period       =  0;
   mon_stimuli_last_val     = 'X;
   mon_stimuli_last_edge    =  0;
   mon_stimuli_cycle_count  =  0;
   
   mon_reset_state            = 0;
   mon_reset_assert_timestamp = 0;
   
endfunction : reset


`endif // STIMULI_CNTXT
