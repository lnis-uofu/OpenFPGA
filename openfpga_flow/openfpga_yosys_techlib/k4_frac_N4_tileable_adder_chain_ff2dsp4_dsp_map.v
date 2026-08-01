//-----------------------------
// 4-bit multiplier
//-----------------------------
module mult_4x4 (
  input [0:3] A,
  input [0:3] B,
  output [0:7] Y
);
  parameter A_SIGNED = 0;
  parameter B_SIGNED = 0;
  parameter A_WIDTH = 0;
  parameter B_WIDTH = 0;
  parameter Y_WIDTH = 0;

  mult_4 #() _TECHMAP_REPLACE_ (
    .A    (A),
    .B    (B),
    .Y    (Y) );

endmodule
