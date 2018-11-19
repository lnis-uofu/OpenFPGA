//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Look-Up Tables 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//-----LUT module, verilog_model_name=lut4 -----
module lut4 (input wire [0:3] in,
output wire [0:0] out,
input wire [0:15] sram_out,
input wire [0:15] sram_outb
);
  wire [0:3] in_b;
  assign in_b = ~ in;
  lut4_mux lut4_mux_0_ ( sram_out, out, in, in_b);
endmodule
//-----END LUT module, verilog_model_name=lut4 -----

