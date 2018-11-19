//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Switch Block  [1][0] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Verilog Module of Switch Box[1][0] -----
module sb_1__0_ ( 

//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
//----- Inputs/outputs of top side -----
  output chany_1__1__out_0_,
  input chany_1__1__in_1_,
  output chany_1__1__out_2_,
  input chany_1__1__in_3_,
  output chany_1__1__out_4_,
  input chany_1__1__in_5_,
  output chany_1__1__out_6_,
  input chany_1__1__in_7_,
  output chany_1__1__out_8_,
  input chany_1__1__in_9_,
  output chany_1__1__out_10_,
  input chany_1__1__in_11_,
  output chany_1__1__out_12_,
  input chany_1__1__in_13_,
  output chany_1__1__out_14_,
  input chany_1__1__in_15_,
  output chany_1__1__out_16_,
  input chany_1__1__in_17_,
  output chany_1__1__out_18_,
  input chany_1__1__in_19_,
  output chany_1__1__out_20_,
  input chany_1__1__in_21_,
  output chany_1__1__out_22_,
  input chany_1__1__in_23_,
  output chany_1__1__out_24_,
  input chany_1__1__in_25_,
  output chany_1__1__out_26_,
  input chany_1__1__in_27_,
  output chany_1__1__out_28_,
  input chany_1__1__in_29_,
input  grid_2__1__pin_0__3__1_,
input  grid_2__1__pin_0__3__3_,
input  grid_2__1__pin_0__3__5_,
input  grid_2__1__pin_0__3__7_,
input  grid_2__1__pin_0__3__9_,
input  grid_2__1__pin_0__3__11_,
input  grid_2__1__pin_0__3__13_,
input  grid_2__1__pin_0__3__15_,
//----- Inputs/outputs of right side -----
//----- Inputs/outputs of bottom side -----
//----- Inputs/outputs of left side -----
  input chanx_1__0__in_0_,
  output chanx_1__0__out_1_,
  input chanx_1__0__in_2_,
  output chanx_1__0__out_3_,
  input chanx_1__0__in_4_,
  output chanx_1__0__out_5_,
  input chanx_1__0__in_6_,
  output chanx_1__0__out_7_,
  input chanx_1__0__in_8_,
  output chanx_1__0__out_9_,
  input chanx_1__0__in_10_,
  output chanx_1__0__out_11_,
  input chanx_1__0__in_12_,
  output chanx_1__0__out_13_,
  input chanx_1__0__in_14_,
  output chanx_1__0__out_15_,
  input chanx_1__0__in_16_,
  output chanx_1__0__out_17_,
  input chanx_1__0__in_18_,
  output chanx_1__0__out_19_,
  input chanx_1__0__in_20_,
  output chanx_1__0__out_21_,
  input chanx_1__0__in_22_,
  output chanx_1__0__out_23_,
  input chanx_1__0__in_24_,
  output chanx_1__0__out_25_,
  input chanx_1__0__in_26_,
  output chanx_1__0__out_27_,
  input chanx_1__0__in_28_,
  output chanx_1__0__out_29_,
input  grid_1__0__pin_0__0__1_,
input  grid_1__0__pin_0__0__3_,
input  grid_1__0__pin_0__0__5_,
input  grid_1__0__pin_0__0__7_,
input  grid_1__0__pin_0__0__9_,
input  grid_1__0__pin_0__0__11_,
input  grid_1__0__pin_0__0__13_,
input  grid_1__0__pin_0__0__15_,
input [72:105] sram_blwl_bl ,
input [72:105] sram_blwl_wl ,
input [72:105] sram_blwl_blb ); 
//----- top side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_60_inbus;
assign mux_1level_tapbuf_size3_60_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size3_60_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_60_inbus[2] = chanx_1__0__in_0_ ;
wire [72:74] mux_1level_tapbuf_size3_60_configbus0;
wire [72:74] mux_1level_tapbuf_size3_60_configbus1;
wire [72:74] mux_1level_tapbuf_size3_60_sram_blwl_out ;
wire [72:74] mux_1level_tapbuf_size3_60_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_60_configbus0[72:74] = sram_blwl_bl[72:74] ;
assign mux_1level_tapbuf_size3_60_configbus1[72:74] = sram_blwl_wl[72:74] ;
wire [72:74] mux_1level_tapbuf_size3_60_configbus0_b;
assign mux_1level_tapbuf_size3_60_configbus0_b[72:74] = sram_blwl_blb[72:74] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_60_ (mux_1level_tapbuf_size3_60_inbus, chany_1__1__out_0_ , mux_1level_tapbuf_size3_60_sram_blwl_out[72:74] ,
mux_1level_tapbuf_size3_60_sram_blwl_outb[72:74] );
//----- SRAM bits for MUX[60], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_72_ (mux_1level_tapbuf_size3_60_sram_blwl_out[72:72] ,mux_1level_tapbuf_size3_60_sram_blwl_out[72:72] ,mux_1level_tapbuf_size3_60_sram_blwl_outb[72:72] ,mux_1level_tapbuf_size3_60_configbus0[72:72], mux_1level_tapbuf_size3_60_configbus1[72:72] , mux_1level_tapbuf_size3_60_configbus0_b[72:72] );
sram6T_blwl sram_blwl_73_ (mux_1level_tapbuf_size3_60_sram_blwl_out[73:73] ,mux_1level_tapbuf_size3_60_sram_blwl_out[73:73] ,mux_1level_tapbuf_size3_60_sram_blwl_outb[73:73] ,mux_1level_tapbuf_size3_60_configbus0[73:73], mux_1level_tapbuf_size3_60_configbus1[73:73] , mux_1level_tapbuf_size3_60_configbus0_b[73:73] );
sram6T_blwl sram_blwl_74_ (mux_1level_tapbuf_size3_60_sram_blwl_out[74:74] ,mux_1level_tapbuf_size3_60_sram_blwl_out[74:74] ,mux_1level_tapbuf_size3_60_sram_blwl_outb[74:74] ,mux_1level_tapbuf_size3_60_configbus0[74:74], mux_1level_tapbuf_size3_60_configbus1[74:74] , mux_1level_tapbuf_size3_60_configbus0_b[74:74] );
wire [0:1] mux_1level_tapbuf_size2_61_inbus;
assign mux_1level_tapbuf_size2_61_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_61_inbus[1] = chanx_1__0__in_28_ ;
wire [75:75] mux_1level_tapbuf_size2_61_configbus0;
wire [75:75] mux_1level_tapbuf_size2_61_configbus1;
wire [75:75] mux_1level_tapbuf_size2_61_sram_blwl_out ;
wire [75:75] mux_1level_tapbuf_size2_61_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_61_configbus0[75:75] = sram_blwl_bl[75:75] ;
assign mux_1level_tapbuf_size2_61_configbus1[75:75] = sram_blwl_wl[75:75] ;
wire [75:75] mux_1level_tapbuf_size2_61_configbus0_b;
assign mux_1level_tapbuf_size2_61_configbus0_b[75:75] = sram_blwl_blb[75:75] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_61_ (mux_1level_tapbuf_size2_61_inbus, chany_1__1__out_2_ , mux_1level_tapbuf_size2_61_sram_blwl_out[75:75] ,
mux_1level_tapbuf_size2_61_sram_blwl_outb[75:75] );
//----- SRAM bits for MUX[61], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_75_ (mux_1level_tapbuf_size2_61_sram_blwl_out[75:75] ,mux_1level_tapbuf_size2_61_sram_blwl_out[75:75] ,mux_1level_tapbuf_size2_61_sram_blwl_outb[75:75] ,mux_1level_tapbuf_size2_61_configbus0[75:75], mux_1level_tapbuf_size2_61_configbus1[75:75] , mux_1level_tapbuf_size2_61_configbus0_b[75:75] );
wire [0:1] mux_1level_tapbuf_size2_62_inbus;
assign mux_1level_tapbuf_size2_62_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_62_inbus[1] = chanx_1__0__in_26_ ;
wire [76:76] mux_1level_tapbuf_size2_62_configbus0;
wire [76:76] mux_1level_tapbuf_size2_62_configbus1;
wire [76:76] mux_1level_tapbuf_size2_62_sram_blwl_out ;
wire [76:76] mux_1level_tapbuf_size2_62_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_62_configbus0[76:76] = sram_blwl_bl[76:76] ;
assign mux_1level_tapbuf_size2_62_configbus1[76:76] = sram_blwl_wl[76:76] ;
wire [76:76] mux_1level_tapbuf_size2_62_configbus0_b;
assign mux_1level_tapbuf_size2_62_configbus0_b[76:76] = sram_blwl_blb[76:76] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_62_ (mux_1level_tapbuf_size2_62_inbus, chany_1__1__out_4_ , mux_1level_tapbuf_size2_62_sram_blwl_out[76:76] ,
mux_1level_tapbuf_size2_62_sram_blwl_outb[76:76] );
//----- SRAM bits for MUX[62], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_76_ (mux_1level_tapbuf_size2_62_sram_blwl_out[76:76] ,mux_1level_tapbuf_size2_62_sram_blwl_out[76:76] ,mux_1level_tapbuf_size2_62_sram_blwl_outb[76:76] ,mux_1level_tapbuf_size2_62_configbus0[76:76], mux_1level_tapbuf_size2_62_configbus1[76:76] , mux_1level_tapbuf_size2_62_configbus0_b[76:76] );
wire [0:1] mux_1level_tapbuf_size2_63_inbus;
assign mux_1level_tapbuf_size2_63_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_63_inbus[1] = chanx_1__0__in_24_ ;
wire [77:77] mux_1level_tapbuf_size2_63_configbus0;
wire [77:77] mux_1level_tapbuf_size2_63_configbus1;
wire [77:77] mux_1level_tapbuf_size2_63_sram_blwl_out ;
wire [77:77] mux_1level_tapbuf_size2_63_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_63_configbus0[77:77] = sram_blwl_bl[77:77] ;
assign mux_1level_tapbuf_size2_63_configbus1[77:77] = sram_blwl_wl[77:77] ;
wire [77:77] mux_1level_tapbuf_size2_63_configbus0_b;
assign mux_1level_tapbuf_size2_63_configbus0_b[77:77] = sram_blwl_blb[77:77] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_63_ (mux_1level_tapbuf_size2_63_inbus, chany_1__1__out_6_ , mux_1level_tapbuf_size2_63_sram_blwl_out[77:77] ,
mux_1level_tapbuf_size2_63_sram_blwl_outb[77:77] );
//----- SRAM bits for MUX[63], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_77_ (mux_1level_tapbuf_size2_63_sram_blwl_out[77:77] ,mux_1level_tapbuf_size2_63_sram_blwl_out[77:77] ,mux_1level_tapbuf_size2_63_sram_blwl_outb[77:77] ,mux_1level_tapbuf_size2_63_configbus0[77:77], mux_1level_tapbuf_size2_63_configbus1[77:77] , mux_1level_tapbuf_size2_63_configbus0_b[77:77] );
wire [0:1] mux_1level_tapbuf_size2_64_inbus;
assign mux_1level_tapbuf_size2_64_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_64_inbus[1] = chanx_1__0__in_22_ ;
wire [78:78] mux_1level_tapbuf_size2_64_configbus0;
wire [78:78] mux_1level_tapbuf_size2_64_configbus1;
wire [78:78] mux_1level_tapbuf_size2_64_sram_blwl_out ;
wire [78:78] mux_1level_tapbuf_size2_64_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_64_configbus0[78:78] = sram_blwl_bl[78:78] ;
assign mux_1level_tapbuf_size2_64_configbus1[78:78] = sram_blwl_wl[78:78] ;
wire [78:78] mux_1level_tapbuf_size2_64_configbus0_b;
assign mux_1level_tapbuf_size2_64_configbus0_b[78:78] = sram_blwl_blb[78:78] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_64_ (mux_1level_tapbuf_size2_64_inbus, chany_1__1__out_8_ , mux_1level_tapbuf_size2_64_sram_blwl_out[78:78] ,
mux_1level_tapbuf_size2_64_sram_blwl_outb[78:78] );
//----- SRAM bits for MUX[64], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_78_ (mux_1level_tapbuf_size2_64_sram_blwl_out[78:78] ,mux_1level_tapbuf_size2_64_sram_blwl_out[78:78] ,mux_1level_tapbuf_size2_64_sram_blwl_outb[78:78] ,mux_1level_tapbuf_size2_64_configbus0[78:78], mux_1level_tapbuf_size2_64_configbus1[78:78] , mux_1level_tapbuf_size2_64_configbus0_b[78:78] );
wire [0:1] mux_1level_tapbuf_size2_65_inbus;
assign mux_1level_tapbuf_size2_65_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_65_inbus[1] = chanx_1__0__in_20_ ;
wire [79:79] mux_1level_tapbuf_size2_65_configbus0;
wire [79:79] mux_1level_tapbuf_size2_65_configbus1;
wire [79:79] mux_1level_tapbuf_size2_65_sram_blwl_out ;
wire [79:79] mux_1level_tapbuf_size2_65_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_65_configbus0[79:79] = sram_blwl_bl[79:79] ;
assign mux_1level_tapbuf_size2_65_configbus1[79:79] = sram_blwl_wl[79:79] ;
wire [79:79] mux_1level_tapbuf_size2_65_configbus0_b;
assign mux_1level_tapbuf_size2_65_configbus0_b[79:79] = sram_blwl_blb[79:79] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_65_ (mux_1level_tapbuf_size2_65_inbus, chany_1__1__out_10_ , mux_1level_tapbuf_size2_65_sram_blwl_out[79:79] ,
mux_1level_tapbuf_size2_65_sram_blwl_outb[79:79] );
//----- SRAM bits for MUX[65], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_79_ (mux_1level_tapbuf_size2_65_sram_blwl_out[79:79] ,mux_1level_tapbuf_size2_65_sram_blwl_out[79:79] ,mux_1level_tapbuf_size2_65_sram_blwl_outb[79:79] ,mux_1level_tapbuf_size2_65_configbus0[79:79], mux_1level_tapbuf_size2_65_configbus1[79:79] , mux_1level_tapbuf_size2_65_configbus0_b[79:79] );
wire [0:1] mux_1level_tapbuf_size2_66_inbus;
assign mux_1level_tapbuf_size2_66_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_66_inbus[1] = chanx_1__0__in_18_ ;
wire [80:80] mux_1level_tapbuf_size2_66_configbus0;
wire [80:80] mux_1level_tapbuf_size2_66_configbus1;
wire [80:80] mux_1level_tapbuf_size2_66_sram_blwl_out ;
wire [80:80] mux_1level_tapbuf_size2_66_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_66_configbus0[80:80] = sram_blwl_bl[80:80] ;
assign mux_1level_tapbuf_size2_66_configbus1[80:80] = sram_blwl_wl[80:80] ;
wire [80:80] mux_1level_tapbuf_size2_66_configbus0_b;
assign mux_1level_tapbuf_size2_66_configbus0_b[80:80] = sram_blwl_blb[80:80] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_66_ (mux_1level_tapbuf_size2_66_inbus, chany_1__1__out_12_ , mux_1level_tapbuf_size2_66_sram_blwl_out[80:80] ,
mux_1level_tapbuf_size2_66_sram_blwl_outb[80:80] );
//----- SRAM bits for MUX[66], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_80_ (mux_1level_tapbuf_size2_66_sram_blwl_out[80:80] ,mux_1level_tapbuf_size2_66_sram_blwl_out[80:80] ,mux_1level_tapbuf_size2_66_sram_blwl_outb[80:80] ,mux_1level_tapbuf_size2_66_configbus0[80:80], mux_1level_tapbuf_size2_66_configbus1[80:80] , mux_1level_tapbuf_size2_66_configbus0_b[80:80] );
wire [0:1] mux_1level_tapbuf_size2_67_inbus;
assign mux_1level_tapbuf_size2_67_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_67_inbus[1] = chanx_1__0__in_16_ ;
wire [81:81] mux_1level_tapbuf_size2_67_configbus0;
wire [81:81] mux_1level_tapbuf_size2_67_configbus1;
wire [81:81] mux_1level_tapbuf_size2_67_sram_blwl_out ;
wire [81:81] mux_1level_tapbuf_size2_67_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_67_configbus0[81:81] = sram_blwl_bl[81:81] ;
assign mux_1level_tapbuf_size2_67_configbus1[81:81] = sram_blwl_wl[81:81] ;
wire [81:81] mux_1level_tapbuf_size2_67_configbus0_b;
assign mux_1level_tapbuf_size2_67_configbus0_b[81:81] = sram_blwl_blb[81:81] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_67_ (mux_1level_tapbuf_size2_67_inbus, chany_1__1__out_14_ , mux_1level_tapbuf_size2_67_sram_blwl_out[81:81] ,
mux_1level_tapbuf_size2_67_sram_blwl_outb[81:81] );
//----- SRAM bits for MUX[67], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_81_ (mux_1level_tapbuf_size2_67_sram_blwl_out[81:81] ,mux_1level_tapbuf_size2_67_sram_blwl_out[81:81] ,mux_1level_tapbuf_size2_67_sram_blwl_outb[81:81] ,mux_1level_tapbuf_size2_67_configbus0[81:81], mux_1level_tapbuf_size2_67_configbus1[81:81] , mux_1level_tapbuf_size2_67_configbus0_b[81:81] );
wire [0:1] mux_1level_tapbuf_size2_68_inbus;
assign mux_1level_tapbuf_size2_68_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_68_inbus[1] = chanx_1__0__in_14_ ;
wire [82:82] mux_1level_tapbuf_size2_68_configbus0;
wire [82:82] mux_1level_tapbuf_size2_68_configbus1;
wire [82:82] mux_1level_tapbuf_size2_68_sram_blwl_out ;
wire [82:82] mux_1level_tapbuf_size2_68_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_68_configbus0[82:82] = sram_blwl_bl[82:82] ;
assign mux_1level_tapbuf_size2_68_configbus1[82:82] = sram_blwl_wl[82:82] ;
wire [82:82] mux_1level_tapbuf_size2_68_configbus0_b;
assign mux_1level_tapbuf_size2_68_configbus0_b[82:82] = sram_blwl_blb[82:82] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_68_ (mux_1level_tapbuf_size2_68_inbus, chany_1__1__out_16_ , mux_1level_tapbuf_size2_68_sram_blwl_out[82:82] ,
mux_1level_tapbuf_size2_68_sram_blwl_outb[82:82] );
//----- SRAM bits for MUX[68], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_82_ (mux_1level_tapbuf_size2_68_sram_blwl_out[82:82] ,mux_1level_tapbuf_size2_68_sram_blwl_out[82:82] ,mux_1level_tapbuf_size2_68_sram_blwl_outb[82:82] ,mux_1level_tapbuf_size2_68_configbus0[82:82], mux_1level_tapbuf_size2_68_configbus1[82:82] , mux_1level_tapbuf_size2_68_configbus0_b[82:82] );
wire [0:1] mux_1level_tapbuf_size2_69_inbus;
assign mux_1level_tapbuf_size2_69_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_69_inbus[1] = chanx_1__0__in_12_ ;
wire [83:83] mux_1level_tapbuf_size2_69_configbus0;
wire [83:83] mux_1level_tapbuf_size2_69_configbus1;
wire [83:83] mux_1level_tapbuf_size2_69_sram_blwl_out ;
wire [83:83] mux_1level_tapbuf_size2_69_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_69_configbus0[83:83] = sram_blwl_bl[83:83] ;
assign mux_1level_tapbuf_size2_69_configbus1[83:83] = sram_blwl_wl[83:83] ;
wire [83:83] mux_1level_tapbuf_size2_69_configbus0_b;
assign mux_1level_tapbuf_size2_69_configbus0_b[83:83] = sram_blwl_blb[83:83] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_69_ (mux_1level_tapbuf_size2_69_inbus, chany_1__1__out_18_ , mux_1level_tapbuf_size2_69_sram_blwl_out[83:83] ,
mux_1level_tapbuf_size2_69_sram_blwl_outb[83:83] );
//----- SRAM bits for MUX[69], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_83_ (mux_1level_tapbuf_size2_69_sram_blwl_out[83:83] ,mux_1level_tapbuf_size2_69_sram_blwl_out[83:83] ,mux_1level_tapbuf_size2_69_sram_blwl_outb[83:83] ,mux_1level_tapbuf_size2_69_configbus0[83:83], mux_1level_tapbuf_size2_69_configbus1[83:83] , mux_1level_tapbuf_size2_69_configbus0_b[83:83] );
wire [0:1] mux_1level_tapbuf_size2_70_inbus;
assign mux_1level_tapbuf_size2_70_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_70_inbus[1] = chanx_1__0__in_10_ ;
wire [84:84] mux_1level_tapbuf_size2_70_configbus0;
wire [84:84] mux_1level_tapbuf_size2_70_configbus1;
wire [84:84] mux_1level_tapbuf_size2_70_sram_blwl_out ;
wire [84:84] mux_1level_tapbuf_size2_70_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_70_configbus0[84:84] = sram_blwl_bl[84:84] ;
assign mux_1level_tapbuf_size2_70_configbus1[84:84] = sram_blwl_wl[84:84] ;
wire [84:84] mux_1level_tapbuf_size2_70_configbus0_b;
assign mux_1level_tapbuf_size2_70_configbus0_b[84:84] = sram_blwl_blb[84:84] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_70_ (mux_1level_tapbuf_size2_70_inbus, chany_1__1__out_20_ , mux_1level_tapbuf_size2_70_sram_blwl_out[84:84] ,
mux_1level_tapbuf_size2_70_sram_blwl_outb[84:84] );
//----- SRAM bits for MUX[70], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_84_ (mux_1level_tapbuf_size2_70_sram_blwl_out[84:84] ,mux_1level_tapbuf_size2_70_sram_blwl_out[84:84] ,mux_1level_tapbuf_size2_70_sram_blwl_outb[84:84] ,mux_1level_tapbuf_size2_70_configbus0[84:84], mux_1level_tapbuf_size2_70_configbus1[84:84] , mux_1level_tapbuf_size2_70_configbus0_b[84:84] );
wire [0:1] mux_1level_tapbuf_size2_71_inbus;
assign mux_1level_tapbuf_size2_71_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_71_inbus[1] = chanx_1__0__in_8_ ;
wire [85:85] mux_1level_tapbuf_size2_71_configbus0;
wire [85:85] mux_1level_tapbuf_size2_71_configbus1;
wire [85:85] mux_1level_tapbuf_size2_71_sram_blwl_out ;
wire [85:85] mux_1level_tapbuf_size2_71_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_71_configbus0[85:85] = sram_blwl_bl[85:85] ;
assign mux_1level_tapbuf_size2_71_configbus1[85:85] = sram_blwl_wl[85:85] ;
wire [85:85] mux_1level_tapbuf_size2_71_configbus0_b;
assign mux_1level_tapbuf_size2_71_configbus0_b[85:85] = sram_blwl_blb[85:85] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_71_ (mux_1level_tapbuf_size2_71_inbus, chany_1__1__out_22_ , mux_1level_tapbuf_size2_71_sram_blwl_out[85:85] ,
mux_1level_tapbuf_size2_71_sram_blwl_outb[85:85] );
//----- SRAM bits for MUX[71], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_85_ (mux_1level_tapbuf_size2_71_sram_blwl_out[85:85] ,mux_1level_tapbuf_size2_71_sram_blwl_out[85:85] ,mux_1level_tapbuf_size2_71_sram_blwl_outb[85:85] ,mux_1level_tapbuf_size2_71_configbus0[85:85], mux_1level_tapbuf_size2_71_configbus1[85:85] , mux_1level_tapbuf_size2_71_configbus0_b[85:85] );
wire [0:1] mux_1level_tapbuf_size2_72_inbus;
assign mux_1level_tapbuf_size2_72_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_72_inbus[1] = chanx_1__0__in_6_ ;
wire [86:86] mux_1level_tapbuf_size2_72_configbus0;
wire [86:86] mux_1level_tapbuf_size2_72_configbus1;
wire [86:86] mux_1level_tapbuf_size2_72_sram_blwl_out ;
wire [86:86] mux_1level_tapbuf_size2_72_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_72_configbus0[86:86] = sram_blwl_bl[86:86] ;
assign mux_1level_tapbuf_size2_72_configbus1[86:86] = sram_blwl_wl[86:86] ;
wire [86:86] mux_1level_tapbuf_size2_72_configbus0_b;
assign mux_1level_tapbuf_size2_72_configbus0_b[86:86] = sram_blwl_blb[86:86] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_72_ (mux_1level_tapbuf_size2_72_inbus, chany_1__1__out_24_ , mux_1level_tapbuf_size2_72_sram_blwl_out[86:86] ,
mux_1level_tapbuf_size2_72_sram_blwl_outb[86:86] );
//----- SRAM bits for MUX[72], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_86_ (mux_1level_tapbuf_size2_72_sram_blwl_out[86:86] ,mux_1level_tapbuf_size2_72_sram_blwl_out[86:86] ,mux_1level_tapbuf_size2_72_sram_blwl_outb[86:86] ,mux_1level_tapbuf_size2_72_configbus0[86:86], mux_1level_tapbuf_size2_72_configbus1[86:86] , mux_1level_tapbuf_size2_72_configbus0_b[86:86] );
wire [0:1] mux_1level_tapbuf_size2_73_inbus;
assign mux_1level_tapbuf_size2_73_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_73_inbus[1] = chanx_1__0__in_4_ ;
wire [87:87] mux_1level_tapbuf_size2_73_configbus0;
wire [87:87] mux_1level_tapbuf_size2_73_configbus1;
wire [87:87] mux_1level_tapbuf_size2_73_sram_blwl_out ;
wire [87:87] mux_1level_tapbuf_size2_73_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_73_configbus0[87:87] = sram_blwl_bl[87:87] ;
assign mux_1level_tapbuf_size2_73_configbus1[87:87] = sram_blwl_wl[87:87] ;
wire [87:87] mux_1level_tapbuf_size2_73_configbus0_b;
assign mux_1level_tapbuf_size2_73_configbus0_b[87:87] = sram_blwl_blb[87:87] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_73_ (mux_1level_tapbuf_size2_73_inbus, chany_1__1__out_26_ , mux_1level_tapbuf_size2_73_sram_blwl_out[87:87] ,
mux_1level_tapbuf_size2_73_sram_blwl_outb[87:87] );
//----- SRAM bits for MUX[73], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_87_ (mux_1level_tapbuf_size2_73_sram_blwl_out[87:87] ,mux_1level_tapbuf_size2_73_sram_blwl_out[87:87] ,mux_1level_tapbuf_size2_73_sram_blwl_outb[87:87] ,mux_1level_tapbuf_size2_73_configbus0[87:87], mux_1level_tapbuf_size2_73_configbus1[87:87] , mux_1level_tapbuf_size2_73_configbus0_b[87:87] );
wire [0:1] mux_1level_tapbuf_size2_74_inbus;
assign mux_1level_tapbuf_size2_74_inbus[0] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size2_74_inbus[1] = chanx_1__0__in_2_ ;
wire [88:88] mux_1level_tapbuf_size2_74_configbus0;
wire [88:88] mux_1level_tapbuf_size2_74_configbus1;
wire [88:88] mux_1level_tapbuf_size2_74_sram_blwl_out ;
wire [88:88] mux_1level_tapbuf_size2_74_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_74_configbus0[88:88] = sram_blwl_bl[88:88] ;
assign mux_1level_tapbuf_size2_74_configbus1[88:88] = sram_blwl_wl[88:88] ;
wire [88:88] mux_1level_tapbuf_size2_74_configbus0_b;
assign mux_1level_tapbuf_size2_74_configbus0_b[88:88] = sram_blwl_blb[88:88] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_74_ (mux_1level_tapbuf_size2_74_inbus, chany_1__1__out_28_ , mux_1level_tapbuf_size2_74_sram_blwl_out[88:88] ,
mux_1level_tapbuf_size2_74_sram_blwl_outb[88:88] );
//----- SRAM bits for MUX[74], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_88_ (mux_1level_tapbuf_size2_74_sram_blwl_out[88:88] ,mux_1level_tapbuf_size2_74_sram_blwl_out[88:88] ,mux_1level_tapbuf_size2_74_sram_blwl_outb[88:88] ,mux_1level_tapbuf_size2_74_configbus0[88:88], mux_1level_tapbuf_size2_74_configbus1[88:88] , mux_1level_tapbuf_size2_74_configbus0_b[88:88] );
//----- right side Multiplexers -----
//----- bottom side Multiplexers -----
//----- left side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_75_inbus;
assign mux_1level_tapbuf_size3_75_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size3_75_inbus[1] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size3_75_inbus[2] = chany_1__1__in_1_ ;
wire [89:91] mux_1level_tapbuf_size3_75_configbus0;
wire [89:91] mux_1level_tapbuf_size3_75_configbus1;
wire [89:91] mux_1level_tapbuf_size3_75_sram_blwl_out ;
wire [89:91] mux_1level_tapbuf_size3_75_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_75_configbus0[89:91] = sram_blwl_bl[89:91] ;
assign mux_1level_tapbuf_size3_75_configbus1[89:91] = sram_blwl_wl[89:91] ;
wire [89:91] mux_1level_tapbuf_size3_75_configbus0_b;
assign mux_1level_tapbuf_size3_75_configbus0_b[89:91] = sram_blwl_blb[89:91] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_75_ (mux_1level_tapbuf_size3_75_inbus, chanx_1__0__out_1_ , mux_1level_tapbuf_size3_75_sram_blwl_out[89:91] ,
mux_1level_tapbuf_size3_75_sram_blwl_outb[89:91] );
//----- SRAM bits for MUX[75], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_89_ (mux_1level_tapbuf_size3_75_sram_blwl_out[89:89] ,mux_1level_tapbuf_size3_75_sram_blwl_out[89:89] ,mux_1level_tapbuf_size3_75_sram_blwl_outb[89:89] ,mux_1level_tapbuf_size3_75_configbus0[89:89], mux_1level_tapbuf_size3_75_configbus1[89:89] , mux_1level_tapbuf_size3_75_configbus0_b[89:89] );
sram6T_blwl sram_blwl_90_ (mux_1level_tapbuf_size3_75_sram_blwl_out[90:90] ,mux_1level_tapbuf_size3_75_sram_blwl_out[90:90] ,mux_1level_tapbuf_size3_75_sram_blwl_outb[90:90] ,mux_1level_tapbuf_size3_75_configbus0[90:90], mux_1level_tapbuf_size3_75_configbus1[90:90] , mux_1level_tapbuf_size3_75_configbus0_b[90:90] );
sram6T_blwl sram_blwl_91_ (mux_1level_tapbuf_size3_75_sram_blwl_out[91:91] ,mux_1level_tapbuf_size3_75_sram_blwl_out[91:91] ,mux_1level_tapbuf_size3_75_sram_blwl_outb[91:91] ,mux_1level_tapbuf_size3_75_configbus0[91:91], mux_1level_tapbuf_size3_75_configbus1[91:91] , mux_1level_tapbuf_size3_75_configbus0_b[91:91] );
wire [0:1] mux_1level_tapbuf_size2_76_inbus;
assign mux_1level_tapbuf_size2_76_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_76_inbus[1] = chany_1__1__in_29_ ;
wire [92:92] mux_1level_tapbuf_size2_76_configbus0;
wire [92:92] mux_1level_tapbuf_size2_76_configbus1;
wire [92:92] mux_1level_tapbuf_size2_76_sram_blwl_out ;
wire [92:92] mux_1level_tapbuf_size2_76_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_76_configbus0[92:92] = sram_blwl_bl[92:92] ;
assign mux_1level_tapbuf_size2_76_configbus1[92:92] = sram_blwl_wl[92:92] ;
wire [92:92] mux_1level_tapbuf_size2_76_configbus0_b;
assign mux_1level_tapbuf_size2_76_configbus0_b[92:92] = sram_blwl_blb[92:92] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_76_ (mux_1level_tapbuf_size2_76_inbus, chanx_1__0__out_3_ , mux_1level_tapbuf_size2_76_sram_blwl_out[92:92] ,
mux_1level_tapbuf_size2_76_sram_blwl_outb[92:92] );
//----- SRAM bits for MUX[76], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_92_ (mux_1level_tapbuf_size2_76_sram_blwl_out[92:92] ,mux_1level_tapbuf_size2_76_sram_blwl_out[92:92] ,mux_1level_tapbuf_size2_76_sram_blwl_outb[92:92] ,mux_1level_tapbuf_size2_76_configbus0[92:92], mux_1level_tapbuf_size2_76_configbus1[92:92] , mux_1level_tapbuf_size2_76_configbus0_b[92:92] );
wire [0:1] mux_1level_tapbuf_size2_77_inbus;
assign mux_1level_tapbuf_size2_77_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_77_inbus[1] = chany_1__1__in_27_ ;
wire [93:93] mux_1level_tapbuf_size2_77_configbus0;
wire [93:93] mux_1level_tapbuf_size2_77_configbus1;
wire [93:93] mux_1level_tapbuf_size2_77_sram_blwl_out ;
wire [93:93] mux_1level_tapbuf_size2_77_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_77_configbus0[93:93] = sram_blwl_bl[93:93] ;
assign mux_1level_tapbuf_size2_77_configbus1[93:93] = sram_blwl_wl[93:93] ;
wire [93:93] mux_1level_tapbuf_size2_77_configbus0_b;
assign mux_1level_tapbuf_size2_77_configbus0_b[93:93] = sram_blwl_blb[93:93] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_77_ (mux_1level_tapbuf_size2_77_inbus, chanx_1__0__out_5_ , mux_1level_tapbuf_size2_77_sram_blwl_out[93:93] ,
mux_1level_tapbuf_size2_77_sram_blwl_outb[93:93] );
//----- SRAM bits for MUX[77], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_93_ (mux_1level_tapbuf_size2_77_sram_blwl_out[93:93] ,mux_1level_tapbuf_size2_77_sram_blwl_out[93:93] ,mux_1level_tapbuf_size2_77_sram_blwl_outb[93:93] ,mux_1level_tapbuf_size2_77_configbus0[93:93], mux_1level_tapbuf_size2_77_configbus1[93:93] , mux_1level_tapbuf_size2_77_configbus0_b[93:93] );
wire [0:1] mux_1level_tapbuf_size2_78_inbus;
assign mux_1level_tapbuf_size2_78_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_78_inbus[1] = chany_1__1__in_25_ ;
wire [94:94] mux_1level_tapbuf_size2_78_configbus0;
wire [94:94] mux_1level_tapbuf_size2_78_configbus1;
wire [94:94] mux_1level_tapbuf_size2_78_sram_blwl_out ;
wire [94:94] mux_1level_tapbuf_size2_78_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_78_configbus0[94:94] = sram_blwl_bl[94:94] ;
assign mux_1level_tapbuf_size2_78_configbus1[94:94] = sram_blwl_wl[94:94] ;
wire [94:94] mux_1level_tapbuf_size2_78_configbus0_b;
assign mux_1level_tapbuf_size2_78_configbus0_b[94:94] = sram_blwl_blb[94:94] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_78_ (mux_1level_tapbuf_size2_78_inbus, chanx_1__0__out_7_ , mux_1level_tapbuf_size2_78_sram_blwl_out[94:94] ,
mux_1level_tapbuf_size2_78_sram_blwl_outb[94:94] );
//----- SRAM bits for MUX[78], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_94_ (mux_1level_tapbuf_size2_78_sram_blwl_out[94:94] ,mux_1level_tapbuf_size2_78_sram_blwl_out[94:94] ,mux_1level_tapbuf_size2_78_sram_blwl_outb[94:94] ,mux_1level_tapbuf_size2_78_configbus0[94:94], mux_1level_tapbuf_size2_78_configbus1[94:94] , mux_1level_tapbuf_size2_78_configbus0_b[94:94] );
wire [0:1] mux_1level_tapbuf_size2_79_inbus;
assign mux_1level_tapbuf_size2_79_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_79_inbus[1] = chany_1__1__in_23_ ;
wire [95:95] mux_1level_tapbuf_size2_79_configbus0;
wire [95:95] mux_1level_tapbuf_size2_79_configbus1;
wire [95:95] mux_1level_tapbuf_size2_79_sram_blwl_out ;
wire [95:95] mux_1level_tapbuf_size2_79_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_79_configbus0[95:95] = sram_blwl_bl[95:95] ;
assign mux_1level_tapbuf_size2_79_configbus1[95:95] = sram_blwl_wl[95:95] ;
wire [95:95] mux_1level_tapbuf_size2_79_configbus0_b;
assign mux_1level_tapbuf_size2_79_configbus0_b[95:95] = sram_blwl_blb[95:95] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_79_ (mux_1level_tapbuf_size2_79_inbus, chanx_1__0__out_9_ , mux_1level_tapbuf_size2_79_sram_blwl_out[95:95] ,
mux_1level_tapbuf_size2_79_sram_blwl_outb[95:95] );
//----- SRAM bits for MUX[79], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_95_ (mux_1level_tapbuf_size2_79_sram_blwl_out[95:95] ,mux_1level_tapbuf_size2_79_sram_blwl_out[95:95] ,mux_1level_tapbuf_size2_79_sram_blwl_outb[95:95] ,mux_1level_tapbuf_size2_79_configbus0[95:95], mux_1level_tapbuf_size2_79_configbus1[95:95] , mux_1level_tapbuf_size2_79_configbus0_b[95:95] );
wire [0:1] mux_1level_tapbuf_size2_80_inbus;
assign mux_1level_tapbuf_size2_80_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_80_inbus[1] = chany_1__1__in_21_ ;
wire [96:96] mux_1level_tapbuf_size2_80_configbus0;
wire [96:96] mux_1level_tapbuf_size2_80_configbus1;
wire [96:96] mux_1level_tapbuf_size2_80_sram_blwl_out ;
wire [96:96] mux_1level_tapbuf_size2_80_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_80_configbus0[96:96] = sram_blwl_bl[96:96] ;
assign mux_1level_tapbuf_size2_80_configbus1[96:96] = sram_blwl_wl[96:96] ;
wire [96:96] mux_1level_tapbuf_size2_80_configbus0_b;
assign mux_1level_tapbuf_size2_80_configbus0_b[96:96] = sram_blwl_blb[96:96] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_80_ (mux_1level_tapbuf_size2_80_inbus, chanx_1__0__out_11_ , mux_1level_tapbuf_size2_80_sram_blwl_out[96:96] ,
mux_1level_tapbuf_size2_80_sram_blwl_outb[96:96] );
//----- SRAM bits for MUX[80], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_96_ (mux_1level_tapbuf_size2_80_sram_blwl_out[96:96] ,mux_1level_tapbuf_size2_80_sram_blwl_out[96:96] ,mux_1level_tapbuf_size2_80_sram_blwl_outb[96:96] ,mux_1level_tapbuf_size2_80_configbus0[96:96], mux_1level_tapbuf_size2_80_configbus1[96:96] , mux_1level_tapbuf_size2_80_configbus0_b[96:96] );
wire [0:1] mux_1level_tapbuf_size2_81_inbus;
assign mux_1level_tapbuf_size2_81_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_81_inbus[1] = chany_1__1__in_19_ ;
wire [97:97] mux_1level_tapbuf_size2_81_configbus0;
wire [97:97] mux_1level_tapbuf_size2_81_configbus1;
wire [97:97] mux_1level_tapbuf_size2_81_sram_blwl_out ;
wire [97:97] mux_1level_tapbuf_size2_81_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_81_configbus0[97:97] = sram_blwl_bl[97:97] ;
assign mux_1level_tapbuf_size2_81_configbus1[97:97] = sram_blwl_wl[97:97] ;
wire [97:97] mux_1level_tapbuf_size2_81_configbus0_b;
assign mux_1level_tapbuf_size2_81_configbus0_b[97:97] = sram_blwl_blb[97:97] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_81_ (mux_1level_tapbuf_size2_81_inbus, chanx_1__0__out_13_ , mux_1level_tapbuf_size2_81_sram_blwl_out[97:97] ,
mux_1level_tapbuf_size2_81_sram_blwl_outb[97:97] );
//----- SRAM bits for MUX[81], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_97_ (mux_1level_tapbuf_size2_81_sram_blwl_out[97:97] ,mux_1level_tapbuf_size2_81_sram_blwl_out[97:97] ,mux_1level_tapbuf_size2_81_sram_blwl_outb[97:97] ,mux_1level_tapbuf_size2_81_configbus0[97:97], mux_1level_tapbuf_size2_81_configbus1[97:97] , mux_1level_tapbuf_size2_81_configbus0_b[97:97] );
wire [0:1] mux_1level_tapbuf_size2_82_inbus;
assign mux_1level_tapbuf_size2_82_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_82_inbus[1] = chany_1__1__in_17_ ;
wire [98:98] mux_1level_tapbuf_size2_82_configbus0;
wire [98:98] mux_1level_tapbuf_size2_82_configbus1;
wire [98:98] mux_1level_tapbuf_size2_82_sram_blwl_out ;
wire [98:98] mux_1level_tapbuf_size2_82_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_82_configbus0[98:98] = sram_blwl_bl[98:98] ;
assign mux_1level_tapbuf_size2_82_configbus1[98:98] = sram_blwl_wl[98:98] ;
wire [98:98] mux_1level_tapbuf_size2_82_configbus0_b;
assign mux_1level_tapbuf_size2_82_configbus0_b[98:98] = sram_blwl_blb[98:98] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_82_ (mux_1level_tapbuf_size2_82_inbus, chanx_1__0__out_15_ , mux_1level_tapbuf_size2_82_sram_blwl_out[98:98] ,
mux_1level_tapbuf_size2_82_sram_blwl_outb[98:98] );
//----- SRAM bits for MUX[82], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_98_ (mux_1level_tapbuf_size2_82_sram_blwl_out[98:98] ,mux_1level_tapbuf_size2_82_sram_blwl_out[98:98] ,mux_1level_tapbuf_size2_82_sram_blwl_outb[98:98] ,mux_1level_tapbuf_size2_82_configbus0[98:98], mux_1level_tapbuf_size2_82_configbus1[98:98] , mux_1level_tapbuf_size2_82_configbus0_b[98:98] );
wire [0:1] mux_1level_tapbuf_size2_83_inbus;
assign mux_1level_tapbuf_size2_83_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_83_inbus[1] = chany_1__1__in_15_ ;
wire [99:99] mux_1level_tapbuf_size2_83_configbus0;
wire [99:99] mux_1level_tapbuf_size2_83_configbus1;
wire [99:99] mux_1level_tapbuf_size2_83_sram_blwl_out ;
wire [99:99] mux_1level_tapbuf_size2_83_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_83_configbus0[99:99] = sram_blwl_bl[99:99] ;
assign mux_1level_tapbuf_size2_83_configbus1[99:99] = sram_blwl_wl[99:99] ;
wire [99:99] mux_1level_tapbuf_size2_83_configbus0_b;
assign mux_1level_tapbuf_size2_83_configbus0_b[99:99] = sram_blwl_blb[99:99] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_83_ (mux_1level_tapbuf_size2_83_inbus, chanx_1__0__out_17_ , mux_1level_tapbuf_size2_83_sram_blwl_out[99:99] ,
mux_1level_tapbuf_size2_83_sram_blwl_outb[99:99] );
//----- SRAM bits for MUX[83], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_99_ (mux_1level_tapbuf_size2_83_sram_blwl_out[99:99] ,mux_1level_tapbuf_size2_83_sram_blwl_out[99:99] ,mux_1level_tapbuf_size2_83_sram_blwl_outb[99:99] ,mux_1level_tapbuf_size2_83_configbus0[99:99], mux_1level_tapbuf_size2_83_configbus1[99:99] , mux_1level_tapbuf_size2_83_configbus0_b[99:99] );
wire [0:1] mux_1level_tapbuf_size2_84_inbus;
assign mux_1level_tapbuf_size2_84_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_84_inbus[1] = chany_1__1__in_13_ ;
wire [100:100] mux_1level_tapbuf_size2_84_configbus0;
wire [100:100] mux_1level_tapbuf_size2_84_configbus1;
wire [100:100] mux_1level_tapbuf_size2_84_sram_blwl_out ;
wire [100:100] mux_1level_tapbuf_size2_84_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_84_configbus0[100:100] = sram_blwl_bl[100:100] ;
assign mux_1level_tapbuf_size2_84_configbus1[100:100] = sram_blwl_wl[100:100] ;
wire [100:100] mux_1level_tapbuf_size2_84_configbus0_b;
assign mux_1level_tapbuf_size2_84_configbus0_b[100:100] = sram_blwl_blb[100:100] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_84_ (mux_1level_tapbuf_size2_84_inbus, chanx_1__0__out_19_ , mux_1level_tapbuf_size2_84_sram_blwl_out[100:100] ,
mux_1level_tapbuf_size2_84_sram_blwl_outb[100:100] );
//----- SRAM bits for MUX[84], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_100_ (mux_1level_tapbuf_size2_84_sram_blwl_out[100:100] ,mux_1level_tapbuf_size2_84_sram_blwl_out[100:100] ,mux_1level_tapbuf_size2_84_sram_blwl_outb[100:100] ,mux_1level_tapbuf_size2_84_configbus0[100:100], mux_1level_tapbuf_size2_84_configbus1[100:100] , mux_1level_tapbuf_size2_84_configbus0_b[100:100] );
wire [0:1] mux_1level_tapbuf_size2_85_inbus;
assign mux_1level_tapbuf_size2_85_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_85_inbus[1] = chany_1__1__in_11_ ;
wire [101:101] mux_1level_tapbuf_size2_85_configbus0;
wire [101:101] mux_1level_tapbuf_size2_85_configbus1;
wire [101:101] mux_1level_tapbuf_size2_85_sram_blwl_out ;
wire [101:101] mux_1level_tapbuf_size2_85_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_85_configbus0[101:101] = sram_blwl_bl[101:101] ;
assign mux_1level_tapbuf_size2_85_configbus1[101:101] = sram_blwl_wl[101:101] ;
wire [101:101] mux_1level_tapbuf_size2_85_configbus0_b;
assign mux_1level_tapbuf_size2_85_configbus0_b[101:101] = sram_blwl_blb[101:101] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_85_ (mux_1level_tapbuf_size2_85_inbus, chanx_1__0__out_21_ , mux_1level_tapbuf_size2_85_sram_blwl_out[101:101] ,
mux_1level_tapbuf_size2_85_sram_blwl_outb[101:101] );
//----- SRAM bits for MUX[85], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_101_ (mux_1level_tapbuf_size2_85_sram_blwl_out[101:101] ,mux_1level_tapbuf_size2_85_sram_blwl_out[101:101] ,mux_1level_tapbuf_size2_85_sram_blwl_outb[101:101] ,mux_1level_tapbuf_size2_85_configbus0[101:101], mux_1level_tapbuf_size2_85_configbus1[101:101] , mux_1level_tapbuf_size2_85_configbus0_b[101:101] );
wire [0:1] mux_1level_tapbuf_size2_86_inbus;
assign mux_1level_tapbuf_size2_86_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_86_inbus[1] = chany_1__1__in_9_ ;
wire [102:102] mux_1level_tapbuf_size2_86_configbus0;
wire [102:102] mux_1level_tapbuf_size2_86_configbus1;
wire [102:102] mux_1level_tapbuf_size2_86_sram_blwl_out ;
wire [102:102] mux_1level_tapbuf_size2_86_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_86_configbus0[102:102] = sram_blwl_bl[102:102] ;
assign mux_1level_tapbuf_size2_86_configbus1[102:102] = sram_blwl_wl[102:102] ;
wire [102:102] mux_1level_tapbuf_size2_86_configbus0_b;
assign mux_1level_tapbuf_size2_86_configbus0_b[102:102] = sram_blwl_blb[102:102] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_86_ (mux_1level_tapbuf_size2_86_inbus, chanx_1__0__out_23_ , mux_1level_tapbuf_size2_86_sram_blwl_out[102:102] ,
mux_1level_tapbuf_size2_86_sram_blwl_outb[102:102] );
//----- SRAM bits for MUX[86], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_102_ (mux_1level_tapbuf_size2_86_sram_blwl_out[102:102] ,mux_1level_tapbuf_size2_86_sram_blwl_out[102:102] ,mux_1level_tapbuf_size2_86_sram_blwl_outb[102:102] ,mux_1level_tapbuf_size2_86_configbus0[102:102], mux_1level_tapbuf_size2_86_configbus1[102:102] , mux_1level_tapbuf_size2_86_configbus0_b[102:102] );
wire [0:1] mux_1level_tapbuf_size2_87_inbus;
assign mux_1level_tapbuf_size2_87_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_87_inbus[1] = chany_1__1__in_7_ ;
wire [103:103] mux_1level_tapbuf_size2_87_configbus0;
wire [103:103] mux_1level_tapbuf_size2_87_configbus1;
wire [103:103] mux_1level_tapbuf_size2_87_sram_blwl_out ;
wire [103:103] mux_1level_tapbuf_size2_87_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_87_configbus0[103:103] = sram_blwl_bl[103:103] ;
assign mux_1level_tapbuf_size2_87_configbus1[103:103] = sram_blwl_wl[103:103] ;
wire [103:103] mux_1level_tapbuf_size2_87_configbus0_b;
assign mux_1level_tapbuf_size2_87_configbus0_b[103:103] = sram_blwl_blb[103:103] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_87_ (mux_1level_tapbuf_size2_87_inbus, chanx_1__0__out_25_ , mux_1level_tapbuf_size2_87_sram_blwl_out[103:103] ,
mux_1level_tapbuf_size2_87_sram_blwl_outb[103:103] );
//----- SRAM bits for MUX[87], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_103_ (mux_1level_tapbuf_size2_87_sram_blwl_out[103:103] ,mux_1level_tapbuf_size2_87_sram_blwl_out[103:103] ,mux_1level_tapbuf_size2_87_sram_blwl_outb[103:103] ,mux_1level_tapbuf_size2_87_configbus0[103:103], mux_1level_tapbuf_size2_87_configbus1[103:103] , mux_1level_tapbuf_size2_87_configbus0_b[103:103] );
wire [0:1] mux_1level_tapbuf_size2_88_inbus;
assign mux_1level_tapbuf_size2_88_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_88_inbus[1] = chany_1__1__in_5_ ;
wire [104:104] mux_1level_tapbuf_size2_88_configbus0;
wire [104:104] mux_1level_tapbuf_size2_88_configbus1;
wire [104:104] mux_1level_tapbuf_size2_88_sram_blwl_out ;
wire [104:104] mux_1level_tapbuf_size2_88_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_88_configbus0[104:104] = sram_blwl_bl[104:104] ;
assign mux_1level_tapbuf_size2_88_configbus1[104:104] = sram_blwl_wl[104:104] ;
wire [104:104] mux_1level_tapbuf_size2_88_configbus0_b;
assign mux_1level_tapbuf_size2_88_configbus0_b[104:104] = sram_blwl_blb[104:104] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_88_ (mux_1level_tapbuf_size2_88_inbus, chanx_1__0__out_27_ , mux_1level_tapbuf_size2_88_sram_blwl_out[104:104] ,
mux_1level_tapbuf_size2_88_sram_blwl_outb[104:104] );
//----- SRAM bits for MUX[88], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_104_ (mux_1level_tapbuf_size2_88_sram_blwl_out[104:104] ,mux_1level_tapbuf_size2_88_sram_blwl_out[104:104] ,mux_1level_tapbuf_size2_88_sram_blwl_outb[104:104] ,mux_1level_tapbuf_size2_88_configbus0[104:104], mux_1level_tapbuf_size2_88_configbus1[104:104] , mux_1level_tapbuf_size2_88_configbus0_b[104:104] );
wire [0:1] mux_1level_tapbuf_size2_89_inbus;
assign mux_1level_tapbuf_size2_89_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_89_inbus[1] = chany_1__1__in_3_ ;
wire [105:105] mux_1level_tapbuf_size2_89_configbus0;
wire [105:105] mux_1level_tapbuf_size2_89_configbus1;
wire [105:105] mux_1level_tapbuf_size2_89_sram_blwl_out ;
wire [105:105] mux_1level_tapbuf_size2_89_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_89_configbus0[105:105] = sram_blwl_bl[105:105] ;
assign mux_1level_tapbuf_size2_89_configbus1[105:105] = sram_blwl_wl[105:105] ;
wire [105:105] mux_1level_tapbuf_size2_89_configbus0_b;
assign mux_1level_tapbuf_size2_89_configbus0_b[105:105] = sram_blwl_blb[105:105] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_89_ (mux_1level_tapbuf_size2_89_inbus, chanx_1__0__out_29_ , mux_1level_tapbuf_size2_89_sram_blwl_out[105:105] ,
mux_1level_tapbuf_size2_89_sram_blwl_outb[105:105] );
//----- SRAM bits for MUX[89], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_105_ (mux_1level_tapbuf_size2_89_sram_blwl_out[105:105] ,mux_1level_tapbuf_size2_89_sram_blwl_out[105:105] ,mux_1level_tapbuf_size2_89_sram_blwl_outb[105:105] ,mux_1level_tapbuf_size2_89_configbus0[105:105], mux_1level_tapbuf_size2_89_configbus1[105:105] , mux_1level_tapbuf_size2_89_configbus0_b[105:105] );
endmodule
//----- END Verilog Module of Switch Box[1][0] -----

