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

// Wire q_reg to Q
assign Q = q_reg;

endmodule //End Of Module
