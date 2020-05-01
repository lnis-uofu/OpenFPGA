//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Top-level Verilog module for FPGA
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Apr 22 16:46:21 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for fpga_top -----
module fpga_core(pReset,
                prog_clk,
                Reset,
                Test_en,
		clk,
		sc_head,
		sc_tail,
                gfpga_pad_GPIO_A,
                gfpga_pad_GPIO_IE,
                gfpga_pad_GPIO_OE,
                gfpga_pad_GPIO_Y,
                ccff_head,
                ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GLOBAL PORTS -----
input [0:0] Reset;
//----- GLOBAL PORTS -----
input [0:0] Test_en;
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- GPOUT PORTS -----
output [0:7] gfpga_pad_GPIO_A;
//----- GPOUT PORTS -----
output [0:7] gfpga_pad_GPIO_IE;
//----- GPOUT PORTS -----
output [0:7] gfpga_pad_GPIO_OE;
//----- GPIO PORTS -----
inout [0:7] gfpga_pad_GPIO_Y;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

input [0:0]sc_head;
output [0:0]sc_tail;
//----- BEGIN wire-connection ports -----
endmodule
