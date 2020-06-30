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

`ifndef CLKNRST_IF
`define CLKNRST_IF

`timescale 1ns / 100ps

interface clknrst_if ();

   import uvm_pkg::*;
   // signals
   logic [0:0] clk  		; // DUT_clk
   logic [0:0] bs_clock_reg	; // Programming clock
   logic [0:0] clk_uvm		; // Driver Clock
   logic [0:0] pReset		;
   logic [0:0] Reset		;
   logic [0:0] OUT		; // Monitoring ref dut
  
   // Control fields
   realtime  clknrst_period    = 5;
//   realtime  clknrst_bs_period = 50.0000038;
   realtime  clknrst_bs_period = 50;
   bit       clknrst_active    = 0;
   bit	     clknrst_bs_active = 0;

   /**
    * Clock generation loop
    */
   initial begin
      wait (clknrst_active);
      forever begin
         #(clknrst_period);
         if (clknrst_active) begin
            case (clk)
               '0: clk = 1; // 0 -> 1
               '1: clk = 0; // 1 -> 0
               'X: clk = 0; // X -> 0
            endcase
         end
      end
   end

   initial begin
      wait (clknrst_bs_active);
      forever begin
         #(clknrst_bs_period);
         if (clknrst_bs_active) begin
            case (bs_clock_reg)
               '0: bs_clock_reg = 1; // 0 -> 1
               '1: bs_clock_reg = 0; // 1 -> 0
               'X: bs_clock_reg = 0; // X -> 0
            endcase
         end
      end
   end

always begin
	#(clknrst_bs_period);
	case(clk_uvm)
               '0: clk_uvm = 1; // 0 -> 1
               '1: clk_uvm = 0; // 1 -> 0
               'X: clk_uvm = 0; // X -> 0   
	endcase
	end
   /**
    * Sets clknrst_period
    */
   function void set_period(realtime new_clknrst_period);
      `uvm_info("CLKNRST", $sformatf("Changing clock period to %0t", new_clknrst_period), UVM_LOW)
      clknrst_period = new_clknrst_period;
   endfunction : set_period
   /**
    * Sets clknrst_period
    */
   function void set_period_bs(realtime new_clknrst_period);
      `uvm_info("CLKNRST", $sformatf("Changing clock period to %0t", new_clknrst_period), UVM_LOW)
      clknrst_bs_period = new_clknrst_period;
   endfunction : set_period_bs
  function void test();
      `uvm_info("CLKNRST", $sformatf("Changing clock period to %0t", 5), UVM_LOW)
      clknrst_bs_period = 5;
  endfunction : test 
   /**
    * Sets clknrst_active to 1
    */
   function void start_clk();
      `uvm_info("CLKNRST", "Starting clock generation", UVM_LOW)
      clknrst_active = 1;
   endfunction : start_clk
   
   /**
    * Sets clknrst_active to 0
    */
   function void stop_clk();
      `uvm_info("CLKNRST", "Stopping clock generation", UVM_LOW)
      clknrst_active = 0;
   endfunction : stop_clk

   /**
    * Sets clknrst_bs_active to 1
    */
   function void start_bs_clk();
      `uvm_info("CLKNRST", "Starting BS clock generation", UVM_LOW)
      clknrst_bs_active = 1;
   endfunction : start_bs_clk
   
   /**
    * Sets clknrst_bs_active to 0
    */
   function void stop_bs_clk();
      `uvm_info("CLKNRST", "Stopping BS clock generation", UVM_LOW)
      clknrst_bs_active = 0;
 //     bs_clock_reg = 0; // Could be useful for impair bitstream
   endfunction : stop_bs_clk

   

endinterface:clknrst_if

`endif // CLKNRST_IF
