/////////////////////////////////////////////////////////////////////
////                                                             ////
////  Register File                                              ////
////  This module contains all top level registers and           ////
////  instantiates the register files for endpoints              ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/usb/       ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2000-2003 Rudolf Usselmann                    ////
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
//  $Id: usbf_rf.v,v 1.6 2003/10/17 02:36:57 rudi Exp $
//
//  $Date: 2003/10/17 02:36:57 $
//  $Revision: 1.6 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: usbf_rf.v,v $
//               Revision 1.6  2003/10/17 02:36:57  rudi
//               - Disabling bit stuffing and NRZI encoding during speed negotiation
//               - Now the core can send zero size packets
//               - Fixed register addresses for some of the higher endpoints
//                 (conversion between decimal/hex was wrong)
//               - The core now does properly evaluate the function address to
//                 determine if the packet was intended for it.
//               - Various other minor bugs and typos
//
//               Revision 1.5  2001/11/04 12:22:45  rudi
//
//               - Fixed previous fix (brocke something else ...)
//               - Majore Synthesis cleanup
//
//               Revision 1.4  2001/11/03 03:26:23  rudi
//
//               - Fixed several interrupt and error condition reporting bugs
//
//               Revision 1.3  2001/09/24 01:15:28  rudi
//
//               Changed reset to be active high async.
//
//               Revision 1.2  2001/08/10 08:48:33  rudi
//
//               - Changed IO names to be more clear.
//               - Uniquifyed define names to be core specific.
//
//               Revision 1.1  2001/08/03 05:30:09  rudi
//
//
//               1) Reorganized directory structure
//
//               Revision 1.2  2001/03/31 13:00:52  rudi
//
//               - Added Core configuration
//               - Added handling of OUT packets less than MAX_PL_SZ in DMA mode
//               - Modified WISHBONE interface and sync logic
//               - Moved SSRAM outside the core (added interface)
//               - Many small bug fixes ...
//
//               Revision 1.0  2001/03/07 09:17:12  rudi
//
//
//               Changed all revisions to revision 1.0. This is because OpenCores CVS
//               interface could not handle the original '0.1' revision ....
//
//               Revision 0.2  2001/03/07 09:08:13  rudi
//
//               Added USB controll signaling (Line Status) block. Fixed some minor
//               typos, added resume bit and signal.
//
//               Revision 0.1.0.1  2001/02/28 08:11:32  rudi
//               Initial Release
//
//

`include "usbf_defines.v"

// Endpoint register File
module usbf_rf(	clk, wclk, rst,

		// Wishbone Interface
		adr, re, we, din, dout, inta, intb,
		dma_req, dma_ack,

		// Internal Interface
		idin,
		ep_sel, match,
		buf0_rl, buf0_set, buf1_set,
		uc_bsel_set, uc_dpd_set,

		int_buf1_set, int_buf0_set, int_upid_set,
		int_crc16_set, int_to_set, int_seqerr_set,
		out_to_small,

		csr, buf0, buf1,
		funct_adr,
		dma_in_buf_sz1, dma_out_buf_avail,

		// Misc
		frm_nat,
		utmi_vend_stat, utmi_vend_ctrl, utmi_vend_wr,
		line_stat, usb_attached, mode_hs, suspend,
		attached, usb_reset, pid_cs_err, nse_err,
		crc5_err, rx_err, rf_resume_req
		);

input		clk, wclk, rst;
input	[6:0]	adr;
input		re;
input		we;
input	[31:0]	din;
output	[31:0]	dout;
output		inta, intb;
output	[15:0]	dma_req;
input	[15:0]	dma_ack;

input	[31:0]	idin;		// Data Input
input	[3:0]	ep_sel;		// Endpoint Number Input
output		match;		// Endpoint Matched
input		buf0_rl;	// Reload Buf 0 with original values

input		buf0_set;	// Write to buf 0
input		buf1_set;	// Write to buf 1
input		uc_bsel_set;	// Write to the uc_bsel field
input		uc_dpd_set;	// Write to the uc_dpd field
input		int_buf1_set;	// Set buf1 full/empty interrupt
input		int_buf0_set;	// Set buf0 full/empty interrupt
input		int_upid_set;	// Set unsupported PID interrupt
input		int_crc16_set;	// Set CRC16 error interrupt
input		int_to_set;	// Set time out interrupt
input		int_seqerr_set;	// Set PID Sequence Error Interrupt
input		out_to_small;	// OUT packet was to small for DMA operation

output	[31:0]	csr;		// Internal CSR Output
output	[31:0]	buf0;		// Internal Buf 0 Output
output	[31:0]	buf1;		// Internal Buf 1 Output
output	[6:0]	funct_adr;	// Function Address
output		dma_in_buf_sz1, dma_out_buf_avail;

input	[31:0]	frm_nat;

input	[7:0]	utmi_vend_stat;	// UTMI Vendor C/S bus
output	[3:0]	utmi_vend_ctrl;
output		utmi_vend_wr;

input	[1:0]	line_stat;	// Below are signals for interrupt generation
input		usb_attached;
input		mode_hs;
input		suspend;
input		attached;
input		usb_reset;
input		nse_err;
input		pid_cs_err;
input		crc5_err;
input		rx_err;
output		rf_resume_req;

///////////////////////////////////////////////////////////////////
//
// Local Wires and Registers
//

wire	[31:0]	ep0_dout, ep1_dout, ep2_dout, ep3_dout;
wire	[31:0]	ep4_dout, ep5_dout, ep6_dout, ep7_dout;
wire	[31:0]	ep8_dout, ep9_dout, ep10_dout, ep11_dout;
wire	[31:0]	ep12_dout, ep13_dout, ep14_dout, ep15_dout;

wire		ep0_re, ep1_re, ep2_re, ep3_re;
wire		ep4_re, ep5_re, ep6_re, ep7_re;
wire		ep8_re, ep9_re, ep10_re, ep11_re;
wire		ep12_re, ep13_re, ep14_re, ep15_re;

wire		ep0_we, ep1_we, ep2_we, ep3_we;
wire		ep4_we, ep5_we, ep6_we, ep7_we;
wire		ep8_we, ep9_we, ep10_we, ep11_we;
wire		ep12_we, ep13_we, ep14_we, ep15_we;

wire		ep0_inta, ep1_inta, ep2_inta, ep3_inta;
wire		ep4_inta, ep5_inta, ep6_inta, ep7_inta;
wire		ep8_inta, ep9_inta, ep10_inta, ep11_inta;
wire		ep12_inta, ep13_inta, ep14_inta, ep15_inta;

wire		ep0_intb, ep1_intb, ep2_intb, ep3_intb;
wire		ep4_intb, ep5_intb, ep6_intb, ep7_intb;
wire		ep8_intb, ep9_intb, ep10_intb, ep11_intb;
wire		ep12_intb, ep13_intb, ep14_intb, ep15_intb;

wire		ep0_match, ep1_match, ep2_match, ep3_match;
wire		ep4_match, ep5_match, ep6_match, ep7_match;
wire		ep8_match, ep9_match, ep10_match, ep11_match;
wire		ep12_match, ep13_match, ep14_match, ep15_match;

wire	[31:0]	ep0_csr, ep1_csr, ep2_csr, ep3_csr;
wire	[31:0]	ep4_csr, ep5_csr, ep6_csr, ep7_csr;
wire	[31:0]	ep8_csr, ep9_csr, ep10_csr, ep11_csr;
wire	[31:0]	ep12_csr, ep13_csr, ep14_csr, ep15_csr;

wire	[31:0]	ep0_buf0, ep1_buf0, ep2_buf0, ep3_buf0;
wire	[31:0]	ep4_buf0, ep5_buf0, ep6_buf0, ep7_buf0;
wire	[31:0]	ep8_buf0, ep9_buf0, ep10_buf0, ep11_buf0;
wire	[31:0]	ep12_buf0, ep13_buf0, ep14_buf0, ep15_buf0;

wire	[31:0]	ep0_buf1, ep1_buf1, ep2_buf1, ep3_buf1;
wire	[31:0]	ep4_buf1, ep5_buf1, ep6_buf1, ep7_buf1;
wire	[31:0]	ep8_buf1, ep9_buf1, ep10_buf1, ep11_buf1;
wire	[31:0]	ep12_buf1, ep13_buf1, ep14_buf1, ep15_buf1;

wire		ep0_dma_in_buf_sz1, ep1_dma_in_buf_sz1;
wire		ep2_dma_in_buf_sz1, ep3_dma_in_buf_sz1;
wire		ep4_dma_in_buf_sz1, ep5_dma_in_buf_sz1;
wire		ep6_dma_in_buf_sz1, ep7_dma_in_buf_sz1;
wire		ep8_dma_in_buf_sz1, ep9_dma_in_buf_sz1;
wire		ep10_dma_in_buf_sz1, ep11_dma_in_buf_sz1;
wire		ep12_dma_in_buf_sz1, ep13_dma_in_buf_sz1;
wire		ep14_dma_in_buf_sz1, ep15_dma_in_buf_sz1;

wire		ep0_dma_out_buf_avail, ep1_dma_out_buf_avail;
wire		ep2_dma_out_buf_avail, ep3_dma_out_buf_avail;
wire		ep4_dma_out_buf_avail, ep5_dma_out_buf_avail;
wire		ep6_dma_out_buf_avail, ep7_dma_out_buf_avail;
wire		ep8_dma_out_buf_avail, ep9_dma_out_buf_avail;
wire		ep10_dma_out_buf_avail, ep11_dma_out_buf_avail;
wire		ep12_dma_out_buf_avail, ep13_dma_out_buf_avail;
wire		ep14_dma_out_buf_avail, ep15_dma_out_buf_avail;

reg		dma_in_buf_sz1;
reg		dma_out_buf_avail;

reg	[31:0]	dtmp;
reg	[31:0]	dout;

wire	[31:0]	main_csr;
reg	[6:0]	funct_adr;
reg	[8:0]	intb_msk, inta_msk;

reg		match_r1;
reg	[31:0]	csr;
reg	[31:0]	buf0;
reg	[31:0]	buf1;

reg	[3:0]	utmi_vend_ctrl;
reg		utmi_vend_wr;
reg	[7:0]	utmi_vend_stat_r;

reg		int_src_re;
reg	[8:0]	int_srcb;
reg	[15:0]	int_srca;
reg		attach_r, attach_r1;
wire		attach, deattach;
reg		suspend_r, suspend_r1;
wire		suspend_start, suspend_end;
reg		usb_reset_r;
reg		rx_err_r;
reg		nse_err_r;
reg		pid_cs_err_r;
reg		crc5_err_r;

reg		rf_resume_req_r, rf_resume_req;

wire		inta_ep, intb_ep;
wire		inta_rf, intb_rf;
reg		inta, intb;

///////////////////////////////////////////////////////////////////
//
// WISHBONE Access
//

// Main CSR Alias
assign main_csr = {27'h0, line_stat, usb_attached, mode_hs, suspend};

// Read Registers Logic
always @(adr or main_csr or funct_adr or inta_msk or intb_msk or int_srca
	or int_srcb or frm_nat or utmi_vend_stat_r)
	case(adr[2:0])		// synopsys full_case parallel_case
	   3'h0: dtmp = main_csr;
	   3'h1: dtmp = { 25'h0, funct_adr};
	   3'h2: dtmp = {  7'h0, intb_msk, 7'h0, inta_msk};
	   3'h3: dtmp = {  3'h0, int_srcb, 4'h0, int_srca};
	   3'h4: dtmp = frm_nat;
	   3'h5: dtmp = { 24'h0, utmi_vend_stat_r};
	endcase

// Interrupt Source Read Register
always @(posedge wclk)
	int_src_re <= adr[6:0] == 7'h3 & re;

// UTMI Vendor Control Stuff
always @(posedge wclk)
	utmi_vend_stat_r <= utmi_vend_stat;

reg		utmi_vend_wr_r;
`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)			utmi_vend_wr_r <= 1'b0;
	else
	if(adr[6:0] == 7'h5 && we)	utmi_vend_wr_r <= 1'b1;	
	else
	if(utmi_vend_wr)		utmi_vend_wr_r <= 1'b0;

always @(posedge clk)	// Second Stage sync
	utmi_vend_wr <= utmi_vend_wr_r;


reg	[3:0]	utmi_vend_ctrl_r;
always @(posedge wclk)
	if(adr[6:0] == 7'h5 && we)	utmi_vend_ctrl_r <= din[3:0];

always @(posedge clk)	// Second Stage sync
	utmi_vend_ctrl <= utmi_vend_ctrl_r;

// Resume Request
`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)			rf_resume_req_r <= 1'b0;
	else
	if(adr[6:0] == 7'h0 && we)	rf_resume_req_r <= din[5];
	else
	if(rf_resume_req)		rf_resume_req_r <= 1'b0;

always @(posedge clk)	// Second Stage sync
	rf_resume_req <= rf_resume_req_r;

// Function Address Register
`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)			funct_adr <= 7'h0;
	else
	if(adr[6:0] == 7'h1 && we)	funct_adr <= din[6:0];

// Interrup Mask Register
`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)
	   begin
		inta_msk <= 9'h0;
		intb_msk <= 9'h0;
	   end
	else
	if(adr[6:0] == 7'h2 && we)
	   begin
		intb_msk <= din[24:16];
		inta_msk <= din[08:00];
	   end

always @(posedge wclk)
	case(adr[6:2])		// synopsys full_case parallel_case
	 5'h00: dout <= dtmp;		// Addr: 0h
	 5'h01: dout <= dtmp;
	 5'h02: dout <= 32'h0;
	 5'h03: dout <= 32'h0;
	 5'h04: dout <= ep0_dout;
	 5'h05: dout <= ep1_dout;
	 5'h06: dout <= ep2_dout;
	 5'h07: dout <= ep3_dout;
	 5'h08: dout <= ep4_dout;
	 5'h09: dout <= ep5_dout;
	 5'h0a: dout <= ep6_dout;
	 5'h0b: dout <= ep7_dout;
	 5'h0c: dout <= ep8_dout;
	 5'h0d: dout <= ep9_dout;
	 5'h0e: dout <= ep10_dout;
	 5'h0f: dout <= ep11_dout;
	 5'h10: dout <= ep12_dout;
	 5'h11: dout <= ep13_dout;
	 5'h12: dout <= ep14_dout;
	 5'h13: dout <= ep15_dout;
	endcase

assign ep0_re  = (adr[6:2] == 5'h04) & re;
assign ep1_re  = (adr[6:2] == 5'h05) & re;
assign ep2_re  = (adr[6:2] == 5'h06) & re;
assign ep3_re  = (adr[6:2] == 5'h07) & re;
assign ep4_re  = (adr[6:2] == 5'h08) & re;
assign ep5_re  = (adr[6:2] == 5'h09) & re;
assign ep6_re  = (adr[6:2] == 5'h0a) & re;
assign ep7_re  = (adr[6:2] == 5'h0b) & re;
assign ep8_re  = (adr[6:2] == 5'h0c) & re;
assign ep9_re  = (adr[6:2] == 5'h0d) & re;
assign ep10_re = (adr[6:2] == 5'h0e) & re;
assign ep11_re = (adr[6:2] == 5'h0f) & re;
assign ep12_re = (adr[6:2] == 5'h10) & re;
assign ep13_re = (adr[6:2] == 5'h11) & re;
assign ep14_re = (adr[6:2] == 5'h12) & re;
assign ep15_re = (adr[6:2] == 5'h13) & re;

assign ep0_we  = (adr[6:2] == 5'h04) & we;
assign ep1_we  = (adr[6:2] == 5'h05) & we;
assign ep2_we  = (adr[6:2] == 5'h06) & we;
assign ep3_we  = (adr[6:2] == 5'h07) & we;
assign ep4_we  = (adr[6:2] == 5'h08) & we;
assign ep5_we  = (adr[6:2] == 5'h09) & we;
assign ep6_we  = (adr[6:2] == 5'h0a) & we;
assign ep7_we  = (adr[6:2] == 5'h0b) & we;
assign ep8_we  = (adr[6:2] == 5'h0c) & we;
assign ep9_we  = (adr[6:2] == 5'h0d) & we;
assign ep10_we = (adr[6:2] == 5'h0e) & we;
assign ep11_we = (adr[6:2] == 5'h0f) & we;
assign ep12_we = (adr[6:2] == 5'h10) & we;
assign ep13_we = (adr[6:2] == 5'h11) & we;
assign ep14_we = (adr[6:2] == 5'h12) & we;
assign ep15_we = (adr[6:2] == 5'h13) & we;

///////////////////////////////////////////////////////////////////
//
// Internal Access
//

assign match = match_r1;

always @(posedge clk)
	match_r1 <= 	ep0_match | ep1_match | ep2_match | ep3_match |
			ep4_match | ep5_match | ep6_match | ep7_match |
			ep8_match | ep9_match | ep10_match | ep11_match |
			ep12_match | ep13_match | ep14_match | ep15_match;

always @(posedge clk)
	if(ep0_match)	csr <= ep0_csr;
	else
	if(ep1_match)	csr <= ep1_csr;
	else
	if(ep2_match)	csr <= ep2_csr;
	else
	if(ep3_match)	csr <= ep3_csr;
	else
	if(ep4_match)	csr <= ep4_csr;
	else
	if(ep5_match)	csr <= ep5_csr;
	else
	if(ep6_match)	csr <= ep6_csr;
	else
	if(ep7_match)	csr <= ep7_csr;
	else
	if(ep8_match)	csr <= ep8_csr;
	else
	if(ep9_match)	csr <= ep9_csr;
	else
	if(ep10_match)	csr <= ep10_csr;
	else
	if(ep11_match)	csr <= ep11_csr;
	else
	if(ep12_match)	csr <= ep12_csr;
	else
	if(ep13_match)	csr <= ep13_csr;
	else
	if(ep14_match)	csr <= ep14_csr;
	else
	if(ep15_match)	csr <= ep15_csr;

always @(posedge clk)
	if(ep0_match)	buf0 <= ep0_buf0;
	else
	if(ep1_match)	buf0 <= ep1_buf0;
	else
	if(ep2_match)	buf0 <= ep2_buf0;
	else
	if(ep3_match)	buf0 <= ep3_buf0;
	else
	if(ep4_match)	buf0 <= ep4_buf0;
	else
	if(ep5_match)	buf0 <= ep5_buf0;
	else
	if(ep6_match)	buf0 <= ep6_buf0;
	else
	if(ep7_match)	buf0 <= ep7_buf0;
	else
	if(ep8_match)	buf0 <= ep8_buf0;
	else
	if(ep9_match)	buf0 <= ep9_buf0;
	else
	if(ep10_match)	buf0 <= ep10_buf0;
	else
	if(ep11_match)	buf0 <= ep11_buf0;
	else
	if(ep12_match)	buf0 <= ep12_buf0;
	else
	if(ep13_match)	buf0 <= ep13_buf0;
	else
	if(ep14_match)	buf0 <= ep14_buf0;
	else
	if(ep15_match)	buf0 <= ep15_buf0;

always @(posedge clk)
	if(ep0_match)	buf1 <= ep0_buf1;
	else
	if(ep1_match)	buf1 <= ep1_buf1;
	else
	if(ep2_match)	buf1 <= ep2_buf1;
	else
	if(ep3_match)	buf1 <= ep3_buf1;
	else
	if(ep4_match)	buf1 <= ep4_buf1;
	else
	if(ep5_match)	buf1 <= ep5_buf1;
	else
	if(ep6_match)	buf1 <= ep6_buf1;
	else
	if(ep7_match)	buf1 <= ep7_buf1;
	else
	if(ep8_match)	buf1 <= ep8_buf1;
	else
	if(ep9_match)	buf1 <= ep9_buf1;
	else
	if(ep10_match)	buf1 <= ep10_buf1;
	else
	if(ep11_match)	buf1 <= ep11_buf1;
	else
	if(ep12_match)	buf1 <= ep12_buf1;
	else
	if(ep13_match)	buf1 <= ep13_buf1;
	else
	if(ep14_match)	buf1 <= ep14_buf1;
	else
	if(ep15_match)	buf1 <= ep15_buf1;

always @(posedge clk)
	if(ep0_match)	dma_in_buf_sz1 <= ep0_dma_in_buf_sz1;
	else
	if(ep1_match)	dma_in_buf_sz1 <= ep1_dma_in_buf_sz1;
	else
	if(ep2_match)	dma_in_buf_sz1 <= ep2_dma_in_buf_sz1;
	else
	if(ep3_match)	dma_in_buf_sz1 <= ep3_dma_in_buf_sz1;
	else
	if(ep4_match)	dma_in_buf_sz1 <= ep4_dma_in_buf_sz1;
	else
	if(ep5_match)	dma_in_buf_sz1 <= ep5_dma_in_buf_sz1;
	else
	if(ep6_match)	dma_in_buf_sz1 <= ep6_dma_in_buf_sz1;
	else
	if(ep7_match)	dma_in_buf_sz1 <= ep7_dma_in_buf_sz1;
	else
	if(ep8_match)	dma_in_buf_sz1 <= ep8_dma_in_buf_sz1;
	else
	if(ep9_match)	dma_in_buf_sz1 <= ep9_dma_in_buf_sz1;
	else
	if(ep10_match)	dma_in_buf_sz1 <= ep10_dma_in_buf_sz1;
	else
	if(ep11_match)	dma_in_buf_sz1 <= ep11_dma_in_buf_sz1;
	else
	if(ep12_match)	dma_in_buf_sz1 <= ep12_dma_in_buf_sz1;
	else
	if(ep13_match)	dma_in_buf_sz1 <= ep13_dma_in_buf_sz1;
	else
	if(ep14_match)	dma_in_buf_sz1 <= ep14_dma_in_buf_sz1;
	else
	if(ep15_match)	dma_in_buf_sz1 <= ep15_dma_in_buf_sz1;

always @(posedge clk)
	if(ep0_match)	dma_out_buf_avail <= ep0_dma_out_buf_avail;
	else
	if(ep1_match)	dma_out_buf_avail <= ep1_dma_out_buf_avail;
	else
	if(ep2_match)	dma_out_buf_avail <= ep2_dma_out_buf_avail;
	else
	if(ep3_match)	dma_out_buf_avail <= ep3_dma_out_buf_avail;
	else
	if(ep4_match)	dma_out_buf_avail <= ep4_dma_out_buf_avail;
	else
	if(ep5_match)	dma_out_buf_avail <= ep5_dma_out_buf_avail;
	else
	if(ep6_match)	dma_out_buf_avail <= ep6_dma_out_buf_avail;
	else
	if(ep7_match)	dma_out_buf_avail <= ep7_dma_out_buf_avail;
	else
	if(ep8_match)	dma_out_buf_avail <= ep8_dma_out_buf_avail;
	else
	if(ep9_match)	dma_out_buf_avail <= ep9_dma_out_buf_avail;
	else
	if(ep10_match)	dma_out_buf_avail <= ep10_dma_out_buf_avail;
	else
	if(ep11_match)	dma_out_buf_avail <= ep11_dma_out_buf_avail;
	else
	if(ep12_match)	dma_out_buf_avail <= ep12_dma_out_buf_avail;
	else
	if(ep13_match)	dma_out_buf_avail <= ep13_dma_out_buf_avail;
	else
	if(ep14_match)	dma_out_buf_avail <= ep14_dma_out_buf_avail;
	else
	if(ep15_match)	dma_out_buf_avail <= ep15_dma_out_buf_avail;


///////////////////////////////////////////////////////////////////
//
// Interrupt Generation
//

always @(posedge wclk)
	attach_r <= usb_attached;

always @(posedge wclk)
	attach_r1 <= attach_r;

always @(posedge wclk)
	suspend_r <= suspend;

always @(posedge wclk)
	suspend_r1 <= suspend_r;

always @(posedge wclk)
	usb_reset_r <= usb_reset;

always @(posedge wclk)
	rx_err_r <= rx_err;

always @(posedge wclk)
	nse_err_r <= nse_err;

always @(posedge wclk)
	pid_cs_err_r <= pid_cs_err;

always @(posedge wclk)
	crc5_err_r <= crc5_err;

assign	attach = !attach_r1 & attach_r;
assign	deattach = attach_r1 & !attach_r;
assign	suspend_start = !suspend_r1 & suspend_r;
assign	suspend_end   = suspend_r1 & !suspend_r;

`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)		int_srcb[8] <= 1'b0;
	else
	if(int_src_re)		int_srcb[8] <= 1'b0;
	else
	if(usb_reset_r)		int_srcb[8] <= 1'b1;

`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)		int_srcb[7] <= 1'b0;
	else
	if(int_src_re)		int_srcb[7] <= 1'b0;
	else
	if(rx_err_r)		int_srcb[7] <= 1'b1;

`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)		int_srcb[6] <= 1'b0;
	else
	if(int_src_re)		int_srcb[6] <= 1'b0;
	else
	if(deattach)		int_srcb[6] <= 1'b1;

`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)		int_srcb[5] <= 1'b0;
	else
	if(int_src_re)		int_srcb[5] <= 1'b0;
	else
	if(attach)		int_srcb[5] <= 1'b1;

`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)		int_srcb[4] <= 1'b0;
	else
	if(int_src_re)		int_srcb[4] <= 1'b0;
	else
	if(suspend_end)		int_srcb[4] <= 1'b1;

`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)		int_srcb[3] <= 1'b0;
	else
	if(int_src_re)		int_srcb[3] <= 1'b0;
	else
	if(suspend_start)	int_srcb[3] <= 1'b1;

`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)		int_srcb[2] <= 1'b0;
	else
	if(int_src_re)		int_srcb[2] <= 1'b0;
	else
	if(nse_err_r)		int_srcb[2] <= 1'b1;

`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)		int_srcb[1] <= 1'b0;
	else
	if(int_src_re)		int_srcb[1] <= 1'b0;
	else
	if(pid_cs_err_r)	int_srcb[1] <= 1'b1;

`ifdef USBF_ASYNC_RESET
always @(posedge wclk or negedge rst)
`else
always @(posedge wclk)
`endif
	if(!rst)		int_srcb[0] <= 1'b0;
	else
	if(int_src_re)		int_srcb[0] <= 1'b0;
	else
	if(crc5_err_r)		int_srcb[0] <= 1'b1;

always @(posedge wclk)
   begin
	int_srca[15] <= ep15_inta | ep15_intb;
	int_srca[14] <= ep14_inta | ep14_intb;
	int_srca[13] <= ep13_inta | ep13_intb;
	int_srca[12] <= ep12_inta | ep12_intb;
	int_srca[11] <= ep11_inta | ep11_intb;
	int_srca[10] <= ep10_inta | ep10_intb;
	int_srca[09] <= ep9_inta | ep9_intb;
	int_srca[08] <= ep8_inta | ep8_intb;
	int_srca[07] <= ep7_inta | ep7_intb;
	int_srca[06] <= ep6_inta | ep6_intb;
	int_srca[05] <= ep5_inta | ep5_intb;
	int_srca[04] <= ep4_inta | ep4_intb;
	int_srca[03] <= ep3_inta | ep3_intb;
	int_srca[02] <= ep2_inta | ep2_intb;
	int_srca[01] <= ep1_inta | ep1_intb;
	int_srca[00] <= ep0_inta | ep0_intb;
   end

assign inta_ep =ep0_inta  | ep1_inta  | ep2_inta  | ep3_inta  |
		ep4_inta  | ep5_inta  | ep6_inta  | ep7_inta  |
		ep8_inta  | ep9_inta  | ep10_inta | ep11_inta |
		ep12_inta | ep13_inta | ep14_inta | ep15_inta;

assign intb_ep =ep0_intb  | ep1_intb  | ep2_intb  | ep3_intb  |
		ep4_intb  | ep5_intb  | ep6_intb  | ep7_intb  |
		ep8_intb  | ep9_intb  | ep10_intb | ep11_intb |
		ep12_intb | ep13_intb | ep14_intb | ep15_intb;

assign inta_rf = |(int_srcb & inta_msk);
assign intb_rf = |(int_srcb & intb_msk);

always @(posedge wclk)
	inta <= inta_ep | inta_rf;

always @(posedge wclk)
	intb <= intb_ep | intb_rf;

///////////////////////////////////////////////////////////////////
//
// Endpoint Register Files
//

usbf_ep_rf	u0(
		.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep0_re			),
		.we(			ep0_we			),
		.din(			din			),
		.dout(			ep0_dout		),
		.inta(			ep0_inta		),
		.intb(			ep0_intb		),
		.dma_req(		dma_req[0]		),
		.dma_ack(		dma_ack[0]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep0_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep0_csr			),
		.buf0(			ep0_buf0		),
		.buf1(			ep0_buf1		),
		.dma_in_buf_sz1(	ep0_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep0_dma_out_buf_avail	)
		);

`ifdef USBF_HAVE_EP1
usbf_ep_rf	u1(
		.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep1_re			),
		.we(			ep1_we			),
		.din(			din			),
		.dout(			ep1_dout		),
		.inta(			ep1_inta		),
		.intb(			ep1_intb		),
		.dma_req(		dma_req[1]		),
		.dma_ack(		dma_ack[1]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep1_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep1_csr			),
		.buf0(			ep1_buf0		),
		.buf1(			ep1_buf1		),
		.dma_in_buf_sz1(	ep1_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep1_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u1(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep1_re			),
		.we(			ep1_we			),
		.din(			din			),
		.dout(			ep1_dout		),
		.inta(			ep1_inta		),
		.intb(			ep1_intb		),
		.dma_req(		dma_req[1]		),
		.dma_ack(		dma_ack[1]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep1_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep1_csr			),
		.buf0(			ep1_buf0		),
		.buf1(			ep1_buf1		),
		.dma_in_buf_sz1(	ep1_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep1_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP2
usbf_ep_rf	u2(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep2_re			),
		.we(			ep2_we			),
		.din(			din			),
		.dout(			ep2_dout		),
		.inta(			ep2_inta		),
		.intb(			ep2_intb		),
		.dma_req(		dma_req[2]		),
		.dma_ack(		dma_ack[2]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep2_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep2_csr			),
		.buf0(			ep2_buf0		),
		.buf1(			ep2_buf1		),
		.dma_in_buf_sz1(	ep2_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep2_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u2(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep2_re			),
		.we(			ep2_we			),
		.din(			din			),
		.dout(			ep2_dout		),
		.inta(			ep2_inta		),
		.intb(			ep2_intb		),
		.dma_req(		dma_req[2]		),
		.dma_ack(		dma_ack[2]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep2_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep2_csr			),
		.buf0(			ep2_buf0		),
		.buf1(			ep2_buf1		),
		.dma_in_buf_sz1(	ep2_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep2_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP3
usbf_ep_rf	u3(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep3_re			),
		.we(			ep3_we			),
		.din(			din			),
		.dout(			ep3_dout		),
		.inta(			ep3_inta		),
		.intb(			ep3_intb		),
		.dma_req(		dma_req[3]		),
		.dma_ack(		dma_ack[3]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep3_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep3_csr			),
		.buf0(			ep3_buf0		),
		.buf1(			ep3_buf1		),
		.dma_in_buf_sz1(	ep3_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep3_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u3(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep3_re			),
		.we(			ep3_we			),
		.din(			din			),
		.dout(			ep3_dout		),
		.inta(			ep3_inta		),
		.intb(			ep3_intb		),
		.dma_req(		dma_req[3]		),
		.dma_ack(		dma_ack[3]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep3_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep3_csr			),
		.buf0(			ep3_buf0		),
		.buf1(			ep3_buf1		),
		.dma_in_buf_sz1(	ep3_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep3_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP4
usbf_ep_rf	u4(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep4_re			),
		.we(			ep4_we			),
		.din(			din			),
		.dout(			ep4_dout		),
		.inta(			ep4_inta		),
		.intb(			ep4_intb		),
		.dma_req(		dma_req[4]		),
		.dma_ack(		dma_ack[4]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep4_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep4_csr			),
		.buf0(			ep4_buf0		),
		.buf1(			ep4_buf1		),
		.dma_in_buf_sz1(	ep4_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep4_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u4(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep4_re			),
		.we(			ep4_we			),
		.din(			din			),
		.dout(			ep4_dout		),
		.inta(			ep4_inta		),
		.intb(			ep4_intb		),
		.dma_req(		dma_req[4]		),
		.dma_ack(		dma_ack[4]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep4_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep4_csr			),
		.buf0(			ep4_buf0		),
		.buf1(			ep4_buf1		),
		.dma_in_buf_sz1(	ep4_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep4_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP5
usbf_ep_rf	u5(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep5_re			),
		.we(			ep5_we			),
		.din(			din			),
		.dout(			ep5_dout		),
		.inta(			ep5_inta		),
		.intb(			ep5_intb		),
		.dma_req(		dma_req[5]		),
		.dma_ack(		dma_ack[5]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep5_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep5_csr			),
		.buf0(			ep5_buf0		),
		.buf1(			ep5_buf1		),
		.dma_in_buf_sz1(	ep5_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep5_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u5(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep5_re			),
		.we(			ep5_we			),
		.din(			din			),
		.dout(			ep5_dout		),
		.inta(			ep5_inta		),
		.intb(			ep5_intb		),
		.dma_req(		dma_req[5]		),
		.dma_ack(		dma_ack[5]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep5_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep5_csr			),
		.buf0(			ep5_buf0		),
		.buf1(			ep5_buf1		),
		.dma_in_buf_sz1(	ep5_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep5_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP6
usbf_ep_rf	u6(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep6_re			),
		.we(			ep6_we			),
		.din(			din			),
		.dout(			ep6_dout		),
		.inta(			ep6_inta		),
		.intb(			ep6_intb		),
		.dma_req(		dma_req[6]		),
		.dma_ack(		dma_ack[6]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep6_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep6_csr			),
		.buf0(			ep6_buf0		),
		.buf1(			ep6_buf1		),
		.dma_in_buf_sz1(	ep6_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep6_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u6(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep6_re			),
		.we(			ep6_we			),
		.din(			din			),
		.dout(			ep6_dout		),
		.inta(			ep6_inta		),
		.intb(			ep6_intb		),
		.dma_req(		dma_req[6]		),
		.dma_ack(		dma_ack[6]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep6_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep6_csr			),
		.buf0(			ep6_buf0		),
		.buf1(			ep6_buf1		),
		.dma_in_buf_sz1(	ep6_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep6_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP7
usbf_ep_rf	u7(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep7_re			),
		.we(			ep7_we			),
		.din(			din			),
		.dout(			ep7_dout		),
		.inta(			ep7_inta		),
		.intb(			ep7_intb		),
		.dma_req(		dma_req[7]		),
		.dma_ack(		dma_ack[7]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep7_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep7_csr			),
		.buf0(			ep7_buf0		),
		.buf1(			ep7_buf1		),
		.dma_in_buf_sz1(	ep7_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep7_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u7(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep7_re			),
		.we(			ep7_we			),
		.din(			din			),
		.dout(			ep7_dout		),
		.inta(			ep7_inta		),
		.intb(			ep7_intb		),
		.dma_req(		dma_req[7]		),
		.dma_ack(		dma_ack[7]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep7_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep7_csr			),
		.buf0(			ep7_buf0		),
		.buf1(			ep7_buf1		),
		.dma_in_buf_sz1(	ep7_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep7_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP8
usbf_ep_rf	u8(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep8_re			),
		.we(			ep8_we			),
		.din(			din			),
		.dout(			ep8_dout		),
		.inta(			ep8_inta		),
		.intb(			ep8_intb		),
		.dma_req(		dma_req[8]		),
		.dma_ack(		dma_ack[8]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep8_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep8_csr			),
		.buf0(			ep8_buf0		),
		.buf1(			ep8_buf1		),
		.dma_in_buf_sz1(	ep8_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep8_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u8(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep8_re			),
		.we(			ep8_we			),
		.din(			din			),
		.dout(			ep8_dout		),
		.inta(			ep8_inta		),
		.intb(			ep8_intb		),
		.dma_req(		dma_req[8]		),
		.dma_ack(		dma_ack[8]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep8_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep8_csr			),
		.buf0(			ep8_buf0		),
		.buf1(			ep8_buf1		),
		.dma_in_buf_sz1(	ep8_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep8_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP9
usbf_ep_rf	u9(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep9_re			),
		.we(			ep9_we			),
		.din(			din			),
		.dout(			ep9_dout		),
		.inta(			ep9_inta		),
		.intb(			ep9_intb		),
		.dma_req(		dma_req[9]		),
		.dma_ack(		dma_ack[9]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep9_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep9_csr			),
		.buf0(			ep9_buf0		),
		.buf1(			ep9_buf1		),
		.dma_in_buf_sz1(	ep9_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep9_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u9(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep9_re			),
		.we(			ep9_we			),
		.din(			din			),
		.dout(			ep9_dout		),
		.inta(			ep9_inta		),
		.intb(			ep9_intb		),
		.dma_req(		dma_req[9]		),
		.dma_ack(		dma_ack[9]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep9_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep9_csr			),
		.buf0(			ep9_buf0		),
		.buf1(			ep9_buf1		),
		.dma_in_buf_sz1(	ep9_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep9_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP10
usbf_ep_rf	u10(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep10_re			),
		.we(			ep10_we			),
		.din(			din			),
		.dout(			ep10_dout		),
		.inta(			ep10_inta		),
		.intb(			ep10_intb		),
		.dma_req(		dma_req[10]		),
		.dma_ack(		dma_ack[10]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep10_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep10_csr			),
		.buf0(			ep10_buf0		),
		.buf1(			ep10_buf1		),
		.dma_in_buf_sz1(	ep10_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep10_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u10(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep10_re			),
		.we(			ep10_we			),
		.din(			din			),
		.dout(			ep10_dout		),
		.inta(			ep10_inta		),
		.intb(			ep10_intb		),
		.dma_req(		dma_req[10]		),
		.dma_ack(		dma_ack[10]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep10_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep10_csr			),
		.buf0(			ep10_buf0		),
		.buf1(			ep10_buf1		),
		.dma_in_buf_sz1(	ep10_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep10_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP11
usbf_ep_rf	u11(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep11_re			),
		.we(			ep11_we			),
		.din(			din			),
		.dout(			ep11_dout		),
		.inta(			ep11_inta		),
		.intb(			ep11_intb		),
		.dma_req(		dma_req[11]		),
		.dma_ack(		dma_ack[11]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep11_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep11_csr			),
		.buf0(			ep11_buf0		),
		.buf1(			ep11_buf1		),
		.dma_in_buf_sz1(	ep11_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep11_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u11(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep11_re			),
		.we(			ep11_we			),
		.din(			din			),
		.dout(			ep11_dout		),
		.inta(			ep11_inta		),
		.intb(			ep11_intb		),
		.dma_req(		dma_req[11]		),
		.dma_ack(		dma_ack[11]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep11_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep11_csr			),
		.buf0(			ep11_buf0		),
		.buf1(			ep11_buf1		),
		.dma_in_buf_sz1(	ep11_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep11_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP12
usbf_ep_rf	u12(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep12_re			),
		.we(			ep12_we			),
		.din(			din			),
		.dout(			ep12_dout		),
		.inta(			ep12_inta		),
		.intb(			ep12_intb		),
		.dma_req(		dma_req[12]		),
		.dma_ack(		dma_ack[12]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep12_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep12_csr			),
		.buf0(			ep12_buf0		),
		.buf1(			ep12_buf1		),
		.dma_in_buf_sz1(	ep12_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep12_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u12(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep12_re			),
		.we(			ep12_we			),
		.din(			din			),
		.dout(			ep12_dout		),
		.inta(			ep12_inta		),
		.intb(			ep12_intb		),
		.dma_req(		dma_req[12]		),
		.dma_ack(		dma_ack[12]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep12_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep12_csr			),
		.buf0(			ep12_buf0		),
		.buf1(			ep12_buf1		),
		.dma_in_buf_sz1(	ep12_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep12_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP13
usbf_ep_rf	u13(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep13_re			),
		.we(			ep13_we			),
		.din(			din			),
		.dout(			ep13_dout		),
		.inta(			ep13_inta		),
		.intb(			ep13_intb		),
		.dma_req(		dma_req[13]		),
		.dma_ack(		dma_ack[13]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep13_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep13_csr			),
		.buf0(			ep13_buf0		),
		.buf1(			ep13_buf1		),
		.dma_in_buf_sz1(	ep13_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep13_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u13(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep13_re			),
		.we(			ep13_we			),
		.din(			din			),
		.dout(			ep13_dout		),
		.inta(			ep13_inta		),
		.intb(			ep13_intb		),
		.dma_req(		dma_req[13]		),
		.dma_ack(		dma_ack[13]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep13_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep13_csr			),
		.buf0(			ep13_buf0		),
		.buf1(			ep13_buf1		),
		.dma_in_buf_sz1(	ep13_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep13_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP14
usbf_ep_rf	u14(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep14_re			),
		.we(			ep14_we			),
		.din(			din			),
		.dout(			ep14_dout		),
		.inta(			ep14_inta		),
		.intb(			ep14_intb		),
		.dma_req(		dma_req[14]		),
		.dma_ack(		dma_ack[14]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep14_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep14_csr			),
		.buf0(			ep14_buf0		),
		.buf1(			ep14_buf1		),
		.dma_in_buf_sz1(	ep14_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep14_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u14(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep14_re			),
		.we(			ep14_we			),
		.din(			din			),
		.dout(			ep14_dout		),
		.inta(			ep14_inta		),
		.intb(			ep14_intb		),
		.dma_req(		dma_req[14]		),
		.dma_ack(		dma_ack[14]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep14_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep14_csr			),
		.buf0(			ep14_buf0		),
		.buf1(			ep14_buf1		),
		.dma_in_buf_sz1(	ep14_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep14_dma_out_buf_avail	)
		);
`endif

`ifdef USBF_HAVE_EP15
usbf_ep_rf	u15(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep15_re			),
		.we(			ep15_we			),
		.din(			din			),
		.dout(			ep15_dout		),
		.inta(			ep15_inta		),
		.intb(			ep15_intb		),
		.dma_req(		dma_req[15]		),
		.dma_ack(		dma_ack[15]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep15_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep15_csr			),
		.buf0(			ep15_buf0		),
		.buf1(			ep15_buf1		),
		.dma_in_buf_sz1(	ep15_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep15_dma_out_buf_avail	)
		);

`else
usbf_ep_rf_dummy u15(	.clk(			clk			),
		.wclk(			wclk			),
		.rst(			rst			),
		.adr(			adr[1:0]		),
		.re(			ep15_re			),
		.we(			ep15_we			),
		.din(			din			),
		.dout(			ep15_dout		),
		.inta(			ep15_inta		),
		.intb(			ep15_intb		),
		.dma_req(		dma_req[15]		),
		.dma_ack(		dma_ack[15]		),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.ep_match(		ep15_match		),
		.buf0_rl(		buf0_rl			),
		.buf0_set(		buf0_set		),
		.buf1_set(		buf1_set		),
		.uc_bsel_set(		uc_bsel_set		),
		.uc_dpd_set(		uc_dpd_set		),
		.int_buf1_set(		int_buf1_set		),
		.int_buf0_set(		int_buf0_set		),
		.int_upid_set(		int_upid_set		),
		.int_crc16_set(		int_crc16_set		),
		.int_to_set(		int_to_set		),
		.int_seqerr_set(	int_seqerr_set		),
		.out_to_small(		out_to_small		),
		.csr(			ep15_csr			),
		.buf0(			ep15_buf0		),
		.buf1(			ep15_buf1		),
		.dma_in_buf_sz1(	ep15_dma_in_buf_sz1	),
		.dma_out_buf_avail(	ep15_dma_out_buf_avail	)
		);
`endif

endmodule

