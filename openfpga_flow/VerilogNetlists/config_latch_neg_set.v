//-----------------------------------------------------
// Design Name : config_latch
// File Name   : config_latch.v
// Function    : A Configurable Latch where data storage
//               can be updated when wl is enabled
//               Reset is active low
// Coder       : Xifan TANG
//-----------------------------------------------------
module config_latch_neg_set (
  input setb, // Reset input
  input wl, // Data Enable
  input bl, // Data Input
  output Q, // Q output
  output Qb // Q output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ (setb or wl or bl) begin
  if (~setb) begin
    q_reg <= 1'b1;
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
