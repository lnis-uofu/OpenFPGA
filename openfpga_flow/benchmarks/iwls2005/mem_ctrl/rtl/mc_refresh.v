/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE Memory Controller                                 ////
////  Refresh Module                                             ////
////                                                             ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/mem_ctrl/  ////
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
//  $Id: mc_refresh.v,v 1.4 2002/01/21 13:08:52 rudi Exp $
//
//  $Date: 2002/01/21 13:08:52 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: mc_refresh.v,v $
//               Revision 1.4  2002/01/21 13:08:52  rudi
//
//               Fixed several minor bugs, cleaned up the code further ...
//
//               Revision 1.3  2001/12/11 02:47:19  rudi
//
//               - Made some changes not to expect clock during reset ...
//
//               Revision 1.2  2001/09/24 00:38:21  rudi
//
//               Changed Reset to be active high and async.
//
//               Revision 1.1  2001/07/29 07:34:41  rudi
//
//
//               1) Changed Directory Structure
//               2) Fixed several minor bugs
//
//               Revision 1.3  2001/06/12 15:19:49  rudi
//
//
//               Minor changes after running lint, and a small bug fix reading csr and ba_mask registers.
//
//               Revision 1.2  2001/06/03 11:37:17  rudi
//
//
//               1) Fixed Chip Select Mask Register
//               	- Power On Value is now all ones
//               	- Comparison Logic is now correct
//
//               2) All resets are now asynchronous
//
//               3) Converted Power On Delay to an configurable item
//
//               4) Added reset to Chip Select Output Registers
//
//               5) Forcing all outputs to Hi-Z state during reset
//
//               Revision 1.1.1.1  2001/05/13 09:39:47  rudi
//               Created Directory Structure
//
//
//
//

`include "mc_defines.v"

module mc_refresh(clk, rst, 
		cs_need_rfr, ref_int, rfr_req, rfr_ack,
		rfr_ps_val
		);

input		clk, rst;
input	[7:0]	cs_need_rfr;
input	[2:0]	ref_int;
output		rfr_req;
input		rfr_ack;
input	[7:0]	rfr_ps_val;

////////////////////////////////////////////////////////////////////
//
// Local Registers & Wires
//

reg		rfr_en;
reg	[7:0]	ps_cnt;
wire		ps_cnt_clr;
reg		rfr_ce;
reg	[7:0]	rfr_cnt;
reg		rfr_clr;
reg		rfr_req;
reg		rfr_early;

/*
Refresh generation

The prescaler generates a 0.48828 uS clock enable

The refresh counter generates the following refresh rates:
(Actual values are about 0.63% below the desired values).
This is for a 200 Mhz WISHBONE Bus.
0.970 uS,
1.940
3.880
7.760
15.520
32.040
62.080
124.160 uS

(desired values)
0.976 uS
1.953
3.906
7.812
15.625
31.250
62.500
125.000 uS
*/

////////////////////////////////////////////////////////////////////
//
// Prescaler
//

always @(posedge clk or posedge rst)
	if(rst)		rfr_en <= #1 1'b0;
	else		rfr_en <= #1 |cs_need_rfr;

always @(posedge clk or posedge rst)
	if(rst)				ps_cnt <= #1 8'h0;
	else	
	if(ps_cnt_clr)			ps_cnt <= #1 8'h0;
	else	
	if(rfr_en)			ps_cnt <= #1 ps_cnt + 8'h1;

assign ps_cnt_clr = (ps_cnt == rfr_ps_val) & (rfr_ps_val != 8'h0);

always @(posedge clk or posedge rst)
	if(rst)		rfr_early <= #1 1'b0;
	else		rfr_early <= #1 (ps_cnt == rfr_ps_val);

////////////////////////////////////////////////////////////////////
//
// Refresh Counter
//

always @(posedge clk or posedge rst)
	if(rst)		rfr_ce <= #1 1'b0;
	else		rfr_ce <= #1 ps_cnt_clr;

always @(posedge clk or posedge rst)
	if(rst)			rfr_cnt <= #1 8'h0;
	else
	if(rfr_ack)		rfr_cnt <= #1 8'h0;
	else
	if(rfr_ce)		rfr_cnt <= #1 rfr_cnt + 8'h1;

always @(posedge clk)
	case(ref_int)		// synopsys full_case parallel_case
	   3'h0: rfr_clr <= #1  rfr_cnt[0]   & rfr_early;
	   3'h1: rfr_clr <= #1 &rfr_cnt[1:0] & rfr_early;
	   3'h2: rfr_clr <= #1 &rfr_cnt[2:0] & rfr_early;
	   3'h3: rfr_clr <= #1 &rfr_cnt[3:0] & rfr_early;
	   3'h4: rfr_clr <= #1 &rfr_cnt[4:0] & rfr_early;
	   3'h5: rfr_clr <= #1 &rfr_cnt[5:0] & rfr_early;
	   3'h6: rfr_clr <= #1 &rfr_cnt[6:0] & rfr_early;
	   3'h7: rfr_clr <= #1 &rfr_cnt[7:0] & rfr_early;
	endcase

always @(posedge clk or posedge rst)
	if(rst)			rfr_req <= #1 1'b0;
	else
	if(rfr_ack)		rfr_req <= #1 1'b0;
	else
	if(rfr_clr)		rfr_req <= #1 1'b1;

endmodule
