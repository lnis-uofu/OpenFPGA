/////////////////////////////////////////////////////////////////////
////                                                             ////
////  Protocol Engine                                            ////
////  Performs automatic protocol functions                      ////
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
//  $Id: usbf_pe.v,v 1.8 2003/10/17 02:36:57 rudi Exp $
//
//  $Date: 2003/10/17 02:36:57 $
//  $Revision: 1.8 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: usbf_pe.v,v $
//               Revision 1.8  2003/10/17 02:36:57  rudi
//               - Disabling bit stuffing and NRZI encoding during speed negotiation
//               - Now the core can send zero size packets
//               - Fixed register addresses for some of the higher endpoints
//                 (conversion between decimal/hex was wrong)
//               - The core now does properly evaluate the function address to
//                 determine if the packet was intended for it.
//               - Various other minor bugs and typos
//
//               Revision 1.7  2001/11/04 12:22:45  rudi
//
//               - Fixed previous fix (brocke something else ...)
//               - Majore Synthesis cleanup
//
//               Revision 1.6  2001/11/03 03:26:22  rudi
//
//               - Fixed several interrupt and error condition reporting bugs
//
//               Revision 1.5  2001/09/24 01:15:28  rudi
//
//               Changed reset to be active high async.
//
//               Revision 1.4  2001/09/23 08:39:33  rudi
//
//               Renamed DEBUG and VERBOSE_DEBUG to USBF_DEBUG and USBF_VERBOSE_DEBUG ...
//
//               Revision 1.3  2001/09/13 13:14:02  rudi
//
//               Fixed a problem that would sometimes prevent the core to come out of
//               reset and immediately be operational ...
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
//               Revision 1.2  2001/03/31 13:00:51  rudi
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
//               Added USB control signaling (Line Status) block. Fixed some minor
//               typos, added resume bit and signal.
//
//               Revision 0.1.0.1  2001/02/28 08:11:07  rudi
//               Initial Release
//
//

`include "usbf_defines.v"

module usbf_pe(	clk, rst,

		// UTMI Interfaces
		tx_valid, rx_active,

		// PID Information
		pid_OUT, pid_IN, pid_SOF, pid_SETUP,
		pid_DATA0, pid_DATA1, pid_DATA2, pid_MDATA,
		pid_ACK, pid_NACK, pid_STALL, pid_NYET,
		pid_PRE, pid_ERR, pid_SPLIT, pid_PING,

		// Speed Mode
		mode_hs,

		// Token Information
		token_valid, crc5_err,

		// Receive Data Output
		rx_data_valid, rx_data_done, crc16_err,

		// Packet Assembler Interface
		send_token, token_pid_sel,
		data_pid_sel, send_zero_length,

		// IDMA Interface
		rx_dma_en, tx_dma_en,
		abort, idma_done,
		adr, size, buf_size,
		sizu_c, dma_en,

		// Register File Interface
		fsel, idin,
		dma_in_buf_sz1, dma_out_buf_avail,
		ep_sel, match, nse_err,
		buf0_rl, buf0_set, buf1_set,
		uc_bsel_set, uc_dpd_set,

		int_buf1_set, int_buf0_set, int_upid_set,
		int_crc16_set, int_to_set, int_seqerr_set,
		out_to_small,

		csr, buf0, buf1

		);

parameter	SSRAM_HADR = 14;

input		clk, rst;
input		tx_valid, rx_active;

// Packet Disassembler Interface
		// Decoded PIDs (used when token_valid is asserted)
input		pid_OUT, pid_IN, pid_SOF, pid_SETUP;
input		pid_DATA0, pid_DATA1, pid_DATA2, pid_MDATA;
input		pid_ACK, pid_NACK, pid_STALL, pid_NYET;
input		pid_PRE, pid_ERR, pid_SPLIT, pid_PING;

input		mode_hs;
input		token_valid;		// Token is valid
input		crc5_err;		// Token crc5 error

input		rx_data_valid;		// Data on rx_data_st is valid
input		rx_data_done;		// Indicates end of a transfer
input		crc16_err;		// Data packet CRC 16 error

// Packet Assembler Interface
output		send_token;
output	[1:0]	token_pid_sel;
output	[1:0]	data_pid_sel;
output		send_zero_length;

// IDMA Interface
output		rx_dma_en;	// Allows the data to be stored
output		tx_dma_en;	// Allows for data to be retrieved
output		abort;		// Abort Transfer (time_out, crc_err or rx_error)
input		idma_done;	// DMA is done indicator
output	[SSRAM_HADR + 2:0]	adr;		// Byte Address
output	[13:0]	size;		// Size in bytes
output	[13:0]	buf_size;	// Actual buffer size
input	[10:0]	sizu_c;		// Up and Down counting size registers, used to update
output		dma_en;		// USB external DMA mode enabled

// Register File interface
input		fsel;		// This function is selected
output	[31:0]	idin;		// Data Output
input	[3:0]	ep_sel;		// Endpoint Number Input
input		match;		// Endpoint Matched
output		nse_err;	// no such endpoint error
input		dma_in_buf_sz1, dma_out_buf_avail;

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



///////////////////////////////////////////////////////////////////
//
// Local Wires and Registers
//

// tx token decoding
parameter	ACK   = 0,
		NACK  = 1,
		STALL = 2,
		NYET  = 3;

// State decoding
parameter	[9:0]	// synopsys enum state
		IDLE	= 10'b000000_0001,
		TOKEN	= 10'b000000_0010,
		IN	= 10'b000000_0100,
		IN2	= 10'b000000_1000,
		OUT	= 10'b000001_0000,
		OUT2A	= 10'b000010_0000,
		OUT2B	= 10'b000100_0000,
		UPDATEW	= 10'b001000_0000,
		UPDATE	= 10'b010000_0000,
		UPDATE2	= 10'b100000_0000;

reg	[1:0]	token_pid_sel;
reg	[1:0]	token_pid_sel_d;
reg		send_token;
reg		send_token_d;
reg		rx_dma_en, tx_dma_en;
reg		int_seqerr_set_d;
reg		int_seqerr_set;
reg		int_upid_set;

reg		match_r;

// Endpoint Decoding
wire		IN_ep, OUT_ep, CTRL_ep;		// Endpoint Types
wire		txfr_iso, txfr_bulk;		// Transfer Types
wire		ep_disabled, ep_stall;		// Endpoint forced conditions

wire		lrg_ok, sml_ok;		// Packet size acceptance
wire	[1:0]	tr_fr;			// Number of transfers per micro-frame
wire	[10:0]	max_pl_sz;		// Max payload size

wire	[1:0]	uc_dpd, uc_bsel;

// Buffer checks
wire		buf_sel;
reg		buf0_na, buf1_na;
wire	[SSRAM_HADR + 2:0]	buf0_adr, buf1_adr;
wire	[13:0]	buf0_sz, buf1_sz;
reg	[9:0]	/* synopsys enum state */ state, next_state;
// synopsys state_vector state

// PID next and current decoders
reg	[1:0]	next_dpid;
reg	[1:0]	this_dpid;
reg		pid_seq_err;
wire	[1:0]	tr_fr_d;

wire	[13:0]	size_next;
wire		buf_smaller;

reg	[SSRAM_HADR + 2:0]	adr;
reg	[13:0]	new_size;
reg	[13:0]	new_sizeb;
reg		buffer_full;
reg		buffer_empty;
wire	[SSRAM_HADR + 2:0]	new_adr;
reg		buffer_done;

reg		no_bufs0, no_bufs1;
wire		no_bufs;

// After sending Data in response to an IN token from host, the
// host must reply with an ack. The host has XXXnS to reply.
// "rx_ack_to" indicates when this time has expired.
// rx_ack_to_clr, clears the timer
reg		rx_ack_to_clr;
reg		rx_ack_to_clr_d;
reg		rx_ack_to;
reg	[7:0]	rx_ack_to_cnt;

// After sending a OUT token the host must send a data packet.
// The host has XX nS to send the packet. "tx_data_to" indicates
// when this time has expired.
// tx_data_to_clr, clears the timer
wire		tx_data_to_clr;
reg		tx_data_to;
reg	[7:0]	tx_data_to_cnt;

wire	[7:0]	rx_ack_to_val, tx_data_to_val;

reg		int_set_en;

wire	[1:0]	next_bsel;
reg		buf_set_d;
reg		uc_stat_set_d;
reg	[31:0]	idin;
reg		buf0_set, buf1_set;
reg		uc_bsel_set;
reg		uc_dpd_set;
reg		buf0_rl_d;
reg		buf0_rl;
wire		no_buf0_dma;
reg		buf0_st_max;
reg		buf1_st_max;

reg	[SSRAM_HADR + 2:0]	adr_r;
reg	[13:0]	size_next_r;

reg		in_token;
reg		out_token;
reg		setup_token;

wire		in_op, out_op;	// Indicate a IN or OUT operation
reg		to_small;	// Indicates a "to small packer" error
reg		to_large;	// Indicates a "to large packer" error

reg		buffer_overflow;
reg	[1:0]	allow_pid;

reg		nse_err;
reg		out_to_small, out_to_small_r;
reg		abort;

reg		buf0_not_aloc, buf1_not_aloc;

reg		send_zero_length;

///////////////////////////////////////////////////////////////////
//
// Misc Logic
//

// Endpoint/CSR Decoding
assign IN_ep   = csr[27:26]==2'b01;
assign OUT_ep  = csr[27:26]==2'b10;
assign CTRL_ep = csr[27:26]==2'b00;

assign txfr_iso  = csr[25:24]==2'b01;
assign txfr_bulk = csr[25:24]==2'b10;

assign ep_disabled = csr[23:22]==2'b01;
assign ep_stall    = csr[23:22]==2'b10;

assign lrg_ok = csr[17];
assign sml_ok = csr[16];
assign dma_en = csr[15] & !CTRL_ep;

assign tr_fr = csr[12:11];
assign max_pl_sz = csr[10:0];

assign uc_dpd = csr[29:28];
assign uc_bsel = csr[31:30];

// Buffer decoding and allocation checks
assign buf0_adr = buf0[SSRAM_HADR + 2:0];
assign buf1_adr = buf1[SSRAM_HADR + 2:0];
assign buf0_sz  = buf0[30:17];
assign buf1_sz  = buf1[30:17];

// Buffers Not Available
always @(posedge clk)
	buf0_na <= buf0[31] | ( &buf0_adr );

always @(posedge clk)
	buf1_na <= buf1[31] | ( &buf1_adr );

// Buffer Not Allocated
always @(posedge clk)
	buf0_not_aloc <= &buf0_adr;

always @(posedge clk)
	buf1_not_aloc <= &buf1_adr;

always @(posedge clk)
	match_r <= match;

// No Such Endpoint Indicator
always @(posedge clk)
	nse_err <= token_valid & (pid_OUT | pid_IN | pid_SETUP) & !match;

always @(posedge clk)
	send_token <= send_token_d;

always @(posedge clk)
	token_pid_sel <= token_pid_sel_d;

///////////////////////////////////////////////////////////////////
//
// Data Pid Sequencer
//

assign tr_fr_d = mode_hs ? tr_fr : 2'h0;

always @(posedge clk)	// tr/mf:ep/type:tr/type:last dpd
	casex({tr_fr_d,csr[27:26],csr[25:24],uc_dpd})	// synopsys full_case parallel_case
	   8'b0?_01_01_??: next_dpid <= 2'b00;	// ISO txfr. IN, 1 tr/mf

	   8'b10_01_01_?0: next_dpid <= 2'b01;	// ISO txfr. IN, 2 tr/mf
	   8'b10_01_01_?1: next_dpid <= 2'b00;	// ISO txfr. IN, 2 tr/mf

	   8'b11_01_01_00: next_dpid <= 2'b01;	// ISO txfr. IN, 3 tr/mf
	   8'b11_01_01_01: next_dpid <= 2'b10;	// ISO txfr. IN, 3 tr/mf
	   8'b11_01_01_10: next_dpid <= 2'b00;	// ISO txfr. IN, 3 tr/mf

	   8'b0?_10_01_??: next_dpid <= 2'b00;	// ISO txfr. OUT, 1 tr/mf

	   8'b10_10_01_??: 				// ISO txfr. OUT, 2 tr/mf
			   begin	// Resynchronize in case of PID error
				case({pid_MDATA, pid_DATA1})	// synopsys full_case parallel_case
				  2'b10: next_dpid <= 2'b01;
				  2'b01: next_dpid <= 2'b00;
				endcase
			   end

	   8'b11_10_01_00: 				// ISO txfr. OUT, 3 tr/mf
			   begin	// Resynchronize in case of PID error
				case({pid_MDATA, pid_DATA2})	// synopsys full_case parallel_case
				  2'b10: next_dpid <= 2'b01;
				  2'b01: next_dpid <= 2'b00;
				endcase
			   end
	   8'b11_10_01_01: 				// ISO txfr. OUT, 3 tr/mf
			   begin	// Resynchronize in case of PID error
				case({pid_MDATA, pid_DATA2})	// synopsys full_case parallel_case
				  2'b10: next_dpid <= 2'b10;
				  2'b01: next_dpid <= 2'b00;
				endcase
			   end
	   8'b11_10_01_10: 				// ISO txfr. OUT, 3 tr/mf
			   begin	// Resynchronize in case of PID error
				case({pid_MDATA, pid_DATA2})	// synopsys full_case parallel_case
				  2'b10: next_dpid <= 2'b01;
				  2'b01: next_dpid <= 2'b00;
				endcase
			   end

	   8'b??_01_00_?0,				// IN/OUT endpoint only
	   8'b??_10_00_?0: next_dpid <= 2'b01;	// INT transfers

	   8'b??_01_00_?1,				// IN/OUT endpoint only
	   8'b??_10_00_?1: next_dpid <= 2'b00;	// INT transfers

	   8'b??_01_10_?0,				// IN/OUT endpoint only
	   8'b??_10_10_?0: next_dpid <= 2'b01;	// BULK transfers

	   8'b??_01_10_?1,				// IN/OUT endpoint only
	   8'b??_10_10_?1: next_dpid <= 2'b00;	// BULK transfers

	   8'b??_00_??_??:				// CTRL Endpoint
		casex({setup_token, in_op, out_op, uc_dpd})	// synopsys full_case parallel_case
		   5'b1_??_??: next_dpid <= 2'b11;	// SETUP operation
		   5'b0_10_0?: next_dpid <= 2'b11;	// IN operation
		   5'b0_10_1?: next_dpid <= 2'b01;	// IN operation
		   5'b0_01_?0: next_dpid <= 2'b11;	// OUT operation
		   5'b0_01_?1: next_dpid <= 2'b10;	// OUT operation
		endcase

	endcase

// Current PID decoder

// Allow any PID for ISO. transfers when mode full speed or tr_fr is zero
always @(pid_DATA0 or pid_DATA1 or pid_DATA2 or pid_MDATA)
	case({pid_DATA0, pid_DATA1, pid_DATA2, pid_MDATA} ) // synopsys full_case parallel_case
	   4'b1000: allow_pid = 2'b00;
	   4'b0100: allow_pid = 2'b01;
	   4'b0010: allow_pid = 2'b10;
	   4'b0001: allow_pid = 2'b11;
	endcase

always @(posedge clk)	// tf/mf:ep/type:tr/type:last dpd
	casex({tr_fr_d,csr[27:26],csr[25:24],uc_dpd})	// synopsys full_case parallel_case
	   8'b0?_01_01_??: this_dpid <= 2'b00;	// ISO txfr. IN, 1 tr/mf

	   8'b10_01_01_?0: this_dpid <= 2'b01;	// ISO txfr. IN, 2 tr/mf
	   8'b10_01_01_?1: this_dpid <= 2'b00;	// ISO txfr. IN, 2 tr/mf

	   8'b11_01_01_00: this_dpid <= 2'b10;	// ISO txfr. IN, 3 tr/mf
	   8'b11_01_01_01: this_dpid <= 2'b01;	// ISO txfr. IN, 3 tr/mf
	   8'b11_01_01_10: this_dpid <= 2'b00;	// ISO txfr. IN, 3 tr/mf

	   8'b00_10_01_??: this_dpid <= allow_pid;	// ISO txfr. OUT, 0 tr/mf
	   8'b01_10_01_??: this_dpid <= 2'b00;	// ISO txfr. OUT, 1 tr/mf

	   8'b10_10_01_?0: this_dpid <= 2'b11;	// ISO txfr. OUT, 2 tr/mf
	   8'b10_10_01_?1: this_dpid <= 2'b01;	// ISO txfr. OUT, 2 tr/mf

	   8'b11_10_01_00: this_dpid <= 2'b11;	// ISO txfr. OUT, 3 tr/mf
	   8'b11_10_01_01: this_dpid <= 2'b11;	// ISO txfr. OUT, 3 tr/mf
	   8'b11_10_01_10: this_dpid <= 2'b10;	// ISO txfr. OUT, 3 tr/mf

	   8'b??_01_00_?0,				// IN/OUT endpoint only
	   8'b??_10_00_?0: this_dpid <= 2'b00;	// INT transfers
	   8'b??_01_00_?1,				// IN/OUT endpoint only
	   8'b??_10_00_?1: this_dpid <= 2'b01;	// INT transfers

	   8'b??_01_10_?0,				// IN/OUT endpoint only
	   8'b??_10_10_?0: this_dpid <= 2'b00;	// BULK transfers
	   8'b??_01_10_?1,				// IN/OUT endpoint only
	   8'b??_10_10_?1: this_dpid <= 2'b01;	// BULK transfers

	   8'b??_00_??_??:				// CTRL Endpoint
		casex({setup_token,in_op, out_op, uc_dpd})	// synopsys full_case parallel_case
		   5'b1_??_??: this_dpid <= 2'b00;	// SETUP operation
		   5'b0_10_0?: this_dpid <= 2'b00;	// IN operation
		   5'b0_10_1?: this_dpid <= 2'b01;	// IN operation
		   5'b0_01_?0: this_dpid <= 2'b00;	// OUT operation
		   5'b0_01_?1: this_dpid <= 2'b01;	// OUT operation
		endcase
	endcase

// Assign PID for outgoing packets
assign data_pid_sel = this_dpid;

// Verify PID for incoming data packets
always @(posedge clk)
	pid_seq_err <= !(	(this_dpid==2'b00 & pid_DATA0) |
				(this_dpid==2'b01 & pid_DATA1) |
				(this_dpid==2'b10 & pid_DATA2) |
				(this_dpid==2'b11 & pid_MDATA)	);

///////////////////////////////////////////////////////////////////
//
// IDMA Setup & src/dst buffer select
//

// For Control endpoints things are different:
// buffer0 is used for OUT (incoming) data packets
// buffer1 is used for IN (outgoing) data packets

// Keep track of last token for control endpoints
`ifdef USBF_ASYNC_RESET
always @(posedge clk or negedge rst)
`else
always @(posedge clk)
`endif
	if(!rst)			in_token <= 1'b0;
	else
	if(pid_IN)			in_token <= 1'b1;
	else
	if(pid_OUT || pid_SETUP)	in_token <= 1'b0;

`ifdef USBF_ASYNC_RESET
always @(posedge clk or negedge rst)
`else
always @(posedge clk)
`endif
	if(!rst)			out_token <= 1'b0;
	else
	if(pid_OUT || pid_SETUP)	out_token <= 1'b1;
	else
	if(pid_IN)			out_token <= 1'b0;

`ifdef USBF_ASYNC_RESET
always @(posedge clk or negedge rst)
`else
always @(posedge clk)
`endif
	if(!rst)			setup_token <= 1'b0;
	else
	if(pid_SETUP)			setup_token <= 1'b1;
	else
	if(pid_OUT || pid_IN)		setup_token <= 1'b0;

// Indicates if we are performing an IN operation
assign	in_op = IN_ep | (CTRL_ep & in_token);

// Indicates if we are performing an OUT operation
assign	out_op = OUT_ep | (CTRL_ep & out_token);

// Select buffer: buf_sel==0 buffer0; buf_sel==1 buffer1
assign buf_sel =  dma_en ? 1'b0 : CTRL_ep ? in_token : ((uc_bsel[0] | buf0_na) & !buf1_na);

// Select Address for IDMA
always @(posedge clk)
	adr <= buf_sel ? buf1_adr : buf0_adr;

// Size from Buffer
assign buf_size = buf_sel ? buf1_sz  : buf0_sz;

// Determine which is smaller: buffer or max_pl_sz
assign buf_smaller = buf_size < {3'h0, max_pl_sz};

// Determine actual size for this transfer (for IDMA) IN endpoint only
// (OUT endpoint uses sizeu_c from IDMA)
assign size_next = buf_smaller ? buf_size : max_pl_sz;
assign size = size_next;	// "size" is an output for IDMA

// Buffer Full (only for OUT endpoints)
// Indicates that there is not enough space in the buffer for one
// more max_pl_sz packet
always @(posedge clk)
	buffer_full <= new_size < {3'h0, max_pl_sz};

// Buffer Empty (only for IN endpoints)
// Indicates that there are zero bytes left in the buffer
always @(posedge clk)
	buffer_empty <= (new_size == 14'h0);

// Joint buffer full/empty flag This is the "USED" flag
always @(posedge clk)
	buffer_done <= in_op ? buffer_empty : buffer_full;

// No More buffer space at all (For high speed out - issue NYET)
assign no_buf0_dma = dma_en &
		((IN_ep & !dma_in_buf_sz1) | (OUT_ep & !dma_out_buf_avail));

always @(posedge clk)
	buf0_st_max <= (buf0_sz < {3'h0, max_pl_sz});

always @(posedge clk)
	buf1_st_max <= (buf1_sz < {3'h0, max_pl_sz});

always @(posedge clk)
	no_bufs0 <= buf0_na | no_buf0_dma |
			(buf_sel ? buf0_st_max : (buffer_full & !dma_en));

always @(posedge clk)
	no_bufs1 <= buf1_na | (buf_sel ? buffer_full : buf1_st_max);

assign no_bufs = no_bufs0 & no_bufs1;

// New Size (to be written to register file)
always @(posedge clk)
	new_sizeb <= (out_op && dma_en) ? max_pl_sz : (in_op ? size_next : sizu_c);

always @(posedge clk)
	new_size <= buf_size - new_sizeb;


// New Buffer Address (to be written to register file)
always @(posedge clk)
	adr_r <= adr;

always @(posedge clk)
	size_next_r <= size_next;

assign new_adr = adr_r[SSRAM_HADR + 2:0] +
		((out_op && dma_en) ? {{SSRAM_HADR + 2-10{1'b0}}, max_pl_sz[10:0]} :
		(in_op ? {{SSRAM_HADR + 2-13{1'b0}}, size_next_r[13:0] } :
		{ {SSRAM_HADR + 2-10{1'b0}}, sizu_c[10:0]}));

// Buffer Overflow
always @(posedge clk)
	buffer_overflow <= ( {3'h0, sizu_c} > buf_size) & rx_data_valid;


// OUT packet smaller than MAX_PL_SZ in DMA operation
always @(posedge clk)
	out_to_small_r <= uc_stat_set_d & out_op & dma_en & (sizu_c != max_pl_sz);

always @(posedge clk)
	out_to_small <= out_to_small_r;

///////////////////////////////////////////////////////////////////
//
// Determine if packet is to small or to large
// This is used to NACK and ignore packet for OUT endpoints
//

always @(posedge clk)
	to_small <= !sml_ok & (sizu_c < max_pl_sz);

always @(posedge clk)
	to_large <= !lrg_ok & (sizu_c > max_pl_sz);

///////////////////////////////////////////////////////////////////
//
// Register File Update Logic
//

assign next_bsel = dma_en ? 2'h0 : buffer_done ? uc_bsel + 2'h1 : uc_bsel;	// FIX_ME

always @(posedge clk)
	idin[31:17] <= out_to_small_r ? {4'h0,sizu_c} : {buffer_done,new_size};

always @(posedge clk)
	idin[SSRAM_HADR + 2:4] <= out_to_small_r ?	buf0_adr[SSRAM_HADR + 2:4] :
							new_adr[SSRAM_HADR + 2:4];

always @(posedge clk)
	if(buf_set_d)			idin[3:0] <= new_adr[3:0];
	else
	if(out_to_small_r)		idin[3:0] <= buf0_adr[3:0];
	else				idin[3:0] <= {next_dpid, next_bsel};

always @(posedge clk)
	buf0_set <= !buf_sel & buf_set_d;

always @(posedge clk)
	buf1_set <= buf_sel & buf_set_d;

always @(posedge clk)
	uc_bsel_set <= uc_stat_set_d;

always @(posedge clk)
	uc_dpd_set <= uc_stat_set_d;

always @(posedge clk)
	buf0_rl <= buf0_rl_d;

// Abort signal
always @(posedge clk)
	abort <= buffer_overflow | (match & (state != IDLE) ) | (match_r & to_large);

///////////////////////////////////////////////////////////////////
//
// TIME OUT TIMERS
//

// After sending Data in response to an IN token from host, the
// host must reply with an ack. The host has 622nS in Full Speed
// mode and 400nS in High Speed mode to reply.
// "rx_ack_to" indicates when this time has expired.
// rx_ack_to_clr, clears the timer

always @(posedge clk)
	rx_ack_to_clr <= tx_valid | rx_ack_to_clr_d;

always @(posedge clk)
	if(rx_ack_to_clr)	rx_ack_to_cnt <= 8'h0;
	else			rx_ack_to_cnt <= rx_ack_to_cnt + 8'h1;

always @(posedge clk)
	rx_ack_to <= (rx_ack_to_cnt == rx_ack_to_val);

assign rx_ack_to_val = mode_hs ? `USBF_RX_ACK_TO_VAL_HS : `USBF_RX_ACK_TO_VAL_FS;

// After sending a OUT token the host must send a data packet.
// The host has 622nS in Full Speed mode and 400nS in High Speed
// mode to send the data packet.
// "tx_data_to" indicates when this time has expired.
// "tx_data_to_clr" clears the timer

assign	tx_data_to_clr = rx_active;

always @(posedge clk)
	if(tx_data_to_clr)	tx_data_to_cnt <= 8'h0;
	else			tx_data_to_cnt <= tx_data_to_cnt + 8'h1;

always @(posedge clk)
	tx_data_to <= (tx_data_to_cnt == tx_data_to_val);

assign tx_data_to_val = mode_hs ? `USBF_TX_DATA_TO_VAL_HS : `USBF_TX_DATA_TO_VAL_FS;

///////////////////////////////////////////////////////////////////
//
// Interrupts
//
reg	pid_OUT_r, pid_IN_r, pid_PING_r, pid_SETUP_r;

assign int_buf1_set = !buf_sel & buffer_done & int_set_en & !buf1_not_aloc;
assign int_buf0_set =  buf_sel & buffer_done & int_set_en & !buf0_not_aloc;

always @(posedge clk)
	pid_OUT_r <= pid_OUT;

always @(posedge clk)
	pid_IN_r <= pid_IN;

always @(posedge clk)
	pid_PING_r <= pid_PING;

always @(posedge clk)
	pid_SETUP_r <= pid_SETUP;

always @(posedge clk)
	int_upid_set <= match_r & !pid_SOF & (
				( OUT_ep & !(pid_OUT_r | pid_PING_r))		|
				(  IN_ep &  !pid_IN_r)				|
				(CTRL_ep & !(pid_IN_r | pid_OUT_r | pid_PING_r | pid_SETUP_r))
					);

assign int_to_set  = ((state == IN2) & rx_ack_to) | ((state == OUT) & tx_data_to);

assign int_crc16_set = rx_data_done & crc16_err;

always @(posedge clk)
	int_seqerr_set <= int_seqerr_set_d;

///////////////////////////////////////////////////////////////////
//
// Main Protocol State Machine
//

`ifdef USBF_ASYNC_RESET
always @(posedge clk or negedge rst)
`else
always @(posedge clk)
`endif
	if(!rst)	state <= IDLE;
	else
	if(match)	state <= IDLE;
	else		state <= next_state;

always @(state or ep_stall or buf0_na or buf1_na or
	pid_seq_err or idma_done or token_valid or pid_ACK or rx_data_done or
	tx_data_to or crc16_err or ep_disabled or no_bufs or mode_hs
	or dma_en or rx_ack_to or pid_PING or txfr_iso or to_small or to_large or
	CTRL_ep or pid_IN or pid_OUT or IN_ep or OUT_ep or pid_SETUP or pid_SOF
	or match_r or abort or buffer_done or no_buf0_dma or max_pl_sz)
   begin
	next_state = state;
	token_pid_sel_d = ACK;
	send_token_d = 1'b0;
	rx_dma_en = 1'b0;
	tx_dma_en = 1'b0;
	buf_set_d = 1'b0;
	uc_stat_set_d = 1'b0;
	buf0_rl_d = 1'b0;
	int_set_en = 1'b0;
	rx_ack_to_clr_d = 1'b1;
	int_seqerr_set_d = 1'b0;
	send_zero_length = 1'b0;

	case(state)	// synopsys full_case parallel_case
	   IDLE:
		   begin
// synopsys translate_off
`ifdef USBF_VERBOSE_DEBUG
		$display("PE: Entered state IDLE (%t)", $time);
`endif
`ifdef USBF_DEBUG
		if(rst && match_r && !ep_disabled && !pid_SOF)
		begin
		if(match_r === 1'bx)	$display("ERROR: IDLE: match_r is unknown. (%t)", $time);
		if(ep_disabled === 1'bx)$display("ERROR: IDLE: ep_disabled is unknown. (%t)", $time);
		if(pid_SOF === 1'bx)	$display("ERROR: IDLE: pid_SOF is unknown. (%t)", $time);
		if(ep_stall === 1'bx)	$display("ERROR: IDLE: ep_stall is unknown. (%t)", $time);
		if(buf0_na === 1'bx)	$display("ERROR: IDLE: buf0_na is unknown. (%t)", $time);
		if(buf1_na === 1'bx)	$display("ERROR: IDLE: buf1_na is unknown. (%t)", $time);
		if(no_buf0_dma === 1'bx)$display("ERROR: IDLE: no_buf0_dma is unknown. (%t)", $time);
		if(CTRL_ep === 1'bx)	$display("ERROR: IDLE: CTRL_ep is unknown. (%t)", $time);
		if(pid_IN === 1'bx)	$display("ERROR: IDLE: pid_IN is unknown. (%t)", $time);
		if(pid_OUT === 1'bx)	$display("ERROR: IDLE: pid_OUT is unknown. (%t)", $time);
		if(pid_SETUP === 1'bx)	$display("ERROR: IDLE: pid_SETUP is unknown. (%t)", $time);
		if(pid_PING === 1'bx)	$display("ERROR: IDLE: pid_PING is unknown. (%t)", $time);
		if(mode_hs === 1'bx)	$display("ERROR: IDLE: mode_hs is unknown. (%t)", $time);
		if(IN_ep === 1'bx)	$display("ERROR: IDLE: IN_ep is unknown. (%t)", $time);
		if(OUT_ep === 1'bx)	$display("ERROR: IDLE: OUT_ep is unknown. (%t)", $time);
		end
`endif
// synopsys translate_on

			if(match_r && !ep_disabled && !pid_SOF)
			   begin
				if(ep_stall)		// Halt Forced send STALL
				   begin
					token_pid_sel_d = STALL;
					send_token_d = 1'b1;
					next_state = TOKEN;
				   end
				else
				if(	(buf0_na && buf1_na) || no_buf0_dma ||
					(CTRL_ep && pid_IN   && buf1_na) ||
					(CTRL_ep && pid_OUT  && buf0_na) 
					)
				   begin		// No buffers send NAK
					token_pid_sel_d = NACK;
					send_token_d = 1'b1;
					next_state = TOKEN;
				   end
				else
				if(pid_PING && mode_hs)
				   begin
					token_pid_sel_d = ACK;
					send_token_d = 1'b1;
					next_state = TOKEN;
				   end
				else
				if(IN_ep || (CTRL_ep && pid_IN))
				   begin
					if(max_pl_sz == 11'h0)	send_zero_length = 1'b1;
					tx_dma_en = 1'b1;
					next_state = IN;
				   end
				else
				if(OUT_ep || (CTRL_ep && (pid_OUT || pid_SETUP)))
				   begin
					rx_dma_en = 1'b1;
					next_state = OUT;
				   end
			   end
		   end

	   TOKEN:
		   begin
// synopsys translate_off
`ifdef USBF_VERBOSE_DEBUG
		$display("PE: Entered state TOKEN (%t)", $time);
`endif
// synopsys translate_on
			next_state = IDLE;
		   end

	   IN:
		   begin
// synopsys translate_off
`ifdef USBF_VERBOSE_DEBUG
		$display("PE: Entered state IN (%t)", $time);
`endif
`ifdef USBF_DEBUG
		if(idma_done === 1'bx)	$display("ERROR: IN: idma_done is unknown. (%t)", $time);
		if(txfr_iso === 1'bx)	$display("ERROR: IN: txfr_iso is unknown. (%t)", $time);
`endif
// synopsys translate_on
			rx_ack_to_clr_d = 1'b0;
			if(idma_done)
			   begin
				if(txfr_iso)	next_state = UPDATE;
				else		next_state = IN2;
			   end

		   end
	   IN2:
		   begin
// synopsys translate_off
`ifdef USBF_VERBOSE_DEBUG
		$display("PE: Entered state IN2 (%t)", $time);
`endif
`ifdef USBF_DEBUG
		if(rx_ack_to === 1'bx)	$display("ERROR: IN2: rx_ack_to is unknown. (%t)", $time);
		if(token_valid === 1'bx)$display("ERROR: IN2: token_valid is unknown. (%t)", $time);
		if(pid_ACK === 1'bx)	$display("ERROR: IN2: pid_ACK is unknown. (%t)", $time);
`endif
// synopsys translate_on
			rx_ack_to_clr_d = 1'b0;
			// Wait for ACK from HOST or Timeout
			if(rx_ack_to)	next_state = IDLE;
			else
			if(token_valid && pid_ACK)
			   begin
				next_state = UPDATE;
			   end
		   end

	   OUT:
		   begin
// synopsys translate_off
`ifdef USBF_VERBOSE_DEBUG
		$display("PE: Entered state OUT (%t)", $time);
`endif
`ifdef USBF_DEBUG
		if(tx_data_to === 1'bx)	$display("ERROR: OUT: tx_data_to is unknown. (%t)", $time);
		if(crc16_err === 1'bx)	$display("ERROR: OUT: crc16_err is unknown. (%t)", $time);
		if(abort === 1'bx)	$display("ERROR: OUT: abort is unknown. (%t)", $time);
		if(rx_data_done === 1'bx)$display("ERROR: OUT: rx_data_done is unknown. (%t)", $time);
		if(txfr_iso === 1'bx)	$display("ERROR: OUT: txfr_iso is unknown. (%t)", $time);
		if(pid_seq_err === 1'bx)$display("ERROR: OUT: rx_data_done is unknown. (%t)", $time);
`endif
// synopsys translate_on
			if(tx_data_to || crc16_err || abort )
				next_state = IDLE;
			else
			if(rx_data_done)
			   begin		// Send Ack
				if(txfr_iso)
				   begin
					if(pid_seq_err)		int_seqerr_set_d = 1'b1;
					next_state = UPDATEW;
				   end
				else		next_state = OUT2A;
			   end
		   end

	   OUT2A:
		   begin	// This is a delay State to NACK to small or to
				// large packets. this state could be skipped
// synopsys translate_off
`ifdef USBF_VERBOSE_DEBUG
		$display("PE: Entered state OUT2A (%t)", $time);
`endif
`ifdef USBF_DEBUG
		if(abort === 1'bx)	$display("ERROR: OUT2A: abort is unknown. (%t)", $time);
`endif
// synopsys translate_on
			if(abort)	next_state = IDLE;
			else		next_state = OUT2B;
		   end
	   OUT2B:
		   begin	// Send ACK/NACK/NYET
// synopsys translate_off
`ifdef USBF_VERBOSE_DEBUG
		$display("PE: Entered state OUT2B (%t)", $time);
`endif
`ifdef USBF_DEBUG
		if(abort === 1'bx)	$display("ERROR: OUT2B: abort is unknown. (%t)", $time);
		if(to_small === 1'bx)	$display("ERROR: OUT2B: to_small is unknown. (%t)", $time);
		if(to_large === 1'bx)	$display("ERROR: OUT2B: to_large is unknown. (%t)", $time);
		if(pid_seq_err === 1'bx)$display("ERROR: OUT2B: rx_data_done is unknown. (%t)", $time);
		if(mode_hs === 1'bx)	$display("ERROR: OUT2B: mode_hs is unknown. (%t)", $time);
		if(no_bufs === 1'bx)	$display("ERROR: OUT2B: no_bufs is unknown. (%t)", $time);
`endif
// synopsys translate_on
			if(abort)	next_state = IDLE;
			else
			if(to_small || to_large)
			   begin
				token_pid_sel_d = NACK;
				next_state = IDLE;
			   end
			else
			if(pid_seq_err)	
			   begin
				token_pid_sel_d = ACK;
				send_token_d = 1'b1;
				next_state = IDLE;
			   end
			else
			   begin
				if(mode_hs && no_bufs)	token_pid_sel_d = NYET;
				else			token_pid_sel_d = ACK;
				send_token_d = 1'b1;
				next_state = UPDATE;
			   end
		   end

	   UPDATEW:
		   begin
// synopsys translate_off
`ifdef USBF_VERBOSE_DEBUG
		$display("PE: Entered state UPDATEW (%t)", $time);
`endif
// synopsys translate_on
			next_state = UPDATE;
		   end
	   UPDATE:
		   begin
// synopsys translate_off
`ifdef USBF_VERBOSE_DEBUG
		$display("PE: Entered state UPDATE (%t)", $time);
`endif
`ifdef USBF_DEBUG
		if(buffer_done === 1'bx)	$display("ERROR: UPDATE: buffer_done is unknown. (%t)", $time);
		if(dma_en === 1'bx)	$display("ERROR: UPDATE: dma_en is unknown. (%t)", $time);
`endif
// synopsys translate_on
			// Interrupts
			int_set_en = 1'b1;
			// Buffer (used, size, adr) set or reload
			if(buffer_done && dma_en)
			   begin
				buf0_rl_d = 1'b1;
			   end
			else
			   begin
				buf_set_d = 1'b1;
			   end
			next_state = UPDATE2;
		   end
	   UPDATE2:	// Update Register File & state
		   begin
// synopsys translate_off
`ifdef USBF_VERBOSE_DEBUG
		$display("PE: Entered state UPDATE2 (%t)", $time);
`endif
// synopsys translate_on
			// pid sequence & buffer usage
			uc_stat_set_d = 1'b1;
			next_state = IDLE;
		   end
	endcase
   end

endmodule

