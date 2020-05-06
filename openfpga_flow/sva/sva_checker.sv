
`timescale 1ns/1ps

`define assert_prog_clk( arg ) \
    assert property (@(posedge prog_clk) disable iff (pReset || (enable_assertions == 1'b0)) arg ) else $fatal("Simulation failed");

`define assert_clk( arg ) \
    assert property (@(posedge clk) disable iff (Reset || ~Test_en || (enable_assertions == 1'b0)) arg ) else $fatal("Simulation failed");

`define assert_undefined( arg ) \
    assert property (@(posedge clk) disable iff (pReset || Reset || (enable_assertions == 1'b0)) !$isunknown(arg)) else $fatal("Simulation failed");

module inv_checker#(
parameter enable_assertions = 1,
	  BS_LGT = 8387,
	  FF_n = 80
)(
//Top level inputs
    input [0:0] prog_clk,
    input [0:0] clk,
    input [0:0] pReset,
    input [0:0] Reset,
    input [0:0] Test_en,
    input [0:0] cc_spypad_0, // ok
    input [0:0] cc_spypad_1, // ok
    input [0:0] cc_spypad_2, // ok
    input [0:0] cout_spypad_0,
    input [0:0] lut4_out_0,
    input [0:0] lut4_out_1,
    input [0:0] lut4_out_2,
    input [0:0] lut4_out_3,
    input [0:0] lut5_out_0,
    input [0:0] lut5_out_1,
    input [0:0] lut6_out_0,
    input [0:0] perf_spypad_0,
    input [0:0] sc_spypad_0,
    input [0:0] shiftreg_spypad_0,

// Inputs for CC PATH Check
    input [0:0] ccff_head,
    input [0:0] ccff_tail_gbot_1_0,
    input [0:0] ccff_head_gbot_2_0,
    input [0:0] ccff_head_gright_3_1,
    input [0:0] ccff_head_gright_3_2,
    input [0:0] ccff_tail_gright_3_2,
    input [0:0] ccff_head_sb_2_2,
    input [0:0] ccff_tail_sb_2_2,
    input [0:0] ccff_head_cbx_2_2,
    input [0:0] ccff_head_g11,
    input [0:0] ccff_head_g21,
    input [0:0] ccff_head_g22,
    input [0:0] ccff_head_g12,
    input [0:0]	ccff_tail,
// Inputs for SC PATH Check
    input [0:0] sc_head,
    input [0:0] sc_tail,
    input [0:0] sc_tail_clb_1_2,
    input [0:0] sc_tail_clb_1_1,
    input [0:0] sc_tail_clb_2_2,
    input [0:0] sc_tail_clb_2_1,
// Ref signals for spypads
    input [0:0] cc_spypad_1_ref,
    input [0:0] cc_spypad_2_ref,
    input [0:0] cout_spypad_0_ref,
    input [0:0] lut4_out_0_ref,
    input [0:0] lut4_out_1_ref,
    input [0:0] lut4_out_2_ref,
    input [0:0] lut4_out_3_ref,
    input [0:0] lut5_out_0_ref,
    input [0:0] lut5_out_1_ref,
    input [0:0] lut6_out_0_ref,
    input [0:0] perf_spypad_0_ref,
    input [0:0] sc_spypad_0_ref,
    input [0:0] shiftreg_spypad_0_ref

);


bit reset_lock =0;
bit preset_lock=0;

reg [0:0] clk_sva = 1'b0;
always
	begin
		#2.5	clk_sva[0] = ~clk_sva[0];
	end
always @(posedge(Reset))
	begin
		reset_lock = 1'b1;
	end
always @(posedge(pReset))
	begin
		preset_lock = 1'b1;
	end
//No Signal should be undefined after Reset apart from gpio_pad

  U_prog_clk: 			`assert_undefined(prog_clk)
  U_clk:			`assert_undefined(clk)
  U_pReset:			`assert_undefined(pReset)
  U_Test_en:			`assert_undefined(Test_en)
  U_ccff_head:			`assert_undefined(ccff_head)
//  U_sc_head:			`assert_undefined(sc_head) // This one depends on the test actually
  U_cc_spypad_0:		`assert_undefined(cc_spypad_0)
  U_cc_spypad_1:		`assert_undefined(cc_spypad_1)
  U_cc_spypad_2:		`assert_undefined(cc_spypad_2)
  U_ccff_tail:			`assert_undefined(ccff_tail)
  U_cout_spypad_0:		`assert_undefined(cout_spypad_0)
  U_lut4_out_0:			`assert_undefined(lut4_out_0)
  U_lut4_out_1:			`assert_undefined(lut4_out_1)
  U_lut4_out_2:			`assert_undefined(lut4_out_2)
  U_lut4_out_3:			`assert_undefined(lut4_out_3)
  U_lut5_out_0:			`assert_undefined(lut5_out_0)
  U_lut5_out_1:			`assert_undefined(lut5_out_1)
  U_lut6_out_0:			`assert_undefined(lut6_out_0)
  U_perf_spypad_0:		`assert_undefined(perf_spypad_0)
  U_sc_spypad_0:		`assert_undefined(sc_spypad_0)
  U_sc_tail:			`assert_undefined(sc_tail)
  U_shiftreg_spypad_0:		`assert_undefined(shiftreg_spypad_0)

// Configuration chain path checker
    CC_TEST_HIGH: 	`assert_prog_clk( ccff_head |-> ##BS_LGT ccff_tail )
    CC_TEST_LOW:	`assert_prog_clk( !ccff_head |-> ##BS_LGT !ccff_tail )
    Path1:		`assert_prog_clk( ccff_tail_gbot_1_0 == ccff_head_gbot_2_0 )
    Path2:		`assert_prog_clk( ccff_head_gright_3_1 |-> ##1 ccff_head_gright_3_2 )
    Path3:		`assert_prog_clk( ccff_tail_gright_3_2 |-> ccff_head_sb_2_2 )
    Path4:		`assert_prog_clk( ccff_head_sb_2_2     |-> ##(220) ccff_tail_sb_2_2 ) 
    Path5:		`assert_prog_clk( ccff_tail_sb_2_2     == ccff_head_cbx_2_2 )
    Core_to_grid:	`assert_prog_clk( ccff_head |-> ##1933 ccff_head_g11 )
    Grid1_1to2_1:	`assert_prog_clk( ccff_head_g11 |-> ##1767 ccff_head_g21 )  
    Grid2_1to2_2:	`assert_prog_clk( ccff_head_g21 |-> ##1799 ccff_head_g22 )  
    Grid2_2to1_2:	`assert_prog_clk( ccff_head_g22 |-> ##1773 ccff_head_g12 )  

// Scan chain path checker
    SC_TEST: 		`assert_clk( sc_head  |-> ##FF_n sc_tail)
    First_sc: 		`assert_clk( sc_head == sc_tail_clb_1_2)
    Second_sc: 		`assert_clk( sc_head  |-> ##(FF_n/4) sc_tail_clb_1_1)
    Third_sc: 		`assert_clk( sc_head  |-> ##(FF_n/2) sc_tail_clb_2_2)
    Fourth_sc: 		`assert_clk( sc_head  |-> ##(3*FF_n/4) sc_tail_clb_2_1)

// Spypad assertions 
   ccff_head_top:	`assert_prog_clk(ccff_head |-> ##1 cc_spypad_0)
   cc_spypad_1_check:	`assert_prog_clk(cc_spypad_1_ref == cc_spypad_1)
   cc_spypad_2_check:	`assert_prog_clk(cc_spypad_2_ref == cc_spypad_2)
   cout_spypad_0_check:	`assert_clk(cout_spypad_0_ref == cout_spypad_0)
   lut4_out_0_check:	`assert_clk(lut4_out_0_ref == lut4_out_0)
   lut4_out_1_check:	`assert_clk(lut4_out_1_ref == lut4_out_1)
   lut4_out_2_check:	`assert_clk(lut4_out_2_ref == lut4_out_2)
   lut4_out_3_check:	`assert_clk(lut4_out_3_ref == lut4_out_3)
   lut5_out_0_check:	`assert_clk(lut5_out_0_ref == lut5_out_0)
   lut5_out_1_check:	`assert_clk(lut5_out_1_ref == lut5_out_1)
   lut6_out_0_check:	`assert_clk(lut6_out_0_ref == lut6_out_0)
   perf_spypad_0_check:	`assert_clk(perf_spypad_0_ref == perf_spypad_0)
   sc_spypad_0_check:	`assert_clk(sc_spypad_0_ref == sc_spypad_0)
   shiftreg_spypad_0_check:`assert_prog_clk(shiftreg_spypad_0_ref == shiftreg_spypad_0)

// Reset assertions
   reset_at_start:	assert property 
				(@(posedge clk_sva) disable iff (enable_assertions == 1'b0) ~reset_lock & ~Reset |-> ~clk)
			 else $fatal("Reset has to trigger before stimuli goes in");
   preset_at_start:	assert property 
				(@(posedge clk_sva) disable iff (enable_assertions == 1'b0) ~preset_lock & ~pReset |-> ~prog_clk) 
			else $fatal("pReset has to trigger before the beginning of programming phase");



endmodule
