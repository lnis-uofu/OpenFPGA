//
// Copyright 2020 OpenHW Group
// Copyright 2020 Datum Technologies
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

`ifndef OPENFPGA_TB_PKG
`define OPENFPGA_TB_PKG


// Pre-processor macros
`include "uvm_macros.svh"
`include "openfpga_tb_macros.sv"

// Time units and precision for this test bench
timeunit       1ns;
timeprecision  1ps;

// All Interfaces used by the TB are here
`include "openfpga_tb_ifs.sv"


/**
 * Encapsulates all the types and test cases 
 */
package openfpga_tb_pkg;
   
   import uvm_pkg::*;
   import bs_pkg::*;
   import clknrst_pkg::*;
   import stimuli_pkg::*;
   import openfpga_env_pkg::*;
   
   // Constants / Structs / Enums
   `include "openfpga_tb_constants.sv"
   `include "openfpga_tb_tdefs.sv"
   
   // Virtual sequence library
   // TODO Add virtual sequences
   //      Ex: `include "uvmt_cv32_sanity_vseq.sv"
//   `include "uvmt_cv32_vseq_lib.sv"
   
   // Test cases
     `include "../openfpga_test/test_cfg.sv"
     `include "../openfpga_test/openfpga_base_test.sv"
     `include "../openfpga_test/andgate_test.sv"
     `include "../openfpga_test/benchmark_gen_test.sv"

endpackage : openfpga_tb_pkg


`endif // TB_PKG
