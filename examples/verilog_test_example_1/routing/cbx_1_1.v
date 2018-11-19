//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Connection Block - X direction  [1][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
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

output  grid_1__2__pin_0__2__0_,

output  grid_1__2__pin_0__2__2_,

output  grid_1__2__pin_0__2__4_,

output  grid_1__2__pin_0__2__6_,

output  grid_1__2__pin_0__2__8_,

output  grid_1__2__pin_0__2__10_,

output  grid_1__2__pin_0__2__12_,

output  grid_1__2__pin_0__2__14_,

output  grid_1__1__pin_0__0__0_,

input [180:215] sram_blwl_bl ,
input [180:215] sram_blwl_wl ,
input [180:215] sram_blwl_blb );
wire [0:3] mux_2level_tapbuf_size4_9_inbus;
assign mux_2level_tapbuf_size4_9_inbus[0] = chanx_1__1__midout_6_;
assign mux_2level_tapbuf_size4_9_inbus[1] = chanx_1__1__midout_7_;
assign mux_2level_tapbuf_size4_9_inbus[2] = chanx_1__1__midout_12_;
assign mux_2level_tapbuf_size4_9_inbus[3] = chanx_1__1__midout_13_;
wire [180:183] mux_2level_tapbuf_size4_9_configbus0;
wire [180:183] mux_2level_tapbuf_size4_9_configbus1;
wire [180:183] mux_2level_tapbuf_size4_9_sram_blwl_out ;
wire [180:183] mux_2level_tapbuf_size4_9_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_9_configbus0[180:183] = sram_blwl_bl[180:183] ;
assign mux_2level_tapbuf_size4_9_configbus1[180:183] = sram_blwl_wl[180:183] ;
wire [180:183] mux_2level_tapbuf_size4_9_configbus0_b;
assign mux_2level_tapbuf_size4_9_configbus0_b[180:183] = sram_blwl_blb[180:183] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_9_ (mux_2level_tapbuf_size4_9_inbus, grid_1__2__pin_0__2__0_, mux_2level_tapbuf_size4_9_sram_blwl_out[180:183] ,
mux_2level_tapbuf_size4_9_sram_blwl_outb[180:183] );
//----- SRAM bits for MUX[9], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_180_ (mux_2level_tapbuf_size4_9_sram_blwl_out[180:180] ,mux_2level_tapbuf_size4_9_sram_blwl_out[180:180] ,mux_2level_tapbuf_size4_9_sram_blwl_outb[180:180] ,mux_2level_tapbuf_size4_9_configbus0[180:180], mux_2level_tapbuf_size4_9_configbus1[180:180] , mux_2level_tapbuf_size4_9_configbus0_b[180:180] );
sram6T_blwl sram_blwl_181_ (mux_2level_tapbuf_size4_9_sram_blwl_out[181:181] ,mux_2level_tapbuf_size4_9_sram_blwl_out[181:181] ,mux_2level_tapbuf_size4_9_sram_blwl_outb[181:181] ,mux_2level_tapbuf_size4_9_configbus0[181:181], mux_2level_tapbuf_size4_9_configbus1[181:181] , mux_2level_tapbuf_size4_9_configbus0_b[181:181] );
sram6T_blwl sram_blwl_182_ (mux_2level_tapbuf_size4_9_sram_blwl_out[182:182] ,mux_2level_tapbuf_size4_9_sram_blwl_out[182:182] ,mux_2level_tapbuf_size4_9_sram_blwl_outb[182:182] ,mux_2level_tapbuf_size4_9_configbus0[182:182], mux_2level_tapbuf_size4_9_configbus1[182:182] , mux_2level_tapbuf_size4_9_configbus0_b[182:182] );
sram6T_blwl sram_blwl_183_ (mux_2level_tapbuf_size4_9_sram_blwl_out[183:183] ,mux_2level_tapbuf_size4_9_sram_blwl_out[183:183] ,mux_2level_tapbuf_size4_9_sram_blwl_outb[183:183] ,mux_2level_tapbuf_size4_9_configbus0[183:183], mux_2level_tapbuf_size4_9_configbus1[183:183] , mux_2level_tapbuf_size4_9_configbus0_b[183:183] );
wire [0:3] mux_2level_tapbuf_size4_10_inbus;
assign mux_2level_tapbuf_size4_10_inbus[0] = chanx_1__1__midout_0_;
assign mux_2level_tapbuf_size4_10_inbus[1] = chanx_1__1__midout_1_;
assign mux_2level_tapbuf_size4_10_inbus[2] = chanx_1__1__midout_18_;
assign mux_2level_tapbuf_size4_10_inbus[3] = chanx_1__1__midout_19_;
wire [184:187] mux_2level_tapbuf_size4_10_configbus0;
wire [184:187] mux_2level_tapbuf_size4_10_configbus1;
wire [184:187] mux_2level_tapbuf_size4_10_sram_blwl_out ;
wire [184:187] mux_2level_tapbuf_size4_10_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_10_configbus0[184:187] = sram_blwl_bl[184:187] ;
assign mux_2level_tapbuf_size4_10_configbus1[184:187] = sram_blwl_wl[184:187] ;
wire [184:187] mux_2level_tapbuf_size4_10_configbus0_b;
assign mux_2level_tapbuf_size4_10_configbus0_b[184:187] = sram_blwl_blb[184:187] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_10_ (mux_2level_tapbuf_size4_10_inbus, grid_1__2__pin_0__2__2_, mux_2level_tapbuf_size4_10_sram_blwl_out[184:187] ,
mux_2level_tapbuf_size4_10_sram_blwl_outb[184:187] );
//----- SRAM bits for MUX[10], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_184_ (mux_2level_tapbuf_size4_10_sram_blwl_out[184:184] ,mux_2level_tapbuf_size4_10_sram_blwl_out[184:184] ,mux_2level_tapbuf_size4_10_sram_blwl_outb[184:184] ,mux_2level_tapbuf_size4_10_configbus0[184:184], mux_2level_tapbuf_size4_10_configbus1[184:184] , mux_2level_tapbuf_size4_10_configbus0_b[184:184] );
sram6T_blwl sram_blwl_185_ (mux_2level_tapbuf_size4_10_sram_blwl_out[185:185] ,mux_2level_tapbuf_size4_10_sram_blwl_out[185:185] ,mux_2level_tapbuf_size4_10_sram_blwl_outb[185:185] ,mux_2level_tapbuf_size4_10_configbus0[185:185], mux_2level_tapbuf_size4_10_configbus1[185:185] , mux_2level_tapbuf_size4_10_configbus0_b[185:185] );
sram6T_blwl sram_blwl_186_ (mux_2level_tapbuf_size4_10_sram_blwl_out[186:186] ,mux_2level_tapbuf_size4_10_sram_blwl_out[186:186] ,mux_2level_tapbuf_size4_10_sram_blwl_outb[186:186] ,mux_2level_tapbuf_size4_10_configbus0[186:186], mux_2level_tapbuf_size4_10_configbus1[186:186] , mux_2level_tapbuf_size4_10_configbus0_b[186:186] );
sram6T_blwl sram_blwl_187_ (mux_2level_tapbuf_size4_10_sram_blwl_out[187:187] ,mux_2level_tapbuf_size4_10_sram_blwl_out[187:187] ,mux_2level_tapbuf_size4_10_sram_blwl_outb[187:187] ,mux_2level_tapbuf_size4_10_configbus0[187:187], mux_2level_tapbuf_size4_10_configbus1[187:187] , mux_2level_tapbuf_size4_10_configbus0_b[187:187] );
wire [0:3] mux_2level_tapbuf_size4_11_inbus;
assign mux_2level_tapbuf_size4_11_inbus[0] = chanx_1__1__midout_2_;
assign mux_2level_tapbuf_size4_11_inbus[1] = chanx_1__1__midout_3_;
assign mux_2level_tapbuf_size4_11_inbus[2] = chanx_1__1__midout_16_;
assign mux_2level_tapbuf_size4_11_inbus[3] = chanx_1__1__midout_17_;
wire [188:191] mux_2level_tapbuf_size4_11_configbus0;
wire [188:191] mux_2level_tapbuf_size4_11_configbus1;
wire [188:191] mux_2level_tapbuf_size4_11_sram_blwl_out ;
wire [188:191] mux_2level_tapbuf_size4_11_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_11_configbus0[188:191] = sram_blwl_bl[188:191] ;
assign mux_2level_tapbuf_size4_11_configbus1[188:191] = sram_blwl_wl[188:191] ;
wire [188:191] mux_2level_tapbuf_size4_11_configbus0_b;
assign mux_2level_tapbuf_size4_11_configbus0_b[188:191] = sram_blwl_blb[188:191] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_11_ (mux_2level_tapbuf_size4_11_inbus, grid_1__2__pin_0__2__4_, mux_2level_tapbuf_size4_11_sram_blwl_out[188:191] ,
mux_2level_tapbuf_size4_11_sram_blwl_outb[188:191] );
//----- SRAM bits for MUX[11], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_188_ (mux_2level_tapbuf_size4_11_sram_blwl_out[188:188] ,mux_2level_tapbuf_size4_11_sram_blwl_out[188:188] ,mux_2level_tapbuf_size4_11_sram_blwl_outb[188:188] ,mux_2level_tapbuf_size4_11_configbus0[188:188], mux_2level_tapbuf_size4_11_configbus1[188:188] , mux_2level_tapbuf_size4_11_configbus0_b[188:188] );
sram6T_blwl sram_blwl_189_ (mux_2level_tapbuf_size4_11_sram_blwl_out[189:189] ,mux_2level_tapbuf_size4_11_sram_blwl_out[189:189] ,mux_2level_tapbuf_size4_11_sram_blwl_outb[189:189] ,mux_2level_tapbuf_size4_11_configbus0[189:189], mux_2level_tapbuf_size4_11_configbus1[189:189] , mux_2level_tapbuf_size4_11_configbus0_b[189:189] );
sram6T_blwl sram_blwl_190_ (mux_2level_tapbuf_size4_11_sram_blwl_out[190:190] ,mux_2level_tapbuf_size4_11_sram_blwl_out[190:190] ,mux_2level_tapbuf_size4_11_sram_blwl_outb[190:190] ,mux_2level_tapbuf_size4_11_configbus0[190:190], mux_2level_tapbuf_size4_11_configbus1[190:190] , mux_2level_tapbuf_size4_11_configbus0_b[190:190] );
sram6T_blwl sram_blwl_191_ (mux_2level_tapbuf_size4_11_sram_blwl_out[191:191] ,mux_2level_tapbuf_size4_11_sram_blwl_out[191:191] ,mux_2level_tapbuf_size4_11_sram_blwl_outb[191:191] ,mux_2level_tapbuf_size4_11_configbus0[191:191], mux_2level_tapbuf_size4_11_configbus1[191:191] , mux_2level_tapbuf_size4_11_configbus0_b[191:191] );
wire [0:3] mux_2level_tapbuf_size4_12_inbus;
assign mux_2level_tapbuf_size4_12_inbus[0] = chanx_1__1__midout_4_;
assign mux_2level_tapbuf_size4_12_inbus[1] = chanx_1__1__midout_5_;
assign mux_2level_tapbuf_size4_12_inbus[2] = chanx_1__1__midout_20_;
assign mux_2level_tapbuf_size4_12_inbus[3] = chanx_1__1__midout_21_;
wire [192:195] mux_2level_tapbuf_size4_12_configbus0;
wire [192:195] mux_2level_tapbuf_size4_12_configbus1;
wire [192:195] mux_2level_tapbuf_size4_12_sram_blwl_out ;
wire [192:195] mux_2level_tapbuf_size4_12_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_12_configbus0[192:195] = sram_blwl_bl[192:195] ;
assign mux_2level_tapbuf_size4_12_configbus1[192:195] = sram_blwl_wl[192:195] ;
wire [192:195] mux_2level_tapbuf_size4_12_configbus0_b;
assign mux_2level_tapbuf_size4_12_configbus0_b[192:195] = sram_blwl_blb[192:195] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_12_ (mux_2level_tapbuf_size4_12_inbus, grid_1__2__pin_0__2__6_, mux_2level_tapbuf_size4_12_sram_blwl_out[192:195] ,
mux_2level_tapbuf_size4_12_sram_blwl_outb[192:195] );
//----- SRAM bits for MUX[12], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_192_ (mux_2level_tapbuf_size4_12_sram_blwl_out[192:192] ,mux_2level_tapbuf_size4_12_sram_blwl_out[192:192] ,mux_2level_tapbuf_size4_12_sram_blwl_outb[192:192] ,mux_2level_tapbuf_size4_12_configbus0[192:192], mux_2level_tapbuf_size4_12_configbus1[192:192] , mux_2level_tapbuf_size4_12_configbus0_b[192:192] );
sram6T_blwl sram_blwl_193_ (mux_2level_tapbuf_size4_12_sram_blwl_out[193:193] ,mux_2level_tapbuf_size4_12_sram_blwl_out[193:193] ,mux_2level_tapbuf_size4_12_sram_blwl_outb[193:193] ,mux_2level_tapbuf_size4_12_configbus0[193:193], mux_2level_tapbuf_size4_12_configbus1[193:193] , mux_2level_tapbuf_size4_12_configbus0_b[193:193] );
sram6T_blwl sram_blwl_194_ (mux_2level_tapbuf_size4_12_sram_blwl_out[194:194] ,mux_2level_tapbuf_size4_12_sram_blwl_out[194:194] ,mux_2level_tapbuf_size4_12_sram_blwl_outb[194:194] ,mux_2level_tapbuf_size4_12_configbus0[194:194], mux_2level_tapbuf_size4_12_configbus1[194:194] , mux_2level_tapbuf_size4_12_configbus0_b[194:194] );
sram6T_blwl sram_blwl_195_ (mux_2level_tapbuf_size4_12_sram_blwl_out[195:195] ,mux_2level_tapbuf_size4_12_sram_blwl_out[195:195] ,mux_2level_tapbuf_size4_12_sram_blwl_outb[195:195] ,mux_2level_tapbuf_size4_12_configbus0[195:195], mux_2level_tapbuf_size4_12_configbus1[195:195] , mux_2level_tapbuf_size4_12_configbus0_b[195:195] );
wire [0:3] mux_2level_tapbuf_size4_13_inbus;
assign mux_2level_tapbuf_size4_13_inbus[0] = chanx_1__1__midout_10_;
assign mux_2level_tapbuf_size4_13_inbus[1] = chanx_1__1__midout_11_;
assign mux_2level_tapbuf_size4_13_inbus[2] = chanx_1__1__midout_22_;
assign mux_2level_tapbuf_size4_13_inbus[3] = chanx_1__1__midout_23_;
wire [196:199] mux_2level_tapbuf_size4_13_configbus0;
wire [196:199] mux_2level_tapbuf_size4_13_configbus1;
wire [196:199] mux_2level_tapbuf_size4_13_sram_blwl_out ;
wire [196:199] mux_2level_tapbuf_size4_13_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_13_configbus0[196:199] = sram_blwl_bl[196:199] ;
assign mux_2level_tapbuf_size4_13_configbus1[196:199] = sram_blwl_wl[196:199] ;
wire [196:199] mux_2level_tapbuf_size4_13_configbus0_b;
assign mux_2level_tapbuf_size4_13_configbus0_b[196:199] = sram_blwl_blb[196:199] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_13_ (mux_2level_tapbuf_size4_13_inbus, grid_1__2__pin_0__2__8_, mux_2level_tapbuf_size4_13_sram_blwl_out[196:199] ,
mux_2level_tapbuf_size4_13_sram_blwl_outb[196:199] );
//----- SRAM bits for MUX[13], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_196_ (mux_2level_tapbuf_size4_13_sram_blwl_out[196:196] ,mux_2level_tapbuf_size4_13_sram_blwl_out[196:196] ,mux_2level_tapbuf_size4_13_sram_blwl_outb[196:196] ,mux_2level_tapbuf_size4_13_configbus0[196:196], mux_2level_tapbuf_size4_13_configbus1[196:196] , mux_2level_tapbuf_size4_13_configbus0_b[196:196] );
sram6T_blwl sram_blwl_197_ (mux_2level_tapbuf_size4_13_sram_blwl_out[197:197] ,mux_2level_tapbuf_size4_13_sram_blwl_out[197:197] ,mux_2level_tapbuf_size4_13_sram_blwl_outb[197:197] ,mux_2level_tapbuf_size4_13_configbus0[197:197], mux_2level_tapbuf_size4_13_configbus1[197:197] , mux_2level_tapbuf_size4_13_configbus0_b[197:197] );
sram6T_blwl sram_blwl_198_ (mux_2level_tapbuf_size4_13_sram_blwl_out[198:198] ,mux_2level_tapbuf_size4_13_sram_blwl_out[198:198] ,mux_2level_tapbuf_size4_13_sram_blwl_outb[198:198] ,mux_2level_tapbuf_size4_13_configbus0[198:198], mux_2level_tapbuf_size4_13_configbus1[198:198] , mux_2level_tapbuf_size4_13_configbus0_b[198:198] );
sram6T_blwl sram_blwl_199_ (mux_2level_tapbuf_size4_13_sram_blwl_out[199:199] ,mux_2level_tapbuf_size4_13_sram_blwl_out[199:199] ,mux_2level_tapbuf_size4_13_sram_blwl_outb[199:199] ,mux_2level_tapbuf_size4_13_configbus0[199:199], mux_2level_tapbuf_size4_13_configbus1[199:199] , mux_2level_tapbuf_size4_13_configbus0_b[199:199] );
wire [0:3] mux_2level_tapbuf_size4_14_inbus;
assign mux_2level_tapbuf_size4_14_inbus[0] = chanx_1__1__midout_8_;
assign mux_2level_tapbuf_size4_14_inbus[1] = chanx_1__1__midout_9_;
assign mux_2level_tapbuf_size4_14_inbus[2] = chanx_1__1__midout_24_;
assign mux_2level_tapbuf_size4_14_inbus[3] = chanx_1__1__midout_25_;
wire [200:203] mux_2level_tapbuf_size4_14_configbus0;
wire [200:203] mux_2level_tapbuf_size4_14_configbus1;
wire [200:203] mux_2level_tapbuf_size4_14_sram_blwl_out ;
wire [200:203] mux_2level_tapbuf_size4_14_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_14_configbus0[200:203] = sram_blwl_bl[200:203] ;
assign mux_2level_tapbuf_size4_14_configbus1[200:203] = sram_blwl_wl[200:203] ;
wire [200:203] mux_2level_tapbuf_size4_14_configbus0_b;
assign mux_2level_tapbuf_size4_14_configbus0_b[200:203] = sram_blwl_blb[200:203] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_14_ (mux_2level_tapbuf_size4_14_inbus, grid_1__2__pin_0__2__10_, mux_2level_tapbuf_size4_14_sram_blwl_out[200:203] ,
mux_2level_tapbuf_size4_14_sram_blwl_outb[200:203] );
//----- SRAM bits for MUX[14], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_200_ (mux_2level_tapbuf_size4_14_sram_blwl_out[200:200] ,mux_2level_tapbuf_size4_14_sram_blwl_out[200:200] ,mux_2level_tapbuf_size4_14_sram_blwl_outb[200:200] ,mux_2level_tapbuf_size4_14_configbus0[200:200], mux_2level_tapbuf_size4_14_configbus1[200:200] , mux_2level_tapbuf_size4_14_configbus0_b[200:200] );
sram6T_blwl sram_blwl_201_ (mux_2level_tapbuf_size4_14_sram_blwl_out[201:201] ,mux_2level_tapbuf_size4_14_sram_blwl_out[201:201] ,mux_2level_tapbuf_size4_14_sram_blwl_outb[201:201] ,mux_2level_tapbuf_size4_14_configbus0[201:201], mux_2level_tapbuf_size4_14_configbus1[201:201] , mux_2level_tapbuf_size4_14_configbus0_b[201:201] );
sram6T_blwl sram_blwl_202_ (mux_2level_tapbuf_size4_14_sram_blwl_out[202:202] ,mux_2level_tapbuf_size4_14_sram_blwl_out[202:202] ,mux_2level_tapbuf_size4_14_sram_blwl_outb[202:202] ,mux_2level_tapbuf_size4_14_configbus0[202:202], mux_2level_tapbuf_size4_14_configbus1[202:202] , mux_2level_tapbuf_size4_14_configbus0_b[202:202] );
sram6T_blwl sram_blwl_203_ (mux_2level_tapbuf_size4_14_sram_blwl_out[203:203] ,mux_2level_tapbuf_size4_14_sram_blwl_out[203:203] ,mux_2level_tapbuf_size4_14_sram_blwl_outb[203:203] ,mux_2level_tapbuf_size4_14_configbus0[203:203], mux_2level_tapbuf_size4_14_configbus1[203:203] , mux_2level_tapbuf_size4_14_configbus0_b[203:203] );
wire [0:3] mux_2level_tapbuf_size4_15_inbus;
assign mux_2level_tapbuf_size4_15_inbus[0] = chanx_1__1__midout_14_;
assign mux_2level_tapbuf_size4_15_inbus[1] = chanx_1__1__midout_15_;
assign mux_2level_tapbuf_size4_15_inbus[2] = chanx_1__1__midout_26_;
assign mux_2level_tapbuf_size4_15_inbus[3] = chanx_1__1__midout_27_;
wire [204:207] mux_2level_tapbuf_size4_15_configbus0;
wire [204:207] mux_2level_tapbuf_size4_15_configbus1;
wire [204:207] mux_2level_tapbuf_size4_15_sram_blwl_out ;
wire [204:207] mux_2level_tapbuf_size4_15_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_15_configbus0[204:207] = sram_blwl_bl[204:207] ;
assign mux_2level_tapbuf_size4_15_configbus1[204:207] = sram_blwl_wl[204:207] ;
wire [204:207] mux_2level_tapbuf_size4_15_configbus0_b;
assign mux_2level_tapbuf_size4_15_configbus0_b[204:207] = sram_blwl_blb[204:207] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_15_ (mux_2level_tapbuf_size4_15_inbus, grid_1__2__pin_0__2__12_, mux_2level_tapbuf_size4_15_sram_blwl_out[204:207] ,
mux_2level_tapbuf_size4_15_sram_blwl_outb[204:207] );
//----- SRAM bits for MUX[15], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_204_ (mux_2level_tapbuf_size4_15_sram_blwl_out[204:204] ,mux_2level_tapbuf_size4_15_sram_blwl_out[204:204] ,mux_2level_tapbuf_size4_15_sram_blwl_outb[204:204] ,mux_2level_tapbuf_size4_15_configbus0[204:204], mux_2level_tapbuf_size4_15_configbus1[204:204] , mux_2level_tapbuf_size4_15_configbus0_b[204:204] );
sram6T_blwl sram_blwl_205_ (mux_2level_tapbuf_size4_15_sram_blwl_out[205:205] ,mux_2level_tapbuf_size4_15_sram_blwl_out[205:205] ,mux_2level_tapbuf_size4_15_sram_blwl_outb[205:205] ,mux_2level_tapbuf_size4_15_configbus0[205:205], mux_2level_tapbuf_size4_15_configbus1[205:205] , mux_2level_tapbuf_size4_15_configbus0_b[205:205] );
sram6T_blwl sram_blwl_206_ (mux_2level_tapbuf_size4_15_sram_blwl_out[206:206] ,mux_2level_tapbuf_size4_15_sram_blwl_out[206:206] ,mux_2level_tapbuf_size4_15_sram_blwl_outb[206:206] ,mux_2level_tapbuf_size4_15_configbus0[206:206], mux_2level_tapbuf_size4_15_configbus1[206:206] , mux_2level_tapbuf_size4_15_configbus0_b[206:206] );
sram6T_blwl sram_blwl_207_ (mux_2level_tapbuf_size4_15_sram_blwl_out[207:207] ,mux_2level_tapbuf_size4_15_sram_blwl_out[207:207] ,mux_2level_tapbuf_size4_15_sram_blwl_outb[207:207] ,mux_2level_tapbuf_size4_15_configbus0[207:207], mux_2level_tapbuf_size4_15_configbus1[207:207] , mux_2level_tapbuf_size4_15_configbus0_b[207:207] );
wire [0:3] mux_2level_tapbuf_size4_16_inbus;
assign mux_2level_tapbuf_size4_16_inbus[0] = chanx_1__1__midout_12_;
assign mux_2level_tapbuf_size4_16_inbus[1] = chanx_1__1__midout_13_;
assign mux_2level_tapbuf_size4_16_inbus[2] = chanx_1__1__midout_28_;
assign mux_2level_tapbuf_size4_16_inbus[3] = chanx_1__1__midout_29_;
wire [208:211] mux_2level_tapbuf_size4_16_configbus0;
wire [208:211] mux_2level_tapbuf_size4_16_configbus1;
wire [208:211] mux_2level_tapbuf_size4_16_sram_blwl_out ;
wire [208:211] mux_2level_tapbuf_size4_16_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_16_configbus0[208:211] = sram_blwl_bl[208:211] ;
assign mux_2level_tapbuf_size4_16_configbus1[208:211] = sram_blwl_wl[208:211] ;
wire [208:211] mux_2level_tapbuf_size4_16_configbus0_b;
assign mux_2level_tapbuf_size4_16_configbus0_b[208:211] = sram_blwl_blb[208:211] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_16_ (mux_2level_tapbuf_size4_16_inbus, grid_1__2__pin_0__2__14_, mux_2level_tapbuf_size4_16_sram_blwl_out[208:211] ,
mux_2level_tapbuf_size4_16_sram_blwl_outb[208:211] );
//----- SRAM bits for MUX[16], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_208_ (mux_2level_tapbuf_size4_16_sram_blwl_out[208:208] ,mux_2level_tapbuf_size4_16_sram_blwl_out[208:208] ,mux_2level_tapbuf_size4_16_sram_blwl_outb[208:208] ,mux_2level_tapbuf_size4_16_configbus0[208:208], mux_2level_tapbuf_size4_16_configbus1[208:208] , mux_2level_tapbuf_size4_16_configbus0_b[208:208] );
sram6T_blwl sram_blwl_209_ (mux_2level_tapbuf_size4_16_sram_blwl_out[209:209] ,mux_2level_tapbuf_size4_16_sram_blwl_out[209:209] ,mux_2level_tapbuf_size4_16_sram_blwl_outb[209:209] ,mux_2level_tapbuf_size4_16_configbus0[209:209], mux_2level_tapbuf_size4_16_configbus1[209:209] , mux_2level_tapbuf_size4_16_configbus0_b[209:209] );
sram6T_blwl sram_blwl_210_ (mux_2level_tapbuf_size4_16_sram_blwl_out[210:210] ,mux_2level_tapbuf_size4_16_sram_blwl_out[210:210] ,mux_2level_tapbuf_size4_16_sram_blwl_outb[210:210] ,mux_2level_tapbuf_size4_16_configbus0[210:210], mux_2level_tapbuf_size4_16_configbus1[210:210] , mux_2level_tapbuf_size4_16_configbus0_b[210:210] );
sram6T_blwl sram_blwl_211_ (mux_2level_tapbuf_size4_16_sram_blwl_out[211:211] ,mux_2level_tapbuf_size4_16_sram_blwl_out[211:211] ,mux_2level_tapbuf_size4_16_sram_blwl_outb[211:211] ,mux_2level_tapbuf_size4_16_configbus0[211:211], mux_2level_tapbuf_size4_16_configbus1[211:211] , mux_2level_tapbuf_size4_16_configbus0_b[211:211] );
wire [0:3] mux_2level_tapbuf_size4_17_inbus;
assign mux_2level_tapbuf_size4_17_inbus[0] = chanx_1__1__midout_6_;
assign mux_2level_tapbuf_size4_17_inbus[1] = chanx_1__1__midout_7_;
assign mux_2level_tapbuf_size4_17_inbus[2] = chanx_1__1__midout_12_;
assign mux_2level_tapbuf_size4_17_inbus[3] = chanx_1__1__midout_13_;
wire [212:215] mux_2level_tapbuf_size4_17_configbus0;
wire [212:215] mux_2level_tapbuf_size4_17_configbus1;
wire [212:215] mux_2level_tapbuf_size4_17_sram_blwl_out ;
wire [212:215] mux_2level_tapbuf_size4_17_sram_blwl_outb ;
assign mux_2level_tapbuf_size4_17_configbus0[212:215] = sram_blwl_bl[212:215] ;
assign mux_2level_tapbuf_size4_17_configbus1[212:215] = sram_blwl_wl[212:215] ;
wire [212:215] mux_2level_tapbuf_size4_17_configbus0_b;
assign mux_2level_tapbuf_size4_17_configbus0_b[212:215] = sram_blwl_blb[212:215] ;
mux_2level_tapbuf_size4 mux_2level_tapbuf_size4_17_ (mux_2level_tapbuf_size4_17_inbus, grid_1__1__pin_0__0__0_, mux_2level_tapbuf_size4_17_sram_blwl_out[212:215] ,
mux_2level_tapbuf_size4_17_sram_blwl_outb[212:215] );
//----- SRAM bits for MUX[17], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1010-----
sram6T_blwl sram_blwl_212_ (mux_2level_tapbuf_size4_17_sram_blwl_out[212:212] ,mux_2level_tapbuf_size4_17_sram_blwl_out[212:212] ,mux_2level_tapbuf_size4_17_sram_blwl_outb[212:212] ,mux_2level_tapbuf_size4_17_configbus0[212:212], mux_2level_tapbuf_size4_17_configbus1[212:212] , mux_2level_tapbuf_size4_17_configbus0_b[212:212] );
sram6T_blwl sram_blwl_213_ (mux_2level_tapbuf_size4_17_sram_blwl_out[213:213] ,mux_2level_tapbuf_size4_17_sram_blwl_out[213:213] ,mux_2level_tapbuf_size4_17_sram_blwl_outb[213:213] ,mux_2level_tapbuf_size4_17_configbus0[213:213], mux_2level_tapbuf_size4_17_configbus1[213:213] , mux_2level_tapbuf_size4_17_configbus0_b[213:213] );
sram6T_blwl sram_blwl_214_ (mux_2level_tapbuf_size4_17_sram_blwl_out[214:214] ,mux_2level_tapbuf_size4_17_sram_blwl_out[214:214] ,mux_2level_tapbuf_size4_17_sram_blwl_outb[214:214] ,mux_2level_tapbuf_size4_17_configbus0[214:214], mux_2level_tapbuf_size4_17_configbus1[214:214] , mux_2level_tapbuf_size4_17_configbus0_b[214:214] );
sram6T_blwl sram_blwl_215_ (mux_2level_tapbuf_size4_17_sram_blwl_out[215:215] ,mux_2level_tapbuf_size4_17_sram_blwl_out[215:215] ,mux_2level_tapbuf_size4_17_sram_blwl_outb[215:215] ,mux_2level_tapbuf_size4_17_configbus0[215:215], mux_2level_tapbuf_size4_17_configbus1[215:215] , mux_2level_tapbuf_size4_17_configbus0_b[215:215] );
endmodule
//----- END Verilog Module of Connection Box -X direction [1][1] -----

