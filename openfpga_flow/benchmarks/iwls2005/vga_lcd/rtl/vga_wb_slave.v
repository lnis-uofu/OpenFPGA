/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE rev.B2 compliant enhanced VGA/LCD Core            ////
////  Wishbone slave interface                                   ////
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
//  $Id: vga_wb_slave.v,v 1.12 2003/05/07 09:48:54 rherveille Exp $
//
//  $Date: 2003/05/07 09:48:54 $
//  $Revision: 1.12 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: vga_wb_slave.v,v $
//               Revision 1.12  2003/05/07 09:48:54  rherveille
//               Fixed some Wishbone RevB.3 related bugs.
//               Changed layout of the core. Blocks are located more logically now.
//               Started work on a dual clocked/double edge 12bit output. Commonly used by external devices like DVI transmitters.
//
//               Revision 1.11  2002/04/20 10:02:39  rherveille
//               Changed video timing generator.
//               Changed wishbone master vertical gate count code.
//               Fixed a potential bug in the wishbone slave (cursor color register readout).
//
//               Revision 1.10  2002/03/28 04:59:25  rherveille
//               Fixed two small bugs that only showed up when the hardware cursors were disabled
//
//               Revision 1.9  2002/03/04 16:05:52  rherveille
//               Added hardware cursor support to wishbone master.
//               Added provision to turn-off 3D cursors.
//               Fixed some minor bugs.
//
//               Revision 1.8  2002/03/04 11:01:59  rherveille
//               Added 64x64pixels 4bpp hardware cursor support.
//
//               Revision 1.7  2002/02/25 06:13:44  rherveille
//               Fixed dat_o incomplete sensitivity list.
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
`include "vga_defines.v"

module vga_wb_slave(
	clk_i, rst_i, arst_i, adr_i, dat_i, dat_o, sel_i, we_i, stb_i, cyc_i, ack_o, rty_o, err_o, inta_o,
	wbm_busy, dvi_odf, bl, csl, vsl, hsl, pc, cd, vbl, cbsw, vbsw, ven, avmp, acmp,
	cursor0_res, cursor0_en, cursor0_xy, cursor0_ba, cursor0_ld, cc0_adr_i, cc0_dat_o,
	cursor1_res, cursor1_en, cursor1_xy, cursor1_ba, cursor1_ld, cc1_adr_i, cc1_dat_o,
	vbsint_in, cbsint_in, hint_in, vint_in, luint_in, sint_in,
	Thsync, Thgdel, Thgate, Thlen, Tvsync, Tvgdel, Tvgate, Tvlen, VBARa, VBARb,
	clut_acc, clut_ack, clut_q
	);

	//
	// inputs & outputs
	//

	// wishbone slave interface
	input         clk_i;
	input         rst_i;
	input         arst_i;
	input  [11:2] adr_i;
	input  [31:0] dat_i;
	output [31:0] dat_o;
	reg [31:0] dat_o;
	input  [ 3:0] sel_i;
	input         we_i;
	input         stb_i;
	input         cyc_i;
	output        ack_o;
	reg ack_o;
	output        rty_o;
	reg rty_o;
	output        err_o;
	reg err_o;
	output        inta_o;
	reg inta_o;

	// wishbone master controller feedback
	input  wbm_busy;             // data transfer in progress

	// control register settings
	output [1:0] dvi_odf;        // DVI output data format
	output bl;                   // blanking level
	output csl;                  // composite sync level
	output vsl;                  // vsync level
	output hsl;                  // hsync level
	output pc;                   // pseudo color
	output [1:0] cd;             // color depth
	output [1:0] vbl;            // video memory burst length
	output cbsw;                 // clut bank switch enable
	output vbsw;                 // video memory bank switch enable
	output ven;                  // video system enable

	// hardware cursor settings
	output         cursor0_res;  // cursor0 resolution
	output         cursor0_en;   // cursor0 enable
	output [31: 0] cursor0_xy;   // cursor0 location
	output [31:11] cursor0_ba;   // cursor0 base address
	output         cursor0_ld;   // reload cursor0 from video memory
	input  [ 3: 0] cc0_adr_i;    // cursor0 color register address
	output [15: 0] cc0_dat_o;    // cursor0 color register data
	output         cursor1_res;  // cursor1 resolution
	output         cursor1_en;   // cursor1 enable
	output [31: 0] cursor1_xy;   // cursor1 location
	output [31:11] cursor1_ba;   // cursor1 base address
	output         cursor1_ld;   // reload cursor1 from video memory
	input  [ 3: 0] cc1_adr_i;    // cursor1 color register address
	output [15: 0] cc1_dat_o;    // cursor1 color register data

	reg [31: 0] cursor0_xy;
	reg [31:11] cursor0_ba;
	reg         cursor0_ld;
	reg [31: 0] cursor1_xy;
	reg [31:11] cursor1_ba;
	reg         cursor1_ld;

	// status register inputs
	input avmp;          // active video memory page
	input acmp;          // active clut memory page
	input vbsint_in;     // bank switch interrupt request
	input cbsint_in;     // clut switch interrupt request
	input hint_in;       // hsync interrupt request
	input vint_in;       // vsync interrupt request
	input luint_in;      // line fifo underrun interrupt request
	input sint_in;       // system error interrupt request

	// Horizontal Timing Register
	output [ 7:0] Thsync;
	output [ 7:0] Thgdel;
	output [15:0] Thgate;
	output [15:0] Thlen;

	// Vertical Timing Register
	output [ 7:0] Tvsync;
	output [ 7:0] Tvgdel;
	output [15:0] Tvgate;
	output [15:0] Tvlen;

	// video base addresses
	output [31:2] VBARa;
	reg [31:2] VBARa;
	output [31:2] VBARb;
	reg [31:2] VBARb;

	// color lookup table signals
	output        clut_acc;
	input         clut_ack;
	input  [23:0] clut_q;


	//
	// variable declarations
	//
	parameter REG_ADR_HIBIT = 7;

	wire [REG_ADR_HIBIT:0] REG_ADR  = adr_i[REG_ADR_HIBIT : 2];
	wire                   CLUT_ADR = adr_i[11];

	parameter [REG_ADR_HIBIT : 0] CTRL_ADR  = 6'b00_0000;
	parameter [REG_ADR_HIBIT : 0] STAT_ADR  = 6'b00_0001;
	parameter [REG_ADR_HIBIT : 0] HTIM_ADR  = 6'b00_0010;
	parameter [REG_ADR_HIBIT : 0] VTIM_ADR  = 6'b00_0011;
	parameter [REG_ADR_HIBIT : 0] HVLEN_ADR = 6'b00_0100;
	parameter [REG_ADR_HIBIT : 0] VBARA_ADR = 6'b00_0101;
	parameter [REG_ADR_HIBIT : 0] VBARB_ADR = 6'b00_0110;
	parameter [REG_ADR_HIBIT : 0] C0XY_ADR  = 6'b00_1100;
	parameter [REG_ADR_HIBIT : 0] C0BAR_ADR = 6'b00_1101;
	parameter [REG_ADR_HIBIT : 0] CCR0_ADR  = 6'b01_0???;
	parameter [REG_ADR_HIBIT : 0] C1XY_ADR  = 6'b01_1100;
	parameter [REG_ADR_HIBIT : 0] C1BAR_ADR = 6'b01_1101;
	parameter [REG_ADR_HIBIT : 0] CCR1_ADR  = 6'b10_0???;


	reg [31:0] ctrl, stat, htim, vtim, hvlen;
	wire hint, vint, vbsint, cbsint, luint, sint;
	wire hie, vie, vbsie, cbsie;
	wire acc, acc32, reg_acc, reg_wacc;
	wire cc0_acc, cc1_acc;
	wire [31:0] ccr0_dat_o, ccr1_dat_o;


	reg [31:0] reg_dato; // data output from registers

	//
	// Module body
	//

	assign acc      =  cyc_i & stb_i;
	assign acc32    = (sel_i == 4'b1111);
	assign clut_acc =  CLUT_ADR & acc & acc32;
	assign reg_acc  = ~CLUT_ADR & acc & acc32;
	assign reg_wacc =  reg_acc & we_i;

	assign cc0_acc  = (REG_ADR == CCR0_ADR) & acc & acc32;
	assign cc1_acc  = (REG_ADR == CCR1_ADR) & acc & acc32;

	always @(posedge clk_i)
	  ack_o <= #1 ((reg_acc & acc32) | clut_ack) & ~(wbm_busy & REG_ADR == CTRL_ADR) & ~ack_o ;

	always @(posedge clk_i)
	  rty_o <= #1 ((reg_acc & acc32) | clut_ack) & (wbm_busy & REG_ADR == CTRL_ADR) & ~rty_o ;

	always @(posedge clk_i)
	  err_o <= #1 acc & ~acc32 & ~err_o;


	// generate registers
	always @(posedge clk_i or negedge arst_i)
	begin : gen_regs
	  if (!arst_i)
	    begin
	        htim       <= #1 0;
	        vtim       <= #1 0;
	        hvlen      <= #1 0;
	        VBARa      <= #1 0;
	        VBARb      <= #1 0;
	        cursor0_xy <= #1 0;
	        cursor0_ba <= #1 0;
	        cursor1_xy <= #1 0;
	        cursor1_ba <= #1 0;
	    end
	  else if (rst_i)
	    begin
	        htim       <= #1 0;
	        vtim       <= #1 0;
	        hvlen      <= #1 0;
	        VBARa      <= #1 0;
	        VBARb      <= #1 0;
	        cursor0_xy <= #1 0;
	        cursor0_ba <= #1 0;
	        cursor1_xy <= #1 0;
	        cursor1_ba <= #1 0;
	    end
	  else if (reg_wacc)
	    case (adr_i) // synopsis full_case parallel_case
	        HTIM_ADR  : htim       <= #1 dat_i;
	        VTIM_ADR  : vtim       <= #1 dat_i;
	        HVLEN_ADR : hvlen      <= #1 dat_i;
	        VBARA_ADR : VBARa      <= #1 dat_i[31: 2];
	        VBARB_ADR : VBARb      <= #1 dat_i[31: 2];
	        C0XY_ADR  : cursor0_xy <= #1 dat_i[31: 0];
	        C0BAR_ADR : cursor0_ba <= #1 dat_i[31:11];
	        C1XY_ADR  : cursor1_xy <= #1 dat_i[31: 0];
	        C1BAR_ADR : cursor1_ba <= #1 dat_i[31:11];
	    endcase
	end

	always @(posedge clk_i)
	  begin
	     cursor0_ld <= #1 reg_wacc && (adr_i == C0BAR_ADR);
	     cursor1_ld <= #1 reg_wacc && (adr_i == C1BAR_ADR);
	  end

	// generate control register
	always @(posedge clk_i or negedge arst_i)
	  if (!arst_i)
	    ctrl <= #1 0;
	  else if (rst_i)
	    ctrl <= #1 0;
	  else if (reg_wacc & (REG_ADR == CTRL_ADR) & ~wbm_busy )
	    ctrl <= #1 dat_i;
	  else begin
	    ctrl[6] <= #1 ctrl[6] & !cbsint_in;
	    ctrl[5] <= #1 ctrl[5] & !vbsint_in;
	  end


	// generate status register
	always @(posedge clk_i or negedge arst_i)
	  if (!arst_i)
	    stat <= #1 0;
	  else if (rst_i)
	    stat <= #1 0;
	  else begin
	    `ifdef VGA_HWC1
	        stat[21] <= #1 1'b1;
	    `else
	        stat[21] <= #1 1'b0;
	    `endif
	    `ifdef VGA_HWC0
	        stat[20] <= #1 1'b1;
	    `else
	        stat[20] <= #1 1'b0;
	    `endif

	    stat[17] <= #1 acmp;
	    stat[16] <= #1 avmp;

	    if (reg_wacc & (REG_ADR == STAT_ADR) )
	      begin
	          stat[7] <= #1 cbsint_in | (stat[7] & !dat_i[7]);
	          stat[6] <= #1 vbsint_in | (stat[6] & !dat_i[6]);
	          stat[5] <= #1 hint_in   | (stat[5] & !dat_i[5]);
	          stat[4] <= #1 vint_in   | (stat[4] & !dat_i[4]);
	          stat[1] <= #1 luint_in  | (stat[3] & !dat_i[1]);
	          stat[0] <= #1 sint_in   | (stat[0] & !dat_i[0]);
	      end
	    else
	      begin
	          stat[7] <= #1 stat[7] | cbsint_in;
	          stat[6] <= #1 stat[6] | vbsint_in;
	          stat[5] <= #1 stat[5] | hint_in;
	          stat[4] <= #1 stat[4] | vint_in;
	          stat[1] <= #1 stat[1] | luint_in;
	          stat[0] <= #1 stat[0] | sint_in;
	      end
	  end


	// decode control register
	assign dvi_odf     = ctrl[29:28];
	assign cursor1_res = ctrl[25];
	assign cursor1_en  = ctrl[24];
	assign cursor0_res = ctrl[23];
	assign cursor0_en  = ctrl[20];
	assign bl          = ctrl[15];
	assign csl         = ctrl[14];
	assign vsl         = ctrl[13];
	assign hsl         = ctrl[12];
	assign pc          = ctrl[11];
	assign cd          = ctrl[10:9];
	assign vbl         = ctrl[8:7];
	assign cbsw        = ctrl[6];
	assign vbsw        = ctrl[5];
	assign cbsie       = ctrl[4];
	assign vbsie       = ctrl[3];
	assign hie         = ctrl[2];
	assign vie         = ctrl[1];
	assign ven         = ctrl[0];

	// decode status register
	assign cbsint = stat[7];
	assign vbsint = stat[6];
	assign hint   = stat[5];
	assign vint   = stat[4];
	assign luint  = stat[1];
	assign sint   = stat[0];

	// decode Horizontal Timing Register
	assign Thsync = htim[31:24];
	assign Thgdel = htim[23:16];
	assign Thgate = htim[15:0];
	assign Thlen  = hvlen[31:16];

	// decode Vertical Timing Register
	assign Tvsync = vtim[31:24];
	assign Tvgdel = vtim[23:16];
	assign Tvgate = vtim[15:0];
	assign Tvlen  = hvlen[15:0];


	`ifdef VGA_HWC0
		// hookup cursor0 color registers
		vga_cur_cregs cregs0(
			.clk_i(clk_i),
			.rst_i(rst_i),
			.arst_i(arst_i),
			.hsel_i(cc0_acc),
			.hadr_i(adr_i[4:2]),
			.hwe_i(we_i),
			.hdat_i(dat_i),
			.hdat_o(ccr0_dat_o), // host access
			.hack_o(),
			.cadr_i(cc0_adr_i),
			.cdat_o(cc0_dat_o)   // cursor processor access
		);
	`else
		assign ccr0_dat_o = 32'h0;
		assign cc0_dat_o = 32'h0;
	`endif

	`ifdef VGA_HWC1
		// hookup cursor1 color registers
		vga_cur_cregs cregs1(
			.clk_i(clk_i),
			.rst_i(rst_i),
			.arst_i(arst_i),
			.hsel_i(cc1_acc),
			.hadr_i(adr_i[4:2]),
			.hwe_i(we_i),
			.hdat_i(dat_i),
			.hdat_o(ccr1_dat_o), // host access
			.hack_o(),
			.cadr_i(cc1_adr_i),
			.cdat_o(cc1_dat_o)   // cursor processor access
		);
	`else
		assign ccr1_dat_o = 32'h0;
		assign cc1_dat_o = 32'h0;
	`endif


	// assign output
	always @(REG_ADR or ctrl or stat or htim or vtim or hvlen or VBARa or VBARb or acmp or
		cursor0_xy or cursor0_ba or cursor1_xy or cursor1_ba or ccr0_dat_o or ccr1_dat_o)
	  casez (REG_ADR) // synopsis full_case parallel_case
	      CTRL_ADR  : reg_dato = ctrl;
	      STAT_ADR  : reg_dato = stat;
	      HTIM_ADR  : reg_dato = htim;
	      VTIM_ADR  : reg_dato = vtim;
	      HVLEN_ADR : reg_dato = hvlen;
	      VBARA_ADR : reg_dato = {VBARa, 2'b0};
	      VBARB_ADR : reg_dato = {VBARb, 2'b0};
	      C0XY_ADR  : reg_dato = cursor0_xy;
	      C0BAR_ADR : reg_dato = {cursor0_ba, 11'h0};
	      CCR0_ADR  : reg_dato = ccr0_dat_o;
	      C1XY_ADR  : reg_dato = cursor1_xy;
	      C1BAR_ADR : reg_dato = {cursor1_ba, 11'h0};
	      CCR1_ADR  : reg_dato = ccr1_dat_o;
	      default   : reg_dato = 32'h0000_0000;
	  endcase

	always @(posedge clk_i)
	  dat_o <= #1 reg_acc ? reg_dato : {8'h0, clut_q};

	// generate interrupt request signal
	always @(posedge clk_i)
	  inta_o <= #1 (hint & hie) | (vint & vie) | (vbsint & vbsie) | (cbsint & cbsie) | luint | sint;
endmodule



