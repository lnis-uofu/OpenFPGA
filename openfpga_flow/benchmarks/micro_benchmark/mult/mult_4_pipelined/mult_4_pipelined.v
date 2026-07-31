//-------------------------------------------------------
//  Functionality: A 4-bit multiply circuit with pipelines
//  Author:        Xifan Tang
//-------------------------------------------------------

module mult_4_pipelined(clk, a, b, out);
parameter DATA_WIDTH = 4;  /* declare a parameter. default required */
input [0: DATA_WIDTH - 1] a, b;
input clk;
output [0: DATA_WIDTH * 2 - 1] out;

reg [0 : DATA_WIDTH - 1] a_reg;
reg [0 : DATA_WIDTH - 1] b_reg;
reg [0 : DATA_WIDTH * 2 - 1] out_reg;

always @(posedge clk) begin
  a_reg <= a;
  b_reg <= b;
  out_reg <= a_reg * b_reg;
end

assign out = out_reg;
endmodule









