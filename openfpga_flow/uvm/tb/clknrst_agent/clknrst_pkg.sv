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

`ifndef CLKNRST_PKG
`define CLKNRST_PKG

  `include "uvm_macros.svh"

  `include "clknrst_if.sv"

package clknrst_pkg;
  import uvm_pkg::*;


  // Constants / Strucs / Enum
  `include "clknrst_constants.sv"
  `include "clknrst_tdefs.sv"

   // Objects
  `include "clknrst_cfg.sv"
  `include "clknrst_cntxt.sv"


//  `include "clknrst_cov.sv"

  `include "clknrst_seq_item.sv"
  `include "clknrst_sqr.sv"
  `include "seq_lib/clknrst_base_seq.sv"
  `include "seq_lib/reset_at_start.sv"
  `include "seq_lib/reset_after_bs.sv"
   
  `include "clknrst_driver.sv" 
  `include "clknrst_monitor.sv"
  `include "clknrst_agent.sv"
   
endpackage : clknrst_pkg

`endif // CLKNRST_PKG
