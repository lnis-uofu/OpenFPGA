/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE Memory Controller Address Select Block            ////
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
//  $Id: mc_adr_sel.v,v 1.4 2002/01/21 13:08:52 rudi Exp $
//
//  $Date: 2002/01/21 13:08:52 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: mc_adr_sel.v,v $
//               Revision 1.4  2002/01/21 13:08:52  rudi
//
//               Fixed several minor bugs, cleaned up the code further ...
//
//               Revision 1.3  2001/11/29 02:16:28  rudi
//
//
//               - More Synthesis cleanup, mostly for speed
//               - Several bug fixes
//               - Changed code to avoid auto-precharge and
//                 burst-terminate combinations (apparently illegal ?)
//                 Now we will do a manual precharge ...
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
//               Revision 1.2  2001/06/12 15:19:49  rudi
//
//
//               Minor changes after running lint, and a small bug fix reading csr and ba_mask registers.
//
//               Revision 1.1.1.1  2001/05/13 09:39:40  rudi
//               Created Directory Structure
//
//
//
//

`include "mc_defines.v"

module mc_adr_sel(clk, csc, tms, wb_ack_o, wb_stb_i, wb_addr_i, wb_we_i,
		wb_write_go, wr_hold, cas_,
		mc_addr, row_adr, bank_adr, rfr_ack,
		cs_le, cmd_a10, row_sel, lmr_sel, next_adr, wr_cycle,
		page_size);

input		clk;
input	[31:0]	csc;
input	[31:0]	tms;
input		wb_ack_o, wb_stb_i;
input	[31:0]	wb_addr_i;
input		wb_we_i;
input		wb_write_go;
input		wr_hold;
input		cas_;
output	[23:0]	mc_addr;
output	[12:0]	row_adr;
output	[1:0]	bank_adr;
input		rfr_ack;
input		cs_le;
input		cmd_a10;
input		row_sel;
input		lmr_sel;
input		next_adr;
input		wr_cycle;
output	[10:0]	page_size;

////////////////////////////////////////////////////////////////////
//
// Local Registers & Wires
//

reg	[23:0]	mc_addr_d;
reg	[23:0]	acs_addr;
wire	[23:0]	acs_addr_pl1;
reg	[23:0]	sram_addr;
wire	[14:0]	sdram_adr;
reg	[12:0]	row_adr;
reg	[9:0]	col_adr;
reg	[1:0]	bank_adr;
reg	[10:0]	page_size;

wire	[2:0]	mem_type;
wire	[1:0]	bus_width;
wire	[1:0]	mem_size;
wire		bas;

// Aliases
assign mem_type  = csc[3:1];
assign bus_width = csc[5:4];
assign mem_size  = csc[7:6];
assign bas       = csc[9];

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

always @(mem_type or wr_hold or sdram_adr or acs_addr or sram_addr or wb_addr_i)
	if(mem_type == `MC_MEM_TYPE_SDRAM)		mc_addr_d = {9'h0, sdram_adr};
	else
	if(mem_type == `MC_MEM_TYPE_ACS)		mc_addr_d = acs_addr;
	else
	if((mem_type == `MC_MEM_TYPE_SRAM) & wr_hold)	mc_addr_d = sram_addr;
	else						mc_addr_d = wb_addr_i[25:2];

assign mc_addr = rfr_ack ? {mc_addr_d[23:11], 1'b1, mc_addr_d[9:0]} : mc_addr_d;

////////////////////////////////////////////////////////////////////
//
// Async Devices Address Latch & Counter
//

mc_incn_r #(24) u0(	.clk(		clk		),
			.inc_in(	acs_addr	),
			.inc_out(	acs_addr_pl1	) );

always @(posedge clk)
	if(wb_stb_i)	sram_addr <= #1 wb_addr_i[25:2];

always @(posedge clk)
	if(cs_le | wb_we_i)
		case(bus_width)		// synopsys full_case parallel_case
		   `MC_BW_8:	acs_addr <= #1 wb_addr_i[23:0];
		   `MC_BW_16:	acs_addr <= #1 wb_addr_i[24:1];
		   `MC_BW_32:	acs_addr <= #1 wb_addr_i[25:2];
		endcase
	else
	if(next_adr)		acs_addr <= #1 acs_addr_pl1;

////////////////////////////////////////////////////////////////////
//
// SDRAM Address Mux
//

assign sdram_adr[12:0] =	(lmr_sel & !cas_) ? tms[12:0] :
				row_sel ? row_adr :
				{2'h0, cmd_a10, col_adr};

assign sdram_adr[14:13] = bank_adr;

always @(posedge clk)
   if(wr_cycle ? wb_ack_o : wb_stb_i)	
	casex({bus_width, mem_size})		// synopsys full_case parallel_case
	   {`MC_BW_8, `MC_MEM_SIZE_64}:		col_adr <= #1 {1'h0, wb_addr_i[10:2]};
	   {`MC_BW_8, `MC_MEM_SIZE_128}:	col_adr <= #1        wb_addr_i[11:2];
	   {`MC_BW_8, `MC_MEM_SIZE_256}:	col_adr <= #1        wb_addr_i[11:2];

	   {`MC_BW_16, `MC_MEM_SIZE_64}:	col_adr <= #1 {2'h0, wb_addr_i[09:2]};
	   {`MC_BW_16, `MC_MEM_SIZE_128}:	col_adr <= #1 {1'h0, wb_addr_i[10:2]};
	   {`MC_BW_16, `MC_MEM_SIZE_256}:	col_adr <= #1 {1'h0, wb_addr_i[10:2]};

	   {`MC_BW_32, `MC_MEM_SIZE_64}:	col_adr <= #1 {2'h0, wb_addr_i[09:2]};
	   {`MC_BW_32, `MC_MEM_SIZE_128}:	col_adr <= #1 {2'h0, wb_addr_i[09:2]};
	   {`MC_BW_32, `MC_MEM_SIZE_256}:	col_adr <= #1 {2'h0, wb_addr_i[09:2]};
	endcase

always @(posedge clk)
   if(cs_le)
     begin
	if(!bas)
		casex({bus_width, mem_size})		// synopsys full_case parallel_case
		   {`MC_BW_8, `MC_MEM_SIZE_64}:		row_adr <= #1 {1'h0, wb_addr_i[24:13]};
		   {`MC_BW_8, `MC_MEM_SIZE_128}:	row_adr <= #1 {1'h0, wb_addr_i[25:14]};
		   {`MC_BW_8, `MC_MEM_SIZE_256}:	row_adr <= #1        wb_addr_i[26:14];
	
		   {`MC_BW_16, `MC_MEM_SIZE_64}:	row_adr <= #1 {1'h0, wb_addr_i[23:12]};
		   {`MC_BW_16, `MC_MEM_SIZE_128}:	row_adr <= #1 {1'h0, wb_addr_i[24:13]};
		   {`MC_BW_16, `MC_MEM_SIZE_256}:	row_adr <= #1        wb_addr_i[25:13];
	
		   {`MC_BW_32, `MC_MEM_SIZE_64}:	row_adr <= #1 {2'h0, wb_addr_i[22:12]};
		   {`MC_BW_32, `MC_MEM_SIZE_128}:	row_adr <= #1 {1'h0, wb_addr_i[23:12]};
		   {`MC_BW_32, `MC_MEM_SIZE_256}:	row_adr <= #1        wb_addr_i[24:12];
		endcase
	else
		casex({bus_width, mem_size})		// synopsys full_case parallel_case
		   {`MC_BW_8, `MC_MEM_SIZE_64}:		row_adr <= #1 {1'h0, wb_addr_i[22:11]};
		   {`MC_BW_8, `MC_MEM_SIZE_128}:	row_adr <= #1 {1'h0, wb_addr_i[23:12]};
		   {`MC_BW_8, `MC_MEM_SIZE_256}:	row_adr <= #1        wb_addr_i[24:12];
	
		   {`MC_BW_16, `MC_MEM_SIZE_64}:	row_adr <= #1 {1'h0, wb_addr_i[21:10]};
		   {`MC_BW_16, `MC_MEM_SIZE_128}:	row_adr <= #1 {1'h0, wb_addr_i[22:11]};
		   {`MC_BW_16, `MC_MEM_SIZE_256}:	row_adr <= #1        wb_addr_i[23:11];
	
		   {`MC_BW_32, `MC_MEM_SIZE_64}:	row_adr <= #1 {2'h0, wb_addr_i[20:10]};
		   {`MC_BW_32, `MC_MEM_SIZE_128}:	row_adr <= #1 {1'h0, wb_addr_i[21:10]};
		   {`MC_BW_32, `MC_MEM_SIZE_256}:	row_adr <= #1        wb_addr_i[22:10];
		endcase
     end


always @(posedge clk)
   if(cs_le)
     begin
	if(!bas)
		casex({bus_width, mem_size})		// synopsys full_case parallel_case
		   {`MC_BW_8, `MC_MEM_SIZE_64}:		bank_adr <= #1 wb_addr_i[12:11];
		   {`MC_BW_8, `MC_MEM_SIZE_128}:	bank_adr <= #1 wb_addr_i[13:12];
		   {`MC_BW_8, `MC_MEM_SIZE_256}:	bank_adr <= #1 wb_addr_i[13:12];
	
		   {`MC_BW_16, `MC_MEM_SIZE_64}:	bank_adr <= #1 wb_addr_i[11:10];
		   {`MC_BW_16, `MC_MEM_SIZE_128}:	bank_adr <= #1 wb_addr_i[12:11];
		   {`MC_BW_16, `MC_MEM_SIZE_256}:	bank_adr <= #1 wb_addr_i[12:11];
	
		   {`MC_BW_32, `MC_MEM_SIZE_64}:	bank_adr <= #1 wb_addr_i[11:10];
		   {`MC_BW_32, `MC_MEM_SIZE_128}:	bank_adr <= #1 wb_addr_i[11:10];
		   {`MC_BW_32, `MC_MEM_SIZE_256}:	bank_adr <= #1 wb_addr_i[11:10];
		endcase
	else
		casex({bus_width, mem_size})		// synopsys full_case parallel_case
		   {`MC_BW_8, `MC_MEM_SIZE_64}:		bank_adr <= #1 wb_addr_i[24:23];
		   {`MC_BW_8, `MC_MEM_SIZE_128}:	bank_adr <= #1 wb_addr_i[25:24];
		   {`MC_BW_8, `MC_MEM_SIZE_256}:	bank_adr <= #1 wb_addr_i[26:25];
	
		   {`MC_BW_16, `MC_MEM_SIZE_64}:	bank_adr <= #1 wb_addr_i[23:22];
		   {`MC_BW_16, `MC_MEM_SIZE_128}:	bank_adr <= #1 wb_addr_i[24:23];
		   {`MC_BW_16, `MC_MEM_SIZE_256}:	bank_adr <= #1 wb_addr_i[25:24];
	
		   {`MC_BW_32, `MC_MEM_SIZE_64}:	bank_adr <= #1 wb_addr_i[22:21];
		   {`MC_BW_32, `MC_MEM_SIZE_128}:	bank_adr <= #1 wb_addr_i[23:22];
		   {`MC_BW_32, `MC_MEM_SIZE_256}:	bank_adr <= #1 wb_addr_i[24:23];
		endcase
     end

always @(bus_width or mem_size)
	casex({bus_width, mem_size})		// synopsys full_case parallel_case
	   {`MC_BW_8, `MC_MEM_SIZE_64}:		page_size = 11'd512;
	   {`MC_BW_8, `MC_MEM_SIZE_128}:	page_size = 11'd1024;
	   {`MC_BW_8, `MC_MEM_SIZE_256}:	page_size = 11'd1024;

	   {`MC_BW_16, `MC_MEM_SIZE_64}:	page_size = 11'd256;
	   {`MC_BW_16, `MC_MEM_SIZE_128}:	page_size = 11'd512;
	   {`MC_BW_16, `MC_MEM_SIZE_256}:	page_size = 11'd512;

	   {`MC_BW_32, `MC_MEM_SIZE_64}:	page_size = 11'd256;
	   {`MC_BW_32, `MC_MEM_SIZE_128}:	page_size = 11'd256;
	   {`MC_BW_32, `MC_MEM_SIZE_256}:	page_size = 11'd256;
	endcase

endmodule

