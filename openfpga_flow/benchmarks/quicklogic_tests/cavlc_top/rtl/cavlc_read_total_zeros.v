//////////////////////////////////////////////////////////////////////
////                                                              ////
////  cavlc_read_total_zeros                                      ////
////                                                              ////
////  Description                                                 ////
////       decode total_zeros                                     ////
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

//2011-8-14 initial version

`include "defines.v"

module cavlc_read_total_zeros
(
    ena,
    sel,
    chroma_DC_sel,
    rbsp,
    TotalCoeff,
    TotalZeros_comb,
    len_comb
);
//------------------------
//ports
//------------------------
input   ena;
input   sel;
input   chroma_DC_sel;
input   [0:8]   rbsp;
input   [3:0]   TotalCoeff;

output  [3:0]   TotalZeros_comb;
output  [3:0]   len_comb;

//-------------------------
//rregs
//-------------------------
reg     [3:0]   TotalZeros_comb;    //TotalZeros will be saved as ZeroLeft in module cavlc_read_run_befores 
reg     [3:0]   len_comb;


//for  chroma_DC
reg     [0:2]   rbsp_chroma_DC;         
reg     [1:0]   TotalZeros_chroma_DC;
reg     [1:0]   len_chroma_DC;

//for TotalCoeff <= 3
reg     [0:8]   rbsp_LE3;       
reg     [3:0]   TotalZeros_LE3;
reg     [3:0]   len_LE3;

//for TotalCoeff > 3
reg     [0:5]   rbsp_G3;        
reg     [3:0]   TotalZeros_G3;
reg     [2:0]   len_G3;


//----------------------------------------
//input mux
//----------------------------------------
always @(*)
if (ena && sel && chroma_DC_sel) begin
    rbsp_chroma_DC  <= rbsp[0:2];
    rbsp_LE3        <= 'hffff;
    rbsp_G3         <= 'hffff;
end
else if (ena && sel && TotalCoeff[3:2] == 2'b00) begin
    rbsp_chroma_DC  <= 'hffff;
    rbsp_LE3        <= rbsp[0:8];
    rbsp_G3         <= 'hffff;
end
else if (ena && sel)begin
    rbsp_chroma_DC  <= 'hffff;
    rbsp_LE3        <= 'hffff;
    rbsp_G3         <= rbsp[0:5];
end
else begin
    rbsp_chroma_DC  <= 'hffff;
    rbsp_LE3        <= 'hffff;
    rbsp_G3         <= 'hffff;
end

//----------------------------------------
//TotalZeros_chroma_DC & len_chroma_DC
//----------------------------------------
always @(*)
if ( TotalCoeff == 1 && rbsp_chroma_DC[0] ) begin
    TotalZeros_chroma_DC    <= 0;
    len_chroma_DC           <= 1;
end
else if ( TotalCoeff == 1 && rbsp_chroma_DC[1] ) begin
    TotalZeros_chroma_DC    <= 1;
    len_chroma_DC           <= 2;
end
else if ( TotalCoeff == 1 && rbsp_chroma_DC[2] ) begin
    TotalZeros_chroma_DC    <= 2;
    len_chroma_DC           <= 3;
end
else if ( TotalCoeff == 1 ) begin
    TotalZeros_chroma_DC    <= 3;
    len_chroma_DC           <= 3;
end
else if ( TotalCoeff == 2 && rbsp_chroma_DC[0] ) begin
    TotalZeros_chroma_DC    <= 0;
    len_chroma_DC           <= 1;
end
else if ( TotalCoeff == 2 && rbsp_chroma_DC[1] ) begin
    TotalZeros_chroma_DC    <= 1;
    len_chroma_DC           <= 2;
end
else if ( TotalCoeff == 2 ) begin
    TotalZeros_chroma_DC    <= 2;
    len_chroma_DC           <= 2;
end
else if ( rbsp_chroma_DC[0] ) begin
    TotalZeros_chroma_DC    <= 0;
    len_chroma_DC           <= 1;
end
else begin
    TotalZeros_chroma_DC    <= 1;
    len_chroma_DC           <= 1;
end


//---------------------------------
//TotalZeros_LE3 & len_LE3
//---------------------------------
always @(rbsp_LE3 or TotalCoeff)
case (TotalCoeff[1:0])
1 :begin
    case(1'b1)
    rbsp_LE3[0] : begin
        TotalZeros_LE3  <= 0;
        len_LE3         <= 1;   
    end
    rbsp_LE3[1] : begin
        len_LE3         <= 3;
        if (rbsp_LE3[2])
            TotalZeros_LE3  <= 1;
        else
            TotalZeros_LE3  <= 2;
    end
    rbsp_LE3[2] : begin
        len_LE3         <= 4;
        if (rbsp_LE3[3])
            TotalZeros_LE3  <= 3;
        else
            TotalZeros_LE3  <= 4;
    end
    rbsp_LE3[3] : begin
        len_LE3         <= 5;
        if (rbsp_LE3[4])
            TotalZeros_LE3  <= 5;
        else
            TotalZeros_LE3  <= 6;
    end
    rbsp_LE3[4] : begin
        len_LE3         <= 6;
        if (rbsp_LE3[5])
            TotalZeros_LE3  <= 7;
        else
            TotalZeros_LE3  <= 8;
    end
    rbsp_LE3[5] : begin
        len_LE3         <= 7;
        if (rbsp_LE3[6])
            TotalZeros_LE3  <= 9;
        else
            TotalZeros_LE3  <= 10;
    end
    rbsp_LE3[6] : begin
        len_LE3         <= 8;
        if (rbsp_LE3[7])
            TotalZeros_LE3  <= 11;
        else
            TotalZeros_LE3  <= 12;
    end
    rbsp_LE3[7] : begin
        len_LE3         <= 9;
        if (rbsp_LE3[8])
            TotalZeros_LE3  <= 13;
        else
            TotalZeros_LE3  <= 14;
    end
    default : begin
        len_LE3         <= 9;
        TotalZeros_LE3  <= 15;
    end
    endcase
end
2 : begin
    case(1'b1)
    rbsp_LE3[0] : begin
        len_LE3 <= 3;
        case(rbsp_LE3[1:2])
        'b11 :  TotalZeros_LE3  <= 0;
        'b10 :  TotalZeros_LE3  <= 1;
        'b01 :  TotalZeros_LE3  <= 2;
        'b00 :  TotalZeros_LE3  <= 3;
        endcase
    end
    rbsp_LE3[1] : begin
        if (rbsp_LE3[2]) begin
            TotalZeros_LE3  <= 4;
            len_LE3         <= 3;
        end
        else begin
            len_LE3         <= 4;
            if (rbsp_LE3[3])
                TotalZeros_LE3  <= 5;
            else
                TotalZeros_LE3  <= 6;
        end
    end
    rbsp_LE3[2] : begin
        len_LE3         <= 4;
        if (rbsp_LE3[3])
            TotalZeros_LE3  <= 7;
        else
            TotalZeros_LE3  <= 8;
    end
    rbsp_LE3[3] : begin
        len_LE3         <= 5;
        if (rbsp_LE3[4])
            TotalZeros_LE3  <= 9;
        else
            TotalZeros_LE3  <= 10;
    end
    default : begin
        len_LE3 <= 6;
        case(rbsp_LE3[4:5])
        'b11 :  TotalZeros_LE3  <= 11;
        'b10 :  TotalZeros_LE3  <= 12;
        'b01 :  TotalZeros_LE3  <= 13;
        'b00 :  TotalZeros_LE3  <= 14;
        endcase
    end
    endcase
end
3 : begin
    case(1'b1)
    rbsp_LE3[0] : begin
        len_LE3 <= 3;
        case(rbsp_LE3[1:2])
        'b11 :  TotalZeros_LE3  <= 1;
        'b10 :  TotalZeros_LE3  <= 2;
        'b01 :  TotalZeros_LE3  <= 3;
        'b00 :  TotalZeros_LE3  <= 6;
        endcase
    end
    rbsp_LE3[1] : begin
        if (rbsp_LE3[2]) begin
            TotalZeros_LE3  <= 7;
            len_LE3         <= 3;
        end
        else begin
            len_LE3         <= 4;
            if (rbsp_LE3[3])
                TotalZeros_LE3  <= 0;
            else
                TotalZeros_LE3  <= 4;
        end
    end
    rbsp_LE3[2] : begin
        len_LE3         <= 4;
        if (rbsp_LE3[3])
            TotalZeros_LE3  <= 5;
        else
            TotalZeros_LE3  <= 8;
    end
    rbsp_LE3[3] : begin
        len_LE3         <= 5;
        if (rbsp_LE3[4])
            TotalZeros_LE3  <= 9;
        else
            TotalZeros_LE3  <= 10;
    end
    rbsp_LE3[4] : begin
        len_LE3         <= 5;
        TotalZeros_LE3  <= 12;
    end
    default : begin
        len_LE3 <= 6;
        if(rbsp_LE3[5])
            TotalZeros_LE3  <= 11;
        else
            TotalZeros_LE3  <= 13;      
    end
    endcase
end
default : begin
    len_LE3         <= 'bx;
    TotalZeros_LE3  <= 'bx;
end
endcase

//---------------------------------
//TotalZeros_G3 & len_G3
//---------------------------------
always @(rbsp_G3 or TotalCoeff)
case (TotalCoeff)
4 : begin
    case(1'b1)
    rbsp_G3[0] : begin
        len_G3  <= 3;
        case(rbsp_G3[1:2])
        'b11 :  TotalZeros_G3   <= 1;
        'b10 :  TotalZeros_G3   <= 4;
        'b01 :  TotalZeros_G3   <= 5;
        'b00 :  TotalZeros_G3   <= 6;
        endcase
    end
    rbsp_G3[1] : begin
        if (rbsp_G3[2]) begin
            TotalZeros_G3   <= 8;
            len_G3          <= 3;
        end
        else begin
            len_G3          <= 4;
            if (rbsp_G3[3])
                TotalZeros_G3   <= 2;
            else
                TotalZeros_G3   <= 3;
        end
    end
    rbsp_G3[2] : begin
        len_G3          <= 4;
        if (rbsp_G3[3])
            TotalZeros_G3   <= 7;
        else
            TotalZeros_G3   <= 9;
    end
    default : begin
        len_G3  <= 5;
        case(rbsp_G3[3:4])
        'b11 :  TotalZeros_G3   <= 0;
        'b10 :  TotalZeros_G3   <= 10;
        'b01 :  TotalZeros_G3   <= 11;
        'b00 :  TotalZeros_G3   <= 12;
        endcase
    end
    endcase
end
5 :begin
    case(1'b1)
    rbsp_G3[0] : begin
        len_G3  <= 3;
        case(rbsp_G3[1:2])
        'b11 :  TotalZeros_G3   <= 3;
        'b10 :  TotalZeros_G3   <= 4;
        'b01 :  TotalZeros_G3   <= 5;
        'b00 :  TotalZeros_G3   <= 6;
        endcase
    end
    rbsp_G3[1] : begin
        if (rbsp_G3[2]) begin
            TotalZeros_G3   <= 7;
            len_G3          <= 3;
        end
        else begin
            len_G3          <= 4;
            if (rbsp_G3[3])
                TotalZeros_G3   <= 0;
            else
                TotalZeros_G3   <= 1;
        end
    end
    rbsp_G3[2] : begin
        len_G3          <= 4;
        if (rbsp_G3[3])
            TotalZeros_G3   <= 2;
        else
            TotalZeros_G3   <= 8;
    end
    rbsp_G3[3] : begin
        len_G3          <= 4;
        TotalZeros_G3   <= 10;
    end
    default : begin
        len_G3  <= 5;
        if (rbsp_G3[4])
            TotalZeros_G3   <= 9;
        else
            TotalZeros_G3   <= 11;
    end
    endcase
end
6 : begin
    case(1'b1)
    rbsp_G3[0] : begin
        len_G3  <= 3;
        case(rbsp_G3[1:2])
        'b11 :  TotalZeros_G3   <= 2;
        'b10 :  TotalZeros_G3   <= 3;
        'b01 :  TotalZeros_G3   <= 4;
        'b00 :  TotalZeros_G3   <= 5;
        endcase
    end
    rbsp_G3[1] : begin
        len_G3          <= 3;
        if (rbsp_G3[2])
            TotalZeros_G3   <= 6;
        else
            TotalZeros_G3   <= 7;
    end
    rbsp_G3[2] : begin
        len_G3          <= 3;
        TotalZeros_G3   <= 9;
    end
    rbsp_G3[3] : begin
        len_G3          <= 4;
        TotalZeros_G3   <= 8;
    end
    rbsp_G3[4] : begin
        len_G3          <= 5;
        TotalZeros_G3   <= 1;
    end
    default : begin
        len_G3  <= 6;
        if (rbsp_G3[5])
            TotalZeros_G3   <= 0;
        else
            TotalZeros_G3   <= 10;
    end
    endcase
end
7 :begin
    case(1'b1)
    rbsp_G3[0] : begin
        if (rbsp_G3[1]) begin
            TotalZeros_G3   <= 5;
            len_G3          <= 2;
        end
        else begin
            len_G3          <= 3;
            if (rbsp_G3[2])
                TotalZeros_G3   <= 2;
            else
                TotalZeros_G3   <= 3;
        end
    end
    rbsp_G3[1] : begin
        len_G3  <= 3;
        if (rbsp_G3[2])
            TotalZeros_G3   <= 4;
        else
            TotalZeros_G3   <= 6;
    end
    rbsp_G3[2] : begin
        len_G3          <= 3;
        TotalZeros_G3   <= 8;
    end
    rbsp_G3[3] : begin
        len_G3          <= 4;
        TotalZeros_G3   <= 7;
    end
    rbsp_G3[4] : begin
        len_G3          <= 5;
        TotalZeros_G3   <= 1;
    end
    default : begin
        len_G3          <= 6;
        if (rbsp_G3[5])
            TotalZeros_G3   <= 0;
        else
            TotalZeros_G3   <= 9;
    end
    endcase
end
8 :begin
    case(1'b1)
    rbsp_G3[0] : begin
        len_G3          <= 2;
        if (rbsp_G3[1]) 
            TotalZeros_G3   <= 4;
        else 
            TotalZeros_G3   <= 5;
    end
    rbsp_G3[1] : begin
        len_G3          <= 3;
        if (rbsp_G3[2]) 
            TotalZeros_G3   <= 3;
        else 
            TotalZeros_G3   <= 6;
    end
    rbsp_G3[2] : begin
        len_G3          <= 3;
        TotalZeros_G3   <= 7;
    end
    rbsp_G3[3] : begin
        len_G3          <= 4;
        TotalZeros_G3   <= 1;
    end
    rbsp_G3[4] : begin
        len_G3          <= 5;
        TotalZeros_G3   <= 2;
    end
    default : begin
        len_G3          <= 6;
        if (rbsp_G3[5])
            TotalZeros_G3   <= 0;
        else
            TotalZeros_G3   <= 8;
    end
    endcase
end
9 : begin
    case(1'b1)
    rbsp_G3[0] : begin
        len_G3          <= 2;
        if (rbsp_G3[1]) 
            TotalZeros_G3   <= 3;
        else 
            TotalZeros_G3   <= 4;
    end
    rbsp_G3[1] : begin
        len_G3          <= 2;
        TotalZeros_G3   <= 6;
    end
    rbsp_G3[2] : begin
        len_G3          <= 3;
        TotalZeros_G3   <= 5;
    end
    rbsp_G3[3] : begin
        len_G3          <= 4;
        TotalZeros_G3   <= 2;
    end
    rbsp_G3[4] : begin
        len_G3          <= 5;
        TotalZeros_G3   <= 7;
    end
    default : begin
        len_G3          <= 6;
        if (rbsp_G3[5])
            TotalZeros_G3   <= 0;
        else
            TotalZeros_G3   <= 1;
    end
    endcase
end
10 : begin
    case(1'b1)
    rbsp_G3[0] : begin
        len_G3          <= 2;
        if (rbsp_G3[1]) 
            TotalZeros_G3   <= 3;
        else 
            TotalZeros_G3   <= 4;
    end
    rbsp_G3[1] : begin
        len_G3          <= 2;
        TotalZeros_G3   <= 5;
    end
    rbsp_G3[2] : begin
        len_G3          <= 3;
        TotalZeros_G3   <= 2;
    end
    rbsp_G3[3] : begin
        len_G3          <= 4;
        TotalZeros_G3   <= 6;
    end
    default : begin
        len_G3          <= 5;
        if (rbsp_G3[4])
            TotalZeros_G3   <= 0;
        else
            TotalZeros_G3   <= 1;
    end
    endcase
end
11 : begin
    case(1'b1)
    rbsp_G3[0] : begin
        len_G3          <= 1;
        TotalZeros_G3   <= 4;
    end
    rbsp_G3[1] : begin
        len_G3          <= 3;
        if (rbsp_G3[2]) 
            TotalZeros_G3   <= 5;
        else 
            TotalZeros_G3   <= 3;
    end
    rbsp_G3[2] : begin
        len_G3          <= 3;
        TotalZeros_G3   <= 2;
    end
    default : begin
        len_G3          <= 4;
        if (rbsp_G3[3])
            TotalZeros_G3   <= 1;
        else
            TotalZeros_G3   <= 0;
    end
    endcase
end
12 : begin
    case(1'b1)
    rbsp_G3[0] : begin
        len_G3          <= 1;
        TotalZeros_G3   <= 3;
    end
    rbsp_G3[1] : begin
        len_G3          <= 2;
        TotalZeros_G3   <= 2;
    end
    rbsp_G3[2] : begin
        len_G3          <= 3;
        TotalZeros_G3   <= 4;
    end
    default : begin
        len_G3          <= 4;
        if (rbsp_G3[3])
            TotalZeros_G3   <= 1;
        else
            TotalZeros_G3   <= 0;
    end
    endcase
end
13  :begin
    if (rbsp_G3[0]) begin
        TotalZeros_G3   <= 2;
        len_G3          <= 1;       
    end
    else if (rbsp_G3[1]) begin
        TotalZeros_G3   <= 3;
        len_G3          <= 2;   
    end
    else if (rbsp_G3[2]) begin
        TotalZeros_G3   <= 1;
        len_G3          <= 3;   
    end
    else begin
        TotalZeros_G3   <= 0;
        len_G3          <= 3;       
    end
end
14  : begin
    if (rbsp_G3[0]) begin
        TotalZeros_G3   <= 2;
        len_G3          <= 1;       
    end
    else if (rbsp_G3[1]) begin
        TotalZeros_G3   <= 1;
        len_G3          <= 2;   
    end
    else begin
        TotalZeros_G3   <= 0;
        len_G3          <= 2;       
    end
end
15  : begin
    len_G3  <= 1;
    if (rbsp_G3[0])
        TotalZeros_G3   <= 1;
    else
        TotalZeros_G3   <= 0;
end
default : begin
    len_G3          <= 'bx;
    TotalZeros_G3   <= 'bx;
end
endcase

//---------------------------------
//TotalZeros_comb & len_comb
//---------------------------------
always @(*)
if (ena && sel && chroma_DC_sel) begin
    TotalZeros_comb     <= TotalZeros_chroma_DC;
    len_comb            <= len_chroma_DC;
end
else if (ena && sel && TotalCoeff[3:2] == 2'b00) begin
    TotalZeros_comb     <= TotalZeros_LE3;
    len_comb            <= len_LE3;
end
else if (ena && sel)begin
    TotalZeros_comb     <= TotalZeros_G3;
    len_comb            <= len_G3;
end
else begin
    TotalZeros_comb     <= 0;
    len_comb            <= 0;
end


endmodule
