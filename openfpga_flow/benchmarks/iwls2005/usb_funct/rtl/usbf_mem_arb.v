/////////////////////////////////////////////////////////////////////
////                                                             ////
////  Memory Buffer Arbiter                                      ////
////  Arbitrates between the internal DMA and external bus       ////
////  interface for the internal buffer memory                   ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/usb/       ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2000-2003 Rudolf Usselmann                    ////
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
//  $Id: usbf_mem_arb.v,v 1.3 2003/10/17 02:36:57 rudi Exp $
//
//  $Date: 2003/10/17 02:36:57 $
//  $Revision: 1.3 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: usbf_mem_arb.v,v $
//               Revision 1.3  2003/10/17 02:36:57  rudi
//               - Disabling bit stuffing and NRZI encoding during speed negotiation
//               - Now the core can send zero size packets
//               - Fixed register addresses for some of the higher endpoints
//                 (conversion between decimal/hex was wrong)
//               - The core now does properly evaluate the function address to
//                 determine if the packet was intended for it.
//               - Various other minor bugs and typos
//
//               Revision 1.2  2001/11/04 12:22:45  rudi
//
//               - Fixed previous fix (brocke something else ...)
//               - Majore Synthesis cleanup
//
//               Revision 1.1  2001/08/03 05:30:09  rudi
//
//
//               1) Reorganized directory structure
//
//               Revision 1.2  2001/03/31 13:00:51  rudi
//
//               - Added Core configuration
//               - Added handling of OUT packets less than MAX_PL_SZ in DMA mode
//               - Modified WISHBONE interface and sync logic
//               - Moved SSRAM outside the core (added interface)
//               - Many small bug fixes ...
//
//               Revision 1.0  2001/03/07 09:17:12  rudi
//
//
//               Changed all revisions to revision 1.0. This is because OpenCores CVS
//               interface could not handle the original '0.1' revision ....
//
//               Revision 0.1.0.1  2001/02/28 08:10:52  rudi
//               Initial Release
//
//

`include "usbf_defines.v"

module usbf_mem_arb(	phy_clk, wclk, rst,

		// SSRAM Interface
		sram_adr, sram_din, sram_dout, sram_re, sram_we,

		// IDMA Memory Interface
		madr, mdout, mdin, mwe, mreq, mack,

		// WISHBONE Memory Interface
		wadr, wdout, wdin, wwe, wreq, wack

		);

parameter	SSRAM_HADR = 14;

input		phy_clk, wclk, rst;

output	[SSRAM_HADR:0]	sram_adr;
input	[31:0]	sram_din;
output	[31:0]	sram_dout;
output		sram_re, sram_we;

input	[SSRAM_HADR:0]	madr;
output	[31:0]	mdout;
input	[31:0]	mdin;
input		mwe;
input		mreq;
output		mack;

input	[SSRAM_HADR:0]	wadr;
output	[31:0]	wdout;
input	[31:0]	wdin;
input		wwe;
input		wreq;
output		wack;

///////////////////////////////////////////////////////////////////
//
// Local Wires and Registers
//

wire		wsel;
reg	[SSRAM_HADR:0]	sram_adr;
reg	[31:0]	sram_dout;
reg		sram_we;
wire		mack;
wire		mcyc;
reg		wack_r;

///////////////////////////////////////////////////////////////////
//
// Memory Arbiter Logic
//

// IDMA has always first priority

// -----------------------------------------
// Ctrl Signals

assign wsel = (wreq | wack) & !mreq;

// -----------------------------------------
// SSRAM Specific
// Data Path
always @(wsel or wdin or mdin)
	if(wsel)	sram_dout = wdin;
	else		sram_dout = mdin;

// Address Path
always @(wsel or wadr or madr)
	if(wsel)	sram_adr = wadr;
	else		sram_adr = madr;

// Write Enable Path
always @(wsel or wwe or wreq or mwe or mcyc)
	if(wsel)	sram_we = wreq & wwe;
	else		sram_we = mwe & mcyc;

assign sram_re = 1'b1;

// -----------------------------------------
// IDMA specific

assign mdout = sram_din;

assign mack = mreq;

assign mcyc = mack;	// Qualifier for writes

// -----------------------------------------
// WISHBONE specific
assign wdout = sram_din;

assign wack = wack_r & !mreq;

`ifdef USBF_ASYNC_RESET
always @(posedge phy_clk or negedge rst)
`else
always @(posedge phy_clk)
`endif
	if(!rst)	wack_r <= 1'b0;
	else		wack_r <= wreq & !mreq & !wack;

endmodule

