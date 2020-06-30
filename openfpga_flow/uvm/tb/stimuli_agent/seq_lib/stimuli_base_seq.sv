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

`ifndef "stimuli_BASE_SEQ"
`define "stimuli_BASE_SEQ"


class stimuli_base_seq extends uvm_sequence#(
   .REQ(stimuli_seq_item),
   .RSP(stimuli_seq_item)
);

   `uvm_object_utils(stimuli_base_seq)
   `uvm_declare_p_sequencer(stimuli_sqr)

   /**
    * Default constructor.
    */
   extern function new(string name="stimuli_base_seq");

endclass : stimuli_base_seq

function stimuli_base_seq::new(string name = "stimuli_base_seq");

    super.new(name);

endfunction
  

`endif // stimuli_BASE_SEQ
