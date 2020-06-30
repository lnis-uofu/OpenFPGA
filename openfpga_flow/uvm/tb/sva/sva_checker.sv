
`timescale 1ns/1ps

`define assert_clk_uvm( arg ) \
    assert property (@(posedge clk_pad) disable iff (!pReset_pad) arg );

`define assert_undefined( arg ) \
    assert property (@(posedge clk_pad) disable iff (!pReset_pad) $isnunknown( arg ) );

module inv_checker(
    input [5:0]	a,
    input [5:0]	b
//  input       pReset_pad,
//  input       prog_clk_pad,
//  input       Reset_pad,
//  input       Test_en_pad,
//  input       clk_pad,
//  input [7:0] gfpga_pad_GPIO,
//  input       ccff_head_pad,
//  input       ccff_tail_pad
//  input       chanx_in_1
//  input	      config_done,
//  input	      clk_uvm  
);
bit clk;
always begin
#5 clk = !clk;
end

    Sva_test1: assert property (@(posedge clk) a !== b);
//  Sva_test1: assert property (@(posedge clk_pad) gfpga_pad_GPIO[0] === gfpga_pad_GPIO[7]);

//`assert_undefined(pReset_pad);
//`assert_undefined(prog_clk_pad);
//`assert_undefined(Reset_pad);
//`assert_undefined(Test_en_pad);
//`assert_undefined(clk_pad);
//`assert_undefined(ccff_head_pad);
//`assert_undefined(ccff_tail_pad);

//prog_clk_start:
//	`assert_clk_uvm(gfpga_pad_GPIO[0] |-> $change(prog_clk_pad))

//prog_clk_done:
//	`assert_clk_uvm(config_done==1 |-> ##1 prog_clk_pad == 0)

//bistream_check: // either python script or 



endmodule
