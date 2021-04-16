/////////////////////////////////////////////////////////////////////
////                                                             ////
////  generic FIFO, uses LFSRs for read/write pointers           ////
////                                                             ////
////  Author: Richard Herveille                                  ////
////          richard@asics.ws                                   ////
////          www.asics.ws                                       ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2001, 2002 Richard Herveille                  ////
////                          richard@asics.ws                   ////
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
//  $Id: vga_fifo.v,v 1.8 2003/08/01 11:46:38 rherveille Exp $
//
//  $Date: 2003/08/01 11:46:38 $
//  $Revision: 1.8 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: vga_fifo.v,v $
//               Revision 1.8  2003/08/01 11:46:38  rherveille
//               1) Rewrote vga_fifo_dc. It now uses gray codes and a more elaborate anti-metastability scheme.
//               2) Changed top level and pixel generator to reflect changes in the fifo.
//               3) Changed a bug in vga_fifo.
//               4) Changed pixel generator and wishbone master to reflect changes.
//
//               Revision 1.7  2003/05/07 09:48:54  rherveille
//               Fixed some Wishbone RevB.3 related bugs.
//               Changed layout of the core. Blocks are located more logically now.
//               Started work on a dual clocked/double edge 12bit output. Commonly used by external devices like DVI transmitters.
//
//               Revision 1.6  2002/02/07 05:42:10  rherveille
//               Fixed some bugs discovered by modified testbench
//               Removed / Changed some strange logic constructions
//               Started work on hardware cursor support (not finished yet)
//               Changed top-level name to vga_enh_top.v
//

//synopsys translate_off
`include "timescale.v"
//synopsys translate_on


// set FIFO_RW_CHECK to prevent writing to a full and reading from an empty FIFO
//`define FIFO_RW_CHECK

// Long Pseudo Random Generators can generate (N^2 -1) combinations. This means
// 1 FIFO entry is unavailable. This might be a problem, especially for small
// FIFOs. Setting VGA_FIFO_ALL_ENTRIES creates additional logic that ensures that
// all FIFO entries are used at the expense of some additional logic.
`define VGA_FIFO_ALL_ENTRIES

module vga_fifo (
	clk,
	aclr,
	sclr,
	wreq,
	rreq,
	d,
	q,
	nword,
	empty,
	full,
	aempty,
	afull
	);

	//
	// parameters
	//
	parameter aw =  3;                         // no.of entries (in bits; 2^7=128 entries)
	parameter dw =  8;                         // datawidth (in bits)

	//
	// inputs & outputs
	//
	input             clk;                     // master clock
	input             aclr;                    // asynchronous active low reset
	input             sclr;                    // synchronous active high reset

	input             wreq;                    // write request
	input             rreq;                    // read request
	input  [dw:1]     d;                       // data-input
	output [dw:1]     q;                       // data-output

	output [aw:0]     nword;                   // number of words in FIFO

	output            empty;                   // fifo empty
	output            full;                    // fifo full

	output            aempty;                  // fifo asynchronous/almost empty (1 entry left)
	output            afull;                   // fifo asynchronous/almost full (1 entry left)

	reg [aw:0] nword;
	reg        empty, full;

	//
	// Module body
	//
	reg  [aw:1] rp, wp;
	wire [dw:1] ramq;
	wire fwreq, frreq;

`ifdef VGA_FIFO_ALL_ENTRIES
	function lsb;
	   input [aw:1] q;
	   case (aw)
	       2: lsb = ~q[2];
	       3: lsb = &q[aw-1:1] ^ ~(q[3] ^ q[2]);
	       4: lsb = &q[aw-1:1] ^ ~(q[4] ^ q[3]);
	       5: lsb = &q[aw-1:1] ^ ~(q[5] ^ q[3]);
	       6: lsb = &q[aw-1:1] ^ ~(q[6] ^ q[5]);
	       7: lsb = &q[aw-1:1] ^ ~(q[7] ^ q[6]);
	       8: lsb = &q[aw-1:1] ^ ~(q[8] ^ q[6] ^ q[5] ^ q[4]);
	       9: lsb = &q[aw-1:1] ^ ~(q[9] ^ q[5]);
	      10: lsb = &q[aw-1:1] ^ ~(q[10] ^ q[7]);
	      11: lsb = &q[aw-1:1] ^ ~(q[11] ^ q[9]);
	      12: lsb = &q[aw-1:1] ^ ~(q[12] ^ q[6] ^ q[4] ^ q[1]);
	      13: lsb = &q[aw-1:1] ^ ~(q[13] ^ q[4] ^ q[3] ^ q[1]);
	      14: lsb = &q[aw-1:1] ^ ~(q[14] ^ q[5] ^ q[3] ^ q[1]);
	      15: lsb = &q[aw-1:1] ^ ~(q[15] ^ q[14]);
	      16: lsb = &q[aw-1:1] ^ ~(q[16] ^ q[15] ^ q[13] ^ q[4]);
	   endcase
	endfunction
`else
	function lsb;
	   input [aw:1] q;
	   case (aw)
	       2: lsb = ~q[2];
	       3: lsb = ~(q[3] ^ q[2]);
	       4: lsb = ~(q[4] ^ q[3]);
	       5: lsb = ~(q[5] ^ q[3]);
	       6: lsb = ~(q[6] ^ q[5]);
	       7: lsb = ~(q[7] ^ q[6]);
	       8: lsb = ~(q[8] ^ q[6] ^ q[5] ^ q[4]);
	       9: lsb = ~(q[9] ^ q[5]);
	      10: lsb = ~(q[10] ^ q[7]);
	      11: lsb = ~(q[11] ^ q[9]);
	      12: lsb = ~(q[12] ^ q[6] ^ q[4] ^ q[1]);
	      13: lsb = ~(q[13] ^ q[4] ^ q[3] ^ q[1]);
	      14: lsb = ~(q[14] ^ q[5] ^ q[3] ^ q[1]);
	      15: lsb = ~(q[15] ^ q[14]);
	      16: lsb = ~(q[16] ^ q[15] ^ q[13] ^ q[4]);
	   endcase
	endfunction
`endif

`ifdef RW_CHECK
  assign fwreq = wreq & ~full;
  assign frreq = rreq & ~empty;
`else
  assign fwreq = wreq;
  assign frreq = rreq;
`endif

	//
	// hookup read-pointer
	//
	always @(posedge clk or negedge aclr)
	  if (~aclr)      rp <= #1 0;
	  else if (sclr)  rp <= #1 0;
	  else if (frreq) rp <= #1 {rp[aw-1:1], lsb(rp)};

	//
	// hookup write-pointer
	//
	always @(posedge clk or negedge aclr)
	  if (~aclr)      wp <= #1 0;
	  else if (sclr)  wp <= #1 0;
	  else if (fwreq) wp <= #1 {wp[aw-1:1], lsb(wp)};


	//
	// hookup memory-block
	//
	reg [dw:1] mem [(1<<aw) -1:0];

	// memory array operations
	always @(posedge clk)
	  if (fwreq)
	    mem[wp] <= #1 d;

	assign q = mem[rp];


	// generate full/empty signals
	assign aempty = (rp[aw-1:1] == wp[aw:2]) & (lsb(rp) == wp[1]) & frreq & ~fwreq;
	always @(posedge clk or negedge aclr)
	  if (~aclr)
	    empty <= #1 1'b1;
	  else if (sclr)
	    empty <= #1 1'b1;
	  else
	    empty <= #1 aempty | (empty & (~fwreq + frreq));

	assign afull = (wp[aw-1:1] == rp[aw:2]) & (lsb(wp) == rp[1]) & fwreq & ~frreq;
	always @(posedge clk or negedge aclr)
	  if (~aclr)
	    full <= #1 1'b0;
	  else if (sclr)
	    full <= #1 1'b0;
	  else
	    full <= #1 afull | ( full & (~frreq + fwreq) );

	// number of words in fifo
	always @(posedge clk or negedge aclr)
	  if (~aclr)
	    nword <= #1 0;
	  else if (sclr)
	    nword <= #1 0;
	  else
	    begin
	        if (wreq & !rreq)
	          nword <= #1 nword +1;
	        else if (rreq & !wreq)
	          nword <= #1 nword -1;
	    end

	//
	// Simulation checks
	//
	// synopsys translate_off
	always @(posedge clk)
	  if (full & fwreq)
	    $display("Writing while FIFO full (%m)\n");

	always @(posedge clk)
	  if (empty & frreq)
	    $display("Reading while FIFO empty (%m)\n");
	// synopsys translate_on
endmodule


