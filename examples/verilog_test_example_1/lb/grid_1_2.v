//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Physical Logic Block  [1][2] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Grid[1][2] type_descriptor: io[0] -----
//----- IO Verilog module: grid_1__2__io_0__mode_io_phy__iopad_0_ -----
module grid_1__2__io_0__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [24:24] gfpga_pad_iopad
,
input [353:353] sram_blwl_bl ,
input [353:353] sram_blwl_wl ,
input [353:353] sram_blwl_blb );
wire [353:353] sram_blwl_out ;
wire [353:353] sram_blwl_outb ;
wire [353:353] sram_blwl_353_configbus0;
wire [353:353] sram_blwl_353_configbus1;
wire [353:353] sram_blwl_353_configbus0_b;
assign sram_blwl_353_configbus0[353:353] = sram_blwl_bl[353:353] ;
assign sram_blwl_353_configbus1[353:353] = sram_blwl_wl[353:353] ;
assign sram_blwl_353_configbus0_b[353:353] = sram_blwl_blb[353:353] ;
iopad iopad_24_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[24], sram_blwl_out[353:353] , sram_blwl_outb[353:353] );
sram6T_blwl sram_blwl_353_ (sram_blwl_out[353], sram_blwl_out[353], sram_blwl_outb[353], sram_blwl_353_configbus0[353:353], sram_blwl_353_configbus1[353:353] , sram_blwl_353_configbus0_b[353:353] );
endmodule
//----- END IO Verilog module: grid_1__2__io_0__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__2__io_0__mode_io_phy_ -----
module grid_1__2__io_0__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [24:24] gfpga_pad_iopad ,
input [353:353] sram_blwl_bl ,
input [353:353] sram_blwl_wl ,
input [353:353] sram_blwl_blb );
grid_1__2__io_0__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[24:24] ,
sram_blwl_bl[353:353] ,
sram_blwl_wl[353:353] ,
sram_blwl_blb[353:353] );
direct_interc direct_interc_62_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_63_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__2__io_0__mode_io_phy_ -----

//----- END -----

//----- Grid[1][2] type_descriptor: io[1] -----
//----- IO Verilog module: grid_1__2__io_1__mode_io_phy__iopad_0_ -----
module grid_1__2__io_1__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [25:25] gfpga_pad_iopad
,
input [354:354] sram_blwl_bl ,
input [354:354] sram_blwl_wl ,
input [354:354] sram_blwl_blb );
wire [354:354] sram_blwl_out ;
wire [354:354] sram_blwl_outb ;
wire [354:354] sram_blwl_354_configbus0;
wire [354:354] sram_blwl_354_configbus1;
wire [354:354] sram_blwl_354_configbus0_b;
assign sram_blwl_354_configbus0[354:354] = sram_blwl_bl[354:354] ;
assign sram_blwl_354_configbus1[354:354] = sram_blwl_wl[354:354] ;
assign sram_blwl_354_configbus0_b[354:354] = sram_blwl_blb[354:354] ;
iopad iopad_25_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[25], sram_blwl_out[354:354] , sram_blwl_outb[354:354] );
sram6T_blwl sram_blwl_354_ (sram_blwl_out[354], sram_blwl_out[354], sram_blwl_outb[354], sram_blwl_354_configbus0[354:354], sram_blwl_354_configbus1[354:354] , sram_blwl_354_configbus0_b[354:354] );
endmodule
//----- END IO Verilog module: grid_1__2__io_1__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__2__io_1__mode_io_phy_ -----
module grid_1__2__io_1__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [25:25] gfpga_pad_iopad ,
input [354:354] sram_blwl_bl ,
input [354:354] sram_blwl_wl ,
input [354:354] sram_blwl_blb );
grid_1__2__io_1__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[25:25] ,
sram_blwl_bl[354:354] ,
sram_blwl_wl[354:354] ,
sram_blwl_blb[354:354] );
direct_interc direct_interc_64_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_65_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__2__io_1__mode_io_phy_ -----

//----- END -----

//----- Grid[1][2] type_descriptor: io[2] -----
//----- IO Verilog module: grid_1__2__io_2__mode_io_phy__iopad_0_ -----
module grid_1__2__io_2__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [26:26] gfpga_pad_iopad
,
input [355:355] sram_blwl_bl ,
input [355:355] sram_blwl_wl ,
input [355:355] sram_blwl_blb );
wire [355:355] sram_blwl_out ;
wire [355:355] sram_blwl_outb ;
wire [355:355] sram_blwl_355_configbus0;
wire [355:355] sram_blwl_355_configbus1;
wire [355:355] sram_blwl_355_configbus0_b;
assign sram_blwl_355_configbus0[355:355] = sram_blwl_bl[355:355] ;
assign sram_blwl_355_configbus1[355:355] = sram_blwl_wl[355:355] ;
assign sram_blwl_355_configbus0_b[355:355] = sram_blwl_blb[355:355] ;
iopad iopad_26_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[26], sram_blwl_out[355:355] , sram_blwl_outb[355:355] );
sram6T_blwl sram_blwl_355_ (sram_blwl_out[355], sram_blwl_out[355], sram_blwl_outb[355], sram_blwl_355_configbus0[355:355], sram_blwl_355_configbus1[355:355] , sram_blwl_355_configbus0_b[355:355] );
endmodule
//----- END IO Verilog module: grid_1__2__io_2__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__2__io_2__mode_io_phy_ -----
module grid_1__2__io_2__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [26:26] gfpga_pad_iopad ,
input [355:355] sram_blwl_bl ,
input [355:355] sram_blwl_wl ,
input [355:355] sram_blwl_blb );
grid_1__2__io_2__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[26:26] ,
sram_blwl_bl[355:355] ,
sram_blwl_wl[355:355] ,
sram_blwl_blb[355:355] );
direct_interc direct_interc_66_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_67_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__2__io_2__mode_io_phy_ -----

//----- END -----

//----- Grid[1][2] type_descriptor: io[3] -----
//----- IO Verilog module: grid_1__2__io_3__mode_io_phy__iopad_0_ -----
module grid_1__2__io_3__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [27:27] gfpga_pad_iopad
,
input [356:356] sram_blwl_bl ,
input [356:356] sram_blwl_wl ,
input [356:356] sram_blwl_blb );
wire [356:356] sram_blwl_out ;
wire [356:356] sram_blwl_outb ;
wire [356:356] sram_blwl_356_configbus0;
wire [356:356] sram_blwl_356_configbus1;
wire [356:356] sram_blwl_356_configbus0_b;
assign sram_blwl_356_configbus0[356:356] = sram_blwl_bl[356:356] ;
assign sram_blwl_356_configbus1[356:356] = sram_blwl_wl[356:356] ;
assign sram_blwl_356_configbus0_b[356:356] = sram_blwl_blb[356:356] ;
iopad iopad_27_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[27], sram_blwl_out[356:356] , sram_blwl_outb[356:356] );
sram6T_blwl sram_blwl_356_ (sram_blwl_out[356], sram_blwl_out[356], sram_blwl_outb[356], sram_blwl_356_configbus0[356:356], sram_blwl_356_configbus1[356:356] , sram_blwl_356_configbus0_b[356:356] );
endmodule
//----- END IO Verilog module: grid_1__2__io_3__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__2__io_3__mode_io_phy_ -----
module grid_1__2__io_3__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [27:27] gfpga_pad_iopad ,
input [356:356] sram_blwl_bl ,
input [356:356] sram_blwl_wl ,
input [356:356] sram_blwl_blb );
grid_1__2__io_3__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[27:27] ,
sram_blwl_bl[356:356] ,
sram_blwl_wl[356:356] ,
sram_blwl_blb[356:356] );
direct_interc direct_interc_68_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_69_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__2__io_3__mode_io_phy_ -----

//----- END -----

//----- Grid[1][2] type_descriptor: io[4] -----
//----- IO Verilog module: grid_1__2__io_4__mode_io_phy__iopad_0_ -----
module grid_1__2__io_4__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [28:28] gfpga_pad_iopad
,
input [357:357] sram_blwl_bl ,
input [357:357] sram_blwl_wl ,
input [357:357] sram_blwl_blb );
wire [357:357] sram_blwl_out ;
wire [357:357] sram_blwl_outb ;
wire [357:357] sram_blwl_357_configbus0;
wire [357:357] sram_blwl_357_configbus1;
wire [357:357] sram_blwl_357_configbus0_b;
assign sram_blwl_357_configbus0[357:357] = sram_blwl_bl[357:357] ;
assign sram_blwl_357_configbus1[357:357] = sram_blwl_wl[357:357] ;
assign sram_blwl_357_configbus0_b[357:357] = sram_blwl_blb[357:357] ;
iopad iopad_28_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[28], sram_blwl_out[357:357] , sram_blwl_outb[357:357] );
sram6T_blwl sram_blwl_357_ (sram_blwl_out[357], sram_blwl_out[357], sram_blwl_outb[357], sram_blwl_357_configbus0[357:357], sram_blwl_357_configbus1[357:357] , sram_blwl_357_configbus0_b[357:357] );
endmodule
//----- END IO Verilog module: grid_1__2__io_4__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__2__io_4__mode_io_phy_ -----
module grid_1__2__io_4__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [28:28] gfpga_pad_iopad ,
input [357:357] sram_blwl_bl ,
input [357:357] sram_blwl_wl ,
input [357:357] sram_blwl_blb );
grid_1__2__io_4__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[28:28] ,
sram_blwl_bl[357:357] ,
sram_blwl_wl[357:357] ,
sram_blwl_blb[357:357] );
direct_interc direct_interc_70_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_71_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__2__io_4__mode_io_phy_ -----

//----- END -----

//----- Grid[1][2] type_descriptor: io[5] -----
//----- IO Verilog module: grid_1__2__io_5__mode_io_phy__iopad_0_ -----
module grid_1__2__io_5__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [29:29] gfpga_pad_iopad
,
input [358:358] sram_blwl_bl ,
input [358:358] sram_blwl_wl ,
input [358:358] sram_blwl_blb );
wire [358:358] sram_blwl_out ;
wire [358:358] sram_blwl_outb ;
wire [358:358] sram_blwl_358_configbus0;
wire [358:358] sram_blwl_358_configbus1;
wire [358:358] sram_blwl_358_configbus0_b;
assign sram_blwl_358_configbus0[358:358] = sram_blwl_bl[358:358] ;
assign sram_blwl_358_configbus1[358:358] = sram_blwl_wl[358:358] ;
assign sram_blwl_358_configbus0_b[358:358] = sram_blwl_blb[358:358] ;
iopad iopad_29_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[29], sram_blwl_out[358:358] , sram_blwl_outb[358:358] );
sram6T_blwl sram_blwl_358_ (sram_blwl_out[358], sram_blwl_out[358], sram_blwl_outb[358], sram_blwl_358_configbus0[358:358], sram_blwl_358_configbus1[358:358] , sram_blwl_358_configbus0_b[358:358] );
endmodule
//----- END IO Verilog module: grid_1__2__io_5__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__2__io_5__mode_io_phy_ -----
module grid_1__2__io_5__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [29:29] gfpga_pad_iopad ,
input [358:358] sram_blwl_bl ,
input [358:358] sram_blwl_wl ,
input [358:358] sram_blwl_blb );
grid_1__2__io_5__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[29:29] ,
sram_blwl_bl[358:358] ,
sram_blwl_wl[358:358] ,
sram_blwl_blb[358:358] );
direct_interc direct_interc_72_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_73_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__2__io_5__mode_io_phy_ -----

//----- END -----

//----- Grid[1][2] type_descriptor: io[6] -----
//----- IO Verilog module: grid_1__2__io_6__mode_io_phy__iopad_0_ -----
module grid_1__2__io_6__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [30:30] gfpga_pad_iopad
,
input [359:359] sram_blwl_bl ,
input [359:359] sram_blwl_wl ,
input [359:359] sram_blwl_blb );
wire [359:359] sram_blwl_out ;
wire [359:359] sram_blwl_outb ;
wire [359:359] sram_blwl_359_configbus0;
wire [359:359] sram_blwl_359_configbus1;
wire [359:359] sram_blwl_359_configbus0_b;
assign sram_blwl_359_configbus0[359:359] = sram_blwl_bl[359:359] ;
assign sram_blwl_359_configbus1[359:359] = sram_blwl_wl[359:359] ;
assign sram_blwl_359_configbus0_b[359:359] = sram_blwl_blb[359:359] ;
iopad iopad_30_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[30], sram_blwl_out[359:359] , sram_blwl_outb[359:359] );
sram6T_blwl sram_blwl_359_ (sram_blwl_out[359], sram_blwl_out[359], sram_blwl_outb[359], sram_blwl_359_configbus0[359:359], sram_blwl_359_configbus1[359:359] , sram_blwl_359_configbus0_b[359:359] );
endmodule
//----- END IO Verilog module: grid_1__2__io_6__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__2__io_6__mode_io_phy_ -----
module grid_1__2__io_6__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [30:30] gfpga_pad_iopad ,
input [359:359] sram_blwl_bl ,
input [359:359] sram_blwl_wl ,
input [359:359] sram_blwl_blb );
grid_1__2__io_6__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[30:30] ,
sram_blwl_bl[359:359] ,
sram_blwl_wl[359:359] ,
sram_blwl_blb[359:359] );
direct_interc direct_interc_74_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_75_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__2__io_6__mode_io_phy_ -----

//----- END -----

//----- Grid[1][2] type_descriptor: io[7] -----
//----- IO Verilog module: grid_1__2__io_7__mode_io_phy__iopad_0_ -----
module grid_1__2__io_7__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [31:31] gfpga_pad_iopad
,
input [360:360] sram_blwl_bl ,
input [360:360] sram_blwl_wl ,
input [360:360] sram_blwl_blb );
wire [360:360] sram_blwl_out ;
wire [360:360] sram_blwl_outb ;
wire [360:360] sram_blwl_360_configbus0;
wire [360:360] sram_blwl_360_configbus1;
wire [360:360] sram_blwl_360_configbus0_b;
assign sram_blwl_360_configbus0[360:360] = sram_blwl_bl[360:360] ;
assign sram_blwl_360_configbus1[360:360] = sram_blwl_wl[360:360] ;
assign sram_blwl_360_configbus0_b[360:360] = sram_blwl_blb[360:360] ;
iopad iopad_31_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[31], sram_blwl_out[360:360] , sram_blwl_outb[360:360] );
sram6T_blwl sram_blwl_360_ (sram_blwl_out[360], sram_blwl_out[360], sram_blwl_outb[360], sram_blwl_360_configbus0[360:360], sram_blwl_360_configbus1[360:360] , sram_blwl_360_configbus0_b[360:360] );
endmodule
//----- END IO Verilog module: grid_1__2__io_7__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_1__2__io_7__mode_io_phy_ -----
module grid_1__2__io_7__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [31:31] gfpga_pad_iopad ,
input [360:360] sram_blwl_bl ,
input [360:360] sram_blwl_wl ,
input [360:360] sram_blwl_blb );
grid_1__2__io_7__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[31:31] ,
sram_blwl_bl[360:360] ,
sram_blwl_wl[360:360] ,
sram_blwl_blb[360:360] );
direct_interc direct_interc_76_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_77_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__2__io_7__mode_io_phy_ -----

//----- END -----

//----- Grid[1][2], Capactity: 8 -----
//----- Top Protocol -----
module grid_1__2_( 

//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input  bottom_height_0__pin_0_,
output  bottom_height_0__pin_1_,
input  bottom_height_0__pin_2_,
output  bottom_height_0__pin_3_,
input  bottom_height_0__pin_4_,
output  bottom_height_0__pin_5_,
input  bottom_height_0__pin_6_,
output  bottom_height_0__pin_7_,
input  bottom_height_0__pin_8_,
output  bottom_height_0__pin_9_,
input  bottom_height_0__pin_10_,
output  bottom_height_0__pin_11_,
input  bottom_height_0__pin_12_,
output  bottom_height_0__pin_13_,
input  bottom_height_0__pin_14_,
output  bottom_height_0__pin_15_,
input [31:24] gfpga_pad_iopad ,
input [353:360] sram_blwl_bl ,
input [353:360] sram_blwl_wl ,
input [353:360] sram_blwl_blb );
grid_1__2__io_0__mode_io_phy_  grid_1__2__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
bottom_height_0__pin_0_,
bottom_height_0__pin_1_
//---- IOPAD ----
,
gfpga_pad_iopad[24:24] ,
//---- SRAM ----
sram_blwl_bl[353:353] ,
sram_blwl_wl[353:353] ,
sram_blwl_blb[353:353] );
grid_1__2__io_1__mode_io_phy_  grid_1__2__1_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
bottom_height_0__pin_2_,
bottom_height_0__pin_3_
//---- IOPAD ----
,
gfpga_pad_iopad[25:25] ,
//---- SRAM ----
sram_blwl_bl[354:354] ,
sram_blwl_wl[354:354] ,
sram_blwl_blb[354:354] );
grid_1__2__io_2__mode_io_phy_  grid_1__2__2_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
bottom_height_0__pin_4_,
bottom_height_0__pin_5_
//---- IOPAD ----
,
gfpga_pad_iopad[26:26] ,
//---- SRAM ----
sram_blwl_bl[355:355] ,
sram_blwl_wl[355:355] ,
sram_blwl_blb[355:355] );
grid_1__2__io_3__mode_io_phy_  grid_1__2__3_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
bottom_height_0__pin_6_,
bottom_height_0__pin_7_
//---- IOPAD ----
,
gfpga_pad_iopad[27:27] ,
//---- SRAM ----
sram_blwl_bl[356:356] ,
sram_blwl_wl[356:356] ,
sram_blwl_blb[356:356] );
grid_1__2__io_4__mode_io_phy_  grid_1__2__4_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
bottom_height_0__pin_8_,
bottom_height_0__pin_9_
//---- IOPAD ----
,
gfpga_pad_iopad[28:28] ,
//---- SRAM ----
sram_blwl_bl[357:357] ,
sram_blwl_wl[357:357] ,
sram_blwl_blb[357:357] );
grid_1__2__io_5__mode_io_phy_  grid_1__2__5_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
bottom_height_0__pin_10_,
bottom_height_0__pin_11_
//---- IOPAD ----
,
gfpga_pad_iopad[29:29] ,
//---- SRAM ----
sram_blwl_bl[358:358] ,
sram_blwl_wl[358:358] ,
sram_blwl_blb[358:358] );
grid_1__2__io_6__mode_io_phy_  grid_1__2__6_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
bottom_height_0__pin_12_,
bottom_height_0__pin_13_
//---- IOPAD ----
,
gfpga_pad_iopad[30:30] ,
//---- SRAM ----
sram_blwl_bl[359:359] ,
sram_blwl_wl[359:359] ,
sram_blwl_blb[359:359] );
grid_1__2__io_7__mode_io_phy_  grid_1__2__7_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
bottom_height_0__pin_14_,
bottom_height_0__pin_15_
//---- IOPAD ----
,
gfpga_pad_iopad[31:31] ,
//---- SRAM ----
sram_blwl_bl[360:360] ,
sram_blwl_wl[360:360] ,
sram_blwl_blb[360:360] );
endmodule
//----- END Top Protocol -----
//----- END Grid[1][2], Capactity: 8 -----

