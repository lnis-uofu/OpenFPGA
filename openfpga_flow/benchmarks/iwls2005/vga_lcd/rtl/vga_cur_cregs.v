/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE rev.B2 compliant enhanced VGA/LCD Core            ////
////  Hardware Cursor Color Registers                            ////
////                                                             ////
////  Author: Richard Herveille                                  ////
////          richard@asics.ws                                   ////
////          www.asics.ws                                       ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/projects/vga_lcd ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2002 Richard Herveille                        ////
////                    richard@asics.ws                         ////
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
//  $Id: vga_cur_cregs.v,v 1.3 2003/05/07 09:48:54 rherveille Exp $
//
//  $Date: 2003/05/07 09:48:54 $
//  $Revision: 1.3 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: vga_cur_cregs.v,v $
//               Revision 1.3  2003/05/07 09:48:54  rherveille
//               Fixed some Wishbone RevB.3 related bugs.
//               Changed layout of the core. Blocks are located more logically now.
//               Started work on a dual clocked/double edge 12bit output. Commonly used by external devices like DVI transmitters.
//
//               Revision 1.2  2002/03/04 16:05:52  rherveille
//               Added hardware cursor support to wishbone master.
//               Added provision to turn-off 3D cursors.
//               Fixed some minor bugs.
//
//               Revision 1.1  2002/03/04 11:01:59  rherveille
//               Added 64x64pixels 4bpp hardware cursor support.
//
//

//synopsys translate_off
`include "timescale.v"
//synopsys translate_on

module vga_cur_cregs (
	clk_i, rst_i, arst_i,
	hsel_i, hadr_i, hwe_i, hdat_i, hdat_o, hack_o,
	cadr_i, cdat_o
	);

	//
	// inputs & outputs
	//

	// wishbone signals
	input         clk_i;         // master clock input
	input         rst_i;         // synchronous active high reset
	input         arst_i;        // asynchronous active low reset

	// host interface
	input         hsel_i;        // host select input
	input  [ 2:0] hadr_i;        // host address input
	input         hwe_i;         // host write enable input
	input  [31:0] hdat_i;        // host data in
	output [31:0] hdat_o;        // host data out
	output        hack_o;        // host acknowledge out

	reg [31:0] hdat_o;
	reg        hack_o;
	
	// cursor processor interface
	input  [ 3:0] cadr_i;        // cursor address in
	output [15:0] cdat_o;        // cursor data out

	reg [15:0] cdat_o;


	//
	// variable declarations
	//
	reg  [31:0] cregs [7:0];  // color registers
	wire [31:0] temp_cdat;

	//
	// module body
	//


	////////////////////////////
	// generate host interface

	// write section
	always@(posedge clk_i)
		if (hsel_i & hwe_i)
			cregs[hadr_i] <= #1 hdat_i;

	// read section
	always@(posedge clk_i)
		hdat_o <= #1 cregs[hadr_i];

	// acknowledge section
	always@(posedge clk_i)
		hack_o <= #1 hsel_i & !hack_o;


	//////////////////////////////
	// generate cursor interface

	// read section
	assign temp_cdat = cregs[cadr_i[3:1]];

	always@(posedge clk_i)
		cdat_o <= #1 cadr_i[0] ? temp_cdat[31:16] : temp_cdat[15:0];

endmodule

