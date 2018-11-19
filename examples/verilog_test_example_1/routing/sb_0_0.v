//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Switch Block  [0][0] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Verilog Module of Switch Box[0][0] -----
module sb_0__0_ ( 

//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
//----- Inputs/outputs of top side -----
  output chany_0__1__out_0_,
  input chany_0__1__in_1_,
  output chany_0__1__out_2_,
  input chany_0__1__in_3_,
  output chany_0__1__out_4_,
  input chany_0__1__in_5_,
  output chany_0__1__out_6_,
  input chany_0__1__in_7_,
  output chany_0__1__out_8_,
  input chany_0__1__in_9_,
  output chany_0__1__out_10_,
  input chany_0__1__in_11_,
  output chany_0__1__out_12_,
  input chany_0__1__in_13_,
  output chany_0__1__out_14_,
  input chany_0__1__in_15_,
  output chany_0__1__out_16_,
  input chany_0__1__in_17_,
  output chany_0__1__out_18_,
  input chany_0__1__in_19_,
  output chany_0__1__out_20_,
  input chany_0__1__in_21_,
  output chany_0__1__out_22_,
  input chany_0__1__in_23_,
  output chany_0__1__out_24_,
  input chany_0__1__in_25_,
  output chany_0__1__out_26_,
  input chany_0__1__in_27_,
  output chany_0__1__out_28_,
  input chany_0__1__in_29_,
input  grid_0__1__pin_0__1__1_,
input  grid_0__1__pin_0__1__3_,
input  grid_0__1__pin_0__1__5_,
input  grid_0__1__pin_0__1__7_,
input  grid_0__1__pin_0__1__9_,
input  grid_0__1__pin_0__1__11_,
input  grid_0__1__pin_0__1__13_,
input  grid_0__1__pin_0__1__15_,
//----- Inputs/outputs of right side -----
  output chanx_1__0__out_0_,
  input chanx_1__0__in_1_,
  output chanx_1__0__out_2_,
  input chanx_1__0__in_3_,
  output chanx_1__0__out_4_,
  input chanx_1__0__in_5_,
  output chanx_1__0__out_6_,
  input chanx_1__0__in_7_,
  output chanx_1__0__out_8_,
  input chanx_1__0__in_9_,
  output chanx_1__0__out_10_,
  input chanx_1__0__in_11_,
  output chanx_1__0__out_12_,
  input chanx_1__0__in_13_,
  output chanx_1__0__out_14_,
  input chanx_1__0__in_15_,
  output chanx_1__0__out_16_,
  input chanx_1__0__in_17_,
  output chanx_1__0__out_18_,
  input chanx_1__0__in_19_,
  output chanx_1__0__out_20_,
  input chanx_1__0__in_21_,
  output chanx_1__0__out_22_,
  input chanx_1__0__in_23_,
  output chanx_1__0__out_24_,
  input chanx_1__0__in_25_,
  output chanx_1__0__out_26_,
  input chanx_1__0__in_27_,
  output chanx_1__0__out_28_,
  input chanx_1__0__in_29_,
input  grid_1__0__pin_0__0__1_,
input  grid_1__0__pin_0__0__3_,
input  grid_1__0__pin_0__0__5_,
input  grid_1__0__pin_0__0__7_,
input  grid_1__0__pin_0__0__9_,
input  grid_1__0__pin_0__0__11_,
input  grid_1__0__pin_0__0__13_,
input  grid_1__0__pin_0__0__15_,
//----- Inputs/outputs of bottom side -----
//----- Inputs/outputs of left side -----
input [0:33] sram_blwl_bl ,
input [0:33] sram_blwl_wl ,
input [0:33] sram_blwl_blb ); 
//----- top side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_0_inbus;
assign mux_1level_tapbuf_size3_0_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size3_0_inbus[1] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size3_0_inbus[2] = chanx_1__0__in_3_ ;
wire [0:2] mux_1level_tapbuf_size3_0_configbus0;
wire [0:2] mux_1level_tapbuf_size3_0_configbus1;
wire [0:2] mux_1level_tapbuf_size3_0_sram_blwl_out ;
wire [0:2] mux_1level_tapbuf_size3_0_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_0_configbus0[0:2] = sram_blwl_bl[0:2] ;
assign mux_1level_tapbuf_size3_0_configbus1[0:2] = sram_blwl_wl[0:2] ;
wire [0:2] mux_1level_tapbuf_size3_0_configbus0_b;
assign mux_1level_tapbuf_size3_0_configbus0_b[0:2] = sram_blwl_blb[0:2] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_0_ (mux_1level_tapbuf_size3_0_inbus, chany_0__1__out_0_ , mux_1level_tapbuf_size3_0_sram_blwl_out[0:2] ,
mux_1level_tapbuf_size3_0_sram_blwl_outb[0:2] );
//----- SRAM bits for MUX[0], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_0_ (mux_1level_tapbuf_size3_0_sram_blwl_out[0:0] ,mux_1level_tapbuf_size3_0_sram_blwl_out[0:0] ,mux_1level_tapbuf_size3_0_sram_blwl_outb[0:0] ,mux_1level_tapbuf_size3_0_configbus0[0:0], mux_1level_tapbuf_size3_0_configbus1[0:0] , mux_1level_tapbuf_size3_0_configbus0_b[0:0] );
sram6T_blwl sram_blwl_1_ (mux_1level_tapbuf_size3_0_sram_blwl_out[1:1] ,mux_1level_tapbuf_size3_0_sram_blwl_out[1:1] ,mux_1level_tapbuf_size3_0_sram_blwl_outb[1:1] ,mux_1level_tapbuf_size3_0_configbus0[1:1], mux_1level_tapbuf_size3_0_configbus1[1:1] , mux_1level_tapbuf_size3_0_configbus0_b[1:1] );
sram6T_blwl sram_blwl_2_ (mux_1level_tapbuf_size3_0_sram_blwl_out[2:2] ,mux_1level_tapbuf_size3_0_sram_blwl_out[2:2] ,mux_1level_tapbuf_size3_0_sram_blwl_outb[2:2] ,mux_1level_tapbuf_size3_0_configbus0[2:2], mux_1level_tapbuf_size3_0_configbus1[2:2] , mux_1level_tapbuf_size3_0_configbus0_b[2:2] );
wire [0:1] mux_1level_tapbuf_size2_1_inbus;
assign mux_1level_tapbuf_size2_1_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_1_inbus[1] = chanx_1__0__in_5_ ;
wire [3:3] mux_1level_tapbuf_size2_1_configbus0;
wire [3:3] mux_1level_tapbuf_size2_1_configbus1;
wire [3:3] mux_1level_tapbuf_size2_1_sram_blwl_out ;
wire [3:3] mux_1level_tapbuf_size2_1_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_1_configbus0[3:3] = sram_blwl_bl[3:3] ;
assign mux_1level_tapbuf_size2_1_configbus1[3:3] = sram_blwl_wl[3:3] ;
wire [3:3] mux_1level_tapbuf_size2_1_configbus0_b;
assign mux_1level_tapbuf_size2_1_configbus0_b[3:3] = sram_blwl_blb[3:3] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_1_ (mux_1level_tapbuf_size2_1_inbus, chany_0__1__out_2_ , mux_1level_tapbuf_size2_1_sram_blwl_out[3:3] ,
mux_1level_tapbuf_size2_1_sram_blwl_outb[3:3] );
//----- SRAM bits for MUX[1], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_3_ (mux_1level_tapbuf_size2_1_sram_blwl_out[3:3] ,mux_1level_tapbuf_size2_1_sram_blwl_out[3:3] ,mux_1level_tapbuf_size2_1_sram_blwl_outb[3:3] ,mux_1level_tapbuf_size2_1_configbus0[3:3], mux_1level_tapbuf_size2_1_configbus1[3:3] , mux_1level_tapbuf_size2_1_configbus0_b[3:3] );
wire [0:1] mux_1level_tapbuf_size2_2_inbus;
assign mux_1level_tapbuf_size2_2_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_2_inbus[1] = chanx_1__0__in_7_ ;
wire [4:4] mux_1level_tapbuf_size2_2_configbus0;
wire [4:4] mux_1level_tapbuf_size2_2_configbus1;
wire [4:4] mux_1level_tapbuf_size2_2_sram_blwl_out ;
wire [4:4] mux_1level_tapbuf_size2_2_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_2_configbus0[4:4] = sram_blwl_bl[4:4] ;
assign mux_1level_tapbuf_size2_2_configbus1[4:4] = sram_blwl_wl[4:4] ;
wire [4:4] mux_1level_tapbuf_size2_2_configbus0_b;
assign mux_1level_tapbuf_size2_2_configbus0_b[4:4] = sram_blwl_blb[4:4] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_2_ (mux_1level_tapbuf_size2_2_inbus, chany_0__1__out_4_ , mux_1level_tapbuf_size2_2_sram_blwl_out[4:4] ,
mux_1level_tapbuf_size2_2_sram_blwl_outb[4:4] );
//----- SRAM bits for MUX[2], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_4_ (mux_1level_tapbuf_size2_2_sram_blwl_out[4:4] ,mux_1level_tapbuf_size2_2_sram_blwl_out[4:4] ,mux_1level_tapbuf_size2_2_sram_blwl_outb[4:4] ,mux_1level_tapbuf_size2_2_configbus0[4:4], mux_1level_tapbuf_size2_2_configbus1[4:4] , mux_1level_tapbuf_size2_2_configbus0_b[4:4] );
wire [0:1] mux_1level_tapbuf_size2_3_inbus;
assign mux_1level_tapbuf_size2_3_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_3_inbus[1] = chanx_1__0__in_9_ ;
wire [5:5] mux_1level_tapbuf_size2_3_configbus0;
wire [5:5] mux_1level_tapbuf_size2_3_configbus1;
wire [5:5] mux_1level_tapbuf_size2_3_sram_blwl_out ;
wire [5:5] mux_1level_tapbuf_size2_3_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_3_configbus0[5:5] = sram_blwl_bl[5:5] ;
assign mux_1level_tapbuf_size2_3_configbus1[5:5] = sram_blwl_wl[5:5] ;
wire [5:5] mux_1level_tapbuf_size2_3_configbus0_b;
assign mux_1level_tapbuf_size2_3_configbus0_b[5:5] = sram_blwl_blb[5:5] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_3_ (mux_1level_tapbuf_size2_3_inbus, chany_0__1__out_6_ , mux_1level_tapbuf_size2_3_sram_blwl_out[5:5] ,
mux_1level_tapbuf_size2_3_sram_blwl_outb[5:5] );
//----- SRAM bits for MUX[3], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_5_ (mux_1level_tapbuf_size2_3_sram_blwl_out[5:5] ,mux_1level_tapbuf_size2_3_sram_blwl_out[5:5] ,mux_1level_tapbuf_size2_3_sram_blwl_outb[5:5] ,mux_1level_tapbuf_size2_3_configbus0[5:5], mux_1level_tapbuf_size2_3_configbus1[5:5] , mux_1level_tapbuf_size2_3_configbus0_b[5:5] );
wire [0:1] mux_1level_tapbuf_size2_4_inbus;
assign mux_1level_tapbuf_size2_4_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_4_inbus[1] = chanx_1__0__in_11_ ;
wire [6:6] mux_1level_tapbuf_size2_4_configbus0;
wire [6:6] mux_1level_tapbuf_size2_4_configbus1;
wire [6:6] mux_1level_tapbuf_size2_4_sram_blwl_out ;
wire [6:6] mux_1level_tapbuf_size2_4_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_4_configbus0[6:6] = sram_blwl_bl[6:6] ;
assign mux_1level_tapbuf_size2_4_configbus1[6:6] = sram_blwl_wl[6:6] ;
wire [6:6] mux_1level_tapbuf_size2_4_configbus0_b;
assign mux_1level_tapbuf_size2_4_configbus0_b[6:6] = sram_blwl_blb[6:6] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_4_ (mux_1level_tapbuf_size2_4_inbus, chany_0__1__out_8_ , mux_1level_tapbuf_size2_4_sram_blwl_out[6:6] ,
mux_1level_tapbuf_size2_4_sram_blwl_outb[6:6] );
//----- SRAM bits for MUX[4], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_6_ (mux_1level_tapbuf_size2_4_sram_blwl_out[6:6] ,mux_1level_tapbuf_size2_4_sram_blwl_out[6:6] ,mux_1level_tapbuf_size2_4_sram_blwl_outb[6:6] ,mux_1level_tapbuf_size2_4_configbus0[6:6], mux_1level_tapbuf_size2_4_configbus1[6:6] , mux_1level_tapbuf_size2_4_configbus0_b[6:6] );
wire [0:1] mux_1level_tapbuf_size2_5_inbus;
assign mux_1level_tapbuf_size2_5_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_5_inbus[1] = chanx_1__0__in_13_ ;
wire [7:7] mux_1level_tapbuf_size2_5_configbus0;
wire [7:7] mux_1level_tapbuf_size2_5_configbus1;
wire [7:7] mux_1level_tapbuf_size2_5_sram_blwl_out ;
wire [7:7] mux_1level_tapbuf_size2_5_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_5_configbus0[7:7] = sram_blwl_bl[7:7] ;
assign mux_1level_tapbuf_size2_5_configbus1[7:7] = sram_blwl_wl[7:7] ;
wire [7:7] mux_1level_tapbuf_size2_5_configbus0_b;
assign mux_1level_tapbuf_size2_5_configbus0_b[7:7] = sram_blwl_blb[7:7] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_5_ (mux_1level_tapbuf_size2_5_inbus, chany_0__1__out_10_ , mux_1level_tapbuf_size2_5_sram_blwl_out[7:7] ,
mux_1level_tapbuf_size2_5_sram_blwl_outb[7:7] );
//----- SRAM bits for MUX[5], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_7_ (mux_1level_tapbuf_size2_5_sram_blwl_out[7:7] ,mux_1level_tapbuf_size2_5_sram_blwl_out[7:7] ,mux_1level_tapbuf_size2_5_sram_blwl_outb[7:7] ,mux_1level_tapbuf_size2_5_configbus0[7:7], mux_1level_tapbuf_size2_5_configbus1[7:7] , mux_1level_tapbuf_size2_5_configbus0_b[7:7] );
wire [0:1] mux_1level_tapbuf_size2_6_inbus;
assign mux_1level_tapbuf_size2_6_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_6_inbus[1] = chanx_1__0__in_15_ ;
wire [8:8] mux_1level_tapbuf_size2_6_configbus0;
wire [8:8] mux_1level_tapbuf_size2_6_configbus1;
wire [8:8] mux_1level_tapbuf_size2_6_sram_blwl_out ;
wire [8:8] mux_1level_tapbuf_size2_6_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_6_configbus0[8:8] = sram_blwl_bl[8:8] ;
assign mux_1level_tapbuf_size2_6_configbus1[8:8] = sram_blwl_wl[8:8] ;
wire [8:8] mux_1level_tapbuf_size2_6_configbus0_b;
assign mux_1level_tapbuf_size2_6_configbus0_b[8:8] = sram_blwl_blb[8:8] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_6_ (mux_1level_tapbuf_size2_6_inbus, chany_0__1__out_12_ , mux_1level_tapbuf_size2_6_sram_blwl_out[8:8] ,
mux_1level_tapbuf_size2_6_sram_blwl_outb[8:8] );
//----- SRAM bits for MUX[6], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_8_ (mux_1level_tapbuf_size2_6_sram_blwl_out[8:8] ,mux_1level_tapbuf_size2_6_sram_blwl_out[8:8] ,mux_1level_tapbuf_size2_6_sram_blwl_outb[8:8] ,mux_1level_tapbuf_size2_6_configbus0[8:8], mux_1level_tapbuf_size2_6_configbus1[8:8] , mux_1level_tapbuf_size2_6_configbus0_b[8:8] );
wire [0:1] mux_1level_tapbuf_size2_7_inbus;
assign mux_1level_tapbuf_size2_7_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_7_inbus[1] = chanx_1__0__in_17_ ;
wire [9:9] mux_1level_tapbuf_size2_7_configbus0;
wire [9:9] mux_1level_tapbuf_size2_7_configbus1;
wire [9:9] mux_1level_tapbuf_size2_7_sram_blwl_out ;
wire [9:9] mux_1level_tapbuf_size2_7_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_7_configbus0[9:9] = sram_blwl_bl[9:9] ;
assign mux_1level_tapbuf_size2_7_configbus1[9:9] = sram_blwl_wl[9:9] ;
wire [9:9] mux_1level_tapbuf_size2_7_configbus0_b;
assign mux_1level_tapbuf_size2_7_configbus0_b[9:9] = sram_blwl_blb[9:9] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_7_ (mux_1level_tapbuf_size2_7_inbus, chany_0__1__out_14_ , mux_1level_tapbuf_size2_7_sram_blwl_out[9:9] ,
mux_1level_tapbuf_size2_7_sram_blwl_outb[9:9] );
//----- SRAM bits for MUX[7], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_9_ (mux_1level_tapbuf_size2_7_sram_blwl_out[9:9] ,mux_1level_tapbuf_size2_7_sram_blwl_out[9:9] ,mux_1level_tapbuf_size2_7_sram_blwl_outb[9:9] ,mux_1level_tapbuf_size2_7_configbus0[9:9], mux_1level_tapbuf_size2_7_configbus1[9:9] , mux_1level_tapbuf_size2_7_configbus0_b[9:9] );
wire [0:1] mux_1level_tapbuf_size2_8_inbus;
assign mux_1level_tapbuf_size2_8_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_8_inbus[1] = chanx_1__0__in_19_ ;
wire [10:10] mux_1level_tapbuf_size2_8_configbus0;
wire [10:10] mux_1level_tapbuf_size2_8_configbus1;
wire [10:10] mux_1level_tapbuf_size2_8_sram_blwl_out ;
wire [10:10] mux_1level_tapbuf_size2_8_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_8_configbus0[10:10] = sram_blwl_bl[10:10] ;
assign mux_1level_tapbuf_size2_8_configbus1[10:10] = sram_blwl_wl[10:10] ;
wire [10:10] mux_1level_tapbuf_size2_8_configbus0_b;
assign mux_1level_tapbuf_size2_8_configbus0_b[10:10] = sram_blwl_blb[10:10] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_8_ (mux_1level_tapbuf_size2_8_inbus, chany_0__1__out_16_ , mux_1level_tapbuf_size2_8_sram_blwl_out[10:10] ,
mux_1level_tapbuf_size2_8_sram_blwl_outb[10:10] );
//----- SRAM bits for MUX[8], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_10_ (mux_1level_tapbuf_size2_8_sram_blwl_out[10:10] ,mux_1level_tapbuf_size2_8_sram_blwl_out[10:10] ,mux_1level_tapbuf_size2_8_sram_blwl_outb[10:10] ,mux_1level_tapbuf_size2_8_configbus0[10:10], mux_1level_tapbuf_size2_8_configbus1[10:10] , mux_1level_tapbuf_size2_8_configbus0_b[10:10] );
wire [0:1] mux_1level_tapbuf_size2_9_inbus;
assign mux_1level_tapbuf_size2_9_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_9_inbus[1] = chanx_1__0__in_21_ ;
wire [11:11] mux_1level_tapbuf_size2_9_configbus0;
wire [11:11] mux_1level_tapbuf_size2_9_configbus1;
wire [11:11] mux_1level_tapbuf_size2_9_sram_blwl_out ;
wire [11:11] mux_1level_tapbuf_size2_9_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_9_configbus0[11:11] = sram_blwl_bl[11:11] ;
assign mux_1level_tapbuf_size2_9_configbus1[11:11] = sram_blwl_wl[11:11] ;
wire [11:11] mux_1level_tapbuf_size2_9_configbus0_b;
assign mux_1level_tapbuf_size2_9_configbus0_b[11:11] = sram_blwl_blb[11:11] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_9_ (mux_1level_tapbuf_size2_9_inbus, chany_0__1__out_18_ , mux_1level_tapbuf_size2_9_sram_blwl_out[11:11] ,
mux_1level_tapbuf_size2_9_sram_blwl_outb[11:11] );
//----- SRAM bits for MUX[9], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_11_ (mux_1level_tapbuf_size2_9_sram_blwl_out[11:11] ,mux_1level_tapbuf_size2_9_sram_blwl_out[11:11] ,mux_1level_tapbuf_size2_9_sram_blwl_outb[11:11] ,mux_1level_tapbuf_size2_9_configbus0[11:11], mux_1level_tapbuf_size2_9_configbus1[11:11] , mux_1level_tapbuf_size2_9_configbus0_b[11:11] );
wire [0:1] mux_1level_tapbuf_size2_10_inbus;
assign mux_1level_tapbuf_size2_10_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_10_inbus[1] = chanx_1__0__in_23_ ;
wire [12:12] mux_1level_tapbuf_size2_10_configbus0;
wire [12:12] mux_1level_tapbuf_size2_10_configbus1;
wire [12:12] mux_1level_tapbuf_size2_10_sram_blwl_out ;
wire [12:12] mux_1level_tapbuf_size2_10_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_10_configbus0[12:12] = sram_blwl_bl[12:12] ;
assign mux_1level_tapbuf_size2_10_configbus1[12:12] = sram_blwl_wl[12:12] ;
wire [12:12] mux_1level_tapbuf_size2_10_configbus0_b;
assign mux_1level_tapbuf_size2_10_configbus0_b[12:12] = sram_blwl_blb[12:12] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_10_ (mux_1level_tapbuf_size2_10_inbus, chany_0__1__out_20_ , mux_1level_tapbuf_size2_10_sram_blwl_out[12:12] ,
mux_1level_tapbuf_size2_10_sram_blwl_outb[12:12] );
//----- SRAM bits for MUX[10], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_12_ (mux_1level_tapbuf_size2_10_sram_blwl_out[12:12] ,mux_1level_tapbuf_size2_10_sram_blwl_out[12:12] ,mux_1level_tapbuf_size2_10_sram_blwl_outb[12:12] ,mux_1level_tapbuf_size2_10_configbus0[12:12], mux_1level_tapbuf_size2_10_configbus1[12:12] , mux_1level_tapbuf_size2_10_configbus0_b[12:12] );
wire [0:1] mux_1level_tapbuf_size2_11_inbus;
assign mux_1level_tapbuf_size2_11_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_11_inbus[1] = chanx_1__0__in_25_ ;
wire [13:13] mux_1level_tapbuf_size2_11_configbus0;
wire [13:13] mux_1level_tapbuf_size2_11_configbus1;
wire [13:13] mux_1level_tapbuf_size2_11_sram_blwl_out ;
wire [13:13] mux_1level_tapbuf_size2_11_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_11_configbus0[13:13] = sram_blwl_bl[13:13] ;
assign mux_1level_tapbuf_size2_11_configbus1[13:13] = sram_blwl_wl[13:13] ;
wire [13:13] mux_1level_tapbuf_size2_11_configbus0_b;
assign mux_1level_tapbuf_size2_11_configbus0_b[13:13] = sram_blwl_blb[13:13] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_11_ (mux_1level_tapbuf_size2_11_inbus, chany_0__1__out_22_ , mux_1level_tapbuf_size2_11_sram_blwl_out[13:13] ,
mux_1level_tapbuf_size2_11_sram_blwl_outb[13:13] );
//----- SRAM bits for MUX[11], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_13_ (mux_1level_tapbuf_size2_11_sram_blwl_out[13:13] ,mux_1level_tapbuf_size2_11_sram_blwl_out[13:13] ,mux_1level_tapbuf_size2_11_sram_blwl_outb[13:13] ,mux_1level_tapbuf_size2_11_configbus0[13:13], mux_1level_tapbuf_size2_11_configbus1[13:13] , mux_1level_tapbuf_size2_11_configbus0_b[13:13] );
wire [0:1] mux_1level_tapbuf_size2_12_inbus;
assign mux_1level_tapbuf_size2_12_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_12_inbus[1] = chanx_1__0__in_27_ ;
wire [14:14] mux_1level_tapbuf_size2_12_configbus0;
wire [14:14] mux_1level_tapbuf_size2_12_configbus1;
wire [14:14] mux_1level_tapbuf_size2_12_sram_blwl_out ;
wire [14:14] mux_1level_tapbuf_size2_12_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_12_configbus0[14:14] = sram_blwl_bl[14:14] ;
assign mux_1level_tapbuf_size2_12_configbus1[14:14] = sram_blwl_wl[14:14] ;
wire [14:14] mux_1level_tapbuf_size2_12_configbus0_b;
assign mux_1level_tapbuf_size2_12_configbus0_b[14:14] = sram_blwl_blb[14:14] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_12_ (mux_1level_tapbuf_size2_12_inbus, chany_0__1__out_24_ , mux_1level_tapbuf_size2_12_sram_blwl_out[14:14] ,
mux_1level_tapbuf_size2_12_sram_blwl_outb[14:14] );
//----- SRAM bits for MUX[12], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_14_ (mux_1level_tapbuf_size2_12_sram_blwl_out[14:14] ,mux_1level_tapbuf_size2_12_sram_blwl_out[14:14] ,mux_1level_tapbuf_size2_12_sram_blwl_outb[14:14] ,mux_1level_tapbuf_size2_12_configbus0[14:14], mux_1level_tapbuf_size2_12_configbus1[14:14] , mux_1level_tapbuf_size2_12_configbus0_b[14:14] );
wire [0:1] mux_1level_tapbuf_size2_13_inbus;
assign mux_1level_tapbuf_size2_13_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_13_inbus[1] = chanx_1__0__in_29_ ;
wire [15:15] mux_1level_tapbuf_size2_13_configbus0;
wire [15:15] mux_1level_tapbuf_size2_13_configbus1;
wire [15:15] mux_1level_tapbuf_size2_13_sram_blwl_out ;
wire [15:15] mux_1level_tapbuf_size2_13_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_13_configbus0[15:15] = sram_blwl_bl[15:15] ;
assign mux_1level_tapbuf_size2_13_configbus1[15:15] = sram_blwl_wl[15:15] ;
wire [15:15] mux_1level_tapbuf_size2_13_configbus0_b;
assign mux_1level_tapbuf_size2_13_configbus0_b[15:15] = sram_blwl_blb[15:15] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_13_ (mux_1level_tapbuf_size2_13_inbus, chany_0__1__out_26_ , mux_1level_tapbuf_size2_13_sram_blwl_out[15:15] ,
mux_1level_tapbuf_size2_13_sram_blwl_outb[15:15] );
//----- SRAM bits for MUX[13], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_15_ (mux_1level_tapbuf_size2_13_sram_blwl_out[15:15] ,mux_1level_tapbuf_size2_13_sram_blwl_out[15:15] ,mux_1level_tapbuf_size2_13_sram_blwl_outb[15:15] ,mux_1level_tapbuf_size2_13_configbus0[15:15], mux_1level_tapbuf_size2_13_configbus1[15:15] , mux_1level_tapbuf_size2_13_configbus0_b[15:15] );
wire [0:1] mux_1level_tapbuf_size2_14_inbus;
assign mux_1level_tapbuf_size2_14_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_14_inbus[1] = chanx_1__0__in_1_ ;
wire [16:16] mux_1level_tapbuf_size2_14_configbus0;
wire [16:16] mux_1level_tapbuf_size2_14_configbus1;
wire [16:16] mux_1level_tapbuf_size2_14_sram_blwl_out ;
wire [16:16] mux_1level_tapbuf_size2_14_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_14_configbus0[16:16] = sram_blwl_bl[16:16] ;
assign mux_1level_tapbuf_size2_14_configbus1[16:16] = sram_blwl_wl[16:16] ;
wire [16:16] mux_1level_tapbuf_size2_14_configbus0_b;
assign mux_1level_tapbuf_size2_14_configbus0_b[16:16] = sram_blwl_blb[16:16] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_14_ (mux_1level_tapbuf_size2_14_inbus, chany_0__1__out_28_ , mux_1level_tapbuf_size2_14_sram_blwl_out[16:16] ,
mux_1level_tapbuf_size2_14_sram_blwl_outb[16:16] );
//----- SRAM bits for MUX[14], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_16_ (mux_1level_tapbuf_size2_14_sram_blwl_out[16:16] ,mux_1level_tapbuf_size2_14_sram_blwl_out[16:16] ,mux_1level_tapbuf_size2_14_sram_blwl_outb[16:16] ,mux_1level_tapbuf_size2_14_configbus0[16:16], mux_1level_tapbuf_size2_14_configbus1[16:16] , mux_1level_tapbuf_size2_14_configbus0_b[16:16] );
//----- right side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_15_inbus;
assign mux_1level_tapbuf_size3_15_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size3_15_inbus[1] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size3_15_inbus[2] = chany_0__1__in_29_ ;
wire [17:19] mux_1level_tapbuf_size3_15_configbus0;
wire [17:19] mux_1level_tapbuf_size3_15_configbus1;
wire [17:19] mux_1level_tapbuf_size3_15_sram_blwl_out ;
wire [17:19] mux_1level_tapbuf_size3_15_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_15_configbus0[17:19] = sram_blwl_bl[17:19] ;
assign mux_1level_tapbuf_size3_15_configbus1[17:19] = sram_blwl_wl[17:19] ;
wire [17:19] mux_1level_tapbuf_size3_15_configbus0_b;
assign mux_1level_tapbuf_size3_15_configbus0_b[17:19] = sram_blwl_blb[17:19] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_15_ (mux_1level_tapbuf_size3_15_inbus, chanx_1__0__out_0_ , mux_1level_tapbuf_size3_15_sram_blwl_out[17:19] ,
mux_1level_tapbuf_size3_15_sram_blwl_outb[17:19] );
//----- SRAM bits for MUX[15], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_17_ (mux_1level_tapbuf_size3_15_sram_blwl_out[17:17] ,mux_1level_tapbuf_size3_15_sram_blwl_out[17:17] ,mux_1level_tapbuf_size3_15_sram_blwl_outb[17:17] ,mux_1level_tapbuf_size3_15_configbus0[17:17], mux_1level_tapbuf_size3_15_configbus1[17:17] , mux_1level_tapbuf_size3_15_configbus0_b[17:17] );
sram6T_blwl sram_blwl_18_ (mux_1level_tapbuf_size3_15_sram_blwl_out[18:18] ,mux_1level_tapbuf_size3_15_sram_blwl_out[18:18] ,mux_1level_tapbuf_size3_15_sram_blwl_outb[18:18] ,mux_1level_tapbuf_size3_15_configbus0[18:18], mux_1level_tapbuf_size3_15_configbus1[18:18] , mux_1level_tapbuf_size3_15_configbus0_b[18:18] );
sram6T_blwl sram_blwl_19_ (mux_1level_tapbuf_size3_15_sram_blwl_out[19:19] ,mux_1level_tapbuf_size3_15_sram_blwl_out[19:19] ,mux_1level_tapbuf_size3_15_sram_blwl_outb[19:19] ,mux_1level_tapbuf_size3_15_configbus0[19:19], mux_1level_tapbuf_size3_15_configbus1[19:19] , mux_1level_tapbuf_size3_15_configbus0_b[19:19] );
wire [0:1] mux_1level_tapbuf_size2_16_inbus;
assign mux_1level_tapbuf_size2_16_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_16_inbus[1] = chany_0__1__in_1_ ;
wire [20:20] mux_1level_tapbuf_size2_16_configbus0;
wire [20:20] mux_1level_tapbuf_size2_16_configbus1;
wire [20:20] mux_1level_tapbuf_size2_16_sram_blwl_out ;
wire [20:20] mux_1level_tapbuf_size2_16_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_16_configbus0[20:20] = sram_blwl_bl[20:20] ;
assign mux_1level_tapbuf_size2_16_configbus1[20:20] = sram_blwl_wl[20:20] ;
wire [20:20] mux_1level_tapbuf_size2_16_configbus0_b;
assign mux_1level_tapbuf_size2_16_configbus0_b[20:20] = sram_blwl_blb[20:20] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_16_ (mux_1level_tapbuf_size2_16_inbus, chanx_1__0__out_2_ , mux_1level_tapbuf_size2_16_sram_blwl_out[20:20] ,
mux_1level_tapbuf_size2_16_sram_blwl_outb[20:20] );
//----- SRAM bits for MUX[16], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_20_ (mux_1level_tapbuf_size2_16_sram_blwl_out[20:20] ,mux_1level_tapbuf_size2_16_sram_blwl_out[20:20] ,mux_1level_tapbuf_size2_16_sram_blwl_outb[20:20] ,mux_1level_tapbuf_size2_16_configbus0[20:20], mux_1level_tapbuf_size2_16_configbus1[20:20] , mux_1level_tapbuf_size2_16_configbus0_b[20:20] );
wire [0:1] mux_1level_tapbuf_size2_17_inbus;
assign mux_1level_tapbuf_size2_17_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_17_inbus[1] = chany_0__1__in_3_ ;
wire [21:21] mux_1level_tapbuf_size2_17_configbus0;
wire [21:21] mux_1level_tapbuf_size2_17_configbus1;
wire [21:21] mux_1level_tapbuf_size2_17_sram_blwl_out ;
wire [21:21] mux_1level_tapbuf_size2_17_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_17_configbus0[21:21] = sram_blwl_bl[21:21] ;
assign mux_1level_tapbuf_size2_17_configbus1[21:21] = sram_blwl_wl[21:21] ;
wire [21:21] mux_1level_tapbuf_size2_17_configbus0_b;
assign mux_1level_tapbuf_size2_17_configbus0_b[21:21] = sram_blwl_blb[21:21] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_17_ (mux_1level_tapbuf_size2_17_inbus, chanx_1__0__out_4_ , mux_1level_tapbuf_size2_17_sram_blwl_out[21:21] ,
mux_1level_tapbuf_size2_17_sram_blwl_outb[21:21] );
//----- SRAM bits for MUX[17], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_21_ (mux_1level_tapbuf_size2_17_sram_blwl_out[21:21] ,mux_1level_tapbuf_size2_17_sram_blwl_out[21:21] ,mux_1level_tapbuf_size2_17_sram_blwl_outb[21:21] ,mux_1level_tapbuf_size2_17_configbus0[21:21], mux_1level_tapbuf_size2_17_configbus1[21:21] , mux_1level_tapbuf_size2_17_configbus0_b[21:21] );
wire [0:1] mux_1level_tapbuf_size2_18_inbus;
assign mux_1level_tapbuf_size2_18_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_18_inbus[1] = chany_0__1__in_5_ ;
wire [22:22] mux_1level_tapbuf_size2_18_configbus0;
wire [22:22] mux_1level_tapbuf_size2_18_configbus1;
wire [22:22] mux_1level_tapbuf_size2_18_sram_blwl_out ;
wire [22:22] mux_1level_tapbuf_size2_18_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_18_configbus0[22:22] = sram_blwl_bl[22:22] ;
assign mux_1level_tapbuf_size2_18_configbus1[22:22] = sram_blwl_wl[22:22] ;
wire [22:22] mux_1level_tapbuf_size2_18_configbus0_b;
assign mux_1level_tapbuf_size2_18_configbus0_b[22:22] = sram_blwl_blb[22:22] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_18_ (mux_1level_tapbuf_size2_18_inbus, chanx_1__0__out_6_ , mux_1level_tapbuf_size2_18_sram_blwl_out[22:22] ,
mux_1level_tapbuf_size2_18_sram_blwl_outb[22:22] );
//----- SRAM bits for MUX[18], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_22_ (mux_1level_tapbuf_size2_18_sram_blwl_out[22:22] ,mux_1level_tapbuf_size2_18_sram_blwl_out[22:22] ,mux_1level_tapbuf_size2_18_sram_blwl_outb[22:22] ,mux_1level_tapbuf_size2_18_configbus0[22:22], mux_1level_tapbuf_size2_18_configbus1[22:22] , mux_1level_tapbuf_size2_18_configbus0_b[22:22] );
wire [0:1] mux_1level_tapbuf_size2_19_inbus;
assign mux_1level_tapbuf_size2_19_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_19_inbus[1] = chany_0__1__in_7_ ;
wire [23:23] mux_1level_tapbuf_size2_19_configbus0;
wire [23:23] mux_1level_tapbuf_size2_19_configbus1;
wire [23:23] mux_1level_tapbuf_size2_19_sram_blwl_out ;
wire [23:23] mux_1level_tapbuf_size2_19_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_19_configbus0[23:23] = sram_blwl_bl[23:23] ;
assign mux_1level_tapbuf_size2_19_configbus1[23:23] = sram_blwl_wl[23:23] ;
wire [23:23] mux_1level_tapbuf_size2_19_configbus0_b;
assign mux_1level_tapbuf_size2_19_configbus0_b[23:23] = sram_blwl_blb[23:23] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_19_ (mux_1level_tapbuf_size2_19_inbus, chanx_1__0__out_8_ , mux_1level_tapbuf_size2_19_sram_blwl_out[23:23] ,
mux_1level_tapbuf_size2_19_sram_blwl_outb[23:23] );
//----- SRAM bits for MUX[19], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_23_ (mux_1level_tapbuf_size2_19_sram_blwl_out[23:23] ,mux_1level_tapbuf_size2_19_sram_blwl_out[23:23] ,mux_1level_tapbuf_size2_19_sram_blwl_outb[23:23] ,mux_1level_tapbuf_size2_19_configbus0[23:23], mux_1level_tapbuf_size2_19_configbus1[23:23] , mux_1level_tapbuf_size2_19_configbus0_b[23:23] );
wire [0:1] mux_1level_tapbuf_size2_20_inbus;
assign mux_1level_tapbuf_size2_20_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_20_inbus[1] = chany_0__1__in_9_ ;
wire [24:24] mux_1level_tapbuf_size2_20_configbus0;
wire [24:24] mux_1level_tapbuf_size2_20_configbus1;
wire [24:24] mux_1level_tapbuf_size2_20_sram_blwl_out ;
wire [24:24] mux_1level_tapbuf_size2_20_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_20_configbus0[24:24] = sram_blwl_bl[24:24] ;
assign mux_1level_tapbuf_size2_20_configbus1[24:24] = sram_blwl_wl[24:24] ;
wire [24:24] mux_1level_tapbuf_size2_20_configbus0_b;
assign mux_1level_tapbuf_size2_20_configbus0_b[24:24] = sram_blwl_blb[24:24] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_20_ (mux_1level_tapbuf_size2_20_inbus, chanx_1__0__out_10_ , mux_1level_tapbuf_size2_20_sram_blwl_out[24:24] ,
mux_1level_tapbuf_size2_20_sram_blwl_outb[24:24] );
//----- SRAM bits for MUX[20], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_24_ (mux_1level_tapbuf_size2_20_sram_blwl_out[24:24] ,mux_1level_tapbuf_size2_20_sram_blwl_out[24:24] ,mux_1level_tapbuf_size2_20_sram_blwl_outb[24:24] ,mux_1level_tapbuf_size2_20_configbus0[24:24], mux_1level_tapbuf_size2_20_configbus1[24:24] , mux_1level_tapbuf_size2_20_configbus0_b[24:24] );
wire [0:1] mux_1level_tapbuf_size2_21_inbus;
assign mux_1level_tapbuf_size2_21_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_21_inbus[1] = chany_0__1__in_11_ ;
wire [25:25] mux_1level_tapbuf_size2_21_configbus0;
wire [25:25] mux_1level_tapbuf_size2_21_configbus1;
wire [25:25] mux_1level_tapbuf_size2_21_sram_blwl_out ;
wire [25:25] mux_1level_tapbuf_size2_21_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_21_configbus0[25:25] = sram_blwl_bl[25:25] ;
assign mux_1level_tapbuf_size2_21_configbus1[25:25] = sram_blwl_wl[25:25] ;
wire [25:25] mux_1level_tapbuf_size2_21_configbus0_b;
assign mux_1level_tapbuf_size2_21_configbus0_b[25:25] = sram_blwl_blb[25:25] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_21_ (mux_1level_tapbuf_size2_21_inbus, chanx_1__0__out_12_ , mux_1level_tapbuf_size2_21_sram_blwl_out[25:25] ,
mux_1level_tapbuf_size2_21_sram_blwl_outb[25:25] );
//----- SRAM bits for MUX[21], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_25_ (mux_1level_tapbuf_size2_21_sram_blwl_out[25:25] ,mux_1level_tapbuf_size2_21_sram_blwl_out[25:25] ,mux_1level_tapbuf_size2_21_sram_blwl_outb[25:25] ,mux_1level_tapbuf_size2_21_configbus0[25:25], mux_1level_tapbuf_size2_21_configbus1[25:25] , mux_1level_tapbuf_size2_21_configbus0_b[25:25] );
wire [0:1] mux_1level_tapbuf_size2_22_inbus;
assign mux_1level_tapbuf_size2_22_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_22_inbus[1] = chany_0__1__in_13_ ;
wire [26:26] mux_1level_tapbuf_size2_22_configbus0;
wire [26:26] mux_1level_tapbuf_size2_22_configbus1;
wire [26:26] mux_1level_tapbuf_size2_22_sram_blwl_out ;
wire [26:26] mux_1level_tapbuf_size2_22_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_22_configbus0[26:26] = sram_blwl_bl[26:26] ;
assign mux_1level_tapbuf_size2_22_configbus1[26:26] = sram_blwl_wl[26:26] ;
wire [26:26] mux_1level_tapbuf_size2_22_configbus0_b;
assign mux_1level_tapbuf_size2_22_configbus0_b[26:26] = sram_blwl_blb[26:26] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_22_ (mux_1level_tapbuf_size2_22_inbus, chanx_1__0__out_14_ , mux_1level_tapbuf_size2_22_sram_blwl_out[26:26] ,
mux_1level_tapbuf_size2_22_sram_blwl_outb[26:26] );
//----- SRAM bits for MUX[22], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_26_ (mux_1level_tapbuf_size2_22_sram_blwl_out[26:26] ,mux_1level_tapbuf_size2_22_sram_blwl_out[26:26] ,mux_1level_tapbuf_size2_22_sram_blwl_outb[26:26] ,mux_1level_tapbuf_size2_22_configbus0[26:26], mux_1level_tapbuf_size2_22_configbus1[26:26] , mux_1level_tapbuf_size2_22_configbus0_b[26:26] );
wire [0:1] mux_1level_tapbuf_size2_23_inbus;
assign mux_1level_tapbuf_size2_23_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_23_inbus[1] = chany_0__1__in_15_ ;
wire [27:27] mux_1level_tapbuf_size2_23_configbus0;
wire [27:27] mux_1level_tapbuf_size2_23_configbus1;
wire [27:27] mux_1level_tapbuf_size2_23_sram_blwl_out ;
wire [27:27] mux_1level_tapbuf_size2_23_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_23_configbus0[27:27] = sram_blwl_bl[27:27] ;
assign mux_1level_tapbuf_size2_23_configbus1[27:27] = sram_blwl_wl[27:27] ;
wire [27:27] mux_1level_tapbuf_size2_23_configbus0_b;
assign mux_1level_tapbuf_size2_23_configbus0_b[27:27] = sram_blwl_blb[27:27] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_23_ (mux_1level_tapbuf_size2_23_inbus, chanx_1__0__out_16_ , mux_1level_tapbuf_size2_23_sram_blwl_out[27:27] ,
mux_1level_tapbuf_size2_23_sram_blwl_outb[27:27] );
//----- SRAM bits for MUX[23], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_27_ (mux_1level_tapbuf_size2_23_sram_blwl_out[27:27] ,mux_1level_tapbuf_size2_23_sram_blwl_out[27:27] ,mux_1level_tapbuf_size2_23_sram_blwl_outb[27:27] ,mux_1level_tapbuf_size2_23_configbus0[27:27], mux_1level_tapbuf_size2_23_configbus1[27:27] , mux_1level_tapbuf_size2_23_configbus0_b[27:27] );
wire [0:1] mux_1level_tapbuf_size2_24_inbus;
assign mux_1level_tapbuf_size2_24_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_24_inbus[1] = chany_0__1__in_17_ ;
wire [28:28] mux_1level_tapbuf_size2_24_configbus0;
wire [28:28] mux_1level_tapbuf_size2_24_configbus1;
wire [28:28] mux_1level_tapbuf_size2_24_sram_blwl_out ;
wire [28:28] mux_1level_tapbuf_size2_24_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_24_configbus0[28:28] = sram_blwl_bl[28:28] ;
assign mux_1level_tapbuf_size2_24_configbus1[28:28] = sram_blwl_wl[28:28] ;
wire [28:28] mux_1level_tapbuf_size2_24_configbus0_b;
assign mux_1level_tapbuf_size2_24_configbus0_b[28:28] = sram_blwl_blb[28:28] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_24_ (mux_1level_tapbuf_size2_24_inbus, chanx_1__0__out_18_ , mux_1level_tapbuf_size2_24_sram_blwl_out[28:28] ,
mux_1level_tapbuf_size2_24_sram_blwl_outb[28:28] );
//----- SRAM bits for MUX[24], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_28_ (mux_1level_tapbuf_size2_24_sram_blwl_out[28:28] ,mux_1level_tapbuf_size2_24_sram_blwl_out[28:28] ,mux_1level_tapbuf_size2_24_sram_blwl_outb[28:28] ,mux_1level_tapbuf_size2_24_configbus0[28:28], mux_1level_tapbuf_size2_24_configbus1[28:28] , mux_1level_tapbuf_size2_24_configbus0_b[28:28] );
wire [0:1] mux_1level_tapbuf_size2_25_inbus;
assign mux_1level_tapbuf_size2_25_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_25_inbus[1] = chany_0__1__in_19_ ;
wire [29:29] mux_1level_tapbuf_size2_25_configbus0;
wire [29:29] mux_1level_tapbuf_size2_25_configbus1;
wire [29:29] mux_1level_tapbuf_size2_25_sram_blwl_out ;
wire [29:29] mux_1level_tapbuf_size2_25_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_25_configbus0[29:29] = sram_blwl_bl[29:29] ;
assign mux_1level_tapbuf_size2_25_configbus1[29:29] = sram_blwl_wl[29:29] ;
wire [29:29] mux_1level_tapbuf_size2_25_configbus0_b;
assign mux_1level_tapbuf_size2_25_configbus0_b[29:29] = sram_blwl_blb[29:29] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_25_ (mux_1level_tapbuf_size2_25_inbus, chanx_1__0__out_20_ , mux_1level_tapbuf_size2_25_sram_blwl_out[29:29] ,
mux_1level_tapbuf_size2_25_sram_blwl_outb[29:29] );
//----- SRAM bits for MUX[25], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_29_ (mux_1level_tapbuf_size2_25_sram_blwl_out[29:29] ,mux_1level_tapbuf_size2_25_sram_blwl_out[29:29] ,mux_1level_tapbuf_size2_25_sram_blwl_outb[29:29] ,mux_1level_tapbuf_size2_25_configbus0[29:29], mux_1level_tapbuf_size2_25_configbus1[29:29] , mux_1level_tapbuf_size2_25_configbus0_b[29:29] );
wire [0:1] mux_1level_tapbuf_size2_26_inbus;
assign mux_1level_tapbuf_size2_26_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_26_inbus[1] = chany_0__1__in_21_ ;
wire [30:30] mux_1level_tapbuf_size2_26_configbus0;
wire [30:30] mux_1level_tapbuf_size2_26_configbus1;
wire [30:30] mux_1level_tapbuf_size2_26_sram_blwl_out ;
wire [30:30] mux_1level_tapbuf_size2_26_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_26_configbus0[30:30] = sram_blwl_bl[30:30] ;
assign mux_1level_tapbuf_size2_26_configbus1[30:30] = sram_blwl_wl[30:30] ;
wire [30:30] mux_1level_tapbuf_size2_26_configbus0_b;
assign mux_1level_tapbuf_size2_26_configbus0_b[30:30] = sram_blwl_blb[30:30] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_26_ (mux_1level_tapbuf_size2_26_inbus, chanx_1__0__out_22_ , mux_1level_tapbuf_size2_26_sram_blwl_out[30:30] ,
mux_1level_tapbuf_size2_26_sram_blwl_outb[30:30] );
//----- SRAM bits for MUX[26], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_30_ (mux_1level_tapbuf_size2_26_sram_blwl_out[30:30] ,mux_1level_tapbuf_size2_26_sram_blwl_out[30:30] ,mux_1level_tapbuf_size2_26_sram_blwl_outb[30:30] ,mux_1level_tapbuf_size2_26_configbus0[30:30], mux_1level_tapbuf_size2_26_configbus1[30:30] , mux_1level_tapbuf_size2_26_configbus0_b[30:30] );
wire [0:1] mux_1level_tapbuf_size2_27_inbus;
assign mux_1level_tapbuf_size2_27_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_27_inbus[1] = chany_0__1__in_23_ ;
wire [31:31] mux_1level_tapbuf_size2_27_configbus0;
wire [31:31] mux_1level_tapbuf_size2_27_configbus1;
wire [31:31] mux_1level_tapbuf_size2_27_sram_blwl_out ;
wire [31:31] mux_1level_tapbuf_size2_27_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_27_configbus0[31:31] = sram_blwl_bl[31:31] ;
assign mux_1level_tapbuf_size2_27_configbus1[31:31] = sram_blwl_wl[31:31] ;
wire [31:31] mux_1level_tapbuf_size2_27_configbus0_b;
assign mux_1level_tapbuf_size2_27_configbus0_b[31:31] = sram_blwl_blb[31:31] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_27_ (mux_1level_tapbuf_size2_27_inbus, chanx_1__0__out_24_ , mux_1level_tapbuf_size2_27_sram_blwl_out[31:31] ,
mux_1level_tapbuf_size2_27_sram_blwl_outb[31:31] );
//----- SRAM bits for MUX[27], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_31_ (mux_1level_tapbuf_size2_27_sram_blwl_out[31:31] ,mux_1level_tapbuf_size2_27_sram_blwl_out[31:31] ,mux_1level_tapbuf_size2_27_sram_blwl_outb[31:31] ,mux_1level_tapbuf_size2_27_configbus0[31:31], mux_1level_tapbuf_size2_27_configbus1[31:31] , mux_1level_tapbuf_size2_27_configbus0_b[31:31] );
wire [0:1] mux_1level_tapbuf_size2_28_inbus;
assign mux_1level_tapbuf_size2_28_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_28_inbus[1] = chany_0__1__in_25_ ;
wire [32:32] mux_1level_tapbuf_size2_28_configbus0;
wire [32:32] mux_1level_tapbuf_size2_28_configbus1;
wire [32:32] mux_1level_tapbuf_size2_28_sram_blwl_out ;
wire [32:32] mux_1level_tapbuf_size2_28_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_28_configbus0[32:32] = sram_blwl_bl[32:32] ;
assign mux_1level_tapbuf_size2_28_configbus1[32:32] = sram_blwl_wl[32:32] ;
wire [32:32] mux_1level_tapbuf_size2_28_configbus0_b;
assign mux_1level_tapbuf_size2_28_configbus0_b[32:32] = sram_blwl_blb[32:32] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_28_ (mux_1level_tapbuf_size2_28_inbus, chanx_1__0__out_26_ , mux_1level_tapbuf_size2_28_sram_blwl_out[32:32] ,
mux_1level_tapbuf_size2_28_sram_blwl_outb[32:32] );
//----- SRAM bits for MUX[28], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_32_ (mux_1level_tapbuf_size2_28_sram_blwl_out[32:32] ,mux_1level_tapbuf_size2_28_sram_blwl_out[32:32] ,mux_1level_tapbuf_size2_28_sram_blwl_outb[32:32] ,mux_1level_tapbuf_size2_28_configbus0[32:32], mux_1level_tapbuf_size2_28_configbus1[32:32] , mux_1level_tapbuf_size2_28_configbus0_b[32:32] );
wire [0:1] mux_1level_tapbuf_size2_29_inbus;
assign mux_1level_tapbuf_size2_29_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_29_inbus[1] = chany_0__1__in_27_ ;
wire [33:33] mux_1level_tapbuf_size2_29_configbus0;
wire [33:33] mux_1level_tapbuf_size2_29_configbus1;
wire [33:33] mux_1level_tapbuf_size2_29_sram_blwl_out ;
wire [33:33] mux_1level_tapbuf_size2_29_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_29_configbus0[33:33] = sram_blwl_bl[33:33] ;
assign mux_1level_tapbuf_size2_29_configbus1[33:33] = sram_blwl_wl[33:33] ;
wire [33:33] mux_1level_tapbuf_size2_29_configbus0_b;
assign mux_1level_tapbuf_size2_29_configbus0_b[33:33] = sram_blwl_blb[33:33] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_29_ (mux_1level_tapbuf_size2_29_inbus, chanx_1__0__out_28_ , mux_1level_tapbuf_size2_29_sram_blwl_out[33:33] ,
mux_1level_tapbuf_size2_29_sram_blwl_outb[33:33] );
//----- SRAM bits for MUX[29], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_33_ (mux_1level_tapbuf_size2_29_sram_blwl_out[33:33] ,mux_1level_tapbuf_size2_29_sram_blwl_out[33:33] ,mux_1level_tapbuf_size2_29_sram_blwl_outb[33:33] ,mux_1level_tapbuf_size2_29_configbus0[33:33], mux_1level_tapbuf_size2_29_configbus1[33:33] , mux_1level_tapbuf_size2_29_configbus0_b[33:33] );
//----- bottom side Multiplexers -----
//----- left side Multiplexers -----
endmodule
//----- END Verilog Module of Switch Box[0][0] -----

