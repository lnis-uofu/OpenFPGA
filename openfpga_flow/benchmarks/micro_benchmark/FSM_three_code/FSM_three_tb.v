module FSM_three_tb;

	reg rst;
	reg clk;
	reg [5:0] sec_in, min_in, hour_in;
	reg load_in;
	wire [5:0] sec_out, min_out, hour_out;

	FSM_top FSM_1(
		.rst(rst),
		.clk(clk),
		.sec_in(sec_in),
		.load_in(load_in),
		.sec_out(sec_out),
		.min_in(min_in),
		.min_out(min_out),
		.hour_in(hour_in),
		.hour_out(hour_out));
		
	initial begin
		#0 rst = 1'd1; clk = 1'd0; load_in = 1'd1; sec_in = 6'd33; min_in = 6'd14; hour_in = 6'd5;
		#100 rst = 1'd0;
		#50 load_in = 1'd0;
	end
	
	always begin
		#10 clk = ~clk;
	end
	
	initial begin
		#100000 $stop;
	end

endmodule