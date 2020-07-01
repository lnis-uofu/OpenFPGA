`timescale 1ns/1ps

`define assert_clk_sva( arg ) \
    assert property (@(posedge clk_sva) disable iff (R || (enable_assertions == 1'b0)) arg ) else $warning("Simulation failed");
`define assert_ck( arg ) \
    assert property (@(posedge CK) disable iff (R || (enable_assertions == 1'b0)) arg ) else $warning("Simulation failed");
`define assert_undefined( arg ) \
    assert property (@(posedge CK) disable iff (R || (enable_assertions == 1'b0)) !$isunknown(arg)) else $warning("Undefined signal");

module FF_checker#(
parameter enable_assertions = 1,
	  enable_one_fire_mode = 1
)(
//Top level inputs
    input [0:0] D,
    input [0:0] CK,
    input [0:0] R,
    input [0:0] Q
);

reg [0:0] clk_gen = 1'b0;
reg disable_clock = 1'b1;
wire clk_sva;

initial begin wait(D)
	begin
		disable_clock = 1'b0;
		#2001
		disable_clock = 1'b1;
	end
	end

always
	begin
		#500.00000381	clk_gen[0] = ~clk_gen[0];
	end
assign clk_sva = clk_gen & (~disable_clock || enable_one_fire_mode == 0);


behavior_check: assert property (@(posedge CK) 	    disable iff (R) D |-> ##1 Q ) else $warning("Behavior with instance clock is wrong");
shortime_check: assert property (@(posedge clk_sva) disable iff (R) D |-> ##1 Q ) else $warning("Short timed flipflop");

D_undefined: `assert_undefined( D )
CK_undefined: `assert_clk_sva( !$isunknown(CK) )
Q_undefined: `assert_undefined( Q )

Reset_check: `assert_clk_sva( R |-> !D)

endmodule
