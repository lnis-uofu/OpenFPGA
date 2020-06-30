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

`ifndef BS_PKG
`define BS_PKG

  `include "uvm_macros.svh"
  `include "bs_if.sv"

package bs_pkg;
  import uvm_pkg::*;

  // Constants / Strucs / Enum
  `include "bs_constants.sv"
  `include "bs_tdefs.sv"
   // Objects
  `include "bs_cfg.sv"
  `include "bs_cntxt.sv"

  `include "bs_cov.sv"

  `include "bs_seq_item.sv"
  `include "bs_sqr.sv"
  `include "seq_lib/bs_base_seq.sv"
  `include "seq_lib/bitstream_andgate.sv"
  `include "seq_lib/prog_seq_generated.sv"

   
  `include "bs_driver.sv" 
  `include "bs_monitor.sv"
  `include "bs_agent.sv"
   
endpackage : bs_pkg

`endif // BS_PKG
