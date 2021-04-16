/////////////////////////////////////////////////////////////////////
////                                                             ////
////  Dummy Endpoint register File                               ////
////  This module contains termination for registers in ONE      ////
////  endpoint. It is used to replace the actual endpoint        ////
////  register file for non existing endpoints.                  ////
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
//  $Id: usbf_ep_rf_dummy.v,v 1.2 2003/10/17 02:36:57 rudi Exp $
//
//  $Date: 2003/10/17 02:36:57 $
//  $Revision: 1.2 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: usbf_ep_rf_dummy.v,v $
//               Revision 1.2  2003/10/17 02:36:57  rudi
//               - Disabling bit stuffing and NRZI encoding during speed negotiation
//               - Now the core can send zero size packets
//               - Fixed register addresses for some of the higher endpoints
//                 (conversion between decimal/hex was wrong)
//               - The core now does properly evaluate the function address to
//                 determine if the packet was intended for it.
//               - Various other minor bugs and typos
//
//               Revision 1.1  2001/08/03 05:30:09  rudi
//
//
//               1) Reorganized directory structure
//
//               Revision 1.1  2001/03/31 12:45:13  rudi
//
//               This is the endpoint register file for non existing endpoints. It will be used for
//               endpoints that are commented out in the usd_defines.v file.
//               It will terminate all outputs to a known good level ...
//
//
//

`include "usbf_defines.v"

// Endpoint register File
module usbf_ep_rf_dummy(
		clk, wclk, rst,

		// Wishbone Interface
		adr, re, we, din, dout, inta, intb,
		dma_req, dma_ack,

		// Internal Interface

		idin,
		ep_sel, ep_match,
		buf0_rl, buf0_set, buf1_set,
		uc_bsel_set, uc_dpd_set,

		int_buf1_set, int_buf0_set, int_upid_set,
		int_crc16_set, int_to_set, int_seqerr_set,
		out_to_small,

		csr, buf0, buf1, dma_in_buf_sz1, dma_out_buf_avail
		);

input		clk, wclk, rst;
input	[1:0]	adr;
input		re;
input		we;
input	[31:0]	din;
output	[31:0]	dout;
output		inta, intb;
output		dma_req;
input		dma_ack;

input	[31:0]	idin;		// Data Input
input	[3:0]	ep_sel;		// Endpoint Number Input
output		ep_match;	// Asserted to indicate a ep no is matched
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
input		int_seqerr_set;	// Set PID sequence error interrupt
input		out_to_small;	// OUT packet was to small for DMA operation

output	[31:0]	csr;		// Internal CSR Output
output	[31:0]	buf0;		// Internal Buf 0 Output
output	[31:0]	buf1;		// Internal Buf 1 Output
output		dma_in_buf_sz1;	// Indicates that the DMA IN buffer has 1 max_pl_sz
				// packet available
output		dma_out_buf_avail;// Indicates that there is space for at least
				// one MAX_PL_SZ packet in the buffer

///////////////////////////////////////////////////////////////////
//
// Internal Access
//

assign	dout = 32'h0;
assign	inta = 1'b0;
assign	intb = 1'b0;
assign	dma_req = 1'b0;
assign	ep_match = 1'b0;
assign	csr = 32'h0;
assign	buf0 = 32'hffff_ffff;
assign	buf1 = 32'hffff_ffff;
assign	dma_in_buf_sz1 = 1'b0;
assign	dma_out_buf_avail = 1'b0;

endmodule

