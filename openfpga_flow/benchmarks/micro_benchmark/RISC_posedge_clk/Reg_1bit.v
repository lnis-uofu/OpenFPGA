module Reg_1bit(Q,D,load,clk,rst);

output	Q;
input	D;
input	load,clk,rst;

reg 	Q;

always@(posedge clk)
	begin
		if(rst==1)Q<=0;
		else if(load==1)Q<=D;
	end

endmodule
		
