//-----------------------------------------------------
// Design Name : D-type Flip-flops
// File Name   : ff.v
// Coder       : Xifan TANG
//-----------------------------------------------------

//-----------------------------------------------------
// Function    : A native D-type flip-flop with single output
//-----------------------------------------------------
module DFFQ (
  input CK, // Clock Input
  input D, // Data Input
  output Q // Q output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ (posedge CK) begin 
  q_reg <= D;
end

endmodule //End Of Module

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

endmodule //End Of Module

//-----------------------------------------------------
// Function    : D-type flip-flop with 
//               - single output
//               - asynchronous active high reset
//-----------------------------------------------------
module DFFRQ (
  input RST, // Reset input
  input CK, // Clock Input
  input D, // Data Input
  output Q // Q output
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

endmodule //End Of Module

//-----------------------------------------------------
// Function    : D-type flip-flop with 
//               - asynchronous active high reset
//               - asynchronous active high set
//-----------------------------------------------------
module DFFSRQ (
  input SET, // set input
  input RST, // Reset input
  input CK, // Clock Input
  input D, // Data Input
  output Q // Q output
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

assign Q = q_reg;

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
  input CK, // Clock Input
  input SE, // Scan-chain Enable
  input D, // Data Input
  input SI, // Scan-chain input
  output Q, // Q output
  output QN // Q negative output
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

endmodule //End Of Module

//-----------------------------------------------------
// Function    : D-type flip-flop with 
//               - asynchronous active high reset
//               - scan-chain input
//               - a scan-chain enable 
//-----------------------------------------------------
module SDFFRQ (
  input RST, // Reset input
  input CK, // Clock Input
  input SE, // Scan-chain Enable
  input D, // Data Input
  input SI, // Scan-chain input
  output Q // Q output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ ( posedge CK or posedge RST)
if (RST) begin
  q_reg <= 1'b0;
end else if (SE) begin
  q_reg <= SI;
end else begin
  q_reg <= D;
end

assign Q = q_reg;

endmodule //End Of Module

//-----------------------------------------------------
// Function    : D-type flip-flop with 
//               - asynchronous active high reset
//               - asynchronous active high set
//               - scan-chain input
//               - a scan-chain enable 
//-----------------------------------------------------
module SDFFSRQ (
  input SET, // Set input
  input RST, // Reset input
  input CK, // Clock Input
  input SE, // Scan-chain Enable
  input D, // Data Input
  input SI, // Scan-chain input
  output Q // Q output
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

assign Q = q_reg;

endmodule //End Of Module

//-----------------------------------------------------
// Function    : D-type flip-flop with 
//               - asynchronous active high reset
//               - scan-chain input
//               - a scan-chain enable 
//               - a configure enable, when enabled the registered output will
//               be released to the Q
//-----------------------------------------------------
module CFGSDFFR (
  input RST, // Reset input
  input CK, // Clock Input
  input SE, // Scan-chain Enable
  input D, // Data Input
  input SI, // Scan-chain input
  input CFGE, // Configure enable
  output Q, // Regular Q output
  output CFGQ, // Data Q output which is released when configure enable is activated
  output CFGQN // Data Qb output which is released when configure enable is activated
);
//------------Internal Variables--------
reg q_reg;
wire QN;

//-------------Code Starts Here---------
always @ ( posedge CK or posedge RST)
if (RST) begin
  q_reg <= 1'b0;
end else if (SE) begin
  q_reg <= SI;
end else begin
  q_reg <= D;
end

assign CFGQ = CFGE ? Q : 1'b0;
assign CFGQN = CFGE ? QN : 1'b1;

endmodule //End Of Module
