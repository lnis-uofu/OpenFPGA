//-----------------------------------------------------
// Design Name : static_dff
// File Name   : ff.v
// Function    : D flip-flop with asyn reset and set
// Coder       : Xifan TANG
//-----------------------------------------------------
//------ Include defines: preproc flags -----
`include "/research/ece/lnis/USERS/tang/github/OpenFPGA/vpr7_x2p/vpr/test_modes_Verilog/SRC/fpga_defines.v"
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
always @ ( posedge clk or posedge reset or posedge set)
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
always @ ( posedge clk or posedge reset or posedge set)
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
// Function    : Scan-chain D flip-flop without reset and set	//Modified to fit Edouards architecture
// Coder       : Xifan TANG
//-----------------------------------------------------
module sc_dff_compact (
/* Global ports go first */
input reset, // Reset input 
//input set,     // set input
input clk, // Clock Input
/* Local ports follow */
input D, // Data Input
output Q, // Q output 
output Qb // Q output 
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge clk or posedge reset /*or posedge set*/)
if (reset) begin
  q_reg <= 1'b0;
//end else if (set) begin
//  q_reg <= 1'b1;
end else begin
  q_reg <= D;
end
/*
// Wire q_reg to Q
assign Q = q_reg; 
assign Qb = ~Q;
*/

`ifndef ENABLE_FORMAL_VERIFICATION
// Wire q_reg to Q
assign Q = q_reg; 
assign Qb = ~q_reg;
`else
assign Q = 1'bZ; 
assign Qb = !Q;
`endif

endmodule //End Of Module static_dff
