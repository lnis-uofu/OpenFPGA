//-----------------------------------------------------
// Design Name : config_latch
// File Name   : config_latch.v
// Function    : A Configurable Latch where data storage
//               can be updated at rising clock edge 
//               when wl is enabled
//               Reset is active low
// Coder       : Xifan TANG
//-----------------------------------------------------
module config_latch (
  input resetb, // Reset input
  input clk, // Clock Input
  input wl, // Data Enable
  input bl, // Data Input
  output Q, // Q output
  output Qb // Q output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge clk or posedge resetb) begin
  if (~resetb) begin
    q_reg <= 1'b0;
  end else if (1'b1 == wl) begin
    q_reg <= bl;
  end
end

`ifndef ENABLE_FORMAL_VERIFICATION
// Wire q_reg to Q
assign Q = q_reg;
assign Qb = ~q_reg;
`else
assign Q = 1'bZ;
assign Qb = !Q;
`endif

endmodule
