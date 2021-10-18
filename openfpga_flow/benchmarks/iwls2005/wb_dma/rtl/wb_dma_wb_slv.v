/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE DMA WISHBONE Slave Interface                      ////
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
//  $Id: wb_dma_wb_slv.v,v 1.4 2002/02/01 01:54:45 rudi Exp $
//
//  $Date: 2002/02/01 01:54:45 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: wb_dma_wb_slv.v,v $
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
//               Revision 1.2  2001/06/05 10:22:37  rudi
//
//
//               - Added Support of up to 31 channels
//               - Added support for 2,4 and 8 priority levels
//               - Now can have up to 31 channels
//               - Added many configuration items
//               - Changed reset to async
//
//               Revision 1.1.1.1  2001/03/19 13:10:59  rudi
//               Initial Release
//
//
//

`include "wb_dma_defines.v"

module wb_dma_wb_slv(clk, rst,

	wb_data_i, wb_data_o, wb_addr_i, wb_sel_i, wb_we_i, wb_cyc_i,
	wb_stb_i, wb_ack_o, wb_err_o, wb_rty_o,

	// This is the register File Interface
	slv_adr, slv_din, slv_dout, slv_re, slv_we,

	// Pass through Interface
	pt_sel, slv_pt_out, slv_pt_in

	);

parameter	rf_addr = 0;

input		clk, rst;

// --------------------------------------
// WISHBONE INTERFACE 

input	[31:0]	wb_data_i;
output	[31:0]	wb_data_o;
input	[31:0]	wb_addr_i;
input	[3:0]	wb_sel_i;
input		wb_we_i;
input		wb_cyc_i;
input		wb_stb_i;
output		wb_ack_o;
output		wb_err_o;
output		wb_rty_o;

// This is the register File Interface
output	[31:0]	slv_adr;	// Slave Address
input	[31:0]	slv_din;	// Slave Input Data
output	[31:0]	slv_dout;	// Slave Output Data
output		slv_re;		// Slave Read Enable
output		slv_we;		// Slave Write Enable

// Pass through Interface
output		pt_sel;		// Pass Through Mode Active
output	[70:0]	slv_pt_out;	// Grouped WISHBONE out signals
input	[34:0]	slv_pt_in;	// Grouped WISHBONE in signals

////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg		slv_re, slv_we;
wire		rf_sel;
reg		rf_ack;
reg	[31:0]	slv_adr, slv_dout;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

assign rf_sel = `WDMA_REG_SEL ;

////////////////////////////////////////////////////////////////////
//
// Pass Through Logic
//

//assign pt_sel = !rf_sel;
assign pt_sel = !rf_sel & wb_cyc_i;

assign slv_pt_out = {wb_data_i, wb_addr_i, wb_sel_i, wb_we_i, wb_cyc_i, wb_stb_i};
assign {wb_data_o, wb_ack_o, wb_err_o, wb_rty_o} = pt_sel ? slv_pt_in :
	{slv_din, rf_ack, 1'b0, 1'b0};

////////////////////////////////////////////////////////////////////
//
// Register File Logic
//

always @(posedge clk)
	slv_adr <= #1 wb_addr_i;

always @(posedge clk)
	slv_re <= #1 rf_sel & wb_cyc_i & wb_stb_i & !wb_we_i & !rf_ack & !slv_re;

always @(posedge clk)
	slv_we <= #1 rf_sel & wb_cyc_i & wb_stb_i &  wb_we_i & !rf_ack;

always @(posedge clk)
	slv_dout <= #1 wb_data_i;

always @(posedge clk)
	rf_ack <= #1 (slv_re | slv_we) & wb_cyc_i & wb_stb_i & !rf_ack ;

endmodule
