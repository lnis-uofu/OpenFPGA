module FSM_top(
	input wire rst,
	input wire clk,
	input wire load_in,
	input wire [5:0] sec_in,
	input wire [5:0] min_in,
	input wire [5:0] hour_in,
	output wire [5:0] sec_out,
	output wire [5:0] min_out,
	output wire [5:0] hour_out
	);

	FSM_second FSM_sec(
		.rst(rst),
		.clk(clk),
		.sec_in(sec_in),
		.sec_in_load(load_in),
		.sec_out(sec_out));

	FSM_minute FSM_min(
		.rst(rst),
		.clk(clk),
		.min_in(min_in),
		.min_in_load(load_in),
		.sec_count(sec_out),
		.min_out(min_out));
		
	FSM_hour FSM_hr(
		.rst(rst),
		.clk(clk),
		.hour_in(hour_in),
		.hour_in_load(load_in),
		.min_count(min_out),
		.hour_out(hour_out),
		.sec_count(sec_out));

endmodule