//===========================================================================
// $Id: pci_async_reset_flop.v,v 1.1 2003/01/27 16:49:31 mihad Exp $
//
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// async_reset_flop                                             ////
////                                                              ////
//// This file is part of the general opencores effort.           ////
//// <http://www.opencores.org/cores/misc/>                       ////
////                                                              ////
//// Module Description:                                          ////
////                                                              ////
//// Make a rising-edge triggered flop with async reset with a    ////
////   distinguished name so that it's output can be easily       ////
////   traced, because it is used for asynchronous reset of some  ////
////   flip-flops.                                                ////
////                                                              ////
//// This flop should be used instead of a regular flop for ALL   ////
////   asynchronous-reset generator flops.                        ////
////                                                              ////
//// To Do:                                                       ////
//// Nothing                                                      ////
////                                                              ////
//// Author(s):                                                   ////
////      - Tadej Markovic, tadej@opencores.org                   ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2001 Authors and OPENCORES.ORG                 ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE. See the GNU Lesser General Public License for more  ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from <http://www.opencores.org/lgpl.shtml>                   ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//
// CVS Revision History
//
// $Log: pci_async_reset_flop.v,v $
// Revision 1.1  2003/01/27 16:49:31  mihad
// Changed module and file names. Updated scripts accordingly. FIFO synchronizations changed.
//
// Revision 1.3  2002/08/14 16:44:19  mihad
// Include statement was enclosed in synosys translate off/on directive - repaired
//
// Revision 1.2  2002/02/25 15:15:43  mihad
// Added include statement that was missing and causing errors
//
// Revision 1.1  2002/02/01 14:43:31  mihad
// *** empty log message ***
//
// 
//

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

`include "pci_constants.v"

module pci_async_reset_flop (
  data_in, clk_in, async_reset_data_out, reset_in
);

input      data_in;
input      clk_in;
output     async_reset_data_out;
input      reset_in;

reg        async_reset_data_out;

always @(posedge clk_in or posedge reset_in)
begin
  if (reset_in)
  begin
    async_reset_data_out <= #`FF_DELAY 1'b0;
  end
  else
  begin
    async_reset_data_out <= #`FF_DELAY data_in;
  end
end

endmodule

