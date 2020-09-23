module MAR(
	output reg [3:0] mar_out,
	input wire [3:0] mar_in,
	input lm_,
	input clk,
	input clr_
	);
	
	always @(posedge clk)
		if(~clr_) mar_out <= 4'b0;
		else if(~lm_) mar_out <= mar_in;	

endmodule
