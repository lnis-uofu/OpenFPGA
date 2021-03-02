//////////////////////////////////////////////////////////////////////
////                                                              ////
////  cavlc_read_run_befores                                      ////
////                                                              ////
////  Description                                                 ////
////       decode runs and combine them with levels               ////
////                                                              ////
////  Author(s):                                                  ////
////      - bin qiu, qiubin@opencores.org                         ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2011 Authors and OPENCORES.ORG                 ////
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

//2011-8-16 initiial revision

`include "defines.v"

module cavlc_read_run_befores
(
    clk,
    rst_n,
    ena,
    sel,
    clr,
    ZeroLeft_init,
    
    rbsp,
    i,
    TotalZeros_comb,
    
    level_0,
    level_1,
    level_2,
    level_3,
    level_4,
    level_5,
    level_6,
    level_7,
    level_8,
    level_9,
    level_10,
    level_11,
    level_12,
    level_13,
    level_14,
    level_15,
        
    coeff_0,
    coeff_1,
    coeff_2,
    coeff_3,
    coeff_4,
    coeff_5,
    coeff_6,
    coeff_7,
    coeff_8,
    coeff_9,
    coeff_10,
    coeff_11,
    coeff_12,
    coeff_13,
    coeff_14,
    coeff_15,
    ZeroLeft,
    len_comb
);  
//----------------------
//ports
//----------------------
input   clk;
input   rst_n;
input   ena;
input   sel;
input   clr;
input   ZeroLeft_init;

input   [0:10]  rbsp;
input   [3:0]   i;          //range from TotalCoeff-1 to 0
input   [3:0]   TotalZeros_comb;

input   [8:0]   level_0;
input   [8:0]   level_1;
input   [8:0]   level_2;
input   [8:0]   level_3;
input   [8:0]   level_4;
input   [8:0]   level_5;
input   [8:0]   level_6;
input   [8:0]   level_7;
input   [8:0]   level_8;
input   [8:0]   level_9;
input   [8:0]   level_10;
input   [8:0]   level_11;
input   [8:0]   level_12;
input   [8:0]   level_13;
input   [8:0]   level_14;
input   [8:0]   level_15;

output  [8:0]   coeff_0;
output  [8:0]   coeff_1;
output  [8:0]   coeff_2;
output  [8:0]   coeff_3;
output  [8:0]   coeff_4;
output  [8:0]   coeff_5;
output  [8:0]   coeff_6;
output  [8:0]   coeff_7;
output  [8:0]   coeff_8;
output  [8:0]   coeff_9;
output  [8:0]   coeff_10;
output  [8:0]   coeff_11;
output  [8:0]   coeff_12;
output  [8:0]   coeff_13;
output  [8:0]   coeff_14;
output  [8:0]   coeff_15;

output  [3:0]   ZeroLeft;

output  [3:0]   len_comb;

//----------------------
//regs
//----------------------
reg     [3:0]   run;
reg     [3:0]   len;
reg     [8:0]   coeff;


reg     [3:0]   len_comb;

//----------------------
//FFs
//----------------------
reg     [3:0]   ZeroLeft;

reg     [8:0]   coeff_0;
reg     [8:0]   coeff_1;
reg     [8:0]   coeff_2;
reg     [8:0]   coeff_3;
reg     [8:0]   coeff_4;
reg     [8:0]   coeff_5;
reg     [8:0]   coeff_6;
reg     [8:0]   coeff_7;
reg     [8:0]   coeff_8;
reg     [8:0]   coeff_9;
reg     [8:0]   coeff_10;
reg     [8:0]   coeff_11;
reg     [8:0]   coeff_12;
reg     [8:0]   coeff_13;
reg     [8:0]   coeff_14;
reg     [8:0]   coeff_15;

//----------------------
//run & len
//----------------------
always @(rbsp or ZeroLeft or ena or sel)
if (ena && sel)
case(ZeroLeft)
    0 : begin len <= 0; run <= 0; end
    1 : begin len <= 1; run <= rbsp[0]? 0:1; end
    2 : begin if (rbsp[0]) begin
            run <= 0;
            len <= 1;
        end
        else if (rbsp[1]) begin
            run <= 1;
            len <= 2;
        end 
        else begin
            run <= 2;
            len <= 2;
        end
    end
    3 : begin
        run <= 3 - rbsp[0:1];
        len <= 2;
    end
    4 : begin
        if (rbsp[0:1] != 0) begin
            run <= 3 - rbsp[0:1];
            len <= 2;
        end
        else begin
            run <= rbsp[2]? 3:4;
            len <= 3;
        end
    end
    5 : begin
        if (rbsp[0]) begin
            run <= rbsp[1]? 0:1;
            len <= 2;
        end
        else if (rbsp[1]) begin
            run <= rbsp[2]? 2:3;
            len <= 3;
        end     
        else begin
            run <= rbsp[2]? 4:5;
            len <= 3;       
        end
    end
    6 : begin
        if (rbsp[0:1] == 2'b11) begin
            run <= 0;
            len <= 2;
        end
        else begin
            len <= 3;
            case(rbsp[0:2])
                3'b000 : run <= 1;
                3'b001 : run <= 2;
                3'b011 : run <= 3;
                3'b010 : run <= 4;
                3'b101 : run <= 5;
                default: run <= 6;
            endcase
        end     
    end
    default : begin
        if (rbsp[0:2] != 0) begin
            run <= 7 - rbsp[0:2];
            len <= 3;
        end
        else begin
            case (1'b1)
                rbsp[3] : begin run <= 7;   len <= 4; end
                rbsp[4] : begin run <= 8;   len <= 5; end
                rbsp[5] : begin run <= 9;   len <= 6; end
                rbsp[6] : begin run <= 10;  len <= 7; end
                rbsp[7] : begin run <= 11;  len <= 8; end
                rbsp[8] : begin run <= 12;  len <= 9; end
                rbsp[9] : begin run <= 13;  len <= 10;end
                rbsp[10]: begin run <= 14;  len <= 11;end
                default : begin run <= 'bx; len <='bx;end
            endcase
        end
    end
endcase
else begin
    len <= 0;
    run <= 0;
end

//----------------------
//len_comb
//----------------------
always @(*)
if (i >  0)
    len_comb <= len;
else
    len_comb <= 0;
    
    
//----------------------
//ZeroLeft
//----------------------
always @(posedge clk or negedge rst_n)
if (!rst_n)
    ZeroLeft <= 0;
else if (ena && clr)    //in case TotalCoeff >= max_coeff_num
    ZeroLeft <= 0;
else if (ena && ZeroLeft_init)
    ZeroLeft <= TotalZeros_comb;
else if (ena && sel )//if ZeroLeft == 0, run will be 0
    ZeroLeft <= ZeroLeft - run; 
        
//----------------------
//coeff
//----------------------
always @(*)
if (ena && sel)
case (i)
    0 :coeff <= level_0;
    1 :coeff <= level_1;
    2 :coeff <= level_2;
    3 :coeff <= level_3;
    4 :coeff <= level_4;
    5 :coeff <= level_5;    
    6 :coeff <= level_6;
    7 :coeff <= level_7;
    8 :coeff <= level_8;
    9 :coeff <= level_9;
    10:coeff <= level_10;
    11:coeff <= level_11;       
    12:coeff <= level_12;
    13:coeff <= level_13;
    14:coeff <= level_14;
    15:coeff <= level_15;
endcase
else
    coeff <= 0;

//----------------------
//coeff_0 to coeff_15
//----------------------
always @(posedge clk or negedge rst_n)
if (!rst_n) begin
    coeff_0 <= 0;   coeff_1 <= 0;   coeff_2 <= 0;   coeff_3 <= 0;
    coeff_4 <= 0;   coeff_5 <= 0;   coeff_6 <= 0;   coeff_7 <= 0;
    coeff_8 <= 0;   coeff_9 <= 0;   coeff_10<= 0;   coeff_11<= 0;
    coeff_12<= 0;   coeff_13<= 0;   coeff_14<= 0;   coeff_15<= 0;
end
else if (ena && clr) begin
    coeff_0 <= 0;   coeff_1 <= 0;   coeff_2 <= 0;   coeff_3 <= 0;
    coeff_4 <= 0;   coeff_5 <= 0;   coeff_6 <= 0;   coeff_7 <= 0;
    coeff_8 <= 0;   coeff_9 <= 0;   coeff_10<= 0;   coeff_11<= 0;
    coeff_12<= 0;   coeff_13<= 0;   coeff_14<= 0;   coeff_15<= 0;
end
else if (ena && sel && ZeroLeft > 0)
case (ZeroLeft+i)
    1 :coeff_1  <= coeff;
    2 :coeff_2  <= coeff;
    3 :coeff_3  <= coeff;
    4 :coeff_4  <= coeff;
    5 :coeff_5  <= coeff;
    6 :coeff_6  <= coeff;
    7 :coeff_7  <= coeff;
    8 :coeff_8  <= coeff;
    9 :coeff_9  <= coeff;
    10:coeff_10 <= coeff;
    11:coeff_11 <= coeff;
    12:coeff_12 <= coeff;
    13:coeff_13 <= coeff;
    14:coeff_14 <= coeff;
    default:
    coeff_15    <= coeff;
endcase
else if (ena && sel) begin
    if (i >= 0) coeff_0  <= level_0;
    if (i >= 1) coeff_1  <= level_1;
    if (i >= 2) coeff_2  <= level_2;
    if (i >= 3) coeff_3  <= level_3;
    if (i >= 4) coeff_4  <= level_4;
    if (i >= 5) coeff_5  <= level_5;
    if (i >= 6) coeff_6  <= level_6;
    if (i >= 7) coeff_7  <= level_7;
    if (i >= 8) coeff_8  <= level_8;
    if (i >= 9) coeff_9  <= level_9;
    if (i >= 10)coeff_10 <= level_10;
    if (i >= 11)coeff_11 <= level_11;
    if (i >= 12)coeff_12 <= level_12;
    if (i >= 13)coeff_13 <= level_13;
    if (i >= 14)coeff_14 <= level_14;
    if (i == 15)coeff_15 <= level_15;
end
endmodule
