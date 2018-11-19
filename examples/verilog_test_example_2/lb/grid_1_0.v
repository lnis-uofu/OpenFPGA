//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Physical Logic Block  [1][0] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
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
input [2642:2642] sram_blwl_bl ,
input [2642:2642] sram_blwl_wl ,
input [2642:2642] sram_blwl_blb );
wire [2642:2642] sram_blwl_out ;
wire [2642:2642] sram_blwl_outb ;
wire [2642:2642] sram_blwl_2642_configbus0;
wire [2642:2642] sram_blwl_2642_configbus1;
wire [2642:2642] sram_blwl_2642_configbus0_b;
assign sram_blwl_2642_configbus0[2642:2642] = sram_blwl_bl[2642:2642] ;
assign sram_blwl_2642_configbus1[2642:2642] = sram_blwl_wl[2642:2642] ;
assign sram_blwl_2642_configbus0_b[2642:2642] = sram_blwl_blb[2642:2642] ;
iopad iopad_16_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[16], sram_blwl_out[2642:2642] , sram_blwl_outb[2642:2642] );
sram6T_blwl sram_blwl_2642_ (sram_blwl_out[2642], sram_blwl_out[2642], sram_blwl_outb[2642], sram_blwl_2642_configbus0[2642:2642], sram_blwl_2642_configbus1[2642:2642] , sram_blwl_2642_configbus0_b[2642:2642] );
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
input [2642:2642] sram_blwl_bl ,
input [2642:2642] sram_blwl_wl ,
input [2642:2642] sram_blwl_blb );
grid_1__0__io_0__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[16:16] ,
sram_blwl_bl[2642:2642] ,
sram_blwl_wl[2642:2642] ,
sram_blwl_blb[2642:2642] );
direct_interc direct_interc_212_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_213_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2643:2643] sram_blwl_bl ,
input [2643:2643] sram_blwl_wl ,
input [2643:2643] sram_blwl_blb );
wire [2643:2643] sram_blwl_out ;
wire [2643:2643] sram_blwl_outb ;
wire [2643:2643] sram_blwl_2643_configbus0;
wire [2643:2643] sram_blwl_2643_configbus1;
wire [2643:2643] sram_blwl_2643_configbus0_b;
assign sram_blwl_2643_configbus0[2643:2643] = sram_blwl_bl[2643:2643] ;
assign sram_blwl_2643_configbus1[2643:2643] = sram_blwl_wl[2643:2643] ;
assign sram_blwl_2643_configbus0_b[2643:2643] = sram_blwl_blb[2643:2643] ;
iopad iopad_17_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[17], sram_blwl_out[2643:2643] , sram_blwl_outb[2643:2643] );
sram6T_blwl sram_blwl_2643_ (sram_blwl_out[2643], sram_blwl_out[2643], sram_blwl_outb[2643], sram_blwl_2643_configbus0[2643:2643], sram_blwl_2643_configbus1[2643:2643] , sram_blwl_2643_configbus0_b[2643:2643] );
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
input [2643:2643] sram_blwl_bl ,
input [2643:2643] sram_blwl_wl ,
input [2643:2643] sram_blwl_blb );
grid_1__0__io_1__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[17:17] ,
sram_blwl_bl[2643:2643] ,
sram_blwl_wl[2643:2643] ,
sram_blwl_blb[2643:2643] );
direct_interc direct_interc_214_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_215_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2644:2644] sram_blwl_bl ,
input [2644:2644] sram_blwl_wl ,
input [2644:2644] sram_blwl_blb );
wire [2644:2644] sram_blwl_out ;
wire [2644:2644] sram_blwl_outb ;
wire [2644:2644] sram_blwl_2644_configbus0;
wire [2644:2644] sram_blwl_2644_configbus1;
wire [2644:2644] sram_blwl_2644_configbus0_b;
assign sram_blwl_2644_configbus0[2644:2644] = sram_blwl_bl[2644:2644] ;
assign sram_blwl_2644_configbus1[2644:2644] = sram_blwl_wl[2644:2644] ;
assign sram_blwl_2644_configbus0_b[2644:2644] = sram_blwl_blb[2644:2644] ;
iopad iopad_18_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[18], sram_blwl_out[2644:2644] , sram_blwl_outb[2644:2644] );
sram6T_blwl sram_blwl_2644_ (sram_blwl_out[2644], sram_blwl_out[2644], sram_blwl_outb[2644], sram_blwl_2644_configbus0[2644:2644], sram_blwl_2644_configbus1[2644:2644] , sram_blwl_2644_configbus0_b[2644:2644] );
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
input [2644:2644] sram_blwl_bl ,
input [2644:2644] sram_blwl_wl ,
input [2644:2644] sram_blwl_blb );
grid_1__0__io_2__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[18:18] ,
sram_blwl_bl[2644:2644] ,
sram_blwl_wl[2644:2644] ,
sram_blwl_blb[2644:2644] );
direct_interc direct_interc_216_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_217_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2645:2645] sram_blwl_bl ,
input [2645:2645] sram_blwl_wl ,
input [2645:2645] sram_blwl_blb );
wire [2645:2645] sram_blwl_out ;
wire [2645:2645] sram_blwl_outb ;
wire [2645:2645] sram_blwl_2645_configbus0;
wire [2645:2645] sram_blwl_2645_configbus1;
wire [2645:2645] sram_blwl_2645_configbus0_b;
assign sram_blwl_2645_configbus0[2645:2645] = sram_blwl_bl[2645:2645] ;
assign sram_blwl_2645_configbus1[2645:2645] = sram_blwl_wl[2645:2645] ;
assign sram_blwl_2645_configbus0_b[2645:2645] = sram_blwl_blb[2645:2645] ;
iopad iopad_19_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[19], sram_blwl_out[2645:2645] , sram_blwl_outb[2645:2645] );
sram6T_blwl sram_blwl_2645_ (sram_blwl_out[2645], sram_blwl_out[2645], sram_blwl_outb[2645], sram_blwl_2645_configbus0[2645:2645], sram_blwl_2645_configbus1[2645:2645] , sram_blwl_2645_configbus0_b[2645:2645] );
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
input [2645:2645] sram_blwl_bl ,
input [2645:2645] sram_blwl_wl ,
input [2645:2645] sram_blwl_blb );
grid_1__0__io_3__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[19:19] ,
sram_blwl_bl[2645:2645] ,
sram_blwl_wl[2645:2645] ,
sram_blwl_blb[2645:2645] );
direct_interc direct_interc_218_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_219_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2646:2646] sram_blwl_bl ,
input [2646:2646] sram_blwl_wl ,
input [2646:2646] sram_blwl_blb );
wire [2646:2646] sram_blwl_out ;
wire [2646:2646] sram_blwl_outb ;
wire [2646:2646] sram_blwl_2646_configbus0;
wire [2646:2646] sram_blwl_2646_configbus1;
wire [2646:2646] sram_blwl_2646_configbus0_b;
assign sram_blwl_2646_configbus0[2646:2646] = sram_blwl_bl[2646:2646] ;
assign sram_blwl_2646_configbus1[2646:2646] = sram_blwl_wl[2646:2646] ;
assign sram_blwl_2646_configbus0_b[2646:2646] = sram_blwl_blb[2646:2646] ;
iopad iopad_20_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[20], sram_blwl_out[2646:2646] , sram_blwl_outb[2646:2646] );
sram6T_blwl sram_blwl_2646_ (sram_blwl_out[2646], sram_blwl_out[2646], sram_blwl_outb[2646], sram_blwl_2646_configbus0[2646:2646], sram_blwl_2646_configbus1[2646:2646] , sram_blwl_2646_configbus0_b[2646:2646] );
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
input [2646:2646] sram_blwl_bl ,
input [2646:2646] sram_blwl_wl ,
input [2646:2646] sram_blwl_blb );
grid_1__0__io_4__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[20:20] ,
sram_blwl_bl[2646:2646] ,
sram_blwl_wl[2646:2646] ,
sram_blwl_blb[2646:2646] );
direct_interc direct_interc_220_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_221_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2647:2647] sram_blwl_bl ,
input [2647:2647] sram_blwl_wl ,
input [2647:2647] sram_blwl_blb );
wire [2647:2647] sram_blwl_out ;
wire [2647:2647] sram_blwl_outb ;
wire [2647:2647] sram_blwl_2647_configbus0;
wire [2647:2647] sram_blwl_2647_configbus1;
wire [2647:2647] sram_blwl_2647_configbus0_b;
assign sram_blwl_2647_configbus0[2647:2647] = sram_blwl_bl[2647:2647] ;
assign sram_blwl_2647_configbus1[2647:2647] = sram_blwl_wl[2647:2647] ;
assign sram_blwl_2647_configbus0_b[2647:2647] = sram_blwl_blb[2647:2647] ;
iopad iopad_21_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[21], sram_blwl_out[2647:2647] , sram_blwl_outb[2647:2647] );
sram6T_blwl sram_blwl_2647_ (sram_blwl_out[2647], sram_blwl_out[2647], sram_blwl_outb[2647], sram_blwl_2647_configbus0[2647:2647], sram_blwl_2647_configbus1[2647:2647] , sram_blwl_2647_configbus0_b[2647:2647] );
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
input [2647:2647] sram_blwl_bl ,
input [2647:2647] sram_blwl_wl ,
input [2647:2647] sram_blwl_blb );
grid_1__0__io_5__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[21:21] ,
sram_blwl_bl[2647:2647] ,
sram_blwl_wl[2647:2647] ,
sram_blwl_blb[2647:2647] );
direct_interc direct_interc_222_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_223_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2648:2648] sram_blwl_bl ,
input [2648:2648] sram_blwl_wl ,
input [2648:2648] sram_blwl_blb );
wire [2648:2648] sram_blwl_out ;
wire [2648:2648] sram_blwl_outb ;
wire [2648:2648] sram_blwl_2648_configbus0;
wire [2648:2648] sram_blwl_2648_configbus1;
wire [2648:2648] sram_blwl_2648_configbus0_b;
assign sram_blwl_2648_configbus0[2648:2648] = sram_blwl_bl[2648:2648] ;
assign sram_blwl_2648_configbus1[2648:2648] = sram_blwl_wl[2648:2648] ;
assign sram_blwl_2648_configbus0_b[2648:2648] = sram_blwl_blb[2648:2648] ;
iopad iopad_22_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[22], sram_blwl_out[2648:2648] , sram_blwl_outb[2648:2648] );
sram6T_blwl sram_blwl_2648_ (sram_blwl_out[2648], sram_blwl_out[2648], sram_blwl_outb[2648], sram_blwl_2648_configbus0[2648:2648], sram_blwl_2648_configbus1[2648:2648] , sram_blwl_2648_configbus0_b[2648:2648] );
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
input [2648:2648] sram_blwl_bl ,
input [2648:2648] sram_blwl_wl ,
input [2648:2648] sram_blwl_blb );
grid_1__0__io_6__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[22:22] ,
sram_blwl_bl[2648:2648] ,
sram_blwl_wl[2648:2648] ,
sram_blwl_blb[2648:2648] );
direct_interc direct_interc_224_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_225_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2649:2649] sram_blwl_bl ,
input [2649:2649] sram_blwl_wl ,
input [2649:2649] sram_blwl_blb );
wire [2649:2649] sram_blwl_out ;
wire [2649:2649] sram_blwl_outb ;
wire [2649:2649] sram_blwl_2649_configbus0;
wire [2649:2649] sram_blwl_2649_configbus1;
wire [2649:2649] sram_blwl_2649_configbus0_b;
assign sram_blwl_2649_configbus0[2649:2649] = sram_blwl_bl[2649:2649] ;
assign sram_blwl_2649_configbus1[2649:2649] = sram_blwl_wl[2649:2649] ;
assign sram_blwl_2649_configbus0_b[2649:2649] = sram_blwl_blb[2649:2649] ;
iopad iopad_23_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[23], sram_blwl_out[2649:2649] , sram_blwl_outb[2649:2649] );
sram6T_blwl sram_blwl_2649_ (sram_blwl_out[2649], sram_blwl_out[2649], sram_blwl_outb[2649], sram_blwl_2649_configbus0[2649:2649], sram_blwl_2649_configbus1[2649:2649] , sram_blwl_2649_configbus0_b[2649:2649] );
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
input [2649:2649] sram_blwl_bl ,
input [2649:2649] sram_blwl_wl ,
input [2649:2649] sram_blwl_blb );
grid_1__0__io_7__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[23:23] ,
sram_blwl_bl[2649:2649] ,
sram_blwl_wl[2649:2649] ,
sram_blwl_blb[2649:2649] );
direct_interc direct_interc_226_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_227_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2642:2649] sram_blwl_bl ,
input [2642:2649] sram_blwl_wl ,
input [2642:2649] sram_blwl_blb );
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
sram_blwl_bl[2642:2642] ,
sram_blwl_wl[2642:2642] ,
sram_blwl_blb[2642:2642] );
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
sram_blwl_bl[2643:2643] ,
sram_blwl_wl[2643:2643] ,
sram_blwl_blb[2643:2643] );
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
sram_blwl_bl[2644:2644] ,
sram_blwl_wl[2644:2644] ,
sram_blwl_blb[2644:2644] );
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
sram_blwl_bl[2645:2645] ,
sram_blwl_wl[2645:2645] ,
sram_blwl_blb[2645:2645] );
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
sram_blwl_bl[2646:2646] ,
sram_blwl_wl[2646:2646] ,
sram_blwl_blb[2646:2646] );
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
sram_blwl_bl[2647:2647] ,
sram_blwl_wl[2647:2647] ,
sram_blwl_blb[2647:2647] );
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
sram_blwl_bl[2648:2648] ,
sram_blwl_wl[2648:2648] ,
sram_blwl_blb[2648:2648] );
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
sram_blwl_bl[2649:2649] ,
sram_blwl_wl[2649:2649] ,
sram_blwl_blb[2649:2649] );
endmodule
//----- END Top Protocol -----
//----- END Grid[1][0], Capactity: 8 -----

