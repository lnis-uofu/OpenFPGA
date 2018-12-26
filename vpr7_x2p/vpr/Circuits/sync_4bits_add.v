////////////////////////////////////////
//                                    //
//    Synchronized adder benchmark    //
//                                    //
////////////////////////////////////////

module sync_4bits_add(
	clk,
	rst,
	a0,
	a1,
	a2,
	a3,
	b0,
	b1,
	b2,
	b3,
	cin,
	sumout0,
	sumout1,
	sumout2,
	sumout3,
	cout);

	input clk;
	input rst;
	input a0;
	input a1;
	input a2;
	input a3;
	input b0;
	input b1;
	input b2;
	input b3;
	input cin;
	output sumout0;
	output sumout1;
	output sumout2;
	output sumout3;
	output reg cout;

	wire[3:0] a;	
	wire[3:0] b;
	reg[3:0] sumout;

	reg[3:0] reg_a;
	reg[3:0] reg_b;
	reg reg_cin;
	wire[4:0] int_sum;
	
	assign a[3] = a3;
	assign a[2] = a2;
	assign a[1] = a1;
	assign a[0] = a0;
	assign b[3] = b3;
	assign b[2] = b2;
	assign b[1] = b1;
	assign b[0] = b0;
	assign sumout3 = sumout[3];
	assign sumout2 = sumout[2];
	assign sumout1 = sumout[1];
	assign sumout0 = sumout[0];

	assign int_sum = reg_a + reg_b + reg_cin;

	always@(posedge clk or posedge rst) begin
		if(rst) begin
			reg_a <= 4'h0;
			reg_b <= 4'h0;
			reg_cin <= 1'h0;
		end
		else begin
			reg_a <= a;
			reg_b <= b;
			reg_cin <= cin;
		end
	end

	always@(posedge clk or posedge rst) begin
		if(rst) begin
			sumout <= 4'h0;
			cout <= 1'h0;
		end
		else begin
			sumout <= int_sum[3:0];
			cout <= int_sum[4];
		end
	end

endmodule
