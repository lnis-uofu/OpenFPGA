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


`ifndef OPENFPGA_ENV_PKG
`define OPENFPGA_ENV_PKG


// Pre-processor macros
`include "uvm_macros.svh"
//`include "inv.v"
// Not including empty file for now
//`include "clknrst_macros.sv"
//`include "openfpga_env_macros.sv"


 /**
 * Encapsulates all the types needed for an UVM openfpga_environment capable of driving/
 * monitoring and verifying the behavior of an OpenFPGA design.
 */
package openfpga_env_pkg;
   
   import uvm_pkg         ::*;
   import clknrst_pkg::*;
   import bs_pkg::*;
   import stimuli_pkg::*;
   
   // Constants / Structs / Enums
   `include "openfpga_env_constants.sv"
   `include "openfpga_env_tdefs.sv"
   
   // Register Abstraction Layer
   `include "ral/openfpga_ral.sv"

   // Objects
   `include "openfpga_env_cfg.sv"
   `include "openfpga_env_cntxt.sv"

   // Reference model   
   `include "reference_model.sv"   
   
   `include "my_scoreboard.svh"
//   `include "uvme_cv32_vsqr.sv"
   `include "openfpga_env.sv"
   
   // TODO : Virtual sequences
//   `include "uvme_cv32_base_vseq.sv"

   
endpackage : openfpga_env_pkg


`endif // OPENFPGA_ENV_PKG
