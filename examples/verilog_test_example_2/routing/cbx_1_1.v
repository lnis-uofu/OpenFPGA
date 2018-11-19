//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Connection Block - X direction  [1][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Verilog Module of Connection Box -X direction [1][1] -----
module cbx_1__1_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input chanx_1__1__midout_0_, 

input chanx_1__1__midout_1_, 

input chanx_1__1__midout_2_, 

input chanx_1__1__midout_3_, 

input chanx_1__1__midout_4_, 

input chanx_1__1__midout_5_, 

input chanx_1__1__midout_6_, 

input chanx_1__1__midout_7_, 

input chanx_1__1__midout_8_, 

input chanx_1__1__midout_9_, 

input chanx_1__1__midout_10_, 

input chanx_1__1__midout_11_, 

input chanx_1__1__midout_12_, 

input chanx_1__1__midout_13_, 

input chanx_1__1__midout_14_, 

input chanx_1__1__midout_15_, 

input chanx_1__1__midout_16_, 

input chanx_1__1__midout_17_, 

input chanx_1__1__midout_18_, 

input chanx_1__1__midout_19_, 

input chanx_1__1__midout_20_, 

input chanx_1__1__midout_21_, 

input chanx_1__1__midout_22_, 

input chanx_1__1__midout_23_, 

input chanx_1__1__midout_24_, 

input chanx_1__1__midout_25_, 

input chanx_1__1__midout_26_, 

input chanx_1__1__midout_27_, 

input chanx_1__1__midout_28_, 

input chanx_1__1__midout_29_, 

input chanx_1__1__midout_30_, 

input chanx_1__1__midout_31_, 

input chanx_1__1__midout_32_, 

input chanx_1__1__midout_33_, 

input chanx_1__1__midout_34_, 

input chanx_1__1__midout_35_, 

input chanx_1__1__midout_36_, 

input chanx_1__1__midout_37_, 

input chanx_1__1__midout_38_, 

input chanx_1__1__midout_39_, 

input chanx_1__1__midout_40_, 

input chanx_1__1__midout_41_, 

input chanx_1__1__midout_42_, 

input chanx_1__1__midout_43_, 

input chanx_1__1__midout_44_, 

input chanx_1__1__midout_45_, 

input chanx_1__1__midout_46_, 

input chanx_1__1__midout_47_, 

input chanx_1__1__midout_48_, 

input chanx_1__1__midout_49_, 

input chanx_1__1__midout_50_, 

input chanx_1__1__midout_51_, 

input chanx_1__1__midout_52_, 

input chanx_1__1__midout_53_, 

input chanx_1__1__midout_54_, 

input chanx_1__1__midout_55_, 

input chanx_1__1__midout_56_, 

input chanx_1__1__midout_57_, 

input chanx_1__1__midout_58_, 

input chanx_1__1__midout_59_, 

input chanx_1__1__midout_60_, 

input chanx_1__1__midout_61_, 

input chanx_1__1__midout_62_, 

input chanx_1__1__midout_63_, 

input chanx_1__1__midout_64_, 

input chanx_1__1__midout_65_, 

input chanx_1__1__midout_66_, 

input chanx_1__1__midout_67_, 

input chanx_1__1__midout_68_, 

input chanx_1__1__midout_69_, 

input chanx_1__1__midout_70_, 

input chanx_1__1__midout_71_, 

input chanx_1__1__midout_72_, 

input chanx_1__1__midout_73_, 

input chanx_1__1__midout_74_, 

input chanx_1__1__midout_75_, 

input chanx_1__1__midout_76_, 

input chanx_1__1__midout_77_, 

input chanx_1__1__midout_78_, 

input chanx_1__1__midout_79_, 

input chanx_1__1__midout_80_, 

input chanx_1__1__midout_81_, 

input chanx_1__1__midout_82_, 

input chanx_1__1__midout_83_, 

input chanx_1__1__midout_84_, 

input chanx_1__1__midout_85_, 

input chanx_1__1__midout_86_, 

input chanx_1__1__midout_87_, 

input chanx_1__1__midout_88_, 

input chanx_1__1__midout_89_, 

input chanx_1__1__midout_90_, 

input chanx_1__1__midout_91_, 

input chanx_1__1__midout_92_, 

input chanx_1__1__midout_93_, 

input chanx_1__1__midout_94_, 

input chanx_1__1__midout_95_, 

input chanx_1__1__midout_96_, 

input chanx_1__1__midout_97_, 

input chanx_1__1__midout_98_, 

input chanx_1__1__midout_99_, 

output  grid_1__2__pin_0__2__0_,

output  grid_1__2__pin_0__2__2_,

output  grid_1__2__pin_0__2__4_,

output  grid_1__2__pin_0__2__6_,

output  grid_1__2__pin_0__2__8_,

output  grid_1__2__pin_0__2__10_,

output  grid_1__2__pin_0__2__12_,

output  grid_1__2__pin_0__2__14_,

output  grid_1__1__pin_0__0__0_,

output  grid_1__1__pin_0__0__4_,

output  grid_1__1__pin_0__0__8_,

output  grid_1__1__pin_0__0__12_,

output  grid_1__1__pin_0__0__16_,

output  grid_1__1__pin_0__0__20_,

output  grid_1__1__pin_0__0__24_,

output  grid_1__1__pin_0__0__28_,

output  grid_1__1__pin_0__0__32_,

output  grid_1__1__pin_0__0__36_,

input [584:727] sram_blwl_bl ,
input [584:727] sram_blwl_wl ,
input [584:727] sram_blwl_blb );
wire [0:15] mux_2level_tapbuf_size16_18_inbus;
assign mux_2level_tapbuf_size16_18_inbus[0] = chanx_1__1__midout_6_;
assign mux_2level_tapbuf_size16_18_inbus[1] = chanx_1__1__midout_7_;
assign mux_2level_tapbuf_size16_18_inbus[2] = chanx_1__1__midout_10_;
assign mux_2level_tapbuf_size16_18_inbus[3] = chanx_1__1__midout_11_;
assign mux_2level_tapbuf_size16_18_inbus[4] = chanx_1__1__midout_30_;
assign mux_2level_tapbuf_size16_18_inbus[5] = chanx_1__1__midout_31_;
assign mux_2level_tapbuf_size16_18_inbus[6] = chanx_1__1__midout_36_;
assign mux_2level_tapbuf_size16_18_inbus[7] = chanx_1__1__midout_37_;
assign mux_2level_tapbuf_size16_18_inbus[8] = chanx_1__1__midout_48_;
assign mux_2level_tapbuf_size16_18_inbus[9] = chanx_1__1__midout_49_;
assign mux_2level_tapbuf_size16_18_inbus[10] = chanx_1__1__midout_60_;
assign mux_2level_tapbuf_size16_18_inbus[11] = chanx_1__1__midout_61_;
assign mux_2level_tapbuf_size16_18_inbus[12] = chanx_1__1__midout_74_;
assign mux_2level_tapbuf_size16_18_inbus[13] = chanx_1__1__midout_75_;
assign mux_2level_tapbuf_size16_18_inbus[14] = chanx_1__1__midout_88_;
assign mux_2level_tapbuf_size16_18_inbus[15] = chanx_1__1__midout_89_;
wire [584:591] mux_2level_tapbuf_size16_18_configbus0;
wire [584:591] mux_2level_tapbuf_size16_18_configbus1;
wire [584:591] mux_2level_tapbuf_size16_18_sram_blwl_out ;
wire [584:591] mux_2level_tapbuf_size16_18_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_18_configbus0[584:591] = sram_blwl_bl[584:591] ;
assign mux_2level_tapbuf_size16_18_configbus1[584:591] = sram_blwl_wl[584:591] ;
wire [584:591] mux_2level_tapbuf_size16_18_configbus0_b;
assign mux_2level_tapbuf_size16_18_configbus0_b[584:591] = sram_blwl_blb[584:591] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_18_ (mux_2level_tapbuf_size16_18_inbus, grid_1__2__pin_0__2__0_, mux_2level_tapbuf_size16_18_sram_blwl_out[584:591] ,
mux_2level_tapbuf_size16_18_sram_blwl_outb[584:591] );
//----- SRAM bits for MUX[18], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_584_ (mux_2level_tapbuf_size16_18_sram_blwl_out[584:584] ,mux_2level_tapbuf_size16_18_sram_blwl_out[584:584] ,mux_2level_tapbuf_size16_18_sram_blwl_outb[584:584] ,mux_2level_tapbuf_size16_18_configbus0[584:584], mux_2level_tapbuf_size16_18_configbus1[584:584] , mux_2level_tapbuf_size16_18_configbus0_b[584:584] );
sram6T_blwl sram_blwl_585_ (mux_2level_tapbuf_size16_18_sram_blwl_out[585:585] ,mux_2level_tapbuf_size16_18_sram_blwl_out[585:585] ,mux_2level_tapbuf_size16_18_sram_blwl_outb[585:585] ,mux_2level_tapbuf_size16_18_configbus0[585:585], mux_2level_tapbuf_size16_18_configbus1[585:585] , mux_2level_tapbuf_size16_18_configbus0_b[585:585] );
sram6T_blwl sram_blwl_586_ (mux_2level_tapbuf_size16_18_sram_blwl_out[586:586] ,mux_2level_tapbuf_size16_18_sram_blwl_out[586:586] ,mux_2level_tapbuf_size16_18_sram_blwl_outb[586:586] ,mux_2level_tapbuf_size16_18_configbus0[586:586], mux_2level_tapbuf_size16_18_configbus1[586:586] , mux_2level_tapbuf_size16_18_configbus0_b[586:586] );
sram6T_blwl sram_blwl_587_ (mux_2level_tapbuf_size16_18_sram_blwl_out[587:587] ,mux_2level_tapbuf_size16_18_sram_blwl_out[587:587] ,mux_2level_tapbuf_size16_18_sram_blwl_outb[587:587] ,mux_2level_tapbuf_size16_18_configbus0[587:587], mux_2level_tapbuf_size16_18_configbus1[587:587] , mux_2level_tapbuf_size16_18_configbus0_b[587:587] );
sram6T_blwl sram_blwl_588_ (mux_2level_tapbuf_size16_18_sram_blwl_out[588:588] ,mux_2level_tapbuf_size16_18_sram_blwl_out[588:588] ,mux_2level_tapbuf_size16_18_sram_blwl_outb[588:588] ,mux_2level_tapbuf_size16_18_configbus0[588:588], mux_2level_tapbuf_size16_18_configbus1[588:588] , mux_2level_tapbuf_size16_18_configbus0_b[588:588] );
sram6T_blwl sram_blwl_589_ (mux_2level_tapbuf_size16_18_sram_blwl_out[589:589] ,mux_2level_tapbuf_size16_18_sram_blwl_out[589:589] ,mux_2level_tapbuf_size16_18_sram_blwl_outb[589:589] ,mux_2level_tapbuf_size16_18_configbus0[589:589], mux_2level_tapbuf_size16_18_configbus1[589:589] , mux_2level_tapbuf_size16_18_configbus0_b[589:589] );
sram6T_blwl sram_blwl_590_ (mux_2level_tapbuf_size16_18_sram_blwl_out[590:590] ,mux_2level_tapbuf_size16_18_sram_blwl_out[590:590] ,mux_2level_tapbuf_size16_18_sram_blwl_outb[590:590] ,mux_2level_tapbuf_size16_18_configbus0[590:590], mux_2level_tapbuf_size16_18_configbus1[590:590] , mux_2level_tapbuf_size16_18_configbus0_b[590:590] );
sram6T_blwl sram_blwl_591_ (mux_2level_tapbuf_size16_18_sram_blwl_out[591:591] ,mux_2level_tapbuf_size16_18_sram_blwl_out[591:591] ,mux_2level_tapbuf_size16_18_sram_blwl_outb[591:591] ,mux_2level_tapbuf_size16_18_configbus0[591:591], mux_2level_tapbuf_size16_18_configbus1[591:591] , mux_2level_tapbuf_size16_18_configbus0_b[591:591] );
wire [0:15] mux_2level_tapbuf_size16_19_inbus;
assign mux_2level_tapbuf_size16_19_inbus[0] = chanx_1__1__midout_0_;
assign mux_2level_tapbuf_size16_19_inbus[1] = chanx_1__1__midout_1_;
assign mux_2level_tapbuf_size16_19_inbus[2] = chanx_1__1__midout_12_;
assign mux_2level_tapbuf_size16_19_inbus[3] = chanx_1__1__midout_13_;
assign mux_2level_tapbuf_size16_19_inbus[4] = chanx_1__1__midout_24_;
assign mux_2level_tapbuf_size16_19_inbus[5] = chanx_1__1__midout_25_;
assign mux_2level_tapbuf_size16_19_inbus[6] = chanx_1__1__midout_36_;
assign mux_2level_tapbuf_size16_19_inbus[7] = chanx_1__1__midout_37_;
assign mux_2level_tapbuf_size16_19_inbus[8] = chanx_1__1__midout_54_;
assign mux_2level_tapbuf_size16_19_inbus[9] = chanx_1__1__midout_55_;
assign mux_2level_tapbuf_size16_19_inbus[10] = chanx_1__1__midout_66_;
assign mux_2level_tapbuf_size16_19_inbus[11] = chanx_1__1__midout_67_;
assign mux_2level_tapbuf_size16_19_inbus[12] = chanx_1__1__midout_76_;
assign mux_2level_tapbuf_size16_19_inbus[13] = chanx_1__1__midout_77_;
assign mux_2level_tapbuf_size16_19_inbus[14] = chanx_1__1__midout_88_;
assign mux_2level_tapbuf_size16_19_inbus[15] = chanx_1__1__midout_89_;
wire [592:599] mux_2level_tapbuf_size16_19_configbus0;
wire [592:599] mux_2level_tapbuf_size16_19_configbus1;
wire [592:599] mux_2level_tapbuf_size16_19_sram_blwl_out ;
wire [592:599] mux_2level_tapbuf_size16_19_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_19_configbus0[592:599] = sram_blwl_bl[592:599] ;
assign mux_2level_tapbuf_size16_19_configbus1[592:599] = sram_blwl_wl[592:599] ;
wire [592:599] mux_2level_tapbuf_size16_19_configbus0_b;
assign mux_2level_tapbuf_size16_19_configbus0_b[592:599] = sram_blwl_blb[592:599] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_19_ (mux_2level_tapbuf_size16_19_inbus, grid_1__2__pin_0__2__2_, mux_2level_tapbuf_size16_19_sram_blwl_out[592:599] ,
mux_2level_tapbuf_size16_19_sram_blwl_outb[592:599] );
//----- SRAM bits for MUX[19], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_592_ (mux_2level_tapbuf_size16_19_sram_blwl_out[592:592] ,mux_2level_tapbuf_size16_19_sram_blwl_out[592:592] ,mux_2level_tapbuf_size16_19_sram_blwl_outb[592:592] ,mux_2level_tapbuf_size16_19_configbus0[592:592], mux_2level_tapbuf_size16_19_configbus1[592:592] , mux_2level_tapbuf_size16_19_configbus0_b[592:592] );
sram6T_blwl sram_blwl_593_ (mux_2level_tapbuf_size16_19_sram_blwl_out[593:593] ,mux_2level_tapbuf_size16_19_sram_blwl_out[593:593] ,mux_2level_tapbuf_size16_19_sram_blwl_outb[593:593] ,mux_2level_tapbuf_size16_19_configbus0[593:593], mux_2level_tapbuf_size16_19_configbus1[593:593] , mux_2level_tapbuf_size16_19_configbus0_b[593:593] );
sram6T_blwl sram_blwl_594_ (mux_2level_tapbuf_size16_19_sram_blwl_out[594:594] ,mux_2level_tapbuf_size16_19_sram_blwl_out[594:594] ,mux_2level_tapbuf_size16_19_sram_blwl_outb[594:594] ,mux_2level_tapbuf_size16_19_configbus0[594:594], mux_2level_tapbuf_size16_19_configbus1[594:594] , mux_2level_tapbuf_size16_19_configbus0_b[594:594] );
sram6T_blwl sram_blwl_595_ (mux_2level_tapbuf_size16_19_sram_blwl_out[595:595] ,mux_2level_tapbuf_size16_19_sram_blwl_out[595:595] ,mux_2level_tapbuf_size16_19_sram_blwl_outb[595:595] ,mux_2level_tapbuf_size16_19_configbus0[595:595], mux_2level_tapbuf_size16_19_configbus1[595:595] , mux_2level_tapbuf_size16_19_configbus0_b[595:595] );
sram6T_blwl sram_blwl_596_ (mux_2level_tapbuf_size16_19_sram_blwl_out[596:596] ,mux_2level_tapbuf_size16_19_sram_blwl_out[596:596] ,mux_2level_tapbuf_size16_19_sram_blwl_outb[596:596] ,mux_2level_tapbuf_size16_19_configbus0[596:596], mux_2level_tapbuf_size16_19_configbus1[596:596] , mux_2level_tapbuf_size16_19_configbus0_b[596:596] );
sram6T_blwl sram_blwl_597_ (mux_2level_tapbuf_size16_19_sram_blwl_out[597:597] ,mux_2level_tapbuf_size16_19_sram_blwl_out[597:597] ,mux_2level_tapbuf_size16_19_sram_blwl_outb[597:597] ,mux_2level_tapbuf_size16_19_configbus0[597:597], mux_2level_tapbuf_size16_19_configbus1[597:597] , mux_2level_tapbuf_size16_19_configbus0_b[597:597] );
sram6T_blwl sram_blwl_598_ (mux_2level_tapbuf_size16_19_sram_blwl_out[598:598] ,mux_2level_tapbuf_size16_19_sram_blwl_out[598:598] ,mux_2level_tapbuf_size16_19_sram_blwl_outb[598:598] ,mux_2level_tapbuf_size16_19_configbus0[598:598], mux_2level_tapbuf_size16_19_configbus1[598:598] , mux_2level_tapbuf_size16_19_configbus0_b[598:598] );
sram6T_blwl sram_blwl_599_ (mux_2level_tapbuf_size16_19_sram_blwl_out[599:599] ,mux_2level_tapbuf_size16_19_sram_blwl_out[599:599] ,mux_2level_tapbuf_size16_19_sram_blwl_outb[599:599] ,mux_2level_tapbuf_size16_19_configbus0[599:599], mux_2level_tapbuf_size16_19_configbus1[599:599] , mux_2level_tapbuf_size16_19_configbus0_b[599:599] );
wire [0:15] mux_2level_tapbuf_size16_20_inbus;
assign mux_2level_tapbuf_size16_20_inbus[0] = chanx_1__1__midout_0_;
assign mux_2level_tapbuf_size16_20_inbus[1] = chanx_1__1__midout_1_;
assign mux_2level_tapbuf_size16_20_inbus[2] = chanx_1__1__midout_22_;
assign mux_2level_tapbuf_size16_20_inbus[3] = chanx_1__1__midout_23_;
assign mux_2level_tapbuf_size16_20_inbus[4] = chanx_1__1__midout_26_;
assign mux_2level_tapbuf_size16_20_inbus[5] = chanx_1__1__midout_27_;
assign mux_2level_tapbuf_size16_20_inbus[6] = chanx_1__1__midout_42_;
assign mux_2level_tapbuf_size16_20_inbus[7] = chanx_1__1__midout_43_;
assign mux_2level_tapbuf_size16_20_inbus[8] = chanx_1__1__midout_54_;
assign mux_2level_tapbuf_size16_20_inbus[9] = chanx_1__1__midout_55_;
assign mux_2level_tapbuf_size16_20_inbus[10] = chanx_1__1__midout_64_;
assign mux_2level_tapbuf_size16_20_inbus[11] = chanx_1__1__midout_65_;
assign mux_2level_tapbuf_size16_20_inbus[12] = chanx_1__1__midout_78_;
assign mux_2level_tapbuf_size16_20_inbus[13] = chanx_1__1__midout_79_;
assign mux_2level_tapbuf_size16_20_inbus[14] = chanx_1__1__midout_90_;
assign mux_2level_tapbuf_size16_20_inbus[15] = chanx_1__1__midout_91_;
wire [600:607] mux_2level_tapbuf_size16_20_configbus0;
wire [600:607] mux_2level_tapbuf_size16_20_configbus1;
wire [600:607] mux_2level_tapbuf_size16_20_sram_blwl_out ;
wire [600:607] mux_2level_tapbuf_size16_20_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_20_configbus0[600:607] = sram_blwl_bl[600:607] ;
assign mux_2level_tapbuf_size16_20_configbus1[600:607] = sram_blwl_wl[600:607] ;
wire [600:607] mux_2level_tapbuf_size16_20_configbus0_b;
assign mux_2level_tapbuf_size16_20_configbus0_b[600:607] = sram_blwl_blb[600:607] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_20_ (mux_2level_tapbuf_size16_20_inbus, grid_1__2__pin_0__2__4_, mux_2level_tapbuf_size16_20_sram_blwl_out[600:607] ,
mux_2level_tapbuf_size16_20_sram_blwl_outb[600:607] );
//----- SRAM bits for MUX[20], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_600_ (mux_2level_tapbuf_size16_20_sram_blwl_out[600:600] ,mux_2level_tapbuf_size16_20_sram_blwl_out[600:600] ,mux_2level_tapbuf_size16_20_sram_blwl_outb[600:600] ,mux_2level_tapbuf_size16_20_configbus0[600:600], mux_2level_tapbuf_size16_20_configbus1[600:600] , mux_2level_tapbuf_size16_20_configbus0_b[600:600] );
sram6T_blwl sram_blwl_601_ (mux_2level_tapbuf_size16_20_sram_blwl_out[601:601] ,mux_2level_tapbuf_size16_20_sram_blwl_out[601:601] ,mux_2level_tapbuf_size16_20_sram_blwl_outb[601:601] ,mux_2level_tapbuf_size16_20_configbus0[601:601], mux_2level_tapbuf_size16_20_configbus1[601:601] , mux_2level_tapbuf_size16_20_configbus0_b[601:601] );
sram6T_blwl sram_blwl_602_ (mux_2level_tapbuf_size16_20_sram_blwl_out[602:602] ,mux_2level_tapbuf_size16_20_sram_blwl_out[602:602] ,mux_2level_tapbuf_size16_20_sram_blwl_outb[602:602] ,mux_2level_tapbuf_size16_20_configbus0[602:602], mux_2level_tapbuf_size16_20_configbus1[602:602] , mux_2level_tapbuf_size16_20_configbus0_b[602:602] );
sram6T_blwl sram_blwl_603_ (mux_2level_tapbuf_size16_20_sram_blwl_out[603:603] ,mux_2level_tapbuf_size16_20_sram_blwl_out[603:603] ,mux_2level_tapbuf_size16_20_sram_blwl_outb[603:603] ,mux_2level_tapbuf_size16_20_configbus0[603:603], mux_2level_tapbuf_size16_20_configbus1[603:603] , mux_2level_tapbuf_size16_20_configbus0_b[603:603] );
sram6T_blwl sram_blwl_604_ (mux_2level_tapbuf_size16_20_sram_blwl_out[604:604] ,mux_2level_tapbuf_size16_20_sram_blwl_out[604:604] ,mux_2level_tapbuf_size16_20_sram_blwl_outb[604:604] ,mux_2level_tapbuf_size16_20_configbus0[604:604], mux_2level_tapbuf_size16_20_configbus1[604:604] , mux_2level_tapbuf_size16_20_configbus0_b[604:604] );
sram6T_blwl sram_blwl_605_ (mux_2level_tapbuf_size16_20_sram_blwl_out[605:605] ,mux_2level_tapbuf_size16_20_sram_blwl_out[605:605] ,mux_2level_tapbuf_size16_20_sram_blwl_outb[605:605] ,mux_2level_tapbuf_size16_20_configbus0[605:605], mux_2level_tapbuf_size16_20_configbus1[605:605] , mux_2level_tapbuf_size16_20_configbus0_b[605:605] );
sram6T_blwl sram_blwl_606_ (mux_2level_tapbuf_size16_20_sram_blwl_out[606:606] ,mux_2level_tapbuf_size16_20_sram_blwl_out[606:606] ,mux_2level_tapbuf_size16_20_sram_blwl_outb[606:606] ,mux_2level_tapbuf_size16_20_configbus0[606:606], mux_2level_tapbuf_size16_20_configbus1[606:606] , mux_2level_tapbuf_size16_20_configbus0_b[606:606] );
sram6T_blwl sram_blwl_607_ (mux_2level_tapbuf_size16_20_sram_blwl_out[607:607] ,mux_2level_tapbuf_size16_20_sram_blwl_out[607:607] ,mux_2level_tapbuf_size16_20_sram_blwl_outb[607:607] ,mux_2level_tapbuf_size16_20_configbus0[607:607], mux_2level_tapbuf_size16_20_configbus1[607:607] , mux_2level_tapbuf_size16_20_configbus0_b[607:607] );
wire [0:15] mux_2level_tapbuf_size16_21_inbus;
assign mux_2level_tapbuf_size16_21_inbus[0] = chanx_1__1__midout_2_;
assign mux_2level_tapbuf_size16_21_inbus[1] = chanx_1__1__midout_3_;
assign mux_2level_tapbuf_size16_21_inbus[2] = chanx_1__1__midout_22_;
assign mux_2level_tapbuf_size16_21_inbus[3] = chanx_1__1__midout_23_;
assign mux_2level_tapbuf_size16_21_inbus[4] = chanx_1__1__midout_28_;
assign mux_2level_tapbuf_size16_21_inbus[5] = chanx_1__1__midout_29_;
assign mux_2level_tapbuf_size16_21_inbus[6] = chanx_1__1__midout_40_;
assign mux_2level_tapbuf_size16_21_inbus[7] = chanx_1__1__midout_41_;
assign mux_2level_tapbuf_size16_21_inbus[8] = chanx_1__1__midout_52_;
assign mux_2level_tapbuf_size16_21_inbus[9] = chanx_1__1__midout_53_;
assign mux_2level_tapbuf_size16_21_inbus[10] = chanx_1__1__midout_64_;
assign mux_2level_tapbuf_size16_21_inbus[11] = chanx_1__1__midout_65_;
assign mux_2level_tapbuf_size16_21_inbus[12] = chanx_1__1__midout_80_;
assign mux_2level_tapbuf_size16_21_inbus[13] = chanx_1__1__midout_81_;
assign mux_2level_tapbuf_size16_21_inbus[14] = chanx_1__1__midout_92_;
assign mux_2level_tapbuf_size16_21_inbus[15] = chanx_1__1__midout_93_;
wire [608:615] mux_2level_tapbuf_size16_21_configbus0;
wire [608:615] mux_2level_tapbuf_size16_21_configbus1;
wire [608:615] mux_2level_tapbuf_size16_21_sram_blwl_out ;
wire [608:615] mux_2level_tapbuf_size16_21_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_21_configbus0[608:615] = sram_blwl_bl[608:615] ;
assign mux_2level_tapbuf_size16_21_configbus1[608:615] = sram_blwl_wl[608:615] ;
wire [608:615] mux_2level_tapbuf_size16_21_configbus0_b;
assign mux_2level_tapbuf_size16_21_configbus0_b[608:615] = sram_blwl_blb[608:615] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_21_ (mux_2level_tapbuf_size16_21_inbus, grid_1__2__pin_0__2__6_, mux_2level_tapbuf_size16_21_sram_blwl_out[608:615] ,
mux_2level_tapbuf_size16_21_sram_blwl_outb[608:615] );
//----- SRAM bits for MUX[21], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_608_ (mux_2level_tapbuf_size16_21_sram_blwl_out[608:608] ,mux_2level_tapbuf_size16_21_sram_blwl_out[608:608] ,mux_2level_tapbuf_size16_21_sram_blwl_outb[608:608] ,mux_2level_tapbuf_size16_21_configbus0[608:608], mux_2level_tapbuf_size16_21_configbus1[608:608] , mux_2level_tapbuf_size16_21_configbus0_b[608:608] );
sram6T_blwl sram_blwl_609_ (mux_2level_tapbuf_size16_21_sram_blwl_out[609:609] ,mux_2level_tapbuf_size16_21_sram_blwl_out[609:609] ,mux_2level_tapbuf_size16_21_sram_blwl_outb[609:609] ,mux_2level_tapbuf_size16_21_configbus0[609:609], mux_2level_tapbuf_size16_21_configbus1[609:609] , mux_2level_tapbuf_size16_21_configbus0_b[609:609] );
sram6T_blwl sram_blwl_610_ (mux_2level_tapbuf_size16_21_sram_blwl_out[610:610] ,mux_2level_tapbuf_size16_21_sram_blwl_out[610:610] ,mux_2level_tapbuf_size16_21_sram_blwl_outb[610:610] ,mux_2level_tapbuf_size16_21_configbus0[610:610], mux_2level_tapbuf_size16_21_configbus1[610:610] , mux_2level_tapbuf_size16_21_configbus0_b[610:610] );
sram6T_blwl sram_blwl_611_ (mux_2level_tapbuf_size16_21_sram_blwl_out[611:611] ,mux_2level_tapbuf_size16_21_sram_blwl_out[611:611] ,mux_2level_tapbuf_size16_21_sram_blwl_outb[611:611] ,mux_2level_tapbuf_size16_21_configbus0[611:611], mux_2level_tapbuf_size16_21_configbus1[611:611] , mux_2level_tapbuf_size16_21_configbus0_b[611:611] );
sram6T_blwl sram_blwl_612_ (mux_2level_tapbuf_size16_21_sram_blwl_out[612:612] ,mux_2level_tapbuf_size16_21_sram_blwl_out[612:612] ,mux_2level_tapbuf_size16_21_sram_blwl_outb[612:612] ,mux_2level_tapbuf_size16_21_configbus0[612:612], mux_2level_tapbuf_size16_21_configbus1[612:612] , mux_2level_tapbuf_size16_21_configbus0_b[612:612] );
sram6T_blwl sram_blwl_613_ (mux_2level_tapbuf_size16_21_sram_blwl_out[613:613] ,mux_2level_tapbuf_size16_21_sram_blwl_out[613:613] ,mux_2level_tapbuf_size16_21_sram_blwl_outb[613:613] ,mux_2level_tapbuf_size16_21_configbus0[613:613], mux_2level_tapbuf_size16_21_configbus1[613:613] , mux_2level_tapbuf_size16_21_configbus0_b[613:613] );
sram6T_blwl sram_blwl_614_ (mux_2level_tapbuf_size16_21_sram_blwl_out[614:614] ,mux_2level_tapbuf_size16_21_sram_blwl_out[614:614] ,mux_2level_tapbuf_size16_21_sram_blwl_outb[614:614] ,mux_2level_tapbuf_size16_21_configbus0[614:614], mux_2level_tapbuf_size16_21_configbus1[614:614] , mux_2level_tapbuf_size16_21_configbus0_b[614:614] );
sram6T_blwl sram_blwl_615_ (mux_2level_tapbuf_size16_21_sram_blwl_out[615:615] ,mux_2level_tapbuf_size16_21_sram_blwl_out[615:615] ,mux_2level_tapbuf_size16_21_sram_blwl_outb[615:615] ,mux_2level_tapbuf_size16_21_configbus0[615:615], mux_2level_tapbuf_size16_21_configbus1[615:615] , mux_2level_tapbuf_size16_21_configbus0_b[615:615] );
wire [0:15] mux_2level_tapbuf_size16_22_inbus;
assign mux_2level_tapbuf_size16_22_inbus[0] = chanx_1__1__midout_4_;
assign mux_2level_tapbuf_size16_22_inbus[1] = chanx_1__1__midout_5_;
assign mux_2level_tapbuf_size16_22_inbus[2] = chanx_1__1__midout_16_;
assign mux_2level_tapbuf_size16_22_inbus[3] = chanx_1__1__midout_17_;
assign mux_2level_tapbuf_size16_22_inbus[4] = chanx_1__1__midout_38_;
assign mux_2level_tapbuf_size16_22_inbus[5] = chanx_1__1__midout_39_;
assign mux_2level_tapbuf_size16_22_inbus[6] = chanx_1__1__midout_46_;
assign mux_2level_tapbuf_size16_22_inbus[7] = chanx_1__1__midout_47_;
assign mux_2level_tapbuf_size16_22_inbus[8] = chanx_1__1__midout_58_;
assign mux_2level_tapbuf_size16_22_inbus[9] = chanx_1__1__midout_59_;
assign mux_2level_tapbuf_size16_22_inbus[10] = chanx_1__1__midout_68_;
assign mux_2level_tapbuf_size16_22_inbus[11] = chanx_1__1__midout_69_;
assign mux_2level_tapbuf_size16_22_inbus[12] = chanx_1__1__midout_82_;
assign mux_2level_tapbuf_size16_22_inbus[13] = chanx_1__1__midout_83_;
assign mux_2level_tapbuf_size16_22_inbus[14] = chanx_1__1__midout_94_;
assign mux_2level_tapbuf_size16_22_inbus[15] = chanx_1__1__midout_95_;
wire [616:623] mux_2level_tapbuf_size16_22_configbus0;
wire [616:623] mux_2level_tapbuf_size16_22_configbus1;
wire [616:623] mux_2level_tapbuf_size16_22_sram_blwl_out ;
wire [616:623] mux_2level_tapbuf_size16_22_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_22_configbus0[616:623] = sram_blwl_bl[616:623] ;
assign mux_2level_tapbuf_size16_22_configbus1[616:623] = sram_blwl_wl[616:623] ;
wire [616:623] mux_2level_tapbuf_size16_22_configbus0_b;
assign mux_2level_tapbuf_size16_22_configbus0_b[616:623] = sram_blwl_blb[616:623] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_22_ (mux_2level_tapbuf_size16_22_inbus, grid_1__2__pin_0__2__8_, mux_2level_tapbuf_size16_22_sram_blwl_out[616:623] ,
mux_2level_tapbuf_size16_22_sram_blwl_outb[616:623] );
//----- SRAM bits for MUX[22], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_616_ (mux_2level_tapbuf_size16_22_sram_blwl_out[616:616] ,mux_2level_tapbuf_size16_22_sram_blwl_out[616:616] ,mux_2level_tapbuf_size16_22_sram_blwl_outb[616:616] ,mux_2level_tapbuf_size16_22_configbus0[616:616], mux_2level_tapbuf_size16_22_configbus1[616:616] , mux_2level_tapbuf_size16_22_configbus0_b[616:616] );
sram6T_blwl sram_blwl_617_ (mux_2level_tapbuf_size16_22_sram_blwl_out[617:617] ,mux_2level_tapbuf_size16_22_sram_blwl_out[617:617] ,mux_2level_tapbuf_size16_22_sram_blwl_outb[617:617] ,mux_2level_tapbuf_size16_22_configbus0[617:617], mux_2level_tapbuf_size16_22_configbus1[617:617] , mux_2level_tapbuf_size16_22_configbus0_b[617:617] );
sram6T_blwl sram_blwl_618_ (mux_2level_tapbuf_size16_22_sram_blwl_out[618:618] ,mux_2level_tapbuf_size16_22_sram_blwl_out[618:618] ,mux_2level_tapbuf_size16_22_sram_blwl_outb[618:618] ,mux_2level_tapbuf_size16_22_configbus0[618:618], mux_2level_tapbuf_size16_22_configbus1[618:618] , mux_2level_tapbuf_size16_22_configbus0_b[618:618] );
sram6T_blwl sram_blwl_619_ (mux_2level_tapbuf_size16_22_sram_blwl_out[619:619] ,mux_2level_tapbuf_size16_22_sram_blwl_out[619:619] ,mux_2level_tapbuf_size16_22_sram_blwl_outb[619:619] ,mux_2level_tapbuf_size16_22_configbus0[619:619], mux_2level_tapbuf_size16_22_configbus1[619:619] , mux_2level_tapbuf_size16_22_configbus0_b[619:619] );
sram6T_blwl sram_blwl_620_ (mux_2level_tapbuf_size16_22_sram_blwl_out[620:620] ,mux_2level_tapbuf_size16_22_sram_blwl_out[620:620] ,mux_2level_tapbuf_size16_22_sram_blwl_outb[620:620] ,mux_2level_tapbuf_size16_22_configbus0[620:620], mux_2level_tapbuf_size16_22_configbus1[620:620] , mux_2level_tapbuf_size16_22_configbus0_b[620:620] );
sram6T_blwl sram_blwl_621_ (mux_2level_tapbuf_size16_22_sram_blwl_out[621:621] ,mux_2level_tapbuf_size16_22_sram_blwl_out[621:621] ,mux_2level_tapbuf_size16_22_sram_blwl_outb[621:621] ,mux_2level_tapbuf_size16_22_configbus0[621:621], mux_2level_tapbuf_size16_22_configbus1[621:621] , mux_2level_tapbuf_size16_22_configbus0_b[621:621] );
sram6T_blwl sram_blwl_622_ (mux_2level_tapbuf_size16_22_sram_blwl_out[622:622] ,mux_2level_tapbuf_size16_22_sram_blwl_out[622:622] ,mux_2level_tapbuf_size16_22_sram_blwl_outb[622:622] ,mux_2level_tapbuf_size16_22_configbus0[622:622], mux_2level_tapbuf_size16_22_configbus1[622:622] , mux_2level_tapbuf_size16_22_configbus0_b[622:622] );
sram6T_blwl sram_blwl_623_ (mux_2level_tapbuf_size16_22_sram_blwl_out[623:623] ,mux_2level_tapbuf_size16_22_sram_blwl_out[623:623] ,mux_2level_tapbuf_size16_22_sram_blwl_outb[623:623] ,mux_2level_tapbuf_size16_22_configbus0[623:623], mux_2level_tapbuf_size16_22_configbus1[623:623] , mux_2level_tapbuf_size16_22_configbus0_b[623:623] );
wire [0:15] mux_2level_tapbuf_size16_23_inbus;
assign mux_2level_tapbuf_size16_23_inbus[0] = chanx_1__1__midout_14_;
assign mux_2level_tapbuf_size16_23_inbus[1] = chanx_1__1__midout_15_;
assign mux_2level_tapbuf_size16_23_inbus[2] = chanx_1__1__midout_18_;
assign mux_2level_tapbuf_size16_23_inbus[3] = chanx_1__1__midout_19_;
assign mux_2level_tapbuf_size16_23_inbus[4] = chanx_1__1__midout_38_;
assign mux_2level_tapbuf_size16_23_inbus[5] = chanx_1__1__midout_39_;
assign mux_2level_tapbuf_size16_23_inbus[6] = chanx_1__1__midout_44_;
assign mux_2level_tapbuf_size16_23_inbus[7] = chanx_1__1__midout_45_;
assign mux_2level_tapbuf_size16_23_inbus[8] = chanx_1__1__midout_56_;
assign mux_2level_tapbuf_size16_23_inbus[9] = chanx_1__1__midout_57_;
assign mux_2level_tapbuf_size16_23_inbus[10] = chanx_1__1__midout_70_;
assign mux_2level_tapbuf_size16_23_inbus[11] = chanx_1__1__midout_71_;
assign mux_2level_tapbuf_size16_23_inbus[12] = chanx_1__1__midout_82_;
assign mux_2level_tapbuf_size16_23_inbus[13] = chanx_1__1__midout_83_;
assign mux_2level_tapbuf_size16_23_inbus[14] = chanx_1__1__midout_96_;
assign mux_2level_tapbuf_size16_23_inbus[15] = chanx_1__1__midout_97_;
wire [624:631] mux_2level_tapbuf_size16_23_configbus0;
wire [624:631] mux_2level_tapbuf_size16_23_configbus1;
wire [624:631] mux_2level_tapbuf_size16_23_sram_blwl_out ;
wire [624:631] mux_2level_tapbuf_size16_23_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_23_configbus0[624:631] = sram_blwl_bl[624:631] ;
assign mux_2level_tapbuf_size16_23_configbus1[624:631] = sram_blwl_wl[624:631] ;
wire [624:631] mux_2level_tapbuf_size16_23_configbus0_b;
assign mux_2level_tapbuf_size16_23_configbus0_b[624:631] = sram_blwl_blb[624:631] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_23_ (mux_2level_tapbuf_size16_23_inbus, grid_1__2__pin_0__2__10_, mux_2level_tapbuf_size16_23_sram_blwl_out[624:631] ,
mux_2level_tapbuf_size16_23_sram_blwl_outb[624:631] );
//----- SRAM bits for MUX[23], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_624_ (mux_2level_tapbuf_size16_23_sram_blwl_out[624:624] ,mux_2level_tapbuf_size16_23_sram_blwl_out[624:624] ,mux_2level_tapbuf_size16_23_sram_blwl_outb[624:624] ,mux_2level_tapbuf_size16_23_configbus0[624:624], mux_2level_tapbuf_size16_23_configbus1[624:624] , mux_2level_tapbuf_size16_23_configbus0_b[624:624] );
sram6T_blwl sram_blwl_625_ (mux_2level_tapbuf_size16_23_sram_blwl_out[625:625] ,mux_2level_tapbuf_size16_23_sram_blwl_out[625:625] ,mux_2level_tapbuf_size16_23_sram_blwl_outb[625:625] ,mux_2level_tapbuf_size16_23_configbus0[625:625], mux_2level_tapbuf_size16_23_configbus1[625:625] , mux_2level_tapbuf_size16_23_configbus0_b[625:625] );
sram6T_blwl sram_blwl_626_ (mux_2level_tapbuf_size16_23_sram_blwl_out[626:626] ,mux_2level_tapbuf_size16_23_sram_blwl_out[626:626] ,mux_2level_tapbuf_size16_23_sram_blwl_outb[626:626] ,mux_2level_tapbuf_size16_23_configbus0[626:626], mux_2level_tapbuf_size16_23_configbus1[626:626] , mux_2level_tapbuf_size16_23_configbus0_b[626:626] );
sram6T_blwl sram_blwl_627_ (mux_2level_tapbuf_size16_23_sram_blwl_out[627:627] ,mux_2level_tapbuf_size16_23_sram_blwl_out[627:627] ,mux_2level_tapbuf_size16_23_sram_blwl_outb[627:627] ,mux_2level_tapbuf_size16_23_configbus0[627:627], mux_2level_tapbuf_size16_23_configbus1[627:627] , mux_2level_tapbuf_size16_23_configbus0_b[627:627] );
sram6T_blwl sram_blwl_628_ (mux_2level_tapbuf_size16_23_sram_blwl_out[628:628] ,mux_2level_tapbuf_size16_23_sram_blwl_out[628:628] ,mux_2level_tapbuf_size16_23_sram_blwl_outb[628:628] ,mux_2level_tapbuf_size16_23_configbus0[628:628], mux_2level_tapbuf_size16_23_configbus1[628:628] , mux_2level_tapbuf_size16_23_configbus0_b[628:628] );
sram6T_blwl sram_blwl_629_ (mux_2level_tapbuf_size16_23_sram_blwl_out[629:629] ,mux_2level_tapbuf_size16_23_sram_blwl_out[629:629] ,mux_2level_tapbuf_size16_23_sram_blwl_outb[629:629] ,mux_2level_tapbuf_size16_23_configbus0[629:629], mux_2level_tapbuf_size16_23_configbus1[629:629] , mux_2level_tapbuf_size16_23_configbus0_b[629:629] );
sram6T_blwl sram_blwl_630_ (mux_2level_tapbuf_size16_23_sram_blwl_out[630:630] ,mux_2level_tapbuf_size16_23_sram_blwl_out[630:630] ,mux_2level_tapbuf_size16_23_sram_blwl_outb[630:630] ,mux_2level_tapbuf_size16_23_configbus0[630:630], mux_2level_tapbuf_size16_23_configbus1[630:630] , mux_2level_tapbuf_size16_23_configbus0_b[630:630] );
sram6T_blwl sram_blwl_631_ (mux_2level_tapbuf_size16_23_sram_blwl_out[631:631] ,mux_2level_tapbuf_size16_23_sram_blwl_out[631:631] ,mux_2level_tapbuf_size16_23_sram_blwl_outb[631:631] ,mux_2level_tapbuf_size16_23_configbus0[631:631], mux_2level_tapbuf_size16_23_configbus1[631:631] , mux_2level_tapbuf_size16_23_configbus0_b[631:631] );
wire [0:15] mux_2level_tapbuf_size16_24_inbus;
assign mux_2level_tapbuf_size16_24_inbus[0] = chanx_1__1__midout_8_;
assign mux_2level_tapbuf_size16_24_inbus[1] = chanx_1__1__midout_9_;
assign mux_2level_tapbuf_size16_24_inbus[2] = chanx_1__1__midout_20_;
assign mux_2level_tapbuf_size16_24_inbus[3] = chanx_1__1__midout_21_;
assign mux_2level_tapbuf_size16_24_inbus[4] = chanx_1__1__midout_32_;
assign mux_2level_tapbuf_size16_24_inbus[5] = chanx_1__1__midout_33_;
assign mux_2level_tapbuf_size16_24_inbus[6] = chanx_1__1__midout_44_;
assign mux_2level_tapbuf_size16_24_inbus[7] = chanx_1__1__midout_45_;
assign mux_2level_tapbuf_size16_24_inbus[8] = chanx_1__1__midout_62_;
assign mux_2level_tapbuf_size16_24_inbus[9] = chanx_1__1__midout_63_;
assign mux_2level_tapbuf_size16_24_inbus[10] = chanx_1__1__midout_72_;
assign mux_2level_tapbuf_size16_24_inbus[11] = chanx_1__1__midout_73_;
assign mux_2level_tapbuf_size16_24_inbus[12] = chanx_1__1__midout_84_;
assign mux_2level_tapbuf_size16_24_inbus[13] = chanx_1__1__midout_85_;
assign mux_2level_tapbuf_size16_24_inbus[14] = chanx_1__1__midout_96_;
assign mux_2level_tapbuf_size16_24_inbus[15] = chanx_1__1__midout_97_;
wire [632:639] mux_2level_tapbuf_size16_24_configbus0;
wire [632:639] mux_2level_tapbuf_size16_24_configbus1;
wire [632:639] mux_2level_tapbuf_size16_24_sram_blwl_out ;
wire [632:639] mux_2level_tapbuf_size16_24_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_24_configbus0[632:639] = sram_blwl_bl[632:639] ;
assign mux_2level_tapbuf_size16_24_configbus1[632:639] = sram_blwl_wl[632:639] ;
wire [632:639] mux_2level_tapbuf_size16_24_configbus0_b;
assign mux_2level_tapbuf_size16_24_configbus0_b[632:639] = sram_blwl_blb[632:639] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_24_ (mux_2level_tapbuf_size16_24_inbus, grid_1__2__pin_0__2__12_, mux_2level_tapbuf_size16_24_sram_blwl_out[632:639] ,
mux_2level_tapbuf_size16_24_sram_blwl_outb[632:639] );
//----- SRAM bits for MUX[24], level=2, select_path_id=2. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10000010-----
sram6T_blwl sram_blwl_632_ (mux_2level_tapbuf_size16_24_sram_blwl_out[632:632] ,mux_2level_tapbuf_size16_24_sram_blwl_out[632:632] ,mux_2level_tapbuf_size16_24_sram_blwl_outb[632:632] ,mux_2level_tapbuf_size16_24_configbus0[632:632], mux_2level_tapbuf_size16_24_configbus1[632:632] , mux_2level_tapbuf_size16_24_configbus0_b[632:632] );
sram6T_blwl sram_blwl_633_ (mux_2level_tapbuf_size16_24_sram_blwl_out[633:633] ,mux_2level_tapbuf_size16_24_sram_blwl_out[633:633] ,mux_2level_tapbuf_size16_24_sram_blwl_outb[633:633] ,mux_2level_tapbuf_size16_24_configbus0[633:633], mux_2level_tapbuf_size16_24_configbus1[633:633] , mux_2level_tapbuf_size16_24_configbus0_b[633:633] );
sram6T_blwl sram_blwl_634_ (mux_2level_tapbuf_size16_24_sram_blwl_out[634:634] ,mux_2level_tapbuf_size16_24_sram_blwl_out[634:634] ,mux_2level_tapbuf_size16_24_sram_blwl_outb[634:634] ,mux_2level_tapbuf_size16_24_configbus0[634:634], mux_2level_tapbuf_size16_24_configbus1[634:634] , mux_2level_tapbuf_size16_24_configbus0_b[634:634] );
sram6T_blwl sram_blwl_635_ (mux_2level_tapbuf_size16_24_sram_blwl_out[635:635] ,mux_2level_tapbuf_size16_24_sram_blwl_out[635:635] ,mux_2level_tapbuf_size16_24_sram_blwl_outb[635:635] ,mux_2level_tapbuf_size16_24_configbus0[635:635], mux_2level_tapbuf_size16_24_configbus1[635:635] , mux_2level_tapbuf_size16_24_configbus0_b[635:635] );
sram6T_blwl sram_blwl_636_ (mux_2level_tapbuf_size16_24_sram_blwl_out[636:636] ,mux_2level_tapbuf_size16_24_sram_blwl_out[636:636] ,mux_2level_tapbuf_size16_24_sram_blwl_outb[636:636] ,mux_2level_tapbuf_size16_24_configbus0[636:636], mux_2level_tapbuf_size16_24_configbus1[636:636] , mux_2level_tapbuf_size16_24_configbus0_b[636:636] );
sram6T_blwl sram_blwl_637_ (mux_2level_tapbuf_size16_24_sram_blwl_out[637:637] ,mux_2level_tapbuf_size16_24_sram_blwl_out[637:637] ,mux_2level_tapbuf_size16_24_sram_blwl_outb[637:637] ,mux_2level_tapbuf_size16_24_configbus0[637:637], mux_2level_tapbuf_size16_24_configbus1[637:637] , mux_2level_tapbuf_size16_24_configbus0_b[637:637] );
sram6T_blwl sram_blwl_638_ (mux_2level_tapbuf_size16_24_sram_blwl_out[638:638] ,mux_2level_tapbuf_size16_24_sram_blwl_out[638:638] ,mux_2level_tapbuf_size16_24_sram_blwl_outb[638:638] ,mux_2level_tapbuf_size16_24_configbus0[638:638], mux_2level_tapbuf_size16_24_configbus1[638:638] , mux_2level_tapbuf_size16_24_configbus0_b[638:638] );
sram6T_blwl sram_blwl_639_ (mux_2level_tapbuf_size16_24_sram_blwl_out[639:639] ,mux_2level_tapbuf_size16_24_sram_blwl_out[639:639] ,mux_2level_tapbuf_size16_24_sram_blwl_outb[639:639] ,mux_2level_tapbuf_size16_24_configbus0[639:639], mux_2level_tapbuf_size16_24_configbus1[639:639] , mux_2level_tapbuf_size16_24_configbus0_b[639:639] );
wire [0:15] mux_2level_tapbuf_size16_25_inbus;
assign mux_2level_tapbuf_size16_25_inbus[0] = chanx_1__1__midout_8_;
assign mux_2level_tapbuf_size16_25_inbus[1] = chanx_1__1__midout_9_;
assign mux_2level_tapbuf_size16_25_inbus[2] = chanx_1__1__midout_30_;
assign mux_2level_tapbuf_size16_25_inbus[3] = chanx_1__1__midout_31_;
assign mux_2level_tapbuf_size16_25_inbus[4] = chanx_1__1__midout_34_;
assign mux_2level_tapbuf_size16_25_inbus[5] = chanx_1__1__midout_35_;
assign mux_2level_tapbuf_size16_25_inbus[6] = chanx_1__1__midout_50_;
assign mux_2level_tapbuf_size16_25_inbus[7] = chanx_1__1__midout_51_;
assign mux_2level_tapbuf_size16_25_inbus[8] = chanx_1__1__midout_62_;
assign mux_2level_tapbuf_size16_25_inbus[9] = chanx_1__1__midout_63_;
assign mux_2level_tapbuf_size16_25_inbus[10] = chanx_1__1__midout_74_;
assign mux_2level_tapbuf_size16_25_inbus[11] = chanx_1__1__midout_75_;
assign mux_2level_tapbuf_size16_25_inbus[12] = chanx_1__1__midout_86_;
assign mux_2level_tapbuf_size16_25_inbus[13] = chanx_1__1__midout_87_;
assign mux_2level_tapbuf_size16_25_inbus[14] = chanx_1__1__midout_98_;
assign mux_2level_tapbuf_size16_25_inbus[15] = chanx_1__1__midout_99_;
wire [640:647] mux_2level_tapbuf_size16_25_configbus0;
wire [640:647] mux_2level_tapbuf_size16_25_configbus1;
wire [640:647] mux_2level_tapbuf_size16_25_sram_blwl_out ;
wire [640:647] mux_2level_tapbuf_size16_25_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_25_configbus0[640:647] = sram_blwl_bl[640:647] ;
assign mux_2level_tapbuf_size16_25_configbus1[640:647] = sram_blwl_wl[640:647] ;
wire [640:647] mux_2level_tapbuf_size16_25_configbus0_b;
assign mux_2level_tapbuf_size16_25_configbus0_b[640:647] = sram_blwl_blb[640:647] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_25_ (mux_2level_tapbuf_size16_25_inbus, grid_1__2__pin_0__2__14_, mux_2level_tapbuf_size16_25_sram_blwl_out[640:647] ,
mux_2level_tapbuf_size16_25_sram_blwl_outb[640:647] );
//----- SRAM bits for MUX[25], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_640_ (mux_2level_tapbuf_size16_25_sram_blwl_out[640:640] ,mux_2level_tapbuf_size16_25_sram_blwl_out[640:640] ,mux_2level_tapbuf_size16_25_sram_blwl_outb[640:640] ,mux_2level_tapbuf_size16_25_configbus0[640:640], mux_2level_tapbuf_size16_25_configbus1[640:640] , mux_2level_tapbuf_size16_25_configbus0_b[640:640] );
sram6T_blwl sram_blwl_641_ (mux_2level_tapbuf_size16_25_sram_blwl_out[641:641] ,mux_2level_tapbuf_size16_25_sram_blwl_out[641:641] ,mux_2level_tapbuf_size16_25_sram_blwl_outb[641:641] ,mux_2level_tapbuf_size16_25_configbus0[641:641], mux_2level_tapbuf_size16_25_configbus1[641:641] , mux_2level_tapbuf_size16_25_configbus0_b[641:641] );
sram6T_blwl sram_blwl_642_ (mux_2level_tapbuf_size16_25_sram_blwl_out[642:642] ,mux_2level_tapbuf_size16_25_sram_blwl_out[642:642] ,mux_2level_tapbuf_size16_25_sram_blwl_outb[642:642] ,mux_2level_tapbuf_size16_25_configbus0[642:642], mux_2level_tapbuf_size16_25_configbus1[642:642] , mux_2level_tapbuf_size16_25_configbus0_b[642:642] );
sram6T_blwl sram_blwl_643_ (mux_2level_tapbuf_size16_25_sram_blwl_out[643:643] ,mux_2level_tapbuf_size16_25_sram_blwl_out[643:643] ,mux_2level_tapbuf_size16_25_sram_blwl_outb[643:643] ,mux_2level_tapbuf_size16_25_configbus0[643:643], mux_2level_tapbuf_size16_25_configbus1[643:643] , mux_2level_tapbuf_size16_25_configbus0_b[643:643] );
sram6T_blwl sram_blwl_644_ (mux_2level_tapbuf_size16_25_sram_blwl_out[644:644] ,mux_2level_tapbuf_size16_25_sram_blwl_out[644:644] ,mux_2level_tapbuf_size16_25_sram_blwl_outb[644:644] ,mux_2level_tapbuf_size16_25_configbus0[644:644], mux_2level_tapbuf_size16_25_configbus1[644:644] , mux_2level_tapbuf_size16_25_configbus0_b[644:644] );
sram6T_blwl sram_blwl_645_ (mux_2level_tapbuf_size16_25_sram_blwl_out[645:645] ,mux_2level_tapbuf_size16_25_sram_blwl_out[645:645] ,mux_2level_tapbuf_size16_25_sram_blwl_outb[645:645] ,mux_2level_tapbuf_size16_25_configbus0[645:645], mux_2level_tapbuf_size16_25_configbus1[645:645] , mux_2level_tapbuf_size16_25_configbus0_b[645:645] );
sram6T_blwl sram_blwl_646_ (mux_2level_tapbuf_size16_25_sram_blwl_out[646:646] ,mux_2level_tapbuf_size16_25_sram_blwl_out[646:646] ,mux_2level_tapbuf_size16_25_sram_blwl_outb[646:646] ,mux_2level_tapbuf_size16_25_configbus0[646:646], mux_2level_tapbuf_size16_25_configbus1[646:646] , mux_2level_tapbuf_size16_25_configbus0_b[646:646] );
sram6T_blwl sram_blwl_647_ (mux_2level_tapbuf_size16_25_sram_blwl_out[647:647] ,mux_2level_tapbuf_size16_25_sram_blwl_out[647:647] ,mux_2level_tapbuf_size16_25_sram_blwl_outb[647:647] ,mux_2level_tapbuf_size16_25_configbus0[647:647], mux_2level_tapbuf_size16_25_configbus1[647:647] , mux_2level_tapbuf_size16_25_configbus0_b[647:647] );
wire [0:15] mux_2level_tapbuf_size16_26_inbus;
assign mux_2level_tapbuf_size16_26_inbus[0] = chanx_1__1__midout_6_;
assign mux_2level_tapbuf_size16_26_inbus[1] = chanx_1__1__midout_7_;
assign mux_2level_tapbuf_size16_26_inbus[2] = chanx_1__1__midout_10_;
assign mux_2level_tapbuf_size16_26_inbus[3] = chanx_1__1__midout_11_;
assign mux_2level_tapbuf_size16_26_inbus[4] = chanx_1__1__midout_30_;
assign mux_2level_tapbuf_size16_26_inbus[5] = chanx_1__1__midout_31_;
assign mux_2level_tapbuf_size16_26_inbus[6] = chanx_1__1__midout_34_;
assign mux_2level_tapbuf_size16_26_inbus[7] = chanx_1__1__midout_35_;
assign mux_2level_tapbuf_size16_26_inbus[8] = chanx_1__1__midout_48_;
assign mux_2level_tapbuf_size16_26_inbus[9] = chanx_1__1__midout_49_;
assign mux_2level_tapbuf_size16_26_inbus[10] = chanx_1__1__midout_60_;
assign mux_2level_tapbuf_size16_26_inbus[11] = chanx_1__1__midout_61_;
assign mux_2level_tapbuf_size16_26_inbus[12] = chanx_1__1__midout_74_;
assign mux_2level_tapbuf_size16_26_inbus[13] = chanx_1__1__midout_75_;
assign mux_2level_tapbuf_size16_26_inbus[14] = chanx_1__1__midout_86_;
assign mux_2level_tapbuf_size16_26_inbus[15] = chanx_1__1__midout_87_;
wire [648:655] mux_2level_tapbuf_size16_26_configbus0;
wire [648:655] mux_2level_tapbuf_size16_26_configbus1;
wire [648:655] mux_2level_tapbuf_size16_26_sram_blwl_out ;
wire [648:655] mux_2level_tapbuf_size16_26_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_26_configbus0[648:655] = sram_blwl_bl[648:655] ;
assign mux_2level_tapbuf_size16_26_configbus1[648:655] = sram_blwl_wl[648:655] ;
wire [648:655] mux_2level_tapbuf_size16_26_configbus0_b;
assign mux_2level_tapbuf_size16_26_configbus0_b[648:655] = sram_blwl_blb[648:655] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_26_ (mux_2level_tapbuf_size16_26_inbus, grid_1__1__pin_0__0__0_, mux_2level_tapbuf_size16_26_sram_blwl_out[648:655] ,
mux_2level_tapbuf_size16_26_sram_blwl_outb[648:655] );
//----- SRAM bits for MUX[26], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_648_ (mux_2level_tapbuf_size16_26_sram_blwl_out[648:648] ,mux_2level_tapbuf_size16_26_sram_blwl_out[648:648] ,mux_2level_tapbuf_size16_26_sram_blwl_outb[648:648] ,mux_2level_tapbuf_size16_26_configbus0[648:648], mux_2level_tapbuf_size16_26_configbus1[648:648] , mux_2level_tapbuf_size16_26_configbus0_b[648:648] );
sram6T_blwl sram_blwl_649_ (mux_2level_tapbuf_size16_26_sram_blwl_out[649:649] ,mux_2level_tapbuf_size16_26_sram_blwl_out[649:649] ,mux_2level_tapbuf_size16_26_sram_blwl_outb[649:649] ,mux_2level_tapbuf_size16_26_configbus0[649:649], mux_2level_tapbuf_size16_26_configbus1[649:649] , mux_2level_tapbuf_size16_26_configbus0_b[649:649] );
sram6T_blwl sram_blwl_650_ (mux_2level_tapbuf_size16_26_sram_blwl_out[650:650] ,mux_2level_tapbuf_size16_26_sram_blwl_out[650:650] ,mux_2level_tapbuf_size16_26_sram_blwl_outb[650:650] ,mux_2level_tapbuf_size16_26_configbus0[650:650], mux_2level_tapbuf_size16_26_configbus1[650:650] , mux_2level_tapbuf_size16_26_configbus0_b[650:650] );
sram6T_blwl sram_blwl_651_ (mux_2level_tapbuf_size16_26_sram_blwl_out[651:651] ,mux_2level_tapbuf_size16_26_sram_blwl_out[651:651] ,mux_2level_tapbuf_size16_26_sram_blwl_outb[651:651] ,mux_2level_tapbuf_size16_26_configbus0[651:651], mux_2level_tapbuf_size16_26_configbus1[651:651] , mux_2level_tapbuf_size16_26_configbus0_b[651:651] );
sram6T_blwl sram_blwl_652_ (mux_2level_tapbuf_size16_26_sram_blwl_out[652:652] ,mux_2level_tapbuf_size16_26_sram_blwl_out[652:652] ,mux_2level_tapbuf_size16_26_sram_blwl_outb[652:652] ,mux_2level_tapbuf_size16_26_configbus0[652:652], mux_2level_tapbuf_size16_26_configbus1[652:652] , mux_2level_tapbuf_size16_26_configbus0_b[652:652] );
sram6T_blwl sram_blwl_653_ (mux_2level_tapbuf_size16_26_sram_blwl_out[653:653] ,mux_2level_tapbuf_size16_26_sram_blwl_out[653:653] ,mux_2level_tapbuf_size16_26_sram_blwl_outb[653:653] ,mux_2level_tapbuf_size16_26_configbus0[653:653], mux_2level_tapbuf_size16_26_configbus1[653:653] , mux_2level_tapbuf_size16_26_configbus0_b[653:653] );
sram6T_blwl sram_blwl_654_ (mux_2level_tapbuf_size16_26_sram_blwl_out[654:654] ,mux_2level_tapbuf_size16_26_sram_blwl_out[654:654] ,mux_2level_tapbuf_size16_26_sram_blwl_outb[654:654] ,mux_2level_tapbuf_size16_26_configbus0[654:654], mux_2level_tapbuf_size16_26_configbus1[654:654] , mux_2level_tapbuf_size16_26_configbus0_b[654:654] );
sram6T_blwl sram_blwl_655_ (mux_2level_tapbuf_size16_26_sram_blwl_out[655:655] ,mux_2level_tapbuf_size16_26_sram_blwl_out[655:655] ,mux_2level_tapbuf_size16_26_sram_blwl_outb[655:655] ,mux_2level_tapbuf_size16_26_configbus0[655:655], mux_2level_tapbuf_size16_26_configbus1[655:655] , mux_2level_tapbuf_size16_26_configbus0_b[655:655] );
wire [0:15] mux_2level_tapbuf_size16_27_inbus;
assign mux_2level_tapbuf_size16_27_inbus[0] = chanx_1__1__midout_6_;
assign mux_2level_tapbuf_size16_27_inbus[1] = chanx_1__1__midout_7_;
assign mux_2level_tapbuf_size16_27_inbus[2] = chanx_1__1__midout_10_;
assign mux_2level_tapbuf_size16_27_inbus[3] = chanx_1__1__midout_11_;
assign mux_2level_tapbuf_size16_27_inbus[4] = chanx_1__1__midout_24_;
assign mux_2level_tapbuf_size16_27_inbus[5] = chanx_1__1__midout_25_;
assign mux_2level_tapbuf_size16_27_inbus[6] = chanx_1__1__midout_36_;
assign mux_2level_tapbuf_size16_27_inbus[7] = chanx_1__1__midout_37_;
assign mux_2level_tapbuf_size16_27_inbus[8] = chanx_1__1__midout_48_;
assign mux_2level_tapbuf_size16_27_inbus[9] = chanx_1__1__midout_49_;
assign mux_2level_tapbuf_size16_27_inbus[10] = chanx_1__1__midout_60_;
assign mux_2level_tapbuf_size16_27_inbus[11] = chanx_1__1__midout_61_;
assign mux_2level_tapbuf_size16_27_inbus[12] = chanx_1__1__midout_76_;
assign mux_2level_tapbuf_size16_27_inbus[13] = chanx_1__1__midout_77_;
assign mux_2level_tapbuf_size16_27_inbus[14] = chanx_1__1__midout_88_;
assign mux_2level_tapbuf_size16_27_inbus[15] = chanx_1__1__midout_89_;
wire [656:663] mux_2level_tapbuf_size16_27_configbus0;
wire [656:663] mux_2level_tapbuf_size16_27_configbus1;
wire [656:663] mux_2level_tapbuf_size16_27_sram_blwl_out ;
wire [656:663] mux_2level_tapbuf_size16_27_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_27_configbus0[656:663] = sram_blwl_bl[656:663] ;
assign mux_2level_tapbuf_size16_27_configbus1[656:663] = sram_blwl_wl[656:663] ;
wire [656:663] mux_2level_tapbuf_size16_27_configbus0_b;
assign mux_2level_tapbuf_size16_27_configbus0_b[656:663] = sram_blwl_blb[656:663] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_27_ (mux_2level_tapbuf_size16_27_inbus, grid_1__1__pin_0__0__4_, mux_2level_tapbuf_size16_27_sram_blwl_out[656:663] ,
mux_2level_tapbuf_size16_27_sram_blwl_outb[656:663] );
//----- SRAM bits for MUX[27], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_656_ (mux_2level_tapbuf_size16_27_sram_blwl_out[656:656] ,mux_2level_tapbuf_size16_27_sram_blwl_out[656:656] ,mux_2level_tapbuf_size16_27_sram_blwl_outb[656:656] ,mux_2level_tapbuf_size16_27_configbus0[656:656], mux_2level_tapbuf_size16_27_configbus1[656:656] , mux_2level_tapbuf_size16_27_configbus0_b[656:656] );
sram6T_blwl sram_blwl_657_ (mux_2level_tapbuf_size16_27_sram_blwl_out[657:657] ,mux_2level_tapbuf_size16_27_sram_blwl_out[657:657] ,mux_2level_tapbuf_size16_27_sram_blwl_outb[657:657] ,mux_2level_tapbuf_size16_27_configbus0[657:657], mux_2level_tapbuf_size16_27_configbus1[657:657] , mux_2level_tapbuf_size16_27_configbus0_b[657:657] );
sram6T_blwl sram_blwl_658_ (mux_2level_tapbuf_size16_27_sram_blwl_out[658:658] ,mux_2level_tapbuf_size16_27_sram_blwl_out[658:658] ,mux_2level_tapbuf_size16_27_sram_blwl_outb[658:658] ,mux_2level_tapbuf_size16_27_configbus0[658:658], mux_2level_tapbuf_size16_27_configbus1[658:658] , mux_2level_tapbuf_size16_27_configbus0_b[658:658] );
sram6T_blwl sram_blwl_659_ (mux_2level_tapbuf_size16_27_sram_blwl_out[659:659] ,mux_2level_tapbuf_size16_27_sram_blwl_out[659:659] ,mux_2level_tapbuf_size16_27_sram_blwl_outb[659:659] ,mux_2level_tapbuf_size16_27_configbus0[659:659], mux_2level_tapbuf_size16_27_configbus1[659:659] , mux_2level_tapbuf_size16_27_configbus0_b[659:659] );
sram6T_blwl sram_blwl_660_ (mux_2level_tapbuf_size16_27_sram_blwl_out[660:660] ,mux_2level_tapbuf_size16_27_sram_blwl_out[660:660] ,mux_2level_tapbuf_size16_27_sram_blwl_outb[660:660] ,mux_2level_tapbuf_size16_27_configbus0[660:660], mux_2level_tapbuf_size16_27_configbus1[660:660] , mux_2level_tapbuf_size16_27_configbus0_b[660:660] );
sram6T_blwl sram_blwl_661_ (mux_2level_tapbuf_size16_27_sram_blwl_out[661:661] ,mux_2level_tapbuf_size16_27_sram_blwl_out[661:661] ,mux_2level_tapbuf_size16_27_sram_blwl_outb[661:661] ,mux_2level_tapbuf_size16_27_configbus0[661:661], mux_2level_tapbuf_size16_27_configbus1[661:661] , mux_2level_tapbuf_size16_27_configbus0_b[661:661] );
sram6T_blwl sram_blwl_662_ (mux_2level_tapbuf_size16_27_sram_blwl_out[662:662] ,mux_2level_tapbuf_size16_27_sram_blwl_out[662:662] ,mux_2level_tapbuf_size16_27_sram_blwl_outb[662:662] ,mux_2level_tapbuf_size16_27_configbus0[662:662], mux_2level_tapbuf_size16_27_configbus1[662:662] , mux_2level_tapbuf_size16_27_configbus0_b[662:662] );
sram6T_blwl sram_blwl_663_ (mux_2level_tapbuf_size16_27_sram_blwl_out[663:663] ,mux_2level_tapbuf_size16_27_sram_blwl_out[663:663] ,mux_2level_tapbuf_size16_27_sram_blwl_outb[663:663] ,mux_2level_tapbuf_size16_27_configbus0[663:663], mux_2level_tapbuf_size16_27_configbus1[663:663] , mux_2level_tapbuf_size16_27_configbus0_b[663:663] );
wire [0:15] mux_2level_tapbuf_size16_28_inbus;
assign mux_2level_tapbuf_size16_28_inbus[0] = chanx_1__1__midout_0_;
assign mux_2level_tapbuf_size16_28_inbus[1] = chanx_1__1__midout_1_;
assign mux_2level_tapbuf_size16_28_inbus[2] = chanx_1__1__midout_12_;
assign mux_2level_tapbuf_size16_28_inbus[3] = chanx_1__1__midout_13_;
assign mux_2level_tapbuf_size16_28_inbus[4] = chanx_1__1__midout_24_;
assign mux_2level_tapbuf_size16_28_inbus[5] = chanx_1__1__midout_25_;
assign mux_2level_tapbuf_size16_28_inbus[6] = chanx_1__1__midout_42_;
assign mux_2level_tapbuf_size16_28_inbus[7] = chanx_1__1__midout_43_;
assign mux_2level_tapbuf_size16_28_inbus[8] = chanx_1__1__midout_54_;
assign mux_2level_tapbuf_size16_28_inbus[9] = chanx_1__1__midout_55_;
assign mux_2level_tapbuf_size16_28_inbus[10] = chanx_1__1__midout_66_;
assign mux_2level_tapbuf_size16_28_inbus[11] = chanx_1__1__midout_67_;
assign mux_2level_tapbuf_size16_28_inbus[12] = chanx_1__1__midout_76_;
assign mux_2level_tapbuf_size16_28_inbus[13] = chanx_1__1__midout_77_;
assign mux_2level_tapbuf_size16_28_inbus[14] = chanx_1__1__midout_90_;
assign mux_2level_tapbuf_size16_28_inbus[15] = chanx_1__1__midout_91_;
wire [664:671] mux_2level_tapbuf_size16_28_configbus0;
wire [664:671] mux_2level_tapbuf_size16_28_configbus1;
wire [664:671] mux_2level_tapbuf_size16_28_sram_blwl_out ;
wire [664:671] mux_2level_tapbuf_size16_28_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_28_configbus0[664:671] = sram_blwl_bl[664:671] ;
assign mux_2level_tapbuf_size16_28_configbus1[664:671] = sram_blwl_wl[664:671] ;
wire [664:671] mux_2level_tapbuf_size16_28_configbus0_b;
assign mux_2level_tapbuf_size16_28_configbus0_b[664:671] = sram_blwl_blb[664:671] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_28_ (mux_2level_tapbuf_size16_28_inbus, grid_1__1__pin_0__0__8_, mux_2level_tapbuf_size16_28_sram_blwl_out[664:671] ,
mux_2level_tapbuf_size16_28_sram_blwl_outb[664:671] );
//----- SRAM bits for MUX[28], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_664_ (mux_2level_tapbuf_size16_28_sram_blwl_out[664:664] ,mux_2level_tapbuf_size16_28_sram_blwl_out[664:664] ,mux_2level_tapbuf_size16_28_sram_blwl_outb[664:664] ,mux_2level_tapbuf_size16_28_configbus0[664:664], mux_2level_tapbuf_size16_28_configbus1[664:664] , mux_2level_tapbuf_size16_28_configbus0_b[664:664] );
sram6T_blwl sram_blwl_665_ (mux_2level_tapbuf_size16_28_sram_blwl_out[665:665] ,mux_2level_tapbuf_size16_28_sram_blwl_out[665:665] ,mux_2level_tapbuf_size16_28_sram_blwl_outb[665:665] ,mux_2level_tapbuf_size16_28_configbus0[665:665], mux_2level_tapbuf_size16_28_configbus1[665:665] , mux_2level_tapbuf_size16_28_configbus0_b[665:665] );
sram6T_blwl sram_blwl_666_ (mux_2level_tapbuf_size16_28_sram_blwl_out[666:666] ,mux_2level_tapbuf_size16_28_sram_blwl_out[666:666] ,mux_2level_tapbuf_size16_28_sram_blwl_outb[666:666] ,mux_2level_tapbuf_size16_28_configbus0[666:666], mux_2level_tapbuf_size16_28_configbus1[666:666] , mux_2level_tapbuf_size16_28_configbus0_b[666:666] );
sram6T_blwl sram_blwl_667_ (mux_2level_tapbuf_size16_28_sram_blwl_out[667:667] ,mux_2level_tapbuf_size16_28_sram_blwl_out[667:667] ,mux_2level_tapbuf_size16_28_sram_blwl_outb[667:667] ,mux_2level_tapbuf_size16_28_configbus0[667:667], mux_2level_tapbuf_size16_28_configbus1[667:667] , mux_2level_tapbuf_size16_28_configbus0_b[667:667] );
sram6T_blwl sram_blwl_668_ (mux_2level_tapbuf_size16_28_sram_blwl_out[668:668] ,mux_2level_tapbuf_size16_28_sram_blwl_out[668:668] ,mux_2level_tapbuf_size16_28_sram_blwl_outb[668:668] ,mux_2level_tapbuf_size16_28_configbus0[668:668], mux_2level_tapbuf_size16_28_configbus1[668:668] , mux_2level_tapbuf_size16_28_configbus0_b[668:668] );
sram6T_blwl sram_blwl_669_ (mux_2level_tapbuf_size16_28_sram_blwl_out[669:669] ,mux_2level_tapbuf_size16_28_sram_blwl_out[669:669] ,mux_2level_tapbuf_size16_28_sram_blwl_outb[669:669] ,mux_2level_tapbuf_size16_28_configbus0[669:669], mux_2level_tapbuf_size16_28_configbus1[669:669] , mux_2level_tapbuf_size16_28_configbus0_b[669:669] );
sram6T_blwl sram_blwl_670_ (mux_2level_tapbuf_size16_28_sram_blwl_out[670:670] ,mux_2level_tapbuf_size16_28_sram_blwl_out[670:670] ,mux_2level_tapbuf_size16_28_sram_blwl_outb[670:670] ,mux_2level_tapbuf_size16_28_configbus0[670:670], mux_2level_tapbuf_size16_28_configbus1[670:670] , mux_2level_tapbuf_size16_28_configbus0_b[670:670] );
sram6T_blwl sram_blwl_671_ (mux_2level_tapbuf_size16_28_sram_blwl_out[671:671] ,mux_2level_tapbuf_size16_28_sram_blwl_out[671:671] ,mux_2level_tapbuf_size16_28_sram_blwl_outb[671:671] ,mux_2level_tapbuf_size16_28_configbus0[671:671], mux_2level_tapbuf_size16_28_configbus1[671:671] , mux_2level_tapbuf_size16_28_configbus0_b[671:671] );
wire [0:15] mux_2level_tapbuf_size16_29_inbus;
assign mux_2level_tapbuf_size16_29_inbus[0] = chanx_1__1__midout_0_;
assign mux_2level_tapbuf_size16_29_inbus[1] = chanx_1__1__midout_1_;
assign mux_2level_tapbuf_size16_29_inbus[2] = chanx_1__1__midout_22_;
assign mux_2level_tapbuf_size16_29_inbus[3] = chanx_1__1__midout_23_;
assign mux_2level_tapbuf_size16_29_inbus[4] = chanx_1__1__midout_26_;
assign mux_2level_tapbuf_size16_29_inbus[5] = chanx_1__1__midout_27_;
assign mux_2level_tapbuf_size16_29_inbus[6] = chanx_1__1__midout_42_;
assign mux_2level_tapbuf_size16_29_inbus[7] = chanx_1__1__midout_43_;
assign mux_2level_tapbuf_size16_29_inbus[8] = chanx_1__1__midout_54_;
assign mux_2level_tapbuf_size16_29_inbus[9] = chanx_1__1__midout_55_;
assign mux_2level_tapbuf_size16_29_inbus[10] = chanx_1__1__midout_64_;
assign mux_2level_tapbuf_size16_29_inbus[11] = chanx_1__1__midout_65_;
assign mux_2level_tapbuf_size16_29_inbus[12] = chanx_1__1__midout_78_;
assign mux_2level_tapbuf_size16_29_inbus[13] = chanx_1__1__midout_79_;
assign mux_2level_tapbuf_size16_29_inbus[14] = chanx_1__1__midout_90_;
assign mux_2level_tapbuf_size16_29_inbus[15] = chanx_1__1__midout_91_;
wire [672:679] mux_2level_tapbuf_size16_29_configbus0;
wire [672:679] mux_2level_tapbuf_size16_29_configbus1;
wire [672:679] mux_2level_tapbuf_size16_29_sram_blwl_out ;
wire [672:679] mux_2level_tapbuf_size16_29_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_29_configbus0[672:679] = sram_blwl_bl[672:679] ;
assign mux_2level_tapbuf_size16_29_configbus1[672:679] = sram_blwl_wl[672:679] ;
wire [672:679] mux_2level_tapbuf_size16_29_configbus0_b;
assign mux_2level_tapbuf_size16_29_configbus0_b[672:679] = sram_blwl_blb[672:679] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_29_ (mux_2level_tapbuf_size16_29_inbus, grid_1__1__pin_0__0__12_, mux_2level_tapbuf_size16_29_sram_blwl_out[672:679] ,
mux_2level_tapbuf_size16_29_sram_blwl_outb[672:679] );
//----- SRAM bits for MUX[29], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_672_ (mux_2level_tapbuf_size16_29_sram_blwl_out[672:672] ,mux_2level_tapbuf_size16_29_sram_blwl_out[672:672] ,mux_2level_tapbuf_size16_29_sram_blwl_outb[672:672] ,mux_2level_tapbuf_size16_29_configbus0[672:672], mux_2level_tapbuf_size16_29_configbus1[672:672] , mux_2level_tapbuf_size16_29_configbus0_b[672:672] );
sram6T_blwl sram_blwl_673_ (mux_2level_tapbuf_size16_29_sram_blwl_out[673:673] ,mux_2level_tapbuf_size16_29_sram_blwl_out[673:673] ,mux_2level_tapbuf_size16_29_sram_blwl_outb[673:673] ,mux_2level_tapbuf_size16_29_configbus0[673:673], mux_2level_tapbuf_size16_29_configbus1[673:673] , mux_2level_tapbuf_size16_29_configbus0_b[673:673] );
sram6T_blwl sram_blwl_674_ (mux_2level_tapbuf_size16_29_sram_blwl_out[674:674] ,mux_2level_tapbuf_size16_29_sram_blwl_out[674:674] ,mux_2level_tapbuf_size16_29_sram_blwl_outb[674:674] ,mux_2level_tapbuf_size16_29_configbus0[674:674], mux_2level_tapbuf_size16_29_configbus1[674:674] , mux_2level_tapbuf_size16_29_configbus0_b[674:674] );
sram6T_blwl sram_blwl_675_ (mux_2level_tapbuf_size16_29_sram_blwl_out[675:675] ,mux_2level_tapbuf_size16_29_sram_blwl_out[675:675] ,mux_2level_tapbuf_size16_29_sram_blwl_outb[675:675] ,mux_2level_tapbuf_size16_29_configbus0[675:675], mux_2level_tapbuf_size16_29_configbus1[675:675] , mux_2level_tapbuf_size16_29_configbus0_b[675:675] );
sram6T_blwl sram_blwl_676_ (mux_2level_tapbuf_size16_29_sram_blwl_out[676:676] ,mux_2level_tapbuf_size16_29_sram_blwl_out[676:676] ,mux_2level_tapbuf_size16_29_sram_blwl_outb[676:676] ,mux_2level_tapbuf_size16_29_configbus0[676:676], mux_2level_tapbuf_size16_29_configbus1[676:676] , mux_2level_tapbuf_size16_29_configbus0_b[676:676] );
sram6T_blwl sram_blwl_677_ (mux_2level_tapbuf_size16_29_sram_blwl_out[677:677] ,mux_2level_tapbuf_size16_29_sram_blwl_out[677:677] ,mux_2level_tapbuf_size16_29_sram_blwl_outb[677:677] ,mux_2level_tapbuf_size16_29_configbus0[677:677], mux_2level_tapbuf_size16_29_configbus1[677:677] , mux_2level_tapbuf_size16_29_configbus0_b[677:677] );
sram6T_blwl sram_blwl_678_ (mux_2level_tapbuf_size16_29_sram_blwl_out[678:678] ,mux_2level_tapbuf_size16_29_sram_blwl_out[678:678] ,mux_2level_tapbuf_size16_29_sram_blwl_outb[678:678] ,mux_2level_tapbuf_size16_29_configbus0[678:678], mux_2level_tapbuf_size16_29_configbus1[678:678] , mux_2level_tapbuf_size16_29_configbus0_b[678:678] );
sram6T_blwl sram_blwl_679_ (mux_2level_tapbuf_size16_29_sram_blwl_out[679:679] ,mux_2level_tapbuf_size16_29_sram_blwl_out[679:679] ,mux_2level_tapbuf_size16_29_sram_blwl_outb[679:679] ,mux_2level_tapbuf_size16_29_configbus0[679:679], mux_2level_tapbuf_size16_29_configbus1[679:679] , mux_2level_tapbuf_size16_29_configbus0_b[679:679] );
wire [0:15] mux_2level_tapbuf_size16_30_inbus;
assign mux_2level_tapbuf_size16_30_inbus[0] = chanx_1__1__midout_2_;
assign mux_2level_tapbuf_size16_30_inbus[1] = chanx_1__1__midout_3_;
assign mux_2level_tapbuf_size16_30_inbus[2] = chanx_1__1__midout_22_;
assign mux_2level_tapbuf_size16_30_inbus[3] = chanx_1__1__midout_23_;
assign mux_2level_tapbuf_size16_30_inbus[4] = chanx_1__1__midout_28_;
assign mux_2level_tapbuf_size16_30_inbus[5] = chanx_1__1__midout_29_;
assign mux_2level_tapbuf_size16_30_inbus[6] = chanx_1__1__midout_40_;
assign mux_2level_tapbuf_size16_30_inbus[7] = chanx_1__1__midout_41_;
assign mux_2level_tapbuf_size16_30_inbus[8] = chanx_1__1__midout_52_;
assign mux_2level_tapbuf_size16_30_inbus[9] = chanx_1__1__midout_53_;
assign mux_2level_tapbuf_size16_30_inbus[10] = chanx_1__1__midout_64_;
assign mux_2level_tapbuf_size16_30_inbus[11] = chanx_1__1__midout_65_;
assign mux_2level_tapbuf_size16_30_inbus[12] = chanx_1__1__midout_80_;
assign mux_2level_tapbuf_size16_30_inbus[13] = chanx_1__1__midout_81_;
assign mux_2level_tapbuf_size16_30_inbus[14] = chanx_1__1__midout_92_;
assign mux_2level_tapbuf_size16_30_inbus[15] = chanx_1__1__midout_93_;
wire [680:687] mux_2level_tapbuf_size16_30_configbus0;
wire [680:687] mux_2level_tapbuf_size16_30_configbus1;
wire [680:687] mux_2level_tapbuf_size16_30_sram_blwl_out ;
wire [680:687] mux_2level_tapbuf_size16_30_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_30_configbus0[680:687] = sram_blwl_bl[680:687] ;
assign mux_2level_tapbuf_size16_30_configbus1[680:687] = sram_blwl_wl[680:687] ;
wire [680:687] mux_2level_tapbuf_size16_30_configbus0_b;
assign mux_2level_tapbuf_size16_30_configbus0_b[680:687] = sram_blwl_blb[680:687] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_30_ (mux_2level_tapbuf_size16_30_inbus, grid_1__1__pin_0__0__16_, mux_2level_tapbuf_size16_30_sram_blwl_out[680:687] ,
mux_2level_tapbuf_size16_30_sram_blwl_outb[680:687] );
//----- SRAM bits for MUX[30], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_680_ (mux_2level_tapbuf_size16_30_sram_blwl_out[680:680] ,mux_2level_tapbuf_size16_30_sram_blwl_out[680:680] ,mux_2level_tapbuf_size16_30_sram_blwl_outb[680:680] ,mux_2level_tapbuf_size16_30_configbus0[680:680], mux_2level_tapbuf_size16_30_configbus1[680:680] , mux_2level_tapbuf_size16_30_configbus0_b[680:680] );
sram6T_blwl sram_blwl_681_ (mux_2level_tapbuf_size16_30_sram_blwl_out[681:681] ,mux_2level_tapbuf_size16_30_sram_blwl_out[681:681] ,mux_2level_tapbuf_size16_30_sram_blwl_outb[681:681] ,mux_2level_tapbuf_size16_30_configbus0[681:681], mux_2level_tapbuf_size16_30_configbus1[681:681] , mux_2level_tapbuf_size16_30_configbus0_b[681:681] );
sram6T_blwl sram_blwl_682_ (mux_2level_tapbuf_size16_30_sram_blwl_out[682:682] ,mux_2level_tapbuf_size16_30_sram_blwl_out[682:682] ,mux_2level_tapbuf_size16_30_sram_blwl_outb[682:682] ,mux_2level_tapbuf_size16_30_configbus0[682:682], mux_2level_tapbuf_size16_30_configbus1[682:682] , mux_2level_tapbuf_size16_30_configbus0_b[682:682] );
sram6T_blwl sram_blwl_683_ (mux_2level_tapbuf_size16_30_sram_blwl_out[683:683] ,mux_2level_tapbuf_size16_30_sram_blwl_out[683:683] ,mux_2level_tapbuf_size16_30_sram_blwl_outb[683:683] ,mux_2level_tapbuf_size16_30_configbus0[683:683], mux_2level_tapbuf_size16_30_configbus1[683:683] , mux_2level_tapbuf_size16_30_configbus0_b[683:683] );
sram6T_blwl sram_blwl_684_ (mux_2level_tapbuf_size16_30_sram_blwl_out[684:684] ,mux_2level_tapbuf_size16_30_sram_blwl_out[684:684] ,mux_2level_tapbuf_size16_30_sram_blwl_outb[684:684] ,mux_2level_tapbuf_size16_30_configbus0[684:684], mux_2level_tapbuf_size16_30_configbus1[684:684] , mux_2level_tapbuf_size16_30_configbus0_b[684:684] );
sram6T_blwl sram_blwl_685_ (mux_2level_tapbuf_size16_30_sram_blwl_out[685:685] ,mux_2level_tapbuf_size16_30_sram_blwl_out[685:685] ,mux_2level_tapbuf_size16_30_sram_blwl_outb[685:685] ,mux_2level_tapbuf_size16_30_configbus0[685:685], mux_2level_tapbuf_size16_30_configbus1[685:685] , mux_2level_tapbuf_size16_30_configbus0_b[685:685] );
sram6T_blwl sram_blwl_686_ (mux_2level_tapbuf_size16_30_sram_blwl_out[686:686] ,mux_2level_tapbuf_size16_30_sram_blwl_out[686:686] ,mux_2level_tapbuf_size16_30_sram_blwl_outb[686:686] ,mux_2level_tapbuf_size16_30_configbus0[686:686], mux_2level_tapbuf_size16_30_configbus1[686:686] , mux_2level_tapbuf_size16_30_configbus0_b[686:686] );
sram6T_blwl sram_blwl_687_ (mux_2level_tapbuf_size16_30_sram_blwl_out[687:687] ,mux_2level_tapbuf_size16_30_sram_blwl_out[687:687] ,mux_2level_tapbuf_size16_30_sram_blwl_outb[687:687] ,mux_2level_tapbuf_size16_30_configbus0[687:687], mux_2level_tapbuf_size16_30_configbus1[687:687] , mux_2level_tapbuf_size16_30_configbus0_b[687:687] );
wire [0:15] mux_2level_tapbuf_size16_31_inbus;
assign mux_2level_tapbuf_size16_31_inbus[0] = chanx_1__1__midout_4_;
assign mux_2level_tapbuf_size16_31_inbus[1] = chanx_1__1__midout_5_;
assign mux_2level_tapbuf_size16_31_inbus[2] = chanx_1__1__midout_16_;
assign mux_2level_tapbuf_size16_31_inbus[3] = chanx_1__1__midout_17_;
assign mux_2level_tapbuf_size16_31_inbus[4] = chanx_1__1__midout_28_;
assign mux_2level_tapbuf_size16_31_inbus[5] = chanx_1__1__midout_29_;
assign mux_2level_tapbuf_size16_31_inbus[6] = chanx_1__1__midout_40_;
assign mux_2level_tapbuf_size16_31_inbus[7] = chanx_1__1__midout_41_;
assign mux_2level_tapbuf_size16_31_inbus[8] = chanx_1__1__midout_58_;
assign mux_2level_tapbuf_size16_31_inbus[9] = chanx_1__1__midout_59_;
assign mux_2level_tapbuf_size16_31_inbus[10] = chanx_1__1__midout_68_;
assign mux_2level_tapbuf_size16_31_inbus[11] = chanx_1__1__midout_69_;
assign mux_2level_tapbuf_size16_31_inbus[12] = chanx_1__1__midout_80_;
assign mux_2level_tapbuf_size16_31_inbus[13] = chanx_1__1__midout_81_;
assign mux_2level_tapbuf_size16_31_inbus[14] = chanx_1__1__midout_92_;
assign mux_2level_tapbuf_size16_31_inbus[15] = chanx_1__1__midout_93_;
wire [688:695] mux_2level_tapbuf_size16_31_configbus0;
wire [688:695] mux_2level_tapbuf_size16_31_configbus1;
wire [688:695] mux_2level_tapbuf_size16_31_sram_blwl_out ;
wire [688:695] mux_2level_tapbuf_size16_31_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_31_configbus0[688:695] = sram_blwl_bl[688:695] ;
assign mux_2level_tapbuf_size16_31_configbus1[688:695] = sram_blwl_wl[688:695] ;
wire [688:695] mux_2level_tapbuf_size16_31_configbus0_b;
assign mux_2level_tapbuf_size16_31_configbus0_b[688:695] = sram_blwl_blb[688:695] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_31_ (mux_2level_tapbuf_size16_31_inbus, grid_1__1__pin_0__0__20_, mux_2level_tapbuf_size16_31_sram_blwl_out[688:695] ,
mux_2level_tapbuf_size16_31_sram_blwl_outb[688:695] );
//----- SRAM bits for MUX[31], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_688_ (mux_2level_tapbuf_size16_31_sram_blwl_out[688:688] ,mux_2level_tapbuf_size16_31_sram_blwl_out[688:688] ,mux_2level_tapbuf_size16_31_sram_blwl_outb[688:688] ,mux_2level_tapbuf_size16_31_configbus0[688:688], mux_2level_tapbuf_size16_31_configbus1[688:688] , mux_2level_tapbuf_size16_31_configbus0_b[688:688] );
sram6T_blwl sram_blwl_689_ (mux_2level_tapbuf_size16_31_sram_blwl_out[689:689] ,mux_2level_tapbuf_size16_31_sram_blwl_out[689:689] ,mux_2level_tapbuf_size16_31_sram_blwl_outb[689:689] ,mux_2level_tapbuf_size16_31_configbus0[689:689], mux_2level_tapbuf_size16_31_configbus1[689:689] , mux_2level_tapbuf_size16_31_configbus0_b[689:689] );
sram6T_blwl sram_blwl_690_ (mux_2level_tapbuf_size16_31_sram_blwl_out[690:690] ,mux_2level_tapbuf_size16_31_sram_blwl_out[690:690] ,mux_2level_tapbuf_size16_31_sram_blwl_outb[690:690] ,mux_2level_tapbuf_size16_31_configbus0[690:690], mux_2level_tapbuf_size16_31_configbus1[690:690] , mux_2level_tapbuf_size16_31_configbus0_b[690:690] );
sram6T_blwl sram_blwl_691_ (mux_2level_tapbuf_size16_31_sram_blwl_out[691:691] ,mux_2level_tapbuf_size16_31_sram_blwl_out[691:691] ,mux_2level_tapbuf_size16_31_sram_blwl_outb[691:691] ,mux_2level_tapbuf_size16_31_configbus0[691:691], mux_2level_tapbuf_size16_31_configbus1[691:691] , mux_2level_tapbuf_size16_31_configbus0_b[691:691] );
sram6T_blwl sram_blwl_692_ (mux_2level_tapbuf_size16_31_sram_blwl_out[692:692] ,mux_2level_tapbuf_size16_31_sram_blwl_out[692:692] ,mux_2level_tapbuf_size16_31_sram_blwl_outb[692:692] ,mux_2level_tapbuf_size16_31_configbus0[692:692], mux_2level_tapbuf_size16_31_configbus1[692:692] , mux_2level_tapbuf_size16_31_configbus0_b[692:692] );
sram6T_blwl sram_blwl_693_ (mux_2level_tapbuf_size16_31_sram_blwl_out[693:693] ,mux_2level_tapbuf_size16_31_sram_blwl_out[693:693] ,mux_2level_tapbuf_size16_31_sram_blwl_outb[693:693] ,mux_2level_tapbuf_size16_31_configbus0[693:693], mux_2level_tapbuf_size16_31_configbus1[693:693] , mux_2level_tapbuf_size16_31_configbus0_b[693:693] );
sram6T_blwl sram_blwl_694_ (mux_2level_tapbuf_size16_31_sram_blwl_out[694:694] ,mux_2level_tapbuf_size16_31_sram_blwl_out[694:694] ,mux_2level_tapbuf_size16_31_sram_blwl_outb[694:694] ,mux_2level_tapbuf_size16_31_configbus0[694:694], mux_2level_tapbuf_size16_31_configbus1[694:694] , mux_2level_tapbuf_size16_31_configbus0_b[694:694] );
sram6T_blwl sram_blwl_695_ (mux_2level_tapbuf_size16_31_sram_blwl_out[695:695] ,mux_2level_tapbuf_size16_31_sram_blwl_out[695:695] ,mux_2level_tapbuf_size16_31_sram_blwl_outb[695:695] ,mux_2level_tapbuf_size16_31_configbus0[695:695], mux_2level_tapbuf_size16_31_configbus1[695:695] , mux_2level_tapbuf_size16_31_configbus0_b[695:695] );
wire [0:15] mux_2level_tapbuf_size16_32_inbus;
assign mux_2level_tapbuf_size16_32_inbus[0] = chanx_1__1__midout_4_;
assign mux_2level_tapbuf_size16_32_inbus[1] = chanx_1__1__midout_5_;
assign mux_2level_tapbuf_size16_32_inbus[2] = chanx_1__1__midout_18_;
assign mux_2level_tapbuf_size16_32_inbus[3] = chanx_1__1__midout_19_;
assign mux_2level_tapbuf_size16_32_inbus[4] = chanx_1__1__midout_38_;
assign mux_2level_tapbuf_size16_32_inbus[5] = chanx_1__1__midout_39_;
assign mux_2level_tapbuf_size16_32_inbus[6] = chanx_1__1__midout_46_;
assign mux_2level_tapbuf_size16_32_inbus[7] = chanx_1__1__midout_47_;
assign mux_2level_tapbuf_size16_32_inbus[8] = chanx_1__1__midout_58_;
assign mux_2level_tapbuf_size16_32_inbus[9] = chanx_1__1__midout_59_;
assign mux_2level_tapbuf_size16_32_inbus[10] = chanx_1__1__midout_70_;
assign mux_2level_tapbuf_size16_32_inbus[11] = chanx_1__1__midout_71_;
assign mux_2level_tapbuf_size16_32_inbus[12] = chanx_1__1__midout_82_;
assign mux_2level_tapbuf_size16_32_inbus[13] = chanx_1__1__midout_83_;
assign mux_2level_tapbuf_size16_32_inbus[14] = chanx_1__1__midout_94_;
assign mux_2level_tapbuf_size16_32_inbus[15] = chanx_1__1__midout_95_;
wire [696:703] mux_2level_tapbuf_size16_32_configbus0;
wire [696:703] mux_2level_tapbuf_size16_32_configbus1;
wire [696:703] mux_2level_tapbuf_size16_32_sram_blwl_out ;
wire [696:703] mux_2level_tapbuf_size16_32_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_32_configbus0[696:703] = sram_blwl_bl[696:703] ;
assign mux_2level_tapbuf_size16_32_configbus1[696:703] = sram_blwl_wl[696:703] ;
wire [696:703] mux_2level_tapbuf_size16_32_configbus0_b;
assign mux_2level_tapbuf_size16_32_configbus0_b[696:703] = sram_blwl_blb[696:703] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_32_ (mux_2level_tapbuf_size16_32_inbus, grid_1__1__pin_0__0__24_, mux_2level_tapbuf_size16_32_sram_blwl_out[696:703] ,
mux_2level_tapbuf_size16_32_sram_blwl_outb[696:703] );
//----- SRAM bits for MUX[32], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_696_ (mux_2level_tapbuf_size16_32_sram_blwl_out[696:696] ,mux_2level_tapbuf_size16_32_sram_blwl_out[696:696] ,mux_2level_tapbuf_size16_32_sram_blwl_outb[696:696] ,mux_2level_tapbuf_size16_32_configbus0[696:696], mux_2level_tapbuf_size16_32_configbus1[696:696] , mux_2level_tapbuf_size16_32_configbus0_b[696:696] );
sram6T_blwl sram_blwl_697_ (mux_2level_tapbuf_size16_32_sram_blwl_out[697:697] ,mux_2level_tapbuf_size16_32_sram_blwl_out[697:697] ,mux_2level_tapbuf_size16_32_sram_blwl_outb[697:697] ,mux_2level_tapbuf_size16_32_configbus0[697:697], mux_2level_tapbuf_size16_32_configbus1[697:697] , mux_2level_tapbuf_size16_32_configbus0_b[697:697] );
sram6T_blwl sram_blwl_698_ (mux_2level_tapbuf_size16_32_sram_blwl_out[698:698] ,mux_2level_tapbuf_size16_32_sram_blwl_out[698:698] ,mux_2level_tapbuf_size16_32_sram_blwl_outb[698:698] ,mux_2level_tapbuf_size16_32_configbus0[698:698], mux_2level_tapbuf_size16_32_configbus1[698:698] , mux_2level_tapbuf_size16_32_configbus0_b[698:698] );
sram6T_blwl sram_blwl_699_ (mux_2level_tapbuf_size16_32_sram_blwl_out[699:699] ,mux_2level_tapbuf_size16_32_sram_blwl_out[699:699] ,mux_2level_tapbuf_size16_32_sram_blwl_outb[699:699] ,mux_2level_tapbuf_size16_32_configbus0[699:699], mux_2level_tapbuf_size16_32_configbus1[699:699] , mux_2level_tapbuf_size16_32_configbus0_b[699:699] );
sram6T_blwl sram_blwl_700_ (mux_2level_tapbuf_size16_32_sram_blwl_out[700:700] ,mux_2level_tapbuf_size16_32_sram_blwl_out[700:700] ,mux_2level_tapbuf_size16_32_sram_blwl_outb[700:700] ,mux_2level_tapbuf_size16_32_configbus0[700:700], mux_2level_tapbuf_size16_32_configbus1[700:700] , mux_2level_tapbuf_size16_32_configbus0_b[700:700] );
sram6T_blwl sram_blwl_701_ (mux_2level_tapbuf_size16_32_sram_blwl_out[701:701] ,mux_2level_tapbuf_size16_32_sram_blwl_out[701:701] ,mux_2level_tapbuf_size16_32_sram_blwl_outb[701:701] ,mux_2level_tapbuf_size16_32_configbus0[701:701], mux_2level_tapbuf_size16_32_configbus1[701:701] , mux_2level_tapbuf_size16_32_configbus0_b[701:701] );
sram6T_blwl sram_blwl_702_ (mux_2level_tapbuf_size16_32_sram_blwl_out[702:702] ,mux_2level_tapbuf_size16_32_sram_blwl_out[702:702] ,mux_2level_tapbuf_size16_32_sram_blwl_outb[702:702] ,mux_2level_tapbuf_size16_32_configbus0[702:702], mux_2level_tapbuf_size16_32_configbus1[702:702] , mux_2level_tapbuf_size16_32_configbus0_b[702:702] );
sram6T_blwl sram_blwl_703_ (mux_2level_tapbuf_size16_32_sram_blwl_out[703:703] ,mux_2level_tapbuf_size16_32_sram_blwl_out[703:703] ,mux_2level_tapbuf_size16_32_sram_blwl_outb[703:703] ,mux_2level_tapbuf_size16_32_configbus0[703:703], mux_2level_tapbuf_size16_32_configbus1[703:703] , mux_2level_tapbuf_size16_32_configbus0_b[703:703] );
wire [0:15] mux_2level_tapbuf_size16_33_inbus;
assign mux_2level_tapbuf_size16_33_inbus[0] = chanx_1__1__midout_14_;
assign mux_2level_tapbuf_size16_33_inbus[1] = chanx_1__1__midout_15_;
assign mux_2level_tapbuf_size16_33_inbus[2] = chanx_1__1__midout_18_;
assign mux_2level_tapbuf_size16_33_inbus[3] = chanx_1__1__midout_19_;
assign mux_2level_tapbuf_size16_33_inbus[4] = chanx_1__1__midout_38_;
assign mux_2level_tapbuf_size16_33_inbus[5] = chanx_1__1__midout_39_;
assign mux_2level_tapbuf_size16_33_inbus[6] = chanx_1__1__midout_44_;
assign mux_2level_tapbuf_size16_33_inbus[7] = chanx_1__1__midout_45_;
assign mux_2level_tapbuf_size16_33_inbus[8] = chanx_1__1__midout_56_;
assign mux_2level_tapbuf_size16_33_inbus[9] = chanx_1__1__midout_57_;
assign mux_2level_tapbuf_size16_33_inbus[10] = chanx_1__1__midout_70_;
assign mux_2level_tapbuf_size16_33_inbus[11] = chanx_1__1__midout_71_;
assign mux_2level_tapbuf_size16_33_inbus[12] = chanx_1__1__midout_82_;
assign mux_2level_tapbuf_size16_33_inbus[13] = chanx_1__1__midout_83_;
assign mux_2level_tapbuf_size16_33_inbus[14] = chanx_1__1__midout_96_;
assign mux_2level_tapbuf_size16_33_inbus[15] = chanx_1__1__midout_97_;
wire [704:711] mux_2level_tapbuf_size16_33_configbus0;
wire [704:711] mux_2level_tapbuf_size16_33_configbus1;
wire [704:711] mux_2level_tapbuf_size16_33_sram_blwl_out ;
wire [704:711] mux_2level_tapbuf_size16_33_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_33_configbus0[704:711] = sram_blwl_bl[704:711] ;
assign mux_2level_tapbuf_size16_33_configbus1[704:711] = sram_blwl_wl[704:711] ;
wire [704:711] mux_2level_tapbuf_size16_33_configbus0_b;
assign mux_2level_tapbuf_size16_33_configbus0_b[704:711] = sram_blwl_blb[704:711] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_33_ (mux_2level_tapbuf_size16_33_inbus, grid_1__1__pin_0__0__28_, mux_2level_tapbuf_size16_33_sram_blwl_out[704:711] ,
mux_2level_tapbuf_size16_33_sram_blwl_outb[704:711] );
//----- SRAM bits for MUX[33], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_704_ (mux_2level_tapbuf_size16_33_sram_blwl_out[704:704] ,mux_2level_tapbuf_size16_33_sram_blwl_out[704:704] ,mux_2level_tapbuf_size16_33_sram_blwl_outb[704:704] ,mux_2level_tapbuf_size16_33_configbus0[704:704], mux_2level_tapbuf_size16_33_configbus1[704:704] , mux_2level_tapbuf_size16_33_configbus0_b[704:704] );
sram6T_blwl sram_blwl_705_ (mux_2level_tapbuf_size16_33_sram_blwl_out[705:705] ,mux_2level_tapbuf_size16_33_sram_blwl_out[705:705] ,mux_2level_tapbuf_size16_33_sram_blwl_outb[705:705] ,mux_2level_tapbuf_size16_33_configbus0[705:705], mux_2level_tapbuf_size16_33_configbus1[705:705] , mux_2level_tapbuf_size16_33_configbus0_b[705:705] );
sram6T_blwl sram_blwl_706_ (mux_2level_tapbuf_size16_33_sram_blwl_out[706:706] ,mux_2level_tapbuf_size16_33_sram_blwl_out[706:706] ,mux_2level_tapbuf_size16_33_sram_blwl_outb[706:706] ,mux_2level_tapbuf_size16_33_configbus0[706:706], mux_2level_tapbuf_size16_33_configbus1[706:706] , mux_2level_tapbuf_size16_33_configbus0_b[706:706] );
sram6T_blwl sram_blwl_707_ (mux_2level_tapbuf_size16_33_sram_blwl_out[707:707] ,mux_2level_tapbuf_size16_33_sram_blwl_out[707:707] ,mux_2level_tapbuf_size16_33_sram_blwl_outb[707:707] ,mux_2level_tapbuf_size16_33_configbus0[707:707], mux_2level_tapbuf_size16_33_configbus1[707:707] , mux_2level_tapbuf_size16_33_configbus0_b[707:707] );
sram6T_blwl sram_blwl_708_ (mux_2level_tapbuf_size16_33_sram_blwl_out[708:708] ,mux_2level_tapbuf_size16_33_sram_blwl_out[708:708] ,mux_2level_tapbuf_size16_33_sram_blwl_outb[708:708] ,mux_2level_tapbuf_size16_33_configbus0[708:708], mux_2level_tapbuf_size16_33_configbus1[708:708] , mux_2level_tapbuf_size16_33_configbus0_b[708:708] );
sram6T_blwl sram_blwl_709_ (mux_2level_tapbuf_size16_33_sram_blwl_out[709:709] ,mux_2level_tapbuf_size16_33_sram_blwl_out[709:709] ,mux_2level_tapbuf_size16_33_sram_blwl_outb[709:709] ,mux_2level_tapbuf_size16_33_configbus0[709:709], mux_2level_tapbuf_size16_33_configbus1[709:709] , mux_2level_tapbuf_size16_33_configbus0_b[709:709] );
sram6T_blwl sram_blwl_710_ (mux_2level_tapbuf_size16_33_sram_blwl_out[710:710] ,mux_2level_tapbuf_size16_33_sram_blwl_out[710:710] ,mux_2level_tapbuf_size16_33_sram_blwl_outb[710:710] ,mux_2level_tapbuf_size16_33_configbus0[710:710], mux_2level_tapbuf_size16_33_configbus1[710:710] , mux_2level_tapbuf_size16_33_configbus0_b[710:710] );
sram6T_blwl sram_blwl_711_ (mux_2level_tapbuf_size16_33_sram_blwl_out[711:711] ,mux_2level_tapbuf_size16_33_sram_blwl_out[711:711] ,mux_2level_tapbuf_size16_33_sram_blwl_outb[711:711] ,mux_2level_tapbuf_size16_33_configbus0[711:711], mux_2level_tapbuf_size16_33_configbus1[711:711] , mux_2level_tapbuf_size16_33_configbus0_b[711:711] );
wire [0:15] mux_2level_tapbuf_size16_34_inbus;
assign mux_2level_tapbuf_size16_34_inbus[0] = chanx_1__1__midout_8_;
assign mux_2level_tapbuf_size16_34_inbus[1] = chanx_1__1__midout_9_;
assign mux_2level_tapbuf_size16_34_inbus[2] = chanx_1__1__midout_20_;
assign mux_2level_tapbuf_size16_34_inbus[3] = chanx_1__1__midout_21_;
assign mux_2level_tapbuf_size16_34_inbus[4] = chanx_1__1__midout_32_;
assign mux_2level_tapbuf_size16_34_inbus[5] = chanx_1__1__midout_33_;
assign mux_2level_tapbuf_size16_34_inbus[6] = chanx_1__1__midout_44_;
assign mux_2level_tapbuf_size16_34_inbus[7] = chanx_1__1__midout_45_;
assign mux_2level_tapbuf_size16_34_inbus[8] = chanx_1__1__midout_62_;
assign mux_2level_tapbuf_size16_34_inbus[9] = chanx_1__1__midout_63_;
assign mux_2level_tapbuf_size16_34_inbus[10] = chanx_1__1__midout_72_;
assign mux_2level_tapbuf_size16_34_inbus[11] = chanx_1__1__midout_73_;
assign mux_2level_tapbuf_size16_34_inbus[12] = chanx_1__1__midout_84_;
assign mux_2level_tapbuf_size16_34_inbus[13] = chanx_1__1__midout_85_;
assign mux_2level_tapbuf_size16_34_inbus[14] = chanx_1__1__midout_96_;
assign mux_2level_tapbuf_size16_34_inbus[15] = chanx_1__1__midout_97_;
wire [712:719] mux_2level_tapbuf_size16_34_configbus0;
wire [712:719] mux_2level_tapbuf_size16_34_configbus1;
wire [712:719] mux_2level_tapbuf_size16_34_sram_blwl_out ;
wire [712:719] mux_2level_tapbuf_size16_34_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_34_configbus0[712:719] = sram_blwl_bl[712:719] ;
assign mux_2level_tapbuf_size16_34_configbus1[712:719] = sram_blwl_wl[712:719] ;
wire [712:719] mux_2level_tapbuf_size16_34_configbus0_b;
assign mux_2level_tapbuf_size16_34_configbus0_b[712:719] = sram_blwl_blb[712:719] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_34_ (mux_2level_tapbuf_size16_34_inbus, grid_1__1__pin_0__0__32_, mux_2level_tapbuf_size16_34_sram_blwl_out[712:719] ,
mux_2level_tapbuf_size16_34_sram_blwl_outb[712:719] );
//----- SRAM bits for MUX[34], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_712_ (mux_2level_tapbuf_size16_34_sram_blwl_out[712:712] ,mux_2level_tapbuf_size16_34_sram_blwl_out[712:712] ,mux_2level_tapbuf_size16_34_sram_blwl_outb[712:712] ,mux_2level_tapbuf_size16_34_configbus0[712:712], mux_2level_tapbuf_size16_34_configbus1[712:712] , mux_2level_tapbuf_size16_34_configbus0_b[712:712] );
sram6T_blwl sram_blwl_713_ (mux_2level_tapbuf_size16_34_sram_blwl_out[713:713] ,mux_2level_tapbuf_size16_34_sram_blwl_out[713:713] ,mux_2level_tapbuf_size16_34_sram_blwl_outb[713:713] ,mux_2level_tapbuf_size16_34_configbus0[713:713], mux_2level_tapbuf_size16_34_configbus1[713:713] , mux_2level_tapbuf_size16_34_configbus0_b[713:713] );
sram6T_blwl sram_blwl_714_ (mux_2level_tapbuf_size16_34_sram_blwl_out[714:714] ,mux_2level_tapbuf_size16_34_sram_blwl_out[714:714] ,mux_2level_tapbuf_size16_34_sram_blwl_outb[714:714] ,mux_2level_tapbuf_size16_34_configbus0[714:714], mux_2level_tapbuf_size16_34_configbus1[714:714] , mux_2level_tapbuf_size16_34_configbus0_b[714:714] );
sram6T_blwl sram_blwl_715_ (mux_2level_tapbuf_size16_34_sram_blwl_out[715:715] ,mux_2level_tapbuf_size16_34_sram_blwl_out[715:715] ,mux_2level_tapbuf_size16_34_sram_blwl_outb[715:715] ,mux_2level_tapbuf_size16_34_configbus0[715:715], mux_2level_tapbuf_size16_34_configbus1[715:715] , mux_2level_tapbuf_size16_34_configbus0_b[715:715] );
sram6T_blwl sram_blwl_716_ (mux_2level_tapbuf_size16_34_sram_blwl_out[716:716] ,mux_2level_tapbuf_size16_34_sram_blwl_out[716:716] ,mux_2level_tapbuf_size16_34_sram_blwl_outb[716:716] ,mux_2level_tapbuf_size16_34_configbus0[716:716], mux_2level_tapbuf_size16_34_configbus1[716:716] , mux_2level_tapbuf_size16_34_configbus0_b[716:716] );
sram6T_blwl sram_blwl_717_ (mux_2level_tapbuf_size16_34_sram_blwl_out[717:717] ,mux_2level_tapbuf_size16_34_sram_blwl_out[717:717] ,mux_2level_tapbuf_size16_34_sram_blwl_outb[717:717] ,mux_2level_tapbuf_size16_34_configbus0[717:717], mux_2level_tapbuf_size16_34_configbus1[717:717] , mux_2level_tapbuf_size16_34_configbus0_b[717:717] );
sram6T_blwl sram_blwl_718_ (mux_2level_tapbuf_size16_34_sram_blwl_out[718:718] ,mux_2level_tapbuf_size16_34_sram_blwl_out[718:718] ,mux_2level_tapbuf_size16_34_sram_blwl_outb[718:718] ,mux_2level_tapbuf_size16_34_configbus0[718:718], mux_2level_tapbuf_size16_34_configbus1[718:718] , mux_2level_tapbuf_size16_34_configbus0_b[718:718] );
sram6T_blwl sram_blwl_719_ (mux_2level_tapbuf_size16_34_sram_blwl_out[719:719] ,mux_2level_tapbuf_size16_34_sram_blwl_out[719:719] ,mux_2level_tapbuf_size16_34_sram_blwl_outb[719:719] ,mux_2level_tapbuf_size16_34_configbus0[719:719], mux_2level_tapbuf_size16_34_configbus1[719:719] , mux_2level_tapbuf_size16_34_configbus0_b[719:719] );
wire [0:15] mux_2level_tapbuf_size16_35_inbus;
assign mux_2level_tapbuf_size16_35_inbus[0] = chanx_1__1__midout_8_;
assign mux_2level_tapbuf_size16_35_inbus[1] = chanx_1__1__midout_9_;
assign mux_2level_tapbuf_size16_35_inbus[2] = chanx_1__1__midout_20_;
assign mux_2level_tapbuf_size16_35_inbus[3] = chanx_1__1__midout_21_;
assign mux_2level_tapbuf_size16_35_inbus[4] = chanx_1__1__midout_34_;
assign mux_2level_tapbuf_size16_35_inbus[5] = chanx_1__1__midout_35_;
assign mux_2level_tapbuf_size16_35_inbus[6] = chanx_1__1__midout_50_;
assign mux_2level_tapbuf_size16_35_inbus[7] = chanx_1__1__midout_51_;
assign mux_2level_tapbuf_size16_35_inbus[8] = chanx_1__1__midout_62_;
assign mux_2level_tapbuf_size16_35_inbus[9] = chanx_1__1__midout_63_;
assign mux_2level_tapbuf_size16_35_inbus[10] = chanx_1__1__midout_72_;
assign mux_2level_tapbuf_size16_35_inbus[11] = chanx_1__1__midout_73_;
assign mux_2level_tapbuf_size16_35_inbus[12] = chanx_1__1__midout_86_;
assign mux_2level_tapbuf_size16_35_inbus[13] = chanx_1__1__midout_87_;
assign mux_2level_tapbuf_size16_35_inbus[14] = chanx_1__1__midout_98_;
assign mux_2level_tapbuf_size16_35_inbus[15] = chanx_1__1__midout_99_;
wire [720:727] mux_2level_tapbuf_size16_35_configbus0;
wire [720:727] mux_2level_tapbuf_size16_35_configbus1;
wire [720:727] mux_2level_tapbuf_size16_35_sram_blwl_out ;
wire [720:727] mux_2level_tapbuf_size16_35_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_35_configbus0[720:727] = sram_blwl_bl[720:727] ;
assign mux_2level_tapbuf_size16_35_configbus1[720:727] = sram_blwl_wl[720:727] ;
wire [720:727] mux_2level_tapbuf_size16_35_configbus0_b;
assign mux_2level_tapbuf_size16_35_configbus0_b[720:727] = sram_blwl_blb[720:727] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_35_ (mux_2level_tapbuf_size16_35_inbus, grid_1__1__pin_0__0__36_, mux_2level_tapbuf_size16_35_sram_blwl_out[720:727] ,
mux_2level_tapbuf_size16_35_sram_blwl_outb[720:727] );
//----- SRAM bits for MUX[35], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_720_ (mux_2level_tapbuf_size16_35_sram_blwl_out[720:720] ,mux_2level_tapbuf_size16_35_sram_blwl_out[720:720] ,mux_2level_tapbuf_size16_35_sram_blwl_outb[720:720] ,mux_2level_tapbuf_size16_35_configbus0[720:720], mux_2level_tapbuf_size16_35_configbus1[720:720] , mux_2level_tapbuf_size16_35_configbus0_b[720:720] );
sram6T_blwl sram_blwl_721_ (mux_2level_tapbuf_size16_35_sram_blwl_out[721:721] ,mux_2level_tapbuf_size16_35_sram_blwl_out[721:721] ,mux_2level_tapbuf_size16_35_sram_blwl_outb[721:721] ,mux_2level_tapbuf_size16_35_configbus0[721:721], mux_2level_tapbuf_size16_35_configbus1[721:721] , mux_2level_tapbuf_size16_35_configbus0_b[721:721] );
sram6T_blwl sram_blwl_722_ (mux_2level_tapbuf_size16_35_sram_blwl_out[722:722] ,mux_2level_tapbuf_size16_35_sram_blwl_out[722:722] ,mux_2level_tapbuf_size16_35_sram_blwl_outb[722:722] ,mux_2level_tapbuf_size16_35_configbus0[722:722], mux_2level_tapbuf_size16_35_configbus1[722:722] , mux_2level_tapbuf_size16_35_configbus0_b[722:722] );
sram6T_blwl sram_blwl_723_ (mux_2level_tapbuf_size16_35_sram_blwl_out[723:723] ,mux_2level_tapbuf_size16_35_sram_blwl_out[723:723] ,mux_2level_tapbuf_size16_35_sram_blwl_outb[723:723] ,mux_2level_tapbuf_size16_35_configbus0[723:723], mux_2level_tapbuf_size16_35_configbus1[723:723] , mux_2level_tapbuf_size16_35_configbus0_b[723:723] );
sram6T_blwl sram_blwl_724_ (mux_2level_tapbuf_size16_35_sram_blwl_out[724:724] ,mux_2level_tapbuf_size16_35_sram_blwl_out[724:724] ,mux_2level_tapbuf_size16_35_sram_blwl_outb[724:724] ,mux_2level_tapbuf_size16_35_configbus0[724:724], mux_2level_tapbuf_size16_35_configbus1[724:724] , mux_2level_tapbuf_size16_35_configbus0_b[724:724] );
sram6T_blwl sram_blwl_725_ (mux_2level_tapbuf_size16_35_sram_blwl_out[725:725] ,mux_2level_tapbuf_size16_35_sram_blwl_out[725:725] ,mux_2level_tapbuf_size16_35_sram_blwl_outb[725:725] ,mux_2level_tapbuf_size16_35_configbus0[725:725], mux_2level_tapbuf_size16_35_configbus1[725:725] , mux_2level_tapbuf_size16_35_configbus0_b[725:725] );
sram6T_blwl sram_blwl_726_ (mux_2level_tapbuf_size16_35_sram_blwl_out[726:726] ,mux_2level_tapbuf_size16_35_sram_blwl_out[726:726] ,mux_2level_tapbuf_size16_35_sram_blwl_outb[726:726] ,mux_2level_tapbuf_size16_35_configbus0[726:726], mux_2level_tapbuf_size16_35_configbus1[726:726] , mux_2level_tapbuf_size16_35_configbus0_b[726:726] );
sram6T_blwl sram_blwl_727_ (mux_2level_tapbuf_size16_35_sram_blwl_out[727:727] ,mux_2level_tapbuf_size16_35_sram_blwl_out[727:727] ,mux_2level_tapbuf_size16_35_sram_blwl_outb[727:727] ,mux_2level_tapbuf_size16_35_configbus0[727:727], mux_2level_tapbuf_size16_35_configbus1[727:727] , mux_2level_tapbuf_size16_35_configbus0_b[727:727] );
endmodule
//----- END Verilog Module of Connection Box -X direction [1][1] -----

