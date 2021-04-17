/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE rev.B2 compliant enhanced VGA/LCD Core            ////
////  Wishbone master interface                                  ////
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
//  $Id: vga_wb_master.v,v 1.15 2003/08/01 11:46:38 rherveille Exp $
//
//  $Date: 2003/08/01 11:46:38 $
//  $Revision: 1.15 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: vga_wb_master.v,v $
//               Revision 1.15  2003/08/01 11:46:38  rherveille
//               1) Rewrote vga_fifo_dc. It now uses gray codes and a more elaborate anti-metastability scheme.
//               2) Changed top level and pixel generator to reflect changes in the fifo.
//               3) Changed a bug in vga_fifo.
//               4) Changed pixel generator and wishbone master to reflect changes.
//
//               Revision 1.14  2003/05/07 09:48:54  rherveille
//               Fixed some Wishbone RevB.3 related bugs.
//               Changed layout of the core. Blocks are located more logically now.
//               Started work on a dual clocked/double edge 12bit output. Commonly used by external devices like DVI transmitters.
//
//               Revision 1.13  2003/03/19 12:50:45  rherveille
//               Changed timing generator; made it smaller and easier.
//
//               Revision 1.12  2003/03/18 21:45:48  rherveille
//               Added WISHBONE revB.3 Registered Feedback Cycles support
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
//               Revision 1.7  2002/02/16 10:40:00  rherveille
//               Some minor bug-fixes.
//               Changed vga_ssel into vga_curproc (cursor processor).
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

module vga_wb_master (clk_i, rst_i, nrst_i,
	cyc_o, stb_o, cti_o, bte_o, we_o, adr_o, sel_o, ack_i, err_i, dat_i, sint,
	ctrl_ven, ctrl_cd, ctrl_vbl, ctrl_vbsw, busy,
	VBAa, VBAb, Thgate, Tvgate,
	stat_avmp, vmem_switch, ImDoneFifoQ,
	cursor_adr, cursor0_ba, cursor1_ba, cursor0_ld, cursor1_ld,
	fb_data_fifo_rreq, fb_data_fifo_q, fb_data_fifo_empty);

	// inputs & outputs

	// wishbone signals
	input         clk_i;    // master clock input
	input         rst_i;    // synchronous active high reset
	input         nrst_i;   // asynchronous low reset
	output        cyc_o;    // cycle output
	reg cyc_o;
	output        stb_o;    // strobe ouput
	reg stb_o;
	output [ 2:0] cti_o;    // cycle type id
	reg [2:0] cti_o;
	output [ 1:0] bte_o;    // burst type extension
	reg [1:0] bte_o;
	output        we_o;     // write enable output
	reg we_o;
	output [31:0] adr_o;    // address output
	output [ 3:0] sel_o;    // byte select outputs (only 32bits accesses are supported)
	reg [3:0] sel_o;
	input         ack_i;    // wishbone cycle acknowledge
	input         err_i;    // wishbone cycle error
	input [31:0]  dat_i;    // wishbone data in

	output        sint;     // non recoverable error, interrupt host

	// control register settings
	input       ctrl_ven;   // video enable bit
	input [1:0] ctrl_cd;    // color depth
	input [1:0] ctrl_vbl;   // burst length
	input       ctrl_vbsw;  // enable video bank switching
	output      busy;       // data transfer in progress

	// video memory addresses
	input [31: 2] VBAa;     // video memory base address A
	input [31: 2] VBAb;     // video memory base address B

	input [15:0] Thgate;    // horizontal visible area (in pixels)
	input [15:0] Tvgate;    // vertical visible area (in horizontal lines)

	output stat_avmp;       // active video memory page
	output vmem_switch;     // video memory bank-switch request: memory page switched (when enabled)
	output ImDoneFifoQ;

	output [ 8: 0] cursor_adr; // cursor address
	input  [31:11] cursor0_ba;
	input  [31:11] cursor1_ba;
	input          cursor0_ld; // load cursor0 (from wbs)
	input          cursor1_ld; // load cursor1 (from wbs)

	input          fb_data_fifo_rreq;
	output [31: 0] fb_data_fifo_q;
	output         fb_data_fifo_empty;


	//
	// variable declarations
	//

	reg vmem_acc;                 // video memory access
	wire vmem_req, vmem_ack;      // video memory access request // video memory access acknowledge

	wire ImDone;                  // Done reading image from video mem
	reg  dImDone;                 // delayed ImDone
	wire ImDoneStrb;              // image done (strobe signal)
	reg  dImDoneStrb;             // delayed ImDoneStrb

	reg sclr;                     // (video/cursor) synchronous clear

	// hardware cursors
	reg [31:11] cursor_ba;              // cursor pattern base address
	reg [ 8: 0] cursor_adr;             // cursor pattern offset
	wire        cursor0_we, cursor1_we; // cursor buffers write_request
	reg         ld_cursor0, ld_cursor1; // reload cursor0, cursor1
	reg         cur_acc;                // cursor processors request memory access
	reg         cur_acc_sel;            // which cursor to reload
	wire        cur_ack;                // cursor processor memory access acknowledge
	wire        cur_done;               // done reading cursor pattern

	//
	// module body
	//

	// generate synchronous clear
	always @(posedge clk_i)
	  sclr <= #1 ~ctrl_ven;

	//
	// WISHBONE block
	//
	reg  [ 2:0] burst_cnt;                       // video memory burst access counter
	wire        burst_done;                      // completed burst access to video mem
	reg         sel_VBA;                         // select video memory base address
	reg  [31:2] vmemA;                           // video memory address

	// wishbone access controller, video memory access request has highest priority (try to keep fifo full)
	always @(posedge clk_i)
	  if (sclr)
	    vmem_acc <= #1 1'b0; // video memory access request
	  else
	    vmem_acc <= #1 (vmem_req | (vmem_acc & !(burst_done & vmem_ack)) ) & !ImDone & !cur_acc;

	always @(posedge clk_i)
	  if (sclr)
	    cur_acc <= #1 1'b0; // cursor processor memory access request
	  else
	    cur_acc <= #1 (cur_acc | ImDone & (ld_cursor0 | ld_cursor1)) & !cur_done;
	    
	assign busy = vmem_acc | cur_acc;

	assign vmem_ack = ack_i & stb_o & vmem_acc;
	assign cur_ack  = ack_i & stb_o & cur_acc;
	assign sint = err_i; // Non recoverable error, interrupt host system


	// select active memory page
	assign vmem_switch = ImDoneStrb;

	always @(posedge clk_i)
	  if (sclr)
	    sel_VBA <= #1 1'b0;
	  else if (ctrl_vbsw)
	    sel_VBA <= #1 sel_VBA ^ vmem_switch;  // select next video memory bank when finished reading current bank (and bank switch enabled)

	assign stat_avmp = sel_VBA; // assign output

	// selecting active clut page / cursor data
	// delay image done same amount as video-memory data
	vga_fifo #(4, 1) clut_sw_fifo (
		.clk    ( clk_i             ),
		.aclr   ( 1'b1              ),
		.sclr   ( sclr              ),
		.d      ( ImDone            ),
		.wreq   ( vmem_ack          ),
		.q      ( ImDoneFifoQ       ),
		.rreq   ( fb_data_fifo_rreq ),
		.nword  ( ),
		.empty  ( ),
		.full   ( ),
		.aempty ( ),
		.afull  ( )
	);


	//
	// generate burst counter
	wire [3:0] burst_cnt_val;
	assign burst_cnt_val = {1'b0, burst_cnt} -4'h1;
	assign burst_done = burst_cnt_val[3];

	always @(posedge clk_i)
	  if ( (burst_done & vmem_ack) | !vmem_acc)
	    case (ctrl_vbl) // synopsis full_case parallel_case
	      2'b00: burst_cnt <= #1 3'b000; // burst length 1
	      2'b01: burst_cnt <= #1 3'b001; // burst length 2
	      2'b10: burst_cnt <= #1 3'b011; // burst length 4
	      2'b11: burst_cnt <= #1 3'b111; // burst length 8
	    endcase
	  else if(vmem_ack)
	    burst_cnt <= #1 burst_cnt_val[2:0];

	//
	// generate image counters
	//

	// hgate counter
	reg  [15:0] hgate_cnt;
	reg  [16:0] hgate_cnt_val;
	reg  [1:0]  hgate_div_cnt;
	reg  [2:0]  hgate_div_val;

	wire hdone = hgate_cnt_val[16] & vmem_ack; // ????

	always @(hgate_cnt or hgate_div_cnt or ctrl_cd)
	  begin
	      hgate_div_val = {1'b0, hgate_div_cnt} - 3'h1;

	      if (ctrl_cd != 2'b10)
	        hgate_cnt_val = {1'b0, hgate_cnt} - 17'h1;
	      else if ( hgate_div_val[2] )
	        hgate_cnt_val = {1'b0, hgate_cnt} - 17'h1;
	      else
	        hgate_cnt_val = {1'b0, hgate_cnt};
	  end

	always @(posedge clk_i)
	  if (sclr)
	    begin
	        case(ctrl_cd) // synopsys full_case parallel_case
	          2'b00: hgate_cnt <= #1 Thgate >> 2; //  8bpp, 4 pixels per cycle
	          2'b01: hgate_cnt <= #1 Thgate >> 1; // 16bpp, 2 pixels per cycle
	          2'b10: hgate_cnt <= #1 Thgate >> 2; // 24bpp, 4/3 pixels per cycle
	          2'b11: hgate_cnt <= #1 Thgate;      // 32bpp, 1 pixel per cycle
	        endcase

	        hgate_div_cnt <= 2'b10;
	    end
	  else if (vmem_ack)
	    if (hdone)
	      begin
	          case(ctrl_cd) // synopsys full_case parallel_case
	            2'b00: hgate_cnt <= #1 Thgate >> 2; //  8bpp, 4 pixels per cycle
	            2'b01: hgate_cnt <= #1 Thgate >> 1; // 16bpp, 2 pixels per cycle
	            2'b10: hgate_cnt <= #1 Thgate >> 2; // 24bpp, 4/3 pixels per cycle
	            2'b11: hgate_cnt <= #1 Thgate;      // 32bpp, 1 pixel per cycle
	          endcase

	          hgate_div_cnt <= 2'b10;
	      end
	    else //if (vmem_ack)
	      begin
	          hgate_cnt <= #1 hgate_cnt_val[15:0];

	          if ( hgate_div_val[2] )
	            hgate_div_cnt <= #1 2'b10;
	          else
	            hgate_div_cnt <= #1 hgate_div_val[1:0];
	      end

	// vgate counter
	reg  [15:0] vgate_cnt;
	wire [16:0] vgate_cnt_val;
	wire        vdone;

	assign vgate_cnt_val = {1'b0, vgate_cnt} - 17'h1;
	assign vdone = vgate_cnt_val[16];

	always @(posedge clk_i)
	  if (sclr | ImDoneStrb)
	    vgate_cnt <= #1 Tvgate;
	  else if (hdone)
	    vgate_cnt <= #1 vgate_cnt_val[15:0];

	assign ImDone = hdone & vdone;

	assign ImDoneStrb = ImDone & !dImDone;

	always @(posedge clk_i)
	  begin
	      dImDone <= #1 ImDone;
	      dImDoneStrb <= #1 ImDoneStrb;
	  end

	//
	// generate addresses
	//

	// select video memory base address
	always @(posedge clk_i)
	  if (sclr | dImDoneStrb)
	    if (!sel_VBA)
	      vmemA <= #1 VBAa;
	    else
	      vmemA <= #1 VBAb;
	  else if (vmem_ack)
	    vmemA <= #1 vmemA +30'h1;


	////////////////////////////////////
	// hardware cursor signals section
	//
	always @(posedge clk_i)
	  if (ImDone)
	    cur_acc_sel <= #1 ld_cursor0; // cursor0 has highest priority

	always @(posedge clk_i)
	if (sclr)
	  begin
	      ld_cursor0 <= #1 1'b0;
	      ld_cursor1 <= #1 1'b0;
	  end
	else
	  begin
	      ld_cursor0 <= #1 cursor0_ld | (ld_cursor0 & !(cur_done &  cur_acc_sel));
	      ld_cursor1 <= #1 cursor1_ld | (ld_cursor1 & !(cur_done & !cur_acc_sel));
	  end

	// select cursor base address
	always @(posedge clk_i)
	  if (!cur_acc)
	    cursor_ba <= #1 ld_cursor0 ? cursor0_ba : cursor1_ba;

	// generate pattern offset
	wire [9:0] next_cursor_adr = {1'b0, cursor_adr} + 10'h1;
	assign cur_done = next_cursor_adr[9] & cur_ack;

	always @(posedge clk_i)
	  if (!cur_acc)
	    cursor_adr <= #1 9'h0;
	  else if (cur_ack)
	    cursor_adr <= #1 next_cursor_adr;

	// generate cursor buffers write enable signals
	assign cursor1_we = cur_ack & !cur_acc_sel;
	assign cursor0_we = cur_ack &  cur_acc_sel;


	//////////////////////////////
	// generate wishbone signals
	//
	assign adr_o = cur_acc ? {cursor_ba, cursor_adr, 2'b00} : {vmemA, 2'b00};
	wire wb_cycle = vmem_acc & !(burst_done & vmem_ack & !vmem_req) & !ImDone ||
	                cur_acc & !cur_done;

	always @(posedge clk_i or negedge nrst_i)
	  if (!nrst_i)
	    begin
	        cyc_o <= #1 1'b0;
	        stb_o <= #1 1'b0;
	        sel_o <= #1 4'b1111;
	        cti_o <= #1 3'b000;
	        bte_o <= #1 2'b00;
	        we_o  <= #1 1'b0;
	    end
	  else
	    if (rst_i)
	      begin
	          cyc_o <= #1 1'b0;
	          stb_o <= #1 1'b0;
	          sel_o <= #1 4'b1111;
	          cti_o <= #1 3'b000;
	          bte_o <= #1 2'b00;
	          we_o  <= #1 1'b0;
	      end
	    else
	      begin
	          cyc_o <= #1 wb_cycle;
	          stb_o <= #1 wb_cycle;
	          sel_o <= #1 4'b1111;   // only 32bit accesses are supported

	          if (wb_cycle) begin
	            if (cur_acc)
	              cti_o <= #1 &next_cursor_adr[8:0] ? 3'b111 : 3'b010;
	            else if (ctrl_vbl == 2'b00)
	              cti_o <= #1 3'b000;
	            else if (vmem_ack)
	              cti_o <= #1 (burst_cnt == 3'h1) ? 3'b111 : 3'b010;
	          end else
	            cti_o <= #1 (ctrl_vbl == 2'b00) ? 3'b000 : 3'b010;

	          bte_o <= #1 2'b00;     // linear burst
	          we_o  <= #1 1'b0;      // read only
	      end

	//
	// video-data buffer (temporary store data read from video memory)
	wire [4:0] fb_data_fifo_nword;
//	wire       fb_data_fifo_full;

	vga_fifo #(4, 32) data_fifo (
		.clk    ( clk_i              ),
		.aclr   ( 1'b1               ),
		.sclr   ( sclr               ),
		.d      ( dat_i              ),
		.wreq   ( vmem_ack           ),
		.q      ( fb_data_fifo_q     ),
		.rreq   ( fb_data_fifo_rreq  ),
		.nword  ( fb_data_fifo_nword ),
		.empty  ( fb_data_fifo_empty ),
		.full   ( ),//fb_data_fifo_full  ),
		.aempty ( ),
		.afull  ( )
	);

	assign vmem_req = ~fb_data_fifo_nword[4] & ~fb_data_fifo_nword[3];

endmodule
