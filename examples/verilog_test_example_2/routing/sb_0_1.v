//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Switch Block  [0][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
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
  output chanx_1__1__out_30_,
  input chanx_1__1__in_31_,
  output chanx_1__1__out_32_,
  input chanx_1__1__in_33_,
  output chanx_1__1__out_34_,
  input chanx_1__1__in_35_,
  output chanx_1__1__out_36_,
  input chanx_1__1__in_37_,
  output chanx_1__1__out_38_,
  input chanx_1__1__in_39_,
  output chanx_1__1__out_40_,
  input chanx_1__1__in_41_,
  output chanx_1__1__out_42_,
  input chanx_1__1__in_43_,
  output chanx_1__1__out_44_,
  input chanx_1__1__in_45_,
  output chanx_1__1__out_46_,
  input chanx_1__1__in_47_,
  output chanx_1__1__out_48_,
  input chanx_1__1__in_49_,
  output chanx_1__1__out_50_,
  input chanx_1__1__in_51_,
  output chanx_1__1__out_52_,
  input chanx_1__1__in_53_,
  output chanx_1__1__out_54_,
  input chanx_1__1__in_55_,
  output chanx_1__1__out_56_,
  input chanx_1__1__in_57_,
  output chanx_1__1__out_58_,
  input chanx_1__1__in_59_,
  output chanx_1__1__out_60_,
  input chanx_1__1__in_61_,
  output chanx_1__1__out_62_,
  input chanx_1__1__in_63_,
  output chanx_1__1__out_64_,
  input chanx_1__1__in_65_,
  output chanx_1__1__out_66_,
  input chanx_1__1__in_67_,
  output chanx_1__1__out_68_,
  input chanx_1__1__in_69_,
  output chanx_1__1__out_70_,
  input chanx_1__1__in_71_,
  output chanx_1__1__out_72_,
  input chanx_1__1__in_73_,
  output chanx_1__1__out_74_,
  input chanx_1__1__in_75_,
  output chanx_1__1__out_76_,
  input chanx_1__1__in_77_,
  output chanx_1__1__out_78_,
  input chanx_1__1__in_79_,
  output chanx_1__1__out_80_,
  input chanx_1__1__in_81_,
  output chanx_1__1__out_82_,
  input chanx_1__1__in_83_,
  output chanx_1__1__out_84_,
  input chanx_1__1__in_85_,
  output chanx_1__1__out_86_,
  input chanx_1__1__in_87_,
  output chanx_1__1__out_88_,
  input chanx_1__1__in_89_,
  output chanx_1__1__out_90_,
  input chanx_1__1__in_91_,
  output chanx_1__1__out_92_,
  input chanx_1__1__in_93_,
  output chanx_1__1__out_94_,
  input chanx_1__1__in_95_,
  output chanx_1__1__out_96_,
  input chanx_1__1__in_97_,
  output chanx_1__1__out_98_,
  input chanx_1__1__in_99_,
input  grid_1__2__pin_0__2__1_,
input  grid_1__2__pin_0__2__3_,
input  grid_1__2__pin_0__2__5_,
input  grid_1__2__pin_0__2__7_,
input  grid_1__2__pin_0__2__9_,
input  grid_1__2__pin_0__2__11_,
input  grid_1__2__pin_0__2__13_,
input  grid_1__2__pin_0__2__15_,
input  grid_1__1__pin_0__0__40_,
input  grid_1__1__pin_0__0__44_,
input  grid_1__1__pin_0__0__48_,
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
  input chany_0__1__in_30_,
  output chany_0__1__out_31_,
  input chany_0__1__in_32_,
  output chany_0__1__out_33_,
  input chany_0__1__in_34_,
  output chany_0__1__out_35_,
  input chany_0__1__in_36_,
  output chany_0__1__out_37_,
  input chany_0__1__in_38_,
  output chany_0__1__out_39_,
  input chany_0__1__in_40_,
  output chany_0__1__out_41_,
  input chany_0__1__in_42_,
  output chany_0__1__out_43_,
  input chany_0__1__in_44_,
  output chany_0__1__out_45_,
  input chany_0__1__in_46_,
  output chany_0__1__out_47_,
  input chany_0__1__in_48_,
  output chany_0__1__out_49_,
  input chany_0__1__in_50_,
  output chany_0__1__out_51_,
  input chany_0__1__in_52_,
  output chany_0__1__out_53_,
  input chany_0__1__in_54_,
  output chany_0__1__out_55_,
  input chany_0__1__in_56_,
  output chany_0__1__out_57_,
  input chany_0__1__in_58_,
  output chany_0__1__out_59_,
  input chany_0__1__in_60_,
  output chany_0__1__out_61_,
  input chany_0__1__in_62_,
  output chany_0__1__out_63_,
  input chany_0__1__in_64_,
  output chany_0__1__out_65_,
  input chany_0__1__in_66_,
  output chany_0__1__out_67_,
  input chany_0__1__in_68_,
  output chany_0__1__out_69_,
  input chany_0__1__in_70_,
  output chany_0__1__out_71_,
  input chany_0__1__in_72_,
  output chany_0__1__out_73_,
  input chany_0__1__in_74_,
  output chany_0__1__out_75_,
  input chany_0__1__in_76_,
  output chany_0__1__out_77_,
  input chany_0__1__in_78_,
  output chany_0__1__out_79_,
  input chany_0__1__in_80_,
  output chany_0__1__out_81_,
  input chany_0__1__in_82_,
  output chany_0__1__out_83_,
  input chany_0__1__in_84_,
  output chany_0__1__out_85_,
  input chany_0__1__in_86_,
  output chany_0__1__out_87_,
  input chany_0__1__in_88_,
  output chany_0__1__out_89_,
  input chany_0__1__in_90_,
  output chany_0__1__out_91_,
  input chany_0__1__in_92_,
  output chany_0__1__out_93_,
  input chany_0__1__in_94_,
  output chany_0__1__out_95_,
  input chany_0__1__in_96_,
  output chany_0__1__out_97_,
  input chany_0__1__in_98_,
  output chany_0__1__out_99_,
input  grid_1__1__pin_0__3__43_,
input  grid_1__1__pin_0__3__47_,
input  grid_0__1__pin_0__1__1_,
input  grid_0__1__pin_0__1__3_,
input  grid_0__1__pin_0__1__5_,
input  grid_0__1__pin_0__1__7_,
input  grid_0__1__pin_0__1__9_,
input  grid_0__1__pin_0__1__11_,
input  grid_0__1__pin_0__1__13_,
input  grid_0__1__pin_0__1__15_,
//----- Inputs/outputs of left side -----
input [100:209] sram_blwl_bl ,
input [100:209] sram_blwl_wl ,
input [100:209] sram_blwl_blb ); 
//----- top side Multiplexers -----
//----- right side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_100_inbus;
assign mux_1level_tapbuf_size3_100_inbus[0] =  grid_1__1__pin_0__0__40_;
assign mux_1level_tapbuf_size3_100_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_100_inbus[2] = chany_0__1__in_96_ ;
wire [100:102] mux_1level_tapbuf_size3_100_configbus0;
wire [100:102] mux_1level_tapbuf_size3_100_configbus1;
wire [100:102] mux_1level_tapbuf_size3_100_sram_blwl_out ;
wire [100:102] mux_1level_tapbuf_size3_100_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_100_configbus0[100:102] = sram_blwl_bl[100:102] ;
assign mux_1level_tapbuf_size3_100_configbus1[100:102] = sram_blwl_wl[100:102] ;
wire [100:102] mux_1level_tapbuf_size3_100_configbus0_b;
assign mux_1level_tapbuf_size3_100_configbus0_b[100:102] = sram_blwl_blb[100:102] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_100_ (mux_1level_tapbuf_size3_100_inbus, chanx_1__1__out_0_ , mux_1level_tapbuf_size3_100_sram_blwl_out[100:102] ,
mux_1level_tapbuf_size3_100_sram_blwl_outb[100:102] );
//----- SRAM bits for MUX[100], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_100_ (mux_1level_tapbuf_size3_100_sram_blwl_out[100:100] ,mux_1level_tapbuf_size3_100_sram_blwl_out[100:100] ,mux_1level_tapbuf_size3_100_sram_blwl_outb[100:100] ,mux_1level_tapbuf_size3_100_configbus0[100:100], mux_1level_tapbuf_size3_100_configbus1[100:100] , mux_1level_tapbuf_size3_100_configbus0_b[100:100] );
sram6T_blwl sram_blwl_101_ (mux_1level_tapbuf_size3_100_sram_blwl_out[101:101] ,mux_1level_tapbuf_size3_100_sram_blwl_out[101:101] ,mux_1level_tapbuf_size3_100_sram_blwl_outb[101:101] ,mux_1level_tapbuf_size3_100_configbus0[101:101], mux_1level_tapbuf_size3_100_configbus1[101:101] , mux_1level_tapbuf_size3_100_configbus0_b[101:101] );
sram6T_blwl sram_blwl_102_ (mux_1level_tapbuf_size3_100_sram_blwl_out[102:102] ,mux_1level_tapbuf_size3_100_sram_blwl_out[102:102] ,mux_1level_tapbuf_size3_100_sram_blwl_outb[102:102] ,mux_1level_tapbuf_size3_100_configbus0[102:102], mux_1level_tapbuf_size3_100_configbus1[102:102] , mux_1level_tapbuf_size3_100_configbus0_b[102:102] );
wire [0:2] mux_1level_tapbuf_size3_101_inbus;
assign mux_1level_tapbuf_size3_101_inbus[0] =  grid_1__1__pin_0__0__40_;
assign mux_1level_tapbuf_size3_101_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_101_inbus[2] = chany_0__1__in_94_ ;
wire [103:105] mux_1level_tapbuf_size3_101_configbus0;
wire [103:105] mux_1level_tapbuf_size3_101_configbus1;
wire [103:105] mux_1level_tapbuf_size3_101_sram_blwl_out ;
wire [103:105] mux_1level_tapbuf_size3_101_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_101_configbus0[103:105] = sram_blwl_bl[103:105] ;
assign mux_1level_tapbuf_size3_101_configbus1[103:105] = sram_blwl_wl[103:105] ;
wire [103:105] mux_1level_tapbuf_size3_101_configbus0_b;
assign mux_1level_tapbuf_size3_101_configbus0_b[103:105] = sram_blwl_blb[103:105] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_101_ (mux_1level_tapbuf_size3_101_inbus, chanx_1__1__out_2_ , mux_1level_tapbuf_size3_101_sram_blwl_out[103:105] ,
mux_1level_tapbuf_size3_101_sram_blwl_outb[103:105] );
//----- SRAM bits for MUX[101], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_103_ (mux_1level_tapbuf_size3_101_sram_blwl_out[103:103] ,mux_1level_tapbuf_size3_101_sram_blwl_out[103:103] ,mux_1level_tapbuf_size3_101_sram_blwl_outb[103:103] ,mux_1level_tapbuf_size3_101_configbus0[103:103], mux_1level_tapbuf_size3_101_configbus1[103:103] , mux_1level_tapbuf_size3_101_configbus0_b[103:103] );
sram6T_blwl sram_blwl_104_ (mux_1level_tapbuf_size3_101_sram_blwl_out[104:104] ,mux_1level_tapbuf_size3_101_sram_blwl_out[104:104] ,mux_1level_tapbuf_size3_101_sram_blwl_outb[104:104] ,mux_1level_tapbuf_size3_101_configbus0[104:104], mux_1level_tapbuf_size3_101_configbus1[104:104] , mux_1level_tapbuf_size3_101_configbus0_b[104:104] );
sram6T_blwl sram_blwl_105_ (mux_1level_tapbuf_size3_101_sram_blwl_out[105:105] ,mux_1level_tapbuf_size3_101_sram_blwl_out[105:105] ,mux_1level_tapbuf_size3_101_sram_blwl_outb[105:105] ,mux_1level_tapbuf_size3_101_configbus0[105:105], mux_1level_tapbuf_size3_101_configbus1[105:105] , mux_1level_tapbuf_size3_101_configbus0_b[105:105] );
wire [0:2] mux_1level_tapbuf_size3_102_inbus;
assign mux_1level_tapbuf_size3_102_inbus[0] =  grid_1__1__pin_0__0__40_;
assign mux_1level_tapbuf_size3_102_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_102_inbus[2] = chany_0__1__in_92_ ;
wire [106:108] mux_1level_tapbuf_size3_102_configbus0;
wire [106:108] mux_1level_tapbuf_size3_102_configbus1;
wire [106:108] mux_1level_tapbuf_size3_102_sram_blwl_out ;
wire [106:108] mux_1level_tapbuf_size3_102_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_102_configbus0[106:108] = sram_blwl_bl[106:108] ;
assign mux_1level_tapbuf_size3_102_configbus1[106:108] = sram_blwl_wl[106:108] ;
wire [106:108] mux_1level_tapbuf_size3_102_configbus0_b;
assign mux_1level_tapbuf_size3_102_configbus0_b[106:108] = sram_blwl_blb[106:108] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_102_ (mux_1level_tapbuf_size3_102_inbus, chanx_1__1__out_4_ , mux_1level_tapbuf_size3_102_sram_blwl_out[106:108] ,
mux_1level_tapbuf_size3_102_sram_blwl_outb[106:108] );
//----- SRAM bits for MUX[102], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_106_ (mux_1level_tapbuf_size3_102_sram_blwl_out[106:106] ,mux_1level_tapbuf_size3_102_sram_blwl_out[106:106] ,mux_1level_tapbuf_size3_102_sram_blwl_outb[106:106] ,mux_1level_tapbuf_size3_102_configbus0[106:106], mux_1level_tapbuf_size3_102_configbus1[106:106] , mux_1level_tapbuf_size3_102_configbus0_b[106:106] );
sram6T_blwl sram_blwl_107_ (mux_1level_tapbuf_size3_102_sram_blwl_out[107:107] ,mux_1level_tapbuf_size3_102_sram_blwl_out[107:107] ,mux_1level_tapbuf_size3_102_sram_blwl_outb[107:107] ,mux_1level_tapbuf_size3_102_configbus0[107:107], mux_1level_tapbuf_size3_102_configbus1[107:107] , mux_1level_tapbuf_size3_102_configbus0_b[107:107] );
sram6T_blwl sram_blwl_108_ (mux_1level_tapbuf_size3_102_sram_blwl_out[108:108] ,mux_1level_tapbuf_size3_102_sram_blwl_out[108:108] ,mux_1level_tapbuf_size3_102_sram_blwl_outb[108:108] ,mux_1level_tapbuf_size3_102_configbus0[108:108], mux_1level_tapbuf_size3_102_configbus1[108:108] , mux_1level_tapbuf_size3_102_configbus0_b[108:108] );
wire [0:2] mux_1level_tapbuf_size3_103_inbus;
assign mux_1level_tapbuf_size3_103_inbus[0] =  grid_1__1__pin_0__0__40_;
assign mux_1level_tapbuf_size3_103_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_103_inbus[2] = chany_0__1__in_90_ ;
wire [109:111] mux_1level_tapbuf_size3_103_configbus0;
wire [109:111] mux_1level_tapbuf_size3_103_configbus1;
wire [109:111] mux_1level_tapbuf_size3_103_sram_blwl_out ;
wire [109:111] mux_1level_tapbuf_size3_103_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_103_configbus0[109:111] = sram_blwl_bl[109:111] ;
assign mux_1level_tapbuf_size3_103_configbus1[109:111] = sram_blwl_wl[109:111] ;
wire [109:111] mux_1level_tapbuf_size3_103_configbus0_b;
assign mux_1level_tapbuf_size3_103_configbus0_b[109:111] = sram_blwl_blb[109:111] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_103_ (mux_1level_tapbuf_size3_103_inbus, chanx_1__1__out_6_ , mux_1level_tapbuf_size3_103_sram_blwl_out[109:111] ,
mux_1level_tapbuf_size3_103_sram_blwl_outb[109:111] );
//----- SRAM bits for MUX[103], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_109_ (mux_1level_tapbuf_size3_103_sram_blwl_out[109:109] ,mux_1level_tapbuf_size3_103_sram_blwl_out[109:109] ,mux_1level_tapbuf_size3_103_sram_blwl_outb[109:109] ,mux_1level_tapbuf_size3_103_configbus0[109:109], mux_1level_tapbuf_size3_103_configbus1[109:109] , mux_1level_tapbuf_size3_103_configbus0_b[109:109] );
sram6T_blwl sram_blwl_110_ (mux_1level_tapbuf_size3_103_sram_blwl_out[110:110] ,mux_1level_tapbuf_size3_103_sram_blwl_out[110:110] ,mux_1level_tapbuf_size3_103_sram_blwl_outb[110:110] ,mux_1level_tapbuf_size3_103_configbus0[110:110], mux_1level_tapbuf_size3_103_configbus1[110:110] , mux_1level_tapbuf_size3_103_configbus0_b[110:110] );
sram6T_blwl sram_blwl_111_ (mux_1level_tapbuf_size3_103_sram_blwl_out[111:111] ,mux_1level_tapbuf_size3_103_sram_blwl_out[111:111] ,mux_1level_tapbuf_size3_103_sram_blwl_outb[111:111] ,mux_1level_tapbuf_size3_103_configbus0[111:111], mux_1level_tapbuf_size3_103_configbus1[111:111] , mux_1level_tapbuf_size3_103_configbus0_b[111:111] );
wire [0:2] mux_1level_tapbuf_size3_104_inbus;
assign mux_1level_tapbuf_size3_104_inbus[0] =  grid_1__1__pin_0__0__40_;
assign mux_1level_tapbuf_size3_104_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_104_inbus[2] = chany_0__1__in_88_ ;
wire [112:114] mux_1level_tapbuf_size3_104_configbus0;
wire [112:114] mux_1level_tapbuf_size3_104_configbus1;
wire [112:114] mux_1level_tapbuf_size3_104_sram_blwl_out ;
wire [112:114] mux_1level_tapbuf_size3_104_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_104_configbus0[112:114] = sram_blwl_bl[112:114] ;
assign mux_1level_tapbuf_size3_104_configbus1[112:114] = sram_blwl_wl[112:114] ;
wire [112:114] mux_1level_tapbuf_size3_104_configbus0_b;
assign mux_1level_tapbuf_size3_104_configbus0_b[112:114] = sram_blwl_blb[112:114] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_104_ (mux_1level_tapbuf_size3_104_inbus, chanx_1__1__out_8_ , mux_1level_tapbuf_size3_104_sram_blwl_out[112:114] ,
mux_1level_tapbuf_size3_104_sram_blwl_outb[112:114] );
//----- SRAM bits for MUX[104], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_112_ (mux_1level_tapbuf_size3_104_sram_blwl_out[112:112] ,mux_1level_tapbuf_size3_104_sram_blwl_out[112:112] ,mux_1level_tapbuf_size3_104_sram_blwl_outb[112:112] ,mux_1level_tapbuf_size3_104_configbus0[112:112], mux_1level_tapbuf_size3_104_configbus1[112:112] , mux_1level_tapbuf_size3_104_configbus0_b[112:112] );
sram6T_blwl sram_blwl_113_ (mux_1level_tapbuf_size3_104_sram_blwl_out[113:113] ,mux_1level_tapbuf_size3_104_sram_blwl_out[113:113] ,mux_1level_tapbuf_size3_104_sram_blwl_outb[113:113] ,mux_1level_tapbuf_size3_104_configbus0[113:113], mux_1level_tapbuf_size3_104_configbus1[113:113] , mux_1level_tapbuf_size3_104_configbus0_b[113:113] );
sram6T_blwl sram_blwl_114_ (mux_1level_tapbuf_size3_104_sram_blwl_out[114:114] ,mux_1level_tapbuf_size3_104_sram_blwl_out[114:114] ,mux_1level_tapbuf_size3_104_sram_blwl_outb[114:114] ,mux_1level_tapbuf_size3_104_configbus0[114:114], mux_1level_tapbuf_size3_104_configbus1[114:114] , mux_1level_tapbuf_size3_104_configbus0_b[114:114] );
wire [0:1] mux_1level_tapbuf_size2_105_inbus;
assign mux_1level_tapbuf_size2_105_inbus[0] =  grid_1__1__pin_0__0__44_;
assign mux_1level_tapbuf_size2_105_inbus[1] = chany_0__1__in_86_ ;
wire [115:115] mux_1level_tapbuf_size2_105_configbus0;
wire [115:115] mux_1level_tapbuf_size2_105_configbus1;
wire [115:115] mux_1level_tapbuf_size2_105_sram_blwl_out ;
wire [115:115] mux_1level_tapbuf_size2_105_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_105_configbus0[115:115] = sram_blwl_bl[115:115] ;
assign mux_1level_tapbuf_size2_105_configbus1[115:115] = sram_blwl_wl[115:115] ;
wire [115:115] mux_1level_tapbuf_size2_105_configbus0_b;
assign mux_1level_tapbuf_size2_105_configbus0_b[115:115] = sram_blwl_blb[115:115] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_105_ (mux_1level_tapbuf_size2_105_inbus, chanx_1__1__out_10_ , mux_1level_tapbuf_size2_105_sram_blwl_out[115:115] ,
mux_1level_tapbuf_size2_105_sram_blwl_outb[115:115] );
//----- SRAM bits for MUX[105], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_115_ (mux_1level_tapbuf_size2_105_sram_blwl_out[115:115] ,mux_1level_tapbuf_size2_105_sram_blwl_out[115:115] ,mux_1level_tapbuf_size2_105_sram_blwl_outb[115:115] ,mux_1level_tapbuf_size2_105_configbus0[115:115], mux_1level_tapbuf_size2_105_configbus1[115:115] , mux_1level_tapbuf_size2_105_configbus0_b[115:115] );
wire [0:1] mux_1level_tapbuf_size2_106_inbus;
assign mux_1level_tapbuf_size2_106_inbus[0] =  grid_1__1__pin_0__0__44_;
assign mux_1level_tapbuf_size2_106_inbus[1] = chany_0__1__in_84_ ;
wire [116:116] mux_1level_tapbuf_size2_106_configbus0;
wire [116:116] mux_1level_tapbuf_size2_106_configbus1;
wire [116:116] mux_1level_tapbuf_size2_106_sram_blwl_out ;
wire [116:116] mux_1level_tapbuf_size2_106_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_106_configbus0[116:116] = sram_blwl_bl[116:116] ;
assign mux_1level_tapbuf_size2_106_configbus1[116:116] = sram_blwl_wl[116:116] ;
wire [116:116] mux_1level_tapbuf_size2_106_configbus0_b;
assign mux_1level_tapbuf_size2_106_configbus0_b[116:116] = sram_blwl_blb[116:116] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_106_ (mux_1level_tapbuf_size2_106_inbus, chanx_1__1__out_12_ , mux_1level_tapbuf_size2_106_sram_blwl_out[116:116] ,
mux_1level_tapbuf_size2_106_sram_blwl_outb[116:116] );
//----- SRAM bits for MUX[106], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_116_ (mux_1level_tapbuf_size2_106_sram_blwl_out[116:116] ,mux_1level_tapbuf_size2_106_sram_blwl_out[116:116] ,mux_1level_tapbuf_size2_106_sram_blwl_outb[116:116] ,mux_1level_tapbuf_size2_106_configbus0[116:116], mux_1level_tapbuf_size2_106_configbus1[116:116] , mux_1level_tapbuf_size2_106_configbus0_b[116:116] );
wire [0:1] mux_1level_tapbuf_size2_107_inbus;
assign mux_1level_tapbuf_size2_107_inbus[0] =  grid_1__1__pin_0__0__44_;
assign mux_1level_tapbuf_size2_107_inbus[1] = chany_0__1__in_82_ ;
wire [117:117] mux_1level_tapbuf_size2_107_configbus0;
wire [117:117] mux_1level_tapbuf_size2_107_configbus1;
wire [117:117] mux_1level_tapbuf_size2_107_sram_blwl_out ;
wire [117:117] mux_1level_tapbuf_size2_107_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_107_configbus0[117:117] = sram_blwl_bl[117:117] ;
assign mux_1level_tapbuf_size2_107_configbus1[117:117] = sram_blwl_wl[117:117] ;
wire [117:117] mux_1level_tapbuf_size2_107_configbus0_b;
assign mux_1level_tapbuf_size2_107_configbus0_b[117:117] = sram_blwl_blb[117:117] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_107_ (mux_1level_tapbuf_size2_107_inbus, chanx_1__1__out_14_ , mux_1level_tapbuf_size2_107_sram_blwl_out[117:117] ,
mux_1level_tapbuf_size2_107_sram_blwl_outb[117:117] );
//----- SRAM bits for MUX[107], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_117_ (mux_1level_tapbuf_size2_107_sram_blwl_out[117:117] ,mux_1level_tapbuf_size2_107_sram_blwl_out[117:117] ,mux_1level_tapbuf_size2_107_sram_blwl_outb[117:117] ,mux_1level_tapbuf_size2_107_configbus0[117:117], mux_1level_tapbuf_size2_107_configbus1[117:117] , mux_1level_tapbuf_size2_107_configbus0_b[117:117] );
wire [0:1] mux_1level_tapbuf_size2_108_inbus;
assign mux_1level_tapbuf_size2_108_inbus[0] =  grid_1__1__pin_0__0__44_;
assign mux_1level_tapbuf_size2_108_inbus[1] = chany_0__1__in_80_ ;
wire [118:118] mux_1level_tapbuf_size2_108_configbus0;
wire [118:118] mux_1level_tapbuf_size2_108_configbus1;
wire [118:118] mux_1level_tapbuf_size2_108_sram_blwl_out ;
wire [118:118] mux_1level_tapbuf_size2_108_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_108_configbus0[118:118] = sram_blwl_bl[118:118] ;
assign mux_1level_tapbuf_size2_108_configbus1[118:118] = sram_blwl_wl[118:118] ;
wire [118:118] mux_1level_tapbuf_size2_108_configbus0_b;
assign mux_1level_tapbuf_size2_108_configbus0_b[118:118] = sram_blwl_blb[118:118] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_108_ (mux_1level_tapbuf_size2_108_inbus, chanx_1__1__out_16_ , mux_1level_tapbuf_size2_108_sram_blwl_out[118:118] ,
mux_1level_tapbuf_size2_108_sram_blwl_outb[118:118] );
//----- SRAM bits for MUX[108], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_118_ (mux_1level_tapbuf_size2_108_sram_blwl_out[118:118] ,mux_1level_tapbuf_size2_108_sram_blwl_out[118:118] ,mux_1level_tapbuf_size2_108_sram_blwl_outb[118:118] ,mux_1level_tapbuf_size2_108_configbus0[118:118], mux_1level_tapbuf_size2_108_configbus1[118:118] , mux_1level_tapbuf_size2_108_configbus0_b[118:118] );
wire [0:1] mux_1level_tapbuf_size2_109_inbus;
assign mux_1level_tapbuf_size2_109_inbus[0] =  grid_1__1__pin_0__0__44_;
assign mux_1level_tapbuf_size2_109_inbus[1] = chany_0__1__in_78_ ;
wire [119:119] mux_1level_tapbuf_size2_109_configbus0;
wire [119:119] mux_1level_tapbuf_size2_109_configbus1;
wire [119:119] mux_1level_tapbuf_size2_109_sram_blwl_out ;
wire [119:119] mux_1level_tapbuf_size2_109_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_109_configbus0[119:119] = sram_blwl_bl[119:119] ;
assign mux_1level_tapbuf_size2_109_configbus1[119:119] = sram_blwl_wl[119:119] ;
wire [119:119] mux_1level_tapbuf_size2_109_configbus0_b;
assign mux_1level_tapbuf_size2_109_configbus0_b[119:119] = sram_blwl_blb[119:119] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_109_ (mux_1level_tapbuf_size2_109_inbus, chanx_1__1__out_18_ , mux_1level_tapbuf_size2_109_sram_blwl_out[119:119] ,
mux_1level_tapbuf_size2_109_sram_blwl_outb[119:119] );
//----- SRAM bits for MUX[109], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_119_ (mux_1level_tapbuf_size2_109_sram_blwl_out[119:119] ,mux_1level_tapbuf_size2_109_sram_blwl_out[119:119] ,mux_1level_tapbuf_size2_109_sram_blwl_outb[119:119] ,mux_1level_tapbuf_size2_109_configbus0[119:119], mux_1level_tapbuf_size2_109_configbus1[119:119] , mux_1level_tapbuf_size2_109_configbus0_b[119:119] );
wire [0:1] mux_1level_tapbuf_size2_110_inbus;
assign mux_1level_tapbuf_size2_110_inbus[0] =  grid_1__1__pin_0__0__48_;
assign mux_1level_tapbuf_size2_110_inbus[1] = chany_0__1__in_76_ ;
wire [120:120] mux_1level_tapbuf_size2_110_configbus0;
wire [120:120] mux_1level_tapbuf_size2_110_configbus1;
wire [120:120] mux_1level_tapbuf_size2_110_sram_blwl_out ;
wire [120:120] mux_1level_tapbuf_size2_110_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_110_configbus0[120:120] = sram_blwl_bl[120:120] ;
assign mux_1level_tapbuf_size2_110_configbus1[120:120] = sram_blwl_wl[120:120] ;
wire [120:120] mux_1level_tapbuf_size2_110_configbus0_b;
assign mux_1level_tapbuf_size2_110_configbus0_b[120:120] = sram_blwl_blb[120:120] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_110_ (mux_1level_tapbuf_size2_110_inbus, chanx_1__1__out_20_ , mux_1level_tapbuf_size2_110_sram_blwl_out[120:120] ,
mux_1level_tapbuf_size2_110_sram_blwl_outb[120:120] );
//----- SRAM bits for MUX[110], level=1, select_path_id=1. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----0-----
sram6T_blwl sram_blwl_120_ (mux_1level_tapbuf_size2_110_sram_blwl_out[120:120] ,mux_1level_tapbuf_size2_110_sram_blwl_out[120:120] ,mux_1level_tapbuf_size2_110_sram_blwl_outb[120:120] ,mux_1level_tapbuf_size2_110_configbus0[120:120], mux_1level_tapbuf_size2_110_configbus1[120:120] , mux_1level_tapbuf_size2_110_configbus0_b[120:120] );
wire [0:1] mux_1level_tapbuf_size2_111_inbus;
assign mux_1level_tapbuf_size2_111_inbus[0] =  grid_1__1__pin_0__0__48_;
assign mux_1level_tapbuf_size2_111_inbus[1] = chany_0__1__in_74_ ;
wire [121:121] mux_1level_tapbuf_size2_111_configbus0;
wire [121:121] mux_1level_tapbuf_size2_111_configbus1;
wire [121:121] mux_1level_tapbuf_size2_111_sram_blwl_out ;
wire [121:121] mux_1level_tapbuf_size2_111_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_111_configbus0[121:121] = sram_blwl_bl[121:121] ;
assign mux_1level_tapbuf_size2_111_configbus1[121:121] = sram_blwl_wl[121:121] ;
wire [121:121] mux_1level_tapbuf_size2_111_configbus0_b;
assign mux_1level_tapbuf_size2_111_configbus0_b[121:121] = sram_blwl_blb[121:121] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_111_ (mux_1level_tapbuf_size2_111_inbus, chanx_1__1__out_22_ , mux_1level_tapbuf_size2_111_sram_blwl_out[121:121] ,
mux_1level_tapbuf_size2_111_sram_blwl_outb[121:121] );
//----- SRAM bits for MUX[111], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_121_ (mux_1level_tapbuf_size2_111_sram_blwl_out[121:121] ,mux_1level_tapbuf_size2_111_sram_blwl_out[121:121] ,mux_1level_tapbuf_size2_111_sram_blwl_outb[121:121] ,mux_1level_tapbuf_size2_111_configbus0[121:121], mux_1level_tapbuf_size2_111_configbus1[121:121] , mux_1level_tapbuf_size2_111_configbus0_b[121:121] );
wire [0:1] mux_1level_tapbuf_size2_112_inbus;
assign mux_1level_tapbuf_size2_112_inbus[0] =  grid_1__1__pin_0__0__48_;
assign mux_1level_tapbuf_size2_112_inbus[1] = chany_0__1__in_72_ ;
wire [122:122] mux_1level_tapbuf_size2_112_configbus0;
wire [122:122] mux_1level_tapbuf_size2_112_configbus1;
wire [122:122] mux_1level_tapbuf_size2_112_sram_blwl_out ;
wire [122:122] mux_1level_tapbuf_size2_112_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_112_configbus0[122:122] = sram_blwl_bl[122:122] ;
assign mux_1level_tapbuf_size2_112_configbus1[122:122] = sram_blwl_wl[122:122] ;
wire [122:122] mux_1level_tapbuf_size2_112_configbus0_b;
assign mux_1level_tapbuf_size2_112_configbus0_b[122:122] = sram_blwl_blb[122:122] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_112_ (mux_1level_tapbuf_size2_112_inbus, chanx_1__1__out_24_ , mux_1level_tapbuf_size2_112_sram_blwl_out[122:122] ,
mux_1level_tapbuf_size2_112_sram_blwl_outb[122:122] );
//----- SRAM bits for MUX[112], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_122_ (mux_1level_tapbuf_size2_112_sram_blwl_out[122:122] ,mux_1level_tapbuf_size2_112_sram_blwl_out[122:122] ,mux_1level_tapbuf_size2_112_sram_blwl_outb[122:122] ,mux_1level_tapbuf_size2_112_configbus0[122:122], mux_1level_tapbuf_size2_112_configbus1[122:122] , mux_1level_tapbuf_size2_112_configbus0_b[122:122] );
wire [0:1] mux_1level_tapbuf_size2_113_inbus;
assign mux_1level_tapbuf_size2_113_inbus[0] =  grid_1__1__pin_0__0__48_;
assign mux_1level_tapbuf_size2_113_inbus[1] = chany_0__1__in_70_ ;
wire [123:123] mux_1level_tapbuf_size2_113_configbus0;
wire [123:123] mux_1level_tapbuf_size2_113_configbus1;
wire [123:123] mux_1level_tapbuf_size2_113_sram_blwl_out ;
wire [123:123] mux_1level_tapbuf_size2_113_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_113_configbus0[123:123] = sram_blwl_bl[123:123] ;
assign mux_1level_tapbuf_size2_113_configbus1[123:123] = sram_blwl_wl[123:123] ;
wire [123:123] mux_1level_tapbuf_size2_113_configbus0_b;
assign mux_1level_tapbuf_size2_113_configbus0_b[123:123] = sram_blwl_blb[123:123] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_113_ (mux_1level_tapbuf_size2_113_inbus, chanx_1__1__out_26_ , mux_1level_tapbuf_size2_113_sram_blwl_out[123:123] ,
mux_1level_tapbuf_size2_113_sram_blwl_outb[123:123] );
//----- SRAM bits for MUX[113], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_123_ (mux_1level_tapbuf_size2_113_sram_blwl_out[123:123] ,mux_1level_tapbuf_size2_113_sram_blwl_out[123:123] ,mux_1level_tapbuf_size2_113_sram_blwl_outb[123:123] ,mux_1level_tapbuf_size2_113_configbus0[123:123], mux_1level_tapbuf_size2_113_configbus1[123:123] , mux_1level_tapbuf_size2_113_configbus0_b[123:123] );
wire [0:1] mux_1level_tapbuf_size2_114_inbus;
assign mux_1level_tapbuf_size2_114_inbus[0] =  grid_1__1__pin_0__0__48_;
assign mux_1level_tapbuf_size2_114_inbus[1] = chany_0__1__in_68_ ;
wire [124:124] mux_1level_tapbuf_size2_114_configbus0;
wire [124:124] mux_1level_tapbuf_size2_114_configbus1;
wire [124:124] mux_1level_tapbuf_size2_114_sram_blwl_out ;
wire [124:124] mux_1level_tapbuf_size2_114_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_114_configbus0[124:124] = sram_blwl_bl[124:124] ;
assign mux_1level_tapbuf_size2_114_configbus1[124:124] = sram_blwl_wl[124:124] ;
wire [124:124] mux_1level_tapbuf_size2_114_configbus0_b;
assign mux_1level_tapbuf_size2_114_configbus0_b[124:124] = sram_blwl_blb[124:124] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_114_ (mux_1level_tapbuf_size2_114_inbus, chanx_1__1__out_28_ , mux_1level_tapbuf_size2_114_sram_blwl_out[124:124] ,
mux_1level_tapbuf_size2_114_sram_blwl_outb[124:124] );
//----- SRAM bits for MUX[114], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_124_ (mux_1level_tapbuf_size2_114_sram_blwl_out[124:124] ,mux_1level_tapbuf_size2_114_sram_blwl_out[124:124] ,mux_1level_tapbuf_size2_114_sram_blwl_outb[124:124] ,mux_1level_tapbuf_size2_114_configbus0[124:124], mux_1level_tapbuf_size2_114_configbus1[124:124] , mux_1level_tapbuf_size2_114_configbus0_b[124:124] );
wire [0:1] mux_1level_tapbuf_size2_115_inbus;
assign mux_1level_tapbuf_size2_115_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_115_inbus[1] = chany_0__1__in_66_ ;
wire [125:125] mux_1level_tapbuf_size2_115_configbus0;
wire [125:125] mux_1level_tapbuf_size2_115_configbus1;
wire [125:125] mux_1level_tapbuf_size2_115_sram_blwl_out ;
wire [125:125] mux_1level_tapbuf_size2_115_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_115_configbus0[125:125] = sram_blwl_bl[125:125] ;
assign mux_1level_tapbuf_size2_115_configbus1[125:125] = sram_blwl_wl[125:125] ;
wire [125:125] mux_1level_tapbuf_size2_115_configbus0_b;
assign mux_1level_tapbuf_size2_115_configbus0_b[125:125] = sram_blwl_blb[125:125] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_115_ (mux_1level_tapbuf_size2_115_inbus, chanx_1__1__out_30_ , mux_1level_tapbuf_size2_115_sram_blwl_out[125:125] ,
mux_1level_tapbuf_size2_115_sram_blwl_outb[125:125] );
//----- SRAM bits for MUX[115], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_125_ (mux_1level_tapbuf_size2_115_sram_blwl_out[125:125] ,mux_1level_tapbuf_size2_115_sram_blwl_out[125:125] ,mux_1level_tapbuf_size2_115_sram_blwl_outb[125:125] ,mux_1level_tapbuf_size2_115_configbus0[125:125], mux_1level_tapbuf_size2_115_configbus1[125:125] , mux_1level_tapbuf_size2_115_configbus0_b[125:125] );
wire [0:1] mux_1level_tapbuf_size2_116_inbus;
assign mux_1level_tapbuf_size2_116_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_116_inbus[1] = chany_0__1__in_64_ ;
wire [126:126] mux_1level_tapbuf_size2_116_configbus0;
wire [126:126] mux_1level_tapbuf_size2_116_configbus1;
wire [126:126] mux_1level_tapbuf_size2_116_sram_blwl_out ;
wire [126:126] mux_1level_tapbuf_size2_116_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_116_configbus0[126:126] = sram_blwl_bl[126:126] ;
assign mux_1level_tapbuf_size2_116_configbus1[126:126] = sram_blwl_wl[126:126] ;
wire [126:126] mux_1level_tapbuf_size2_116_configbus0_b;
assign mux_1level_tapbuf_size2_116_configbus0_b[126:126] = sram_blwl_blb[126:126] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_116_ (mux_1level_tapbuf_size2_116_inbus, chanx_1__1__out_32_ , mux_1level_tapbuf_size2_116_sram_blwl_out[126:126] ,
mux_1level_tapbuf_size2_116_sram_blwl_outb[126:126] );
//----- SRAM bits for MUX[116], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_126_ (mux_1level_tapbuf_size2_116_sram_blwl_out[126:126] ,mux_1level_tapbuf_size2_116_sram_blwl_out[126:126] ,mux_1level_tapbuf_size2_116_sram_blwl_outb[126:126] ,mux_1level_tapbuf_size2_116_configbus0[126:126], mux_1level_tapbuf_size2_116_configbus1[126:126] , mux_1level_tapbuf_size2_116_configbus0_b[126:126] );
wire [0:1] mux_1level_tapbuf_size2_117_inbus;
assign mux_1level_tapbuf_size2_117_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_117_inbus[1] = chany_0__1__in_62_ ;
wire [127:127] mux_1level_tapbuf_size2_117_configbus0;
wire [127:127] mux_1level_tapbuf_size2_117_configbus1;
wire [127:127] mux_1level_tapbuf_size2_117_sram_blwl_out ;
wire [127:127] mux_1level_tapbuf_size2_117_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_117_configbus0[127:127] = sram_blwl_bl[127:127] ;
assign mux_1level_tapbuf_size2_117_configbus1[127:127] = sram_blwl_wl[127:127] ;
wire [127:127] mux_1level_tapbuf_size2_117_configbus0_b;
assign mux_1level_tapbuf_size2_117_configbus0_b[127:127] = sram_blwl_blb[127:127] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_117_ (mux_1level_tapbuf_size2_117_inbus, chanx_1__1__out_34_ , mux_1level_tapbuf_size2_117_sram_blwl_out[127:127] ,
mux_1level_tapbuf_size2_117_sram_blwl_outb[127:127] );
//----- SRAM bits for MUX[117], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_127_ (mux_1level_tapbuf_size2_117_sram_blwl_out[127:127] ,mux_1level_tapbuf_size2_117_sram_blwl_out[127:127] ,mux_1level_tapbuf_size2_117_sram_blwl_outb[127:127] ,mux_1level_tapbuf_size2_117_configbus0[127:127], mux_1level_tapbuf_size2_117_configbus1[127:127] , mux_1level_tapbuf_size2_117_configbus0_b[127:127] );
wire [0:1] mux_1level_tapbuf_size2_118_inbus;
assign mux_1level_tapbuf_size2_118_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_118_inbus[1] = chany_0__1__in_60_ ;
wire [128:128] mux_1level_tapbuf_size2_118_configbus0;
wire [128:128] mux_1level_tapbuf_size2_118_configbus1;
wire [128:128] mux_1level_tapbuf_size2_118_sram_blwl_out ;
wire [128:128] mux_1level_tapbuf_size2_118_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_118_configbus0[128:128] = sram_blwl_bl[128:128] ;
assign mux_1level_tapbuf_size2_118_configbus1[128:128] = sram_blwl_wl[128:128] ;
wire [128:128] mux_1level_tapbuf_size2_118_configbus0_b;
assign mux_1level_tapbuf_size2_118_configbus0_b[128:128] = sram_blwl_blb[128:128] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_118_ (mux_1level_tapbuf_size2_118_inbus, chanx_1__1__out_36_ , mux_1level_tapbuf_size2_118_sram_blwl_out[128:128] ,
mux_1level_tapbuf_size2_118_sram_blwl_outb[128:128] );
//----- SRAM bits for MUX[118], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_128_ (mux_1level_tapbuf_size2_118_sram_blwl_out[128:128] ,mux_1level_tapbuf_size2_118_sram_blwl_out[128:128] ,mux_1level_tapbuf_size2_118_sram_blwl_outb[128:128] ,mux_1level_tapbuf_size2_118_configbus0[128:128], mux_1level_tapbuf_size2_118_configbus1[128:128] , mux_1level_tapbuf_size2_118_configbus0_b[128:128] );
wire [0:1] mux_1level_tapbuf_size2_119_inbus;
assign mux_1level_tapbuf_size2_119_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_119_inbus[1] = chany_0__1__in_58_ ;
wire [129:129] mux_1level_tapbuf_size2_119_configbus0;
wire [129:129] mux_1level_tapbuf_size2_119_configbus1;
wire [129:129] mux_1level_tapbuf_size2_119_sram_blwl_out ;
wire [129:129] mux_1level_tapbuf_size2_119_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_119_configbus0[129:129] = sram_blwl_bl[129:129] ;
assign mux_1level_tapbuf_size2_119_configbus1[129:129] = sram_blwl_wl[129:129] ;
wire [129:129] mux_1level_tapbuf_size2_119_configbus0_b;
assign mux_1level_tapbuf_size2_119_configbus0_b[129:129] = sram_blwl_blb[129:129] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_119_ (mux_1level_tapbuf_size2_119_inbus, chanx_1__1__out_38_ , mux_1level_tapbuf_size2_119_sram_blwl_out[129:129] ,
mux_1level_tapbuf_size2_119_sram_blwl_outb[129:129] );
//----- SRAM bits for MUX[119], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_129_ (mux_1level_tapbuf_size2_119_sram_blwl_out[129:129] ,mux_1level_tapbuf_size2_119_sram_blwl_out[129:129] ,mux_1level_tapbuf_size2_119_sram_blwl_outb[129:129] ,mux_1level_tapbuf_size2_119_configbus0[129:129], mux_1level_tapbuf_size2_119_configbus1[129:129] , mux_1level_tapbuf_size2_119_configbus0_b[129:129] );
wire [0:1] mux_1level_tapbuf_size2_120_inbus;
assign mux_1level_tapbuf_size2_120_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_120_inbus[1] = chany_0__1__in_56_ ;
wire [130:130] mux_1level_tapbuf_size2_120_configbus0;
wire [130:130] mux_1level_tapbuf_size2_120_configbus1;
wire [130:130] mux_1level_tapbuf_size2_120_sram_blwl_out ;
wire [130:130] mux_1level_tapbuf_size2_120_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_120_configbus0[130:130] = sram_blwl_bl[130:130] ;
assign mux_1level_tapbuf_size2_120_configbus1[130:130] = sram_blwl_wl[130:130] ;
wire [130:130] mux_1level_tapbuf_size2_120_configbus0_b;
assign mux_1level_tapbuf_size2_120_configbus0_b[130:130] = sram_blwl_blb[130:130] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_120_ (mux_1level_tapbuf_size2_120_inbus, chanx_1__1__out_40_ , mux_1level_tapbuf_size2_120_sram_blwl_out[130:130] ,
mux_1level_tapbuf_size2_120_sram_blwl_outb[130:130] );
//----- SRAM bits for MUX[120], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_130_ (mux_1level_tapbuf_size2_120_sram_blwl_out[130:130] ,mux_1level_tapbuf_size2_120_sram_blwl_out[130:130] ,mux_1level_tapbuf_size2_120_sram_blwl_outb[130:130] ,mux_1level_tapbuf_size2_120_configbus0[130:130], mux_1level_tapbuf_size2_120_configbus1[130:130] , mux_1level_tapbuf_size2_120_configbus0_b[130:130] );
wire [0:1] mux_1level_tapbuf_size2_121_inbus;
assign mux_1level_tapbuf_size2_121_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_121_inbus[1] = chany_0__1__in_54_ ;
wire [131:131] mux_1level_tapbuf_size2_121_configbus0;
wire [131:131] mux_1level_tapbuf_size2_121_configbus1;
wire [131:131] mux_1level_tapbuf_size2_121_sram_blwl_out ;
wire [131:131] mux_1level_tapbuf_size2_121_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_121_configbus0[131:131] = sram_blwl_bl[131:131] ;
assign mux_1level_tapbuf_size2_121_configbus1[131:131] = sram_blwl_wl[131:131] ;
wire [131:131] mux_1level_tapbuf_size2_121_configbus0_b;
assign mux_1level_tapbuf_size2_121_configbus0_b[131:131] = sram_blwl_blb[131:131] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_121_ (mux_1level_tapbuf_size2_121_inbus, chanx_1__1__out_42_ , mux_1level_tapbuf_size2_121_sram_blwl_out[131:131] ,
mux_1level_tapbuf_size2_121_sram_blwl_outb[131:131] );
//----- SRAM bits for MUX[121], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_131_ (mux_1level_tapbuf_size2_121_sram_blwl_out[131:131] ,mux_1level_tapbuf_size2_121_sram_blwl_out[131:131] ,mux_1level_tapbuf_size2_121_sram_blwl_outb[131:131] ,mux_1level_tapbuf_size2_121_configbus0[131:131], mux_1level_tapbuf_size2_121_configbus1[131:131] , mux_1level_tapbuf_size2_121_configbus0_b[131:131] );
wire [0:1] mux_1level_tapbuf_size2_122_inbus;
assign mux_1level_tapbuf_size2_122_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_122_inbus[1] = chany_0__1__in_52_ ;
wire [132:132] mux_1level_tapbuf_size2_122_configbus0;
wire [132:132] mux_1level_tapbuf_size2_122_configbus1;
wire [132:132] mux_1level_tapbuf_size2_122_sram_blwl_out ;
wire [132:132] mux_1level_tapbuf_size2_122_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_122_configbus0[132:132] = sram_blwl_bl[132:132] ;
assign mux_1level_tapbuf_size2_122_configbus1[132:132] = sram_blwl_wl[132:132] ;
wire [132:132] mux_1level_tapbuf_size2_122_configbus0_b;
assign mux_1level_tapbuf_size2_122_configbus0_b[132:132] = sram_blwl_blb[132:132] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_122_ (mux_1level_tapbuf_size2_122_inbus, chanx_1__1__out_44_ , mux_1level_tapbuf_size2_122_sram_blwl_out[132:132] ,
mux_1level_tapbuf_size2_122_sram_blwl_outb[132:132] );
//----- SRAM bits for MUX[122], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_132_ (mux_1level_tapbuf_size2_122_sram_blwl_out[132:132] ,mux_1level_tapbuf_size2_122_sram_blwl_out[132:132] ,mux_1level_tapbuf_size2_122_sram_blwl_outb[132:132] ,mux_1level_tapbuf_size2_122_configbus0[132:132], mux_1level_tapbuf_size2_122_configbus1[132:132] , mux_1level_tapbuf_size2_122_configbus0_b[132:132] );
wire [0:1] mux_1level_tapbuf_size2_123_inbus;
assign mux_1level_tapbuf_size2_123_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_123_inbus[1] = chany_0__1__in_50_ ;
wire [133:133] mux_1level_tapbuf_size2_123_configbus0;
wire [133:133] mux_1level_tapbuf_size2_123_configbus1;
wire [133:133] mux_1level_tapbuf_size2_123_sram_blwl_out ;
wire [133:133] mux_1level_tapbuf_size2_123_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_123_configbus0[133:133] = sram_blwl_bl[133:133] ;
assign mux_1level_tapbuf_size2_123_configbus1[133:133] = sram_blwl_wl[133:133] ;
wire [133:133] mux_1level_tapbuf_size2_123_configbus0_b;
assign mux_1level_tapbuf_size2_123_configbus0_b[133:133] = sram_blwl_blb[133:133] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_123_ (mux_1level_tapbuf_size2_123_inbus, chanx_1__1__out_46_ , mux_1level_tapbuf_size2_123_sram_blwl_out[133:133] ,
mux_1level_tapbuf_size2_123_sram_blwl_outb[133:133] );
//----- SRAM bits for MUX[123], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_133_ (mux_1level_tapbuf_size2_123_sram_blwl_out[133:133] ,mux_1level_tapbuf_size2_123_sram_blwl_out[133:133] ,mux_1level_tapbuf_size2_123_sram_blwl_outb[133:133] ,mux_1level_tapbuf_size2_123_configbus0[133:133], mux_1level_tapbuf_size2_123_configbus1[133:133] , mux_1level_tapbuf_size2_123_configbus0_b[133:133] );
wire [0:1] mux_1level_tapbuf_size2_124_inbus;
assign mux_1level_tapbuf_size2_124_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_124_inbus[1] = chany_0__1__in_48_ ;
wire [134:134] mux_1level_tapbuf_size2_124_configbus0;
wire [134:134] mux_1level_tapbuf_size2_124_configbus1;
wire [134:134] mux_1level_tapbuf_size2_124_sram_blwl_out ;
wire [134:134] mux_1level_tapbuf_size2_124_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_124_configbus0[134:134] = sram_blwl_bl[134:134] ;
assign mux_1level_tapbuf_size2_124_configbus1[134:134] = sram_blwl_wl[134:134] ;
wire [134:134] mux_1level_tapbuf_size2_124_configbus0_b;
assign mux_1level_tapbuf_size2_124_configbus0_b[134:134] = sram_blwl_blb[134:134] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_124_ (mux_1level_tapbuf_size2_124_inbus, chanx_1__1__out_48_ , mux_1level_tapbuf_size2_124_sram_blwl_out[134:134] ,
mux_1level_tapbuf_size2_124_sram_blwl_outb[134:134] );
//----- SRAM bits for MUX[124], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_134_ (mux_1level_tapbuf_size2_124_sram_blwl_out[134:134] ,mux_1level_tapbuf_size2_124_sram_blwl_out[134:134] ,mux_1level_tapbuf_size2_124_sram_blwl_outb[134:134] ,mux_1level_tapbuf_size2_124_configbus0[134:134], mux_1level_tapbuf_size2_124_configbus1[134:134] , mux_1level_tapbuf_size2_124_configbus0_b[134:134] );
wire [0:1] mux_1level_tapbuf_size2_125_inbus;
assign mux_1level_tapbuf_size2_125_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_125_inbus[1] = chany_0__1__in_46_ ;
wire [135:135] mux_1level_tapbuf_size2_125_configbus0;
wire [135:135] mux_1level_tapbuf_size2_125_configbus1;
wire [135:135] mux_1level_tapbuf_size2_125_sram_blwl_out ;
wire [135:135] mux_1level_tapbuf_size2_125_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_125_configbus0[135:135] = sram_blwl_bl[135:135] ;
assign mux_1level_tapbuf_size2_125_configbus1[135:135] = sram_blwl_wl[135:135] ;
wire [135:135] mux_1level_tapbuf_size2_125_configbus0_b;
assign mux_1level_tapbuf_size2_125_configbus0_b[135:135] = sram_blwl_blb[135:135] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_125_ (mux_1level_tapbuf_size2_125_inbus, chanx_1__1__out_50_ , mux_1level_tapbuf_size2_125_sram_blwl_out[135:135] ,
mux_1level_tapbuf_size2_125_sram_blwl_outb[135:135] );
//----- SRAM bits for MUX[125], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_135_ (mux_1level_tapbuf_size2_125_sram_blwl_out[135:135] ,mux_1level_tapbuf_size2_125_sram_blwl_out[135:135] ,mux_1level_tapbuf_size2_125_sram_blwl_outb[135:135] ,mux_1level_tapbuf_size2_125_configbus0[135:135], mux_1level_tapbuf_size2_125_configbus1[135:135] , mux_1level_tapbuf_size2_125_configbus0_b[135:135] );
wire [0:1] mux_1level_tapbuf_size2_126_inbus;
assign mux_1level_tapbuf_size2_126_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_126_inbus[1] = chany_0__1__in_44_ ;
wire [136:136] mux_1level_tapbuf_size2_126_configbus0;
wire [136:136] mux_1level_tapbuf_size2_126_configbus1;
wire [136:136] mux_1level_tapbuf_size2_126_sram_blwl_out ;
wire [136:136] mux_1level_tapbuf_size2_126_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_126_configbus0[136:136] = sram_blwl_bl[136:136] ;
assign mux_1level_tapbuf_size2_126_configbus1[136:136] = sram_blwl_wl[136:136] ;
wire [136:136] mux_1level_tapbuf_size2_126_configbus0_b;
assign mux_1level_tapbuf_size2_126_configbus0_b[136:136] = sram_blwl_blb[136:136] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_126_ (mux_1level_tapbuf_size2_126_inbus, chanx_1__1__out_52_ , mux_1level_tapbuf_size2_126_sram_blwl_out[136:136] ,
mux_1level_tapbuf_size2_126_sram_blwl_outb[136:136] );
//----- SRAM bits for MUX[126], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_136_ (mux_1level_tapbuf_size2_126_sram_blwl_out[136:136] ,mux_1level_tapbuf_size2_126_sram_blwl_out[136:136] ,mux_1level_tapbuf_size2_126_sram_blwl_outb[136:136] ,mux_1level_tapbuf_size2_126_configbus0[136:136], mux_1level_tapbuf_size2_126_configbus1[136:136] , mux_1level_tapbuf_size2_126_configbus0_b[136:136] );
wire [0:1] mux_1level_tapbuf_size2_127_inbus;
assign mux_1level_tapbuf_size2_127_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_127_inbus[1] = chany_0__1__in_42_ ;
wire [137:137] mux_1level_tapbuf_size2_127_configbus0;
wire [137:137] mux_1level_tapbuf_size2_127_configbus1;
wire [137:137] mux_1level_tapbuf_size2_127_sram_blwl_out ;
wire [137:137] mux_1level_tapbuf_size2_127_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_127_configbus0[137:137] = sram_blwl_bl[137:137] ;
assign mux_1level_tapbuf_size2_127_configbus1[137:137] = sram_blwl_wl[137:137] ;
wire [137:137] mux_1level_tapbuf_size2_127_configbus0_b;
assign mux_1level_tapbuf_size2_127_configbus0_b[137:137] = sram_blwl_blb[137:137] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_127_ (mux_1level_tapbuf_size2_127_inbus, chanx_1__1__out_54_ , mux_1level_tapbuf_size2_127_sram_blwl_out[137:137] ,
mux_1level_tapbuf_size2_127_sram_blwl_outb[137:137] );
//----- SRAM bits for MUX[127], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_137_ (mux_1level_tapbuf_size2_127_sram_blwl_out[137:137] ,mux_1level_tapbuf_size2_127_sram_blwl_out[137:137] ,mux_1level_tapbuf_size2_127_sram_blwl_outb[137:137] ,mux_1level_tapbuf_size2_127_configbus0[137:137], mux_1level_tapbuf_size2_127_configbus1[137:137] , mux_1level_tapbuf_size2_127_configbus0_b[137:137] );
wire [0:1] mux_1level_tapbuf_size2_128_inbus;
assign mux_1level_tapbuf_size2_128_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_128_inbus[1] = chany_0__1__in_40_ ;
wire [138:138] mux_1level_tapbuf_size2_128_configbus0;
wire [138:138] mux_1level_tapbuf_size2_128_configbus1;
wire [138:138] mux_1level_tapbuf_size2_128_sram_blwl_out ;
wire [138:138] mux_1level_tapbuf_size2_128_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_128_configbus0[138:138] = sram_blwl_bl[138:138] ;
assign mux_1level_tapbuf_size2_128_configbus1[138:138] = sram_blwl_wl[138:138] ;
wire [138:138] mux_1level_tapbuf_size2_128_configbus0_b;
assign mux_1level_tapbuf_size2_128_configbus0_b[138:138] = sram_blwl_blb[138:138] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_128_ (mux_1level_tapbuf_size2_128_inbus, chanx_1__1__out_56_ , mux_1level_tapbuf_size2_128_sram_blwl_out[138:138] ,
mux_1level_tapbuf_size2_128_sram_blwl_outb[138:138] );
//----- SRAM bits for MUX[128], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_138_ (mux_1level_tapbuf_size2_128_sram_blwl_out[138:138] ,mux_1level_tapbuf_size2_128_sram_blwl_out[138:138] ,mux_1level_tapbuf_size2_128_sram_blwl_outb[138:138] ,mux_1level_tapbuf_size2_128_configbus0[138:138], mux_1level_tapbuf_size2_128_configbus1[138:138] , mux_1level_tapbuf_size2_128_configbus0_b[138:138] );
wire [0:1] mux_1level_tapbuf_size2_129_inbus;
assign mux_1level_tapbuf_size2_129_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_129_inbus[1] = chany_0__1__in_38_ ;
wire [139:139] mux_1level_tapbuf_size2_129_configbus0;
wire [139:139] mux_1level_tapbuf_size2_129_configbus1;
wire [139:139] mux_1level_tapbuf_size2_129_sram_blwl_out ;
wire [139:139] mux_1level_tapbuf_size2_129_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_129_configbus0[139:139] = sram_blwl_bl[139:139] ;
assign mux_1level_tapbuf_size2_129_configbus1[139:139] = sram_blwl_wl[139:139] ;
wire [139:139] mux_1level_tapbuf_size2_129_configbus0_b;
assign mux_1level_tapbuf_size2_129_configbus0_b[139:139] = sram_blwl_blb[139:139] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_129_ (mux_1level_tapbuf_size2_129_inbus, chanx_1__1__out_58_ , mux_1level_tapbuf_size2_129_sram_blwl_out[139:139] ,
mux_1level_tapbuf_size2_129_sram_blwl_outb[139:139] );
//----- SRAM bits for MUX[129], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_139_ (mux_1level_tapbuf_size2_129_sram_blwl_out[139:139] ,mux_1level_tapbuf_size2_129_sram_blwl_out[139:139] ,mux_1level_tapbuf_size2_129_sram_blwl_outb[139:139] ,mux_1level_tapbuf_size2_129_configbus0[139:139], mux_1level_tapbuf_size2_129_configbus1[139:139] , mux_1level_tapbuf_size2_129_configbus0_b[139:139] );
wire [0:1] mux_1level_tapbuf_size2_130_inbus;
assign mux_1level_tapbuf_size2_130_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_130_inbus[1] = chany_0__1__in_36_ ;
wire [140:140] mux_1level_tapbuf_size2_130_configbus0;
wire [140:140] mux_1level_tapbuf_size2_130_configbus1;
wire [140:140] mux_1level_tapbuf_size2_130_sram_blwl_out ;
wire [140:140] mux_1level_tapbuf_size2_130_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_130_configbus0[140:140] = sram_blwl_bl[140:140] ;
assign mux_1level_tapbuf_size2_130_configbus1[140:140] = sram_blwl_wl[140:140] ;
wire [140:140] mux_1level_tapbuf_size2_130_configbus0_b;
assign mux_1level_tapbuf_size2_130_configbus0_b[140:140] = sram_blwl_blb[140:140] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_130_ (mux_1level_tapbuf_size2_130_inbus, chanx_1__1__out_60_ , mux_1level_tapbuf_size2_130_sram_blwl_out[140:140] ,
mux_1level_tapbuf_size2_130_sram_blwl_outb[140:140] );
//----- SRAM bits for MUX[130], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_140_ (mux_1level_tapbuf_size2_130_sram_blwl_out[140:140] ,mux_1level_tapbuf_size2_130_sram_blwl_out[140:140] ,mux_1level_tapbuf_size2_130_sram_blwl_outb[140:140] ,mux_1level_tapbuf_size2_130_configbus0[140:140], mux_1level_tapbuf_size2_130_configbus1[140:140] , mux_1level_tapbuf_size2_130_configbus0_b[140:140] );
wire [0:1] mux_1level_tapbuf_size2_131_inbus;
assign mux_1level_tapbuf_size2_131_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_131_inbus[1] = chany_0__1__in_34_ ;
wire [141:141] mux_1level_tapbuf_size2_131_configbus0;
wire [141:141] mux_1level_tapbuf_size2_131_configbus1;
wire [141:141] mux_1level_tapbuf_size2_131_sram_blwl_out ;
wire [141:141] mux_1level_tapbuf_size2_131_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_131_configbus0[141:141] = sram_blwl_bl[141:141] ;
assign mux_1level_tapbuf_size2_131_configbus1[141:141] = sram_blwl_wl[141:141] ;
wire [141:141] mux_1level_tapbuf_size2_131_configbus0_b;
assign mux_1level_tapbuf_size2_131_configbus0_b[141:141] = sram_blwl_blb[141:141] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_131_ (mux_1level_tapbuf_size2_131_inbus, chanx_1__1__out_62_ , mux_1level_tapbuf_size2_131_sram_blwl_out[141:141] ,
mux_1level_tapbuf_size2_131_sram_blwl_outb[141:141] );
//----- SRAM bits for MUX[131], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_141_ (mux_1level_tapbuf_size2_131_sram_blwl_out[141:141] ,mux_1level_tapbuf_size2_131_sram_blwl_out[141:141] ,mux_1level_tapbuf_size2_131_sram_blwl_outb[141:141] ,mux_1level_tapbuf_size2_131_configbus0[141:141], mux_1level_tapbuf_size2_131_configbus1[141:141] , mux_1level_tapbuf_size2_131_configbus0_b[141:141] );
wire [0:1] mux_1level_tapbuf_size2_132_inbus;
assign mux_1level_tapbuf_size2_132_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_132_inbus[1] = chany_0__1__in_32_ ;
wire [142:142] mux_1level_tapbuf_size2_132_configbus0;
wire [142:142] mux_1level_tapbuf_size2_132_configbus1;
wire [142:142] mux_1level_tapbuf_size2_132_sram_blwl_out ;
wire [142:142] mux_1level_tapbuf_size2_132_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_132_configbus0[142:142] = sram_blwl_bl[142:142] ;
assign mux_1level_tapbuf_size2_132_configbus1[142:142] = sram_blwl_wl[142:142] ;
wire [142:142] mux_1level_tapbuf_size2_132_configbus0_b;
assign mux_1level_tapbuf_size2_132_configbus0_b[142:142] = sram_blwl_blb[142:142] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_132_ (mux_1level_tapbuf_size2_132_inbus, chanx_1__1__out_64_ , mux_1level_tapbuf_size2_132_sram_blwl_out[142:142] ,
mux_1level_tapbuf_size2_132_sram_blwl_outb[142:142] );
//----- SRAM bits for MUX[132], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_142_ (mux_1level_tapbuf_size2_132_sram_blwl_out[142:142] ,mux_1level_tapbuf_size2_132_sram_blwl_out[142:142] ,mux_1level_tapbuf_size2_132_sram_blwl_outb[142:142] ,mux_1level_tapbuf_size2_132_configbus0[142:142], mux_1level_tapbuf_size2_132_configbus1[142:142] , mux_1level_tapbuf_size2_132_configbus0_b[142:142] );
wire [0:1] mux_1level_tapbuf_size2_133_inbus;
assign mux_1level_tapbuf_size2_133_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_133_inbus[1] = chany_0__1__in_30_ ;
wire [143:143] mux_1level_tapbuf_size2_133_configbus0;
wire [143:143] mux_1level_tapbuf_size2_133_configbus1;
wire [143:143] mux_1level_tapbuf_size2_133_sram_blwl_out ;
wire [143:143] mux_1level_tapbuf_size2_133_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_133_configbus0[143:143] = sram_blwl_bl[143:143] ;
assign mux_1level_tapbuf_size2_133_configbus1[143:143] = sram_blwl_wl[143:143] ;
wire [143:143] mux_1level_tapbuf_size2_133_configbus0_b;
assign mux_1level_tapbuf_size2_133_configbus0_b[143:143] = sram_blwl_blb[143:143] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_133_ (mux_1level_tapbuf_size2_133_inbus, chanx_1__1__out_66_ , mux_1level_tapbuf_size2_133_sram_blwl_out[143:143] ,
mux_1level_tapbuf_size2_133_sram_blwl_outb[143:143] );
//----- SRAM bits for MUX[133], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_143_ (mux_1level_tapbuf_size2_133_sram_blwl_out[143:143] ,mux_1level_tapbuf_size2_133_sram_blwl_out[143:143] ,mux_1level_tapbuf_size2_133_sram_blwl_outb[143:143] ,mux_1level_tapbuf_size2_133_configbus0[143:143], mux_1level_tapbuf_size2_133_configbus1[143:143] , mux_1level_tapbuf_size2_133_configbus0_b[143:143] );
wire [0:1] mux_1level_tapbuf_size2_134_inbus;
assign mux_1level_tapbuf_size2_134_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_134_inbus[1] = chany_0__1__in_28_ ;
wire [144:144] mux_1level_tapbuf_size2_134_configbus0;
wire [144:144] mux_1level_tapbuf_size2_134_configbus1;
wire [144:144] mux_1level_tapbuf_size2_134_sram_blwl_out ;
wire [144:144] mux_1level_tapbuf_size2_134_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_134_configbus0[144:144] = sram_blwl_bl[144:144] ;
assign mux_1level_tapbuf_size2_134_configbus1[144:144] = sram_blwl_wl[144:144] ;
wire [144:144] mux_1level_tapbuf_size2_134_configbus0_b;
assign mux_1level_tapbuf_size2_134_configbus0_b[144:144] = sram_blwl_blb[144:144] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_134_ (mux_1level_tapbuf_size2_134_inbus, chanx_1__1__out_68_ , mux_1level_tapbuf_size2_134_sram_blwl_out[144:144] ,
mux_1level_tapbuf_size2_134_sram_blwl_outb[144:144] );
//----- SRAM bits for MUX[134], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_144_ (mux_1level_tapbuf_size2_134_sram_blwl_out[144:144] ,mux_1level_tapbuf_size2_134_sram_blwl_out[144:144] ,mux_1level_tapbuf_size2_134_sram_blwl_outb[144:144] ,mux_1level_tapbuf_size2_134_configbus0[144:144], mux_1level_tapbuf_size2_134_configbus1[144:144] , mux_1level_tapbuf_size2_134_configbus0_b[144:144] );
wire [0:1] mux_1level_tapbuf_size2_135_inbus;
assign mux_1level_tapbuf_size2_135_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_135_inbus[1] = chany_0__1__in_26_ ;
wire [145:145] mux_1level_tapbuf_size2_135_configbus0;
wire [145:145] mux_1level_tapbuf_size2_135_configbus1;
wire [145:145] mux_1level_tapbuf_size2_135_sram_blwl_out ;
wire [145:145] mux_1level_tapbuf_size2_135_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_135_configbus0[145:145] = sram_blwl_bl[145:145] ;
assign mux_1level_tapbuf_size2_135_configbus1[145:145] = sram_blwl_wl[145:145] ;
wire [145:145] mux_1level_tapbuf_size2_135_configbus0_b;
assign mux_1level_tapbuf_size2_135_configbus0_b[145:145] = sram_blwl_blb[145:145] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_135_ (mux_1level_tapbuf_size2_135_inbus, chanx_1__1__out_70_ , mux_1level_tapbuf_size2_135_sram_blwl_out[145:145] ,
mux_1level_tapbuf_size2_135_sram_blwl_outb[145:145] );
//----- SRAM bits for MUX[135], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_145_ (mux_1level_tapbuf_size2_135_sram_blwl_out[145:145] ,mux_1level_tapbuf_size2_135_sram_blwl_out[145:145] ,mux_1level_tapbuf_size2_135_sram_blwl_outb[145:145] ,mux_1level_tapbuf_size2_135_configbus0[145:145], mux_1level_tapbuf_size2_135_configbus1[145:145] , mux_1level_tapbuf_size2_135_configbus0_b[145:145] );
wire [0:1] mux_1level_tapbuf_size2_136_inbus;
assign mux_1level_tapbuf_size2_136_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_136_inbus[1] = chany_0__1__in_24_ ;
wire [146:146] mux_1level_tapbuf_size2_136_configbus0;
wire [146:146] mux_1level_tapbuf_size2_136_configbus1;
wire [146:146] mux_1level_tapbuf_size2_136_sram_blwl_out ;
wire [146:146] mux_1level_tapbuf_size2_136_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_136_configbus0[146:146] = sram_blwl_bl[146:146] ;
assign mux_1level_tapbuf_size2_136_configbus1[146:146] = sram_blwl_wl[146:146] ;
wire [146:146] mux_1level_tapbuf_size2_136_configbus0_b;
assign mux_1level_tapbuf_size2_136_configbus0_b[146:146] = sram_blwl_blb[146:146] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_136_ (mux_1level_tapbuf_size2_136_inbus, chanx_1__1__out_72_ , mux_1level_tapbuf_size2_136_sram_blwl_out[146:146] ,
mux_1level_tapbuf_size2_136_sram_blwl_outb[146:146] );
//----- SRAM bits for MUX[136], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_146_ (mux_1level_tapbuf_size2_136_sram_blwl_out[146:146] ,mux_1level_tapbuf_size2_136_sram_blwl_out[146:146] ,mux_1level_tapbuf_size2_136_sram_blwl_outb[146:146] ,mux_1level_tapbuf_size2_136_configbus0[146:146], mux_1level_tapbuf_size2_136_configbus1[146:146] , mux_1level_tapbuf_size2_136_configbus0_b[146:146] );
wire [0:1] mux_1level_tapbuf_size2_137_inbus;
assign mux_1level_tapbuf_size2_137_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_137_inbus[1] = chany_0__1__in_22_ ;
wire [147:147] mux_1level_tapbuf_size2_137_configbus0;
wire [147:147] mux_1level_tapbuf_size2_137_configbus1;
wire [147:147] mux_1level_tapbuf_size2_137_sram_blwl_out ;
wire [147:147] mux_1level_tapbuf_size2_137_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_137_configbus0[147:147] = sram_blwl_bl[147:147] ;
assign mux_1level_tapbuf_size2_137_configbus1[147:147] = sram_blwl_wl[147:147] ;
wire [147:147] mux_1level_tapbuf_size2_137_configbus0_b;
assign mux_1level_tapbuf_size2_137_configbus0_b[147:147] = sram_blwl_blb[147:147] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_137_ (mux_1level_tapbuf_size2_137_inbus, chanx_1__1__out_74_ , mux_1level_tapbuf_size2_137_sram_blwl_out[147:147] ,
mux_1level_tapbuf_size2_137_sram_blwl_outb[147:147] );
//----- SRAM bits for MUX[137], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_147_ (mux_1level_tapbuf_size2_137_sram_blwl_out[147:147] ,mux_1level_tapbuf_size2_137_sram_blwl_out[147:147] ,mux_1level_tapbuf_size2_137_sram_blwl_outb[147:147] ,mux_1level_tapbuf_size2_137_configbus0[147:147], mux_1level_tapbuf_size2_137_configbus1[147:147] , mux_1level_tapbuf_size2_137_configbus0_b[147:147] );
wire [0:1] mux_1level_tapbuf_size2_138_inbus;
assign mux_1level_tapbuf_size2_138_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_138_inbus[1] = chany_0__1__in_20_ ;
wire [148:148] mux_1level_tapbuf_size2_138_configbus0;
wire [148:148] mux_1level_tapbuf_size2_138_configbus1;
wire [148:148] mux_1level_tapbuf_size2_138_sram_blwl_out ;
wire [148:148] mux_1level_tapbuf_size2_138_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_138_configbus0[148:148] = sram_blwl_bl[148:148] ;
assign mux_1level_tapbuf_size2_138_configbus1[148:148] = sram_blwl_wl[148:148] ;
wire [148:148] mux_1level_tapbuf_size2_138_configbus0_b;
assign mux_1level_tapbuf_size2_138_configbus0_b[148:148] = sram_blwl_blb[148:148] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_138_ (mux_1level_tapbuf_size2_138_inbus, chanx_1__1__out_76_ , mux_1level_tapbuf_size2_138_sram_blwl_out[148:148] ,
mux_1level_tapbuf_size2_138_sram_blwl_outb[148:148] );
//----- SRAM bits for MUX[138], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_148_ (mux_1level_tapbuf_size2_138_sram_blwl_out[148:148] ,mux_1level_tapbuf_size2_138_sram_blwl_out[148:148] ,mux_1level_tapbuf_size2_138_sram_blwl_outb[148:148] ,mux_1level_tapbuf_size2_138_configbus0[148:148], mux_1level_tapbuf_size2_138_configbus1[148:148] , mux_1level_tapbuf_size2_138_configbus0_b[148:148] );
wire [0:1] mux_1level_tapbuf_size2_139_inbus;
assign mux_1level_tapbuf_size2_139_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_139_inbus[1] = chany_0__1__in_18_ ;
wire [149:149] mux_1level_tapbuf_size2_139_configbus0;
wire [149:149] mux_1level_tapbuf_size2_139_configbus1;
wire [149:149] mux_1level_tapbuf_size2_139_sram_blwl_out ;
wire [149:149] mux_1level_tapbuf_size2_139_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_139_configbus0[149:149] = sram_blwl_bl[149:149] ;
assign mux_1level_tapbuf_size2_139_configbus1[149:149] = sram_blwl_wl[149:149] ;
wire [149:149] mux_1level_tapbuf_size2_139_configbus0_b;
assign mux_1level_tapbuf_size2_139_configbus0_b[149:149] = sram_blwl_blb[149:149] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_139_ (mux_1level_tapbuf_size2_139_inbus, chanx_1__1__out_78_ , mux_1level_tapbuf_size2_139_sram_blwl_out[149:149] ,
mux_1level_tapbuf_size2_139_sram_blwl_outb[149:149] );
//----- SRAM bits for MUX[139], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_149_ (mux_1level_tapbuf_size2_139_sram_blwl_out[149:149] ,mux_1level_tapbuf_size2_139_sram_blwl_out[149:149] ,mux_1level_tapbuf_size2_139_sram_blwl_outb[149:149] ,mux_1level_tapbuf_size2_139_configbus0[149:149], mux_1level_tapbuf_size2_139_configbus1[149:149] , mux_1level_tapbuf_size2_139_configbus0_b[149:149] );
wire [0:1] mux_1level_tapbuf_size2_140_inbus;
assign mux_1level_tapbuf_size2_140_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_140_inbus[1] = chany_0__1__in_16_ ;
wire [150:150] mux_1level_tapbuf_size2_140_configbus0;
wire [150:150] mux_1level_tapbuf_size2_140_configbus1;
wire [150:150] mux_1level_tapbuf_size2_140_sram_blwl_out ;
wire [150:150] mux_1level_tapbuf_size2_140_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_140_configbus0[150:150] = sram_blwl_bl[150:150] ;
assign mux_1level_tapbuf_size2_140_configbus1[150:150] = sram_blwl_wl[150:150] ;
wire [150:150] mux_1level_tapbuf_size2_140_configbus0_b;
assign mux_1level_tapbuf_size2_140_configbus0_b[150:150] = sram_blwl_blb[150:150] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_140_ (mux_1level_tapbuf_size2_140_inbus, chanx_1__1__out_80_ , mux_1level_tapbuf_size2_140_sram_blwl_out[150:150] ,
mux_1level_tapbuf_size2_140_sram_blwl_outb[150:150] );
//----- SRAM bits for MUX[140], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_150_ (mux_1level_tapbuf_size2_140_sram_blwl_out[150:150] ,mux_1level_tapbuf_size2_140_sram_blwl_out[150:150] ,mux_1level_tapbuf_size2_140_sram_blwl_outb[150:150] ,mux_1level_tapbuf_size2_140_configbus0[150:150], mux_1level_tapbuf_size2_140_configbus1[150:150] , mux_1level_tapbuf_size2_140_configbus0_b[150:150] );
wire [0:1] mux_1level_tapbuf_size2_141_inbus;
assign mux_1level_tapbuf_size2_141_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_141_inbus[1] = chany_0__1__in_14_ ;
wire [151:151] mux_1level_tapbuf_size2_141_configbus0;
wire [151:151] mux_1level_tapbuf_size2_141_configbus1;
wire [151:151] mux_1level_tapbuf_size2_141_sram_blwl_out ;
wire [151:151] mux_1level_tapbuf_size2_141_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_141_configbus0[151:151] = sram_blwl_bl[151:151] ;
assign mux_1level_tapbuf_size2_141_configbus1[151:151] = sram_blwl_wl[151:151] ;
wire [151:151] mux_1level_tapbuf_size2_141_configbus0_b;
assign mux_1level_tapbuf_size2_141_configbus0_b[151:151] = sram_blwl_blb[151:151] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_141_ (mux_1level_tapbuf_size2_141_inbus, chanx_1__1__out_82_ , mux_1level_tapbuf_size2_141_sram_blwl_out[151:151] ,
mux_1level_tapbuf_size2_141_sram_blwl_outb[151:151] );
//----- SRAM bits for MUX[141], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_151_ (mux_1level_tapbuf_size2_141_sram_blwl_out[151:151] ,mux_1level_tapbuf_size2_141_sram_blwl_out[151:151] ,mux_1level_tapbuf_size2_141_sram_blwl_outb[151:151] ,mux_1level_tapbuf_size2_141_configbus0[151:151], mux_1level_tapbuf_size2_141_configbus1[151:151] , mux_1level_tapbuf_size2_141_configbus0_b[151:151] );
wire [0:1] mux_1level_tapbuf_size2_142_inbus;
assign mux_1level_tapbuf_size2_142_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_142_inbus[1] = chany_0__1__in_12_ ;
wire [152:152] mux_1level_tapbuf_size2_142_configbus0;
wire [152:152] mux_1level_tapbuf_size2_142_configbus1;
wire [152:152] mux_1level_tapbuf_size2_142_sram_blwl_out ;
wire [152:152] mux_1level_tapbuf_size2_142_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_142_configbus0[152:152] = sram_blwl_bl[152:152] ;
assign mux_1level_tapbuf_size2_142_configbus1[152:152] = sram_blwl_wl[152:152] ;
wire [152:152] mux_1level_tapbuf_size2_142_configbus0_b;
assign mux_1level_tapbuf_size2_142_configbus0_b[152:152] = sram_blwl_blb[152:152] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_142_ (mux_1level_tapbuf_size2_142_inbus, chanx_1__1__out_84_ , mux_1level_tapbuf_size2_142_sram_blwl_out[152:152] ,
mux_1level_tapbuf_size2_142_sram_blwl_outb[152:152] );
//----- SRAM bits for MUX[142], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_152_ (mux_1level_tapbuf_size2_142_sram_blwl_out[152:152] ,mux_1level_tapbuf_size2_142_sram_blwl_out[152:152] ,mux_1level_tapbuf_size2_142_sram_blwl_outb[152:152] ,mux_1level_tapbuf_size2_142_configbus0[152:152], mux_1level_tapbuf_size2_142_configbus1[152:152] , mux_1level_tapbuf_size2_142_configbus0_b[152:152] );
wire [0:1] mux_1level_tapbuf_size2_143_inbus;
assign mux_1level_tapbuf_size2_143_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_143_inbus[1] = chany_0__1__in_10_ ;
wire [153:153] mux_1level_tapbuf_size2_143_configbus0;
wire [153:153] mux_1level_tapbuf_size2_143_configbus1;
wire [153:153] mux_1level_tapbuf_size2_143_sram_blwl_out ;
wire [153:153] mux_1level_tapbuf_size2_143_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_143_configbus0[153:153] = sram_blwl_bl[153:153] ;
assign mux_1level_tapbuf_size2_143_configbus1[153:153] = sram_blwl_wl[153:153] ;
wire [153:153] mux_1level_tapbuf_size2_143_configbus0_b;
assign mux_1level_tapbuf_size2_143_configbus0_b[153:153] = sram_blwl_blb[153:153] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_143_ (mux_1level_tapbuf_size2_143_inbus, chanx_1__1__out_86_ , mux_1level_tapbuf_size2_143_sram_blwl_out[153:153] ,
mux_1level_tapbuf_size2_143_sram_blwl_outb[153:153] );
//----- SRAM bits for MUX[143], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_153_ (mux_1level_tapbuf_size2_143_sram_blwl_out[153:153] ,mux_1level_tapbuf_size2_143_sram_blwl_out[153:153] ,mux_1level_tapbuf_size2_143_sram_blwl_outb[153:153] ,mux_1level_tapbuf_size2_143_configbus0[153:153], mux_1level_tapbuf_size2_143_configbus1[153:153] , mux_1level_tapbuf_size2_143_configbus0_b[153:153] );
wire [0:1] mux_1level_tapbuf_size2_144_inbus;
assign mux_1level_tapbuf_size2_144_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_144_inbus[1] = chany_0__1__in_8_ ;
wire [154:154] mux_1level_tapbuf_size2_144_configbus0;
wire [154:154] mux_1level_tapbuf_size2_144_configbus1;
wire [154:154] mux_1level_tapbuf_size2_144_sram_blwl_out ;
wire [154:154] mux_1level_tapbuf_size2_144_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_144_configbus0[154:154] = sram_blwl_bl[154:154] ;
assign mux_1level_tapbuf_size2_144_configbus1[154:154] = sram_blwl_wl[154:154] ;
wire [154:154] mux_1level_tapbuf_size2_144_configbus0_b;
assign mux_1level_tapbuf_size2_144_configbus0_b[154:154] = sram_blwl_blb[154:154] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_144_ (mux_1level_tapbuf_size2_144_inbus, chanx_1__1__out_88_ , mux_1level_tapbuf_size2_144_sram_blwl_out[154:154] ,
mux_1level_tapbuf_size2_144_sram_blwl_outb[154:154] );
//----- SRAM bits for MUX[144], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_154_ (mux_1level_tapbuf_size2_144_sram_blwl_out[154:154] ,mux_1level_tapbuf_size2_144_sram_blwl_out[154:154] ,mux_1level_tapbuf_size2_144_sram_blwl_outb[154:154] ,mux_1level_tapbuf_size2_144_configbus0[154:154], mux_1level_tapbuf_size2_144_configbus1[154:154] , mux_1level_tapbuf_size2_144_configbus0_b[154:154] );
wire [0:1] mux_1level_tapbuf_size2_145_inbus;
assign mux_1level_tapbuf_size2_145_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_145_inbus[1] = chany_0__1__in_6_ ;
wire [155:155] mux_1level_tapbuf_size2_145_configbus0;
wire [155:155] mux_1level_tapbuf_size2_145_configbus1;
wire [155:155] mux_1level_tapbuf_size2_145_sram_blwl_out ;
wire [155:155] mux_1level_tapbuf_size2_145_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_145_configbus0[155:155] = sram_blwl_bl[155:155] ;
assign mux_1level_tapbuf_size2_145_configbus1[155:155] = sram_blwl_wl[155:155] ;
wire [155:155] mux_1level_tapbuf_size2_145_configbus0_b;
assign mux_1level_tapbuf_size2_145_configbus0_b[155:155] = sram_blwl_blb[155:155] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_145_ (mux_1level_tapbuf_size2_145_inbus, chanx_1__1__out_90_ , mux_1level_tapbuf_size2_145_sram_blwl_out[155:155] ,
mux_1level_tapbuf_size2_145_sram_blwl_outb[155:155] );
//----- SRAM bits for MUX[145], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_155_ (mux_1level_tapbuf_size2_145_sram_blwl_out[155:155] ,mux_1level_tapbuf_size2_145_sram_blwl_out[155:155] ,mux_1level_tapbuf_size2_145_sram_blwl_outb[155:155] ,mux_1level_tapbuf_size2_145_configbus0[155:155], mux_1level_tapbuf_size2_145_configbus1[155:155] , mux_1level_tapbuf_size2_145_configbus0_b[155:155] );
wire [0:1] mux_1level_tapbuf_size2_146_inbus;
assign mux_1level_tapbuf_size2_146_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_146_inbus[1] = chany_0__1__in_4_ ;
wire [156:156] mux_1level_tapbuf_size2_146_configbus0;
wire [156:156] mux_1level_tapbuf_size2_146_configbus1;
wire [156:156] mux_1level_tapbuf_size2_146_sram_blwl_out ;
wire [156:156] mux_1level_tapbuf_size2_146_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_146_configbus0[156:156] = sram_blwl_bl[156:156] ;
assign mux_1level_tapbuf_size2_146_configbus1[156:156] = sram_blwl_wl[156:156] ;
wire [156:156] mux_1level_tapbuf_size2_146_configbus0_b;
assign mux_1level_tapbuf_size2_146_configbus0_b[156:156] = sram_blwl_blb[156:156] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_146_ (mux_1level_tapbuf_size2_146_inbus, chanx_1__1__out_92_ , mux_1level_tapbuf_size2_146_sram_blwl_out[156:156] ,
mux_1level_tapbuf_size2_146_sram_blwl_outb[156:156] );
//----- SRAM bits for MUX[146], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_156_ (mux_1level_tapbuf_size2_146_sram_blwl_out[156:156] ,mux_1level_tapbuf_size2_146_sram_blwl_out[156:156] ,mux_1level_tapbuf_size2_146_sram_blwl_outb[156:156] ,mux_1level_tapbuf_size2_146_configbus0[156:156], mux_1level_tapbuf_size2_146_configbus1[156:156] , mux_1level_tapbuf_size2_146_configbus0_b[156:156] );
wire [0:1] mux_1level_tapbuf_size2_147_inbus;
assign mux_1level_tapbuf_size2_147_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_147_inbus[1] = chany_0__1__in_2_ ;
wire [157:157] mux_1level_tapbuf_size2_147_configbus0;
wire [157:157] mux_1level_tapbuf_size2_147_configbus1;
wire [157:157] mux_1level_tapbuf_size2_147_sram_blwl_out ;
wire [157:157] mux_1level_tapbuf_size2_147_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_147_configbus0[157:157] = sram_blwl_bl[157:157] ;
assign mux_1level_tapbuf_size2_147_configbus1[157:157] = sram_blwl_wl[157:157] ;
wire [157:157] mux_1level_tapbuf_size2_147_configbus0_b;
assign mux_1level_tapbuf_size2_147_configbus0_b[157:157] = sram_blwl_blb[157:157] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_147_ (mux_1level_tapbuf_size2_147_inbus, chanx_1__1__out_94_ , mux_1level_tapbuf_size2_147_sram_blwl_out[157:157] ,
mux_1level_tapbuf_size2_147_sram_blwl_outb[157:157] );
//----- SRAM bits for MUX[147], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_157_ (mux_1level_tapbuf_size2_147_sram_blwl_out[157:157] ,mux_1level_tapbuf_size2_147_sram_blwl_out[157:157] ,mux_1level_tapbuf_size2_147_sram_blwl_outb[157:157] ,mux_1level_tapbuf_size2_147_configbus0[157:157], mux_1level_tapbuf_size2_147_configbus1[157:157] , mux_1level_tapbuf_size2_147_configbus0_b[157:157] );
wire [0:1] mux_1level_tapbuf_size2_148_inbus;
assign mux_1level_tapbuf_size2_148_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_148_inbus[1] = chany_0__1__in_0_ ;
wire [158:158] mux_1level_tapbuf_size2_148_configbus0;
wire [158:158] mux_1level_tapbuf_size2_148_configbus1;
wire [158:158] mux_1level_tapbuf_size2_148_sram_blwl_out ;
wire [158:158] mux_1level_tapbuf_size2_148_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_148_configbus0[158:158] = sram_blwl_bl[158:158] ;
assign mux_1level_tapbuf_size2_148_configbus1[158:158] = sram_blwl_wl[158:158] ;
wire [158:158] mux_1level_tapbuf_size2_148_configbus0_b;
assign mux_1level_tapbuf_size2_148_configbus0_b[158:158] = sram_blwl_blb[158:158] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_148_ (mux_1level_tapbuf_size2_148_inbus, chanx_1__1__out_96_ , mux_1level_tapbuf_size2_148_sram_blwl_out[158:158] ,
mux_1level_tapbuf_size2_148_sram_blwl_outb[158:158] );
//----- SRAM bits for MUX[148], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_158_ (mux_1level_tapbuf_size2_148_sram_blwl_out[158:158] ,mux_1level_tapbuf_size2_148_sram_blwl_out[158:158] ,mux_1level_tapbuf_size2_148_sram_blwl_outb[158:158] ,mux_1level_tapbuf_size2_148_configbus0[158:158], mux_1level_tapbuf_size2_148_configbus1[158:158] , mux_1level_tapbuf_size2_148_configbus0_b[158:158] );
wire [0:1] mux_1level_tapbuf_size2_149_inbus;
assign mux_1level_tapbuf_size2_149_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_149_inbus[1] = chany_0__1__in_98_ ;
wire [159:159] mux_1level_tapbuf_size2_149_configbus0;
wire [159:159] mux_1level_tapbuf_size2_149_configbus1;
wire [159:159] mux_1level_tapbuf_size2_149_sram_blwl_out ;
wire [159:159] mux_1level_tapbuf_size2_149_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_149_configbus0[159:159] = sram_blwl_bl[159:159] ;
assign mux_1level_tapbuf_size2_149_configbus1[159:159] = sram_blwl_wl[159:159] ;
wire [159:159] mux_1level_tapbuf_size2_149_configbus0_b;
assign mux_1level_tapbuf_size2_149_configbus0_b[159:159] = sram_blwl_blb[159:159] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_149_ (mux_1level_tapbuf_size2_149_inbus, chanx_1__1__out_98_ , mux_1level_tapbuf_size2_149_sram_blwl_out[159:159] ,
mux_1level_tapbuf_size2_149_sram_blwl_outb[159:159] );
//----- SRAM bits for MUX[149], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_159_ (mux_1level_tapbuf_size2_149_sram_blwl_out[159:159] ,mux_1level_tapbuf_size2_149_sram_blwl_out[159:159] ,mux_1level_tapbuf_size2_149_sram_blwl_outb[159:159] ,mux_1level_tapbuf_size2_149_configbus0[159:159], mux_1level_tapbuf_size2_149_configbus1[159:159] , mux_1level_tapbuf_size2_149_configbus0_b[159:159] );
//----- bottom side Multiplexers -----
wire [0:1] mux_1level_tapbuf_size2_150_inbus;
assign mux_1level_tapbuf_size2_150_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_150_inbus[1] = chanx_1__1__in_97_ ;
wire [160:160] mux_1level_tapbuf_size2_150_configbus0;
wire [160:160] mux_1level_tapbuf_size2_150_configbus1;
wire [160:160] mux_1level_tapbuf_size2_150_sram_blwl_out ;
wire [160:160] mux_1level_tapbuf_size2_150_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_150_configbus0[160:160] = sram_blwl_bl[160:160] ;
assign mux_1level_tapbuf_size2_150_configbus1[160:160] = sram_blwl_wl[160:160] ;
wire [160:160] mux_1level_tapbuf_size2_150_configbus0_b;
assign mux_1level_tapbuf_size2_150_configbus0_b[160:160] = sram_blwl_blb[160:160] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_150_ (mux_1level_tapbuf_size2_150_inbus, chany_0__1__out_1_ , mux_1level_tapbuf_size2_150_sram_blwl_out[160:160] ,
mux_1level_tapbuf_size2_150_sram_blwl_outb[160:160] );
//----- SRAM bits for MUX[150], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_160_ (mux_1level_tapbuf_size2_150_sram_blwl_out[160:160] ,mux_1level_tapbuf_size2_150_sram_blwl_out[160:160] ,mux_1level_tapbuf_size2_150_sram_blwl_outb[160:160] ,mux_1level_tapbuf_size2_150_configbus0[160:160], mux_1level_tapbuf_size2_150_configbus1[160:160] , mux_1level_tapbuf_size2_150_configbus0_b[160:160] );
wire [0:1] mux_1level_tapbuf_size2_151_inbus;
assign mux_1level_tapbuf_size2_151_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_151_inbus[1] = chanx_1__1__in_95_ ;
wire [161:161] mux_1level_tapbuf_size2_151_configbus0;
wire [161:161] mux_1level_tapbuf_size2_151_configbus1;
wire [161:161] mux_1level_tapbuf_size2_151_sram_blwl_out ;
wire [161:161] mux_1level_tapbuf_size2_151_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_151_configbus0[161:161] = sram_blwl_bl[161:161] ;
assign mux_1level_tapbuf_size2_151_configbus1[161:161] = sram_blwl_wl[161:161] ;
wire [161:161] mux_1level_tapbuf_size2_151_configbus0_b;
assign mux_1level_tapbuf_size2_151_configbus0_b[161:161] = sram_blwl_blb[161:161] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_151_ (mux_1level_tapbuf_size2_151_inbus, chany_0__1__out_3_ , mux_1level_tapbuf_size2_151_sram_blwl_out[161:161] ,
mux_1level_tapbuf_size2_151_sram_blwl_outb[161:161] );
//----- SRAM bits for MUX[151], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_161_ (mux_1level_tapbuf_size2_151_sram_blwl_out[161:161] ,mux_1level_tapbuf_size2_151_sram_blwl_out[161:161] ,mux_1level_tapbuf_size2_151_sram_blwl_outb[161:161] ,mux_1level_tapbuf_size2_151_configbus0[161:161], mux_1level_tapbuf_size2_151_configbus1[161:161] , mux_1level_tapbuf_size2_151_configbus0_b[161:161] );
wire [0:1] mux_1level_tapbuf_size2_152_inbus;
assign mux_1level_tapbuf_size2_152_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_152_inbus[1] = chanx_1__1__in_93_ ;
wire [162:162] mux_1level_tapbuf_size2_152_configbus0;
wire [162:162] mux_1level_tapbuf_size2_152_configbus1;
wire [162:162] mux_1level_tapbuf_size2_152_sram_blwl_out ;
wire [162:162] mux_1level_tapbuf_size2_152_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_152_configbus0[162:162] = sram_blwl_bl[162:162] ;
assign mux_1level_tapbuf_size2_152_configbus1[162:162] = sram_blwl_wl[162:162] ;
wire [162:162] mux_1level_tapbuf_size2_152_configbus0_b;
assign mux_1level_tapbuf_size2_152_configbus0_b[162:162] = sram_blwl_blb[162:162] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_152_ (mux_1level_tapbuf_size2_152_inbus, chany_0__1__out_5_ , mux_1level_tapbuf_size2_152_sram_blwl_out[162:162] ,
mux_1level_tapbuf_size2_152_sram_blwl_outb[162:162] );
//----- SRAM bits for MUX[152], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_162_ (mux_1level_tapbuf_size2_152_sram_blwl_out[162:162] ,mux_1level_tapbuf_size2_152_sram_blwl_out[162:162] ,mux_1level_tapbuf_size2_152_sram_blwl_outb[162:162] ,mux_1level_tapbuf_size2_152_configbus0[162:162], mux_1level_tapbuf_size2_152_configbus1[162:162] , mux_1level_tapbuf_size2_152_configbus0_b[162:162] );
wire [0:1] mux_1level_tapbuf_size2_153_inbus;
assign mux_1level_tapbuf_size2_153_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_153_inbus[1] = chanx_1__1__in_91_ ;
wire [163:163] mux_1level_tapbuf_size2_153_configbus0;
wire [163:163] mux_1level_tapbuf_size2_153_configbus1;
wire [163:163] mux_1level_tapbuf_size2_153_sram_blwl_out ;
wire [163:163] mux_1level_tapbuf_size2_153_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_153_configbus0[163:163] = sram_blwl_bl[163:163] ;
assign mux_1level_tapbuf_size2_153_configbus1[163:163] = sram_blwl_wl[163:163] ;
wire [163:163] mux_1level_tapbuf_size2_153_configbus0_b;
assign mux_1level_tapbuf_size2_153_configbus0_b[163:163] = sram_blwl_blb[163:163] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_153_ (mux_1level_tapbuf_size2_153_inbus, chany_0__1__out_7_ , mux_1level_tapbuf_size2_153_sram_blwl_out[163:163] ,
mux_1level_tapbuf_size2_153_sram_blwl_outb[163:163] );
//----- SRAM bits for MUX[153], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_163_ (mux_1level_tapbuf_size2_153_sram_blwl_out[163:163] ,mux_1level_tapbuf_size2_153_sram_blwl_out[163:163] ,mux_1level_tapbuf_size2_153_sram_blwl_outb[163:163] ,mux_1level_tapbuf_size2_153_configbus0[163:163], mux_1level_tapbuf_size2_153_configbus1[163:163] , mux_1level_tapbuf_size2_153_configbus0_b[163:163] );
wire [0:1] mux_1level_tapbuf_size2_154_inbus;
assign mux_1level_tapbuf_size2_154_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_154_inbus[1] = chanx_1__1__in_89_ ;
wire [164:164] mux_1level_tapbuf_size2_154_configbus0;
wire [164:164] mux_1level_tapbuf_size2_154_configbus1;
wire [164:164] mux_1level_tapbuf_size2_154_sram_blwl_out ;
wire [164:164] mux_1level_tapbuf_size2_154_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_154_configbus0[164:164] = sram_blwl_bl[164:164] ;
assign mux_1level_tapbuf_size2_154_configbus1[164:164] = sram_blwl_wl[164:164] ;
wire [164:164] mux_1level_tapbuf_size2_154_configbus0_b;
assign mux_1level_tapbuf_size2_154_configbus0_b[164:164] = sram_blwl_blb[164:164] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_154_ (mux_1level_tapbuf_size2_154_inbus, chany_0__1__out_9_ , mux_1level_tapbuf_size2_154_sram_blwl_out[164:164] ,
mux_1level_tapbuf_size2_154_sram_blwl_outb[164:164] );
//----- SRAM bits for MUX[154], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_164_ (mux_1level_tapbuf_size2_154_sram_blwl_out[164:164] ,mux_1level_tapbuf_size2_154_sram_blwl_out[164:164] ,mux_1level_tapbuf_size2_154_sram_blwl_outb[164:164] ,mux_1level_tapbuf_size2_154_configbus0[164:164], mux_1level_tapbuf_size2_154_configbus1[164:164] , mux_1level_tapbuf_size2_154_configbus0_b[164:164] );
wire [0:1] mux_1level_tapbuf_size2_155_inbus;
assign mux_1level_tapbuf_size2_155_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_155_inbus[1] = chanx_1__1__in_87_ ;
wire [165:165] mux_1level_tapbuf_size2_155_configbus0;
wire [165:165] mux_1level_tapbuf_size2_155_configbus1;
wire [165:165] mux_1level_tapbuf_size2_155_sram_blwl_out ;
wire [165:165] mux_1level_tapbuf_size2_155_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_155_configbus0[165:165] = sram_blwl_bl[165:165] ;
assign mux_1level_tapbuf_size2_155_configbus1[165:165] = sram_blwl_wl[165:165] ;
wire [165:165] mux_1level_tapbuf_size2_155_configbus0_b;
assign mux_1level_tapbuf_size2_155_configbus0_b[165:165] = sram_blwl_blb[165:165] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_155_ (mux_1level_tapbuf_size2_155_inbus, chany_0__1__out_11_ , mux_1level_tapbuf_size2_155_sram_blwl_out[165:165] ,
mux_1level_tapbuf_size2_155_sram_blwl_outb[165:165] );
//----- SRAM bits for MUX[155], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_165_ (mux_1level_tapbuf_size2_155_sram_blwl_out[165:165] ,mux_1level_tapbuf_size2_155_sram_blwl_out[165:165] ,mux_1level_tapbuf_size2_155_sram_blwl_outb[165:165] ,mux_1level_tapbuf_size2_155_configbus0[165:165], mux_1level_tapbuf_size2_155_configbus1[165:165] , mux_1level_tapbuf_size2_155_configbus0_b[165:165] );
wire [0:1] mux_1level_tapbuf_size2_156_inbus;
assign mux_1level_tapbuf_size2_156_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_156_inbus[1] = chanx_1__1__in_85_ ;
wire [166:166] mux_1level_tapbuf_size2_156_configbus0;
wire [166:166] mux_1level_tapbuf_size2_156_configbus1;
wire [166:166] mux_1level_tapbuf_size2_156_sram_blwl_out ;
wire [166:166] mux_1level_tapbuf_size2_156_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_156_configbus0[166:166] = sram_blwl_bl[166:166] ;
assign mux_1level_tapbuf_size2_156_configbus1[166:166] = sram_blwl_wl[166:166] ;
wire [166:166] mux_1level_tapbuf_size2_156_configbus0_b;
assign mux_1level_tapbuf_size2_156_configbus0_b[166:166] = sram_blwl_blb[166:166] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_156_ (mux_1level_tapbuf_size2_156_inbus, chany_0__1__out_13_ , mux_1level_tapbuf_size2_156_sram_blwl_out[166:166] ,
mux_1level_tapbuf_size2_156_sram_blwl_outb[166:166] );
//----- SRAM bits for MUX[156], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_166_ (mux_1level_tapbuf_size2_156_sram_blwl_out[166:166] ,mux_1level_tapbuf_size2_156_sram_blwl_out[166:166] ,mux_1level_tapbuf_size2_156_sram_blwl_outb[166:166] ,mux_1level_tapbuf_size2_156_configbus0[166:166], mux_1level_tapbuf_size2_156_configbus1[166:166] , mux_1level_tapbuf_size2_156_configbus0_b[166:166] );
wire [0:1] mux_1level_tapbuf_size2_157_inbus;
assign mux_1level_tapbuf_size2_157_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_157_inbus[1] = chanx_1__1__in_83_ ;
wire [167:167] mux_1level_tapbuf_size2_157_configbus0;
wire [167:167] mux_1level_tapbuf_size2_157_configbus1;
wire [167:167] mux_1level_tapbuf_size2_157_sram_blwl_out ;
wire [167:167] mux_1level_tapbuf_size2_157_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_157_configbus0[167:167] = sram_blwl_bl[167:167] ;
assign mux_1level_tapbuf_size2_157_configbus1[167:167] = sram_blwl_wl[167:167] ;
wire [167:167] mux_1level_tapbuf_size2_157_configbus0_b;
assign mux_1level_tapbuf_size2_157_configbus0_b[167:167] = sram_blwl_blb[167:167] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_157_ (mux_1level_tapbuf_size2_157_inbus, chany_0__1__out_15_ , mux_1level_tapbuf_size2_157_sram_blwl_out[167:167] ,
mux_1level_tapbuf_size2_157_sram_blwl_outb[167:167] );
//----- SRAM bits for MUX[157], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_167_ (mux_1level_tapbuf_size2_157_sram_blwl_out[167:167] ,mux_1level_tapbuf_size2_157_sram_blwl_out[167:167] ,mux_1level_tapbuf_size2_157_sram_blwl_outb[167:167] ,mux_1level_tapbuf_size2_157_configbus0[167:167], mux_1level_tapbuf_size2_157_configbus1[167:167] , mux_1level_tapbuf_size2_157_configbus0_b[167:167] );
wire [0:1] mux_1level_tapbuf_size2_158_inbus;
assign mux_1level_tapbuf_size2_158_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_158_inbus[1] = chanx_1__1__in_81_ ;
wire [168:168] mux_1level_tapbuf_size2_158_configbus0;
wire [168:168] mux_1level_tapbuf_size2_158_configbus1;
wire [168:168] mux_1level_tapbuf_size2_158_sram_blwl_out ;
wire [168:168] mux_1level_tapbuf_size2_158_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_158_configbus0[168:168] = sram_blwl_bl[168:168] ;
assign mux_1level_tapbuf_size2_158_configbus1[168:168] = sram_blwl_wl[168:168] ;
wire [168:168] mux_1level_tapbuf_size2_158_configbus0_b;
assign mux_1level_tapbuf_size2_158_configbus0_b[168:168] = sram_blwl_blb[168:168] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_158_ (mux_1level_tapbuf_size2_158_inbus, chany_0__1__out_17_ , mux_1level_tapbuf_size2_158_sram_blwl_out[168:168] ,
mux_1level_tapbuf_size2_158_sram_blwl_outb[168:168] );
//----- SRAM bits for MUX[158], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_168_ (mux_1level_tapbuf_size2_158_sram_blwl_out[168:168] ,mux_1level_tapbuf_size2_158_sram_blwl_out[168:168] ,mux_1level_tapbuf_size2_158_sram_blwl_outb[168:168] ,mux_1level_tapbuf_size2_158_configbus0[168:168], mux_1level_tapbuf_size2_158_configbus1[168:168] , mux_1level_tapbuf_size2_158_configbus0_b[168:168] );
wire [0:1] mux_1level_tapbuf_size2_159_inbus;
assign mux_1level_tapbuf_size2_159_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_159_inbus[1] = chanx_1__1__in_79_ ;
wire [169:169] mux_1level_tapbuf_size2_159_configbus0;
wire [169:169] mux_1level_tapbuf_size2_159_configbus1;
wire [169:169] mux_1level_tapbuf_size2_159_sram_blwl_out ;
wire [169:169] mux_1level_tapbuf_size2_159_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_159_configbus0[169:169] = sram_blwl_bl[169:169] ;
assign mux_1level_tapbuf_size2_159_configbus1[169:169] = sram_blwl_wl[169:169] ;
wire [169:169] mux_1level_tapbuf_size2_159_configbus0_b;
assign mux_1level_tapbuf_size2_159_configbus0_b[169:169] = sram_blwl_blb[169:169] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_159_ (mux_1level_tapbuf_size2_159_inbus, chany_0__1__out_19_ , mux_1level_tapbuf_size2_159_sram_blwl_out[169:169] ,
mux_1level_tapbuf_size2_159_sram_blwl_outb[169:169] );
//----- SRAM bits for MUX[159], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_169_ (mux_1level_tapbuf_size2_159_sram_blwl_out[169:169] ,mux_1level_tapbuf_size2_159_sram_blwl_out[169:169] ,mux_1level_tapbuf_size2_159_sram_blwl_outb[169:169] ,mux_1level_tapbuf_size2_159_configbus0[169:169], mux_1level_tapbuf_size2_159_configbus1[169:169] , mux_1level_tapbuf_size2_159_configbus0_b[169:169] );
wire [0:1] mux_1level_tapbuf_size2_160_inbus;
assign mux_1level_tapbuf_size2_160_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_160_inbus[1] = chanx_1__1__in_77_ ;
wire [170:170] mux_1level_tapbuf_size2_160_configbus0;
wire [170:170] mux_1level_tapbuf_size2_160_configbus1;
wire [170:170] mux_1level_tapbuf_size2_160_sram_blwl_out ;
wire [170:170] mux_1level_tapbuf_size2_160_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_160_configbus0[170:170] = sram_blwl_bl[170:170] ;
assign mux_1level_tapbuf_size2_160_configbus1[170:170] = sram_blwl_wl[170:170] ;
wire [170:170] mux_1level_tapbuf_size2_160_configbus0_b;
assign mux_1level_tapbuf_size2_160_configbus0_b[170:170] = sram_blwl_blb[170:170] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_160_ (mux_1level_tapbuf_size2_160_inbus, chany_0__1__out_21_ , mux_1level_tapbuf_size2_160_sram_blwl_out[170:170] ,
mux_1level_tapbuf_size2_160_sram_blwl_outb[170:170] );
//----- SRAM bits for MUX[160], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_170_ (mux_1level_tapbuf_size2_160_sram_blwl_out[170:170] ,mux_1level_tapbuf_size2_160_sram_blwl_out[170:170] ,mux_1level_tapbuf_size2_160_sram_blwl_outb[170:170] ,mux_1level_tapbuf_size2_160_configbus0[170:170], mux_1level_tapbuf_size2_160_configbus1[170:170] , mux_1level_tapbuf_size2_160_configbus0_b[170:170] );
wire [0:1] mux_1level_tapbuf_size2_161_inbus;
assign mux_1level_tapbuf_size2_161_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_161_inbus[1] = chanx_1__1__in_75_ ;
wire [171:171] mux_1level_tapbuf_size2_161_configbus0;
wire [171:171] mux_1level_tapbuf_size2_161_configbus1;
wire [171:171] mux_1level_tapbuf_size2_161_sram_blwl_out ;
wire [171:171] mux_1level_tapbuf_size2_161_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_161_configbus0[171:171] = sram_blwl_bl[171:171] ;
assign mux_1level_tapbuf_size2_161_configbus1[171:171] = sram_blwl_wl[171:171] ;
wire [171:171] mux_1level_tapbuf_size2_161_configbus0_b;
assign mux_1level_tapbuf_size2_161_configbus0_b[171:171] = sram_blwl_blb[171:171] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_161_ (mux_1level_tapbuf_size2_161_inbus, chany_0__1__out_23_ , mux_1level_tapbuf_size2_161_sram_blwl_out[171:171] ,
mux_1level_tapbuf_size2_161_sram_blwl_outb[171:171] );
//----- SRAM bits for MUX[161], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_171_ (mux_1level_tapbuf_size2_161_sram_blwl_out[171:171] ,mux_1level_tapbuf_size2_161_sram_blwl_out[171:171] ,mux_1level_tapbuf_size2_161_sram_blwl_outb[171:171] ,mux_1level_tapbuf_size2_161_configbus0[171:171], mux_1level_tapbuf_size2_161_configbus1[171:171] , mux_1level_tapbuf_size2_161_configbus0_b[171:171] );
wire [0:1] mux_1level_tapbuf_size2_162_inbus;
assign mux_1level_tapbuf_size2_162_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_162_inbus[1] = chanx_1__1__in_73_ ;
wire [172:172] mux_1level_tapbuf_size2_162_configbus0;
wire [172:172] mux_1level_tapbuf_size2_162_configbus1;
wire [172:172] mux_1level_tapbuf_size2_162_sram_blwl_out ;
wire [172:172] mux_1level_tapbuf_size2_162_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_162_configbus0[172:172] = sram_blwl_bl[172:172] ;
assign mux_1level_tapbuf_size2_162_configbus1[172:172] = sram_blwl_wl[172:172] ;
wire [172:172] mux_1level_tapbuf_size2_162_configbus0_b;
assign mux_1level_tapbuf_size2_162_configbus0_b[172:172] = sram_blwl_blb[172:172] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_162_ (mux_1level_tapbuf_size2_162_inbus, chany_0__1__out_25_ , mux_1level_tapbuf_size2_162_sram_blwl_out[172:172] ,
mux_1level_tapbuf_size2_162_sram_blwl_outb[172:172] );
//----- SRAM bits for MUX[162], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_172_ (mux_1level_tapbuf_size2_162_sram_blwl_out[172:172] ,mux_1level_tapbuf_size2_162_sram_blwl_out[172:172] ,mux_1level_tapbuf_size2_162_sram_blwl_outb[172:172] ,mux_1level_tapbuf_size2_162_configbus0[172:172], mux_1level_tapbuf_size2_162_configbus1[172:172] , mux_1level_tapbuf_size2_162_configbus0_b[172:172] );
wire [0:1] mux_1level_tapbuf_size2_163_inbus;
assign mux_1level_tapbuf_size2_163_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_163_inbus[1] = chanx_1__1__in_71_ ;
wire [173:173] mux_1level_tapbuf_size2_163_configbus0;
wire [173:173] mux_1level_tapbuf_size2_163_configbus1;
wire [173:173] mux_1level_tapbuf_size2_163_sram_blwl_out ;
wire [173:173] mux_1level_tapbuf_size2_163_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_163_configbus0[173:173] = sram_blwl_bl[173:173] ;
assign mux_1level_tapbuf_size2_163_configbus1[173:173] = sram_blwl_wl[173:173] ;
wire [173:173] mux_1level_tapbuf_size2_163_configbus0_b;
assign mux_1level_tapbuf_size2_163_configbus0_b[173:173] = sram_blwl_blb[173:173] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_163_ (mux_1level_tapbuf_size2_163_inbus, chany_0__1__out_27_ , mux_1level_tapbuf_size2_163_sram_blwl_out[173:173] ,
mux_1level_tapbuf_size2_163_sram_blwl_outb[173:173] );
//----- SRAM bits for MUX[163], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_173_ (mux_1level_tapbuf_size2_163_sram_blwl_out[173:173] ,mux_1level_tapbuf_size2_163_sram_blwl_out[173:173] ,mux_1level_tapbuf_size2_163_sram_blwl_outb[173:173] ,mux_1level_tapbuf_size2_163_configbus0[173:173], mux_1level_tapbuf_size2_163_configbus1[173:173] , mux_1level_tapbuf_size2_163_configbus0_b[173:173] );
wire [0:1] mux_1level_tapbuf_size2_164_inbus;
assign mux_1level_tapbuf_size2_164_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_164_inbus[1] = chanx_1__1__in_69_ ;
wire [174:174] mux_1level_tapbuf_size2_164_configbus0;
wire [174:174] mux_1level_tapbuf_size2_164_configbus1;
wire [174:174] mux_1level_tapbuf_size2_164_sram_blwl_out ;
wire [174:174] mux_1level_tapbuf_size2_164_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_164_configbus0[174:174] = sram_blwl_bl[174:174] ;
assign mux_1level_tapbuf_size2_164_configbus1[174:174] = sram_blwl_wl[174:174] ;
wire [174:174] mux_1level_tapbuf_size2_164_configbus0_b;
assign mux_1level_tapbuf_size2_164_configbus0_b[174:174] = sram_blwl_blb[174:174] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_164_ (mux_1level_tapbuf_size2_164_inbus, chany_0__1__out_29_ , mux_1level_tapbuf_size2_164_sram_blwl_out[174:174] ,
mux_1level_tapbuf_size2_164_sram_blwl_outb[174:174] );
//----- SRAM bits for MUX[164], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_174_ (mux_1level_tapbuf_size2_164_sram_blwl_out[174:174] ,mux_1level_tapbuf_size2_164_sram_blwl_out[174:174] ,mux_1level_tapbuf_size2_164_sram_blwl_outb[174:174] ,mux_1level_tapbuf_size2_164_configbus0[174:174], mux_1level_tapbuf_size2_164_configbus1[174:174] , mux_1level_tapbuf_size2_164_configbus0_b[174:174] );
wire [0:1] mux_1level_tapbuf_size2_165_inbus;
assign mux_1level_tapbuf_size2_165_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_165_inbus[1] = chanx_1__1__in_67_ ;
wire [175:175] mux_1level_tapbuf_size2_165_configbus0;
wire [175:175] mux_1level_tapbuf_size2_165_configbus1;
wire [175:175] mux_1level_tapbuf_size2_165_sram_blwl_out ;
wire [175:175] mux_1level_tapbuf_size2_165_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_165_configbus0[175:175] = sram_blwl_bl[175:175] ;
assign mux_1level_tapbuf_size2_165_configbus1[175:175] = sram_blwl_wl[175:175] ;
wire [175:175] mux_1level_tapbuf_size2_165_configbus0_b;
assign mux_1level_tapbuf_size2_165_configbus0_b[175:175] = sram_blwl_blb[175:175] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_165_ (mux_1level_tapbuf_size2_165_inbus, chany_0__1__out_31_ , mux_1level_tapbuf_size2_165_sram_blwl_out[175:175] ,
mux_1level_tapbuf_size2_165_sram_blwl_outb[175:175] );
//----- SRAM bits for MUX[165], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_175_ (mux_1level_tapbuf_size2_165_sram_blwl_out[175:175] ,mux_1level_tapbuf_size2_165_sram_blwl_out[175:175] ,mux_1level_tapbuf_size2_165_sram_blwl_outb[175:175] ,mux_1level_tapbuf_size2_165_configbus0[175:175], mux_1level_tapbuf_size2_165_configbus1[175:175] , mux_1level_tapbuf_size2_165_configbus0_b[175:175] );
wire [0:1] mux_1level_tapbuf_size2_166_inbus;
assign mux_1level_tapbuf_size2_166_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_166_inbus[1] = chanx_1__1__in_65_ ;
wire [176:176] mux_1level_tapbuf_size2_166_configbus0;
wire [176:176] mux_1level_tapbuf_size2_166_configbus1;
wire [176:176] mux_1level_tapbuf_size2_166_sram_blwl_out ;
wire [176:176] mux_1level_tapbuf_size2_166_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_166_configbus0[176:176] = sram_blwl_bl[176:176] ;
assign mux_1level_tapbuf_size2_166_configbus1[176:176] = sram_blwl_wl[176:176] ;
wire [176:176] mux_1level_tapbuf_size2_166_configbus0_b;
assign mux_1level_tapbuf_size2_166_configbus0_b[176:176] = sram_blwl_blb[176:176] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_166_ (mux_1level_tapbuf_size2_166_inbus, chany_0__1__out_33_ , mux_1level_tapbuf_size2_166_sram_blwl_out[176:176] ,
mux_1level_tapbuf_size2_166_sram_blwl_outb[176:176] );
//----- SRAM bits for MUX[166], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_176_ (mux_1level_tapbuf_size2_166_sram_blwl_out[176:176] ,mux_1level_tapbuf_size2_166_sram_blwl_out[176:176] ,mux_1level_tapbuf_size2_166_sram_blwl_outb[176:176] ,mux_1level_tapbuf_size2_166_configbus0[176:176], mux_1level_tapbuf_size2_166_configbus1[176:176] , mux_1level_tapbuf_size2_166_configbus0_b[176:176] );
wire [0:1] mux_1level_tapbuf_size2_167_inbus;
assign mux_1level_tapbuf_size2_167_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_167_inbus[1] = chanx_1__1__in_63_ ;
wire [177:177] mux_1level_tapbuf_size2_167_configbus0;
wire [177:177] mux_1level_tapbuf_size2_167_configbus1;
wire [177:177] mux_1level_tapbuf_size2_167_sram_blwl_out ;
wire [177:177] mux_1level_tapbuf_size2_167_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_167_configbus0[177:177] = sram_blwl_bl[177:177] ;
assign mux_1level_tapbuf_size2_167_configbus1[177:177] = sram_blwl_wl[177:177] ;
wire [177:177] mux_1level_tapbuf_size2_167_configbus0_b;
assign mux_1level_tapbuf_size2_167_configbus0_b[177:177] = sram_blwl_blb[177:177] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_167_ (mux_1level_tapbuf_size2_167_inbus, chany_0__1__out_35_ , mux_1level_tapbuf_size2_167_sram_blwl_out[177:177] ,
mux_1level_tapbuf_size2_167_sram_blwl_outb[177:177] );
//----- SRAM bits for MUX[167], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_177_ (mux_1level_tapbuf_size2_167_sram_blwl_out[177:177] ,mux_1level_tapbuf_size2_167_sram_blwl_out[177:177] ,mux_1level_tapbuf_size2_167_sram_blwl_outb[177:177] ,mux_1level_tapbuf_size2_167_configbus0[177:177], mux_1level_tapbuf_size2_167_configbus1[177:177] , mux_1level_tapbuf_size2_167_configbus0_b[177:177] );
wire [0:1] mux_1level_tapbuf_size2_168_inbus;
assign mux_1level_tapbuf_size2_168_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_168_inbus[1] = chanx_1__1__in_61_ ;
wire [178:178] mux_1level_tapbuf_size2_168_configbus0;
wire [178:178] mux_1level_tapbuf_size2_168_configbus1;
wire [178:178] mux_1level_tapbuf_size2_168_sram_blwl_out ;
wire [178:178] mux_1level_tapbuf_size2_168_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_168_configbus0[178:178] = sram_blwl_bl[178:178] ;
assign mux_1level_tapbuf_size2_168_configbus1[178:178] = sram_blwl_wl[178:178] ;
wire [178:178] mux_1level_tapbuf_size2_168_configbus0_b;
assign mux_1level_tapbuf_size2_168_configbus0_b[178:178] = sram_blwl_blb[178:178] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_168_ (mux_1level_tapbuf_size2_168_inbus, chany_0__1__out_37_ , mux_1level_tapbuf_size2_168_sram_blwl_out[178:178] ,
mux_1level_tapbuf_size2_168_sram_blwl_outb[178:178] );
//----- SRAM bits for MUX[168], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_178_ (mux_1level_tapbuf_size2_168_sram_blwl_out[178:178] ,mux_1level_tapbuf_size2_168_sram_blwl_out[178:178] ,mux_1level_tapbuf_size2_168_sram_blwl_outb[178:178] ,mux_1level_tapbuf_size2_168_configbus0[178:178], mux_1level_tapbuf_size2_168_configbus1[178:178] , mux_1level_tapbuf_size2_168_configbus0_b[178:178] );
wire [0:1] mux_1level_tapbuf_size2_169_inbus;
assign mux_1level_tapbuf_size2_169_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_169_inbus[1] = chanx_1__1__in_59_ ;
wire [179:179] mux_1level_tapbuf_size2_169_configbus0;
wire [179:179] mux_1level_tapbuf_size2_169_configbus1;
wire [179:179] mux_1level_tapbuf_size2_169_sram_blwl_out ;
wire [179:179] mux_1level_tapbuf_size2_169_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_169_configbus0[179:179] = sram_blwl_bl[179:179] ;
assign mux_1level_tapbuf_size2_169_configbus1[179:179] = sram_blwl_wl[179:179] ;
wire [179:179] mux_1level_tapbuf_size2_169_configbus0_b;
assign mux_1level_tapbuf_size2_169_configbus0_b[179:179] = sram_blwl_blb[179:179] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_169_ (mux_1level_tapbuf_size2_169_inbus, chany_0__1__out_39_ , mux_1level_tapbuf_size2_169_sram_blwl_out[179:179] ,
mux_1level_tapbuf_size2_169_sram_blwl_outb[179:179] );
//----- SRAM bits for MUX[169], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_179_ (mux_1level_tapbuf_size2_169_sram_blwl_out[179:179] ,mux_1level_tapbuf_size2_169_sram_blwl_out[179:179] ,mux_1level_tapbuf_size2_169_sram_blwl_outb[179:179] ,mux_1level_tapbuf_size2_169_configbus0[179:179], mux_1level_tapbuf_size2_169_configbus1[179:179] , mux_1level_tapbuf_size2_169_configbus0_b[179:179] );
wire [0:1] mux_1level_tapbuf_size2_170_inbus;
assign mux_1level_tapbuf_size2_170_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_170_inbus[1] = chanx_1__1__in_57_ ;
wire [180:180] mux_1level_tapbuf_size2_170_configbus0;
wire [180:180] mux_1level_tapbuf_size2_170_configbus1;
wire [180:180] mux_1level_tapbuf_size2_170_sram_blwl_out ;
wire [180:180] mux_1level_tapbuf_size2_170_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_170_configbus0[180:180] = sram_blwl_bl[180:180] ;
assign mux_1level_tapbuf_size2_170_configbus1[180:180] = sram_blwl_wl[180:180] ;
wire [180:180] mux_1level_tapbuf_size2_170_configbus0_b;
assign mux_1level_tapbuf_size2_170_configbus0_b[180:180] = sram_blwl_blb[180:180] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_170_ (mux_1level_tapbuf_size2_170_inbus, chany_0__1__out_41_ , mux_1level_tapbuf_size2_170_sram_blwl_out[180:180] ,
mux_1level_tapbuf_size2_170_sram_blwl_outb[180:180] );
//----- SRAM bits for MUX[170], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_180_ (mux_1level_tapbuf_size2_170_sram_blwl_out[180:180] ,mux_1level_tapbuf_size2_170_sram_blwl_out[180:180] ,mux_1level_tapbuf_size2_170_sram_blwl_outb[180:180] ,mux_1level_tapbuf_size2_170_configbus0[180:180], mux_1level_tapbuf_size2_170_configbus1[180:180] , mux_1level_tapbuf_size2_170_configbus0_b[180:180] );
wire [0:1] mux_1level_tapbuf_size2_171_inbus;
assign mux_1level_tapbuf_size2_171_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_171_inbus[1] = chanx_1__1__in_55_ ;
wire [181:181] mux_1level_tapbuf_size2_171_configbus0;
wire [181:181] mux_1level_tapbuf_size2_171_configbus1;
wire [181:181] mux_1level_tapbuf_size2_171_sram_blwl_out ;
wire [181:181] mux_1level_tapbuf_size2_171_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_171_configbus0[181:181] = sram_blwl_bl[181:181] ;
assign mux_1level_tapbuf_size2_171_configbus1[181:181] = sram_blwl_wl[181:181] ;
wire [181:181] mux_1level_tapbuf_size2_171_configbus0_b;
assign mux_1level_tapbuf_size2_171_configbus0_b[181:181] = sram_blwl_blb[181:181] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_171_ (mux_1level_tapbuf_size2_171_inbus, chany_0__1__out_43_ , mux_1level_tapbuf_size2_171_sram_blwl_out[181:181] ,
mux_1level_tapbuf_size2_171_sram_blwl_outb[181:181] );
//----- SRAM bits for MUX[171], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_181_ (mux_1level_tapbuf_size2_171_sram_blwl_out[181:181] ,mux_1level_tapbuf_size2_171_sram_blwl_out[181:181] ,mux_1level_tapbuf_size2_171_sram_blwl_outb[181:181] ,mux_1level_tapbuf_size2_171_configbus0[181:181], mux_1level_tapbuf_size2_171_configbus1[181:181] , mux_1level_tapbuf_size2_171_configbus0_b[181:181] );
wire [0:1] mux_1level_tapbuf_size2_172_inbus;
assign mux_1level_tapbuf_size2_172_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_172_inbus[1] = chanx_1__1__in_53_ ;
wire [182:182] mux_1level_tapbuf_size2_172_configbus0;
wire [182:182] mux_1level_tapbuf_size2_172_configbus1;
wire [182:182] mux_1level_tapbuf_size2_172_sram_blwl_out ;
wire [182:182] mux_1level_tapbuf_size2_172_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_172_configbus0[182:182] = sram_blwl_bl[182:182] ;
assign mux_1level_tapbuf_size2_172_configbus1[182:182] = sram_blwl_wl[182:182] ;
wire [182:182] mux_1level_tapbuf_size2_172_configbus0_b;
assign mux_1level_tapbuf_size2_172_configbus0_b[182:182] = sram_blwl_blb[182:182] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_172_ (mux_1level_tapbuf_size2_172_inbus, chany_0__1__out_45_ , mux_1level_tapbuf_size2_172_sram_blwl_out[182:182] ,
mux_1level_tapbuf_size2_172_sram_blwl_outb[182:182] );
//----- SRAM bits for MUX[172], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_182_ (mux_1level_tapbuf_size2_172_sram_blwl_out[182:182] ,mux_1level_tapbuf_size2_172_sram_blwl_out[182:182] ,mux_1level_tapbuf_size2_172_sram_blwl_outb[182:182] ,mux_1level_tapbuf_size2_172_configbus0[182:182], mux_1level_tapbuf_size2_172_configbus1[182:182] , mux_1level_tapbuf_size2_172_configbus0_b[182:182] );
wire [0:1] mux_1level_tapbuf_size2_173_inbus;
assign mux_1level_tapbuf_size2_173_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_173_inbus[1] = chanx_1__1__in_51_ ;
wire [183:183] mux_1level_tapbuf_size2_173_configbus0;
wire [183:183] mux_1level_tapbuf_size2_173_configbus1;
wire [183:183] mux_1level_tapbuf_size2_173_sram_blwl_out ;
wire [183:183] mux_1level_tapbuf_size2_173_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_173_configbus0[183:183] = sram_blwl_bl[183:183] ;
assign mux_1level_tapbuf_size2_173_configbus1[183:183] = sram_blwl_wl[183:183] ;
wire [183:183] mux_1level_tapbuf_size2_173_configbus0_b;
assign mux_1level_tapbuf_size2_173_configbus0_b[183:183] = sram_blwl_blb[183:183] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_173_ (mux_1level_tapbuf_size2_173_inbus, chany_0__1__out_47_ , mux_1level_tapbuf_size2_173_sram_blwl_out[183:183] ,
mux_1level_tapbuf_size2_173_sram_blwl_outb[183:183] );
//----- SRAM bits for MUX[173], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_183_ (mux_1level_tapbuf_size2_173_sram_blwl_out[183:183] ,mux_1level_tapbuf_size2_173_sram_blwl_out[183:183] ,mux_1level_tapbuf_size2_173_sram_blwl_outb[183:183] ,mux_1level_tapbuf_size2_173_configbus0[183:183], mux_1level_tapbuf_size2_173_configbus1[183:183] , mux_1level_tapbuf_size2_173_configbus0_b[183:183] );
wire [0:1] mux_1level_tapbuf_size2_174_inbus;
assign mux_1level_tapbuf_size2_174_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_174_inbus[1] = chanx_1__1__in_49_ ;
wire [184:184] mux_1level_tapbuf_size2_174_configbus0;
wire [184:184] mux_1level_tapbuf_size2_174_configbus1;
wire [184:184] mux_1level_tapbuf_size2_174_sram_blwl_out ;
wire [184:184] mux_1level_tapbuf_size2_174_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_174_configbus0[184:184] = sram_blwl_bl[184:184] ;
assign mux_1level_tapbuf_size2_174_configbus1[184:184] = sram_blwl_wl[184:184] ;
wire [184:184] mux_1level_tapbuf_size2_174_configbus0_b;
assign mux_1level_tapbuf_size2_174_configbus0_b[184:184] = sram_blwl_blb[184:184] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_174_ (mux_1level_tapbuf_size2_174_inbus, chany_0__1__out_49_ , mux_1level_tapbuf_size2_174_sram_blwl_out[184:184] ,
mux_1level_tapbuf_size2_174_sram_blwl_outb[184:184] );
//----- SRAM bits for MUX[174], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_184_ (mux_1level_tapbuf_size2_174_sram_blwl_out[184:184] ,mux_1level_tapbuf_size2_174_sram_blwl_out[184:184] ,mux_1level_tapbuf_size2_174_sram_blwl_outb[184:184] ,mux_1level_tapbuf_size2_174_configbus0[184:184], mux_1level_tapbuf_size2_174_configbus1[184:184] , mux_1level_tapbuf_size2_174_configbus0_b[184:184] );
wire [0:1] mux_1level_tapbuf_size2_175_inbus;
assign mux_1level_tapbuf_size2_175_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_175_inbus[1] = chanx_1__1__in_47_ ;
wire [185:185] mux_1level_tapbuf_size2_175_configbus0;
wire [185:185] mux_1level_tapbuf_size2_175_configbus1;
wire [185:185] mux_1level_tapbuf_size2_175_sram_blwl_out ;
wire [185:185] mux_1level_tapbuf_size2_175_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_175_configbus0[185:185] = sram_blwl_bl[185:185] ;
assign mux_1level_tapbuf_size2_175_configbus1[185:185] = sram_blwl_wl[185:185] ;
wire [185:185] mux_1level_tapbuf_size2_175_configbus0_b;
assign mux_1level_tapbuf_size2_175_configbus0_b[185:185] = sram_blwl_blb[185:185] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_175_ (mux_1level_tapbuf_size2_175_inbus, chany_0__1__out_51_ , mux_1level_tapbuf_size2_175_sram_blwl_out[185:185] ,
mux_1level_tapbuf_size2_175_sram_blwl_outb[185:185] );
//----- SRAM bits for MUX[175], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_185_ (mux_1level_tapbuf_size2_175_sram_blwl_out[185:185] ,mux_1level_tapbuf_size2_175_sram_blwl_out[185:185] ,mux_1level_tapbuf_size2_175_sram_blwl_outb[185:185] ,mux_1level_tapbuf_size2_175_configbus0[185:185], mux_1level_tapbuf_size2_175_configbus1[185:185] , mux_1level_tapbuf_size2_175_configbus0_b[185:185] );
wire [0:1] mux_1level_tapbuf_size2_176_inbus;
assign mux_1level_tapbuf_size2_176_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_176_inbus[1] = chanx_1__1__in_45_ ;
wire [186:186] mux_1level_tapbuf_size2_176_configbus0;
wire [186:186] mux_1level_tapbuf_size2_176_configbus1;
wire [186:186] mux_1level_tapbuf_size2_176_sram_blwl_out ;
wire [186:186] mux_1level_tapbuf_size2_176_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_176_configbus0[186:186] = sram_blwl_bl[186:186] ;
assign mux_1level_tapbuf_size2_176_configbus1[186:186] = sram_blwl_wl[186:186] ;
wire [186:186] mux_1level_tapbuf_size2_176_configbus0_b;
assign mux_1level_tapbuf_size2_176_configbus0_b[186:186] = sram_blwl_blb[186:186] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_176_ (mux_1level_tapbuf_size2_176_inbus, chany_0__1__out_53_ , mux_1level_tapbuf_size2_176_sram_blwl_out[186:186] ,
mux_1level_tapbuf_size2_176_sram_blwl_outb[186:186] );
//----- SRAM bits for MUX[176], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_186_ (mux_1level_tapbuf_size2_176_sram_blwl_out[186:186] ,mux_1level_tapbuf_size2_176_sram_blwl_out[186:186] ,mux_1level_tapbuf_size2_176_sram_blwl_outb[186:186] ,mux_1level_tapbuf_size2_176_configbus0[186:186], mux_1level_tapbuf_size2_176_configbus1[186:186] , mux_1level_tapbuf_size2_176_configbus0_b[186:186] );
wire [0:1] mux_1level_tapbuf_size2_177_inbus;
assign mux_1level_tapbuf_size2_177_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_177_inbus[1] = chanx_1__1__in_43_ ;
wire [187:187] mux_1level_tapbuf_size2_177_configbus0;
wire [187:187] mux_1level_tapbuf_size2_177_configbus1;
wire [187:187] mux_1level_tapbuf_size2_177_sram_blwl_out ;
wire [187:187] mux_1level_tapbuf_size2_177_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_177_configbus0[187:187] = sram_blwl_bl[187:187] ;
assign mux_1level_tapbuf_size2_177_configbus1[187:187] = sram_blwl_wl[187:187] ;
wire [187:187] mux_1level_tapbuf_size2_177_configbus0_b;
assign mux_1level_tapbuf_size2_177_configbus0_b[187:187] = sram_blwl_blb[187:187] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_177_ (mux_1level_tapbuf_size2_177_inbus, chany_0__1__out_55_ , mux_1level_tapbuf_size2_177_sram_blwl_out[187:187] ,
mux_1level_tapbuf_size2_177_sram_blwl_outb[187:187] );
//----- SRAM bits for MUX[177], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_187_ (mux_1level_tapbuf_size2_177_sram_blwl_out[187:187] ,mux_1level_tapbuf_size2_177_sram_blwl_out[187:187] ,mux_1level_tapbuf_size2_177_sram_blwl_outb[187:187] ,mux_1level_tapbuf_size2_177_configbus0[187:187], mux_1level_tapbuf_size2_177_configbus1[187:187] , mux_1level_tapbuf_size2_177_configbus0_b[187:187] );
wire [0:1] mux_1level_tapbuf_size2_178_inbus;
assign mux_1level_tapbuf_size2_178_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_178_inbus[1] = chanx_1__1__in_41_ ;
wire [188:188] mux_1level_tapbuf_size2_178_configbus0;
wire [188:188] mux_1level_tapbuf_size2_178_configbus1;
wire [188:188] mux_1level_tapbuf_size2_178_sram_blwl_out ;
wire [188:188] mux_1level_tapbuf_size2_178_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_178_configbus0[188:188] = sram_blwl_bl[188:188] ;
assign mux_1level_tapbuf_size2_178_configbus1[188:188] = sram_blwl_wl[188:188] ;
wire [188:188] mux_1level_tapbuf_size2_178_configbus0_b;
assign mux_1level_tapbuf_size2_178_configbus0_b[188:188] = sram_blwl_blb[188:188] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_178_ (mux_1level_tapbuf_size2_178_inbus, chany_0__1__out_57_ , mux_1level_tapbuf_size2_178_sram_blwl_out[188:188] ,
mux_1level_tapbuf_size2_178_sram_blwl_outb[188:188] );
//----- SRAM bits for MUX[178], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_188_ (mux_1level_tapbuf_size2_178_sram_blwl_out[188:188] ,mux_1level_tapbuf_size2_178_sram_blwl_out[188:188] ,mux_1level_tapbuf_size2_178_sram_blwl_outb[188:188] ,mux_1level_tapbuf_size2_178_configbus0[188:188], mux_1level_tapbuf_size2_178_configbus1[188:188] , mux_1level_tapbuf_size2_178_configbus0_b[188:188] );
wire [0:1] mux_1level_tapbuf_size2_179_inbus;
assign mux_1level_tapbuf_size2_179_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_179_inbus[1] = chanx_1__1__in_39_ ;
wire [189:189] mux_1level_tapbuf_size2_179_configbus0;
wire [189:189] mux_1level_tapbuf_size2_179_configbus1;
wire [189:189] mux_1level_tapbuf_size2_179_sram_blwl_out ;
wire [189:189] mux_1level_tapbuf_size2_179_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_179_configbus0[189:189] = sram_blwl_bl[189:189] ;
assign mux_1level_tapbuf_size2_179_configbus1[189:189] = sram_blwl_wl[189:189] ;
wire [189:189] mux_1level_tapbuf_size2_179_configbus0_b;
assign mux_1level_tapbuf_size2_179_configbus0_b[189:189] = sram_blwl_blb[189:189] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_179_ (mux_1level_tapbuf_size2_179_inbus, chany_0__1__out_59_ , mux_1level_tapbuf_size2_179_sram_blwl_out[189:189] ,
mux_1level_tapbuf_size2_179_sram_blwl_outb[189:189] );
//----- SRAM bits for MUX[179], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_189_ (mux_1level_tapbuf_size2_179_sram_blwl_out[189:189] ,mux_1level_tapbuf_size2_179_sram_blwl_out[189:189] ,mux_1level_tapbuf_size2_179_sram_blwl_outb[189:189] ,mux_1level_tapbuf_size2_179_configbus0[189:189], mux_1level_tapbuf_size2_179_configbus1[189:189] , mux_1level_tapbuf_size2_179_configbus0_b[189:189] );
wire [0:1] mux_1level_tapbuf_size2_180_inbus;
assign mux_1level_tapbuf_size2_180_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_180_inbus[1] = chanx_1__1__in_37_ ;
wire [190:190] mux_1level_tapbuf_size2_180_configbus0;
wire [190:190] mux_1level_tapbuf_size2_180_configbus1;
wire [190:190] mux_1level_tapbuf_size2_180_sram_blwl_out ;
wire [190:190] mux_1level_tapbuf_size2_180_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_180_configbus0[190:190] = sram_blwl_bl[190:190] ;
assign mux_1level_tapbuf_size2_180_configbus1[190:190] = sram_blwl_wl[190:190] ;
wire [190:190] mux_1level_tapbuf_size2_180_configbus0_b;
assign mux_1level_tapbuf_size2_180_configbus0_b[190:190] = sram_blwl_blb[190:190] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_180_ (mux_1level_tapbuf_size2_180_inbus, chany_0__1__out_61_ , mux_1level_tapbuf_size2_180_sram_blwl_out[190:190] ,
mux_1level_tapbuf_size2_180_sram_blwl_outb[190:190] );
//----- SRAM bits for MUX[180], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_190_ (mux_1level_tapbuf_size2_180_sram_blwl_out[190:190] ,mux_1level_tapbuf_size2_180_sram_blwl_out[190:190] ,mux_1level_tapbuf_size2_180_sram_blwl_outb[190:190] ,mux_1level_tapbuf_size2_180_configbus0[190:190], mux_1level_tapbuf_size2_180_configbus1[190:190] , mux_1level_tapbuf_size2_180_configbus0_b[190:190] );
wire [0:1] mux_1level_tapbuf_size2_181_inbus;
assign mux_1level_tapbuf_size2_181_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_181_inbus[1] = chanx_1__1__in_35_ ;
wire [191:191] mux_1level_tapbuf_size2_181_configbus0;
wire [191:191] mux_1level_tapbuf_size2_181_configbus1;
wire [191:191] mux_1level_tapbuf_size2_181_sram_blwl_out ;
wire [191:191] mux_1level_tapbuf_size2_181_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_181_configbus0[191:191] = sram_blwl_bl[191:191] ;
assign mux_1level_tapbuf_size2_181_configbus1[191:191] = sram_blwl_wl[191:191] ;
wire [191:191] mux_1level_tapbuf_size2_181_configbus0_b;
assign mux_1level_tapbuf_size2_181_configbus0_b[191:191] = sram_blwl_blb[191:191] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_181_ (mux_1level_tapbuf_size2_181_inbus, chany_0__1__out_63_ , mux_1level_tapbuf_size2_181_sram_blwl_out[191:191] ,
mux_1level_tapbuf_size2_181_sram_blwl_outb[191:191] );
//----- SRAM bits for MUX[181], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_191_ (mux_1level_tapbuf_size2_181_sram_blwl_out[191:191] ,mux_1level_tapbuf_size2_181_sram_blwl_out[191:191] ,mux_1level_tapbuf_size2_181_sram_blwl_outb[191:191] ,mux_1level_tapbuf_size2_181_configbus0[191:191], mux_1level_tapbuf_size2_181_configbus1[191:191] , mux_1level_tapbuf_size2_181_configbus0_b[191:191] );
wire [0:1] mux_1level_tapbuf_size2_182_inbus;
assign mux_1level_tapbuf_size2_182_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_182_inbus[1] = chanx_1__1__in_33_ ;
wire [192:192] mux_1level_tapbuf_size2_182_configbus0;
wire [192:192] mux_1level_tapbuf_size2_182_configbus1;
wire [192:192] mux_1level_tapbuf_size2_182_sram_blwl_out ;
wire [192:192] mux_1level_tapbuf_size2_182_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_182_configbus0[192:192] = sram_blwl_bl[192:192] ;
assign mux_1level_tapbuf_size2_182_configbus1[192:192] = sram_blwl_wl[192:192] ;
wire [192:192] mux_1level_tapbuf_size2_182_configbus0_b;
assign mux_1level_tapbuf_size2_182_configbus0_b[192:192] = sram_blwl_blb[192:192] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_182_ (mux_1level_tapbuf_size2_182_inbus, chany_0__1__out_65_ , mux_1level_tapbuf_size2_182_sram_blwl_out[192:192] ,
mux_1level_tapbuf_size2_182_sram_blwl_outb[192:192] );
//----- SRAM bits for MUX[182], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_192_ (mux_1level_tapbuf_size2_182_sram_blwl_out[192:192] ,mux_1level_tapbuf_size2_182_sram_blwl_out[192:192] ,mux_1level_tapbuf_size2_182_sram_blwl_outb[192:192] ,mux_1level_tapbuf_size2_182_configbus0[192:192], mux_1level_tapbuf_size2_182_configbus1[192:192] , mux_1level_tapbuf_size2_182_configbus0_b[192:192] );
wire [0:1] mux_1level_tapbuf_size2_183_inbus;
assign mux_1level_tapbuf_size2_183_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_183_inbus[1] = chanx_1__1__in_31_ ;
wire [193:193] mux_1level_tapbuf_size2_183_configbus0;
wire [193:193] mux_1level_tapbuf_size2_183_configbus1;
wire [193:193] mux_1level_tapbuf_size2_183_sram_blwl_out ;
wire [193:193] mux_1level_tapbuf_size2_183_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_183_configbus0[193:193] = sram_blwl_bl[193:193] ;
assign mux_1level_tapbuf_size2_183_configbus1[193:193] = sram_blwl_wl[193:193] ;
wire [193:193] mux_1level_tapbuf_size2_183_configbus0_b;
assign mux_1level_tapbuf_size2_183_configbus0_b[193:193] = sram_blwl_blb[193:193] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_183_ (mux_1level_tapbuf_size2_183_inbus, chany_0__1__out_67_ , mux_1level_tapbuf_size2_183_sram_blwl_out[193:193] ,
mux_1level_tapbuf_size2_183_sram_blwl_outb[193:193] );
//----- SRAM bits for MUX[183], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_193_ (mux_1level_tapbuf_size2_183_sram_blwl_out[193:193] ,mux_1level_tapbuf_size2_183_sram_blwl_out[193:193] ,mux_1level_tapbuf_size2_183_sram_blwl_outb[193:193] ,mux_1level_tapbuf_size2_183_configbus0[193:193], mux_1level_tapbuf_size2_183_configbus1[193:193] , mux_1level_tapbuf_size2_183_configbus0_b[193:193] );
wire [0:1] mux_1level_tapbuf_size2_184_inbus;
assign mux_1level_tapbuf_size2_184_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_184_inbus[1] = chanx_1__1__in_29_ ;
wire [194:194] mux_1level_tapbuf_size2_184_configbus0;
wire [194:194] mux_1level_tapbuf_size2_184_configbus1;
wire [194:194] mux_1level_tapbuf_size2_184_sram_blwl_out ;
wire [194:194] mux_1level_tapbuf_size2_184_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_184_configbus0[194:194] = sram_blwl_bl[194:194] ;
assign mux_1level_tapbuf_size2_184_configbus1[194:194] = sram_blwl_wl[194:194] ;
wire [194:194] mux_1level_tapbuf_size2_184_configbus0_b;
assign mux_1level_tapbuf_size2_184_configbus0_b[194:194] = sram_blwl_blb[194:194] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_184_ (mux_1level_tapbuf_size2_184_inbus, chany_0__1__out_69_ , mux_1level_tapbuf_size2_184_sram_blwl_out[194:194] ,
mux_1level_tapbuf_size2_184_sram_blwl_outb[194:194] );
//----- SRAM bits for MUX[184], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_194_ (mux_1level_tapbuf_size2_184_sram_blwl_out[194:194] ,mux_1level_tapbuf_size2_184_sram_blwl_out[194:194] ,mux_1level_tapbuf_size2_184_sram_blwl_outb[194:194] ,mux_1level_tapbuf_size2_184_configbus0[194:194], mux_1level_tapbuf_size2_184_configbus1[194:194] , mux_1level_tapbuf_size2_184_configbus0_b[194:194] );
wire [0:1] mux_1level_tapbuf_size2_185_inbus;
assign mux_1level_tapbuf_size2_185_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_185_inbus[1] = chanx_1__1__in_27_ ;
wire [195:195] mux_1level_tapbuf_size2_185_configbus0;
wire [195:195] mux_1level_tapbuf_size2_185_configbus1;
wire [195:195] mux_1level_tapbuf_size2_185_sram_blwl_out ;
wire [195:195] mux_1level_tapbuf_size2_185_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_185_configbus0[195:195] = sram_blwl_bl[195:195] ;
assign mux_1level_tapbuf_size2_185_configbus1[195:195] = sram_blwl_wl[195:195] ;
wire [195:195] mux_1level_tapbuf_size2_185_configbus0_b;
assign mux_1level_tapbuf_size2_185_configbus0_b[195:195] = sram_blwl_blb[195:195] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_185_ (mux_1level_tapbuf_size2_185_inbus, chany_0__1__out_71_ , mux_1level_tapbuf_size2_185_sram_blwl_out[195:195] ,
mux_1level_tapbuf_size2_185_sram_blwl_outb[195:195] );
//----- SRAM bits for MUX[185], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_195_ (mux_1level_tapbuf_size2_185_sram_blwl_out[195:195] ,mux_1level_tapbuf_size2_185_sram_blwl_out[195:195] ,mux_1level_tapbuf_size2_185_sram_blwl_outb[195:195] ,mux_1level_tapbuf_size2_185_configbus0[195:195], mux_1level_tapbuf_size2_185_configbus1[195:195] , mux_1level_tapbuf_size2_185_configbus0_b[195:195] );
wire [0:1] mux_1level_tapbuf_size2_186_inbus;
assign mux_1level_tapbuf_size2_186_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_186_inbus[1] = chanx_1__1__in_25_ ;
wire [196:196] mux_1level_tapbuf_size2_186_configbus0;
wire [196:196] mux_1level_tapbuf_size2_186_configbus1;
wire [196:196] mux_1level_tapbuf_size2_186_sram_blwl_out ;
wire [196:196] mux_1level_tapbuf_size2_186_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_186_configbus0[196:196] = sram_blwl_bl[196:196] ;
assign mux_1level_tapbuf_size2_186_configbus1[196:196] = sram_blwl_wl[196:196] ;
wire [196:196] mux_1level_tapbuf_size2_186_configbus0_b;
assign mux_1level_tapbuf_size2_186_configbus0_b[196:196] = sram_blwl_blb[196:196] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_186_ (mux_1level_tapbuf_size2_186_inbus, chany_0__1__out_73_ , mux_1level_tapbuf_size2_186_sram_blwl_out[196:196] ,
mux_1level_tapbuf_size2_186_sram_blwl_outb[196:196] );
//----- SRAM bits for MUX[186], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_196_ (mux_1level_tapbuf_size2_186_sram_blwl_out[196:196] ,mux_1level_tapbuf_size2_186_sram_blwl_out[196:196] ,mux_1level_tapbuf_size2_186_sram_blwl_outb[196:196] ,mux_1level_tapbuf_size2_186_configbus0[196:196], mux_1level_tapbuf_size2_186_configbus1[196:196] , mux_1level_tapbuf_size2_186_configbus0_b[196:196] );
wire [0:1] mux_1level_tapbuf_size2_187_inbus;
assign mux_1level_tapbuf_size2_187_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_187_inbus[1] = chanx_1__1__in_23_ ;
wire [197:197] mux_1level_tapbuf_size2_187_configbus0;
wire [197:197] mux_1level_tapbuf_size2_187_configbus1;
wire [197:197] mux_1level_tapbuf_size2_187_sram_blwl_out ;
wire [197:197] mux_1level_tapbuf_size2_187_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_187_configbus0[197:197] = sram_blwl_bl[197:197] ;
assign mux_1level_tapbuf_size2_187_configbus1[197:197] = sram_blwl_wl[197:197] ;
wire [197:197] mux_1level_tapbuf_size2_187_configbus0_b;
assign mux_1level_tapbuf_size2_187_configbus0_b[197:197] = sram_blwl_blb[197:197] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_187_ (mux_1level_tapbuf_size2_187_inbus, chany_0__1__out_75_ , mux_1level_tapbuf_size2_187_sram_blwl_out[197:197] ,
mux_1level_tapbuf_size2_187_sram_blwl_outb[197:197] );
//----- SRAM bits for MUX[187], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_197_ (mux_1level_tapbuf_size2_187_sram_blwl_out[197:197] ,mux_1level_tapbuf_size2_187_sram_blwl_out[197:197] ,mux_1level_tapbuf_size2_187_sram_blwl_outb[197:197] ,mux_1level_tapbuf_size2_187_configbus0[197:197], mux_1level_tapbuf_size2_187_configbus1[197:197] , mux_1level_tapbuf_size2_187_configbus0_b[197:197] );
wire [0:1] mux_1level_tapbuf_size2_188_inbus;
assign mux_1level_tapbuf_size2_188_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_188_inbus[1] = chanx_1__1__in_21_ ;
wire [198:198] mux_1level_tapbuf_size2_188_configbus0;
wire [198:198] mux_1level_tapbuf_size2_188_configbus1;
wire [198:198] mux_1level_tapbuf_size2_188_sram_blwl_out ;
wire [198:198] mux_1level_tapbuf_size2_188_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_188_configbus0[198:198] = sram_blwl_bl[198:198] ;
assign mux_1level_tapbuf_size2_188_configbus1[198:198] = sram_blwl_wl[198:198] ;
wire [198:198] mux_1level_tapbuf_size2_188_configbus0_b;
assign mux_1level_tapbuf_size2_188_configbus0_b[198:198] = sram_blwl_blb[198:198] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_188_ (mux_1level_tapbuf_size2_188_inbus, chany_0__1__out_77_ , mux_1level_tapbuf_size2_188_sram_blwl_out[198:198] ,
mux_1level_tapbuf_size2_188_sram_blwl_outb[198:198] );
//----- SRAM bits for MUX[188], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_198_ (mux_1level_tapbuf_size2_188_sram_blwl_out[198:198] ,mux_1level_tapbuf_size2_188_sram_blwl_out[198:198] ,mux_1level_tapbuf_size2_188_sram_blwl_outb[198:198] ,mux_1level_tapbuf_size2_188_configbus0[198:198], mux_1level_tapbuf_size2_188_configbus1[198:198] , mux_1level_tapbuf_size2_188_configbus0_b[198:198] );
wire [0:1] mux_1level_tapbuf_size2_189_inbus;
assign mux_1level_tapbuf_size2_189_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_189_inbus[1] = chanx_1__1__in_19_ ;
wire [199:199] mux_1level_tapbuf_size2_189_configbus0;
wire [199:199] mux_1level_tapbuf_size2_189_configbus1;
wire [199:199] mux_1level_tapbuf_size2_189_sram_blwl_out ;
wire [199:199] mux_1level_tapbuf_size2_189_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_189_configbus0[199:199] = sram_blwl_bl[199:199] ;
assign mux_1level_tapbuf_size2_189_configbus1[199:199] = sram_blwl_wl[199:199] ;
wire [199:199] mux_1level_tapbuf_size2_189_configbus0_b;
assign mux_1level_tapbuf_size2_189_configbus0_b[199:199] = sram_blwl_blb[199:199] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_189_ (mux_1level_tapbuf_size2_189_inbus, chany_0__1__out_79_ , mux_1level_tapbuf_size2_189_sram_blwl_out[199:199] ,
mux_1level_tapbuf_size2_189_sram_blwl_outb[199:199] );
//----- SRAM bits for MUX[189], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_199_ (mux_1level_tapbuf_size2_189_sram_blwl_out[199:199] ,mux_1level_tapbuf_size2_189_sram_blwl_out[199:199] ,mux_1level_tapbuf_size2_189_sram_blwl_outb[199:199] ,mux_1level_tapbuf_size2_189_configbus0[199:199], mux_1level_tapbuf_size2_189_configbus1[199:199] , mux_1level_tapbuf_size2_189_configbus0_b[199:199] );
wire [0:1] mux_1level_tapbuf_size2_190_inbus;
assign mux_1level_tapbuf_size2_190_inbus[0] =  grid_1__1__pin_0__3__43_;
assign mux_1level_tapbuf_size2_190_inbus[1] = chanx_1__1__in_17_ ;
wire [200:200] mux_1level_tapbuf_size2_190_configbus0;
wire [200:200] mux_1level_tapbuf_size2_190_configbus1;
wire [200:200] mux_1level_tapbuf_size2_190_sram_blwl_out ;
wire [200:200] mux_1level_tapbuf_size2_190_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_190_configbus0[200:200] = sram_blwl_bl[200:200] ;
assign mux_1level_tapbuf_size2_190_configbus1[200:200] = sram_blwl_wl[200:200] ;
wire [200:200] mux_1level_tapbuf_size2_190_configbus0_b;
assign mux_1level_tapbuf_size2_190_configbus0_b[200:200] = sram_blwl_blb[200:200] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_190_ (mux_1level_tapbuf_size2_190_inbus, chany_0__1__out_81_ , mux_1level_tapbuf_size2_190_sram_blwl_out[200:200] ,
mux_1level_tapbuf_size2_190_sram_blwl_outb[200:200] );
//----- SRAM bits for MUX[190], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_200_ (mux_1level_tapbuf_size2_190_sram_blwl_out[200:200] ,mux_1level_tapbuf_size2_190_sram_blwl_out[200:200] ,mux_1level_tapbuf_size2_190_sram_blwl_outb[200:200] ,mux_1level_tapbuf_size2_190_configbus0[200:200], mux_1level_tapbuf_size2_190_configbus1[200:200] , mux_1level_tapbuf_size2_190_configbus0_b[200:200] );
wire [0:1] mux_1level_tapbuf_size2_191_inbus;
assign mux_1level_tapbuf_size2_191_inbus[0] =  grid_1__1__pin_0__3__43_;
assign mux_1level_tapbuf_size2_191_inbus[1] = chanx_1__1__in_15_ ;
wire [201:201] mux_1level_tapbuf_size2_191_configbus0;
wire [201:201] mux_1level_tapbuf_size2_191_configbus1;
wire [201:201] mux_1level_tapbuf_size2_191_sram_blwl_out ;
wire [201:201] mux_1level_tapbuf_size2_191_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_191_configbus0[201:201] = sram_blwl_bl[201:201] ;
assign mux_1level_tapbuf_size2_191_configbus1[201:201] = sram_blwl_wl[201:201] ;
wire [201:201] mux_1level_tapbuf_size2_191_configbus0_b;
assign mux_1level_tapbuf_size2_191_configbus0_b[201:201] = sram_blwl_blb[201:201] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_191_ (mux_1level_tapbuf_size2_191_inbus, chany_0__1__out_83_ , mux_1level_tapbuf_size2_191_sram_blwl_out[201:201] ,
mux_1level_tapbuf_size2_191_sram_blwl_outb[201:201] );
//----- SRAM bits for MUX[191], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_201_ (mux_1level_tapbuf_size2_191_sram_blwl_out[201:201] ,mux_1level_tapbuf_size2_191_sram_blwl_out[201:201] ,mux_1level_tapbuf_size2_191_sram_blwl_outb[201:201] ,mux_1level_tapbuf_size2_191_configbus0[201:201], mux_1level_tapbuf_size2_191_configbus1[201:201] , mux_1level_tapbuf_size2_191_configbus0_b[201:201] );
wire [0:1] mux_1level_tapbuf_size2_192_inbus;
assign mux_1level_tapbuf_size2_192_inbus[0] =  grid_1__1__pin_0__3__43_;
assign mux_1level_tapbuf_size2_192_inbus[1] = chanx_1__1__in_13_ ;
wire [202:202] mux_1level_tapbuf_size2_192_configbus0;
wire [202:202] mux_1level_tapbuf_size2_192_configbus1;
wire [202:202] mux_1level_tapbuf_size2_192_sram_blwl_out ;
wire [202:202] mux_1level_tapbuf_size2_192_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_192_configbus0[202:202] = sram_blwl_bl[202:202] ;
assign mux_1level_tapbuf_size2_192_configbus1[202:202] = sram_blwl_wl[202:202] ;
wire [202:202] mux_1level_tapbuf_size2_192_configbus0_b;
assign mux_1level_tapbuf_size2_192_configbus0_b[202:202] = sram_blwl_blb[202:202] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_192_ (mux_1level_tapbuf_size2_192_inbus, chany_0__1__out_85_ , mux_1level_tapbuf_size2_192_sram_blwl_out[202:202] ,
mux_1level_tapbuf_size2_192_sram_blwl_outb[202:202] );
//----- SRAM bits for MUX[192], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_202_ (mux_1level_tapbuf_size2_192_sram_blwl_out[202:202] ,mux_1level_tapbuf_size2_192_sram_blwl_out[202:202] ,mux_1level_tapbuf_size2_192_sram_blwl_outb[202:202] ,mux_1level_tapbuf_size2_192_configbus0[202:202], mux_1level_tapbuf_size2_192_configbus1[202:202] , mux_1level_tapbuf_size2_192_configbus0_b[202:202] );
wire [0:1] mux_1level_tapbuf_size2_193_inbus;
assign mux_1level_tapbuf_size2_193_inbus[0] =  grid_1__1__pin_0__3__43_;
assign mux_1level_tapbuf_size2_193_inbus[1] = chanx_1__1__in_11_ ;
wire [203:203] mux_1level_tapbuf_size2_193_configbus0;
wire [203:203] mux_1level_tapbuf_size2_193_configbus1;
wire [203:203] mux_1level_tapbuf_size2_193_sram_blwl_out ;
wire [203:203] mux_1level_tapbuf_size2_193_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_193_configbus0[203:203] = sram_blwl_bl[203:203] ;
assign mux_1level_tapbuf_size2_193_configbus1[203:203] = sram_blwl_wl[203:203] ;
wire [203:203] mux_1level_tapbuf_size2_193_configbus0_b;
assign mux_1level_tapbuf_size2_193_configbus0_b[203:203] = sram_blwl_blb[203:203] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_193_ (mux_1level_tapbuf_size2_193_inbus, chany_0__1__out_87_ , mux_1level_tapbuf_size2_193_sram_blwl_out[203:203] ,
mux_1level_tapbuf_size2_193_sram_blwl_outb[203:203] );
//----- SRAM bits for MUX[193], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_203_ (mux_1level_tapbuf_size2_193_sram_blwl_out[203:203] ,mux_1level_tapbuf_size2_193_sram_blwl_out[203:203] ,mux_1level_tapbuf_size2_193_sram_blwl_outb[203:203] ,mux_1level_tapbuf_size2_193_configbus0[203:203], mux_1level_tapbuf_size2_193_configbus1[203:203] , mux_1level_tapbuf_size2_193_configbus0_b[203:203] );
wire [0:1] mux_1level_tapbuf_size2_194_inbus;
assign mux_1level_tapbuf_size2_194_inbus[0] =  grid_1__1__pin_0__3__43_;
assign mux_1level_tapbuf_size2_194_inbus[1] = chanx_1__1__in_9_ ;
wire [204:204] mux_1level_tapbuf_size2_194_configbus0;
wire [204:204] mux_1level_tapbuf_size2_194_configbus1;
wire [204:204] mux_1level_tapbuf_size2_194_sram_blwl_out ;
wire [204:204] mux_1level_tapbuf_size2_194_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_194_configbus0[204:204] = sram_blwl_bl[204:204] ;
assign mux_1level_tapbuf_size2_194_configbus1[204:204] = sram_blwl_wl[204:204] ;
wire [204:204] mux_1level_tapbuf_size2_194_configbus0_b;
assign mux_1level_tapbuf_size2_194_configbus0_b[204:204] = sram_blwl_blb[204:204] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_194_ (mux_1level_tapbuf_size2_194_inbus, chany_0__1__out_89_ , mux_1level_tapbuf_size2_194_sram_blwl_out[204:204] ,
mux_1level_tapbuf_size2_194_sram_blwl_outb[204:204] );
//----- SRAM bits for MUX[194], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_204_ (mux_1level_tapbuf_size2_194_sram_blwl_out[204:204] ,mux_1level_tapbuf_size2_194_sram_blwl_out[204:204] ,mux_1level_tapbuf_size2_194_sram_blwl_outb[204:204] ,mux_1level_tapbuf_size2_194_configbus0[204:204], mux_1level_tapbuf_size2_194_configbus1[204:204] , mux_1level_tapbuf_size2_194_configbus0_b[204:204] );
wire [0:1] mux_1level_tapbuf_size2_195_inbus;
assign mux_1level_tapbuf_size2_195_inbus[0] =  grid_1__1__pin_0__3__47_;
assign mux_1level_tapbuf_size2_195_inbus[1] = chanx_1__1__in_7_ ;
wire [205:205] mux_1level_tapbuf_size2_195_configbus0;
wire [205:205] mux_1level_tapbuf_size2_195_configbus1;
wire [205:205] mux_1level_tapbuf_size2_195_sram_blwl_out ;
wire [205:205] mux_1level_tapbuf_size2_195_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_195_configbus0[205:205] = sram_blwl_bl[205:205] ;
assign mux_1level_tapbuf_size2_195_configbus1[205:205] = sram_blwl_wl[205:205] ;
wire [205:205] mux_1level_tapbuf_size2_195_configbus0_b;
assign mux_1level_tapbuf_size2_195_configbus0_b[205:205] = sram_blwl_blb[205:205] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_195_ (mux_1level_tapbuf_size2_195_inbus, chany_0__1__out_91_ , mux_1level_tapbuf_size2_195_sram_blwl_out[205:205] ,
mux_1level_tapbuf_size2_195_sram_blwl_outb[205:205] );
//----- SRAM bits for MUX[195], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_205_ (mux_1level_tapbuf_size2_195_sram_blwl_out[205:205] ,mux_1level_tapbuf_size2_195_sram_blwl_out[205:205] ,mux_1level_tapbuf_size2_195_sram_blwl_outb[205:205] ,mux_1level_tapbuf_size2_195_configbus0[205:205], mux_1level_tapbuf_size2_195_configbus1[205:205] , mux_1level_tapbuf_size2_195_configbus0_b[205:205] );
wire [0:1] mux_1level_tapbuf_size2_196_inbus;
assign mux_1level_tapbuf_size2_196_inbus[0] =  grid_1__1__pin_0__3__47_;
assign mux_1level_tapbuf_size2_196_inbus[1] = chanx_1__1__in_5_ ;
wire [206:206] mux_1level_tapbuf_size2_196_configbus0;
wire [206:206] mux_1level_tapbuf_size2_196_configbus1;
wire [206:206] mux_1level_tapbuf_size2_196_sram_blwl_out ;
wire [206:206] mux_1level_tapbuf_size2_196_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_196_configbus0[206:206] = sram_blwl_bl[206:206] ;
assign mux_1level_tapbuf_size2_196_configbus1[206:206] = sram_blwl_wl[206:206] ;
wire [206:206] mux_1level_tapbuf_size2_196_configbus0_b;
assign mux_1level_tapbuf_size2_196_configbus0_b[206:206] = sram_blwl_blb[206:206] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_196_ (mux_1level_tapbuf_size2_196_inbus, chany_0__1__out_93_ , mux_1level_tapbuf_size2_196_sram_blwl_out[206:206] ,
mux_1level_tapbuf_size2_196_sram_blwl_outb[206:206] );
//----- SRAM bits for MUX[196], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_206_ (mux_1level_tapbuf_size2_196_sram_blwl_out[206:206] ,mux_1level_tapbuf_size2_196_sram_blwl_out[206:206] ,mux_1level_tapbuf_size2_196_sram_blwl_outb[206:206] ,mux_1level_tapbuf_size2_196_configbus0[206:206], mux_1level_tapbuf_size2_196_configbus1[206:206] , mux_1level_tapbuf_size2_196_configbus0_b[206:206] );
wire [0:1] mux_1level_tapbuf_size2_197_inbus;
assign mux_1level_tapbuf_size2_197_inbus[0] =  grid_1__1__pin_0__3__47_;
assign mux_1level_tapbuf_size2_197_inbus[1] = chanx_1__1__in_3_ ;
wire [207:207] mux_1level_tapbuf_size2_197_configbus0;
wire [207:207] mux_1level_tapbuf_size2_197_configbus1;
wire [207:207] mux_1level_tapbuf_size2_197_sram_blwl_out ;
wire [207:207] mux_1level_tapbuf_size2_197_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_197_configbus0[207:207] = sram_blwl_bl[207:207] ;
assign mux_1level_tapbuf_size2_197_configbus1[207:207] = sram_blwl_wl[207:207] ;
wire [207:207] mux_1level_tapbuf_size2_197_configbus0_b;
assign mux_1level_tapbuf_size2_197_configbus0_b[207:207] = sram_blwl_blb[207:207] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_197_ (mux_1level_tapbuf_size2_197_inbus, chany_0__1__out_95_ , mux_1level_tapbuf_size2_197_sram_blwl_out[207:207] ,
mux_1level_tapbuf_size2_197_sram_blwl_outb[207:207] );
//----- SRAM bits for MUX[197], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_207_ (mux_1level_tapbuf_size2_197_sram_blwl_out[207:207] ,mux_1level_tapbuf_size2_197_sram_blwl_out[207:207] ,mux_1level_tapbuf_size2_197_sram_blwl_outb[207:207] ,mux_1level_tapbuf_size2_197_configbus0[207:207], mux_1level_tapbuf_size2_197_configbus1[207:207] , mux_1level_tapbuf_size2_197_configbus0_b[207:207] );
wire [0:1] mux_1level_tapbuf_size2_198_inbus;
assign mux_1level_tapbuf_size2_198_inbus[0] =  grid_1__1__pin_0__3__47_;
assign mux_1level_tapbuf_size2_198_inbus[1] = chanx_1__1__in_1_ ;
wire [208:208] mux_1level_tapbuf_size2_198_configbus0;
wire [208:208] mux_1level_tapbuf_size2_198_configbus1;
wire [208:208] mux_1level_tapbuf_size2_198_sram_blwl_out ;
wire [208:208] mux_1level_tapbuf_size2_198_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_198_configbus0[208:208] = sram_blwl_bl[208:208] ;
assign mux_1level_tapbuf_size2_198_configbus1[208:208] = sram_blwl_wl[208:208] ;
wire [208:208] mux_1level_tapbuf_size2_198_configbus0_b;
assign mux_1level_tapbuf_size2_198_configbus0_b[208:208] = sram_blwl_blb[208:208] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_198_ (mux_1level_tapbuf_size2_198_inbus, chany_0__1__out_97_ , mux_1level_tapbuf_size2_198_sram_blwl_out[208:208] ,
mux_1level_tapbuf_size2_198_sram_blwl_outb[208:208] );
//----- SRAM bits for MUX[198], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_208_ (mux_1level_tapbuf_size2_198_sram_blwl_out[208:208] ,mux_1level_tapbuf_size2_198_sram_blwl_out[208:208] ,mux_1level_tapbuf_size2_198_sram_blwl_outb[208:208] ,mux_1level_tapbuf_size2_198_configbus0[208:208], mux_1level_tapbuf_size2_198_configbus1[208:208] , mux_1level_tapbuf_size2_198_configbus0_b[208:208] );
wire [0:1] mux_1level_tapbuf_size2_199_inbus;
assign mux_1level_tapbuf_size2_199_inbus[0] =  grid_1__1__pin_0__3__47_;
assign mux_1level_tapbuf_size2_199_inbus[1] = chanx_1__1__in_99_ ;
wire [209:209] mux_1level_tapbuf_size2_199_configbus0;
wire [209:209] mux_1level_tapbuf_size2_199_configbus1;
wire [209:209] mux_1level_tapbuf_size2_199_sram_blwl_out ;
wire [209:209] mux_1level_tapbuf_size2_199_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_199_configbus0[209:209] = sram_blwl_bl[209:209] ;
assign mux_1level_tapbuf_size2_199_configbus1[209:209] = sram_blwl_wl[209:209] ;
wire [209:209] mux_1level_tapbuf_size2_199_configbus0_b;
assign mux_1level_tapbuf_size2_199_configbus0_b[209:209] = sram_blwl_blb[209:209] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_199_ (mux_1level_tapbuf_size2_199_inbus, chany_0__1__out_99_ , mux_1level_tapbuf_size2_199_sram_blwl_out[209:209] ,
mux_1level_tapbuf_size2_199_sram_blwl_outb[209:209] );
//----- SRAM bits for MUX[199], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_209_ (mux_1level_tapbuf_size2_199_sram_blwl_out[209:209] ,mux_1level_tapbuf_size2_199_sram_blwl_out[209:209] ,mux_1level_tapbuf_size2_199_sram_blwl_outb[209:209] ,mux_1level_tapbuf_size2_199_configbus0[209:209], mux_1level_tapbuf_size2_199_configbus1[209:209] , mux_1level_tapbuf_size2_199_configbus0_b[209:209] );
//----- left side Multiplexers -----
endmodule
//----- END Verilog Module of Switch Box[0][1] -----

