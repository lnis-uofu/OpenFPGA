//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Physical Logic Block  [2][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
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
input [337:337] sram_blwl_bl ,
input [337:337] sram_blwl_wl ,
input [337:337] sram_blwl_blb );
wire [337:337] sram_blwl_out ;
wire [337:337] sram_blwl_outb ;
wire [337:337] sram_blwl_337_configbus0;
wire [337:337] sram_blwl_337_configbus1;
wire [337:337] sram_blwl_337_configbus0_b;
assign sram_blwl_337_configbus0[337:337] = sram_blwl_bl[337:337] ;
assign sram_blwl_337_configbus1[337:337] = sram_blwl_wl[337:337] ;
assign sram_blwl_337_configbus0_b[337:337] = sram_blwl_blb[337:337] ;
iopad iopad_8_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[8], sram_blwl_out[337:337] , sram_blwl_outb[337:337] );
sram6T_blwl sram_blwl_337_ (sram_blwl_out[337], sram_blwl_out[337], sram_blwl_outb[337], sram_blwl_337_configbus0[337:337], sram_blwl_337_configbus1[337:337] , sram_blwl_337_configbus0_b[337:337] );
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
input [337:337] sram_blwl_bl ,
input [337:337] sram_blwl_wl ,
input [337:337] sram_blwl_blb );
grid_2__1__io_0__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[8:8] ,
sram_blwl_bl[337:337] ,
sram_blwl_wl[337:337] ,
sram_blwl_blb[337:337] );
direct_interc direct_interc_30_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_31_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [338:338] sram_blwl_bl ,
input [338:338] sram_blwl_wl ,
input [338:338] sram_blwl_blb );
wire [338:338] sram_blwl_out ;
wire [338:338] sram_blwl_outb ;
wire [338:338] sram_blwl_338_configbus0;
wire [338:338] sram_blwl_338_configbus1;
wire [338:338] sram_blwl_338_configbus0_b;
assign sram_blwl_338_configbus0[338:338] = sram_blwl_bl[338:338] ;
assign sram_blwl_338_configbus1[338:338] = sram_blwl_wl[338:338] ;
assign sram_blwl_338_configbus0_b[338:338] = sram_blwl_blb[338:338] ;
iopad iopad_9_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[9], sram_blwl_out[338:338] , sram_blwl_outb[338:338] );
sram6T_blwl sram_blwl_338_ (sram_blwl_out[338], sram_blwl_out[338], sram_blwl_outb[338], sram_blwl_338_configbus0[338:338], sram_blwl_338_configbus1[338:338] , sram_blwl_338_configbus0_b[338:338] );
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
input [338:338] sram_blwl_bl ,
input [338:338] sram_blwl_wl ,
input [338:338] sram_blwl_blb );
grid_2__1__io_1__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[9:9] ,
sram_blwl_bl[338:338] ,
sram_blwl_wl[338:338] ,
sram_blwl_blb[338:338] );
direct_interc direct_interc_32_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_33_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [339:339] sram_blwl_bl ,
input [339:339] sram_blwl_wl ,
input [339:339] sram_blwl_blb );
wire [339:339] sram_blwl_out ;
wire [339:339] sram_blwl_outb ;
wire [339:339] sram_blwl_339_configbus0;
wire [339:339] sram_blwl_339_configbus1;
wire [339:339] sram_blwl_339_configbus0_b;
assign sram_blwl_339_configbus0[339:339] = sram_blwl_bl[339:339] ;
assign sram_blwl_339_configbus1[339:339] = sram_blwl_wl[339:339] ;
assign sram_blwl_339_configbus0_b[339:339] = sram_blwl_blb[339:339] ;
iopad iopad_10_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[10], sram_blwl_out[339:339] , sram_blwl_outb[339:339] );
sram6T_blwl sram_blwl_339_ (sram_blwl_out[339], sram_blwl_out[339], sram_blwl_outb[339], sram_blwl_339_configbus0[339:339], sram_blwl_339_configbus1[339:339] , sram_blwl_339_configbus0_b[339:339] );
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
input [339:339] sram_blwl_bl ,
input [339:339] sram_blwl_wl ,
input [339:339] sram_blwl_blb );
grid_2__1__io_2__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[10:10] ,
sram_blwl_bl[339:339] ,
sram_blwl_wl[339:339] ,
sram_blwl_blb[339:339] );
direct_interc direct_interc_34_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_35_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [340:340] sram_blwl_bl ,
input [340:340] sram_blwl_wl ,
input [340:340] sram_blwl_blb );
wire [340:340] sram_blwl_out ;
wire [340:340] sram_blwl_outb ;
wire [340:340] sram_blwl_340_configbus0;
wire [340:340] sram_blwl_340_configbus1;
wire [340:340] sram_blwl_340_configbus0_b;
assign sram_blwl_340_configbus0[340:340] = sram_blwl_bl[340:340] ;
assign sram_blwl_340_configbus1[340:340] = sram_blwl_wl[340:340] ;
assign sram_blwl_340_configbus0_b[340:340] = sram_blwl_blb[340:340] ;
iopad iopad_11_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[11], sram_blwl_out[340:340] , sram_blwl_outb[340:340] );
sram6T_blwl sram_blwl_340_ (sram_blwl_out[340], sram_blwl_out[340], sram_blwl_outb[340], sram_blwl_340_configbus0[340:340], sram_blwl_340_configbus1[340:340] , sram_blwl_340_configbus0_b[340:340] );
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
input [340:340] sram_blwl_bl ,
input [340:340] sram_blwl_wl ,
input [340:340] sram_blwl_blb );
grid_2__1__io_3__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[11:11] ,
sram_blwl_bl[340:340] ,
sram_blwl_wl[340:340] ,
sram_blwl_blb[340:340] );
direct_interc direct_interc_36_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_37_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [341:341] sram_blwl_bl ,
input [341:341] sram_blwl_wl ,
input [341:341] sram_blwl_blb );
wire [341:341] sram_blwl_out ;
wire [341:341] sram_blwl_outb ;
wire [341:341] sram_blwl_341_configbus0;
wire [341:341] sram_blwl_341_configbus1;
wire [341:341] sram_blwl_341_configbus0_b;
assign sram_blwl_341_configbus0[341:341] = sram_blwl_bl[341:341] ;
assign sram_blwl_341_configbus1[341:341] = sram_blwl_wl[341:341] ;
assign sram_blwl_341_configbus0_b[341:341] = sram_blwl_blb[341:341] ;
iopad iopad_12_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[12], sram_blwl_out[341:341] , sram_blwl_outb[341:341] );
sram6T_blwl sram_blwl_341_ (sram_blwl_out[341], sram_blwl_out[341], sram_blwl_outb[341], sram_blwl_341_configbus0[341:341], sram_blwl_341_configbus1[341:341] , sram_blwl_341_configbus0_b[341:341] );
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
input [341:341] sram_blwl_bl ,
input [341:341] sram_blwl_wl ,
input [341:341] sram_blwl_blb );
grid_2__1__io_4__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[12:12] ,
sram_blwl_bl[341:341] ,
sram_blwl_wl[341:341] ,
sram_blwl_blb[341:341] );
direct_interc direct_interc_38_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_39_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [342:342] sram_blwl_bl ,
input [342:342] sram_blwl_wl ,
input [342:342] sram_blwl_blb );
wire [342:342] sram_blwl_out ;
wire [342:342] sram_blwl_outb ;
wire [342:342] sram_blwl_342_configbus0;
wire [342:342] sram_blwl_342_configbus1;
wire [342:342] sram_blwl_342_configbus0_b;
assign sram_blwl_342_configbus0[342:342] = sram_blwl_bl[342:342] ;
assign sram_blwl_342_configbus1[342:342] = sram_blwl_wl[342:342] ;
assign sram_blwl_342_configbus0_b[342:342] = sram_blwl_blb[342:342] ;
iopad iopad_13_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[13], sram_blwl_out[342:342] , sram_blwl_outb[342:342] );
sram6T_blwl sram_blwl_342_ (sram_blwl_out[342], sram_blwl_out[342], sram_blwl_outb[342], sram_blwl_342_configbus0[342:342], sram_blwl_342_configbus1[342:342] , sram_blwl_342_configbus0_b[342:342] );
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
input [342:342] sram_blwl_bl ,
input [342:342] sram_blwl_wl ,
input [342:342] sram_blwl_blb );
grid_2__1__io_5__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[13:13] ,
sram_blwl_bl[342:342] ,
sram_blwl_wl[342:342] ,
sram_blwl_blb[342:342] );
direct_interc direct_interc_40_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_41_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [343:343] sram_blwl_bl ,
input [343:343] sram_blwl_wl ,
input [343:343] sram_blwl_blb );
wire [343:343] sram_blwl_out ;
wire [343:343] sram_blwl_outb ;
wire [343:343] sram_blwl_343_configbus0;
wire [343:343] sram_blwl_343_configbus1;
wire [343:343] sram_blwl_343_configbus0_b;
assign sram_blwl_343_configbus0[343:343] = sram_blwl_bl[343:343] ;
assign sram_blwl_343_configbus1[343:343] = sram_blwl_wl[343:343] ;
assign sram_blwl_343_configbus0_b[343:343] = sram_blwl_blb[343:343] ;
iopad iopad_14_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[14], sram_blwl_out[343:343] , sram_blwl_outb[343:343] );
sram6T_blwl sram_blwl_343_ (sram_blwl_out[343], sram_blwl_out[343], sram_blwl_outb[343], sram_blwl_343_configbus0[343:343], sram_blwl_343_configbus1[343:343] , sram_blwl_343_configbus0_b[343:343] );
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
input [343:343] sram_blwl_bl ,
input [343:343] sram_blwl_wl ,
input [343:343] sram_blwl_blb );
grid_2__1__io_6__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[14:14] ,
sram_blwl_bl[343:343] ,
sram_blwl_wl[343:343] ,
sram_blwl_blb[343:343] );
direct_interc direct_interc_42_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_43_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [344:344] sram_blwl_bl ,
input [344:344] sram_blwl_wl ,
input [344:344] sram_blwl_blb );
wire [344:344] sram_blwl_out ;
wire [344:344] sram_blwl_outb ;
wire [344:344] sram_blwl_344_configbus0;
wire [344:344] sram_blwl_344_configbus1;
wire [344:344] sram_blwl_344_configbus0_b;
assign sram_blwl_344_configbus0[344:344] = sram_blwl_bl[344:344] ;
assign sram_blwl_344_configbus1[344:344] = sram_blwl_wl[344:344] ;
assign sram_blwl_344_configbus0_b[344:344] = sram_blwl_blb[344:344] ;
iopad iopad_15_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,gfpga_pad_iopad[15], sram_blwl_out[344:344] , sram_blwl_outb[344:344] );
sram6T_blwl sram_blwl_344_ (sram_blwl_out[344], sram_blwl_out[344], sram_blwl_outb[344], sram_blwl_344_configbus0[344:344], sram_blwl_344_configbus1[344:344] , sram_blwl_344_configbus0_b[344:344] );
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
input [344:344] sram_blwl_bl ,
input [344:344] sram_blwl_wl ,
input [344:344] sram_blwl_blb );
grid_2__1__io_7__mode_io_phy__iopad_0_ iopad_0_ (
//----- BEGIN Global ports of SPICE_MODEL(iopad) -----
zin[0:0]
//----- END Global ports of SPICE_MODEL(iopad)-----
,
 iopad_0___outpad_0_,  iopad_0___inpad_0_,
gfpga_pad_iopad[15:15] ,
sram_blwl_bl[344:344] ,
sram_blwl_wl[344:344] ,
sram_blwl_blb[344:344] );
direct_interc direct_interc_44_ (iopad_0___inpad_0_, mode_io_phy___inpad_0_ );
direct_interc direct_interc_45_ (mode_io_phy___outpad_0_, iopad_0___outpad_0_ );
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
input [337:344] sram_blwl_bl ,
input [337:344] sram_blwl_wl ,
input [337:344] sram_blwl_blb );
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
sram_blwl_bl[337:337] ,
sram_blwl_wl[337:337] ,
sram_blwl_blb[337:337] );
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
sram_blwl_bl[338:338] ,
sram_blwl_wl[338:338] ,
sram_blwl_blb[338:338] );
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
sram_blwl_bl[339:339] ,
sram_blwl_wl[339:339] ,
sram_blwl_blb[339:339] );
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
sram_blwl_bl[340:340] ,
sram_blwl_wl[340:340] ,
sram_blwl_blb[340:340] );
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
sram_blwl_bl[341:341] ,
sram_blwl_wl[341:341] ,
sram_blwl_blb[341:341] );
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
sram_blwl_bl[342:342] ,
sram_blwl_wl[342:342] ,
sram_blwl_blb[342:342] );
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
sram_blwl_bl[343:343] ,
sram_blwl_wl[343:343] ,
sram_blwl_blb[343:343] );
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
sram_blwl_bl[344:344] ,
sram_blwl_wl[344:344] ,
sram_blwl_blb[344:344] );
endmodule
//----- END Top Protocol -----
//----- END Grid[2][1], Capactity: 8 -----

