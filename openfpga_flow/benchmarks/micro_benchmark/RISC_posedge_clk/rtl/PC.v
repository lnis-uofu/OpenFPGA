module PC(PC_out,PC_in,load,inc,clk,rst);

output	[7:0]PC_out;
input	[7:0]PC_in;
input	load,inc,clk,rst;

reg	[7:0]PC_out;

always@(posedge clk)
	begin
		if(rst==1)PC_out<=8'b0;
		else if(load==1)PC_out<=PC_in;
		else if(inc==1)PC_out<=PC_out+8'b00000001;		
	end
	
endmodule

