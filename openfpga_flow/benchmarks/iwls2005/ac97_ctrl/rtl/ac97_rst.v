/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller Reset Module                     ////
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
//// Copyright (C) 2001 Rudolf Usselmann                         ////
////                    rudi@asics.ws                            ////
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
//  $Id: ac97_rst.v,v 1.1 2001/08/03 06:54:50 rudi Exp $
//
//  $Date: 2001/08/03 06:54:50 $
//  $Revision: 1.1 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_rst.v,v $
//               Revision 1.1  2001/08/03 06:54:50  rudi
//
//
//               - Changed to new directory structure
//
//               Revision 1.1.1.1  2001/05/19 02:29:19  rudi
//               Initial Checkin
//
//
//
//

`include "ac97_defines.v"

module ac97_rst(clk, rst, rst_force, ps_ce, ac97_rst_);
input		clk, rst;
input		rst_force;
output		ps_ce;
output		ac97_rst_;

reg		ac97_rst_;
reg	[2:0]	cnt;
wire		ce;
wire		to;
reg	[5:0]	ps_cnt;
wire		ps_ce;

always @(posedge clk or negedge rst)
	if(!rst)	ac97_rst_ <= #1 0;
	else
	if(rst_force)	ac97_rst_ <= #1 0;
	else
	if(to)		ac97_rst_ <= #1 1;

assign to = (cnt == `AC97_RST_DEL);

always @(posedge clk or negedge rst)
	if(!rst)	cnt <= #1 0;
	else
	if(rst_force)	cnt <= #1 0;
	else
	if(ce)		cnt <= #1 cnt + 1;

assign ce = ps_ce & (cnt != `AC97_RST_DEL);

always @(posedge clk or negedge rst)
	if(!rst)		ps_cnt <= #1 0;
	else
	if(ps_ce | rst_force)	ps_cnt <= #1 0;
	else			ps_cnt <= #1 ps_cnt + 1;

assign ps_ce = (ps_cnt == `AC97_250_PS);

endmodule
