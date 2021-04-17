/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE rev.B2 compliant VGA/LCD Core; Dual Clocked Fifo  ////
////                                                             ////
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
//  $Id: vga_fifo_dc.v,v 1.6 2003/08/01 11:46:38 rherveille Exp $
//
//  $Date: 2003/08/01 11:46:38 $
//  $Revision: 1.6 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: vga_fifo_dc.v,v $
//               Revision 1.6  2003/08/01 11:46:38  rherveille
//               1) Rewrote vga_fifo_dc. It now uses gray codes and a more elaborate anti-metastability scheme.
//               2) Changed top level and pixel generator to reflect changes in the fifo.
//               3) Changed a bug in vga_fifo.
//               4) Changed pixel generator and wishbone master to reflect changes.
//
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


/*

  Dual clock FIFO.

  Uses gray codes to move from one clock domain to the other.

  Flags are synchronous to the related clock domain;
  - empty: synchronous to read_clock
  - full : synchronous to write_clock

  CLR is available in both clock-domains.
  Asserting any clr signal resets the entire FIFO.
  When crossing clock domains the clears are synchronized.
  Therefore one clock domain can enter or leave the reset state before the other.
*/

module vga_fifo_dc (rclk, wclk, rclr, wclr, wreq, d, rreq, q, empty, full);

	// parameters
	parameter AWIDTH = 7;  //128 entries
	parameter DWIDTH = 16; //16bit databus

	// inputs & outputs
	input rclk;             // read clock
	input wclk;             // write clock
	input rclr;             // active high synchronous clear, synchronous to read clock
	input wclr;             // active high synchronous clear, synchronous to write clock
	input wreq;             // write request
	input [DWIDTH -1:0] d;  // data input
	input rreq;             // read request
	output [DWIDTH -1:0] q; // data output

	output empty;           // FIFO is empty, synchronous to read clock
	reg empty;
	output full;            // FIFO is full, synchronous to write clock
	reg full;

	// variable declarations
	reg rrst, wrst, srclr, ssrclr, swclr, sswclr;
	reg [AWIDTH -1:0] rptr, wptr, rptr_gray, wptr_gray;

	//
	// module body
	//


	function [AWIDTH:1] bin2gray;
		input [AWIDTH:1] bin;
		integer n;
	begin
		for (n=1; n<AWIDTH; n=n+1)
			bin2gray[n] = bin[n+1] ^ bin[n];

		bin2gray[AWIDTH] = bin[AWIDTH];
	end
	endfunction

	function [AWIDTH:1] gray2bin;
		input [AWIDTH:1] gray;
	begin
		// same logic as bin2gray
		gray2bin = bin2gray(gray);
	end
	endfunction

	//
	// Pointers
	//

	// generate synchronized resets
	always @(posedge rclk)
	begin
	    swclr  <= #1 wclr;
	    sswclr <= #1 swclr;
	    rrst   <= #1 rclr | sswclr;
	end

	always @(posedge wclk)
	begin
	    srclr  <= #1 rclr;
	    ssrclr <= #1 srclr;
	    wrst   <= #1 wclr | ssrclr;
	end


	// read pointer
	always @(posedge rclk)
	  if (rrst) begin
	      rptr      <= #1 0;
	      rptr_gray <= #1 0;
	  end else if (rreq) begin
	      rptr      <= #1 rptr +1'h1;
	      rptr_gray <= #1 bin2gray(rptr +1'h1);
	  end

	// write pointer
	always @(posedge wclk)
	  if (wrst) begin
	      wptr      <= #1 0;
	      wptr_gray <= #1 0;
	  end else if (wreq) begin
	      wptr      <= #1 wptr +1'h1;
	      wptr_gray <= #1 bin2gray(wptr +1'h1);
	  end

	//
	// status flags
	//
	reg [AWIDTH-1:0] srptr_gray, ssrptr_gray;
	reg [AWIDTH-1:0] swptr_gray, sswptr_gray;

	// from one clock domain, to the other
	always @(posedge rclk)
	begin
	    swptr_gray  <= #1 wptr_gray;
	    sswptr_gray <= #1 swptr_gray;
	end

	always @(posedge wclk)
	begin
	    srptr_gray  <= #1 rptr_gray;
	    ssrptr_gray <= #1 srptr_gray;
	end

	// EMPTY
	// WC: wptr did not increase
	always @(posedge rclk)
	  if (rrst)
	    empty <= #1 1'b1;
	  else if (rreq)
	    empty <= #1 bin2gray(rptr +1'h1) == sswptr_gray;
	  else
	    empty <= #1 empty & (rptr_gray == sswptr_gray);


	// FULL
	// WC: rptr did not increase
	always @(posedge wclk)
	  if (wrst)
	    full <= #1 1'b0;
	  else if (wreq)
	    full <= #1 bin2gray(wptr +2'h2) == ssrptr_gray;
	  else
	    full <= #1 full & (bin2gray(wptr + 2'h1) == ssrptr_gray);


	// hookup generic dual ported memory
	generic_dpram #(AWIDTH, DWIDTH) fifo_dc_mem(
		.rclk(rclk),
		.rrst(1'b0),
		.rce(1'b1),
		.oe(1'b1),
		.raddr(rptr),
		.do(q),
		.wclk(wclk),
		.wrst(1'b0),
		.wce(1'b1),
		.we(wreq),
		.waddr(wptr),
		.di(d)
	);

endmodule
