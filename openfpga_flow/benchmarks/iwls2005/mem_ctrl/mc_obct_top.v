/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE Memory Controller                                 ////
////  Open Bank & Row Tracking Block Top Level                   ////
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
//  $Id: mc_obct_top.v,v 1.4 2002/01/21 13:08:52 rudi Exp $
//
//  $Date: 2002/01/21 13:08:52 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: mc_obct_top.v,v $
//               Revision 1.4  2002/01/21 13:08:52  rudi
//
//               Fixed several minor bugs, cleaned up the code further ...
//
//               Revision 1.3  2001/12/21 05:09:29  rudi
//
//               - Fixed combinatorial loops in synthesis
//               - Fixed byte select bug
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
//               Revision 1.1.1.1  2001/05/13 09:39:47  rudi
//               Created Directory Structure
//
//
//
//

`include "mc_defines.v"

module mc_obct_top(clk, rst, cs, row_adr, bank_adr, bank_set, bank_clr, bank_clr_all,
		bank_open, any_bank_open, row_same, rfr_ack);
input		clk, rst;
input	[7:0]	cs;
input	[12:0]	row_adr;
input	[1:0]	bank_adr;
input		bank_set;
input		bank_clr;
input		bank_clr_all;
output		bank_open;
output		any_bank_open;
output		row_same;
input		rfr_ack;

////////////////////////////////////////////////////////////////////
//
// Local Registers & Wires
//

reg		bank_open;
reg		row_same;
reg		any_bank_open;

wire		bank_set_0;
wire		bank_clr_0;
wire		bank_clr_all_0;
wire		bank_open_0;
wire		row_same_0;
wire		any_bank_open_0;

wire		bank_set_1;
wire		bank_clr_1;
wire		bank_clr_all_1;
wire		bank_open_1;
wire		row_same_1;
wire		any_bank_open_1;

wire		bank_set_2;
wire		bank_clr_2;
wire		bank_clr_all_2;
wire		bank_open_2;
wire		row_same_2;
wire		any_bank_open_2;

wire		bank_set_3;
wire		bank_clr_3;
wire		bank_clr_all_3;
wire		bank_open_3;
wire		row_same_3;
wire		any_bank_open_3;

wire		bank_set_4;
wire		bank_clr_4;
wire		bank_clr_all_4;
wire		bank_open_4;
wire		row_same_4;
wire		any_bank_open_4;

wire		bank_set_5;
wire		bank_clr_5;
wire		bank_clr_all_5;
wire		bank_open_5;
wire		row_same_5;
wire		any_bank_open_5;

wire		bank_set_6;
wire		bank_clr_6;
wire		bank_clr_all_6;
wire		bank_open_6;
wire		row_same_6;
wire		any_bank_open_6;

wire		bank_set_7;
wire		bank_clr_7;
wire		bank_clr_all_7;
wire		bank_open_7;
wire		row_same_7;
wire		any_bank_open_7;

////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

assign bank_set_0 = cs[0] & bank_set;
assign bank_set_1 = cs[1] & bank_set;
assign bank_set_2 = cs[2] & bank_set;
assign bank_set_3 = cs[3] & bank_set;
assign bank_set_4 = cs[4] & bank_set;
assign bank_set_5 = cs[5] & bank_set;
assign bank_set_6 = cs[6] & bank_set;
assign bank_set_7 = cs[7] & bank_set;

assign bank_clr_0 = cs[0] & bank_clr;
assign bank_clr_1 = cs[1] & bank_clr;
assign bank_clr_2 = cs[2] & bank_clr;
assign bank_clr_3 = cs[3] & bank_clr;
assign bank_clr_4 = cs[4] & bank_clr;
assign bank_clr_5 = cs[5] & bank_clr;
assign bank_clr_6 = cs[6] & bank_clr;
assign bank_clr_7 = cs[7] & bank_clr;

assign bank_clr_all_0 = (cs[0] & bank_clr_all) | rfr_ack;
assign bank_clr_all_1 = (cs[1] & bank_clr_all) | rfr_ack;
assign bank_clr_all_2 = (cs[2] & bank_clr_all) | rfr_ack;
assign bank_clr_all_3 = (cs[3] & bank_clr_all) | rfr_ack;
assign bank_clr_all_4 = (cs[4] & bank_clr_all) | rfr_ack;
assign bank_clr_all_5 = (cs[5] & bank_clr_all) | rfr_ack;
assign bank_clr_all_6 = (cs[6] & bank_clr_all) | rfr_ack;
assign bank_clr_all_7 = (cs[7] & bank_clr_all) | rfr_ack;

always @(posedge clk)
	bank_open <= #1	(cs[0] & bank_open_0) | (cs[1] & bank_open_1) |
			(cs[2] & bank_open_2) | (cs[3] & bank_open_3) |
			(cs[4] & bank_open_4) | (cs[5] & bank_open_5) |
			(cs[6] & bank_open_6) | (cs[7] & bank_open_7);

always @(posedge clk)
	row_same <= #1	(cs[0] & row_same_0) | (cs[1] & row_same_1) |
			(cs[2] & row_same_2) | (cs[3] & row_same_3) |
			(cs[4] & row_same_4) | (cs[5] & row_same_5) |
			(cs[6] & row_same_6) | (cs[7] & row_same_7);

always @(posedge clk)
	any_bank_open <= #1	(cs[0] & any_bank_open_0) | (cs[1] & any_bank_open_1) |
				(cs[2] & any_bank_open_2) | (cs[3] & any_bank_open_3) |
				(cs[4] & any_bank_open_4) | (cs[5] & any_bank_open_5) |
				(cs[6] & any_bank_open_6) | (cs[7] & any_bank_open_7);


////////////////////////////////////////////////////////////////////
//
// OBCT Modules for each Chip Select
//

mc_obct	u0(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_0	),
		.bank_clr(	bank_clr_0	),
		.bank_clr_all(	bank_clr_all_0	),
		.bank_open(	bank_open_0	),
		.any_bank_open(	any_bank_open_0	),
		.row_same(	row_same_0	)
		);

`ifdef MC_HAVE_CS1
mc_obct	u1(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_1	),
		.bank_clr(	bank_clr_1	),
		.bank_clr_all(	bank_clr_all_1	),
		.bank_open(	bank_open_1	),
		.any_bank_open(	any_bank_open_1	),
		.row_same(	row_same_1	)
		);
`else
mc_obct_dummy	u1(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_1	),
		.bank_clr(	bank_clr_1	),
		.bank_clr_all(	bank_clr_all_1	),
		.bank_open(	bank_open_1	),
		.any_bank_open(	any_bank_open_1	),
		.row_same(	row_same_1	)
		);
`endif

`ifdef MC_HAVE_CS2
mc_obct	u2(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_2	),
		.bank_clr(	bank_clr_2	),
		.bank_clr_all(	bank_clr_all_2	),
		.bank_open(	bank_open_2	),
		.any_bank_open(	any_bank_open_2	),
		.row_same(	row_same_2	)
		);
`else
mc_obct_dummy	u2(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_2	),
		.bank_clr(	bank_clr_2	),
		.bank_clr_all(	bank_clr_all_2	),
		.bank_open(	bank_open_2	),
		.any_bank_open(	any_bank_open_2	),
		.row_same(	row_same_2	)
		);
`endif

`ifdef MC_HAVE_CS3
mc_obct	u3(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_3	),
		.bank_clr(	bank_clr_3	),
		.bank_clr_all(	bank_clr_all_3	),
		.bank_open(	bank_open_3	),
		.any_bank_open(	any_bank_open_3	),
		.row_same(	row_same_3	)
		);
`else
mc_obct_dummy	u3(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_3	),
		.bank_clr(	bank_clr_3	),
		.bank_clr_all(	bank_clr_all_3	),
		.bank_open(	bank_open_3	),
		.any_bank_open(	any_bank_open_3	),
		.row_same(	row_same_3	)
		);
`endif

`ifdef MC_HAVE_CS4
mc_obct	u4(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_4	),
		.bank_clr(	bank_clr_4	),
		.bank_clr_all(	bank_clr_all_4	),
		.bank_open(	bank_open_4	),
		.any_bank_open(	any_bank_open_4	),
		.row_same(	row_same_4	)
		);
`else
mc_obct_dummy	u4(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_4	),
		.bank_clr(	bank_clr_4	),
		.bank_clr_all(	bank_clr_all_4	),
		.bank_open(	bank_open_4	),
		.any_bank_open(	any_bank_open_4	),
		.row_same(	row_same_4	)
		);
`endif

`ifdef MC_HAVE_CS5
mc_obct	u5(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_5	),
		.bank_clr(	bank_clr_5	),
		.bank_clr_all(	bank_clr_all_5	),
		.bank_open(	bank_open_5	),
		.any_bank_open(	any_bank_open_5	),
		.row_same(	row_same_5	)
		);
`else
mc_obct_dummy	u5(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_5	),
		.bank_clr(	bank_clr_5	),
		.bank_clr_all(	bank_clr_all_5	),
		.bank_open(	bank_open_5	),
		.any_bank_open(	any_bank_open_5	),
		.row_same(	row_same_5	)
		);
`endif

`ifdef MC_HAVE_CS6
mc_obct	u6(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_6	),
		.bank_clr(	bank_clr_6	),
		.bank_clr_all(	bank_clr_all_6	),
		.bank_open(	bank_open_6	),
		.any_bank_open(	any_bank_open_6	),
		.row_same(	row_same_6	)
		);
`else
mc_obct_dummy	u6(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_6	),
		.bank_clr(	bank_clr_6	),
		.bank_clr_all(	bank_clr_all_6	),
		.bank_open(	bank_open_6	),
		.any_bank_open(	any_bank_open_6	),
		.row_same(	row_same_6	)
		);
`endif

`ifdef MC_HAVE_CS7
mc_obct	u7(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_7	),
		.bank_clr(	bank_clr_7	),
		.bank_clr_all(	bank_clr_all_7	),
		.bank_open(	bank_open_7	),
		.any_bank_open(	any_bank_open_7	),
		.row_same(	row_same_7	)
		);
`else
mc_obct_dummy	u7(
		.clk(		clk		),
		.rst(		rst		),
		.row_adr(	row_adr		),
		.bank_adr(	bank_adr	),
		.bank_set(	bank_set_7	),
		.bank_clr(	bank_clr_7	),
		.bank_clr_all(	bank_clr_all_7	),
		.bank_open(	bank_open_7	),
		.any_bank_open(	any_bank_open_7	),
		.row_same(	row_same_7	)
		);
`endif

endmodule
