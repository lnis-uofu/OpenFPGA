module testSAPone;

	wire [7:0] SAP_out;
	wire [11:0] con;
	wire [7:0] bus;
//	wire clk_out, clr_out;
	reg clk, clr_;

	always #5 clk = ~clk;
	
	SAPone sapone1(
		.SAP_out(SAP_out),
		.con(con),
		.bus(bus),
//		.clk_out(clk_out),
//		.clr_out(clr_out),
		.clk(clk),
		.clr_(clr_)
		);
		
//	PC pc1(bus[3:0], clk, clr_, cp, ep);
//	MAR mar1(mar, clk, lm_, bus[3:0]);

	initial
	begin
		clk = 0; clr_ = 0;
		#10 clr_ = 1;
		
		
		
		#990 $stop;	
	end

endmodule
