module counter_tb;

	reg clk_counter, rst_counter;
	wire [7:0] q_counter;

	counter_original C_1(
		clk_counter, 
		q_counter, 
		rst_counter);
	
	initial begin
		#0 rst_counter = 1'b1; clk_counter = 1'b0;
		#100 rst_counter = 1'b0;
	end

	always begin
		#10 clk_counter = ~clk_counter;
	end
	
	initial begin
		#5000 $stop;
	end

endmodule