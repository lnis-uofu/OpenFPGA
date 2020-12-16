module ACC(
	output [7:0] acc_out1,
	output [7:0] acc_out2,
	input [7:0] acc_in,
	input la_,
	input clk,
	input clr_
	);
	
	reg [7:0] q;
	
	always @(posedge clk)
		if (~clr_) q <= 8'b0;
		else if(~la_) q <= acc_in;	
	
	assign acc_out1 = q;
	assign acc_out2 = q;
	
endmodule
