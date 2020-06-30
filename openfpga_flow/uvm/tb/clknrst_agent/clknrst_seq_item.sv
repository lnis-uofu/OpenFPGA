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

`ifndef CLKNRST_SEQ_ITEM
`define CLKNRST_SEQ_ITEM

class clknrst_seq_item extends uvm_sequence_item;
 
   rand bit [0:0] Reset;
   rand bit [0:0] pReset ;
   
   rand clknrst_seq_item_action_enum         action         ; ///< What operation to perform
   rand clknrst_seq_item_initial_value_enum  initial_value  ; ///< The initial value (if starting or asserting)
   
   rand int unsigned  clknrst_period         ; ///< Setting to 0 will conserve the current clock period
   rand int unsigned  rst_deassert_period; ///< The amount of time (ps) after which to deassert reset  
   
  //Utility and Field macros
  `uvm_object_utils_begin(clknrst_seq_item)
    `uvm_field_int(Reset,UVM_ALL_ON)
    `uvm_field_int(pReset,UVM_ALL_ON)

    `uvm_field_enum(clknrst_seq_item_action_enum       , action         , UVM_DEFAULT)
    `uvm_field_enum(clknrst_seq_item_initial_value_enum, initial_value  , UVM_DEFAULT)
      
    `uvm_field_int(clknrst_period         , UVM_DEFAULT)
    `uvm_field_int(rst_deassert_period, UVM_DEFAULT)
  `uvm_object_utils_end
   
   constraint default_cons {
      soft clknrst_period          == 0;
      soft rst_deassert_period     == clknrst_default_rst_deassert_period;
   }


   /**
    * Default constructor.
    */
   extern function new(string name="clknrst_seq_item");

endclass : clknrst_seq_item


function clknrst_seq_item::new(string name = "clknrst_seq_item");

    super.new(name);

endfunction
   


`endif // CLKNRST_SEQ_ITEM
