
`timescale 1ns / 1ns

module clock_buffer (
	input	A,
	output	Z
);



GCLKBUFF gclkbuff_0 (.A(A), .Z(Z));
//pragma attribute gclkbuff_0 ql_pack 1
//pragma attribute gclkbuff_0 hierarchy preserve

endmodule


