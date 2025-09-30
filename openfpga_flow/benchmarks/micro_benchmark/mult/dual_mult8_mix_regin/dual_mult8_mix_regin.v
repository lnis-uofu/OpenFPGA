//-------------------------------------------------------
//  Functionality: Two 8-bit multiply circuit using macro, one with input registers while the other without
//  Author:        Xifan Tang
//-------------------------------------------------------

module dual_mult8_mix_regin(clk, rst, a0, b0, out0, a1, b1, out1);
parameter DATA_WIDTH = 8;  /* declare a parameter. default required */
input clk;
input rst;
input [0 : DATA_WIDTH - 1] a0, b0, a1, b1;
output [0 : 2*DATA_WIDTH - 1] out0, out1;

  mult_8 mult8_dsp (
		.A(a0),
		.B(b0),
		.Y(out0)
	);

  mult_8_regin mult8_regin_dsp (
        .clk(clk),
        .reset(rst),
		.A(a1),
		.B(b1),
		.Y(out1)
	);

endmodule
