module counter_4bit_2clock_tb;

	reg clk0, rst0;
	wire [3:0] q0;

	reg clk1, rst1;
	wire [3:0] q1;

	counter_4bit_2clock C_1(
		clk0, 
		q0, 
		rst0);

	counter_4bit_2clock C_1(
		clk1, 
		q1, 
		rst1);
	
	initial begin
		#0 rst0 = 1'b1; clk0 = 1'b0;
		#100 rst0 = 1'b0;
	end

	always begin
		#10 clk0 = ~clk0;
	end

	initial begin
		#0 rst1 = 1'b1; clk1 = 1'b0;
		#100 rst1 = 1'b0;
	end

	always begin
		#20 clk1 = ~clk1;
	end

	
	initial begin
		#5000 $stop;
	end

endmodule
