/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE DMA WISHBONE Master Interface                     ////
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
//  $Id: wb_dma_wb_mast.v,v 1.2 2002/02/01 01:54:45 rudi Exp $
//
//  $Date: 2002/02/01 01:54:45 $
//  $Revision: 1.2 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: wb_dma_wb_mast.v,v $
//               Revision 1.2  2002/02/01 01:54:45  rudi
//
//               - Minor cleanup
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
//               Revision 1.1.1.1  2001/03/19 13:11:05  rudi
//               Initial Release
//
//
//

`include "wb_dma_defines.v"

module wb_dma_wb_mast(clk, rst,

	wb_data_i, wb_data_o, wb_addr_o, wb_sel_o, wb_we_o, wb_cyc_o,
	wb_stb_o, wb_ack_i, wb_err_i, wb_rty_i,

	mast_go, mast_we, mast_adr, mast_din, mast_dout, mast_err,
	mast_drdy, mast_wait,

	pt_sel, mast_pt_in, mast_pt_out
	);

input		clk, rst;

// --------------------------------------
// WISHBONE INTERFACE 

input	[31:0]	wb_data_i;
output	[31:0]	wb_data_o;
output	[31:0]	wb_addr_o;
output	[3:0]	wb_sel_o;
output		wb_we_o;
output		wb_cyc_o;
output		wb_stb_o;
input		wb_ack_i;
input		wb_err_i;
input		wb_rty_i;

// --------------------------------------
// INTERNAL DMA INTERFACE 
input		mast_go;	// Perform a Master Cycle (as long as this
				// line is asserted)
input		mast_we;	// Read/Write
input	[31:0]	mast_adr;	// Address for the transfer
input	[31:0]	mast_din;	// Internal Input Data
output	[31:0]	mast_dout;	// Internal Output Data
output		mast_err;	// Indicates an error has occurred

output		mast_drdy;	// Indicated that either data is available
				// during a read, or that the master can accept
				// the next data during a write
input		mast_wait;	// Tells the master to insert wait cycles
				// because data can not be accepted/provided

// Pass Through Interface
input		pt_sel;		// Pass Through Mode Selected
input	[70:0]	mast_pt_in;	// Grouped WISHBONE inputs
output	[34:0]	mast_pt_out;	// Grouped WISHBONE outputs

////////////////////////////////////////////////////////////////////
//
// Local Wires
//

reg		mast_cyc, mast_stb;
reg		mast_we_r;
reg	[3:0]	mast_be;
reg	[31:0]	mast_dout;

////////////////////////////////////////////////////////////////////
//
// Pass-Through Interface
//

assign {wb_data_o, wb_addr_o, wb_sel_o, wb_we_o, wb_cyc_o, wb_stb_o} =
	pt_sel ? mast_pt_in :
	{mast_din, mast_adr, mast_be, mast_we_r, mast_cyc, mast_stb};

assign mast_pt_out = {wb_data_i, wb_ack_i, wb_err_i, wb_rty_i};

////////////////////////////////////////////////////////////////////
//
// DMA Engine Interface
//

always @(posedge clk)
	if(wb_ack_i)	mast_dout <= #1 wb_data_i;

always @(posedge clk)
	mast_be <= #1 4'hf;

always @(posedge clk)
	mast_we_r <= #1 mast_we;

always @(posedge clk)
	mast_cyc <= #1 mast_go;

always @(posedge clk)
	mast_stb <= #1 mast_go & !mast_wait; 

assign mast_drdy = wb_ack_i;
assign mast_err  = wb_err_i;

endmodule
