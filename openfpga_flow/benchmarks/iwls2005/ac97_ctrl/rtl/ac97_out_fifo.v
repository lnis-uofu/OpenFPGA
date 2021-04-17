/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller                                  ////
////  Output FIFO                                                ////
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
//  $Id: ac97_out_fifo.v,v 1.4 2002/09/19 06:30:56 rudi Exp $
//
//  $Date: 2002/09/19 06:30:56 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_out_fifo.v,v $
//               Revision 1.4  2002/09/19 06:30:56  rudi
//               Fixed a bug reported by Igor. Apparently this bug only shows up when
//               the WB clock is very low (2x bit_clk). Updated Copyright header.
//
//               Revision 1.3  2002/03/11 03:21:22  rudi
//
//               - Added defines to select fifo depth between 4, 8 and 16 entries.
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

`ifdef AC97_OUT_FIFO_DEPTH_4

// 4 Entry Deep version of the Output FIFO

module ac97_out_fifo(clk, rst, en, mode, din, we, dout, re, status, full, empty);

input		clk, rst;
input		en;
input	[1:0]	mode;
input	[31:0]	din;
input		we;
output	[19:0]	dout;
input		re;
output	[1:0]	status;
output		full;
output		empty;


////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg	[31:0]	mem[0:3];

reg	[2:0]	wp;
reg	[3:0]	rp;

wire	[2:0]	wp_p1;

reg	[1:0]	status;
reg	[19:0]	dout;
wire	[31:0]	dout_tmp;
wire	[15:0]	dout_tmp1;
wire		m16b;
reg		empty;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

assign m16b = (mode == 2'h0);	// 16 Bit Mode

always @(posedge clk)
	if(!en)		wp <= #1 3'h0;
	else
	if(we)		wp <= #1 wp_p1;

assign wp_p1 = wp + 3'h1;

always @(posedge clk)
	if(!en)		rp <= #1 4'h0;
	else
	if(re & m16b)	rp <= #1 rp + 4'h1;
	else
	if(re & !m16b)	rp <= #1 rp + 4'h2;

always @(posedge clk)
	status <= #1 (wp[1:0] - rp[2:1]) - 2'h1;

wire	[3:0]	rp_p1 = rp[3:0] + 4'h1;

always @(posedge clk)
	empty <= #1 (rp_p1[3:1] == wp[2:0]) & (m16b ? rp_p1[0] : 1'b1);

assign full  = (wp[1:0] == rp[2:1]) & (wp[2] != rp[3]);

// Fifo Output
assign dout_tmp = mem[ rp[2:1] ];

// Fifo Output Half Word Select
assign dout_tmp1 = rp[0] ? dout_tmp[31:16] : dout_tmp[15:0];

always @(posedge clk)
	if(!en)		dout <= #1 20'h0;
	else
	if(re)
		case(mode)	// synopsys parallel_case full_case
		   2'h0: dout <= #1 {dout_tmp1, 4'h0};		// 16 Bit Output
		   2'h1: dout <= #1 {dout_tmp[17:0], 2'h0};	// 18 bit Output
		   2'h2: dout <= #1 dout_tmp[19:0];		// 20 Bit Output
		endcase

always @(posedge clk)
	if(we)	mem[wp[1:0]] <= #1 din;

endmodule

`endif

`ifdef AC97_OUT_FIFO_DEPTH_8

// 8 Entry Deep version of the Output FIFO

module ac97_out_fifo(clk, rst, en, mode, din, we, dout, re, status, full, empty);

input		clk, rst;
input		en;
input	[1:0]	mode;
input	[31:0]	din;
input		we;
output	[19:0]	dout;
input		re;
output	[1:0]	status;
output		full;
output		empty;


////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg	[31:0]	mem[0:7];

reg	[3:0]	wp;
reg	[4:0]	rp;

wire	[3:0]	wp_p1;

reg	[1:0]	status;
reg	[19:0]	dout;
wire	[31:0]	dout_tmp;
wire	[15:0]	dout_tmp1;
wire		m16b;
reg		empty;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

assign m16b = (mode == 2'h0);	// 16 Bit Mode

always @(posedge clk)
	if(!en)		wp <= #1 4'h0;
	else
	if(we)		wp <= #1 wp_p1;

assign wp_p1 = wp + 4'h1;

always @(posedge clk)
	if(!en)		rp <= #1 5'h0;
	else
	if(re & m16b)	rp <= #1 rp + 5'h1;
	else
	if(re & !m16b)	rp <= #1 rp + 5'h2;

always @(posedge clk)
	status <= #1 (wp[2:1] - rp[3:2]) - 2'h1;

wire	[4:0]	rp_p1 = rp[4:0] + 5'h1;

always @(posedge clk)
	empty <= #1 (rp_p1[4:1] == wp[3:0]) & (m16b ? rp_p1[0] : 1'b1);

assign full  = (wp[2:0] == rp[3:1]) & (wp[3] != rp[4]);

// Fifo Output
assign dout_tmp = mem[ rp[3:1] ];

// Fifo Output Half Word Select
assign dout_tmp1 = rp[0] ? dout_tmp[31:16] : dout_tmp[15:0];

always @(posedge clk)
	if(!en)		dout <= #1 20'h0;
	else
	if(re)
		case(mode)	// synopsys parallel_case full_case
		   2'h0: dout <= #1 {dout_tmp1, 4'h0};		// 16 Bit Output
		   2'h1: dout <= #1 {dout_tmp[17:0], 2'h0};	// 18 bit Output
		   2'h2: dout <= #1 dout_tmp[19:0];		// 20 Bit Output
		endcase


always @(posedge clk)
	if(we)	mem[wp[2:0]] <= #1 din;

endmodule

`endif


`ifdef AC97_OUT_FIFO_DEPTH_16

// 16 Entry Deep version of the Output FIFO

module ac97_out_fifo(clk, rst, en, mode, din, we, dout, re, status, full, empty);

input		clk, rst;
input		en;
input	[1:0]	mode;
input	[31:0]	din;
input		we;
output	[19:0]	dout;
input		re;
output	[1:0]	status;
output		full;
output		empty;


////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg	[31:0]	mem[0:15];

reg	[4:0]	wp;
reg	[5:0]	rp;

wire	[4:0]	wp_p1;

reg	[1:0]	status;
reg	[19:0]	dout;
wire	[31:0]	dout_tmp;
wire	[15:0]	dout_tmp1;
wire		m16b;
reg		empty;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

assign m16b = (mode == 2'h0);	// 16 Bit Mode

always @(posedge clk)
	if(!en)		wp <= #1 5'h0;
	else
	if(we)		wp <= #1 wp_p1;

assign wp_p1 = wp + 4'h1;

always @(posedge clk)
	if(!en)		rp <= #1 6'h0;
	else
	if(re & m16b)	rp <= #1 rp + 6'h1;
	else
	if(re & !m16b)	rp <= #1 rp + 6'h2;

always @(posedge clk)
	status <= #1 (wp[3:2] - rp[4:3]) - 2'h1;

wire	[5:0]	rp_p1 = rp[5:0] + 6'h1;

always @(posedge clk)
	empty <= #1 (rp_p1[5:1] == wp[4:0]) & (m16b ? rp_p1[0] : 1'b1);

assign full  = (wp[3:0] == rp[4:1]) & (wp[4] != rp[5]);

// Fifo Output
assign dout_tmp = mem[ rp[4:1] ];

// Fifo Output Half Word Select
assign dout_tmp1 = rp[0] ? dout_tmp[31:16] : dout_tmp[15:0];

always @(posedge clk)
	if(!en)		dout <= #1 20'h0;
	else
	if(re)
		case(mode)	// synopsys parallel_case full_case
		   2'h0: dout <= #1 {dout_tmp1, 4'h0};		// 16 Bit Output
		   2'h1: dout <= #1 {dout_tmp[17:0], 2'h0};	// 18 bit Output
		   2'h2: dout <= #1 dout_tmp[19:0];		// 20 Bit Output
		endcase


always @(posedge clk)
	if(we)	mem[wp[3:0]] <= #1 din;

endmodule

`endif
