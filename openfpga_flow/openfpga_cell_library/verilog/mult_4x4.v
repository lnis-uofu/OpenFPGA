//-----------------------------------------------------
// Design Name : mult_4x4
// File Name   : mult_4x4.v
// Function    : A 4-bit multiplier
// Coder       : Xifan Tang
//-----------------------------------------------------

module mult_4x4 (
  input [0:3] A,
  input [0:3] B,
  output [0:7] Y
);

  assign Y = A * B;

endmodule
