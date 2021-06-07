//////////////////////////////////////////////////////////////////////
////                                                              ////
////  SBOX 3                                                      ////
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
// $Log: s3.v,v $
// Revision 1.1.1.1  2004/07/05 17:31:17  jcastillo
// First import
//


module s3(stage1_input,stage1_output);
input [5:0] stage1_input;
output [3:0] stage1_output;

reg [3:0] stage1_output;



always @(  stage1_input)

begin

   case(stage1_input)

        0: stage1_output = (10); 
        1: stage1_output = (13); 
        2: stage1_output = (0); 
        3: stage1_output = (7); 
        4: stage1_output = (9); 
        5: stage1_output = (0); 
        6: stage1_output = (14); 
        7: stage1_output = (9); 
        8: stage1_output = (6); 
        9: stage1_output = (3); 
        10: stage1_output = (3); 
        11: stage1_output = (4); 
        12: stage1_output = (15); 
        13: stage1_output = (6); 
        14: stage1_output = (5); 
        15: stage1_output = (10); 
        16: stage1_output = (1); 
        17: stage1_output = (2); 
        18: stage1_output = (13); 
        19: stage1_output = (8); 
        20: stage1_output = (12); 
        21: stage1_output = (5); 
        22: stage1_output = (7); 
        23: stage1_output = (14); 
        24: stage1_output = (11); 
        25: stage1_output = (12); 
        26: stage1_output = (4); 
        27: stage1_output = (11); 
        28: stage1_output = (2); 
        29: stage1_output = (15); 
        30: stage1_output = (8); 
        31: stage1_output = (1); 
        32: stage1_output = (13); 
        33: stage1_output = (1); 
        34: stage1_output = (6); 
        35: stage1_output = (10); 
        36: stage1_output = (4); 
        37: stage1_output = (13); 
        38: stage1_output = (9); 
        39: stage1_output = (0); 
        40: stage1_output = (8); 
        41: stage1_output = (6); 
        42: stage1_output = (15); 
        43: stage1_output = (9); 
        44: stage1_output = (3); 
        45: stage1_output = (8); 
        46: stage1_output = (0); 
        47: stage1_output = (7); 
        48: stage1_output = (11); 
        49: stage1_output = (4); 
        50: stage1_output = (1); 
        51: stage1_output = (15); 
        52: stage1_output = (2); 
        53: stage1_output = (14); 
        54: stage1_output = (12); 
        55: stage1_output = (3); 
        56: stage1_output = (5); 
        57: stage1_output = (11); 
        58: stage1_output = (10); 
        59: stage1_output = (5); 
        60: stage1_output = (14); 
        61: stage1_output = (2); 
        62: stage1_output = (7); 
        63: stage1_output = (12); 
  
endcase



end

endmodule
