/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE DMA Priority Encoder Sub-Module                   ////
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
//  $Id: wb_dma_pri_enc_sub.v,v 1.4 2002/02/01 01:54:45 rudi Exp $
//
//  $Date: 2002/02/01 01:54:45 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: wb_dma_pri_enc_sub.v,v $
//               Revision 1.4  2002/02/01 01:54:45  rudi
//
//               - Minor cleanup
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
//               Revision 1.1  2001/08/07 08:00:43  rudi
//
//
//               Split up priority encoder modules to separate files
//
//
//
//
//
//

`include "wb_dma_defines.v"

// Priority Encoder
//
// Determines the channel with the highest priority, also takes
// the valid bit in consideration

module wb_dma_pri_enc_sub(valid, pri_in, pri_out);

parameter [3:0]	ch_conf = 4'b0000;
parameter [1:0]	pri_sel = 2'd0;

input		valid;
input	[2:0]	pri_in;
output	[7:0]	pri_out;

wire	[7:0]	pri_out;
reg	[7:0]	pri_out_d;
reg	[7:0]	pri_out_d0;
reg	[7:0]	pri_out_d1;
reg	[7:0]	pri_out_d2;

assign pri_out = ch_conf[0] ? pri_out_d : 8'h0;

// Select Configured Priority
always @(pri_sel or pri_out_d0 or pri_out_d1 or  pri_out_d2)
	case(pri_sel)		// synopsys parallel_case full_case
	   2'd0: pri_out_d = pri_out_d0;
	   2'd1: pri_out_d = pri_out_d1;
	   2'd2: pri_out_d = pri_out_d2;
	endcase

// 8 Priority Levels
always @(valid or pri_in)
	if(!valid)		pri_out_d2 = 8'b0000_0001;
	else
	if(pri_in==3'h0)	pri_out_d2 = 8'b0000_0001;
	else
	if(pri_in==3'h1)	pri_out_d2 = 8'b0000_0010;
	else
	if(pri_in==3'h2)	pri_out_d2 = 8'b0000_0100;
	else
	if(pri_in==3'h3)	pri_out_d2 = 8'b0000_1000;
	else
	if(pri_in==3'h4)	pri_out_d2 = 8'b0001_0000;
	else
	if(pri_in==3'h5)	pri_out_d2 = 8'b0010_0000;
	else
	if(pri_in==3'h6)	pri_out_d2 = 8'b0100_0000;
	else			pri_out_d2 = 8'b1000_0000;

// 4 Priority Levels
always @(valid or pri_in)
	if(!valid)		pri_out_d1 = 8'b0000_0001;
	else
	if(pri_in==3'h0)	pri_out_d1 = 8'b0000_0001;
	else
	if(pri_in==3'h1)	pri_out_d1 = 8'b0000_0010;
	else
	if(pri_in==3'h2)	pri_out_d1 = 8'b0000_0100;
	else			pri_out_d1 = 8'b0000_1000;

// 2 Priority Levels
always @(valid or pri_in)
	if(!valid)		pri_out_d0 = 8'b0000_0001;
	else
	if(pri_in==3'h0)	pri_out_d0 = 8'b0000_0001;
	else			pri_out_d0 = 8'b0000_0010;

endmodule

