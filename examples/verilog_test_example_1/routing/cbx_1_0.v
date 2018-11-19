//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Connection Block - X direction  [1][0] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
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

output  grid_1__1__pin_0__2__2_,

output  grid_1__0__pin_0__0__0_,

output  grid_1__0__pin_0__0__2_,

output  grid_1__0__pin_0__0__4_,

output  grid_1__0__pin_0__0__6_,

output  grid_1__0__pin_0__0__8_,

output  grid_1__0__pin_0__0__10_,

output  grid_1__0__pin_0__0__12_,

output  grid_1__0__pin_0__0__14_,

input [144:179] sram_blwl_bl ,
input [144:179] sram_blwl_wl ,
input [144:179] sram_blwl_blb );
wire [0:3] mux_2level_tapbuf_size4_0_inbus;
assign mux_2level_tapbuf_size4_0_inbus[0] = chanx_1__0__midout_6_;
assign mux_2level_tapbuf_size4_0_inbus[1] = chanx_1__0__midout_7_;
assign mux_2level_tapbuf_size4_0_inbus[2] = chanx_1__0__midout_22_;
assign mux_2level_tapbuf_size4_0_inbus[3] = chanx_1__0__midout_23_;
wire [144:147] mux_2level_tapbuf_size4_0_configbus0;
wire [144:147] mux_2level_tapbuf_size4_0_configbus1;
wire [144:147] mux_2level_tapbuf_size4_0_sram_blwl_out ;
wire [144:147] mux_2level_tapbuf_size4_0_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_0_configbus0[144:147] = sram_blwl_bl[144:147] ;
assign mux_2level_tapbuf_size4_0_configbus1[144:147] = sram_blwl_wl[144:147] ;
wire [144:147] mux_2level_tapbuf_size4_0_configbus0_b;
assign mux_2level_tapbuf_size4_0_configbus0_b[144:147] = sram_blwl_blb[144:147] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_0_ (mux_2level_tapbuf_size4_0_inbus, grid_1__1__pin_0__2__2_, mux_2level_tapbuf_size4_0_sram_blwl_out[144:147] ,
mux_2level_tapbuf_size4_0_sram_blwl_outb[144:147] );
//----- SRAM bits for MUX[0], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_144_ (mux_2level_tapbuf_size4_0_sram_blwl_out[144:144] ,mux_2level_tapbuf_size4_0_sram_blwl_out[144:144] ,mux_2level_tapbuf_size4_0_sram_blwl_outb[144:144] ,mux_2level_tapbuf_size4_0_configbus0[144:144], mux_2level_tapbuf_size4_0_configbus1[144:144] , mux_2level_tapbuf_size4_0_configbus0_b[144:144] );
sram6T_blwl sram_blwl_145_ (mux_2level_tapbuf_size4_0_sram_blwl_out[145:145] ,mux_2level_tapbuf_size4_0_sram_blwl_out[145:145] ,mux_2level_tapbuf_size4_0_sram_blwl_outb[145:145] ,mux_2level_tapbuf_size4_0_configbus0[145:145], mux_2level_tapbuf_size4_0_configbus1[145:145] , mux_2level_tapbuf_size4_0_configbus0_b[145:145] );
sram6T_blwl sram_blwl_146_ (mux_2level_tapbuf_size4_0_sram_blwl_out[146:146] ,mux_2level_tapbuf_size4_0_sram_blwl_out[146:146] ,mux_2level_tapbuf_size4_0_sram_blwl_outb[146:146] ,mux_2level_tapbuf_size4_0_configbus0[146:146], mux_2level_tapbuf_size4_0_configbus1[146:146] , mux_2level_tapbuf_size4_0_configbus0_b[146:146] );
sram6T_blwl sram_blwl_147_ (mux_2level_tapbuf_size4_0_sram_blwl_out[147:147] ,mux_2level_tapbuf_size4_0_sram_blwl_out[147:147] ,mux_2level_tapbuf_size4_0_sram_blwl_outb[147:147] ,mux_2level_tapbuf_size4_0_configbus0[147:147], mux_2level_tapbuf_size4_0_configbus1[147:147] , mux_2level_tapbuf_size4_0_configbus0_b[147:147] );
wire [0:3] mux_2level_tapbuf_size4_1_inbus;
assign mux_2level_tapbuf_size4_1_inbus[0] = chanx_1__0__midout_0_;
assign mux_2level_tapbuf_size4_1_inbus[1] = chanx_1__0__midout_1_;
assign mux_2level_tapbuf_size4_1_inbus[2] = chanx_1__0__midout_14_;
assign mux_2level_tapbuf_size4_1_inbus[3] = chanx_1__0__midout_15_;
wire [148:151] mux_2level_tapbuf_size4_1_configbus0;
wire [148:151] mux_2level_tapbuf_size4_1_configbus1;
wire [148:151] mux_2level_tapbuf_size4_1_sram_blwl_out ;
wire [148:151] mux_2level_tapbuf_size4_1_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_1_configbus0[148:151] = sram_blwl_bl[148:151] ;
assign mux_2level_tapbuf_size4_1_configbus1[148:151] = sram_blwl_wl[148:151] ;
wire [148:151] mux_2level_tapbuf_size4_1_configbus0_b;
assign mux_2level_tapbuf_size4_1_configbus0_b[148:151] = sram_blwl_blb[148:151] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_1_ (mux_2level_tapbuf_size4_1_inbus, grid_1__0__pin_0__0__0_, mux_2level_tapbuf_size4_1_sram_blwl_out[148:151] ,
mux_2level_tapbuf_size4_1_sram_blwl_outb[148:151] );
//----- SRAM bits for MUX[1], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_148_ (mux_2level_tapbuf_size4_1_sram_blwl_out[148:148] ,mux_2level_tapbuf_size4_1_sram_blwl_out[148:148] ,mux_2level_tapbuf_size4_1_sram_blwl_outb[148:148] ,mux_2level_tapbuf_size4_1_configbus0[148:148], mux_2level_tapbuf_size4_1_configbus1[148:148] , mux_2level_tapbuf_size4_1_configbus0_b[148:148] );
sram6T_blwl sram_blwl_149_ (mux_2level_tapbuf_size4_1_sram_blwl_out[149:149] ,mux_2level_tapbuf_size4_1_sram_blwl_out[149:149] ,mux_2level_tapbuf_size4_1_sram_blwl_outb[149:149] ,mux_2level_tapbuf_size4_1_configbus0[149:149], mux_2level_tapbuf_size4_1_configbus1[149:149] , mux_2level_tapbuf_size4_1_configbus0_b[149:149] );
sram6T_blwl sram_blwl_150_ (mux_2level_tapbuf_size4_1_sram_blwl_out[150:150] ,mux_2level_tapbuf_size4_1_sram_blwl_out[150:150] ,mux_2level_tapbuf_size4_1_sram_blwl_outb[150:150] ,mux_2level_tapbuf_size4_1_configbus0[150:150], mux_2level_tapbuf_size4_1_configbus1[150:150] , mux_2level_tapbuf_size4_1_configbus0_b[150:150] );
sram6T_blwl sram_blwl_151_ (mux_2level_tapbuf_size4_1_sram_blwl_out[151:151] ,mux_2level_tapbuf_size4_1_sram_blwl_out[151:151] ,mux_2level_tapbuf_size4_1_sram_blwl_outb[151:151] ,mux_2level_tapbuf_size4_1_configbus0[151:151], mux_2level_tapbuf_size4_1_configbus1[151:151] , mux_2level_tapbuf_size4_1_configbus0_b[151:151] );
wire [0:3] mux_2level_tapbuf_size4_2_inbus;
assign mux_2level_tapbuf_size4_2_inbus[0] = chanx_1__0__midout_0_;
assign mux_2level_tapbuf_size4_2_inbus[1] = chanx_1__0__midout_1_;
assign mux_2level_tapbuf_size4_2_inbus[2] = chanx_1__0__midout_16_;
assign mux_2level_tapbuf_size4_2_inbus[3] = chanx_1__0__midout_17_;
wire [152:155] mux_2level_tapbuf_size4_2_configbus0;
wire [152:155] mux_2level_tapbuf_size4_2_configbus1;
wire [152:155] mux_2level_tapbuf_size4_2_sram_blwl_out ;
wire [152:155] mux_2level_tapbuf_size4_2_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_2_configbus0[152:155] = sram_blwl_bl[152:155] ;
assign mux_2level_tapbuf_size4_2_configbus1[152:155] = sram_blwl_wl[152:155] ;
wire [152:155] mux_2level_tapbuf_size4_2_configbus0_b;
assign mux_2level_tapbuf_size4_2_configbus0_b[152:155] = sram_blwl_blb[152:155] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_2_ (mux_2level_tapbuf_size4_2_inbus, grid_1__0__pin_0__0__2_, mux_2level_tapbuf_size4_2_sram_blwl_out[152:155] ,
mux_2level_tapbuf_size4_2_sram_blwl_outb[152:155] );
//----- SRAM bits for MUX[2], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_152_ (mux_2level_tapbuf_size4_2_sram_blwl_out[152:152] ,mux_2level_tapbuf_size4_2_sram_blwl_out[152:152] ,mux_2level_tapbuf_size4_2_sram_blwl_outb[152:152] ,mux_2level_tapbuf_size4_2_configbus0[152:152], mux_2level_tapbuf_size4_2_configbus1[152:152] , mux_2level_tapbuf_size4_2_configbus0_b[152:152] );
sram6T_blwl sram_blwl_153_ (mux_2level_tapbuf_size4_2_sram_blwl_out[153:153] ,mux_2level_tapbuf_size4_2_sram_blwl_out[153:153] ,mux_2level_tapbuf_size4_2_sram_blwl_outb[153:153] ,mux_2level_tapbuf_size4_2_configbus0[153:153], mux_2level_tapbuf_size4_2_configbus1[153:153] , mux_2level_tapbuf_size4_2_configbus0_b[153:153] );
sram6T_blwl sram_blwl_154_ (mux_2level_tapbuf_size4_2_sram_blwl_out[154:154] ,mux_2level_tapbuf_size4_2_sram_blwl_out[154:154] ,mux_2level_tapbuf_size4_2_sram_blwl_outb[154:154] ,mux_2level_tapbuf_size4_2_configbus0[154:154], mux_2level_tapbuf_size4_2_configbus1[154:154] , mux_2level_tapbuf_size4_2_configbus0_b[154:154] );
sram6T_blwl sram_blwl_155_ (mux_2level_tapbuf_size4_2_sram_blwl_out[155:155] ,mux_2level_tapbuf_size4_2_sram_blwl_out[155:155] ,mux_2level_tapbuf_size4_2_sram_blwl_outb[155:155] ,mux_2level_tapbuf_size4_2_configbus0[155:155], mux_2level_tapbuf_size4_2_configbus1[155:155] , mux_2level_tapbuf_size4_2_configbus0_b[155:155] );
wire [0:3] mux_2level_tapbuf_size4_3_inbus;
assign mux_2level_tapbuf_size4_3_inbus[0] = chanx_1__0__midout_2_;
assign mux_2level_tapbuf_size4_3_inbus[1] = chanx_1__0__midout_3_;
assign mux_2level_tapbuf_size4_3_inbus[2] = chanx_1__0__midout_18_;
assign mux_2level_tapbuf_size4_3_inbus[3] = chanx_1__0__midout_19_;
wire [156:159] mux_2level_tapbuf_size4_3_configbus0;
wire [156:159] mux_2level_tapbuf_size4_3_configbus1;
wire [156:159] mux_2level_tapbuf_size4_3_sram_blwl_out ;
wire [156:159] mux_2level_tapbuf_size4_3_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_3_configbus0[156:159] = sram_blwl_bl[156:159] ;
assign mux_2level_tapbuf_size4_3_configbus1[156:159] = sram_blwl_wl[156:159] ;
wire [156:159] mux_2level_tapbuf_size4_3_configbus0_b;
assign mux_2level_tapbuf_size4_3_configbus0_b[156:159] = sram_blwl_blb[156:159] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_3_ (mux_2level_tapbuf_size4_3_inbus, grid_1__0__pin_0__0__4_, mux_2level_tapbuf_size4_3_sram_blwl_out[156:159] ,
mux_2level_tapbuf_size4_3_sram_blwl_outb[156:159] );
//----- SRAM bits for MUX[3], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_156_ (mux_2level_tapbuf_size4_3_sram_blwl_out[156:156] ,mux_2level_tapbuf_size4_3_sram_blwl_out[156:156] ,mux_2level_tapbuf_size4_3_sram_blwl_outb[156:156] ,mux_2level_tapbuf_size4_3_configbus0[156:156], mux_2level_tapbuf_size4_3_configbus1[156:156] , mux_2level_tapbuf_size4_3_configbus0_b[156:156] );
sram6T_blwl sram_blwl_157_ (mux_2level_tapbuf_size4_3_sram_blwl_out[157:157] ,mux_2level_tapbuf_size4_3_sram_blwl_out[157:157] ,mux_2level_tapbuf_size4_3_sram_blwl_outb[157:157] ,mux_2level_tapbuf_size4_3_configbus0[157:157], mux_2level_tapbuf_size4_3_configbus1[157:157] , mux_2level_tapbuf_size4_3_configbus0_b[157:157] );
sram6T_blwl sram_blwl_158_ (mux_2level_tapbuf_size4_3_sram_blwl_out[158:158] ,mux_2level_tapbuf_size4_3_sram_blwl_out[158:158] ,mux_2level_tapbuf_size4_3_sram_blwl_outb[158:158] ,mux_2level_tapbuf_size4_3_configbus0[158:158], mux_2level_tapbuf_size4_3_configbus1[158:158] , mux_2level_tapbuf_size4_3_configbus0_b[158:158] );
sram6T_blwl sram_blwl_159_ (mux_2level_tapbuf_size4_3_sram_blwl_out[159:159] ,mux_2level_tapbuf_size4_3_sram_blwl_out[159:159] ,mux_2level_tapbuf_size4_3_sram_blwl_outb[159:159] ,mux_2level_tapbuf_size4_3_configbus0[159:159], mux_2level_tapbuf_size4_3_configbus1[159:159] , mux_2level_tapbuf_size4_3_configbus0_b[159:159] );
wire [0:3] mux_2level_tapbuf_size4_4_inbus;
assign mux_2level_tapbuf_size4_4_inbus[0] = chanx_1__0__midout_4_;
assign mux_2level_tapbuf_size4_4_inbus[1] = chanx_1__0__midout_5_;
assign mux_2level_tapbuf_size4_4_inbus[2] = chanx_1__0__midout_20_;
assign mux_2level_tapbuf_size4_4_inbus[3] = chanx_1__0__midout_21_;
wire [160:163] mux_2level_tapbuf_size4_4_configbus0;
wire [160:163] mux_2level_tapbuf_size4_4_configbus1;
wire [160:163] mux_2level_tapbuf_size4_4_sram_blwl_out ;
wire [160:163] mux_2level_tapbuf_size4_4_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_4_configbus0[160:163] = sram_blwl_bl[160:163] ;
assign mux_2level_tapbuf_size4_4_configbus1[160:163] = sram_blwl_wl[160:163] ;
wire [160:163] mux_2level_tapbuf_size4_4_configbus0_b;
assign mux_2level_tapbuf_size4_4_configbus0_b[160:163] = sram_blwl_blb[160:163] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_4_ (mux_2level_tapbuf_size4_4_inbus, grid_1__0__pin_0__0__6_, mux_2level_tapbuf_size4_4_sram_blwl_out[160:163] ,
mux_2level_tapbuf_size4_4_sram_blwl_outb[160:163] );
//----- SRAM bits for MUX[4], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_160_ (mux_2level_tapbuf_size4_4_sram_blwl_out[160:160] ,mux_2level_tapbuf_size4_4_sram_blwl_out[160:160] ,mux_2level_tapbuf_size4_4_sram_blwl_outb[160:160] ,mux_2level_tapbuf_size4_4_configbus0[160:160], mux_2level_tapbuf_size4_4_configbus1[160:160] , mux_2level_tapbuf_size4_4_configbus0_b[160:160] );
sram6T_blwl sram_blwl_161_ (mux_2level_tapbuf_size4_4_sram_blwl_out[161:161] ,mux_2level_tapbuf_size4_4_sram_blwl_out[161:161] ,mux_2level_tapbuf_size4_4_sram_blwl_outb[161:161] ,mux_2level_tapbuf_size4_4_configbus0[161:161], mux_2level_tapbuf_size4_4_configbus1[161:161] , mux_2level_tapbuf_size4_4_configbus0_b[161:161] );
sram6T_blwl sram_blwl_162_ (mux_2level_tapbuf_size4_4_sram_blwl_out[162:162] ,mux_2level_tapbuf_size4_4_sram_blwl_out[162:162] ,mux_2level_tapbuf_size4_4_sram_blwl_outb[162:162] ,mux_2level_tapbuf_size4_4_configbus0[162:162], mux_2level_tapbuf_size4_4_configbus1[162:162] , mux_2level_tapbuf_size4_4_configbus0_b[162:162] );
sram6T_blwl sram_blwl_163_ (mux_2level_tapbuf_size4_4_sram_blwl_out[163:163] ,mux_2level_tapbuf_size4_4_sram_blwl_out[163:163] ,mux_2level_tapbuf_size4_4_sram_blwl_outb[163:163] ,mux_2level_tapbuf_size4_4_configbus0[163:163], mux_2level_tapbuf_size4_4_configbus1[163:163] , mux_2level_tapbuf_size4_4_configbus0_b[163:163] );
wire [0:3] mux_2level_tapbuf_size4_5_inbus;
assign mux_2level_tapbuf_size4_5_inbus[0] = chanx_1__0__midout_6_;
assign mux_2level_tapbuf_size4_5_inbus[1] = chanx_1__0__midout_7_;
assign mux_2level_tapbuf_size4_5_inbus[2] = chanx_1__0__midout_22_;
assign mux_2level_tapbuf_size4_5_inbus[3] = chanx_1__0__midout_23_;
wire [164:167] mux_2level_tapbuf_size4_5_configbus0;
wire [164:167] mux_2level_tapbuf_size4_5_configbus1;
wire [164:167] mux_2level_tapbuf_size4_5_sram_blwl_out ;
wire [164:167] mux_2level_tapbuf_size4_5_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_5_configbus0[164:167] = sram_blwl_bl[164:167] ;
assign mux_2level_tapbuf_size4_5_configbus1[164:167] = sram_blwl_wl[164:167] ;
wire [164:167] mux_2level_tapbuf_size4_5_configbus0_b;
assign mux_2level_tapbuf_size4_5_configbus0_b[164:167] = sram_blwl_blb[164:167] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_5_ (mux_2level_tapbuf_size4_5_inbus, grid_1__0__pin_0__0__8_, mux_2level_tapbuf_size4_5_sram_blwl_out[164:167] ,
mux_2level_tapbuf_size4_5_sram_blwl_outb[164:167] );
//----- SRAM bits for MUX[5], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_164_ (mux_2level_tapbuf_size4_5_sram_blwl_out[164:164] ,mux_2level_tapbuf_size4_5_sram_blwl_out[164:164] ,mux_2level_tapbuf_size4_5_sram_blwl_outb[164:164] ,mux_2level_tapbuf_size4_5_configbus0[164:164], mux_2level_tapbuf_size4_5_configbus1[164:164] , mux_2level_tapbuf_size4_5_configbus0_b[164:164] );
sram6T_blwl sram_blwl_165_ (mux_2level_tapbuf_size4_5_sram_blwl_out[165:165] ,mux_2level_tapbuf_size4_5_sram_blwl_out[165:165] ,mux_2level_tapbuf_size4_5_sram_blwl_outb[165:165] ,mux_2level_tapbuf_size4_5_configbus0[165:165], mux_2level_tapbuf_size4_5_configbus1[165:165] , mux_2level_tapbuf_size4_5_configbus0_b[165:165] );
sram6T_blwl sram_blwl_166_ (mux_2level_tapbuf_size4_5_sram_blwl_out[166:166] ,mux_2level_tapbuf_size4_5_sram_blwl_out[166:166] ,mux_2level_tapbuf_size4_5_sram_blwl_outb[166:166] ,mux_2level_tapbuf_size4_5_configbus0[166:166], mux_2level_tapbuf_size4_5_configbus1[166:166] , mux_2level_tapbuf_size4_5_configbus0_b[166:166] );
sram6T_blwl sram_blwl_167_ (mux_2level_tapbuf_size4_5_sram_blwl_out[167:167] ,mux_2level_tapbuf_size4_5_sram_blwl_out[167:167] ,mux_2level_tapbuf_size4_5_sram_blwl_outb[167:167] ,mux_2level_tapbuf_size4_5_configbus0[167:167], mux_2level_tapbuf_size4_5_configbus1[167:167] , mux_2level_tapbuf_size4_5_configbus0_b[167:167] );
wire [0:3] mux_2level_tapbuf_size4_6_inbus;
assign mux_2level_tapbuf_size4_6_inbus[0] = chanx_1__0__midout_8_;
assign mux_2level_tapbuf_size4_6_inbus[1] = chanx_1__0__midout_9_;
assign mux_2level_tapbuf_size4_6_inbus[2] = chanx_1__0__midout_24_;
assign mux_2level_tapbuf_size4_6_inbus[3] = chanx_1__0__midout_25_;
wire [168:171] mux_2level_tapbuf_size4_6_configbus0;
wire [168:171] mux_2level_tapbuf_size4_6_configbus1;
wire [168:171] mux_2level_tapbuf_size4_6_sram_blwl_out ;
wire [168:171] mux_2level_tapbuf_size4_6_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_6_configbus0[168:171] = sram_blwl_bl[168:171] ;
assign mux_2level_tapbuf_size4_6_configbus1[168:171] = sram_blwl_wl[168:171] ;
wire [168:171] mux_2level_tapbuf_size4_6_configbus0_b;
assign mux_2level_tapbuf_size4_6_configbus0_b[168:171] = sram_blwl_blb[168:171] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_6_ (mux_2level_tapbuf_size4_6_inbus, grid_1__0__pin_0__0__10_, mux_2level_tapbuf_size4_6_sram_blwl_out[168:171] ,
mux_2level_tapbuf_size4_6_sram_blwl_outb[168:171] );
//----- SRAM bits for MUX[6], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_168_ (mux_2level_tapbuf_size4_6_sram_blwl_out[168:168] ,mux_2level_tapbuf_size4_6_sram_blwl_out[168:168] ,mux_2level_tapbuf_size4_6_sram_blwl_outb[168:168] ,mux_2level_tapbuf_size4_6_configbus0[168:168], mux_2level_tapbuf_size4_6_configbus1[168:168] , mux_2level_tapbuf_size4_6_configbus0_b[168:168] );
sram6T_blwl sram_blwl_169_ (mux_2level_tapbuf_size4_6_sram_blwl_out[169:169] ,mux_2level_tapbuf_size4_6_sram_blwl_out[169:169] ,mux_2level_tapbuf_size4_6_sram_blwl_outb[169:169] ,mux_2level_tapbuf_size4_6_configbus0[169:169], mux_2level_tapbuf_size4_6_configbus1[169:169] , mux_2level_tapbuf_size4_6_configbus0_b[169:169] );
sram6T_blwl sram_blwl_170_ (mux_2level_tapbuf_size4_6_sram_blwl_out[170:170] ,mux_2level_tapbuf_size4_6_sram_blwl_out[170:170] ,mux_2level_tapbuf_size4_6_sram_blwl_outb[170:170] ,mux_2level_tapbuf_size4_6_configbus0[170:170], mux_2level_tapbuf_size4_6_configbus1[170:170] , mux_2level_tapbuf_size4_6_configbus0_b[170:170] );
sram6T_blwl sram_blwl_171_ (mux_2level_tapbuf_size4_6_sram_blwl_out[171:171] ,mux_2level_tapbuf_size4_6_sram_blwl_out[171:171] ,mux_2level_tapbuf_size4_6_sram_blwl_outb[171:171] ,mux_2level_tapbuf_size4_6_configbus0[171:171], mux_2level_tapbuf_size4_6_configbus1[171:171] , mux_2level_tapbuf_size4_6_configbus0_b[171:171] );
wire [0:3] mux_2level_tapbuf_size4_7_inbus;
assign mux_2level_tapbuf_size4_7_inbus[0] = chanx_1__0__midout_10_;
assign mux_2level_tapbuf_size4_7_inbus[1] = chanx_1__0__midout_11_;
assign mux_2level_tapbuf_size4_7_inbus[2] = chanx_1__0__midout_26_;
assign mux_2level_tapbuf_size4_7_inbus[3] = chanx_1__0__midout_27_;
wire [172:175] mux_2level_tapbuf_size4_7_configbus0;
wire [172:175] mux_2level_tapbuf_size4_7_configbus1;
wire [172:175] mux_2level_tapbuf_size4_7_sram_blwl_out ;
wire [172:175] mux_2level_tapbuf_size4_7_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_7_configbus0[172:175] = sram_blwl_bl[172:175] ;
assign mux_2level_tapbuf_size4_7_configbus1[172:175] = sram_blwl_wl[172:175] ;
wire [172:175] mux_2level_tapbuf_size4_7_configbus0_b;
assign mux_2level_tapbuf_size4_7_configbus0_b[172:175] = sram_blwl_blb[172:175] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_7_ (mux_2level_tapbuf_size4_7_inbus, grid_1__0__pin_0__0__12_, mux_2level_tapbuf_size4_7_sram_blwl_out[172:175] ,
mux_2level_tapbuf_size4_7_sram_blwl_outb[172:175] );
//----- SRAM bits for MUX[7], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_172_ (mux_2level_tapbuf_size4_7_sram_blwl_out[172:172] ,mux_2level_tapbuf_size4_7_sram_blwl_out[172:172] ,mux_2level_tapbuf_size4_7_sram_blwl_outb[172:172] ,mux_2level_tapbuf_size4_7_configbus0[172:172], mux_2level_tapbuf_size4_7_configbus1[172:172] , mux_2level_tapbuf_size4_7_configbus0_b[172:172] );
sram6T_blwl sram_blwl_173_ (mux_2level_tapbuf_size4_7_sram_blwl_out[173:173] ,mux_2level_tapbuf_size4_7_sram_blwl_out[173:173] ,mux_2level_tapbuf_size4_7_sram_blwl_outb[173:173] ,mux_2level_tapbuf_size4_7_configbus0[173:173], mux_2level_tapbuf_size4_7_configbus1[173:173] , mux_2level_tapbuf_size4_7_configbus0_b[173:173] );
sram6T_blwl sram_blwl_174_ (mux_2level_tapbuf_size4_7_sram_blwl_out[174:174] ,mux_2level_tapbuf_size4_7_sram_blwl_out[174:174] ,mux_2level_tapbuf_size4_7_sram_blwl_outb[174:174] ,mux_2level_tapbuf_size4_7_configbus0[174:174], mux_2level_tapbuf_size4_7_configbus1[174:174] , mux_2level_tapbuf_size4_7_configbus0_b[174:174] );
sram6T_blwl sram_blwl_175_ (mux_2level_tapbuf_size4_7_sram_blwl_out[175:175] ,mux_2level_tapbuf_size4_7_sram_blwl_out[175:175] ,mux_2level_tapbuf_size4_7_sram_blwl_outb[175:175] ,mux_2level_tapbuf_size4_7_configbus0[175:175], mux_2level_tapbuf_size4_7_configbus1[175:175] , mux_2level_tapbuf_size4_7_configbus0_b[175:175] );
wire [0:3] mux_2level_tapbuf_size4_8_inbus;
assign mux_2level_tapbuf_size4_8_inbus[0] = chanx_1__0__midout_12_;
assign mux_2level_tapbuf_size4_8_inbus[1] = chanx_1__0__midout_13_;
assign mux_2level_tapbuf_size4_8_inbus[2] = chanx_1__0__midout_28_;
assign mux_2level_tapbuf_size4_8_inbus[3] = chanx_1__0__midout_29_;
wire [176:179] mux_2level_tapbuf_size4_8_configbus0;
wire [176:179] mux_2level_tapbuf_size4_8_configbus1;
wire [176:179] mux_2level_tapbuf_size4_8_sram_blwl_out ;
wire [176:179] mux_2level_tapbuf_size4_8_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_8_configbus0[176:179] = sram_blwl_bl[176:179] ;
assign mux_2level_tapbuf_size4_8_configbus1[176:179] = sram_blwl_wl[176:179] ;
wire [176:179] mux_2level_tapbuf_size4_8_configbus0_b;
assign mux_2level_tapbuf_size4_8_configbus0_b[176:179] = sram_blwl_blb[176:179] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_8_ (mux_2level_tapbuf_size4_8_inbus, grid_1__0__pin_0__0__14_, mux_2level_tapbuf_size4_8_sram_blwl_out[176:179] ,
mux_2level_tapbuf_size4_8_sram_blwl_outb[176:179] );
//----- SRAM bits for MUX[8], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_176_ (mux_2level_tapbuf_size4_8_sram_blwl_out[176:176] ,mux_2level_tapbuf_size4_8_sram_blwl_out[176:176] ,mux_2level_tapbuf_size4_8_sram_blwl_outb[176:176] ,mux_2level_tapbuf_size4_8_configbus0[176:176], mux_2level_tapbuf_size4_8_configbus1[176:176] , mux_2level_tapbuf_size4_8_configbus0_b[176:176] );
sram6T_blwl sram_blwl_177_ (mux_2level_tapbuf_size4_8_sram_blwl_out[177:177] ,mux_2level_tapbuf_size4_8_sram_blwl_out[177:177] ,mux_2level_tapbuf_size4_8_sram_blwl_outb[177:177] ,mux_2level_tapbuf_size4_8_configbus0[177:177], mux_2level_tapbuf_size4_8_configbus1[177:177] , mux_2level_tapbuf_size4_8_configbus0_b[177:177] );
sram6T_blwl sram_blwl_178_ (mux_2level_tapbuf_size4_8_sram_blwl_out[178:178] ,mux_2level_tapbuf_size4_8_sram_blwl_out[178:178] ,mux_2level_tapbuf_size4_8_sram_blwl_outb[178:178] ,mux_2level_tapbuf_size4_8_configbus0[178:178], mux_2level_tapbuf_size4_8_configbus1[178:178] , mux_2level_tapbuf_size4_8_configbus0_b[178:178] );
sram6T_blwl sram_blwl_179_ (mux_2level_tapbuf_size4_8_sram_blwl_out[179:179] ,mux_2level_tapbuf_size4_8_sram_blwl_out[179:179] ,mux_2level_tapbuf_size4_8_sram_blwl_outb[179:179] ,mux_2level_tapbuf_size4_8_configbus0[179:179], mux_2level_tapbuf_size4_8_configbus1[179:179] , mux_2level_tapbuf_size4_8_configbus0_b[179:179] );
endmodule
//----- END Verilog Module of Connection Box -X direction [1][0] -----

