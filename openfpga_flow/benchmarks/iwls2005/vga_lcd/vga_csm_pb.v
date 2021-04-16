/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE rev.B2 compliant VGA/LCD Core; CycleShared Memory ////
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
//  $Id: vga_csm_pb.v,v 1.7 2003/05/07 09:48:54 rherveille Exp $
//
//  $Date: 2003/05/07 09:48:54 $
//  $Revision: 1.7 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: vga_csm_pb.v,v $
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

module vga_csm_pb (clk_i, req0_i, ack0_o, adr0_i, dat0_i, dat0_o, we0_i, req1_i, ack1_o, adr1_i, dat1_i, dat1_o, we1_i);
		
	//
	// parameters
	//
	parameter DWIDTH = 32; // databus width
	parameter AWIDTH = 8;  // address bus width

	//
	// inputs & outputs
	//
	
	input clk_i;                    // clock input

	// wishbone slave0 connections
	input  [ AWIDTH   -1:0] adr0_i; // address input
	input  [ DWIDTH   -1:0] dat0_i; // data input
	output [ DWIDTH   -1:0] dat0_o; // data output
	input                   we0_i;  // write enable input
	input                   req0_i; // access request input
	output                  ack0_o; // access acknowledge output

	// wishbone slave1 connections
	input  [ AWIDTH   -1:0] adr1_i; // address input
	input  [ DWIDTH   -1:0] dat1_i; // data input
	output [ DWIDTH   -1:0] dat1_o; // data output
	input                   we1_i;  // write enable input
	input                   req1_i; // access request input
	output                  ack1_o; // access acknowledge output

	//
	// variable declarations
	//

	// multiplexor select signal
	wire acc0, acc1;
	reg  dacc0, dacc1;
	wire sel0, sel1;
	reg  ack0, ack1;
	
	// memory data output
	wire [DWIDTH -1:0] mem_q;


	//
	// module body
	//

	// generate multiplexor select signal
	assign acc0 = req0_i;
	assign acc1 = req1_i && !sel0;

	always@(posedge clk_i)
		begin
			dacc0 <= #1 acc0 & !ack0_o;
			dacc1 <= #1 acc1 & !ack1_o;
		end

	assign sel0 = acc0 && !dacc0;
	assign sel1 = acc1 && !dacc1;

	always@(posedge clk_i)
		begin
			ack0 <= #1 sel0 && !ack0_o;
			ack1 <= #1 sel1 && !ack1_o;
		end

	wire [AWIDTH -1:0] mem_adr = sel0 ? adr0_i : adr1_i;
	wire [DWIDTH -1:0] mem_d   = sel0 ? dat0_i : dat1_i;
	wire               mem_we  = sel0 ? req0_i && we0_i : req1_i && we1_i;

	// hookup generic synchronous single port memory
	generic_spram #(AWIDTH, DWIDTH) clut_mem(
		.clk(clk_i),
		.rst(1'b0),       // no reset
		.ce(1'b1),        // always enable memory
		.we(mem_we),
		.oe(1'b1),        // always output data
		.addr(mem_adr),
		.di(mem_d),
		.do(mem_q)
	);

	// assign DAT_O outputs
	assign dat0_o = mem_q;
	assign dat1_o = mem_q;

	// generate ack outputs
	assign ack0_o = ( (sel0 && we0_i) || ack0 );
	assign ack1_o = ( (sel1 && we1_i) || ack1 );
endmodule
