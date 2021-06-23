/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE DMA Priority Encoder                              ////
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
//  $Id: wb_dma_ch_pri_enc.v,v 1.5 2002/02/01 01:54:44 rudi Exp $
//
//  $Date: 2002/02/01 01:54:44 $
//  $Revision: 1.5 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: wb_dma_ch_pri_enc.v,v $
//               Revision 1.5  2002/02/01 01:54:44  rudi
//
//               - Minor cleanup
//
//               Revision 1.4  2001/10/19 04:35:04  rudi
//
//               - Made the core parameterized
//
//               Revision 1.3  2001/08/15 05:40:30  rudi
//
//               - Changed IO names to be more clear.
//               - Uniquifyed define names to be core specific.
//               - Added Section 3.10, describing DMA restart.
//
//               Revision 1.2  2001/08/07 08:00:43  rudi
//
//
//               Split up priority encoder modules to separate files
//
//               Revision 1.1  2001/07/29 08:57:02  rudi
//
//
//               1) Changed Directory Structure
//               2) Added restart signal (REST)
//
//               Revision 1.2  2001/06/05 10:22:36  rudi
//
//
//               - Added Support of up to 31 channels
//               - Added support for 2,4 and 8 priority levels
//               - Now can have up to 31 channels
//               - Added many configuration items
//               - Changed reset to async
//
//               Revision 1.1.1.1  2001/03/19 13:10:50  rudi
//               Initial Release
//
//
//

`include "wb_dma_defines.v"

// Priority Encoder
//
// Determines the channel with the highest priority, also takes
// the valid bit in consideration

module wb_dma_ch_pri_enc(clk, valid,
		pri0, pri1, pri2, pri3,
		pri4, pri5, pri6, pri7,
		pri8, pri9, pri10, pri11,
		pri12, pri13, pri14, pri15,
		pri16, pri17, pri18, pri19,
		pri20, pri21, pri22, pri23,
		pri24, pri25, pri26, pri27,
		pri28, pri29, pri30,
		pri_out);

////////////////////////////////////////////////////////////////////
//
// Module Parameters
//

// chXX_conf = { CBUF, ED, ARS, EN }
parameter	[1:0]	pri_sel = 2'd0;
parameter	[3:0]	ch0_conf = 4'h1;
parameter	[3:0]	ch1_conf = 4'h0;
parameter	[3:0]	ch2_conf = 4'h0;
parameter	[3:0]	ch3_conf = 4'h0;
parameter	[3:0]	ch4_conf = 4'h0;
parameter	[3:0]	ch5_conf = 4'h0;
parameter	[3:0]	ch6_conf = 4'h0;
parameter	[3:0]	ch7_conf = 4'h0;
parameter	[3:0]	ch8_conf = 4'h0;
parameter	[3:0]	ch9_conf = 4'h0;
parameter	[3:0]	ch10_conf = 4'h0;
parameter	[3:0]	ch11_conf = 4'h0;
parameter	[3:0]	ch12_conf = 4'h0;
parameter	[3:0]	ch13_conf = 4'h0;
parameter	[3:0]	ch14_conf = 4'h0;
parameter	[3:0]	ch15_conf = 4'h0;
parameter	[3:0]	ch16_conf = 4'h0;
parameter	[3:0]	ch17_conf = 4'h0;
parameter	[3:0]	ch18_conf = 4'h0;
parameter	[3:0]	ch19_conf = 4'h0;
parameter	[3:0]	ch20_conf = 4'h0;
parameter	[3:0]	ch21_conf = 4'h0;
parameter	[3:0]	ch22_conf = 4'h0;
parameter	[3:0]	ch23_conf = 4'h0;
parameter	[3:0]	ch24_conf = 4'h0;
parameter	[3:0]	ch25_conf = 4'h0;
parameter	[3:0]	ch26_conf = 4'h0;
parameter	[3:0]	ch27_conf = 4'h0;
parameter	[3:0]	ch28_conf = 4'h0;
parameter	[3:0]	ch29_conf = 4'h0;
parameter	[3:0]	ch30_conf = 4'h0;

////////////////////////////////////////////////////////////////////
//
// Module IOs
//

input		clk;
input	[30:0]	valid;				// Channel Valid bits
input	[2:0]	pri0, pri1, pri2, pri3;		// Channel Priorities
input	[2:0]	pri4, pri5, pri6, pri7;
input	[2:0]	pri8, pri9, pri10, pri11;
input	[2:0]	pri12, pri13, pri14, pri15;
input	[2:0]	pri16, pri17, pri18, pri19;
input	[2:0]	pri20, pri21, pri22, pri23;
input	[2:0]	pri24, pri25, pri26, pri27;
input	[2:0]	pri28, pri29, pri30;
output	[2:0]	pri_out;			// Highest unserviced priority

wire	[7:0]	pri0_out, pri1_out, pri2_out, pri3_out;
wire	[7:0]	pri4_out, pri5_out, pri6_out, pri7_out;
wire	[7:0]	pri8_out, pri9_out, pri10_out, pri11_out;
wire	[7:0]	pri12_out, pri13_out, pri14_out, pri15_out;
wire	[7:0]	pri16_out, pri17_out, pri18_out, pri19_out;
wire	[7:0]	pri20_out, pri21_out, pri22_out, pri23_out;
wire	[7:0]	pri24_out, pri25_out, pri26_out, pri27_out;
wire	[7:0]	pri28_out, pri29_out, pri30_out;

wire	[7:0]	pri_out_tmp;
reg	[2:0]	pri_out;
reg	[2:0]	pri_out2;
reg	[2:0]	pri_out1;
reg	[2:0]	pri_out0;

wb_dma_pri_enc_sub #(ch1_conf,pri_sel) u0(	// Use channel config 1 for channel 0 encoder
		.valid(		valid[0]	),
		.pri_in(	pri0		),
		.pri_out(	pri0_out	)
		);
wb_dma_pri_enc_sub #(ch1_conf,pri_sel) u1(
		.valid(		valid[1]	),
		.pri_in(	pri1		),
		.pri_out(	pri1_out	)
		);
wb_dma_pri_enc_sub #(ch2_conf,pri_sel) u2(
		.valid(		valid[2]	),
		.pri_in(	pri2		),
		.pri_out(	pri2_out	)
		);
wb_dma_pri_enc_sub #(ch3_conf,pri_sel) u3(
		.valid(		valid[3]	),
		.pri_in(	pri3		),
		.pri_out(	pri3_out	)
		);
wb_dma_pri_enc_sub #(ch4_conf,pri_sel) u4(
		.valid(		valid[4]	),
		.pri_in(	pri4		),
		.pri_out(	pri4_out	)
		);
wb_dma_pri_enc_sub #(ch5_conf,pri_sel) u5(
		.valid(		valid[5]	),
		.pri_in(	pri5		),
		.pri_out(	pri5_out	)
		);
wb_dma_pri_enc_sub #(ch6_conf,pri_sel) u6(
		.valid(		valid[6]	),
		.pri_in(	pri6		),
		.pri_out(	pri6_out	)
		);
wb_dma_pri_enc_sub #(ch7_conf,pri_sel) u7(
		.valid(		valid[7]	),
		.pri_in(	pri7		),
		.pri_out(	pri7_out	)
		);
wb_dma_pri_enc_sub #(ch8_conf,pri_sel) u8(
		.valid(		valid[8]	),
		.pri_in(	pri8		),
		.pri_out(	pri8_out	)
		);
wb_dma_pri_enc_sub #(ch9_conf,pri_sel) u9(
		.valid(		valid[9]	),
		.pri_in(	pri9		),
		.pri_out(	pri9_out	)
		);
wb_dma_pri_enc_sub #(ch10_conf,pri_sel) u10(
		.valid(		valid[10]	),
		.pri_in(	pri10		),
		.pri_out(	pri10_out	)
		);
wb_dma_pri_enc_sub #(ch11_conf,pri_sel) u11(
		.valid(		valid[11]	),
		.pri_in(	pri11		),
		.pri_out(	pri11_out	)
		);
wb_dma_pri_enc_sub #(ch12_conf,pri_sel) u12(
		.valid(		valid[12]	),
		.pri_in(	pri12		),
		.pri_out(	pri12_out	)
		);
wb_dma_pri_enc_sub #(ch13_conf,pri_sel) u13(
		.valid(		valid[13]	),
		.pri_in(	pri13		),
		.pri_out(	pri13_out	)
		);
wb_dma_pri_enc_sub #(ch14_conf,pri_sel) u14(
		.valid(		valid[14]	),
		.pri_in(	pri14		),
		.pri_out(	pri14_out	)
		);
wb_dma_pri_enc_sub #(ch15_conf,pri_sel) u15(
		.valid(		valid[15]	),
		.pri_in(	pri15		),
		.pri_out(	pri15_out	)
		);
wb_dma_pri_enc_sub #(ch16_conf,pri_sel) u16(
		.valid(		valid[16]	),
		.pri_in(	pri16		),
		.pri_out(	pri16_out	)
		);
wb_dma_pri_enc_sub #(ch17_conf,pri_sel) u17(
		.valid(		valid[17]	),
		.pri_in(	pri17		),
		.pri_out(	pri17_out	)
		);
wb_dma_pri_enc_sub #(ch18_conf,pri_sel) u18(
		.valid(		valid[18]	),
		.pri_in(	pri18		),
		.pri_out(	pri18_out	)
		);
wb_dma_pri_enc_sub #(ch19_conf,pri_sel) u19(
		.valid(		valid[19]	),
		.pri_in(	pri19		),
		.pri_out(	pri19_out	)
		);
wb_dma_pri_enc_sub #(ch20_conf,pri_sel) u20(
		.valid(		valid[20]	),
		.pri_in(	pri20		),
		.pri_out(	pri20_out	)
		);
wb_dma_pri_enc_sub #(ch21_conf,pri_sel) u21(
		.valid(		valid[21]	),
		.pri_in(	pri21		),
		.pri_out(	pri21_out	)
		);
wb_dma_pri_enc_sub #(ch22_conf,pri_sel) u22(
		.valid(		valid[22]	),
		.pri_in(	pri22		),
		.pri_out(	pri22_out	)
		);
wb_dma_pri_enc_sub #(ch23_conf,pri_sel) u23(
		.valid(		valid[23]	),
		.pri_in(	pri23		),
		.pri_out(	pri23_out	)
		);
wb_dma_pri_enc_sub #(ch24_conf,pri_sel) u24(
		.valid(		valid[24]	),
		.pri_in(	pri24		),
		.pri_out(	pri24_out	)
		);
wb_dma_pri_enc_sub #(ch25_conf,pri_sel) u25(
		.valid(		valid[25]	),
		.pri_in(	pri25		),
		.pri_out(	pri25_out	)
		);
wb_dma_pri_enc_sub #(ch26_conf,pri_sel) u26(
		.valid(		valid[26]	),
		.pri_in(	pri26		),
		.pri_out(	pri26_out	)
		);
wb_dma_pri_enc_sub #(ch27_conf,pri_sel) u27(
		.valid(		valid[27]	),
		.pri_in(	pri27		),
		.pri_out(	pri27_out	)
		);
wb_dma_pri_enc_sub #(ch28_conf,pri_sel) u28(
		.valid(		valid[28]	),
		.pri_in(	pri28		),
		.pri_out(	pri28_out	)
		);
wb_dma_pri_enc_sub #(ch29_conf,pri_sel) u29(
		.valid(		valid[29]	),
		.pri_in(	pri29		),
		.pri_out(	pri29_out	)
		);
wb_dma_pri_enc_sub #(ch30_conf,pri_sel) u30(
		.valid(		valid[30]	),
		.pri_in(	pri30		),
		.pri_out(	pri30_out	)
		);

assign pri_out_tmp =	pri0_out | pri1_out | pri2_out | pri3_out |
			pri4_out | pri5_out | pri6_out | pri7_out |
			pri8_out | pri9_out | pri10_out | pri11_out |
			pri12_out | pri13_out | pri14_out | pri15_out |
			pri16_out | pri17_out | pri18_out | pri19_out |
			pri20_out | pri21_out | pri22_out | pri23_out |
			pri24_out | pri25_out | pri26_out | pri27_out |
			pri28_out | pri29_out | pri30_out;

// 8 Priority Levels
always @(posedge clk)
	if(pri_out_tmp[7])	pri_out2 <= #1 3'h7;
	else
	if(pri_out_tmp[6])	pri_out2 <= #1 3'h6;
	else
	if(pri_out_tmp[5])	pri_out2 <= #1 3'h5;
	else
	if(pri_out_tmp[4])	pri_out2 <= #1 3'h4;
	else
	if(pri_out_tmp[3])	pri_out2 <= #1 3'h3;
	else
	if(pri_out_tmp[2])	pri_out2 <= #1 3'h2;
	else
	if(pri_out_tmp[1])	pri_out2 <= #1 3'h1;
	else			pri_out2 <= #1 3'h0;

// 4 Priority Levels
always @(posedge clk)
	if(pri_out_tmp[3])	pri_out1 <= #1 3'h3;
	else
	if(pri_out_tmp[2])	pri_out1 <= #1 3'h2;
	else
	if(pri_out_tmp[1])	pri_out1 <= #1 3'h1;
	else			pri_out1 <= #1 3'h0;

// 2 Priority Levels
always @(posedge clk)
	if(pri_out_tmp[1])	pri_out0 <= #1 3'h1;
	else			pri_out0 <= #1 3'h0;

// Select configured priority
always @(pri_sel or pri_out0 or pri_out1 or  pri_out2)
	case(pri_sel)		// synopsys parallel_case full_case
	   2'd0: pri_out = pri_out0;
	   2'd1: pri_out = pri_out1;
	   2'd2: pri_out = pri_out2;
	endcase

endmodule
