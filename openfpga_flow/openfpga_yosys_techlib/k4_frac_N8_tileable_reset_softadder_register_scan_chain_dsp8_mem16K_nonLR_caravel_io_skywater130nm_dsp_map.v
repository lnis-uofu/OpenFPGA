//-----------------------------
// 8-bit multiplier
//-----------------------------
module mult_8x8 (
  input [0:7] A,
  input [0:7] B,
  output [0:15] Y
);
  parameter A_SIGNED = 0;
  parameter B_SIGNED = 0;
  parameter A_WIDTH = 0;
  parameter B_WIDTH = 0;
  parameter Y_WIDTH = 0;

  mult_8 #() _TECHMAP_REPLACE_ (
    .A    (A),
    .B    (B),
    .Y    (Y) );

endmodule
