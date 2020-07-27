module IR(IR_out,IR_in,load,clk,rst);

output reg	[7:0]IR_out;
input	[7:0]IR_in;
input	load,clk,rst;

always@(posedge clk)
	begin
		if(rst==1)IR_out<=8'b0;
		else if(load==1)IR_out<=IR_in;		
	end

endmodule
