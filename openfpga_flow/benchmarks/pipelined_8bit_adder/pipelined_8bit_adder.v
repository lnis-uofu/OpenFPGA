//-----------------------------------------------------
// Design Name : pipelined_8bit_adder
// File Name   : pipelined_8bit_adder.v
// Function    : Pipelined 8-bit adders, whose sum and carry outputs
//               are cached in a memory
// Coder       : Aurelien Alacchi
//-----------------------------------------------------

`timescale 1 ns/ 1 ps

// To match the port definition in BLIF format, so that we can do verification
// Each input/output bus is expanded here.
// In future, we should be able to support buses in verification!

module pipelined_8bit_adder(
    input clk,
    input ren,
    input wen,
    input raddr_0_,
    input raddr_1_,
    input raddr_2_,
    input raddr_3_,
    input raddr_4_,
    input raddr_5_,
    input waddr_0_,
    input waddr_1_,
    input waddr_2_,
    input waddr_3_,
    input waddr_4_,
    input waddr_5_,
    input a_0_,
    input a_1_,
    input a_2_,
    input a_3_,
    input a_4_,
    input a_5_,
    input a_6_,
    input b_0_,
    input b_1_,
    input b_2_,
    input b_3_,
    input b_4_,
    input b_5_,
    input b_6_,
    output q_0_,
    output q_1_,
    output q_2_,
    output q_3_,
    output q_4_,
    output q_5_,
    output q_6_,
    output q_7_);

	wire [5:0] raddr;
	wire [5:0] waddr;
	wire [6:0] a;
	wire [6:0] b;
	wire [7:0] q;


	assign raddr = { raddr_5_, raddr_4_, raddr_3_, raddr_2_, raddr_1_, raddr_0_ };
	assign waddr = { waddr_5_, waddr_4_, waddr_3_, waddr_2_, waddr_1_, waddr_0_ };
	assign a = { a_6_, a_5_, a_4_, a_3_, a_2_, a_1_, a_0_ };
	assign b = { b_6_, b_5_, b_4_, b_3_, b_2_, b_1_, b_0_ };
    assign q_7_ = q[7];
    assign q_6_ = q[6];
    assign q_5_ = q[5];
    assign q_4_ = q[4];
    assign q_3_ = q[3];
    assign q_2_ = q[2];
    assign q_1_ = q[1];
    assign q_0_ = q[0];

	reg[7:0] ram[63:0];
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
