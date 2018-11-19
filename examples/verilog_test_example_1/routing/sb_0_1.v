//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Switch Block  [0][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Verilog Module of Switch Box[0][1] -----
module sb_0__1_ ( 

//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
//----- Inputs/outputs of top side -----
//----- Inputs/outputs of right side -----
  output chanx_1__1__out_0_,
  input chanx_1__1__in_1_,
  output chanx_1__1__out_2_,
  input chanx_1__1__in_3_,
  output chanx_1__1__out_4_,
  input chanx_1__1__in_5_,
  output chanx_1__1__out_6_,
  input chanx_1__1__in_7_,
  output chanx_1__1__out_8_,
  input chanx_1__1__in_9_,
  output chanx_1__1__out_10_,
  input chanx_1__1__in_11_,
  output chanx_1__1__out_12_,
  input chanx_1__1__in_13_,
  output chanx_1__1__out_14_,
  input chanx_1__1__in_15_,
  output chanx_1__1__out_16_,
  input chanx_1__1__in_17_,
  output chanx_1__1__out_18_,
  input chanx_1__1__in_19_,
  output chanx_1__1__out_20_,
  input chanx_1__1__in_21_,
  output chanx_1__1__out_22_,
  input chanx_1__1__in_23_,
  output chanx_1__1__out_24_,
  input chanx_1__1__in_25_,
  output chanx_1__1__out_26_,
  input chanx_1__1__in_27_,
  output chanx_1__1__out_28_,
  input chanx_1__1__in_29_,
input  grid_1__2__pin_0__2__1_,
input  grid_1__2__pin_0__2__3_,
input  grid_1__2__pin_0__2__5_,
input  grid_1__2__pin_0__2__7_,
input  grid_1__2__pin_0__2__9_,
input  grid_1__2__pin_0__2__11_,
input  grid_1__2__pin_0__2__13_,
input  grid_1__2__pin_0__2__15_,
input  grid_1__1__pin_0__0__4_,
//----- Inputs/outputs of bottom side -----
  input chany_0__1__in_0_,
  output chany_0__1__out_1_,
  input chany_0__1__in_2_,
  output chany_0__1__out_3_,
  input chany_0__1__in_4_,
  output chany_0__1__out_5_,
  input chany_0__1__in_6_,
  output chany_0__1__out_7_,
  input chany_0__1__in_8_,
  output chany_0__1__out_9_,
  input chany_0__1__in_10_,
  output chany_0__1__out_11_,
  input chany_0__1__in_12_,
  output chany_0__1__out_13_,
  input chany_0__1__in_14_,
  output chany_0__1__out_15_,
  input chany_0__1__in_16_,
  output chany_0__1__out_17_,
  input chany_0__1__in_18_,
  output chany_0__1__out_19_,
  input chany_0__1__in_20_,
  output chany_0__1__out_21_,
  input chany_0__1__in_22_,
  output chany_0__1__out_23_,
  input chany_0__1__in_24_,
  output chany_0__1__out_25_,
  input chany_0__1__in_26_,
  output chany_0__1__out_27_,
  input chany_0__1__in_28_,
  output chany_0__1__out_29_,
input  grid_0__1__pin_0__1__1_,
input  grid_0__1__pin_0__1__3_,
input  grid_0__1__pin_0__1__5_,
input  grid_0__1__pin_0__1__7_,
input  grid_0__1__pin_0__1__9_,
input  grid_0__1__pin_0__1__11_,
input  grid_0__1__pin_0__1__13_,
input  grid_0__1__pin_0__1__15_,
//----- Inputs/outputs of left side -----
input [34:71] sram_blwl_bl ,
input [34:71] sram_blwl_wl ,
input [34:71] sram_blwl_blb ); 
//----- top side Multiplexers -----
//----- right side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_30_inbus;
assign mux_1level_tapbuf_size3_30_inbus[0] =  grid_1__1__pin_0__0__4_;
assign mux_1level_tapbuf_size3_30_inbus[1] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size3_30_inbus[2] = chany_0__1__in_26_ ;
wire [34:36] mux_1level_tapbuf_size3_30_configbus0;
wire [34:36] mux_1level_tapbuf_size3_30_configbus1;
wire [34:36] mux_1level_tapbuf_size3_30_sram_blwl_out ;
wire [34:36] mux_1level_tapbuf_size3_30_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_30_configbus0[34:36] = sram_blwl_bl[34:36] ;
assign mux_1level_tapbuf_size3_30_configbus1[34:36] = sram_blwl_wl[34:36] ;
wire [34:36] mux_1level_tapbuf_size3_30_configbus0_b;
assign mux_1level_tapbuf_size3_30_configbus0_b[34:36] = sram_blwl_blb[34:36] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_30_ (mux_1level_tapbuf_size3_30_inbus, chanx_1__1__out_0_ , mux_1level_tapbuf_size3_30_sram_blwl_out[34:36] ,
mux_1level_tapbuf_size3_30_sram_blwl_outb[34:36] );
//----- SRAM bits for MUX[30], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_34_ (mux_1level_tapbuf_size3_30_sram_blwl_out[34:34] ,mux_1level_tapbuf_size3_30_sram_blwl_out[34:34] ,mux_1level_tapbuf_size3_30_sram_blwl_outb[34:34] ,mux_1level_tapbuf_size3_30_configbus0[34:34], mux_1level_tapbuf_size3_30_configbus1[34:34] , mux_1level_tapbuf_size3_30_configbus0_b[34:34] );
sram6T_blwl sram_blwl_35_ (mux_1level_tapbuf_size3_30_sram_blwl_out[35:35] ,mux_1level_tapbuf_size3_30_sram_blwl_out[35:35] ,mux_1level_tapbuf_size3_30_sram_blwl_outb[35:35] ,mux_1level_tapbuf_size3_30_configbus0[35:35], mux_1level_tapbuf_size3_30_configbus1[35:35] , mux_1level_tapbuf_size3_30_configbus0_b[35:35] );
sram6T_blwl sram_blwl_36_ (mux_1level_tapbuf_size3_30_sram_blwl_out[36:36] ,mux_1level_tapbuf_size3_30_sram_blwl_out[36:36] ,mux_1level_tapbuf_size3_30_sram_blwl_outb[36:36] ,mux_1level_tapbuf_size3_30_configbus0[36:36], mux_1level_tapbuf_size3_30_configbus1[36:36] , mux_1level_tapbuf_size3_30_configbus0_b[36:36] );
wire [0:2] mux_1level_tapbuf_size3_31_inbus;
assign mux_1level_tapbuf_size3_31_inbus[0] =  grid_1__1__pin_0__0__4_;
assign mux_1level_tapbuf_size3_31_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_31_inbus[2] = chany_0__1__in_24_ ;
wire [37:39] mux_1level_tapbuf_size3_31_configbus0;
wire [37:39] mux_1level_tapbuf_size3_31_configbus1;
wire [37:39] mux_1level_tapbuf_size3_31_sram_blwl_out ;
wire [37:39] mux_1level_tapbuf_size3_31_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_31_configbus0[37:39] = sram_blwl_bl[37:39] ;
assign mux_1level_tapbuf_size3_31_configbus1[37:39] = sram_blwl_wl[37:39] ;
wire [37:39] mux_1level_tapbuf_size3_31_configbus0_b;
assign mux_1level_tapbuf_size3_31_configbus0_b[37:39] = sram_blwl_blb[37:39] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_31_ (mux_1level_tapbuf_size3_31_inbus, chanx_1__1__out_2_ , mux_1level_tapbuf_size3_31_sram_blwl_out[37:39] ,
mux_1level_tapbuf_size3_31_sram_blwl_outb[37:39] );
//----- SRAM bits for MUX[31], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_37_ (mux_1level_tapbuf_size3_31_sram_blwl_out[37:37] ,mux_1level_tapbuf_size3_31_sram_blwl_out[37:37] ,mux_1level_tapbuf_size3_31_sram_blwl_outb[37:37] ,mux_1level_tapbuf_size3_31_configbus0[37:37], mux_1level_tapbuf_size3_31_configbus1[37:37] , mux_1level_tapbuf_size3_31_configbus0_b[37:37] );
sram6T_blwl sram_blwl_38_ (mux_1level_tapbuf_size3_31_sram_blwl_out[38:38] ,mux_1level_tapbuf_size3_31_sram_blwl_out[38:38] ,mux_1level_tapbuf_size3_31_sram_blwl_outb[38:38] ,mux_1level_tapbuf_size3_31_configbus0[38:38], mux_1level_tapbuf_size3_31_configbus1[38:38] , mux_1level_tapbuf_size3_31_configbus0_b[38:38] );
sram6T_blwl sram_blwl_39_ (mux_1level_tapbuf_size3_31_sram_blwl_out[39:39] ,mux_1level_tapbuf_size3_31_sram_blwl_out[39:39] ,mux_1level_tapbuf_size3_31_sram_blwl_outb[39:39] ,mux_1level_tapbuf_size3_31_configbus0[39:39], mux_1level_tapbuf_size3_31_configbus1[39:39] , mux_1level_tapbuf_size3_31_configbus0_b[39:39] );
wire [0:2] mux_1level_tapbuf_size3_32_inbus;
assign mux_1level_tapbuf_size3_32_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size3_32_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_32_inbus[2] = chany_0__1__in_22_ ;
wire [40:42] mux_1level_tapbuf_size3_32_configbus0;
wire [40:42] mux_1level_tapbuf_size3_32_configbus1;
wire [40:42] mux_1level_tapbuf_size3_32_sram_blwl_out ;
wire [40:42] mux_1level_tapbuf_size3_32_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_32_configbus0[40:42] = sram_blwl_bl[40:42] ;
assign mux_1level_tapbuf_size3_32_configbus1[40:42] = sram_blwl_wl[40:42] ;
wire [40:42] mux_1level_tapbuf_size3_32_configbus0_b;
assign mux_1level_tapbuf_size3_32_configbus0_b[40:42] = sram_blwl_blb[40:42] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_32_ (mux_1level_tapbuf_size3_32_inbus, chanx_1__1__out_4_ , mux_1level_tapbuf_size3_32_sram_blwl_out[40:42] ,
mux_1level_tapbuf_size3_32_sram_blwl_outb[40:42] );
//----- SRAM bits for MUX[32], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_40_ (mux_1level_tapbuf_size3_32_sram_blwl_out[40:40] ,mux_1level_tapbuf_size3_32_sram_blwl_out[40:40] ,mux_1level_tapbuf_size3_32_sram_blwl_outb[40:40] ,mux_1level_tapbuf_size3_32_configbus0[40:40], mux_1level_tapbuf_size3_32_configbus1[40:40] , mux_1level_tapbuf_size3_32_configbus0_b[40:40] );
sram6T_blwl sram_blwl_41_ (mux_1level_tapbuf_size3_32_sram_blwl_out[41:41] ,mux_1level_tapbuf_size3_32_sram_blwl_out[41:41] ,mux_1level_tapbuf_size3_32_sram_blwl_outb[41:41] ,mux_1level_tapbuf_size3_32_configbus0[41:41], mux_1level_tapbuf_size3_32_configbus1[41:41] , mux_1level_tapbuf_size3_32_configbus0_b[41:41] );
sram6T_blwl sram_blwl_42_ (mux_1level_tapbuf_size3_32_sram_blwl_out[42:42] ,mux_1level_tapbuf_size3_32_sram_blwl_out[42:42] ,mux_1level_tapbuf_size3_32_sram_blwl_outb[42:42] ,mux_1level_tapbuf_size3_32_configbus0[42:42], mux_1level_tapbuf_size3_32_configbus1[42:42] , mux_1level_tapbuf_size3_32_configbus0_b[42:42] );
wire [0:1] mux_1level_tapbuf_size2_33_inbus;
assign mux_1level_tapbuf_size2_33_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_33_inbus[1] = chany_0__1__in_20_ ;
wire [43:43] mux_1level_tapbuf_size2_33_configbus0;
wire [43:43] mux_1level_tapbuf_size2_33_configbus1;
wire [43:43] mux_1level_tapbuf_size2_33_sram_blwl_out ;
wire [43:43] mux_1level_tapbuf_size2_33_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_33_configbus0[43:43] = sram_blwl_bl[43:43] ;
assign mux_1level_tapbuf_size2_33_configbus1[43:43] = sram_blwl_wl[43:43] ;
wire [43:43] mux_1level_tapbuf_size2_33_configbus0_b;
assign mux_1level_tapbuf_size2_33_configbus0_b[43:43] = sram_blwl_blb[43:43] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_33_ (mux_1level_tapbuf_size2_33_inbus, chanx_1__1__out_6_ , mux_1level_tapbuf_size2_33_sram_blwl_out[43:43] ,
mux_1level_tapbuf_size2_33_sram_blwl_outb[43:43] );
//----- SRAM bits for MUX[33], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_43_ (mux_1level_tapbuf_size2_33_sram_blwl_out[43:43] ,mux_1level_tapbuf_size2_33_sram_blwl_out[43:43] ,mux_1level_tapbuf_size2_33_sram_blwl_outb[43:43] ,mux_1level_tapbuf_size2_33_configbus0[43:43], mux_1level_tapbuf_size2_33_configbus1[43:43] , mux_1level_tapbuf_size2_33_configbus0_b[43:43] );
wire [0:1] mux_1level_tapbuf_size2_34_inbus;
assign mux_1level_tapbuf_size2_34_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_34_inbus[1] = chany_0__1__in_18_ ;
wire [44:44] mux_1level_tapbuf_size2_34_configbus0;
wire [44:44] mux_1level_tapbuf_size2_34_configbus1;
wire [44:44] mux_1level_tapbuf_size2_34_sram_blwl_out ;
wire [44:44] mux_1level_tapbuf_size2_34_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_34_configbus0[44:44] = sram_blwl_bl[44:44] ;
assign mux_1level_tapbuf_size2_34_configbus1[44:44] = sram_blwl_wl[44:44] ;
wire [44:44] mux_1level_tapbuf_size2_34_configbus0_b;
assign mux_1level_tapbuf_size2_34_configbus0_b[44:44] = sram_blwl_blb[44:44] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_34_ (mux_1level_tapbuf_size2_34_inbus, chanx_1__1__out_8_ , mux_1level_tapbuf_size2_34_sram_blwl_out[44:44] ,
mux_1level_tapbuf_size2_34_sram_blwl_outb[44:44] );
//----- SRAM bits for MUX[34], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_44_ (mux_1level_tapbuf_size2_34_sram_blwl_out[44:44] ,mux_1level_tapbuf_size2_34_sram_blwl_out[44:44] ,mux_1level_tapbuf_size2_34_sram_blwl_outb[44:44] ,mux_1level_tapbuf_size2_34_configbus0[44:44], mux_1level_tapbuf_size2_34_configbus1[44:44] , mux_1level_tapbuf_size2_34_configbus0_b[44:44] );
wire [0:1] mux_1level_tapbuf_size2_35_inbus;
assign mux_1level_tapbuf_size2_35_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_35_inbus[1] = chany_0__1__in_16_ ;
wire [45:45] mux_1level_tapbuf_size2_35_configbus0;
wire [45:45] mux_1level_tapbuf_size2_35_configbus1;
wire [45:45] mux_1level_tapbuf_size2_35_sram_blwl_out ;
wire [45:45] mux_1level_tapbuf_size2_35_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_35_configbus0[45:45] = sram_blwl_bl[45:45] ;
assign mux_1level_tapbuf_size2_35_configbus1[45:45] = sram_blwl_wl[45:45] ;
wire [45:45] mux_1level_tapbuf_size2_35_configbus0_b;
assign mux_1level_tapbuf_size2_35_configbus0_b[45:45] = sram_blwl_blb[45:45] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_35_ (mux_1level_tapbuf_size2_35_inbus, chanx_1__1__out_10_ , mux_1level_tapbuf_size2_35_sram_blwl_out[45:45] ,
mux_1level_tapbuf_size2_35_sram_blwl_outb[45:45] );
//----- SRAM bits for MUX[35], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_45_ (mux_1level_tapbuf_size2_35_sram_blwl_out[45:45] ,mux_1level_tapbuf_size2_35_sram_blwl_out[45:45] ,mux_1level_tapbuf_size2_35_sram_blwl_outb[45:45] ,mux_1level_tapbuf_size2_35_configbus0[45:45], mux_1level_tapbuf_size2_35_configbus1[45:45] , mux_1level_tapbuf_size2_35_configbus0_b[45:45] );
wire [0:1] mux_1level_tapbuf_size2_36_inbus;
assign mux_1level_tapbuf_size2_36_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_36_inbus[1] = chany_0__1__in_14_ ;
wire [46:46] mux_1level_tapbuf_size2_36_configbus0;
wire [46:46] mux_1level_tapbuf_size2_36_configbus1;
wire [46:46] mux_1level_tapbuf_size2_36_sram_blwl_out ;
wire [46:46] mux_1level_tapbuf_size2_36_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_36_configbus0[46:46] = sram_blwl_bl[46:46] ;
assign mux_1level_tapbuf_size2_36_configbus1[46:46] = sram_blwl_wl[46:46] ;
wire [46:46] mux_1level_tapbuf_size2_36_configbus0_b;
assign mux_1level_tapbuf_size2_36_configbus0_b[46:46] = sram_blwl_blb[46:46] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_36_ (mux_1level_tapbuf_size2_36_inbus, chanx_1__1__out_12_ , mux_1level_tapbuf_size2_36_sram_blwl_out[46:46] ,
mux_1level_tapbuf_size2_36_sram_blwl_outb[46:46] );
//----- SRAM bits for MUX[36], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_46_ (mux_1level_tapbuf_size2_36_sram_blwl_out[46:46] ,mux_1level_tapbuf_size2_36_sram_blwl_out[46:46] ,mux_1level_tapbuf_size2_36_sram_blwl_outb[46:46] ,mux_1level_tapbuf_size2_36_configbus0[46:46], mux_1level_tapbuf_size2_36_configbus1[46:46] , mux_1level_tapbuf_size2_36_configbus0_b[46:46] );
wire [0:1] mux_1level_tapbuf_size2_37_inbus;
assign mux_1level_tapbuf_size2_37_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_37_inbus[1] = chany_0__1__in_12_ ;
wire [47:47] mux_1level_tapbuf_size2_37_configbus0;
wire [47:47] mux_1level_tapbuf_size2_37_configbus1;
wire [47:47] mux_1level_tapbuf_size2_37_sram_blwl_out ;
wire [47:47] mux_1level_tapbuf_size2_37_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_37_configbus0[47:47] = sram_blwl_bl[47:47] ;
assign mux_1level_tapbuf_size2_37_configbus1[47:47] = sram_blwl_wl[47:47] ;
wire [47:47] mux_1level_tapbuf_size2_37_configbus0_b;
assign mux_1level_tapbuf_size2_37_configbus0_b[47:47] = sram_blwl_blb[47:47] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_37_ (mux_1level_tapbuf_size2_37_inbus, chanx_1__1__out_14_ , mux_1level_tapbuf_size2_37_sram_blwl_out[47:47] ,
mux_1level_tapbuf_size2_37_sram_blwl_outb[47:47] );
//----- SRAM bits for MUX[37], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_47_ (mux_1level_tapbuf_size2_37_sram_blwl_out[47:47] ,mux_1level_tapbuf_size2_37_sram_blwl_out[47:47] ,mux_1level_tapbuf_size2_37_sram_blwl_outb[47:47] ,mux_1level_tapbuf_size2_37_configbus0[47:47], mux_1level_tapbuf_size2_37_configbus1[47:47] , mux_1level_tapbuf_size2_37_configbus0_b[47:47] );
wire [0:1] mux_1level_tapbuf_size2_38_inbus;
assign mux_1level_tapbuf_size2_38_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_38_inbus[1] = chany_0__1__in_10_ ;
wire [48:48] mux_1level_tapbuf_size2_38_configbus0;
wire [48:48] mux_1level_tapbuf_size2_38_configbus1;
wire [48:48] mux_1level_tapbuf_size2_38_sram_blwl_out ;
wire [48:48] mux_1level_tapbuf_size2_38_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_38_configbus0[48:48] = sram_blwl_bl[48:48] ;
assign mux_1level_tapbuf_size2_38_configbus1[48:48] = sram_blwl_wl[48:48] ;
wire [48:48] mux_1level_tapbuf_size2_38_configbus0_b;
assign mux_1level_tapbuf_size2_38_configbus0_b[48:48] = sram_blwl_blb[48:48] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_38_ (mux_1level_tapbuf_size2_38_inbus, chanx_1__1__out_16_ , mux_1level_tapbuf_size2_38_sram_blwl_out[48:48] ,
mux_1level_tapbuf_size2_38_sram_blwl_outb[48:48] );
//----- SRAM bits for MUX[38], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_48_ (mux_1level_tapbuf_size2_38_sram_blwl_out[48:48] ,mux_1level_tapbuf_size2_38_sram_blwl_out[48:48] ,mux_1level_tapbuf_size2_38_sram_blwl_outb[48:48] ,mux_1level_tapbuf_size2_38_configbus0[48:48], mux_1level_tapbuf_size2_38_configbus1[48:48] , mux_1level_tapbuf_size2_38_configbus0_b[48:48] );
wire [0:1] mux_1level_tapbuf_size2_39_inbus;
assign mux_1level_tapbuf_size2_39_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_39_inbus[1] = chany_0__1__in_8_ ;
wire [49:49] mux_1level_tapbuf_size2_39_configbus0;
wire [49:49] mux_1level_tapbuf_size2_39_configbus1;
wire [49:49] mux_1level_tapbuf_size2_39_sram_blwl_out ;
wire [49:49] mux_1level_tapbuf_size2_39_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_39_configbus0[49:49] = sram_blwl_bl[49:49] ;
assign mux_1level_tapbuf_size2_39_configbus1[49:49] = sram_blwl_wl[49:49] ;
wire [49:49] mux_1level_tapbuf_size2_39_configbus0_b;
assign mux_1level_tapbuf_size2_39_configbus0_b[49:49] = sram_blwl_blb[49:49] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_39_ (mux_1level_tapbuf_size2_39_inbus, chanx_1__1__out_18_ , mux_1level_tapbuf_size2_39_sram_blwl_out[49:49] ,
mux_1level_tapbuf_size2_39_sram_blwl_outb[49:49] );
//----- SRAM bits for MUX[39], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_49_ (mux_1level_tapbuf_size2_39_sram_blwl_out[49:49] ,mux_1level_tapbuf_size2_39_sram_blwl_out[49:49] ,mux_1level_tapbuf_size2_39_sram_blwl_outb[49:49] ,mux_1level_tapbuf_size2_39_configbus0[49:49], mux_1level_tapbuf_size2_39_configbus1[49:49] , mux_1level_tapbuf_size2_39_configbus0_b[49:49] );
wire [0:1] mux_1level_tapbuf_size2_40_inbus;
assign mux_1level_tapbuf_size2_40_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_40_inbus[1] = chany_0__1__in_6_ ;
wire [50:50] mux_1level_tapbuf_size2_40_configbus0;
wire [50:50] mux_1level_tapbuf_size2_40_configbus1;
wire [50:50] mux_1level_tapbuf_size2_40_sram_blwl_out ;
wire [50:50] mux_1level_tapbuf_size2_40_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_40_configbus0[50:50] = sram_blwl_bl[50:50] ;
assign mux_1level_tapbuf_size2_40_configbus1[50:50] = sram_blwl_wl[50:50] ;
wire [50:50] mux_1level_tapbuf_size2_40_configbus0_b;
assign mux_1level_tapbuf_size2_40_configbus0_b[50:50] = sram_blwl_blb[50:50] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_40_ (mux_1level_tapbuf_size2_40_inbus, chanx_1__1__out_20_ , mux_1level_tapbuf_size2_40_sram_blwl_out[50:50] ,
mux_1level_tapbuf_size2_40_sram_blwl_outb[50:50] );
//----- SRAM bits for MUX[40], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_50_ (mux_1level_tapbuf_size2_40_sram_blwl_out[50:50] ,mux_1level_tapbuf_size2_40_sram_blwl_out[50:50] ,mux_1level_tapbuf_size2_40_sram_blwl_outb[50:50] ,mux_1level_tapbuf_size2_40_configbus0[50:50], mux_1level_tapbuf_size2_40_configbus1[50:50] , mux_1level_tapbuf_size2_40_configbus0_b[50:50] );
wire [0:1] mux_1level_tapbuf_size2_41_inbus;
assign mux_1level_tapbuf_size2_41_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_41_inbus[1] = chany_0__1__in_4_ ;
wire [51:51] mux_1level_tapbuf_size2_41_configbus0;
wire [51:51] mux_1level_tapbuf_size2_41_configbus1;
wire [51:51] mux_1level_tapbuf_size2_41_sram_blwl_out ;
wire [51:51] mux_1level_tapbuf_size2_41_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_41_configbus0[51:51] = sram_blwl_bl[51:51] ;
assign mux_1level_tapbuf_size2_41_configbus1[51:51] = sram_blwl_wl[51:51] ;
wire [51:51] mux_1level_tapbuf_size2_41_configbus0_b;
assign mux_1level_tapbuf_size2_41_configbus0_b[51:51] = sram_blwl_blb[51:51] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_41_ (mux_1level_tapbuf_size2_41_inbus, chanx_1__1__out_22_ , mux_1level_tapbuf_size2_41_sram_blwl_out[51:51] ,
mux_1level_tapbuf_size2_41_sram_blwl_outb[51:51] );
//----- SRAM bits for MUX[41], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_51_ (mux_1level_tapbuf_size2_41_sram_blwl_out[51:51] ,mux_1level_tapbuf_size2_41_sram_blwl_out[51:51] ,mux_1level_tapbuf_size2_41_sram_blwl_outb[51:51] ,mux_1level_tapbuf_size2_41_configbus0[51:51], mux_1level_tapbuf_size2_41_configbus1[51:51] , mux_1level_tapbuf_size2_41_configbus0_b[51:51] );
wire [0:1] mux_1level_tapbuf_size2_42_inbus;
assign mux_1level_tapbuf_size2_42_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_42_inbus[1] = chany_0__1__in_2_ ;
wire [52:52] mux_1level_tapbuf_size2_42_configbus0;
wire [52:52] mux_1level_tapbuf_size2_42_configbus1;
wire [52:52] mux_1level_tapbuf_size2_42_sram_blwl_out ;
wire [52:52] mux_1level_tapbuf_size2_42_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_42_configbus0[52:52] = sram_blwl_bl[52:52] ;
assign mux_1level_tapbuf_size2_42_configbus1[52:52] = sram_blwl_wl[52:52] ;
wire [52:52] mux_1level_tapbuf_size2_42_configbus0_b;
assign mux_1level_tapbuf_size2_42_configbus0_b[52:52] = sram_blwl_blb[52:52] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_42_ (mux_1level_tapbuf_size2_42_inbus, chanx_1__1__out_24_ , mux_1level_tapbuf_size2_42_sram_blwl_out[52:52] ,
mux_1level_tapbuf_size2_42_sram_blwl_outb[52:52] );
//----- SRAM bits for MUX[42], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_52_ (mux_1level_tapbuf_size2_42_sram_blwl_out[52:52] ,mux_1level_tapbuf_size2_42_sram_blwl_out[52:52] ,mux_1level_tapbuf_size2_42_sram_blwl_outb[52:52] ,mux_1level_tapbuf_size2_42_configbus0[52:52], mux_1level_tapbuf_size2_42_configbus1[52:52] , mux_1level_tapbuf_size2_42_configbus0_b[52:52] );
wire [0:1] mux_1level_tapbuf_size2_43_inbus;
assign mux_1level_tapbuf_size2_43_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_43_inbus[1] = chany_0__1__in_0_ ;
wire [53:53] mux_1level_tapbuf_size2_43_configbus0;
wire [53:53] mux_1level_tapbuf_size2_43_configbus1;
wire [53:53] mux_1level_tapbuf_size2_43_sram_blwl_out ;
wire [53:53] mux_1level_tapbuf_size2_43_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_43_configbus0[53:53] = sram_blwl_bl[53:53] ;
assign mux_1level_tapbuf_size2_43_configbus1[53:53] = sram_blwl_wl[53:53] ;
wire [53:53] mux_1level_tapbuf_size2_43_configbus0_b;
assign mux_1level_tapbuf_size2_43_configbus0_b[53:53] = sram_blwl_blb[53:53] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_43_ (mux_1level_tapbuf_size2_43_inbus, chanx_1__1__out_26_ , mux_1level_tapbuf_size2_43_sram_blwl_out[53:53] ,
mux_1level_tapbuf_size2_43_sram_blwl_outb[53:53] );
//----- SRAM bits for MUX[43], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_53_ (mux_1level_tapbuf_size2_43_sram_blwl_out[53:53] ,mux_1level_tapbuf_size2_43_sram_blwl_out[53:53] ,mux_1level_tapbuf_size2_43_sram_blwl_outb[53:53] ,mux_1level_tapbuf_size2_43_configbus0[53:53], mux_1level_tapbuf_size2_43_configbus1[53:53] , mux_1level_tapbuf_size2_43_configbus0_b[53:53] );
wire [0:1] mux_1level_tapbuf_size2_44_inbus;
assign mux_1level_tapbuf_size2_44_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_44_inbus[1] = chany_0__1__in_28_ ;
wire [54:54] mux_1level_tapbuf_size2_44_configbus0;
wire [54:54] mux_1level_tapbuf_size2_44_configbus1;
wire [54:54] mux_1level_tapbuf_size2_44_sram_blwl_out ;
wire [54:54] mux_1level_tapbuf_size2_44_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_44_configbus0[54:54] = sram_blwl_bl[54:54] ;
assign mux_1level_tapbuf_size2_44_configbus1[54:54] = sram_blwl_wl[54:54] ;
wire [54:54] mux_1level_tapbuf_size2_44_configbus0_b;
assign mux_1level_tapbuf_size2_44_configbus0_b[54:54] = sram_blwl_blb[54:54] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_44_ (mux_1level_tapbuf_size2_44_inbus, chanx_1__1__out_28_ , mux_1level_tapbuf_size2_44_sram_blwl_out[54:54] ,
mux_1level_tapbuf_size2_44_sram_blwl_outb[54:54] );
//----- SRAM bits for MUX[44], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_54_ (mux_1level_tapbuf_size2_44_sram_blwl_out[54:54] ,mux_1level_tapbuf_size2_44_sram_blwl_out[54:54] ,mux_1level_tapbuf_size2_44_sram_blwl_outb[54:54] ,mux_1level_tapbuf_size2_44_configbus0[54:54], mux_1level_tapbuf_size2_44_configbus1[54:54] , mux_1level_tapbuf_size2_44_configbus0_b[54:54] );
//----- bottom side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_45_inbus;
assign mux_1level_tapbuf_size3_45_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size3_45_inbus[1] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size3_45_inbus[2] = chanx_1__1__in_27_ ;
wire [55:57] mux_1level_tapbuf_size3_45_configbus0;
wire [55:57] mux_1level_tapbuf_size3_45_configbus1;
wire [55:57] mux_1level_tapbuf_size3_45_sram_blwl_out ;
wire [55:57] mux_1level_tapbuf_size3_45_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_45_configbus0[55:57] = sram_blwl_bl[55:57] ;
assign mux_1level_tapbuf_size3_45_configbus1[55:57] = sram_blwl_wl[55:57] ;
wire [55:57] mux_1level_tapbuf_size3_45_configbus0_b;
assign mux_1level_tapbuf_size3_45_configbus0_b[55:57] = sram_blwl_blb[55:57] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_45_ (mux_1level_tapbuf_size3_45_inbus, chany_0__1__out_1_ , mux_1level_tapbuf_size3_45_sram_blwl_out[55:57] ,
mux_1level_tapbuf_size3_45_sram_blwl_outb[55:57] );
//----- SRAM bits for MUX[45], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_55_ (mux_1level_tapbuf_size3_45_sram_blwl_out[55:55] ,mux_1level_tapbuf_size3_45_sram_blwl_out[55:55] ,mux_1level_tapbuf_size3_45_sram_blwl_outb[55:55] ,mux_1level_tapbuf_size3_45_configbus0[55:55], mux_1level_tapbuf_size3_45_configbus1[55:55] , mux_1level_tapbuf_size3_45_configbus0_b[55:55] );
sram6T_blwl sram_blwl_56_ (mux_1level_tapbuf_size3_45_sram_blwl_out[56:56] ,mux_1level_tapbuf_size3_45_sram_blwl_out[56:56] ,mux_1level_tapbuf_size3_45_sram_blwl_outb[56:56] ,mux_1level_tapbuf_size3_45_configbus0[56:56], mux_1level_tapbuf_size3_45_configbus1[56:56] , mux_1level_tapbuf_size3_45_configbus0_b[56:56] );
sram6T_blwl sram_blwl_57_ (mux_1level_tapbuf_size3_45_sram_blwl_out[57:57] ,mux_1level_tapbuf_size3_45_sram_blwl_out[57:57] ,mux_1level_tapbuf_size3_45_sram_blwl_outb[57:57] ,mux_1level_tapbuf_size3_45_configbus0[57:57], mux_1level_tapbuf_size3_45_configbus1[57:57] , mux_1level_tapbuf_size3_45_configbus0_b[57:57] );
wire [0:1] mux_1level_tapbuf_size2_46_inbus;
assign mux_1level_tapbuf_size2_46_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_46_inbus[1] = chanx_1__1__in_25_ ;
wire [58:58] mux_1level_tapbuf_size2_46_configbus0;
wire [58:58] mux_1level_tapbuf_size2_46_configbus1;
wire [58:58] mux_1level_tapbuf_size2_46_sram_blwl_out ;
wire [58:58] mux_1level_tapbuf_size2_46_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_46_configbus0[58:58] = sram_blwl_bl[58:58] ;
assign mux_1level_tapbuf_size2_46_configbus1[58:58] = sram_blwl_wl[58:58] ;
wire [58:58] mux_1level_tapbuf_size2_46_configbus0_b;
assign mux_1level_tapbuf_size2_46_configbus0_b[58:58] = sram_blwl_blb[58:58] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_46_ (mux_1level_tapbuf_size2_46_inbus, chany_0__1__out_3_ , mux_1level_tapbuf_size2_46_sram_blwl_out[58:58] ,
mux_1level_tapbuf_size2_46_sram_blwl_outb[58:58] );
//----- SRAM bits for MUX[46], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_58_ (mux_1level_tapbuf_size2_46_sram_blwl_out[58:58] ,mux_1level_tapbuf_size2_46_sram_blwl_out[58:58] ,mux_1level_tapbuf_size2_46_sram_blwl_outb[58:58] ,mux_1level_tapbuf_size2_46_configbus0[58:58], mux_1level_tapbuf_size2_46_configbus1[58:58] , mux_1level_tapbuf_size2_46_configbus0_b[58:58] );
wire [0:1] mux_1level_tapbuf_size2_47_inbus;
assign mux_1level_tapbuf_size2_47_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_47_inbus[1] = chanx_1__1__in_23_ ;
wire [59:59] mux_1level_tapbuf_size2_47_configbus0;
wire [59:59] mux_1level_tapbuf_size2_47_configbus1;
wire [59:59] mux_1level_tapbuf_size2_47_sram_blwl_out ;
wire [59:59] mux_1level_tapbuf_size2_47_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_47_configbus0[59:59] = sram_blwl_bl[59:59] ;
assign mux_1level_tapbuf_size2_47_configbus1[59:59] = sram_blwl_wl[59:59] ;
wire [59:59] mux_1level_tapbuf_size2_47_configbus0_b;
assign mux_1level_tapbuf_size2_47_configbus0_b[59:59] = sram_blwl_blb[59:59] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_47_ (mux_1level_tapbuf_size2_47_inbus, chany_0__1__out_5_ , mux_1level_tapbuf_size2_47_sram_blwl_out[59:59] ,
mux_1level_tapbuf_size2_47_sram_blwl_outb[59:59] );
//----- SRAM bits for MUX[47], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_59_ (mux_1level_tapbuf_size2_47_sram_blwl_out[59:59] ,mux_1level_tapbuf_size2_47_sram_blwl_out[59:59] ,mux_1level_tapbuf_size2_47_sram_blwl_outb[59:59] ,mux_1level_tapbuf_size2_47_configbus0[59:59], mux_1level_tapbuf_size2_47_configbus1[59:59] , mux_1level_tapbuf_size2_47_configbus0_b[59:59] );
wire [0:1] mux_1level_tapbuf_size2_48_inbus;
assign mux_1level_tapbuf_size2_48_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_48_inbus[1] = chanx_1__1__in_21_ ;
wire [60:60] mux_1level_tapbuf_size2_48_configbus0;
wire [60:60] mux_1level_tapbuf_size2_48_configbus1;
wire [60:60] mux_1level_tapbuf_size2_48_sram_blwl_out ;
wire [60:60] mux_1level_tapbuf_size2_48_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_48_configbus0[60:60] = sram_blwl_bl[60:60] ;
assign mux_1level_tapbuf_size2_48_configbus1[60:60] = sram_blwl_wl[60:60] ;
wire [60:60] mux_1level_tapbuf_size2_48_configbus0_b;
assign mux_1level_tapbuf_size2_48_configbus0_b[60:60] = sram_blwl_blb[60:60] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_48_ (mux_1level_tapbuf_size2_48_inbus, chany_0__1__out_7_ , mux_1level_tapbuf_size2_48_sram_blwl_out[60:60] ,
mux_1level_tapbuf_size2_48_sram_blwl_outb[60:60] );
//----- SRAM bits for MUX[48], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_60_ (mux_1level_tapbuf_size2_48_sram_blwl_out[60:60] ,mux_1level_tapbuf_size2_48_sram_blwl_out[60:60] ,mux_1level_tapbuf_size2_48_sram_blwl_outb[60:60] ,mux_1level_tapbuf_size2_48_configbus0[60:60], mux_1level_tapbuf_size2_48_configbus1[60:60] , mux_1level_tapbuf_size2_48_configbus0_b[60:60] );
wire [0:1] mux_1level_tapbuf_size2_49_inbus;
assign mux_1level_tapbuf_size2_49_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_49_inbus[1] = chanx_1__1__in_19_ ;
wire [61:61] mux_1level_tapbuf_size2_49_configbus0;
wire [61:61] mux_1level_tapbuf_size2_49_configbus1;
wire [61:61] mux_1level_tapbuf_size2_49_sram_blwl_out ;
wire [61:61] mux_1level_tapbuf_size2_49_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_49_configbus0[61:61] = sram_blwl_bl[61:61] ;
assign mux_1level_tapbuf_size2_49_configbus1[61:61] = sram_blwl_wl[61:61] ;
wire [61:61] mux_1level_tapbuf_size2_49_configbus0_b;
assign mux_1level_tapbuf_size2_49_configbus0_b[61:61] = sram_blwl_blb[61:61] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_49_ (mux_1level_tapbuf_size2_49_inbus, chany_0__1__out_9_ , mux_1level_tapbuf_size2_49_sram_blwl_out[61:61] ,
mux_1level_tapbuf_size2_49_sram_blwl_outb[61:61] );
//----- SRAM bits for MUX[49], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_61_ (mux_1level_tapbuf_size2_49_sram_blwl_out[61:61] ,mux_1level_tapbuf_size2_49_sram_blwl_out[61:61] ,mux_1level_tapbuf_size2_49_sram_blwl_outb[61:61] ,mux_1level_tapbuf_size2_49_configbus0[61:61], mux_1level_tapbuf_size2_49_configbus1[61:61] , mux_1level_tapbuf_size2_49_configbus0_b[61:61] );
wire [0:1] mux_1level_tapbuf_size2_50_inbus;
assign mux_1level_tapbuf_size2_50_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_50_inbus[1] = chanx_1__1__in_17_ ;
wire [62:62] mux_1level_tapbuf_size2_50_configbus0;
wire [62:62] mux_1level_tapbuf_size2_50_configbus1;
wire [62:62] mux_1level_tapbuf_size2_50_sram_blwl_out ;
wire [62:62] mux_1level_tapbuf_size2_50_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_50_configbus0[62:62] = sram_blwl_bl[62:62] ;
assign mux_1level_tapbuf_size2_50_configbus1[62:62] = sram_blwl_wl[62:62] ;
wire [62:62] mux_1level_tapbuf_size2_50_configbus0_b;
assign mux_1level_tapbuf_size2_50_configbus0_b[62:62] = sram_blwl_blb[62:62] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_50_ (mux_1level_tapbuf_size2_50_inbus, chany_0__1__out_11_ , mux_1level_tapbuf_size2_50_sram_blwl_out[62:62] ,
mux_1level_tapbuf_size2_50_sram_blwl_outb[62:62] );
//----- SRAM bits for MUX[50], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_62_ (mux_1level_tapbuf_size2_50_sram_blwl_out[62:62] ,mux_1level_tapbuf_size2_50_sram_blwl_out[62:62] ,mux_1level_tapbuf_size2_50_sram_blwl_outb[62:62] ,mux_1level_tapbuf_size2_50_configbus0[62:62], mux_1level_tapbuf_size2_50_configbus1[62:62] , mux_1level_tapbuf_size2_50_configbus0_b[62:62] );
wire [0:1] mux_1level_tapbuf_size2_51_inbus;
assign mux_1level_tapbuf_size2_51_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_51_inbus[1] = chanx_1__1__in_15_ ;
wire [63:63] mux_1level_tapbuf_size2_51_configbus0;
wire [63:63] mux_1level_tapbuf_size2_51_configbus1;
wire [63:63] mux_1level_tapbuf_size2_51_sram_blwl_out ;
wire [63:63] mux_1level_tapbuf_size2_51_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_51_configbus0[63:63] = sram_blwl_bl[63:63] ;
assign mux_1level_tapbuf_size2_51_configbus1[63:63] = sram_blwl_wl[63:63] ;
wire [63:63] mux_1level_tapbuf_size2_51_configbus0_b;
assign mux_1level_tapbuf_size2_51_configbus0_b[63:63] = sram_blwl_blb[63:63] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_51_ (mux_1level_tapbuf_size2_51_inbus, chany_0__1__out_13_ , mux_1level_tapbuf_size2_51_sram_blwl_out[63:63] ,
mux_1level_tapbuf_size2_51_sram_blwl_outb[63:63] );
//----- SRAM bits for MUX[51], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_63_ (mux_1level_tapbuf_size2_51_sram_blwl_out[63:63] ,mux_1level_tapbuf_size2_51_sram_blwl_out[63:63] ,mux_1level_tapbuf_size2_51_sram_blwl_outb[63:63] ,mux_1level_tapbuf_size2_51_configbus0[63:63], mux_1level_tapbuf_size2_51_configbus1[63:63] , mux_1level_tapbuf_size2_51_configbus0_b[63:63] );
wire [0:1] mux_1level_tapbuf_size2_52_inbus;
assign mux_1level_tapbuf_size2_52_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_52_inbus[1] = chanx_1__1__in_13_ ;
wire [64:64] mux_1level_tapbuf_size2_52_configbus0;
wire [64:64] mux_1level_tapbuf_size2_52_configbus1;
wire [64:64] mux_1level_tapbuf_size2_52_sram_blwl_out ;
wire [64:64] mux_1level_tapbuf_size2_52_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_52_configbus0[64:64] = sram_blwl_bl[64:64] ;
assign mux_1level_tapbuf_size2_52_configbus1[64:64] = sram_blwl_wl[64:64] ;
wire [64:64] mux_1level_tapbuf_size2_52_configbus0_b;
assign mux_1level_tapbuf_size2_52_configbus0_b[64:64] = sram_blwl_blb[64:64] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_52_ (mux_1level_tapbuf_size2_52_inbus, chany_0__1__out_15_ , mux_1level_tapbuf_size2_52_sram_blwl_out[64:64] ,
mux_1level_tapbuf_size2_52_sram_blwl_outb[64:64] );
//----- SRAM bits for MUX[52], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_64_ (mux_1level_tapbuf_size2_52_sram_blwl_out[64:64] ,mux_1level_tapbuf_size2_52_sram_blwl_out[64:64] ,mux_1level_tapbuf_size2_52_sram_blwl_outb[64:64] ,mux_1level_tapbuf_size2_52_configbus0[64:64], mux_1level_tapbuf_size2_52_configbus1[64:64] , mux_1level_tapbuf_size2_52_configbus0_b[64:64] );
wire [0:1] mux_1level_tapbuf_size2_53_inbus;
assign mux_1level_tapbuf_size2_53_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_53_inbus[1] = chanx_1__1__in_11_ ;
wire [65:65] mux_1level_tapbuf_size2_53_configbus0;
wire [65:65] mux_1level_tapbuf_size2_53_configbus1;
wire [65:65] mux_1level_tapbuf_size2_53_sram_blwl_out ;
wire [65:65] mux_1level_tapbuf_size2_53_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_53_configbus0[65:65] = sram_blwl_bl[65:65] ;
assign mux_1level_tapbuf_size2_53_configbus1[65:65] = sram_blwl_wl[65:65] ;
wire [65:65] mux_1level_tapbuf_size2_53_configbus0_b;
assign mux_1level_tapbuf_size2_53_configbus0_b[65:65] = sram_blwl_blb[65:65] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_53_ (mux_1level_tapbuf_size2_53_inbus, chany_0__1__out_17_ , mux_1level_tapbuf_size2_53_sram_blwl_out[65:65] ,
mux_1level_tapbuf_size2_53_sram_blwl_outb[65:65] );
//----- SRAM bits for MUX[53], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_65_ (mux_1level_tapbuf_size2_53_sram_blwl_out[65:65] ,mux_1level_tapbuf_size2_53_sram_blwl_out[65:65] ,mux_1level_tapbuf_size2_53_sram_blwl_outb[65:65] ,mux_1level_tapbuf_size2_53_configbus0[65:65], mux_1level_tapbuf_size2_53_configbus1[65:65] , mux_1level_tapbuf_size2_53_configbus0_b[65:65] );
wire [0:1] mux_1level_tapbuf_size2_54_inbus;
assign mux_1level_tapbuf_size2_54_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_54_inbus[1] = chanx_1__1__in_9_ ;
wire [66:66] mux_1level_tapbuf_size2_54_configbus0;
wire [66:66] mux_1level_tapbuf_size2_54_configbus1;
wire [66:66] mux_1level_tapbuf_size2_54_sram_blwl_out ;
wire [66:66] mux_1level_tapbuf_size2_54_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_54_configbus0[66:66] = sram_blwl_bl[66:66] ;
assign mux_1level_tapbuf_size2_54_configbus1[66:66] = sram_blwl_wl[66:66] ;
wire [66:66] mux_1level_tapbuf_size2_54_configbus0_b;
assign mux_1level_tapbuf_size2_54_configbus0_b[66:66] = sram_blwl_blb[66:66] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_54_ (mux_1level_tapbuf_size2_54_inbus, chany_0__1__out_19_ , mux_1level_tapbuf_size2_54_sram_blwl_out[66:66] ,
mux_1level_tapbuf_size2_54_sram_blwl_outb[66:66] );
//----- SRAM bits for MUX[54], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_66_ (mux_1level_tapbuf_size2_54_sram_blwl_out[66:66] ,mux_1level_tapbuf_size2_54_sram_blwl_out[66:66] ,mux_1level_tapbuf_size2_54_sram_blwl_outb[66:66] ,mux_1level_tapbuf_size2_54_configbus0[66:66], mux_1level_tapbuf_size2_54_configbus1[66:66] , mux_1level_tapbuf_size2_54_configbus0_b[66:66] );
wire [0:1] mux_1level_tapbuf_size2_55_inbus;
assign mux_1level_tapbuf_size2_55_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_55_inbus[1] = chanx_1__1__in_7_ ;
wire [67:67] mux_1level_tapbuf_size2_55_configbus0;
wire [67:67] mux_1level_tapbuf_size2_55_configbus1;
wire [67:67] mux_1level_tapbuf_size2_55_sram_blwl_out ;
wire [67:67] mux_1level_tapbuf_size2_55_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_55_configbus0[67:67] = sram_blwl_bl[67:67] ;
assign mux_1level_tapbuf_size2_55_configbus1[67:67] = sram_blwl_wl[67:67] ;
wire [67:67] mux_1level_tapbuf_size2_55_configbus0_b;
assign mux_1level_tapbuf_size2_55_configbus0_b[67:67] = sram_blwl_blb[67:67] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_55_ (mux_1level_tapbuf_size2_55_inbus, chany_0__1__out_21_ , mux_1level_tapbuf_size2_55_sram_blwl_out[67:67] ,
mux_1level_tapbuf_size2_55_sram_blwl_outb[67:67] );
//----- SRAM bits for MUX[55], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_67_ (mux_1level_tapbuf_size2_55_sram_blwl_out[67:67] ,mux_1level_tapbuf_size2_55_sram_blwl_out[67:67] ,mux_1level_tapbuf_size2_55_sram_blwl_outb[67:67] ,mux_1level_tapbuf_size2_55_configbus0[67:67], mux_1level_tapbuf_size2_55_configbus1[67:67] , mux_1level_tapbuf_size2_55_configbus0_b[67:67] );
wire [0:1] mux_1level_tapbuf_size2_56_inbus;
assign mux_1level_tapbuf_size2_56_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_56_inbus[1] = chanx_1__1__in_5_ ;
wire [68:68] mux_1level_tapbuf_size2_56_configbus0;
wire [68:68] mux_1level_tapbuf_size2_56_configbus1;
wire [68:68] mux_1level_tapbuf_size2_56_sram_blwl_out ;
wire [68:68] mux_1level_tapbuf_size2_56_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_56_configbus0[68:68] = sram_blwl_bl[68:68] ;
assign mux_1level_tapbuf_size2_56_configbus1[68:68] = sram_blwl_wl[68:68] ;
wire [68:68] mux_1level_tapbuf_size2_56_configbus0_b;
assign mux_1level_tapbuf_size2_56_configbus0_b[68:68] = sram_blwl_blb[68:68] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_56_ (mux_1level_tapbuf_size2_56_inbus, chany_0__1__out_23_ , mux_1level_tapbuf_size2_56_sram_blwl_out[68:68] ,
mux_1level_tapbuf_size2_56_sram_blwl_outb[68:68] );
//----- SRAM bits for MUX[56], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_68_ (mux_1level_tapbuf_size2_56_sram_blwl_out[68:68] ,mux_1level_tapbuf_size2_56_sram_blwl_out[68:68] ,mux_1level_tapbuf_size2_56_sram_blwl_outb[68:68] ,mux_1level_tapbuf_size2_56_configbus0[68:68], mux_1level_tapbuf_size2_56_configbus1[68:68] , mux_1level_tapbuf_size2_56_configbus0_b[68:68] );
wire [0:1] mux_1level_tapbuf_size2_57_inbus;
assign mux_1level_tapbuf_size2_57_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_57_inbus[1] = chanx_1__1__in_3_ ;
wire [69:69] mux_1level_tapbuf_size2_57_configbus0;
wire [69:69] mux_1level_tapbuf_size2_57_configbus1;
wire [69:69] mux_1level_tapbuf_size2_57_sram_blwl_out ;
wire [69:69] mux_1level_tapbuf_size2_57_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_57_configbus0[69:69] = sram_blwl_bl[69:69] ;
assign mux_1level_tapbuf_size2_57_configbus1[69:69] = sram_blwl_wl[69:69] ;
wire [69:69] mux_1level_tapbuf_size2_57_configbus0_b;
assign mux_1level_tapbuf_size2_57_configbus0_b[69:69] = sram_blwl_blb[69:69] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_57_ (mux_1level_tapbuf_size2_57_inbus, chany_0__1__out_25_ , mux_1level_tapbuf_size2_57_sram_blwl_out[69:69] ,
mux_1level_tapbuf_size2_57_sram_blwl_outb[69:69] );
//----- SRAM bits for MUX[57], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_69_ (mux_1level_tapbuf_size2_57_sram_blwl_out[69:69] ,mux_1level_tapbuf_size2_57_sram_blwl_out[69:69] ,mux_1level_tapbuf_size2_57_sram_blwl_outb[69:69] ,mux_1level_tapbuf_size2_57_configbus0[69:69], mux_1level_tapbuf_size2_57_configbus1[69:69] , mux_1level_tapbuf_size2_57_configbus0_b[69:69] );
wire [0:1] mux_1level_tapbuf_size2_58_inbus;
assign mux_1level_tapbuf_size2_58_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_58_inbus[1] = chanx_1__1__in_1_ ;
wire [70:70] mux_1level_tapbuf_size2_58_configbus0;
wire [70:70] mux_1level_tapbuf_size2_58_configbus1;
wire [70:70] mux_1level_tapbuf_size2_58_sram_blwl_out ;
wire [70:70] mux_1level_tapbuf_size2_58_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_58_configbus0[70:70] = sram_blwl_bl[70:70] ;
assign mux_1level_tapbuf_size2_58_configbus1[70:70] = sram_blwl_wl[70:70] ;
wire [70:70] mux_1level_tapbuf_size2_58_configbus0_b;
assign mux_1level_tapbuf_size2_58_configbus0_b[70:70] = sram_blwl_blb[70:70] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_58_ (mux_1level_tapbuf_size2_58_inbus, chany_0__1__out_27_ , mux_1level_tapbuf_size2_58_sram_blwl_out[70:70] ,
mux_1level_tapbuf_size2_58_sram_blwl_outb[70:70] );
//----- SRAM bits for MUX[58], level=1, select_path_id=1. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----0-----
sram6T_blwl sram_blwl_70_ (mux_1level_tapbuf_size2_58_sram_blwl_out[70:70] ,mux_1level_tapbuf_size2_58_sram_blwl_out[70:70] ,mux_1level_tapbuf_size2_58_sram_blwl_outb[70:70] ,mux_1level_tapbuf_size2_58_configbus0[70:70], mux_1level_tapbuf_size2_58_configbus1[70:70] , mux_1level_tapbuf_size2_58_configbus0_b[70:70] );
wire [0:1] mux_1level_tapbuf_size2_59_inbus;
assign mux_1level_tapbuf_size2_59_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_59_inbus[1] = chanx_1__1__in_29_ ;
wire [71:71] mux_1level_tapbuf_size2_59_configbus0;
wire [71:71] mux_1level_tapbuf_size2_59_configbus1;
wire [71:71] mux_1level_tapbuf_size2_59_sram_blwl_out ;
wire [71:71] mux_1level_tapbuf_size2_59_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_59_configbus0[71:71] = sram_blwl_bl[71:71] ;
assign mux_1level_tapbuf_size2_59_configbus1[71:71] = sram_blwl_wl[71:71] ;
wire [71:71] mux_1level_tapbuf_size2_59_configbus0_b;
assign mux_1level_tapbuf_size2_59_configbus0_b[71:71] = sram_blwl_blb[71:71] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_59_ (mux_1level_tapbuf_size2_59_inbus, chany_0__1__out_29_ , mux_1level_tapbuf_size2_59_sram_blwl_out[71:71] ,
mux_1level_tapbuf_size2_59_sram_blwl_outb[71:71] );
//----- SRAM bits for MUX[59], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_71_ (mux_1level_tapbuf_size2_59_sram_blwl_out[71:71] ,mux_1level_tapbuf_size2_59_sram_blwl_out[71:71] ,mux_1level_tapbuf_size2_59_sram_blwl_outb[71:71] ,mux_1level_tapbuf_size2_59_configbus0[71:71], mux_1level_tapbuf_size2_59_configbus1[71:71] , mux_1level_tapbuf_size2_59_configbus0_b[71:71] );
//----- left side Multiplexers -----
endmodule
//----- END Verilog Module of Switch Box[0][1] -----

