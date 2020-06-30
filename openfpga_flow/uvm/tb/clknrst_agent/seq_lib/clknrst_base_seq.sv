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

`ifndef "CLKNRST_BASE_SEQ"
`define "CLKNRST_BASE_SEQ"


class clknrst_base_seq extends uvm_sequence#(
   .REQ(clknrst_seq_item),
   .RSP(clknrst_seq_item)
);

   `uvm_object_utils(clknrst_base_seq)
   `uvm_declare_p_sequencer(clknrst_sqr)

   /**
    * Default constructor.
    */
   extern function new(string name="clknrst_base_seq");

endclass : clknrst_base_seq

function clknrst_base_seq::new(string name = "clknrst_base_seq");

    super.new(name);

endfunction
  

`endif // CLKNRST_BASE_SEQ

