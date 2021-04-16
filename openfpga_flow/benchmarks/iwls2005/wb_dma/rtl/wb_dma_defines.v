/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE DMA Definitions                                   ////
////                                                             ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/wb_dma/    ////
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
//  $Id: wb_dma_defines.v,v 1.5 2002/02/01 01:54:45 rudi Exp $
//
//  $Date: 2002/02/01 01:54:45 $
//  $Revision: 1.5 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: wb_dma_defines.v,v $
//               Revision 1.5  2002/02/01 01:54:45  rudi
//
//               - Minor cleanup
//
//               Revision 1.4  2001/10/19 04:35:04  rudi
//
//               - Made the core parameterized
//
//               Revision 1.3  2001/09/07 15:34:38  rudi
//
//               Changed reset to active high.
//
//               Revision 1.2  2001/08/15 05:40:30  rudi
//
//               - Changed IO names to be more clear.
//               - Uniquifyed define names to be core specific.
//               - Added Section 3.10, describing DMA restart.
//
//               Revision 1.1  2001/07/29 08:57:02  rudi
//
//
//               1) Changed Directory Structure
//               2) Added restart signal (REST)
//
//               Revision 1.2  2001/06/05 10:22:37  rudi
//
//
//               - Added Support of up to 31 channels
//               - Added support for 2,4 and 8 priority levels
//               - Now can have up to 31 channels
//               - Added many configuration items
//               - Changed reset to async
//
//               Revision 1.1.1.1  2001/03/19 13:11:09  rudi
//               Initial Release
//
//
//

`timescale 1ns / 10ps

// This define selects how the slave interface determines if
// the internal register file or pass through mode are selected.
// This should be a simple address decoder. "wb_addr_i" is the
// WISHBONE address bus (32 bits wide).
// NOTE: The entire pass-through mode is implemented in combinatorial
// logic only. So the more address lines we look at and compare here
// the higher will be the initial delay when pass-through mode is selected.
// Here we look at the top 8 address bit. If they are all 1, the
// register file is selected. Use this with caution !!!
`define	WDMA_REG_SEL		(wb_addr_i[31:28] == rf_addr)


// DO NOT MODIFY BEYOND THIS POINT
// CSR Bits
`define	WDMA_CH_EN		0
`define	WDMA_DST_SEL		1
`define	WDMA_SRC_SEL		2
`define	WDMA_INC_DST		3
`define	WDMA_INC_SRC		4
`define	WDMA_MODE		5
`define	WDMA_ARS		6
`define WDMA_USE_ED		7
`define WDMA_WRB		8
`define	WDMA_STOP		9
`define	WDMA_BUSY		10
`define	WDMA_DONE		11
`define	WDMA_ERR		12
`define WDMA_ED_EOL		20

