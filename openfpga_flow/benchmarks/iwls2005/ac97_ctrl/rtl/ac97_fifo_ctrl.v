/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller                                  ////
////  FIFO Control Module                                        ////
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
//  $Id: ac97_fifo_ctrl.v,v 1.3 2002/09/19 06:30:56 rudi Exp $
//
//  $Date: 2002/09/19 06:30:56 $
//  $Revision: 1.3 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_fifo_ctrl.v,v $
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
//               Revision 1.1.1.1  2001/05/19 02:29:18  rudi
//               Initial Checkin
//
//
//
//

`include "ac97_defines.v"

module ac97_fifo_ctrl(	clk, 
			valid, ch_en, srs, full_empty, req, crdy,
			en_out, en_out_l
			);
input		clk;
input		valid;
input		ch_en;		// Channel Enable
input		srs;		// Sample Rate Select
input		full_empty;	// Fifo Status
input		req;		// Codec Request
input		crdy;		// Codec Ready
output		en_out;		// Output read/write pulse
output		en_out_l;	// Latched Output

////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg	en_out_l, en_out_l2;
reg	full_empty_r;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

always @(posedge clk)
	if(!valid)	full_empty_r <= #1 full_empty;

always @(posedge clk)
	if(valid & ch_en & !full_empty_r & crdy & (!srs | (srs & req) ) )
		en_out_l <= #1 1'b1;
	else
	if(!valid & !(ch_en & !full_empty_r & crdy & (!srs | (srs & req) )) )
		en_out_l <= #1 1'b0;

always @(posedge clk)
	en_out_l2 <= #1 en_out_l & valid;

assign en_out = en_out_l & !en_out_l2 & valid;

endmodule
