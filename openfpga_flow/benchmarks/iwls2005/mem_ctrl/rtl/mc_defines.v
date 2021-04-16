/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE Memory Controller Definitions                     ////
////                                                             ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/mem_ctrl/  ////
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
//  $Id: mc_defines.v,v 1.7 2002/01/21 13:08:52 rudi Exp $
//
//  $Date: 2002/01/21 13:08:52 $
//  $Revision: 1.7 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: mc_defines.v,v $
//               Revision 1.7  2002/01/21 13:08:52  rudi
//
//               Fixed several minor bugs, cleaned up the code further ...
//
//               Revision 1.6  2001/12/12 06:35:15  rudi
//               *** empty log message ***
//
//               Revision 1.5  2001/12/11 02:47:19  rudi
//
//               - Made some changes not to expect clock during reset ...
//
//               Revision 1.4  2001/11/29 02:16:28  rudi
//
//
//               - More Synthesis cleanup, mostly for speed
//               - Several bug fixes
//               - Changed code to avoid auto-precharge and
//                 burst-terminate combinations (apparently illegal ?)
//                 Now we will do a manual precharge ...
//
//               Revision 1.3  2001/09/10 13:44:17  rudi
//               *** empty log message ***
//
//               Revision 1.2  2001/08/10 08:16:21  rudi
//
//               - Changed IO names to be more clear.
//               - Uniquifyed define names to be core specific.
//               - Removed "Refresh Early" configuration
//
//               Revision 1.1  2001/07/29 07:34:41  rudi
//
//
//               1) Changed Directory Structure
//               2) Fixed several minor bugs
//
//               Revision 1.3  2001/06/12 15:19:49  rudi
//
//
//              Minor changes after running lint, and a small bug
//		fix reading csr and ba_mask registers.
//
//               Revision 1.2  2001/06/03 11:37:17  rudi
//
//
//               1) Fixed Chip Select Mask Register
//               	- Power On Value is now all ones
//               	- Comparison Logic is now correct
//
//               2) All resets are now asynchronous
//
//               3) Converted Power On Delay to an configurable item
//
//               4) Added reset to Chip Select Output Registers
//
//               5) Forcing all outputs to Hi-Z state during reset
//
//               Revision 1.1.1.1  2001/05/13 09:39:38  rudi
//               Created Directory Structure
//
//
//
//

`timescale 1ns / 10ps

/////////////////////////////////////////////////////////////////////
//
// This define selects how the WISHBONE interface determines if
// the internal register file is selected.
// This should be a simple address decoder. "wb_addr_i" is the
// WISHBONE address bus (32 bits wide).
`define	MC_REG_SEL		(wb_addr_i[31:29] == 3'b011)

// This define selects how the WISHBONE interface determines if
// the memory is selected.
// This should be a simple address decoder. "wb_addr_i" is the
// WISHBONE address bus (32 bits wide).
`define	MC_MEM_SEL		(wb_addr_i[31:29] == 3'h0)

/////////////////////////////////////////////////////////////////////
//
// This are the default Power-On Reset values for Chip Select
//

// This will be defined by the run script for my test bench ...
// Alternatively force here for synthesis ...
//`define RUDIS_TB 1

// Defines which chip select is used for Power On booting

// To run my default testbench default boot CS must be 3 !!!
`ifdef RUDIS_TB
`define MC_DEF_SEL		3'h3
`else
`define MC_DEF_SEL		3'h0
`endif

// Defines the default (reset) TMS value for the DEF_SEL chip select
`define	MC_DEF_POR_TMS 	32'hffff_ffff


/////////////////////////////////////////////////////////////////////
//
// Define how many Chip Selects to Implement
//
`define MC_HAVE_CS1	1
//`define MC_HAVE_CS2	1
//`define MC_HAVE_CS3	1
//`define MC_HAVE_CS4	1
//`define MC_HAVE_CS5	1
//`define MC_HAVE_CS6	1
//`define MC_HAVE_CS7	1


// To run my default testbench those need to there !!!
`ifdef RUDIS_TB
`define MC_HAVE_CS2	1
`define MC_HAVE_CS3	1
`define MC_HAVE_CS4	1
`define MC_HAVE_CS5	1
`endif

/////////////////////////////////////////////////////////////////////
//
// Init Refresh
//
// Number of Refresh Cycles to perform during SDRAM initialization.
// This varies between SDRAM manufacturer. Typically this value is
// between 2 and 8. This number must be smaller than 16.
`define	MC_INIT_RFRC_CNT	2

/////////////////////////////////////////////////////////////////////
//
// Power On Delay
//
// Most if SDRAMs require some time to initialize before they can be used
// after power on. If the Memory Controller shall stall after power on to
// allow SDRAMs to finish the initialization process uncomment the below
// define statement
`define	MC_POR_DELAY	1

// This value defines how many MEM_CLK cycles the Memory Controller should
// stall. Default is 2.5uS. At a 10nS MEM_CLK cycle time, this would 250
// cycles.
`define	MC_POR_DELAY_VAL	8'd250


// ===============================================================
// ===============================================================
// Various internal defines (DO NOT MODIFY !)
// ===============================================================
// ===============================================================

// Register settings encodings
`define MC_BW_8			2'h0
`define MC_BW_16		2'h1
`define MC_BW_32		2'h2

`define MC_MEM_TYPE_SDRAM	3'h0
`define MC_MEM_TYPE_SRAM	3'h1
`define MC_MEM_TYPE_ACS		3'h2
`define MC_MEM_TYPE_SCS		3'h3

`define MC_MEM_SIZE_64		2'h0
`define MC_MEM_SIZE_128		2'h1
`define MC_MEM_SIZE_256		2'h2

// Command Valid, Ras_, Cas_, We_
`define MC_CMD_NOP		4'b0111
`define MC_CMD_PC		4'b1010
`define MC_CMD_ACT		4'b1011
`define MC_CMD_WR		4'b1100
`define MC_CMD_RD		4'b1101
`define MC_CMD_BT		4'b1110
`define MC_CMD_ARFR		4'b1001
`define MC_CMD_LMR		4'b1000
`define MC_CMD_XRD		4'b1111
`define MC_CMD_XWR		4'b1110

`define MC_SINGLE_BANK		1'b0
`define MC_ALL_BANKS		1'b1

