/////////////////////////////////////////////////////////////////////
////                                                             ////
////  Packet Disassembler                                        ////
////  Disassembles Token and Data USB packets                    ////
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
//  $Id: usbf_pd.v,v 1.7 2003/10/17 02:36:57 rudi Exp $
//
//  $Date: 2003/10/17 02:36:57 $
//  $Revision: 1.7 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: usbf_pd.v,v $
//               Revision 1.7  2003/10/17 02:36:57  rudi
//               - Disabling bit stuffing and NRZI encoding during speed negotiation
//               - Now the core can send zero size packets
//               - Fixed register addresses for some of the higher endpoints
//                 (conversion between decimal/hex was wrong)
//               - The core now does properly evaluate the function address to
//                 determine if the packet was intended for it.
//               - Various other minor bugs and typos
//
//               Revision 1.5  2001/11/03 03:26:22  rudi
//
//               - Fixed several interrupt and error condition reporting bugs
//
//               Revision 1.4  2001/09/24 01:15:28  rudi
//
//               Changed reset to be active high async.
//
//               Revision 1.3  2001/09/10 15:54:20  rudi
//
//               Fixed crc5 checking.
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
//               Revision 0.1.0.1  2001/02/28 08:10:59  rudi
//               Initial Release
//
//

`include "usbf_defines.v"

module usbf_pd(	clk, rst,

		// UTMI RX I/F
		rx_data, rx_valid, rx_active, rx_err,

		// PID Information
		pid_OUT, pid_IN, pid_SOF, pid_SETUP,
		pid_DATA0, pid_DATA1, pid_DATA2, pid_MDATA,
		pid_ACK, pid_NACK, pid_STALL, pid_NYET,
		pid_PRE, pid_ERR, pid_SPLIT, pid_PING,
		pid_cks_err,

		// Token Information
		token_fadr, token_endp, token_valid, crc5_err,
		frame_no,

		// Receive Data Output
		rx_data_st, rx_data_valid, rx_data_done, crc16_err,

		// Misc.
		seq_err
		);

input		clk, rst;

		//UTMI RX Interface
input	[7:0]	rx_data;
input		rx_valid, rx_active, rx_err;

		// Decoded PIDs (used when token_valid is asserted)
output		pid_OUT, pid_IN, pid_SOF, pid_SETUP;
output		pid_DATA0, pid_DATA1, pid_DATA2, pid_MDATA;
output		pid_ACK, pid_NACK, pid_STALL, pid_NYET;
output		pid_PRE, pid_ERR, pid_SPLIT, pid_PING;
output		pid_cks_err;		// Indicates a PID checksum error


output	[6:0]	token_fadr;		// Function address from token
output	[3:0]	token_endp;		// Endpoint number from token
output		token_valid;		// Token is valid
output		crc5_err;		// Token crc5 error
output	[10:0]	frame_no;		// Frame number for SOF tokens

output	[7:0]	rx_data_st;		// Data to memory store unit
output		rx_data_valid;		// Data on rx_data_st is valid
output		rx_data_done;		// Indicates end of a transfer
output		crc16_err;		// Data packet CRC 16 error

output		seq_err;		// State Machine Sequence Error

///////////////////////////////////////////////////////////////////
//
// Local Wires and Registers
//

parameter	[3:0]	// synopsys enum state
		IDLE   = 4'b0001,
		ACTIVE = 4'b0010,
		TOKEN  = 4'b0100,
		DATA   = 4'b1000;

reg	[3:0]	/* synopsys enum state */ state, next_state;
// synopsys state_vector state

reg	[7:0]	pid;			// Packet PDI
reg		pid_le_sm;		// PID Load enable from State Machine
wire		pid_ld_en;		// Enable loading of PID (all conditions)
wire		pid_cks_err;		// Indicates a pid checksum err

		// Decoded PID values
wire		pid_OUT, pid_IN, pid_SOF, pid_SETUP;
wire		pid_DATA0, pid_DATA1, pid_DATA2, pid_MDATA;
wire		pid_ACK, pid_NACK, pid_STALL, pid_NYET;
wire		pid_PRE, pid_ERR, pid_SPLIT, pid_PING, pid_RES;
wire		pid_TOKEN;		// All TOKEN packet that we recognize
wire		pid_DATA;		// All DATA packets that we recognize

reg	[7:0]	token0, token1;		// Token Registers
reg		token_le_1, token_le_2;	// Latch enables for token storage registers
wire	[4:0]	token_crc5;

reg	[7:0]	d0, d1, d2;		// Data path delay line (used to filter out crcs)
reg		data_valid_d;		// Data Valid output from State Machine
reg		data_done;		// Data cycle complete output from State Machine
reg		data_valid0; 		// Data valid delay line
reg		rxv1;
reg		rxv2;

reg		seq_err;		// State machine sequence error

reg		got_pid_ack;

reg		token_valid_r1;
reg		token_valid_str1;

reg		rx_active_r;

wire	[4:0]	crc5_out;
wire	[4:0]	crc5_out2;
wire		crc16_clr;
reg	[15:0]	crc16_sum;
wire	[15:0]	crc16_out;

///////////////////////////////////////////////////////////////////
//
// Misc Logic
//

// PID Decoding Logic
assign pid_ld_en = pid_le_sm & rx_active & rx_valid;

`ifdef USBF_ASYNC_RESET
always @(posedge clk or negedge rst)
`else
always @(posedge clk)
`endif
	if(!rst)		pid <= 8'hf0;
	else
	if(pid_ld_en)		pid <= rx_data;

assign	pid_cks_err = (pid[3:0] != ~pid[7:4]);

assign	pid_OUT   = pid[3:0] == `USBF_T_PID_OUT;
assign	pid_IN    = pid[3:0] == `USBF_T_PID_IN;
assign	pid_SOF   = pid[3:0] == `USBF_T_PID_SOF;
assign	pid_SETUP = pid[3:0] == `USBF_T_PID_SETUP;
assign	pid_DATA0 = pid[3:0] == `USBF_T_PID_DATA0;
assign	pid_DATA1 = pid[3:0] == `USBF_T_PID_DATA1;
assign	pid_DATA2 = pid[3:0] == `USBF_T_PID_DATA2;
assign	pid_MDATA = pid[3:0] == `USBF_T_PID_MDATA;
assign	pid_ACK   = pid[3:0] == `USBF_T_PID_ACK;
assign	pid_NACK  = pid[3:0] == `USBF_T_PID_NACK;
assign	pid_STALL = pid[3:0] == `USBF_T_PID_STALL;
assign	pid_NYET  = pid[3:0] == `USBF_T_PID_NYET;
assign	pid_PRE   = pid[3:0] == `USBF_T_PID_PRE;
assign	pid_ERR   = pid[3:0] == `USBF_T_PID_ERR;
assign	pid_SPLIT = pid[3:0] == `USBF_T_PID_SPLIT;
assign	pid_PING  = pid[3:0] == `USBF_T_PID_PING;
assign	pid_RES   = pid[3:0] == `USBF_T_PID_RES;

assign	pid_TOKEN = pid_OUT | pid_IN | pid_SOF | pid_SETUP | pid_PING;
assign	pid_DATA = pid_DATA0 | pid_DATA1 | pid_DATA2 | pid_MDATA;

// Token Decoding LOGIC
always @(posedge clk)
	if(token_le_1)	token0 <= rx_data;

always @(posedge clk)
	if(token_le_2)	token1 <= rx_data;

always @(posedge clk)
	token_valid_r1 <= token_le_2;

always @(posedge clk)
	token_valid_str1 <= token_valid_r1 | got_pid_ack;

assign token_valid = token_valid_str1;

// CRC 5 should perform the check in one cycle (flow through logic)
// 11 bits and crc5 input, 1 bit output
assign crc5_err = token_valid & (crc5_out2 != token_crc5);

usbf_crc5 u0(
	.crc_in(	5'h1f			),
	.din(	{	token_fadr[0],
			token_fadr[1],
			token_fadr[2],
			token_fadr[3],
			token_fadr[4],
			token_fadr[5],
			token_fadr[6],
			token_endp[0],
			token_endp[1],
			token_endp[2],
			token_endp[3]   }	),
	.crc_out(	crc5_out		) );

// Invert and reverse result bits
assign	crc5_out2 = ~{crc5_out[0], crc5_out[1], crc5_out[2], crc5_out[3],
			crc5_out[4]};

assign frame_no = { token1[2:0], token0};
assign token_fadr = token0[6:0];
assign token_endp = {token1[2:0], token0[7]};
assign token_crc5 = token1[7:3];

// Data receiving logic
// build a delay line and stop when we are about to get crc
`ifdef USBF_ASYNC_RESET
always @(posedge clk or negedge rst)
`else
always @(posedge clk)
`endif
	if(!rst)		rxv1 <= 1'b0;
	else
	if(data_valid_d)	rxv1 <= 1'b1;
	else
	if(data_done)		rxv1 <= 1'b0;

`ifdef USBF_ASYNC_RESET
always @(posedge clk or negedge rst)
`else
always @(posedge clk)
`endif
	if(!rst)		rxv2 <= 1'b0;
	else
	if(rxv1 && data_valid_d)rxv2 <= 1'b1;
	else
	if(data_done)		rxv2 <= 1'b0;

always @(posedge clk)
	data_valid0 <= rxv2 & data_valid_d;

always @(posedge clk)
   begin
	if(data_valid_d)	d0 <= rx_data;
	if(data_valid_d)	d1 <= d0;
	if(data_valid_d)	d2 <= d1;
   end

assign rx_data_st = d2;
assign rx_data_valid = data_valid0;
assign rx_data_done = data_done;

// crc16 accumulates rx_data as long as data_valid_d is asserted.
// when data_done is asserted, crc16 reports status, and resets itself
// next cycle.
always @(posedge clk)
	rx_active_r <= rx_active;

assign crc16_clr = rx_active & !rx_active_r;

always @(posedge clk)
	if(crc16_clr)		crc16_sum <= 16'hffff;
	else
	if(data_valid_d)	crc16_sum <= crc16_out;

usbf_crc16 u1(
	.crc_in(	crc16_sum		),
	.din(	{rx_data[0], rx_data[1], rx_data[2], rx_data[3],
		rx_data[4], rx_data[5], rx_data[6], rx_data[7]}	),
	.crc_out(	crc16_out		) );

// Verify against polynomial 
assign crc16_err = data_done & (crc16_sum != 16'h800d);

///////////////////////////////////////////////////////////////////
//
// Receive/Decode State machine
//

`ifdef USBF_ASYNC_RESET
always @(posedge clk or negedge rst)
`else
always @(posedge clk)
`endif
	if(!rst)	state <= IDLE;
	else		state <= next_state;

always @(state or rx_valid or rx_active or rx_err or pid_ACK or pid_TOKEN
	or pid_DATA)
   begin
	next_state = state;	// Default don't change current state
	pid_le_sm = 1'b0;
	token_le_1 = 1'b0;
	token_le_2 = 1'b0;
	data_valid_d = 1'b0;
	data_done = 1'b0;
	seq_err = 1'b0;
	got_pid_ack = 1'b0;
	case(state)		// synopsys full_case parallel_case
	   IDLE:
		   begin
			pid_le_sm = 1'b1;
			if(rx_valid && rx_active)	next_state = ACTIVE;
		   end
	   ACTIVE:
		   begin
			// Received a ACK from Host
			if(pid_ACK && !rx_err)
			   begin
				got_pid_ack = 1'b1;
				if(!rx_active)	next_state = IDLE;
			   end
			else
			// Receiving a TOKEN
			if(pid_TOKEN && rx_valid && rx_active && !rx_err)
			   begin
				token_le_1 = 1'b1;
				next_state = TOKEN;
			   end
			else
			// Receiving DATA
			if(pid_DATA && rx_valid && rx_active && !rx_err)
			   begin
				data_valid_d = 1'b1;
				next_state = DATA;
			   end
			else
			if(	!rx_active || rx_err ||
				(rx_valid && !(pid_TOKEN || pid_DATA)) )
			   begin
				seq_err = !rx_err;
				if(!rx_active)	next_state = IDLE;
			   end
		   end
	   TOKEN:
		   begin
			if(rx_valid && rx_active && !rx_err)
			   begin
				token_le_2 = 1'b1;
				next_state = IDLE;
			   end
			else
			if(!rx_active || rx_err)
			   begin
				seq_err = !rx_err;
				if(!rx_active)	next_state = IDLE;
			   end
		   end
	   DATA:
		   begin
			if(rx_valid && rx_active && !rx_err)	data_valid_d = 1'b1;
			if(!rx_active || rx_err)
			   begin
				data_done = 1'b1;
				if(!rx_active)	next_state = IDLE;
			   end
		   end
		
	endcase
   end

endmodule

