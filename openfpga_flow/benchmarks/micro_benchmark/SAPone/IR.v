module IR(
	output [7:4] opcode,
	output [3:0] oprand,
	input wire [7:0] IR_in,
	input li_,
	input clk,
	input clr_
	);
	
	reg [7:0] q;

	always @(posedge clk)
	begin
		if(~clr_) q <=8'b0;
		else if(~li_) q <= IR_in;
	end
	
	assign opcode = q[7:4];
	assign oprand = q[3:0];

endmodule