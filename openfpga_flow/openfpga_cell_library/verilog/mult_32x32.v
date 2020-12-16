//-----------------------------------------------------
// Design Name : mult_32x32
// File Name   : mult_32x32.v
// Function    : A 32-bit multiplier which can operate in fracturable modes:
//               1. four 8-bit multipliers
//               2. two 16-bit multipliers
//               3. one 32-bit multipliers
// Coder       : Xifan Tang
//-----------------------------------------------------

module mult_32x32 (
	input [0:31] a,
	input [0:31] b,
	output [0:63] out,
    input [0:1] mode);

    reg [0:63] out_reg;

    always @(mode, a, b) begin 
       if (2'b01 == mode) begin
         out_reg[0:15] <= a[0:7] * b[0:7];
         out_reg[16:31] <= a[8:15] * b[8:15];
         out_reg[32:47] <= a[16:23] * b[16:23];
         out_reg[48:63] <= a[24:31] * b[24:31];
       end else if (2'b10 == mode) begin
         out_reg[0:31] <= a[0:15] * b[0:15];
         out_reg[32:63] <= a[16:31] * b[16:31];
       end else begin
         out_reg <= a * b;
       end
    end

    assign out = out_reg;

endmodule
