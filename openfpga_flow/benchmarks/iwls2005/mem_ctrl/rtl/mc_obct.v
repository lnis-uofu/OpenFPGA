/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE Memory Controller                                 ////
////  Open Bank & Row Tracking Block                             ////
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
//  $Id: mc_obct.v,v 1.4 2002/01/21 13:08:52 rudi Exp $
//
//  $Date: 2002/01/21 13:08:52 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: mc_obct.v,v $
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
//               Revision 1.1.1.1  2001/05/13 09:39:45  rudi
//               Created Directory Structure
//
//
//
//

`include "mc_defines.v"

module mc_obct(clk, rst, row_adr, bank_adr, bank_set, bank_clr, bank_clr_all,
		bank_open, any_bank_open, row_same);
input		clk, rst;
input	[12:0]	row_adr;
input	[1:0]	bank_adr;
input		bank_set;
input		bank_clr;
input		bank_clr_all;
output		bank_open;
output		any_bank_open;
output		row_same;

////////////////////////////////////////////////////////////////////
//
// Local Registers & Wires
//

reg		bank0_open, bank1_open, bank2_open, bank3_open;
reg		bank_open;
reg	[12:0]	b0_last_row;
reg	[12:0]	b1_last_row;
reg	[12:0]	b2_last_row;
reg	[12:0]	b3_last_row;
wire		row0_same, row1_same, row2_same, row3_same;
reg		row_same;

////////////////////////////////////////////////////////////////////
//
// Bank Open/Closed Tracking
//

always @(posedge clk or posedge rst)
	if(rst)					bank0_open <= #1 1'b0;
	else
	if((bank_adr == 2'h0) & bank_set)	bank0_open <= #1 1'b1;
	else
	if((bank_adr == 2'h0) & bank_clr)	bank0_open <= #1 1'b0;
	else
	if(bank_clr_all)			bank0_open <= #1 1'b0;

always @(posedge clk or posedge rst)
	if(rst)					bank1_open <= #1 1'b0;
	else
	if((bank_adr == 2'h1) & bank_set)	bank1_open <= #1 1'b1;
	else
	if((bank_adr == 2'h1) & bank_clr)	bank1_open <= #1 1'b0;
	else
	if(bank_clr_all)			bank1_open <= #1 1'b0;

always @(posedge clk or posedge rst)
	if(rst)					bank2_open <= #1 1'b0;
	else
	if((bank_adr == 2'h2) & bank_set)	bank2_open <= #1 1'b1;
	else
	if((bank_adr == 2'h2) & bank_clr)	bank2_open <= #1 1'b0;
	else
	if(bank_clr_all)			bank2_open <= #1 1'b0;

always @(posedge clk or posedge rst)
	if(rst)					bank3_open <= #1 1'b0;
	else
	if((bank_adr == 2'h3) & bank_set)	bank3_open <= #1 1'b1;
	else
	if((bank_adr == 2'h3) & bank_clr)	bank3_open <= #1 1'b0;
	else
	if(bank_clr_all)			bank3_open <= #1 1'b0;

always @(bank_adr or bank0_open or bank1_open or bank2_open or bank3_open)
	case(bank_adr)		// synopsys full_case parallel_case
	   2'h0: bank_open = bank0_open;
	   2'h1: bank_open = bank1_open;
	   2'h2: bank_open = bank2_open;
	   2'h3: bank_open = bank3_open;
	endcase

assign any_bank_open = bank0_open | bank1_open | bank2_open | bank3_open;

////////////////////////////////////////////////////////////////////
//
// Raw Address Tracking
//

always @(posedge clk)
	if((bank_adr == 2'h0) & bank_set)	b0_last_row <= #1 row_adr;

always @(posedge clk)
	if((bank_adr == 2'h1) & bank_set)	b1_last_row <= #1 row_adr;

always @(posedge clk)
	if((bank_adr == 2'h2) & bank_set)	b2_last_row <= #1 row_adr;

always @(posedge clk)
	if((bank_adr == 2'h3) & bank_set)	b3_last_row <= #1 row_adr;

////////////////////////////////////////////////////////////////////
//
// Raw address checking
//

assign row0_same = (b0_last_row == row_adr);
assign row1_same = (b1_last_row == row_adr);
assign row2_same = (b2_last_row == row_adr);
assign row3_same = (b3_last_row == row_adr);

always @(bank_adr or row0_same or row1_same or row2_same or row3_same)
	case(bank_adr)		// synopsys full_case parallel_case
	   2'h0: row_same = row0_same;
	   2'h1: row_same = row1_same;
	   2'h2: row_same = row2_same;
	   2'h3: row_same = row3_same;
	endcase

endmodule


// This is used for unused Chip Selects
module mc_obct_dummy(clk, rst, row_adr, bank_adr, bank_set, bank_clr, bank_clr_all,
		bank_open, any_bank_open, row_same);
input		clk, rst;
input	[12:0]	row_adr;
input	[1:0]	bank_adr;
input		bank_set;
input		bank_clr;
input		bank_clr_all;
output		bank_open;
output		any_bank_open;
output		row_same;

assign bank_open = 1'b0;
assign any_bank_open = 1'b0;
assign row_same = 1'b0;

endmodule
