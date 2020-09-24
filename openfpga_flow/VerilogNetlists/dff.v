//-----------------------------------------------------
// Design Name : D-type Flip-flops
// File Name   : ff.v
// Coder       : Xifan TANG
//-----------------------------------------------------

//-----------------------------------------------------
// Function    : A native D-type flip-flop
//-----------------------------------------------------
module DFF (
  input CK, // Clock Input
  input D, // Data Input
  output Q, // Q output
  output QN // QB output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ (posedge CK) begin 
  q_reg <= D;
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule //End Of Module

//-----------------------------------------------------
// Function    : D-type flip-flop with 
//               - asynchronous active high reset
//-----------------------------------------------------
module DFFR (
  input RST, // Reset input
  input CK, // Clock Input
  input D, // Data Input
  output Q, // Q output
  output QN // QB output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge CK or posedge RST)
if (RST) begin
  q_reg <= 1'b0;
end else begin
  q_reg <= D;
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule //End Of Module

//-----------------------------------------------------
// Function    : D-type flip-flop with 
//               - asynchronous active low reset
//-----------------------------------------------------
module DFFRN (
  input RSTN, // Reset input
  input CK, // Clock Input
  input D, // Data Input
  output Q, // Q output
  output QN // QB output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge CK or negedge RSTN)
if (~RSTN) begin
  q_reg <= 1'b0;
end else begin
  q_reg <= D;
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule //End Of Module


//-----------------------------------------------------
// Function    : D-type flip-flop with 
//               - asynchronous active high set
//-----------------------------------------------------
module DFFS (
  input SET, // Set input
  input CK, // Clock Input
  input D, // Data Input
  output Q, // Q output
  output QN // QB output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge CK or posedge SET)
if (SET) begin
  q_reg <= 1'b1;
end else begin
  q_reg <= D;
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule //End Of Module

//-----------------------------------------------------
// Function    : D-type flip-flop with 
//               - asynchronous active low set
//-----------------------------------------------------
module DFFSN (
  input SETN, // Set input
  input CK, // Clock Input
  input D, // Data Input
  output Q, // Q output
  output QN // QB output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge CK or negedge SETN)
if (~SETN) begin
  q_reg <= 1'b1;
end else begin
  q_reg <= D;
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule //End Of Module


//-----------------------------------------------------
// Function    : D-type flip-flop with 
//               - asynchronous active high reset
//               - asynchronous active high set
//-----------------------------------------------------
module DFFSR (
  input SET, // set input
  input RST, // Reset input
  input CK, // Clock Input
  input D, // Data Input
  output Q, // Q output
  output QN // QB output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge CK or posedge RST or posedge SET)
if (RST) begin
  q_reg <= 1'b0;
end else if (SET) begin
  q_reg <= 1'b1;
end else begin
  q_reg <= D;
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule //End Of Module

//-----------------------------------------------------
// Function    : D-type flip-flop with 
//               - asynchronous active high reset
//               - asynchronous active high set
//               - scan-chain input
//               - a scan-chain enable 
//-----------------------------------------------------
module SDFFSR (
  input SET, // Set input
  input RST, // Reset input
  input SE, // Scan-chain Enable
  input SI, // Scan-chain input
  input CK, // Clock Input
  input D, // Data Input
  output Q, // Q output
  output QN // QB output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge CK or posedge RST or posedge SET)
if (RST) begin
  q_reg <= 1'b0;
end else if (SET) begin
  q_reg <= 1'b1;
end else if (SE) begin
  q_reg <= SI;
end else begin
  q_reg <= D;
end

`ifndef ENABLE_FORMAL_VERIFICATION
// Wire q_reg to Q
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule //End Of Module
