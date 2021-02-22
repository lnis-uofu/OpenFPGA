/*-------------------------------------------------------------------
CM_FIFO_1x
	Communication Manager (CM) FIFO, using 1 RAM block.
	18-bit write port (512 deep), 9-bit read port (1024 deep).
	
	The LSB on the write port will be the first byte to appear on
	the read port.
	
	Valid data appears on the output data port without first
	having to do a pop.
	
	Over-run and under-run protection are both implemented:
	reads when empty will be ignored and provide invalid data,
	writes when full will be ignored.

-------------------------------------------------------------------*/


`timescale 1ns / 10ps

module CM_FIFO_1x (
	rst,

	push_clk,
	push,
	din,
	full,
	push_flag,
	overflow,

	pop_clk,
	pop,
	dout,
	empty,
	pop_flag,
	
	CM_FIFO_1x_din_o			,		
	CM_FIFO_1x_push_int_o		,
	CM_FIFO_1x_pop_int_o		,
	CM_FIFO_1x_push_clk_o		,
	CM_FIFO_1x_pop_clk_o		,
	CM_FIFO_1x_rst_o			,
	                           
	CM_FIFO_1x_almost_full_i	,
	CM_FIFO_1x_almost_empty_i	,
	CM_FIFO_1x_push_flag_i		,
	CM_FIFO_1x_pop_flag_i		,
	CM_FIFO_1x_dout_i			
	
	
);

input			rst;

input			push_clk;
input			push;
input	[17:0]	din;
output			full;
output	[3:0]	push_flag;
output			overflow;

input			pop_clk;
input			pop;
output	[8:0]	dout;
output			empty;
output	[3:0]	pop_flag;

output 	[17:0]	CM_FIFO_1x_din_o			;
output 			CM_FIFO_1x_push_int_o		;
output 			CM_FIFO_1x_pop_int_o		;
output 			CM_FIFO_1x_push_clk_o		;
output 			CM_FIFO_1x_pop_clk_o		;
output 			CM_FIFO_1x_rst_o			;

input 			CM_FIFO_1x_almost_full_i	;
input 			CM_FIFO_1x_almost_empty_i	;
input 	[3:0]	CM_FIFO_1x_push_flag_i		;
input 	[3:0]	CM_FIFO_1x_pop_flag_i		;
input 	[8:0]	CM_FIFO_1x_dout_i			;



reg				overflow;


wire			push_int;
wire			pop_int;
reg				pop_r1, pop_r2, pop_r3;


// over-run/under-run protection
assign push_int = full ? 1'b0 : push;

// changed to match the current S2 functionality
//assign pop_int = empty ? 1'b0 : pop;
assign pop_int = empty ? 1'b0 : (pop_r2 ^ pop_r3);


assign CM_FIFO_1x_din_o 		= din;
assign CM_FIFO_1x_push_int_o 	= push_int;
assign CM_FIFO_1x_pop_int_o 	= pop_int;
assign CM_FIFO_1x_push_clk_o 	= push_clk;
assign CM_FIFO_1x_pop_clk_o 	= pop_clk;
assign CM_FIFO_1x_rst_o 		= rst;
assign almost_full 				= CM_FIFO_1x_almost_full_i;
assign almost_empty 			= CM_FIFO_1x_almost_empty_i;
assign push_flag 				= CM_FIFO_1x_push_flag_i;
assign pop_flag 				= CM_FIFO_1x_pop_flag_i;
assign dout 					= CM_FIFO_1x_dout_i;




assign full = (push_flag == 4'h0);
assign empty = (pop_flag == 4'h0);


// overflow detection
always @(posedge push_clk or posedge rst)
	if (rst)
		overflow <= 0;
	else
		if (push && full)
			overflow <= 1;
		else
			overflow <= 0;

/// Synchronize SPI FIFO Read to SPI CLock due to delay
always @(posedge pop_clk or posedge rst)
	if (rst) begin
		pop_r1 <= 1'b0;
		pop_r2 <= 1'b0;
		pop_r3 <= 1'b0;
	end
	else begin
		pop_r1 <= pop;
		pop_r2 <= pop_r1;
		pop_r3 <= pop_r2;
	end


endmodule

