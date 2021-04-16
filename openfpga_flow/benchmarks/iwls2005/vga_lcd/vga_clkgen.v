/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE rev.B2 compliant VGA/LCD Core; Clock Generator    ////
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
//// Copyright (C) 2003 Richard Herveille                        ////
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

/////////////////////////////////////////////////////////////////////
//                                                                 //
// !! SPECIAL LOGIC, USE PRECAUTION DURING SYNTHESIS AND LAYOUT !! //
//                                                                 //
// This is a clock generation circuit. Although all output clocks  //
// are generated synchronous to the input clock, special care must //
// be taken during synthesis and physical layout.                  //
//                                                                 //
/////////////////////////////////////////////////////////////////////



//  CVS Log
//
//  $Id: vga_clkgen.v,v 1.1 2003/05/07 14:43:01 rherveille Exp $
//
//  $Date: 2003/05/07 14:43:01 $
//  $Revision: 1.1 $
//  $Author: rherveille $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: vga_clkgen.v,v $
//               Revision 1.1  2003/05/07 14:43:01  rherveille
//               Initial release.
//
//

//synopsys translate_off
`include "timescale.v"
//synopsys translate_on

`include "vga_defines.v"

module vga_clkgen (
	pclk_i, rst_i, pclk_o, dvi_pclk_p_o, dvi_pclk_m_o, pclk_ena_o
);

	// inputs & outputs

	input  pclk_i;       // pixel clock in
	input  rst_i;        // reset input

	output pclk_o;       // pixel clock out

	output dvi_pclk_p_o; // dvi cpclk+ output
	output dvi_pclk_m_o; // dvi cpclk- output

	output pclk_ena_o;   // pixel clock enable output

	//
	// variable declarations
	//
	reg dvi_pclk_p_o;
	reg dvi_pclk_m_o;

	//////////////////////////////////
	//
	// module body
	//

	// These should be registers in or near IO buffers
	always @(posedge pclk_i)
	  if (rst_i) begin
	    dvi_pclk_p_o <= #1 1'b0;
	    dvi_pclk_m_o <= #1 1'b0;
	  end else begin
	    dvi_pclk_p_o <= #1 ~dvi_pclk_p_o;
	    dvi_pclk_m_o <= #1 dvi_pclk_p_o;
	  end


`ifdef VGA_12BIT_DVI
	// DVI circuit
	// pixel clock is half of the input pixel clock

	reg pclk_o, pclk_ena_o;

	always @(posedge pclk_i)
	  if (rst_i)
	    pclk_o <= #1 1'b0;
	  else
	    pclk_o <= #1 ~pclk_o;

	always @(posedge pclk_i)
	  if (rst_i)
	    pclk_ena_o <= #1 1'b1;
	  else
	    pclk_ena_o <= #1 ~pclk_ena_o;

`else
	// No DVI circuit
	// Simply reroute the pixel clock input

	assign pclk_o     = pclk_i;
	assign pclk_ena_o = 1'b1;
`endif

endmodule

