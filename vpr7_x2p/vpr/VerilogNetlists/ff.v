//-----------------------------------------------------
// Design Name : static_dff
// File Name   : ff.v
// Function    : D flip-flop with asyn reset and set
// Coder       : Xifan TANG
//-----------------------------------------------------
module static_dff (
/* Global ports go first */
input set,     // set input
input reset, // Reset input 
input clk, // Clock Input
/* Local ports follow */
input D, // Data Input
output Q // Q output 
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge clk or reset or set)
if (reset) begin
  q_reg <= 1'b0;
end else if (set) begin
  q_reg <= 1'b1;
end else begin
  q_reg <= D;
end

// Wire q_reg to Q
assign Q = q_reg; 

endmodule //End Of Module static_dff

//-----------------------------------------------------
// Design Name : scan_chain_dff
// File Name   : ff.v
// Function    : D flip-flop with asyn reset and set
// Coder       : Xifan TANG
//-----------------------------------------------------
module sc_dff (
/* Global ports go first */
input set,     // set input
input reset, // Reset input 
input clk, // Clock Input
/* Local ports follow */
input D, // Data Input
output Q, // Q output 
output Qb // Q output 
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge clk or reset or set)
if (reset) begin
  q_reg <= 1'b0;
end else if (set) begin
  q_reg <= 1'b1;
end else begin
  q_reg <= D;
end

// Wire q_reg to Q
assign Q = q_reg; 
assign Qb = ~Q;

endmodule //End Of Module static_dff

//-----------------------------------------------------
// Design Name : scan_chain_dff compact
// File Name   : ff.v
// Function    : Scan-chain D flip-flop without reset and set
// Coder       : Xifan TANG
//-----------------------------------------------------
module sc_dff_compact (
/* Global ports go first */
input reset, // Reset input 
input clk, // Clock Input
input clkb, // Clock Input
/* Local ports follow */
input D, // Data Input
output Q, // Q output 
output Qb // Q output 
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge clk or reset)
if (reset) begin
  q_reg <= 1'b0;
end else begin
  q_reg <= D;
end

// Wire q_reg to Q
assign Q = q_reg; 
assign Qb = ~Q;

endmodule //End Of Module static_dff
