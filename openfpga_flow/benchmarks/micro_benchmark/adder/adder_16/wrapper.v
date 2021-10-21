module adder_16_wrapper(
	output cout,
	output sum_0,
	output sum_1,
	output sum_2,
	output sum_3,
	input cin,
	input a_0,
	input a_1,
	input a_2,
	input a_3,
	input b_0,
	input b_1,
	input b_2,
	input b_3
);

wire [4:0] sum;
assign sum_0 = sum[0];
assign sum_1 = sum[1];
assign sum_2 = sum[2];
assign sum_3 = sum[3];
wire [4:0] a;
assign a_0 = a[0];
assign a_1 = a[1];
assign a_2 = a[2];
assign a_3 = a[3];
wire [4:0] b;
assign b_0 = b[0];
assign b_1 = b[1];
assign b_2 = b[2];
assign b_3 = b[3];

adder_16 DUT(
	.cout(cout),
	.sum(sum),
	.cin(cin),
	.a(a),
	.b(b) );

endmodule