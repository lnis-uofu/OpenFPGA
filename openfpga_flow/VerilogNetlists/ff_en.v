//-----------------------------------------------------
// Design Name : D-type Flip-flop with Write Enable 
// File Name   : ff_en.v
// Function    : D flip-flop with asyn reset and set
// Coder       : Xifan TANG
//-----------------------------------------------------
module DFF_EN (
/* Global ports go first */
input SET,     // set input
input RST, // Reset input
input WE, // Write Enable
input CK, // Clock Input
/* Local ports follow */
input D, // Data Input
output Q // Q output
output QB // QB output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge CK or posedge RST or posedge SET)
if (RESET) begin
  q_reg <= 1'b0;
end else if (SET) begin
  q_reg <= 1'b1;
end else if (WE) begin
  q_reg <= D;
end

`ifndef ENABLE_FORMAL_VERIFICATION
// Wire q_reg to Q
assign Q = q_reg;
assign QB = ~q_reg;
`else
assign Q = 1'bZ;
assign QB = !Q;
`endif

endmodule //End Of Module
