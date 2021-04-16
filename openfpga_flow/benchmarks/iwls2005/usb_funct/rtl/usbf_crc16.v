/////////////////////////////////////////////////////////////////////
////                                                             ////
////  USB CRC5 and CRC16 Modules                                 ////
////                                                             ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/usb/       ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2000-2003 Rudolf Usselmann                    ////
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
//  $Id: usbf_crc16.v,v 1.2 2003/10/17 02:36:57 rudi Exp $
//
//  $Date: 2003/10/17 02:36:57 $
//  $Revision: 1.2 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: usbf_crc16.v,v $
//               Revision 1.2  2003/10/17 02:36:57  rudi
//               - Disabling bit stuffing and NRZI encoding during speed negotiation
//               - Now the core can send zero size packets
//               - Fixed register addresses for some of the higher endpoints
//                 (conversion between decimal/hex was wrong)
//               - The core now does properly evaluate the function address to
//                 determine if the packet was intended for it.
//               - Various other minor bugs and typos
//
//               Revision 1.1  2001/08/03 05:30:09  rudi
//
//
//               1) Reorganized directory structure
//
//               Revision 1.0  2001/03/07 09:17:12  rudi
//
//
//               Changed all revisions to revision 1.0. This is because OpenCores CVS
//               interface could not handle the original '0.1' revision ....
//
//               Revision 0.1.0.1  2001/02/28 08:10:42  rudi
//               Initial Release
//
//                            

`include "usbf_defines.v"

///////////////////////////////////////////////////////////////////
//
// CRC16
//
///////////////////////////////////////////////////////////////////

module usbf_crc16(crc_in, din, crc_out);
input	[15:0]	crc_in;
input	[7:0]	din;
output	[15:0]	crc_out;

assign crc_out[0] =	din[7] ^ din[6] ^ din[5] ^ din[4] ^ din[3] ^
			din[2] ^ din[1] ^ din[0] ^ crc_in[8] ^ crc_in[9] ^
			crc_in[10] ^ crc_in[11] ^ crc_in[12] ^ crc_in[13] ^
			crc_in[14] ^ crc_in[15];
assign crc_out[1] =	din[7] ^ din[6] ^ din[5] ^ din[4] ^ din[3] ^ din[2] ^
			din[1] ^ crc_in[9] ^ crc_in[10] ^ crc_in[11] ^
			crc_in[12] ^ crc_in[13] ^ crc_in[14] ^ crc_in[15];
assign crc_out[2] =	din[1] ^ din[0] ^ crc_in[8] ^ crc_in[9];
assign crc_out[3] =	din[2] ^ din[1] ^ crc_in[9] ^ crc_in[10];
assign crc_out[4] =	din[3] ^ din[2] ^ crc_in[10] ^ crc_in[11];
assign crc_out[5] =	din[4] ^ din[3] ^ crc_in[11] ^ crc_in[12];
assign crc_out[6] =	din[5] ^ din[4] ^ crc_in[12] ^ crc_in[13];
assign crc_out[7] =	din[6] ^ din[5] ^ crc_in[13] ^ crc_in[14];
assign crc_out[8] =	din[7] ^ din[6] ^ crc_in[0] ^ crc_in[14] ^ crc_in[15];
assign crc_out[9] =	din[7] ^ crc_in[1] ^ crc_in[15];
assign crc_out[10] =	crc_in[2];
assign crc_out[11] =	crc_in[3];
assign crc_out[12] =	crc_in[4];
assign crc_out[13] =	crc_in[5];
assign crc_out[14] =	crc_in[6];
assign crc_out[15] =	din[7] ^ din[6] ^ din[5] ^ din[4] ^ din[3] ^ din[2] ^
			din[1] ^ din[0] ^ crc_in[7] ^ crc_in[8] ^ crc_in[9] ^
			crc_in[10] ^ crc_in[11] ^ crc_in[12] ^ crc_in[13] ^
			crc_in[14] ^ crc_in[15];

endmodule

