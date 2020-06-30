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

`ifndef stimuli_PKG
`define stimuli_PKG

  `include "uvm_macros.svh"

  `include "stimuli_if.sv"

package stimuli_pkg;
  import uvm_pkg::*;


  // Constants / Strucs / Enum
  `include "stimuli_constants.sv"
  `include "stimuli_tdefs.sv"

   // Objects
  `include "stimuli_cfg.sv"
  `include "stimuli_cntxt.sv"

  `include "stimuli_seq_item.sv"
  `include "stimuli_sqr.sv"
  `include "seq_lib/stimuli_base_seq.sv"
  `include "seq_lib/stimuli_standard_seq.sv"
   
  `include "stimuli_driver.sv" 
  `include "stimuli_monitor.sv"
  `include "stimuli_agent.sv"
   
endpackage : stimuli_pkg

`endif // stimuli_PKG
