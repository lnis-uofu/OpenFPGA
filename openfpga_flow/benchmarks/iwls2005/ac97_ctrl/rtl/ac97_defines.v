/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE AC 97 Controller Definitions                      ////
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
//  $Id: ac97_defines.v,v 1.5 2002/09/19 06:30:56 rudi Exp $
//
//  $Date: 2002/09/19 06:30:56 $
//  $Revision: 1.5 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: ac97_defines.v,v $
//               Revision 1.5  2002/09/19 06:30:56  rudi
//               Fixed a bug reported by Igor. Apparently this bug only shows up when
//               the WB clock is very low (2x bit_clk). Updated Copyright header.
//
//               Revision 1.4  2002/03/11 03:21:22  rudi
//
//               - Added defines to select fifo depth between 4, 8 and 16 entries.
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
//               Revision 1.1.1.1  2001/05/19 02:29:14  rudi
//               Initial Checkin
//
//
//
//

`timescale 1ns / 10ps

/////////////////////////////////////////////////////////////////////
// This AC97 Controller supports up to 6 Output and 3 Input Channels.
// Comment out the define statement for which channels you do not wish
// to support in your implementation. The main Left and Right channels
// are always supported. 

// Surround Left + Right
`define AC97_SURROUND		1

// Center Channel
`define AC97_CENTER		1

// LFE Channel
`define AC97_LFE		1

// Stereo Input
`define AC97_SIN		1

// Mono Microphone Input
`define AC97_MICIN		1

/////////////////////////////////////////////////////////////////////
//
// This define selects how the WISHBONE interface determines if
// the internal register file is selected.
// This should be a simple address decoder. "wb_addr_i" is the
// WISHBONE address bus (32 bits wide).
`define	AC97_REG_SEL		(wb_addr_i[31:29] == 3'h0)

/////////////////////////////////////////////////////////////////////
//
// This is a prescaler that generates a pulse every 250 nS.
// The value here should one less than the actually calculated
// value.
// For a 200 MHz wishbone clock, this value is 49 (50-1).
`define	AC97_250_PS	6'h31

/////////////////////////////////////////////////////////////////////
//
// AC97 Cold reset Must be asserted for at least 1uS. The AC97
// controller will stretch the reset pulse to at least 1uS.
// The reset timer is driven by the AC97_250_PS prescaler.
// This value should probably be never changed. Adjust the
// AC97_250_PS instead.
`define	AC97_RST_DEL	3'h4

/////////////////////////////////////////////////////////////////////
//
// This value indicates for how long the resume signaling (asserting sync)
// should be done. This counter is driven by the AC97_250_PS prescaler.
// This value times 250nS is the duration of the resume signaling.
// The actual value must be incremented by one, as we do not know
// the current state of the prescaler, and must somehow insure we
// meet the minimum 1uS length. This value should probably be never
// changed. Modify the AC97_250_PS instead.
`define AC97_RES_SIG	3'h5

/////////////////////////////////////////////////////////////////////
//
// If the bit clock is absent for at least two "predicted" bit
// clock periods (163 nS) we should signal "suspended".
// This value defines how many WISHBONE cycles must pass without
// any change on the bit clock input before we signal "suspended".
// For a 200 MHz WISHBONE clock this would be about (163/5) 33 cycles.
`define AC97_SUSP_DET	6'h21

/////////////////////////////////////////////////////////////////////
//
// Select FIFO Depth. For most applications a FIFO depth of 4 should
// be sufficient. For systems with slow interrupt processing or slow
// DMA response or systems with low internal bus bandwidth you might
// want to increase the FIFO sizes to reduce the interrupt/DMA service
// request frequencies.
// Service request frequency can be calculated as follows:
// Channel bandwidth / FIFO size = Service Request Frequency
// For Example: 48KHz / 4 = 12 kHz
//
// Select Input FIFO depth by uncommenting ONE of the following define
// statements:
`define AC97_IN_FIFO_DEPTH_4
//`define AC97_IN_FIFO_DEPTH_8
//`define AC97_IN_FIFO_DEPTH_16
//
// Select Output FIFO depth by uncommenting ONE of the following define
// statements:
`define AC97_OUT_FIFO_DEPTH_4
//`define AC97_OUT_FIFO_DEPTH_8
//`define AC97_OUT_FIFO_DEPTH_16

