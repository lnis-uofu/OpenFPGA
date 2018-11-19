//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Switch Block  [1][0] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
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
  output chany_1__1__out_30_,
  input chany_1__1__in_31_,
  output chany_1__1__out_32_,
  input chany_1__1__in_33_,
  output chany_1__1__out_34_,
  input chany_1__1__in_35_,
  output chany_1__1__out_36_,
  input chany_1__1__in_37_,
  output chany_1__1__out_38_,
  input chany_1__1__in_39_,
  output chany_1__1__out_40_,
  input chany_1__1__in_41_,
  output chany_1__1__out_42_,
  input chany_1__1__in_43_,
  output chany_1__1__out_44_,
  input chany_1__1__in_45_,
  output chany_1__1__out_46_,
  input chany_1__1__in_47_,
  output chany_1__1__out_48_,
  input chany_1__1__in_49_,
  output chany_1__1__out_50_,
  input chany_1__1__in_51_,
  output chany_1__1__out_52_,
  input chany_1__1__in_53_,
  output chany_1__1__out_54_,
  input chany_1__1__in_55_,
  output chany_1__1__out_56_,
  input chany_1__1__in_57_,
  output chany_1__1__out_58_,
  input chany_1__1__in_59_,
  output chany_1__1__out_60_,
  input chany_1__1__in_61_,
  output chany_1__1__out_62_,
  input chany_1__1__in_63_,
  output chany_1__1__out_64_,
  input chany_1__1__in_65_,
  output chany_1__1__out_66_,
  input chany_1__1__in_67_,
  output chany_1__1__out_68_,
  input chany_1__1__in_69_,
  output chany_1__1__out_70_,
  input chany_1__1__in_71_,
  output chany_1__1__out_72_,
  input chany_1__1__in_73_,
  output chany_1__1__out_74_,
  input chany_1__1__in_75_,
  output chany_1__1__out_76_,
  input chany_1__1__in_77_,
  output chany_1__1__out_78_,
  input chany_1__1__in_79_,
  output chany_1__1__out_80_,
  input chany_1__1__in_81_,
  output chany_1__1__out_82_,
  input chany_1__1__in_83_,
  output chany_1__1__out_84_,
  input chany_1__1__in_85_,
  output chany_1__1__out_86_,
  input chany_1__1__in_87_,
  output chany_1__1__out_88_,
  input chany_1__1__in_89_,
  output chany_1__1__out_90_,
  input chany_1__1__in_91_,
  output chany_1__1__out_92_,
  input chany_1__1__in_93_,
  output chany_1__1__out_94_,
  input chany_1__1__in_95_,
  output chany_1__1__out_96_,
  input chany_1__1__in_97_,
  output chany_1__1__out_98_,
  input chany_1__1__in_99_,
input  grid_1__1__pin_0__1__41_,
input  grid_1__1__pin_0__1__45_,
input  grid_1__1__pin_0__1__49_,
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
  input chanx_1__0__in_30_,
  output chanx_1__0__out_31_,
  input chanx_1__0__in_32_,
  output chanx_1__0__out_33_,
  input chanx_1__0__in_34_,
  output chanx_1__0__out_35_,
  input chanx_1__0__in_36_,
  output chanx_1__0__out_37_,
  input chanx_1__0__in_38_,
  output chanx_1__0__out_39_,
  input chanx_1__0__in_40_,
  output chanx_1__0__out_41_,
  input chanx_1__0__in_42_,
  output chanx_1__0__out_43_,
  input chanx_1__0__in_44_,
  output chanx_1__0__out_45_,
  input chanx_1__0__in_46_,
  output chanx_1__0__out_47_,
  input chanx_1__0__in_48_,
  output chanx_1__0__out_49_,
  input chanx_1__0__in_50_,
  output chanx_1__0__out_51_,
  input chanx_1__0__in_52_,
  output chanx_1__0__out_53_,
  input chanx_1__0__in_54_,
  output chanx_1__0__out_55_,
  input chanx_1__0__in_56_,
  output chanx_1__0__out_57_,
  input chanx_1__0__in_58_,
  output chanx_1__0__out_59_,
  input chanx_1__0__in_60_,
  output chanx_1__0__out_61_,
  input chanx_1__0__in_62_,
  output chanx_1__0__out_63_,
  input chanx_1__0__in_64_,
  output chanx_1__0__out_65_,
  input chanx_1__0__in_66_,
  output chanx_1__0__out_67_,
  input chanx_1__0__in_68_,
  output chanx_1__0__out_69_,
  input chanx_1__0__in_70_,
  output chanx_1__0__out_71_,
  input chanx_1__0__in_72_,
  output chanx_1__0__out_73_,
  input chanx_1__0__in_74_,
  output chanx_1__0__out_75_,
  input chanx_1__0__in_76_,
  output chanx_1__0__out_77_,
  input chanx_1__0__in_78_,
  output chanx_1__0__out_79_,
  input chanx_1__0__in_80_,
  output chanx_1__0__out_81_,
  input chanx_1__0__in_82_,
  output chanx_1__0__out_83_,
  input chanx_1__0__in_84_,
  output chanx_1__0__out_85_,
  input chanx_1__0__in_86_,
  output chanx_1__0__out_87_,
  input chanx_1__0__in_88_,
  output chanx_1__0__out_89_,
  input chanx_1__0__in_90_,
  output chanx_1__0__out_91_,
  input chanx_1__0__in_92_,
  output chanx_1__0__out_93_,
  input chanx_1__0__in_94_,
  output chanx_1__0__out_95_,
  input chanx_1__0__in_96_,
  output chanx_1__0__out_97_,
  input chanx_1__0__in_98_,
  output chanx_1__0__out_99_,
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
input [210:319] sram_blwl_bl ,
input [210:319] sram_blwl_wl ,
input [210:319] sram_blwl_blb ); 
//----- top side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_200_inbus;
assign mux_1level_tapbuf_size3_200_inbus[0] =  grid_1__1__pin_0__1__41_;
assign mux_1level_tapbuf_size3_200_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_200_inbus[2] = chanx_1__0__in_0_ ;
wire [210:212] mux_1level_tapbuf_size3_200_configbus0;
wire [210:212] mux_1level_tapbuf_size3_200_configbus1;
wire [210:212] mux_1level_tapbuf_size3_200_sram_blwl_out ;
wire [210:212] mux_1level_tapbuf_size3_200_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_200_configbus0[210:212] = sram_blwl_bl[210:212] ;
assign mux_1level_tapbuf_size3_200_configbus1[210:212] = sram_blwl_wl[210:212] ;
wire [210:212] mux_1level_tapbuf_size3_200_configbus0_b;
assign mux_1level_tapbuf_size3_200_configbus0_b[210:212] = sram_blwl_blb[210:212] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_200_ (mux_1level_tapbuf_size3_200_inbus, chany_1__1__out_0_ , mux_1level_tapbuf_size3_200_sram_blwl_out[210:212] ,
mux_1level_tapbuf_size3_200_sram_blwl_outb[210:212] );
//----- SRAM bits for MUX[200], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_210_ (mux_1level_tapbuf_size3_200_sram_blwl_out[210:210] ,mux_1level_tapbuf_size3_200_sram_blwl_out[210:210] ,mux_1level_tapbuf_size3_200_sram_blwl_outb[210:210] ,mux_1level_tapbuf_size3_200_configbus0[210:210], mux_1level_tapbuf_size3_200_configbus1[210:210] , mux_1level_tapbuf_size3_200_configbus0_b[210:210] );
sram6T_blwl sram_blwl_211_ (mux_1level_tapbuf_size3_200_sram_blwl_out[211:211] ,mux_1level_tapbuf_size3_200_sram_blwl_out[211:211] ,mux_1level_tapbuf_size3_200_sram_blwl_outb[211:211] ,mux_1level_tapbuf_size3_200_configbus0[211:211], mux_1level_tapbuf_size3_200_configbus1[211:211] , mux_1level_tapbuf_size3_200_configbus0_b[211:211] );
sram6T_blwl sram_blwl_212_ (mux_1level_tapbuf_size3_200_sram_blwl_out[212:212] ,mux_1level_tapbuf_size3_200_sram_blwl_out[212:212] ,mux_1level_tapbuf_size3_200_sram_blwl_outb[212:212] ,mux_1level_tapbuf_size3_200_configbus0[212:212], mux_1level_tapbuf_size3_200_configbus1[212:212] , mux_1level_tapbuf_size3_200_configbus0_b[212:212] );
wire [0:2] mux_1level_tapbuf_size3_201_inbus;
assign mux_1level_tapbuf_size3_201_inbus[0] =  grid_1__1__pin_0__1__41_;
assign mux_1level_tapbuf_size3_201_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_201_inbus[2] = chanx_1__0__in_98_ ;
wire [213:215] mux_1level_tapbuf_size3_201_configbus0;
wire [213:215] mux_1level_tapbuf_size3_201_configbus1;
wire [213:215] mux_1level_tapbuf_size3_201_sram_blwl_out ;
wire [213:215] mux_1level_tapbuf_size3_201_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_201_configbus0[213:215] = sram_blwl_bl[213:215] ;
assign mux_1level_tapbuf_size3_201_configbus1[213:215] = sram_blwl_wl[213:215] ;
wire [213:215] mux_1level_tapbuf_size3_201_configbus0_b;
assign mux_1level_tapbuf_size3_201_configbus0_b[213:215] = sram_blwl_blb[213:215] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_201_ (mux_1level_tapbuf_size3_201_inbus, chany_1__1__out_2_ , mux_1level_tapbuf_size3_201_sram_blwl_out[213:215] ,
mux_1level_tapbuf_size3_201_sram_blwl_outb[213:215] );
//----- SRAM bits for MUX[201], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_213_ (mux_1level_tapbuf_size3_201_sram_blwl_out[213:213] ,mux_1level_tapbuf_size3_201_sram_blwl_out[213:213] ,mux_1level_tapbuf_size3_201_sram_blwl_outb[213:213] ,mux_1level_tapbuf_size3_201_configbus0[213:213], mux_1level_tapbuf_size3_201_configbus1[213:213] , mux_1level_tapbuf_size3_201_configbus0_b[213:213] );
sram6T_blwl sram_blwl_214_ (mux_1level_tapbuf_size3_201_sram_blwl_out[214:214] ,mux_1level_tapbuf_size3_201_sram_blwl_out[214:214] ,mux_1level_tapbuf_size3_201_sram_blwl_outb[214:214] ,mux_1level_tapbuf_size3_201_configbus0[214:214], mux_1level_tapbuf_size3_201_configbus1[214:214] , mux_1level_tapbuf_size3_201_configbus0_b[214:214] );
sram6T_blwl sram_blwl_215_ (mux_1level_tapbuf_size3_201_sram_blwl_out[215:215] ,mux_1level_tapbuf_size3_201_sram_blwl_out[215:215] ,mux_1level_tapbuf_size3_201_sram_blwl_outb[215:215] ,mux_1level_tapbuf_size3_201_configbus0[215:215], mux_1level_tapbuf_size3_201_configbus1[215:215] , mux_1level_tapbuf_size3_201_configbus0_b[215:215] );
wire [0:2] mux_1level_tapbuf_size3_202_inbus;
assign mux_1level_tapbuf_size3_202_inbus[0] =  grid_1__1__pin_0__1__41_;
assign mux_1level_tapbuf_size3_202_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_202_inbus[2] = chanx_1__0__in_96_ ;
wire [216:218] mux_1level_tapbuf_size3_202_configbus0;
wire [216:218] mux_1level_tapbuf_size3_202_configbus1;
wire [216:218] mux_1level_tapbuf_size3_202_sram_blwl_out ;
wire [216:218] mux_1level_tapbuf_size3_202_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_202_configbus0[216:218] = sram_blwl_bl[216:218] ;
assign mux_1level_tapbuf_size3_202_configbus1[216:218] = sram_blwl_wl[216:218] ;
wire [216:218] mux_1level_tapbuf_size3_202_configbus0_b;
assign mux_1level_tapbuf_size3_202_configbus0_b[216:218] = sram_blwl_blb[216:218] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_202_ (mux_1level_tapbuf_size3_202_inbus, chany_1__1__out_4_ , mux_1level_tapbuf_size3_202_sram_blwl_out[216:218] ,
mux_1level_tapbuf_size3_202_sram_blwl_outb[216:218] );
//----- SRAM bits for MUX[202], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_216_ (mux_1level_tapbuf_size3_202_sram_blwl_out[216:216] ,mux_1level_tapbuf_size3_202_sram_blwl_out[216:216] ,mux_1level_tapbuf_size3_202_sram_blwl_outb[216:216] ,mux_1level_tapbuf_size3_202_configbus0[216:216], mux_1level_tapbuf_size3_202_configbus1[216:216] , mux_1level_tapbuf_size3_202_configbus0_b[216:216] );
sram6T_blwl sram_blwl_217_ (mux_1level_tapbuf_size3_202_sram_blwl_out[217:217] ,mux_1level_tapbuf_size3_202_sram_blwl_out[217:217] ,mux_1level_tapbuf_size3_202_sram_blwl_outb[217:217] ,mux_1level_tapbuf_size3_202_configbus0[217:217], mux_1level_tapbuf_size3_202_configbus1[217:217] , mux_1level_tapbuf_size3_202_configbus0_b[217:217] );
sram6T_blwl sram_blwl_218_ (mux_1level_tapbuf_size3_202_sram_blwl_out[218:218] ,mux_1level_tapbuf_size3_202_sram_blwl_out[218:218] ,mux_1level_tapbuf_size3_202_sram_blwl_outb[218:218] ,mux_1level_tapbuf_size3_202_configbus0[218:218], mux_1level_tapbuf_size3_202_configbus1[218:218] , mux_1level_tapbuf_size3_202_configbus0_b[218:218] );
wire [0:2] mux_1level_tapbuf_size3_203_inbus;
assign mux_1level_tapbuf_size3_203_inbus[0] =  grid_1__1__pin_0__1__41_;
assign mux_1level_tapbuf_size3_203_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_203_inbus[2] = chanx_1__0__in_94_ ;
wire [219:221] mux_1level_tapbuf_size3_203_configbus0;
wire [219:221] mux_1level_tapbuf_size3_203_configbus1;
wire [219:221] mux_1level_tapbuf_size3_203_sram_blwl_out ;
wire [219:221] mux_1level_tapbuf_size3_203_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_203_configbus0[219:221] = sram_blwl_bl[219:221] ;
assign mux_1level_tapbuf_size3_203_configbus1[219:221] = sram_blwl_wl[219:221] ;
wire [219:221] mux_1level_tapbuf_size3_203_configbus0_b;
assign mux_1level_tapbuf_size3_203_configbus0_b[219:221] = sram_blwl_blb[219:221] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_203_ (mux_1level_tapbuf_size3_203_inbus, chany_1__1__out_6_ , mux_1level_tapbuf_size3_203_sram_blwl_out[219:221] ,
mux_1level_tapbuf_size3_203_sram_blwl_outb[219:221] );
//----- SRAM bits for MUX[203], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_219_ (mux_1level_tapbuf_size3_203_sram_blwl_out[219:219] ,mux_1level_tapbuf_size3_203_sram_blwl_out[219:219] ,mux_1level_tapbuf_size3_203_sram_blwl_outb[219:219] ,mux_1level_tapbuf_size3_203_configbus0[219:219], mux_1level_tapbuf_size3_203_configbus1[219:219] , mux_1level_tapbuf_size3_203_configbus0_b[219:219] );
sram6T_blwl sram_blwl_220_ (mux_1level_tapbuf_size3_203_sram_blwl_out[220:220] ,mux_1level_tapbuf_size3_203_sram_blwl_out[220:220] ,mux_1level_tapbuf_size3_203_sram_blwl_outb[220:220] ,mux_1level_tapbuf_size3_203_configbus0[220:220], mux_1level_tapbuf_size3_203_configbus1[220:220] , mux_1level_tapbuf_size3_203_configbus0_b[220:220] );
sram6T_blwl sram_blwl_221_ (mux_1level_tapbuf_size3_203_sram_blwl_out[221:221] ,mux_1level_tapbuf_size3_203_sram_blwl_out[221:221] ,mux_1level_tapbuf_size3_203_sram_blwl_outb[221:221] ,mux_1level_tapbuf_size3_203_configbus0[221:221], mux_1level_tapbuf_size3_203_configbus1[221:221] , mux_1level_tapbuf_size3_203_configbus0_b[221:221] );
wire [0:2] mux_1level_tapbuf_size3_204_inbus;
assign mux_1level_tapbuf_size3_204_inbus[0] =  grid_1__1__pin_0__1__41_;
assign mux_1level_tapbuf_size3_204_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_204_inbus[2] = chanx_1__0__in_92_ ;
wire [222:224] mux_1level_tapbuf_size3_204_configbus0;
wire [222:224] mux_1level_tapbuf_size3_204_configbus1;
wire [222:224] mux_1level_tapbuf_size3_204_sram_blwl_out ;
wire [222:224] mux_1level_tapbuf_size3_204_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_204_configbus0[222:224] = sram_blwl_bl[222:224] ;
assign mux_1level_tapbuf_size3_204_configbus1[222:224] = sram_blwl_wl[222:224] ;
wire [222:224] mux_1level_tapbuf_size3_204_configbus0_b;
assign mux_1level_tapbuf_size3_204_configbus0_b[222:224] = sram_blwl_blb[222:224] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_204_ (mux_1level_tapbuf_size3_204_inbus, chany_1__1__out_8_ , mux_1level_tapbuf_size3_204_sram_blwl_out[222:224] ,
mux_1level_tapbuf_size3_204_sram_blwl_outb[222:224] );
//----- SRAM bits for MUX[204], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_222_ (mux_1level_tapbuf_size3_204_sram_blwl_out[222:222] ,mux_1level_tapbuf_size3_204_sram_blwl_out[222:222] ,mux_1level_tapbuf_size3_204_sram_blwl_outb[222:222] ,mux_1level_tapbuf_size3_204_configbus0[222:222], mux_1level_tapbuf_size3_204_configbus1[222:222] , mux_1level_tapbuf_size3_204_configbus0_b[222:222] );
sram6T_blwl sram_blwl_223_ (mux_1level_tapbuf_size3_204_sram_blwl_out[223:223] ,mux_1level_tapbuf_size3_204_sram_blwl_out[223:223] ,mux_1level_tapbuf_size3_204_sram_blwl_outb[223:223] ,mux_1level_tapbuf_size3_204_configbus0[223:223], mux_1level_tapbuf_size3_204_configbus1[223:223] , mux_1level_tapbuf_size3_204_configbus0_b[223:223] );
sram6T_blwl sram_blwl_224_ (mux_1level_tapbuf_size3_204_sram_blwl_out[224:224] ,mux_1level_tapbuf_size3_204_sram_blwl_out[224:224] ,mux_1level_tapbuf_size3_204_sram_blwl_outb[224:224] ,mux_1level_tapbuf_size3_204_configbus0[224:224], mux_1level_tapbuf_size3_204_configbus1[224:224] , mux_1level_tapbuf_size3_204_configbus0_b[224:224] );
wire [0:1] mux_1level_tapbuf_size2_205_inbus;
assign mux_1level_tapbuf_size2_205_inbus[0] =  grid_1__1__pin_0__1__45_;
assign mux_1level_tapbuf_size2_205_inbus[1] = chanx_1__0__in_90_ ;
wire [225:225] mux_1level_tapbuf_size2_205_configbus0;
wire [225:225] mux_1level_tapbuf_size2_205_configbus1;
wire [225:225] mux_1level_tapbuf_size2_205_sram_blwl_out ;
wire [225:225] mux_1level_tapbuf_size2_205_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_205_configbus0[225:225] = sram_blwl_bl[225:225] ;
assign mux_1level_tapbuf_size2_205_configbus1[225:225] = sram_blwl_wl[225:225] ;
wire [225:225] mux_1level_tapbuf_size2_205_configbus0_b;
assign mux_1level_tapbuf_size2_205_configbus0_b[225:225] = sram_blwl_blb[225:225] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_205_ (mux_1level_tapbuf_size2_205_inbus, chany_1__1__out_10_ , mux_1level_tapbuf_size2_205_sram_blwl_out[225:225] ,
mux_1level_tapbuf_size2_205_sram_blwl_outb[225:225] );
//----- SRAM bits for MUX[205], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_225_ (mux_1level_tapbuf_size2_205_sram_blwl_out[225:225] ,mux_1level_tapbuf_size2_205_sram_blwl_out[225:225] ,mux_1level_tapbuf_size2_205_sram_blwl_outb[225:225] ,mux_1level_tapbuf_size2_205_configbus0[225:225], mux_1level_tapbuf_size2_205_configbus1[225:225] , mux_1level_tapbuf_size2_205_configbus0_b[225:225] );
wire [0:1] mux_1level_tapbuf_size2_206_inbus;
assign mux_1level_tapbuf_size2_206_inbus[0] =  grid_1__1__pin_0__1__45_;
assign mux_1level_tapbuf_size2_206_inbus[1] = chanx_1__0__in_88_ ;
wire [226:226] mux_1level_tapbuf_size2_206_configbus0;
wire [226:226] mux_1level_tapbuf_size2_206_configbus1;
wire [226:226] mux_1level_tapbuf_size2_206_sram_blwl_out ;
wire [226:226] mux_1level_tapbuf_size2_206_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_206_configbus0[226:226] = sram_blwl_bl[226:226] ;
assign mux_1level_tapbuf_size2_206_configbus1[226:226] = sram_blwl_wl[226:226] ;
wire [226:226] mux_1level_tapbuf_size2_206_configbus0_b;
assign mux_1level_tapbuf_size2_206_configbus0_b[226:226] = sram_blwl_blb[226:226] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_206_ (mux_1level_tapbuf_size2_206_inbus, chany_1__1__out_12_ , mux_1level_tapbuf_size2_206_sram_blwl_out[226:226] ,
mux_1level_tapbuf_size2_206_sram_blwl_outb[226:226] );
//----- SRAM bits for MUX[206], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_226_ (mux_1level_tapbuf_size2_206_sram_blwl_out[226:226] ,mux_1level_tapbuf_size2_206_sram_blwl_out[226:226] ,mux_1level_tapbuf_size2_206_sram_blwl_outb[226:226] ,mux_1level_tapbuf_size2_206_configbus0[226:226], mux_1level_tapbuf_size2_206_configbus1[226:226] , mux_1level_tapbuf_size2_206_configbus0_b[226:226] );
wire [0:1] mux_1level_tapbuf_size2_207_inbus;
assign mux_1level_tapbuf_size2_207_inbus[0] =  grid_1__1__pin_0__1__45_;
assign mux_1level_tapbuf_size2_207_inbus[1] = chanx_1__0__in_86_ ;
wire [227:227] mux_1level_tapbuf_size2_207_configbus0;
wire [227:227] mux_1level_tapbuf_size2_207_configbus1;
wire [227:227] mux_1level_tapbuf_size2_207_sram_blwl_out ;
wire [227:227] mux_1level_tapbuf_size2_207_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_207_configbus0[227:227] = sram_blwl_bl[227:227] ;
assign mux_1level_tapbuf_size2_207_configbus1[227:227] = sram_blwl_wl[227:227] ;
wire [227:227] mux_1level_tapbuf_size2_207_configbus0_b;
assign mux_1level_tapbuf_size2_207_configbus0_b[227:227] = sram_blwl_blb[227:227] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_207_ (mux_1level_tapbuf_size2_207_inbus, chany_1__1__out_14_ , mux_1level_tapbuf_size2_207_sram_blwl_out[227:227] ,
mux_1level_tapbuf_size2_207_sram_blwl_outb[227:227] );
//----- SRAM bits for MUX[207], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_227_ (mux_1level_tapbuf_size2_207_sram_blwl_out[227:227] ,mux_1level_tapbuf_size2_207_sram_blwl_out[227:227] ,mux_1level_tapbuf_size2_207_sram_blwl_outb[227:227] ,mux_1level_tapbuf_size2_207_configbus0[227:227], mux_1level_tapbuf_size2_207_configbus1[227:227] , mux_1level_tapbuf_size2_207_configbus0_b[227:227] );
wire [0:1] mux_1level_tapbuf_size2_208_inbus;
assign mux_1level_tapbuf_size2_208_inbus[0] =  grid_1__1__pin_0__1__45_;
assign mux_1level_tapbuf_size2_208_inbus[1] = chanx_1__0__in_84_ ;
wire [228:228] mux_1level_tapbuf_size2_208_configbus0;
wire [228:228] mux_1level_tapbuf_size2_208_configbus1;
wire [228:228] mux_1level_tapbuf_size2_208_sram_blwl_out ;
wire [228:228] mux_1level_tapbuf_size2_208_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_208_configbus0[228:228] = sram_blwl_bl[228:228] ;
assign mux_1level_tapbuf_size2_208_configbus1[228:228] = sram_blwl_wl[228:228] ;
wire [228:228] mux_1level_tapbuf_size2_208_configbus0_b;
assign mux_1level_tapbuf_size2_208_configbus0_b[228:228] = sram_blwl_blb[228:228] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_208_ (mux_1level_tapbuf_size2_208_inbus, chany_1__1__out_16_ , mux_1level_tapbuf_size2_208_sram_blwl_out[228:228] ,
mux_1level_tapbuf_size2_208_sram_blwl_outb[228:228] );
//----- SRAM bits for MUX[208], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_228_ (mux_1level_tapbuf_size2_208_sram_blwl_out[228:228] ,mux_1level_tapbuf_size2_208_sram_blwl_out[228:228] ,mux_1level_tapbuf_size2_208_sram_blwl_outb[228:228] ,mux_1level_tapbuf_size2_208_configbus0[228:228], mux_1level_tapbuf_size2_208_configbus1[228:228] , mux_1level_tapbuf_size2_208_configbus0_b[228:228] );
wire [0:1] mux_1level_tapbuf_size2_209_inbus;
assign mux_1level_tapbuf_size2_209_inbus[0] =  grid_1__1__pin_0__1__45_;
assign mux_1level_tapbuf_size2_209_inbus[1] = chanx_1__0__in_82_ ;
wire [229:229] mux_1level_tapbuf_size2_209_configbus0;
wire [229:229] mux_1level_tapbuf_size2_209_configbus1;
wire [229:229] mux_1level_tapbuf_size2_209_sram_blwl_out ;
wire [229:229] mux_1level_tapbuf_size2_209_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_209_configbus0[229:229] = sram_blwl_bl[229:229] ;
assign mux_1level_tapbuf_size2_209_configbus1[229:229] = sram_blwl_wl[229:229] ;
wire [229:229] mux_1level_tapbuf_size2_209_configbus0_b;
assign mux_1level_tapbuf_size2_209_configbus0_b[229:229] = sram_blwl_blb[229:229] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_209_ (mux_1level_tapbuf_size2_209_inbus, chany_1__1__out_18_ , mux_1level_tapbuf_size2_209_sram_blwl_out[229:229] ,
mux_1level_tapbuf_size2_209_sram_blwl_outb[229:229] );
//----- SRAM bits for MUX[209], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_229_ (mux_1level_tapbuf_size2_209_sram_blwl_out[229:229] ,mux_1level_tapbuf_size2_209_sram_blwl_out[229:229] ,mux_1level_tapbuf_size2_209_sram_blwl_outb[229:229] ,mux_1level_tapbuf_size2_209_configbus0[229:229], mux_1level_tapbuf_size2_209_configbus1[229:229] , mux_1level_tapbuf_size2_209_configbus0_b[229:229] );
wire [0:1] mux_1level_tapbuf_size2_210_inbus;
assign mux_1level_tapbuf_size2_210_inbus[0] =  grid_1__1__pin_0__1__49_;
assign mux_1level_tapbuf_size2_210_inbus[1] = chanx_1__0__in_80_ ;
wire [230:230] mux_1level_tapbuf_size2_210_configbus0;
wire [230:230] mux_1level_tapbuf_size2_210_configbus1;
wire [230:230] mux_1level_tapbuf_size2_210_sram_blwl_out ;
wire [230:230] mux_1level_tapbuf_size2_210_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_210_configbus0[230:230] = sram_blwl_bl[230:230] ;
assign mux_1level_tapbuf_size2_210_configbus1[230:230] = sram_blwl_wl[230:230] ;
wire [230:230] mux_1level_tapbuf_size2_210_configbus0_b;
assign mux_1level_tapbuf_size2_210_configbus0_b[230:230] = sram_blwl_blb[230:230] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_210_ (mux_1level_tapbuf_size2_210_inbus, chany_1__1__out_20_ , mux_1level_tapbuf_size2_210_sram_blwl_out[230:230] ,
mux_1level_tapbuf_size2_210_sram_blwl_outb[230:230] );
//----- SRAM bits for MUX[210], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_230_ (mux_1level_tapbuf_size2_210_sram_blwl_out[230:230] ,mux_1level_tapbuf_size2_210_sram_blwl_out[230:230] ,mux_1level_tapbuf_size2_210_sram_blwl_outb[230:230] ,mux_1level_tapbuf_size2_210_configbus0[230:230], mux_1level_tapbuf_size2_210_configbus1[230:230] , mux_1level_tapbuf_size2_210_configbus0_b[230:230] );
wire [0:1] mux_1level_tapbuf_size2_211_inbus;
assign mux_1level_tapbuf_size2_211_inbus[0] =  grid_1__1__pin_0__1__49_;
assign mux_1level_tapbuf_size2_211_inbus[1] = chanx_1__0__in_78_ ;
wire [231:231] mux_1level_tapbuf_size2_211_configbus0;
wire [231:231] mux_1level_tapbuf_size2_211_configbus1;
wire [231:231] mux_1level_tapbuf_size2_211_sram_blwl_out ;
wire [231:231] mux_1level_tapbuf_size2_211_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_211_configbus0[231:231] = sram_blwl_bl[231:231] ;
assign mux_1level_tapbuf_size2_211_configbus1[231:231] = sram_blwl_wl[231:231] ;
wire [231:231] mux_1level_tapbuf_size2_211_configbus0_b;
assign mux_1level_tapbuf_size2_211_configbus0_b[231:231] = sram_blwl_blb[231:231] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_211_ (mux_1level_tapbuf_size2_211_inbus, chany_1__1__out_22_ , mux_1level_tapbuf_size2_211_sram_blwl_out[231:231] ,
mux_1level_tapbuf_size2_211_sram_blwl_outb[231:231] );
//----- SRAM bits for MUX[211], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_231_ (mux_1level_tapbuf_size2_211_sram_blwl_out[231:231] ,mux_1level_tapbuf_size2_211_sram_blwl_out[231:231] ,mux_1level_tapbuf_size2_211_sram_blwl_outb[231:231] ,mux_1level_tapbuf_size2_211_configbus0[231:231], mux_1level_tapbuf_size2_211_configbus1[231:231] , mux_1level_tapbuf_size2_211_configbus0_b[231:231] );
wire [0:1] mux_1level_tapbuf_size2_212_inbus;
assign mux_1level_tapbuf_size2_212_inbus[0] =  grid_1__1__pin_0__1__49_;
assign mux_1level_tapbuf_size2_212_inbus[1] = chanx_1__0__in_76_ ;
wire [232:232] mux_1level_tapbuf_size2_212_configbus0;
wire [232:232] mux_1level_tapbuf_size2_212_configbus1;
wire [232:232] mux_1level_tapbuf_size2_212_sram_blwl_out ;
wire [232:232] mux_1level_tapbuf_size2_212_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_212_configbus0[232:232] = sram_blwl_bl[232:232] ;
assign mux_1level_tapbuf_size2_212_configbus1[232:232] = sram_blwl_wl[232:232] ;
wire [232:232] mux_1level_tapbuf_size2_212_configbus0_b;
assign mux_1level_tapbuf_size2_212_configbus0_b[232:232] = sram_blwl_blb[232:232] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_212_ (mux_1level_tapbuf_size2_212_inbus, chany_1__1__out_24_ , mux_1level_tapbuf_size2_212_sram_blwl_out[232:232] ,
mux_1level_tapbuf_size2_212_sram_blwl_outb[232:232] );
//----- SRAM bits for MUX[212], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_232_ (mux_1level_tapbuf_size2_212_sram_blwl_out[232:232] ,mux_1level_tapbuf_size2_212_sram_blwl_out[232:232] ,mux_1level_tapbuf_size2_212_sram_blwl_outb[232:232] ,mux_1level_tapbuf_size2_212_configbus0[232:232], mux_1level_tapbuf_size2_212_configbus1[232:232] , mux_1level_tapbuf_size2_212_configbus0_b[232:232] );
wire [0:1] mux_1level_tapbuf_size2_213_inbus;
assign mux_1level_tapbuf_size2_213_inbus[0] =  grid_1__1__pin_0__1__49_;
assign mux_1level_tapbuf_size2_213_inbus[1] = chanx_1__0__in_74_ ;
wire [233:233] mux_1level_tapbuf_size2_213_configbus0;
wire [233:233] mux_1level_tapbuf_size2_213_configbus1;
wire [233:233] mux_1level_tapbuf_size2_213_sram_blwl_out ;
wire [233:233] mux_1level_tapbuf_size2_213_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_213_configbus0[233:233] = sram_blwl_bl[233:233] ;
assign mux_1level_tapbuf_size2_213_configbus1[233:233] = sram_blwl_wl[233:233] ;
wire [233:233] mux_1level_tapbuf_size2_213_configbus0_b;
assign mux_1level_tapbuf_size2_213_configbus0_b[233:233] = sram_blwl_blb[233:233] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_213_ (mux_1level_tapbuf_size2_213_inbus, chany_1__1__out_26_ , mux_1level_tapbuf_size2_213_sram_blwl_out[233:233] ,
mux_1level_tapbuf_size2_213_sram_blwl_outb[233:233] );
//----- SRAM bits for MUX[213], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_233_ (mux_1level_tapbuf_size2_213_sram_blwl_out[233:233] ,mux_1level_tapbuf_size2_213_sram_blwl_out[233:233] ,mux_1level_tapbuf_size2_213_sram_blwl_outb[233:233] ,mux_1level_tapbuf_size2_213_configbus0[233:233], mux_1level_tapbuf_size2_213_configbus1[233:233] , mux_1level_tapbuf_size2_213_configbus0_b[233:233] );
wire [0:1] mux_1level_tapbuf_size2_214_inbus;
assign mux_1level_tapbuf_size2_214_inbus[0] =  grid_1__1__pin_0__1__49_;
assign mux_1level_tapbuf_size2_214_inbus[1] = chanx_1__0__in_72_ ;
wire [234:234] mux_1level_tapbuf_size2_214_configbus0;
wire [234:234] mux_1level_tapbuf_size2_214_configbus1;
wire [234:234] mux_1level_tapbuf_size2_214_sram_blwl_out ;
wire [234:234] mux_1level_tapbuf_size2_214_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_214_configbus0[234:234] = sram_blwl_bl[234:234] ;
assign mux_1level_tapbuf_size2_214_configbus1[234:234] = sram_blwl_wl[234:234] ;
wire [234:234] mux_1level_tapbuf_size2_214_configbus0_b;
assign mux_1level_tapbuf_size2_214_configbus0_b[234:234] = sram_blwl_blb[234:234] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_214_ (mux_1level_tapbuf_size2_214_inbus, chany_1__1__out_28_ , mux_1level_tapbuf_size2_214_sram_blwl_out[234:234] ,
mux_1level_tapbuf_size2_214_sram_blwl_outb[234:234] );
//----- SRAM bits for MUX[214], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_234_ (mux_1level_tapbuf_size2_214_sram_blwl_out[234:234] ,mux_1level_tapbuf_size2_214_sram_blwl_out[234:234] ,mux_1level_tapbuf_size2_214_sram_blwl_outb[234:234] ,mux_1level_tapbuf_size2_214_configbus0[234:234], mux_1level_tapbuf_size2_214_configbus1[234:234] , mux_1level_tapbuf_size2_214_configbus0_b[234:234] );
wire [0:1] mux_1level_tapbuf_size2_215_inbus;
assign mux_1level_tapbuf_size2_215_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_215_inbus[1] = chanx_1__0__in_70_ ;
wire [235:235] mux_1level_tapbuf_size2_215_configbus0;
wire [235:235] mux_1level_tapbuf_size2_215_configbus1;
wire [235:235] mux_1level_tapbuf_size2_215_sram_blwl_out ;
wire [235:235] mux_1level_tapbuf_size2_215_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_215_configbus0[235:235] = sram_blwl_bl[235:235] ;
assign mux_1level_tapbuf_size2_215_configbus1[235:235] = sram_blwl_wl[235:235] ;
wire [235:235] mux_1level_tapbuf_size2_215_configbus0_b;
assign mux_1level_tapbuf_size2_215_configbus0_b[235:235] = sram_blwl_blb[235:235] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_215_ (mux_1level_tapbuf_size2_215_inbus, chany_1__1__out_30_ , mux_1level_tapbuf_size2_215_sram_blwl_out[235:235] ,
mux_1level_tapbuf_size2_215_sram_blwl_outb[235:235] );
//----- SRAM bits for MUX[215], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_235_ (mux_1level_tapbuf_size2_215_sram_blwl_out[235:235] ,mux_1level_tapbuf_size2_215_sram_blwl_out[235:235] ,mux_1level_tapbuf_size2_215_sram_blwl_outb[235:235] ,mux_1level_tapbuf_size2_215_configbus0[235:235], mux_1level_tapbuf_size2_215_configbus1[235:235] , mux_1level_tapbuf_size2_215_configbus0_b[235:235] );
wire [0:1] mux_1level_tapbuf_size2_216_inbus;
assign mux_1level_tapbuf_size2_216_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_216_inbus[1] = chanx_1__0__in_68_ ;
wire [236:236] mux_1level_tapbuf_size2_216_configbus0;
wire [236:236] mux_1level_tapbuf_size2_216_configbus1;
wire [236:236] mux_1level_tapbuf_size2_216_sram_blwl_out ;
wire [236:236] mux_1level_tapbuf_size2_216_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_216_configbus0[236:236] = sram_blwl_bl[236:236] ;
assign mux_1level_tapbuf_size2_216_configbus1[236:236] = sram_blwl_wl[236:236] ;
wire [236:236] mux_1level_tapbuf_size2_216_configbus0_b;
assign mux_1level_tapbuf_size2_216_configbus0_b[236:236] = sram_blwl_blb[236:236] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_216_ (mux_1level_tapbuf_size2_216_inbus, chany_1__1__out_32_ , mux_1level_tapbuf_size2_216_sram_blwl_out[236:236] ,
mux_1level_tapbuf_size2_216_sram_blwl_outb[236:236] );
//----- SRAM bits for MUX[216], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_236_ (mux_1level_tapbuf_size2_216_sram_blwl_out[236:236] ,mux_1level_tapbuf_size2_216_sram_blwl_out[236:236] ,mux_1level_tapbuf_size2_216_sram_blwl_outb[236:236] ,mux_1level_tapbuf_size2_216_configbus0[236:236], mux_1level_tapbuf_size2_216_configbus1[236:236] , mux_1level_tapbuf_size2_216_configbus0_b[236:236] );
wire [0:1] mux_1level_tapbuf_size2_217_inbus;
assign mux_1level_tapbuf_size2_217_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_217_inbus[1] = chanx_1__0__in_66_ ;
wire [237:237] mux_1level_tapbuf_size2_217_configbus0;
wire [237:237] mux_1level_tapbuf_size2_217_configbus1;
wire [237:237] mux_1level_tapbuf_size2_217_sram_blwl_out ;
wire [237:237] mux_1level_tapbuf_size2_217_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_217_configbus0[237:237] = sram_blwl_bl[237:237] ;
assign mux_1level_tapbuf_size2_217_configbus1[237:237] = sram_blwl_wl[237:237] ;
wire [237:237] mux_1level_tapbuf_size2_217_configbus0_b;
assign mux_1level_tapbuf_size2_217_configbus0_b[237:237] = sram_blwl_blb[237:237] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_217_ (mux_1level_tapbuf_size2_217_inbus, chany_1__1__out_34_ , mux_1level_tapbuf_size2_217_sram_blwl_out[237:237] ,
mux_1level_tapbuf_size2_217_sram_blwl_outb[237:237] );
//----- SRAM bits for MUX[217], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_237_ (mux_1level_tapbuf_size2_217_sram_blwl_out[237:237] ,mux_1level_tapbuf_size2_217_sram_blwl_out[237:237] ,mux_1level_tapbuf_size2_217_sram_blwl_outb[237:237] ,mux_1level_tapbuf_size2_217_configbus0[237:237], mux_1level_tapbuf_size2_217_configbus1[237:237] , mux_1level_tapbuf_size2_217_configbus0_b[237:237] );
wire [0:1] mux_1level_tapbuf_size2_218_inbus;
assign mux_1level_tapbuf_size2_218_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_218_inbus[1] = chanx_1__0__in_64_ ;
wire [238:238] mux_1level_tapbuf_size2_218_configbus0;
wire [238:238] mux_1level_tapbuf_size2_218_configbus1;
wire [238:238] mux_1level_tapbuf_size2_218_sram_blwl_out ;
wire [238:238] mux_1level_tapbuf_size2_218_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_218_configbus0[238:238] = sram_blwl_bl[238:238] ;
assign mux_1level_tapbuf_size2_218_configbus1[238:238] = sram_blwl_wl[238:238] ;
wire [238:238] mux_1level_tapbuf_size2_218_configbus0_b;
assign mux_1level_tapbuf_size2_218_configbus0_b[238:238] = sram_blwl_blb[238:238] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_218_ (mux_1level_tapbuf_size2_218_inbus, chany_1__1__out_36_ , mux_1level_tapbuf_size2_218_sram_blwl_out[238:238] ,
mux_1level_tapbuf_size2_218_sram_blwl_outb[238:238] );
//----- SRAM bits for MUX[218], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_238_ (mux_1level_tapbuf_size2_218_sram_blwl_out[238:238] ,mux_1level_tapbuf_size2_218_sram_blwl_out[238:238] ,mux_1level_tapbuf_size2_218_sram_blwl_outb[238:238] ,mux_1level_tapbuf_size2_218_configbus0[238:238], mux_1level_tapbuf_size2_218_configbus1[238:238] , mux_1level_tapbuf_size2_218_configbus0_b[238:238] );
wire [0:1] mux_1level_tapbuf_size2_219_inbus;
assign mux_1level_tapbuf_size2_219_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_219_inbus[1] = chanx_1__0__in_62_ ;
wire [239:239] mux_1level_tapbuf_size2_219_configbus0;
wire [239:239] mux_1level_tapbuf_size2_219_configbus1;
wire [239:239] mux_1level_tapbuf_size2_219_sram_blwl_out ;
wire [239:239] mux_1level_tapbuf_size2_219_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_219_configbus0[239:239] = sram_blwl_bl[239:239] ;
assign mux_1level_tapbuf_size2_219_configbus1[239:239] = sram_blwl_wl[239:239] ;
wire [239:239] mux_1level_tapbuf_size2_219_configbus0_b;
assign mux_1level_tapbuf_size2_219_configbus0_b[239:239] = sram_blwl_blb[239:239] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_219_ (mux_1level_tapbuf_size2_219_inbus, chany_1__1__out_38_ , mux_1level_tapbuf_size2_219_sram_blwl_out[239:239] ,
mux_1level_tapbuf_size2_219_sram_blwl_outb[239:239] );
//----- SRAM bits for MUX[219], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_239_ (mux_1level_tapbuf_size2_219_sram_blwl_out[239:239] ,mux_1level_tapbuf_size2_219_sram_blwl_out[239:239] ,mux_1level_tapbuf_size2_219_sram_blwl_outb[239:239] ,mux_1level_tapbuf_size2_219_configbus0[239:239], mux_1level_tapbuf_size2_219_configbus1[239:239] , mux_1level_tapbuf_size2_219_configbus0_b[239:239] );
wire [0:1] mux_1level_tapbuf_size2_220_inbus;
assign mux_1level_tapbuf_size2_220_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_220_inbus[1] = chanx_1__0__in_60_ ;
wire [240:240] mux_1level_tapbuf_size2_220_configbus0;
wire [240:240] mux_1level_tapbuf_size2_220_configbus1;
wire [240:240] mux_1level_tapbuf_size2_220_sram_blwl_out ;
wire [240:240] mux_1level_tapbuf_size2_220_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_220_configbus0[240:240] = sram_blwl_bl[240:240] ;
assign mux_1level_tapbuf_size2_220_configbus1[240:240] = sram_blwl_wl[240:240] ;
wire [240:240] mux_1level_tapbuf_size2_220_configbus0_b;
assign mux_1level_tapbuf_size2_220_configbus0_b[240:240] = sram_blwl_blb[240:240] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_220_ (mux_1level_tapbuf_size2_220_inbus, chany_1__1__out_40_ , mux_1level_tapbuf_size2_220_sram_blwl_out[240:240] ,
mux_1level_tapbuf_size2_220_sram_blwl_outb[240:240] );
//----- SRAM bits for MUX[220], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_240_ (mux_1level_tapbuf_size2_220_sram_blwl_out[240:240] ,mux_1level_tapbuf_size2_220_sram_blwl_out[240:240] ,mux_1level_tapbuf_size2_220_sram_blwl_outb[240:240] ,mux_1level_tapbuf_size2_220_configbus0[240:240], mux_1level_tapbuf_size2_220_configbus1[240:240] , mux_1level_tapbuf_size2_220_configbus0_b[240:240] );
wire [0:1] mux_1level_tapbuf_size2_221_inbus;
assign mux_1level_tapbuf_size2_221_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_221_inbus[1] = chanx_1__0__in_58_ ;
wire [241:241] mux_1level_tapbuf_size2_221_configbus0;
wire [241:241] mux_1level_tapbuf_size2_221_configbus1;
wire [241:241] mux_1level_tapbuf_size2_221_sram_blwl_out ;
wire [241:241] mux_1level_tapbuf_size2_221_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_221_configbus0[241:241] = sram_blwl_bl[241:241] ;
assign mux_1level_tapbuf_size2_221_configbus1[241:241] = sram_blwl_wl[241:241] ;
wire [241:241] mux_1level_tapbuf_size2_221_configbus0_b;
assign mux_1level_tapbuf_size2_221_configbus0_b[241:241] = sram_blwl_blb[241:241] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_221_ (mux_1level_tapbuf_size2_221_inbus, chany_1__1__out_42_ , mux_1level_tapbuf_size2_221_sram_blwl_out[241:241] ,
mux_1level_tapbuf_size2_221_sram_blwl_outb[241:241] );
//----- SRAM bits for MUX[221], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_241_ (mux_1level_tapbuf_size2_221_sram_blwl_out[241:241] ,mux_1level_tapbuf_size2_221_sram_blwl_out[241:241] ,mux_1level_tapbuf_size2_221_sram_blwl_outb[241:241] ,mux_1level_tapbuf_size2_221_configbus0[241:241], mux_1level_tapbuf_size2_221_configbus1[241:241] , mux_1level_tapbuf_size2_221_configbus0_b[241:241] );
wire [0:1] mux_1level_tapbuf_size2_222_inbus;
assign mux_1level_tapbuf_size2_222_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_222_inbus[1] = chanx_1__0__in_56_ ;
wire [242:242] mux_1level_tapbuf_size2_222_configbus0;
wire [242:242] mux_1level_tapbuf_size2_222_configbus1;
wire [242:242] mux_1level_tapbuf_size2_222_sram_blwl_out ;
wire [242:242] mux_1level_tapbuf_size2_222_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_222_configbus0[242:242] = sram_blwl_bl[242:242] ;
assign mux_1level_tapbuf_size2_222_configbus1[242:242] = sram_blwl_wl[242:242] ;
wire [242:242] mux_1level_tapbuf_size2_222_configbus0_b;
assign mux_1level_tapbuf_size2_222_configbus0_b[242:242] = sram_blwl_blb[242:242] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_222_ (mux_1level_tapbuf_size2_222_inbus, chany_1__1__out_44_ , mux_1level_tapbuf_size2_222_sram_blwl_out[242:242] ,
mux_1level_tapbuf_size2_222_sram_blwl_outb[242:242] );
//----- SRAM bits for MUX[222], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_242_ (mux_1level_tapbuf_size2_222_sram_blwl_out[242:242] ,mux_1level_tapbuf_size2_222_sram_blwl_out[242:242] ,mux_1level_tapbuf_size2_222_sram_blwl_outb[242:242] ,mux_1level_tapbuf_size2_222_configbus0[242:242], mux_1level_tapbuf_size2_222_configbus1[242:242] , mux_1level_tapbuf_size2_222_configbus0_b[242:242] );
wire [0:1] mux_1level_tapbuf_size2_223_inbus;
assign mux_1level_tapbuf_size2_223_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_223_inbus[1] = chanx_1__0__in_54_ ;
wire [243:243] mux_1level_tapbuf_size2_223_configbus0;
wire [243:243] mux_1level_tapbuf_size2_223_configbus1;
wire [243:243] mux_1level_tapbuf_size2_223_sram_blwl_out ;
wire [243:243] mux_1level_tapbuf_size2_223_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_223_configbus0[243:243] = sram_blwl_bl[243:243] ;
assign mux_1level_tapbuf_size2_223_configbus1[243:243] = sram_blwl_wl[243:243] ;
wire [243:243] mux_1level_tapbuf_size2_223_configbus0_b;
assign mux_1level_tapbuf_size2_223_configbus0_b[243:243] = sram_blwl_blb[243:243] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_223_ (mux_1level_tapbuf_size2_223_inbus, chany_1__1__out_46_ , mux_1level_tapbuf_size2_223_sram_blwl_out[243:243] ,
mux_1level_tapbuf_size2_223_sram_blwl_outb[243:243] );
//----- SRAM bits for MUX[223], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_243_ (mux_1level_tapbuf_size2_223_sram_blwl_out[243:243] ,mux_1level_tapbuf_size2_223_sram_blwl_out[243:243] ,mux_1level_tapbuf_size2_223_sram_blwl_outb[243:243] ,mux_1level_tapbuf_size2_223_configbus0[243:243], mux_1level_tapbuf_size2_223_configbus1[243:243] , mux_1level_tapbuf_size2_223_configbus0_b[243:243] );
wire [0:1] mux_1level_tapbuf_size2_224_inbus;
assign mux_1level_tapbuf_size2_224_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_224_inbus[1] = chanx_1__0__in_52_ ;
wire [244:244] mux_1level_tapbuf_size2_224_configbus0;
wire [244:244] mux_1level_tapbuf_size2_224_configbus1;
wire [244:244] mux_1level_tapbuf_size2_224_sram_blwl_out ;
wire [244:244] mux_1level_tapbuf_size2_224_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_224_configbus0[244:244] = sram_blwl_bl[244:244] ;
assign mux_1level_tapbuf_size2_224_configbus1[244:244] = sram_blwl_wl[244:244] ;
wire [244:244] mux_1level_tapbuf_size2_224_configbus0_b;
assign mux_1level_tapbuf_size2_224_configbus0_b[244:244] = sram_blwl_blb[244:244] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_224_ (mux_1level_tapbuf_size2_224_inbus, chany_1__1__out_48_ , mux_1level_tapbuf_size2_224_sram_blwl_out[244:244] ,
mux_1level_tapbuf_size2_224_sram_blwl_outb[244:244] );
//----- SRAM bits for MUX[224], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_244_ (mux_1level_tapbuf_size2_224_sram_blwl_out[244:244] ,mux_1level_tapbuf_size2_224_sram_blwl_out[244:244] ,mux_1level_tapbuf_size2_224_sram_blwl_outb[244:244] ,mux_1level_tapbuf_size2_224_configbus0[244:244], mux_1level_tapbuf_size2_224_configbus1[244:244] , mux_1level_tapbuf_size2_224_configbus0_b[244:244] );
wire [0:1] mux_1level_tapbuf_size2_225_inbus;
assign mux_1level_tapbuf_size2_225_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_225_inbus[1] = chanx_1__0__in_50_ ;
wire [245:245] mux_1level_tapbuf_size2_225_configbus0;
wire [245:245] mux_1level_tapbuf_size2_225_configbus1;
wire [245:245] mux_1level_tapbuf_size2_225_sram_blwl_out ;
wire [245:245] mux_1level_tapbuf_size2_225_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_225_configbus0[245:245] = sram_blwl_bl[245:245] ;
assign mux_1level_tapbuf_size2_225_configbus1[245:245] = sram_blwl_wl[245:245] ;
wire [245:245] mux_1level_tapbuf_size2_225_configbus0_b;
assign mux_1level_tapbuf_size2_225_configbus0_b[245:245] = sram_blwl_blb[245:245] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_225_ (mux_1level_tapbuf_size2_225_inbus, chany_1__1__out_50_ , mux_1level_tapbuf_size2_225_sram_blwl_out[245:245] ,
mux_1level_tapbuf_size2_225_sram_blwl_outb[245:245] );
//----- SRAM bits for MUX[225], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_245_ (mux_1level_tapbuf_size2_225_sram_blwl_out[245:245] ,mux_1level_tapbuf_size2_225_sram_blwl_out[245:245] ,mux_1level_tapbuf_size2_225_sram_blwl_outb[245:245] ,mux_1level_tapbuf_size2_225_configbus0[245:245], mux_1level_tapbuf_size2_225_configbus1[245:245] , mux_1level_tapbuf_size2_225_configbus0_b[245:245] );
wire [0:1] mux_1level_tapbuf_size2_226_inbus;
assign mux_1level_tapbuf_size2_226_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_226_inbus[1] = chanx_1__0__in_48_ ;
wire [246:246] mux_1level_tapbuf_size2_226_configbus0;
wire [246:246] mux_1level_tapbuf_size2_226_configbus1;
wire [246:246] mux_1level_tapbuf_size2_226_sram_blwl_out ;
wire [246:246] mux_1level_tapbuf_size2_226_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_226_configbus0[246:246] = sram_blwl_bl[246:246] ;
assign mux_1level_tapbuf_size2_226_configbus1[246:246] = sram_blwl_wl[246:246] ;
wire [246:246] mux_1level_tapbuf_size2_226_configbus0_b;
assign mux_1level_tapbuf_size2_226_configbus0_b[246:246] = sram_blwl_blb[246:246] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_226_ (mux_1level_tapbuf_size2_226_inbus, chany_1__1__out_52_ , mux_1level_tapbuf_size2_226_sram_blwl_out[246:246] ,
mux_1level_tapbuf_size2_226_sram_blwl_outb[246:246] );
//----- SRAM bits for MUX[226], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_246_ (mux_1level_tapbuf_size2_226_sram_blwl_out[246:246] ,mux_1level_tapbuf_size2_226_sram_blwl_out[246:246] ,mux_1level_tapbuf_size2_226_sram_blwl_outb[246:246] ,mux_1level_tapbuf_size2_226_configbus0[246:246], mux_1level_tapbuf_size2_226_configbus1[246:246] , mux_1level_tapbuf_size2_226_configbus0_b[246:246] );
wire [0:1] mux_1level_tapbuf_size2_227_inbus;
assign mux_1level_tapbuf_size2_227_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_227_inbus[1] = chanx_1__0__in_46_ ;
wire [247:247] mux_1level_tapbuf_size2_227_configbus0;
wire [247:247] mux_1level_tapbuf_size2_227_configbus1;
wire [247:247] mux_1level_tapbuf_size2_227_sram_blwl_out ;
wire [247:247] mux_1level_tapbuf_size2_227_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_227_configbus0[247:247] = sram_blwl_bl[247:247] ;
assign mux_1level_tapbuf_size2_227_configbus1[247:247] = sram_blwl_wl[247:247] ;
wire [247:247] mux_1level_tapbuf_size2_227_configbus0_b;
assign mux_1level_tapbuf_size2_227_configbus0_b[247:247] = sram_blwl_blb[247:247] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_227_ (mux_1level_tapbuf_size2_227_inbus, chany_1__1__out_54_ , mux_1level_tapbuf_size2_227_sram_blwl_out[247:247] ,
mux_1level_tapbuf_size2_227_sram_blwl_outb[247:247] );
//----- SRAM bits for MUX[227], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_247_ (mux_1level_tapbuf_size2_227_sram_blwl_out[247:247] ,mux_1level_tapbuf_size2_227_sram_blwl_out[247:247] ,mux_1level_tapbuf_size2_227_sram_blwl_outb[247:247] ,mux_1level_tapbuf_size2_227_configbus0[247:247], mux_1level_tapbuf_size2_227_configbus1[247:247] , mux_1level_tapbuf_size2_227_configbus0_b[247:247] );
wire [0:1] mux_1level_tapbuf_size2_228_inbus;
assign mux_1level_tapbuf_size2_228_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_228_inbus[1] = chanx_1__0__in_44_ ;
wire [248:248] mux_1level_tapbuf_size2_228_configbus0;
wire [248:248] mux_1level_tapbuf_size2_228_configbus1;
wire [248:248] mux_1level_tapbuf_size2_228_sram_blwl_out ;
wire [248:248] mux_1level_tapbuf_size2_228_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_228_configbus0[248:248] = sram_blwl_bl[248:248] ;
assign mux_1level_tapbuf_size2_228_configbus1[248:248] = sram_blwl_wl[248:248] ;
wire [248:248] mux_1level_tapbuf_size2_228_configbus0_b;
assign mux_1level_tapbuf_size2_228_configbus0_b[248:248] = sram_blwl_blb[248:248] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_228_ (mux_1level_tapbuf_size2_228_inbus, chany_1__1__out_56_ , mux_1level_tapbuf_size2_228_sram_blwl_out[248:248] ,
mux_1level_tapbuf_size2_228_sram_blwl_outb[248:248] );
//----- SRAM bits for MUX[228], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_248_ (mux_1level_tapbuf_size2_228_sram_blwl_out[248:248] ,mux_1level_tapbuf_size2_228_sram_blwl_out[248:248] ,mux_1level_tapbuf_size2_228_sram_blwl_outb[248:248] ,mux_1level_tapbuf_size2_228_configbus0[248:248], mux_1level_tapbuf_size2_228_configbus1[248:248] , mux_1level_tapbuf_size2_228_configbus0_b[248:248] );
wire [0:1] mux_1level_tapbuf_size2_229_inbus;
assign mux_1level_tapbuf_size2_229_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_229_inbus[1] = chanx_1__0__in_42_ ;
wire [249:249] mux_1level_tapbuf_size2_229_configbus0;
wire [249:249] mux_1level_tapbuf_size2_229_configbus1;
wire [249:249] mux_1level_tapbuf_size2_229_sram_blwl_out ;
wire [249:249] mux_1level_tapbuf_size2_229_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_229_configbus0[249:249] = sram_blwl_bl[249:249] ;
assign mux_1level_tapbuf_size2_229_configbus1[249:249] = sram_blwl_wl[249:249] ;
wire [249:249] mux_1level_tapbuf_size2_229_configbus0_b;
assign mux_1level_tapbuf_size2_229_configbus0_b[249:249] = sram_blwl_blb[249:249] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_229_ (mux_1level_tapbuf_size2_229_inbus, chany_1__1__out_58_ , mux_1level_tapbuf_size2_229_sram_blwl_out[249:249] ,
mux_1level_tapbuf_size2_229_sram_blwl_outb[249:249] );
//----- SRAM bits for MUX[229], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_249_ (mux_1level_tapbuf_size2_229_sram_blwl_out[249:249] ,mux_1level_tapbuf_size2_229_sram_blwl_out[249:249] ,mux_1level_tapbuf_size2_229_sram_blwl_outb[249:249] ,mux_1level_tapbuf_size2_229_configbus0[249:249], mux_1level_tapbuf_size2_229_configbus1[249:249] , mux_1level_tapbuf_size2_229_configbus0_b[249:249] );
wire [0:1] mux_1level_tapbuf_size2_230_inbus;
assign mux_1level_tapbuf_size2_230_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_230_inbus[1] = chanx_1__0__in_40_ ;
wire [250:250] mux_1level_tapbuf_size2_230_configbus0;
wire [250:250] mux_1level_tapbuf_size2_230_configbus1;
wire [250:250] mux_1level_tapbuf_size2_230_sram_blwl_out ;
wire [250:250] mux_1level_tapbuf_size2_230_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_230_configbus0[250:250] = sram_blwl_bl[250:250] ;
assign mux_1level_tapbuf_size2_230_configbus1[250:250] = sram_blwl_wl[250:250] ;
wire [250:250] mux_1level_tapbuf_size2_230_configbus0_b;
assign mux_1level_tapbuf_size2_230_configbus0_b[250:250] = sram_blwl_blb[250:250] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_230_ (mux_1level_tapbuf_size2_230_inbus, chany_1__1__out_60_ , mux_1level_tapbuf_size2_230_sram_blwl_out[250:250] ,
mux_1level_tapbuf_size2_230_sram_blwl_outb[250:250] );
//----- SRAM bits for MUX[230], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_250_ (mux_1level_tapbuf_size2_230_sram_blwl_out[250:250] ,mux_1level_tapbuf_size2_230_sram_blwl_out[250:250] ,mux_1level_tapbuf_size2_230_sram_blwl_outb[250:250] ,mux_1level_tapbuf_size2_230_configbus0[250:250], mux_1level_tapbuf_size2_230_configbus1[250:250] , mux_1level_tapbuf_size2_230_configbus0_b[250:250] );
wire [0:1] mux_1level_tapbuf_size2_231_inbus;
assign mux_1level_tapbuf_size2_231_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_231_inbus[1] = chanx_1__0__in_38_ ;
wire [251:251] mux_1level_tapbuf_size2_231_configbus0;
wire [251:251] mux_1level_tapbuf_size2_231_configbus1;
wire [251:251] mux_1level_tapbuf_size2_231_sram_blwl_out ;
wire [251:251] mux_1level_tapbuf_size2_231_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_231_configbus0[251:251] = sram_blwl_bl[251:251] ;
assign mux_1level_tapbuf_size2_231_configbus1[251:251] = sram_blwl_wl[251:251] ;
wire [251:251] mux_1level_tapbuf_size2_231_configbus0_b;
assign mux_1level_tapbuf_size2_231_configbus0_b[251:251] = sram_blwl_blb[251:251] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_231_ (mux_1level_tapbuf_size2_231_inbus, chany_1__1__out_62_ , mux_1level_tapbuf_size2_231_sram_blwl_out[251:251] ,
mux_1level_tapbuf_size2_231_sram_blwl_outb[251:251] );
//----- SRAM bits for MUX[231], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_251_ (mux_1level_tapbuf_size2_231_sram_blwl_out[251:251] ,mux_1level_tapbuf_size2_231_sram_blwl_out[251:251] ,mux_1level_tapbuf_size2_231_sram_blwl_outb[251:251] ,mux_1level_tapbuf_size2_231_configbus0[251:251], mux_1level_tapbuf_size2_231_configbus1[251:251] , mux_1level_tapbuf_size2_231_configbus0_b[251:251] );
wire [0:1] mux_1level_tapbuf_size2_232_inbus;
assign mux_1level_tapbuf_size2_232_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_232_inbus[1] = chanx_1__0__in_36_ ;
wire [252:252] mux_1level_tapbuf_size2_232_configbus0;
wire [252:252] mux_1level_tapbuf_size2_232_configbus1;
wire [252:252] mux_1level_tapbuf_size2_232_sram_blwl_out ;
wire [252:252] mux_1level_tapbuf_size2_232_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_232_configbus0[252:252] = sram_blwl_bl[252:252] ;
assign mux_1level_tapbuf_size2_232_configbus1[252:252] = sram_blwl_wl[252:252] ;
wire [252:252] mux_1level_tapbuf_size2_232_configbus0_b;
assign mux_1level_tapbuf_size2_232_configbus0_b[252:252] = sram_blwl_blb[252:252] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_232_ (mux_1level_tapbuf_size2_232_inbus, chany_1__1__out_64_ , mux_1level_tapbuf_size2_232_sram_blwl_out[252:252] ,
mux_1level_tapbuf_size2_232_sram_blwl_outb[252:252] );
//----- SRAM bits for MUX[232], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_252_ (mux_1level_tapbuf_size2_232_sram_blwl_out[252:252] ,mux_1level_tapbuf_size2_232_sram_blwl_out[252:252] ,mux_1level_tapbuf_size2_232_sram_blwl_outb[252:252] ,mux_1level_tapbuf_size2_232_configbus0[252:252], mux_1level_tapbuf_size2_232_configbus1[252:252] , mux_1level_tapbuf_size2_232_configbus0_b[252:252] );
wire [0:1] mux_1level_tapbuf_size2_233_inbus;
assign mux_1level_tapbuf_size2_233_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_233_inbus[1] = chanx_1__0__in_34_ ;
wire [253:253] mux_1level_tapbuf_size2_233_configbus0;
wire [253:253] mux_1level_tapbuf_size2_233_configbus1;
wire [253:253] mux_1level_tapbuf_size2_233_sram_blwl_out ;
wire [253:253] mux_1level_tapbuf_size2_233_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_233_configbus0[253:253] = sram_blwl_bl[253:253] ;
assign mux_1level_tapbuf_size2_233_configbus1[253:253] = sram_blwl_wl[253:253] ;
wire [253:253] mux_1level_tapbuf_size2_233_configbus0_b;
assign mux_1level_tapbuf_size2_233_configbus0_b[253:253] = sram_blwl_blb[253:253] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_233_ (mux_1level_tapbuf_size2_233_inbus, chany_1__1__out_66_ , mux_1level_tapbuf_size2_233_sram_blwl_out[253:253] ,
mux_1level_tapbuf_size2_233_sram_blwl_outb[253:253] );
//----- SRAM bits for MUX[233], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_253_ (mux_1level_tapbuf_size2_233_sram_blwl_out[253:253] ,mux_1level_tapbuf_size2_233_sram_blwl_out[253:253] ,mux_1level_tapbuf_size2_233_sram_blwl_outb[253:253] ,mux_1level_tapbuf_size2_233_configbus0[253:253], mux_1level_tapbuf_size2_233_configbus1[253:253] , mux_1level_tapbuf_size2_233_configbus0_b[253:253] );
wire [0:1] mux_1level_tapbuf_size2_234_inbus;
assign mux_1level_tapbuf_size2_234_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_234_inbus[1] = chanx_1__0__in_32_ ;
wire [254:254] mux_1level_tapbuf_size2_234_configbus0;
wire [254:254] mux_1level_tapbuf_size2_234_configbus1;
wire [254:254] mux_1level_tapbuf_size2_234_sram_blwl_out ;
wire [254:254] mux_1level_tapbuf_size2_234_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_234_configbus0[254:254] = sram_blwl_bl[254:254] ;
assign mux_1level_tapbuf_size2_234_configbus1[254:254] = sram_blwl_wl[254:254] ;
wire [254:254] mux_1level_tapbuf_size2_234_configbus0_b;
assign mux_1level_tapbuf_size2_234_configbus0_b[254:254] = sram_blwl_blb[254:254] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_234_ (mux_1level_tapbuf_size2_234_inbus, chany_1__1__out_68_ , mux_1level_tapbuf_size2_234_sram_blwl_out[254:254] ,
mux_1level_tapbuf_size2_234_sram_blwl_outb[254:254] );
//----- SRAM bits for MUX[234], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_254_ (mux_1level_tapbuf_size2_234_sram_blwl_out[254:254] ,mux_1level_tapbuf_size2_234_sram_blwl_out[254:254] ,mux_1level_tapbuf_size2_234_sram_blwl_outb[254:254] ,mux_1level_tapbuf_size2_234_configbus0[254:254], mux_1level_tapbuf_size2_234_configbus1[254:254] , mux_1level_tapbuf_size2_234_configbus0_b[254:254] );
wire [0:1] mux_1level_tapbuf_size2_235_inbus;
assign mux_1level_tapbuf_size2_235_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_235_inbus[1] = chanx_1__0__in_30_ ;
wire [255:255] mux_1level_tapbuf_size2_235_configbus0;
wire [255:255] mux_1level_tapbuf_size2_235_configbus1;
wire [255:255] mux_1level_tapbuf_size2_235_sram_blwl_out ;
wire [255:255] mux_1level_tapbuf_size2_235_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_235_configbus0[255:255] = sram_blwl_bl[255:255] ;
assign mux_1level_tapbuf_size2_235_configbus1[255:255] = sram_blwl_wl[255:255] ;
wire [255:255] mux_1level_tapbuf_size2_235_configbus0_b;
assign mux_1level_tapbuf_size2_235_configbus0_b[255:255] = sram_blwl_blb[255:255] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_235_ (mux_1level_tapbuf_size2_235_inbus, chany_1__1__out_70_ , mux_1level_tapbuf_size2_235_sram_blwl_out[255:255] ,
mux_1level_tapbuf_size2_235_sram_blwl_outb[255:255] );
//----- SRAM bits for MUX[235], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_255_ (mux_1level_tapbuf_size2_235_sram_blwl_out[255:255] ,mux_1level_tapbuf_size2_235_sram_blwl_out[255:255] ,mux_1level_tapbuf_size2_235_sram_blwl_outb[255:255] ,mux_1level_tapbuf_size2_235_configbus0[255:255], mux_1level_tapbuf_size2_235_configbus1[255:255] , mux_1level_tapbuf_size2_235_configbus0_b[255:255] );
wire [0:1] mux_1level_tapbuf_size2_236_inbus;
assign mux_1level_tapbuf_size2_236_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_236_inbus[1] = chanx_1__0__in_28_ ;
wire [256:256] mux_1level_tapbuf_size2_236_configbus0;
wire [256:256] mux_1level_tapbuf_size2_236_configbus1;
wire [256:256] mux_1level_tapbuf_size2_236_sram_blwl_out ;
wire [256:256] mux_1level_tapbuf_size2_236_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_236_configbus0[256:256] = sram_blwl_bl[256:256] ;
assign mux_1level_tapbuf_size2_236_configbus1[256:256] = sram_blwl_wl[256:256] ;
wire [256:256] mux_1level_tapbuf_size2_236_configbus0_b;
assign mux_1level_tapbuf_size2_236_configbus0_b[256:256] = sram_blwl_blb[256:256] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_236_ (mux_1level_tapbuf_size2_236_inbus, chany_1__1__out_72_ , mux_1level_tapbuf_size2_236_sram_blwl_out[256:256] ,
mux_1level_tapbuf_size2_236_sram_blwl_outb[256:256] );
//----- SRAM bits for MUX[236], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_256_ (mux_1level_tapbuf_size2_236_sram_blwl_out[256:256] ,mux_1level_tapbuf_size2_236_sram_blwl_out[256:256] ,mux_1level_tapbuf_size2_236_sram_blwl_outb[256:256] ,mux_1level_tapbuf_size2_236_configbus0[256:256], mux_1level_tapbuf_size2_236_configbus1[256:256] , mux_1level_tapbuf_size2_236_configbus0_b[256:256] );
wire [0:1] mux_1level_tapbuf_size2_237_inbus;
assign mux_1level_tapbuf_size2_237_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_237_inbus[1] = chanx_1__0__in_26_ ;
wire [257:257] mux_1level_tapbuf_size2_237_configbus0;
wire [257:257] mux_1level_tapbuf_size2_237_configbus1;
wire [257:257] mux_1level_tapbuf_size2_237_sram_blwl_out ;
wire [257:257] mux_1level_tapbuf_size2_237_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_237_configbus0[257:257] = sram_blwl_bl[257:257] ;
assign mux_1level_tapbuf_size2_237_configbus1[257:257] = sram_blwl_wl[257:257] ;
wire [257:257] mux_1level_tapbuf_size2_237_configbus0_b;
assign mux_1level_tapbuf_size2_237_configbus0_b[257:257] = sram_blwl_blb[257:257] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_237_ (mux_1level_tapbuf_size2_237_inbus, chany_1__1__out_74_ , mux_1level_tapbuf_size2_237_sram_blwl_out[257:257] ,
mux_1level_tapbuf_size2_237_sram_blwl_outb[257:257] );
//----- SRAM bits for MUX[237], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_257_ (mux_1level_tapbuf_size2_237_sram_blwl_out[257:257] ,mux_1level_tapbuf_size2_237_sram_blwl_out[257:257] ,mux_1level_tapbuf_size2_237_sram_blwl_outb[257:257] ,mux_1level_tapbuf_size2_237_configbus0[257:257], mux_1level_tapbuf_size2_237_configbus1[257:257] , mux_1level_tapbuf_size2_237_configbus0_b[257:257] );
wire [0:1] mux_1level_tapbuf_size2_238_inbus;
assign mux_1level_tapbuf_size2_238_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_238_inbus[1] = chanx_1__0__in_24_ ;
wire [258:258] mux_1level_tapbuf_size2_238_configbus0;
wire [258:258] mux_1level_tapbuf_size2_238_configbus1;
wire [258:258] mux_1level_tapbuf_size2_238_sram_blwl_out ;
wire [258:258] mux_1level_tapbuf_size2_238_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_238_configbus0[258:258] = sram_blwl_bl[258:258] ;
assign mux_1level_tapbuf_size2_238_configbus1[258:258] = sram_blwl_wl[258:258] ;
wire [258:258] mux_1level_tapbuf_size2_238_configbus0_b;
assign mux_1level_tapbuf_size2_238_configbus0_b[258:258] = sram_blwl_blb[258:258] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_238_ (mux_1level_tapbuf_size2_238_inbus, chany_1__1__out_76_ , mux_1level_tapbuf_size2_238_sram_blwl_out[258:258] ,
mux_1level_tapbuf_size2_238_sram_blwl_outb[258:258] );
//----- SRAM bits for MUX[238], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_258_ (mux_1level_tapbuf_size2_238_sram_blwl_out[258:258] ,mux_1level_tapbuf_size2_238_sram_blwl_out[258:258] ,mux_1level_tapbuf_size2_238_sram_blwl_outb[258:258] ,mux_1level_tapbuf_size2_238_configbus0[258:258], mux_1level_tapbuf_size2_238_configbus1[258:258] , mux_1level_tapbuf_size2_238_configbus0_b[258:258] );
wire [0:1] mux_1level_tapbuf_size2_239_inbus;
assign mux_1level_tapbuf_size2_239_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_239_inbus[1] = chanx_1__0__in_22_ ;
wire [259:259] mux_1level_tapbuf_size2_239_configbus0;
wire [259:259] mux_1level_tapbuf_size2_239_configbus1;
wire [259:259] mux_1level_tapbuf_size2_239_sram_blwl_out ;
wire [259:259] mux_1level_tapbuf_size2_239_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_239_configbus0[259:259] = sram_blwl_bl[259:259] ;
assign mux_1level_tapbuf_size2_239_configbus1[259:259] = sram_blwl_wl[259:259] ;
wire [259:259] mux_1level_tapbuf_size2_239_configbus0_b;
assign mux_1level_tapbuf_size2_239_configbus0_b[259:259] = sram_blwl_blb[259:259] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_239_ (mux_1level_tapbuf_size2_239_inbus, chany_1__1__out_78_ , mux_1level_tapbuf_size2_239_sram_blwl_out[259:259] ,
mux_1level_tapbuf_size2_239_sram_blwl_outb[259:259] );
//----- SRAM bits for MUX[239], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_259_ (mux_1level_tapbuf_size2_239_sram_blwl_out[259:259] ,mux_1level_tapbuf_size2_239_sram_blwl_out[259:259] ,mux_1level_tapbuf_size2_239_sram_blwl_outb[259:259] ,mux_1level_tapbuf_size2_239_configbus0[259:259], mux_1level_tapbuf_size2_239_configbus1[259:259] , mux_1level_tapbuf_size2_239_configbus0_b[259:259] );
wire [0:1] mux_1level_tapbuf_size2_240_inbus;
assign mux_1level_tapbuf_size2_240_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_240_inbus[1] = chanx_1__0__in_20_ ;
wire [260:260] mux_1level_tapbuf_size2_240_configbus0;
wire [260:260] mux_1level_tapbuf_size2_240_configbus1;
wire [260:260] mux_1level_tapbuf_size2_240_sram_blwl_out ;
wire [260:260] mux_1level_tapbuf_size2_240_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_240_configbus0[260:260] = sram_blwl_bl[260:260] ;
assign mux_1level_tapbuf_size2_240_configbus1[260:260] = sram_blwl_wl[260:260] ;
wire [260:260] mux_1level_tapbuf_size2_240_configbus0_b;
assign mux_1level_tapbuf_size2_240_configbus0_b[260:260] = sram_blwl_blb[260:260] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_240_ (mux_1level_tapbuf_size2_240_inbus, chany_1__1__out_80_ , mux_1level_tapbuf_size2_240_sram_blwl_out[260:260] ,
mux_1level_tapbuf_size2_240_sram_blwl_outb[260:260] );
//----- SRAM bits for MUX[240], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_260_ (mux_1level_tapbuf_size2_240_sram_blwl_out[260:260] ,mux_1level_tapbuf_size2_240_sram_blwl_out[260:260] ,mux_1level_tapbuf_size2_240_sram_blwl_outb[260:260] ,mux_1level_tapbuf_size2_240_configbus0[260:260], mux_1level_tapbuf_size2_240_configbus1[260:260] , mux_1level_tapbuf_size2_240_configbus0_b[260:260] );
wire [0:1] mux_1level_tapbuf_size2_241_inbus;
assign mux_1level_tapbuf_size2_241_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_241_inbus[1] = chanx_1__0__in_18_ ;
wire [261:261] mux_1level_tapbuf_size2_241_configbus0;
wire [261:261] mux_1level_tapbuf_size2_241_configbus1;
wire [261:261] mux_1level_tapbuf_size2_241_sram_blwl_out ;
wire [261:261] mux_1level_tapbuf_size2_241_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_241_configbus0[261:261] = sram_blwl_bl[261:261] ;
assign mux_1level_tapbuf_size2_241_configbus1[261:261] = sram_blwl_wl[261:261] ;
wire [261:261] mux_1level_tapbuf_size2_241_configbus0_b;
assign mux_1level_tapbuf_size2_241_configbus0_b[261:261] = sram_blwl_blb[261:261] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_241_ (mux_1level_tapbuf_size2_241_inbus, chany_1__1__out_82_ , mux_1level_tapbuf_size2_241_sram_blwl_out[261:261] ,
mux_1level_tapbuf_size2_241_sram_blwl_outb[261:261] );
//----- SRAM bits for MUX[241], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_261_ (mux_1level_tapbuf_size2_241_sram_blwl_out[261:261] ,mux_1level_tapbuf_size2_241_sram_blwl_out[261:261] ,mux_1level_tapbuf_size2_241_sram_blwl_outb[261:261] ,mux_1level_tapbuf_size2_241_configbus0[261:261], mux_1level_tapbuf_size2_241_configbus1[261:261] , mux_1level_tapbuf_size2_241_configbus0_b[261:261] );
wire [0:1] mux_1level_tapbuf_size2_242_inbus;
assign mux_1level_tapbuf_size2_242_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_242_inbus[1] = chanx_1__0__in_16_ ;
wire [262:262] mux_1level_tapbuf_size2_242_configbus0;
wire [262:262] mux_1level_tapbuf_size2_242_configbus1;
wire [262:262] mux_1level_tapbuf_size2_242_sram_blwl_out ;
wire [262:262] mux_1level_tapbuf_size2_242_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_242_configbus0[262:262] = sram_blwl_bl[262:262] ;
assign mux_1level_tapbuf_size2_242_configbus1[262:262] = sram_blwl_wl[262:262] ;
wire [262:262] mux_1level_tapbuf_size2_242_configbus0_b;
assign mux_1level_tapbuf_size2_242_configbus0_b[262:262] = sram_blwl_blb[262:262] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_242_ (mux_1level_tapbuf_size2_242_inbus, chany_1__1__out_84_ , mux_1level_tapbuf_size2_242_sram_blwl_out[262:262] ,
mux_1level_tapbuf_size2_242_sram_blwl_outb[262:262] );
//----- SRAM bits for MUX[242], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_262_ (mux_1level_tapbuf_size2_242_sram_blwl_out[262:262] ,mux_1level_tapbuf_size2_242_sram_blwl_out[262:262] ,mux_1level_tapbuf_size2_242_sram_blwl_outb[262:262] ,mux_1level_tapbuf_size2_242_configbus0[262:262], mux_1level_tapbuf_size2_242_configbus1[262:262] , mux_1level_tapbuf_size2_242_configbus0_b[262:262] );
wire [0:1] mux_1level_tapbuf_size2_243_inbus;
assign mux_1level_tapbuf_size2_243_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_243_inbus[1] = chanx_1__0__in_14_ ;
wire [263:263] mux_1level_tapbuf_size2_243_configbus0;
wire [263:263] mux_1level_tapbuf_size2_243_configbus1;
wire [263:263] mux_1level_tapbuf_size2_243_sram_blwl_out ;
wire [263:263] mux_1level_tapbuf_size2_243_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_243_configbus0[263:263] = sram_blwl_bl[263:263] ;
assign mux_1level_tapbuf_size2_243_configbus1[263:263] = sram_blwl_wl[263:263] ;
wire [263:263] mux_1level_tapbuf_size2_243_configbus0_b;
assign mux_1level_tapbuf_size2_243_configbus0_b[263:263] = sram_blwl_blb[263:263] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_243_ (mux_1level_tapbuf_size2_243_inbus, chany_1__1__out_86_ , mux_1level_tapbuf_size2_243_sram_blwl_out[263:263] ,
mux_1level_tapbuf_size2_243_sram_blwl_outb[263:263] );
//----- SRAM bits for MUX[243], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_263_ (mux_1level_tapbuf_size2_243_sram_blwl_out[263:263] ,mux_1level_tapbuf_size2_243_sram_blwl_out[263:263] ,mux_1level_tapbuf_size2_243_sram_blwl_outb[263:263] ,mux_1level_tapbuf_size2_243_configbus0[263:263], mux_1level_tapbuf_size2_243_configbus1[263:263] , mux_1level_tapbuf_size2_243_configbus0_b[263:263] );
wire [0:1] mux_1level_tapbuf_size2_244_inbus;
assign mux_1level_tapbuf_size2_244_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_244_inbus[1] = chanx_1__0__in_12_ ;
wire [264:264] mux_1level_tapbuf_size2_244_configbus0;
wire [264:264] mux_1level_tapbuf_size2_244_configbus1;
wire [264:264] mux_1level_tapbuf_size2_244_sram_blwl_out ;
wire [264:264] mux_1level_tapbuf_size2_244_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_244_configbus0[264:264] = sram_blwl_bl[264:264] ;
assign mux_1level_tapbuf_size2_244_configbus1[264:264] = sram_blwl_wl[264:264] ;
wire [264:264] mux_1level_tapbuf_size2_244_configbus0_b;
assign mux_1level_tapbuf_size2_244_configbus0_b[264:264] = sram_blwl_blb[264:264] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_244_ (mux_1level_tapbuf_size2_244_inbus, chany_1__1__out_88_ , mux_1level_tapbuf_size2_244_sram_blwl_out[264:264] ,
mux_1level_tapbuf_size2_244_sram_blwl_outb[264:264] );
//----- SRAM bits for MUX[244], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_264_ (mux_1level_tapbuf_size2_244_sram_blwl_out[264:264] ,mux_1level_tapbuf_size2_244_sram_blwl_out[264:264] ,mux_1level_tapbuf_size2_244_sram_blwl_outb[264:264] ,mux_1level_tapbuf_size2_244_configbus0[264:264], mux_1level_tapbuf_size2_244_configbus1[264:264] , mux_1level_tapbuf_size2_244_configbus0_b[264:264] );
wire [0:1] mux_1level_tapbuf_size2_245_inbus;
assign mux_1level_tapbuf_size2_245_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_245_inbus[1] = chanx_1__0__in_10_ ;
wire [265:265] mux_1level_tapbuf_size2_245_configbus0;
wire [265:265] mux_1level_tapbuf_size2_245_configbus1;
wire [265:265] mux_1level_tapbuf_size2_245_sram_blwl_out ;
wire [265:265] mux_1level_tapbuf_size2_245_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_245_configbus0[265:265] = sram_blwl_bl[265:265] ;
assign mux_1level_tapbuf_size2_245_configbus1[265:265] = sram_blwl_wl[265:265] ;
wire [265:265] mux_1level_tapbuf_size2_245_configbus0_b;
assign mux_1level_tapbuf_size2_245_configbus0_b[265:265] = sram_blwl_blb[265:265] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_245_ (mux_1level_tapbuf_size2_245_inbus, chany_1__1__out_90_ , mux_1level_tapbuf_size2_245_sram_blwl_out[265:265] ,
mux_1level_tapbuf_size2_245_sram_blwl_outb[265:265] );
//----- SRAM bits for MUX[245], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_265_ (mux_1level_tapbuf_size2_245_sram_blwl_out[265:265] ,mux_1level_tapbuf_size2_245_sram_blwl_out[265:265] ,mux_1level_tapbuf_size2_245_sram_blwl_outb[265:265] ,mux_1level_tapbuf_size2_245_configbus0[265:265], mux_1level_tapbuf_size2_245_configbus1[265:265] , mux_1level_tapbuf_size2_245_configbus0_b[265:265] );
wire [0:1] mux_1level_tapbuf_size2_246_inbus;
assign mux_1level_tapbuf_size2_246_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_246_inbus[1] = chanx_1__0__in_8_ ;
wire [266:266] mux_1level_tapbuf_size2_246_configbus0;
wire [266:266] mux_1level_tapbuf_size2_246_configbus1;
wire [266:266] mux_1level_tapbuf_size2_246_sram_blwl_out ;
wire [266:266] mux_1level_tapbuf_size2_246_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_246_configbus0[266:266] = sram_blwl_bl[266:266] ;
assign mux_1level_tapbuf_size2_246_configbus1[266:266] = sram_blwl_wl[266:266] ;
wire [266:266] mux_1level_tapbuf_size2_246_configbus0_b;
assign mux_1level_tapbuf_size2_246_configbus0_b[266:266] = sram_blwl_blb[266:266] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_246_ (mux_1level_tapbuf_size2_246_inbus, chany_1__1__out_92_ , mux_1level_tapbuf_size2_246_sram_blwl_out[266:266] ,
mux_1level_tapbuf_size2_246_sram_blwl_outb[266:266] );
//----- SRAM bits for MUX[246], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_266_ (mux_1level_tapbuf_size2_246_sram_blwl_out[266:266] ,mux_1level_tapbuf_size2_246_sram_blwl_out[266:266] ,mux_1level_tapbuf_size2_246_sram_blwl_outb[266:266] ,mux_1level_tapbuf_size2_246_configbus0[266:266], mux_1level_tapbuf_size2_246_configbus1[266:266] , mux_1level_tapbuf_size2_246_configbus0_b[266:266] );
wire [0:1] mux_1level_tapbuf_size2_247_inbus;
assign mux_1level_tapbuf_size2_247_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_247_inbus[1] = chanx_1__0__in_6_ ;
wire [267:267] mux_1level_tapbuf_size2_247_configbus0;
wire [267:267] mux_1level_tapbuf_size2_247_configbus1;
wire [267:267] mux_1level_tapbuf_size2_247_sram_blwl_out ;
wire [267:267] mux_1level_tapbuf_size2_247_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_247_configbus0[267:267] = sram_blwl_bl[267:267] ;
assign mux_1level_tapbuf_size2_247_configbus1[267:267] = sram_blwl_wl[267:267] ;
wire [267:267] mux_1level_tapbuf_size2_247_configbus0_b;
assign mux_1level_tapbuf_size2_247_configbus0_b[267:267] = sram_blwl_blb[267:267] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_247_ (mux_1level_tapbuf_size2_247_inbus, chany_1__1__out_94_ , mux_1level_tapbuf_size2_247_sram_blwl_out[267:267] ,
mux_1level_tapbuf_size2_247_sram_blwl_outb[267:267] );
//----- SRAM bits for MUX[247], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_267_ (mux_1level_tapbuf_size2_247_sram_blwl_out[267:267] ,mux_1level_tapbuf_size2_247_sram_blwl_out[267:267] ,mux_1level_tapbuf_size2_247_sram_blwl_outb[267:267] ,mux_1level_tapbuf_size2_247_configbus0[267:267], mux_1level_tapbuf_size2_247_configbus1[267:267] , mux_1level_tapbuf_size2_247_configbus0_b[267:267] );
wire [0:1] mux_1level_tapbuf_size2_248_inbus;
assign mux_1level_tapbuf_size2_248_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_248_inbus[1] = chanx_1__0__in_4_ ;
wire [268:268] mux_1level_tapbuf_size2_248_configbus0;
wire [268:268] mux_1level_tapbuf_size2_248_configbus1;
wire [268:268] mux_1level_tapbuf_size2_248_sram_blwl_out ;
wire [268:268] mux_1level_tapbuf_size2_248_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_248_configbus0[268:268] = sram_blwl_bl[268:268] ;
assign mux_1level_tapbuf_size2_248_configbus1[268:268] = sram_blwl_wl[268:268] ;
wire [268:268] mux_1level_tapbuf_size2_248_configbus0_b;
assign mux_1level_tapbuf_size2_248_configbus0_b[268:268] = sram_blwl_blb[268:268] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_248_ (mux_1level_tapbuf_size2_248_inbus, chany_1__1__out_96_ , mux_1level_tapbuf_size2_248_sram_blwl_out[268:268] ,
mux_1level_tapbuf_size2_248_sram_blwl_outb[268:268] );
//----- SRAM bits for MUX[248], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_268_ (mux_1level_tapbuf_size2_248_sram_blwl_out[268:268] ,mux_1level_tapbuf_size2_248_sram_blwl_out[268:268] ,mux_1level_tapbuf_size2_248_sram_blwl_outb[268:268] ,mux_1level_tapbuf_size2_248_configbus0[268:268], mux_1level_tapbuf_size2_248_configbus1[268:268] , mux_1level_tapbuf_size2_248_configbus0_b[268:268] );
wire [0:1] mux_1level_tapbuf_size2_249_inbus;
assign mux_1level_tapbuf_size2_249_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_249_inbus[1] = chanx_1__0__in_2_ ;
wire [269:269] mux_1level_tapbuf_size2_249_configbus0;
wire [269:269] mux_1level_tapbuf_size2_249_configbus1;
wire [269:269] mux_1level_tapbuf_size2_249_sram_blwl_out ;
wire [269:269] mux_1level_tapbuf_size2_249_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_249_configbus0[269:269] = sram_blwl_bl[269:269] ;
assign mux_1level_tapbuf_size2_249_configbus1[269:269] = sram_blwl_wl[269:269] ;
wire [269:269] mux_1level_tapbuf_size2_249_configbus0_b;
assign mux_1level_tapbuf_size2_249_configbus0_b[269:269] = sram_blwl_blb[269:269] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_249_ (mux_1level_tapbuf_size2_249_inbus, chany_1__1__out_98_ , mux_1level_tapbuf_size2_249_sram_blwl_out[269:269] ,
mux_1level_tapbuf_size2_249_sram_blwl_outb[269:269] );
//----- SRAM bits for MUX[249], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_269_ (mux_1level_tapbuf_size2_249_sram_blwl_out[269:269] ,mux_1level_tapbuf_size2_249_sram_blwl_out[269:269] ,mux_1level_tapbuf_size2_249_sram_blwl_outb[269:269] ,mux_1level_tapbuf_size2_249_configbus0[269:269], mux_1level_tapbuf_size2_249_configbus1[269:269] , mux_1level_tapbuf_size2_249_configbus0_b[269:269] );
//----- right side Multiplexers -----
//----- bottom side Multiplexers -----
//----- left side Multiplexers -----
wire [0:1] mux_1level_tapbuf_size2_250_inbus;
assign mux_1level_tapbuf_size2_250_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_250_inbus[1] = chany_1__1__in_1_ ;
wire [270:270] mux_1level_tapbuf_size2_250_configbus0;
wire [270:270] mux_1level_tapbuf_size2_250_configbus1;
wire [270:270] mux_1level_tapbuf_size2_250_sram_blwl_out ;
wire [270:270] mux_1level_tapbuf_size2_250_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_250_configbus0[270:270] = sram_blwl_bl[270:270] ;
assign mux_1level_tapbuf_size2_250_configbus1[270:270] = sram_blwl_wl[270:270] ;
wire [270:270] mux_1level_tapbuf_size2_250_configbus0_b;
assign mux_1level_tapbuf_size2_250_configbus0_b[270:270] = sram_blwl_blb[270:270] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_250_ (mux_1level_tapbuf_size2_250_inbus, chanx_1__0__out_1_ , mux_1level_tapbuf_size2_250_sram_blwl_out[270:270] ,
mux_1level_tapbuf_size2_250_sram_blwl_outb[270:270] );
//----- SRAM bits for MUX[250], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_270_ (mux_1level_tapbuf_size2_250_sram_blwl_out[270:270] ,mux_1level_tapbuf_size2_250_sram_blwl_out[270:270] ,mux_1level_tapbuf_size2_250_sram_blwl_outb[270:270] ,mux_1level_tapbuf_size2_250_configbus0[270:270], mux_1level_tapbuf_size2_250_configbus1[270:270] , mux_1level_tapbuf_size2_250_configbus0_b[270:270] );
wire [0:1] mux_1level_tapbuf_size2_251_inbus;
assign mux_1level_tapbuf_size2_251_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_251_inbus[1] = chany_1__1__in_99_ ;
wire [271:271] mux_1level_tapbuf_size2_251_configbus0;
wire [271:271] mux_1level_tapbuf_size2_251_configbus1;
wire [271:271] mux_1level_tapbuf_size2_251_sram_blwl_out ;
wire [271:271] mux_1level_tapbuf_size2_251_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_251_configbus0[271:271] = sram_blwl_bl[271:271] ;
assign mux_1level_tapbuf_size2_251_configbus1[271:271] = sram_blwl_wl[271:271] ;
wire [271:271] mux_1level_tapbuf_size2_251_configbus0_b;
assign mux_1level_tapbuf_size2_251_configbus0_b[271:271] = sram_blwl_blb[271:271] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_251_ (mux_1level_tapbuf_size2_251_inbus, chanx_1__0__out_3_ , mux_1level_tapbuf_size2_251_sram_blwl_out[271:271] ,
mux_1level_tapbuf_size2_251_sram_blwl_outb[271:271] );
//----- SRAM bits for MUX[251], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_271_ (mux_1level_tapbuf_size2_251_sram_blwl_out[271:271] ,mux_1level_tapbuf_size2_251_sram_blwl_out[271:271] ,mux_1level_tapbuf_size2_251_sram_blwl_outb[271:271] ,mux_1level_tapbuf_size2_251_configbus0[271:271], mux_1level_tapbuf_size2_251_configbus1[271:271] , mux_1level_tapbuf_size2_251_configbus0_b[271:271] );
wire [0:1] mux_1level_tapbuf_size2_252_inbus;
assign mux_1level_tapbuf_size2_252_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_252_inbus[1] = chany_1__1__in_97_ ;
wire [272:272] mux_1level_tapbuf_size2_252_configbus0;
wire [272:272] mux_1level_tapbuf_size2_252_configbus1;
wire [272:272] mux_1level_tapbuf_size2_252_sram_blwl_out ;
wire [272:272] mux_1level_tapbuf_size2_252_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_252_configbus0[272:272] = sram_blwl_bl[272:272] ;
assign mux_1level_tapbuf_size2_252_configbus1[272:272] = sram_blwl_wl[272:272] ;
wire [272:272] mux_1level_tapbuf_size2_252_configbus0_b;
assign mux_1level_tapbuf_size2_252_configbus0_b[272:272] = sram_blwl_blb[272:272] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_252_ (mux_1level_tapbuf_size2_252_inbus, chanx_1__0__out_5_ , mux_1level_tapbuf_size2_252_sram_blwl_out[272:272] ,
mux_1level_tapbuf_size2_252_sram_blwl_outb[272:272] );
//----- SRAM bits for MUX[252], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_272_ (mux_1level_tapbuf_size2_252_sram_blwl_out[272:272] ,mux_1level_tapbuf_size2_252_sram_blwl_out[272:272] ,mux_1level_tapbuf_size2_252_sram_blwl_outb[272:272] ,mux_1level_tapbuf_size2_252_configbus0[272:272], mux_1level_tapbuf_size2_252_configbus1[272:272] , mux_1level_tapbuf_size2_252_configbus0_b[272:272] );
wire [0:1] mux_1level_tapbuf_size2_253_inbus;
assign mux_1level_tapbuf_size2_253_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_253_inbus[1] = chany_1__1__in_95_ ;
wire [273:273] mux_1level_tapbuf_size2_253_configbus0;
wire [273:273] mux_1level_tapbuf_size2_253_configbus1;
wire [273:273] mux_1level_tapbuf_size2_253_sram_blwl_out ;
wire [273:273] mux_1level_tapbuf_size2_253_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_253_configbus0[273:273] = sram_blwl_bl[273:273] ;
assign mux_1level_tapbuf_size2_253_configbus1[273:273] = sram_blwl_wl[273:273] ;
wire [273:273] mux_1level_tapbuf_size2_253_configbus0_b;
assign mux_1level_tapbuf_size2_253_configbus0_b[273:273] = sram_blwl_blb[273:273] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_253_ (mux_1level_tapbuf_size2_253_inbus, chanx_1__0__out_7_ , mux_1level_tapbuf_size2_253_sram_blwl_out[273:273] ,
mux_1level_tapbuf_size2_253_sram_blwl_outb[273:273] );
//----- SRAM bits for MUX[253], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_273_ (mux_1level_tapbuf_size2_253_sram_blwl_out[273:273] ,mux_1level_tapbuf_size2_253_sram_blwl_out[273:273] ,mux_1level_tapbuf_size2_253_sram_blwl_outb[273:273] ,mux_1level_tapbuf_size2_253_configbus0[273:273], mux_1level_tapbuf_size2_253_configbus1[273:273] , mux_1level_tapbuf_size2_253_configbus0_b[273:273] );
wire [0:1] mux_1level_tapbuf_size2_254_inbus;
assign mux_1level_tapbuf_size2_254_inbus[0] =  grid_1__0__pin_0__0__1_;
assign mux_1level_tapbuf_size2_254_inbus[1] = chany_1__1__in_93_ ;
wire [274:274] mux_1level_tapbuf_size2_254_configbus0;
wire [274:274] mux_1level_tapbuf_size2_254_configbus1;
wire [274:274] mux_1level_tapbuf_size2_254_sram_blwl_out ;
wire [274:274] mux_1level_tapbuf_size2_254_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_254_configbus0[274:274] = sram_blwl_bl[274:274] ;
assign mux_1level_tapbuf_size2_254_configbus1[274:274] = sram_blwl_wl[274:274] ;
wire [274:274] mux_1level_tapbuf_size2_254_configbus0_b;
assign mux_1level_tapbuf_size2_254_configbus0_b[274:274] = sram_blwl_blb[274:274] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_254_ (mux_1level_tapbuf_size2_254_inbus, chanx_1__0__out_9_ , mux_1level_tapbuf_size2_254_sram_blwl_out[274:274] ,
mux_1level_tapbuf_size2_254_sram_blwl_outb[274:274] );
//----- SRAM bits for MUX[254], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_274_ (mux_1level_tapbuf_size2_254_sram_blwl_out[274:274] ,mux_1level_tapbuf_size2_254_sram_blwl_out[274:274] ,mux_1level_tapbuf_size2_254_sram_blwl_outb[274:274] ,mux_1level_tapbuf_size2_254_configbus0[274:274], mux_1level_tapbuf_size2_254_configbus1[274:274] , mux_1level_tapbuf_size2_254_configbus0_b[274:274] );
wire [0:1] mux_1level_tapbuf_size2_255_inbus;
assign mux_1level_tapbuf_size2_255_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_255_inbus[1] = chany_1__1__in_91_ ;
wire [275:275] mux_1level_tapbuf_size2_255_configbus0;
wire [275:275] mux_1level_tapbuf_size2_255_configbus1;
wire [275:275] mux_1level_tapbuf_size2_255_sram_blwl_out ;
wire [275:275] mux_1level_tapbuf_size2_255_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_255_configbus0[275:275] = sram_blwl_bl[275:275] ;
assign mux_1level_tapbuf_size2_255_configbus1[275:275] = sram_blwl_wl[275:275] ;
wire [275:275] mux_1level_tapbuf_size2_255_configbus0_b;
assign mux_1level_tapbuf_size2_255_configbus0_b[275:275] = sram_blwl_blb[275:275] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_255_ (mux_1level_tapbuf_size2_255_inbus, chanx_1__0__out_11_ , mux_1level_tapbuf_size2_255_sram_blwl_out[275:275] ,
mux_1level_tapbuf_size2_255_sram_blwl_outb[275:275] );
//----- SRAM bits for MUX[255], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_275_ (mux_1level_tapbuf_size2_255_sram_blwl_out[275:275] ,mux_1level_tapbuf_size2_255_sram_blwl_out[275:275] ,mux_1level_tapbuf_size2_255_sram_blwl_outb[275:275] ,mux_1level_tapbuf_size2_255_configbus0[275:275], mux_1level_tapbuf_size2_255_configbus1[275:275] , mux_1level_tapbuf_size2_255_configbus0_b[275:275] );
wire [0:1] mux_1level_tapbuf_size2_256_inbus;
assign mux_1level_tapbuf_size2_256_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_256_inbus[1] = chany_1__1__in_89_ ;
wire [276:276] mux_1level_tapbuf_size2_256_configbus0;
wire [276:276] mux_1level_tapbuf_size2_256_configbus1;
wire [276:276] mux_1level_tapbuf_size2_256_sram_blwl_out ;
wire [276:276] mux_1level_tapbuf_size2_256_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_256_configbus0[276:276] = sram_blwl_bl[276:276] ;
assign mux_1level_tapbuf_size2_256_configbus1[276:276] = sram_blwl_wl[276:276] ;
wire [276:276] mux_1level_tapbuf_size2_256_configbus0_b;
assign mux_1level_tapbuf_size2_256_configbus0_b[276:276] = sram_blwl_blb[276:276] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_256_ (mux_1level_tapbuf_size2_256_inbus, chanx_1__0__out_13_ , mux_1level_tapbuf_size2_256_sram_blwl_out[276:276] ,
mux_1level_tapbuf_size2_256_sram_blwl_outb[276:276] );
//----- SRAM bits for MUX[256], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_276_ (mux_1level_tapbuf_size2_256_sram_blwl_out[276:276] ,mux_1level_tapbuf_size2_256_sram_blwl_out[276:276] ,mux_1level_tapbuf_size2_256_sram_blwl_outb[276:276] ,mux_1level_tapbuf_size2_256_configbus0[276:276], mux_1level_tapbuf_size2_256_configbus1[276:276] , mux_1level_tapbuf_size2_256_configbus0_b[276:276] );
wire [0:1] mux_1level_tapbuf_size2_257_inbus;
assign mux_1level_tapbuf_size2_257_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_257_inbus[1] = chany_1__1__in_87_ ;
wire [277:277] mux_1level_tapbuf_size2_257_configbus0;
wire [277:277] mux_1level_tapbuf_size2_257_configbus1;
wire [277:277] mux_1level_tapbuf_size2_257_sram_blwl_out ;
wire [277:277] mux_1level_tapbuf_size2_257_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_257_configbus0[277:277] = sram_blwl_bl[277:277] ;
assign mux_1level_tapbuf_size2_257_configbus1[277:277] = sram_blwl_wl[277:277] ;
wire [277:277] mux_1level_tapbuf_size2_257_configbus0_b;
assign mux_1level_tapbuf_size2_257_configbus0_b[277:277] = sram_blwl_blb[277:277] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_257_ (mux_1level_tapbuf_size2_257_inbus, chanx_1__0__out_15_ , mux_1level_tapbuf_size2_257_sram_blwl_out[277:277] ,
mux_1level_tapbuf_size2_257_sram_blwl_outb[277:277] );
//----- SRAM bits for MUX[257], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_277_ (mux_1level_tapbuf_size2_257_sram_blwl_out[277:277] ,mux_1level_tapbuf_size2_257_sram_blwl_out[277:277] ,mux_1level_tapbuf_size2_257_sram_blwl_outb[277:277] ,mux_1level_tapbuf_size2_257_configbus0[277:277], mux_1level_tapbuf_size2_257_configbus1[277:277] , mux_1level_tapbuf_size2_257_configbus0_b[277:277] );
wire [0:1] mux_1level_tapbuf_size2_258_inbus;
assign mux_1level_tapbuf_size2_258_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_258_inbus[1] = chany_1__1__in_85_ ;
wire [278:278] mux_1level_tapbuf_size2_258_configbus0;
wire [278:278] mux_1level_tapbuf_size2_258_configbus1;
wire [278:278] mux_1level_tapbuf_size2_258_sram_blwl_out ;
wire [278:278] mux_1level_tapbuf_size2_258_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_258_configbus0[278:278] = sram_blwl_bl[278:278] ;
assign mux_1level_tapbuf_size2_258_configbus1[278:278] = sram_blwl_wl[278:278] ;
wire [278:278] mux_1level_tapbuf_size2_258_configbus0_b;
assign mux_1level_tapbuf_size2_258_configbus0_b[278:278] = sram_blwl_blb[278:278] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_258_ (mux_1level_tapbuf_size2_258_inbus, chanx_1__0__out_17_ , mux_1level_tapbuf_size2_258_sram_blwl_out[278:278] ,
mux_1level_tapbuf_size2_258_sram_blwl_outb[278:278] );
//----- SRAM bits for MUX[258], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_278_ (mux_1level_tapbuf_size2_258_sram_blwl_out[278:278] ,mux_1level_tapbuf_size2_258_sram_blwl_out[278:278] ,mux_1level_tapbuf_size2_258_sram_blwl_outb[278:278] ,mux_1level_tapbuf_size2_258_configbus0[278:278], mux_1level_tapbuf_size2_258_configbus1[278:278] , mux_1level_tapbuf_size2_258_configbus0_b[278:278] );
wire [0:1] mux_1level_tapbuf_size2_259_inbus;
assign mux_1level_tapbuf_size2_259_inbus[0] =  grid_1__0__pin_0__0__3_;
assign mux_1level_tapbuf_size2_259_inbus[1] = chany_1__1__in_83_ ;
wire [279:279] mux_1level_tapbuf_size2_259_configbus0;
wire [279:279] mux_1level_tapbuf_size2_259_configbus1;
wire [279:279] mux_1level_tapbuf_size2_259_sram_blwl_out ;
wire [279:279] mux_1level_tapbuf_size2_259_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_259_configbus0[279:279] = sram_blwl_bl[279:279] ;
assign mux_1level_tapbuf_size2_259_configbus1[279:279] = sram_blwl_wl[279:279] ;
wire [279:279] mux_1level_tapbuf_size2_259_configbus0_b;
assign mux_1level_tapbuf_size2_259_configbus0_b[279:279] = sram_blwl_blb[279:279] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_259_ (mux_1level_tapbuf_size2_259_inbus, chanx_1__0__out_19_ , mux_1level_tapbuf_size2_259_sram_blwl_out[279:279] ,
mux_1level_tapbuf_size2_259_sram_blwl_outb[279:279] );
//----- SRAM bits for MUX[259], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_279_ (mux_1level_tapbuf_size2_259_sram_blwl_out[279:279] ,mux_1level_tapbuf_size2_259_sram_blwl_out[279:279] ,mux_1level_tapbuf_size2_259_sram_blwl_outb[279:279] ,mux_1level_tapbuf_size2_259_configbus0[279:279], mux_1level_tapbuf_size2_259_configbus1[279:279] , mux_1level_tapbuf_size2_259_configbus0_b[279:279] );
wire [0:1] mux_1level_tapbuf_size2_260_inbus;
assign mux_1level_tapbuf_size2_260_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_260_inbus[1] = chany_1__1__in_81_ ;
wire [280:280] mux_1level_tapbuf_size2_260_configbus0;
wire [280:280] mux_1level_tapbuf_size2_260_configbus1;
wire [280:280] mux_1level_tapbuf_size2_260_sram_blwl_out ;
wire [280:280] mux_1level_tapbuf_size2_260_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_260_configbus0[280:280] = sram_blwl_bl[280:280] ;
assign mux_1level_tapbuf_size2_260_configbus1[280:280] = sram_blwl_wl[280:280] ;
wire [280:280] mux_1level_tapbuf_size2_260_configbus0_b;
assign mux_1level_tapbuf_size2_260_configbus0_b[280:280] = sram_blwl_blb[280:280] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_260_ (mux_1level_tapbuf_size2_260_inbus, chanx_1__0__out_21_ , mux_1level_tapbuf_size2_260_sram_blwl_out[280:280] ,
mux_1level_tapbuf_size2_260_sram_blwl_outb[280:280] );
//----- SRAM bits for MUX[260], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_280_ (mux_1level_tapbuf_size2_260_sram_blwl_out[280:280] ,mux_1level_tapbuf_size2_260_sram_blwl_out[280:280] ,mux_1level_tapbuf_size2_260_sram_blwl_outb[280:280] ,mux_1level_tapbuf_size2_260_configbus0[280:280], mux_1level_tapbuf_size2_260_configbus1[280:280] , mux_1level_tapbuf_size2_260_configbus0_b[280:280] );
wire [0:1] mux_1level_tapbuf_size2_261_inbus;
assign mux_1level_tapbuf_size2_261_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_261_inbus[1] = chany_1__1__in_79_ ;
wire [281:281] mux_1level_tapbuf_size2_261_configbus0;
wire [281:281] mux_1level_tapbuf_size2_261_configbus1;
wire [281:281] mux_1level_tapbuf_size2_261_sram_blwl_out ;
wire [281:281] mux_1level_tapbuf_size2_261_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_261_configbus0[281:281] = sram_blwl_bl[281:281] ;
assign mux_1level_tapbuf_size2_261_configbus1[281:281] = sram_blwl_wl[281:281] ;
wire [281:281] mux_1level_tapbuf_size2_261_configbus0_b;
assign mux_1level_tapbuf_size2_261_configbus0_b[281:281] = sram_blwl_blb[281:281] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_261_ (mux_1level_tapbuf_size2_261_inbus, chanx_1__0__out_23_ , mux_1level_tapbuf_size2_261_sram_blwl_out[281:281] ,
mux_1level_tapbuf_size2_261_sram_blwl_outb[281:281] );
//----- SRAM bits for MUX[261], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_281_ (mux_1level_tapbuf_size2_261_sram_blwl_out[281:281] ,mux_1level_tapbuf_size2_261_sram_blwl_out[281:281] ,mux_1level_tapbuf_size2_261_sram_blwl_outb[281:281] ,mux_1level_tapbuf_size2_261_configbus0[281:281], mux_1level_tapbuf_size2_261_configbus1[281:281] , mux_1level_tapbuf_size2_261_configbus0_b[281:281] );
wire [0:1] mux_1level_tapbuf_size2_262_inbus;
assign mux_1level_tapbuf_size2_262_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_262_inbus[1] = chany_1__1__in_77_ ;
wire [282:282] mux_1level_tapbuf_size2_262_configbus0;
wire [282:282] mux_1level_tapbuf_size2_262_configbus1;
wire [282:282] mux_1level_tapbuf_size2_262_sram_blwl_out ;
wire [282:282] mux_1level_tapbuf_size2_262_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_262_configbus0[282:282] = sram_blwl_bl[282:282] ;
assign mux_1level_tapbuf_size2_262_configbus1[282:282] = sram_blwl_wl[282:282] ;
wire [282:282] mux_1level_tapbuf_size2_262_configbus0_b;
assign mux_1level_tapbuf_size2_262_configbus0_b[282:282] = sram_blwl_blb[282:282] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_262_ (mux_1level_tapbuf_size2_262_inbus, chanx_1__0__out_25_ , mux_1level_tapbuf_size2_262_sram_blwl_out[282:282] ,
mux_1level_tapbuf_size2_262_sram_blwl_outb[282:282] );
//----- SRAM bits for MUX[262], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_282_ (mux_1level_tapbuf_size2_262_sram_blwl_out[282:282] ,mux_1level_tapbuf_size2_262_sram_blwl_out[282:282] ,mux_1level_tapbuf_size2_262_sram_blwl_outb[282:282] ,mux_1level_tapbuf_size2_262_configbus0[282:282], mux_1level_tapbuf_size2_262_configbus1[282:282] , mux_1level_tapbuf_size2_262_configbus0_b[282:282] );
wire [0:1] mux_1level_tapbuf_size2_263_inbus;
assign mux_1level_tapbuf_size2_263_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_263_inbus[1] = chany_1__1__in_75_ ;
wire [283:283] mux_1level_tapbuf_size2_263_configbus0;
wire [283:283] mux_1level_tapbuf_size2_263_configbus1;
wire [283:283] mux_1level_tapbuf_size2_263_sram_blwl_out ;
wire [283:283] mux_1level_tapbuf_size2_263_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_263_configbus0[283:283] = sram_blwl_bl[283:283] ;
assign mux_1level_tapbuf_size2_263_configbus1[283:283] = sram_blwl_wl[283:283] ;
wire [283:283] mux_1level_tapbuf_size2_263_configbus0_b;
assign mux_1level_tapbuf_size2_263_configbus0_b[283:283] = sram_blwl_blb[283:283] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_263_ (mux_1level_tapbuf_size2_263_inbus, chanx_1__0__out_27_ , mux_1level_tapbuf_size2_263_sram_blwl_out[283:283] ,
mux_1level_tapbuf_size2_263_sram_blwl_outb[283:283] );
//----- SRAM bits for MUX[263], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_283_ (mux_1level_tapbuf_size2_263_sram_blwl_out[283:283] ,mux_1level_tapbuf_size2_263_sram_blwl_out[283:283] ,mux_1level_tapbuf_size2_263_sram_blwl_outb[283:283] ,mux_1level_tapbuf_size2_263_configbus0[283:283], mux_1level_tapbuf_size2_263_configbus1[283:283] , mux_1level_tapbuf_size2_263_configbus0_b[283:283] );
wire [0:1] mux_1level_tapbuf_size2_264_inbus;
assign mux_1level_tapbuf_size2_264_inbus[0] =  grid_1__0__pin_0__0__5_;
assign mux_1level_tapbuf_size2_264_inbus[1] = chany_1__1__in_73_ ;
wire [284:284] mux_1level_tapbuf_size2_264_configbus0;
wire [284:284] mux_1level_tapbuf_size2_264_configbus1;
wire [284:284] mux_1level_tapbuf_size2_264_sram_blwl_out ;
wire [284:284] mux_1level_tapbuf_size2_264_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_264_configbus0[284:284] = sram_blwl_bl[284:284] ;
assign mux_1level_tapbuf_size2_264_configbus1[284:284] = sram_blwl_wl[284:284] ;
wire [284:284] mux_1level_tapbuf_size2_264_configbus0_b;
assign mux_1level_tapbuf_size2_264_configbus0_b[284:284] = sram_blwl_blb[284:284] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_264_ (mux_1level_tapbuf_size2_264_inbus, chanx_1__0__out_29_ , mux_1level_tapbuf_size2_264_sram_blwl_out[284:284] ,
mux_1level_tapbuf_size2_264_sram_blwl_outb[284:284] );
//----- SRAM bits for MUX[264], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_284_ (mux_1level_tapbuf_size2_264_sram_blwl_out[284:284] ,mux_1level_tapbuf_size2_264_sram_blwl_out[284:284] ,mux_1level_tapbuf_size2_264_sram_blwl_outb[284:284] ,mux_1level_tapbuf_size2_264_configbus0[284:284], mux_1level_tapbuf_size2_264_configbus1[284:284] , mux_1level_tapbuf_size2_264_configbus0_b[284:284] );
wire [0:1] mux_1level_tapbuf_size2_265_inbus;
assign mux_1level_tapbuf_size2_265_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_265_inbus[1] = chany_1__1__in_71_ ;
wire [285:285] mux_1level_tapbuf_size2_265_configbus0;
wire [285:285] mux_1level_tapbuf_size2_265_configbus1;
wire [285:285] mux_1level_tapbuf_size2_265_sram_blwl_out ;
wire [285:285] mux_1level_tapbuf_size2_265_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_265_configbus0[285:285] = sram_blwl_bl[285:285] ;
assign mux_1level_tapbuf_size2_265_configbus1[285:285] = sram_blwl_wl[285:285] ;
wire [285:285] mux_1level_tapbuf_size2_265_configbus0_b;
assign mux_1level_tapbuf_size2_265_configbus0_b[285:285] = sram_blwl_blb[285:285] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_265_ (mux_1level_tapbuf_size2_265_inbus, chanx_1__0__out_31_ , mux_1level_tapbuf_size2_265_sram_blwl_out[285:285] ,
mux_1level_tapbuf_size2_265_sram_blwl_outb[285:285] );
//----- SRAM bits for MUX[265], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_285_ (mux_1level_tapbuf_size2_265_sram_blwl_out[285:285] ,mux_1level_tapbuf_size2_265_sram_blwl_out[285:285] ,mux_1level_tapbuf_size2_265_sram_blwl_outb[285:285] ,mux_1level_tapbuf_size2_265_configbus0[285:285], mux_1level_tapbuf_size2_265_configbus1[285:285] , mux_1level_tapbuf_size2_265_configbus0_b[285:285] );
wire [0:1] mux_1level_tapbuf_size2_266_inbus;
assign mux_1level_tapbuf_size2_266_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_266_inbus[1] = chany_1__1__in_69_ ;
wire [286:286] mux_1level_tapbuf_size2_266_configbus0;
wire [286:286] mux_1level_tapbuf_size2_266_configbus1;
wire [286:286] mux_1level_tapbuf_size2_266_sram_blwl_out ;
wire [286:286] mux_1level_tapbuf_size2_266_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_266_configbus0[286:286] = sram_blwl_bl[286:286] ;
assign mux_1level_tapbuf_size2_266_configbus1[286:286] = sram_blwl_wl[286:286] ;
wire [286:286] mux_1level_tapbuf_size2_266_configbus0_b;
assign mux_1level_tapbuf_size2_266_configbus0_b[286:286] = sram_blwl_blb[286:286] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_266_ (mux_1level_tapbuf_size2_266_inbus, chanx_1__0__out_33_ , mux_1level_tapbuf_size2_266_sram_blwl_out[286:286] ,
mux_1level_tapbuf_size2_266_sram_blwl_outb[286:286] );
//----- SRAM bits for MUX[266], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_286_ (mux_1level_tapbuf_size2_266_sram_blwl_out[286:286] ,mux_1level_tapbuf_size2_266_sram_blwl_out[286:286] ,mux_1level_tapbuf_size2_266_sram_blwl_outb[286:286] ,mux_1level_tapbuf_size2_266_configbus0[286:286], mux_1level_tapbuf_size2_266_configbus1[286:286] , mux_1level_tapbuf_size2_266_configbus0_b[286:286] );
wire [0:1] mux_1level_tapbuf_size2_267_inbus;
assign mux_1level_tapbuf_size2_267_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_267_inbus[1] = chany_1__1__in_67_ ;
wire [287:287] mux_1level_tapbuf_size2_267_configbus0;
wire [287:287] mux_1level_tapbuf_size2_267_configbus1;
wire [287:287] mux_1level_tapbuf_size2_267_sram_blwl_out ;
wire [287:287] mux_1level_tapbuf_size2_267_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_267_configbus0[287:287] = sram_blwl_bl[287:287] ;
assign mux_1level_tapbuf_size2_267_configbus1[287:287] = sram_blwl_wl[287:287] ;
wire [287:287] mux_1level_tapbuf_size2_267_configbus0_b;
assign mux_1level_tapbuf_size2_267_configbus0_b[287:287] = sram_blwl_blb[287:287] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_267_ (mux_1level_tapbuf_size2_267_inbus, chanx_1__0__out_35_ , mux_1level_tapbuf_size2_267_sram_blwl_out[287:287] ,
mux_1level_tapbuf_size2_267_sram_blwl_outb[287:287] );
//----- SRAM bits for MUX[267], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_287_ (mux_1level_tapbuf_size2_267_sram_blwl_out[287:287] ,mux_1level_tapbuf_size2_267_sram_blwl_out[287:287] ,mux_1level_tapbuf_size2_267_sram_blwl_outb[287:287] ,mux_1level_tapbuf_size2_267_configbus0[287:287], mux_1level_tapbuf_size2_267_configbus1[287:287] , mux_1level_tapbuf_size2_267_configbus0_b[287:287] );
wire [0:1] mux_1level_tapbuf_size2_268_inbus;
assign mux_1level_tapbuf_size2_268_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_268_inbus[1] = chany_1__1__in_65_ ;
wire [288:288] mux_1level_tapbuf_size2_268_configbus0;
wire [288:288] mux_1level_tapbuf_size2_268_configbus1;
wire [288:288] mux_1level_tapbuf_size2_268_sram_blwl_out ;
wire [288:288] mux_1level_tapbuf_size2_268_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_268_configbus0[288:288] = sram_blwl_bl[288:288] ;
assign mux_1level_tapbuf_size2_268_configbus1[288:288] = sram_blwl_wl[288:288] ;
wire [288:288] mux_1level_tapbuf_size2_268_configbus0_b;
assign mux_1level_tapbuf_size2_268_configbus0_b[288:288] = sram_blwl_blb[288:288] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_268_ (mux_1level_tapbuf_size2_268_inbus, chanx_1__0__out_37_ , mux_1level_tapbuf_size2_268_sram_blwl_out[288:288] ,
mux_1level_tapbuf_size2_268_sram_blwl_outb[288:288] );
//----- SRAM bits for MUX[268], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_288_ (mux_1level_tapbuf_size2_268_sram_blwl_out[288:288] ,mux_1level_tapbuf_size2_268_sram_blwl_out[288:288] ,mux_1level_tapbuf_size2_268_sram_blwl_outb[288:288] ,mux_1level_tapbuf_size2_268_configbus0[288:288], mux_1level_tapbuf_size2_268_configbus1[288:288] , mux_1level_tapbuf_size2_268_configbus0_b[288:288] );
wire [0:1] mux_1level_tapbuf_size2_269_inbus;
assign mux_1level_tapbuf_size2_269_inbus[0] =  grid_1__0__pin_0__0__7_;
assign mux_1level_tapbuf_size2_269_inbus[1] = chany_1__1__in_63_ ;
wire [289:289] mux_1level_tapbuf_size2_269_configbus0;
wire [289:289] mux_1level_tapbuf_size2_269_configbus1;
wire [289:289] mux_1level_tapbuf_size2_269_sram_blwl_out ;
wire [289:289] mux_1level_tapbuf_size2_269_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_269_configbus0[289:289] = sram_blwl_bl[289:289] ;
assign mux_1level_tapbuf_size2_269_configbus1[289:289] = sram_blwl_wl[289:289] ;
wire [289:289] mux_1level_tapbuf_size2_269_configbus0_b;
assign mux_1level_tapbuf_size2_269_configbus0_b[289:289] = sram_blwl_blb[289:289] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_269_ (mux_1level_tapbuf_size2_269_inbus, chanx_1__0__out_39_ , mux_1level_tapbuf_size2_269_sram_blwl_out[289:289] ,
mux_1level_tapbuf_size2_269_sram_blwl_outb[289:289] );
//----- SRAM bits for MUX[269], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_289_ (mux_1level_tapbuf_size2_269_sram_blwl_out[289:289] ,mux_1level_tapbuf_size2_269_sram_blwl_out[289:289] ,mux_1level_tapbuf_size2_269_sram_blwl_outb[289:289] ,mux_1level_tapbuf_size2_269_configbus0[289:289], mux_1level_tapbuf_size2_269_configbus1[289:289] , mux_1level_tapbuf_size2_269_configbus0_b[289:289] );
wire [0:1] mux_1level_tapbuf_size2_270_inbus;
assign mux_1level_tapbuf_size2_270_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_270_inbus[1] = chany_1__1__in_61_ ;
wire [290:290] mux_1level_tapbuf_size2_270_configbus0;
wire [290:290] mux_1level_tapbuf_size2_270_configbus1;
wire [290:290] mux_1level_tapbuf_size2_270_sram_blwl_out ;
wire [290:290] mux_1level_tapbuf_size2_270_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_270_configbus0[290:290] = sram_blwl_bl[290:290] ;
assign mux_1level_tapbuf_size2_270_configbus1[290:290] = sram_blwl_wl[290:290] ;
wire [290:290] mux_1level_tapbuf_size2_270_configbus0_b;
assign mux_1level_tapbuf_size2_270_configbus0_b[290:290] = sram_blwl_blb[290:290] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_270_ (mux_1level_tapbuf_size2_270_inbus, chanx_1__0__out_41_ , mux_1level_tapbuf_size2_270_sram_blwl_out[290:290] ,
mux_1level_tapbuf_size2_270_sram_blwl_outb[290:290] );
//----- SRAM bits for MUX[270], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_290_ (mux_1level_tapbuf_size2_270_sram_blwl_out[290:290] ,mux_1level_tapbuf_size2_270_sram_blwl_out[290:290] ,mux_1level_tapbuf_size2_270_sram_blwl_outb[290:290] ,mux_1level_tapbuf_size2_270_configbus0[290:290], mux_1level_tapbuf_size2_270_configbus1[290:290] , mux_1level_tapbuf_size2_270_configbus0_b[290:290] );
wire [0:1] mux_1level_tapbuf_size2_271_inbus;
assign mux_1level_tapbuf_size2_271_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_271_inbus[1] = chany_1__1__in_59_ ;
wire [291:291] mux_1level_tapbuf_size2_271_configbus0;
wire [291:291] mux_1level_tapbuf_size2_271_configbus1;
wire [291:291] mux_1level_tapbuf_size2_271_sram_blwl_out ;
wire [291:291] mux_1level_tapbuf_size2_271_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_271_configbus0[291:291] = sram_blwl_bl[291:291] ;
assign mux_1level_tapbuf_size2_271_configbus1[291:291] = sram_blwl_wl[291:291] ;
wire [291:291] mux_1level_tapbuf_size2_271_configbus0_b;
assign mux_1level_tapbuf_size2_271_configbus0_b[291:291] = sram_blwl_blb[291:291] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_271_ (mux_1level_tapbuf_size2_271_inbus, chanx_1__0__out_43_ , mux_1level_tapbuf_size2_271_sram_blwl_out[291:291] ,
mux_1level_tapbuf_size2_271_sram_blwl_outb[291:291] );
//----- SRAM bits for MUX[271], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_291_ (mux_1level_tapbuf_size2_271_sram_blwl_out[291:291] ,mux_1level_tapbuf_size2_271_sram_blwl_out[291:291] ,mux_1level_tapbuf_size2_271_sram_blwl_outb[291:291] ,mux_1level_tapbuf_size2_271_configbus0[291:291], mux_1level_tapbuf_size2_271_configbus1[291:291] , mux_1level_tapbuf_size2_271_configbus0_b[291:291] );
wire [0:1] mux_1level_tapbuf_size2_272_inbus;
assign mux_1level_tapbuf_size2_272_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_272_inbus[1] = chany_1__1__in_57_ ;
wire [292:292] mux_1level_tapbuf_size2_272_configbus0;
wire [292:292] mux_1level_tapbuf_size2_272_configbus1;
wire [292:292] mux_1level_tapbuf_size2_272_sram_blwl_out ;
wire [292:292] mux_1level_tapbuf_size2_272_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_272_configbus0[292:292] = sram_blwl_bl[292:292] ;
assign mux_1level_tapbuf_size2_272_configbus1[292:292] = sram_blwl_wl[292:292] ;
wire [292:292] mux_1level_tapbuf_size2_272_configbus0_b;
assign mux_1level_tapbuf_size2_272_configbus0_b[292:292] = sram_blwl_blb[292:292] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_272_ (mux_1level_tapbuf_size2_272_inbus, chanx_1__0__out_45_ , mux_1level_tapbuf_size2_272_sram_blwl_out[292:292] ,
mux_1level_tapbuf_size2_272_sram_blwl_outb[292:292] );
//----- SRAM bits for MUX[272], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_292_ (mux_1level_tapbuf_size2_272_sram_blwl_out[292:292] ,mux_1level_tapbuf_size2_272_sram_blwl_out[292:292] ,mux_1level_tapbuf_size2_272_sram_blwl_outb[292:292] ,mux_1level_tapbuf_size2_272_configbus0[292:292], mux_1level_tapbuf_size2_272_configbus1[292:292] , mux_1level_tapbuf_size2_272_configbus0_b[292:292] );
wire [0:1] mux_1level_tapbuf_size2_273_inbus;
assign mux_1level_tapbuf_size2_273_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_273_inbus[1] = chany_1__1__in_55_ ;
wire [293:293] mux_1level_tapbuf_size2_273_configbus0;
wire [293:293] mux_1level_tapbuf_size2_273_configbus1;
wire [293:293] mux_1level_tapbuf_size2_273_sram_blwl_out ;
wire [293:293] mux_1level_tapbuf_size2_273_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_273_configbus0[293:293] = sram_blwl_bl[293:293] ;
assign mux_1level_tapbuf_size2_273_configbus1[293:293] = sram_blwl_wl[293:293] ;
wire [293:293] mux_1level_tapbuf_size2_273_configbus0_b;
assign mux_1level_tapbuf_size2_273_configbus0_b[293:293] = sram_blwl_blb[293:293] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_273_ (mux_1level_tapbuf_size2_273_inbus, chanx_1__0__out_47_ , mux_1level_tapbuf_size2_273_sram_blwl_out[293:293] ,
mux_1level_tapbuf_size2_273_sram_blwl_outb[293:293] );
//----- SRAM bits for MUX[273], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_293_ (mux_1level_tapbuf_size2_273_sram_blwl_out[293:293] ,mux_1level_tapbuf_size2_273_sram_blwl_out[293:293] ,mux_1level_tapbuf_size2_273_sram_blwl_outb[293:293] ,mux_1level_tapbuf_size2_273_configbus0[293:293], mux_1level_tapbuf_size2_273_configbus1[293:293] , mux_1level_tapbuf_size2_273_configbus0_b[293:293] );
wire [0:1] mux_1level_tapbuf_size2_274_inbus;
assign mux_1level_tapbuf_size2_274_inbus[0] =  grid_1__0__pin_0__0__9_;
assign mux_1level_tapbuf_size2_274_inbus[1] = chany_1__1__in_53_ ;
wire [294:294] mux_1level_tapbuf_size2_274_configbus0;
wire [294:294] mux_1level_tapbuf_size2_274_configbus1;
wire [294:294] mux_1level_tapbuf_size2_274_sram_blwl_out ;
wire [294:294] mux_1level_tapbuf_size2_274_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_274_configbus0[294:294] = sram_blwl_bl[294:294] ;
assign mux_1level_tapbuf_size2_274_configbus1[294:294] = sram_blwl_wl[294:294] ;
wire [294:294] mux_1level_tapbuf_size2_274_configbus0_b;
assign mux_1level_tapbuf_size2_274_configbus0_b[294:294] = sram_blwl_blb[294:294] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_274_ (mux_1level_tapbuf_size2_274_inbus, chanx_1__0__out_49_ , mux_1level_tapbuf_size2_274_sram_blwl_out[294:294] ,
mux_1level_tapbuf_size2_274_sram_blwl_outb[294:294] );
//----- SRAM bits for MUX[274], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_294_ (mux_1level_tapbuf_size2_274_sram_blwl_out[294:294] ,mux_1level_tapbuf_size2_274_sram_blwl_out[294:294] ,mux_1level_tapbuf_size2_274_sram_blwl_outb[294:294] ,mux_1level_tapbuf_size2_274_configbus0[294:294], mux_1level_tapbuf_size2_274_configbus1[294:294] , mux_1level_tapbuf_size2_274_configbus0_b[294:294] );
wire [0:1] mux_1level_tapbuf_size2_275_inbus;
assign mux_1level_tapbuf_size2_275_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_275_inbus[1] = chany_1__1__in_51_ ;
wire [295:295] mux_1level_tapbuf_size2_275_configbus0;
wire [295:295] mux_1level_tapbuf_size2_275_configbus1;
wire [295:295] mux_1level_tapbuf_size2_275_sram_blwl_out ;
wire [295:295] mux_1level_tapbuf_size2_275_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_275_configbus0[295:295] = sram_blwl_bl[295:295] ;
assign mux_1level_tapbuf_size2_275_configbus1[295:295] = sram_blwl_wl[295:295] ;
wire [295:295] mux_1level_tapbuf_size2_275_configbus0_b;
assign mux_1level_tapbuf_size2_275_configbus0_b[295:295] = sram_blwl_blb[295:295] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_275_ (mux_1level_tapbuf_size2_275_inbus, chanx_1__0__out_51_ , mux_1level_tapbuf_size2_275_sram_blwl_out[295:295] ,
mux_1level_tapbuf_size2_275_sram_blwl_outb[295:295] );
//----- SRAM bits for MUX[275], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_295_ (mux_1level_tapbuf_size2_275_sram_blwl_out[295:295] ,mux_1level_tapbuf_size2_275_sram_blwl_out[295:295] ,mux_1level_tapbuf_size2_275_sram_blwl_outb[295:295] ,mux_1level_tapbuf_size2_275_configbus0[295:295], mux_1level_tapbuf_size2_275_configbus1[295:295] , mux_1level_tapbuf_size2_275_configbus0_b[295:295] );
wire [0:1] mux_1level_tapbuf_size2_276_inbus;
assign mux_1level_tapbuf_size2_276_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_276_inbus[1] = chany_1__1__in_49_ ;
wire [296:296] mux_1level_tapbuf_size2_276_configbus0;
wire [296:296] mux_1level_tapbuf_size2_276_configbus1;
wire [296:296] mux_1level_tapbuf_size2_276_sram_blwl_out ;
wire [296:296] mux_1level_tapbuf_size2_276_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_276_configbus0[296:296] = sram_blwl_bl[296:296] ;
assign mux_1level_tapbuf_size2_276_configbus1[296:296] = sram_blwl_wl[296:296] ;
wire [296:296] mux_1level_tapbuf_size2_276_configbus0_b;
assign mux_1level_tapbuf_size2_276_configbus0_b[296:296] = sram_blwl_blb[296:296] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_276_ (mux_1level_tapbuf_size2_276_inbus, chanx_1__0__out_53_ , mux_1level_tapbuf_size2_276_sram_blwl_out[296:296] ,
mux_1level_tapbuf_size2_276_sram_blwl_outb[296:296] );
//----- SRAM bits for MUX[276], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_296_ (mux_1level_tapbuf_size2_276_sram_blwl_out[296:296] ,mux_1level_tapbuf_size2_276_sram_blwl_out[296:296] ,mux_1level_tapbuf_size2_276_sram_blwl_outb[296:296] ,mux_1level_tapbuf_size2_276_configbus0[296:296], mux_1level_tapbuf_size2_276_configbus1[296:296] , mux_1level_tapbuf_size2_276_configbus0_b[296:296] );
wire [0:1] mux_1level_tapbuf_size2_277_inbus;
assign mux_1level_tapbuf_size2_277_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_277_inbus[1] = chany_1__1__in_47_ ;
wire [297:297] mux_1level_tapbuf_size2_277_configbus0;
wire [297:297] mux_1level_tapbuf_size2_277_configbus1;
wire [297:297] mux_1level_tapbuf_size2_277_sram_blwl_out ;
wire [297:297] mux_1level_tapbuf_size2_277_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_277_configbus0[297:297] = sram_blwl_bl[297:297] ;
assign mux_1level_tapbuf_size2_277_configbus1[297:297] = sram_blwl_wl[297:297] ;
wire [297:297] mux_1level_tapbuf_size2_277_configbus0_b;
assign mux_1level_tapbuf_size2_277_configbus0_b[297:297] = sram_blwl_blb[297:297] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_277_ (mux_1level_tapbuf_size2_277_inbus, chanx_1__0__out_55_ , mux_1level_tapbuf_size2_277_sram_blwl_out[297:297] ,
mux_1level_tapbuf_size2_277_sram_blwl_outb[297:297] );
//----- SRAM bits for MUX[277], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_297_ (mux_1level_tapbuf_size2_277_sram_blwl_out[297:297] ,mux_1level_tapbuf_size2_277_sram_blwl_out[297:297] ,mux_1level_tapbuf_size2_277_sram_blwl_outb[297:297] ,mux_1level_tapbuf_size2_277_configbus0[297:297], mux_1level_tapbuf_size2_277_configbus1[297:297] , mux_1level_tapbuf_size2_277_configbus0_b[297:297] );
wire [0:1] mux_1level_tapbuf_size2_278_inbus;
assign mux_1level_tapbuf_size2_278_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_278_inbus[1] = chany_1__1__in_45_ ;
wire [298:298] mux_1level_tapbuf_size2_278_configbus0;
wire [298:298] mux_1level_tapbuf_size2_278_configbus1;
wire [298:298] mux_1level_tapbuf_size2_278_sram_blwl_out ;
wire [298:298] mux_1level_tapbuf_size2_278_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_278_configbus0[298:298] = sram_blwl_bl[298:298] ;
assign mux_1level_tapbuf_size2_278_configbus1[298:298] = sram_blwl_wl[298:298] ;
wire [298:298] mux_1level_tapbuf_size2_278_configbus0_b;
assign mux_1level_tapbuf_size2_278_configbus0_b[298:298] = sram_blwl_blb[298:298] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_278_ (mux_1level_tapbuf_size2_278_inbus, chanx_1__0__out_57_ , mux_1level_tapbuf_size2_278_sram_blwl_out[298:298] ,
mux_1level_tapbuf_size2_278_sram_blwl_outb[298:298] );
//----- SRAM bits for MUX[278], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_298_ (mux_1level_tapbuf_size2_278_sram_blwl_out[298:298] ,mux_1level_tapbuf_size2_278_sram_blwl_out[298:298] ,mux_1level_tapbuf_size2_278_sram_blwl_outb[298:298] ,mux_1level_tapbuf_size2_278_configbus0[298:298], mux_1level_tapbuf_size2_278_configbus1[298:298] , mux_1level_tapbuf_size2_278_configbus0_b[298:298] );
wire [0:1] mux_1level_tapbuf_size2_279_inbus;
assign mux_1level_tapbuf_size2_279_inbus[0] =  grid_1__0__pin_0__0__11_;
assign mux_1level_tapbuf_size2_279_inbus[1] = chany_1__1__in_43_ ;
wire [299:299] mux_1level_tapbuf_size2_279_configbus0;
wire [299:299] mux_1level_tapbuf_size2_279_configbus1;
wire [299:299] mux_1level_tapbuf_size2_279_sram_blwl_out ;
wire [299:299] mux_1level_tapbuf_size2_279_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_279_configbus0[299:299] = sram_blwl_bl[299:299] ;
assign mux_1level_tapbuf_size2_279_configbus1[299:299] = sram_blwl_wl[299:299] ;
wire [299:299] mux_1level_tapbuf_size2_279_configbus0_b;
assign mux_1level_tapbuf_size2_279_configbus0_b[299:299] = sram_blwl_blb[299:299] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_279_ (mux_1level_tapbuf_size2_279_inbus, chanx_1__0__out_59_ , mux_1level_tapbuf_size2_279_sram_blwl_out[299:299] ,
mux_1level_tapbuf_size2_279_sram_blwl_outb[299:299] );
//----- SRAM bits for MUX[279], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_299_ (mux_1level_tapbuf_size2_279_sram_blwl_out[299:299] ,mux_1level_tapbuf_size2_279_sram_blwl_out[299:299] ,mux_1level_tapbuf_size2_279_sram_blwl_outb[299:299] ,mux_1level_tapbuf_size2_279_configbus0[299:299], mux_1level_tapbuf_size2_279_configbus1[299:299] , mux_1level_tapbuf_size2_279_configbus0_b[299:299] );
wire [0:1] mux_1level_tapbuf_size2_280_inbus;
assign mux_1level_tapbuf_size2_280_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_280_inbus[1] = chany_1__1__in_41_ ;
wire [300:300] mux_1level_tapbuf_size2_280_configbus0;
wire [300:300] mux_1level_tapbuf_size2_280_configbus1;
wire [300:300] mux_1level_tapbuf_size2_280_sram_blwl_out ;
wire [300:300] mux_1level_tapbuf_size2_280_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_280_configbus0[300:300] = sram_blwl_bl[300:300] ;
assign mux_1level_tapbuf_size2_280_configbus1[300:300] = sram_blwl_wl[300:300] ;
wire [300:300] mux_1level_tapbuf_size2_280_configbus0_b;
assign mux_1level_tapbuf_size2_280_configbus0_b[300:300] = sram_blwl_blb[300:300] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_280_ (mux_1level_tapbuf_size2_280_inbus, chanx_1__0__out_61_ , mux_1level_tapbuf_size2_280_sram_blwl_out[300:300] ,
mux_1level_tapbuf_size2_280_sram_blwl_outb[300:300] );
//----- SRAM bits for MUX[280], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_300_ (mux_1level_tapbuf_size2_280_sram_blwl_out[300:300] ,mux_1level_tapbuf_size2_280_sram_blwl_out[300:300] ,mux_1level_tapbuf_size2_280_sram_blwl_outb[300:300] ,mux_1level_tapbuf_size2_280_configbus0[300:300], mux_1level_tapbuf_size2_280_configbus1[300:300] , mux_1level_tapbuf_size2_280_configbus0_b[300:300] );
wire [0:1] mux_1level_tapbuf_size2_281_inbus;
assign mux_1level_tapbuf_size2_281_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_281_inbus[1] = chany_1__1__in_39_ ;
wire [301:301] mux_1level_tapbuf_size2_281_configbus0;
wire [301:301] mux_1level_tapbuf_size2_281_configbus1;
wire [301:301] mux_1level_tapbuf_size2_281_sram_blwl_out ;
wire [301:301] mux_1level_tapbuf_size2_281_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_281_configbus0[301:301] = sram_blwl_bl[301:301] ;
assign mux_1level_tapbuf_size2_281_configbus1[301:301] = sram_blwl_wl[301:301] ;
wire [301:301] mux_1level_tapbuf_size2_281_configbus0_b;
assign mux_1level_tapbuf_size2_281_configbus0_b[301:301] = sram_blwl_blb[301:301] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_281_ (mux_1level_tapbuf_size2_281_inbus, chanx_1__0__out_63_ , mux_1level_tapbuf_size2_281_sram_blwl_out[301:301] ,
mux_1level_tapbuf_size2_281_sram_blwl_outb[301:301] );
//----- SRAM bits for MUX[281], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_301_ (mux_1level_tapbuf_size2_281_sram_blwl_out[301:301] ,mux_1level_tapbuf_size2_281_sram_blwl_out[301:301] ,mux_1level_tapbuf_size2_281_sram_blwl_outb[301:301] ,mux_1level_tapbuf_size2_281_configbus0[301:301], mux_1level_tapbuf_size2_281_configbus1[301:301] , mux_1level_tapbuf_size2_281_configbus0_b[301:301] );
wire [0:1] mux_1level_tapbuf_size2_282_inbus;
assign mux_1level_tapbuf_size2_282_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_282_inbus[1] = chany_1__1__in_37_ ;
wire [302:302] mux_1level_tapbuf_size2_282_configbus0;
wire [302:302] mux_1level_tapbuf_size2_282_configbus1;
wire [302:302] mux_1level_tapbuf_size2_282_sram_blwl_out ;
wire [302:302] mux_1level_tapbuf_size2_282_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_282_configbus0[302:302] = sram_blwl_bl[302:302] ;
assign mux_1level_tapbuf_size2_282_configbus1[302:302] = sram_blwl_wl[302:302] ;
wire [302:302] mux_1level_tapbuf_size2_282_configbus0_b;
assign mux_1level_tapbuf_size2_282_configbus0_b[302:302] = sram_blwl_blb[302:302] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_282_ (mux_1level_tapbuf_size2_282_inbus, chanx_1__0__out_65_ , mux_1level_tapbuf_size2_282_sram_blwl_out[302:302] ,
mux_1level_tapbuf_size2_282_sram_blwl_outb[302:302] );
//----- SRAM bits for MUX[282], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_302_ (mux_1level_tapbuf_size2_282_sram_blwl_out[302:302] ,mux_1level_tapbuf_size2_282_sram_blwl_out[302:302] ,mux_1level_tapbuf_size2_282_sram_blwl_outb[302:302] ,mux_1level_tapbuf_size2_282_configbus0[302:302], mux_1level_tapbuf_size2_282_configbus1[302:302] , mux_1level_tapbuf_size2_282_configbus0_b[302:302] );
wire [0:1] mux_1level_tapbuf_size2_283_inbus;
assign mux_1level_tapbuf_size2_283_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_283_inbus[1] = chany_1__1__in_35_ ;
wire [303:303] mux_1level_tapbuf_size2_283_configbus0;
wire [303:303] mux_1level_tapbuf_size2_283_configbus1;
wire [303:303] mux_1level_tapbuf_size2_283_sram_blwl_out ;
wire [303:303] mux_1level_tapbuf_size2_283_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_283_configbus0[303:303] = sram_blwl_bl[303:303] ;
assign mux_1level_tapbuf_size2_283_configbus1[303:303] = sram_blwl_wl[303:303] ;
wire [303:303] mux_1level_tapbuf_size2_283_configbus0_b;
assign mux_1level_tapbuf_size2_283_configbus0_b[303:303] = sram_blwl_blb[303:303] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_283_ (mux_1level_tapbuf_size2_283_inbus, chanx_1__0__out_67_ , mux_1level_tapbuf_size2_283_sram_blwl_out[303:303] ,
mux_1level_tapbuf_size2_283_sram_blwl_outb[303:303] );
//----- SRAM bits for MUX[283], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_303_ (mux_1level_tapbuf_size2_283_sram_blwl_out[303:303] ,mux_1level_tapbuf_size2_283_sram_blwl_out[303:303] ,mux_1level_tapbuf_size2_283_sram_blwl_outb[303:303] ,mux_1level_tapbuf_size2_283_configbus0[303:303], mux_1level_tapbuf_size2_283_configbus1[303:303] , mux_1level_tapbuf_size2_283_configbus0_b[303:303] );
wire [0:1] mux_1level_tapbuf_size2_284_inbus;
assign mux_1level_tapbuf_size2_284_inbus[0] =  grid_1__0__pin_0__0__13_;
assign mux_1level_tapbuf_size2_284_inbus[1] = chany_1__1__in_33_ ;
wire [304:304] mux_1level_tapbuf_size2_284_configbus0;
wire [304:304] mux_1level_tapbuf_size2_284_configbus1;
wire [304:304] mux_1level_tapbuf_size2_284_sram_blwl_out ;
wire [304:304] mux_1level_tapbuf_size2_284_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_284_configbus0[304:304] = sram_blwl_bl[304:304] ;
assign mux_1level_tapbuf_size2_284_configbus1[304:304] = sram_blwl_wl[304:304] ;
wire [304:304] mux_1level_tapbuf_size2_284_configbus0_b;
assign mux_1level_tapbuf_size2_284_configbus0_b[304:304] = sram_blwl_blb[304:304] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_284_ (mux_1level_tapbuf_size2_284_inbus, chanx_1__0__out_69_ , mux_1level_tapbuf_size2_284_sram_blwl_out[304:304] ,
mux_1level_tapbuf_size2_284_sram_blwl_outb[304:304] );
//----- SRAM bits for MUX[284], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_304_ (mux_1level_tapbuf_size2_284_sram_blwl_out[304:304] ,mux_1level_tapbuf_size2_284_sram_blwl_out[304:304] ,mux_1level_tapbuf_size2_284_sram_blwl_outb[304:304] ,mux_1level_tapbuf_size2_284_configbus0[304:304], mux_1level_tapbuf_size2_284_configbus1[304:304] , mux_1level_tapbuf_size2_284_configbus0_b[304:304] );
wire [0:1] mux_1level_tapbuf_size2_285_inbus;
assign mux_1level_tapbuf_size2_285_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_285_inbus[1] = chany_1__1__in_31_ ;
wire [305:305] mux_1level_tapbuf_size2_285_configbus0;
wire [305:305] mux_1level_tapbuf_size2_285_configbus1;
wire [305:305] mux_1level_tapbuf_size2_285_sram_blwl_out ;
wire [305:305] mux_1level_tapbuf_size2_285_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_285_configbus0[305:305] = sram_blwl_bl[305:305] ;
assign mux_1level_tapbuf_size2_285_configbus1[305:305] = sram_blwl_wl[305:305] ;
wire [305:305] mux_1level_tapbuf_size2_285_configbus0_b;
assign mux_1level_tapbuf_size2_285_configbus0_b[305:305] = sram_blwl_blb[305:305] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_285_ (mux_1level_tapbuf_size2_285_inbus, chanx_1__0__out_71_ , mux_1level_tapbuf_size2_285_sram_blwl_out[305:305] ,
mux_1level_tapbuf_size2_285_sram_blwl_outb[305:305] );
//----- SRAM bits for MUX[285], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_305_ (mux_1level_tapbuf_size2_285_sram_blwl_out[305:305] ,mux_1level_tapbuf_size2_285_sram_blwl_out[305:305] ,mux_1level_tapbuf_size2_285_sram_blwl_outb[305:305] ,mux_1level_tapbuf_size2_285_configbus0[305:305], mux_1level_tapbuf_size2_285_configbus1[305:305] , mux_1level_tapbuf_size2_285_configbus0_b[305:305] );
wire [0:1] mux_1level_tapbuf_size2_286_inbus;
assign mux_1level_tapbuf_size2_286_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_286_inbus[1] = chany_1__1__in_29_ ;
wire [306:306] mux_1level_tapbuf_size2_286_configbus0;
wire [306:306] mux_1level_tapbuf_size2_286_configbus1;
wire [306:306] mux_1level_tapbuf_size2_286_sram_blwl_out ;
wire [306:306] mux_1level_tapbuf_size2_286_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_286_configbus0[306:306] = sram_blwl_bl[306:306] ;
assign mux_1level_tapbuf_size2_286_configbus1[306:306] = sram_blwl_wl[306:306] ;
wire [306:306] mux_1level_tapbuf_size2_286_configbus0_b;
assign mux_1level_tapbuf_size2_286_configbus0_b[306:306] = sram_blwl_blb[306:306] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_286_ (mux_1level_tapbuf_size2_286_inbus, chanx_1__0__out_73_ , mux_1level_tapbuf_size2_286_sram_blwl_out[306:306] ,
mux_1level_tapbuf_size2_286_sram_blwl_outb[306:306] );
//----- SRAM bits for MUX[286], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_306_ (mux_1level_tapbuf_size2_286_sram_blwl_out[306:306] ,mux_1level_tapbuf_size2_286_sram_blwl_out[306:306] ,mux_1level_tapbuf_size2_286_sram_blwl_outb[306:306] ,mux_1level_tapbuf_size2_286_configbus0[306:306], mux_1level_tapbuf_size2_286_configbus1[306:306] , mux_1level_tapbuf_size2_286_configbus0_b[306:306] );
wire [0:1] mux_1level_tapbuf_size2_287_inbus;
assign mux_1level_tapbuf_size2_287_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_287_inbus[1] = chany_1__1__in_27_ ;
wire [307:307] mux_1level_tapbuf_size2_287_configbus0;
wire [307:307] mux_1level_tapbuf_size2_287_configbus1;
wire [307:307] mux_1level_tapbuf_size2_287_sram_blwl_out ;
wire [307:307] mux_1level_tapbuf_size2_287_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_287_configbus0[307:307] = sram_blwl_bl[307:307] ;
assign mux_1level_tapbuf_size2_287_configbus1[307:307] = sram_blwl_wl[307:307] ;
wire [307:307] mux_1level_tapbuf_size2_287_configbus0_b;
assign mux_1level_tapbuf_size2_287_configbus0_b[307:307] = sram_blwl_blb[307:307] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_287_ (mux_1level_tapbuf_size2_287_inbus, chanx_1__0__out_75_ , mux_1level_tapbuf_size2_287_sram_blwl_out[307:307] ,
mux_1level_tapbuf_size2_287_sram_blwl_outb[307:307] );
//----- SRAM bits for MUX[287], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_307_ (mux_1level_tapbuf_size2_287_sram_blwl_out[307:307] ,mux_1level_tapbuf_size2_287_sram_blwl_out[307:307] ,mux_1level_tapbuf_size2_287_sram_blwl_outb[307:307] ,mux_1level_tapbuf_size2_287_configbus0[307:307], mux_1level_tapbuf_size2_287_configbus1[307:307] , mux_1level_tapbuf_size2_287_configbus0_b[307:307] );
wire [0:1] mux_1level_tapbuf_size2_288_inbus;
assign mux_1level_tapbuf_size2_288_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_288_inbus[1] = chany_1__1__in_25_ ;
wire [308:308] mux_1level_tapbuf_size2_288_configbus0;
wire [308:308] mux_1level_tapbuf_size2_288_configbus1;
wire [308:308] mux_1level_tapbuf_size2_288_sram_blwl_out ;
wire [308:308] mux_1level_tapbuf_size2_288_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_288_configbus0[308:308] = sram_blwl_bl[308:308] ;
assign mux_1level_tapbuf_size2_288_configbus1[308:308] = sram_blwl_wl[308:308] ;
wire [308:308] mux_1level_tapbuf_size2_288_configbus0_b;
assign mux_1level_tapbuf_size2_288_configbus0_b[308:308] = sram_blwl_blb[308:308] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_288_ (mux_1level_tapbuf_size2_288_inbus, chanx_1__0__out_77_ , mux_1level_tapbuf_size2_288_sram_blwl_out[308:308] ,
mux_1level_tapbuf_size2_288_sram_blwl_outb[308:308] );
//----- SRAM bits for MUX[288], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_308_ (mux_1level_tapbuf_size2_288_sram_blwl_out[308:308] ,mux_1level_tapbuf_size2_288_sram_blwl_out[308:308] ,mux_1level_tapbuf_size2_288_sram_blwl_outb[308:308] ,mux_1level_tapbuf_size2_288_configbus0[308:308], mux_1level_tapbuf_size2_288_configbus1[308:308] , mux_1level_tapbuf_size2_288_configbus0_b[308:308] );
wire [0:1] mux_1level_tapbuf_size2_289_inbus;
assign mux_1level_tapbuf_size2_289_inbus[0] =  grid_1__0__pin_0__0__15_;
assign mux_1level_tapbuf_size2_289_inbus[1] = chany_1__1__in_23_ ;
wire [309:309] mux_1level_tapbuf_size2_289_configbus0;
wire [309:309] mux_1level_tapbuf_size2_289_configbus1;
wire [309:309] mux_1level_tapbuf_size2_289_sram_blwl_out ;
wire [309:309] mux_1level_tapbuf_size2_289_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_289_configbus0[309:309] = sram_blwl_bl[309:309] ;
assign mux_1level_tapbuf_size2_289_configbus1[309:309] = sram_blwl_wl[309:309] ;
wire [309:309] mux_1level_tapbuf_size2_289_configbus0_b;
assign mux_1level_tapbuf_size2_289_configbus0_b[309:309] = sram_blwl_blb[309:309] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_289_ (mux_1level_tapbuf_size2_289_inbus, chanx_1__0__out_79_ , mux_1level_tapbuf_size2_289_sram_blwl_out[309:309] ,
mux_1level_tapbuf_size2_289_sram_blwl_outb[309:309] );
//----- SRAM bits for MUX[289], level=1, select_path_id=1. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----0-----
sram6T_blwl sram_blwl_309_ (mux_1level_tapbuf_size2_289_sram_blwl_out[309:309] ,mux_1level_tapbuf_size2_289_sram_blwl_out[309:309] ,mux_1level_tapbuf_size2_289_sram_blwl_outb[309:309] ,mux_1level_tapbuf_size2_289_configbus0[309:309], mux_1level_tapbuf_size2_289_configbus1[309:309] , mux_1level_tapbuf_size2_289_configbus0_b[309:309] );
wire [0:1] mux_1level_tapbuf_size2_290_inbus;
assign mux_1level_tapbuf_size2_290_inbus[0] =  grid_1__1__pin_0__2__42_;
assign mux_1level_tapbuf_size2_290_inbus[1] = chany_1__1__in_21_ ;
wire [310:310] mux_1level_tapbuf_size2_290_configbus0;
wire [310:310] mux_1level_tapbuf_size2_290_configbus1;
wire [310:310] mux_1level_tapbuf_size2_290_sram_blwl_out ;
wire [310:310] mux_1level_tapbuf_size2_290_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_290_configbus0[310:310] = sram_blwl_bl[310:310] ;
assign mux_1level_tapbuf_size2_290_configbus1[310:310] = sram_blwl_wl[310:310] ;
wire [310:310] mux_1level_tapbuf_size2_290_configbus0_b;
assign mux_1level_tapbuf_size2_290_configbus0_b[310:310] = sram_blwl_blb[310:310] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_290_ (mux_1level_tapbuf_size2_290_inbus, chanx_1__0__out_81_ , mux_1level_tapbuf_size2_290_sram_blwl_out[310:310] ,
mux_1level_tapbuf_size2_290_sram_blwl_outb[310:310] );
//----- SRAM bits for MUX[290], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_310_ (mux_1level_tapbuf_size2_290_sram_blwl_out[310:310] ,mux_1level_tapbuf_size2_290_sram_blwl_out[310:310] ,mux_1level_tapbuf_size2_290_sram_blwl_outb[310:310] ,mux_1level_tapbuf_size2_290_configbus0[310:310], mux_1level_tapbuf_size2_290_configbus1[310:310] , mux_1level_tapbuf_size2_290_configbus0_b[310:310] );
wire [0:1] mux_1level_tapbuf_size2_291_inbus;
assign mux_1level_tapbuf_size2_291_inbus[0] =  grid_1__1__pin_0__2__42_;
assign mux_1level_tapbuf_size2_291_inbus[1] = chany_1__1__in_19_ ;
wire [311:311] mux_1level_tapbuf_size2_291_configbus0;
wire [311:311] mux_1level_tapbuf_size2_291_configbus1;
wire [311:311] mux_1level_tapbuf_size2_291_sram_blwl_out ;
wire [311:311] mux_1level_tapbuf_size2_291_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_291_configbus0[311:311] = sram_blwl_bl[311:311] ;
assign mux_1level_tapbuf_size2_291_configbus1[311:311] = sram_blwl_wl[311:311] ;
wire [311:311] mux_1level_tapbuf_size2_291_configbus0_b;
assign mux_1level_tapbuf_size2_291_configbus0_b[311:311] = sram_blwl_blb[311:311] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_291_ (mux_1level_tapbuf_size2_291_inbus, chanx_1__0__out_83_ , mux_1level_tapbuf_size2_291_sram_blwl_out[311:311] ,
mux_1level_tapbuf_size2_291_sram_blwl_outb[311:311] );
//----- SRAM bits for MUX[291], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_311_ (mux_1level_tapbuf_size2_291_sram_blwl_out[311:311] ,mux_1level_tapbuf_size2_291_sram_blwl_out[311:311] ,mux_1level_tapbuf_size2_291_sram_blwl_outb[311:311] ,mux_1level_tapbuf_size2_291_configbus0[311:311], mux_1level_tapbuf_size2_291_configbus1[311:311] , mux_1level_tapbuf_size2_291_configbus0_b[311:311] );
wire [0:1] mux_1level_tapbuf_size2_292_inbus;
assign mux_1level_tapbuf_size2_292_inbus[0] =  grid_1__1__pin_0__2__42_;
assign mux_1level_tapbuf_size2_292_inbus[1] = chany_1__1__in_17_ ;
wire [312:312] mux_1level_tapbuf_size2_292_configbus0;
wire [312:312] mux_1level_tapbuf_size2_292_configbus1;
wire [312:312] mux_1level_tapbuf_size2_292_sram_blwl_out ;
wire [312:312] mux_1level_tapbuf_size2_292_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_292_configbus0[312:312] = sram_blwl_bl[312:312] ;
assign mux_1level_tapbuf_size2_292_configbus1[312:312] = sram_blwl_wl[312:312] ;
wire [312:312] mux_1level_tapbuf_size2_292_configbus0_b;
assign mux_1level_tapbuf_size2_292_configbus0_b[312:312] = sram_blwl_blb[312:312] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_292_ (mux_1level_tapbuf_size2_292_inbus, chanx_1__0__out_85_ , mux_1level_tapbuf_size2_292_sram_blwl_out[312:312] ,
mux_1level_tapbuf_size2_292_sram_blwl_outb[312:312] );
//----- SRAM bits for MUX[292], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_312_ (mux_1level_tapbuf_size2_292_sram_blwl_out[312:312] ,mux_1level_tapbuf_size2_292_sram_blwl_out[312:312] ,mux_1level_tapbuf_size2_292_sram_blwl_outb[312:312] ,mux_1level_tapbuf_size2_292_configbus0[312:312], mux_1level_tapbuf_size2_292_configbus1[312:312] , mux_1level_tapbuf_size2_292_configbus0_b[312:312] );
wire [0:1] mux_1level_tapbuf_size2_293_inbus;
assign mux_1level_tapbuf_size2_293_inbus[0] =  grid_1__1__pin_0__2__42_;
assign mux_1level_tapbuf_size2_293_inbus[1] = chany_1__1__in_15_ ;
wire [313:313] mux_1level_tapbuf_size2_293_configbus0;
wire [313:313] mux_1level_tapbuf_size2_293_configbus1;
wire [313:313] mux_1level_tapbuf_size2_293_sram_blwl_out ;
wire [313:313] mux_1level_tapbuf_size2_293_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_293_configbus0[313:313] = sram_blwl_bl[313:313] ;
assign mux_1level_tapbuf_size2_293_configbus1[313:313] = sram_blwl_wl[313:313] ;
wire [313:313] mux_1level_tapbuf_size2_293_configbus0_b;
assign mux_1level_tapbuf_size2_293_configbus0_b[313:313] = sram_blwl_blb[313:313] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_293_ (mux_1level_tapbuf_size2_293_inbus, chanx_1__0__out_87_ , mux_1level_tapbuf_size2_293_sram_blwl_out[313:313] ,
mux_1level_tapbuf_size2_293_sram_blwl_outb[313:313] );
//----- SRAM bits for MUX[293], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_313_ (mux_1level_tapbuf_size2_293_sram_blwl_out[313:313] ,mux_1level_tapbuf_size2_293_sram_blwl_out[313:313] ,mux_1level_tapbuf_size2_293_sram_blwl_outb[313:313] ,mux_1level_tapbuf_size2_293_configbus0[313:313], mux_1level_tapbuf_size2_293_configbus1[313:313] , mux_1level_tapbuf_size2_293_configbus0_b[313:313] );
wire [0:1] mux_1level_tapbuf_size2_294_inbus;
assign mux_1level_tapbuf_size2_294_inbus[0] =  grid_1__1__pin_0__2__42_;
assign mux_1level_tapbuf_size2_294_inbus[1] = chany_1__1__in_13_ ;
wire [314:314] mux_1level_tapbuf_size2_294_configbus0;
wire [314:314] mux_1level_tapbuf_size2_294_configbus1;
wire [314:314] mux_1level_tapbuf_size2_294_sram_blwl_out ;
wire [314:314] mux_1level_tapbuf_size2_294_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_294_configbus0[314:314] = sram_blwl_bl[314:314] ;
assign mux_1level_tapbuf_size2_294_configbus1[314:314] = sram_blwl_wl[314:314] ;
wire [314:314] mux_1level_tapbuf_size2_294_configbus0_b;
assign mux_1level_tapbuf_size2_294_configbus0_b[314:314] = sram_blwl_blb[314:314] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_294_ (mux_1level_tapbuf_size2_294_inbus, chanx_1__0__out_89_ , mux_1level_tapbuf_size2_294_sram_blwl_out[314:314] ,
mux_1level_tapbuf_size2_294_sram_blwl_outb[314:314] );
//----- SRAM bits for MUX[294], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_314_ (mux_1level_tapbuf_size2_294_sram_blwl_out[314:314] ,mux_1level_tapbuf_size2_294_sram_blwl_out[314:314] ,mux_1level_tapbuf_size2_294_sram_blwl_outb[314:314] ,mux_1level_tapbuf_size2_294_configbus0[314:314], mux_1level_tapbuf_size2_294_configbus1[314:314] , mux_1level_tapbuf_size2_294_configbus0_b[314:314] );
wire [0:1] mux_1level_tapbuf_size2_295_inbus;
assign mux_1level_tapbuf_size2_295_inbus[0] =  grid_1__1__pin_0__2__46_;
assign mux_1level_tapbuf_size2_295_inbus[1] = chany_1__1__in_11_ ;
wire [315:315] mux_1level_tapbuf_size2_295_configbus0;
wire [315:315] mux_1level_tapbuf_size2_295_configbus1;
wire [315:315] mux_1level_tapbuf_size2_295_sram_blwl_out ;
wire [315:315] mux_1level_tapbuf_size2_295_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_295_configbus0[315:315] = sram_blwl_bl[315:315] ;
assign mux_1level_tapbuf_size2_295_configbus1[315:315] = sram_blwl_wl[315:315] ;
wire [315:315] mux_1level_tapbuf_size2_295_configbus0_b;
assign mux_1level_tapbuf_size2_295_configbus0_b[315:315] = sram_blwl_blb[315:315] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_295_ (mux_1level_tapbuf_size2_295_inbus, chanx_1__0__out_91_ , mux_1level_tapbuf_size2_295_sram_blwl_out[315:315] ,
mux_1level_tapbuf_size2_295_sram_blwl_outb[315:315] );
//----- SRAM bits for MUX[295], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_315_ (mux_1level_tapbuf_size2_295_sram_blwl_out[315:315] ,mux_1level_tapbuf_size2_295_sram_blwl_out[315:315] ,mux_1level_tapbuf_size2_295_sram_blwl_outb[315:315] ,mux_1level_tapbuf_size2_295_configbus0[315:315], mux_1level_tapbuf_size2_295_configbus1[315:315] , mux_1level_tapbuf_size2_295_configbus0_b[315:315] );
wire [0:1] mux_1level_tapbuf_size2_296_inbus;
assign mux_1level_tapbuf_size2_296_inbus[0] =  grid_1__1__pin_0__2__46_;
assign mux_1level_tapbuf_size2_296_inbus[1] = chany_1__1__in_9_ ;
wire [316:316] mux_1level_tapbuf_size2_296_configbus0;
wire [316:316] mux_1level_tapbuf_size2_296_configbus1;
wire [316:316] mux_1level_tapbuf_size2_296_sram_blwl_out ;
wire [316:316] mux_1level_tapbuf_size2_296_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_296_configbus0[316:316] = sram_blwl_bl[316:316] ;
assign mux_1level_tapbuf_size2_296_configbus1[316:316] = sram_blwl_wl[316:316] ;
wire [316:316] mux_1level_tapbuf_size2_296_configbus0_b;
assign mux_1level_tapbuf_size2_296_configbus0_b[316:316] = sram_blwl_blb[316:316] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_296_ (mux_1level_tapbuf_size2_296_inbus, chanx_1__0__out_93_ , mux_1level_tapbuf_size2_296_sram_blwl_out[316:316] ,
mux_1level_tapbuf_size2_296_sram_blwl_outb[316:316] );
//----- SRAM bits for MUX[296], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_316_ (mux_1level_tapbuf_size2_296_sram_blwl_out[316:316] ,mux_1level_tapbuf_size2_296_sram_blwl_out[316:316] ,mux_1level_tapbuf_size2_296_sram_blwl_outb[316:316] ,mux_1level_tapbuf_size2_296_configbus0[316:316], mux_1level_tapbuf_size2_296_configbus1[316:316] , mux_1level_tapbuf_size2_296_configbus0_b[316:316] );
wire [0:1] mux_1level_tapbuf_size2_297_inbus;
assign mux_1level_tapbuf_size2_297_inbus[0] =  grid_1__1__pin_0__2__46_;
assign mux_1level_tapbuf_size2_297_inbus[1] = chany_1__1__in_7_ ;
wire [317:317] mux_1level_tapbuf_size2_297_configbus0;
wire [317:317] mux_1level_tapbuf_size2_297_configbus1;
wire [317:317] mux_1level_tapbuf_size2_297_sram_blwl_out ;
wire [317:317] mux_1level_tapbuf_size2_297_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_297_configbus0[317:317] = sram_blwl_bl[317:317] ;
assign mux_1level_tapbuf_size2_297_configbus1[317:317] = sram_blwl_wl[317:317] ;
wire [317:317] mux_1level_tapbuf_size2_297_configbus0_b;
assign mux_1level_tapbuf_size2_297_configbus0_b[317:317] = sram_blwl_blb[317:317] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_297_ (mux_1level_tapbuf_size2_297_inbus, chanx_1__0__out_95_ , mux_1level_tapbuf_size2_297_sram_blwl_out[317:317] ,
mux_1level_tapbuf_size2_297_sram_blwl_outb[317:317] );
//----- SRAM bits for MUX[297], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_317_ (mux_1level_tapbuf_size2_297_sram_blwl_out[317:317] ,mux_1level_tapbuf_size2_297_sram_blwl_out[317:317] ,mux_1level_tapbuf_size2_297_sram_blwl_outb[317:317] ,mux_1level_tapbuf_size2_297_configbus0[317:317], mux_1level_tapbuf_size2_297_configbus1[317:317] , mux_1level_tapbuf_size2_297_configbus0_b[317:317] );
wire [0:1] mux_1level_tapbuf_size2_298_inbus;
assign mux_1level_tapbuf_size2_298_inbus[0] =  grid_1__1__pin_0__2__46_;
assign mux_1level_tapbuf_size2_298_inbus[1] = chany_1__1__in_5_ ;
wire [318:318] mux_1level_tapbuf_size2_298_configbus0;
wire [318:318] mux_1level_tapbuf_size2_298_configbus1;
wire [318:318] mux_1level_tapbuf_size2_298_sram_blwl_out ;
wire [318:318] mux_1level_tapbuf_size2_298_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_298_configbus0[318:318] = sram_blwl_bl[318:318] ;
assign mux_1level_tapbuf_size2_298_configbus1[318:318] = sram_blwl_wl[318:318] ;
wire [318:318] mux_1level_tapbuf_size2_298_configbus0_b;
assign mux_1level_tapbuf_size2_298_configbus0_b[318:318] = sram_blwl_blb[318:318] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_298_ (mux_1level_tapbuf_size2_298_inbus, chanx_1__0__out_97_ , mux_1level_tapbuf_size2_298_sram_blwl_out[318:318] ,
mux_1level_tapbuf_size2_298_sram_blwl_outb[318:318] );
//----- SRAM bits for MUX[298], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_318_ (mux_1level_tapbuf_size2_298_sram_blwl_out[318:318] ,mux_1level_tapbuf_size2_298_sram_blwl_out[318:318] ,mux_1level_tapbuf_size2_298_sram_blwl_outb[318:318] ,mux_1level_tapbuf_size2_298_configbus0[318:318], mux_1level_tapbuf_size2_298_configbus1[318:318] , mux_1level_tapbuf_size2_298_configbus0_b[318:318] );
wire [0:1] mux_1level_tapbuf_size2_299_inbus;
assign mux_1level_tapbuf_size2_299_inbus[0] =  grid_1__1__pin_0__2__46_;
assign mux_1level_tapbuf_size2_299_inbus[1] = chany_1__1__in_3_ ;
wire [319:319] mux_1level_tapbuf_size2_299_configbus0;
wire [319:319] mux_1level_tapbuf_size2_299_configbus1;
wire [319:319] mux_1level_tapbuf_size2_299_sram_blwl_out ;
wire [319:319] mux_1level_tapbuf_size2_299_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_299_configbus0[319:319] = sram_blwl_bl[319:319] ;
assign mux_1level_tapbuf_size2_299_configbus1[319:319] = sram_blwl_wl[319:319] ;
wire [319:319] mux_1level_tapbuf_size2_299_configbus0_b;
assign mux_1level_tapbuf_size2_299_configbus0_b[319:319] = sram_blwl_blb[319:319] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_299_ (mux_1level_tapbuf_size2_299_inbus, chanx_1__0__out_99_ , mux_1level_tapbuf_size2_299_sram_blwl_out[319:319] ,
mux_1level_tapbuf_size2_299_sram_blwl_outb[319:319] );
//----- SRAM bits for MUX[299], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_319_ (mux_1level_tapbuf_size2_299_sram_blwl_out[319:319] ,mux_1level_tapbuf_size2_299_sram_blwl_out[319:319] ,mux_1level_tapbuf_size2_299_sram_blwl_outb[319:319] ,mux_1level_tapbuf_size2_299_configbus0[319:319], mux_1level_tapbuf_size2_299_configbus1[319:319] , mux_1level_tapbuf_size2_299_configbus0_b[319:319] );
endmodule
//----- END Verilog Module of Switch Box[1][0] -----

