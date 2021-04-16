//////////////////////////////////////////////////////////////////////
////                                                              ////
////  SBOX 8                                                      ////
////                                                              ////
////  This file is part of the SystemC DES                        ////
////                                                              ////
////  Description:                                                ////
////  Sbox of DES algorithm                                       ////
////                                                              ////
////  Generated automatically using SystemC to Verilog translator ////
////                                                              ////
////  To Do:                                                      ////
////   - done                                                     ////
////                                                              ////
////  Author(s):                                                  ////
////      - Javier Castillo, jcastilo@opencores.org               ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Authors and OPENCORES.ORG                 ////
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
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//
// CVS Revision History
//
// $Log: s8.v,v $
// Revision 1.1.1.1  2004/07/05 17:31:17  jcastillo
// First import
//

module s8(stage1_input,stage1_output);
input [5:0] stage1_input;
output [3:0] stage1_output;

reg [3:0] stage1_output;



always @(stage1_input)

begin

   case(stage1_input)

        0: stage1_output = (13); 
        1: stage1_output = (1); 
        2: stage1_output = (2); 
        3: stage1_output = (15); 
        4: stage1_output = (8); 
        5: stage1_output = (13); 
        6: stage1_output = (4); 
        7: stage1_output = (8); 
        8: stage1_output = (6); 
        9: stage1_output = (10); 
        10: stage1_output = (15); 
        11: stage1_output = (3); 
        12: stage1_output = (11); 
        13: stage1_output = (7); 
        14: stage1_output = (1); 
        15: stage1_output = (4); 
        16: stage1_output = (10); 
        17: stage1_output = (12); 
        18: stage1_output = (9); 
        19: stage1_output = (5); 
        20: stage1_output = (3); 
        21: stage1_output = (6); 
        22: stage1_output = (14); 
        23: stage1_output = (11); 
        24: stage1_output = (5); 
        25: stage1_output = (0); 
        26: stage1_output = (0); 
        27: stage1_output = (14); 
        28: stage1_output = (12); 
        29: stage1_output = (9); 
        30: stage1_output = (7); 
        31: stage1_output = (2); 
        32: stage1_output = (7); 
        33: stage1_output = (2); 
        34: stage1_output = (11); 
        35: stage1_output = (1); 
        36: stage1_output = (4); 
        37: stage1_output = (14); 
        38: stage1_output = (1); 
        39: stage1_output = (7); 
        40: stage1_output = (9); 
        41: stage1_output = (4); 
        42: stage1_output = (12); 
        43: stage1_output = (10); 
        44: stage1_output = (14); 
        45: stage1_output = (8); 
        46: stage1_output = (2); 
        47: stage1_output = (13); 
        48: stage1_output = (0); 
        49: stage1_output = (15); 
        50: stage1_output = (6); 
        51: stage1_output = (12); 
        52: stage1_output = (10); 
        53: stage1_output = (9); 
        54: stage1_output = (13); 
        55: stage1_output = (0); 
        56: stage1_output = (15); 
        57: stage1_output = (3); 
        58: stage1_output = (3); 
        59: stage1_output = (5); 
        60: stage1_output = (5); 
        61: stage1_output = (6); 
        62: stage1_output = (8); 
        63: stage1_output = (11); 

endcase

	

end

endmodule
