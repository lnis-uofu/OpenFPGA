/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE DMA WISHBONE Interface                            ////
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
//  $Id: wb_dma_wb_if.v,v 1.3 2002/02/01 01:54:45 rudi Exp $
//
//  $Date: 2002/02/01 01:54:45 $
//  $Revision: 1.3 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: wb_dma_wb_if.v,v $
//               Revision 1.3  2002/02/01 01:54:45  rudi
//
//               - Minor cleanup
//
//               Revision 1.2  2001/10/19 04:35:04  rudi
//
//               - Made the core parameterized
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
//               Revision 1.1.1.1  2001/03/19 13:10:54  rudi
//               Initial Release
//
//
//

`include "wb_dma_defines.v"

module wb_dma_wb_if(clk, rst,

	// Wishbone
	wbs_data_i, wbs_data_o, wb_addr_i, wb_sel_i, wb_we_i, wb_cyc_i,
	wb_stb_i, wb_ack_o, wb_err_o, wb_rty_o,
	wbm_data_i, wbm_data_o, wb_addr_o, wb_sel_o, wb_we_o, wb_cyc_o,
	wb_stb_o, wb_ack_i, wb_err_i, wb_rty_i,

	// Master
	mast_go, mast_we, mast_adr, mast_din, mast_dout, mast_err,
	mast_drdy, mast_wait, pt_sel_i, mast_pt_in, mast_pt_out,

	// Slave
	slv_adr, slv_din, slv_dout, slv_re, slv_we,
	pt_sel_o, slv_pt_out, slv_pt_in

	);

parameter	rf_addr = 0;

input		clk, rst;

// --------------------------------------
// WISHBONE INTERFACE 

// Slave Interface
input	[31:0]	wbs_data_i;
output	[31:0]	wbs_data_o;
input	[31:0]	wb_addr_i;
input	[3:0]	wb_sel_i;
input		wb_we_i;
input		wb_cyc_i;
input		wb_stb_i;
output		wb_ack_o;
output		wb_err_o;
output		wb_rty_o;

// Master Interface
input	[31:0]	wbm_data_i;
output	[31:0]	wbm_data_o;
output	[31:0]	wb_addr_o;
output	[3:0]	wb_sel_o;
output		wb_we_o;
output		wb_cyc_o;
output		wb_stb_o;
input		wb_ack_i;
input		wb_err_i;
input		wb_rty_i;

// --------------------------------------
// MASTER INTERFACE 
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
input		pt_sel_i;	// Pass Through Mode Selected
input	[70:0]	mast_pt_in;	// Grouped WISHBONE inputs
output	[34:0]	mast_pt_out;	// Grouped WISHBONE outputs

// --------------------------------------
// Slave INTERFACE 

// This is the register File Interface
output	[31:0]	slv_adr;	// Slave Address
input	[31:0]	slv_din;	// Slave Input Data
output	[31:0]	slv_dout;	// Slave Output Data
output		slv_re;		// Slave Read Enable
output		slv_we;		// Slave Write Enable

// Pass through Interface
output		pt_sel_o;	// Pass Through Mode Active
output	[70:0]	slv_pt_out;	// Grouped WISHBONE out signals
input	[34:0]	slv_pt_in;	// Grouped WISHBONE in signals

////////////////////////////////////////////////////////////////////
//
// Modules
//

wb_dma_wb_mast	u0(
		.clk(		clk		),
		.rst(		rst		),
		.wb_data_i(	wbs_data_i	),
		.wb_data_o(	wbs_data_o	),
		.wb_addr_o(	wb_addr_o	),
		.wb_sel_o(	wb_sel_o	),
		.wb_we_o(	wb_we_o		),
		.wb_cyc_o(	wb_cyc_o	),
		.wb_stb_o(	wb_stb_o	),
		.wb_ack_i(	wb_ack_i	),
		.wb_err_i(	wb_err_i	),
		.wb_rty_i(	wb_rty_i	),
		.mast_go(	mast_go		),
		.mast_we(	mast_we		),
		.mast_adr(	mast_adr	),
		.mast_din(	mast_din	),
		.mast_dout(	mast_dout	),
		.mast_err(	mast_err	),
		.mast_drdy(	mast_drdy	),
		.mast_wait(	mast_wait	),
		.pt_sel(	pt_sel_i	),
		.mast_pt_in(	mast_pt_in	),
		.mast_pt_out(	mast_pt_out	)
		);


wb_dma_wb_slv #(rf_addr)	u1(
		.clk(		clk		),
		.rst(		rst		),
		.wb_data_i(	wbm_data_i	),
		.wb_data_o(	wbm_data_o	),
		.wb_addr_i(	wb_addr_i	),
		.wb_sel_i(	wb_sel_i	),
		.wb_we_i(	wb_we_i		),
		.wb_cyc_i(	wb_cyc_i	),
		.wb_stb_i(	wb_stb_i	),
		.wb_ack_o(	wb_ack_o	),
		.wb_err_o(	wb_err_o	),
		.wb_rty_o(	wb_rty_o	),
		.slv_adr(	slv_adr		),
		.slv_din(	slv_din		),
		.slv_dout(	slv_dout	),
		.slv_re(	slv_re		),
		.slv_we(	slv_we		),
		.pt_sel(	pt_sel_o	),
		.slv_pt_out(	slv_pt_out	),
		.slv_pt_in(	slv_pt_in	)
		);


endmodule
