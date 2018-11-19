//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Physical Logic Block  [1][0] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Grid[1][0] type_descriptor: io[0] -----
//----- IO Verilog module: grid_1__0__io_0__mode_io_phy__iopad_0_ -----
module grid_1__0__io_0__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [16:16] gfpga_pad_iopad
,
input [345:345] sram_blwl_bl ,
input [345:345] sram_blwl_wl ,
input [345:345] sram_blwl_blb );
wire [345:345] sram_blwl_out ;
wire [345:345] sram_blwl_outb ;
wire [345:345] sram_blwl_345_configbus0;
wire [345:345] sram_blwl_345_configbus1;
wire [345:345] sram_blwl_345_configbus0_b;
assign sram_blwl_345_configbus0[345:345] = sram_blwl_bl[345:345] ;
assign sram_blwl_345_configbus1[345:345] = sram_blwl_wl[345:345] ;
assign sram_blwl_345_configbus0_b[345:345] = sram_blwl_blb[345:345] ;
iopad iopad_16_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[16], sram_blwl_out[345:345] , sram_blwl_outb[345:345] );
sram6T_blwl sram_blwl_345_ (sram_blwl_out[345], sram_blwl_out[345], sram_blwl_outb[345], sram_blwl_345_configbus0[345:345], sram_blwl_345_configbus1[345:345] , sram_blwl_345_configbus0_b[345:345] );
endmodule
//----- END IO Verilog module: grid_1__0__io_0__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__0__io_0__mode_io_phy_ -----
module grid_1__0__io_0__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [16:16] gfpga_pad_iopad ,
input [345:345] sram_blwl_bl ,
input [345:345] sram_blwl_wl ,
input [345:345] sram_blwl_blb );
grid_1__0__io_0__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[16:16] ,
sram_blwl_bl[345:345] ,
sram_blwl_wl[345:345] ,
sram_blwl_blb[345:345] );
direct_interc direct_interc_46_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_47_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__0__io_0__mode_io_phy_ -----

//----- END -----

//----- Grid[1][0] type_descriptor: io[1] -----
//----- IO Verilog module: grid_1__0__io_1__mode_io_phy__iopad_0_ -----
module grid_1__0__io_1__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [17:17] gfpga_pad_iopad
,
input [346:346] sram_blwl_bl ,
input [346:346] sram_blwl_wl ,
input [346:346] sram_blwl_blb );
wire [346:346] sram_blwl_out ;
wire [346:346] sram_blwl_outb ;
wire [346:346] sram_blwl_346_configbus0;
wire [346:346] sram_blwl_346_configbus1;
wire [346:346] sram_blwl_346_configbus0_b;
assign sram_blwl_346_configbus0[346:346] = sram_blwl_bl[346:346] ;
assign sram_blwl_346_configbus1[346:346] = sram_blwl_wl[346:346] ;
assign sram_blwl_346_configbus0_b[346:346] = sram_blwl_blb[346:346] ;
iopad iopad_17_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[17], sram_blwl_out[346:346] , sram_blwl_outb[346:346] );
sram6T_blwl sram_blwl_346_ (sram_blwl_out[346], sram_blwl_out[346], sram_blwl_outb[346], sram_blwl_346_configbus0[346:346], sram_blwl_346_configbus1[346:346] , sram_blwl_346_configbus0_b[346:346] );
endmodule
//----- END IO Verilog module: grid_1__0__io_1__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__0__io_1__mode_io_phy_ -----
module grid_1__0__io_1__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [17:17] gfpga_pad_iopad ,
input [346:346] sram_blwl_bl ,
input [346:346] sram_blwl_wl ,
input [346:346] sram_blwl_blb );
grid_1__0__io_1__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[17:17] ,
sram_blwl_bl[346:346] ,
sram_blwl_wl[346:346] ,
sram_blwl_blb[346:346] );
direct_interc direct_interc_48_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_49_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__0__io_1__mode_io_phy_ -----

//----- END -----

//----- Grid[1][0] type_descriptor: io[2] -----
//----- IO Verilog module: grid_1__0__io_2__mode_io_phy__iopad_0_ -----
module grid_1__0__io_2__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [18:18] gfpga_pad_iopad
,
input [347:347] sram_blwl_bl ,
input [347:347] sram_blwl_wl ,
input [347:347] sram_blwl_blb );
wire [347:347] sram_blwl_out ;
wire [347:347] sram_blwl_outb ;
wire [347:347] sram_blwl_347_configbus0;
wire [347:347] sram_blwl_347_configbus1;
wire [347:347] sram_blwl_347_configbus0_b;
assign sram_blwl_347_configbus0[347:347] = sram_blwl_bl[347:347] ;
assign sram_blwl_347_configbus1[347:347] = sram_blwl_wl[347:347] ;
assign sram_blwl_347_configbus0_b[347:347] = sram_blwl_blb[347:347] ;
iopad iopad_18_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[18], sram_blwl_out[347:347] , sram_blwl_outb[347:347] );
sram6T_blwl sram_blwl_347_ (sram_blwl_out[347], sram_blwl_out[347], sram_blwl_outb[347], sram_blwl_347_configbus0[347:347], sram_blwl_347_configbus1[347:347] , sram_blwl_347_configbus0_b[347:347] );
endmodule
//----- END IO Verilog module: grid_1__0__io_2__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__0__io_2__mode_io_phy_ -----
module grid_1__0__io_2__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [18:18] gfpga_pad_iopad ,
input [347:347] sram_blwl_bl ,
input [347:347] sram_blwl_wl ,
input [347:347] sram_blwl_blb );
grid_1__0__io_2__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[18:18] ,
sram_blwl_bl[347:347] ,
sram_blwl_wl[347:347] ,
sram_blwl_blb[347:347] );
direct_interc direct_interc_50_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_51_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__0__io_2__mode_io_phy_ -----

//----- END -----

//----- Grid[1][0] type_descriptor: io[3] -----
//----- IO Verilog module: grid_1__0__io_3__mode_io_phy__iopad_0_ -----
module grid_1__0__io_3__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [19:19] gfpga_pad_iopad
,
input [348:348] sram_blwl_bl ,
input [348:348] sram_blwl_wl ,
input [348:348] sram_blwl_blb );
wire [348:348] sram_blwl_out ;
wire [348:348] sram_blwl_outb ;
wire [348:348] sram_blwl_348_configbus0;
wire [348:348] sram_blwl_348_configbus1;
wire [348:348] sram_blwl_348_configbus0_b;
assign sram_blwl_348_configbus0[348:348] = sram_blwl_bl[348:348] ;
assign sram_blwl_348_configbus1[348:348] = sram_blwl_wl[348:348] ;
assign sram_blwl_348_configbus0_b[348:348] = sram_blwl_blb[348:348] ;
iopad iopad_19_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[19], sram_blwl_out[348:348] , sram_blwl_outb[348:348] );
sram6T_blwl sram_blwl_348_ (sram_blwl_out[348], sram_blwl_out[348], sram_blwl_outb[348], sram_blwl_348_configbus0[348:348], sram_blwl_348_configbus1[348:348] , sram_blwl_348_configbus0_b[348:348] );
endmodule
//----- END IO Verilog module: grid_1__0__io_3__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__0__io_3__mode_io_phy_ -----
module grid_1__0__io_3__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [19:19] gfpga_pad_iopad ,
input [348:348] sram_blwl_bl ,
input [348:348] sram_blwl_wl ,
input [348:348] sram_blwl_blb );
grid_1__0__io_3__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[19:19] ,
sram_blwl_bl[348:348] ,
sram_blwl_wl[348:348] ,
sram_blwl_blb[348:348] );
direct_interc direct_interc_52_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_53_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__0__io_3__mode_io_phy_ -----

//----- END -----

//----- Grid[1][0] type_descriptor: io[4] -----
//----- IO Verilog module: grid_1__0__io_4__mode_io_phy__iopad_0_ -----
module grid_1__0__io_4__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [20:20] gfpga_pad_iopad
,
input [349:349] sram_blwl_bl ,
input [349:349] sram_blwl_wl ,
input [349:349] sram_blwl_blb );
wire [349:349] sram_blwl_out ;
wire [349:349] sram_blwl_outb ;
wire [349:349] sram_blwl_349_configbus0;
wire [349:349] sram_blwl_349_configbus1;
wire [349:349] sram_blwl_349_configbus0_b;
assign sram_blwl_349_configbus0[349:349] = sram_blwl_bl[349:349] ;
assign sram_blwl_349_configbus1[349:349] = sram_blwl_wl[349:349] ;
assign sram_blwl_349_configbus0_b[349:349] = sram_blwl_blb[349:349] ;
iopad iopad_20_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[20], sram_blwl_out[349:349] , sram_blwl_outb[349:349] );
sram6T_blwl sram_blwl_349_ (sram_blwl_out[349], sram_blwl_out[349], sram_blwl_outb[349], sram_blwl_349_configbus0[349:349], sram_blwl_349_configbus1[349:349] , sram_blwl_349_configbus0_b[349:349] );
endmodule
//----- END IO Verilog module: grid_1__0__io_4__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__0__io_4__mode_io_phy_ -----
module grid_1__0__io_4__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [20:20] gfpga_pad_iopad ,
input [349:349] sram_blwl_bl ,
input [349:349] sram_blwl_wl ,
input [349:349] sram_blwl_blb );
grid_1__0__io_4__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[20:20] ,
sram_blwl_bl[349:349] ,
sram_blwl_wl[349:349] ,
sram_blwl_blb[349:349] );
direct_interc direct_interc_54_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_55_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__0__io_4__mode_io_phy_ -----

//----- END -----

//----- Grid[1][0] type_descriptor: io[5] -----
//----- IO Verilog module: grid_1__0__io_5__mode_io_phy__iopad_0_ -----
module grid_1__0__io_5__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [21:21] gfpga_pad_iopad
,
input [350:350] sram_blwl_bl ,
input [350:350] sram_blwl_wl ,
input [350:350] sram_blwl_blb );
wire [350:350] sram_blwl_out ;
wire [350:350] sram_blwl_outb ;
wire [350:350] sram_blwl_350_configbus0;
wire [350:350] sram_blwl_350_configbus1;
wire [350:350] sram_blwl_350_configbus0_b;
assign sram_blwl_350_configbus0[350:350] = sram_blwl_bl[350:350] ;
assign sram_blwl_350_configbus1[350:350] = sram_blwl_wl[350:350] ;
assign sram_blwl_350_configbus0_b[350:350] = sram_blwl_blb[350:350] ;
iopad iopad_21_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[21], sram_blwl_out[350:350] , sram_blwl_outb[350:350] );
sram6T_blwl sram_blwl_350_ (sram_blwl_out[350], sram_blwl_out[350], sram_blwl_outb[350], sram_blwl_350_configbus0[350:350], sram_blwl_350_configbus1[350:350] , sram_blwl_350_configbus0_b[350:350] );
endmodule
//----- END IO Verilog module: grid_1__0__io_5__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__0__io_5__mode_io_phy_ -----
module grid_1__0__io_5__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [21:21] gfpga_pad_iopad ,
input [350:350] sram_blwl_bl ,
input [350:350] sram_blwl_wl ,
input [350:350] sram_blwl_blb );
grid_1__0__io_5__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[21:21] ,
sram_blwl_bl[350:350] ,
sram_blwl_wl[350:350] ,
sram_blwl_blb[350:350] );
direct_interc direct_interc_56_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_57_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__0__io_5__mode_io_phy_ -----

//----- END -----

//----- Grid[1][0] type_descriptor: io[6] -----
//----- IO Verilog module: grid_1__0__io_6__mode_io_phy__iopad_0_ -----
module grid_1__0__io_6__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [22:22] gfpga_pad_iopad
,
input [351:351] sram_blwl_bl ,
input [351:351] sram_blwl_wl ,
input [351:351] sram_blwl_blb );
wire [351:351] sram_blwl_out ;
wire [351:351] sram_blwl_outb ;
wire [351:351] sram_blwl_351_configbus0;
wire [351:351] sram_blwl_351_configbus1;
wire [351:351] sram_blwl_351_configbus0_b;
assign sram_blwl_351_configbus0[351:351] = sram_blwl_bl[351:351] ;
assign sram_blwl_351_configbus1[351:351] = sram_blwl_wl[351:351] ;
assign sram_blwl_351_configbus0_b[351:351] = sram_blwl_blb[351:351] ;
iopad iopad_22_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[22], sram_blwl_out[351:351] , sram_blwl_outb[351:351] );
sram6T_blwl sram_blwl_351_ (sram_blwl_out[351], sram_blwl_out[351], sram_blwl_outb[351], sram_blwl_351_configbus0[351:351], sram_blwl_351_configbus1[351:351] , sram_blwl_351_configbus0_b[351:351] );
endmodule
//----- END IO Verilog module: grid_1__0__io_6__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__0__io_6__mode_io_phy_ -----
module grid_1__0__io_6__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [22:22] gfpga_pad_iopad ,
input [351:351] sram_blwl_bl ,
input [351:351] sram_blwl_wl ,
input [351:351] sram_blwl_blb );
grid_1__0__io_6__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[22:22] ,
sram_blwl_bl[351:351] ,
sram_blwl_wl[351:351] ,
sram_blwl_blb[351:351] );
direct_interc direct_interc_58_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_59_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__0__io_6__mode_io_phy_ -----

//----- END -----

//----- Grid[1][0] type_descriptor: io[7] -----
//----- IO Verilog module: grid_1__0__io_7__mode_io_phy__iopad_0_ -----
module grid_1__0__io_7__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [23:23] gfpga_pad_iopad
,
input [352:352] sram_blwl_bl ,
input [352:352] sram_blwl_wl ,
input [352:352] sram_blwl_blb );
wire [352:352] sram_blwl_out ;
wire [352:352] sram_blwl_outb ;
wire [352:352] sram_blwl_352_configbus0;
wire [352:352] sram_blwl_352_configbus1;
wire [352:352] sram_blwl_352_configbus0_b;
assign sram_blwl_352_configbus0[352:352] = sram_blwl_bl[352:352] ;
assign sram_blwl_352_configbus1[352:352] = sram_blwl_wl[352:352] ;
assign sram_blwl_352_configbus0_b[352:352] = sram_blwl_blb[352:352] ;
iopad iopad_23_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[23], sram_blwl_out[352:352] , sram_blwl_outb[352:352] );
sram6T_blwl sram_blwl_352_ (sram_blwl_out[352], sram_blwl_out[352], sram_blwl_outb[352], sram_blwl_352_configbus0[352:352], sram_blwl_352_configbus1[352:352] , sram_blwl_352_configbus0_b[352:352] );
endmodule
//----- END IO Verilog module: grid_1__0__io_7__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__0__io_7__mode_io_phy_ -----
module grid_1__0__io_7__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [23:23] gfpga_pad_iopad ,
input [352:352] sram_blwl_bl ,
input [352:352] sram_blwl_wl ,
input [352:352] sram_blwl_blb );
grid_1__0__io_7__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[23:23] ,
sram_blwl_bl[352:352] ,
sram_blwl_wl[352:352] ,
sram_blwl_blb[352:352] );
direct_interc direct_interc_60_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_61_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__0__io_7__mode_io_phy_ -----

//----- END -----

//----- Grid[1][0], Capactity: 8 -----
//----- Top Protocol -----
module grid_1__0_( 

//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input  top_height_0__pin_0_,
output  top_height_0__pin_1_,
input  top_height_0__pin_2_,
output  top_height_0__pin_3_,
input  top_height_0__pin_4_,
output  top_height_0__pin_5_,
input  top_height_0__pin_6_,
output  top_height_0__pin_7_,
input  top_height_0__pin_8_,
output  top_height_0__pin_9_,
input  top_height_0__pin_10_,
output  top_height_0__pin_11_,
input  top_height_0__pin_12_,
output  top_height_0__pin_13_,
input  top_height_0__pin_14_,
output  top_height_0__pin_15_,
input [23:16] gfpga_pad_iopad ,
input [345:352] sram_blwl_bl ,
input [345:352] sram_blwl_wl ,
input [345:352] sram_blwl_blb );
grid_1__0__io_0__mode_io_phy_  grid_1__0__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
top_height_0__pin_0_,
top_height_0__pin_1_
//---- IOPAD ----
,
gfpga_pad_iopad[16:16] ,
//---- SRAM ----
sram_blwl_bl[345:345] ,
sram_blwl_wl[345:345] ,
sram_blwl_blb[345:345] );
grid_1__0__io_1__mode_io_phy_  grid_1__0__1_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
top_height_0__pin_2_,
top_height_0__pin_3_
//---- IOPAD ----
,
gfpga_pad_iopad[17:17] ,
//---- SRAM ----
sram_blwl_bl[346:346] ,
sram_blwl_wl[346:346] ,
sram_blwl_blb[346:346] );
grid_1__0__io_2__mode_io_phy_  grid_1__0__2_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
top_height_0__pin_4_,
top_height_0__pin_5_
//---- IOPAD ----
,
gfpga_pad_iopad[18:18] ,
//---- SRAM ----
sram_blwl_bl[347:347] ,
sram_blwl_wl[347:347] ,
sram_blwl_blb[347:347] );
grid_1__0__io_3__mode_io_phy_  grid_1__0__3_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
top_height_0__pin_6_,
top_height_0__pin_7_
//---- IOPAD ----
,
gfpga_pad_iopad[19:19] ,
//---- SRAM ----
sram_blwl_bl[348:348] ,
sram_blwl_wl[348:348] ,
sram_blwl_blb[348:348] );
grid_1__0__io_4__mode_io_phy_  grid_1__0__4_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
top_height_0__pin_8_,
top_height_0__pin_9_
//---- IOPAD ----
,
gfpga_pad_iopad[20:20] ,
//---- SRAM ----
sram_blwl_bl[349:349] ,
sram_blwl_wl[349:349] ,
sram_blwl_blb[349:349] );
grid_1__0__io_5__mode_io_phy_  grid_1__0__5_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
top_height_0__pin_10_,
top_height_0__pin_11_
//---- IOPAD ----
,
gfpga_pad_iopad[21:21] ,
//---- SRAM ----
sram_blwl_bl[350:350] ,
sram_blwl_wl[350:350] ,
sram_blwl_blb[350:350] );
grid_1__0__io_6__mode_io_phy_  grid_1__0__6_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
top_height_0__pin_12_,
top_height_0__pin_13_
//---- IOPAD ----
,
gfpga_pad_iopad[22:22] ,
//---- SRAM ----
sram_blwl_bl[351:351] ,
sram_blwl_wl[351:351] ,
sram_blwl_blb[351:351] );
grid_1__0__io_7__mode_io_phy_  grid_1__0__7_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
top_height_0__pin_14_,
top_height_0__pin_15_
//---- IOPAD ----
,
gfpga_pad_iopad[23:23] ,
//---- SRAM ----
sram_blwl_bl[352:352] ,
sram_blwl_wl[352:352] ,
sram_blwl_blb[352:352] );
endmodule
//----- END Top Protocol -----
//----- END Grid[1][0], Capactity: 8 -----

