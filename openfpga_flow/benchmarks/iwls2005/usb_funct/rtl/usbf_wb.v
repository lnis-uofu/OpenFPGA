/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE Interface                                         ////
////  This is the external bus interface, that is WISHBONE       ////
////  SoC compliant.                                             ////
////                                                             ////
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
//  $Id: usbf_wb.v,v 1.4 2003/10/17 02:36:57 rudi Exp $
//
//  $Date: 2003/10/17 02:36:57 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: usbf_wb.v,v $
//               Revision 1.4  2003/10/17 02:36:57  rudi
//               - Disabling bit stuffing and NRZI encoding during speed negotiation
//               - Now the core can send zero size packets
//               - Fixed register addresses for some of the higher endpoints
//                 (conversion between decimal/hex was wrong)
//               - The core now does properly evaluate the function address to
//                 determine if the packet was intended for it.
//               - Various other minor bugs and typos
//
//               Revision 1.3  2001/09/24 01:15:28  rudi
//
//               Changed reset to be active high async.
//
//               Revision 1.2  2001/08/10 08:48:33  rudi
//
//               - Changed IO names to be more clear.
//               - Uniquifyed define names to be core specific.
//
//               Revision 1.1  2001/08/03 05:30:09  rudi
//
//
//               1) Reorganized directory structure
//
//               Revision 1.2  2001/03/31 13:00:52  rudi
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
//               Revision 0.1.0.1  2001/02/28 08:11:47  rudi
//               Initial Release
//
//

`include "usbf_defines.v"

module usbf_wb(	// WISHBONE Interface
		wb_clk, phy_clk, rst, wb_addr_i, wb_data_i, wb_data_o, 
		wb_ack_o, wb_we_i, wb_stb_i, wb_cyc_i,

		// Memory Arbiter Interface
		ma_adr, ma_dout, ma_din, ma_we, ma_req, ma_ack,

		// Register File interface
		rf_re, rf_we, rf_din, rf_dout);

input		wb_clk, phy_clk;
input		rst;
input	[`USBF_UFC_HADR:0]	wb_addr_i;
input	[31:0]	wb_data_i;
output	[31:0]	wb_data_o;
output		wb_ack_o;
input		wb_we_i;
input		wb_stb_i;
input		wb_cyc_i;

// Memory Arbiter Interface
output	[`USBF_UFC_HADR:0]	ma_adr;
output	[31:0]	ma_dout;
input	[31:0]	ma_din;
output		ma_we;
output		ma_req;
input		ma_ack;

// Register File interface
output		rf_re;
output		rf_we;
input	[31:0]	rf_din;
output	[31:0]	rf_dout;

///////////////////////////////////////////////////////////////////
//
// Local Wires and Registers
//

parameter	[5:0]	// synopsys enum state
		IDLE	= 6'b00_0001,
		MA_WR	= 6'b00_0010,
		MA_RD	= 6'b00_0100,
		W0	= 6'b00_1000,
		W1	= 6'b01_0000,
		W2	= 6'b10_0000;

reg	[5:0]	/* synopsys enum state */ state, next_state;
// synopsys state_vector state

reg		wb_req_s1;
reg		wb_ack_d, wb_ack_s1, wb_ack_s1a, wb_ack_s2;
reg		ma_we;
reg		rf_re, rf_we_d;
reg		ma_req;
reg		wb_ack_o;
reg	[31:0]	wb_data_o;

///////////////////////////////////////////////////////////////////
//
// Interface Logic
//

assign ma_adr = wb_addr_i;
assign ma_dout = wb_data_i;
assign rf_dout = wb_data_i;

always @(posedge wb_clk)
	if( `USBF_RF_SEL )	wb_data_o <= rf_din;
	else			wb_data_o <= ma_din;

// Sync WISHBONE Request
always @(posedge phy_clk)
	wb_req_s1 <= wb_stb_i & wb_cyc_i;

// Sync WISHBONE Ack
always @(posedge wb_clk)
	wb_ack_s1 <= wb_ack_d;

always @(posedge wb_clk)
	wb_ack_o <= wb_ack_s1 & !wb_ack_s2 & !wb_ack_o;

always @(posedge wb_clk)
	wb_ack_s1a <= wb_ack_s1;

always @(posedge wb_clk)
	wb_ack_s2 <= wb_ack_s1a;

assign	rf_we = rf_we_d;

///////////////////////////////////////////////////////////////////
//
// Interface State Machine
//

`ifdef USBF_ASYNC_RESET
always @(posedge phy_clk or negedge rst)
`else
always @(posedge phy_clk)
`endif
	if(!rst)	state <= IDLE;
	else		state <= next_state;

always @(state or wb_req_s1 or wb_addr_i or ma_ack or wb_we_i)
   begin
	next_state = state;
	ma_req = 1'b0;
	ma_we = 1'b0;
	wb_ack_d = 1'b0;
	rf_re = 1'b0;
	rf_we_d = 1'b0;

	case(state)		// synopsys full_case parallel_case
	   IDLE:
	     begin
		if(wb_req_s1 && `USBF_MEM_SEL && wb_we_i)	
		   begin
			ma_req = 1'b1;
			ma_we = 1'b1;
			next_state = MA_WR;
		   end
		if(wb_req_s1 && `USBF_MEM_SEL && !wb_we_i)
		   begin
			ma_req = 1'b1;
			next_state = MA_RD;
		   end
		if(wb_req_s1 && `USBF_RF_SEL && wb_we_i)
		   begin
			rf_we_d = 1'b1;
			next_state = W0;
		   end
		if(wb_req_s1 && `USBF_RF_SEL && !wb_we_i)
		   begin
			rf_re = 1'b1;
			next_state = W0;
		   end
	     end

	   MA_WR:
	     begin
		if(!ma_ack)
		   begin
			ma_req = 1'b1;
			ma_we = 1'b1;
		   end
		else
		   begin
			wb_ack_d = 1'b1;
			next_state = W1;
		   end
	     end

	   MA_RD:
	     begin
		if(!ma_ack)
		   begin
			ma_req = 1'b1;
		   end
		else
		   begin
			wb_ack_d = 1'b1;
			next_state = W1;
		   end
	     end

	   W0:
	     begin
			wb_ack_d = 1'b1;
			next_state = W1;
	     end

	   W1:
	     begin
			next_state = W2;
	     end

	   W2:
	     begin
			next_state = IDLE;
	     end

	endcase
   end

endmodule

