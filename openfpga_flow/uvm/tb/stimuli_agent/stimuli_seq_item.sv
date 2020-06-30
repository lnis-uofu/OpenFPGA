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

`ifndef stimuli_SEQ_ITEM
`define stimuli_SEQ_ITEM

class stimuli_seq_item extends uvm_sequence_item;
 
   rand bit [0:63] 	gfpga_pad_GPIO_IN_drv; // Scalability
   bit 	    [0:63]	gfpga_pad_GPIO;

   
   rand stimuli_seq_item_action_enum         action         ; ///< What operation to perform
   rand stimuli_seq_item_initial_value_enum  initial_value  ; ///< The initial value (if starting or asserting)
   
   rand int unsigned  stimuli_period         ; ///< Setting to 0 will conserve the current clock period
   rand int unsigned  rst_deassert_period; ///< The amount of time (ps) after which to deassert reset  
   
  //Utility and Field macros
  `uvm_object_utils_begin(stimuli_seq_item)
    `uvm_field_int(gfpga_pad_GPIO_IN_drv,UVM_ALL_ON)
    `uvm_field_int(gfpga_pad_GPIO	,UVM_ALL_ON)
    `uvm_field_enum(stimuli_seq_item_action_enum       , action         , UVM_DEFAULT)
    `uvm_field_enum(stimuli_seq_item_initial_value_enum, initial_value  , UVM_DEFAULT)
      
    `uvm_field_int(stimuli_period         , UVM_DEFAULT)
    `uvm_field_int(rst_deassert_period, UVM_DEFAULT)
  `uvm_object_utils_end
   
   constraint default_cons {
      soft stimuli_period          == 0;
   }
// Following constraint hotfix scoreboard error for undefined + unused bits from GPIO_pad
   constraint undef_bit {
      soft gfpga_pad_GPIO_IN_drv[32] 	== 0;
   }
   /**
    * Default constructor.
    */
   extern function new(string name="stimuli_seq_item");

endclass : stimuli_seq_item


function stimuli_seq_item::new(string name = "stimuli_seq_item");

    super.new(name);

endfunction
   


`endif // stimuli_SEQ_ITEM
