/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE DMA Channel Arbiter                               ////
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
//  $Id: wb_dma_ch_arb.v,v 1.2 2002/02/01 01:54:44 rudi Exp $
//
//  $Date: 2002/02/01 01:54:44 $
//  $Revision: 1.2 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: wb_dma_ch_arb.v,v $
//               Revision 1.2  2002/02/01 01:54:44  rudi
//
//               - Minor cleanup
//
//               Revision 1.1  2001/07/29 08:57:02  rudi
//
//
//               1) Changed Directory Structure
//               2) Added restart signal (REST)
//
//               Revision 1.4  2001/06/14 08:51:25  rudi
//
//
//               Changed Module name to match file name.
//
//               Revision 1.3  2001/06/13 02:26:46  rudi
//
//
//               Small changes after running lint.
//
//               Revision 1.2  2001/06/05 10:22:34  rudi
//
//
//               - Added Support of up to 31 channels
//               - Added support for 2,4 and 8 priority levels
//               - Now can have up to 31 channels
//               - Added many configuration items
//               - Changed reset to async
//
//               Revision 1.1.1.1  2001/03/19 13:10:47  rudi
//               Initial Release
//
//
//                        

`include "wb_dma_defines.v"

// Arbiter
//
// Implements a simple round robin arbiter for DMA channels of
// same priority

module wb_dma_ch_arb(clk, rst, req, gnt, advance);

input		clk;
input		rst;
input	[30:0]	req;		// Req input
output	[4:0]	gnt; 		// Grant output
input		advance;	// Next Target

///////////////////////////////////////////////////////////////////////
//
// Definitions
//

parameter	[4:0]
		grant0 = 5'h0,
		grant1 = 5'h1,
		grant2 = 5'h2,
		grant3 = 5'h3,
		grant4 = 5'h4,
		grant5 = 5'h5,
		grant6 = 5'h6,
		grant7 = 5'h7,
		grant8 = 5'h8,
		grant9 = 5'h9,
		grant10 = 5'ha,
		grant11 = 5'hb,
		grant12 = 5'hc,
		grant13 = 5'hd,
		grant14 = 5'he,
		grant15 = 5'hf,
		grant16 = 5'h10,
		grant17 = 5'h11,
		grant18 = 5'h12,
		grant19 = 5'h13,
		grant20 = 5'h14,
		grant21 = 5'h15,
		grant22 = 5'h16,
		grant23 = 5'h17,
		grant24 = 5'h18,
		grant25 = 5'h19,
		grant26 = 5'h1a,
		grant27 = 5'h1b,
		grant28 = 5'h1c,
		grant29 = 5'h1d,
		grant30 = 5'h1e;

///////////////////////////////////////////////////////////////////////
//
// Local Registers and Wires
//

reg [4:0]	state, next_state;

///////////////////////////////////////////////////////////////////////
//
//  Misc Logic 
//

assign	gnt = state;

always@(posedge clk or negedge rst)
	if(!rst)	state <= #1 grant0;
	else		state <= #1 next_state;

///////////////////////////////////////////////////////////////////////
//
// Next State Logic
//   - implements round robin arbitration algorithm
//   - switches grant if current req is dropped or next is asserted
//   - parks at last grant
//

always@(state or req or advance)
   begin
	next_state = state;	// Default Keep State
	case(state)		// synopsys parallel_case full_case
 	   grant0:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[0] | advance)
		   begin
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
		   end
 	   grant1:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[1] | advance)
		   begin
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
		   end
 	   grant2:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[2] | advance)
		   begin
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
		   end
 	   grant3:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[3] | advance)
		   begin
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
		   end
 	   grant4:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[4] | advance)
		   begin
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
		   end
 	   grant5:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[5] | advance)
		   begin
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
		   end
 	   grant6:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[6] | advance)
		   begin
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
		   end
 	   grant7:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[7] | advance)
		   begin
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
		   end
 	   grant8:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[8] | advance)
		   begin
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
		   end
 	   grant9:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[9] | advance)
		   begin
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
		   end
 	   grant10:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[10] | advance)
		   begin
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
		   end
 	   grant11:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[11] | advance)
		   begin
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
		   end
 	   grant12:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[12] | advance)
		   begin
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
		   end
 	   grant13:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[13] | advance)
		   begin
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
		   end
 	   grant14:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[14] | advance)
		   begin
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
		   end
 	   grant15:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[15] | advance)
		   begin
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
		   end
 	   grant16:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[16] | advance)
		   begin
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
		   end
 	   grant17:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[17] | advance)
		   begin
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
		   end
 	   grant18:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[18] | advance)
		   begin
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
		   end
 	   grant19:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[19] | advance)
		   begin
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
		   end
 	   grant20:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[20] | advance)
		   begin
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
		   end
 	   grant21:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[21] | advance)
		   begin
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
		   end
 	   grant22:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[22] | advance)
		   begin
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
		   end
 	   grant23:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[23] | advance)
		   begin
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
		   end
 	   grant24:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[24] | advance)
		   begin
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
		   end
 	   grant25:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[25] | advance)
		   begin
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
		   end
 	   grant26:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[26] | advance)
		   begin
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
		   end
 	   grant27:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[27] | advance)
		   begin
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
		   end
 	   grant28:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[28] | advance)
		   begin
			if(req[29])	next_state = grant29;
			else
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
		   end
 	   grant29:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[29] | advance)
		   begin
			if(req[30])	next_state = grant30;
			else
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
		   end
 	   grant30:
		// if this req is dropped or next is asserted, check for other req's
		if(!req[30] | advance)
		   begin
			if(req[0])	next_state = grant0;
			else
			if(req[1])	next_state = grant1;
			else
			if(req[2])	next_state = grant2;
			else
			if(req[3])	next_state = grant3;
			else
			if(req[4])	next_state = grant4;
			else
			if(req[5])	next_state = grant5;
			else
			if(req[6])	next_state = grant6;
			else
			if(req[7])	next_state = grant7;
			else
			if(req[8])	next_state = grant8;
			else
			if(req[9])	next_state = grant9;
			else
			if(req[10])	next_state = grant10;
			else
			if(req[11])	next_state = grant11;
			else
			if(req[12])	next_state = grant12;
			else
			if(req[13])	next_state = grant13;
			else
			if(req[14])	next_state = grant14;
			else
			if(req[15])	next_state = grant15;
			else
			if(req[16])	next_state = grant16;
			else
			if(req[17])	next_state = grant17;
			else
			if(req[18])	next_state = grant18;
			else
			if(req[19])	next_state = grant19;
			else
			if(req[20])	next_state = grant20;
			else
			if(req[21])	next_state = grant21;
			else
			if(req[22])	next_state = grant22;
			else
			if(req[23])	next_state = grant23;
			else
			if(req[24])	next_state = grant24;
			else
			if(req[25])	next_state = grant25;
			else
			if(req[26])	next_state = grant26;
			else
			if(req[27])	next_state = grant27;
			else
			if(req[28])	next_state = grant28;
			else
			if(req[29])	next_state = grant29;
		   end
	endcase
   end

endmodule 

