module BRegister(
	output reg [7:0] BRegister_out,
	input [7:0] BRegister_in,
	input lb_,
	input clk,
	input clr_
	);

	always @(posedge clk)
		if(~clr_) BRegister_out <= 8'b0;
		else if(~lb_) BRegister_out <= BRegister_in;	

endmodule
