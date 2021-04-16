/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE DMA Channel Select                                ////
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
//  $Id: wb_dma_ch_sel.v,v 1.4 2002/02/01 01:54:45 rudi Exp $
//
//  $Date: 2002/02/01 01:54:45 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: wb_dma_ch_sel.v,v $
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
//               Revision 1.1  2001/07/29 08:57:02  rudi
//
//
//               1) Changed Directory Structure
//               2) Added restart signal (REST)
//
//               Revision 1.4  2001/06/14 08:52:00  rudi
//
//
//               Changed arbiter module name.
//
//               Revision 1.3  2001/06/13 02:26:48  rudi
//
//
//               Small changes after running lint.
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
//               Revision 1.1.1.1  2001/03/19 13:10:35  rudi
//               Initial Release
//
//
//

`include "wb_dma_defines.v"

module wb_dma_ch_sel(clk, rst,

	// DMA Request Lines
	req_i, ack_o, nd_i,

	// DMA Registers Inputs
	pointer0, pointer0_s, ch0_csr, ch0_txsz, ch0_adr0, ch0_adr1, ch0_am0, ch0_am1,
	pointer1, pointer1_s, ch1_csr, ch1_txsz, ch1_adr0, ch1_adr1, ch1_am0, ch1_am1,
	pointer2, pointer2_s, ch2_csr, ch2_txsz, ch2_adr0, ch2_adr1, ch2_am0, ch2_am1,
	pointer3, pointer3_s, ch3_csr, ch3_txsz, ch3_adr0, ch3_adr1, ch3_am0, ch3_am1,
	pointer4, pointer4_s, ch4_csr, ch4_txsz, ch4_adr0, ch4_adr1, ch4_am0, ch4_am1,
	pointer5, pointer5_s, ch5_csr, ch5_txsz, ch5_adr0, ch5_adr1, ch5_am0, ch5_am1,
	pointer6, pointer6_s, ch6_csr, ch6_txsz, ch6_adr0, ch6_adr1, ch6_am0, ch6_am1,
	pointer7, pointer7_s, ch7_csr, ch7_txsz, ch7_adr0, ch7_adr1, ch7_am0, ch7_am1,
	pointer8, pointer8_s, ch8_csr, ch8_txsz, ch8_adr0, ch8_adr1, ch8_am0, ch8_am1,
	pointer9, pointer9_s, ch9_csr, ch9_txsz, ch9_adr0, ch9_adr1, ch9_am0, ch9_am1,
	pointer10, pointer10_s, ch10_csr, ch10_txsz, ch10_adr0, ch10_adr1, ch10_am0, ch10_am1,
	pointer11, pointer11_s, ch11_csr, ch11_txsz, ch11_adr0, ch11_adr1, ch11_am0, ch11_am1,
	pointer12, pointer12_s, ch12_csr, ch12_txsz, ch12_adr0, ch12_adr1, ch12_am0, ch12_am1,
	pointer13, pointer13_s, ch13_csr, ch13_txsz, ch13_adr0, ch13_adr1, ch13_am0, ch13_am1,
	pointer14, pointer14_s, ch14_csr, ch14_txsz, ch14_adr0, ch14_adr1, ch14_am0, ch14_am1,
	pointer15, pointer15_s, ch15_csr, ch15_txsz, ch15_adr0, ch15_adr1, ch15_am0, ch15_am1,
	pointer16, pointer16_s, ch16_csr, ch16_txsz, ch16_adr0, ch16_adr1, ch16_am0, ch16_am1,
	pointer17, pointer17_s, ch17_csr, ch17_txsz, ch17_adr0, ch17_adr1, ch17_am0, ch17_am1,
	pointer18, pointer18_s, ch18_csr, ch18_txsz, ch18_adr0, ch18_adr1, ch18_am0, ch18_am1,
	pointer19, pointer19_s, ch19_csr, ch19_txsz, ch19_adr0, ch19_adr1, ch19_am0, ch19_am1,
	pointer20, pointer20_s, ch20_csr, ch20_txsz, ch20_adr0, ch20_adr1, ch20_am0, ch20_am1,
	pointer21, pointer21_s, ch21_csr, ch21_txsz, ch21_adr0, ch21_adr1, ch21_am0, ch21_am1,
	pointer22, pointer22_s, ch22_csr, ch22_txsz, ch22_adr0, ch22_adr1, ch22_am0, ch22_am1,
	pointer23, pointer23_s, ch23_csr, ch23_txsz, ch23_adr0, ch23_adr1, ch23_am0, ch23_am1,
	pointer24, pointer24_s, ch24_csr, ch24_txsz, ch24_adr0, ch24_adr1, ch24_am0, ch24_am1,
	pointer25, pointer25_s, ch25_csr, ch25_txsz, ch25_adr0, ch25_adr1, ch25_am0, ch25_am1,
	pointer26, pointer26_s, ch26_csr, ch26_txsz, ch26_adr0, ch26_adr1, ch26_am0, ch26_am1,
	pointer27, pointer27_s, ch27_csr, ch27_txsz, ch27_adr0, ch27_adr1, ch27_am0, ch27_am1,
	pointer28, pointer28_s, ch28_csr, ch28_txsz, ch28_adr0, ch28_adr1, ch28_am0, ch28_am1,
	pointer29, pointer29_s, ch29_csr, ch29_txsz, ch29_adr0, ch29_adr1, ch29_am0, ch29_am1,
	pointer30, pointer30_s, ch30_csr, ch30_txsz, ch30_adr0, ch30_adr1, ch30_am0, ch30_am1,

	// DMA Registers Write Back Channel Select
	ch_sel, ndnr,

	// DMA Engine Interface
	de_start, ndr, csr, pointer, txsz, adr0, adr1, am0, am1,
	pointer_s, next_ch, de_ack, dma_busy
	);

////////////////////////////////////////////////////////////////////
//
// Module Parameters
//

// chXX_conf = { CBUF, ED, ARS, EN }
parameter	[1:0]	pri_sel  = 2'h0;
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

input		clk, rst;

// DMA Request Lines
input	[30:0]	req_i;
output	[30:0]	ack_o;
input	[30:0]	nd_i;

// Channel Registers Inputs
input	[31:0]	pointer0, pointer0_s, ch0_csr, ch0_txsz, ch0_adr0, ch0_adr1, ch0_am0, ch0_am1;
input	[31:0]	pointer1, pointer1_s, ch1_csr, ch1_txsz, ch1_adr0, ch1_adr1, ch1_am0, ch1_am1;
input	[31:0]	pointer2, pointer2_s, ch2_csr, ch2_txsz, ch2_adr0, ch2_adr1, ch2_am0, ch2_am1;
input	[31:0]	pointer3, pointer3_s, ch3_csr, ch3_txsz, ch3_adr0, ch3_adr1, ch3_am0, ch3_am1;
input	[31:0]	pointer4, pointer4_s, ch4_csr, ch4_txsz, ch4_adr0, ch4_adr1, ch4_am0, ch4_am1;
input	[31:0]	pointer5, pointer5_s, ch5_csr, ch5_txsz, ch5_adr0, ch5_adr1, ch5_am0, ch5_am1;
input	[31:0]	pointer6, pointer6_s, ch6_csr, ch6_txsz, ch6_adr0, ch6_adr1, ch6_am0, ch6_am1;
input	[31:0]	pointer7, pointer7_s, ch7_csr, ch7_txsz, ch7_adr0, ch7_adr1, ch7_am0, ch7_am1;
input	[31:0]	pointer8, pointer8_s, ch8_csr, ch8_txsz, ch8_adr0, ch8_adr1, ch8_am0, ch8_am1;
input	[31:0]	pointer9, pointer9_s, ch9_csr, ch9_txsz, ch9_adr0, ch9_adr1, ch9_am0, ch9_am1;
input	[31:0]	pointer10, pointer10_s, ch10_csr, ch10_txsz, ch10_adr0, ch10_adr1, ch10_am0, ch10_am1;
input	[31:0]	pointer11, pointer11_s, ch11_csr, ch11_txsz, ch11_adr0, ch11_adr1, ch11_am0, ch11_am1;
input	[31:0]	pointer12, pointer12_s, ch12_csr, ch12_txsz, ch12_adr0, ch12_adr1, ch12_am0, ch12_am1;
input	[31:0]	pointer13, pointer13_s, ch13_csr, ch13_txsz, ch13_adr0, ch13_adr1, ch13_am0, ch13_am1;
input	[31:0]	pointer14, pointer14_s, ch14_csr, ch14_txsz, ch14_adr0, ch14_adr1, ch14_am0, ch14_am1;
input	[31:0]	pointer15, pointer15_s, ch15_csr, ch15_txsz, ch15_adr0, ch15_adr1, ch15_am0, ch15_am1;
input	[31:0]	pointer16, pointer16_s, ch16_csr, ch16_txsz, ch16_adr0, ch16_adr1, ch16_am0, ch16_am1;
input	[31:0]	pointer17, pointer17_s, ch17_csr, ch17_txsz, ch17_adr0, ch17_adr1, ch17_am0, ch17_am1;
input	[31:0]	pointer18, pointer18_s, ch18_csr, ch18_txsz, ch18_adr0, ch18_adr1, ch18_am0, ch18_am1;
input	[31:0]	pointer19, pointer19_s, ch19_csr, ch19_txsz, ch19_adr0, ch19_adr1, ch19_am0, ch19_am1;
input	[31:0]	pointer20, pointer20_s, ch20_csr, ch20_txsz, ch20_adr0, ch20_adr1, ch20_am0, ch20_am1;
input	[31:0]	pointer21, pointer21_s, ch21_csr, ch21_txsz, ch21_adr0, ch21_adr1, ch21_am0, ch21_am1;
input	[31:0]	pointer22, pointer22_s, ch22_csr, ch22_txsz, ch22_adr0, ch22_adr1, ch22_am0, ch22_am1;
input	[31:0]	pointer23, pointer23_s, ch23_csr, ch23_txsz, ch23_adr0, ch23_adr1, ch23_am0, ch23_am1;
input	[31:0]	pointer24, pointer24_s, ch24_csr, ch24_txsz, ch24_adr0, ch24_adr1, ch24_am0, ch24_am1;
input	[31:0]	pointer25, pointer25_s, ch25_csr, ch25_txsz, ch25_adr0, ch25_adr1, ch25_am0, ch25_am1;
input	[31:0]	pointer26, pointer26_s, ch26_csr, ch26_txsz, ch26_adr0, ch26_adr1, ch26_am0, ch26_am1;
input	[31:0]	pointer27, pointer27_s, ch27_csr, ch27_txsz, ch27_adr0, ch27_adr1, ch27_am0, ch27_am1;
input	[31:0]	pointer28, pointer28_s, ch28_csr, ch28_txsz, ch28_adr0, ch28_adr1, ch28_am0, ch28_am1;
input	[31:0]	pointer29, pointer29_s, ch29_csr, ch29_txsz, ch29_adr0, ch29_adr1, ch29_am0, ch29_am1;
input	[31:0]	pointer30, pointer30_s, ch30_csr, ch30_txsz, ch30_adr0, ch30_adr1, ch30_am0, ch30_am1;

output	[4:0]	ch_sel;		// Write Back Channel Select
output	[30:0]	ndnr;		// Next Descriptor No Request

output		de_start;	// Start DMA Engine Indicator
output		ndr;		// Next Descriptor With Request (for current channel)
output	[31:0]	csr;		// Selected Channel CSR
output	[31:0]	pointer;	// LL Descriptor pointer
output	[31:0]	pointer_s;	// LL Descriptor previous pointer
output	[31:0]	txsz;		// Selected Channel Transfer Size
output	[31:0]	adr0, adr1;	// Selected Channel Addresses
output	[31:0]	am0, am1;	// Selected Channel Address Masks

input		next_ch;	// Indicates the DMA Engine is done
				// with current transfer
input		de_ack;		// DMA engine ack output

input		dma_busy;

////////////////////////////////////////////////////////////////////
//
// Local Wires and Registers
//

reg	[30:0]	ack_o;
wire	[30:0]	valid;		// Indicates which channel is valid
reg		valid_sel;
reg	[30:0]	req_r;		// Channel Request inputs
reg	[30:0]	ndr_r;		// Next Descriptor Registered (and Request)
reg	[30:0]	ndnr;		// Next Descriptor Registered (and Not Request)
wire	[2:0]	pri_out;	// Highest unserviced priority
wire	[2:0]	pri0, pri1, pri2, pri3;		// Channel Priorities
wire	[2:0]	pri4, pri5, pri6, pri7;
wire	[2:0]	pri8, pri9, pri10, pri11;
wire	[2:0]	pri12, pri13, pri14, pri15;
wire	[2:0]	pri16, pri17, pri18, pri19;
wire	[2:0]	pri20, pri21, pri22, pri23;
wire	[2:0]	pri24, pri25, pri26, pri27;
wire	[2:0]	pri28, pri29, pri30;
reg	[4:0]	ch_sel_d;
reg	[4:0]	ch_sel_r;

reg		ndr;
reg		next_start;
reg		de_start_r;
reg	[31:0]	csr;		// Selected Channel CSR
reg	[31:0]	pointer;
reg	[31:0]	pointer_s;
reg	[31:0]	txsz;		// Selected Channel Transfer Size
reg	[31:0]	adr0, adr1;	// Selected Channel Addresses
reg	[31:0]	am0, am1;	// Selected Channel Address Masks

				// Arbiter Request Inputs
wire	[30:0]	req_p0, req_p1, req_p2, req_p3;
wire	[30:0]	req_p4, req_p5, req_p6, req_p7;
wire	[30:0]	req_p8, req_p9, req_p10, req_p11;
wire	[30:0]	req_p12, req_p13, req_p14, req_p15;
wire	[30:0]	req_p16, req_p17, req_p18, req_p19;
wire	[30:0]	req_p20, req_p21, req_p22, req_p23;
wire	[30:0]	req_p24, req_p25, req_p26, req_p27;
wire	[30:0]	req_p28, req_p29, req_p30;
				// Arbiter Grant Outputs
wire	[4:0]	gnt_p0_d, gnt_p1_d, gnt_p2_d, gnt_p3_d;
wire	[4:0]	gnt_p4_d, gnt_p5_d, gnt_p6_d, gnt_p7_d;
wire	[4:0]	gnt_p0, gnt_p1, gnt_p2, gnt_p3;
wire	[4:0]	gnt_p4, gnt_p5, gnt_p6, gnt_p7;
wire	[4:0]	gnt_p8, gnt_p9, gnt_p10, gnt_p11;
wire	[4:0]	gnt_p12, gnt_p13, gnt_p14, gnt_p15;
wire	[4:0]	gnt_p16, gnt_p17, gnt_p18, gnt_p19;
wire	[4:0]	gnt_p20, gnt_p21, gnt_p22, gnt_p23;
wire	[4:0]	gnt_p24, gnt_p25, gnt_p26, gnt_p27;
wire	[4:0]	gnt_p28, gnt_p29, gnt_p30;


////////////////////////////////////////////////////////////////////
//
// Aliases
//

assign pri0[0] = ch0_csr[13];
assign pri0[1] = (pri_sel == 2'd0) ? 1'b0 : ch0_csr[14];
assign pri0[2] = (pri_sel == 2'd2) ? ch0_csr[15] : 1'b0;
assign pri1[0] = ch1_csr[13];
assign pri1[1] = (pri_sel == 2'd0) ? 1'b0 : ch1_csr[14];
assign pri1[2] = (pri_sel == 2'd2) ? ch1_csr[15] : 1'b0;
assign pri2[0] = ch2_csr[13];
assign pri2[1] = (pri_sel == 2'd0) ? 1'b0 : ch2_csr[14];
assign pri2[2] = (pri_sel == 2'd2) ? ch2_csr[15] : 1'b0;
assign pri3[0] = ch3_csr[13];
assign pri3[1] = (pri_sel == 2'd0) ? 1'b0 : ch3_csr[14];
assign pri3[2] = (pri_sel == 2'd2) ? ch3_csr[15] : 1'b0;
assign pri4[0] = ch4_csr[13];
assign pri4[1] = (pri_sel == 2'd0) ? 1'b0 : ch4_csr[14];
assign pri4[2] = (pri_sel == 2'd2) ? ch4_csr[15] : 1'b0;
assign pri5[0] = ch5_csr[13];
assign pri5[1] = (pri_sel == 2'd0) ? 1'b0 : ch5_csr[14];
assign pri5[2] = (pri_sel == 2'd2) ? ch5_csr[15] : 1'b0;
assign pri6[0] = ch6_csr[13];
assign pri6[1] = (pri_sel == 2'd0) ? 1'b0 : ch6_csr[14];
assign pri6[2] = (pri_sel == 2'd2) ? ch6_csr[15] : 1'b0;
assign pri7[0] = ch7_csr[13];
assign pri7[1] = (pri_sel == 2'd0) ? 1'b0 : ch7_csr[14];
assign pri7[2] = (pri_sel == 2'd2) ? ch7_csr[15] : 1'b0;
assign pri8[0] = ch8_csr[13];
assign pri8[1] = (pri_sel == 2'd0) ? 1'b0 : ch8_csr[14];
assign pri8[2] = (pri_sel == 2'd2) ? ch8_csr[15] : 1'b0;
assign pri9[0] = ch9_csr[13];
assign pri9[1] = (pri_sel == 2'd0) ? 1'b0 : ch9_csr[14];
assign pri9[2] = (pri_sel == 2'd2) ? ch9_csr[15] : 1'b0;
assign pri10[0] = ch10_csr[13];
assign pri10[1] = (pri_sel == 2'd0) ? 1'b0 : ch10_csr[14];
assign pri10[2] = (pri_sel == 2'd2) ? ch10_csr[15] : 1'b0;
assign pri11[0] = ch11_csr[13];
assign pri11[1] = (pri_sel == 2'd0) ? 1'b0 : ch11_csr[14];
assign pri11[2] = (pri_sel == 2'd2) ? ch11_csr[15] : 1'b0;
assign pri12[0] = ch12_csr[13];
assign pri12[1] = (pri_sel == 2'd0) ? 1'b0 : ch12_csr[14];
assign pri12[2] = (pri_sel == 2'd2) ? ch12_csr[15] : 1'b0;
assign pri13[0] = ch13_csr[13];
assign pri13[1] = (pri_sel == 2'd0) ? 1'b0 : ch13_csr[14];
assign pri13[2] = (pri_sel == 2'd2) ? ch13_csr[15] : 1'b0;
assign pri14[0] = ch14_csr[13];
assign pri14[1] = (pri_sel == 2'd0) ? 1'b0 : ch14_csr[14];
assign pri14[2] = (pri_sel == 2'd2) ? ch14_csr[15] : 1'b0;
assign pri15[0] = ch15_csr[13];
assign pri15[1] = (pri_sel == 2'd0) ? 1'b0 : ch15_csr[14];
assign pri15[2] = (pri_sel == 2'd2) ? ch15_csr[15] : 1'b0;
assign pri16[0] = ch16_csr[13];
assign pri16[1] = (pri_sel == 2'd0) ? 1'b0 : ch16_csr[14];
assign pri16[2] = (pri_sel == 2'd2) ? ch16_csr[15] : 1'b0;
assign pri17[0] = ch17_csr[13];
assign pri17[1] = (pri_sel == 2'd0) ? 1'b0 : ch17_csr[14];
assign pri17[2] = (pri_sel == 2'd2) ? ch17_csr[15] : 1'b0;
assign pri18[0] = ch18_csr[13];
assign pri18[1] = (pri_sel == 2'd0) ? 1'b0 : ch18_csr[14];
assign pri18[2] = (pri_sel == 2'd2) ? ch18_csr[15] : 1'b0;
assign pri19[0] = ch19_csr[13];
assign pri19[1] = (pri_sel == 2'd0) ? 1'b0 : ch19_csr[14];
assign pri19[2] = (pri_sel == 2'd2) ? ch19_csr[15] : 1'b0;
assign pri20[0] = ch20_csr[13];
assign pri20[1] = (pri_sel == 2'd0) ? 1'b0 : ch20_csr[14];
assign pri20[2] = (pri_sel == 2'd2) ? ch20_csr[15] : 1'b0;
assign pri21[0] = ch21_csr[13];
assign pri21[1] = (pri_sel == 2'd0) ? 1'b0 : ch21_csr[14];
assign pri21[2] = (pri_sel == 2'd2) ? ch21_csr[15] : 1'b0;
assign pri22[0] = ch22_csr[13];
assign pri22[1] = (pri_sel == 2'd0) ? 1'b0 : ch22_csr[14];
assign pri22[2] = (pri_sel == 2'd2) ? ch22_csr[15] : 1'b0;
assign pri23[0] = ch23_csr[13];
assign pri23[1] = (pri_sel == 2'd0) ? 1'b0 : ch23_csr[14];
assign pri23[2] = (pri_sel == 2'd2) ? ch23_csr[15] : 1'b0;
assign pri24[0] = ch24_csr[13];
assign pri24[1] = (pri_sel == 2'd0) ? 1'b0 : ch24_csr[14];
assign pri24[2] = (pri_sel == 2'd2) ? ch24_csr[15] : 1'b0;
assign pri25[0] = ch25_csr[13];
assign pri25[1] = (pri_sel == 2'd0) ? 1'b0 : ch25_csr[14];
assign pri25[2] = (pri_sel == 2'd2) ? ch25_csr[15] : 1'b0;
assign pri26[0] = ch26_csr[13];
assign pri26[1] = (pri_sel == 2'd0) ? 1'b0 : ch26_csr[14];
assign pri26[2] = (pri_sel == 2'd2) ? ch26_csr[15] : 1'b0;
assign pri27[0] = ch27_csr[13];
assign pri27[1] = (pri_sel == 2'd0) ? 1'b0 : ch27_csr[14];
assign pri27[2] = (pri_sel == 2'd2) ? ch27_csr[15] : 1'b0;
assign pri28[0] = ch28_csr[13];
assign pri28[1] = (pri_sel == 2'd0) ? 1'b0 : ch28_csr[14];
assign pri28[2] = (pri_sel == 2'd2) ? ch28_csr[15] : 1'b0;
assign pri29[0] = ch29_csr[13];
assign pri29[1] = (pri_sel == 2'd0) ? 1'b0 : ch29_csr[14];
assign pri29[2] = (pri_sel == 2'd2) ? ch29_csr[15] : 1'b0;
assign pri30[0] = ch30_csr[13];
assign pri30[1] = (pri_sel == 2'd0) ? 1'b0 : ch30_csr[14];
assign pri30[2] = (pri_sel == 2'd2) ? ch30_csr[15] : 1'b0;

////////////////////////////////////////////////////////////////////
//
// Misc logic
//

// Chanel Valid flag
// The valid flag is asserted when the channel is enabled,
// and is either in "normal mode" (software control) or
// "hw handshake mode" (reqN control)
// validN = ch_enabled & (sw_mode | (hw_mode & reqN) )

always @(posedge clk)
	req_r <= #1 req_i & ~ack_o;

assign valid[0] = ch0_conf[0] & ch0_csr[`WDMA_CH_EN] & (ch0_csr[`WDMA_MODE] ? (req_r[0] & !ack_o[0]) : 1'b1);
assign valid[1] = ch1_conf[0] & ch1_csr[`WDMA_CH_EN] & (ch1_csr[`WDMA_MODE] ? (req_r[1] & !ack_o[1]) : 1'b1);
assign valid[2] = ch2_conf[0] & ch2_csr[`WDMA_CH_EN] & (ch2_csr[`WDMA_MODE] ? (req_r[2] & !ack_o[2]) : 1'b1);
assign valid[3] = ch3_conf[0] & ch3_csr[`WDMA_CH_EN] & (ch3_csr[`WDMA_MODE] ? (req_r[3] & !ack_o[3]) : 1'b1);
assign valid[4] = ch4_conf[0] & ch4_csr[`WDMA_CH_EN] & (ch4_csr[`WDMA_MODE] ? (req_r[4] & !ack_o[4]) : 1'b1);
assign valid[5] = ch5_conf[0] & ch5_csr[`WDMA_CH_EN] & (ch5_csr[`WDMA_MODE] ? (req_r[5] & !ack_o[5]) : 1'b1);
assign valid[6] = ch6_conf[0] & ch6_csr[`WDMA_CH_EN] & (ch6_csr[`WDMA_MODE] ? (req_r[6] & !ack_o[6]) : 1'b1);
assign valid[7] = ch7_conf[0] & ch7_csr[`WDMA_CH_EN] & (ch7_csr[`WDMA_MODE] ? (req_r[7] & !ack_o[7]) : 1'b1);
assign valid[8] = ch8_conf[0] & ch8_csr[`WDMA_CH_EN] & (ch8_csr[`WDMA_MODE] ? (req_r[8] & !ack_o[8]) : 1'b1);
assign valid[9] = ch9_conf[0] & ch9_csr[`WDMA_CH_EN] & (ch9_csr[`WDMA_MODE] ? (req_r[9] & !ack_o[9]) : 1'b1);
assign valid[10] = ch10_conf[0] & ch10_csr[`WDMA_CH_EN] & (ch10_csr[`WDMA_MODE] ? (req_r[10] & !ack_o[10]) : 1'b1);
assign valid[11] = ch11_conf[0] & ch11_csr[`WDMA_CH_EN] & (ch11_csr[`WDMA_MODE] ? (req_r[11] & !ack_o[11]) : 1'b1);
assign valid[12] = ch12_conf[0] & ch12_csr[`WDMA_CH_EN] & (ch12_csr[`WDMA_MODE] ? (req_r[12] & !ack_o[12]) : 1'b1);
assign valid[13] = ch13_conf[0] & ch13_csr[`WDMA_CH_EN] & (ch13_csr[`WDMA_MODE] ? (req_r[13] & !ack_o[13]) : 1'b1);
assign valid[14] = ch14_conf[0] & ch14_csr[`WDMA_CH_EN] & (ch14_csr[`WDMA_MODE] ? (req_r[14] & !ack_o[14]) : 1'b1);
assign valid[15] = ch15_conf[0] & ch15_csr[`WDMA_CH_EN] & (ch15_csr[`WDMA_MODE] ? (req_r[15] & !ack_o[15]) : 1'b1);
assign valid[16] = ch16_conf[0] & ch16_csr[`WDMA_CH_EN] & (ch16_csr[`WDMA_MODE] ? (req_r[16] & !ack_o[16]) : 1'b1);
assign valid[17] = ch17_conf[0] & ch17_csr[`WDMA_CH_EN] & (ch17_csr[`WDMA_MODE] ? (req_r[17] & !ack_o[17]) : 1'b1);
assign valid[18] = ch18_conf[0] & ch18_csr[`WDMA_CH_EN] & (ch18_csr[`WDMA_MODE] ? (req_r[18] & !ack_o[18]) : 1'b1);
assign valid[19] = ch19_conf[0] & ch19_csr[`WDMA_CH_EN] & (ch19_csr[`WDMA_MODE] ? (req_r[19] & !ack_o[19]) : 1'b1);
assign valid[20] = ch20_conf[0] & ch20_csr[`WDMA_CH_EN] & (ch20_csr[`WDMA_MODE] ? (req_r[20] & !ack_o[20]) : 1'b1);
assign valid[21] = ch21_conf[0] & ch21_csr[`WDMA_CH_EN] & (ch21_csr[`WDMA_MODE] ? (req_r[21] & !ack_o[21]) : 1'b1);
assign valid[22] = ch22_conf[0] & ch22_csr[`WDMA_CH_EN] & (ch22_csr[`WDMA_MODE] ? (req_r[22] & !ack_o[22]) : 1'b1);
assign valid[23] = ch23_conf[0] & ch23_csr[`WDMA_CH_EN] & (ch23_csr[`WDMA_MODE] ? (req_r[23] & !ack_o[23]) : 1'b1);
assign valid[24] = ch24_conf[0] & ch24_csr[`WDMA_CH_EN] & (ch24_csr[`WDMA_MODE] ? (req_r[24] & !ack_o[24]) : 1'b1);
assign valid[25] = ch25_conf[0] & ch25_csr[`WDMA_CH_EN] & (ch25_csr[`WDMA_MODE] ? (req_r[25] & !ack_o[25]) : 1'b1);
assign valid[26] = ch26_conf[0] & ch26_csr[`WDMA_CH_EN] & (ch26_csr[`WDMA_MODE] ? (req_r[26] & !ack_o[26]) : 1'b1);
assign valid[27] = ch27_conf[0] & ch27_csr[`WDMA_CH_EN] & (ch27_csr[`WDMA_MODE] ? (req_r[27] & !ack_o[27]) : 1'b1);
assign valid[28] = ch28_conf[0] & ch28_csr[`WDMA_CH_EN] & (ch28_csr[`WDMA_MODE] ? (req_r[28] & !ack_o[28]) : 1'b1);
assign valid[29] = ch29_conf[0] & ch29_csr[`WDMA_CH_EN] & (ch29_csr[`WDMA_MODE] ? (req_r[29] & !ack_o[29]) : 1'b1);
assign valid[30] = ch30_conf[0] & ch30_csr[`WDMA_CH_EN] & (ch30_csr[`WDMA_MODE] ? (req_r[30] & !ack_o[30]) : 1'b1);

always @(posedge clk)
	ndr_r <= #1 nd_i & req_i;

always @(posedge clk)
	ndnr <= #1 nd_i & ~req_i;

// Start Signal for DMA engine
assign de_start = (valid_sel & !de_start_r ) | next_start;

always @(posedge clk)
	de_start_r <= #1 valid_sel;

always @(posedge clk)
	next_start <= #1 next_ch & valid_sel;

// Ack outputs for HW handshake mode
always @(posedge clk)
	ack_o[0] <= #1 ch0_conf[0] & (ch_sel == 5'h0) & ch0_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[1] <= #1 ch1_conf[0] & (ch_sel == 5'h1) & ch1_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[2] <= #1 ch2_conf[0] & (ch_sel == 5'h2) & ch2_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[3] <= #1 ch3_conf[0] & (ch_sel == 5'h3) & ch3_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[4] <= #1 ch4_conf[0] & (ch_sel == 5'h4) & ch4_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[5] <= #1 ch5_conf[0] & (ch_sel == 5'h5) & ch5_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[6] <= #1 ch6_conf[0] & (ch_sel == 5'h6) & ch6_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[7] <= #1 ch7_conf[0] & (ch_sel == 5'h7) & ch7_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[8] <= #1 ch8_conf[0] & (ch_sel == 5'h8) & ch8_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[9] <= #1 ch9_conf[0] & (ch_sel == 5'h9) & ch9_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[10] <= #1 ch10_conf[0] & (ch_sel == 5'ha) & ch10_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[11] <= #1 ch11_conf[0] & (ch_sel == 5'hb) & ch11_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[12] <= #1 ch12_conf[0] & (ch_sel == 5'hc) & ch12_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[13] <= #1 ch13_conf[0] & (ch_sel == 5'hd) & ch13_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[14] <= #1 ch14_conf[0] & (ch_sel == 5'he) & ch14_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[15] <= #1 ch15_conf[0] & (ch_sel == 5'hf) & ch15_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[16] <= #1 ch16_conf[0] & (ch_sel == 5'h10) & ch16_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[17] <= #1 ch17_conf[0] & (ch_sel == 5'h11) & ch17_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[18] <= #1 ch18_conf[0] & (ch_sel == 5'h12) & ch18_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[19] <= #1 ch19_conf[0] & (ch_sel == 5'h13) & ch19_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[20] <= #1 ch20_conf[0] & (ch_sel == 5'h14) & ch20_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[21] <= #1 ch21_conf[0] & (ch_sel == 5'h15) & ch21_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[22] <= #1 ch22_conf[0] & (ch_sel == 5'h16) & ch22_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[23] <= #1 ch23_conf[0] & (ch_sel == 5'h17) & ch23_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[24] <= #1 ch24_conf[0] & (ch_sel == 5'h18) & ch24_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[25] <= #1 ch25_conf[0] & (ch_sel == 5'h19) & ch25_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[26] <= #1 ch26_conf[0] & (ch_sel == 5'h1a) & ch26_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[27] <= #1 ch27_conf[0] & (ch_sel == 5'h1b) & ch27_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[28] <= #1 ch28_conf[0] & (ch_sel == 5'h1c) & ch28_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[29] <= #1 ch29_conf[0] & (ch_sel == 5'h1d) & ch29_csr[`WDMA_MODE] & de_ack;

always @(posedge clk)
	ack_o[30] <= #1 ch30_conf[0] & (ch_sel == 5'h1e) & ch30_csr[`WDMA_MODE] & de_ack;

// Channel Select
always @(posedge clk or negedge rst)
	if(!rst)	ch_sel_r <= #1 0;
	else
	if(de_start)	ch_sel_r <= #1 ch_sel_d;

assign ch_sel = !dma_busy ? ch_sel_d : ch_sel_r;

////////////////////////////////////////////////////////////////////
//
// Select Registers based on arbiter (and priority) outputs
//

always @(ch_sel or valid)
	case(ch_sel)		// synopsys parallel_case full_case
	   5'h0:	valid_sel = valid[0];
	   5'h1:	valid_sel = valid[1];
	   5'h2:	valid_sel = valid[2];
	   5'h3:	valid_sel = valid[3];
	   5'h4:	valid_sel = valid[4];
	   5'h5:	valid_sel = valid[5];
	   5'h6:	valid_sel = valid[6];
	   5'h7:	valid_sel = valid[7];
	   5'h8:	valid_sel = valid[8];
	   5'h9:	valid_sel = valid[9];
	   5'ha:	valid_sel = valid[10];
	   5'hb:	valid_sel = valid[11];
	   5'hc:	valid_sel = valid[12];
	   5'hd:	valid_sel = valid[13];
	   5'he:	valid_sel = valid[14];
	   5'hf:	valid_sel = valid[15];
	   5'h10:	valid_sel = valid[16];
	   5'h11:	valid_sel = valid[17];
	   5'h12:	valid_sel = valid[18];
	   5'h13:	valid_sel = valid[19];
	   5'h14:	valid_sel = valid[20];
	   5'h15:	valid_sel = valid[21];
	   5'h16:	valid_sel = valid[22];
	   5'h17:	valid_sel = valid[23];
	   5'h18:	valid_sel = valid[24];
	   5'h19:	valid_sel = valid[25];
	   5'h1a:	valid_sel = valid[26];
	   5'h1b:	valid_sel = valid[27];
	   5'h1c:	valid_sel = valid[28];
	   5'h1d:	valid_sel = valid[29];
	   5'h1e:	valid_sel = valid[30];
	endcase

always @(ch_sel or ndr_r)
	case(ch_sel)		// synopsys parallel_case full_case
	   5'h0:	ndr = ndr_r[0];
	   5'h1:	ndr = ndr_r[1];
	   5'h2:	ndr = ndr_r[2];
	   5'h3:	ndr = ndr_r[3];
	   5'h4:	ndr = ndr_r[4];
	   5'h5:	ndr = ndr_r[5];
	   5'h6:	ndr = ndr_r[6];
	   5'h7:	ndr = ndr_r[7];
	   5'h8:	ndr = ndr_r[8];
	   5'h9:	ndr = ndr_r[9];
	   5'ha:	ndr = ndr_r[10];
	   5'hb:	ndr = ndr_r[11];
	   5'hc:	ndr = ndr_r[12];
	   5'hd:	ndr = ndr_r[13];
	   5'he:	ndr = ndr_r[14];
	   5'hf:	ndr = ndr_r[15];
	   5'h10:	ndr = ndr_r[16];
	   5'h11:	ndr = ndr_r[17];
	   5'h12:	ndr = ndr_r[18];
	   5'h13:	ndr = ndr_r[19];
	   5'h14:	ndr = ndr_r[20];
	   5'h15:	ndr = ndr_r[21];
	   5'h16:	ndr = ndr_r[22];
	   5'h17:	ndr = ndr_r[23];
	   5'h18:	ndr = ndr_r[24];
	   5'h19:	ndr = ndr_r[25];
	   5'h1a:	ndr = ndr_r[26];
	   5'h1b:	ndr = ndr_r[27];
	   5'h1c:	ndr = ndr_r[28];
	   5'h1d:	ndr = ndr_r[29];
	   5'h1e:	ndr = ndr_r[30];
	endcase

always @(ch_sel or pointer0 or pointer1 or pointer2 or pointer3 or pointer4
		or pointer5 or pointer6 or pointer7 or pointer8 or pointer9
		or pointer10 or pointer11 or pointer12 or pointer13 or pointer14
		or pointer15 or pointer16 or pointer17 or pointer18 or pointer19
		or pointer20 or pointer21 or pointer22 or pointer23 or pointer24
		or pointer25 or pointer26 or pointer27 or pointer28 or pointer29
		or pointer30 )
	case(ch_sel)		// synopsys parallel_case full_case
	   5'h0:	pointer = pointer0;
	   5'h1:	pointer = pointer1;
	   5'h2:	pointer = pointer2;
	   5'h3:	pointer = pointer3;
	   5'h4:	pointer = pointer4;
	   5'h5:	pointer = pointer5;
	   5'h6:	pointer = pointer6;
	   5'h7:	pointer = pointer7;
	   5'h8:	pointer = pointer8;
	   5'h9:	pointer = pointer9;
	   5'ha:	pointer = pointer10;
	   5'hb:	pointer = pointer11;
	   5'hc:	pointer = pointer12;
	   5'hd:	pointer = pointer13;
	   5'he:	pointer = pointer14;
	   5'hf:	pointer = pointer15;
	   5'h10:	pointer = pointer16;
	   5'h11:	pointer = pointer17;
	   5'h12:	pointer = pointer18;
	   5'h13:	pointer = pointer19;
	   5'h14:	pointer = pointer20;
	   5'h15:	pointer = pointer21;
	   5'h16:	pointer = pointer22;
	   5'h17:	pointer = pointer23;
	   5'h18:	pointer = pointer24;
	   5'h19:	pointer = pointer25;
	   5'h1a:	pointer = pointer26;
	   5'h1b:	pointer = pointer27;
	   5'h1c:	pointer = pointer28;
	   5'h1d:	pointer = pointer29;
	   5'h1e:	pointer = pointer30;
	endcase

always @(ch_sel or pointer0_s or pointer1_s or pointer2_s or pointer3_s or pointer4_s
		or pointer5_s or pointer6_s or pointer7_s or pointer8_s or pointer9_s
		or pointer10_s or pointer11_s or pointer12_s or pointer13_s or pointer14_s
		or pointer15_s or pointer16_s or pointer17_s or pointer18_s or pointer19_s
		or pointer20_s or pointer21_s or pointer22_s or pointer23_s or pointer24_s
		or pointer25_s or pointer26_s or pointer27_s or pointer28_s or pointer29_s
		or pointer30_s )
	case(ch_sel)		// synopsys parallel_case full_case
	   5'h0:	pointer_s = pointer0_s;
	   5'h1:	pointer_s = pointer1_s;
	   5'h2:	pointer_s = pointer2_s;
	   5'h3:	pointer_s = pointer3_s;
	   5'h4:	pointer_s = pointer4_s;
	   5'h5:	pointer_s = pointer5_s;
	   5'h6:	pointer_s = pointer6_s;
	   5'h7:	pointer_s = pointer7_s;
	   5'h8:	pointer_s = pointer8_s;
	   5'h9:	pointer_s = pointer9_s;
	   5'ha:	pointer_s = pointer10_s;
	   5'hb:	pointer_s = pointer11_s;
	   5'hc:	pointer_s = pointer12_s;
	   5'hd:	pointer_s = pointer13_s;
	   5'he:	pointer_s = pointer14_s;
	   5'hf:	pointer_s = pointer15_s;
	   5'h10:	pointer_s = pointer16_s;
	   5'h11:	pointer_s = pointer17_s;
	   5'h12:	pointer_s = pointer18_s;
	   5'h13:	pointer_s = pointer19_s;
	   5'h14:	pointer_s = pointer20_s;
	   5'h15:	pointer_s = pointer21_s;
	   5'h16:	pointer_s = pointer22_s;
	   5'h17:	pointer_s = pointer23_s;
	   5'h18:	pointer_s = pointer24_s;
	   5'h19:	pointer_s = pointer25_s;
	   5'h1a:	pointer_s = pointer26_s;
	   5'h1b:	pointer_s = pointer27_s;
	   5'h1c:	pointer_s = pointer28_s;
	   5'h1d:	pointer_s = pointer29_s;
	   5'h1e:	pointer_s = pointer30_s;
	endcase

always @(ch_sel or ch0_csr or ch1_csr or ch2_csr or ch3_csr or ch4_csr
		or ch5_csr or ch6_csr or ch7_csr or ch8_csr or ch9_csr
		or ch10_csr or ch11_csr or ch12_csr or ch13_csr or ch14_csr
		or ch15_csr or ch16_csr or ch17_csr or ch18_csr or ch19_csr
		or ch20_csr or ch21_csr or ch22_csr or ch23_csr or ch24_csr
		or ch25_csr or ch26_csr or ch27_csr or ch28_csr or ch29_csr
		or ch30_csr )
	case(ch_sel)		// synopsys parallel_case full_case
	   5'h0:	csr = ch0_csr;
	   5'h1:	csr = ch1_csr;
	   5'h2:	csr = ch2_csr;
	   5'h3:	csr = ch3_csr;
	   5'h4:	csr = ch4_csr;
	   5'h5:	csr = ch5_csr;
	   5'h6:	csr = ch6_csr;
	   5'h7:	csr = ch7_csr;
	   5'h8:	csr = ch8_csr;
	   5'h9:	csr = ch9_csr;
	   5'ha:	csr = ch10_csr;
	   5'hb:	csr = ch11_csr;
	   5'hc:	csr = ch12_csr;
	   5'hd:	csr = ch13_csr;
	   5'he:	csr = ch14_csr;
	   5'hf:	csr = ch15_csr;
	   5'h10:	csr = ch16_csr;
	   5'h11:	csr = ch17_csr;
	   5'h12:	csr = ch18_csr;
	   5'h13:	csr = ch19_csr;
	   5'h14:	csr = ch20_csr;
	   5'h15:	csr = ch21_csr;
	   5'h16:	csr = ch22_csr;
	   5'h17:	csr = ch23_csr;
	   5'h18:	csr = ch24_csr;
	   5'h19:	csr = ch25_csr;
	   5'h1a:	csr = ch26_csr;
	   5'h1b:	csr = ch27_csr;
	   5'h1c:	csr = ch28_csr;
	   5'h1d:	csr = ch29_csr;
	   5'h1e:	csr = ch30_csr;
	endcase

always @(ch_sel or ch0_txsz or ch1_txsz or ch2_txsz or ch3_txsz or ch4_txsz
		or ch5_txsz or ch6_txsz or ch7_txsz or ch8_txsz or ch9_txsz
		or ch10_txsz or ch11_txsz or ch12_txsz or ch13_txsz or ch14_txsz
		or ch15_txsz or ch16_txsz or ch17_txsz or ch18_txsz or ch19_txsz
		or ch20_txsz or ch21_txsz or ch22_txsz or ch23_txsz or ch24_txsz
		or ch25_txsz or ch26_txsz or ch27_txsz or ch28_txsz or ch29_txsz
		or ch30_txsz )
	case(ch_sel)		// synopsys parallel_case full_case
	   5'h0:	txsz = ch0_txsz;
	   5'h1:	txsz = ch1_txsz;
	   5'h2:	txsz = ch2_txsz;
	   5'h3:	txsz = ch3_txsz;
	   5'h4:	txsz = ch4_txsz;
	   5'h5:	txsz = ch5_txsz;
	   5'h6:	txsz = ch6_txsz;
	   5'h7:	txsz = ch7_txsz;
	   5'h8:	txsz = ch8_txsz;
	   5'h9:	txsz = ch9_txsz;
	   5'ha:	txsz = ch10_txsz;
	   5'hb:	txsz = ch11_txsz;
	   5'hc:	txsz = ch12_txsz;
	   5'hd:	txsz = ch13_txsz;
	   5'he:	txsz = ch14_txsz;
	   5'hf:	txsz = ch15_txsz;
	   5'h10:	txsz = ch16_txsz;
	   5'h11:	txsz = ch17_txsz;
	   5'h12:	txsz = ch18_txsz;
	   5'h13:	txsz = ch19_txsz;
	   5'h14:	txsz = ch20_txsz;
	   5'h15:	txsz = ch21_txsz;
	   5'h16:	txsz = ch22_txsz;
	   5'h17:	txsz = ch23_txsz;
	   5'h18:	txsz = ch24_txsz;
	   5'h19:	txsz = ch25_txsz;
	   5'h1a:	txsz = ch26_txsz;
	   5'h1b:	txsz = ch27_txsz;
	   5'h1c:	txsz = ch28_txsz;
	   5'h1d:	txsz = ch29_txsz;
	   5'h1e:	txsz = ch30_txsz;
	endcase

always @(ch_sel or ch0_adr0 or ch1_adr0 or ch2_adr0 or ch3_adr0 or ch4_adr0
		or ch5_adr0 or ch6_adr0 or ch7_adr0 or ch8_adr0 or ch9_adr0
		or ch10_adr0 or ch11_adr0 or ch12_adr0 or ch13_adr0 or ch14_adr0
		or ch15_adr0 or ch16_adr0 or ch17_adr0 or ch18_adr0 or ch19_adr0
		or ch20_adr0 or ch21_adr0 or ch22_adr0 or ch23_adr0 or ch24_adr0
		or ch25_adr0 or ch26_adr0 or ch27_adr0 or ch28_adr0 or ch29_adr0
		or ch30_adr0 )
	case(ch_sel)		// synopsys parallel_case full_case
	   5'h0:	adr0 = ch0_adr0;
	   5'h1:	adr0 = ch1_adr0;
	   5'h2:	adr0 = ch2_adr0;
	   5'h3:	adr0 = ch3_adr0;
	   5'h4:	adr0 = ch4_adr0;
	   5'h5:	adr0 = ch5_adr0;
	   5'h6:	adr0 = ch6_adr0;
	   5'h7:	adr0 = ch7_adr0;
	   5'h8:	adr0 = ch8_adr0;
	   5'h9:	adr0 = ch9_adr0;
	   5'ha:	adr0 = ch10_adr0;
	   5'hb:	adr0 = ch11_adr0;
	   5'hc:	adr0 = ch12_adr0;
	   5'hd:	adr0 = ch13_adr0;
	   5'he:	adr0 = ch14_adr0;
	   5'hf:	adr0 = ch15_adr0;
	   5'h10:	adr0 = ch16_adr0;
	   5'h11:	adr0 = ch17_adr0;
	   5'h12:	adr0 = ch18_adr0;
	   5'h13:	adr0 = ch19_adr0;
	   5'h14:	adr0 = ch20_adr0;
	   5'h15:	adr0 = ch21_adr0;
	   5'h16:	adr0 = ch22_adr0;
	   5'h17:	adr0 = ch23_adr0;
	   5'h18:	adr0 = ch24_adr0;
	   5'h19:	adr0 = ch25_adr0;
	   5'h1a:	adr0 = ch26_adr0;
	   5'h1b:	adr0 = ch27_adr0;
	   5'h1c:	adr0 = ch28_adr0;
	   5'h1d:	adr0 = ch29_adr0;
	   5'h1e:	adr0 = ch30_adr0;
	endcase

always @(ch_sel or ch0_adr1 or ch1_adr1 or ch2_adr1 or ch3_adr1 or ch4_adr1
		or ch5_adr1 or ch6_adr1 or ch7_adr1 or ch8_adr1 or ch9_adr1
		or ch10_adr1 or ch11_adr1 or ch12_adr1 or ch13_adr1 or ch14_adr1
		or ch15_adr1 or ch16_adr1 or ch17_adr1 or ch18_adr1 or ch19_adr1
		or ch20_adr1 or ch21_adr1 or ch22_adr1 or ch23_adr1 or ch24_adr1
		or ch25_adr1 or ch26_adr1 or ch27_adr1 or ch28_adr1 or ch29_adr1
		or ch30_adr1 )
	case(ch_sel)		// synopsys parallel_case full_case
	   5'h0:	adr1 = ch0_adr1;
	   5'h1:	adr1 = ch1_adr1;
	   5'h2:	adr1 = ch2_adr1;
	   5'h3:	adr1 = ch3_adr1;
	   5'h4:	adr1 = ch4_adr1;
	   5'h5:	adr1 = ch5_adr1;
	   5'h6:	adr1 = ch6_adr1;
	   5'h7:	adr1 = ch7_adr1;
	   5'h8:	adr1 = ch8_adr1;
	   5'h9:	adr1 = ch9_adr1;
	   5'ha:	adr1 = ch10_adr1;
	   5'hb:	adr1 = ch11_adr1;
	   5'hc:	adr1 = ch12_adr1;
	   5'hd:	adr1 = ch13_adr1;
	   5'he:	adr1 = ch14_adr1;
	   5'hf:	adr1 = ch15_adr1;
	   5'h10:	adr1 = ch16_adr1;
	   5'h11:	adr1 = ch17_adr1;
	   5'h12:	adr1 = ch18_adr1;
	   5'h13:	adr1 = ch19_adr1;
	   5'h14:	adr1 = ch20_adr1;
	   5'h15:	adr1 = ch21_adr1;
	   5'h16:	adr1 = ch22_adr1;
	   5'h17:	adr1 = ch23_adr1;
	   5'h18:	adr1 = ch24_adr1;
	   5'h19:	adr1 = ch25_adr1;
	   5'h1a:	adr1 = ch26_adr1;
	   5'h1b:	adr1 = ch27_adr1;
	   5'h1c:	adr1 = ch28_adr1;
	   5'h1d:	adr1 = ch29_adr1;
	   5'h1e:	adr1 = ch30_adr1;
	endcase

always @(ch_sel or ch0_am0 or ch1_am0 or ch2_am0 or ch3_am0 or ch4_am0
		or ch5_am0 or ch6_am0 or ch7_am0 or ch8_am0 or ch9_am0
		or ch10_am0 or ch11_am0 or ch12_am0 or ch13_am0 or ch14_am0
		or ch15_am0 or ch16_am0 or ch17_am0 or ch18_am0 or ch19_am0
		or ch20_am0 or ch21_am0 or ch22_am0 or ch23_am0 or ch24_am0
		or ch25_am0 or ch26_am0 or ch27_am0 or ch28_am0 or ch29_am0
		or ch30_am0 )
	case(ch_sel)		// synopsys parallel_case full_case
	   5'h0:	am0 = ch0_am0;
	   5'h1:	am0 = ch1_am0;
	   5'h2:	am0 = ch2_am0;
	   5'h3:	am0 = ch3_am0;
	   5'h4:	am0 = ch4_am0;
	   5'h5:	am0 = ch5_am0;
	   5'h6:	am0 = ch6_am0;
	   5'h7:	am0 = ch7_am0;
	   5'h8:	am0 = ch8_am0;
	   5'h9:	am0 = ch9_am0;
	   5'ha:	am0 = ch10_am0;
	   5'hb:	am0 = ch11_am0;
	   5'hc:	am0 = ch12_am0;
	   5'hd:	am0 = ch13_am0;
	   5'he:	am0 = ch14_am0;
	   5'hf:	am0 = ch15_am0;
	   5'h10:	am0 = ch16_am0;
	   5'h11:	am0 = ch17_am0;
	   5'h12:	am0 = ch18_am0;
	   5'h13:	am0 = ch19_am0;
	   5'h14:	am0 = ch20_am0;
	   5'h15:	am0 = ch21_am0;
	   5'h16:	am0 = ch22_am0;
	   5'h17:	am0 = ch23_am0;
	   5'h18:	am0 = ch24_am0;
	   5'h19:	am0 = ch25_am0;
	   5'h1a:	am0 = ch26_am0;
	   5'h1b:	am0 = ch27_am0;
	   5'h1c:	am0 = ch28_am0;
	   5'h1d:	am0 = ch29_am0;
	   5'h1e:	am0 = ch30_am0;
	endcase

always @(ch_sel or ch0_am1 or ch1_am1 or ch2_am1 or ch3_am1 or ch4_am1
		or ch5_am1 or ch6_am1 or ch7_am1 or ch8_am1 or ch9_am1
		or ch10_am1 or ch11_am1 or ch12_am1 or ch13_am1 or ch14_am1
		or ch15_am1 or ch16_am1 or ch17_am1 or ch18_am1 or ch19_am1
		or ch20_am1 or ch21_am1 or ch22_am1 or ch23_am1 or ch24_am1
		or ch25_am1 or ch26_am1 or ch27_am1 or ch28_am1 or ch29_am1
		or ch30_am1 )
	case(ch_sel)		// synopsys parallel_case full_case
	   5'h0:	am1 = ch0_am1;
	   5'h1:	am1 = ch1_am1;
	   5'h2:	am1 = ch2_am1;
	   5'h3:	am1 = ch3_am1;
	   5'h4:	am1 = ch4_am1;
	   5'h5:	am1 = ch5_am1;
	   5'h6:	am1 = ch6_am1;
	   5'h7:	am1 = ch7_am1;
	   5'h8:	am1 = ch8_am1;
	   5'h9:	am1 = ch9_am1;
	   5'ha:	am1 = ch10_am1;
	   5'hb:	am1 = ch11_am1;
	   5'hc:	am1 = ch12_am1;
	   5'hd:	am1 = ch13_am1;
	   5'he:	am1 = ch14_am1;
	   5'hf:	am1 = ch15_am1;
	   5'h10:	am1 = ch16_am1;
	   5'h11:	am1 = ch17_am1;
	   5'h12:	am1 = ch18_am1;
	   5'h13:	am1 = ch19_am1;
	   5'h14:	am1 = ch20_am1;
	   5'h15:	am1 = ch21_am1;
	   5'h16:	am1 = ch22_am1;
	   5'h17:	am1 = ch23_am1;
	   5'h18:	am1 = ch24_am1;
	   5'h19:	am1 = ch25_am1;
	   5'h1a:	am1 = ch26_am1;
	   5'h1b:	am1 = ch27_am1;
	   5'h1c:	am1 = ch28_am1;
	   5'h1d:	am1 = ch29_am1;
	   5'h1e:	am1 = ch30_am1;
	endcase

////////////////////////////////////////////////////////////////////
//
// Actual Chanel Arbiter and Priority Encoder
//

// Select the arbiter for current highest priority
always @(pri_out or gnt_p0 or gnt_p1 or gnt_p2 or gnt_p3 or gnt_p4
		or gnt_p5 or gnt_p6 or gnt_p7 )
	case(pri_out)		// synopsys parallel_case full_case
	   3'h0:	ch_sel_d = gnt_p0;
	   3'h1:	ch_sel_d = gnt_p1;
	   3'h2:	ch_sel_d = gnt_p2;
	   3'h3:	ch_sel_d = gnt_p3;
	   3'h4:	ch_sel_d = gnt_p4;
	   3'h5:	ch_sel_d = gnt_p5;
	   3'h6:	ch_sel_d = gnt_p6;
	   3'h7:	ch_sel_d = gnt_p7;
	endcase


// Priority Encoder
wb_dma_ch_pri_enc
	#(	pri_sel,
		ch0_conf,
		ch1_conf,
		ch2_conf,
		ch3_conf,
		ch4_conf,
		ch5_conf,
		ch6_conf,
		ch7_conf,
		ch8_conf,
		ch9_conf,
		ch10_conf,
		ch11_conf,
		ch12_conf,
		ch13_conf,
		ch14_conf,
		ch15_conf,
		ch16_conf,
		ch17_conf,
		ch18_conf,
		ch19_conf,
		ch20_conf,
		ch21_conf,
		ch22_conf,
		ch23_conf,
		ch24_conf,
		ch25_conf,
		ch26_conf,
		ch27_conf,
		ch28_conf,
		ch29_conf,
		ch30_conf)
		u0(
		.clk(		clk		),
		.valid(		valid		),
		.pri0(		pri0		),
		.pri1(		pri1		),
		.pri2(		pri2		),
		.pri3(		pri3		),
		.pri4(		pri4		),
		.pri5(		pri5		),
		.pri6(		pri6		),
		.pri7(		pri7		),
		.pri8(		pri8		),
		.pri9(		pri9		),
		.pri10(		pri10		),
		.pri11(		pri11		),
		.pri12(		pri12		),
		.pri13(		pri13		),
		.pri14(		pri14		),
		.pri15(		pri15		),
		.pri16(		pri16		),
		.pri17(		pri17		),
		.pri18(		pri18		),
		.pri19(		pri19		),
		.pri20(		pri20		),
		.pri21(		pri21		),
		.pri22(		pri22		),
		.pri23(		pri23		),
		.pri24(		pri24		),
		.pri25(		pri25		),
		.pri26(		pri26		),
		.pri27(		pri27		),
		.pri28(		pri28		),
		.pri29(		pri29		),
		.pri30(		pri30		),
		.pri_out(	pri_out		)
		);

// Arbiter request lines
// Generate request depending on priority and valid bits

assign req_p0[0] = valid[0] & (pri0==3'h0);
assign req_p0[1] = valid[1] & (pri1==3'h0);
assign req_p0[2] = valid[2] & (pri2==3'h0);
assign req_p0[3] = valid[3] & (pri3==3'h0);
assign req_p0[4] = valid[4] & (pri4==3'h0);
assign req_p0[5] = valid[5] & (pri5==3'h0);
assign req_p0[6] = valid[6] & (pri6==3'h0);
assign req_p0[7] = valid[7] & (pri7==3'h0);
assign req_p0[8] = valid[8] & (pri8==3'h0);
assign req_p0[9] = valid[9] & (pri9==3'h0);
assign req_p0[10] = valid[10] & (pri10==3'h0);
assign req_p0[11] = valid[11] & (pri11==3'h0);
assign req_p0[12] = valid[12] & (pri12==3'h0);
assign req_p0[13] = valid[13] & (pri13==3'h0);
assign req_p0[14] = valid[14] & (pri14==3'h0);
assign req_p0[15] = valid[15] & (pri15==3'h0);
assign req_p0[16] = valid[16] & (pri16==3'h0);
assign req_p0[17] = valid[17] & (pri17==3'h0);
assign req_p0[18] = valid[18] & (pri18==3'h0);
assign req_p0[19] = valid[19] & (pri19==3'h0);
assign req_p0[20] = valid[20] & (pri20==3'h0);
assign req_p0[21] = valid[21] & (pri21==3'h0);
assign req_p0[22] = valid[22] & (pri22==3'h0);
assign req_p0[23] = valid[23] & (pri23==3'h0);
assign req_p0[24] = valid[24] & (pri24==3'h0);
assign req_p0[25] = valid[25] & (pri25==3'h0);
assign req_p0[26] = valid[26] & (pri26==3'h0);
assign req_p0[27] = valid[27] & (pri27==3'h0);
assign req_p0[28] = valid[28] & (pri28==3'h0);
assign req_p0[29] = valid[29] & (pri29==3'h0);
assign req_p0[30] = valid[30] & (pri30==3'h0);

assign req_p1[0] = valid[0] & (pri0==3'h1);
assign req_p1[1] = valid[1] & (pri1==3'h1);
assign req_p1[2] = valid[2] & (pri2==3'h1);
assign req_p1[3] = valid[3] & (pri3==3'h1);
assign req_p1[4] = valid[4] & (pri4==3'h1);
assign req_p1[5] = valid[5] & (pri5==3'h1);
assign req_p1[6] = valid[6] & (pri6==3'h1);
assign req_p1[7] = valid[7] & (pri7==3'h1);
assign req_p1[8] = valid[8] & (pri8==3'h1);
assign req_p1[9] = valid[9] & (pri9==3'h1);
assign req_p1[10] = valid[10] & (pri10==3'h1);
assign req_p1[11] = valid[11] & (pri11==3'h1);
assign req_p1[12] = valid[12] & (pri12==3'h1);
assign req_p1[13] = valid[13] & (pri13==3'h1);
assign req_p1[14] = valid[14] & (pri14==3'h1);
assign req_p1[15] = valid[15] & (pri15==3'h1);
assign req_p1[16] = valid[16] & (pri16==3'h1);
assign req_p1[17] = valid[17] & (pri17==3'h1);
assign req_p1[18] = valid[18] & (pri18==3'h1);
assign req_p1[19] = valid[19] & (pri19==3'h1);
assign req_p1[20] = valid[20] & (pri20==3'h1);
assign req_p1[21] = valid[21] & (pri21==3'h1);
assign req_p1[22] = valid[22] & (pri22==3'h1);
assign req_p1[23] = valid[23] & (pri23==3'h1);
assign req_p1[24] = valid[24] & (pri24==3'h1);
assign req_p1[25] = valid[25] & (pri25==3'h1);
assign req_p1[26] = valid[26] & (pri26==3'h1);
assign req_p1[27] = valid[27] & (pri27==3'h1);
assign req_p1[28] = valid[28] & (pri28==3'h1);
assign req_p1[29] = valid[29] & (pri29==3'h1);
assign req_p1[30] = valid[30] & (pri30==3'h1);

assign req_p2[0] = valid[0] & (pri0==3'h2);
assign req_p2[1] = valid[1] & (pri1==3'h2);
assign req_p2[2] = valid[2] & (pri2==3'h2);
assign req_p2[3] = valid[3] & (pri3==3'h2);
assign req_p2[4] = valid[4] & (pri4==3'h2);
assign req_p2[5] = valid[5] & (pri5==3'h2);
assign req_p2[6] = valid[6] & (pri6==3'h2);
assign req_p2[7] = valid[7] & (pri7==3'h2);
assign req_p2[8] = valid[8] & (pri8==3'h2);
assign req_p2[9] = valid[9] & (pri9==3'h2);
assign req_p2[10] = valid[10] & (pri10==3'h2);
assign req_p2[11] = valid[11] & (pri11==3'h2);
assign req_p2[12] = valid[12] & (pri12==3'h2);
assign req_p2[13] = valid[13] & (pri13==3'h2);
assign req_p2[14] = valid[14] & (pri14==3'h2);
assign req_p2[15] = valid[15] & (pri15==3'h2);
assign req_p2[16] = valid[16] & (pri16==3'h2);
assign req_p2[17] = valid[17] & (pri17==3'h2);
assign req_p2[18] = valid[18] & (pri18==3'h2);
assign req_p2[19] = valid[19] & (pri19==3'h2);
assign req_p2[20] = valid[20] & (pri20==3'h2);
assign req_p2[21] = valid[21] & (pri21==3'h2);
assign req_p2[22] = valid[22] & (pri22==3'h2);
assign req_p2[23] = valid[23] & (pri23==3'h2);
assign req_p2[24] = valid[24] & (pri24==3'h2);
assign req_p2[25] = valid[25] & (pri25==3'h2);
assign req_p2[26] = valid[26] & (pri26==3'h2);
assign req_p2[27] = valid[27] & (pri27==3'h2);
assign req_p2[28] = valid[28] & (pri28==3'h2);
assign req_p2[29] = valid[29] & (pri29==3'h2);
assign req_p2[30] = valid[30] & (pri30==3'h2);

assign req_p3[0] = valid[0] & (pri0==3'h3);
assign req_p3[1] = valid[1] & (pri1==3'h3);
assign req_p3[2] = valid[2] & (pri2==3'h3);
assign req_p3[3] = valid[3] & (pri3==3'h3);
assign req_p3[4] = valid[4] & (pri4==3'h3);
assign req_p3[5] = valid[5] & (pri5==3'h3);
assign req_p3[6] = valid[6] & (pri6==3'h3);
assign req_p3[7] = valid[7] & (pri7==3'h3);
assign req_p3[8] = valid[8] & (pri8==3'h3);
assign req_p3[9] = valid[9] & (pri9==3'h3);
assign req_p3[10] = valid[10] & (pri10==3'h3);
assign req_p3[11] = valid[11] & (pri11==3'h3);
assign req_p3[12] = valid[12] & (pri12==3'h3);
assign req_p3[13] = valid[13] & (pri13==3'h3);
assign req_p3[14] = valid[14] & (pri14==3'h3);
assign req_p3[15] = valid[15] & (pri15==3'h3);
assign req_p3[16] = valid[16] & (pri16==3'h3);
assign req_p3[17] = valid[17] & (pri17==3'h3);
assign req_p3[18] = valid[18] & (pri18==3'h3);
assign req_p3[19] = valid[19] & (pri19==3'h3);
assign req_p3[20] = valid[20] & (pri20==3'h3);
assign req_p3[21] = valid[21] & (pri21==3'h3);
assign req_p3[22] = valid[22] & (pri22==3'h3);
assign req_p3[23] = valid[23] & (pri23==3'h3);
assign req_p3[24] = valid[24] & (pri24==3'h3);
assign req_p3[25] = valid[25] & (pri25==3'h3);
assign req_p3[26] = valid[26] & (pri26==3'h3);
assign req_p3[27] = valid[27] & (pri27==3'h3);
assign req_p3[28] = valid[28] & (pri28==3'h3);
assign req_p3[29] = valid[29] & (pri29==3'h3);
assign req_p3[30] = valid[30] & (pri30==3'h3);

assign req_p4[0] = valid[0] & (pri0==3'h4);
assign req_p4[1] = valid[1] & (pri1==3'h4);
assign req_p4[2] = valid[2] & (pri2==3'h4);
assign req_p4[3] = valid[3] & (pri3==3'h4);
assign req_p4[4] = valid[4] & (pri4==3'h4);
assign req_p4[5] = valid[5] & (pri5==3'h4);
assign req_p4[6] = valid[6] & (pri6==3'h4);
assign req_p4[7] = valid[7] & (pri7==3'h4);
assign req_p4[8] = valid[8] & (pri8==3'h4);
assign req_p4[9] = valid[9] & (pri9==3'h4);
assign req_p4[10] = valid[10] & (pri10==3'h4);
assign req_p4[11] = valid[11] & (pri11==3'h4);
assign req_p4[12] = valid[12] & (pri12==3'h4);
assign req_p4[13] = valid[13] & (pri13==3'h4);
assign req_p4[14] = valid[14] & (pri14==3'h4);
assign req_p4[15] = valid[15] & (pri15==3'h4);
assign req_p4[16] = valid[16] & (pri16==3'h4);
assign req_p4[17] = valid[17] & (pri17==3'h4);
assign req_p4[18] = valid[18] & (pri18==3'h4);
assign req_p4[19] = valid[19] & (pri19==3'h4);
assign req_p4[20] = valid[20] & (pri20==3'h4);
assign req_p4[21] = valid[21] & (pri21==3'h4);
assign req_p4[22] = valid[22] & (pri22==3'h4);
assign req_p4[23] = valid[23] & (pri23==3'h4);
assign req_p4[24] = valid[24] & (pri24==3'h4);
assign req_p4[25] = valid[25] & (pri25==3'h4);
assign req_p4[26] = valid[26] & (pri26==3'h4);
assign req_p4[27] = valid[27] & (pri27==3'h4);
assign req_p4[28] = valid[28] & (pri28==3'h4);
assign req_p4[29] = valid[29] & (pri29==3'h4);
assign req_p4[30] = valid[30] & (pri30==3'h4);

assign req_p5[0] = valid[0] & (pri0==3'h5);
assign req_p5[1] = valid[1] & (pri1==3'h5);
assign req_p5[2] = valid[2] & (pri2==3'h5);
assign req_p5[3] = valid[3] & (pri3==3'h5);
assign req_p5[4] = valid[4] & (pri4==3'h5);
assign req_p5[5] = valid[5] & (pri5==3'h5);
assign req_p5[6] = valid[6] & (pri6==3'h5);
assign req_p5[7] = valid[7] & (pri7==3'h5);
assign req_p5[8] = valid[8] & (pri8==3'h5);
assign req_p5[9] = valid[9] & (pri9==3'h5);
assign req_p5[10] = valid[10] & (pri10==3'h5);
assign req_p5[11] = valid[11] & (pri11==3'h5);
assign req_p5[12] = valid[12] & (pri12==3'h5);
assign req_p5[13] = valid[13] & (pri13==3'h5);
assign req_p5[14] = valid[14] & (pri14==3'h5);
assign req_p5[15] = valid[15] & (pri15==3'h5);
assign req_p5[16] = valid[16] & (pri16==3'h5);
assign req_p5[17] = valid[17] & (pri17==3'h5);
assign req_p5[18] = valid[18] & (pri18==3'h5);
assign req_p5[19] = valid[19] & (pri19==3'h5);
assign req_p5[20] = valid[20] & (pri20==3'h5);
assign req_p5[21] = valid[21] & (pri21==3'h5);
assign req_p5[22] = valid[22] & (pri22==3'h5);
assign req_p5[23] = valid[23] & (pri23==3'h5);
assign req_p5[24] = valid[24] & (pri24==3'h5);
assign req_p5[25] = valid[25] & (pri25==3'h5);
assign req_p5[26] = valid[26] & (pri26==3'h5);
assign req_p5[27] = valid[27] & (pri27==3'h5);
assign req_p5[28] = valid[28] & (pri28==3'h5);
assign req_p5[29] = valid[29] & (pri29==3'h5);
assign req_p5[30] = valid[30] & (pri30==3'h5);

assign req_p6[0] = valid[0] & (pri0==3'h6);
assign req_p6[1] = valid[1] & (pri1==3'h6);
assign req_p6[2] = valid[2] & (pri2==3'h6);
assign req_p6[3] = valid[3] & (pri3==3'h6);
assign req_p6[4] = valid[4] & (pri4==3'h6);
assign req_p6[5] = valid[5] & (pri5==3'h6);
assign req_p6[6] = valid[6] & (pri6==3'h6);
assign req_p6[7] = valid[7] & (pri7==3'h6);
assign req_p6[8] = valid[8] & (pri8==3'h6);
assign req_p6[9] = valid[9] & (pri9==3'h6);
assign req_p6[10] = valid[10] & (pri10==3'h6);
assign req_p6[11] = valid[11] & (pri11==3'h6);
assign req_p6[12] = valid[12] & (pri12==3'h6);
assign req_p6[13] = valid[13] & (pri13==3'h6);
assign req_p6[14] = valid[14] & (pri14==3'h6);
assign req_p6[15] = valid[15] & (pri15==3'h6);
assign req_p6[16] = valid[16] & (pri16==3'h6);
assign req_p6[17] = valid[17] & (pri17==3'h6);
assign req_p6[18] = valid[18] & (pri18==3'h6);
assign req_p6[19] = valid[19] & (pri19==3'h6);
assign req_p6[20] = valid[20] & (pri20==3'h6);
assign req_p6[21] = valid[21] & (pri21==3'h6);
assign req_p6[22] = valid[22] & (pri22==3'h6);
assign req_p6[23] = valid[23] & (pri23==3'h6);
assign req_p6[24] = valid[24] & (pri24==3'h6);
assign req_p6[25] = valid[25] & (pri25==3'h6);
assign req_p6[26] = valid[26] & (pri26==3'h6);
assign req_p6[27] = valid[27] & (pri27==3'h6);
assign req_p6[28] = valid[28] & (pri28==3'h6);
assign req_p6[29] = valid[29] & (pri29==3'h6);
assign req_p6[30] = valid[30] & (pri30==3'h6);

assign req_p7[0] = valid[0] & (pri0==3'h7);
assign req_p7[1] = valid[1] & (pri1==3'h7);
assign req_p7[2] = valid[2] & (pri2==3'h7);
assign req_p7[3] = valid[3] & (pri3==3'h7);
assign req_p7[4] = valid[4] & (pri4==3'h7);
assign req_p7[5] = valid[5] & (pri5==3'h7);
assign req_p7[6] = valid[6] & (pri6==3'h7);
assign req_p7[7] = valid[7] & (pri7==3'h7);
assign req_p7[8] = valid[8] & (pri8==3'h7);
assign req_p7[9] = valid[9] & (pri9==3'h7);
assign req_p7[10] = valid[10] & (pri10==3'h7);
assign req_p7[11] = valid[11] & (pri11==3'h7);
assign req_p7[12] = valid[12] & (pri12==3'h7);
assign req_p7[13] = valid[13] & (pri13==3'h7);
assign req_p7[14] = valid[14] & (pri14==3'h7);
assign req_p7[15] = valid[15] & (pri15==3'h7);
assign req_p7[16] = valid[16] & (pri16==3'h7);
assign req_p7[17] = valid[17] & (pri17==3'h7);
assign req_p7[18] = valid[18] & (pri18==3'h7);
assign req_p7[19] = valid[19] & (pri19==3'h7);
assign req_p7[20] = valid[20] & (pri20==3'h7);
assign req_p7[21] = valid[21] & (pri21==3'h7);
assign req_p7[22] = valid[22] & (pri22==3'h7);
assign req_p7[23] = valid[23] & (pri23==3'h7);
assign req_p7[24] = valid[24] & (pri24==3'h7);
assign req_p7[25] = valid[25] & (pri25==3'h7);
assign req_p7[26] = valid[26] & (pri26==3'h7);
assign req_p7[27] = valid[27] & (pri27==3'h7);
assign req_p7[28] = valid[28] & (pri28==3'h7);
assign req_p7[29] = valid[29] & (pri29==3'h7);
assign req_p7[30] = valid[30] & (pri30==3'h7);

// RR Arbiter for priority 0
wb_dma_ch_arb u1(
	.clk(		clk		),
	.rst(		rst		),
	.req(		req_p0		),
	.gnt(		gnt_p0_d	),
	.advance(	next_ch		)
	);
// RR Arbiter for priority 1
wb_dma_ch_arb u2(
	.clk(		clk		),
	.rst(		rst		),
	.req(		req_p1		),
	.gnt(		gnt_p1_d	),
	.advance(	next_ch		)
	);

// RR Arbiter for priority 2
wb_dma_ch_arb u3(
	.clk(		clk		),
	.rst(		rst		),
	.req(		req_p2		),
	.gnt(		gnt_p2_d	),
	.advance(	next_ch		)
	);
// RR Arbiter for priority 3
wb_dma_ch_arb u4(
	.clk(		clk		),
	.rst(		rst		),
	.req(		req_p3		),
	.gnt(		gnt_p3_d	),
	.advance(	next_ch		)
	);
// RR Arbiter for priority 4
wb_dma_ch_arb u5(
	.clk(		clk		),
	.rst(		rst		),
	.req(		req_p4		),
	.gnt(		gnt_p4_d	),
	.advance(	next_ch		)
	);
// RR Arbiter for priority 5
wb_dma_ch_arb u6(
	.clk(		clk		),
	.rst(		rst		),
	.req(		req_p5		),
	.gnt(		gnt_p5_d	),
	.advance(	next_ch		)
	);
// RR Arbiter for priority 6
wb_dma_ch_arb u7(
	.clk(		clk		),
	.rst(		rst		),
	.req(		req_p6		),
	.gnt(		gnt_p6_d	),
	.advance(	next_ch		)
	);
// RR Arbiter for priority 7
wb_dma_ch_arb u8(
	.clk(		clk		),
	.rst(		rst		),
	.req(		req_p7		),
	.gnt(		gnt_p7_d	),
	.advance(	next_ch		)
	);

// Select grant based on number of priorities
assign gnt_p0 = gnt_p0_d;
assign gnt_p1 = gnt_p1_d;
assign gnt_p2 = (pri_sel==2'd0) ? 5'h0 : gnt_p2_d;
assign gnt_p3 = (pri_sel==2'd0) ? 5'h0 : gnt_p3_d;
assign gnt_p4 = (pri_sel==2'd2) ? gnt_p4_d : 5'h0;
assign gnt_p5 = (pri_sel==2'd2) ? gnt_p5_d : 5'h0;
assign gnt_p6 = (pri_sel==2'd2) ? gnt_p6_d : 5'h0;
assign gnt_p7 = (pri_sel==2'd2) ? gnt_p7_d : 5'h0;

endmodule
