/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE rev.B2 compliant enhanced VGA/LCD Core            ////
////  Hardware Cursor Processor                                  ////
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
//  $Id: vga_curproc.v,v 1.4 2003/05/07 09:48:54 rherveille Exp $
//
//  $Date: 2003/05/07 09:48:54 $
//  $Revision: 1.4 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: vga_curproc.v,v $
//               Revision 1.4  2003/05/07 09:48:54  rherveille
//               Fixed some Wishbone RevB.3 related bugs.
//               Changed layout of the core. Blocks are located more logically now.
//               Started work on a dual clocked/double edge 12bit output. Commonly used by external devices like DVI transmitters.
//
//               Revision 1.3  2002/03/04 16:05:52  rherveille
//               Added hardware cursor support to wishbone master.
//               Added provision to turn-off 3D cursors.
//               Fixed some minor bugs.
//
//               Revision 1.2  2002/03/04 11:01:59  rherveille
//               Added 64x64pixels 4bpp hardware cursor support.
//
//               Revision 1.1  2002/02/16 10:40:00  rherveille
//               Some minor bug-fixes.
//               Changed vga_ssel into vga_curproc (cursor processor).
//
//               Revision 1.1  2002/02/07 05:42:10  rherveille
//               Fixed some bugs discovered by modified testbench
//               Removed / Changed some strange logic constructions
//               Started work on hardware cursor support (not finished yet)
//               Changed top-level name to vga_enh_top.v
//

//synopsys translate_off
`include "timescale.v"
//synopsys translate_on

module vga_curproc (clk, rst_i, Thgate, Tvgate, idat, idat_wreq, 
	cursor_xy, cursor_en, cursor_res, 
	cursor_wadr, cursor_wdat, cursor_we,
	cc_adr_o, cc_dat_i,
	rgb_fifo_wreq, rgb);

	//
	// inputs & outputs
	//

	// wishbone signals
	input         clk;           // master clock input
	input         rst_i;         // synchronous active high reset

	// image size
	input [15:0] Thgate, Tvgate; // horizontal/vertical gate
	// image data
	input [23:0] idat;           // image data input
	input        idat_wreq;      // image data write request

	// cursor data
	input [31:0] cursor_xy;      // cursor (x,y)
	input        cursor_en;      // cursor enable (on/off)
	input        cursor_res;     // cursor resolution (32x32 or 64x64 pixels)
	input [ 8:0] cursor_wadr;    // cursor buffer write address
	input [31:0] cursor_wdat;    // cursor buffer write data
	input        cursor_we;      // cursor buffer write enable

	// color registers interface
	output [ 3:0] cc_adr_o;      // cursor color registers address
	reg  [ 3:0] cc_adr_o;    
	input  [15:0] cc_dat_i;      // cursor color registers data

	// rgb-fifo connections
	output        rgb_fifo_wreq; // rgb-out write request
	reg        rgb_fifo_wreq;
	output [23:0] rgb;           // rgb data output
	reg [23:0] rgb;

	//
	// variable declarations
	//
	reg         dcursor_en, ddcursor_en, dddcursor_en;
	reg  [15:0] xcnt, ycnt;
	wire        xdone, ydone;
	wire [15:0] cursor_x, cursor_y;
	wire        cursor_isalpha;
	reg  [15:0] cdat, dcdat;
	wire [ 7:0] cursor_r, cursor_g, cursor_b, cursor_alpha;
	reg         inbox_x, inbox_y;
	wire        inbox;
	reg         dinbox, ddinbox, dddinbox;

	reg  [23:0] didat, ddidat, dddidat;
	reg         didat_wreq, ddidat_wreq;
	wire [31:0] cbuf_q;
	reg  [11:0] cbuf_ra;
	reg  [ 2:0] dcbuf_ra;
	wire [ 8:0] cbuf_a;

	reg         store1, store2;

	//
	// module body
	//

	//
	// generate x-y counters
	always@(posedge clk)
		if(rst_i || xdone)
			xcnt <= #1 16'h0;
		else if (idat_wreq)
			xcnt <= #1 xcnt + 16'h1;

	assign xdone = (xcnt == Thgate) && idat_wreq;

	always@(posedge clk)
		if(rst_i || ydone)
			ycnt <= #1 16'h0;
		else if (xdone)
			ycnt <= #1 ycnt + 16'h1;

	assign ydone = (ycnt == Tvgate) && xdone;


	// decode cursor (x,y)
	assign cursor_x = cursor_xy[15: 0];
	assign cursor_y = cursor_xy[31:16];

	//
	// generate inbox signals
	always@(posedge clk)
		begin
			inbox_x <= #1 (xcnt >= cursor_x) && (xcnt < (cursor_x + (cursor_res ? 16'h7f : 16'h1f) ));
			inbox_y <= #1 (ycnt >= cursor_y) && (ycnt < (cursor_y + (cursor_res ? 16'h7f : 16'h1f) ));
		end

	assign inbox = inbox_x && inbox_y;

	always@(posedge clk)
		dinbox <= #1 inbox;

	always@(posedge clk)
		if (didat_wreq)
			ddinbox <= #1 dinbox;

	always@(posedge clk)
		dddinbox <= #1 ddinbox;

	//
	// generate cursor buffer address counter
	always@(posedge clk)
		if (!cursor_en || ydone)
			cbuf_ra <= #1 12'h0;
		else if (inbox && idat_wreq)
			cbuf_ra <= #1 cbuf_ra +12'h1;

	always@(posedge clk)
		dcbuf_ra <= #1 cbuf_ra[2:0];

	assign cbuf_a = cursor_we ? cursor_wadr : cursor_res ? cbuf_ra[11:3] : cbuf_ra[9:1];

	// hookup local cursor memory (generic synchronous single port memory)
	// cursor memory should never be written to/read from at the same time
	generic_spram #(9, 32) cbuf(
		.clk(clk),
		.rst(1'b0),       // no reset
		.ce(1'b1),        // always enable memory
		.we(cursor_we),
		.oe(1'b1),        // always output data
		.addr(cbuf_a),
		.di(cursor_wdat),
		.do(cbuf_q)
	);

	//
	// decode cursor data for 32x32x16bpp mode
	always@(posedge clk)
		if (didat_wreq)
			cdat <= #1 dcbuf_ra[0] ? cbuf_q[31:16] : cbuf_q[15:0];

	always@(posedge clk)
		dcdat <= #1 cdat;

	//
	// decode cursor data for 64x64x4bpp mode

	// generate cursor-color address
	always@(posedge clk)
		if (didat_wreq)
			case (dcbuf_ra)
				3'b000: cc_adr_o <= cbuf_q[ 3: 0];
				3'b001: cc_adr_o <= cbuf_q[ 7: 4];
				3'b010: cc_adr_o <= cbuf_q[11: 8];
				3'b011: cc_adr_o <= cbuf_q[15:12];
				3'b100: cc_adr_o <= cbuf_q[19:16];
				3'b101: cc_adr_o <= cbuf_q[23:20];
				3'b110: cc_adr_o <= cbuf_q[27:24];
				3'b111: cc_adr_o <= cbuf_q[31:28];
			endcase

	//
	// generate cursor colors
	assign cursor_isalpha =  cursor_res ? cc_dat_i[15]    : dcdat[15];
	assign cursor_alpha   =  cursor_res ? cc_dat_i[7:0]   : dcdat[7:0];
	assign cursor_r       = {cursor_res ? cc_dat_i[14:10] : dcdat[14:10], 3'h0};
	assign cursor_g       = {cursor_res ? cc_dat_i[ 9: 5] : dcdat[ 9: 5], 3'h0};
	assign cursor_b       = {cursor_res ? cc_dat_i[ 4: 0] : dcdat[ 4: 0], 3'h0};

	//
	// delay image data
	always@(posedge clk)
		didat <= #1 idat;

	always@(posedge clk)
		if (didat_wreq)
			ddidat <= #1 didat;

	always@(posedge clk)
		dddidat <= #1 ddidat;

	always@(posedge clk)
		begin
			didat_wreq  <= #1 idat_wreq;
			ddidat_wreq <= #1 didat_wreq;
		end

	//
	// generate selection unit
	always@(posedge clk)
		dcursor_en <= #1 cursor_en;

	always@(posedge clk)
		if (didat_wreq)
			ddcursor_en <= #1 dcursor_en;

	always@(posedge clk)
		dddcursor_en <= #1 ddcursor_en;

	// Alpha blending:
	// rgb = (rgb1 * alhpa1) + (rgb2 * alpha2)
	// We generate an alpha mixer (alpha1 + alpha2 = 1)
	// rgb = (alpha1)(rgb1) + (1-alpha1)(rgb2)
	// We always mix to black (rgb2 = 0)
	// rgb = (alpha1)(rgb1)
	always@(posedge clk)
		if (ddidat_wreq)
			if (!dddcursor_en || !dddinbox)
				rgb <= #1 dddidat;
			else if (cursor_isalpha)
				`ifdef VGA_HWC_3D
					rgb <= #1 dddidat * cursor_alpha;
				`else
					rgb <= #1 dddidat;
				`endif
			else
				rgb <= #1 {cursor_r, cursor_g, cursor_b};

	//
	// generate write request signal
	always@(posedge clk)
		if (rst_i)
		begin
			store1 <= #1 1'b0;
			store2 <= #1 1'b0;
		end
		else
		begin
			store1 <= #1  didat_wreq           | store1;
			store2 <= #1 (didat_wreq & store1) | store2;
		end

	// skip 2 idat_wreq signal, to keep in pace with rgb_fifo_full signal
	always@(posedge clk)
		rgb_fifo_wreq <= #1 ddidat_wreq & store2;

endmodule
