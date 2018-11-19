//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Essential gates 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Verilog module for INVTX1 -----
module INVTX1 (
input in,
output out
);
assign out = ~in;
endmodule

//----- Verilog module for buf4 -----
module buf4 (
input in,
output out
);
assign out = in;
endmodule

//----- Verilog module for tap_buf4 -----
module tap_buf4 (
input in,
output out
);
assign out = ~in;
endmodule

//----- Verilog module for TGATE -----
module TGATE (
input in,
input sel,
input selb,
output out
);
assign out = sel? in : 1'bz;
endmodule

