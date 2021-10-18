///////////////////////////////////////////
//  Functionality: Counter with asynchronous reset
//  Author:        Xifan Tang
////////////////////////////////////////

module counter (
	clk,
	resetb,
	result
);

	input clk;
	input resetb;
	output [7:0] result;

	reg [7:0] result;

	always @(posedge clk or negedge resetb)
	begin
		if (!resetb) 
			result = 0;		
		else 
			result = result + 1;
	end
endmodule		
