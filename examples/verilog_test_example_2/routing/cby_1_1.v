//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Connection Block - Y direction  [1][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Verilog Module of Connection Box -Y direction [1][1] -----
module cby_1__1_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input chany_1__1__midout_0_, 

input chany_1__1__midout_1_, 

input chany_1__1__midout_2_, 

input chany_1__1__midout_3_, 

input chany_1__1__midout_4_, 

input chany_1__1__midout_5_, 

input chany_1__1__midout_6_, 

input chany_1__1__midout_7_, 

input chany_1__1__midout_8_, 

input chany_1__1__midout_9_, 

input chany_1__1__midout_10_, 

input chany_1__1__midout_11_, 

input chany_1__1__midout_12_, 

input chany_1__1__midout_13_, 

input chany_1__1__midout_14_, 

input chany_1__1__midout_15_, 

input chany_1__1__midout_16_, 

input chany_1__1__midout_17_, 

input chany_1__1__midout_18_, 

input chany_1__1__midout_19_, 

input chany_1__1__midout_20_, 

input chany_1__1__midout_21_, 

input chany_1__1__midout_22_, 

input chany_1__1__midout_23_, 

input chany_1__1__midout_24_, 

input chany_1__1__midout_25_, 

input chany_1__1__midout_26_, 

input chany_1__1__midout_27_, 

input chany_1__1__midout_28_, 

input chany_1__1__midout_29_, 

input chany_1__1__midout_30_, 

input chany_1__1__midout_31_, 

input chany_1__1__midout_32_, 

input chany_1__1__midout_33_, 

input chany_1__1__midout_34_, 

input chany_1__1__midout_35_, 

input chany_1__1__midout_36_, 

input chany_1__1__midout_37_, 

input chany_1__1__midout_38_, 

input chany_1__1__midout_39_, 

input chany_1__1__midout_40_, 

input chany_1__1__midout_41_, 

input chany_1__1__midout_42_, 

input chany_1__1__midout_43_, 

input chany_1__1__midout_44_, 

input chany_1__1__midout_45_, 

input chany_1__1__midout_46_, 

input chany_1__1__midout_47_, 

input chany_1__1__midout_48_, 

input chany_1__1__midout_49_, 

input chany_1__1__midout_50_, 

input chany_1__1__midout_51_, 

input chany_1__1__midout_52_, 

input chany_1__1__midout_53_, 

input chany_1__1__midout_54_, 

input chany_1__1__midout_55_, 

input chany_1__1__midout_56_, 

input chany_1__1__midout_57_, 

input chany_1__1__midout_58_, 

input chany_1__1__midout_59_, 

input chany_1__1__midout_60_, 

input chany_1__1__midout_61_, 

input chany_1__1__midout_62_, 

input chany_1__1__midout_63_, 

input chany_1__1__midout_64_, 

input chany_1__1__midout_65_, 

input chany_1__1__midout_66_, 

input chany_1__1__midout_67_, 

input chany_1__1__midout_68_, 

input chany_1__1__midout_69_, 

input chany_1__1__midout_70_, 

input chany_1__1__midout_71_, 

input chany_1__1__midout_72_, 

input chany_1__1__midout_73_, 

input chany_1__1__midout_74_, 

input chany_1__1__midout_75_, 

input chany_1__1__midout_76_, 

input chany_1__1__midout_77_, 

input chany_1__1__midout_78_, 

input chany_1__1__midout_79_, 

input chany_1__1__midout_80_, 

input chany_1__1__midout_81_, 

input chany_1__1__midout_82_, 

input chany_1__1__midout_83_, 

input chany_1__1__midout_84_, 

input chany_1__1__midout_85_, 

input chany_1__1__midout_86_, 

input chany_1__1__midout_87_, 

input chany_1__1__midout_88_, 

input chany_1__1__midout_89_, 

input chany_1__1__midout_90_, 

input chany_1__1__midout_91_, 

input chany_1__1__midout_92_, 

input chany_1__1__midout_93_, 

input chany_1__1__midout_94_, 

input chany_1__1__midout_95_, 

input chany_1__1__midout_96_, 

input chany_1__1__midout_97_, 

input chany_1__1__midout_98_, 

input chany_1__1__midout_99_, 

output  grid_2__1__pin_0__3__0_,

output  grid_2__1__pin_0__3__2_,

output  grid_2__1__pin_0__3__4_,

output  grid_2__1__pin_0__3__6_,

output  grid_2__1__pin_0__3__8_,

output  grid_2__1__pin_0__3__10_,

output  grid_2__1__pin_0__3__12_,

output  grid_2__1__pin_0__3__14_,

output  grid_1__1__pin_0__1__1_,

output  grid_1__1__pin_0__1__5_,

output  grid_1__1__pin_0__1__9_,

output  grid_1__1__pin_0__1__13_,

output  grid_1__1__pin_0__1__17_,

output  grid_1__1__pin_0__1__21_,

output  grid_1__1__pin_0__1__25_,

output  grid_1__1__pin_0__1__29_,

output  grid_1__1__pin_0__1__33_,

output  grid_1__1__pin_0__1__37_,

input [872:1015] sram_blwl_bl ,
input [872:1015] sram_blwl_wl ,
input [872:1015] sram_blwl_blb );
wire [0:15] mux_2level_tapbuf_size16_54_inbus;
assign mux_2level_tapbuf_size16_54_inbus[0] = chany_1__1__midout_6_;
assign mux_2level_tapbuf_size16_54_inbus[1] = chany_1__1__midout_7_;
assign mux_2level_tapbuf_size16_54_inbus[2] = chany_1__1__midout_10_;
assign mux_2level_tapbuf_size16_54_inbus[3] = chany_1__1__midout_11_;
assign mux_2level_tapbuf_size16_54_inbus[4] = chany_1__1__midout_24_;
assign mux_2level_tapbuf_size16_54_inbus[5] = chany_1__1__midout_25_;
assign mux_2level_tapbuf_size16_54_inbus[6] = chany_1__1__midout_36_;
assign mux_2level_tapbuf_size16_54_inbus[7] = chany_1__1__midout_37_;
assign mux_2level_tapbuf_size16_54_inbus[8] = chany_1__1__midout_48_;
assign mux_2level_tapbuf_size16_54_inbus[9] = chany_1__1__midout_49_;
assign mux_2level_tapbuf_size16_54_inbus[10] = chany_1__1__midout_60_;
assign mux_2level_tapbuf_size16_54_inbus[11] = chany_1__1__midout_61_;
assign mux_2level_tapbuf_size16_54_inbus[12] = chany_1__1__midout_76_;
assign mux_2level_tapbuf_size16_54_inbus[13] = chany_1__1__midout_77_;
assign mux_2level_tapbuf_size16_54_inbus[14] = chany_1__1__midout_88_;
assign mux_2level_tapbuf_size16_54_inbus[15] = chany_1__1__midout_89_;
wire [872:879] mux_2level_tapbuf_size16_54_configbus0;
wire [872:879] mux_2level_tapbuf_size16_54_configbus1;
wire [872:879] mux_2level_tapbuf_size16_54_sram_blwl_out ;
wire [872:879] mux_2level_tapbuf_size16_54_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_54_configbus0[872:879] = sram_blwl_bl[872:879] ;
assign mux_2level_tapbuf_size16_54_configbus1[872:879] = sram_blwl_wl[872:879] ;
wire [872:879] mux_2level_tapbuf_size16_54_configbus0_b;
assign mux_2level_tapbuf_size16_54_configbus0_b[872:879] = sram_blwl_blb[872:879] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_54_ (mux_2level_tapbuf_size16_54_inbus, grid_2__1__pin_0__3__0_, mux_2level_tapbuf_size16_54_sram_blwl_out[872:879] ,
mux_2level_tapbuf_size16_54_sram_blwl_outb[872:879] );
//----- SRAM bits for MUX[54], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_872_ (mux_2level_tapbuf_size16_54_sram_blwl_out[872:872] ,mux_2level_tapbuf_size16_54_sram_blwl_out[872:872] ,mux_2level_tapbuf_size16_54_sram_blwl_outb[872:872] ,mux_2level_tapbuf_size16_54_configbus0[872:872], mux_2level_tapbuf_size16_54_configbus1[872:872] , mux_2level_tapbuf_size16_54_configbus0_b[872:872] );
sram6T_blwl sram_blwl_873_ (mux_2level_tapbuf_size16_54_sram_blwl_out[873:873] ,mux_2level_tapbuf_size16_54_sram_blwl_out[873:873] ,mux_2level_tapbuf_size16_54_sram_blwl_outb[873:873] ,mux_2level_tapbuf_size16_54_configbus0[873:873], mux_2level_tapbuf_size16_54_configbus1[873:873] , mux_2level_tapbuf_size16_54_configbus0_b[873:873] );
sram6T_blwl sram_blwl_874_ (mux_2level_tapbuf_size16_54_sram_blwl_out[874:874] ,mux_2level_tapbuf_size16_54_sram_blwl_out[874:874] ,mux_2level_tapbuf_size16_54_sram_blwl_outb[874:874] ,mux_2level_tapbuf_size16_54_configbus0[874:874], mux_2level_tapbuf_size16_54_configbus1[874:874] , mux_2level_tapbuf_size16_54_configbus0_b[874:874] );
sram6T_blwl sram_blwl_875_ (mux_2level_tapbuf_size16_54_sram_blwl_out[875:875] ,mux_2level_tapbuf_size16_54_sram_blwl_out[875:875] ,mux_2level_tapbuf_size16_54_sram_blwl_outb[875:875] ,mux_2level_tapbuf_size16_54_configbus0[875:875], mux_2level_tapbuf_size16_54_configbus1[875:875] , mux_2level_tapbuf_size16_54_configbus0_b[875:875] );
sram6T_blwl sram_blwl_876_ (mux_2level_tapbuf_size16_54_sram_blwl_out[876:876] ,mux_2level_tapbuf_size16_54_sram_blwl_out[876:876] ,mux_2level_tapbuf_size16_54_sram_blwl_outb[876:876] ,mux_2level_tapbuf_size16_54_configbus0[876:876], mux_2level_tapbuf_size16_54_configbus1[876:876] , mux_2level_tapbuf_size16_54_configbus0_b[876:876] );
sram6T_blwl sram_blwl_877_ (mux_2level_tapbuf_size16_54_sram_blwl_out[877:877] ,mux_2level_tapbuf_size16_54_sram_blwl_out[877:877] ,mux_2level_tapbuf_size16_54_sram_blwl_outb[877:877] ,mux_2level_tapbuf_size16_54_configbus0[877:877], mux_2level_tapbuf_size16_54_configbus1[877:877] , mux_2level_tapbuf_size16_54_configbus0_b[877:877] );
sram6T_blwl sram_blwl_878_ (mux_2level_tapbuf_size16_54_sram_blwl_out[878:878] ,mux_2level_tapbuf_size16_54_sram_blwl_out[878:878] ,mux_2level_tapbuf_size16_54_sram_blwl_outb[878:878] ,mux_2level_tapbuf_size16_54_configbus0[878:878], mux_2level_tapbuf_size16_54_configbus1[878:878] , mux_2level_tapbuf_size16_54_configbus0_b[878:878] );
sram6T_blwl sram_blwl_879_ (mux_2level_tapbuf_size16_54_sram_blwl_out[879:879] ,mux_2level_tapbuf_size16_54_sram_blwl_out[879:879] ,mux_2level_tapbuf_size16_54_sram_blwl_outb[879:879] ,mux_2level_tapbuf_size16_54_configbus0[879:879], mux_2level_tapbuf_size16_54_configbus1[879:879] , mux_2level_tapbuf_size16_54_configbus0_b[879:879] );
wire [0:15] mux_2level_tapbuf_size16_55_inbus;
assign mux_2level_tapbuf_size16_55_inbus[0] = chany_1__1__midout_0_;
assign mux_2level_tapbuf_size16_55_inbus[1] = chany_1__1__midout_1_;
assign mux_2level_tapbuf_size16_55_inbus[2] = chany_1__1__midout_12_;
assign mux_2level_tapbuf_size16_55_inbus[3] = chany_1__1__midout_13_;
assign mux_2level_tapbuf_size16_55_inbus[4] = chany_1__1__midout_24_;
assign mux_2level_tapbuf_size16_55_inbus[5] = chany_1__1__midout_25_;
assign mux_2level_tapbuf_size16_55_inbus[6] = chany_1__1__midout_42_;
assign mux_2level_tapbuf_size16_55_inbus[7] = chany_1__1__midout_43_;
assign mux_2level_tapbuf_size16_55_inbus[8] = chany_1__1__midout_54_;
assign mux_2level_tapbuf_size16_55_inbus[9] = chany_1__1__midout_55_;
assign mux_2level_tapbuf_size16_55_inbus[10] = chany_1__1__midout_66_;
assign mux_2level_tapbuf_size16_55_inbus[11] = chany_1__1__midout_67_;
assign mux_2level_tapbuf_size16_55_inbus[12] = chany_1__1__midout_76_;
assign mux_2level_tapbuf_size16_55_inbus[13] = chany_1__1__midout_77_;
assign mux_2level_tapbuf_size16_55_inbus[14] = chany_1__1__midout_90_;
assign mux_2level_tapbuf_size16_55_inbus[15] = chany_1__1__midout_91_;
wire [880:887] mux_2level_tapbuf_size16_55_configbus0;
wire [880:887] mux_2level_tapbuf_size16_55_configbus1;
wire [880:887] mux_2level_tapbuf_size16_55_sram_blwl_out ;
wire [880:887] mux_2level_tapbuf_size16_55_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_55_configbus0[880:887] = sram_blwl_bl[880:887] ;
assign mux_2level_tapbuf_size16_55_configbus1[880:887] = sram_blwl_wl[880:887] ;
wire [880:887] mux_2level_tapbuf_size16_55_configbus0_b;
assign mux_2level_tapbuf_size16_55_configbus0_b[880:887] = sram_blwl_blb[880:887] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_55_ (mux_2level_tapbuf_size16_55_inbus, grid_2__1__pin_0__3__2_, mux_2level_tapbuf_size16_55_sram_blwl_out[880:887] ,
mux_2level_tapbuf_size16_55_sram_blwl_outb[880:887] );
//----- SRAM bits for MUX[55], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_880_ (mux_2level_tapbuf_size16_55_sram_blwl_out[880:880] ,mux_2level_tapbuf_size16_55_sram_blwl_out[880:880] ,mux_2level_tapbuf_size16_55_sram_blwl_outb[880:880] ,mux_2level_tapbuf_size16_55_configbus0[880:880], mux_2level_tapbuf_size16_55_configbus1[880:880] , mux_2level_tapbuf_size16_55_configbus0_b[880:880] );
sram6T_blwl sram_blwl_881_ (mux_2level_tapbuf_size16_55_sram_blwl_out[881:881] ,mux_2level_tapbuf_size16_55_sram_blwl_out[881:881] ,mux_2level_tapbuf_size16_55_sram_blwl_outb[881:881] ,mux_2level_tapbuf_size16_55_configbus0[881:881], mux_2level_tapbuf_size16_55_configbus1[881:881] , mux_2level_tapbuf_size16_55_configbus0_b[881:881] );
sram6T_blwl sram_blwl_882_ (mux_2level_tapbuf_size16_55_sram_blwl_out[882:882] ,mux_2level_tapbuf_size16_55_sram_blwl_out[882:882] ,mux_2level_tapbuf_size16_55_sram_blwl_outb[882:882] ,mux_2level_tapbuf_size16_55_configbus0[882:882], mux_2level_tapbuf_size16_55_configbus1[882:882] , mux_2level_tapbuf_size16_55_configbus0_b[882:882] );
sram6T_blwl sram_blwl_883_ (mux_2level_tapbuf_size16_55_sram_blwl_out[883:883] ,mux_2level_tapbuf_size16_55_sram_blwl_out[883:883] ,mux_2level_tapbuf_size16_55_sram_blwl_outb[883:883] ,mux_2level_tapbuf_size16_55_configbus0[883:883], mux_2level_tapbuf_size16_55_configbus1[883:883] , mux_2level_tapbuf_size16_55_configbus0_b[883:883] );
sram6T_blwl sram_blwl_884_ (mux_2level_tapbuf_size16_55_sram_blwl_out[884:884] ,mux_2level_tapbuf_size16_55_sram_blwl_out[884:884] ,mux_2level_tapbuf_size16_55_sram_blwl_outb[884:884] ,mux_2level_tapbuf_size16_55_configbus0[884:884], mux_2level_tapbuf_size16_55_configbus1[884:884] , mux_2level_tapbuf_size16_55_configbus0_b[884:884] );
sram6T_blwl sram_blwl_885_ (mux_2level_tapbuf_size16_55_sram_blwl_out[885:885] ,mux_2level_tapbuf_size16_55_sram_blwl_out[885:885] ,mux_2level_tapbuf_size16_55_sram_blwl_outb[885:885] ,mux_2level_tapbuf_size16_55_configbus0[885:885], mux_2level_tapbuf_size16_55_configbus1[885:885] , mux_2level_tapbuf_size16_55_configbus0_b[885:885] );
sram6T_blwl sram_blwl_886_ (mux_2level_tapbuf_size16_55_sram_blwl_out[886:886] ,mux_2level_tapbuf_size16_55_sram_blwl_out[886:886] ,mux_2level_tapbuf_size16_55_sram_blwl_outb[886:886] ,mux_2level_tapbuf_size16_55_configbus0[886:886], mux_2level_tapbuf_size16_55_configbus1[886:886] , mux_2level_tapbuf_size16_55_configbus0_b[886:886] );
sram6T_blwl sram_blwl_887_ (mux_2level_tapbuf_size16_55_sram_blwl_out[887:887] ,mux_2level_tapbuf_size16_55_sram_blwl_out[887:887] ,mux_2level_tapbuf_size16_55_sram_blwl_outb[887:887] ,mux_2level_tapbuf_size16_55_configbus0[887:887], mux_2level_tapbuf_size16_55_configbus1[887:887] , mux_2level_tapbuf_size16_55_configbus0_b[887:887] );
wire [0:15] mux_2level_tapbuf_size16_56_inbus;
assign mux_2level_tapbuf_size16_56_inbus[0] = chany_1__1__midout_2_;
assign mux_2level_tapbuf_size16_56_inbus[1] = chany_1__1__midout_3_;
assign mux_2level_tapbuf_size16_56_inbus[2] = chany_1__1__midout_22_;
assign mux_2level_tapbuf_size16_56_inbus[3] = chany_1__1__midout_23_;
assign mux_2level_tapbuf_size16_56_inbus[4] = chany_1__1__midout_26_;
assign mux_2level_tapbuf_size16_56_inbus[5] = chany_1__1__midout_27_;
assign mux_2level_tapbuf_size16_56_inbus[6] = chany_1__1__midout_42_;
assign mux_2level_tapbuf_size16_56_inbus[7] = chany_1__1__midout_43_;
assign mux_2level_tapbuf_size16_56_inbus[8] = chany_1__1__midout_52_;
assign mux_2level_tapbuf_size16_56_inbus[9] = chany_1__1__midout_53_;
assign mux_2level_tapbuf_size16_56_inbus[10] = chany_1__1__midout_64_;
assign mux_2level_tapbuf_size16_56_inbus[11] = chany_1__1__midout_65_;
assign mux_2level_tapbuf_size16_56_inbus[12] = chany_1__1__midout_78_;
assign mux_2level_tapbuf_size16_56_inbus[13] = chany_1__1__midout_79_;
assign mux_2level_tapbuf_size16_56_inbus[14] = chany_1__1__midout_90_;
assign mux_2level_tapbuf_size16_56_inbus[15] = chany_1__1__midout_91_;
wire [888:895] mux_2level_tapbuf_size16_56_configbus0;
wire [888:895] mux_2level_tapbuf_size16_56_configbus1;
wire [888:895] mux_2level_tapbuf_size16_56_sram_blwl_out ;
wire [888:895] mux_2level_tapbuf_size16_56_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_56_configbus0[888:895] = sram_blwl_bl[888:895] ;
assign mux_2level_tapbuf_size16_56_configbus1[888:895] = sram_blwl_wl[888:895] ;
wire [888:895] mux_2level_tapbuf_size16_56_configbus0_b;
assign mux_2level_tapbuf_size16_56_configbus0_b[888:895] = sram_blwl_blb[888:895] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_56_ (mux_2level_tapbuf_size16_56_inbus, grid_2__1__pin_0__3__4_, mux_2level_tapbuf_size16_56_sram_blwl_out[888:895] ,
mux_2level_tapbuf_size16_56_sram_blwl_outb[888:895] );
//----- SRAM bits for MUX[56], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_888_ (mux_2level_tapbuf_size16_56_sram_blwl_out[888:888] ,mux_2level_tapbuf_size16_56_sram_blwl_out[888:888] ,mux_2level_tapbuf_size16_56_sram_blwl_outb[888:888] ,mux_2level_tapbuf_size16_56_configbus0[888:888], mux_2level_tapbuf_size16_56_configbus1[888:888] , mux_2level_tapbuf_size16_56_configbus0_b[888:888] );
sram6T_blwl sram_blwl_889_ (mux_2level_tapbuf_size16_56_sram_blwl_out[889:889] ,mux_2level_tapbuf_size16_56_sram_blwl_out[889:889] ,mux_2level_tapbuf_size16_56_sram_blwl_outb[889:889] ,mux_2level_tapbuf_size16_56_configbus0[889:889], mux_2level_tapbuf_size16_56_configbus1[889:889] , mux_2level_tapbuf_size16_56_configbus0_b[889:889] );
sram6T_blwl sram_blwl_890_ (mux_2level_tapbuf_size16_56_sram_blwl_out[890:890] ,mux_2level_tapbuf_size16_56_sram_blwl_out[890:890] ,mux_2level_tapbuf_size16_56_sram_blwl_outb[890:890] ,mux_2level_tapbuf_size16_56_configbus0[890:890], mux_2level_tapbuf_size16_56_configbus1[890:890] , mux_2level_tapbuf_size16_56_configbus0_b[890:890] );
sram6T_blwl sram_blwl_891_ (mux_2level_tapbuf_size16_56_sram_blwl_out[891:891] ,mux_2level_tapbuf_size16_56_sram_blwl_out[891:891] ,mux_2level_tapbuf_size16_56_sram_blwl_outb[891:891] ,mux_2level_tapbuf_size16_56_configbus0[891:891], mux_2level_tapbuf_size16_56_configbus1[891:891] , mux_2level_tapbuf_size16_56_configbus0_b[891:891] );
sram6T_blwl sram_blwl_892_ (mux_2level_tapbuf_size16_56_sram_blwl_out[892:892] ,mux_2level_tapbuf_size16_56_sram_blwl_out[892:892] ,mux_2level_tapbuf_size16_56_sram_blwl_outb[892:892] ,mux_2level_tapbuf_size16_56_configbus0[892:892], mux_2level_tapbuf_size16_56_configbus1[892:892] , mux_2level_tapbuf_size16_56_configbus0_b[892:892] );
sram6T_blwl sram_blwl_893_ (mux_2level_tapbuf_size16_56_sram_blwl_out[893:893] ,mux_2level_tapbuf_size16_56_sram_blwl_out[893:893] ,mux_2level_tapbuf_size16_56_sram_blwl_outb[893:893] ,mux_2level_tapbuf_size16_56_configbus0[893:893], mux_2level_tapbuf_size16_56_configbus1[893:893] , mux_2level_tapbuf_size16_56_configbus0_b[893:893] );
sram6T_blwl sram_blwl_894_ (mux_2level_tapbuf_size16_56_sram_blwl_out[894:894] ,mux_2level_tapbuf_size16_56_sram_blwl_out[894:894] ,mux_2level_tapbuf_size16_56_sram_blwl_outb[894:894] ,mux_2level_tapbuf_size16_56_configbus0[894:894], mux_2level_tapbuf_size16_56_configbus1[894:894] , mux_2level_tapbuf_size16_56_configbus0_b[894:894] );
sram6T_blwl sram_blwl_895_ (mux_2level_tapbuf_size16_56_sram_blwl_out[895:895] ,mux_2level_tapbuf_size16_56_sram_blwl_out[895:895] ,mux_2level_tapbuf_size16_56_sram_blwl_outb[895:895] ,mux_2level_tapbuf_size16_56_configbus0[895:895], mux_2level_tapbuf_size16_56_configbus1[895:895] , mux_2level_tapbuf_size16_56_configbus0_b[895:895] );
wire [0:15] mux_2level_tapbuf_size16_57_inbus;
assign mux_2level_tapbuf_size16_57_inbus[0] = chany_1__1__midout_2_;
assign mux_2level_tapbuf_size16_57_inbus[1] = chany_1__1__midout_3_;
assign mux_2level_tapbuf_size16_57_inbus[2] = chany_1__1__midout_16_;
assign mux_2level_tapbuf_size16_57_inbus[3] = chany_1__1__midout_17_;
assign mux_2level_tapbuf_size16_57_inbus[4] = chany_1__1__midout_28_;
assign mux_2level_tapbuf_size16_57_inbus[5] = chany_1__1__midout_29_;
assign mux_2level_tapbuf_size16_57_inbus[6] = chany_1__1__midout_40_;
assign mux_2level_tapbuf_size16_57_inbus[7] = chany_1__1__midout_41_;
assign mux_2level_tapbuf_size16_57_inbus[8] = chany_1__1__midout_52_;
assign mux_2level_tapbuf_size16_57_inbus[9] = chany_1__1__midout_53_;
assign mux_2level_tapbuf_size16_57_inbus[10] = chany_1__1__midout_68_;
assign mux_2level_tapbuf_size16_57_inbus[11] = chany_1__1__midout_69_;
assign mux_2level_tapbuf_size16_57_inbus[12] = chany_1__1__midout_80_;
assign mux_2level_tapbuf_size16_57_inbus[13] = chany_1__1__midout_81_;
assign mux_2level_tapbuf_size16_57_inbus[14] = chany_1__1__midout_92_;
assign mux_2level_tapbuf_size16_57_inbus[15] = chany_1__1__midout_93_;
wire [896:903] mux_2level_tapbuf_size16_57_configbus0;
wire [896:903] mux_2level_tapbuf_size16_57_configbus1;
wire [896:903] mux_2level_tapbuf_size16_57_sram_blwl_out ;
wire [896:903] mux_2level_tapbuf_size16_57_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_57_configbus0[896:903] = sram_blwl_bl[896:903] ;
assign mux_2level_tapbuf_size16_57_configbus1[896:903] = sram_blwl_wl[896:903] ;
wire [896:903] mux_2level_tapbuf_size16_57_configbus0_b;
assign mux_2level_tapbuf_size16_57_configbus0_b[896:903] = sram_blwl_blb[896:903] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_57_ (mux_2level_tapbuf_size16_57_inbus, grid_2__1__pin_0__3__6_, mux_2level_tapbuf_size16_57_sram_blwl_out[896:903] ,
mux_2level_tapbuf_size16_57_sram_blwl_outb[896:903] );
//----- SRAM bits for MUX[57], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_896_ (mux_2level_tapbuf_size16_57_sram_blwl_out[896:896] ,mux_2level_tapbuf_size16_57_sram_blwl_out[896:896] ,mux_2level_tapbuf_size16_57_sram_blwl_outb[896:896] ,mux_2level_tapbuf_size16_57_configbus0[896:896], mux_2level_tapbuf_size16_57_configbus1[896:896] , mux_2level_tapbuf_size16_57_configbus0_b[896:896] );
sram6T_blwl sram_blwl_897_ (mux_2level_tapbuf_size16_57_sram_blwl_out[897:897] ,mux_2level_tapbuf_size16_57_sram_blwl_out[897:897] ,mux_2level_tapbuf_size16_57_sram_blwl_outb[897:897] ,mux_2level_tapbuf_size16_57_configbus0[897:897], mux_2level_tapbuf_size16_57_configbus1[897:897] , mux_2level_tapbuf_size16_57_configbus0_b[897:897] );
sram6T_blwl sram_blwl_898_ (mux_2level_tapbuf_size16_57_sram_blwl_out[898:898] ,mux_2level_tapbuf_size16_57_sram_blwl_out[898:898] ,mux_2level_tapbuf_size16_57_sram_blwl_outb[898:898] ,mux_2level_tapbuf_size16_57_configbus0[898:898], mux_2level_tapbuf_size16_57_configbus1[898:898] , mux_2level_tapbuf_size16_57_configbus0_b[898:898] );
sram6T_blwl sram_blwl_899_ (mux_2level_tapbuf_size16_57_sram_blwl_out[899:899] ,mux_2level_tapbuf_size16_57_sram_blwl_out[899:899] ,mux_2level_tapbuf_size16_57_sram_blwl_outb[899:899] ,mux_2level_tapbuf_size16_57_configbus0[899:899], mux_2level_tapbuf_size16_57_configbus1[899:899] , mux_2level_tapbuf_size16_57_configbus0_b[899:899] );
sram6T_blwl sram_blwl_900_ (mux_2level_tapbuf_size16_57_sram_blwl_out[900:900] ,mux_2level_tapbuf_size16_57_sram_blwl_out[900:900] ,mux_2level_tapbuf_size16_57_sram_blwl_outb[900:900] ,mux_2level_tapbuf_size16_57_configbus0[900:900], mux_2level_tapbuf_size16_57_configbus1[900:900] , mux_2level_tapbuf_size16_57_configbus0_b[900:900] );
sram6T_blwl sram_blwl_901_ (mux_2level_tapbuf_size16_57_sram_blwl_out[901:901] ,mux_2level_tapbuf_size16_57_sram_blwl_out[901:901] ,mux_2level_tapbuf_size16_57_sram_blwl_outb[901:901] ,mux_2level_tapbuf_size16_57_configbus0[901:901], mux_2level_tapbuf_size16_57_configbus1[901:901] , mux_2level_tapbuf_size16_57_configbus0_b[901:901] );
sram6T_blwl sram_blwl_902_ (mux_2level_tapbuf_size16_57_sram_blwl_out[902:902] ,mux_2level_tapbuf_size16_57_sram_blwl_out[902:902] ,mux_2level_tapbuf_size16_57_sram_blwl_outb[902:902] ,mux_2level_tapbuf_size16_57_configbus0[902:902], mux_2level_tapbuf_size16_57_configbus1[902:902] , mux_2level_tapbuf_size16_57_configbus0_b[902:902] );
sram6T_blwl sram_blwl_903_ (mux_2level_tapbuf_size16_57_sram_blwl_out[903:903] ,mux_2level_tapbuf_size16_57_sram_blwl_out[903:903] ,mux_2level_tapbuf_size16_57_sram_blwl_outb[903:903] ,mux_2level_tapbuf_size16_57_configbus0[903:903], mux_2level_tapbuf_size16_57_configbus1[903:903] , mux_2level_tapbuf_size16_57_configbus0_b[903:903] );
wire [0:15] mux_2level_tapbuf_size16_58_inbus;
assign mux_2level_tapbuf_size16_58_inbus[0] = chany_1__1__midout_4_;
assign mux_2level_tapbuf_size16_58_inbus[1] = chany_1__1__midout_5_;
assign mux_2level_tapbuf_size16_58_inbus[2] = chany_1__1__midout_16_;
assign mux_2level_tapbuf_size16_58_inbus[3] = chany_1__1__midout_17_;
assign mux_2level_tapbuf_size16_58_inbus[4] = chany_1__1__midout_38_;
assign mux_2level_tapbuf_size16_58_inbus[5] = chany_1__1__midout_39_;
assign mux_2level_tapbuf_size16_58_inbus[6] = chany_1__1__midout_46_;
assign mux_2level_tapbuf_size16_58_inbus[7] = chany_1__1__midout_47_;
assign mux_2level_tapbuf_size16_58_inbus[8] = chany_1__1__midout_58_;
assign mux_2level_tapbuf_size16_58_inbus[9] = chany_1__1__midout_59_;
assign mux_2level_tapbuf_size16_58_inbus[10] = chany_1__1__midout_68_;
assign mux_2level_tapbuf_size16_58_inbus[11] = chany_1__1__midout_69_;
assign mux_2level_tapbuf_size16_58_inbus[12] = chany_1__1__midout_82_;
assign mux_2level_tapbuf_size16_58_inbus[13] = chany_1__1__midout_83_;
assign mux_2level_tapbuf_size16_58_inbus[14] = chany_1__1__midout_94_;
assign mux_2level_tapbuf_size16_58_inbus[15] = chany_1__1__midout_95_;
wire [904:911] mux_2level_tapbuf_size16_58_configbus0;
wire [904:911] mux_2level_tapbuf_size16_58_configbus1;
wire [904:911] mux_2level_tapbuf_size16_58_sram_blwl_out ;
wire [904:911] mux_2level_tapbuf_size16_58_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_58_configbus0[904:911] = sram_blwl_bl[904:911] ;
assign mux_2level_tapbuf_size16_58_configbus1[904:911] = sram_blwl_wl[904:911] ;
wire [904:911] mux_2level_tapbuf_size16_58_configbus0_b;
assign mux_2level_tapbuf_size16_58_configbus0_b[904:911] = sram_blwl_blb[904:911] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_58_ (mux_2level_tapbuf_size16_58_inbus, grid_2__1__pin_0__3__8_, mux_2level_tapbuf_size16_58_sram_blwl_out[904:911] ,
mux_2level_tapbuf_size16_58_sram_blwl_outb[904:911] );
//----- SRAM bits for MUX[58], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_904_ (mux_2level_tapbuf_size16_58_sram_blwl_out[904:904] ,mux_2level_tapbuf_size16_58_sram_blwl_out[904:904] ,mux_2level_tapbuf_size16_58_sram_blwl_outb[904:904] ,mux_2level_tapbuf_size16_58_configbus0[904:904], mux_2level_tapbuf_size16_58_configbus1[904:904] , mux_2level_tapbuf_size16_58_configbus0_b[904:904] );
sram6T_blwl sram_blwl_905_ (mux_2level_tapbuf_size16_58_sram_blwl_out[905:905] ,mux_2level_tapbuf_size16_58_sram_blwl_out[905:905] ,mux_2level_tapbuf_size16_58_sram_blwl_outb[905:905] ,mux_2level_tapbuf_size16_58_configbus0[905:905], mux_2level_tapbuf_size16_58_configbus1[905:905] , mux_2level_tapbuf_size16_58_configbus0_b[905:905] );
sram6T_blwl sram_blwl_906_ (mux_2level_tapbuf_size16_58_sram_blwl_out[906:906] ,mux_2level_tapbuf_size16_58_sram_blwl_out[906:906] ,mux_2level_tapbuf_size16_58_sram_blwl_outb[906:906] ,mux_2level_tapbuf_size16_58_configbus0[906:906], mux_2level_tapbuf_size16_58_configbus1[906:906] , mux_2level_tapbuf_size16_58_configbus0_b[906:906] );
sram6T_blwl sram_blwl_907_ (mux_2level_tapbuf_size16_58_sram_blwl_out[907:907] ,mux_2level_tapbuf_size16_58_sram_blwl_out[907:907] ,mux_2level_tapbuf_size16_58_sram_blwl_outb[907:907] ,mux_2level_tapbuf_size16_58_configbus0[907:907], mux_2level_tapbuf_size16_58_configbus1[907:907] , mux_2level_tapbuf_size16_58_configbus0_b[907:907] );
sram6T_blwl sram_blwl_908_ (mux_2level_tapbuf_size16_58_sram_blwl_out[908:908] ,mux_2level_tapbuf_size16_58_sram_blwl_out[908:908] ,mux_2level_tapbuf_size16_58_sram_blwl_outb[908:908] ,mux_2level_tapbuf_size16_58_configbus0[908:908], mux_2level_tapbuf_size16_58_configbus1[908:908] , mux_2level_tapbuf_size16_58_configbus0_b[908:908] );
sram6T_blwl sram_blwl_909_ (mux_2level_tapbuf_size16_58_sram_blwl_out[909:909] ,mux_2level_tapbuf_size16_58_sram_blwl_out[909:909] ,mux_2level_tapbuf_size16_58_sram_blwl_outb[909:909] ,mux_2level_tapbuf_size16_58_configbus0[909:909], mux_2level_tapbuf_size16_58_configbus1[909:909] , mux_2level_tapbuf_size16_58_configbus0_b[909:909] );
sram6T_blwl sram_blwl_910_ (mux_2level_tapbuf_size16_58_sram_blwl_out[910:910] ,mux_2level_tapbuf_size16_58_sram_blwl_out[910:910] ,mux_2level_tapbuf_size16_58_sram_blwl_outb[910:910] ,mux_2level_tapbuf_size16_58_configbus0[910:910], mux_2level_tapbuf_size16_58_configbus1[910:910] , mux_2level_tapbuf_size16_58_configbus0_b[910:910] );
sram6T_blwl sram_blwl_911_ (mux_2level_tapbuf_size16_58_sram_blwl_out[911:911] ,mux_2level_tapbuf_size16_58_sram_blwl_out[911:911] ,mux_2level_tapbuf_size16_58_sram_blwl_outb[911:911] ,mux_2level_tapbuf_size16_58_configbus0[911:911], mux_2level_tapbuf_size16_58_configbus1[911:911] , mux_2level_tapbuf_size16_58_configbus0_b[911:911] );
wire [0:15] mux_2level_tapbuf_size16_59_inbus;
assign mux_2level_tapbuf_size16_59_inbus[0] = chany_1__1__midout_14_;
assign mux_2level_tapbuf_size16_59_inbus[1] = chany_1__1__midout_15_;
assign mux_2level_tapbuf_size16_59_inbus[2] = chany_1__1__midout_18_;
assign mux_2level_tapbuf_size16_59_inbus[3] = chany_1__1__midout_19_;
assign mux_2level_tapbuf_size16_59_inbus[4] = chany_1__1__midout_38_;
assign mux_2level_tapbuf_size16_59_inbus[5] = chany_1__1__midout_39_;
assign mux_2level_tapbuf_size16_59_inbus[6] = chany_1__1__midout_44_;
assign mux_2level_tapbuf_size16_59_inbus[7] = chany_1__1__midout_45_;
assign mux_2level_tapbuf_size16_59_inbus[8] = chany_1__1__midout_56_;
assign mux_2level_tapbuf_size16_59_inbus[9] = chany_1__1__midout_57_;
assign mux_2level_tapbuf_size16_59_inbus[10] = chany_1__1__midout_70_;
assign mux_2level_tapbuf_size16_59_inbus[11] = chany_1__1__midout_71_;
assign mux_2level_tapbuf_size16_59_inbus[12] = chany_1__1__midout_82_;
assign mux_2level_tapbuf_size16_59_inbus[13] = chany_1__1__midout_83_;
assign mux_2level_tapbuf_size16_59_inbus[14] = chany_1__1__midout_96_;
assign mux_2level_tapbuf_size16_59_inbus[15] = chany_1__1__midout_97_;
wire [912:919] mux_2level_tapbuf_size16_59_configbus0;
wire [912:919] mux_2level_tapbuf_size16_59_configbus1;
wire [912:919] mux_2level_tapbuf_size16_59_sram_blwl_out ;
wire [912:919] mux_2level_tapbuf_size16_59_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_59_configbus0[912:919] = sram_blwl_bl[912:919] ;
assign mux_2level_tapbuf_size16_59_configbus1[912:919] = sram_blwl_wl[912:919] ;
wire [912:919] mux_2level_tapbuf_size16_59_configbus0_b;
assign mux_2level_tapbuf_size16_59_configbus0_b[912:919] = sram_blwl_blb[912:919] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_59_ (mux_2level_tapbuf_size16_59_inbus, grid_2__1__pin_0__3__10_, mux_2level_tapbuf_size16_59_sram_blwl_out[912:919] ,
mux_2level_tapbuf_size16_59_sram_blwl_outb[912:919] );
//----- SRAM bits for MUX[59], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_912_ (mux_2level_tapbuf_size16_59_sram_blwl_out[912:912] ,mux_2level_tapbuf_size16_59_sram_blwl_out[912:912] ,mux_2level_tapbuf_size16_59_sram_blwl_outb[912:912] ,mux_2level_tapbuf_size16_59_configbus0[912:912], mux_2level_tapbuf_size16_59_configbus1[912:912] , mux_2level_tapbuf_size16_59_configbus0_b[912:912] );
sram6T_blwl sram_blwl_913_ (mux_2level_tapbuf_size16_59_sram_blwl_out[913:913] ,mux_2level_tapbuf_size16_59_sram_blwl_out[913:913] ,mux_2level_tapbuf_size16_59_sram_blwl_outb[913:913] ,mux_2level_tapbuf_size16_59_configbus0[913:913], mux_2level_tapbuf_size16_59_configbus1[913:913] , mux_2level_tapbuf_size16_59_configbus0_b[913:913] );
sram6T_blwl sram_blwl_914_ (mux_2level_tapbuf_size16_59_sram_blwl_out[914:914] ,mux_2level_tapbuf_size16_59_sram_blwl_out[914:914] ,mux_2level_tapbuf_size16_59_sram_blwl_outb[914:914] ,mux_2level_tapbuf_size16_59_configbus0[914:914], mux_2level_tapbuf_size16_59_configbus1[914:914] , mux_2level_tapbuf_size16_59_configbus0_b[914:914] );
sram6T_blwl sram_blwl_915_ (mux_2level_tapbuf_size16_59_sram_blwl_out[915:915] ,mux_2level_tapbuf_size16_59_sram_blwl_out[915:915] ,mux_2level_tapbuf_size16_59_sram_blwl_outb[915:915] ,mux_2level_tapbuf_size16_59_configbus0[915:915], mux_2level_tapbuf_size16_59_configbus1[915:915] , mux_2level_tapbuf_size16_59_configbus0_b[915:915] );
sram6T_blwl sram_blwl_916_ (mux_2level_tapbuf_size16_59_sram_blwl_out[916:916] ,mux_2level_tapbuf_size16_59_sram_blwl_out[916:916] ,mux_2level_tapbuf_size16_59_sram_blwl_outb[916:916] ,mux_2level_tapbuf_size16_59_configbus0[916:916], mux_2level_tapbuf_size16_59_configbus1[916:916] , mux_2level_tapbuf_size16_59_configbus0_b[916:916] );
sram6T_blwl sram_blwl_917_ (mux_2level_tapbuf_size16_59_sram_blwl_out[917:917] ,mux_2level_tapbuf_size16_59_sram_blwl_out[917:917] ,mux_2level_tapbuf_size16_59_sram_blwl_outb[917:917] ,mux_2level_tapbuf_size16_59_configbus0[917:917], mux_2level_tapbuf_size16_59_configbus1[917:917] , mux_2level_tapbuf_size16_59_configbus0_b[917:917] );
sram6T_blwl sram_blwl_918_ (mux_2level_tapbuf_size16_59_sram_blwl_out[918:918] ,mux_2level_tapbuf_size16_59_sram_blwl_out[918:918] ,mux_2level_tapbuf_size16_59_sram_blwl_outb[918:918] ,mux_2level_tapbuf_size16_59_configbus0[918:918], mux_2level_tapbuf_size16_59_configbus1[918:918] , mux_2level_tapbuf_size16_59_configbus0_b[918:918] );
sram6T_blwl sram_blwl_919_ (mux_2level_tapbuf_size16_59_sram_blwl_out[919:919] ,mux_2level_tapbuf_size16_59_sram_blwl_out[919:919] ,mux_2level_tapbuf_size16_59_sram_blwl_outb[919:919] ,mux_2level_tapbuf_size16_59_configbus0[919:919], mux_2level_tapbuf_size16_59_configbus1[919:919] , mux_2level_tapbuf_size16_59_configbus0_b[919:919] );
wire [0:15] mux_2level_tapbuf_size16_60_inbus;
assign mux_2level_tapbuf_size16_60_inbus[0] = chany_1__1__midout_8_;
assign mux_2level_tapbuf_size16_60_inbus[1] = chany_1__1__midout_9_;
assign mux_2level_tapbuf_size16_60_inbus[2] = chany_1__1__midout_20_;
assign mux_2level_tapbuf_size16_60_inbus[3] = chany_1__1__midout_21_;
assign mux_2level_tapbuf_size16_60_inbus[4] = chany_1__1__midout_32_;
assign mux_2level_tapbuf_size16_60_inbus[5] = chany_1__1__midout_33_;
assign mux_2level_tapbuf_size16_60_inbus[6] = chany_1__1__midout_50_;
assign mux_2level_tapbuf_size16_60_inbus[7] = chany_1__1__midout_51_;
assign mux_2level_tapbuf_size16_60_inbus[8] = chany_1__1__midout_62_;
assign mux_2level_tapbuf_size16_60_inbus[9] = chany_1__1__midout_63_;
assign mux_2level_tapbuf_size16_60_inbus[10] = chany_1__1__midout_72_;
assign mux_2level_tapbuf_size16_60_inbus[11] = chany_1__1__midout_73_;
assign mux_2level_tapbuf_size16_60_inbus[12] = chany_1__1__midout_84_;
assign mux_2level_tapbuf_size16_60_inbus[13] = chany_1__1__midout_85_;
assign mux_2level_tapbuf_size16_60_inbus[14] = chany_1__1__midout_98_;
assign mux_2level_tapbuf_size16_60_inbus[15] = chany_1__1__midout_99_;
wire [920:927] mux_2level_tapbuf_size16_60_configbus0;
wire [920:927] mux_2level_tapbuf_size16_60_configbus1;
wire [920:927] mux_2level_tapbuf_size16_60_sram_blwl_out ;
wire [920:927] mux_2level_tapbuf_size16_60_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_60_configbus0[920:927] = sram_blwl_bl[920:927] ;
assign mux_2level_tapbuf_size16_60_configbus1[920:927] = sram_blwl_wl[920:927] ;
wire [920:927] mux_2level_tapbuf_size16_60_configbus0_b;
assign mux_2level_tapbuf_size16_60_configbus0_b[920:927] = sram_blwl_blb[920:927] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_60_ (mux_2level_tapbuf_size16_60_inbus, grid_2__1__pin_0__3__12_, mux_2level_tapbuf_size16_60_sram_blwl_out[920:927] ,
mux_2level_tapbuf_size16_60_sram_blwl_outb[920:927] );
//----- SRAM bits for MUX[60], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_920_ (mux_2level_tapbuf_size16_60_sram_blwl_out[920:920] ,mux_2level_tapbuf_size16_60_sram_blwl_out[920:920] ,mux_2level_tapbuf_size16_60_sram_blwl_outb[920:920] ,mux_2level_tapbuf_size16_60_configbus0[920:920], mux_2level_tapbuf_size16_60_configbus1[920:920] , mux_2level_tapbuf_size16_60_configbus0_b[920:920] );
sram6T_blwl sram_blwl_921_ (mux_2level_tapbuf_size16_60_sram_blwl_out[921:921] ,mux_2level_tapbuf_size16_60_sram_blwl_out[921:921] ,mux_2level_tapbuf_size16_60_sram_blwl_outb[921:921] ,mux_2level_tapbuf_size16_60_configbus0[921:921], mux_2level_tapbuf_size16_60_configbus1[921:921] , mux_2level_tapbuf_size16_60_configbus0_b[921:921] );
sram6T_blwl sram_blwl_922_ (mux_2level_tapbuf_size16_60_sram_blwl_out[922:922] ,mux_2level_tapbuf_size16_60_sram_blwl_out[922:922] ,mux_2level_tapbuf_size16_60_sram_blwl_outb[922:922] ,mux_2level_tapbuf_size16_60_configbus0[922:922], mux_2level_tapbuf_size16_60_configbus1[922:922] , mux_2level_tapbuf_size16_60_configbus0_b[922:922] );
sram6T_blwl sram_blwl_923_ (mux_2level_tapbuf_size16_60_sram_blwl_out[923:923] ,mux_2level_tapbuf_size16_60_sram_blwl_out[923:923] ,mux_2level_tapbuf_size16_60_sram_blwl_outb[923:923] ,mux_2level_tapbuf_size16_60_configbus0[923:923], mux_2level_tapbuf_size16_60_configbus1[923:923] , mux_2level_tapbuf_size16_60_configbus0_b[923:923] );
sram6T_blwl sram_blwl_924_ (mux_2level_tapbuf_size16_60_sram_blwl_out[924:924] ,mux_2level_tapbuf_size16_60_sram_blwl_out[924:924] ,mux_2level_tapbuf_size16_60_sram_blwl_outb[924:924] ,mux_2level_tapbuf_size16_60_configbus0[924:924], mux_2level_tapbuf_size16_60_configbus1[924:924] , mux_2level_tapbuf_size16_60_configbus0_b[924:924] );
sram6T_blwl sram_blwl_925_ (mux_2level_tapbuf_size16_60_sram_blwl_out[925:925] ,mux_2level_tapbuf_size16_60_sram_blwl_out[925:925] ,mux_2level_tapbuf_size16_60_sram_blwl_outb[925:925] ,mux_2level_tapbuf_size16_60_configbus0[925:925], mux_2level_tapbuf_size16_60_configbus1[925:925] , mux_2level_tapbuf_size16_60_configbus0_b[925:925] );
sram6T_blwl sram_blwl_926_ (mux_2level_tapbuf_size16_60_sram_blwl_out[926:926] ,mux_2level_tapbuf_size16_60_sram_blwl_out[926:926] ,mux_2level_tapbuf_size16_60_sram_blwl_outb[926:926] ,mux_2level_tapbuf_size16_60_configbus0[926:926], mux_2level_tapbuf_size16_60_configbus1[926:926] , mux_2level_tapbuf_size16_60_configbus0_b[926:926] );
sram6T_blwl sram_blwl_927_ (mux_2level_tapbuf_size16_60_sram_blwl_out[927:927] ,mux_2level_tapbuf_size16_60_sram_blwl_out[927:927] ,mux_2level_tapbuf_size16_60_sram_blwl_outb[927:927] ,mux_2level_tapbuf_size16_60_configbus0[927:927], mux_2level_tapbuf_size16_60_configbus1[927:927] , mux_2level_tapbuf_size16_60_configbus0_b[927:927] );
wire [0:15] mux_2level_tapbuf_size16_61_inbus;
assign mux_2level_tapbuf_size16_61_inbus[0] = chany_1__1__midout_10_;
assign mux_2level_tapbuf_size16_61_inbus[1] = chany_1__1__midout_11_;
assign mux_2level_tapbuf_size16_61_inbus[2] = chany_1__1__midout_30_;
assign mux_2level_tapbuf_size16_61_inbus[3] = chany_1__1__midout_31_;
assign mux_2level_tapbuf_size16_61_inbus[4] = chany_1__1__midout_34_;
assign mux_2level_tapbuf_size16_61_inbus[5] = chany_1__1__midout_35_;
assign mux_2level_tapbuf_size16_61_inbus[6] = chany_1__1__midout_50_;
assign mux_2level_tapbuf_size16_61_inbus[7] = chany_1__1__midout_51_;
assign mux_2level_tapbuf_size16_61_inbus[8] = chany_1__1__midout_60_;
assign mux_2level_tapbuf_size16_61_inbus[9] = chany_1__1__midout_61_;
assign mux_2level_tapbuf_size16_61_inbus[10] = chany_1__1__midout_74_;
assign mux_2level_tapbuf_size16_61_inbus[11] = chany_1__1__midout_75_;
assign mux_2level_tapbuf_size16_61_inbus[12] = chany_1__1__midout_86_;
assign mux_2level_tapbuf_size16_61_inbus[13] = chany_1__1__midout_87_;
assign mux_2level_tapbuf_size16_61_inbus[14] = chany_1__1__midout_98_;
assign mux_2level_tapbuf_size16_61_inbus[15] = chany_1__1__midout_99_;
wire [928:935] mux_2level_tapbuf_size16_61_configbus0;
wire [928:935] mux_2level_tapbuf_size16_61_configbus1;
wire [928:935] mux_2level_tapbuf_size16_61_sram_blwl_out ;
wire [928:935] mux_2level_tapbuf_size16_61_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_61_configbus0[928:935] = sram_blwl_bl[928:935] ;
assign mux_2level_tapbuf_size16_61_configbus1[928:935] = sram_blwl_wl[928:935] ;
wire [928:935] mux_2level_tapbuf_size16_61_configbus0_b;
assign mux_2level_tapbuf_size16_61_configbus0_b[928:935] = sram_blwl_blb[928:935] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_61_ (mux_2level_tapbuf_size16_61_inbus, grid_2__1__pin_0__3__14_, mux_2level_tapbuf_size16_61_sram_blwl_out[928:935] ,
mux_2level_tapbuf_size16_61_sram_blwl_outb[928:935] );
//----- SRAM bits for MUX[61], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_928_ (mux_2level_tapbuf_size16_61_sram_blwl_out[928:928] ,mux_2level_tapbuf_size16_61_sram_blwl_out[928:928] ,mux_2level_tapbuf_size16_61_sram_blwl_outb[928:928] ,mux_2level_tapbuf_size16_61_configbus0[928:928], mux_2level_tapbuf_size16_61_configbus1[928:928] , mux_2level_tapbuf_size16_61_configbus0_b[928:928] );
sram6T_blwl sram_blwl_929_ (mux_2level_tapbuf_size16_61_sram_blwl_out[929:929] ,mux_2level_tapbuf_size16_61_sram_blwl_out[929:929] ,mux_2level_tapbuf_size16_61_sram_blwl_outb[929:929] ,mux_2level_tapbuf_size16_61_configbus0[929:929], mux_2level_tapbuf_size16_61_configbus1[929:929] , mux_2level_tapbuf_size16_61_configbus0_b[929:929] );
sram6T_blwl sram_blwl_930_ (mux_2level_tapbuf_size16_61_sram_blwl_out[930:930] ,mux_2level_tapbuf_size16_61_sram_blwl_out[930:930] ,mux_2level_tapbuf_size16_61_sram_blwl_outb[930:930] ,mux_2level_tapbuf_size16_61_configbus0[930:930], mux_2level_tapbuf_size16_61_configbus1[930:930] , mux_2level_tapbuf_size16_61_configbus0_b[930:930] );
sram6T_blwl sram_blwl_931_ (mux_2level_tapbuf_size16_61_sram_blwl_out[931:931] ,mux_2level_tapbuf_size16_61_sram_blwl_out[931:931] ,mux_2level_tapbuf_size16_61_sram_blwl_outb[931:931] ,mux_2level_tapbuf_size16_61_configbus0[931:931], mux_2level_tapbuf_size16_61_configbus1[931:931] , mux_2level_tapbuf_size16_61_configbus0_b[931:931] );
sram6T_blwl sram_blwl_932_ (mux_2level_tapbuf_size16_61_sram_blwl_out[932:932] ,mux_2level_tapbuf_size16_61_sram_blwl_out[932:932] ,mux_2level_tapbuf_size16_61_sram_blwl_outb[932:932] ,mux_2level_tapbuf_size16_61_configbus0[932:932], mux_2level_tapbuf_size16_61_configbus1[932:932] , mux_2level_tapbuf_size16_61_configbus0_b[932:932] );
sram6T_blwl sram_blwl_933_ (mux_2level_tapbuf_size16_61_sram_blwl_out[933:933] ,mux_2level_tapbuf_size16_61_sram_blwl_out[933:933] ,mux_2level_tapbuf_size16_61_sram_blwl_outb[933:933] ,mux_2level_tapbuf_size16_61_configbus0[933:933], mux_2level_tapbuf_size16_61_configbus1[933:933] , mux_2level_tapbuf_size16_61_configbus0_b[933:933] );
sram6T_blwl sram_blwl_934_ (mux_2level_tapbuf_size16_61_sram_blwl_out[934:934] ,mux_2level_tapbuf_size16_61_sram_blwl_out[934:934] ,mux_2level_tapbuf_size16_61_sram_blwl_outb[934:934] ,mux_2level_tapbuf_size16_61_configbus0[934:934], mux_2level_tapbuf_size16_61_configbus1[934:934] , mux_2level_tapbuf_size16_61_configbus0_b[934:934] );
sram6T_blwl sram_blwl_935_ (mux_2level_tapbuf_size16_61_sram_blwl_out[935:935] ,mux_2level_tapbuf_size16_61_sram_blwl_out[935:935] ,mux_2level_tapbuf_size16_61_sram_blwl_outb[935:935] ,mux_2level_tapbuf_size16_61_configbus0[935:935], mux_2level_tapbuf_size16_61_configbus1[935:935] , mux_2level_tapbuf_size16_61_configbus0_b[935:935] );
wire [0:15] mux_2level_tapbuf_size16_62_inbus;
assign mux_2level_tapbuf_size16_62_inbus[0] = chany_1__1__midout_6_;
assign mux_2level_tapbuf_size16_62_inbus[1] = chany_1__1__midout_7_;
assign mux_2level_tapbuf_size16_62_inbus[2] = chany_1__1__midout_10_;
assign mux_2level_tapbuf_size16_62_inbus[3] = chany_1__1__midout_11_;
assign mux_2level_tapbuf_size16_62_inbus[4] = chany_1__1__midout_30_;
assign mux_2level_tapbuf_size16_62_inbus[5] = chany_1__1__midout_31_;
assign mux_2level_tapbuf_size16_62_inbus[6] = chany_1__1__midout_34_;
assign mux_2level_tapbuf_size16_62_inbus[7] = chany_1__1__midout_35_;
assign mux_2level_tapbuf_size16_62_inbus[8] = chany_1__1__midout_48_;
assign mux_2level_tapbuf_size16_62_inbus[9] = chany_1__1__midout_49_;
assign mux_2level_tapbuf_size16_62_inbus[10] = chany_1__1__midout_60_;
assign mux_2level_tapbuf_size16_62_inbus[11] = chany_1__1__midout_61_;
assign mux_2level_tapbuf_size16_62_inbus[12] = chany_1__1__midout_74_;
assign mux_2level_tapbuf_size16_62_inbus[13] = chany_1__1__midout_75_;
assign mux_2level_tapbuf_size16_62_inbus[14] = chany_1__1__midout_86_;
assign mux_2level_tapbuf_size16_62_inbus[15] = chany_1__1__midout_87_;
wire [936:943] mux_2level_tapbuf_size16_62_configbus0;
wire [936:943] mux_2level_tapbuf_size16_62_configbus1;
wire [936:943] mux_2level_tapbuf_size16_62_sram_blwl_out ;
wire [936:943] mux_2level_tapbuf_size16_62_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_62_configbus0[936:943] = sram_blwl_bl[936:943] ;
assign mux_2level_tapbuf_size16_62_configbus1[936:943] = sram_blwl_wl[936:943] ;
wire [936:943] mux_2level_tapbuf_size16_62_configbus0_b;
assign mux_2level_tapbuf_size16_62_configbus0_b[936:943] = sram_blwl_blb[936:943] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_62_ (mux_2level_tapbuf_size16_62_inbus, grid_1__1__pin_0__1__1_, mux_2level_tapbuf_size16_62_sram_blwl_out[936:943] ,
mux_2level_tapbuf_size16_62_sram_blwl_outb[936:943] );
//----- SRAM bits for MUX[62], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_936_ (mux_2level_tapbuf_size16_62_sram_blwl_out[936:936] ,mux_2level_tapbuf_size16_62_sram_blwl_out[936:936] ,mux_2level_tapbuf_size16_62_sram_blwl_outb[936:936] ,mux_2level_tapbuf_size16_62_configbus0[936:936], mux_2level_tapbuf_size16_62_configbus1[936:936] , mux_2level_tapbuf_size16_62_configbus0_b[936:936] );
sram6T_blwl sram_blwl_937_ (mux_2level_tapbuf_size16_62_sram_blwl_out[937:937] ,mux_2level_tapbuf_size16_62_sram_blwl_out[937:937] ,mux_2level_tapbuf_size16_62_sram_blwl_outb[937:937] ,mux_2level_tapbuf_size16_62_configbus0[937:937], mux_2level_tapbuf_size16_62_configbus1[937:937] , mux_2level_tapbuf_size16_62_configbus0_b[937:937] );
sram6T_blwl sram_blwl_938_ (mux_2level_tapbuf_size16_62_sram_blwl_out[938:938] ,mux_2level_tapbuf_size16_62_sram_blwl_out[938:938] ,mux_2level_tapbuf_size16_62_sram_blwl_outb[938:938] ,mux_2level_tapbuf_size16_62_configbus0[938:938], mux_2level_tapbuf_size16_62_configbus1[938:938] , mux_2level_tapbuf_size16_62_configbus0_b[938:938] );
sram6T_blwl sram_blwl_939_ (mux_2level_tapbuf_size16_62_sram_blwl_out[939:939] ,mux_2level_tapbuf_size16_62_sram_blwl_out[939:939] ,mux_2level_tapbuf_size16_62_sram_blwl_outb[939:939] ,mux_2level_tapbuf_size16_62_configbus0[939:939], mux_2level_tapbuf_size16_62_configbus1[939:939] , mux_2level_tapbuf_size16_62_configbus0_b[939:939] );
sram6T_blwl sram_blwl_940_ (mux_2level_tapbuf_size16_62_sram_blwl_out[940:940] ,mux_2level_tapbuf_size16_62_sram_blwl_out[940:940] ,mux_2level_tapbuf_size16_62_sram_blwl_outb[940:940] ,mux_2level_tapbuf_size16_62_configbus0[940:940], mux_2level_tapbuf_size16_62_configbus1[940:940] , mux_2level_tapbuf_size16_62_configbus0_b[940:940] );
sram6T_blwl sram_blwl_941_ (mux_2level_tapbuf_size16_62_sram_blwl_out[941:941] ,mux_2level_tapbuf_size16_62_sram_blwl_out[941:941] ,mux_2level_tapbuf_size16_62_sram_blwl_outb[941:941] ,mux_2level_tapbuf_size16_62_configbus0[941:941], mux_2level_tapbuf_size16_62_configbus1[941:941] , mux_2level_tapbuf_size16_62_configbus0_b[941:941] );
sram6T_blwl sram_blwl_942_ (mux_2level_tapbuf_size16_62_sram_blwl_out[942:942] ,mux_2level_tapbuf_size16_62_sram_blwl_out[942:942] ,mux_2level_tapbuf_size16_62_sram_blwl_outb[942:942] ,mux_2level_tapbuf_size16_62_configbus0[942:942], mux_2level_tapbuf_size16_62_configbus1[942:942] , mux_2level_tapbuf_size16_62_configbus0_b[942:942] );
sram6T_blwl sram_blwl_943_ (mux_2level_tapbuf_size16_62_sram_blwl_out[943:943] ,mux_2level_tapbuf_size16_62_sram_blwl_out[943:943] ,mux_2level_tapbuf_size16_62_sram_blwl_outb[943:943] ,mux_2level_tapbuf_size16_62_configbus0[943:943], mux_2level_tapbuf_size16_62_configbus1[943:943] , mux_2level_tapbuf_size16_62_configbus0_b[943:943] );
wire [0:15] mux_2level_tapbuf_size16_63_inbus;
assign mux_2level_tapbuf_size16_63_inbus[0] = chany_1__1__midout_6_;
assign mux_2level_tapbuf_size16_63_inbus[1] = chany_1__1__midout_7_;
assign mux_2level_tapbuf_size16_63_inbus[2] = chany_1__1__midout_12_;
assign mux_2level_tapbuf_size16_63_inbus[3] = chany_1__1__midout_13_;
assign mux_2level_tapbuf_size16_63_inbus[4] = chany_1__1__midout_24_;
assign mux_2level_tapbuf_size16_63_inbus[5] = chany_1__1__midout_25_;
assign mux_2level_tapbuf_size16_63_inbus[6] = chany_1__1__midout_36_;
assign mux_2level_tapbuf_size16_63_inbus[7] = chany_1__1__midout_37_;
assign mux_2level_tapbuf_size16_63_inbus[8] = chany_1__1__midout_48_;
assign mux_2level_tapbuf_size16_63_inbus[9] = chany_1__1__midout_49_;
assign mux_2level_tapbuf_size16_63_inbus[10] = chany_1__1__midout_66_;
assign mux_2level_tapbuf_size16_63_inbus[11] = chany_1__1__midout_67_;
assign mux_2level_tapbuf_size16_63_inbus[12] = chany_1__1__midout_76_;
assign mux_2level_tapbuf_size16_63_inbus[13] = chany_1__1__midout_77_;
assign mux_2level_tapbuf_size16_63_inbus[14] = chany_1__1__midout_88_;
assign mux_2level_tapbuf_size16_63_inbus[15] = chany_1__1__midout_89_;
wire [944:951] mux_2level_tapbuf_size16_63_configbus0;
wire [944:951] mux_2level_tapbuf_size16_63_configbus1;
wire [944:951] mux_2level_tapbuf_size16_63_sram_blwl_out ;
wire [944:951] mux_2level_tapbuf_size16_63_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_63_configbus0[944:951] = sram_blwl_bl[944:951] ;
assign mux_2level_tapbuf_size16_63_configbus1[944:951] = sram_blwl_wl[944:951] ;
wire [944:951] mux_2level_tapbuf_size16_63_configbus0_b;
assign mux_2level_tapbuf_size16_63_configbus0_b[944:951] = sram_blwl_blb[944:951] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_63_ (mux_2level_tapbuf_size16_63_inbus, grid_1__1__pin_0__1__5_, mux_2level_tapbuf_size16_63_sram_blwl_out[944:951] ,
mux_2level_tapbuf_size16_63_sram_blwl_outb[944:951] );
//----- SRAM bits for MUX[63], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_944_ (mux_2level_tapbuf_size16_63_sram_blwl_out[944:944] ,mux_2level_tapbuf_size16_63_sram_blwl_out[944:944] ,mux_2level_tapbuf_size16_63_sram_blwl_outb[944:944] ,mux_2level_tapbuf_size16_63_configbus0[944:944], mux_2level_tapbuf_size16_63_configbus1[944:944] , mux_2level_tapbuf_size16_63_configbus0_b[944:944] );
sram6T_blwl sram_blwl_945_ (mux_2level_tapbuf_size16_63_sram_blwl_out[945:945] ,mux_2level_tapbuf_size16_63_sram_blwl_out[945:945] ,mux_2level_tapbuf_size16_63_sram_blwl_outb[945:945] ,mux_2level_tapbuf_size16_63_configbus0[945:945], mux_2level_tapbuf_size16_63_configbus1[945:945] , mux_2level_tapbuf_size16_63_configbus0_b[945:945] );
sram6T_blwl sram_blwl_946_ (mux_2level_tapbuf_size16_63_sram_blwl_out[946:946] ,mux_2level_tapbuf_size16_63_sram_blwl_out[946:946] ,mux_2level_tapbuf_size16_63_sram_blwl_outb[946:946] ,mux_2level_tapbuf_size16_63_configbus0[946:946], mux_2level_tapbuf_size16_63_configbus1[946:946] , mux_2level_tapbuf_size16_63_configbus0_b[946:946] );
sram6T_blwl sram_blwl_947_ (mux_2level_tapbuf_size16_63_sram_blwl_out[947:947] ,mux_2level_tapbuf_size16_63_sram_blwl_out[947:947] ,mux_2level_tapbuf_size16_63_sram_blwl_outb[947:947] ,mux_2level_tapbuf_size16_63_configbus0[947:947], mux_2level_tapbuf_size16_63_configbus1[947:947] , mux_2level_tapbuf_size16_63_configbus0_b[947:947] );
sram6T_blwl sram_blwl_948_ (mux_2level_tapbuf_size16_63_sram_blwl_out[948:948] ,mux_2level_tapbuf_size16_63_sram_blwl_out[948:948] ,mux_2level_tapbuf_size16_63_sram_blwl_outb[948:948] ,mux_2level_tapbuf_size16_63_configbus0[948:948], mux_2level_tapbuf_size16_63_configbus1[948:948] , mux_2level_tapbuf_size16_63_configbus0_b[948:948] );
sram6T_blwl sram_blwl_949_ (mux_2level_tapbuf_size16_63_sram_blwl_out[949:949] ,mux_2level_tapbuf_size16_63_sram_blwl_out[949:949] ,mux_2level_tapbuf_size16_63_sram_blwl_outb[949:949] ,mux_2level_tapbuf_size16_63_configbus0[949:949], mux_2level_tapbuf_size16_63_configbus1[949:949] , mux_2level_tapbuf_size16_63_configbus0_b[949:949] );
sram6T_blwl sram_blwl_950_ (mux_2level_tapbuf_size16_63_sram_blwl_out[950:950] ,mux_2level_tapbuf_size16_63_sram_blwl_out[950:950] ,mux_2level_tapbuf_size16_63_sram_blwl_outb[950:950] ,mux_2level_tapbuf_size16_63_configbus0[950:950], mux_2level_tapbuf_size16_63_configbus1[950:950] , mux_2level_tapbuf_size16_63_configbus0_b[950:950] );
sram6T_blwl sram_blwl_951_ (mux_2level_tapbuf_size16_63_sram_blwl_out[951:951] ,mux_2level_tapbuf_size16_63_sram_blwl_out[951:951] ,mux_2level_tapbuf_size16_63_sram_blwl_outb[951:951] ,mux_2level_tapbuf_size16_63_configbus0[951:951], mux_2level_tapbuf_size16_63_configbus1[951:951] , mux_2level_tapbuf_size16_63_configbus0_b[951:951] );
wire [0:15] mux_2level_tapbuf_size16_64_inbus;
assign mux_2level_tapbuf_size16_64_inbus[0] = chany_1__1__midout_0_;
assign mux_2level_tapbuf_size16_64_inbus[1] = chany_1__1__midout_1_;
assign mux_2level_tapbuf_size16_64_inbus[2] = chany_1__1__midout_12_;
assign mux_2level_tapbuf_size16_64_inbus[3] = chany_1__1__midout_13_;
assign mux_2level_tapbuf_size16_64_inbus[4] = chany_1__1__midout_24_;
assign mux_2level_tapbuf_size16_64_inbus[5] = chany_1__1__midout_25_;
assign mux_2level_tapbuf_size16_64_inbus[6] = chany_1__1__midout_42_;
assign mux_2level_tapbuf_size16_64_inbus[7] = chany_1__1__midout_43_;
assign mux_2level_tapbuf_size16_64_inbus[8] = chany_1__1__midout_54_;
assign mux_2level_tapbuf_size16_64_inbus[9] = chany_1__1__midout_55_;
assign mux_2level_tapbuf_size16_64_inbus[10] = chany_1__1__midout_66_;
assign mux_2level_tapbuf_size16_64_inbus[11] = chany_1__1__midout_67_;
assign mux_2level_tapbuf_size16_64_inbus[12] = chany_1__1__midout_76_;
assign mux_2level_tapbuf_size16_64_inbus[13] = chany_1__1__midout_77_;
assign mux_2level_tapbuf_size16_64_inbus[14] = chany_1__1__midout_90_;
assign mux_2level_tapbuf_size16_64_inbus[15] = chany_1__1__midout_91_;
wire [952:959] mux_2level_tapbuf_size16_64_configbus0;
wire [952:959] mux_2level_tapbuf_size16_64_configbus1;
wire [952:959] mux_2level_tapbuf_size16_64_sram_blwl_out ;
wire [952:959] mux_2level_tapbuf_size16_64_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_64_configbus0[952:959] = sram_blwl_bl[952:959] ;
assign mux_2level_tapbuf_size16_64_configbus1[952:959] = sram_blwl_wl[952:959] ;
wire [952:959] mux_2level_tapbuf_size16_64_configbus0_b;
assign mux_2level_tapbuf_size16_64_configbus0_b[952:959] = sram_blwl_blb[952:959] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_64_ (mux_2level_tapbuf_size16_64_inbus, grid_1__1__pin_0__1__9_, mux_2level_tapbuf_size16_64_sram_blwl_out[952:959] ,
mux_2level_tapbuf_size16_64_sram_blwl_outb[952:959] );
//----- SRAM bits for MUX[64], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_952_ (mux_2level_tapbuf_size16_64_sram_blwl_out[952:952] ,mux_2level_tapbuf_size16_64_sram_blwl_out[952:952] ,mux_2level_tapbuf_size16_64_sram_blwl_outb[952:952] ,mux_2level_tapbuf_size16_64_configbus0[952:952], mux_2level_tapbuf_size16_64_configbus1[952:952] , mux_2level_tapbuf_size16_64_configbus0_b[952:952] );
sram6T_blwl sram_blwl_953_ (mux_2level_tapbuf_size16_64_sram_blwl_out[953:953] ,mux_2level_tapbuf_size16_64_sram_blwl_out[953:953] ,mux_2level_tapbuf_size16_64_sram_blwl_outb[953:953] ,mux_2level_tapbuf_size16_64_configbus0[953:953], mux_2level_tapbuf_size16_64_configbus1[953:953] , mux_2level_tapbuf_size16_64_configbus0_b[953:953] );
sram6T_blwl sram_blwl_954_ (mux_2level_tapbuf_size16_64_sram_blwl_out[954:954] ,mux_2level_tapbuf_size16_64_sram_blwl_out[954:954] ,mux_2level_tapbuf_size16_64_sram_blwl_outb[954:954] ,mux_2level_tapbuf_size16_64_configbus0[954:954], mux_2level_tapbuf_size16_64_configbus1[954:954] , mux_2level_tapbuf_size16_64_configbus0_b[954:954] );
sram6T_blwl sram_blwl_955_ (mux_2level_tapbuf_size16_64_sram_blwl_out[955:955] ,mux_2level_tapbuf_size16_64_sram_blwl_out[955:955] ,mux_2level_tapbuf_size16_64_sram_blwl_outb[955:955] ,mux_2level_tapbuf_size16_64_configbus0[955:955], mux_2level_tapbuf_size16_64_configbus1[955:955] , mux_2level_tapbuf_size16_64_configbus0_b[955:955] );
sram6T_blwl sram_blwl_956_ (mux_2level_tapbuf_size16_64_sram_blwl_out[956:956] ,mux_2level_tapbuf_size16_64_sram_blwl_out[956:956] ,mux_2level_tapbuf_size16_64_sram_blwl_outb[956:956] ,mux_2level_tapbuf_size16_64_configbus0[956:956], mux_2level_tapbuf_size16_64_configbus1[956:956] , mux_2level_tapbuf_size16_64_configbus0_b[956:956] );
sram6T_blwl sram_blwl_957_ (mux_2level_tapbuf_size16_64_sram_blwl_out[957:957] ,mux_2level_tapbuf_size16_64_sram_blwl_out[957:957] ,mux_2level_tapbuf_size16_64_sram_blwl_outb[957:957] ,mux_2level_tapbuf_size16_64_configbus0[957:957], mux_2level_tapbuf_size16_64_configbus1[957:957] , mux_2level_tapbuf_size16_64_configbus0_b[957:957] );
sram6T_blwl sram_blwl_958_ (mux_2level_tapbuf_size16_64_sram_blwl_out[958:958] ,mux_2level_tapbuf_size16_64_sram_blwl_out[958:958] ,mux_2level_tapbuf_size16_64_sram_blwl_outb[958:958] ,mux_2level_tapbuf_size16_64_configbus0[958:958], mux_2level_tapbuf_size16_64_configbus1[958:958] , mux_2level_tapbuf_size16_64_configbus0_b[958:958] );
sram6T_blwl sram_blwl_959_ (mux_2level_tapbuf_size16_64_sram_blwl_out[959:959] ,mux_2level_tapbuf_size16_64_sram_blwl_out[959:959] ,mux_2level_tapbuf_size16_64_sram_blwl_outb[959:959] ,mux_2level_tapbuf_size16_64_configbus0[959:959], mux_2level_tapbuf_size16_64_configbus1[959:959] , mux_2level_tapbuf_size16_64_configbus0_b[959:959] );
wire [0:15] mux_2level_tapbuf_size16_65_inbus;
assign mux_2level_tapbuf_size16_65_inbus[0] = chany_1__1__midout_2_;
assign mux_2level_tapbuf_size16_65_inbus[1] = chany_1__1__midout_3_;
assign mux_2level_tapbuf_size16_65_inbus[2] = chany_1__1__midout_22_;
assign mux_2level_tapbuf_size16_65_inbus[3] = chany_1__1__midout_23_;
assign mux_2level_tapbuf_size16_65_inbus[4] = chany_1__1__midout_26_;
assign mux_2level_tapbuf_size16_65_inbus[5] = chany_1__1__midout_27_;
assign mux_2level_tapbuf_size16_65_inbus[6] = chany_1__1__midout_42_;
assign mux_2level_tapbuf_size16_65_inbus[7] = chany_1__1__midout_43_;
assign mux_2level_tapbuf_size16_65_inbus[8] = chany_1__1__midout_52_;
assign mux_2level_tapbuf_size16_65_inbus[9] = chany_1__1__midout_53_;
assign mux_2level_tapbuf_size16_65_inbus[10] = chany_1__1__midout_64_;
assign mux_2level_tapbuf_size16_65_inbus[11] = chany_1__1__midout_65_;
assign mux_2level_tapbuf_size16_65_inbus[12] = chany_1__1__midout_78_;
assign mux_2level_tapbuf_size16_65_inbus[13] = chany_1__1__midout_79_;
assign mux_2level_tapbuf_size16_65_inbus[14] = chany_1__1__midout_90_;
assign mux_2level_tapbuf_size16_65_inbus[15] = chany_1__1__midout_91_;
wire [960:967] mux_2level_tapbuf_size16_65_configbus0;
wire [960:967] mux_2level_tapbuf_size16_65_configbus1;
wire [960:967] mux_2level_tapbuf_size16_65_sram_blwl_out ;
wire [960:967] mux_2level_tapbuf_size16_65_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_65_configbus0[960:967] = sram_blwl_bl[960:967] ;
assign mux_2level_tapbuf_size16_65_configbus1[960:967] = sram_blwl_wl[960:967] ;
wire [960:967] mux_2level_tapbuf_size16_65_configbus0_b;
assign mux_2level_tapbuf_size16_65_configbus0_b[960:967] = sram_blwl_blb[960:967] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_65_ (mux_2level_tapbuf_size16_65_inbus, grid_1__1__pin_0__1__13_, mux_2level_tapbuf_size16_65_sram_blwl_out[960:967] ,
mux_2level_tapbuf_size16_65_sram_blwl_outb[960:967] );
//----- SRAM bits for MUX[65], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_960_ (mux_2level_tapbuf_size16_65_sram_blwl_out[960:960] ,mux_2level_tapbuf_size16_65_sram_blwl_out[960:960] ,mux_2level_tapbuf_size16_65_sram_blwl_outb[960:960] ,mux_2level_tapbuf_size16_65_configbus0[960:960], mux_2level_tapbuf_size16_65_configbus1[960:960] , mux_2level_tapbuf_size16_65_configbus0_b[960:960] );
sram6T_blwl sram_blwl_961_ (mux_2level_tapbuf_size16_65_sram_blwl_out[961:961] ,mux_2level_tapbuf_size16_65_sram_blwl_out[961:961] ,mux_2level_tapbuf_size16_65_sram_blwl_outb[961:961] ,mux_2level_tapbuf_size16_65_configbus0[961:961], mux_2level_tapbuf_size16_65_configbus1[961:961] , mux_2level_tapbuf_size16_65_configbus0_b[961:961] );
sram6T_blwl sram_blwl_962_ (mux_2level_tapbuf_size16_65_sram_blwl_out[962:962] ,mux_2level_tapbuf_size16_65_sram_blwl_out[962:962] ,mux_2level_tapbuf_size16_65_sram_blwl_outb[962:962] ,mux_2level_tapbuf_size16_65_configbus0[962:962], mux_2level_tapbuf_size16_65_configbus1[962:962] , mux_2level_tapbuf_size16_65_configbus0_b[962:962] );
sram6T_blwl sram_blwl_963_ (mux_2level_tapbuf_size16_65_sram_blwl_out[963:963] ,mux_2level_tapbuf_size16_65_sram_blwl_out[963:963] ,mux_2level_tapbuf_size16_65_sram_blwl_outb[963:963] ,mux_2level_tapbuf_size16_65_configbus0[963:963], mux_2level_tapbuf_size16_65_configbus1[963:963] , mux_2level_tapbuf_size16_65_configbus0_b[963:963] );
sram6T_blwl sram_blwl_964_ (mux_2level_tapbuf_size16_65_sram_blwl_out[964:964] ,mux_2level_tapbuf_size16_65_sram_blwl_out[964:964] ,mux_2level_tapbuf_size16_65_sram_blwl_outb[964:964] ,mux_2level_tapbuf_size16_65_configbus0[964:964], mux_2level_tapbuf_size16_65_configbus1[964:964] , mux_2level_tapbuf_size16_65_configbus0_b[964:964] );
sram6T_blwl sram_blwl_965_ (mux_2level_tapbuf_size16_65_sram_blwl_out[965:965] ,mux_2level_tapbuf_size16_65_sram_blwl_out[965:965] ,mux_2level_tapbuf_size16_65_sram_blwl_outb[965:965] ,mux_2level_tapbuf_size16_65_configbus0[965:965], mux_2level_tapbuf_size16_65_configbus1[965:965] , mux_2level_tapbuf_size16_65_configbus0_b[965:965] );
sram6T_blwl sram_blwl_966_ (mux_2level_tapbuf_size16_65_sram_blwl_out[966:966] ,mux_2level_tapbuf_size16_65_sram_blwl_out[966:966] ,mux_2level_tapbuf_size16_65_sram_blwl_outb[966:966] ,mux_2level_tapbuf_size16_65_configbus0[966:966], mux_2level_tapbuf_size16_65_configbus1[966:966] , mux_2level_tapbuf_size16_65_configbus0_b[966:966] );
sram6T_blwl sram_blwl_967_ (mux_2level_tapbuf_size16_65_sram_blwl_out[967:967] ,mux_2level_tapbuf_size16_65_sram_blwl_out[967:967] ,mux_2level_tapbuf_size16_65_sram_blwl_outb[967:967] ,mux_2level_tapbuf_size16_65_configbus0[967:967], mux_2level_tapbuf_size16_65_configbus1[967:967] , mux_2level_tapbuf_size16_65_configbus0_b[967:967] );
wire [0:15] mux_2level_tapbuf_size16_66_inbus;
assign mux_2level_tapbuf_size16_66_inbus[0] = chany_1__1__midout_2_;
assign mux_2level_tapbuf_size16_66_inbus[1] = chany_1__1__midout_3_;
assign mux_2level_tapbuf_size16_66_inbus[2] = chany_1__1__midout_22_;
assign mux_2level_tapbuf_size16_66_inbus[3] = chany_1__1__midout_23_;
assign mux_2level_tapbuf_size16_66_inbus[4] = chany_1__1__midout_28_;
assign mux_2level_tapbuf_size16_66_inbus[5] = chany_1__1__midout_29_;
assign mux_2level_tapbuf_size16_66_inbus[6] = chany_1__1__midout_40_;
assign mux_2level_tapbuf_size16_66_inbus[7] = chany_1__1__midout_41_;
assign mux_2level_tapbuf_size16_66_inbus[8] = chany_1__1__midout_52_;
assign mux_2level_tapbuf_size16_66_inbus[9] = chany_1__1__midout_53_;
assign mux_2level_tapbuf_size16_66_inbus[10] = chany_1__1__midout_64_;
assign mux_2level_tapbuf_size16_66_inbus[11] = chany_1__1__midout_65_;
assign mux_2level_tapbuf_size16_66_inbus[12] = chany_1__1__midout_80_;
assign mux_2level_tapbuf_size16_66_inbus[13] = chany_1__1__midout_81_;
assign mux_2level_tapbuf_size16_66_inbus[14] = chany_1__1__midout_92_;
assign mux_2level_tapbuf_size16_66_inbus[15] = chany_1__1__midout_93_;
wire [968:975] mux_2level_tapbuf_size16_66_configbus0;
wire [968:975] mux_2level_tapbuf_size16_66_configbus1;
wire [968:975] mux_2level_tapbuf_size16_66_sram_blwl_out ;
wire [968:975] mux_2level_tapbuf_size16_66_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_66_configbus0[968:975] = sram_blwl_bl[968:975] ;
assign mux_2level_tapbuf_size16_66_configbus1[968:975] = sram_blwl_wl[968:975] ;
wire [968:975] mux_2level_tapbuf_size16_66_configbus0_b;
assign mux_2level_tapbuf_size16_66_configbus0_b[968:975] = sram_blwl_blb[968:975] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_66_ (mux_2level_tapbuf_size16_66_inbus, grid_1__1__pin_0__1__17_, mux_2level_tapbuf_size16_66_sram_blwl_out[968:975] ,
mux_2level_tapbuf_size16_66_sram_blwl_outb[968:975] );
//----- SRAM bits for MUX[66], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_968_ (mux_2level_tapbuf_size16_66_sram_blwl_out[968:968] ,mux_2level_tapbuf_size16_66_sram_blwl_out[968:968] ,mux_2level_tapbuf_size16_66_sram_blwl_outb[968:968] ,mux_2level_tapbuf_size16_66_configbus0[968:968], mux_2level_tapbuf_size16_66_configbus1[968:968] , mux_2level_tapbuf_size16_66_configbus0_b[968:968] );
sram6T_blwl sram_blwl_969_ (mux_2level_tapbuf_size16_66_sram_blwl_out[969:969] ,mux_2level_tapbuf_size16_66_sram_blwl_out[969:969] ,mux_2level_tapbuf_size16_66_sram_blwl_outb[969:969] ,mux_2level_tapbuf_size16_66_configbus0[969:969], mux_2level_tapbuf_size16_66_configbus1[969:969] , mux_2level_tapbuf_size16_66_configbus0_b[969:969] );
sram6T_blwl sram_blwl_970_ (mux_2level_tapbuf_size16_66_sram_blwl_out[970:970] ,mux_2level_tapbuf_size16_66_sram_blwl_out[970:970] ,mux_2level_tapbuf_size16_66_sram_blwl_outb[970:970] ,mux_2level_tapbuf_size16_66_configbus0[970:970], mux_2level_tapbuf_size16_66_configbus1[970:970] , mux_2level_tapbuf_size16_66_configbus0_b[970:970] );
sram6T_blwl sram_blwl_971_ (mux_2level_tapbuf_size16_66_sram_blwl_out[971:971] ,mux_2level_tapbuf_size16_66_sram_blwl_out[971:971] ,mux_2level_tapbuf_size16_66_sram_blwl_outb[971:971] ,mux_2level_tapbuf_size16_66_configbus0[971:971], mux_2level_tapbuf_size16_66_configbus1[971:971] , mux_2level_tapbuf_size16_66_configbus0_b[971:971] );
sram6T_blwl sram_blwl_972_ (mux_2level_tapbuf_size16_66_sram_blwl_out[972:972] ,mux_2level_tapbuf_size16_66_sram_blwl_out[972:972] ,mux_2level_tapbuf_size16_66_sram_blwl_outb[972:972] ,mux_2level_tapbuf_size16_66_configbus0[972:972], mux_2level_tapbuf_size16_66_configbus1[972:972] , mux_2level_tapbuf_size16_66_configbus0_b[972:972] );
sram6T_blwl sram_blwl_973_ (mux_2level_tapbuf_size16_66_sram_blwl_out[973:973] ,mux_2level_tapbuf_size16_66_sram_blwl_out[973:973] ,mux_2level_tapbuf_size16_66_sram_blwl_outb[973:973] ,mux_2level_tapbuf_size16_66_configbus0[973:973], mux_2level_tapbuf_size16_66_configbus1[973:973] , mux_2level_tapbuf_size16_66_configbus0_b[973:973] );
sram6T_blwl sram_blwl_974_ (mux_2level_tapbuf_size16_66_sram_blwl_out[974:974] ,mux_2level_tapbuf_size16_66_sram_blwl_out[974:974] ,mux_2level_tapbuf_size16_66_sram_blwl_outb[974:974] ,mux_2level_tapbuf_size16_66_configbus0[974:974], mux_2level_tapbuf_size16_66_configbus1[974:974] , mux_2level_tapbuf_size16_66_configbus0_b[974:974] );
sram6T_blwl sram_blwl_975_ (mux_2level_tapbuf_size16_66_sram_blwl_out[975:975] ,mux_2level_tapbuf_size16_66_sram_blwl_out[975:975] ,mux_2level_tapbuf_size16_66_sram_blwl_outb[975:975] ,mux_2level_tapbuf_size16_66_configbus0[975:975], mux_2level_tapbuf_size16_66_configbus1[975:975] , mux_2level_tapbuf_size16_66_configbus0_b[975:975] );
wire [0:15] mux_2level_tapbuf_size16_67_inbus;
assign mux_2level_tapbuf_size16_67_inbus[0] = chany_1__1__midout_4_;
assign mux_2level_tapbuf_size16_67_inbus[1] = chany_1__1__midout_5_;
assign mux_2level_tapbuf_size16_67_inbus[2] = chany_1__1__midout_16_;
assign mux_2level_tapbuf_size16_67_inbus[3] = chany_1__1__midout_17_;
assign mux_2level_tapbuf_size16_67_inbus[4] = chany_1__1__midout_28_;
assign mux_2level_tapbuf_size16_67_inbus[5] = chany_1__1__midout_29_;
assign mux_2level_tapbuf_size16_67_inbus[6] = chany_1__1__midout_46_;
assign mux_2level_tapbuf_size16_67_inbus[7] = chany_1__1__midout_47_;
assign mux_2level_tapbuf_size16_67_inbus[8] = chany_1__1__midout_58_;
assign mux_2level_tapbuf_size16_67_inbus[9] = chany_1__1__midout_59_;
assign mux_2level_tapbuf_size16_67_inbus[10] = chany_1__1__midout_68_;
assign mux_2level_tapbuf_size16_67_inbus[11] = chany_1__1__midout_69_;
assign mux_2level_tapbuf_size16_67_inbus[12] = chany_1__1__midout_80_;
assign mux_2level_tapbuf_size16_67_inbus[13] = chany_1__1__midout_81_;
assign mux_2level_tapbuf_size16_67_inbus[14] = chany_1__1__midout_94_;
assign mux_2level_tapbuf_size16_67_inbus[15] = chany_1__1__midout_95_;
wire [976:983] mux_2level_tapbuf_size16_67_configbus0;
wire [976:983] mux_2level_tapbuf_size16_67_configbus1;
wire [976:983] mux_2level_tapbuf_size16_67_sram_blwl_out ;
wire [976:983] mux_2level_tapbuf_size16_67_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_67_configbus0[976:983] = sram_blwl_bl[976:983] ;
assign mux_2level_tapbuf_size16_67_configbus1[976:983] = sram_blwl_wl[976:983] ;
wire [976:983] mux_2level_tapbuf_size16_67_configbus0_b;
assign mux_2level_tapbuf_size16_67_configbus0_b[976:983] = sram_blwl_blb[976:983] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_67_ (mux_2level_tapbuf_size16_67_inbus, grid_1__1__pin_0__1__21_, mux_2level_tapbuf_size16_67_sram_blwl_out[976:983] ,
mux_2level_tapbuf_size16_67_sram_blwl_outb[976:983] );
//----- SRAM bits for MUX[67], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_976_ (mux_2level_tapbuf_size16_67_sram_blwl_out[976:976] ,mux_2level_tapbuf_size16_67_sram_blwl_out[976:976] ,mux_2level_tapbuf_size16_67_sram_blwl_outb[976:976] ,mux_2level_tapbuf_size16_67_configbus0[976:976], mux_2level_tapbuf_size16_67_configbus1[976:976] , mux_2level_tapbuf_size16_67_configbus0_b[976:976] );
sram6T_blwl sram_blwl_977_ (mux_2level_tapbuf_size16_67_sram_blwl_out[977:977] ,mux_2level_tapbuf_size16_67_sram_blwl_out[977:977] ,mux_2level_tapbuf_size16_67_sram_blwl_outb[977:977] ,mux_2level_tapbuf_size16_67_configbus0[977:977], mux_2level_tapbuf_size16_67_configbus1[977:977] , mux_2level_tapbuf_size16_67_configbus0_b[977:977] );
sram6T_blwl sram_blwl_978_ (mux_2level_tapbuf_size16_67_sram_blwl_out[978:978] ,mux_2level_tapbuf_size16_67_sram_blwl_out[978:978] ,mux_2level_tapbuf_size16_67_sram_blwl_outb[978:978] ,mux_2level_tapbuf_size16_67_configbus0[978:978], mux_2level_tapbuf_size16_67_configbus1[978:978] , mux_2level_tapbuf_size16_67_configbus0_b[978:978] );
sram6T_blwl sram_blwl_979_ (mux_2level_tapbuf_size16_67_sram_blwl_out[979:979] ,mux_2level_tapbuf_size16_67_sram_blwl_out[979:979] ,mux_2level_tapbuf_size16_67_sram_blwl_outb[979:979] ,mux_2level_tapbuf_size16_67_configbus0[979:979], mux_2level_tapbuf_size16_67_configbus1[979:979] , mux_2level_tapbuf_size16_67_configbus0_b[979:979] );
sram6T_blwl sram_blwl_980_ (mux_2level_tapbuf_size16_67_sram_blwl_out[980:980] ,mux_2level_tapbuf_size16_67_sram_blwl_out[980:980] ,mux_2level_tapbuf_size16_67_sram_blwl_outb[980:980] ,mux_2level_tapbuf_size16_67_configbus0[980:980], mux_2level_tapbuf_size16_67_configbus1[980:980] , mux_2level_tapbuf_size16_67_configbus0_b[980:980] );
sram6T_blwl sram_blwl_981_ (mux_2level_tapbuf_size16_67_sram_blwl_out[981:981] ,mux_2level_tapbuf_size16_67_sram_blwl_out[981:981] ,mux_2level_tapbuf_size16_67_sram_blwl_outb[981:981] ,mux_2level_tapbuf_size16_67_configbus0[981:981], mux_2level_tapbuf_size16_67_configbus1[981:981] , mux_2level_tapbuf_size16_67_configbus0_b[981:981] );
sram6T_blwl sram_blwl_982_ (mux_2level_tapbuf_size16_67_sram_blwl_out[982:982] ,mux_2level_tapbuf_size16_67_sram_blwl_out[982:982] ,mux_2level_tapbuf_size16_67_sram_blwl_outb[982:982] ,mux_2level_tapbuf_size16_67_configbus0[982:982], mux_2level_tapbuf_size16_67_configbus1[982:982] , mux_2level_tapbuf_size16_67_configbus0_b[982:982] );
sram6T_blwl sram_blwl_983_ (mux_2level_tapbuf_size16_67_sram_blwl_out[983:983] ,mux_2level_tapbuf_size16_67_sram_blwl_out[983:983] ,mux_2level_tapbuf_size16_67_sram_blwl_outb[983:983] ,mux_2level_tapbuf_size16_67_configbus0[983:983], mux_2level_tapbuf_size16_67_configbus1[983:983] , mux_2level_tapbuf_size16_67_configbus0_b[983:983] );
wire [0:15] mux_2level_tapbuf_size16_68_inbus;
assign mux_2level_tapbuf_size16_68_inbus[0] = chany_1__1__midout_4_;
assign mux_2level_tapbuf_size16_68_inbus[1] = chany_1__1__midout_5_;
assign mux_2level_tapbuf_size16_68_inbus[2] = chany_1__1__midout_18_;
assign mux_2level_tapbuf_size16_68_inbus[3] = chany_1__1__midout_19_;
assign mux_2level_tapbuf_size16_68_inbus[4] = chany_1__1__midout_38_;
assign mux_2level_tapbuf_size16_68_inbus[5] = chany_1__1__midout_39_;
assign mux_2level_tapbuf_size16_68_inbus[6] = chany_1__1__midout_46_;
assign mux_2level_tapbuf_size16_68_inbus[7] = chany_1__1__midout_47_;
assign mux_2level_tapbuf_size16_68_inbus[8] = chany_1__1__midout_58_;
assign mux_2level_tapbuf_size16_68_inbus[9] = chany_1__1__midout_59_;
assign mux_2level_tapbuf_size16_68_inbus[10] = chany_1__1__midout_70_;
assign mux_2level_tapbuf_size16_68_inbus[11] = chany_1__1__midout_71_;
assign mux_2level_tapbuf_size16_68_inbus[12] = chany_1__1__midout_82_;
assign mux_2level_tapbuf_size16_68_inbus[13] = chany_1__1__midout_83_;
assign mux_2level_tapbuf_size16_68_inbus[14] = chany_1__1__midout_94_;
assign mux_2level_tapbuf_size16_68_inbus[15] = chany_1__1__midout_95_;
wire [984:991] mux_2level_tapbuf_size16_68_configbus0;
wire [984:991] mux_2level_tapbuf_size16_68_configbus1;
wire [984:991] mux_2level_tapbuf_size16_68_sram_blwl_out ;
wire [984:991] mux_2level_tapbuf_size16_68_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_68_configbus0[984:991] = sram_blwl_bl[984:991] ;
assign mux_2level_tapbuf_size16_68_configbus1[984:991] = sram_blwl_wl[984:991] ;
wire [984:991] mux_2level_tapbuf_size16_68_configbus0_b;
assign mux_2level_tapbuf_size16_68_configbus0_b[984:991] = sram_blwl_blb[984:991] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_68_ (mux_2level_tapbuf_size16_68_inbus, grid_1__1__pin_0__1__25_, mux_2level_tapbuf_size16_68_sram_blwl_out[984:991] ,
mux_2level_tapbuf_size16_68_sram_blwl_outb[984:991] );
//----- SRAM bits for MUX[68], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_984_ (mux_2level_tapbuf_size16_68_sram_blwl_out[984:984] ,mux_2level_tapbuf_size16_68_sram_blwl_out[984:984] ,mux_2level_tapbuf_size16_68_sram_blwl_outb[984:984] ,mux_2level_tapbuf_size16_68_configbus0[984:984], mux_2level_tapbuf_size16_68_configbus1[984:984] , mux_2level_tapbuf_size16_68_configbus0_b[984:984] );
sram6T_blwl sram_blwl_985_ (mux_2level_tapbuf_size16_68_sram_blwl_out[985:985] ,mux_2level_tapbuf_size16_68_sram_blwl_out[985:985] ,mux_2level_tapbuf_size16_68_sram_blwl_outb[985:985] ,mux_2level_tapbuf_size16_68_configbus0[985:985], mux_2level_tapbuf_size16_68_configbus1[985:985] , mux_2level_tapbuf_size16_68_configbus0_b[985:985] );
sram6T_blwl sram_blwl_986_ (mux_2level_tapbuf_size16_68_sram_blwl_out[986:986] ,mux_2level_tapbuf_size16_68_sram_blwl_out[986:986] ,mux_2level_tapbuf_size16_68_sram_blwl_outb[986:986] ,mux_2level_tapbuf_size16_68_configbus0[986:986], mux_2level_tapbuf_size16_68_configbus1[986:986] , mux_2level_tapbuf_size16_68_configbus0_b[986:986] );
sram6T_blwl sram_blwl_987_ (mux_2level_tapbuf_size16_68_sram_blwl_out[987:987] ,mux_2level_tapbuf_size16_68_sram_blwl_out[987:987] ,mux_2level_tapbuf_size16_68_sram_blwl_outb[987:987] ,mux_2level_tapbuf_size16_68_configbus0[987:987], mux_2level_tapbuf_size16_68_configbus1[987:987] , mux_2level_tapbuf_size16_68_configbus0_b[987:987] );
sram6T_blwl sram_blwl_988_ (mux_2level_tapbuf_size16_68_sram_blwl_out[988:988] ,mux_2level_tapbuf_size16_68_sram_blwl_out[988:988] ,mux_2level_tapbuf_size16_68_sram_blwl_outb[988:988] ,mux_2level_tapbuf_size16_68_configbus0[988:988], mux_2level_tapbuf_size16_68_configbus1[988:988] , mux_2level_tapbuf_size16_68_configbus0_b[988:988] );
sram6T_blwl sram_blwl_989_ (mux_2level_tapbuf_size16_68_sram_blwl_out[989:989] ,mux_2level_tapbuf_size16_68_sram_blwl_out[989:989] ,mux_2level_tapbuf_size16_68_sram_blwl_outb[989:989] ,mux_2level_tapbuf_size16_68_configbus0[989:989], mux_2level_tapbuf_size16_68_configbus1[989:989] , mux_2level_tapbuf_size16_68_configbus0_b[989:989] );
sram6T_blwl sram_blwl_990_ (mux_2level_tapbuf_size16_68_sram_blwl_out[990:990] ,mux_2level_tapbuf_size16_68_sram_blwl_out[990:990] ,mux_2level_tapbuf_size16_68_sram_blwl_outb[990:990] ,mux_2level_tapbuf_size16_68_configbus0[990:990], mux_2level_tapbuf_size16_68_configbus1[990:990] , mux_2level_tapbuf_size16_68_configbus0_b[990:990] );
sram6T_blwl sram_blwl_991_ (mux_2level_tapbuf_size16_68_sram_blwl_out[991:991] ,mux_2level_tapbuf_size16_68_sram_blwl_out[991:991] ,mux_2level_tapbuf_size16_68_sram_blwl_outb[991:991] ,mux_2level_tapbuf_size16_68_configbus0[991:991], mux_2level_tapbuf_size16_68_configbus1[991:991] , mux_2level_tapbuf_size16_68_configbus0_b[991:991] );
wire [0:15] mux_2level_tapbuf_size16_69_inbus;
assign mux_2level_tapbuf_size16_69_inbus[0] = chany_1__1__midout_14_;
assign mux_2level_tapbuf_size16_69_inbus[1] = chany_1__1__midout_15_;
assign mux_2level_tapbuf_size16_69_inbus[2] = chany_1__1__midout_18_;
assign mux_2level_tapbuf_size16_69_inbus[3] = chany_1__1__midout_19_;
assign mux_2level_tapbuf_size16_69_inbus[4] = chany_1__1__midout_32_;
assign mux_2level_tapbuf_size16_69_inbus[5] = chany_1__1__midout_33_;
assign mux_2level_tapbuf_size16_69_inbus[6] = chany_1__1__midout_44_;
assign mux_2level_tapbuf_size16_69_inbus[7] = chany_1__1__midout_45_;
assign mux_2level_tapbuf_size16_69_inbus[8] = chany_1__1__midout_56_;
assign mux_2level_tapbuf_size16_69_inbus[9] = chany_1__1__midout_57_;
assign mux_2level_tapbuf_size16_69_inbus[10] = chany_1__1__midout_70_;
assign mux_2level_tapbuf_size16_69_inbus[11] = chany_1__1__midout_71_;
assign mux_2level_tapbuf_size16_69_inbus[12] = chany_1__1__midout_84_;
assign mux_2level_tapbuf_size16_69_inbus[13] = chany_1__1__midout_85_;
assign mux_2level_tapbuf_size16_69_inbus[14] = chany_1__1__midout_96_;
assign mux_2level_tapbuf_size16_69_inbus[15] = chany_1__1__midout_97_;
wire [992:999] mux_2level_tapbuf_size16_69_configbus0;
wire [992:999] mux_2level_tapbuf_size16_69_configbus1;
wire [992:999] mux_2level_tapbuf_size16_69_sram_blwl_out ;
wire [992:999] mux_2level_tapbuf_size16_69_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_69_configbus0[992:999] = sram_blwl_bl[992:999] ;
assign mux_2level_tapbuf_size16_69_configbus1[992:999] = sram_blwl_wl[992:999] ;
wire [992:999] mux_2level_tapbuf_size16_69_configbus0_b;
assign mux_2level_tapbuf_size16_69_configbus0_b[992:999] = sram_blwl_blb[992:999] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_69_ (mux_2level_tapbuf_size16_69_inbus, grid_1__1__pin_0__1__29_, mux_2level_tapbuf_size16_69_sram_blwl_out[992:999] ,
mux_2level_tapbuf_size16_69_sram_blwl_outb[992:999] );
//----- SRAM bits for MUX[69], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_992_ (mux_2level_tapbuf_size16_69_sram_blwl_out[992:992] ,mux_2level_tapbuf_size16_69_sram_blwl_out[992:992] ,mux_2level_tapbuf_size16_69_sram_blwl_outb[992:992] ,mux_2level_tapbuf_size16_69_configbus0[992:992], mux_2level_tapbuf_size16_69_configbus1[992:992] , mux_2level_tapbuf_size16_69_configbus0_b[992:992] );
sram6T_blwl sram_blwl_993_ (mux_2level_tapbuf_size16_69_sram_blwl_out[993:993] ,mux_2level_tapbuf_size16_69_sram_blwl_out[993:993] ,mux_2level_tapbuf_size16_69_sram_blwl_outb[993:993] ,mux_2level_tapbuf_size16_69_configbus0[993:993], mux_2level_tapbuf_size16_69_configbus1[993:993] , mux_2level_tapbuf_size16_69_configbus0_b[993:993] );
sram6T_blwl sram_blwl_994_ (mux_2level_tapbuf_size16_69_sram_blwl_out[994:994] ,mux_2level_tapbuf_size16_69_sram_blwl_out[994:994] ,mux_2level_tapbuf_size16_69_sram_blwl_outb[994:994] ,mux_2level_tapbuf_size16_69_configbus0[994:994], mux_2level_tapbuf_size16_69_configbus1[994:994] , mux_2level_tapbuf_size16_69_configbus0_b[994:994] );
sram6T_blwl sram_blwl_995_ (mux_2level_tapbuf_size16_69_sram_blwl_out[995:995] ,mux_2level_tapbuf_size16_69_sram_blwl_out[995:995] ,mux_2level_tapbuf_size16_69_sram_blwl_outb[995:995] ,mux_2level_tapbuf_size16_69_configbus0[995:995], mux_2level_tapbuf_size16_69_configbus1[995:995] , mux_2level_tapbuf_size16_69_configbus0_b[995:995] );
sram6T_blwl sram_blwl_996_ (mux_2level_tapbuf_size16_69_sram_blwl_out[996:996] ,mux_2level_tapbuf_size16_69_sram_blwl_out[996:996] ,mux_2level_tapbuf_size16_69_sram_blwl_outb[996:996] ,mux_2level_tapbuf_size16_69_configbus0[996:996], mux_2level_tapbuf_size16_69_configbus1[996:996] , mux_2level_tapbuf_size16_69_configbus0_b[996:996] );
sram6T_blwl sram_blwl_997_ (mux_2level_tapbuf_size16_69_sram_blwl_out[997:997] ,mux_2level_tapbuf_size16_69_sram_blwl_out[997:997] ,mux_2level_tapbuf_size16_69_sram_blwl_outb[997:997] ,mux_2level_tapbuf_size16_69_configbus0[997:997], mux_2level_tapbuf_size16_69_configbus1[997:997] , mux_2level_tapbuf_size16_69_configbus0_b[997:997] );
sram6T_blwl sram_blwl_998_ (mux_2level_tapbuf_size16_69_sram_blwl_out[998:998] ,mux_2level_tapbuf_size16_69_sram_blwl_out[998:998] ,mux_2level_tapbuf_size16_69_sram_blwl_outb[998:998] ,mux_2level_tapbuf_size16_69_configbus0[998:998], mux_2level_tapbuf_size16_69_configbus1[998:998] , mux_2level_tapbuf_size16_69_configbus0_b[998:998] );
sram6T_blwl sram_blwl_999_ (mux_2level_tapbuf_size16_69_sram_blwl_out[999:999] ,mux_2level_tapbuf_size16_69_sram_blwl_out[999:999] ,mux_2level_tapbuf_size16_69_sram_blwl_outb[999:999] ,mux_2level_tapbuf_size16_69_configbus0[999:999], mux_2level_tapbuf_size16_69_configbus1[999:999] , mux_2level_tapbuf_size16_69_configbus0_b[999:999] );
wire [0:15] mux_2level_tapbuf_size16_70_inbus;
assign mux_2level_tapbuf_size16_70_inbus[0] = chany_1__1__midout_8_;
assign mux_2level_tapbuf_size16_70_inbus[1] = chany_1__1__midout_9_;
assign mux_2level_tapbuf_size16_70_inbus[2] = chany_1__1__midout_20_;
assign mux_2level_tapbuf_size16_70_inbus[3] = chany_1__1__midout_21_;
assign mux_2level_tapbuf_size16_70_inbus[4] = chany_1__1__midout_32_;
assign mux_2level_tapbuf_size16_70_inbus[5] = chany_1__1__midout_33_;
assign mux_2level_tapbuf_size16_70_inbus[6] = chany_1__1__midout_44_;
assign mux_2level_tapbuf_size16_70_inbus[7] = chany_1__1__midout_45_;
assign mux_2level_tapbuf_size16_70_inbus[8] = chany_1__1__midout_62_;
assign mux_2level_tapbuf_size16_70_inbus[9] = chany_1__1__midout_63_;
assign mux_2level_tapbuf_size16_70_inbus[10] = chany_1__1__midout_72_;
assign mux_2level_tapbuf_size16_70_inbus[11] = chany_1__1__midout_73_;
assign mux_2level_tapbuf_size16_70_inbus[12] = chany_1__1__midout_84_;
assign mux_2level_tapbuf_size16_70_inbus[13] = chany_1__1__midout_85_;
assign mux_2level_tapbuf_size16_70_inbus[14] = chany_1__1__midout_96_;
assign mux_2level_tapbuf_size16_70_inbus[15] = chany_1__1__midout_97_;
wire [1000:1007] mux_2level_tapbuf_size16_70_configbus0;
wire [1000:1007] mux_2level_tapbuf_size16_70_configbus1;
wire [1000:1007] mux_2level_tapbuf_size16_70_sram_blwl_out ;
wire [1000:1007] mux_2level_tapbuf_size16_70_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_70_configbus0[1000:1007] = sram_blwl_bl[1000:1007] ;
assign mux_2level_tapbuf_size16_70_configbus1[1000:1007] = sram_blwl_wl[1000:1007] ;
wire [1000:1007] mux_2level_tapbuf_size16_70_configbus0_b;
assign mux_2level_tapbuf_size16_70_configbus0_b[1000:1007] = sram_blwl_blb[1000:1007] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_70_ (mux_2level_tapbuf_size16_70_inbus, grid_1__1__pin_0__1__33_, mux_2level_tapbuf_size16_70_sram_blwl_out[1000:1007] ,
mux_2level_tapbuf_size16_70_sram_blwl_outb[1000:1007] );
//----- SRAM bits for MUX[70], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_1000_ (mux_2level_tapbuf_size16_70_sram_blwl_out[1000:1000] ,mux_2level_tapbuf_size16_70_sram_blwl_out[1000:1000] ,mux_2level_tapbuf_size16_70_sram_blwl_outb[1000:1000] ,mux_2level_tapbuf_size16_70_configbus0[1000:1000], mux_2level_tapbuf_size16_70_configbus1[1000:1000] , mux_2level_tapbuf_size16_70_configbus0_b[1000:1000] );
sram6T_blwl sram_blwl_1001_ (mux_2level_tapbuf_size16_70_sram_blwl_out[1001:1001] ,mux_2level_tapbuf_size16_70_sram_blwl_out[1001:1001] ,mux_2level_tapbuf_size16_70_sram_blwl_outb[1001:1001] ,mux_2level_tapbuf_size16_70_configbus0[1001:1001], mux_2level_tapbuf_size16_70_configbus1[1001:1001] , mux_2level_tapbuf_size16_70_configbus0_b[1001:1001] );
sram6T_blwl sram_blwl_1002_ (mux_2level_tapbuf_size16_70_sram_blwl_out[1002:1002] ,mux_2level_tapbuf_size16_70_sram_blwl_out[1002:1002] ,mux_2level_tapbuf_size16_70_sram_blwl_outb[1002:1002] ,mux_2level_tapbuf_size16_70_configbus0[1002:1002], mux_2level_tapbuf_size16_70_configbus1[1002:1002] , mux_2level_tapbuf_size16_70_configbus0_b[1002:1002] );
sram6T_blwl sram_blwl_1003_ (mux_2level_tapbuf_size16_70_sram_blwl_out[1003:1003] ,mux_2level_tapbuf_size16_70_sram_blwl_out[1003:1003] ,mux_2level_tapbuf_size16_70_sram_blwl_outb[1003:1003] ,mux_2level_tapbuf_size16_70_configbus0[1003:1003], mux_2level_tapbuf_size16_70_configbus1[1003:1003] , mux_2level_tapbuf_size16_70_configbus0_b[1003:1003] );
sram6T_blwl sram_blwl_1004_ (mux_2level_tapbuf_size16_70_sram_blwl_out[1004:1004] ,mux_2level_tapbuf_size16_70_sram_blwl_out[1004:1004] ,mux_2level_tapbuf_size16_70_sram_blwl_outb[1004:1004] ,mux_2level_tapbuf_size16_70_configbus0[1004:1004], mux_2level_tapbuf_size16_70_configbus1[1004:1004] , mux_2level_tapbuf_size16_70_configbus0_b[1004:1004] );
sram6T_blwl sram_blwl_1005_ (mux_2level_tapbuf_size16_70_sram_blwl_out[1005:1005] ,mux_2level_tapbuf_size16_70_sram_blwl_out[1005:1005] ,mux_2level_tapbuf_size16_70_sram_blwl_outb[1005:1005] ,mux_2level_tapbuf_size16_70_configbus0[1005:1005], mux_2level_tapbuf_size16_70_configbus1[1005:1005] , mux_2level_tapbuf_size16_70_configbus0_b[1005:1005] );
sram6T_blwl sram_blwl_1006_ (mux_2level_tapbuf_size16_70_sram_blwl_out[1006:1006] ,mux_2level_tapbuf_size16_70_sram_blwl_out[1006:1006] ,mux_2level_tapbuf_size16_70_sram_blwl_outb[1006:1006] ,mux_2level_tapbuf_size16_70_configbus0[1006:1006], mux_2level_tapbuf_size16_70_configbus1[1006:1006] , mux_2level_tapbuf_size16_70_configbus0_b[1006:1006] );
sram6T_blwl sram_blwl_1007_ (mux_2level_tapbuf_size16_70_sram_blwl_out[1007:1007] ,mux_2level_tapbuf_size16_70_sram_blwl_out[1007:1007] ,mux_2level_tapbuf_size16_70_sram_blwl_outb[1007:1007] ,mux_2level_tapbuf_size16_70_configbus0[1007:1007], mux_2level_tapbuf_size16_70_configbus1[1007:1007] , mux_2level_tapbuf_size16_70_configbus0_b[1007:1007] );
wire [0:15] mux_2level_tapbuf_size16_71_inbus;
assign mux_2level_tapbuf_size16_71_inbus[0] = chany_1__1__midout_8_;
assign mux_2level_tapbuf_size16_71_inbus[1] = chany_1__1__midout_9_;
assign mux_2level_tapbuf_size16_71_inbus[2] = chany_1__1__midout_30_;
assign mux_2level_tapbuf_size16_71_inbus[3] = chany_1__1__midout_31_;
assign mux_2level_tapbuf_size16_71_inbus[4] = chany_1__1__midout_34_;
assign mux_2level_tapbuf_size16_71_inbus[5] = chany_1__1__midout_35_;
assign mux_2level_tapbuf_size16_71_inbus[6] = chany_1__1__midout_50_;
assign mux_2level_tapbuf_size16_71_inbus[7] = chany_1__1__midout_51_;
assign mux_2level_tapbuf_size16_71_inbus[8] = chany_1__1__midout_62_;
assign mux_2level_tapbuf_size16_71_inbus[9] = chany_1__1__midout_63_;
assign mux_2level_tapbuf_size16_71_inbus[10] = chany_1__1__midout_74_;
assign mux_2level_tapbuf_size16_71_inbus[11] = chany_1__1__midout_75_;
assign mux_2level_tapbuf_size16_71_inbus[12] = chany_1__1__midout_86_;
assign mux_2level_tapbuf_size16_71_inbus[13] = chany_1__1__midout_87_;
assign mux_2level_tapbuf_size16_71_inbus[14] = chany_1__1__midout_98_;
assign mux_2level_tapbuf_size16_71_inbus[15] = chany_1__1__midout_99_;
wire [1008:1015] mux_2level_tapbuf_size16_71_configbus0;
wire [1008:1015] mux_2level_tapbuf_size16_71_configbus1;
wire [1008:1015] mux_2level_tapbuf_size16_71_sram_blwl_out ;
wire [1008:1015] mux_2level_tapbuf_size16_71_sram_blwl_outb ;
assign mux_2level_tapbuf_size16_71_configbus0[1008:1015] = sram_blwl_bl[1008:1015] ;
assign mux_2level_tapbuf_size16_71_configbus1[1008:1015] = sram_blwl_wl[1008:1015] ;
wire [1008:1015] mux_2level_tapbuf_size16_71_configbus0_b;
assign mux_2level_tapbuf_size16_71_configbus0_b[1008:1015] = sram_blwl_blb[1008:1015] ;
mux_2level_tapbuf_size16 mux_2level_tapbuf_size16_71_ (mux_2level_tapbuf_size16_71_inbus, grid_1__1__pin_0__1__37_, mux_2level_tapbuf_size16_71_sram_blwl_out[1008:1015] ,
mux_2level_tapbuf_size16_71_sram_blwl_outb[1008:1015] );
//----- SRAM bits for MUX[71], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----10001000-----
sram6T_blwl sram_blwl_1008_ (mux_2level_tapbuf_size16_71_sram_blwl_out[1008:1008] ,mux_2level_tapbuf_size16_71_sram_blwl_out[1008:1008] ,mux_2level_tapbuf_size16_71_sram_blwl_outb[1008:1008] ,mux_2level_tapbuf_size16_71_configbus0[1008:1008], mux_2level_tapbuf_size16_71_configbus1[1008:1008] , mux_2level_tapbuf_size16_71_configbus0_b[1008:1008] );
sram6T_blwl sram_blwl_1009_ (mux_2level_tapbuf_size16_71_sram_blwl_out[1009:1009] ,mux_2level_tapbuf_size16_71_sram_blwl_out[1009:1009] ,mux_2level_tapbuf_size16_71_sram_blwl_outb[1009:1009] ,mux_2level_tapbuf_size16_71_configbus0[1009:1009], mux_2level_tapbuf_size16_71_configbus1[1009:1009] , mux_2level_tapbuf_size16_71_configbus0_b[1009:1009] );
sram6T_blwl sram_blwl_1010_ (mux_2level_tapbuf_size16_71_sram_blwl_out[1010:1010] ,mux_2level_tapbuf_size16_71_sram_blwl_out[1010:1010] ,mux_2level_tapbuf_size16_71_sram_blwl_outb[1010:1010] ,mux_2level_tapbuf_size16_71_configbus0[1010:1010], mux_2level_tapbuf_size16_71_configbus1[1010:1010] , mux_2level_tapbuf_size16_71_configbus0_b[1010:1010] );
sram6T_blwl sram_blwl_1011_ (mux_2level_tapbuf_size16_71_sram_blwl_out[1011:1011] ,mux_2level_tapbuf_size16_71_sram_blwl_out[1011:1011] ,mux_2level_tapbuf_size16_71_sram_blwl_outb[1011:1011] ,mux_2level_tapbuf_size16_71_configbus0[1011:1011], mux_2level_tapbuf_size16_71_configbus1[1011:1011] , mux_2level_tapbuf_size16_71_configbus0_b[1011:1011] );
sram6T_blwl sram_blwl_1012_ (mux_2level_tapbuf_size16_71_sram_blwl_out[1012:1012] ,mux_2level_tapbuf_size16_71_sram_blwl_out[1012:1012] ,mux_2level_tapbuf_size16_71_sram_blwl_outb[1012:1012] ,mux_2level_tapbuf_size16_71_configbus0[1012:1012], mux_2level_tapbuf_size16_71_configbus1[1012:1012] , mux_2level_tapbuf_size16_71_configbus0_b[1012:1012] );
sram6T_blwl sram_blwl_1013_ (mux_2level_tapbuf_size16_71_sram_blwl_out[1013:1013] ,mux_2level_tapbuf_size16_71_sram_blwl_out[1013:1013] ,mux_2level_tapbuf_size16_71_sram_blwl_outb[1013:1013] ,mux_2level_tapbuf_size16_71_configbus0[1013:1013], mux_2level_tapbuf_size16_71_configbus1[1013:1013] , mux_2level_tapbuf_size16_71_configbus0_b[1013:1013] );
sram6T_blwl sram_blwl_1014_ (mux_2level_tapbuf_size16_71_sram_blwl_out[1014:1014] ,mux_2level_tapbuf_size16_71_sram_blwl_out[1014:1014] ,mux_2level_tapbuf_size16_71_sram_blwl_outb[1014:1014] ,mux_2level_tapbuf_size16_71_configbus0[1014:1014], mux_2level_tapbuf_size16_71_configbus1[1014:1014] , mux_2level_tapbuf_size16_71_configbus0_b[1014:1014] );
sram6T_blwl sram_blwl_1015_ (mux_2level_tapbuf_size16_71_sram_blwl_out[1015:1015] ,mux_2level_tapbuf_size16_71_sram_blwl_out[1015:1015] ,mux_2level_tapbuf_size16_71_sram_blwl_outb[1015:1015] ,mux_2level_tapbuf_size16_71_configbus0[1015:1015], mux_2level_tapbuf_size16_71_configbus1[1015:1015] , mux_2level_tapbuf_size16_71_configbus0_b[1015:1015] );
endmodule
//----- END Verilog Module of Connection Box -Y direction [1][1] -----

