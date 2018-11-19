//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Physical Logic Block  [0][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Grid[0][1] type_descriptor: io[0] -----
//----- IO Verilog module: grid_0__1__io_0__mode_io_phy__iopad_0_ -----
module grid_0__1__io_0__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [0:0] gfpga_pad_iopad
,
input [329:329] sram_blwl_bl ,
input [329:329] sram_blwl_wl ,
input [329:329] sram_blwl_blb );
wire [329:329] sram_blwl_out ;
wire [329:329] sram_blwl_outb ;
wire [329:329] sram_blwl_329_configbus0;
wire [329:329] sram_blwl_329_configbus1;
wire [329:329] sram_blwl_329_configbus0_b;
assign sram_blwl_329_configbus0[329:329] = sram_blwl_bl[329:329] ;
assign sram_blwl_329_configbus1[329:329] = sram_blwl_wl[329:329] ;
assign sram_blwl_329_configbus0_b[329:329] = sram_blwl_blb[329:329] ;
iopad iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[0], sram_blwl_out[329:329] , sram_blwl_outb[329:329] );
sram6T_blwl sram_blwl_329_ (sram_blwl_out[329], sram_blwl_out[329], sram_blwl_outb[329], sram_blwl_329_configbus0[329:329], sram_blwl_329_configbus1[329:329] , sram_blwl_329_configbus0_b[329:329] );
endmodule
//----- END IO Verilog module: grid_0__1__io_0__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_0__1__io_0__mode_io_phy_ -----
module grid_0__1__io_0__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [0:0] gfpga_pad_iopad ,
input [329:329] sram_blwl_bl ,
input [329:329] sram_blwl_wl ,
input [329:329] sram_blwl_blb );
grid_0__1__io_0__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[0:0] ,
sram_blwl_bl[329:329] ,
sram_blwl_wl[329:329] ,
sram_blwl_blb[329:329] );
direct_interc direct_interc_14_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_15_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_0__1__io_0__mode_io_phy_ -----

//----- END -----

//----- Grid[0][1] type_descriptor: io[1] -----
//----- IO Verilog module: grid_0__1__io_1__mode_io_phy__iopad_0_ -----
module grid_0__1__io_1__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [1:1] gfpga_pad_iopad
,
input [330:330] sram_blwl_bl ,
input [330:330] sram_blwl_wl ,
input [330:330] sram_blwl_blb );
wire [330:330] sram_blwl_out ;
wire [330:330] sram_blwl_outb ;
wire [330:330] sram_blwl_330_configbus0;
wire [330:330] sram_blwl_330_configbus1;
wire [330:330] sram_blwl_330_configbus0_b;
assign sram_blwl_330_configbus0[330:330] = sram_blwl_bl[330:330] ;
assign sram_blwl_330_configbus1[330:330] = sram_blwl_wl[330:330] ;
assign sram_blwl_330_configbus0_b[330:330] = sram_blwl_blb[330:330] ;
iopad iopad_1_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[1], sram_blwl_out[330:330] , sram_blwl_outb[330:330] );
sram6T_blwl sram_blwl_330_ (sram_blwl_out[330], sram_blwl_out[330], sram_blwl_outb[330], sram_blwl_330_configbus0[330:330], sram_blwl_330_configbus1[330:330] , sram_blwl_330_configbus0_b[330:330] );
endmodule
//----- END IO Verilog module: grid_0__1__io_1__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_0__1__io_1__mode_io_phy_ -----
module grid_0__1__io_1__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [1:1] gfpga_pad_iopad ,
input [330:330] sram_blwl_bl ,
input [330:330] sram_blwl_wl ,
input [330:330] sram_blwl_blb );
grid_0__1__io_1__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[1:1] ,
sram_blwl_bl[330:330] ,
sram_blwl_wl[330:330] ,
sram_blwl_blb[330:330] );
direct_interc direct_interc_16_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_17_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_0__1__io_1__mode_io_phy_ -----

//----- END -----

//----- Grid[0][1] type_descriptor: io[2] -----
//----- IO Verilog module: grid_0__1__io_2__mode_io_phy__iopad_0_ -----
module grid_0__1__io_2__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [2:2] gfpga_pad_iopad
,
input [331:331] sram_blwl_bl ,
input [331:331] sram_blwl_wl ,
input [331:331] sram_blwl_blb );
wire [331:331] sram_blwl_out ;
wire [331:331] sram_blwl_outb ;
wire [331:331] sram_blwl_331_configbus0;
wire [331:331] sram_blwl_331_configbus1;
wire [331:331] sram_blwl_331_configbus0_b;
assign sram_blwl_331_configbus0[331:331] = sram_blwl_bl[331:331] ;
assign sram_blwl_331_configbus1[331:331] = sram_blwl_wl[331:331] ;
assign sram_blwl_331_configbus0_b[331:331] = sram_blwl_blb[331:331] ;
iopad iopad_2_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[2], sram_blwl_out[331:331] , sram_blwl_outb[331:331] );
sram6T_blwl sram_blwl_331_ (sram_blwl_out[331], sram_blwl_out[331], sram_blwl_outb[331], sram_blwl_331_configbus0[331:331], sram_blwl_331_configbus1[331:331] , sram_blwl_331_configbus0_b[331:331] );
endmodule
//----- END IO Verilog module: grid_0__1__io_2__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_0__1__io_2__mode_io_phy_ -----
module grid_0__1__io_2__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [2:2] gfpga_pad_iopad ,
input [331:331] sram_blwl_bl ,
input [331:331] sram_blwl_wl ,
input [331:331] sram_blwl_blb );
grid_0__1__io_2__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[2:2] ,
sram_blwl_bl[331:331] ,
sram_blwl_wl[331:331] ,
sram_blwl_blb[331:331] );
direct_interc direct_interc_18_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_19_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_0__1__io_2__mode_io_phy_ -----

//----- END -----

//----- Grid[0][1] type_descriptor: io[3] -----
//----- IO Verilog module: grid_0__1__io_3__mode_io_phy__iopad_0_ -----
module grid_0__1__io_3__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [3:3] gfpga_pad_iopad
,
input [332:332] sram_blwl_bl ,
input [332:332] sram_blwl_wl ,
input [332:332] sram_blwl_blb );
wire [332:332] sram_blwl_out ;
wire [332:332] sram_blwl_outb ;
wire [332:332] sram_blwl_332_configbus0;
wire [332:332] sram_blwl_332_configbus1;
wire [332:332] sram_blwl_332_configbus0_b;
assign sram_blwl_332_configbus0[332:332] = sram_blwl_bl[332:332] ;
assign sram_blwl_332_configbus1[332:332] = sram_blwl_wl[332:332] ;
assign sram_blwl_332_configbus0_b[332:332] = sram_blwl_blb[332:332] ;
iopad iopad_3_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[3], sram_blwl_out[332:332] , sram_blwl_outb[332:332] );
sram6T_blwl sram_blwl_332_ (sram_blwl_out[332], sram_blwl_out[332], sram_blwl_outb[332], sram_blwl_332_configbus0[332:332], sram_blwl_332_configbus1[332:332] , sram_blwl_332_configbus0_b[332:332] );
endmodule
//----- END IO Verilog module: grid_0__1__io_3__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_0__1__io_3__mode_io_phy_ -----
module grid_0__1__io_3__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [3:3] gfpga_pad_iopad ,
input [332:332] sram_blwl_bl ,
input [332:332] sram_blwl_wl ,
input [332:332] sram_blwl_blb );
grid_0__1__io_3__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[3:3] ,
sram_blwl_bl[332:332] ,
sram_blwl_wl[332:332] ,
sram_blwl_blb[332:332] );
direct_interc direct_interc_20_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_21_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_0__1__io_3__mode_io_phy_ -----

//----- END -----

//----- Grid[0][1] type_descriptor: io[4] -----
//----- IO Verilog module: grid_0__1__io_4__mode_io_phy__iopad_0_ -----
module grid_0__1__io_4__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [4:4] gfpga_pad_iopad
,
input [333:333] sram_blwl_bl ,
input [333:333] sram_blwl_wl ,
input [333:333] sram_blwl_blb );
wire [333:333] sram_blwl_out ;
wire [333:333] sram_blwl_outb ;
wire [333:333] sram_blwl_333_configbus0;
wire [333:333] sram_blwl_333_configbus1;
wire [333:333] sram_blwl_333_configbus0_b;
assign sram_blwl_333_configbus0[333:333] = sram_blwl_bl[333:333] ;
assign sram_blwl_333_configbus1[333:333] = sram_blwl_wl[333:333] ;
assign sram_blwl_333_configbus0_b[333:333] = sram_blwl_blb[333:333] ;
iopad iopad_4_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[4], sram_blwl_out[333:333] , sram_blwl_outb[333:333] );
sram6T_blwl sram_blwl_333_ (sram_blwl_out[333], sram_blwl_out[333], sram_blwl_outb[333], sram_blwl_333_configbus0[333:333], sram_blwl_333_configbus1[333:333] , sram_blwl_333_configbus0_b[333:333] );
endmodule
//----- END IO Verilog module: grid_0__1__io_4__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_0__1__io_4__mode_io_phy_ -----
module grid_0__1__io_4__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [4:4] gfpga_pad_iopad ,
input [333:333] sram_blwl_bl ,
input [333:333] sram_blwl_wl ,
input [333:333] sram_blwl_blb );
grid_0__1__io_4__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[4:4] ,
sram_blwl_bl[333:333] ,
sram_blwl_wl[333:333] ,
sram_blwl_blb[333:333] );
direct_interc direct_interc_22_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_23_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_0__1__io_4__mode_io_phy_ -----

//----- END -----

//----- Grid[0][1] type_descriptor: io[5] -----
//----- IO Verilog module: grid_0__1__io_5__mode_io_phy__iopad_0_ -----
module grid_0__1__io_5__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [5:5] gfpga_pad_iopad
,
input [334:334] sram_blwl_bl ,
input [334:334] sram_blwl_wl ,
input [334:334] sram_blwl_blb );
wire [334:334] sram_blwl_out ;
wire [334:334] sram_blwl_outb ;
wire [334:334] sram_blwl_334_configbus0;
wire [334:334] sram_blwl_334_configbus1;
wire [334:334] sram_blwl_334_configbus0_b;
assign sram_blwl_334_configbus0[334:334] = sram_blwl_bl[334:334] ;
assign sram_blwl_334_configbus1[334:334] = sram_blwl_wl[334:334] ;
assign sram_blwl_334_configbus0_b[334:334] = sram_blwl_blb[334:334] ;
iopad iopad_5_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[5], sram_blwl_out[334:334] , sram_blwl_outb[334:334] );
sram6T_blwl sram_blwl_334_ (sram_blwl_out[334], sram_blwl_out[334], sram_blwl_outb[334], sram_blwl_334_configbus0[334:334], sram_blwl_334_configbus1[334:334] , sram_blwl_334_configbus0_b[334:334] );
endmodule
//----- END IO Verilog module: grid_0__1__io_5__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_0__1__io_5__mode_io_phy_ -----
module grid_0__1__io_5__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [5:5] gfpga_pad_iopad ,
input [334:334] sram_blwl_bl ,
input [334:334] sram_blwl_wl ,
input [334:334] sram_blwl_blb );
grid_0__1__io_5__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[5:5] ,
sram_blwl_bl[334:334] ,
sram_blwl_wl[334:334] ,
sram_blwl_blb[334:334] );
direct_interc direct_interc_24_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_25_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_0__1__io_5__mode_io_phy_ -----

//----- END -----

//----- Grid[0][1] type_descriptor: io[6] -----
//----- IO Verilog module: grid_0__1__io_6__mode_io_phy__iopad_0_ -----
module grid_0__1__io_6__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [6:6] gfpga_pad_iopad
,
input [335:335] sram_blwl_bl ,
input [335:335] sram_blwl_wl ,
input [335:335] sram_blwl_blb );
wire [335:335] sram_blwl_out ;
wire [335:335] sram_blwl_outb ;
wire [335:335] sram_blwl_335_configbus0;
wire [335:335] sram_blwl_335_configbus1;
wire [335:335] sram_blwl_335_configbus0_b;
assign sram_blwl_335_configbus0[335:335] = sram_blwl_bl[335:335] ;
assign sram_blwl_335_configbus1[335:335] = sram_blwl_wl[335:335] ;
assign sram_blwl_335_configbus0_b[335:335] = sram_blwl_blb[335:335] ;
iopad iopad_6_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[6], sram_blwl_out[335:335] , sram_blwl_outb[335:335] );
sram6T_blwl sram_blwl_335_ (sram_blwl_out[335], sram_blwl_out[335], sram_blwl_outb[335], sram_blwl_335_configbus0[335:335], sram_blwl_335_configbus1[335:335] , sram_blwl_335_configbus0_b[335:335] );
endmodule
//----- END IO Verilog module: grid_0__1__io_6__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_0__1__io_6__mode_io_phy_ -----
module grid_0__1__io_6__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [6:6] gfpga_pad_iopad ,
input [335:335] sram_blwl_bl ,
input [335:335] sram_blwl_wl ,
input [335:335] sram_blwl_blb );
grid_0__1__io_6__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[6:6] ,
sram_blwl_bl[335:335] ,
sram_blwl_wl[335:335] ,
sram_blwl_blb[335:335] );
direct_interc direct_interc_26_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_27_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_0__1__io_6__mode_io_phy_ -----

//----- END -----

//----- Grid[0][1] type_descriptor: io[7] -----
//----- IO Verilog module: grid_0__1__io_7__mode_io_phy__iopad_0_ -----
module grid_0__1__io_7__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [7:7] gfpga_pad_iopad
,
input [336:336] sram_blwl_bl ,
input [336:336] sram_blwl_wl ,
input [336:336] sram_blwl_blb );
wire [336:336] sram_blwl_out ;
wire [336:336] sram_blwl_outb ;
wire [336:336] sram_blwl_336_configbus0;
wire [336:336] sram_blwl_336_configbus1;
wire [336:336] sram_blwl_336_configbus0_b;
assign sram_blwl_336_configbus0[336:336] = sram_blwl_bl[336:336] ;
assign sram_blwl_336_configbus1[336:336] = sram_blwl_wl[336:336] ;
assign sram_blwl_336_configbus0_b[336:336] = sram_blwl_blb[336:336] ;
iopad iopad_7_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[7], sram_blwl_out[336:336] , sram_blwl_outb[336:336] );
sram6T_blwl sram_blwl_336_ (sram_blwl_out[336], sram_blwl_out[336], sram_blwl_outb[336], sram_blwl_336_configbus0[336:336], sram_blwl_336_configbus1[336:336] , sram_blwl_336_configbus0_b[336:336] );
endmodule
//----- END IO Verilog module: grid_0__1__io_7__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_0__1__io_7__mode_io_phy_ -----
module grid_0__1__io_7__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [7:7] gfpga_pad_iopad ,
input [336:336] sram_blwl_bl ,
input [336:336] sram_blwl_wl ,
input [336:336] sram_blwl_blb );
grid_0__1__io_7__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[7:7] ,
sram_blwl_bl[336:336] ,
sram_blwl_wl[336:336] ,
sram_blwl_blb[336:336] );
direct_interc direct_interc_28_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_29_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_0__1__io_7__mode_io_phy_ -----

//----- END -----

//----- Grid[0][1], Capactity: 8 -----
//----- Top Protocol -----
module grid_0__1_( 

//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input  right_height_0__pin_0_,
output  right_height_0__pin_1_,
input  right_height_0__pin_2_,
output  right_height_0__pin_3_,
input  right_height_0__pin_4_,
output  right_height_0__pin_5_,
input  right_height_0__pin_6_,
output  right_height_0__pin_7_,
input  right_height_0__pin_8_,
output  right_height_0__pin_9_,
input  right_height_0__pin_10_,
output  right_height_0__pin_11_,
input  right_height_0__pin_12_,
output  right_height_0__pin_13_,
input  right_height_0__pin_14_,
output  right_height_0__pin_15_,
input [7:0] gfpga_pad_iopad ,
input [329:336] sram_blwl_bl ,
input [329:336] sram_blwl_wl ,
input [329:336] sram_blwl_blb );
grid_0__1__io_0__mode_io_phy_  grid_0__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
right_height_0__pin_0_,
right_height_0__pin_1_
//---- IOPAD ----
,
gfpga_pad_iopad[0:0] ,
//---- SRAM ----
sram_blwl_bl[329:329] ,
sram_blwl_wl[329:329] ,
sram_blwl_blb[329:329] );
grid_0__1__io_1__mode_io_phy_  grid_0__1__1_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
right_height_0__pin_2_,
right_height_0__pin_3_
//---- IOPAD ----
,
gfpga_pad_iopad[1:1] ,
//---- SRAM ----
sram_blwl_bl[330:330] ,
sram_blwl_wl[330:330] ,
sram_blwl_blb[330:330] );
grid_0__1__io_2__mode_io_phy_  grid_0__1__2_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
right_height_0__pin_4_,
right_height_0__pin_5_
//---- IOPAD ----
,
gfpga_pad_iopad[2:2] ,
//---- SRAM ----
sram_blwl_bl[331:331] ,
sram_blwl_wl[331:331] ,
sram_blwl_blb[331:331] );
grid_0__1__io_3__mode_io_phy_  grid_0__1__3_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
right_height_0__pin_6_,
right_height_0__pin_7_
//---- IOPAD ----
,
gfpga_pad_iopad[3:3] ,
//---- SRAM ----
sram_blwl_bl[332:332] ,
sram_blwl_wl[332:332] ,
sram_blwl_blb[332:332] );
grid_0__1__io_4__mode_io_phy_  grid_0__1__4_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
right_height_0__pin_8_,
right_height_0__pin_9_
//---- IOPAD ----
,
gfpga_pad_iopad[4:4] ,
//---- SRAM ----
sram_blwl_bl[333:333] ,
sram_blwl_wl[333:333] ,
sram_blwl_blb[333:333] );
grid_0__1__io_5__mode_io_phy_  grid_0__1__5_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
right_height_0__pin_10_,
right_height_0__pin_11_
//---- IOPAD ----
,
gfpga_pad_iopad[5:5] ,
//---- SRAM ----
sram_blwl_bl[334:334] ,
sram_blwl_wl[334:334] ,
sram_blwl_blb[334:334] );
grid_0__1__io_6__mode_io_phy_  grid_0__1__6_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
right_height_0__pin_12_,
right_height_0__pin_13_
//---- IOPAD ----
,
gfpga_pad_iopad[6:6] ,
//---- SRAM ----
sram_blwl_bl[335:335] ,
sram_blwl_wl[335:335] ,
sram_blwl_blb[335:335] );
grid_0__1__io_7__mode_io_phy_  grid_0__1__7_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
right_height_0__pin_14_,
right_height_0__pin_15_
//---- IOPAD ----
,
gfpga_pad_iopad[7:7] ,
//---- SRAM ----
sram_blwl_bl[336:336] ,
sram_blwl_wl[336:336] ,
sram_blwl_blb[336:336] );
endmodule
//----- END Top Protocol -----
//----- END Grid[0][1], Capactity: 8 -----

