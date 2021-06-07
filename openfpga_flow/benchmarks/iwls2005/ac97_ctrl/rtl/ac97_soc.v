/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller                                  ////
////  Serial Output Controller                                   ////
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
//  $Id: ac97_soc.v,v 1.3 2002/09/19 06:30:56 rudi Exp $
//
//  $Date: 2002/09/19 06:30:56 $
//  $Revision: 1.3 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_soc.v,v $
//               Revision 1.3  2002/09/19 06:30:56  rudi
//               Fixed a bug reported by Igor. Apparently this bug only shows up when
//               the WB clock is very low (2x bit_clk). Updated Copyright header.
//
//               Revision 1.2  2002/03/05 04:44:05  rudi
//
//               - Fixed the order of the thrash hold bits to match the spec.
//               - Many minor synthesis cleanup items ...
//
//               Revision 1.1  2001/08/03 06:54:50  rudi
//
//
//               - Changed to new directory structure
//
//               Revision 1.1.1.1  2001/05/19 02:29:15  rudi
//               Initial Checkin
//
//
//
//

`include "ac97_defines.v"

module ac97_soc(clk, wclk, rst,
		ps_ce, resume, suspended,
		sync, out_le, in_valid, ld, valid
		);

input		clk, wclk, rst;
input		ps_ce;
input		resume;
output		suspended;
output		sync;
output	[5:0]	out_le;
output	[2:0]	in_valid;
output		ld;
output		valid;

////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg	[7:0]	cnt;
reg		sync_beat;
reg		sync_resume;
reg	[5:0]	out_le;
reg		ld;
reg		valid;
reg	[2:0]	in_valid;
reg		bit_clk_r;
reg		bit_clk_r1;
reg		bit_clk_e;
reg		suspended;
wire		to;
reg	[5:0]	to_cnt;
reg	[3:0]	res_cnt;
wire		resume_done;

assign sync = sync_beat | sync_resume;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

always @(posedge clk or negedge rst)
	if(!rst)		cnt <= #1 8'hff;
	else
	if(suspended)		cnt <= #1 8'hff;
	else			cnt <= #1 cnt + 8'h1;

always @(posedge clk)
	ld <= #1 (cnt == 8'h00);

always @(posedge clk)
	sync_beat <= #1 (cnt == 8'h00) | ((cnt > 8'h00) & (cnt < 8'h10));

always @(posedge clk)
	valid <= #1 (cnt > 8'h39);

always @(posedge clk)
	out_le[0] <= #1 (cnt == 8'h11);		// Slot 0 Latch Enable

always @(posedge clk)
	out_le[1] <= #1 (cnt == 8'h25);		// Slot 1 Latch Enable

always @(posedge clk)
	out_le[2] <= #1 (cnt == 8'h39);		// Slot 2 Latch Enable

always @(posedge clk)
	out_le[3] <= #1 (cnt == 8'h4d);		// Slot 3 Latch Enable

always @(posedge clk)
	out_le[4] <= #1 (cnt == 8'h61);		// Slot 4 Latch Enable

always @(posedge clk)
	out_le[5] <= #1 (cnt == 8'h89);		// Slot 6 Latch Enable

always @(posedge clk)
	in_valid[0] <= #1 (cnt > 8'h4d);	// Input Slot 3 Valid

always @(posedge clk)
	in_valid[1] <= #1 (cnt > 8'h61);	// Input Slot 3 Valid

always @(posedge clk)
	in_valid[2] <= #1 (cnt > 8'h89);	// Input Slot 3 Valid

////////////////////////////////////////////////////////////////////
//
// Suspend Detect
//

always @(posedge wclk)
	bit_clk_r <= #1 clk;

always @(posedge wclk)
	bit_clk_r1 <= #1 bit_clk_r;

always @(posedge wclk)
	bit_clk_e <= #1 (bit_clk_r & !bit_clk_r1) | (!bit_clk_r & bit_clk_r1);

always @(posedge wclk)
	suspended <= #1 to;

assign to = (to_cnt == `AC97_SUSP_DET);

always @(posedge wclk or negedge rst)
	if(!rst)		to_cnt <= #1 6'h0;
	else
	if(bit_clk_e)		to_cnt <= #1 6'h0;
	else
	if(!to)			to_cnt <= #1 to_cnt + 6'h1;

////////////////////////////////////////////////////////////////////
//
// Resume Signaling
//

always @(posedge wclk or negedge rst)
	if(!rst)			sync_resume <= #1 1'b0;
	else
	if(resume_done)			sync_resume <= #1 1'b0;
	else
	if(suspended & resume)		sync_resume <= #1 1'b1;

assign resume_done = (res_cnt == `AC97_RES_SIG);

always @(posedge wclk)
	if(!sync_resume)	res_cnt <= #1 4'h0;
	else
	if(ps_ce)		res_cnt <= #1 res_cnt + 4'h1;

endmodule
