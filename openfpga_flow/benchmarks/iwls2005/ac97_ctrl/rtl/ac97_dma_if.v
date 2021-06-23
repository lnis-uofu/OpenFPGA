/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller                                  ////
////  DMA Interface                                              ////
////                                                             ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/ac97_ctrl/ ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2000-2002 Rudolf Usselmann                    ////
////                         www.asics.ws                        ////
////                         rudi@asics.ws                       ////
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
//  $Id: ac97_dma_if.v,v 1.4 2002/09/19 06:30:56 rudi Exp $
//
//  $Date: 2002/09/19 06:30:56 $
//  $Revision: 1.4 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_dma_if.v,v $
//               Revision 1.4  2002/09/19 06:30:56  rudi
//               Fixed a bug reported by Igor. Apparently this bug only shows up when
//               the WB clock is very low (2x bit_clk). Updated Copyright header.
//
//               Revision 1.3  2002/03/05 04:44:05  rudi
//
//               - Fixed the order of the thrash hold bits to match the spec.
//               - Many minor synthesis cleanup items ...
//
//               Revision 1.2  2001/08/10 08:09:42  rudi
//
//               - Removed RTY_O output.
//               - Added Clock and Reset Inputs to documentation.
//               - Changed IO names to be more clear.
//               - Uniquifyed define names to be core specific.
//
//               Revision 1.1  2001/08/03 06:54:49  rudi
//
//
//               - Changed to new directory structure
//
//               Revision 1.1.1.1  2001/05/19 02:29:18  rudi
//               Initial Checkin
//
//
//
//

`include "ac97_defines.v"

module ac97_dma_if(clk, rst,
		o3_status, o4_status, o6_status, o7_status, o8_status, o9_status,
		o3_empty, o4_empty, o6_empty, o7_empty, o8_empty, o9_empty,
		i3_status, i4_status, i6_status,
		i3_full, i4_full, i6_full,

		oc0_cfg, oc1_cfg, oc2_cfg, oc3_cfg, oc4_cfg, oc5_cfg,
		ic0_cfg, ic1_cfg, ic2_cfg,

		dma_req, dma_ack);

input		clk, rst;
input	[1:0]	o3_status, o4_status, o6_status, o7_status, o8_status, o9_status;
input		o3_empty, o4_empty, o6_empty, o7_empty, o8_empty, o9_empty;
input	[1:0]	i3_status, i4_status, i6_status;
input		i3_full, i4_full, i6_full;
input	[7:0]	oc0_cfg;
input	[7:0]	oc1_cfg;
input	[7:0]	oc2_cfg;
input	[7:0]	oc3_cfg;
input	[7:0]	oc4_cfg;
input	[7:0]	oc5_cfg;
input	[7:0]	ic0_cfg;
input	[7:0]	ic1_cfg;
input	[7:0]	ic2_cfg;
output	[8:0]	dma_req;
input	[8:0]	dma_ack;

////////////////////////////////////////////////////////////////////
//
// DMA Request Modules
//

ac97_dma_req u0(.clk(		clk		),
		.rst(		rst		),
		.cfg(		oc0_cfg		),
		.status(	o3_status	),
		.full_empty(	o3_empty	),
		.dma_req(	dma_req[0]	),
		.dma_ack(	dma_ack[0]	)
		);

ac97_dma_req u1(.clk(		clk		),
		.rst(		rst		),
		.cfg(		oc1_cfg		),
		.status(	o4_status	),
		.full_empty(	o4_empty	),
		.dma_req(	dma_req[1]	),
		.dma_ack(	dma_ack[1]	)
		);

`ifdef AC97_CENTER
ac97_dma_req u2(.clk(		clk		),
		.rst(		rst		),
		.cfg(		oc2_cfg		),
		.status(	o6_status	),
		.full_empty(	o6_empty	),
		.dma_req(	dma_req[2]	),
		.dma_ack(	dma_ack[2]	)
		);
`else
assign dma_req[2] = 1'b0;
`endif

`ifdef AC97_SURROUND
ac97_dma_req u3(.clk(		clk		),
		.rst(		rst		),
		.cfg(		oc3_cfg		),
		.status(	o7_status	),
		.full_empty(	o7_empty	),
		.dma_req(	dma_req[3]	),
		.dma_ack(	dma_ack[3]	)
		);

ac97_dma_req u4(.clk(		clk		),
		.rst(		rst		),
		.cfg(		oc4_cfg		),
		.status(	o8_status	),
		.full_empty(	o8_empty	),
		.dma_req(	dma_req[4]	),
		.dma_ack(	dma_ack[4]	)
		);
`else
assign dma_req[3] = 1'b0;
assign dma_req[4] = 1'b0;
`endif

`ifdef AC97_LFE
ac97_dma_req u5(.clk(		clk		),
		.rst(		rst		),
		.cfg(		oc5_cfg		),
		.status(	o9_status	),
		.full_empty(	o9_empty	),
		.dma_req(	dma_req[5]	),
		.dma_ack(	dma_ack[5]	)
		);
`else
assign dma_req[5] = 1'b0;
`endif

`ifdef AC97_SIN
ac97_dma_req u6(.clk(		clk		),
		.rst(		rst		),
		.cfg(		ic0_cfg		),
		.status(	i3_status	),
		.full_empty(	i3_full		),
		.dma_req(	dma_req[6]	),
		.dma_ack(	dma_ack[6]	)
		);

ac97_dma_req u7(.clk(		clk		),
		.rst(		rst		),
		.cfg(		ic1_cfg		),
		.status(	i4_status	),
		.full_empty(	i4_full		),
		.dma_req(	dma_req[7]	),
		.dma_ack(	dma_ack[7]	)
		);
`else
assign dma_req[6] = 1'b0;
assign dma_req[7] = 1'b0;
`endif

`ifdef AC97_MICIN
ac97_dma_req u8(.clk(		clk		),
		.rst(		rst		),
		.cfg(		ic2_cfg		),
		.status(	i6_status	),
		.full_empty(	i6_full		),
		.dma_req(	dma_req[8]	),
		.dma_ack(	dma_ack[8]	)
		);
`else
assign dma_req[8] = 1'b0;
`endif

endmodule


