/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE rev.B2 compliant VGA/LCD Core; Timing Generator   ////
////  Horizontal and Vertical Timing Generator                   ////
////                                                             ////
////  Author: Richard Herveille                                  ////
////          richard@asics.ws                                   ////
////          www.asics.ws                                       ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/projects/vga_lcd ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2001 Richard Herveille                        ////
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
//  $Id: vga_tgen.v,v 1.5 2003/05/07 09:48:54 rherveille Exp $
//
//  $Date: 2003/05/07 09:48:54 $
//  $Revision: 1.5 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: vga_tgen.v,v $
//               Revision 1.5  2003/05/07 09:48:54  rherveille
//               Fixed some Wishbone RevB.3 related bugs.
//               Changed layout of the core. Blocks are located more logically now.
//               Started work on a dual clocked/double edge 12bit output. Commonly used by external devices like DVI transmitters.
//
//               Revision 1.4  2002/01/28 03:47:16  rherveille
//               Changed counter-library.
//               Changed vga-core.
//               Added 32bpp mode.
//

//synopsys translate_off
`include "timescale.v"
//synopsys translate_on

module vga_tgen(
	clk, clk_ena, rst,
	Thsync, Thgdel, Thgate, Thlen, Tvsync, Tvgdel, Tvgate, Tvlen,
	eol, eof, gate, hsync, vsync, csync, blank
	);

	// inputs & outputs
	input clk;
	input clk_ena;
	input rst;

	// horizontal timing settings inputs
	input [ 7:0] Thsync; // horizontal sync pule width (in pixels)
	input [ 7:0] Thgdel; // horizontal gate delay
	input [15:0] Thgate; // horizontal gate (number of visible pixels per line)
	input [15:0] Thlen;  // horizontal length (number of pixels per line)

	// vertical timing settings inputs
	input [ 7:0] Tvsync; // vertical sync pule width (in pixels)
	input [ 7:0] Tvgdel; // vertical gate delay
	input [15:0] Tvgate; // vertical gate (number of visible pixels per line)
	input [15:0] Tvlen;  // vertical length (number of pixels per line)

	// outputs
	output eol;  // end of line
	output eof;  // end of frame
	output gate; // vertical AND horizontal gate (logical AND function)

	output hsync; // horizontal sync pulse
	output vsync; // vertical sync pulse
	output csync; // composite sync
	output blank; // blank signal

	//
	// variable declarations
	//
	wire Hgate, Vgate;
	wire Hdone;

	//
	// module body
	//

	// hookup horizontal timing generator
	vga_vtim hor_gen(
		.clk(clk),
		.ena(clk_ena),
		.rst(rst),
		.Tsync(Thsync),
		.Tgdel(Thgdel),
		.Tgate(Thgate),
		.Tlen(Thlen),
		.Sync(hsync),
		.Gate(Hgate),
		.Done(Hdone)
	);


	// hookup vertical timing generator
	wire vclk_ena = Hdone & clk_ena;

	vga_vtim ver_gen(
		.clk(clk),
		.ena(vclk_ena),
		.rst(rst),
		.Tsync(Tvsync),
		.Tgdel(Tvgdel),
		.Tgate(Tvgate),
		.Tlen(Tvlen),
		.Sync(vsync),
		.Gate(Vgate),
		.Done(eof)
	);

	// assign outputs
	assign eol  = Hdone;
	assign gate = Hgate & Vgate;
	assign csync = hsync | vsync;
	assign blank = ~gate;
endmodule
