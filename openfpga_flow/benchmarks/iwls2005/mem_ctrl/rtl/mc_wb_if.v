/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE Memory Controller                                 ////
////  WISHBONE Interface                                         ////
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
//  $Id: mc_wb_if.v,v 1.6 2002/01/21 13:08:52 rudi Exp $
//
//  $Date: 2002/01/21 13:08:52 $
//  $Revision: 1.6 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: mc_wb_if.v,v $
//               Revision 1.6  2002/01/21 13:08:52  rudi
//
//               Fixed several minor bugs, cleaned up the code further ...
//
//               Revision 1.5  2001/12/11 02:47:19  rudi
//
//               - Made some changes not to expect clock during reset ...
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
//               Revision 1.2  2001/08/10 08:16:21  rudi
//
//               - Changed IO names to be more clear.
//               - Uniquifyed define names to be core specific.
//               - Removed "Refresh Early" configuration
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
//              Minor changes after running lint, and a small bug
//		fix reading csr and ba_mask registers.
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

module mc_wb_if(clk, rst,
		wb_addr_i, wb_cyc_i, wb_stb_i, wb_we_i, wb_err, wb_ack_o,
		wb_read_go, wb_write_go, 
		wb_first, wb_wait, mem_ack, wr_hold,
		err, par_err, wp_err,
		wb_data_o, mem_dout, rf_dout);

input		clk, rst;
input	[31:0]	wb_addr_i;
input		wb_cyc_i;
input		wb_stb_i;
input		wb_we_i;
output		wb_err;
output		wb_ack_o;
output		wb_read_go;
output		wb_write_go;
output		wb_first;
output		wb_wait;
input		mem_ack;
output		wr_hold;
input		err, par_err, wp_err;
output	[31:0]	wb_data_o;
input	[31:0]	mem_dout, rf_dout;

////////////////////////////////////////////////////////////////////
//
// Local Wires and Registers
//

wire		mem_sel;
reg		read_go_r;
reg		read_go_r1;
reg		write_go_r;
reg		write_go_r1;
reg		wb_first_r;
wire		wb_first_set;
reg		wr_hold;
wire		rmw;
reg		rmw_r;
reg		rmw_en;
reg		wb_ack_o;
reg		wb_err;
reg	[31:0]	wb_data_o;

////////////////////////////////////////////////////////////////////
//
// Memory Go Logic
//

assign mem_sel = `MC_MEM_SEL;

always @(posedge clk or posedge rst)
	if(rst)			rmw_en <= #1 1'b0;
	else
	if(wb_ack_o)		rmw_en <= #1 1'b1;
	else
	if(!wb_cyc_i)		rmw_en <= #1 1'b0;

always @(posedge clk or posedge rst)
	if(rst)	rmw_r <= #1 1'b0;
	else	rmw_r <= #1 !wr_hold & wb_we_i & wb_cyc_i & wb_stb_i & rmw_en;

assign rmw = rmw_r | (!wr_hold & wb_we_i & wb_cyc_i & wb_stb_i & rmw_en);

always @(posedge clk or posedge rst)
	if(rst)	read_go_r1 <= #1 1'b0;
	else	read_go_r1 <= #1 !rmw & wb_cyc_i &
				((wb_stb_i & mem_sel & !wb_we_i) | read_go_r);

always @(posedge clk or posedge rst)
	if(rst)	read_go_r <= #1 1'b0;
	else	read_go_r <= #1 read_go_r1 & wb_cyc_i;

assign	wb_read_go = !rmw & read_go_r1 & wb_cyc_i;

always @(posedge clk or posedge rst)
	if(rst)	write_go_r1 <= #1 1'b0;
	else	write_go_r1 <= #1 wb_cyc_i &
				((wb_stb_i & mem_sel & wb_we_i) | write_go_r);

always @(posedge clk or posedge rst)
	if(rst)		write_go_r <= #1 1'b0;
	else		write_go_r <= #1 write_go_r1 & wb_cyc_i &
					((wb_we_i & wb_stb_i) | !wb_stb_i);

assign wb_write_go =	!rmw & write_go_r1 & wb_cyc_i &
			((wb_we_i & wb_stb_i) | !wb_stb_i);

assign wb_first_set = mem_sel & wb_cyc_i & wb_stb_i & !(read_go_r | write_go_r);
assign wb_first = wb_first_set | (wb_first_r & !wb_ack_o & !wb_err);

always @(posedge clk or posedge rst)
	if(rst)			wb_first_r <= #1 1'b0;
	else
	if(wb_first_set)	wb_first_r <= #1 1'b1;
	else
	if(wb_ack_o | wb_err)	wb_first_r <= #1 1'b0;

always @(posedge clk or posedge rst)
	if(rst)			wr_hold <= #1 1'b0;
	else
	if(wb_cyc_i & wb_stb_i)	wr_hold <= #1 wb_we_i;

////////////////////////////////////////////////////////////////////
//
// WB Ack
//

wire	wb_err_d;

// Ack no longer asserted when wb_err is asserted
always @(posedge clk or posedge rst)
	if(rst)	wb_ack_o <= #1 1'b0;
	else	wb_ack_o <= #1 `MC_MEM_SEL ? mem_ack & !wb_err_d :
				`MC_REG_SEL & wb_cyc_i & wb_stb_i & !wb_ack_o;

assign wb_err_d = wb_cyc_i & wb_stb_i & (par_err | err | wp_err);

always @(posedge clk or posedge rst)
	if(rst)	wb_err <= #1 1'b0;
	else	wb_err <= #1 `MC_MEM_SEL & wb_err_d & !wb_err;

////////////////////////////////////////////////////////////////////
//
// Memory Wait Logic
//

assign wb_wait = wb_cyc_i & !wb_stb_i & (wb_write_go | wb_read_go);

////////////////////////////////////////////////////////////////////
//
// WISHBONE Data Output
//

always @(posedge clk)
	wb_data_o <= #1 `MC_MEM_SEL ? mem_dout : rf_dout;

endmodule
