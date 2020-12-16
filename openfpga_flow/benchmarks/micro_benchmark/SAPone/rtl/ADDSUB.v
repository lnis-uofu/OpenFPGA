module ADDSUB(
	output [7:0] ADDSUB_out,
	input [7:0] ADDSUB_in1,
	input [7:0] ADDSUB_in2,
	input su
	);

	wire [7:0] d;
	
	assign d = su ? ADDSUB_in1 - ADDSUB_in2 : ADDSUB_in1 + ADDSUB_in2;
	assign ADDSUB_out = d;
	
endmodule	
