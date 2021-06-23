/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE rev.B2 compliant VGA/LCD Core                     ////
////  Enhanced Color Processor                                   ////
////                                                             ////
////  Author: Richard Herveille                                  ////
////          richard@asics.ws                                   ////
////          www.asics.ws                                       ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/projects/vga_lcd ////
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
//  $Id: vga_colproc.v,v 1.8 2003/05/07 09:48:54 rherveille Exp $
//
//  $Date: 2003/05/07 09:48:54 $
//  $Revision: 1.8 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: vga_colproc.v,v $
//               Revision 1.8  2003/05/07 09:48:54  rherveille
//               Fixed some Wishbone RevB.3 related bugs.
//               Changed layout of the core. Blocks are located more logically now.
//               Started work on a dual clocked/double edge 12bit output. Commonly used by external devices like DVI transmitters.
//
//               Revision 1.7  2002/03/04 11:01:59  rherveille
//               Added 64x64pixels 4bpp hardware cursor support.
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

module vga_colproc(clk, srst, vdat_buffer_di, ColorDepth, PseudoColor, 
	vdat_buffer_empty, vdat_buffer_rreq, rgb_fifo_full,
	rgb_fifo_wreq, r, g, b,
	clut_req, clut_ack, clut_offs, clut_q
	);

	//
	// inputs & outputs
	//
	input clk;                    // master clock
	input srst;                   // synchronous reset

	input [31:0] vdat_buffer_di;  // video memory data input

	input [1:0] ColorDepth;       // color depth (8bpp, 16bpp, 24bpp)
	input       PseudoColor;      // pseudo color enabled (only for 8bpp color depth)

	input  vdat_buffer_empty;
	output vdat_buffer_rreq;      // pixel buffer read request
	reg    vdat_buffer_rreq;

	input  rgb_fifo_full;
	output rgb_fifo_wreq;
	reg    rgb_fifo_wreq;
	output [7:0] r, g, b;         // pixel color information
	reg    [7:0] r, g, b;

	output        clut_req;       // clut request
	reg clut_req;
	input         clut_ack;       // clut acknowledge
	output [ 7:0] clut_offs;      // clut offset
	reg [7:0] clut_offs;
	input  [23:0] clut_q;         // clut data in

	//
	// variable declarations
	//
	reg [31:0] DataBuffer;

	reg [7:0] Ra, Ga, Ba;
	reg [1:0] colcnt;
	reg RGBbuf_wreq;

	//
	// Module body
	//

	// store word from pixelbuffer / wishbone input
	always @(posedge clk)
		if (vdat_buffer_rreq)
			DataBuffer <= #1 vdat_buffer_di;

	//
	// generate statemachine
	//
	// extract color information from data buffer
	parameter idle        = 7'b000_0000, 
	          fill_buf    = 7'b000_0001,
	          bw_8bpp     = 7'b000_0010,
	          col_8bpp    = 7'b000_0100,
	          col_16bpp_a = 7'b000_1000,
	          col_16bpp_b = 7'b001_0000,
	          col_24bpp   = 7'b010_0000,
	          col_32bpp   = 7'b100_0000;

	reg [6:0] c_state;   // xsynopsys enum_state
	reg [6:0] nxt_state; // xsynopsys enum_state

	// next state decoder
	always @(c_state or vdat_buffer_empty or ColorDepth or PseudoColor or rgb_fifo_full or colcnt or clut_ack)
	begin : nxt_state_decoder
		// initial value
		nxt_state = c_state;

		case (c_state) // synopsis full_case parallel_case
			// idle state
			idle:
				if (!vdat_buffer_empty && !rgb_fifo_full)
					nxt_state = fill_buf;

			// fill data buffer
			fill_buf:
				case (ColorDepth) // synopsis full_case parallel_case
					2'b00: 
						if (PseudoColor)
							nxt_state = col_8bpp;
						else
							nxt_state = bw_8bpp;

					2'b01:
						nxt_state = col_16bpp_a;

					2'b10:
						nxt_state = col_24bpp;

					2'b11:
						nxt_state = col_32bpp;

				endcase

			//
			// 8 bits per pixel
			//
			bw_8bpp:
				if (!rgb_fifo_full && !(|colcnt) )
					if (!vdat_buffer_empty)
						nxt_state = fill_buf;
					else
						nxt_state = idle;

			col_8bpp:
				// Do NOT check for rgb_fifo_full here.
				// In 8bpp pseudo-color mode the color-processor must always finish
				// the current 4pixel-block(i.e. it runs until colcnt = '11').
				// This is because of the late clut-response which shuffles all
				// signals the state-machine depends on.
				// Because of this we can not do an early video_memory_data fetch,
				// i.e. we can not jump to the fill_buf state. Instead we always
				// jump to idle and check for rgb_fifo_full there.
				//
				// The addition of the cursor-processors forces us to increase the
				// rgb-fifo size. The increased rgb-fifo also handles the above
				// described problem. Thus erradicating the above comment.
				// We add the early video_memory_data fetch again.
				if (!(|colcnt))
					if (!vdat_buffer_empty && !rgb_fifo_full)
						nxt_state = fill_buf;
					else
						nxt_state = idle;

			//
			// 16 bits per pixel
			//
			col_16bpp_a:
				if (!rgb_fifo_full)
					nxt_state = col_16bpp_b;

			col_16bpp_b:
				if (!rgb_fifo_full)
					if (!vdat_buffer_empty)
						nxt_state = fill_buf;
					else
						nxt_state = idle;

			//
			// 24 bits per pixel
			//
			col_24bpp:
				if (!rgb_fifo_full)
					if (colcnt == 2'h1) // (colcnt == 1)
						nxt_state = col_24bpp; // stay in current state
					else if (!vdat_buffer_empty)
						nxt_state = fill_buf;
					else
						nxt_state = idle;

			//
			// 32 bits per pixel
			//
			col_32bpp:
				if (!rgb_fifo_full)
					if (!vdat_buffer_empty)
						nxt_state = fill_buf;
					else
						nxt_state = idle;
		endcase
	end // next state decoder

	// generate state registers
	always @(posedge clk)
			if (srst)
				c_state <= #1 idle;
			else
				c_state <= #1 nxt_state;


	reg iclut_req;
	reg ivdat_buf_rreq;
	reg [7:0] iR, iG, iB, iRa, iGa, iBa;

	// output decoder
	always @(c_state or vdat_buffer_empty or colcnt or DataBuffer or rgb_fifo_full or clut_ack or clut_q or Ba or Ga or Ra)
	begin : output_decoder

		// initial values
		ivdat_buf_rreq = 1'b0;
		RGBbuf_wreq = 1'b0;
		iclut_req = 1'b0;
				
		iR  = 'h0;
		iG  = 'h0;
		iB  = 'h0;
		iRa = 'h0;
		iGa = 'h0;
		iBa = 'h0;

		case (c_state) // synopsis full_case parallel_case
			idle:
				begin
					if (!rgb_fifo_full)
						if (!vdat_buffer_empty)
							ivdat_buf_rreq = 1'b1;

					// when entering from 8bpp_pseudo_color_mode
					RGBbuf_wreq = clut_ack;

					iR = clut_q[23:16];
					iG = clut_q[15: 8];
					iB = clut_q[ 7: 0];
				end

			fill_buf:
				begin
					// when entering from 8bpp_pseudo_color_mode
					RGBbuf_wreq = clut_ack;

					iR = clut_q[23:16];
					iG = clut_q[15: 8];
					iB = clut_q[ 7: 0];
				end

			//		
			// 8 bits per pixel
			//
			bw_8bpp:
			begin
				if (!rgb_fifo_full)
					begin
						RGBbuf_wreq = 1'b1;

						if ( (!vdat_buffer_empty) && !(|colcnt) )
							ivdat_buf_rreq = 1'b1;
					end

				case (colcnt) // synopsis full_case parallel_case
					2'b11:
					begin
						iR = DataBuffer[31:24];
						iG = DataBuffer[31:24];
						iB = DataBuffer[31:24];
					end

					2'b10:
					begin
						iR = DataBuffer[23:16];
						iG = DataBuffer[23:16];
						iB = DataBuffer[23:16];
					end

					2'b01:
					begin
						iR = DataBuffer[15:8];
						iG = DataBuffer[15:8];
						iB = DataBuffer[15:8];
					end

					default:
					begin
						iR = DataBuffer[7:0];
						iG = DataBuffer[7:0];
						iB = DataBuffer[7:0];
					end
				endcase
			end

			col_8bpp:
			begin
				// Do NOT check for rgb_fifo_full here.
				// In 8bpp pseudo-color mode the color-processor must always finish
				// the current 4pixel-block(i.e. it runs until colcnt = '11').
				// This is because of the late clut-response which shuffles all
				// signals the state-machine depends on.
				// Because of this we can not do an early video_memory_data fetch,
				// i.e. we can not jump to the fill_buf state. Instead we always
				// jump to idle and check for rgb_fifo_full there.
				//
				// The addition of the cursor-processors forces us to increase the
				// rgb-fifo size. The increased rgb-fifo also handles the above
				// described problem. Thus erradicating the above comment.
				// We add the early video_memory_data fetch again.
				if (!(|colcnt))
					if (!vdat_buffer_empty && !rgb_fifo_full)
						ivdat_buf_rreq = 1'b1;

				RGBbuf_wreq = clut_ack;

				iR = clut_q[23:16];
				iG = clut_q[15: 8];
				iB = clut_q[ 7: 0];

				iclut_req = !rgb_fifo_full || (colcnt[1] ^ colcnt[0]);
			end

			//
			// 16 bits per pixel
			//
			col_16bpp_a:
			begin
				if (!rgb_fifo_full)
					RGBbuf_wreq = 1'b1;

				iR[7:3] = DataBuffer[31:27];
				iG[7:2] = DataBuffer[26:21];
				iB[7:3] = DataBuffer[20:16];
			end

			col_16bpp_b:
			begin
				if (!rgb_fifo_full)
					begin
						RGBbuf_wreq = 1'b1;

						if (!vdat_buffer_empty)
							ivdat_buf_rreq = 1'b1;
					end

				iR[7:3] = DataBuffer[15:11];
				iG[7:2] = DataBuffer[10: 5];
				iB[7:3] = DataBuffer[ 4: 0];
			end

			//
			// 24 bits per pixel
			//
			col_24bpp:
			begin
				if (!rgb_fifo_full)
					begin
						RGBbuf_wreq = 1'b1;

						if ( (colcnt != 2'h1) && !vdat_buffer_empty)
							ivdat_buf_rreq = 1'b1;
					end


				case (colcnt) // synopsis full_case parallel_case
					2'b11:
					begin
						iR  = DataBuffer[31:24];
						iG  = DataBuffer[23:16];
						iB  = DataBuffer[15: 8];
						iRa = DataBuffer[ 7: 0];
					end

					2'b10:
					begin
						iR  = Ra;
						iG  = DataBuffer[31:24];
						iB  = DataBuffer[23:16];
						iRa = DataBuffer[15: 8];
						iGa = DataBuffer[ 7: 0];
					end

					2'b01:
					begin
						iR  = Ra;
						iG  = Ga;
						iB  = DataBuffer[31:24];
						iRa = DataBuffer[23:16];
						iGa = DataBuffer[15: 8];
						iBa = DataBuffer[ 7: 0];
					end

					default:
					begin
						iR = Ra;
						iG = Ga;
						iB = Ba;
					end
				endcase
			end

			//
			// 32 bits per pixel
			//
			col_32bpp:
			begin
				if (!rgb_fifo_full)
					begin
						RGBbuf_wreq = 1'b1;

						if (!vdat_buffer_empty)
							ivdat_buf_rreq = 1'b1;
					end

				iR[7:0] = DataBuffer[23:16];
				iG[7:0] = DataBuffer[15:8];
				iB[7:0] = DataBuffer[7:0];
			end

		endcase
	end // output decoder

	// generate output registers
	always @(posedge clk)
		begin
			r  <= #1 iR;
			g  <= #1 iG;
			b  <= #1 iB;

			if (RGBbuf_wreq)
				begin
					Ra <= #1 iRa;
					Ba <= #1 iBa;
					Ga <= #1 iGa;
				end

			if (srst)
				begin
					vdat_buffer_rreq <= #1 1'b0;
					rgb_fifo_wreq <= #1 1'b0;
					clut_req <= #1 1'b0;
				end
			else
				begin
					vdat_buffer_rreq <= #1 ivdat_buf_rreq;
					rgb_fifo_wreq <= #1 RGBbuf_wreq;
					clut_req <= #1 iclut_req;
				end
	end

	// assign clut offset
	always @(colcnt or DataBuffer)
	  case (colcnt) // synopsis full_case parallel_case
	      2'b11: clut_offs = DataBuffer[31:24];
	      2'b10: clut_offs = DataBuffer[23:16];
	      2'b01: clut_offs = DataBuffer[15: 8];
	      2'b00: clut_offs = DataBuffer[ 7: 0];
	  endcase


	//
	// color counter
	//
	always @(posedge clk)
	  if (srst)
	    colcnt <= #1 2'b11;
	  else if (RGBbuf_wreq)
	    colcnt <= #1 colcnt -2'h1;
endmodule


