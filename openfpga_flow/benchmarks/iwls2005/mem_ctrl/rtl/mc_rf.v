/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE Memory Controller Register File                   ////
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
//  $Id: mc_rf.v,v 1.8 2002/01/21 13:08:52 rudi Exp $
//
//  $Date: 2002/01/21 13:08:52 $
//  $Revision: 1.8 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: mc_rf.v,v $
//               Revision 1.8  2002/01/21 13:08:52  rudi
//
//               Fixed several minor bugs, cleaned up the code further ...
//
//               Revision 1.7  2001/12/21 05:09:29  rudi
//
//               - Fixed combinatorial loops in synthesis
//               - Fixed byte select bug
//
//               Revision 1.6  2001/12/11 02:47:19  rudi
//
//               - Made some changes not to expect clock during reset ...
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
//               Revision 1.4  2001/10/04 03:19:37  rudi
//
//               Fixed Register reads
//               Tightened up timing for register rd/wr
//
//               Revision 1.3  2001/09/24 00:38:21  rudi
//
//               Changed Reset to be active high and async.
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
//               Revision 1.1.1.1  2001/05/13 09:39:42  rudi
//               Created Directory Structure
//
//
//
//

`include "mc_defines.v"

module mc_rf(clk, rst,

	wb_data_i, rf_dout, wb_addr_i, wb_we_i, wb_cyc_i,
	wb_stb_i, wb_ack_o, wp_err,

	csc, tms, poc,
	sp_csc, sp_tms, cs,
	mc_data_i, mc_sts, mc_vpen, fs,

	cs_le_d, cs_le, cs_need_rfr, ref_int, rfr_ps_val, init_req,
	init_ack, lmr_req, lmr_ack,
	spec_req_cs
	);

input		clk, rst;

// --------------------------------------
// WISHBONE INTERFACE 

// Slave Interface
input	[31:0]	wb_data_i;
output	[31:0]	rf_dout;
input	[31:0]	wb_addr_i;
input		wb_we_i;
input		wb_cyc_i;
input		wb_stb_i;
output		wb_ack_o;
output		wp_err;

// --------------------------------------
// Misc Signals
output	[31:0]	csc;
output	[31:0]	tms;
output	[31:0]	poc;
output	[31:0]	sp_csc;
output	[31:0]	sp_tms;
output	[7:0]	cs;

input	[31:0]	mc_data_i;
input		mc_sts;
output		mc_vpen;
output		fs;

input		cs_le_d;
input		cs_le;

output	[7:0]	cs_need_rfr;	// Indicates which chip selects have SDRAM
				// attached and need to be refreshed
output	[2:0]	ref_int;	// Refresh Interval
output	[7:0]	rfr_ps_val;

output		init_req;
input		init_ack;
output		lmr_req;
input		lmr_ack;

output	[7:0]	spec_req_cs;

////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg		wb_ack_o;

reg	[31:0]	csc;
reg	[31:0]	tms;
reg	[31:0]	sp_csc;
reg	[31:0]	sp_tms;
reg	[31:0]	rf_dout;
reg	[7:0]	cs;

reg		rf_we;
wire	[31:0]	csr;
reg	[10:0]	csr_r;
reg	[7:0]	csr_r2;
reg	[31:0]	poc;

wire	[31:0]	csc_mask;
reg	[10:0]	csc_mask_r;

wire	[31:0]	csc0, tms0;
wire	[31:0]	csc1, tms1;
wire	[31:0]	csc2, tms2;
wire	[31:0]	csc3, tms3;
wire	[31:0]	csc4, tms4;
wire	[31:0]	csc5, tms5;
wire	[31:0]	csc6, tms6;
wire	[31:0]	csc7, tms7;

wire		cs0, cs1, cs2, cs3;
wire		cs4, cs5, cs6, cs7;
wire		wp_err0, wp_err1, wp_err2, wp_err3;
wire		wp_err4, wp_err5, wp_err6, wp_err7;
reg		wp_err;

wire		lmr_req7, lmr_req6, lmr_req5, lmr_req4;
wire		lmr_req3, lmr_req2, lmr_req1, lmr_req0;
wire		lmr_ack7, lmr_ack6, lmr_ack5, lmr_ack4;
wire		lmr_ack3, lmr_ack2, lmr_ack1, lmr_ack0;

wire		init_req7, init_req6, init_req5, init_req4;
wire		init_req3, init_req2, init_req1, init_req0;
wire		init_ack7, init_ack6, init_ack5, init_ack4;
wire		init_ack3, init_ack2, init_ack1, init_ack0;

reg		init_ack_r;
wire		init_ack_fe;
reg		lmr_ack_r;
wire		lmr_ack_fe;
wire	[7:0]	spec_req_cs_t;
wire	[7:0]	spec_req_cs_d;
reg	[7:0]	spec_req_cs;
reg		init_req, lmr_req;
reg		sreq_cs_le;

// Aliases
assign csr = {csr_r2, 8'h0, 5'h0, csr_r};
assign csc_mask = {21'h0, csc_mask_r};

////////////////////////////////////////////////////////////////////
//
// WISHBONE Register Read logic
//

always @(wb_addr_i or csr or poc or csc_mask or csc0 or tms0 or csc1 or
	tms1 or csc2 or tms2 or csc3 or tms3 or csc4 or tms4 or csc5 or
	tms5 or csc6 or tms6 or csc7 or tms7)
	case(wb_addr_i[6:2])		// synopsys full_case parallel_case
	   5'h00:	rf_dout <= #1 csr;
	   5'h01:	rf_dout <= #1 poc;
	   5'h02:	rf_dout <= #1 csc_mask;

	   5'h04:	rf_dout <= #1 csc0;
	   5'h05:	rf_dout <= #1 tms0;
	   5'h06:	rf_dout <= #1 csc1;
	   5'h07:	rf_dout <= #1 tms1;
	   5'h08:	rf_dout <= #1 csc2;
	   5'h09:	rf_dout <= #1 tms2;
	   5'h0a:	rf_dout <= #1 csc3;
	   5'h0b:	rf_dout <= #1 tms3;
	   5'h0c:	rf_dout <= #1 csc4;
	   5'h0d:	rf_dout <= #1 tms4;
	   5'h0e:	rf_dout <= #1 csc5;
	   5'h0f:	rf_dout <= #1 tms5;
	   5'h10:	rf_dout <= #1 csc6;
	   5'h11:	rf_dout <= #1 tms6;
	   5'h12:	rf_dout <= #1 csc7;
	   5'h13:	rf_dout <= #1 tms7;
	endcase

////////////////////////////////////////////////////////////////////
//
// WISHBONE Register Write logic
//

reg	[6:0]	wb_addr_r;

always @(posedge clk)
	wb_addr_r <= #1 wb_addr_i[6:0];

always @(posedge clk or posedge rst)
	if(rst)		rf_we <= #1 1'b0;
	else		rf_we <= #1 `MC_REG_SEL & wb_we_i & wb_cyc_i & wb_stb_i & !rf_we;

always @(posedge clk or posedge rst)
	if(rst)		csr_r2 <= #1 8'h0;
	else
	if(rf_we & (wb_addr_r[6:2] == 5'h0) )
			csr_r2 <= #1 wb_data_i[31:24];

always @(posedge clk or posedge rst)
	if(rst)		csr_r[10:1] <= #1 10'h0;
	else
	if(rf_we & (wb_addr_r[6:2] == 5'h0) )
			csr_r[10:1] <= #1 wb_data_i[10:1];

always @(posedge clk)
	csr_r[0] <= #1 mc_sts;

assign mc_vpen = csr_r[1];
assign fs = csr_r[2];
assign rfr_ps_val = csr_r2[7:0];

always @(posedge clk or posedge rst)
	if(rst)		csc_mask_r <= #1 11'h7ff;
	else
	if(rf_we & (wb_addr_r[6:2] == 5'h2) )
			csc_mask_r <= #1 wb_data_i[10:0];

////////////////////////////////////////////////////////////////////
//
// A kludge for cases where there is no clock during reset ...
//

reg	rst_r1, rst_r2, rst_r3;

always @(posedge clk or posedge rst)
	if(rst)		rst_r1 <= #1 1'b1;
	else		rst_r1 <= #1 1'b0;

always @(posedge clk or posedge rst)
	if(rst)		rst_r2 <= #1 1'b1;
	else		rst_r2 <= #1 rst_r1;

always @(posedge clk or posedge rst)
	if(rst)		rst_r3 <= #1 1'b1;
	else		rst_r3 <= #1 rst_r2;

always @(posedge clk)
	if(rst_r3)	poc <= #1 mc_data_i;

////////////////////////////////////////////////////////////////////
//
// WISHBONE Register Ack logic
//

always @(posedge clk)
	wb_ack_o <= #1 `MC_REG_SEL & wb_cyc_i & wb_stb_i & !wb_ack_o;

////////////////////////////////////////////////////////////////////
//
// Select CSC and TMS Registers
//

always @(posedge clk or posedge rst)
	if(rst)		cs <= #1 8'h0;
	else
	if(cs_le)	cs <= #1 {cs7, cs6, cs5, cs4, cs3, cs2, cs1, cs0};

always @(posedge clk or posedge rst)
	if(rst)		wp_err <= #1 1'b0;
	else
	if(cs_le & wb_cyc_i & wb_stb_i)
			wp_err <= #1	wp_err7 | wp_err6 | wp_err5 | wp_err4 |
					wp_err3 | wp_err2 | wp_err1 | wp_err0;
	else
	if(!wb_cyc_i)	wp_err <= #1 1'b0;

always @(posedge clk or posedge rst)
	if(rst)		csc <= #1 32'h0;
	else
	if(cs_le_d & wb_cyc_i & wb_stb_i)
	   begin
		if(cs0)	csc <= #1 csc0;
		else
		if(cs1)	csc <= #1 csc1;
		else
		if(cs2)	csc <= #1 csc2;
		else
		if(cs3)	csc <= #1 csc3;
		else
		if(cs4)	csc <= #1 csc4;
		else
		if(cs5)	csc <= #1 csc5;
		else
		if(cs6)	csc <= #1 csc6;
		else	csc <= #1 csc7;
	   end

always @(posedge clk or posedge rst)
	if(rst)		tms <= #1 32'hffff_ffff;
	else
	if((cs_le_d | rf_we) & wb_cyc_i & wb_stb_i)
	   begin
		if(cs0)	tms <= #1 tms0;
		else
		if(cs1)	tms <= #1 tms1;
		else
		if(cs2)	tms <= #1 tms2;
		else
		if(cs3)	tms <= #1 tms3;
		else
		if(cs4)	tms <= #1 tms4;
		else
		if(cs5)	tms <= #1 tms5;
		else
		if(cs6)	tms <= #1 tms6;
		else	tms <= #1 tms7;
	   end

always @(posedge clk or posedge rst)
	if(rst)				sp_csc <= #1 32'h0;
	else
	if(cs_le_d & wb_cyc_i & wb_stb_i)
	   begin
		if(spec_req_cs[0])	sp_csc <= #1 csc0;
		else
		if(spec_req_cs[1])	sp_csc <= #1 csc1;
		else
		if(spec_req_cs[2])	sp_csc <= #1 csc2;
		else
		if(spec_req_cs[3])	sp_csc <= #1 csc3;
		else
		if(spec_req_cs[4])	sp_csc <= #1 csc4;
		else
		if(spec_req_cs[5])	sp_csc <= #1 csc5;
		else
		if(spec_req_cs[6])	sp_csc <= #1 csc6;
		else			sp_csc <= #1 csc7;
	   end

always @(posedge clk or posedge rst)
	if(rst)				sp_tms <= #1 32'hffff_ffff;
	else
	if((cs_le_d | rf_we) & wb_cyc_i & wb_stb_i)
	   begin
		if(spec_req_cs[0])	sp_tms <= #1 tms0;
		else
		if(spec_req_cs[1])	sp_tms <= #1 tms1;
		else
		if(spec_req_cs[2])	sp_tms <= #1 tms2;
		else
		if(spec_req_cs[3])	sp_tms <= #1 tms3;
		else
		if(spec_req_cs[4])	sp_tms <= #1 tms4;
		else
		if(spec_req_cs[5])	sp_tms <= #1 tms5;
		else
		if(spec_req_cs[6])	sp_tms <= #1 tms6;
		else			sp_tms <= #1 tms7;
	   end

assign	cs_need_rfr[0] = csc0[0] & (csc0[3:1] == `MC_MEM_TYPE_SDRAM);
assign	cs_need_rfr[1] = csc1[0] & (csc1[3:1] == `MC_MEM_TYPE_SDRAM);
assign	cs_need_rfr[2] = csc2[0] & (csc2[3:1] == `MC_MEM_TYPE_SDRAM);
assign	cs_need_rfr[3] = csc3[0] & (csc3[3:1] == `MC_MEM_TYPE_SDRAM);
assign	cs_need_rfr[4] = csc4[0] & (csc4[3:1] == `MC_MEM_TYPE_SDRAM);
assign	cs_need_rfr[5] = csc5[0] & (csc5[3:1] == `MC_MEM_TYPE_SDRAM);
assign	cs_need_rfr[6] = csc6[0] & (csc6[3:1] == `MC_MEM_TYPE_SDRAM);
assign	cs_need_rfr[7] = csc7[0] & (csc7[3:1] == `MC_MEM_TYPE_SDRAM);

assign ref_int = csr_r[10:8];

////////////////////////////////////////////////////////////////////
//
// Init & Lmr Logic
//

// Init Ack falling edge detector
always @(posedge clk)
	init_ack_r <= #1 init_ack;

assign	init_ack_fe = init_ack_r & !init_ack;

// LMR Ack falling edge detector
always @(posedge clk)
	lmr_ack_r <= #1 lmr_ack;

assign lmr_ack_fe = lmr_ack_r & !lmr_ack;

// Chip Select Output
always @(posedge clk or posedge rst)
	if(rst)		spec_req_cs <= #1 8'h0;
	else
	if(sreq_cs_le)	spec_req_cs <= #1 spec_req_cs_d;

always @(posedge clk or posedge rst)
	if(rst)	sreq_cs_le <= #1 1'b0;
	else	sreq_cs_le <= #1 (!init_req & !lmr_req) | lmr_ack_fe | init_ack_fe;

// Make sure only one is serviced at a time
assign	spec_req_cs_d[0] = spec_req_cs_t[0];
assign	spec_req_cs_d[1] = spec_req_cs_t[1] & !spec_req_cs_t[0];
assign	spec_req_cs_d[2] = spec_req_cs_t[2] & !( |spec_req_cs_t[1:0] );
assign	spec_req_cs_d[3] = spec_req_cs_t[3] & !( |spec_req_cs_t[2:0] );
assign	spec_req_cs_d[4] = spec_req_cs_t[4] & !( |spec_req_cs_t[3:0] );
assign	spec_req_cs_d[5] = spec_req_cs_t[5] & !( |spec_req_cs_t[4:0] );
assign	spec_req_cs_d[6] = spec_req_cs_t[6] & !( |spec_req_cs_t[5:0] );
assign	spec_req_cs_d[7] = spec_req_cs_t[7] & !( |spec_req_cs_t[6:0] );

// Request Tracking
always @(posedge clk or posedge rst)
	if(rst)	init_req <= #1 1'b0;
	else	init_req <= #1	init_req0 | init_req1 | init_req2 | init_req3 |
				init_req4 | init_req5 | init_req6 | init_req7;

always @(posedge clk or posedge rst)
	if(rst)	lmr_req <= #1 1'b0;
	else	lmr_req <= #1	lmr_req0 | lmr_req1 | lmr_req2 | lmr_req3 |
				lmr_req4 | lmr_req5 | lmr_req6 | lmr_req7;

assign spec_req_cs_t = !init_req ?	// Load Mode Register Requests
				{lmr_req7, lmr_req6, lmr_req5, lmr_req4,
				lmr_req3, lmr_req2, lmr_req1, lmr_req0 } :
				// Initialize SDRAM Requests
				{init_req7, init_req6, init_req5, init_req4,
				init_req3, init_req2, init_req1, init_req0 };

// Ack distribution
assign lmr_ack0 = spec_req_cs[0] & lmr_ack_fe;
assign lmr_ack1 = spec_req_cs[1] & lmr_ack_fe;
assign lmr_ack2 = spec_req_cs[2] & lmr_ack_fe;
assign lmr_ack3 = spec_req_cs[3] & lmr_ack_fe;
assign lmr_ack4 = spec_req_cs[4] & lmr_ack_fe;
assign lmr_ack5 = spec_req_cs[5] & lmr_ack_fe;
assign lmr_ack6 = spec_req_cs[6] & lmr_ack_fe;
assign lmr_ack7 = spec_req_cs[7] & lmr_ack_fe;

assign init_ack0 = spec_req_cs[0] & init_ack_fe;
assign init_ack1 = spec_req_cs[1] & init_ack_fe;
assign init_ack2 = spec_req_cs[2] & init_ack_fe;
assign init_ack3 = spec_req_cs[3] & init_ack_fe;
assign init_ack4 = spec_req_cs[4] & init_ack_fe;
assign init_ack5 = spec_req_cs[5] & init_ack_fe;
assign init_ack6 = spec_req_cs[6] & init_ack_fe;
assign init_ack7 = spec_req_cs[7] & init_ack_fe;

////////////////////////////////////////////////////////////////////
//
// Modules
//

mc_cs_rf #(3'h0) u0(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc0		),
		.tms(		tms0		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs0		),
		.wp_err(	wp_err0		),
		.lmr_req(	lmr_req0	),
		.lmr_ack(	lmr_ack0	),
		.init_req(	init_req0	),
		.init_ack(	init_ack0	)
		);

`ifdef MC_HAVE_CS1
mc_cs_rf #(3'h1) u1(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc1		),
		.tms(		tms1		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs1		),
		.wp_err(	wp_err1		),
		.lmr_req(	lmr_req1	),
		.lmr_ack(	lmr_ack1	),
		.init_req(	init_req1	),
		.init_ack(	init_ack1	)
		);
`else
mc_cs_rf_dummy #(3'h1) u1(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc1		),
		.tms(		tms1		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs1		),
		.wp_err(	wp_err1		),
		.lmr_req(	lmr_req1	),
		.lmr_ack(	lmr_ack1	),
		.init_req(	init_req1	),
		.init_ack(	init_ack1	)
		);
`endif

`ifdef MC_HAVE_CS2
mc_cs_rf #(3'h2) u2(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc2		),
		.tms(		tms2		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs2		),
		.wp_err(	wp_err2		),
		.lmr_req(	lmr_req2	),
		.lmr_ack(	lmr_ack2	),
		.init_req(	init_req2	),
		.init_ack(	init_ack2	)
		);
`else
mc_cs_rf_dummy #(3'h2) u2(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc2		),
		.tms(		tms2		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs2		),
		.wp_err(	wp_err2		),
		.lmr_req(	lmr_req2	),
		.lmr_ack(	lmr_ack2	),
		.init_req(	init_req2	),
		.init_ack(	init_ack2	)
		);
`endif

`ifdef MC_HAVE_CS3
mc_cs_rf #(3'h3) u3(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc3		),
		.tms(		tms3		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs3		),
		.wp_err(	wp_err3		),
		.lmr_req(	lmr_req3	),
		.lmr_ack(	lmr_ack3	),
		.init_req(	init_req3	),
		.init_ack(	init_ack3	)
		);
`else
mc_cs_rf_dummy #(3'h3) u3(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc3		),
		.tms(		tms3		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs3		),
		.wp_err(	wp_err3		),
		.lmr_req(	lmr_req3	),
		.lmr_ack(	lmr_ack3	),
		.init_req(	init_req3	),
		.init_ack(	init_ack3	)
		);
`endif

`ifdef MC_HAVE_CS4
mc_cs_rf #(3'h4) u4(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc4		),
		.tms(		tms4		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs4		),
		.wp_err(	wp_err4		),
		.lmr_req(	lmr_req4	),
		.lmr_ack(	lmr_ack4	),
		.init_req(	init_req4	),
		.init_ack(	init_ack4	)
		);
`else
mc_cs_rf_dummy #(3'h4) u4(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc4		),
		.tms(		tms4		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs4		),
		.wp_err(	wp_err4		),
		.lmr_req(	lmr_req4	),
		.lmr_ack(	lmr_ack4	),
		.init_req(	init_req4	),
		.init_ack(	init_ack4	)
		);
`endif

`ifdef MC_HAVE_CS5
mc_cs_rf #(3'h5) u5(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc5		),
		.tms(		tms5		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs5		),
		.wp_err(	wp_err5		),
		.lmr_req(	lmr_req5	),
		.lmr_ack(	lmr_ack5	),
		.init_req(	init_req5	),
		.init_ack(	init_ack5	)
		);
`else
mc_cs_rf_dummy #(3'h5) u5(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc5		),
		.tms(		tms5		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs5		),
		.wp_err(	wp_err5		),
		.lmr_req(	lmr_req5	),
		.lmr_ack(	lmr_ack5	),
		.init_req(	init_req5	),
		.init_ack(	init_ack5	)
		);
`endif

`ifdef MC_HAVE_CS6
mc_cs_rf #(3'h6) u6(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc6		),
		.tms(		tms6		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs6		),
		.wp_err(	wp_err6		),
		.lmr_req(	lmr_req6	),
		.lmr_ack(	lmr_ack6	),
		.init_req(	init_req6	),
		.init_ack(	init_ack6	)
		);
`else
mc_cs_rf_dummy #(3'h6) u6(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc6		),
		.tms(		tms6		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs6		),
		.wp_err(	wp_err6		),
		.lmr_req(	lmr_req6	),
		.lmr_ack(	lmr_ack6	),
		.init_req(	init_req6	),
		.init_ack(	init_ack6	)
		);
`endif

`ifdef MC_HAVE_CS7
mc_cs_rf #(3'h7) u7(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc7		),
		.tms(		tms7		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs7		),
		.wp_err(	wp_err7		),
		.lmr_req(	lmr_req7	),
		.lmr_ack(	lmr_ack7	),
		.init_req(	init_req7	),
		.init_ack(	init_ack7	)
		);
`else
mc_cs_rf_dummy #(3'h7) u7(
		.clk(		clk		),
		.rst(		rst		),
		.wb_we_i(	wb_we_i		),
		.din(		wb_data_i	),
		.rf_we(		rf_we		),
		.addr(		wb_addr_i	),
		.csc(		csc7		),
		.tms(		tms7		),
		.poc(		poc		),
		.csc_mask(	csc_mask	),
		.cs(		cs7		),
		.wp_err(	wp_err7		),
		.lmr_req(	lmr_req7	),
		.lmr_ack(	lmr_ack7	),
		.init_req(	init_req7	),
		.init_ack(	init_ack7	)
		);
`endif

endmodule

