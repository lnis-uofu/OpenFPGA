/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller                                  ////
////  Codec Register Access Module                               ////
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
//  $Id: ac97_cra.v,v 1.3 2002/09/19 06:30:56 rudi Exp $
//
//  $Date: 2002/09/19 06:30:56 $
//  $Revision: 1.3 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_cra.v,v $
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

module ac97_cra(clk, rst,

		crac_we, crac_din, crac_out,
		crac_wr_done, crac_rd_done,

		valid, out_slt1, out_slt2,
		in_slt2,

		crac_valid, crac_wr
		);

input		clk, rst;
input		crac_we;
output	[15:0]	crac_din;
input	[31:0]	crac_out;
output		crac_wr_done, crac_rd_done;

input		valid;
output	[19:0]	out_slt1;
output	[19:0]	out_slt2;
input	[19:0]	in_slt2;

output		crac_valid;
output		crac_wr;


////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg		crac_wr;
reg		crac_rd;
reg		crac_rd_done;
reg	[15:0]	crac_din;
reg		crac_we_r;
reg		valid_r;
wire		valid_ne;
wire		valid_pe;
reg		rdd1, rdd2, rdd3;

////////////////////////////////////////////////////////////////////
//
// Codec Register Data Path
//

// Control
assign out_slt1[19]    = crac_out[31];
assign out_slt1[18:12] = crac_out[22:16];
assign out_slt1[11:0]  = 12'h0;

// Write Data
assign out_slt2[19:4] = crac_out[15:0];
assign out_slt2[3:0] = 4'h0;

// Read Data
always @(posedge clk or negedge rst)
   begin
	if(!rst)		crac_din <= #1 16'h0;
	else
	if(crac_rd_done)	crac_din <= #1 in_slt2[19:4];
   end

////////////////////////////////////////////////////////////////////
//
// Codec Register Access Tracking
//

assign crac_valid = crac_wr | crac_rd;

always @(posedge clk)
	crac_we_r <= #1 crac_we;

always @(posedge clk or negedge rst)
	if(!rst)			crac_wr <= #1 1'b0;
	else
	if(crac_we_r & !crac_out[31])	crac_wr <= #1 1'b1;
	else
	if(valid_ne)			crac_wr <= #1 1'b0;

assign crac_wr_done = crac_wr & valid_ne;

always @(posedge clk or negedge rst)
	if(!rst)			crac_rd <= #1 1'b0;
	else
	if(crac_we_r & crac_out[31])	crac_rd <= #1 1'b1;
	else
	if(rdd1 & valid_pe)		crac_rd <= #1 1'b0;

always @(posedge clk or negedge rst)
	if(!rst)			rdd1 <= #1 1'b0;
	else
	if(crac_rd & valid_ne)		rdd1 <= #1 1'b1;
	else
	if(!crac_rd)			rdd1 <= #1 1'b0;

always @(posedge clk or negedge rst)
	if(!rst)					rdd2 <= #1 1'b0;
	else
	if( (crac_rd & valid_ne) | (!rdd3 & rdd2) )	rdd2 <= #1 1'b1;
	else
	if(crac_rd_done)				rdd2 <= #1 1'b0;

always @(posedge clk or negedge rst)
	if(!rst)			rdd3 <= #1 1'b0;
	else
	if(rdd2 & valid_pe)		rdd3 <= #1 1'b1;
	else
	if(crac_rd_done)		rdd3 <= #1 1'b0;

always @(posedge clk)
	crac_rd_done <= #1 rdd3 & valid_pe;

always @(posedge clk)
	valid_r <= #1 valid;

assign valid_ne = !valid & valid_r;

assign valid_pe = valid & !valid_r;

endmodule
