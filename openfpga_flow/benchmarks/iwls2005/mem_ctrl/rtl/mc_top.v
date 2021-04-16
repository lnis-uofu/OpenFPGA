/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE Memory Controller Top Level                       ////
////                                                             ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/mem_ctrl/  ////
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
//  $Id: mc_top.v,v 1.7 2002/01/21 13:08:52 rudi Exp $
//
//  $Date: 2002/01/21 13:08:52 $
//  $Revision: 1.7 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: mc_top.v,v $
//               Revision 1.7  2002/01/21 13:08:52  rudi
//
//               Fixed several minor bugs, cleaned up the code further ...
//
//               Revision 1.6  2001/12/21 05:09:30  rudi
//
//               - Fixed combinatorial loops in synthesis
//               - Fixed byte select bug
//
//               Revision 1.5  2001/11/29 02:16:28  rudi
//
//
//               - More Synthesis cleanup, mostly for speed
//               - Several bug fixes
//               - Changed code to avoid auto-precharge and
//                 burst-terminate combinations (apparently illegal ?)
//                 Now we will do a manual precharge ...
//
//               Revision 1.4  2001/09/10 13:44:17  rudi
//               *** empty log message ***
//
//               Revision 1.3  2001/09/02 02:28:28  rudi
//
//               Many fixes for minor bugs that showed up in gate level simulations.
//
//               Revision 1.2  2001/08/10 08:16:21  rudi
//
//               - Changed IO names to be more clear.
//               - Uniquifyed define names to be core specific.
//               - Removed "Refresh Early" configuration
//
//               Revision 1.1  2001/07/29 07:34:41  rudi
//
//
//               1) Changed Directory Structure
//               2) Fixed several minor bugs
//
//               Revision 1.3  2001/06/12 15:19:49  rudi
//
//
//               Minor changes after running lint, and a small bug fix reading csr and ba_mask registers.
//
//               Revision 1.2  2001/06/03 11:37:17  rudi
//
//
//               1) Fixed Chip Select Mask Register
//               	- Power On Value is now all ones
//               	- Comparison Logic is now correct
//
//               2) All resets are now asynchronous
//
//               3) Converted Power On Delay to an configurable item
//
//               4) Added reset to Chip Select Output Registers
//
//               5) Forcing all outputs to Hi-Z state during reset
//
//               Revision 1.1.1.1  2001/05/13 09:39:39  rudi
//               Created Directory Structure
//
//
//
//

`include "mc_defines.v"

module mc_top(clk_i, rst_i,

	wb_data_i, wb_data_o, wb_addr_i, wb_sel_i, wb_we_i, wb_cyc_i,
	wb_stb_i, wb_ack_o, wb_err_o, 

	susp_req_i, resume_req_i, suspended_o, poc_o,

	mc_clk_i, mc_br_pad_i, mc_bg_pad_o, mc_ack_pad_i,
	mc_addr_pad_o, mc_data_pad_i, mc_data_pad_o, mc_dp_pad_i,
	mc_dp_pad_o, mc_doe_pad_doe_o, mc_dqm_pad_o, mc_oe_pad_o_,
	mc_we_pad_o_, mc_cas_pad_o_, mc_ras_pad_o_, mc_cke_pad_o_,
	mc_cs_pad_o_, mc_sts_pad_i, mc_rp_pad_o_, mc_vpen_pad_o,
	mc_adsc_pad_o_, mc_adv_pad_o_, mc_zz_pad_o, mc_coe_pad_coe_o
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
// Suspend Resume Interface
input		susp_req_i;
input		resume_req_i;
output		suspended_o;

// POC
output	[31:0]	poc_o;

// --------------------------------------
// Memory Bus Signals
input		mc_clk_i;
input		mc_br_pad_i;
output		mc_bg_pad_o;
input		mc_ack_pad_i;
output	[23:0]	mc_addr_pad_o;
input	[31:0]	mc_data_pad_i;
output	[31:0]	mc_data_pad_o;
input	[3:0]	mc_dp_pad_i;
output	[3:0]	mc_dp_pad_o;
output		mc_doe_pad_doe_o;
output	[3:0]	mc_dqm_pad_o;
output		mc_oe_pad_o_;
output		mc_we_pad_o_;
output		mc_cas_pad_o_;
output		mc_ras_pad_o_;
output		mc_cke_pad_o_;
output	[7:0]	mc_cs_pad_o_;
input		mc_sts_pad_i;
output		mc_rp_pad_o_;
output		mc_vpen_pad_o;
output		mc_adsc_pad_o_;
output		mc_adv_pad_o_;
output		mc_zz_pad_o;
output		mc_coe_pad_coe_o;

////////////////////////////////////////////////////////////////////
//
// Local Wires
//

// WISHBONE Interface Interconnects
wire		wb_read_go;
wire		wb_write_go;
wire		wb_first;
wire		wb_wait;
wire		mem_ack;

// Suspend Resume Interface
wire		susp_sel;

// Register File Interconnects
wire	[31:0]	rf_dout;
wire	[31:0]	csc;
wire	[31:0]	tms;
wire	[31:0]	sp_csc;
wire	[31:0]	sp_tms;
wire	[7:0]	cs;
wire		fs;
wire		cs_le;
wire	[7:0]	cs_need_rfr;
wire	[2:0]	ref_int;
wire	[31:0]	mem_dout;
wire		wp_err;

// Address Select Signals
wire	[12:0]	row_adr;
wire	[1:0]	bank_adr;
wire		cmd_a10;
wire		row_sel;
wire		next_adr;
wire	[10:0]	page_size;
wire		lmr_sel;
wire		wr_hold;

// OBCT Signals
wire		bank_set;
wire		bank_clr;
wire		bank_clr_all;
wire		bank_open;
wire		row_same;
wire	[7:0]	obct_cs;
wire		any_bank_open;

// Data path Controller Signals
wire		dv;
wire		pack_le0, pack_le1, pack_le2;	// Pack Latch Enable
wire		par_err;
wire	[31:0]	mc_data_od;
wire	[3:0]	mc_dp_od;
wire	[23:0]	mc_addr_d;
wire	[35:0]	mc_data_ir;

// Refresh Counter Signals
wire		rfr_req;
wire		rfr_ack;
wire	[7:0]	rfr_ps_val;

// Memory Timing Block Signals
wire		data_oe;
wire		oe_;
wire		we_;
wire		cas_;
wire		ras_;
wire		cke_;
wire		lmr_req;
wire		lmr_ack;
wire		init_req;
wire		init_ack;
wire	[7:0]	spec_req_cs;
wire		cs_en;
wire		wb_cycle, wr_cycle;
wire	[31:0]	tms_s;
wire	[31:0]	csc_s;
wire		mc_c_oe_d;
wire		mc_br_r;
wire		mc_bg_d;
wire		mc_adsc_d;
wire		mc_adv_d;
wire		mc_ack_r;
wire		err;
wire		mc_sts_i;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

assign obct_cs =	(rfr_ack | susp_sel) ? cs_need_rfr :
			(lmr_ack | init_ack) ? spec_req_cs : cs;

assign lmr_sel = lmr_ack | init_ack;

assign tms_s = lmr_sel ? sp_tms : tms;
assign csc_s = lmr_sel ? sp_csc : csc;


wire		not_mem_cyc;

assign	not_mem_cyc = wb_cyc_i & wb_stb_i & !( `MC_MEM_SEL );

reg		mem_ack_r;

always @(posedge clk_i)
	mem_ack_r <= #1 mem_ack;

////////////////////////////////////////////////////////////////////
//
// Modules
//

mc_rf		u0(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.wb_data_i(	wb_data_i	),
		.rf_dout(	rf_dout		),
		.wb_addr_i(	wb_addr_i	),
		.wb_we_i(	wb_we_i		),
		.wb_cyc_i(	wb_cyc_i	),
		.wb_stb_i(	wb_stb_i 	),
		.wb_ack_o(			),
		.wp_err(	wp_err		),
		.csc(		csc		),
		.tms(		tms		),
		.poc(		poc_o		),
		.sp_csc(	sp_csc		),
		.sp_tms(	sp_tms		),
		.cs(		cs		),
		.mc_data_i(	mc_data_ir[31:0]),
		.mc_sts(	mc_sts_ir	),
		.mc_vpen(	mc_vpen_pad_o 	),
		.fs(		fs		),
		.cs_le(		cs_le		),
		.cs_le_d(	cs_le_d		),
		.cs_need_rfr(	cs_need_rfr	),
		.ref_int(	ref_int		),
		.rfr_ps_val(	rfr_ps_val	),
		.spec_req_cs(	spec_req_cs	),
		.init_req(	init_req	),
		.init_ack(	init_ack	),
		.lmr_req(	lmr_req		),
		.lmr_ack(	lmr_ack		)
		);

mc_adr_sel	u1(
		.clk(		clk_i		),
		.csc(		csc_s		),
		.tms(		tms_s		),
		.wb_stb_i(	wb_stb_i 	),
		//.wb_ack_o(	wb_ack_o	),
		.wb_ack_o(	mem_ack_r	),
		.wb_addr_i(	wb_addr_i	),
		.wb_we_i(	wb_we_i		),
		.wb_write_go(	wb_write_go	),
		.wr_hold(	wr_hold		),
		.cas_(		cas_		),
		.mc_addr(	mc_addr_d	),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.rfr_ack(	rfr_ack		),
		.cs_le(		cs_le		),
		.cmd_a10(	cmd_a10		),
		.row_sel(	row_sel		),
		.lmr_sel(	lmr_sel		),
		.next_adr(	next_adr	),
		.wr_cycle(	wr_cycle	),
		.page_size(	page_size	)
		);

mc_obct_top	u2(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.cs(		obct_cs		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set	),
		.bank_clr(	bank_clr	),
		.bank_clr_all(	bank_clr_all	),
		.bank_open(	bank_open	),
		.any_bank_open(	any_bank_open	),
		.row_same(	row_same	),
		.rfr_ack(	rfr_ack		)
		);

mc_dp		u3(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.csc(		csc		),
		.wb_cyc_i(	wb_cyc_i	),
		.wb_stb_i(	wb_stb_i	),
		.mem_ack(	mem_ack 	),
		//.wb_ack_o(	wb_ack_o 	),
		.wb_ack_o(	mem_ack_r 	),
		.wb_we_i(	wb_we_i		),
		.wb_data_i(	wb_data_i	),
		.wb_data_o(	mem_dout	),
		.wb_read_go(	wb_read_go	),
		.mc_clk(	mc_clk_i	),
		.mc_data_del(	mc_data_ir	),
		.mc_dp_i(	mc_dp_pad_i	),
		.mc_data_o(	mc_data_od	),
		.mc_dp_o(	mc_dp_od	),
		.dv(		dv		),
		.pack_le0(	pack_le0	),
		.pack_le1(	pack_le1	),
		.pack_le2(	pack_le2	),
		.byte_en(	wb_sel_i	),
		.par_err(	par_err		)
		);

mc_refresh	u4(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.cs_need_rfr(	cs_need_rfr	),
		.ref_int(	ref_int		),
		.rfr_req(	rfr_req		),
		.rfr_ack(	rfr_ack		),
		.rfr_ps_val(	rfr_ps_val	)
		);
 
mc_timing	u5(
		.clk(		clk_i		),
		.mc_clk(	mc_clk_i	),
		.rst(		rst_i		),
		.wb_cyc_i(	wb_cyc_i	),
		.wb_stb_i(	wb_stb_i 	),
		.wb_we_i(	wb_we_i		),
		.wb_read_go(	wb_read_go	),
		.wb_write_go(	wb_write_go	),
		.wb_first(	wb_first	),
		.wb_wait(	wb_wait		),
		.mem_ack(	mem_ack		),
		.err(		err		),
		.susp_req(	susp_req_i	),
		.resume_req(	resume_req_i	),
		.suspended(	suspended_o	),
		.susp_sel(	susp_sel	),
		.mc_br(		mc_br_r		),
		.mc_bg(		mc_bg_d		),
		.mc_ack(	mc_ack_r	),
		.not_mem_cyc(	not_mem_cyc	),
		.data_oe(	data_oe		),
		.oe_(		oe_		),
		.we_(		we_		),
		.cas_(		cas_		),
		.ras_(		ras_		),
		.cke_(		cke_		),
		.cs_en(		cs_en		),
		.mc_adsc(	mc_adsc_d	),
		.mc_adv(	mc_adv_d	),
		.mc_c_oe(	mc_c_oe_d	),
		.wb_cycle(	wb_cycle	),
		.wr_cycle(	wr_cycle	),
		.csc(		csc_s		),
		.tms(		tms_s		),
		.cs(		obct_cs		),
		.lmr_req(	lmr_req		),
		.lmr_ack(	lmr_ack		),
		.cs_le(		cs_le		),
		.cs_le_d(	cs_le_d		),
		.cmd_a10(	cmd_a10		),
		.row_sel(	row_sel		),
		.next_adr(	next_adr	),
		.page_size(	page_size	),
		.bank_set(	bank_set	),
		.bank_clr(	bank_clr	),
		.bank_clr_all(	bank_clr_all	),
		.bank_open(	bank_open	),
		.any_bank_open(	any_bank_open	),
		.row_same(	row_same	),
		.dv(		dv		),
		.pack_le0(	pack_le0	),
		.pack_le1(	pack_le1	),
		.pack_le2(	pack_le2	),
		.par_err(	par_err		),
		.rfr_req(	rfr_req		),
		.rfr_ack(	rfr_ack		),
		.init_req(	init_req	),
		.init_ack(	init_ack	)
		);

mc_wb_if	u6(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.wb_addr_i(	wb_addr_i	),
		.wb_cyc_i(	wb_cyc_i	),
		.wb_stb_i(	wb_stb_i 	),
		.wb_we_i(	wb_we_i		),
		.wb_ack_o(	wb_ack_o	),
		.wb_err(	wb_err_o	),
		.wb_read_go(	wb_read_go	),
		.wb_write_go(	wb_write_go	),
		.wb_first(	wb_first	),
		.wb_wait(	wb_wait		),
		.mem_ack(	mem_ack		),
		.wr_hold(	wr_hold		),
		.err(		err		),
		.par_err(	par_err		),
		.wp_err(	wp_err		),
		.wb_data_o(	wb_data_o	),
		.mem_dout(	mem_dout	),
		.rf_dout(	rf_dout		)
		);

mc_mem_if	u7(
		.clk(		clk_i		),
		.rst(		rst_i		),
		.mc_rp(		mc_rp_pad_o_	),
		.mc_clk(	mc_clk_i	),
		.mc_br(		mc_br_pad_i	),
		.mc_bg(		mc_bg_pad_o	),
		.mc_addr(	mc_addr_pad_o	),
		.mc_data_o(	mc_data_pad_o	),
		.mc_dp_o(	mc_dp_pad_o	),
		.mc_data_oe(	mc_doe_pad_doe_o),
		.mc_dqm(	mc_dqm_pad_o	),
		.mc_oe_(	mc_oe_pad_o_	),
		.mc_we_(	mc_we_pad_o_	),
		.mc_cas_(	mc_cas_pad_o_	),
		.mc_ras_(	mc_ras_pad_o_	),
		.mc_cke_(	mc_cke_pad_o_	),
		.mc_cs_(	mc_cs_pad_o_	),
		.mc_adsc_(	mc_adsc_pad_o_	),
		.mc_adv_(	mc_adv_pad_o_	),
		.mc_br_r(	mc_br_r		),
		.mc_bg_d(	mc_bg_d		),
		.mc_data_od(	mc_data_od	),
		.mc_dp_od(	mc_dp_od	),
		.mc_addr_d(	mc_addr_d	),
		.mc_ack(	mc_ack_pad_i	),
		.mc_zz_o(	mc_zz_pad_o	),
		.we_(		we_		),
		.ras_(		ras_		),
		.cas_(		cas_		),
		.cke_(		cke_		),
		.mc_adsc_d(	mc_adsc_d	),
		.mc_adv_d(	mc_adv_d	),
		.cs_en(		cs_en		),
		.rfr_ack(	rfr_ack		),
		.cs_need_rfr(	cs_need_rfr	),
		.lmr_sel(	lmr_sel		),
		.spec_req_cs(	spec_req_cs	),
		.cs(		cs		),
		.fs(		fs		),
		.data_oe(	data_oe		),
		.susp_sel(	susp_sel	),
		.suspended_o(	suspended_o	),
		.mc_c_oe(	mc_coe_pad_coe_o),
		.mc_c_oe_d(	mc_c_oe_d	),
		.mc_ack_r(	mc_ack_r	),
		.oe_(		oe_		),
		.wb_cyc_i(	wb_cyc_i 	),
		.wb_stb_i(	wb_stb_i 	),
		.wb_sel_i(	wb_sel_i	),
		.wb_cycle(	wb_cycle	),
		.wr_cycle(	wr_cycle	),
		.mc_data_i(	mc_data_pad_i	),
		.mc_dp_i(	mc_dp_pad_i	),
		.mc_data_ir(	mc_data_ir	),
		.mc_sts_i(	mc_sts_pad_i	),
		.mc_sts_ir(	mc_sts_ir	)
		);

endmodule
