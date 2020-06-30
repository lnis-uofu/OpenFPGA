//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Top-level Verilog module for FPGA
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:40 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for fpga_top -----
module fpga_top(pReset,
                prog_clk,
                set,
                reset,
                clk,
                gfpga_pad_iopad_pad,
                enable,
                address,
                data_in);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- GLOBAL PORTS -----
input [0:0] set;
//----- GLOBAL PORTS -----
input [0:0] reset;
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- GPIO PORTS -----
inout [0:63] gfpga_pad_iopad_pad;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:15] address;
//----- INPUT PORTS -----
input [0:0] data_in;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] cbx_1__0__0_bottom_grid_pin_0_;
wire [0:0] cbx_1__0__0_bottom_grid_pin_10_;
wire [0:0] cbx_1__0__0_bottom_grid_pin_12_;
wire [0:0] cbx_1__0__0_bottom_grid_pin_14_;
wire [0:0] cbx_1__0__0_bottom_grid_pin_2_;
wire [0:0] cbx_1__0__0_bottom_grid_pin_4_;
wire [0:0] cbx_1__0__0_bottom_grid_pin_6_;
wire [0:0] cbx_1__0__0_bottom_grid_pin_8_;
wire [0:0] cbx_1__0__0_chanx_out_0_;
wire [0:0] cbx_1__0__0_chanx_out_10_;
wire [0:0] cbx_1__0__0_chanx_out_11_;
wire [0:0] cbx_1__0__0_chanx_out_12_;
wire [0:0] cbx_1__0__0_chanx_out_13_;
wire [0:0] cbx_1__0__0_chanx_out_14_;
wire [0:0] cbx_1__0__0_chanx_out_15_;
wire [0:0] cbx_1__0__0_chanx_out_16_;
wire [0:0] cbx_1__0__0_chanx_out_17_;
wire [0:0] cbx_1__0__0_chanx_out_1_;
wire [0:0] cbx_1__0__0_chanx_out_2_;
wire [0:0] cbx_1__0__0_chanx_out_3_;
wire [0:0] cbx_1__0__0_chanx_out_4_;
wire [0:0] cbx_1__0__0_chanx_out_5_;
wire [0:0] cbx_1__0__0_chanx_out_6_;
wire [0:0] cbx_1__0__0_chanx_out_7_;
wire [0:0] cbx_1__0__0_chanx_out_8_;
wire [0:0] cbx_1__0__0_chanx_out_9_;
wire [0:0] cbx_1__0__0_top_grid_pin_14_;
wire [0:0] cbx_1__0__0_top_grid_pin_2_;
wire [0:0] cbx_1__0__0_top_grid_pin_6_;
wire [0:0] cbx_1__0__1_bottom_grid_pin_0_;
wire [0:0] cbx_1__0__1_bottom_grid_pin_10_;
wire [0:0] cbx_1__0__1_bottom_grid_pin_12_;
wire [0:0] cbx_1__0__1_bottom_grid_pin_14_;
wire [0:0] cbx_1__0__1_bottom_grid_pin_2_;
wire [0:0] cbx_1__0__1_bottom_grid_pin_4_;
wire [0:0] cbx_1__0__1_bottom_grid_pin_6_;
wire [0:0] cbx_1__0__1_bottom_grid_pin_8_;
wire [0:0] cbx_1__0__1_chanx_out_0_;
wire [0:0] cbx_1__0__1_chanx_out_10_;
wire [0:0] cbx_1__0__1_chanx_out_11_;
wire [0:0] cbx_1__0__1_chanx_out_12_;
wire [0:0] cbx_1__0__1_chanx_out_13_;
wire [0:0] cbx_1__0__1_chanx_out_14_;
wire [0:0] cbx_1__0__1_chanx_out_15_;
wire [0:0] cbx_1__0__1_chanx_out_16_;
wire [0:0] cbx_1__0__1_chanx_out_17_;
wire [0:0] cbx_1__0__1_chanx_out_1_;
wire [0:0] cbx_1__0__1_chanx_out_2_;
wire [0:0] cbx_1__0__1_chanx_out_3_;
wire [0:0] cbx_1__0__1_chanx_out_4_;
wire [0:0] cbx_1__0__1_chanx_out_5_;
wire [0:0] cbx_1__0__1_chanx_out_6_;
wire [0:0] cbx_1__0__1_chanx_out_7_;
wire [0:0] cbx_1__0__1_chanx_out_8_;
wire [0:0] cbx_1__0__1_chanx_out_9_;
wire [0:0] cbx_1__0__1_top_grid_pin_14_;
wire [0:0] cbx_1__0__1_top_grid_pin_2_;
wire [0:0] cbx_1__0__1_top_grid_pin_6_;
wire [0:0] cbx_1__1__0_bottom_grid_pin_0_;
wire [0:0] cbx_1__1__0_bottom_grid_pin_4_;
wire [0:0] cbx_1__1__0_bottom_grid_pin_8_;
wire [0:0] cbx_1__1__0_chanx_out_0_;
wire [0:0] cbx_1__1__0_chanx_out_10_;
wire [0:0] cbx_1__1__0_chanx_out_11_;
wire [0:0] cbx_1__1__0_chanx_out_12_;
wire [0:0] cbx_1__1__0_chanx_out_13_;
wire [0:0] cbx_1__1__0_chanx_out_14_;
wire [0:0] cbx_1__1__0_chanx_out_15_;
wire [0:0] cbx_1__1__0_chanx_out_16_;
wire [0:0] cbx_1__1__0_chanx_out_17_;
wire [0:0] cbx_1__1__0_chanx_out_1_;
wire [0:0] cbx_1__1__0_chanx_out_2_;
wire [0:0] cbx_1__1__0_chanx_out_3_;
wire [0:0] cbx_1__1__0_chanx_out_4_;
wire [0:0] cbx_1__1__0_chanx_out_5_;
wire [0:0] cbx_1__1__0_chanx_out_6_;
wire [0:0] cbx_1__1__0_chanx_out_7_;
wire [0:0] cbx_1__1__0_chanx_out_8_;
wire [0:0] cbx_1__1__0_chanx_out_9_;
wire [0:0] cbx_1__1__0_top_grid_pin_14_;
wire [0:0] cbx_1__1__0_top_grid_pin_2_;
wire [0:0] cbx_1__1__0_top_grid_pin_6_;
wire [0:0] cbx_1__1__1_bottom_grid_pin_0_;
wire [0:0] cbx_1__1__1_bottom_grid_pin_4_;
wire [0:0] cbx_1__1__1_bottom_grid_pin_8_;
wire [0:0] cbx_1__1__1_chanx_out_0_;
wire [0:0] cbx_1__1__1_chanx_out_10_;
wire [0:0] cbx_1__1__1_chanx_out_11_;
wire [0:0] cbx_1__1__1_chanx_out_12_;
wire [0:0] cbx_1__1__1_chanx_out_13_;
wire [0:0] cbx_1__1__1_chanx_out_14_;
wire [0:0] cbx_1__1__1_chanx_out_15_;
wire [0:0] cbx_1__1__1_chanx_out_16_;
wire [0:0] cbx_1__1__1_chanx_out_17_;
wire [0:0] cbx_1__1__1_chanx_out_1_;
wire [0:0] cbx_1__1__1_chanx_out_2_;
wire [0:0] cbx_1__1__1_chanx_out_3_;
wire [0:0] cbx_1__1__1_chanx_out_4_;
wire [0:0] cbx_1__1__1_chanx_out_5_;
wire [0:0] cbx_1__1__1_chanx_out_6_;
wire [0:0] cbx_1__1__1_chanx_out_7_;
wire [0:0] cbx_1__1__1_chanx_out_8_;
wire [0:0] cbx_1__1__1_chanx_out_9_;
wire [0:0] cbx_1__1__1_top_grid_pin_14_;
wire [0:0] cbx_1__1__1_top_grid_pin_2_;
wire [0:0] cbx_1__1__1_top_grid_pin_6_;
wire [0:0] cbx_1__2__0_bottom_grid_pin_0_;
wire [0:0] cbx_1__2__0_bottom_grid_pin_4_;
wire [0:0] cbx_1__2__0_bottom_grid_pin_8_;
wire [0:0] cbx_1__2__0_chanx_out_0_;
wire [0:0] cbx_1__2__0_chanx_out_10_;
wire [0:0] cbx_1__2__0_chanx_out_11_;
wire [0:0] cbx_1__2__0_chanx_out_12_;
wire [0:0] cbx_1__2__0_chanx_out_13_;
wire [0:0] cbx_1__2__0_chanx_out_14_;
wire [0:0] cbx_1__2__0_chanx_out_15_;
wire [0:0] cbx_1__2__0_chanx_out_16_;
wire [0:0] cbx_1__2__0_chanx_out_17_;
wire [0:0] cbx_1__2__0_chanx_out_1_;
wire [0:0] cbx_1__2__0_chanx_out_2_;
wire [0:0] cbx_1__2__0_chanx_out_3_;
wire [0:0] cbx_1__2__0_chanx_out_4_;
wire [0:0] cbx_1__2__0_chanx_out_5_;
wire [0:0] cbx_1__2__0_chanx_out_6_;
wire [0:0] cbx_1__2__0_chanx_out_7_;
wire [0:0] cbx_1__2__0_chanx_out_8_;
wire [0:0] cbx_1__2__0_chanx_out_9_;
wire [0:0] cbx_1__2__0_top_grid_pin_0_;
wire [0:0] cbx_1__2__0_top_grid_pin_10_;
wire [0:0] cbx_1__2__0_top_grid_pin_12_;
wire [0:0] cbx_1__2__0_top_grid_pin_14_;
wire [0:0] cbx_1__2__0_top_grid_pin_2_;
wire [0:0] cbx_1__2__0_top_grid_pin_4_;
wire [0:0] cbx_1__2__0_top_grid_pin_6_;
wire [0:0] cbx_1__2__0_top_grid_pin_8_;
wire [0:0] cbx_1__2__1_bottom_grid_pin_0_;
wire [0:0] cbx_1__2__1_bottom_grid_pin_4_;
wire [0:0] cbx_1__2__1_bottom_grid_pin_8_;
wire [0:0] cbx_1__2__1_chanx_out_0_;
wire [0:0] cbx_1__2__1_chanx_out_10_;
wire [0:0] cbx_1__2__1_chanx_out_11_;
wire [0:0] cbx_1__2__1_chanx_out_12_;
wire [0:0] cbx_1__2__1_chanx_out_13_;
wire [0:0] cbx_1__2__1_chanx_out_14_;
wire [0:0] cbx_1__2__1_chanx_out_15_;
wire [0:0] cbx_1__2__1_chanx_out_16_;
wire [0:0] cbx_1__2__1_chanx_out_17_;
wire [0:0] cbx_1__2__1_chanx_out_1_;
wire [0:0] cbx_1__2__1_chanx_out_2_;
wire [0:0] cbx_1__2__1_chanx_out_3_;
wire [0:0] cbx_1__2__1_chanx_out_4_;
wire [0:0] cbx_1__2__1_chanx_out_5_;
wire [0:0] cbx_1__2__1_chanx_out_6_;
wire [0:0] cbx_1__2__1_chanx_out_7_;
wire [0:0] cbx_1__2__1_chanx_out_8_;
wire [0:0] cbx_1__2__1_chanx_out_9_;
wire [0:0] cbx_1__2__1_top_grid_pin_0_;
wire [0:0] cbx_1__2__1_top_grid_pin_10_;
wire [0:0] cbx_1__2__1_top_grid_pin_12_;
wire [0:0] cbx_1__2__1_top_grid_pin_14_;
wire [0:0] cbx_1__2__1_top_grid_pin_2_;
wire [0:0] cbx_1__2__1_top_grid_pin_4_;
wire [0:0] cbx_1__2__1_top_grid_pin_6_;
wire [0:0] cbx_1__2__1_top_grid_pin_8_;
wire [0:0] cby_0__1__0_chany_out_0_;
wire [0:0] cby_0__1__0_chany_out_10_;
wire [0:0] cby_0__1__0_chany_out_11_;
wire [0:0] cby_0__1__0_chany_out_12_;
wire [0:0] cby_0__1__0_chany_out_13_;
wire [0:0] cby_0__1__0_chany_out_14_;
wire [0:0] cby_0__1__0_chany_out_15_;
wire [0:0] cby_0__1__0_chany_out_16_;
wire [0:0] cby_0__1__0_chany_out_17_;
wire [0:0] cby_0__1__0_chany_out_1_;
wire [0:0] cby_0__1__0_chany_out_2_;
wire [0:0] cby_0__1__0_chany_out_3_;
wire [0:0] cby_0__1__0_chany_out_4_;
wire [0:0] cby_0__1__0_chany_out_5_;
wire [0:0] cby_0__1__0_chany_out_6_;
wire [0:0] cby_0__1__0_chany_out_7_;
wire [0:0] cby_0__1__0_chany_out_8_;
wire [0:0] cby_0__1__0_chany_out_9_;
wire [0:0] cby_0__1__0_left_grid_pin_0_;
wire [0:0] cby_0__1__0_left_grid_pin_10_;
wire [0:0] cby_0__1__0_left_grid_pin_12_;
wire [0:0] cby_0__1__0_left_grid_pin_14_;
wire [0:0] cby_0__1__0_left_grid_pin_2_;
wire [0:0] cby_0__1__0_left_grid_pin_4_;
wire [0:0] cby_0__1__0_left_grid_pin_6_;
wire [0:0] cby_0__1__0_left_grid_pin_8_;
wire [0:0] cby_0__1__0_right_grid_pin_3_;
wire [0:0] cby_0__1__0_right_grid_pin_7_;
wire [0:0] cby_0__1__1_chany_out_0_;
wire [0:0] cby_0__1__1_chany_out_10_;
wire [0:0] cby_0__1__1_chany_out_11_;
wire [0:0] cby_0__1__1_chany_out_12_;
wire [0:0] cby_0__1__1_chany_out_13_;
wire [0:0] cby_0__1__1_chany_out_14_;
wire [0:0] cby_0__1__1_chany_out_15_;
wire [0:0] cby_0__1__1_chany_out_16_;
wire [0:0] cby_0__1__1_chany_out_17_;
wire [0:0] cby_0__1__1_chany_out_1_;
wire [0:0] cby_0__1__1_chany_out_2_;
wire [0:0] cby_0__1__1_chany_out_3_;
wire [0:0] cby_0__1__1_chany_out_4_;
wire [0:0] cby_0__1__1_chany_out_5_;
wire [0:0] cby_0__1__1_chany_out_6_;
wire [0:0] cby_0__1__1_chany_out_7_;
wire [0:0] cby_0__1__1_chany_out_8_;
wire [0:0] cby_0__1__1_chany_out_9_;
wire [0:0] cby_0__1__1_left_grid_pin_0_;
wire [0:0] cby_0__1__1_left_grid_pin_10_;
wire [0:0] cby_0__1__1_left_grid_pin_12_;
wire [0:0] cby_0__1__1_left_grid_pin_14_;
wire [0:0] cby_0__1__1_left_grid_pin_2_;
wire [0:0] cby_0__1__1_left_grid_pin_4_;
wire [0:0] cby_0__1__1_left_grid_pin_6_;
wire [0:0] cby_0__1__1_left_grid_pin_8_;
wire [0:0] cby_0__1__1_right_grid_pin_3_;
wire [0:0] cby_0__1__1_right_grid_pin_7_;
wire [0:0] cby_1__1__0_chany_out_0_;
wire [0:0] cby_1__1__0_chany_out_10_;
wire [0:0] cby_1__1__0_chany_out_11_;
wire [0:0] cby_1__1__0_chany_out_12_;
wire [0:0] cby_1__1__0_chany_out_13_;
wire [0:0] cby_1__1__0_chany_out_14_;
wire [0:0] cby_1__1__0_chany_out_15_;
wire [0:0] cby_1__1__0_chany_out_16_;
wire [0:0] cby_1__1__0_chany_out_17_;
wire [0:0] cby_1__1__0_chany_out_1_;
wire [0:0] cby_1__1__0_chany_out_2_;
wire [0:0] cby_1__1__0_chany_out_3_;
wire [0:0] cby_1__1__0_chany_out_4_;
wire [0:0] cby_1__1__0_chany_out_5_;
wire [0:0] cby_1__1__0_chany_out_6_;
wire [0:0] cby_1__1__0_chany_out_7_;
wire [0:0] cby_1__1__0_chany_out_8_;
wire [0:0] cby_1__1__0_chany_out_9_;
wire [0:0] cby_1__1__0_left_grid_pin_1_;
wire [0:0] cby_1__1__0_left_grid_pin_5_;
wire [0:0] cby_1__1__0_left_grid_pin_9_;
wire [0:0] cby_1__1__0_right_grid_pin_3_;
wire [0:0] cby_1__1__0_right_grid_pin_7_;
wire [0:0] cby_1__1__1_chany_out_0_;
wire [0:0] cby_1__1__1_chany_out_10_;
wire [0:0] cby_1__1__1_chany_out_11_;
wire [0:0] cby_1__1__1_chany_out_12_;
wire [0:0] cby_1__1__1_chany_out_13_;
wire [0:0] cby_1__1__1_chany_out_14_;
wire [0:0] cby_1__1__1_chany_out_15_;
wire [0:0] cby_1__1__1_chany_out_16_;
wire [0:0] cby_1__1__1_chany_out_17_;
wire [0:0] cby_1__1__1_chany_out_1_;
wire [0:0] cby_1__1__1_chany_out_2_;
wire [0:0] cby_1__1__1_chany_out_3_;
wire [0:0] cby_1__1__1_chany_out_4_;
wire [0:0] cby_1__1__1_chany_out_5_;
wire [0:0] cby_1__1__1_chany_out_6_;
wire [0:0] cby_1__1__1_chany_out_7_;
wire [0:0] cby_1__1__1_chany_out_8_;
wire [0:0] cby_1__1__1_chany_out_9_;
wire [0:0] cby_1__1__1_left_grid_pin_1_;
wire [0:0] cby_1__1__1_left_grid_pin_5_;
wire [0:0] cby_1__1__1_left_grid_pin_9_;
wire [0:0] cby_1__1__1_right_grid_pin_3_;
wire [0:0] cby_1__1__1_right_grid_pin_7_;
wire [0:0] cby_2__1__0_chany_out_0_;
wire [0:0] cby_2__1__0_chany_out_10_;
wire [0:0] cby_2__1__0_chany_out_11_;
wire [0:0] cby_2__1__0_chany_out_12_;
wire [0:0] cby_2__1__0_chany_out_13_;
wire [0:0] cby_2__1__0_chany_out_14_;
wire [0:0] cby_2__1__0_chany_out_15_;
wire [0:0] cby_2__1__0_chany_out_16_;
wire [0:0] cby_2__1__0_chany_out_17_;
wire [0:0] cby_2__1__0_chany_out_1_;
wire [0:0] cby_2__1__0_chany_out_2_;
wire [0:0] cby_2__1__0_chany_out_3_;
wire [0:0] cby_2__1__0_chany_out_4_;
wire [0:0] cby_2__1__0_chany_out_5_;
wire [0:0] cby_2__1__0_chany_out_6_;
wire [0:0] cby_2__1__0_chany_out_7_;
wire [0:0] cby_2__1__0_chany_out_8_;
wire [0:0] cby_2__1__0_chany_out_9_;
wire [0:0] cby_2__1__0_left_grid_pin_1_;
wire [0:0] cby_2__1__0_left_grid_pin_5_;
wire [0:0] cby_2__1__0_left_grid_pin_9_;
wire [0:0] cby_2__1__0_right_grid_pin_0_;
wire [0:0] cby_2__1__0_right_grid_pin_10_;
wire [0:0] cby_2__1__0_right_grid_pin_12_;
wire [0:0] cby_2__1__0_right_grid_pin_14_;
wire [0:0] cby_2__1__0_right_grid_pin_2_;
wire [0:0] cby_2__1__0_right_grid_pin_4_;
wire [0:0] cby_2__1__0_right_grid_pin_6_;
wire [0:0] cby_2__1__0_right_grid_pin_8_;
wire [0:0] cby_2__1__1_chany_out_0_;
wire [0:0] cby_2__1__1_chany_out_10_;
wire [0:0] cby_2__1__1_chany_out_11_;
wire [0:0] cby_2__1__1_chany_out_12_;
wire [0:0] cby_2__1__1_chany_out_13_;
wire [0:0] cby_2__1__1_chany_out_14_;
wire [0:0] cby_2__1__1_chany_out_15_;
wire [0:0] cby_2__1__1_chany_out_16_;
wire [0:0] cby_2__1__1_chany_out_17_;
wire [0:0] cby_2__1__1_chany_out_1_;
wire [0:0] cby_2__1__1_chany_out_2_;
wire [0:0] cby_2__1__1_chany_out_3_;
wire [0:0] cby_2__1__1_chany_out_4_;
wire [0:0] cby_2__1__1_chany_out_5_;
wire [0:0] cby_2__1__1_chany_out_6_;
wire [0:0] cby_2__1__1_chany_out_7_;
wire [0:0] cby_2__1__1_chany_out_8_;
wire [0:0] cby_2__1__1_chany_out_9_;
wire [0:0] cby_2__1__1_left_grid_pin_1_;
wire [0:0] cby_2__1__1_left_grid_pin_5_;
wire [0:0] cby_2__1__1_left_grid_pin_9_;
wire [0:0] cby_2__1__1_right_grid_pin_0_;
wire [0:0] cby_2__1__1_right_grid_pin_10_;
wire [0:0] cby_2__1__1_right_grid_pin_12_;
wire [0:0] cby_2__1__1_right_grid_pin_14_;
wire [0:0] cby_2__1__1_right_grid_pin_2_;
wire [0:0] cby_2__1__1_right_grid_pin_4_;
wire [0:0] cby_2__1__1_right_grid_pin_6_;
wire [0:0] cby_2__1__1_right_grid_pin_8_;
wire [0:32] decoder6to33_0_data_out;
wire [0:0] grid_clb_0_bottom_width_0_height_0__pin_10_;
wire [0:0] grid_clb_0_left_width_0_height_0__pin_11_;
wire [0:0] grid_clb_0_right_width_0_height_0__pin_13_;
wire [0:0] grid_clb_0_top_width_0_height_0__pin_12_;
wire [0:0] grid_clb_1_bottom_width_0_height_0__pin_10_;
wire [0:0] grid_clb_1_left_width_0_height_0__pin_11_;
wire [0:0] grid_clb_1_right_width_0_height_0__pin_13_;
wire [0:0] grid_clb_1_top_width_0_height_0__pin_12_;
wire [0:0] grid_clb_2_bottom_width_0_height_0__pin_10_;
wire [0:0] grid_clb_2_left_width_0_height_0__pin_11_;
wire [0:0] grid_clb_2_right_width_0_height_0__pin_13_;
wire [0:0] grid_clb_2_top_width_0_height_0__pin_12_;
wire [0:0] grid_clb_3_bottom_width_0_height_0__pin_10_;
wire [0:0] grid_clb_3_left_width_0_height_0__pin_11_;
wire [0:0] grid_clb_3_right_width_0_height_0__pin_13_;
wire [0:0] grid_clb_3_top_width_0_height_0__pin_12_;
wire [0:0] grid_io_bottom_0_top_width_0_height_0__pin_11_;
wire [0:0] grid_io_bottom_0_top_width_0_height_0__pin_13_;
wire [0:0] grid_io_bottom_0_top_width_0_height_0__pin_15_;
wire [0:0] grid_io_bottom_0_top_width_0_height_0__pin_1_;
wire [0:0] grid_io_bottom_0_top_width_0_height_0__pin_3_;
wire [0:0] grid_io_bottom_0_top_width_0_height_0__pin_5_;
wire [0:0] grid_io_bottom_0_top_width_0_height_0__pin_7_;
wire [0:0] grid_io_bottom_0_top_width_0_height_0__pin_9_;
wire [0:0] grid_io_bottom_1_top_width_0_height_0__pin_11_;
wire [0:0] grid_io_bottom_1_top_width_0_height_0__pin_13_;
wire [0:0] grid_io_bottom_1_top_width_0_height_0__pin_15_;
wire [0:0] grid_io_bottom_1_top_width_0_height_0__pin_1_;
wire [0:0] grid_io_bottom_1_top_width_0_height_0__pin_3_;
wire [0:0] grid_io_bottom_1_top_width_0_height_0__pin_5_;
wire [0:0] grid_io_bottom_1_top_width_0_height_0__pin_7_;
wire [0:0] grid_io_bottom_1_top_width_0_height_0__pin_9_;
wire [0:0] grid_io_left_0_right_width_0_height_0__pin_11_;
wire [0:0] grid_io_left_0_right_width_0_height_0__pin_13_;
wire [0:0] grid_io_left_0_right_width_0_height_0__pin_15_;
wire [0:0] grid_io_left_0_right_width_0_height_0__pin_1_;
wire [0:0] grid_io_left_0_right_width_0_height_0__pin_3_;
wire [0:0] grid_io_left_0_right_width_0_height_0__pin_5_;
wire [0:0] grid_io_left_0_right_width_0_height_0__pin_7_;
wire [0:0] grid_io_left_0_right_width_0_height_0__pin_9_;
wire [0:0] grid_io_left_1_right_width_0_height_0__pin_11_;
wire [0:0] grid_io_left_1_right_width_0_height_0__pin_13_;
wire [0:0] grid_io_left_1_right_width_0_height_0__pin_15_;
wire [0:0] grid_io_left_1_right_width_0_height_0__pin_1_;
wire [0:0] grid_io_left_1_right_width_0_height_0__pin_3_;
wire [0:0] grid_io_left_1_right_width_0_height_0__pin_5_;
wire [0:0] grid_io_left_1_right_width_0_height_0__pin_7_;
wire [0:0] grid_io_left_1_right_width_0_height_0__pin_9_;
wire [0:0] grid_io_right_0_left_width_0_height_0__pin_11_;
wire [0:0] grid_io_right_0_left_width_0_height_0__pin_13_;
wire [0:0] grid_io_right_0_left_width_0_height_0__pin_15_;
wire [0:0] grid_io_right_0_left_width_0_height_0__pin_1_;
wire [0:0] grid_io_right_0_left_width_0_height_0__pin_3_;
wire [0:0] grid_io_right_0_left_width_0_height_0__pin_5_;
wire [0:0] grid_io_right_0_left_width_0_height_0__pin_7_;
wire [0:0] grid_io_right_0_left_width_0_height_0__pin_9_;
wire [0:0] grid_io_right_1_left_width_0_height_0__pin_11_;
wire [0:0] grid_io_right_1_left_width_0_height_0__pin_13_;
wire [0:0] grid_io_right_1_left_width_0_height_0__pin_15_;
wire [0:0] grid_io_right_1_left_width_0_height_0__pin_1_;
wire [0:0] grid_io_right_1_left_width_0_height_0__pin_3_;
wire [0:0] grid_io_right_1_left_width_0_height_0__pin_5_;
wire [0:0] grid_io_right_1_left_width_0_height_0__pin_7_;
wire [0:0] grid_io_right_1_left_width_0_height_0__pin_9_;
wire [0:0] grid_io_top_0_bottom_width_0_height_0__pin_11_;
wire [0:0] grid_io_top_0_bottom_width_0_height_0__pin_13_;
wire [0:0] grid_io_top_0_bottom_width_0_height_0__pin_15_;
wire [0:0] grid_io_top_0_bottom_width_0_height_0__pin_1_;
wire [0:0] grid_io_top_0_bottom_width_0_height_0__pin_3_;
wire [0:0] grid_io_top_0_bottom_width_0_height_0__pin_5_;
wire [0:0] grid_io_top_0_bottom_width_0_height_0__pin_7_;
wire [0:0] grid_io_top_0_bottom_width_0_height_0__pin_9_;
wire [0:0] grid_io_top_1_bottom_width_0_height_0__pin_11_;
wire [0:0] grid_io_top_1_bottom_width_0_height_0__pin_13_;
wire [0:0] grid_io_top_1_bottom_width_0_height_0__pin_15_;
wire [0:0] grid_io_top_1_bottom_width_0_height_0__pin_1_;
wire [0:0] grid_io_top_1_bottom_width_0_height_0__pin_3_;
wire [0:0] grid_io_top_1_bottom_width_0_height_0__pin_5_;
wire [0:0] grid_io_top_1_bottom_width_0_height_0__pin_7_;
wire [0:0] grid_io_top_1_bottom_width_0_height_0__pin_9_;
wire [0:0] sb_0__0__0_chanx_right_out_0_;
wire [0:0] sb_0__0__0_chanx_right_out_10_;
wire [0:0] sb_0__0__0_chanx_right_out_12_;
wire [0:0] sb_0__0__0_chanx_right_out_14_;
wire [0:0] sb_0__0__0_chanx_right_out_16_;
wire [0:0] sb_0__0__0_chanx_right_out_2_;
wire [0:0] sb_0__0__0_chanx_right_out_4_;
wire [0:0] sb_0__0__0_chanx_right_out_6_;
wire [0:0] sb_0__0__0_chanx_right_out_8_;
wire [0:0] sb_0__0__0_chany_top_out_0_;
wire [0:0] sb_0__0__0_chany_top_out_10_;
wire [0:0] sb_0__0__0_chany_top_out_12_;
wire [0:0] sb_0__0__0_chany_top_out_14_;
wire [0:0] sb_0__0__0_chany_top_out_16_;
wire [0:0] sb_0__0__0_chany_top_out_2_;
wire [0:0] sb_0__0__0_chany_top_out_4_;
wire [0:0] sb_0__0__0_chany_top_out_6_;
wire [0:0] sb_0__0__0_chany_top_out_8_;
wire [0:0] sb_0__1__0_chanx_right_out_0_;
wire [0:0] sb_0__1__0_chanx_right_out_10_;
wire [0:0] sb_0__1__0_chanx_right_out_12_;
wire [0:0] sb_0__1__0_chanx_right_out_14_;
wire [0:0] sb_0__1__0_chanx_right_out_16_;
wire [0:0] sb_0__1__0_chanx_right_out_2_;
wire [0:0] sb_0__1__0_chanx_right_out_4_;
wire [0:0] sb_0__1__0_chanx_right_out_6_;
wire [0:0] sb_0__1__0_chanx_right_out_8_;
wire [0:0] sb_0__1__0_chany_bottom_out_11_;
wire [0:0] sb_0__1__0_chany_bottom_out_13_;
wire [0:0] sb_0__1__0_chany_bottom_out_15_;
wire [0:0] sb_0__1__0_chany_bottom_out_17_;
wire [0:0] sb_0__1__0_chany_bottom_out_1_;
wire [0:0] sb_0__1__0_chany_bottom_out_3_;
wire [0:0] sb_0__1__0_chany_bottom_out_5_;
wire [0:0] sb_0__1__0_chany_bottom_out_7_;
wire [0:0] sb_0__1__0_chany_bottom_out_9_;
wire [0:0] sb_0__1__0_chany_top_out_0_;
wire [0:0] sb_0__1__0_chany_top_out_10_;
wire [0:0] sb_0__1__0_chany_top_out_12_;
wire [0:0] sb_0__1__0_chany_top_out_14_;
wire [0:0] sb_0__1__0_chany_top_out_16_;
wire [0:0] sb_0__1__0_chany_top_out_2_;
wire [0:0] sb_0__1__0_chany_top_out_4_;
wire [0:0] sb_0__1__0_chany_top_out_6_;
wire [0:0] sb_0__1__0_chany_top_out_8_;
wire [0:0] sb_0__2__0_chanx_right_out_0_;
wire [0:0] sb_0__2__0_chanx_right_out_10_;
wire [0:0] sb_0__2__0_chanx_right_out_12_;
wire [0:0] sb_0__2__0_chanx_right_out_14_;
wire [0:0] sb_0__2__0_chanx_right_out_16_;
wire [0:0] sb_0__2__0_chanx_right_out_2_;
wire [0:0] sb_0__2__0_chanx_right_out_4_;
wire [0:0] sb_0__2__0_chanx_right_out_6_;
wire [0:0] sb_0__2__0_chanx_right_out_8_;
wire [0:0] sb_0__2__0_chany_bottom_out_11_;
wire [0:0] sb_0__2__0_chany_bottom_out_13_;
wire [0:0] sb_0__2__0_chany_bottom_out_15_;
wire [0:0] sb_0__2__0_chany_bottom_out_17_;
wire [0:0] sb_0__2__0_chany_bottom_out_1_;
wire [0:0] sb_0__2__0_chany_bottom_out_3_;
wire [0:0] sb_0__2__0_chany_bottom_out_5_;
wire [0:0] sb_0__2__0_chany_bottom_out_7_;
wire [0:0] sb_0__2__0_chany_bottom_out_9_;
wire [0:0] sb_1__0__0_chanx_left_out_11_;
wire [0:0] sb_1__0__0_chanx_left_out_13_;
wire [0:0] sb_1__0__0_chanx_left_out_15_;
wire [0:0] sb_1__0__0_chanx_left_out_17_;
wire [0:0] sb_1__0__0_chanx_left_out_1_;
wire [0:0] sb_1__0__0_chanx_left_out_3_;
wire [0:0] sb_1__0__0_chanx_left_out_5_;
wire [0:0] sb_1__0__0_chanx_left_out_7_;
wire [0:0] sb_1__0__0_chanx_left_out_9_;
wire [0:0] sb_1__0__0_chanx_right_out_0_;
wire [0:0] sb_1__0__0_chanx_right_out_10_;
wire [0:0] sb_1__0__0_chanx_right_out_12_;
wire [0:0] sb_1__0__0_chanx_right_out_14_;
wire [0:0] sb_1__0__0_chanx_right_out_16_;
wire [0:0] sb_1__0__0_chanx_right_out_2_;
wire [0:0] sb_1__0__0_chanx_right_out_4_;
wire [0:0] sb_1__0__0_chanx_right_out_6_;
wire [0:0] sb_1__0__0_chanx_right_out_8_;
wire [0:0] sb_1__0__0_chany_top_out_0_;
wire [0:0] sb_1__0__0_chany_top_out_10_;
wire [0:0] sb_1__0__0_chany_top_out_12_;
wire [0:0] sb_1__0__0_chany_top_out_14_;
wire [0:0] sb_1__0__0_chany_top_out_16_;
wire [0:0] sb_1__0__0_chany_top_out_2_;
wire [0:0] sb_1__0__0_chany_top_out_4_;
wire [0:0] sb_1__0__0_chany_top_out_6_;
wire [0:0] sb_1__0__0_chany_top_out_8_;
wire [0:0] sb_1__1__0_chanx_left_out_11_;
wire [0:0] sb_1__1__0_chanx_left_out_13_;
wire [0:0] sb_1__1__0_chanx_left_out_15_;
wire [0:0] sb_1__1__0_chanx_left_out_17_;
wire [0:0] sb_1__1__0_chanx_left_out_1_;
wire [0:0] sb_1__1__0_chanx_left_out_3_;
wire [0:0] sb_1__1__0_chanx_left_out_5_;
wire [0:0] sb_1__1__0_chanx_left_out_7_;
wire [0:0] sb_1__1__0_chanx_left_out_9_;
wire [0:0] sb_1__1__0_chanx_right_out_0_;
wire [0:0] sb_1__1__0_chanx_right_out_10_;
wire [0:0] sb_1__1__0_chanx_right_out_12_;
wire [0:0] sb_1__1__0_chanx_right_out_14_;
wire [0:0] sb_1__1__0_chanx_right_out_16_;
wire [0:0] sb_1__1__0_chanx_right_out_2_;
wire [0:0] sb_1__1__0_chanx_right_out_4_;
wire [0:0] sb_1__1__0_chanx_right_out_6_;
wire [0:0] sb_1__1__0_chanx_right_out_8_;
wire [0:0] sb_1__1__0_chany_bottom_out_11_;
wire [0:0] sb_1__1__0_chany_bottom_out_13_;
wire [0:0] sb_1__1__0_chany_bottom_out_15_;
wire [0:0] sb_1__1__0_chany_bottom_out_17_;
wire [0:0] sb_1__1__0_chany_bottom_out_1_;
wire [0:0] sb_1__1__0_chany_bottom_out_3_;
wire [0:0] sb_1__1__0_chany_bottom_out_5_;
wire [0:0] sb_1__1__0_chany_bottom_out_7_;
wire [0:0] sb_1__1__0_chany_bottom_out_9_;
wire [0:0] sb_1__1__0_chany_top_out_0_;
wire [0:0] sb_1__1__0_chany_top_out_10_;
wire [0:0] sb_1__1__0_chany_top_out_12_;
wire [0:0] sb_1__1__0_chany_top_out_14_;
wire [0:0] sb_1__1__0_chany_top_out_16_;
wire [0:0] sb_1__1__0_chany_top_out_2_;
wire [0:0] sb_1__1__0_chany_top_out_4_;
wire [0:0] sb_1__1__0_chany_top_out_6_;
wire [0:0] sb_1__1__0_chany_top_out_8_;
wire [0:0] sb_1__2__0_chanx_left_out_11_;
wire [0:0] sb_1__2__0_chanx_left_out_13_;
wire [0:0] sb_1__2__0_chanx_left_out_15_;
wire [0:0] sb_1__2__0_chanx_left_out_17_;
wire [0:0] sb_1__2__0_chanx_left_out_1_;
wire [0:0] sb_1__2__0_chanx_left_out_3_;
wire [0:0] sb_1__2__0_chanx_left_out_5_;
wire [0:0] sb_1__2__0_chanx_left_out_7_;
wire [0:0] sb_1__2__0_chanx_left_out_9_;
wire [0:0] sb_1__2__0_chanx_right_out_0_;
wire [0:0] sb_1__2__0_chanx_right_out_10_;
wire [0:0] sb_1__2__0_chanx_right_out_12_;
wire [0:0] sb_1__2__0_chanx_right_out_14_;
wire [0:0] sb_1__2__0_chanx_right_out_16_;
wire [0:0] sb_1__2__0_chanx_right_out_2_;
wire [0:0] sb_1__2__0_chanx_right_out_4_;
wire [0:0] sb_1__2__0_chanx_right_out_6_;
wire [0:0] sb_1__2__0_chanx_right_out_8_;
wire [0:0] sb_1__2__0_chany_bottom_out_11_;
wire [0:0] sb_1__2__0_chany_bottom_out_13_;
wire [0:0] sb_1__2__0_chany_bottom_out_15_;
wire [0:0] sb_1__2__0_chany_bottom_out_17_;
wire [0:0] sb_1__2__0_chany_bottom_out_1_;
wire [0:0] sb_1__2__0_chany_bottom_out_3_;
wire [0:0] sb_1__2__0_chany_bottom_out_5_;
wire [0:0] sb_1__2__0_chany_bottom_out_7_;
wire [0:0] sb_1__2__0_chany_bottom_out_9_;
wire [0:0] sb_2__0__0_chanx_left_out_11_;
wire [0:0] sb_2__0__0_chanx_left_out_13_;
wire [0:0] sb_2__0__0_chanx_left_out_15_;
wire [0:0] sb_2__0__0_chanx_left_out_17_;
wire [0:0] sb_2__0__0_chanx_left_out_1_;
wire [0:0] sb_2__0__0_chanx_left_out_3_;
wire [0:0] sb_2__0__0_chanx_left_out_5_;
wire [0:0] sb_2__0__0_chanx_left_out_7_;
wire [0:0] sb_2__0__0_chanx_left_out_9_;
wire [0:0] sb_2__0__0_chany_top_out_0_;
wire [0:0] sb_2__0__0_chany_top_out_10_;
wire [0:0] sb_2__0__0_chany_top_out_12_;
wire [0:0] sb_2__0__0_chany_top_out_14_;
wire [0:0] sb_2__0__0_chany_top_out_16_;
wire [0:0] sb_2__0__0_chany_top_out_2_;
wire [0:0] sb_2__0__0_chany_top_out_4_;
wire [0:0] sb_2__0__0_chany_top_out_6_;
wire [0:0] sb_2__0__0_chany_top_out_8_;
wire [0:0] sb_2__1__0_chanx_left_out_11_;
wire [0:0] sb_2__1__0_chanx_left_out_13_;
wire [0:0] sb_2__1__0_chanx_left_out_15_;
wire [0:0] sb_2__1__0_chanx_left_out_17_;
wire [0:0] sb_2__1__0_chanx_left_out_1_;
wire [0:0] sb_2__1__0_chanx_left_out_3_;
wire [0:0] sb_2__1__0_chanx_left_out_5_;
wire [0:0] sb_2__1__0_chanx_left_out_7_;
wire [0:0] sb_2__1__0_chanx_left_out_9_;
wire [0:0] sb_2__1__0_chany_bottom_out_11_;
wire [0:0] sb_2__1__0_chany_bottom_out_13_;
wire [0:0] sb_2__1__0_chany_bottom_out_15_;
wire [0:0] sb_2__1__0_chany_bottom_out_17_;
wire [0:0] sb_2__1__0_chany_bottom_out_1_;
wire [0:0] sb_2__1__0_chany_bottom_out_3_;
wire [0:0] sb_2__1__0_chany_bottom_out_5_;
wire [0:0] sb_2__1__0_chany_bottom_out_7_;
wire [0:0] sb_2__1__0_chany_bottom_out_9_;
wire [0:0] sb_2__1__0_chany_top_out_0_;
wire [0:0] sb_2__1__0_chany_top_out_10_;
wire [0:0] sb_2__1__0_chany_top_out_12_;
wire [0:0] sb_2__1__0_chany_top_out_14_;
wire [0:0] sb_2__1__0_chany_top_out_16_;
wire [0:0] sb_2__1__0_chany_top_out_2_;
wire [0:0] sb_2__1__0_chany_top_out_4_;
wire [0:0] sb_2__1__0_chany_top_out_6_;
wire [0:0] sb_2__1__0_chany_top_out_8_;
wire [0:0] sb_2__2__0_chanx_left_out_11_;
wire [0:0] sb_2__2__0_chanx_left_out_13_;
wire [0:0] sb_2__2__0_chanx_left_out_15_;
wire [0:0] sb_2__2__0_chanx_left_out_17_;
wire [0:0] sb_2__2__0_chanx_left_out_1_;
wire [0:0] sb_2__2__0_chanx_left_out_3_;
wire [0:0] sb_2__2__0_chanx_left_out_5_;
wire [0:0] sb_2__2__0_chanx_left_out_7_;
wire [0:0] sb_2__2__0_chanx_left_out_9_;
wire [0:0] sb_2__2__0_chany_bottom_out_11_;
wire [0:0] sb_2__2__0_chany_bottom_out_13_;
wire [0:0] sb_2__2__0_chany_bottom_out_15_;
wire [0:0] sb_2__2__0_chany_bottom_out_17_;
wire [0:0] sb_2__2__0_chany_bottom_out_1_;
wire [0:0] sb_2__2__0_chany_bottom_out_3_;
wire [0:0] sb_2__2__0_chany_bottom_out_5_;
wire [0:0] sb_2__2__0_chany_bottom_out_7_;
wire [0:0] sb_2__2__0_chany_bottom_out_9_;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	grid_clb grid_clb_1_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.set(set[0]),
		.reset(reset[0]),
		.clk(clk[0]),
		.top_width_0_height_0__pin_0_(cbx_1__1__0_bottom_grid_pin_0_[0]),
		.top_width_0_height_0__pin_4_(cbx_1__1__0_bottom_grid_pin_4_[0]),
		.top_width_0_height_0__pin_8_(cbx_1__1__0_bottom_grid_pin_8_[0]),
		.right_width_0_height_0__pin_1_(cby_1__1__0_left_grid_pin_1_[0]),
		.right_width_0_height_0__pin_5_(cby_1__1__0_left_grid_pin_5_[0]),
		.right_width_0_height_0__pin_9_(cby_1__1__0_left_grid_pin_9_[0]),
		.bottom_width_0_height_0__pin_2_(cbx_1__0__0_top_grid_pin_2_[0]),
		.bottom_width_0_height_0__pin_6_(cbx_1__0__0_top_grid_pin_6_[0]),
		.bottom_width_0_height_0__pin_14_(cbx_1__0__0_top_grid_pin_14_[0]),
		.left_width_0_height_0__pin_3_(cby_0__1__0_right_grid_pin_3_[0]),
		.left_width_0_height_0__pin_7_(cby_0__1__0_right_grid_pin_7_[0]),
		.enable(decoder6to33_0_data_out[20]),
		.address(address[0:9]),
		.data_in(data_in[0]),
		.top_width_0_height_0__pin_12_(grid_clb_0_top_width_0_height_0__pin_12_[0]),
		.right_width_0_height_0__pin_13_(grid_clb_0_right_width_0_height_0__pin_13_[0]),
		.bottom_width_0_height_0__pin_10_(grid_clb_0_bottom_width_0_height_0__pin_10_[0]),
		.left_width_0_height_0__pin_11_(grid_clb_0_left_width_0_height_0__pin_11_[0]));

	grid_clb grid_clb_1_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.set(set[0]),
		.reset(reset[0]),
		.clk(clk[0]),
		.top_width_0_height_0__pin_0_(cbx_1__2__0_bottom_grid_pin_0_[0]),
		.top_width_0_height_0__pin_4_(cbx_1__2__0_bottom_grid_pin_4_[0]),
		.top_width_0_height_0__pin_8_(cbx_1__2__0_bottom_grid_pin_8_[0]),
		.right_width_0_height_0__pin_1_(cby_1__1__1_left_grid_pin_1_[0]),
		.right_width_0_height_0__pin_5_(cby_1__1__1_left_grid_pin_5_[0]),
		.right_width_0_height_0__pin_9_(cby_1__1__1_left_grid_pin_9_[0]),
		.bottom_width_0_height_0__pin_2_(cbx_1__1__0_top_grid_pin_2_[0]),
		.bottom_width_0_height_0__pin_6_(cbx_1__1__0_top_grid_pin_6_[0]),
		.bottom_width_0_height_0__pin_14_(cbx_1__1__0_top_grid_pin_14_[0]),
		.left_width_0_height_0__pin_3_(cby_0__1__1_right_grid_pin_3_[0]),
		.left_width_0_height_0__pin_7_(cby_0__1__1_right_grid_pin_7_[0]),
		.enable(decoder6to33_0_data_out[32]),
		.address(address[0:9]),
		.data_in(data_in[0]),
		.top_width_0_height_0__pin_12_(grid_clb_1_top_width_0_height_0__pin_12_[0]),
		.right_width_0_height_0__pin_13_(grid_clb_1_right_width_0_height_0__pin_13_[0]),
		.bottom_width_0_height_0__pin_10_(grid_clb_1_bottom_width_0_height_0__pin_10_[0]),
		.left_width_0_height_0__pin_11_(grid_clb_1_left_width_0_height_0__pin_11_[0]));

	grid_clb grid_clb_2_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.set(set[0]),
		.reset(reset[0]),
		.clk(clk[0]),
		.top_width_0_height_0__pin_0_(cbx_1__1__1_bottom_grid_pin_0_[0]),
		.top_width_0_height_0__pin_4_(cbx_1__1__1_bottom_grid_pin_4_[0]),
		.top_width_0_height_0__pin_8_(cbx_1__1__1_bottom_grid_pin_8_[0]),
		.right_width_0_height_0__pin_1_(cby_2__1__0_left_grid_pin_1_[0]),
		.right_width_0_height_0__pin_5_(cby_2__1__0_left_grid_pin_5_[0]),
		.right_width_0_height_0__pin_9_(cby_2__1__0_left_grid_pin_9_[0]),
		.bottom_width_0_height_0__pin_2_(cbx_1__0__1_top_grid_pin_2_[0]),
		.bottom_width_0_height_0__pin_6_(cbx_1__0__1_top_grid_pin_6_[0]),
		.bottom_width_0_height_0__pin_14_(cbx_1__0__1_top_grid_pin_14_[0]),
		.left_width_0_height_0__pin_3_(cby_1__1__0_right_grid_pin_3_[0]),
		.left_width_0_height_0__pin_7_(cby_1__1__0_right_grid_pin_7_[0]),
		.enable(decoder6to33_0_data_out[24]),
		.address(address[0:9]),
		.data_in(data_in[0]),
		.top_width_0_height_0__pin_12_(grid_clb_2_top_width_0_height_0__pin_12_[0]),
		.right_width_0_height_0__pin_13_(grid_clb_2_right_width_0_height_0__pin_13_[0]),
		.bottom_width_0_height_0__pin_10_(grid_clb_2_bottom_width_0_height_0__pin_10_[0]),
		.left_width_0_height_0__pin_11_(grid_clb_2_left_width_0_height_0__pin_11_[0]));

	grid_clb grid_clb_2_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.set(set[0]),
		.reset(reset[0]),
		.clk(clk[0]),
		.top_width_0_height_0__pin_0_(cbx_1__2__1_bottom_grid_pin_0_[0]),
		.top_width_0_height_0__pin_4_(cbx_1__2__1_bottom_grid_pin_4_[0]),
		.top_width_0_height_0__pin_8_(cbx_1__2__1_bottom_grid_pin_8_[0]),
		.right_width_0_height_0__pin_1_(cby_2__1__1_left_grid_pin_1_[0]),
		.right_width_0_height_0__pin_5_(cby_2__1__1_left_grid_pin_5_[0]),
		.right_width_0_height_0__pin_9_(cby_2__1__1_left_grid_pin_9_[0]),
		.bottom_width_0_height_0__pin_2_(cbx_1__1__1_top_grid_pin_2_[0]),
		.bottom_width_0_height_0__pin_6_(cbx_1__1__1_top_grid_pin_6_[0]),
		.bottom_width_0_height_0__pin_14_(cbx_1__1__1_top_grid_pin_14_[0]),
		.left_width_0_height_0__pin_3_(cby_1__1__1_right_grid_pin_3_[0]),
		.left_width_0_height_0__pin_7_(cby_1__1__1_right_grid_pin_7_[0]),
		.enable(decoder6to33_0_data_out[28]),
		.address(address[0:9]),
		.data_in(data_in[0]),
		.top_width_0_height_0__pin_12_(grid_clb_3_top_width_0_height_0__pin_12_[0]),
		.right_width_0_height_0__pin_13_(grid_clb_3_right_width_0_height_0__pin_13_[0]),
		.bottom_width_0_height_0__pin_10_(grid_clb_3_bottom_width_0_height_0__pin_10_[0]),
		.left_width_0_height_0__pin_11_(grid_clb_3_left_width_0_height_0__pin_11_[0]));

	grid_io_top grid_io_top_1_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.gfpga_pad_iopad_pad(gfpga_pad_iopad_pad[0:7]),
		.bottom_width_0_height_0__pin_0_(cbx_1__2__0_top_grid_pin_0_[0]),
		.bottom_width_0_height_0__pin_2_(cbx_1__2__0_top_grid_pin_2_[0]),
		.bottom_width_0_height_0__pin_4_(cbx_1__2__0_top_grid_pin_4_[0]),
		.bottom_width_0_height_0__pin_6_(cbx_1__2__0_top_grid_pin_6_[0]),
		.bottom_width_0_height_0__pin_8_(cbx_1__2__0_top_grid_pin_8_[0]),
		.bottom_width_0_height_0__pin_10_(cbx_1__2__0_top_grid_pin_10_[0]),
		.bottom_width_0_height_0__pin_12_(cbx_1__2__0_top_grid_pin_12_[0]),
		.bottom_width_0_height_0__pin_14_(cbx_1__2__0_top_grid_pin_14_[0]),
		.enable(decoder6to33_0_data_out[9]),
		.address(address[0:3]),
		.data_in(data_in[0]),
		.bottom_width_0_height_0__pin_1_(grid_io_top_0_bottom_width_0_height_0__pin_1_[0]),
		.bottom_width_0_height_0__pin_3_(grid_io_top_0_bottom_width_0_height_0__pin_3_[0]),
		.bottom_width_0_height_0__pin_5_(grid_io_top_0_bottom_width_0_height_0__pin_5_[0]),
		.bottom_width_0_height_0__pin_7_(grid_io_top_0_bottom_width_0_height_0__pin_7_[0]),
		.bottom_width_0_height_0__pin_9_(grid_io_top_0_bottom_width_0_height_0__pin_9_[0]),
		.bottom_width_0_height_0__pin_11_(grid_io_top_0_bottom_width_0_height_0__pin_11_[0]),
		.bottom_width_0_height_0__pin_13_(grid_io_top_0_bottom_width_0_height_0__pin_13_[0]),
		.bottom_width_0_height_0__pin_15_(grid_io_top_0_bottom_width_0_height_0__pin_15_[0]));

	grid_io_top grid_io_top_2_3 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.gfpga_pad_iopad_pad(gfpga_pad_iopad_pad[8:15]),
		.bottom_width_0_height_0__pin_0_(cbx_1__2__1_top_grid_pin_0_[0]),
		.bottom_width_0_height_0__pin_2_(cbx_1__2__1_top_grid_pin_2_[0]),
		.bottom_width_0_height_0__pin_4_(cbx_1__2__1_top_grid_pin_4_[0]),
		.bottom_width_0_height_0__pin_6_(cbx_1__2__1_top_grid_pin_6_[0]),
		.bottom_width_0_height_0__pin_8_(cbx_1__2__1_top_grid_pin_8_[0]),
		.bottom_width_0_height_0__pin_10_(cbx_1__2__1_top_grid_pin_10_[0]),
		.bottom_width_0_height_0__pin_12_(cbx_1__2__1_top_grid_pin_12_[0]),
		.bottom_width_0_height_0__pin_14_(cbx_1__2__1_top_grid_pin_14_[0]),
		.enable(decoder6to33_0_data_out[6]),
		.address(address[0:3]),
		.data_in(data_in[0]),
		.bottom_width_0_height_0__pin_1_(grid_io_top_1_bottom_width_0_height_0__pin_1_[0]),
		.bottom_width_0_height_0__pin_3_(grid_io_top_1_bottom_width_0_height_0__pin_3_[0]),
		.bottom_width_0_height_0__pin_5_(grid_io_top_1_bottom_width_0_height_0__pin_5_[0]),
		.bottom_width_0_height_0__pin_7_(grid_io_top_1_bottom_width_0_height_0__pin_7_[0]),
		.bottom_width_0_height_0__pin_9_(grid_io_top_1_bottom_width_0_height_0__pin_9_[0]),
		.bottom_width_0_height_0__pin_11_(grid_io_top_1_bottom_width_0_height_0__pin_11_[0]),
		.bottom_width_0_height_0__pin_13_(grid_io_top_1_bottom_width_0_height_0__pin_13_[0]),
		.bottom_width_0_height_0__pin_15_(grid_io_top_1_bottom_width_0_height_0__pin_15_[0]));

	grid_io_right grid_io_right_3_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.gfpga_pad_iopad_pad(gfpga_pad_iopad_pad[16:23]),
		.left_width_0_height_0__pin_0_(cby_2__1__0_right_grid_pin_0_[0]),
		.left_width_0_height_0__pin_2_(cby_2__1__0_right_grid_pin_2_[0]),
		.left_width_0_height_0__pin_4_(cby_2__1__0_right_grid_pin_4_[0]),
		.left_width_0_height_0__pin_6_(cby_2__1__0_right_grid_pin_6_[0]),
		.left_width_0_height_0__pin_8_(cby_2__1__0_right_grid_pin_8_[0]),
		.left_width_0_height_0__pin_10_(cby_2__1__0_right_grid_pin_10_[0]),
		.left_width_0_height_0__pin_12_(cby_2__1__0_right_grid_pin_12_[0]),
		.left_width_0_height_0__pin_14_(cby_2__1__0_right_grid_pin_14_[0]),
		.enable(decoder6to33_0_data_out[2]),
		.address(address[0:3]),
		.data_in(data_in[0]),
		.left_width_0_height_0__pin_1_(grid_io_right_0_left_width_0_height_0__pin_1_[0]),
		.left_width_0_height_0__pin_3_(grid_io_right_0_left_width_0_height_0__pin_3_[0]),
		.left_width_0_height_0__pin_5_(grid_io_right_0_left_width_0_height_0__pin_5_[0]),
		.left_width_0_height_0__pin_7_(grid_io_right_0_left_width_0_height_0__pin_7_[0]),
		.left_width_0_height_0__pin_9_(grid_io_right_0_left_width_0_height_0__pin_9_[0]),
		.left_width_0_height_0__pin_11_(grid_io_right_0_left_width_0_height_0__pin_11_[0]),
		.left_width_0_height_0__pin_13_(grid_io_right_0_left_width_0_height_0__pin_13_[0]),
		.left_width_0_height_0__pin_15_(grid_io_right_0_left_width_0_height_0__pin_15_[0]));

	grid_io_right grid_io_right_3_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.gfpga_pad_iopad_pad(gfpga_pad_iopad_pad[24:31]),
		.left_width_0_height_0__pin_0_(cby_2__1__1_right_grid_pin_0_[0]),
		.left_width_0_height_0__pin_2_(cby_2__1__1_right_grid_pin_2_[0]),
		.left_width_0_height_0__pin_4_(cby_2__1__1_right_grid_pin_4_[0]),
		.left_width_0_height_0__pin_6_(cby_2__1__1_right_grid_pin_6_[0]),
		.left_width_0_height_0__pin_8_(cby_2__1__1_right_grid_pin_8_[0]),
		.left_width_0_height_0__pin_10_(cby_2__1__1_right_grid_pin_10_[0]),
		.left_width_0_height_0__pin_12_(cby_2__1__1_right_grid_pin_12_[0]),
		.left_width_0_height_0__pin_14_(cby_2__1__1_right_grid_pin_14_[0]),
		.enable(decoder6to33_0_data_out[3]),
		.address(address[0:3]),
		.data_in(data_in[0]),
		.left_width_0_height_0__pin_1_(grid_io_right_1_left_width_0_height_0__pin_1_[0]),
		.left_width_0_height_0__pin_3_(grid_io_right_1_left_width_0_height_0__pin_3_[0]),
		.left_width_0_height_0__pin_5_(grid_io_right_1_left_width_0_height_0__pin_5_[0]),
		.left_width_0_height_0__pin_7_(grid_io_right_1_left_width_0_height_0__pin_7_[0]),
		.left_width_0_height_0__pin_9_(grid_io_right_1_left_width_0_height_0__pin_9_[0]),
		.left_width_0_height_0__pin_11_(grid_io_right_1_left_width_0_height_0__pin_11_[0]),
		.left_width_0_height_0__pin_13_(grid_io_right_1_left_width_0_height_0__pin_13_[0]),
		.left_width_0_height_0__pin_15_(grid_io_right_1_left_width_0_height_0__pin_15_[0]));

	grid_io_bottom grid_io_bottom_1_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.gfpga_pad_iopad_pad(gfpga_pad_iopad_pad[32:39]),
		.top_width_0_height_0__pin_0_(cbx_1__0__0_bottom_grid_pin_0_[0]),
		.top_width_0_height_0__pin_2_(cbx_1__0__0_bottom_grid_pin_2_[0]),
		.top_width_0_height_0__pin_4_(cbx_1__0__0_bottom_grid_pin_4_[0]),
		.top_width_0_height_0__pin_6_(cbx_1__0__0_bottom_grid_pin_6_[0]),
		.top_width_0_height_0__pin_8_(cbx_1__0__0_bottom_grid_pin_8_[0]),
		.top_width_0_height_0__pin_10_(cbx_1__0__0_bottom_grid_pin_10_[0]),
		.top_width_0_height_0__pin_12_(cbx_1__0__0_bottom_grid_pin_12_[0]),
		.top_width_0_height_0__pin_14_(cbx_1__0__0_bottom_grid_pin_14_[0]),
		.enable(decoder6to33_0_data_out[0]),
		.address(address[0:3]),
		.data_in(data_in[0]),
		.top_width_0_height_0__pin_1_(grid_io_bottom_0_top_width_0_height_0__pin_1_[0]),
		.top_width_0_height_0__pin_3_(grid_io_bottom_0_top_width_0_height_0__pin_3_[0]),
		.top_width_0_height_0__pin_5_(grid_io_bottom_0_top_width_0_height_0__pin_5_[0]),
		.top_width_0_height_0__pin_7_(grid_io_bottom_0_top_width_0_height_0__pin_7_[0]),
		.top_width_0_height_0__pin_9_(grid_io_bottom_0_top_width_0_height_0__pin_9_[0]),
		.top_width_0_height_0__pin_11_(grid_io_bottom_0_top_width_0_height_0__pin_11_[0]),
		.top_width_0_height_0__pin_13_(grid_io_bottom_0_top_width_0_height_0__pin_13_[0]),
		.top_width_0_height_0__pin_15_(grid_io_bottom_0_top_width_0_height_0__pin_15_[0]));

	grid_io_bottom grid_io_bottom_2_0 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.gfpga_pad_iopad_pad(gfpga_pad_iopad_pad[40:47]),
		.top_width_0_height_0__pin_0_(cbx_1__0__1_bottom_grid_pin_0_[0]),
		.top_width_0_height_0__pin_2_(cbx_1__0__1_bottom_grid_pin_2_[0]),
		.top_width_0_height_0__pin_4_(cbx_1__0__1_bottom_grid_pin_4_[0]),
		.top_width_0_height_0__pin_6_(cbx_1__0__1_bottom_grid_pin_6_[0]),
		.top_width_0_height_0__pin_8_(cbx_1__0__1_bottom_grid_pin_8_[0]),
		.top_width_0_height_0__pin_10_(cbx_1__0__1_bottom_grid_pin_10_[0]),
		.top_width_0_height_0__pin_12_(cbx_1__0__1_bottom_grid_pin_12_[0]),
		.top_width_0_height_0__pin_14_(cbx_1__0__1_bottom_grid_pin_14_[0]),
		.enable(decoder6to33_0_data_out[1]),
		.address(address[0:3]),
		.data_in(data_in[0]),
		.top_width_0_height_0__pin_1_(grid_io_bottom_1_top_width_0_height_0__pin_1_[0]),
		.top_width_0_height_0__pin_3_(grid_io_bottom_1_top_width_0_height_0__pin_3_[0]),
		.top_width_0_height_0__pin_5_(grid_io_bottom_1_top_width_0_height_0__pin_5_[0]),
		.top_width_0_height_0__pin_7_(grid_io_bottom_1_top_width_0_height_0__pin_7_[0]),
		.top_width_0_height_0__pin_9_(grid_io_bottom_1_top_width_0_height_0__pin_9_[0]),
		.top_width_0_height_0__pin_11_(grid_io_bottom_1_top_width_0_height_0__pin_11_[0]),
		.top_width_0_height_0__pin_13_(grid_io_bottom_1_top_width_0_height_0__pin_13_[0]),
		.top_width_0_height_0__pin_15_(grid_io_bottom_1_top_width_0_height_0__pin_15_[0]));

	grid_io_left grid_io_left_0_1 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.gfpga_pad_iopad_pad(gfpga_pad_iopad_pad[48:55]),
		.right_width_0_height_0__pin_0_(cby_0__1__0_left_grid_pin_0_[0]),
		.right_width_0_height_0__pin_2_(cby_0__1__0_left_grid_pin_2_[0]),
		.right_width_0_height_0__pin_4_(cby_0__1__0_left_grid_pin_4_[0]),
		.right_width_0_height_0__pin_6_(cby_0__1__0_left_grid_pin_6_[0]),
		.right_width_0_height_0__pin_8_(cby_0__1__0_left_grid_pin_8_[0]),
		.right_width_0_height_0__pin_10_(cby_0__1__0_left_grid_pin_10_[0]),
		.right_width_0_height_0__pin_12_(cby_0__1__0_left_grid_pin_12_[0]),
		.right_width_0_height_0__pin_14_(cby_0__1__0_left_grid_pin_14_[0]),
		.enable(decoder6to33_0_data_out[16]),
		.address(address[0:3]),
		.data_in(data_in[0]),
		.right_width_0_height_0__pin_1_(grid_io_left_0_right_width_0_height_0__pin_1_[0]),
		.right_width_0_height_0__pin_3_(grid_io_left_0_right_width_0_height_0__pin_3_[0]),
		.right_width_0_height_0__pin_5_(grid_io_left_0_right_width_0_height_0__pin_5_[0]),
		.right_width_0_height_0__pin_7_(grid_io_left_0_right_width_0_height_0__pin_7_[0]),
		.right_width_0_height_0__pin_9_(grid_io_left_0_right_width_0_height_0__pin_9_[0]),
		.right_width_0_height_0__pin_11_(grid_io_left_0_right_width_0_height_0__pin_11_[0]),
		.right_width_0_height_0__pin_13_(grid_io_left_0_right_width_0_height_0__pin_13_[0]),
		.right_width_0_height_0__pin_15_(grid_io_left_0_right_width_0_height_0__pin_15_[0]));

	grid_io_left grid_io_left_0_2 (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.gfpga_pad_iopad_pad(gfpga_pad_iopad_pad[56:63]),
		.right_width_0_height_0__pin_0_(cby_0__1__1_left_grid_pin_0_[0]),
		.right_width_0_height_0__pin_2_(cby_0__1__1_left_grid_pin_2_[0]),
		.right_width_0_height_0__pin_4_(cby_0__1__1_left_grid_pin_4_[0]),
		.right_width_0_height_0__pin_6_(cby_0__1__1_left_grid_pin_6_[0]),
		.right_width_0_height_0__pin_8_(cby_0__1__1_left_grid_pin_8_[0]),
		.right_width_0_height_0__pin_10_(cby_0__1__1_left_grid_pin_10_[0]),
		.right_width_0_height_0__pin_12_(cby_0__1__1_left_grid_pin_12_[0]),
		.right_width_0_height_0__pin_14_(cby_0__1__1_left_grid_pin_14_[0]),
		.enable(decoder6to33_0_data_out[13]),
		.address(address[0:3]),
		.data_in(data_in[0]),
		.right_width_0_height_0__pin_1_(grid_io_left_1_right_width_0_height_0__pin_1_[0]),
		.right_width_0_height_0__pin_3_(grid_io_left_1_right_width_0_height_0__pin_3_[0]),
		.right_width_0_height_0__pin_5_(grid_io_left_1_right_width_0_height_0__pin_5_[0]),
		.right_width_0_height_0__pin_7_(grid_io_left_1_right_width_0_height_0__pin_7_[0]),
		.right_width_0_height_0__pin_9_(grid_io_left_1_right_width_0_height_0__pin_9_[0]),
		.right_width_0_height_0__pin_11_(grid_io_left_1_right_width_0_height_0__pin_11_[0]),
		.right_width_0_height_0__pin_13_(grid_io_left_1_right_width_0_height_0__pin_13_[0]),
		.right_width_0_height_0__pin_15_(grid_io_left_1_right_width_0_height_0__pin_15_[0]));

	sb_0__0_ sb_0__0_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_top_in_1_(cby_0__1__0_chany_out_1_[0]),
		.chany_top_in_3_(cby_0__1__0_chany_out_3_[0]),
		.chany_top_in_5_(cby_0__1__0_chany_out_5_[0]),
		.chany_top_in_7_(cby_0__1__0_chany_out_7_[0]),
		.chany_top_in_9_(cby_0__1__0_chany_out_9_[0]),
		.chany_top_in_11_(cby_0__1__0_chany_out_11_[0]),
		.chany_top_in_13_(cby_0__1__0_chany_out_13_[0]),
		.chany_top_in_15_(cby_0__1__0_chany_out_15_[0]),
		.chany_top_in_17_(cby_0__1__0_chany_out_17_[0]),
		.top_left_grid_pin_1_(grid_io_left_0_right_width_0_height_0__pin_1_[0]),
		.top_left_grid_pin_3_(grid_io_left_0_right_width_0_height_0__pin_3_[0]),
		.top_left_grid_pin_5_(grid_io_left_0_right_width_0_height_0__pin_5_[0]),
		.top_left_grid_pin_7_(grid_io_left_0_right_width_0_height_0__pin_7_[0]),
		.top_left_grid_pin_9_(grid_io_left_0_right_width_0_height_0__pin_9_[0]),
		.top_left_grid_pin_11_(grid_io_left_0_right_width_0_height_0__pin_11_[0]),
		.top_left_grid_pin_13_(grid_io_left_0_right_width_0_height_0__pin_13_[0]),
		.top_left_grid_pin_15_(grid_io_left_0_right_width_0_height_0__pin_15_[0]),
		.top_right_grid_pin_11_(grid_clb_0_left_width_0_height_0__pin_11_[0]),
		.chanx_right_in_1_(cbx_1__0__0_chanx_out_1_[0]),
		.chanx_right_in_3_(cbx_1__0__0_chanx_out_3_[0]),
		.chanx_right_in_5_(cbx_1__0__0_chanx_out_5_[0]),
		.chanx_right_in_7_(cbx_1__0__0_chanx_out_7_[0]),
		.chanx_right_in_9_(cbx_1__0__0_chanx_out_9_[0]),
		.chanx_right_in_11_(cbx_1__0__0_chanx_out_11_[0]),
		.chanx_right_in_13_(cbx_1__0__0_chanx_out_13_[0]),
		.chanx_right_in_15_(cbx_1__0__0_chanx_out_15_[0]),
		.chanx_right_in_17_(cbx_1__0__0_chanx_out_17_[0]),
		.right_top_grid_pin_10_(grid_clb_0_bottom_width_0_height_0__pin_10_[0]),
		.right_bottom_grid_pin_1_(grid_io_bottom_0_top_width_0_height_0__pin_1_[0]),
		.right_bottom_grid_pin_3_(grid_io_bottom_0_top_width_0_height_0__pin_3_[0]),
		.right_bottom_grid_pin_5_(grid_io_bottom_0_top_width_0_height_0__pin_5_[0]),
		.right_bottom_grid_pin_7_(grid_io_bottom_0_top_width_0_height_0__pin_7_[0]),
		.right_bottom_grid_pin_9_(grid_io_bottom_0_top_width_0_height_0__pin_9_[0]),
		.right_bottom_grid_pin_11_(grid_io_bottom_0_top_width_0_height_0__pin_11_[0]),
		.right_bottom_grid_pin_13_(grid_io_bottom_0_top_width_0_height_0__pin_13_[0]),
		.right_bottom_grid_pin_15_(grid_io_bottom_0_top_width_0_height_0__pin_15_[0]),
		.enable(decoder6to33_0_data_out[14]),
		.address(address[0:5]),
		.data_in(data_in[0]),
		.chany_top_out_0_(sb_0__0__0_chany_top_out_0_[0]),
		.chany_top_out_2_(sb_0__0__0_chany_top_out_2_[0]),
		.chany_top_out_4_(sb_0__0__0_chany_top_out_4_[0]),
		.chany_top_out_6_(sb_0__0__0_chany_top_out_6_[0]),
		.chany_top_out_8_(sb_0__0__0_chany_top_out_8_[0]),
		.chany_top_out_10_(sb_0__0__0_chany_top_out_10_[0]),
		.chany_top_out_12_(sb_0__0__0_chany_top_out_12_[0]),
		.chany_top_out_14_(sb_0__0__0_chany_top_out_14_[0]),
		.chany_top_out_16_(sb_0__0__0_chany_top_out_16_[0]),
		.chanx_right_out_0_(sb_0__0__0_chanx_right_out_0_[0]),
		.chanx_right_out_2_(sb_0__0__0_chanx_right_out_2_[0]),
		.chanx_right_out_4_(sb_0__0__0_chanx_right_out_4_[0]),
		.chanx_right_out_6_(sb_0__0__0_chanx_right_out_6_[0]),
		.chanx_right_out_8_(sb_0__0__0_chanx_right_out_8_[0]),
		.chanx_right_out_10_(sb_0__0__0_chanx_right_out_10_[0]),
		.chanx_right_out_12_(sb_0__0__0_chanx_right_out_12_[0]),
		.chanx_right_out_14_(sb_0__0__0_chanx_right_out_14_[0]),
		.chanx_right_out_16_(sb_0__0__0_chanx_right_out_16_[0]));

	sb_0__1_ sb_0__1_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_top_in_1_(cby_0__1__1_chany_out_1_[0]),
		.chany_top_in_3_(cby_0__1__1_chany_out_3_[0]),
		.chany_top_in_5_(cby_0__1__1_chany_out_5_[0]),
		.chany_top_in_7_(cby_0__1__1_chany_out_7_[0]),
		.chany_top_in_9_(cby_0__1__1_chany_out_9_[0]),
		.chany_top_in_11_(cby_0__1__1_chany_out_11_[0]),
		.chany_top_in_13_(cby_0__1__1_chany_out_13_[0]),
		.chany_top_in_15_(cby_0__1__1_chany_out_15_[0]),
		.chany_top_in_17_(cby_0__1__1_chany_out_17_[0]),
		.top_left_grid_pin_1_(grid_io_left_1_right_width_0_height_0__pin_1_[0]),
		.top_left_grid_pin_3_(grid_io_left_1_right_width_0_height_0__pin_3_[0]),
		.top_left_grid_pin_5_(grid_io_left_1_right_width_0_height_0__pin_5_[0]),
		.top_left_grid_pin_7_(grid_io_left_1_right_width_0_height_0__pin_7_[0]),
		.top_left_grid_pin_9_(grid_io_left_1_right_width_0_height_0__pin_9_[0]),
		.top_left_grid_pin_11_(grid_io_left_1_right_width_0_height_0__pin_11_[0]),
		.top_left_grid_pin_13_(grid_io_left_1_right_width_0_height_0__pin_13_[0]),
		.top_left_grid_pin_15_(grid_io_left_1_right_width_0_height_0__pin_15_[0]),
		.top_right_grid_pin_11_(grid_clb_1_left_width_0_height_0__pin_11_[0]),
		.chanx_right_in_1_(cbx_1__1__0_chanx_out_1_[0]),
		.chanx_right_in_3_(cbx_1__1__0_chanx_out_3_[0]),
		.chanx_right_in_5_(cbx_1__1__0_chanx_out_5_[0]),
		.chanx_right_in_7_(cbx_1__1__0_chanx_out_7_[0]),
		.chanx_right_in_9_(cbx_1__1__0_chanx_out_9_[0]),
		.chanx_right_in_11_(cbx_1__1__0_chanx_out_11_[0]),
		.chanx_right_in_13_(cbx_1__1__0_chanx_out_13_[0]),
		.chanx_right_in_15_(cbx_1__1__0_chanx_out_15_[0]),
		.chanx_right_in_17_(cbx_1__1__0_chanx_out_17_[0]),
		.right_top_grid_pin_10_(grid_clb_1_bottom_width_0_height_0__pin_10_[0]),
		.right_bottom_grid_pin_12_(grid_clb_0_top_width_0_height_0__pin_12_[0]),
		.chany_bottom_in_0_(cby_0__1__0_chany_out_0_[0]),
		.chany_bottom_in_2_(cby_0__1__0_chany_out_2_[0]),
		.chany_bottom_in_4_(cby_0__1__0_chany_out_4_[0]),
		.chany_bottom_in_6_(cby_0__1__0_chany_out_6_[0]),
		.chany_bottom_in_8_(cby_0__1__0_chany_out_8_[0]),
		.chany_bottom_in_10_(cby_0__1__0_chany_out_10_[0]),
		.chany_bottom_in_12_(cby_0__1__0_chany_out_12_[0]),
		.chany_bottom_in_14_(cby_0__1__0_chany_out_14_[0]),
		.chany_bottom_in_16_(cby_0__1__0_chany_out_16_[0]),
		.bottom_right_grid_pin_11_(grid_clb_0_left_width_0_height_0__pin_11_[0]),
		.bottom_left_grid_pin_1_(grid_io_left_0_right_width_0_height_0__pin_1_[0]),
		.bottom_left_grid_pin_3_(grid_io_left_0_right_width_0_height_0__pin_3_[0]),
		.bottom_left_grid_pin_5_(grid_io_left_0_right_width_0_height_0__pin_5_[0]),
		.bottom_left_grid_pin_7_(grid_io_left_0_right_width_0_height_0__pin_7_[0]),
		.bottom_left_grid_pin_9_(grid_io_left_0_right_width_0_height_0__pin_9_[0]),
		.bottom_left_grid_pin_11_(grid_io_left_0_right_width_0_height_0__pin_11_[0]),
		.bottom_left_grid_pin_13_(grid_io_left_0_right_width_0_height_0__pin_13_[0]),
		.bottom_left_grid_pin_15_(grid_io_left_0_right_width_0_height_0__pin_15_[0]),
		.enable(decoder6to33_0_data_out[11]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chany_top_out_0_(sb_0__1__0_chany_top_out_0_[0]),
		.chany_top_out_2_(sb_0__1__0_chany_top_out_2_[0]),
		.chany_top_out_4_(sb_0__1__0_chany_top_out_4_[0]),
		.chany_top_out_6_(sb_0__1__0_chany_top_out_6_[0]),
		.chany_top_out_8_(sb_0__1__0_chany_top_out_8_[0]),
		.chany_top_out_10_(sb_0__1__0_chany_top_out_10_[0]),
		.chany_top_out_12_(sb_0__1__0_chany_top_out_12_[0]),
		.chany_top_out_14_(sb_0__1__0_chany_top_out_14_[0]),
		.chany_top_out_16_(sb_0__1__0_chany_top_out_16_[0]),
		.chanx_right_out_0_(sb_0__1__0_chanx_right_out_0_[0]),
		.chanx_right_out_2_(sb_0__1__0_chanx_right_out_2_[0]),
		.chanx_right_out_4_(sb_0__1__0_chanx_right_out_4_[0]),
		.chanx_right_out_6_(sb_0__1__0_chanx_right_out_6_[0]),
		.chanx_right_out_8_(sb_0__1__0_chanx_right_out_8_[0]),
		.chanx_right_out_10_(sb_0__1__0_chanx_right_out_10_[0]),
		.chanx_right_out_12_(sb_0__1__0_chanx_right_out_12_[0]),
		.chanx_right_out_14_(sb_0__1__0_chanx_right_out_14_[0]),
		.chanx_right_out_16_(sb_0__1__0_chanx_right_out_16_[0]),
		.chany_bottom_out_1_(sb_0__1__0_chany_bottom_out_1_[0]),
		.chany_bottom_out_3_(sb_0__1__0_chany_bottom_out_3_[0]),
		.chany_bottom_out_5_(sb_0__1__0_chany_bottom_out_5_[0]),
		.chany_bottom_out_7_(sb_0__1__0_chany_bottom_out_7_[0]),
		.chany_bottom_out_9_(sb_0__1__0_chany_bottom_out_9_[0]),
		.chany_bottom_out_11_(sb_0__1__0_chany_bottom_out_11_[0]),
		.chany_bottom_out_13_(sb_0__1__0_chany_bottom_out_13_[0]),
		.chany_bottom_out_15_(sb_0__1__0_chany_bottom_out_15_[0]),
		.chany_bottom_out_17_(sb_0__1__0_chany_bottom_out_17_[0]));

	sb_0__2_ sb_0__2_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chanx_right_in_1_(cbx_1__2__0_chanx_out_1_[0]),
		.chanx_right_in_3_(cbx_1__2__0_chanx_out_3_[0]),
		.chanx_right_in_5_(cbx_1__2__0_chanx_out_5_[0]),
		.chanx_right_in_7_(cbx_1__2__0_chanx_out_7_[0]),
		.chanx_right_in_9_(cbx_1__2__0_chanx_out_9_[0]),
		.chanx_right_in_11_(cbx_1__2__0_chanx_out_11_[0]),
		.chanx_right_in_13_(cbx_1__2__0_chanx_out_13_[0]),
		.chanx_right_in_15_(cbx_1__2__0_chanx_out_15_[0]),
		.chanx_right_in_17_(cbx_1__2__0_chanx_out_17_[0]),
		.right_top_grid_pin_1_(grid_io_top_0_bottom_width_0_height_0__pin_1_[0]),
		.right_top_grid_pin_3_(grid_io_top_0_bottom_width_0_height_0__pin_3_[0]),
		.right_top_grid_pin_5_(grid_io_top_0_bottom_width_0_height_0__pin_5_[0]),
		.right_top_grid_pin_7_(grid_io_top_0_bottom_width_0_height_0__pin_7_[0]),
		.right_top_grid_pin_9_(grid_io_top_0_bottom_width_0_height_0__pin_9_[0]),
		.right_top_grid_pin_11_(grid_io_top_0_bottom_width_0_height_0__pin_11_[0]),
		.right_top_grid_pin_13_(grid_io_top_0_bottom_width_0_height_0__pin_13_[0]),
		.right_top_grid_pin_15_(grid_io_top_0_bottom_width_0_height_0__pin_15_[0]),
		.right_bottom_grid_pin_12_(grid_clb_1_top_width_0_height_0__pin_12_[0]),
		.chany_bottom_in_0_(cby_0__1__1_chany_out_0_[0]),
		.chany_bottom_in_2_(cby_0__1__1_chany_out_2_[0]),
		.chany_bottom_in_4_(cby_0__1__1_chany_out_4_[0]),
		.chany_bottom_in_6_(cby_0__1__1_chany_out_6_[0]),
		.chany_bottom_in_8_(cby_0__1__1_chany_out_8_[0]),
		.chany_bottom_in_10_(cby_0__1__1_chany_out_10_[0]),
		.chany_bottom_in_12_(cby_0__1__1_chany_out_12_[0]),
		.chany_bottom_in_14_(cby_0__1__1_chany_out_14_[0]),
		.chany_bottom_in_16_(cby_0__1__1_chany_out_16_[0]),
		.bottom_right_grid_pin_11_(grid_clb_1_left_width_0_height_0__pin_11_[0]),
		.bottom_left_grid_pin_1_(grid_io_left_1_right_width_0_height_0__pin_1_[0]),
		.bottom_left_grid_pin_3_(grid_io_left_1_right_width_0_height_0__pin_3_[0]),
		.bottom_left_grid_pin_5_(grid_io_left_1_right_width_0_height_0__pin_5_[0]),
		.bottom_left_grid_pin_7_(grid_io_left_1_right_width_0_height_0__pin_7_[0]),
		.bottom_left_grid_pin_9_(grid_io_left_1_right_width_0_height_0__pin_9_[0]),
		.bottom_left_grid_pin_11_(grid_io_left_1_right_width_0_height_0__pin_11_[0]),
		.bottom_left_grid_pin_13_(grid_io_left_1_right_width_0_height_0__pin_13_[0]),
		.bottom_left_grid_pin_15_(grid_io_left_1_right_width_0_height_0__pin_15_[0]),
		.enable(decoder6to33_0_data_out[10]),
		.address(address[0:5]),
		.data_in(data_in[0]),
		.chanx_right_out_0_(sb_0__2__0_chanx_right_out_0_[0]),
		.chanx_right_out_2_(sb_0__2__0_chanx_right_out_2_[0]),
		.chanx_right_out_4_(sb_0__2__0_chanx_right_out_4_[0]),
		.chanx_right_out_6_(sb_0__2__0_chanx_right_out_6_[0]),
		.chanx_right_out_8_(sb_0__2__0_chanx_right_out_8_[0]),
		.chanx_right_out_10_(sb_0__2__0_chanx_right_out_10_[0]),
		.chanx_right_out_12_(sb_0__2__0_chanx_right_out_12_[0]),
		.chanx_right_out_14_(sb_0__2__0_chanx_right_out_14_[0]),
		.chanx_right_out_16_(sb_0__2__0_chanx_right_out_16_[0]),
		.chany_bottom_out_1_(sb_0__2__0_chany_bottom_out_1_[0]),
		.chany_bottom_out_3_(sb_0__2__0_chany_bottom_out_3_[0]),
		.chany_bottom_out_5_(sb_0__2__0_chany_bottom_out_5_[0]),
		.chany_bottom_out_7_(sb_0__2__0_chany_bottom_out_7_[0]),
		.chany_bottom_out_9_(sb_0__2__0_chany_bottom_out_9_[0]),
		.chany_bottom_out_11_(sb_0__2__0_chany_bottom_out_11_[0]),
		.chany_bottom_out_13_(sb_0__2__0_chany_bottom_out_13_[0]),
		.chany_bottom_out_15_(sb_0__2__0_chany_bottom_out_15_[0]),
		.chany_bottom_out_17_(sb_0__2__0_chany_bottom_out_17_[0]));

	sb_1__0_ sb_1__0_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_top_in_1_(cby_1__1__0_chany_out_1_[0]),
		.chany_top_in_3_(cby_1__1__0_chany_out_3_[0]),
		.chany_top_in_5_(cby_1__1__0_chany_out_5_[0]),
		.chany_top_in_7_(cby_1__1__0_chany_out_7_[0]),
		.chany_top_in_9_(cby_1__1__0_chany_out_9_[0]),
		.chany_top_in_11_(cby_1__1__0_chany_out_11_[0]),
		.chany_top_in_13_(cby_1__1__0_chany_out_13_[0]),
		.chany_top_in_15_(cby_1__1__0_chany_out_15_[0]),
		.chany_top_in_17_(cby_1__1__0_chany_out_17_[0]),
		.top_left_grid_pin_13_(grid_clb_0_right_width_0_height_0__pin_13_[0]),
		.top_right_grid_pin_11_(grid_clb_2_left_width_0_height_0__pin_11_[0]),
		.chanx_right_in_1_(cbx_1__0__1_chanx_out_1_[0]),
		.chanx_right_in_3_(cbx_1__0__1_chanx_out_3_[0]),
		.chanx_right_in_5_(cbx_1__0__1_chanx_out_5_[0]),
		.chanx_right_in_7_(cbx_1__0__1_chanx_out_7_[0]),
		.chanx_right_in_9_(cbx_1__0__1_chanx_out_9_[0]),
		.chanx_right_in_11_(cbx_1__0__1_chanx_out_11_[0]),
		.chanx_right_in_13_(cbx_1__0__1_chanx_out_13_[0]),
		.chanx_right_in_15_(cbx_1__0__1_chanx_out_15_[0]),
		.chanx_right_in_17_(cbx_1__0__1_chanx_out_17_[0]),
		.right_top_grid_pin_10_(grid_clb_2_bottom_width_0_height_0__pin_10_[0]),
		.right_bottom_grid_pin_1_(grid_io_bottom_1_top_width_0_height_0__pin_1_[0]),
		.right_bottom_grid_pin_3_(grid_io_bottom_1_top_width_0_height_0__pin_3_[0]),
		.right_bottom_grid_pin_5_(grid_io_bottom_1_top_width_0_height_0__pin_5_[0]),
		.right_bottom_grid_pin_7_(grid_io_bottom_1_top_width_0_height_0__pin_7_[0]),
		.right_bottom_grid_pin_9_(grid_io_bottom_1_top_width_0_height_0__pin_9_[0]),
		.right_bottom_grid_pin_11_(grid_io_bottom_1_top_width_0_height_0__pin_11_[0]),
		.right_bottom_grid_pin_13_(grid_io_bottom_1_top_width_0_height_0__pin_13_[0]),
		.right_bottom_grid_pin_15_(grid_io_bottom_1_top_width_0_height_0__pin_15_[0]),
		.chanx_left_in_0_(cbx_1__0__0_chanx_out_0_[0]),
		.chanx_left_in_2_(cbx_1__0__0_chanx_out_2_[0]),
		.chanx_left_in_4_(cbx_1__0__0_chanx_out_4_[0]),
		.chanx_left_in_6_(cbx_1__0__0_chanx_out_6_[0]),
		.chanx_left_in_8_(cbx_1__0__0_chanx_out_8_[0]),
		.chanx_left_in_10_(cbx_1__0__0_chanx_out_10_[0]),
		.chanx_left_in_12_(cbx_1__0__0_chanx_out_12_[0]),
		.chanx_left_in_14_(cbx_1__0__0_chanx_out_14_[0]),
		.chanx_left_in_16_(cbx_1__0__0_chanx_out_16_[0]),
		.left_top_grid_pin_10_(grid_clb_0_bottom_width_0_height_0__pin_10_[0]),
		.left_bottom_grid_pin_1_(grid_io_bottom_0_top_width_0_height_0__pin_1_[0]),
		.left_bottom_grid_pin_3_(grid_io_bottom_0_top_width_0_height_0__pin_3_[0]),
		.left_bottom_grid_pin_5_(grid_io_bottom_0_top_width_0_height_0__pin_5_[0]),
		.left_bottom_grid_pin_7_(grid_io_bottom_0_top_width_0_height_0__pin_7_[0]),
		.left_bottom_grid_pin_9_(grid_io_bottom_0_top_width_0_height_0__pin_9_[0]),
		.left_bottom_grid_pin_11_(grid_io_bottom_0_top_width_0_height_0__pin_11_[0]),
		.left_bottom_grid_pin_13_(grid_io_bottom_0_top_width_0_height_0__pin_13_[0]),
		.left_bottom_grid_pin_15_(grid_io_bottom_0_top_width_0_height_0__pin_15_[0]),
		.enable(decoder6to33_0_data_out[17]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chany_top_out_0_(sb_1__0__0_chany_top_out_0_[0]),
		.chany_top_out_2_(sb_1__0__0_chany_top_out_2_[0]),
		.chany_top_out_4_(sb_1__0__0_chany_top_out_4_[0]),
		.chany_top_out_6_(sb_1__0__0_chany_top_out_6_[0]),
		.chany_top_out_8_(sb_1__0__0_chany_top_out_8_[0]),
		.chany_top_out_10_(sb_1__0__0_chany_top_out_10_[0]),
		.chany_top_out_12_(sb_1__0__0_chany_top_out_12_[0]),
		.chany_top_out_14_(sb_1__0__0_chany_top_out_14_[0]),
		.chany_top_out_16_(sb_1__0__0_chany_top_out_16_[0]),
		.chanx_right_out_0_(sb_1__0__0_chanx_right_out_0_[0]),
		.chanx_right_out_2_(sb_1__0__0_chanx_right_out_2_[0]),
		.chanx_right_out_4_(sb_1__0__0_chanx_right_out_4_[0]),
		.chanx_right_out_6_(sb_1__0__0_chanx_right_out_6_[0]),
		.chanx_right_out_8_(sb_1__0__0_chanx_right_out_8_[0]),
		.chanx_right_out_10_(sb_1__0__0_chanx_right_out_10_[0]),
		.chanx_right_out_12_(sb_1__0__0_chanx_right_out_12_[0]),
		.chanx_right_out_14_(sb_1__0__0_chanx_right_out_14_[0]),
		.chanx_right_out_16_(sb_1__0__0_chanx_right_out_16_[0]),
		.chanx_left_out_1_(sb_1__0__0_chanx_left_out_1_[0]),
		.chanx_left_out_3_(sb_1__0__0_chanx_left_out_3_[0]),
		.chanx_left_out_5_(sb_1__0__0_chanx_left_out_5_[0]),
		.chanx_left_out_7_(sb_1__0__0_chanx_left_out_7_[0]),
		.chanx_left_out_9_(sb_1__0__0_chanx_left_out_9_[0]),
		.chanx_left_out_11_(sb_1__0__0_chanx_left_out_11_[0]),
		.chanx_left_out_13_(sb_1__0__0_chanx_left_out_13_[0]),
		.chanx_left_out_15_(sb_1__0__0_chanx_left_out_15_[0]),
		.chanx_left_out_17_(sb_1__0__0_chanx_left_out_17_[0]));

	sb_1__1_ sb_1__1_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_top_in_1_(cby_1__1__1_chany_out_1_[0]),
		.chany_top_in_3_(cby_1__1__1_chany_out_3_[0]),
		.chany_top_in_5_(cby_1__1__1_chany_out_5_[0]),
		.chany_top_in_7_(cby_1__1__1_chany_out_7_[0]),
		.chany_top_in_9_(cby_1__1__1_chany_out_9_[0]),
		.chany_top_in_11_(cby_1__1__1_chany_out_11_[0]),
		.chany_top_in_13_(cby_1__1__1_chany_out_13_[0]),
		.chany_top_in_15_(cby_1__1__1_chany_out_15_[0]),
		.chany_top_in_17_(cby_1__1__1_chany_out_17_[0]),
		.top_left_grid_pin_13_(grid_clb_1_right_width_0_height_0__pin_13_[0]),
		.top_right_grid_pin_11_(grid_clb_3_left_width_0_height_0__pin_11_[0]),
		.chanx_right_in_1_(cbx_1__1__1_chanx_out_1_[0]),
		.chanx_right_in_3_(cbx_1__1__1_chanx_out_3_[0]),
		.chanx_right_in_5_(cbx_1__1__1_chanx_out_5_[0]),
		.chanx_right_in_7_(cbx_1__1__1_chanx_out_7_[0]),
		.chanx_right_in_9_(cbx_1__1__1_chanx_out_9_[0]),
		.chanx_right_in_11_(cbx_1__1__1_chanx_out_11_[0]),
		.chanx_right_in_13_(cbx_1__1__1_chanx_out_13_[0]),
		.chanx_right_in_15_(cbx_1__1__1_chanx_out_15_[0]),
		.chanx_right_in_17_(cbx_1__1__1_chanx_out_17_[0]),
		.right_top_grid_pin_10_(grid_clb_3_bottom_width_0_height_0__pin_10_[0]),
		.right_bottom_grid_pin_12_(grid_clb_2_top_width_0_height_0__pin_12_[0]),
		.chany_bottom_in_0_(cby_1__1__0_chany_out_0_[0]),
		.chany_bottom_in_2_(cby_1__1__0_chany_out_2_[0]),
		.chany_bottom_in_4_(cby_1__1__0_chany_out_4_[0]),
		.chany_bottom_in_6_(cby_1__1__0_chany_out_6_[0]),
		.chany_bottom_in_8_(cby_1__1__0_chany_out_8_[0]),
		.chany_bottom_in_10_(cby_1__1__0_chany_out_10_[0]),
		.chany_bottom_in_12_(cby_1__1__0_chany_out_12_[0]),
		.chany_bottom_in_14_(cby_1__1__0_chany_out_14_[0]),
		.chany_bottom_in_16_(cby_1__1__0_chany_out_16_[0]),
		.bottom_right_grid_pin_11_(grid_clb_2_left_width_0_height_0__pin_11_[0]),
		.bottom_left_grid_pin_13_(grid_clb_0_right_width_0_height_0__pin_13_[0]),
		.chanx_left_in_0_(cbx_1__1__0_chanx_out_0_[0]),
		.chanx_left_in_2_(cbx_1__1__0_chanx_out_2_[0]),
		.chanx_left_in_4_(cbx_1__1__0_chanx_out_4_[0]),
		.chanx_left_in_6_(cbx_1__1__0_chanx_out_6_[0]),
		.chanx_left_in_8_(cbx_1__1__0_chanx_out_8_[0]),
		.chanx_left_in_10_(cbx_1__1__0_chanx_out_10_[0]),
		.chanx_left_in_12_(cbx_1__1__0_chanx_out_12_[0]),
		.chanx_left_in_14_(cbx_1__1__0_chanx_out_14_[0]),
		.chanx_left_in_16_(cbx_1__1__0_chanx_out_16_[0]),
		.left_top_grid_pin_10_(grid_clb_1_bottom_width_0_height_0__pin_10_[0]),
		.left_bottom_grid_pin_12_(grid_clb_0_top_width_0_height_0__pin_12_[0]),
		.enable(decoder6to33_0_data_out[29]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chany_top_out_0_(sb_1__1__0_chany_top_out_0_[0]),
		.chany_top_out_2_(sb_1__1__0_chany_top_out_2_[0]),
		.chany_top_out_4_(sb_1__1__0_chany_top_out_4_[0]),
		.chany_top_out_6_(sb_1__1__0_chany_top_out_6_[0]),
		.chany_top_out_8_(sb_1__1__0_chany_top_out_8_[0]),
		.chany_top_out_10_(sb_1__1__0_chany_top_out_10_[0]),
		.chany_top_out_12_(sb_1__1__0_chany_top_out_12_[0]),
		.chany_top_out_14_(sb_1__1__0_chany_top_out_14_[0]),
		.chany_top_out_16_(sb_1__1__0_chany_top_out_16_[0]),
		.chanx_right_out_0_(sb_1__1__0_chanx_right_out_0_[0]),
		.chanx_right_out_2_(sb_1__1__0_chanx_right_out_2_[0]),
		.chanx_right_out_4_(sb_1__1__0_chanx_right_out_4_[0]),
		.chanx_right_out_6_(sb_1__1__0_chanx_right_out_6_[0]),
		.chanx_right_out_8_(sb_1__1__0_chanx_right_out_8_[0]),
		.chanx_right_out_10_(sb_1__1__0_chanx_right_out_10_[0]),
		.chanx_right_out_12_(sb_1__1__0_chanx_right_out_12_[0]),
		.chanx_right_out_14_(sb_1__1__0_chanx_right_out_14_[0]),
		.chanx_right_out_16_(sb_1__1__0_chanx_right_out_16_[0]),
		.chany_bottom_out_1_(sb_1__1__0_chany_bottom_out_1_[0]),
		.chany_bottom_out_3_(sb_1__1__0_chany_bottom_out_3_[0]),
		.chany_bottom_out_5_(sb_1__1__0_chany_bottom_out_5_[0]),
		.chany_bottom_out_7_(sb_1__1__0_chany_bottom_out_7_[0]),
		.chany_bottom_out_9_(sb_1__1__0_chany_bottom_out_9_[0]),
		.chany_bottom_out_11_(sb_1__1__0_chany_bottom_out_11_[0]),
		.chany_bottom_out_13_(sb_1__1__0_chany_bottom_out_13_[0]),
		.chany_bottom_out_15_(sb_1__1__0_chany_bottom_out_15_[0]),
		.chany_bottom_out_17_(sb_1__1__0_chany_bottom_out_17_[0]),
		.chanx_left_out_1_(sb_1__1__0_chanx_left_out_1_[0]),
		.chanx_left_out_3_(sb_1__1__0_chanx_left_out_3_[0]),
		.chanx_left_out_5_(sb_1__1__0_chanx_left_out_5_[0]),
		.chanx_left_out_7_(sb_1__1__0_chanx_left_out_7_[0]),
		.chanx_left_out_9_(sb_1__1__0_chanx_left_out_9_[0]),
		.chanx_left_out_11_(sb_1__1__0_chanx_left_out_11_[0]),
		.chanx_left_out_13_(sb_1__1__0_chanx_left_out_13_[0]),
		.chanx_left_out_15_(sb_1__1__0_chanx_left_out_15_[0]),
		.chanx_left_out_17_(sb_1__1__0_chanx_left_out_17_[0]));

	sb_1__2_ sb_1__2_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chanx_right_in_1_(cbx_1__2__1_chanx_out_1_[0]),
		.chanx_right_in_3_(cbx_1__2__1_chanx_out_3_[0]),
		.chanx_right_in_5_(cbx_1__2__1_chanx_out_5_[0]),
		.chanx_right_in_7_(cbx_1__2__1_chanx_out_7_[0]),
		.chanx_right_in_9_(cbx_1__2__1_chanx_out_9_[0]),
		.chanx_right_in_11_(cbx_1__2__1_chanx_out_11_[0]),
		.chanx_right_in_13_(cbx_1__2__1_chanx_out_13_[0]),
		.chanx_right_in_15_(cbx_1__2__1_chanx_out_15_[0]),
		.chanx_right_in_17_(cbx_1__2__1_chanx_out_17_[0]),
		.right_top_grid_pin_1_(grid_io_top_1_bottom_width_0_height_0__pin_1_[0]),
		.right_top_grid_pin_3_(grid_io_top_1_bottom_width_0_height_0__pin_3_[0]),
		.right_top_grid_pin_5_(grid_io_top_1_bottom_width_0_height_0__pin_5_[0]),
		.right_top_grid_pin_7_(grid_io_top_1_bottom_width_0_height_0__pin_7_[0]),
		.right_top_grid_pin_9_(grid_io_top_1_bottom_width_0_height_0__pin_9_[0]),
		.right_top_grid_pin_11_(grid_io_top_1_bottom_width_0_height_0__pin_11_[0]),
		.right_top_grid_pin_13_(grid_io_top_1_bottom_width_0_height_0__pin_13_[0]),
		.right_top_grid_pin_15_(grid_io_top_1_bottom_width_0_height_0__pin_15_[0]),
		.right_bottom_grid_pin_12_(grid_clb_3_top_width_0_height_0__pin_12_[0]),
		.chany_bottom_in_0_(cby_1__1__1_chany_out_0_[0]),
		.chany_bottom_in_2_(cby_1__1__1_chany_out_2_[0]),
		.chany_bottom_in_4_(cby_1__1__1_chany_out_4_[0]),
		.chany_bottom_in_6_(cby_1__1__1_chany_out_6_[0]),
		.chany_bottom_in_8_(cby_1__1__1_chany_out_8_[0]),
		.chany_bottom_in_10_(cby_1__1__1_chany_out_10_[0]),
		.chany_bottom_in_12_(cby_1__1__1_chany_out_12_[0]),
		.chany_bottom_in_14_(cby_1__1__1_chany_out_14_[0]),
		.chany_bottom_in_16_(cby_1__1__1_chany_out_16_[0]),
		.bottom_right_grid_pin_11_(grid_clb_3_left_width_0_height_0__pin_11_[0]),
		.bottom_left_grid_pin_13_(grid_clb_1_right_width_0_height_0__pin_13_[0]),
		.chanx_left_in_0_(cbx_1__2__0_chanx_out_0_[0]),
		.chanx_left_in_2_(cbx_1__2__0_chanx_out_2_[0]),
		.chanx_left_in_4_(cbx_1__2__0_chanx_out_4_[0]),
		.chanx_left_in_6_(cbx_1__2__0_chanx_out_6_[0]),
		.chanx_left_in_8_(cbx_1__2__0_chanx_out_8_[0]),
		.chanx_left_in_10_(cbx_1__2__0_chanx_out_10_[0]),
		.chanx_left_in_12_(cbx_1__2__0_chanx_out_12_[0]),
		.chanx_left_in_14_(cbx_1__2__0_chanx_out_14_[0]),
		.chanx_left_in_16_(cbx_1__2__0_chanx_out_16_[0]),
		.left_top_grid_pin_1_(grid_io_top_0_bottom_width_0_height_0__pin_1_[0]),
		.left_top_grid_pin_3_(grid_io_top_0_bottom_width_0_height_0__pin_3_[0]),
		.left_top_grid_pin_5_(grid_io_top_0_bottom_width_0_height_0__pin_5_[0]),
		.left_top_grid_pin_7_(grid_io_top_0_bottom_width_0_height_0__pin_7_[0]),
		.left_top_grid_pin_9_(grid_io_top_0_bottom_width_0_height_0__pin_9_[0]),
		.left_top_grid_pin_11_(grid_io_top_0_bottom_width_0_height_0__pin_11_[0]),
		.left_top_grid_pin_13_(grid_io_top_0_bottom_width_0_height_0__pin_13_[0]),
		.left_top_grid_pin_15_(grid_io_top_0_bottom_width_0_height_0__pin_15_[0]),
		.left_bottom_grid_pin_12_(grid_clb_1_top_width_0_height_0__pin_12_[0]),
		.enable(decoder6to33_0_data_out[7]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chanx_right_out_0_(sb_1__2__0_chanx_right_out_0_[0]),
		.chanx_right_out_2_(sb_1__2__0_chanx_right_out_2_[0]),
		.chanx_right_out_4_(sb_1__2__0_chanx_right_out_4_[0]),
		.chanx_right_out_6_(sb_1__2__0_chanx_right_out_6_[0]),
		.chanx_right_out_8_(sb_1__2__0_chanx_right_out_8_[0]),
		.chanx_right_out_10_(sb_1__2__0_chanx_right_out_10_[0]),
		.chanx_right_out_12_(sb_1__2__0_chanx_right_out_12_[0]),
		.chanx_right_out_14_(sb_1__2__0_chanx_right_out_14_[0]),
		.chanx_right_out_16_(sb_1__2__0_chanx_right_out_16_[0]),
		.chany_bottom_out_1_(sb_1__2__0_chany_bottom_out_1_[0]),
		.chany_bottom_out_3_(sb_1__2__0_chany_bottom_out_3_[0]),
		.chany_bottom_out_5_(sb_1__2__0_chany_bottom_out_5_[0]),
		.chany_bottom_out_7_(sb_1__2__0_chany_bottom_out_7_[0]),
		.chany_bottom_out_9_(sb_1__2__0_chany_bottom_out_9_[0]),
		.chany_bottom_out_11_(sb_1__2__0_chany_bottom_out_11_[0]),
		.chany_bottom_out_13_(sb_1__2__0_chany_bottom_out_13_[0]),
		.chany_bottom_out_15_(sb_1__2__0_chany_bottom_out_15_[0]),
		.chany_bottom_out_17_(sb_1__2__0_chany_bottom_out_17_[0]),
		.chanx_left_out_1_(sb_1__2__0_chanx_left_out_1_[0]),
		.chanx_left_out_3_(sb_1__2__0_chanx_left_out_3_[0]),
		.chanx_left_out_5_(sb_1__2__0_chanx_left_out_5_[0]),
		.chanx_left_out_7_(sb_1__2__0_chanx_left_out_7_[0]),
		.chanx_left_out_9_(sb_1__2__0_chanx_left_out_9_[0]),
		.chanx_left_out_11_(sb_1__2__0_chanx_left_out_11_[0]),
		.chanx_left_out_13_(sb_1__2__0_chanx_left_out_13_[0]),
		.chanx_left_out_15_(sb_1__2__0_chanx_left_out_15_[0]),
		.chanx_left_out_17_(sb_1__2__0_chanx_left_out_17_[0]));

	sb_2__0_ sb_2__0_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_top_in_1_(cby_2__1__0_chany_out_1_[0]),
		.chany_top_in_3_(cby_2__1__0_chany_out_3_[0]),
		.chany_top_in_5_(cby_2__1__0_chany_out_5_[0]),
		.chany_top_in_7_(cby_2__1__0_chany_out_7_[0]),
		.chany_top_in_9_(cby_2__1__0_chany_out_9_[0]),
		.chany_top_in_11_(cby_2__1__0_chany_out_11_[0]),
		.chany_top_in_13_(cby_2__1__0_chany_out_13_[0]),
		.chany_top_in_15_(cby_2__1__0_chany_out_15_[0]),
		.chany_top_in_17_(cby_2__1__0_chany_out_17_[0]),
		.top_left_grid_pin_13_(grid_clb_2_right_width_0_height_0__pin_13_[0]),
		.top_right_grid_pin_1_(grid_io_right_0_left_width_0_height_0__pin_1_[0]),
		.top_right_grid_pin_3_(grid_io_right_0_left_width_0_height_0__pin_3_[0]),
		.top_right_grid_pin_5_(grid_io_right_0_left_width_0_height_0__pin_5_[0]),
		.top_right_grid_pin_7_(grid_io_right_0_left_width_0_height_0__pin_7_[0]),
		.top_right_grid_pin_9_(grid_io_right_0_left_width_0_height_0__pin_9_[0]),
		.top_right_grid_pin_11_(grid_io_right_0_left_width_0_height_0__pin_11_[0]),
		.top_right_grid_pin_13_(grid_io_right_0_left_width_0_height_0__pin_13_[0]),
		.top_right_grid_pin_15_(grid_io_right_0_left_width_0_height_0__pin_15_[0]),
		.chanx_left_in_0_(cbx_1__0__1_chanx_out_0_[0]),
		.chanx_left_in_2_(cbx_1__0__1_chanx_out_2_[0]),
		.chanx_left_in_4_(cbx_1__0__1_chanx_out_4_[0]),
		.chanx_left_in_6_(cbx_1__0__1_chanx_out_6_[0]),
		.chanx_left_in_8_(cbx_1__0__1_chanx_out_8_[0]),
		.chanx_left_in_10_(cbx_1__0__1_chanx_out_10_[0]),
		.chanx_left_in_12_(cbx_1__0__1_chanx_out_12_[0]),
		.chanx_left_in_14_(cbx_1__0__1_chanx_out_14_[0]),
		.chanx_left_in_16_(cbx_1__0__1_chanx_out_16_[0]),
		.left_top_grid_pin_10_(grid_clb_2_bottom_width_0_height_0__pin_10_[0]),
		.left_bottom_grid_pin_1_(grid_io_bottom_1_top_width_0_height_0__pin_1_[0]),
		.left_bottom_grid_pin_3_(grid_io_bottom_1_top_width_0_height_0__pin_3_[0]),
		.left_bottom_grid_pin_5_(grid_io_bottom_1_top_width_0_height_0__pin_5_[0]),
		.left_bottom_grid_pin_7_(grid_io_bottom_1_top_width_0_height_0__pin_7_[0]),
		.left_bottom_grid_pin_9_(grid_io_bottom_1_top_width_0_height_0__pin_9_[0]),
		.left_bottom_grid_pin_11_(grid_io_bottom_1_top_width_0_height_0__pin_11_[0]),
		.left_bottom_grid_pin_13_(grid_io_bottom_1_top_width_0_height_0__pin_13_[0]),
		.left_bottom_grid_pin_15_(grid_io_bottom_1_top_width_0_height_0__pin_15_[0]),
		.enable(decoder6to33_0_data_out[21]),
		.address(address[0:5]),
		.data_in(data_in[0]),
		.chany_top_out_0_(sb_2__0__0_chany_top_out_0_[0]),
		.chany_top_out_2_(sb_2__0__0_chany_top_out_2_[0]),
		.chany_top_out_4_(sb_2__0__0_chany_top_out_4_[0]),
		.chany_top_out_6_(sb_2__0__0_chany_top_out_6_[0]),
		.chany_top_out_8_(sb_2__0__0_chany_top_out_8_[0]),
		.chany_top_out_10_(sb_2__0__0_chany_top_out_10_[0]),
		.chany_top_out_12_(sb_2__0__0_chany_top_out_12_[0]),
		.chany_top_out_14_(sb_2__0__0_chany_top_out_14_[0]),
		.chany_top_out_16_(sb_2__0__0_chany_top_out_16_[0]),
		.chanx_left_out_1_(sb_2__0__0_chanx_left_out_1_[0]),
		.chanx_left_out_3_(sb_2__0__0_chanx_left_out_3_[0]),
		.chanx_left_out_5_(sb_2__0__0_chanx_left_out_5_[0]),
		.chanx_left_out_7_(sb_2__0__0_chanx_left_out_7_[0]),
		.chanx_left_out_9_(sb_2__0__0_chanx_left_out_9_[0]),
		.chanx_left_out_11_(sb_2__0__0_chanx_left_out_11_[0]),
		.chanx_left_out_13_(sb_2__0__0_chanx_left_out_13_[0]),
		.chanx_left_out_15_(sb_2__0__0_chanx_left_out_15_[0]),
		.chanx_left_out_17_(sb_2__0__0_chanx_left_out_17_[0]));

	sb_2__1_ sb_2__1_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_top_in_1_(cby_2__1__1_chany_out_1_[0]),
		.chany_top_in_3_(cby_2__1__1_chany_out_3_[0]),
		.chany_top_in_5_(cby_2__1__1_chany_out_5_[0]),
		.chany_top_in_7_(cby_2__1__1_chany_out_7_[0]),
		.chany_top_in_9_(cby_2__1__1_chany_out_9_[0]),
		.chany_top_in_11_(cby_2__1__1_chany_out_11_[0]),
		.chany_top_in_13_(cby_2__1__1_chany_out_13_[0]),
		.chany_top_in_15_(cby_2__1__1_chany_out_15_[0]),
		.chany_top_in_17_(cby_2__1__1_chany_out_17_[0]),
		.top_left_grid_pin_13_(grid_clb_3_right_width_0_height_0__pin_13_[0]),
		.top_right_grid_pin_1_(grid_io_right_1_left_width_0_height_0__pin_1_[0]),
		.top_right_grid_pin_3_(grid_io_right_1_left_width_0_height_0__pin_3_[0]),
		.top_right_grid_pin_5_(grid_io_right_1_left_width_0_height_0__pin_5_[0]),
		.top_right_grid_pin_7_(grid_io_right_1_left_width_0_height_0__pin_7_[0]),
		.top_right_grid_pin_9_(grid_io_right_1_left_width_0_height_0__pin_9_[0]),
		.top_right_grid_pin_11_(grid_io_right_1_left_width_0_height_0__pin_11_[0]),
		.top_right_grid_pin_13_(grid_io_right_1_left_width_0_height_0__pin_13_[0]),
		.top_right_grid_pin_15_(grid_io_right_1_left_width_0_height_0__pin_15_[0]),
		.chany_bottom_in_0_(cby_2__1__0_chany_out_0_[0]),
		.chany_bottom_in_2_(cby_2__1__0_chany_out_2_[0]),
		.chany_bottom_in_4_(cby_2__1__0_chany_out_4_[0]),
		.chany_bottom_in_6_(cby_2__1__0_chany_out_6_[0]),
		.chany_bottom_in_8_(cby_2__1__0_chany_out_8_[0]),
		.chany_bottom_in_10_(cby_2__1__0_chany_out_10_[0]),
		.chany_bottom_in_12_(cby_2__1__0_chany_out_12_[0]),
		.chany_bottom_in_14_(cby_2__1__0_chany_out_14_[0]),
		.chany_bottom_in_16_(cby_2__1__0_chany_out_16_[0]),
		.bottom_right_grid_pin_1_(grid_io_right_0_left_width_0_height_0__pin_1_[0]),
		.bottom_right_grid_pin_3_(grid_io_right_0_left_width_0_height_0__pin_3_[0]),
		.bottom_right_grid_pin_5_(grid_io_right_0_left_width_0_height_0__pin_5_[0]),
		.bottom_right_grid_pin_7_(grid_io_right_0_left_width_0_height_0__pin_7_[0]),
		.bottom_right_grid_pin_9_(grid_io_right_0_left_width_0_height_0__pin_9_[0]),
		.bottom_right_grid_pin_11_(grid_io_right_0_left_width_0_height_0__pin_11_[0]),
		.bottom_right_grid_pin_13_(grid_io_right_0_left_width_0_height_0__pin_13_[0]),
		.bottom_right_grid_pin_15_(grid_io_right_0_left_width_0_height_0__pin_15_[0]),
		.bottom_left_grid_pin_13_(grid_clb_2_right_width_0_height_0__pin_13_[0]),
		.chanx_left_in_0_(cbx_1__1__1_chanx_out_0_[0]),
		.chanx_left_in_2_(cbx_1__1__1_chanx_out_2_[0]),
		.chanx_left_in_4_(cbx_1__1__1_chanx_out_4_[0]),
		.chanx_left_in_6_(cbx_1__1__1_chanx_out_6_[0]),
		.chanx_left_in_8_(cbx_1__1__1_chanx_out_8_[0]),
		.chanx_left_in_10_(cbx_1__1__1_chanx_out_10_[0]),
		.chanx_left_in_12_(cbx_1__1__1_chanx_out_12_[0]),
		.chanx_left_in_14_(cbx_1__1__1_chanx_out_14_[0]),
		.chanx_left_in_16_(cbx_1__1__1_chanx_out_16_[0]),
		.left_top_grid_pin_10_(grid_clb_3_bottom_width_0_height_0__pin_10_[0]),
		.left_bottom_grid_pin_12_(grid_clb_2_top_width_0_height_0__pin_12_[0]),
		.enable(decoder6to33_0_data_out[25]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chany_top_out_0_(sb_2__1__0_chany_top_out_0_[0]),
		.chany_top_out_2_(sb_2__1__0_chany_top_out_2_[0]),
		.chany_top_out_4_(sb_2__1__0_chany_top_out_4_[0]),
		.chany_top_out_6_(sb_2__1__0_chany_top_out_6_[0]),
		.chany_top_out_8_(sb_2__1__0_chany_top_out_8_[0]),
		.chany_top_out_10_(sb_2__1__0_chany_top_out_10_[0]),
		.chany_top_out_12_(sb_2__1__0_chany_top_out_12_[0]),
		.chany_top_out_14_(sb_2__1__0_chany_top_out_14_[0]),
		.chany_top_out_16_(sb_2__1__0_chany_top_out_16_[0]),
		.chany_bottom_out_1_(sb_2__1__0_chany_bottom_out_1_[0]),
		.chany_bottom_out_3_(sb_2__1__0_chany_bottom_out_3_[0]),
		.chany_bottom_out_5_(sb_2__1__0_chany_bottom_out_5_[0]),
		.chany_bottom_out_7_(sb_2__1__0_chany_bottom_out_7_[0]),
		.chany_bottom_out_9_(sb_2__1__0_chany_bottom_out_9_[0]),
		.chany_bottom_out_11_(sb_2__1__0_chany_bottom_out_11_[0]),
		.chany_bottom_out_13_(sb_2__1__0_chany_bottom_out_13_[0]),
		.chany_bottom_out_15_(sb_2__1__0_chany_bottom_out_15_[0]),
		.chany_bottom_out_17_(sb_2__1__0_chany_bottom_out_17_[0]),
		.chanx_left_out_1_(sb_2__1__0_chanx_left_out_1_[0]),
		.chanx_left_out_3_(sb_2__1__0_chanx_left_out_3_[0]),
		.chanx_left_out_5_(sb_2__1__0_chanx_left_out_5_[0]),
		.chanx_left_out_7_(sb_2__1__0_chanx_left_out_7_[0]),
		.chanx_left_out_9_(sb_2__1__0_chanx_left_out_9_[0]),
		.chanx_left_out_11_(sb_2__1__0_chanx_left_out_11_[0]),
		.chanx_left_out_13_(sb_2__1__0_chanx_left_out_13_[0]),
		.chanx_left_out_15_(sb_2__1__0_chanx_left_out_15_[0]),
		.chanx_left_out_17_(sb_2__1__0_chanx_left_out_17_[0]));

	sb_2__2_ sb_2__2_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_bottom_in_0_(cby_2__1__1_chany_out_0_[0]),
		.chany_bottom_in_2_(cby_2__1__1_chany_out_2_[0]),
		.chany_bottom_in_4_(cby_2__1__1_chany_out_4_[0]),
		.chany_bottom_in_6_(cby_2__1__1_chany_out_6_[0]),
		.chany_bottom_in_8_(cby_2__1__1_chany_out_8_[0]),
		.chany_bottom_in_10_(cby_2__1__1_chany_out_10_[0]),
		.chany_bottom_in_12_(cby_2__1__1_chany_out_12_[0]),
		.chany_bottom_in_14_(cby_2__1__1_chany_out_14_[0]),
		.chany_bottom_in_16_(cby_2__1__1_chany_out_16_[0]),
		.bottom_right_grid_pin_1_(grid_io_right_1_left_width_0_height_0__pin_1_[0]),
		.bottom_right_grid_pin_3_(grid_io_right_1_left_width_0_height_0__pin_3_[0]),
		.bottom_right_grid_pin_5_(grid_io_right_1_left_width_0_height_0__pin_5_[0]),
		.bottom_right_grid_pin_7_(grid_io_right_1_left_width_0_height_0__pin_7_[0]),
		.bottom_right_grid_pin_9_(grid_io_right_1_left_width_0_height_0__pin_9_[0]),
		.bottom_right_grid_pin_11_(grid_io_right_1_left_width_0_height_0__pin_11_[0]),
		.bottom_right_grid_pin_13_(grid_io_right_1_left_width_0_height_0__pin_13_[0]),
		.bottom_right_grid_pin_15_(grid_io_right_1_left_width_0_height_0__pin_15_[0]),
		.bottom_left_grid_pin_13_(grid_clb_3_right_width_0_height_0__pin_13_[0]),
		.chanx_left_in_0_(cbx_1__2__1_chanx_out_0_[0]),
		.chanx_left_in_2_(cbx_1__2__1_chanx_out_2_[0]),
		.chanx_left_in_4_(cbx_1__2__1_chanx_out_4_[0]),
		.chanx_left_in_6_(cbx_1__2__1_chanx_out_6_[0]),
		.chanx_left_in_8_(cbx_1__2__1_chanx_out_8_[0]),
		.chanx_left_in_10_(cbx_1__2__1_chanx_out_10_[0]),
		.chanx_left_in_12_(cbx_1__2__1_chanx_out_12_[0]),
		.chanx_left_in_14_(cbx_1__2__1_chanx_out_14_[0]),
		.chanx_left_in_16_(cbx_1__2__1_chanx_out_16_[0]),
		.left_top_grid_pin_1_(grid_io_top_1_bottom_width_0_height_0__pin_1_[0]),
		.left_top_grid_pin_3_(grid_io_top_1_bottom_width_0_height_0__pin_3_[0]),
		.left_top_grid_pin_5_(grid_io_top_1_bottom_width_0_height_0__pin_5_[0]),
		.left_top_grid_pin_7_(grid_io_top_1_bottom_width_0_height_0__pin_7_[0]),
		.left_top_grid_pin_9_(grid_io_top_1_bottom_width_0_height_0__pin_9_[0]),
		.left_top_grid_pin_11_(grid_io_top_1_bottom_width_0_height_0__pin_11_[0]),
		.left_top_grid_pin_13_(grid_io_top_1_bottom_width_0_height_0__pin_13_[0]),
		.left_top_grid_pin_15_(grid_io_top_1_bottom_width_0_height_0__pin_15_[0]),
		.left_bottom_grid_pin_12_(grid_clb_3_top_width_0_height_0__pin_12_[0]),
		.enable(decoder6to33_0_data_out[4]),
		.address(address[0:5]),
		.data_in(data_in[0]),
		.chany_bottom_out_1_(sb_2__2__0_chany_bottom_out_1_[0]),
		.chany_bottom_out_3_(sb_2__2__0_chany_bottom_out_3_[0]),
		.chany_bottom_out_5_(sb_2__2__0_chany_bottom_out_5_[0]),
		.chany_bottom_out_7_(sb_2__2__0_chany_bottom_out_7_[0]),
		.chany_bottom_out_9_(sb_2__2__0_chany_bottom_out_9_[0]),
		.chany_bottom_out_11_(sb_2__2__0_chany_bottom_out_11_[0]),
		.chany_bottom_out_13_(sb_2__2__0_chany_bottom_out_13_[0]),
		.chany_bottom_out_15_(sb_2__2__0_chany_bottom_out_15_[0]),
		.chany_bottom_out_17_(sb_2__2__0_chany_bottom_out_17_[0]),
		.chanx_left_out_1_(sb_2__2__0_chanx_left_out_1_[0]),
		.chanx_left_out_3_(sb_2__2__0_chanx_left_out_3_[0]),
		.chanx_left_out_5_(sb_2__2__0_chanx_left_out_5_[0]),
		.chanx_left_out_7_(sb_2__2__0_chanx_left_out_7_[0]),
		.chanx_left_out_9_(sb_2__2__0_chanx_left_out_9_[0]),
		.chanx_left_out_11_(sb_2__2__0_chanx_left_out_11_[0]),
		.chanx_left_out_13_(sb_2__2__0_chanx_left_out_13_[0]),
		.chanx_left_out_15_(sb_2__2__0_chanx_left_out_15_[0]),
		.chanx_left_out_17_(sb_2__2__0_chanx_left_out_17_[0]));

	cbx_1__0_ cbx_1__0_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chanx_in_0_(sb_0__0__0_chanx_right_out_0_[0]),
		.chanx_in_1_(sb_1__0__0_chanx_left_out_1_[0]),
		.chanx_in_2_(sb_0__0__0_chanx_right_out_2_[0]),
		.chanx_in_3_(sb_1__0__0_chanx_left_out_3_[0]),
		.chanx_in_4_(sb_0__0__0_chanx_right_out_4_[0]),
		.chanx_in_5_(sb_1__0__0_chanx_left_out_5_[0]),
		.chanx_in_6_(sb_0__0__0_chanx_right_out_6_[0]),
		.chanx_in_7_(sb_1__0__0_chanx_left_out_7_[0]),
		.chanx_in_8_(sb_0__0__0_chanx_right_out_8_[0]),
		.chanx_in_9_(sb_1__0__0_chanx_left_out_9_[0]),
		.chanx_in_10_(sb_0__0__0_chanx_right_out_10_[0]),
		.chanx_in_11_(sb_1__0__0_chanx_left_out_11_[0]),
		.chanx_in_12_(sb_0__0__0_chanx_right_out_12_[0]),
		.chanx_in_13_(sb_1__0__0_chanx_left_out_13_[0]),
		.chanx_in_14_(sb_0__0__0_chanx_right_out_14_[0]),
		.chanx_in_15_(sb_1__0__0_chanx_left_out_15_[0]),
		.chanx_in_16_(sb_0__0__0_chanx_right_out_16_[0]),
		.chanx_in_17_(sb_1__0__0_chanx_left_out_17_[0]),
		.enable(decoder6to33_0_data_out[18]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chanx_out_0_(cbx_1__0__0_chanx_out_0_[0]),
		.chanx_out_1_(cbx_1__0__0_chanx_out_1_[0]),
		.chanx_out_2_(cbx_1__0__0_chanx_out_2_[0]),
		.chanx_out_3_(cbx_1__0__0_chanx_out_3_[0]),
		.chanx_out_4_(cbx_1__0__0_chanx_out_4_[0]),
		.chanx_out_5_(cbx_1__0__0_chanx_out_5_[0]),
		.chanx_out_6_(cbx_1__0__0_chanx_out_6_[0]),
		.chanx_out_7_(cbx_1__0__0_chanx_out_7_[0]),
		.chanx_out_8_(cbx_1__0__0_chanx_out_8_[0]),
		.chanx_out_9_(cbx_1__0__0_chanx_out_9_[0]),
		.chanx_out_10_(cbx_1__0__0_chanx_out_10_[0]),
		.chanx_out_11_(cbx_1__0__0_chanx_out_11_[0]),
		.chanx_out_12_(cbx_1__0__0_chanx_out_12_[0]),
		.chanx_out_13_(cbx_1__0__0_chanx_out_13_[0]),
		.chanx_out_14_(cbx_1__0__0_chanx_out_14_[0]),
		.chanx_out_15_(cbx_1__0__0_chanx_out_15_[0]),
		.chanx_out_16_(cbx_1__0__0_chanx_out_16_[0]),
		.chanx_out_17_(cbx_1__0__0_chanx_out_17_[0]),
		.top_grid_pin_2_(cbx_1__0__0_top_grid_pin_2_[0]),
		.top_grid_pin_6_(cbx_1__0__0_top_grid_pin_6_[0]),
		.top_grid_pin_14_(cbx_1__0__0_top_grid_pin_14_[0]),
		.bottom_grid_pin_0_(cbx_1__0__0_bottom_grid_pin_0_[0]),
		.bottom_grid_pin_2_(cbx_1__0__0_bottom_grid_pin_2_[0]),
		.bottom_grid_pin_4_(cbx_1__0__0_bottom_grid_pin_4_[0]),
		.bottom_grid_pin_6_(cbx_1__0__0_bottom_grid_pin_6_[0]),
		.bottom_grid_pin_8_(cbx_1__0__0_bottom_grid_pin_8_[0]),
		.bottom_grid_pin_10_(cbx_1__0__0_bottom_grid_pin_10_[0]),
		.bottom_grid_pin_12_(cbx_1__0__0_bottom_grid_pin_12_[0]),
		.bottom_grid_pin_14_(cbx_1__0__0_bottom_grid_pin_14_[0]));

	cbx_1__0_ cbx_2__0_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chanx_in_0_(sb_1__0__0_chanx_right_out_0_[0]),
		.chanx_in_1_(sb_2__0__0_chanx_left_out_1_[0]),
		.chanx_in_2_(sb_1__0__0_chanx_right_out_2_[0]),
		.chanx_in_3_(sb_2__0__0_chanx_left_out_3_[0]),
		.chanx_in_4_(sb_1__0__0_chanx_right_out_4_[0]),
		.chanx_in_5_(sb_2__0__0_chanx_left_out_5_[0]),
		.chanx_in_6_(sb_1__0__0_chanx_right_out_6_[0]),
		.chanx_in_7_(sb_2__0__0_chanx_left_out_7_[0]),
		.chanx_in_8_(sb_1__0__0_chanx_right_out_8_[0]),
		.chanx_in_9_(sb_2__0__0_chanx_left_out_9_[0]),
		.chanx_in_10_(sb_1__0__0_chanx_right_out_10_[0]),
		.chanx_in_11_(sb_2__0__0_chanx_left_out_11_[0]),
		.chanx_in_12_(sb_1__0__0_chanx_right_out_12_[0]),
		.chanx_in_13_(sb_2__0__0_chanx_left_out_13_[0]),
		.chanx_in_14_(sb_1__0__0_chanx_right_out_14_[0]),
		.chanx_in_15_(sb_2__0__0_chanx_left_out_15_[0]),
		.chanx_in_16_(sb_1__0__0_chanx_right_out_16_[0]),
		.chanx_in_17_(sb_2__0__0_chanx_left_out_17_[0]),
		.enable(decoder6to33_0_data_out[22]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chanx_out_0_(cbx_1__0__1_chanx_out_0_[0]),
		.chanx_out_1_(cbx_1__0__1_chanx_out_1_[0]),
		.chanx_out_2_(cbx_1__0__1_chanx_out_2_[0]),
		.chanx_out_3_(cbx_1__0__1_chanx_out_3_[0]),
		.chanx_out_4_(cbx_1__0__1_chanx_out_4_[0]),
		.chanx_out_5_(cbx_1__0__1_chanx_out_5_[0]),
		.chanx_out_6_(cbx_1__0__1_chanx_out_6_[0]),
		.chanx_out_7_(cbx_1__0__1_chanx_out_7_[0]),
		.chanx_out_8_(cbx_1__0__1_chanx_out_8_[0]),
		.chanx_out_9_(cbx_1__0__1_chanx_out_9_[0]),
		.chanx_out_10_(cbx_1__0__1_chanx_out_10_[0]),
		.chanx_out_11_(cbx_1__0__1_chanx_out_11_[0]),
		.chanx_out_12_(cbx_1__0__1_chanx_out_12_[0]),
		.chanx_out_13_(cbx_1__0__1_chanx_out_13_[0]),
		.chanx_out_14_(cbx_1__0__1_chanx_out_14_[0]),
		.chanx_out_15_(cbx_1__0__1_chanx_out_15_[0]),
		.chanx_out_16_(cbx_1__0__1_chanx_out_16_[0]),
		.chanx_out_17_(cbx_1__0__1_chanx_out_17_[0]),
		.top_grid_pin_2_(cbx_1__0__1_top_grid_pin_2_[0]),
		.top_grid_pin_6_(cbx_1__0__1_top_grid_pin_6_[0]),
		.top_grid_pin_14_(cbx_1__0__1_top_grid_pin_14_[0]),
		.bottom_grid_pin_0_(cbx_1__0__1_bottom_grid_pin_0_[0]),
		.bottom_grid_pin_2_(cbx_1__0__1_bottom_grid_pin_2_[0]),
		.bottom_grid_pin_4_(cbx_1__0__1_bottom_grid_pin_4_[0]),
		.bottom_grid_pin_6_(cbx_1__0__1_bottom_grid_pin_6_[0]),
		.bottom_grid_pin_8_(cbx_1__0__1_bottom_grid_pin_8_[0]),
		.bottom_grid_pin_10_(cbx_1__0__1_bottom_grid_pin_10_[0]),
		.bottom_grid_pin_12_(cbx_1__0__1_bottom_grid_pin_12_[0]),
		.bottom_grid_pin_14_(cbx_1__0__1_bottom_grid_pin_14_[0]));

	cbx_1__1_ cbx_1__1_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chanx_in_0_(sb_0__1__0_chanx_right_out_0_[0]),
		.chanx_in_1_(sb_1__1__0_chanx_left_out_1_[0]),
		.chanx_in_2_(sb_0__1__0_chanx_right_out_2_[0]),
		.chanx_in_3_(sb_1__1__0_chanx_left_out_3_[0]),
		.chanx_in_4_(sb_0__1__0_chanx_right_out_4_[0]),
		.chanx_in_5_(sb_1__1__0_chanx_left_out_5_[0]),
		.chanx_in_6_(sb_0__1__0_chanx_right_out_6_[0]),
		.chanx_in_7_(sb_1__1__0_chanx_left_out_7_[0]),
		.chanx_in_8_(sb_0__1__0_chanx_right_out_8_[0]),
		.chanx_in_9_(sb_1__1__0_chanx_left_out_9_[0]),
		.chanx_in_10_(sb_0__1__0_chanx_right_out_10_[0]),
		.chanx_in_11_(sb_1__1__0_chanx_left_out_11_[0]),
		.chanx_in_12_(sb_0__1__0_chanx_right_out_12_[0]),
		.chanx_in_13_(sb_1__1__0_chanx_left_out_13_[0]),
		.chanx_in_14_(sb_0__1__0_chanx_right_out_14_[0]),
		.chanx_in_15_(sb_1__1__0_chanx_left_out_15_[0]),
		.chanx_in_16_(sb_0__1__0_chanx_right_out_16_[0]),
		.chanx_in_17_(sb_1__1__0_chanx_left_out_17_[0]),
		.enable(decoder6to33_0_data_out[30]),
		.address(address[0:5]),
		.data_in(data_in[0]),
		.chanx_out_0_(cbx_1__1__0_chanx_out_0_[0]),
		.chanx_out_1_(cbx_1__1__0_chanx_out_1_[0]),
		.chanx_out_2_(cbx_1__1__0_chanx_out_2_[0]),
		.chanx_out_3_(cbx_1__1__0_chanx_out_3_[0]),
		.chanx_out_4_(cbx_1__1__0_chanx_out_4_[0]),
		.chanx_out_5_(cbx_1__1__0_chanx_out_5_[0]),
		.chanx_out_6_(cbx_1__1__0_chanx_out_6_[0]),
		.chanx_out_7_(cbx_1__1__0_chanx_out_7_[0]),
		.chanx_out_8_(cbx_1__1__0_chanx_out_8_[0]),
		.chanx_out_9_(cbx_1__1__0_chanx_out_9_[0]),
		.chanx_out_10_(cbx_1__1__0_chanx_out_10_[0]),
		.chanx_out_11_(cbx_1__1__0_chanx_out_11_[0]),
		.chanx_out_12_(cbx_1__1__0_chanx_out_12_[0]),
		.chanx_out_13_(cbx_1__1__0_chanx_out_13_[0]),
		.chanx_out_14_(cbx_1__1__0_chanx_out_14_[0]),
		.chanx_out_15_(cbx_1__1__0_chanx_out_15_[0]),
		.chanx_out_16_(cbx_1__1__0_chanx_out_16_[0]),
		.chanx_out_17_(cbx_1__1__0_chanx_out_17_[0]),
		.top_grid_pin_2_(cbx_1__1__0_top_grid_pin_2_[0]),
		.top_grid_pin_6_(cbx_1__1__0_top_grid_pin_6_[0]),
		.top_grid_pin_14_(cbx_1__1__0_top_grid_pin_14_[0]),
		.bottom_grid_pin_0_(cbx_1__1__0_bottom_grid_pin_0_[0]),
		.bottom_grid_pin_4_(cbx_1__1__0_bottom_grid_pin_4_[0]),
		.bottom_grid_pin_8_(cbx_1__1__0_bottom_grid_pin_8_[0]));

	cbx_1__1_ cbx_2__1_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chanx_in_0_(sb_1__1__0_chanx_right_out_0_[0]),
		.chanx_in_1_(sb_2__1__0_chanx_left_out_1_[0]),
		.chanx_in_2_(sb_1__1__0_chanx_right_out_2_[0]),
		.chanx_in_3_(sb_2__1__0_chanx_left_out_3_[0]),
		.chanx_in_4_(sb_1__1__0_chanx_right_out_4_[0]),
		.chanx_in_5_(sb_2__1__0_chanx_left_out_5_[0]),
		.chanx_in_6_(sb_1__1__0_chanx_right_out_6_[0]),
		.chanx_in_7_(sb_2__1__0_chanx_left_out_7_[0]),
		.chanx_in_8_(sb_1__1__0_chanx_right_out_8_[0]),
		.chanx_in_9_(sb_2__1__0_chanx_left_out_9_[0]),
		.chanx_in_10_(sb_1__1__0_chanx_right_out_10_[0]),
		.chanx_in_11_(sb_2__1__0_chanx_left_out_11_[0]),
		.chanx_in_12_(sb_1__1__0_chanx_right_out_12_[0]),
		.chanx_in_13_(sb_2__1__0_chanx_left_out_13_[0]),
		.chanx_in_14_(sb_1__1__0_chanx_right_out_14_[0]),
		.chanx_in_15_(sb_2__1__0_chanx_left_out_15_[0]),
		.chanx_in_16_(sb_1__1__0_chanx_right_out_16_[0]),
		.chanx_in_17_(sb_2__1__0_chanx_left_out_17_[0]),
		.enable(decoder6to33_0_data_out[26]),
		.address(address[0:5]),
		.data_in(data_in[0]),
		.chanx_out_0_(cbx_1__1__1_chanx_out_0_[0]),
		.chanx_out_1_(cbx_1__1__1_chanx_out_1_[0]),
		.chanx_out_2_(cbx_1__1__1_chanx_out_2_[0]),
		.chanx_out_3_(cbx_1__1__1_chanx_out_3_[0]),
		.chanx_out_4_(cbx_1__1__1_chanx_out_4_[0]),
		.chanx_out_5_(cbx_1__1__1_chanx_out_5_[0]),
		.chanx_out_6_(cbx_1__1__1_chanx_out_6_[0]),
		.chanx_out_7_(cbx_1__1__1_chanx_out_7_[0]),
		.chanx_out_8_(cbx_1__1__1_chanx_out_8_[0]),
		.chanx_out_9_(cbx_1__1__1_chanx_out_9_[0]),
		.chanx_out_10_(cbx_1__1__1_chanx_out_10_[0]),
		.chanx_out_11_(cbx_1__1__1_chanx_out_11_[0]),
		.chanx_out_12_(cbx_1__1__1_chanx_out_12_[0]),
		.chanx_out_13_(cbx_1__1__1_chanx_out_13_[0]),
		.chanx_out_14_(cbx_1__1__1_chanx_out_14_[0]),
		.chanx_out_15_(cbx_1__1__1_chanx_out_15_[0]),
		.chanx_out_16_(cbx_1__1__1_chanx_out_16_[0]),
		.chanx_out_17_(cbx_1__1__1_chanx_out_17_[0]),
		.top_grid_pin_2_(cbx_1__1__1_top_grid_pin_2_[0]),
		.top_grid_pin_6_(cbx_1__1__1_top_grid_pin_6_[0]),
		.top_grid_pin_14_(cbx_1__1__1_top_grid_pin_14_[0]),
		.bottom_grid_pin_0_(cbx_1__1__1_bottom_grid_pin_0_[0]),
		.bottom_grid_pin_4_(cbx_1__1__1_bottom_grid_pin_4_[0]),
		.bottom_grid_pin_8_(cbx_1__1__1_bottom_grid_pin_8_[0]));

	cbx_1__2_ cbx_1__2_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chanx_in_0_(sb_0__2__0_chanx_right_out_0_[0]),
		.chanx_in_1_(sb_1__2__0_chanx_left_out_1_[0]),
		.chanx_in_2_(sb_0__2__0_chanx_right_out_2_[0]),
		.chanx_in_3_(sb_1__2__0_chanx_left_out_3_[0]),
		.chanx_in_4_(sb_0__2__0_chanx_right_out_4_[0]),
		.chanx_in_5_(sb_1__2__0_chanx_left_out_5_[0]),
		.chanx_in_6_(sb_0__2__0_chanx_right_out_6_[0]),
		.chanx_in_7_(sb_1__2__0_chanx_left_out_7_[0]),
		.chanx_in_8_(sb_0__2__0_chanx_right_out_8_[0]),
		.chanx_in_9_(sb_1__2__0_chanx_left_out_9_[0]),
		.chanx_in_10_(sb_0__2__0_chanx_right_out_10_[0]),
		.chanx_in_11_(sb_1__2__0_chanx_left_out_11_[0]),
		.chanx_in_12_(sb_0__2__0_chanx_right_out_12_[0]),
		.chanx_in_13_(sb_1__2__0_chanx_left_out_13_[0]),
		.chanx_in_14_(sb_0__2__0_chanx_right_out_14_[0]),
		.chanx_in_15_(sb_1__2__0_chanx_left_out_15_[0]),
		.chanx_in_16_(sb_0__2__0_chanx_right_out_16_[0]),
		.chanx_in_17_(sb_1__2__0_chanx_left_out_17_[0]),
		.enable(decoder6to33_0_data_out[8]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chanx_out_0_(cbx_1__2__0_chanx_out_0_[0]),
		.chanx_out_1_(cbx_1__2__0_chanx_out_1_[0]),
		.chanx_out_2_(cbx_1__2__0_chanx_out_2_[0]),
		.chanx_out_3_(cbx_1__2__0_chanx_out_3_[0]),
		.chanx_out_4_(cbx_1__2__0_chanx_out_4_[0]),
		.chanx_out_5_(cbx_1__2__0_chanx_out_5_[0]),
		.chanx_out_6_(cbx_1__2__0_chanx_out_6_[0]),
		.chanx_out_7_(cbx_1__2__0_chanx_out_7_[0]),
		.chanx_out_8_(cbx_1__2__0_chanx_out_8_[0]),
		.chanx_out_9_(cbx_1__2__0_chanx_out_9_[0]),
		.chanx_out_10_(cbx_1__2__0_chanx_out_10_[0]),
		.chanx_out_11_(cbx_1__2__0_chanx_out_11_[0]),
		.chanx_out_12_(cbx_1__2__0_chanx_out_12_[0]),
		.chanx_out_13_(cbx_1__2__0_chanx_out_13_[0]),
		.chanx_out_14_(cbx_1__2__0_chanx_out_14_[0]),
		.chanx_out_15_(cbx_1__2__0_chanx_out_15_[0]),
		.chanx_out_16_(cbx_1__2__0_chanx_out_16_[0]),
		.chanx_out_17_(cbx_1__2__0_chanx_out_17_[0]),
		.top_grid_pin_0_(cbx_1__2__0_top_grid_pin_0_[0]),
		.top_grid_pin_2_(cbx_1__2__0_top_grid_pin_2_[0]),
		.top_grid_pin_4_(cbx_1__2__0_top_grid_pin_4_[0]),
		.top_grid_pin_6_(cbx_1__2__0_top_grid_pin_6_[0]),
		.top_grid_pin_8_(cbx_1__2__0_top_grid_pin_8_[0]),
		.top_grid_pin_10_(cbx_1__2__0_top_grid_pin_10_[0]),
		.top_grid_pin_12_(cbx_1__2__0_top_grid_pin_12_[0]),
		.top_grid_pin_14_(cbx_1__2__0_top_grid_pin_14_[0]),
		.bottom_grid_pin_0_(cbx_1__2__0_bottom_grid_pin_0_[0]),
		.bottom_grid_pin_4_(cbx_1__2__0_bottom_grid_pin_4_[0]),
		.bottom_grid_pin_8_(cbx_1__2__0_bottom_grid_pin_8_[0]));

	cbx_1__2_ cbx_2__2_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chanx_in_0_(sb_1__2__0_chanx_right_out_0_[0]),
		.chanx_in_1_(sb_2__2__0_chanx_left_out_1_[0]),
		.chanx_in_2_(sb_1__2__0_chanx_right_out_2_[0]),
		.chanx_in_3_(sb_2__2__0_chanx_left_out_3_[0]),
		.chanx_in_4_(sb_1__2__0_chanx_right_out_4_[0]),
		.chanx_in_5_(sb_2__2__0_chanx_left_out_5_[0]),
		.chanx_in_6_(sb_1__2__0_chanx_right_out_6_[0]),
		.chanx_in_7_(sb_2__2__0_chanx_left_out_7_[0]),
		.chanx_in_8_(sb_1__2__0_chanx_right_out_8_[0]),
		.chanx_in_9_(sb_2__2__0_chanx_left_out_9_[0]),
		.chanx_in_10_(sb_1__2__0_chanx_right_out_10_[0]),
		.chanx_in_11_(sb_2__2__0_chanx_left_out_11_[0]),
		.chanx_in_12_(sb_1__2__0_chanx_right_out_12_[0]),
		.chanx_in_13_(sb_2__2__0_chanx_left_out_13_[0]),
		.chanx_in_14_(sb_1__2__0_chanx_right_out_14_[0]),
		.chanx_in_15_(sb_2__2__0_chanx_left_out_15_[0]),
		.chanx_in_16_(sb_1__2__0_chanx_right_out_16_[0]),
		.chanx_in_17_(sb_2__2__0_chanx_left_out_17_[0]),
		.enable(decoder6to33_0_data_out[5]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chanx_out_0_(cbx_1__2__1_chanx_out_0_[0]),
		.chanx_out_1_(cbx_1__2__1_chanx_out_1_[0]),
		.chanx_out_2_(cbx_1__2__1_chanx_out_2_[0]),
		.chanx_out_3_(cbx_1__2__1_chanx_out_3_[0]),
		.chanx_out_4_(cbx_1__2__1_chanx_out_4_[0]),
		.chanx_out_5_(cbx_1__2__1_chanx_out_5_[0]),
		.chanx_out_6_(cbx_1__2__1_chanx_out_6_[0]),
		.chanx_out_7_(cbx_1__2__1_chanx_out_7_[0]),
		.chanx_out_8_(cbx_1__2__1_chanx_out_8_[0]),
		.chanx_out_9_(cbx_1__2__1_chanx_out_9_[0]),
		.chanx_out_10_(cbx_1__2__1_chanx_out_10_[0]),
		.chanx_out_11_(cbx_1__2__1_chanx_out_11_[0]),
		.chanx_out_12_(cbx_1__2__1_chanx_out_12_[0]),
		.chanx_out_13_(cbx_1__2__1_chanx_out_13_[0]),
		.chanx_out_14_(cbx_1__2__1_chanx_out_14_[0]),
		.chanx_out_15_(cbx_1__2__1_chanx_out_15_[0]),
		.chanx_out_16_(cbx_1__2__1_chanx_out_16_[0]),
		.chanx_out_17_(cbx_1__2__1_chanx_out_17_[0]),
		.top_grid_pin_0_(cbx_1__2__1_top_grid_pin_0_[0]),
		.top_grid_pin_2_(cbx_1__2__1_top_grid_pin_2_[0]),
		.top_grid_pin_4_(cbx_1__2__1_top_grid_pin_4_[0]),
		.top_grid_pin_6_(cbx_1__2__1_top_grid_pin_6_[0]),
		.top_grid_pin_8_(cbx_1__2__1_top_grid_pin_8_[0]),
		.top_grid_pin_10_(cbx_1__2__1_top_grid_pin_10_[0]),
		.top_grid_pin_12_(cbx_1__2__1_top_grid_pin_12_[0]),
		.top_grid_pin_14_(cbx_1__2__1_top_grid_pin_14_[0]),
		.bottom_grid_pin_0_(cbx_1__2__1_bottom_grid_pin_0_[0]),
		.bottom_grid_pin_4_(cbx_1__2__1_bottom_grid_pin_4_[0]),
		.bottom_grid_pin_8_(cbx_1__2__1_bottom_grid_pin_8_[0]));

	cby_0__1_ cby_0__1_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_in_0_(sb_0__0__0_chany_top_out_0_[0]),
		.chany_in_1_(sb_0__1__0_chany_bottom_out_1_[0]),
		.chany_in_2_(sb_0__0__0_chany_top_out_2_[0]),
		.chany_in_3_(sb_0__1__0_chany_bottom_out_3_[0]),
		.chany_in_4_(sb_0__0__0_chany_top_out_4_[0]),
		.chany_in_5_(sb_0__1__0_chany_bottom_out_5_[0]),
		.chany_in_6_(sb_0__0__0_chany_top_out_6_[0]),
		.chany_in_7_(sb_0__1__0_chany_bottom_out_7_[0]),
		.chany_in_8_(sb_0__0__0_chany_top_out_8_[0]),
		.chany_in_9_(sb_0__1__0_chany_bottom_out_9_[0]),
		.chany_in_10_(sb_0__0__0_chany_top_out_10_[0]),
		.chany_in_11_(sb_0__1__0_chany_bottom_out_11_[0]),
		.chany_in_12_(sb_0__0__0_chany_top_out_12_[0]),
		.chany_in_13_(sb_0__1__0_chany_bottom_out_13_[0]),
		.chany_in_14_(sb_0__0__0_chany_top_out_14_[0]),
		.chany_in_15_(sb_0__1__0_chany_bottom_out_15_[0]),
		.chany_in_16_(sb_0__0__0_chany_top_out_16_[0]),
		.chany_in_17_(sb_0__1__0_chany_bottom_out_17_[0]),
		.enable(decoder6to33_0_data_out[15]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chany_out_0_(cby_0__1__0_chany_out_0_[0]),
		.chany_out_1_(cby_0__1__0_chany_out_1_[0]),
		.chany_out_2_(cby_0__1__0_chany_out_2_[0]),
		.chany_out_3_(cby_0__1__0_chany_out_3_[0]),
		.chany_out_4_(cby_0__1__0_chany_out_4_[0]),
		.chany_out_5_(cby_0__1__0_chany_out_5_[0]),
		.chany_out_6_(cby_0__1__0_chany_out_6_[0]),
		.chany_out_7_(cby_0__1__0_chany_out_7_[0]),
		.chany_out_8_(cby_0__1__0_chany_out_8_[0]),
		.chany_out_9_(cby_0__1__0_chany_out_9_[0]),
		.chany_out_10_(cby_0__1__0_chany_out_10_[0]),
		.chany_out_11_(cby_0__1__0_chany_out_11_[0]),
		.chany_out_12_(cby_0__1__0_chany_out_12_[0]),
		.chany_out_13_(cby_0__1__0_chany_out_13_[0]),
		.chany_out_14_(cby_0__1__0_chany_out_14_[0]),
		.chany_out_15_(cby_0__1__0_chany_out_15_[0]),
		.chany_out_16_(cby_0__1__0_chany_out_16_[0]),
		.chany_out_17_(cby_0__1__0_chany_out_17_[0]),
		.right_grid_pin_3_(cby_0__1__0_right_grid_pin_3_[0]),
		.right_grid_pin_7_(cby_0__1__0_right_grid_pin_7_[0]),
		.left_grid_pin_0_(cby_0__1__0_left_grid_pin_0_[0]),
		.left_grid_pin_2_(cby_0__1__0_left_grid_pin_2_[0]),
		.left_grid_pin_4_(cby_0__1__0_left_grid_pin_4_[0]),
		.left_grid_pin_6_(cby_0__1__0_left_grid_pin_6_[0]),
		.left_grid_pin_8_(cby_0__1__0_left_grid_pin_8_[0]),
		.left_grid_pin_10_(cby_0__1__0_left_grid_pin_10_[0]),
		.left_grid_pin_12_(cby_0__1__0_left_grid_pin_12_[0]),
		.left_grid_pin_14_(cby_0__1__0_left_grid_pin_14_[0]));

	cby_0__1_ cby_0__2_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_in_0_(sb_0__1__0_chany_top_out_0_[0]),
		.chany_in_1_(sb_0__2__0_chany_bottom_out_1_[0]),
		.chany_in_2_(sb_0__1__0_chany_top_out_2_[0]),
		.chany_in_3_(sb_0__2__0_chany_bottom_out_3_[0]),
		.chany_in_4_(sb_0__1__0_chany_top_out_4_[0]),
		.chany_in_5_(sb_0__2__0_chany_bottom_out_5_[0]),
		.chany_in_6_(sb_0__1__0_chany_top_out_6_[0]),
		.chany_in_7_(sb_0__2__0_chany_bottom_out_7_[0]),
		.chany_in_8_(sb_0__1__0_chany_top_out_8_[0]),
		.chany_in_9_(sb_0__2__0_chany_bottom_out_9_[0]),
		.chany_in_10_(sb_0__1__0_chany_top_out_10_[0]),
		.chany_in_11_(sb_0__2__0_chany_bottom_out_11_[0]),
		.chany_in_12_(sb_0__1__0_chany_top_out_12_[0]),
		.chany_in_13_(sb_0__2__0_chany_bottom_out_13_[0]),
		.chany_in_14_(sb_0__1__0_chany_top_out_14_[0]),
		.chany_in_15_(sb_0__2__0_chany_bottom_out_15_[0]),
		.chany_in_16_(sb_0__1__0_chany_top_out_16_[0]),
		.chany_in_17_(sb_0__2__0_chany_bottom_out_17_[0]),
		.enable(decoder6to33_0_data_out[12]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chany_out_0_(cby_0__1__1_chany_out_0_[0]),
		.chany_out_1_(cby_0__1__1_chany_out_1_[0]),
		.chany_out_2_(cby_0__1__1_chany_out_2_[0]),
		.chany_out_3_(cby_0__1__1_chany_out_3_[0]),
		.chany_out_4_(cby_0__1__1_chany_out_4_[0]),
		.chany_out_5_(cby_0__1__1_chany_out_5_[0]),
		.chany_out_6_(cby_0__1__1_chany_out_6_[0]),
		.chany_out_7_(cby_0__1__1_chany_out_7_[0]),
		.chany_out_8_(cby_0__1__1_chany_out_8_[0]),
		.chany_out_9_(cby_0__1__1_chany_out_9_[0]),
		.chany_out_10_(cby_0__1__1_chany_out_10_[0]),
		.chany_out_11_(cby_0__1__1_chany_out_11_[0]),
		.chany_out_12_(cby_0__1__1_chany_out_12_[0]),
		.chany_out_13_(cby_0__1__1_chany_out_13_[0]),
		.chany_out_14_(cby_0__1__1_chany_out_14_[0]),
		.chany_out_15_(cby_0__1__1_chany_out_15_[0]),
		.chany_out_16_(cby_0__1__1_chany_out_16_[0]),
		.chany_out_17_(cby_0__1__1_chany_out_17_[0]),
		.right_grid_pin_3_(cby_0__1__1_right_grid_pin_3_[0]),
		.right_grid_pin_7_(cby_0__1__1_right_grid_pin_7_[0]),
		.left_grid_pin_0_(cby_0__1__1_left_grid_pin_0_[0]),
		.left_grid_pin_2_(cby_0__1__1_left_grid_pin_2_[0]),
		.left_grid_pin_4_(cby_0__1__1_left_grid_pin_4_[0]),
		.left_grid_pin_6_(cby_0__1__1_left_grid_pin_6_[0]),
		.left_grid_pin_8_(cby_0__1__1_left_grid_pin_8_[0]),
		.left_grid_pin_10_(cby_0__1__1_left_grid_pin_10_[0]),
		.left_grid_pin_12_(cby_0__1__1_left_grid_pin_12_[0]),
		.left_grid_pin_14_(cby_0__1__1_left_grid_pin_14_[0]));

	cby_1__1_ cby_1__1_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_in_0_(sb_1__0__0_chany_top_out_0_[0]),
		.chany_in_1_(sb_1__1__0_chany_bottom_out_1_[0]),
		.chany_in_2_(sb_1__0__0_chany_top_out_2_[0]),
		.chany_in_3_(sb_1__1__0_chany_bottom_out_3_[0]),
		.chany_in_4_(sb_1__0__0_chany_top_out_4_[0]),
		.chany_in_5_(sb_1__1__0_chany_bottom_out_5_[0]),
		.chany_in_6_(sb_1__0__0_chany_top_out_6_[0]),
		.chany_in_7_(sb_1__1__0_chany_bottom_out_7_[0]),
		.chany_in_8_(sb_1__0__0_chany_top_out_8_[0]),
		.chany_in_9_(sb_1__1__0_chany_bottom_out_9_[0]),
		.chany_in_10_(sb_1__0__0_chany_top_out_10_[0]),
		.chany_in_11_(sb_1__1__0_chany_bottom_out_11_[0]),
		.chany_in_12_(sb_1__0__0_chany_top_out_12_[0]),
		.chany_in_13_(sb_1__1__0_chany_bottom_out_13_[0]),
		.chany_in_14_(sb_1__0__0_chany_top_out_14_[0]),
		.chany_in_15_(sb_1__1__0_chany_bottom_out_15_[0]),
		.chany_in_16_(sb_1__0__0_chany_top_out_16_[0]),
		.chany_in_17_(sb_1__1__0_chany_bottom_out_17_[0]),
		.enable(decoder6to33_0_data_out[19]),
		.address(address[0:5]),
		.data_in(data_in[0]),
		.chany_out_0_(cby_1__1__0_chany_out_0_[0]),
		.chany_out_1_(cby_1__1__0_chany_out_1_[0]),
		.chany_out_2_(cby_1__1__0_chany_out_2_[0]),
		.chany_out_3_(cby_1__1__0_chany_out_3_[0]),
		.chany_out_4_(cby_1__1__0_chany_out_4_[0]),
		.chany_out_5_(cby_1__1__0_chany_out_5_[0]),
		.chany_out_6_(cby_1__1__0_chany_out_6_[0]),
		.chany_out_7_(cby_1__1__0_chany_out_7_[0]),
		.chany_out_8_(cby_1__1__0_chany_out_8_[0]),
		.chany_out_9_(cby_1__1__0_chany_out_9_[0]),
		.chany_out_10_(cby_1__1__0_chany_out_10_[0]),
		.chany_out_11_(cby_1__1__0_chany_out_11_[0]),
		.chany_out_12_(cby_1__1__0_chany_out_12_[0]),
		.chany_out_13_(cby_1__1__0_chany_out_13_[0]),
		.chany_out_14_(cby_1__1__0_chany_out_14_[0]),
		.chany_out_15_(cby_1__1__0_chany_out_15_[0]),
		.chany_out_16_(cby_1__1__0_chany_out_16_[0]),
		.chany_out_17_(cby_1__1__0_chany_out_17_[0]),
		.right_grid_pin_3_(cby_1__1__0_right_grid_pin_3_[0]),
		.right_grid_pin_7_(cby_1__1__0_right_grid_pin_7_[0]),
		.left_grid_pin_1_(cby_1__1__0_left_grid_pin_1_[0]),
		.left_grid_pin_5_(cby_1__1__0_left_grid_pin_5_[0]),
		.left_grid_pin_9_(cby_1__1__0_left_grid_pin_9_[0]));

	cby_1__1_ cby_1__2_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_in_0_(sb_1__1__0_chany_top_out_0_[0]),
		.chany_in_1_(sb_1__2__0_chany_bottom_out_1_[0]),
		.chany_in_2_(sb_1__1__0_chany_top_out_2_[0]),
		.chany_in_3_(sb_1__2__0_chany_bottom_out_3_[0]),
		.chany_in_4_(sb_1__1__0_chany_top_out_4_[0]),
		.chany_in_5_(sb_1__2__0_chany_bottom_out_5_[0]),
		.chany_in_6_(sb_1__1__0_chany_top_out_6_[0]),
		.chany_in_7_(sb_1__2__0_chany_bottom_out_7_[0]),
		.chany_in_8_(sb_1__1__0_chany_top_out_8_[0]),
		.chany_in_9_(sb_1__2__0_chany_bottom_out_9_[0]),
		.chany_in_10_(sb_1__1__0_chany_top_out_10_[0]),
		.chany_in_11_(sb_1__2__0_chany_bottom_out_11_[0]),
		.chany_in_12_(sb_1__1__0_chany_top_out_12_[0]),
		.chany_in_13_(sb_1__2__0_chany_bottom_out_13_[0]),
		.chany_in_14_(sb_1__1__0_chany_top_out_14_[0]),
		.chany_in_15_(sb_1__2__0_chany_bottom_out_15_[0]),
		.chany_in_16_(sb_1__1__0_chany_top_out_16_[0]),
		.chany_in_17_(sb_1__2__0_chany_bottom_out_17_[0]),
		.enable(decoder6to33_0_data_out[31]),
		.address(address[0:5]),
		.data_in(data_in[0]),
		.chany_out_0_(cby_1__1__1_chany_out_0_[0]),
		.chany_out_1_(cby_1__1__1_chany_out_1_[0]),
		.chany_out_2_(cby_1__1__1_chany_out_2_[0]),
		.chany_out_3_(cby_1__1__1_chany_out_3_[0]),
		.chany_out_4_(cby_1__1__1_chany_out_4_[0]),
		.chany_out_5_(cby_1__1__1_chany_out_5_[0]),
		.chany_out_6_(cby_1__1__1_chany_out_6_[0]),
		.chany_out_7_(cby_1__1__1_chany_out_7_[0]),
		.chany_out_8_(cby_1__1__1_chany_out_8_[0]),
		.chany_out_9_(cby_1__1__1_chany_out_9_[0]),
		.chany_out_10_(cby_1__1__1_chany_out_10_[0]),
		.chany_out_11_(cby_1__1__1_chany_out_11_[0]),
		.chany_out_12_(cby_1__1__1_chany_out_12_[0]),
		.chany_out_13_(cby_1__1__1_chany_out_13_[0]),
		.chany_out_14_(cby_1__1__1_chany_out_14_[0]),
		.chany_out_15_(cby_1__1__1_chany_out_15_[0]),
		.chany_out_16_(cby_1__1__1_chany_out_16_[0]),
		.chany_out_17_(cby_1__1__1_chany_out_17_[0]),
		.right_grid_pin_3_(cby_1__1__1_right_grid_pin_3_[0]),
		.right_grid_pin_7_(cby_1__1__1_right_grid_pin_7_[0]),
		.left_grid_pin_1_(cby_1__1__1_left_grid_pin_1_[0]),
		.left_grid_pin_5_(cby_1__1__1_left_grid_pin_5_[0]),
		.left_grid_pin_9_(cby_1__1__1_left_grid_pin_9_[0]));

	cby_2__1_ cby_2__1_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_in_0_(sb_2__0__0_chany_top_out_0_[0]),
		.chany_in_1_(sb_2__1__0_chany_bottom_out_1_[0]),
		.chany_in_2_(sb_2__0__0_chany_top_out_2_[0]),
		.chany_in_3_(sb_2__1__0_chany_bottom_out_3_[0]),
		.chany_in_4_(sb_2__0__0_chany_top_out_4_[0]),
		.chany_in_5_(sb_2__1__0_chany_bottom_out_5_[0]),
		.chany_in_6_(sb_2__0__0_chany_top_out_6_[0]),
		.chany_in_7_(sb_2__1__0_chany_bottom_out_7_[0]),
		.chany_in_8_(sb_2__0__0_chany_top_out_8_[0]),
		.chany_in_9_(sb_2__1__0_chany_bottom_out_9_[0]),
		.chany_in_10_(sb_2__0__0_chany_top_out_10_[0]),
		.chany_in_11_(sb_2__1__0_chany_bottom_out_11_[0]),
		.chany_in_12_(sb_2__0__0_chany_top_out_12_[0]),
		.chany_in_13_(sb_2__1__0_chany_bottom_out_13_[0]),
		.chany_in_14_(sb_2__0__0_chany_top_out_14_[0]),
		.chany_in_15_(sb_2__1__0_chany_bottom_out_15_[0]),
		.chany_in_16_(sb_2__0__0_chany_top_out_16_[0]),
		.chany_in_17_(sb_2__1__0_chany_bottom_out_17_[0]),
		.enable(decoder6to33_0_data_out[23]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chany_out_0_(cby_2__1__0_chany_out_0_[0]),
		.chany_out_1_(cby_2__1__0_chany_out_1_[0]),
		.chany_out_2_(cby_2__1__0_chany_out_2_[0]),
		.chany_out_3_(cby_2__1__0_chany_out_3_[0]),
		.chany_out_4_(cby_2__1__0_chany_out_4_[0]),
		.chany_out_5_(cby_2__1__0_chany_out_5_[0]),
		.chany_out_6_(cby_2__1__0_chany_out_6_[0]),
		.chany_out_7_(cby_2__1__0_chany_out_7_[0]),
		.chany_out_8_(cby_2__1__0_chany_out_8_[0]),
		.chany_out_9_(cby_2__1__0_chany_out_9_[0]),
		.chany_out_10_(cby_2__1__0_chany_out_10_[0]),
		.chany_out_11_(cby_2__1__0_chany_out_11_[0]),
		.chany_out_12_(cby_2__1__0_chany_out_12_[0]),
		.chany_out_13_(cby_2__1__0_chany_out_13_[0]),
		.chany_out_14_(cby_2__1__0_chany_out_14_[0]),
		.chany_out_15_(cby_2__1__0_chany_out_15_[0]),
		.chany_out_16_(cby_2__1__0_chany_out_16_[0]),
		.chany_out_17_(cby_2__1__0_chany_out_17_[0]),
		.right_grid_pin_0_(cby_2__1__0_right_grid_pin_0_[0]),
		.right_grid_pin_2_(cby_2__1__0_right_grid_pin_2_[0]),
		.right_grid_pin_4_(cby_2__1__0_right_grid_pin_4_[0]),
		.right_grid_pin_6_(cby_2__1__0_right_grid_pin_6_[0]),
		.right_grid_pin_8_(cby_2__1__0_right_grid_pin_8_[0]),
		.right_grid_pin_10_(cby_2__1__0_right_grid_pin_10_[0]),
		.right_grid_pin_12_(cby_2__1__0_right_grid_pin_12_[0]),
		.right_grid_pin_14_(cby_2__1__0_right_grid_pin_14_[0]),
		.left_grid_pin_1_(cby_2__1__0_left_grid_pin_1_[0]),
		.left_grid_pin_5_(cby_2__1__0_left_grid_pin_5_[0]),
		.left_grid_pin_9_(cby_2__1__0_left_grid_pin_9_[0]));

	cby_2__1_ cby_2__2_ (
		.pReset(pReset[0]),
		.prog_clk(prog_clk[0]),
		.chany_in_0_(sb_2__1__0_chany_top_out_0_[0]),
		.chany_in_1_(sb_2__2__0_chany_bottom_out_1_[0]),
		.chany_in_2_(sb_2__1__0_chany_top_out_2_[0]),
		.chany_in_3_(sb_2__2__0_chany_bottom_out_3_[0]),
		.chany_in_4_(sb_2__1__0_chany_top_out_4_[0]),
		.chany_in_5_(sb_2__2__0_chany_bottom_out_5_[0]),
		.chany_in_6_(sb_2__1__0_chany_top_out_6_[0]),
		.chany_in_7_(sb_2__2__0_chany_bottom_out_7_[0]),
		.chany_in_8_(sb_2__1__0_chany_top_out_8_[0]),
		.chany_in_9_(sb_2__2__0_chany_bottom_out_9_[0]),
		.chany_in_10_(sb_2__1__0_chany_top_out_10_[0]),
		.chany_in_11_(sb_2__2__0_chany_bottom_out_11_[0]),
		.chany_in_12_(sb_2__1__0_chany_top_out_12_[0]),
		.chany_in_13_(sb_2__2__0_chany_bottom_out_13_[0]),
		.chany_in_14_(sb_2__1__0_chany_top_out_14_[0]),
		.chany_in_15_(sb_2__2__0_chany_bottom_out_15_[0]),
		.chany_in_16_(sb_2__1__0_chany_top_out_16_[0]),
		.chany_in_17_(sb_2__2__0_chany_bottom_out_17_[0]),
		.enable(decoder6to33_0_data_out[27]),
		.address(address[0:6]),
		.data_in(data_in[0]),
		.chany_out_0_(cby_2__1__1_chany_out_0_[0]),
		.chany_out_1_(cby_2__1__1_chany_out_1_[0]),
		.chany_out_2_(cby_2__1__1_chany_out_2_[0]),
		.chany_out_3_(cby_2__1__1_chany_out_3_[0]),
		.chany_out_4_(cby_2__1__1_chany_out_4_[0]),
		.chany_out_5_(cby_2__1__1_chany_out_5_[0]),
		.chany_out_6_(cby_2__1__1_chany_out_6_[0]),
		.chany_out_7_(cby_2__1__1_chany_out_7_[0]),
		.chany_out_8_(cby_2__1__1_chany_out_8_[0]),
		.chany_out_9_(cby_2__1__1_chany_out_9_[0]),
		.chany_out_10_(cby_2__1__1_chany_out_10_[0]),
		.chany_out_11_(cby_2__1__1_chany_out_11_[0]),
		.chany_out_12_(cby_2__1__1_chany_out_12_[0]),
		.chany_out_13_(cby_2__1__1_chany_out_13_[0]),
		.chany_out_14_(cby_2__1__1_chany_out_14_[0]),
		.chany_out_15_(cby_2__1__1_chany_out_15_[0]),
		.chany_out_16_(cby_2__1__1_chany_out_16_[0]),
		.chany_out_17_(cby_2__1__1_chany_out_17_[0]),
		.right_grid_pin_0_(cby_2__1__1_right_grid_pin_0_[0]),
		.right_grid_pin_2_(cby_2__1__1_right_grid_pin_2_[0]),
		.right_grid_pin_4_(cby_2__1__1_right_grid_pin_4_[0]),
		.right_grid_pin_6_(cby_2__1__1_right_grid_pin_6_[0]),
		.right_grid_pin_8_(cby_2__1__1_right_grid_pin_8_[0]),
		.right_grid_pin_10_(cby_2__1__1_right_grid_pin_10_[0]),
		.right_grid_pin_12_(cby_2__1__1_right_grid_pin_12_[0]),
		.right_grid_pin_14_(cby_2__1__1_right_grid_pin_14_[0]),
		.left_grid_pin_1_(cby_2__1__1_left_grid_pin_1_[0]),
		.left_grid_pin_5_(cby_2__1__1_left_grid_pin_5_[0]),
		.left_grid_pin_9_(cby_2__1__1_left_grid_pin_9_[0]));

	decoder6to33 decoder6to33_0_ (
		.enable(enable[0]),
		.address(address[10:15]),
		.data_out(decoder6to33_0_data_out[0:32]));

endmodule
// ----- END Verilog module for fpga_top -----



