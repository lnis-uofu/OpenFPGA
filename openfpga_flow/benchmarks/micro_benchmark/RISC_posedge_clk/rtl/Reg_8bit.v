module Reg_8bit(Q,D,load,clk,rst);

output	[7:0]Q;
input	[7:0]D;
input	load,clk,rst;

reg [7:0]Q;

always@(posedge clk)
	begin
		if(rst==1)Q<=8'b0;
		else if(load==1)Q<=D;
	end

endmodule
		
