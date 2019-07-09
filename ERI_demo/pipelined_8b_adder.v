/////////////////////////////////////
//                                 //
//    ERI summit demo-benchmark    //
//      pipelined_8b_adder.v       //
//          by Aurelien            // 
//                                 //
/////////////////////////////////////

`timescale 1 ns/ 1 ps

module pipelined_8b_adder(
	clk,
	raddr,
	waddr,
	ren,
	wen,
	a,
	b,
	q );

	input clk;
	input[5:0] raddr;
	input[5:0] waddr;
	input ren;
	input wen;
	input[6:0] a;
	input[6:0] b;
	output[7:0] q;

	reg[63:0] ram[7:0];
	reg[6:0] a_st0;
	reg[6:0] a_st1;
	reg[6:0] b_st0;
	reg[6:0] b_st1;
	reg[8:0] waddr_st0;
	reg[8:0] waddr_st1;
	reg wen_st0;
	reg wen_st1;
	reg[7:0] q_int;

	wire[7:0] AplusB;

	assign AplusB = a_st1 + b_st1;
	assign q = q_int;

	always@(posedge clk) begin
		waddr_st0 <= waddr;
		waddr_st1 <= waddr_st0;
		a_st0 <= a;
		a_st1 <= a_st0;
		b_st0 <= b;
		b_st1 <= b_st0;
		wen_st0 <= wen;
		wen_st1 <= wen_st0;
		if(wen_st1) begin
			ram[waddr_st1] <= AplusB;
		end
		if(ren) begin
			q_int <= ram[raddr];
		end
	end

endmodule
