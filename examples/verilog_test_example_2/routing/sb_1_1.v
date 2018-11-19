//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Switch Block  [1][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
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
  input chany_1__1__in_30_,
  output chany_1__1__out_31_,
  input chany_1__1__in_32_,
  output chany_1__1__out_33_,
  input chany_1__1__in_34_,
  output chany_1__1__out_35_,
  input chany_1__1__in_36_,
  output chany_1__1__out_37_,
  input chany_1__1__in_38_,
  output chany_1__1__out_39_,
  input chany_1__1__in_40_,
  output chany_1__1__out_41_,
  input chany_1__1__in_42_,
  output chany_1__1__out_43_,
  input chany_1__1__in_44_,
  output chany_1__1__out_45_,
  input chany_1__1__in_46_,
  output chany_1__1__out_47_,
  input chany_1__1__in_48_,
  output chany_1__1__out_49_,
  input chany_1__1__in_50_,
  output chany_1__1__out_51_,
  input chany_1__1__in_52_,
  output chany_1__1__out_53_,
  input chany_1__1__in_54_,
  output chany_1__1__out_55_,
  input chany_1__1__in_56_,
  output chany_1__1__out_57_,
  input chany_1__1__in_58_,
  output chany_1__1__out_59_,
  input chany_1__1__in_60_,
  output chany_1__1__out_61_,
  input chany_1__1__in_62_,
  output chany_1__1__out_63_,
  input chany_1__1__in_64_,
  output chany_1__1__out_65_,
  input chany_1__1__in_66_,
  output chany_1__1__out_67_,
  input chany_1__1__in_68_,
  output chany_1__1__out_69_,
  input chany_1__1__in_70_,
  output chany_1__1__out_71_,
  input chany_1__1__in_72_,
  output chany_1__1__out_73_,
  input chany_1__1__in_74_,
  output chany_1__1__out_75_,
  input chany_1__1__in_76_,
  output chany_1__1__out_77_,
  input chany_1__1__in_78_,
  output chany_1__1__out_79_,
  input chany_1__1__in_80_,
  output chany_1__1__out_81_,
  input chany_1__1__in_82_,
  output chany_1__1__out_83_,
  input chany_1__1__in_84_,
  output chany_1__1__out_85_,
  input chany_1__1__in_86_,
  output chany_1__1__out_87_,
  input chany_1__1__in_88_,
  output chany_1__1__out_89_,
  input chany_1__1__in_90_,
  output chany_1__1__out_91_,
  input chany_1__1__in_92_,
  output chany_1__1__out_93_,
  input chany_1__1__in_94_,
  output chany_1__1__out_95_,
  input chany_1__1__in_96_,
  output chany_1__1__out_97_,
  input chany_1__1__in_98_,
  output chany_1__1__out_99_,
input  grid_2__1__pin_0__3__1_,
input  grid_2__1__pin_0__3__3_,
input  grid_2__1__pin_0__3__5_,
input  grid_2__1__pin_0__3__7_,
input  grid_2__1__pin_0__3__9_,
input  grid_2__1__pin_0__3__11_,
input  grid_2__1__pin_0__3__13_,
input  grid_2__1__pin_0__3__15_,
input  grid_1__1__pin_0__1__41_,
input  grid_1__1__pin_0__1__45_,
input  grid_1__1__pin_0__1__49_,
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
  input chanx_1__1__in_30_,
  output chanx_1__1__out_31_,
  input chanx_1__1__in_32_,
  output chanx_1__1__out_33_,
  input chanx_1__1__in_34_,
  output chanx_1__1__out_35_,
  input chanx_1__1__in_36_,
  output chanx_1__1__out_37_,
  input chanx_1__1__in_38_,
  output chanx_1__1__out_39_,
  input chanx_1__1__in_40_,
  output chanx_1__1__out_41_,
  input chanx_1__1__in_42_,
  output chanx_1__1__out_43_,
  input chanx_1__1__in_44_,
  output chanx_1__1__out_45_,
  input chanx_1__1__in_46_,
  output chanx_1__1__out_47_,
  input chanx_1__1__in_48_,
  output chanx_1__1__out_49_,
  input chanx_1__1__in_50_,
  output chanx_1__1__out_51_,
  input chanx_1__1__in_52_,
  output chanx_1__1__out_53_,
  input chanx_1__1__in_54_,
  output chanx_1__1__out_55_,
  input chanx_1__1__in_56_,
  output chanx_1__1__out_57_,
  input chanx_1__1__in_58_,
  output chanx_1__1__out_59_,
  input chanx_1__1__in_60_,
  output chanx_1__1__out_61_,
  input chanx_1__1__in_62_,
  output chanx_1__1__out_63_,
  input chanx_1__1__in_64_,
  output chanx_1__1__out_65_,
  input chanx_1__1__in_66_,
  output chanx_1__1__out_67_,
  input chanx_1__1__in_68_,
  output chanx_1__1__out_69_,
  input chanx_1__1__in_70_,
  output chanx_1__1__out_71_,
  input chanx_1__1__in_72_,
  output chanx_1__1__out_73_,
  input chanx_1__1__in_74_,
  output chanx_1__1__out_75_,
  input chanx_1__1__in_76_,
  output chanx_1__1__out_77_,
  input chanx_1__1__in_78_,
  output chanx_1__1__out_79_,
  input chanx_1__1__in_80_,
  output chanx_1__1__out_81_,
  input chanx_1__1__in_82_,
  output chanx_1__1__out_83_,
  input chanx_1__1__in_84_,
  output chanx_1__1__out_85_,
  input chanx_1__1__in_86_,
  output chanx_1__1__out_87_,
  input chanx_1__1__in_88_,
  output chanx_1__1__out_89_,
  input chanx_1__1__in_90_,
  output chanx_1__1__out_91_,
  input chanx_1__1__in_92_,
  output chanx_1__1__out_93_,
  input chanx_1__1__in_94_,
  output chanx_1__1__out_95_,
  input chanx_1__1__in_96_,
  output chanx_1__1__out_97_,
  input chanx_1__1__in_98_,
  output chanx_1__1__out_99_,
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
input [320:439] sram_blwl_bl ,
input [320:439] sram_blwl_wl ,
input [320:439] sram_blwl_blb ); 
//----- top side Multiplexers -----
//----- right side Multiplexers -----
//----- bottom side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_300_inbus;
assign mux_1level_tapbuf_size3_300_inbus[0] =  grid_1__1__pin_0__1__41_;
assign mux_1level_tapbuf_size3_300_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_300_inbus[2] = chanx_1__1__in_2_ ;
wire [320:322] mux_1level_tapbuf_size3_300_configbus0;
wire [320:322] mux_1level_tapbuf_size3_300_configbus1;
wire [320:322] mux_1level_tapbuf_size3_300_sram_blwl_out ;
wire [320:322] mux_1level_tapbuf_size3_300_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_300_configbus0[320:322] = sram_blwl_bl[320:322] ;
assign mux_1level_tapbuf_size3_300_configbus1[320:322] = sram_blwl_wl[320:322] ;
wire [320:322] mux_1level_tapbuf_size3_300_configbus0_b;
assign mux_1level_tapbuf_size3_300_configbus0_b[320:322] = sram_blwl_blb[320:322] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_300_ (mux_1level_tapbuf_size3_300_inbus, chany_1__1__out_1_ , mux_1level_tapbuf_size3_300_sram_blwl_out[320:322] ,
mux_1level_tapbuf_size3_300_sram_blwl_outb[320:322] );
//----- SRAM bits for MUX[300], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_320_ (mux_1level_tapbuf_size3_300_sram_blwl_out[320:320] ,mux_1level_tapbuf_size3_300_sram_blwl_out[320:320] ,mux_1level_tapbuf_size3_300_sram_blwl_outb[320:320] ,mux_1level_tapbuf_size3_300_configbus0[320:320], mux_1level_tapbuf_size3_300_configbus1[320:320] , mux_1level_tapbuf_size3_300_configbus0_b[320:320] );
sram6T_blwl sram_blwl_321_ (mux_1level_tapbuf_size3_300_sram_blwl_out[321:321] ,mux_1level_tapbuf_size3_300_sram_blwl_out[321:321] ,mux_1level_tapbuf_size3_300_sram_blwl_outb[321:321] ,mux_1level_tapbuf_size3_300_configbus0[321:321], mux_1level_tapbuf_size3_300_configbus1[321:321] , mux_1level_tapbuf_size3_300_configbus0_b[321:321] );
sram6T_blwl sram_blwl_322_ (mux_1level_tapbuf_size3_300_sram_blwl_out[322:322] ,mux_1level_tapbuf_size3_300_sram_blwl_out[322:322] ,mux_1level_tapbuf_size3_300_sram_blwl_outb[322:322] ,mux_1level_tapbuf_size3_300_configbus0[322:322], mux_1level_tapbuf_size3_300_configbus1[322:322] , mux_1level_tapbuf_size3_300_configbus0_b[322:322] );
wire [0:2] mux_1level_tapbuf_size3_301_inbus;
assign mux_1level_tapbuf_size3_301_inbus[0] =  grid_1__1__pin_0__1__41_;
assign mux_1level_tapbuf_size3_301_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_301_inbus[2] = chanx_1__1__in_4_ ;
wire [323:325] mux_1level_tapbuf_size3_301_configbus0;
wire [323:325] mux_1level_tapbuf_size3_301_configbus1;
wire [323:325] mux_1level_tapbuf_size3_301_sram_blwl_out ;
wire [323:325] mux_1level_tapbuf_size3_301_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_301_configbus0[323:325] = sram_blwl_bl[323:325] ;
assign mux_1level_tapbuf_size3_301_configbus1[323:325] = sram_blwl_wl[323:325] ;
wire [323:325] mux_1level_tapbuf_size3_301_configbus0_b;
assign mux_1level_tapbuf_size3_301_configbus0_b[323:325] = sram_blwl_blb[323:325] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_301_ (mux_1level_tapbuf_size3_301_inbus, chany_1__1__out_3_ , mux_1level_tapbuf_size3_301_sram_blwl_out[323:325] ,
mux_1level_tapbuf_size3_301_sram_blwl_outb[323:325] );
//----- SRAM bits for MUX[301], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_323_ (mux_1level_tapbuf_size3_301_sram_blwl_out[323:323] ,mux_1level_tapbuf_size3_301_sram_blwl_out[323:323] ,mux_1level_tapbuf_size3_301_sram_blwl_outb[323:323] ,mux_1level_tapbuf_size3_301_configbus0[323:323], mux_1level_tapbuf_size3_301_configbus1[323:323] , mux_1level_tapbuf_size3_301_configbus0_b[323:323] );
sram6T_blwl sram_blwl_324_ (mux_1level_tapbuf_size3_301_sram_blwl_out[324:324] ,mux_1level_tapbuf_size3_301_sram_blwl_out[324:324] ,mux_1level_tapbuf_size3_301_sram_blwl_outb[324:324] ,mux_1level_tapbuf_size3_301_configbus0[324:324], mux_1level_tapbuf_size3_301_configbus1[324:324] , mux_1level_tapbuf_size3_301_configbus0_b[324:324] );
sram6T_blwl sram_blwl_325_ (mux_1level_tapbuf_size3_301_sram_blwl_out[325:325] ,mux_1level_tapbuf_size3_301_sram_blwl_out[325:325] ,mux_1level_tapbuf_size3_301_sram_blwl_outb[325:325] ,mux_1level_tapbuf_size3_301_configbus0[325:325], mux_1level_tapbuf_size3_301_configbus1[325:325] , mux_1level_tapbuf_size3_301_configbus0_b[325:325] );
wire [0:2] mux_1level_tapbuf_size3_302_inbus;
assign mux_1level_tapbuf_size3_302_inbus[0] =  grid_1__1__pin_0__1__41_;
assign mux_1level_tapbuf_size3_302_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_302_inbus[2] = chanx_1__1__in_6_ ;
wire [326:328] mux_1level_tapbuf_size3_302_configbus0;
wire [326:328] mux_1level_tapbuf_size3_302_configbus1;
wire [326:328] mux_1level_tapbuf_size3_302_sram_blwl_out ;
wire [326:328] mux_1level_tapbuf_size3_302_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_302_configbus0[326:328] = sram_blwl_bl[326:328] ;
assign mux_1level_tapbuf_size3_302_configbus1[326:328] = sram_blwl_wl[326:328] ;
wire [326:328] mux_1level_tapbuf_size3_302_configbus0_b;
assign mux_1level_tapbuf_size3_302_configbus0_b[326:328] = sram_blwl_blb[326:328] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_302_ (mux_1level_tapbuf_size3_302_inbus, chany_1__1__out_5_ , mux_1level_tapbuf_size3_302_sram_blwl_out[326:328] ,
mux_1level_tapbuf_size3_302_sram_blwl_outb[326:328] );
//----- SRAM bits for MUX[302], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_326_ (mux_1level_tapbuf_size3_302_sram_blwl_out[326:326] ,mux_1level_tapbuf_size3_302_sram_blwl_out[326:326] ,mux_1level_tapbuf_size3_302_sram_blwl_outb[326:326] ,mux_1level_tapbuf_size3_302_configbus0[326:326], mux_1level_tapbuf_size3_302_configbus1[326:326] , mux_1level_tapbuf_size3_302_configbus0_b[326:326] );
sram6T_blwl sram_blwl_327_ (mux_1level_tapbuf_size3_302_sram_blwl_out[327:327] ,mux_1level_tapbuf_size3_302_sram_blwl_out[327:327] ,mux_1level_tapbuf_size3_302_sram_blwl_outb[327:327] ,mux_1level_tapbuf_size3_302_configbus0[327:327], mux_1level_tapbuf_size3_302_configbus1[327:327] , mux_1level_tapbuf_size3_302_configbus0_b[327:327] );
sram6T_blwl sram_blwl_328_ (mux_1level_tapbuf_size3_302_sram_blwl_out[328:328] ,mux_1level_tapbuf_size3_302_sram_blwl_out[328:328] ,mux_1level_tapbuf_size3_302_sram_blwl_outb[328:328] ,mux_1level_tapbuf_size3_302_configbus0[328:328], mux_1level_tapbuf_size3_302_configbus1[328:328] , mux_1level_tapbuf_size3_302_configbus0_b[328:328] );
wire [0:2] mux_1level_tapbuf_size3_303_inbus;
assign mux_1level_tapbuf_size3_303_inbus[0] =  grid_1__1__pin_0__1__41_;
assign mux_1level_tapbuf_size3_303_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_303_inbus[2] = chanx_1__1__in_8_ ;
wire [329:331] mux_1level_tapbuf_size3_303_configbus0;
wire [329:331] mux_1level_tapbuf_size3_303_configbus1;
wire [329:331] mux_1level_tapbuf_size3_303_sram_blwl_out ;
wire [329:331] mux_1level_tapbuf_size3_303_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_303_configbus0[329:331] = sram_blwl_bl[329:331] ;
assign mux_1level_tapbuf_size3_303_configbus1[329:331] = sram_blwl_wl[329:331] ;
wire [329:331] mux_1level_tapbuf_size3_303_configbus0_b;
assign mux_1level_tapbuf_size3_303_configbus0_b[329:331] = sram_blwl_blb[329:331] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_303_ (mux_1level_tapbuf_size3_303_inbus, chany_1__1__out_7_ , mux_1level_tapbuf_size3_303_sram_blwl_out[329:331] ,
mux_1level_tapbuf_size3_303_sram_blwl_outb[329:331] );
//----- SRAM bits for MUX[303], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_329_ (mux_1level_tapbuf_size3_303_sram_blwl_out[329:329] ,mux_1level_tapbuf_size3_303_sram_blwl_out[329:329] ,mux_1level_tapbuf_size3_303_sram_blwl_outb[329:329] ,mux_1level_tapbuf_size3_303_configbus0[329:329], mux_1level_tapbuf_size3_303_configbus1[329:329] , mux_1level_tapbuf_size3_303_configbus0_b[329:329] );
sram6T_blwl sram_blwl_330_ (mux_1level_tapbuf_size3_303_sram_blwl_out[330:330] ,mux_1level_tapbuf_size3_303_sram_blwl_out[330:330] ,mux_1level_tapbuf_size3_303_sram_blwl_outb[330:330] ,mux_1level_tapbuf_size3_303_configbus0[330:330], mux_1level_tapbuf_size3_303_configbus1[330:330] , mux_1level_tapbuf_size3_303_configbus0_b[330:330] );
sram6T_blwl sram_blwl_331_ (mux_1level_tapbuf_size3_303_sram_blwl_out[331:331] ,mux_1level_tapbuf_size3_303_sram_blwl_out[331:331] ,mux_1level_tapbuf_size3_303_sram_blwl_outb[331:331] ,mux_1level_tapbuf_size3_303_configbus0[331:331], mux_1level_tapbuf_size3_303_configbus1[331:331] , mux_1level_tapbuf_size3_303_configbus0_b[331:331] );
wire [0:2] mux_1level_tapbuf_size3_304_inbus;
assign mux_1level_tapbuf_size3_304_inbus[0] =  grid_1__1__pin_0__1__41_;
assign mux_1level_tapbuf_size3_304_inbus[1] =  grid_2__1__pin_0__3__15_;
assign mux_1level_tapbuf_size3_304_inbus[2] = chanx_1__1__in_10_ ;
wire [332:334] mux_1level_tapbuf_size3_304_configbus0;
wire [332:334] mux_1level_tapbuf_size3_304_configbus1;
wire [332:334] mux_1level_tapbuf_size3_304_sram_blwl_out ;
wire [332:334] mux_1level_tapbuf_size3_304_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_304_configbus0[332:334] = sram_blwl_bl[332:334] ;
assign mux_1level_tapbuf_size3_304_configbus1[332:334] = sram_blwl_wl[332:334] ;
wire [332:334] mux_1level_tapbuf_size3_304_configbus0_b;
assign mux_1level_tapbuf_size3_304_configbus0_b[332:334] = sram_blwl_blb[332:334] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_304_ (mux_1level_tapbuf_size3_304_inbus, chany_1__1__out_9_ , mux_1level_tapbuf_size3_304_sram_blwl_out[332:334] ,
mux_1level_tapbuf_size3_304_sram_blwl_outb[332:334] );
//----- SRAM bits for MUX[304], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_332_ (mux_1level_tapbuf_size3_304_sram_blwl_out[332:332] ,mux_1level_tapbuf_size3_304_sram_blwl_out[332:332] ,mux_1level_tapbuf_size3_304_sram_blwl_outb[332:332] ,mux_1level_tapbuf_size3_304_configbus0[332:332], mux_1level_tapbuf_size3_304_configbus1[332:332] , mux_1level_tapbuf_size3_304_configbus0_b[332:332] );
sram6T_blwl sram_blwl_333_ (mux_1level_tapbuf_size3_304_sram_blwl_out[333:333] ,mux_1level_tapbuf_size3_304_sram_blwl_out[333:333] ,mux_1level_tapbuf_size3_304_sram_blwl_outb[333:333] ,mux_1level_tapbuf_size3_304_configbus0[333:333], mux_1level_tapbuf_size3_304_configbus1[333:333] , mux_1level_tapbuf_size3_304_configbus0_b[333:333] );
sram6T_blwl sram_blwl_334_ (mux_1level_tapbuf_size3_304_sram_blwl_out[334:334] ,mux_1level_tapbuf_size3_304_sram_blwl_out[334:334] ,mux_1level_tapbuf_size3_304_sram_blwl_outb[334:334] ,mux_1level_tapbuf_size3_304_configbus0[334:334], mux_1level_tapbuf_size3_304_configbus1[334:334] , mux_1level_tapbuf_size3_304_configbus0_b[334:334] );
wire [0:1] mux_1level_tapbuf_size2_305_inbus;
assign mux_1level_tapbuf_size2_305_inbus[0] =  grid_1__1__pin_0__1__45_;
assign mux_1level_tapbuf_size2_305_inbus[1] = chanx_1__1__in_12_ ;
wire [335:335] mux_1level_tapbuf_size2_305_configbus0;
wire [335:335] mux_1level_tapbuf_size2_305_configbus1;
wire [335:335] mux_1level_tapbuf_size2_305_sram_blwl_out ;
wire [335:335] mux_1level_tapbuf_size2_305_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_305_configbus0[335:335] = sram_blwl_bl[335:335] ;
assign mux_1level_tapbuf_size2_305_configbus1[335:335] = sram_blwl_wl[335:335] ;
wire [335:335] mux_1level_tapbuf_size2_305_configbus0_b;
assign mux_1level_tapbuf_size2_305_configbus0_b[335:335] = sram_blwl_blb[335:335] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_305_ (mux_1level_tapbuf_size2_305_inbus, chany_1__1__out_11_ , mux_1level_tapbuf_size2_305_sram_blwl_out[335:335] ,
mux_1level_tapbuf_size2_305_sram_blwl_outb[335:335] );
//----- SRAM bits for MUX[305], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_335_ (mux_1level_tapbuf_size2_305_sram_blwl_out[335:335] ,mux_1level_tapbuf_size2_305_sram_blwl_out[335:335] ,mux_1level_tapbuf_size2_305_sram_blwl_outb[335:335] ,mux_1level_tapbuf_size2_305_configbus0[335:335], mux_1level_tapbuf_size2_305_configbus1[335:335] , mux_1level_tapbuf_size2_305_configbus0_b[335:335] );
wire [0:1] mux_1level_tapbuf_size2_306_inbus;
assign mux_1level_tapbuf_size2_306_inbus[0] =  grid_1__1__pin_0__1__45_;
assign mux_1level_tapbuf_size2_306_inbus[1] = chanx_1__1__in_14_ ;
wire [336:336] mux_1level_tapbuf_size2_306_configbus0;
wire [336:336] mux_1level_tapbuf_size2_306_configbus1;
wire [336:336] mux_1level_tapbuf_size2_306_sram_blwl_out ;
wire [336:336] mux_1level_tapbuf_size2_306_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_306_configbus0[336:336] = sram_blwl_bl[336:336] ;
assign mux_1level_tapbuf_size2_306_configbus1[336:336] = sram_blwl_wl[336:336] ;
wire [336:336] mux_1level_tapbuf_size2_306_configbus0_b;
assign mux_1level_tapbuf_size2_306_configbus0_b[336:336] = sram_blwl_blb[336:336] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_306_ (mux_1level_tapbuf_size2_306_inbus, chany_1__1__out_13_ , mux_1level_tapbuf_size2_306_sram_blwl_out[336:336] ,
mux_1level_tapbuf_size2_306_sram_blwl_outb[336:336] );
//----- SRAM bits for MUX[306], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_336_ (mux_1level_tapbuf_size2_306_sram_blwl_out[336:336] ,mux_1level_tapbuf_size2_306_sram_blwl_out[336:336] ,mux_1level_tapbuf_size2_306_sram_blwl_outb[336:336] ,mux_1level_tapbuf_size2_306_configbus0[336:336], mux_1level_tapbuf_size2_306_configbus1[336:336] , mux_1level_tapbuf_size2_306_configbus0_b[336:336] );
wire [0:1] mux_1level_tapbuf_size2_307_inbus;
assign mux_1level_tapbuf_size2_307_inbus[0] =  grid_1__1__pin_0__1__45_;
assign mux_1level_tapbuf_size2_307_inbus[1] = chanx_1__1__in_16_ ;
wire [337:337] mux_1level_tapbuf_size2_307_configbus0;
wire [337:337] mux_1level_tapbuf_size2_307_configbus1;
wire [337:337] mux_1level_tapbuf_size2_307_sram_blwl_out ;
wire [337:337] mux_1level_tapbuf_size2_307_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_307_configbus0[337:337] = sram_blwl_bl[337:337] ;
assign mux_1level_tapbuf_size2_307_configbus1[337:337] = sram_blwl_wl[337:337] ;
wire [337:337] mux_1level_tapbuf_size2_307_configbus0_b;
assign mux_1level_tapbuf_size2_307_configbus0_b[337:337] = sram_blwl_blb[337:337] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_307_ (mux_1level_tapbuf_size2_307_inbus, chany_1__1__out_15_ , mux_1level_tapbuf_size2_307_sram_blwl_out[337:337] ,
mux_1level_tapbuf_size2_307_sram_blwl_outb[337:337] );
//----- SRAM bits for MUX[307], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_337_ (mux_1level_tapbuf_size2_307_sram_blwl_out[337:337] ,mux_1level_tapbuf_size2_307_sram_blwl_out[337:337] ,mux_1level_tapbuf_size2_307_sram_blwl_outb[337:337] ,mux_1level_tapbuf_size2_307_configbus0[337:337], mux_1level_tapbuf_size2_307_configbus1[337:337] , mux_1level_tapbuf_size2_307_configbus0_b[337:337] );
wire [0:1] mux_1level_tapbuf_size2_308_inbus;
assign mux_1level_tapbuf_size2_308_inbus[0] =  grid_1__1__pin_0__1__45_;
assign mux_1level_tapbuf_size2_308_inbus[1] = chanx_1__1__in_18_ ;
wire [338:338] mux_1level_tapbuf_size2_308_configbus0;
wire [338:338] mux_1level_tapbuf_size2_308_configbus1;
wire [338:338] mux_1level_tapbuf_size2_308_sram_blwl_out ;
wire [338:338] mux_1level_tapbuf_size2_308_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_308_configbus0[338:338] = sram_blwl_bl[338:338] ;
assign mux_1level_tapbuf_size2_308_configbus1[338:338] = sram_blwl_wl[338:338] ;
wire [338:338] mux_1level_tapbuf_size2_308_configbus0_b;
assign mux_1level_tapbuf_size2_308_configbus0_b[338:338] = sram_blwl_blb[338:338] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_308_ (mux_1level_tapbuf_size2_308_inbus, chany_1__1__out_17_ , mux_1level_tapbuf_size2_308_sram_blwl_out[338:338] ,
mux_1level_tapbuf_size2_308_sram_blwl_outb[338:338] );
//----- SRAM bits for MUX[308], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_338_ (mux_1level_tapbuf_size2_308_sram_blwl_out[338:338] ,mux_1level_tapbuf_size2_308_sram_blwl_out[338:338] ,mux_1level_tapbuf_size2_308_sram_blwl_outb[338:338] ,mux_1level_tapbuf_size2_308_configbus0[338:338], mux_1level_tapbuf_size2_308_configbus1[338:338] , mux_1level_tapbuf_size2_308_configbus0_b[338:338] );
wire [0:1] mux_1level_tapbuf_size2_309_inbus;
assign mux_1level_tapbuf_size2_309_inbus[0] =  grid_1__1__pin_0__1__45_;
assign mux_1level_tapbuf_size2_309_inbus[1] = chanx_1__1__in_20_ ;
wire [339:339] mux_1level_tapbuf_size2_309_configbus0;
wire [339:339] mux_1level_tapbuf_size2_309_configbus1;
wire [339:339] mux_1level_tapbuf_size2_309_sram_blwl_out ;
wire [339:339] mux_1level_tapbuf_size2_309_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_309_configbus0[339:339] = sram_blwl_bl[339:339] ;
assign mux_1level_tapbuf_size2_309_configbus1[339:339] = sram_blwl_wl[339:339] ;
wire [339:339] mux_1level_tapbuf_size2_309_configbus0_b;
assign mux_1level_tapbuf_size2_309_configbus0_b[339:339] = sram_blwl_blb[339:339] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_309_ (mux_1level_tapbuf_size2_309_inbus, chany_1__1__out_19_ , mux_1level_tapbuf_size2_309_sram_blwl_out[339:339] ,
mux_1level_tapbuf_size2_309_sram_blwl_outb[339:339] );
//----- SRAM bits for MUX[309], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_339_ (mux_1level_tapbuf_size2_309_sram_blwl_out[339:339] ,mux_1level_tapbuf_size2_309_sram_blwl_out[339:339] ,mux_1level_tapbuf_size2_309_sram_blwl_outb[339:339] ,mux_1level_tapbuf_size2_309_configbus0[339:339], mux_1level_tapbuf_size2_309_configbus1[339:339] , mux_1level_tapbuf_size2_309_configbus0_b[339:339] );
wire [0:1] mux_1level_tapbuf_size2_310_inbus;
assign mux_1level_tapbuf_size2_310_inbus[0] =  grid_1__1__pin_0__1__49_;
assign mux_1level_tapbuf_size2_310_inbus[1] = chanx_1__1__in_22_ ;
wire [340:340] mux_1level_tapbuf_size2_310_configbus0;
wire [340:340] mux_1level_tapbuf_size2_310_configbus1;
wire [340:340] mux_1level_tapbuf_size2_310_sram_blwl_out ;
wire [340:340] mux_1level_tapbuf_size2_310_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_310_configbus0[340:340] = sram_blwl_bl[340:340] ;
assign mux_1level_tapbuf_size2_310_configbus1[340:340] = sram_blwl_wl[340:340] ;
wire [340:340] mux_1level_tapbuf_size2_310_configbus0_b;
assign mux_1level_tapbuf_size2_310_configbus0_b[340:340] = sram_blwl_blb[340:340] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_310_ (mux_1level_tapbuf_size2_310_inbus, chany_1__1__out_21_ , mux_1level_tapbuf_size2_310_sram_blwl_out[340:340] ,
mux_1level_tapbuf_size2_310_sram_blwl_outb[340:340] );
//----- SRAM bits for MUX[310], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_340_ (mux_1level_tapbuf_size2_310_sram_blwl_out[340:340] ,mux_1level_tapbuf_size2_310_sram_blwl_out[340:340] ,mux_1level_tapbuf_size2_310_sram_blwl_outb[340:340] ,mux_1level_tapbuf_size2_310_configbus0[340:340], mux_1level_tapbuf_size2_310_configbus1[340:340] , mux_1level_tapbuf_size2_310_configbus0_b[340:340] );
wire [0:1] mux_1level_tapbuf_size2_311_inbus;
assign mux_1level_tapbuf_size2_311_inbus[0] =  grid_1__1__pin_0__1__49_;
assign mux_1level_tapbuf_size2_311_inbus[1] = chanx_1__1__in_24_ ;
wire [341:341] mux_1level_tapbuf_size2_311_configbus0;
wire [341:341] mux_1level_tapbuf_size2_311_configbus1;
wire [341:341] mux_1level_tapbuf_size2_311_sram_blwl_out ;
wire [341:341] mux_1level_tapbuf_size2_311_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_311_configbus0[341:341] = sram_blwl_bl[341:341] ;
assign mux_1level_tapbuf_size2_311_configbus1[341:341] = sram_blwl_wl[341:341] ;
wire [341:341] mux_1level_tapbuf_size2_311_configbus0_b;
assign mux_1level_tapbuf_size2_311_configbus0_b[341:341] = sram_blwl_blb[341:341] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_311_ (mux_1level_tapbuf_size2_311_inbus, chany_1__1__out_23_ , mux_1level_tapbuf_size2_311_sram_blwl_out[341:341] ,
mux_1level_tapbuf_size2_311_sram_blwl_outb[341:341] );
//----- SRAM bits for MUX[311], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_341_ (mux_1level_tapbuf_size2_311_sram_blwl_out[341:341] ,mux_1level_tapbuf_size2_311_sram_blwl_out[341:341] ,mux_1level_tapbuf_size2_311_sram_blwl_outb[341:341] ,mux_1level_tapbuf_size2_311_configbus0[341:341], mux_1level_tapbuf_size2_311_configbus1[341:341] , mux_1level_tapbuf_size2_311_configbus0_b[341:341] );
wire [0:1] mux_1level_tapbuf_size2_312_inbus;
assign mux_1level_tapbuf_size2_312_inbus[0] =  grid_1__1__pin_0__1__49_;
assign mux_1level_tapbuf_size2_312_inbus[1] = chanx_1__1__in_26_ ;
wire [342:342] mux_1level_tapbuf_size2_312_configbus0;
wire [342:342] mux_1level_tapbuf_size2_312_configbus1;
wire [342:342] mux_1level_tapbuf_size2_312_sram_blwl_out ;
wire [342:342] mux_1level_tapbuf_size2_312_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_312_configbus0[342:342] = sram_blwl_bl[342:342] ;
assign mux_1level_tapbuf_size2_312_configbus1[342:342] = sram_blwl_wl[342:342] ;
wire [342:342] mux_1level_tapbuf_size2_312_configbus0_b;
assign mux_1level_tapbuf_size2_312_configbus0_b[342:342] = sram_blwl_blb[342:342] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_312_ (mux_1level_tapbuf_size2_312_inbus, chany_1__1__out_25_ , mux_1level_tapbuf_size2_312_sram_blwl_out[342:342] ,
mux_1level_tapbuf_size2_312_sram_blwl_outb[342:342] );
//----- SRAM bits for MUX[312], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_342_ (mux_1level_tapbuf_size2_312_sram_blwl_out[342:342] ,mux_1level_tapbuf_size2_312_sram_blwl_out[342:342] ,mux_1level_tapbuf_size2_312_sram_blwl_outb[342:342] ,mux_1level_tapbuf_size2_312_configbus0[342:342], mux_1level_tapbuf_size2_312_configbus1[342:342] , mux_1level_tapbuf_size2_312_configbus0_b[342:342] );
wire [0:1] mux_1level_tapbuf_size2_313_inbus;
assign mux_1level_tapbuf_size2_313_inbus[0] =  grid_1__1__pin_0__1__49_;
assign mux_1level_tapbuf_size2_313_inbus[1] = chanx_1__1__in_28_ ;
wire [343:343] mux_1level_tapbuf_size2_313_configbus0;
wire [343:343] mux_1level_tapbuf_size2_313_configbus1;
wire [343:343] mux_1level_tapbuf_size2_313_sram_blwl_out ;
wire [343:343] mux_1level_tapbuf_size2_313_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_313_configbus0[343:343] = sram_blwl_bl[343:343] ;
assign mux_1level_tapbuf_size2_313_configbus1[343:343] = sram_blwl_wl[343:343] ;
wire [343:343] mux_1level_tapbuf_size2_313_configbus0_b;
assign mux_1level_tapbuf_size2_313_configbus0_b[343:343] = sram_blwl_blb[343:343] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_313_ (mux_1level_tapbuf_size2_313_inbus, chany_1__1__out_27_ , mux_1level_tapbuf_size2_313_sram_blwl_out[343:343] ,
mux_1level_tapbuf_size2_313_sram_blwl_outb[343:343] );
//----- SRAM bits for MUX[313], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_343_ (mux_1level_tapbuf_size2_313_sram_blwl_out[343:343] ,mux_1level_tapbuf_size2_313_sram_blwl_out[343:343] ,mux_1level_tapbuf_size2_313_sram_blwl_outb[343:343] ,mux_1level_tapbuf_size2_313_configbus0[343:343], mux_1level_tapbuf_size2_313_configbus1[343:343] , mux_1level_tapbuf_size2_313_configbus0_b[343:343] );
wire [0:1] mux_1level_tapbuf_size2_314_inbus;
assign mux_1level_tapbuf_size2_314_inbus[0] =  grid_1__1__pin_0__1__49_;
assign mux_1level_tapbuf_size2_314_inbus[1] = chanx_1__1__in_30_ ;
wire [344:344] mux_1level_tapbuf_size2_314_configbus0;
wire [344:344] mux_1level_tapbuf_size2_314_configbus1;
wire [344:344] mux_1level_tapbuf_size2_314_sram_blwl_out ;
wire [344:344] mux_1level_tapbuf_size2_314_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_314_configbus0[344:344] = sram_blwl_bl[344:344] ;
assign mux_1level_tapbuf_size2_314_configbus1[344:344] = sram_blwl_wl[344:344] ;
wire [344:344] mux_1level_tapbuf_size2_314_configbus0_b;
assign mux_1level_tapbuf_size2_314_configbus0_b[344:344] = sram_blwl_blb[344:344] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_314_ (mux_1level_tapbuf_size2_314_inbus, chany_1__1__out_29_ , mux_1level_tapbuf_size2_314_sram_blwl_out[344:344] ,
mux_1level_tapbuf_size2_314_sram_blwl_outb[344:344] );
//----- SRAM bits for MUX[314], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_344_ (mux_1level_tapbuf_size2_314_sram_blwl_out[344:344] ,mux_1level_tapbuf_size2_314_sram_blwl_out[344:344] ,mux_1level_tapbuf_size2_314_sram_blwl_outb[344:344] ,mux_1level_tapbuf_size2_314_configbus0[344:344], mux_1level_tapbuf_size2_314_configbus1[344:344] , mux_1level_tapbuf_size2_314_configbus0_b[344:344] );
wire [0:1] mux_1level_tapbuf_size2_315_inbus;
assign mux_1level_tapbuf_size2_315_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_315_inbus[1] = chanx_1__1__in_32_ ;
wire [345:345] mux_1level_tapbuf_size2_315_configbus0;
wire [345:345] mux_1level_tapbuf_size2_315_configbus1;
wire [345:345] mux_1level_tapbuf_size2_315_sram_blwl_out ;
wire [345:345] mux_1level_tapbuf_size2_315_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_315_configbus0[345:345] = sram_blwl_bl[345:345] ;
assign mux_1level_tapbuf_size2_315_configbus1[345:345] = sram_blwl_wl[345:345] ;
wire [345:345] mux_1level_tapbuf_size2_315_configbus0_b;
assign mux_1level_tapbuf_size2_315_configbus0_b[345:345] = sram_blwl_blb[345:345] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_315_ (mux_1level_tapbuf_size2_315_inbus, chany_1__1__out_31_ , mux_1level_tapbuf_size2_315_sram_blwl_out[345:345] ,
mux_1level_tapbuf_size2_315_sram_blwl_outb[345:345] );
//----- SRAM bits for MUX[315], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_345_ (mux_1level_tapbuf_size2_315_sram_blwl_out[345:345] ,mux_1level_tapbuf_size2_315_sram_blwl_out[345:345] ,mux_1level_tapbuf_size2_315_sram_blwl_outb[345:345] ,mux_1level_tapbuf_size2_315_configbus0[345:345], mux_1level_tapbuf_size2_315_configbus1[345:345] , mux_1level_tapbuf_size2_315_configbus0_b[345:345] );
wire [0:1] mux_1level_tapbuf_size2_316_inbus;
assign mux_1level_tapbuf_size2_316_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_316_inbus[1] = chanx_1__1__in_34_ ;
wire [346:346] mux_1level_tapbuf_size2_316_configbus0;
wire [346:346] mux_1level_tapbuf_size2_316_configbus1;
wire [346:346] mux_1level_tapbuf_size2_316_sram_blwl_out ;
wire [346:346] mux_1level_tapbuf_size2_316_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_316_configbus0[346:346] = sram_blwl_bl[346:346] ;
assign mux_1level_tapbuf_size2_316_configbus1[346:346] = sram_blwl_wl[346:346] ;
wire [346:346] mux_1level_tapbuf_size2_316_configbus0_b;
assign mux_1level_tapbuf_size2_316_configbus0_b[346:346] = sram_blwl_blb[346:346] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_316_ (mux_1level_tapbuf_size2_316_inbus, chany_1__1__out_33_ , mux_1level_tapbuf_size2_316_sram_blwl_out[346:346] ,
mux_1level_tapbuf_size2_316_sram_blwl_outb[346:346] );
//----- SRAM bits for MUX[316], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_346_ (mux_1level_tapbuf_size2_316_sram_blwl_out[346:346] ,mux_1level_tapbuf_size2_316_sram_blwl_out[346:346] ,mux_1level_tapbuf_size2_316_sram_blwl_outb[346:346] ,mux_1level_tapbuf_size2_316_configbus0[346:346], mux_1level_tapbuf_size2_316_configbus1[346:346] , mux_1level_tapbuf_size2_316_configbus0_b[346:346] );
wire [0:1] mux_1level_tapbuf_size2_317_inbus;
assign mux_1level_tapbuf_size2_317_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_317_inbus[1] = chanx_1__1__in_36_ ;
wire [347:347] mux_1level_tapbuf_size2_317_configbus0;
wire [347:347] mux_1level_tapbuf_size2_317_configbus1;
wire [347:347] mux_1level_tapbuf_size2_317_sram_blwl_out ;
wire [347:347] mux_1level_tapbuf_size2_317_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_317_configbus0[347:347] = sram_blwl_bl[347:347] ;
assign mux_1level_tapbuf_size2_317_configbus1[347:347] = sram_blwl_wl[347:347] ;
wire [347:347] mux_1level_tapbuf_size2_317_configbus0_b;
assign mux_1level_tapbuf_size2_317_configbus0_b[347:347] = sram_blwl_blb[347:347] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_317_ (mux_1level_tapbuf_size2_317_inbus, chany_1__1__out_35_ , mux_1level_tapbuf_size2_317_sram_blwl_out[347:347] ,
mux_1level_tapbuf_size2_317_sram_blwl_outb[347:347] );
//----- SRAM bits for MUX[317], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_347_ (mux_1level_tapbuf_size2_317_sram_blwl_out[347:347] ,mux_1level_tapbuf_size2_317_sram_blwl_out[347:347] ,mux_1level_tapbuf_size2_317_sram_blwl_outb[347:347] ,mux_1level_tapbuf_size2_317_configbus0[347:347], mux_1level_tapbuf_size2_317_configbus1[347:347] , mux_1level_tapbuf_size2_317_configbus0_b[347:347] );
wire [0:1] mux_1level_tapbuf_size2_318_inbus;
assign mux_1level_tapbuf_size2_318_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_318_inbus[1] = chanx_1__1__in_38_ ;
wire [348:348] mux_1level_tapbuf_size2_318_configbus0;
wire [348:348] mux_1level_tapbuf_size2_318_configbus1;
wire [348:348] mux_1level_tapbuf_size2_318_sram_blwl_out ;
wire [348:348] mux_1level_tapbuf_size2_318_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_318_configbus0[348:348] = sram_blwl_bl[348:348] ;
assign mux_1level_tapbuf_size2_318_configbus1[348:348] = sram_blwl_wl[348:348] ;
wire [348:348] mux_1level_tapbuf_size2_318_configbus0_b;
assign mux_1level_tapbuf_size2_318_configbus0_b[348:348] = sram_blwl_blb[348:348] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_318_ (mux_1level_tapbuf_size2_318_inbus, chany_1__1__out_37_ , mux_1level_tapbuf_size2_318_sram_blwl_out[348:348] ,
mux_1level_tapbuf_size2_318_sram_blwl_outb[348:348] );
//----- SRAM bits for MUX[318], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_348_ (mux_1level_tapbuf_size2_318_sram_blwl_out[348:348] ,mux_1level_tapbuf_size2_318_sram_blwl_out[348:348] ,mux_1level_tapbuf_size2_318_sram_blwl_outb[348:348] ,mux_1level_tapbuf_size2_318_configbus0[348:348], mux_1level_tapbuf_size2_318_configbus1[348:348] , mux_1level_tapbuf_size2_318_configbus0_b[348:348] );
wire [0:1] mux_1level_tapbuf_size2_319_inbus;
assign mux_1level_tapbuf_size2_319_inbus[0] =  grid_2__1__pin_0__3__1_;
assign mux_1level_tapbuf_size2_319_inbus[1] = chanx_1__1__in_40_ ;
wire [349:349] mux_1level_tapbuf_size2_319_configbus0;
wire [349:349] mux_1level_tapbuf_size2_319_configbus1;
wire [349:349] mux_1level_tapbuf_size2_319_sram_blwl_out ;
wire [349:349] mux_1level_tapbuf_size2_319_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_319_configbus0[349:349] = sram_blwl_bl[349:349] ;
assign mux_1level_tapbuf_size2_319_configbus1[349:349] = sram_blwl_wl[349:349] ;
wire [349:349] mux_1level_tapbuf_size2_319_configbus0_b;
assign mux_1level_tapbuf_size2_319_configbus0_b[349:349] = sram_blwl_blb[349:349] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_319_ (mux_1level_tapbuf_size2_319_inbus, chany_1__1__out_39_ , mux_1level_tapbuf_size2_319_sram_blwl_out[349:349] ,
mux_1level_tapbuf_size2_319_sram_blwl_outb[349:349] );
//----- SRAM bits for MUX[319], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_349_ (mux_1level_tapbuf_size2_319_sram_blwl_out[349:349] ,mux_1level_tapbuf_size2_319_sram_blwl_out[349:349] ,mux_1level_tapbuf_size2_319_sram_blwl_outb[349:349] ,mux_1level_tapbuf_size2_319_configbus0[349:349], mux_1level_tapbuf_size2_319_configbus1[349:349] , mux_1level_tapbuf_size2_319_configbus0_b[349:349] );
wire [0:1] mux_1level_tapbuf_size2_320_inbus;
assign mux_1level_tapbuf_size2_320_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_320_inbus[1] = chanx_1__1__in_42_ ;
wire [350:350] mux_1level_tapbuf_size2_320_configbus0;
wire [350:350] mux_1level_tapbuf_size2_320_configbus1;
wire [350:350] mux_1level_tapbuf_size2_320_sram_blwl_out ;
wire [350:350] mux_1level_tapbuf_size2_320_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_320_configbus0[350:350] = sram_blwl_bl[350:350] ;
assign mux_1level_tapbuf_size2_320_configbus1[350:350] = sram_blwl_wl[350:350] ;
wire [350:350] mux_1level_tapbuf_size2_320_configbus0_b;
assign mux_1level_tapbuf_size2_320_configbus0_b[350:350] = sram_blwl_blb[350:350] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_320_ (mux_1level_tapbuf_size2_320_inbus, chany_1__1__out_41_ , mux_1level_tapbuf_size2_320_sram_blwl_out[350:350] ,
mux_1level_tapbuf_size2_320_sram_blwl_outb[350:350] );
//----- SRAM bits for MUX[320], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_350_ (mux_1level_tapbuf_size2_320_sram_blwl_out[350:350] ,mux_1level_tapbuf_size2_320_sram_blwl_out[350:350] ,mux_1level_tapbuf_size2_320_sram_blwl_outb[350:350] ,mux_1level_tapbuf_size2_320_configbus0[350:350], mux_1level_tapbuf_size2_320_configbus1[350:350] , mux_1level_tapbuf_size2_320_configbus0_b[350:350] );
wire [0:1] mux_1level_tapbuf_size2_321_inbus;
assign mux_1level_tapbuf_size2_321_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_321_inbus[1] = chanx_1__1__in_44_ ;
wire [351:351] mux_1level_tapbuf_size2_321_configbus0;
wire [351:351] mux_1level_tapbuf_size2_321_configbus1;
wire [351:351] mux_1level_tapbuf_size2_321_sram_blwl_out ;
wire [351:351] mux_1level_tapbuf_size2_321_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_321_configbus0[351:351] = sram_blwl_bl[351:351] ;
assign mux_1level_tapbuf_size2_321_configbus1[351:351] = sram_blwl_wl[351:351] ;
wire [351:351] mux_1level_tapbuf_size2_321_configbus0_b;
assign mux_1level_tapbuf_size2_321_configbus0_b[351:351] = sram_blwl_blb[351:351] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_321_ (mux_1level_tapbuf_size2_321_inbus, chany_1__1__out_43_ , mux_1level_tapbuf_size2_321_sram_blwl_out[351:351] ,
mux_1level_tapbuf_size2_321_sram_blwl_outb[351:351] );
//----- SRAM bits for MUX[321], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_351_ (mux_1level_tapbuf_size2_321_sram_blwl_out[351:351] ,mux_1level_tapbuf_size2_321_sram_blwl_out[351:351] ,mux_1level_tapbuf_size2_321_sram_blwl_outb[351:351] ,mux_1level_tapbuf_size2_321_configbus0[351:351], mux_1level_tapbuf_size2_321_configbus1[351:351] , mux_1level_tapbuf_size2_321_configbus0_b[351:351] );
wire [0:1] mux_1level_tapbuf_size2_322_inbus;
assign mux_1level_tapbuf_size2_322_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_322_inbus[1] = chanx_1__1__in_46_ ;
wire [352:352] mux_1level_tapbuf_size2_322_configbus0;
wire [352:352] mux_1level_tapbuf_size2_322_configbus1;
wire [352:352] mux_1level_tapbuf_size2_322_sram_blwl_out ;
wire [352:352] mux_1level_tapbuf_size2_322_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_322_configbus0[352:352] = sram_blwl_bl[352:352] ;
assign mux_1level_tapbuf_size2_322_configbus1[352:352] = sram_blwl_wl[352:352] ;
wire [352:352] mux_1level_tapbuf_size2_322_configbus0_b;
assign mux_1level_tapbuf_size2_322_configbus0_b[352:352] = sram_blwl_blb[352:352] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_322_ (mux_1level_tapbuf_size2_322_inbus, chany_1__1__out_45_ , mux_1level_tapbuf_size2_322_sram_blwl_out[352:352] ,
mux_1level_tapbuf_size2_322_sram_blwl_outb[352:352] );
//----- SRAM bits for MUX[322], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_352_ (mux_1level_tapbuf_size2_322_sram_blwl_out[352:352] ,mux_1level_tapbuf_size2_322_sram_blwl_out[352:352] ,mux_1level_tapbuf_size2_322_sram_blwl_outb[352:352] ,mux_1level_tapbuf_size2_322_configbus0[352:352], mux_1level_tapbuf_size2_322_configbus1[352:352] , mux_1level_tapbuf_size2_322_configbus0_b[352:352] );
wire [0:1] mux_1level_tapbuf_size2_323_inbus;
assign mux_1level_tapbuf_size2_323_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_323_inbus[1] = chanx_1__1__in_48_ ;
wire [353:353] mux_1level_tapbuf_size2_323_configbus0;
wire [353:353] mux_1level_tapbuf_size2_323_configbus1;
wire [353:353] mux_1level_tapbuf_size2_323_sram_blwl_out ;
wire [353:353] mux_1level_tapbuf_size2_323_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_323_configbus0[353:353] = sram_blwl_bl[353:353] ;
assign mux_1level_tapbuf_size2_323_configbus1[353:353] = sram_blwl_wl[353:353] ;
wire [353:353] mux_1level_tapbuf_size2_323_configbus0_b;
assign mux_1level_tapbuf_size2_323_configbus0_b[353:353] = sram_blwl_blb[353:353] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_323_ (mux_1level_tapbuf_size2_323_inbus, chany_1__1__out_47_ , mux_1level_tapbuf_size2_323_sram_blwl_out[353:353] ,
mux_1level_tapbuf_size2_323_sram_blwl_outb[353:353] );
//----- SRAM bits for MUX[323], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_353_ (mux_1level_tapbuf_size2_323_sram_blwl_out[353:353] ,mux_1level_tapbuf_size2_323_sram_blwl_out[353:353] ,mux_1level_tapbuf_size2_323_sram_blwl_outb[353:353] ,mux_1level_tapbuf_size2_323_configbus0[353:353], mux_1level_tapbuf_size2_323_configbus1[353:353] , mux_1level_tapbuf_size2_323_configbus0_b[353:353] );
wire [0:1] mux_1level_tapbuf_size2_324_inbus;
assign mux_1level_tapbuf_size2_324_inbus[0] =  grid_2__1__pin_0__3__3_;
assign mux_1level_tapbuf_size2_324_inbus[1] = chanx_1__1__in_50_ ;
wire [354:354] mux_1level_tapbuf_size2_324_configbus0;
wire [354:354] mux_1level_tapbuf_size2_324_configbus1;
wire [354:354] mux_1level_tapbuf_size2_324_sram_blwl_out ;
wire [354:354] mux_1level_tapbuf_size2_324_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_324_configbus0[354:354] = sram_blwl_bl[354:354] ;
assign mux_1level_tapbuf_size2_324_configbus1[354:354] = sram_blwl_wl[354:354] ;
wire [354:354] mux_1level_tapbuf_size2_324_configbus0_b;
assign mux_1level_tapbuf_size2_324_configbus0_b[354:354] = sram_blwl_blb[354:354] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_324_ (mux_1level_tapbuf_size2_324_inbus, chany_1__1__out_49_ , mux_1level_tapbuf_size2_324_sram_blwl_out[354:354] ,
mux_1level_tapbuf_size2_324_sram_blwl_outb[354:354] );
//----- SRAM bits for MUX[324], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_354_ (mux_1level_tapbuf_size2_324_sram_blwl_out[354:354] ,mux_1level_tapbuf_size2_324_sram_blwl_out[354:354] ,mux_1level_tapbuf_size2_324_sram_blwl_outb[354:354] ,mux_1level_tapbuf_size2_324_configbus0[354:354], mux_1level_tapbuf_size2_324_configbus1[354:354] , mux_1level_tapbuf_size2_324_configbus0_b[354:354] );
wire [0:1] mux_1level_tapbuf_size2_325_inbus;
assign mux_1level_tapbuf_size2_325_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_325_inbus[1] = chanx_1__1__in_52_ ;
wire [355:355] mux_1level_tapbuf_size2_325_configbus0;
wire [355:355] mux_1level_tapbuf_size2_325_configbus1;
wire [355:355] mux_1level_tapbuf_size2_325_sram_blwl_out ;
wire [355:355] mux_1level_tapbuf_size2_325_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_325_configbus0[355:355] = sram_blwl_bl[355:355] ;
assign mux_1level_tapbuf_size2_325_configbus1[355:355] = sram_blwl_wl[355:355] ;
wire [355:355] mux_1level_tapbuf_size2_325_configbus0_b;
assign mux_1level_tapbuf_size2_325_configbus0_b[355:355] = sram_blwl_blb[355:355] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_325_ (mux_1level_tapbuf_size2_325_inbus, chany_1__1__out_51_ , mux_1level_tapbuf_size2_325_sram_blwl_out[355:355] ,
mux_1level_tapbuf_size2_325_sram_blwl_outb[355:355] );
//----- SRAM bits for MUX[325], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_355_ (mux_1level_tapbuf_size2_325_sram_blwl_out[355:355] ,mux_1level_tapbuf_size2_325_sram_blwl_out[355:355] ,mux_1level_tapbuf_size2_325_sram_blwl_outb[355:355] ,mux_1level_tapbuf_size2_325_configbus0[355:355], mux_1level_tapbuf_size2_325_configbus1[355:355] , mux_1level_tapbuf_size2_325_configbus0_b[355:355] );
wire [0:1] mux_1level_tapbuf_size2_326_inbus;
assign mux_1level_tapbuf_size2_326_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_326_inbus[1] = chanx_1__1__in_54_ ;
wire [356:356] mux_1level_tapbuf_size2_326_configbus0;
wire [356:356] mux_1level_tapbuf_size2_326_configbus1;
wire [356:356] mux_1level_tapbuf_size2_326_sram_blwl_out ;
wire [356:356] mux_1level_tapbuf_size2_326_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_326_configbus0[356:356] = sram_blwl_bl[356:356] ;
assign mux_1level_tapbuf_size2_326_configbus1[356:356] = sram_blwl_wl[356:356] ;
wire [356:356] mux_1level_tapbuf_size2_326_configbus0_b;
assign mux_1level_tapbuf_size2_326_configbus0_b[356:356] = sram_blwl_blb[356:356] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_326_ (mux_1level_tapbuf_size2_326_inbus, chany_1__1__out_53_ , mux_1level_tapbuf_size2_326_sram_blwl_out[356:356] ,
mux_1level_tapbuf_size2_326_sram_blwl_outb[356:356] );
//----- SRAM bits for MUX[326], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_356_ (mux_1level_tapbuf_size2_326_sram_blwl_out[356:356] ,mux_1level_tapbuf_size2_326_sram_blwl_out[356:356] ,mux_1level_tapbuf_size2_326_sram_blwl_outb[356:356] ,mux_1level_tapbuf_size2_326_configbus0[356:356], mux_1level_tapbuf_size2_326_configbus1[356:356] , mux_1level_tapbuf_size2_326_configbus0_b[356:356] );
wire [0:1] mux_1level_tapbuf_size2_327_inbus;
assign mux_1level_tapbuf_size2_327_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_327_inbus[1] = chanx_1__1__in_56_ ;
wire [357:357] mux_1level_tapbuf_size2_327_configbus0;
wire [357:357] mux_1level_tapbuf_size2_327_configbus1;
wire [357:357] mux_1level_tapbuf_size2_327_sram_blwl_out ;
wire [357:357] mux_1level_tapbuf_size2_327_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_327_configbus0[357:357] = sram_blwl_bl[357:357] ;
assign mux_1level_tapbuf_size2_327_configbus1[357:357] = sram_blwl_wl[357:357] ;
wire [357:357] mux_1level_tapbuf_size2_327_configbus0_b;
assign mux_1level_tapbuf_size2_327_configbus0_b[357:357] = sram_blwl_blb[357:357] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_327_ (mux_1level_tapbuf_size2_327_inbus, chany_1__1__out_55_ , mux_1level_tapbuf_size2_327_sram_blwl_out[357:357] ,
mux_1level_tapbuf_size2_327_sram_blwl_outb[357:357] );
//----- SRAM bits for MUX[327], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_357_ (mux_1level_tapbuf_size2_327_sram_blwl_out[357:357] ,mux_1level_tapbuf_size2_327_sram_blwl_out[357:357] ,mux_1level_tapbuf_size2_327_sram_blwl_outb[357:357] ,mux_1level_tapbuf_size2_327_configbus0[357:357], mux_1level_tapbuf_size2_327_configbus1[357:357] , mux_1level_tapbuf_size2_327_configbus0_b[357:357] );
wire [0:1] mux_1level_tapbuf_size2_328_inbus;
assign mux_1level_tapbuf_size2_328_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_328_inbus[1] = chanx_1__1__in_58_ ;
wire [358:358] mux_1level_tapbuf_size2_328_configbus0;
wire [358:358] mux_1level_tapbuf_size2_328_configbus1;
wire [358:358] mux_1level_tapbuf_size2_328_sram_blwl_out ;
wire [358:358] mux_1level_tapbuf_size2_328_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_328_configbus0[358:358] = sram_blwl_bl[358:358] ;
assign mux_1level_tapbuf_size2_328_configbus1[358:358] = sram_blwl_wl[358:358] ;
wire [358:358] mux_1level_tapbuf_size2_328_configbus0_b;
assign mux_1level_tapbuf_size2_328_configbus0_b[358:358] = sram_blwl_blb[358:358] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_328_ (mux_1level_tapbuf_size2_328_inbus, chany_1__1__out_57_ , mux_1level_tapbuf_size2_328_sram_blwl_out[358:358] ,
mux_1level_tapbuf_size2_328_sram_blwl_outb[358:358] );
//----- SRAM bits for MUX[328], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_358_ (mux_1level_tapbuf_size2_328_sram_blwl_out[358:358] ,mux_1level_tapbuf_size2_328_sram_blwl_out[358:358] ,mux_1level_tapbuf_size2_328_sram_blwl_outb[358:358] ,mux_1level_tapbuf_size2_328_configbus0[358:358], mux_1level_tapbuf_size2_328_configbus1[358:358] , mux_1level_tapbuf_size2_328_configbus0_b[358:358] );
wire [0:1] mux_1level_tapbuf_size2_329_inbus;
assign mux_1level_tapbuf_size2_329_inbus[0] =  grid_2__1__pin_0__3__5_;
assign mux_1level_tapbuf_size2_329_inbus[1] = chanx_1__1__in_60_ ;
wire [359:359] mux_1level_tapbuf_size2_329_configbus0;
wire [359:359] mux_1level_tapbuf_size2_329_configbus1;
wire [359:359] mux_1level_tapbuf_size2_329_sram_blwl_out ;
wire [359:359] mux_1level_tapbuf_size2_329_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_329_configbus0[359:359] = sram_blwl_bl[359:359] ;
assign mux_1level_tapbuf_size2_329_configbus1[359:359] = sram_blwl_wl[359:359] ;
wire [359:359] mux_1level_tapbuf_size2_329_configbus0_b;
assign mux_1level_tapbuf_size2_329_configbus0_b[359:359] = sram_blwl_blb[359:359] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_329_ (mux_1level_tapbuf_size2_329_inbus, chany_1__1__out_59_ , mux_1level_tapbuf_size2_329_sram_blwl_out[359:359] ,
mux_1level_tapbuf_size2_329_sram_blwl_outb[359:359] );
//----- SRAM bits for MUX[329], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_359_ (mux_1level_tapbuf_size2_329_sram_blwl_out[359:359] ,mux_1level_tapbuf_size2_329_sram_blwl_out[359:359] ,mux_1level_tapbuf_size2_329_sram_blwl_outb[359:359] ,mux_1level_tapbuf_size2_329_configbus0[359:359], mux_1level_tapbuf_size2_329_configbus1[359:359] , mux_1level_tapbuf_size2_329_configbus0_b[359:359] );
wire [0:1] mux_1level_tapbuf_size2_330_inbus;
assign mux_1level_tapbuf_size2_330_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_330_inbus[1] = chanx_1__1__in_62_ ;
wire [360:360] mux_1level_tapbuf_size2_330_configbus0;
wire [360:360] mux_1level_tapbuf_size2_330_configbus1;
wire [360:360] mux_1level_tapbuf_size2_330_sram_blwl_out ;
wire [360:360] mux_1level_tapbuf_size2_330_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_330_configbus0[360:360] = sram_blwl_bl[360:360] ;
assign mux_1level_tapbuf_size2_330_configbus1[360:360] = sram_blwl_wl[360:360] ;
wire [360:360] mux_1level_tapbuf_size2_330_configbus0_b;
assign mux_1level_tapbuf_size2_330_configbus0_b[360:360] = sram_blwl_blb[360:360] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_330_ (mux_1level_tapbuf_size2_330_inbus, chany_1__1__out_61_ , mux_1level_tapbuf_size2_330_sram_blwl_out[360:360] ,
mux_1level_tapbuf_size2_330_sram_blwl_outb[360:360] );
//----- SRAM bits for MUX[330], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_360_ (mux_1level_tapbuf_size2_330_sram_blwl_out[360:360] ,mux_1level_tapbuf_size2_330_sram_blwl_out[360:360] ,mux_1level_tapbuf_size2_330_sram_blwl_outb[360:360] ,mux_1level_tapbuf_size2_330_configbus0[360:360], mux_1level_tapbuf_size2_330_configbus1[360:360] , mux_1level_tapbuf_size2_330_configbus0_b[360:360] );
wire [0:1] mux_1level_tapbuf_size2_331_inbus;
assign mux_1level_tapbuf_size2_331_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_331_inbus[1] = chanx_1__1__in_64_ ;
wire [361:361] mux_1level_tapbuf_size2_331_configbus0;
wire [361:361] mux_1level_tapbuf_size2_331_configbus1;
wire [361:361] mux_1level_tapbuf_size2_331_sram_blwl_out ;
wire [361:361] mux_1level_tapbuf_size2_331_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_331_configbus0[361:361] = sram_blwl_bl[361:361] ;
assign mux_1level_tapbuf_size2_331_configbus1[361:361] = sram_blwl_wl[361:361] ;
wire [361:361] mux_1level_tapbuf_size2_331_configbus0_b;
assign mux_1level_tapbuf_size2_331_configbus0_b[361:361] = sram_blwl_blb[361:361] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_331_ (mux_1level_tapbuf_size2_331_inbus, chany_1__1__out_63_ , mux_1level_tapbuf_size2_331_sram_blwl_out[361:361] ,
mux_1level_tapbuf_size2_331_sram_blwl_outb[361:361] );
//----- SRAM bits for MUX[331], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_361_ (mux_1level_tapbuf_size2_331_sram_blwl_out[361:361] ,mux_1level_tapbuf_size2_331_sram_blwl_out[361:361] ,mux_1level_tapbuf_size2_331_sram_blwl_outb[361:361] ,mux_1level_tapbuf_size2_331_configbus0[361:361], mux_1level_tapbuf_size2_331_configbus1[361:361] , mux_1level_tapbuf_size2_331_configbus0_b[361:361] );
wire [0:1] mux_1level_tapbuf_size2_332_inbus;
assign mux_1level_tapbuf_size2_332_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_332_inbus[1] = chanx_1__1__in_66_ ;
wire [362:362] mux_1level_tapbuf_size2_332_configbus0;
wire [362:362] mux_1level_tapbuf_size2_332_configbus1;
wire [362:362] mux_1level_tapbuf_size2_332_sram_blwl_out ;
wire [362:362] mux_1level_tapbuf_size2_332_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_332_configbus0[362:362] = sram_blwl_bl[362:362] ;
assign mux_1level_tapbuf_size2_332_configbus1[362:362] = sram_blwl_wl[362:362] ;
wire [362:362] mux_1level_tapbuf_size2_332_configbus0_b;
assign mux_1level_tapbuf_size2_332_configbus0_b[362:362] = sram_blwl_blb[362:362] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_332_ (mux_1level_tapbuf_size2_332_inbus, chany_1__1__out_65_ , mux_1level_tapbuf_size2_332_sram_blwl_out[362:362] ,
mux_1level_tapbuf_size2_332_sram_blwl_outb[362:362] );
//----- SRAM bits for MUX[332], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_362_ (mux_1level_tapbuf_size2_332_sram_blwl_out[362:362] ,mux_1level_tapbuf_size2_332_sram_blwl_out[362:362] ,mux_1level_tapbuf_size2_332_sram_blwl_outb[362:362] ,mux_1level_tapbuf_size2_332_configbus0[362:362], mux_1level_tapbuf_size2_332_configbus1[362:362] , mux_1level_tapbuf_size2_332_configbus0_b[362:362] );
wire [0:1] mux_1level_tapbuf_size2_333_inbus;
assign mux_1level_tapbuf_size2_333_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_333_inbus[1] = chanx_1__1__in_68_ ;
wire [363:363] mux_1level_tapbuf_size2_333_configbus0;
wire [363:363] mux_1level_tapbuf_size2_333_configbus1;
wire [363:363] mux_1level_tapbuf_size2_333_sram_blwl_out ;
wire [363:363] mux_1level_tapbuf_size2_333_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_333_configbus0[363:363] = sram_blwl_bl[363:363] ;
assign mux_1level_tapbuf_size2_333_configbus1[363:363] = sram_blwl_wl[363:363] ;
wire [363:363] mux_1level_tapbuf_size2_333_configbus0_b;
assign mux_1level_tapbuf_size2_333_configbus0_b[363:363] = sram_blwl_blb[363:363] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_333_ (mux_1level_tapbuf_size2_333_inbus, chany_1__1__out_67_ , mux_1level_tapbuf_size2_333_sram_blwl_out[363:363] ,
mux_1level_tapbuf_size2_333_sram_blwl_outb[363:363] );
//----- SRAM bits for MUX[333], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_363_ (mux_1level_tapbuf_size2_333_sram_blwl_out[363:363] ,mux_1level_tapbuf_size2_333_sram_blwl_out[363:363] ,mux_1level_tapbuf_size2_333_sram_blwl_outb[363:363] ,mux_1level_tapbuf_size2_333_configbus0[363:363], mux_1level_tapbuf_size2_333_configbus1[363:363] , mux_1level_tapbuf_size2_333_configbus0_b[363:363] );
wire [0:1] mux_1level_tapbuf_size2_334_inbus;
assign mux_1level_tapbuf_size2_334_inbus[0] =  grid_2__1__pin_0__3__7_;
assign mux_1level_tapbuf_size2_334_inbus[1] = chanx_1__1__in_70_ ;
wire [364:364] mux_1level_tapbuf_size2_334_configbus0;
wire [364:364] mux_1level_tapbuf_size2_334_configbus1;
wire [364:364] mux_1level_tapbuf_size2_334_sram_blwl_out ;
wire [364:364] mux_1level_tapbuf_size2_334_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_334_configbus0[364:364] = sram_blwl_bl[364:364] ;
assign mux_1level_tapbuf_size2_334_configbus1[364:364] = sram_blwl_wl[364:364] ;
wire [364:364] mux_1level_tapbuf_size2_334_configbus0_b;
assign mux_1level_tapbuf_size2_334_configbus0_b[364:364] = sram_blwl_blb[364:364] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_334_ (mux_1level_tapbuf_size2_334_inbus, chany_1__1__out_69_ , mux_1level_tapbuf_size2_334_sram_blwl_out[364:364] ,
mux_1level_tapbuf_size2_334_sram_blwl_outb[364:364] );
//----- SRAM bits for MUX[334], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_364_ (mux_1level_tapbuf_size2_334_sram_blwl_out[364:364] ,mux_1level_tapbuf_size2_334_sram_blwl_out[364:364] ,mux_1level_tapbuf_size2_334_sram_blwl_outb[364:364] ,mux_1level_tapbuf_size2_334_configbus0[364:364], mux_1level_tapbuf_size2_334_configbus1[364:364] , mux_1level_tapbuf_size2_334_configbus0_b[364:364] );
wire [0:1] mux_1level_tapbuf_size2_335_inbus;
assign mux_1level_tapbuf_size2_335_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_335_inbus[1] = chanx_1__1__in_72_ ;
wire [365:365] mux_1level_tapbuf_size2_335_configbus0;
wire [365:365] mux_1level_tapbuf_size2_335_configbus1;
wire [365:365] mux_1level_tapbuf_size2_335_sram_blwl_out ;
wire [365:365] mux_1level_tapbuf_size2_335_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_335_configbus0[365:365] = sram_blwl_bl[365:365] ;
assign mux_1level_tapbuf_size2_335_configbus1[365:365] = sram_blwl_wl[365:365] ;
wire [365:365] mux_1level_tapbuf_size2_335_configbus0_b;
assign mux_1level_tapbuf_size2_335_configbus0_b[365:365] = sram_blwl_blb[365:365] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_335_ (mux_1level_tapbuf_size2_335_inbus, chany_1__1__out_71_ , mux_1level_tapbuf_size2_335_sram_blwl_out[365:365] ,
mux_1level_tapbuf_size2_335_sram_blwl_outb[365:365] );
//----- SRAM bits for MUX[335], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_365_ (mux_1level_tapbuf_size2_335_sram_blwl_out[365:365] ,mux_1level_tapbuf_size2_335_sram_blwl_out[365:365] ,mux_1level_tapbuf_size2_335_sram_blwl_outb[365:365] ,mux_1level_tapbuf_size2_335_configbus0[365:365], mux_1level_tapbuf_size2_335_configbus1[365:365] , mux_1level_tapbuf_size2_335_configbus0_b[365:365] );
wire [0:1] mux_1level_tapbuf_size2_336_inbus;
assign mux_1level_tapbuf_size2_336_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_336_inbus[1] = chanx_1__1__in_74_ ;
wire [366:366] mux_1level_tapbuf_size2_336_configbus0;
wire [366:366] mux_1level_tapbuf_size2_336_configbus1;
wire [366:366] mux_1level_tapbuf_size2_336_sram_blwl_out ;
wire [366:366] mux_1level_tapbuf_size2_336_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_336_configbus0[366:366] = sram_blwl_bl[366:366] ;
assign mux_1level_tapbuf_size2_336_configbus1[366:366] = sram_blwl_wl[366:366] ;
wire [366:366] mux_1level_tapbuf_size2_336_configbus0_b;
assign mux_1level_tapbuf_size2_336_configbus0_b[366:366] = sram_blwl_blb[366:366] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_336_ (mux_1level_tapbuf_size2_336_inbus, chany_1__1__out_73_ , mux_1level_tapbuf_size2_336_sram_blwl_out[366:366] ,
mux_1level_tapbuf_size2_336_sram_blwl_outb[366:366] );
//----- SRAM bits for MUX[336], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_366_ (mux_1level_tapbuf_size2_336_sram_blwl_out[366:366] ,mux_1level_tapbuf_size2_336_sram_blwl_out[366:366] ,mux_1level_tapbuf_size2_336_sram_blwl_outb[366:366] ,mux_1level_tapbuf_size2_336_configbus0[366:366], mux_1level_tapbuf_size2_336_configbus1[366:366] , mux_1level_tapbuf_size2_336_configbus0_b[366:366] );
wire [0:1] mux_1level_tapbuf_size2_337_inbus;
assign mux_1level_tapbuf_size2_337_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_337_inbus[1] = chanx_1__1__in_76_ ;
wire [367:367] mux_1level_tapbuf_size2_337_configbus0;
wire [367:367] mux_1level_tapbuf_size2_337_configbus1;
wire [367:367] mux_1level_tapbuf_size2_337_sram_blwl_out ;
wire [367:367] mux_1level_tapbuf_size2_337_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_337_configbus0[367:367] = sram_blwl_bl[367:367] ;
assign mux_1level_tapbuf_size2_337_configbus1[367:367] = sram_blwl_wl[367:367] ;
wire [367:367] mux_1level_tapbuf_size2_337_configbus0_b;
assign mux_1level_tapbuf_size2_337_configbus0_b[367:367] = sram_blwl_blb[367:367] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_337_ (mux_1level_tapbuf_size2_337_inbus, chany_1__1__out_75_ , mux_1level_tapbuf_size2_337_sram_blwl_out[367:367] ,
mux_1level_tapbuf_size2_337_sram_blwl_outb[367:367] );
//----- SRAM bits for MUX[337], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_367_ (mux_1level_tapbuf_size2_337_sram_blwl_out[367:367] ,mux_1level_tapbuf_size2_337_sram_blwl_out[367:367] ,mux_1level_tapbuf_size2_337_sram_blwl_outb[367:367] ,mux_1level_tapbuf_size2_337_configbus0[367:367], mux_1level_tapbuf_size2_337_configbus1[367:367] , mux_1level_tapbuf_size2_337_configbus0_b[367:367] );
wire [0:1] mux_1level_tapbuf_size2_338_inbus;
assign mux_1level_tapbuf_size2_338_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_338_inbus[1] = chanx_1__1__in_78_ ;
wire [368:368] mux_1level_tapbuf_size2_338_configbus0;
wire [368:368] mux_1level_tapbuf_size2_338_configbus1;
wire [368:368] mux_1level_tapbuf_size2_338_sram_blwl_out ;
wire [368:368] mux_1level_tapbuf_size2_338_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_338_configbus0[368:368] = sram_blwl_bl[368:368] ;
assign mux_1level_tapbuf_size2_338_configbus1[368:368] = sram_blwl_wl[368:368] ;
wire [368:368] mux_1level_tapbuf_size2_338_configbus0_b;
assign mux_1level_tapbuf_size2_338_configbus0_b[368:368] = sram_blwl_blb[368:368] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_338_ (mux_1level_tapbuf_size2_338_inbus, chany_1__1__out_77_ , mux_1level_tapbuf_size2_338_sram_blwl_out[368:368] ,
mux_1level_tapbuf_size2_338_sram_blwl_outb[368:368] );
//----- SRAM bits for MUX[338], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_368_ (mux_1level_tapbuf_size2_338_sram_blwl_out[368:368] ,mux_1level_tapbuf_size2_338_sram_blwl_out[368:368] ,mux_1level_tapbuf_size2_338_sram_blwl_outb[368:368] ,mux_1level_tapbuf_size2_338_configbus0[368:368], mux_1level_tapbuf_size2_338_configbus1[368:368] , mux_1level_tapbuf_size2_338_configbus0_b[368:368] );
wire [0:1] mux_1level_tapbuf_size2_339_inbus;
assign mux_1level_tapbuf_size2_339_inbus[0] =  grid_2__1__pin_0__3__9_;
assign mux_1level_tapbuf_size2_339_inbus[1] = chanx_1__1__in_80_ ;
wire [369:369] mux_1level_tapbuf_size2_339_configbus0;
wire [369:369] mux_1level_tapbuf_size2_339_configbus1;
wire [369:369] mux_1level_tapbuf_size2_339_sram_blwl_out ;
wire [369:369] mux_1level_tapbuf_size2_339_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_339_configbus0[369:369] = sram_blwl_bl[369:369] ;
assign mux_1level_tapbuf_size2_339_configbus1[369:369] = sram_blwl_wl[369:369] ;
wire [369:369] mux_1level_tapbuf_size2_339_configbus0_b;
assign mux_1level_tapbuf_size2_339_configbus0_b[369:369] = sram_blwl_blb[369:369] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_339_ (mux_1level_tapbuf_size2_339_inbus, chany_1__1__out_79_ , mux_1level_tapbuf_size2_339_sram_blwl_out[369:369] ,
mux_1level_tapbuf_size2_339_sram_blwl_outb[369:369] );
//----- SRAM bits for MUX[339], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_369_ (mux_1level_tapbuf_size2_339_sram_blwl_out[369:369] ,mux_1level_tapbuf_size2_339_sram_blwl_out[369:369] ,mux_1level_tapbuf_size2_339_sram_blwl_outb[369:369] ,mux_1level_tapbuf_size2_339_configbus0[369:369], mux_1level_tapbuf_size2_339_configbus1[369:369] , mux_1level_tapbuf_size2_339_configbus0_b[369:369] );
wire [0:1] mux_1level_tapbuf_size2_340_inbus;
assign mux_1level_tapbuf_size2_340_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_340_inbus[1] = chanx_1__1__in_82_ ;
wire [370:370] mux_1level_tapbuf_size2_340_configbus0;
wire [370:370] mux_1level_tapbuf_size2_340_configbus1;
wire [370:370] mux_1level_tapbuf_size2_340_sram_blwl_out ;
wire [370:370] mux_1level_tapbuf_size2_340_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_340_configbus0[370:370] = sram_blwl_bl[370:370] ;
assign mux_1level_tapbuf_size2_340_configbus1[370:370] = sram_blwl_wl[370:370] ;
wire [370:370] mux_1level_tapbuf_size2_340_configbus0_b;
assign mux_1level_tapbuf_size2_340_configbus0_b[370:370] = sram_blwl_blb[370:370] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_340_ (mux_1level_tapbuf_size2_340_inbus, chany_1__1__out_81_ , mux_1level_tapbuf_size2_340_sram_blwl_out[370:370] ,
mux_1level_tapbuf_size2_340_sram_blwl_outb[370:370] );
//----- SRAM bits for MUX[340], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_370_ (mux_1level_tapbuf_size2_340_sram_blwl_out[370:370] ,mux_1level_tapbuf_size2_340_sram_blwl_out[370:370] ,mux_1level_tapbuf_size2_340_sram_blwl_outb[370:370] ,mux_1level_tapbuf_size2_340_configbus0[370:370], mux_1level_tapbuf_size2_340_configbus1[370:370] , mux_1level_tapbuf_size2_340_configbus0_b[370:370] );
wire [0:1] mux_1level_tapbuf_size2_341_inbus;
assign mux_1level_tapbuf_size2_341_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_341_inbus[1] = chanx_1__1__in_84_ ;
wire [371:371] mux_1level_tapbuf_size2_341_configbus0;
wire [371:371] mux_1level_tapbuf_size2_341_configbus1;
wire [371:371] mux_1level_tapbuf_size2_341_sram_blwl_out ;
wire [371:371] mux_1level_tapbuf_size2_341_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_341_configbus0[371:371] = sram_blwl_bl[371:371] ;
assign mux_1level_tapbuf_size2_341_configbus1[371:371] = sram_blwl_wl[371:371] ;
wire [371:371] mux_1level_tapbuf_size2_341_configbus0_b;
assign mux_1level_tapbuf_size2_341_configbus0_b[371:371] = sram_blwl_blb[371:371] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_341_ (mux_1level_tapbuf_size2_341_inbus, chany_1__1__out_83_ , mux_1level_tapbuf_size2_341_sram_blwl_out[371:371] ,
mux_1level_tapbuf_size2_341_sram_blwl_outb[371:371] );
//----- SRAM bits for MUX[341], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_371_ (mux_1level_tapbuf_size2_341_sram_blwl_out[371:371] ,mux_1level_tapbuf_size2_341_sram_blwl_out[371:371] ,mux_1level_tapbuf_size2_341_sram_blwl_outb[371:371] ,mux_1level_tapbuf_size2_341_configbus0[371:371], mux_1level_tapbuf_size2_341_configbus1[371:371] , mux_1level_tapbuf_size2_341_configbus0_b[371:371] );
wire [0:1] mux_1level_tapbuf_size2_342_inbus;
assign mux_1level_tapbuf_size2_342_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_342_inbus[1] = chanx_1__1__in_86_ ;
wire [372:372] mux_1level_tapbuf_size2_342_configbus0;
wire [372:372] mux_1level_tapbuf_size2_342_configbus1;
wire [372:372] mux_1level_tapbuf_size2_342_sram_blwl_out ;
wire [372:372] mux_1level_tapbuf_size2_342_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_342_configbus0[372:372] = sram_blwl_bl[372:372] ;
assign mux_1level_tapbuf_size2_342_configbus1[372:372] = sram_blwl_wl[372:372] ;
wire [372:372] mux_1level_tapbuf_size2_342_configbus0_b;
assign mux_1level_tapbuf_size2_342_configbus0_b[372:372] = sram_blwl_blb[372:372] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_342_ (mux_1level_tapbuf_size2_342_inbus, chany_1__1__out_85_ , mux_1level_tapbuf_size2_342_sram_blwl_out[372:372] ,
mux_1level_tapbuf_size2_342_sram_blwl_outb[372:372] );
//----- SRAM bits for MUX[342], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_372_ (mux_1level_tapbuf_size2_342_sram_blwl_out[372:372] ,mux_1level_tapbuf_size2_342_sram_blwl_out[372:372] ,mux_1level_tapbuf_size2_342_sram_blwl_outb[372:372] ,mux_1level_tapbuf_size2_342_configbus0[372:372], mux_1level_tapbuf_size2_342_configbus1[372:372] , mux_1level_tapbuf_size2_342_configbus0_b[372:372] );
wire [0:1] mux_1level_tapbuf_size2_343_inbus;
assign mux_1level_tapbuf_size2_343_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_343_inbus[1] = chanx_1__1__in_88_ ;
wire [373:373] mux_1level_tapbuf_size2_343_configbus0;
wire [373:373] mux_1level_tapbuf_size2_343_configbus1;
wire [373:373] mux_1level_tapbuf_size2_343_sram_blwl_out ;
wire [373:373] mux_1level_tapbuf_size2_343_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_343_configbus0[373:373] = sram_blwl_bl[373:373] ;
assign mux_1level_tapbuf_size2_343_configbus1[373:373] = sram_blwl_wl[373:373] ;
wire [373:373] mux_1level_tapbuf_size2_343_configbus0_b;
assign mux_1level_tapbuf_size2_343_configbus0_b[373:373] = sram_blwl_blb[373:373] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_343_ (mux_1level_tapbuf_size2_343_inbus, chany_1__1__out_87_ , mux_1level_tapbuf_size2_343_sram_blwl_out[373:373] ,
mux_1level_tapbuf_size2_343_sram_blwl_outb[373:373] );
//----- SRAM bits for MUX[343], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_373_ (mux_1level_tapbuf_size2_343_sram_blwl_out[373:373] ,mux_1level_tapbuf_size2_343_sram_blwl_out[373:373] ,mux_1level_tapbuf_size2_343_sram_blwl_outb[373:373] ,mux_1level_tapbuf_size2_343_configbus0[373:373], mux_1level_tapbuf_size2_343_configbus1[373:373] , mux_1level_tapbuf_size2_343_configbus0_b[373:373] );
wire [0:1] mux_1level_tapbuf_size2_344_inbus;
assign mux_1level_tapbuf_size2_344_inbus[0] =  grid_2__1__pin_0__3__11_;
assign mux_1level_tapbuf_size2_344_inbus[1] = chanx_1__1__in_90_ ;
wire [374:374] mux_1level_tapbuf_size2_344_configbus0;
wire [374:374] mux_1level_tapbuf_size2_344_configbus1;
wire [374:374] mux_1level_tapbuf_size2_344_sram_blwl_out ;
wire [374:374] mux_1level_tapbuf_size2_344_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_344_configbus0[374:374] = sram_blwl_bl[374:374] ;
assign mux_1level_tapbuf_size2_344_configbus1[374:374] = sram_blwl_wl[374:374] ;
wire [374:374] mux_1level_tapbuf_size2_344_configbus0_b;
assign mux_1level_tapbuf_size2_344_configbus0_b[374:374] = sram_blwl_blb[374:374] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_344_ (mux_1level_tapbuf_size2_344_inbus, chany_1__1__out_89_ , mux_1level_tapbuf_size2_344_sram_blwl_out[374:374] ,
mux_1level_tapbuf_size2_344_sram_blwl_outb[374:374] );
//----- SRAM bits for MUX[344], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_374_ (mux_1level_tapbuf_size2_344_sram_blwl_out[374:374] ,mux_1level_tapbuf_size2_344_sram_blwl_out[374:374] ,mux_1level_tapbuf_size2_344_sram_blwl_outb[374:374] ,mux_1level_tapbuf_size2_344_configbus0[374:374], mux_1level_tapbuf_size2_344_configbus1[374:374] , mux_1level_tapbuf_size2_344_configbus0_b[374:374] );
wire [0:1] mux_1level_tapbuf_size2_345_inbus;
assign mux_1level_tapbuf_size2_345_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_345_inbus[1] = chanx_1__1__in_92_ ;
wire [375:375] mux_1level_tapbuf_size2_345_configbus0;
wire [375:375] mux_1level_tapbuf_size2_345_configbus1;
wire [375:375] mux_1level_tapbuf_size2_345_sram_blwl_out ;
wire [375:375] mux_1level_tapbuf_size2_345_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_345_configbus0[375:375] = sram_blwl_bl[375:375] ;
assign mux_1level_tapbuf_size2_345_configbus1[375:375] = sram_blwl_wl[375:375] ;
wire [375:375] mux_1level_tapbuf_size2_345_configbus0_b;
assign mux_1level_tapbuf_size2_345_configbus0_b[375:375] = sram_blwl_blb[375:375] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_345_ (mux_1level_tapbuf_size2_345_inbus, chany_1__1__out_91_ , mux_1level_tapbuf_size2_345_sram_blwl_out[375:375] ,
mux_1level_tapbuf_size2_345_sram_blwl_outb[375:375] );
//----- SRAM bits for MUX[345], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_375_ (mux_1level_tapbuf_size2_345_sram_blwl_out[375:375] ,mux_1level_tapbuf_size2_345_sram_blwl_out[375:375] ,mux_1level_tapbuf_size2_345_sram_blwl_outb[375:375] ,mux_1level_tapbuf_size2_345_configbus0[375:375], mux_1level_tapbuf_size2_345_configbus1[375:375] , mux_1level_tapbuf_size2_345_configbus0_b[375:375] );
wire [0:1] mux_1level_tapbuf_size2_346_inbus;
assign mux_1level_tapbuf_size2_346_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_346_inbus[1] = chanx_1__1__in_94_ ;
wire [376:376] mux_1level_tapbuf_size2_346_configbus0;
wire [376:376] mux_1level_tapbuf_size2_346_configbus1;
wire [376:376] mux_1level_tapbuf_size2_346_sram_blwl_out ;
wire [376:376] mux_1level_tapbuf_size2_346_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_346_configbus0[376:376] = sram_blwl_bl[376:376] ;
assign mux_1level_tapbuf_size2_346_configbus1[376:376] = sram_blwl_wl[376:376] ;
wire [376:376] mux_1level_tapbuf_size2_346_configbus0_b;
assign mux_1level_tapbuf_size2_346_configbus0_b[376:376] = sram_blwl_blb[376:376] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_346_ (mux_1level_tapbuf_size2_346_inbus, chany_1__1__out_93_ , mux_1level_tapbuf_size2_346_sram_blwl_out[376:376] ,
mux_1level_tapbuf_size2_346_sram_blwl_outb[376:376] );
//----- SRAM bits for MUX[346], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_376_ (mux_1level_tapbuf_size2_346_sram_blwl_out[376:376] ,mux_1level_tapbuf_size2_346_sram_blwl_out[376:376] ,mux_1level_tapbuf_size2_346_sram_blwl_outb[376:376] ,mux_1level_tapbuf_size2_346_configbus0[376:376], mux_1level_tapbuf_size2_346_configbus1[376:376] , mux_1level_tapbuf_size2_346_configbus0_b[376:376] );
wire [0:1] mux_1level_tapbuf_size2_347_inbus;
assign mux_1level_tapbuf_size2_347_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_347_inbus[1] = chanx_1__1__in_96_ ;
wire [377:377] mux_1level_tapbuf_size2_347_configbus0;
wire [377:377] mux_1level_tapbuf_size2_347_configbus1;
wire [377:377] mux_1level_tapbuf_size2_347_sram_blwl_out ;
wire [377:377] mux_1level_tapbuf_size2_347_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_347_configbus0[377:377] = sram_blwl_bl[377:377] ;
assign mux_1level_tapbuf_size2_347_configbus1[377:377] = sram_blwl_wl[377:377] ;
wire [377:377] mux_1level_tapbuf_size2_347_configbus0_b;
assign mux_1level_tapbuf_size2_347_configbus0_b[377:377] = sram_blwl_blb[377:377] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_347_ (mux_1level_tapbuf_size2_347_inbus, chany_1__1__out_95_ , mux_1level_tapbuf_size2_347_sram_blwl_out[377:377] ,
mux_1level_tapbuf_size2_347_sram_blwl_outb[377:377] );
//----- SRAM bits for MUX[347], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_377_ (mux_1level_tapbuf_size2_347_sram_blwl_out[377:377] ,mux_1level_tapbuf_size2_347_sram_blwl_out[377:377] ,mux_1level_tapbuf_size2_347_sram_blwl_outb[377:377] ,mux_1level_tapbuf_size2_347_configbus0[377:377], mux_1level_tapbuf_size2_347_configbus1[377:377] , mux_1level_tapbuf_size2_347_configbus0_b[377:377] );
wire [0:1] mux_1level_tapbuf_size2_348_inbus;
assign mux_1level_tapbuf_size2_348_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_348_inbus[1] = chanx_1__1__in_98_ ;
wire [378:378] mux_1level_tapbuf_size2_348_configbus0;
wire [378:378] mux_1level_tapbuf_size2_348_configbus1;
wire [378:378] mux_1level_tapbuf_size2_348_sram_blwl_out ;
wire [378:378] mux_1level_tapbuf_size2_348_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_348_configbus0[378:378] = sram_blwl_bl[378:378] ;
assign mux_1level_tapbuf_size2_348_configbus1[378:378] = sram_blwl_wl[378:378] ;
wire [378:378] mux_1level_tapbuf_size2_348_configbus0_b;
assign mux_1level_tapbuf_size2_348_configbus0_b[378:378] = sram_blwl_blb[378:378] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_348_ (mux_1level_tapbuf_size2_348_inbus, chany_1__1__out_97_ , mux_1level_tapbuf_size2_348_sram_blwl_out[378:378] ,
mux_1level_tapbuf_size2_348_sram_blwl_outb[378:378] );
//----- SRAM bits for MUX[348], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_378_ (mux_1level_tapbuf_size2_348_sram_blwl_out[378:378] ,mux_1level_tapbuf_size2_348_sram_blwl_out[378:378] ,mux_1level_tapbuf_size2_348_sram_blwl_outb[378:378] ,mux_1level_tapbuf_size2_348_configbus0[378:378], mux_1level_tapbuf_size2_348_configbus1[378:378] , mux_1level_tapbuf_size2_348_configbus0_b[378:378] );
wire [0:1] mux_1level_tapbuf_size2_349_inbus;
assign mux_1level_tapbuf_size2_349_inbus[0] =  grid_2__1__pin_0__3__13_;
assign mux_1level_tapbuf_size2_349_inbus[1] = chanx_1__1__in_0_ ;
wire [379:379] mux_1level_tapbuf_size2_349_configbus0;
wire [379:379] mux_1level_tapbuf_size2_349_configbus1;
wire [379:379] mux_1level_tapbuf_size2_349_sram_blwl_out ;
wire [379:379] mux_1level_tapbuf_size2_349_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_349_configbus0[379:379] = sram_blwl_bl[379:379] ;
assign mux_1level_tapbuf_size2_349_configbus1[379:379] = sram_blwl_wl[379:379] ;
wire [379:379] mux_1level_tapbuf_size2_349_configbus0_b;
assign mux_1level_tapbuf_size2_349_configbus0_b[379:379] = sram_blwl_blb[379:379] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_349_ (mux_1level_tapbuf_size2_349_inbus, chany_1__1__out_99_ , mux_1level_tapbuf_size2_349_sram_blwl_out[379:379] ,
mux_1level_tapbuf_size2_349_sram_blwl_outb[379:379] );
//----- SRAM bits for MUX[349], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_379_ (mux_1level_tapbuf_size2_349_sram_blwl_out[379:379] ,mux_1level_tapbuf_size2_349_sram_blwl_out[379:379] ,mux_1level_tapbuf_size2_349_sram_blwl_outb[379:379] ,mux_1level_tapbuf_size2_349_configbus0[379:379], mux_1level_tapbuf_size2_349_configbus1[379:379] , mux_1level_tapbuf_size2_349_configbus0_b[379:379] );
//----- left side Multiplexers -----
wire [0:2] mux_1level_tapbuf_size3_350_inbus;
assign mux_1level_tapbuf_size3_350_inbus[0] =  grid_1__1__pin_0__0__40_;
assign mux_1level_tapbuf_size3_350_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_350_inbus[2] = chany_1__1__in_98_ ;
wire [380:382] mux_1level_tapbuf_size3_350_configbus0;
wire [380:382] mux_1level_tapbuf_size3_350_configbus1;
wire [380:382] mux_1level_tapbuf_size3_350_sram_blwl_out ;
wire [380:382] mux_1level_tapbuf_size3_350_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_350_configbus0[380:382] = sram_blwl_bl[380:382] ;
assign mux_1level_tapbuf_size3_350_configbus1[380:382] = sram_blwl_wl[380:382] ;
wire [380:382] mux_1level_tapbuf_size3_350_configbus0_b;
assign mux_1level_tapbuf_size3_350_configbus0_b[380:382] = sram_blwl_blb[380:382] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_350_ (mux_1level_tapbuf_size3_350_inbus, chanx_1__1__out_1_ , mux_1level_tapbuf_size3_350_sram_blwl_out[380:382] ,
mux_1level_tapbuf_size3_350_sram_blwl_outb[380:382] );
//----- SRAM bits for MUX[350], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_380_ (mux_1level_tapbuf_size3_350_sram_blwl_out[380:380] ,mux_1level_tapbuf_size3_350_sram_blwl_out[380:380] ,mux_1level_tapbuf_size3_350_sram_blwl_outb[380:380] ,mux_1level_tapbuf_size3_350_configbus0[380:380], mux_1level_tapbuf_size3_350_configbus1[380:380] , mux_1level_tapbuf_size3_350_configbus0_b[380:380] );
sram6T_blwl sram_blwl_381_ (mux_1level_tapbuf_size3_350_sram_blwl_out[381:381] ,mux_1level_tapbuf_size3_350_sram_blwl_out[381:381] ,mux_1level_tapbuf_size3_350_sram_blwl_outb[381:381] ,mux_1level_tapbuf_size3_350_configbus0[381:381], mux_1level_tapbuf_size3_350_configbus1[381:381] , mux_1level_tapbuf_size3_350_configbus0_b[381:381] );
sram6T_blwl sram_blwl_382_ (mux_1level_tapbuf_size3_350_sram_blwl_out[382:382] ,mux_1level_tapbuf_size3_350_sram_blwl_out[382:382] ,mux_1level_tapbuf_size3_350_sram_blwl_outb[382:382] ,mux_1level_tapbuf_size3_350_configbus0[382:382], mux_1level_tapbuf_size3_350_configbus1[382:382] , mux_1level_tapbuf_size3_350_configbus0_b[382:382] );
wire [0:2] mux_1level_tapbuf_size3_351_inbus;
assign mux_1level_tapbuf_size3_351_inbus[0] =  grid_1__1__pin_0__0__40_;
assign mux_1level_tapbuf_size3_351_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_351_inbus[2] = chany_1__1__in_0_ ;
wire [383:385] mux_1level_tapbuf_size3_351_configbus0;
wire [383:385] mux_1level_tapbuf_size3_351_configbus1;
wire [383:385] mux_1level_tapbuf_size3_351_sram_blwl_out ;
wire [383:385] mux_1level_tapbuf_size3_351_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_351_configbus0[383:385] = sram_blwl_bl[383:385] ;
assign mux_1level_tapbuf_size3_351_configbus1[383:385] = sram_blwl_wl[383:385] ;
wire [383:385] mux_1level_tapbuf_size3_351_configbus0_b;
assign mux_1level_tapbuf_size3_351_configbus0_b[383:385] = sram_blwl_blb[383:385] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_351_ (mux_1level_tapbuf_size3_351_inbus, chanx_1__1__out_3_ , mux_1level_tapbuf_size3_351_sram_blwl_out[383:385] ,
mux_1level_tapbuf_size3_351_sram_blwl_outb[383:385] );
//----- SRAM bits for MUX[351], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_383_ (mux_1level_tapbuf_size3_351_sram_blwl_out[383:383] ,mux_1level_tapbuf_size3_351_sram_blwl_out[383:383] ,mux_1level_tapbuf_size3_351_sram_blwl_outb[383:383] ,mux_1level_tapbuf_size3_351_configbus0[383:383], mux_1level_tapbuf_size3_351_configbus1[383:383] , mux_1level_tapbuf_size3_351_configbus0_b[383:383] );
sram6T_blwl sram_blwl_384_ (mux_1level_tapbuf_size3_351_sram_blwl_out[384:384] ,mux_1level_tapbuf_size3_351_sram_blwl_out[384:384] ,mux_1level_tapbuf_size3_351_sram_blwl_outb[384:384] ,mux_1level_tapbuf_size3_351_configbus0[384:384], mux_1level_tapbuf_size3_351_configbus1[384:384] , mux_1level_tapbuf_size3_351_configbus0_b[384:384] );
sram6T_blwl sram_blwl_385_ (mux_1level_tapbuf_size3_351_sram_blwl_out[385:385] ,mux_1level_tapbuf_size3_351_sram_blwl_out[385:385] ,mux_1level_tapbuf_size3_351_sram_blwl_outb[385:385] ,mux_1level_tapbuf_size3_351_configbus0[385:385], mux_1level_tapbuf_size3_351_configbus1[385:385] , mux_1level_tapbuf_size3_351_configbus0_b[385:385] );
wire [0:2] mux_1level_tapbuf_size3_352_inbus;
assign mux_1level_tapbuf_size3_352_inbus[0] =  grid_1__1__pin_0__0__40_;
assign mux_1level_tapbuf_size3_352_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_352_inbus[2] = chany_1__1__in_2_ ;
wire [386:388] mux_1level_tapbuf_size3_352_configbus0;
wire [386:388] mux_1level_tapbuf_size3_352_configbus1;
wire [386:388] mux_1level_tapbuf_size3_352_sram_blwl_out ;
wire [386:388] mux_1level_tapbuf_size3_352_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_352_configbus0[386:388] = sram_blwl_bl[386:388] ;
assign mux_1level_tapbuf_size3_352_configbus1[386:388] = sram_blwl_wl[386:388] ;
wire [386:388] mux_1level_tapbuf_size3_352_configbus0_b;
assign mux_1level_tapbuf_size3_352_configbus0_b[386:388] = sram_blwl_blb[386:388] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_352_ (mux_1level_tapbuf_size3_352_inbus, chanx_1__1__out_5_ , mux_1level_tapbuf_size3_352_sram_blwl_out[386:388] ,
mux_1level_tapbuf_size3_352_sram_blwl_outb[386:388] );
//----- SRAM bits for MUX[352], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_386_ (mux_1level_tapbuf_size3_352_sram_blwl_out[386:386] ,mux_1level_tapbuf_size3_352_sram_blwl_out[386:386] ,mux_1level_tapbuf_size3_352_sram_blwl_outb[386:386] ,mux_1level_tapbuf_size3_352_configbus0[386:386], mux_1level_tapbuf_size3_352_configbus1[386:386] , mux_1level_tapbuf_size3_352_configbus0_b[386:386] );
sram6T_blwl sram_blwl_387_ (mux_1level_tapbuf_size3_352_sram_blwl_out[387:387] ,mux_1level_tapbuf_size3_352_sram_blwl_out[387:387] ,mux_1level_tapbuf_size3_352_sram_blwl_outb[387:387] ,mux_1level_tapbuf_size3_352_configbus0[387:387], mux_1level_tapbuf_size3_352_configbus1[387:387] , mux_1level_tapbuf_size3_352_configbus0_b[387:387] );
sram6T_blwl sram_blwl_388_ (mux_1level_tapbuf_size3_352_sram_blwl_out[388:388] ,mux_1level_tapbuf_size3_352_sram_blwl_out[388:388] ,mux_1level_tapbuf_size3_352_sram_blwl_outb[388:388] ,mux_1level_tapbuf_size3_352_configbus0[388:388], mux_1level_tapbuf_size3_352_configbus1[388:388] , mux_1level_tapbuf_size3_352_configbus0_b[388:388] );
wire [0:2] mux_1level_tapbuf_size3_353_inbus;
assign mux_1level_tapbuf_size3_353_inbus[0] =  grid_1__1__pin_0__0__40_;
assign mux_1level_tapbuf_size3_353_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_353_inbus[2] = chany_1__1__in_4_ ;
wire [389:391] mux_1level_tapbuf_size3_353_configbus0;
wire [389:391] mux_1level_tapbuf_size3_353_configbus1;
wire [389:391] mux_1level_tapbuf_size3_353_sram_blwl_out ;
wire [389:391] mux_1level_tapbuf_size3_353_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_353_configbus0[389:391] = sram_blwl_bl[389:391] ;
assign mux_1level_tapbuf_size3_353_configbus1[389:391] = sram_blwl_wl[389:391] ;
wire [389:391] mux_1level_tapbuf_size3_353_configbus0_b;
assign mux_1level_tapbuf_size3_353_configbus0_b[389:391] = sram_blwl_blb[389:391] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_353_ (mux_1level_tapbuf_size3_353_inbus, chanx_1__1__out_7_ , mux_1level_tapbuf_size3_353_sram_blwl_out[389:391] ,
mux_1level_tapbuf_size3_353_sram_blwl_outb[389:391] );
//----- SRAM bits for MUX[353], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_389_ (mux_1level_tapbuf_size3_353_sram_blwl_out[389:389] ,mux_1level_tapbuf_size3_353_sram_blwl_out[389:389] ,mux_1level_tapbuf_size3_353_sram_blwl_outb[389:389] ,mux_1level_tapbuf_size3_353_configbus0[389:389], mux_1level_tapbuf_size3_353_configbus1[389:389] , mux_1level_tapbuf_size3_353_configbus0_b[389:389] );
sram6T_blwl sram_blwl_390_ (mux_1level_tapbuf_size3_353_sram_blwl_out[390:390] ,mux_1level_tapbuf_size3_353_sram_blwl_out[390:390] ,mux_1level_tapbuf_size3_353_sram_blwl_outb[390:390] ,mux_1level_tapbuf_size3_353_configbus0[390:390], mux_1level_tapbuf_size3_353_configbus1[390:390] , mux_1level_tapbuf_size3_353_configbus0_b[390:390] );
sram6T_blwl sram_blwl_391_ (mux_1level_tapbuf_size3_353_sram_blwl_out[391:391] ,mux_1level_tapbuf_size3_353_sram_blwl_out[391:391] ,mux_1level_tapbuf_size3_353_sram_blwl_outb[391:391] ,mux_1level_tapbuf_size3_353_configbus0[391:391], mux_1level_tapbuf_size3_353_configbus1[391:391] , mux_1level_tapbuf_size3_353_configbus0_b[391:391] );
wire [0:2] mux_1level_tapbuf_size3_354_inbus;
assign mux_1level_tapbuf_size3_354_inbus[0] =  grid_1__1__pin_0__0__40_;
assign mux_1level_tapbuf_size3_354_inbus[1] =  grid_1__2__pin_0__2__15_;
assign mux_1level_tapbuf_size3_354_inbus[2] = chany_1__1__in_6_ ;
wire [392:394] mux_1level_tapbuf_size3_354_configbus0;
wire [392:394] mux_1level_tapbuf_size3_354_configbus1;
wire [392:394] mux_1level_tapbuf_size3_354_sram_blwl_out ;
wire [392:394] mux_1level_tapbuf_size3_354_sram_blwl_outb ;
assign mux_1level_tapbuf_size3_354_configbus0[392:394] = sram_blwl_bl[392:394] ;
assign mux_1level_tapbuf_size3_354_configbus1[392:394] = sram_blwl_wl[392:394] ;
wire [392:394] mux_1level_tapbuf_size3_354_configbus0_b;
assign mux_1level_tapbuf_size3_354_configbus0_b[392:394] = sram_blwl_blb[392:394] ;
mux_1level_tapbuf_size3 mux_1level_tapbuf_size3_354_ (mux_1level_tapbuf_size3_354_inbus, chanx_1__1__out_9_ , mux_1level_tapbuf_size3_354_sram_blwl_out[392:394] ,
mux_1level_tapbuf_size3_354_sram_blwl_outb[392:394] );
//----- SRAM bits for MUX[354], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100-----
sram6T_blwl sram_blwl_392_ (mux_1level_tapbuf_size3_354_sram_blwl_out[392:392] ,mux_1level_tapbuf_size3_354_sram_blwl_out[392:392] ,mux_1level_tapbuf_size3_354_sram_blwl_outb[392:392] ,mux_1level_tapbuf_size3_354_configbus0[392:392], mux_1level_tapbuf_size3_354_configbus1[392:392] , mux_1level_tapbuf_size3_354_configbus0_b[392:392] );
sram6T_blwl sram_blwl_393_ (mux_1level_tapbuf_size3_354_sram_blwl_out[393:393] ,mux_1level_tapbuf_size3_354_sram_blwl_out[393:393] ,mux_1level_tapbuf_size3_354_sram_blwl_outb[393:393] ,mux_1level_tapbuf_size3_354_configbus0[393:393], mux_1level_tapbuf_size3_354_configbus1[393:393] , mux_1level_tapbuf_size3_354_configbus0_b[393:393] );
sram6T_blwl sram_blwl_394_ (mux_1level_tapbuf_size3_354_sram_blwl_out[394:394] ,mux_1level_tapbuf_size3_354_sram_blwl_out[394:394] ,mux_1level_tapbuf_size3_354_sram_blwl_outb[394:394] ,mux_1level_tapbuf_size3_354_configbus0[394:394], mux_1level_tapbuf_size3_354_configbus1[394:394] , mux_1level_tapbuf_size3_354_configbus0_b[394:394] );
wire [0:1] mux_1level_tapbuf_size2_355_inbus;
assign mux_1level_tapbuf_size2_355_inbus[0] =  grid_1__1__pin_0__0__44_;
assign mux_1level_tapbuf_size2_355_inbus[1] = chany_1__1__in_8_ ;
wire [395:395] mux_1level_tapbuf_size2_355_configbus0;
wire [395:395] mux_1level_tapbuf_size2_355_configbus1;
wire [395:395] mux_1level_tapbuf_size2_355_sram_blwl_out ;
wire [395:395] mux_1level_tapbuf_size2_355_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_355_configbus0[395:395] = sram_blwl_bl[395:395] ;
assign mux_1level_tapbuf_size2_355_configbus1[395:395] = sram_blwl_wl[395:395] ;
wire [395:395] mux_1level_tapbuf_size2_355_configbus0_b;
assign mux_1level_tapbuf_size2_355_configbus0_b[395:395] = sram_blwl_blb[395:395] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_355_ (mux_1level_tapbuf_size2_355_inbus, chanx_1__1__out_11_ , mux_1level_tapbuf_size2_355_sram_blwl_out[395:395] ,
mux_1level_tapbuf_size2_355_sram_blwl_outb[395:395] );
//----- SRAM bits for MUX[355], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_395_ (mux_1level_tapbuf_size2_355_sram_blwl_out[395:395] ,mux_1level_tapbuf_size2_355_sram_blwl_out[395:395] ,mux_1level_tapbuf_size2_355_sram_blwl_outb[395:395] ,mux_1level_tapbuf_size2_355_configbus0[395:395], mux_1level_tapbuf_size2_355_configbus1[395:395] , mux_1level_tapbuf_size2_355_configbus0_b[395:395] );
wire [0:1] mux_1level_tapbuf_size2_356_inbus;
assign mux_1level_tapbuf_size2_356_inbus[0] =  grid_1__1__pin_0__0__44_;
assign mux_1level_tapbuf_size2_356_inbus[1] = chany_1__1__in_10_ ;
wire [396:396] mux_1level_tapbuf_size2_356_configbus0;
wire [396:396] mux_1level_tapbuf_size2_356_configbus1;
wire [396:396] mux_1level_tapbuf_size2_356_sram_blwl_out ;
wire [396:396] mux_1level_tapbuf_size2_356_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_356_configbus0[396:396] = sram_blwl_bl[396:396] ;
assign mux_1level_tapbuf_size2_356_configbus1[396:396] = sram_blwl_wl[396:396] ;
wire [396:396] mux_1level_tapbuf_size2_356_configbus0_b;
assign mux_1level_tapbuf_size2_356_configbus0_b[396:396] = sram_blwl_blb[396:396] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_356_ (mux_1level_tapbuf_size2_356_inbus, chanx_1__1__out_13_ , mux_1level_tapbuf_size2_356_sram_blwl_out[396:396] ,
mux_1level_tapbuf_size2_356_sram_blwl_outb[396:396] );
//----- SRAM bits for MUX[356], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_396_ (mux_1level_tapbuf_size2_356_sram_blwl_out[396:396] ,mux_1level_tapbuf_size2_356_sram_blwl_out[396:396] ,mux_1level_tapbuf_size2_356_sram_blwl_outb[396:396] ,mux_1level_tapbuf_size2_356_configbus0[396:396], mux_1level_tapbuf_size2_356_configbus1[396:396] , mux_1level_tapbuf_size2_356_configbus0_b[396:396] );
wire [0:1] mux_1level_tapbuf_size2_357_inbus;
assign mux_1level_tapbuf_size2_357_inbus[0] =  grid_1__1__pin_0__0__44_;
assign mux_1level_tapbuf_size2_357_inbus[1] = chany_1__1__in_12_ ;
wire [397:397] mux_1level_tapbuf_size2_357_configbus0;
wire [397:397] mux_1level_tapbuf_size2_357_configbus1;
wire [397:397] mux_1level_tapbuf_size2_357_sram_blwl_out ;
wire [397:397] mux_1level_tapbuf_size2_357_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_357_configbus0[397:397] = sram_blwl_bl[397:397] ;
assign mux_1level_tapbuf_size2_357_configbus1[397:397] = sram_blwl_wl[397:397] ;
wire [397:397] mux_1level_tapbuf_size2_357_configbus0_b;
assign mux_1level_tapbuf_size2_357_configbus0_b[397:397] = sram_blwl_blb[397:397] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_357_ (mux_1level_tapbuf_size2_357_inbus, chanx_1__1__out_15_ , mux_1level_tapbuf_size2_357_sram_blwl_out[397:397] ,
mux_1level_tapbuf_size2_357_sram_blwl_outb[397:397] );
//----- SRAM bits for MUX[357], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_397_ (mux_1level_tapbuf_size2_357_sram_blwl_out[397:397] ,mux_1level_tapbuf_size2_357_sram_blwl_out[397:397] ,mux_1level_tapbuf_size2_357_sram_blwl_outb[397:397] ,mux_1level_tapbuf_size2_357_configbus0[397:397], mux_1level_tapbuf_size2_357_configbus1[397:397] , mux_1level_tapbuf_size2_357_configbus0_b[397:397] );
wire [0:1] mux_1level_tapbuf_size2_358_inbus;
assign mux_1level_tapbuf_size2_358_inbus[0] =  grid_1__1__pin_0__0__44_;
assign mux_1level_tapbuf_size2_358_inbus[1] = chany_1__1__in_14_ ;
wire [398:398] mux_1level_tapbuf_size2_358_configbus0;
wire [398:398] mux_1level_tapbuf_size2_358_configbus1;
wire [398:398] mux_1level_tapbuf_size2_358_sram_blwl_out ;
wire [398:398] mux_1level_tapbuf_size2_358_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_358_configbus0[398:398] = sram_blwl_bl[398:398] ;
assign mux_1level_tapbuf_size2_358_configbus1[398:398] = sram_blwl_wl[398:398] ;
wire [398:398] mux_1level_tapbuf_size2_358_configbus0_b;
assign mux_1level_tapbuf_size2_358_configbus0_b[398:398] = sram_blwl_blb[398:398] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_358_ (mux_1level_tapbuf_size2_358_inbus, chanx_1__1__out_17_ , mux_1level_tapbuf_size2_358_sram_blwl_out[398:398] ,
mux_1level_tapbuf_size2_358_sram_blwl_outb[398:398] );
//----- SRAM bits for MUX[358], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_398_ (mux_1level_tapbuf_size2_358_sram_blwl_out[398:398] ,mux_1level_tapbuf_size2_358_sram_blwl_out[398:398] ,mux_1level_tapbuf_size2_358_sram_blwl_outb[398:398] ,mux_1level_tapbuf_size2_358_configbus0[398:398], mux_1level_tapbuf_size2_358_configbus1[398:398] , mux_1level_tapbuf_size2_358_configbus0_b[398:398] );
wire [0:1] mux_1level_tapbuf_size2_359_inbus;
assign mux_1level_tapbuf_size2_359_inbus[0] =  grid_1__1__pin_0__0__44_;
assign mux_1level_tapbuf_size2_359_inbus[1] = chany_1__1__in_16_ ;
wire [399:399] mux_1level_tapbuf_size2_359_configbus0;
wire [399:399] mux_1level_tapbuf_size2_359_configbus1;
wire [399:399] mux_1level_tapbuf_size2_359_sram_blwl_out ;
wire [399:399] mux_1level_tapbuf_size2_359_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_359_configbus0[399:399] = sram_blwl_bl[399:399] ;
assign mux_1level_tapbuf_size2_359_configbus1[399:399] = sram_blwl_wl[399:399] ;
wire [399:399] mux_1level_tapbuf_size2_359_configbus0_b;
assign mux_1level_tapbuf_size2_359_configbus0_b[399:399] = sram_blwl_blb[399:399] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_359_ (mux_1level_tapbuf_size2_359_inbus, chanx_1__1__out_19_ , mux_1level_tapbuf_size2_359_sram_blwl_out[399:399] ,
mux_1level_tapbuf_size2_359_sram_blwl_outb[399:399] );
//----- SRAM bits for MUX[359], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_399_ (mux_1level_tapbuf_size2_359_sram_blwl_out[399:399] ,mux_1level_tapbuf_size2_359_sram_blwl_out[399:399] ,mux_1level_tapbuf_size2_359_sram_blwl_outb[399:399] ,mux_1level_tapbuf_size2_359_configbus0[399:399], mux_1level_tapbuf_size2_359_configbus1[399:399] , mux_1level_tapbuf_size2_359_configbus0_b[399:399] );
wire [0:1] mux_1level_tapbuf_size2_360_inbus;
assign mux_1level_tapbuf_size2_360_inbus[0] =  grid_1__1__pin_0__0__48_;
assign mux_1level_tapbuf_size2_360_inbus[1] = chany_1__1__in_18_ ;
wire [400:400] mux_1level_tapbuf_size2_360_configbus0;
wire [400:400] mux_1level_tapbuf_size2_360_configbus1;
wire [400:400] mux_1level_tapbuf_size2_360_sram_blwl_out ;
wire [400:400] mux_1level_tapbuf_size2_360_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_360_configbus0[400:400] = sram_blwl_bl[400:400] ;
assign mux_1level_tapbuf_size2_360_configbus1[400:400] = sram_blwl_wl[400:400] ;
wire [400:400] mux_1level_tapbuf_size2_360_configbus0_b;
assign mux_1level_tapbuf_size2_360_configbus0_b[400:400] = sram_blwl_blb[400:400] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_360_ (mux_1level_tapbuf_size2_360_inbus, chanx_1__1__out_21_ , mux_1level_tapbuf_size2_360_sram_blwl_out[400:400] ,
mux_1level_tapbuf_size2_360_sram_blwl_outb[400:400] );
//----- SRAM bits for MUX[360], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_400_ (mux_1level_tapbuf_size2_360_sram_blwl_out[400:400] ,mux_1level_tapbuf_size2_360_sram_blwl_out[400:400] ,mux_1level_tapbuf_size2_360_sram_blwl_outb[400:400] ,mux_1level_tapbuf_size2_360_configbus0[400:400], mux_1level_tapbuf_size2_360_configbus1[400:400] , mux_1level_tapbuf_size2_360_configbus0_b[400:400] );
wire [0:1] mux_1level_tapbuf_size2_361_inbus;
assign mux_1level_tapbuf_size2_361_inbus[0] =  grid_1__1__pin_0__0__48_;
assign mux_1level_tapbuf_size2_361_inbus[1] = chany_1__1__in_20_ ;
wire [401:401] mux_1level_tapbuf_size2_361_configbus0;
wire [401:401] mux_1level_tapbuf_size2_361_configbus1;
wire [401:401] mux_1level_tapbuf_size2_361_sram_blwl_out ;
wire [401:401] mux_1level_tapbuf_size2_361_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_361_configbus0[401:401] = sram_blwl_bl[401:401] ;
assign mux_1level_tapbuf_size2_361_configbus1[401:401] = sram_blwl_wl[401:401] ;
wire [401:401] mux_1level_tapbuf_size2_361_configbus0_b;
assign mux_1level_tapbuf_size2_361_configbus0_b[401:401] = sram_blwl_blb[401:401] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_361_ (mux_1level_tapbuf_size2_361_inbus, chanx_1__1__out_23_ , mux_1level_tapbuf_size2_361_sram_blwl_out[401:401] ,
mux_1level_tapbuf_size2_361_sram_blwl_outb[401:401] );
//----- SRAM bits for MUX[361], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_401_ (mux_1level_tapbuf_size2_361_sram_blwl_out[401:401] ,mux_1level_tapbuf_size2_361_sram_blwl_out[401:401] ,mux_1level_tapbuf_size2_361_sram_blwl_outb[401:401] ,mux_1level_tapbuf_size2_361_configbus0[401:401], mux_1level_tapbuf_size2_361_configbus1[401:401] , mux_1level_tapbuf_size2_361_configbus0_b[401:401] );
wire [0:1] mux_1level_tapbuf_size2_362_inbus;
assign mux_1level_tapbuf_size2_362_inbus[0] =  grid_1__1__pin_0__0__48_;
assign mux_1level_tapbuf_size2_362_inbus[1] = chany_1__1__in_22_ ;
wire [402:402] mux_1level_tapbuf_size2_362_configbus0;
wire [402:402] mux_1level_tapbuf_size2_362_configbus1;
wire [402:402] mux_1level_tapbuf_size2_362_sram_blwl_out ;
wire [402:402] mux_1level_tapbuf_size2_362_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_362_configbus0[402:402] = sram_blwl_bl[402:402] ;
assign mux_1level_tapbuf_size2_362_configbus1[402:402] = sram_blwl_wl[402:402] ;
wire [402:402] mux_1level_tapbuf_size2_362_configbus0_b;
assign mux_1level_tapbuf_size2_362_configbus0_b[402:402] = sram_blwl_blb[402:402] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_362_ (mux_1level_tapbuf_size2_362_inbus, chanx_1__1__out_25_ , mux_1level_tapbuf_size2_362_sram_blwl_out[402:402] ,
mux_1level_tapbuf_size2_362_sram_blwl_outb[402:402] );
//----- SRAM bits for MUX[362], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_402_ (mux_1level_tapbuf_size2_362_sram_blwl_out[402:402] ,mux_1level_tapbuf_size2_362_sram_blwl_out[402:402] ,mux_1level_tapbuf_size2_362_sram_blwl_outb[402:402] ,mux_1level_tapbuf_size2_362_configbus0[402:402], mux_1level_tapbuf_size2_362_configbus1[402:402] , mux_1level_tapbuf_size2_362_configbus0_b[402:402] );
wire [0:1] mux_1level_tapbuf_size2_363_inbus;
assign mux_1level_tapbuf_size2_363_inbus[0] =  grid_1__1__pin_0__0__48_;
assign mux_1level_tapbuf_size2_363_inbus[1] = chany_1__1__in_24_ ;
wire [403:403] mux_1level_tapbuf_size2_363_configbus0;
wire [403:403] mux_1level_tapbuf_size2_363_configbus1;
wire [403:403] mux_1level_tapbuf_size2_363_sram_blwl_out ;
wire [403:403] mux_1level_tapbuf_size2_363_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_363_configbus0[403:403] = sram_blwl_bl[403:403] ;
assign mux_1level_tapbuf_size2_363_configbus1[403:403] = sram_blwl_wl[403:403] ;
wire [403:403] mux_1level_tapbuf_size2_363_configbus0_b;
assign mux_1level_tapbuf_size2_363_configbus0_b[403:403] = sram_blwl_blb[403:403] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_363_ (mux_1level_tapbuf_size2_363_inbus, chanx_1__1__out_27_ , mux_1level_tapbuf_size2_363_sram_blwl_out[403:403] ,
mux_1level_tapbuf_size2_363_sram_blwl_outb[403:403] );
//----- SRAM bits for MUX[363], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_403_ (mux_1level_tapbuf_size2_363_sram_blwl_out[403:403] ,mux_1level_tapbuf_size2_363_sram_blwl_out[403:403] ,mux_1level_tapbuf_size2_363_sram_blwl_outb[403:403] ,mux_1level_tapbuf_size2_363_configbus0[403:403], mux_1level_tapbuf_size2_363_configbus1[403:403] , mux_1level_tapbuf_size2_363_configbus0_b[403:403] );
wire [0:1] mux_1level_tapbuf_size2_364_inbus;
assign mux_1level_tapbuf_size2_364_inbus[0] =  grid_1__1__pin_0__0__48_;
assign mux_1level_tapbuf_size2_364_inbus[1] = chany_1__1__in_26_ ;
wire [404:404] mux_1level_tapbuf_size2_364_configbus0;
wire [404:404] mux_1level_tapbuf_size2_364_configbus1;
wire [404:404] mux_1level_tapbuf_size2_364_sram_blwl_out ;
wire [404:404] mux_1level_tapbuf_size2_364_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_364_configbus0[404:404] = sram_blwl_bl[404:404] ;
assign mux_1level_tapbuf_size2_364_configbus1[404:404] = sram_blwl_wl[404:404] ;
wire [404:404] mux_1level_tapbuf_size2_364_configbus0_b;
assign mux_1level_tapbuf_size2_364_configbus0_b[404:404] = sram_blwl_blb[404:404] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_364_ (mux_1level_tapbuf_size2_364_inbus, chanx_1__1__out_29_ , mux_1level_tapbuf_size2_364_sram_blwl_out[404:404] ,
mux_1level_tapbuf_size2_364_sram_blwl_outb[404:404] );
//----- SRAM bits for MUX[364], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_404_ (mux_1level_tapbuf_size2_364_sram_blwl_out[404:404] ,mux_1level_tapbuf_size2_364_sram_blwl_out[404:404] ,mux_1level_tapbuf_size2_364_sram_blwl_outb[404:404] ,mux_1level_tapbuf_size2_364_configbus0[404:404], mux_1level_tapbuf_size2_364_configbus1[404:404] , mux_1level_tapbuf_size2_364_configbus0_b[404:404] );
wire [0:1] mux_1level_tapbuf_size2_365_inbus;
assign mux_1level_tapbuf_size2_365_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_365_inbus[1] = chany_1__1__in_28_ ;
wire [405:405] mux_1level_tapbuf_size2_365_configbus0;
wire [405:405] mux_1level_tapbuf_size2_365_configbus1;
wire [405:405] mux_1level_tapbuf_size2_365_sram_blwl_out ;
wire [405:405] mux_1level_tapbuf_size2_365_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_365_configbus0[405:405] = sram_blwl_bl[405:405] ;
assign mux_1level_tapbuf_size2_365_configbus1[405:405] = sram_blwl_wl[405:405] ;
wire [405:405] mux_1level_tapbuf_size2_365_configbus0_b;
assign mux_1level_tapbuf_size2_365_configbus0_b[405:405] = sram_blwl_blb[405:405] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_365_ (mux_1level_tapbuf_size2_365_inbus, chanx_1__1__out_31_ , mux_1level_tapbuf_size2_365_sram_blwl_out[405:405] ,
mux_1level_tapbuf_size2_365_sram_blwl_outb[405:405] );
//----- SRAM bits for MUX[365], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_405_ (mux_1level_tapbuf_size2_365_sram_blwl_out[405:405] ,mux_1level_tapbuf_size2_365_sram_blwl_out[405:405] ,mux_1level_tapbuf_size2_365_sram_blwl_outb[405:405] ,mux_1level_tapbuf_size2_365_configbus0[405:405], mux_1level_tapbuf_size2_365_configbus1[405:405] , mux_1level_tapbuf_size2_365_configbus0_b[405:405] );
wire [0:1] mux_1level_tapbuf_size2_366_inbus;
assign mux_1level_tapbuf_size2_366_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_366_inbus[1] = chany_1__1__in_30_ ;
wire [406:406] mux_1level_tapbuf_size2_366_configbus0;
wire [406:406] mux_1level_tapbuf_size2_366_configbus1;
wire [406:406] mux_1level_tapbuf_size2_366_sram_blwl_out ;
wire [406:406] mux_1level_tapbuf_size2_366_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_366_configbus0[406:406] = sram_blwl_bl[406:406] ;
assign mux_1level_tapbuf_size2_366_configbus1[406:406] = sram_blwl_wl[406:406] ;
wire [406:406] mux_1level_tapbuf_size2_366_configbus0_b;
assign mux_1level_tapbuf_size2_366_configbus0_b[406:406] = sram_blwl_blb[406:406] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_366_ (mux_1level_tapbuf_size2_366_inbus, chanx_1__1__out_33_ , mux_1level_tapbuf_size2_366_sram_blwl_out[406:406] ,
mux_1level_tapbuf_size2_366_sram_blwl_outb[406:406] );
//----- SRAM bits for MUX[366], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_406_ (mux_1level_tapbuf_size2_366_sram_blwl_out[406:406] ,mux_1level_tapbuf_size2_366_sram_blwl_out[406:406] ,mux_1level_tapbuf_size2_366_sram_blwl_outb[406:406] ,mux_1level_tapbuf_size2_366_configbus0[406:406], mux_1level_tapbuf_size2_366_configbus1[406:406] , mux_1level_tapbuf_size2_366_configbus0_b[406:406] );
wire [0:1] mux_1level_tapbuf_size2_367_inbus;
assign mux_1level_tapbuf_size2_367_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_367_inbus[1] = chany_1__1__in_32_ ;
wire [407:407] mux_1level_tapbuf_size2_367_configbus0;
wire [407:407] mux_1level_tapbuf_size2_367_configbus1;
wire [407:407] mux_1level_tapbuf_size2_367_sram_blwl_out ;
wire [407:407] mux_1level_tapbuf_size2_367_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_367_configbus0[407:407] = sram_blwl_bl[407:407] ;
assign mux_1level_tapbuf_size2_367_configbus1[407:407] = sram_blwl_wl[407:407] ;
wire [407:407] mux_1level_tapbuf_size2_367_configbus0_b;
assign mux_1level_tapbuf_size2_367_configbus0_b[407:407] = sram_blwl_blb[407:407] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_367_ (mux_1level_tapbuf_size2_367_inbus, chanx_1__1__out_35_ , mux_1level_tapbuf_size2_367_sram_blwl_out[407:407] ,
mux_1level_tapbuf_size2_367_sram_blwl_outb[407:407] );
//----- SRAM bits for MUX[367], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_407_ (mux_1level_tapbuf_size2_367_sram_blwl_out[407:407] ,mux_1level_tapbuf_size2_367_sram_blwl_out[407:407] ,mux_1level_tapbuf_size2_367_sram_blwl_outb[407:407] ,mux_1level_tapbuf_size2_367_configbus0[407:407], mux_1level_tapbuf_size2_367_configbus1[407:407] , mux_1level_tapbuf_size2_367_configbus0_b[407:407] );
wire [0:1] mux_1level_tapbuf_size2_368_inbus;
assign mux_1level_tapbuf_size2_368_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_368_inbus[1] = chany_1__1__in_34_ ;
wire [408:408] mux_1level_tapbuf_size2_368_configbus0;
wire [408:408] mux_1level_tapbuf_size2_368_configbus1;
wire [408:408] mux_1level_tapbuf_size2_368_sram_blwl_out ;
wire [408:408] mux_1level_tapbuf_size2_368_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_368_configbus0[408:408] = sram_blwl_bl[408:408] ;
assign mux_1level_tapbuf_size2_368_configbus1[408:408] = sram_blwl_wl[408:408] ;
wire [408:408] mux_1level_tapbuf_size2_368_configbus0_b;
assign mux_1level_tapbuf_size2_368_configbus0_b[408:408] = sram_blwl_blb[408:408] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_368_ (mux_1level_tapbuf_size2_368_inbus, chanx_1__1__out_37_ , mux_1level_tapbuf_size2_368_sram_blwl_out[408:408] ,
mux_1level_tapbuf_size2_368_sram_blwl_outb[408:408] );
//----- SRAM bits for MUX[368], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_408_ (mux_1level_tapbuf_size2_368_sram_blwl_out[408:408] ,mux_1level_tapbuf_size2_368_sram_blwl_out[408:408] ,mux_1level_tapbuf_size2_368_sram_blwl_outb[408:408] ,mux_1level_tapbuf_size2_368_configbus0[408:408], mux_1level_tapbuf_size2_368_configbus1[408:408] , mux_1level_tapbuf_size2_368_configbus0_b[408:408] );
wire [0:1] mux_1level_tapbuf_size2_369_inbus;
assign mux_1level_tapbuf_size2_369_inbus[0] =  grid_1__2__pin_0__2__1_;
assign mux_1level_tapbuf_size2_369_inbus[1] = chany_1__1__in_36_ ;
wire [409:409] mux_1level_tapbuf_size2_369_configbus0;
wire [409:409] mux_1level_tapbuf_size2_369_configbus1;
wire [409:409] mux_1level_tapbuf_size2_369_sram_blwl_out ;
wire [409:409] mux_1level_tapbuf_size2_369_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_369_configbus0[409:409] = sram_blwl_bl[409:409] ;
assign mux_1level_tapbuf_size2_369_configbus1[409:409] = sram_blwl_wl[409:409] ;
wire [409:409] mux_1level_tapbuf_size2_369_configbus0_b;
assign mux_1level_tapbuf_size2_369_configbus0_b[409:409] = sram_blwl_blb[409:409] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_369_ (mux_1level_tapbuf_size2_369_inbus, chanx_1__1__out_39_ , mux_1level_tapbuf_size2_369_sram_blwl_out[409:409] ,
mux_1level_tapbuf_size2_369_sram_blwl_outb[409:409] );
//----- SRAM bits for MUX[369], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_409_ (mux_1level_tapbuf_size2_369_sram_blwl_out[409:409] ,mux_1level_tapbuf_size2_369_sram_blwl_out[409:409] ,mux_1level_tapbuf_size2_369_sram_blwl_outb[409:409] ,mux_1level_tapbuf_size2_369_configbus0[409:409], mux_1level_tapbuf_size2_369_configbus1[409:409] , mux_1level_tapbuf_size2_369_configbus0_b[409:409] );
wire [0:1] mux_1level_tapbuf_size2_370_inbus;
assign mux_1level_tapbuf_size2_370_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_370_inbus[1] = chany_1__1__in_38_ ;
wire [410:410] mux_1level_tapbuf_size2_370_configbus0;
wire [410:410] mux_1level_tapbuf_size2_370_configbus1;
wire [410:410] mux_1level_tapbuf_size2_370_sram_blwl_out ;
wire [410:410] mux_1level_tapbuf_size2_370_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_370_configbus0[410:410] = sram_blwl_bl[410:410] ;
assign mux_1level_tapbuf_size2_370_configbus1[410:410] = sram_blwl_wl[410:410] ;
wire [410:410] mux_1level_tapbuf_size2_370_configbus0_b;
assign mux_1level_tapbuf_size2_370_configbus0_b[410:410] = sram_blwl_blb[410:410] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_370_ (mux_1level_tapbuf_size2_370_inbus, chanx_1__1__out_41_ , mux_1level_tapbuf_size2_370_sram_blwl_out[410:410] ,
mux_1level_tapbuf_size2_370_sram_blwl_outb[410:410] );
//----- SRAM bits for MUX[370], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_410_ (mux_1level_tapbuf_size2_370_sram_blwl_out[410:410] ,mux_1level_tapbuf_size2_370_sram_blwl_out[410:410] ,mux_1level_tapbuf_size2_370_sram_blwl_outb[410:410] ,mux_1level_tapbuf_size2_370_configbus0[410:410], mux_1level_tapbuf_size2_370_configbus1[410:410] , mux_1level_tapbuf_size2_370_configbus0_b[410:410] );
wire [0:1] mux_1level_tapbuf_size2_371_inbus;
assign mux_1level_tapbuf_size2_371_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_371_inbus[1] = chany_1__1__in_40_ ;
wire [411:411] mux_1level_tapbuf_size2_371_configbus0;
wire [411:411] mux_1level_tapbuf_size2_371_configbus1;
wire [411:411] mux_1level_tapbuf_size2_371_sram_blwl_out ;
wire [411:411] mux_1level_tapbuf_size2_371_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_371_configbus0[411:411] = sram_blwl_bl[411:411] ;
assign mux_1level_tapbuf_size2_371_configbus1[411:411] = sram_blwl_wl[411:411] ;
wire [411:411] mux_1level_tapbuf_size2_371_configbus0_b;
assign mux_1level_tapbuf_size2_371_configbus0_b[411:411] = sram_blwl_blb[411:411] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_371_ (mux_1level_tapbuf_size2_371_inbus, chanx_1__1__out_43_ , mux_1level_tapbuf_size2_371_sram_blwl_out[411:411] ,
mux_1level_tapbuf_size2_371_sram_blwl_outb[411:411] );
//----- SRAM bits for MUX[371], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_411_ (mux_1level_tapbuf_size2_371_sram_blwl_out[411:411] ,mux_1level_tapbuf_size2_371_sram_blwl_out[411:411] ,mux_1level_tapbuf_size2_371_sram_blwl_outb[411:411] ,mux_1level_tapbuf_size2_371_configbus0[411:411], mux_1level_tapbuf_size2_371_configbus1[411:411] , mux_1level_tapbuf_size2_371_configbus0_b[411:411] );
wire [0:1] mux_1level_tapbuf_size2_372_inbus;
assign mux_1level_tapbuf_size2_372_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_372_inbus[1] = chany_1__1__in_42_ ;
wire [412:412] mux_1level_tapbuf_size2_372_configbus0;
wire [412:412] mux_1level_tapbuf_size2_372_configbus1;
wire [412:412] mux_1level_tapbuf_size2_372_sram_blwl_out ;
wire [412:412] mux_1level_tapbuf_size2_372_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_372_configbus0[412:412] = sram_blwl_bl[412:412] ;
assign mux_1level_tapbuf_size2_372_configbus1[412:412] = sram_blwl_wl[412:412] ;
wire [412:412] mux_1level_tapbuf_size2_372_configbus0_b;
assign mux_1level_tapbuf_size2_372_configbus0_b[412:412] = sram_blwl_blb[412:412] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_372_ (mux_1level_tapbuf_size2_372_inbus, chanx_1__1__out_45_ , mux_1level_tapbuf_size2_372_sram_blwl_out[412:412] ,
mux_1level_tapbuf_size2_372_sram_blwl_outb[412:412] );
//----- SRAM bits for MUX[372], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_412_ (mux_1level_tapbuf_size2_372_sram_blwl_out[412:412] ,mux_1level_tapbuf_size2_372_sram_blwl_out[412:412] ,mux_1level_tapbuf_size2_372_sram_blwl_outb[412:412] ,mux_1level_tapbuf_size2_372_configbus0[412:412], mux_1level_tapbuf_size2_372_configbus1[412:412] , mux_1level_tapbuf_size2_372_configbus0_b[412:412] );
wire [0:1] mux_1level_tapbuf_size2_373_inbus;
assign mux_1level_tapbuf_size2_373_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_373_inbus[1] = chany_1__1__in_44_ ;
wire [413:413] mux_1level_tapbuf_size2_373_configbus0;
wire [413:413] mux_1level_tapbuf_size2_373_configbus1;
wire [413:413] mux_1level_tapbuf_size2_373_sram_blwl_out ;
wire [413:413] mux_1level_tapbuf_size2_373_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_373_configbus0[413:413] = sram_blwl_bl[413:413] ;
assign mux_1level_tapbuf_size2_373_configbus1[413:413] = sram_blwl_wl[413:413] ;
wire [413:413] mux_1level_tapbuf_size2_373_configbus0_b;
assign mux_1level_tapbuf_size2_373_configbus0_b[413:413] = sram_blwl_blb[413:413] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_373_ (mux_1level_tapbuf_size2_373_inbus, chanx_1__1__out_47_ , mux_1level_tapbuf_size2_373_sram_blwl_out[413:413] ,
mux_1level_tapbuf_size2_373_sram_blwl_outb[413:413] );
//----- SRAM bits for MUX[373], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_413_ (mux_1level_tapbuf_size2_373_sram_blwl_out[413:413] ,mux_1level_tapbuf_size2_373_sram_blwl_out[413:413] ,mux_1level_tapbuf_size2_373_sram_blwl_outb[413:413] ,mux_1level_tapbuf_size2_373_configbus0[413:413], mux_1level_tapbuf_size2_373_configbus1[413:413] , mux_1level_tapbuf_size2_373_configbus0_b[413:413] );
wire [0:1] mux_1level_tapbuf_size2_374_inbus;
assign mux_1level_tapbuf_size2_374_inbus[0] =  grid_1__2__pin_0__2__3_;
assign mux_1level_tapbuf_size2_374_inbus[1] = chany_1__1__in_46_ ;
wire [414:414] mux_1level_tapbuf_size2_374_configbus0;
wire [414:414] mux_1level_tapbuf_size2_374_configbus1;
wire [414:414] mux_1level_tapbuf_size2_374_sram_blwl_out ;
wire [414:414] mux_1level_tapbuf_size2_374_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_374_configbus0[414:414] = sram_blwl_bl[414:414] ;
assign mux_1level_tapbuf_size2_374_configbus1[414:414] = sram_blwl_wl[414:414] ;
wire [414:414] mux_1level_tapbuf_size2_374_configbus0_b;
assign mux_1level_tapbuf_size2_374_configbus0_b[414:414] = sram_blwl_blb[414:414] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_374_ (mux_1level_tapbuf_size2_374_inbus, chanx_1__1__out_49_ , mux_1level_tapbuf_size2_374_sram_blwl_out[414:414] ,
mux_1level_tapbuf_size2_374_sram_blwl_outb[414:414] );
//----- SRAM bits for MUX[374], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_414_ (mux_1level_tapbuf_size2_374_sram_blwl_out[414:414] ,mux_1level_tapbuf_size2_374_sram_blwl_out[414:414] ,mux_1level_tapbuf_size2_374_sram_blwl_outb[414:414] ,mux_1level_tapbuf_size2_374_configbus0[414:414], mux_1level_tapbuf_size2_374_configbus1[414:414] , mux_1level_tapbuf_size2_374_configbus0_b[414:414] );
wire [0:1] mux_1level_tapbuf_size2_375_inbus;
assign mux_1level_tapbuf_size2_375_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_375_inbus[1] = chany_1__1__in_48_ ;
wire [415:415] mux_1level_tapbuf_size2_375_configbus0;
wire [415:415] mux_1level_tapbuf_size2_375_configbus1;
wire [415:415] mux_1level_tapbuf_size2_375_sram_blwl_out ;
wire [415:415] mux_1level_tapbuf_size2_375_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_375_configbus0[415:415] = sram_blwl_bl[415:415] ;
assign mux_1level_tapbuf_size2_375_configbus1[415:415] = sram_blwl_wl[415:415] ;
wire [415:415] mux_1level_tapbuf_size2_375_configbus0_b;
assign mux_1level_tapbuf_size2_375_configbus0_b[415:415] = sram_blwl_blb[415:415] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_375_ (mux_1level_tapbuf_size2_375_inbus, chanx_1__1__out_51_ , mux_1level_tapbuf_size2_375_sram_blwl_out[415:415] ,
mux_1level_tapbuf_size2_375_sram_blwl_outb[415:415] );
//----- SRAM bits for MUX[375], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_415_ (mux_1level_tapbuf_size2_375_sram_blwl_out[415:415] ,mux_1level_tapbuf_size2_375_sram_blwl_out[415:415] ,mux_1level_tapbuf_size2_375_sram_blwl_outb[415:415] ,mux_1level_tapbuf_size2_375_configbus0[415:415], mux_1level_tapbuf_size2_375_configbus1[415:415] , mux_1level_tapbuf_size2_375_configbus0_b[415:415] );
wire [0:1] mux_1level_tapbuf_size2_376_inbus;
assign mux_1level_tapbuf_size2_376_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_376_inbus[1] = chany_1__1__in_50_ ;
wire [416:416] mux_1level_tapbuf_size2_376_configbus0;
wire [416:416] mux_1level_tapbuf_size2_376_configbus1;
wire [416:416] mux_1level_tapbuf_size2_376_sram_blwl_out ;
wire [416:416] mux_1level_tapbuf_size2_376_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_376_configbus0[416:416] = sram_blwl_bl[416:416] ;
assign mux_1level_tapbuf_size2_376_configbus1[416:416] = sram_blwl_wl[416:416] ;
wire [416:416] mux_1level_tapbuf_size2_376_configbus0_b;
assign mux_1level_tapbuf_size2_376_configbus0_b[416:416] = sram_blwl_blb[416:416] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_376_ (mux_1level_tapbuf_size2_376_inbus, chanx_1__1__out_53_ , mux_1level_tapbuf_size2_376_sram_blwl_out[416:416] ,
mux_1level_tapbuf_size2_376_sram_blwl_outb[416:416] );
//----- SRAM bits for MUX[376], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_416_ (mux_1level_tapbuf_size2_376_sram_blwl_out[416:416] ,mux_1level_tapbuf_size2_376_sram_blwl_out[416:416] ,mux_1level_tapbuf_size2_376_sram_blwl_outb[416:416] ,mux_1level_tapbuf_size2_376_configbus0[416:416], mux_1level_tapbuf_size2_376_configbus1[416:416] , mux_1level_tapbuf_size2_376_configbus0_b[416:416] );
wire [0:1] mux_1level_tapbuf_size2_377_inbus;
assign mux_1level_tapbuf_size2_377_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_377_inbus[1] = chany_1__1__in_52_ ;
wire [417:417] mux_1level_tapbuf_size2_377_configbus0;
wire [417:417] mux_1level_tapbuf_size2_377_configbus1;
wire [417:417] mux_1level_tapbuf_size2_377_sram_blwl_out ;
wire [417:417] mux_1level_tapbuf_size2_377_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_377_configbus0[417:417] = sram_blwl_bl[417:417] ;
assign mux_1level_tapbuf_size2_377_configbus1[417:417] = sram_blwl_wl[417:417] ;
wire [417:417] mux_1level_tapbuf_size2_377_configbus0_b;
assign mux_1level_tapbuf_size2_377_configbus0_b[417:417] = sram_blwl_blb[417:417] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_377_ (mux_1level_tapbuf_size2_377_inbus, chanx_1__1__out_55_ , mux_1level_tapbuf_size2_377_sram_blwl_out[417:417] ,
mux_1level_tapbuf_size2_377_sram_blwl_outb[417:417] );
//----- SRAM bits for MUX[377], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_417_ (mux_1level_tapbuf_size2_377_sram_blwl_out[417:417] ,mux_1level_tapbuf_size2_377_sram_blwl_out[417:417] ,mux_1level_tapbuf_size2_377_sram_blwl_outb[417:417] ,mux_1level_tapbuf_size2_377_configbus0[417:417], mux_1level_tapbuf_size2_377_configbus1[417:417] , mux_1level_tapbuf_size2_377_configbus0_b[417:417] );
wire [0:1] mux_1level_tapbuf_size2_378_inbus;
assign mux_1level_tapbuf_size2_378_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_378_inbus[1] = chany_1__1__in_54_ ;
wire [418:418] mux_1level_tapbuf_size2_378_configbus0;
wire [418:418] mux_1level_tapbuf_size2_378_configbus1;
wire [418:418] mux_1level_tapbuf_size2_378_sram_blwl_out ;
wire [418:418] mux_1level_tapbuf_size2_378_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_378_configbus0[418:418] = sram_blwl_bl[418:418] ;
assign mux_1level_tapbuf_size2_378_configbus1[418:418] = sram_blwl_wl[418:418] ;
wire [418:418] mux_1level_tapbuf_size2_378_configbus0_b;
assign mux_1level_tapbuf_size2_378_configbus0_b[418:418] = sram_blwl_blb[418:418] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_378_ (mux_1level_tapbuf_size2_378_inbus, chanx_1__1__out_57_ , mux_1level_tapbuf_size2_378_sram_blwl_out[418:418] ,
mux_1level_tapbuf_size2_378_sram_blwl_outb[418:418] );
//----- SRAM bits for MUX[378], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_418_ (mux_1level_tapbuf_size2_378_sram_blwl_out[418:418] ,mux_1level_tapbuf_size2_378_sram_blwl_out[418:418] ,mux_1level_tapbuf_size2_378_sram_blwl_outb[418:418] ,mux_1level_tapbuf_size2_378_configbus0[418:418], mux_1level_tapbuf_size2_378_configbus1[418:418] , mux_1level_tapbuf_size2_378_configbus0_b[418:418] );
wire [0:1] mux_1level_tapbuf_size2_379_inbus;
assign mux_1level_tapbuf_size2_379_inbus[0] =  grid_1__2__pin_0__2__5_;
assign mux_1level_tapbuf_size2_379_inbus[1] = chany_1__1__in_56_ ;
wire [419:419] mux_1level_tapbuf_size2_379_configbus0;
wire [419:419] mux_1level_tapbuf_size2_379_configbus1;
wire [419:419] mux_1level_tapbuf_size2_379_sram_blwl_out ;
wire [419:419] mux_1level_tapbuf_size2_379_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_379_configbus0[419:419] = sram_blwl_bl[419:419] ;
assign mux_1level_tapbuf_size2_379_configbus1[419:419] = sram_blwl_wl[419:419] ;
wire [419:419] mux_1level_tapbuf_size2_379_configbus0_b;
assign mux_1level_tapbuf_size2_379_configbus0_b[419:419] = sram_blwl_blb[419:419] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_379_ (mux_1level_tapbuf_size2_379_inbus, chanx_1__1__out_59_ , mux_1level_tapbuf_size2_379_sram_blwl_out[419:419] ,
mux_1level_tapbuf_size2_379_sram_blwl_outb[419:419] );
//----- SRAM bits for MUX[379], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_419_ (mux_1level_tapbuf_size2_379_sram_blwl_out[419:419] ,mux_1level_tapbuf_size2_379_sram_blwl_out[419:419] ,mux_1level_tapbuf_size2_379_sram_blwl_outb[419:419] ,mux_1level_tapbuf_size2_379_configbus0[419:419], mux_1level_tapbuf_size2_379_configbus1[419:419] , mux_1level_tapbuf_size2_379_configbus0_b[419:419] );
wire [0:1] mux_1level_tapbuf_size2_380_inbus;
assign mux_1level_tapbuf_size2_380_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_380_inbus[1] = chany_1__1__in_58_ ;
wire [420:420] mux_1level_tapbuf_size2_380_configbus0;
wire [420:420] mux_1level_tapbuf_size2_380_configbus1;
wire [420:420] mux_1level_tapbuf_size2_380_sram_blwl_out ;
wire [420:420] mux_1level_tapbuf_size2_380_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_380_configbus0[420:420] = sram_blwl_bl[420:420] ;
assign mux_1level_tapbuf_size2_380_configbus1[420:420] = sram_blwl_wl[420:420] ;
wire [420:420] mux_1level_tapbuf_size2_380_configbus0_b;
assign mux_1level_tapbuf_size2_380_configbus0_b[420:420] = sram_blwl_blb[420:420] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_380_ (mux_1level_tapbuf_size2_380_inbus, chanx_1__1__out_61_ , mux_1level_tapbuf_size2_380_sram_blwl_out[420:420] ,
mux_1level_tapbuf_size2_380_sram_blwl_outb[420:420] );
//----- SRAM bits for MUX[380], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_420_ (mux_1level_tapbuf_size2_380_sram_blwl_out[420:420] ,mux_1level_tapbuf_size2_380_sram_blwl_out[420:420] ,mux_1level_tapbuf_size2_380_sram_blwl_outb[420:420] ,mux_1level_tapbuf_size2_380_configbus0[420:420], mux_1level_tapbuf_size2_380_configbus1[420:420] , mux_1level_tapbuf_size2_380_configbus0_b[420:420] );
wire [0:1] mux_1level_tapbuf_size2_381_inbus;
assign mux_1level_tapbuf_size2_381_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_381_inbus[1] = chany_1__1__in_60_ ;
wire [421:421] mux_1level_tapbuf_size2_381_configbus0;
wire [421:421] mux_1level_tapbuf_size2_381_configbus1;
wire [421:421] mux_1level_tapbuf_size2_381_sram_blwl_out ;
wire [421:421] mux_1level_tapbuf_size2_381_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_381_configbus0[421:421] = sram_blwl_bl[421:421] ;
assign mux_1level_tapbuf_size2_381_configbus1[421:421] = sram_blwl_wl[421:421] ;
wire [421:421] mux_1level_tapbuf_size2_381_configbus0_b;
assign mux_1level_tapbuf_size2_381_configbus0_b[421:421] = sram_blwl_blb[421:421] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_381_ (mux_1level_tapbuf_size2_381_inbus, chanx_1__1__out_63_ , mux_1level_tapbuf_size2_381_sram_blwl_out[421:421] ,
mux_1level_tapbuf_size2_381_sram_blwl_outb[421:421] );
//----- SRAM bits for MUX[381], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_421_ (mux_1level_tapbuf_size2_381_sram_blwl_out[421:421] ,mux_1level_tapbuf_size2_381_sram_blwl_out[421:421] ,mux_1level_tapbuf_size2_381_sram_blwl_outb[421:421] ,mux_1level_tapbuf_size2_381_configbus0[421:421], mux_1level_tapbuf_size2_381_configbus1[421:421] , mux_1level_tapbuf_size2_381_configbus0_b[421:421] );
wire [0:1] mux_1level_tapbuf_size2_382_inbus;
assign mux_1level_tapbuf_size2_382_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_382_inbus[1] = chany_1__1__in_62_ ;
wire [422:422] mux_1level_tapbuf_size2_382_configbus0;
wire [422:422] mux_1level_tapbuf_size2_382_configbus1;
wire [422:422] mux_1level_tapbuf_size2_382_sram_blwl_out ;
wire [422:422] mux_1level_tapbuf_size2_382_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_382_configbus0[422:422] = sram_blwl_bl[422:422] ;
assign mux_1level_tapbuf_size2_382_configbus1[422:422] = sram_blwl_wl[422:422] ;
wire [422:422] mux_1level_tapbuf_size2_382_configbus0_b;
assign mux_1level_tapbuf_size2_382_configbus0_b[422:422] = sram_blwl_blb[422:422] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_382_ (mux_1level_tapbuf_size2_382_inbus, chanx_1__1__out_65_ , mux_1level_tapbuf_size2_382_sram_blwl_out[422:422] ,
mux_1level_tapbuf_size2_382_sram_blwl_outb[422:422] );
//----- SRAM bits for MUX[382], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_422_ (mux_1level_tapbuf_size2_382_sram_blwl_out[422:422] ,mux_1level_tapbuf_size2_382_sram_blwl_out[422:422] ,mux_1level_tapbuf_size2_382_sram_blwl_outb[422:422] ,mux_1level_tapbuf_size2_382_configbus0[422:422], mux_1level_tapbuf_size2_382_configbus1[422:422] , mux_1level_tapbuf_size2_382_configbus0_b[422:422] );
wire [0:1] mux_1level_tapbuf_size2_383_inbus;
assign mux_1level_tapbuf_size2_383_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_383_inbus[1] = chany_1__1__in_64_ ;
wire [423:423] mux_1level_tapbuf_size2_383_configbus0;
wire [423:423] mux_1level_tapbuf_size2_383_configbus1;
wire [423:423] mux_1level_tapbuf_size2_383_sram_blwl_out ;
wire [423:423] mux_1level_tapbuf_size2_383_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_383_configbus0[423:423] = sram_blwl_bl[423:423] ;
assign mux_1level_tapbuf_size2_383_configbus1[423:423] = sram_blwl_wl[423:423] ;
wire [423:423] mux_1level_tapbuf_size2_383_configbus0_b;
assign mux_1level_tapbuf_size2_383_configbus0_b[423:423] = sram_blwl_blb[423:423] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_383_ (mux_1level_tapbuf_size2_383_inbus, chanx_1__1__out_67_ , mux_1level_tapbuf_size2_383_sram_blwl_out[423:423] ,
mux_1level_tapbuf_size2_383_sram_blwl_outb[423:423] );
//----- SRAM bits for MUX[383], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_423_ (mux_1level_tapbuf_size2_383_sram_blwl_out[423:423] ,mux_1level_tapbuf_size2_383_sram_blwl_out[423:423] ,mux_1level_tapbuf_size2_383_sram_blwl_outb[423:423] ,mux_1level_tapbuf_size2_383_configbus0[423:423], mux_1level_tapbuf_size2_383_configbus1[423:423] , mux_1level_tapbuf_size2_383_configbus0_b[423:423] );
wire [0:1] mux_1level_tapbuf_size2_384_inbus;
assign mux_1level_tapbuf_size2_384_inbus[0] =  grid_1__2__pin_0__2__7_;
assign mux_1level_tapbuf_size2_384_inbus[1] = chany_1__1__in_66_ ;
wire [424:424] mux_1level_tapbuf_size2_384_configbus0;
wire [424:424] mux_1level_tapbuf_size2_384_configbus1;
wire [424:424] mux_1level_tapbuf_size2_384_sram_blwl_out ;
wire [424:424] mux_1level_tapbuf_size2_384_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_384_configbus0[424:424] = sram_blwl_bl[424:424] ;
assign mux_1level_tapbuf_size2_384_configbus1[424:424] = sram_blwl_wl[424:424] ;
wire [424:424] mux_1level_tapbuf_size2_384_configbus0_b;
assign mux_1level_tapbuf_size2_384_configbus0_b[424:424] = sram_blwl_blb[424:424] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_384_ (mux_1level_tapbuf_size2_384_inbus, chanx_1__1__out_69_ , mux_1level_tapbuf_size2_384_sram_blwl_out[424:424] ,
mux_1level_tapbuf_size2_384_sram_blwl_outb[424:424] );
//----- SRAM bits for MUX[384], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_424_ (mux_1level_tapbuf_size2_384_sram_blwl_out[424:424] ,mux_1level_tapbuf_size2_384_sram_blwl_out[424:424] ,mux_1level_tapbuf_size2_384_sram_blwl_outb[424:424] ,mux_1level_tapbuf_size2_384_configbus0[424:424], mux_1level_tapbuf_size2_384_configbus1[424:424] , mux_1level_tapbuf_size2_384_configbus0_b[424:424] );
wire [0:1] mux_1level_tapbuf_size2_385_inbus;
assign mux_1level_tapbuf_size2_385_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_385_inbus[1] = chany_1__1__in_68_ ;
wire [425:425] mux_1level_tapbuf_size2_385_configbus0;
wire [425:425] mux_1level_tapbuf_size2_385_configbus1;
wire [425:425] mux_1level_tapbuf_size2_385_sram_blwl_out ;
wire [425:425] mux_1level_tapbuf_size2_385_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_385_configbus0[425:425] = sram_blwl_bl[425:425] ;
assign mux_1level_tapbuf_size2_385_configbus1[425:425] = sram_blwl_wl[425:425] ;
wire [425:425] mux_1level_tapbuf_size2_385_configbus0_b;
assign mux_1level_tapbuf_size2_385_configbus0_b[425:425] = sram_blwl_blb[425:425] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_385_ (mux_1level_tapbuf_size2_385_inbus, chanx_1__1__out_71_ , mux_1level_tapbuf_size2_385_sram_blwl_out[425:425] ,
mux_1level_tapbuf_size2_385_sram_blwl_outb[425:425] );
//----- SRAM bits for MUX[385], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_425_ (mux_1level_tapbuf_size2_385_sram_blwl_out[425:425] ,mux_1level_tapbuf_size2_385_sram_blwl_out[425:425] ,mux_1level_tapbuf_size2_385_sram_blwl_outb[425:425] ,mux_1level_tapbuf_size2_385_configbus0[425:425], mux_1level_tapbuf_size2_385_configbus1[425:425] , mux_1level_tapbuf_size2_385_configbus0_b[425:425] );
wire [0:1] mux_1level_tapbuf_size2_386_inbus;
assign mux_1level_tapbuf_size2_386_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_386_inbus[1] = chany_1__1__in_70_ ;
wire [426:426] mux_1level_tapbuf_size2_386_configbus0;
wire [426:426] mux_1level_tapbuf_size2_386_configbus1;
wire [426:426] mux_1level_tapbuf_size2_386_sram_blwl_out ;
wire [426:426] mux_1level_tapbuf_size2_386_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_386_configbus0[426:426] = sram_blwl_bl[426:426] ;
assign mux_1level_tapbuf_size2_386_configbus1[426:426] = sram_blwl_wl[426:426] ;
wire [426:426] mux_1level_tapbuf_size2_386_configbus0_b;
assign mux_1level_tapbuf_size2_386_configbus0_b[426:426] = sram_blwl_blb[426:426] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_386_ (mux_1level_tapbuf_size2_386_inbus, chanx_1__1__out_73_ , mux_1level_tapbuf_size2_386_sram_blwl_out[426:426] ,
mux_1level_tapbuf_size2_386_sram_blwl_outb[426:426] );
//----- SRAM bits for MUX[386], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_426_ (mux_1level_tapbuf_size2_386_sram_blwl_out[426:426] ,mux_1level_tapbuf_size2_386_sram_blwl_out[426:426] ,mux_1level_tapbuf_size2_386_sram_blwl_outb[426:426] ,mux_1level_tapbuf_size2_386_configbus0[426:426], mux_1level_tapbuf_size2_386_configbus1[426:426] , mux_1level_tapbuf_size2_386_configbus0_b[426:426] );
wire [0:1] mux_1level_tapbuf_size2_387_inbus;
assign mux_1level_tapbuf_size2_387_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_387_inbus[1] = chany_1__1__in_72_ ;
wire [427:427] mux_1level_tapbuf_size2_387_configbus0;
wire [427:427] mux_1level_tapbuf_size2_387_configbus1;
wire [427:427] mux_1level_tapbuf_size2_387_sram_blwl_out ;
wire [427:427] mux_1level_tapbuf_size2_387_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_387_configbus0[427:427] = sram_blwl_bl[427:427] ;
assign mux_1level_tapbuf_size2_387_configbus1[427:427] = sram_blwl_wl[427:427] ;
wire [427:427] mux_1level_tapbuf_size2_387_configbus0_b;
assign mux_1level_tapbuf_size2_387_configbus0_b[427:427] = sram_blwl_blb[427:427] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_387_ (mux_1level_tapbuf_size2_387_inbus, chanx_1__1__out_75_ , mux_1level_tapbuf_size2_387_sram_blwl_out[427:427] ,
mux_1level_tapbuf_size2_387_sram_blwl_outb[427:427] );
//----- SRAM bits for MUX[387], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_427_ (mux_1level_tapbuf_size2_387_sram_blwl_out[427:427] ,mux_1level_tapbuf_size2_387_sram_blwl_out[427:427] ,mux_1level_tapbuf_size2_387_sram_blwl_outb[427:427] ,mux_1level_tapbuf_size2_387_configbus0[427:427], mux_1level_tapbuf_size2_387_configbus1[427:427] , mux_1level_tapbuf_size2_387_configbus0_b[427:427] );
wire [0:1] mux_1level_tapbuf_size2_388_inbus;
assign mux_1level_tapbuf_size2_388_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_388_inbus[1] = chany_1__1__in_74_ ;
wire [428:428] mux_1level_tapbuf_size2_388_configbus0;
wire [428:428] mux_1level_tapbuf_size2_388_configbus1;
wire [428:428] mux_1level_tapbuf_size2_388_sram_blwl_out ;
wire [428:428] mux_1level_tapbuf_size2_388_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_388_configbus0[428:428] = sram_blwl_bl[428:428] ;
assign mux_1level_tapbuf_size2_388_configbus1[428:428] = sram_blwl_wl[428:428] ;
wire [428:428] mux_1level_tapbuf_size2_388_configbus0_b;
assign mux_1level_tapbuf_size2_388_configbus0_b[428:428] = sram_blwl_blb[428:428] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_388_ (mux_1level_tapbuf_size2_388_inbus, chanx_1__1__out_77_ , mux_1level_tapbuf_size2_388_sram_blwl_out[428:428] ,
mux_1level_tapbuf_size2_388_sram_blwl_outb[428:428] );
//----- SRAM bits for MUX[388], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_428_ (mux_1level_tapbuf_size2_388_sram_blwl_out[428:428] ,mux_1level_tapbuf_size2_388_sram_blwl_out[428:428] ,mux_1level_tapbuf_size2_388_sram_blwl_outb[428:428] ,mux_1level_tapbuf_size2_388_configbus0[428:428], mux_1level_tapbuf_size2_388_configbus1[428:428] , mux_1level_tapbuf_size2_388_configbus0_b[428:428] );
wire [0:1] mux_1level_tapbuf_size2_389_inbus;
assign mux_1level_tapbuf_size2_389_inbus[0] =  grid_1__2__pin_0__2__9_;
assign mux_1level_tapbuf_size2_389_inbus[1] = chany_1__1__in_76_ ;
wire [429:429] mux_1level_tapbuf_size2_389_configbus0;
wire [429:429] mux_1level_tapbuf_size2_389_configbus1;
wire [429:429] mux_1level_tapbuf_size2_389_sram_blwl_out ;
wire [429:429] mux_1level_tapbuf_size2_389_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_389_configbus0[429:429] = sram_blwl_bl[429:429] ;
assign mux_1level_tapbuf_size2_389_configbus1[429:429] = sram_blwl_wl[429:429] ;
wire [429:429] mux_1level_tapbuf_size2_389_configbus0_b;
assign mux_1level_tapbuf_size2_389_configbus0_b[429:429] = sram_blwl_blb[429:429] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_389_ (mux_1level_tapbuf_size2_389_inbus, chanx_1__1__out_79_ , mux_1level_tapbuf_size2_389_sram_blwl_out[429:429] ,
mux_1level_tapbuf_size2_389_sram_blwl_outb[429:429] );
//----- SRAM bits for MUX[389], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_429_ (mux_1level_tapbuf_size2_389_sram_blwl_out[429:429] ,mux_1level_tapbuf_size2_389_sram_blwl_out[429:429] ,mux_1level_tapbuf_size2_389_sram_blwl_outb[429:429] ,mux_1level_tapbuf_size2_389_configbus0[429:429], mux_1level_tapbuf_size2_389_configbus1[429:429] , mux_1level_tapbuf_size2_389_configbus0_b[429:429] );
wire [0:1] mux_1level_tapbuf_size2_390_inbus;
assign mux_1level_tapbuf_size2_390_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_390_inbus[1] = chany_1__1__in_78_ ;
wire [430:430] mux_1level_tapbuf_size2_390_configbus0;
wire [430:430] mux_1level_tapbuf_size2_390_configbus1;
wire [430:430] mux_1level_tapbuf_size2_390_sram_blwl_out ;
wire [430:430] mux_1level_tapbuf_size2_390_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_390_configbus0[430:430] = sram_blwl_bl[430:430] ;
assign mux_1level_tapbuf_size2_390_configbus1[430:430] = sram_blwl_wl[430:430] ;
wire [430:430] mux_1level_tapbuf_size2_390_configbus0_b;
assign mux_1level_tapbuf_size2_390_configbus0_b[430:430] = sram_blwl_blb[430:430] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_390_ (mux_1level_tapbuf_size2_390_inbus, chanx_1__1__out_81_ , mux_1level_tapbuf_size2_390_sram_blwl_out[430:430] ,
mux_1level_tapbuf_size2_390_sram_blwl_outb[430:430] );
//----- SRAM bits for MUX[390], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_430_ (mux_1level_tapbuf_size2_390_sram_blwl_out[430:430] ,mux_1level_tapbuf_size2_390_sram_blwl_out[430:430] ,mux_1level_tapbuf_size2_390_sram_blwl_outb[430:430] ,mux_1level_tapbuf_size2_390_configbus0[430:430], mux_1level_tapbuf_size2_390_configbus1[430:430] , mux_1level_tapbuf_size2_390_configbus0_b[430:430] );
wire [0:1] mux_1level_tapbuf_size2_391_inbus;
assign mux_1level_tapbuf_size2_391_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_391_inbus[1] = chany_1__1__in_80_ ;
wire [431:431] mux_1level_tapbuf_size2_391_configbus0;
wire [431:431] mux_1level_tapbuf_size2_391_configbus1;
wire [431:431] mux_1level_tapbuf_size2_391_sram_blwl_out ;
wire [431:431] mux_1level_tapbuf_size2_391_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_391_configbus0[431:431] = sram_blwl_bl[431:431] ;
assign mux_1level_tapbuf_size2_391_configbus1[431:431] = sram_blwl_wl[431:431] ;
wire [431:431] mux_1level_tapbuf_size2_391_configbus0_b;
assign mux_1level_tapbuf_size2_391_configbus0_b[431:431] = sram_blwl_blb[431:431] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_391_ (mux_1level_tapbuf_size2_391_inbus, chanx_1__1__out_83_ , mux_1level_tapbuf_size2_391_sram_blwl_out[431:431] ,
mux_1level_tapbuf_size2_391_sram_blwl_outb[431:431] );
//----- SRAM bits for MUX[391], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_431_ (mux_1level_tapbuf_size2_391_sram_blwl_out[431:431] ,mux_1level_tapbuf_size2_391_sram_blwl_out[431:431] ,mux_1level_tapbuf_size2_391_sram_blwl_outb[431:431] ,mux_1level_tapbuf_size2_391_configbus0[431:431], mux_1level_tapbuf_size2_391_configbus1[431:431] , mux_1level_tapbuf_size2_391_configbus0_b[431:431] );
wire [0:1] mux_1level_tapbuf_size2_392_inbus;
assign mux_1level_tapbuf_size2_392_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_392_inbus[1] = chany_1__1__in_82_ ;
wire [432:432] mux_1level_tapbuf_size2_392_configbus0;
wire [432:432] mux_1level_tapbuf_size2_392_configbus1;
wire [432:432] mux_1level_tapbuf_size2_392_sram_blwl_out ;
wire [432:432] mux_1level_tapbuf_size2_392_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_392_configbus0[432:432] = sram_blwl_bl[432:432] ;
assign mux_1level_tapbuf_size2_392_configbus1[432:432] = sram_blwl_wl[432:432] ;
wire [432:432] mux_1level_tapbuf_size2_392_configbus0_b;
assign mux_1level_tapbuf_size2_392_configbus0_b[432:432] = sram_blwl_blb[432:432] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_392_ (mux_1level_tapbuf_size2_392_inbus, chanx_1__1__out_85_ , mux_1level_tapbuf_size2_392_sram_blwl_out[432:432] ,
mux_1level_tapbuf_size2_392_sram_blwl_outb[432:432] );
//----- SRAM bits for MUX[392], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_432_ (mux_1level_tapbuf_size2_392_sram_blwl_out[432:432] ,mux_1level_tapbuf_size2_392_sram_blwl_out[432:432] ,mux_1level_tapbuf_size2_392_sram_blwl_outb[432:432] ,mux_1level_tapbuf_size2_392_configbus0[432:432], mux_1level_tapbuf_size2_392_configbus1[432:432] , mux_1level_tapbuf_size2_392_configbus0_b[432:432] );
wire [0:1] mux_1level_tapbuf_size2_393_inbus;
assign mux_1level_tapbuf_size2_393_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_393_inbus[1] = chany_1__1__in_84_ ;
wire [433:433] mux_1level_tapbuf_size2_393_configbus0;
wire [433:433] mux_1level_tapbuf_size2_393_configbus1;
wire [433:433] mux_1level_tapbuf_size2_393_sram_blwl_out ;
wire [433:433] mux_1level_tapbuf_size2_393_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_393_configbus0[433:433] = sram_blwl_bl[433:433] ;
assign mux_1level_tapbuf_size2_393_configbus1[433:433] = sram_blwl_wl[433:433] ;
wire [433:433] mux_1level_tapbuf_size2_393_configbus0_b;
assign mux_1level_tapbuf_size2_393_configbus0_b[433:433] = sram_blwl_blb[433:433] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_393_ (mux_1level_tapbuf_size2_393_inbus, chanx_1__1__out_87_ , mux_1level_tapbuf_size2_393_sram_blwl_out[433:433] ,
mux_1level_tapbuf_size2_393_sram_blwl_outb[433:433] );
//----- SRAM bits for MUX[393], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_433_ (mux_1level_tapbuf_size2_393_sram_blwl_out[433:433] ,mux_1level_tapbuf_size2_393_sram_blwl_out[433:433] ,mux_1level_tapbuf_size2_393_sram_blwl_outb[433:433] ,mux_1level_tapbuf_size2_393_configbus0[433:433], mux_1level_tapbuf_size2_393_configbus1[433:433] , mux_1level_tapbuf_size2_393_configbus0_b[433:433] );
wire [0:1] mux_1level_tapbuf_size2_394_inbus;
assign mux_1level_tapbuf_size2_394_inbus[0] =  grid_1__2__pin_0__2__11_;
assign mux_1level_tapbuf_size2_394_inbus[1] = chany_1__1__in_86_ ;
wire [434:434] mux_1level_tapbuf_size2_394_configbus0;
wire [434:434] mux_1level_tapbuf_size2_394_configbus1;
wire [434:434] mux_1level_tapbuf_size2_394_sram_blwl_out ;
wire [434:434] mux_1level_tapbuf_size2_394_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_394_configbus0[434:434] = sram_blwl_bl[434:434] ;
assign mux_1level_tapbuf_size2_394_configbus1[434:434] = sram_blwl_wl[434:434] ;
wire [434:434] mux_1level_tapbuf_size2_394_configbus0_b;
assign mux_1level_tapbuf_size2_394_configbus0_b[434:434] = sram_blwl_blb[434:434] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_394_ (mux_1level_tapbuf_size2_394_inbus, chanx_1__1__out_89_ , mux_1level_tapbuf_size2_394_sram_blwl_out[434:434] ,
mux_1level_tapbuf_size2_394_sram_blwl_outb[434:434] );
//----- SRAM bits for MUX[394], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_434_ (mux_1level_tapbuf_size2_394_sram_blwl_out[434:434] ,mux_1level_tapbuf_size2_394_sram_blwl_out[434:434] ,mux_1level_tapbuf_size2_394_sram_blwl_outb[434:434] ,mux_1level_tapbuf_size2_394_configbus0[434:434], mux_1level_tapbuf_size2_394_configbus1[434:434] , mux_1level_tapbuf_size2_394_configbus0_b[434:434] );
wire [0:1] mux_1level_tapbuf_size2_395_inbus;
assign mux_1level_tapbuf_size2_395_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_395_inbus[1] = chany_1__1__in_88_ ;
wire [435:435] mux_1level_tapbuf_size2_395_configbus0;
wire [435:435] mux_1level_tapbuf_size2_395_configbus1;
wire [435:435] mux_1level_tapbuf_size2_395_sram_blwl_out ;
wire [435:435] mux_1level_tapbuf_size2_395_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_395_configbus0[435:435] = sram_blwl_bl[435:435] ;
assign mux_1level_tapbuf_size2_395_configbus1[435:435] = sram_blwl_wl[435:435] ;
wire [435:435] mux_1level_tapbuf_size2_395_configbus0_b;
assign mux_1level_tapbuf_size2_395_configbus0_b[435:435] = sram_blwl_blb[435:435] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_395_ (mux_1level_tapbuf_size2_395_inbus, chanx_1__1__out_91_ , mux_1level_tapbuf_size2_395_sram_blwl_out[435:435] ,
mux_1level_tapbuf_size2_395_sram_blwl_outb[435:435] );
//----- SRAM bits for MUX[395], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_435_ (mux_1level_tapbuf_size2_395_sram_blwl_out[435:435] ,mux_1level_tapbuf_size2_395_sram_blwl_out[435:435] ,mux_1level_tapbuf_size2_395_sram_blwl_outb[435:435] ,mux_1level_tapbuf_size2_395_configbus0[435:435], mux_1level_tapbuf_size2_395_configbus1[435:435] , mux_1level_tapbuf_size2_395_configbus0_b[435:435] );
wire [0:1] mux_1level_tapbuf_size2_396_inbus;
assign mux_1level_tapbuf_size2_396_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_396_inbus[1] = chany_1__1__in_90_ ;
wire [436:436] mux_1level_tapbuf_size2_396_configbus0;
wire [436:436] mux_1level_tapbuf_size2_396_configbus1;
wire [436:436] mux_1level_tapbuf_size2_396_sram_blwl_out ;
wire [436:436] mux_1level_tapbuf_size2_396_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_396_configbus0[436:436] = sram_blwl_bl[436:436] ;
assign mux_1level_tapbuf_size2_396_configbus1[436:436] = sram_blwl_wl[436:436] ;
wire [436:436] mux_1level_tapbuf_size2_396_configbus0_b;
assign mux_1level_tapbuf_size2_396_configbus0_b[436:436] = sram_blwl_blb[436:436] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_396_ (mux_1level_tapbuf_size2_396_inbus, chanx_1__1__out_93_ , mux_1level_tapbuf_size2_396_sram_blwl_out[436:436] ,
mux_1level_tapbuf_size2_396_sram_blwl_outb[436:436] );
//----- SRAM bits for MUX[396], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_436_ (mux_1level_tapbuf_size2_396_sram_blwl_out[436:436] ,mux_1level_tapbuf_size2_396_sram_blwl_out[436:436] ,mux_1level_tapbuf_size2_396_sram_blwl_outb[436:436] ,mux_1level_tapbuf_size2_396_configbus0[436:436], mux_1level_tapbuf_size2_396_configbus1[436:436] , mux_1level_tapbuf_size2_396_configbus0_b[436:436] );
wire [0:1] mux_1level_tapbuf_size2_397_inbus;
assign mux_1level_tapbuf_size2_397_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_397_inbus[1] = chany_1__1__in_92_ ;
wire [437:437] mux_1level_tapbuf_size2_397_configbus0;
wire [437:437] mux_1level_tapbuf_size2_397_configbus1;
wire [437:437] mux_1level_tapbuf_size2_397_sram_blwl_out ;
wire [437:437] mux_1level_tapbuf_size2_397_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_397_configbus0[437:437] = sram_blwl_bl[437:437] ;
assign mux_1level_tapbuf_size2_397_configbus1[437:437] = sram_blwl_wl[437:437] ;
wire [437:437] mux_1level_tapbuf_size2_397_configbus0_b;
assign mux_1level_tapbuf_size2_397_configbus0_b[437:437] = sram_blwl_blb[437:437] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_397_ (mux_1level_tapbuf_size2_397_inbus, chanx_1__1__out_95_ , mux_1level_tapbuf_size2_397_sram_blwl_out[437:437] ,
mux_1level_tapbuf_size2_397_sram_blwl_outb[437:437] );
//----- SRAM bits for MUX[397], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_437_ (mux_1level_tapbuf_size2_397_sram_blwl_out[437:437] ,mux_1level_tapbuf_size2_397_sram_blwl_out[437:437] ,mux_1level_tapbuf_size2_397_sram_blwl_outb[437:437] ,mux_1level_tapbuf_size2_397_configbus0[437:437], mux_1level_tapbuf_size2_397_configbus1[437:437] , mux_1level_tapbuf_size2_397_configbus0_b[437:437] );
wire [0:1] mux_1level_tapbuf_size2_398_inbus;
assign mux_1level_tapbuf_size2_398_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_398_inbus[1] = chany_1__1__in_94_ ;
wire [438:438] mux_1level_tapbuf_size2_398_configbus0;
wire [438:438] mux_1level_tapbuf_size2_398_configbus1;
wire [438:438] mux_1level_tapbuf_size2_398_sram_blwl_out ;
wire [438:438] mux_1level_tapbuf_size2_398_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_398_configbus0[438:438] = sram_blwl_bl[438:438] ;
assign mux_1level_tapbuf_size2_398_configbus1[438:438] = sram_blwl_wl[438:438] ;
wire [438:438] mux_1level_tapbuf_size2_398_configbus0_b;
assign mux_1level_tapbuf_size2_398_configbus0_b[438:438] = sram_blwl_blb[438:438] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_398_ (mux_1level_tapbuf_size2_398_inbus, chanx_1__1__out_97_ , mux_1level_tapbuf_size2_398_sram_blwl_out[438:438] ,
mux_1level_tapbuf_size2_398_sram_blwl_outb[438:438] );
//----- SRAM bits for MUX[398], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_438_ (mux_1level_tapbuf_size2_398_sram_blwl_out[438:438] ,mux_1level_tapbuf_size2_398_sram_blwl_out[438:438] ,mux_1level_tapbuf_size2_398_sram_blwl_outb[438:438] ,mux_1level_tapbuf_size2_398_configbus0[438:438], mux_1level_tapbuf_size2_398_configbus1[438:438] , mux_1level_tapbuf_size2_398_configbus0_b[438:438] );
wire [0:1] mux_1level_tapbuf_size2_399_inbus;
assign mux_1level_tapbuf_size2_399_inbus[0] =  grid_1__2__pin_0__2__13_;
assign mux_1level_tapbuf_size2_399_inbus[1] = chany_1__1__in_96_ ;
wire [439:439] mux_1level_tapbuf_size2_399_configbus0;
wire [439:439] mux_1level_tapbuf_size2_399_configbus1;
wire [439:439] mux_1level_tapbuf_size2_399_sram_blwl_out ;
wire [439:439] mux_1level_tapbuf_size2_399_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_399_configbus0[439:439] = sram_blwl_bl[439:439] ;
assign mux_1level_tapbuf_size2_399_configbus1[439:439] = sram_blwl_wl[439:439] ;
wire [439:439] mux_1level_tapbuf_size2_399_configbus0_b;
assign mux_1level_tapbuf_size2_399_configbus0_b[439:439] = sram_blwl_blb[439:439] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_399_ (mux_1level_tapbuf_size2_399_inbus, chanx_1__1__out_99_ , mux_1level_tapbuf_size2_399_sram_blwl_out[439:439] ,
mux_1level_tapbuf_size2_399_sram_blwl_outb[439:439] );
//----- SRAM bits for MUX[399], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_439_ (mux_1level_tapbuf_size2_399_sram_blwl_out[439:439] ,mux_1level_tapbuf_size2_399_sram_blwl_out[439:439] ,mux_1level_tapbuf_size2_399_sram_blwl_outb[439:439] ,mux_1level_tapbuf_size2_399_configbus0[439:439], mux_1level_tapbuf_size2_399_configbus1[439:439] , mux_1level_tapbuf_size2_399_configbus0_b[439:439] );
endmodule
//----- END Verilog Module of Switch Box[1][1] -----

