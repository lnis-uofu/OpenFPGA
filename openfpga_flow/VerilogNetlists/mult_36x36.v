//-----------------------------------------------------
// Design Name : mult_36x36
// File Name   : mult_36x36.v
// Function    : A 36-bit multiplier which can operate in fracturable modes:
//               1. four 9-bit multipliers
//               2. two 18-bit multipliers
//               3. one 36-bit multipliers
// Coder       : Xifan Tang
//-----------------------------------------------------

module mult_36x36 (
	input [0:35] a,
	input [0:35] b,
	output [0:71] out,
    input [0:1] mode);

    reg [0:71] out_reg;

    always @(mode, a, b) begin 
       if (2'b01 == mode) begin
         out_reg[0:17] <= a[0:8] * b[0:8];
         out_reg[18:35] <= a[9:17] * b[9:17];
         out_reg[36:53] <= a[18:26] * b[18:26];
         out_reg[54:71] <= a[27:35] * b[27:35];
       end else if (2'b10 == mode) begin
         out_reg[0:35] <= a[0:17] * b[0:17];
         out_reg[36:71] <= a[18:35] * b[18:35];
       end else begin
         out_reg <= a * b;
       end
    end

    assign out = out_reg;

endmodule
