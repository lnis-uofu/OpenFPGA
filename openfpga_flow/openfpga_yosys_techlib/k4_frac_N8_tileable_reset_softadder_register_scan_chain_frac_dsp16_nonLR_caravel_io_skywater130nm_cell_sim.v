//-----------------------------
// 8-bit multiplier
//-----------------------------
module mult_8(
  input [0:7] A,
  input [0:7] B,
  output [0:15] Y
);

assign Y = A * B;

endmodule

//-----------------------------
// 16-bit multiplier
//-----------------------------
module mult_16(
  input [0:15] A,
  input [0:15] B,
  output [0:31] Y
);

assign Y = A * B;

endmodule
