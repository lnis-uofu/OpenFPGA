//-----------------------------
// 4-bit multiplier
//-----------------------------
module mult_4(
  input [0:3] A,
  input [0:3] B,
  output [0:7] Y
);

assign Y = A * B;

endmodule
