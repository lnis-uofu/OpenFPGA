//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Connection Block - Y direction  [0][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Verilog Module of Connection Box -Y direction [0][1] -----
module cby_0__1_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input chany_0__1__midout_0_, 

input chany_0__1__midout_1_, 

input chany_0__1__midout_2_, 

input chany_0__1__midout_3_, 

input chany_0__1__midout_4_, 

input chany_0__1__midout_5_, 

input chany_0__1__midout_6_, 

input chany_0__1__midout_7_, 

input chany_0__1__midout_8_, 

input chany_0__1__midout_9_, 

input chany_0__1__midout_10_, 

input chany_0__1__midout_11_, 

input chany_0__1__midout_12_, 

input chany_0__1__midout_13_, 

input chany_0__1__midout_14_, 

input chany_0__1__midout_15_, 

input chany_0__1__midout_16_, 

input chany_0__1__midout_17_, 

input chany_0__1__midout_18_, 

input chany_0__1__midout_19_, 

input chany_0__1__midout_20_, 

input chany_0__1__midout_21_, 

input chany_0__1__midout_22_, 

input chany_0__1__midout_23_, 

input chany_0__1__midout_24_, 

input chany_0__1__midout_25_, 

input chany_0__1__midout_26_, 

input chany_0__1__midout_27_, 

input chany_0__1__midout_28_, 

input chany_0__1__midout_29_, 

input chany_0__1__midout_30_, 

input chany_0__1__midout_31_, 

input chany_0__1__midout_32_, 

input chany_0__1__midout_33_, 

input chany_0__1__midout_34_, 

input chany_0__1__midout_35_, 

input chany_0__1__midout_36_, 

input chany_0__1__midout_37_, 

input chany_0__1__midout_38_, 

input chany_0__1__midout_39_, 

input chany_0__1__midout_40_, 

input chany_0__1__midout_41_, 

input chany_0__1__midout_42_, 

input chany_0__1__midout_43_, 

input chany_0__1__midout_44_, 

input chany_0__1__midout_45_, 

input chany_0__1__midout_46_, 

input chany_0__1__midout_47_, 

input chany_0__1__midout_48_, 

input chany_0__1__midout_49_, 

input chany_0__1__midout_50_, 

input chany_0__1__midout_51_, 

input chany_0__1__midout_52_, 

input chany_0__1__midout_53_, 

input chany_0__1__midout_54_, 

input chany_0__1__midout_55_, 

input chany_0__1__midout_56_, 

input chany_0__1__midout_57_, 

input chany_0__1__midout_58_, 

input chany_0__1__midout_59_, 

input chany_0__1__midout_60_, 

input chany_0__1__midout_61_, 

input chany_0__1__midout_62_, 

input chany_0__1__midout_63_, 

input chany_0__1__midout_64_, 

input chany_0__1__midout_65_, 

input chany_0__1__midout_66_, 

input chany_0__1__midout_67_, 

input chany_0__1__midout_68_, 

input chany_0__1__midout_69_, 

input chany_0__1__midout_70_, 

input chany_0__1__midout_71_, 

input chany_0__1__midout_72_, 

input chany_0__1__midout_73_, 

input chany_0__1__midout_74_, 

input chany_0__1__midout_75_, 

input chany_0__1__midout_76_, 

input chany_0__1__midout_77_, 

input chany_0__1__midout_78_, 

input chany_0__1__midout_79_, 

input chany_0__1__midout_80_, 

input chany_0__1__midout_81_, 

input chany_0__1__midout_82_, 

input chany_0__1__midout_83_, 

input chany_0__1__midout_84_, 

input chany_0__1__midout_85_, 

input chany_0__1__midout_86_, 

input chany_0__1__midout_87_, 

input chany_0__1__midout_88_, 

input chany_0__1__midout_89_, 

input chany_0__1__midout_90_, 

input chany_0__1__midout_91_, 

input chany_0__1__midout_92_, 

input chany_0__1__midout_93_, 

input chany_0__1__midout_94_, 

input chany_0__1__midout_95_, 

input chany_0__1__midout_96_, 

input chany_0__1__midout_97_, 

input chany_0__1__midout_98_, 

input chany_0__1__midout_99_, 

output  grid_1__1__pin_0__3__3_,

output  grid_1__1__pin_0__3__7_,

output  grid_1__1__pin_0__3__11_,

output  grid_1__1__pin_0__3__15_,

output  grid_1__1__pin_0__3__19_,

output  grid_1__1__pin_0__3__23_,

output  grid_1__1__pin_0__3__27_,

output  grid_1__1__pin_0__3__31_,

output  grid_1__1__pin_0__3__35_,

output  grid_1__1__pin_0__3__39_,

output  grid_0__1__pin_0__1__0_,

output  grid_0__1__pin_0__1__2_,

output  grid_0__1__pin_0__1__4_,

output  grid_0__1__pin_0__1__6_,

output  grid_0__1__pin_0__1__8_,

output  grid_0__1__pin_0__1__10_,

output  grid_0__1__pin_0__1__12_,

output  grid_0__1__pin_0__1__14_,

input [728:871] sram_blwl_bl ,
input [728:871] sram_blwl_wl ,
input [728:871] sram_blwl_blb );
wire [0:15] mux_2level_tapbuf_size16_36_inbus;
assign mux_2level_tapbuf_size16_36_inbus[0] = chany_0__1__midout_0_;
assign mux_2level_tapbuf_size16_36_inbus[1] = chany_0__1__midout_1_;
assign mux_2level_tapbuf_size16_36_inbus[2] = chany_0__1__midout_12_;
assign mux_2level_tapbuf_size16_36_inbus[3] = chany_0__1__midout_13_;
assign mux_2level_tapbuf_size16_36_inbus[4] = chany_0__1__midout_24_;
assign mux_2level_tapbuf_size16_36_inbus[5] = chany_0__1__midout_25_;
assign mux_2level_tapbuf_size16_36_inbus[6] = chany_0__1__midout_38_;
assign mux_2level_tapbuf_size16_36_inbus[7] = chany_0__1__midout_39_;
assign mux_2level_tapbuf_size16_36_inbus[8] = chany_0__1__midout_50_;
assign mux_2level_tapbuf_size16_36_inbus[9] = chany_0__1__midout_51_;
assign mux_2level_tapbuf_size16_36_inbus[10] = chany_0__1__midout_62_;
assign mux_2level_tapbuf_size16_36_inbus[11] = chany_0__1__midout_63_;
assign mux_2level_tapbuf_size16_36_inbus[12] = chany_0__1__midout_74_;
assign mux_2level_tapbuf_size16_36_inbus[13] = chany_0__1__midout_75_;
assign mux_2level_tapbuf_size16_36_inbus[14] = chany_0__1__midout_88_;
assign mux_2level_tapbuf_size16_36_inbus[15] = chany_0__1__midout_89_;
wire [728:735] mux_2level_tapbuf_size16_36_configbus0;
wire [728:735] mux_2level_tapbuf_size16_36_configbus1;
wire [728:735] mux_2level_tapbuf_size16_36_sram_blwl_out ;
wire [728:735] mux_2level_tapbuf_size16_36_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_36_configbus0[728:735] = sram_blwl_bl[728:735] ;
assign mux_2level_tapbuf_size16_36_configbus1[728:735] = sram_blwl_wl[728:735] ;
wire [728:735] mux_2level_tapbuf_size16_36_configbus0_b;
assign mux_2level_tapbuf_size16_36_configbus0_b[728:735] = sram_blwl_blb[728:735] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_36_ (mux_2level_tapbuf_size16_36_inbus, grid_1__1__pin_0__3__3_, mux_2level_tapbuf_size16_36_sram_blwl_out[728:735] ,
mux_2level_tapbuf_size16_36_sram_blwl_outb[728:735] );
//----- SRAM bits for MUX[36], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_728_ (mux_2level_tapbuf_size16_36_sram_blwl_out[728:728] ,mux_2level_tapbuf_size16_36_sram_blwl_out[728:728] ,mux_2level_tapbuf_size16_36_sram_blwl_outb[728:728] ,mux_2level_tapbuf_size16_36_configbus0[728:728], mux_2level_tapbuf_size16_36_configbus1[728:728] , mux_2level_tapbuf_size16_36_configbus0_b[728:728] );
sram6T_blwl sram_blwl_729_ (mux_2level_tapbuf_size16_36_sram_blwl_out[729:729] ,mux_2level_tapbuf_size16_36_sram_blwl_out[729:729] ,mux_2level_tapbuf_size16_36_sram_blwl_outb[729:729] ,mux_2level_tapbuf_size16_36_configbus0[729:729], mux_2level_tapbuf_size16_36_configbus1[729:729] , mux_2level_tapbuf_size16_36_configbus0_b[729:729] );
sram6T_blwl sram_blwl_730_ (mux_2level_tapbuf_size16_36_sram_blwl_out[730:730] ,mux_2level_tapbuf_size16_36_sram_blwl_out[730:730] ,mux_2level_tapbuf_size16_36_sram_blwl_outb[730:730] ,mux_2level_tapbuf_size16_36_configbus0[730:730], mux_2level_tapbuf_size16_36_configbus1[730:730] , mux_2level_tapbuf_size16_36_configbus0_b[730:730] );
sram6T_blwl sram_blwl_731_ (mux_2level_tapbuf_size16_36_sram_blwl_out[731:731] ,mux_2level_tapbuf_size16_36_sram_blwl_out[731:731] ,mux_2level_tapbuf_size16_36_sram_blwl_outb[731:731] ,mux_2level_tapbuf_size16_36_configbus0[731:731], mux_2level_tapbuf_size16_36_configbus1[731:731] , mux_2level_tapbuf_size16_36_configbus0_b[731:731] );
sram6T_blwl sram_blwl_732_ (mux_2level_tapbuf_size16_36_sram_blwl_out[732:732] ,mux_2level_tapbuf_size16_36_sram_blwl_out[732:732] ,mux_2level_tapbuf_size16_36_sram_blwl_outb[732:732] ,mux_2level_tapbuf_size16_36_configbus0[732:732], mux_2level_tapbuf_size16_36_configbus1[732:732] , mux_2level_tapbuf_size16_36_configbus0_b[732:732] );
sram6T_blwl sram_blwl_733_ (mux_2level_tapbuf_size16_36_sram_blwl_out[733:733] ,mux_2level_tapbuf_size16_36_sram_blwl_out[733:733] ,mux_2level_tapbuf_size16_36_sram_blwl_outb[733:733] ,mux_2level_tapbuf_size16_36_configbus0[733:733], mux_2level_tapbuf_size16_36_configbus1[733:733] , mux_2level_tapbuf_size16_36_configbus0_b[733:733] );
sram6T_blwl sram_blwl_734_ (mux_2level_tapbuf_size16_36_sram_blwl_out[734:734] ,mux_2level_tapbuf_size16_36_sram_blwl_out[734:734] ,mux_2level_tapbuf_size16_36_sram_blwl_outb[734:734] ,mux_2level_tapbuf_size16_36_configbus0[734:734], mux_2level_tapbuf_size16_36_configbus1[734:734] , mux_2level_tapbuf_size16_36_configbus0_b[734:734] );
sram6T_blwl sram_blwl_735_ (mux_2level_tapbuf_size16_36_sram_blwl_out[735:735] ,mux_2level_tapbuf_size16_36_sram_blwl_out[735:735] ,mux_2level_tapbuf_size16_36_sram_blwl_outb[735:735] ,mux_2level_tapbuf_size16_36_configbus0[735:735], mux_2level_tapbuf_size16_36_configbus1[735:735] , mux_2level_tapbuf_size16_36_configbus0_b[735:735] );
wire [0:15] mux_2level_tapbuf_size16_37_inbus;
assign mux_2level_tapbuf_size16_37_inbus[0] = chany_0__1__midout_2_;
assign mux_2level_tapbuf_size16_37_inbus[1] = chany_0__1__midout_3_;
assign mux_2level_tapbuf_size16_37_inbus[2] = chany_0__1__midout_14_;
assign mux_2level_tapbuf_size16_37_inbus[3] = chany_0__1__midout_15_;
assign mux_2level_tapbuf_size16_37_inbus[4] = chany_0__1__midout_26_;
assign mux_2level_tapbuf_size16_37_inbus[5] = chany_0__1__midout_27_;
assign mux_2level_tapbuf_size16_37_inbus[6] = chany_0__1__midout_38_;
assign mux_2level_tapbuf_size16_37_inbus[7] = chany_0__1__midout_39_;
assign mux_2level_tapbuf_size16_37_inbus[8] = chany_0__1__midout_52_;
assign mux_2level_tapbuf_size16_37_inbus[9] = chany_0__1__midout_53_;
assign mux_2level_tapbuf_size16_37_inbus[10] = chany_0__1__midout_64_;
assign mux_2level_tapbuf_size16_37_inbus[11] = chany_0__1__midout_65_;
assign mux_2level_tapbuf_size16_37_inbus[12] = chany_0__1__midout_76_;
assign mux_2level_tapbuf_size16_37_inbus[13] = chany_0__1__midout_77_;
assign mux_2level_tapbuf_size16_37_inbus[14] = chany_0__1__midout_88_;
assign mux_2level_tapbuf_size16_37_inbus[15] = chany_0__1__midout_89_;
wire [736:743] mux_2level_tapbuf_size16_37_configbus0;
wire [736:743] mux_2level_tapbuf_size16_37_configbus1;
wire [736:743] mux_2level_tapbuf_size16_37_sram_blwl_out ;
wire [736:743] mux_2level_tapbuf_size16_37_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_37_configbus0[736:743] = sram_blwl_bl[736:743] ;
assign mux_2level_tapbuf_size16_37_configbus1[736:743] = sram_blwl_wl[736:743] ;
wire [736:743] mux_2level_tapbuf_size16_37_configbus0_b;
assign mux_2level_tapbuf_size16_37_configbus0_b[736:743] = sram_blwl_blb[736:743] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_37_ (mux_2level_tapbuf_size16_37_inbus, grid_1__1__pin_0__3__7_, mux_2level_tapbuf_size16_37_sram_blwl_out[736:743] ,
mux_2level_tapbuf_size16_37_sram_blwl_outb[736:743] );
//----- SRAM bits for MUX[37], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_736_ (mux_2level_tapbuf_size16_37_sram_blwl_out[736:736] ,mux_2level_tapbuf_size16_37_sram_blwl_out[736:736] ,mux_2level_tapbuf_size16_37_sram_blwl_outb[736:736] ,mux_2level_tapbuf_size16_37_configbus0[736:736], mux_2level_tapbuf_size16_37_configbus1[736:736] , mux_2level_tapbuf_size16_37_configbus0_b[736:736] );
sram6T_blwl sram_blwl_737_ (mux_2level_tapbuf_size16_37_sram_blwl_out[737:737] ,mux_2level_tapbuf_size16_37_sram_blwl_out[737:737] ,mux_2level_tapbuf_size16_37_sram_blwl_outb[737:737] ,mux_2level_tapbuf_size16_37_configbus0[737:737], mux_2level_tapbuf_size16_37_configbus1[737:737] , mux_2level_tapbuf_size16_37_configbus0_b[737:737] );
sram6T_blwl sram_blwl_738_ (mux_2level_tapbuf_size16_37_sram_blwl_out[738:738] ,mux_2level_tapbuf_size16_37_sram_blwl_out[738:738] ,mux_2level_tapbuf_size16_37_sram_blwl_outb[738:738] ,mux_2level_tapbuf_size16_37_configbus0[738:738], mux_2level_tapbuf_size16_37_configbus1[738:738] , mux_2level_tapbuf_size16_37_configbus0_b[738:738] );
sram6T_blwl sram_blwl_739_ (mux_2level_tapbuf_size16_37_sram_blwl_out[739:739] ,mux_2level_tapbuf_size16_37_sram_blwl_out[739:739] ,mux_2level_tapbuf_size16_37_sram_blwl_outb[739:739] ,mux_2level_tapbuf_size16_37_configbus0[739:739], mux_2level_tapbuf_size16_37_configbus1[739:739] , mux_2level_tapbuf_size16_37_configbus0_b[739:739] );
sram6T_blwl sram_blwl_740_ (mux_2level_tapbuf_size16_37_sram_blwl_out[740:740] ,mux_2level_tapbuf_size16_37_sram_blwl_out[740:740] ,mux_2level_tapbuf_size16_37_sram_blwl_outb[740:740] ,mux_2level_tapbuf_size16_37_configbus0[740:740], mux_2level_tapbuf_size16_37_configbus1[740:740] , mux_2level_tapbuf_size16_37_configbus0_b[740:740] );
sram6T_blwl sram_blwl_741_ (mux_2level_tapbuf_size16_37_sram_blwl_out[741:741] ,mux_2level_tapbuf_size16_37_sram_blwl_out[741:741] ,mux_2level_tapbuf_size16_37_sram_blwl_outb[741:741] ,mux_2level_tapbuf_size16_37_configbus0[741:741], mux_2level_tapbuf_size16_37_configbus1[741:741] , mux_2level_tapbuf_size16_37_configbus0_b[741:741] );
sram6T_blwl sram_blwl_742_ (mux_2level_tapbuf_size16_37_sram_blwl_out[742:742] ,mux_2level_tapbuf_size16_37_sram_blwl_out[742:742] ,mux_2level_tapbuf_size16_37_sram_blwl_outb[742:742] ,mux_2level_tapbuf_size16_37_configbus0[742:742], mux_2level_tapbuf_size16_37_configbus1[742:742] , mux_2level_tapbuf_size16_37_configbus0_b[742:742] );
sram6T_blwl sram_blwl_743_ (mux_2level_tapbuf_size16_37_sram_blwl_out[743:743] ,mux_2level_tapbuf_size16_37_sram_blwl_out[743:743] ,mux_2level_tapbuf_size16_37_sram_blwl_outb[743:743] ,mux_2level_tapbuf_size16_37_configbus0[743:743], mux_2level_tapbuf_size16_37_configbus1[743:743] , mux_2level_tapbuf_size16_37_configbus0_b[743:743] );
wire [0:15] mux_2level_tapbuf_size16_38_inbus;
assign mux_2level_tapbuf_size16_38_inbus[0] = chany_0__1__midout_2_;
assign mux_2level_tapbuf_size16_38_inbus[1] = chany_0__1__midout_3_;
assign mux_2level_tapbuf_size16_38_inbus[2] = chany_0__1__midout_14_;
assign mux_2level_tapbuf_size16_38_inbus[3] = chany_0__1__midout_15_;
assign mux_2level_tapbuf_size16_38_inbus[4] = chany_0__1__midout_28_;
assign mux_2level_tapbuf_size16_38_inbus[5] = chany_0__1__midout_29_;
assign mux_2level_tapbuf_size16_38_inbus[6] = chany_0__1__midout_40_;
assign mux_2level_tapbuf_size16_38_inbus[7] = chany_0__1__midout_41_;
assign mux_2level_tapbuf_size16_38_inbus[8] = chany_0__1__midout_52_;
assign mux_2level_tapbuf_size16_38_inbus[9] = chany_0__1__midout_53_;
assign mux_2level_tapbuf_size16_38_inbus[10] = chany_0__1__midout_64_;
assign mux_2level_tapbuf_size16_38_inbus[11] = chany_0__1__midout_65_;
assign mux_2level_tapbuf_size16_38_inbus[12] = chany_0__1__midout_78_;
assign mux_2level_tapbuf_size16_38_inbus[13] = chany_0__1__midout_79_;
assign mux_2level_tapbuf_size16_38_inbus[14] = chany_0__1__midout_90_;
assign mux_2level_tapbuf_size16_38_inbus[15] = chany_0__1__midout_91_;
wire [744:751] mux_2level_tapbuf_size16_38_configbus0;
wire [744:751] mux_2level_tapbuf_size16_38_configbus1;
wire [744:751] mux_2level_tapbuf_size16_38_sram_blwl_out ;
wire [744:751] mux_2level_tapbuf_size16_38_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_38_configbus0[744:751] = sram_blwl_bl[744:751] ;
assign mux_2level_tapbuf_size16_38_configbus1[744:751] = sram_blwl_wl[744:751] ;
wire [744:751] mux_2level_tapbuf_size16_38_configbus0_b;
assign mux_2level_tapbuf_size16_38_configbus0_b[744:751] = sram_blwl_blb[744:751] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_38_ (mux_2level_tapbuf_size16_38_inbus, grid_1__1__pin_0__3__11_, mux_2level_tapbuf_size16_38_sram_blwl_out[744:751] ,
mux_2level_tapbuf_size16_38_sram_blwl_outb[744:751] );
//----- SRAM bits for MUX[38], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_744_ (mux_2level_tapbuf_size16_38_sram_blwl_out[744:744] ,mux_2level_tapbuf_size16_38_sram_blwl_out[744:744] ,mux_2level_tapbuf_size16_38_sram_blwl_outb[744:744] ,mux_2level_tapbuf_size16_38_configbus0[744:744], mux_2level_tapbuf_size16_38_configbus1[744:744] , mux_2level_tapbuf_size16_38_configbus0_b[744:744] );
sram6T_blwl sram_blwl_745_ (mux_2level_tapbuf_size16_38_sram_blwl_out[745:745] ,mux_2level_tapbuf_size16_38_sram_blwl_out[745:745] ,mux_2level_tapbuf_size16_38_sram_blwl_outb[745:745] ,mux_2level_tapbuf_size16_38_configbus0[745:745], mux_2level_tapbuf_size16_38_configbus1[745:745] , mux_2level_tapbuf_size16_38_configbus0_b[745:745] );
sram6T_blwl sram_blwl_746_ (mux_2level_tapbuf_size16_38_sram_blwl_out[746:746] ,mux_2level_tapbuf_size16_38_sram_blwl_out[746:746] ,mux_2level_tapbuf_size16_38_sram_blwl_outb[746:746] ,mux_2level_tapbuf_size16_38_configbus0[746:746], mux_2level_tapbuf_size16_38_configbus1[746:746] , mux_2level_tapbuf_size16_38_configbus0_b[746:746] );
sram6T_blwl sram_blwl_747_ (mux_2level_tapbuf_size16_38_sram_blwl_out[747:747] ,mux_2level_tapbuf_size16_38_sram_blwl_out[747:747] ,mux_2level_tapbuf_size16_38_sram_blwl_outb[747:747] ,mux_2level_tapbuf_size16_38_configbus0[747:747], mux_2level_tapbuf_size16_38_configbus1[747:747] , mux_2level_tapbuf_size16_38_configbus0_b[747:747] );
sram6T_blwl sram_blwl_748_ (mux_2level_tapbuf_size16_38_sram_blwl_out[748:748] ,mux_2level_tapbuf_size16_38_sram_blwl_out[748:748] ,mux_2level_tapbuf_size16_38_sram_blwl_outb[748:748] ,mux_2level_tapbuf_size16_38_configbus0[748:748], mux_2level_tapbuf_size16_38_configbus1[748:748] , mux_2level_tapbuf_size16_38_configbus0_b[748:748] );
sram6T_blwl sram_blwl_749_ (mux_2level_tapbuf_size16_38_sram_blwl_out[749:749] ,mux_2level_tapbuf_size16_38_sram_blwl_out[749:749] ,mux_2level_tapbuf_size16_38_sram_blwl_outb[749:749] ,mux_2level_tapbuf_size16_38_configbus0[749:749], mux_2level_tapbuf_size16_38_configbus1[749:749] , mux_2level_tapbuf_size16_38_configbus0_b[749:749] );
sram6T_blwl sram_blwl_750_ (mux_2level_tapbuf_size16_38_sram_blwl_out[750:750] ,mux_2level_tapbuf_size16_38_sram_blwl_out[750:750] ,mux_2level_tapbuf_size16_38_sram_blwl_outb[750:750] ,mux_2level_tapbuf_size16_38_configbus0[750:750], mux_2level_tapbuf_size16_38_configbus1[750:750] , mux_2level_tapbuf_size16_38_configbus0_b[750:750] );
sram6T_blwl sram_blwl_751_ (mux_2level_tapbuf_size16_38_sram_blwl_out[751:751] ,mux_2level_tapbuf_size16_38_sram_blwl_out[751:751] ,mux_2level_tapbuf_size16_38_sram_blwl_outb[751:751] ,mux_2level_tapbuf_size16_38_configbus0[751:751], mux_2level_tapbuf_size16_38_configbus1[751:751] , mux_2level_tapbuf_size16_38_configbus0_b[751:751] );
wire [0:15] mux_2level_tapbuf_size16_39_inbus;
assign mux_2level_tapbuf_size16_39_inbus[0] = chany_0__1__midout_4_;
assign mux_2level_tapbuf_size16_39_inbus[1] = chany_0__1__midout_5_;
assign mux_2level_tapbuf_size16_39_inbus[2] = chany_0__1__midout_16_;
assign mux_2level_tapbuf_size16_39_inbus[3] = chany_0__1__midout_17_;
assign mux_2level_tapbuf_size16_39_inbus[4] = chany_0__1__midout_28_;
assign mux_2level_tapbuf_size16_39_inbus[5] = chany_0__1__midout_29_;
assign mux_2level_tapbuf_size16_39_inbus[6] = chany_0__1__midout_42_;
assign mux_2level_tapbuf_size16_39_inbus[7] = chany_0__1__midout_43_;
assign mux_2level_tapbuf_size16_39_inbus[8] = chany_0__1__midout_54_;
assign mux_2level_tapbuf_size16_39_inbus[9] = chany_0__1__midout_55_;
assign mux_2level_tapbuf_size16_39_inbus[10] = chany_0__1__midout_66_;
assign mux_2level_tapbuf_size16_39_inbus[11] = chany_0__1__midout_67_;
assign mux_2level_tapbuf_size16_39_inbus[12] = chany_0__1__midout_78_;
assign mux_2level_tapbuf_size16_39_inbus[13] = chany_0__1__midout_79_;
assign mux_2level_tapbuf_size16_39_inbus[14] = chany_0__1__midout_92_;
assign mux_2level_tapbuf_size16_39_inbus[15] = chany_0__1__midout_93_;
wire [752:759] mux_2level_tapbuf_size16_39_configbus0;
wire [752:759] mux_2level_tapbuf_size16_39_configbus1;
wire [752:759] mux_2level_tapbuf_size16_39_sram_blwl_out ;
wire [752:759] mux_2level_tapbuf_size16_39_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_39_configbus0[752:759] = sram_blwl_bl[752:759] ;
assign mux_2level_tapbuf_size16_39_configbus1[752:759] = sram_blwl_wl[752:759] ;
wire [752:759] mux_2level_tapbuf_size16_39_configbus0_b;
assign mux_2level_tapbuf_size16_39_configbus0_b[752:759] = sram_blwl_blb[752:759] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_39_ (mux_2level_tapbuf_size16_39_inbus, grid_1__1__pin_0__3__15_, mux_2level_tapbuf_size16_39_sram_blwl_out[752:759] ,
mux_2level_tapbuf_size16_39_sram_blwl_outb[752:759] );
//----- SRAM bits for MUX[39], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_752_ (mux_2level_tapbuf_size16_39_sram_blwl_out[752:752] ,mux_2level_tapbuf_size16_39_sram_blwl_out[752:752] ,mux_2level_tapbuf_size16_39_sram_blwl_outb[752:752] ,mux_2level_tapbuf_size16_39_configbus0[752:752], mux_2level_tapbuf_size16_39_configbus1[752:752] , mux_2level_tapbuf_size16_39_configbus0_b[752:752] );
sram6T_blwl sram_blwl_753_ (mux_2level_tapbuf_size16_39_sram_blwl_out[753:753] ,mux_2level_tapbuf_size16_39_sram_blwl_out[753:753] ,mux_2level_tapbuf_size16_39_sram_blwl_outb[753:753] ,mux_2level_tapbuf_size16_39_configbus0[753:753], mux_2level_tapbuf_size16_39_configbus1[753:753] , mux_2level_tapbuf_size16_39_configbus0_b[753:753] );
sram6T_blwl sram_blwl_754_ (mux_2level_tapbuf_size16_39_sram_blwl_out[754:754] ,mux_2level_tapbuf_size16_39_sram_blwl_out[754:754] ,mux_2level_tapbuf_size16_39_sram_blwl_outb[754:754] ,mux_2level_tapbuf_size16_39_configbus0[754:754], mux_2level_tapbuf_size16_39_configbus1[754:754] , mux_2level_tapbuf_size16_39_configbus0_b[754:754] );
sram6T_blwl sram_blwl_755_ (mux_2level_tapbuf_size16_39_sram_blwl_out[755:755] ,mux_2level_tapbuf_size16_39_sram_blwl_out[755:755] ,mux_2level_tapbuf_size16_39_sram_blwl_outb[755:755] ,mux_2level_tapbuf_size16_39_configbus0[755:755], mux_2level_tapbuf_size16_39_configbus1[755:755] , mux_2level_tapbuf_size16_39_configbus0_b[755:755] );
sram6T_blwl sram_blwl_756_ (mux_2level_tapbuf_size16_39_sram_blwl_out[756:756] ,mux_2level_tapbuf_size16_39_sram_blwl_out[756:756] ,mux_2level_tapbuf_size16_39_sram_blwl_outb[756:756] ,mux_2level_tapbuf_size16_39_configbus0[756:756], mux_2level_tapbuf_size16_39_configbus1[756:756] , mux_2level_tapbuf_size16_39_configbus0_b[756:756] );
sram6T_blwl sram_blwl_757_ (mux_2level_tapbuf_size16_39_sram_blwl_out[757:757] ,mux_2level_tapbuf_size16_39_sram_blwl_out[757:757] ,mux_2level_tapbuf_size16_39_sram_blwl_outb[757:757] ,mux_2level_tapbuf_size16_39_configbus0[757:757], mux_2level_tapbuf_size16_39_configbus1[757:757] , mux_2level_tapbuf_size16_39_configbus0_b[757:757] );
sram6T_blwl sram_blwl_758_ (mux_2level_tapbuf_size16_39_sram_blwl_out[758:758] ,mux_2level_tapbuf_size16_39_sram_blwl_out[758:758] ,mux_2level_tapbuf_size16_39_sram_blwl_outb[758:758] ,mux_2level_tapbuf_size16_39_configbus0[758:758], mux_2level_tapbuf_size16_39_configbus1[758:758] , mux_2level_tapbuf_size16_39_configbus0_b[758:758] );
sram6T_blwl sram_blwl_759_ (mux_2level_tapbuf_size16_39_sram_blwl_out[759:759] ,mux_2level_tapbuf_size16_39_sram_blwl_out[759:759] ,mux_2level_tapbuf_size16_39_sram_blwl_outb[759:759] ,mux_2level_tapbuf_size16_39_configbus0[759:759], mux_2level_tapbuf_size16_39_configbus1[759:759] , mux_2level_tapbuf_size16_39_configbus0_b[759:759] );
wire [0:15] mux_2level_tapbuf_size16_40_inbus;
assign mux_2level_tapbuf_size16_40_inbus[0] = chany_0__1__midout_4_;
assign mux_2level_tapbuf_size16_40_inbus[1] = chany_0__1__midout_5_;
assign mux_2level_tapbuf_size16_40_inbus[2] = chany_0__1__midout_18_;
assign mux_2level_tapbuf_size16_40_inbus[3] = chany_0__1__midout_19_;
assign mux_2level_tapbuf_size16_40_inbus[4] = chany_0__1__midout_30_;
assign mux_2level_tapbuf_size16_40_inbus[5] = chany_0__1__midout_31_;
assign mux_2level_tapbuf_size16_40_inbus[6] = chany_0__1__midout_42_;
assign mux_2level_tapbuf_size16_40_inbus[7] = chany_0__1__midout_43_;
assign mux_2level_tapbuf_size16_40_inbus[8] = chany_0__1__midout_54_;
assign mux_2level_tapbuf_size16_40_inbus[9] = chany_0__1__midout_55_;
assign mux_2level_tapbuf_size16_40_inbus[10] = chany_0__1__midout_68_;
assign mux_2level_tapbuf_size16_40_inbus[11] = chany_0__1__midout_69_;
assign mux_2level_tapbuf_size16_40_inbus[12] = chany_0__1__midout_80_;
assign mux_2level_tapbuf_size16_40_inbus[13] = chany_0__1__midout_81_;
assign mux_2level_tapbuf_size16_40_inbus[14] = chany_0__1__midout_92_;
assign mux_2level_tapbuf_size16_40_inbus[15] = chany_0__1__midout_93_;
wire [760:767] mux_2level_tapbuf_size16_40_configbus0;
wire [760:767] mux_2level_tapbuf_size16_40_configbus1;
wire [760:767] mux_2level_tapbuf_size16_40_sram_blwl_out ;
wire [760:767] mux_2level_tapbuf_size16_40_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_40_configbus0[760:767] = sram_blwl_bl[760:767] ;
assign mux_2level_tapbuf_size16_40_configbus1[760:767] = sram_blwl_wl[760:767] ;
wire [760:767] mux_2level_tapbuf_size16_40_configbus0_b;
assign mux_2level_tapbuf_size16_40_configbus0_b[760:767] = sram_blwl_blb[760:767] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_40_ (mux_2level_tapbuf_size16_40_inbus, grid_1__1__pin_0__3__19_, mux_2level_tapbuf_size16_40_sram_blwl_out[760:767] ,
mux_2level_tapbuf_size16_40_sram_blwl_outb[760:767] );
//----- SRAM bits for MUX[40], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_760_ (mux_2level_tapbuf_size16_40_sram_blwl_out[760:760] ,mux_2level_tapbuf_size16_40_sram_blwl_out[760:760] ,mux_2level_tapbuf_size16_40_sram_blwl_outb[760:760] ,mux_2level_tapbuf_size16_40_configbus0[760:760], mux_2level_tapbuf_size16_40_configbus1[760:760] , mux_2level_tapbuf_size16_40_configbus0_b[760:760] );
sram6T_blwl sram_blwl_761_ (mux_2level_tapbuf_size16_40_sram_blwl_out[761:761] ,mux_2level_tapbuf_size16_40_sram_blwl_out[761:761] ,mux_2level_tapbuf_size16_40_sram_blwl_outb[761:761] ,mux_2level_tapbuf_size16_40_configbus0[761:761], mux_2level_tapbuf_size16_40_configbus1[761:761] , mux_2level_tapbuf_size16_40_configbus0_b[761:761] );
sram6T_blwl sram_blwl_762_ (mux_2level_tapbuf_size16_40_sram_blwl_out[762:762] ,mux_2level_tapbuf_size16_40_sram_blwl_out[762:762] ,mux_2level_tapbuf_size16_40_sram_blwl_outb[762:762] ,mux_2level_tapbuf_size16_40_configbus0[762:762], mux_2level_tapbuf_size16_40_configbus1[762:762] , mux_2level_tapbuf_size16_40_configbus0_b[762:762] );
sram6T_blwl sram_blwl_763_ (mux_2level_tapbuf_size16_40_sram_blwl_out[763:763] ,mux_2level_tapbuf_size16_40_sram_blwl_out[763:763] ,mux_2level_tapbuf_size16_40_sram_blwl_outb[763:763] ,mux_2level_tapbuf_size16_40_configbus0[763:763], mux_2level_tapbuf_size16_40_configbus1[763:763] , mux_2level_tapbuf_size16_40_configbus0_b[763:763] );
sram6T_blwl sram_blwl_764_ (mux_2level_tapbuf_size16_40_sram_blwl_out[764:764] ,mux_2level_tapbuf_size16_40_sram_blwl_out[764:764] ,mux_2level_tapbuf_size16_40_sram_blwl_outb[764:764] ,mux_2level_tapbuf_size16_40_configbus0[764:764], mux_2level_tapbuf_size16_40_configbus1[764:764] , mux_2level_tapbuf_size16_40_configbus0_b[764:764] );
sram6T_blwl sram_blwl_765_ (mux_2level_tapbuf_size16_40_sram_blwl_out[765:765] ,mux_2level_tapbuf_size16_40_sram_blwl_out[765:765] ,mux_2level_tapbuf_size16_40_sram_blwl_outb[765:765] ,mux_2level_tapbuf_size16_40_configbus0[765:765], mux_2level_tapbuf_size16_40_configbus1[765:765] , mux_2level_tapbuf_size16_40_configbus0_b[765:765] );
sram6T_blwl sram_blwl_766_ (mux_2level_tapbuf_size16_40_sram_blwl_out[766:766] ,mux_2level_tapbuf_size16_40_sram_blwl_out[766:766] ,mux_2level_tapbuf_size16_40_sram_blwl_outb[766:766] ,mux_2level_tapbuf_size16_40_configbus0[766:766], mux_2level_tapbuf_size16_40_configbus1[766:766] , mux_2level_tapbuf_size16_40_configbus0_b[766:766] );
sram6T_blwl sram_blwl_767_ (mux_2level_tapbuf_size16_40_sram_blwl_out[767:767] ,mux_2level_tapbuf_size16_40_sram_blwl_out[767:767] ,mux_2level_tapbuf_size16_40_sram_blwl_outb[767:767] ,mux_2level_tapbuf_size16_40_configbus0[767:767], mux_2level_tapbuf_size16_40_configbus1[767:767] , mux_2level_tapbuf_size16_40_configbus0_b[767:767] );
wire [0:15] mux_2level_tapbuf_size16_41_inbus;
assign mux_2level_tapbuf_size16_41_inbus[0] = chany_0__1__midout_6_;
assign mux_2level_tapbuf_size16_41_inbus[1] = chany_0__1__midout_7_;
assign mux_2level_tapbuf_size16_41_inbus[2] = chany_0__1__midout_18_;
assign mux_2level_tapbuf_size16_41_inbus[3] = chany_0__1__midout_19_;
assign mux_2level_tapbuf_size16_41_inbus[4] = chany_0__1__midout_32_;
assign mux_2level_tapbuf_size16_41_inbus[5] = chany_0__1__midout_33_;
assign mux_2level_tapbuf_size16_41_inbus[6] = chany_0__1__midout_44_;
assign mux_2level_tapbuf_size16_41_inbus[7] = chany_0__1__midout_45_;
assign mux_2level_tapbuf_size16_41_inbus[8] = chany_0__1__midout_56_;
assign mux_2level_tapbuf_size16_41_inbus[9] = chany_0__1__midout_57_;
assign mux_2level_tapbuf_size16_41_inbus[10] = chany_0__1__midout_68_;
assign mux_2level_tapbuf_size16_41_inbus[11] = chany_0__1__midout_69_;
assign mux_2level_tapbuf_size16_41_inbus[12] = chany_0__1__midout_82_;
assign mux_2level_tapbuf_size16_41_inbus[13] = chany_0__1__midout_83_;
assign mux_2level_tapbuf_size16_41_inbus[14] = chany_0__1__midout_94_;
assign mux_2level_tapbuf_size16_41_inbus[15] = chany_0__1__midout_95_;
wire [768:775] mux_2level_tapbuf_size16_41_configbus0;
wire [768:775] mux_2level_tapbuf_size16_41_configbus1;
wire [768:775] mux_2level_tapbuf_size16_41_sram_blwl_out ;
wire [768:775] mux_2level_tapbuf_size16_41_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_41_configbus0[768:775] = sram_blwl_bl[768:775] ;
assign mux_2level_tapbuf_size16_41_configbus1[768:775] = sram_blwl_wl[768:775] ;
wire [768:775] mux_2level_tapbuf_size16_41_configbus0_b;
assign mux_2level_tapbuf_size16_41_configbus0_b[768:775] = sram_blwl_blb[768:775] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_41_ (mux_2level_tapbuf_size16_41_inbus, grid_1__1__pin_0__3__23_, mux_2level_tapbuf_size16_41_sram_blwl_out[768:775] ,
mux_2level_tapbuf_size16_41_sram_blwl_outb[768:775] );
//----- SRAM bits for MUX[41], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_768_ (mux_2level_tapbuf_size16_41_sram_blwl_out[768:768] ,mux_2level_tapbuf_size16_41_sram_blwl_out[768:768] ,mux_2level_tapbuf_size16_41_sram_blwl_outb[768:768] ,mux_2level_tapbuf_size16_41_configbus0[768:768], mux_2level_tapbuf_size16_41_configbus1[768:768] , mux_2level_tapbuf_size16_41_configbus0_b[768:768] );
sram6T_blwl sram_blwl_769_ (mux_2level_tapbuf_size16_41_sram_blwl_out[769:769] ,mux_2level_tapbuf_size16_41_sram_blwl_out[769:769] ,mux_2level_tapbuf_size16_41_sram_blwl_outb[769:769] ,mux_2level_tapbuf_size16_41_configbus0[769:769], mux_2level_tapbuf_size16_41_configbus1[769:769] , mux_2level_tapbuf_size16_41_configbus0_b[769:769] );
sram6T_blwl sram_blwl_770_ (mux_2level_tapbuf_size16_41_sram_blwl_out[770:770] ,mux_2level_tapbuf_size16_41_sram_blwl_out[770:770] ,mux_2level_tapbuf_size16_41_sram_blwl_outb[770:770] ,mux_2level_tapbuf_size16_41_configbus0[770:770], mux_2level_tapbuf_size16_41_configbus1[770:770] , mux_2level_tapbuf_size16_41_configbus0_b[770:770] );
sram6T_blwl sram_blwl_771_ (mux_2level_tapbuf_size16_41_sram_blwl_out[771:771] ,mux_2level_tapbuf_size16_41_sram_blwl_out[771:771] ,mux_2level_tapbuf_size16_41_sram_blwl_outb[771:771] ,mux_2level_tapbuf_size16_41_configbus0[771:771], mux_2level_tapbuf_size16_41_configbus1[771:771] , mux_2level_tapbuf_size16_41_configbus0_b[771:771] );
sram6T_blwl sram_blwl_772_ (mux_2level_tapbuf_size16_41_sram_blwl_out[772:772] ,mux_2level_tapbuf_size16_41_sram_blwl_out[772:772] ,mux_2level_tapbuf_size16_41_sram_blwl_outb[772:772] ,mux_2level_tapbuf_size16_41_configbus0[772:772], mux_2level_tapbuf_size16_41_configbus1[772:772] , mux_2level_tapbuf_size16_41_configbus0_b[772:772] );
sram6T_blwl sram_blwl_773_ (mux_2level_tapbuf_size16_41_sram_blwl_out[773:773] ,mux_2level_tapbuf_size16_41_sram_blwl_out[773:773] ,mux_2level_tapbuf_size16_41_sram_blwl_outb[773:773] ,mux_2level_tapbuf_size16_41_configbus0[773:773], mux_2level_tapbuf_size16_41_configbus1[773:773] , mux_2level_tapbuf_size16_41_configbus0_b[773:773] );
sram6T_blwl sram_blwl_774_ (mux_2level_tapbuf_size16_41_sram_blwl_out[774:774] ,mux_2level_tapbuf_size16_41_sram_blwl_out[774:774] ,mux_2level_tapbuf_size16_41_sram_blwl_outb[774:774] ,mux_2level_tapbuf_size16_41_configbus0[774:774], mux_2level_tapbuf_size16_41_configbus1[774:774] , mux_2level_tapbuf_size16_41_configbus0_b[774:774] );
sram6T_blwl sram_blwl_775_ (mux_2level_tapbuf_size16_41_sram_blwl_out[775:775] ,mux_2level_tapbuf_size16_41_sram_blwl_out[775:775] ,mux_2level_tapbuf_size16_41_sram_blwl_outb[775:775] ,mux_2level_tapbuf_size16_41_configbus0[775:775], mux_2level_tapbuf_size16_41_configbus1[775:775] , mux_2level_tapbuf_size16_41_configbus0_b[775:775] );
wire [0:15] mux_2level_tapbuf_size16_42_inbus;
assign mux_2level_tapbuf_size16_42_inbus[0] = chany_0__1__midout_8_;
assign mux_2level_tapbuf_size16_42_inbus[1] = chany_0__1__midout_9_;
assign mux_2level_tapbuf_size16_42_inbus[2] = chany_0__1__midout_20_;
assign mux_2level_tapbuf_size16_42_inbus[3] = chany_0__1__midout_21_;
assign mux_2level_tapbuf_size16_42_inbus[4] = chany_0__1__midout_32_;
assign mux_2level_tapbuf_size16_42_inbus[5] = chany_0__1__midout_33_;
assign mux_2level_tapbuf_size16_42_inbus[6] = chany_0__1__midout_44_;
assign mux_2level_tapbuf_size16_42_inbus[7] = chany_0__1__midout_45_;
assign mux_2level_tapbuf_size16_42_inbus[8] = chany_0__1__midout_58_;
assign mux_2level_tapbuf_size16_42_inbus[9] = chany_0__1__midout_59_;
assign mux_2level_tapbuf_size16_42_inbus[10] = chany_0__1__midout_70_;
assign mux_2level_tapbuf_size16_42_inbus[11] = chany_0__1__midout_71_;
assign mux_2level_tapbuf_size16_42_inbus[12] = chany_0__1__midout_82_;
assign mux_2level_tapbuf_size16_42_inbus[13] = chany_0__1__midout_83_;
assign mux_2level_tapbuf_size16_42_inbus[14] = chany_0__1__midout_94_;
assign mux_2level_tapbuf_size16_42_inbus[15] = chany_0__1__midout_95_;
wire [776:783] mux_2level_tapbuf_size16_42_configbus0;
wire [776:783] mux_2level_tapbuf_size16_42_configbus1;
wire [776:783] mux_2level_tapbuf_size16_42_sram_blwl_out ;
wire [776:783] mux_2level_tapbuf_size16_42_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_42_configbus0[776:783] = sram_blwl_bl[776:783] ;
assign mux_2level_tapbuf_size16_42_configbus1[776:783] = sram_blwl_wl[776:783] ;
wire [776:783] mux_2level_tapbuf_size16_42_configbus0_b;
assign mux_2level_tapbuf_size16_42_configbus0_b[776:783] = sram_blwl_blb[776:783] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_42_ (mux_2level_tapbuf_size16_42_inbus, grid_1__1__pin_0__3__27_, mux_2level_tapbuf_size16_42_sram_blwl_out[776:783] ,
mux_2level_tapbuf_size16_42_sram_blwl_outb[776:783] );
//----- SRAM bits for MUX[42], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_776_ (mux_2level_tapbuf_size16_42_sram_blwl_out[776:776] ,mux_2level_tapbuf_size16_42_sram_blwl_out[776:776] ,mux_2level_tapbuf_size16_42_sram_blwl_outb[776:776] ,mux_2level_tapbuf_size16_42_configbus0[776:776], mux_2level_tapbuf_size16_42_configbus1[776:776] , mux_2level_tapbuf_size16_42_configbus0_b[776:776] );
sram6T_blwl sram_blwl_777_ (mux_2level_tapbuf_size16_42_sram_blwl_out[777:777] ,mux_2level_tapbuf_size16_42_sram_blwl_out[777:777] ,mux_2level_tapbuf_size16_42_sram_blwl_outb[777:777] ,mux_2level_tapbuf_size16_42_configbus0[777:777], mux_2level_tapbuf_size16_42_configbus1[777:777] , mux_2level_tapbuf_size16_42_configbus0_b[777:777] );
sram6T_blwl sram_blwl_778_ (mux_2level_tapbuf_size16_42_sram_blwl_out[778:778] ,mux_2level_tapbuf_size16_42_sram_blwl_out[778:778] ,mux_2level_tapbuf_size16_42_sram_blwl_outb[778:778] ,mux_2level_tapbuf_size16_42_configbus0[778:778], mux_2level_tapbuf_size16_42_configbus1[778:778] , mux_2level_tapbuf_size16_42_configbus0_b[778:778] );
sram6T_blwl sram_blwl_779_ (mux_2level_tapbuf_size16_42_sram_blwl_out[779:779] ,mux_2level_tapbuf_size16_42_sram_blwl_out[779:779] ,mux_2level_tapbuf_size16_42_sram_blwl_outb[779:779] ,mux_2level_tapbuf_size16_42_configbus0[779:779], mux_2level_tapbuf_size16_42_configbus1[779:779] , mux_2level_tapbuf_size16_42_configbus0_b[779:779] );
sram6T_blwl sram_blwl_780_ (mux_2level_tapbuf_size16_42_sram_blwl_out[780:780] ,mux_2level_tapbuf_size16_42_sram_blwl_out[780:780] ,mux_2level_tapbuf_size16_42_sram_blwl_outb[780:780] ,mux_2level_tapbuf_size16_42_configbus0[780:780], mux_2level_tapbuf_size16_42_configbus1[780:780] , mux_2level_tapbuf_size16_42_configbus0_b[780:780] );
sram6T_blwl sram_blwl_781_ (mux_2level_tapbuf_size16_42_sram_blwl_out[781:781] ,mux_2level_tapbuf_size16_42_sram_blwl_out[781:781] ,mux_2level_tapbuf_size16_42_sram_blwl_outb[781:781] ,mux_2level_tapbuf_size16_42_configbus0[781:781], mux_2level_tapbuf_size16_42_configbus1[781:781] , mux_2level_tapbuf_size16_42_configbus0_b[781:781] );
sram6T_blwl sram_blwl_782_ (mux_2level_tapbuf_size16_42_sram_blwl_out[782:782] ,mux_2level_tapbuf_size16_42_sram_blwl_out[782:782] ,mux_2level_tapbuf_size16_42_sram_blwl_outb[782:782] ,mux_2level_tapbuf_size16_42_configbus0[782:782], mux_2level_tapbuf_size16_42_configbus1[782:782] , mux_2level_tapbuf_size16_42_configbus0_b[782:782] );
sram6T_blwl sram_blwl_783_ (mux_2level_tapbuf_size16_42_sram_blwl_out[783:783] ,mux_2level_tapbuf_size16_42_sram_blwl_out[783:783] ,mux_2level_tapbuf_size16_42_sram_blwl_outb[783:783] ,mux_2level_tapbuf_size16_42_configbus0[783:783], mux_2level_tapbuf_size16_42_configbus1[783:783] , mux_2level_tapbuf_size16_42_configbus0_b[783:783] );
wire [0:15] mux_2level_tapbuf_size16_43_inbus;
assign mux_2level_tapbuf_size16_43_inbus[0] = chany_0__1__midout_8_;
assign mux_2level_tapbuf_size16_43_inbus[1] = chany_0__1__midout_9_;
assign mux_2level_tapbuf_size16_43_inbus[2] = chany_0__1__midout_22_;
assign mux_2level_tapbuf_size16_43_inbus[3] = chany_0__1__midout_23_;
assign mux_2level_tapbuf_size16_43_inbus[4] = chany_0__1__midout_34_;
assign mux_2level_tapbuf_size16_43_inbus[5] = chany_0__1__midout_35_;
assign mux_2level_tapbuf_size16_43_inbus[6] = chany_0__1__midout_46_;
assign mux_2level_tapbuf_size16_43_inbus[7] = chany_0__1__midout_47_;
assign mux_2level_tapbuf_size16_43_inbus[8] = chany_0__1__midout_58_;
assign mux_2level_tapbuf_size16_43_inbus[9] = chany_0__1__midout_59_;
assign mux_2level_tapbuf_size16_43_inbus[10] = chany_0__1__midout_72_;
assign mux_2level_tapbuf_size16_43_inbus[11] = chany_0__1__midout_73_;
assign mux_2level_tapbuf_size16_43_inbus[12] = chany_0__1__midout_84_;
assign mux_2level_tapbuf_size16_43_inbus[13] = chany_0__1__midout_85_;
assign mux_2level_tapbuf_size16_43_inbus[14] = chany_0__1__midout_96_;
assign mux_2level_tapbuf_size16_43_inbus[15] = chany_0__1__midout_97_;
wire [784:791] mux_2level_tapbuf_size16_43_configbus0;
wire [784:791] mux_2level_tapbuf_size16_43_configbus1;
wire [784:791] mux_2level_tapbuf_size16_43_sram_blwl_out ;
wire [784:791] mux_2level_tapbuf_size16_43_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_43_configbus0[784:791] = sram_blwl_bl[784:791] ;
assign mux_2level_tapbuf_size16_43_configbus1[784:791] = sram_blwl_wl[784:791] ;
wire [784:791] mux_2level_tapbuf_size16_43_configbus0_b;
assign mux_2level_tapbuf_size16_43_configbus0_b[784:791] = sram_blwl_blb[784:791] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_43_ (mux_2level_tapbuf_size16_43_inbus, grid_1__1__pin_0__3__31_, mux_2level_tapbuf_size16_43_sram_blwl_out[784:791] ,
mux_2level_tapbuf_size16_43_sram_blwl_outb[784:791] );
//----- SRAM bits for MUX[43], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_784_ (mux_2level_tapbuf_size16_43_sram_blwl_out[784:784] ,mux_2level_tapbuf_size16_43_sram_blwl_out[784:784] ,mux_2level_tapbuf_size16_43_sram_blwl_outb[784:784] ,mux_2level_tapbuf_size16_43_configbus0[784:784], mux_2level_tapbuf_size16_43_configbus1[784:784] , mux_2level_tapbuf_size16_43_configbus0_b[784:784] );
sram6T_blwl sram_blwl_785_ (mux_2level_tapbuf_size16_43_sram_blwl_out[785:785] ,mux_2level_tapbuf_size16_43_sram_blwl_out[785:785] ,mux_2level_tapbuf_size16_43_sram_blwl_outb[785:785] ,mux_2level_tapbuf_size16_43_configbus0[785:785], mux_2level_tapbuf_size16_43_configbus1[785:785] , mux_2level_tapbuf_size16_43_configbus0_b[785:785] );
sram6T_blwl sram_blwl_786_ (mux_2level_tapbuf_size16_43_sram_blwl_out[786:786] ,mux_2level_tapbuf_size16_43_sram_blwl_out[786:786] ,mux_2level_tapbuf_size16_43_sram_blwl_outb[786:786] ,mux_2level_tapbuf_size16_43_configbus0[786:786], mux_2level_tapbuf_size16_43_configbus1[786:786] , mux_2level_tapbuf_size16_43_configbus0_b[786:786] );
sram6T_blwl sram_blwl_787_ (mux_2level_tapbuf_size16_43_sram_blwl_out[787:787] ,mux_2level_tapbuf_size16_43_sram_blwl_out[787:787] ,mux_2level_tapbuf_size16_43_sram_blwl_outb[787:787] ,mux_2level_tapbuf_size16_43_configbus0[787:787], mux_2level_tapbuf_size16_43_configbus1[787:787] , mux_2level_tapbuf_size16_43_configbus0_b[787:787] );
sram6T_blwl sram_blwl_788_ (mux_2level_tapbuf_size16_43_sram_blwl_out[788:788] ,mux_2level_tapbuf_size16_43_sram_blwl_out[788:788] ,mux_2level_tapbuf_size16_43_sram_blwl_outb[788:788] ,mux_2level_tapbuf_size16_43_configbus0[788:788], mux_2level_tapbuf_size16_43_configbus1[788:788] , mux_2level_tapbuf_size16_43_configbus0_b[788:788] );
sram6T_blwl sram_blwl_789_ (mux_2level_tapbuf_size16_43_sram_blwl_out[789:789] ,mux_2level_tapbuf_size16_43_sram_blwl_out[789:789] ,mux_2level_tapbuf_size16_43_sram_blwl_outb[789:789] ,mux_2level_tapbuf_size16_43_configbus0[789:789], mux_2level_tapbuf_size16_43_configbus1[789:789] , mux_2level_tapbuf_size16_43_configbus0_b[789:789] );
sram6T_blwl sram_blwl_790_ (mux_2level_tapbuf_size16_43_sram_blwl_out[790:790] ,mux_2level_tapbuf_size16_43_sram_blwl_out[790:790] ,mux_2level_tapbuf_size16_43_sram_blwl_outb[790:790] ,mux_2level_tapbuf_size16_43_configbus0[790:790], mux_2level_tapbuf_size16_43_configbus1[790:790] , mux_2level_tapbuf_size16_43_configbus0_b[790:790] );
sram6T_blwl sram_blwl_791_ (mux_2level_tapbuf_size16_43_sram_blwl_out[791:791] ,mux_2level_tapbuf_size16_43_sram_blwl_out[791:791] ,mux_2level_tapbuf_size16_43_sram_blwl_outb[791:791] ,mux_2level_tapbuf_size16_43_configbus0[791:791], mux_2level_tapbuf_size16_43_configbus1[791:791] , mux_2level_tapbuf_size16_43_configbus0_b[791:791] );
wire [0:15] mux_2level_tapbuf_size16_44_inbus;
assign mux_2level_tapbuf_size16_44_inbus[0] = chany_0__1__midout_10_;
assign mux_2level_tapbuf_size16_44_inbus[1] = chany_0__1__midout_11_;
assign mux_2level_tapbuf_size16_44_inbus[2] = chany_0__1__midout_22_;
assign mux_2level_tapbuf_size16_44_inbus[3] = chany_0__1__midout_23_;
assign mux_2level_tapbuf_size16_44_inbus[4] = chany_0__1__midout_34_;
assign mux_2level_tapbuf_size16_44_inbus[5] = chany_0__1__midout_35_;
assign mux_2level_tapbuf_size16_44_inbus[6] = chany_0__1__midout_48_;
assign mux_2level_tapbuf_size16_44_inbus[7] = chany_0__1__midout_49_;
assign mux_2level_tapbuf_size16_44_inbus[8] = chany_0__1__midout_60_;
assign mux_2level_tapbuf_size16_44_inbus[9] = chany_0__1__midout_61_;
assign mux_2level_tapbuf_size16_44_inbus[10] = chany_0__1__midout_72_;
assign mux_2level_tapbuf_size16_44_inbus[11] = chany_0__1__midout_73_;
assign mux_2level_tapbuf_size16_44_inbus[12] = chany_0__1__midout_84_;
assign mux_2level_tapbuf_size16_44_inbus[13] = chany_0__1__midout_85_;
assign mux_2level_tapbuf_size16_44_inbus[14] = chany_0__1__midout_98_;
assign mux_2level_tapbuf_size16_44_inbus[15] = chany_0__1__midout_99_;
wire [792:799] mux_2level_tapbuf_size16_44_configbus0;
wire [792:799] mux_2level_tapbuf_size16_44_configbus1;
wire [792:799] mux_2level_tapbuf_size16_44_sram_blwl_out ;
wire [792:799] mux_2level_tapbuf_size16_44_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_44_configbus0[792:799] = sram_blwl_bl[792:799] ;
assign mux_2level_tapbuf_size16_44_configbus1[792:799] = sram_blwl_wl[792:799] ;
wire [792:799] mux_2level_tapbuf_size16_44_configbus0_b;
assign mux_2level_tapbuf_size16_44_configbus0_b[792:799] = sram_blwl_blb[792:799] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_44_ (mux_2level_tapbuf_size16_44_inbus, grid_1__1__pin_0__3__35_, mux_2level_tapbuf_size16_44_sram_blwl_out[792:799] ,
mux_2level_tapbuf_size16_44_sram_blwl_outb[792:799] );
//----- SRAM bits for MUX[44], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_792_ (mux_2level_tapbuf_size16_44_sram_blwl_out[792:792] ,mux_2level_tapbuf_size16_44_sram_blwl_out[792:792] ,mux_2level_tapbuf_size16_44_sram_blwl_outb[792:792] ,mux_2level_tapbuf_size16_44_configbus0[792:792], mux_2level_tapbuf_size16_44_configbus1[792:792] , mux_2level_tapbuf_size16_44_configbus0_b[792:792] );
sram6T_blwl sram_blwl_793_ (mux_2level_tapbuf_size16_44_sram_blwl_out[793:793] ,mux_2level_tapbuf_size16_44_sram_blwl_out[793:793] ,mux_2level_tapbuf_size16_44_sram_blwl_outb[793:793] ,mux_2level_tapbuf_size16_44_configbus0[793:793], mux_2level_tapbuf_size16_44_configbus1[793:793] , mux_2level_tapbuf_size16_44_configbus0_b[793:793] );
sram6T_blwl sram_blwl_794_ (mux_2level_tapbuf_size16_44_sram_blwl_out[794:794] ,mux_2level_tapbuf_size16_44_sram_blwl_out[794:794] ,mux_2level_tapbuf_size16_44_sram_blwl_outb[794:794] ,mux_2level_tapbuf_size16_44_configbus0[794:794], mux_2level_tapbuf_size16_44_configbus1[794:794] , mux_2level_tapbuf_size16_44_configbus0_b[794:794] );
sram6T_blwl sram_blwl_795_ (mux_2level_tapbuf_size16_44_sram_blwl_out[795:795] ,mux_2level_tapbuf_size16_44_sram_blwl_out[795:795] ,mux_2level_tapbuf_size16_44_sram_blwl_outb[795:795] ,mux_2level_tapbuf_size16_44_configbus0[795:795], mux_2level_tapbuf_size16_44_configbus1[795:795] , mux_2level_tapbuf_size16_44_configbus0_b[795:795] );
sram6T_blwl sram_blwl_796_ (mux_2level_tapbuf_size16_44_sram_blwl_out[796:796] ,mux_2level_tapbuf_size16_44_sram_blwl_out[796:796] ,mux_2level_tapbuf_size16_44_sram_blwl_outb[796:796] ,mux_2level_tapbuf_size16_44_configbus0[796:796], mux_2level_tapbuf_size16_44_configbus1[796:796] , mux_2level_tapbuf_size16_44_configbus0_b[796:796] );
sram6T_blwl sram_blwl_797_ (mux_2level_tapbuf_size16_44_sram_blwl_out[797:797] ,mux_2level_tapbuf_size16_44_sram_blwl_out[797:797] ,mux_2level_tapbuf_size16_44_sram_blwl_outb[797:797] ,mux_2level_tapbuf_size16_44_configbus0[797:797], mux_2level_tapbuf_size16_44_configbus1[797:797] , mux_2level_tapbuf_size16_44_configbus0_b[797:797] );
sram6T_blwl sram_blwl_798_ (mux_2level_tapbuf_size16_44_sram_blwl_out[798:798] ,mux_2level_tapbuf_size16_44_sram_blwl_out[798:798] ,mux_2level_tapbuf_size16_44_sram_blwl_outb[798:798] ,mux_2level_tapbuf_size16_44_configbus0[798:798], mux_2level_tapbuf_size16_44_configbus1[798:798] , mux_2level_tapbuf_size16_44_configbus0_b[798:798] );
sram6T_blwl sram_blwl_799_ (mux_2level_tapbuf_size16_44_sram_blwl_out[799:799] ,mux_2level_tapbuf_size16_44_sram_blwl_out[799:799] ,mux_2level_tapbuf_size16_44_sram_blwl_outb[799:799] ,mux_2level_tapbuf_size16_44_configbus0[799:799], mux_2level_tapbuf_size16_44_configbus1[799:799] , mux_2level_tapbuf_size16_44_configbus0_b[799:799] );
wire [0:15] mux_2level_tapbuf_size16_45_inbus;
assign mux_2level_tapbuf_size16_45_inbus[0] = chany_0__1__midout_12_;
assign mux_2level_tapbuf_size16_45_inbus[1] = chany_0__1__midout_13_;
assign mux_2level_tapbuf_size16_45_inbus[2] = chany_0__1__midout_24_;
assign mux_2level_tapbuf_size16_45_inbus[3] = chany_0__1__midout_25_;
assign mux_2level_tapbuf_size16_45_inbus[4] = chany_0__1__midout_36_;
assign mux_2level_tapbuf_size16_45_inbus[5] = chany_0__1__midout_37_;
assign mux_2level_tapbuf_size16_45_inbus[6] = chany_0__1__midout_48_;
assign mux_2level_tapbuf_size16_45_inbus[7] = chany_0__1__midout_49_;
assign mux_2level_tapbuf_size16_45_inbus[8] = chany_0__1__midout_62_;
assign mux_2level_tapbuf_size16_45_inbus[9] = chany_0__1__midout_63_;
assign mux_2level_tapbuf_size16_45_inbus[10] = chany_0__1__midout_74_;
assign mux_2level_tapbuf_size16_45_inbus[11] = chany_0__1__midout_75_;
assign mux_2level_tapbuf_size16_45_inbus[12] = chany_0__1__midout_86_;
assign mux_2level_tapbuf_size16_45_inbus[13] = chany_0__1__midout_87_;
assign mux_2level_tapbuf_size16_45_inbus[14] = chany_0__1__midout_98_;
assign mux_2level_tapbuf_size16_45_inbus[15] = chany_0__1__midout_99_;
wire [800:807] mux_2level_tapbuf_size16_45_configbus0;
wire [800:807] mux_2level_tapbuf_size16_45_configbus1;
wire [800:807] mux_2level_tapbuf_size16_45_sram_blwl_out ;
wire [800:807] mux_2level_tapbuf_size16_45_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_45_configbus0[800:807] = sram_blwl_bl[800:807] ;
assign mux_2level_tapbuf_size16_45_configbus1[800:807] = sram_blwl_wl[800:807] ;
wire [800:807] mux_2level_tapbuf_size16_45_configbus0_b;
assign mux_2level_tapbuf_size16_45_configbus0_b[800:807] = sram_blwl_blb[800:807] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_45_ (mux_2level_tapbuf_size16_45_inbus, grid_1__1__pin_0__3__39_, mux_2level_tapbuf_size16_45_sram_blwl_out[800:807] ,
mux_2level_tapbuf_size16_45_sram_blwl_outb[800:807] );
//----- SRAM bits for MUX[45], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_800_ (mux_2level_tapbuf_size16_45_sram_blwl_out[800:800] ,mux_2level_tapbuf_size16_45_sram_blwl_out[800:800] ,mux_2level_tapbuf_size16_45_sram_blwl_outb[800:800] ,mux_2level_tapbuf_size16_45_configbus0[800:800], mux_2level_tapbuf_size16_45_configbus1[800:800] , mux_2level_tapbuf_size16_45_configbus0_b[800:800] );
sram6T_blwl sram_blwl_801_ (mux_2level_tapbuf_size16_45_sram_blwl_out[801:801] ,mux_2level_tapbuf_size16_45_sram_blwl_out[801:801] ,mux_2level_tapbuf_size16_45_sram_blwl_outb[801:801] ,mux_2level_tapbuf_size16_45_configbus0[801:801], mux_2level_tapbuf_size16_45_configbus1[801:801] , mux_2level_tapbuf_size16_45_configbus0_b[801:801] );
sram6T_blwl sram_blwl_802_ (mux_2level_tapbuf_size16_45_sram_blwl_out[802:802] ,mux_2level_tapbuf_size16_45_sram_blwl_out[802:802] ,mux_2level_tapbuf_size16_45_sram_blwl_outb[802:802] ,mux_2level_tapbuf_size16_45_configbus0[802:802], mux_2level_tapbuf_size16_45_configbus1[802:802] , mux_2level_tapbuf_size16_45_configbus0_b[802:802] );
sram6T_blwl sram_blwl_803_ (mux_2level_tapbuf_size16_45_sram_blwl_out[803:803] ,mux_2level_tapbuf_size16_45_sram_blwl_out[803:803] ,mux_2level_tapbuf_size16_45_sram_blwl_outb[803:803] ,mux_2level_tapbuf_size16_45_configbus0[803:803], mux_2level_tapbuf_size16_45_configbus1[803:803] , mux_2level_tapbuf_size16_45_configbus0_b[803:803] );
sram6T_blwl sram_blwl_804_ (mux_2level_tapbuf_size16_45_sram_blwl_out[804:804] ,mux_2level_tapbuf_size16_45_sram_blwl_out[804:804] ,mux_2level_tapbuf_size16_45_sram_blwl_outb[804:804] ,mux_2level_tapbuf_size16_45_configbus0[804:804], mux_2level_tapbuf_size16_45_configbus1[804:804] , mux_2level_tapbuf_size16_45_configbus0_b[804:804] );
sram6T_blwl sram_blwl_805_ (mux_2level_tapbuf_size16_45_sram_blwl_out[805:805] ,mux_2level_tapbuf_size16_45_sram_blwl_out[805:805] ,mux_2level_tapbuf_size16_45_sram_blwl_outb[805:805] ,mux_2level_tapbuf_size16_45_configbus0[805:805], mux_2level_tapbuf_size16_45_configbus1[805:805] , mux_2level_tapbuf_size16_45_configbus0_b[805:805] );
sram6T_blwl sram_blwl_806_ (mux_2level_tapbuf_size16_45_sram_blwl_out[806:806] ,mux_2level_tapbuf_size16_45_sram_blwl_out[806:806] ,mux_2level_tapbuf_size16_45_sram_blwl_outb[806:806] ,mux_2level_tapbuf_size16_45_configbus0[806:806], mux_2level_tapbuf_size16_45_configbus1[806:806] , mux_2level_tapbuf_size16_45_configbus0_b[806:806] );
sram6T_blwl sram_blwl_807_ (mux_2level_tapbuf_size16_45_sram_blwl_out[807:807] ,mux_2level_tapbuf_size16_45_sram_blwl_out[807:807] ,mux_2level_tapbuf_size16_45_sram_blwl_outb[807:807] ,mux_2level_tapbuf_size16_45_configbus0[807:807], mux_2level_tapbuf_size16_45_configbus1[807:807] , mux_2level_tapbuf_size16_45_configbus0_b[807:807] );
wire [0:15] mux_2level_tapbuf_size16_46_inbus;
assign mux_2level_tapbuf_size16_46_inbus[0] = chany_0__1__midout_0_;
assign mux_2level_tapbuf_size16_46_inbus[1] = chany_0__1__midout_1_;
assign mux_2level_tapbuf_size16_46_inbus[2] = chany_0__1__midout_12_;
assign mux_2level_tapbuf_size16_46_inbus[3] = chany_0__1__midout_13_;
assign mux_2level_tapbuf_size16_46_inbus[4] = chany_0__1__midout_24_;
assign mux_2level_tapbuf_size16_46_inbus[5] = chany_0__1__midout_25_;
assign mux_2level_tapbuf_size16_46_inbus[6] = chany_0__1__midout_36_;
assign mux_2level_tapbuf_size16_46_inbus[7] = chany_0__1__midout_37_;
assign mux_2level_tapbuf_size16_46_inbus[8] = chany_0__1__midout_50_;
assign mux_2level_tapbuf_size16_46_inbus[9] = chany_0__1__midout_51_;
assign mux_2level_tapbuf_size16_46_inbus[10] = chany_0__1__midout_62_;
assign mux_2level_tapbuf_size16_46_inbus[11] = chany_0__1__midout_63_;
assign mux_2level_tapbuf_size16_46_inbus[12] = chany_0__1__midout_74_;
assign mux_2level_tapbuf_size16_46_inbus[13] = chany_0__1__midout_75_;
assign mux_2level_tapbuf_size16_46_inbus[14] = chany_0__1__midout_86_;
assign mux_2level_tapbuf_size16_46_inbus[15] = chany_0__1__midout_87_;
wire [808:815] mux_2level_tapbuf_size16_46_configbus0;
wire [808:815] mux_2level_tapbuf_size16_46_configbus1;
wire [808:815] mux_2level_tapbuf_size16_46_sram_blwl_out ;
wire [808:815] mux_2level_tapbuf_size16_46_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_46_configbus0[808:815] = sram_blwl_bl[808:815] ;
assign mux_2level_tapbuf_size16_46_configbus1[808:815] = sram_blwl_wl[808:815] ;
wire [808:815] mux_2level_tapbuf_size16_46_configbus0_b;
assign mux_2level_tapbuf_size16_46_configbus0_b[808:815] = sram_blwl_blb[808:815] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_46_ (mux_2level_tapbuf_size16_46_inbus, grid_0__1__pin_0__1__0_, mux_2level_tapbuf_size16_46_sram_blwl_out[808:815] ,
mux_2level_tapbuf_size16_46_sram_blwl_outb[808:815] );
//----- SRAM bits for MUX[46], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_808_ (mux_2level_tapbuf_size16_46_sram_blwl_out[808:808] ,mux_2level_tapbuf_size16_46_sram_blwl_out[808:808] ,mux_2level_tapbuf_size16_46_sram_blwl_outb[808:808] ,mux_2level_tapbuf_size16_46_configbus0[808:808], mux_2level_tapbuf_size16_46_configbus1[808:808] , mux_2level_tapbuf_size16_46_configbus0_b[808:808] );
sram6T_blwl sram_blwl_809_ (mux_2level_tapbuf_size16_46_sram_blwl_out[809:809] ,mux_2level_tapbuf_size16_46_sram_blwl_out[809:809] ,mux_2level_tapbuf_size16_46_sram_blwl_outb[809:809] ,mux_2level_tapbuf_size16_46_configbus0[809:809], mux_2level_tapbuf_size16_46_configbus1[809:809] , mux_2level_tapbuf_size16_46_configbus0_b[809:809] );
sram6T_blwl sram_blwl_810_ (mux_2level_tapbuf_size16_46_sram_blwl_out[810:810] ,mux_2level_tapbuf_size16_46_sram_blwl_out[810:810] ,mux_2level_tapbuf_size16_46_sram_blwl_outb[810:810] ,mux_2level_tapbuf_size16_46_configbus0[810:810], mux_2level_tapbuf_size16_46_configbus1[810:810] , mux_2level_tapbuf_size16_46_configbus0_b[810:810] );
sram6T_blwl sram_blwl_811_ (mux_2level_tapbuf_size16_46_sram_blwl_out[811:811] ,mux_2level_tapbuf_size16_46_sram_blwl_out[811:811] ,mux_2level_tapbuf_size16_46_sram_blwl_outb[811:811] ,mux_2level_tapbuf_size16_46_configbus0[811:811], mux_2level_tapbuf_size16_46_configbus1[811:811] , mux_2level_tapbuf_size16_46_configbus0_b[811:811] );
sram6T_blwl sram_blwl_812_ (mux_2level_tapbuf_size16_46_sram_blwl_out[812:812] ,mux_2level_tapbuf_size16_46_sram_blwl_out[812:812] ,mux_2level_tapbuf_size16_46_sram_blwl_outb[812:812] ,mux_2level_tapbuf_size16_46_configbus0[812:812], mux_2level_tapbuf_size16_46_configbus1[812:812] , mux_2level_tapbuf_size16_46_configbus0_b[812:812] );
sram6T_blwl sram_blwl_813_ (mux_2level_tapbuf_size16_46_sram_blwl_out[813:813] ,mux_2level_tapbuf_size16_46_sram_blwl_out[813:813] ,mux_2level_tapbuf_size16_46_sram_blwl_outb[813:813] ,mux_2level_tapbuf_size16_46_configbus0[813:813], mux_2level_tapbuf_size16_46_configbus1[813:813] , mux_2level_tapbuf_size16_46_configbus0_b[813:813] );
sram6T_blwl sram_blwl_814_ (mux_2level_tapbuf_size16_46_sram_blwl_out[814:814] ,mux_2level_tapbuf_size16_46_sram_blwl_out[814:814] ,mux_2level_tapbuf_size16_46_sram_blwl_outb[814:814] ,mux_2level_tapbuf_size16_46_configbus0[814:814], mux_2level_tapbuf_size16_46_configbus1[814:814] , mux_2level_tapbuf_size16_46_configbus0_b[814:814] );
sram6T_blwl sram_blwl_815_ (mux_2level_tapbuf_size16_46_sram_blwl_out[815:815] ,mux_2level_tapbuf_size16_46_sram_blwl_out[815:815] ,mux_2level_tapbuf_size16_46_sram_blwl_outb[815:815] ,mux_2level_tapbuf_size16_46_configbus0[815:815], mux_2level_tapbuf_size16_46_configbus1[815:815] , mux_2level_tapbuf_size16_46_configbus0_b[815:815] );
wire [0:15] mux_2level_tapbuf_size16_47_inbus;
assign mux_2level_tapbuf_size16_47_inbus[0] = chany_0__1__midout_0_;
assign mux_2level_tapbuf_size16_47_inbus[1] = chany_0__1__midout_1_;
assign mux_2level_tapbuf_size16_47_inbus[2] = chany_0__1__midout_14_;
assign mux_2level_tapbuf_size16_47_inbus[3] = chany_0__1__midout_15_;
assign mux_2level_tapbuf_size16_47_inbus[4] = chany_0__1__midout_26_;
assign mux_2level_tapbuf_size16_47_inbus[5] = chany_0__1__midout_27_;
assign mux_2level_tapbuf_size16_47_inbus[6] = chany_0__1__midout_38_;
assign mux_2level_tapbuf_size16_47_inbus[7] = chany_0__1__midout_39_;
assign mux_2level_tapbuf_size16_47_inbus[8] = chany_0__1__midout_50_;
assign mux_2level_tapbuf_size16_47_inbus[9] = chany_0__1__midout_51_;
assign mux_2level_tapbuf_size16_47_inbus[10] = chany_0__1__midout_64_;
assign mux_2level_tapbuf_size16_47_inbus[11] = chany_0__1__midout_65_;
assign mux_2level_tapbuf_size16_47_inbus[12] = chany_0__1__midout_76_;
assign mux_2level_tapbuf_size16_47_inbus[13] = chany_0__1__midout_77_;
assign mux_2level_tapbuf_size16_47_inbus[14] = chany_0__1__midout_88_;
assign mux_2level_tapbuf_size16_47_inbus[15] = chany_0__1__midout_89_;
wire [816:823] mux_2level_tapbuf_size16_47_configbus0;
wire [816:823] mux_2level_tapbuf_size16_47_configbus1;
wire [816:823] mux_2level_tapbuf_size16_47_sram_blwl_out ;
wire [816:823] mux_2level_tapbuf_size16_47_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_47_configbus0[816:823] = sram_blwl_bl[816:823] ;
assign mux_2level_tapbuf_size16_47_configbus1[816:823] = sram_blwl_wl[816:823] ;
wire [816:823] mux_2level_tapbuf_size16_47_configbus0_b;
assign mux_2level_tapbuf_size16_47_configbus0_b[816:823] = sram_blwl_blb[816:823] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_47_ (mux_2level_tapbuf_size16_47_inbus, grid_0__1__pin_0__1__2_, mux_2level_tapbuf_size16_47_sram_blwl_out[816:823] ,
mux_2level_tapbuf_size16_47_sram_blwl_outb[816:823] );
//----- SRAM bits for MUX[47], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_816_ (mux_2level_tapbuf_size16_47_sram_blwl_out[816:816] ,mux_2level_tapbuf_size16_47_sram_blwl_out[816:816] ,mux_2level_tapbuf_size16_47_sram_blwl_outb[816:816] ,mux_2level_tapbuf_size16_47_configbus0[816:816], mux_2level_tapbuf_size16_47_configbus1[816:816] , mux_2level_tapbuf_size16_47_configbus0_b[816:816] );
sram6T_blwl sram_blwl_817_ (mux_2level_tapbuf_size16_47_sram_blwl_out[817:817] ,mux_2level_tapbuf_size16_47_sram_blwl_out[817:817] ,mux_2level_tapbuf_size16_47_sram_blwl_outb[817:817] ,mux_2level_tapbuf_size16_47_configbus0[817:817], mux_2level_tapbuf_size16_47_configbus1[817:817] , mux_2level_tapbuf_size16_47_configbus0_b[817:817] );
sram6T_blwl sram_blwl_818_ (mux_2level_tapbuf_size16_47_sram_blwl_out[818:818] ,mux_2level_tapbuf_size16_47_sram_blwl_out[818:818] ,mux_2level_tapbuf_size16_47_sram_blwl_outb[818:818] ,mux_2level_tapbuf_size16_47_configbus0[818:818], mux_2level_tapbuf_size16_47_configbus1[818:818] , mux_2level_tapbuf_size16_47_configbus0_b[818:818] );
sram6T_blwl sram_blwl_819_ (mux_2level_tapbuf_size16_47_sram_blwl_out[819:819] ,mux_2level_tapbuf_size16_47_sram_blwl_out[819:819] ,mux_2level_tapbuf_size16_47_sram_blwl_outb[819:819] ,mux_2level_tapbuf_size16_47_configbus0[819:819], mux_2level_tapbuf_size16_47_configbus1[819:819] , mux_2level_tapbuf_size16_47_configbus0_b[819:819] );
sram6T_blwl sram_blwl_820_ (mux_2level_tapbuf_size16_47_sram_blwl_out[820:820] ,mux_2level_tapbuf_size16_47_sram_blwl_out[820:820] ,mux_2level_tapbuf_size16_47_sram_blwl_outb[820:820] ,mux_2level_tapbuf_size16_47_configbus0[820:820], mux_2level_tapbuf_size16_47_configbus1[820:820] , mux_2level_tapbuf_size16_47_configbus0_b[820:820] );
sram6T_blwl sram_blwl_821_ (mux_2level_tapbuf_size16_47_sram_blwl_out[821:821] ,mux_2level_tapbuf_size16_47_sram_blwl_out[821:821] ,mux_2level_tapbuf_size16_47_sram_blwl_outb[821:821] ,mux_2level_tapbuf_size16_47_configbus0[821:821], mux_2level_tapbuf_size16_47_configbus1[821:821] , mux_2level_tapbuf_size16_47_configbus0_b[821:821] );
sram6T_blwl sram_blwl_822_ (mux_2level_tapbuf_size16_47_sram_blwl_out[822:822] ,mux_2level_tapbuf_size16_47_sram_blwl_out[822:822] ,mux_2level_tapbuf_size16_47_sram_blwl_outb[822:822] ,mux_2level_tapbuf_size16_47_configbus0[822:822], mux_2level_tapbuf_size16_47_configbus1[822:822] , mux_2level_tapbuf_size16_47_configbus0_b[822:822] );
sram6T_blwl sram_blwl_823_ (mux_2level_tapbuf_size16_47_sram_blwl_out[823:823] ,mux_2level_tapbuf_size16_47_sram_blwl_out[823:823] ,mux_2level_tapbuf_size16_47_sram_blwl_outb[823:823] ,mux_2level_tapbuf_size16_47_configbus0[823:823], mux_2level_tapbuf_size16_47_configbus1[823:823] , mux_2level_tapbuf_size16_47_configbus0_b[823:823] );
wire [0:15] mux_2level_tapbuf_size16_48_inbus;
assign mux_2level_tapbuf_size16_48_inbus[0] = chany_0__1__midout_2_;
assign mux_2level_tapbuf_size16_48_inbus[1] = chany_0__1__midout_3_;
assign mux_2level_tapbuf_size16_48_inbus[2] = chany_0__1__midout_16_;
assign mux_2level_tapbuf_size16_48_inbus[3] = chany_0__1__midout_17_;
assign mux_2level_tapbuf_size16_48_inbus[4] = chany_0__1__midout_28_;
assign mux_2level_tapbuf_size16_48_inbus[5] = chany_0__1__midout_29_;
assign mux_2level_tapbuf_size16_48_inbus[6] = chany_0__1__midout_40_;
assign mux_2level_tapbuf_size16_48_inbus[7] = chany_0__1__midout_41_;
assign mux_2level_tapbuf_size16_48_inbus[8] = chany_0__1__midout_52_;
assign mux_2level_tapbuf_size16_48_inbus[9] = chany_0__1__midout_53_;
assign mux_2level_tapbuf_size16_48_inbus[10] = chany_0__1__midout_66_;
assign mux_2level_tapbuf_size16_48_inbus[11] = chany_0__1__midout_67_;
assign mux_2level_tapbuf_size16_48_inbus[12] = chany_0__1__midout_78_;
assign mux_2level_tapbuf_size16_48_inbus[13] = chany_0__1__midout_79_;
assign mux_2level_tapbuf_size16_48_inbus[14] = chany_0__1__midout_90_;
assign mux_2level_tapbuf_size16_48_inbus[15] = chany_0__1__midout_91_;
wire [824:831] mux_2level_tapbuf_size16_48_configbus0;
wire [824:831] mux_2level_tapbuf_size16_48_configbus1;
wire [824:831] mux_2level_tapbuf_size16_48_sram_blwl_out ;
wire [824:831] mux_2level_tapbuf_size16_48_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_48_configbus0[824:831] = sram_blwl_bl[824:831] ;
assign mux_2level_tapbuf_size16_48_configbus1[824:831] = sram_blwl_wl[824:831] ;
wire [824:831] mux_2level_tapbuf_size16_48_configbus0_b;
assign mux_2level_tapbuf_size16_48_configbus0_b[824:831] = sram_blwl_blb[824:831] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_48_ (mux_2level_tapbuf_size16_48_inbus, grid_0__1__pin_0__1__4_, mux_2level_tapbuf_size16_48_sram_blwl_out[824:831] ,
mux_2level_tapbuf_size16_48_sram_blwl_outb[824:831] );
//----- SRAM bits for MUX[48], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_824_ (mux_2level_tapbuf_size16_48_sram_blwl_out[824:824] ,mux_2level_tapbuf_size16_48_sram_blwl_out[824:824] ,mux_2level_tapbuf_size16_48_sram_blwl_outb[824:824] ,mux_2level_tapbuf_size16_48_configbus0[824:824], mux_2level_tapbuf_size16_48_configbus1[824:824] , mux_2level_tapbuf_size16_48_configbus0_b[824:824] );
sram6T_blwl sram_blwl_825_ (mux_2level_tapbuf_size16_48_sram_blwl_out[825:825] ,mux_2level_tapbuf_size16_48_sram_blwl_out[825:825] ,mux_2level_tapbuf_size16_48_sram_blwl_outb[825:825] ,mux_2level_tapbuf_size16_48_configbus0[825:825], mux_2level_tapbuf_size16_48_configbus1[825:825] , mux_2level_tapbuf_size16_48_configbus0_b[825:825] );
sram6T_blwl sram_blwl_826_ (mux_2level_tapbuf_size16_48_sram_blwl_out[826:826] ,mux_2level_tapbuf_size16_48_sram_blwl_out[826:826] ,mux_2level_tapbuf_size16_48_sram_blwl_outb[826:826] ,mux_2level_tapbuf_size16_48_configbus0[826:826], mux_2level_tapbuf_size16_48_configbus1[826:826] , mux_2level_tapbuf_size16_48_configbus0_b[826:826] );
sram6T_blwl sram_blwl_827_ (mux_2level_tapbuf_size16_48_sram_blwl_out[827:827] ,mux_2level_tapbuf_size16_48_sram_blwl_out[827:827] ,mux_2level_tapbuf_size16_48_sram_blwl_outb[827:827] ,mux_2level_tapbuf_size16_48_configbus0[827:827], mux_2level_tapbuf_size16_48_configbus1[827:827] , mux_2level_tapbuf_size16_48_configbus0_b[827:827] );
sram6T_blwl sram_blwl_828_ (mux_2level_tapbuf_size16_48_sram_blwl_out[828:828] ,mux_2level_tapbuf_size16_48_sram_blwl_out[828:828] ,mux_2level_tapbuf_size16_48_sram_blwl_outb[828:828] ,mux_2level_tapbuf_size16_48_configbus0[828:828], mux_2level_tapbuf_size16_48_configbus1[828:828] , mux_2level_tapbuf_size16_48_configbus0_b[828:828] );
sram6T_blwl sram_blwl_829_ (mux_2level_tapbuf_size16_48_sram_blwl_out[829:829] ,mux_2level_tapbuf_size16_48_sram_blwl_out[829:829] ,mux_2level_tapbuf_size16_48_sram_blwl_outb[829:829] ,mux_2level_tapbuf_size16_48_configbus0[829:829], mux_2level_tapbuf_size16_48_configbus1[829:829] , mux_2level_tapbuf_size16_48_configbus0_b[829:829] );
sram6T_blwl sram_blwl_830_ (mux_2level_tapbuf_size16_48_sram_blwl_out[830:830] ,mux_2level_tapbuf_size16_48_sram_blwl_out[830:830] ,mux_2level_tapbuf_size16_48_sram_blwl_outb[830:830] ,mux_2level_tapbuf_size16_48_configbus0[830:830], mux_2level_tapbuf_size16_48_configbus1[830:830] , mux_2level_tapbuf_size16_48_configbus0_b[830:830] );
sram6T_blwl sram_blwl_831_ (mux_2level_tapbuf_size16_48_sram_blwl_out[831:831] ,mux_2level_tapbuf_size16_48_sram_blwl_out[831:831] ,mux_2level_tapbuf_size16_48_sram_blwl_outb[831:831] ,mux_2level_tapbuf_size16_48_configbus0[831:831], mux_2level_tapbuf_size16_48_configbus1[831:831] , mux_2level_tapbuf_size16_48_configbus0_b[831:831] );
wire [0:15] mux_2level_tapbuf_size16_49_inbus;
assign mux_2level_tapbuf_size16_49_inbus[0] = chany_0__1__midout_4_;
assign mux_2level_tapbuf_size16_49_inbus[1] = chany_0__1__midout_5_;
assign mux_2level_tapbuf_size16_49_inbus[2] = chany_0__1__midout_16_;
assign mux_2level_tapbuf_size16_49_inbus[3] = chany_0__1__midout_17_;
assign mux_2level_tapbuf_size16_49_inbus[4] = chany_0__1__midout_30_;
assign mux_2level_tapbuf_size16_49_inbus[5] = chany_0__1__midout_31_;
assign mux_2level_tapbuf_size16_49_inbus[6] = chany_0__1__midout_42_;
assign mux_2level_tapbuf_size16_49_inbus[7] = chany_0__1__midout_43_;
assign mux_2level_tapbuf_size16_49_inbus[8] = chany_0__1__midout_54_;
assign mux_2level_tapbuf_size16_49_inbus[9] = chany_0__1__midout_55_;
assign mux_2level_tapbuf_size16_49_inbus[10] = chany_0__1__midout_66_;
assign mux_2level_tapbuf_size16_49_inbus[11] = chany_0__1__midout_67_;
assign mux_2level_tapbuf_size16_49_inbus[12] = chany_0__1__midout_80_;
assign mux_2level_tapbuf_size16_49_inbus[13] = chany_0__1__midout_81_;
assign mux_2level_tapbuf_size16_49_inbus[14] = chany_0__1__midout_92_;
assign mux_2level_tapbuf_size16_49_inbus[15] = chany_0__1__midout_93_;
wire [832:839] mux_2level_tapbuf_size16_49_configbus0;
wire [832:839] mux_2level_tapbuf_size16_49_configbus1;
wire [832:839] mux_2level_tapbuf_size16_49_sram_blwl_out ;
wire [832:839] mux_2level_tapbuf_size16_49_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_49_configbus0[832:839] = sram_blwl_bl[832:839] ;
assign mux_2level_tapbuf_size16_49_configbus1[832:839] = sram_blwl_wl[832:839] ;
wire [832:839] mux_2level_tapbuf_size16_49_configbus0_b;
assign mux_2level_tapbuf_size16_49_configbus0_b[832:839] = sram_blwl_blb[832:839] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_49_ (mux_2level_tapbuf_size16_49_inbus, grid_0__1__pin_0__1__6_, mux_2level_tapbuf_size16_49_sram_blwl_out[832:839] ,
mux_2level_tapbuf_size16_49_sram_blwl_outb[832:839] );
//----- SRAM bits for MUX[49], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_832_ (mux_2level_tapbuf_size16_49_sram_blwl_out[832:832] ,mux_2level_tapbuf_size16_49_sram_blwl_out[832:832] ,mux_2level_tapbuf_size16_49_sram_blwl_outb[832:832] ,mux_2level_tapbuf_size16_49_configbus0[832:832], mux_2level_tapbuf_size16_49_configbus1[832:832] , mux_2level_tapbuf_size16_49_configbus0_b[832:832] );
sram6T_blwl sram_blwl_833_ (mux_2level_tapbuf_size16_49_sram_blwl_out[833:833] ,mux_2level_tapbuf_size16_49_sram_blwl_out[833:833] ,mux_2level_tapbuf_size16_49_sram_blwl_outb[833:833] ,mux_2level_tapbuf_size16_49_configbus0[833:833], mux_2level_tapbuf_size16_49_configbus1[833:833] , mux_2level_tapbuf_size16_49_configbus0_b[833:833] );
sram6T_blwl sram_blwl_834_ (mux_2level_tapbuf_size16_49_sram_blwl_out[834:834] ,mux_2level_tapbuf_size16_49_sram_blwl_out[834:834] ,mux_2level_tapbuf_size16_49_sram_blwl_outb[834:834] ,mux_2level_tapbuf_size16_49_configbus0[834:834], mux_2level_tapbuf_size16_49_configbus1[834:834] , mux_2level_tapbuf_size16_49_configbus0_b[834:834] );
sram6T_blwl sram_blwl_835_ (mux_2level_tapbuf_size16_49_sram_blwl_out[835:835] ,mux_2level_tapbuf_size16_49_sram_blwl_out[835:835] ,mux_2level_tapbuf_size16_49_sram_blwl_outb[835:835] ,mux_2level_tapbuf_size16_49_configbus0[835:835], mux_2level_tapbuf_size16_49_configbus1[835:835] , mux_2level_tapbuf_size16_49_configbus0_b[835:835] );
sram6T_blwl sram_blwl_836_ (mux_2level_tapbuf_size16_49_sram_blwl_out[836:836] ,mux_2level_tapbuf_size16_49_sram_blwl_out[836:836] ,mux_2level_tapbuf_size16_49_sram_blwl_outb[836:836] ,mux_2level_tapbuf_size16_49_configbus0[836:836], mux_2level_tapbuf_size16_49_configbus1[836:836] , mux_2level_tapbuf_size16_49_configbus0_b[836:836] );
sram6T_blwl sram_blwl_837_ (mux_2level_tapbuf_size16_49_sram_blwl_out[837:837] ,mux_2level_tapbuf_size16_49_sram_blwl_out[837:837] ,mux_2level_tapbuf_size16_49_sram_blwl_outb[837:837] ,mux_2level_tapbuf_size16_49_configbus0[837:837], mux_2level_tapbuf_size16_49_configbus1[837:837] , mux_2level_tapbuf_size16_49_configbus0_b[837:837] );
sram6T_blwl sram_blwl_838_ (mux_2level_tapbuf_size16_49_sram_blwl_out[838:838] ,mux_2level_tapbuf_size16_49_sram_blwl_out[838:838] ,mux_2level_tapbuf_size16_49_sram_blwl_outb[838:838] ,mux_2level_tapbuf_size16_49_configbus0[838:838], mux_2level_tapbuf_size16_49_configbus1[838:838] , mux_2level_tapbuf_size16_49_configbus0_b[838:838] );
sram6T_blwl sram_blwl_839_ (mux_2level_tapbuf_size16_49_sram_blwl_out[839:839] ,mux_2level_tapbuf_size16_49_sram_blwl_out[839:839] ,mux_2level_tapbuf_size16_49_sram_blwl_outb[839:839] ,mux_2level_tapbuf_size16_49_configbus0[839:839], mux_2level_tapbuf_size16_49_configbus1[839:839] , mux_2level_tapbuf_size16_49_configbus0_b[839:839] );
wire [0:15] mux_2level_tapbuf_size16_50_inbus;
assign mux_2level_tapbuf_size16_50_inbus[0] = chany_0__1__midout_6_;
assign mux_2level_tapbuf_size16_50_inbus[1] = chany_0__1__midout_7_;
assign mux_2level_tapbuf_size16_50_inbus[2] = chany_0__1__midout_18_;
assign mux_2level_tapbuf_size16_50_inbus[3] = chany_0__1__midout_19_;
assign mux_2level_tapbuf_size16_50_inbus[4] = chany_0__1__midout_30_;
assign mux_2level_tapbuf_size16_50_inbus[5] = chany_0__1__midout_31_;
assign mux_2level_tapbuf_size16_50_inbus[6] = chany_0__1__midout_44_;
assign mux_2level_tapbuf_size16_50_inbus[7] = chany_0__1__midout_45_;
assign mux_2level_tapbuf_size16_50_inbus[8] = chany_0__1__midout_56_;
assign mux_2level_tapbuf_size16_50_inbus[9] = chany_0__1__midout_57_;
assign mux_2level_tapbuf_size16_50_inbus[10] = chany_0__1__midout_68_;
assign mux_2level_tapbuf_size16_50_inbus[11] = chany_0__1__midout_69_;
assign mux_2level_tapbuf_size16_50_inbus[12] = chany_0__1__midout_80_;
assign mux_2level_tapbuf_size16_50_inbus[13] = chany_0__1__midout_81_;
assign mux_2level_tapbuf_size16_50_inbus[14] = chany_0__1__midout_94_;
assign mux_2level_tapbuf_size16_50_inbus[15] = chany_0__1__midout_95_;
wire [840:847] mux_2level_tapbuf_size16_50_configbus0;
wire [840:847] mux_2level_tapbuf_size16_50_configbus1;
wire [840:847] mux_2level_tapbuf_size16_50_sram_blwl_out ;
wire [840:847] mux_2level_tapbuf_size16_50_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_50_configbus0[840:847] = sram_blwl_bl[840:847] ;
assign mux_2level_tapbuf_size16_50_configbus1[840:847] = sram_blwl_wl[840:847] ;
wire [840:847] mux_2level_tapbuf_size16_50_configbus0_b;
assign mux_2level_tapbuf_size16_50_configbus0_b[840:847] = sram_blwl_blb[840:847] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_50_ (mux_2level_tapbuf_size16_50_inbus, grid_0__1__pin_0__1__8_, mux_2level_tapbuf_size16_50_sram_blwl_out[840:847] ,
mux_2level_tapbuf_size16_50_sram_blwl_outb[840:847] );
//----- SRAM bits for MUX[50], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_840_ (mux_2level_tapbuf_size16_50_sram_blwl_out[840:840] ,mux_2level_tapbuf_size16_50_sram_blwl_out[840:840] ,mux_2level_tapbuf_size16_50_sram_blwl_outb[840:840] ,mux_2level_tapbuf_size16_50_configbus0[840:840], mux_2level_tapbuf_size16_50_configbus1[840:840] , mux_2level_tapbuf_size16_50_configbus0_b[840:840] );
sram6T_blwl sram_blwl_841_ (mux_2level_tapbuf_size16_50_sram_blwl_out[841:841] ,mux_2level_tapbuf_size16_50_sram_blwl_out[841:841] ,mux_2level_tapbuf_size16_50_sram_blwl_outb[841:841] ,mux_2level_tapbuf_size16_50_configbus0[841:841], mux_2level_tapbuf_size16_50_configbus1[841:841] , mux_2level_tapbuf_size16_50_configbus0_b[841:841] );
sram6T_blwl sram_blwl_842_ (mux_2level_tapbuf_size16_50_sram_blwl_out[842:842] ,mux_2level_tapbuf_size16_50_sram_blwl_out[842:842] ,mux_2level_tapbuf_size16_50_sram_blwl_outb[842:842] ,mux_2level_tapbuf_size16_50_configbus0[842:842], mux_2level_tapbuf_size16_50_configbus1[842:842] , mux_2level_tapbuf_size16_50_configbus0_b[842:842] );
sram6T_blwl sram_blwl_843_ (mux_2level_tapbuf_size16_50_sram_blwl_out[843:843] ,mux_2level_tapbuf_size16_50_sram_blwl_out[843:843] ,mux_2level_tapbuf_size16_50_sram_blwl_outb[843:843] ,mux_2level_tapbuf_size16_50_configbus0[843:843], mux_2level_tapbuf_size16_50_configbus1[843:843] , mux_2level_tapbuf_size16_50_configbus0_b[843:843] );
sram6T_blwl sram_blwl_844_ (mux_2level_tapbuf_size16_50_sram_blwl_out[844:844] ,mux_2level_tapbuf_size16_50_sram_blwl_out[844:844] ,mux_2level_tapbuf_size16_50_sram_blwl_outb[844:844] ,mux_2level_tapbuf_size16_50_configbus0[844:844], mux_2level_tapbuf_size16_50_configbus1[844:844] , mux_2level_tapbuf_size16_50_configbus0_b[844:844] );
sram6T_blwl sram_blwl_845_ (mux_2level_tapbuf_size16_50_sram_blwl_out[845:845] ,mux_2level_tapbuf_size16_50_sram_blwl_out[845:845] ,mux_2level_tapbuf_size16_50_sram_blwl_outb[845:845] ,mux_2level_tapbuf_size16_50_configbus0[845:845], mux_2level_tapbuf_size16_50_configbus1[845:845] , mux_2level_tapbuf_size16_50_configbus0_b[845:845] );
sram6T_blwl sram_blwl_846_ (mux_2level_tapbuf_size16_50_sram_blwl_out[846:846] ,mux_2level_tapbuf_size16_50_sram_blwl_out[846:846] ,mux_2level_tapbuf_size16_50_sram_blwl_outb[846:846] ,mux_2level_tapbuf_size16_50_configbus0[846:846], mux_2level_tapbuf_size16_50_configbus1[846:846] , mux_2level_tapbuf_size16_50_configbus0_b[846:846] );
sram6T_blwl sram_blwl_847_ (mux_2level_tapbuf_size16_50_sram_blwl_out[847:847] ,mux_2level_tapbuf_size16_50_sram_blwl_out[847:847] ,mux_2level_tapbuf_size16_50_sram_blwl_outb[847:847] ,mux_2level_tapbuf_size16_50_configbus0[847:847], mux_2level_tapbuf_size16_50_configbus1[847:847] , mux_2level_tapbuf_size16_50_configbus0_b[847:847] );
wire [0:15] mux_2level_tapbuf_size16_51_inbus;
assign mux_2level_tapbuf_size16_51_inbus[0] = chany_0__1__midout_8_;
assign mux_2level_tapbuf_size16_51_inbus[1] = chany_0__1__midout_9_;
assign mux_2level_tapbuf_size16_51_inbus[2] = chany_0__1__midout_20_;
assign mux_2level_tapbuf_size16_51_inbus[3] = chany_0__1__midout_21_;
assign mux_2level_tapbuf_size16_51_inbus[4] = chany_0__1__midout_32_;
assign mux_2level_tapbuf_size16_51_inbus[5] = chany_0__1__midout_33_;
assign mux_2level_tapbuf_size16_51_inbus[6] = chany_0__1__midout_44_;
assign mux_2level_tapbuf_size16_51_inbus[7] = chany_0__1__midout_45_;
assign mux_2level_tapbuf_size16_51_inbus[8] = chany_0__1__midout_58_;
assign mux_2level_tapbuf_size16_51_inbus[9] = chany_0__1__midout_59_;
assign mux_2level_tapbuf_size16_51_inbus[10] = chany_0__1__midout_70_;
assign mux_2level_tapbuf_size16_51_inbus[11] = chany_0__1__midout_71_;
assign mux_2level_tapbuf_size16_51_inbus[12] = chany_0__1__midout_82_;
assign mux_2level_tapbuf_size16_51_inbus[13] = chany_0__1__midout_83_;
assign mux_2level_tapbuf_size16_51_inbus[14] = chany_0__1__midout_94_;
assign mux_2level_tapbuf_size16_51_inbus[15] = chany_0__1__midout_95_;
wire [848:855] mux_2level_tapbuf_size16_51_configbus0;
wire [848:855] mux_2level_tapbuf_size16_51_configbus1;
wire [848:855] mux_2level_tapbuf_size16_51_sram_blwl_out ;
wire [848:855] mux_2level_tapbuf_size16_51_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_51_configbus0[848:855] = sram_blwl_bl[848:855] ;
assign mux_2level_tapbuf_size16_51_configbus1[848:855] = sram_blwl_wl[848:855] ;
wire [848:855] mux_2level_tapbuf_size16_51_configbus0_b;
assign mux_2level_tapbuf_size16_51_configbus0_b[848:855] = sram_blwl_blb[848:855] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_51_ (mux_2level_tapbuf_size16_51_inbus, grid_0__1__pin_0__1__10_, mux_2level_tapbuf_size16_51_sram_blwl_out[848:855] ,
mux_2level_tapbuf_size16_51_sram_blwl_outb[848:855] );
//----- SRAM bits for MUX[51], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_848_ (mux_2level_tapbuf_size16_51_sram_blwl_out[848:848] ,mux_2level_tapbuf_size16_51_sram_blwl_out[848:848] ,mux_2level_tapbuf_size16_51_sram_blwl_outb[848:848] ,mux_2level_tapbuf_size16_51_configbus0[848:848], mux_2level_tapbuf_size16_51_configbus1[848:848] , mux_2level_tapbuf_size16_51_configbus0_b[848:848] );
sram6T_blwl sram_blwl_849_ (mux_2level_tapbuf_size16_51_sram_blwl_out[849:849] ,mux_2level_tapbuf_size16_51_sram_blwl_out[849:849] ,mux_2level_tapbuf_size16_51_sram_blwl_outb[849:849] ,mux_2level_tapbuf_size16_51_configbus0[849:849], mux_2level_tapbuf_size16_51_configbus1[849:849] , mux_2level_tapbuf_size16_51_configbus0_b[849:849] );
sram6T_blwl sram_blwl_850_ (mux_2level_tapbuf_size16_51_sram_blwl_out[850:850] ,mux_2level_tapbuf_size16_51_sram_blwl_out[850:850] ,mux_2level_tapbuf_size16_51_sram_blwl_outb[850:850] ,mux_2level_tapbuf_size16_51_configbus0[850:850], mux_2level_tapbuf_size16_51_configbus1[850:850] , mux_2level_tapbuf_size16_51_configbus0_b[850:850] );
sram6T_blwl sram_blwl_851_ (mux_2level_tapbuf_size16_51_sram_blwl_out[851:851] ,mux_2level_tapbuf_size16_51_sram_blwl_out[851:851] ,mux_2level_tapbuf_size16_51_sram_blwl_outb[851:851] ,mux_2level_tapbuf_size16_51_configbus0[851:851], mux_2level_tapbuf_size16_51_configbus1[851:851] , mux_2level_tapbuf_size16_51_configbus0_b[851:851] );
sram6T_blwl sram_blwl_852_ (mux_2level_tapbuf_size16_51_sram_blwl_out[852:852] ,mux_2level_tapbuf_size16_51_sram_blwl_out[852:852] ,mux_2level_tapbuf_size16_51_sram_blwl_outb[852:852] ,mux_2level_tapbuf_size16_51_configbus0[852:852], mux_2level_tapbuf_size16_51_configbus1[852:852] , mux_2level_tapbuf_size16_51_configbus0_b[852:852] );
sram6T_blwl sram_blwl_853_ (mux_2level_tapbuf_size16_51_sram_blwl_out[853:853] ,mux_2level_tapbuf_size16_51_sram_blwl_out[853:853] ,mux_2level_tapbuf_size16_51_sram_blwl_outb[853:853] ,mux_2level_tapbuf_size16_51_configbus0[853:853], mux_2level_tapbuf_size16_51_configbus1[853:853] , mux_2level_tapbuf_size16_51_configbus0_b[853:853] );
sram6T_blwl sram_blwl_854_ (mux_2level_tapbuf_size16_51_sram_blwl_out[854:854] ,mux_2level_tapbuf_size16_51_sram_blwl_out[854:854] ,mux_2level_tapbuf_size16_51_sram_blwl_outb[854:854] ,mux_2level_tapbuf_size16_51_configbus0[854:854], mux_2level_tapbuf_size16_51_configbus1[854:854] , mux_2level_tapbuf_size16_51_configbus0_b[854:854] );
sram6T_blwl sram_blwl_855_ (mux_2level_tapbuf_size16_51_sram_blwl_out[855:855] ,mux_2level_tapbuf_size16_51_sram_blwl_out[855:855] ,mux_2level_tapbuf_size16_51_sram_blwl_outb[855:855] ,mux_2level_tapbuf_size16_51_configbus0[855:855], mux_2level_tapbuf_size16_51_configbus1[855:855] , mux_2level_tapbuf_size16_51_configbus0_b[855:855] );
wire [0:15] mux_2level_tapbuf_size16_52_inbus;
assign mux_2level_tapbuf_size16_52_inbus[0] = chany_0__1__midout_8_;
assign mux_2level_tapbuf_size16_52_inbus[1] = chany_0__1__midout_9_;
assign mux_2level_tapbuf_size16_52_inbus[2] = chany_0__1__midout_22_;
assign mux_2level_tapbuf_size16_52_inbus[3] = chany_0__1__midout_23_;
assign mux_2level_tapbuf_size16_52_inbus[4] = chany_0__1__midout_34_;
assign mux_2level_tapbuf_size16_52_inbus[5] = chany_0__1__midout_35_;
assign mux_2level_tapbuf_size16_52_inbus[6] = chany_0__1__midout_46_;
assign mux_2level_tapbuf_size16_52_inbus[7] = chany_0__1__midout_47_;
assign mux_2level_tapbuf_size16_52_inbus[8] = chany_0__1__midout_58_;
assign mux_2level_tapbuf_size16_52_inbus[9] = chany_0__1__midout_59_;
assign mux_2level_tapbuf_size16_52_inbus[10] = chany_0__1__midout_72_;
assign mux_2level_tapbuf_size16_52_inbus[11] = chany_0__1__midout_73_;
assign mux_2level_tapbuf_size16_52_inbus[12] = chany_0__1__midout_84_;
assign mux_2level_tapbuf_size16_52_inbus[13] = chany_0__1__midout_85_;
assign mux_2level_tapbuf_size16_52_inbus[14] = chany_0__1__midout_96_;
assign mux_2level_tapbuf_size16_52_inbus[15] = chany_0__1__midout_97_;
wire [856:863] mux_2level_tapbuf_size16_52_configbus0;
wire [856:863] mux_2level_tapbuf_size16_52_configbus1;
wire [856:863] mux_2level_tapbuf_size16_52_sram_blwl_out ;
wire [856:863] mux_2level_tapbuf_size16_52_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_52_configbus0[856:863] = sram_blwl_bl[856:863] ;
assign mux_2level_tapbuf_size16_52_configbus1[856:863] = sram_blwl_wl[856:863] ;
wire [856:863] mux_2level_tapbuf_size16_52_configbus0_b;
assign mux_2level_tapbuf_size16_52_configbus0_b[856:863] = sram_blwl_blb[856:863] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_52_ (mux_2level_tapbuf_size16_52_inbus, grid_0__1__pin_0__1__12_, mux_2level_tapbuf_size16_52_sram_blwl_out[856:863] ,
mux_2level_tapbuf_size16_52_sram_blwl_outb[856:863] );
//----- SRAM bits for MUX[52], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_856_ (mux_2level_tapbuf_size16_52_sram_blwl_out[856:856] ,mux_2level_tapbuf_size16_52_sram_blwl_out[856:856] ,mux_2level_tapbuf_size16_52_sram_blwl_outb[856:856] ,mux_2level_tapbuf_size16_52_configbus0[856:856], mux_2level_tapbuf_size16_52_configbus1[856:856] , mux_2level_tapbuf_size16_52_configbus0_b[856:856] );
sram6T_blwl sram_blwl_857_ (mux_2level_tapbuf_size16_52_sram_blwl_out[857:857] ,mux_2level_tapbuf_size16_52_sram_blwl_out[857:857] ,mux_2level_tapbuf_size16_52_sram_blwl_outb[857:857] ,mux_2level_tapbuf_size16_52_configbus0[857:857], mux_2level_tapbuf_size16_52_configbus1[857:857] , mux_2level_tapbuf_size16_52_configbus0_b[857:857] );
sram6T_blwl sram_blwl_858_ (mux_2level_tapbuf_size16_52_sram_blwl_out[858:858] ,mux_2level_tapbuf_size16_52_sram_blwl_out[858:858] ,mux_2level_tapbuf_size16_52_sram_blwl_outb[858:858] ,mux_2level_tapbuf_size16_52_configbus0[858:858], mux_2level_tapbuf_size16_52_configbus1[858:858] , mux_2level_tapbuf_size16_52_configbus0_b[858:858] );
sram6T_blwl sram_blwl_859_ (mux_2level_tapbuf_size16_52_sram_blwl_out[859:859] ,mux_2level_tapbuf_size16_52_sram_blwl_out[859:859] ,mux_2level_tapbuf_size16_52_sram_blwl_outb[859:859] ,mux_2level_tapbuf_size16_52_configbus0[859:859], mux_2level_tapbuf_size16_52_configbus1[859:859] , mux_2level_tapbuf_size16_52_configbus0_b[859:859] );
sram6T_blwl sram_blwl_860_ (mux_2level_tapbuf_size16_52_sram_blwl_out[860:860] ,mux_2level_tapbuf_size16_52_sram_blwl_out[860:860] ,mux_2level_tapbuf_size16_52_sram_blwl_outb[860:860] ,mux_2level_tapbuf_size16_52_configbus0[860:860], mux_2level_tapbuf_size16_52_configbus1[860:860] , mux_2level_tapbuf_size16_52_configbus0_b[860:860] );
sram6T_blwl sram_blwl_861_ (mux_2level_tapbuf_size16_52_sram_blwl_out[861:861] ,mux_2level_tapbuf_size16_52_sram_blwl_out[861:861] ,mux_2level_tapbuf_size16_52_sram_blwl_outb[861:861] ,mux_2level_tapbuf_size16_52_configbus0[861:861], mux_2level_tapbuf_size16_52_configbus1[861:861] , mux_2level_tapbuf_size16_52_configbus0_b[861:861] );
sram6T_blwl sram_blwl_862_ (mux_2level_tapbuf_size16_52_sram_blwl_out[862:862] ,mux_2level_tapbuf_size16_52_sram_blwl_out[862:862] ,mux_2level_tapbuf_size16_52_sram_blwl_outb[862:862] ,mux_2level_tapbuf_size16_52_configbus0[862:862], mux_2level_tapbuf_size16_52_configbus1[862:862] , mux_2level_tapbuf_size16_52_configbus0_b[862:862] );
sram6T_blwl sram_blwl_863_ (mux_2level_tapbuf_size16_52_sram_blwl_out[863:863] ,mux_2level_tapbuf_size16_52_sram_blwl_out[863:863] ,mux_2level_tapbuf_size16_52_sram_blwl_outb[863:863] ,mux_2level_tapbuf_size16_52_configbus0[863:863], mux_2level_tapbuf_size16_52_configbus1[863:863] , mux_2level_tapbuf_size16_52_configbus0_b[863:863] );
wire [0:15] mux_2level_tapbuf_size16_53_inbus;
assign mux_2level_tapbuf_size16_53_inbus[0] = chany_0__1__midout_10_;
assign mux_2level_tapbuf_size16_53_inbus[1] = chany_0__1__midout_11_;
assign mux_2level_tapbuf_size16_53_inbus[2] = chany_0__1__midout_22_;
assign mux_2level_tapbuf_size16_53_inbus[3] = chany_0__1__midout_23_;
assign mux_2level_tapbuf_size16_53_inbus[4] = chany_0__1__midout_36_;
assign mux_2level_tapbuf_size16_53_inbus[5] = chany_0__1__midout_37_;
assign mux_2level_tapbuf_size16_53_inbus[6] = chany_0__1__midout_48_;
assign mux_2level_tapbuf_size16_53_inbus[7] = chany_0__1__midout_49_;
assign mux_2level_tapbuf_size16_53_inbus[8] = chany_0__1__midout_60_;
assign mux_2level_tapbuf_size16_53_inbus[9] = chany_0__1__midout_61_;
assign mux_2level_tapbuf_size16_53_inbus[10] = chany_0__1__midout_72_;
assign mux_2level_tapbuf_size16_53_inbus[11] = chany_0__1__midout_73_;
assign mux_2level_tapbuf_size16_53_inbus[12] = chany_0__1__midout_86_;
assign mux_2level_tapbuf_size16_53_inbus[13] = chany_0__1__midout_87_;
assign mux_2level_tapbuf_size16_53_inbus[14] = chany_0__1__midout_98_;
assign mux_2level_tapbuf_size16_53_inbus[15] = chany_0__1__midout_99_;
wire [864:871] mux_2level_tapbuf_size16_53_configbus0;
wire [864:871] mux_2level_tapbuf_size16_53_configbus1;
wire [864:871] mux_2level_tapbuf_size16_53_sram_blwl_out ;
wire [864:871] mux_2level_tapbuf_size16_53_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_53_configbus0[864:871] = sram_blwl_bl[864:871] ;
assign mux_2level_tapbuf_size16_53_configbus1[864:871] = sram_blwl_wl[864:871] ;
wire [864:871] mux_2level_tapbuf_size16_53_configbus0_b;
assign mux_2level_tapbuf_size16_53_configbus0_b[864:871] = sram_blwl_blb[864:871] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_53_ (mux_2level_tapbuf_size16_53_inbus, grid_0__1__pin_0__1__14_, mux_2level_tapbuf_size16_53_sram_blwl_out[864:871] ,
mux_2level_tapbuf_size16_53_sram_blwl_outb[864:871] );
//----- SRAM bits for MUX[53], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_864_ (mux_2level_tapbuf_size16_53_sram_blwl_out[864:864] ,mux_2level_tapbuf_size16_53_sram_blwl_out[864:864] ,mux_2level_tapbuf_size16_53_sram_blwl_outb[864:864] ,mux_2level_tapbuf_size16_53_configbus0[864:864], mux_2level_tapbuf_size16_53_configbus1[864:864] , mux_2level_tapbuf_size16_53_configbus0_b[864:864] );
sram6T_blwl sram_blwl_865_ (mux_2level_tapbuf_size16_53_sram_blwl_out[865:865] ,mux_2level_tapbuf_size16_53_sram_blwl_out[865:865] ,mux_2level_tapbuf_size16_53_sram_blwl_outb[865:865] ,mux_2level_tapbuf_size16_53_configbus0[865:865], mux_2level_tapbuf_size16_53_configbus1[865:865] , mux_2level_tapbuf_size16_53_configbus0_b[865:865] );
sram6T_blwl sram_blwl_866_ (mux_2level_tapbuf_size16_53_sram_blwl_out[866:866] ,mux_2level_tapbuf_size16_53_sram_blwl_out[866:866] ,mux_2level_tapbuf_size16_53_sram_blwl_outb[866:866] ,mux_2level_tapbuf_size16_53_configbus0[866:866], mux_2level_tapbuf_size16_53_configbus1[866:866] , mux_2level_tapbuf_size16_53_configbus0_b[866:866] );
sram6T_blwl sram_blwl_867_ (mux_2level_tapbuf_size16_53_sram_blwl_out[867:867] ,mux_2level_tapbuf_size16_53_sram_blwl_out[867:867] ,mux_2level_tapbuf_size16_53_sram_blwl_outb[867:867] ,mux_2level_tapbuf_size16_53_configbus0[867:867], mux_2level_tapbuf_size16_53_configbus1[867:867] , mux_2level_tapbuf_size16_53_configbus0_b[867:867] );
sram6T_blwl sram_blwl_868_ (mux_2level_tapbuf_size16_53_sram_blwl_out[868:868] ,mux_2level_tapbuf_size16_53_sram_blwl_out[868:868] ,mux_2level_tapbuf_size16_53_sram_blwl_outb[868:868] ,mux_2level_tapbuf_size16_53_configbus0[868:868], mux_2level_tapbuf_size16_53_configbus1[868:868] , mux_2level_tapbuf_size16_53_configbus0_b[868:868] );
sram6T_blwl sram_blwl_869_ (mux_2level_tapbuf_size16_53_sram_blwl_out[869:869] ,mux_2level_tapbuf_size16_53_sram_blwl_out[869:869] ,mux_2level_tapbuf_size16_53_sram_blwl_outb[869:869] ,mux_2level_tapbuf_size16_53_configbus0[869:869], mux_2level_tapbuf_size16_53_configbus1[869:869] , mux_2level_tapbuf_size16_53_configbus0_b[869:869] );
sram6T_blwl sram_blwl_870_ (mux_2level_tapbuf_size16_53_sram_blwl_out[870:870] ,mux_2level_tapbuf_size16_53_sram_blwl_out[870:870] ,mux_2level_tapbuf_size16_53_sram_blwl_outb[870:870] ,mux_2level_tapbuf_size16_53_configbus0[870:870], mux_2level_tapbuf_size16_53_configbus1[870:870] , mux_2level_tapbuf_size16_53_configbus0_b[870:870] );
sram6T_blwl sram_blwl_871_ (mux_2level_tapbuf_size16_53_sram_blwl_out[871:871] ,mux_2level_tapbuf_size16_53_sram_blwl_out[871:871] ,mux_2level_tapbuf_size16_53_sram_blwl_outb[871:871] ,mux_2level_tapbuf_size16_53_configbus0[871:871], mux_2level_tapbuf_size16_53_configbus1[871:871] , mux_2level_tapbuf_size16_53_configbus0_b[871:871] );
endmodule
//----- END Verilog Module of Connection Box -Y direction [0][1] -----

