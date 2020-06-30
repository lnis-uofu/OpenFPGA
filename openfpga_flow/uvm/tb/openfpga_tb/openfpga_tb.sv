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

`ifndef OPENFPGA_TB
`define OPENFPGA_TB

`include "uvm_macros.svh"

module openfpga_tb;
   
  
   import uvm_pkg::*;
   import openfpga_tb_pkg::*;
   import openfpga_env_pkg::*;
   


   // Creating instance of interface
   clknrst_if m_clknrst_if();
   bs_if      m_bs_if(m_clknrst_if.clk_uvm, m_clknrst_if.clk);

wire set = 1'b0;
wire enable;

assign enable = ~m_clknrst_if.bs_clock_reg;

		fpga_top FPGA_DUT (
					.pReset(m_clknrst_if.pReset[0]),
			      		.prog_clk(m_clknrst_if.bs_clock_reg[0]),
					.set(set),
					.reset(m_clknrst_if.Reset[0]),
					.clk(m_clknrst_if.clk[0]),
	              		  	.gfpga_pad_iopad_pad(m_bs_if.gfpga_pad_GPIO[0:63]),
	              		  	.enable(enable),
	              		  	.address(m_bs_if.address),
		                	.data_in(m_bs_if.data_in));

// SVA binding


   /**
   * Testbench entry point.
   */
   initial begin : Testbench_entry_point

     // Specify time format for simulation (units_number, precision_number, suffix_string, minimum_field_width)
     $timeformat(-9, 3, " ns", 8);

     // Add interfaces handles to uvm_config_db
     uvm_config_db#(virtual clknrst_if        )::set(.cntxt(null), .inst_name("*.openfpga_env.clknrst_agent"), .field_name("vif"),         .value(m_clknrst_if));
     uvm_config_db#(virtual bs_if             )::set(.cntxt(null), .inst_name("*.openfpga_env.bs_agent")     , .field_name("vif"),         .value(m_bs_if));
     uvm_config_db#(virtual bs_if             )::set(.cntxt(null), .inst_name("*.openfpga_env.stimuli_agent"), .field_name("vif"),         .value(m_bs_if));

     // Run the test
     uvm_top.enable_print_topology = 1;
     uvm_top.finish_on_completion  = 1;
     uvm_top.run_test("andgate_test");
//     uvm_top.run_test("openfpga_base_test");
     $dumpfile("log.vcd");
     $dumpvars;
   end
 

   /**
    * End-of-test summary printout.
    */
   final begin: end_of_test
      string             summary_string;
      uvm_report_server  rs;
      int                err_count;
      int                warning_count;
      int                fatal_count;
      static bit         sim_finished = 0;
      
      static string  red   = "\033[31m\033[1m";
      static string  green = "\033[32m\033[1m";
      static string  reset = "\033[0m";
      
      rs            = uvm_top.get_report_server();
      err_count     = rs.get_severity_count(UVM_ERROR);
      warning_count = rs.get_severity_count(UVM_WARNING);
      fatal_count   = rs.get_severity_count(UVM_FATAL);
      
      void'(uvm_config_db#(bit)::get(null, "", "sim_finished", sim_finished));

      $display("\n%m: *** Test Summary ***\n");
      
      if (sim_finished && (err_count == 0) && (fatal_count == 0)) begin
         $display("    PPPPPPP    AAAAAA    SSSSSS    SSSSSS   EEEEEEEE  DDDDDDD     ");
         $display("    PP    PP  AA    AA  SS    SS  SS    SS  EE        DD    DD    ");
         $display("    PP    PP  AA    AA  SS        SS        EE        DD    DD    ");
         $display("    PPPPPPP   AAAAAAAA   SSSSSS    SSSSSS   EEEEE     DD    DD    ");
         $display("    PP        AA    AA        SS        SS  EE        DD    DD    ");
         $display("    PP        AA    AA  SS    SS  SS    SS  EE        DD    DD    ");
         $display("    PP        AA    AA   SSSSSS    SSSSSS   EEEEEEEE  DDDDDDD     ");
         $display("    ----------------------------------------------------------");
         if (warning_count == 0) begin
           $display("                        SIMULATION PASSED                     ");
         end
         else begin
           $display("                 SIMULATION PASSED with WARNINGS              ");
         end
         $display("    ----------------------------------------------------------");
      end
      else begin
         $display("    FFFFFFFF   AAAAAA   IIIIII  LL        EEEEEEEE  DDDDDDD       ");
         $display("    FF        AA    AA    II    LL        EE        DD    DD      ");
         $display("    FF        AA    AA    II    LL        EE        DD    DD      ");
         $display("    FFFFF     AAAAAAAA    II    LL        EEEEE     DD    DD      ");
         $display("    FF        AA    AA    II    LL        EE        DD    DD      ");
         $display("    FF        AA    AA    II    LL        EE        DD    DD      ");
         $display("    FF        AA    AA  IIIIII  LLLLLLLL  EEEEEEEE  DDDDDDD       ");
         
         if (sim_finished == 0) begin
            $display("    --------------------------------------------------------");
            $display("                   SIMULATION FAILED - ABORTED              ");
            $display("    --------------------------------------------------------");
         end
         else begin
            $display("    --------------------------------------------------------");
            $display("                       SIMULATION FAILED                    ");
            $display("    --------------------------------------------------------");
         end
      end
   end
  
  
endmodule : openfpga_tb

`endif // OPENFPGA_TB

