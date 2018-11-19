//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Logic Block  [1][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Grid[1][1] type_descriptor: clb[0] -----
//----- LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4__lut4_0_ -----
module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4__lut4_0_ (
input wire lut4_0___in_0_,
input wire lut4_0___in_1_,
input wire lut4_0___in_2_,
input wire lut4_0___in_3_,
output wire lut4_0___out_0_,
input [288:303] sram_blwl_bl ,
input [288:303] sram_blwl_wl ,
input [288:303] sram_blwl_blb );
wire [0:3] lut4_0___in;
assign lut4_0___in[0] = lut4_0___in_0_;
assign lut4_0___in[1] = lut4_0___in_1_;
assign lut4_0___in[2] = lut4_0___in_2_;
assign lut4_0___in[3] = lut4_0___in_3_;
wire [0:0] lut4_0___out;
assign lut4_0___out_0_ = lut4_0___out[0];
wire [288:303] sram_blwl_out ;
wire [288:303] sram_blwl_outb ;
lut4 lut4_0_ (
//----- Input and output ports -----
 lut4_0___in[0:3] ,  lut4_0___out[0:0],//----- SRAM ports -----
sram_blwl_out[288:303] , sram_blwl_outb[288:303] );
//----- Truth Table for LUT node (n7). -----
//----- Truth Table for LUT[0], size=4. -----
//  0--- 1 
//----- SRAM bits for LUT[0], size=4, num_sram=16. -----
//-----0101010101010101-----
wire [288:288] sram_blwl_288_configbus0;
wire [288:288] sram_blwl_288_configbus1;
wire [288:288] sram_blwl_288_configbus0_b;
assign sram_blwl_288_configbus0[288:288] = sram_blwl_bl[288:288] ;
assign sram_blwl_288_configbus1[288:288] = sram_blwl_wl[288:288] ;
assign sram_blwl_288_configbus0_b[288:288] = sram_blwl_blb[288:288] ;
sram6T_blwl sram_blwl_288_ (sram_blwl_out[288], sram_blwl_out[288], sram_blwl_outb[288], sram_blwl_288_configbus0[288:288], sram_blwl_288_configbus1[288:288] , sram_blwl_288_configbus0_b[288:288] );
wire [289:289] sram_blwl_289_configbus0;
wire [289:289] sram_blwl_289_configbus1;
wire [289:289] sram_blwl_289_configbus0_b;
assign sram_blwl_289_configbus0[289:289] = sram_blwl_bl[289:289] ;
assign sram_blwl_289_configbus1[289:289] = sram_blwl_wl[289:289] ;
assign sram_blwl_289_configbus0_b[289:289] = sram_blwl_blb[289:289] ;
sram6T_blwl sram_blwl_289_ (sram_blwl_out[289], sram_blwl_out[289], sram_blwl_outb[289], sram_blwl_289_configbus0[289:289], sram_blwl_289_configbus1[289:289] , sram_blwl_289_configbus0_b[289:289] );
wire [290:290] sram_blwl_290_configbus0;
wire [290:290] sram_blwl_290_configbus1;
wire [290:290] sram_blwl_290_configbus0_b;
assign sram_blwl_290_configbus0[290:290] = sram_blwl_bl[290:290] ;
assign sram_blwl_290_configbus1[290:290] = sram_blwl_wl[290:290] ;
assign sram_blwl_290_configbus0_b[290:290] = sram_blwl_blb[290:290] ;
sram6T_blwl sram_blwl_290_ (sram_blwl_out[290], sram_blwl_out[290], sram_blwl_outb[290], sram_blwl_290_configbus0[290:290], sram_blwl_290_configbus1[290:290] , sram_blwl_290_configbus0_b[290:290] );
wire [291:291] sram_blwl_291_configbus0;
wire [291:291] sram_blwl_291_configbus1;
wire [291:291] sram_blwl_291_configbus0_b;
assign sram_blwl_291_configbus0[291:291] = sram_blwl_bl[291:291] ;
assign sram_blwl_291_configbus1[291:291] = sram_blwl_wl[291:291] ;
assign sram_blwl_291_configbus0_b[291:291] = sram_blwl_blb[291:291] ;
sram6T_blwl sram_blwl_291_ (sram_blwl_out[291], sram_blwl_out[291], sram_blwl_outb[291], sram_blwl_291_configbus0[291:291], sram_blwl_291_configbus1[291:291] , sram_blwl_291_configbus0_b[291:291] );
wire [292:292] sram_blwl_292_configbus0;
wire [292:292] sram_blwl_292_configbus1;
wire [292:292] sram_blwl_292_configbus0_b;
assign sram_blwl_292_configbus0[292:292] = sram_blwl_bl[292:292] ;
assign sram_blwl_292_configbus1[292:292] = sram_blwl_wl[292:292] ;
assign sram_blwl_292_configbus0_b[292:292] = sram_blwl_blb[292:292] ;
sram6T_blwl sram_blwl_292_ (sram_blwl_out[292], sram_blwl_out[292], sram_blwl_outb[292], sram_blwl_292_configbus0[292:292], sram_blwl_292_configbus1[292:292] , sram_blwl_292_configbus0_b[292:292] );
wire [293:293] sram_blwl_293_configbus0;
wire [293:293] sram_blwl_293_configbus1;
wire [293:293] sram_blwl_293_configbus0_b;
assign sram_blwl_293_configbus0[293:293] = sram_blwl_bl[293:293] ;
assign sram_blwl_293_configbus1[293:293] = sram_blwl_wl[293:293] ;
assign sram_blwl_293_configbus0_b[293:293] = sram_blwl_blb[293:293] ;
sram6T_blwl sram_blwl_293_ (sram_blwl_out[293], sram_blwl_out[293], sram_blwl_outb[293], sram_blwl_293_configbus0[293:293], sram_blwl_293_configbus1[293:293] , sram_blwl_293_configbus0_b[293:293] );
wire [294:294] sram_blwl_294_configbus0;
wire [294:294] sram_blwl_294_configbus1;
wire [294:294] sram_blwl_294_configbus0_b;
assign sram_blwl_294_configbus0[294:294] = sram_blwl_bl[294:294] ;
assign sram_blwl_294_configbus1[294:294] = sram_blwl_wl[294:294] ;
assign sram_blwl_294_configbus0_b[294:294] = sram_blwl_blb[294:294] ;
sram6T_blwl sram_blwl_294_ (sram_blwl_out[294], sram_blwl_out[294], sram_blwl_outb[294], sram_blwl_294_configbus0[294:294], sram_blwl_294_configbus1[294:294] , sram_blwl_294_configbus0_b[294:294] );
wire [295:295] sram_blwl_295_configbus0;
wire [295:295] sram_blwl_295_configbus1;
wire [295:295] sram_blwl_295_configbus0_b;
assign sram_blwl_295_configbus0[295:295] = sram_blwl_bl[295:295] ;
assign sram_blwl_295_configbus1[295:295] = sram_blwl_wl[295:295] ;
assign sram_blwl_295_configbus0_b[295:295] = sram_blwl_blb[295:295] ;
sram6T_blwl sram_blwl_295_ (sram_blwl_out[295], sram_blwl_out[295], sram_blwl_outb[295], sram_blwl_295_configbus0[295:295], sram_blwl_295_configbus1[295:295] , sram_blwl_295_configbus0_b[295:295] );
wire [296:296] sram_blwl_296_configbus0;
wire [296:296] sram_blwl_296_configbus1;
wire [296:296] sram_blwl_296_configbus0_b;
assign sram_blwl_296_configbus0[296:296] = sram_blwl_bl[296:296] ;
assign sram_blwl_296_configbus1[296:296] = sram_blwl_wl[296:296] ;
assign sram_blwl_296_configbus0_b[296:296] = sram_blwl_blb[296:296] ;
sram6T_blwl sram_blwl_296_ (sram_blwl_out[296], sram_blwl_out[296], sram_blwl_outb[296], sram_blwl_296_configbus0[296:296], sram_blwl_296_configbus1[296:296] , sram_blwl_296_configbus0_b[296:296] );
wire [297:297] sram_blwl_297_configbus0;
wire [297:297] sram_blwl_297_configbus1;
wire [297:297] sram_blwl_297_configbus0_b;
assign sram_blwl_297_configbus0[297:297] = sram_blwl_bl[297:297] ;
assign sram_blwl_297_configbus1[297:297] = sram_blwl_wl[297:297] ;
assign sram_blwl_297_configbus0_b[297:297] = sram_blwl_blb[297:297] ;
sram6T_blwl sram_blwl_297_ (sram_blwl_out[297], sram_blwl_out[297], sram_blwl_outb[297], sram_blwl_297_configbus0[297:297], sram_blwl_297_configbus1[297:297] , sram_blwl_297_configbus0_b[297:297] );
wire [298:298] sram_blwl_298_configbus0;
wire [298:298] sram_blwl_298_configbus1;
wire [298:298] sram_blwl_298_configbus0_b;
assign sram_blwl_298_configbus0[298:298] = sram_blwl_bl[298:298] ;
assign sram_blwl_298_configbus1[298:298] = sram_blwl_wl[298:298] ;
assign sram_blwl_298_configbus0_b[298:298] = sram_blwl_blb[298:298] ;
sram6T_blwl sram_blwl_298_ (sram_blwl_out[298], sram_blwl_out[298], sram_blwl_outb[298], sram_blwl_298_configbus0[298:298], sram_blwl_298_configbus1[298:298] , sram_blwl_298_configbus0_b[298:298] );
wire [299:299] sram_blwl_299_configbus0;
wire [299:299] sram_blwl_299_configbus1;
wire [299:299] sram_blwl_299_configbus0_b;
assign sram_blwl_299_configbus0[299:299] = sram_blwl_bl[299:299] ;
assign sram_blwl_299_configbus1[299:299] = sram_blwl_wl[299:299] ;
assign sram_blwl_299_configbus0_b[299:299] = sram_blwl_blb[299:299] ;
sram6T_blwl sram_blwl_299_ (sram_blwl_out[299], sram_blwl_out[299], sram_blwl_outb[299], sram_blwl_299_configbus0[299:299], sram_blwl_299_configbus1[299:299] , sram_blwl_299_configbus0_b[299:299] );
wire [300:300] sram_blwl_300_configbus0;
wire [300:300] sram_blwl_300_configbus1;
wire [300:300] sram_blwl_300_configbus0_b;
assign sram_blwl_300_configbus0[300:300] = sram_blwl_bl[300:300] ;
assign sram_blwl_300_configbus1[300:300] = sram_blwl_wl[300:300] ;
assign sram_blwl_300_configbus0_b[300:300] = sram_blwl_blb[300:300] ;
sram6T_blwl sram_blwl_300_ (sram_blwl_out[300], sram_blwl_out[300], sram_blwl_outb[300], sram_blwl_300_configbus0[300:300], sram_blwl_300_configbus1[300:300] , sram_blwl_300_configbus0_b[300:300] );
wire [301:301] sram_blwl_301_configbus0;
wire [301:301] sram_blwl_301_configbus1;
wire [301:301] sram_blwl_301_configbus0_b;
assign sram_blwl_301_configbus0[301:301] = sram_blwl_bl[301:301] ;
assign sram_blwl_301_configbus1[301:301] = sram_blwl_wl[301:301] ;
assign sram_blwl_301_configbus0_b[301:301] = sram_blwl_blb[301:301] ;
sram6T_blwl sram_blwl_301_ (sram_blwl_out[301], sram_blwl_out[301], sram_blwl_outb[301], sram_blwl_301_configbus0[301:301], sram_blwl_301_configbus1[301:301] , sram_blwl_301_configbus0_b[301:301] );
wire [302:302] sram_blwl_302_configbus0;
wire [302:302] sram_blwl_302_configbus1;
wire [302:302] sram_blwl_302_configbus0_b;
assign sram_blwl_302_configbus0[302:302] = sram_blwl_bl[302:302] ;
assign sram_blwl_302_configbus1[302:302] = sram_blwl_wl[302:302] ;
assign sram_blwl_302_configbus0_b[302:302] = sram_blwl_blb[302:302] ;
sram6T_blwl sram_blwl_302_ (sram_blwl_out[302], sram_blwl_out[302], sram_blwl_outb[302], sram_blwl_302_configbus0[302:302], sram_blwl_302_configbus1[302:302] , sram_blwl_302_configbus0_b[302:302] );
wire [303:303] sram_blwl_303_configbus0;
wire [303:303] sram_blwl_303_configbus1;
wire [303:303] sram_blwl_303_configbus0_b;
assign sram_blwl_303_configbus0[303:303] = sram_blwl_bl[303:303] ;
assign sram_blwl_303_configbus1[303:303] = sram_blwl_wl[303:303] ;
assign sram_blwl_303_configbus0_b[303:303] = sram_blwl_blb[303:303] ;
sram6T_blwl sram_blwl_303_ (sram_blwl_out[303], sram_blwl_out[303], sram_blwl_outb[303], sram_blwl_303_configbus0[303:303], sram_blwl_303_configbus1[303:303] , sram_blwl_303_configbus0_b[303:303] );
endmodule
//----- END LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4__lut4_0_ -----

//----- Flip-flop Verilog module: Q0 -----
//----- Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4__ff_0_ -----
module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4__ff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
input [0:0] Set,
input [0:0] Reset,
input [0:0] clk
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
input wire ff_0___D_0_,
output wire ff_0___Q_0_);
static_dff dff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
endmodule
//----- END Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4__ff_0_ -----

//----- Programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4_ -----
module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_ble4___in_0_,
input wire mode_ble4___in_1_,
input wire mode_ble4___in_2_,
input wire mode_ble4___in_3_,
output wire mode_ble4___out_0_,
input wire mode_ble4___clk_0_,
input [288:304] sram_blwl_bl ,
input [288:304] sram_blwl_wl ,
input [288:304] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4__lut4_0_ lut4_0_ (
 lut4_0___in_0_,  lut4_0___in_1_,  lut4_0___in_2_,  lut4_0___in_3_,  lut4_0___out_0_,
sram_blwl_bl[288:303] ,
sram_blwl_wl[288:303] ,
sram_blwl_blb[288:303] ); 
grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4__ff_0_ ff_0_ (
//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_); 
wire [0:1] in_bus_mux_1level_tapbuf_size2_120_ ;
assign in_bus_mux_1level_tapbuf_size2_120_[0] = ff_0___Q_0_ ; 
assign in_bus_mux_1level_tapbuf_size2_120_[1] = lut4_0___out_0_ ; 
wire [304:304] mux_1level_tapbuf_size2_120_configbus0;
wire [304:304] mux_1level_tapbuf_size2_120_configbus1;
wire [304:304] mux_1level_tapbuf_size2_120_sram_blwl_out ;
wire [304:304] mux_1level_tapbuf_size2_120_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_120_configbus0[304:304] = sram_blwl_bl[304:304] ;
assign mux_1level_tapbuf_size2_120_configbus1[304:304] = sram_blwl_wl[304:304] ;
wire [304:304] mux_1level_tapbuf_size2_120_configbus0_b;
assign mux_1level_tapbuf_size2_120_configbus0_b[304:304] = sram_blwl_blb[304:304] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_120_ (in_bus_mux_1level_tapbuf_size2_120_, mode_ble4___out_0_, mux_1level_tapbuf_size2_120_sram_blwl_out[304:304] ,
mux_1level_tapbuf_size2_120_sram_blwl_outb[304:304] );
//----- SRAM bits for MUX[120], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_304_ (mux_1level_tapbuf_size2_120_sram_blwl_out[304:304] ,mux_1level_tapbuf_size2_120_sram_blwl_out[304:304] ,mux_1level_tapbuf_size2_120_sram_blwl_outb[304:304] ,mux_1level_tapbuf_size2_120_configbus0[304:304], mux_1level_tapbuf_size2_120_configbus1[304:304] , mux_1level_tapbuf_size2_120_configbus0_b[304:304] );
direct_interc direct_interc_0_ (mode_ble4___in_0_, lut4_0___in_0_ );
direct_interc direct_interc_1_ (mode_ble4___in_1_, lut4_0___in_1_ );
direct_interc direct_interc_2_ (mode_ble4___in_2_, lut4_0___in_2_ );
direct_interc direct_interc_3_ (mode_ble4___in_3_, lut4_0___in_3_ );
direct_interc direct_interc_4_ (lut4_0___out_0_, ff_0___D_0_ );
direct_interc direct_interc_5_ (mode_ble4___clk_0_, ff_0___clk_0_ );
endmodule
//----- END Programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4_ -----

//----- Programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4_ -----
module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_n1_lut4___in_0_,
input wire mode_n1_lut4___in_1_,
input wire mode_n1_lut4___in_2_,
input wire mode_n1_lut4___in_3_,
output wire mode_n1_lut4___out_0_,
input wire mode_n1_lut4___clk_0_,
input [288:304] sram_blwl_bl ,
input [288:304] sram_blwl_wl ,
input [288:304] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4__ble4_0__mode_ble4_ ble4_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 ble4_0___in_0_,  ble4_0___in_1_,  ble4_0___in_2_,  ble4_0___in_3_,  ble4_0___out_0_,  ble4_0___clk_0_,
sram_blwl_bl[288:304] ,
sram_blwl_wl[288:304] ,
sram_blwl_blb[288:304] ); 
direct_interc direct_interc_6_ (ble4_0___out_0_, mode_n1_lut4___out_0_ );
direct_interc direct_interc_7_ (mode_n1_lut4___in_0_, ble4_0___in_0_ );
direct_interc direct_interc_8_ (mode_n1_lut4___in_1_, ble4_0___in_1_ );
direct_interc direct_interc_9_ (mode_n1_lut4___in_2_, ble4_0___in_2_ );
direct_interc direct_interc_10_ (mode_n1_lut4___in_3_, ble4_0___in_3_ );
direct_interc direct_interc_11_ (mode_n1_lut4___clk_0_, ble4_0___clk_0_ );
endmodule
//----- END Programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4_ -----

//----- Programmable logic block Verilog module grid_1__1__clb_0__mode_clb_ -----
module grid_1__1__clb_0__mode_clb_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_clb___I_0_,
input wire mode_clb___I_1_,
input wire mode_clb___I_2_,
input wire mode_clb___I_3_,
output wire mode_clb___O_0_,
input wire mode_clb___clk_0_,
input [288:328] sram_blwl_bl ,
input [288:328] sram_blwl_wl ,
input [288:328] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut4_ fle_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 fle_0___in_0_,  fle_0___in_1_,  fle_0___in_2_,  fle_0___in_3_,  fle_0___out_0_,  fle_0___clk_0_,
sram_blwl_bl[288:304] ,
sram_blwl_wl[288:304] ,
sram_blwl_blb[288:304] ); 
direct_interc direct_interc_12_ (fle_0___out_0_, mode_clb___O_0_ );
wire [0:4] in_bus_mux_2level_size5_0_ ;
assign in_bus_mux_2level_size5_0_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size5_0_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size5_0_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size5_0_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size5_0_[4] = fle_0___out_0_ ; 
wire [305:310] mux_2level_size5_0_configbus0;
wire [305:310] mux_2level_size5_0_configbus1;
wire [305:310] mux_2level_size5_0_sram_blwl_out ;
wire [305:310] mux_2level_size5_0_sram_blwl_outb ;
assign mux_2level_size5_0_configbus0[305:310] = sram_blwl_bl[305:310] ;
assign mux_2level_size5_0_configbus1[305:310] = sram_blwl_wl[305:310] ;
wire [305:310] mux_2level_size5_0_configbus0_b;
assign mux_2level_size5_0_configbus0_b[305:310] = sram_blwl_blb[305:310] ;
mux_2level_size5 mux_2level_size5_0_ (in_bus_mux_2level_size5_0_, fle_0___in_0_, mux_2level_size5_0_sram_blwl_out[305:310] ,
mux_2level_size5_0_sram_blwl_outb[305:310] );
//----- SRAM bits for MUX[0], level=2, select_path_id=3. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----010100-----
sram6T_blwl sram_blwl_305_ (mux_2level_size5_0_sram_blwl_out[305:305] ,mux_2level_size5_0_sram_blwl_out[305:305] ,mux_2level_size5_0_sram_blwl_outb[305:305] ,mux_2level_size5_0_configbus0[305:305], mux_2level_size5_0_configbus1[305:305] , mux_2level_size5_0_configbus0_b[305:305] );
sram6T_blwl sram_blwl_306_ (mux_2level_size5_0_sram_blwl_out[306:306] ,mux_2level_size5_0_sram_blwl_out[306:306] ,mux_2level_size5_0_sram_blwl_outb[306:306] ,mux_2level_size5_0_configbus0[306:306], mux_2level_size5_0_configbus1[306:306] , mux_2level_size5_0_configbus0_b[306:306] );
sram6T_blwl sram_blwl_307_ (mux_2level_size5_0_sram_blwl_out[307:307] ,mux_2level_size5_0_sram_blwl_out[307:307] ,mux_2level_size5_0_sram_blwl_outb[307:307] ,mux_2level_size5_0_configbus0[307:307], mux_2level_size5_0_configbus1[307:307] , mux_2level_size5_0_configbus0_b[307:307] );
sram6T_blwl sram_blwl_308_ (mux_2level_size5_0_sram_blwl_out[308:308] ,mux_2level_size5_0_sram_blwl_out[308:308] ,mux_2level_size5_0_sram_blwl_outb[308:308] ,mux_2level_size5_0_configbus0[308:308], mux_2level_size5_0_configbus1[308:308] , mux_2level_size5_0_configbus0_b[308:308] );
sram6T_blwl sram_blwl_309_ (mux_2level_size5_0_sram_blwl_out[309:309] ,mux_2level_size5_0_sram_blwl_out[309:309] ,mux_2level_size5_0_sram_blwl_outb[309:309] ,mux_2level_size5_0_configbus0[309:309], mux_2level_size5_0_configbus1[309:309] , mux_2level_size5_0_configbus0_b[309:309] );
sram6T_blwl sram_blwl_310_ (mux_2level_size5_0_sram_blwl_out[310:310] ,mux_2level_size5_0_sram_blwl_out[310:310] ,mux_2level_size5_0_sram_blwl_outb[310:310] ,mux_2level_size5_0_configbus0[310:310], mux_2level_size5_0_configbus1[310:310] , mux_2level_size5_0_configbus0_b[310:310] );
wire [0:4] in_bus_mux_2level_size5_1_ ;
assign in_bus_mux_2level_size5_1_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size5_1_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size5_1_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size5_1_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size5_1_[4] = fle_0___out_0_ ; 
wire [311:316] mux_2level_size5_1_configbus0;
wire [311:316] mux_2level_size5_1_configbus1;
wire [311:316] mux_2level_size5_1_sram_blwl_out ;
wire [311:316] mux_2level_size5_1_sram_blwl_outb ;
assign mux_2level_size5_1_configbus0[311:316] = sram_blwl_bl[311:316] ;
assign mux_2level_size5_1_configbus1[311:316] = sram_blwl_wl[311:316] ;
wire [311:316] mux_2level_size5_1_configbus0_b;
assign mux_2level_size5_1_configbus0_b[311:316] = sram_blwl_blb[311:316] ;
mux_2level_size5 mux_2level_size5_1_ (in_bus_mux_2level_size5_1_, fle_0___in_1_, mux_2level_size5_1_sram_blwl_out[311:316] ,
mux_2level_size5_1_sram_blwl_outb[311:316] );
//----- SRAM bits for MUX[1], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100100-----
sram6T_blwl sram_blwl_311_ (mux_2level_size5_1_sram_blwl_out[311:311] ,mux_2level_size5_1_sram_blwl_out[311:311] ,mux_2level_size5_1_sram_blwl_outb[311:311] ,mux_2level_size5_1_configbus0[311:311], mux_2level_size5_1_configbus1[311:311] , mux_2level_size5_1_configbus0_b[311:311] );
sram6T_blwl sram_blwl_312_ (mux_2level_size5_1_sram_blwl_out[312:312] ,mux_2level_size5_1_sram_blwl_out[312:312] ,mux_2level_size5_1_sram_blwl_outb[312:312] ,mux_2level_size5_1_configbus0[312:312], mux_2level_size5_1_configbus1[312:312] , mux_2level_size5_1_configbus0_b[312:312] );
sram6T_blwl sram_blwl_313_ (mux_2level_size5_1_sram_blwl_out[313:313] ,mux_2level_size5_1_sram_blwl_out[313:313] ,mux_2level_size5_1_sram_blwl_outb[313:313] ,mux_2level_size5_1_configbus0[313:313], mux_2level_size5_1_configbus1[313:313] , mux_2level_size5_1_configbus0_b[313:313] );
sram6T_blwl sram_blwl_314_ (mux_2level_size5_1_sram_blwl_out[314:314] ,mux_2level_size5_1_sram_blwl_out[314:314] ,mux_2level_size5_1_sram_blwl_outb[314:314] ,mux_2level_size5_1_configbus0[314:314], mux_2level_size5_1_configbus1[314:314] , mux_2level_size5_1_configbus0_b[314:314] );
sram6T_blwl sram_blwl_315_ (mux_2level_size5_1_sram_blwl_out[315:315] ,mux_2level_size5_1_sram_blwl_out[315:315] ,mux_2level_size5_1_sram_blwl_outb[315:315] ,mux_2level_size5_1_configbus0[315:315], mux_2level_size5_1_configbus1[315:315] , mux_2level_size5_1_configbus0_b[315:315] );
sram6T_blwl sram_blwl_316_ (mux_2level_size5_1_sram_blwl_out[316:316] ,mux_2level_size5_1_sram_blwl_out[316:316] ,mux_2level_size5_1_sram_blwl_outb[316:316] ,mux_2level_size5_1_configbus0[316:316], mux_2level_size5_1_configbus1[316:316] , mux_2level_size5_1_configbus0_b[316:316] );
wire [0:4] in_bus_mux_2level_size5_2_ ;
assign in_bus_mux_2level_size5_2_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size5_2_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size5_2_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size5_2_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size5_2_[4] = fle_0___out_0_ ; 
wire [317:322] mux_2level_size5_2_configbus0;
wire [317:322] mux_2level_size5_2_configbus1;
wire [317:322] mux_2level_size5_2_sram_blwl_out ;
wire [317:322] mux_2level_size5_2_sram_blwl_outb ;
assign mux_2level_size5_2_configbus0[317:322] = sram_blwl_bl[317:322] ;
assign mux_2level_size5_2_configbus1[317:322] = sram_blwl_wl[317:322] ;
wire [317:322] mux_2level_size5_2_configbus0_b;
assign mux_2level_size5_2_configbus0_b[317:322] = sram_blwl_blb[317:322] ;
mux_2level_size5 mux_2level_size5_2_ (in_bus_mux_2level_size5_2_, fle_0___in_2_, mux_2level_size5_2_sram_blwl_out[317:322] ,
mux_2level_size5_2_sram_blwl_outb[317:322] );
//----- SRAM bits for MUX[2], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100100-----
sram6T_blwl sram_blwl_317_ (mux_2level_size5_2_sram_blwl_out[317:317] ,mux_2level_size5_2_sram_blwl_out[317:317] ,mux_2level_size5_2_sram_blwl_outb[317:317] ,mux_2level_size5_2_configbus0[317:317], mux_2level_size5_2_configbus1[317:317] , mux_2level_size5_2_configbus0_b[317:317] );
sram6T_blwl sram_blwl_318_ (mux_2level_size5_2_sram_blwl_out[318:318] ,mux_2level_size5_2_sram_blwl_out[318:318] ,mux_2level_size5_2_sram_blwl_outb[318:318] ,mux_2level_size5_2_configbus0[318:318], mux_2level_size5_2_configbus1[318:318] , mux_2level_size5_2_configbus0_b[318:318] );
sram6T_blwl sram_blwl_319_ (mux_2level_size5_2_sram_blwl_out[319:319] ,mux_2level_size5_2_sram_blwl_out[319:319] ,mux_2level_size5_2_sram_blwl_outb[319:319] ,mux_2level_size5_2_configbus0[319:319], mux_2level_size5_2_configbus1[319:319] , mux_2level_size5_2_configbus0_b[319:319] );
sram6T_blwl sram_blwl_320_ (mux_2level_size5_2_sram_blwl_out[320:320] ,mux_2level_size5_2_sram_blwl_out[320:320] ,mux_2level_size5_2_sram_blwl_outb[320:320] ,mux_2level_size5_2_configbus0[320:320], mux_2level_size5_2_configbus1[320:320] , mux_2level_size5_2_configbus0_b[320:320] );
sram6T_blwl sram_blwl_321_ (mux_2level_size5_2_sram_blwl_out[321:321] ,mux_2level_size5_2_sram_blwl_out[321:321] ,mux_2level_size5_2_sram_blwl_outb[321:321] ,mux_2level_size5_2_configbus0[321:321], mux_2level_size5_2_configbus1[321:321] , mux_2level_size5_2_configbus0_b[321:321] );
sram6T_blwl sram_blwl_322_ (mux_2level_size5_2_sram_blwl_out[322:322] ,mux_2level_size5_2_sram_blwl_out[322:322] ,mux_2level_size5_2_sram_blwl_outb[322:322] ,mux_2level_size5_2_configbus0[322:322], mux_2level_size5_2_configbus1[322:322] , mux_2level_size5_2_configbus0_b[322:322] );
wire [0:4] in_bus_mux_2level_size5_3_ ;
assign in_bus_mux_2level_size5_3_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size5_3_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size5_3_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size5_3_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size5_3_[4] = fle_0___out_0_ ; 
wire [323:328] mux_2level_size5_3_configbus0;
wire [323:328] mux_2level_size5_3_configbus1;
wire [323:328] mux_2level_size5_3_sram_blwl_out ;
wire [323:328] mux_2level_size5_3_sram_blwl_outb ;
assign mux_2level_size5_3_configbus0[323:328] = sram_blwl_bl[323:328] ;
assign mux_2level_size5_3_configbus1[323:328] = sram_blwl_wl[323:328] ;
wire [323:328] mux_2level_size5_3_configbus0_b;
assign mux_2level_size5_3_configbus0_b[323:328] = sram_blwl_blb[323:328] ;
mux_2level_size5 mux_2level_size5_3_ (in_bus_mux_2level_size5_3_, fle_0___in_3_, mux_2level_size5_3_sram_blwl_out[323:328] ,
mux_2level_size5_3_sram_blwl_outb[323:328] );
//----- SRAM bits for MUX[3], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----100100-----
sram6T_blwl sram_blwl_323_ (mux_2level_size5_3_sram_blwl_out[323:323] ,mux_2level_size5_3_sram_blwl_out[323:323] ,mux_2level_size5_3_sram_blwl_outb[323:323] ,mux_2level_size5_3_configbus0[323:323], mux_2level_size5_3_configbus1[323:323] , mux_2level_size5_3_configbus0_b[323:323] );
sram6T_blwl sram_blwl_324_ (mux_2level_size5_3_sram_blwl_out[324:324] ,mux_2level_size5_3_sram_blwl_out[324:324] ,mux_2level_size5_3_sram_blwl_outb[324:324] ,mux_2level_size5_3_configbus0[324:324], mux_2level_size5_3_configbus1[324:324] , mux_2level_size5_3_configbus0_b[324:324] );
sram6T_blwl sram_blwl_325_ (mux_2level_size5_3_sram_blwl_out[325:325] ,mux_2level_size5_3_sram_blwl_out[325:325] ,mux_2level_size5_3_sram_blwl_outb[325:325] ,mux_2level_size5_3_configbus0[325:325], mux_2level_size5_3_configbus1[325:325] , mux_2level_size5_3_configbus0_b[325:325] );
sram6T_blwl sram_blwl_326_ (mux_2level_size5_3_sram_blwl_out[326:326] ,mux_2level_size5_3_sram_blwl_out[326:326] ,mux_2level_size5_3_sram_blwl_outb[326:326] ,mux_2level_size5_3_configbus0[326:326], mux_2level_size5_3_configbus1[326:326] , mux_2level_size5_3_configbus0_b[326:326] );
sram6T_blwl sram_blwl_327_ (mux_2level_size5_3_sram_blwl_out[327:327] ,mux_2level_size5_3_sram_blwl_out[327:327] ,mux_2level_size5_3_sram_blwl_outb[327:327] ,mux_2level_size5_3_configbus0[327:327], mux_2level_size5_3_configbus1[327:327] , mux_2level_size5_3_configbus0_b[327:327] );
sram6T_blwl sram_blwl_328_ (mux_2level_size5_3_sram_blwl_out[328:328] ,mux_2level_size5_3_sram_blwl_out[328:328] ,mux_2level_size5_3_sram_blwl_outb[328:328] ,mux_2level_size5_3_configbus0[328:328], mux_2level_size5_3_configbus1[328:328] , mux_2level_size5_3_configbus0_b[328:328] );
direct_interc direct_interc_13_ (mode_clb___clk_0_, fle_0___clk_0_ );
endmodule
//----- END Programmable logic block Verilog module grid_1__1__clb_0__mode_clb_ -----

//----- END -----

//----- Grid[1][1], Capactity: 1 -----
//----- Top Protocol -----
module grid_1__1_( 

//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input  top_height_0__pin_0_,
output  top_height_0__pin_4_,
input  right_height_0__pin_1_,
input  right_height_0__pin_5_,
input  bottom_height_0__pin_2_,
input  left_height_0__pin_3_,
input [288:328] sram_blwl_bl ,
input [288:328] sram_blwl_wl ,
input [288:328] sram_blwl_blb );
grid_1__1__clb_0__mode_clb_  grid_1__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
top_height_0__pin_0_ ,
right_height_0__pin_1_ ,
bottom_height_0__pin_2_ ,
left_height_0__pin_3_ ,
top_height_0__pin_4_ ,
right_height_0__pin_5_ 
//---- IOPAD ----
,
//---- SRAM ----
sram_blwl_bl[288:328] ,
sram_blwl_wl[288:328] ,
sram_blwl_blb[288:328] );
endmodule
//----- END Top Protocol -----
//----- END Grid[1][1], Capactity: 1 -----

