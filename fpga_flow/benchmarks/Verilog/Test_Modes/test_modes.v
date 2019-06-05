////////////////////////////////////////////////////////
//                                                    //
//    Benchmark using all modes of k8 architecture    //
//                                                    //
////////////////////////////////////////////////////////

`timescale 1 ns/ 1 ps

module test_modes(
	clk,
	a_0,
	a_1,
	a_2,
	a_3,
	b_0,
	b_1,
	b_2,
	b_3,
	cin,
	e,
	f,
	g,
	sum_0,
	sum_1,
	sum_2,
	sum_3,
	cout,
	x,
	y,
	z );

	input wire clk, a_0, a_1, a_2, a_3, b_0, b_1, b_2, b_3, cin, e, f, g;
	output reg sum_0, sum_1, sum_2, sum_3, cout;
	output wire x, y, z;

	wire d0;
	wire [4:0] n0;
	wire [3:0] a, b;
	reg reg0, reg1, reg2, reg3, reg4, reg5, reg6, reg7, reg8, reg9, reg10, reg11, reg_a_0, reg_a_1, reg_a_2, reg_a_3, reg_b_0, reg_b_1, reg_b_2, reg_b_3, reg_cin;

	assign a = {reg_a_3, reg_a_2, reg_a_1, reg_a_0};
	assign b = {reg_b_3, reg_b_2, reg_b_1, reg_b_0};
	assign d0 = (e && g) || !f;
	assign n0 = a + b + reg_cin;
	assign x = reg3;
	assign y = reg7;
	assign z = reg11;

	always @(posedge clk) begin
		reg0 <= d0;
		reg1 <= reg0;
		reg2 <= reg1;
		reg3 <= reg2;
		reg4 <= reg3;
		reg5 <= reg4;
		reg6 <= reg5;
		reg7 <= reg6;
		reg8 <= reg7;
		reg9 <= reg8;
		reg10 <= reg9;
		reg11 <= reg10;
		reg_a_0 <= a_0;
		reg_a_1 <= a_1;
		reg_a_2 <= a_2;
		reg_a_3 <= a_3;
		reg_b_0 <= b_0;
		reg_b_1 <= b_1;
		reg_b_2 <= b_2;
		reg_b_3 <= b_3;
		reg_cin <= cin;
		sum_0 <= n0[0];
		sum_1 <= n0[1];
		sum_2 <= n0[2];
		sum_3 <= n0[3];
		cout <= n0[4];
	end

endmodule
