//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Switch Block  [1][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Verilog Module of Switch Box[1][1] -----
module sb_1__1_ ( 

//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
//----- Inputs/outputs of top side -----
//----- Inputs/outputs of right side -----
//----- Inputs/outputs of bottom side -----
  input chany_1__1__in_0_,
  output chany_1__1__out_1_,
  input chany_1__1__in_2_,
  output chany_1__1__out_3_,
  input chany_1__1__in_4_,
  output chany_1__1__out_5_,
  input chany_1__1__in_6_,
  output chany_1__1__out_7_,
  input chany_1__1__in_8_,
  output chany_1__1__out_9_,
  input chany_1__1__in_10_,
  output chany_1__1__out_11_,
  input chany_1__1__in_12_,
  output chany_1__1__out_13_,
  input chany_1__1__in_14_,
  output chany_1__1__out_15_,
  input chany_1__1__in_16_,
  output chany_1__1__out_17_,
  input chany_1__1__in_18_,
  output chany_1__1__out_19_,
  input chany_1__1__in_20_,
  output chany_1__1__out_21_,
  input chany_1__1__in_22_,
  output chany_1__1__out_23_,
  input chany_1__1__in_24_,
  output chany_1__1__out_25_,
  input chany_1__1__in_26_,
  output chany_1__1__out_27_,
  input chany_1__1__in_28_,
  output chany_1__1__out_29_,
input  grid_2__1__pin_0__3__1_,
input  grid_2__1__pin_0__3__3_,
input  grid_2__1__pin_0__3__5_,
input  grid_2__1__pin_0__3__7_,
input  grid_2__1__pin_0__3__9_,
input  grid_2__1__pin_0__3__11_,
input  grid_2__1__pin_0__3__13_,
input  grid_2__1__pin_0__3__15_,
//----- Inputs/outputs of left side -----
  input chanx_1__1__in_0_,
  output chanx_1__1__out_1_,
  input chanx_1__1__in_2_,
  output chanx_1__1__out_3_,
  input chanx_1__1__in_4_,
  output chanx_1__1__out_5_,
  input chanx_1__1__in_6_,
  output chanx_1__1__out_7_,
  input chanx_1__1__in_8_,
  output chanx_1__1__out_9_,
  input chanx_1__1__in_10_,
  output chanx_1__1__out_11_,
  input chanx_1__1__in_12_,
  output chanx_1__1__out_13_,
  input chanx_1__1__in_14_,
  output chanx_1__1__out_15_,
  input chanx_1__1__in_16_,
  output chanx_1__1__out_17_,
  input chanx_1__1__in_18_,
  output chanx_1__1__out_19_,
  input chanx_1__1__in_20_,
  output chanx_1__1__out_21_,
  input chanx_1__1__in_22_,
  output chanx_1__1__out_23_,
  input chanx_1__1__in_24_,
  output chanx_1__1__out_25_,
  input chanx_1__1__in_26_,
  output chanx_1__1__out_27_,
  input chanx_1__1__in_28_,
  output chanx_1__1__out_29_,
input  grid_1__2__pin_0__2__1_,
input  grid_1__2__pin_0__2__3_,
input  grid_1__2__pin_0__2__5_,
input  grid_1__2__pin_0__2__7_,
input  grid_1__2__pin_0__2__9_,
input  grid_1__2__pin_0__2__11_,
input  grid_1__2__pin_0__2__13_,
input  grid_1__2__pin_0__2__15_,
input  grid_1__1__pin_0__0__4_,
input [106:143] sram_blwl_bl ,
input [106:143] sram_blwl_wl ,
input [106:143] sram_blwl_blb ); 
//----- top side Multiplexers -----
//----- right side Multiplexers -----
//----- bottom side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_90_inbus;
assign mux_1level_tapbuf_size3_90_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size3_90_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_90_inbus[2] = chanx_1__1__in_2_ ;
wire [106:108] mux_1level_tapbuf_size3_90_configbus0;
wire [106:108] mux_1level_tapbuf_size3_90_configbus1;
wire [106:108] mux_1level_tapbuf_size3_90_sram_blwl_out ;
wire [106:108] mux_1level_tapbuf_size3_90_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_90_configbus0[106:108] = sram_blwl_bl[106:108] ;
assign mux_1level_tapbuf_size3_90_configbus1[106:108] = sram_blwl_wl[106:108] ;
wire [106:108] mux_1level_tapbuf_size3_90_configbus0_b;
assign mux_1level_tapbuf_size3_90_configbus0_b[106:108] = sram_blwl_blb[106:108] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_90_ (mux_1level_tapbuf_size3_90_inbus, chany_1__1__out_1_ , mux_1level_tapbuf_size3_90_sram_blwl_out[106:108] ,
mux_1level_tapbuf_size3_90_sram_blwl_outb[106:108] );
//----- SRAM bits for MUX[90], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_106_ (mux_1level_tapbuf_size3_90_sram_blwl_out[106:106] ,mux_1level_tapbuf_size3_90_sram_blwl_out[106:106] ,mux_1level_tapbuf_size3_90_sram_blwl_outb[106:106] ,mux_1level_tapbuf_size3_90_configbus0[106:106], mux_1level_tapbuf_size3_90_configbus1[106:106] , mux_1level_tapbuf_size3_90_configbus0_b[106:106] );
sram6T_blwl sram_blwl_107_ (mux_1level_tapbuf_size3_90_sram_blwl_out[107:107] ,mux_1level_tapbuf_size3_90_sram_blwl_out[107:107] ,mux_1level_tapbuf_size3_90_sram_blwl_outb[107:107] ,mux_1level_tapbuf_size3_90_configbus0[107:107], mux_1level_tapbuf_size3_90_configbus1[107:107] , mux_1level_tapbuf_size3_90_configbus0_b[107:107] );
sram6T_blwl sram_blwl_108_ (mux_1level_tapbuf_size3_90_sram_blwl_out[108:108] ,mux_1level_tapbuf_size3_90_sram_blwl_out[108:108] ,mux_1level_tapbuf_size3_90_sram_blwl_outb[108:108] ,mux_1level_tapbuf_size3_90_configbus0[108:108], mux_1level_tapbuf_size3_90_configbus1[108:108] , mux_1level_tapbuf_size3_90_configbus0_b[108:108] );
wire [0:1] mux_1level_tapbuf_size2_91_inbus;
assign mux_1level_tapbuf_size2_91_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_91_inbus[1] = chanx_1__1__in_4_ ;
wire [109:109] mux_1level_tapbuf_size2_91_configbus0;
wire [109:109] mux_1level_tapbuf_size2_91_configbus1;
wire [109:109] mux_1level_tapbuf_size2_91_sram_blwl_out ;
wire [109:109] mux_1level_tapbuf_size2_91_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_91_configbus0[109:109] = sram_blwl_bl[109:109] ;
assign mux_1level_tapbuf_size2_91_configbus1[109:109] = sram_blwl_wl[109:109] ;
wire [109:109] mux_1level_tapbuf_size2_91_configbus0_b;
assign mux_1level_tapbuf_size2_91_configbus0_b[109:109] = sram_blwl_blb[109:109] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_91_ (mux_1level_tapbuf_size2_91_inbus, chany_1__1__out_3_ , mux_1level_tapbuf_size2_91_sram_blwl_out[109:109] ,
mux_1level_tapbuf_size2_91_sram_blwl_outb[109:109] );
//----- SRAM bits for MUX[91], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_109_ (mux_1level_tapbuf_size2_91_sram_blwl_out[109:109] ,mux_1level_tapbuf_size2_91_sram_blwl_out[109:109] ,mux_1level_tapbuf_size2_91_sram_blwl_outb[109:109] ,mux_1level_tapbuf_size2_91_configbus0[109:109], mux_1level_tapbuf_size2_91_configbus1[109:109] , mux_1level_tapbuf_size2_91_configbus0_b[109:109] );
wire [0:1] mux_1level_tapbuf_size2_92_inbus;
assign mux_1level_tapbuf_size2_92_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_92_inbus[1] = chanx_1__1__in_6_ ;
wire [110:110] mux_1level_tapbuf_size2_92_configbus0;
wire [110:110] mux_1level_tapbuf_size2_92_configbus1;
wire [110:110] mux_1level_tapbuf_size2_92_sram_blwl_out ;
wire [110:110] mux_1level_tapbuf_size2_92_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_92_configbus0[110:110] = sram_blwl_bl[110:110] ;
assign mux_1level_tapbuf_size2_92_configbus1[110:110] = sram_blwl_wl[110:110] ;
wire [110:110] mux_1level_tapbuf_size2_92_configbus0_b;
assign mux_1level_tapbuf_size2_92_configbus0_b[110:110] = sram_blwl_blb[110:110] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_92_ (mux_1level_tapbuf_size2_92_inbus, chany_1__1__out_5_ , mux_1level_tapbuf_size2_92_sram_blwl_out[110:110] ,
mux_1level_tapbuf_size2_92_sram_blwl_outb[110:110] );
//----- SRAM bits for MUX[92], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_110_ (mux_1level_tapbuf_size2_92_sram_blwl_out[110:110] ,mux_1level_tapbuf_size2_92_sram_blwl_out[110:110] ,mux_1level_tapbuf_size2_92_sram_blwl_outb[110:110] ,mux_1level_tapbuf_size2_92_configbus0[110:110], mux_1level_tapbuf_size2_92_configbus1[110:110] , mux_1level_tapbuf_size2_92_configbus0_b[110:110] );
wire [0:1] mux_1level_tapbuf_size2_93_inbus;
assign mux_1level_tapbuf_size2_93_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_93_inbus[1] = chanx_1__1__in_8_ ;
wire [111:111] mux_1level_tapbuf_size2_93_configbus0;
wire [111:111] mux_1level_tapbuf_size2_93_configbus1;
wire [111:111] mux_1level_tapbuf_size2_93_sram_blwl_out ;
wire [111:111] mux_1level_tapbuf_size2_93_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_93_configbus0[111:111] = sram_blwl_bl[111:111] ;
assign mux_1level_tapbuf_size2_93_configbus1[111:111] = sram_blwl_wl[111:111] ;
wire [111:111] mux_1level_tapbuf_size2_93_configbus0_b;
assign mux_1level_tapbuf_size2_93_configbus0_b[111:111] = sram_blwl_blb[111:111] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_93_ (mux_1level_tapbuf_size2_93_inbus, chany_1__1__out_7_ , mux_1level_tapbuf_size2_93_sram_blwl_out[111:111] ,
mux_1level_tapbuf_size2_93_sram_blwl_outb[111:111] );
//----- SRAM bits for MUX[93], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_111_ (mux_1level_tapbuf_size2_93_sram_blwl_out[111:111] ,mux_1level_tapbuf_size2_93_sram_blwl_out[111:111] ,mux_1level_tapbuf_size2_93_sram_blwl_outb[111:111] ,mux_1level_tapbuf_size2_93_configbus0[111:111], mux_1level_tapbuf_size2_93_configbus1[111:111] , mux_1level_tapbuf_size2_93_configbus0_b[111:111] );
wire [0:1] mux_1level_tapbuf_size2_94_inbus;
assign mux_1level_tapbuf_size2_94_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_94_inbus[1] = chanx_1__1__in_10_ ;
wire [112:112] mux_1level_tapbuf_size2_94_configbus0;
wire [112:112] mux_1level_tapbuf_size2_94_configbus1;
wire [112:112] mux_1level_tapbuf_size2_94_sram_blwl_out ;
wire [112:112] mux_1level_tapbuf_size2_94_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_94_configbus0[112:112] = sram_blwl_bl[112:112] ;
assign mux_1level_tapbuf_size2_94_configbus1[112:112] = sram_blwl_wl[112:112] ;
wire [112:112] mux_1level_tapbuf_size2_94_configbus0_b;
assign mux_1level_tapbuf_size2_94_configbus0_b[112:112] = sram_blwl_blb[112:112] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_94_ (mux_1level_tapbuf_size2_94_inbus, chany_1__1__out_9_ , mux_1level_tapbuf_size2_94_sram_blwl_out[112:112] ,
mux_1level_tapbuf_size2_94_sram_blwl_outb[112:112] );
//----- SRAM bits for MUX[94], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_112_ (mux_1level_tapbuf_size2_94_sram_blwl_out[112:112] ,mux_1level_tapbuf_size2_94_sram_blwl_out[112:112] ,mux_1level_tapbuf_size2_94_sram_blwl_outb[112:112] ,mux_1level_tapbuf_size2_94_configbus0[112:112], mux_1level_tapbuf_size2_94_configbus1[112:112] , mux_1level_tapbuf_size2_94_configbus0_b[112:112] );
wire [0:1] mux_1level_tapbuf_size2_95_inbus;
assign mux_1level_tapbuf_size2_95_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_95_inbus[1] = chanx_1__1__in_12_ ;
wire [113:113] mux_1level_tapbuf_size2_95_configbus0;
wire [113:113] mux_1level_tapbuf_size2_95_configbus1;
wire [113:113] mux_1level_tapbuf_size2_95_sram_blwl_out ;
wire [113:113] mux_1level_tapbuf_size2_95_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_95_configbus0[113:113] = sram_blwl_bl[113:113] ;
assign mux_1level_tapbuf_size2_95_configbus1[113:113] = sram_blwl_wl[113:113] ;
wire [113:113] mux_1level_tapbuf_size2_95_configbus0_b;
assign mux_1level_tapbuf_size2_95_configbus0_b[113:113] = sram_blwl_blb[113:113] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_95_ (mux_1level_tapbuf_size2_95_inbus, chany_1__1__out_11_ , mux_1level_tapbuf_size2_95_sram_blwl_out[113:113] ,
mux_1level_tapbuf_size2_95_sram_blwl_outb[113:113] );
//----- SRAM bits for MUX[95], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_113_ (mux_1level_tapbuf_size2_95_sram_blwl_out[113:113] ,mux_1level_tapbuf_size2_95_sram_blwl_out[113:113] ,mux_1level_tapbuf_size2_95_sram_blwl_outb[113:113] ,mux_1level_tapbuf_size2_95_configbus0[113:113], mux_1level_tapbuf_size2_95_configbus1[113:113] , mux_1level_tapbuf_size2_95_configbus0_b[113:113] );
wire [0:1] mux_1level_tapbuf_size2_96_inbus;
assign mux_1level_tapbuf_size2_96_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_96_inbus[1] = chanx_1__1__in_14_ ;
wire [114:114] mux_1level_tapbuf_size2_96_configbus0;
wire [114:114] mux_1level_tapbuf_size2_96_configbus1;
wire [114:114] mux_1level_tapbuf_size2_96_sram_blwl_out ;
wire [114:114] mux_1level_tapbuf_size2_96_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_96_configbus0[114:114] = sram_blwl_bl[114:114] ;
assign mux_1level_tapbuf_size2_96_configbus1[114:114] = sram_blwl_wl[114:114] ;
wire [114:114] mux_1level_tapbuf_size2_96_configbus0_b;
assign mux_1level_tapbuf_size2_96_configbus0_b[114:114] = sram_blwl_blb[114:114] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_96_ (mux_1level_tapbuf_size2_96_inbus, chany_1__1__out_13_ , mux_1level_tapbuf_size2_96_sram_blwl_out[114:114] ,
mux_1level_tapbuf_size2_96_sram_blwl_outb[114:114] );
//----- SRAM bits for MUX[96], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_114_ (mux_1level_tapbuf_size2_96_sram_blwl_out[114:114] ,mux_1level_tapbuf_size2_96_sram_blwl_out[114:114] ,mux_1level_tapbuf_size2_96_sram_blwl_outb[114:114] ,mux_1level_tapbuf_size2_96_configbus0[114:114], mux_1level_tapbuf_size2_96_configbus1[114:114] , mux_1level_tapbuf_size2_96_configbus0_b[114:114] );
wire [0:1] mux_1level_tapbuf_size2_97_inbus;
assign mux_1level_tapbuf_size2_97_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_97_inbus[1] = chanx_1__1__in_16_ ;
wire [115:115] mux_1level_tapbuf_size2_97_configbus0;
wire [115:115] mux_1level_tapbuf_size2_97_configbus1;
wire [115:115] mux_1level_tapbuf_size2_97_sram_blwl_out ;
wire [115:115] mux_1level_tapbuf_size2_97_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_97_configbus0[115:115] = sram_blwl_bl[115:115] ;
assign mux_1level_tapbuf_size2_97_configbus1[115:115] = sram_blwl_wl[115:115] ;
wire [115:115] mux_1level_tapbuf_size2_97_configbus0_b;
assign mux_1level_tapbuf_size2_97_configbus0_b[115:115] = sram_blwl_blb[115:115] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_97_ (mux_1level_tapbuf_size2_97_inbus, chany_1__1__out_15_ , mux_1level_tapbuf_size2_97_sram_blwl_out[115:115] ,
mux_1level_tapbuf_size2_97_sram_blwl_outb[115:115] );
//----- SRAM bits for MUX[97], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_115_ (mux_1level_tapbuf_size2_97_sram_blwl_out[115:115] ,mux_1level_tapbuf_size2_97_sram_blwl_out[115:115] ,mux_1level_tapbuf_size2_97_sram_blwl_outb[115:115] ,mux_1level_tapbuf_size2_97_configbus0[115:115], mux_1level_tapbuf_size2_97_configbus1[115:115] , mux_1level_tapbuf_size2_97_configbus0_b[115:115] );
wire [0:1] mux_1level_tapbuf_size2_98_inbus;
assign mux_1level_tapbuf_size2_98_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_98_inbus[1] = chanx_1__1__in_18_ ;
wire [116:116] mux_1level_tapbuf_size2_98_configbus0;
wire [116:116] mux_1level_tapbuf_size2_98_configbus1;
wire [116:116] mux_1level_tapbuf_size2_98_sram_blwl_out ;
wire [116:116] mux_1level_tapbuf_size2_98_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_98_configbus0[116:116] = sram_blwl_bl[116:116] ;
assign mux_1level_tapbuf_size2_98_configbus1[116:116] = sram_blwl_wl[116:116] ;
wire [116:116] mux_1level_tapbuf_size2_98_configbus0_b;
assign mux_1level_tapbuf_size2_98_configbus0_b[116:116] = sram_blwl_blb[116:116] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_98_ (mux_1level_tapbuf_size2_98_inbus, chany_1__1__out_17_ , mux_1level_tapbuf_size2_98_sram_blwl_out[116:116] ,
mux_1level_tapbuf_size2_98_sram_blwl_outb[116:116] );
//----- SRAM bits for MUX[98], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_116_ (mux_1level_tapbuf_size2_98_sram_blwl_out[116:116] ,mux_1level_tapbuf_size2_98_sram_blwl_out[116:116] ,mux_1level_tapbuf_size2_98_sram_blwl_outb[116:116] ,mux_1level_tapbuf_size2_98_configbus0[116:116], mux_1level_tapbuf_size2_98_configbus1[116:116] , mux_1level_tapbuf_size2_98_configbus0_b[116:116] );
wire [0:1] mux_1level_tapbuf_size2_99_inbus;
assign mux_1level_tapbuf_size2_99_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_99_inbus[1] = chanx_1__1__in_20_ ;
wire [117:117] mux_1level_tapbuf_size2_99_configbus0;
wire [117:117] mux_1level_tapbuf_size2_99_configbus1;
wire [117:117] mux_1level_tapbuf_size2_99_sram_blwl_out ;
wire [117:117] mux_1level_tapbuf_size2_99_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_99_configbus0[117:117] = sram_blwl_bl[117:117] ;
assign mux_1level_tapbuf_size2_99_configbus1[117:117] = sram_blwl_wl[117:117] ;
wire [117:117] mux_1level_tapbuf_size2_99_configbus0_b;
assign mux_1level_tapbuf_size2_99_configbus0_b[117:117] = sram_blwl_blb[117:117] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_99_ (mux_1level_tapbuf_size2_99_inbus, chany_1__1__out_19_ , mux_1level_tapbuf_size2_99_sram_blwl_out[117:117] ,
mux_1level_tapbuf_size2_99_sram_blwl_outb[117:117] );
//----- SRAM bits for MUX[99], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_117_ (mux_1level_tapbuf_size2_99_sram_blwl_out[117:117] ,mux_1level_tapbuf_size2_99_sram_blwl_out[117:117] ,mux_1level_tapbuf_size2_99_sram_blwl_outb[117:117] ,mux_1level_tapbuf_size2_99_configbus0[117:117], mux_1level_tapbuf_size2_99_configbus1[117:117] , mux_1level_tapbuf_size2_99_configbus0_b[117:117] );
wire [0:1] mux_1level_tapbuf_size2_100_inbus;
assign mux_1level_tapbuf_size2_100_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_100_inbus[1] = chanx_1__1__in_22_ ;
wire [118:118] mux_1level_tapbuf_size2_100_configbus0;
wire [118:118] mux_1level_tapbuf_size2_100_configbus1;
wire [118:118] mux_1level_tapbuf_size2_100_sram_blwl_out ;
wire [118:118] mux_1level_tapbuf_size2_100_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_100_configbus0[118:118] = sram_blwl_bl[118:118] ;
assign mux_1level_tapbuf_size2_100_configbus1[118:118] = sram_blwl_wl[118:118] ;
wire [118:118] mux_1level_tapbuf_size2_100_configbus0_b;
assign mux_1level_tapbuf_size2_100_configbus0_b[118:118] = sram_blwl_blb[118:118] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_100_ (mux_1level_tapbuf_size2_100_inbus, chany_1__1__out_21_ , mux_1level_tapbuf_size2_100_sram_blwl_out[118:118] ,
mux_1level_tapbuf_size2_100_sram_blwl_outb[118:118] );
//----- SRAM bits for MUX[100], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_118_ (mux_1level_tapbuf_size2_100_sram_blwl_out[118:118] ,mux_1level_tapbuf_size2_100_sram_blwl_out[118:118] ,mux_1level_tapbuf_size2_100_sram_blwl_outb[118:118] ,mux_1level_tapbuf_size2_100_configbus0[118:118], mux_1level_tapbuf_size2_100_configbus1[118:118] , mux_1level_tapbuf_size2_100_configbus0_b[118:118] );
wire [0:1] mux_1level_tapbuf_size2_101_inbus;
assign mux_1level_tapbuf_size2_101_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_101_inbus[1] = chanx_1__1__in_24_ ;
wire [119:119] mux_1level_tapbuf_size2_101_configbus0;
wire [119:119] mux_1level_tapbuf_size2_101_configbus1;
wire [119:119] mux_1level_tapbuf_size2_101_sram_blwl_out ;
wire [119:119] mux_1level_tapbuf_size2_101_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_101_configbus0[119:119] = sram_blwl_bl[119:119] ;
assign mux_1level_tapbuf_size2_101_configbus1[119:119] = sram_blwl_wl[119:119] ;
wire [119:119] mux_1level_tapbuf_size2_101_configbus0_b;
assign mux_1level_tapbuf_size2_101_configbus0_b[119:119] = sram_blwl_blb[119:119] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_101_ (mux_1level_tapbuf_size2_101_inbus, chany_1__1__out_23_ , mux_1level_tapbuf_size2_101_sram_blwl_out[119:119] ,
mux_1level_tapbuf_size2_101_sram_blwl_outb[119:119] );
//----- SRAM bits for MUX[101], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_119_ (mux_1level_tapbuf_size2_101_sram_blwl_out[119:119] ,mux_1level_tapbuf_size2_101_sram_blwl_out[119:119] ,mux_1level_tapbuf_size2_101_sram_blwl_outb[119:119] ,mux_1level_tapbuf_size2_101_configbus0[119:119], mux_1level_tapbuf_size2_101_configbus1[119:119] , mux_1level_tapbuf_size2_101_configbus0_b[119:119] );
wire [0:1] mux_1level_tapbuf_size2_102_inbus;
assign mux_1level_tapbuf_size2_102_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_102_inbus[1] = chanx_1__1__in_26_ ;
wire [120:120] mux_1level_tapbuf_size2_102_configbus0;
wire [120:120] mux_1level_tapbuf_size2_102_configbus1;
wire [120:120] mux_1level_tapbuf_size2_102_sram_blwl_out ;
wire [120:120] mux_1level_tapbuf_size2_102_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_102_configbus0[120:120] = sram_blwl_bl[120:120] ;
assign mux_1level_tapbuf_size2_102_configbus1[120:120] = sram_blwl_wl[120:120] ;
wire [120:120] mux_1level_tapbuf_size2_102_configbus0_b;
assign mux_1level_tapbuf_size2_102_configbus0_b[120:120] = sram_blwl_blb[120:120] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_102_ (mux_1level_tapbuf_size2_102_inbus, chany_1__1__out_25_ , mux_1level_tapbuf_size2_102_sram_blwl_out[120:120] ,
mux_1level_tapbuf_size2_102_sram_blwl_outb[120:120] );
//----- SRAM bits for MUX[102], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_120_ (mux_1level_tapbuf_size2_102_sram_blwl_out[120:120] ,mux_1level_tapbuf_size2_102_sram_blwl_out[120:120] ,mux_1level_tapbuf_size2_102_sram_blwl_outb[120:120] ,mux_1level_tapbuf_size2_102_configbus0[120:120], mux_1level_tapbuf_size2_102_configbus1[120:120] , mux_1level_tapbuf_size2_102_configbus0_b[120:120] );
wire [0:1] mux_1level_tapbuf_size2_103_inbus;
assign mux_1level_tapbuf_size2_103_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_103_inbus[1] = chanx_1__1__in_28_ ;
wire [121:121] mux_1level_tapbuf_size2_103_configbus0;
wire [121:121] mux_1level_tapbuf_size2_103_configbus1;
wire [121:121] mux_1level_tapbuf_size2_103_sram_blwl_out ;
wire [121:121] mux_1level_tapbuf_size2_103_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_103_configbus0[121:121] = sram_blwl_bl[121:121] ;
assign mux_1level_tapbuf_size2_103_configbus1[121:121] = sram_blwl_wl[121:121] ;
wire [121:121] mux_1level_tapbuf_size2_103_configbus0_b;
assign mux_1level_tapbuf_size2_103_configbus0_b[121:121] = sram_blwl_blb[121:121] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_103_ (mux_1level_tapbuf_size2_103_inbus, chany_1__1__out_27_ , mux_1level_tapbuf_size2_103_sram_blwl_out[121:121] ,
mux_1level_tapbuf_size2_103_sram_blwl_outb[121:121] );
//----- SRAM bits for MUX[103], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_121_ (mux_1level_tapbuf_size2_103_sram_blwl_out[121:121] ,mux_1level_tapbuf_size2_103_sram_blwl_out[121:121] ,mux_1level_tapbuf_size2_103_sram_blwl_outb[121:121] ,mux_1level_tapbuf_size2_103_configbus0[121:121], mux_1level_tapbuf_size2_103_configbus1[121:121] , mux_1level_tapbuf_size2_103_configbus0_b[121:121] );
wire [0:1] mux_1level_tapbuf_size2_104_inbus;
assign mux_1level_tapbuf_size2_104_inbus[0] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size2_104_inbus[1] = chanx_1__1__in_0_ ;
wire [122:122] mux_1level_tapbuf_size2_104_configbus0;
wire [122:122] mux_1level_tapbuf_size2_104_configbus1;
wire [122:122] mux_1level_tapbuf_size2_104_sram_blwl_out ;
wire [122:122] mux_1level_tapbuf_size2_104_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_104_configbus0[122:122] = sram_blwl_bl[122:122] ;
assign mux_1level_tapbuf_size2_104_configbus1[122:122] = sram_blwl_wl[122:122] ;
wire [122:122] mux_1level_tapbuf_size2_104_configbus0_b;
assign mux_1level_tapbuf_size2_104_configbus0_b[122:122] = sram_blwl_blb[122:122] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_104_ (mux_1level_tapbuf_size2_104_inbus, chany_1__1__out_29_ , mux_1level_tapbuf_size2_104_sram_blwl_out[122:122] ,
mux_1level_tapbuf_size2_104_sram_blwl_outb[122:122] );
//----- SRAM bits for MUX[104], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_122_ (mux_1level_tapbuf_size2_104_sram_blwl_out[122:122] ,mux_1level_tapbuf_size2_104_sram_blwl_out[122:122] ,mux_1level_tapbuf_size2_104_sram_blwl_outb[122:122] ,mux_1level_tapbuf_size2_104_configbus0[122:122], mux_1level_tapbuf_size2_104_configbus1[122:122] , mux_1level_tapbuf_size2_104_configbus0_b[122:122] );
//----- left side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_105_inbus;
assign mux_1level_tapbuf_size3_105_inbus[0] =  grid_1__1__pin_0__0__4_;
assign mux_1level_tapbuf_size3_105_inbus[1] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size3_105_inbus[2] = chany_1__1__in_28_ ;
wire [123:125] mux_1level_tapbuf_size3_105_configbus0;
wire [123:125] mux_1level_tapbuf_size3_105_configbus1;
wire [123:125] mux_1level_tapbuf_size3_105_sram_blwl_out ;
wire [123:125] mux_1level_tapbuf_size3_105_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_105_configbus0[123:125] = sram_blwl_bl[123:125] ;
assign mux_1level_tapbuf_size3_105_configbus1[123:125] = sram_blwl_wl[123:125] ;
wire [123:125] mux_1level_tapbuf_size3_105_configbus0_b;
assign mux_1level_tapbuf_size3_105_configbus0_b[123:125] = sram_blwl_blb[123:125] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_105_ (mux_1level_tapbuf_size3_105_inbus, chanx_1__1__out_1_ , mux_1level_tapbuf_size3_105_sram_blwl_out[123:125] ,
mux_1level_tapbuf_size3_105_sram_blwl_outb[123:125] );
//----- SRAM bits for MUX[105], level=1, select_path_id=1. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----010-----
sram6T_blwl sram_blwl_123_ (mux_1level_tapbuf_size3_105_sram_blwl_out[123:123] ,mux_1level_tapbuf_size3_105_sram_blwl_out[123:123] ,mux_1level_tapbuf_size3_105_sram_blwl_outb[123:123] ,mux_1level_tapbuf_size3_105_configbus0[123:123], mux_1level_tapbuf_size3_105_configbus1[123:123] , mux_1level_tapbuf_size3_105_configbus0_b[123:123] );
sram6T_blwl sram_blwl_124_ (mux_1level_tapbuf_size3_105_sram_blwl_out[124:124] ,mux_1level_tapbuf_size3_105_sram_blwl_out[124:124] ,mux_1level_tapbuf_size3_105_sram_blwl_outb[124:124] ,mux_1level_tapbuf_size3_105_configbus0[124:124], mux_1level_tapbuf_size3_105_configbus1[124:124] , mux_1level_tapbuf_size3_105_configbus0_b[124:124] );
sram6T_blwl sram_blwl_125_ (mux_1level_tapbuf_size3_105_sram_blwl_out[125:125] ,mux_1level_tapbuf_size3_105_sram_blwl_out[125:125] ,mux_1level_tapbuf_size3_105_sram_blwl_outb[125:125] ,mux_1level_tapbuf_size3_105_configbus0[125:125], mux_1level_tapbuf_size3_105_configbus1[125:125] , mux_1level_tapbuf_size3_105_configbus0_b[125:125] );
wire [0:2] mux_1level_tapbuf_size3_106_inbus;
assign mux_1level_tapbuf_size3_106_inbus[0] =  grid_1__1__pin_0__0__4_;
assign mux_1level_tapbuf_size3_106_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_106_inbus[2] = chany_1__1__in_0_ ;
wire [126:128] mux_1level_tapbuf_size3_106_configbus0;
wire [126:128] mux_1level_tapbuf_size3_106_configbus1;
wire [126:128] mux_1level_tapbuf_size3_106_sram_blwl_out ;
wire [126:128] mux_1level_tapbuf_size3_106_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_106_configbus0[126:128] = sram_blwl_bl[126:128] ;
assign mux_1level_tapbuf_size3_106_configbus1[126:128] = sram_blwl_wl[126:128] ;
wire [126:128] mux_1level_tapbuf_size3_106_configbus0_b;
assign mux_1level_tapbuf_size3_106_configbus0_b[126:128] = sram_blwl_blb[126:128] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_106_ (mux_1level_tapbuf_size3_106_inbus, chanx_1__1__out_3_ , mux_1level_tapbuf_size3_106_sram_blwl_out[126:128] ,
mux_1level_tapbuf_size3_106_sram_blwl_outb[126:128] );
//----- SRAM bits for MUX[106], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_126_ (mux_1level_tapbuf_size3_106_sram_blwl_out[126:126] ,mux_1level_tapbuf_size3_106_sram_blwl_out[126:126] ,mux_1level_tapbuf_size3_106_sram_blwl_outb[126:126] ,mux_1level_tapbuf_size3_106_configbus0[126:126], mux_1level_tapbuf_size3_106_configbus1[126:126] , mux_1level_tapbuf_size3_106_configbus0_b[126:126] );
sram6T_blwl sram_blwl_127_ (mux_1level_tapbuf_size3_106_sram_blwl_out[127:127] ,mux_1level_tapbuf_size3_106_sram_blwl_out[127:127] ,mux_1level_tapbuf_size3_106_sram_blwl_outb[127:127] ,mux_1level_tapbuf_size3_106_configbus0[127:127], mux_1level_tapbuf_size3_106_configbus1[127:127] , mux_1level_tapbuf_size3_106_configbus0_b[127:127] );
sram6T_blwl sram_blwl_128_ (mux_1level_tapbuf_size3_106_sram_blwl_out[128:128] ,mux_1level_tapbuf_size3_106_sram_blwl_out[128:128] ,mux_1level_tapbuf_size3_106_sram_blwl_outb[128:128] ,mux_1level_tapbuf_size3_106_configbus0[128:128], mux_1level_tapbuf_size3_106_configbus1[128:128] , mux_1level_tapbuf_size3_106_configbus0_b[128:128] );
wire [0:2] mux_1level_tapbuf_size3_107_inbus;
assign mux_1level_tapbuf_size3_107_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size3_107_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_107_inbus[2] = chany_1__1__in_2_ ;
wire [129:131] mux_1level_tapbuf_size3_107_configbus0;
wire [129:131] mux_1level_tapbuf_size3_107_configbus1;
wire [129:131] mux_1level_tapbuf_size3_107_sram_blwl_out ;
wire [129:131] mux_1level_tapbuf_size3_107_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_107_configbus0[129:131] = sram_blwl_bl[129:131] ;
assign mux_1level_tapbuf_size3_107_configbus1[129:131] = sram_blwl_wl[129:131] ;
wire [129:131] mux_1level_tapbuf_size3_107_configbus0_b;
assign mux_1level_tapbuf_size3_107_configbus0_b[129:131] = sram_blwl_blb[129:131] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_107_ (mux_1level_tapbuf_size3_107_inbus, chanx_1__1__out_5_ , mux_1level_tapbuf_size3_107_sram_blwl_out[129:131] ,
mux_1level_tapbuf_size3_107_sram_blwl_outb[129:131] );
//----- SRAM bits for MUX[107], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_129_ (mux_1level_tapbuf_size3_107_sram_blwl_out[129:129] ,mux_1level_tapbuf_size3_107_sram_blwl_out[129:129] ,mux_1level_tapbuf_size3_107_sram_blwl_outb[129:129] ,mux_1level_tapbuf_size3_107_configbus0[129:129], mux_1level_tapbuf_size3_107_configbus1[129:129] , mux_1level_tapbuf_size3_107_configbus0_b[129:129] );
sram6T_blwl sram_blwl_130_ (mux_1level_tapbuf_size3_107_sram_blwl_out[130:130] ,mux_1level_tapbuf_size3_107_sram_blwl_out[130:130] ,mux_1level_tapbuf_size3_107_sram_blwl_outb[130:130] ,mux_1level_tapbuf_size3_107_configbus0[130:130], mux_1level_tapbuf_size3_107_configbus1[130:130] , mux_1level_tapbuf_size3_107_configbus0_b[130:130] );
sram6T_blwl sram_blwl_131_ (mux_1level_tapbuf_size3_107_sram_blwl_out[131:131] ,mux_1level_tapbuf_size3_107_sram_blwl_out[131:131] ,mux_1level_tapbuf_size3_107_sram_blwl_outb[131:131] ,mux_1level_tapbuf_size3_107_configbus0[131:131], mux_1level_tapbuf_size3_107_configbus1[131:131] , mux_1level_tapbuf_size3_107_configbus0_b[131:131] );
wire [0:1] mux_1level_tapbuf_size2_108_inbus;
assign mux_1level_tapbuf_size2_108_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_108_inbus[1] = chany_1__1__in_4_ ;
wire [132:132] mux_1level_tapbuf_size2_108_configbus0;
wire [132:132] mux_1level_tapbuf_size2_108_configbus1;
wire [132:132] mux_1level_tapbuf_size2_108_sram_blwl_out ;
wire [132:132] mux_1level_tapbuf_size2_108_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_108_configbus0[132:132] = sram_blwl_bl[132:132] ;
assign mux_1level_tapbuf_size2_108_configbus1[132:132] = sram_blwl_wl[132:132] ;
wire [132:132] mux_1level_tapbuf_size2_108_configbus0_b;
assign mux_1level_tapbuf_size2_108_configbus0_b[132:132] = sram_blwl_blb[132:132] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_108_ (mux_1level_tapbuf_size2_108_inbus, chanx_1__1__out_7_ , mux_1level_tapbuf_size2_108_sram_blwl_out[132:132] ,
mux_1level_tapbuf_size2_108_sram_blwl_outb[132:132] );
//----- SRAM bits for MUX[108], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_132_ (mux_1level_tapbuf_size2_108_sram_blwl_out[132:132] ,mux_1level_tapbuf_size2_108_sram_blwl_out[132:132] ,mux_1level_tapbuf_size2_108_sram_blwl_outb[132:132] ,mux_1level_tapbuf_size2_108_configbus0[132:132], mux_1level_tapbuf_size2_108_configbus1[132:132] , mux_1level_tapbuf_size2_108_configbus0_b[132:132] );
wire [0:1] mux_1level_tapbuf_size2_109_inbus;
assign mux_1level_tapbuf_size2_109_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_109_inbus[1] = chany_1__1__in_6_ ;
wire [133:133] mux_1level_tapbuf_size2_109_configbus0;
wire [133:133] mux_1level_tapbuf_size2_109_configbus1;
wire [133:133] mux_1level_tapbuf_size2_109_sram_blwl_out ;
wire [133:133] mux_1level_tapbuf_size2_109_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_109_configbus0[133:133] = sram_blwl_bl[133:133] ;
assign mux_1level_tapbuf_size2_109_configbus1[133:133] = sram_blwl_wl[133:133] ;
wire [133:133] mux_1level_tapbuf_size2_109_configbus0_b;
assign mux_1level_tapbuf_size2_109_configbus0_b[133:133] = sram_blwl_blb[133:133] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_109_ (mux_1level_tapbuf_size2_109_inbus, chanx_1__1__out_9_ , mux_1level_tapbuf_size2_109_sram_blwl_out[133:133] ,
mux_1level_tapbuf_size2_109_sram_blwl_outb[133:133] );
//----- SRAM bits for MUX[109], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_133_ (mux_1level_tapbuf_size2_109_sram_blwl_out[133:133] ,mux_1level_tapbuf_size2_109_sram_blwl_out[133:133] ,mux_1level_tapbuf_size2_109_sram_blwl_outb[133:133] ,mux_1level_tapbuf_size2_109_configbus0[133:133], mux_1level_tapbuf_size2_109_configbus1[133:133] , mux_1level_tapbuf_size2_109_configbus0_b[133:133] );
wire [0:1] mux_1level_tapbuf_size2_110_inbus;
assign mux_1level_tapbuf_size2_110_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_110_inbus[1] = chany_1__1__in_8_ ;
wire [134:134] mux_1level_tapbuf_size2_110_configbus0;
wire [134:134] mux_1level_tapbuf_size2_110_configbus1;
wire [134:134] mux_1level_tapbuf_size2_110_sram_blwl_out ;
wire [134:134] mux_1level_tapbuf_size2_110_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_110_configbus0[134:134] = sram_blwl_bl[134:134] ;
assign mux_1level_tapbuf_size2_110_configbus1[134:134] = sram_blwl_wl[134:134] ;
wire [134:134] mux_1level_tapbuf_size2_110_configbus0_b;
assign mux_1level_tapbuf_size2_110_configbus0_b[134:134] = sram_blwl_blb[134:134] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_110_ (mux_1level_tapbuf_size2_110_inbus, chanx_1__1__out_11_ , mux_1level_tapbuf_size2_110_sram_blwl_out[134:134] ,
mux_1level_tapbuf_size2_110_sram_blwl_outb[134:134] );
//----- SRAM bits for MUX[110], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_134_ (mux_1level_tapbuf_size2_110_sram_blwl_out[134:134] ,mux_1level_tapbuf_size2_110_sram_blwl_out[134:134] ,mux_1level_tapbuf_size2_110_sram_blwl_outb[134:134] ,mux_1level_tapbuf_size2_110_configbus0[134:134], mux_1level_tapbuf_size2_110_configbus1[134:134] , mux_1level_tapbuf_size2_110_configbus0_b[134:134] );
wire [0:1] mux_1level_tapbuf_size2_111_inbus;
assign mux_1level_tapbuf_size2_111_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_111_inbus[1] = chany_1__1__in_10_ ;
wire [135:135] mux_1level_tapbuf_size2_111_configbus0;
wire [135:135] mux_1level_tapbuf_size2_111_configbus1;
wire [135:135] mux_1level_tapbuf_size2_111_sram_blwl_out ;
wire [135:135] mux_1level_tapbuf_size2_111_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_111_configbus0[135:135] = sram_blwl_bl[135:135] ;
assign mux_1level_tapbuf_size2_111_configbus1[135:135] = sram_blwl_wl[135:135] ;
wire [135:135] mux_1level_tapbuf_size2_111_configbus0_b;
assign mux_1level_tapbuf_size2_111_configbus0_b[135:135] = sram_blwl_blb[135:135] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_111_ (mux_1level_tapbuf_size2_111_inbus, chanx_1__1__out_13_ , mux_1level_tapbuf_size2_111_sram_blwl_out[135:135] ,
mux_1level_tapbuf_size2_111_sram_blwl_outb[135:135] );
//----- SRAM bits for MUX[111], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_135_ (mux_1level_tapbuf_size2_111_sram_blwl_out[135:135] ,mux_1level_tapbuf_size2_111_sram_blwl_out[135:135] ,mux_1level_tapbuf_size2_111_sram_blwl_outb[135:135] ,mux_1level_tapbuf_size2_111_configbus0[135:135], mux_1level_tapbuf_size2_111_configbus1[135:135] , mux_1level_tapbuf_size2_111_configbus0_b[135:135] );
wire [0:1] mux_1level_tapbuf_size2_112_inbus;
assign mux_1level_tapbuf_size2_112_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_112_inbus[1] = chany_1__1__in_12_ ;
wire [136:136] mux_1level_tapbuf_size2_112_configbus0;
wire [136:136] mux_1level_tapbuf_size2_112_configbus1;
wire [136:136] mux_1level_tapbuf_size2_112_sram_blwl_out ;
wire [136:136] mux_1level_tapbuf_size2_112_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_112_configbus0[136:136] = sram_blwl_bl[136:136] ;
assign mux_1level_tapbuf_size2_112_configbus1[136:136] = sram_blwl_wl[136:136] ;
wire [136:136] mux_1level_tapbuf_size2_112_configbus0_b;
assign mux_1level_tapbuf_size2_112_configbus0_b[136:136] = sram_blwl_blb[136:136] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_112_ (mux_1level_tapbuf_size2_112_inbus, chanx_1__1__out_15_ , mux_1level_tapbuf_size2_112_sram_blwl_out[136:136] ,
mux_1level_tapbuf_size2_112_sram_blwl_outb[136:136] );
//----- SRAM bits for MUX[112], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_136_ (mux_1level_tapbuf_size2_112_sram_blwl_out[136:136] ,mux_1level_tapbuf_size2_112_sram_blwl_out[136:136] ,mux_1level_tapbuf_size2_112_sram_blwl_outb[136:136] ,mux_1level_tapbuf_size2_112_configbus0[136:136], mux_1level_tapbuf_size2_112_configbus1[136:136] , mux_1level_tapbuf_size2_112_configbus0_b[136:136] );
wire [0:1] mux_1level_tapbuf_size2_113_inbus;
assign mux_1level_tapbuf_size2_113_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_113_inbus[1] = chany_1__1__in_14_ ;
wire [137:137] mux_1level_tapbuf_size2_113_configbus0;
wire [137:137] mux_1level_tapbuf_size2_113_configbus1;
wire [137:137] mux_1level_tapbuf_size2_113_sram_blwl_out ;
wire [137:137] mux_1level_tapbuf_size2_113_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_113_configbus0[137:137] = sram_blwl_bl[137:137] ;
assign mux_1level_tapbuf_size2_113_configbus1[137:137] = sram_blwl_wl[137:137] ;
wire [137:137] mux_1level_tapbuf_size2_113_configbus0_b;
assign mux_1level_tapbuf_size2_113_configbus0_b[137:137] = sram_blwl_blb[137:137] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_113_ (mux_1level_tapbuf_size2_113_inbus, chanx_1__1__out_17_ , mux_1level_tapbuf_size2_113_sram_blwl_out[137:137] ,
mux_1level_tapbuf_size2_113_sram_blwl_outb[137:137] );
//----- SRAM bits for MUX[113], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_137_ (mux_1level_tapbuf_size2_113_sram_blwl_out[137:137] ,mux_1level_tapbuf_size2_113_sram_blwl_out[137:137] ,mux_1level_tapbuf_size2_113_sram_blwl_outb[137:137] ,mux_1level_tapbuf_size2_113_configbus0[137:137], mux_1level_tapbuf_size2_113_configbus1[137:137] , mux_1level_tapbuf_size2_113_configbus0_b[137:137] );
wire [0:1] mux_1level_tapbuf_size2_114_inbus;
assign mux_1level_tapbuf_size2_114_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_114_inbus[1] = chany_1__1__in_16_ ;
wire [138:138] mux_1level_tapbuf_size2_114_configbus0;
wire [138:138] mux_1level_tapbuf_size2_114_configbus1;
wire [138:138] mux_1level_tapbuf_size2_114_sram_blwl_out ;
wire [138:138] mux_1level_tapbuf_size2_114_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_114_configbus0[138:138] = sram_blwl_bl[138:138] ;
assign mux_1level_tapbuf_size2_114_configbus1[138:138] = sram_blwl_wl[138:138] ;
wire [138:138] mux_1level_tapbuf_size2_114_configbus0_b;
assign mux_1level_tapbuf_size2_114_configbus0_b[138:138] = sram_blwl_blb[138:138] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_114_ (mux_1level_tapbuf_size2_114_inbus, chanx_1__1__out_19_ , mux_1level_tapbuf_size2_114_sram_blwl_out[138:138] ,
mux_1level_tapbuf_size2_114_sram_blwl_outb[138:138] );
//----- SRAM bits for MUX[114], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_138_ (mux_1level_tapbuf_size2_114_sram_blwl_out[138:138] ,mux_1level_tapbuf_size2_114_sram_blwl_out[138:138] ,mux_1level_tapbuf_size2_114_sram_blwl_outb[138:138] ,mux_1level_tapbuf_size2_114_configbus0[138:138], mux_1level_tapbuf_size2_114_configbus1[138:138] , mux_1level_tapbuf_size2_114_configbus0_b[138:138] );
wire [0:1] mux_1level_tapbuf_size2_115_inbus;
assign mux_1level_tapbuf_size2_115_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_115_inbus[1] = chany_1__1__in_18_ ;
wire [139:139] mux_1level_tapbuf_size2_115_configbus0;
wire [139:139] mux_1level_tapbuf_size2_115_configbus1;
wire [139:139] mux_1level_tapbuf_size2_115_sram_blwl_out ;
wire [139:139] mux_1level_tapbuf_size2_115_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_115_configbus0[139:139] = sram_blwl_bl[139:139] ;
assign mux_1level_tapbuf_size2_115_configbus1[139:139] = sram_blwl_wl[139:139] ;
wire [139:139] mux_1level_tapbuf_size2_115_configbus0_b;
assign mux_1level_tapbuf_size2_115_configbus0_b[139:139] = sram_blwl_blb[139:139] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_115_ (mux_1level_tapbuf_size2_115_inbus, chanx_1__1__out_21_ , mux_1level_tapbuf_size2_115_sram_blwl_out[139:139] ,
mux_1level_tapbuf_size2_115_sram_blwl_outb[139:139] );
//----- SRAM bits for MUX[115], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_139_ (mux_1level_tapbuf_size2_115_sram_blwl_out[139:139] ,mux_1level_tapbuf_size2_115_sram_blwl_out[139:139] ,mux_1level_tapbuf_size2_115_sram_blwl_outb[139:139] ,mux_1level_tapbuf_size2_115_configbus0[139:139], mux_1level_tapbuf_size2_115_configbus1[139:139] , mux_1level_tapbuf_size2_115_configbus0_b[139:139] );
wire [0:1] mux_1level_tapbuf_size2_116_inbus;
assign mux_1level_tapbuf_size2_116_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_116_inbus[1] = chany_1__1__in_20_ ;
wire [140:140] mux_1level_tapbuf_size2_116_configbus0;
wire [140:140] mux_1level_tapbuf_size2_116_configbus1;
wire [140:140] mux_1level_tapbuf_size2_116_sram_blwl_out ;
wire [140:140] mux_1level_tapbuf_size2_116_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_116_configbus0[140:140] = sram_blwl_bl[140:140] ;
assign mux_1level_tapbuf_size2_116_configbus1[140:140] = sram_blwl_wl[140:140] ;
wire [140:140] mux_1level_tapbuf_size2_116_configbus0_b;
assign mux_1level_tapbuf_size2_116_configbus0_b[140:140] = sram_blwl_blb[140:140] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_116_ (mux_1level_tapbuf_size2_116_inbus, chanx_1__1__out_23_ , mux_1level_tapbuf_size2_116_sram_blwl_out[140:140] ,
mux_1level_tapbuf_size2_116_sram_blwl_outb[140:140] );
//----- SRAM bits for MUX[116], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_140_ (mux_1level_tapbuf_size2_116_sram_blwl_out[140:140] ,mux_1level_tapbuf_size2_116_sram_blwl_out[140:140] ,mux_1level_tapbuf_size2_116_sram_blwl_outb[140:140] ,mux_1level_tapbuf_size2_116_configbus0[140:140], mux_1level_tapbuf_size2_116_configbus1[140:140] , mux_1level_tapbuf_size2_116_configbus0_b[140:140] );
wire [0:1] mux_1level_tapbuf_size2_117_inbus;
assign mux_1level_tapbuf_size2_117_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_117_inbus[1] = chany_1__1__in_22_ ;
wire [141:141] mux_1level_tapbuf_size2_117_configbus0;
wire [141:141] mux_1level_tapbuf_size2_117_configbus1;
wire [141:141] mux_1level_tapbuf_size2_117_sram_blwl_out ;
wire [141:141] mux_1level_tapbuf_size2_117_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_117_configbus0[141:141] = sram_blwl_bl[141:141] ;
assign mux_1level_tapbuf_size2_117_configbus1[141:141] = sram_blwl_wl[141:141] ;
wire [141:141] mux_1level_tapbuf_size2_117_configbus0_b;
assign mux_1level_tapbuf_size2_117_configbus0_b[141:141] = sram_blwl_blb[141:141] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_117_ (mux_1level_tapbuf_size2_117_inbus, chanx_1__1__out_25_ , mux_1level_tapbuf_size2_117_sram_blwl_out[141:141] ,
mux_1level_tapbuf_size2_117_sram_blwl_outb[141:141] );
//----- SRAM bits for MUX[117], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_141_ (mux_1level_tapbuf_size2_117_sram_blwl_out[141:141] ,mux_1level_tapbuf_size2_117_sram_blwl_out[141:141] ,mux_1level_tapbuf_size2_117_sram_blwl_outb[141:141] ,mux_1level_tapbuf_size2_117_configbus0[141:141], mux_1level_tapbuf_size2_117_configbus1[141:141] , mux_1level_tapbuf_size2_117_configbus0_b[141:141] );
wire [0:1] mux_1level_tapbuf_size2_118_inbus;
assign mux_1level_tapbuf_size2_118_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_118_inbus[1] = chany_1__1__in_24_ ;
wire [142:142] mux_1level_tapbuf_size2_118_configbus0;
wire [142:142] mux_1level_tapbuf_size2_118_configbus1;
wire [142:142] mux_1level_tapbuf_size2_118_sram_blwl_out ;
wire [142:142] mux_1level_tapbuf_size2_118_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_118_configbus0[142:142] = sram_blwl_bl[142:142] ;
assign mux_1level_tapbuf_size2_118_configbus1[142:142] = sram_blwl_wl[142:142] ;
wire [142:142] mux_1level_tapbuf_size2_118_configbus0_b;
assign mux_1level_tapbuf_size2_118_configbus0_b[142:142] = sram_blwl_blb[142:142] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_118_ (mux_1level_tapbuf_size2_118_inbus, chanx_1__1__out_27_ , mux_1level_tapbuf_size2_118_sram_blwl_out[142:142] ,
mux_1level_tapbuf_size2_118_sram_blwl_outb[142:142] );
//----- SRAM bits for MUX[118], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_142_ (mux_1level_tapbuf_size2_118_sram_blwl_out[142:142] ,mux_1level_tapbuf_size2_118_sram_blwl_out[142:142] ,mux_1level_tapbuf_size2_118_sram_blwl_outb[142:142] ,mux_1level_tapbuf_size2_118_configbus0[142:142], mux_1level_tapbuf_size2_118_configbus1[142:142] , mux_1level_tapbuf_size2_118_configbus0_b[142:142] );
wire [0:1] mux_1level_tapbuf_size2_119_inbus;
assign mux_1level_tapbuf_size2_119_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_119_inbus[1] = chany_1__1__in_26_ ;
wire [143:143] mux_1level_tapbuf_size2_119_configbus0;
wire [143:143] mux_1level_tapbuf_size2_119_configbus1;
wire [143:143] mux_1level_tapbuf_size2_119_sram_blwl_out ;
wire [143:143] mux_1level_tapbuf_size2_119_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_119_configbus0[143:143] = sram_blwl_bl[143:143] ;
assign mux_1level_tapbuf_size2_119_configbus1[143:143] = sram_blwl_wl[143:143] ;
wire [143:143] mux_1level_tapbuf_size2_119_configbus0_b;
assign mux_1level_tapbuf_size2_119_configbus0_b[143:143] = sram_blwl_blb[143:143] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_119_ (mux_1level_tapbuf_size2_119_inbus, chanx_1__1__out_29_ , mux_1level_tapbuf_size2_119_sram_blwl_out[143:143] ,
mux_1level_tapbuf_size2_119_sram_blwl_outb[143:143] );
//----- SRAM bits for MUX[119], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_143_ (mux_1level_tapbuf_size2_119_sram_blwl_out[143:143] ,mux_1level_tapbuf_size2_119_sram_blwl_out[143:143] ,mux_1level_tapbuf_size2_119_sram_blwl_outb[143:143] ,mux_1level_tapbuf_size2_119_configbus0[143:143], mux_1level_tapbuf_size2_119_configbus1[143:143] , mux_1level_tapbuf_size2_119_configbus0_b[143:143] );
endmodule
//----- END Verilog Module of Switch Box[1][1] -----

