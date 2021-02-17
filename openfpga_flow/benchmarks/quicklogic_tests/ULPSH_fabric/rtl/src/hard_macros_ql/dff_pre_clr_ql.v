
`timescale 1ns / 1ns


module dff_pre_clr (
	input	CLK,
	input	CLR,
	input	D,
	input	PRE,
	output	Q
);

dffpc dffpc_0 ( .CLK(CLK) , .CLR(CLR), .D(D), .PRE(PRE), .Q(Q) )/* synthesis black_box */;
//pragma attribute dffpc_0 dont_touch true

endmodule


