//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Connection Block - X direction  [1][0] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Verilog Module of Connection Box -X direction [1][0] -----
module cbx_1__0_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input chanx_1__0__midout_0_, 

input chanx_1__0__midout_1_, 

input chanx_1__0__midout_2_, 

input chanx_1__0__midout_3_, 

input chanx_1__0__midout_4_, 

input chanx_1__0__midout_5_, 

input chanx_1__0__midout_6_, 

input chanx_1__0__midout_7_, 

input chanx_1__0__midout_8_, 

input chanx_1__0__midout_9_, 

input chanx_1__0__midout_10_, 

input chanx_1__0__midout_11_, 

input chanx_1__0__midout_12_, 

input chanx_1__0__midout_13_, 

input chanx_1__0__midout_14_, 

input chanx_1__0__midout_15_, 

input chanx_1__0__midout_16_, 

input chanx_1__0__midout_17_, 

input chanx_1__0__midout_18_, 

input chanx_1__0__midout_19_, 

input chanx_1__0__midout_20_, 

input chanx_1__0__midout_21_, 

input chanx_1__0__midout_22_, 

input chanx_1__0__midout_23_, 

input chanx_1__0__midout_24_, 

input chanx_1__0__midout_25_, 

input chanx_1__0__midout_26_, 

input chanx_1__0__midout_27_, 

input chanx_1__0__midout_28_, 

input chanx_1__0__midout_29_, 

input chanx_1__0__midout_30_, 

input chanx_1__0__midout_31_, 

input chanx_1__0__midout_32_, 

input chanx_1__0__midout_33_, 

input chanx_1__0__midout_34_, 

input chanx_1__0__midout_35_, 

input chanx_1__0__midout_36_, 

input chanx_1__0__midout_37_, 

input chanx_1__0__midout_38_, 

input chanx_1__0__midout_39_, 

input chanx_1__0__midout_40_, 

input chanx_1__0__midout_41_, 

input chanx_1__0__midout_42_, 

input chanx_1__0__midout_43_, 

input chanx_1__0__midout_44_, 

input chanx_1__0__midout_45_, 

input chanx_1__0__midout_46_, 

input chanx_1__0__midout_47_, 

input chanx_1__0__midout_48_, 

input chanx_1__0__midout_49_, 

input chanx_1__0__midout_50_, 

input chanx_1__0__midout_51_, 

input chanx_1__0__midout_52_, 

input chanx_1__0__midout_53_, 

input chanx_1__0__midout_54_, 

input chanx_1__0__midout_55_, 

input chanx_1__0__midout_56_, 

input chanx_1__0__midout_57_, 

input chanx_1__0__midout_58_, 

input chanx_1__0__midout_59_, 

input chanx_1__0__midout_60_, 

input chanx_1__0__midout_61_, 

input chanx_1__0__midout_62_, 

input chanx_1__0__midout_63_, 

input chanx_1__0__midout_64_, 

input chanx_1__0__midout_65_, 

input chanx_1__0__midout_66_, 

input chanx_1__0__midout_67_, 

input chanx_1__0__midout_68_, 

input chanx_1__0__midout_69_, 

input chanx_1__0__midout_70_, 

input chanx_1__0__midout_71_, 

input chanx_1__0__midout_72_, 

input chanx_1__0__midout_73_, 

input chanx_1__0__midout_74_, 

input chanx_1__0__midout_75_, 

input chanx_1__0__midout_76_, 

input chanx_1__0__midout_77_, 

input chanx_1__0__midout_78_, 

input chanx_1__0__midout_79_, 

input chanx_1__0__midout_80_, 

input chanx_1__0__midout_81_, 

input chanx_1__0__midout_82_, 

input chanx_1__0__midout_83_, 

input chanx_1__0__midout_84_, 

input chanx_1__0__midout_85_, 

input chanx_1__0__midout_86_, 

input chanx_1__0__midout_87_, 

input chanx_1__0__midout_88_, 

input chanx_1__0__midout_89_, 

input chanx_1__0__midout_90_, 

input chanx_1__0__midout_91_, 

input chanx_1__0__midout_92_, 

input chanx_1__0__midout_93_, 

input chanx_1__0__midout_94_, 

input chanx_1__0__midout_95_, 

input chanx_1__0__midout_96_, 

input chanx_1__0__midout_97_, 

input chanx_1__0__midout_98_, 

input chanx_1__0__midout_99_, 

output  grid_1__1__pin_0__2__2_,

output  grid_1__1__pin_0__2__6_,

output  grid_1__1__pin_0__2__10_,

output  grid_1__1__pin_0__2__14_,

output  grid_1__1__pin_0__2__18_,

output  grid_1__1__pin_0__2__22_,

output  grid_1__1__pin_0__2__26_,

output  grid_1__1__pin_0__2__30_,

output  grid_1__1__pin_0__2__34_,

output  grid_1__1__pin_0__2__38_,

output  grid_1__1__pin_0__2__50_,

output  grid_1__0__pin_0__0__0_,

output  grid_1__0__pin_0__0__2_,

output  grid_1__0__pin_0__0__4_,

output  grid_1__0__pin_0__0__6_,

output  grid_1__0__pin_0__0__8_,

output  grid_1__0__pin_0__0__10_,

output  grid_1__0__pin_0__0__12_,

output  grid_1__0__pin_0__0__14_,

input [440:583] sram_blwl_bl ,
input [440:583] sram_blwl_wl ,
input [440:583] sram_blwl_blb );
wire [0:15] mux_2level_tapbuf_size16_0_inbus;
assign mux_2level_tapbuf_size16_0_inbus[0] = chanx_1__0__midout_0_;
assign mux_2level_tapbuf_size16_0_inbus[1] = chanx_1__0__midout_1_;
assign mux_2level_tapbuf_size16_0_inbus[2] = chanx_1__0__midout_12_;
assign mux_2level_tapbuf_size16_0_inbus[3] = chanx_1__0__midout_13_;
assign mux_2level_tapbuf_size16_0_inbus[4] = chanx_1__0__midout_24_;
assign mux_2level_tapbuf_size16_0_inbus[5] = chanx_1__0__midout_25_;
assign mux_2level_tapbuf_size16_0_inbus[6] = chanx_1__0__midout_38_;
assign mux_2level_tapbuf_size16_0_inbus[7] = chanx_1__0__midout_39_;
assign mux_2level_tapbuf_size16_0_inbus[8] = chanx_1__0__midout_50_;
assign mux_2level_tapbuf_size16_0_inbus[9] = chanx_1__0__midout_51_;
assign mux_2level_tapbuf_size16_0_inbus[10] = chanx_1__0__midout_62_;
assign mux_2level_tapbuf_size16_0_inbus[11] = chanx_1__0__midout_63_;
assign mux_2level_tapbuf_size16_0_inbus[12] = chanx_1__0__midout_74_;
assign mux_2level_tapbuf_size16_0_inbus[13] = chanx_1__0__midout_75_;
assign mux_2level_tapbuf_size16_0_inbus[14] = chanx_1__0__midout_88_;
assign mux_2level_tapbuf_size16_0_inbus[15] = chanx_1__0__midout_89_;
wire [440:447] mux_2level_tapbuf_size16_0_configbus0;
wire [440:447] mux_2level_tapbuf_size16_0_configbus1;
wire [440:447] mux_2level_tapbuf_size16_0_sram_blwl_out ;
wire [440:447] mux_2level_tapbuf_size16_0_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_0_configbus0[440:447] = sram_blwl_bl[440:447] ;
assign mux_2level_tapbuf_size16_0_configbus1[440:447] = sram_blwl_wl[440:447] ;
wire [440:447] mux_2level_tapbuf_size16_0_configbus0_b;
assign mux_2level_tapbuf_size16_0_configbus0_b[440:447] = sram_blwl_blb[440:447] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_0_ (mux_2level_tapbuf_size16_0_inbus, grid_1__1__pin_0__2__2_, mux_2level_tapbuf_size16_0_sram_blwl_out[440:447] ,
mux_2level_tapbuf_size16_0_sram_blwl_outb[440:447] );
//----- SRAM bits for MUX[0], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_440_ (mux_2level_tapbuf_size16_0_sram_blwl_out[440:440] ,mux_2level_tapbuf_size16_0_sram_blwl_out[440:440] ,mux_2level_tapbuf_size16_0_sram_blwl_outb[440:440] ,mux_2level_tapbuf_size16_0_configbus0[440:440], mux_2level_tapbuf_size16_0_configbus1[440:440] , mux_2level_tapbuf_size16_0_configbus0_b[440:440] );
sram6T_blwl sram_blwl_441_ (mux_2level_tapbuf_size16_0_sram_blwl_out[441:441] ,mux_2level_tapbuf_size16_0_sram_blwl_out[441:441] ,mux_2level_tapbuf_size16_0_sram_blwl_outb[441:441] ,mux_2level_tapbuf_size16_0_configbus0[441:441], mux_2level_tapbuf_size16_0_configbus1[441:441] , mux_2level_tapbuf_size16_0_configbus0_b[441:441] );
sram6T_blwl sram_blwl_442_ (mux_2level_tapbuf_size16_0_sram_blwl_out[442:442] ,mux_2level_tapbuf_size16_0_sram_blwl_out[442:442] ,mux_2level_tapbuf_size16_0_sram_blwl_outb[442:442] ,mux_2level_tapbuf_size16_0_configbus0[442:442], mux_2level_tapbuf_size16_0_configbus1[442:442] , mux_2level_tapbuf_size16_0_configbus0_b[442:442] );
sram6T_blwl sram_blwl_443_ (mux_2level_tapbuf_size16_0_sram_blwl_out[443:443] ,mux_2level_tapbuf_size16_0_sram_blwl_out[443:443] ,mux_2level_tapbuf_size16_0_sram_blwl_outb[443:443] ,mux_2level_tapbuf_size16_0_configbus0[443:443], mux_2level_tapbuf_size16_0_configbus1[443:443] , mux_2level_tapbuf_size16_0_configbus0_b[443:443] );
sram6T_blwl sram_blwl_444_ (mux_2level_tapbuf_size16_0_sram_blwl_out[444:444] ,mux_2level_tapbuf_size16_0_sram_blwl_out[444:444] ,mux_2level_tapbuf_size16_0_sram_blwl_outb[444:444] ,mux_2level_tapbuf_size16_0_configbus0[444:444], mux_2level_tapbuf_size16_0_configbus1[444:444] , mux_2level_tapbuf_size16_0_configbus0_b[444:444] );
sram6T_blwl sram_blwl_445_ (mux_2level_tapbuf_size16_0_sram_blwl_out[445:445] ,mux_2level_tapbuf_size16_0_sram_blwl_out[445:445] ,mux_2level_tapbuf_size16_0_sram_blwl_outb[445:445] ,mux_2level_tapbuf_size16_0_configbus0[445:445], mux_2level_tapbuf_size16_0_configbus1[445:445] , mux_2level_tapbuf_size16_0_configbus0_b[445:445] );
sram6T_blwl sram_blwl_446_ (mux_2level_tapbuf_size16_0_sram_blwl_out[446:446] ,mux_2level_tapbuf_size16_0_sram_blwl_out[446:446] ,mux_2level_tapbuf_size16_0_sram_blwl_outb[446:446] ,mux_2level_tapbuf_size16_0_configbus0[446:446], mux_2level_tapbuf_size16_0_configbus1[446:446] , mux_2level_tapbuf_size16_0_configbus0_b[446:446] );
sram6T_blwl sram_blwl_447_ (mux_2level_tapbuf_size16_0_sram_blwl_out[447:447] ,mux_2level_tapbuf_size16_0_sram_blwl_out[447:447] ,mux_2level_tapbuf_size16_0_sram_blwl_outb[447:447] ,mux_2level_tapbuf_size16_0_configbus0[447:447], mux_2level_tapbuf_size16_0_configbus1[447:447] , mux_2level_tapbuf_size16_0_configbus0_b[447:447] );
wire [0:15] mux_2level_tapbuf_size16_1_inbus;
assign mux_2level_tapbuf_size16_1_inbus[0] = chanx_1__0__midout_0_;
assign mux_2level_tapbuf_size16_1_inbus[1] = chanx_1__0__midout_1_;
assign mux_2level_tapbuf_size16_1_inbus[2] = chanx_1__0__midout_14_;
assign mux_2level_tapbuf_size16_1_inbus[3] = chanx_1__0__midout_15_;
assign mux_2level_tapbuf_size16_1_inbus[4] = chanx_1__0__midout_26_;
assign mux_2level_tapbuf_size16_1_inbus[5] = chanx_1__0__midout_27_;
assign mux_2level_tapbuf_size16_1_inbus[6] = chanx_1__0__midout_38_;
assign mux_2level_tapbuf_size16_1_inbus[7] = chanx_1__0__midout_39_;
assign mux_2level_tapbuf_size16_1_inbus[8] = chanx_1__0__midout_50_;
assign mux_2level_tapbuf_size16_1_inbus[9] = chanx_1__0__midout_51_;
assign mux_2level_tapbuf_size16_1_inbus[10] = chanx_1__0__midout_64_;
assign mux_2level_tapbuf_size16_1_inbus[11] = chanx_1__0__midout_65_;
assign mux_2level_tapbuf_size16_1_inbus[12] = chanx_1__0__midout_76_;
assign mux_2level_tapbuf_size16_1_inbus[13] = chanx_1__0__midout_77_;
assign mux_2level_tapbuf_size16_1_inbus[14] = chanx_1__0__midout_88_;
assign mux_2level_tapbuf_size16_1_inbus[15] = chanx_1__0__midout_89_;
wire [448:455] mux_2level_tapbuf_size16_1_configbus0;
wire [448:455] mux_2level_tapbuf_size16_1_configbus1;
wire [448:455] mux_2level_tapbuf_size16_1_sram_blwl_out ;
wire [448:455] mux_2level_tapbuf_size16_1_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_1_configbus0[448:455] = sram_blwl_bl[448:455] ;
assign mux_2level_tapbuf_size16_1_configbus1[448:455] = sram_blwl_wl[448:455] ;
wire [448:455] mux_2level_tapbuf_size16_1_configbus0_b;
assign mux_2level_tapbuf_size16_1_configbus0_b[448:455] = sram_blwl_blb[448:455] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_1_ (mux_2level_tapbuf_size16_1_inbus, grid_1__1__pin_0__2__6_, mux_2level_tapbuf_size16_1_sram_blwl_out[448:455] ,
mux_2level_tapbuf_size16_1_sram_blwl_outb[448:455] );
//----- SRAM bits for MUX[1], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_448_ (mux_2level_tapbuf_size16_1_sram_blwl_out[448:448] ,mux_2level_tapbuf_size16_1_sram_blwl_out[448:448] ,mux_2level_tapbuf_size16_1_sram_blwl_outb[448:448] ,mux_2level_tapbuf_size16_1_configbus0[448:448], mux_2level_tapbuf_size16_1_configbus1[448:448] , mux_2level_tapbuf_size16_1_configbus0_b[448:448] );
sram6T_blwl sram_blwl_449_ (mux_2level_tapbuf_size16_1_sram_blwl_out[449:449] ,mux_2level_tapbuf_size16_1_sram_blwl_out[449:449] ,mux_2level_tapbuf_size16_1_sram_blwl_outb[449:449] ,mux_2level_tapbuf_size16_1_configbus0[449:449], mux_2level_tapbuf_size16_1_configbus1[449:449] , mux_2level_tapbuf_size16_1_configbus0_b[449:449] );
sram6T_blwl sram_blwl_450_ (mux_2level_tapbuf_size16_1_sram_blwl_out[450:450] ,mux_2level_tapbuf_size16_1_sram_blwl_out[450:450] ,mux_2level_tapbuf_size16_1_sram_blwl_outb[450:450] ,mux_2level_tapbuf_size16_1_configbus0[450:450], mux_2level_tapbuf_size16_1_configbus1[450:450] , mux_2level_tapbuf_size16_1_configbus0_b[450:450] );
sram6T_blwl sram_blwl_451_ (mux_2level_tapbuf_size16_1_sram_blwl_out[451:451] ,mux_2level_tapbuf_size16_1_sram_blwl_out[451:451] ,mux_2level_tapbuf_size16_1_sram_blwl_outb[451:451] ,mux_2level_tapbuf_size16_1_configbus0[451:451], mux_2level_tapbuf_size16_1_configbus1[451:451] , mux_2level_tapbuf_size16_1_configbus0_b[451:451] );
sram6T_blwl sram_blwl_452_ (mux_2level_tapbuf_size16_1_sram_blwl_out[452:452] ,mux_2level_tapbuf_size16_1_sram_blwl_out[452:452] ,mux_2level_tapbuf_size16_1_sram_blwl_outb[452:452] ,mux_2level_tapbuf_size16_1_configbus0[452:452], mux_2level_tapbuf_size16_1_configbus1[452:452] , mux_2level_tapbuf_size16_1_configbus0_b[452:452] );
sram6T_blwl sram_blwl_453_ (mux_2level_tapbuf_size16_1_sram_blwl_out[453:453] ,mux_2level_tapbuf_size16_1_sram_blwl_out[453:453] ,mux_2level_tapbuf_size16_1_sram_blwl_outb[453:453] ,mux_2level_tapbuf_size16_1_configbus0[453:453], mux_2level_tapbuf_size16_1_configbus1[453:453] , mux_2level_tapbuf_size16_1_configbus0_b[453:453] );
sram6T_blwl sram_blwl_454_ (mux_2level_tapbuf_size16_1_sram_blwl_out[454:454] ,mux_2level_tapbuf_size16_1_sram_blwl_out[454:454] ,mux_2level_tapbuf_size16_1_sram_blwl_outb[454:454] ,mux_2level_tapbuf_size16_1_configbus0[454:454], mux_2level_tapbuf_size16_1_configbus1[454:454] , mux_2level_tapbuf_size16_1_configbus0_b[454:454] );
sram6T_blwl sram_blwl_455_ (mux_2level_tapbuf_size16_1_sram_blwl_out[455:455] ,mux_2level_tapbuf_size16_1_sram_blwl_out[455:455] ,mux_2level_tapbuf_size16_1_sram_blwl_outb[455:455] ,mux_2level_tapbuf_size16_1_configbus0[455:455], mux_2level_tapbuf_size16_1_configbus1[455:455] , mux_2level_tapbuf_size16_1_configbus0_b[455:455] );
wire [0:15] mux_2level_tapbuf_size16_2_inbus;
assign mux_2level_tapbuf_size16_2_inbus[0] = chanx_1__0__midout_2_;
assign mux_2level_tapbuf_size16_2_inbus[1] = chanx_1__0__midout_3_;
assign mux_2level_tapbuf_size16_2_inbus[2] = chanx_1__0__midout_14_;
assign mux_2level_tapbuf_size16_2_inbus[3] = chanx_1__0__midout_15_;
assign mux_2level_tapbuf_size16_2_inbus[4] = chanx_1__0__midout_28_;
assign mux_2level_tapbuf_size16_2_inbus[5] = chanx_1__0__midout_29_;
assign mux_2level_tapbuf_size16_2_inbus[6] = chanx_1__0__midout_40_;
assign mux_2level_tapbuf_size16_2_inbus[7] = chanx_1__0__midout_41_;
assign mux_2level_tapbuf_size16_2_inbus[8] = chanx_1__0__midout_52_;
assign mux_2level_tapbuf_size16_2_inbus[9] = chanx_1__0__midout_53_;
assign mux_2level_tapbuf_size16_2_inbus[10] = chanx_1__0__midout_64_;
assign mux_2level_tapbuf_size16_2_inbus[11] = chanx_1__0__midout_65_;
assign mux_2level_tapbuf_size16_2_inbus[12] = chanx_1__0__midout_78_;
assign mux_2level_tapbuf_size16_2_inbus[13] = chanx_1__0__midout_79_;
assign mux_2level_tapbuf_size16_2_inbus[14] = chanx_1__0__midout_90_;
assign mux_2level_tapbuf_size16_2_inbus[15] = chanx_1__0__midout_91_;
wire [456:463] mux_2level_tapbuf_size16_2_configbus0;
wire [456:463] mux_2level_tapbuf_size16_2_configbus1;
wire [456:463] mux_2level_tapbuf_size16_2_sram_blwl_out ;
wire [456:463] mux_2level_tapbuf_size16_2_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_2_configbus0[456:463] = sram_blwl_bl[456:463] ;
assign mux_2level_tapbuf_size16_2_configbus1[456:463] = sram_blwl_wl[456:463] ;
wire [456:463] mux_2level_tapbuf_size16_2_configbus0_b;
assign mux_2level_tapbuf_size16_2_configbus0_b[456:463] = sram_blwl_blb[456:463] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_2_ (mux_2level_tapbuf_size16_2_inbus, grid_1__1__pin_0__2__10_, mux_2level_tapbuf_size16_2_sram_blwl_out[456:463] ,
mux_2level_tapbuf_size16_2_sram_blwl_outb[456:463] );
//----- SRAM bits for MUX[2], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_456_ (mux_2level_tapbuf_size16_2_sram_blwl_out[456:456] ,mux_2level_tapbuf_size16_2_sram_blwl_out[456:456] ,mux_2level_tapbuf_size16_2_sram_blwl_outb[456:456] ,mux_2level_tapbuf_size16_2_configbus0[456:456], mux_2level_tapbuf_size16_2_configbus1[456:456] , mux_2level_tapbuf_size16_2_configbus0_b[456:456] );
sram6T_blwl sram_blwl_457_ (mux_2level_tapbuf_size16_2_sram_blwl_out[457:457] ,mux_2level_tapbuf_size16_2_sram_blwl_out[457:457] ,mux_2level_tapbuf_size16_2_sram_blwl_outb[457:457] ,mux_2level_tapbuf_size16_2_configbus0[457:457], mux_2level_tapbuf_size16_2_configbus1[457:457] , mux_2level_tapbuf_size16_2_configbus0_b[457:457] );
sram6T_blwl sram_blwl_458_ (mux_2level_tapbuf_size16_2_sram_blwl_out[458:458] ,mux_2level_tapbuf_size16_2_sram_blwl_out[458:458] ,mux_2level_tapbuf_size16_2_sram_blwl_outb[458:458] ,mux_2level_tapbuf_size16_2_configbus0[458:458], mux_2level_tapbuf_size16_2_configbus1[458:458] , mux_2level_tapbuf_size16_2_configbus0_b[458:458] );
sram6T_blwl sram_blwl_459_ (mux_2level_tapbuf_size16_2_sram_blwl_out[459:459] ,mux_2level_tapbuf_size16_2_sram_blwl_out[459:459] ,mux_2level_tapbuf_size16_2_sram_blwl_outb[459:459] ,mux_2level_tapbuf_size16_2_configbus0[459:459], mux_2level_tapbuf_size16_2_configbus1[459:459] , mux_2level_tapbuf_size16_2_configbus0_b[459:459] );
sram6T_blwl sram_blwl_460_ (mux_2level_tapbuf_size16_2_sram_blwl_out[460:460] ,mux_2level_tapbuf_size16_2_sram_blwl_out[460:460] ,mux_2level_tapbuf_size16_2_sram_blwl_outb[460:460] ,mux_2level_tapbuf_size16_2_configbus0[460:460], mux_2level_tapbuf_size16_2_configbus1[460:460] , mux_2level_tapbuf_size16_2_configbus0_b[460:460] );
sram6T_blwl sram_blwl_461_ (mux_2level_tapbuf_size16_2_sram_blwl_out[461:461] ,mux_2level_tapbuf_size16_2_sram_blwl_out[461:461] ,mux_2level_tapbuf_size16_2_sram_blwl_outb[461:461] ,mux_2level_tapbuf_size16_2_configbus0[461:461], mux_2level_tapbuf_size16_2_configbus1[461:461] , mux_2level_tapbuf_size16_2_configbus0_b[461:461] );
sram6T_blwl sram_blwl_462_ (mux_2level_tapbuf_size16_2_sram_blwl_out[462:462] ,mux_2level_tapbuf_size16_2_sram_blwl_out[462:462] ,mux_2level_tapbuf_size16_2_sram_blwl_outb[462:462] ,mux_2level_tapbuf_size16_2_configbus0[462:462], mux_2level_tapbuf_size16_2_configbus1[462:462] , mux_2level_tapbuf_size16_2_configbus0_b[462:462] );
sram6T_blwl sram_blwl_463_ (mux_2level_tapbuf_size16_2_sram_blwl_out[463:463] ,mux_2level_tapbuf_size16_2_sram_blwl_out[463:463] ,mux_2level_tapbuf_size16_2_sram_blwl_outb[463:463] ,mux_2level_tapbuf_size16_2_configbus0[463:463], mux_2level_tapbuf_size16_2_configbus1[463:463] , mux_2level_tapbuf_size16_2_configbus0_b[463:463] );
wire [0:15] mux_2level_tapbuf_size16_3_inbus;
assign mux_2level_tapbuf_size16_3_inbus[0] = chanx_1__0__midout_4_;
assign mux_2level_tapbuf_size16_3_inbus[1] = chanx_1__0__midout_5_;
assign mux_2level_tapbuf_size16_3_inbus[2] = chanx_1__0__midout_16_;
assign mux_2level_tapbuf_size16_3_inbus[3] = chanx_1__0__midout_17_;
assign mux_2level_tapbuf_size16_3_inbus[4] = chanx_1__0__midout_28_;
assign mux_2level_tapbuf_size16_3_inbus[5] = chanx_1__0__midout_29_;
assign mux_2level_tapbuf_size16_3_inbus[6] = chanx_1__0__midout_40_;
assign mux_2level_tapbuf_size16_3_inbus[7] = chanx_1__0__midout_41_;
assign mux_2level_tapbuf_size16_3_inbus[8] = chanx_1__0__midout_54_;
assign mux_2level_tapbuf_size16_3_inbus[9] = chanx_1__0__midout_55_;
assign mux_2level_tapbuf_size16_3_inbus[10] = chanx_1__0__midout_66_;
assign mux_2level_tapbuf_size16_3_inbus[11] = chanx_1__0__midout_67_;
assign mux_2level_tapbuf_size16_3_inbus[12] = chanx_1__0__midout_78_;
assign mux_2level_tapbuf_size16_3_inbus[13] = chanx_1__0__midout_79_;
assign mux_2level_tapbuf_size16_3_inbus[14] = chanx_1__0__midout_90_;
assign mux_2level_tapbuf_size16_3_inbus[15] = chanx_1__0__midout_91_;
wire [464:471] mux_2level_tapbuf_size16_3_configbus0;
wire [464:471] mux_2level_tapbuf_size16_3_configbus1;
wire [464:471] mux_2level_tapbuf_size16_3_sram_blwl_out ;
wire [464:471] mux_2level_tapbuf_size16_3_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_3_configbus0[464:471] = sram_blwl_bl[464:471] ;
assign mux_2level_tapbuf_size16_3_configbus1[464:471] = sram_blwl_wl[464:471] ;
wire [464:471] mux_2level_tapbuf_size16_3_configbus0_b;
assign mux_2level_tapbuf_size16_3_configbus0_b[464:471] = sram_blwl_blb[464:471] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_3_ (mux_2level_tapbuf_size16_3_inbus, grid_1__1__pin_0__2__14_, mux_2level_tapbuf_size16_3_sram_blwl_out[464:471] ,
mux_2level_tapbuf_size16_3_sram_blwl_outb[464:471] );
//----- SRAM bits for MUX[3], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_464_ (mux_2level_tapbuf_size16_3_sram_blwl_out[464:464] ,mux_2level_tapbuf_size16_3_sram_blwl_out[464:464] ,mux_2level_tapbuf_size16_3_sram_blwl_outb[464:464] ,mux_2level_tapbuf_size16_3_configbus0[464:464], mux_2level_tapbuf_size16_3_configbus1[464:464] , mux_2level_tapbuf_size16_3_configbus0_b[464:464] );
sram6T_blwl sram_blwl_465_ (mux_2level_tapbuf_size16_3_sram_blwl_out[465:465] ,mux_2level_tapbuf_size16_3_sram_blwl_out[465:465] ,mux_2level_tapbuf_size16_3_sram_blwl_outb[465:465] ,mux_2level_tapbuf_size16_3_configbus0[465:465], mux_2level_tapbuf_size16_3_configbus1[465:465] , mux_2level_tapbuf_size16_3_configbus0_b[465:465] );
sram6T_blwl sram_blwl_466_ (mux_2level_tapbuf_size16_3_sram_blwl_out[466:466] ,mux_2level_tapbuf_size16_3_sram_blwl_out[466:466] ,mux_2level_tapbuf_size16_3_sram_blwl_outb[466:466] ,mux_2level_tapbuf_size16_3_configbus0[466:466], mux_2level_tapbuf_size16_3_configbus1[466:466] , mux_2level_tapbuf_size16_3_configbus0_b[466:466] );
sram6T_blwl sram_blwl_467_ (mux_2level_tapbuf_size16_3_sram_blwl_out[467:467] ,mux_2level_tapbuf_size16_3_sram_blwl_out[467:467] ,mux_2level_tapbuf_size16_3_sram_blwl_outb[467:467] ,mux_2level_tapbuf_size16_3_configbus0[467:467], mux_2level_tapbuf_size16_3_configbus1[467:467] , mux_2level_tapbuf_size16_3_configbus0_b[467:467] );
sram6T_blwl sram_blwl_468_ (mux_2level_tapbuf_size16_3_sram_blwl_out[468:468] ,mux_2level_tapbuf_size16_3_sram_blwl_out[468:468] ,mux_2level_tapbuf_size16_3_sram_blwl_outb[468:468] ,mux_2level_tapbuf_size16_3_configbus0[468:468], mux_2level_tapbuf_size16_3_configbus1[468:468] , mux_2level_tapbuf_size16_3_configbus0_b[468:468] );
sram6T_blwl sram_blwl_469_ (mux_2level_tapbuf_size16_3_sram_blwl_out[469:469] ,mux_2level_tapbuf_size16_3_sram_blwl_out[469:469] ,mux_2level_tapbuf_size16_3_sram_blwl_outb[469:469] ,mux_2level_tapbuf_size16_3_configbus0[469:469], mux_2level_tapbuf_size16_3_configbus1[469:469] , mux_2level_tapbuf_size16_3_configbus0_b[469:469] );
sram6T_blwl sram_blwl_470_ (mux_2level_tapbuf_size16_3_sram_blwl_out[470:470] ,mux_2level_tapbuf_size16_3_sram_blwl_out[470:470] ,mux_2level_tapbuf_size16_3_sram_blwl_outb[470:470] ,mux_2level_tapbuf_size16_3_configbus0[470:470], mux_2level_tapbuf_size16_3_configbus1[470:470] , mux_2level_tapbuf_size16_3_configbus0_b[470:470] );
sram6T_blwl sram_blwl_471_ (mux_2level_tapbuf_size16_3_sram_blwl_out[471:471] ,mux_2level_tapbuf_size16_3_sram_blwl_out[471:471] ,mux_2level_tapbuf_size16_3_sram_blwl_outb[471:471] ,mux_2level_tapbuf_size16_3_configbus0[471:471], mux_2level_tapbuf_size16_3_configbus1[471:471] , mux_2level_tapbuf_size16_3_configbus0_b[471:471] );
wire [0:15] mux_2level_tapbuf_size16_4_inbus;
assign mux_2level_tapbuf_size16_4_inbus[0] = chanx_1__0__midout_4_;
assign mux_2level_tapbuf_size16_4_inbus[1] = chanx_1__0__midout_5_;
assign mux_2level_tapbuf_size16_4_inbus[2] = chanx_1__0__midout_18_;
assign mux_2level_tapbuf_size16_4_inbus[3] = chanx_1__0__midout_19_;
assign mux_2level_tapbuf_size16_4_inbus[4] = chanx_1__0__midout_30_;
assign mux_2level_tapbuf_size16_4_inbus[5] = chanx_1__0__midout_31_;
assign mux_2level_tapbuf_size16_4_inbus[6] = chanx_1__0__midout_42_;
assign mux_2level_tapbuf_size16_4_inbus[7] = chanx_1__0__midout_43_;
assign mux_2level_tapbuf_size16_4_inbus[8] = chanx_1__0__midout_54_;
assign mux_2level_tapbuf_size16_4_inbus[9] = chanx_1__0__midout_55_;
assign mux_2level_tapbuf_size16_4_inbus[10] = chanx_1__0__midout_68_;
assign mux_2level_tapbuf_size16_4_inbus[11] = chanx_1__0__midout_69_;
assign mux_2level_tapbuf_size16_4_inbus[12] = chanx_1__0__midout_80_;
assign mux_2level_tapbuf_size16_4_inbus[13] = chanx_1__0__midout_81_;
assign mux_2level_tapbuf_size16_4_inbus[14] = chanx_1__0__midout_92_;
assign mux_2level_tapbuf_size16_4_inbus[15] = chanx_1__0__midout_93_;
wire [472:479] mux_2level_tapbuf_size16_4_configbus0;
wire [472:479] mux_2level_tapbuf_size16_4_configbus1;
wire [472:479] mux_2level_tapbuf_size16_4_sram_blwl_out ;
wire [472:479] mux_2level_tapbuf_size16_4_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_4_configbus0[472:479] = sram_blwl_bl[472:479] ;
assign mux_2level_tapbuf_size16_4_configbus1[472:479] = sram_blwl_wl[472:479] ;
wire [472:479] mux_2level_tapbuf_size16_4_configbus0_b;
assign mux_2level_tapbuf_size16_4_configbus0_b[472:479] = sram_blwl_blb[472:479] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_4_ (mux_2level_tapbuf_size16_4_inbus, grid_1__1__pin_0__2__18_, mux_2level_tapbuf_size16_4_sram_blwl_out[472:479] ,
mux_2level_tapbuf_size16_4_sram_blwl_outb[472:479] );
//----- SRAM bits for MUX[4], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_472_ (mux_2level_tapbuf_size16_4_sram_blwl_out[472:472] ,mux_2level_tapbuf_size16_4_sram_blwl_out[472:472] ,mux_2level_tapbuf_size16_4_sram_blwl_outb[472:472] ,mux_2level_tapbuf_size16_4_configbus0[472:472], mux_2level_tapbuf_size16_4_configbus1[472:472] , mux_2level_tapbuf_size16_4_configbus0_b[472:472] );
sram6T_blwl sram_blwl_473_ (mux_2level_tapbuf_size16_4_sram_blwl_out[473:473] ,mux_2level_tapbuf_size16_4_sram_blwl_out[473:473] ,mux_2level_tapbuf_size16_4_sram_blwl_outb[473:473] ,mux_2level_tapbuf_size16_4_configbus0[473:473], mux_2level_tapbuf_size16_4_configbus1[473:473] , mux_2level_tapbuf_size16_4_configbus0_b[473:473] );
sram6T_blwl sram_blwl_474_ (mux_2level_tapbuf_size16_4_sram_blwl_out[474:474] ,mux_2level_tapbuf_size16_4_sram_blwl_out[474:474] ,mux_2level_tapbuf_size16_4_sram_blwl_outb[474:474] ,mux_2level_tapbuf_size16_4_configbus0[474:474], mux_2level_tapbuf_size16_4_configbus1[474:474] , mux_2level_tapbuf_size16_4_configbus0_b[474:474] );
sram6T_blwl sram_blwl_475_ (mux_2level_tapbuf_size16_4_sram_blwl_out[475:475] ,mux_2level_tapbuf_size16_4_sram_blwl_out[475:475] ,mux_2level_tapbuf_size16_4_sram_blwl_outb[475:475] ,mux_2level_tapbuf_size16_4_configbus0[475:475], mux_2level_tapbuf_size16_4_configbus1[475:475] , mux_2level_tapbuf_size16_4_configbus0_b[475:475] );
sram6T_blwl sram_blwl_476_ (mux_2level_tapbuf_size16_4_sram_blwl_out[476:476] ,mux_2level_tapbuf_size16_4_sram_blwl_out[476:476] ,mux_2level_tapbuf_size16_4_sram_blwl_outb[476:476] ,mux_2level_tapbuf_size16_4_configbus0[476:476], mux_2level_tapbuf_size16_4_configbus1[476:476] , mux_2level_tapbuf_size16_4_configbus0_b[476:476] );
sram6T_blwl sram_blwl_477_ (mux_2level_tapbuf_size16_4_sram_blwl_out[477:477] ,mux_2level_tapbuf_size16_4_sram_blwl_out[477:477] ,mux_2level_tapbuf_size16_4_sram_blwl_outb[477:477] ,mux_2level_tapbuf_size16_4_configbus0[477:477], mux_2level_tapbuf_size16_4_configbus1[477:477] , mux_2level_tapbuf_size16_4_configbus0_b[477:477] );
sram6T_blwl sram_blwl_478_ (mux_2level_tapbuf_size16_4_sram_blwl_out[478:478] ,mux_2level_tapbuf_size16_4_sram_blwl_out[478:478] ,mux_2level_tapbuf_size16_4_sram_blwl_outb[478:478] ,mux_2level_tapbuf_size16_4_configbus0[478:478], mux_2level_tapbuf_size16_4_configbus1[478:478] , mux_2level_tapbuf_size16_4_configbus0_b[478:478] );
sram6T_blwl sram_blwl_479_ (mux_2level_tapbuf_size16_4_sram_blwl_out[479:479] ,mux_2level_tapbuf_size16_4_sram_blwl_out[479:479] ,mux_2level_tapbuf_size16_4_sram_blwl_outb[479:479] ,mux_2level_tapbuf_size16_4_configbus0[479:479], mux_2level_tapbuf_size16_4_configbus1[479:479] , mux_2level_tapbuf_size16_4_configbus0_b[479:479] );
wire [0:15] mux_2level_tapbuf_size16_5_inbus;
assign mux_2level_tapbuf_size16_5_inbus[0] = chanx_1__0__midout_6_;
assign mux_2level_tapbuf_size16_5_inbus[1] = chanx_1__0__midout_7_;
assign mux_2level_tapbuf_size16_5_inbus[2] = chanx_1__0__midout_18_;
assign mux_2level_tapbuf_size16_5_inbus[3] = chanx_1__0__midout_19_;
assign mux_2level_tapbuf_size16_5_inbus[4] = chanx_1__0__midout_30_;
assign mux_2level_tapbuf_size16_5_inbus[5] = chanx_1__0__midout_31_;
assign mux_2level_tapbuf_size16_5_inbus[6] = chanx_1__0__midout_44_;
assign mux_2level_tapbuf_size16_5_inbus[7] = chanx_1__0__midout_45_;
assign mux_2level_tapbuf_size16_5_inbus[8] = chanx_1__0__midout_56_;
assign mux_2level_tapbuf_size16_5_inbus[9] = chanx_1__0__midout_57_;
assign mux_2level_tapbuf_size16_5_inbus[10] = chanx_1__0__midout_68_;
assign mux_2level_tapbuf_size16_5_inbus[11] = chanx_1__0__midout_69_;
assign mux_2level_tapbuf_size16_5_inbus[12] = chanx_1__0__midout_80_;
assign mux_2level_tapbuf_size16_5_inbus[13] = chanx_1__0__midout_81_;
assign mux_2level_tapbuf_size16_5_inbus[14] = chanx_1__0__midout_94_;
assign mux_2level_tapbuf_size16_5_inbus[15] = chanx_1__0__midout_95_;
wire [480:487] mux_2level_tapbuf_size16_5_configbus0;
wire [480:487] mux_2level_tapbuf_size16_5_configbus1;
wire [480:487] mux_2level_tapbuf_size16_5_sram_blwl_out ;
wire [480:487] mux_2level_tapbuf_size16_5_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_5_configbus0[480:487] = sram_blwl_bl[480:487] ;
assign mux_2level_tapbuf_size16_5_configbus1[480:487] = sram_blwl_wl[480:487] ;
wire [480:487] mux_2level_tapbuf_size16_5_configbus0_b;
assign mux_2level_tapbuf_size16_5_configbus0_b[480:487] = sram_blwl_blb[480:487] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_5_ (mux_2level_tapbuf_size16_5_inbus, grid_1__1__pin_0__2__22_, mux_2level_tapbuf_size16_5_sram_blwl_out[480:487] ,
mux_2level_tapbuf_size16_5_sram_blwl_outb[480:487] );
//----- SRAM bits for MUX[5], level=2, select_path_id=3. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10000001-----
sram6T_blwl sram_blwl_480_ (mux_2level_tapbuf_size16_5_sram_blwl_out[480:480] ,mux_2level_tapbuf_size16_5_sram_blwl_out[480:480] ,mux_2level_tapbuf_size16_5_sram_blwl_outb[480:480] ,mux_2level_tapbuf_size16_5_configbus0[480:480], mux_2level_tapbuf_size16_5_configbus1[480:480] , mux_2level_tapbuf_size16_5_configbus0_b[480:480] );
sram6T_blwl sram_blwl_481_ (mux_2level_tapbuf_size16_5_sram_blwl_out[481:481] ,mux_2level_tapbuf_size16_5_sram_blwl_out[481:481] ,mux_2level_tapbuf_size16_5_sram_blwl_outb[481:481] ,mux_2level_tapbuf_size16_5_configbus0[481:481], mux_2level_tapbuf_size16_5_configbus1[481:481] , mux_2level_tapbuf_size16_5_configbus0_b[481:481] );
sram6T_blwl sram_blwl_482_ (mux_2level_tapbuf_size16_5_sram_blwl_out[482:482] ,mux_2level_tapbuf_size16_5_sram_blwl_out[482:482] ,mux_2level_tapbuf_size16_5_sram_blwl_outb[482:482] ,mux_2level_tapbuf_size16_5_configbus0[482:482], mux_2level_tapbuf_size16_5_configbus1[482:482] , mux_2level_tapbuf_size16_5_configbus0_b[482:482] );
sram6T_blwl sram_blwl_483_ (mux_2level_tapbuf_size16_5_sram_blwl_out[483:483] ,mux_2level_tapbuf_size16_5_sram_blwl_out[483:483] ,mux_2level_tapbuf_size16_5_sram_blwl_outb[483:483] ,mux_2level_tapbuf_size16_5_configbus0[483:483], mux_2level_tapbuf_size16_5_configbus1[483:483] , mux_2level_tapbuf_size16_5_configbus0_b[483:483] );
sram6T_blwl sram_blwl_484_ (mux_2level_tapbuf_size16_5_sram_blwl_out[484:484] ,mux_2level_tapbuf_size16_5_sram_blwl_out[484:484] ,mux_2level_tapbuf_size16_5_sram_blwl_outb[484:484] ,mux_2level_tapbuf_size16_5_configbus0[484:484], mux_2level_tapbuf_size16_5_configbus1[484:484] , mux_2level_tapbuf_size16_5_configbus0_b[484:484] );
sram6T_blwl sram_blwl_485_ (mux_2level_tapbuf_size16_5_sram_blwl_out[485:485] ,mux_2level_tapbuf_size16_5_sram_blwl_out[485:485] ,mux_2level_tapbuf_size16_5_sram_blwl_outb[485:485] ,mux_2level_tapbuf_size16_5_configbus0[485:485], mux_2level_tapbuf_size16_5_configbus1[485:485] , mux_2level_tapbuf_size16_5_configbus0_b[485:485] );
sram6T_blwl sram_blwl_486_ (mux_2level_tapbuf_size16_5_sram_blwl_out[486:486] ,mux_2level_tapbuf_size16_5_sram_blwl_out[486:486] ,mux_2level_tapbuf_size16_5_sram_blwl_outb[486:486] ,mux_2level_tapbuf_size16_5_configbus0[486:486], mux_2level_tapbuf_size16_5_configbus1[486:486] , mux_2level_tapbuf_size16_5_configbus0_b[486:486] );
sram6T_blwl sram_blwl_487_ (mux_2level_tapbuf_size16_5_sram_blwl_out[487:487] ,mux_2level_tapbuf_size16_5_sram_blwl_out[487:487] ,mux_2level_tapbuf_size16_5_sram_blwl_outb[487:487] ,mux_2level_tapbuf_size16_5_configbus0[487:487], mux_2level_tapbuf_size16_5_configbus1[487:487] , mux_2level_tapbuf_size16_5_configbus0_b[487:487] );
wire [0:15] mux_2level_tapbuf_size16_6_inbus;
assign mux_2level_tapbuf_size16_6_inbus[0] = chanx_1__0__midout_8_;
assign mux_2level_tapbuf_size16_6_inbus[1] = chanx_1__0__midout_9_;
assign mux_2level_tapbuf_size16_6_inbus[2] = chanx_1__0__midout_20_;
assign mux_2level_tapbuf_size16_6_inbus[3] = chanx_1__0__midout_21_;
assign mux_2level_tapbuf_size16_6_inbus[4] = chanx_1__0__midout_32_;
assign mux_2level_tapbuf_size16_6_inbus[5] = chanx_1__0__midout_33_;
assign mux_2level_tapbuf_size16_6_inbus[6] = chanx_1__0__midout_44_;
assign mux_2level_tapbuf_size16_6_inbus[7] = chanx_1__0__midout_45_;
assign mux_2level_tapbuf_size16_6_inbus[8] = chanx_1__0__midout_58_;
assign mux_2level_tapbuf_size16_6_inbus[9] = chanx_1__0__midout_59_;
assign mux_2level_tapbuf_size16_6_inbus[10] = chanx_1__0__midout_70_;
assign mux_2level_tapbuf_size16_6_inbus[11] = chanx_1__0__midout_71_;
assign mux_2level_tapbuf_size16_6_inbus[12] = chanx_1__0__midout_82_;
assign mux_2level_tapbuf_size16_6_inbus[13] = chanx_1__0__midout_83_;
assign mux_2level_tapbuf_size16_6_inbus[14] = chanx_1__0__midout_94_;
assign mux_2level_tapbuf_size16_6_inbus[15] = chanx_1__0__midout_95_;
wire [488:495] mux_2level_tapbuf_size16_6_configbus0;
wire [488:495] mux_2level_tapbuf_size16_6_configbus1;
wire [488:495] mux_2level_tapbuf_size16_6_sram_blwl_out ;
wire [488:495] mux_2level_tapbuf_size16_6_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_6_configbus0[488:495] = sram_blwl_bl[488:495] ;
assign mux_2level_tapbuf_size16_6_configbus1[488:495] = sram_blwl_wl[488:495] ;
wire [488:495] mux_2level_tapbuf_size16_6_configbus0_b;
assign mux_2level_tapbuf_size16_6_configbus0_b[488:495] = sram_blwl_blb[488:495] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_6_ (mux_2level_tapbuf_size16_6_inbus, grid_1__1__pin_0__2__26_, mux_2level_tapbuf_size16_6_sram_blwl_out[488:495] ,
mux_2level_tapbuf_size16_6_sram_blwl_outb[488:495] );
//----- SRAM bits for MUX[6], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_488_ (mux_2level_tapbuf_size16_6_sram_blwl_out[488:488] ,mux_2level_tapbuf_size16_6_sram_blwl_out[488:488] ,mux_2level_tapbuf_size16_6_sram_blwl_outb[488:488] ,mux_2level_tapbuf_size16_6_configbus0[488:488], mux_2level_tapbuf_size16_6_configbus1[488:488] , mux_2level_tapbuf_size16_6_configbus0_b[488:488] );
sram6T_blwl sram_blwl_489_ (mux_2level_tapbuf_size16_6_sram_blwl_out[489:489] ,mux_2level_tapbuf_size16_6_sram_blwl_out[489:489] ,mux_2level_tapbuf_size16_6_sram_blwl_outb[489:489] ,mux_2level_tapbuf_size16_6_configbus0[489:489], mux_2level_tapbuf_size16_6_configbus1[489:489] , mux_2level_tapbuf_size16_6_configbus0_b[489:489] );
sram6T_blwl sram_blwl_490_ (mux_2level_tapbuf_size16_6_sram_blwl_out[490:490] ,mux_2level_tapbuf_size16_6_sram_blwl_out[490:490] ,mux_2level_tapbuf_size16_6_sram_blwl_outb[490:490] ,mux_2level_tapbuf_size16_6_configbus0[490:490], mux_2level_tapbuf_size16_6_configbus1[490:490] , mux_2level_tapbuf_size16_6_configbus0_b[490:490] );
sram6T_blwl sram_blwl_491_ (mux_2level_tapbuf_size16_6_sram_blwl_out[491:491] ,mux_2level_tapbuf_size16_6_sram_blwl_out[491:491] ,mux_2level_tapbuf_size16_6_sram_blwl_outb[491:491] ,mux_2level_tapbuf_size16_6_configbus0[491:491], mux_2level_tapbuf_size16_6_configbus1[491:491] , mux_2level_tapbuf_size16_6_configbus0_b[491:491] );
sram6T_blwl sram_blwl_492_ (mux_2level_tapbuf_size16_6_sram_blwl_out[492:492] ,mux_2level_tapbuf_size16_6_sram_blwl_out[492:492] ,mux_2level_tapbuf_size16_6_sram_blwl_outb[492:492] ,mux_2level_tapbuf_size16_6_configbus0[492:492], mux_2level_tapbuf_size16_6_configbus1[492:492] , mux_2level_tapbuf_size16_6_configbus0_b[492:492] );
sram6T_blwl sram_blwl_493_ (mux_2level_tapbuf_size16_6_sram_blwl_out[493:493] ,mux_2level_tapbuf_size16_6_sram_blwl_out[493:493] ,mux_2level_tapbuf_size16_6_sram_blwl_outb[493:493] ,mux_2level_tapbuf_size16_6_configbus0[493:493], mux_2level_tapbuf_size16_6_configbus1[493:493] , mux_2level_tapbuf_size16_6_configbus0_b[493:493] );
sram6T_blwl sram_blwl_494_ (mux_2level_tapbuf_size16_6_sram_blwl_out[494:494] ,mux_2level_tapbuf_size16_6_sram_blwl_out[494:494] ,mux_2level_tapbuf_size16_6_sram_blwl_outb[494:494] ,mux_2level_tapbuf_size16_6_configbus0[494:494], mux_2level_tapbuf_size16_6_configbus1[494:494] , mux_2level_tapbuf_size16_6_configbus0_b[494:494] );
sram6T_blwl sram_blwl_495_ (mux_2level_tapbuf_size16_6_sram_blwl_out[495:495] ,mux_2level_tapbuf_size16_6_sram_blwl_out[495:495] ,mux_2level_tapbuf_size16_6_sram_blwl_outb[495:495] ,mux_2level_tapbuf_size16_6_configbus0[495:495], mux_2level_tapbuf_size16_6_configbus1[495:495] , mux_2level_tapbuf_size16_6_configbus0_b[495:495] );
wire [0:15] mux_2level_tapbuf_size16_7_inbus;
assign mux_2level_tapbuf_size16_7_inbus[0] = chanx_1__0__midout_8_;
assign mux_2level_tapbuf_size16_7_inbus[1] = chanx_1__0__midout_9_;
assign mux_2level_tapbuf_size16_7_inbus[2] = chanx_1__0__midout_20_;
assign mux_2level_tapbuf_size16_7_inbus[3] = chanx_1__0__midout_21_;
assign mux_2level_tapbuf_size16_7_inbus[4] = chanx_1__0__midout_34_;
assign mux_2level_tapbuf_size16_7_inbus[5] = chanx_1__0__midout_35_;
assign mux_2level_tapbuf_size16_7_inbus[6] = chanx_1__0__midout_46_;
assign mux_2level_tapbuf_size16_7_inbus[7] = chanx_1__0__midout_47_;
assign mux_2level_tapbuf_size16_7_inbus[8] = chanx_1__0__midout_58_;
assign mux_2level_tapbuf_size16_7_inbus[9] = chanx_1__0__midout_59_;
assign mux_2level_tapbuf_size16_7_inbus[10] = chanx_1__0__midout_70_;
assign mux_2level_tapbuf_size16_7_inbus[11] = chanx_1__0__midout_71_;
assign mux_2level_tapbuf_size16_7_inbus[12] = chanx_1__0__midout_84_;
assign mux_2level_tapbuf_size16_7_inbus[13] = chanx_1__0__midout_85_;
assign mux_2level_tapbuf_size16_7_inbus[14] = chanx_1__0__midout_96_;
assign mux_2level_tapbuf_size16_7_inbus[15] = chanx_1__0__midout_97_;
wire [496:503] mux_2level_tapbuf_size16_7_configbus0;
wire [496:503] mux_2level_tapbuf_size16_7_configbus1;
wire [496:503] mux_2level_tapbuf_size16_7_sram_blwl_out ;
wire [496:503] mux_2level_tapbuf_size16_7_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_7_configbus0[496:503] = sram_blwl_bl[496:503] ;
assign mux_2level_tapbuf_size16_7_configbus1[496:503] = sram_blwl_wl[496:503] ;
wire [496:503] mux_2level_tapbuf_size16_7_configbus0_b;
assign mux_2level_tapbuf_size16_7_configbus0_b[496:503] = sram_blwl_blb[496:503] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_7_ (mux_2level_tapbuf_size16_7_inbus, grid_1__1__pin_0__2__30_, mux_2level_tapbuf_size16_7_sram_blwl_out[496:503] ,
mux_2level_tapbuf_size16_7_sram_blwl_outb[496:503] );
//----- SRAM bits for MUX[7], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_496_ (mux_2level_tapbuf_size16_7_sram_blwl_out[496:496] ,mux_2level_tapbuf_size16_7_sram_blwl_out[496:496] ,mux_2level_tapbuf_size16_7_sram_blwl_outb[496:496] ,mux_2level_tapbuf_size16_7_configbus0[496:496], mux_2level_tapbuf_size16_7_configbus1[496:496] , mux_2level_tapbuf_size16_7_configbus0_b[496:496] );
sram6T_blwl sram_blwl_497_ (mux_2level_tapbuf_size16_7_sram_blwl_out[497:497] ,mux_2level_tapbuf_size16_7_sram_blwl_out[497:497] ,mux_2level_tapbuf_size16_7_sram_blwl_outb[497:497] ,mux_2level_tapbuf_size16_7_configbus0[497:497], mux_2level_tapbuf_size16_7_configbus1[497:497] , mux_2level_tapbuf_size16_7_configbus0_b[497:497] );
sram6T_blwl sram_blwl_498_ (mux_2level_tapbuf_size16_7_sram_blwl_out[498:498] ,mux_2level_tapbuf_size16_7_sram_blwl_out[498:498] ,mux_2level_tapbuf_size16_7_sram_blwl_outb[498:498] ,mux_2level_tapbuf_size16_7_configbus0[498:498], mux_2level_tapbuf_size16_7_configbus1[498:498] , mux_2level_tapbuf_size16_7_configbus0_b[498:498] );
sram6T_blwl sram_blwl_499_ (mux_2level_tapbuf_size16_7_sram_blwl_out[499:499] ,mux_2level_tapbuf_size16_7_sram_blwl_out[499:499] ,mux_2level_tapbuf_size16_7_sram_blwl_outb[499:499] ,mux_2level_tapbuf_size16_7_configbus0[499:499], mux_2level_tapbuf_size16_7_configbus1[499:499] , mux_2level_tapbuf_size16_7_configbus0_b[499:499] );
sram6T_blwl sram_blwl_500_ (mux_2level_tapbuf_size16_7_sram_blwl_out[500:500] ,mux_2level_tapbuf_size16_7_sram_blwl_out[500:500] ,mux_2level_tapbuf_size16_7_sram_blwl_outb[500:500] ,mux_2level_tapbuf_size16_7_configbus0[500:500], mux_2level_tapbuf_size16_7_configbus1[500:500] , mux_2level_tapbuf_size16_7_configbus0_b[500:500] );
sram6T_blwl sram_blwl_501_ (mux_2level_tapbuf_size16_7_sram_blwl_out[501:501] ,mux_2level_tapbuf_size16_7_sram_blwl_out[501:501] ,mux_2level_tapbuf_size16_7_sram_blwl_outb[501:501] ,mux_2level_tapbuf_size16_7_configbus0[501:501], mux_2level_tapbuf_size16_7_configbus1[501:501] , mux_2level_tapbuf_size16_7_configbus0_b[501:501] );
sram6T_blwl sram_blwl_502_ (mux_2level_tapbuf_size16_7_sram_blwl_out[502:502] ,mux_2level_tapbuf_size16_7_sram_blwl_out[502:502] ,mux_2level_tapbuf_size16_7_sram_blwl_outb[502:502] ,mux_2level_tapbuf_size16_7_configbus0[502:502], mux_2level_tapbuf_size16_7_configbus1[502:502] , mux_2level_tapbuf_size16_7_configbus0_b[502:502] );
sram6T_blwl sram_blwl_503_ (mux_2level_tapbuf_size16_7_sram_blwl_out[503:503] ,mux_2level_tapbuf_size16_7_sram_blwl_out[503:503] ,mux_2level_tapbuf_size16_7_sram_blwl_outb[503:503] ,mux_2level_tapbuf_size16_7_configbus0[503:503], mux_2level_tapbuf_size16_7_configbus1[503:503] , mux_2level_tapbuf_size16_7_configbus0_b[503:503] );
wire [0:15] mux_2level_tapbuf_size16_8_inbus;
assign mux_2level_tapbuf_size16_8_inbus[0] = chanx_1__0__midout_10_;
assign mux_2level_tapbuf_size16_8_inbus[1] = chanx_1__0__midout_11_;
assign mux_2level_tapbuf_size16_8_inbus[2] = chanx_1__0__midout_22_;
assign mux_2level_tapbuf_size16_8_inbus[3] = chanx_1__0__midout_23_;
assign mux_2level_tapbuf_size16_8_inbus[4] = chanx_1__0__midout_34_;
assign mux_2level_tapbuf_size16_8_inbus[5] = chanx_1__0__midout_35_;
assign mux_2level_tapbuf_size16_8_inbus[6] = chanx_1__0__midout_48_;
assign mux_2level_tapbuf_size16_8_inbus[7] = chanx_1__0__midout_49_;
assign mux_2level_tapbuf_size16_8_inbus[8] = chanx_1__0__midout_60_;
assign mux_2level_tapbuf_size16_8_inbus[9] = chanx_1__0__midout_61_;
assign mux_2level_tapbuf_size16_8_inbus[10] = chanx_1__0__midout_72_;
assign mux_2level_tapbuf_size16_8_inbus[11] = chanx_1__0__midout_73_;
assign mux_2level_tapbuf_size16_8_inbus[12] = chanx_1__0__midout_84_;
assign mux_2level_tapbuf_size16_8_inbus[13] = chanx_1__0__midout_85_;
assign mux_2level_tapbuf_size16_8_inbus[14] = chanx_1__0__midout_98_;
assign mux_2level_tapbuf_size16_8_inbus[15] = chanx_1__0__midout_99_;
wire [504:511] mux_2level_tapbuf_size16_8_configbus0;
wire [504:511] mux_2level_tapbuf_size16_8_configbus1;
wire [504:511] mux_2level_tapbuf_size16_8_sram_blwl_out ;
wire [504:511] mux_2level_tapbuf_size16_8_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_8_configbus0[504:511] = sram_blwl_bl[504:511] ;
assign mux_2level_tapbuf_size16_8_configbus1[504:511] = sram_blwl_wl[504:511] ;
wire [504:511] mux_2level_tapbuf_size16_8_configbus0_b;
assign mux_2level_tapbuf_size16_8_configbus0_b[504:511] = sram_blwl_blb[504:511] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_8_ (mux_2level_tapbuf_size16_8_inbus, grid_1__1__pin_0__2__34_, mux_2level_tapbuf_size16_8_sram_blwl_out[504:511] ,
mux_2level_tapbuf_size16_8_sram_blwl_outb[504:511] );
//----- SRAM bits for MUX[8], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_504_ (mux_2level_tapbuf_size16_8_sram_blwl_out[504:504] ,mux_2level_tapbuf_size16_8_sram_blwl_out[504:504] ,mux_2level_tapbuf_size16_8_sram_blwl_outb[504:504] ,mux_2level_tapbuf_size16_8_configbus0[504:504], mux_2level_tapbuf_size16_8_configbus1[504:504] , mux_2level_tapbuf_size16_8_configbus0_b[504:504] );
sram6T_blwl sram_blwl_505_ (mux_2level_tapbuf_size16_8_sram_blwl_out[505:505] ,mux_2level_tapbuf_size16_8_sram_blwl_out[505:505] ,mux_2level_tapbuf_size16_8_sram_blwl_outb[505:505] ,mux_2level_tapbuf_size16_8_configbus0[505:505], mux_2level_tapbuf_size16_8_configbus1[505:505] , mux_2level_tapbuf_size16_8_configbus0_b[505:505] );
sram6T_blwl sram_blwl_506_ (mux_2level_tapbuf_size16_8_sram_blwl_out[506:506] ,mux_2level_tapbuf_size16_8_sram_blwl_out[506:506] ,mux_2level_tapbuf_size16_8_sram_blwl_outb[506:506] ,mux_2level_tapbuf_size16_8_configbus0[506:506], mux_2level_tapbuf_size16_8_configbus1[506:506] , mux_2level_tapbuf_size16_8_configbus0_b[506:506] );
sram6T_blwl sram_blwl_507_ (mux_2level_tapbuf_size16_8_sram_blwl_out[507:507] ,mux_2level_tapbuf_size16_8_sram_blwl_out[507:507] ,mux_2level_tapbuf_size16_8_sram_blwl_outb[507:507] ,mux_2level_tapbuf_size16_8_configbus0[507:507], mux_2level_tapbuf_size16_8_configbus1[507:507] , mux_2level_tapbuf_size16_8_configbus0_b[507:507] );
sram6T_blwl sram_blwl_508_ (mux_2level_tapbuf_size16_8_sram_blwl_out[508:508] ,mux_2level_tapbuf_size16_8_sram_blwl_out[508:508] ,mux_2level_tapbuf_size16_8_sram_blwl_outb[508:508] ,mux_2level_tapbuf_size16_8_configbus0[508:508], mux_2level_tapbuf_size16_8_configbus1[508:508] , mux_2level_tapbuf_size16_8_configbus0_b[508:508] );
sram6T_blwl sram_blwl_509_ (mux_2level_tapbuf_size16_8_sram_blwl_out[509:509] ,mux_2level_tapbuf_size16_8_sram_blwl_out[509:509] ,mux_2level_tapbuf_size16_8_sram_blwl_outb[509:509] ,mux_2level_tapbuf_size16_8_configbus0[509:509], mux_2level_tapbuf_size16_8_configbus1[509:509] , mux_2level_tapbuf_size16_8_configbus0_b[509:509] );
sram6T_blwl sram_blwl_510_ (mux_2level_tapbuf_size16_8_sram_blwl_out[510:510] ,mux_2level_tapbuf_size16_8_sram_blwl_out[510:510] ,mux_2level_tapbuf_size16_8_sram_blwl_outb[510:510] ,mux_2level_tapbuf_size16_8_configbus0[510:510], mux_2level_tapbuf_size16_8_configbus1[510:510] , mux_2level_tapbuf_size16_8_configbus0_b[510:510] );
sram6T_blwl sram_blwl_511_ (mux_2level_tapbuf_size16_8_sram_blwl_out[511:511] ,mux_2level_tapbuf_size16_8_sram_blwl_out[511:511] ,mux_2level_tapbuf_size16_8_sram_blwl_outb[511:511] ,mux_2level_tapbuf_size16_8_configbus0[511:511], mux_2level_tapbuf_size16_8_configbus1[511:511] , mux_2level_tapbuf_size16_8_configbus0_b[511:511] );
wire [0:15] mux_2level_tapbuf_size16_9_inbus;
assign mux_2level_tapbuf_size16_9_inbus[0] = chanx_1__0__midout_10_;
assign mux_2level_tapbuf_size16_9_inbus[1] = chanx_1__0__midout_11_;
assign mux_2level_tapbuf_size16_9_inbus[2] = chanx_1__0__midout_24_;
assign mux_2level_tapbuf_size16_9_inbus[3] = chanx_1__0__midout_25_;
assign mux_2level_tapbuf_size16_9_inbus[4] = chanx_1__0__midout_36_;
assign mux_2level_tapbuf_size16_9_inbus[5] = chanx_1__0__midout_37_;
assign mux_2level_tapbuf_size16_9_inbus[6] = chanx_1__0__midout_48_;
assign mux_2level_tapbuf_size16_9_inbus[7] = chanx_1__0__midout_49_;
assign mux_2level_tapbuf_size16_9_inbus[8] = chanx_1__0__midout_60_;
assign mux_2level_tapbuf_size16_9_inbus[9] = chanx_1__0__midout_61_;
assign mux_2level_tapbuf_size16_9_inbus[10] = chanx_1__0__midout_74_;
assign mux_2level_tapbuf_size16_9_inbus[11] = chanx_1__0__midout_75_;
assign mux_2level_tapbuf_size16_9_inbus[12] = chanx_1__0__midout_86_;
assign mux_2level_tapbuf_size16_9_inbus[13] = chanx_1__0__midout_87_;
assign mux_2level_tapbuf_size16_9_inbus[14] = chanx_1__0__midout_98_;
assign mux_2level_tapbuf_size16_9_inbus[15] = chanx_1__0__midout_99_;
wire [512:519] mux_2level_tapbuf_size16_9_configbus0;
wire [512:519] mux_2level_tapbuf_size16_9_configbus1;
wire [512:519] mux_2level_tapbuf_size16_9_sram_blwl_out ;
wire [512:519] mux_2level_tapbuf_size16_9_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_9_configbus0[512:519] = sram_blwl_bl[512:519] ;
assign mux_2level_tapbuf_size16_9_configbus1[512:519] = sram_blwl_wl[512:519] ;
wire [512:519] mux_2level_tapbuf_size16_9_configbus0_b;
assign mux_2level_tapbuf_size16_9_configbus0_b[512:519] = sram_blwl_blb[512:519] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_9_ (mux_2level_tapbuf_size16_9_inbus, grid_1__1__pin_0__2__38_, mux_2level_tapbuf_size16_9_sram_blwl_out[512:519] ,
mux_2level_tapbuf_size16_9_sram_blwl_outb[512:519] );
//----- SRAM bits for MUX[9], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_512_ (mux_2level_tapbuf_size16_9_sram_blwl_out[512:512] ,mux_2level_tapbuf_size16_9_sram_blwl_out[512:512] ,mux_2level_tapbuf_size16_9_sram_blwl_outb[512:512] ,mux_2level_tapbuf_size16_9_configbus0[512:512], mux_2level_tapbuf_size16_9_configbus1[512:512] , mux_2level_tapbuf_size16_9_configbus0_b[512:512] );
sram6T_blwl sram_blwl_513_ (mux_2level_tapbuf_size16_9_sram_blwl_out[513:513] ,mux_2level_tapbuf_size16_9_sram_blwl_out[513:513] ,mux_2level_tapbuf_size16_9_sram_blwl_outb[513:513] ,mux_2level_tapbuf_size16_9_configbus0[513:513], mux_2level_tapbuf_size16_9_configbus1[513:513] , mux_2level_tapbuf_size16_9_configbus0_b[513:513] );
sram6T_blwl sram_blwl_514_ (mux_2level_tapbuf_size16_9_sram_blwl_out[514:514] ,mux_2level_tapbuf_size16_9_sram_blwl_out[514:514] ,mux_2level_tapbuf_size16_9_sram_blwl_outb[514:514] ,mux_2level_tapbuf_size16_9_configbus0[514:514], mux_2level_tapbuf_size16_9_configbus1[514:514] , mux_2level_tapbuf_size16_9_configbus0_b[514:514] );
sram6T_blwl sram_blwl_515_ (mux_2level_tapbuf_size16_9_sram_blwl_out[515:515] ,mux_2level_tapbuf_size16_9_sram_blwl_out[515:515] ,mux_2level_tapbuf_size16_9_sram_blwl_outb[515:515] ,mux_2level_tapbuf_size16_9_configbus0[515:515], mux_2level_tapbuf_size16_9_configbus1[515:515] , mux_2level_tapbuf_size16_9_configbus0_b[515:515] );
sram6T_blwl sram_blwl_516_ (mux_2level_tapbuf_size16_9_sram_blwl_out[516:516] ,mux_2level_tapbuf_size16_9_sram_blwl_out[516:516] ,mux_2level_tapbuf_size16_9_sram_blwl_outb[516:516] ,mux_2level_tapbuf_size16_9_configbus0[516:516], mux_2level_tapbuf_size16_9_configbus1[516:516] , mux_2level_tapbuf_size16_9_configbus0_b[516:516] );
sram6T_blwl sram_blwl_517_ (mux_2level_tapbuf_size16_9_sram_blwl_out[517:517] ,mux_2level_tapbuf_size16_9_sram_blwl_out[517:517] ,mux_2level_tapbuf_size16_9_sram_blwl_outb[517:517] ,mux_2level_tapbuf_size16_9_configbus0[517:517], mux_2level_tapbuf_size16_9_configbus1[517:517] , mux_2level_tapbuf_size16_9_configbus0_b[517:517] );
sram6T_blwl sram_blwl_518_ (mux_2level_tapbuf_size16_9_sram_blwl_out[518:518] ,mux_2level_tapbuf_size16_9_sram_blwl_out[518:518] ,mux_2level_tapbuf_size16_9_sram_blwl_outb[518:518] ,mux_2level_tapbuf_size16_9_configbus0[518:518], mux_2level_tapbuf_size16_9_configbus1[518:518] , mux_2level_tapbuf_size16_9_configbus0_b[518:518] );
sram6T_blwl sram_blwl_519_ (mux_2level_tapbuf_size16_9_sram_blwl_out[519:519] ,mux_2level_tapbuf_size16_9_sram_blwl_out[519:519] ,mux_2level_tapbuf_size16_9_sram_blwl_outb[519:519] ,mux_2level_tapbuf_size16_9_configbus0[519:519], mux_2level_tapbuf_size16_9_configbus1[519:519] , mux_2level_tapbuf_size16_9_configbus0_b[519:519] );
wire [0:15] mux_2level_tapbuf_size16_10_inbus;
assign mux_2level_tapbuf_size16_10_inbus[0] = chanx_1__0__midout_0_;
assign mux_2level_tapbuf_size16_10_inbus[1] = chanx_1__0__midout_1_;
assign mux_2level_tapbuf_size16_10_inbus[2] = chanx_1__0__midout_12_;
assign mux_2level_tapbuf_size16_10_inbus[3] = chanx_1__0__midout_13_;
assign mux_2level_tapbuf_size16_10_inbus[4] = chanx_1__0__midout_24_;
assign mux_2level_tapbuf_size16_10_inbus[5] = chanx_1__0__midout_25_;
assign mux_2level_tapbuf_size16_10_inbus[6] = chanx_1__0__midout_36_;
assign mux_2level_tapbuf_size16_10_inbus[7] = chanx_1__0__midout_37_;
assign mux_2level_tapbuf_size16_10_inbus[8] = chanx_1__0__midout_50_;
assign mux_2level_tapbuf_size16_10_inbus[9] = chanx_1__0__midout_51_;
assign mux_2level_tapbuf_size16_10_inbus[10] = chanx_1__0__midout_62_;
assign mux_2level_tapbuf_size16_10_inbus[11] = chanx_1__0__midout_63_;
assign mux_2level_tapbuf_size16_10_inbus[12] = chanx_1__0__midout_74_;
assign mux_2level_tapbuf_size16_10_inbus[13] = chanx_1__0__midout_75_;
assign mux_2level_tapbuf_size16_10_inbus[14] = chanx_1__0__midout_86_;
assign mux_2level_tapbuf_size16_10_inbus[15] = chanx_1__0__midout_87_;
wire [520:527] mux_2level_tapbuf_size16_10_configbus0;
wire [520:527] mux_2level_tapbuf_size16_10_configbus1;
wire [520:527] mux_2level_tapbuf_size16_10_sram_blwl_out ;
wire [520:527] mux_2level_tapbuf_size16_10_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_10_configbus0[520:527] = sram_blwl_bl[520:527] ;
assign mux_2level_tapbuf_size16_10_configbus1[520:527] = sram_blwl_wl[520:527] ;
wire [520:527] mux_2level_tapbuf_size16_10_configbus0_b;
assign mux_2level_tapbuf_size16_10_configbus0_b[520:527] = sram_blwl_blb[520:527] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_10_ (mux_2level_tapbuf_size16_10_inbus, grid_1__0__pin_0__0__0_, mux_2level_tapbuf_size16_10_sram_blwl_out[520:527] ,
mux_2level_tapbuf_size16_10_sram_blwl_outb[520:527] );
//----- SRAM bits for MUX[10], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_520_ (mux_2level_tapbuf_size16_10_sram_blwl_out[520:520] ,mux_2level_tapbuf_size16_10_sram_blwl_out[520:520] ,mux_2level_tapbuf_size16_10_sram_blwl_outb[520:520] ,mux_2level_tapbuf_size16_10_configbus0[520:520], mux_2level_tapbuf_size16_10_configbus1[520:520] , mux_2level_tapbuf_size16_10_configbus0_b[520:520] );
sram6T_blwl sram_blwl_521_ (mux_2level_tapbuf_size16_10_sram_blwl_out[521:521] ,mux_2level_tapbuf_size16_10_sram_blwl_out[521:521] ,mux_2level_tapbuf_size16_10_sram_blwl_outb[521:521] ,mux_2level_tapbuf_size16_10_configbus0[521:521], mux_2level_tapbuf_size16_10_configbus1[521:521] , mux_2level_tapbuf_size16_10_configbus0_b[521:521] );
sram6T_blwl sram_blwl_522_ (mux_2level_tapbuf_size16_10_sram_blwl_out[522:522] ,mux_2level_tapbuf_size16_10_sram_blwl_out[522:522] ,mux_2level_tapbuf_size16_10_sram_blwl_outb[522:522] ,mux_2level_tapbuf_size16_10_configbus0[522:522], mux_2level_tapbuf_size16_10_configbus1[522:522] , mux_2level_tapbuf_size16_10_configbus0_b[522:522] );
sram6T_blwl sram_blwl_523_ (mux_2level_tapbuf_size16_10_sram_blwl_out[523:523] ,mux_2level_tapbuf_size16_10_sram_blwl_out[523:523] ,mux_2level_tapbuf_size16_10_sram_blwl_outb[523:523] ,mux_2level_tapbuf_size16_10_configbus0[523:523], mux_2level_tapbuf_size16_10_configbus1[523:523] , mux_2level_tapbuf_size16_10_configbus0_b[523:523] );
sram6T_blwl sram_blwl_524_ (mux_2level_tapbuf_size16_10_sram_blwl_out[524:524] ,mux_2level_tapbuf_size16_10_sram_blwl_out[524:524] ,mux_2level_tapbuf_size16_10_sram_blwl_outb[524:524] ,mux_2level_tapbuf_size16_10_configbus0[524:524], mux_2level_tapbuf_size16_10_configbus1[524:524] , mux_2level_tapbuf_size16_10_configbus0_b[524:524] );
sram6T_blwl sram_blwl_525_ (mux_2level_tapbuf_size16_10_sram_blwl_out[525:525] ,mux_2level_tapbuf_size16_10_sram_blwl_out[525:525] ,mux_2level_tapbuf_size16_10_sram_blwl_outb[525:525] ,mux_2level_tapbuf_size16_10_configbus0[525:525], mux_2level_tapbuf_size16_10_configbus1[525:525] , mux_2level_tapbuf_size16_10_configbus0_b[525:525] );
sram6T_blwl sram_blwl_526_ (mux_2level_tapbuf_size16_10_sram_blwl_out[526:526] ,mux_2level_tapbuf_size16_10_sram_blwl_out[526:526] ,mux_2level_tapbuf_size16_10_sram_blwl_outb[526:526] ,mux_2level_tapbuf_size16_10_configbus0[526:526], mux_2level_tapbuf_size16_10_configbus1[526:526] , mux_2level_tapbuf_size16_10_configbus0_b[526:526] );
sram6T_blwl sram_blwl_527_ (mux_2level_tapbuf_size16_10_sram_blwl_out[527:527] ,mux_2level_tapbuf_size16_10_sram_blwl_out[527:527] ,mux_2level_tapbuf_size16_10_sram_blwl_outb[527:527] ,mux_2level_tapbuf_size16_10_configbus0[527:527], mux_2level_tapbuf_size16_10_configbus1[527:527] , mux_2level_tapbuf_size16_10_configbus0_b[527:527] );
wire [0:15] mux_2level_tapbuf_size16_11_inbus;
assign mux_2level_tapbuf_size16_11_inbus[0] = chanx_1__0__midout_0_;
assign mux_2level_tapbuf_size16_11_inbus[1] = chanx_1__0__midout_1_;
assign mux_2level_tapbuf_size16_11_inbus[2] = chanx_1__0__midout_14_;
assign mux_2level_tapbuf_size16_11_inbus[3] = chanx_1__0__midout_15_;
assign mux_2level_tapbuf_size16_11_inbus[4] = chanx_1__0__midout_26_;
assign mux_2level_tapbuf_size16_11_inbus[5] = chanx_1__0__midout_27_;
assign mux_2level_tapbuf_size16_11_inbus[6] = chanx_1__0__midout_38_;
assign mux_2level_tapbuf_size16_11_inbus[7] = chanx_1__0__midout_39_;
assign mux_2level_tapbuf_size16_11_inbus[8] = chanx_1__0__midout_50_;
assign mux_2level_tapbuf_size16_11_inbus[9] = chanx_1__0__midout_51_;
assign mux_2level_tapbuf_size16_11_inbus[10] = chanx_1__0__midout_64_;
assign mux_2level_tapbuf_size16_11_inbus[11] = chanx_1__0__midout_65_;
assign mux_2level_tapbuf_size16_11_inbus[12] = chanx_1__0__midout_76_;
assign mux_2level_tapbuf_size16_11_inbus[13] = chanx_1__0__midout_77_;
assign mux_2level_tapbuf_size16_11_inbus[14] = chanx_1__0__midout_88_;
assign mux_2level_tapbuf_size16_11_inbus[15] = chanx_1__0__midout_89_;
wire [528:535] mux_2level_tapbuf_size16_11_configbus0;
wire [528:535] mux_2level_tapbuf_size16_11_configbus1;
wire [528:535] mux_2level_tapbuf_size16_11_sram_blwl_out ;
wire [528:535] mux_2level_tapbuf_size16_11_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_11_configbus0[528:535] = sram_blwl_bl[528:535] ;
assign mux_2level_tapbuf_size16_11_configbus1[528:535] = sram_blwl_wl[528:535] ;
wire [528:535] mux_2level_tapbuf_size16_11_configbus0_b;
assign mux_2level_tapbuf_size16_11_configbus0_b[528:535] = sram_blwl_blb[528:535] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_11_ (mux_2level_tapbuf_size16_11_inbus, grid_1__0__pin_0__0__2_, mux_2level_tapbuf_size16_11_sram_blwl_out[528:535] ,
mux_2level_tapbuf_size16_11_sram_blwl_outb[528:535] );
//----- SRAM bits for MUX[11], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_528_ (mux_2level_tapbuf_size16_11_sram_blwl_out[528:528] ,mux_2level_tapbuf_size16_11_sram_blwl_out[528:528] ,mux_2level_tapbuf_size16_11_sram_blwl_outb[528:528] ,mux_2level_tapbuf_size16_11_configbus0[528:528], mux_2level_tapbuf_size16_11_configbus1[528:528] , mux_2level_tapbuf_size16_11_configbus0_b[528:528] );
sram6T_blwl sram_blwl_529_ (mux_2level_tapbuf_size16_11_sram_blwl_out[529:529] ,mux_2level_tapbuf_size16_11_sram_blwl_out[529:529] ,mux_2level_tapbuf_size16_11_sram_blwl_outb[529:529] ,mux_2level_tapbuf_size16_11_configbus0[529:529], mux_2level_tapbuf_size16_11_configbus1[529:529] , mux_2level_tapbuf_size16_11_configbus0_b[529:529] );
sram6T_blwl sram_blwl_530_ (mux_2level_tapbuf_size16_11_sram_blwl_out[530:530] ,mux_2level_tapbuf_size16_11_sram_blwl_out[530:530] ,mux_2level_tapbuf_size16_11_sram_blwl_outb[530:530] ,mux_2level_tapbuf_size16_11_configbus0[530:530], mux_2level_tapbuf_size16_11_configbus1[530:530] , mux_2level_tapbuf_size16_11_configbus0_b[530:530] );
sram6T_blwl sram_blwl_531_ (mux_2level_tapbuf_size16_11_sram_blwl_out[531:531] ,mux_2level_tapbuf_size16_11_sram_blwl_out[531:531] ,mux_2level_tapbuf_size16_11_sram_blwl_outb[531:531] ,mux_2level_tapbuf_size16_11_configbus0[531:531], mux_2level_tapbuf_size16_11_configbus1[531:531] , mux_2level_tapbuf_size16_11_configbus0_b[531:531] );
sram6T_blwl sram_blwl_532_ (mux_2level_tapbuf_size16_11_sram_blwl_out[532:532] ,mux_2level_tapbuf_size16_11_sram_blwl_out[532:532] ,mux_2level_tapbuf_size16_11_sram_blwl_outb[532:532] ,mux_2level_tapbuf_size16_11_configbus0[532:532], mux_2level_tapbuf_size16_11_configbus1[532:532] , mux_2level_tapbuf_size16_11_configbus0_b[532:532] );
sram6T_blwl sram_blwl_533_ (mux_2level_tapbuf_size16_11_sram_blwl_out[533:533] ,mux_2level_tapbuf_size16_11_sram_blwl_out[533:533] ,mux_2level_tapbuf_size16_11_sram_blwl_outb[533:533] ,mux_2level_tapbuf_size16_11_configbus0[533:533], mux_2level_tapbuf_size16_11_configbus1[533:533] , mux_2level_tapbuf_size16_11_configbus0_b[533:533] );
sram6T_blwl sram_blwl_534_ (mux_2level_tapbuf_size16_11_sram_blwl_out[534:534] ,mux_2level_tapbuf_size16_11_sram_blwl_out[534:534] ,mux_2level_tapbuf_size16_11_sram_blwl_outb[534:534] ,mux_2level_tapbuf_size16_11_configbus0[534:534], mux_2level_tapbuf_size16_11_configbus1[534:534] , mux_2level_tapbuf_size16_11_configbus0_b[534:534] );
sram6T_blwl sram_blwl_535_ (mux_2level_tapbuf_size16_11_sram_blwl_out[535:535] ,mux_2level_tapbuf_size16_11_sram_blwl_out[535:535] ,mux_2level_tapbuf_size16_11_sram_blwl_outb[535:535] ,mux_2level_tapbuf_size16_11_configbus0[535:535], mux_2level_tapbuf_size16_11_configbus1[535:535] , mux_2level_tapbuf_size16_11_configbus0_b[535:535] );
wire [0:15] mux_2level_tapbuf_size16_12_inbus;
assign mux_2level_tapbuf_size16_12_inbus[0] = chanx_1__0__midout_2_;
assign mux_2level_tapbuf_size16_12_inbus[1] = chanx_1__0__midout_3_;
assign mux_2level_tapbuf_size16_12_inbus[2] = chanx_1__0__midout_14_;
assign mux_2level_tapbuf_size16_12_inbus[3] = chanx_1__0__midout_15_;
assign mux_2level_tapbuf_size16_12_inbus[4] = chanx_1__0__midout_28_;
assign mux_2level_tapbuf_size16_12_inbus[5] = chanx_1__0__midout_29_;
assign mux_2level_tapbuf_size16_12_inbus[6] = chanx_1__0__midout_40_;
assign mux_2level_tapbuf_size16_12_inbus[7] = chanx_1__0__midout_41_;
assign mux_2level_tapbuf_size16_12_inbus[8] = chanx_1__0__midout_52_;
assign mux_2level_tapbuf_size16_12_inbus[9] = chanx_1__0__midout_53_;
assign mux_2level_tapbuf_size16_12_inbus[10] = chanx_1__0__midout_64_;
assign mux_2level_tapbuf_size16_12_inbus[11] = chanx_1__0__midout_65_;
assign mux_2level_tapbuf_size16_12_inbus[12] = chanx_1__0__midout_78_;
assign mux_2level_tapbuf_size16_12_inbus[13] = chanx_1__0__midout_79_;
assign mux_2level_tapbuf_size16_12_inbus[14] = chanx_1__0__midout_90_;
assign mux_2level_tapbuf_size16_12_inbus[15] = chanx_1__0__midout_91_;
wire [536:543] mux_2level_tapbuf_size16_12_configbus0;
wire [536:543] mux_2level_tapbuf_size16_12_configbus1;
wire [536:543] mux_2level_tapbuf_size16_12_sram_blwl_out ;
wire [536:543] mux_2level_tapbuf_size16_12_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_12_configbus0[536:543] = sram_blwl_bl[536:543] ;
assign mux_2level_tapbuf_size16_12_configbus1[536:543] = sram_blwl_wl[536:543] ;
wire [536:543] mux_2level_tapbuf_size16_12_configbus0_b;
assign mux_2level_tapbuf_size16_12_configbus0_b[536:543] = sram_blwl_blb[536:543] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_12_ (mux_2level_tapbuf_size16_12_inbus, grid_1__0__pin_0__0__4_, mux_2level_tapbuf_size16_12_sram_blwl_out[536:543] ,
mux_2level_tapbuf_size16_12_sram_blwl_outb[536:543] );
//----- SRAM bits for MUX[12], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_536_ (mux_2level_tapbuf_size16_12_sram_blwl_out[536:536] ,mux_2level_tapbuf_size16_12_sram_blwl_out[536:536] ,mux_2level_tapbuf_size16_12_sram_blwl_outb[536:536] ,mux_2level_tapbuf_size16_12_configbus0[536:536], mux_2level_tapbuf_size16_12_configbus1[536:536] , mux_2level_tapbuf_size16_12_configbus0_b[536:536] );
sram6T_blwl sram_blwl_537_ (mux_2level_tapbuf_size16_12_sram_blwl_out[537:537] ,mux_2level_tapbuf_size16_12_sram_blwl_out[537:537] ,mux_2level_tapbuf_size16_12_sram_blwl_outb[537:537] ,mux_2level_tapbuf_size16_12_configbus0[537:537], mux_2level_tapbuf_size16_12_configbus1[537:537] , mux_2level_tapbuf_size16_12_configbus0_b[537:537] );
sram6T_blwl sram_blwl_538_ (mux_2level_tapbuf_size16_12_sram_blwl_out[538:538] ,mux_2level_tapbuf_size16_12_sram_blwl_out[538:538] ,mux_2level_tapbuf_size16_12_sram_blwl_outb[538:538] ,mux_2level_tapbuf_size16_12_configbus0[538:538], mux_2level_tapbuf_size16_12_configbus1[538:538] , mux_2level_tapbuf_size16_12_configbus0_b[538:538] );
sram6T_blwl sram_blwl_539_ (mux_2level_tapbuf_size16_12_sram_blwl_out[539:539] ,mux_2level_tapbuf_size16_12_sram_blwl_out[539:539] ,mux_2level_tapbuf_size16_12_sram_blwl_outb[539:539] ,mux_2level_tapbuf_size16_12_configbus0[539:539], mux_2level_tapbuf_size16_12_configbus1[539:539] , mux_2level_tapbuf_size16_12_configbus0_b[539:539] );
sram6T_blwl sram_blwl_540_ (mux_2level_tapbuf_size16_12_sram_blwl_out[540:540] ,mux_2level_tapbuf_size16_12_sram_blwl_out[540:540] ,mux_2level_tapbuf_size16_12_sram_blwl_outb[540:540] ,mux_2level_tapbuf_size16_12_configbus0[540:540], mux_2level_tapbuf_size16_12_configbus1[540:540] , mux_2level_tapbuf_size16_12_configbus0_b[540:540] );
sram6T_blwl sram_blwl_541_ (mux_2level_tapbuf_size16_12_sram_blwl_out[541:541] ,mux_2level_tapbuf_size16_12_sram_blwl_out[541:541] ,mux_2level_tapbuf_size16_12_sram_blwl_outb[541:541] ,mux_2level_tapbuf_size16_12_configbus0[541:541], mux_2level_tapbuf_size16_12_configbus1[541:541] , mux_2level_tapbuf_size16_12_configbus0_b[541:541] );
sram6T_blwl sram_blwl_542_ (mux_2level_tapbuf_size16_12_sram_blwl_out[542:542] ,mux_2level_tapbuf_size16_12_sram_blwl_out[542:542] ,mux_2level_tapbuf_size16_12_sram_blwl_outb[542:542] ,mux_2level_tapbuf_size16_12_configbus0[542:542], mux_2level_tapbuf_size16_12_configbus1[542:542] , mux_2level_tapbuf_size16_12_configbus0_b[542:542] );
sram6T_blwl sram_blwl_543_ (mux_2level_tapbuf_size16_12_sram_blwl_out[543:543] ,mux_2level_tapbuf_size16_12_sram_blwl_out[543:543] ,mux_2level_tapbuf_size16_12_sram_blwl_outb[543:543] ,mux_2level_tapbuf_size16_12_configbus0[543:543], mux_2level_tapbuf_size16_12_configbus1[543:543] , mux_2level_tapbuf_size16_12_configbus0_b[543:543] );
wire [0:15] mux_2level_tapbuf_size16_13_inbus;
assign mux_2level_tapbuf_size16_13_inbus[0] = chanx_1__0__midout_4_;
assign mux_2level_tapbuf_size16_13_inbus[1] = chanx_1__0__midout_5_;
assign mux_2level_tapbuf_size16_13_inbus[2] = chanx_1__0__midout_16_;
assign mux_2level_tapbuf_size16_13_inbus[3] = chanx_1__0__midout_17_;
assign mux_2level_tapbuf_size16_13_inbus[4] = chanx_1__0__midout_28_;
assign mux_2level_tapbuf_size16_13_inbus[5] = chanx_1__0__midout_29_;
assign mux_2level_tapbuf_size16_13_inbus[6] = chanx_1__0__midout_42_;
assign mux_2level_tapbuf_size16_13_inbus[7] = chanx_1__0__midout_43_;
assign mux_2level_tapbuf_size16_13_inbus[8] = chanx_1__0__midout_54_;
assign mux_2level_tapbuf_size16_13_inbus[9] = chanx_1__0__midout_55_;
assign mux_2level_tapbuf_size16_13_inbus[10] = chanx_1__0__midout_66_;
assign mux_2level_tapbuf_size16_13_inbus[11] = chanx_1__0__midout_67_;
assign mux_2level_tapbuf_size16_13_inbus[12] = chanx_1__0__midout_78_;
assign mux_2level_tapbuf_size16_13_inbus[13] = chanx_1__0__midout_79_;
assign mux_2level_tapbuf_size16_13_inbus[14] = chanx_1__0__midout_92_;
assign mux_2level_tapbuf_size16_13_inbus[15] = chanx_1__0__midout_93_;
wire [544:551] mux_2level_tapbuf_size16_13_configbus0;
wire [544:551] mux_2level_tapbuf_size16_13_configbus1;
wire [544:551] mux_2level_tapbuf_size16_13_sram_blwl_out ;
wire [544:551] mux_2level_tapbuf_size16_13_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_13_configbus0[544:551] = sram_blwl_bl[544:551] ;
assign mux_2level_tapbuf_size16_13_configbus1[544:551] = sram_blwl_wl[544:551] ;
wire [544:551] mux_2level_tapbuf_size16_13_configbus0_b;
assign mux_2level_tapbuf_size16_13_configbus0_b[544:551] = sram_blwl_blb[544:551] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_13_ (mux_2level_tapbuf_size16_13_inbus, grid_1__0__pin_0__0__6_, mux_2level_tapbuf_size16_13_sram_blwl_out[544:551] ,
mux_2level_tapbuf_size16_13_sram_blwl_outb[544:551] );
//----- SRAM bits for MUX[13], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_544_ (mux_2level_tapbuf_size16_13_sram_blwl_out[544:544] ,mux_2level_tapbuf_size16_13_sram_blwl_out[544:544] ,mux_2level_tapbuf_size16_13_sram_blwl_outb[544:544] ,mux_2level_tapbuf_size16_13_configbus0[544:544], mux_2level_tapbuf_size16_13_configbus1[544:544] , mux_2level_tapbuf_size16_13_configbus0_b[544:544] );
sram6T_blwl sram_blwl_545_ (mux_2level_tapbuf_size16_13_sram_blwl_out[545:545] ,mux_2level_tapbuf_size16_13_sram_blwl_out[545:545] ,mux_2level_tapbuf_size16_13_sram_blwl_outb[545:545] ,mux_2level_tapbuf_size16_13_configbus0[545:545], mux_2level_tapbuf_size16_13_configbus1[545:545] , mux_2level_tapbuf_size16_13_configbus0_b[545:545] );
sram6T_blwl sram_blwl_546_ (mux_2level_tapbuf_size16_13_sram_blwl_out[546:546] ,mux_2level_tapbuf_size16_13_sram_blwl_out[546:546] ,mux_2level_tapbuf_size16_13_sram_blwl_outb[546:546] ,mux_2level_tapbuf_size16_13_configbus0[546:546], mux_2level_tapbuf_size16_13_configbus1[546:546] , mux_2level_tapbuf_size16_13_configbus0_b[546:546] );
sram6T_blwl sram_blwl_547_ (mux_2level_tapbuf_size16_13_sram_blwl_out[547:547] ,mux_2level_tapbuf_size16_13_sram_blwl_out[547:547] ,mux_2level_tapbuf_size16_13_sram_blwl_outb[547:547] ,mux_2level_tapbuf_size16_13_configbus0[547:547], mux_2level_tapbuf_size16_13_configbus1[547:547] , mux_2level_tapbuf_size16_13_configbus0_b[547:547] );
sram6T_blwl sram_blwl_548_ (mux_2level_tapbuf_size16_13_sram_blwl_out[548:548] ,mux_2level_tapbuf_size16_13_sram_blwl_out[548:548] ,mux_2level_tapbuf_size16_13_sram_blwl_outb[548:548] ,mux_2level_tapbuf_size16_13_configbus0[548:548], mux_2level_tapbuf_size16_13_configbus1[548:548] , mux_2level_tapbuf_size16_13_configbus0_b[548:548] );
sram6T_blwl sram_blwl_549_ (mux_2level_tapbuf_size16_13_sram_blwl_out[549:549] ,mux_2level_tapbuf_size16_13_sram_blwl_out[549:549] ,mux_2level_tapbuf_size16_13_sram_blwl_outb[549:549] ,mux_2level_tapbuf_size16_13_configbus0[549:549], mux_2level_tapbuf_size16_13_configbus1[549:549] , mux_2level_tapbuf_size16_13_configbus0_b[549:549] );
sram6T_blwl sram_blwl_550_ (mux_2level_tapbuf_size16_13_sram_blwl_out[550:550] ,mux_2level_tapbuf_size16_13_sram_blwl_out[550:550] ,mux_2level_tapbuf_size16_13_sram_blwl_outb[550:550] ,mux_2level_tapbuf_size16_13_configbus0[550:550], mux_2level_tapbuf_size16_13_configbus1[550:550] , mux_2level_tapbuf_size16_13_configbus0_b[550:550] );
sram6T_blwl sram_blwl_551_ (mux_2level_tapbuf_size16_13_sram_blwl_out[551:551] ,mux_2level_tapbuf_size16_13_sram_blwl_out[551:551] ,mux_2level_tapbuf_size16_13_sram_blwl_outb[551:551] ,mux_2level_tapbuf_size16_13_configbus0[551:551], mux_2level_tapbuf_size16_13_configbus1[551:551] , mux_2level_tapbuf_size16_13_configbus0_b[551:551] );
wire [0:15] mux_2level_tapbuf_size16_14_inbus;
assign mux_2level_tapbuf_size16_14_inbus[0] = chanx_1__0__midout_6_;
assign mux_2level_tapbuf_size16_14_inbus[1] = chanx_1__0__midout_7_;
assign mux_2level_tapbuf_size16_14_inbus[2] = chanx_1__0__midout_18_;
assign mux_2level_tapbuf_size16_14_inbus[3] = chanx_1__0__midout_19_;
assign mux_2level_tapbuf_size16_14_inbus[4] = chanx_1__0__midout_30_;
assign mux_2level_tapbuf_size16_14_inbus[5] = chanx_1__0__midout_31_;
assign mux_2level_tapbuf_size16_14_inbus[6] = chanx_1__0__midout_42_;
assign mux_2level_tapbuf_size16_14_inbus[7] = chanx_1__0__midout_43_;
assign mux_2level_tapbuf_size16_14_inbus[8] = chanx_1__0__midout_56_;
assign mux_2level_tapbuf_size16_14_inbus[9] = chanx_1__0__midout_57_;
assign mux_2level_tapbuf_size16_14_inbus[10] = chanx_1__0__midout_68_;
assign mux_2level_tapbuf_size16_14_inbus[11] = chanx_1__0__midout_69_;
assign mux_2level_tapbuf_size16_14_inbus[12] = chanx_1__0__midout_80_;
assign mux_2level_tapbuf_size16_14_inbus[13] = chanx_1__0__midout_81_;
assign mux_2level_tapbuf_size16_14_inbus[14] = chanx_1__0__midout_92_;
assign mux_2level_tapbuf_size16_14_inbus[15] = chanx_1__0__midout_93_;
wire [552:559] mux_2level_tapbuf_size16_14_configbus0;
wire [552:559] mux_2level_tapbuf_size16_14_configbus1;
wire [552:559] mux_2level_tapbuf_size16_14_sram_blwl_out ;
wire [552:559] mux_2level_tapbuf_size16_14_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_14_configbus0[552:559] = sram_blwl_bl[552:559] ;
assign mux_2level_tapbuf_size16_14_configbus1[552:559] = sram_blwl_wl[552:559] ;
wire [552:559] mux_2level_tapbuf_size16_14_configbus0_b;
assign mux_2level_tapbuf_size16_14_configbus0_b[552:559] = sram_blwl_blb[552:559] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_14_ (mux_2level_tapbuf_size16_14_inbus, grid_1__0__pin_0__0__8_, mux_2level_tapbuf_size16_14_sram_blwl_out[552:559] ,
mux_2level_tapbuf_size16_14_sram_blwl_outb[552:559] );
//----- SRAM bits for MUX[14], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_552_ (mux_2level_tapbuf_size16_14_sram_blwl_out[552:552] ,mux_2level_tapbuf_size16_14_sram_blwl_out[552:552] ,mux_2level_tapbuf_size16_14_sram_blwl_outb[552:552] ,mux_2level_tapbuf_size16_14_configbus0[552:552], mux_2level_tapbuf_size16_14_configbus1[552:552] , mux_2level_tapbuf_size16_14_configbus0_b[552:552] );
sram6T_blwl sram_blwl_553_ (mux_2level_tapbuf_size16_14_sram_blwl_out[553:553] ,mux_2level_tapbuf_size16_14_sram_blwl_out[553:553] ,mux_2level_tapbuf_size16_14_sram_blwl_outb[553:553] ,mux_2level_tapbuf_size16_14_configbus0[553:553], mux_2level_tapbuf_size16_14_configbus1[553:553] , mux_2level_tapbuf_size16_14_configbus0_b[553:553] );
sram6T_blwl sram_blwl_554_ (mux_2level_tapbuf_size16_14_sram_blwl_out[554:554] ,mux_2level_tapbuf_size16_14_sram_blwl_out[554:554] ,mux_2level_tapbuf_size16_14_sram_blwl_outb[554:554] ,mux_2level_tapbuf_size16_14_configbus0[554:554], mux_2level_tapbuf_size16_14_configbus1[554:554] , mux_2level_tapbuf_size16_14_configbus0_b[554:554] );
sram6T_blwl sram_blwl_555_ (mux_2level_tapbuf_size16_14_sram_blwl_out[555:555] ,mux_2level_tapbuf_size16_14_sram_blwl_out[555:555] ,mux_2level_tapbuf_size16_14_sram_blwl_outb[555:555] ,mux_2level_tapbuf_size16_14_configbus0[555:555], mux_2level_tapbuf_size16_14_configbus1[555:555] , mux_2level_tapbuf_size16_14_configbus0_b[555:555] );
sram6T_blwl sram_blwl_556_ (mux_2level_tapbuf_size16_14_sram_blwl_out[556:556] ,mux_2level_tapbuf_size16_14_sram_blwl_out[556:556] ,mux_2level_tapbuf_size16_14_sram_blwl_outb[556:556] ,mux_2level_tapbuf_size16_14_configbus0[556:556], mux_2level_tapbuf_size16_14_configbus1[556:556] , mux_2level_tapbuf_size16_14_configbus0_b[556:556] );
sram6T_blwl sram_blwl_557_ (mux_2level_tapbuf_size16_14_sram_blwl_out[557:557] ,mux_2level_tapbuf_size16_14_sram_blwl_out[557:557] ,mux_2level_tapbuf_size16_14_sram_blwl_outb[557:557] ,mux_2level_tapbuf_size16_14_configbus0[557:557], mux_2level_tapbuf_size16_14_configbus1[557:557] , mux_2level_tapbuf_size16_14_configbus0_b[557:557] );
sram6T_blwl sram_blwl_558_ (mux_2level_tapbuf_size16_14_sram_blwl_out[558:558] ,mux_2level_tapbuf_size16_14_sram_blwl_out[558:558] ,mux_2level_tapbuf_size16_14_sram_blwl_outb[558:558] ,mux_2level_tapbuf_size16_14_configbus0[558:558], mux_2level_tapbuf_size16_14_configbus1[558:558] , mux_2level_tapbuf_size16_14_configbus0_b[558:558] );
sram6T_blwl sram_blwl_559_ (mux_2level_tapbuf_size16_14_sram_blwl_out[559:559] ,mux_2level_tapbuf_size16_14_sram_blwl_out[559:559] ,mux_2level_tapbuf_size16_14_sram_blwl_outb[559:559] ,mux_2level_tapbuf_size16_14_configbus0[559:559], mux_2level_tapbuf_size16_14_configbus1[559:559] , mux_2level_tapbuf_size16_14_configbus0_b[559:559] );
wire [0:15] mux_2level_tapbuf_size16_15_inbus;
assign mux_2level_tapbuf_size16_15_inbus[0] = chanx_1__0__midout_6_;
assign mux_2level_tapbuf_size16_15_inbus[1] = chanx_1__0__midout_7_;
assign mux_2level_tapbuf_size16_15_inbus[2] = chanx_1__0__midout_20_;
assign mux_2level_tapbuf_size16_15_inbus[3] = chanx_1__0__midout_21_;
assign mux_2level_tapbuf_size16_15_inbus[4] = chanx_1__0__midout_32_;
assign mux_2level_tapbuf_size16_15_inbus[5] = chanx_1__0__midout_33_;
assign mux_2level_tapbuf_size16_15_inbus[6] = chanx_1__0__midout_44_;
assign mux_2level_tapbuf_size16_15_inbus[7] = chanx_1__0__midout_45_;
assign mux_2level_tapbuf_size16_15_inbus[8] = chanx_1__0__midout_56_;
assign mux_2level_tapbuf_size16_15_inbus[9] = chanx_1__0__midout_57_;
assign mux_2level_tapbuf_size16_15_inbus[10] = chanx_1__0__midout_70_;
assign mux_2level_tapbuf_size16_15_inbus[11] = chanx_1__0__midout_71_;
assign mux_2level_tapbuf_size16_15_inbus[12] = chanx_1__0__midout_82_;
assign mux_2level_tapbuf_size16_15_inbus[13] = chanx_1__0__midout_83_;
assign mux_2level_tapbuf_size16_15_inbus[14] = chanx_1__0__midout_94_;
assign mux_2level_tapbuf_size16_15_inbus[15] = chanx_1__0__midout_95_;
wire [560:567] mux_2level_tapbuf_size16_15_configbus0;
wire [560:567] mux_2level_tapbuf_size16_15_configbus1;
wire [560:567] mux_2level_tapbuf_size16_15_sram_blwl_out ;
wire [560:567] mux_2level_tapbuf_size16_15_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_15_configbus0[560:567] = sram_blwl_bl[560:567] ;
assign mux_2level_tapbuf_size16_15_configbus1[560:567] = sram_blwl_wl[560:567] ;
wire [560:567] mux_2level_tapbuf_size16_15_configbus0_b;
assign mux_2level_tapbuf_size16_15_configbus0_b[560:567] = sram_blwl_blb[560:567] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_15_ (mux_2level_tapbuf_size16_15_inbus, grid_1__0__pin_0__0__10_, mux_2level_tapbuf_size16_15_sram_blwl_out[560:567] ,
mux_2level_tapbuf_size16_15_sram_blwl_outb[560:567] );
//----- SRAM bits for MUX[15], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_560_ (mux_2level_tapbuf_size16_15_sram_blwl_out[560:560] ,mux_2level_tapbuf_size16_15_sram_blwl_out[560:560] ,mux_2level_tapbuf_size16_15_sram_blwl_outb[560:560] ,mux_2level_tapbuf_size16_15_configbus0[560:560], mux_2level_tapbuf_size16_15_configbus1[560:560] , mux_2level_tapbuf_size16_15_configbus0_b[560:560] );
sram6T_blwl sram_blwl_561_ (mux_2level_tapbuf_size16_15_sram_blwl_out[561:561] ,mux_2level_tapbuf_size16_15_sram_blwl_out[561:561] ,mux_2level_tapbuf_size16_15_sram_blwl_outb[561:561] ,mux_2level_tapbuf_size16_15_configbus0[561:561], mux_2level_tapbuf_size16_15_configbus1[561:561] , mux_2level_tapbuf_size16_15_configbus0_b[561:561] );
sram6T_blwl sram_blwl_562_ (mux_2level_tapbuf_size16_15_sram_blwl_out[562:562] ,mux_2level_tapbuf_size16_15_sram_blwl_out[562:562] ,mux_2level_tapbuf_size16_15_sram_blwl_outb[562:562] ,mux_2level_tapbuf_size16_15_configbus0[562:562], mux_2level_tapbuf_size16_15_configbus1[562:562] , mux_2level_tapbuf_size16_15_configbus0_b[562:562] );
sram6T_blwl sram_blwl_563_ (mux_2level_tapbuf_size16_15_sram_blwl_out[563:563] ,mux_2level_tapbuf_size16_15_sram_blwl_out[563:563] ,mux_2level_tapbuf_size16_15_sram_blwl_outb[563:563] ,mux_2level_tapbuf_size16_15_configbus0[563:563], mux_2level_tapbuf_size16_15_configbus1[563:563] , mux_2level_tapbuf_size16_15_configbus0_b[563:563] );
sram6T_blwl sram_blwl_564_ (mux_2level_tapbuf_size16_15_sram_blwl_out[564:564] ,mux_2level_tapbuf_size16_15_sram_blwl_out[564:564] ,mux_2level_tapbuf_size16_15_sram_blwl_outb[564:564] ,mux_2level_tapbuf_size16_15_configbus0[564:564], mux_2level_tapbuf_size16_15_configbus1[564:564] , mux_2level_tapbuf_size16_15_configbus0_b[564:564] );
sram6T_blwl sram_blwl_565_ (mux_2level_tapbuf_size16_15_sram_blwl_out[565:565] ,mux_2level_tapbuf_size16_15_sram_blwl_out[565:565] ,mux_2level_tapbuf_size16_15_sram_blwl_outb[565:565] ,mux_2level_tapbuf_size16_15_configbus0[565:565], mux_2level_tapbuf_size16_15_configbus1[565:565] , mux_2level_tapbuf_size16_15_configbus0_b[565:565] );
sram6T_blwl sram_blwl_566_ (mux_2level_tapbuf_size16_15_sram_blwl_out[566:566] ,mux_2level_tapbuf_size16_15_sram_blwl_out[566:566] ,mux_2level_tapbuf_size16_15_sram_blwl_outb[566:566] ,mux_2level_tapbuf_size16_15_configbus0[566:566], mux_2level_tapbuf_size16_15_configbus1[566:566] , mux_2level_tapbuf_size16_15_configbus0_b[566:566] );
sram6T_blwl sram_blwl_567_ (mux_2level_tapbuf_size16_15_sram_blwl_out[567:567] ,mux_2level_tapbuf_size16_15_sram_blwl_out[567:567] ,mux_2level_tapbuf_size16_15_sram_blwl_outb[567:567] ,mux_2level_tapbuf_size16_15_configbus0[567:567], mux_2level_tapbuf_size16_15_configbus1[567:567] , mux_2level_tapbuf_size16_15_configbus0_b[567:567] );
wire [0:15] mux_2level_tapbuf_size16_16_inbus;
assign mux_2level_tapbuf_size16_16_inbus[0] = chanx_1__0__midout_8_;
assign mux_2level_tapbuf_size16_16_inbus[1] = chanx_1__0__midout_9_;
assign mux_2level_tapbuf_size16_16_inbus[2] = chanx_1__0__midout_20_;
assign mux_2level_tapbuf_size16_16_inbus[3] = chanx_1__0__midout_21_;
assign mux_2level_tapbuf_size16_16_inbus[4] = chanx_1__0__midout_34_;
assign mux_2level_tapbuf_size16_16_inbus[5] = chanx_1__0__midout_35_;
assign mux_2level_tapbuf_size16_16_inbus[6] = chanx_1__0__midout_46_;
assign mux_2level_tapbuf_size16_16_inbus[7] = chanx_1__0__midout_47_;
assign mux_2level_tapbuf_size16_16_inbus[8] = chanx_1__0__midout_58_;
assign mux_2level_tapbuf_size16_16_inbus[9] = chanx_1__0__midout_59_;
assign mux_2level_tapbuf_size16_16_inbus[10] = chanx_1__0__midout_70_;
assign mux_2level_tapbuf_size16_16_inbus[11] = chanx_1__0__midout_71_;
assign mux_2level_tapbuf_size16_16_inbus[12] = chanx_1__0__midout_84_;
assign mux_2level_tapbuf_size16_16_inbus[13] = chanx_1__0__midout_85_;
assign mux_2level_tapbuf_size16_16_inbus[14] = chanx_1__0__midout_96_;
assign mux_2level_tapbuf_size16_16_inbus[15] = chanx_1__0__midout_97_;
wire [568:575] mux_2level_tapbuf_size16_16_configbus0;
wire [568:575] mux_2level_tapbuf_size16_16_configbus1;
wire [568:575] mux_2level_tapbuf_size16_16_sram_blwl_out ;
wire [568:575] mux_2level_tapbuf_size16_16_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_16_configbus0[568:575] = sram_blwl_bl[568:575] ;
assign mux_2level_tapbuf_size16_16_configbus1[568:575] = sram_blwl_wl[568:575] ;
wire [568:575] mux_2level_tapbuf_size16_16_configbus0_b;
assign mux_2level_tapbuf_size16_16_configbus0_b[568:575] = sram_blwl_blb[568:575] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_16_ (mux_2level_tapbuf_size16_16_inbus, grid_1__0__pin_0__0__12_, mux_2level_tapbuf_size16_16_sram_blwl_out[568:575] ,
mux_2level_tapbuf_size16_16_sram_blwl_outb[568:575] );
//----- SRAM bits for MUX[16], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_568_ (mux_2level_tapbuf_size16_16_sram_blwl_out[568:568] ,mux_2level_tapbuf_size16_16_sram_blwl_out[568:568] ,mux_2level_tapbuf_size16_16_sram_blwl_outb[568:568] ,mux_2level_tapbuf_size16_16_configbus0[568:568], mux_2level_tapbuf_size16_16_configbus1[568:568] , mux_2level_tapbuf_size16_16_configbus0_b[568:568] );
sram6T_blwl sram_blwl_569_ (mux_2level_tapbuf_size16_16_sram_blwl_out[569:569] ,mux_2level_tapbuf_size16_16_sram_blwl_out[569:569] ,mux_2level_tapbuf_size16_16_sram_blwl_outb[569:569] ,mux_2level_tapbuf_size16_16_configbus0[569:569], mux_2level_tapbuf_size16_16_configbus1[569:569] , mux_2level_tapbuf_size16_16_configbus0_b[569:569] );
sram6T_blwl sram_blwl_570_ (mux_2level_tapbuf_size16_16_sram_blwl_out[570:570] ,mux_2level_tapbuf_size16_16_sram_blwl_out[570:570] ,mux_2level_tapbuf_size16_16_sram_blwl_outb[570:570] ,mux_2level_tapbuf_size16_16_configbus0[570:570], mux_2level_tapbuf_size16_16_configbus1[570:570] , mux_2level_tapbuf_size16_16_configbus0_b[570:570] );
sram6T_blwl sram_blwl_571_ (mux_2level_tapbuf_size16_16_sram_blwl_out[571:571] ,mux_2level_tapbuf_size16_16_sram_blwl_out[571:571] ,mux_2level_tapbuf_size16_16_sram_blwl_outb[571:571] ,mux_2level_tapbuf_size16_16_configbus0[571:571], mux_2level_tapbuf_size16_16_configbus1[571:571] , mux_2level_tapbuf_size16_16_configbus0_b[571:571] );
sram6T_blwl sram_blwl_572_ (mux_2level_tapbuf_size16_16_sram_blwl_out[572:572] ,mux_2level_tapbuf_size16_16_sram_blwl_out[572:572] ,mux_2level_tapbuf_size16_16_sram_blwl_outb[572:572] ,mux_2level_tapbuf_size16_16_configbus0[572:572], mux_2level_tapbuf_size16_16_configbus1[572:572] , mux_2level_tapbuf_size16_16_configbus0_b[572:572] );
sram6T_blwl sram_blwl_573_ (mux_2level_tapbuf_size16_16_sram_blwl_out[573:573] ,mux_2level_tapbuf_size16_16_sram_blwl_out[573:573] ,mux_2level_tapbuf_size16_16_sram_blwl_outb[573:573] ,mux_2level_tapbuf_size16_16_configbus0[573:573], mux_2level_tapbuf_size16_16_configbus1[573:573] , mux_2level_tapbuf_size16_16_configbus0_b[573:573] );
sram6T_blwl sram_blwl_574_ (mux_2level_tapbuf_size16_16_sram_blwl_out[574:574] ,mux_2level_tapbuf_size16_16_sram_blwl_out[574:574] ,mux_2level_tapbuf_size16_16_sram_blwl_outb[574:574] ,mux_2level_tapbuf_size16_16_configbus0[574:574], mux_2level_tapbuf_size16_16_configbus1[574:574] , mux_2level_tapbuf_size16_16_configbus0_b[574:574] );
sram6T_blwl sram_blwl_575_ (mux_2level_tapbuf_size16_16_sram_blwl_out[575:575] ,mux_2level_tapbuf_size16_16_sram_blwl_out[575:575] ,mux_2level_tapbuf_size16_16_sram_blwl_outb[575:575] ,mux_2level_tapbuf_size16_16_configbus0[575:575], mux_2level_tapbuf_size16_16_configbus1[575:575] , mux_2level_tapbuf_size16_16_configbus0_b[575:575] );
wire [0:15] mux_2level_tapbuf_size16_17_inbus;
assign mux_2level_tapbuf_size16_17_inbus[0] = chanx_1__0__midout_10_;
assign mux_2level_tapbuf_size16_17_inbus[1] = chanx_1__0__midout_11_;
assign mux_2level_tapbuf_size16_17_inbus[2] = chanx_1__0__midout_22_;
assign mux_2level_tapbuf_size16_17_inbus[3] = chanx_1__0__midout_23_;
assign mux_2level_tapbuf_size16_17_inbus[4] = chanx_1__0__midout_34_;
assign mux_2level_tapbuf_size16_17_inbus[5] = chanx_1__0__midout_35_;
assign mux_2level_tapbuf_size16_17_inbus[6] = chanx_1__0__midout_48_;
assign mux_2level_tapbuf_size16_17_inbus[7] = chanx_1__0__midout_49_;
assign mux_2level_tapbuf_size16_17_inbus[8] = chanx_1__0__midout_60_;
assign mux_2level_tapbuf_size16_17_inbus[9] = chanx_1__0__midout_61_;
assign mux_2level_tapbuf_size16_17_inbus[10] = chanx_1__0__midout_72_;
assign mux_2level_tapbuf_size16_17_inbus[11] = chanx_1__0__midout_73_;
assign mux_2level_tapbuf_size16_17_inbus[12] = chanx_1__0__midout_84_;
assign mux_2level_tapbuf_size16_17_inbus[13] = chanx_1__0__midout_85_;
assign mux_2level_tapbuf_size16_17_inbus[14] = chanx_1__0__midout_98_;
assign mux_2level_tapbuf_size16_17_inbus[15] = chanx_1__0__midout_99_;
wire [576:583] mux_2level_tapbuf_size16_17_configbus0;
wire [576:583] mux_2level_tapbuf_size16_17_configbus1;
wire [576:583] mux_2level_tapbuf_size16_17_sram_blwl_out ;
wire [576:583] mux_2level_tapbuf_size16_17_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_17_configbus0[576:583] = sram_blwl_bl[576:583] ;
assign mux_2level_tapbuf_size16_17_configbus1[576:583] = sram_blwl_wl[576:583] ;
wire [576:583] mux_2level_tapbuf_size16_17_configbus0_b;
assign mux_2level_tapbuf_size16_17_configbus0_b[576:583] = sram_blwl_blb[576:583] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_17_ (mux_2level_tapbuf_size16_17_inbus, grid_1__0__pin_0__0__14_, mux_2level_tapbuf_size16_17_sram_blwl_out[576:583] ,
mux_2level_tapbuf_size16_17_sram_blwl_outb[576:583] );
//----- SRAM bits for MUX[17], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_576_ (mux_2level_tapbuf_size16_17_sram_blwl_out[576:576] ,mux_2level_tapbuf_size16_17_sram_blwl_out[576:576] ,mux_2level_tapbuf_size16_17_sram_blwl_outb[576:576] ,mux_2level_tapbuf_size16_17_configbus0[576:576], mux_2level_tapbuf_size16_17_configbus1[576:576] , mux_2level_tapbuf_size16_17_configbus0_b[576:576] );
sram6T_blwl sram_blwl_577_ (mux_2level_tapbuf_size16_17_sram_blwl_out[577:577] ,mux_2level_tapbuf_size16_17_sram_blwl_out[577:577] ,mux_2level_tapbuf_size16_17_sram_blwl_outb[577:577] ,mux_2level_tapbuf_size16_17_configbus0[577:577], mux_2level_tapbuf_size16_17_configbus1[577:577] , mux_2level_tapbuf_size16_17_configbus0_b[577:577] );
sram6T_blwl sram_blwl_578_ (mux_2level_tapbuf_size16_17_sram_blwl_out[578:578] ,mux_2level_tapbuf_size16_17_sram_blwl_out[578:578] ,mux_2level_tapbuf_size16_17_sram_blwl_outb[578:578] ,mux_2level_tapbuf_size16_17_configbus0[578:578], mux_2level_tapbuf_size16_17_configbus1[578:578] , mux_2level_tapbuf_size16_17_configbus0_b[578:578] );
sram6T_blwl sram_blwl_579_ (mux_2level_tapbuf_size16_17_sram_blwl_out[579:579] ,mux_2level_tapbuf_size16_17_sram_blwl_out[579:579] ,mux_2level_tapbuf_size16_17_sram_blwl_outb[579:579] ,mux_2level_tapbuf_size16_17_configbus0[579:579], mux_2level_tapbuf_size16_17_configbus1[579:579] , mux_2level_tapbuf_size16_17_configbus0_b[579:579] );
sram6T_blwl sram_blwl_580_ (mux_2level_tapbuf_size16_17_sram_blwl_out[580:580] ,mux_2level_tapbuf_size16_17_sram_blwl_out[580:580] ,mux_2level_tapbuf_size16_17_sram_blwl_outb[580:580] ,mux_2level_tapbuf_size16_17_configbus0[580:580], mux_2level_tapbuf_size16_17_configbus1[580:580] , mux_2level_tapbuf_size16_17_configbus0_b[580:580] );
sram6T_blwl sram_blwl_581_ (mux_2level_tapbuf_size16_17_sram_blwl_out[581:581] ,mux_2level_tapbuf_size16_17_sram_blwl_out[581:581] ,mux_2level_tapbuf_size16_17_sram_blwl_outb[581:581] ,mux_2level_tapbuf_size16_17_configbus0[581:581], mux_2level_tapbuf_size16_17_configbus1[581:581] , mux_2level_tapbuf_size16_17_configbus0_b[581:581] );
sram6T_blwl sram_blwl_582_ (mux_2level_tapbuf_size16_17_sram_blwl_out[582:582] ,mux_2level_tapbuf_size16_17_sram_blwl_out[582:582] ,mux_2level_tapbuf_size16_17_sram_blwl_outb[582:582] ,mux_2level_tapbuf_size16_17_configbus0[582:582], mux_2level_tapbuf_size16_17_configbus1[582:582] , mux_2level_tapbuf_size16_17_configbus0_b[582:582] );
sram6T_blwl sram_blwl_583_ (mux_2level_tapbuf_size16_17_sram_blwl_out[583:583] ,mux_2level_tapbuf_size16_17_sram_blwl_out[583:583] ,mux_2level_tapbuf_size16_17_sram_blwl_outb[583:583] ,mux_2level_tapbuf_size16_17_configbus0[583:583], mux_2level_tapbuf_size16_17_configbus1[583:583] , mux_2level_tapbuf_size16_17_configbus0_b[583:583] );
endmodule
//----- END Verilog Module of Connection Box -X direction [1][0] -----

