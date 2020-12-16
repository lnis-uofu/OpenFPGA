 module PC(
	output reg [3:0] pc_out,
	input cp,
	input clk,
	input clr_
	);
	
	always @(posedge clk)
	begin
		if(~clr_) pc_out <= 0;
		else if (cp) pc_out <= pc_out + 1;
	end
			
endmodule

