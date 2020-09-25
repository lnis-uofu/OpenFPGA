//-----------------------------------------------------
// Design Name : lut6
// File Name   : lut6.v
// Function    : 6-input Look Up Table
// Coder       : Xifan TANG
//-----------------------------------------------------
module lut6 (
input [5:0] in,
output out,
input [63:0] sram,
input [63:0] sram_inv);

  assign out = sram[in];

endmodule
