//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Switch Block  [0][0] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
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
  output chany_0__1__out_30_,
  input chany_0__1__in_31_,
  output chany_0__1__out_32_,
  input chany_0__1__in_33_,
  output chany_0__1__out_34_,
  input chany_0__1__in_35_,
  output chany_0__1__out_36_,
  input chany_0__1__in_37_,
  output chany_0__1__out_38_,
  input chany_0__1__in_39_,
  output chany_0__1__out_40_,
  input chany_0__1__in_41_,
  output chany_0__1__out_42_,
  input chany_0__1__in_43_,
  output chany_0__1__out_44_,
  input chany_0__1__in_45_,
  output chany_0__1__out_46_,
  input chany_0__1__in_47_,
  output chany_0__1__out_48_,
  input chany_0__1__in_49_,
  output chany_0__1__out_50_,
  input chany_0__1__in_51_,
  output chany_0__1__out_52_,
  input chany_0__1__in_53_,
  output chany_0__1__out_54_,
  input chany_0__1__in_55_,
  output chany_0__1__out_56_,
  input chany_0__1__in_57_,
  output chany_0__1__out_58_,
  input chany_0__1__in_59_,
  output chany_0__1__out_60_,
  input chany_0__1__in_61_,
  output chany_0__1__out_62_,
  input chany_0__1__in_63_,
  output chany_0__1__out_64_,
  input chany_0__1__in_65_,
  output chany_0__1__out_66_,
  input chany_0__1__in_67_,
  output chany_0__1__out_68_,
  input chany_0__1__in_69_,
  output chany_0__1__out_70_,
  input chany_0__1__in_71_,
  output chany_0__1__out_72_,
  input chany_0__1__in_73_,
  output chany_0__1__out_74_,
  input chany_0__1__in_75_,
  output chany_0__1__out_76_,
  input chany_0__1__in_77_,
  output chany_0__1__out_78_,
  input chany_0__1__in_79_,
  output chany_0__1__out_80_,
  input chany_0__1__in_81_,
  output chany_0__1__out_82_,
  input chany_0__1__in_83_,
  output chany_0__1__out_84_,
  input chany_0__1__in_85_,
  output chany_0__1__out_86_,
  input chany_0__1__in_87_,
  output chany_0__1__out_88_,
  input chany_0__1__in_89_,
  output chany_0__1__out_90_,
  input chany_0__1__in_91_,
  output chany_0__1__out_92_,
  input chany_0__1__in_93_,
  output chany_0__1__out_94_,
  input chany_0__1__in_95_,
  output chany_0__1__out_96_,
  input chany_0__1__in_97_,
  output chany_0__1__out_98_,
  input chany_0__1__in_99_,
input  grid_0__1__pin_0__1__1_,
input  grid_0__1__pin_0__1__3_,
input  grid_0__1__pin_0__1__5_,
input  grid_0__1__pin_0__1__7_,
input  grid_0__1__pin_0__1__9_,
input  grid_0__1__pin_0__1__11_,
input  grid_0__1__pin_0__1__13_,
input  grid_0__1__pin_0__1__15_,
input  grid_1__1__pin_0__3__43_,
input  grid_1__1__pin_0__3__47_,
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
  output chanx_1__0__out_30_,
  input chanx_1__0__in_31_,
  output chanx_1__0__out_32_,
  input chanx_1__0__in_33_,
  output chanx_1__0__out_34_,
  input chanx_1__0__in_35_,
  output chanx_1__0__out_36_,
  input chanx_1__0__in_37_,
  output chanx_1__0__out_38_,
  input chanx_1__0__in_39_,
  output chanx_1__0__out_40_,
  input chanx_1__0__in_41_,
  output chanx_1__0__out_42_,
  input chanx_1__0__in_43_,
  output chanx_1__0__out_44_,
  input chanx_1__0__in_45_,
  output chanx_1__0__out_46_,
  input chanx_1__0__in_47_,
  output chanx_1__0__out_48_,
  input chanx_1__0__in_49_,
  output chanx_1__0__out_50_,
  input chanx_1__0__in_51_,
  output chanx_1__0__out_52_,
  input chanx_1__0__in_53_,
  output chanx_1__0__out_54_,
  input chanx_1__0__in_55_,
  output chanx_1__0__out_56_,
  input chanx_1__0__in_57_,
  output chanx_1__0__out_58_,
  input chanx_1__0__in_59_,
  output chanx_1__0__out_60_,
  input chanx_1__0__in_61_,
  output chanx_1__0__out_62_,
  input chanx_1__0__in_63_,
  output chanx_1__0__out_64_,
  input chanx_1__0__in_65_,
  output chanx_1__0__out_66_,
  input chanx_1__0__in_67_,
  output chanx_1__0__out_68_,
  input chanx_1__0__in_69_,
  output chanx_1__0__out_70_,
  input chanx_1__0__in_71_,
  output chanx_1__0__out_72_,
  input chanx_1__0__in_73_,
  output chanx_1__0__out_74_,
  input chanx_1__0__in_75_,
  output chanx_1__0__out_76_,
  input chanx_1__0__in_77_,
  output chanx_1__0__out_78_,
  input chanx_1__0__in_79_,
  output chanx_1__0__out_80_,
  input chanx_1__0__in_81_,
  output chanx_1__0__out_82_,
  input chanx_1__0__in_83_,
  output chanx_1__0__out_84_,
  input chanx_1__0__in_85_,
  output chanx_1__0__out_86_,
  input chanx_1__0__in_87_,
  output chanx_1__0__out_88_,
  input chanx_1__0__in_89_,
  output chanx_1__0__out_90_,
  input chanx_1__0__in_91_,
  output chanx_1__0__out_92_,
  input chanx_1__0__in_93_,
  output chanx_1__0__out_94_,
  input chanx_1__0__in_95_,
  output chanx_1__0__out_96_,
  input chanx_1__0__in_97_,
  output chanx_1__0__out_98_,
  input chanx_1__0__in_99_,
input  grid_1__1__pin_0__2__42_,
input  grid_1__1__pin_0__2__46_,
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
input [0:99] sram_blwl_bl ,
input [0:99] sram_blwl_wl ,
input [0:99] sram_blwl_blb ); 
//----- top side Multiplexers -----
wire [0:1] mux_1level_tapbuf_size2_0_inbus;
assign mux_1level_tapbuf_size2_0_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_0_inbus[1] = chanx_1__0__in_3_ ;
wire [0:0] mux_1level_tapbuf_size2_0_configbus0;
wire [0:0] mux_1level_tapbuf_size2_0_configbus1;
wire [0:0] mux_1level_tapbuf_size2_0_sram_blwl_out ;
wire [0:0] mux_1level_tapbuf_size2_0_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_0_configbus0[0:0] = sram_blwl_bl[0:0] ;
assign mux_1level_tapbuf_size2_0_configbus1[0:0] = sram_blwl_wl[0:0] ;
wire [0:0] mux_1level_tapbuf_size2_0_configbus0_b;
assign mux_1level_tapbuf_size2_0_configbus0_b[0:0] = sram_blwl_blb[0:0] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_0_ (mux_1level_tapbuf_size2_0_inbus, chany_0__1__out_0_ , mux_1level_tapbuf_size2_0_sram_blwl_out[0:0] ,
mux_1level_tapbuf_size2_0_sram_blwl_outb[0:0] );
//----- SRAM bits for MUX[0], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_0_ (mux_1level_tapbuf_size2_0_sram_blwl_out[0:0] ,mux_1level_tapbuf_size2_0_sram_blwl_out[0:0] ,mux_1level_tapbuf_size2_0_sram_blwl_outb[0:0] ,mux_1level_tapbuf_size2_0_configbus0[0:0], mux_1level_tapbuf_size2_0_configbus1[0:0] , mux_1level_tapbuf_size2_0_configbus0_b[0:0] );
wire [0:1] mux_1level_tapbuf_size2_1_inbus;
assign mux_1level_tapbuf_size2_1_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_1_inbus[1] = chanx_1__0__in_5_ ;
wire [1:1] mux_1level_tapbuf_size2_1_configbus0;
wire [1:1] mux_1level_tapbuf_size2_1_configbus1;
wire [1:1] mux_1level_tapbuf_size2_1_sram_blwl_out ;
wire [1:1] mux_1level_tapbuf_size2_1_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_1_configbus0[1:1] = sram_blwl_bl[1:1] ;
assign mux_1level_tapbuf_size2_1_configbus1[1:1] = sram_blwl_wl[1:1] ;
wire [1:1] mux_1level_tapbuf_size2_1_configbus0_b;
assign mux_1level_tapbuf_size2_1_configbus0_b[1:1] = sram_blwl_blb[1:1] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_1_ (mux_1level_tapbuf_size2_1_inbus, chany_0__1__out_2_ , mux_1level_tapbuf_size2_1_sram_blwl_out[1:1] ,
mux_1level_tapbuf_size2_1_sram_blwl_outb[1:1] );
//----- SRAM bits for MUX[1], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_1_ (mux_1level_tapbuf_size2_1_sram_blwl_out[1:1] ,mux_1level_tapbuf_size2_1_sram_blwl_out[1:1] ,mux_1level_tapbuf_size2_1_sram_blwl_outb[1:1] ,mux_1level_tapbuf_size2_1_configbus0[1:1], mux_1level_tapbuf_size2_1_configbus1[1:1] , mux_1level_tapbuf_size2_1_configbus0_b[1:1] );
wire [0:1] mux_1level_tapbuf_size2_2_inbus;
assign mux_1level_tapbuf_size2_2_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_2_inbus[1] = chanx_1__0__in_7_ ;
wire [2:2] mux_1level_tapbuf_size2_2_configbus0;
wire [2:2] mux_1level_tapbuf_size2_2_configbus1;
wire [2:2] mux_1level_tapbuf_size2_2_sram_blwl_out ;
wire [2:2] mux_1level_tapbuf_size2_2_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_2_configbus0[2:2] = sram_blwl_bl[2:2] ;
assign mux_1level_tapbuf_size2_2_configbus1[2:2] = sram_blwl_wl[2:2] ;
wire [2:2] mux_1level_tapbuf_size2_2_configbus0_b;
assign mux_1level_tapbuf_size2_2_configbus0_b[2:2] = sram_blwl_blb[2:2] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_2_ (mux_1level_tapbuf_size2_2_inbus, chany_0__1__out_4_ , mux_1level_tapbuf_size2_2_sram_blwl_out[2:2] ,
mux_1level_tapbuf_size2_2_sram_blwl_outb[2:2] );
//----- SRAM bits for MUX[2], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_2_ (mux_1level_tapbuf_size2_2_sram_blwl_out[2:2] ,mux_1level_tapbuf_size2_2_sram_blwl_out[2:2] ,mux_1level_tapbuf_size2_2_sram_blwl_outb[2:2] ,mux_1level_tapbuf_size2_2_configbus0[2:2], mux_1level_tapbuf_size2_2_configbus1[2:2] , mux_1level_tapbuf_size2_2_configbus0_b[2:2] );
wire [0:1] mux_1level_tapbuf_size2_3_inbus;
assign mux_1level_tapbuf_size2_3_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_3_inbus[1] = chanx_1__0__in_9_ ;
wire [3:3] mux_1level_tapbuf_size2_3_configbus0;
wire [3:3] mux_1level_tapbuf_size2_3_configbus1;
wire [3:3] mux_1level_tapbuf_size2_3_sram_blwl_out ;
wire [3:3] mux_1level_tapbuf_size2_3_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_3_configbus0[3:3] = sram_blwl_bl[3:3] ;
assign mux_1level_tapbuf_size2_3_configbus1[3:3] = sram_blwl_wl[3:3] ;
wire [3:3] mux_1level_tapbuf_size2_3_configbus0_b;
assign mux_1level_tapbuf_size2_3_configbus0_b[3:3] = sram_blwl_blb[3:3] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_3_ (mux_1level_tapbuf_size2_3_inbus, chany_0__1__out_6_ , mux_1level_tapbuf_size2_3_sram_blwl_out[3:3] ,
mux_1level_tapbuf_size2_3_sram_blwl_outb[3:3] );
//----- SRAM bits for MUX[3], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_3_ (mux_1level_tapbuf_size2_3_sram_blwl_out[3:3] ,mux_1level_tapbuf_size2_3_sram_blwl_out[3:3] ,mux_1level_tapbuf_size2_3_sram_blwl_outb[3:3] ,mux_1level_tapbuf_size2_3_configbus0[3:3], mux_1level_tapbuf_size2_3_configbus1[3:3] , mux_1level_tapbuf_size2_3_configbus0_b[3:3] );
wire [0:1] mux_1level_tapbuf_size2_4_inbus;
assign mux_1level_tapbuf_size2_4_inbus[0] =  grid_0__1__pin_0__1__1_;
assign mux_1level_tapbuf_size2_4_inbus[1] = chanx_1__0__in_11_ ;
wire [4:4] mux_1level_tapbuf_size2_4_configbus0;
wire [4:4] mux_1level_tapbuf_size2_4_configbus1;
wire [4:4] mux_1level_tapbuf_size2_4_sram_blwl_out ;
wire [4:4] mux_1level_tapbuf_size2_4_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_4_configbus0[4:4] = sram_blwl_bl[4:4] ;
assign mux_1level_tapbuf_size2_4_configbus1[4:4] = sram_blwl_wl[4:4] ;
wire [4:4] mux_1level_tapbuf_size2_4_configbus0_b;
assign mux_1level_tapbuf_size2_4_configbus0_b[4:4] = sram_blwl_blb[4:4] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_4_ (mux_1level_tapbuf_size2_4_inbus, chany_0__1__out_8_ , mux_1level_tapbuf_size2_4_sram_blwl_out[4:4] ,
mux_1level_tapbuf_size2_4_sram_blwl_outb[4:4] );
//----- SRAM bits for MUX[4], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_4_ (mux_1level_tapbuf_size2_4_sram_blwl_out[4:4] ,mux_1level_tapbuf_size2_4_sram_blwl_out[4:4] ,mux_1level_tapbuf_size2_4_sram_blwl_outb[4:4] ,mux_1level_tapbuf_size2_4_configbus0[4:4], mux_1level_tapbuf_size2_4_configbus1[4:4] , mux_1level_tapbuf_size2_4_configbus0_b[4:4] );
wire [0:1] mux_1level_tapbuf_size2_5_inbus;
assign mux_1level_tapbuf_size2_5_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_5_inbus[1] = chanx_1__0__in_13_ ;
wire [5:5] mux_1level_tapbuf_size2_5_configbus0;
wire [5:5] mux_1level_tapbuf_size2_5_configbus1;
wire [5:5] mux_1level_tapbuf_size2_5_sram_blwl_out ;
wire [5:5] mux_1level_tapbuf_size2_5_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_5_configbus0[5:5] = sram_blwl_bl[5:5] ;
assign mux_1level_tapbuf_size2_5_configbus1[5:5] = sram_blwl_wl[5:5] ;
wire [5:5] mux_1level_tapbuf_size2_5_configbus0_b;
assign mux_1level_tapbuf_size2_5_configbus0_b[5:5] = sram_blwl_blb[5:5] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_5_ (mux_1level_tapbuf_size2_5_inbus, chany_0__1__out_10_ , mux_1level_tapbuf_size2_5_sram_blwl_out[5:5] ,
mux_1level_tapbuf_size2_5_sram_blwl_outb[5:5] );
//----- SRAM bits for MUX[5], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_5_ (mux_1level_tapbuf_size2_5_sram_blwl_out[5:5] ,mux_1level_tapbuf_size2_5_sram_blwl_out[5:5] ,mux_1level_tapbuf_size2_5_sram_blwl_outb[5:5] ,mux_1level_tapbuf_size2_5_configbus0[5:5], mux_1level_tapbuf_size2_5_configbus1[5:5] , mux_1level_tapbuf_size2_5_configbus0_b[5:5] );
wire [0:1] mux_1level_tapbuf_size2_6_inbus;
assign mux_1level_tapbuf_size2_6_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_6_inbus[1] = chanx_1__0__in_15_ ;
wire [6:6] mux_1level_tapbuf_size2_6_configbus0;
wire [6:6] mux_1level_tapbuf_size2_6_configbus1;
wire [6:6] mux_1level_tapbuf_size2_6_sram_blwl_out ;
wire [6:6] mux_1level_tapbuf_size2_6_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_6_configbus0[6:6] = sram_blwl_bl[6:6] ;
assign mux_1level_tapbuf_size2_6_configbus1[6:6] = sram_blwl_wl[6:6] ;
wire [6:6] mux_1level_tapbuf_size2_6_configbus0_b;
assign mux_1level_tapbuf_size2_6_configbus0_b[6:6] = sram_blwl_blb[6:6] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_6_ (mux_1level_tapbuf_size2_6_inbus, chany_0__1__out_12_ , mux_1level_tapbuf_size2_6_sram_blwl_out[6:6] ,
mux_1level_tapbuf_size2_6_sram_blwl_outb[6:6] );
//----- SRAM bits for MUX[6], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_6_ (mux_1level_tapbuf_size2_6_sram_blwl_out[6:6] ,mux_1level_tapbuf_size2_6_sram_blwl_out[6:6] ,mux_1level_tapbuf_size2_6_sram_blwl_outb[6:6] ,mux_1level_tapbuf_size2_6_configbus0[6:6], mux_1level_tapbuf_size2_6_configbus1[6:6] , mux_1level_tapbuf_size2_6_configbus0_b[6:6] );
wire [0:1] mux_1level_tapbuf_size2_7_inbus;
assign mux_1level_tapbuf_size2_7_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_7_inbus[1] = chanx_1__0__in_17_ ;
wire [7:7] mux_1level_tapbuf_size2_7_configbus0;
wire [7:7] mux_1level_tapbuf_size2_7_configbus1;
wire [7:7] mux_1level_tapbuf_size2_7_sram_blwl_out ;
wire [7:7] mux_1level_tapbuf_size2_7_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_7_configbus0[7:7] = sram_blwl_bl[7:7] ;
assign mux_1level_tapbuf_size2_7_configbus1[7:7] = sram_blwl_wl[7:7] ;
wire [7:7] mux_1level_tapbuf_size2_7_configbus0_b;
assign mux_1level_tapbuf_size2_7_configbus0_b[7:7] = sram_blwl_blb[7:7] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_7_ (mux_1level_tapbuf_size2_7_inbus, chany_0__1__out_14_ , mux_1level_tapbuf_size2_7_sram_blwl_out[7:7] ,
mux_1level_tapbuf_size2_7_sram_blwl_outb[7:7] );
//----- SRAM bits for MUX[7], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_7_ (mux_1level_tapbuf_size2_7_sram_blwl_out[7:7] ,mux_1level_tapbuf_size2_7_sram_blwl_out[7:7] ,mux_1level_tapbuf_size2_7_sram_blwl_outb[7:7] ,mux_1level_tapbuf_size2_7_configbus0[7:7], mux_1level_tapbuf_size2_7_configbus1[7:7] , mux_1level_tapbuf_size2_7_configbus0_b[7:7] );
wire [0:1] mux_1level_tapbuf_size2_8_inbus;
assign mux_1level_tapbuf_size2_8_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_8_inbus[1] = chanx_1__0__in_19_ ;
wire [8:8] mux_1level_tapbuf_size2_8_configbus0;
wire [8:8] mux_1level_tapbuf_size2_8_configbus1;
wire [8:8] mux_1level_tapbuf_size2_8_sram_blwl_out ;
wire [8:8] mux_1level_tapbuf_size2_8_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_8_configbus0[8:8] = sram_blwl_bl[8:8] ;
assign mux_1level_tapbuf_size2_8_configbus1[8:8] = sram_blwl_wl[8:8] ;
wire [8:8] mux_1level_tapbuf_size2_8_configbus0_b;
assign mux_1level_tapbuf_size2_8_configbus0_b[8:8] = sram_blwl_blb[8:8] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_8_ (mux_1level_tapbuf_size2_8_inbus, chany_0__1__out_16_ , mux_1level_tapbuf_size2_8_sram_blwl_out[8:8] ,
mux_1level_tapbuf_size2_8_sram_blwl_outb[8:8] );
//----- SRAM bits for MUX[8], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_8_ (mux_1level_tapbuf_size2_8_sram_blwl_out[8:8] ,mux_1level_tapbuf_size2_8_sram_blwl_out[8:8] ,mux_1level_tapbuf_size2_8_sram_blwl_outb[8:8] ,mux_1level_tapbuf_size2_8_configbus0[8:8], mux_1level_tapbuf_size2_8_configbus1[8:8] , mux_1level_tapbuf_size2_8_configbus0_b[8:8] );
wire [0:1] mux_1level_tapbuf_size2_9_inbus;
assign mux_1level_tapbuf_size2_9_inbus[0] =  grid_0__1__pin_0__1__3_;
assign mux_1level_tapbuf_size2_9_inbus[1] = chanx_1__0__in_21_ ;
wire [9:9] mux_1level_tapbuf_size2_9_configbus0;
wire [9:9] mux_1level_tapbuf_size2_9_configbus1;
wire [9:9] mux_1level_tapbuf_size2_9_sram_blwl_out ;
wire [9:9] mux_1level_tapbuf_size2_9_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_9_configbus0[9:9] = sram_blwl_bl[9:9] ;
assign mux_1level_tapbuf_size2_9_configbus1[9:9] = sram_blwl_wl[9:9] ;
wire [9:9] mux_1level_tapbuf_size2_9_configbus0_b;
assign mux_1level_tapbuf_size2_9_configbus0_b[9:9] = sram_blwl_blb[9:9] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_9_ (mux_1level_tapbuf_size2_9_inbus, chany_0__1__out_18_ , mux_1level_tapbuf_size2_9_sram_blwl_out[9:9] ,
mux_1level_tapbuf_size2_9_sram_blwl_outb[9:9] );
//----- SRAM bits for MUX[9], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_9_ (mux_1level_tapbuf_size2_9_sram_blwl_out[9:9] ,mux_1level_tapbuf_size2_9_sram_blwl_out[9:9] ,mux_1level_tapbuf_size2_9_sram_blwl_outb[9:9] ,mux_1level_tapbuf_size2_9_configbus0[9:9], mux_1level_tapbuf_size2_9_configbus1[9:9] , mux_1level_tapbuf_size2_9_configbus0_b[9:9] );
wire [0:1] mux_1level_tapbuf_size2_10_inbus;
assign mux_1level_tapbuf_size2_10_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_10_inbus[1] = chanx_1__0__in_23_ ;
wire [10:10] mux_1level_tapbuf_size2_10_configbus0;
wire [10:10] mux_1level_tapbuf_size2_10_configbus1;
wire [10:10] mux_1level_tapbuf_size2_10_sram_blwl_out ;
wire [10:10] mux_1level_tapbuf_size2_10_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_10_configbus0[10:10] = sram_blwl_bl[10:10] ;
assign mux_1level_tapbuf_size2_10_configbus1[10:10] = sram_blwl_wl[10:10] ;
wire [10:10] mux_1level_tapbuf_size2_10_configbus0_b;
assign mux_1level_tapbuf_size2_10_configbus0_b[10:10] = sram_blwl_blb[10:10] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_10_ (mux_1level_tapbuf_size2_10_inbus, chany_0__1__out_20_ , mux_1level_tapbuf_size2_10_sram_blwl_out[10:10] ,
mux_1level_tapbuf_size2_10_sram_blwl_outb[10:10] );
//----- SRAM bits for MUX[10], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_10_ (mux_1level_tapbuf_size2_10_sram_blwl_out[10:10] ,mux_1level_tapbuf_size2_10_sram_blwl_out[10:10] ,mux_1level_tapbuf_size2_10_sram_blwl_outb[10:10] ,mux_1level_tapbuf_size2_10_configbus0[10:10], mux_1level_tapbuf_size2_10_configbus1[10:10] , mux_1level_tapbuf_size2_10_configbus0_b[10:10] );
wire [0:1] mux_1level_tapbuf_size2_11_inbus;
assign mux_1level_tapbuf_size2_11_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_11_inbus[1] = chanx_1__0__in_25_ ;
wire [11:11] mux_1level_tapbuf_size2_11_configbus0;
wire [11:11] mux_1level_tapbuf_size2_11_configbus1;
wire [11:11] mux_1level_tapbuf_size2_11_sram_blwl_out ;
wire [11:11] mux_1level_tapbuf_size2_11_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_11_configbus0[11:11] = sram_blwl_bl[11:11] ;
assign mux_1level_tapbuf_size2_11_configbus1[11:11] = sram_blwl_wl[11:11] ;
wire [11:11] mux_1level_tapbuf_size2_11_configbus0_b;
assign mux_1level_tapbuf_size2_11_configbus0_b[11:11] = sram_blwl_blb[11:11] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_11_ (mux_1level_tapbuf_size2_11_inbus, chany_0__1__out_22_ , mux_1level_tapbuf_size2_11_sram_blwl_out[11:11] ,
mux_1level_tapbuf_size2_11_sram_blwl_outb[11:11] );
//----- SRAM bits for MUX[11], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_11_ (mux_1level_tapbuf_size2_11_sram_blwl_out[11:11] ,mux_1level_tapbuf_size2_11_sram_blwl_out[11:11] ,mux_1level_tapbuf_size2_11_sram_blwl_outb[11:11] ,mux_1level_tapbuf_size2_11_configbus0[11:11], mux_1level_tapbuf_size2_11_configbus1[11:11] , mux_1level_tapbuf_size2_11_configbus0_b[11:11] );
wire [0:1] mux_1level_tapbuf_size2_12_inbus;
assign mux_1level_tapbuf_size2_12_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_12_inbus[1] = chanx_1__0__in_27_ ;
wire [12:12] mux_1level_tapbuf_size2_12_configbus0;
wire [12:12] mux_1level_tapbuf_size2_12_configbus1;
wire [12:12] mux_1level_tapbuf_size2_12_sram_blwl_out ;
wire [12:12] mux_1level_tapbuf_size2_12_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_12_configbus0[12:12] = sram_blwl_bl[12:12] ;
assign mux_1level_tapbuf_size2_12_configbus1[12:12] = sram_blwl_wl[12:12] ;
wire [12:12] mux_1level_tapbuf_size2_12_configbus0_b;
assign mux_1level_tapbuf_size2_12_configbus0_b[12:12] = sram_blwl_blb[12:12] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_12_ (mux_1level_tapbuf_size2_12_inbus, chany_0__1__out_24_ , mux_1level_tapbuf_size2_12_sram_blwl_out[12:12] ,
mux_1level_tapbuf_size2_12_sram_blwl_outb[12:12] );
//----- SRAM bits for MUX[12], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_12_ (mux_1level_tapbuf_size2_12_sram_blwl_out[12:12] ,mux_1level_tapbuf_size2_12_sram_blwl_out[12:12] ,mux_1level_tapbuf_size2_12_sram_blwl_outb[12:12] ,mux_1level_tapbuf_size2_12_configbus0[12:12], mux_1level_tapbuf_size2_12_configbus1[12:12] , mux_1level_tapbuf_size2_12_configbus0_b[12:12] );
wire [0:1] mux_1level_tapbuf_size2_13_inbus;
assign mux_1level_tapbuf_size2_13_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_13_inbus[1] = chanx_1__0__in_29_ ;
wire [13:13] mux_1level_tapbuf_size2_13_configbus0;
wire [13:13] mux_1level_tapbuf_size2_13_configbus1;
wire [13:13] mux_1level_tapbuf_size2_13_sram_blwl_out ;
wire [13:13] mux_1level_tapbuf_size2_13_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_13_configbus0[13:13] = sram_blwl_bl[13:13] ;
assign mux_1level_tapbuf_size2_13_configbus1[13:13] = sram_blwl_wl[13:13] ;
wire [13:13] mux_1level_tapbuf_size2_13_configbus0_b;
assign mux_1level_tapbuf_size2_13_configbus0_b[13:13] = sram_blwl_blb[13:13] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_13_ (mux_1level_tapbuf_size2_13_inbus, chany_0__1__out_26_ , mux_1level_tapbuf_size2_13_sram_blwl_out[13:13] ,
mux_1level_tapbuf_size2_13_sram_blwl_outb[13:13] );
//----- SRAM bits for MUX[13], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_13_ (mux_1level_tapbuf_size2_13_sram_blwl_out[13:13] ,mux_1level_tapbuf_size2_13_sram_blwl_out[13:13] ,mux_1level_tapbuf_size2_13_sram_blwl_outb[13:13] ,mux_1level_tapbuf_size2_13_configbus0[13:13], mux_1level_tapbuf_size2_13_configbus1[13:13] , mux_1level_tapbuf_size2_13_configbus0_b[13:13] );
wire [0:1] mux_1level_tapbuf_size2_14_inbus;
assign mux_1level_tapbuf_size2_14_inbus[0] =  grid_0__1__pin_0__1__5_;
assign mux_1level_tapbuf_size2_14_inbus[1] = chanx_1__0__in_31_ ;
wire [14:14] mux_1level_tapbuf_size2_14_configbus0;
wire [14:14] mux_1level_tapbuf_size2_14_configbus1;
wire [14:14] mux_1level_tapbuf_size2_14_sram_blwl_out ;
wire [14:14] mux_1level_tapbuf_size2_14_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_14_configbus0[14:14] = sram_blwl_bl[14:14] ;
assign mux_1level_tapbuf_size2_14_configbus1[14:14] = sram_blwl_wl[14:14] ;
wire [14:14] mux_1level_tapbuf_size2_14_configbus0_b;
assign mux_1level_tapbuf_size2_14_configbus0_b[14:14] = sram_blwl_blb[14:14] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_14_ (mux_1level_tapbuf_size2_14_inbus, chany_0__1__out_28_ , mux_1level_tapbuf_size2_14_sram_blwl_out[14:14] ,
mux_1level_tapbuf_size2_14_sram_blwl_outb[14:14] );
//----- SRAM bits for MUX[14], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_14_ (mux_1level_tapbuf_size2_14_sram_blwl_out[14:14] ,mux_1level_tapbuf_size2_14_sram_blwl_out[14:14] ,mux_1level_tapbuf_size2_14_sram_blwl_outb[14:14] ,mux_1level_tapbuf_size2_14_configbus0[14:14], mux_1level_tapbuf_size2_14_configbus1[14:14] , mux_1level_tapbuf_size2_14_configbus0_b[14:14] );
wire [0:1] mux_1level_tapbuf_size2_15_inbus;
assign mux_1level_tapbuf_size2_15_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_15_inbus[1] = chanx_1__0__in_33_ ;
wire [15:15] mux_1level_tapbuf_size2_15_configbus0;
wire [15:15] mux_1level_tapbuf_size2_15_configbus1;
wire [15:15] mux_1level_tapbuf_size2_15_sram_blwl_out ;
wire [15:15] mux_1level_tapbuf_size2_15_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_15_configbus0[15:15] = sram_blwl_bl[15:15] ;
assign mux_1level_tapbuf_size2_15_configbus1[15:15] = sram_blwl_wl[15:15] ;
wire [15:15] mux_1level_tapbuf_size2_15_configbus0_b;
assign mux_1level_tapbuf_size2_15_configbus0_b[15:15] = sram_blwl_blb[15:15] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_15_ (mux_1level_tapbuf_size2_15_inbus, chany_0__1__out_30_ , mux_1level_tapbuf_size2_15_sram_blwl_out[15:15] ,
mux_1level_tapbuf_size2_15_sram_blwl_outb[15:15] );
//----- SRAM bits for MUX[15], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_15_ (mux_1level_tapbuf_size2_15_sram_blwl_out[15:15] ,mux_1level_tapbuf_size2_15_sram_blwl_out[15:15] ,mux_1level_tapbuf_size2_15_sram_blwl_outb[15:15] ,mux_1level_tapbuf_size2_15_configbus0[15:15], mux_1level_tapbuf_size2_15_configbus1[15:15] , mux_1level_tapbuf_size2_15_configbus0_b[15:15] );
wire [0:1] mux_1level_tapbuf_size2_16_inbus;
assign mux_1level_tapbuf_size2_16_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_16_inbus[1] = chanx_1__0__in_35_ ;
wire [16:16] mux_1level_tapbuf_size2_16_configbus0;
wire [16:16] mux_1level_tapbuf_size2_16_configbus1;
wire [16:16] mux_1level_tapbuf_size2_16_sram_blwl_out ;
wire [16:16] mux_1level_tapbuf_size2_16_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_16_configbus0[16:16] = sram_blwl_bl[16:16] ;
assign mux_1level_tapbuf_size2_16_configbus1[16:16] = sram_blwl_wl[16:16] ;
wire [16:16] mux_1level_tapbuf_size2_16_configbus0_b;
assign mux_1level_tapbuf_size2_16_configbus0_b[16:16] = sram_blwl_blb[16:16] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_16_ (mux_1level_tapbuf_size2_16_inbus, chany_0__1__out_32_ , mux_1level_tapbuf_size2_16_sram_blwl_out[16:16] ,
mux_1level_tapbuf_size2_16_sram_blwl_outb[16:16] );
//----- SRAM bits for MUX[16], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_16_ (mux_1level_tapbuf_size2_16_sram_blwl_out[16:16] ,mux_1level_tapbuf_size2_16_sram_blwl_out[16:16] ,mux_1level_tapbuf_size2_16_sram_blwl_outb[16:16] ,mux_1level_tapbuf_size2_16_configbus0[16:16], mux_1level_tapbuf_size2_16_configbus1[16:16] , mux_1level_tapbuf_size2_16_configbus0_b[16:16] );
wire [0:1] mux_1level_tapbuf_size2_17_inbus;
assign mux_1level_tapbuf_size2_17_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_17_inbus[1] = chanx_1__0__in_37_ ;
wire [17:17] mux_1level_tapbuf_size2_17_configbus0;
wire [17:17] mux_1level_tapbuf_size2_17_configbus1;
wire [17:17] mux_1level_tapbuf_size2_17_sram_blwl_out ;
wire [17:17] mux_1level_tapbuf_size2_17_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_17_configbus0[17:17] = sram_blwl_bl[17:17] ;
assign mux_1level_tapbuf_size2_17_configbus1[17:17] = sram_blwl_wl[17:17] ;
wire [17:17] mux_1level_tapbuf_size2_17_configbus0_b;
assign mux_1level_tapbuf_size2_17_configbus0_b[17:17] = sram_blwl_blb[17:17] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_17_ (mux_1level_tapbuf_size2_17_inbus, chany_0__1__out_34_ , mux_1level_tapbuf_size2_17_sram_blwl_out[17:17] ,
mux_1level_tapbuf_size2_17_sram_blwl_outb[17:17] );
//----- SRAM bits for MUX[17], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_17_ (mux_1level_tapbuf_size2_17_sram_blwl_out[17:17] ,mux_1level_tapbuf_size2_17_sram_blwl_out[17:17] ,mux_1level_tapbuf_size2_17_sram_blwl_outb[17:17] ,mux_1level_tapbuf_size2_17_configbus0[17:17], mux_1level_tapbuf_size2_17_configbus1[17:17] , mux_1level_tapbuf_size2_17_configbus0_b[17:17] );
wire [0:1] mux_1level_tapbuf_size2_18_inbus;
assign mux_1level_tapbuf_size2_18_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_18_inbus[1] = chanx_1__0__in_39_ ;
wire [18:18] mux_1level_tapbuf_size2_18_configbus0;
wire [18:18] mux_1level_tapbuf_size2_18_configbus1;
wire [18:18] mux_1level_tapbuf_size2_18_sram_blwl_out ;
wire [18:18] mux_1level_tapbuf_size2_18_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_18_configbus0[18:18] = sram_blwl_bl[18:18] ;
assign mux_1level_tapbuf_size2_18_configbus1[18:18] = sram_blwl_wl[18:18] ;
wire [18:18] mux_1level_tapbuf_size2_18_configbus0_b;
assign mux_1level_tapbuf_size2_18_configbus0_b[18:18] = sram_blwl_blb[18:18] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_18_ (mux_1level_tapbuf_size2_18_inbus, chany_0__1__out_36_ , mux_1level_tapbuf_size2_18_sram_blwl_out[18:18] ,
mux_1level_tapbuf_size2_18_sram_blwl_outb[18:18] );
//----- SRAM bits for MUX[18], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_18_ (mux_1level_tapbuf_size2_18_sram_blwl_out[18:18] ,mux_1level_tapbuf_size2_18_sram_blwl_out[18:18] ,mux_1level_tapbuf_size2_18_sram_blwl_outb[18:18] ,mux_1level_tapbuf_size2_18_configbus0[18:18], mux_1level_tapbuf_size2_18_configbus1[18:18] , mux_1level_tapbuf_size2_18_configbus0_b[18:18] );
wire [0:1] mux_1level_tapbuf_size2_19_inbus;
assign mux_1level_tapbuf_size2_19_inbus[0] =  grid_0__1__pin_0__1__7_;
assign mux_1level_tapbuf_size2_19_inbus[1] = chanx_1__0__in_41_ ;
wire [19:19] mux_1level_tapbuf_size2_19_configbus0;
wire [19:19] mux_1level_tapbuf_size2_19_configbus1;
wire [19:19] mux_1level_tapbuf_size2_19_sram_blwl_out ;
wire [19:19] mux_1level_tapbuf_size2_19_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_19_configbus0[19:19] = sram_blwl_bl[19:19] ;
assign mux_1level_tapbuf_size2_19_configbus1[19:19] = sram_blwl_wl[19:19] ;
wire [19:19] mux_1level_tapbuf_size2_19_configbus0_b;
assign mux_1level_tapbuf_size2_19_configbus0_b[19:19] = sram_blwl_blb[19:19] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_19_ (mux_1level_tapbuf_size2_19_inbus, chany_0__1__out_38_ , mux_1level_tapbuf_size2_19_sram_blwl_out[19:19] ,
mux_1level_tapbuf_size2_19_sram_blwl_outb[19:19] );
//----- SRAM bits for MUX[19], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_19_ (mux_1level_tapbuf_size2_19_sram_blwl_out[19:19] ,mux_1level_tapbuf_size2_19_sram_blwl_out[19:19] ,mux_1level_tapbuf_size2_19_sram_blwl_outb[19:19] ,mux_1level_tapbuf_size2_19_configbus0[19:19], mux_1level_tapbuf_size2_19_configbus1[19:19] , mux_1level_tapbuf_size2_19_configbus0_b[19:19] );
wire [0:1] mux_1level_tapbuf_size2_20_inbus;
assign mux_1level_tapbuf_size2_20_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_20_inbus[1] = chanx_1__0__in_43_ ;
wire [20:20] mux_1level_tapbuf_size2_20_configbus0;
wire [20:20] mux_1level_tapbuf_size2_20_configbus1;
wire [20:20] mux_1level_tapbuf_size2_20_sram_blwl_out ;
wire [20:20] mux_1level_tapbuf_size2_20_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_20_configbus0[20:20] = sram_blwl_bl[20:20] ;
assign mux_1level_tapbuf_size2_20_configbus1[20:20] = sram_blwl_wl[20:20] ;
wire [20:20] mux_1level_tapbuf_size2_20_configbus0_b;
assign mux_1level_tapbuf_size2_20_configbus0_b[20:20] = sram_blwl_blb[20:20] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_20_ (mux_1level_tapbuf_size2_20_inbus, chany_0__1__out_40_ , mux_1level_tapbuf_size2_20_sram_blwl_out[20:20] ,
mux_1level_tapbuf_size2_20_sram_blwl_outb[20:20] );
//----- SRAM bits for MUX[20], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_20_ (mux_1level_tapbuf_size2_20_sram_blwl_out[20:20] ,mux_1level_tapbuf_size2_20_sram_blwl_out[20:20] ,mux_1level_tapbuf_size2_20_sram_blwl_outb[20:20] ,mux_1level_tapbuf_size2_20_configbus0[20:20], mux_1level_tapbuf_size2_20_configbus1[20:20] , mux_1level_tapbuf_size2_20_configbus0_b[20:20] );
wire [0:1] mux_1level_tapbuf_size2_21_inbus;
assign mux_1level_tapbuf_size2_21_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_21_inbus[1] = chanx_1__0__in_45_ ;
wire [21:21] mux_1level_tapbuf_size2_21_configbus0;
wire [21:21] mux_1level_tapbuf_size2_21_configbus1;
wire [21:21] mux_1level_tapbuf_size2_21_sram_blwl_out ;
wire [21:21] mux_1level_tapbuf_size2_21_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_21_configbus0[21:21] = sram_blwl_bl[21:21] ;
assign mux_1level_tapbuf_size2_21_configbus1[21:21] = sram_blwl_wl[21:21] ;
wire [21:21] mux_1level_tapbuf_size2_21_configbus0_b;
assign mux_1level_tapbuf_size2_21_configbus0_b[21:21] = sram_blwl_blb[21:21] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_21_ (mux_1level_tapbuf_size2_21_inbus, chany_0__1__out_42_ , mux_1level_tapbuf_size2_21_sram_blwl_out[21:21] ,
mux_1level_tapbuf_size2_21_sram_blwl_outb[21:21] );
//----- SRAM bits for MUX[21], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_21_ (mux_1level_tapbuf_size2_21_sram_blwl_out[21:21] ,mux_1level_tapbuf_size2_21_sram_blwl_out[21:21] ,mux_1level_tapbuf_size2_21_sram_blwl_outb[21:21] ,mux_1level_tapbuf_size2_21_configbus0[21:21], mux_1level_tapbuf_size2_21_configbus1[21:21] , mux_1level_tapbuf_size2_21_configbus0_b[21:21] );
wire [0:1] mux_1level_tapbuf_size2_22_inbus;
assign mux_1level_tapbuf_size2_22_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_22_inbus[1] = chanx_1__0__in_47_ ;
wire [22:22] mux_1level_tapbuf_size2_22_configbus0;
wire [22:22] mux_1level_tapbuf_size2_22_configbus1;
wire [22:22] mux_1level_tapbuf_size2_22_sram_blwl_out ;
wire [22:22] mux_1level_tapbuf_size2_22_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_22_configbus0[22:22] = sram_blwl_bl[22:22] ;
assign mux_1level_tapbuf_size2_22_configbus1[22:22] = sram_blwl_wl[22:22] ;
wire [22:22] mux_1level_tapbuf_size2_22_configbus0_b;
assign mux_1level_tapbuf_size2_22_configbus0_b[22:22] = sram_blwl_blb[22:22] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_22_ (mux_1level_tapbuf_size2_22_inbus, chany_0__1__out_44_ , mux_1level_tapbuf_size2_22_sram_blwl_out[22:22] ,
mux_1level_tapbuf_size2_22_sram_blwl_outb[22:22] );
//----- SRAM bits for MUX[22], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_22_ (mux_1level_tapbuf_size2_22_sram_blwl_out[22:22] ,mux_1level_tapbuf_size2_22_sram_blwl_out[22:22] ,mux_1level_tapbuf_size2_22_sram_blwl_outb[22:22] ,mux_1level_tapbuf_size2_22_configbus0[22:22], mux_1level_tapbuf_size2_22_configbus1[22:22] , mux_1level_tapbuf_size2_22_configbus0_b[22:22] );
wire [0:1] mux_1level_tapbuf_size2_23_inbus;
assign mux_1level_tapbuf_size2_23_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_23_inbus[1] = chanx_1__0__in_49_ ;
wire [23:23] mux_1level_tapbuf_size2_23_configbus0;
wire [23:23] mux_1level_tapbuf_size2_23_configbus1;
wire [23:23] mux_1level_tapbuf_size2_23_sram_blwl_out ;
wire [23:23] mux_1level_tapbuf_size2_23_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_23_configbus0[23:23] = sram_blwl_bl[23:23] ;
assign mux_1level_tapbuf_size2_23_configbus1[23:23] = sram_blwl_wl[23:23] ;
wire [23:23] mux_1level_tapbuf_size2_23_configbus0_b;
assign mux_1level_tapbuf_size2_23_configbus0_b[23:23] = sram_blwl_blb[23:23] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_23_ (mux_1level_tapbuf_size2_23_inbus, chany_0__1__out_46_ , mux_1level_tapbuf_size2_23_sram_blwl_out[23:23] ,
mux_1level_tapbuf_size2_23_sram_blwl_outb[23:23] );
//----- SRAM bits for MUX[23], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_23_ (mux_1level_tapbuf_size2_23_sram_blwl_out[23:23] ,mux_1level_tapbuf_size2_23_sram_blwl_out[23:23] ,mux_1level_tapbuf_size2_23_sram_blwl_outb[23:23] ,mux_1level_tapbuf_size2_23_configbus0[23:23], mux_1level_tapbuf_size2_23_configbus1[23:23] , mux_1level_tapbuf_size2_23_configbus0_b[23:23] );
wire [0:1] mux_1level_tapbuf_size2_24_inbus;
assign mux_1level_tapbuf_size2_24_inbus[0] =  grid_0__1__pin_0__1__9_;
assign mux_1level_tapbuf_size2_24_inbus[1] = chanx_1__0__in_51_ ;
wire [24:24] mux_1level_tapbuf_size2_24_configbus0;
wire [24:24] mux_1level_tapbuf_size2_24_configbus1;
wire [24:24] mux_1level_tapbuf_size2_24_sram_blwl_out ;
wire [24:24] mux_1level_tapbuf_size2_24_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_24_configbus0[24:24] = sram_blwl_bl[24:24] ;
assign mux_1level_tapbuf_size2_24_configbus1[24:24] = sram_blwl_wl[24:24] ;
wire [24:24] mux_1level_tapbuf_size2_24_configbus0_b;
assign mux_1level_tapbuf_size2_24_configbus0_b[24:24] = sram_blwl_blb[24:24] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_24_ (mux_1level_tapbuf_size2_24_inbus, chany_0__1__out_48_ , mux_1level_tapbuf_size2_24_sram_blwl_out[24:24] ,
mux_1level_tapbuf_size2_24_sram_blwl_outb[24:24] );
//----- SRAM bits for MUX[24], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_24_ (mux_1level_tapbuf_size2_24_sram_blwl_out[24:24] ,mux_1level_tapbuf_size2_24_sram_blwl_out[24:24] ,mux_1level_tapbuf_size2_24_sram_blwl_outb[24:24] ,mux_1level_tapbuf_size2_24_configbus0[24:24], mux_1level_tapbuf_size2_24_configbus1[24:24] , mux_1level_tapbuf_size2_24_configbus0_b[24:24] );
wire [0:1] mux_1level_tapbuf_size2_25_inbus;
assign mux_1level_tapbuf_size2_25_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_25_inbus[1] = chanx_1__0__in_53_ ;
wire [25:25] mux_1level_tapbuf_size2_25_configbus0;
wire [25:25] mux_1level_tapbuf_size2_25_configbus1;
wire [25:25] mux_1level_tapbuf_size2_25_sram_blwl_out ;
wire [25:25] mux_1level_tapbuf_size2_25_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_25_configbus0[25:25] = sram_blwl_bl[25:25] ;
assign mux_1level_tapbuf_size2_25_configbus1[25:25] = sram_blwl_wl[25:25] ;
wire [25:25] mux_1level_tapbuf_size2_25_configbus0_b;
assign mux_1level_tapbuf_size2_25_configbus0_b[25:25] = sram_blwl_blb[25:25] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_25_ (mux_1level_tapbuf_size2_25_inbus, chany_0__1__out_50_ , mux_1level_tapbuf_size2_25_sram_blwl_out[25:25] ,
mux_1level_tapbuf_size2_25_sram_blwl_outb[25:25] );
//----- SRAM bits for MUX[25], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_25_ (mux_1level_tapbuf_size2_25_sram_blwl_out[25:25] ,mux_1level_tapbuf_size2_25_sram_blwl_out[25:25] ,mux_1level_tapbuf_size2_25_sram_blwl_outb[25:25] ,mux_1level_tapbuf_size2_25_configbus0[25:25], mux_1level_tapbuf_size2_25_configbus1[25:25] , mux_1level_tapbuf_size2_25_configbus0_b[25:25] );
wire [0:1] mux_1level_tapbuf_size2_26_inbus;
assign mux_1level_tapbuf_size2_26_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_26_inbus[1] = chanx_1__0__in_55_ ;
wire [26:26] mux_1level_tapbuf_size2_26_configbus0;
wire [26:26] mux_1level_tapbuf_size2_26_configbus1;
wire [26:26] mux_1level_tapbuf_size2_26_sram_blwl_out ;
wire [26:26] mux_1level_tapbuf_size2_26_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_26_configbus0[26:26] = sram_blwl_bl[26:26] ;
assign mux_1level_tapbuf_size2_26_configbus1[26:26] = sram_blwl_wl[26:26] ;
wire [26:26] mux_1level_tapbuf_size2_26_configbus0_b;
assign mux_1level_tapbuf_size2_26_configbus0_b[26:26] = sram_blwl_blb[26:26] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_26_ (mux_1level_tapbuf_size2_26_inbus, chany_0__1__out_52_ , mux_1level_tapbuf_size2_26_sram_blwl_out[26:26] ,
mux_1level_tapbuf_size2_26_sram_blwl_outb[26:26] );
//----- SRAM bits for MUX[26], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_26_ (mux_1level_tapbuf_size2_26_sram_blwl_out[26:26] ,mux_1level_tapbuf_size2_26_sram_blwl_out[26:26] ,mux_1level_tapbuf_size2_26_sram_blwl_outb[26:26] ,mux_1level_tapbuf_size2_26_configbus0[26:26], mux_1level_tapbuf_size2_26_configbus1[26:26] , mux_1level_tapbuf_size2_26_configbus0_b[26:26] );
wire [0:1] mux_1level_tapbuf_size2_27_inbus;
assign mux_1level_tapbuf_size2_27_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_27_inbus[1] = chanx_1__0__in_57_ ;
wire [27:27] mux_1level_tapbuf_size2_27_configbus0;
wire [27:27] mux_1level_tapbuf_size2_27_configbus1;
wire [27:27] mux_1level_tapbuf_size2_27_sram_blwl_out ;
wire [27:27] mux_1level_tapbuf_size2_27_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_27_configbus0[27:27] = sram_blwl_bl[27:27] ;
assign mux_1level_tapbuf_size2_27_configbus1[27:27] = sram_blwl_wl[27:27] ;
wire [27:27] mux_1level_tapbuf_size2_27_configbus0_b;
assign mux_1level_tapbuf_size2_27_configbus0_b[27:27] = sram_blwl_blb[27:27] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_27_ (mux_1level_tapbuf_size2_27_inbus, chany_0__1__out_54_ , mux_1level_tapbuf_size2_27_sram_blwl_out[27:27] ,
mux_1level_tapbuf_size2_27_sram_blwl_outb[27:27] );
//----- SRAM bits for MUX[27], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_27_ (mux_1level_tapbuf_size2_27_sram_blwl_out[27:27] ,mux_1level_tapbuf_size2_27_sram_blwl_out[27:27] ,mux_1level_tapbuf_size2_27_sram_blwl_outb[27:27] ,mux_1level_tapbuf_size2_27_configbus0[27:27], mux_1level_tapbuf_size2_27_configbus1[27:27] , mux_1level_tapbuf_size2_27_configbus0_b[27:27] );
wire [0:1] mux_1level_tapbuf_size2_28_inbus;
assign mux_1level_tapbuf_size2_28_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_28_inbus[1] = chanx_1__0__in_59_ ;
wire [28:28] mux_1level_tapbuf_size2_28_configbus0;
wire [28:28] mux_1level_tapbuf_size2_28_configbus1;
wire [28:28] mux_1level_tapbuf_size2_28_sram_blwl_out ;
wire [28:28] mux_1level_tapbuf_size2_28_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_28_configbus0[28:28] = sram_blwl_bl[28:28] ;
assign mux_1level_tapbuf_size2_28_configbus1[28:28] = sram_blwl_wl[28:28] ;
wire [28:28] mux_1level_tapbuf_size2_28_configbus0_b;
assign mux_1level_tapbuf_size2_28_configbus0_b[28:28] = sram_blwl_blb[28:28] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_28_ (mux_1level_tapbuf_size2_28_inbus, chany_0__1__out_56_ , mux_1level_tapbuf_size2_28_sram_blwl_out[28:28] ,
mux_1level_tapbuf_size2_28_sram_blwl_outb[28:28] );
//----- SRAM bits for MUX[28], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_28_ (mux_1level_tapbuf_size2_28_sram_blwl_out[28:28] ,mux_1level_tapbuf_size2_28_sram_blwl_out[28:28] ,mux_1level_tapbuf_size2_28_sram_blwl_outb[28:28] ,mux_1level_tapbuf_size2_28_configbus0[28:28], mux_1level_tapbuf_size2_28_configbus1[28:28] , mux_1level_tapbuf_size2_28_configbus0_b[28:28] );
wire [0:1] mux_1level_tapbuf_size2_29_inbus;
assign mux_1level_tapbuf_size2_29_inbus[0] =  grid_0__1__pin_0__1__11_;
assign mux_1level_tapbuf_size2_29_inbus[1] = chanx_1__0__in_61_ ;
wire [29:29] mux_1level_tapbuf_size2_29_configbus0;
wire [29:29] mux_1level_tapbuf_size2_29_configbus1;
wire [29:29] mux_1level_tapbuf_size2_29_sram_blwl_out ;
wire [29:29] mux_1level_tapbuf_size2_29_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_29_configbus0[29:29] = sram_blwl_bl[29:29] ;
assign mux_1level_tapbuf_size2_29_configbus1[29:29] = sram_blwl_wl[29:29] ;
wire [29:29] mux_1level_tapbuf_size2_29_configbus0_b;
assign mux_1level_tapbuf_size2_29_configbus0_b[29:29] = sram_blwl_blb[29:29] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_29_ (mux_1level_tapbuf_size2_29_inbus, chany_0__1__out_58_ , mux_1level_tapbuf_size2_29_sram_blwl_out[29:29] ,
mux_1level_tapbuf_size2_29_sram_blwl_outb[29:29] );
//----- SRAM bits for MUX[29], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_29_ (mux_1level_tapbuf_size2_29_sram_blwl_out[29:29] ,mux_1level_tapbuf_size2_29_sram_blwl_out[29:29] ,mux_1level_tapbuf_size2_29_sram_blwl_outb[29:29] ,mux_1level_tapbuf_size2_29_configbus0[29:29], mux_1level_tapbuf_size2_29_configbus1[29:29] , mux_1level_tapbuf_size2_29_configbus0_b[29:29] );
wire [0:1] mux_1level_tapbuf_size2_30_inbus;
assign mux_1level_tapbuf_size2_30_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_30_inbus[1] = chanx_1__0__in_63_ ;
wire [30:30] mux_1level_tapbuf_size2_30_configbus0;
wire [30:30] mux_1level_tapbuf_size2_30_configbus1;
wire [30:30] mux_1level_tapbuf_size2_30_sram_blwl_out ;
wire [30:30] mux_1level_tapbuf_size2_30_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_30_configbus0[30:30] = sram_blwl_bl[30:30] ;
assign mux_1level_tapbuf_size2_30_configbus1[30:30] = sram_blwl_wl[30:30] ;
wire [30:30] mux_1level_tapbuf_size2_30_configbus0_b;
assign mux_1level_tapbuf_size2_30_configbus0_b[30:30] = sram_blwl_blb[30:30] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_30_ (mux_1level_tapbuf_size2_30_inbus, chany_0__1__out_60_ , mux_1level_tapbuf_size2_30_sram_blwl_out[30:30] ,
mux_1level_tapbuf_size2_30_sram_blwl_outb[30:30] );
//----- SRAM bits for MUX[30], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_30_ (mux_1level_tapbuf_size2_30_sram_blwl_out[30:30] ,mux_1level_tapbuf_size2_30_sram_blwl_out[30:30] ,mux_1level_tapbuf_size2_30_sram_blwl_outb[30:30] ,mux_1level_tapbuf_size2_30_configbus0[30:30], mux_1level_tapbuf_size2_30_configbus1[30:30] , mux_1level_tapbuf_size2_30_configbus0_b[30:30] );
wire [0:1] mux_1level_tapbuf_size2_31_inbus;
assign mux_1level_tapbuf_size2_31_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_31_inbus[1] = chanx_1__0__in_65_ ;
wire [31:31] mux_1level_tapbuf_size2_31_configbus0;
wire [31:31] mux_1level_tapbuf_size2_31_configbus1;
wire [31:31] mux_1level_tapbuf_size2_31_sram_blwl_out ;
wire [31:31] mux_1level_tapbuf_size2_31_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_31_configbus0[31:31] = sram_blwl_bl[31:31] ;
assign mux_1level_tapbuf_size2_31_configbus1[31:31] = sram_blwl_wl[31:31] ;
wire [31:31] mux_1level_tapbuf_size2_31_configbus0_b;
assign mux_1level_tapbuf_size2_31_configbus0_b[31:31] = sram_blwl_blb[31:31] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_31_ (mux_1level_tapbuf_size2_31_inbus, chany_0__1__out_62_ , mux_1level_tapbuf_size2_31_sram_blwl_out[31:31] ,
mux_1level_tapbuf_size2_31_sram_blwl_outb[31:31] );
//----- SRAM bits for MUX[31], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_31_ (mux_1level_tapbuf_size2_31_sram_blwl_out[31:31] ,mux_1level_tapbuf_size2_31_sram_blwl_out[31:31] ,mux_1level_tapbuf_size2_31_sram_blwl_outb[31:31] ,mux_1level_tapbuf_size2_31_configbus0[31:31], mux_1level_tapbuf_size2_31_configbus1[31:31] , mux_1level_tapbuf_size2_31_configbus0_b[31:31] );
wire [0:1] mux_1level_tapbuf_size2_32_inbus;
assign mux_1level_tapbuf_size2_32_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_32_inbus[1] = chanx_1__0__in_67_ ;
wire [32:32] mux_1level_tapbuf_size2_32_configbus0;
wire [32:32] mux_1level_tapbuf_size2_32_configbus1;
wire [32:32] mux_1level_tapbuf_size2_32_sram_blwl_out ;
wire [32:32] mux_1level_tapbuf_size2_32_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_32_configbus0[32:32] = sram_blwl_bl[32:32] ;
assign mux_1level_tapbuf_size2_32_configbus1[32:32] = sram_blwl_wl[32:32] ;
wire [32:32] mux_1level_tapbuf_size2_32_configbus0_b;
assign mux_1level_tapbuf_size2_32_configbus0_b[32:32] = sram_blwl_blb[32:32] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_32_ (mux_1level_tapbuf_size2_32_inbus, chany_0__1__out_64_ , mux_1level_tapbuf_size2_32_sram_blwl_out[32:32] ,
mux_1level_tapbuf_size2_32_sram_blwl_outb[32:32] );
//----- SRAM bits for MUX[32], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_32_ (mux_1level_tapbuf_size2_32_sram_blwl_out[32:32] ,mux_1level_tapbuf_size2_32_sram_blwl_out[32:32] ,mux_1level_tapbuf_size2_32_sram_blwl_outb[32:32] ,mux_1level_tapbuf_size2_32_configbus0[32:32], mux_1level_tapbuf_size2_32_configbus1[32:32] , mux_1level_tapbuf_size2_32_configbus0_b[32:32] );
wire [0:1] mux_1level_tapbuf_size2_33_inbus;
assign mux_1level_tapbuf_size2_33_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_33_inbus[1] = chanx_1__0__in_69_ ;
wire [33:33] mux_1level_tapbuf_size2_33_configbus0;
wire [33:33] mux_1level_tapbuf_size2_33_configbus1;
wire [33:33] mux_1level_tapbuf_size2_33_sram_blwl_out ;
wire [33:33] mux_1level_tapbuf_size2_33_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_33_configbus0[33:33] = sram_blwl_bl[33:33] ;
assign mux_1level_tapbuf_size2_33_configbus1[33:33] = sram_blwl_wl[33:33] ;
wire [33:33] mux_1level_tapbuf_size2_33_configbus0_b;
assign mux_1level_tapbuf_size2_33_configbus0_b[33:33] = sram_blwl_blb[33:33] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_33_ (mux_1level_tapbuf_size2_33_inbus, chany_0__1__out_66_ , mux_1level_tapbuf_size2_33_sram_blwl_out[33:33] ,
mux_1level_tapbuf_size2_33_sram_blwl_outb[33:33] );
//----- SRAM bits for MUX[33], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_33_ (mux_1level_tapbuf_size2_33_sram_blwl_out[33:33] ,mux_1level_tapbuf_size2_33_sram_blwl_out[33:33] ,mux_1level_tapbuf_size2_33_sram_blwl_outb[33:33] ,mux_1level_tapbuf_size2_33_configbus0[33:33], mux_1level_tapbuf_size2_33_configbus1[33:33] , mux_1level_tapbuf_size2_33_configbus0_b[33:33] );
wire [0:1] mux_1level_tapbuf_size2_34_inbus;
assign mux_1level_tapbuf_size2_34_inbus[0] =  grid_0__1__pin_0__1__13_;
assign mux_1level_tapbuf_size2_34_inbus[1] = chanx_1__0__in_71_ ;
wire [34:34] mux_1level_tapbuf_size2_34_configbus0;
wire [34:34] mux_1level_tapbuf_size2_34_configbus1;
wire [34:34] mux_1level_tapbuf_size2_34_sram_blwl_out ;
wire [34:34] mux_1level_tapbuf_size2_34_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_34_configbus0[34:34] = sram_blwl_bl[34:34] ;
assign mux_1level_tapbuf_size2_34_configbus1[34:34] = sram_blwl_wl[34:34] ;
wire [34:34] mux_1level_tapbuf_size2_34_configbus0_b;
assign mux_1level_tapbuf_size2_34_configbus0_b[34:34] = sram_blwl_blb[34:34] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_34_ (mux_1level_tapbuf_size2_34_inbus, chany_0__1__out_68_ , mux_1level_tapbuf_size2_34_sram_blwl_out[34:34] ,
mux_1level_tapbuf_size2_34_sram_blwl_outb[34:34] );
//----- SRAM bits for MUX[34], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_34_ (mux_1level_tapbuf_size2_34_sram_blwl_out[34:34] ,mux_1level_tapbuf_size2_34_sram_blwl_out[34:34] ,mux_1level_tapbuf_size2_34_sram_blwl_outb[34:34] ,mux_1level_tapbuf_size2_34_configbus0[34:34], mux_1level_tapbuf_size2_34_configbus1[34:34] , mux_1level_tapbuf_size2_34_configbus0_b[34:34] );
wire [0:1] mux_1level_tapbuf_size2_35_inbus;
assign mux_1level_tapbuf_size2_35_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_35_inbus[1] = chanx_1__0__in_73_ ;
wire [35:35] mux_1level_tapbuf_size2_35_configbus0;
wire [35:35] mux_1level_tapbuf_size2_35_configbus1;
wire [35:35] mux_1level_tapbuf_size2_35_sram_blwl_out ;
wire [35:35] mux_1level_tapbuf_size2_35_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_35_configbus0[35:35] = sram_blwl_bl[35:35] ;
assign mux_1level_tapbuf_size2_35_configbus1[35:35] = sram_blwl_wl[35:35] ;
wire [35:35] mux_1level_tapbuf_size2_35_configbus0_b;
assign mux_1level_tapbuf_size2_35_configbus0_b[35:35] = sram_blwl_blb[35:35] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_35_ (mux_1level_tapbuf_size2_35_inbus, chany_0__1__out_70_ , mux_1level_tapbuf_size2_35_sram_blwl_out[35:35] ,
mux_1level_tapbuf_size2_35_sram_blwl_outb[35:35] );
//----- SRAM bits for MUX[35], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_35_ (mux_1level_tapbuf_size2_35_sram_blwl_out[35:35] ,mux_1level_tapbuf_size2_35_sram_blwl_out[35:35] ,mux_1level_tapbuf_size2_35_sram_blwl_outb[35:35] ,mux_1level_tapbuf_size2_35_configbus0[35:35], mux_1level_tapbuf_size2_35_configbus1[35:35] , mux_1level_tapbuf_size2_35_configbus0_b[35:35] );
wire [0:1] mux_1level_tapbuf_size2_36_inbus;
assign mux_1level_tapbuf_size2_36_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_36_inbus[1] = chanx_1__0__in_75_ ;
wire [36:36] mux_1level_tapbuf_size2_36_configbus0;
wire [36:36] mux_1level_tapbuf_size2_36_configbus1;
wire [36:36] mux_1level_tapbuf_size2_36_sram_blwl_out ;
wire [36:36] mux_1level_tapbuf_size2_36_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_36_configbus0[36:36] = sram_blwl_bl[36:36] ;
assign mux_1level_tapbuf_size2_36_configbus1[36:36] = sram_blwl_wl[36:36] ;
wire [36:36] mux_1level_tapbuf_size2_36_configbus0_b;
assign mux_1level_tapbuf_size2_36_configbus0_b[36:36] = sram_blwl_blb[36:36] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_36_ (mux_1level_tapbuf_size2_36_inbus, chany_0__1__out_72_ , mux_1level_tapbuf_size2_36_sram_blwl_out[36:36] ,
mux_1level_tapbuf_size2_36_sram_blwl_outb[36:36] );
//----- SRAM bits for MUX[36], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_36_ (mux_1level_tapbuf_size2_36_sram_blwl_out[36:36] ,mux_1level_tapbuf_size2_36_sram_blwl_out[36:36] ,mux_1level_tapbuf_size2_36_sram_blwl_outb[36:36] ,mux_1level_tapbuf_size2_36_configbus0[36:36], mux_1level_tapbuf_size2_36_configbus1[36:36] , mux_1level_tapbuf_size2_36_configbus0_b[36:36] );
wire [0:1] mux_1level_tapbuf_size2_37_inbus;
assign mux_1level_tapbuf_size2_37_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_37_inbus[1] = chanx_1__0__in_77_ ;
wire [37:37] mux_1level_tapbuf_size2_37_configbus0;
wire [37:37] mux_1level_tapbuf_size2_37_configbus1;
wire [37:37] mux_1level_tapbuf_size2_37_sram_blwl_out ;
wire [37:37] mux_1level_tapbuf_size2_37_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_37_configbus0[37:37] = sram_blwl_bl[37:37] ;
assign mux_1level_tapbuf_size2_37_configbus1[37:37] = sram_blwl_wl[37:37] ;
wire [37:37] mux_1level_tapbuf_size2_37_configbus0_b;
assign mux_1level_tapbuf_size2_37_configbus0_b[37:37] = sram_blwl_blb[37:37] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_37_ (mux_1level_tapbuf_size2_37_inbus, chany_0__1__out_74_ , mux_1level_tapbuf_size2_37_sram_blwl_out[37:37] ,
mux_1level_tapbuf_size2_37_sram_blwl_outb[37:37] );
//----- SRAM bits for MUX[37], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_37_ (mux_1level_tapbuf_size2_37_sram_blwl_out[37:37] ,mux_1level_tapbuf_size2_37_sram_blwl_out[37:37] ,mux_1level_tapbuf_size2_37_sram_blwl_outb[37:37] ,mux_1level_tapbuf_size2_37_configbus0[37:37], mux_1level_tapbuf_size2_37_configbus1[37:37] , mux_1level_tapbuf_size2_37_configbus0_b[37:37] );
wire [0:1] mux_1level_tapbuf_size2_38_inbus;
assign mux_1level_tapbuf_size2_38_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_38_inbus[1] = chanx_1__0__in_79_ ;
wire [38:38] mux_1level_tapbuf_size2_38_configbus0;
wire [38:38] mux_1level_tapbuf_size2_38_configbus1;
wire [38:38] mux_1level_tapbuf_size2_38_sram_blwl_out ;
wire [38:38] mux_1level_tapbuf_size2_38_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_38_configbus0[38:38] = sram_blwl_bl[38:38] ;
assign mux_1level_tapbuf_size2_38_configbus1[38:38] = sram_blwl_wl[38:38] ;
wire [38:38] mux_1level_tapbuf_size2_38_configbus0_b;
assign mux_1level_tapbuf_size2_38_configbus0_b[38:38] = sram_blwl_blb[38:38] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_38_ (mux_1level_tapbuf_size2_38_inbus, chany_0__1__out_76_ , mux_1level_tapbuf_size2_38_sram_blwl_out[38:38] ,
mux_1level_tapbuf_size2_38_sram_blwl_outb[38:38] );
//----- SRAM bits for MUX[38], level=1, select_path_id=1. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----0-----
sram6T_blwl sram_blwl_38_ (mux_1level_tapbuf_size2_38_sram_blwl_out[38:38] ,mux_1level_tapbuf_size2_38_sram_blwl_out[38:38] ,mux_1level_tapbuf_size2_38_sram_blwl_outb[38:38] ,mux_1level_tapbuf_size2_38_configbus0[38:38], mux_1level_tapbuf_size2_38_configbus1[38:38] , mux_1level_tapbuf_size2_38_configbus0_b[38:38] );
wire [0:1] mux_1level_tapbuf_size2_39_inbus;
assign mux_1level_tapbuf_size2_39_inbus[0] =  grid_0__1__pin_0__1__15_;
assign mux_1level_tapbuf_size2_39_inbus[1] = chanx_1__0__in_81_ ;
wire [39:39] mux_1level_tapbuf_size2_39_configbus0;
wire [39:39] mux_1level_tapbuf_size2_39_configbus1;
wire [39:39] mux_1level_tapbuf_size2_39_sram_blwl_out ;
wire [39:39] mux_1level_tapbuf_size2_39_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_39_configbus0[39:39] = sram_blwl_bl[39:39] ;
assign mux_1level_tapbuf_size2_39_configbus1[39:39] = sram_blwl_wl[39:39] ;
wire [39:39] mux_1level_tapbuf_size2_39_configbus0_b;
assign mux_1level_tapbuf_size2_39_configbus0_b[39:39] = sram_blwl_blb[39:39] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_39_ (mux_1level_tapbuf_size2_39_inbus, chany_0__1__out_78_ , mux_1level_tapbuf_size2_39_sram_blwl_out[39:39] ,
mux_1level_tapbuf_size2_39_sram_blwl_outb[39:39] );
//----- SRAM bits for MUX[39], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_39_ (mux_1level_tapbuf_size2_39_sram_blwl_out[39:39] ,mux_1level_tapbuf_size2_39_sram_blwl_out[39:39] ,mux_1level_tapbuf_size2_39_sram_blwl_outb[39:39] ,mux_1level_tapbuf_size2_39_configbus0[39:39], mux_1level_tapbuf_size2_39_configbus1[39:39] , mux_1level_tapbuf_size2_39_configbus0_b[39:39] );
wire [0:1] mux_1level_tapbuf_size2_40_inbus;
assign mux_1level_tapbuf_size2_40_inbus[0] =  grid_1__1__pin_0__3__43_;
assign mux_1level_tapbuf_size2_40_inbus[1] = chanx_1__0__in_83_ ;
wire [40:40] mux_1level_tapbuf_size2_40_configbus0;
wire [40:40] mux_1level_tapbuf_size2_40_configbus1;
wire [40:40] mux_1level_tapbuf_size2_40_sram_blwl_out ;
wire [40:40] mux_1level_tapbuf_size2_40_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_40_configbus0[40:40] = sram_blwl_bl[40:40] ;
assign mux_1level_tapbuf_size2_40_configbus1[40:40] = sram_blwl_wl[40:40] ;
wire [40:40] mux_1level_tapbuf_size2_40_configbus0_b;
assign mux_1level_tapbuf_size2_40_configbus0_b[40:40] = sram_blwl_blb[40:40] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_40_ (mux_1level_tapbuf_size2_40_inbus, chany_0__1__out_80_ , mux_1level_tapbuf_size2_40_sram_blwl_out[40:40] ,
mux_1level_tapbuf_size2_40_sram_blwl_outb[40:40] );
//----- SRAM bits for MUX[40], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_40_ (mux_1level_tapbuf_size2_40_sram_blwl_out[40:40] ,mux_1level_tapbuf_size2_40_sram_blwl_out[40:40] ,mux_1level_tapbuf_size2_40_sram_blwl_outb[40:40] ,mux_1level_tapbuf_size2_40_configbus0[40:40], mux_1level_tapbuf_size2_40_configbus1[40:40] , mux_1level_tapbuf_size2_40_configbus0_b[40:40] );
wire [0:1] mux_1level_tapbuf_size2_41_inbus;
assign mux_1level_tapbuf_size2_41_inbus[0] =  grid_1__1__pin_0__3__43_;
assign mux_1level_tapbuf_size2_41_inbus[1] = chanx_1__0__in_85_ ;
wire [41:41] mux_1level_tapbuf_size2_41_configbus0;
wire [41:41] mux_1level_tapbuf_size2_41_configbus1;
wire [41:41] mux_1level_tapbuf_size2_41_sram_blwl_out ;
wire [41:41] mux_1level_tapbuf_size2_41_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_41_configbus0[41:41] = sram_blwl_bl[41:41] ;
assign mux_1level_tapbuf_size2_41_configbus1[41:41] = sram_blwl_wl[41:41] ;
wire [41:41] mux_1level_tapbuf_size2_41_configbus0_b;
assign mux_1level_tapbuf_size2_41_configbus0_b[41:41] = sram_blwl_blb[41:41] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_41_ (mux_1level_tapbuf_size2_41_inbus, chany_0__1__out_82_ , mux_1level_tapbuf_size2_41_sram_blwl_out[41:41] ,
mux_1level_tapbuf_size2_41_sram_blwl_outb[41:41] );
//----- SRAM bits for MUX[41], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_41_ (mux_1level_tapbuf_size2_41_sram_blwl_out[41:41] ,mux_1level_tapbuf_size2_41_sram_blwl_out[41:41] ,mux_1level_tapbuf_size2_41_sram_blwl_outb[41:41] ,mux_1level_tapbuf_size2_41_configbus0[41:41], mux_1level_tapbuf_size2_41_configbus1[41:41] , mux_1level_tapbuf_size2_41_configbus0_b[41:41] );
wire [0:1] mux_1level_tapbuf_size2_42_inbus;
assign mux_1level_tapbuf_size2_42_inbus[0] =  grid_1__1__pin_0__3__43_;
assign mux_1level_tapbuf_size2_42_inbus[1] = chanx_1__0__in_87_ ;
wire [42:42] mux_1level_tapbuf_size2_42_configbus0;
wire [42:42] mux_1level_tapbuf_size2_42_configbus1;
wire [42:42] mux_1level_tapbuf_size2_42_sram_blwl_out ;
wire [42:42] mux_1level_tapbuf_size2_42_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_42_configbus0[42:42] = sram_blwl_bl[42:42] ;
assign mux_1level_tapbuf_size2_42_configbus1[42:42] = sram_blwl_wl[42:42] ;
wire [42:42] mux_1level_tapbuf_size2_42_configbus0_b;
assign mux_1level_tapbuf_size2_42_configbus0_b[42:42] = sram_blwl_blb[42:42] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_42_ (mux_1level_tapbuf_size2_42_inbus, chany_0__1__out_84_ , mux_1level_tapbuf_size2_42_sram_blwl_out[42:42] ,
mux_1level_tapbuf_size2_42_sram_blwl_outb[42:42] );
//----- SRAM bits for MUX[42], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_42_ (mux_1level_tapbuf_size2_42_sram_blwl_out[42:42] ,mux_1level_tapbuf_size2_42_sram_blwl_out[42:42] ,mux_1level_tapbuf_size2_42_sram_blwl_outb[42:42] ,mux_1level_tapbuf_size2_42_configbus0[42:42], mux_1level_tapbuf_size2_42_configbus1[42:42] , mux_1level_tapbuf_size2_42_configbus0_b[42:42] );
wire [0:1] mux_1level_tapbuf_size2_43_inbus;
assign mux_1level_tapbuf_size2_43_inbus[0] =  grid_1__1__pin_0__3__43_;
assign mux_1level_tapbuf_size2_43_inbus[1] = chanx_1__0__in_89_ ;
wire [43:43] mux_1level_tapbuf_size2_43_configbus0;
wire [43:43] mux_1level_tapbuf_size2_43_configbus1;
wire [43:43] mux_1level_tapbuf_size2_43_sram_blwl_out ;
wire [43:43] mux_1level_tapbuf_size2_43_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_43_configbus0[43:43] = sram_blwl_bl[43:43] ;
assign mux_1level_tapbuf_size2_43_configbus1[43:43] = sram_blwl_wl[43:43] ;
wire [43:43] mux_1level_tapbuf_size2_43_configbus0_b;
assign mux_1level_tapbuf_size2_43_configbus0_b[43:43] = sram_blwl_blb[43:43] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_43_ (mux_1level_tapbuf_size2_43_inbus, chany_0__1__out_86_ , mux_1level_tapbuf_size2_43_sram_blwl_out[43:43] ,
mux_1level_tapbuf_size2_43_sram_blwl_outb[43:43] );
//----- SRAM bits for MUX[43], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_43_ (mux_1level_tapbuf_size2_43_sram_blwl_out[43:43] ,mux_1level_tapbuf_size2_43_sram_blwl_out[43:43] ,mux_1level_tapbuf_size2_43_sram_blwl_outb[43:43] ,mux_1level_tapbuf_size2_43_configbus0[43:43], mux_1level_tapbuf_size2_43_configbus1[43:43] , mux_1level_tapbuf_size2_43_configbus0_b[43:43] );
wire [0:1] mux_1level_tapbuf_size2_44_inbus;
assign mux_1level_tapbuf_size2_44_inbus[0] =  grid_1__1__pin_0__3__43_;
assign mux_1level_tapbuf_size2_44_inbus[1] = chanx_1__0__in_91_ ;
wire [44:44] mux_1level_tapbuf_size2_44_configbus0;
wire [44:44] mux_1level_tapbuf_size2_44_configbus1;
wire [44:44] mux_1level_tapbuf_size2_44_sram_blwl_out ;
wire [44:44] mux_1level_tapbuf_size2_44_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_44_configbus0[44:44] = sram_blwl_bl[44:44] ;
assign mux_1level_tapbuf_size2_44_configbus1[44:44] = sram_blwl_wl[44:44] ;
wire [44:44] mux_1level_tapbuf_size2_44_configbus0_b;
assign mux_1level_tapbuf_size2_44_configbus0_b[44:44] = sram_blwl_blb[44:44] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_44_ (mux_1level_tapbuf_size2_44_inbus, chany_0__1__out_88_ , mux_1level_tapbuf_size2_44_sram_blwl_out[44:44] ,
mux_1level_tapbuf_size2_44_sram_blwl_outb[44:44] );
//----- SRAM bits for MUX[44], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_44_ (mux_1level_tapbuf_size2_44_sram_blwl_out[44:44] ,mux_1level_tapbuf_size2_44_sram_blwl_out[44:44] ,mux_1level_tapbuf_size2_44_sram_blwl_outb[44:44] ,mux_1level_tapbuf_size2_44_configbus0[44:44], mux_1level_tapbuf_size2_44_configbus1[44:44] , mux_1level_tapbuf_size2_44_configbus0_b[44:44] );
wire [0:1] mux_1level_tapbuf_size2_45_inbus;
assign mux_1level_tapbuf_size2_45_inbus[0] =  grid_1__1__pin_0__3__47_;
assign mux_1level_tapbuf_size2_45_inbus[1] = chanx_1__0__in_93_ ;
wire [45:45] mux_1level_tapbuf_size2_45_configbus0;
wire [45:45] mux_1level_tapbuf_size2_45_configbus1;
wire [45:45] mux_1level_tapbuf_size2_45_sram_blwl_out ;
wire [45:45] mux_1level_tapbuf_size2_45_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_45_configbus0[45:45] = sram_blwl_bl[45:45] ;
assign mux_1level_tapbuf_size2_45_configbus1[45:45] = sram_blwl_wl[45:45] ;
wire [45:45] mux_1level_tapbuf_size2_45_configbus0_b;
assign mux_1level_tapbuf_size2_45_configbus0_b[45:45] = sram_blwl_blb[45:45] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_45_ (mux_1level_tapbuf_size2_45_inbus, chany_0__1__out_90_ , mux_1level_tapbuf_size2_45_sram_blwl_out[45:45] ,
mux_1level_tapbuf_size2_45_sram_blwl_outb[45:45] );
//----- SRAM bits for MUX[45], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_45_ (mux_1level_tapbuf_size2_45_sram_blwl_out[45:45] ,mux_1level_tapbuf_size2_45_sram_blwl_out[45:45] ,mux_1level_tapbuf_size2_45_sram_blwl_outb[45:45] ,mux_1level_tapbuf_size2_45_configbus0[45:45], mux_1level_tapbuf_size2_45_configbus1[45:45] , mux_1level_tapbuf_size2_45_configbus0_b[45:45] );
wire [0:1] mux_1level_tapbuf_size2_46_inbus;
assign mux_1level_tapbuf_size2_46_inbus[0] =  grid_1__1__pin_0__3__47_;
assign mux_1level_tapbuf_size2_46_inbus[1] = chanx_1__0__in_95_ ;
wire [46:46] mux_1level_tapbuf_size2_46_configbus0;
wire [46:46] mux_1level_tapbuf_size2_46_configbus1;
wire [46:46] mux_1level_tapbuf_size2_46_sram_blwl_out ;
wire [46:46] mux_1level_tapbuf_size2_46_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_46_configbus0[46:46] = sram_blwl_bl[46:46] ;
assign mux_1level_tapbuf_size2_46_configbus1[46:46] = sram_blwl_wl[46:46] ;
wire [46:46] mux_1level_tapbuf_size2_46_configbus0_b;
assign mux_1level_tapbuf_size2_46_configbus0_b[46:46] = sram_blwl_blb[46:46] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_46_ (mux_1level_tapbuf_size2_46_inbus, chany_0__1__out_92_ , mux_1level_tapbuf_size2_46_sram_blwl_out[46:46] ,
mux_1level_tapbuf_size2_46_sram_blwl_outb[46:46] );
//----- SRAM bits for MUX[46], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_46_ (mux_1level_tapbuf_size2_46_sram_blwl_out[46:46] ,mux_1level_tapbuf_size2_46_sram_blwl_out[46:46] ,mux_1level_tapbuf_size2_46_sram_blwl_outb[46:46] ,mux_1level_tapbuf_size2_46_configbus0[46:46], mux_1level_tapbuf_size2_46_configbus1[46:46] , mux_1level_tapbuf_size2_46_configbus0_b[46:46] );
wire [0:1] mux_1level_tapbuf_size2_47_inbus;
assign mux_1level_tapbuf_size2_47_inbus[0] =  grid_1__1__pin_0__3__47_;
assign mux_1level_tapbuf_size2_47_inbus[1] = chanx_1__0__in_97_ ;
wire [47:47] mux_1level_tapbuf_size2_47_configbus0;
wire [47:47] mux_1level_tapbuf_size2_47_configbus1;
wire [47:47] mux_1level_tapbuf_size2_47_sram_blwl_out ;
wire [47:47] mux_1level_tapbuf_size2_47_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_47_configbus0[47:47] = sram_blwl_bl[47:47] ;
assign mux_1level_tapbuf_size2_47_configbus1[47:47] = sram_blwl_wl[47:47] ;
wire [47:47] mux_1level_tapbuf_size2_47_configbus0_b;
assign mux_1level_tapbuf_size2_47_configbus0_b[47:47] = sram_blwl_blb[47:47] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_47_ (mux_1level_tapbuf_size2_47_inbus, chany_0__1__out_94_ , mux_1level_tapbuf_size2_47_sram_blwl_out[47:47] ,
mux_1level_tapbuf_size2_47_sram_blwl_outb[47:47] );
//----- SRAM bits for MUX[47], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_47_ (mux_1level_tapbuf_size2_47_sram_blwl_out[47:47] ,mux_1level_tapbuf_size2_47_sram_blwl_out[47:47] ,mux_1level_tapbuf_size2_47_sram_blwl_outb[47:47] ,mux_1level_tapbuf_size2_47_configbus0[47:47], mux_1level_tapbuf_size2_47_configbus1[47:47] , mux_1level_tapbuf_size2_47_configbus0_b[47:47] );
wire [0:1] mux_1level_tapbuf_size2_48_inbus;
assign mux_1level_tapbuf_size2_48_inbus[0] =  grid_1__1__pin_0__3__47_;
assign mux_1level_tapbuf_size2_48_inbus[1] = chanx_1__0__in_99_ ;
wire [48:48] mux_1level_tapbuf_size2_48_configbus0;
wire [48:48] mux_1level_tapbuf_size2_48_configbus1;
wire [48:48] mux_1level_tapbuf_size2_48_sram_blwl_out ;
wire [48:48] mux_1level_tapbuf_size2_48_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_48_configbus0[48:48] = sram_blwl_bl[48:48] ;
assign mux_1level_tapbuf_size2_48_configbus1[48:48] = sram_blwl_wl[48:48] ;
wire [48:48] mux_1level_tapbuf_size2_48_configbus0_b;
assign mux_1level_tapbuf_size2_48_configbus0_b[48:48] = sram_blwl_blb[48:48] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_48_ (mux_1level_tapbuf_size2_48_inbus, chany_0__1__out_96_ , mux_1level_tapbuf_size2_48_sram_blwl_out[48:48] ,
mux_1level_tapbuf_size2_48_sram_blwl_outb[48:48] );
//----- SRAM bits for MUX[48], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_48_ (mux_1level_tapbuf_size2_48_sram_blwl_out[48:48] ,mux_1level_tapbuf_size2_48_sram_blwl_out[48:48] ,mux_1level_tapbuf_size2_48_sram_blwl_outb[48:48] ,mux_1level_tapbuf_size2_48_configbus0[48:48], mux_1level_tapbuf_size2_48_configbus1[48:48] , mux_1level_tapbuf_size2_48_configbus0_b[48:48] );
wire [0:1] mux_1level_tapbuf_size2_49_inbus;
assign mux_1level_tapbuf_size2_49_inbus[0] =  grid_1__1__pin_0__3__47_;
assign mux_1level_tapbuf_size2_49_inbus[1] = chanx_1__0__in_1_ ;
wire [49:49] mux_1level_tapbuf_size2_49_configbus0;
wire [49:49] mux_1level_tapbuf_size2_49_configbus1;
wire [49:49] mux_1level_tapbuf_size2_49_sram_blwl_out ;
wire [49:49] mux_1level_tapbuf_size2_49_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_49_configbus0[49:49] = sram_blwl_bl[49:49] ;
assign mux_1level_tapbuf_size2_49_configbus1[49:49] = sram_blwl_wl[49:49] ;
wire [49:49] mux_1level_tapbuf_size2_49_configbus0_b;
assign mux_1level_tapbuf_size2_49_configbus0_b[49:49] = sram_blwl_blb[49:49] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_49_ (mux_1level_tapbuf_size2_49_inbus, chany_0__1__out_98_ , mux_1level_tapbuf_size2_49_sram_blwl_out[49:49] ,
mux_1level_tapbuf_size2_49_sram_blwl_outb[49:49] );
//----- SRAM bits for MUX[49], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_49_ (mux_1level_tapbuf_size2_49_sram_blwl_out[49:49] ,mux_1level_tapbuf_size2_49_sram_blwl_out[49:49] ,mux_1level_tapbuf_size2_49_sram_blwl_outb[49:49] ,mux_1level_tapbuf_size2_49_configbus0[49:49], mux_1level_tapbuf_size2_49_configbus1[49:49] , mux_1level_tapbuf_size2_49_configbus0_b[49:49] );
//----- right side Multiplexers -----
wire [0:1] mux_1level_tapbuf_size2_50_inbus;
assign mux_1level_tapbuf_size2_50_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_50_inbus[1] = chany_0__1__in_99_ ;
wire [50:50] mux_1level_tapbuf_size2_50_configbus0;
wire [50:50] mux_1level_tapbuf_size2_50_configbus1;
wire [50:50] mux_1level_tapbuf_size2_50_sram_blwl_out ;
wire [50:50] mux_1level_tapbuf_size2_50_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_50_configbus0[50:50] = sram_blwl_bl[50:50] ;
assign mux_1level_tapbuf_size2_50_configbus1[50:50] = sram_blwl_wl[50:50] ;
wire [50:50] mux_1level_tapbuf_size2_50_configbus0_b;
assign mux_1level_tapbuf_size2_50_configbus0_b[50:50] = sram_blwl_blb[50:50] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_50_ (mux_1level_tapbuf_size2_50_inbus, chanx_1__0__out_0_ , mux_1level_tapbuf_size2_50_sram_blwl_out[50:50] ,
mux_1level_tapbuf_size2_50_sram_blwl_outb[50:50] );
//----- SRAM bits for MUX[50], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_50_ (mux_1level_tapbuf_size2_50_sram_blwl_out[50:50] ,mux_1level_tapbuf_size2_50_sram_blwl_out[50:50] ,mux_1level_tapbuf_size2_50_sram_blwl_outb[50:50] ,mux_1level_tapbuf_size2_50_configbus0[50:50], mux_1level_tapbuf_size2_50_configbus1[50:50] , mux_1level_tapbuf_size2_50_configbus0_b[50:50] );
wire [0:1] mux_1level_tapbuf_size2_51_inbus;
assign mux_1level_tapbuf_size2_51_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_51_inbus[1] = chany_0__1__in_1_ ;
wire [51:51] mux_1level_tapbuf_size2_51_configbus0;
wire [51:51] mux_1level_tapbuf_size2_51_configbus1;
wire [51:51] mux_1level_tapbuf_size2_51_sram_blwl_out ;
wire [51:51] mux_1level_tapbuf_size2_51_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_51_configbus0[51:51] = sram_blwl_bl[51:51] ;
assign mux_1level_tapbuf_size2_51_configbus1[51:51] = sram_blwl_wl[51:51] ;
wire [51:51] mux_1level_tapbuf_size2_51_configbus0_b;
assign mux_1level_tapbuf_size2_51_configbus0_b[51:51] = sram_blwl_blb[51:51] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_51_ (mux_1level_tapbuf_size2_51_inbus, chanx_1__0__out_2_ , mux_1level_tapbuf_size2_51_sram_blwl_out[51:51] ,
mux_1level_tapbuf_size2_51_sram_blwl_outb[51:51] );
//----- SRAM bits for MUX[51], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_51_ (mux_1level_tapbuf_size2_51_sram_blwl_out[51:51] ,mux_1level_tapbuf_size2_51_sram_blwl_out[51:51] ,mux_1level_tapbuf_size2_51_sram_blwl_outb[51:51] ,mux_1level_tapbuf_size2_51_configbus0[51:51], mux_1level_tapbuf_size2_51_configbus1[51:51] , mux_1level_tapbuf_size2_51_configbus0_b[51:51] );
wire [0:1] mux_1level_tapbuf_size2_52_inbus;
assign mux_1level_tapbuf_size2_52_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_52_inbus[1] = chany_0__1__in_3_ ;
wire [52:52] mux_1level_tapbuf_size2_52_configbus0;
wire [52:52] mux_1level_tapbuf_size2_52_configbus1;
wire [52:52] mux_1level_tapbuf_size2_52_sram_blwl_out ;
wire [52:52] mux_1level_tapbuf_size2_52_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_52_configbus0[52:52] = sram_blwl_bl[52:52] ;
assign mux_1level_tapbuf_size2_52_configbus1[52:52] = sram_blwl_wl[52:52] ;
wire [52:52] mux_1level_tapbuf_size2_52_configbus0_b;
assign mux_1level_tapbuf_size2_52_configbus0_b[52:52] = sram_blwl_blb[52:52] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_52_ (mux_1level_tapbuf_size2_52_inbus, chanx_1__0__out_4_ , mux_1level_tapbuf_size2_52_sram_blwl_out[52:52] ,
mux_1level_tapbuf_size2_52_sram_blwl_outb[52:52] );
//----- SRAM bits for MUX[52], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_52_ (mux_1level_tapbuf_size2_52_sram_blwl_out[52:52] ,mux_1level_tapbuf_size2_52_sram_blwl_out[52:52] ,mux_1level_tapbuf_size2_52_sram_blwl_outb[52:52] ,mux_1level_tapbuf_size2_52_configbus0[52:52], mux_1level_tapbuf_size2_52_configbus1[52:52] , mux_1level_tapbuf_size2_52_configbus0_b[52:52] );
wire [0:1] mux_1level_tapbuf_size2_53_inbus;
assign mux_1level_tapbuf_size2_53_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_53_inbus[1] = chany_0__1__in_5_ ;
wire [53:53] mux_1level_tapbuf_size2_53_configbus0;
wire [53:53] mux_1level_tapbuf_size2_53_configbus1;
wire [53:53] mux_1level_tapbuf_size2_53_sram_blwl_out ;
wire [53:53] mux_1level_tapbuf_size2_53_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_53_configbus0[53:53] = sram_blwl_bl[53:53] ;
assign mux_1level_tapbuf_size2_53_configbus1[53:53] = sram_blwl_wl[53:53] ;
wire [53:53] mux_1level_tapbuf_size2_53_configbus0_b;
assign mux_1level_tapbuf_size2_53_configbus0_b[53:53] = sram_blwl_blb[53:53] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_53_ (mux_1level_tapbuf_size2_53_inbus, chanx_1__0__out_6_ , mux_1level_tapbuf_size2_53_sram_blwl_out[53:53] ,
mux_1level_tapbuf_size2_53_sram_blwl_outb[53:53] );
//----- SRAM bits for MUX[53], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_53_ (mux_1level_tapbuf_size2_53_sram_blwl_out[53:53] ,mux_1level_tapbuf_size2_53_sram_blwl_out[53:53] ,mux_1level_tapbuf_size2_53_sram_blwl_outb[53:53] ,mux_1level_tapbuf_size2_53_configbus0[53:53], mux_1level_tapbuf_size2_53_configbus1[53:53] , mux_1level_tapbuf_size2_53_configbus0_b[53:53] );
wire [0:1] mux_1level_tapbuf_size2_54_inbus;
assign mux_1level_tapbuf_size2_54_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_54_inbus[1] = chany_0__1__in_7_ ;
wire [54:54] mux_1level_tapbuf_size2_54_configbus0;
wire [54:54] mux_1level_tapbuf_size2_54_configbus1;
wire [54:54] mux_1level_tapbuf_size2_54_sram_blwl_out ;
wire [54:54] mux_1level_tapbuf_size2_54_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_54_configbus0[54:54] = sram_blwl_bl[54:54] ;
assign mux_1level_tapbuf_size2_54_configbus1[54:54] = sram_blwl_wl[54:54] ;
wire [54:54] mux_1level_tapbuf_size2_54_configbus0_b;
assign mux_1level_tapbuf_size2_54_configbus0_b[54:54] = sram_blwl_blb[54:54] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_54_ (mux_1level_tapbuf_size2_54_inbus, chanx_1__0__out_8_ , mux_1level_tapbuf_size2_54_sram_blwl_out[54:54] ,
mux_1level_tapbuf_size2_54_sram_blwl_outb[54:54] );
//----- SRAM bits for MUX[54], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_54_ (mux_1level_tapbuf_size2_54_sram_blwl_out[54:54] ,mux_1level_tapbuf_size2_54_sram_blwl_out[54:54] ,mux_1level_tapbuf_size2_54_sram_blwl_outb[54:54] ,mux_1level_tapbuf_size2_54_configbus0[54:54], mux_1level_tapbuf_size2_54_configbus1[54:54] , mux_1level_tapbuf_size2_54_configbus0_b[54:54] );
wire [0:1] mux_1level_tapbuf_size2_55_inbus;
assign mux_1level_tapbuf_size2_55_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_55_inbus[1] = chany_0__1__in_9_ ;
wire [55:55] mux_1level_tapbuf_size2_55_configbus0;
wire [55:55] mux_1level_tapbuf_size2_55_configbus1;
wire [55:55] mux_1level_tapbuf_size2_55_sram_blwl_out ;
wire [55:55] mux_1level_tapbuf_size2_55_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_55_configbus0[55:55] = sram_blwl_bl[55:55] ;
assign mux_1level_tapbuf_size2_55_configbus1[55:55] = sram_blwl_wl[55:55] ;
wire [55:55] mux_1level_tapbuf_size2_55_configbus0_b;
assign mux_1level_tapbuf_size2_55_configbus0_b[55:55] = sram_blwl_blb[55:55] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_55_ (mux_1level_tapbuf_size2_55_inbus, chanx_1__0__out_10_ , mux_1level_tapbuf_size2_55_sram_blwl_out[55:55] ,
mux_1level_tapbuf_size2_55_sram_blwl_outb[55:55] );
//----- SRAM bits for MUX[55], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_55_ (mux_1level_tapbuf_size2_55_sram_blwl_out[55:55] ,mux_1level_tapbuf_size2_55_sram_blwl_out[55:55] ,mux_1level_tapbuf_size2_55_sram_blwl_outb[55:55] ,mux_1level_tapbuf_size2_55_configbus0[55:55], mux_1level_tapbuf_size2_55_configbus1[55:55] , mux_1level_tapbuf_size2_55_configbus0_b[55:55] );
wire [0:1] mux_1level_tapbuf_size2_56_inbus;
assign mux_1level_tapbuf_size2_56_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_56_inbus[1] = chany_0__1__in_11_ ;
wire [56:56] mux_1level_tapbuf_size2_56_configbus0;
wire [56:56] mux_1level_tapbuf_size2_56_configbus1;
wire [56:56] mux_1level_tapbuf_size2_56_sram_blwl_out ;
wire [56:56] mux_1level_tapbuf_size2_56_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_56_configbus0[56:56] = sram_blwl_bl[56:56] ;
assign mux_1level_tapbuf_size2_56_configbus1[56:56] = sram_blwl_wl[56:56] ;
wire [56:56] mux_1level_tapbuf_size2_56_configbus0_b;
assign mux_1level_tapbuf_size2_56_configbus0_b[56:56] = sram_blwl_blb[56:56] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_56_ (mux_1level_tapbuf_size2_56_inbus, chanx_1__0__out_12_ , mux_1level_tapbuf_size2_56_sram_blwl_out[56:56] ,
mux_1level_tapbuf_size2_56_sram_blwl_outb[56:56] );
//----- SRAM bits for MUX[56], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_56_ (mux_1level_tapbuf_size2_56_sram_blwl_out[56:56] ,mux_1level_tapbuf_size2_56_sram_blwl_out[56:56] ,mux_1level_tapbuf_size2_56_sram_blwl_outb[56:56] ,mux_1level_tapbuf_size2_56_configbus0[56:56], mux_1level_tapbuf_size2_56_configbus1[56:56] , mux_1level_tapbuf_size2_56_configbus0_b[56:56] );
wire [0:1] mux_1level_tapbuf_size2_57_inbus;
assign mux_1level_tapbuf_size2_57_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_57_inbus[1] = chany_0__1__in_13_ ;
wire [57:57] mux_1level_tapbuf_size2_57_configbus0;
wire [57:57] mux_1level_tapbuf_size2_57_configbus1;
wire [57:57] mux_1level_tapbuf_size2_57_sram_blwl_out ;
wire [57:57] mux_1level_tapbuf_size2_57_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_57_configbus0[57:57] = sram_blwl_bl[57:57] ;
assign mux_1level_tapbuf_size2_57_configbus1[57:57] = sram_blwl_wl[57:57] ;
wire [57:57] mux_1level_tapbuf_size2_57_configbus0_b;
assign mux_1level_tapbuf_size2_57_configbus0_b[57:57] = sram_blwl_blb[57:57] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_57_ (mux_1level_tapbuf_size2_57_inbus, chanx_1__0__out_14_ , mux_1level_tapbuf_size2_57_sram_blwl_out[57:57] ,
mux_1level_tapbuf_size2_57_sram_blwl_outb[57:57] );
//----- SRAM bits for MUX[57], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_57_ (mux_1level_tapbuf_size2_57_sram_blwl_out[57:57] ,mux_1level_tapbuf_size2_57_sram_blwl_out[57:57] ,mux_1level_tapbuf_size2_57_sram_blwl_outb[57:57] ,mux_1level_tapbuf_size2_57_configbus0[57:57], mux_1level_tapbuf_size2_57_configbus1[57:57] , mux_1level_tapbuf_size2_57_configbus0_b[57:57] );
wire [0:1] mux_1level_tapbuf_size2_58_inbus;
assign mux_1level_tapbuf_size2_58_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_58_inbus[1] = chany_0__1__in_15_ ;
wire [58:58] mux_1level_tapbuf_size2_58_configbus0;
wire [58:58] mux_1level_tapbuf_size2_58_configbus1;
wire [58:58] mux_1level_tapbuf_size2_58_sram_blwl_out ;
wire [58:58] mux_1level_tapbuf_size2_58_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_58_configbus0[58:58] = sram_blwl_bl[58:58] ;
assign mux_1level_tapbuf_size2_58_configbus1[58:58] = sram_blwl_wl[58:58] ;
wire [58:58] mux_1level_tapbuf_size2_58_configbus0_b;
assign mux_1level_tapbuf_size2_58_configbus0_b[58:58] = sram_blwl_blb[58:58] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_58_ (mux_1level_tapbuf_size2_58_inbus, chanx_1__0__out_16_ , mux_1level_tapbuf_size2_58_sram_blwl_out[58:58] ,
mux_1level_tapbuf_size2_58_sram_blwl_outb[58:58] );
//----- SRAM bits for MUX[58], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_58_ (mux_1level_tapbuf_size2_58_sram_blwl_out[58:58] ,mux_1level_tapbuf_size2_58_sram_blwl_out[58:58] ,mux_1level_tapbuf_size2_58_sram_blwl_outb[58:58] ,mux_1level_tapbuf_size2_58_configbus0[58:58], mux_1level_tapbuf_size2_58_configbus1[58:58] , mux_1level_tapbuf_size2_58_configbus0_b[58:58] );
wire [0:1] mux_1level_tapbuf_size2_59_inbus;
assign mux_1level_tapbuf_size2_59_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_59_inbus[1] = chany_0__1__in_17_ ;
wire [59:59] mux_1level_tapbuf_size2_59_configbus0;
wire [59:59] mux_1level_tapbuf_size2_59_configbus1;
wire [59:59] mux_1level_tapbuf_size2_59_sram_blwl_out ;
wire [59:59] mux_1level_tapbuf_size2_59_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_59_configbus0[59:59] = sram_blwl_bl[59:59] ;
assign mux_1level_tapbuf_size2_59_configbus1[59:59] = sram_blwl_wl[59:59] ;
wire [59:59] mux_1level_tapbuf_size2_59_configbus0_b;
assign mux_1level_tapbuf_size2_59_configbus0_b[59:59] = sram_blwl_blb[59:59] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_59_ (mux_1level_tapbuf_size2_59_inbus, chanx_1__0__out_18_ , mux_1level_tapbuf_size2_59_sram_blwl_out[59:59] ,
mux_1level_tapbuf_size2_59_sram_blwl_outb[59:59] );
//----- SRAM bits for MUX[59], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_59_ (mux_1level_tapbuf_size2_59_sram_blwl_out[59:59] ,mux_1level_tapbuf_size2_59_sram_blwl_out[59:59] ,mux_1level_tapbuf_size2_59_sram_blwl_outb[59:59] ,mux_1level_tapbuf_size2_59_configbus0[59:59], mux_1level_tapbuf_size2_59_configbus1[59:59] , mux_1level_tapbuf_size2_59_configbus0_b[59:59] );
wire [0:1] mux_1level_tapbuf_size2_60_inbus;
assign mux_1level_tapbuf_size2_60_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_60_inbus[1] = chany_0__1__in_19_ ;
wire [60:60] mux_1level_tapbuf_size2_60_configbus0;
wire [60:60] mux_1level_tapbuf_size2_60_configbus1;
wire [60:60] mux_1level_tapbuf_size2_60_sram_blwl_out ;
wire [60:60] mux_1level_tapbuf_size2_60_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_60_configbus0[60:60] = sram_blwl_bl[60:60] ;
assign mux_1level_tapbuf_size2_60_configbus1[60:60] = sram_blwl_wl[60:60] ;
wire [60:60] mux_1level_tapbuf_size2_60_configbus0_b;
assign mux_1level_tapbuf_size2_60_configbus0_b[60:60] = sram_blwl_blb[60:60] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_60_ (mux_1level_tapbuf_size2_60_inbus, chanx_1__0__out_20_ , mux_1level_tapbuf_size2_60_sram_blwl_out[60:60] ,
mux_1level_tapbuf_size2_60_sram_blwl_outb[60:60] );
//----- SRAM bits for MUX[60], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_60_ (mux_1level_tapbuf_size2_60_sram_blwl_out[60:60] ,mux_1level_tapbuf_size2_60_sram_blwl_out[60:60] ,mux_1level_tapbuf_size2_60_sram_blwl_outb[60:60] ,mux_1level_tapbuf_size2_60_configbus0[60:60], mux_1level_tapbuf_size2_60_configbus1[60:60] , mux_1level_tapbuf_size2_60_configbus0_b[60:60] );
wire [0:1] mux_1level_tapbuf_size2_61_inbus;
assign mux_1level_tapbuf_size2_61_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_61_inbus[1] = chany_0__1__in_21_ ;
wire [61:61] mux_1level_tapbuf_size2_61_configbus0;
wire [61:61] mux_1level_tapbuf_size2_61_configbus1;
wire [61:61] mux_1level_tapbuf_size2_61_sram_blwl_out ;
wire [61:61] mux_1level_tapbuf_size2_61_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_61_configbus0[61:61] = sram_blwl_bl[61:61] ;
assign mux_1level_tapbuf_size2_61_configbus1[61:61] = sram_blwl_wl[61:61] ;
wire [61:61] mux_1level_tapbuf_size2_61_configbus0_b;
assign mux_1level_tapbuf_size2_61_configbus0_b[61:61] = sram_blwl_blb[61:61] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_61_ (mux_1level_tapbuf_size2_61_inbus, chanx_1__0__out_22_ , mux_1level_tapbuf_size2_61_sram_blwl_out[61:61] ,
mux_1level_tapbuf_size2_61_sram_blwl_outb[61:61] );
//----- SRAM bits for MUX[61], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_61_ (mux_1level_tapbuf_size2_61_sram_blwl_out[61:61] ,mux_1level_tapbuf_size2_61_sram_blwl_out[61:61] ,mux_1level_tapbuf_size2_61_sram_blwl_outb[61:61] ,mux_1level_tapbuf_size2_61_configbus0[61:61], mux_1level_tapbuf_size2_61_configbus1[61:61] , mux_1level_tapbuf_size2_61_configbus0_b[61:61] );
wire [0:1] mux_1level_tapbuf_size2_62_inbus;
assign mux_1level_tapbuf_size2_62_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_62_inbus[1] = chany_0__1__in_23_ ;
wire [62:62] mux_1level_tapbuf_size2_62_configbus0;
wire [62:62] mux_1level_tapbuf_size2_62_configbus1;
wire [62:62] mux_1level_tapbuf_size2_62_sram_blwl_out ;
wire [62:62] mux_1level_tapbuf_size2_62_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_62_configbus0[62:62] = sram_blwl_bl[62:62] ;
assign mux_1level_tapbuf_size2_62_configbus1[62:62] = sram_blwl_wl[62:62] ;
wire [62:62] mux_1level_tapbuf_size2_62_configbus0_b;
assign mux_1level_tapbuf_size2_62_configbus0_b[62:62] = sram_blwl_blb[62:62] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_62_ (mux_1level_tapbuf_size2_62_inbus, chanx_1__0__out_24_ , mux_1level_tapbuf_size2_62_sram_blwl_out[62:62] ,
mux_1level_tapbuf_size2_62_sram_blwl_outb[62:62] );
//----- SRAM bits for MUX[62], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_62_ (mux_1level_tapbuf_size2_62_sram_blwl_out[62:62] ,mux_1level_tapbuf_size2_62_sram_blwl_out[62:62] ,mux_1level_tapbuf_size2_62_sram_blwl_outb[62:62] ,mux_1level_tapbuf_size2_62_configbus0[62:62], mux_1level_tapbuf_size2_62_configbus1[62:62] , mux_1level_tapbuf_size2_62_configbus0_b[62:62] );
wire [0:1] mux_1level_tapbuf_size2_63_inbus;
assign mux_1level_tapbuf_size2_63_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_63_inbus[1] = chany_0__1__in_25_ ;
wire [63:63] mux_1level_tapbuf_size2_63_configbus0;
wire [63:63] mux_1level_tapbuf_size2_63_configbus1;
wire [63:63] mux_1level_tapbuf_size2_63_sram_blwl_out ;
wire [63:63] mux_1level_tapbuf_size2_63_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_63_configbus0[63:63] = sram_blwl_bl[63:63] ;
assign mux_1level_tapbuf_size2_63_configbus1[63:63] = sram_blwl_wl[63:63] ;
wire [63:63] mux_1level_tapbuf_size2_63_configbus0_b;
assign mux_1level_tapbuf_size2_63_configbus0_b[63:63] = sram_blwl_blb[63:63] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_63_ (mux_1level_tapbuf_size2_63_inbus, chanx_1__0__out_26_ , mux_1level_tapbuf_size2_63_sram_blwl_out[63:63] ,
mux_1level_tapbuf_size2_63_sram_blwl_outb[63:63] );
//----- SRAM bits for MUX[63], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_63_ (mux_1level_tapbuf_size2_63_sram_blwl_out[63:63] ,mux_1level_tapbuf_size2_63_sram_blwl_out[63:63] ,mux_1level_tapbuf_size2_63_sram_blwl_outb[63:63] ,mux_1level_tapbuf_size2_63_configbus0[63:63], mux_1level_tapbuf_size2_63_configbus1[63:63] , mux_1level_tapbuf_size2_63_configbus0_b[63:63] );
wire [0:1] mux_1level_tapbuf_size2_64_inbus;
assign mux_1level_tapbuf_size2_64_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_64_inbus[1] = chany_0__1__in_27_ ;
wire [64:64] mux_1level_tapbuf_size2_64_configbus0;
wire [64:64] mux_1level_tapbuf_size2_64_configbus1;
wire [64:64] mux_1level_tapbuf_size2_64_sram_blwl_out ;
wire [64:64] mux_1level_tapbuf_size2_64_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_64_configbus0[64:64] = sram_blwl_bl[64:64] ;
assign mux_1level_tapbuf_size2_64_configbus1[64:64] = sram_blwl_wl[64:64] ;
wire [64:64] mux_1level_tapbuf_size2_64_configbus0_b;
assign mux_1level_tapbuf_size2_64_configbus0_b[64:64] = sram_blwl_blb[64:64] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_64_ (mux_1level_tapbuf_size2_64_inbus, chanx_1__0__out_28_ , mux_1level_tapbuf_size2_64_sram_blwl_out[64:64] ,
mux_1level_tapbuf_size2_64_sram_blwl_outb[64:64] );
//----- SRAM bits for MUX[64], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_64_ (mux_1level_tapbuf_size2_64_sram_blwl_out[64:64] ,mux_1level_tapbuf_size2_64_sram_blwl_out[64:64] ,mux_1level_tapbuf_size2_64_sram_blwl_outb[64:64] ,mux_1level_tapbuf_size2_64_configbus0[64:64], mux_1level_tapbuf_size2_64_configbus1[64:64] , mux_1level_tapbuf_size2_64_configbus0_b[64:64] );
wire [0:1] mux_1level_tapbuf_size2_65_inbus;
assign mux_1level_tapbuf_size2_65_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_65_inbus[1] = chany_0__1__in_29_ ;
wire [65:65] mux_1level_tapbuf_size2_65_configbus0;
wire [65:65] mux_1level_tapbuf_size2_65_configbus1;
wire [65:65] mux_1level_tapbuf_size2_65_sram_blwl_out ;
wire [65:65] mux_1level_tapbuf_size2_65_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_65_configbus0[65:65] = sram_blwl_bl[65:65] ;
assign mux_1level_tapbuf_size2_65_configbus1[65:65] = sram_blwl_wl[65:65] ;
wire [65:65] mux_1level_tapbuf_size2_65_configbus0_b;
assign mux_1level_tapbuf_size2_65_configbus0_b[65:65] = sram_blwl_blb[65:65] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_65_ (mux_1level_tapbuf_size2_65_inbus, chanx_1__0__out_30_ , mux_1level_tapbuf_size2_65_sram_blwl_out[65:65] ,
mux_1level_tapbuf_size2_65_sram_blwl_outb[65:65] );
//----- SRAM bits for MUX[65], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_65_ (mux_1level_tapbuf_size2_65_sram_blwl_out[65:65] ,mux_1level_tapbuf_size2_65_sram_blwl_out[65:65] ,mux_1level_tapbuf_size2_65_sram_blwl_outb[65:65] ,mux_1level_tapbuf_size2_65_configbus0[65:65], mux_1level_tapbuf_size2_65_configbus1[65:65] , mux_1level_tapbuf_size2_65_configbus0_b[65:65] );
wire [0:1] mux_1level_tapbuf_size2_66_inbus;
assign mux_1level_tapbuf_size2_66_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_66_inbus[1] = chany_0__1__in_31_ ;
wire [66:66] mux_1level_tapbuf_size2_66_configbus0;
wire [66:66] mux_1level_tapbuf_size2_66_configbus1;
wire [66:66] mux_1level_tapbuf_size2_66_sram_blwl_out ;
wire [66:66] mux_1level_tapbuf_size2_66_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_66_configbus0[66:66] = sram_blwl_bl[66:66] ;
assign mux_1level_tapbuf_size2_66_configbus1[66:66] = sram_blwl_wl[66:66] ;
wire [66:66] mux_1level_tapbuf_size2_66_configbus0_b;
assign mux_1level_tapbuf_size2_66_configbus0_b[66:66] = sram_blwl_blb[66:66] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_66_ (mux_1level_tapbuf_size2_66_inbus, chanx_1__0__out_32_ , mux_1level_tapbuf_size2_66_sram_blwl_out[66:66] ,
mux_1level_tapbuf_size2_66_sram_blwl_outb[66:66] );
//----- SRAM bits for MUX[66], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_66_ (mux_1level_tapbuf_size2_66_sram_blwl_out[66:66] ,mux_1level_tapbuf_size2_66_sram_blwl_out[66:66] ,mux_1level_tapbuf_size2_66_sram_blwl_outb[66:66] ,mux_1level_tapbuf_size2_66_configbus0[66:66], mux_1level_tapbuf_size2_66_configbus1[66:66] , mux_1level_tapbuf_size2_66_configbus0_b[66:66] );
wire [0:1] mux_1level_tapbuf_size2_67_inbus;
assign mux_1level_tapbuf_size2_67_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_67_inbus[1] = chany_0__1__in_33_ ;
wire [67:67] mux_1level_tapbuf_size2_67_configbus0;
wire [67:67] mux_1level_tapbuf_size2_67_configbus1;
wire [67:67] mux_1level_tapbuf_size2_67_sram_blwl_out ;
wire [67:67] mux_1level_tapbuf_size2_67_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_67_configbus0[67:67] = sram_blwl_bl[67:67] ;
assign mux_1level_tapbuf_size2_67_configbus1[67:67] = sram_blwl_wl[67:67] ;
wire [67:67] mux_1level_tapbuf_size2_67_configbus0_b;
assign mux_1level_tapbuf_size2_67_configbus0_b[67:67] = sram_blwl_blb[67:67] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_67_ (mux_1level_tapbuf_size2_67_inbus, chanx_1__0__out_34_ , mux_1level_tapbuf_size2_67_sram_blwl_out[67:67] ,
mux_1level_tapbuf_size2_67_sram_blwl_outb[67:67] );
//----- SRAM bits for MUX[67], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_67_ (mux_1level_tapbuf_size2_67_sram_blwl_out[67:67] ,mux_1level_tapbuf_size2_67_sram_blwl_out[67:67] ,mux_1level_tapbuf_size2_67_sram_blwl_outb[67:67] ,mux_1level_tapbuf_size2_67_configbus0[67:67], mux_1level_tapbuf_size2_67_configbus1[67:67] , mux_1level_tapbuf_size2_67_configbus0_b[67:67] );
wire [0:1] mux_1level_tapbuf_size2_68_inbus;
assign mux_1level_tapbuf_size2_68_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_68_inbus[1] = chany_0__1__in_35_ ;
wire [68:68] mux_1level_tapbuf_size2_68_configbus0;
wire [68:68] mux_1level_tapbuf_size2_68_configbus1;
wire [68:68] mux_1level_tapbuf_size2_68_sram_blwl_out ;
wire [68:68] mux_1level_tapbuf_size2_68_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_68_configbus0[68:68] = sram_blwl_bl[68:68] ;
assign mux_1level_tapbuf_size2_68_configbus1[68:68] = sram_blwl_wl[68:68] ;
wire [68:68] mux_1level_tapbuf_size2_68_configbus0_b;
assign mux_1level_tapbuf_size2_68_configbus0_b[68:68] = sram_blwl_blb[68:68] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_68_ (mux_1level_tapbuf_size2_68_inbus, chanx_1__0__out_36_ , mux_1level_tapbuf_size2_68_sram_blwl_out[68:68] ,
mux_1level_tapbuf_size2_68_sram_blwl_outb[68:68] );
//----- SRAM bits for MUX[68], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_68_ (mux_1level_tapbuf_size2_68_sram_blwl_out[68:68] ,mux_1level_tapbuf_size2_68_sram_blwl_out[68:68] ,mux_1level_tapbuf_size2_68_sram_blwl_outb[68:68] ,mux_1level_tapbuf_size2_68_configbus0[68:68], mux_1level_tapbuf_size2_68_configbus1[68:68] , mux_1level_tapbuf_size2_68_configbus0_b[68:68] );
wire [0:1] mux_1level_tapbuf_size2_69_inbus;
assign mux_1level_tapbuf_size2_69_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_69_inbus[1] = chany_0__1__in_37_ ;
wire [69:69] mux_1level_tapbuf_size2_69_configbus0;
wire [69:69] mux_1level_tapbuf_size2_69_configbus1;
wire [69:69] mux_1level_tapbuf_size2_69_sram_blwl_out ;
wire [69:69] mux_1level_tapbuf_size2_69_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_69_configbus0[69:69] = sram_blwl_bl[69:69] ;
assign mux_1level_tapbuf_size2_69_configbus1[69:69] = sram_blwl_wl[69:69] ;
wire [69:69] mux_1level_tapbuf_size2_69_configbus0_b;
assign mux_1level_tapbuf_size2_69_configbus0_b[69:69] = sram_blwl_blb[69:69] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_69_ (mux_1level_tapbuf_size2_69_inbus, chanx_1__0__out_38_ , mux_1level_tapbuf_size2_69_sram_blwl_out[69:69] ,
mux_1level_tapbuf_size2_69_sram_blwl_outb[69:69] );
//----- SRAM bits for MUX[69], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_69_ (mux_1level_tapbuf_size2_69_sram_blwl_out[69:69] ,mux_1level_tapbuf_size2_69_sram_blwl_out[69:69] ,mux_1level_tapbuf_size2_69_sram_blwl_outb[69:69] ,mux_1level_tapbuf_size2_69_configbus0[69:69], mux_1level_tapbuf_size2_69_configbus1[69:69] , mux_1level_tapbuf_size2_69_configbus0_b[69:69] );
wire [0:1] mux_1level_tapbuf_size2_70_inbus;
assign mux_1level_tapbuf_size2_70_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_70_inbus[1] = chany_0__1__in_39_ ;
wire [70:70] mux_1level_tapbuf_size2_70_configbus0;
wire [70:70] mux_1level_tapbuf_size2_70_configbus1;
wire [70:70] mux_1level_tapbuf_size2_70_sram_blwl_out ;
wire [70:70] mux_1level_tapbuf_size2_70_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_70_configbus0[70:70] = sram_blwl_bl[70:70] ;
assign mux_1level_tapbuf_size2_70_configbus1[70:70] = sram_blwl_wl[70:70] ;
wire [70:70] mux_1level_tapbuf_size2_70_configbus0_b;
assign mux_1level_tapbuf_size2_70_configbus0_b[70:70] = sram_blwl_blb[70:70] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_70_ (mux_1level_tapbuf_size2_70_inbus, chanx_1__0__out_40_ , mux_1level_tapbuf_size2_70_sram_blwl_out[70:70] ,
mux_1level_tapbuf_size2_70_sram_blwl_outb[70:70] );
//----- SRAM bits for MUX[70], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_70_ (mux_1level_tapbuf_size2_70_sram_blwl_out[70:70] ,mux_1level_tapbuf_size2_70_sram_blwl_out[70:70] ,mux_1level_tapbuf_size2_70_sram_blwl_outb[70:70] ,mux_1level_tapbuf_size2_70_configbus0[70:70], mux_1level_tapbuf_size2_70_configbus1[70:70] , mux_1level_tapbuf_size2_70_configbus0_b[70:70] );
wire [0:1] mux_1level_tapbuf_size2_71_inbus;
assign mux_1level_tapbuf_size2_71_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_71_inbus[1] = chany_0__1__in_41_ ;
wire [71:71] mux_1level_tapbuf_size2_71_configbus0;
wire [71:71] mux_1level_tapbuf_size2_71_configbus1;
wire [71:71] mux_1level_tapbuf_size2_71_sram_blwl_out ;
wire [71:71] mux_1level_tapbuf_size2_71_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_71_configbus0[71:71] = sram_blwl_bl[71:71] ;
assign mux_1level_tapbuf_size2_71_configbus1[71:71] = sram_blwl_wl[71:71] ;
wire [71:71] mux_1level_tapbuf_size2_71_configbus0_b;
assign mux_1level_tapbuf_size2_71_configbus0_b[71:71] = sram_blwl_blb[71:71] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_71_ (mux_1level_tapbuf_size2_71_inbus, chanx_1__0__out_42_ , mux_1level_tapbuf_size2_71_sram_blwl_out[71:71] ,
mux_1level_tapbuf_size2_71_sram_blwl_outb[71:71] );
//----- SRAM bits for MUX[71], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_71_ (mux_1level_tapbuf_size2_71_sram_blwl_out[71:71] ,mux_1level_tapbuf_size2_71_sram_blwl_out[71:71] ,mux_1level_tapbuf_size2_71_sram_blwl_outb[71:71] ,mux_1level_tapbuf_size2_71_configbus0[71:71], mux_1level_tapbuf_size2_71_configbus1[71:71] , mux_1level_tapbuf_size2_71_configbus0_b[71:71] );
wire [0:1] mux_1level_tapbuf_size2_72_inbus;
assign mux_1level_tapbuf_size2_72_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_72_inbus[1] = chany_0__1__in_43_ ;
wire [72:72] mux_1level_tapbuf_size2_72_configbus0;
wire [72:72] mux_1level_tapbuf_size2_72_configbus1;
wire [72:72] mux_1level_tapbuf_size2_72_sram_blwl_out ;
wire [72:72] mux_1level_tapbuf_size2_72_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_72_configbus0[72:72] = sram_blwl_bl[72:72] ;
assign mux_1level_tapbuf_size2_72_configbus1[72:72] = sram_blwl_wl[72:72] ;
wire [72:72] mux_1level_tapbuf_size2_72_configbus0_b;
assign mux_1level_tapbuf_size2_72_configbus0_b[72:72] = sram_blwl_blb[72:72] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_72_ (mux_1level_tapbuf_size2_72_inbus, chanx_1__0__out_44_ , mux_1level_tapbuf_size2_72_sram_blwl_out[72:72] ,
mux_1level_tapbuf_size2_72_sram_blwl_outb[72:72] );
//----- SRAM bits for MUX[72], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_72_ (mux_1level_tapbuf_size2_72_sram_blwl_out[72:72] ,mux_1level_tapbuf_size2_72_sram_blwl_out[72:72] ,mux_1level_tapbuf_size2_72_sram_blwl_outb[72:72] ,mux_1level_tapbuf_size2_72_configbus0[72:72], mux_1level_tapbuf_size2_72_configbus1[72:72] , mux_1level_tapbuf_size2_72_configbus0_b[72:72] );
wire [0:1] mux_1level_tapbuf_size2_73_inbus;
assign mux_1level_tapbuf_size2_73_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_73_inbus[1] = chany_0__1__in_45_ ;
wire [73:73] mux_1level_tapbuf_size2_73_configbus0;
wire [73:73] mux_1level_tapbuf_size2_73_configbus1;
wire [73:73] mux_1level_tapbuf_size2_73_sram_blwl_out ;
wire [73:73] mux_1level_tapbuf_size2_73_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_73_configbus0[73:73] = sram_blwl_bl[73:73] ;
assign mux_1level_tapbuf_size2_73_configbus1[73:73] = sram_blwl_wl[73:73] ;
wire [73:73] mux_1level_tapbuf_size2_73_configbus0_b;
assign mux_1level_tapbuf_size2_73_configbus0_b[73:73] = sram_blwl_blb[73:73] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_73_ (mux_1level_tapbuf_size2_73_inbus, chanx_1__0__out_46_ , mux_1level_tapbuf_size2_73_sram_blwl_out[73:73] ,
mux_1level_tapbuf_size2_73_sram_blwl_outb[73:73] );
//----- SRAM bits for MUX[73], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_73_ (mux_1level_tapbuf_size2_73_sram_blwl_out[73:73] ,mux_1level_tapbuf_size2_73_sram_blwl_out[73:73] ,mux_1level_tapbuf_size2_73_sram_blwl_outb[73:73] ,mux_1level_tapbuf_size2_73_configbus0[73:73], mux_1level_tapbuf_size2_73_configbus1[73:73] , mux_1level_tapbuf_size2_73_configbus0_b[73:73] );
wire [0:1] mux_1level_tapbuf_size2_74_inbus;
assign mux_1level_tapbuf_size2_74_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_74_inbus[1] = chany_0__1__in_47_ ;
wire [74:74] mux_1level_tapbuf_size2_74_configbus0;
wire [74:74] mux_1level_tapbuf_size2_74_configbus1;
wire [74:74] mux_1level_tapbuf_size2_74_sram_blwl_out ;
wire [74:74] mux_1level_tapbuf_size2_74_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_74_configbus0[74:74] = sram_blwl_bl[74:74] ;
assign mux_1level_tapbuf_size2_74_configbus1[74:74] = sram_blwl_wl[74:74] ;
wire [74:74] mux_1level_tapbuf_size2_74_configbus0_b;
assign mux_1level_tapbuf_size2_74_configbus0_b[74:74] = sram_blwl_blb[74:74] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_74_ (mux_1level_tapbuf_size2_74_inbus, chanx_1__0__out_48_ , mux_1level_tapbuf_size2_74_sram_blwl_out[74:74] ,
mux_1level_tapbuf_size2_74_sram_blwl_outb[74:74] );
//----- SRAM bits for MUX[74], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_74_ (mux_1level_tapbuf_size2_74_sram_blwl_out[74:74] ,mux_1level_tapbuf_size2_74_sram_blwl_out[74:74] ,mux_1level_tapbuf_size2_74_sram_blwl_outb[74:74] ,mux_1level_tapbuf_size2_74_configbus0[74:74], mux_1level_tapbuf_size2_74_configbus1[74:74] , mux_1level_tapbuf_size2_74_configbus0_b[74:74] );
wire [0:1] mux_1level_tapbuf_size2_75_inbus;
assign mux_1level_tapbuf_size2_75_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_75_inbus[1] = chany_0__1__in_49_ ;
wire [75:75] mux_1level_tapbuf_size2_75_configbus0;
wire [75:75] mux_1level_tapbuf_size2_75_configbus1;
wire [75:75] mux_1level_tapbuf_size2_75_sram_blwl_out ;
wire [75:75] mux_1level_tapbuf_size2_75_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_75_configbus0[75:75] = sram_blwl_bl[75:75] ;
assign mux_1level_tapbuf_size2_75_configbus1[75:75] = sram_blwl_wl[75:75] ;
wire [75:75] mux_1level_tapbuf_size2_75_configbus0_b;
assign mux_1level_tapbuf_size2_75_configbus0_b[75:75] = sram_blwl_blb[75:75] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_75_ (mux_1level_tapbuf_size2_75_inbus, chanx_1__0__out_50_ , mux_1level_tapbuf_size2_75_sram_blwl_out[75:75] ,
mux_1level_tapbuf_size2_75_sram_blwl_outb[75:75] );
//----- SRAM bits for MUX[75], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_75_ (mux_1level_tapbuf_size2_75_sram_blwl_out[75:75] ,mux_1level_tapbuf_size2_75_sram_blwl_out[75:75] ,mux_1level_tapbuf_size2_75_sram_blwl_outb[75:75] ,mux_1level_tapbuf_size2_75_configbus0[75:75], mux_1level_tapbuf_size2_75_configbus1[75:75] , mux_1level_tapbuf_size2_75_configbus0_b[75:75] );
wire [0:1] mux_1level_tapbuf_size2_76_inbus;
assign mux_1level_tapbuf_size2_76_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_76_inbus[1] = chany_0__1__in_51_ ;
wire [76:76] mux_1level_tapbuf_size2_76_configbus0;
wire [76:76] mux_1level_tapbuf_size2_76_configbus1;
wire [76:76] mux_1level_tapbuf_size2_76_sram_blwl_out ;
wire [76:76] mux_1level_tapbuf_size2_76_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_76_configbus0[76:76] = sram_blwl_bl[76:76] ;
assign mux_1level_tapbuf_size2_76_configbus1[76:76] = sram_blwl_wl[76:76] ;
wire [76:76] mux_1level_tapbuf_size2_76_configbus0_b;
assign mux_1level_tapbuf_size2_76_configbus0_b[76:76] = sram_blwl_blb[76:76] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_76_ (mux_1level_tapbuf_size2_76_inbus, chanx_1__0__out_52_ , mux_1level_tapbuf_size2_76_sram_blwl_out[76:76] ,
mux_1level_tapbuf_size2_76_sram_blwl_outb[76:76] );
//----- SRAM bits for MUX[76], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_76_ (mux_1level_tapbuf_size2_76_sram_blwl_out[76:76] ,mux_1level_tapbuf_size2_76_sram_blwl_out[76:76] ,mux_1level_tapbuf_size2_76_sram_blwl_outb[76:76] ,mux_1level_tapbuf_size2_76_configbus0[76:76], mux_1level_tapbuf_size2_76_configbus1[76:76] , mux_1level_tapbuf_size2_76_configbus0_b[76:76] );
wire [0:1] mux_1level_tapbuf_size2_77_inbus;
assign mux_1level_tapbuf_size2_77_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_77_inbus[1] = chany_0__1__in_53_ ;
wire [77:77] mux_1level_tapbuf_size2_77_configbus0;
wire [77:77] mux_1level_tapbuf_size2_77_configbus1;
wire [77:77] mux_1level_tapbuf_size2_77_sram_blwl_out ;
wire [77:77] mux_1level_tapbuf_size2_77_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_77_configbus0[77:77] = sram_blwl_bl[77:77] ;
assign mux_1level_tapbuf_size2_77_configbus1[77:77] = sram_blwl_wl[77:77] ;
wire [77:77] mux_1level_tapbuf_size2_77_configbus0_b;
assign mux_1level_tapbuf_size2_77_configbus0_b[77:77] = sram_blwl_blb[77:77] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_77_ (mux_1level_tapbuf_size2_77_inbus, chanx_1__0__out_54_ , mux_1level_tapbuf_size2_77_sram_blwl_out[77:77] ,
mux_1level_tapbuf_size2_77_sram_blwl_outb[77:77] );
//----- SRAM bits for MUX[77], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_77_ (mux_1level_tapbuf_size2_77_sram_blwl_out[77:77] ,mux_1level_tapbuf_size2_77_sram_blwl_out[77:77] ,mux_1level_tapbuf_size2_77_sram_blwl_outb[77:77] ,mux_1level_tapbuf_size2_77_configbus0[77:77], mux_1level_tapbuf_size2_77_configbus1[77:77] , mux_1level_tapbuf_size2_77_configbus0_b[77:77] );
wire [0:1] mux_1level_tapbuf_size2_78_inbus;
assign mux_1level_tapbuf_size2_78_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_78_inbus[1] = chany_0__1__in_55_ ;
wire [78:78] mux_1level_tapbuf_size2_78_configbus0;
wire [78:78] mux_1level_tapbuf_size2_78_configbus1;
wire [78:78] mux_1level_tapbuf_size2_78_sram_blwl_out ;
wire [78:78] mux_1level_tapbuf_size2_78_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_78_configbus0[78:78] = sram_blwl_bl[78:78] ;
assign mux_1level_tapbuf_size2_78_configbus1[78:78] = sram_blwl_wl[78:78] ;
wire [78:78] mux_1level_tapbuf_size2_78_configbus0_b;
assign mux_1level_tapbuf_size2_78_configbus0_b[78:78] = sram_blwl_blb[78:78] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_78_ (mux_1level_tapbuf_size2_78_inbus, chanx_1__0__out_56_ , mux_1level_tapbuf_size2_78_sram_blwl_out[78:78] ,
mux_1level_tapbuf_size2_78_sram_blwl_outb[78:78] );
//----- SRAM bits for MUX[78], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_78_ (mux_1level_tapbuf_size2_78_sram_blwl_out[78:78] ,mux_1level_tapbuf_size2_78_sram_blwl_out[78:78] ,mux_1level_tapbuf_size2_78_sram_blwl_outb[78:78] ,mux_1level_tapbuf_size2_78_configbus0[78:78], mux_1level_tapbuf_size2_78_configbus1[78:78] , mux_1level_tapbuf_size2_78_configbus0_b[78:78] );
wire [0:1] mux_1level_tapbuf_size2_79_inbus;
assign mux_1level_tapbuf_size2_79_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_79_inbus[1] = chany_0__1__in_57_ ;
wire [79:79] mux_1level_tapbuf_size2_79_configbus0;
wire [79:79] mux_1level_tapbuf_size2_79_configbus1;
wire [79:79] mux_1level_tapbuf_size2_79_sram_blwl_out ;
wire [79:79] mux_1level_tapbuf_size2_79_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_79_configbus0[79:79] = sram_blwl_bl[79:79] ;
assign mux_1level_tapbuf_size2_79_configbus1[79:79] = sram_blwl_wl[79:79] ;
wire [79:79] mux_1level_tapbuf_size2_79_configbus0_b;
assign mux_1level_tapbuf_size2_79_configbus0_b[79:79] = sram_blwl_blb[79:79] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_79_ (mux_1level_tapbuf_size2_79_inbus, chanx_1__0__out_58_ , mux_1level_tapbuf_size2_79_sram_blwl_out[79:79] ,
mux_1level_tapbuf_size2_79_sram_blwl_outb[79:79] );
//----- SRAM bits for MUX[79], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_79_ (mux_1level_tapbuf_size2_79_sram_blwl_out[79:79] ,mux_1level_tapbuf_size2_79_sram_blwl_out[79:79] ,mux_1level_tapbuf_size2_79_sram_blwl_outb[79:79] ,mux_1level_tapbuf_size2_79_configbus0[79:79], mux_1level_tapbuf_size2_79_configbus1[79:79] , mux_1level_tapbuf_size2_79_configbus0_b[79:79] );
wire [0:1] mux_1level_tapbuf_size2_80_inbus;
assign mux_1level_tapbuf_size2_80_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_80_inbus[1] = chany_0__1__in_59_ ;
wire [80:80] mux_1level_tapbuf_size2_80_configbus0;
wire [80:80] mux_1level_tapbuf_size2_80_configbus1;
wire [80:80] mux_1level_tapbuf_size2_80_sram_blwl_out ;
wire [80:80] mux_1level_tapbuf_size2_80_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_80_configbus0[80:80] = sram_blwl_bl[80:80] ;
assign mux_1level_tapbuf_size2_80_configbus1[80:80] = sram_blwl_wl[80:80] ;
wire [80:80] mux_1level_tapbuf_size2_80_configbus0_b;
assign mux_1level_tapbuf_size2_80_configbus0_b[80:80] = sram_blwl_blb[80:80] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_80_ (mux_1level_tapbuf_size2_80_inbus, chanx_1__0__out_60_ , mux_1level_tapbuf_size2_80_sram_blwl_out[80:80] ,
mux_1level_tapbuf_size2_80_sram_blwl_outb[80:80] );
//----- SRAM bits for MUX[80], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_80_ (mux_1level_tapbuf_size2_80_sram_blwl_out[80:80] ,mux_1level_tapbuf_size2_80_sram_blwl_out[80:80] ,mux_1level_tapbuf_size2_80_sram_blwl_outb[80:80] ,mux_1level_tapbuf_size2_80_configbus0[80:80], mux_1level_tapbuf_size2_80_configbus1[80:80] , mux_1level_tapbuf_size2_80_configbus0_b[80:80] );
wire [0:1] mux_1level_tapbuf_size2_81_inbus;
assign mux_1level_tapbuf_size2_81_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_81_inbus[1] = chany_0__1__in_61_ ;
wire [81:81] mux_1level_tapbuf_size2_81_configbus0;
wire [81:81] mux_1level_tapbuf_size2_81_configbus1;
wire [81:81] mux_1level_tapbuf_size2_81_sram_blwl_out ;
wire [81:81] mux_1level_tapbuf_size2_81_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_81_configbus0[81:81] = sram_blwl_bl[81:81] ;
assign mux_1level_tapbuf_size2_81_configbus1[81:81] = sram_blwl_wl[81:81] ;
wire [81:81] mux_1level_tapbuf_size2_81_configbus0_b;
assign mux_1level_tapbuf_size2_81_configbus0_b[81:81] = sram_blwl_blb[81:81] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_81_ (mux_1level_tapbuf_size2_81_inbus, chanx_1__0__out_62_ , mux_1level_tapbuf_size2_81_sram_blwl_out[81:81] ,
mux_1level_tapbuf_size2_81_sram_blwl_outb[81:81] );
//----- SRAM bits for MUX[81], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_81_ (mux_1level_tapbuf_size2_81_sram_blwl_out[81:81] ,mux_1level_tapbuf_size2_81_sram_blwl_out[81:81] ,mux_1level_tapbuf_size2_81_sram_blwl_outb[81:81] ,mux_1level_tapbuf_size2_81_configbus0[81:81], mux_1level_tapbuf_size2_81_configbus1[81:81] , mux_1level_tapbuf_size2_81_configbus0_b[81:81] );
wire [0:1] mux_1level_tapbuf_size2_82_inbus;
assign mux_1level_tapbuf_size2_82_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_82_inbus[1] = chany_0__1__in_63_ ;
wire [82:82] mux_1level_tapbuf_size2_82_configbus0;
wire [82:82] mux_1level_tapbuf_size2_82_configbus1;
wire [82:82] mux_1level_tapbuf_size2_82_sram_blwl_out ;
wire [82:82] mux_1level_tapbuf_size2_82_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_82_configbus0[82:82] = sram_blwl_bl[82:82] ;
assign mux_1level_tapbuf_size2_82_configbus1[82:82] = sram_blwl_wl[82:82] ;
wire [82:82] mux_1level_tapbuf_size2_82_configbus0_b;
assign mux_1level_tapbuf_size2_82_configbus0_b[82:82] = sram_blwl_blb[82:82] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_82_ (mux_1level_tapbuf_size2_82_inbus, chanx_1__0__out_64_ , mux_1level_tapbuf_size2_82_sram_blwl_out[82:82] ,
mux_1level_tapbuf_size2_82_sram_blwl_outb[82:82] );
//----- SRAM bits for MUX[82], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_82_ (mux_1level_tapbuf_size2_82_sram_blwl_out[82:82] ,mux_1level_tapbuf_size2_82_sram_blwl_out[82:82] ,mux_1level_tapbuf_size2_82_sram_blwl_outb[82:82] ,mux_1level_tapbuf_size2_82_configbus0[82:82], mux_1level_tapbuf_size2_82_configbus1[82:82] , mux_1level_tapbuf_size2_82_configbus0_b[82:82] );
wire [0:1] mux_1level_tapbuf_size2_83_inbus;
assign mux_1level_tapbuf_size2_83_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_83_inbus[1] = chany_0__1__in_65_ ;
wire [83:83] mux_1level_tapbuf_size2_83_configbus0;
wire [83:83] mux_1level_tapbuf_size2_83_configbus1;
wire [83:83] mux_1level_tapbuf_size2_83_sram_blwl_out ;
wire [83:83] mux_1level_tapbuf_size2_83_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_83_configbus0[83:83] = sram_blwl_bl[83:83] ;
assign mux_1level_tapbuf_size2_83_configbus1[83:83] = sram_blwl_wl[83:83] ;
wire [83:83] mux_1level_tapbuf_size2_83_configbus0_b;
assign mux_1level_tapbuf_size2_83_configbus0_b[83:83] = sram_blwl_blb[83:83] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_83_ (mux_1level_tapbuf_size2_83_inbus, chanx_1__0__out_66_ , mux_1level_tapbuf_size2_83_sram_blwl_out[83:83] ,
mux_1level_tapbuf_size2_83_sram_blwl_outb[83:83] );
//----- SRAM bits for MUX[83], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_83_ (mux_1level_tapbuf_size2_83_sram_blwl_out[83:83] ,mux_1level_tapbuf_size2_83_sram_blwl_out[83:83] ,mux_1level_tapbuf_size2_83_sram_blwl_outb[83:83] ,mux_1level_tapbuf_size2_83_configbus0[83:83], mux_1level_tapbuf_size2_83_configbus1[83:83] , mux_1level_tapbuf_size2_83_configbus0_b[83:83] );
wire [0:1] mux_1level_tapbuf_size2_84_inbus;
assign mux_1level_tapbuf_size2_84_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_84_inbus[1] = chany_0__1__in_67_ ;
wire [84:84] mux_1level_tapbuf_size2_84_configbus0;
wire [84:84] mux_1level_tapbuf_size2_84_configbus1;
wire [84:84] mux_1level_tapbuf_size2_84_sram_blwl_out ;
wire [84:84] mux_1level_tapbuf_size2_84_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_84_configbus0[84:84] = sram_blwl_bl[84:84] ;
assign mux_1level_tapbuf_size2_84_configbus1[84:84] = sram_blwl_wl[84:84] ;
wire [84:84] mux_1level_tapbuf_size2_84_configbus0_b;
assign mux_1level_tapbuf_size2_84_configbus0_b[84:84] = sram_blwl_blb[84:84] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_84_ (mux_1level_tapbuf_size2_84_inbus, chanx_1__0__out_68_ , mux_1level_tapbuf_size2_84_sram_blwl_out[84:84] ,
mux_1level_tapbuf_size2_84_sram_blwl_outb[84:84] );
//----- SRAM bits for MUX[84], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_84_ (mux_1level_tapbuf_size2_84_sram_blwl_out[84:84] ,mux_1level_tapbuf_size2_84_sram_blwl_out[84:84] ,mux_1level_tapbuf_size2_84_sram_blwl_outb[84:84] ,mux_1level_tapbuf_size2_84_configbus0[84:84], mux_1level_tapbuf_size2_84_configbus1[84:84] , mux_1level_tapbuf_size2_84_configbus0_b[84:84] );
wire [0:1] mux_1level_tapbuf_size2_85_inbus;
assign mux_1level_tapbuf_size2_85_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_85_inbus[1] = chany_0__1__in_69_ ;
wire [85:85] mux_1level_tapbuf_size2_85_configbus0;
wire [85:85] mux_1level_tapbuf_size2_85_configbus1;
wire [85:85] mux_1level_tapbuf_size2_85_sram_blwl_out ;
wire [85:85] mux_1level_tapbuf_size2_85_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_85_configbus0[85:85] = sram_blwl_bl[85:85] ;
assign mux_1level_tapbuf_size2_85_configbus1[85:85] = sram_blwl_wl[85:85] ;
wire [85:85] mux_1level_tapbuf_size2_85_configbus0_b;
assign mux_1level_tapbuf_size2_85_configbus0_b[85:85] = sram_blwl_blb[85:85] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_85_ (mux_1level_tapbuf_size2_85_inbus, chanx_1__0__out_70_ , mux_1level_tapbuf_size2_85_sram_blwl_out[85:85] ,
mux_1level_tapbuf_size2_85_sram_blwl_outb[85:85] );
//----- SRAM bits for MUX[85], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_85_ (mux_1level_tapbuf_size2_85_sram_blwl_out[85:85] ,mux_1level_tapbuf_size2_85_sram_blwl_out[85:85] ,mux_1level_tapbuf_size2_85_sram_blwl_outb[85:85] ,mux_1level_tapbuf_size2_85_configbus0[85:85], mux_1level_tapbuf_size2_85_configbus1[85:85] , mux_1level_tapbuf_size2_85_configbus0_b[85:85] );
wire [0:1] mux_1level_tapbuf_size2_86_inbus;
assign mux_1level_tapbuf_size2_86_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_86_inbus[1] = chany_0__1__in_71_ ;
wire [86:86] mux_1level_tapbuf_size2_86_configbus0;
wire [86:86] mux_1level_tapbuf_size2_86_configbus1;
wire [86:86] mux_1level_tapbuf_size2_86_sram_blwl_out ;
wire [86:86] mux_1level_tapbuf_size2_86_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_86_configbus0[86:86] = sram_blwl_bl[86:86] ;
assign mux_1level_tapbuf_size2_86_configbus1[86:86] = sram_blwl_wl[86:86] ;
wire [86:86] mux_1level_tapbuf_size2_86_configbus0_b;
assign mux_1level_tapbuf_size2_86_configbus0_b[86:86] = sram_blwl_blb[86:86] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_86_ (mux_1level_tapbuf_size2_86_inbus, chanx_1__0__out_72_ , mux_1level_tapbuf_size2_86_sram_blwl_out[86:86] ,
mux_1level_tapbuf_size2_86_sram_blwl_outb[86:86] );
//----- SRAM bits for MUX[86], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_86_ (mux_1level_tapbuf_size2_86_sram_blwl_out[86:86] ,mux_1level_tapbuf_size2_86_sram_blwl_out[86:86] ,mux_1level_tapbuf_size2_86_sram_blwl_outb[86:86] ,mux_1level_tapbuf_size2_86_configbus0[86:86], mux_1level_tapbuf_size2_86_configbus1[86:86] , mux_1level_tapbuf_size2_86_configbus0_b[86:86] );
wire [0:1] mux_1level_tapbuf_size2_87_inbus;
assign mux_1level_tapbuf_size2_87_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_87_inbus[1] = chany_0__1__in_73_ ;
wire [87:87] mux_1level_tapbuf_size2_87_configbus0;
wire [87:87] mux_1level_tapbuf_size2_87_configbus1;
wire [87:87] mux_1level_tapbuf_size2_87_sram_blwl_out ;
wire [87:87] mux_1level_tapbuf_size2_87_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_87_configbus0[87:87] = sram_blwl_bl[87:87] ;
assign mux_1level_tapbuf_size2_87_configbus1[87:87] = sram_blwl_wl[87:87] ;
wire [87:87] mux_1level_tapbuf_size2_87_configbus0_b;
assign mux_1level_tapbuf_size2_87_configbus0_b[87:87] = sram_blwl_blb[87:87] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_87_ (mux_1level_tapbuf_size2_87_inbus, chanx_1__0__out_74_ , mux_1level_tapbuf_size2_87_sram_blwl_out[87:87] ,
mux_1level_tapbuf_size2_87_sram_blwl_outb[87:87] );
//----- SRAM bits for MUX[87], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_87_ (mux_1level_tapbuf_size2_87_sram_blwl_out[87:87] ,mux_1level_tapbuf_size2_87_sram_blwl_out[87:87] ,mux_1level_tapbuf_size2_87_sram_blwl_outb[87:87] ,mux_1level_tapbuf_size2_87_configbus0[87:87], mux_1level_tapbuf_size2_87_configbus1[87:87] , mux_1level_tapbuf_size2_87_configbus0_b[87:87] );
wire [0:1] mux_1level_tapbuf_size2_88_inbus;
assign mux_1level_tapbuf_size2_88_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_88_inbus[1] = chany_0__1__in_75_ ;
wire [88:88] mux_1level_tapbuf_size2_88_configbus0;
wire [88:88] mux_1level_tapbuf_size2_88_configbus1;
wire [88:88] mux_1level_tapbuf_size2_88_sram_blwl_out ;
wire [88:88] mux_1level_tapbuf_size2_88_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_88_configbus0[88:88] = sram_blwl_bl[88:88] ;
assign mux_1level_tapbuf_size2_88_configbus1[88:88] = sram_blwl_wl[88:88] ;
wire [88:88] mux_1level_tapbuf_size2_88_configbus0_b;
assign mux_1level_tapbuf_size2_88_configbus0_b[88:88] = sram_blwl_blb[88:88] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_88_ (mux_1level_tapbuf_size2_88_inbus, chanx_1__0__out_76_ , mux_1level_tapbuf_size2_88_sram_blwl_out[88:88] ,
mux_1level_tapbuf_size2_88_sram_blwl_outb[88:88] );
//----- SRAM bits for MUX[88], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_88_ (mux_1level_tapbuf_size2_88_sram_blwl_out[88:88] ,mux_1level_tapbuf_size2_88_sram_blwl_out[88:88] ,mux_1level_tapbuf_size2_88_sram_blwl_outb[88:88] ,mux_1level_tapbuf_size2_88_configbus0[88:88], mux_1level_tapbuf_size2_88_configbus1[88:88] , mux_1level_tapbuf_size2_88_configbus0_b[88:88] );
wire [0:1] mux_1level_tapbuf_size2_89_inbus;
assign mux_1level_tapbuf_size2_89_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_89_inbus[1] = chany_0__1__in_77_ ;
wire [89:89] mux_1level_tapbuf_size2_89_configbus0;
wire [89:89] mux_1level_tapbuf_size2_89_configbus1;
wire [89:89] mux_1level_tapbuf_size2_89_sram_blwl_out ;
wire [89:89] mux_1level_tapbuf_size2_89_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_89_configbus0[89:89] = sram_blwl_bl[89:89] ;
assign mux_1level_tapbuf_size2_89_configbus1[89:89] = sram_blwl_wl[89:89] ;
wire [89:89] mux_1level_tapbuf_size2_89_configbus0_b;
assign mux_1level_tapbuf_size2_89_configbus0_b[89:89] = sram_blwl_blb[89:89] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_89_ (mux_1level_tapbuf_size2_89_inbus, chanx_1__0__out_78_ , mux_1level_tapbuf_size2_89_sram_blwl_out[89:89] ,
mux_1level_tapbuf_size2_89_sram_blwl_outb[89:89] );
//----- SRAM bits for MUX[89], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_89_ (mux_1level_tapbuf_size2_89_sram_blwl_out[89:89] ,mux_1level_tapbuf_size2_89_sram_blwl_out[89:89] ,mux_1level_tapbuf_size2_89_sram_blwl_outb[89:89] ,mux_1level_tapbuf_size2_89_configbus0[89:89], mux_1level_tapbuf_size2_89_configbus1[89:89] , mux_1level_tapbuf_size2_89_configbus0_b[89:89] );
wire [0:1] mux_1level_tapbuf_size2_90_inbus;
assign mux_1level_tapbuf_size2_90_inbus[0] =  grid_1__1__pin_0__2__42_;
assign mux_1level_tapbuf_size2_90_inbus[1] = chany_0__1__in_79_ ;
wire [90:90] mux_1level_tapbuf_size2_90_configbus0;
wire [90:90] mux_1level_tapbuf_size2_90_configbus1;
wire [90:90] mux_1level_tapbuf_size2_90_sram_blwl_out ;
wire [90:90] mux_1level_tapbuf_size2_90_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_90_configbus0[90:90] = sram_blwl_bl[90:90] ;
assign mux_1level_tapbuf_size2_90_configbus1[90:90] = sram_blwl_wl[90:90] ;
wire [90:90] mux_1level_tapbuf_size2_90_configbus0_b;
assign mux_1level_tapbuf_size2_90_configbus0_b[90:90] = sram_blwl_blb[90:90] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_90_ (mux_1level_tapbuf_size2_90_inbus, chanx_1__0__out_80_ , mux_1level_tapbuf_size2_90_sram_blwl_out[90:90] ,
mux_1level_tapbuf_size2_90_sram_blwl_outb[90:90] );
//----- SRAM bits for MUX[90], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_90_ (mux_1level_tapbuf_size2_90_sram_blwl_out[90:90] ,mux_1level_tapbuf_size2_90_sram_blwl_out[90:90] ,mux_1level_tapbuf_size2_90_sram_blwl_outb[90:90] ,mux_1level_tapbuf_size2_90_configbus0[90:90], mux_1level_tapbuf_size2_90_configbus1[90:90] , mux_1level_tapbuf_size2_90_configbus0_b[90:90] );
wire [0:1] mux_1level_tapbuf_size2_91_inbus;
assign mux_1level_tapbuf_size2_91_inbus[0] =  grid_1__1__pin_0__2__42_;
assign mux_1level_tapbuf_size2_91_inbus[1] = chany_0__1__in_81_ ;
wire [91:91] mux_1level_tapbuf_size2_91_configbus0;
wire [91:91] mux_1level_tapbuf_size2_91_configbus1;
wire [91:91] mux_1level_tapbuf_size2_91_sram_blwl_out ;
wire [91:91] mux_1level_tapbuf_size2_91_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_91_configbus0[91:91] = sram_blwl_bl[91:91] ;
assign mux_1level_tapbuf_size2_91_configbus1[91:91] = sram_blwl_wl[91:91] ;
wire [91:91] mux_1level_tapbuf_size2_91_configbus0_b;
assign mux_1level_tapbuf_size2_91_configbus0_b[91:91] = sram_blwl_blb[91:91] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_91_ (mux_1level_tapbuf_size2_91_inbus, chanx_1__0__out_82_ , mux_1level_tapbuf_size2_91_sram_blwl_out[91:91] ,
mux_1level_tapbuf_size2_91_sram_blwl_outb[91:91] );
//----- SRAM bits for MUX[91], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_91_ (mux_1level_tapbuf_size2_91_sram_blwl_out[91:91] ,mux_1level_tapbuf_size2_91_sram_blwl_out[91:91] ,mux_1level_tapbuf_size2_91_sram_blwl_outb[91:91] ,mux_1level_tapbuf_size2_91_configbus0[91:91], mux_1level_tapbuf_size2_91_configbus1[91:91] , mux_1level_tapbuf_size2_91_configbus0_b[91:91] );
wire [0:1] mux_1level_tapbuf_size2_92_inbus;
assign mux_1level_tapbuf_size2_92_inbus[0] =  grid_1__1__pin_0__2__42_;
assign mux_1level_tapbuf_size2_92_inbus[1] = chany_0__1__in_83_ ;
wire [92:92] mux_1level_tapbuf_size2_92_configbus0;
wire [92:92] mux_1level_tapbuf_size2_92_configbus1;
wire [92:92] mux_1level_tapbuf_size2_92_sram_blwl_out ;
wire [92:92] mux_1level_tapbuf_size2_92_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_92_configbus0[92:92] = sram_blwl_bl[92:92] ;
assign mux_1level_tapbuf_size2_92_configbus1[92:92] = sram_blwl_wl[92:92] ;
wire [92:92] mux_1level_tapbuf_size2_92_configbus0_b;
assign mux_1level_tapbuf_size2_92_configbus0_b[92:92] = sram_blwl_blb[92:92] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_92_ (mux_1level_tapbuf_size2_92_inbus, chanx_1__0__out_84_ , mux_1level_tapbuf_size2_92_sram_blwl_out[92:92] ,
mux_1level_tapbuf_size2_92_sram_blwl_outb[92:92] );
//----- SRAM bits for MUX[92], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_92_ (mux_1level_tapbuf_size2_92_sram_blwl_out[92:92] ,mux_1level_tapbuf_size2_92_sram_blwl_out[92:92] ,mux_1level_tapbuf_size2_92_sram_blwl_outb[92:92] ,mux_1level_tapbuf_size2_92_configbus0[92:92], mux_1level_tapbuf_size2_92_configbus1[92:92] , mux_1level_tapbuf_size2_92_configbus0_b[92:92] );
wire [0:1] mux_1level_tapbuf_size2_93_inbus;
assign mux_1level_tapbuf_size2_93_inbus[0] =  grid_1__1__pin_0__2__42_;
assign mux_1level_tapbuf_size2_93_inbus[1] = chany_0__1__in_85_ ;
wire [93:93] mux_1level_tapbuf_size2_93_configbus0;
wire [93:93] mux_1level_tapbuf_size2_93_configbus1;
wire [93:93] mux_1level_tapbuf_size2_93_sram_blwl_out ;
wire [93:93] mux_1level_tapbuf_size2_93_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_93_configbus0[93:93] = sram_blwl_bl[93:93] ;
assign mux_1level_tapbuf_size2_93_configbus1[93:93] = sram_blwl_wl[93:93] ;
wire [93:93] mux_1level_tapbuf_size2_93_configbus0_b;
assign mux_1level_tapbuf_size2_93_configbus0_b[93:93] = sram_blwl_blb[93:93] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_93_ (mux_1level_tapbuf_size2_93_inbus, chanx_1__0__out_86_ , mux_1level_tapbuf_size2_93_sram_blwl_out[93:93] ,
mux_1level_tapbuf_size2_93_sram_blwl_outb[93:93] );
//----- SRAM bits for MUX[93], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_93_ (mux_1level_tapbuf_size2_93_sram_blwl_out[93:93] ,mux_1level_tapbuf_size2_93_sram_blwl_out[93:93] ,mux_1level_tapbuf_size2_93_sram_blwl_outb[93:93] ,mux_1level_tapbuf_size2_93_configbus0[93:93], mux_1level_tapbuf_size2_93_configbus1[93:93] , mux_1level_tapbuf_size2_93_configbus0_b[93:93] );
wire [0:1] mux_1level_tapbuf_size2_94_inbus;
assign mux_1level_tapbuf_size2_94_inbus[0] =  grid_1__1__pin_0__2__42_;
assign mux_1level_tapbuf_size2_94_inbus[1] = chany_0__1__in_87_ ;
wire [94:94] mux_1level_tapbuf_size2_94_configbus0;
wire [94:94] mux_1level_tapbuf_size2_94_configbus1;
wire [94:94] mux_1level_tapbuf_size2_94_sram_blwl_out ;
wire [94:94] mux_1level_tapbuf_size2_94_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_94_configbus0[94:94] = sram_blwl_bl[94:94] ;
assign mux_1level_tapbuf_size2_94_configbus1[94:94] = sram_blwl_wl[94:94] ;
wire [94:94] mux_1level_tapbuf_size2_94_configbus0_b;
assign mux_1level_tapbuf_size2_94_configbus0_b[94:94] = sram_blwl_blb[94:94] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_94_ (mux_1level_tapbuf_size2_94_inbus, chanx_1__0__out_88_ , mux_1level_tapbuf_size2_94_sram_blwl_out[94:94] ,
mux_1level_tapbuf_size2_94_sram_blwl_outb[94:94] );
//----- SRAM bits for MUX[94], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_94_ (mux_1level_tapbuf_size2_94_sram_blwl_out[94:94] ,mux_1level_tapbuf_size2_94_sram_blwl_out[94:94] ,mux_1level_tapbuf_size2_94_sram_blwl_outb[94:94] ,mux_1level_tapbuf_size2_94_configbus0[94:94], mux_1level_tapbuf_size2_94_configbus1[94:94] , mux_1level_tapbuf_size2_94_configbus0_b[94:94] );
wire [0:1] mux_1level_tapbuf_size2_95_inbus;
assign mux_1level_tapbuf_size2_95_inbus[0] =  grid_1__1__pin_0__2__46_;
assign mux_1level_tapbuf_size2_95_inbus[1] = chany_0__1__in_89_ ;
wire [95:95] mux_1level_tapbuf_size2_95_configbus0;
wire [95:95] mux_1level_tapbuf_size2_95_configbus1;
wire [95:95] mux_1level_tapbuf_size2_95_sram_blwl_out ;
wire [95:95] mux_1level_tapbuf_size2_95_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_95_configbus0[95:95] = sram_blwl_bl[95:95] ;
assign mux_1level_tapbuf_size2_95_configbus1[95:95] = sram_blwl_wl[95:95] ;
wire [95:95] mux_1level_tapbuf_size2_95_configbus0_b;
assign mux_1level_tapbuf_size2_95_configbus0_b[95:95] = sram_blwl_blb[95:95] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_95_ (mux_1level_tapbuf_size2_95_inbus, chanx_1__0__out_90_ , mux_1level_tapbuf_size2_95_sram_blwl_out[95:95] ,
mux_1level_tapbuf_size2_95_sram_blwl_outb[95:95] );
//----- SRAM bits for MUX[95], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_95_ (mux_1level_tapbuf_size2_95_sram_blwl_out[95:95] ,mux_1level_tapbuf_size2_95_sram_blwl_out[95:95] ,mux_1level_tapbuf_size2_95_sram_blwl_outb[95:95] ,mux_1level_tapbuf_size2_95_configbus0[95:95], mux_1level_tapbuf_size2_95_configbus1[95:95] , mux_1level_tapbuf_size2_95_configbus0_b[95:95] );
wire [0:1] mux_1level_tapbuf_size2_96_inbus;
assign mux_1level_tapbuf_size2_96_inbus[0] =  grid_1__1__pin_0__2__46_;
assign mux_1level_tapbuf_size2_96_inbus[1] = chany_0__1__in_91_ ;
wire [96:96] mux_1level_tapbuf_size2_96_configbus0;
wire [96:96] mux_1level_tapbuf_size2_96_configbus1;
wire [96:96] mux_1level_tapbuf_size2_96_sram_blwl_out ;
wire [96:96] mux_1level_tapbuf_size2_96_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_96_configbus0[96:96] = sram_blwl_bl[96:96] ;
assign mux_1level_tapbuf_size2_96_configbus1[96:96] = sram_blwl_wl[96:96] ;
wire [96:96] mux_1level_tapbuf_size2_96_configbus0_b;
assign mux_1level_tapbuf_size2_96_configbus0_b[96:96] = sram_blwl_blb[96:96] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_96_ (mux_1level_tapbuf_size2_96_inbus, chanx_1__0__out_92_ , mux_1level_tapbuf_size2_96_sram_blwl_out[96:96] ,
mux_1level_tapbuf_size2_96_sram_blwl_outb[96:96] );
//----- SRAM bits for MUX[96], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_96_ (mux_1level_tapbuf_size2_96_sram_blwl_out[96:96] ,mux_1level_tapbuf_size2_96_sram_blwl_out[96:96] ,mux_1level_tapbuf_size2_96_sram_blwl_outb[96:96] ,mux_1level_tapbuf_size2_96_configbus0[96:96], mux_1level_tapbuf_size2_96_configbus1[96:96] , mux_1level_tapbuf_size2_96_configbus0_b[96:96] );
wire [0:1] mux_1level_tapbuf_size2_97_inbus;
assign mux_1level_tapbuf_size2_97_inbus[0] =  grid_1__1__pin_0__2__46_;
assign mux_1level_tapbuf_size2_97_inbus[1] = chany_0__1__in_93_ ;
wire [97:97] mux_1level_tapbuf_size2_97_configbus0;
wire [97:97] mux_1level_tapbuf_size2_97_configbus1;
wire [97:97] mux_1level_tapbuf_size2_97_sram_blwl_out ;
wire [97:97] mux_1level_tapbuf_size2_97_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_97_configbus0[97:97] = sram_blwl_bl[97:97] ;
assign mux_1level_tapbuf_size2_97_configbus1[97:97] = sram_blwl_wl[97:97] ;
wire [97:97] mux_1level_tapbuf_size2_97_configbus0_b;
assign mux_1level_tapbuf_size2_97_configbus0_b[97:97] = sram_blwl_blb[97:97] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_97_ (mux_1level_tapbuf_size2_97_inbus, chanx_1__0__out_94_ , mux_1level_tapbuf_size2_97_sram_blwl_out[97:97] ,
mux_1level_tapbuf_size2_97_sram_blwl_outb[97:97] );
//----- SRAM bits for MUX[97], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_97_ (mux_1level_tapbuf_size2_97_sram_blwl_out[97:97] ,mux_1level_tapbuf_size2_97_sram_blwl_out[97:97] ,mux_1level_tapbuf_size2_97_sram_blwl_outb[97:97] ,mux_1level_tapbuf_size2_97_configbus0[97:97], mux_1level_tapbuf_size2_97_configbus1[97:97] , mux_1level_tapbuf_size2_97_configbus0_b[97:97] );
wire [0:1] mux_1level_tapbuf_size2_98_inbus;
assign mux_1level_tapbuf_size2_98_inbus[0] =  grid_1__1__pin_0__2__46_;
assign mux_1level_tapbuf_size2_98_inbus[1] = chany_0__1__in_95_ ;
wire [98:98] mux_1level_tapbuf_size2_98_configbus0;
wire [98:98] mux_1level_tapbuf_size2_98_configbus1;
wire [98:98] mux_1level_tapbuf_size2_98_sram_blwl_out ;
wire [98:98] mux_1level_tapbuf_size2_98_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_98_configbus0[98:98] = sram_blwl_bl[98:98] ;
assign mux_1level_tapbuf_size2_98_configbus1[98:98] = sram_blwl_wl[98:98] ;
wire [98:98] mux_1level_tapbuf_size2_98_configbus0_b;
assign mux_1level_tapbuf_size2_98_configbus0_b[98:98] = sram_blwl_blb[98:98] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_98_ (mux_1level_tapbuf_size2_98_inbus, chanx_1__0__out_96_ , mux_1level_tapbuf_size2_98_sram_blwl_out[98:98] ,
mux_1level_tapbuf_size2_98_sram_blwl_outb[98:98] );
//----- SRAM bits for MUX[98], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_98_ (mux_1level_tapbuf_size2_98_sram_blwl_out[98:98] ,mux_1level_tapbuf_size2_98_sram_blwl_out[98:98] ,mux_1level_tapbuf_size2_98_sram_blwl_outb[98:98] ,mux_1level_tapbuf_size2_98_configbus0[98:98], mux_1level_tapbuf_size2_98_configbus1[98:98] , mux_1level_tapbuf_size2_98_configbus0_b[98:98] );
wire [0:1] mux_1level_tapbuf_size2_99_inbus;
assign mux_1level_tapbuf_size2_99_inbus[0] =  grid_1__1__pin_0__2__46_;
assign mux_1level_tapbuf_size2_99_inbus[1] = chany_0__1__in_97_ ;
wire [99:99] mux_1level_tapbuf_size2_99_configbus0;
wire [99:99] mux_1level_tapbuf_size2_99_configbus1;
wire [99:99] mux_1level_tapbuf_size2_99_sram_blwl_out ;
wire [99:99] mux_1level_tapbuf_size2_99_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_99_configbus0[99:99] = sram_blwl_bl[99:99] ;
assign mux_1level_tapbuf_size2_99_configbus1[99:99] = sram_blwl_wl[99:99] ;
wire [99:99] mux_1level_tapbuf_size2_99_configbus0_b;
assign mux_1level_tapbuf_size2_99_configbus0_b[99:99] = sram_blwl_blb[99:99] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_99_ (mux_1level_tapbuf_size2_99_inbus, chanx_1__0__out_98_ , mux_1level_tapbuf_size2_99_sram_blwl_out[99:99] ,
mux_1level_tapbuf_size2_99_sram_blwl_outb[99:99] );
//----- SRAM bits for MUX[99], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_99_ (mux_1level_tapbuf_size2_99_sram_blwl_out[99:99] ,mux_1level_tapbuf_size2_99_sram_blwl_out[99:99] ,mux_1level_tapbuf_size2_99_sram_blwl_outb[99:99] ,mux_1level_tapbuf_size2_99_configbus0[99:99], mux_1level_tapbuf_size2_99_configbus1[99:99] , mux_1level_tapbuf_size2_99_configbus0_b[99:99] );
//----- bottom side Multiplexers -----
//----- left side Multiplexers -----
endmodule
//----- END Verilog Module of Switch Box[0][0] -----

