//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Physical Logic Block  [0][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
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
input [2626:2626] sram_blwl_bl ,
input [2626:2626] sram_blwl_wl ,
input [2626:2626] sram_blwl_blb );
wire [2626:2626] sram_blwl_out ;
wire [2626:2626] sram_blwl_outb ;
wire [2626:2626] sram_blwl_2626_configbus0;
wire [2626:2626] sram_blwl_2626_configbus1;
wire [2626:2626] sram_blwl_2626_configbus0_b;
assign sram_blwl_2626_configbus0[2626:2626] = sram_blwl_bl[2626:2626] ;
assign sram_blwl_2626_configbus1[2626:2626] = sram_blwl_wl[2626:2626] ;
assign sram_blwl_2626_configbus0_b[2626:2626] = sram_blwl_blb[2626:2626] ;
iopad iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[0], sram_blwl_out[2626:2626] , sram_blwl_outb[2626:2626] );
sram6T_blwl sram_blwl_2626_ (sram_blwl_out[2626], sram_blwl_out[2626], sram_blwl_outb[2626], sram_blwl_2626_configbus0[2626:2626], sram_blwl_2626_configbus1[2626:2626] , sram_blwl_2626_configbus0_b[2626:2626] );
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
input [2626:2626] sram_blwl_bl ,
input [2626:2626] sram_blwl_wl ,
input [2626:2626] sram_blwl_blb );
grid_0__1__io_0__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[0:0] ,
sram_blwl_bl[2626:2626] ,
sram_blwl_wl[2626:2626] ,
sram_blwl_blb[2626:2626] );
direct_interc direct_interc_180_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_181_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2627:2627] sram_blwl_bl ,
input [2627:2627] sram_blwl_wl ,
input [2627:2627] sram_blwl_blb );
wire [2627:2627] sram_blwl_out ;
wire [2627:2627] sram_blwl_outb ;
wire [2627:2627] sram_blwl_2627_configbus0;
wire [2627:2627] sram_blwl_2627_configbus1;
wire [2627:2627] sram_blwl_2627_configbus0_b;
assign sram_blwl_2627_configbus0[2627:2627] = sram_blwl_bl[2627:2627] ;
assign sram_blwl_2627_configbus1[2627:2627] = sram_blwl_wl[2627:2627] ;
assign sram_blwl_2627_configbus0_b[2627:2627] = sram_blwl_blb[2627:2627] ;
iopad iopad_1_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[1], sram_blwl_out[2627:2627] , sram_blwl_outb[2627:2627] );
sram6T_blwl sram_blwl_2627_ (sram_blwl_out[2627], sram_blwl_out[2627], sram_blwl_outb[2627], sram_blwl_2627_configbus0[2627:2627], sram_blwl_2627_configbus1[2627:2627] , sram_blwl_2627_configbus0_b[2627:2627] );
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
input [2627:2627] sram_blwl_bl ,
input [2627:2627] sram_blwl_wl ,
input [2627:2627] sram_blwl_blb );
grid_0__1__io_1__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[1:1] ,
sram_blwl_bl[2627:2627] ,
sram_blwl_wl[2627:2627] ,
sram_blwl_blb[2627:2627] );
direct_interc direct_interc_182_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_183_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2628:2628] sram_blwl_bl ,
input [2628:2628] sram_blwl_wl ,
input [2628:2628] sram_blwl_blb );
wire [2628:2628] sram_blwl_out ;
wire [2628:2628] sram_blwl_outb ;
wire [2628:2628] sram_blwl_2628_configbus0;
wire [2628:2628] sram_blwl_2628_configbus1;
wire [2628:2628] sram_blwl_2628_configbus0_b;
assign sram_blwl_2628_configbus0[2628:2628] = sram_blwl_bl[2628:2628] ;
assign sram_blwl_2628_configbus1[2628:2628] = sram_blwl_wl[2628:2628] ;
assign sram_blwl_2628_configbus0_b[2628:2628] = sram_blwl_blb[2628:2628] ;
iopad iopad_2_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[2], sram_blwl_out[2628:2628] , sram_blwl_outb[2628:2628] );
sram6T_blwl sram_blwl_2628_ (sram_blwl_out[2628], sram_blwl_out[2628], sram_blwl_outb[2628], sram_blwl_2628_configbus0[2628:2628], sram_blwl_2628_configbus1[2628:2628] , sram_blwl_2628_configbus0_b[2628:2628] );
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
input [2628:2628] sram_blwl_bl ,
input [2628:2628] sram_blwl_wl ,
input [2628:2628] sram_blwl_blb );
grid_0__1__io_2__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[2:2] ,
sram_blwl_bl[2628:2628] ,
sram_blwl_wl[2628:2628] ,
sram_blwl_blb[2628:2628] );
direct_interc direct_interc_184_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_185_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2629:2629] sram_blwl_bl ,
input [2629:2629] sram_blwl_wl ,
input [2629:2629] sram_blwl_blb );
wire [2629:2629] sram_blwl_out ;
wire [2629:2629] sram_blwl_outb ;
wire [2629:2629] sram_blwl_2629_configbus0;
wire [2629:2629] sram_blwl_2629_configbus1;
wire [2629:2629] sram_blwl_2629_configbus0_b;
assign sram_blwl_2629_configbus0[2629:2629] = sram_blwl_bl[2629:2629] ;
assign sram_blwl_2629_configbus1[2629:2629] = sram_blwl_wl[2629:2629] ;
assign sram_blwl_2629_configbus0_b[2629:2629] = sram_blwl_blb[2629:2629] ;
iopad iopad_3_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[3], sram_blwl_out[2629:2629] , sram_blwl_outb[2629:2629] );
sram6T_blwl sram_blwl_2629_ (sram_blwl_out[2629], sram_blwl_out[2629], sram_blwl_outb[2629], sram_blwl_2629_configbus0[2629:2629], sram_blwl_2629_configbus1[2629:2629] , sram_blwl_2629_configbus0_b[2629:2629] );
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
input [2629:2629] sram_blwl_bl ,
input [2629:2629] sram_blwl_wl ,
input [2629:2629] sram_blwl_blb );
grid_0__1__io_3__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[3:3] ,
sram_blwl_bl[2629:2629] ,
sram_blwl_wl[2629:2629] ,
sram_blwl_blb[2629:2629] );
direct_interc direct_interc_186_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_187_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2630:2630] sram_blwl_bl ,
input [2630:2630] sram_blwl_wl ,
input [2630:2630] sram_blwl_blb );
wire [2630:2630] sram_blwl_out ;
wire [2630:2630] sram_blwl_outb ;
wire [2630:2630] sram_blwl_2630_configbus0;
wire [2630:2630] sram_blwl_2630_configbus1;
wire [2630:2630] sram_blwl_2630_configbus0_b;
assign sram_blwl_2630_configbus0[2630:2630] = sram_blwl_bl[2630:2630] ;
assign sram_blwl_2630_configbus1[2630:2630] = sram_blwl_wl[2630:2630] ;
assign sram_blwl_2630_configbus0_b[2630:2630] = sram_blwl_blb[2630:2630] ;
iopad iopad_4_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[4], sram_blwl_out[2630:2630] , sram_blwl_outb[2630:2630] );
sram6T_blwl sram_blwl_2630_ (sram_blwl_out[2630], sram_blwl_out[2630], sram_blwl_outb[2630], sram_blwl_2630_configbus0[2630:2630], sram_blwl_2630_configbus1[2630:2630] , sram_blwl_2630_configbus0_b[2630:2630] );
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
input [2630:2630] sram_blwl_bl ,
input [2630:2630] sram_blwl_wl ,
input [2630:2630] sram_blwl_blb );
grid_0__1__io_4__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[4:4] ,
sram_blwl_bl[2630:2630] ,
sram_blwl_wl[2630:2630] ,
sram_blwl_blb[2630:2630] );
direct_interc direct_interc_188_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_189_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2631:2631] sram_blwl_bl ,
input [2631:2631] sram_blwl_wl ,
input [2631:2631] sram_blwl_blb );
wire [2631:2631] sram_blwl_out ;
wire [2631:2631] sram_blwl_outb ;
wire [2631:2631] sram_blwl_2631_configbus0;
wire [2631:2631] sram_blwl_2631_configbus1;
wire [2631:2631] sram_blwl_2631_configbus0_b;
assign sram_blwl_2631_configbus0[2631:2631] = sram_blwl_bl[2631:2631] ;
assign sram_blwl_2631_configbus1[2631:2631] = sram_blwl_wl[2631:2631] ;
assign sram_blwl_2631_configbus0_b[2631:2631] = sram_blwl_blb[2631:2631] ;
iopad iopad_5_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[5], sram_blwl_out[2631:2631] , sram_blwl_outb[2631:2631] );
sram6T_blwl sram_blwl_2631_ (sram_blwl_out[2631], sram_blwl_out[2631], sram_blwl_outb[2631], sram_blwl_2631_configbus0[2631:2631], sram_blwl_2631_configbus1[2631:2631] , sram_blwl_2631_configbus0_b[2631:2631] );
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
input [2631:2631] sram_blwl_bl ,
input [2631:2631] sram_blwl_wl ,
input [2631:2631] sram_blwl_blb );
grid_0__1__io_5__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[5:5] ,
sram_blwl_bl[2631:2631] ,
sram_blwl_wl[2631:2631] ,
sram_blwl_blb[2631:2631] );
direct_interc direct_interc_190_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_191_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2632:2632] sram_blwl_bl ,
input [2632:2632] sram_blwl_wl ,
input [2632:2632] sram_blwl_blb );
wire [2632:2632] sram_blwl_out ;
wire [2632:2632] sram_blwl_outb ;
wire [2632:2632] sram_blwl_2632_configbus0;
wire [2632:2632] sram_blwl_2632_configbus1;
wire [2632:2632] sram_blwl_2632_configbus0_b;
assign sram_blwl_2632_configbus0[2632:2632] = sram_blwl_bl[2632:2632] ;
assign sram_blwl_2632_configbus1[2632:2632] = sram_blwl_wl[2632:2632] ;
assign sram_blwl_2632_configbus0_b[2632:2632] = sram_blwl_blb[2632:2632] ;
iopad iopad_6_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[6], sram_blwl_out[2632:2632] , sram_blwl_outb[2632:2632] );
sram6T_blwl sram_blwl_2632_ (sram_blwl_out[2632], sram_blwl_out[2632], sram_blwl_outb[2632], sram_blwl_2632_configbus0[2632:2632], sram_blwl_2632_configbus1[2632:2632] , sram_blwl_2632_configbus0_b[2632:2632] );
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
input [2632:2632] sram_blwl_bl ,
input [2632:2632] sram_blwl_wl ,
input [2632:2632] sram_blwl_blb );
grid_0__1__io_6__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[6:6] ,
sram_blwl_bl[2632:2632] ,
sram_blwl_wl[2632:2632] ,
sram_blwl_blb[2632:2632] );
direct_interc direct_interc_192_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_193_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2633:2633] sram_blwl_bl ,
input [2633:2633] sram_blwl_wl ,
input [2633:2633] sram_blwl_blb );
wire [2633:2633] sram_blwl_out ;
wire [2633:2633] sram_blwl_outb ;
wire [2633:2633] sram_blwl_2633_configbus0;
wire [2633:2633] sram_blwl_2633_configbus1;
wire [2633:2633] sram_blwl_2633_configbus0_b;
assign sram_blwl_2633_configbus0[2633:2633] = sram_blwl_bl[2633:2633] ;
assign sram_blwl_2633_configbus1[2633:2633] = sram_blwl_wl[2633:2633] ;
assign sram_blwl_2633_configbus0_b[2633:2633] = sram_blwl_blb[2633:2633] ;
iopad iopad_7_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[7], sram_blwl_out[2633:2633] , sram_blwl_outb[2633:2633] );
sram6T_blwl sram_blwl_2633_ (sram_blwl_out[2633], sram_blwl_out[2633], sram_blwl_outb[2633], sram_blwl_2633_configbus0[2633:2633], sram_blwl_2633_configbus1[2633:2633] , sram_blwl_2633_configbus0_b[2633:2633] );
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
input [2633:2633] sram_blwl_bl ,
input [2633:2633] sram_blwl_wl ,
input [2633:2633] sram_blwl_blb );
grid_0__1__io_7__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[7:7] ,
sram_blwl_bl[2633:2633] ,
sram_blwl_wl[2633:2633] ,
sram_blwl_blb[2633:2633] );
direct_interc direct_interc_194_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_195_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2626:2633] sram_blwl_bl ,
input [2626:2633] sram_blwl_wl ,
input [2626:2633] sram_blwl_blb );
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
sram_blwl_bl[2626:2626] ,
sram_blwl_wl[2626:2626] ,
sram_blwl_blb[2626:2626] );
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
sram_blwl_bl[2627:2627] ,
sram_blwl_wl[2627:2627] ,
sram_blwl_blb[2627:2627] );
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
sram_blwl_bl[2628:2628] ,
sram_blwl_wl[2628:2628] ,
sram_blwl_blb[2628:2628] );
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
sram_blwl_bl[2629:2629] ,
sram_blwl_wl[2629:2629] ,
sram_blwl_blb[2629:2629] );
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
sram_blwl_bl[2630:2630] ,
sram_blwl_wl[2630:2630] ,
sram_blwl_blb[2630:2630] );
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
sram_blwl_bl[2631:2631] ,
sram_blwl_wl[2631:2631] ,
sram_blwl_blb[2631:2631] );
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
sram_blwl_bl[2632:2632] ,
sram_blwl_wl[2632:2632] ,
sram_blwl_blb[2632:2632] );
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
sram_blwl_bl[2633:2633] ,
sram_blwl_wl[2633:2633] ,
sram_blwl_blb[2633:2633] );
endmodule
//----- END Top Protocol -----
//----- END Grid[0][1], Capactity: 8 -----

