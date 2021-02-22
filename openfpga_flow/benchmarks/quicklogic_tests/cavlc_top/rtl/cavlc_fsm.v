//////////////////////////////////////////////////////////////////////
////                                                              ////
////  cavlc_fsm                                                   ////
////                                                              ////
////  Description                                                 ////
////      controls the cavlc parsing process                      ////
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

//2011-8-7 18:57    initial revision

`include "defines.v"

module cavlc_fsm
(
    clk,
    rst_n,
    ena,
    start,
    max_coeff_num,
    TotalCoeff,
    TotalCoeff_comb,
    TrailingOnes,
    TrailingOnes_comb,
    ZeroLeft,
    state,
    i,
    idle,
    valid
);
//------------------------
//ports
//------------------------
input  clk;
input  rst_n;
input  ena;
input  start;

input  [4:0]  max_coeff_num;
input  [4:0]  TotalCoeff;
input  [4:0]  TotalCoeff_comb;
input  [1:0]  TrailingOnes;
input  [1:0]  TrailingOnes_comb;
input  [3:0]  ZeroLeft;

output [7:0]  state;
output [3:0]  i;
output idle;
output valid;

//------------------------
//FFs
//------------------------
reg  [7:0]  state;
reg  [3:0]  i;
reg  valid;

//------------------------
//state & i & valid
//------------------------
always @(posedge clk or negedge rst_n)
if (!rst_n) begin
    state   <= `cavlc_idle_s;
    i <= 0;
    valid <= 0;
end
else if (ena)
case(state)
    `cavlc_idle_s : begin
        if (start) begin
            state <= `cavlc_read_total_coeffs_s;
            valid <= 0;
        end
        else begin
            state <= `cavlc_idle_s;
        end     
    end
    `cavlc_read_total_coeffs_s : begin
        i <= TotalCoeff_comb -1;
        if (TrailingOnes_comb > 0 && TotalCoeff_comb > 0)
            state <= `cavlc_read_t1s_flags_s;
        else if (TotalCoeff_comb > 0)
            state <= `cavlc_read_level_prefix_s;
        else begin
            state <= `cavlc_idle_s;     
            valid <= 1;
        end
    end
    `cavlc_read_t1s_flags_s : begin
        if (TrailingOnes == TotalCoeff)
            state <= `cavlc_read_total_zeros_s;         
        else begin
            state <= `cavlc_read_level_prefix_s;
            i <= i - TrailingOnes;
        end
    end
    `cavlc_read_level_prefix_s : begin
        state <= `cavlc_read_level_suffix_s;        
    end
    `cavlc_read_level_suffix_s : begin
        state <= `cavlc_calc_level_s;       
    end
    `cavlc_calc_level_s : begin
        if ( i == 0  && TotalCoeff < max_coeff_num)
            state <= `cavlc_read_total_zeros_s;
        else if (i == 0) begin
            state <= `cavlc_read_run_befores_s;
            i <= TotalCoeff - 1;
        end
        else begin
            state <= `cavlc_read_level_prefix_s;
            i <= i - 1;
        end
    end 
    `cavlc_read_total_zeros_s : begin
        state <= `cavlc_read_run_befores_s;
        i <= TotalCoeff - 1;
    end
    `cavlc_read_run_befores_s : begin
        if (i == 0 || ZeroLeft == 0) begin
            state <= `cavlc_idle_s;
            valid <= 1;
        end
        else begin
            state <= `cavlc_read_run_befores_s;
            i <= i - 1;
        end
    end     
endcase

assign idle = state[`cavlc_idle_bit];

endmodule

