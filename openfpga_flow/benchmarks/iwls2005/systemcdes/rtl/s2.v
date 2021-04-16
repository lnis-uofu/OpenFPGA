//////////////////////////////////////////////////////////////////////
////                                                              ////
////  SBOX 2                                                      ////
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
// $Log: s2.v,v $
// Revision 1.1.1.1  2004/07/05 17:31:17  jcastillo
// First import
//


module s2(stage1_input,stage1_output);
input [5:0] stage1_input;
output [3:0] stage1_output;

reg [3:0] stage1_output;



always @(  stage1_input)

begin


   case(stage1_input)
        0: stage1_output = (15); 
        1: stage1_output = (3); 
        2: stage1_output = (1); 
        3: stage1_output = (13); 
        4: stage1_output = (8); 
        5: stage1_output = (4); 
        6: stage1_output = (14); 
        7: stage1_output = (7); 
        8: stage1_output = (6); 
        9: stage1_output = (15); 
        10: stage1_output = (11); 
        11: stage1_output = (2); 
        12: stage1_output = (3); 
        13: stage1_output = (8); 
        14: stage1_output = (4); 
        15: stage1_output = (14); 
        16: stage1_output = (9); 
        17: stage1_output = (12); 
        18: stage1_output = (7); 
        19: stage1_output = (0); 
        20: stage1_output = (2); 
        21: stage1_output = (1); 
        22: stage1_output = (13); 
        23: stage1_output = (10); 
        24: stage1_output = (12); 
        25: stage1_output = (6); 
        26: stage1_output = (0); 
        27: stage1_output = (9); 
        28: stage1_output = (5); 
        29: stage1_output = (11); 
        30: stage1_output = (10); 
        31: stage1_output = (5); 
        32: stage1_output = (0); 
        33: stage1_output = (13); 
        34: stage1_output = (14); 
        35: stage1_output = (8); 
        36: stage1_output = (7); 
        37: stage1_output = (10); 
        38: stage1_output = (11); 
        39: stage1_output = (1); 
        40: stage1_output = (10); 
        41: stage1_output = (3); 
        42: stage1_output = (4); 
        43: stage1_output = (15); 
        44: stage1_output = (13); 
        45: stage1_output = (4); 
        46: stage1_output = (1); 
        47: stage1_output = (2); 
        48: stage1_output = (5); 
        49: stage1_output = (11); 
        50: stage1_output = (8); 
        51: stage1_output = (6); 
        52: stage1_output = (12); 
        53: stage1_output = (7); 
        54: stage1_output = (6); 
        55: stage1_output = (12); 
        56: stage1_output = (9); 
        57: stage1_output = (0); 
        58: stage1_output = (3); 
        59: stage1_output = (5); 
        60: stage1_output = (2); 
        61: stage1_output = (14); 
        62: stage1_output = (15); 
        63: stage1_output = (9); 
   
endcase


end

endmodule
