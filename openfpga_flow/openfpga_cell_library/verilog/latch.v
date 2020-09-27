//-----------------------------------------------------
// Design Name : config_latch
// File Name   : config_latch.v
// Coder       : Xifan TANG
//-----------------------------------------------------

//-----------------------------------------------------
// Function    : A Configurable Latch with
//               - an active-high write enable signal
//-----------------------------------------------------
module LATCH (
  input WE, // Write enable
  input D, // Data input
  output Q, // Q output
  output QN // Q negative output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ (WE or D) begin
  if (1'b1 == WE) begin
    q_reg <= D;
  end
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule

//-----------------------------------------------------
// Function    : A Configurable Latch with
//               - an active-high write enable signal
//               - an active-high reset signal
//-----------------------------------------------------
module LATCHR (
  input RST, // Reset signal
  input WE, // Write enable
  input D, // Data input
  output Q, // Q output
  output QN // Q negative output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ (RST or WE or D) begin
  if (RST) begin
    q_reg <= 1'b0;
  end else if (1'b1 == WE) begin
    q_reg <= D;
  end
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule

//-----------------------------------------------------
// Function    : A Configurable Latch with
//               - an active-high write enable signal
//               - an active-low reset signal
//-----------------------------------------------------
module LATCHRN (
  input RSTN, // Reset signal
  input WE, // Write enable
  input D, // Data input
  output Q, // Q output
  output QN // Q negative output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ (RSTN or WE or D) begin
  if (~RSTN) begin
    q_reg <= 1'b0;
  end else if (1'b1 == WE) begin
    q_reg <= D;
  end
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule

//-----------------------------------------------------
// Function    : A Configurable Latch with
//               - an active-high write enable signal
//               - an active-high set signal
//-----------------------------------------------------
module LATCHS (
  input SET, // Set signal
  input WE, // Write enable
  input D, // Data input
  output Q, // Q output
  output QN // Q negative output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ (SET or WE or D) begin
  if (SET) begin
    q_reg <= 1'b1;
  end else if (1'b1 == WE) begin
    q_reg <= D;
  end
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule

//-----------------------------------------------------
// Function    : A Configurable Latch with
//               - an active-high write enable signal
//               - an active-low set signal
//-----------------------------------------------------
module LATCHSN (
  input SETN, // Set signal
  input WE, // Write enable
  input D, // Data input
  output Q, // Q output
  output QN // Q negative output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ (SETN or WE or D) begin
  if (~SETN) begin
    q_reg <= 1'b1;
  end else if (1'b1 == WE) begin
    q_reg <= D;
  end
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule

//-----------------------------------------------------
// Function    : A Configurable Latch with
//               - an active-high write enable signal
//               - an active-high reset signal
//               - an active-high set signal
//-----------------------------------------------------
module LATCHSR (
  input RST, // Reset signal
  input SET, // Set signal
  input WE, // Write enable
  input D, // Data input
  output Q, // Q output
  output QN // Q negative output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ (RST or SET or WE or D) begin
  if (RST) begin
    q_reg <= 1'b0;
  end else if (SET) begin
    q_reg <= 1'b1;
  end else if (1'b1 == WE) begin
    q_reg <= D;
  end
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule

//-----------------------------------------------------
// Function    : A Configurable Latch with
//               - an active-high write enable signal
//               - an active-high reset signal
//               - an active-high set signal
//-----------------------------------------------------
module LATCHSNRN (
  input RSTN, // Reset signal
  input SETN, // Set signal
  input WE, // Write enable
  input D, // Data input
  output Q, // Q output
  output QN // Q negative output
);
//------------Internal Variables--------
reg q_reg;

//-------------Code Starts Here---------
always @ (RSTN or SETN or WE or D) begin
  if (~RSTN) begin
    q_reg <= 1'b0;
  end else if (~SETN) begin
    q_reg <= 1'b1;
  end else if (1'b1 == WE) begin
    q_reg <= D;
  end
end

// Wire q_reg to Q
`ifndef ENABLE_FORMAL_VERIFICATION
  assign Q = q_reg;
  assign QN = ~q_reg;
`else
  assign Q = 1'bZ;
  assign QN = !Q;
`endif

endmodule

