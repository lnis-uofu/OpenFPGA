//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Physical Logic Block  [2][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Grid[2][1] type_descriptor: io[0] -----
//----- IO Verilog module: grid_2__1__io_0__mode_io_phy__iopad_0_ -----
module grid_2__1__io_0__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [8:8] gfpga_pad_iopad
,
input [2634:2634] sram_blwl_bl ,
input [2634:2634] sram_blwl_wl ,
input [2634:2634] sram_blwl_blb );
wire [2634:2634] sram_blwl_out ;
wire [2634:2634] sram_blwl_outb ;
wire [2634:2634] sram_blwl_2634_configbus0;
wire [2634:2634] sram_blwl_2634_configbus1;
wire [2634:2634] sram_blwl_2634_configbus0_b;
assign sram_blwl_2634_configbus0[2634:2634] = sram_blwl_bl[2634:2634] ;
assign sram_blwl_2634_configbus1[2634:2634] = sram_blwl_wl[2634:2634] ;
assign sram_blwl_2634_configbus0_b[2634:2634] = sram_blwl_blb[2634:2634] ;
iopad iopad_8_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[8], sram_blwl_out[2634:2634] , sram_blwl_outb[2634:2634] );
sram6T_blwl sram_blwl_2634_ (sram_blwl_out[2634], sram_blwl_out[2634], sram_blwl_outb[2634], sram_blwl_2634_configbus0[2634:2634], sram_blwl_2634_configbus1[2634:2634] , sram_blwl_2634_configbus0_b[2634:2634] );
endmodule
//----- END IO Verilog module: grid_2__1__io_0__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_2__1__io_0__mode_io_phy_ -----
module grid_2__1__io_0__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [8:8] gfpga_pad_iopad ,
input [2634:2634] sram_blwl_bl ,
input [2634:2634] sram_blwl_wl ,
input [2634:2634] sram_blwl_blb );
grid_2__1__io_0__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[8:8] ,
sram_blwl_bl[2634:2634] ,
sram_blwl_wl[2634:2634] ,
sram_blwl_blb[2634:2634] );
direct_interc direct_interc_196_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_197_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_2__1__io_0__mode_io_phy_ -----

//----- END -----

//----- Grid[2][1] type_descriptor: io[1] -----
//----- IO Verilog module: grid_2__1__io_1__mode_io_phy__iopad_0_ -----
module grid_2__1__io_1__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [9:9] gfpga_pad_iopad
,
input [2635:2635] sram_blwl_bl ,
input [2635:2635] sram_blwl_wl ,
input [2635:2635] sram_blwl_blb );
wire [2635:2635] sram_blwl_out ;
wire [2635:2635] sram_blwl_outb ;
wire [2635:2635] sram_blwl_2635_configbus0;
wire [2635:2635] sram_blwl_2635_configbus1;
wire [2635:2635] sram_blwl_2635_configbus0_b;
assign sram_blwl_2635_configbus0[2635:2635] = sram_blwl_bl[2635:2635] ;
assign sram_blwl_2635_configbus1[2635:2635] = sram_blwl_wl[2635:2635] ;
assign sram_blwl_2635_configbus0_b[2635:2635] = sram_blwl_blb[2635:2635] ;
iopad iopad_9_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[9], sram_blwl_out[2635:2635] , sram_blwl_outb[2635:2635] );
sram6T_blwl sram_blwl_2635_ (sram_blwl_out[2635], sram_blwl_out[2635], sram_blwl_outb[2635], sram_blwl_2635_configbus0[2635:2635], sram_blwl_2635_configbus1[2635:2635] , sram_blwl_2635_configbus0_b[2635:2635] );
endmodule
//----- END IO Verilog module: grid_2__1__io_1__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_2__1__io_1__mode_io_phy_ -----
module grid_2__1__io_1__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [9:9] gfpga_pad_iopad ,
input [2635:2635] sram_blwl_bl ,
input [2635:2635] sram_blwl_wl ,
input [2635:2635] sram_blwl_blb );
grid_2__1__io_1__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[9:9] ,
sram_blwl_bl[2635:2635] ,
sram_blwl_wl[2635:2635] ,
sram_blwl_blb[2635:2635] );
direct_interc direct_interc_198_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_199_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_2__1__io_1__mode_io_phy_ -----

//----- END -----

//----- Grid[2][1] type_descriptor: io[2] -----
//----- IO Verilog module: grid_2__1__io_2__mode_io_phy__iopad_0_ -----
module grid_2__1__io_2__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [10:10] gfpga_pad_iopad
,
input [2636:2636] sram_blwl_bl ,
input [2636:2636] sram_blwl_wl ,
input [2636:2636] sram_blwl_blb );
wire [2636:2636] sram_blwl_out ;
wire [2636:2636] sram_blwl_outb ;
wire [2636:2636] sram_blwl_2636_configbus0;
wire [2636:2636] sram_blwl_2636_configbus1;
wire [2636:2636] sram_blwl_2636_configbus0_b;
assign sram_blwl_2636_configbus0[2636:2636] = sram_blwl_bl[2636:2636] ;
assign sram_blwl_2636_configbus1[2636:2636] = sram_blwl_wl[2636:2636] ;
assign sram_blwl_2636_configbus0_b[2636:2636] = sram_blwl_blb[2636:2636] ;
iopad iopad_10_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[10], sram_blwl_out[2636:2636] , sram_blwl_outb[2636:2636] );
sram6T_blwl sram_blwl_2636_ (sram_blwl_out[2636], sram_blwl_out[2636], sram_blwl_outb[2636], sram_blwl_2636_configbus0[2636:2636], sram_blwl_2636_configbus1[2636:2636] , sram_blwl_2636_configbus0_b[2636:2636] );
endmodule
//----- END IO Verilog module: grid_2__1__io_2__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_2__1__io_2__mode_io_phy_ -----
module grid_2__1__io_2__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [10:10] gfpga_pad_iopad ,
input [2636:2636] sram_blwl_bl ,
input [2636:2636] sram_blwl_wl ,
input [2636:2636] sram_blwl_blb );
grid_2__1__io_2__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[10:10] ,
sram_blwl_bl[2636:2636] ,
sram_blwl_wl[2636:2636] ,
sram_blwl_blb[2636:2636] );
direct_interc direct_interc_200_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_201_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_2__1__io_2__mode_io_phy_ -----

//----- END -----

//----- Grid[2][1] type_descriptor: io[3] -----
//----- IO Verilog module: grid_2__1__io_3__mode_io_phy__iopad_0_ -----
module grid_2__1__io_3__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [11:11] gfpga_pad_iopad
,
input [2637:2637] sram_blwl_bl ,
input [2637:2637] sram_blwl_wl ,
input [2637:2637] sram_blwl_blb );
wire [2637:2637] sram_blwl_out ;
wire [2637:2637] sram_blwl_outb ;
wire [2637:2637] sram_blwl_2637_configbus0;
wire [2637:2637] sram_blwl_2637_configbus1;
wire [2637:2637] sram_blwl_2637_configbus0_b;
assign sram_blwl_2637_configbus0[2637:2637] = sram_blwl_bl[2637:2637] ;
assign sram_blwl_2637_configbus1[2637:2637] = sram_blwl_wl[2637:2637] ;
assign sram_blwl_2637_configbus0_b[2637:2637] = sram_blwl_blb[2637:2637] ;
iopad iopad_11_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[11], sram_blwl_out[2637:2637] , sram_blwl_outb[2637:2637] );
sram6T_blwl sram_blwl_2637_ (sram_blwl_out[2637], sram_blwl_out[2637], sram_blwl_outb[2637], sram_blwl_2637_configbus0[2637:2637], sram_blwl_2637_configbus1[2637:2637] , sram_blwl_2637_configbus0_b[2637:2637] );
endmodule
//----- END IO Verilog module: grid_2__1__io_3__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_2__1__io_3__mode_io_phy_ -----
module grid_2__1__io_3__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [11:11] gfpga_pad_iopad ,
input [2637:2637] sram_blwl_bl ,
input [2637:2637] sram_blwl_wl ,
input [2637:2637] sram_blwl_blb );
grid_2__1__io_3__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[11:11] ,
sram_blwl_bl[2637:2637] ,
sram_blwl_wl[2637:2637] ,
sram_blwl_blb[2637:2637] );
direct_interc direct_interc_202_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_203_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_2__1__io_3__mode_io_phy_ -----

//----- END -----

//----- Grid[2][1] type_descriptor: io[4] -----
//----- IO Verilog module: grid_2__1__io_4__mode_io_phy__iopad_0_ -----
module grid_2__1__io_4__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [12:12] gfpga_pad_iopad
,
input [2638:2638] sram_blwl_bl ,
input [2638:2638] sram_blwl_wl ,
input [2638:2638] sram_blwl_blb );
wire [2638:2638] sram_blwl_out ;
wire [2638:2638] sram_blwl_outb ;
wire [2638:2638] sram_blwl_2638_configbus0;
wire [2638:2638] sram_blwl_2638_configbus1;
wire [2638:2638] sram_blwl_2638_configbus0_b;
assign sram_blwl_2638_configbus0[2638:2638] = sram_blwl_bl[2638:2638] ;
assign sram_blwl_2638_configbus1[2638:2638] = sram_blwl_wl[2638:2638] ;
assign sram_blwl_2638_configbus0_b[2638:2638] = sram_blwl_blb[2638:2638] ;
iopad iopad_12_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[12], sram_blwl_out[2638:2638] , sram_blwl_outb[2638:2638] );
sram6T_blwl sram_blwl_2638_ (sram_blwl_out[2638], sram_blwl_out[2638], sram_blwl_outb[2638], sram_blwl_2638_configbus0[2638:2638], sram_blwl_2638_configbus1[2638:2638] , sram_blwl_2638_configbus0_b[2638:2638] );
endmodule
//----- END IO Verilog module: grid_2__1__io_4__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_2__1__io_4__mode_io_phy_ -----
module grid_2__1__io_4__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [12:12] gfpga_pad_iopad ,
input [2638:2638] sram_blwl_bl ,
input [2638:2638] sram_blwl_wl ,
input [2638:2638] sram_blwl_blb );
grid_2__1__io_4__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[12:12] ,
sram_blwl_bl[2638:2638] ,
sram_blwl_wl[2638:2638] ,
sram_blwl_blb[2638:2638] );
direct_interc direct_interc_204_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_205_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_2__1__io_4__mode_io_phy_ -----

//----- END -----

//----- Grid[2][1] type_descriptor: io[5] -----
//----- IO Verilog module: grid_2__1__io_5__mode_io_phy__iopad_0_ -----
module grid_2__1__io_5__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [13:13] gfpga_pad_iopad
,
input [2639:2639] sram_blwl_bl ,
input [2639:2639] sram_blwl_wl ,
input [2639:2639] sram_blwl_blb );
wire [2639:2639] sram_blwl_out ;
wire [2639:2639] sram_blwl_outb ;
wire [2639:2639] sram_blwl_2639_configbus0;
wire [2639:2639] sram_blwl_2639_configbus1;
wire [2639:2639] sram_blwl_2639_configbus0_b;
assign sram_blwl_2639_configbus0[2639:2639] = sram_blwl_bl[2639:2639] ;
assign sram_blwl_2639_configbus1[2639:2639] = sram_blwl_wl[2639:2639] ;
assign sram_blwl_2639_configbus0_b[2639:2639] = sram_blwl_blb[2639:2639] ;
iopad iopad_13_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[13], sram_blwl_out[2639:2639] , sram_blwl_outb[2639:2639] );
sram6T_blwl sram_blwl_2639_ (sram_blwl_out[2639], sram_blwl_out[2639], sram_blwl_outb[2639], sram_blwl_2639_configbus0[2639:2639], sram_blwl_2639_configbus1[2639:2639] , sram_blwl_2639_configbus0_b[2639:2639] );
endmodule
//----- END IO Verilog module: grid_2__1__io_5__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_2__1__io_5__mode_io_phy_ -----
module grid_2__1__io_5__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [13:13] gfpga_pad_iopad ,
input [2639:2639] sram_blwl_bl ,
input [2639:2639] sram_blwl_wl ,
input [2639:2639] sram_blwl_blb );
grid_2__1__io_5__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[13:13] ,
sram_blwl_bl[2639:2639] ,
sram_blwl_wl[2639:2639] ,
sram_blwl_blb[2639:2639] );
direct_interc direct_interc_206_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_207_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_2__1__io_5__mode_io_phy_ -----

//----- END -----

//----- Grid[2][1] type_descriptor: io[6] -----
//----- IO Verilog module: grid_2__1__io_6__mode_io_phy__iopad_0_ -----
module grid_2__1__io_6__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [14:14] gfpga_pad_iopad
,
input [2640:2640] sram_blwl_bl ,
input [2640:2640] sram_blwl_wl ,
input [2640:2640] sram_blwl_blb );
wire [2640:2640] sram_blwl_out ;
wire [2640:2640] sram_blwl_outb ;
wire [2640:2640] sram_blwl_2640_configbus0;
wire [2640:2640] sram_blwl_2640_configbus1;
wire [2640:2640] sram_blwl_2640_configbus0_b;
assign sram_blwl_2640_configbus0[2640:2640] = sram_blwl_bl[2640:2640] ;
assign sram_blwl_2640_configbus1[2640:2640] = sram_blwl_wl[2640:2640] ;
assign sram_blwl_2640_configbus0_b[2640:2640] = sram_blwl_blb[2640:2640] ;
iopad iopad_14_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[14], sram_blwl_out[2640:2640] , sram_blwl_outb[2640:2640] );
sram6T_blwl sram_blwl_2640_ (sram_blwl_out[2640], sram_blwl_out[2640], sram_blwl_outb[2640], sram_blwl_2640_configbus0[2640:2640], sram_blwl_2640_configbus1[2640:2640] , sram_blwl_2640_configbus0_b[2640:2640] );
endmodule
//----- END IO Verilog module: grid_2__1__io_6__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_2__1__io_6__mode_io_phy_ -----
module grid_2__1__io_6__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [14:14] gfpga_pad_iopad ,
input [2640:2640] sram_blwl_bl ,
input [2640:2640] sram_blwl_wl ,
input [2640:2640] sram_blwl_blb );
grid_2__1__io_6__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[14:14] ,
sram_blwl_bl[2640:2640] ,
sram_blwl_wl[2640:2640] ,
sram_blwl_blb[2640:2640] );
direct_interc direct_interc_208_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_209_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_2__1__io_6__mode_io_phy_ -----

//----- END -----

//----- Grid[2][1] type_descriptor: io[7] -----
//----- IO Verilog module: grid_2__1__io_7__mode_io_phy__iopad_0_ -----
module grid_2__1__io_7__mode_io_phy__iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
input [0:0] zin
//----- END Global ports of SPICE_MODEL(iopad)-----
,
input wire iopad_0___outpad_0_,
output wire iopad_0___inpad_0_,
inout [15:15] gfpga_pad_iopad
,
input [2641:2641] sram_blwl_bl ,
input [2641:2641] sram_blwl_wl ,
input [2641:2641] sram_blwl_blb );
wire [2641:2641] sram_blwl_out ;
wire [2641:2641] sram_blwl_outb ;
wire [2641:2641] sram_blwl_2641_configbus0;
wire [2641:2641] sram_blwl_2641_configbus1;
wire [2641:2641] sram_blwl_2641_configbus0_b;
assign sram_blwl_2641_configbus0[2641:2641] = sram_blwl_bl[2641:2641] ;
assign sram_blwl_2641_configbus1[2641:2641] = sram_blwl_wl[2641:2641] ;
assign sram_blwl_2641_configbus0_b[2641:2641] = sram_blwl_blb[2641:2641] ;
iopad iopad_15_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[15], sram_blwl_out[2641:2641] , sram_blwl_outb[2641:2641] );
sram6T_blwl sram_blwl_2641_ (sram_blwl_out[2641], sram_blwl_out[2641], sram_blwl_outb[2641], sram_blwl_2641_configbus0[2641:2641], sram_blwl_2641_configbus1[2641:2641] , sram_blwl_2641_configbus0_b[2641:2641] );
endmodule
//----- END IO Verilog module: grid_2__1__io_7__mode_io_phy__iopad_0_ -----

//----- Physical programmable logic block Verilog module grid_2__1__io_7__mode_io_phy_ -----
module grid_2__1__io_7__mode_io_phy_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_io_phy___outpad_0_,
output wire mode_io_phy___inpad_0_,
input [15:15] gfpga_pad_iopad ,
input [2641:2641] sram_blwl_bl ,
input [2641:2641] sram_blwl_wl ,
input [2641:2641] sram_blwl_blb );
grid_2__1__io_7__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[15:15] ,
sram_blwl_bl[2641:2641] ,
sram_blwl_wl[2641:2641] ,
sram_blwl_blb[2641:2641] );
direct_interc direct_interc_210_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_211_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_2__1__io_7__mode_io_phy_ -----

//----- END -----

//----- Grid[2][1], Capactity: 8 -----
//----- Top Protocol -----
module grid_2__1_( 

//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input  left_height_0__pin_0_,
output  left_height_0__pin_1_,
input  left_height_0__pin_2_,
output  left_height_0__pin_3_,
input  left_height_0__pin_4_,
output  left_height_0__pin_5_,
input  left_height_0__pin_6_,
output  left_height_0__pin_7_,
input  left_height_0__pin_8_,
output  left_height_0__pin_9_,
input  left_height_0__pin_10_,
output  left_height_0__pin_11_,
input  left_height_0__pin_12_,
output  left_height_0__pin_13_,
input  left_height_0__pin_14_,
output  left_height_0__pin_15_,
input [15:8] gfpga_pad_iopad ,
input [2634:2641] sram_blwl_bl ,
input [2634:2641] sram_blwl_wl ,
input [2634:2641] sram_blwl_blb );
grid_2__1__io_0__mode_io_phy_  grid_2__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
left_height_0__pin_0_,
left_height_0__pin_1_
//---- IOPAD ----
,
gfpga_pad_iopad[8:8] ,
//---- SRAM ----
sram_blwl_bl[2634:2634] ,
sram_blwl_wl[2634:2634] ,
sram_blwl_blb[2634:2634] );
grid_2__1__io_1__mode_io_phy_  grid_2__1__1_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
left_height_0__pin_2_,
left_height_0__pin_3_
//---- IOPAD ----
,
gfpga_pad_iopad[9:9] ,
//---- SRAM ----
sram_blwl_bl[2635:2635] ,
sram_blwl_wl[2635:2635] ,
sram_blwl_blb[2635:2635] );
grid_2__1__io_2__mode_io_phy_  grid_2__1__2_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
left_height_0__pin_4_,
left_height_0__pin_5_
//---- IOPAD ----
,
gfpga_pad_iopad[10:10] ,
//---- SRAM ----
sram_blwl_bl[2636:2636] ,
sram_blwl_wl[2636:2636] ,
sram_blwl_blb[2636:2636] );
grid_2__1__io_3__mode_io_phy_  grid_2__1__3_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
left_height_0__pin_6_,
left_height_0__pin_7_
//---- IOPAD ----
,
gfpga_pad_iopad[11:11] ,
//---- SRAM ----
sram_blwl_bl[2637:2637] ,
sram_blwl_wl[2637:2637] ,
sram_blwl_blb[2637:2637] );
grid_2__1__io_4__mode_io_phy_  grid_2__1__4_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
left_height_0__pin_8_,
left_height_0__pin_9_
//---- IOPAD ----
,
gfpga_pad_iopad[12:12] ,
//---- SRAM ----
sram_blwl_bl[2638:2638] ,
sram_blwl_wl[2638:2638] ,
sram_blwl_blb[2638:2638] );
grid_2__1__io_5__mode_io_phy_  grid_2__1__5_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
left_height_0__pin_10_,
left_height_0__pin_11_
//---- IOPAD ----
,
gfpga_pad_iopad[13:13] ,
//---- SRAM ----
sram_blwl_bl[2639:2639] ,
sram_blwl_wl[2639:2639] ,
sram_blwl_blb[2639:2639] );
grid_2__1__io_6__mode_io_phy_  grid_2__1__6_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
left_height_0__pin_12_,
left_height_0__pin_13_
//---- IOPAD ----
,
gfpga_pad_iopad[14:14] ,
//---- SRAM ----
sram_blwl_bl[2640:2640] ,
sram_blwl_wl[2640:2640] ,
sram_blwl_blb[2640:2640] );
grid_2__1__io_7__mode_io_phy_  grid_2__1__7_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
left_height_0__pin_14_,
left_height_0__pin_15_
//---- IOPAD ----
,
gfpga_pad_iopad[15:15] ,
//---- SRAM ----
sram_blwl_bl[2641:2641] ,
sram_blwl_wl[2641:2641] ,
sram_blwl_blb[2641:2641] );
endmodule
//----- END Top Protocol -----
//----- END Grid[2][1], Capactity: 8 -----

