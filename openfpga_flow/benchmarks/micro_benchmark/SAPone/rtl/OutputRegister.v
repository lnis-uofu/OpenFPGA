module OutputRegister(
	output reg [7:0] OutputRegister_out,
	input [7:0] OutputRegister_in,
	input lo_,
	input clk,
	input clr_
	);

	always @(posedge clk)
		if(~clr_) OutputRegister_out <= 8'b0;
		else if(~lo_) OutputRegister_out <= OutputRegister_in;

endmodule
