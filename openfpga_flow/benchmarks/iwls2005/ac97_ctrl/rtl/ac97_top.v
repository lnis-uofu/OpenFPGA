/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller Top Level                        ////
////                                                             ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/ac97_ctrl/ ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2000-2002 Rudolf Usselmann                    ////
////                         www.asics.ws                        ////
////                         rudi@asics.ws                       ////
////                                                             ////
//// This source file may be used and distributed without        ////
//// restriction provided that this copyright statement is not   ////
//// removed from the file and that any derivative work contains ////
//// the original copyright notice and the associated disclaimer.////
////                                                             ////
////     THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY     ////
//// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   ////
//// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS   ////
//// FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHOR      ////
//// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,         ////
//// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    ////
//// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE   ////
//// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR        ////
//// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF  ////
//// LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT  ////
//// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  ////
//// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE         ////
//// POSSIBILITY OF SUCH DAMAGE.                                 ////
////                                                             ////
/////////////////////////////////////////////////////////////////////

//  CVS Log
//
//  $Id: ac97_top.v,v 1.4 2002/09/19 06:30:56 rudi Exp $
//
//  $Date: 2002/09/19 06:30:56 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_top.v,v $
//               Revision 1.4  2002/09/19 06:30:56  rudi
//               Fixed a bug reported by Igor. Apparently this bug only shows up when
//               the WB clock is very low (2x bit_clk). Updated Copyright header.
//
//               Revision 1.3  2002/03/05 04:44:05  rudi
//
//               - Fixed the order of the thrash hold bits to match the spec.
//               - Many minor synthesis cleanup items ...
//
//               Revision 1.2  2001/08/10 08:09:42  rudi
//
//               - Removed RTY_O output.
//               - Added Clock and Reset Inputs to documentation.
//               - Changed IO names to be more clear.
//               - Uniquifyed define names to be core specific.
//
//               Revision 1.1  2001/08/03 06:54:50  rudi
//
//
//               - Changed to new directory structure
//
//               Revision 1.1.1.1  2001/05/19 02:29:14  rudi
//               Initial Checkin
//
//
//
//

`include "ac97_defines.v"

module ac97_top(clk_i, rst_i,

	wb_data_i, wb_data_o, wb_addr_i, wb_sel_i, wb_we_i, wb_cyc_i,
	wb_stb_i, wb_ack_o, wb_err_o, 

	int_o, dma_req_o, dma_ack_i,
	suspended_o,

	bit_clk_pad_i, sync_pad_o, sdata_pad_o, sdata_pad_i,
	ac97_reset_pad_o_
	);

input		clk_i, rst_i;

// --------------------------------------
// WISHBONE SLAVE INTERFACE 
input	[31:0]	wb_data_i;
output	[31:0]	wb_data_o;
input	[31:0]	wb_addr_i;
input	[3:0]	wb_sel_i;
input		wb_we_i;
input		wb_cyc_i;
input		wb_stb_i;
output		wb_ack_o;
output		wb_err_o;

// --------------------------------------
// Misc Signals
output		int_o;
output	[8:0]	dma_req_o;
input	[8:0]	dma_ack_i;

// --------------------------------------
// Suspend Resume Interface
output		suspended_o;

// --------------------------------------
// AC97 Codec Interface
input		bit_clk_pad_i;
output		sync_pad_o;
output		sdata_pad_o;
input		sdata_pad_i;
output		ac97_reset_pad_o_;

////////////////////////////////////////////////////////////////////
//
// Local Wires
//

// Serial Output register interface
wire	[15:0]	out_slt0;
wire	[19:0]	out_slt1;
wire	[19:0]	out_slt2;
wire	[19:0]	out_slt3;
wire	[19:0]	out_slt4;
wire	[19:0]	out_slt6;
wire	[19:0]	out_slt7;
wire	[19:0]	out_slt8;
wire	[19:0]	out_slt9;

// Serial Input register interface
wire	[15:0]	in_slt0;
wire	[19:0]	in_slt1;
wire	[19:0]	in_slt2;
wire	[19:0]	in_slt3;
wire	[19:0]	in_slt4;
wire	[19:0]	in_slt6;

// Serial IO Controller Interface
wire		ld;
wire		valid;
wire	[5:0]	out_le;
wire	[2:0]	in_valid;
wire		ps_ce;

// Valid Sync
reg		valid_s1, valid_s;
reg	[2:0]	in_valid_s1, in_valid_s;

// Out FIFO interface
wire	[31:0]	wb_din;
wire	[1:0]	o3_mode, o4_mode, o6_mode, o7_mode, o8_mode, o9_mode;
wire		o3_re, o4_re, o6_re, o7_re, o8_re, o9_re;
wire		o3_we, o4_we, o6_we, o7_we, o8_we, o9_we;
wire	[1:0]	o3_status, o4_status, o6_status, o7_status, o8_status, o9_status;
wire		o3_full, o4_full, o6_full, o7_full, o8_full, o9_full;
wire		o3_empty, o4_empty, o6_empty, o7_empty, o8_empty, o9_empty;

// In FIFO interface
wire	[31:0]	i3_dout, i4_dout, i6_dout;
wire	[1:0]	i3_mode, i4_mode, i6_mode;
wire		i3_we, i4_we, i6_we;
wire		i3_re, i4_re, i6_re;
wire	[1:0]	i3_status, i4_status, i6_status;
wire		i3_full, i4_full, i6_full;
wire		i3_empty, i4_empty, i6_empty;

// Register File Interface
wire	[3:0]	adr;
wire	[31:0]	rf_dout;
wire	[31:0]	rf_din;
wire		rf_we;
wire		rf_re;
wire		ac97_rst_force;
wire		resume_req;
wire		crac_we;
wire	[15:0]	crac_din;
wire	[31:0]	crac_out;
wire	[7:0]	oc0_cfg;
wire	[7:0]	oc1_cfg;
wire	[7:0]	oc2_cfg;
wire	[7:0]	oc3_cfg;
wire	[7:0]	oc4_cfg;
wire	[7:0]	oc5_cfg;
wire	[7:0]	ic0_cfg;
wire	[7:0]	ic1_cfg;
wire	[7:0]	ic2_cfg;
wire	[2:0]	oc0_int_set;
wire	[2:0]	oc1_int_set;
wire	[2:0]	oc2_int_set;
wire	[2:0]	oc3_int_set;
wire	[2:0]	oc4_int_set;
wire	[2:0]	oc5_int_set;
wire	[2:0]	ic0_int_set;
wire	[2:0]	ic1_int_set;
wire	[2:0]	ic2_int_set;

// CRA Module interface
wire		crac_valid;
wire		crac_wr;
wire		crac_wr_done, crac_rd_done;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

// Sync Valid to WISHBONE Clock
always @(posedge clk_i)
	valid_s1 <= #1 valid;

always @(posedge clk_i)
	valid_s <= #1 valid_s1;

always @(posedge clk_i)
	in_valid_s1 <= #1 in_valid;

always @(posedge clk_i)
	in_valid_s <= #1 in_valid_s1;

// "valid_s" Indicates when any of the outputs to the output S/R may
// change or when outputs from input S/R may be sampled
assign o3_mode = oc0_cfg[3:2];
assign o4_mode = oc1_cfg[3:2];
assign o6_mode = oc2_cfg[3:2];
assign o7_mode = oc3_cfg[3:2];
assign o8_mode = oc4_cfg[3:2];
assign o9_mode = oc5_cfg[3:2];
assign i3_mode = ic0_cfg[3:2];
assign i4_mode = ic1_cfg[3:2];
assign i6_mode = ic2_cfg[3:2];

////////////////////////////////////////////////////////////////////
//
// Modules
//

ac97_sout	u0(
		.clk(		bit_clk_pad_i	),
		.rst(		rst_i		),
		.so_ld(		ld		),
		.slt0(		out_slt0	),
		.slt1(		out_slt1	),
		.slt2(		out_slt2	),
		.slt3(		out_slt3	),
		.slt4(		out_slt4	),
		.slt6(		out_slt6	),
		.slt7(		out_slt7	),
		.slt8(		out_slt8	),
		.slt9(		out_slt9	),
		.sdata_out(	sdata_pad_o	)
		);

ac97_sin	u1(
		.clk(		bit_clk_pad_i	),
		.rst(		rst_i		),
		.out_le(	out_le		),
		.slt0(		in_slt0		),
		.slt1(		in_slt1		),
		.slt2(		in_slt2		),
		.slt3(		in_slt3		),
		.slt4(		in_slt4		),
		.slt6(		in_slt6		),
		.sdata_in(	sdata_pad_i	)
		);

ac97_soc	u2(
		.clk(		bit_clk_pad_i	),
		.wclk(		clk_i		),
		.rst(		rst_i		),
		.ps_ce(		ps_ce		),
		.resume(	resume_req	),
		.suspended(	suspended_o	),
		.sync(		sync_pad_o	),
		.out_le(	out_le		),
		.in_valid(	in_valid	),
		.ld(		ld		),
		.valid(		valid		)
		);

ac97_out_fifo	u3(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.en(		oc0_cfg[0]	),
		.mode(		o3_mode		),
		.din(		wb_din		),
		.we(		o3_we		),
		.dout(		out_slt3	),
		.re(		o3_re		),
		.status(	o3_status	),
		.full(		o3_full		),
		.empty(		o3_empty	)
		);

ac97_out_fifo	u4(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.en(		oc1_cfg[0]	),
		.mode(		o4_mode		),
		.din(		wb_din		),
		.we(		o4_we		),
		.dout(		out_slt4	),
		.re(		o4_re		),
		.status(	o4_status	),
		.full(		o4_full		),
		.empty(		o4_empty	)
		);

`ifdef AC97_CENTER
ac97_out_fifo	u5(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.en(		oc2_cfg[0]	),
		.mode(		o6_mode		),
		.din(		wb_din		),
		.we(		o6_we		),
		.dout(		out_slt6	),
		.re(		o6_re		),
		.status(	o6_status	),
		.full(		o6_full		),
		.empty(		o6_empty	)
		);
`else
assign out_slt6 = 20'h0;
assign o6_status = 2'h0;
assign o6_full = 1'b0;
assign o6_empty = 1'b0;
`endif

`ifdef AC97_SURROUND
ac97_out_fifo	u6(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.en(		oc3_cfg[0]	),
		.mode(		o7_mode		),
		.din(		wb_din		),
		.we(		o7_we		),
		.dout(		out_slt7	),
		.re(		o7_re		),
		.status(	o7_status	),
		.full(		o7_full		),
		.empty(		o7_empty	)
		);

ac97_out_fifo	u7(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.en(		oc4_cfg[0]	),
		.mode(		o8_mode		),
		.din(		wb_din		),
		.we(		o8_we		),
		.dout(		out_slt8	),
		.re(		o8_re		),
		.status(	o8_status	),
		.full(		o8_full		),
		.empty(		o8_empty	)
		);
`else
assign out_slt7 = 20'h0;
assign o7_status = 2'h0;
assign o7_full = 1'b0;
assign o7_empty = 1'b0;
assign out_slt8 = 20'h0;
assign o8_status = 2'h0;
assign o8_full = 1'b0;
assign o8_empty = 1'b0;
`endif

`ifdef AC97_LFE
ac97_out_fifo	u8(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.en(		oc5_cfg[0]	),
		.mode(		o9_mode		),
		.din(		wb_din		),
		.we(		o9_we		),
		.dout(		out_slt9	),
		.re(		o9_re		),
		.status(	o9_status	),
		.full(		o9_full		),
		.empty(		o9_empty	)
		);
`else
assign out_slt9 = 20'h0;
assign o9_status = 2'h0;
assign o9_full = 1'b0;
assign o9_empty = 1'b0;
`endif

`ifdef AC97_SIN
ac97_in_fifo	u9(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.en(		ic0_cfg[0]	),
		.mode(		i3_mode		),
		.din(		in_slt3		),
		.we(		i3_we		),
		.dout(		i3_dout		),
		.re(		i3_re		),
		.status(	i3_status	),
		.full(		i3_full		),
		.empty(		i3_empty	)
		);

ac97_in_fifo	u10(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.en(		ic1_cfg[0]	),
		.mode(		i4_mode		),
		.din(		in_slt4		),
		.we(		i4_we		),
		.dout(		i4_dout		),
		.re(		i4_re		),
		.status(	i4_status	),
		.full(		i4_full		),
		.empty(		i4_empty	)
		);
`else
assign i3_dout = 20'h0;
assign i3_status = 2'h0;
assign i3_full = 1'b0;
assign i3_empty = 1'b0;
assign i4_dout = 20'h0;
assign i4_status = 2'h0;
assign i4_full = 1'b0;
assign i4_empty = 1'b0;
`endif

`ifdef AC97_MICIN
ac97_in_fifo	u11(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.en(		ic2_cfg[0]	),
		.mode(		i6_mode		),
		.din(		in_slt6		),
		.we(		i6_we		),
		.dout(		i6_dout		),
		.re(		i6_re		),
		.status(	i6_status	),
		.full(		i6_full		),
		.empty(		i6_empty	)
		);
`else
assign i6_dout = 20'h0;
assign i6_status = 2'h0;
assign i6_full = 1'b0;
assign i6_empty = 1'b0;
`endif

ac97_wb_if	u12(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.wb_data_i(	wb_data_i	),
		.wb_data_o(	wb_data_o	),
		.wb_addr_i(	wb_addr_i	),
		.wb_sel_i(	wb_sel_i	),
		.wb_we_i(	wb_we_i		),
		.wb_cyc_i(	wb_cyc_i	),
		.wb_stb_i(	wb_stb_i	),
		.wb_ack_o(	wb_ack_o	),
		.wb_err_o(	wb_err_o	),
		.adr(		adr		),
		.dout(		wb_din		),
		.rf_din(	rf_dout		),
		.i3_din(	i3_dout		),
		.i4_din(	i4_dout		),
		.i6_din(	i6_dout		),
		.rf_we(		rf_we		),
		.rf_re(		rf_re		),
		.o3_we(		o3_we		),
		.o4_we(		o4_we		),
		.o6_we(		o6_we		),
		.o7_we(		o7_we		),
		.o8_we(		o8_we		),
		.o9_we(		o9_we		),
		.i3_re(		i3_re		),
		.i4_re(		i4_re		),
		.i6_re(		i6_re		)
		);

ac97_rf	u13(	.clk(		clk_i		),
		.rst(		rst_i		),
		.adr(		adr		),
		.rf_dout(	rf_dout		),
		.rf_din(	wb_din		),
		.rf_we(		rf_we		),
		.rf_re(		rf_re		),
		.int(		int_o		),
		.ac97_rst_force(ac97_rst_force	),
		.resume_req(	resume_req	),
		.suspended(	suspended_o	),
		.crac_we(	crac_we		),
		.crac_din(	crac_din	),
		.crac_out(	crac_out	),
		.crac_wr_done(	crac_wr_done	),
		.crac_rd_done(	crac_rd_done	),
		.oc0_cfg(	oc0_cfg		),
		.oc1_cfg(	oc1_cfg		),
		.oc2_cfg(	oc2_cfg		),
		.oc3_cfg(	oc3_cfg		),
		.oc4_cfg(	oc4_cfg		),
		.oc5_cfg(	oc5_cfg		),
		.ic0_cfg(	ic0_cfg		),
		.ic1_cfg(	ic1_cfg		),
		.ic2_cfg(	ic2_cfg		),
		.oc0_int_set(	oc0_int_set	),
		.oc1_int_set(	oc1_int_set	),
		.oc2_int_set(	oc2_int_set	),
		.oc3_int_set(	oc3_int_set	),
		.oc4_int_set(	oc4_int_set	),
		.oc5_int_set(	oc5_int_set	),
		.ic0_int_set(	ic0_int_set	),
		.ic1_int_set(	ic1_int_set	),
		.ic2_int_set(	ic2_int_set	)
		);

ac97_prc u14(	.clk(		clk_i		),
		.rst(		rst_i		),
		.valid(		valid_s		),
		.in_valid(	in_valid_s	),
		.out_slt0(	out_slt0	),
		.in_slt0(	in_slt0		),
		.in_slt1(	in_slt1		),
		.crac_valid(	crac_valid	),
		.crac_wr(	crac_wr		),
		.oc0_cfg(	oc0_cfg		),
		.oc1_cfg(	oc1_cfg		),
		.oc2_cfg(	oc2_cfg		),
		.oc3_cfg(	oc3_cfg		),
		.oc4_cfg(	oc4_cfg		),
		.oc5_cfg(	oc5_cfg		),
		.ic0_cfg(	ic0_cfg		),
		.ic1_cfg(	ic1_cfg		),
		.ic2_cfg(	ic2_cfg		),
		.o3_empty(	o3_empty	),
		.o4_empty(	o4_empty	),
		.o6_empty(	o6_empty	),
		.o7_empty(	o7_empty	),
		.o8_empty(	o8_empty	),
		.o9_empty(	o9_empty	),
		.i3_full(	i3_full		),
		.i4_full(	i4_full		),
		.i6_full(	i6_full		),
		.o3_re(		o3_re		),
		.o4_re(		o4_re		),
		.o6_re(		o6_re		),
		.o7_re(		o7_re		),
		.o8_re(		o8_re		),
		.o9_re(		o9_re		),
		.i3_we(		i3_we		),
		.i4_we(		i4_we		),
		.i6_we(		i6_we		)
		);

ac97_cra u15(	.clk(		clk_i		),
		.rst(		rst_i		),
		.crac_we(	crac_we		),
		.crac_din(	crac_din	),
		.crac_out(	crac_out	),
		.crac_wr_done(	crac_wr_done	),
		.crac_rd_done(	crac_rd_done	),
		.valid(		valid_s		),
		.out_slt1(	out_slt1	),
		.out_slt2(	out_slt2	),
		.in_slt2(	in_slt2		),
		.crac_valid(	crac_valid	),
		.crac_wr(	crac_wr		)
		);

ac97_dma_if u16(.clk(		clk_i		),
		.rst(		rst_i		),
		.o3_status(	o3_status	),
		.o4_status(	o4_status	),
		.o6_status(	o6_status	),
		.o7_status(	o7_status	),
		.o8_status(	o8_status	),
		.o9_status(	o9_status	),
		.o3_empty(	o3_empty	),
		.o4_empty(	o4_empty	),
		.o6_empty(	o6_empty	),
		.o7_empty(	o7_empty	),
		.o8_empty(	o8_empty	),
		.o9_empty(	o9_empty	),
		.i3_status(	i3_status	),
		.i4_status(	i4_status	),
		.i6_status(	i6_status	),
		.i3_full(	i3_full		),
		.i4_full(	i4_full		),
		.i6_full(	i6_full		),
		.oc0_cfg(	oc0_cfg		),
		.oc1_cfg(	oc1_cfg		),
		.oc2_cfg(	oc2_cfg		),
		.oc3_cfg(	oc3_cfg		),
		.oc4_cfg(	oc4_cfg		),
		.oc5_cfg(	oc5_cfg		),
		.ic0_cfg(	ic0_cfg		),
		.ic1_cfg(	ic1_cfg		),
		.ic2_cfg(	ic2_cfg		),
		.dma_req(	dma_req_o	),
		.dma_ack(	dma_ack_i	)
		);

ac97_int	u17(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.int_set(	oc0_int_set	),
		.cfg(		oc0_cfg		),
		.status(	o3_status	),
		.full_empty(	o3_empty	),
		.full(		o3_full		),
		.empty(		o3_empty	),
		.re(		o3_re		),
		.we(		o3_we		)
		);

ac97_int	u18(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.int_set(	oc1_int_set	),
		.cfg(		oc1_cfg		),
		.status(	o4_status	),
		.full_empty(	o4_empty	),
		.full(		o4_full		),
		.empty(		o4_empty	),
		.re(		o4_re		),
		.we(		o4_we		)
		);

`ifdef AC97_CENTER
ac97_int	u19(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.int_set(	oc2_int_set	),
		.cfg(		oc2_cfg		),
		.status(	o6_status	),
		.full_empty(	o6_empty	),
		.full(		o6_full		),
		.empty(		o6_empty	),
		.re(		o6_re		),
		.we(		o6_we		)
		);
`else
assign oc2_int_set = 1'b0;
`endif

`ifdef AC97_SURROUND
ac97_int	u20(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.int_set(	oc3_int_set	),
		.cfg(		oc3_cfg		),
		.status(	o7_status	),
		.full_empty(	o7_empty	),
		.full(		o7_full		),
		.empty(		o7_empty	),
		.re(		o7_re		),
		.we(		o7_we		)
		);

ac97_int	u21(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.int_set(	oc4_int_set	),
		.cfg(		oc4_cfg		),
		.status(	o8_status	),
		.full_empty(	o8_empty	),
		.full(		o8_full		),
		.empty(		o8_empty	),
		.re(		o8_re		),
		.we(		o8_we		)
		);
`else
assign oc3_int_set = 1'b0;
assign oc4_int_set = 1'b0;
`endif

`ifdef AC97_LFE
ac97_int	u22(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.int_set(	oc5_int_set	),
		.cfg(		oc5_cfg		),
		.status(	o9_status	),
		.full_empty(	o9_empty	),
		.full(		o9_full		),
		.empty(		o9_empty	),
		.re(		o9_re		),
		.we(		o9_we		)
		);
`else
assign oc5_int_set = 1'b0;
`endif

`ifdef AC97_SIN
ac97_int	u23(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.int_set(	ic0_int_set	),
		.cfg(		ic0_cfg		),
		.status(	i3_status	),
		.full_empty(	i3_full		),
		.full(		i3_full		),
		.empty(		i3_empty	),
		.re(		i3_re		),
		.we(		i3_we		)
		);

ac97_int	u24(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.int_set(	ic1_int_set	),
		.cfg(		ic1_cfg		),
		.status(	i4_status	),
		.full_empty(	i4_full		),
		.full(		i4_full		),
		.empty(		i4_empty	),
		.re(		i4_re		),
		.we(		i4_we		)
		);
`else
assign ic0_int_set = 1'b0;
assign ic1_int_set = 1'b0;
`endif

`ifdef AC97_MICIN
ac97_int	u25(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.int_set(	ic2_int_set	),
		.cfg(		ic2_cfg		),
		.status(	i6_status	),
		.full_empty(	i6_full		),
		.full(		i6_full		),
		.empty(		i6_empty	),
		.re(		i6_re		),
		.we(		i6_we		)
		);
`else
assign ic2_int_set = 1'b0;
`endif

ac97_rst	u26(
		.clk(		clk_i				),
		.rst(		rst_i 				),
		.rst_force(	ac97_rst_force			),
		.ps_ce(		ps_ce				),
		.ac97_rst_(	ac97_reset_pad_o_		)
		);

endmodule

