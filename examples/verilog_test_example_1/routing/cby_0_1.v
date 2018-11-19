//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Connection Block - Y direction  [0][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
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

output  grid_1__1__pin_0__3__3_,

output  grid_0__1__pin_0__1__0_,

output  grid_0__1__pin_0__1__2_,

output  grid_0__1__pin_0__1__4_,

output  grid_0__1__pin_0__1__6_,

output  grid_0__1__pin_0__1__8_,

output  grid_0__1__pin_0__1__10_,

output  grid_0__1__pin_0__1__12_,

output  grid_0__1__pin_0__1__14_,

input [216:251] sram_blwl_bl ,
input [216:251] sram_blwl_wl ,
input [216:251] sram_blwl_blb );
wire [0:3] mux_2level_tapbuf_size4_18_inbus;
assign mux_2level_tapbuf_size4_18_inbus[0] = chany_0__1__midout_10_;
assign mux_2level_tapbuf_size4_18_inbus[1] = chany_0__1__midout_11_;
assign mux_2level_tapbuf_size4_18_inbus[2] = chany_0__1__midout_26_;
assign mux_2level_tapbuf_size4_18_inbus[3] = chany_0__1__midout_27_;
wire [216:219] mux_2level_tapbuf_size4_18_configbus0;
wire [216:219] mux_2level_tapbuf_size4_18_configbus1;
wire [216:219] mux_2level_tapbuf_size4_18_sram_blwl_out ;
wire [216:219] mux_2level_tapbuf_size4_18_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_18_configbus0[216:219] = sram_blwl_bl[216:219] ;
assign mux_2level_tapbuf_size4_18_configbus1[216:219] = sram_blwl_wl[216:219] ;
wire [216:219] mux_2level_tapbuf_size4_18_configbus0_b;
assign mux_2level_tapbuf_size4_18_configbus0_b[216:219] = sram_blwl_blb[216:219] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_18_ (mux_2level_tapbuf_size4_18_inbus, grid_1__1__pin_0__3__3_, mux_2level_tapbuf_size4_18_sram_blwl_out[216:219] ,
mux_2level_tapbuf_size4_18_sram_blwl_outb[216:219] );
//----- SRAM bits for MUX[18], level=2, select_path_id=3. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----0101-----
sram6T_blwl sram_blwl_216_ (mux_2level_tapbuf_size4_18_sram_blwl_out[216:216] ,mux_2level_tapbuf_size4_18_sram_blwl_out[216:216] ,mux_2level_tapbuf_size4_18_sram_blwl_outb[216:216] ,mux_2level_tapbuf_size4_18_configbus0[216:216], mux_2level_tapbuf_size4_18_configbus1[216:216] , mux_2level_tapbuf_size4_18_configbus0_b[216:216] );
sram6T_blwl sram_blwl_217_ (mux_2level_tapbuf_size4_18_sram_blwl_out[217:217] ,mux_2level_tapbuf_size4_18_sram_blwl_out[217:217] ,mux_2level_tapbuf_size4_18_sram_blwl_outb[217:217] ,mux_2level_tapbuf_size4_18_configbus0[217:217], mux_2level_tapbuf_size4_18_configbus1[217:217] , mux_2level_tapbuf_size4_18_configbus0_b[217:217] );
sram6T_blwl sram_blwl_218_ (mux_2level_tapbuf_size4_18_sram_blwl_out[218:218] ,mux_2level_tapbuf_size4_18_sram_blwl_out[218:218] ,mux_2level_tapbuf_size4_18_sram_blwl_outb[218:218] ,mux_2level_tapbuf_size4_18_configbus0[218:218], mux_2level_tapbuf_size4_18_configbus1[218:218] , mux_2level_tapbuf_size4_18_configbus0_b[218:218] );
sram6T_blwl sram_blwl_219_ (mux_2level_tapbuf_size4_18_sram_blwl_out[219:219] ,mux_2level_tapbuf_size4_18_sram_blwl_out[219:219] ,mux_2level_tapbuf_size4_18_sram_blwl_outb[219:219] ,mux_2level_tapbuf_size4_18_configbus0[219:219], mux_2level_tapbuf_size4_18_configbus1[219:219] , mux_2level_tapbuf_size4_18_configbus0_b[219:219] );
wire [0:3] mux_2level_tapbuf_size4_19_inbus;
assign mux_2level_tapbuf_size4_19_inbus[0] = chany_0__1__midout_0_;
assign mux_2level_tapbuf_size4_19_inbus[1] = chany_0__1__midout_1_;
assign mux_2level_tapbuf_size4_19_inbus[2] = chany_0__1__midout_14_;
assign mux_2level_tapbuf_size4_19_inbus[3] = chany_0__1__midout_15_;
wire [220:223] mux_2level_tapbuf_size4_19_configbus0;
wire [220:223] mux_2level_tapbuf_size4_19_configbus1;
wire [220:223] mux_2level_tapbuf_size4_19_sram_blwl_out ;
wire [220:223] mux_2level_tapbuf_size4_19_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_19_configbus0[220:223] = sram_blwl_bl[220:223] ;
assign mux_2level_tapbuf_size4_19_configbus1[220:223] = sram_blwl_wl[220:223] ;
wire [220:223] mux_2level_tapbuf_size4_19_configbus0_b;
assign mux_2level_tapbuf_size4_19_configbus0_b[220:223] = sram_blwl_blb[220:223] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_19_ (mux_2level_tapbuf_size4_19_inbus, grid_0__1__pin_0__1__0_, mux_2level_tapbuf_size4_19_sram_blwl_out[220:223] ,
mux_2level_tapbuf_size4_19_sram_blwl_outb[220:223] );
//----- SRAM bits for MUX[19], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_220_ (mux_2level_tapbuf_size4_19_sram_blwl_out[220:220] ,mux_2level_tapbuf_size4_19_sram_blwl_out[220:220] ,mux_2level_tapbuf_size4_19_sram_blwl_outb[220:220] ,mux_2level_tapbuf_size4_19_configbus0[220:220], mux_2level_tapbuf_size4_19_configbus1[220:220] , mux_2level_tapbuf_size4_19_configbus0_b[220:220] );
sram6T_blwl sram_blwl_221_ (mux_2level_tapbuf_size4_19_sram_blwl_out[221:221] ,mux_2level_tapbuf_size4_19_sram_blwl_out[221:221] ,mux_2level_tapbuf_size4_19_sram_blwl_outb[221:221] ,mux_2level_tapbuf_size4_19_configbus0[221:221], mux_2level_tapbuf_size4_19_configbus1[221:221] , mux_2level_tapbuf_size4_19_configbus0_b[221:221] );
sram6T_blwl sram_blwl_222_ (mux_2level_tapbuf_size4_19_sram_blwl_out[222:222] ,mux_2level_tapbuf_size4_19_sram_blwl_out[222:222] ,mux_2level_tapbuf_size4_19_sram_blwl_outb[222:222] ,mux_2level_tapbuf_size4_19_configbus0[222:222], mux_2level_tapbuf_size4_19_configbus1[222:222] , mux_2level_tapbuf_size4_19_configbus0_b[222:222] );
sram6T_blwl sram_blwl_223_ (mux_2level_tapbuf_size4_19_sram_blwl_out[223:223] ,mux_2level_tapbuf_size4_19_sram_blwl_out[223:223] ,mux_2level_tapbuf_size4_19_sram_blwl_outb[223:223] ,mux_2level_tapbuf_size4_19_configbus0[223:223], mux_2level_tapbuf_size4_19_configbus1[223:223] , mux_2level_tapbuf_size4_19_configbus0_b[223:223] );
wire [0:3] mux_2level_tapbuf_size4_20_inbus;
assign mux_2level_tapbuf_size4_20_inbus[0] = chany_0__1__midout_2_;
assign mux_2level_tapbuf_size4_20_inbus[1] = chany_0__1__midout_3_;
assign mux_2level_tapbuf_size4_20_inbus[2] = chany_0__1__midout_16_;
assign mux_2level_tapbuf_size4_20_inbus[3] = chany_0__1__midout_17_;
wire [224:227] mux_2level_tapbuf_size4_20_configbus0;
wire [224:227] mux_2level_tapbuf_size4_20_configbus1;
wire [224:227] mux_2level_tapbuf_size4_20_sram_blwl_out ;
wire [224:227] mux_2level_tapbuf_size4_20_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_20_configbus0[224:227] = sram_blwl_bl[224:227] ;
assign mux_2level_tapbuf_size4_20_configbus1[224:227] = sram_blwl_wl[224:227] ;
wire [224:227] mux_2level_tapbuf_size4_20_configbus0_b;
assign mux_2level_tapbuf_size4_20_configbus0_b[224:227] = sram_blwl_blb[224:227] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_20_ (mux_2level_tapbuf_size4_20_inbus, grid_0__1__pin_0__1__2_, mux_2level_tapbuf_size4_20_sram_blwl_out[224:227] ,
mux_2level_tapbuf_size4_20_sram_blwl_outb[224:227] );
//----- SRAM bits for MUX[20], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_224_ (mux_2level_tapbuf_size4_20_sram_blwl_out[224:224] ,mux_2level_tapbuf_size4_20_sram_blwl_out[224:224] ,mux_2level_tapbuf_size4_20_sram_blwl_outb[224:224] ,mux_2level_tapbuf_size4_20_configbus0[224:224], mux_2level_tapbuf_size4_20_configbus1[224:224] , mux_2level_tapbuf_size4_20_configbus0_b[224:224] );
sram6T_blwl sram_blwl_225_ (mux_2level_tapbuf_size4_20_sram_blwl_out[225:225] ,mux_2level_tapbuf_size4_20_sram_blwl_out[225:225] ,mux_2level_tapbuf_size4_20_sram_blwl_outb[225:225] ,mux_2level_tapbuf_size4_20_configbus0[225:225], mux_2level_tapbuf_size4_20_configbus1[225:225] , mux_2level_tapbuf_size4_20_configbus0_b[225:225] );
sram6T_blwl sram_blwl_226_ (mux_2level_tapbuf_size4_20_sram_blwl_out[226:226] ,mux_2level_tapbuf_size4_20_sram_blwl_out[226:226] ,mux_2level_tapbuf_size4_20_sram_blwl_outb[226:226] ,mux_2level_tapbuf_size4_20_configbus0[226:226], mux_2level_tapbuf_size4_20_configbus1[226:226] , mux_2level_tapbuf_size4_20_configbus0_b[226:226] );
sram6T_blwl sram_blwl_227_ (mux_2level_tapbuf_size4_20_sram_blwl_out[227:227] ,mux_2level_tapbuf_size4_20_sram_blwl_out[227:227] ,mux_2level_tapbuf_size4_20_sram_blwl_outb[227:227] ,mux_2level_tapbuf_size4_20_configbus0[227:227], mux_2level_tapbuf_size4_20_configbus1[227:227] , mux_2level_tapbuf_size4_20_configbus0_b[227:227] );
wire [0:3] mux_2level_tapbuf_size4_21_inbus;
assign mux_2level_tapbuf_size4_21_inbus[0] = chany_0__1__midout_4_;
assign mux_2level_tapbuf_size4_21_inbus[1] = chany_0__1__midout_5_;
assign mux_2level_tapbuf_size4_21_inbus[2] = chany_0__1__midout_18_;
assign mux_2level_tapbuf_size4_21_inbus[3] = chany_0__1__midout_19_;
wire [228:231] mux_2level_tapbuf_size4_21_configbus0;
wire [228:231] mux_2level_tapbuf_size4_21_configbus1;
wire [228:231] mux_2level_tapbuf_size4_21_sram_blwl_out ;
wire [228:231] mux_2level_tapbuf_size4_21_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_21_configbus0[228:231] = sram_blwl_bl[228:231] ;
assign mux_2level_tapbuf_size4_21_configbus1[228:231] = sram_blwl_wl[228:231] ;
wire [228:231] mux_2level_tapbuf_size4_21_configbus0_b;
assign mux_2level_tapbuf_size4_21_configbus0_b[228:231] = sram_blwl_blb[228:231] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_21_ (mux_2level_tapbuf_size4_21_inbus, grid_0__1__pin_0__1__4_, mux_2level_tapbuf_size4_21_sram_blwl_out[228:231] ,
mux_2level_tapbuf_size4_21_sram_blwl_outb[228:231] );
//----- SRAM bits for MUX[21], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_228_ (mux_2level_tapbuf_size4_21_sram_blwl_out[228:228] ,mux_2level_tapbuf_size4_21_sram_blwl_out[228:228] ,mux_2level_tapbuf_size4_21_sram_blwl_outb[228:228] ,mux_2level_tapbuf_size4_21_configbus0[228:228], mux_2level_tapbuf_size4_21_configbus1[228:228] , mux_2level_tapbuf_size4_21_configbus0_b[228:228] );
sram6T_blwl sram_blwl_229_ (mux_2level_tapbuf_size4_21_sram_blwl_out[229:229] ,mux_2level_tapbuf_size4_21_sram_blwl_out[229:229] ,mux_2level_tapbuf_size4_21_sram_blwl_outb[229:229] ,mux_2level_tapbuf_size4_21_configbus0[229:229], mux_2level_tapbuf_size4_21_configbus1[229:229] , mux_2level_tapbuf_size4_21_configbus0_b[229:229] );
sram6T_blwl sram_blwl_230_ (mux_2level_tapbuf_size4_21_sram_blwl_out[230:230] ,mux_2level_tapbuf_size4_21_sram_blwl_out[230:230] ,mux_2level_tapbuf_size4_21_sram_blwl_outb[230:230] ,mux_2level_tapbuf_size4_21_configbus0[230:230], mux_2level_tapbuf_size4_21_configbus1[230:230] , mux_2level_tapbuf_size4_21_configbus0_b[230:230] );
sram6T_blwl sram_blwl_231_ (mux_2level_tapbuf_size4_21_sram_blwl_out[231:231] ,mux_2level_tapbuf_size4_21_sram_blwl_out[231:231] ,mux_2level_tapbuf_size4_21_sram_blwl_outb[231:231] ,mux_2level_tapbuf_size4_21_configbus0[231:231], mux_2level_tapbuf_size4_21_configbus1[231:231] , mux_2level_tapbuf_size4_21_configbus0_b[231:231] );
wire [0:3] mux_2level_tapbuf_size4_22_inbus;
assign mux_2level_tapbuf_size4_22_inbus[0] = chany_0__1__midout_6_;
assign mux_2level_tapbuf_size4_22_inbus[1] = chany_0__1__midout_7_;
assign mux_2level_tapbuf_size4_22_inbus[2] = chany_0__1__midout_20_;
assign mux_2level_tapbuf_size4_22_inbus[3] = chany_0__1__midout_21_;
wire [232:235] mux_2level_tapbuf_size4_22_configbus0;
wire [232:235] mux_2level_tapbuf_size4_22_configbus1;
wire [232:235] mux_2level_tapbuf_size4_22_sram_blwl_out ;
wire [232:235] mux_2level_tapbuf_size4_22_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_22_configbus0[232:235] = sram_blwl_bl[232:235] ;
assign mux_2level_tapbuf_size4_22_configbus1[232:235] = sram_blwl_wl[232:235] ;
wire [232:235] mux_2level_tapbuf_size4_22_configbus0_b;
assign mux_2level_tapbuf_size4_22_configbus0_b[232:235] = sram_blwl_blb[232:235] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_22_ (mux_2level_tapbuf_size4_22_inbus, grid_0__1__pin_0__1__6_, mux_2level_tapbuf_size4_22_sram_blwl_out[232:235] ,
mux_2level_tapbuf_size4_22_sram_blwl_outb[232:235] );
//----- SRAM bits for MUX[22], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_232_ (mux_2level_tapbuf_size4_22_sram_blwl_out[232:232] ,mux_2level_tapbuf_size4_22_sram_blwl_out[232:232] ,mux_2level_tapbuf_size4_22_sram_blwl_outb[232:232] ,mux_2level_tapbuf_size4_22_configbus0[232:232], mux_2level_tapbuf_size4_22_configbus1[232:232] , mux_2level_tapbuf_size4_22_configbus0_b[232:232] );
sram6T_blwl sram_blwl_233_ (mux_2level_tapbuf_size4_22_sram_blwl_out[233:233] ,mux_2level_tapbuf_size4_22_sram_blwl_out[233:233] ,mux_2level_tapbuf_size4_22_sram_blwl_outb[233:233] ,mux_2level_tapbuf_size4_22_configbus0[233:233], mux_2level_tapbuf_size4_22_configbus1[233:233] , mux_2level_tapbuf_size4_22_configbus0_b[233:233] );
sram6T_blwl sram_blwl_234_ (mux_2level_tapbuf_size4_22_sram_blwl_out[234:234] ,mux_2level_tapbuf_size4_22_sram_blwl_out[234:234] ,mux_2level_tapbuf_size4_22_sram_blwl_outb[234:234] ,mux_2level_tapbuf_size4_22_configbus0[234:234], mux_2level_tapbuf_size4_22_configbus1[234:234] , mux_2level_tapbuf_size4_22_configbus0_b[234:234] );
sram6T_blwl sram_blwl_235_ (mux_2level_tapbuf_size4_22_sram_blwl_out[235:235] ,mux_2level_tapbuf_size4_22_sram_blwl_out[235:235] ,mux_2level_tapbuf_size4_22_sram_blwl_outb[235:235] ,mux_2level_tapbuf_size4_22_configbus0[235:235], mux_2level_tapbuf_size4_22_configbus1[235:235] , mux_2level_tapbuf_size4_22_configbus0_b[235:235] );
wire [0:3] mux_2level_tapbuf_size4_23_inbus;
assign mux_2level_tapbuf_size4_23_inbus[0] = chany_0__1__midout_6_;
assign mux_2level_tapbuf_size4_23_inbus[1] = chany_0__1__midout_7_;
assign mux_2level_tapbuf_size4_23_inbus[2] = chany_0__1__midout_22_;
assign mux_2level_tapbuf_size4_23_inbus[3] = chany_0__1__midout_23_;
wire [236:239] mux_2level_tapbuf_size4_23_configbus0;
wire [236:239] mux_2level_tapbuf_size4_23_configbus1;
wire [236:239] mux_2level_tapbuf_size4_23_sram_blwl_out ;
wire [236:239] mux_2level_tapbuf_size4_23_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_23_configbus0[236:239] = sram_blwl_bl[236:239] ;
assign mux_2level_tapbuf_size4_23_configbus1[236:239] = sram_blwl_wl[236:239] ;
wire [236:239] mux_2level_tapbuf_size4_23_configbus0_b;
assign mux_2level_tapbuf_size4_23_configbus0_b[236:239] = sram_blwl_blb[236:239] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_23_ (mux_2level_tapbuf_size4_23_inbus, grid_0__1__pin_0__1__8_, mux_2level_tapbuf_size4_23_sram_blwl_out[236:239] ,
mux_2level_tapbuf_size4_23_sram_blwl_outb[236:239] );
//----- SRAM bits for MUX[23], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_236_ (mux_2level_tapbuf_size4_23_sram_blwl_out[236:236] ,mux_2level_tapbuf_size4_23_sram_blwl_out[236:236] ,mux_2level_tapbuf_size4_23_sram_blwl_outb[236:236] ,mux_2level_tapbuf_size4_23_configbus0[236:236], mux_2level_tapbuf_size4_23_configbus1[236:236] , mux_2level_tapbuf_size4_23_configbus0_b[236:236] );
sram6T_blwl sram_blwl_237_ (mux_2level_tapbuf_size4_23_sram_blwl_out[237:237] ,mux_2level_tapbuf_size4_23_sram_blwl_out[237:237] ,mux_2level_tapbuf_size4_23_sram_blwl_outb[237:237] ,mux_2level_tapbuf_size4_23_configbus0[237:237], mux_2level_tapbuf_size4_23_configbus1[237:237] , mux_2level_tapbuf_size4_23_configbus0_b[237:237] );
sram6T_blwl sram_blwl_238_ (mux_2level_tapbuf_size4_23_sram_blwl_out[238:238] ,mux_2level_tapbuf_size4_23_sram_blwl_out[238:238] ,mux_2level_tapbuf_size4_23_sram_blwl_outb[238:238] ,mux_2level_tapbuf_size4_23_configbus0[238:238], mux_2level_tapbuf_size4_23_configbus1[238:238] , mux_2level_tapbuf_size4_23_configbus0_b[238:238] );
sram6T_blwl sram_blwl_239_ (mux_2level_tapbuf_size4_23_sram_blwl_out[239:239] ,mux_2level_tapbuf_size4_23_sram_blwl_out[239:239] ,mux_2level_tapbuf_size4_23_sram_blwl_outb[239:239] ,mux_2level_tapbuf_size4_23_configbus0[239:239], mux_2level_tapbuf_size4_23_configbus1[239:239] , mux_2level_tapbuf_size4_23_configbus0_b[239:239] );
wire [0:3] mux_2level_tapbuf_size4_24_inbus;
assign mux_2level_tapbuf_size4_24_inbus[0] = chany_0__1__midout_8_;
assign mux_2level_tapbuf_size4_24_inbus[1] = chany_0__1__midout_9_;
assign mux_2level_tapbuf_size4_24_inbus[2] = chany_0__1__midout_24_;
assign mux_2level_tapbuf_size4_24_inbus[3] = chany_0__1__midout_25_;
wire [240:243] mux_2level_tapbuf_size4_24_configbus0;
wire [240:243] mux_2level_tapbuf_size4_24_configbus1;
wire [240:243] mux_2level_tapbuf_size4_24_sram_blwl_out ;
wire [240:243] mux_2level_tapbuf_size4_24_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_24_configbus0[240:243] = sram_blwl_bl[240:243] ;
assign mux_2level_tapbuf_size4_24_configbus1[240:243] = sram_blwl_wl[240:243] ;
wire [240:243] mux_2level_tapbuf_size4_24_configbus0_b;
assign mux_2level_tapbuf_size4_24_configbus0_b[240:243] = sram_blwl_blb[240:243] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_24_ (mux_2level_tapbuf_size4_24_inbus, grid_0__1__pin_0__1__10_, mux_2level_tapbuf_size4_24_sram_blwl_out[240:243] ,
mux_2level_tapbuf_size4_24_sram_blwl_outb[240:243] );
//----- SRAM bits for MUX[24], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_240_ (mux_2level_tapbuf_size4_24_sram_blwl_out[240:240] ,mux_2level_tapbuf_size4_24_sram_blwl_out[240:240] ,mux_2level_tapbuf_size4_24_sram_blwl_outb[240:240] ,mux_2level_tapbuf_size4_24_configbus0[240:240], mux_2level_tapbuf_size4_24_configbus1[240:240] , mux_2level_tapbuf_size4_24_configbus0_b[240:240] );
sram6T_blwl sram_blwl_241_ (mux_2level_tapbuf_size4_24_sram_blwl_out[241:241] ,mux_2level_tapbuf_size4_24_sram_blwl_out[241:241] ,mux_2level_tapbuf_size4_24_sram_blwl_outb[241:241] ,mux_2level_tapbuf_size4_24_configbus0[241:241], mux_2level_tapbuf_size4_24_configbus1[241:241] , mux_2level_tapbuf_size4_24_configbus0_b[241:241] );
sram6T_blwl sram_blwl_242_ (mux_2level_tapbuf_size4_24_sram_blwl_out[242:242] ,mux_2level_tapbuf_size4_24_sram_blwl_out[242:242] ,mux_2level_tapbuf_size4_24_sram_blwl_outb[242:242] ,mux_2level_tapbuf_size4_24_configbus0[242:242], mux_2level_tapbuf_size4_24_configbus1[242:242] , mux_2level_tapbuf_size4_24_configbus0_b[242:242] );
sram6T_blwl sram_blwl_243_ (mux_2level_tapbuf_size4_24_sram_blwl_out[243:243] ,mux_2level_tapbuf_size4_24_sram_blwl_out[243:243] ,mux_2level_tapbuf_size4_24_sram_blwl_outb[243:243] ,mux_2level_tapbuf_size4_24_configbus0[243:243], mux_2level_tapbuf_size4_24_configbus1[243:243] , mux_2level_tapbuf_size4_24_configbus0_b[243:243] );
wire [0:3] mux_2level_tapbuf_size4_25_inbus;
assign mux_2level_tapbuf_size4_25_inbus[0] = chany_0__1__midout_10_;
assign mux_2level_tapbuf_size4_25_inbus[1] = chany_0__1__midout_11_;
assign mux_2level_tapbuf_size4_25_inbus[2] = chany_0__1__midout_26_;
assign mux_2level_tapbuf_size4_25_inbus[3] = chany_0__1__midout_27_;
wire [244:247] mux_2level_tapbuf_size4_25_configbus0;
wire [244:247] mux_2level_tapbuf_size4_25_configbus1;
wire [244:247] mux_2level_tapbuf_size4_25_sram_blwl_out ;
wire [244:247] mux_2level_tapbuf_size4_25_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_25_configbus0[244:247] = sram_blwl_bl[244:247] ;
assign mux_2level_tapbuf_size4_25_configbus1[244:247] = sram_blwl_wl[244:247] ;
wire [244:247] mux_2level_tapbuf_size4_25_configbus0_b;
assign mux_2level_tapbuf_size4_25_configbus0_b[244:247] = sram_blwl_blb[244:247] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_25_ (mux_2level_tapbuf_size4_25_inbus, grid_0__1__pin_0__1__12_, mux_2level_tapbuf_size4_25_sram_blwl_out[244:247] ,
mux_2level_tapbuf_size4_25_sram_blwl_outb[244:247] );
//----- SRAM bits for MUX[25], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_244_ (mux_2level_tapbuf_size4_25_sram_blwl_out[244:244] ,mux_2level_tapbuf_size4_25_sram_blwl_out[244:244] ,mux_2level_tapbuf_size4_25_sram_blwl_outb[244:244] ,mux_2level_tapbuf_size4_25_configbus0[244:244], mux_2level_tapbuf_size4_25_configbus1[244:244] , mux_2level_tapbuf_size4_25_configbus0_b[244:244] );
sram6T_blwl sram_blwl_245_ (mux_2level_tapbuf_size4_25_sram_blwl_out[245:245] ,mux_2level_tapbuf_size4_25_sram_blwl_out[245:245] ,mux_2level_tapbuf_size4_25_sram_blwl_outb[245:245] ,mux_2level_tapbuf_size4_25_configbus0[245:245], mux_2level_tapbuf_size4_25_configbus1[245:245] , mux_2level_tapbuf_size4_25_configbus0_b[245:245] );
sram6T_blwl sram_blwl_246_ (mux_2level_tapbuf_size4_25_sram_blwl_out[246:246] ,mux_2level_tapbuf_size4_25_sram_blwl_out[246:246] ,mux_2level_tapbuf_size4_25_sram_blwl_outb[246:246] ,mux_2level_tapbuf_size4_25_configbus0[246:246], mux_2level_tapbuf_size4_25_configbus1[246:246] , mux_2level_tapbuf_size4_25_configbus0_b[246:246] );
sram6T_blwl sram_blwl_247_ (mux_2level_tapbuf_size4_25_sram_blwl_out[247:247] ,mux_2level_tapbuf_size4_25_sram_blwl_out[247:247] ,mux_2level_tapbuf_size4_25_sram_blwl_outb[247:247] ,mux_2level_tapbuf_size4_25_configbus0[247:247], mux_2level_tapbuf_size4_25_configbus1[247:247] , mux_2level_tapbuf_size4_25_configbus0_b[247:247] );
wire [0:3] mux_2level_tapbuf_size4_26_inbus;
assign mux_2level_tapbuf_size4_26_inbus[0] = chany_0__1__midout_12_;
assign mux_2level_tapbuf_size4_26_inbus[1] = chany_0__1__midout_13_;
assign mux_2level_tapbuf_size4_26_inbus[2] = chany_0__1__midout_28_;
assign mux_2level_tapbuf_size4_26_inbus[3] = chany_0__1__midout_29_;
wire [248:251] mux_2level_tapbuf_size4_26_configbus0;
wire [248:251] mux_2level_tapbuf_size4_26_configbus1;
wire [248:251] mux_2level_tapbuf_size4_26_sram_blwl_out ;
wire [248:251] mux_2level_tapbuf_size4_26_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_26_configbus0[248:251] = sram_blwl_bl[248:251] ;
assign mux_2level_tapbuf_size4_26_configbus1[248:251] = sram_blwl_wl[248:251] ;
wire [248:251] mux_2level_tapbuf_size4_26_configbus0_b;
assign mux_2level_tapbuf_size4_26_configbus0_b[248:251] = sram_blwl_blb[248:251] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_26_ (mux_2level_tapbuf_size4_26_inbus, grid_0__1__pin_0__1__14_, mux_2level_tapbuf_size4_26_sram_blwl_out[248:251] ,
mux_2level_tapbuf_size4_26_sram_blwl_outb[248:251] );
//----- SRAM bits for MUX[26], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_248_ (mux_2level_tapbuf_size4_26_sram_blwl_out[248:248] ,mux_2level_tapbuf_size4_26_sram_blwl_out[248:248] ,mux_2level_tapbuf_size4_26_sram_blwl_outb[248:248] ,mux_2level_tapbuf_size4_26_configbus0[248:248], mux_2level_tapbuf_size4_26_configbus1[248:248] , mux_2level_tapbuf_size4_26_configbus0_b[248:248] );
sram6T_blwl sram_blwl_249_ (mux_2level_tapbuf_size4_26_sram_blwl_out[249:249] ,mux_2level_tapbuf_size4_26_sram_blwl_out[249:249] ,mux_2level_tapbuf_size4_26_sram_blwl_outb[249:249] ,mux_2level_tapbuf_size4_26_configbus0[249:249], mux_2level_tapbuf_size4_26_configbus1[249:249] , mux_2level_tapbuf_size4_26_configbus0_b[249:249] );
sram6T_blwl sram_blwl_250_ (mux_2level_tapbuf_size4_26_sram_blwl_out[250:250] ,mux_2level_tapbuf_size4_26_sram_blwl_out[250:250] ,mux_2level_tapbuf_size4_26_sram_blwl_outb[250:250] ,mux_2level_tapbuf_size4_26_configbus0[250:250], mux_2level_tapbuf_size4_26_configbus1[250:250] , mux_2level_tapbuf_size4_26_configbus0_b[250:250] );
sram6T_blwl sram_blwl_251_ (mux_2level_tapbuf_size4_26_sram_blwl_out[251:251] ,mux_2level_tapbuf_size4_26_sram_blwl_out[251:251] ,mux_2level_tapbuf_size4_26_sram_blwl_outb[251:251] ,mux_2level_tapbuf_size4_26_configbus0[251:251], mux_2level_tapbuf_size4_26_configbus1[251:251] , mux_2level_tapbuf_size4_26_configbus0_b[251:251] );
endmodule
//----- END Verilog Module of Connection Box -Y direction [0][1] -----

