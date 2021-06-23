/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller                                  ////
////  Serial Input Block                                         ////
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
//  $Id: ac97_sin.v,v 1.2 2002/09/19 06:30:56 rudi Exp $
//
//  $Date: 2002/09/19 06:30:56 $
//  $Revision: 1.2 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_sin.v,v $
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

module ac97_sin(clk, rst,

	out_le, slt0, slt1, slt2, slt3, slt4,
	slt6, 

	sdata_in
	);

input		clk, rst;

// --------------------------------------
// Misc Signals
input	[5:0]	out_le;
output	[15:0]	slt0;
output	[19:0]	slt1;
output	[19:0]	slt2;
output	[19:0]	slt3;
output	[19:0]	slt4;
output	[19:0]	slt6;

// --------------------------------------
// AC97 Codec Interface
input		sdata_in;

////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg		sdata_in_r;
reg	[19:0]	sr;

reg	[15:0]	slt0;
reg	[19:0]	slt1;
reg	[19:0]	slt2;
reg	[19:0]	slt3;
reg	[19:0]	slt4;
reg	[19:0]	slt6;

////////////////////////////////////////////////////////////////////
//
// Output Registers
//

always @(posedge clk)
	if(out_le[0])	slt0 <= #1 sr[15:0];

always @(posedge clk)
	if(out_le[1])	slt1 <= #1 sr;

always @(posedge clk)
	if(out_le[2])	slt2 <= #1 sr;

always @(posedge clk)
	if(out_le[3])	slt3 <= #1 sr;

always @(posedge clk)
	if(out_le[4])	slt4 <= #1 sr;

always @(posedge clk)
	if(out_le[5])	slt6 <= #1 sr;

////////////////////////////////////////////////////////////////////
//
// Serial Shift Register
//

always @(negedge clk)
	sdata_in_r <= #1 sdata_in;

always @(posedge clk)
	sr <= #1 {sr[18:0], sdata_in_r };

endmodule


