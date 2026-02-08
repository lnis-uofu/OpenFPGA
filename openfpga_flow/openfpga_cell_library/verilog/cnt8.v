//-----------------------------------------------------
// Design Name : Multi-bit Full Adder
// File Name   : adder.v
// Coder       : Xifan TANG
//-----------------------------------------------------

//-----------------------------------------------------
// Function    : A 1-bit full adder
//-----------------------------------------------------
module cnt8(
  input [0:0] CK, // Input CLOCK
  input [0:0] RST, // Input RESET
  input [0:15] MODE, // MODE SELECTION
  output [0:0] M0, // Output match0
  output [0:0] M1 // Output match1
);
reg [0:7] q_reg;
always @(posedge CK) begin
  if (~RST)
    q_reg <= 0;
  else
    q_reg <= q_reg + 1;
end 

assign M0 = (q_reg == MODE[0:7]) ? 1 : 0;
assign M1 = (q_reg == MODE[8:15]) ? 1 : 0;
  
endmodule

