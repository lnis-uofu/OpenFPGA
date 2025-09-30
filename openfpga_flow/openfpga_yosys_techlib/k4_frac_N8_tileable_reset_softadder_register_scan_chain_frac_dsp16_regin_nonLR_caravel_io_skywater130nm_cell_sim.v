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

//-----------------------------
// 8-bit multiplier with input register
//-----------------------------
module mult_8_regin(
  input clk,
  input reset,
  input [0:7] A,
  input [0:7] B,
  output [0:15] Y
);

reg [0:7] A_reg;
reg [0:7] B_reg;
always @(posedge clk or posedge reset) begin
  if (reset == 1'b1) begin
    A_reg <= 0;
    B_reg <= 0;
  end else begin
    A_reg <= A;
    B_reg <= B;
  end
end

assign Y = A_reg * B_reg;

endmodule

