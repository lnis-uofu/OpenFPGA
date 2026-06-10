//-----------------------------------------------------
// Design Name : dpram_8x16_preload
// File Name   : dpram128preload.v
// Function    : Dual port RAM 8x16 with preload blocks for initialization 
// Coder       : Xifan Tang
//-----------------------------------------------------

module dpram_8x16_preload (
	input clk,
	input wen,
	input ren,
	input[0:2] waddr,
	input[0:2] raddr,
	input[0:15] d_in,
	output[0:15] d_out );

// Preload block for initialization

// Core memory block 
		dpram_8x16_core memory_0 (
			.wclk		(clk),
			.wen		(wen),
			.waddr		(waddr),
			.data_in	(d_in),
			.rclk		(clk),
			.ren		(ren),
			.raddr		(raddr),
			.d_out		(d_out) );

endmodule

module dpram_8x16_core (
	input wclk,
	input wen,
	input[0:2] waddr,
	input[0:15] data_in,
	input rclk,
	input ren,
	input[0:2] raddr,
	output[0:15] d_out );

	reg[0:15] ram[0:2];
	reg[0:15] internal;

	assign d_out = internal;

	always @(posedge wclk) begin
		if(wen) begin
			ram[waddr] <= data_in;
		end
	end

	always @(posedge rclk) begin
		if(ren) begin
			internal <= ram[raddr];
		end
	end

endmodule
