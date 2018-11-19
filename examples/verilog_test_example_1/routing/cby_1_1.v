//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Connection Block - Y direction  [1][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
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

input [252:287] sram_blwl_bl ,
input [252:287] sram_blwl_wl ,
input [252:287] sram_blwl_blb );
wire [0:3] mux_2level_tapbuf_size4_27_inbus;
assign mux_2level_tapbuf_size4_27_inbus[0] = chany_1__1__midout_6_;
assign mux_2level_tapbuf_size4_27_inbus[1] = chany_1__1__midout_7_;
assign mux_2level_tapbuf_size4_27_inbus[2] = chany_1__1__midout_18_;
assign mux_2level_tapbuf_size4_27_inbus[3] = chany_1__1__midout_19_;
wire [252:255] mux_2level_tapbuf_size4_27_configbus0;
wire [252:255] mux_2level_tapbuf_size4_27_configbus1;
wire [252:255] mux_2level_tapbuf_size4_27_sram_blwl_out ;
wire [252:255] mux_2level_tapbuf_size4_27_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_27_configbus0[252:255] = sram_blwl_bl[252:255] ;
assign mux_2level_tapbuf_size4_27_configbus1[252:255] = sram_blwl_wl[252:255] ;
wire [252:255] mux_2level_tapbuf_size4_27_configbus0_b;
assign mux_2level_tapbuf_size4_27_configbus0_b[252:255] = sram_blwl_blb[252:255] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_27_ (mux_2level_tapbuf_size4_27_inbus, grid_2__1__pin_0__3__0_, mux_2level_tapbuf_size4_27_sram_blwl_out[252:255] ,
mux_2level_tapbuf_size4_27_sram_blwl_outb[252:255] );
//----- SRAM bits for MUX[27], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_252_ (mux_2level_tapbuf_size4_27_sram_blwl_out[252:252] ,mux_2level_tapbuf_size4_27_sram_blwl_out[252:252] ,mux_2level_tapbuf_size4_27_sram_blwl_outb[252:252] ,mux_2level_tapbuf_size4_27_configbus0[252:252], mux_2level_tapbuf_size4_27_configbus1[252:252] , mux_2level_tapbuf_size4_27_configbus0_b[252:252] );
sram6T_blwl sram_blwl_253_ (mux_2level_tapbuf_size4_27_sram_blwl_out[253:253] ,mux_2level_tapbuf_size4_27_sram_blwl_out[253:253] ,mux_2level_tapbuf_size4_27_sram_blwl_outb[253:253] ,mux_2level_tapbuf_size4_27_configbus0[253:253], mux_2level_tapbuf_size4_27_configbus1[253:253] , mux_2level_tapbuf_size4_27_configbus0_b[253:253] );
sram6T_blwl sram_blwl_254_ (mux_2level_tapbuf_size4_27_sram_blwl_out[254:254] ,mux_2level_tapbuf_size4_27_sram_blwl_out[254:254] ,mux_2level_tapbuf_size4_27_sram_blwl_outb[254:254] ,mux_2level_tapbuf_size4_27_configbus0[254:254], mux_2level_tapbuf_size4_27_configbus1[254:254] , mux_2level_tapbuf_size4_27_configbus0_b[254:254] );
sram6T_blwl sram_blwl_255_ (mux_2level_tapbuf_size4_27_sram_blwl_out[255:255] ,mux_2level_tapbuf_size4_27_sram_blwl_out[255:255] ,mux_2level_tapbuf_size4_27_sram_blwl_outb[255:255] ,mux_2level_tapbuf_size4_27_configbus0[255:255], mux_2level_tapbuf_size4_27_configbus1[255:255] , mux_2level_tapbuf_size4_27_configbus0_b[255:255] );
wire [0:3] mux_2level_tapbuf_size4_28_inbus;
assign mux_2level_tapbuf_size4_28_inbus[0] = chany_1__1__midout_0_;
assign mux_2level_tapbuf_size4_28_inbus[1] = chany_1__1__midout_1_;
assign mux_2level_tapbuf_size4_28_inbus[2] = chany_1__1__midout_16_;
assign mux_2level_tapbuf_size4_28_inbus[3] = chany_1__1__midout_17_;
wire [256:259] mux_2level_tapbuf_size4_28_configbus0;
wire [256:259] mux_2level_tapbuf_size4_28_configbus1;
wire [256:259] mux_2level_tapbuf_size4_28_sram_blwl_out ;
wire [256:259] mux_2level_tapbuf_size4_28_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_28_configbus0[256:259] = sram_blwl_bl[256:259] ;
assign mux_2level_tapbuf_size4_28_configbus1[256:259] = sram_blwl_wl[256:259] ;
wire [256:259] mux_2level_tapbuf_size4_28_configbus0_b;
assign mux_2level_tapbuf_size4_28_configbus0_b[256:259] = sram_blwl_blb[256:259] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_28_ (mux_2level_tapbuf_size4_28_inbus, grid_2__1__pin_0__3__2_, mux_2level_tapbuf_size4_28_sram_blwl_out[256:259] ,
mux_2level_tapbuf_size4_28_sram_blwl_outb[256:259] );
//----- SRAM bits for MUX[28], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_256_ (mux_2level_tapbuf_size4_28_sram_blwl_out[256:256] ,mux_2level_tapbuf_size4_28_sram_blwl_out[256:256] ,mux_2level_tapbuf_size4_28_sram_blwl_outb[256:256] ,mux_2level_tapbuf_size4_28_configbus0[256:256], mux_2level_tapbuf_size4_28_configbus1[256:256] , mux_2level_tapbuf_size4_28_configbus0_b[256:256] );
sram6T_blwl sram_blwl_257_ (mux_2level_tapbuf_size4_28_sram_blwl_out[257:257] ,mux_2level_tapbuf_size4_28_sram_blwl_out[257:257] ,mux_2level_tapbuf_size4_28_sram_blwl_outb[257:257] ,mux_2level_tapbuf_size4_28_configbus0[257:257], mux_2level_tapbuf_size4_28_configbus1[257:257] , mux_2level_tapbuf_size4_28_configbus0_b[257:257] );
sram6T_blwl sram_blwl_258_ (mux_2level_tapbuf_size4_28_sram_blwl_out[258:258] ,mux_2level_tapbuf_size4_28_sram_blwl_out[258:258] ,mux_2level_tapbuf_size4_28_sram_blwl_outb[258:258] ,mux_2level_tapbuf_size4_28_configbus0[258:258], mux_2level_tapbuf_size4_28_configbus1[258:258] , mux_2level_tapbuf_size4_28_configbus0_b[258:258] );
sram6T_blwl sram_blwl_259_ (mux_2level_tapbuf_size4_28_sram_blwl_out[259:259] ,mux_2level_tapbuf_size4_28_sram_blwl_out[259:259] ,mux_2level_tapbuf_size4_28_sram_blwl_outb[259:259] ,mux_2level_tapbuf_size4_28_configbus0[259:259], mux_2level_tapbuf_size4_28_configbus1[259:259] , mux_2level_tapbuf_size4_28_configbus0_b[259:259] );
wire [0:3] mux_2level_tapbuf_size4_29_inbus;
assign mux_2level_tapbuf_size4_29_inbus[0] = chany_1__1__midout_2_;
assign mux_2level_tapbuf_size4_29_inbus[1] = chany_1__1__midout_3_;
assign mux_2level_tapbuf_size4_29_inbus[2] = chany_1__1__midout_20_;
assign mux_2level_tapbuf_size4_29_inbus[3] = chany_1__1__midout_21_;
wire [260:263] mux_2level_tapbuf_size4_29_configbus0;
wire [260:263] mux_2level_tapbuf_size4_29_configbus1;
wire [260:263] mux_2level_tapbuf_size4_29_sram_blwl_out ;
wire [260:263] mux_2level_tapbuf_size4_29_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_29_configbus0[260:263] = sram_blwl_bl[260:263] ;
assign mux_2level_tapbuf_size4_29_configbus1[260:263] = sram_blwl_wl[260:263] ;
wire [260:263] mux_2level_tapbuf_size4_29_configbus0_b;
assign mux_2level_tapbuf_size4_29_configbus0_b[260:263] = sram_blwl_blb[260:263] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_29_ (mux_2level_tapbuf_size4_29_inbus, grid_2__1__pin_0__3__4_, mux_2level_tapbuf_size4_29_sram_blwl_out[260:263] ,
mux_2level_tapbuf_size4_29_sram_blwl_outb[260:263] );
//----- SRAM bits for MUX[29], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_260_ (mux_2level_tapbuf_size4_29_sram_blwl_out[260:260] ,mux_2level_tapbuf_size4_29_sram_blwl_out[260:260] ,mux_2level_tapbuf_size4_29_sram_blwl_outb[260:260] ,mux_2level_tapbuf_size4_29_configbus0[260:260], mux_2level_tapbuf_size4_29_configbus1[260:260] , mux_2level_tapbuf_size4_29_configbus0_b[260:260] );
sram6T_blwl sram_blwl_261_ (mux_2level_tapbuf_size4_29_sram_blwl_out[261:261] ,mux_2level_tapbuf_size4_29_sram_blwl_out[261:261] ,mux_2level_tapbuf_size4_29_sram_blwl_outb[261:261] ,mux_2level_tapbuf_size4_29_configbus0[261:261], mux_2level_tapbuf_size4_29_configbus1[261:261] , mux_2level_tapbuf_size4_29_configbus0_b[261:261] );
sram6T_blwl sram_blwl_262_ (mux_2level_tapbuf_size4_29_sram_blwl_out[262:262] ,mux_2level_tapbuf_size4_29_sram_blwl_out[262:262] ,mux_2level_tapbuf_size4_29_sram_blwl_outb[262:262] ,mux_2level_tapbuf_size4_29_configbus0[262:262], mux_2level_tapbuf_size4_29_configbus1[262:262] , mux_2level_tapbuf_size4_29_configbus0_b[262:262] );
sram6T_blwl sram_blwl_263_ (mux_2level_tapbuf_size4_29_sram_blwl_out[263:263] ,mux_2level_tapbuf_size4_29_sram_blwl_out[263:263] ,mux_2level_tapbuf_size4_29_sram_blwl_outb[263:263] ,mux_2level_tapbuf_size4_29_configbus0[263:263], mux_2level_tapbuf_size4_29_configbus1[263:263] , mux_2level_tapbuf_size4_29_configbus0_b[263:263] );
wire [0:3] mux_2level_tapbuf_size4_30_inbus;
assign mux_2level_tapbuf_size4_30_inbus[0] = chany_1__1__midout_4_;
assign mux_2level_tapbuf_size4_30_inbus[1] = chany_1__1__midout_5_;
assign mux_2level_tapbuf_size4_30_inbus[2] = chany_1__1__midout_22_;
assign mux_2level_tapbuf_size4_30_inbus[3] = chany_1__1__midout_23_;
wire [264:267] mux_2level_tapbuf_size4_30_configbus0;
wire [264:267] mux_2level_tapbuf_size4_30_configbus1;
wire [264:267] mux_2level_tapbuf_size4_30_sram_blwl_out ;
wire [264:267] mux_2level_tapbuf_size4_30_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_30_configbus0[264:267] = sram_blwl_bl[264:267] ;
assign mux_2level_tapbuf_size4_30_configbus1[264:267] = sram_blwl_wl[264:267] ;
wire [264:267] mux_2level_tapbuf_size4_30_configbus0_b;
assign mux_2level_tapbuf_size4_30_configbus0_b[264:267] = sram_blwl_blb[264:267] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_30_ (mux_2level_tapbuf_size4_30_inbus, grid_2__1__pin_0__3__6_, mux_2level_tapbuf_size4_30_sram_blwl_out[264:267] ,
mux_2level_tapbuf_size4_30_sram_blwl_outb[264:267] );
//----- SRAM bits for MUX[30], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_264_ (mux_2level_tapbuf_size4_30_sram_blwl_out[264:264] ,mux_2level_tapbuf_size4_30_sram_blwl_out[264:264] ,mux_2level_tapbuf_size4_30_sram_blwl_outb[264:264] ,mux_2level_tapbuf_size4_30_configbus0[264:264], mux_2level_tapbuf_size4_30_configbus1[264:264] , mux_2level_tapbuf_size4_30_configbus0_b[264:264] );
sram6T_blwl sram_blwl_265_ (mux_2level_tapbuf_size4_30_sram_blwl_out[265:265] ,mux_2level_tapbuf_size4_30_sram_blwl_out[265:265] ,mux_2level_tapbuf_size4_30_sram_blwl_outb[265:265] ,mux_2level_tapbuf_size4_30_configbus0[265:265], mux_2level_tapbuf_size4_30_configbus1[265:265] , mux_2level_tapbuf_size4_30_configbus0_b[265:265] );
sram6T_blwl sram_blwl_266_ (mux_2level_tapbuf_size4_30_sram_blwl_out[266:266] ,mux_2level_tapbuf_size4_30_sram_blwl_out[266:266] ,mux_2level_tapbuf_size4_30_sram_blwl_outb[266:266] ,mux_2level_tapbuf_size4_30_configbus0[266:266], mux_2level_tapbuf_size4_30_configbus1[266:266] , mux_2level_tapbuf_size4_30_configbus0_b[266:266] );
sram6T_blwl sram_blwl_267_ (mux_2level_tapbuf_size4_30_sram_blwl_out[267:267] ,mux_2level_tapbuf_size4_30_sram_blwl_out[267:267] ,mux_2level_tapbuf_size4_30_sram_blwl_outb[267:267] ,mux_2level_tapbuf_size4_30_configbus0[267:267], mux_2level_tapbuf_size4_30_configbus1[267:267] , mux_2level_tapbuf_size4_30_configbus0_b[267:267] );
wire [0:3] mux_2level_tapbuf_size4_31_inbus;
assign mux_2level_tapbuf_size4_31_inbus[0] = chany_1__1__midout_10_;
assign mux_2level_tapbuf_size4_31_inbus[1] = chany_1__1__midout_11_;
assign mux_2level_tapbuf_size4_31_inbus[2] = chany_1__1__midout_22_;
assign mux_2level_tapbuf_size4_31_inbus[3] = chany_1__1__midout_23_;
wire [268:271] mux_2level_tapbuf_size4_31_configbus0;
wire [268:271] mux_2level_tapbuf_size4_31_configbus1;
wire [268:271] mux_2level_tapbuf_size4_31_sram_blwl_out ;
wire [268:271] mux_2level_tapbuf_size4_31_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_31_configbus0[268:271] = sram_blwl_bl[268:271] ;
assign mux_2level_tapbuf_size4_31_configbus1[268:271] = sram_blwl_wl[268:271] ;
wire [268:271] mux_2level_tapbuf_size4_31_configbus0_b;
assign mux_2level_tapbuf_size4_31_configbus0_b[268:271] = sram_blwl_blb[268:271] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_31_ (mux_2level_tapbuf_size4_31_inbus, grid_2__1__pin_0__3__8_, mux_2level_tapbuf_size4_31_sram_blwl_out[268:271] ,
mux_2level_tapbuf_size4_31_sram_blwl_outb[268:271] );
//----- SRAM bits for MUX[31], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_268_ (mux_2level_tapbuf_size4_31_sram_blwl_out[268:268] ,mux_2level_tapbuf_size4_31_sram_blwl_out[268:268] ,mux_2level_tapbuf_size4_31_sram_blwl_outb[268:268] ,mux_2level_tapbuf_size4_31_configbus0[268:268], mux_2level_tapbuf_size4_31_configbus1[268:268] , mux_2level_tapbuf_size4_31_configbus0_b[268:268] );
sram6T_blwl sram_blwl_269_ (mux_2level_tapbuf_size4_31_sram_blwl_out[269:269] ,mux_2level_tapbuf_size4_31_sram_blwl_out[269:269] ,mux_2level_tapbuf_size4_31_sram_blwl_outb[269:269] ,mux_2level_tapbuf_size4_31_configbus0[269:269], mux_2level_tapbuf_size4_31_configbus1[269:269] , mux_2level_tapbuf_size4_31_configbus0_b[269:269] );
sram6T_blwl sram_blwl_270_ (mux_2level_tapbuf_size4_31_sram_blwl_out[270:270] ,mux_2level_tapbuf_size4_31_sram_blwl_out[270:270] ,mux_2level_tapbuf_size4_31_sram_blwl_outb[270:270] ,mux_2level_tapbuf_size4_31_configbus0[270:270], mux_2level_tapbuf_size4_31_configbus1[270:270] , mux_2level_tapbuf_size4_31_configbus0_b[270:270] );
sram6T_blwl sram_blwl_271_ (mux_2level_tapbuf_size4_31_sram_blwl_out[271:271] ,mux_2level_tapbuf_size4_31_sram_blwl_out[271:271] ,mux_2level_tapbuf_size4_31_sram_blwl_outb[271:271] ,mux_2level_tapbuf_size4_31_configbus0[271:271], mux_2level_tapbuf_size4_31_configbus1[271:271] , mux_2level_tapbuf_size4_31_configbus0_b[271:271] );
wire [0:3] mux_2level_tapbuf_size4_32_inbus;
assign mux_2level_tapbuf_size4_32_inbus[0] = chany_1__1__midout_8_;
assign mux_2level_tapbuf_size4_32_inbus[1] = chany_1__1__midout_9_;
assign mux_2level_tapbuf_size4_32_inbus[2] = chany_1__1__midout_24_;
assign mux_2level_tapbuf_size4_32_inbus[3] = chany_1__1__midout_25_;
wire [272:275] mux_2level_tapbuf_size4_32_configbus0;
wire [272:275] mux_2level_tapbuf_size4_32_configbus1;
wire [272:275] mux_2level_tapbuf_size4_32_sram_blwl_out ;
wire [272:275] mux_2level_tapbuf_size4_32_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_32_configbus0[272:275] = sram_blwl_bl[272:275] ;
assign mux_2level_tapbuf_size4_32_configbus1[272:275] = sram_blwl_wl[272:275] ;
wire [272:275] mux_2level_tapbuf_size4_32_configbus0_b;
assign mux_2level_tapbuf_size4_32_configbus0_b[272:275] = sram_blwl_blb[272:275] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_32_ (mux_2level_tapbuf_size4_32_inbus, grid_2__1__pin_0__3__10_, mux_2level_tapbuf_size4_32_sram_blwl_out[272:275] ,
mux_2level_tapbuf_size4_32_sram_blwl_outb[272:275] );
//----- SRAM bits for MUX[32], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_272_ (mux_2level_tapbuf_size4_32_sram_blwl_out[272:272] ,mux_2level_tapbuf_size4_32_sram_blwl_out[272:272] ,mux_2level_tapbuf_size4_32_sram_blwl_outb[272:272] ,mux_2level_tapbuf_size4_32_configbus0[272:272], mux_2level_tapbuf_size4_32_configbus1[272:272] , mux_2level_tapbuf_size4_32_configbus0_b[272:272] );
sram6T_blwl sram_blwl_273_ (mux_2level_tapbuf_size4_32_sram_blwl_out[273:273] ,mux_2level_tapbuf_size4_32_sram_blwl_out[273:273] ,mux_2level_tapbuf_size4_32_sram_blwl_outb[273:273] ,mux_2level_tapbuf_size4_32_configbus0[273:273], mux_2level_tapbuf_size4_32_configbus1[273:273] , mux_2level_tapbuf_size4_32_configbus0_b[273:273] );
sram6T_blwl sram_blwl_274_ (mux_2level_tapbuf_size4_32_sram_blwl_out[274:274] ,mux_2level_tapbuf_size4_32_sram_blwl_out[274:274] ,mux_2level_tapbuf_size4_32_sram_blwl_outb[274:274] ,mux_2level_tapbuf_size4_32_configbus0[274:274], mux_2level_tapbuf_size4_32_configbus1[274:274] , mux_2level_tapbuf_size4_32_configbus0_b[274:274] );
sram6T_blwl sram_blwl_275_ (mux_2level_tapbuf_size4_32_sram_blwl_out[275:275] ,mux_2level_tapbuf_size4_32_sram_blwl_out[275:275] ,mux_2level_tapbuf_size4_32_sram_blwl_outb[275:275] ,mux_2level_tapbuf_size4_32_configbus0[275:275], mux_2level_tapbuf_size4_32_configbus1[275:275] , mux_2level_tapbuf_size4_32_configbus0_b[275:275] );
wire [0:3] mux_2level_tapbuf_size4_33_inbus;
assign mux_2level_tapbuf_size4_33_inbus[0] = chany_1__1__midout_14_;
assign mux_2level_tapbuf_size4_33_inbus[1] = chany_1__1__midout_15_;
assign mux_2level_tapbuf_size4_33_inbus[2] = chany_1__1__midout_26_;
assign mux_2level_tapbuf_size4_33_inbus[3] = chany_1__1__midout_27_;
wire [276:279] mux_2level_tapbuf_size4_33_configbus0;
wire [276:279] mux_2level_tapbuf_size4_33_configbus1;
wire [276:279] mux_2level_tapbuf_size4_33_sram_blwl_out ;
wire [276:279] mux_2level_tapbuf_size4_33_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_33_configbus0[276:279] = sram_blwl_bl[276:279] ;
assign mux_2level_tapbuf_size4_33_configbus1[276:279] = sram_blwl_wl[276:279] ;
wire [276:279] mux_2level_tapbuf_size4_33_configbus0_b;
assign mux_2level_tapbuf_size4_33_configbus0_b[276:279] = sram_blwl_blb[276:279] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_33_ (mux_2level_tapbuf_size4_33_inbus, grid_2__1__pin_0__3__12_, mux_2level_tapbuf_size4_33_sram_blwl_out[276:279] ,
mux_2level_tapbuf_size4_33_sram_blwl_outb[276:279] );
//----- SRAM bits for MUX[33], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_276_ (mux_2level_tapbuf_size4_33_sram_blwl_out[276:276] ,mux_2level_tapbuf_size4_33_sram_blwl_out[276:276] ,mux_2level_tapbuf_size4_33_sram_blwl_outb[276:276] ,mux_2level_tapbuf_size4_33_configbus0[276:276], mux_2level_tapbuf_size4_33_configbus1[276:276] , mux_2level_tapbuf_size4_33_configbus0_b[276:276] );
sram6T_blwl sram_blwl_277_ (mux_2level_tapbuf_size4_33_sram_blwl_out[277:277] ,mux_2level_tapbuf_size4_33_sram_blwl_out[277:277] ,mux_2level_tapbuf_size4_33_sram_blwl_outb[277:277] ,mux_2level_tapbuf_size4_33_configbus0[277:277], mux_2level_tapbuf_size4_33_configbus1[277:277] , mux_2level_tapbuf_size4_33_configbus0_b[277:277] );
sram6T_blwl sram_blwl_278_ (mux_2level_tapbuf_size4_33_sram_blwl_out[278:278] ,mux_2level_tapbuf_size4_33_sram_blwl_out[278:278] ,mux_2level_tapbuf_size4_33_sram_blwl_outb[278:278] ,mux_2level_tapbuf_size4_33_configbus0[278:278], mux_2level_tapbuf_size4_33_configbus1[278:278] , mux_2level_tapbuf_size4_33_configbus0_b[278:278] );
sram6T_blwl sram_blwl_279_ (mux_2level_tapbuf_size4_33_sram_blwl_out[279:279] ,mux_2level_tapbuf_size4_33_sram_blwl_out[279:279] ,mux_2level_tapbuf_size4_33_sram_blwl_outb[279:279] ,mux_2level_tapbuf_size4_33_configbus0[279:279], mux_2level_tapbuf_size4_33_configbus1[279:279] , mux_2level_tapbuf_size4_33_configbus0_b[279:279] );
wire [0:3] mux_2level_tapbuf_size4_34_inbus;
assign mux_2level_tapbuf_size4_34_inbus[0] = chany_1__1__midout_12_;
assign mux_2level_tapbuf_size4_34_inbus[1] = chany_1__1__midout_13_;
assign mux_2level_tapbuf_size4_34_inbus[2] = chany_1__1__midout_28_;
assign mux_2level_tapbuf_size4_34_inbus[3] = chany_1__1__midout_29_;
wire [280:283] mux_2level_tapbuf_size4_34_configbus0;
wire [280:283] mux_2level_tapbuf_size4_34_configbus1;
wire [280:283] mux_2level_tapbuf_size4_34_sram_blwl_out ;
wire [280:283] mux_2level_tapbuf_size4_34_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_34_configbus0[280:283] = sram_blwl_bl[280:283] ;
assign mux_2level_tapbuf_size4_34_configbus1[280:283] = sram_blwl_wl[280:283] ;
wire [280:283] mux_2level_tapbuf_size4_34_configbus0_b;
assign mux_2level_tapbuf_size4_34_configbus0_b[280:283] = sram_blwl_blb[280:283] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_34_ (mux_2level_tapbuf_size4_34_inbus, grid_2__1__pin_0__3__14_, mux_2level_tapbuf_size4_34_sram_blwl_out[280:283] ,
mux_2level_tapbuf_size4_34_sram_blwl_outb[280:283] );
//----- SRAM bits for MUX[34], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_280_ (mux_2level_tapbuf_size4_34_sram_blwl_out[280:280] ,mux_2level_tapbuf_size4_34_sram_blwl_out[280:280] ,mux_2level_tapbuf_size4_34_sram_blwl_outb[280:280] ,mux_2level_tapbuf_size4_34_configbus0[280:280], mux_2level_tapbuf_size4_34_configbus1[280:280] , mux_2level_tapbuf_size4_34_configbus0_b[280:280] );
sram6T_blwl sram_blwl_281_ (mux_2level_tapbuf_size4_34_sram_blwl_out[281:281] ,mux_2level_tapbuf_size4_34_sram_blwl_out[281:281] ,mux_2level_tapbuf_size4_34_sram_blwl_outb[281:281] ,mux_2level_tapbuf_size4_34_configbus0[281:281], mux_2level_tapbuf_size4_34_configbus1[281:281] , mux_2level_tapbuf_size4_34_configbus0_b[281:281] );
sram6T_blwl sram_blwl_282_ (mux_2level_tapbuf_size4_34_sram_blwl_out[282:282] ,mux_2level_tapbuf_size4_34_sram_blwl_out[282:282] ,mux_2level_tapbuf_size4_34_sram_blwl_outb[282:282] ,mux_2level_tapbuf_size4_34_configbus0[282:282], mux_2level_tapbuf_size4_34_configbus1[282:282] , mux_2level_tapbuf_size4_34_configbus0_b[282:282] );
sram6T_blwl sram_blwl_283_ (mux_2level_tapbuf_size4_34_sram_blwl_out[283:283] ,mux_2level_tapbuf_size4_34_sram_blwl_out[283:283] ,mux_2level_tapbuf_size4_34_sram_blwl_outb[283:283] ,mux_2level_tapbuf_size4_34_configbus0[283:283], mux_2level_tapbuf_size4_34_configbus1[283:283] , mux_2level_tapbuf_size4_34_configbus0_b[283:283] );
wire [0:3] mux_2level_tapbuf_size4_35_inbus;
assign mux_2level_tapbuf_size4_35_inbus[0] = chany_1__1__midout_0_;
assign mux_2level_tapbuf_size4_35_inbus[1] = chany_1__1__midout_1_;
assign mux_2level_tapbuf_size4_35_inbus[2] = chany_1__1__midout_16_;
assign mux_2level_tapbuf_size4_35_inbus[3] = chany_1__1__midout_17_;
wire [284:287] mux_2level_tapbuf_size4_35_configbus0;
wire [284:287] mux_2level_tapbuf_size4_35_configbus1;
wire [284:287] mux_2level_tapbuf_size4_35_sram_blwl_out ;
wire [284:287] mux_2level_tapbuf_size4_35_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_35_configbus0[284:287] = sram_blwl_bl[284:287] ;
assign mux_2level_tapbuf_size4_35_configbus1[284:287] = sram_blwl_wl[284:287] ;
wire [284:287] mux_2level_tapbuf_size4_35_configbus0_b;
assign mux_2level_tapbuf_size4_35_configbus0_b[284:287] = sram_blwl_blb[284:287] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_35_ (mux_2level_tapbuf_size4_35_inbus, grid_1__1__pin_0__1__1_, mux_2level_tapbuf_size4_35_sram_blwl_out[284:287] ,
mux_2level_tapbuf_size4_35_sram_blwl_outb[284:287] );
//----- SRAM bits for MUX[35], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_284_ (mux_2level_tapbuf_size4_35_sram_blwl_out[284:284] ,mux_2level_tapbuf_size4_35_sram_blwl_out[284:284] ,mux_2level_tapbuf_size4_35_sram_blwl_outb[284:284] ,mux_2level_tapbuf_size4_35_configbus0[284:284], mux_2level_tapbuf_size4_35_configbus1[284:284] , mux_2level_tapbuf_size4_35_configbus0_b[284:284] );
sram6T_blwl sram_blwl_285_ (mux_2level_tapbuf_size4_35_sram_blwl_out[285:285] ,mux_2level_tapbuf_size4_35_sram_blwl_out[285:285] ,mux_2level_tapbuf_size4_35_sram_blwl_outb[285:285] ,mux_2level_tapbuf_size4_35_configbus0[285:285], mux_2level_tapbuf_size4_35_configbus1[285:285] , mux_2level_tapbuf_size4_35_configbus0_b[285:285] );
sram6T_blwl sram_blwl_286_ (mux_2level_tapbuf_size4_35_sram_blwl_out[286:286] ,mux_2level_tapbuf_size4_35_sram_blwl_out[286:286] ,mux_2level_tapbuf_size4_35_sram_blwl_outb[286:286] ,mux_2level_tapbuf_size4_35_configbus0[286:286], mux_2level_tapbuf_size4_35_configbus1[286:286] , mux_2level_tapbuf_size4_35_configbus0_b[286:286] );
sram6T_blwl sram_blwl_287_ (mux_2level_tapbuf_size4_35_sram_blwl_out[287:287] ,mux_2level_tapbuf_size4_35_sram_blwl_out[287:287] ,mux_2level_tapbuf_size4_35_sram_blwl_outb[287:287] ,mux_2level_tapbuf_size4_35_configbus0[287:287], mux_2level_tapbuf_size4_35_configbus1[287:287] , mux_2level_tapbuf_size4_35_configbus0_b[287:287] );
endmodule
//----- END Verilog Module of Connection Box -Y direction [1][1] -----

