/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE DMA One Channel Register File                     ////
////                                                             ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/wb_dma/    ////
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
//  $Id: wb_dma_ch_rf.v,v 1.5 2002/02/01 01:54:45 rudi Exp $
//
//  $Date: 2002/02/01 01:54:45 $
//  $Revision: 1.5 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: wb_dma_ch_rf.v,v $
//               Revision 1.5  2002/02/01 01:54:45  rudi
//
//               - Minor cleanup
//
//               Revision 1.4  2001/10/30 02:06:17  rudi
//
//               - Fixed problem where synthesis tools would instantiate latches instead of flip-flops
//
//               Revision 1.3  2001/10/19 04:35:04  rudi
//
//               - Made the core parameterized
//
//               Revision 1.2  2001/08/15 05:40:30  rudi
//
//               - Changed IO names to be more clear.
//               - Uniquifyed define names to be core specific.
//               - Added Section 3.10, describing DMA restart.
//
//               Revision 1.1  2001/07/29 08:57:02  rudi
//
//
//               1) Changed Directory Structure
//               2) Added restart signal (REST)
//
//               Revision 1.3  2001/06/14 08:50:01  rudi
//
//               Changed Module Name to match file name.
//
//               Revision 1.2  2001/06/13 02:26:48  rudi
//
//
//               Small changes after running lint.
//
//               Revision 1.1  2001/06/05 10:25:27  rudi
//
//
//               Initial checkin of register file for one channel.
//
//
//
//

`include "wb_dma_defines.v"

module wb_dma_ch_rf(	clk, rst,
			pointer, pointer_s, ch_csr, ch_txsz, ch_adr0, ch_adr1,
			ch_am0, ch_am1, sw_pointer, ch_stop, ch_dis, int,

			wb_rf_din, wb_rf_adr, wb_rf_we, wb_rf_re,

			// DMA Registers Write Back Channel Select
			ch_sel, ndnr,

			// DMA Engine Status
			dma_busy, dma_err, dma_done, dma_done_all,

			// DMA Engine Reg File Update ctrl signals
			de_csr, de_txsz, de_adr0, de_adr1,
			de_csr_we, de_txsz_we, de_adr0_we, de_adr1_we,
			de_fetch_descr, dma_rest,
			ptr_set

		);

parameter	[4:0]	CH_NO    = 5'h0;  // This Instances Channel ID
parameter	[0:0]	CH_EN    = 1'b1;  // This channel exists
parameter	[0:0]	HAVE_ARS = 1'b1;  // 1=this Instance Supports ARS
parameter	[0:0]	HAVE_ED  = 1'b1;  // 1=this Instance Supports External Descriptors
parameter	[0:0]	HAVE_CBUF= 1'b1;  // 1=this Instance Supports Cyclic Buffers

input		clk, rst;

output	[31:0]	pointer;
output	[31:0]	pointer_s;
output	[31:0]	ch_csr;
output	[31:0]	ch_txsz;
output	[31:0]	ch_adr0;
output	[31:0]	ch_adr1;
output	[31:0]	ch_am0;
output	[31:0]	ch_am1;
output	[31:0]	sw_pointer;
output		ch_stop;
output		ch_dis;
output		int;

input	[31:0]	wb_rf_din;
input	[7:0]	wb_rf_adr;
input		wb_rf_we;
input		wb_rf_re;

input	[4:0]	ch_sel;
input		ndnr;

// DMA Engine Status
input		dma_busy, dma_err, dma_done, dma_done_all;

// DMA Engine Reg File Update ctrl signals
input	[31:0]	de_csr;
input	[11:0]	de_txsz;
input	[31:0]	de_adr0;
input	[31:0]	de_adr1;
input		de_csr_we, de_txsz_we, de_adr0_we, de_adr1_we, ptr_set;
input		de_fetch_descr;
input		dma_rest;

////////////////////////////////////////////////////////////////////
//
// Local Wires and Registers
//

wire	[31:0]	pointer;
reg	[27:0]	pointer_r;
reg	[27:0]	pointer_sr;
reg		ptr_valid;
reg		ch_eol;

wire	[31:0]	ch_csr, ch_txsz;

reg	[8:0]	ch_csr_r;
reg	[2:0]	ch_csr_r2;
reg	[2:0]	ch_csr_r3;
reg	[2:0]	int_src_r;
reg		ch_err_r;
reg		ch_stop;
reg		ch_busy;
reg		ch_done;
reg		ch_err;
reg		rest_en;

reg	[10:0]	ch_chk_sz_r;
reg	[11:0]	ch_tot_sz_r;
reg	[22:0]	ch_txsz_s;
reg		ch_sz_inf;

wire	[31:0]	ch_adr0, ch_adr1;
reg	[29:0]	ch_adr0_r, ch_adr1_r;
wire	[31:0]	ch_am0, ch_am1;
reg	[27:0]	ch_am0_r, ch_am1_r;

reg	[29:0]	ch_adr0_s, ch_adr1_s;

reg	[29:0]	sw_pointer_r;
wire		sw_pointer_we;
wire	[28:0]	cmp_adr;
reg		ch_dis;
wire		ch_enable;

wire		pointer_we;
wire		ch_csr_we, ch_csr_re, ch_txsz_we, ch_adr0_we, ch_adr1_we;
wire		ch_am0_we, ch_am1_we;
reg		ch_rl;
wire		ch_done_we;
wire		ch_err_we;
wire		chunk_done_we;

wire		ch_csr_dewe, ch_txsz_dewe, ch_adr0_dewe, ch_adr1_dewe;

wire		this_ptr_set;
wire		ptr_inv;

////////////////////////////////////////////////////////////////////
//
// Aliases
//

assign ch_adr0		= CH_EN ? {ch_adr0_r, 2'h0}   : 32'h0;
assign ch_adr1		= CH_EN ? {ch_adr1_r, 2'h0}   : 32'h0;
assign ch_am0		= (CH_EN & HAVE_CBUF) ? {ch_am0_r, 4'h0}    : 32'hffff_fff0;
assign ch_am1		= (CH_EN & HAVE_CBUF) ? {ch_am1_r, 4'h0}    : 32'hffff_fff0;
assign sw_pointer	= (CH_EN & HAVE_CBUF) ? {sw_pointer_r,2'h0} : 32'h0;

assign pointer		= CH_EN ? {pointer_r, 3'h0, ptr_valid} : 32'h0;
assign pointer_s	= CH_EN ? {pointer_sr, 4'h0}  : 32'h0;
assign ch_csr		= CH_EN ? {9'h0, int_src_r, ch_csr_r3, rest_en, ch_csr_r2,
					ch_err, ch_done, ch_busy, 1'b0, ch_csr_r[8:1], ch_enable} : 32'h0;
assign ch_txsz		= CH_EN ? {5'h0, ch_chk_sz_r, ch_sz_inf, 3'h0, ch_tot_sz_r} : 32'h0;

assign ch_enable	= CH_EN ? (ch_csr_r[`WDMA_CH_EN] & (HAVE_CBUF ? !ch_dis : 1'b1) ) : 1'b0;

////////////////////////////////////////////////////////////////////
//
// CH0 control signals
//

parameter	[4:0]	CH_ADR = CH_NO + 5'h1;

assign ch_csr_we	= CH_EN & wb_rf_we & (wb_rf_adr[7:3] == CH_ADR) & (wb_rf_adr[2:0] == 3'h0);
assign ch_csr_re	= CH_EN & wb_rf_re & (wb_rf_adr[7:3] == CH_ADR) & (wb_rf_adr[2:0] == 3'h0);
assign ch_txsz_we	= CH_EN & wb_rf_we & (wb_rf_adr[7:3] == CH_ADR) & (wb_rf_adr[2:0] == 3'h1);
assign ch_adr0_we	= CH_EN & wb_rf_we & (wb_rf_adr[7:3] == CH_ADR) & (wb_rf_adr[2:0] == 3'h2);
assign ch_am0_we	= CH_EN & wb_rf_we & (wb_rf_adr[7:3] == CH_ADR) & (wb_rf_adr[2:0] == 3'h3);
assign ch_adr1_we	= CH_EN & wb_rf_we & (wb_rf_adr[7:3] == CH_ADR) & (wb_rf_adr[2:0] == 3'h4);
assign ch_am1_we	= CH_EN & wb_rf_we & (wb_rf_adr[7:3] == CH_ADR) & (wb_rf_adr[2:0] == 3'h5);
assign pointer_we	= CH_EN & wb_rf_we & (wb_rf_adr[7:3] == CH_ADR) & (wb_rf_adr[2:0] == 3'h6);
assign sw_pointer_we	= CH_EN & wb_rf_we & (wb_rf_adr[7:3] == CH_ADR) & (wb_rf_adr[2:0] == 3'h7);

assign ch_done_we	= CH_EN & (((ch_sel==CH_NO) & dma_done_all) | ndnr) &
			  (ch_csr[`WDMA_USE_ED] ? ch_eol : !ch_csr[`WDMA_ARS]);
assign chunk_done_we	= CH_EN & (ch_sel==CH_NO) & dma_done;
assign ch_err_we	= CH_EN & (ch_sel==CH_NO) & dma_err;
assign ch_csr_dewe	= CH_EN & de_csr_we & (ch_sel==CH_NO);
assign ch_txsz_dewe	= CH_EN & de_txsz_we & (ch_sel==CH_NO);
assign ch_adr0_dewe	= CH_EN & de_adr0_we & (ch_sel==CH_NO);
assign ch_adr1_dewe	= CH_EN & de_adr1_we & (ch_sel==CH_NO);

assign ptr_inv		= CH_EN & ((ch_sel==CH_NO) & dma_done_all) | ndnr;
assign this_ptr_set	= CH_EN & ptr_set & (ch_sel==CH_NO);

always @(posedge clk)
	ch_rl <= #1	CH_EN & HAVE_ARS & (
			(rest_en & dma_rest) |
			((ch_sel==CH_NO) & dma_done_all & ch_csr[`WDMA_ARS] & !ch_csr[`WDMA_USE_ED])
			);

// ---------------------------------------------------
// Pointers

always @(posedge clk or negedge rst)
	if(!rst)			ptr_valid <= #1 1'b0;
	else
	if(CH_EN & HAVE_ED)
	   begin
		if( this_ptr_set | (rest_en & dma_rest) )
					ptr_valid <= #1 1'b1;
		else
		if(ptr_inv)		ptr_valid <= #1 1'b0;
	   end
	else				ptr_valid <= #1 1'b0;

always @(posedge clk or negedge rst)
	if(!rst)			ch_eol <= #1 1'b0;
	else
	if(CH_EN & HAVE_ED)
	   begin
		if(ch_csr_dewe)		ch_eol <= #1 de_csr[`WDMA_ED_EOL];
		else
		if(ch_done_we)		ch_eol <= #1 1'b0;
	   end
	else				ch_eol <= #1 1'b0;

always @(posedge clk)
	if(CH_EN & HAVE_ED)
	   begin
		if(pointer_we)		pointer_r <= #1 wb_rf_din[31:4];
		else
		if(this_ptr_set)	pointer_r <= #1 de_csr[31:4];
	   end
	else				pointer_r <= #1 1'b0;

always @(posedge clk)
	if(CH_EN & HAVE_ED)
	   begin
		if(this_ptr_set)	pointer_sr <= #1 pointer_r;
	   end
	else				pointer_sr <= #1 1'b0;

// ---------------------------------------------------
// CSR

always @(posedge clk or negedge rst)
	if(!rst)			ch_csr_r <= #1 1'b0;
	else
	if(CH_EN)
	   begin
		if(ch_csr_we)		ch_csr_r <= #1 wb_rf_din[8:0];
		else
		   begin
			if(ch_done_we)	ch_csr_r[`WDMA_CH_EN] <= #1 1'b0;
			if(ch_csr_dewe)	ch_csr_r[4:1] <= #1 de_csr[19:16];
		   end
	   end

// done bit
always @(posedge clk or negedge rst)
	if(!rst)		ch_done <= #1 1'b0;
	else
	if(CH_EN)
	   begin
		if(ch_csr_we)		ch_done <= #1 !wb_rf_din[`WDMA_CH_EN];
		else
		if(ch_done_we)		ch_done <= #1 1'b1;
	   end

// busy bit
always @(posedge clk)
	ch_busy <= #1 CH_EN & (ch_sel==CH_NO) & dma_busy;

// stop bit
always @(posedge clk)
	ch_stop <= #1 CH_EN & ch_csr_we & wb_rf_din[`WDMA_STOP];

// error bit
always @(posedge clk or negedge rst)
	if(!rst)			ch_err <= #1 1'b0;
	else
	if(CH_EN)
	   begin
		if(ch_err_we)		ch_err <= #1 1'b1;
		else
		if(ch_csr_re)		ch_err <= #1 1'b0;
	   end

// Priority Bits
always @(posedge clk or negedge rst)
	if(!rst)			ch_csr_r2 <= #1 3'h0;
	else
	if(CH_EN & ch_csr_we)		ch_csr_r2 <= #1 wb_rf_din[15:13];

// Restart Enable Bit (REST)
always @(posedge clk or negedge rst)
	if(!rst)			rest_en <= #1 1'b0;
	else
	if(CH_EN & ch_csr_we)		rest_en <= #1 wb_rf_din[16];

// INT Mask
always @(posedge clk or negedge rst)
	if(!rst)			ch_csr_r3 <= #1 3'h0;
	else
	if(CH_EN & ch_csr_we)		ch_csr_r3 <= #1 wb_rf_din[19:17];

// INT Source
always @(posedge clk or negedge rst)
	if(!rst)			int_src_r[2] <= #1 1'b0;
	else
	if(CH_EN)
	   begin
		if(chunk_done_we)	int_src_r[2] <= #1 1'b1;
		else
		if(ch_csr_re)		int_src_r[2] <= #1 1'b0;
	   end

always @(posedge clk or negedge rst)
	if(!rst)			int_src_r[1] <= #1 1'b0;
	else
	if(CH_EN)
	   begin
		if(ch_done_we)		int_src_r[1] <= #1 1'b1;
		else
		if(ch_csr_re)		int_src_r[1] <= #1 1'b0;
	   end

always @(posedge clk or negedge rst)
	if(!rst)			int_src_r[0] <= #1 1'b0;
	else
	if(CH_EN)
	   begin
		if(ch_err_we)		int_src_r[0] <= #1 1'b1;
		else
		if(ch_csr_re)		int_src_r[0] <= #1 1'b0;
	   end

// Interrupt Output
assign int = |(int_src_r & ch_csr_r3) & CH_EN;

// ---------------------------------------------------
// TXZS
always @(posedge clk)
	if(CH_EN)
	   begin
		if(ch_txsz_we)		
			{ch_chk_sz_r, ch_tot_sz_r} <= #1 {wb_rf_din[26:16], wb_rf_din[11:0]};
		else
		if(ch_txsz_dewe)
			ch_tot_sz_r <= #1 de_txsz;
		else
		if(ch_rl)
			{ch_chk_sz_r, ch_tot_sz_r} <= #1 ch_txsz_s;
	   end

// txsz shadow register
always @(posedge clk)
	if(CH_EN & HAVE_ARS)
	   begin

		if(ch_txsz_we)	ch_txsz_s <= #1 {wb_rf_din[26:16], wb_rf_din[11:0]};
		else
		if(rest_en & ch_txsz_dewe & de_fetch_descr)
				ch_txsz_s[11:0] <= #1 de_txsz[11:0];
	   end

// Infinite Size indicator
always @(posedge clk)
	if(CH_EN)
	   begin
		if(ch_txsz_we)		ch_sz_inf <= #1 wb_rf_din[15];
	   end
	
// ---------------------------------------------------
// ADR0
always @(posedge clk)
	if(CH_EN)
	   begin
		if(ch_adr0_we)		ch_adr0_r <= #1 wb_rf_din[31:2];
		else
		if(ch_adr0_dewe)	ch_adr0_r <= #1 de_adr0[31:2];
		else
		if(ch_rl)		ch_adr0_r <= #1 ch_adr0_s;
	   end

// Adr0 shadow register
always @(posedge clk)
	if(CH_EN & HAVE_ARS)
	   begin
		if(ch_adr0_we)	ch_adr0_s <= #1 wb_rf_din[31:2];
		else
		if(rest_en & ch_adr0_dewe & de_fetch_descr)
				ch_adr0_s <= #1 de_adr0[31:2];
	   end

// ---------------------------------------------------
// AM0
always @(posedge clk or negedge rst)
	if(!rst)		ch_am0_r <= #1 28'hfffffff;
	else
	if(ch_am0_we)		ch_am0_r <= #1 wb_rf_din[31:4];

// ---------------------------------------------------
// ADR1
always @(posedge clk)
	if(CH_EN)
	   begin
		if(ch_adr1_we)		ch_adr1_r <= #1 wb_rf_din[31:2];
		else
		if(ch_adr1_dewe)	ch_adr1_r <= #1 de_adr1[31:2];
		else
		if(ch_rl)		ch_adr1_r <= #1 ch_adr1_s;
	   end

// Adr1 shadow register
always @(posedge clk)
	if(CH_EN & HAVE_ARS)
	   begin
		if(ch_adr1_we)	ch_adr1_s <= #1 wb_rf_din[31:2];
		else
		if(rest_en & ch_adr1_dewe & de_fetch_descr)
				ch_adr1_s <= #1 de_adr1[31:2];
	   end

// ---------------------------------------------------
// AM1
always @(posedge clk or negedge rst)
	if(!rst)				ch_am1_r <= #1 28'hfffffff;
	else
	if(ch_am1_we & CH_EN & HAVE_CBUF)	ch_am1_r <= #1 wb_rf_din[31:4];

// ---------------------------------------------------
// Software Pointer
always @(posedge clk or negedge rst)
	if(!rst)				sw_pointer_r <= #1 28'h0;
	else
	if(sw_pointer_we & CH_EN & HAVE_CBUF)	sw_pointer_r <= #1 wb_rf_din[31:4];

// ---------------------------------------------------
// Software Pointer Match logic

assign cmp_adr = ch_csr[2] ? ch_adr1[30:2] : ch_adr0[30:2];

always @(posedge clk)
	ch_dis <= #1 CH_EN & HAVE_CBUF & (sw_pointer[30:2] == cmp_adr) & sw_pointer[31];

endmodule


module wb_dma_ch_rf_dummy(clk, rst,
			pointer, pointer_s, ch_csr, ch_txsz, ch_adr0, ch_adr1,
			ch_am0, ch_am1, sw_pointer, ch_stop, ch_dis, int,

			wb_rf_din, wb_rf_adr, wb_rf_we, wb_rf_re,

			// DMA Registers Write Back Channel Select
			ch_sel, ndnr,

			// DMA Engine Status
			dma_busy, dma_err, dma_done, dma_done_all,

			// DMA Engine Reg File Update ctrl signals
			de_csr, de_txsz, de_adr0, de_adr1,
			de_csr_we, de_txsz_we, de_adr0_we, de_adr1_we,
			de_fetch_descr, dma_rest,
			ptr_set

		);

parameter	CH_NO = 0;
parameter	HAVE_ARS = 1;
parameter	HAVE_ED  = 1;
parameter	HAVE_CBUF= 1;

input		clk, rst;

output	[31:0]	pointer;
output	[31:0]	pointer_s;
output	[31:0]	ch_csr;
output	[31:0]	ch_txsz;
output	[31:0]	ch_adr0;
output	[31:0]	ch_adr1;
output	[31:0]	ch_am0;
output	[31:0]	ch_am1;
output	[31:0]	sw_pointer;
output		ch_stop;
output		ch_dis;
output		int;

input	[31:0]	wb_rf_din;
input	[7:0]	wb_rf_adr;
input		wb_rf_we;
input		wb_rf_re;

input	[4:0]	ch_sel;
input		ndnr;

// DMA Engine Status
input		dma_busy, dma_err, dma_done, dma_done_all;

// DMA Engine Reg File Update ctrl signals
input	[31:0]	de_csr;
input	[11:0]	de_txsz;
input	[31:0]	de_adr0;
input	[31:0]	de_adr1;
input		de_csr_we, de_txsz_we, de_adr0_we, de_adr1_we, ptr_set;
input		de_fetch_descr;
input		dma_rest;

assign		pointer = 32'h0;
assign		pointer_s = 32'h0;
assign		ch_csr = 32'h0;
assign		ch_txsz = 32'h0;
assign		ch_adr0 = 32'h0;
assign		ch_adr1 = 32'h0;
assign		ch_am0 = 32'h0;
assign		ch_am1 = 32'h0;
assign		sw_pointer = 32'h0;
assign		ch_stop = 1'b0;
assign		ch_dis = 1'b0;
assign		int = 1'b0;

endmodule
