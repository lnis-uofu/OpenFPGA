//-----------------------------
// 8-bit counter with multiple match values
//-----------------------------
`default_nettype none 
module cnt_8 #(
  parameter [0:7] MATCH0_REF = {8{1'b0}},
  parameter [0:7] MATCH1_REF = {8{1'b0}}
)(
  input clk,
  input rst,
  output match0,
  output match1
);
reg [0:7] q_reg;

always @(posedge clk) begin
  if (~rst_i)
    q_reg <= 0;
  else
    q_reg <= q_reg + 1;
end

assign match0 = (q_reg == MATCH0_REF) ? 1 : 0;
assign match1 = (q_reg == MATCH1_REF) ? 1 : 0;

endmodule

`default_nettype wire
