//-------------------------------------------------------
//  Functionality: A 8-bit combinational multiply circuit
//-------------------------------------------------------

module mult8(a_0, a_1, a_2, a_3, a_4, a_5, a_6, a_7,
			b_0, b_1, b_2, b_3, b_4, b_5, b_6, b_7,
			out_0, out_1, out_2, out_3, out_4, out_5, out_6, out_7,
			out_8, out_9, out_10, out_11, out_12, out_13, out_14, out_15);
input a_0, a_1, a_2, a_3, a_4, a_5, a_6, a_7;
input b_0, b_1, b_2, b_3, b_4, b_5, b_6, b_7;
output out_0, out_1, out_2, out_3, out_4, out_5, out_6, out_7;
output out_8, out_9, out_10, out_11, out_12, out_13, out_14, out_15;

 assign a = {a_0, a_1, a_2, a_3, a_4, a_5, a_6, a_7};
 assign b = {b_0, b_1, b_2, b_3, b_4, b_5, b_6, b_7};
 assign out = {out_0, out_1, out_2, out_3, out_4, out_5, out_6, out_7, out_8, out_9, out_10, out_11, out_12, out_13, out_14, out_15};

 assign out = a*b;

endmodule


