/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE rev.B2 compliant VGA/LCD Core; Pixel Generator    ////
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
//  $Id: vga_pgen.v,v 1.7 2003/08/01 11:46:38 rherveille Exp $
//
//  $Date: 2003/08/01 11:46:38 $
//  $Revision: 1.7 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: vga_pgen.v,v $
//               Revision 1.7  2003/08/01 11:46:38  rherveille
//               1) Rewrote vga_fifo_dc. It now uses gray codes and a more elaborate anti-metastability scheme.
//               2) Changed top level and pixel generator to reflect changes in the fifo.
//               3) Changed a bug in vga_fifo.
//               4) Changed pixel generator and wishbone master to reflect changes.
//
//               Revision 1.6  2003/05/07 09:48:54  rherveille
//               Fixed some Wishbone RevB.3 related bugs.
//               Changed layout of the core. Blocks are located more logically now.
//               Started work on a dual clocked/double edge 12bit output. Commonly used by external devices like DVI transmitters.
//
//               Revision 1.5  2002/04/05 06:24:35  rherveille
//               Fixed a potential reset bug in the hint & vint generation.
//
//               Revision 1.4  2002/01/28 03:47:16  rherveille
//               Changed counter-library.
//               Changed vga-core.
//               Added 32bpp mode.
//

//synopsys translate_off
`include "timescale.v"
//synopsys translate_on

`include "vga_defines.v"

module vga_pgen (
	clk_i, ctrl_ven, ctrl_HSyncL, Thsync, Thgdel, Thgate, Thlen,
	ctrl_VSyncL, Tvsync, Tvgdel, Tvgate, Tvlen, ctrl_CSyncL, ctrl_BlankL,
	eoh, eov,
	ctrl_dvi_odf, ctrl_cd, ctrl_pc,
	fb_data_fifo_rreq, fb_data_fifo_empty, fb_data_fifo_q, ImDoneFifoQ,
	stat_acmp, clut_req, clut_adr, clut_q, clut_ack, ctrl_cbsw, clut_switch,
	cursor_adr,
	cursor0_en, cursor0_res, cursor0_xy, cc0_adr_o, cc0_dat_i,
	cursor1_en, cursor1_res, cursor1_xy, cc1_adr_o, cc1_dat_i,
	line_fifo_wreq, line_fifo_full, line_fifo_d, line_fifo_rreq, line_fifo_q,
	pclk_i,
`ifdef VGA_12BIT_DVI
	dvi_pclk_p_o, dvi_pclk_m_o, dvi_hsync_o, dvi_vsync_o, dvi_de_o, dvi_d_o,
`endif
	pclk_o, hsync_o, vsync_o, csync_o, blank_o, r_o, g_o, b_o
);

	// inputs & outputs

	input clk_i; // master clock

	input ctrl_ven;           // Video enable signal

	// horiontal timing settings
	input        ctrl_HSyncL; // horizontal sync pulse polarization level (pos/neg)
	input [ 7:0] Thsync;      // horizontal sync pulse width (in pixels)
	input [ 7:0] Thgdel;      // horizontal gate delay (in pixels)
	input [15:0] Thgate;      // horizontal gate length (number of visible pixels per line)
	input [15:0] Thlen;       // horizontal length (number of pixels per line)

	// vertical timing settings
	input        ctrl_VSyncL; // vertical sync pulse polarization level (pos/neg)
	input [ 7:0] Tvsync;      // vertical sync pulse width (in lines)
	input [ 7:0] Tvgdel;      // vertical gate delay (in lines)
	input [15:0] Tvgate;      // vertical gate length (number of visible lines in frame)
	input [15:0] Tvlen;       // vertical length (number of lines in frame)

	// composite signals
	input ctrl_CSyncL;        // composite sync pulse polarization level
	input ctrl_BlankL;        // blank signal polarization level

	// status outputs
	output eoh;               // end of horizontal
	reg eoh;
	output eov;               // end of vertical;
	reg eov;


	// Pixel signals
	input  [ 1: 0] ctrl_dvi_odf;
	input  [ 1: 0] ctrl_cd;
	input          ctrl_pc;

	input  [31: 0] fb_data_fifo_q;
	input          fb_data_fifo_empty;
	output         fb_data_fifo_rreq;
	input          ImDoneFifoQ;

	output         stat_acmp;   // active CLUT memory page
	reg stat_acmp;
	output         clut_req;
	output [ 8: 0] clut_adr;
	input  [23: 0] clut_q;
	input          clut_ack;
	input          ctrl_cbsw;   // enable clut bank switching
	output         clut_switch; // clut memory bank-switch request: clut page switched (when enabled)

	input  [ 8: 0] cursor_adr;  // cursor data address (from wbm)
	input          cursor0_en;  // enable hardware cursor0
	input          cursor0_res; // cursor0 resolution
	input  [31: 0] cursor0_xy;  // (x,y) address hardware cursor0
	output [ 3: 0] cc0_adr_o;   // cursor0 color registers address output
	input  [15: 0] cc0_dat_i;   // cursor0 color registers data input
	input          cursor1_en;  // enable hardware cursor1
	input          cursor1_res; // cursor1 resolution
	input  [31: 0] cursor1_xy;  // (x,y) address hardware cursor1
	output [ 3: 0] cc1_adr_o;   // cursor1 color registers address output
	input  [15: 0] cc1_dat_i;   // cursor1 color registers data input

	input          line_fifo_full;
	output         line_fifo_wreq;
	output [23: 0] line_fifo_d;
	output         line_fifo_rreq;
	input  [23: 0] line_fifo_q;


	// pixel clock related outputs
	input  pclk_i;            // pixel clock in
	output pclk_o;            // pixel clock out

	output hsync_o;           // horizontal sync pulse
	output vsync_o;           // vertical sync pulse
	output csync_o;           // composite sync: Hsync OR Vsync (logical OR function)
	output blank_o;           // blanking signal
	output [ 7:0] r_o, g_o, b_o;

	reg       hsync_o, vsync_o, csync_o, blank_o;
	reg [7:0] r_o, g_o, b_o;

	`ifdef VGA_12BIT_DVI
	    output        dvi_pclk_p_o;  // dvi pclk+
	    output        dvi_pclk_m_o;  // dvi pclk-
	    output        dvi_hsync_o;   // dvi hsync
	    output        dvi_vsync_o;   // dvi vsync
	    output        dvi_de_o;      // dvi data enable
	    output [11:0] dvi_d_o;       // dvi 12bit output
	`endif



	//
	// variable declarations
	//
	reg nVen; // video enable signal (active low)
	wire eol, eof;
	wire ihsync, ivsync, icsync, iblank;
	wire pclk_ena;

	//////////////////////////////////
	//
	// module body
	//

	// synchronize timing/control settings (from master-clock-domain to pixel-clock-domain)
	always @(posedge pclk_i)
	  nVen <= #1 ~ctrl_ven;


	//////////////////////////////////
	//
	// Pixel Clock generator
	//

	vga_clkgen clk_gen(
	  .pclk_i       ( pclk_i       ),
	  .rst_i        ( nVen         ),
	  .pclk_o       ( pclk_o       ),
	  .dvi_pclk_p_o ( dvi_pclk_p_o ),
	  .dvi_pclk_m_o ( dvi_pclk_m_o ),
	  .pclk_ena_o   ( pclk_ena     )
	);


	//////////////////////////////////
	//
	// Timing generator
	//

	// hookup video timing generator
	vga_tgen vtgen(
		.clk(pclk_i),
		.clk_ena ( pclk_ena    ),
		.rst     ( nVen        ),
		.Thsync  ( Thsync      ),
		.Thgdel  ( Thgdel      ),
		.Thgate  ( Thgate      ),
		.Thlen   ( Thlen       ),
		.Tvsync  ( Tvsync      ),
		.Tvgdel  ( Tvgdel      ),
		.Tvgate  ( Tvgate      ),
		.Tvlen   ( Tvlen       ),
		.eol     ( eol         ),
		.eof     ( eof         ),
		.gate    ( gate        ),
		.hsync   ( ihsync      ),
		.vsync   ( ivsync      ),
		.csync   ( icsync      ),
		.blank   ( iblank      )
	);

	//
	// from pixel-clock-domain to master-clock-domain
	//
	reg seol, seof;   // synchronized end-of-line, end-of-frame
	reg dseol, dseof; // delayed seol, seof

	always @(posedge clk_i)
	  if (~ctrl_ven)
	    begin
	        seol  <= #1 1'b0;
	        dseol <= #1 1'b0;

	        seof  <= #1 1'b0;
	        dseof <= #1 1'b0;

	        eoh   <= #1 1'b0;
	        eov   <= #1 1'b0;
	    end
	  else
	    begin
	        seol  <= #1 eol;
	        dseol <= #1 seol;

	        seof  <= #1 eof;
	        dseof <= #1 seof;

	        eoh   <= #1 seol & !dseol;
	        eov   <= #1 seof & !dseof;
	    end


`ifdef VGA_12BIT_DVI
	always @(posedge pclk_i)
	  if (pclk_ena)
	    begin
	        hsync_o <= #1 ihsync ^ ctrl_HSyncL;
	        vsync_o <= #1 ivsync ^ ctrl_VSyncL;
	        csync_o <= #1 icsync ^ ctrl_CSyncL;
	        blank_o <= #1 iblank ^ ctrl_BlankL;
	    end
`else
	reg hsync, vsync, csync, blank;
	always @(posedge pclk_i)
	    begin
	        hsync <= #1 ihsync ^ ctrl_HSyncL;
	        vsync <= #1 ivsync ^ ctrl_VSyncL;
	        csync <= #1 icsync ^ ctrl_CSyncL;
	        blank <= #1 iblank ^ ctrl_BlankL;

	        hsync_o <= #1 hsync;
	        vsync_o <= #1 vsync;
	        csync_o <= #1 csync;
	        blank_o <= #1 blank;
	    end
`endif



	//////////////////////////////////
	//
	// Pixel generator section
	//

	wire [23:0] color_proc_q;           // data from color processor
	wire        color_proc_wreq;
	wire [ 7:0] clut_offs;               // color lookup table offset

	wire ImDoneFifoQ;
	reg  dImDoneFifoQ, ddImDoneFifoQ;

	wire [23:0] cur1_q;
	wire        cur1_wreq;

	wire [23:0] rgb_fifo_d;
	wire        rgb_fifo_empty, rgb_fifo_full, rgb_fifo_rreq, rgb_fifo_wreq;

	wire sclr = ~ctrl_ven;

	//
	// hookup color processor
	vga_colproc color_proc (
		.clk               ( clk_i               ),
		.srst              ( sclr                ),
		.vdat_buffer_di    ( fb_data_fifo_q      ), //data_fifo_q),
		.ColorDepth        ( ctrl_cd             ),
		.PseudoColor       ( ctrl_pc             ),
		.vdat_buffer_empty ( fb_data_fifo_empty  ), //data_fifo_empty),
		.vdat_buffer_rreq  ( fb_data_fifo_rreq   ), //data_fifo_rreq),
		.rgb_fifo_full     ( rgb_fifo_full       ),
		.rgb_fifo_wreq     ( color_proc_wreq     ),
		.r                 ( color_proc_q[23:16] ),
		.g                 ( color_proc_q[15: 8] ),
		.b                 ( color_proc_q[ 7: 0] ),
		.clut_req          ( clut_req            ),
		.clut_ack          ( clut_ack            ),
		.clut_offs         ( clut_offs           ),
		.clut_q            ( clut_q              )
	);

	//
	// clut bank switch / cursor data delay2: Account for ColorProcessor DataBuffer delay
	always @(posedge clk_i)
	  if (sclr)
	    dImDoneFifoQ <= #1 1'b0;
	  else if (fb_data_fifo_rreq)
	    dImDoneFifoQ <= #1 ImDoneFifoQ;

	always @(posedge clk_i)
	  if (sclr)
	    ddImDoneFifoQ <= #1 1'b0;
	  else
	    ddImDoneFifoQ <= #1 dImDoneFifoQ;

	assign clut_switch = ddImDoneFifoQ & !dImDoneFifoQ;

	always @(posedge clk_i)
	  if (sclr)
	    stat_acmp <= #1 1'b0;
	  else if (ctrl_cbsw)
	    stat_acmp <= #1 stat_acmp ^ clut_switch;  // select next clut when finished reading clut for current video bank (and bank switch enabled)

	// generate clut-address
	assign clut_adr = {stat_acmp, clut_offs};


	//
	// hookup data-source-selector && hardware cursor module
`ifdef VGA_HWC1	// generate Hardware Cursor1 (if enabled)
	wire cursor1_ld_strb;
	reg scursor1_en;
	reg scursor1_res;
	reg [31:0] scursor1_xy;

	assign cursor1_ld_strb = ddImDoneFifoQ & !dImDoneFifoQ;

	always @(posedge clk_i)
	  if (sclr)
	    scursor1_en <= #1 1'b0;
	  else if (cursor1_ld_strb)
	    scursor1_en <= #1 cursor1_en;

	always @(posedge clk_i)
	  if (cursor1_ld_strb)
	    scursor1_xy <= #1 cursor1_xy;

	always @(posedge clk_i)
	  if (cursor1_ld_strb)
	    scursor1_res <= #1 cursor1_res;

	vga_curproc hw_cursor1 (
		.clk           ( clk_i           ),
		.rst_i         ( sclr            ),
		.Thgate        ( Thgate          ),
		.Tvgate        ( Tvgate          ),
		.idat          ( color_proc_q    ),
		.idat_wreq     ( color_proc_wreq ),
		.cursor_xy     ( scursor1_xy     ),
		.cursor_res    ( scursor1_res    ),
		.cursor_en     ( scursor1_en     ),
		.cursor_wadr   ( cursor_adr      ),
		.cursor_we     ( cursor1_we      ),
		.cursor_wdat   ( dat_i           ),
		.cc_adr_o      ( cc1_adr_o       ),
		.cc_dat_i      ( cc1_dat_i       ),
		.rgb_fifo_wreq ( cur1_wreq       ),
		.rgb           ( cur1_q          )
	);

`ifdef VGA_HWC0	// generate additional signals for Hardware Cursor0 (if enabled)
	reg sddImDoneFifoQ, sdImDoneFifoQ;

	always @(posedge clk_i)
	  if (cur1_wreq)
	    begin
	        sdImDoneFifoQ  <= #1 dImDoneFifoQ;
	        sddImDoneFifoQ <= #1 sdImDoneFifoQ;
	    end
`endif

`else		// Hardware Cursor1 disabled, generate pass-through signals
	assign cur1_wreq = color_proc_wreq;
	assign cur1_q    = color_proc_q;

	assign cc1_adr_o  = 4'h0;

`ifdef VGA_HWC0	// generate additional signals for Hardware Cursor0 (if enabled)
	wire sddImDoneFifoQ, sdImDoneFifoQ;

	assign sdImDoneFifoQ  = dImDoneFifoQ;
	assign sddImDoneFifoQ = ddImDoneFifoQ;
`endif

`endif


`ifdef VGA_HWC0	// generate Hardware Cursor0 (if enabled)
	wire cursor0_ld_strb;
	reg scursor0_en;
	reg scursor0_res;
	reg [31:0] scursor0_xy;

	assign cursor0_ld_strb = sddImDoneFifoQ & !sdImDoneFifoQ;

	always @(posedge clk_i)
	  if (sclr)
	    scursor0_en <= #1 1'b0;
	  else if (cursor0_ld_strb)
	    scursor0_en <= #1 cursor0_en;

	always @(posedge clk_i)
	  if (cursor0_ld_strb)
	    scursor0_xy <= #1 cursor0_xy;

	always @(posedge clk_i)
	  if (cursor0_ld_strb)
	    scursor0_res <= #1 cursor0_res;

	vga_curproc hw_cursor0 (
		.clk           ( clk_i         ),
		.rst_i         ( sclr          ),
		.Thgate        ( Thgate        ),
		.Tvgate        ( Tvgate        ),
		.idat          ( ssel1_q       ),
		.idat_wreq     ( ssel1_wreq    ),
		.cursor_xy     ( scursor0_xy   ),
		.cursor_en     ( scursor0_en   ),
		.cursor_res    ( scursor0_res  ),
		.cursor_wadr   ( cursor_adr    ),
		.cursor_we     ( cursor0_we    ),
		.cursor_wdat   ( dat_i         ),
		.cc_adr_o      ( cc0_adr_o     ),
		.cc_dat_i      ( cc0_dat_i     ),
		.rgb_fifo_wreq ( rgb_fifo_wreq ),
		.rgb           ( rgb_fifo_d    )
	);
`else	// Hardware Cursor0 disabled, generate pass-through signals
	assign rgb_fifo_wreq = cur1_wreq;
	assign rgb_fifo_d = cur1_q;

	assign cc0_adr_o  = 4'h0;
`endif

	//
	// hookup RGB buffer (temporary station between WISHBONE-clock-domain
	// and pixel-clock-domain)
	// The cursor_processor pipelines introduce a delay between the color
	// processor's rgb_fifo_wreq and the rgb_fifo_full signals. To compensate
	// for this we double the rgb_fifo.
	wire [4:0] rgb_fifo_nword;

	vga_fifo #(4, 24) rgb_fifo (
		.clk    ( clk_i          ),
		.aclr   ( 1'b1           ),
		.sclr   ( sclr           ),
		.d      ( rgb_fifo_d     ),
		.wreq   ( rgb_fifo_wreq  ),
		.q      ( line_fifo_d    ),
		.rreq   ( rgb_fifo_rreq  ),
		.empty  ( rgb_fifo_empty ),
		.nword  ( rgb_fifo_nword ),
		.full   ( ),
		.aempty ( ),
		.afull  ( )
	);

	assign rgb_fifo_full = rgb_fifo_nword[3]; // actually half full

	assign line_fifo_rreq = gate & pclk_ena;

	assign rgb_fifo_rreq = ~line_fifo_full & ~rgb_fifo_empty;
	assign line_fifo_wreq = rgb_fifo_rreq;

	wire [7:0] r = line_fifo_q[23:16];
	wire [7:0] g = line_fifo_q[15: 8];
	wire [7:0] b = line_fifo_q[ 7: 0];

	always @(posedge pclk_i)
	  if (pclk_ena) begin
	    r_o <= #1 r;
	    g_o <= #1 g;
	    b_o <= #1 b;
	  end


	//
	// DVI section
	//

`ifdef VGA_12BIT_DVI
	reg [11:0] dvi_d_o;
	reg        dvi_de_o;
	reg        dvi_hsync_o;
	reg        dvi_vsync_o;

	reg [11:0] pA, pB;
	reg        dgate, ddgate;
	reg        dhsync, ddhsync;
	reg        dvsync, ddvsync;

	always @(posedge pclk_i)
	  if (pclk_ena)
	    case (ctrl_dvi_odf) // synopsys full_case parallel_case
	      2'b00: pA <= #1 {g[3:0], b[7:0]};
	      2'b01: pA <= #1 {g[4:2], b[7:3], g[0], b[2:0]};
	      2'b10: pA <= #1 {g[4:2], b[7:3], 4'h0};
	      2'b11: pA <= #1 {g[5:3], b[7:3], 4'h0};
	    endcase

	always @(posedge pclk_i)
	  if (pclk_ena)
	    case (ctrl_dvi_odf) // synopsys full_case parallel_case
	      2'b00: pB <= #1 {r[7:0], g[7:4]};
	      2'b01: pB <= #1 {r[7:3], g[7:5], r[2:0], g[1]};
	      2'b10: pB <= #1 {r[7:3], g[7:5], 4'h0};
	      2'b11: pB <= #1 {1'b0, r[7:3], g[7:6], 4'h0};
	    endcase

	always @(posedge pclk_i)
	  if (pclk_ena)
	    dvi_d_o <= #1 pB;
	  else
	    dvi_d_o <= #1 pA;

	always @(posedge pclk_i)
	  if (pclk_ena) begin
	    dgate  <= #1 gate;  // delay once: delayed line fifo output

	    dhsync  <= #1 ~ihsync;
	    ddhsync <= #1 dhsync;

	    dvsync  <= #1 ~ivsync;
	    ddvsync <= #1 dvsync;
	  end

	always @(posedge pclk_i)
	  begin
	      dvi_de_o    <= #1 dgate;
	      dvi_hsync_o <= #1 dhsync;
	      dvi_vsync_o <= #1 dvsync;
	  end

`endif

endmodule

