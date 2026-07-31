//-------------------------------------------------------
//  Functionality: A 2-bit multiply circuit with pipelines
//  Author:        Xifan Tang
//-------------------------------------------------------

module mult_2_pipelined(clk, a, b, out);
parameter DATA_WIDTH = 2;  /* declare a parameter. default required */
input [DATA_WIDTH - 1 : 0] a, b;
input clk;
output [DATA_WIDTH - 1 : 0] out;

reg [DATA_WIDTH - 1 : 0] a_reg;
reg [DATA_WIDTH - 1 : 0] b_reg;
reg [DATA_WIDTH - 1 : 0] out_reg;

always @(posedge clk) begin
  a_reg <= a;
  b_reg <= b;
  out_reg <= a_reg * b_reg;
  out = out_reg;
end

endmodule









