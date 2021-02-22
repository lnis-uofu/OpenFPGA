//////////////////////////////////////////////////////////////////////
////                                                              ////
////  cavlc_top                                                   ////
////                                                              ////
////  Description                                                 ////
////       top module of cavlc decoder                            ////
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

//2011-8-7 initial version

`include "defines.v"

module cavlc_top
(
    clk,
    rst_n,
    ena,
    start,
    rbsp,
    nC,
    max_coeff_num,

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
    TotalCoeff,
    len_comb,
    idle,
    valid
);
//------------------------
// ports
//------------------------
input   clk, rst_n;
input   ena;
input   start;
input   [0:15]  rbsp;
input   signed [5:0]    nC;
input   [4:0]   max_coeff_num;

output  signed [8:0]    coeff_0;
output  signed [8:0]    coeff_1;
output  signed [8:0]    coeff_2;
output  signed [8:0]    coeff_3;
output  signed [8:0]    coeff_4;
output  signed [8:0]    coeff_5;
output  signed [8:0]    coeff_6;
output  signed [8:0]    coeff_7;
output  signed [8:0]    coeff_8;
output  signed [8:0]    coeff_9;
output  signed [8:0]    coeff_10;
output  signed [8:0]    coeff_11;
output  signed [8:0]    coeff_12;
output  signed [8:0]    coeff_13;
output  signed [8:0]    coeff_14;
output  signed [8:0]    coeff_15;
output  [4:0]   TotalCoeff;
output  [4:0]   len_comb;
output  idle;
output  valid;

//------------------------
// cavlc_read_total_coeffs
//------------------------
wire [1:0] TrailingOnes;
wire [4:0] TotalCoeff;
wire [1:0] TrailingOnes_comb;
wire [4:0] TotalCoeff_comb;
wire [4:0] len_read_total_coeffs_comb;
wire [7:0] cavlc_state;

cavlc_read_total_coeffs cavlc_read_total_coeffs(
    .clk(clk),
    .rst_n(rst_n),
    .ena(ena),
    .start(start),
    .sel(cavlc_state[`cavlc_read_total_coeffs_bit]),
    
    .rbsp(rbsp),
    .nC(nC),
    
    .TrailingOnes(TrailingOnes), 
    .TotalCoeff(TotalCoeff),

    .TrailingOnes_comb(TrailingOnes_comb), 
    .TotalCoeff_comb(TotalCoeff_comb),

    .len_comb(len_read_total_coeffs_comb)
);

//------------------------
// cavlc_read_levels
//------------------------
wire    [4:0]   len_read_levels_comb;
wire    [3:0]   i;

wire    [8:0]   level_0;
wire    [8:0]   level_1;
wire    [8:0]   level_2;
wire    [8:0]   level_3;
wire    [8:0]   level_4;
wire    [8:0]   level_5;
wire    [8:0]   level_6;
wire    [8:0]   level_7;
wire    [8:0]   level_8;
wire    [8:0]   level_9;
wire    [8:0]   level_10;
wire    [8:0]   level_11;
wire    [8:0]   level_12;
wire    [8:0]   level_13;
wire    [8:0]   level_14;
wire    [8:0]   level_15;

cavlc_read_levels cavlc_read_levels(
    .clk(clk),
    .rst_n(rst_n),
    .ena(ena),  
    .t1s_sel(cavlc_state[`cavlc_read_t1s_flags_bit]),
    .prefix_sel(cavlc_state[`cavlc_read_level_prefix_bit]),
    .suffix_sel(cavlc_state[`cavlc_read_level_suffix_bit]),
    .calc_sel(cavlc_state[`cavlc_calc_level_bit]),
    .TrailingOnes(TrailingOnes), 
    .TotalCoeff(TotalCoeff),
    .i(i),
    .rbsp(rbsp),
    
    .level_0(level_0),
    .level_1(level_1),
    .level_2(level_2),
    .level_3(level_3),
    .level_4(level_4),
    .level_5(level_5),
    .level_6(level_6),
    .level_7(level_7),
    .level_8(level_8),
    .level_9(level_9),
    .level_10(level_10),
    .level_11(level_11),
    .level_12(level_12),
    .level_13(level_13),
    .level_14(level_14),
    .level_15(level_15),
    .len_comb(len_read_levels_comb)
);

//------------------------
// cavlc_read_total_zeros
//------------------------
wire    [3:0]   TotalZeros_comb;
wire    [3:0]   len_read_total_zeros_comb;

cavlc_read_total_zeros cavlc_read_total_zeros(
    .ena(ena),
    .sel(cavlc_state[`cavlc_read_total_zeros_bit]),
    .chroma_DC_sel(nC[5]),
    .rbsp(rbsp[0:8]),
    .TotalCoeff(TotalCoeff[3:0]),
    .TotalZeros_comb(TotalZeros_comb),
    .len_comb(len_read_total_zeros_comb)
);

//------------------------
// read_run_before
//------------------------
wire    [3:0]   ZeroLeft;
wire    [3:0]   len_read_run_befores_comb;

cavlc_read_run_befores cavlc_read_run_befores(
    .clk(clk),
    .rst_n(rst_n),
    .ena(ena),
    .clr(cavlc_state[`cavlc_read_total_coeffs_bit]),
    .sel(cavlc_state[`cavlc_read_run_befores_bit]),
    .ZeroLeft_init(cavlc_state[`cavlc_read_total_zeros_bit]),
    
    .rbsp(rbsp[0:10]),
    .i(i),
    .TotalZeros_comb(TotalZeros_comb),
    
    .level_0(level_0),
    .level_1(level_1),
    .level_2(level_2),
    .level_3(level_3),
    .level_4(level_4),
    .level_5(level_5),
    .level_6(level_6),
    .level_7(level_7),
    .level_8(level_8),
    .level_9(level_9),
    .level_10(level_10),
    .level_11(level_11),
    .level_12(level_12),
    .level_13(level_13),
    .level_14(level_14),
    .level_15(level_15),
        
    .coeff_0(coeff_0),
    .coeff_1(coeff_1),
    .coeff_2(coeff_2),
    .coeff_3(coeff_3),
    .coeff_4(coeff_4),
    .coeff_5(coeff_5),
    .coeff_6(coeff_6),
    .coeff_7(coeff_7),
    .coeff_8(coeff_8),
    .coeff_9(coeff_9),
    .coeff_10(coeff_10),
    .coeff_11(coeff_11),
    .coeff_12(coeff_12),
    .coeff_13(coeff_13),
    .coeff_14(coeff_14),
    .coeff_15(coeff_15),
    .ZeroLeft(ZeroLeft),
    .len_comb(len_read_run_befores_comb)
);

//------------------------
// cavlc_len_gen
//------------------------
wire [4:0] len_comb;

cavlc_len_gen cavlc_len_gen(
    .cavlc_state(cavlc_state),
    .len_read_total_coeffs_comb(len_read_total_coeffs_comb),
    .len_read_levels_comb(len_read_levels_comb),
    .len_read_total_zeros_comb(len_read_total_zeros_comb),
    .len_read_run_befores_comb(len_read_run_befores_comb),
    .len_comb(len_comb)
);

//------------------------
// fsm
//------------------------
cavlc_fsm cavlc_fsm(
    .clk(clk),
    .rst_n(rst_n),
    .ena(ena),
    .start(start),
    
    .max_coeff_num(max_coeff_num),
    .TotalCoeff(TotalCoeff),
    .TotalCoeff_comb(TotalCoeff_comb),
    .TrailingOnes(TrailingOnes),
    .TrailingOnes_comb(TrailingOnes_comb),
    .ZeroLeft(ZeroLeft),
    .state(cavlc_state),
    .i(i),
    .idle(idle),
    .valid(valid) 
);

endmodule
