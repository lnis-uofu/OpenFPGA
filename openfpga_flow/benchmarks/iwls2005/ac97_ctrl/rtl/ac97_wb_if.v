/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller                                  ////
////  WISHBONE Interface Module                                  ////
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
//  $Id: ac97_wb_if.v,v 1.4 2002/09/19 06:30:56 rudi Exp $
//
//  $Date: 2002/09/19 06:30:56 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_wb_if.v,v $
//               Revision 1.4  2002/09/19 06:30:56  rudi
//               Fixed a bug reported by Igor. Apparently this bug only shows up when
//               the WB clock is very low (2x bit_clk). Updated Copyright header.
//
//               Revision 1.3  2002/03/05 04:44:05  rudi
//
//               - Fixed the order of the thrash hold bits to match the spec.
//               - Many minor synthesis cleanup items ...
//
//               Revision 1.2  2001/08/10 08:09:42  rudi
//
//               - Removed RTY_O output.
//               - Added Clock and Reset Inputs to documentation.
//               - Changed IO names to be more clear.
//               - Uniquifyed define names to be core specific.
//
//               Revision 1.1  2001/08/03 06:54:50  rudi
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

module ac97_wb_if(clk, rst,

		wb_data_i, wb_data_o, wb_addr_i, wb_sel_i, wb_we_i, wb_cyc_i,
		wb_stb_i, wb_ack_o, wb_err_o, 

		adr, dout, rf_din, i3_din, i4_din, i6_din,
		rf_we, rf_re, o3_we, o4_we, o6_we, o7_we, o8_we, o9_we,
		i3_re, i4_re, i6_re

		);

input		clk,rst;

// WISHBONE Interface
input	[31:0]	wb_data_i;
output	[31:0]	wb_data_o;
input	[31:0]	wb_addr_i;
input	[3:0]	wb_sel_i;
input		wb_we_i;
input		wb_cyc_i;
input		wb_stb_i;
output		wb_ack_o;
output		wb_err_o;

// Internal Interface
output	[3:0]	adr;
output	[31:0]	dout;
input	[31:0]	rf_din, i3_din, i4_din, i6_din;
output		rf_we;
output		rf_re;
output		o3_we, o4_we, o6_we, o7_we, o8_we, o9_we;
output		i3_re, i4_re, i6_re;

////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg	[31:0]	wb_data_o;
reg	[31:0]	dout;
reg		wb_ack_o;

reg		rf_we;
reg		o3_we, o4_we, o6_we, o7_we, o8_we, o9_we;
reg		i3_re, i4_re, i6_re;

reg		we1, we2;
wire		we;
reg		re2, re1;
wire		re;

////////////////////////////////////////////////////////////////////
//
// Modules
//

assign adr = wb_addr_i[5:2];

assign wb_err_o = 1'b0;

always @(posedge clk)
	dout <= #1 wb_data_i;

always @(posedge clk)
	case(wb_addr_i[6:2])	// synopsys parallel_case full_case
	   5'he: wb_data_o <= #1 i3_din;
	   5'hf: wb_data_o <= #1 i4_din;
	   5'h10: wb_data_o <= #1 i6_din;
	   default: wb_data_o <= #1 rf_din;
	endcase

always @(posedge clk)
	re1 <= #1 !re2 & wb_cyc_i & wb_stb_i & !wb_we_i & `AC97_REG_SEL;

always @(posedge clk)
	re2 <= #1 re & wb_cyc_i & wb_stb_i & !wb_we_i ;

assign re = re1 & !re2 & wb_cyc_i & wb_stb_i & !wb_we_i;

assign rf_re = re & (wb_addr_i[6:2] < 5'h8);

always @(posedge clk)
	we1 <= #1 !we & wb_cyc_i & wb_stb_i & wb_we_i & `AC97_REG_SEL;

always @(posedge clk)
	we2 <= #1 we1 & wb_cyc_i & wb_stb_i & wb_we_i;

assign we = we1 & !we2 & wb_cyc_i & wb_stb_i & wb_we_i;

always @(posedge clk)
	wb_ack_o <= #1 (re | we) & wb_cyc_i & wb_stb_i & ~wb_ack_o;

always @(posedge clk)
	rf_we <= #1 we & (wb_addr_i[6:2] < 5'h8);

always @(posedge clk)
	o3_we <= #1 we & (wb_addr_i[6:2] == 5'h8);

always @(posedge clk)
	o4_we <= #1 we & (wb_addr_i[6:2] == 5'h9);

always @(posedge clk)
	o6_we <= #1 we & (wb_addr_i[6:2] == 5'ha);

always @(posedge clk)
	o7_we <= #1 we & (wb_addr_i[6:2] == 5'hb);

always @(posedge clk)
	o8_we <= #1 we & (wb_addr_i[6:2] == 5'hc);

always @(posedge clk)
	o9_we <= #1 we & (wb_addr_i[6:2] == 5'hd);

always @(posedge clk)
	i3_re <= #1 re & (wb_addr_i[6:2] == 5'he);

always @(posedge clk)
	i4_re <= #1 re & (wb_addr_i[6:2] == 5'hf);

always @(posedge clk)
	i6_re <= #1 re & (wb_addr_i[6:2] == 5'h10);

endmodule
