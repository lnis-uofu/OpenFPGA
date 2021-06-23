/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller                                  ////
////  Serial Output Block                                        ////
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
//  $Id: ac97_sout.v,v 1.2 2002/09/19 06:30:56 rudi Exp $
//
//  $Date: 2002/09/19 06:30:56 $
//  $Revision: 1.2 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_sout.v,v $
//               Revision 1.2  2002/09/19 06:30:56  rudi
//               Fixed a bug reported by Igor. Apparently this bug only shows up when
//               the WB clock is very low (2x bit_clk). Updated Copyright header.
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

module ac97_sout(clk, rst,

	so_ld, slt0, slt1, slt2, slt3, slt4,
	slt6, slt7, slt8, slt9,

	sdata_out
	);

input		clk, rst;

// --------------------------------------
// Misc Signals
input		so_ld;
input	[15:0]	slt0;
input	[19:0]	slt1;
input	[19:0]	slt2;
input	[19:0]	slt3;
input	[19:0]	slt4;
input	[19:0]	slt6;
input	[19:0]	slt7;
input	[19:0]	slt8;
input	[19:0]	slt9;

// --------------------------------------
// AC97 Codec Interface
output		sdata_out;

////////////////////////////////////////////////////////////////////
//
// Local Wires
//

wire		sdata_out;

reg	[15:0]	slt0_r;
reg	[19:0]	slt1_r;
reg	[19:0]	slt2_r;
reg	[19:0]	slt3_r;
reg	[19:0]	slt4_r;
reg	[19:0]	slt5_r;
reg	[19:0]	slt6_r;
reg	[19:0]	slt7_r;
reg	[19:0]	slt8_r;
reg	[19:0]	slt9_r;
reg	[19:0]	slt10_r;
reg	[19:0]	slt11_r;
reg	[19:0]	slt12_r;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

////////////////////////////////////////////////////////////////////
//
// Serial Shift Register
//

assign	sdata_out = slt0_r[15];

always @(posedge clk)
	if(so_ld)	slt0_r <= #1 slt0;
	else		slt0_r <= #1 {slt0_r[14:0], slt1_r[19]};

always @(posedge clk)
	if(so_ld)	slt1_r <= #1 slt1;
	else		slt1_r <= #1 {slt1_r[18:0], slt2_r[19]};

always @(posedge clk)
	if(so_ld)	slt2_r <= #1 slt2;
	else		slt2_r <= #1 {slt2_r[18:0], slt3_r[19]};

always @(posedge clk)
	if(so_ld)	slt3_r <= #1 slt3;
	else		slt3_r <= #1 {slt3_r[18:0], slt4_r[19]};

always @(posedge clk)
	if(so_ld)	slt4_r <= #1 slt4;
	else		slt4_r <= #1 {slt4_r[18:0], slt5_r[19]};

always @(posedge clk)
	if(so_ld)	slt5_r <= #1 20'h0;
	else		slt5_r <= #1 {slt5_r[18:0], slt6_r[19]};

always @(posedge clk)
	if(so_ld)	slt6_r <= #1 slt6;
	else		slt6_r <= #1 {slt6_r[18:0], slt7_r[19]};

always @(posedge clk)
	if(so_ld)	slt7_r <= #1 slt7;
	else		slt7_r <= #1 {slt7_r[18:0], slt8_r[19]};

always @(posedge clk)
	if(so_ld)	slt8_r <= #1 slt8;
	else		slt8_r <= #1 {slt8_r[18:0], slt9_r[19]};

always @(posedge clk)
	if(so_ld)	slt9_r <= #1 slt9;
	else		slt9_r <= #1 {slt9_r[18:0], slt10_r[19]};

always @(posedge clk)
	if(so_ld)	slt10_r <= #1 20'h0;
	else		slt10_r <= #1 {slt10_r[18:0], slt11_r[19]};

always @(posedge clk)
	if(so_ld)	slt11_r <= #1 20'h0;
	else		slt11_r <= #1 {slt11_r[18:0], slt12_r[19]};

always @(posedge clk)
	if(so_ld)	slt12_r <= #1 20'h0;
	else		slt12_r <= #1 {slt12_r[18:0], 1'b0 };

endmodule

