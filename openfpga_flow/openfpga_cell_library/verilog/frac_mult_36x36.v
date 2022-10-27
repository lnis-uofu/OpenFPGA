//----------------------------------------------------------------------------
// Design Name : frac_mult_36x36
// File Name   : frac_mult_36x36.v
// Function    : A 36-bit multiplier which form from 9-bit multipliers.
//               It can operate in 3 fracturable modes:
//                  1. one 36-bit multiplier  : mode[0] == 0, mode[1] == 0
//                  2. two 18-bit multipliers : mode[0] == 1, mode[1] == 0
//                  3. four 9-bit multipliers : mode[0] == 1, mode[1] == 1
// Coder       : mustafaarslan0
//----------------------------------------------------------------------------

module frac_mult_36x36 
(
	input   wire [0:35] a,
	input   wire [0:35] b,
	output  wire [0:71] out,
  input   wire [0:1]  mode  
);

wire [0:35] mult_ll_out;
wire [0:35] mult_lh_out;
wire [0:35] mult_hl_out;
wire [0:35] mult_hh_out;

wire [0:36] sub_result1; // carry included
wire [0:35] sub_result2;
wire [0:71] result;

  assign sub_result1 = mult_lh_out + mult_hl_out + {18'd0, mult_ll_out[0:17]};
  assign sub_result2 = mult_hh_out + {17'd0, sub_result1[0:18]};

  assign result[54:71]   = mult_ll_out[18:35];
  assign result[36:53]  = sub_result1[19:36];
  assign result[0:35]  = sub_result2;

  assign out[36:71] = (mode[0] == 1'b1) ? mult_ll_out : result[36:71];
  assign out[0:35]  = (mode[0] == 1'b1) ? mult_hh_out : result[0:35];                              

  frac_mult_18x18 mult_ll (.a(a[18:35]), .b(b[18:35]), .out(mult_ll_out), .mode(mode[1]) ); // A_low*B_low
  frac_mult_18x18 mult_lh (.a(a[18:35]), .b(b[0:17]), .out(mult_lh_out), .mode(1'b0) ); // A_low*B_high
  frac_mult_18x18 mult_hl (.a(a[0:17]), .b(b[18:35]), .out(mult_hl_out), .mode(1'b0) ); // A_high*B_low
  frac_mult_18x18 mult_hh (.a(a[0:17]), .b(b[0:17]),  .out(mult_hh_out), .mode(mode[1])); // A_high*B_high

endmodule

module frac_mult_18x18
(
	input   wire [0:17] a,
	input   wire [0:17] b,
	output  wire [0:35] out,
  input   wire [0:0]  mode  
);

wire [0:17] mult_ll_out;
wire [0:17] mult_lh_out;
wire [0:17] mult_hl_out;
wire [0:17] mult_hh_out;

wire [0:18] sub_result1;  // carry included
wire [0:17] sub_result2;
wire [0:35] result;

  assign sub_result1 = mult_lh_out + mult_hl_out + {9'd0, mult_ll_out[0:8]};
  assign sub_result2 = mult_hh_out + {8'd0, sub_result1[0:9]};

  assign result[27:35] = mult_ll_out[9:17];
  assign result[18:26] = sub_result1[10:18];
  assign result[0:17]  = sub_result2;

  assign out[18:35] = (mode == 1'b1) ? mult_ll_out : result[18:35];
  assign out[0:17]  = (mode == 1'b1) ? mult_hh_out : result[0:17];

  multiplier #(9) mult_ll (.a(a[9:17]), .b(b[9:17]), .out(mult_ll_out) ); // A_low*B_low
  multiplier #(9) mult_lh (.a(a[9:17]), .b(b[0:8]), .out(mult_lh_out) ); // A_low*B_high
  multiplier #(9) mult_hl (.a(a[0:8]), .b(b[9:17]), .out(mult_hl_out) ); // A_high*B_low
  multiplier #(9) mult_hh (.a(a[0:8]), .b(b[0:8]), .out(mult_hh_out) ); // A_high*B_high

endmodule

module multiplier
#( parameter WIDTH = 9 )
(
	input   wire [0:WIDTH-1] a,
	input   wire [0:WIDTH-1] b,
	output  wire [0:2*WIDTH-1] out 
);

  assign  out = a * b;

endmodule
