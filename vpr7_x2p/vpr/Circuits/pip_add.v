////////////////////////////////////////
//                                    //
//      Pipelined adder benchmark     //
//                                    //
////////////////////////////////////////

module pip_add(
	rst,
	clk,
	a_0,
	a_1,
	a_2,
	a_3,
	a_4,
	a_5,
	a_6,
	a_7,
	b_0,
	b_1,
	b_2,
	b_3,
	b_4,
	b_5,
	b_6,
	b_7,
	cin,
	sumout_0,
	sumout_1,
	sumout_2,
	sumout_3,
	sumout_4,
	sumout_5,
	sumout_6,
	sumout_7,
	cout);

	input rst;
	input clk;
	input a_0;
	input a_1;
	input a_2;
	input a_3;
	input a_4;
	input a_5;
	input a_6;
	input a_7;
	input b_0;
	input b_1;
	input b_2;
	input b_3;
	input b_4;
	input b_5;
	input b_6;
	input b_7;
	input cin;
	output sumout_0;
	output sumout_1;
	output sumout_2;
	output sumout_3;
	output sumout_4;
	output sumout_5;
	output sumout_6;
	output sumout_7;
	output reg cout;

	reg[7:0] reg0_a;
	reg[7:0] reg0_b;
	reg[7:0] reg1_a;
	reg[7:0] reg1_b;
	reg[7:0] reg2_a;
	reg[7:0] reg2_b;
	reg reg0_cin;
	reg reg1_cin;
	reg reg2_cin;
	wire[8:0] int_sum;
	wire int_cout;
	wire[7:0] a;
	wire[7:0] b;
	reg[7:0] sumout;

	assign a = {a_7, a_6, a_5, a_4, a_3, a_2, a_1, a_0};
	assign b = {b_7, b_6, b_5, b_4, b_3, b_2, b_1, b_0};
	assign int_sum = reg2_a + reg2_b + reg2_cin;
	assign int_cout = int_sum[8] || 1'b0;
	assign sumout_0 = sumout[0];
	assign sumout_1 = sumout[1];
	assign sumout_2 = sumout[2];
	assign sumout_3 = sumout[3];
	assign sumout_4 = sumout[4];
	assign sumout_5 = sumout[5];
	assign sumout_6 = sumout[6];
	assign sumout_7 = sumout[7];

	always@(posedge clk or posedge rst) begin
		if(rst) begin
			reg0_a <= 8'h00;
			reg0_b <= 8'h00;
			reg0_cin <= 1'b0;
		end
		else begin
			reg0_a <= a;
			reg0_b <= b;
			reg0_cin <= cin;
		end
	end
	always@(posedge clk or posedge rst) begin
		if(rst) begin
			reg1_a <= 8'h00;
			reg1_b <= 8'h00;
			reg1_cin <= 1'b0;
		end
		else begin
			reg1_a <= reg0_a;
			reg1_b <= reg0_b;
			reg1_cin <= reg0_cin;
		end
	end
	always@(posedge clk or posedge rst) begin
		if(rst) begin
			reg2_a <= 8'h00;
			reg2_b <= 8'h00;
			reg2_cin <= 1'b0;
		end
		else begin
			reg2_a <= reg1_a;
			reg2_b <= reg1_b;
			reg2_cin <= reg1_cin;
		end
	end

	always@(posedge clk or posedge rst) begin
		if(rst) begin
			sumout <= 8'h00;
			cout <= 1'b0;
		end
		else begin
			sumout <= int_sum[7:0];
			cout <= int_cout;
		end
	end

endmodule
