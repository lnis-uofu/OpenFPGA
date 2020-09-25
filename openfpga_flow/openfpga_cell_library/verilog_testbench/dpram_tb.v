//-----------------------------------------------------
// Design Name : dual_port_ram_tb
// File Name   : memory_wrapper_tb.v
// Function    : Dual port RAM 64x2048 
// Coder       : Aurelien
//-----------------------------------------------------
`timescale 1 ns/1 ps

module dpram_tb ();
	reg clk;
	reg wen;
	reg ren;
	reg[0:9] waddr;
	reg[0:9] raddr;
	reg[0:31] d_in;
	wire[0:31] d_out;

	integer count;
	integer lim_max = 1023;

		dpram memory_0 (
			.clk		(clk),
			.wen		(wen),
			.waddr		(waddr),
			.d_in		(d_in),
			.ren		(ren),
			.raddr		(raddr),
			.d_out		(d_out) );

	initial begin
		clk <= 1'b0;
		ren <= 1'b0;
		wen <= 1'b0;
		raddr <= 10'h000;
		waddr <= 10'h000;
		d_in <= 32'h00000000;
		for(count = 0; count < lim_max; count = count +1) begin
			#5
			wen <= 1'b1;
			clk <= !clk;
			if(clk) begin
				waddr <= waddr + 1;
			end
		end	
		wen <= 1'b0;
		for(count = 0; count < lim_max; count = count +1) begin
			#5
			ren <= 1'b1;
			clk <= !clk;
			if(clk) begin
				raddr <= raddr + 1;
			end
		end	
		$finish;
	end

	always@(negedge clk) begin
		d_in <= $random;
	end

endmodule
