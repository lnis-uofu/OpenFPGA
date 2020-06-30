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

//TODO : Change to my own control fields when moving to a more complexe design

`ifndef TEST_CFG
`define TEST_CFG


/**
 * Configuration object for testcases
 */
class test_cfg extends uvm_object;
   
   // Knobs for openfpga_environment control
   rand int unsigned  startup_timeout ; // Specified in nanoseconds (ns)
   rand int unsigned  heartbeat_period; // Specified in nanoseconds (ns)
   rand int unsigned  watchdog_timeout; // Specified in nanoseconds (ns)
   // Knobs for test-bsram control
   rand test_bsram_type tpt;

   // Command line arguments for controlling RAL
   // (note: its not clear if this ENV will use the RAL)
   string cli_block_name_str      = "BLKNM";
   bit    cli_block_name_override = 0;
   //uvm_reg_block  cli_selected_block;

   // Command line arguments for FIRMWARE (Test Program) selection
   // +firmware=<path_to_hexfile_test_bsram>
   string cli_firmware_select_str      = "firmware";
   bit    cli_firmware_select_override = 0;
   string cli_firmware_name_str        = "";

   // Run-time control
   bit    run_riscv_gcc_toolchain = 0;
   
   `uvm_object_utils_begin(test_cfg)
      `uvm_field_int(heartbeat_period, UVM_DEFAULT)
      `uvm_field_int(watchdog_timeout, UVM_DEFAULT)
      `uvm_field_enum(test_bsram_type, tpt, UVM_DEFAULT)
      
      //`uvm_field_object(cli_selected_block, UVM_DEFAULT)
      `uvm_field_int(run_riscv_gcc_toolchain, UVM_DEFAULT)
   `uvm_object_utils_end
   
   
   constraint timeouts_default_cons {
      soft startup_timeout  == 100_000_000; // Set to be huge for now so that sim can finish
      soft heartbeat_period ==    200_000; //  2 us // TODO Set default Heartbeat Monitor period for uvmt_cv32_base_test_c
      soft watchdog_timeout == 100_000_000; // 10 ms // TODO Set default Watchdog timeout period for uvmt_cv32_base_test_c
   }
   
   //constraint test_type_default_cons {
   //  soft tpt == NONE;
   //}
   
   /**
    * Default constructor.
    */
   extern function new(string name="test_cfg");
   
   /**
    * TODO Describe uvmt_cv32_test_cfg_c::process_cli_args()
    */
   extern function void process_cli_args();
   
endclass : test_cfg


function test_cfg::new(string name="test_cfg");
   
   super.new(name);

endfunction : new


function void test_cfg::process_cli_args();
   
   string  cli_block_name_parsed_str           = "";
   
   // Process plusarg for RAL control
   cli_block_name_override = 0; //default
   if (uvm_cmdline_proc.get_arg_value({"+", cli_block_name_str, "="}, cli_block_name_parsed_str)) begin
      if (cli_block_name_parsed_str != "") begin
         cli_block_name_override = 1;
         //cli_selected_block = ral.get_block_by_name(cli_block_name_parsed_str);
         `uvm_info("TEST_CFG", $sformatf("process_cli_args() RAL block_name=%s", cli_block_name_str), UVM_LOW)
      end
   end

   // Process plusarg for Firmware selection
   cli_firmware_select_override = 0; // default
   if (uvm_cmdline_proc.get_arg_value({"+", cli_firmware_select_str, "="}, cli_firmware_name_str)) begin
      if (cli_firmware_name_str != "") begin
         cli_firmware_select_override = 1;
         run_riscv_gcc_toolchain      = 1;
         `uvm_info("TEST_CFG", $sformatf("process_cli_args() firmware=%s", cli_firmware_name_str), UVM_LOW)
      end
   end

   `uvm_info("TEST_CFG", "process_cli_args() complete", UVM_HIGH)
   
endfunction : process_cli_args


`endif // TEST_CFG
