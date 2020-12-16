//-----------------------------------------------------
// Design Name : dual_port_ram
// File Name   : dpram.v
// Function    : Dual port RAM 8x128 
// Coder       : Xifan Tang
//-----------------------------------------------------

module dpram_128x8 (
	input clk,
	input wen,
	input ren,
	input[0:6] waddr,
	input[0:6] raddr,
	input[0:7] d_in,
	output[0:7] d_out );

		dual_port_sram memory_0 (
			.wclk		(clk),
			.wen		(wen),
			.waddr		(waddr),
			.data_in	(d_in),
			.rclk		(clk),
			.ren		(ren),
			.raddr		(raddr),
			.d_out		(d_out) );

endmodule

module dual_port_sram (
	input wclk,
	input wen,
	input[0:6] waddr,
	input[0:7] data_in,
	input rclk,
	input ren,
	input[0:6] raddr,
	output[0:7] d_out );

	reg[0:7] ram[0:127];
	reg[0:7] internal;

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
