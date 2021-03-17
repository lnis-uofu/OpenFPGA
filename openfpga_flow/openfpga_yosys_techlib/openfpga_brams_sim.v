module dpram (
	input clk,
	input wen,
	input ren,
	input[9:0] waddr,
	input[9:0] raddr,
	input[31:0] d_in,
	output[31:0] d_out );

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
	input[9:0] waddr,
	input[31:0] data_in,
	input rclk,
	input ren,
	input[9:0] raddr,
	output[31:0] d_out );

	reg[31:0] ram[1023:0];
	reg[31:0] internal;

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

//---------------------------------------
// A single-port 32x8bit RAM 
// This module is tuned for VTR's benchmarks
//---------------------------------------
module single_port_ram (
	input clk,
	input we,
	input [4:0] addr,
	input [7:0] data,
	output [7:0] out );

	reg [7:0] ram[31:0];
	reg [7:0] internal;

	assign out = internal;

	always @(posedge clk) begin
		if(wen) begin
			ram[addr] <= data;
		end

		if(ren) begin
			internal <= ram[addr];
		end
	end

endmodule
