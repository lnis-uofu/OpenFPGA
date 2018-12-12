///////////////////////////////
//                           //
//    fifo_1bit benchmark    //
//                           //
///////////////////////////////

module fifo_1bit(
	rst,
	clk,
	data_in,
	data_out );

	input rst;
	input clk;
	input data_in;
	output data_out;

	reg[31:0] int_reg;

	assign data_out = int_reg[31];

	always@(posedge clk or posedge rst) begin
		if(rst) begin
			int_reg <= 32'h00;
		end
		else begin
			int_reg[0] <= data_in;
			int_reg[32:1] = int_reg[31:0];
		end
	end 

endmodule
