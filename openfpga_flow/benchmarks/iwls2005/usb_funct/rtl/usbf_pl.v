/////////////////////////////////////////////////////////////////////
////                                                             ////
////  Protocol Layer                                             ////
////  This block is typically referred to as the SEI in USB      ////
////  Specification. It encapsulates the Packet Assembler,       ////
////  disassembler, protocol engine and internal DMA             ////
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
//  $Id: usbf_pl.v,v 1.5 2003/10/17 02:36:57 rudi Exp $
//
//  $Date: 2003/10/17 02:36:57 $
//  $Revision: 1.5 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: usbf_pl.v,v $
//               Revision 1.5  2003/10/17 02:36:57  rudi
//               - Disabling bit stuffing and NRZI encoding during speed negotiation
//               - Now the core can send zero size packets
//               - Fixed register addresses for some of the higher endpoints
//                 (conversion between decimal/hex was wrong)
//               - The core now does properly evaluate the function address to
//                 determine if the packet was intended for it.
//               - Various other minor bugs and typos
//
//               Revision 1.4  2001/11/04 12:22:45  rudi
//
//               - Fixed previous fix (brocke something else ...)
//               - Majore Synthesis cleanup
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
//               Revision 0.1.0.1  2001/02/28 08:11:11  rudi
//               Initial Release
//
//

`include "usbf_defines.v"

module usbf_pl(	clk, rst,

		// UTMI Interface
		rx_data, rx_valid, rx_active, rx_err,
		tx_data, tx_valid, tx_valid_last, tx_ready,
		tx_first, tx_valid_out,
		mode_hs, usb_reset, usb_suspend, usb_attached,

		// memory interface
		madr, mdout, mdin, mwe, mreq, mack,

		// Register File Interface
		fa, idin,
		ep_sel, match,
		dma_in_buf_sz1, dma_out_buf_avail,
		buf0_rl, buf0_set, buf1_set,
		uc_bsel_set, uc_dpd_set,

		int_buf1_set, int_buf0_set, int_upid_set,
		int_crc16_set, int_to_set, int_seqerr_set,
		out_to_small, csr, buf0, buf1,

		// Misc
		frm_nat,
		pid_cs_err, nse_err,
		crc5_err
		);

parameter	SSRAM_HADR = 14;

// UTMI Interface
input		clk, rst;
input	[7:0]	rx_data;
input		rx_valid, rx_active, rx_err;
output	[7:0]	tx_data;
output		tx_valid;
output		tx_valid_last;
input		tx_ready;
output		tx_first;
input		tx_valid_out;
input		mode_hs;	// High Speed Mode
input		usb_reset;	// USB Reset
input		usb_suspend;	// USB Suspend
input		usb_attached;	// Attached to USB

// Memory Arbiter Interface
output	[SSRAM_HADR:0]	madr;		// word address
output	[31:0]	mdout;
input	[31:0]	mdin;
output		mwe;
output		mreq;
input		mack;

// Register File interface
input	[6:0]	fa;		// Function Address (as set by the controller)
output	[31:0]	idin;		// Data Input
output	[3:0]	ep_sel;		// Endpoint Number Input
input		match;		// Endpoint Matched
input		dma_in_buf_sz1;
input		dma_out_buf_avail;
output		nse_err;	// no such endpoint error

output		buf0_rl;	// Reload Buf 0 with original values
output		buf0_set;	// Write to buf 0
output		buf1_set;	// Write to buf 1
output		uc_bsel_set;	// Write to the uc_bsel field
output		uc_dpd_set;	// Write to the uc_dpd field
output		int_buf1_set;	// Set buf1 full/empty interrupt
output		int_buf0_set;	// Set buf0 full/empty interrupt
output		int_upid_set;	// Set unsupported PID interrupt
output		int_crc16_set;	// Set CRC16 error interrupt
output		int_to_set;	// Set time out interrupt
output		int_seqerr_set;	// Set PID sequence error interrupt
output		out_to_small;	// OUT packet was to small for DMA operation

input	[31:0]	csr;		// Internal CSR Output
input	[31:0]	buf0;		// Internal Buf 0 Output
input	[31:0]	buf1;		// Internal Buf 1 Output

// Misc
output		pid_cs_err;	// pid checksum error
output		crc5_err;	// crc5 error
output	[31:0]	frm_nat;

///////////////////////////////////////////////////////////////////
//
// Local Wires and Registers
//

// Packet Disassembler Interface
wire		clk, rst;
wire	[7:0]	rx_data;
wire		pid_OUT, pid_IN, pid_SOF, pid_SETUP;
wire		pid_DATA0, pid_DATA1, pid_DATA2, pid_MDATA;
wire		pid_ACK, pid_NACK, pid_STALL, pid_NYET;
wire		pid_PRE, pid_ERR, pid_SPLIT, pid_PING;
wire	[6:0]	token_fadr;
wire		token_valid;
wire		crc5_err;
wire	[10:0]	frame_no;
wire	[7:0]	rx_data_st;
wire		rx_data_valid;
wire		rx_data_done;
wire		crc16_err;
wire		rx_seq_err;

// Packet Assembler Interface
wire		send_token;
wire	[1:0]	token_pid_sel;
wire		send_data;
wire	[1:0]	data_pid_sel;
wire	[7:0]	tx_data_st;
wire		rd_next;

// IDMA Interface
wire		rx_dma_en;	// Allows the data to be stored
wire		tx_dma_en;	// Allows for data to be retrieved
wire		abort;		// Abort Transfer (time_out, crc_err or rx_error)
wire		idma_done;	// DMA is done
wire	[SSRAM_HADR + 2:0]	adr;		// Byte Address
wire	[13:0]	size;		// Size in bytes
wire	[10:0]	sizu_c;		// Up and Down counting size registers, used
				// to update
wire	[13:0]	buf_size;	// Actual buffer size
wire		dma_en;		// external dma enabled

// Memory Arbiter Interface
wire	[SSRAM_HADR:0]	madr;	// word address
wire	[31:0]	mdout;
wire	[31:0]	mdin;
wire		mwe;
wire		mreq;
wire		mack;

// Local signals
wire		pid_bad, pid_bad1, pid_bad2;

reg		hms_clk;	// 0.5 Micro Second Clock
reg	[4:0]	hms_cnt;
reg	[10:0]	frame_no_r;	// Current Frame Number register
wire		frame_no_we;
reg		frame_no_same;	// Indicates current and prev. frame numbers
				// are equal
reg	[3:0]	mfm_cnt;	// Micro Frame Counter
reg	[11:0]	sof_time;	// Time since last sof
reg		clr_sof_time;
wire		fsel;		// This Function is selected
wire		match_o;

reg		frame_no_we_r;

///////////////////////////////////////////////////////////////////
//
// Misc Logic
//

// PIDs we should never receive
assign pid_bad1 = pid_ACK | pid_NACK | pid_STALL | pid_NYET | pid_PRE |
			pid_ERR | pid_SPLIT;

// PIDs we should never get in full speed mode (high speed mode only)
assign pid_bad2 = !mode_hs & pid_PING;

// All bad pids
assign pid_bad = pid_bad1 | pid_bad2;

assign match_o = !pid_bad & fsel & match & token_valid & !crc5_err;

// Frame Number (from SOF token)
assign frame_no_we = token_valid & !crc5_err & pid_SOF;

always @(posedge clk)
	frame_no_we_r <= frame_no_we;

`ifdef USBF_ASYNC_RESET
always @(posedge clk or negedge rst)
`else
always @(posedge clk)
`endif
	if(!rst)		frame_no_r <= 11'h0;
	else
	if(frame_no_we_r)	frame_no_r <= frame_no;

// Micro Frame Counter
always @(posedge clk)
	frame_no_same <= frame_no_we & (frame_no_r == frame_no);

`ifdef USBF_ASYNC_RESET
always @(posedge clk or negedge rst)
`else
always @(posedge clk)
`endif
	if(!rst)		mfm_cnt <= 4'h0;
	else
	if(frame_no_we_r && !frame_no_same)
				mfm_cnt <= 4'h0;
	else
	if(frame_no_same)	mfm_cnt <= mfm_cnt + 4'h1;

//SOF delay counter
always @(posedge clk)
	clr_sof_time <= frame_no_we;

always @(posedge clk)
	if(clr_sof_time)	sof_time <= 12'h0;
	else
	if(hms_clk)		sof_time <= sof_time + 12'h1;

assign frm_nat = {mfm_cnt, 1'b0, frame_no_r, 4'h0, sof_time};

// 0.5 Micro Seconds Clock Generator
`ifdef USBF_ASYNC_RESET
always @(posedge clk or negedge rst)
`else
always @(posedge clk)
`endif
	if(!rst)				hms_cnt <= 5'h0;
	else
	if(hms_clk || frame_no_we_r)		hms_cnt <= 5'h0;
	else					hms_cnt <= hms_cnt + 5'h1;

always @(posedge clk)
	hms_clk <= (hms_cnt == `USBF_HMS_DEL);

///////////////////////////////////////////////////////////////////

// This function is addressed
assign fsel = (token_fadr == fa);

///////////////////////////////////////////////////////////////////
//
// Module Instantiations
//

//Packet Decoder
usbf_pd	u0(	.clk(			clk			),
		.rst(			rst			),
		.rx_data(		rx_data			),
		.rx_valid(		rx_valid		),
		.rx_active(		rx_active		),
		.rx_err(		rx_err			),
		.pid_OUT(		pid_OUT			),
		.pid_IN(		pid_IN			),
		.pid_SOF(		pid_SOF			),
		.pid_SETUP(		pid_SETUP		),
		.pid_DATA0(		pid_DATA0		),
		.pid_DATA1(		pid_DATA1		),
		.pid_DATA2(		pid_DATA2		),
		.pid_MDATA(		pid_MDATA		),
		.pid_ACK(		pid_ACK			),
		.pid_NACK(		pid_NACK		),
		.pid_STALL(		pid_STALL		),
		.pid_NYET(		pid_NYET		),
		.pid_PRE(		pid_PRE			),
		.pid_ERR(		pid_ERR			),
		.pid_SPLIT(		pid_SPLIT		),
		.pid_PING(		pid_PING		),
		.pid_cks_err(		pid_cs_err		),
		.token_fadr(		token_fadr		),
		.token_endp(		ep_sel			),
		.token_valid(		token_valid		),
		.crc5_err(		crc5_err		),
		.frame_no(		frame_no		),
		.rx_data_st(		rx_data_st		),
		.rx_data_valid(		rx_data_valid		),
		.rx_data_done(		rx_data_done		),
		.crc16_err(		crc16_err		),
		.seq_err(		rx_seq_err		)
		);

// Packet Assembler
usbf_pa	u1(	.clk(			clk			),
		.rst(			rst			),
		.tx_data(		tx_data			),
		.tx_valid(		tx_valid		),
		.tx_valid_last(		tx_valid_last		),
		.tx_ready(		tx_ready		),
		.tx_first(		tx_first		),
		.send_token(		send_token		),
		.token_pid_sel(		token_pid_sel		),
		.send_data(		send_data		),
		.data_pid_sel(		data_pid_sel		),
		.send_zero_length(	send_zero_length	),
		.tx_data_st(		tx_data_st		),
		.rd_next(		rd_next			)
		);

// Internal DMA / Memory Arbiter Interface
usbf_idma #(SSRAM_HADR)
	u2(	.clk(			clk			),
		.rst(			rst			),
		.rx_data_st(		rx_data_st		),
		.rx_data_valid(		rx_data_valid		),
		.rx_data_done(		rx_data_done		),
		.send_data(		send_data		),
		.tx_data_st(		tx_data_st		),
		.rd_next(		rd_next			),
		.rx_dma_en(		rx_dma_en		),
		.tx_dma_en(		tx_dma_en		),
		.abort(			abort			),
		.idma_done(		idma_done		),
		.adr(			adr			),
		.size(			size			),
		.buf_size(		buf_size		),
		.dma_en(		dma_en			),
		.send_zero_length(	send_zero_length	),
		.madr(			madr			),
		.sizu_c(		sizu_c			),
		.mdout(			mdout			),
		.mdin(			mdin			),
		.mwe(			mwe			),
		.mreq(			mreq			),
		.mack(			mack			)
		);

// Protocol Engine
usbf_pe #(SSRAM_HADR)
	u3(	.clk(			clk			),
		.rst(			rst			),
		.tx_valid(		tx_valid_out		),
		.rx_active(		rx_active		),
		.pid_OUT(		pid_OUT			),
		.pid_IN(		pid_IN			),
		.pid_SOF(		pid_SOF			),
		.pid_SETUP(		pid_SETUP		),
		.pid_DATA0(		pid_DATA0		),
		.pid_DATA1(		pid_DATA1		),
		.pid_DATA2(		pid_DATA2		),
		.pid_MDATA(		pid_MDATA		),
		.pid_ACK(		pid_ACK			),
		.pid_NACK(		pid_NACK		),
		.pid_STALL(		pid_STALL		),
		.pid_NYET(		pid_NYET		),
		.pid_PRE(		pid_PRE			),
		.pid_ERR(		pid_ERR			),
		.pid_SPLIT(		pid_SPLIT		),
		.pid_PING(		pid_PING		),
		.mode_hs(		mode_hs			),
		.token_valid(		token_valid		),
		.crc5_err(		crc5_err		),
		.rx_data_valid(		rx_data_valid		),
		.rx_data_done(		rx_data_done		),
		.crc16_err(		crc16_err		),
		.send_token(		send_token		),
		.token_pid_sel(		token_pid_sel		),
		.data_pid_sel(		data_pid_sel		),
		.send_zero_length(	send_zero_length	),
		.rx_dma_en(		rx_dma_en		),
		.tx_dma_en(		tx_dma_en		),
		.abort(			abort			),
		.idma_done(		idma_done		),
		.adr(			adr			),
		.size(			size			),
		.buf_size(		buf_size		),
		.sizu_c(		sizu_c			),
		.dma_en(		dma_en			),
		.fsel(			fsel			),
		.idin(			idin			),
		.ep_sel(		ep_sel			),
		.match(			match_o			),
		.dma_in_buf_sz1(	dma_in_buf_sz1		),
		.dma_out_buf_avail(	dma_out_buf_avail	),
		.nse_err(		nse_err			),
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
		.csr(			csr			),
		.buf0(			buf0			),
		.buf1(			buf1			)
		);

endmodule

