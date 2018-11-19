//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Physical Logic Block  [1][2] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
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
input [2650:2650] sram_blwl_bl ,
input [2650:2650] sram_blwl_wl ,
input [2650:2650] sram_blwl_blb );
wire [2650:2650] sram_blwl_out ;
wire [2650:2650] sram_blwl_outb ;
wire [2650:2650] sram_blwl_2650_configbus0;
wire [2650:2650] sram_blwl_2650_configbus1;
wire [2650:2650] sram_blwl_2650_configbus0_b;
assign sram_blwl_2650_configbus0[2650:2650] = sram_blwl_bl[2650:2650] ;
assign sram_blwl_2650_configbus1[2650:2650] = sram_blwl_wl[2650:2650] ;
assign sram_blwl_2650_configbus0_b[2650:2650] = sram_blwl_blb[2650:2650] ;
iopad iopad_24_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[24], sram_blwl_out[2650:2650] , sram_blwl_outb[2650:2650] );
sram6T_blwl sram_blwl_2650_ (sram_blwl_out[2650], sram_blwl_out[2650], sram_blwl_outb[2650], sram_blwl_2650_configbus0[2650:2650], sram_blwl_2650_configbus1[2650:2650] , sram_blwl_2650_configbus0_b[2650:2650] );
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
input [2650:2650] sram_blwl_bl ,
input [2650:2650] sram_blwl_wl ,
input [2650:2650] sram_blwl_blb );
grid_1__2__io_0__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[24:24] ,
sram_blwl_bl[2650:2650] ,
sram_blwl_wl[2650:2650] ,
sram_blwl_blb[2650:2650] );
direct_interc direct_interc_228_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_229_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2651:2651] sram_blwl_bl ,
input [2651:2651] sram_blwl_wl ,
input [2651:2651] sram_blwl_blb );
wire [2651:2651] sram_blwl_out ;
wire [2651:2651] sram_blwl_outb ;
wire [2651:2651] sram_blwl_2651_configbus0;
wire [2651:2651] sram_blwl_2651_configbus1;
wire [2651:2651] sram_blwl_2651_configbus0_b;
assign sram_blwl_2651_configbus0[2651:2651] = sram_blwl_bl[2651:2651] ;
assign sram_blwl_2651_configbus1[2651:2651] = sram_blwl_wl[2651:2651] ;
assign sram_blwl_2651_configbus0_b[2651:2651] = sram_blwl_blb[2651:2651] ;
iopad iopad_25_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[25], sram_blwl_out[2651:2651] , sram_blwl_outb[2651:2651] );
sram6T_blwl sram_blwl_2651_ (sram_blwl_out[2651], sram_blwl_out[2651], sram_blwl_outb[2651], sram_blwl_2651_configbus0[2651:2651], sram_blwl_2651_configbus1[2651:2651] , sram_blwl_2651_configbus0_b[2651:2651] );
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
input [2651:2651] sram_blwl_bl ,
input [2651:2651] sram_blwl_wl ,
input [2651:2651] sram_blwl_blb );
grid_1__2__io_1__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[25:25] ,
sram_blwl_bl[2651:2651] ,
sram_blwl_wl[2651:2651] ,
sram_blwl_blb[2651:2651] );
direct_interc direct_interc_230_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_231_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2652:2652] sram_blwl_bl ,
input [2652:2652] sram_blwl_wl ,
input [2652:2652] sram_blwl_blb );
wire [2652:2652] sram_blwl_out ;
wire [2652:2652] sram_blwl_outb ;
wire [2652:2652] sram_blwl_2652_configbus0;
wire [2652:2652] sram_blwl_2652_configbus1;
wire [2652:2652] sram_blwl_2652_configbus0_b;
assign sram_blwl_2652_configbus0[2652:2652] = sram_blwl_bl[2652:2652] ;
assign sram_blwl_2652_configbus1[2652:2652] = sram_blwl_wl[2652:2652] ;
assign sram_blwl_2652_configbus0_b[2652:2652] = sram_blwl_blb[2652:2652] ;
iopad iopad_26_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[26], sram_blwl_out[2652:2652] , sram_blwl_outb[2652:2652] );
sram6T_blwl sram_blwl_2652_ (sram_blwl_out[2652], sram_blwl_out[2652], sram_blwl_outb[2652], sram_blwl_2652_configbus0[2652:2652], sram_blwl_2652_configbus1[2652:2652] , sram_blwl_2652_configbus0_b[2652:2652] );
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
input [2652:2652] sram_blwl_bl ,
input [2652:2652] sram_blwl_wl ,
input [2652:2652] sram_blwl_blb );
grid_1__2__io_2__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[26:26] ,
sram_blwl_bl[2652:2652] ,
sram_blwl_wl[2652:2652] ,
sram_blwl_blb[2652:2652] );
direct_interc direct_interc_232_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_233_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2653:2653] sram_blwl_bl ,
input [2653:2653] sram_blwl_wl ,
input [2653:2653] sram_blwl_blb );
wire [2653:2653] sram_blwl_out ;
wire [2653:2653] sram_blwl_outb ;
wire [2653:2653] sram_blwl_2653_configbus0;
wire [2653:2653] sram_blwl_2653_configbus1;
wire [2653:2653] sram_blwl_2653_configbus0_b;
assign sram_blwl_2653_configbus0[2653:2653] = sram_blwl_bl[2653:2653] ;
assign sram_blwl_2653_configbus1[2653:2653] = sram_blwl_wl[2653:2653] ;
assign sram_blwl_2653_configbus0_b[2653:2653] = sram_blwl_blb[2653:2653] ;
iopad iopad_27_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[27], sram_blwl_out[2653:2653] , sram_blwl_outb[2653:2653] );
sram6T_blwl sram_blwl_2653_ (sram_blwl_out[2653], sram_blwl_out[2653], sram_blwl_outb[2653], sram_blwl_2653_configbus0[2653:2653], sram_blwl_2653_configbus1[2653:2653] , sram_blwl_2653_configbus0_b[2653:2653] );
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
input [2653:2653] sram_blwl_bl ,
input [2653:2653] sram_blwl_wl ,
input [2653:2653] sram_blwl_blb );
grid_1__2__io_3__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[27:27] ,
sram_blwl_bl[2653:2653] ,
sram_blwl_wl[2653:2653] ,
sram_blwl_blb[2653:2653] );
direct_interc direct_interc_234_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_235_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2654:2654] sram_blwl_bl ,
input [2654:2654] sram_blwl_wl ,
input [2654:2654] sram_blwl_blb );
wire [2654:2654] sram_blwl_out ;
wire [2654:2654] sram_blwl_outb ;
wire [2654:2654] sram_blwl_2654_configbus0;
wire [2654:2654] sram_blwl_2654_configbus1;
wire [2654:2654] sram_blwl_2654_configbus0_b;
assign sram_blwl_2654_configbus0[2654:2654] = sram_blwl_bl[2654:2654] ;
assign sram_blwl_2654_configbus1[2654:2654] = sram_blwl_wl[2654:2654] ;
assign sram_blwl_2654_configbus0_b[2654:2654] = sram_blwl_blb[2654:2654] ;
iopad iopad_28_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[28], sram_blwl_out[2654:2654] , sram_blwl_outb[2654:2654] );
sram6T_blwl sram_blwl_2654_ (sram_blwl_out[2654], sram_blwl_out[2654], sram_blwl_outb[2654], sram_blwl_2654_configbus0[2654:2654], sram_blwl_2654_configbus1[2654:2654] , sram_blwl_2654_configbus0_b[2654:2654] );
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
input [2654:2654] sram_blwl_bl ,
input [2654:2654] sram_blwl_wl ,
input [2654:2654] sram_blwl_blb );
grid_1__2__io_4__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[28:28] ,
sram_blwl_bl[2654:2654] ,
sram_blwl_wl[2654:2654] ,
sram_blwl_blb[2654:2654] );
direct_interc direct_interc_236_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_237_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2655:2655] sram_blwl_bl ,
input [2655:2655] sram_blwl_wl ,
input [2655:2655] sram_blwl_blb );
wire [2655:2655] sram_blwl_out ;
wire [2655:2655] sram_blwl_outb ;
wire [2655:2655] sram_blwl_2655_configbus0;
wire [2655:2655] sram_blwl_2655_configbus1;
wire [2655:2655] sram_blwl_2655_configbus0_b;
assign sram_blwl_2655_configbus0[2655:2655] = sram_blwl_bl[2655:2655] ;
assign sram_blwl_2655_configbus1[2655:2655] = sram_blwl_wl[2655:2655] ;
assign sram_blwl_2655_configbus0_b[2655:2655] = sram_blwl_blb[2655:2655] ;
iopad iopad_29_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[29], sram_blwl_out[2655:2655] , sram_blwl_outb[2655:2655] );
sram6T_blwl sram_blwl_2655_ (sram_blwl_out[2655], sram_blwl_out[2655], sram_blwl_outb[2655], sram_blwl_2655_configbus0[2655:2655], sram_blwl_2655_configbus1[2655:2655] , sram_blwl_2655_configbus0_b[2655:2655] );
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
input [2655:2655] sram_blwl_bl ,
input [2655:2655] sram_blwl_wl ,
input [2655:2655] sram_blwl_blb );
grid_1__2__io_5__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[29:29] ,
sram_blwl_bl[2655:2655] ,
sram_blwl_wl[2655:2655] ,
sram_blwl_blb[2655:2655] );
direct_interc direct_interc_238_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_239_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2656:2656] sram_blwl_bl ,
input [2656:2656] sram_blwl_wl ,
input [2656:2656] sram_blwl_blb );
wire [2656:2656] sram_blwl_out ;
wire [2656:2656] sram_blwl_outb ;
wire [2656:2656] sram_blwl_2656_configbus0;
wire [2656:2656] sram_blwl_2656_configbus1;
wire [2656:2656] sram_blwl_2656_configbus0_b;
assign sram_blwl_2656_configbus0[2656:2656] = sram_blwl_bl[2656:2656] ;
assign sram_blwl_2656_configbus1[2656:2656] = sram_blwl_wl[2656:2656] ;
assign sram_blwl_2656_configbus0_b[2656:2656] = sram_blwl_blb[2656:2656] ;
iopad iopad_30_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[30], sram_blwl_out[2656:2656] , sram_blwl_outb[2656:2656] );
sram6T_blwl sram_blwl_2656_ (sram_blwl_out[2656], sram_blwl_out[2656], sram_blwl_outb[2656], sram_blwl_2656_configbus0[2656:2656], sram_blwl_2656_configbus1[2656:2656] , sram_blwl_2656_configbus0_b[2656:2656] );
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
input [2656:2656] sram_blwl_bl ,
input [2656:2656] sram_blwl_wl ,
input [2656:2656] sram_blwl_blb );
grid_1__2__io_6__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[30:30] ,
sram_blwl_bl[2656:2656] ,
sram_blwl_wl[2656:2656] ,
sram_blwl_blb[2656:2656] );
direct_interc direct_interc_240_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_241_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2657:2657] sram_blwl_bl ,
input [2657:2657] sram_blwl_wl ,
input [2657:2657] sram_blwl_blb );
wire [2657:2657] sram_blwl_out ;
wire [2657:2657] sram_blwl_outb ;
wire [2657:2657] sram_blwl_2657_configbus0;
wire [2657:2657] sram_blwl_2657_configbus1;
wire [2657:2657] sram_blwl_2657_configbus0_b;
assign sram_blwl_2657_configbus0[2657:2657] = sram_blwl_bl[2657:2657] ;
assign sram_blwl_2657_configbus1[2657:2657] = sram_blwl_wl[2657:2657] ;
assign sram_blwl_2657_configbus0_b[2657:2657] = sram_blwl_blb[2657:2657] ;
iopad iopad_31_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[31], sram_blwl_out[2657:2657] , sram_blwl_outb[2657:2657] );
sram6T_blwl sram_blwl_2657_ (sram_blwl_out[2657], sram_blwl_out[2657], sram_blwl_outb[2657], sram_blwl_2657_configbus0[2657:2657], sram_blwl_2657_configbus1[2657:2657] , sram_blwl_2657_configbus0_b[2657:2657] );
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
input [2657:2657] sram_blwl_bl ,
input [2657:2657] sram_blwl_wl ,
input [2657:2657] sram_blwl_blb );
grid_1__2__io_7__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[31:31] ,
sram_blwl_bl[2657:2657] ,
sram_blwl_wl[2657:2657] ,
sram_blwl_blb[2657:2657] );
direct_interc direct_interc_242_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_243_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [2650:2657] sram_blwl_bl ,
input [2650:2657] sram_blwl_wl ,
input [2650:2657] sram_blwl_blb );
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
sram_blwl_bl[2650:2650] ,
sram_blwl_wl[2650:2650] ,
sram_blwl_blb[2650:2650] );
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
sram_blwl_bl[2651:2651] ,
sram_blwl_wl[2651:2651] ,
sram_blwl_blb[2651:2651] );
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
sram_blwl_bl[2652:2652] ,
sram_blwl_wl[2652:2652] ,
sram_blwl_blb[2652:2652] );
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
sram_blwl_bl[2653:2653] ,
sram_blwl_wl[2653:2653] ,
sram_blwl_blb[2653:2653] );
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
sram_blwl_bl[2654:2654] ,
sram_blwl_wl[2654:2654] ,
sram_blwl_blb[2654:2654] );
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
sram_blwl_bl[2655:2655] ,
sram_blwl_wl[2655:2655] ,
sram_blwl_blb[2655:2655] );
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
sram_blwl_bl[2656:2656] ,
sram_blwl_wl[2656:2656] ,
sram_blwl_blb[2656:2656] );
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
sram_blwl_bl[2657:2657] ,
sram_blwl_wl[2657:2657] ,
sram_blwl_blb[2657:2657] );
endmodule
//----- END Top Protocol -----
//----- END Grid[1][2], Capactity: 8 -----

