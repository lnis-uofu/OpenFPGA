/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller                                  ////
////  DMA Request Module                                         ////
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
//  $Id: ac97_dma_req.v,v 1.3 2002/09/19 06:30:56 rudi Exp $
//
//  $Date: 2002/09/19 06:30:56 $
//  $Revision: 1.3 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_dma_req.v,v $
//               Revision 1.3  2002/09/19 06:30:56  rudi
//               Fixed a bug reported by Igor. Apparently this bug only shows up when
//               the WB clock is very low (2x bit_clk). Updated Copyright header.
//
//               Revision 1.2  2002/03/05 04:44:05  rudi
//
//               - Fixed the order of the thrash hold bits to match the spec.
//               - Many minor synthesis cleanup items ...
//
//               Revision 1.1  2001/08/03 06:54:49  rudi
//
//
//               - Changed to new directory structure
//
//               Revision 1.1.1.1  2001/05/19 02:29:16  rudi
//               Initial Checkin
//
//
//
//

`include "ac97_defines.v"

module ac97_dma_req(clk, rst, cfg, status, full_empty, dma_req, dma_ack);
input		clk, rst;
input	[7:0]	cfg;
input	[1:0]	status;
input		full_empty;
output		dma_req;
input		dma_ack;

////////////////////////////////////////////////////////////////////
//
// Local Wires
//
reg	dma_req_d;
reg	dma_req_r1;
reg	dma_req;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

always @(cfg or status or full_empty)
	case(cfg[5:4])	// synopsys parallel_case full_case
			// REQ = Ch_EN & DMA_EN & Status
			// 1/4 full/empty
	   2'h2: dma_req_d = cfg[0] & cfg[6] & (full_empty | (status == 2'h0));
			// 1/2 full/empty
	   2'h1: dma_req_d = cfg[0] & cfg[6] & (full_empty | (status[1] == 1'h0));
			// 3/4 full/empty
	   2'h0: dma_req_d = cfg[0] & cfg[6] & (full_empty | (status < 2'h3));
	   2'h3: dma_req_d = cfg[0] & cfg[6] & full_empty;
	endcase

always @(posedge clk)
	dma_req_r1 <= #1 dma_req_d & !dma_ack;

always @(posedge clk or negedge rst)
	if(!rst)				dma_req <= #1 1'b0;
	else
	if(dma_req_r1 & dma_req_d & !dma_ack) 	dma_req <= #1 1'b1;
	else
	if(dma_ack) 				dma_req <= #1 1'b0;

endmodule

