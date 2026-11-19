//-----------------------------------------------------
// Design Name : frac_mult_16x16
// File Name   : frac_mult_16x16.v
// Function    : A 16-bit multiplier which can operate in fracturable modes:
//               1. two 8-bit multipliers
//               2. one 16-bit multipliers
// Coder       : Xifan Tang
//-----------------------------------------------------

module frac_mult_16x16 (
	input [0:15] a,
	input [0:15] b,
	output [0:31] out,
    input [0:0] mode);

    reg [0:31] out_reg;

    always @(mode, a, b) begin 
       if (1'b1 == mode) begin
         out_reg[0:15] <= a[0:7] * b[0:7];
         out_reg[16:31] <= a[8:15] * b[8:15];
       end else begin
         out_reg <= a * b;
       end
    end

    assign out = out_reg;

endmodule
