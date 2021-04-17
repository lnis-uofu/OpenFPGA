/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE Memory Controller                                 ////
////  Memory Bus Interface                                       ////
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
//  $Id: mc_mem_if.v,v 1.6 2002/01/21 13:08:52 rudi Exp $
//
//  $Date: 2002/01/21 13:08:52 $
//  $Revision: 1.6 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: mc_mem_if.v,v $
//               Revision 1.6  2002/01/21 13:08:52  rudi
//
//               Fixed several minor bugs, cleaned up the code further ...
//
//               Revision 1.5  2001/12/21 05:09:29  rudi
//
//               - Fixed combinatorial loops in synthesis
//               - Fixed byte select bug
//
//               Revision 1.4  2001/11/29 02:16:28  rudi
//
//
//               - More Synthesis cleanup, mostly for speed
//               - Several bug fixes
//               - Changed code to avoid auto-precharge and
//                 burst-terminate combinations (apparently illegal ?)
//                 Now we will do a manual precharge ...
//
//               Revision 1.3  2001/09/24 00:38:21  rudi
//
//               Changed Reset to be active high and async.
//
//               Revision 1.2  2001/09/02 02:28:28  rudi
//
//               Many fixes for minor bugs that showed up in gate level simulations.
//
//               Revision 1.1  2001/07/29 07:34:41  rudi
//
//
//               1) Changed Directory Structure
//               2) Fixed several minor bugs
//
//               Revision 1.3  2001/06/14 01:57:37  rudi
//
//
//               Fixed a potential bug in a corner case situation where the TMS register
//               does not propegate properly during initialisation.
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
//               Revision 1.1.1.1  2001/05/13 09:39:48  rudi
//               Created Directory Structure
//
//
//
//

`include "mc_defines.v"

module mc_mem_if(clk, rst, mc_clk, mc_br, mc_bg, 
		mc_addr, mc_data_o, mc_dp_o, mc_data_oe,
		mc_dqm, mc_oe_, mc_we_, mc_cas_, mc_ras_, mc_cke_, mc_cs_,
		mc_adsc_, mc_adv_, mc_ack, mc_rp, mc_c_oe, mc_c_oe_d,
		mc_br_r, mc_bg_d, mc_data_od, mc_dp_od, mc_addr_d, mc_ack_r,
		we_, ras_, cas_, cke_, mc_adsc_d, mc_adv_d, cs_en, rfr_ack,
		cs_need_rfr, lmr_sel, spec_req_cs, cs, fs, data_oe, susp_sel,
		suspended_o, oe_, wb_cyc_i, wb_stb_i, wb_sel_i, wb_cycle,
		wr_cycle, mc_data_ir, mc_data_i, mc_dp_i, mc_sts_ir, mc_sts_i,
		mc_zz_o
		);
// Memory Interface
input		clk;
input		rst;
input		mc_clk;
input		mc_br;
output		mc_bg;
output	[23:0]	mc_addr;
output	[31:0]	mc_data_o;
output	[3:0]	mc_dp_o;
output		mc_data_oe;
output	[3:0]	mc_dqm;
output		mc_oe_;
output		mc_we_;
output		mc_cas_;
output		mc_ras_;
output		mc_cke_;
output	[7:0]	mc_cs_;
output		mc_adsc_;
output		mc_adv_;
input		mc_ack;
output		mc_rp;
output		mc_c_oe;
output	[35:0]	mc_data_ir;
output		mc_sts_ir;
output		mc_zz_o;

// Internal Interface
output		mc_br_r;
input		mc_bg_d;
input		data_oe;
input		susp_sel;
input		suspended_o;
input	[31:0]	mc_data_od;
input	[3:0]	mc_dp_od;
input	[23:0]	mc_addr_d;
output		mc_ack_r;
input		wb_cyc_i;
input		wb_stb_i;
input	[3:0]	wb_sel_i;
input		wb_cycle;
input		wr_cycle;
input		oe_ ;
input		we_;
input		ras_;
input		cas_;
input		cke_;
input		cs_en;
input		rfr_ack;
input	[7:0]	cs_need_rfr;
input		lmr_sel;
input	[7:0]	spec_req_cs;
input	[7:0]	cs;
input		fs;
input		mc_adsc_d;
input		mc_adv_d;
input		mc_c_oe_d;
input	[31:0]	mc_data_i;
input	[3:0]	mc_dp_i;
input		mc_sts_i;

////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg		mc_data_oe;
reg	[31:0]	mc_data_o;
reg	[3:0]	mc_dp_o;
reg	[3:0]	mc_dqm;
reg	[3:0]	mc_dqm_r;
reg	[23:0]	mc_addr;
reg		mc_oe_;
reg		mc_we_;
reg		mc_cas_;
reg		mc_ras_;
wire		mc_cke_;
reg	[7:0]	mc_cs_;
reg		mc_bg;
reg		mc_adsc_;
reg		mc_adv_;
reg		mc_br_r;
reg		mc_ack_r;
reg		mc_rp;
reg		mc_c_oe;
reg		mc_zz_o;

reg	[35:0]	mc_data_ir;
reg		mc_sts_ir;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

always @(posedge mc_clk)
	mc_zz_o <= #1 suspended_o;	

always @(posedge mc_clk)
	mc_sts_ir <= #1 mc_sts_i;

always @(posedge mc_clk)
	mc_data_ir <= #1 {mc_dp_i, mc_data_i};

always @(posedge mc_clk)
	mc_c_oe <= #1 mc_c_oe_d;

always @(posedge mc_clk)
	mc_rp <= #1 !suspended_o & !fs;

always @(posedge mc_clk)
	mc_br_r <= #1 mc_br;

always @(posedge mc_clk)
	mc_ack_r <= #1 mc_ack;

always @(posedge mc_clk)
	mc_bg <= #1 mc_bg_d;

always @(posedge mc_clk or posedge rst)
	if(rst)		mc_data_oe <= #1 1'b0;
	else		mc_data_oe <= #1 data_oe & !susp_sel & mc_c_oe_d;

always @(posedge mc_clk)
	mc_data_o <= #1 mc_data_od;

always @(posedge mc_clk)
	mc_dp_o <= #1 mc_dp_od;

always @(posedge mc_clk)
	mc_addr <= #1 mc_addr_d;

always @(posedge clk)
	if(wb_cyc_i & wb_stb_i)
		mc_dqm_r <= #1 wb_sel_i;

reg	[3:0]	mc_dqm_r2;
always @(posedge clk)
		mc_dqm_r2 <= #1 mc_dqm_r;

always @(posedge mc_clk)
	mc_dqm <= #1	susp_sel ? 4'hf :
			data_oe ? ~mc_dqm_r2 :
			(wb_cycle & !wr_cycle) ? 4'h0 : 4'hf;

always @(posedge mc_clk or posedge rst)
	if(rst)		mc_oe_ <= #1 1'b1;
	else		mc_oe_ <= #1 oe_ | susp_sel;

always @(posedge mc_clk)
	mc_we_ <= #1 we_;

always @(posedge mc_clk)
	mc_cas_ <= #1 cas_;

always @(posedge mc_clk)
	mc_ras_ <= #1 ras_;

assign	mc_cke_ = cke_;

always @(posedge mc_clk or posedge rst)
	if(rst)		mc_cs_[0] <= #1 1'b1;
	else
	mc_cs_[0] <= #1 ~(cs_en & (
				(rfr_ack | susp_sel) ? cs_need_rfr[0] :
				lmr_sel ? spec_req_cs[0] :
				cs[0]
			));

always @(posedge mc_clk or posedge rst)
	if(rst)		mc_cs_[1] <= #1 1'b1;
	else
	   mc_cs_[1] <= #1 ~(cs_en & (
				(rfr_ack | susp_sel) ? cs_need_rfr[1] :
				lmr_sel ? spec_req_cs[1] :
				cs[1]
			));

always @(posedge mc_clk or posedge rst)
	if(rst)		mc_cs_[2] <= #1 1'b1;
	else
	   mc_cs_[2] <= #1 ~(cs_en & (
				(rfr_ack | susp_sel) ? cs_need_rfr[2] :
				lmr_sel ? spec_req_cs[2] :
				cs[2]
			));

always @(posedge mc_clk or posedge rst)
	if(rst)		mc_cs_[3] <= #1 1'b1;
	else
	   mc_cs_[3] <= #1 ~(cs_en & (
				(rfr_ack | susp_sel) ? cs_need_rfr[3] :
				lmr_sel ? spec_req_cs[3] :
				cs[3]
			));

always @(posedge mc_clk or posedge rst)
	if(rst)		mc_cs_[4] <= #1 1'b1;
	else
	   mc_cs_[4] <= #1 ~(cs_en & (
				(rfr_ack | susp_sel) ? cs_need_rfr[4] :
				lmr_sel ? spec_req_cs[4] :
				cs[4]
			));

always @(posedge mc_clk or posedge rst)
	if(rst)		mc_cs_[5] <= #1 1'b1;
	else
	   mc_cs_[5] <= #1 ~(cs_en & (
				(rfr_ack | susp_sel) ? cs_need_rfr[5] :
				lmr_sel ? spec_req_cs[5] :
				cs[5]
			));

always @(posedge mc_clk or posedge rst)
	if(rst)		mc_cs_[6] <= #1 1'b1;
	else
	   mc_cs_[6] <= #1 ~(cs_en & (
				(rfr_ack | susp_sel) ? cs_need_rfr[6] :
				lmr_sel ? spec_req_cs[6] :
				cs[6]
			));

always @(posedge mc_clk or posedge rst)
	if(rst)		mc_cs_[7] <= #1 1'b1;
	else
	   mc_cs_[7] <= #1 ~(cs_en & (
				(rfr_ack | susp_sel) ? cs_need_rfr[7] :
				lmr_sel ? spec_req_cs[7] :
				cs[7]
			));

always @(posedge mc_clk)
	mc_adsc_ <= #1 ~mc_adsc_d;

always @(posedge mc_clk)
	mc_adv_  <= #1 ~mc_adv_d;

endmodule
