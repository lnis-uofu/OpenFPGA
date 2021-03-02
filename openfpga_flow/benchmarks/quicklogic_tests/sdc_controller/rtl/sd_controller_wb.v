//////////////////////////////////////////////////////////////////////
////                                                              ////
//// WISHBONE SD Card Controller IP Core                          ////
////                                                              ////
//// sd_controller_wb.v                                           ////
////                                                              ////
//// This file is part of the WISHBONE SD Card                    ////
//// Controller IP Core project                                   ////
//// http://opencores.org/project,sd_card_controller              ////
////                                                              ////
//// Description                                                  ////
//// Wishbone interface responsible for comunication with core    ////
////                                                              ////
//// Author(s):                                                   ////
////     - Marek Czerski, ma.czerski@gmail.com                    ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2013 Authors                                   ////
////                                                              ////
//// Based on original work by                                    ////
////     Adam Edvardsson (adam.edvardsson@orsoc.se)               ////
////                                                              ////
////     Copyright (C) 2009 Authors                               ////
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
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
`include "sd_defines.v"

module sd_controller_wb(
           // WISHBONE slave
           wb_clk_i, wb_rst_i, wb_dat_i, wb_dat_o,
           wb_adr_i, wb_sel_i, wb_we_i, wb_cyc_i, wb_stb_i, wb_ack_o,
           cmd_start,
           data_int_rst,
           cmd_int_rst,
           argument_reg,
           command_reg,
           response_0_reg,
           response_1_reg,
           response_2_reg,
           response_3_reg,
           software_reset_reg,
           timeout_reg,
           block_size_reg,
           controll_setting_reg,
           cmd_int_status_reg,
           cmd_int_enable_reg,
           clock_divider_reg,
           block_count_reg,
           dma_addr_reg,
           data_int_status_reg,
           data_int_enable_reg
       );

// WISHBONE common
input wb_clk_i;     // WISHBONE clock
input wb_rst_i;     // WISHBONE reset
input [31:0] wb_dat_i;     // WISHBONE data input
output reg [31:0] wb_dat_o;     // WISHBONE data output
// WISHBONE error output

// WISHBONE slave
input [7:0] wb_adr_i;     // WISHBONE address input
input [3:0] wb_sel_i;     // WISHBONE byte select input
input wb_we_i;      // WISHBONE write enable input
input wb_cyc_i;     // WISHBONE cycle input
input wb_stb_i;     // WISHBONE strobe input
output reg wb_ack_o;     // WISHBONE acknowledge output
output reg cmd_start;
//Buss accessible registers
output reg [31:0] argument_reg;
output reg [`CMD_REG_SIZE-1:0] command_reg;
input wire [31:0] response_0_reg;
input wire [31:0] response_1_reg;
input wire [31:0] response_2_reg;
input wire [31:0] response_3_reg;
output reg [0:0] software_reset_reg;
output reg [15:0] timeout_reg;
output reg [`BLKSIZE_W-1:0] block_size_reg;
output reg [15:0] controll_setting_reg;
input wire [`INT_CMD_SIZE-1:0] cmd_int_status_reg;
output reg [`INT_CMD_SIZE-1:0] cmd_int_enable_reg;
output reg [7:0] clock_divider_reg;
input  wire [`INT_DATA_SIZE-1:0] data_int_status_reg;
output reg [`INT_DATA_SIZE-1:0] data_int_enable_reg;
//Register Controll
output reg data_int_rst;
output reg cmd_int_rst;
output reg [`BLKCNT_W-1:0]block_count_reg;
output reg [31:0] dma_addr_reg;

parameter voltage_controll_reg  = `SUPPLY_VOLTAGE_mV;
parameter capabilies_reg = 16'b0000_0000_0000_0000;

always @(posedge wb_clk_i or posedge wb_rst_i)
begin
    if (wb_rst_i)begin
        argument_reg <= 0;
        command_reg <= 0;
        software_reset_reg <= 0;
        timeout_reg <= 0;
        block_size_reg <= `RESET_BLOCK_SIZE;
        controll_setting_reg <= 0;
        cmd_int_enable_reg <= 0;
        clock_divider_reg <= `RESET_CLK_DIV;
        wb_ack_o <= 0;
        cmd_start <= 0;
        data_int_rst <= 0;
        data_int_enable_reg <= 0;
        cmd_int_rst <= 0;
        block_count_reg <= 0;
        dma_addr_reg <= 0;
    end
    else
    begin
        cmd_start <= 1'b0;
        data_int_rst <= 0;
        cmd_int_rst <= 0;
        if ((wb_stb_i & wb_cyc_i) || wb_ack_o)begin
            if (wb_we_i) begin
                case (wb_adr_i)
                    `argument: begin
                        argument_reg <= wb_dat_i;
                        cmd_start <= 1'b1;
                    end
                    `command: command_reg <= wb_dat_i[`CMD_REG_SIZE-1:0];
                    `reset: software_reset_reg <= wb_dat_i[0];
                    `timeout: timeout_reg  <=  wb_dat_i[15:0];
                    `blksize: block_size_reg <= wb_dat_i[`BLKSIZE_W-1:0];
                    `controller: controll_setting_reg <= wb_dat_i[15:0];
                    `cmd_iser: cmd_int_enable_reg <= wb_dat_i[4:0];
                    `cmd_isr: cmd_int_rst <= 1;
                    `clock_d: clock_divider_reg <= wb_dat_i[7:0];
                    `data_isr: data_int_rst <= 1;
                    `data_iser: data_int_enable_reg <= wb_dat_i[`INT_DATA_SIZE-1:0];
                    `dst_src_addr: dma_addr_reg <= wb_dat_i;
                    `blkcnt: block_count_reg <= wb_dat_i[`BLKCNT_W-1:0];
                endcase
            end
            wb_ack_o <= wb_cyc_i & wb_stb_i & ~wb_ack_o;
        end
    end
end

always @(posedge wb_clk_i or posedge wb_rst_i)begin
    if (wb_rst_i == 1)
        wb_dat_o <= 0;
    else
        if (wb_stb_i & wb_cyc_i) begin //CS
            case (wb_adr_i)
                `argument: wb_dat_o <= argument_reg;
                `command: wb_dat_o <= command_reg;
                `resp0: wb_dat_o <= response_0_reg;
                `resp1: wb_dat_o <= response_1_reg;
                `resp2: wb_dat_o <= response_2_reg;
                `resp3: wb_dat_o <= response_3_reg;
                `controller: wb_dat_o <= controll_setting_reg;
                `blksize: wb_dat_o <= block_size_reg;
                `voltage: wb_dat_o <= voltage_controll_reg;
                `reset: wb_dat_o <= software_reset_reg;
                `timeout: wb_dat_o <= timeout_reg;
                `cmd_isr: wb_dat_o <= cmd_int_status_reg;
                `cmd_iser: wb_dat_o <= cmd_int_enable_reg;
                `clock_d: wb_dat_o <= clock_divider_reg;
                `capa: wb_dat_o <= capabilies_reg;
                `data_isr: wb_dat_o <= data_int_status_reg;
                `blkcnt: wb_dat_o <= block_count_reg;
                `data_iser: wb_dat_o <= data_int_enable_reg;
                `dst_src_addr: wb_dat_o <= dma_addr_reg;
            endcase
        end
end

endmodule
