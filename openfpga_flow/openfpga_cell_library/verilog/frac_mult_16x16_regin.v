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

// Mode[0]: switch between dual 8-bit and single 16-bit mode
// Mode[1]: Enable/disable registers on the first multiplier in the dual 8-bit mode
// Mode[2]: Enable/disable registers on the second multiplier in the dual 8-bit mode
module frac_mult_16x16_regin (
	input [0:15] a,
	input [0:15] b,
	input ck,
	input rst,
	output [0:31] out,
    input [0:2] mode);

  reg [0:15] a_reg;
  reg [0:15] b_reg;
  wire [0:15] mult_a;
  wire [0:15] mult_b;

  always @(posedge ck or posedge rst) begin
    if (rst == 1'b1) begin
      a_reg <= 0;
      b_reg <= 0;
    end else begin
      a_reg <= a;
      b_reg <= b;
    end
  end

  assign mult_a[0:7] = mode[1] ? a_reg[0:7] : a[0:7]; 
  assign mult_a[8:15] = mode[2] ? a_reg[8:15] : a[8:15]; 
  assign mult_b[0:7] = mode[1] ? b_reg[0:7] : b[0:7]; 
  assign mult_b[8:15] = mode[2] ? b_reg[8:15] : b[8:15]; 

  frac_mult_16x16 mult_core (
    .a(mult_a),
    .b(mult_b),
    .out(out),
    .mode(mode[0])
  );

endmodule
