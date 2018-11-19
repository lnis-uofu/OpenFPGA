//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Logic Block  [1][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Grid[1][1] type_descriptor: clb[0] -----
//----- LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----
module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ (
input wire lut6_0___in_0_,
input wire lut6_0___in_1_,
input wire lut6_0___in_2_,
input wire lut6_0___in_3_,
input wire lut6_0___in_4_,
input wire lut6_0___in_5_,
output wire lut6_0___out_0_,
input [1016:1079] sram_blwl_bl ,
input [1016:1079] sram_blwl_wl ,
input [1016:1079] sram_blwl_blb );
wire [0:5] lut6_0___in;
assign lut6_0___in[0] = lut6_0___in_0_;
assign lut6_0___in[1] = lut6_0___in_1_;
assign lut6_0___in[2] = lut6_0___in_2_;
assign lut6_0___in[3] = lut6_0___in_3_;
assign lut6_0___in[4] = lut6_0___in_4_;
assign lut6_0___in[5] = lut6_0___in_5_;
wire [0:0] lut6_0___out;
assign lut6_0___out_0_ = lut6_0___out[0];
wire [1016:1079] sram_blwl_out ;
wire [1016:1079] sram_blwl_outb ;
lut6 lut6_0_ (
//----- Input and output ports -----
 lut6_0___in[0:5] ,  lut6_0___out[0:0],//----- SRAM ports -----
sram_blwl_out[1016:1079] , sram_blwl_outb[1016:1079] );
//----- Truth Table for LUT[0], size=6. -----
//----- SRAM bits for LUT[0], size=6, num_sram=64. -----
//-----0000000000000000000000000000000000000000000000000000000000000000-----
wire [1016:1016] sram_blwl_1016_configbus0;
wire [1016:1016] sram_blwl_1016_configbus1;
wire [1016:1016] sram_blwl_1016_configbus0_b;
assign sram_blwl_1016_configbus0[1016:1016] = sram_blwl_bl[1016:1016] ;
assign sram_blwl_1016_configbus1[1016:1016] = sram_blwl_wl[1016:1016] ;
assign sram_blwl_1016_configbus0_b[1016:1016] = sram_blwl_blb[1016:1016] ;
sram6T_blwl sram_blwl_1016_ (sram_blwl_out[1016], sram_blwl_out[1016], sram_blwl_outb[1016], sram_blwl_1016_configbus0[1016:1016], sram_blwl_1016_configbus1[1016:1016] , sram_blwl_1016_configbus0_b[1016:1016] );
wire [1017:1017] sram_blwl_1017_configbus0;
wire [1017:1017] sram_blwl_1017_configbus1;
wire [1017:1017] sram_blwl_1017_configbus0_b;
assign sram_blwl_1017_configbus0[1017:1017] = sram_blwl_bl[1017:1017] ;
assign sram_blwl_1017_configbus1[1017:1017] = sram_blwl_wl[1017:1017] ;
assign sram_blwl_1017_configbus0_b[1017:1017] = sram_blwl_blb[1017:1017] ;
sram6T_blwl sram_blwl_1017_ (sram_blwl_out[1017], sram_blwl_out[1017], sram_blwl_outb[1017], sram_blwl_1017_configbus0[1017:1017], sram_blwl_1017_configbus1[1017:1017] , sram_blwl_1017_configbus0_b[1017:1017] );
wire [1018:1018] sram_blwl_1018_configbus0;
wire [1018:1018] sram_blwl_1018_configbus1;
wire [1018:1018] sram_blwl_1018_configbus0_b;
assign sram_blwl_1018_configbus0[1018:1018] = sram_blwl_bl[1018:1018] ;
assign sram_blwl_1018_configbus1[1018:1018] = sram_blwl_wl[1018:1018] ;
assign sram_blwl_1018_configbus0_b[1018:1018] = sram_blwl_blb[1018:1018] ;
sram6T_blwl sram_blwl_1018_ (sram_blwl_out[1018], sram_blwl_out[1018], sram_blwl_outb[1018], sram_blwl_1018_configbus0[1018:1018], sram_blwl_1018_configbus1[1018:1018] , sram_blwl_1018_configbus0_b[1018:1018] );
wire [1019:1019] sram_blwl_1019_configbus0;
wire [1019:1019] sram_blwl_1019_configbus1;
wire [1019:1019] sram_blwl_1019_configbus0_b;
assign sram_blwl_1019_configbus0[1019:1019] = sram_blwl_bl[1019:1019] ;
assign sram_blwl_1019_configbus1[1019:1019] = sram_blwl_wl[1019:1019] ;
assign sram_blwl_1019_configbus0_b[1019:1019] = sram_blwl_blb[1019:1019] ;
sram6T_blwl sram_blwl_1019_ (sram_blwl_out[1019], sram_blwl_out[1019], sram_blwl_outb[1019], sram_blwl_1019_configbus0[1019:1019], sram_blwl_1019_configbus1[1019:1019] , sram_blwl_1019_configbus0_b[1019:1019] );
wire [1020:1020] sram_blwl_1020_configbus0;
wire [1020:1020] sram_blwl_1020_configbus1;
wire [1020:1020] sram_blwl_1020_configbus0_b;
assign sram_blwl_1020_configbus0[1020:1020] = sram_blwl_bl[1020:1020] ;
assign sram_blwl_1020_configbus1[1020:1020] = sram_blwl_wl[1020:1020] ;
assign sram_blwl_1020_configbus0_b[1020:1020] = sram_blwl_blb[1020:1020] ;
sram6T_blwl sram_blwl_1020_ (sram_blwl_out[1020], sram_blwl_out[1020], sram_blwl_outb[1020], sram_blwl_1020_configbus0[1020:1020], sram_blwl_1020_configbus1[1020:1020] , sram_blwl_1020_configbus0_b[1020:1020] );
wire [1021:1021] sram_blwl_1021_configbus0;
wire [1021:1021] sram_blwl_1021_configbus1;
wire [1021:1021] sram_blwl_1021_configbus0_b;
assign sram_blwl_1021_configbus0[1021:1021] = sram_blwl_bl[1021:1021] ;
assign sram_blwl_1021_configbus1[1021:1021] = sram_blwl_wl[1021:1021] ;
assign sram_blwl_1021_configbus0_b[1021:1021] = sram_blwl_blb[1021:1021] ;
sram6T_blwl sram_blwl_1021_ (sram_blwl_out[1021], sram_blwl_out[1021], sram_blwl_outb[1021], sram_blwl_1021_configbus0[1021:1021], sram_blwl_1021_configbus1[1021:1021] , sram_blwl_1021_configbus0_b[1021:1021] );
wire [1022:1022] sram_blwl_1022_configbus0;
wire [1022:1022] sram_blwl_1022_configbus1;
wire [1022:1022] sram_blwl_1022_configbus0_b;
assign sram_blwl_1022_configbus0[1022:1022] = sram_blwl_bl[1022:1022] ;
assign sram_blwl_1022_configbus1[1022:1022] = sram_blwl_wl[1022:1022] ;
assign sram_blwl_1022_configbus0_b[1022:1022] = sram_blwl_blb[1022:1022] ;
sram6T_blwl sram_blwl_1022_ (sram_blwl_out[1022], sram_blwl_out[1022], sram_blwl_outb[1022], sram_blwl_1022_configbus0[1022:1022], sram_blwl_1022_configbus1[1022:1022] , sram_blwl_1022_configbus0_b[1022:1022] );
wire [1023:1023] sram_blwl_1023_configbus0;
wire [1023:1023] sram_blwl_1023_configbus1;
wire [1023:1023] sram_blwl_1023_configbus0_b;
assign sram_blwl_1023_configbus0[1023:1023] = sram_blwl_bl[1023:1023] ;
assign sram_blwl_1023_configbus1[1023:1023] = sram_blwl_wl[1023:1023] ;
assign sram_blwl_1023_configbus0_b[1023:1023] = sram_blwl_blb[1023:1023] ;
sram6T_blwl sram_blwl_1023_ (sram_blwl_out[1023], sram_blwl_out[1023], sram_blwl_outb[1023], sram_blwl_1023_configbus0[1023:1023], sram_blwl_1023_configbus1[1023:1023] , sram_blwl_1023_configbus0_b[1023:1023] );
wire [1024:1024] sram_blwl_1024_configbus0;
wire [1024:1024] sram_blwl_1024_configbus1;
wire [1024:1024] sram_blwl_1024_configbus0_b;
assign sram_blwl_1024_configbus0[1024:1024] = sram_blwl_bl[1024:1024] ;
assign sram_blwl_1024_configbus1[1024:1024] = sram_blwl_wl[1024:1024] ;
assign sram_blwl_1024_configbus0_b[1024:1024] = sram_blwl_blb[1024:1024] ;
sram6T_blwl sram_blwl_1024_ (sram_blwl_out[1024], sram_blwl_out[1024], sram_blwl_outb[1024], sram_blwl_1024_configbus0[1024:1024], sram_blwl_1024_configbus1[1024:1024] , sram_blwl_1024_configbus0_b[1024:1024] );
wire [1025:1025] sram_blwl_1025_configbus0;
wire [1025:1025] sram_blwl_1025_configbus1;
wire [1025:1025] sram_blwl_1025_configbus0_b;
assign sram_blwl_1025_configbus0[1025:1025] = sram_blwl_bl[1025:1025] ;
assign sram_blwl_1025_configbus1[1025:1025] = sram_blwl_wl[1025:1025] ;
assign sram_blwl_1025_configbus0_b[1025:1025] = sram_blwl_blb[1025:1025] ;
sram6T_blwl sram_blwl_1025_ (sram_blwl_out[1025], sram_blwl_out[1025], sram_blwl_outb[1025], sram_blwl_1025_configbus0[1025:1025], sram_blwl_1025_configbus1[1025:1025] , sram_blwl_1025_configbus0_b[1025:1025] );
wire [1026:1026] sram_blwl_1026_configbus0;
wire [1026:1026] sram_blwl_1026_configbus1;
wire [1026:1026] sram_blwl_1026_configbus0_b;
assign sram_blwl_1026_configbus0[1026:1026] = sram_blwl_bl[1026:1026] ;
assign sram_blwl_1026_configbus1[1026:1026] = sram_blwl_wl[1026:1026] ;
assign sram_blwl_1026_configbus0_b[1026:1026] = sram_blwl_blb[1026:1026] ;
sram6T_blwl sram_blwl_1026_ (sram_blwl_out[1026], sram_blwl_out[1026], sram_blwl_outb[1026], sram_blwl_1026_configbus0[1026:1026], sram_blwl_1026_configbus1[1026:1026] , sram_blwl_1026_configbus0_b[1026:1026] );
wire [1027:1027] sram_blwl_1027_configbus0;
wire [1027:1027] sram_blwl_1027_configbus1;
wire [1027:1027] sram_blwl_1027_configbus0_b;
assign sram_blwl_1027_configbus0[1027:1027] = sram_blwl_bl[1027:1027] ;
assign sram_blwl_1027_configbus1[1027:1027] = sram_blwl_wl[1027:1027] ;
assign sram_blwl_1027_configbus0_b[1027:1027] = sram_blwl_blb[1027:1027] ;
sram6T_blwl sram_blwl_1027_ (sram_blwl_out[1027], sram_blwl_out[1027], sram_blwl_outb[1027], sram_blwl_1027_configbus0[1027:1027], sram_blwl_1027_configbus1[1027:1027] , sram_blwl_1027_configbus0_b[1027:1027] );
wire [1028:1028] sram_blwl_1028_configbus0;
wire [1028:1028] sram_blwl_1028_configbus1;
wire [1028:1028] sram_blwl_1028_configbus0_b;
assign sram_blwl_1028_configbus0[1028:1028] = sram_blwl_bl[1028:1028] ;
assign sram_blwl_1028_configbus1[1028:1028] = sram_blwl_wl[1028:1028] ;
assign sram_blwl_1028_configbus0_b[1028:1028] = sram_blwl_blb[1028:1028] ;
sram6T_blwl sram_blwl_1028_ (sram_blwl_out[1028], sram_blwl_out[1028], sram_blwl_outb[1028], sram_blwl_1028_configbus0[1028:1028], sram_blwl_1028_configbus1[1028:1028] , sram_blwl_1028_configbus0_b[1028:1028] );
wire [1029:1029] sram_blwl_1029_configbus0;
wire [1029:1029] sram_blwl_1029_configbus1;
wire [1029:1029] sram_blwl_1029_configbus0_b;
assign sram_blwl_1029_configbus0[1029:1029] = sram_blwl_bl[1029:1029] ;
assign sram_blwl_1029_configbus1[1029:1029] = sram_blwl_wl[1029:1029] ;
assign sram_blwl_1029_configbus0_b[1029:1029] = sram_blwl_blb[1029:1029] ;
sram6T_blwl sram_blwl_1029_ (sram_blwl_out[1029], sram_blwl_out[1029], sram_blwl_outb[1029], sram_blwl_1029_configbus0[1029:1029], sram_blwl_1029_configbus1[1029:1029] , sram_blwl_1029_configbus0_b[1029:1029] );
wire [1030:1030] sram_blwl_1030_configbus0;
wire [1030:1030] sram_blwl_1030_configbus1;
wire [1030:1030] sram_blwl_1030_configbus0_b;
assign sram_blwl_1030_configbus0[1030:1030] = sram_blwl_bl[1030:1030] ;
assign sram_blwl_1030_configbus1[1030:1030] = sram_blwl_wl[1030:1030] ;
assign sram_blwl_1030_configbus0_b[1030:1030] = sram_blwl_blb[1030:1030] ;
sram6T_blwl sram_blwl_1030_ (sram_blwl_out[1030], sram_blwl_out[1030], sram_blwl_outb[1030], sram_blwl_1030_configbus0[1030:1030], sram_blwl_1030_configbus1[1030:1030] , sram_blwl_1030_configbus0_b[1030:1030] );
wire [1031:1031] sram_blwl_1031_configbus0;
wire [1031:1031] sram_blwl_1031_configbus1;
wire [1031:1031] sram_blwl_1031_configbus0_b;
assign sram_blwl_1031_configbus0[1031:1031] = sram_blwl_bl[1031:1031] ;
assign sram_blwl_1031_configbus1[1031:1031] = sram_blwl_wl[1031:1031] ;
assign sram_blwl_1031_configbus0_b[1031:1031] = sram_blwl_blb[1031:1031] ;
sram6T_blwl sram_blwl_1031_ (sram_blwl_out[1031], sram_blwl_out[1031], sram_blwl_outb[1031], sram_blwl_1031_configbus0[1031:1031], sram_blwl_1031_configbus1[1031:1031] , sram_blwl_1031_configbus0_b[1031:1031] );
wire [1032:1032] sram_blwl_1032_configbus0;
wire [1032:1032] sram_blwl_1032_configbus1;
wire [1032:1032] sram_blwl_1032_configbus0_b;
assign sram_blwl_1032_configbus0[1032:1032] = sram_blwl_bl[1032:1032] ;
assign sram_blwl_1032_configbus1[1032:1032] = sram_blwl_wl[1032:1032] ;
assign sram_blwl_1032_configbus0_b[1032:1032] = sram_blwl_blb[1032:1032] ;
sram6T_blwl sram_blwl_1032_ (sram_blwl_out[1032], sram_blwl_out[1032], sram_blwl_outb[1032], sram_blwl_1032_configbus0[1032:1032], sram_blwl_1032_configbus1[1032:1032] , sram_blwl_1032_configbus0_b[1032:1032] );
wire [1033:1033] sram_blwl_1033_configbus0;
wire [1033:1033] sram_blwl_1033_configbus1;
wire [1033:1033] sram_blwl_1033_configbus0_b;
assign sram_blwl_1033_configbus0[1033:1033] = sram_blwl_bl[1033:1033] ;
assign sram_blwl_1033_configbus1[1033:1033] = sram_blwl_wl[1033:1033] ;
assign sram_blwl_1033_configbus0_b[1033:1033] = sram_blwl_blb[1033:1033] ;
sram6T_blwl sram_blwl_1033_ (sram_blwl_out[1033], sram_blwl_out[1033], sram_blwl_outb[1033], sram_blwl_1033_configbus0[1033:1033], sram_blwl_1033_configbus1[1033:1033] , sram_blwl_1033_configbus0_b[1033:1033] );
wire [1034:1034] sram_blwl_1034_configbus0;
wire [1034:1034] sram_blwl_1034_configbus1;
wire [1034:1034] sram_blwl_1034_configbus0_b;
assign sram_blwl_1034_configbus0[1034:1034] = sram_blwl_bl[1034:1034] ;
assign sram_blwl_1034_configbus1[1034:1034] = sram_blwl_wl[1034:1034] ;
assign sram_blwl_1034_configbus0_b[1034:1034] = sram_blwl_blb[1034:1034] ;
sram6T_blwl sram_blwl_1034_ (sram_blwl_out[1034], sram_blwl_out[1034], sram_blwl_outb[1034], sram_blwl_1034_configbus0[1034:1034], sram_blwl_1034_configbus1[1034:1034] , sram_blwl_1034_configbus0_b[1034:1034] );
wire [1035:1035] sram_blwl_1035_configbus0;
wire [1035:1035] sram_blwl_1035_configbus1;
wire [1035:1035] sram_blwl_1035_configbus0_b;
assign sram_blwl_1035_configbus0[1035:1035] = sram_blwl_bl[1035:1035] ;
assign sram_blwl_1035_configbus1[1035:1035] = sram_blwl_wl[1035:1035] ;
assign sram_blwl_1035_configbus0_b[1035:1035] = sram_blwl_blb[1035:1035] ;
sram6T_blwl sram_blwl_1035_ (sram_blwl_out[1035], sram_blwl_out[1035], sram_blwl_outb[1035], sram_blwl_1035_configbus0[1035:1035], sram_blwl_1035_configbus1[1035:1035] , sram_blwl_1035_configbus0_b[1035:1035] );
wire [1036:1036] sram_blwl_1036_configbus0;
wire [1036:1036] sram_blwl_1036_configbus1;
wire [1036:1036] sram_blwl_1036_configbus0_b;
assign sram_blwl_1036_configbus0[1036:1036] = sram_blwl_bl[1036:1036] ;
assign sram_blwl_1036_configbus1[1036:1036] = sram_blwl_wl[1036:1036] ;
assign sram_blwl_1036_configbus0_b[1036:1036] = sram_blwl_blb[1036:1036] ;
sram6T_blwl sram_blwl_1036_ (sram_blwl_out[1036], sram_blwl_out[1036], sram_blwl_outb[1036], sram_blwl_1036_configbus0[1036:1036], sram_blwl_1036_configbus1[1036:1036] , sram_blwl_1036_configbus0_b[1036:1036] );
wire [1037:1037] sram_blwl_1037_configbus0;
wire [1037:1037] sram_blwl_1037_configbus1;
wire [1037:1037] sram_blwl_1037_configbus0_b;
assign sram_blwl_1037_configbus0[1037:1037] = sram_blwl_bl[1037:1037] ;
assign sram_blwl_1037_configbus1[1037:1037] = sram_blwl_wl[1037:1037] ;
assign sram_blwl_1037_configbus0_b[1037:1037] = sram_blwl_blb[1037:1037] ;
sram6T_blwl sram_blwl_1037_ (sram_blwl_out[1037], sram_blwl_out[1037], sram_blwl_outb[1037], sram_blwl_1037_configbus0[1037:1037], sram_blwl_1037_configbus1[1037:1037] , sram_blwl_1037_configbus0_b[1037:1037] );
wire [1038:1038] sram_blwl_1038_configbus0;
wire [1038:1038] sram_blwl_1038_configbus1;
wire [1038:1038] sram_blwl_1038_configbus0_b;
assign sram_blwl_1038_configbus0[1038:1038] = sram_blwl_bl[1038:1038] ;
assign sram_blwl_1038_configbus1[1038:1038] = sram_blwl_wl[1038:1038] ;
assign sram_blwl_1038_configbus0_b[1038:1038] = sram_blwl_blb[1038:1038] ;
sram6T_blwl sram_blwl_1038_ (sram_blwl_out[1038], sram_blwl_out[1038], sram_blwl_outb[1038], sram_blwl_1038_configbus0[1038:1038], sram_blwl_1038_configbus1[1038:1038] , sram_blwl_1038_configbus0_b[1038:1038] );
wire [1039:1039] sram_blwl_1039_configbus0;
wire [1039:1039] sram_blwl_1039_configbus1;
wire [1039:1039] sram_blwl_1039_configbus0_b;
assign sram_blwl_1039_configbus0[1039:1039] = sram_blwl_bl[1039:1039] ;
assign sram_blwl_1039_configbus1[1039:1039] = sram_blwl_wl[1039:1039] ;
assign sram_blwl_1039_configbus0_b[1039:1039] = sram_blwl_blb[1039:1039] ;
sram6T_blwl sram_blwl_1039_ (sram_blwl_out[1039], sram_blwl_out[1039], sram_blwl_outb[1039], sram_blwl_1039_configbus0[1039:1039], sram_blwl_1039_configbus1[1039:1039] , sram_blwl_1039_configbus0_b[1039:1039] );
wire [1040:1040] sram_blwl_1040_configbus0;
wire [1040:1040] sram_blwl_1040_configbus1;
wire [1040:1040] sram_blwl_1040_configbus0_b;
assign sram_blwl_1040_configbus0[1040:1040] = sram_blwl_bl[1040:1040] ;
assign sram_blwl_1040_configbus1[1040:1040] = sram_blwl_wl[1040:1040] ;
assign sram_blwl_1040_configbus0_b[1040:1040] = sram_blwl_blb[1040:1040] ;
sram6T_blwl sram_blwl_1040_ (sram_blwl_out[1040], sram_blwl_out[1040], sram_blwl_outb[1040], sram_blwl_1040_configbus0[1040:1040], sram_blwl_1040_configbus1[1040:1040] , sram_blwl_1040_configbus0_b[1040:1040] );
wire [1041:1041] sram_blwl_1041_configbus0;
wire [1041:1041] sram_blwl_1041_configbus1;
wire [1041:1041] sram_blwl_1041_configbus0_b;
assign sram_blwl_1041_configbus0[1041:1041] = sram_blwl_bl[1041:1041] ;
assign sram_blwl_1041_configbus1[1041:1041] = sram_blwl_wl[1041:1041] ;
assign sram_blwl_1041_configbus0_b[1041:1041] = sram_blwl_blb[1041:1041] ;
sram6T_blwl sram_blwl_1041_ (sram_blwl_out[1041], sram_blwl_out[1041], sram_blwl_outb[1041], sram_blwl_1041_configbus0[1041:1041], sram_blwl_1041_configbus1[1041:1041] , sram_blwl_1041_configbus0_b[1041:1041] );
wire [1042:1042] sram_blwl_1042_configbus0;
wire [1042:1042] sram_blwl_1042_configbus1;
wire [1042:1042] sram_blwl_1042_configbus0_b;
assign sram_blwl_1042_configbus0[1042:1042] = sram_blwl_bl[1042:1042] ;
assign sram_blwl_1042_configbus1[1042:1042] = sram_blwl_wl[1042:1042] ;
assign sram_blwl_1042_configbus0_b[1042:1042] = sram_blwl_blb[1042:1042] ;
sram6T_blwl sram_blwl_1042_ (sram_blwl_out[1042], sram_blwl_out[1042], sram_blwl_outb[1042], sram_blwl_1042_configbus0[1042:1042], sram_blwl_1042_configbus1[1042:1042] , sram_blwl_1042_configbus0_b[1042:1042] );
wire [1043:1043] sram_blwl_1043_configbus0;
wire [1043:1043] sram_blwl_1043_configbus1;
wire [1043:1043] sram_blwl_1043_configbus0_b;
assign sram_blwl_1043_configbus0[1043:1043] = sram_blwl_bl[1043:1043] ;
assign sram_blwl_1043_configbus1[1043:1043] = sram_blwl_wl[1043:1043] ;
assign sram_blwl_1043_configbus0_b[1043:1043] = sram_blwl_blb[1043:1043] ;
sram6T_blwl sram_blwl_1043_ (sram_blwl_out[1043], sram_blwl_out[1043], sram_blwl_outb[1043], sram_blwl_1043_configbus0[1043:1043], sram_blwl_1043_configbus1[1043:1043] , sram_blwl_1043_configbus0_b[1043:1043] );
wire [1044:1044] sram_blwl_1044_configbus0;
wire [1044:1044] sram_blwl_1044_configbus1;
wire [1044:1044] sram_blwl_1044_configbus0_b;
assign sram_blwl_1044_configbus0[1044:1044] = sram_blwl_bl[1044:1044] ;
assign sram_blwl_1044_configbus1[1044:1044] = sram_blwl_wl[1044:1044] ;
assign sram_blwl_1044_configbus0_b[1044:1044] = sram_blwl_blb[1044:1044] ;
sram6T_blwl sram_blwl_1044_ (sram_blwl_out[1044], sram_blwl_out[1044], sram_blwl_outb[1044], sram_blwl_1044_configbus0[1044:1044], sram_blwl_1044_configbus1[1044:1044] , sram_blwl_1044_configbus0_b[1044:1044] );
wire [1045:1045] sram_blwl_1045_configbus0;
wire [1045:1045] sram_blwl_1045_configbus1;
wire [1045:1045] sram_blwl_1045_configbus0_b;
assign sram_blwl_1045_configbus0[1045:1045] = sram_blwl_bl[1045:1045] ;
assign sram_blwl_1045_configbus1[1045:1045] = sram_blwl_wl[1045:1045] ;
assign sram_blwl_1045_configbus0_b[1045:1045] = sram_blwl_blb[1045:1045] ;
sram6T_blwl sram_blwl_1045_ (sram_blwl_out[1045], sram_blwl_out[1045], sram_blwl_outb[1045], sram_blwl_1045_configbus0[1045:1045], sram_blwl_1045_configbus1[1045:1045] , sram_blwl_1045_configbus0_b[1045:1045] );
wire [1046:1046] sram_blwl_1046_configbus0;
wire [1046:1046] sram_blwl_1046_configbus1;
wire [1046:1046] sram_blwl_1046_configbus0_b;
assign sram_blwl_1046_configbus0[1046:1046] = sram_blwl_bl[1046:1046] ;
assign sram_blwl_1046_configbus1[1046:1046] = sram_blwl_wl[1046:1046] ;
assign sram_blwl_1046_configbus0_b[1046:1046] = sram_blwl_blb[1046:1046] ;
sram6T_blwl sram_blwl_1046_ (sram_blwl_out[1046], sram_blwl_out[1046], sram_blwl_outb[1046], sram_blwl_1046_configbus0[1046:1046], sram_blwl_1046_configbus1[1046:1046] , sram_blwl_1046_configbus0_b[1046:1046] );
wire [1047:1047] sram_blwl_1047_configbus0;
wire [1047:1047] sram_blwl_1047_configbus1;
wire [1047:1047] sram_blwl_1047_configbus0_b;
assign sram_blwl_1047_configbus0[1047:1047] = sram_blwl_bl[1047:1047] ;
assign sram_blwl_1047_configbus1[1047:1047] = sram_blwl_wl[1047:1047] ;
assign sram_blwl_1047_configbus0_b[1047:1047] = sram_blwl_blb[1047:1047] ;
sram6T_blwl sram_blwl_1047_ (sram_blwl_out[1047], sram_blwl_out[1047], sram_blwl_outb[1047], sram_blwl_1047_configbus0[1047:1047], sram_blwl_1047_configbus1[1047:1047] , sram_blwl_1047_configbus0_b[1047:1047] );
wire [1048:1048] sram_blwl_1048_configbus0;
wire [1048:1048] sram_blwl_1048_configbus1;
wire [1048:1048] sram_blwl_1048_configbus0_b;
assign sram_blwl_1048_configbus0[1048:1048] = sram_blwl_bl[1048:1048] ;
assign sram_blwl_1048_configbus1[1048:1048] = sram_blwl_wl[1048:1048] ;
assign sram_blwl_1048_configbus0_b[1048:1048] = sram_blwl_blb[1048:1048] ;
sram6T_blwl sram_blwl_1048_ (sram_blwl_out[1048], sram_blwl_out[1048], sram_blwl_outb[1048], sram_blwl_1048_configbus0[1048:1048], sram_blwl_1048_configbus1[1048:1048] , sram_blwl_1048_configbus0_b[1048:1048] );
wire [1049:1049] sram_blwl_1049_configbus0;
wire [1049:1049] sram_blwl_1049_configbus1;
wire [1049:1049] sram_blwl_1049_configbus0_b;
assign sram_blwl_1049_configbus0[1049:1049] = sram_blwl_bl[1049:1049] ;
assign sram_blwl_1049_configbus1[1049:1049] = sram_blwl_wl[1049:1049] ;
assign sram_blwl_1049_configbus0_b[1049:1049] = sram_blwl_blb[1049:1049] ;
sram6T_blwl sram_blwl_1049_ (sram_blwl_out[1049], sram_blwl_out[1049], sram_blwl_outb[1049], sram_blwl_1049_configbus0[1049:1049], sram_blwl_1049_configbus1[1049:1049] , sram_blwl_1049_configbus0_b[1049:1049] );
wire [1050:1050] sram_blwl_1050_configbus0;
wire [1050:1050] sram_blwl_1050_configbus1;
wire [1050:1050] sram_blwl_1050_configbus0_b;
assign sram_blwl_1050_configbus0[1050:1050] = sram_blwl_bl[1050:1050] ;
assign sram_blwl_1050_configbus1[1050:1050] = sram_blwl_wl[1050:1050] ;
assign sram_blwl_1050_configbus0_b[1050:1050] = sram_blwl_blb[1050:1050] ;
sram6T_blwl sram_blwl_1050_ (sram_blwl_out[1050], sram_blwl_out[1050], sram_blwl_outb[1050], sram_blwl_1050_configbus0[1050:1050], sram_blwl_1050_configbus1[1050:1050] , sram_blwl_1050_configbus0_b[1050:1050] );
wire [1051:1051] sram_blwl_1051_configbus0;
wire [1051:1051] sram_blwl_1051_configbus1;
wire [1051:1051] sram_blwl_1051_configbus0_b;
assign sram_blwl_1051_configbus0[1051:1051] = sram_blwl_bl[1051:1051] ;
assign sram_blwl_1051_configbus1[1051:1051] = sram_blwl_wl[1051:1051] ;
assign sram_blwl_1051_configbus0_b[1051:1051] = sram_blwl_blb[1051:1051] ;
sram6T_blwl sram_blwl_1051_ (sram_blwl_out[1051], sram_blwl_out[1051], sram_blwl_outb[1051], sram_blwl_1051_configbus0[1051:1051], sram_blwl_1051_configbus1[1051:1051] , sram_blwl_1051_configbus0_b[1051:1051] );
wire [1052:1052] sram_blwl_1052_configbus0;
wire [1052:1052] sram_blwl_1052_configbus1;
wire [1052:1052] sram_blwl_1052_configbus0_b;
assign sram_blwl_1052_configbus0[1052:1052] = sram_blwl_bl[1052:1052] ;
assign sram_blwl_1052_configbus1[1052:1052] = sram_blwl_wl[1052:1052] ;
assign sram_blwl_1052_configbus0_b[1052:1052] = sram_blwl_blb[1052:1052] ;
sram6T_blwl sram_blwl_1052_ (sram_blwl_out[1052], sram_blwl_out[1052], sram_blwl_outb[1052], sram_blwl_1052_configbus0[1052:1052], sram_blwl_1052_configbus1[1052:1052] , sram_blwl_1052_configbus0_b[1052:1052] );
wire [1053:1053] sram_blwl_1053_configbus0;
wire [1053:1053] sram_blwl_1053_configbus1;
wire [1053:1053] sram_blwl_1053_configbus0_b;
assign sram_blwl_1053_configbus0[1053:1053] = sram_blwl_bl[1053:1053] ;
assign sram_blwl_1053_configbus1[1053:1053] = sram_blwl_wl[1053:1053] ;
assign sram_blwl_1053_configbus0_b[1053:1053] = sram_blwl_blb[1053:1053] ;
sram6T_blwl sram_blwl_1053_ (sram_blwl_out[1053], sram_blwl_out[1053], sram_blwl_outb[1053], sram_blwl_1053_configbus0[1053:1053], sram_blwl_1053_configbus1[1053:1053] , sram_blwl_1053_configbus0_b[1053:1053] );
wire [1054:1054] sram_blwl_1054_configbus0;
wire [1054:1054] sram_blwl_1054_configbus1;
wire [1054:1054] sram_blwl_1054_configbus0_b;
assign sram_blwl_1054_configbus0[1054:1054] = sram_blwl_bl[1054:1054] ;
assign sram_blwl_1054_configbus1[1054:1054] = sram_blwl_wl[1054:1054] ;
assign sram_blwl_1054_configbus0_b[1054:1054] = sram_blwl_blb[1054:1054] ;
sram6T_blwl sram_blwl_1054_ (sram_blwl_out[1054], sram_blwl_out[1054], sram_blwl_outb[1054], sram_blwl_1054_configbus0[1054:1054], sram_blwl_1054_configbus1[1054:1054] , sram_blwl_1054_configbus0_b[1054:1054] );
wire [1055:1055] sram_blwl_1055_configbus0;
wire [1055:1055] sram_blwl_1055_configbus1;
wire [1055:1055] sram_blwl_1055_configbus0_b;
assign sram_blwl_1055_configbus0[1055:1055] = sram_blwl_bl[1055:1055] ;
assign sram_blwl_1055_configbus1[1055:1055] = sram_blwl_wl[1055:1055] ;
assign sram_blwl_1055_configbus0_b[1055:1055] = sram_blwl_blb[1055:1055] ;
sram6T_blwl sram_blwl_1055_ (sram_blwl_out[1055], sram_blwl_out[1055], sram_blwl_outb[1055], sram_blwl_1055_configbus0[1055:1055], sram_blwl_1055_configbus1[1055:1055] , sram_blwl_1055_configbus0_b[1055:1055] );
wire [1056:1056] sram_blwl_1056_configbus0;
wire [1056:1056] sram_blwl_1056_configbus1;
wire [1056:1056] sram_blwl_1056_configbus0_b;
assign sram_blwl_1056_configbus0[1056:1056] = sram_blwl_bl[1056:1056] ;
assign sram_blwl_1056_configbus1[1056:1056] = sram_blwl_wl[1056:1056] ;
assign sram_blwl_1056_configbus0_b[1056:1056] = sram_blwl_blb[1056:1056] ;
sram6T_blwl sram_blwl_1056_ (sram_blwl_out[1056], sram_blwl_out[1056], sram_blwl_outb[1056], sram_blwl_1056_configbus0[1056:1056], sram_blwl_1056_configbus1[1056:1056] , sram_blwl_1056_configbus0_b[1056:1056] );
wire [1057:1057] sram_blwl_1057_configbus0;
wire [1057:1057] sram_blwl_1057_configbus1;
wire [1057:1057] sram_blwl_1057_configbus0_b;
assign sram_blwl_1057_configbus0[1057:1057] = sram_blwl_bl[1057:1057] ;
assign sram_blwl_1057_configbus1[1057:1057] = sram_blwl_wl[1057:1057] ;
assign sram_blwl_1057_configbus0_b[1057:1057] = sram_blwl_blb[1057:1057] ;
sram6T_blwl sram_blwl_1057_ (sram_blwl_out[1057], sram_blwl_out[1057], sram_blwl_outb[1057], sram_blwl_1057_configbus0[1057:1057], sram_blwl_1057_configbus1[1057:1057] , sram_blwl_1057_configbus0_b[1057:1057] );
wire [1058:1058] sram_blwl_1058_configbus0;
wire [1058:1058] sram_blwl_1058_configbus1;
wire [1058:1058] sram_blwl_1058_configbus0_b;
assign sram_blwl_1058_configbus0[1058:1058] = sram_blwl_bl[1058:1058] ;
assign sram_blwl_1058_configbus1[1058:1058] = sram_blwl_wl[1058:1058] ;
assign sram_blwl_1058_configbus0_b[1058:1058] = sram_blwl_blb[1058:1058] ;
sram6T_blwl sram_blwl_1058_ (sram_blwl_out[1058], sram_blwl_out[1058], sram_blwl_outb[1058], sram_blwl_1058_configbus0[1058:1058], sram_blwl_1058_configbus1[1058:1058] , sram_blwl_1058_configbus0_b[1058:1058] );
wire [1059:1059] sram_blwl_1059_configbus0;
wire [1059:1059] sram_blwl_1059_configbus1;
wire [1059:1059] sram_blwl_1059_configbus0_b;
assign sram_blwl_1059_configbus0[1059:1059] = sram_blwl_bl[1059:1059] ;
assign sram_blwl_1059_configbus1[1059:1059] = sram_blwl_wl[1059:1059] ;
assign sram_blwl_1059_configbus0_b[1059:1059] = sram_blwl_blb[1059:1059] ;
sram6T_blwl sram_blwl_1059_ (sram_blwl_out[1059], sram_blwl_out[1059], sram_blwl_outb[1059], sram_blwl_1059_configbus0[1059:1059], sram_blwl_1059_configbus1[1059:1059] , sram_blwl_1059_configbus0_b[1059:1059] );
wire [1060:1060] sram_blwl_1060_configbus0;
wire [1060:1060] sram_blwl_1060_configbus1;
wire [1060:1060] sram_blwl_1060_configbus0_b;
assign sram_blwl_1060_configbus0[1060:1060] = sram_blwl_bl[1060:1060] ;
assign sram_blwl_1060_configbus1[1060:1060] = sram_blwl_wl[1060:1060] ;
assign sram_blwl_1060_configbus0_b[1060:1060] = sram_blwl_blb[1060:1060] ;
sram6T_blwl sram_blwl_1060_ (sram_blwl_out[1060], sram_blwl_out[1060], sram_blwl_outb[1060], sram_blwl_1060_configbus0[1060:1060], sram_blwl_1060_configbus1[1060:1060] , sram_blwl_1060_configbus0_b[1060:1060] );
wire [1061:1061] sram_blwl_1061_configbus0;
wire [1061:1061] sram_blwl_1061_configbus1;
wire [1061:1061] sram_blwl_1061_configbus0_b;
assign sram_blwl_1061_configbus0[1061:1061] = sram_blwl_bl[1061:1061] ;
assign sram_blwl_1061_configbus1[1061:1061] = sram_blwl_wl[1061:1061] ;
assign sram_blwl_1061_configbus0_b[1061:1061] = sram_blwl_blb[1061:1061] ;
sram6T_blwl sram_blwl_1061_ (sram_blwl_out[1061], sram_blwl_out[1061], sram_blwl_outb[1061], sram_blwl_1061_configbus0[1061:1061], sram_blwl_1061_configbus1[1061:1061] , sram_blwl_1061_configbus0_b[1061:1061] );
wire [1062:1062] sram_blwl_1062_configbus0;
wire [1062:1062] sram_blwl_1062_configbus1;
wire [1062:1062] sram_blwl_1062_configbus0_b;
assign sram_blwl_1062_configbus0[1062:1062] = sram_blwl_bl[1062:1062] ;
assign sram_blwl_1062_configbus1[1062:1062] = sram_blwl_wl[1062:1062] ;
assign sram_blwl_1062_configbus0_b[1062:1062] = sram_blwl_blb[1062:1062] ;
sram6T_blwl sram_blwl_1062_ (sram_blwl_out[1062], sram_blwl_out[1062], sram_blwl_outb[1062], sram_blwl_1062_configbus0[1062:1062], sram_blwl_1062_configbus1[1062:1062] , sram_blwl_1062_configbus0_b[1062:1062] );
wire [1063:1063] sram_blwl_1063_configbus0;
wire [1063:1063] sram_blwl_1063_configbus1;
wire [1063:1063] sram_blwl_1063_configbus0_b;
assign sram_blwl_1063_configbus0[1063:1063] = sram_blwl_bl[1063:1063] ;
assign sram_blwl_1063_configbus1[1063:1063] = sram_blwl_wl[1063:1063] ;
assign sram_blwl_1063_configbus0_b[1063:1063] = sram_blwl_blb[1063:1063] ;
sram6T_blwl sram_blwl_1063_ (sram_blwl_out[1063], sram_blwl_out[1063], sram_blwl_outb[1063], sram_blwl_1063_configbus0[1063:1063], sram_blwl_1063_configbus1[1063:1063] , sram_blwl_1063_configbus0_b[1063:1063] );
wire [1064:1064] sram_blwl_1064_configbus0;
wire [1064:1064] sram_blwl_1064_configbus1;
wire [1064:1064] sram_blwl_1064_configbus0_b;
assign sram_blwl_1064_configbus0[1064:1064] = sram_blwl_bl[1064:1064] ;
assign sram_blwl_1064_configbus1[1064:1064] = sram_blwl_wl[1064:1064] ;
assign sram_blwl_1064_configbus0_b[1064:1064] = sram_blwl_blb[1064:1064] ;
sram6T_blwl sram_blwl_1064_ (sram_blwl_out[1064], sram_blwl_out[1064], sram_blwl_outb[1064], sram_blwl_1064_configbus0[1064:1064], sram_blwl_1064_configbus1[1064:1064] , sram_blwl_1064_configbus0_b[1064:1064] );
wire [1065:1065] sram_blwl_1065_configbus0;
wire [1065:1065] sram_blwl_1065_configbus1;
wire [1065:1065] sram_blwl_1065_configbus0_b;
assign sram_blwl_1065_configbus0[1065:1065] = sram_blwl_bl[1065:1065] ;
assign sram_blwl_1065_configbus1[1065:1065] = sram_blwl_wl[1065:1065] ;
assign sram_blwl_1065_configbus0_b[1065:1065] = sram_blwl_blb[1065:1065] ;
sram6T_blwl sram_blwl_1065_ (sram_blwl_out[1065], sram_blwl_out[1065], sram_blwl_outb[1065], sram_blwl_1065_configbus0[1065:1065], sram_blwl_1065_configbus1[1065:1065] , sram_blwl_1065_configbus0_b[1065:1065] );
wire [1066:1066] sram_blwl_1066_configbus0;
wire [1066:1066] sram_blwl_1066_configbus1;
wire [1066:1066] sram_blwl_1066_configbus0_b;
assign sram_blwl_1066_configbus0[1066:1066] = sram_blwl_bl[1066:1066] ;
assign sram_blwl_1066_configbus1[1066:1066] = sram_blwl_wl[1066:1066] ;
assign sram_blwl_1066_configbus0_b[1066:1066] = sram_blwl_blb[1066:1066] ;
sram6T_blwl sram_blwl_1066_ (sram_blwl_out[1066], sram_blwl_out[1066], sram_blwl_outb[1066], sram_blwl_1066_configbus0[1066:1066], sram_blwl_1066_configbus1[1066:1066] , sram_blwl_1066_configbus0_b[1066:1066] );
wire [1067:1067] sram_blwl_1067_configbus0;
wire [1067:1067] sram_blwl_1067_configbus1;
wire [1067:1067] sram_blwl_1067_configbus0_b;
assign sram_blwl_1067_configbus0[1067:1067] = sram_blwl_bl[1067:1067] ;
assign sram_blwl_1067_configbus1[1067:1067] = sram_blwl_wl[1067:1067] ;
assign sram_blwl_1067_configbus0_b[1067:1067] = sram_blwl_blb[1067:1067] ;
sram6T_blwl sram_blwl_1067_ (sram_blwl_out[1067], sram_blwl_out[1067], sram_blwl_outb[1067], sram_blwl_1067_configbus0[1067:1067], sram_blwl_1067_configbus1[1067:1067] , sram_blwl_1067_configbus0_b[1067:1067] );
wire [1068:1068] sram_blwl_1068_configbus0;
wire [1068:1068] sram_blwl_1068_configbus1;
wire [1068:1068] sram_blwl_1068_configbus0_b;
assign sram_blwl_1068_configbus0[1068:1068] = sram_blwl_bl[1068:1068] ;
assign sram_blwl_1068_configbus1[1068:1068] = sram_blwl_wl[1068:1068] ;
assign sram_blwl_1068_configbus0_b[1068:1068] = sram_blwl_blb[1068:1068] ;
sram6T_blwl sram_blwl_1068_ (sram_blwl_out[1068], sram_blwl_out[1068], sram_blwl_outb[1068], sram_blwl_1068_configbus0[1068:1068], sram_blwl_1068_configbus1[1068:1068] , sram_blwl_1068_configbus0_b[1068:1068] );
wire [1069:1069] sram_blwl_1069_configbus0;
wire [1069:1069] sram_blwl_1069_configbus1;
wire [1069:1069] sram_blwl_1069_configbus0_b;
assign sram_blwl_1069_configbus0[1069:1069] = sram_blwl_bl[1069:1069] ;
assign sram_blwl_1069_configbus1[1069:1069] = sram_blwl_wl[1069:1069] ;
assign sram_blwl_1069_configbus0_b[1069:1069] = sram_blwl_blb[1069:1069] ;
sram6T_blwl sram_blwl_1069_ (sram_blwl_out[1069], sram_blwl_out[1069], sram_blwl_outb[1069], sram_blwl_1069_configbus0[1069:1069], sram_blwl_1069_configbus1[1069:1069] , sram_blwl_1069_configbus0_b[1069:1069] );
wire [1070:1070] sram_blwl_1070_configbus0;
wire [1070:1070] sram_blwl_1070_configbus1;
wire [1070:1070] sram_blwl_1070_configbus0_b;
assign sram_blwl_1070_configbus0[1070:1070] = sram_blwl_bl[1070:1070] ;
assign sram_blwl_1070_configbus1[1070:1070] = sram_blwl_wl[1070:1070] ;
assign sram_blwl_1070_configbus0_b[1070:1070] = sram_blwl_blb[1070:1070] ;
sram6T_blwl sram_blwl_1070_ (sram_blwl_out[1070], sram_blwl_out[1070], sram_blwl_outb[1070], sram_blwl_1070_configbus0[1070:1070], sram_blwl_1070_configbus1[1070:1070] , sram_blwl_1070_configbus0_b[1070:1070] );
wire [1071:1071] sram_blwl_1071_configbus0;
wire [1071:1071] sram_blwl_1071_configbus1;
wire [1071:1071] sram_blwl_1071_configbus0_b;
assign sram_blwl_1071_configbus0[1071:1071] = sram_blwl_bl[1071:1071] ;
assign sram_blwl_1071_configbus1[1071:1071] = sram_blwl_wl[1071:1071] ;
assign sram_blwl_1071_configbus0_b[1071:1071] = sram_blwl_blb[1071:1071] ;
sram6T_blwl sram_blwl_1071_ (sram_blwl_out[1071], sram_blwl_out[1071], sram_blwl_outb[1071], sram_blwl_1071_configbus0[1071:1071], sram_blwl_1071_configbus1[1071:1071] , sram_blwl_1071_configbus0_b[1071:1071] );
wire [1072:1072] sram_blwl_1072_configbus0;
wire [1072:1072] sram_blwl_1072_configbus1;
wire [1072:1072] sram_blwl_1072_configbus0_b;
assign sram_blwl_1072_configbus0[1072:1072] = sram_blwl_bl[1072:1072] ;
assign sram_blwl_1072_configbus1[1072:1072] = sram_blwl_wl[1072:1072] ;
assign sram_blwl_1072_configbus0_b[1072:1072] = sram_blwl_blb[1072:1072] ;
sram6T_blwl sram_blwl_1072_ (sram_blwl_out[1072], sram_blwl_out[1072], sram_blwl_outb[1072], sram_blwl_1072_configbus0[1072:1072], sram_blwl_1072_configbus1[1072:1072] , sram_blwl_1072_configbus0_b[1072:1072] );
wire [1073:1073] sram_blwl_1073_configbus0;
wire [1073:1073] sram_blwl_1073_configbus1;
wire [1073:1073] sram_blwl_1073_configbus0_b;
assign sram_blwl_1073_configbus0[1073:1073] = sram_blwl_bl[1073:1073] ;
assign sram_blwl_1073_configbus1[1073:1073] = sram_blwl_wl[1073:1073] ;
assign sram_blwl_1073_configbus0_b[1073:1073] = sram_blwl_blb[1073:1073] ;
sram6T_blwl sram_blwl_1073_ (sram_blwl_out[1073], sram_blwl_out[1073], sram_blwl_outb[1073], sram_blwl_1073_configbus0[1073:1073], sram_blwl_1073_configbus1[1073:1073] , sram_blwl_1073_configbus0_b[1073:1073] );
wire [1074:1074] sram_blwl_1074_configbus0;
wire [1074:1074] sram_blwl_1074_configbus1;
wire [1074:1074] sram_blwl_1074_configbus0_b;
assign sram_blwl_1074_configbus0[1074:1074] = sram_blwl_bl[1074:1074] ;
assign sram_blwl_1074_configbus1[1074:1074] = sram_blwl_wl[1074:1074] ;
assign sram_blwl_1074_configbus0_b[1074:1074] = sram_blwl_blb[1074:1074] ;
sram6T_blwl sram_blwl_1074_ (sram_blwl_out[1074], sram_blwl_out[1074], sram_blwl_outb[1074], sram_blwl_1074_configbus0[1074:1074], sram_blwl_1074_configbus1[1074:1074] , sram_blwl_1074_configbus0_b[1074:1074] );
wire [1075:1075] sram_blwl_1075_configbus0;
wire [1075:1075] sram_blwl_1075_configbus1;
wire [1075:1075] sram_blwl_1075_configbus0_b;
assign sram_blwl_1075_configbus0[1075:1075] = sram_blwl_bl[1075:1075] ;
assign sram_blwl_1075_configbus1[1075:1075] = sram_blwl_wl[1075:1075] ;
assign sram_blwl_1075_configbus0_b[1075:1075] = sram_blwl_blb[1075:1075] ;
sram6T_blwl sram_blwl_1075_ (sram_blwl_out[1075], sram_blwl_out[1075], sram_blwl_outb[1075], sram_blwl_1075_configbus0[1075:1075], sram_blwl_1075_configbus1[1075:1075] , sram_blwl_1075_configbus0_b[1075:1075] );
wire [1076:1076] sram_blwl_1076_configbus0;
wire [1076:1076] sram_blwl_1076_configbus1;
wire [1076:1076] sram_blwl_1076_configbus0_b;
assign sram_blwl_1076_configbus0[1076:1076] = sram_blwl_bl[1076:1076] ;
assign sram_blwl_1076_configbus1[1076:1076] = sram_blwl_wl[1076:1076] ;
assign sram_blwl_1076_configbus0_b[1076:1076] = sram_blwl_blb[1076:1076] ;
sram6T_blwl sram_blwl_1076_ (sram_blwl_out[1076], sram_blwl_out[1076], sram_blwl_outb[1076], sram_blwl_1076_configbus0[1076:1076], sram_blwl_1076_configbus1[1076:1076] , sram_blwl_1076_configbus0_b[1076:1076] );
wire [1077:1077] sram_blwl_1077_configbus0;
wire [1077:1077] sram_blwl_1077_configbus1;
wire [1077:1077] sram_blwl_1077_configbus0_b;
assign sram_blwl_1077_configbus0[1077:1077] = sram_blwl_bl[1077:1077] ;
assign sram_blwl_1077_configbus1[1077:1077] = sram_blwl_wl[1077:1077] ;
assign sram_blwl_1077_configbus0_b[1077:1077] = sram_blwl_blb[1077:1077] ;
sram6T_blwl sram_blwl_1077_ (sram_blwl_out[1077], sram_blwl_out[1077], sram_blwl_outb[1077], sram_blwl_1077_configbus0[1077:1077], sram_blwl_1077_configbus1[1077:1077] , sram_blwl_1077_configbus0_b[1077:1077] );
wire [1078:1078] sram_blwl_1078_configbus0;
wire [1078:1078] sram_blwl_1078_configbus1;
wire [1078:1078] sram_blwl_1078_configbus0_b;
assign sram_blwl_1078_configbus0[1078:1078] = sram_blwl_bl[1078:1078] ;
assign sram_blwl_1078_configbus1[1078:1078] = sram_blwl_wl[1078:1078] ;
assign sram_blwl_1078_configbus0_b[1078:1078] = sram_blwl_blb[1078:1078] ;
sram6T_blwl sram_blwl_1078_ (sram_blwl_out[1078], sram_blwl_out[1078], sram_blwl_outb[1078], sram_blwl_1078_configbus0[1078:1078], sram_blwl_1078_configbus1[1078:1078] , sram_blwl_1078_configbus0_b[1078:1078] );
wire [1079:1079] sram_blwl_1079_configbus0;
wire [1079:1079] sram_blwl_1079_configbus1;
wire [1079:1079] sram_blwl_1079_configbus0_b;
assign sram_blwl_1079_configbus0[1079:1079] = sram_blwl_bl[1079:1079] ;
assign sram_blwl_1079_configbus1[1079:1079] = sram_blwl_wl[1079:1079] ;
assign sram_blwl_1079_configbus0_b[1079:1079] = sram_blwl_blb[1079:1079] ;
sram6T_blwl sram_blwl_1079_ (sram_blwl_out[1079], sram_blwl_out[1079], sram_blwl_outb[1079], sram_blwl_1079_configbus0[1079:1079], sram_blwl_1079_configbus1[1079:1079] , sram_blwl_1079_configbus0_b[1079:1079] );
endmodule
//----- END LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----

//----- Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----
module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
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
//----- END Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6_ -----
module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_ble6___in_0_,
input wire mode_ble6___in_1_,
input wire mode_ble6___in_2_,
input wire mode_ble6___in_3_,
input wire mode_ble6___in_4_,
input wire mode_ble6___in_5_,
output wire mode_ble6___out_0_,
input wire mode_ble6___clk_0_,
input [1016:1080] sram_blwl_bl ,
input [1016:1080] sram_blwl_wl ,
input [1016:1080] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ lut6_0_ (
 lut6_0___in_0_,  lut6_0___in_1_,  lut6_0___in_2_,  lut6_0___in_3_,  lut6_0___in_4_,  lut6_0___in_5_,  lut6_0___out_0_,
sram_blwl_bl[1016:1079] ,
sram_blwl_wl[1016:1079] ,
sram_blwl_blb[1016:1079] );
grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ ff_0_ (
//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
wire [0:1] in_bus_mux_1level_tapbuf_size2_400_ ;
assign in_bus_mux_1level_tapbuf_size2_400_[0] = ff_0___Q_0_ ; 
assign in_bus_mux_1level_tapbuf_size2_400_[1] = lut6_0___out_0_ ; 
wire [1080:1080] mux_1level_tapbuf_size2_400_configbus0;
wire [1080:1080] mux_1level_tapbuf_size2_400_configbus1;
wire [1080:1080] mux_1level_tapbuf_size2_400_sram_blwl_out ;
wire [1080:1080] mux_1level_tapbuf_size2_400_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_400_configbus0[1080:1080] = sram_blwl_bl[1080:1080] ;
assign mux_1level_tapbuf_size2_400_configbus1[1080:1080] = sram_blwl_wl[1080:1080] ;
wire [1080:1080] mux_1level_tapbuf_size2_400_configbus0_b;
assign mux_1level_tapbuf_size2_400_configbus0_b[1080:1080] = sram_blwl_blb[1080:1080] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_400_ (in_bus_mux_1level_tapbuf_size2_400_, mode_ble6___out_0_, mux_1level_tapbuf_size2_400_sram_blwl_out[1080:1080] ,
mux_1level_tapbuf_size2_400_sram_blwl_outb[1080:1080] );
//----- SRAM bits for MUX[400], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_1080_ (mux_1level_tapbuf_size2_400_sram_blwl_out[1080:1080] ,mux_1level_tapbuf_size2_400_sram_blwl_out[1080:1080] ,mux_1level_tapbuf_size2_400_sram_blwl_outb[1080:1080] ,mux_1level_tapbuf_size2_400_configbus0[1080:1080], mux_1level_tapbuf_size2_400_configbus1[1080:1080] , mux_1level_tapbuf_size2_400_configbus0_b[1080:1080] );
direct_interc direct_interc_0_ (mode_ble6___in_0_, lut6_0___in_0_ );
direct_interc direct_interc_1_ (mode_ble6___in_1_, lut6_0___in_1_ );
direct_interc direct_interc_2_ (mode_ble6___in_2_, lut6_0___in_2_ );
direct_interc direct_interc_3_ (mode_ble6___in_3_, lut6_0___in_3_ );
direct_interc direct_interc_4_ (mode_ble6___in_4_, lut6_0___in_4_ );
direct_interc direct_interc_5_ (mode_ble6___in_5_, lut6_0___in_5_ );
direct_interc direct_interc_6_ (lut6_0___out_0_, ff_0___D_0_ );
direct_interc direct_interc_7_ (mode_ble6___clk_0_, ff_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6_ -----
module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_n1_lut6___in_0_,
input wire mode_n1_lut6___in_1_,
input wire mode_n1_lut6___in_2_,
input wire mode_n1_lut6___in_3_,
input wire mode_n1_lut6___in_4_,
input wire mode_n1_lut6___in_5_,
output wire mode_n1_lut6___out_0_,
input wire mode_n1_lut6___clk_0_,
input [1016:1080] sram_blwl_bl ,
input [1016:1080] sram_blwl_wl ,
input [1016:1080] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6__ble6_0__mode_ble6_ ble6_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 ble6_0___in_0_,  ble6_0___in_1_,  ble6_0___in_2_,  ble6_0___in_3_,  ble6_0___in_4_,  ble6_0___in_5_,  ble6_0___out_0_,  ble6_0___clk_0_,
sram_blwl_bl[1016:1080] ,
sram_blwl_wl[1016:1080] ,
sram_blwl_blb[1016:1080] );
direct_interc direct_interc_8_ (ble6_0___out_0_, mode_n1_lut6___out_0_ );
direct_interc direct_interc_9_ (mode_n1_lut6___in_0_, ble6_0___in_0_ );
direct_interc direct_interc_10_ (mode_n1_lut6___in_1_, ble6_0___in_1_ );
direct_interc direct_interc_11_ (mode_n1_lut6___in_2_, ble6_0___in_2_ );
direct_interc direct_interc_12_ (mode_n1_lut6___in_3_, ble6_0___in_3_ );
direct_interc direct_interc_13_ (mode_n1_lut6___in_4_, ble6_0___in_4_ );
direct_interc direct_interc_14_ (mode_n1_lut6___in_5_, ble6_0___in_5_ );
direct_interc direct_interc_15_ (mode_n1_lut6___clk_0_, ble6_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6_ -----

//----- LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----
module grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ (
input wire lut6_0___in_0_,
input wire lut6_0___in_1_,
input wire lut6_0___in_2_,
input wire lut6_0___in_3_,
input wire lut6_0___in_4_,
input wire lut6_0___in_5_,
output wire lut6_0___out_0_,
input [1081:1144] sram_blwl_bl ,
input [1081:1144] sram_blwl_wl ,
input [1081:1144] sram_blwl_blb );
wire [0:5] lut6_0___in;
assign lut6_0___in[0] = lut6_0___in_0_;
assign lut6_0___in[1] = lut6_0___in_1_;
assign lut6_0___in[2] = lut6_0___in_2_;
assign lut6_0___in[3] = lut6_0___in_3_;
assign lut6_0___in[4] = lut6_0___in_4_;
assign lut6_0___in[5] = lut6_0___in_5_;
wire [0:0] lut6_0___out;
assign lut6_0___out_0_ = lut6_0___out[0];
wire [1081:1144] sram_blwl_out ;
wire [1081:1144] sram_blwl_outb ;
lut6 lut6_1_ (
//----- Input and output ports -----
 lut6_0___in[0:5] ,  lut6_0___out[0:0],//----- SRAM ports -----
sram_blwl_out[1081:1144] , sram_blwl_outb[1081:1144] );
//----- Truth Table for LUT[1], size=6. -----
//----- SRAM bits for LUT[1], size=6, num_sram=64. -----
//-----0000000000000000000000000000000000000000000000000000000000000000-----
wire [1081:1081] sram_blwl_1081_configbus0;
wire [1081:1081] sram_blwl_1081_configbus1;
wire [1081:1081] sram_blwl_1081_configbus0_b;
assign sram_blwl_1081_configbus0[1081:1081] = sram_blwl_bl[1081:1081] ;
assign sram_blwl_1081_configbus1[1081:1081] = sram_blwl_wl[1081:1081] ;
assign sram_blwl_1081_configbus0_b[1081:1081] = sram_blwl_blb[1081:1081] ;
sram6T_blwl sram_blwl_1081_ (sram_blwl_out[1081], sram_blwl_out[1081], sram_blwl_outb[1081], sram_blwl_1081_configbus0[1081:1081], sram_blwl_1081_configbus1[1081:1081] , sram_blwl_1081_configbus0_b[1081:1081] );
wire [1082:1082] sram_blwl_1082_configbus0;
wire [1082:1082] sram_blwl_1082_configbus1;
wire [1082:1082] sram_blwl_1082_configbus0_b;
assign sram_blwl_1082_configbus0[1082:1082] = sram_blwl_bl[1082:1082] ;
assign sram_blwl_1082_configbus1[1082:1082] = sram_blwl_wl[1082:1082] ;
assign sram_blwl_1082_configbus0_b[1082:1082] = sram_blwl_blb[1082:1082] ;
sram6T_blwl sram_blwl_1082_ (sram_blwl_out[1082], sram_blwl_out[1082], sram_blwl_outb[1082], sram_blwl_1082_configbus0[1082:1082], sram_blwl_1082_configbus1[1082:1082] , sram_blwl_1082_configbus0_b[1082:1082] );
wire [1083:1083] sram_blwl_1083_configbus0;
wire [1083:1083] sram_blwl_1083_configbus1;
wire [1083:1083] sram_blwl_1083_configbus0_b;
assign sram_blwl_1083_configbus0[1083:1083] = sram_blwl_bl[1083:1083] ;
assign sram_blwl_1083_configbus1[1083:1083] = sram_blwl_wl[1083:1083] ;
assign sram_blwl_1083_configbus0_b[1083:1083] = sram_blwl_blb[1083:1083] ;
sram6T_blwl sram_blwl_1083_ (sram_blwl_out[1083], sram_blwl_out[1083], sram_blwl_outb[1083], sram_blwl_1083_configbus0[1083:1083], sram_blwl_1083_configbus1[1083:1083] , sram_blwl_1083_configbus0_b[1083:1083] );
wire [1084:1084] sram_blwl_1084_configbus0;
wire [1084:1084] sram_blwl_1084_configbus1;
wire [1084:1084] sram_blwl_1084_configbus0_b;
assign sram_blwl_1084_configbus0[1084:1084] = sram_blwl_bl[1084:1084] ;
assign sram_blwl_1084_configbus1[1084:1084] = sram_blwl_wl[1084:1084] ;
assign sram_blwl_1084_configbus0_b[1084:1084] = sram_blwl_blb[1084:1084] ;
sram6T_blwl sram_blwl_1084_ (sram_blwl_out[1084], sram_blwl_out[1084], sram_blwl_outb[1084], sram_blwl_1084_configbus0[1084:1084], sram_blwl_1084_configbus1[1084:1084] , sram_blwl_1084_configbus0_b[1084:1084] );
wire [1085:1085] sram_blwl_1085_configbus0;
wire [1085:1085] sram_blwl_1085_configbus1;
wire [1085:1085] sram_blwl_1085_configbus0_b;
assign sram_blwl_1085_configbus0[1085:1085] = sram_blwl_bl[1085:1085] ;
assign sram_blwl_1085_configbus1[1085:1085] = sram_blwl_wl[1085:1085] ;
assign sram_blwl_1085_configbus0_b[1085:1085] = sram_blwl_blb[1085:1085] ;
sram6T_blwl sram_blwl_1085_ (sram_blwl_out[1085], sram_blwl_out[1085], sram_blwl_outb[1085], sram_blwl_1085_configbus0[1085:1085], sram_blwl_1085_configbus1[1085:1085] , sram_blwl_1085_configbus0_b[1085:1085] );
wire [1086:1086] sram_blwl_1086_configbus0;
wire [1086:1086] sram_blwl_1086_configbus1;
wire [1086:1086] sram_blwl_1086_configbus0_b;
assign sram_blwl_1086_configbus0[1086:1086] = sram_blwl_bl[1086:1086] ;
assign sram_blwl_1086_configbus1[1086:1086] = sram_blwl_wl[1086:1086] ;
assign sram_blwl_1086_configbus0_b[1086:1086] = sram_blwl_blb[1086:1086] ;
sram6T_blwl sram_blwl_1086_ (sram_blwl_out[1086], sram_blwl_out[1086], sram_blwl_outb[1086], sram_blwl_1086_configbus0[1086:1086], sram_blwl_1086_configbus1[1086:1086] , sram_blwl_1086_configbus0_b[1086:1086] );
wire [1087:1087] sram_blwl_1087_configbus0;
wire [1087:1087] sram_blwl_1087_configbus1;
wire [1087:1087] sram_blwl_1087_configbus0_b;
assign sram_blwl_1087_configbus0[1087:1087] = sram_blwl_bl[1087:1087] ;
assign sram_blwl_1087_configbus1[1087:1087] = sram_blwl_wl[1087:1087] ;
assign sram_blwl_1087_configbus0_b[1087:1087] = sram_blwl_blb[1087:1087] ;
sram6T_blwl sram_blwl_1087_ (sram_blwl_out[1087], sram_blwl_out[1087], sram_blwl_outb[1087], sram_blwl_1087_configbus0[1087:1087], sram_blwl_1087_configbus1[1087:1087] , sram_blwl_1087_configbus0_b[1087:1087] );
wire [1088:1088] sram_blwl_1088_configbus0;
wire [1088:1088] sram_blwl_1088_configbus1;
wire [1088:1088] sram_blwl_1088_configbus0_b;
assign sram_blwl_1088_configbus0[1088:1088] = sram_blwl_bl[1088:1088] ;
assign sram_blwl_1088_configbus1[1088:1088] = sram_blwl_wl[1088:1088] ;
assign sram_blwl_1088_configbus0_b[1088:1088] = sram_blwl_blb[1088:1088] ;
sram6T_blwl sram_blwl_1088_ (sram_blwl_out[1088], sram_blwl_out[1088], sram_blwl_outb[1088], sram_blwl_1088_configbus0[1088:1088], sram_blwl_1088_configbus1[1088:1088] , sram_blwl_1088_configbus0_b[1088:1088] );
wire [1089:1089] sram_blwl_1089_configbus0;
wire [1089:1089] sram_blwl_1089_configbus1;
wire [1089:1089] sram_blwl_1089_configbus0_b;
assign sram_blwl_1089_configbus0[1089:1089] = sram_blwl_bl[1089:1089] ;
assign sram_blwl_1089_configbus1[1089:1089] = sram_blwl_wl[1089:1089] ;
assign sram_blwl_1089_configbus0_b[1089:1089] = sram_blwl_blb[1089:1089] ;
sram6T_blwl sram_blwl_1089_ (sram_blwl_out[1089], sram_blwl_out[1089], sram_blwl_outb[1089], sram_blwl_1089_configbus0[1089:1089], sram_blwl_1089_configbus1[1089:1089] , sram_blwl_1089_configbus0_b[1089:1089] );
wire [1090:1090] sram_blwl_1090_configbus0;
wire [1090:1090] sram_blwl_1090_configbus1;
wire [1090:1090] sram_blwl_1090_configbus0_b;
assign sram_blwl_1090_configbus0[1090:1090] = sram_blwl_bl[1090:1090] ;
assign sram_blwl_1090_configbus1[1090:1090] = sram_blwl_wl[1090:1090] ;
assign sram_blwl_1090_configbus0_b[1090:1090] = sram_blwl_blb[1090:1090] ;
sram6T_blwl sram_blwl_1090_ (sram_blwl_out[1090], sram_blwl_out[1090], sram_blwl_outb[1090], sram_blwl_1090_configbus0[1090:1090], sram_blwl_1090_configbus1[1090:1090] , sram_blwl_1090_configbus0_b[1090:1090] );
wire [1091:1091] sram_blwl_1091_configbus0;
wire [1091:1091] sram_blwl_1091_configbus1;
wire [1091:1091] sram_blwl_1091_configbus0_b;
assign sram_blwl_1091_configbus0[1091:1091] = sram_blwl_bl[1091:1091] ;
assign sram_blwl_1091_configbus1[1091:1091] = sram_blwl_wl[1091:1091] ;
assign sram_blwl_1091_configbus0_b[1091:1091] = sram_blwl_blb[1091:1091] ;
sram6T_blwl sram_blwl_1091_ (sram_blwl_out[1091], sram_blwl_out[1091], sram_blwl_outb[1091], sram_blwl_1091_configbus0[1091:1091], sram_blwl_1091_configbus1[1091:1091] , sram_blwl_1091_configbus0_b[1091:1091] );
wire [1092:1092] sram_blwl_1092_configbus0;
wire [1092:1092] sram_blwl_1092_configbus1;
wire [1092:1092] sram_blwl_1092_configbus0_b;
assign sram_blwl_1092_configbus0[1092:1092] = sram_blwl_bl[1092:1092] ;
assign sram_blwl_1092_configbus1[1092:1092] = sram_blwl_wl[1092:1092] ;
assign sram_blwl_1092_configbus0_b[1092:1092] = sram_blwl_blb[1092:1092] ;
sram6T_blwl sram_blwl_1092_ (sram_blwl_out[1092], sram_blwl_out[1092], sram_blwl_outb[1092], sram_blwl_1092_configbus0[1092:1092], sram_blwl_1092_configbus1[1092:1092] , sram_blwl_1092_configbus0_b[1092:1092] );
wire [1093:1093] sram_blwl_1093_configbus0;
wire [1093:1093] sram_blwl_1093_configbus1;
wire [1093:1093] sram_blwl_1093_configbus0_b;
assign sram_blwl_1093_configbus0[1093:1093] = sram_blwl_bl[1093:1093] ;
assign sram_blwl_1093_configbus1[1093:1093] = sram_blwl_wl[1093:1093] ;
assign sram_blwl_1093_configbus0_b[1093:1093] = sram_blwl_blb[1093:1093] ;
sram6T_blwl sram_blwl_1093_ (sram_blwl_out[1093], sram_blwl_out[1093], sram_blwl_outb[1093], sram_blwl_1093_configbus0[1093:1093], sram_blwl_1093_configbus1[1093:1093] , sram_blwl_1093_configbus0_b[1093:1093] );
wire [1094:1094] sram_blwl_1094_configbus0;
wire [1094:1094] sram_blwl_1094_configbus1;
wire [1094:1094] sram_blwl_1094_configbus0_b;
assign sram_blwl_1094_configbus0[1094:1094] = sram_blwl_bl[1094:1094] ;
assign sram_blwl_1094_configbus1[1094:1094] = sram_blwl_wl[1094:1094] ;
assign sram_blwl_1094_configbus0_b[1094:1094] = sram_blwl_blb[1094:1094] ;
sram6T_blwl sram_blwl_1094_ (sram_blwl_out[1094], sram_blwl_out[1094], sram_blwl_outb[1094], sram_blwl_1094_configbus0[1094:1094], sram_blwl_1094_configbus1[1094:1094] , sram_blwl_1094_configbus0_b[1094:1094] );
wire [1095:1095] sram_blwl_1095_configbus0;
wire [1095:1095] sram_blwl_1095_configbus1;
wire [1095:1095] sram_blwl_1095_configbus0_b;
assign sram_blwl_1095_configbus0[1095:1095] = sram_blwl_bl[1095:1095] ;
assign sram_blwl_1095_configbus1[1095:1095] = sram_blwl_wl[1095:1095] ;
assign sram_blwl_1095_configbus0_b[1095:1095] = sram_blwl_blb[1095:1095] ;
sram6T_blwl sram_blwl_1095_ (sram_blwl_out[1095], sram_blwl_out[1095], sram_blwl_outb[1095], sram_blwl_1095_configbus0[1095:1095], sram_blwl_1095_configbus1[1095:1095] , sram_blwl_1095_configbus0_b[1095:1095] );
wire [1096:1096] sram_blwl_1096_configbus0;
wire [1096:1096] sram_blwl_1096_configbus1;
wire [1096:1096] sram_blwl_1096_configbus0_b;
assign sram_blwl_1096_configbus0[1096:1096] = sram_blwl_bl[1096:1096] ;
assign sram_blwl_1096_configbus1[1096:1096] = sram_blwl_wl[1096:1096] ;
assign sram_blwl_1096_configbus0_b[1096:1096] = sram_blwl_blb[1096:1096] ;
sram6T_blwl sram_blwl_1096_ (sram_blwl_out[1096], sram_blwl_out[1096], sram_blwl_outb[1096], sram_blwl_1096_configbus0[1096:1096], sram_blwl_1096_configbus1[1096:1096] , sram_blwl_1096_configbus0_b[1096:1096] );
wire [1097:1097] sram_blwl_1097_configbus0;
wire [1097:1097] sram_blwl_1097_configbus1;
wire [1097:1097] sram_blwl_1097_configbus0_b;
assign sram_blwl_1097_configbus0[1097:1097] = sram_blwl_bl[1097:1097] ;
assign sram_blwl_1097_configbus1[1097:1097] = sram_blwl_wl[1097:1097] ;
assign sram_blwl_1097_configbus0_b[1097:1097] = sram_blwl_blb[1097:1097] ;
sram6T_blwl sram_blwl_1097_ (sram_blwl_out[1097], sram_blwl_out[1097], sram_blwl_outb[1097], sram_blwl_1097_configbus0[1097:1097], sram_blwl_1097_configbus1[1097:1097] , sram_blwl_1097_configbus0_b[1097:1097] );
wire [1098:1098] sram_blwl_1098_configbus0;
wire [1098:1098] sram_blwl_1098_configbus1;
wire [1098:1098] sram_blwl_1098_configbus0_b;
assign sram_blwl_1098_configbus0[1098:1098] = sram_blwl_bl[1098:1098] ;
assign sram_blwl_1098_configbus1[1098:1098] = sram_blwl_wl[1098:1098] ;
assign sram_blwl_1098_configbus0_b[1098:1098] = sram_blwl_blb[1098:1098] ;
sram6T_blwl sram_blwl_1098_ (sram_blwl_out[1098], sram_blwl_out[1098], sram_blwl_outb[1098], sram_blwl_1098_configbus0[1098:1098], sram_blwl_1098_configbus1[1098:1098] , sram_blwl_1098_configbus0_b[1098:1098] );
wire [1099:1099] sram_blwl_1099_configbus0;
wire [1099:1099] sram_blwl_1099_configbus1;
wire [1099:1099] sram_blwl_1099_configbus0_b;
assign sram_blwl_1099_configbus0[1099:1099] = sram_blwl_bl[1099:1099] ;
assign sram_blwl_1099_configbus1[1099:1099] = sram_blwl_wl[1099:1099] ;
assign sram_blwl_1099_configbus0_b[1099:1099] = sram_blwl_blb[1099:1099] ;
sram6T_blwl sram_blwl_1099_ (sram_blwl_out[1099], sram_blwl_out[1099], sram_blwl_outb[1099], sram_blwl_1099_configbus0[1099:1099], sram_blwl_1099_configbus1[1099:1099] , sram_blwl_1099_configbus0_b[1099:1099] );
wire [1100:1100] sram_blwl_1100_configbus0;
wire [1100:1100] sram_blwl_1100_configbus1;
wire [1100:1100] sram_blwl_1100_configbus0_b;
assign sram_blwl_1100_configbus0[1100:1100] = sram_blwl_bl[1100:1100] ;
assign sram_blwl_1100_configbus1[1100:1100] = sram_blwl_wl[1100:1100] ;
assign sram_blwl_1100_configbus0_b[1100:1100] = sram_blwl_blb[1100:1100] ;
sram6T_blwl sram_blwl_1100_ (sram_blwl_out[1100], sram_blwl_out[1100], sram_blwl_outb[1100], sram_blwl_1100_configbus0[1100:1100], sram_blwl_1100_configbus1[1100:1100] , sram_blwl_1100_configbus0_b[1100:1100] );
wire [1101:1101] sram_blwl_1101_configbus0;
wire [1101:1101] sram_blwl_1101_configbus1;
wire [1101:1101] sram_blwl_1101_configbus0_b;
assign sram_blwl_1101_configbus0[1101:1101] = sram_blwl_bl[1101:1101] ;
assign sram_blwl_1101_configbus1[1101:1101] = sram_blwl_wl[1101:1101] ;
assign sram_blwl_1101_configbus0_b[1101:1101] = sram_blwl_blb[1101:1101] ;
sram6T_blwl sram_blwl_1101_ (sram_blwl_out[1101], sram_blwl_out[1101], sram_blwl_outb[1101], sram_blwl_1101_configbus0[1101:1101], sram_blwl_1101_configbus1[1101:1101] , sram_blwl_1101_configbus0_b[1101:1101] );
wire [1102:1102] sram_blwl_1102_configbus0;
wire [1102:1102] sram_blwl_1102_configbus1;
wire [1102:1102] sram_blwl_1102_configbus0_b;
assign sram_blwl_1102_configbus0[1102:1102] = sram_blwl_bl[1102:1102] ;
assign sram_blwl_1102_configbus1[1102:1102] = sram_blwl_wl[1102:1102] ;
assign sram_blwl_1102_configbus0_b[1102:1102] = sram_blwl_blb[1102:1102] ;
sram6T_blwl sram_blwl_1102_ (sram_blwl_out[1102], sram_blwl_out[1102], sram_blwl_outb[1102], sram_blwl_1102_configbus0[1102:1102], sram_blwl_1102_configbus1[1102:1102] , sram_blwl_1102_configbus0_b[1102:1102] );
wire [1103:1103] sram_blwl_1103_configbus0;
wire [1103:1103] sram_blwl_1103_configbus1;
wire [1103:1103] sram_blwl_1103_configbus0_b;
assign sram_blwl_1103_configbus0[1103:1103] = sram_blwl_bl[1103:1103] ;
assign sram_blwl_1103_configbus1[1103:1103] = sram_blwl_wl[1103:1103] ;
assign sram_blwl_1103_configbus0_b[1103:1103] = sram_blwl_blb[1103:1103] ;
sram6T_blwl sram_blwl_1103_ (sram_blwl_out[1103], sram_blwl_out[1103], sram_blwl_outb[1103], sram_blwl_1103_configbus0[1103:1103], sram_blwl_1103_configbus1[1103:1103] , sram_blwl_1103_configbus0_b[1103:1103] );
wire [1104:1104] sram_blwl_1104_configbus0;
wire [1104:1104] sram_blwl_1104_configbus1;
wire [1104:1104] sram_blwl_1104_configbus0_b;
assign sram_blwl_1104_configbus0[1104:1104] = sram_blwl_bl[1104:1104] ;
assign sram_blwl_1104_configbus1[1104:1104] = sram_blwl_wl[1104:1104] ;
assign sram_blwl_1104_configbus0_b[1104:1104] = sram_blwl_blb[1104:1104] ;
sram6T_blwl sram_blwl_1104_ (sram_blwl_out[1104], sram_blwl_out[1104], sram_blwl_outb[1104], sram_blwl_1104_configbus0[1104:1104], sram_blwl_1104_configbus1[1104:1104] , sram_blwl_1104_configbus0_b[1104:1104] );
wire [1105:1105] sram_blwl_1105_configbus0;
wire [1105:1105] sram_blwl_1105_configbus1;
wire [1105:1105] sram_blwl_1105_configbus0_b;
assign sram_blwl_1105_configbus0[1105:1105] = sram_blwl_bl[1105:1105] ;
assign sram_blwl_1105_configbus1[1105:1105] = sram_blwl_wl[1105:1105] ;
assign sram_blwl_1105_configbus0_b[1105:1105] = sram_blwl_blb[1105:1105] ;
sram6T_blwl sram_blwl_1105_ (sram_blwl_out[1105], sram_blwl_out[1105], sram_blwl_outb[1105], sram_blwl_1105_configbus0[1105:1105], sram_blwl_1105_configbus1[1105:1105] , sram_blwl_1105_configbus0_b[1105:1105] );
wire [1106:1106] sram_blwl_1106_configbus0;
wire [1106:1106] sram_blwl_1106_configbus1;
wire [1106:1106] sram_blwl_1106_configbus0_b;
assign sram_blwl_1106_configbus0[1106:1106] = sram_blwl_bl[1106:1106] ;
assign sram_blwl_1106_configbus1[1106:1106] = sram_blwl_wl[1106:1106] ;
assign sram_blwl_1106_configbus0_b[1106:1106] = sram_blwl_blb[1106:1106] ;
sram6T_blwl sram_blwl_1106_ (sram_blwl_out[1106], sram_blwl_out[1106], sram_blwl_outb[1106], sram_blwl_1106_configbus0[1106:1106], sram_blwl_1106_configbus1[1106:1106] , sram_blwl_1106_configbus0_b[1106:1106] );
wire [1107:1107] sram_blwl_1107_configbus0;
wire [1107:1107] sram_blwl_1107_configbus1;
wire [1107:1107] sram_blwl_1107_configbus0_b;
assign sram_blwl_1107_configbus0[1107:1107] = sram_blwl_bl[1107:1107] ;
assign sram_blwl_1107_configbus1[1107:1107] = sram_blwl_wl[1107:1107] ;
assign sram_blwl_1107_configbus0_b[1107:1107] = sram_blwl_blb[1107:1107] ;
sram6T_blwl sram_blwl_1107_ (sram_blwl_out[1107], sram_blwl_out[1107], sram_blwl_outb[1107], sram_blwl_1107_configbus0[1107:1107], sram_blwl_1107_configbus1[1107:1107] , sram_blwl_1107_configbus0_b[1107:1107] );
wire [1108:1108] sram_blwl_1108_configbus0;
wire [1108:1108] sram_blwl_1108_configbus1;
wire [1108:1108] sram_blwl_1108_configbus0_b;
assign sram_blwl_1108_configbus0[1108:1108] = sram_blwl_bl[1108:1108] ;
assign sram_blwl_1108_configbus1[1108:1108] = sram_blwl_wl[1108:1108] ;
assign sram_blwl_1108_configbus0_b[1108:1108] = sram_blwl_blb[1108:1108] ;
sram6T_blwl sram_blwl_1108_ (sram_blwl_out[1108], sram_blwl_out[1108], sram_blwl_outb[1108], sram_blwl_1108_configbus0[1108:1108], sram_blwl_1108_configbus1[1108:1108] , sram_blwl_1108_configbus0_b[1108:1108] );
wire [1109:1109] sram_blwl_1109_configbus0;
wire [1109:1109] sram_blwl_1109_configbus1;
wire [1109:1109] sram_blwl_1109_configbus0_b;
assign sram_blwl_1109_configbus0[1109:1109] = sram_blwl_bl[1109:1109] ;
assign sram_blwl_1109_configbus1[1109:1109] = sram_blwl_wl[1109:1109] ;
assign sram_blwl_1109_configbus0_b[1109:1109] = sram_blwl_blb[1109:1109] ;
sram6T_blwl sram_blwl_1109_ (sram_blwl_out[1109], sram_blwl_out[1109], sram_blwl_outb[1109], sram_blwl_1109_configbus0[1109:1109], sram_blwl_1109_configbus1[1109:1109] , sram_blwl_1109_configbus0_b[1109:1109] );
wire [1110:1110] sram_blwl_1110_configbus0;
wire [1110:1110] sram_blwl_1110_configbus1;
wire [1110:1110] sram_blwl_1110_configbus0_b;
assign sram_blwl_1110_configbus0[1110:1110] = sram_blwl_bl[1110:1110] ;
assign sram_blwl_1110_configbus1[1110:1110] = sram_blwl_wl[1110:1110] ;
assign sram_blwl_1110_configbus0_b[1110:1110] = sram_blwl_blb[1110:1110] ;
sram6T_blwl sram_blwl_1110_ (sram_blwl_out[1110], sram_blwl_out[1110], sram_blwl_outb[1110], sram_blwl_1110_configbus0[1110:1110], sram_blwl_1110_configbus1[1110:1110] , sram_blwl_1110_configbus0_b[1110:1110] );
wire [1111:1111] sram_blwl_1111_configbus0;
wire [1111:1111] sram_blwl_1111_configbus1;
wire [1111:1111] sram_blwl_1111_configbus0_b;
assign sram_blwl_1111_configbus0[1111:1111] = sram_blwl_bl[1111:1111] ;
assign sram_blwl_1111_configbus1[1111:1111] = sram_blwl_wl[1111:1111] ;
assign sram_blwl_1111_configbus0_b[1111:1111] = sram_blwl_blb[1111:1111] ;
sram6T_blwl sram_blwl_1111_ (sram_blwl_out[1111], sram_blwl_out[1111], sram_blwl_outb[1111], sram_blwl_1111_configbus0[1111:1111], sram_blwl_1111_configbus1[1111:1111] , sram_blwl_1111_configbus0_b[1111:1111] );
wire [1112:1112] sram_blwl_1112_configbus0;
wire [1112:1112] sram_blwl_1112_configbus1;
wire [1112:1112] sram_blwl_1112_configbus0_b;
assign sram_blwl_1112_configbus0[1112:1112] = sram_blwl_bl[1112:1112] ;
assign sram_blwl_1112_configbus1[1112:1112] = sram_blwl_wl[1112:1112] ;
assign sram_blwl_1112_configbus0_b[1112:1112] = sram_blwl_blb[1112:1112] ;
sram6T_blwl sram_blwl_1112_ (sram_blwl_out[1112], sram_blwl_out[1112], sram_blwl_outb[1112], sram_blwl_1112_configbus0[1112:1112], sram_blwl_1112_configbus1[1112:1112] , sram_blwl_1112_configbus0_b[1112:1112] );
wire [1113:1113] sram_blwl_1113_configbus0;
wire [1113:1113] sram_blwl_1113_configbus1;
wire [1113:1113] sram_blwl_1113_configbus0_b;
assign sram_blwl_1113_configbus0[1113:1113] = sram_blwl_bl[1113:1113] ;
assign sram_blwl_1113_configbus1[1113:1113] = sram_blwl_wl[1113:1113] ;
assign sram_blwl_1113_configbus0_b[1113:1113] = sram_blwl_blb[1113:1113] ;
sram6T_blwl sram_blwl_1113_ (sram_blwl_out[1113], sram_blwl_out[1113], sram_blwl_outb[1113], sram_blwl_1113_configbus0[1113:1113], sram_blwl_1113_configbus1[1113:1113] , sram_blwl_1113_configbus0_b[1113:1113] );
wire [1114:1114] sram_blwl_1114_configbus0;
wire [1114:1114] sram_blwl_1114_configbus1;
wire [1114:1114] sram_blwl_1114_configbus0_b;
assign sram_blwl_1114_configbus0[1114:1114] = sram_blwl_bl[1114:1114] ;
assign sram_blwl_1114_configbus1[1114:1114] = sram_blwl_wl[1114:1114] ;
assign sram_blwl_1114_configbus0_b[1114:1114] = sram_blwl_blb[1114:1114] ;
sram6T_blwl sram_blwl_1114_ (sram_blwl_out[1114], sram_blwl_out[1114], sram_blwl_outb[1114], sram_blwl_1114_configbus0[1114:1114], sram_blwl_1114_configbus1[1114:1114] , sram_blwl_1114_configbus0_b[1114:1114] );
wire [1115:1115] sram_blwl_1115_configbus0;
wire [1115:1115] sram_blwl_1115_configbus1;
wire [1115:1115] sram_blwl_1115_configbus0_b;
assign sram_blwl_1115_configbus0[1115:1115] = sram_blwl_bl[1115:1115] ;
assign sram_blwl_1115_configbus1[1115:1115] = sram_blwl_wl[1115:1115] ;
assign sram_blwl_1115_configbus0_b[1115:1115] = sram_blwl_blb[1115:1115] ;
sram6T_blwl sram_blwl_1115_ (sram_blwl_out[1115], sram_blwl_out[1115], sram_blwl_outb[1115], sram_blwl_1115_configbus0[1115:1115], sram_blwl_1115_configbus1[1115:1115] , sram_blwl_1115_configbus0_b[1115:1115] );
wire [1116:1116] sram_blwl_1116_configbus0;
wire [1116:1116] sram_blwl_1116_configbus1;
wire [1116:1116] sram_blwl_1116_configbus0_b;
assign sram_blwl_1116_configbus0[1116:1116] = sram_blwl_bl[1116:1116] ;
assign sram_blwl_1116_configbus1[1116:1116] = sram_blwl_wl[1116:1116] ;
assign sram_blwl_1116_configbus0_b[1116:1116] = sram_blwl_blb[1116:1116] ;
sram6T_blwl sram_blwl_1116_ (sram_blwl_out[1116], sram_blwl_out[1116], sram_blwl_outb[1116], sram_blwl_1116_configbus0[1116:1116], sram_blwl_1116_configbus1[1116:1116] , sram_blwl_1116_configbus0_b[1116:1116] );
wire [1117:1117] sram_blwl_1117_configbus0;
wire [1117:1117] sram_blwl_1117_configbus1;
wire [1117:1117] sram_blwl_1117_configbus0_b;
assign sram_blwl_1117_configbus0[1117:1117] = sram_blwl_bl[1117:1117] ;
assign sram_blwl_1117_configbus1[1117:1117] = sram_blwl_wl[1117:1117] ;
assign sram_blwl_1117_configbus0_b[1117:1117] = sram_blwl_blb[1117:1117] ;
sram6T_blwl sram_blwl_1117_ (sram_blwl_out[1117], sram_blwl_out[1117], sram_blwl_outb[1117], sram_blwl_1117_configbus0[1117:1117], sram_blwl_1117_configbus1[1117:1117] , sram_blwl_1117_configbus0_b[1117:1117] );
wire [1118:1118] sram_blwl_1118_configbus0;
wire [1118:1118] sram_blwl_1118_configbus1;
wire [1118:1118] sram_blwl_1118_configbus0_b;
assign sram_blwl_1118_configbus0[1118:1118] = sram_blwl_bl[1118:1118] ;
assign sram_blwl_1118_configbus1[1118:1118] = sram_blwl_wl[1118:1118] ;
assign sram_blwl_1118_configbus0_b[1118:1118] = sram_blwl_blb[1118:1118] ;
sram6T_blwl sram_blwl_1118_ (sram_blwl_out[1118], sram_blwl_out[1118], sram_blwl_outb[1118], sram_blwl_1118_configbus0[1118:1118], sram_blwl_1118_configbus1[1118:1118] , sram_blwl_1118_configbus0_b[1118:1118] );
wire [1119:1119] sram_blwl_1119_configbus0;
wire [1119:1119] sram_blwl_1119_configbus1;
wire [1119:1119] sram_blwl_1119_configbus0_b;
assign sram_blwl_1119_configbus0[1119:1119] = sram_blwl_bl[1119:1119] ;
assign sram_blwl_1119_configbus1[1119:1119] = sram_blwl_wl[1119:1119] ;
assign sram_blwl_1119_configbus0_b[1119:1119] = sram_blwl_blb[1119:1119] ;
sram6T_blwl sram_blwl_1119_ (sram_blwl_out[1119], sram_blwl_out[1119], sram_blwl_outb[1119], sram_blwl_1119_configbus0[1119:1119], sram_blwl_1119_configbus1[1119:1119] , sram_blwl_1119_configbus0_b[1119:1119] );
wire [1120:1120] sram_blwl_1120_configbus0;
wire [1120:1120] sram_blwl_1120_configbus1;
wire [1120:1120] sram_blwl_1120_configbus0_b;
assign sram_blwl_1120_configbus0[1120:1120] = sram_blwl_bl[1120:1120] ;
assign sram_blwl_1120_configbus1[1120:1120] = sram_blwl_wl[1120:1120] ;
assign sram_blwl_1120_configbus0_b[1120:1120] = sram_blwl_blb[1120:1120] ;
sram6T_blwl sram_blwl_1120_ (sram_blwl_out[1120], sram_blwl_out[1120], sram_blwl_outb[1120], sram_blwl_1120_configbus0[1120:1120], sram_blwl_1120_configbus1[1120:1120] , sram_blwl_1120_configbus0_b[1120:1120] );
wire [1121:1121] sram_blwl_1121_configbus0;
wire [1121:1121] sram_blwl_1121_configbus1;
wire [1121:1121] sram_blwl_1121_configbus0_b;
assign sram_blwl_1121_configbus0[1121:1121] = sram_blwl_bl[1121:1121] ;
assign sram_blwl_1121_configbus1[1121:1121] = sram_blwl_wl[1121:1121] ;
assign sram_blwl_1121_configbus0_b[1121:1121] = sram_blwl_blb[1121:1121] ;
sram6T_blwl sram_blwl_1121_ (sram_blwl_out[1121], sram_blwl_out[1121], sram_blwl_outb[1121], sram_blwl_1121_configbus0[1121:1121], sram_blwl_1121_configbus1[1121:1121] , sram_blwl_1121_configbus0_b[1121:1121] );
wire [1122:1122] sram_blwl_1122_configbus0;
wire [1122:1122] sram_blwl_1122_configbus1;
wire [1122:1122] sram_blwl_1122_configbus0_b;
assign sram_blwl_1122_configbus0[1122:1122] = sram_blwl_bl[1122:1122] ;
assign sram_blwl_1122_configbus1[1122:1122] = sram_blwl_wl[1122:1122] ;
assign sram_blwl_1122_configbus0_b[1122:1122] = sram_blwl_blb[1122:1122] ;
sram6T_blwl sram_blwl_1122_ (sram_blwl_out[1122], sram_blwl_out[1122], sram_blwl_outb[1122], sram_blwl_1122_configbus0[1122:1122], sram_blwl_1122_configbus1[1122:1122] , sram_blwl_1122_configbus0_b[1122:1122] );
wire [1123:1123] sram_blwl_1123_configbus0;
wire [1123:1123] sram_blwl_1123_configbus1;
wire [1123:1123] sram_blwl_1123_configbus0_b;
assign sram_blwl_1123_configbus0[1123:1123] = sram_blwl_bl[1123:1123] ;
assign sram_blwl_1123_configbus1[1123:1123] = sram_blwl_wl[1123:1123] ;
assign sram_blwl_1123_configbus0_b[1123:1123] = sram_blwl_blb[1123:1123] ;
sram6T_blwl sram_blwl_1123_ (sram_blwl_out[1123], sram_blwl_out[1123], sram_blwl_outb[1123], sram_blwl_1123_configbus0[1123:1123], sram_blwl_1123_configbus1[1123:1123] , sram_blwl_1123_configbus0_b[1123:1123] );
wire [1124:1124] sram_blwl_1124_configbus0;
wire [1124:1124] sram_blwl_1124_configbus1;
wire [1124:1124] sram_blwl_1124_configbus0_b;
assign sram_blwl_1124_configbus0[1124:1124] = sram_blwl_bl[1124:1124] ;
assign sram_blwl_1124_configbus1[1124:1124] = sram_blwl_wl[1124:1124] ;
assign sram_blwl_1124_configbus0_b[1124:1124] = sram_blwl_blb[1124:1124] ;
sram6T_blwl sram_blwl_1124_ (sram_blwl_out[1124], sram_blwl_out[1124], sram_blwl_outb[1124], sram_blwl_1124_configbus0[1124:1124], sram_blwl_1124_configbus1[1124:1124] , sram_blwl_1124_configbus0_b[1124:1124] );
wire [1125:1125] sram_blwl_1125_configbus0;
wire [1125:1125] sram_blwl_1125_configbus1;
wire [1125:1125] sram_blwl_1125_configbus0_b;
assign sram_blwl_1125_configbus0[1125:1125] = sram_blwl_bl[1125:1125] ;
assign sram_blwl_1125_configbus1[1125:1125] = sram_blwl_wl[1125:1125] ;
assign sram_blwl_1125_configbus0_b[1125:1125] = sram_blwl_blb[1125:1125] ;
sram6T_blwl sram_blwl_1125_ (sram_blwl_out[1125], sram_blwl_out[1125], sram_blwl_outb[1125], sram_blwl_1125_configbus0[1125:1125], sram_blwl_1125_configbus1[1125:1125] , sram_blwl_1125_configbus0_b[1125:1125] );
wire [1126:1126] sram_blwl_1126_configbus0;
wire [1126:1126] sram_blwl_1126_configbus1;
wire [1126:1126] sram_blwl_1126_configbus0_b;
assign sram_blwl_1126_configbus0[1126:1126] = sram_blwl_bl[1126:1126] ;
assign sram_blwl_1126_configbus1[1126:1126] = sram_blwl_wl[1126:1126] ;
assign sram_blwl_1126_configbus0_b[1126:1126] = sram_blwl_blb[1126:1126] ;
sram6T_blwl sram_blwl_1126_ (sram_blwl_out[1126], sram_blwl_out[1126], sram_blwl_outb[1126], sram_blwl_1126_configbus0[1126:1126], sram_blwl_1126_configbus1[1126:1126] , sram_blwl_1126_configbus0_b[1126:1126] );
wire [1127:1127] sram_blwl_1127_configbus0;
wire [1127:1127] sram_blwl_1127_configbus1;
wire [1127:1127] sram_blwl_1127_configbus0_b;
assign sram_blwl_1127_configbus0[1127:1127] = sram_blwl_bl[1127:1127] ;
assign sram_blwl_1127_configbus1[1127:1127] = sram_blwl_wl[1127:1127] ;
assign sram_blwl_1127_configbus0_b[1127:1127] = sram_blwl_blb[1127:1127] ;
sram6T_blwl sram_blwl_1127_ (sram_blwl_out[1127], sram_blwl_out[1127], sram_blwl_outb[1127], sram_blwl_1127_configbus0[1127:1127], sram_blwl_1127_configbus1[1127:1127] , sram_blwl_1127_configbus0_b[1127:1127] );
wire [1128:1128] sram_blwl_1128_configbus0;
wire [1128:1128] sram_blwl_1128_configbus1;
wire [1128:1128] sram_blwl_1128_configbus0_b;
assign sram_blwl_1128_configbus0[1128:1128] = sram_blwl_bl[1128:1128] ;
assign sram_blwl_1128_configbus1[1128:1128] = sram_blwl_wl[1128:1128] ;
assign sram_blwl_1128_configbus0_b[1128:1128] = sram_blwl_blb[1128:1128] ;
sram6T_blwl sram_blwl_1128_ (sram_blwl_out[1128], sram_blwl_out[1128], sram_blwl_outb[1128], sram_blwl_1128_configbus0[1128:1128], sram_blwl_1128_configbus1[1128:1128] , sram_blwl_1128_configbus0_b[1128:1128] );
wire [1129:1129] sram_blwl_1129_configbus0;
wire [1129:1129] sram_blwl_1129_configbus1;
wire [1129:1129] sram_blwl_1129_configbus0_b;
assign sram_blwl_1129_configbus0[1129:1129] = sram_blwl_bl[1129:1129] ;
assign sram_blwl_1129_configbus1[1129:1129] = sram_blwl_wl[1129:1129] ;
assign sram_blwl_1129_configbus0_b[1129:1129] = sram_blwl_blb[1129:1129] ;
sram6T_blwl sram_blwl_1129_ (sram_blwl_out[1129], sram_blwl_out[1129], sram_blwl_outb[1129], sram_blwl_1129_configbus0[1129:1129], sram_blwl_1129_configbus1[1129:1129] , sram_blwl_1129_configbus0_b[1129:1129] );
wire [1130:1130] sram_blwl_1130_configbus0;
wire [1130:1130] sram_blwl_1130_configbus1;
wire [1130:1130] sram_blwl_1130_configbus0_b;
assign sram_blwl_1130_configbus0[1130:1130] = sram_blwl_bl[1130:1130] ;
assign sram_blwl_1130_configbus1[1130:1130] = sram_blwl_wl[1130:1130] ;
assign sram_blwl_1130_configbus0_b[1130:1130] = sram_blwl_blb[1130:1130] ;
sram6T_blwl sram_blwl_1130_ (sram_blwl_out[1130], sram_blwl_out[1130], sram_blwl_outb[1130], sram_blwl_1130_configbus0[1130:1130], sram_blwl_1130_configbus1[1130:1130] , sram_blwl_1130_configbus0_b[1130:1130] );
wire [1131:1131] sram_blwl_1131_configbus0;
wire [1131:1131] sram_blwl_1131_configbus1;
wire [1131:1131] sram_blwl_1131_configbus0_b;
assign sram_blwl_1131_configbus0[1131:1131] = sram_blwl_bl[1131:1131] ;
assign sram_blwl_1131_configbus1[1131:1131] = sram_blwl_wl[1131:1131] ;
assign sram_blwl_1131_configbus0_b[1131:1131] = sram_blwl_blb[1131:1131] ;
sram6T_blwl sram_blwl_1131_ (sram_blwl_out[1131], sram_blwl_out[1131], sram_blwl_outb[1131], sram_blwl_1131_configbus0[1131:1131], sram_blwl_1131_configbus1[1131:1131] , sram_blwl_1131_configbus0_b[1131:1131] );
wire [1132:1132] sram_blwl_1132_configbus0;
wire [1132:1132] sram_blwl_1132_configbus1;
wire [1132:1132] sram_blwl_1132_configbus0_b;
assign sram_blwl_1132_configbus0[1132:1132] = sram_blwl_bl[1132:1132] ;
assign sram_blwl_1132_configbus1[1132:1132] = sram_blwl_wl[1132:1132] ;
assign sram_blwl_1132_configbus0_b[1132:1132] = sram_blwl_blb[1132:1132] ;
sram6T_blwl sram_blwl_1132_ (sram_blwl_out[1132], sram_blwl_out[1132], sram_blwl_outb[1132], sram_blwl_1132_configbus0[1132:1132], sram_blwl_1132_configbus1[1132:1132] , sram_blwl_1132_configbus0_b[1132:1132] );
wire [1133:1133] sram_blwl_1133_configbus0;
wire [1133:1133] sram_blwl_1133_configbus1;
wire [1133:1133] sram_blwl_1133_configbus0_b;
assign sram_blwl_1133_configbus0[1133:1133] = sram_blwl_bl[1133:1133] ;
assign sram_blwl_1133_configbus1[1133:1133] = sram_blwl_wl[1133:1133] ;
assign sram_blwl_1133_configbus0_b[1133:1133] = sram_blwl_blb[1133:1133] ;
sram6T_blwl sram_blwl_1133_ (sram_blwl_out[1133], sram_blwl_out[1133], sram_blwl_outb[1133], sram_blwl_1133_configbus0[1133:1133], sram_blwl_1133_configbus1[1133:1133] , sram_blwl_1133_configbus0_b[1133:1133] );
wire [1134:1134] sram_blwl_1134_configbus0;
wire [1134:1134] sram_blwl_1134_configbus1;
wire [1134:1134] sram_blwl_1134_configbus0_b;
assign sram_blwl_1134_configbus0[1134:1134] = sram_blwl_bl[1134:1134] ;
assign sram_blwl_1134_configbus1[1134:1134] = sram_blwl_wl[1134:1134] ;
assign sram_blwl_1134_configbus0_b[1134:1134] = sram_blwl_blb[1134:1134] ;
sram6T_blwl sram_blwl_1134_ (sram_blwl_out[1134], sram_blwl_out[1134], sram_blwl_outb[1134], sram_blwl_1134_configbus0[1134:1134], sram_blwl_1134_configbus1[1134:1134] , sram_blwl_1134_configbus0_b[1134:1134] );
wire [1135:1135] sram_blwl_1135_configbus0;
wire [1135:1135] sram_blwl_1135_configbus1;
wire [1135:1135] sram_blwl_1135_configbus0_b;
assign sram_blwl_1135_configbus0[1135:1135] = sram_blwl_bl[1135:1135] ;
assign sram_blwl_1135_configbus1[1135:1135] = sram_blwl_wl[1135:1135] ;
assign sram_blwl_1135_configbus0_b[1135:1135] = sram_blwl_blb[1135:1135] ;
sram6T_blwl sram_blwl_1135_ (sram_blwl_out[1135], sram_blwl_out[1135], sram_blwl_outb[1135], sram_blwl_1135_configbus0[1135:1135], sram_blwl_1135_configbus1[1135:1135] , sram_blwl_1135_configbus0_b[1135:1135] );
wire [1136:1136] sram_blwl_1136_configbus0;
wire [1136:1136] sram_blwl_1136_configbus1;
wire [1136:1136] sram_blwl_1136_configbus0_b;
assign sram_blwl_1136_configbus0[1136:1136] = sram_blwl_bl[1136:1136] ;
assign sram_blwl_1136_configbus1[1136:1136] = sram_blwl_wl[1136:1136] ;
assign sram_blwl_1136_configbus0_b[1136:1136] = sram_blwl_blb[1136:1136] ;
sram6T_blwl sram_blwl_1136_ (sram_blwl_out[1136], sram_blwl_out[1136], sram_blwl_outb[1136], sram_blwl_1136_configbus0[1136:1136], sram_blwl_1136_configbus1[1136:1136] , sram_blwl_1136_configbus0_b[1136:1136] );
wire [1137:1137] sram_blwl_1137_configbus0;
wire [1137:1137] sram_blwl_1137_configbus1;
wire [1137:1137] sram_blwl_1137_configbus0_b;
assign sram_blwl_1137_configbus0[1137:1137] = sram_blwl_bl[1137:1137] ;
assign sram_blwl_1137_configbus1[1137:1137] = sram_blwl_wl[1137:1137] ;
assign sram_blwl_1137_configbus0_b[1137:1137] = sram_blwl_blb[1137:1137] ;
sram6T_blwl sram_blwl_1137_ (sram_blwl_out[1137], sram_blwl_out[1137], sram_blwl_outb[1137], sram_blwl_1137_configbus0[1137:1137], sram_blwl_1137_configbus1[1137:1137] , sram_blwl_1137_configbus0_b[1137:1137] );
wire [1138:1138] sram_blwl_1138_configbus0;
wire [1138:1138] sram_blwl_1138_configbus1;
wire [1138:1138] sram_blwl_1138_configbus0_b;
assign sram_blwl_1138_configbus0[1138:1138] = sram_blwl_bl[1138:1138] ;
assign sram_blwl_1138_configbus1[1138:1138] = sram_blwl_wl[1138:1138] ;
assign sram_blwl_1138_configbus0_b[1138:1138] = sram_blwl_blb[1138:1138] ;
sram6T_blwl sram_blwl_1138_ (sram_blwl_out[1138], sram_blwl_out[1138], sram_blwl_outb[1138], sram_blwl_1138_configbus0[1138:1138], sram_blwl_1138_configbus1[1138:1138] , sram_blwl_1138_configbus0_b[1138:1138] );
wire [1139:1139] sram_blwl_1139_configbus0;
wire [1139:1139] sram_blwl_1139_configbus1;
wire [1139:1139] sram_blwl_1139_configbus0_b;
assign sram_blwl_1139_configbus0[1139:1139] = sram_blwl_bl[1139:1139] ;
assign sram_blwl_1139_configbus1[1139:1139] = sram_blwl_wl[1139:1139] ;
assign sram_blwl_1139_configbus0_b[1139:1139] = sram_blwl_blb[1139:1139] ;
sram6T_blwl sram_blwl_1139_ (sram_blwl_out[1139], sram_blwl_out[1139], sram_blwl_outb[1139], sram_blwl_1139_configbus0[1139:1139], sram_blwl_1139_configbus1[1139:1139] , sram_blwl_1139_configbus0_b[1139:1139] );
wire [1140:1140] sram_blwl_1140_configbus0;
wire [1140:1140] sram_blwl_1140_configbus1;
wire [1140:1140] sram_blwl_1140_configbus0_b;
assign sram_blwl_1140_configbus0[1140:1140] = sram_blwl_bl[1140:1140] ;
assign sram_blwl_1140_configbus1[1140:1140] = sram_blwl_wl[1140:1140] ;
assign sram_blwl_1140_configbus0_b[1140:1140] = sram_blwl_blb[1140:1140] ;
sram6T_blwl sram_blwl_1140_ (sram_blwl_out[1140], sram_blwl_out[1140], sram_blwl_outb[1140], sram_blwl_1140_configbus0[1140:1140], sram_blwl_1140_configbus1[1140:1140] , sram_blwl_1140_configbus0_b[1140:1140] );
wire [1141:1141] sram_blwl_1141_configbus0;
wire [1141:1141] sram_blwl_1141_configbus1;
wire [1141:1141] sram_blwl_1141_configbus0_b;
assign sram_blwl_1141_configbus0[1141:1141] = sram_blwl_bl[1141:1141] ;
assign sram_blwl_1141_configbus1[1141:1141] = sram_blwl_wl[1141:1141] ;
assign sram_blwl_1141_configbus0_b[1141:1141] = sram_blwl_blb[1141:1141] ;
sram6T_blwl sram_blwl_1141_ (sram_blwl_out[1141], sram_blwl_out[1141], sram_blwl_outb[1141], sram_blwl_1141_configbus0[1141:1141], sram_blwl_1141_configbus1[1141:1141] , sram_blwl_1141_configbus0_b[1141:1141] );
wire [1142:1142] sram_blwl_1142_configbus0;
wire [1142:1142] sram_blwl_1142_configbus1;
wire [1142:1142] sram_blwl_1142_configbus0_b;
assign sram_blwl_1142_configbus0[1142:1142] = sram_blwl_bl[1142:1142] ;
assign sram_blwl_1142_configbus1[1142:1142] = sram_blwl_wl[1142:1142] ;
assign sram_blwl_1142_configbus0_b[1142:1142] = sram_blwl_blb[1142:1142] ;
sram6T_blwl sram_blwl_1142_ (sram_blwl_out[1142], sram_blwl_out[1142], sram_blwl_outb[1142], sram_blwl_1142_configbus0[1142:1142], sram_blwl_1142_configbus1[1142:1142] , sram_blwl_1142_configbus0_b[1142:1142] );
wire [1143:1143] sram_blwl_1143_configbus0;
wire [1143:1143] sram_blwl_1143_configbus1;
wire [1143:1143] sram_blwl_1143_configbus0_b;
assign sram_blwl_1143_configbus0[1143:1143] = sram_blwl_bl[1143:1143] ;
assign sram_blwl_1143_configbus1[1143:1143] = sram_blwl_wl[1143:1143] ;
assign sram_blwl_1143_configbus0_b[1143:1143] = sram_blwl_blb[1143:1143] ;
sram6T_blwl sram_blwl_1143_ (sram_blwl_out[1143], sram_blwl_out[1143], sram_blwl_outb[1143], sram_blwl_1143_configbus0[1143:1143], sram_blwl_1143_configbus1[1143:1143] , sram_blwl_1143_configbus0_b[1143:1143] );
wire [1144:1144] sram_blwl_1144_configbus0;
wire [1144:1144] sram_blwl_1144_configbus1;
wire [1144:1144] sram_blwl_1144_configbus0_b;
assign sram_blwl_1144_configbus0[1144:1144] = sram_blwl_bl[1144:1144] ;
assign sram_blwl_1144_configbus1[1144:1144] = sram_blwl_wl[1144:1144] ;
assign sram_blwl_1144_configbus0_b[1144:1144] = sram_blwl_blb[1144:1144] ;
sram6T_blwl sram_blwl_1144_ (sram_blwl_out[1144], sram_blwl_out[1144], sram_blwl_outb[1144], sram_blwl_1144_configbus0[1144:1144], sram_blwl_1144_configbus1[1144:1144] , sram_blwl_1144_configbus0_b[1144:1144] );
endmodule
//----- END LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----

//----- Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----
module grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
input [0:0] Set,
input [0:0] Reset,
input [0:0] clk
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
input wire ff_0___D_0_,
output wire ff_0___Q_0_);
static_dff dff_1_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
endmodule
//----- END Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6_ -----
module grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_ble6___in_0_,
input wire mode_ble6___in_1_,
input wire mode_ble6___in_2_,
input wire mode_ble6___in_3_,
input wire mode_ble6___in_4_,
input wire mode_ble6___in_5_,
output wire mode_ble6___out_0_,
input wire mode_ble6___clk_0_,
input [1081:1145] sram_blwl_bl ,
input [1081:1145] sram_blwl_wl ,
input [1081:1145] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ lut6_0_ (
 lut6_0___in_0_,  lut6_0___in_1_,  lut6_0___in_2_,  lut6_0___in_3_,  lut6_0___in_4_,  lut6_0___in_5_,  lut6_0___out_0_,
sram_blwl_bl[1081:1144] ,
sram_blwl_wl[1081:1144] ,
sram_blwl_blb[1081:1144] );
grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ ff_0_ (
//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
wire [0:1] in_bus_mux_1level_tapbuf_size2_401_ ;
assign in_bus_mux_1level_tapbuf_size2_401_[0] = ff_0___Q_0_ ; 
assign in_bus_mux_1level_tapbuf_size2_401_[1] = lut6_0___out_0_ ; 
wire [1145:1145] mux_1level_tapbuf_size2_401_configbus0;
wire [1145:1145] mux_1level_tapbuf_size2_401_configbus1;
wire [1145:1145] mux_1level_tapbuf_size2_401_sram_blwl_out ;
wire [1145:1145] mux_1level_tapbuf_size2_401_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_401_configbus0[1145:1145] = sram_blwl_bl[1145:1145] ;
assign mux_1level_tapbuf_size2_401_configbus1[1145:1145] = sram_blwl_wl[1145:1145] ;
wire [1145:1145] mux_1level_tapbuf_size2_401_configbus0_b;
assign mux_1level_tapbuf_size2_401_configbus0_b[1145:1145] = sram_blwl_blb[1145:1145] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_401_ (in_bus_mux_1level_tapbuf_size2_401_, mode_ble6___out_0_, mux_1level_tapbuf_size2_401_sram_blwl_out[1145:1145] ,
mux_1level_tapbuf_size2_401_sram_blwl_outb[1145:1145] );
//----- SRAM bits for MUX[401], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_1145_ (mux_1level_tapbuf_size2_401_sram_blwl_out[1145:1145] ,mux_1level_tapbuf_size2_401_sram_blwl_out[1145:1145] ,mux_1level_tapbuf_size2_401_sram_blwl_outb[1145:1145] ,mux_1level_tapbuf_size2_401_configbus0[1145:1145], mux_1level_tapbuf_size2_401_configbus1[1145:1145] , mux_1level_tapbuf_size2_401_configbus0_b[1145:1145] );
direct_interc direct_interc_16_ (mode_ble6___in_0_, lut6_0___in_0_ );
direct_interc direct_interc_17_ (mode_ble6___in_1_, lut6_0___in_1_ );
direct_interc direct_interc_18_ (mode_ble6___in_2_, lut6_0___in_2_ );
direct_interc direct_interc_19_ (mode_ble6___in_3_, lut6_0___in_3_ );
direct_interc direct_interc_20_ (mode_ble6___in_4_, lut6_0___in_4_ );
direct_interc direct_interc_21_ (mode_ble6___in_5_, lut6_0___in_5_ );
direct_interc direct_interc_22_ (lut6_0___out_0_, ff_0___D_0_ );
direct_interc direct_interc_23_ (mode_ble6___clk_0_, ff_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6_ -----
module grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_n1_lut6___in_0_,
input wire mode_n1_lut6___in_1_,
input wire mode_n1_lut6___in_2_,
input wire mode_n1_lut6___in_3_,
input wire mode_n1_lut6___in_4_,
input wire mode_n1_lut6___in_5_,
output wire mode_n1_lut6___out_0_,
input wire mode_n1_lut6___clk_0_,
input [1081:1145] sram_blwl_bl ,
input [1081:1145] sram_blwl_wl ,
input [1081:1145] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6__ble6_0__mode_ble6_ ble6_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 ble6_0___in_0_,  ble6_0___in_1_,  ble6_0___in_2_,  ble6_0___in_3_,  ble6_0___in_4_,  ble6_0___in_5_,  ble6_0___out_0_,  ble6_0___clk_0_,
sram_blwl_bl[1081:1145] ,
sram_blwl_wl[1081:1145] ,
sram_blwl_blb[1081:1145] );
direct_interc direct_interc_24_ (ble6_0___out_0_, mode_n1_lut6___out_0_ );
direct_interc direct_interc_25_ (mode_n1_lut6___in_0_, ble6_0___in_0_ );
direct_interc direct_interc_26_ (mode_n1_lut6___in_1_, ble6_0___in_1_ );
direct_interc direct_interc_27_ (mode_n1_lut6___in_2_, ble6_0___in_2_ );
direct_interc direct_interc_28_ (mode_n1_lut6___in_3_, ble6_0___in_3_ );
direct_interc direct_interc_29_ (mode_n1_lut6___in_4_, ble6_0___in_4_ );
direct_interc direct_interc_30_ (mode_n1_lut6___in_5_, ble6_0___in_5_ );
direct_interc direct_interc_31_ (mode_n1_lut6___clk_0_, ble6_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6_ -----

//----- LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----
module grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ (
input wire lut6_0___in_0_,
input wire lut6_0___in_1_,
input wire lut6_0___in_2_,
input wire lut6_0___in_3_,
input wire lut6_0___in_4_,
input wire lut6_0___in_5_,
output wire lut6_0___out_0_,
input [1146:1209] sram_blwl_bl ,
input [1146:1209] sram_blwl_wl ,
input [1146:1209] sram_blwl_blb );
wire [0:5] lut6_0___in;
assign lut6_0___in[0] = lut6_0___in_0_;
assign lut6_0___in[1] = lut6_0___in_1_;
assign lut6_0___in[2] = lut6_0___in_2_;
assign lut6_0___in[3] = lut6_0___in_3_;
assign lut6_0___in[4] = lut6_0___in_4_;
assign lut6_0___in[5] = lut6_0___in_5_;
wire [0:0] lut6_0___out;
assign lut6_0___out_0_ = lut6_0___out[0];
wire [1146:1209] sram_blwl_out ;
wire [1146:1209] sram_blwl_outb ;
lut6 lut6_2_ (
//----- Input and output ports -----
 lut6_0___in[0:5] ,  lut6_0___out[0:0],//----- SRAM ports -----
sram_blwl_out[1146:1209] , sram_blwl_outb[1146:1209] );
//----- Truth Table for LUT[2], size=6. -----
//----- SRAM bits for LUT[2], size=6, num_sram=64. -----
//-----0000000000000000000000000000000000000000000000000000000000000000-----
wire [1146:1146] sram_blwl_1146_configbus0;
wire [1146:1146] sram_blwl_1146_configbus1;
wire [1146:1146] sram_blwl_1146_configbus0_b;
assign sram_blwl_1146_configbus0[1146:1146] = sram_blwl_bl[1146:1146] ;
assign sram_blwl_1146_configbus1[1146:1146] = sram_blwl_wl[1146:1146] ;
assign sram_blwl_1146_configbus0_b[1146:1146] = sram_blwl_blb[1146:1146] ;
sram6T_blwl sram_blwl_1146_ (sram_blwl_out[1146], sram_blwl_out[1146], sram_blwl_outb[1146], sram_blwl_1146_configbus0[1146:1146], sram_blwl_1146_configbus1[1146:1146] , sram_blwl_1146_configbus0_b[1146:1146] );
wire [1147:1147] sram_blwl_1147_configbus0;
wire [1147:1147] sram_blwl_1147_configbus1;
wire [1147:1147] sram_blwl_1147_configbus0_b;
assign sram_blwl_1147_configbus0[1147:1147] = sram_blwl_bl[1147:1147] ;
assign sram_blwl_1147_configbus1[1147:1147] = sram_blwl_wl[1147:1147] ;
assign sram_blwl_1147_configbus0_b[1147:1147] = sram_blwl_blb[1147:1147] ;
sram6T_blwl sram_blwl_1147_ (sram_blwl_out[1147], sram_blwl_out[1147], sram_blwl_outb[1147], sram_blwl_1147_configbus0[1147:1147], sram_blwl_1147_configbus1[1147:1147] , sram_blwl_1147_configbus0_b[1147:1147] );
wire [1148:1148] sram_blwl_1148_configbus0;
wire [1148:1148] sram_blwl_1148_configbus1;
wire [1148:1148] sram_blwl_1148_configbus0_b;
assign sram_blwl_1148_configbus0[1148:1148] = sram_blwl_bl[1148:1148] ;
assign sram_blwl_1148_configbus1[1148:1148] = sram_blwl_wl[1148:1148] ;
assign sram_blwl_1148_configbus0_b[1148:1148] = sram_blwl_blb[1148:1148] ;
sram6T_blwl sram_blwl_1148_ (sram_blwl_out[1148], sram_blwl_out[1148], sram_blwl_outb[1148], sram_blwl_1148_configbus0[1148:1148], sram_blwl_1148_configbus1[1148:1148] , sram_blwl_1148_configbus0_b[1148:1148] );
wire [1149:1149] sram_blwl_1149_configbus0;
wire [1149:1149] sram_blwl_1149_configbus1;
wire [1149:1149] sram_blwl_1149_configbus0_b;
assign sram_blwl_1149_configbus0[1149:1149] = sram_blwl_bl[1149:1149] ;
assign sram_blwl_1149_configbus1[1149:1149] = sram_blwl_wl[1149:1149] ;
assign sram_blwl_1149_configbus0_b[1149:1149] = sram_blwl_blb[1149:1149] ;
sram6T_blwl sram_blwl_1149_ (sram_blwl_out[1149], sram_blwl_out[1149], sram_blwl_outb[1149], sram_blwl_1149_configbus0[1149:1149], sram_blwl_1149_configbus1[1149:1149] , sram_blwl_1149_configbus0_b[1149:1149] );
wire [1150:1150] sram_blwl_1150_configbus0;
wire [1150:1150] sram_blwl_1150_configbus1;
wire [1150:1150] sram_blwl_1150_configbus0_b;
assign sram_blwl_1150_configbus0[1150:1150] = sram_blwl_bl[1150:1150] ;
assign sram_blwl_1150_configbus1[1150:1150] = sram_blwl_wl[1150:1150] ;
assign sram_blwl_1150_configbus0_b[1150:1150] = sram_blwl_blb[1150:1150] ;
sram6T_blwl sram_blwl_1150_ (sram_blwl_out[1150], sram_blwl_out[1150], sram_blwl_outb[1150], sram_blwl_1150_configbus0[1150:1150], sram_blwl_1150_configbus1[1150:1150] , sram_blwl_1150_configbus0_b[1150:1150] );
wire [1151:1151] sram_blwl_1151_configbus0;
wire [1151:1151] sram_blwl_1151_configbus1;
wire [1151:1151] sram_blwl_1151_configbus0_b;
assign sram_blwl_1151_configbus0[1151:1151] = sram_blwl_bl[1151:1151] ;
assign sram_blwl_1151_configbus1[1151:1151] = sram_blwl_wl[1151:1151] ;
assign sram_blwl_1151_configbus0_b[1151:1151] = sram_blwl_blb[1151:1151] ;
sram6T_blwl sram_blwl_1151_ (sram_blwl_out[1151], sram_blwl_out[1151], sram_blwl_outb[1151], sram_blwl_1151_configbus0[1151:1151], sram_blwl_1151_configbus1[1151:1151] , sram_blwl_1151_configbus0_b[1151:1151] );
wire [1152:1152] sram_blwl_1152_configbus0;
wire [1152:1152] sram_blwl_1152_configbus1;
wire [1152:1152] sram_blwl_1152_configbus0_b;
assign sram_blwl_1152_configbus0[1152:1152] = sram_blwl_bl[1152:1152] ;
assign sram_blwl_1152_configbus1[1152:1152] = sram_blwl_wl[1152:1152] ;
assign sram_blwl_1152_configbus0_b[1152:1152] = sram_blwl_blb[1152:1152] ;
sram6T_blwl sram_blwl_1152_ (sram_blwl_out[1152], sram_blwl_out[1152], sram_blwl_outb[1152], sram_blwl_1152_configbus0[1152:1152], sram_blwl_1152_configbus1[1152:1152] , sram_blwl_1152_configbus0_b[1152:1152] );
wire [1153:1153] sram_blwl_1153_configbus0;
wire [1153:1153] sram_blwl_1153_configbus1;
wire [1153:1153] sram_blwl_1153_configbus0_b;
assign sram_blwl_1153_configbus0[1153:1153] = sram_blwl_bl[1153:1153] ;
assign sram_blwl_1153_configbus1[1153:1153] = sram_blwl_wl[1153:1153] ;
assign sram_blwl_1153_configbus0_b[1153:1153] = sram_blwl_blb[1153:1153] ;
sram6T_blwl sram_blwl_1153_ (sram_blwl_out[1153], sram_blwl_out[1153], sram_blwl_outb[1153], sram_blwl_1153_configbus0[1153:1153], sram_blwl_1153_configbus1[1153:1153] , sram_blwl_1153_configbus0_b[1153:1153] );
wire [1154:1154] sram_blwl_1154_configbus0;
wire [1154:1154] sram_blwl_1154_configbus1;
wire [1154:1154] sram_blwl_1154_configbus0_b;
assign sram_blwl_1154_configbus0[1154:1154] = sram_blwl_bl[1154:1154] ;
assign sram_blwl_1154_configbus1[1154:1154] = sram_blwl_wl[1154:1154] ;
assign sram_blwl_1154_configbus0_b[1154:1154] = sram_blwl_blb[1154:1154] ;
sram6T_blwl sram_blwl_1154_ (sram_blwl_out[1154], sram_blwl_out[1154], sram_blwl_outb[1154], sram_blwl_1154_configbus0[1154:1154], sram_blwl_1154_configbus1[1154:1154] , sram_blwl_1154_configbus0_b[1154:1154] );
wire [1155:1155] sram_blwl_1155_configbus0;
wire [1155:1155] sram_blwl_1155_configbus1;
wire [1155:1155] sram_blwl_1155_configbus0_b;
assign sram_blwl_1155_configbus0[1155:1155] = sram_blwl_bl[1155:1155] ;
assign sram_blwl_1155_configbus1[1155:1155] = sram_blwl_wl[1155:1155] ;
assign sram_blwl_1155_configbus0_b[1155:1155] = sram_blwl_blb[1155:1155] ;
sram6T_blwl sram_blwl_1155_ (sram_blwl_out[1155], sram_blwl_out[1155], sram_blwl_outb[1155], sram_blwl_1155_configbus0[1155:1155], sram_blwl_1155_configbus1[1155:1155] , sram_blwl_1155_configbus0_b[1155:1155] );
wire [1156:1156] sram_blwl_1156_configbus0;
wire [1156:1156] sram_blwl_1156_configbus1;
wire [1156:1156] sram_blwl_1156_configbus0_b;
assign sram_blwl_1156_configbus0[1156:1156] = sram_blwl_bl[1156:1156] ;
assign sram_blwl_1156_configbus1[1156:1156] = sram_blwl_wl[1156:1156] ;
assign sram_blwl_1156_configbus0_b[1156:1156] = sram_blwl_blb[1156:1156] ;
sram6T_blwl sram_blwl_1156_ (sram_blwl_out[1156], sram_blwl_out[1156], sram_blwl_outb[1156], sram_blwl_1156_configbus0[1156:1156], sram_blwl_1156_configbus1[1156:1156] , sram_blwl_1156_configbus0_b[1156:1156] );
wire [1157:1157] sram_blwl_1157_configbus0;
wire [1157:1157] sram_blwl_1157_configbus1;
wire [1157:1157] sram_blwl_1157_configbus0_b;
assign sram_blwl_1157_configbus0[1157:1157] = sram_blwl_bl[1157:1157] ;
assign sram_blwl_1157_configbus1[1157:1157] = sram_blwl_wl[1157:1157] ;
assign sram_blwl_1157_configbus0_b[1157:1157] = sram_blwl_blb[1157:1157] ;
sram6T_blwl sram_blwl_1157_ (sram_blwl_out[1157], sram_blwl_out[1157], sram_blwl_outb[1157], sram_blwl_1157_configbus0[1157:1157], sram_blwl_1157_configbus1[1157:1157] , sram_blwl_1157_configbus0_b[1157:1157] );
wire [1158:1158] sram_blwl_1158_configbus0;
wire [1158:1158] sram_blwl_1158_configbus1;
wire [1158:1158] sram_blwl_1158_configbus0_b;
assign sram_blwl_1158_configbus0[1158:1158] = sram_blwl_bl[1158:1158] ;
assign sram_blwl_1158_configbus1[1158:1158] = sram_blwl_wl[1158:1158] ;
assign sram_blwl_1158_configbus0_b[1158:1158] = sram_blwl_blb[1158:1158] ;
sram6T_blwl sram_blwl_1158_ (sram_blwl_out[1158], sram_blwl_out[1158], sram_blwl_outb[1158], sram_blwl_1158_configbus0[1158:1158], sram_blwl_1158_configbus1[1158:1158] , sram_blwl_1158_configbus0_b[1158:1158] );
wire [1159:1159] sram_blwl_1159_configbus0;
wire [1159:1159] sram_blwl_1159_configbus1;
wire [1159:1159] sram_blwl_1159_configbus0_b;
assign sram_blwl_1159_configbus0[1159:1159] = sram_blwl_bl[1159:1159] ;
assign sram_blwl_1159_configbus1[1159:1159] = sram_blwl_wl[1159:1159] ;
assign sram_blwl_1159_configbus0_b[1159:1159] = sram_blwl_blb[1159:1159] ;
sram6T_blwl sram_blwl_1159_ (sram_blwl_out[1159], sram_blwl_out[1159], sram_blwl_outb[1159], sram_blwl_1159_configbus0[1159:1159], sram_blwl_1159_configbus1[1159:1159] , sram_blwl_1159_configbus0_b[1159:1159] );
wire [1160:1160] sram_blwl_1160_configbus0;
wire [1160:1160] sram_blwl_1160_configbus1;
wire [1160:1160] sram_blwl_1160_configbus0_b;
assign sram_blwl_1160_configbus0[1160:1160] = sram_blwl_bl[1160:1160] ;
assign sram_blwl_1160_configbus1[1160:1160] = sram_blwl_wl[1160:1160] ;
assign sram_blwl_1160_configbus0_b[1160:1160] = sram_blwl_blb[1160:1160] ;
sram6T_blwl sram_blwl_1160_ (sram_blwl_out[1160], sram_blwl_out[1160], sram_blwl_outb[1160], sram_blwl_1160_configbus0[1160:1160], sram_blwl_1160_configbus1[1160:1160] , sram_blwl_1160_configbus0_b[1160:1160] );
wire [1161:1161] sram_blwl_1161_configbus0;
wire [1161:1161] sram_blwl_1161_configbus1;
wire [1161:1161] sram_blwl_1161_configbus0_b;
assign sram_blwl_1161_configbus0[1161:1161] = sram_blwl_bl[1161:1161] ;
assign sram_blwl_1161_configbus1[1161:1161] = sram_blwl_wl[1161:1161] ;
assign sram_blwl_1161_configbus0_b[1161:1161] = sram_blwl_blb[1161:1161] ;
sram6T_blwl sram_blwl_1161_ (sram_blwl_out[1161], sram_blwl_out[1161], sram_blwl_outb[1161], sram_blwl_1161_configbus0[1161:1161], sram_blwl_1161_configbus1[1161:1161] , sram_blwl_1161_configbus0_b[1161:1161] );
wire [1162:1162] sram_blwl_1162_configbus0;
wire [1162:1162] sram_blwl_1162_configbus1;
wire [1162:1162] sram_blwl_1162_configbus0_b;
assign sram_blwl_1162_configbus0[1162:1162] = sram_blwl_bl[1162:1162] ;
assign sram_blwl_1162_configbus1[1162:1162] = sram_blwl_wl[1162:1162] ;
assign sram_blwl_1162_configbus0_b[1162:1162] = sram_blwl_blb[1162:1162] ;
sram6T_blwl sram_blwl_1162_ (sram_blwl_out[1162], sram_blwl_out[1162], sram_blwl_outb[1162], sram_blwl_1162_configbus0[1162:1162], sram_blwl_1162_configbus1[1162:1162] , sram_blwl_1162_configbus0_b[1162:1162] );
wire [1163:1163] sram_blwl_1163_configbus0;
wire [1163:1163] sram_blwl_1163_configbus1;
wire [1163:1163] sram_blwl_1163_configbus0_b;
assign sram_blwl_1163_configbus0[1163:1163] = sram_blwl_bl[1163:1163] ;
assign sram_blwl_1163_configbus1[1163:1163] = sram_blwl_wl[1163:1163] ;
assign sram_blwl_1163_configbus0_b[1163:1163] = sram_blwl_blb[1163:1163] ;
sram6T_blwl sram_blwl_1163_ (sram_blwl_out[1163], sram_blwl_out[1163], sram_blwl_outb[1163], sram_blwl_1163_configbus0[1163:1163], sram_blwl_1163_configbus1[1163:1163] , sram_blwl_1163_configbus0_b[1163:1163] );
wire [1164:1164] sram_blwl_1164_configbus0;
wire [1164:1164] sram_blwl_1164_configbus1;
wire [1164:1164] sram_blwl_1164_configbus0_b;
assign sram_blwl_1164_configbus0[1164:1164] = sram_blwl_bl[1164:1164] ;
assign sram_blwl_1164_configbus1[1164:1164] = sram_blwl_wl[1164:1164] ;
assign sram_blwl_1164_configbus0_b[1164:1164] = sram_blwl_blb[1164:1164] ;
sram6T_blwl sram_blwl_1164_ (sram_blwl_out[1164], sram_blwl_out[1164], sram_blwl_outb[1164], sram_blwl_1164_configbus0[1164:1164], sram_blwl_1164_configbus1[1164:1164] , sram_blwl_1164_configbus0_b[1164:1164] );
wire [1165:1165] sram_blwl_1165_configbus0;
wire [1165:1165] sram_blwl_1165_configbus1;
wire [1165:1165] sram_blwl_1165_configbus0_b;
assign sram_blwl_1165_configbus0[1165:1165] = sram_blwl_bl[1165:1165] ;
assign sram_blwl_1165_configbus1[1165:1165] = sram_blwl_wl[1165:1165] ;
assign sram_blwl_1165_configbus0_b[1165:1165] = sram_blwl_blb[1165:1165] ;
sram6T_blwl sram_blwl_1165_ (sram_blwl_out[1165], sram_blwl_out[1165], sram_blwl_outb[1165], sram_blwl_1165_configbus0[1165:1165], sram_blwl_1165_configbus1[1165:1165] , sram_blwl_1165_configbus0_b[1165:1165] );
wire [1166:1166] sram_blwl_1166_configbus0;
wire [1166:1166] sram_blwl_1166_configbus1;
wire [1166:1166] sram_blwl_1166_configbus0_b;
assign sram_blwl_1166_configbus0[1166:1166] = sram_blwl_bl[1166:1166] ;
assign sram_blwl_1166_configbus1[1166:1166] = sram_blwl_wl[1166:1166] ;
assign sram_blwl_1166_configbus0_b[1166:1166] = sram_blwl_blb[1166:1166] ;
sram6T_blwl sram_blwl_1166_ (sram_blwl_out[1166], sram_blwl_out[1166], sram_blwl_outb[1166], sram_blwl_1166_configbus0[1166:1166], sram_blwl_1166_configbus1[1166:1166] , sram_blwl_1166_configbus0_b[1166:1166] );
wire [1167:1167] sram_blwl_1167_configbus0;
wire [1167:1167] sram_blwl_1167_configbus1;
wire [1167:1167] sram_blwl_1167_configbus0_b;
assign sram_blwl_1167_configbus0[1167:1167] = sram_blwl_bl[1167:1167] ;
assign sram_blwl_1167_configbus1[1167:1167] = sram_blwl_wl[1167:1167] ;
assign sram_blwl_1167_configbus0_b[1167:1167] = sram_blwl_blb[1167:1167] ;
sram6T_blwl sram_blwl_1167_ (sram_blwl_out[1167], sram_blwl_out[1167], sram_blwl_outb[1167], sram_blwl_1167_configbus0[1167:1167], sram_blwl_1167_configbus1[1167:1167] , sram_blwl_1167_configbus0_b[1167:1167] );
wire [1168:1168] sram_blwl_1168_configbus0;
wire [1168:1168] sram_blwl_1168_configbus1;
wire [1168:1168] sram_blwl_1168_configbus0_b;
assign sram_blwl_1168_configbus0[1168:1168] = sram_blwl_bl[1168:1168] ;
assign sram_blwl_1168_configbus1[1168:1168] = sram_blwl_wl[1168:1168] ;
assign sram_blwl_1168_configbus0_b[1168:1168] = sram_blwl_blb[1168:1168] ;
sram6T_blwl sram_blwl_1168_ (sram_blwl_out[1168], sram_blwl_out[1168], sram_blwl_outb[1168], sram_blwl_1168_configbus0[1168:1168], sram_blwl_1168_configbus1[1168:1168] , sram_blwl_1168_configbus0_b[1168:1168] );
wire [1169:1169] sram_blwl_1169_configbus0;
wire [1169:1169] sram_blwl_1169_configbus1;
wire [1169:1169] sram_blwl_1169_configbus0_b;
assign sram_blwl_1169_configbus0[1169:1169] = sram_blwl_bl[1169:1169] ;
assign sram_blwl_1169_configbus1[1169:1169] = sram_blwl_wl[1169:1169] ;
assign sram_blwl_1169_configbus0_b[1169:1169] = sram_blwl_blb[1169:1169] ;
sram6T_blwl sram_blwl_1169_ (sram_blwl_out[1169], sram_blwl_out[1169], sram_blwl_outb[1169], sram_blwl_1169_configbus0[1169:1169], sram_blwl_1169_configbus1[1169:1169] , sram_blwl_1169_configbus0_b[1169:1169] );
wire [1170:1170] sram_blwl_1170_configbus0;
wire [1170:1170] sram_blwl_1170_configbus1;
wire [1170:1170] sram_blwl_1170_configbus0_b;
assign sram_blwl_1170_configbus0[1170:1170] = sram_blwl_bl[1170:1170] ;
assign sram_blwl_1170_configbus1[1170:1170] = sram_blwl_wl[1170:1170] ;
assign sram_blwl_1170_configbus0_b[1170:1170] = sram_blwl_blb[1170:1170] ;
sram6T_blwl sram_blwl_1170_ (sram_blwl_out[1170], sram_blwl_out[1170], sram_blwl_outb[1170], sram_blwl_1170_configbus0[1170:1170], sram_blwl_1170_configbus1[1170:1170] , sram_blwl_1170_configbus0_b[1170:1170] );
wire [1171:1171] sram_blwl_1171_configbus0;
wire [1171:1171] sram_blwl_1171_configbus1;
wire [1171:1171] sram_blwl_1171_configbus0_b;
assign sram_blwl_1171_configbus0[1171:1171] = sram_blwl_bl[1171:1171] ;
assign sram_blwl_1171_configbus1[1171:1171] = sram_blwl_wl[1171:1171] ;
assign sram_blwl_1171_configbus0_b[1171:1171] = sram_blwl_blb[1171:1171] ;
sram6T_blwl sram_blwl_1171_ (sram_blwl_out[1171], sram_blwl_out[1171], sram_blwl_outb[1171], sram_blwl_1171_configbus0[1171:1171], sram_blwl_1171_configbus1[1171:1171] , sram_blwl_1171_configbus0_b[1171:1171] );
wire [1172:1172] sram_blwl_1172_configbus0;
wire [1172:1172] sram_blwl_1172_configbus1;
wire [1172:1172] sram_blwl_1172_configbus0_b;
assign sram_blwl_1172_configbus0[1172:1172] = sram_blwl_bl[1172:1172] ;
assign sram_blwl_1172_configbus1[1172:1172] = sram_blwl_wl[1172:1172] ;
assign sram_blwl_1172_configbus0_b[1172:1172] = sram_blwl_blb[1172:1172] ;
sram6T_blwl sram_blwl_1172_ (sram_blwl_out[1172], sram_blwl_out[1172], sram_blwl_outb[1172], sram_blwl_1172_configbus0[1172:1172], sram_blwl_1172_configbus1[1172:1172] , sram_blwl_1172_configbus0_b[1172:1172] );
wire [1173:1173] sram_blwl_1173_configbus0;
wire [1173:1173] sram_blwl_1173_configbus1;
wire [1173:1173] sram_blwl_1173_configbus0_b;
assign sram_blwl_1173_configbus0[1173:1173] = sram_blwl_bl[1173:1173] ;
assign sram_blwl_1173_configbus1[1173:1173] = sram_blwl_wl[1173:1173] ;
assign sram_blwl_1173_configbus0_b[1173:1173] = sram_blwl_blb[1173:1173] ;
sram6T_blwl sram_blwl_1173_ (sram_blwl_out[1173], sram_blwl_out[1173], sram_blwl_outb[1173], sram_blwl_1173_configbus0[1173:1173], sram_blwl_1173_configbus1[1173:1173] , sram_blwl_1173_configbus0_b[1173:1173] );
wire [1174:1174] sram_blwl_1174_configbus0;
wire [1174:1174] sram_blwl_1174_configbus1;
wire [1174:1174] sram_blwl_1174_configbus0_b;
assign sram_blwl_1174_configbus0[1174:1174] = sram_blwl_bl[1174:1174] ;
assign sram_blwl_1174_configbus1[1174:1174] = sram_blwl_wl[1174:1174] ;
assign sram_blwl_1174_configbus0_b[1174:1174] = sram_blwl_blb[1174:1174] ;
sram6T_blwl sram_blwl_1174_ (sram_blwl_out[1174], sram_blwl_out[1174], sram_blwl_outb[1174], sram_blwl_1174_configbus0[1174:1174], sram_blwl_1174_configbus1[1174:1174] , sram_blwl_1174_configbus0_b[1174:1174] );
wire [1175:1175] sram_blwl_1175_configbus0;
wire [1175:1175] sram_blwl_1175_configbus1;
wire [1175:1175] sram_blwl_1175_configbus0_b;
assign sram_blwl_1175_configbus0[1175:1175] = sram_blwl_bl[1175:1175] ;
assign sram_blwl_1175_configbus1[1175:1175] = sram_blwl_wl[1175:1175] ;
assign sram_blwl_1175_configbus0_b[1175:1175] = sram_blwl_blb[1175:1175] ;
sram6T_blwl sram_blwl_1175_ (sram_blwl_out[1175], sram_blwl_out[1175], sram_blwl_outb[1175], sram_blwl_1175_configbus0[1175:1175], sram_blwl_1175_configbus1[1175:1175] , sram_blwl_1175_configbus0_b[1175:1175] );
wire [1176:1176] sram_blwl_1176_configbus0;
wire [1176:1176] sram_blwl_1176_configbus1;
wire [1176:1176] sram_blwl_1176_configbus0_b;
assign sram_blwl_1176_configbus0[1176:1176] = sram_blwl_bl[1176:1176] ;
assign sram_blwl_1176_configbus1[1176:1176] = sram_blwl_wl[1176:1176] ;
assign sram_blwl_1176_configbus0_b[1176:1176] = sram_blwl_blb[1176:1176] ;
sram6T_blwl sram_blwl_1176_ (sram_blwl_out[1176], sram_blwl_out[1176], sram_blwl_outb[1176], sram_blwl_1176_configbus0[1176:1176], sram_blwl_1176_configbus1[1176:1176] , sram_blwl_1176_configbus0_b[1176:1176] );
wire [1177:1177] sram_blwl_1177_configbus0;
wire [1177:1177] sram_blwl_1177_configbus1;
wire [1177:1177] sram_blwl_1177_configbus0_b;
assign sram_blwl_1177_configbus0[1177:1177] = sram_blwl_bl[1177:1177] ;
assign sram_blwl_1177_configbus1[1177:1177] = sram_blwl_wl[1177:1177] ;
assign sram_blwl_1177_configbus0_b[1177:1177] = sram_blwl_blb[1177:1177] ;
sram6T_blwl sram_blwl_1177_ (sram_blwl_out[1177], sram_blwl_out[1177], sram_blwl_outb[1177], sram_blwl_1177_configbus0[1177:1177], sram_blwl_1177_configbus1[1177:1177] , sram_blwl_1177_configbus0_b[1177:1177] );
wire [1178:1178] sram_blwl_1178_configbus0;
wire [1178:1178] sram_blwl_1178_configbus1;
wire [1178:1178] sram_blwl_1178_configbus0_b;
assign sram_blwl_1178_configbus0[1178:1178] = sram_blwl_bl[1178:1178] ;
assign sram_blwl_1178_configbus1[1178:1178] = sram_blwl_wl[1178:1178] ;
assign sram_blwl_1178_configbus0_b[1178:1178] = sram_blwl_blb[1178:1178] ;
sram6T_blwl sram_blwl_1178_ (sram_blwl_out[1178], sram_blwl_out[1178], sram_blwl_outb[1178], sram_blwl_1178_configbus0[1178:1178], sram_blwl_1178_configbus1[1178:1178] , sram_blwl_1178_configbus0_b[1178:1178] );
wire [1179:1179] sram_blwl_1179_configbus0;
wire [1179:1179] sram_blwl_1179_configbus1;
wire [1179:1179] sram_blwl_1179_configbus0_b;
assign sram_blwl_1179_configbus0[1179:1179] = sram_blwl_bl[1179:1179] ;
assign sram_blwl_1179_configbus1[1179:1179] = sram_blwl_wl[1179:1179] ;
assign sram_blwl_1179_configbus0_b[1179:1179] = sram_blwl_blb[1179:1179] ;
sram6T_blwl sram_blwl_1179_ (sram_blwl_out[1179], sram_blwl_out[1179], sram_blwl_outb[1179], sram_blwl_1179_configbus0[1179:1179], sram_blwl_1179_configbus1[1179:1179] , sram_blwl_1179_configbus0_b[1179:1179] );
wire [1180:1180] sram_blwl_1180_configbus0;
wire [1180:1180] sram_blwl_1180_configbus1;
wire [1180:1180] sram_blwl_1180_configbus0_b;
assign sram_blwl_1180_configbus0[1180:1180] = sram_blwl_bl[1180:1180] ;
assign sram_blwl_1180_configbus1[1180:1180] = sram_blwl_wl[1180:1180] ;
assign sram_blwl_1180_configbus0_b[1180:1180] = sram_blwl_blb[1180:1180] ;
sram6T_blwl sram_blwl_1180_ (sram_blwl_out[1180], sram_blwl_out[1180], sram_blwl_outb[1180], sram_blwl_1180_configbus0[1180:1180], sram_blwl_1180_configbus1[1180:1180] , sram_blwl_1180_configbus0_b[1180:1180] );
wire [1181:1181] sram_blwl_1181_configbus0;
wire [1181:1181] sram_blwl_1181_configbus1;
wire [1181:1181] sram_blwl_1181_configbus0_b;
assign sram_blwl_1181_configbus0[1181:1181] = sram_blwl_bl[1181:1181] ;
assign sram_blwl_1181_configbus1[1181:1181] = sram_blwl_wl[1181:1181] ;
assign sram_blwl_1181_configbus0_b[1181:1181] = sram_blwl_blb[1181:1181] ;
sram6T_blwl sram_blwl_1181_ (sram_blwl_out[1181], sram_blwl_out[1181], sram_blwl_outb[1181], sram_blwl_1181_configbus0[1181:1181], sram_blwl_1181_configbus1[1181:1181] , sram_blwl_1181_configbus0_b[1181:1181] );
wire [1182:1182] sram_blwl_1182_configbus0;
wire [1182:1182] sram_blwl_1182_configbus1;
wire [1182:1182] sram_blwl_1182_configbus0_b;
assign sram_blwl_1182_configbus0[1182:1182] = sram_blwl_bl[1182:1182] ;
assign sram_blwl_1182_configbus1[1182:1182] = sram_blwl_wl[1182:1182] ;
assign sram_blwl_1182_configbus0_b[1182:1182] = sram_blwl_blb[1182:1182] ;
sram6T_blwl sram_blwl_1182_ (sram_blwl_out[1182], sram_blwl_out[1182], sram_blwl_outb[1182], sram_blwl_1182_configbus0[1182:1182], sram_blwl_1182_configbus1[1182:1182] , sram_blwl_1182_configbus0_b[1182:1182] );
wire [1183:1183] sram_blwl_1183_configbus0;
wire [1183:1183] sram_blwl_1183_configbus1;
wire [1183:1183] sram_blwl_1183_configbus0_b;
assign sram_blwl_1183_configbus0[1183:1183] = sram_blwl_bl[1183:1183] ;
assign sram_blwl_1183_configbus1[1183:1183] = sram_blwl_wl[1183:1183] ;
assign sram_blwl_1183_configbus0_b[1183:1183] = sram_blwl_blb[1183:1183] ;
sram6T_blwl sram_blwl_1183_ (sram_blwl_out[1183], sram_blwl_out[1183], sram_blwl_outb[1183], sram_blwl_1183_configbus0[1183:1183], sram_blwl_1183_configbus1[1183:1183] , sram_blwl_1183_configbus0_b[1183:1183] );
wire [1184:1184] sram_blwl_1184_configbus0;
wire [1184:1184] sram_blwl_1184_configbus1;
wire [1184:1184] sram_blwl_1184_configbus0_b;
assign sram_blwl_1184_configbus0[1184:1184] = sram_blwl_bl[1184:1184] ;
assign sram_blwl_1184_configbus1[1184:1184] = sram_blwl_wl[1184:1184] ;
assign sram_blwl_1184_configbus0_b[1184:1184] = sram_blwl_blb[1184:1184] ;
sram6T_blwl sram_blwl_1184_ (sram_blwl_out[1184], sram_blwl_out[1184], sram_blwl_outb[1184], sram_blwl_1184_configbus0[1184:1184], sram_blwl_1184_configbus1[1184:1184] , sram_blwl_1184_configbus0_b[1184:1184] );
wire [1185:1185] sram_blwl_1185_configbus0;
wire [1185:1185] sram_blwl_1185_configbus1;
wire [1185:1185] sram_blwl_1185_configbus0_b;
assign sram_blwl_1185_configbus0[1185:1185] = sram_blwl_bl[1185:1185] ;
assign sram_blwl_1185_configbus1[1185:1185] = sram_blwl_wl[1185:1185] ;
assign sram_blwl_1185_configbus0_b[1185:1185] = sram_blwl_blb[1185:1185] ;
sram6T_blwl sram_blwl_1185_ (sram_blwl_out[1185], sram_blwl_out[1185], sram_blwl_outb[1185], sram_blwl_1185_configbus0[1185:1185], sram_blwl_1185_configbus1[1185:1185] , sram_blwl_1185_configbus0_b[1185:1185] );
wire [1186:1186] sram_blwl_1186_configbus0;
wire [1186:1186] sram_blwl_1186_configbus1;
wire [1186:1186] sram_blwl_1186_configbus0_b;
assign sram_blwl_1186_configbus0[1186:1186] = sram_blwl_bl[1186:1186] ;
assign sram_blwl_1186_configbus1[1186:1186] = sram_blwl_wl[1186:1186] ;
assign sram_blwl_1186_configbus0_b[1186:1186] = sram_blwl_blb[1186:1186] ;
sram6T_blwl sram_blwl_1186_ (sram_blwl_out[1186], sram_blwl_out[1186], sram_blwl_outb[1186], sram_blwl_1186_configbus0[1186:1186], sram_blwl_1186_configbus1[1186:1186] , sram_blwl_1186_configbus0_b[1186:1186] );
wire [1187:1187] sram_blwl_1187_configbus0;
wire [1187:1187] sram_blwl_1187_configbus1;
wire [1187:1187] sram_blwl_1187_configbus0_b;
assign sram_blwl_1187_configbus0[1187:1187] = sram_blwl_bl[1187:1187] ;
assign sram_blwl_1187_configbus1[1187:1187] = sram_blwl_wl[1187:1187] ;
assign sram_blwl_1187_configbus0_b[1187:1187] = sram_blwl_blb[1187:1187] ;
sram6T_blwl sram_blwl_1187_ (sram_blwl_out[1187], sram_blwl_out[1187], sram_blwl_outb[1187], sram_blwl_1187_configbus0[1187:1187], sram_blwl_1187_configbus1[1187:1187] , sram_blwl_1187_configbus0_b[1187:1187] );
wire [1188:1188] sram_blwl_1188_configbus0;
wire [1188:1188] sram_blwl_1188_configbus1;
wire [1188:1188] sram_blwl_1188_configbus0_b;
assign sram_blwl_1188_configbus0[1188:1188] = sram_blwl_bl[1188:1188] ;
assign sram_blwl_1188_configbus1[1188:1188] = sram_blwl_wl[1188:1188] ;
assign sram_blwl_1188_configbus0_b[1188:1188] = sram_blwl_blb[1188:1188] ;
sram6T_blwl sram_blwl_1188_ (sram_blwl_out[1188], sram_blwl_out[1188], sram_blwl_outb[1188], sram_blwl_1188_configbus0[1188:1188], sram_blwl_1188_configbus1[1188:1188] , sram_blwl_1188_configbus0_b[1188:1188] );
wire [1189:1189] sram_blwl_1189_configbus0;
wire [1189:1189] sram_blwl_1189_configbus1;
wire [1189:1189] sram_blwl_1189_configbus0_b;
assign sram_blwl_1189_configbus0[1189:1189] = sram_blwl_bl[1189:1189] ;
assign sram_blwl_1189_configbus1[1189:1189] = sram_blwl_wl[1189:1189] ;
assign sram_blwl_1189_configbus0_b[1189:1189] = sram_blwl_blb[1189:1189] ;
sram6T_blwl sram_blwl_1189_ (sram_blwl_out[1189], sram_blwl_out[1189], sram_blwl_outb[1189], sram_blwl_1189_configbus0[1189:1189], sram_blwl_1189_configbus1[1189:1189] , sram_blwl_1189_configbus0_b[1189:1189] );
wire [1190:1190] sram_blwl_1190_configbus0;
wire [1190:1190] sram_blwl_1190_configbus1;
wire [1190:1190] sram_blwl_1190_configbus0_b;
assign sram_blwl_1190_configbus0[1190:1190] = sram_blwl_bl[1190:1190] ;
assign sram_blwl_1190_configbus1[1190:1190] = sram_blwl_wl[1190:1190] ;
assign sram_blwl_1190_configbus0_b[1190:1190] = sram_blwl_blb[1190:1190] ;
sram6T_blwl sram_blwl_1190_ (sram_blwl_out[1190], sram_blwl_out[1190], sram_blwl_outb[1190], sram_blwl_1190_configbus0[1190:1190], sram_blwl_1190_configbus1[1190:1190] , sram_blwl_1190_configbus0_b[1190:1190] );
wire [1191:1191] sram_blwl_1191_configbus0;
wire [1191:1191] sram_blwl_1191_configbus1;
wire [1191:1191] sram_blwl_1191_configbus0_b;
assign sram_blwl_1191_configbus0[1191:1191] = sram_blwl_bl[1191:1191] ;
assign sram_blwl_1191_configbus1[1191:1191] = sram_blwl_wl[1191:1191] ;
assign sram_blwl_1191_configbus0_b[1191:1191] = sram_blwl_blb[1191:1191] ;
sram6T_blwl sram_blwl_1191_ (sram_blwl_out[1191], sram_blwl_out[1191], sram_blwl_outb[1191], sram_blwl_1191_configbus0[1191:1191], sram_blwl_1191_configbus1[1191:1191] , sram_blwl_1191_configbus0_b[1191:1191] );
wire [1192:1192] sram_blwl_1192_configbus0;
wire [1192:1192] sram_blwl_1192_configbus1;
wire [1192:1192] sram_blwl_1192_configbus0_b;
assign sram_blwl_1192_configbus0[1192:1192] = sram_blwl_bl[1192:1192] ;
assign sram_blwl_1192_configbus1[1192:1192] = sram_blwl_wl[1192:1192] ;
assign sram_blwl_1192_configbus0_b[1192:1192] = sram_blwl_blb[1192:1192] ;
sram6T_blwl sram_blwl_1192_ (sram_blwl_out[1192], sram_blwl_out[1192], sram_blwl_outb[1192], sram_blwl_1192_configbus0[1192:1192], sram_blwl_1192_configbus1[1192:1192] , sram_blwl_1192_configbus0_b[1192:1192] );
wire [1193:1193] sram_blwl_1193_configbus0;
wire [1193:1193] sram_blwl_1193_configbus1;
wire [1193:1193] sram_blwl_1193_configbus0_b;
assign sram_blwl_1193_configbus0[1193:1193] = sram_blwl_bl[1193:1193] ;
assign sram_blwl_1193_configbus1[1193:1193] = sram_blwl_wl[1193:1193] ;
assign sram_blwl_1193_configbus0_b[1193:1193] = sram_blwl_blb[1193:1193] ;
sram6T_blwl sram_blwl_1193_ (sram_blwl_out[1193], sram_blwl_out[1193], sram_blwl_outb[1193], sram_blwl_1193_configbus0[1193:1193], sram_blwl_1193_configbus1[1193:1193] , sram_blwl_1193_configbus0_b[1193:1193] );
wire [1194:1194] sram_blwl_1194_configbus0;
wire [1194:1194] sram_blwl_1194_configbus1;
wire [1194:1194] sram_blwl_1194_configbus0_b;
assign sram_blwl_1194_configbus0[1194:1194] = sram_blwl_bl[1194:1194] ;
assign sram_blwl_1194_configbus1[1194:1194] = sram_blwl_wl[1194:1194] ;
assign sram_blwl_1194_configbus0_b[1194:1194] = sram_blwl_blb[1194:1194] ;
sram6T_blwl sram_blwl_1194_ (sram_blwl_out[1194], sram_blwl_out[1194], sram_blwl_outb[1194], sram_blwl_1194_configbus0[1194:1194], sram_blwl_1194_configbus1[1194:1194] , sram_blwl_1194_configbus0_b[1194:1194] );
wire [1195:1195] sram_blwl_1195_configbus0;
wire [1195:1195] sram_blwl_1195_configbus1;
wire [1195:1195] sram_blwl_1195_configbus0_b;
assign sram_blwl_1195_configbus0[1195:1195] = sram_blwl_bl[1195:1195] ;
assign sram_blwl_1195_configbus1[1195:1195] = sram_blwl_wl[1195:1195] ;
assign sram_blwl_1195_configbus0_b[1195:1195] = sram_blwl_blb[1195:1195] ;
sram6T_blwl sram_blwl_1195_ (sram_blwl_out[1195], sram_blwl_out[1195], sram_blwl_outb[1195], sram_blwl_1195_configbus0[1195:1195], sram_blwl_1195_configbus1[1195:1195] , sram_blwl_1195_configbus0_b[1195:1195] );
wire [1196:1196] sram_blwl_1196_configbus0;
wire [1196:1196] sram_blwl_1196_configbus1;
wire [1196:1196] sram_blwl_1196_configbus0_b;
assign sram_blwl_1196_configbus0[1196:1196] = sram_blwl_bl[1196:1196] ;
assign sram_blwl_1196_configbus1[1196:1196] = sram_blwl_wl[1196:1196] ;
assign sram_blwl_1196_configbus0_b[1196:1196] = sram_blwl_blb[1196:1196] ;
sram6T_blwl sram_blwl_1196_ (sram_blwl_out[1196], sram_blwl_out[1196], sram_blwl_outb[1196], sram_blwl_1196_configbus0[1196:1196], sram_blwl_1196_configbus1[1196:1196] , sram_blwl_1196_configbus0_b[1196:1196] );
wire [1197:1197] sram_blwl_1197_configbus0;
wire [1197:1197] sram_blwl_1197_configbus1;
wire [1197:1197] sram_blwl_1197_configbus0_b;
assign sram_blwl_1197_configbus0[1197:1197] = sram_blwl_bl[1197:1197] ;
assign sram_blwl_1197_configbus1[1197:1197] = sram_blwl_wl[1197:1197] ;
assign sram_blwl_1197_configbus0_b[1197:1197] = sram_blwl_blb[1197:1197] ;
sram6T_blwl sram_blwl_1197_ (sram_blwl_out[1197], sram_blwl_out[1197], sram_blwl_outb[1197], sram_blwl_1197_configbus0[1197:1197], sram_blwl_1197_configbus1[1197:1197] , sram_blwl_1197_configbus0_b[1197:1197] );
wire [1198:1198] sram_blwl_1198_configbus0;
wire [1198:1198] sram_blwl_1198_configbus1;
wire [1198:1198] sram_blwl_1198_configbus0_b;
assign sram_blwl_1198_configbus0[1198:1198] = sram_blwl_bl[1198:1198] ;
assign sram_blwl_1198_configbus1[1198:1198] = sram_blwl_wl[1198:1198] ;
assign sram_blwl_1198_configbus0_b[1198:1198] = sram_blwl_blb[1198:1198] ;
sram6T_blwl sram_blwl_1198_ (sram_blwl_out[1198], sram_blwl_out[1198], sram_blwl_outb[1198], sram_blwl_1198_configbus0[1198:1198], sram_blwl_1198_configbus1[1198:1198] , sram_blwl_1198_configbus0_b[1198:1198] );
wire [1199:1199] sram_blwl_1199_configbus0;
wire [1199:1199] sram_blwl_1199_configbus1;
wire [1199:1199] sram_blwl_1199_configbus0_b;
assign sram_blwl_1199_configbus0[1199:1199] = sram_blwl_bl[1199:1199] ;
assign sram_blwl_1199_configbus1[1199:1199] = sram_blwl_wl[1199:1199] ;
assign sram_blwl_1199_configbus0_b[1199:1199] = sram_blwl_blb[1199:1199] ;
sram6T_blwl sram_blwl_1199_ (sram_blwl_out[1199], sram_blwl_out[1199], sram_blwl_outb[1199], sram_blwl_1199_configbus0[1199:1199], sram_blwl_1199_configbus1[1199:1199] , sram_blwl_1199_configbus0_b[1199:1199] );
wire [1200:1200] sram_blwl_1200_configbus0;
wire [1200:1200] sram_blwl_1200_configbus1;
wire [1200:1200] sram_blwl_1200_configbus0_b;
assign sram_blwl_1200_configbus0[1200:1200] = sram_blwl_bl[1200:1200] ;
assign sram_blwl_1200_configbus1[1200:1200] = sram_blwl_wl[1200:1200] ;
assign sram_blwl_1200_configbus0_b[1200:1200] = sram_blwl_blb[1200:1200] ;
sram6T_blwl sram_blwl_1200_ (sram_blwl_out[1200], sram_blwl_out[1200], sram_blwl_outb[1200], sram_blwl_1200_configbus0[1200:1200], sram_blwl_1200_configbus1[1200:1200] , sram_blwl_1200_configbus0_b[1200:1200] );
wire [1201:1201] sram_blwl_1201_configbus0;
wire [1201:1201] sram_blwl_1201_configbus1;
wire [1201:1201] sram_blwl_1201_configbus0_b;
assign sram_blwl_1201_configbus0[1201:1201] = sram_blwl_bl[1201:1201] ;
assign sram_blwl_1201_configbus1[1201:1201] = sram_blwl_wl[1201:1201] ;
assign sram_blwl_1201_configbus0_b[1201:1201] = sram_blwl_blb[1201:1201] ;
sram6T_blwl sram_blwl_1201_ (sram_blwl_out[1201], sram_blwl_out[1201], sram_blwl_outb[1201], sram_blwl_1201_configbus0[1201:1201], sram_blwl_1201_configbus1[1201:1201] , sram_blwl_1201_configbus0_b[1201:1201] );
wire [1202:1202] sram_blwl_1202_configbus0;
wire [1202:1202] sram_blwl_1202_configbus1;
wire [1202:1202] sram_blwl_1202_configbus0_b;
assign sram_blwl_1202_configbus0[1202:1202] = sram_blwl_bl[1202:1202] ;
assign sram_blwl_1202_configbus1[1202:1202] = sram_blwl_wl[1202:1202] ;
assign sram_blwl_1202_configbus0_b[1202:1202] = sram_blwl_blb[1202:1202] ;
sram6T_blwl sram_blwl_1202_ (sram_blwl_out[1202], sram_blwl_out[1202], sram_blwl_outb[1202], sram_blwl_1202_configbus0[1202:1202], sram_blwl_1202_configbus1[1202:1202] , sram_blwl_1202_configbus0_b[1202:1202] );
wire [1203:1203] sram_blwl_1203_configbus0;
wire [1203:1203] sram_blwl_1203_configbus1;
wire [1203:1203] sram_blwl_1203_configbus0_b;
assign sram_blwl_1203_configbus0[1203:1203] = sram_blwl_bl[1203:1203] ;
assign sram_blwl_1203_configbus1[1203:1203] = sram_blwl_wl[1203:1203] ;
assign sram_blwl_1203_configbus0_b[1203:1203] = sram_blwl_blb[1203:1203] ;
sram6T_blwl sram_blwl_1203_ (sram_blwl_out[1203], sram_blwl_out[1203], sram_blwl_outb[1203], sram_blwl_1203_configbus0[1203:1203], sram_blwl_1203_configbus1[1203:1203] , sram_blwl_1203_configbus0_b[1203:1203] );
wire [1204:1204] sram_blwl_1204_configbus0;
wire [1204:1204] sram_blwl_1204_configbus1;
wire [1204:1204] sram_blwl_1204_configbus0_b;
assign sram_blwl_1204_configbus0[1204:1204] = sram_blwl_bl[1204:1204] ;
assign sram_blwl_1204_configbus1[1204:1204] = sram_blwl_wl[1204:1204] ;
assign sram_blwl_1204_configbus0_b[1204:1204] = sram_blwl_blb[1204:1204] ;
sram6T_blwl sram_blwl_1204_ (sram_blwl_out[1204], sram_blwl_out[1204], sram_blwl_outb[1204], sram_blwl_1204_configbus0[1204:1204], sram_blwl_1204_configbus1[1204:1204] , sram_blwl_1204_configbus0_b[1204:1204] );
wire [1205:1205] sram_blwl_1205_configbus0;
wire [1205:1205] sram_blwl_1205_configbus1;
wire [1205:1205] sram_blwl_1205_configbus0_b;
assign sram_blwl_1205_configbus0[1205:1205] = sram_blwl_bl[1205:1205] ;
assign sram_blwl_1205_configbus1[1205:1205] = sram_blwl_wl[1205:1205] ;
assign sram_blwl_1205_configbus0_b[1205:1205] = sram_blwl_blb[1205:1205] ;
sram6T_blwl sram_blwl_1205_ (sram_blwl_out[1205], sram_blwl_out[1205], sram_blwl_outb[1205], sram_blwl_1205_configbus0[1205:1205], sram_blwl_1205_configbus1[1205:1205] , sram_blwl_1205_configbus0_b[1205:1205] );
wire [1206:1206] sram_blwl_1206_configbus0;
wire [1206:1206] sram_blwl_1206_configbus1;
wire [1206:1206] sram_blwl_1206_configbus0_b;
assign sram_blwl_1206_configbus0[1206:1206] = sram_blwl_bl[1206:1206] ;
assign sram_blwl_1206_configbus1[1206:1206] = sram_blwl_wl[1206:1206] ;
assign sram_blwl_1206_configbus0_b[1206:1206] = sram_blwl_blb[1206:1206] ;
sram6T_blwl sram_blwl_1206_ (sram_blwl_out[1206], sram_blwl_out[1206], sram_blwl_outb[1206], sram_blwl_1206_configbus0[1206:1206], sram_blwl_1206_configbus1[1206:1206] , sram_blwl_1206_configbus0_b[1206:1206] );
wire [1207:1207] sram_blwl_1207_configbus0;
wire [1207:1207] sram_blwl_1207_configbus1;
wire [1207:1207] sram_blwl_1207_configbus0_b;
assign sram_blwl_1207_configbus0[1207:1207] = sram_blwl_bl[1207:1207] ;
assign sram_blwl_1207_configbus1[1207:1207] = sram_blwl_wl[1207:1207] ;
assign sram_blwl_1207_configbus0_b[1207:1207] = sram_blwl_blb[1207:1207] ;
sram6T_blwl sram_blwl_1207_ (sram_blwl_out[1207], sram_blwl_out[1207], sram_blwl_outb[1207], sram_blwl_1207_configbus0[1207:1207], sram_blwl_1207_configbus1[1207:1207] , sram_blwl_1207_configbus0_b[1207:1207] );
wire [1208:1208] sram_blwl_1208_configbus0;
wire [1208:1208] sram_blwl_1208_configbus1;
wire [1208:1208] sram_blwl_1208_configbus0_b;
assign sram_blwl_1208_configbus0[1208:1208] = sram_blwl_bl[1208:1208] ;
assign sram_blwl_1208_configbus1[1208:1208] = sram_blwl_wl[1208:1208] ;
assign sram_blwl_1208_configbus0_b[1208:1208] = sram_blwl_blb[1208:1208] ;
sram6T_blwl sram_blwl_1208_ (sram_blwl_out[1208], sram_blwl_out[1208], sram_blwl_outb[1208], sram_blwl_1208_configbus0[1208:1208], sram_blwl_1208_configbus1[1208:1208] , sram_blwl_1208_configbus0_b[1208:1208] );
wire [1209:1209] sram_blwl_1209_configbus0;
wire [1209:1209] sram_blwl_1209_configbus1;
wire [1209:1209] sram_blwl_1209_configbus0_b;
assign sram_blwl_1209_configbus0[1209:1209] = sram_blwl_bl[1209:1209] ;
assign sram_blwl_1209_configbus1[1209:1209] = sram_blwl_wl[1209:1209] ;
assign sram_blwl_1209_configbus0_b[1209:1209] = sram_blwl_blb[1209:1209] ;
sram6T_blwl sram_blwl_1209_ (sram_blwl_out[1209], sram_blwl_out[1209], sram_blwl_outb[1209], sram_blwl_1209_configbus0[1209:1209], sram_blwl_1209_configbus1[1209:1209] , sram_blwl_1209_configbus0_b[1209:1209] );
endmodule
//----- END LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----

//----- Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----
module grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
input [0:0] Set,
input [0:0] Reset,
input [0:0] clk
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
input wire ff_0___D_0_,
output wire ff_0___Q_0_);
static_dff dff_2_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
endmodule
//----- END Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6_ -----
module grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_ble6___in_0_,
input wire mode_ble6___in_1_,
input wire mode_ble6___in_2_,
input wire mode_ble6___in_3_,
input wire mode_ble6___in_4_,
input wire mode_ble6___in_5_,
output wire mode_ble6___out_0_,
input wire mode_ble6___clk_0_,
input [1146:1210] sram_blwl_bl ,
input [1146:1210] sram_blwl_wl ,
input [1146:1210] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ lut6_0_ (
 lut6_0___in_0_,  lut6_0___in_1_,  lut6_0___in_2_,  lut6_0___in_3_,  lut6_0___in_4_,  lut6_0___in_5_,  lut6_0___out_0_,
sram_blwl_bl[1146:1209] ,
sram_blwl_wl[1146:1209] ,
sram_blwl_blb[1146:1209] );
grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ ff_0_ (
//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
wire [0:1] in_bus_mux_1level_tapbuf_size2_402_ ;
assign in_bus_mux_1level_tapbuf_size2_402_[0] = ff_0___Q_0_ ; 
assign in_bus_mux_1level_tapbuf_size2_402_[1] = lut6_0___out_0_ ; 
wire [1210:1210] mux_1level_tapbuf_size2_402_configbus0;
wire [1210:1210] mux_1level_tapbuf_size2_402_configbus1;
wire [1210:1210] mux_1level_tapbuf_size2_402_sram_blwl_out ;
wire [1210:1210] mux_1level_tapbuf_size2_402_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_402_configbus0[1210:1210] = sram_blwl_bl[1210:1210] ;
assign mux_1level_tapbuf_size2_402_configbus1[1210:1210] = sram_blwl_wl[1210:1210] ;
wire [1210:1210] mux_1level_tapbuf_size2_402_configbus0_b;
assign mux_1level_tapbuf_size2_402_configbus0_b[1210:1210] = sram_blwl_blb[1210:1210] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_402_ (in_bus_mux_1level_tapbuf_size2_402_, mode_ble6___out_0_, mux_1level_tapbuf_size2_402_sram_blwl_out[1210:1210] ,
mux_1level_tapbuf_size2_402_sram_blwl_outb[1210:1210] );
//----- SRAM bits for MUX[402], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_1210_ (mux_1level_tapbuf_size2_402_sram_blwl_out[1210:1210] ,mux_1level_tapbuf_size2_402_sram_blwl_out[1210:1210] ,mux_1level_tapbuf_size2_402_sram_blwl_outb[1210:1210] ,mux_1level_tapbuf_size2_402_configbus0[1210:1210], mux_1level_tapbuf_size2_402_configbus1[1210:1210] , mux_1level_tapbuf_size2_402_configbus0_b[1210:1210] );
direct_interc direct_interc_32_ (mode_ble6___in_0_, lut6_0___in_0_ );
direct_interc direct_interc_33_ (mode_ble6___in_1_, lut6_0___in_1_ );
direct_interc direct_interc_34_ (mode_ble6___in_2_, lut6_0___in_2_ );
direct_interc direct_interc_35_ (mode_ble6___in_3_, lut6_0___in_3_ );
direct_interc direct_interc_36_ (mode_ble6___in_4_, lut6_0___in_4_ );
direct_interc direct_interc_37_ (mode_ble6___in_5_, lut6_0___in_5_ );
direct_interc direct_interc_38_ (lut6_0___out_0_, ff_0___D_0_ );
direct_interc direct_interc_39_ (mode_ble6___clk_0_, ff_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6_ -----
module grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_n1_lut6___in_0_,
input wire mode_n1_lut6___in_1_,
input wire mode_n1_lut6___in_2_,
input wire mode_n1_lut6___in_3_,
input wire mode_n1_lut6___in_4_,
input wire mode_n1_lut6___in_5_,
output wire mode_n1_lut6___out_0_,
input wire mode_n1_lut6___clk_0_,
input [1146:1210] sram_blwl_bl ,
input [1146:1210] sram_blwl_wl ,
input [1146:1210] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6__ble6_0__mode_ble6_ ble6_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 ble6_0___in_0_,  ble6_0___in_1_,  ble6_0___in_2_,  ble6_0___in_3_,  ble6_0___in_4_,  ble6_0___in_5_,  ble6_0___out_0_,  ble6_0___clk_0_,
sram_blwl_bl[1146:1210] ,
sram_blwl_wl[1146:1210] ,
sram_blwl_blb[1146:1210] );
direct_interc direct_interc_40_ (ble6_0___out_0_, mode_n1_lut6___out_0_ );
direct_interc direct_interc_41_ (mode_n1_lut6___in_0_, ble6_0___in_0_ );
direct_interc direct_interc_42_ (mode_n1_lut6___in_1_, ble6_0___in_1_ );
direct_interc direct_interc_43_ (mode_n1_lut6___in_2_, ble6_0___in_2_ );
direct_interc direct_interc_44_ (mode_n1_lut6___in_3_, ble6_0___in_3_ );
direct_interc direct_interc_45_ (mode_n1_lut6___in_4_, ble6_0___in_4_ );
direct_interc direct_interc_46_ (mode_n1_lut6___in_5_, ble6_0___in_5_ );
direct_interc direct_interc_47_ (mode_n1_lut6___clk_0_, ble6_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6_ -----

//----- LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----
module grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ (
input wire lut6_0___in_0_,
input wire lut6_0___in_1_,
input wire lut6_0___in_2_,
input wire lut6_0___in_3_,
input wire lut6_0___in_4_,
input wire lut6_0___in_5_,
output wire lut6_0___out_0_,
input [1211:1274] sram_blwl_bl ,
input [1211:1274] sram_blwl_wl ,
input [1211:1274] sram_blwl_blb );
wire [0:5] lut6_0___in;
assign lut6_0___in[0] = lut6_0___in_0_;
assign lut6_0___in[1] = lut6_0___in_1_;
assign lut6_0___in[2] = lut6_0___in_2_;
assign lut6_0___in[3] = lut6_0___in_3_;
assign lut6_0___in[4] = lut6_0___in_4_;
assign lut6_0___in[5] = lut6_0___in_5_;
wire [0:0] lut6_0___out;
assign lut6_0___out_0_ = lut6_0___out[0];
wire [1211:1274] sram_blwl_out ;
wire [1211:1274] sram_blwl_outb ;
lut6 lut6_3_ (
//----- Input and output ports -----
 lut6_0___in[0:5] ,  lut6_0___out[0:0],//----- SRAM ports -----
sram_blwl_out[1211:1274] , sram_blwl_outb[1211:1274] );
//----- Truth Table for LUT[3], size=6. -----
//----- SRAM bits for LUT[3], size=6, num_sram=64. -----
//-----0000000000000000000000000000000000000000000000000000000000000000-----
wire [1211:1211] sram_blwl_1211_configbus0;
wire [1211:1211] sram_blwl_1211_configbus1;
wire [1211:1211] sram_blwl_1211_configbus0_b;
assign sram_blwl_1211_configbus0[1211:1211] = sram_blwl_bl[1211:1211] ;
assign sram_blwl_1211_configbus1[1211:1211] = sram_blwl_wl[1211:1211] ;
assign sram_blwl_1211_configbus0_b[1211:1211] = sram_blwl_blb[1211:1211] ;
sram6T_blwl sram_blwl_1211_ (sram_blwl_out[1211], sram_blwl_out[1211], sram_blwl_outb[1211], sram_blwl_1211_configbus0[1211:1211], sram_blwl_1211_configbus1[1211:1211] , sram_blwl_1211_configbus0_b[1211:1211] );
wire [1212:1212] sram_blwl_1212_configbus0;
wire [1212:1212] sram_blwl_1212_configbus1;
wire [1212:1212] sram_blwl_1212_configbus0_b;
assign sram_blwl_1212_configbus0[1212:1212] = sram_blwl_bl[1212:1212] ;
assign sram_blwl_1212_configbus1[1212:1212] = sram_blwl_wl[1212:1212] ;
assign sram_blwl_1212_configbus0_b[1212:1212] = sram_blwl_blb[1212:1212] ;
sram6T_blwl sram_blwl_1212_ (sram_blwl_out[1212], sram_blwl_out[1212], sram_blwl_outb[1212], sram_blwl_1212_configbus0[1212:1212], sram_blwl_1212_configbus1[1212:1212] , sram_blwl_1212_configbus0_b[1212:1212] );
wire [1213:1213] sram_blwl_1213_configbus0;
wire [1213:1213] sram_blwl_1213_configbus1;
wire [1213:1213] sram_blwl_1213_configbus0_b;
assign sram_blwl_1213_configbus0[1213:1213] = sram_blwl_bl[1213:1213] ;
assign sram_blwl_1213_configbus1[1213:1213] = sram_blwl_wl[1213:1213] ;
assign sram_blwl_1213_configbus0_b[1213:1213] = sram_blwl_blb[1213:1213] ;
sram6T_blwl sram_blwl_1213_ (sram_blwl_out[1213], sram_blwl_out[1213], sram_blwl_outb[1213], sram_blwl_1213_configbus0[1213:1213], sram_blwl_1213_configbus1[1213:1213] , sram_blwl_1213_configbus0_b[1213:1213] );
wire [1214:1214] sram_blwl_1214_configbus0;
wire [1214:1214] sram_blwl_1214_configbus1;
wire [1214:1214] sram_blwl_1214_configbus0_b;
assign sram_blwl_1214_configbus0[1214:1214] = sram_blwl_bl[1214:1214] ;
assign sram_blwl_1214_configbus1[1214:1214] = sram_blwl_wl[1214:1214] ;
assign sram_blwl_1214_configbus0_b[1214:1214] = sram_blwl_blb[1214:1214] ;
sram6T_blwl sram_blwl_1214_ (sram_blwl_out[1214], sram_blwl_out[1214], sram_blwl_outb[1214], sram_blwl_1214_configbus0[1214:1214], sram_blwl_1214_configbus1[1214:1214] , sram_blwl_1214_configbus0_b[1214:1214] );
wire [1215:1215] sram_blwl_1215_configbus0;
wire [1215:1215] sram_blwl_1215_configbus1;
wire [1215:1215] sram_blwl_1215_configbus0_b;
assign sram_blwl_1215_configbus0[1215:1215] = sram_blwl_bl[1215:1215] ;
assign sram_blwl_1215_configbus1[1215:1215] = sram_blwl_wl[1215:1215] ;
assign sram_blwl_1215_configbus0_b[1215:1215] = sram_blwl_blb[1215:1215] ;
sram6T_blwl sram_blwl_1215_ (sram_blwl_out[1215], sram_blwl_out[1215], sram_blwl_outb[1215], sram_blwl_1215_configbus0[1215:1215], sram_blwl_1215_configbus1[1215:1215] , sram_blwl_1215_configbus0_b[1215:1215] );
wire [1216:1216] sram_blwl_1216_configbus0;
wire [1216:1216] sram_blwl_1216_configbus1;
wire [1216:1216] sram_blwl_1216_configbus0_b;
assign sram_blwl_1216_configbus0[1216:1216] = sram_blwl_bl[1216:1216] ;
assign sram_blwl_1216_configbus1[1216:1216] = sram_blwl_wl[1216:1216] ;
assign sram_blwl_1216_configbus0_b[1216:1216] = sram_blwl_blb[1216:1216] ;
sram6T_blwl sram_blwl_1216_ (sram_blwl_out[1216], sram_blwl_out[1216], sram_blwl_outb[1216], sram_blwl_1216_configbus0[1216:1216], sram_blwl_1216_configbus1[1216:1216] , sram_blwl_1216_configbus0_b[1216:1216] );
wire [1217:1217] sram_blwl_1217_configbus0;
wire [1217:1217] sram_blwl_1217_configbus1;
wire [1217:1217] sram_blwl_1217_configbus0_b;
assign sram_blwl_1217_configbus0[1217:1217] = sram_blwl_bl[1217:1217] ;
assign sram_blwl_1217_configbus1[1217:1217] = sram_blwl_wl[1217:1217] ;
assign sram_blwl_1217_configbus0_b[1217:1217] = sram_blwl_blb[1217:1217] ;
sram6T_blwl sram_blwl_1217_ (sram_blwl_out[1217], sram_blwl_out[1217], sram_blwl_outb[1217], sram_blwl_1217_configbus0[1217:1217], sram_blwl_1217_configbus1[1217:1217] , sram_blwl_1217_configbus0_b[1217:1217] );
wire [1218:1218] sram_blwl_1218_configbus0;
wire [1218:1218] sram_blwl_1218_configbus1;
wire [1218:1218] sram_blwl_1218_configbus0_b;
assign sram_blwl_1218_configbus0[1218:1218] = sram_blwl_bl[1218:1218] ;
assign sram_blwl_1218_configbus1[1218:1218] = sram_blwl_wl[1218:1218] ;
assign sram_blwl_1218_configbus0_b[1218:1218] = sram_blwl_blb[1218:1218] ;
sram6T_blwl sram_blwl_1218_ (sram_blwl_out[1218], sram_blwl_out[1218], sram_blwl_outb[1218], sram_blwl_1218_configbus0[1218:1218], sram_blwl_1218_configbus1[1218:1218] , sram_blwl_1218_configbus0_b[1218:1218] );
wire [1219:1219] sram_blwl_1219_configbus0;
wire [1219:1219] sram_blwl_1219_configbus1;
wire [1219:1219] sram_blwl_1219_configbus0_b;
assign sram_blwl_1219_configbus0[1219:1219] = sram_blwl_bl[1219:1219] ;
assign sram_blwl_1219_configbus1[1219:1219] = sram_blwl_wl[1219:1219] ;
assign sram_blwl_1219_configbus0_b[1219:1219] = sram_blwl_blb[1219:1219] ;
sram6T_blwl sram_blwl_1219_ (sram_blwl_out[1219], sram_blwl_out[1219], sram_blwl_outb[1219], sram_blwl_1219_configbus0[1219:1219], sram_blwl_1219_configbus1[1219:1219] , sram_blwl_1219_configbus0_b[1219:1219] );
wire [1220:1220] sram_blwl_1220_configbus0;
wire [1220:1220] sram_blwl_1220_configbus1;
wire [1220:1220] sram_blwl_1220_configbus0_b;
assign sram_blwl_1220_configbus0[1220:1220] = sram_blwl_bl[1220:1220] ;
assign sram_blwl_1220_configbus1[1220:1220] = sram_blwl_wl[1220:1220] ;
assign sram_blwl_1220_configbus0_b[1220:1220] = sram_blwl_blb[1220:1220] ;
sram6T_blwl sram_blwl_1220_ (sram_blwl_out[1220], sram_blwl_out[1220], sram_blwl_outb[1220], sram_blwl_1220_configbus0[1220:1220], sram_blwl_1220_configbus1[1220:1220] , sram_blwl_1220_configbus0_b[1220:1220] );
wire [1221:1221] sram_blwl_1221_configbus0;
wire [1221:1221] sram_blwl_1221_configbus1;
wire [1221:1221] sram_blwl_1221_configbus0_b;
assign sram_blwl_1221_configbus0[1221:1221] = sram_blwl_bl[1221:1221] ;
assign sram_blwl_1221_configbus1[1221:1221] = sram_blwl_wl[1221:1221] ;
assign sram_blwl_1221_configbus0_b[1221:1221] = sram_blwl_blb[1221:1221] ;
sram6T_blwl sram_blwl_1221_ (sram_blwl_out[1221], sram_blwl_out[1221], sram_blwl_outb[1221], sram_blwl_1221_configbus0[1221:1221], sram_blwl_1221_configbus1[1221:1221] , sram_blwl_1221_configbus0_b[1221:1221] );
wire [1222:1222] sram_blwl_1222_configbus0;
wire [1222:1222] sram_blwl_1222_configbus1;
wire [1222:1222] sram_blwl_1222_configbus0_b;
assign sram_blwl_1222_configbus0[1222:1222] = sram_blwl_bl[1222:1222] ;
assign sram_blwl_1222_configbus1[1222:1222] = sram_blwl_wl[1222:1222] ;
assign sram_blwl_1222_configbus0_b[1222:1222] = sram_blwl_blb[1222:1222] ;
sram6T_blwl sram_blwl_1222_ (sram_blwl_out[1222], sram_blwl_out[1222], sram_blwl_outb[1222], sram_blwl_1222_configbus0[1222:1222], sram_blwl_1222_configbus1[1222:1222] , sram_blwl_1222_configbus0_b[1222:1222] );
wire [1223:1223] sram_blwl_1223_configbus0;
wire [1223:1223] sram_blwl_1223_configbus1;
wire [1223:1223] sram_blwl_1223_configbus0_b;
assign sram_blwl_1223_configbus0[1223:1223] = sram_blwl_bl[1223:1223] ;
assign sram_blwl_1223_configbus1[1223:1223] = sram_blwl_wl[1223:1223] ;
assign sram_blwl_1223_configbus0_b[1223:1223] = sram_blwl_blb[1223:1223] ;
sram6T_blwl sram_blwl_1223_ (sram_blwl_out[1223], sram_blwl_out[1223], sram_blwl_outb[1223], sram_blwl_1223_configbus0[1223:1223], sram_blwl_1223_configbus1[1223:1223] , sram_blwl_1223_configbus0_b[1223:1223] );
wire [1224:1224] sram_blwl_1224_configbus0;
wire [1224:1224] sram_blwl_1224_configbus1;
wire [1224:1224] sram_blwl_1224_configbus0_b;
assign sram_blwl_1224_configbus0[1224:1224] = sram_blwl_bl[1224:1224] ;
assign sram_blwl_1224_configbus1[1224:1224] = sram_blwl_wl[1224:1224] ;
assign sram_blwl_1224_configbus0_b[1224:1224] = sram_blwl_blb[1224:1224] ;
sram6T_blwl sram_blwl_1224_ (sram_blwl_out[1224], sram_blwl_out[1224], sram_blwl_outb[1224], sram_blwl_1224_configbus0[1224:1224], sram_blwl_1224_configbus1[1224:1224] , sram_blwl_1224_configbus0_b[1224:1224] );
wire [1225:1225] sram_blwl_1225_configbus0;
wire [1225:1225] sram_blwl_1225_configbus1;
wire [1225:1225] sram_blwl_1225_configbus0_b;
assign sram_blwl_1225_configbus0[1225:1225] = sram_blwl_bl[1225:1225] ;
assign sram_blwl_1225_configbus1[1225:1225] = sram_blwl_wl[1225:1225] ;
assign sram_blwl_1225_configbus0_b[1225:1225] = sram_blwl_blb[1225:1225] ;
sram6T_blwl sram_blwl_1225_ (sram_blwl_out[1225], sram_blwl_out[1225], sram_blwl_outb[1225], sram_blwl_1225_configbus0[1225:1225], sram_blwl_1225_configbus1[1225:1225] , sram_blwl_1225_configbus0_b[1225:1225] );
wire [1226:1226] sram_blwl_1226_configbus0;
wire [1226:1226] sram_blwl_1226_configbus1;
wire [1226:1226] sram_blwl_1226_configbus0_b;
assign sram_blwl_1226_configbus0[1226:1226] = sram_blwl_bl[1226:1226] ;
assign sram_blwl_1226_configbus1[1226:1226] = sram_blwl_wl[1226:1226] ;
assign sram_blwl_1226_configbus0_b[1226:1226] = sram_blwl_blb[1226:1226] ;
sram6T_blwl sram_blwl_1226_ (sram_blwl_out[1226], sram_blwl_out[1226], sram_blwl_outb[1226], sram_blwl_1226_configbus0[1226:1226], sram_blwl_1226_configbus1[1226:1226] , sram_blwl_1226_configbus0_b[1226:1226] );
wire [1227:1227] sram_blwl_1227_configbus0;
wire [1227:1227] sram_blwl_1227_configbus1;
wire [1227:1227] sram_blwl_1227_configbus0_b;
assign sram_blwl_1227_configbus0[1227:1227] = sram_blwl_bl[1227:1227] ;
assign sram_blwl_1227_configbus1[1227:1227] = sram_blwl_wl[1227:1227] ;
assign sram_blwl_1227_configbus0_b[1227:1227] = sram_blwl_blb[1227:1227] ;
sram6T_blwl sram_blwl_1227_ (sram_blwl_out[1227], sram_blwl_out[1227], sram_blwl_outb[1227], sram_blwl_1227_configbus0[1227:1227], sram_blwl_1227_configbus1[1227:1227] , sram_blwl_1227_configbus0_b[1227:1227] );
wire [1228:1228] sram_blwl_1228_configbus0;
wire [1228:1228] sram_blwl_1228_configbus1;
wire [1228:1228] sram_blwl_1228_configbus0_b;
assign sram_blwl_1228_configbus0[1228:1228] = sram_blwl_bl[1228:1228] ;
assign sram_blwl_1228_configbus1[1228:1228] = sram_blwl_wl[1228:1228] ;
assign sram_blwl_1228_configbus0_b[1228:1228] = sram_blwl_blb[1228:1228] ;
sram6T_blwl sram_blwl_1228_ (sram_blwl_out[1228], sram_blwl_out[1228], sram_blwl_outb[1228], sram_blwl_1228_configbus0[1228:1228], sram_blwl_1228_configbus1[1228:1228] , sram_blwl_1228_configbus0_b[1228:1228] );
wire [1229:1229] sram_blwl_1229_configbus0;
wire [1229:1229] sram_blwl_1229_configbus1;
wire [1229:1229] sram_blwl_1229_configbus0_b;
assign sram_blwl_1229_configbus0[1229:1229] = sram_blwl_bl[1229:1229] ;
assign sram_blwl_1229_configbus1[1229:1229] = sram_blwl_wl[1229:1229] ;
assign sram_blwl_1229_configbus0_b[1229:1229] = sram_blwl_blb[1229:1229] ;
sram6T_blwl sram_blwl_1229_ (sram_blwl_out[1229], sram_blwl_out[1229], sram_blwl_outb[1229], sram_blwl_1229_configbus0[1229:1229], sram_blwl_1229_configbus1[1229:1229] , sram_blwl_1229_configbus0_b[1229:1229] );
wire [1230:1230] sram_blwl_1230_configbus0;
wire [1230:1230] sram_blwl_1230_configbus1;
wire [1230:1230] sram_blwl_1230_configbus0_b;
assign sram_blwl_1230_configbus0[1230:1230] = sram_blwl_bl[1230:1230] ;
assign sram_blwl_1230_configbus1[1230:1230] = sram_blwl_wl[1230:1230] ;
assign sram_blwl_1230_configbus0_b[1230:1230] = sram_blwl_blb[1230:1230] ;
sram6T_blwl sram_blwl_1230_ (sram_blwl_out[1230], sram_blwl_out[1230], sram_blwl_outb[1230], sram_blwl_1230_configbus0[1230:1230], sram_blwl_1230_configbus1[1230:1230] , sram_blwl_1230_configbus0_b[1230:1230] );
wire [1231:1231] sram_blwl_1231_configbus0;
wire [1231:1231] sram_blwl_1231_configbus1;
wire [1231:1231] sram_blwl_1231_configbus0_b;
assign sram_blwl_1231_configbus0[1231:1231] = sram_blwl_bl[1231:1231] ;
assign sram_blwl_1231_configbus1[1231:1231] = sram_blwl_wl[1231:1231] ;
assign sram_blwl_1231_configbus0_b[1231:1231] = sram_blwl_blb[1231:1231] ;
sram6T_blwl sram_blwl_1231_ (sram_blwl_out[1231], sram_blwl_out[1231], sram_blwl_outb[1231], sram_blwl_1231_configbus0[1231:1231], sram_blwl_1231_configbus1[1231:1231] , sram_blwl_1231_configbus0_b[1231:1231] );
wire [1232:1232] sram_blwl_1232_configbus0;
wire [1232:1232] sram_blwl_1232_configbus1;
wire [1232:1232] sram_blwl_1232_configbus0_b;
assign sram_blwl_1232_configbus0[1232:1232] = sram_blwl_bl[1232:1232] ;
assign sram_blwl_1232_configbus1[1232:1232] = sram_blwl_wl[1232:1232] ;
assign sram_blwl_1232_configbus0_b[1232:1232] = sram_blwl_blb[1232:1232] ;
sram6T_blwl sram_blwl_1232_ (sram_blwl_out[1232], sram_blwl_out[1232], sram_blwl_outb[1232], sram_blwl_1232_configbus0[1232:1232], sram_blwl_1232_configbus1[1232:1232] , sram_blwl_1232_configbus0_b[1232:1232] );
wire [1233:1233] sram_blwl_1233_configbus0;
wire [1233:1233] sram_blwl_1233_configbus1;
wire [1233:1233] sram_blwl_1233_configbus0_b;
assign sram_blwl_1233_configbus0[1233:1233] = sram_blwl_bl[1233:1233] ;
assign sram_blwl_1233_configbus1[1233:1233] = sram_blwl_wl[1233:1233] ;
assign sram_blwl_1233_configbus0_b[1233:1233] = sram_blwl_blb[1233:1233] ;
sram6T_blwl sram_blwl_1233_ (sram_blwl_out[1233], sram_blwl_out[1233], sram_blwl_outb[1233], sram_blwl_1233_configbus0[1233:1233], sram_blwl_1233_configbus1[1233:1233] , sram_blwl_1233_configbus0_b[1233:1233] );
wire [1234:1234] sram_blwl_1234_configbus0;
wire [1234:1234] sram_blwl_1234_configbus1;
wire [1234:1234] sram_blwl_1234_configbus0_b;
assign sram_blwl_1234_configbus0[1234:1234] = sram_blwl_bl[1234:1234] ;
assign sram_blwl_1234_configbus1[1234:1234] = sram_blwl_wl[1234:1234] ;
assign sram_blwl_1234_configbus0_b[1234:1234] = sram_blwl_blb[1234:1234] ;
sram6T_blwl sram_blwl_1234_ (sram_blwl_out[1234], sram_blwl_out[1234], sram_blwl_outb[1234], sram_blwl_1234_configbus0[1234:1234], sram_blwl_1234_configbus1[1234:1234] , sram_blwl_1234_configbus0_b[1234:1234] );
wire [1235:1235] sram_blwl_1235_configbus0;
wire [1235:1235] sram_blwl_1235_configbus1;
wire [1235:1235] sram_blwl_1235_configbus0_b;
assign sram_blwl_1235_configbus0[1235:1235] = sram_blwl_bl[1235:1235] ;
assign sram_blwl_1235_configbus1[1235:1235] = sram_blwl_wl[1235:1235] ;
assign sram_blwl_1235_configbus0_b[1235:1235] = sram_blwl_blb[1235:1235] ;
sram6T_blwl sram_blwl_1235_ (sram_blwl_out[1235], sram_blwl_out[1235], sram_blwl_outb[1235], sram_blwl_1235_configbus0[1235:1235], sram_blwl_1235_configbus1[1235:1235] , sram_blwl_1235_configbus0_b[1235:1235] );
wire [1236:1236] sram_blwl_1236_configbus0;
wire [1236:1236] sram_blwl_1236_configbus1;
wire [1236:1236] sram_blwl_1236_configbus0_b;
assign sram_blwl_1236_configbus0[1236:1236] = sram_blwl_bl[1236:1236] ;
assign sram_blwl_1236_configbus1[1236:1236] = sram_blwl_wl[1236:1236] ;
assign sram_blwl_1236_configbus0_b[1236:1236] = sram_blwl_blb[1236:1236] ;
sram6T_blwl sram_blwl_1236_ (sram_blwl_out[1236], sram_blwl_out[1236], sram_blwl_outb[1236], sram_blwl_1236_configbus0[1236:1236], sram_blwl_1236_configbus1[1236:1236] , sram_blwl_1236_configbus0_b[1236:1236] );
wire [1237:1237] sram_blwl_1237_configbus0;
wire [1237:1237] sram_blwl_1237_configbus1;
wire [1237:1237] sram_blwl_1237_configbus0_b;
assign sram_blwl_1237_configbus0[1237:1237] = sram_blwl_bl[1237:1237] ;
assign sram_blwl_1237_configbus1[1237:1237] = sram_blwl_wl[1237:1237] ;
assign sram_blwl_1237_configbus0_b[1237:1237] = sram_blwl_blb[1237:1237] ;
sram6T_blwl sram_blwl_1237_ (sram_blwl_out[1237], sram_blwl_out[1237], sram_blwl_outb[1237], sram_blwl_1237_configbus0[1237:1237], sram_blwl_1237_configbus1[1237:1237] , sram_blwl_1237_configbus0_b[1237:1237] );
wire [1238:1238] sram_blwl_1238_configbus0;
wire [1238:1238] sram_blwl_1238_configbus1;
wire [1238:1238] sram_blwl_1238_configbus0_b;
assign sram_blwl_1238_configbus0[1238:1238] = sram_blwl_bl[1238:1238] ;
assign sram_blwl_1238_configbus1[1238:1238] = sram_blwl_wl[1238:1238] ;
assign sram_blwl_1238_configbus0_b[1238:1238] = sram_blwl_blb[1238:1238] ;
sram6T_blwl sram_blwl_1238_ (sram_blwl_out[1238], sram_blwl_out[1238], sram_blwl_outb[1238], sram_blwl_1238_configbus0[1238:1238], sram_blwl_1238_configbus1[1238:1238] , sram_blwl_1238_configbus0_b[1238:1238] );
wire [1239:1239] sram_blwl_1239_configbus0;
wire [1239:1239] sram_blwl_1239_configbus1;
wire [1239:1239] sram_blwl_1239_configbus0_b;
assign sram_blwl_1239_configbus0[1239:1239] = sram_blwl_bl[1239:1239] ;
assign sram_blwl_1239_configbus1[1239:1239] = sram_blwl_wl[1239:1239] ;
assign sram_blwl_1239_configbus0_b[1239:1239] = sram_blwl_blb[1239:1239] ;
sram6T_blwl sram_blwl_1239_ (sram_blwl_out[1239], sram_blwl_out[1239], sram_blwl_outb[1239], sram_blwl_1239_configbus0[1239:1239], sram_blwl_1239_configbus1[1239:1239] , sram_blwl_1239_configbus0_b[1239:1239] );
wire [1240:1240] sram_blwl_1240_configbus0;
wire [1240:1240] sram_blwl_1240_configbus1;
wire [1240:1240] sram_blwl_1240_configbus0_b;
assign sram_blwl_1240_configbus0[1240:1240] = sram_blwl_bl[1240:1240] ;
assign sram_blwl_1240_configbus1[1240:1240] = sram_blwl_wl[1240:1240] ;
assign sram_blwl_1240_configbus0_b[1240:1240] = sram_blwl_blb[1240:1240] ;
sram6T_blwl sram_blwl_1240_ (sram_blwl_out[1240], sram_blwl_out[1240], sram_blwl_outb[1240], sram_blwl_1240_configbus0[1240:1240], sram_blwl_1240_configbus1[1240:1240] , sram_blwl_1240_configbus0_b[1240:1240] );
wire [1241:1241] sram_blwl_1241_configbus0;
wire [1241:1241] sram_blwl_1241_configbus1;
wire [1241:1241] sram_blwl_1241_configbus0_b;
assign sram_blwl_1241_configbus0[1241:1241] = sram_blwl_bl[1241:1241] ;
assign sram_blwl_1241_configbus1[1241:1241] = sram_blwl_wl[1241:1241] ;
assign sram_blwl_1241_configbus0_b[1241:1241] = sram_blwl_blb[1241:1241] ;
sram6T_blwl sram_blwl_1241_ (sram_blwl_out[1241], sram_blwl_out[1241], sram_blwl_outb[1241], sram_blwl_1241_configbus0[1241:1241], sram_blwl_1241_configbus1[1241:1241] , sram_blwl_1241_configbus0_b[1241:1241] );
wire [1242:1242] sram_blwl_1242_configbus0;
wire [1242:1242] sram_blwl_1242_configbus1;
wire [1242:1242] sram_blwl_1242_configbus0_b;
assign sram_blwl_1242_configbus0[1242:1242] = sram_blwl_bl[1242:1242] ;
assign sram_blwl_1242_configbus1[1242:1242] = sram_blwl_wl[1242:1242] ;
assign sram_blwl_1242_configbus0_b[1242:1242] = sram_blwl_blb[1242:1242] ;
sram6T_blwl sram_blwl_1242_ (sram_blwl_out[1242], sram_blwl_out[1242], sram_blwl_outb[1242], sram_blwl_1242_configbus0[1242:1242], sram_blwl_1242_configbus1[1242:1242] , sram_blwl_1242_configbus0_b[1242:1242] );
wire [1243:1243] sram_blwl_1243_configbus0;
wire [1243:1243] sram_blwl_1243_configbus1;
wire [1243:1243] sram_blwl_1243_configbus0_b;
assign sram_blwl_1243_configbus0[1243:1243] = sram_blwl_bl[1243:1243] ;
assign sram_blwl_1243_configbus1[1243:1243] = sram_blwl_wl[1243:1243] ;
assign sram_blwl_1243_configbus0_b[1243:1243] = sram_blwl_blb[1243:1243] ;
sram6T_blwl sram_blwl_1243_ (sram_blwl_out[1243], sram_blwl_out[1243], sram_blwl_outb[1243], sram_blwl_1243_configbus0[1243:1243], sram_blwl_1243_configbus1[1243:1243] , sram_blwl_1243_configbus0_b[1243:1243] );
wire [1244:1244] sram_blwl_1244_configbus0;
wire [1244:1244] sram_blwl_1244_configbus1;
wire [1244:1244] sram_blwl_1244_configbus0_b;
assign sram_blwl_1244_configbus0[1244:1244] = sram_blwl_bl[1244:1244] ;
assign sram_blwl_1244_configbus1[1244:1244] = sram_blwl_wl[1244:1244] ;
assign sram_blwl_1244_configbus0_b[1244:1244] = sram_blwl_blb[1244:1244] ;
sram6T_blwl sram_blwl_1244_ (sram_blwl_out[1244], sram_blwl_out[1244], sram_blwl_outb[1244], sram_blwl_1244_configbus0[1244:1244], sram_blwl_1244_configbus1[1244:1244] , sram_blwl_1244_configbus0_b[1244:1244] );
wire [1245:1245] sram_blwl_1245_configbus0;
wire [1245:1245] sram_blwl_1245_configbus1;
wire [1245:1245] sram_blwl_1245_configbus0_b;
assign sram_blwl_1245_configbus0[1245:1245] = sram_blwl_bl[1245:1245] ;
assign sram_blwl_1245_configbus1[1245:1245] = sram_blwl_wl[1245:1245] ;
assign sram_blwl_1245_configbus0_b[1245:1245] = sram_blwl_blb[1245:1245] ;
sram6T_blwl sram_blwl_1245_ (sram_blwl_out[1245], sram_blwl_out[1245], sram_blwl_outb[1245], sram_blwl_1245_configbus0[1245:1245], sram_blwl_1245_configbus1[1245:1245] , sram_blwl_1245_configbus0_b[1245:1245] );
wire [1246:1246] sram_blwl_1246_configbus0;
wire [1246:1246] sram_blwl_1246_configbus1;
wire [1246:1246] sram_blwl_1246_configbus0_b;
assign sram_blwl_1246_configbus0[1246:1246] = sram_blwl_bl[1246:1246] ;
assign sram_blwl_1246_configbus1[1246:1246] = sram_blwl_wl[1246:1246] ;
assign sram_blwl_1246_configbus0_b[1246:1246] = sram_blwl_blb[1246:1246] ;
sram6T_blwl sram_blwl_1246_ (sram_blwl_out[1246], sram_blwl_out[1246], sram_blwl_outb[1246], sram_blwl_1246_configbus0[1246:1246], sram_blwl_1246_configbus1[1246:1246] , sram_blwl_1246_configbus0_b[1246:1246] );
wire [1247:1247] sram_blwl_1247_configbus0;
wire [1247:1247] sram_blwl_1247_configbus1;
wire [1247:1247] sram_blwl_1247_configbus0_b;
assign sram_blwl_1247_configbus0[1247:1247] = sram_blwl_bl[1247:1247] ;
assign sram_blwl_1247_configbus1[1247:1247] = sram_blwl_wl[1247:1247] ;
assign sram_blwl_1247_configbus0_b[1247:1247] = sram_blwl_blb[1247:1247] ;
sram6T_blwl sram_blwl_1247_ (sram_blwl_out[1247], sram_blwl_out[1247], sram_blwl_outb[1247], sram_blwl_1247_configbus0[1247:1247], sram_blwl_1247_configbus1[1247:1247] , sram_blwl_1247_configbus0_b[1247:1247] );
wire [1248:1248] sram_blwl_1248_configbus0;
wire [1248:1248] sram_blwl_1248_configbus1;
wire [1248:1248] sram_blwl_1248_configbus0_b;
assign sram_blwl_1248_configbus0[1248:1248] = sram_blwl_bl[1248:1248] ;
assign sram_blwl_1248_configbus1[1248:1248] = sram_blwl_wl[1248:1248] ;
assign sram_blwl_1248_configbus0_b[1248:1248] = sram_blwl_blb[1248:1248] ;
sram6T_blwl sram_blwl_1248_ (sram_blwl_out[1248], sram_blwl_out[1248], sram_blwl_outb[1248], sram_blwl_1248_configbus0[1248:1248], sram_blwl_1248_configbus1[1248:1248] , sram_blwl_1248_configbus0_b[1248:1248] );
wire [1249:1249] sram_blwl_1249_configbus0;
wire [1249:1249] sram_blwl_1249_configbus1;
wire [1249:1249] sram_blwl_1249_configbus0_b;
assign sram_blwl_1249_configbus0[1249:1249] = sram_blwl_bl[1249:1249] ;
assign sram_blwl_1249_configbus1[1249:1249] = sram_blwl_wl[1249:1249] ;
assign sram_blwl_1249_configbus0_b[1249:1249] = sram_blwl_blb[1249:1249] ;
sram6T_blwl sram_blwl_1249_ (sram_blwl_out[1249], sram_blwl_out[1249], sram_blwl_outb[1249], sram_blwl_1249_configbus0[1249:1249], sram_blwl_1249_configbus1[1249:1249] , sram_blwl_1249_configbus0_b[1249:1249] );
wire [1250:1250] sram_blwl_1250_configbus0;
wire [1250:1250] sram_blwl_1250_configbus1;
wire [1250:1250] sram_blwl_1250_configbus0_b;
assign sram_blwl_1250_configbus0[1250:1250] = sram_blwl_bl[1250:1250] ;
assign sram_blwl_1250_configbus1[1250:1250] = sram_blwl_wl[1250:1250] ;
assign sram_blwl_1250_configbus0_b[1250:1250] = sram_blwl_blb[1250:1250] ;
sram6T_blwl sram_blwl_1250_ (sram_blwl_out[1250], sram_blwl_out[1250], sram_blwl_outb[1250], sram_blwl_1250_configbus0[1250:1250], sram_blwl_1250_configbus1[1250:1250] , sram_blwl_1250_configbus0_b[1250:1250] );
wire [1251:1251] sram_blwl_1251_configbus0;
wire [1251:1251] sram_blwl_1251_configbus1;
wire [1251:1251] sram_blwl_1251_configbus0_b;
assign sram_blwl_1251_configbus0[1251:1251] = sram_blwl_bl[1251:1251] ;
assign sram_blwl_1251_configbus1[1251:1251] = sram_blwl_wl[1251:1251] ;
assign sram_blwl_1251_configbus0_b[1251:1251] = sram_blwl_blb[1251:1251] ;
sram6T_blwl sram_blwl_1251_ (sram_blwl_out[1251], sram_blwl_out[1251], sram_blwl_outb[1251], sram_blwl_1251_configbus0[1251:1251], sram_blwl_1251_configbus1[1251:1251] , sram_blwl_1251_configbus0_b[1251:1251] );
wire [1252:1252] sram_blwl_1252_configbus0;
wire [1252:1252] sram_blwl_1252_configbus1;
wire [1252:1252] sram_blwl_1252_configbus0_b;
assign sram_blwl_1252_configbus0[1252:1252] = sram_blwl_bl[1252:1252] ;
assign sram_blwl_1252_configbus1[1252:1252] = sram_blwl_wl[1252:1252] ;
assign sram_blwl_1252_configbus0_b[1252:1252] = sram_blwl_blb[1252:1252] ;
sram6T_blwl sram_blwl_1252_ (sram_blwl_out[1252], sram_blwl_out[1252], sram_blwl_outb[1252], sram_blwl_1252_configbus0[1252:1252], sram_blwl_1252_configbus1[1252:1252] , sram_blwl_1252_configbus0_b[1252:1252] );
wire [1253:1253] sram_blwl_1253_configbus0;
wire [1253:1253] sram_blwl_1253_configbus1;
wire [1253:1253] sram_blwl_1253_configbus0_b;
assign sram_blwl_1253_configbus0[1253:1253] = sram_blwl_bl[1253:1253] ;
assign sram_blwl_1253_configbus1[1253:1253] = sram_blwl_wl[1253:1253] ;
assign sram_blwl_1253_configbus0_b[1253:1253] = sram_blwl_blb[1253:1253] ;
sram6T_blwl sram_blwl_1253_ (sram_blwl_out[1253], sram_blwl_out[1253], sram_blwl_outb[1253], sram_blwl_1253_configbus0[1253:1253], sram_blwl_1253_configbus1[1253:1253] , sram_blwl_1253_configbus0_b[1253:1253] );
wire [1254:1254] sram_blwl_1254_configbus0;
wire [1254:1254] sram_blwl_1254_configbus1;
wire [1254:1254] sram_blwl_1254_configbus0_b;
assign sram_blwl_1254_configbus0[1254:1254] = sram_blwl_bl[1254:1254] ;
assign sram_blwl_1254_configbus1[1254:1254] = sram_blwl_wl[1254:1254] ;
assign sram_blwl_1254_configbus0_b[1254:1254] = sram_blwl_blb[1254:1254] ;
sram6T_blwl sram_blwl_1254_ (sram_blwl_out[1254], sram_blwl_out[1254], sram_blwl_outb[1254], sram_blwl_1254_configbus0[1254:1254], sram_blwl_1254_configbus1[1254:1254] , sram_blwl_1254_configbus0_b[1254:1254] );
wire [1255:1255] sram_blwl_1255_configbus0;
wire [1255:1255] sram_blwl_1255_configbus1;
wire [1255:1255] sram_blwl_1255_configbus0_b;
assign sram_blwl_1255_configbus0[1255:1255] = sram_blwl_bl[1255:1255] ;
assign sram_blwl_1255_configbus1[1255:1255] = sram_blwl_wl[1255:1255] ;
assign sram_blwl_1255_configbus0_b[1255:1255] = sram_blwl_blb[1255:1255] ;
sram6T_blwl sram_blwl_1255_ (sram_blwl_out[1255], sram_blwl_out[1255], sram_blwl_outb[1255], sram_blwl_1255_configbus0[1255:1255], sram_blwl_1255_configbus1[1255:1255] , sram_blwl_1255_configbus0_b[1255:1255] );
wire [1256:1256] sram_blwl_1256_configbus0;
wire [1256:1256] sram_blwl_1256_configbus1;
wire [1256:1256] sram_blwl_1256_configbus0_b;
assign sram_blwl_1256_configbus0[1256:1256] = sram_blwl_bl[1256:1256] ;
assign sram_blwl_1256_configbus1[1256:1256] = sram_blwl_wl[1256:1256] ;
assign sram_blwl_1256_configbus0_b[1256:1256] = sram_blwl_blb[1256:1256] ;
sram6T_blwl sram_blwl_1256_ (sram_blwl_out[1256], sram_blwl_out[1256], sram_blwl_outb[1256], sram_blwl_1256_configbus0[1256:1256], sram_blwl_1256_configbus1[1256:1256] , sram_blwl_1256_configbus0_b[1256:1256] );
wire [1257:1257] sram_blwl_1257_configbus0;
wire [1257:1257] sram_blwl_1257_configbus1;
wire [1257:1257] sram_blwl_1257_configbus0_b;
assign sram_blwl_1257_configbus0[1257:1257] = sram_blwl_bl[1257:1257] ;
assign sram_blwl_1257_configbus1[1257:1257] = sram_blwl_wl[1257:1257] ;
assign sram_blwl_1257_configbus0_b[1257:1257] = sram_blwl_blb[1257:1257] ;
sram6T_blwl sram_blwl_1257_ (sram_blwl_out[1257], sram_blwl_out[1257], sram_blwl_outb[1257], sram_blwl_1257_configbus0[1257:1257], sram_blwl_1257_configbus1[1257:1257] , sram_blwl_1257_configbus0_b[1257:1257] );
wire [1258:1258] sram_blwl_1258_configbus0;
wire [1258:1258] sram_blwl_1258_configbus1;
wire [1258:1258] sram_blwl_1258_configbus0_b;
assign sram_blwl_1258_configbus0[1258:1258] = sram_blwl_bl[1258:1258] ;
assign sram_blwl_1258_configbus1[1258:1258] = sram_blwl_wl[1258:1258] ;
assign sram_blwl_1258_configbus0_b[1258:1258] = sram_blwl_blb[1258:1258] ;
sram6T_blwl sram_blwl_1258_ (sram_blwl_out[1258], sram_blwl_out[1258], sram_blwl_outb[1258], sram_blwl_1258_configbus0[1258:1258], sram_blwl_1258_configbus1[1258:1258] , sram_blwl_1258_configbus0_b[1258:1258] );
wire [1259:1259] sram_blwl_1259_configbus0;
wire [1259:1259] sram_blwl_1259_configbus1;
wire [1259:1259] sram_blwl_1259_configbus0_b;
assign sram_blwl_1259_configbus0[1259:1259] = sram_blwl_bl[1259:1259] ;
assign sram_blwl_1259_configbus1[1259:1259] = sram_blwl_wl[1259:1259] ;
assign sram_blwl_1259_configbus0_b[1259:1259] = sram_blwl_blb[1259:1259] ;
sram6T_blwl sram_blwl_1259_ (sram_blwl_out[1259], sram_blwl_out[1259], sram_blwl_outb[1259], sram_blwl_1259_configbus0[1259:1259], sram_blwl_1259_configbus1[1259:1259] , sram_blwl_1259_configbus0_b[1259:1259] );
wire [1260:1260] sram_blwl_1260_configbus0;
wire [1260:1260] sram_blwl_1260_configbus1;
wire [1260:1260] sram_blwl_1260_configbus0_b;
assign sram_blwl_1260_configbus0[1260:1260] = sram_blwl_bl[1260:1260] ;
assign sram_blwl_1260_configbus1[1260:1260] = sram_blwl_wl[1260:1260] ;
assign sram_blwl_1260_configbus0_b[1260:1260] = sram_blwl_blb[1260:1260] ;
sram6T_blwl sram_blwl_1260_ (sram_blwl_out[1260], sram_blwl_out[1260], sram_blwl_outb[1260], sram_blwl_1260_configbus0[1260:1260], sram_blwl_1260_configbus1[1260:1260] , sram_blwl_1260_configbus0_b[1260:1260] );
wire [1261:1261] sram_blwl_1261_configbus0;
wire [1261:1261] sram_blwl_1261_configbus1;
wire [1261:1261] sram_blwl_1261_configbus0_b;
assign sram_blwl_1261_configbus0[1261:1261] = sram_blwl_bl[1261:1261] ;
assign sram_blwl_1261_configbus1[1261:1261] = sram_blwl_wl[1261:1261] ;
assign sram_blwl_1261_configbus0_b[1261:1261] = sram_blwl_blb[1261:1261] ;
sram6T_blwl sram_blwl_1261_ (sram_blwl_out[1261], sram_blwl_out[1261], sram_blwl_outb[1261], sram_blwl_1261_configbus0[1261:1261], sram_blwl_1261_configbus1[1261:1261] , sram_blwl_1261_configbus0_b[1261:1261] );
wire [1262:1262] sram_blwl_1262_configbus0;
wire [1262:1262] sram_blwl_1262_configbus1;
wire [1262:1262] sram_blwl_1262_configbus0_b;
assign sram_blwl_1262_configbus0[1262:1262] = sram_blwl_bl[1262:1262] ;
assign sram_blwl_1262_configbus1[1262:1262] = sram_blwl_wl[1262:1262] ;
assign sram_blwl_1262_configbus0_b[1262:1262] = sram_blwl_blb[1262:1262] ;
sram6T_blwl sram_blwl_1262_ (sram_blwl_out[1262], sram_blwl_out[1262], sram_blwl_outb[1262], sram_blwl_1262_configbus0[1262:1262], sram_blwl_1262_configbus1[1262:1262] , sram_blwl_1262_configbus0_b[1262:1262] );
wire [1263:1263] sram_blwl_1263_configbus0;
wire [1263:1263] sram_blwl_1263_configbus1;
wire [1263:1263] sram_blwl_1263_configbus0_b;
assign sram_blwl_1263_configbus0[1263:1263] = sram_blwl_bl[1263:1263] ;
assign sram_blwl_1263_configbus1[1263:1263] = sram_blwl_wl[1263:1263] ;
assign sram_blwl_1263_configbus0_b[1263:1263] = sram_blwl_blb[1263:1263] ;
sram6T_blwl sram_blwl_1263_ (sram_blwl_out[1263], sram_blwl_out[1263], sram_blwl_outb[1263], sram_blwl_1263_configbus0[1263:1263], sram_blwl_1263_configbus1[1263:1263] , sram_blwl_1263_configbus0_b[1263:1263] );
wire [1264:1264] sram_blwl_1264_configbus0;
wire [1264:1264] sram_blwl_1264_configbus1;
wire [1264:1264] sram_blwl_1264_configbus0_b;
assign sram_blwl_1264_configbus0[1264:1264] = sram_blwl_bl[1264:1264] ;
assign sram_blwl_1264_configbus1[1264:1264] = sram_blwl_wl[1264:1264] ;
assign sram_blwl_1264_configbus0_b[1264:1264] = sram_blwl_blb[1264:1264] ;
sram6T_blwl sram_blwl_1264_ (sram_blwl_out[1264], sram_blwl_out[1264], sram_blwl_outb[1264], sram_blwl_1264_configbus0[1264:1264], sram_blwl_1264_configbus1[1264:1264] , sram_blwl_1264_configbus0_b[1264:1264] );
wire [1265:1265] sram_blwl_1265_configbus0;
wire [1265:1265] sram_blwl_1265_configbus1;
wire [1265:1265] sram_blwl_1265_configbus0_b;
assign sram_blwl_1265_configbus0[1265:1265] = sram_blwl_bl[1265:1265] ;
assign sram_blwl_1265_configbus1[1265:1265] = sram_blwl_wl[1265:1265] ;
assign sram_blwl_1265_configbus0_b[1265:1265] = sram_blwl_blb[1265:1265] ;
sram6T_blwl sram_blwl_1265_ (sram_blwl_out[1265], sram_blwl_out[1265], sram_blwl_outb[1265], sram_blwl_1265_configbus0[1265:1265], sram_blwl_1265_configbus1[1265:1265] , sram_blwl_1265_configbus0_b[1265:1265] );
wire [1266:1266] sram_blwl_1266_configbus0;
wire [1266:1266] sram_blwl_1266_configbus1;
wire [1266:1266] sram_blwl_1266_configbus0_b;
assign sram_blwl_1266_configbus0[1266:1266] = sram_blwl_bl[1266:1266] ;
assign sram_blwl_1266_configbus1[1266:1266] = sram_blwl_wl[1266:1266] ;
assign sram_blwl_1266_configbus0_b[1266:1266] = sram_blwl_blb[1266:1266] ;
sram6T_blwl sram_blwl_1266_ (sram_blwl_out[1266], sram_blwl_out[1266], sram_blwl_outb[1266], sram_blwl_1266_configbus0[1266:1266], sram_blwl_1266_configbus1[1266:1266] , sram_blwl_1266_configbus0_b[1266:1266] );
wire [1267:1267] sram_blwl_1267_configbus0;
wire [1267:1267] sram_blwl_1267_configbus1;
wire [1267:1267] sram_blwl_1267_configbus0_b;
assign sram_blwl_1267_configbus0[1267:1267] = sram_blwl_bl[1267:1267] ;
assign sram_blwl_1267_configbus1[1267:1267] = sram_blwl_wl[1267:1267] ;
assign sram_blwl_1267_configbus0_b[1267:1267] = sram_blwl_blb[1267:1267] ;
sram6T_blwl sram_blwl_1267_ (sram_blwl_out[1267], sram_blwl_out[1267], sram_blwl_outb[1267], sram_blwl_1267_configbus0[1267:1267], sram_blwl_1267_configbus1[1267:1267] , sram_blwl_1267_configbus0_b[1267:1267] );
wire [1268:1268] sram_blwl_1268_configbus0;
wire [1268:1268] sram_blwl_1268_configbus1;
wire [1268:1268] sram_blwl_1268_configbus0_b;
assign sram_blwl_1268_configbus0[1268:1268] = sram_blwl_bl[1268:1268] ;
assign sram_blwl_1268_configbus1[1268:1268] = sram_blwl_wl[1268:1268] ;
assign sram_blwl_1268_configbus0_b[1268:1268] = sram_blwl_blb[1268:1268] ;
sram6T_blwl sram_blwl_1268_ (sram_blwl_out[1268], sram_blwl_out[1268], sram_blwl_outb[1268], sram_blwl_1268_configbus0[1268:1268], sram_blwl_1268_configbus1[1268:1268] , sram_blwl_1268_configbus0_b[1268:1268] );
wire [1269:1269] sram_blwl_1269_configbus0;
wire [1269:1269] sram_blwl_1269_configbus1;
wire [1269:1269] sram_blwl_1269_configbus0_b;
assign sram_blwl_1269_configbus0[1269:1269] = sram_blwl_bl[1269:1269] ;
assign sram_blwl_1269_configbus1[1269:1269] = sram_blwl_wl[1269:1269] ;
assign sram_blwl_1269_configbus0_b[1269:1269] = sram_blwl_blb[1269:1269] ;
sram6T_blwl sram_blwl_1269_ (sram_blwl_out[1269], sram_blwl_out[1269], sram_blwl_outb[1269], sram_blwl_1269_configbus0[1269:1269], sram_blwl_1269_configbus1[1269:1269] , sram_blwl_1269_configbus0_b[1269:1269] );
wire [1270:1270] sram_blwl_1270_configbus0;
wire [1270:1270] sram_blwl_1270_configbus1;
wire [1270:1270] sram_blwl_1270_configbus0_b;
assign sram_blwl_1270_configbus0[1270:1270] = sram_blwl_bl[1270:1270] ;
assign sram_blwl_1270_configbus1[1270:1270] = sram_blwl_wl[1270:1270] ;
assign sram_blwl_1270_configbus0_b[1270:1270] = sram_blwl_blb[1270:1270] ;
sram6T_blwl sram_blwl_1270_ (sram_blwl_out[1270], sram_blwl_out[1270], sram_blwl_outb[1270], sram_blwl_1270_configbus0[1270:1270], sram_blwl_1270_configbus1[1270:1270] , sram_blwl_1270_configbus0_b[1270:1270] );
wire [1271:1271] sram_blwl_1271_configbus0;
wire [1271:1271] sram_blwl_1271_configbus1;
wire [1271:1271] sram_blwl_1271_configbus0_b;
assign sram_blwl_1271_configbus0[1271:1271] = sram_blwl_bl[1271:1271] ;
assign sram_blwl_1271_configbus1[1271:1271] = sram_blwl_wl[1271:1271] ;
assign sram_blwl_1271_configbus0_b[1271:1271] = sram_blwl_blb[1271:1271] ;
sram6T_blwl sram_blwl_1271_ (sram_blwl_out[1271], sram_blwl_out[1271], sram_blwl_outb[1271], sram_blwl_1271_configbus0[1271:1271], sram_blwl_1271_configbus1[1271:1271] , sram_blwl_1271_configbus0_b[1271:1271] );
wire [1272:1272] sram_blwl_1272_configbus0;
wire [1272:1272] sram_blwl_1272_configbus1;
wire [1272:1272] sram_blwl_1272_configbus0_b;
assign sram_blwl_1272_configbus0[1272:1272] = sram_blwl_bl[1272:1272] ;
assign sram_blwl_1272_configbus1[1272:1272] = sram_blwl_wl[1272:1272] ;
assign sram_blwl_1272_configbus0_b[1272:1272] = sram_blwl_blb[1272:1272] ;
sram6T_blwl sram_blwl_1272_ (sram_blwl_out[1272], sram_blwl_out[1272], sram_blwl_outb[1272], sram_blwl_1272_configbus0[1272:1272], sram_blwl_1272_configbus1[1272:1272] , sram_blwl_1272_configbus0_b[1272:1272] );
wire [1273:1273] sram_blwl_1273_configbus0;
wire [1273:1273] sram_blwl_1273_configbus1;
wire [1273:1273] sram_blwl_1273_configbus0_b;
assign sram_blwl_1273_configbus0[1273:1273] = sram_blwl_bl[1273:1273] ;
assign sram_blwl_1273_configbus1[1273:1273] = sram_blwl_wl[1273:1273] ;
assign sram_blwl_1273_configbus0_b[1273:1273] = sram_blwl_blb[1273:1273] ;
sram6T_blwl sram_blwl_1273_ (sram_blwl_out[1273], sram_blwl_out[1273], sram_blwl_outb[1273], sram_blwl_1273_configbus0[1273:1273], sram_blwl_1273_configbus1[1273:1273] , sram_blwl_1273_configbus0_b[1273:1273] );
wire [1274:1274] sram_blwl_1274_configbus0;
wire [1274:1274] sram_blwl_1274_configbus1;
wire [1274:1274] sram_blwl_1274_configbus0_b;
assign sram_blwl_1274_configbus0[1274:1274] = sram_blwl_bl[1274:1274] ;
assign sram_blwl_1274_configbus1[1274:1274] = sram_blwl_wl[1274:1274] ;
assign sram_blwl_1274_configbus0_b[1274:1274] = sram_blwl_blb[1274:1274] ;
sram6T_blwl sram_blwl_1274_ (sram_blwl_out[1274], sram_blwl_out[1274], sram_blwl_outb[1274], sram_blwl_1274_configbus0[1274:1274], sram_blwl_1274_configbus1[1274:1274] , sram_blwl_1274_configbus0_b[1274:1274] );
endmodule
//----- END LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----

//----- Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----
module grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
input [0:0] Set,
input [0:0] Reset,
input [0:0] clk
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
input wire ff_0___D_0_,
output wire ff_0___Q_0_);
static_dff dff_3_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
endmodule
//----- END Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6_ -----
module grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_ble6___in_0_,
input wire mode_ble6___in_1_,
input wire mode_ble6___in_2_,
input wire mode_ble6___in_3_,
input wire mode_ble6___in_4_,
input wire mode_ble6___in_5_,
output wire mode_ble6___out_0_,
input wire mode_ble6___clk_0_,
input [1211:1275] sram_blwl_bl ,
input [1211:1275] sram_blwl_wl ,
input [1211:1275] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ lut6_0_ (
 lut6_0___in_0_,  lut6_0___in_1_,  lut6_0___in_2_,  lut6_0___in_3_,  lut6_0___in_4_,  lut6_0___in_5_,  lut6_0___out_0_,
sram_blwl_bl[1211:1274] ,
sram_blwl_wl[1211:1274] ,
sram_blwl_blb[1211:1274] );
grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ ff_0_ (
//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
wire [0:1] in_bus_mux_1level_tapbuf_size2_403_ ;
assign in_bus_mux_1level_tapbuf_size2_403_[0] = ff_0___Q_0_ ; 
assign in_bus_mux_1level_tapbuf_size2_403_[1] = lut6_0___out_0_ ; 
wire [1275:1275] mux_1level_tapbuf_size2_403_configbus0;
wire [1275:1275] mux_1level_tapbuf_size2_403_configbus1;
wire [1275:1275] mux_1level_tapbuf_size2_403_sram_blwl_out ;
wire [1275:1275] mux_1level_tapbuf_size2_403_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_403_configbus0[1275:1275] = sram_blwl_bl[1275:1275] ;
assign mux_1level_tapbuf_size2_403_configbus1[1275:1275] = sram_blwl_wl[1275:1275] ;
wire [1275:1275] mux_1level_tapbuf_size2_403_configbus0_b;
assign mux_1level_tapbuf_size2_403_configbus0_b[1275:1275] = sram_blwl_blb[1275:1275] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_403_ (in_bus_mux_1level_tapbuf_size2_403_, mode_ble6___out_0_, mux_1level_tapbuf_size2_403_sram_blwl_out[1275:1275] ,
mux_1level_tapbuf_size2_403_sram_blwl_outb[1275:1275] );
//----- SRAM bits for MUX[403], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_1275_ (mux_1level_tapbuf_size2_403_sram_blwl_out[1275:1275] ,mux_1level_tapbuf_size2_403_sram_blwl_out[1275:1275] ,mux_1level_tapbuf_size2_403_sram_blwl_outb[1275:1275] ,mux_1level_tapbuf_size2_403_configbus0[1275:1275], mux_1level_tapbuf_size2_403_configbus1[1275:1275] , mux_1level_tapbuf_size2_403_configbus0_b[1275:1275] );
direct_interc direct_interc_48_ (mode_ble6___in_0_, lut6_0___in_0_ );
direct_interc direct_interc_49_ (mode_ble6___in_1_, lut6_0___in_1_ );
direct_interc direct_interc_50_ (mode_ble6___in_2_, lut6_0___in_2_ );
direct_interc direct_interc_51_ (mode_ble6___in_3_, lut6_0___in_3_ );
direct_interc direct_interc_52_ (mode_ble6___in_4_, lut6_0___in_4_ );
direct_interc direct_interc_53_ (mode_ble6___in_5_, lut6_0___in_5_ );
direct_interc direct_interc_54_ (lut6_0___out_0_, ff_0___D_0_ );
direct_interc direct_interc_55_ (mode_ble6___clk_0_, ff_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6_ -----
module grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_n1_lut6___in_0_,
input wire mode_n1_lut6___in_1_,
input wire mode_n1_lut6___in_2_,
input wire mode_n1_lut6___in_3_,
input wire mode_n1_lut6___in_4_,
input wire mode_n1_lut6___in_5_,
output wire mode_n1_lut6___out_0_,
input wire mode_n1_lut6___clk_0_,
input [1211:1275] sram_blwl_bl ,
input [1211:1275] sram_blwl_wl ,
input [1211:1275] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6__ble6_0__mode_ble6_ ble6_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 ble6_0___in_0_,  ble6_0___in_1_,  ble6_0___in_2_,  ble6_0___in_3_,  ble6_0___in_4_,  ble6_0___in_5_,  ble6_0___out_0_,  ble6_0___clk_0_,
sram_blwl_bl[1211:1275] ,
sram_blwl_wl[1211:1275] ,
sram_blwl_blb[1211:1275] );
direct_interc direct_interc_56_ (ble6_0___out_0_, mode_n1_lut6___out_0_ );
direct_interc direct_interc_57_ (mode_n1_lut6___in_0_, ble6_0___in_0_ );
direct_interc direct_interc_58_ (mode_n1_lut6___in_1_, ble6_0___in_1_ );
direct_interc direct_interc_59_ (mode_n1_lut6___in_2_, ble6_0___in_2_ );
direct_interc direct_interc_60_ (mode_n1_lut6___in_3_, ble6_0___in_3_ );
direct_interc direct_interc_61_ (mode_n1_lut6___in_4_, ble6_0___in_4_ );
direct_interc direct_interc_62_ (mode_n1_lut6___in_5_, ble6_0___in_5_ );
direct_interc direct_interc_63_ (mode_n1_lut6___clk_0_, ble6_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6_ -----

//----- LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----
module grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ (
input wire lut6_0___in_0_,
input wire lut6_0___in_1_,
input wire lut6_0___in_2_,
input wire lut6_0___in_3_,
input wire lut6_0___in_4_,
input wire lut6_0___in_5_,
output wire lut6_0___out_0_,
input [1276:1339] sram_blwl_bl ,
input [1276:1339] sram_blwl_wl ,
input [1276:1339] sram_blwl_blb );
wire [0:5] lut6_0___in;
assign lut6_0___in[0] = lut6_0___in_0_;
assign lut6_0___in[1] = lut6_0___in_1_;
assign lut6_0___in[2] = lut6_0___in_2_;
assign lut6_0___in[3] = lut6_0___in_3_;
assign lut6_0___in[4] = lut6_0___in_4_;
assign lut6_0___in[5] = lut6_0___in_5_;
wire [0:0] lut6_0___out;
assign lut6_0___out_0_ = lut6_0___out[0];
wire [1276:1339] sram_blwl_out ;
wire [1276:1339] sram_blwl_outb ;
lut6 lut6_4_ (
//----- Input and output ports -----
 lut6_0___in[0:5] ,  lut6_0___out[0:0],//----- SRAM ports -----
sram_blwl_out[1276:1339] , sram_blwl_outb[1276:1339] );
//----- Truth Table for LUT[4], size=6. -----
//----- SRAM bits for LUT[4], size=6, num_sram=64. -----
//-----0000000000000000000000000000000000000000000000000000000000000000-----
wire [1276:1276] sram_blwl_1276_configbus0;
wire [1276:1276] sram_blwl_1276_configbus1;
wire [1276:1276] sram_blwl_1276_configbus0_b;
assign sram_blwl_1276_configbus0[1276:1276] = sram_blwl_bl[1276:1276] ;
assign sram_blwl_1276_configbus1[1276:1276] = sram_blwl_wl[1276:1276] ;
assign sram_blwl_1276_configbus0_b[1276:1276] = sram_blwl_blb[1276:1276] ;
sram6T_blwl sram_blwl_1276_ (sram_blwl_out[1276], sram_blwl_out[1276], sram_blwl_outb[1276], sram_blwl_1276_configbus0[1276:1276], sram_blwl_1276_configbus1[1276:1276] , sram_blwl_1276_configbus0_b[1276:1276] );
wire [1277:1277] sram_blwl_1277_configbus0;
wire [1277:1277] sram_blwl_1277_configbus1;
wire [1277:1277] sram_blwl_1277_configbus0_b;
assign sram_blwl_1277_configbus0[1277:1277] = sram_blwl_bl[1277:1277] ;
assign sram_blwl_1277_configbus1[1277:1277] = sram_blwl_wl[1277:1277] ;
assign sram_blwl_1277_configbus0_b[1277:1277] = sram_blwl_blb[1277:1277] ;
sram6T_blwl sram_blwl_1277_ (sram_blwl_out[1277], sram_blwl_out[1277], sram_blwl_outb[1277], sram_blwl_1277_configbus0[1277:1277], sram_blwl_1277_configbus1[1277:1277] , sram_blwl_1277_configbus0_b[1277:1277] );
wire [1278:1278] sram_blwl_1278_configbus0;
wire [1278:1278] sram_blwl_1278_configbus1;
wire [1278:1278] sram_blwl_1278_configbus0_b;
assign sram_blwl_1278_configbus0[1278:1278] = sram_blwl_bl[1278:1278] ;
assign sram_blwl_1278_configbus1[1278:1278] = sram_blwl_wl[1278:1278] ;
assign sram_blwl_1278_configbus0_b[1278:1278] = sram_blwl_blb[1278:1278] ;
sram6T_blwl sram_blwl_1278_ (sram_blwl_out[1278], sram_blwl_out[1278], sram_blwl_outb[1278], sram_blwl_1278_configbus0[1278:1278], sram_blwl_1278_configbus1[1278:1278] , sram_blwl_1278_configbus0_b[1278:1278] );
wire [1279:1279] sram_blwl_1279_configbus0;
wire [1279:1279] sram_blwl_1279_configbus1;
wire [1279:1279] sram_blwl_1279_configbus0_b;
assign sram_blwl_1279_configbus0[1279:1279] = sram_blwl_bl[1279:1279] ;
assign sram_blwl_1279_configbus1[1279:1279] = sram_blwl_wl[1279:1279] ;
assign sram_blwl_1279_configbus0_b[1279:1279] = sram_blwl_blb[1279:1279] ;
sram6T_blwl sram_blwl_1279_ (sram_blwl_out[1279], sram_blwl_out[1279], sram_blwl_outb[1279], sram_blwl_1279_configbus0[1279:1279], sram_blwl_1279_configbus1[1279:1279] , sram_blwl_1279_configbus0_b[1279:1279] );
wire [1280:1280] sram_blwl_1280_configbus0;
wire [1280:1280] sram_blwl_1280_configbus1;
wire [1280:1280] sram_blwl_1280_configbus0_b;
assign sram_blwl_1280_configbus0[1280:1280] = sram_blwl_bl[1280:1280] ;
assign sram_blwl_1280_configbus1[1280:1280] = sram_blwl_wl[1280:1280] ;
assign sram_blwl_1280_configbus0_b[1280:1280] = sram_blwl_blb[1280:1280] ;
sram6T_blwl sram_blwl_1280_ (sram_blwl_out[1280], sram_blwl_out[1280], sram_blwl_outb[1280], sram_blwl_1280_configbus0[1280:1280], sram_blwl_1280_configbus1[1280:1280] , sram_blwl_1280_configbus0_b[1280:1280] );
wire [1281:1281] sram_blwl_1281_configbus0;
wire [1281:1281] sram_blwl_1281_configbus1;
wire [1281:1281] sram_blwl_1281_configbus0_b;
assign sram_blwl_1281_configbus0[1281:1281] = sram_blwl_bl[1281:1281] ;
assign sram_blwl_1281_configbus1[1281:1281] = sram_blwl_wl[1281:1281] ;
assign sram_blwl_1281_configbus0_b[1281:1281] = sram_blwl_blb[1281:1281] ;
sram6T_blwl sram_blwl_1281_ (sram_blwl_out[1281], sram_blwl_out[1281], sram_blwl_outb[1281], sram_blwl_1281_configbus0[1281:1281], sram_blwl_1281_configbus1[1281:1281] , sram_blwl_1281_configbus0_b[1281:1281] );
wire [1282:1282] sram_blwl_1282_configbus0;
wire [1282:1282] sram_blwl_1282_configbus1;
wire [1282:1282] sram_blwl_1282_configbus0_b;
assign sram_blwl_1282_configbus0[1282:1282] = sram_blwl_bl[1282:1282] ;
assign sram_blwl_1282_configbus1[1282:1282] = sram_blwl_wl[1282:1282] ;
assign sram_blwl_1282_configbus0_b[1282:1282] = sram_blwl_blb[1282:1282] ;
sram6T_blwl sram_blwl_1282_ (sram_blwl_out[1282], sram_blwl_out[1282], sram_blwl_outb[1282], sram_blwl_1282_configbus0[1282:1282], sram_blwl_1282_configbus1[1282:1282] , sram_blwl_1282_configbus0_b[1282:1282] );
wire [1283:1283] sram_blwl_1283_configbus0;
wire [1283:1283] sram_blwl_1283_configbus1;
wire [1283:1283] sram_blwl_1283_configbus0_b;
assign sram_blwl_1283_configbus0[1283:1283] = sram_blwl_bl[1283:1283] ;
assign sram_blwl_1283_configbus1[1283:1283] = sram_blwl_wl[1283:1283] ;
assign sram_blwl_1283_configbus0_b[1283:1283] = sram_blwl_blb[1283:1283] ;
sram6T_blwl sram_blwl_1283_ (sram_blwl_out[1283], sram_blwl_out[1283], sram_blwl_outb[1283], sram_blwl_1283_configbus0[1283:1283], sram_blwl_1283_configbus1[1283:1283] , sram_blwl_1283_configbus0_b[1283:1283] );
wire [1284:1284] sram_blwl_1284_configbus0;
wire [1284:1284] sram_blwl_1284_configbus1;
wire [1284:1284] sram_blwl_1284_configbus0_b;
assign sram_blwl_1284_configbus0[1284:1284] = sram_blwl_bl[1284:1284] ;
assign sram_blwl_1284_configbus1[1284:1284] = sram_blwl_wl[1284:1284] ;
assign sram_blwl_1284_configbus0_b[1284:1284] = sram_blwl_blb[1284:1284] ;
sram6T_blwl sram_blwl_1284_ (sram_blwl_out[1284], sram_blwl_out[1284], sram_blwl_outb[1284], sram_blwl_1284_configbus0[1284:1284], sram_blwl_1284_configbus1[1284:1284] , sram_blwl_1284_configbus0_b[1284:1284] );
wire [1285:1285] sram_blwl_1285_configbus0;
wire [1285:1285] sram_blwl_1285_configbus1;
wire [1285:1285] sram_blwl_1285_configbus0_b;
assign sram_blwl_1285_configbus0[1285:1285] = sram_blwl_bl[1285:1285] ;
assign sram_blwl_1285_configbus1[1285:1285] = sram_blwl_wl[1285:1285] ;
assign sram_blwl_1285_configbus0_b[1285:1285] = sram_blwl_blb[1285:1285] ;
sram6T_blwl sram_blwl_1285_ (sram_blwl_out[1285], sram_blwl_out[1285], sram_blwl_outb[1285], sram_blwl_1285_configbus0[1285:1285], sram_blwl_1285_configbus1[1285:1285] , sram_blwl_1285_configbus0_b[1285:1285] );
wire [1286:1286] sram_blwl_1286_configbus0;
wire [1286:1286] sram_blwl_1286_configbus1;
wire [1286:1286] sram_blwl_1286_configbus0_b;
assign sram_blwl_1286_configbus0[1286:1286] = sram_blwl_bl[1286:1286] ;
assign sram_blwl_1286_configbus1[1286:1286] = sram_blwl_wl[1286:1286] ;
assign sram_blwl_1286_configbus0_b[1286:1286] = sram_blwl_blb[1286:1286] ;
sram6T_blwl sram_blwl_1286_ (sram_blwl_out[1286], sram_blwl_out[1286], sram_blwl_outb[1286], sram_blwl_1286_configbus0[1286:1286], sram_blwl_1286_configbus1[1286:1286] , sram_blwl_1286_configbus0_b[1286:1286] );
wire [1287:1287] sram_blwl_1287_configbus0;
wire [1287:1287] sram_blwl_1287_configbus1;
wire [1287:1287] sram_blwl_1287_configbus0_b;
assign sram_blwl_1287_configbus0[1287:1287] = sram_blwl_bl[1287:1287] ;
assign sram_blwl_1287_configbus1[1287:1287] = sram_blwl_wl[1287:1287] ;
assign sram_blwl_1287_configbus0_b[1287:1287] = sram_blwl_blb[1287:1287] ;
sram6T_blwl sram_blwl_1287_ (sram_blwl_out[1287], sram_blwl_out[1287], sram_blwl_outb[1287], sram_blwl_1287_configbus0[1287:1287], sram_blwl_1287_configbus1[1287:1287] , sram_blwl_1287_configbus0_b[1287:1287] );
wire [1288:1288] sram_blwl_1288_configbus0;
wire [1288:1288] sram_blwl_1288_configbus1;
wire [1288:1288] sram_blwl_1288_configbus0_b;
assign sram_blwl_1288_configbus0[1288:1288] = sram_blwl_bl[1288:1288] ;
assign sram_blwl_1288_configbus1[1288:1288] = sram_blwl_wl[1288:1288] ;
assign sram_blwl_1288_configbus0_b[1288:1288] = sram_blwl_blb[1288:1288] ;
sram6T_blwl sram_blwl_1288_ (sram_blwl_out[1288], sram_blwl_out[1288], sram_blwl_outb[1288], sram_blwl_1288_configbus0[1288:1288], sram_blwl_1288_configbus1[1288:1288] , sram_blwl_1288_configbus0_b[1288:1288] );
wire [1289:1289] sram_blwl_1289_configbus0;
wire [1289:1289] sram_blwl_1289_configbus1;
wire [1289:1289] sram_blwl_1289_configbus0_b;
assign sram_blwl_1289_configbus0[1289:1289] = sram_blwl_bl[1289:1289] ;
assign sram_blwl_1289_configbus1[1289:1289] = sram_blwl_wl[1289:1289] ;
assign sram_blwl_1289_configbus0_b[1289:1289] = sram_blwl_blb[1289:1289] ;
sram6T_blwl sram_blwl_1289_ (sram_blwl_out[1289], sram_blwl_out[1289], sram_blwl_outb[1289], sram_blwl_1289_configbus0[1289:1289], sram_blwl_1289_configbus1[1289:1289] , sram_blwl_1289_configbus0_b[1289:1289] );
wire [1290:1290] sram_blwl_1290_configbus0;
wire [1290:1290] sram_blwl_1290_configbus1;
wire [1290:1290] sram_blwl_1290_configbus0_b;
assign sram_blwl_1290_configbus0[1290:1290] = sram_blwl_bl[1290:1290] ;
assign sram_blwl_1290_configbus1[1290:1290] = sram_blwl_wl[1290:1290] ;
assign sram_blwl_1290_configbus0_b[1290:1290] = sram_blwl_blb[1290:1290] ;
sram6T_blwl sram_blwl_1290_ (sram_blwl_out[1290], sram_blwl_out[1290], sram_blwl_outb[1290], sram_blwl_1290_configbus0[1290:1290], sram_blwl_1290_configbus1[1290:1290] , sram_blwl_1290_configbus0_b[1290:1290] );
wire [1291:1291] sram_blwl_1291_configbus0;
wire [1291:1291] sram_blwl_1291_configbus1;
wire [1291:1291] sram_blwl_1291_configbus0_b;
assign sram_blwl_1291_configbus0[1291:1291] = sram_blwl_bl[1291:1291] ;
assign sram_blwl_1291_configbus1[1291:1291] = sram_blwl_wl[1291:1291] ;
assign sram_blwl_1291_configbus0_b[1291:1291] = sram_blwl_blb[1291:1291] ;
sram6T_blwl sram_blwl_1291_ (sram_blwl_out[1291], sram_blwl_out[1291], sram_blwl_outb[1291], sram_blwl_1291_configbus0[1291:1291], sram_blwl_1291_configbus1[1291:1291] , sram_blwl_1291_configbus0_b[1291:1291] );
wire [1292:1292] sram_blwl_1292_configbus0;
wire [1292:1292] sram_blwl_1292_configbus1;
wire [1292:1292] sram_blwl_1292_configbus0_b;
assign sram_blwl_1292_configbus0[1292:1292] = sram_blwl_bl[1292:1292] ;
assign sram_blwl_1292_configbus1[1292:1292] = sram_blwl_wl[1292:1292] ;
assign sram_blwl_1292_configbus0_b[1292:1292] = sram_blwl_blb[1292:1292] ;
sram6T_blwl sram_blwl_1292_ (sram_blwl_out[1292], sram_blwl_out[1292], sram_blwl_outb[1292], sram_blwl_1292_configbus0[1292:1292], sram_blwl_1292_configbus1[1292:1292] , sram_blwl_1292_configbus0_b[1292:1292] );
wire [1293:1293] sram_blwl_1293_configbus0;
wire [1293:1293] sram_blwl_1293_configbus1;
wire [1293:1293] sram_blwl_1293_configbus0_b;
assign sram_blwl_1293_configbus0[1293:1293] = sram_blwl_bl[1293:1293] ;
assign sram_blwl_1293_configbus1[1293:1293] = sram_blwl_wl[1293:1293] ;
assign sram_blwl_1293_configbus0_b[1293:1293] = sram_blwl_blb[1293:1293] ;
sram6T_blwl sram_blwl_1293_ (sram_blwl_out[1293], sram_blwl_out[1293], sram_blwl_outb[1293], sram_blwl_1293_configbus0[1293:1293], sram_blwl_1293_configbus1[1293:1293] , sram_blwl_1293_configbus0_b[1293:1293] );
wire [1294:1294] sram_blwl_1294_configbus0;
wire [1294:1294] sram_blwl_1294_configbus1;
wire [1294:1294] sram_blwl_1294_configbus0_b;
assign sram_blwl_1294_configbus0[1294:1294] = sram_blwl_bl[1294:1294] ;
assign sram_blwl_1294_configbus1[1294:1294] = sram_blwl_wl[1294:1294] ;
assign sram_blwl_1294_configbus0_b[1294:1294] = sram_blwl_blb[1294:1294] ;
sram6T_blwl sram_blwl_1294_ (sram_blwl_out[1294], sram_blwl_out[1294], sram_blwl_outb[1294], sram_blwl_1294_configbus0[1294:1294], sram_blwl_1294_configbus1[1294:1294] , sram_blwl_1294_configbus0_b[1294:1294] );
wire [1295:1295] sram_blwl_1295_configbus0;
wire [1295:1295] sram_blwl_1295_configbus1;
wire [1295:1295] sram_blwl_1295_configbus0_b;
assign sram_blwl_1295_configbus0[1295:1295] = sram_blwl_bl[1295:1295] ;
assign sram_blwl_1295_configbus1[1295:1295] = sram_blwl_wl[1295:1295] ;
assign sram_blwl_1295_configbus0_b[1295:1295] = sram_blwl_blb[1295:1295] ;
sram6T_blwl sram_blwl_1295_ (sram_blwl_out[1295], sram_blwl_out[1295], sram_blwl_outb[1295], sram_blwl_1295_configbus0[1295:1295], sram_blwl_1295_configbus1[1295:1295] , sram_blwl_1295_configbus0_b[1295:1295] );
wire [1296:1296] sram_blwl_1296_configbus0;
wire [1296:1296] sram_blwl_1296_configbus1;
wire [1296:1296] sram_blwl_1296_configbus0_b;
assign sram_blwl_1296_configbus0[1296:1296] = sram_blwl_bl[1296:1296] ;
assign sram_blwl_1296_configbus1[1296:1296] = sram_blwl_wl[1296:1296] ;
assign sram_blwl_1296_configbus0_b[1296:1296] = sram_blwl_blb[1296:1296] ;
sram6T_blwl sram_blwl_1296_ (sram_blwl_out[1296], sram_blwl_out[1296], sram_blwl_outb[1296], sram_blwl_1296_configbus0[1296:1296], sram_blwl_1296_configbus1[1296:1296] , sram_blwl_1296_configbus0_b[1296:1296] );
wire [1297:1297] sram_blwl_1297_configbus0;
wire [1297:1297] sram_blwl_1297_configbus1;
wire [1297:1297] sram_blwl_1297_configbus0_b;
assign sram_blwl_1297_configbus0[1297:1297] = sram_blwl_bl[1297:1297] ;
assign sram_blwl_1297_configbus1[1297:1297] = sram_blwl_wl[1297:1297] ;
assign sram_blwl_1297_configbus0_b[1297:1297] = sram_blwl_blb[1297:1297] ;
sram6T_blwl sram_blwl_1297_ (sram_blwl_out[1297], sram_blwl_out[1297], sram_blwl_outb[1297], sram_blwl_1297_configbus0[1297:1297], sram_blwl_1297_configbus1[1297:1297] , sram_blwl_1297_configbus0_b[1297:1297] );
wire [1298:1298] sram_blwl_1298_configbus0;
wire [1298:1298] sram_blwl_1298_configbus1;
wire [1298:1298] sram_blwl_1298_configbus0_b;
assign sram_blwl_1298_configbus0[1298:1298] = sram_blwl_bl[1298:1298] ;
assign sram_blwl_1298_configbus1[1298:1298] = sram_blwl_wl[1298:1298] ;
assign sram_blwl_1298_configbus0_b[1298:1298] = sram_blwl_blb[1298:1298] ;
sram6T_blwl sram_blwl_1298_ (sram_blwl_out[1298], sram_blwl_out[1298], sram_blwl_outb[1298], sram_blwl_1298_configbus0[1298:1298], sram_blwl_1298_configbus1[1298:1298] , sram_blwl_1298_configbus0_b[1298:1298] );
wire [1299:1299] sram_blwl_1299_configbus0;
wire [1299:1299] sram_blwl_1299_configbus1;
wire [1299:1299] sram_blwl_1299_configbus0_b;
assign sram_blwl_1299_configbus0[1299:1299] = sram_blwl_bl[1299:1299] ;
assign sram_blwl_1299_configbus1[1299:1299] = sram_blwl_wl[1299:1299] ;
assign sram_blwl_1299_configbus0_b[1299:1299] = sram_blwl_blb[1299:1299] ;
sram6T_blwl sram_blwl_1299_ (sram_blwl_out[1299], sram_blwl_out[1299], sram_blwl_outb[1299], sram_blwl_1299_configbus0[1299:1299], sram_blwl_1299_configbus1[1299:1299] , sram_blwl_1299_configbus0_b[1299:1299] );
wire [1300:1300] sram_blwl_1300_configbus0;
wire [1300:1300] sram_blwl_1300_configbus1;
wire [1300:1300] sram_blwl_1300_configbus0_b;
assign sram_blwl_1300_configbus0[1300:1300] = sram_blwl_bl[1300:1300] ;
assign sram_blwl_1300_configbus1[1300:1300] = sram_blwl_wl[1300:1300] ;
assign sram_blwl_1300_configbus0_b[1300:1300] = sram_blwl_blb[1300:1300] ;
sram6T_blwl sram_blwl_1300_ (sram_blwl_out[1300], sram_blwl_out[1300], sram_blwl_outb[1300], sram_blwl_1300_configbus0[1300:1300], sram_blwl_1300_configbus1[1300:1300] , sram_blwl_1300_configbus0_b[1300:1300] );
wire [1301:1301] sram_blwl_1301_configbus0;
wire [1301:1301] sram_blwl_1301_configbus1;
wire [1301:1301] sram_blwl_1301_configbus0_b;
assign sram_blwl_1301_configbus0[1301:1301] = sram_blwl_bl[1301:1301] ;
assign sram_blwl_1301_configbus1[1301:1301] = sram_blwl_wl[1301:1301] ;
assign sram_blwl_1301_configbus0_b[1301:1301] = sram_blwl_blb[1301:1301] ;
sram6T_blwl sram_blwl_1301_ (sram_blwl_out[1301], sram_blwl_out[1301], sram_blwl_outb[1301], sram_blwl_1301_configbus0[1301:1301], sram_blwl_1301_configbus1[1301:1301] , sram_blwl_1301_configbus0_b[1301:1301] );
wire [1302:1302] sram_blwl_1302_configbus0;
wire [1302:1302] sram_blwl_1302_configbus1;
wire [1302:1302] sram_blwl_1302_configbus0_b;
assign sram_blwl_1302_configbus0[1302:1302] = sram_blwl_bl[1302:1302] ;
assign sram_blwl_1302_configbus1[1302:1302] = sram_blwl_wl[1302:1302] ;
assign sram_blwl_1302_configbus0_b[1302:1302] = sram_blwl_blb[1302:1302] ;
sram6T_blwl sram_blwl_1302_ (sram_blwl_out[1302], sram_blwl_out[1302], sram_blwl_outb[1302], sram_blwl_1302_configbus0[1302:1302], sram_blwl_1302_configbus1[1302:1302] , sram_blwl_1302_configbus0_b[1302:1302] );
wire [1303:1303] sram_blwl_1303_configbus0;
wire [1303:1303] sram_blwl_1303_configbus1;
wire [1303:1303] sram_blwl_1303_configbus0_b;
assign sram_blwl_1303_configbus0[1303:1303] = sram_blwl_bl[1303:1303] ;
assign sram_blwl_1303_configbus1[1303:1303] = sram_blwl_wl[1303:1303] ;
assign sram_blwl_1303_configbus0_b[1303:1303] = sram_blwl_blb[1303:1303] ;
sram6T_blwl sram_blwl_1303_ (sram_blwl_out[1303], sram_blwl_out[1303], sram_blwl_outb[1303], sram_blwl_1303_configbus0[1303:1303], sram_blwl_1303_configbus1[1303:1303] , sram_blwl_1303_configbus0_b[1303:1303] );
wire [1304:1304] sram_blwl_1304_configbus0;
wire [1304:1304] sram_blwl_1304_configbus1;
wire [1304:1304] sram_blwl_1304_configbus0_b;
assign sram_blwl_1304_configbus0[1304:1304] = sram_blwl_bl[1304:1304] ;
assign sram_blwl_1304_configbus1[1304:1304] = sram_blwl_wl[1304:1304] ;
assign sram_blwl_1304_configbus0_b[1304:1304] = sram_blwl_blb[1304:1304] ;
sram6T_blwl sram_blwl_1304_ (sram_blwl_out[1304], sram_blwl_out[1304], sram_blwl_outb[1304], sram_blwl_1304_configbus0[1304:1304], sram_blwl_1304_configbus1[1304:1304] , sram_blwl_1304_configbus0_b[1304:1304] );
wire [1305:1305] sram_blwl_1305_configbus0;
wire [1305:1305] sram_blwl_1305_configbus1;
wire [1305:1305] sram_blwl_1305_configbus0_b;
assign sram_blwl_1305_configbus0[1305:1305] = sram_blwl_bl[1305:1305] ;
assign sram_blwl_1305_configbus1[1305:1305] = sram_blwl_wl[1305:1305] ;
assign sram_blwl_1305_configbus0_b[1305:1305] = sram_blwl_blb[1305:1305] ;
sram6T_blwl sram_blwl_1305_ (sram_blwl_out[1305], sram_blwl_out[1305], sram_blwl_outb[1305], sram_blwl_1305_configbus0[1305:1305], sram_blwl_1305_configbus1[1305:1305] , sram_blwl_1305_configbus0_b[1305:1305] );
wire [1306:1306] sram_blwl_1306_configbus0;
wire [1306:1306] sram_blwl_1306_configbus1;
wire [1306:1306] sram_blwl_1306_configbus0_b;
assign sram_blwl_1306_configbus0[1306:1306] = sram_blwl_bl[1306:1306] ;
assign sram_blwl_1306_configbus1[1306:1306] = sram_blwl_wl[1306:1306] ;
assign sram_blwl_1306_configbus0_b[1306:1306] = sram_blwl_blb[1306:1306] ;
sram6T_blwl sram_blwl_1306_ (sram_blwl_out[1306], sram_blwl_out[1306], sram_blwl_outb[1306], sram_blwl_1306_configbus0[1306:1306], sram_blwl_1306_configbus1[1306:1306] , sram_blwl_1306_configbus0_b[1306:1306] );
wire [1307:1307] sram_blwl_1307_configbus0;
wire [1307:1307] sram_blwl_1307_configbus1;
wire [1307:1307] sram_blwl_1307_configbus0_b;
assign sram_blwl_1307_configbus0[1307:1307] = sram_blwl_bl[1307:1307] ;
assign sram_blwl_1307_configbus1[1307:1307] = sram_blwl_wl[1307:1307] ;
assign sram_blwl_1307_configbus0_b[1307:1307] = sram_blwl_blb[1307:1307] ;
sram6T_blwl sram_blwl_1307_ (sram_blwl_out[1307], sram_blwl_out[1307], sram_blwl_outb[1307], sram_blwl_1307_configbus0[1307:1307], sram_blwl_1307_configbus1[1307:1307] , sram_blwl_1307_configbus0_b[1307:1307] );
wire [1308:1308] sram_blwl_1308_configbus0;
wire [1308:1308] sram_blwl_1308_configbus1;
wire [1308:1308] sram_blwl_1308_configbus0_b;
assign sram_blwl_1308_configbus0[1308:1308] = sram_blwl_bl[1308:1308] ;
assign sram_blwl_1308_configbus1[1308:1308] = sram_blwl_wl[1308:1308] ;
assign sram_blwl_1308_configbus0_b[1308:1308] = sram_blwl_blb[1308:1308] ;
sram6T_blwl sram_blwl_1308_ (sram_blwl_out[1308], sram_blwl_out[1308], sram_blwl_outb[1308], sram_blwl_1308_configbus0[1308:1308], sram_blwl_1308_configbus1[1308:1308] , sram_blwl_1308_configbus0_b[1308:1308] );
wire [1309:1309] sram_blwl_1309_configbus0;
wire [1309:1309] sram_blwl_1309_configbus1;
wire [1309:1309] sram_blwl_1309_configbus0_b;
assign sram_blwl_1309_configbus0[1309:1309] = sram_blwl_bl[1309:1309] ;
assign sram_blwl_1309_configbus1[1309:1309] = sram_blwl_wl[1309:1309] ;
assign sram_blwl_1309_configbus0_b[1309:1309] = sram_blwl_blb[1309:1309] ;
sram6T_blwl sram_blwl_1309_ (sram_blwl_out[1309], sram_blwl_out[1309], sram_blwl_outb[1309], sram_blwl_1309_configbus0[1309:1309], sram_blwl_1309_configbus1[1309:1309] , sram_blwl_1309_configbus0_b[1309:1309] );
wire [1310:1310] sram_blwl_1310_configbus0;
wire [1310:1310] sram_blwl_1310_configbus1;
wire [1310:1310] sram_blwl_1310_configbus0_b;
assign sram_blwl_1310_configbus0[1310:1310] = sram_blwl_bl[1310:1310] ;
assign sram_blwl_1310_configbus1[1310:1310] = sram_blwl_wl[1310:1310] ;
assign sram_blwl_1310_configbus0_b[1310:1310] = sram_blwl_blb[1310:1310] ;
sram6T_blwl sram_blwl_1310_ (sram_blwl_out[1310], sram_blwl_out[1310], sram_blwl_outb[1310], sram_blwl_1310_configbus0[1310:1310], sram_blwl_1310_configbus1[1310:1310] , sram_blwl_1310_configbus0_b[1310:1310] );
wire [1311:1311] sram_blwl_1311_configbus0;
wire [1311:1311] sram_blwl_1311_configbus1;
wire [1311:1311] sram_blwl_1311_configbus0_b;
assign sram_blwl_1311_configbus0[1311:1311] = sram_blwl_bl[1311:1311] ;
assign sram_blwl_1311_configbus1[1311:1311] = sram_blwl_wl[1311:1311] ;
assign sram_blwl_1311_configbus0_b[1311:1311] = sram_blwl_blb[1311:1311] ;
sram6T_blwl sram_blwl_1311_ (sram_blwl_out[1311], sram_blwl_out[1311], sram_blwl_outb[1311], sram_blwl_1311_configbus0[1311:1311], sram_blwl_1311_configbus1[1311:1311] , sram_blwl_1311_configbus0_b[1311:1311] );
wire [1312:1312] sram_blwl_1312_configbus0;
wire [1312:1312] sram_blwl_1312_configbus1;
wire [1312:1312] sram_blwl_1312_configbus0_b;
assign sram_blwl_1312_configbus0[1312:1312] = sram_blwl_bl[1312:1312] ;
assign sram_blwl_1312_configbus1[1312:1312] = sram_blwl_wl[1312:1312] ;
assign sram_blwl_1312_configbus0_b[1312:1312] = sram_blwl_blb[1312:1312] ;
sram6T_blwl sram_blwl_1312_ (sram_blwl_out[1312], sram_blwl_out[1312], sram_blwl_outb[1312], sram_blwl_1312_configbus0[1312:1312], sram_blwl_1312_configbus1[1312:1312] , sram_blwl_1312_configbus0_b[1312:1312] );
wire [1313:1313] sram_blwl_1313_configbus0;
wire [1313:1313] sram_blwl_1313_configbus1;
wire [1313:1313] sram_blwl_1313_configbus0_b;
assign sram_blwl_1313_configbus0[1313:1313] = sram_blwl_bl[1313:1313] ;
assign sram_blwl_1313_configbus1[1313:1313] = sram_blwl_wl[1313:1313] ;
assign sram_blwl_1313_configbus0_b[1313:1313] = sram_blwl_blb[1313:1313] ;
sram6T_blwl sram_blwl_1313_ (sram_blwl_out[1313], sram_blwl_out[1313], sram_blwl_outb[1313], sram_blwl_1313_configbus0[1313:1313], sram_blwl_1313_configbus1[1313:1313] , sram_blwl_1313_configbus0_b[1313:1313] );
wire [1314:1314] sram_blwl_1314_configbus0;
wire [1314:1314] sram_blwl_1314_configbus1;
wire [1314:1314] sram_blwl_1314_configbus0_b;
assign sram_blwl_1314_configbus0[1314:1314] = sram_blwl_bl[1314:1314] ;
assign sram_blwl_1314_configbus1[1314:1314] = sram_blwl_wl[1314:1314] ;
assign sram_blwl_1314_configbus0_b[1314:1314] = sram_blwl_blb[1314:1314] ;
sram6T_blwl sram_blwl_1314_ (sram_blwl_out[1314], sram_blwl_out[1314], sram_blwl_outb[1314], sram_blwl_1314_configbus0[1314:1314], sram_blwl_1314_configbus1[1314:1314] , sram_blwl_1314_configbus0_b[1314:1314] );
wire [1315:1315] sram_blwl_1315_configbus0;
wire [1315:1315] sram_blwl_1315_configbus1;
wire [1315:1315] sram_blwl_1315_configbus0_b;
assign sram_blwl_1315_configbus0[1315:1315] = sram_blwl_bl[1315:1315] ;
assign sram_blwl_1315_configbus1[1315:1315] = sram_blwl_wl[1315:1315] ;
assign sram_blwl_1315_configbus0_b[1315:1315] = sram_blwl_blb[1315:1315] ;
sram6T_blwl sram_blwl_1315_ (sram_blwl_out[1315], sram_blwl_out[1315], sram_blwl_outb[1315], sram_blwl_1315_configbus0[1315:1315], sram_blwl_1315_configbus1[1315:1315] , sram_blwl_1315_configbus0_b[1315:1315] );
wire [1316:1316] sram_blwl_1316_configbus0;
wire [1316:1316] sram_blwl_1316_configbus1;
wire [1316:1316] sram_blwl_1316_configbus0_b;
assign sram_blwl_1316_configbus0[1316:1316] = sram_blwl_bl[1316:1316] ;
assign sram_blwl_1316_configbus1[1316:1316] = sram_blwl_wl[1316:1316] ;
assign sram_blwl_1316_configbus0_b[1316:1316] = sram_blwl_blb[1316:1316] ;
sram6T_blwl sram_blwl_1316_ (sram_blwl_out[1316], sram_blwl_out[1316], sram_blwl_outb[1316], sram_blwl_1316_configbus0[1316:1316], sram_blwl_1316_configbus1[1316:1316] , sram_blwl_1316_configbus0_b[1316:1316] );
wire [1317:1317] sram_blwl_1317_configbus0;
wire [1317:1317] sram_blwl_1317_configbus1;
wire [1317:1317] sram_blwl_1317_configbus0_b;
assign sram_blwl_1317_configbus0[1317:1317] = sram_blwl_bl[1317:1317] ;
assign sram_blwl_1317_configbus1[1317:1317] = sram_blwl_wl[1317:1317] ;
assign sram_blwl_1317_configbus0_b[1317:1317] = sram_blwl_blb[1317:1317] ;
sram6T_blwl sram_blwl_1317_ (sram_blwl_out[1317], sram_blwl_out[1317], sram_blwl_outb[1317], sram_blwl_1317_configbus0[1317:1317], sram_blwl_1317_configbus1[1317:1317] , sram_blwl_1317_configbus0_b[1317:1317] );
wire [1318:1318] sram_blwl_1318_configbus0;
wire [1318:1318] sram_blwl_1318_configbus1;
wire [1318:1318] sram_blwl_1318_configbus0_b;
assign sram_blwl_1318_configbus0[1318:1318] = sram_blwl_bl[1318:1318] ;
assign sram_blwl_1318_configbus1[1318:1318] = sram_blwl_wl[1318:1318] ;
assign sram_blwl_1318_configbus0_b[1318:1318] = sram_blwl_blb[1318:1318] ;
sram6T_blwl sram_blwl_1318_ (sram_blwl_out[1318], sram_blwl_out[1318], sram_blwl_outb[1318], sram_blwl_1318_configbus0[1318:1318], sram_blwl_1318_configbus1[1318:1318] , sram_blwl_1318_configbus0_b[1318:1318] );
wire [1319:1319] sram_blwl_1319_configbus0;
wire [1319:1319] sram_blwl_1319_configbus1;
wire [1319:1319] sram_blwl_1319_configbus0_b;
assign sram_blwl_1319_configbus0[1319:1319] = sram_blwl_bl[1319:1319] ;
assign sram_blwl_1319_configbus1[1319:1319] = sram_blwl_wl[1319:1319] ;
assign sram_blwl_1319_configbus0_b[1319:1319] = sram_blwl_blb[1319:1319] ;
sram6T_blwl sram_blwl_1319_ (sram_blwl_out[1319], sram_blwl_out[1319], sram_blwl_outb[1319], sram_blwl_1319_configbus0[1319:1319], sram_blwl_1319_configbus1[1319:1319] , sram_blwl_1319_configbus0_b[1319:1319] );
wire [1320:1320] sram_blwl_1320_configbus0;
wire [1320:1320] sram_blwl_1320_configbus1;
wire [1320:1320] sram_blwl_1320_configbus0_b;
assign sram_blwl_1320_configbus0[1320:1320] = sram_blwl_bl[1320:1320] ;
assign sram_blwl_1320_configbus1[1320:1320] = sram_blwl_wl[1320:1320] ;
assign sram_blwl_1320_configbus0_b[1320:1320] = sram_blwl_blb[1320:1320] ;
sram6T_blwl sram_blwl_1320_ (sram_blwl_out[1320], sram_blwl_out[1320], sram_blwl_outb[1320], sram_blwl_1320_configbus0[1320:1320], sram_blwl_1320_configbus1[1320:1320] , sram_blwl_1320_configbus0_b[1320:1320] );
wire [1321:1321] sram_blwl_1321_configbus0;
wire [1321:1321] sram_blwl_1321_configbus1;
wire [1321:1321] sram_blwl_1321_configbus0_b;
assign sram_blwl_1321_configbus0[1321:1321] = sram_blwl_bl[1321:1321] ;
assign sram_blwl_1321_configbus1[1321:1321] = sram_blwl_wl[1321:1321] ;
assign sram_blwl_1321_configbus0_b[1321:1321] = sram_blwl_blb[1321:1321] ;
sram6T_blwl sram_blwl_1321_ (sram_blwl_out[1321], sram_blwl_out[1321], sram_blwl_outb[1321], sram_blwl_1321_configbus0[1321:1321], sram_blwl_1321_configbus1[1321:1321] , sram_blwl_1321_configbus0_b[1321:1321] );
wire [1322:1322] sram_blwl_1322_configbus0;
wire [1322:1322] sram_blwl_1322_configbus1;
wire [1322:1322] sram_blwl_1322_configbus0_b;
assign sram_blwl_1322_configbus0[1322:1322] = sram_blwl_bl[1322:1322] ;
assign sram_blwl_1322_configbus1[1322:1322] = sram_blwl_wl[1322:1322] ;
assign sram_blwl_1322_configbus0_b[1322:1322] = sram_blwl_blb[1322:1322] ;
sram6T_blwl sram_blwl_1322_ (sram_blwl_out[1322], sram_blwl_out[1322], sram_blwl_outb[1322], sram_blwl_1322_configbus0[1322:1322], sram_blwl_1322_configbus1[1322:1322] , sram_blwl_1322_configbus0_b[1322:1322] );
wire [1323:1323] sram_blwl_1323_configbus0;
wire [1323:1323] sram_blwl_1323_configbus1;
wire [1323:1323] sram_blwl_1323_configbus0_b;
assign sram_blwl_1323_configbus0[1323:1323] = sram_blwl_bl[1323:1323] ;
assign sram_blwl_1323_configbus1[1323:1323] = sram_blwl_wl[1323:1323] ;
assign sram_blwl_1323_configbus0_b[1323:1323] = sram_blwl_blb[1323:1323] ;
sram6T_blwl sram_blwl_1323_ (sram_blwl_out[1323], sram_blwl_out[1323], sram_blwl_outb[1323], sram_blwl_1323_configbus0[1323:1323], sram_blwl_1323_configbus1[1323:1323] , sram_blwl_1323_configbus0_b[1323:1323] );
wire [1324:1324] sram_blwl_1324_configbus0;
wire [1324:1324] sram_blwl_1324_configbus1;
wire [1324:1324] sram_blwl_1324_configbus0_b;
assign sram_blwl_1324_configbus0[1324:1324] = sram_blwl_bl[1324:1324] ;
assign sram_blwl_1324_configbus1[1324:1324] = sram_blwl_wl[1324:1324] ;
assign sram_blwl_1324_configbus0_b[1324:1324] = sram_blwl_blb[1324:1324] ;
sram6T_blwl sram_blwl_1324_ (sram_blwl_out[1324], sram_blwl_out[1324], sram_blwl_outb[1324], sram_blwl_1324_configbus0[1324:1324], sram_blwl_1324_configbus1[1324:1324] , sram_blwl_1324_configbus0_b[1324:1324] );
wire [1325:1325] sram_blwl_1325_configbus0;
wire [1325:1325] sram_blwl_1325_configbus1;
wire [1325:1325] sram_blwl_1325_configbus0_b;
assign sram_blwl_1325_configbus0[1325:1325] = sram_blwl_bl[1325:1325] ;
assign sram_blwl_1325_configbus1[1325:1325] = sram_blwl_wl[1325:1325] ;
assign sram_blwl_1325_configbus0_b[1325:1325] = sram_blwl_blb[1325:1325] ;
sram6T_blwl sram_blwl_1325_ (sram_blwl_out[1325], sram_blwl_out[1325], sram_blwl_outb[1325], sram_blwl_1325_configbus0[1325:1325], sram_blwl_1325_configbus1[1325:1325] , sram_blwl_1325_configbus0_b[1325:1325] );
wire [1326:1326] sram_blwl_1326_configbus0;
wire [1326:1326] sram_blwl_1326_configbus1;
wire [1326:1326] sram_blwl_1326_configbus0_b;
assign sram_blwl_1326_configbus0[1326:1326] = sram_blwl_bl[1326:1326] ;
assign sram_blwl_1326_configbus1[1326:1326] = sram_blwl_wl[1326:1326] ;
assign sram_blwl_1326_configbus0_b[1326:1326] = sram_blwl_blb[1326:1326] ;
sram6T_blwl sram_blwl_1326_ (sram_blwl_out[1326], sram_blwl_out[1326], sram_blwl_outb[1326], sram_blwl_1326_configbus0[1326:1326], sram_blwl_1326_configbus1[1326:1326] , sram_blwl_1326_configbus0_b[1326:1326] );
wire [1327:1327] sram_blwl_1327_configbus0;
wire [1327:1327] sram_blwl_1327_configbus1;
wire [1327:1327] sram_blwl_1327_configbus0_b;
assign sram_blwl_1327_configbus0[1327:1327] = sram_blwl_bl[1327:1327] ;
assign sram_blwl_1327_configbus1[1327:1327] = sram_blwl_wl[1327:1327] ;
assign sram_blwl_1327_configbus0_b[1327:1327] = sram_blwl_blb[1327:1327] ;
sram6T_blwl sram_blwl_1327_ (sram_blwl_out[1327], sram_blwl_out[1327], sram_blwl_outb[1327], sram_blwl_1327_configbus0[1327:1327], sram_blwl_1327_configbus1[1327:1327] , sram_blwl_1327_configbus0_b[1327:1327] );
wire [1328:1328] sram_blwl_1328_configbus0;
wire [1328:1328] sram_blwl_1328_configbus1;
wire [1328:1328] sram_blwl_1328_configbus0_b;
assign sram_blwl_1328_configbus0[1328:1328] = sram_blwl_bl[1328:1328] ;
assign sram_blwl_1328_configbus1[1328:1328] = sram_blwl_wl[1328:1328] ;
assign sram_blwl_1328_configbus0_b[1328:1328] = sram_blwl_blb[1328:1328] ;
sram6T_blwl sram_blwl_1328_ (sram_blwl_out[1328], sram_blwl_out[1328], sram_blwl_outb[1328], sram_blwl_1328_configbus0[1328:1328], sram_blwl_1328_configbus1[1328:1328] , sram_blwl_1328_configbus0_b[1328:1328] );
wire [1329:1329] sram_blwl_1329_configbus0;
wire [1329:1329] sram_blwl_1329_configbus1;
wire [1329:1329] sram_blwl_1329_configbus0_b;
assign sram_blwl_1329_configbus0[1329:1329] = sram_blwl_bl[1329:1329] ;
assign sram_blwl_1329_configbus1[1329:1329] = sram_blwl_wl[1329:1329] ;
assign sram_blwl_1329_configbus0_b[1329:1329] = sram_blwl_blb[1329:1329] ;
sram6T_blwl sram_blwl_1329_ (sram_blwl_out[1329], sram_blwl_out[1329], sram_blwl_outb[1329], sram_blwl_1329_configbus0[1329:1329], sram_blwl_1329_configbus1[1329:1329] , sram_blwl_1329_configbus0_b[1329:1329] );
wire [1330:1330] sram_blwl_1330_configbus0;
wire [1330:1330] sram_blwl_1330_configbus1;
wire [1330:1330] sram_blwl_1330_configbus0_b;
assign sram_blwl_1330_configbus0[1330:1330] = sram_blwl_bl[1330:1330] ;
assign sram_blwl_1330_configbus1[1330:1330] = sram_blwl_wl[1330:1330] ;
assign sram_blwl_1330_configbus0_b[1330:1330] = sram_blwl_blb[1330:1330] ;
sram6T_blwl sram_blwl_1330_ (sram_blwl_out[1330], sram_blwl_out[1330], sram_blwl_outb[1330], sram_blwl_1330_configbus0[1330:1330], sram_blwl_1330_configbus1[1330:1330] , sram_blwl_1330_configbus0_b[1330:1330] );
wire [1331:1331] sram_blwl_1331_configbus0;
wire [1331:1331] sram_blwl_1331_configbus1;
wire [1331:1331] sram_blwl_1331_configbus0_b;
assign sram_blwl_1331_configbus0[1331:1331] = sram_blwl_bl[1331:1331] ;
assign sram_blwl_1331_configbus1[1331:1331] = sram_blwl_wl[1331:1331] ;
assign sram_blwl_1331_configbus0_b[1331:1331] = sram_blwl_blb[1331:1331] ;
sram6T_blwl sram_blwl_1331_ (sram_blwl_out[1331], sram_blwl_out[1331], sram_blwl_outb[1331], sram_blwl_1331_configbus0[1331:1331], sram_blwl_1331_configbus1[1331:1331] , sram_blwl_1331_configbus0_b[1331:1331] );
wire [1332:1332] sram_blwl_1332_configbus0;
wire [1332:1332] sram_blwl_1332_configbus1;
wire [1332:1332] sram_blwl_1332_configbus0_b;
assign sram_blwl_1332_configbus0[1332:1332] = sram_blwl_bl[1332:1332] ;
assign sram_blwl_1332_configbus1[1332:1332] = sram_blwl_wl[1332:1332] ;
assign sram_blwl_1332_configbus0_b[1332:1332] = sram_blwl_blb[1332:1332] ;
sram6T_blwl sram_blwl_1332_ (sram_blwl_out[1332], sram_blwl_out[1332], sram_blwl_outb[1332], sram_blwl_1332_configbus0[1332:1332], sram_blwl_1332_configbus1[1332:1332] , sram_blwl_1332_configbus0_b[1332:1332] );
wire [1333:1333] sram_blwl_1333_configbus0;
wire [1333:1333] sram_blwl_1333_configbus1;
wire [1333:1333] sram_blwl_1333_configbus0_b;
assign sram_blwl_1333_configbus0[1333:1333] = sram_blwl_bl[1333:1333] ;
assign sram_blwl_1333_configbus1[1333:1333] = sram_blwl_wl[1333:1333] ;
assign sram_blwl_1333_configbus0_b[1333:1333] = sram_blwl_blb[1333:1333] ;
sram6T_blwl sram_blwl_1333_ (sram_blwl_out[1333], sram_blwl_out[1333], sram_blwl_outb[1333], sram_blwl_1333_configbus0[1333:1333], sram_blwl_1333_configbus1[1333:1333] , sram_blwl_1333_configbus0_b[1333:1333] );
wire [1334:1334] sram_blwl_1334_configbus0;
wire [1334:1334] sram_blwl_1334_configbus1;
wire [1334:1334] sram_blwl_1334_configbus0_b;
assign sram_blwl_1334_configbus0[1334:1334] = sram_blwl_bl[1334:1334] ;
assign sram_blwl_1334_configbus1[1334:1334] = sram_blwl_wl[1334:1334] ;
assign sram_blwl_1334_configbus0_b[1334:1334] = sram_blwl_blb[1334:1334] ;
sram6T_blwl sram_blwl_1334_ (sram_blwl_out[1334], sram_blwl_out[1334], sram_blwl_outb[1334], sram_blwl_1334_configbus0[1334:1334], sram_blwl_1334_configbus1[1334:1334] , sram_blwl_1334_configbus0_b[1334:1334] );
wire [1335:1335] sram_blwl_1335_configbus0;
wire [1335:1335] sram_blwl_1335_configbus1;
wire [1335:1335] sram_blwl_1335_configbus0_b;
assign sram_blwl_1335_configbus0[1335:1335] = sram_blwl_bl[1335:1335] ;
assign sram_blwl_1335_configbus1[1335:1335] = sram_blwl_wl[1335:1335] ;
assign sram_blwl_1335_configbus0_b[1335:1335] = sram_blwl_blb[1335:1335] ;
sram6T_blwl sram_blwl_1335_ (sram_blwl_out[1335], sram_blwl_out[1335], sram_blwl_outb[1335], sram_blwl_1335_configbus0[1335:1335], sram_blwl_1335_configbus1[1335:1335] , sram_blwl_1335_configbus0_b[1335:1335] );
wire [1336:1336] sram_blwl_1336_configbus0;
wire [1336:1336] sram_blwl_1336_configbus1;
wire [1336:1336] sram_blwl_1336_configbus0_b;
assign sram_blwl_1336_configbus0[1336:1336] = sram_blwl_bl[1336:1336] ;
assign sram_blwl_1336_configbus1[1336:1336] = sram_blwl_wl[1336:1336] ;
assign sram_blwl_1336_configbus0_b[1336:1336] = sram_blwl_blb[1336:1336] ;
sram6T_blwl sram_blwl_1336_ (sram_blwl_out[1336], sram_blwl_out[1336], sram_blwl_outb[1336], sram_blwl_1336_configbus0[1336:1336], sram_blwl_1336_configbus1[1336:1336] , sram_blwl_1336_configbus0_b[1336:1336] );
wire [1337:1337] sram_blwl_1337_configbus0;
wire [1337:1337] sram_blwl_1337_configbus1;
wire [1337:1337] sram_blwl_1337_configbus0_b;
assign sram_blwl_1337_configbus0[1337:1337] = sram_blwl_bl[1337:1337] ;
assign sram_blwl_1337_configbus1[1337:1337] = sram_blwl_wl[1337:1337] ;
assign sram_blwl_1337_configbus0_b[1337:1337] = sram_blwl_blb[1337:1337] ;
sram6T_blwl sram_blwl_1337_ (sram_blwl_out[1337], sram_blwl_out[1337], sram_blwl_outb[1337], sram_blwl_1337_configbus0[1337:1337], sram_blwl_1337_configbus1[1337:1337] , sram_blwl_1337_configbus0_b[1337:1337] );
wire [1338:1338] sram_blwl_1338_configbus0;
wire [1338:1338] sram_blwl_1338_configbus1;
wire [1338:1338] sram_blwl_1338_configbus0_b;
assign sram_blwl_1338_configbus0[1338:1338] = sram_blwl_bl[1338:1338] ;
assign sram_blwl_1338_configbus1[1338:1338] = sram_blwl_wl[1338:1338] ;
assign sram_blwl_1338_configbus0_b[1338:1338] = sram_blwl_blb[1338:1338] ;
sram6T_blwl sram_blwl_1338_ (sram_blwl_out[1338], sram_blwl_out[1338], sram_blwl_outb[1338], sram_blwl_1338_configbus0[1338:1338], sram_blwl_1338_configbus1[1338:1338] , sram_blwl_1338_configbus0_b[1338:1338] );
wire [1339:1339] sram_blwl_1339_configbus0;
wire [1339:1339] sram_blwl_1339_configbus1;
wire [1339:1339] sram_blwl_1339_configbus0_b;
assign sram_blwl_1339_configbus0[1339:1339] = sram_blwl_bl[1339:1339] ;
assign sram_blwl_1339_configbus1[1339:1339] = sram_blwl_wl[1339:1339] ;
assign sram_blwl_1339_configbus0_b[1339:1339] = sram_blwl_blb[1339:1339] ;
sram6T_blwl sram_blwl_1339_ (sram_blwl_out[1339], sram_blwl_out[1339], sram_blwl_outb[1339], sram_blwl_1339_configbus0[1339:1339], sram_blwl_1339_configbus1[1339:1339] , sram_blwl_1339_configbus0_b[1339:1339] );
endmodule
//----- END LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----

//----- Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----
module grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
input [0:0] Set,
input [0:0] Reset,
input [0:0] clk
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
input wire ff_0___D_0_,
output wire ff_0___Q_0_);
static_dff dff_4_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
endmodule
//----- END Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6_ -----
module grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_ble6___in_0_,
input wire mode_ble6___in_1_,
input wire mode_ble6___in_2_,
input wire mode_ble6___in_3_,
input wire mode_ble6___in_4_,
input wire mode_ble6___in_5_,
output wire mode_ble6___out_0_,
input wire mode_ble6___clk_0_,
input [1276:1340] sram_blwl_bl ,
input [1276:1340] sram_blwl_wl ,
input [1276:1340] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ lut6_0_ (
 lut6_0___in_0_,  lut6_0___in_1_,  lut6_0___in_2_,  lut6_0___in_3_,  lut6_0___in_4_,  lut6_0___in_5_,  lut6_0___out_0_,
sram_blwl_bl[1276:1339] ,
sram_blwl_wl[1276:1339] ,
sram_blwl_blb[1276:1339] );
grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ ff_0_ (
//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
wire [0:1] in_bus_mux_1level_tapbuf_size2_404_ ;
assign in_bus_mux_1level_tapbuf_size2_404_[0] = ff_0___Q_0_ ; 
assign in_bus_mux_1level_tapbuf_size2_404_[1] = lut6_0___out_0_ ; 
wire [1340:1340] mux_1level_tapbuf_size2_404_configbus0;
wire [1340:1340] mux_1level_tapbuf_size2_404_configbus1;
wire [1340:1340] mux_1level_tapbuf_size2_404_sram_blwl_out ;
wire [1340:1340] mux_1level_tapbuf_size2_404_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_404_configbus0[1340:1340] = sram_blwl_bl[1340:1340] ;
assign mux_1level_tapbuf_size2_404_configbus1[1340:1340] = sram_blwl_wl[1340:1340] ;
wire [1340:1340] mux_1level_tapbuf_size2_404_configbus0_b;
assign mux_1level_tapbuf_size2_404_configbus0_b[1340:1340] = sram_blwl_blb[1340:1340] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_404_ (in_bus_mux_1level_tapbuf_size2_404_, mode_ble6___out_0_, mux_1level_tapbuf_size2_404_sram_blwl_out[1340:1340] ,
mux_1level_tapbuf_size2_404_sram_blwl_outb[1340:1340] );
//----- SRAM bits for MUX[404], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_1340_ (mux_1level_tapbuf_size2_404_sram_blwl_out[1340:1340] ,mux_1level_tapbuf_size2_404_sram_blwl_out[1340:1340] ,mux_1level_tapbuf_size2_404_sram_blwl_outb[1340:1340] ,mux_1level_tapbuf_size2_404_configbus0[1340:1340], mux_1level_tapbuf_size2_404_configbus1[1340:1340] , mux_1level_tapbuf_size2_404_configbus0_b[1340:1340] );
direct_interc direct_interc_64_ (mode_ble6___in_0_, lut6_0___in_0_ );
direct_interc direct_interc_65_ (mode_ble6___in_1_, lut6_0___in_1_ );
direct_interc direct_interc_66_ (mode_ble6___in_2_, lut6_0___in_2_ );
direct_interc direct_interc_67_ (mode_ble6___in_3_, lut6_0___in_3_ );
direct_interc direct_interc_68_ (mode_ble6___in_4_, lut6_0___in_4_ );
direct_interc direct_interc_69_ (mode_ble6___in_5_, lut6_0___in_5_ );
direct_interc direct_interc_70_ (lut6_0___out_0_, ff_0___D_0_ );
direct_interc direct_interc_71_ (mode_ble6___clk_0_, ff_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6_ -----
module grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_n1_lut6___in_0_,
input wire mode_n1_lut6___in_1_,
input wire mode_n1_lut6___in_2_,
input wire mode_n1_lut6___in_3_,
input wire mode_n1_lut6___in_4_,
input wire mode_n1_lut6___in_5_,
output wire mode_n1_lut6___out_0_,
input wire mode_n1_lut6___clk_0_,
input [1276:1340] sram_blwl_bl ,
input [1276:1340] sram_blwl_wl ,
input [1276:1340] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6__ble6_0__mode_ble6_ ble6_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 ble6_0___in_0_,  ble6_0___in_1_,  ble6_0___in_2_,  ble6_0___in_3_,  ble6_0___in_4_,  ble6_0___in_5_,  ble6_0___out_0_,  ble6_0___clk_0_,
sram_blwl_bl[1276:1340] ,
sram_blwl_wl[1276:1340] ,
sram_blwl_blb[1276:1340] );
direct_interc direct_interc_72_ (ble6_0___out_0_, mode_n1_lut6___out_0_ );
direct_interc direct_interc_73_ (mode_n1_lut6___in_0_, ble6_0___in_0_ );
direct_interc direct_interc_74_ (mode_n1_lut6___in_1_, ble6_0___in_1_ );
direct_interc direct_interc_75_ (mode_n1_lut6___in_2_, ble6_0___in_2_ );
direct_interc direct_interc_76_ (mode_n1_lut6___in_3_, ble6_0___in_3_ );
direct_interc direct_interc_77_ (mode_n1_lut6___in_4_, ble6_0___in_4_ );
direct_interc direct_interc_78_ (mode_n1_lut6___in_5_, ble6_0___in_5_ );
direct_interc direct_interc_79_ (mode_n1_lut6___clk_0_, ble6_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6_ -----

//----- LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----
module grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ (
input wire lut6_0___in_0_,
input wire lut6_0___in_1_,
input wire lut6_0___in_2_,
input wire lut6_0___in_3_,
input wire lut6_0___in_4_,
input wire lut6_0___in_5_,
output wire lut6_0___out_0_,
input [1341:1404] sram_blwl_bl ,
input [1341:1404] sram_blwl_wl ,
input [1341:1404] sram_blwl_blb );
wire [0:5] lut6_0___in;
assign lut6_0___in[0] = lut6_0___in_0_;
assign lut6_0___in[1] = lut6_0___in_1_;
assign lut6_0___in[2] = lut6_0___in_2_;
assign lut6_0___in[3] = lut6_0___in_3_;
assign lut6_0___in[4] = lut6_0___in_4_;
assign lut6_0___in[5] = lut6_0___in_5_;
wire [0:0] lut6_0___out;
assign lut6_0___out_0_ = lut6_0___out[0];
wire [1341:1404] sram_blwl_out ;
wire [1341:1404] sram_blwl_outb ;
lut6 lut6_5_ (
//----- Input and output ports -----
 lut6_0___in[0:5] ,  lut6_0___out[0:0],//----- SRAM ports -----
sram_blwl_out[1341:1404] , sram_blwl_outb[1341:1404] );
//----- Truth Table for LUT[5], size=6. -----
//----- SRAM bits for LUT[5], size=6, num_sram=64. -----
//-----0000000000000000000000000000000000000000000000000000000000000000-----
wire [1341:1341] sram_blwl_1341_configbus0;
wire [1341:1341] sram_blwl_1341_configbus1;
wire [1341:1341] sram_blwl_1341_configbus0_b;
assign sram_blwl_1341_configbus0[1341:1341] = sram_blwl_bl[1341:1341] ;
assign sram_blwl_1341_configbus1[1341:1341] = sram_blwl_wl[1341:1341] ;
assign sram_blwl_1341_configbus0_b[1341:1341] = sram_blwl_blb[1341:1341] ;
sram6T_blwl sram_blwl_1341_ (sram_blwl_out[1341], sram_blwl_out[1341], sram_blwl_outb[1341], sram_blwl_1341_configbus0[1341:1341], sram_blwl_1341_configbus1[1341:1341] , sram_blwl_1341_configbus0_b[1341:1341] );
wire [1342:1342] sram_blwl_1342_configbus0;
wire [1342:1342] sram_blwl_1342_configbus1;
wire [1342:1342] sram_blwl_1342_configbus0_b;
assign sram_blwl_1342_configbus0[1342:1342] = sram_blwl_bl[1342:1342] ;
assign sram_blwl_1342_configbus1[1342:1342] = sram_blwl_wl[1342:1342] ;
assign sram_blwl_1342_configbus0_b[1342:1342] = sram_blwl_blb[1342:1342] ;
sram6T_blwl sram_blwl_1342_ (sram_blwl_out[1342], sram_blwl_out[1342], sram_blwl_outb[1342], sram_blwl_1342_configbus0[1342:1342], sram_blwl_1342_configbus1[1342:1342] , sram_blwl_1342_configbus0_b[1342:1342] );
wire [1343:1343] sram_blwl_1343_configbus0;
wire [1343:1343] sram_blwl_1343_configbus1;
wire [1343:1343] sram_blwl_1343_configbus0_b;
assign sram_blwl_1343_configbus0[1343:1343] = sram_blwl_bl[1343:1343] ;
assign sram_blwl_1343_configbus1[1343:1343] = sram_blwl_wl[1343:1343] ;
assign sram_blwl_1343_configbus0_b[1343:1343] = sram_blwl_blb[1343:1343] ;
sram6T_blwl sram_blwl_1343_ (sram_blwl_out[1343], sram_blwl_out[1343], sram_blwl_outb[1343], sram_blwl_1343_configbus0[1343:1343], sram_blwl_1343_configbus1[1343:1343] , sram_blwl_1343_configbus0_b[1343:1343] );
wire [1344:1344] sram_blwl_1344_configbus0;
wire [1344:1344] sram_blwl_1344_configbus1;
wire [1344:1344] sram_blwl_1344_configbus0_b;
assign sram_blwl_1344_configbus0[1344:1344] = sram_blwl_bl[1344:1344] ;
assign sram_blwl_1344_configbus1[1344:1344] = sram_blwl_wl[1344:1344] ;
assign sram_blwl_1344_configbus0_b[1344:1344] = sram_blwl_blb[1344:1344] ;
sram6T_blwl sram_blwl_1344_ (sram_blwl_out[1344], sram_blwl_out[1344], sram_blwl_outb[1344], sram_blwl_1344_configbus0[1344:1344], sram_blwl_1344_configbus1[1344:1344] , sram_blwl_1344_configbus0_b[1344:1344] );
wire [1345:1345] sram_blwl_1345_configbus0;
wire [1345:1345] sram_blwl_1345_configbus1;
wire [1345:1345] sram_blwl_1345_configbus0_b;
assign sram_blwl_1345_configbus0[1345:1345] = sram_blwl_bl[1345:1345] ;
assign sram_blwl_1345_configbus1[1345:1345] = sram_blwl_wl[1345:1345] ;
assign sram_blwl_1345_configbus0_b[1345:1345] = sram_blwl_blb[1345:1345] ;
sram6T_blwl sram_blwl_1345_ (sram_blwl_out[1345], sram_blwl_out[1345], sram_blwl_outb[1345], sram_blwl_1345_configbus0[1345:1345], sram_blwl_1345_configbus1[1345:1345] , sram_blwl_1345_configbus0_b[1345:1345] );
wire [1346:1346] sram_blwl_1346_configbus0;
wire [1346:1346] sram_blwl_1346_configbus1;
wire [1346:1346] sram_blwl_1346_configbus0_b;
assign sram_blwl_1346_configbus0[1346:1346] = sram_blwl_bl[1346:1346] ;
assign sram_blwl_1346_configbus1[1346:1346] = sram_blwl_wl[1346:1346] ;
assign sram_blwl_1346_configbus0_b[1346:1346] = sram_blwl_blb[1346:1346] ;
sram6T_blwl sram_blwl_1346_ (sram_blwl_out[1346], sram_blwl_out[1346], sram_blwl_outb[1346], sram_blwl_1346_configbus0[1346:1346], sram_blwl_1346_configbus1[1346:1346] , sram_blwl_1346_configbus0_b[1346:1346] );
wire [1347:1347] sram_blwl_1347_configbus0;
wire [1347:1347] sram_blwl_1347_configbus1;
wire [1347:1347] sram_blwl_1347_configbus0_b;
assign sram_blwl_1347_configbus0[1347:1347] = sram_blwl_bl[1347:1347] ;
assign sram_blwl_1347_configbus1[1347:1347] = sram_blwl_wl[1347:1347] ;
assign sram_blwl_1347_configbus0_b[1347:1347] = sram_blwl_blb[1347:1347] ;
sram6T_blwl sram_blwl_1347_ (sram_blwl_out[1347], sram_blwl_out[1347], sram_blwl_outb[1347], sram_blwl_1347_configbus0[1347:1347], sram_blwl_1347_configbus1[1347:1347] , sram_blwl_1347_configbus0_b[1347:1347] );
wire [1348:1348] sram_blwl_1348_configbus0;
wire [1348:1348] sram_blwl_1348_configbus1;
wire [1348:1348] sram_blwl_1348_configbus0_b;
assign sram_blwl_1348_configbus0[1348:1348] = sram_blwl_bl[1348:1348] ;
assign sram_blwl_1348_configbus1[1348:1348] = sram_blwl_wl[1348:1348] ;
assign sram_blwl_1348_configbus0_b[1348:1348] = sram_blwl_blb[1348:1348] ;
sram6T_blwl sram_blwl_1348_ (sram_blwl_out[1348], sram_blwl_out[1348], sram_blwl_outb[1348], sram_blwl_1348_configbus0[1348:1348], sram_blwl_1348_configbus1[1348:1348] , sram_blwl_1348_configbus0_b[1348:1348] );
wire [1349:1349] sram_blwl_1349_configbus0;
wire [1349:1349] sram_blwl_1349_configbus1;
wire [1349:1349] sram_blwl_1349_configbus0_b;
assign sram_blwl_1349_configbus0[1349:1349] = sram_blwl_bl[1349:1349] ;
assign sram_blwl_1349_configbus1[1349:1349] = sram_blwl_wl[1349:1349] ;
assign sram_blwl_1349_configbus0_b[1349:1349] = sram_blwl_blb[1349:1349] ;
sram6T_blwl sram_blwl_1349_ (sram_blwl_out[1349], sram_blwl_out[1349], sram_blwl_outb[1349], sram_blwl_1349_configbus0[1349:1349], sram_blwl_1349_configbus1[1349:1349] , sram_blwl_1349_configbus0_b[1349:1349] );
wire [1350:1350] sram_blwl_1350_configbus0;
wire [1350:1350] sram_blwl_1350_configbus1;
wire [1350:1350] sram_blwl_1350_configbus0_b;
assign sram_blwl_1350_configbus0[1350:1350] = sram_blwl_bl[1350:1350] ;
assign sram_blwl_1350_configbus1[1350:1350] = sram_blwl_wl[1350:1350] ;
assign sram_blwl_1350_configbus0_b[1350:1350] = sram_blwl_blb[1350:1350] ;
sram6T_blwl sram_blwl_1350_ (sram_blwl_out[1350], sram_blwl_out[1350], sram_blwl_outb[1350], sram_blwl_1350_configbus0[1350:1350], sram_blwl_1350_configbus1[1350:1350] , sram_blwl_1350_configbus0_b[1350:1350] );
wire [1351:1351] sram_blwl_1351_configbus0;
wire [1351:1351] sram_blwl_1351_configbus1;
wire [1351:1351] sram_blwl_1351_configbus0_b;
assign sram_blwl_1351_configbus0[1351:1351] = sram_blwl_bl[1351:1351] ;
assign sram_blwl_1351_configbus1[1351:1351] = sram_blwl_wl[1351:1351] ;
assign sram_blwl_1351_configbus0_b[1351:1351] = sram_blwl_blb[1351:1351] ;
sram6T_blwl sram_blwl_1351_ (sram_blwl_out[1351], sram_blwl_out[1351], sram_blwl_outb[1351], sram_blwl_1351_configbus0[1351:1351], sram_blwl_1351_configbus1[1351:1351] , sram_blwl_1351_configbus0_b[1351:1351] );
wire [1352:1352] sram_blwl_1352_configbus0;
wire [1352:1352] sram_blwl_1352_configbus1;
wire [1352:1352] sram_blwl_1352_configbus0_b;
assign sram_blwl_1352_configbus0[1352:1352] = sram_blwl_bl[1352:1352] ;
assign sram_blwl_1352_configbus1[1352:1352] = sram_blwl_wl[1352:1352] ;
assign sram_blwl_1352_configbus0_b[1352:1352] = sram_blwl_blb[1352:1352] ;
sram6T_blwl sram_blwl_1352_ (sram_blwl_out[1352], sram_blwl_out[1352], sram_blwl_outb[1352], sram_blwl_1352_configbus0[1352:1352], sram_blwl_1352_configbus1[1352:1352] , sram_blwl_1352_configbus0_b[1352:1352] );
wire [1353:1353] sram_blwl_1353_configbus0;
wire [1353:1353] sram_blwl_1353_configbus1;
wire [1353:1353] sram_blwl_1353_configbus0_b;
assign sram_blwl_1353_configbus0[1353:1353] = sram_blwl_bl[1353:1353] ;
assign sram_blwl_1353_configbus1[1353:1353] = sram_blwl_wl[1353:1353] ;
assign sram_blwl_1353_configbus0_b[1353:1353] = sram_blwl_blb[1353:1353] ;
sram6T_blwl sram_blwl_1353_ (sram_blwl_out[1353], sram_blwl_out[1353], sram_blwl_outb[1353], sram_blwl_1353_configbus0[1353:1353], sram_blwl_1353_configbus1[1353:1353] , sram_blwl_1353_configbus0_b[1353:1353] );
wire [1354:1354] sram_blwl_1354_configbus0;
wire [1354:1354] sram_blwl_1354_configbus1;
wire [1354:1354] sram_blwl_1354_configbus0_b;
assign sram_blwl_1354_configbus0[1354:1354] = sram_blwl_bl[1354:1354] ;
assign sram_blwl_1354_configbus1[1354:1354] = sram_blwl_wl[1354:1354] ;
assign sram_blwl_1354_configbus0_b[1354:1354] = sram_blwl_blb[1354:1354] ;
sram6T_blwl sram_blwl_1354_ (sram_blwl_out[1354], sram_blwl_out[1354], sram_blwl_outb[1354], sram_blwl_1354_configbus0[1354:1354], sram_blwl_1354_configbus1[1354:1354] , sram_blwl_1354_configbus0_b[1354:1354] );
wire [1355:1355] sram_blwl_1355_configbus0;
wire [1355:1355] sram_blwl_1355_configbus1;
wire [1355:1355] sram_blwl_1355_configbus0_b;
assign sram_blwl_1355_configbus0[1355:1355] = sram_blwl_bl[1355:1355] ;
assign sram_blwl_1355_configbus1[1355:1355] = sram_blwl_wl[1355:1355] ;
assign sram_blwl_1355_configbus0_b[1355:1355] = sram_blwl_blb[1355:1355] ;
sram6T_blwl sram_blwl_1355_ (sram_blwl_out[1355], sram_blwl_out[1355], sram_blwl_outb[1355], sram_blwl_1355_configbus0[1355:1355], sram_blwl_1355_configbus1[1355:1355] , sram_blwl_1355_configbus0_b[1355:1355] );
wire [1356:1356] sram_blwl_1356_configbus0;
wire [1356:1356] sram_blwl_1356_configbus1;
wire [1356:1356] sram_blwl_1356_configbus0_b;
assign sram_blwl_1356_configbus0[1356:1356] = sram_blwl_bl[1356:1356] ;
assign sram_blwl_1356_configbus1[1356:1356] = sram_blwl_wl[1356:1356] ;
assign sram_blwl_1356_configbus0_b[1356:1356] = sram_blwl_blb[1356:1356] ;
sram6T_blwl sram_blwl_1356_ (sram_blwl_out[1356], sram_blwl_out[1356], sram_blwl_outb[1356], sram_blwl_1356_configbus0[1356:1356], sram_blwl_1356_configbus1[1356:1356] , sram_blwl_1356_configbus0_b[1356:1356] );
wire [1357:1357] sram_blwl_1357_configbus0;
wire [1357:1357] sram_blwl_1357_configbus1;
wire [1357:1357] sram_blwl_1357_configbus0_b;
assign sram_blwl_1357_configbus0[1357:1357] = sram_blwl_bl[1357:1357] ;
assign sram_blwl_1357_configbus1[1357:1357] = sram_blwl_wl[1357:1357] ;
assign sram_blwl_1357_configbus0_b[1357:1357] = sram_blwl_blb[1357:1357] ;
sram6T_blwl sram_blwl_1357_ (sram_blwl_out[1357], sram_blwl_out[1357], sram_blwl_outb[1357], sram_blwl_1357_configbus0[1357:1357], sram_blwl_1357_configbus1[1357:1357] , sram_blwl_1357_configbus0_b[1357:1357] );
wire [1358:1358] sram_blwl_1358_configbus0;
wire [1358:1358] sram_blwl_1358_configbus1;
wire [1358:1358] sram_blwl_1358_configbus0_b;
assign sram_blwl_1358_configbus0[1358:1358] = sram_blwl_bl[1358:1358] ;
assign sram_blwl_1358_configbus1[1358:1358] = sram_blwl_wl[1358:1358] ;
assign sram_blwl_1358_configbus0_b[1358:1358] = sram_blwl_blb[1358:1358] ;
sram6T_blwl sram_blwl_1358_ (sram_blwl_out[1358], sram_blwl_out[1358], sram_blwl_outb[1358], sram_blwl_1358_configbus0[1358:1358], sram_blwl_1358_configbus1[1358:1358] , sram_blwl_1358_configbus0_b[1358:1358] );
wire [1359:1359] sram_blwl_1359_configbus0;
wire [1359:1359] sram_blwl_1359_configbus1;
wire [1359:1359] sram_blwl_1359_configbus0_b;
assign sram_blwl_1359_configbus0[1359:1359] = sram_blwl_bl[1359:1359] ;
assign sram_blwl_1359_configbus1[1359:1359] = sram_blwl_wl[1359:1359] ;
assign sram_blwl_1359_configbus0_b[1359:1359] = sram_blwl_blb[1359:1359] ;
sram6T_blwl sram_blwl_1359_ (sram_blwl_out[1359], sram_blwl_out[1359], sram_blwl_outb[1359], sram_blwl_1359_configbus0[1359:1359], sram_blwl_1359_configbus1[1359:1359] , sram_blwl_1359_configbus0_b[1359:1359] );
wire [1360:1360] sram_blwl_1360_configbus0;
wire [1360:1360] sram_blwl_1360_configbus1;
wire [1360:1360] sram_blwl_1360_configbus0_b;
assign sram_blwl_1360_configbus0[1360:1360] = sram_blwl_bl[1360:1360] ;
assign sram_blwl_1360_configbus1[1360:1360] = sram_blwl_wl[1360:1360] ;
assign sram_blwl_1360_configbus0_b[1360:1360] = sram_blwl_blb[1360:1360] ;
sram6T_blwl sram_blwl_1360_ (sram_blwl_out[1360], sram_blwl_out[1360], sram_blwl_outb[1360], sram_blwl_1360_configbus0[1360:1360], sram_blwl_1360_configbus1[1360:1360] , sram_blwl_1360_configbus0_b[1360:1360] );
wire [1361:1361] sram_blwl_1361_configbus0;
wire [1361:1361] sram_blwl_1361_configbus1;
wire [1361:1361] sram_blwl_1361_configbus0_b;
assign sram_blwl_1361_configbus0[1361:1361] = sram_blwl_bl[1361:1361] ;
assign sram_blwl_1361_configbus1[1361:1361] = sram_blwl_wl[1361:1361] ;
assign sram_blwl_1361_configbus0_b[1361:1361] = sram_blwl_blb[1361:1361] ;
sram6T_blwl sram_blwl_1361_ (sram_blwl_out[1361], sram_blwl_out[1361], sram_blwl_outb[1361], sram_blwl_1361_configbus0[1361:1361], sram_blwl_1361_configbus1[1361:1361] , sram_blwl_1361_configbus0_b[1361:1361] );
wire [1362:1362] sram_blwl_1362_configbus0;
wire [1362:1362] sram_blwl_1362_configbus1;
wire [1362:1362] sram_blwl_1362_configbus0_b;
assign sram_blwl_1362_configbus0[1362:1362] = sram_blwl_bl[1362:1362] ;
assign sram_blwl_1362_configbus1[1362:1362] = sram_blwl_wl[1362:1362] ;
assign sram_blwl_1362_configbus0_b[1362:1362] = sram_blwl_blb[1362:1362] ;
sram6T_blwl sram_blwl_1362_ (sram_blwl_out[1362], sram_blwl_out[1362], sram_blwl_outb[1362], sram_blwl_1362_configbus0[1362:1362], sram_blwl_1362_configbus1[1362:1362] , sram_blwl_1362_configbus0_b[1362:1362] );
wire [1363:1363] sram_blwl_1363_configbus0;
wire [1363:1363] sram_blwl_1363_configbus1;
wire [1363:1363] sram_blwl_1363_configbus0_b;
assign sram_blwl_1363_configbus0[1363:1363] = sram_blwl_bl[1363:1363] ;
assign sram_blwl_1363_configbus1[1363:1363] = sram_blwl_wl[1363:1363] ;
assign sram_blwl_1363_configbus0_b[1363:1363] = sram_blwl_blb[1363:1363] ;
sram6T_blwl sram_blwl_1363_ (sram_blwl_out[1363], sram_blwl_out[1363], sram_blwl_outb[1363], sram_blwl_1363_configbus0[1363:1363], sram_blwl_1363_configbus1[1363:1363] , sram_blwl_1363_configbus0_b[1363:1363] );
wire [1364:1364] sram_blwl_1364_configbus0;
wire [1364:1364] sram_blwl_1364_configbus1;
wire [1364:1364] sram_blwl_1364_configbus0_b;
assign sram_blwl_1364_configbus0[1364:1364] = sram_blwl_bl[1364:1364] ;
assign sram_blwl_1364_configbus1[1364:1364] = sram_blwl_wl[1364:1364] ;
assign sram_blwl_1364_configbus0_b[1364:1364] = sram_blwl_blb[1364:1364] ;
sram6T_blwl sram_blwl_1364_ (sram_blwl_out[1364], sram_blwl_out[1364], sram_blwl_outb[1364], sram_blwl_1364_configbus0[1364:1364], sram_blwl_1364_configbus1[1364:1364] , sram_blwl_1364_configbus0_b[1364:1364] );
wire [1365:1365] sram_blwl_1365_configbus0;
wire [1365:1365] sram_blwl_1365_configbus1;
wire [1365:1365] sram_blwl_1365_configbus0_b;
assign sram_blwl_1365_configbus0[1365:1365] = sram_blwl_bl[1365:1365] ;
assign sram_blwl_1365_configbus1[1365:1365] = sram_blwl_wl[1365:1365] ;
assign sram_blwl_1365_configbus0_b[1365:1365] = sram_blwl_blb[1365:1365] ;
sram6T_blwl sram_blwl_1365_ (sram_blwl_out[1365], sram_blwl_out[1365], sram_blwl_outb[1365], sram_blwl_1365_configbus0[1365:1365], sram_blwl_1365_configbus1[1365:1365] , sram_blwl_1365_configbus0_b[1365:1365] );
wire [1366:1366] sram_blwl_1366_configbus0;
wire [1366:1366] sram_blwl_1366_configbus1;
wire [1366:1366] sram_blwl_1366_configbus0_b;
assign sram_blwl_1366_configbus0[1366:1366] = sram_blwl_bl[1366:1366] ;
assign sram_blwl_1366_configbus1[1366:1366] = sram_blwl_wl[1366:1366] ;
assign sram_blwl_1366_configbus0_b[1366:1366] = sram_blwl_blb[1366:1366] ;
sram6T_blwl sram_blwl_1366_ (sram_blwl_out[1366], sram_blwl_out[1366], sram_blwl_outb[1366], sram_blwl_1366_configbus0[1366:1366], sram_blwl_1366_configbus1[1366:1366] , sram_blwl_1366_configbus0_b[1366:1366] );
wire [1367:1367] sram_blwl_1367_configbus0;
wire [1367:1367] sram_blwl_1367_configbus1;
wire [1367:1367] sram_blwl_1367_configbus0_b;
assign sram_blwl_1367_configbus0[1367:1367] = sram_blwl_bl[1367:1367] ;
assign sram_blwl_1367_configbus1[1367:1367] = sram_blwl_wl[1367:1367] ;
assign sram_blwl_1367_configbus0_b[1367:1367] = sram_blwl_blb[1367:1367] ;
sram6T_blwl sram_blwl_1367_ (sram_blwl_out[1367], sram_blwl_out[1367], sram_blwl_outb[1367], sram_blwl_1367_configbus0[1367:1367], sram_blwl_1367_configbus1[1367:1367] , sram_blwl_1367_configbus0_b[1367:1367] );
wire [1368:1368] sram_blwl_1368_configbus0;
wire [1368:1368] sram_blwl_1368_configbus1;
wire [1368:1368] sram_blwl_1368_configbus0_b;
assign sram_blwl_1368_configbus0[1368:1368] = sram_blwl_bl[1368:1368] ;
assign sram_blwl_1368_configbus1[1368:1368] = sram_blwl_wl[1368:1368] ;
assign sram_blwl_1368_configbus0_b[1368:1368] = sram_blwl_blb[1368:1368] ;
sram6T_blwl sram_blwl_1368_ (sram_blwl_out[1368], sram_blwl_out[1368], sram_blwl_outb[1368], sram_blwl_1368_configbus0[1368:1368], sram_blwl_1368_configbus1[1368:1368] , sram_blwl_1368_configbus0_b[1368:1368] );
wire [1369:1369] sram_blwl_1369_configbus0;
wire [1369:1369] sram_blwl_1369_configbus1;
wire [1369:1369] sram_blwl_1369_configbus0_b;
assign sram_blwl_1369_configbus0[1369:1369] = sram_blwl_bl[1369:1369] ;
assign sram_blwl_1369_configbus1[1369:1369] = sram_blwl_wl[1369:1369] ;
assign sram_blwl_1369_configbus0_b[1369:1369] = sram_blwl_blb[1369:1369] ;
sram6T_blwl sram_blwl_1369_ (sram_blwl_out[1369], sram_blwl_out[1369], sram_blwl_outb[1369], sram_blwl_1369_configbus0[1369:1369], sram_blwl_1369_configbus1[1369:1369] , sram_blwl_1369_configbus0_b[1369:1369] );
wire [1370:1370] sram_blwl_1370_configbus0;
wire [1370:1370] sram_blwl_1370_configbus1;
wire [1370:1370] sram_blwl_1370_configbus0_b;
assign sram_blwl_1370_configbus0[1370:1370] = sram_blwl_bl[1370:1370] ;
assign sram_blwl_1370_configbus1[1370:1370] = sram_blwl_wl[1370:1370] ;
assign sram_blwl_1370_configbus0_b[1370:1370] = sram_blwl_blb[1370:1370] ;
sram6T_blwl sram_blwl_1370_ (sram_blwl_out[1370], sram_blwl_out[1370], sram_blwl_outb[1370], sram_blwl_1370_configbus0[1370:1370], sram_blwl_1370_configbus1[1370:1370] , sram_blwl_1370_configbus0_b[1370:1370] );
wire [1371:1371] sram_blwl_1371_configbus0;
wire [1371:1371] sram_blwl_1371_configbus1;
wire [1371:1371] sram_blwl_1371_configbus0_b;
assign sram_blwl_1371_configbus0[1371:1371] = sram_blwl_bl[1371:1371] ;
assign sram_blwl_1371_configbus1[1371:1371] = sram_blwl_wl[1371:1371] ;
assign sram_blwl_1371_configbus0_b[1371:1371] = sram_blwl_blb[1371:1371] ;
sram6T_blwl sram_blwl_1371_ (sram_blwl_out[1371], sram_blwl_out[1371], sram_blwl_outb[1371], sram_blwl_1371_configbus0[1371:1371], sram_blwl_1371_configbus1[1371:1371] , sram_blwl_1371_configbus0_b[1371:1371] );
wire [1372:1372] sram_blwl_1372_configbus0;
wire [1372:1372] sram_blwl_1372_configbus1;
wire [1372:1372] sram_blwl_1372_configbus0_b;
assign sram_blwl_1372_configbus0[1372:1372] = sram_blwl_bl[1372:1372] ;
assign sram_blwl_1372_configbus1[1372:1372] = sram_blwl_wl[1372:1372] ;
assign sram_blwl_1372_configbus0_b[1372:1372] = sram_blwl_blb[1372:1372] ;
sram6T_blwl sram_blwl_1372_ (sram_blwl_out[1372], sram_blwl_out[1372], sram_blwl_outb[1372], sram_blwl_1372_configbus0[1372:1372], sram_blwl_1372_configbus1[1372:1372] , sram_blwl_1372_configbus0_b[1372:1372] );
wire [1373:1373] sram_blwl_1373_configbus0;
wire [1373:1373] sram_blwl_1373_configbus1;
wire [1373:1373] sram_blwl_1373_configbus0_b;
assign sram_blwl_1373_configbus0[1373:1373] = sram_blwl_bl[1373:1373] ;
assign sram_blwl_1373_configbus1[1373:1373] = sram_blwl_wl[1373:1373] ;
assign sram_blwl_1373_configbus0_b[1373:1373] = sram_blwl_blb[1373:1373] ;
sram6T_blwl sram_blwl_1373_ (sram_blwl_out[1373], sram_blwl_out[1373], sram_blwl_outb[1373], sram_blwl_1373_configbus0[1373:1373], sram_blwl_1373_configbus1[1373:1373] , sram_blwl_1373_configbus0_b[1373:1373] );
wire [1374:1374] sram_blwl_1374_configbus0;
wire [1374:1374] sram_blwl_1374_configbus1;
wire [1374:1374] sram_blwl_1374_configbus0_b;
assign sram_blwl_1374_configbus0[1374:1374] = sram_blwl_bl[1374:1374] ;
assign sram_blwl_1374_configbus1[1374:1374] = sram_blwl_wl[1374:1374] ;
assign sram_blwl_1374_configbus0_b[1374:1374] = sram_blwl_blb[1374:1374] ;
sram6T_blwl sram_blwl_1374_ (sram_blwl_out[1374], sram_blwl_out[1374], sram_blwl_outb[1374], sram_blwl_1374_configbus0[1374:1374], sram_blwl_1374_configbus1[1374:1374] , sram_blwl_1374_configbus0_b[1374:1374] );
wire [1375:1375] sram_blwl_1375_configbus0;
wire [1375:1375] sram_blwl_1375_configbus1;
wire [1375:1375] sram_blwl_1375_configbus0_b;
assign sram_blwl_1375_configbus0[1375:1375] = sram_blwl_bl[1375:1375] ;
assign sram_blwl_1375_configbus1[1375:1375] = sram_blwl_wl[1375:1375] ;
assign sram_blwl_1375_configbus0_b[1375:1375] = sram_blwl_blb[1375:1375] ;
sram6T_blwl sram_blwl_1375_ (sram_blwl_out[1375], sram_blwl_out[1375], sram_blwl_outb[1375], sram_blwl_1375_configbus0[1375:1375], sram_blwl_1375_configbus1[1375:1375] , sram_blwl_1375_configbus0_b[1375:1375] );
wire [1376:1376] sram_blwl_1376_configbus0;
wire [1376:1376] sram_blwl_1376_configbus1;
wire [1376:1376] sram_blwl_1376_configbus0_b;
assign sram_blwl_1376_configbus0[1376:1376] = sram_blwl_bl[1376:1376] ;
assign sram_blwl_1376_configbus1[1376:1376] = sram_blwl_wl[1376:1376] ;
assign sram_blwl_1376_configbus0_b[1376:1376] = sram_blwl_blb[1376:1376] ;
sram6T_blwl sram_blwl_1376_ (sram_blwl_out[1376], sram_blwl_out[1376], sram_blwl_outb[1376], sram_blwl_1376_configbus0[1376:1376], sram_blwl_1376_configbus1[1376:1376] , sram_blwl_1376_configbus0_b[1376:1376] );
wire [1377:1377] sram_blwl_1377_configbus0;
wire [1377:1377] sram_blwl_1377_configbus1;
wire [1377:1377] sram_blwl_1377_configbus0_b;
assign sram_blwl_1377_configbus0[1377:1377] = sram_blwl_bl[1377:1377] ;
assign sram_blwl_1377_configbus1[1377:1377] = sram_blwl_wl[1377:1377] ;
assign sram_blwl_1377_configbus0_b[1377:1377] = sram_blwl_blb[1377:1377] ;
sram6T_blwl sram_blwl_1377_ (sram_blwl_out[1377], sram_blwl_out[1377], sram_blwl_outb[1377], sram_blwl_1377_configbus0[1377:1377], sram_blwl_1377_configbus1[1377:1377] , sram_blwl_1377_configbus0_b[1377:1377] );
wire [1378:1378] sram_blwl_1378_configbus0;
wire [1378:1378] sram_blwl_1378_configbus1;
wire [1378:1378] sram_blwl_1378_configbus0_b;
assign sram_blwl_1378_configbus0[1378:1378] = sram_blwl_bl[1378:1378] ;
assign sram_blwl_1378_configbus1[1378:1378] = sram_blwl_wl[1378:1378] ;
assign sram_blwl_1378_configbus0_b[1378:1378] = sram_blwl_blb[1378:1378] ;
sram6T_blwl sram_blwl_1378_ (sram_blwl_out[1378], sram_blwl_out[1378], sram_blwl_outb[1378], sram_blwl_1378_configbus0[1378:1378], sram_blwl_1378_configbus1[1378:1378] , sram_blwl_1378_configbus0_b[1378:1378] );
wire [1379:1379] sram_blwl_1379_configbus0;
wire [1379:1379] sram_blwl_1379_configbus1;
wire [1379:1379] sram_blwl_1379_configbus0_b;
assign sram_blwl_1379_configbus0[1379:1379] = sram_blwl_bl[1379:1379] ;
assign sram_blwl_1379_configbus1[1379:1379] = sram_blwl_wl[1379:1379] ;
assign sram_blwl_1379_configbus0_b[1379:1379] = sram_blwl_blb[1379:1379] ;
sram6T_blwl sram_blwl_1379_ (sram_blwl_out[1379], sram_blwl_out[1379], sram_blwl_outb[1379], sram_blwl_1379_configbus0[1379:1379], sram_blwl_1379_configbus1[1379:1379] , sram_blwl_1379_configbus0_b[1379:1379] );
wire [1380:1380] sram_blwl_1380_configbus0;
wire [1380:1380] sram_blwl_1380_configbus1;
wire [1380:1380] sram_blwl_1380_configbus0_b;
assign sram_blwl_1380_configbus0[1380:1380] = sram_blwl_bl[1380:1380] ;
assign sram_blwl_1380_configbus1[1380:1380] = sram_blwl_wl[1380:1380] ;
assign sram_blwl_1380_configbus0_b[1380:1380] = sram_blwl_blb[1380:1380] ;
sram6T_blwl sram_blwl_1380_ (sram_blwl_out[1380], sram_blwl_out[1380], sram_blwl_outb[1380], sram_blwl_1380_configbus0[1380:1380], sram_blwl_1380_configbus1[1380:1380] , sram_blwl_1380_configbus0_b[1380:1380] );
wire [1381:1381] sram_blwl_1381_configbus0;
wire [1381:1381] sram_blwl_1381_configbus1;
wire [1381:1381] sram_blwl_1381_configbus0_b;
assign sram_blwl_1381_configbus0[1381:1381] = sram_blwl_bl[1381:1381] ;
assign sram_blwl_1381_configbus1[1381:1381] = sram_blwl_wl[1381:1381] ;
assign sram_blwl_1381_configbus0_b[1381:1381] = sram_blwl_blb[1381:1381] ;
sram6T_blwl sram_blwl_1381_ (sram_blwl_out[1381], sram_blwl_out[1381], sram_blwl_outb[1381], sram_blwl_1381_configbus0[1381:1381], sram_blwl_1381_configbus1[1381:1381] , sram_blwl_1381_configbus0_b[1381:1381] );
wire [1382:1382] sram_blwl_1382_configbus0;
wire [1382:1382] sram_blwl_1382_configbus1;
wire [1382:1382] sram_blwl_1382_configbus0_b;
assign sram_blwl_1382_configbus0[1382:1382] = sram_blwl_bl[1382:1382] ;
assign sram_blwl_1382_configbus1[1382:1382] = sram_blwl_wl[1382:1382] ;
assign sram_blwl_1382_configbus0_b[1382:1382] = sram_blwl_blb[1382:1382] ;
sram6T_blwl sram_blwl_1382_ (sram_blwl_out[1382], sram_blwl_out[1382], sram_blwl_outb[1382], sram_blwl_1382_configbus0[1382:1382], sram_blwl_1382_configbus1[1382:1382] , sram_blwl_1382_configbus0_b[1382:1382] );
wire [1383:1383] sram_blwl_1383_configbus0;
wire [1383:1383] sram_blwl_1383_configbus1;
wire [1383:1383] sram_blwl_1383_configbus0_b;
assign sram_blwl_1383_configbus0[1383:1383] = sram_blwl_bl[1383:1383] ;
assign sram_blwl_1383_configbus1[1383:1383] = sram_blwl_wl[1383:1383] ;
assign sram_blwl_1383_configbus0_b[1383:1383] = sram_blwl_blb[1383:1383] ;
sram6T_blwl sram_blwl_1383_ (sram_blwl_out[1383], sram_blwl_out[1383], sram_blwl_outb[1383], sram_blwl_1383_configbus0[1383:1383], sram_blwl_1383_configbus1[1383:1383] , sram_blwl_1383_configbus0_b[1383:1383] );
wire [1384:1384] sram_blwl_1384_configbus0;
wire [1384:1384] sram_blwl_1384_configbus1;
wire [1384:1384] sram_blwl_1384_configbus0_b;
assign sram_blwl_1384_configbus0[1384:1384] = sram_blwl_bl[1384:1384] ;
assign sram_blwl_1384_configbus1[1384:1384] = sram_blwl_wl[1384:1384] ;
assign sram_blwl_1384_configbus0_b[1384:1384] = sram_blwl_blb[1384:1384] ;
sram6T_blwl sram_blwl_1384_ (sram_blwl_out[1384], sram_blwl_out[1384], sram_blwl_outb[1384], sram_blwl_1384_configbus0[1384:1384], sram_blwl_1384_configbus1[1384:1384] , sram_blwl_1384_configbus0_b[1384:1384] );
wire [1385:1385] sram_blwl_1385_configbus0;
wire [1385:1385] sram_blwl_1385_configbus1;
wire [1385:1385] sram_blwl_1385_configbus0_b;
assign sram_blwl_1385_configbus0[1385:1385] = sram_blwl_bl[1385:1385] ;
assign sram_blwl_1385_configbus1[1385:1385] = sram_blwl_wl[1385:1385] ;
assign sram_blwl_1385_configbus0_b[1385:1385] = sram_blwl_blb[1385:1385] ;
sram6T_blwl sram_blwl_1385_ (sram_blwl_out[1385], sram_blwl_out[1385], sram_blwl_outb[1385], sram_blwl_1385_configbus0[1385:1385], sram_blwl_1385_configbus1[1385:1385] , sram_blwl_1385_configbus0_b[1385:1385] );
wire [1386:1386] sram_blwl_1386_configbus0;
wire [1386:1386] sram_blwl_1386_configbus1;
wire [1386:1386] sram_blwl_1386_configbus0_b;
assign sram_blwl_1386_configbus0[1386:1386] = sram_blwl_bl[1386:1386] ;
assign sram_blwl_1386_configbus1[1386:1386] = sram_blwl_wl[1386:1386] ;
assign sram_blwl_1386_configbus0_b[1386:1386] = sram_blwl_blb[1386:1386] ;
sram6T_blwl sram_blwl_1386_ (sram_blwl_out[1386], sram_blwl_out[1386], sram_blwl_outb[1386], sram_blwl_1386_configbus0[1386:1386], sram_blwl_1386_configbus1[1386:1386] , sram_blwl_1386_configbus0_b[1386:1386] );
wire [1387:1387] sram_blwl_1387_configbus0;
wire [1387:1387] sram_blwl_1387_configbus1;
wire [1387:1387] sram_blwl_1387_configbus0_b;
assign sram_blwl_1387_configbus0[1387:1387] = sram_blwl_bl[1387:1387] ;
assign sram_blwl_1387_configbus1[1387:1387] = sram_blwl_wl[1387:1387] ;
assign sram_blwl_1387_configbus0_b[1387:1387] = sram_blwl_blb[1387:1387] ;
sram6T_blwl sram_blwl_1387_ (sram_blwl_out[1387], sram_blwl_out[1387], sram_blwl_outb[1387], sram_blwl_1387_configbus0[1387:1387], sram_blwl_1387_configbus1[1387:1387] , sram_blwl_1387_configbus0_b[1387:1387] );
wire [1388:1388] sram_blwl_1388_configbus0;
wire [1388:1388] sram_blwl_1388_configbus1;
wire [1388:1388] sram_blwl_1388_configbus0_b;
assign sram_blwl_1388_configbus0[1388:1388] = sram_blwl_bl[1388:1388] ;
assign sram_blwl_1388_configbus1[1388:1388] = sram_blwl_wl[1388:1388] ;
assign sram_blwl_1388_configbus0_b[1388:1388] = sram_blwl_blb[1388:1388] ;
sram6T_blwl sram_blwl_1388_ (sram_blwl_out[1388], sram_blwl_out[1388], sram_blwl_outb[1388], sram_blwl_1388_configbus0[1388:1388], sram_blwl_1388_configbus1[1388:1388] , sram_blwl_1388_configbus0_b[1388:1388] );
wire [1389:1389] sram_blwl_1389_configbus0;
wire [1389:1389] sram_blwl_1389_configbus1;
wire [1389:1389] sram_blwl_1389_configbus0_b;
assign sram_blwl_1389_configbus0[1389:1389] = sram_blwl_bl[1389:1389] ;
assign sram_blwl_1389_configbus1[1389:1389] = sram_blwl_wl[1389:1389] ;
assign sram_blwl_1389_configbus0_b[1389:1389] = sram_blwl_blb[1389:1389] ;
sram6T_blwl sram_blwl_1389_ (sram_blwl_out[1389], sram_blwl_out[1389], sram_blwl_outb[1389], sram_blwl_1389_configbus0[1389:1389], sram_blwl_1389_configbus1[1389:1389] , sram_blwl_1389_configbus0_b[1389:1389] );
wire [1390:1390] sram_blwl_1390_configbus0;
wire [1390:1390] sram_blwl_1390_configbus1;
wire [1390:1390] sram_blwl_1390_configbus0_b;
assign sram_blwl_1390_configbus0[1390:1390] = sram_blwl_bl[1390:1390] ;
assign sram_blwl_1390_configbus1[1390:1390] = sram_blwl_wl[1390:1390] ;
assign sram_blwl_1390_configbus0_b[1390:1390] = sram_blwl_blb[1390:1390] ;
sram6T_blwl sram_blwl_1390_ (sram_blwl_out[1390], sram_blwl_out[1390], sram_blwl_outb[1390], sram_blwl_1390_configbus0[1390:1390], sram_blwl_1390_configbus1[1390:1390] , sram_blwl_1390_configbus0_b[1390:1390] );
wire [1391:1391] sram_blwl_1391_configbus0;
wire [1391:1391] sram_blwl_1391_configbus1;
wire [1391:1391] sram_blwl_1391_configbus0_b;
assign sram_blwl_1391_configbus0[1391:1391] = sram_blwl_bl[1391:1391] ;
assign sram_blwl_1391_configbus1[1391:1391] = sram_blwl_wl[1391:1391] ;
assign sram_blwl_1391_configbus0_b[1391:1391] = sram_blwl_blb[1391:1391] ;
sram6T_blwl sram_blwl_1391_ (sram_blwl_out[1391], sram_blwl_out[1391], sram_blwl_outb[1391], sram_blwl_1391_configbus0[1391:1391], sram_blwl_1391_configbus1[1391:1391] , sram_blwl_1391_configbus0_b[1391:1391] );
wire [1392:1392] sram_blwl_1392_configbus0;
wire [1392:1392] sram_blwl_1392_configbus1;
wire [1392:1392] sram_blwl_1392_configbus0_b;
assign sram_blwl_1392_configbus0[1392:1392] = sram_blwl_bl[1392:1392] ;
assign sram_blwl_1392_configbus1[1392:1392] = sram_blwl_wl[1392:1392] ;
assign sram_blwl_1392_configbus0_b[1392:1392] = sram_blwl_blb[1392:1392] ;
sram6T_blwl sram_blwl_1392_ (sram_blwl_out[1392], sram_blwl_out[1392], sram_blwl_outb[1392], sram_blwl_1392_configbus0[1392:1392], sram_blwl_1392_configbus1[1392:1392] , sram_blwl_1392_configbus0_b[1392:1392] );
wire [1393:1393] sram_blwl_1393_configbus0;
wire [1393:1393] sram_blwl_1393_configbus1;
wire [1393:1393] sram_blwl_1393_configbus0_b;
assign sram_blwl_1393_configbus0[1393:1393] = sram_blwl_bl[1393:1393] ;
assign sram_blwl_1393_configbus1[1393:1393] = sram_blwl_wl[1393:1393] ;
assign sram_blwl_1393_configbus0_b[1393:1393] = sram_blwl_blb[1393:1393] ;
sram6T_blwl sram_blwl_1393_ (sram_blwl_out[1393], sram_blwl_out[1393], sram_blwl_outb[1393], sram_blwl_1393_configbus0[1393:1393], sram_blwl_1393_configbus1[1393:1393] , sram_blwl_1393_configbus0_b[1393:1393] );
wire [1394:1394] sram_blwl_1394_configbus0;
wire [1394:1394] sram_blwl_1394_configbus1;
wire [1394:1394] sram_blwl_1394_configbus0_b;
assign sram_blwl_1394_configbus0[1394:1394] = sram_blwl_bl[1394:1394] ;
assign sram_blwl_1394_configbus1[1394:1394] = sram_blwl_wl[1394:1394] ;
assign sram_blwl_1394_configbus0_b[1394:1394] = sram_blwl_blb[1394:1394] ;
sram6T_blwl sram_blwl_1394_ (sram_blwl_out[1394], sram_blwl_out[1394], sram_blwl_outb[1394], sram_blwl_1394_configbus0[1394:1394], sram_blwl_1394_configbus1[1394:1394] , sram_blwl_1394_configbus0_b[1394:1394] );
wire [1395:1395] sram_blwl_1395_configbus0;
wire [1395:1395] sram_blwl_1395_configbus1;
wire [1395:1395] sram_blwl_1395_configbus0_b;
assign sram_blwl_1395_configbus0[1395:1395] = sram_blwl_bl[1395:1395] ;
assign sram_blwl_1395_configbus1[1395:1395] = sram_blwl_wl[1395:1395] ;
assign sram_blwl_1395_configbus0_b[1395:1395] = sram_blwl_blb[1395:1395] ;
sram6T_blwl sram_blwl_1395_ (sram_blwl_out[1395], sram_blwl_out[1395], sram_blwl_outb[1395], sram_blwl_1395_configbus0[1395:1395], sram_blwl_1395_configbus1[1395:1395] , sram_blwl_1395_configbus0_b[1395:1395] );
wire [1396:1396] sram_blwl_1396_configbus0;
wire [1396:1396] sram_blwl_1396_configbus1;
wire [1396:1396] sram_blwl_1396_configbus0_b;
assign sram_blwl_1396_configbus0[1396:1396] = sram_blwl_bl[1396:1396] ;
assign sram_blwl_1396_configbus1[1396:1396] = sram_blwl_wl[1396:1396] ;
assign sram_blwl_1396_configbus0_b[1396:1396] = sram_blwl_blb[1396:1396] ;
sram6T_blwl sram_blwl_1396_ (sram_blwl_out[1396], sram_blwl_out[1396], sram_blwl_outb[1396], sram_blwl_1396_configbus0[1396:1396], sram_blwl_1396_configbus1[1396:1396] , sram_blwl_1396_configbus0_b[1396:1396] );
wire [1397:1397] sram_blwl_1397_configbus0;
wire [1397:1397] sram_blwl_1397_configbus1;
wire [1397:1397] sram_blwl_1397_configbus0_b;
assign sram_blwl_1397_configbus0[1397:1397] = sram_blwl_bl[1397:1397] ;
assign sram_blwl_1397_configbus1[1397:1397] = sram_blwl_wl[1397:1397] ;
assign sram_blwl_1397_configbus0_b[1397:1397] = sram_blwl_blb[1397:1397] ;
sram6T_blwl sram_blwl_1397_ (sram_blwl_out[1397], sram_blwl_out[1397], sram_blwl_outb[1397], sram_blwl_1397_configbus0[1397:1397], sram_blwl_1397_configbus1[1397:1397] , sram_blwl_1397_configbus0_b[1397:1397] );
wire [1398:1398] sram_blwl_1398_configbus0;
wire [1398:1398] sram_blwl_1398_configbus1;
wire [1398:1398] sram_blwl_1398_configbus0_b;
assign sram_blwl_1398_configbus0[1398:1398] = sram_blwl_bl[1398:1398] ;
assign sram_blwl_1398_configbus1[1398:1398] = sram_blwl_wl[1398:1398] ;
assign sram_blwl_1398_configbus0_b[1398:1398] = sram_blwl_blb[1398:1398] ;
sram6T_blwl sram_blwl_1398_ (sram_blwl_out[1398], sram_blwl_out[1398], sram_blwl_outb[1398], sram_blwl_1398_configbus0[1398:1398], sram_blwl_1398_configbus1[1398:1398] , sram_blwl_1398_configbus0_b[1398:1398] );
wire [1399:1399] sram_blwl_1399_configbus0;
wire [1399:1399] sram_blwl_1399_configbus1;
wire [1399:1399] sram_blwl_1399_configbus0_b;
assign sram_blwl_1399_configbus0[1399:1399] = sram_blwl_bl[1399:1399] ;
assign sram_blwl_1399_configbus1[1399:1399] = sram_blwl_wl[1399:1399] ;
assign sram_blwl_1399_configbus0_b[1399:1399] = sram_blwl_blb[1399:1399] ;
sram6T_blwl sram_blwl_1399_ (sram_blwl_out[1399], sram_blwl_out[1399], sram_blwl_outb[1399], sram_blwl_1399_configbus0[1399:1399], sram_blwl_1399_configbus1[1399:1399] , sram_blwl_1399_configbus0_b[1399:1399] );
wire [1400:1400] sram_blwl_1400_configbus0;
wire [1400:1400] sram_blwl_1400_configbus1;
wire [1400:1400] sram_blwl_1400_configbus0_b;
assign sram_blwl_1400_configbus0[1400:1400] = sram_blwl_bl[1400:1400] ;
assign sram_blwl_1400_configbus1[1400:1400] = sram_blwl_wl[1400:1400] ;
assign sram_blwl_1400_configbus0_b[1400:1400] = sram_blwl_blb[1400:1400] ;
sram6T_blwl sram_blwl_1400_ (sram_blwl_out[1400], sram_blwl_out[1400], sram_blwl_outb[1400], sram_blwl_1400_configbus0[1400:1400], sram_blwl_1400_configbus1[1400:1400] , sram_blwl_1400_configbus0_b[1400:1400] );
wire [1401:1401] sram_blwl_1401_configbus0;
wire [1401:1401] sram_blwl_1401_configbus1;
wire [1401:1401] sram_blwl_1401_configbus0_b;
assign sram_blwl_1401_configbus0[1401:1401] = sram_blwl_bl[1401:1401] ;
assign sram_blwl_1401_configbus1[1401:1401] = sram_blwl_wl[1401:1401] ;
assign sram_blwl_1401_configbus0_b[1401:1401] = sram_blwl_blb[1401:1401] ;
sram6T_blwl sram_blwl_1401_ (sram_blwl_out[1401], sram_blwl_out[1401], sram_blwl_outb[1401], sram_blwl_1401_configbus0[1401:1401], sram_blwl_1401_configbus1[1401:1401] , sram_blwl_1401_configbus0_b[1401:1401] );
wire [1402:1402] sram_blwl_1402_configbus0;
wire [1402:1402] sram_blwl_1402_configbus1;
wire [1402:1402] sram_blwl_1402_configbus0_b;
assign sram_blwl_1402_configbus0[1402:1402] = sram_blwl_bl[1402:1402] ;
assign sram_blwl_1402_configbus1[1402:1402] = sram_blwl_wl[1402:1402] ;
assign sram_blwl_1402_configbus0_b[1402:1402] = sram_blwl_blb[1402:1402] ;
sram6T_blwl sram_blwl_1402_ (sram_blwl_out[1402], sram_blwl_out[1402], sram_blwl_outb[1402], sram_blwl_1402_configbus0[1402:1402], sram_blwl_1402_configbus1[1402:1402] , sram_blwl_1402_configbus0_b[1402:1402] );
wire [1403:1403] sram_blwl_1403_configbus0;
wire [1403:1403] sram_blwl_1403_configbus1;
wire [1403:1403] sram_blwl_1403_configbus0_b;
assign sram_blwl_1403_configbus0[1403:1403] = sram_blwl_bl[1403:1403] ;
assign sram_blwl_1403_configbus1[1403:1403] = sram_blwl_wl[1403:1403] ;
assign sram_blwl_1403_configbus0_b[1403:1403] = sram_blwl_blb[1403:1403] ;
sram6T_blwl sram_blwl_1403_ (sram_blwl_out[1403], sram_blwl_out[1403], sram_blwl_outb[1403], sram_blwl_1403_configbus0[1403:1403], sram_blwl_1403_configbus1[1403:1403] , sram_blwl_1403_configbus0_b[1403:1403] );
wire [1404:1404] sram_blwl_1404_configbus0;
wire [1404:1404] sram_blwl_1404_configbus1;
wire [1404:1404] sram_blwl_1404_configbus0_b;
assign sram_blwl_1404_configbus0[1404:1404] = sram_blwl_bl[1404:1404] ;
assign sram_blwl_1404_configbus1[1404:1404] = sram_blwl_wl[1404:1404] ;
assign sram_blwl_1404_configbus0_b[1404:1404] = sram_blwl_blb[1404:1404] ;
sram6T_blwl sram_blwl_1404_ (sram_blwl_out[1404], sram_blwl_out[1404], sram_blwl_outb[1404], sram_blwl_1404_configbus0[1404:1404], sram_blwl_1404_configbus1[1404:1404] , sram_blwl_1404_configbus0_b[1404:1404] );
endmodule
//----- END LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----

//----- Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----
module grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
input [0:0] Set,
input [0:0] Reset,
input [0:0] clk
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
input wire ff_0___D_0_,
output wire ff_0___Q_0_);
static_dff dff_5_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
endmodule
//----- END Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6_ -----
module grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_ble6___in_0_,
input wire mode_ble6___in_1_,
input wire mode_ble6___in_2_,
input wire mode_ble6___in_3_,
input wire mode_ble6___in_4_,
input wire mode_ble6___in_5_,
output wire mode_ble6___out_0_,
input wire mode_ble6___clk_0_,
input [1341:1405] sram_blwl_bl ,
input [1341:1405] sram_blwl_wl ,
input [1341:1405] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ lut6_0_ (
 lut6_0___in_0_,  lut6_0___in_1_,  lut6_0___in_2_,  lut6_0___in_3_,  lut6_0___in_4_,  lut6_0___in_5_,  lut6_0___out_0_,
sram_blwl_bl[1341:1404] ,
sram_blwl_wl[1341:1404] ,
sram_blwl_blb[1341:1404] );
grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ ff_0_ (
//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
wire [0:1] in_bus_mux_1level_tapbuf_size2_405_ ;
assign in_bus_mux_1level_tapbuf_size2_405_[0] = ff_0___Q_0_ ; 
assign in_bus_mux_1level_tapbuf_size2_405_[1] = lut6_0___out_0_ ; 
wire [1405:1405] mux_1level_tapbuf_size2_405_configbus0;
wire [1405:1405] mux_1level_tapbuf_size2_405_configbus1;
wire [1405:1405] mux_1level_tapbuf_size2_405_sram_blwl_out ;
wire [1405:1405] mux_1level_tapbuf_size2_405_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_405_configbus0[1405:1405] = sram_blwl_bl[1405:1405] ;
assign mux_1level_tapbuf_size2_405_configbus1[1405:1405] = sram_blwl_wl[1405:1405] ;
wire [1405:1405] mux_1level_tapbuf_size2_405_configbus0_b;
assign mux_1level_tapbuf_size2_405_configbus0_b[1405:1405] = sram_blwl_blb[1405:1405] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_405_ (in_bus_mux_1level_tapbuf_size2_405_, mode_ble6___out_0_, mux_1level_tapbuf_size2_405_sram_blwl_out[1405:1405] ,
mux_1level_tapbuf_size2_405_sram_blwl_outb[1405:1405] );
//----- SRAM bits for MUX[405], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_1405_ (mux_1level_tapbuf_size2_405_sram_blwl_out[1405:1405] ,mux_1level_tapbuf_size2_405_sram_blwl_out[1405:1405] ,mux_1level_tapbuf_size2_405_sram_blwl_outb[1405:1405] ,mux_1level_tapbuf_size2_405_configbus0[1405:1405], mux_1level_tapbuf_size2_405_configbus1[1405:1405] , mux_1level_tapbuf_size2_405_configbus0_b[1405:1405] );
direct_interc direct_interc_80_ (mode_ble6___in_0_, lut6_0___in_0_ );
direct_interc direct_interc_81_ (mode_ble6___in_1_, lut6_0___in_1_ );
direct_interc direct_interc_82_ (mode_ble6___in_2_, lut6_0___in_2_ );
direct_interc direct_interc_83_ (mode_ble6___in_3_, lut6_0___in_3_ );
direct_interc direct_interc_84_ (mode_ble6___in_4_, lut6_0___in_4_ );
direct_interc direct_interc_85_ (mode_ble6___in_5_, lut6_0___in_5_ );
direct_interc direct_interc_86_ (lut6_0___out_0_, ff_0___D_0_ );
direct_interc direct_interc_87_ (mode_ble6___clk_0_, ff_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6_ -----
module grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_n1_lut6___in_0_,
input wire mode_n1_lut6___in_1_,
input wire mode_n1_lut6___in_2_,
input wire mode_n1_lut6___in_3_,
input wire mode_n1_lut6___in_4_,
input wire mode_n1_lut6___in_5_,
output wire mode_n1_lut6___out_0_,
input wire mode_n1_lut6___clk_0_,
input [1341:1405] sram_blwl_bl ,
input [1341:1405] sram_blwl_wl ,
input [1341:1405] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6__ble6_0__mode_ble6_ ble6_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 ble6_0___in_0_,  ble6_0___in_1_,  ble6_0___in_2_,  ble6_0___in_3_,  ble6_0___in_4_,  ble6_0___in_5_,  ble6_0___out_0_,  ble6_0___clk_0_,
sram_blwl_bl[1341:1405] ,
sram_blwl_wl[1341:1405] ,
sram_blwl_blb[1341:1405] );
direct_interc direct_interc_88_ (ble6_0___out_0_, mode_n1_lut6___out_0_ );
direct_interc direct_interc_89_ (mode_n1_lut6___in_0_, ble6_0___in_0_ );
direct_interc direct_interc_90_ (mode_n1_lut6___in_1_, ble6_0___in_1_ );
direct_interc direct_interc_91_ (mode_n1_lut6___in_2_, ble6_0___in_2_ );
direct_interc direct_interc_92_ (mode_n1_lut6___in_3_, ble6_0___in_3_ );
direct_interc direct_interc_93_ (mode_n1_lut6___in_4_, ble6_0___in_4_ );
direct_interc direct_interc_94_ (mode_n1_lut6___in_5_, ble6_0___in_5_ );
direct_interc direct_interc_95_ (mode_n1_lut6___clk_0_, ble6_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6_ -----

//----- LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----
module grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ (
input wire lut6_0___in_0_,
input wire lut6_0___in_1_,
input wire lut6_0___in_2_,
input wire lut6_0___in_3_,
input wire lut6_0___in_4_,
input wire lut6_0___in_5_,
output wire lut6_0___out_0_,
input [1406:1469] sram_blwl_bl ,
input [1406:1469] sram_blwl_wl ,
input [1406:1469] sram_blwl_blb );
wire [0:5] lut6_0___in;
assign lut6_0___in[0] = lut6_0___in_0_;
assign lut6_0___in[1] = lut6_0___in_1_;
assign lut6_0___in[2] = lut6_0___in_2_;
assign lut6_0___in[3] = lut6_0___in_3_;
assign lut6_0___in[4] = lut6_0___in_4_;
assign lut6_0___in[5] = lut6_0___in_5_;
wire [0:0] lut6_0___out;
assign lut6_0___out_0_ = lut6_0___out[0];
wire [1406:1469] sram_blwl_out ;
wire [1406:1469] sram_blwl_outb ;
lut6 lut6_6_ (
//----- Input and output ports -----
 lut6_0___in[0:5] ,  lut6_0___out[0:0],//----- SRAM ports -----
sram_blwl_out[1406:1469] , sram_blwl_outb[1406:1469] );
//----- Truth Table for LUT[6], size=6. -----
//----- SRAM bits for LUT[6], size=6, num_sram=64. -----
//-----0000000000000000000000000000000000000000000000000000000000000000-----
wire [1406:1406] sram_blwl_1406_configbus0;
wire [1406:1406] sram_blwl_1406_configbus1;
wire [1406:1406] sram_blwl_1406_configbus0_b;
assign sram_blwl_1406_configbus0[1406:1406] = sram_blwl_bl[1406:1406] ;
assign sram_blwl_1406_configbus1[1406:1406] = sram_blwl_wl[1406:1406] ;
assign sram_blwl_1406_configbus0_b[1406:1406] = sram_blwl_blb[1406:1406] ;
sram6T_blwl sram_blwl_1406_ (sram_blwl_out[1406], sram_blwl_out[1406], sram_blwl_outb[1406], sram_blwl_1406_configbus0[1406:1406], sram_blwl_1406_configbus1[1406:1406] , sram_blwl_1406_configbus0_b[1406:1406] );
wire [1407:1407] sram_blwl_1407_configbus0;
wire [1407:1407] sram_blwl_1407_configbus1;
wire [1407:1407] sram_blwl_1407_configbus0_b;
assign sram_blwl_1407_configbus0[1407:1407] = sram_blwl_bl[1407:1407] ;
assign sram_blwl_1407_configbus1[1407:1407] = sram_blwl_wl[1407:1407] ;
assign sram_blwl_1407_configbus0_b[1407:1407] = sram_blwl_blb[1407:1407] ;
sram6T_blwl sram_blwl_1407_ (sram_blwl_out[1407], sram_blwl_out[1407], sram_blwl_outb[1407], sram_blwl_1407_configbus0[1407:1407], sram_blwl_1407_configbus1[1407:1407] , sram_blwl_1407_configbus0_b[1407:1407] );
wire [1408:1408] sram_blwl_1408_configbus0;
wire [1408:1408] sram_blwl_1408_configbus1;
wire [1408:1408] sram_blwl_1408_configbus0_b;
assign sram_blwl_1408_configbus0[1408:1408] = sram_blwl_bl[1408:1408] ;
assign sram_blwl_1408_configbus1[1408:1408] = sram_blwl_wl[1408:1408] ;
assign sram_blwl_1408_configbus0_b[1408:1408] = sram_blwl_blb[1408:1408] ;
sram6T_blwl sram_blwl_1408_ (sram_blwl_out[1408], sram_blwl_out[1408], sram_blwl_outb[1408], sram_blwl_1408_configbus0[1408:1408], sram_blwl_1408_configbus1[1408:1408] , sram_blwl_1408_configbus0_b[1408:1408] );
wire [1409:1409] sram_blwl_1409_configbus0;
wire [1409:1409] sram_blwl_1409_configbus1;
wire [1409:1409] sram_blwl_1409_configbus0_b;
assign sram_blwl_1409_configbus0[1409:1409] = sram_blwl_bl[1409:1409] ;
assign sram_blwl_1409_configbus1[1409:1409] = sram_blwl_wl[1409:1409] ;
assign sram_blwl_1409_configbus0_b[1409:1409] = sram_blwl_blb[1409:1409] ;
sram6T_blwl sram_blwl_1409_ (sram_blwl_out[1409], sram_blwl_out[1409], sram_blwl_outb[1409], sram_blwl_1409_configbus0[1409:1409], sram_blwl_1409_configbus1[1409:1409] , sram_blwl_1409_configbus0_b[1409:1409] );
wire [1410:1410] sram_blwl_1410_configbus0;
wire [1410:1410] sram_blwl_1410_configbus1;
wire [1410:1410] sram_blwl_1410_configbus0_b;
assign sram_blwl_1410_configbus0[1410:1410] = sram_blwl_bl[1410:1410] ;
assign sram_blwl_1410_configbus1[1410:1410] = sram_blwl_wl[1410:1410] ;
assign sram_blwl_1410_configbus0_b[1410:1410] = sram_blwl_blb[1410:1410] ;
sram6T_blwl sram_blwl_1410_ (sram_blwl_out[1410], sram_blwl_out[1410], sram_blwl_outb[1410], sram_blwl_1410_configbus0[1410:1410], sram_blwl_1410_configbus1[1410:1410] , sram_blwl_1410_configbus0_b[1410:1410] );
wire [1411:1411] sram_blwl_1411_configbus0;
wire [1411:1411] sram_blwl_1411_configbus1;
wire [1411:1411] sram_blwl_1411_configbus0_b;
assign sram_blwl_1411_configbus0[1411:1411] = sram_blwl_bl[1411:1411] ;
assign sram_blwl_1411_configbus1[1411:1411] = sram_blwl_wl[1411:1411] ;
assign sram_blwl_1411_configbus0_b[1411:1411] = sram_blwl_blb[1411:1411] ;
sram6T_blwl sram_blwl_1411_ (sram_blwl_out[1411], sram_blwl_out[1411], sram_blwl_outb[1411], sram_blwl_1411_configbus0[1411:1411], sram_blwl_1411_configbus1[1411:1411] , sram_blwl_1411_configbus0_b[1411:1411] );
wire [1412:1412] sram_blwl_1412_configbus0;
wire [1412:1412] sram_blwl_1412_configbus1;
wire [1412:1412] sram_blwl_1412_configbus0_b;
assign sram_blwl_1412_configbus0[1412:1412] = sram_blwl_bl[1412:1412] ;
assign sram_blwl_1412_configbus1[1412:1412] = sram_blwl_wl[1412:1412] ;
assign sram_blwl_1412_configbus0_b[1412:1412] = sram_blwl_blb[1412:1412] ;
sram6T_blwl sram_blwl_1412_ (sram_blwl_out[1412], sram_blwl_out[1412], sram_blwl_outb[1412], sram_blwl_1412_configbus0[1412:1412], sram_blwl_1412_configbus1[1412:1412] , sram_blwl_1412_configbus0_b[1412:1412] );
wire [1413:1413] sram_blwl_1413_configbus0;
wire [1413:1413] sram_blwl_1413_configbus1;
wire [1413:1413] sram_blwl_1413_configbus0_b;
assign sram_blwl_1413_configbus0[1413:1413] = sram_blwl_bl[1413:1413] ;
assign sram_blwl_1413_configbus1[1413:1413] = sram_blwl_wl[1413:1413] ;
assign sram_blwl_1413_configbus0_b[1413:1413] = sram_blwl_blb[1413:1413] ;
sram6T_blwl sram_blwl_1413_ (sram_blwl_out[1413], sram_blwl_out[1413], sram_blwl_outb[1413], sram_blwl_1413_configbus0[1413:1413], sram_blwl_1413_configbus1[1413:1413] , sram_blwl_1413_configbus0_b[1413:1413] );
wire [1414:1414] sram_blwl_1414_configbus0;
wire [1414:1414] sram_blwl_1414_configbus1;
wire [1414:1414] sram_blwl_1414_configbus0_b;
assign sram_blwl_1414_configbus0[1414:1414] = sram_blwl_bl[1414:1414] ;
assign sram_blwl_1414_configbus1[1414:1414] = sram_blwl_wl[1414:1414] ;
assign sram_blwl_1414_configbus0_b[1414:1414] = sram_blwl_blb[1414:1414] ;
sram6T_blwl sram_blwl_1414_ (sram_blwl_out[1414], sram_blwl_out[1414], sram_blwl_outb[1414], sram_blwl_1414_configbus0[1414:1414], sram_blwl_1414_configbus1[1414:1414] , sram_blwl_1414_configbus0_b[1414:1414] );
wire [1415:1415] sram_blwl_1415_configbus0;
wire [1415:1415] sram_blwl_1415_configbus1;
wire [1415:1415] sram_blwl_1415_configbus0_b;
assign sram_blwl_1415_configbus0[1415:1415] = sram_blwl_bl[1415:1415] ;
assign sram_blwl_1415_configbus1[1415:1415] = sram_blwl_wl[1415:1415] ;
assign sram_blwl_1415_configbus0_b[1415:1415] = sram_blwl_blb[1415:1415] ;
sram6T_blwl sram_blwl_1415_ (sram_blwl_out[1415], sram_blwl_out[1415], sram_blwl_outb[1415], sram_blwl_1415_configbus0[1415:1415], sram_blwl_1415_configbus1[1415:1415] , sram_blwl_1415_configbus0_b[1415:1415] );
wire [1416:1416] sram_blwl_1416_configbus0;
wire [1416:1416] sram_blwl_1416_configbus1;
wire [1416:1416] sram_blwl_1416_configbus0_b;
assign sram_blwl_1416_configbus0[1416:1416] = sram_blwl_bl[1416:1416] ;
assign sram_blwl_1416_configbus1[1416:1416] = sram_blwl_wl[1416:1416] ;
assign sram_blwl_1416_configbus0_b[1416:1416] = sram_blwl_blb[1416:1416] ;
sram6T_blwl sram_blwl_1416_ (sram_blwl_out[1416], sram_blwl_out[1416], sram_blwl_outb[1416], sram_blwl_1416_configbus0[1416:1416], sram_blwl_1416_configbus1[1416:1416] , sram_blwl_1416_configbus0_b[1416:1416] );
wire [1417:1417] sram_blwl_1417_configbus0;
wire [1417:1417] sram_blwl_1417_configbus1;
wire [1417:1417] sram_blwl_1417_configbus0_b;
assign sram_blwl_1417_configbus0[1417:1417] = sram_blwl_bl[1417:1417] ;
assign sram_blwl_1417_configbus1[1417:1417] = sram_blwl_wl[1417:1417] ;
assign sram_blwl_1417_configbus0_b[1417:1417] = sram_blwl_blb[1417:1417] ;
sram6T_blwl sram_blwl_1417_ (sram_blwl_out[1417], sram_blwl_out[1417], sram_blwl_outb[1417], sram_blwl_1417_configbus0[1417:1417], sram_blwl_1417_configbus1[1417:1417] , sram_blwl_1417_configbus0_b[1417:1417] );
wire [1418:1418] sram_blwl_1418_configbus0;
wire [1418:1418] sram_blwl_1418_configbus1;
wire [1418:1418] sram_blwl_1418_configbus0_b;
assign sram_blwl_1418_configbus0[1418:1418] = sram_blwl_bl[1418:1418] ;
assign sram_blwl_1418_configbus1[1418:1418] = sram_blwl_wl[1418:1418] ;
assign sram_blwl_1418_configbus0_b[1418:1418] = sram_blwl_blb[1418:1418] ;
sram6T_blwl sram_blwl_1418_ (sram_blwl_out[1418], sram_blwl_out[1418], sram_blwl_outb[1418], sram_blwl_1418_configbus0[1418:1418], sram_blwl_1418_configbus1[1418:1418] , sram_blwl_1418_configbus0_b[1418:1418] );
wire [1419:1419] sram_blwl_1419_configbus0;
wire [1419:1419] sram_blwl_1419_configbus1;
wire [1419:1419] sram_blwl_1419_configbus0_b;
assign sram_blwl_1419_configbus0[1419:1419] = sram_blwl_bl[1419:1419] ;
assign sram_blwl_1419_configbus1[1419:1419] = sram_blwl_wl[1419:1419] ;
assign sram_blwl_1419_configbus0_b[1419:1419] = sram_blwl_blb[1419:1419] ;
sram6T_blwl sram_blwl_1419_ (sram_blwl_out[1419], sram_blwl_out[1419], sram_blwl_outb[1419], sram_blwl_1419_configbus0[1419:1419], sram_blwl_1419_configbus1[1419:1419] , sram_blwl_1419_configbus0_b[1419:1419] );
wire [1420:1420] sram_blwl_1420_configbus0;
wire [1420:1420] sram_blwl_1420_configbus1;
wire [1420:1420] sram_blwl_1420_configbus0_b;
assign sram_blwl_1420_configbus0[1420:1420] = sram_blwl_bl[1420:1420] ;
assign sram_blwl_1420_configbus1[1420:1420] = sram_blwl_wl[1420:1420] ;
assign sram_blwl_1420_configbus0_b[1420:1420] = sram_blwl_blb[1420:1420] ;
sram6T_blwl sram_blwl_1420_ (sram_blwl_out[1420], sram_blwl_out[1420], sram_blwl_outb[1420], sram_blwl_1420_configbus0[1420:1420], sram_blwl_1420_configbus1[1420:1420] , sram_blwl_1420_configbus0_b[1420:1420] );
wire [1421:1421] sram_blwl_1421_configbus0;
wire [1421:1421] sram_blwl_1421_configbus1;
wire [1421:1421] sram_blwl_1421_configbus0_b;
assign sram_blwl_1421_configbus0[1421:1421] = sram_blwl_bl[1421:1421] ;
assign sram_blwl_1421_configbus1[1421:1421] = sram_blwl_wl[1421:1421] ;
assign sram_blwl_1421_configbus0_b[1421:1421] = sram_blwl_blb[1421:1421] ;
sram6T_blwl sram_blwl_1421_ (sram_blwl_out[1421], sram_blwl_out[1421], sram_blwl_outb[1421], sram_blwl_1421_configbus0[1421:1421], sram_blwl_1421_configbus1[1421:1421] , sram_blwl_1421_configbus0_b[1421:1421] );
wire [1422:1422] sram_blwl_1422_configbus0;
wire [1422:1422] sram_blwl_1422_configbus1;
wire [1422:1422] sram_blwl_1422_configbus0_b;
assign sram_blwl_1422_configbus0[1422:1422] = sram_blwl_bl[1422:1422] ;
assign sram_blwl_1422_configbus1[1422:1422] = sram_blwl_wl[1422:1422] ;
assign sram_blwl_1422_configbus0_b[1422:1422] = sram_blwl_blb[1422:1422] ;
sram6T_blwl sram_blwl_1422_ (sram_blwl_out[1422], sram_blwl_out[1422], sram_blwl_outb[1422], sram_blwl_1422_configbus0[1422:1422], sram_blwl_1422_configbus1[1422:1422] , sram_blwl_1422_configbus0_b[1422:1422] );
wire [1423:1423] sram_blwl_1423_configbus0;
wire [1423:1423] sram_blwl_1423_configbus1;
wire [1423:1423] sram_blwl_1423_configbus0_b;
assign sram_blwl_1423_configbus0[1423:1423] = sram_blwl_bl[1423:1423] ;
assign sram_blwl_1423_configbus1[1423:1423] = sram_blwl_wl[1423:1423] ;
assign sram_blwl_1423_configbus0_b[1423:1423] = sram_blwl_blb[1423:1423] ;
sram6T_blwl sram_blwl_1423_ (sram_blwl_out[1423], sram_blwl_out[1423], sram_blwl_outb[1423], sram_blwl_1423_configbus0[1423:1423], sram_blwl_1423_configbus1[1423:1423] , sram_blwl_1423_configbus0_b[1423:1423] );
wire [1424:1424] sram_blwl_1424_configbus0;
wire [1424:1424] sram_blwl_1424_configbus1;
wire [1424:1424] sram_blwl_1424_configbus0_b;
assign sram_blwl_1424_configbus0[1424:1424] = sram_blwl_bl[1424:1424] ;
assign sram_blwl_1424_configbus1[1424:1424] = sram_blwl_wl[1424:1424] ;
assign sram_blwl_1424_configbus0_b[1424:1424] = sram_blwl_blb[1424:1424] ;
sram6T_blwl sram_blwl_1424_ (sram_blwl_out[1424], sram_blwl_out[1424], sram_blwl_outb[1424], sram_blwl_1424_configbus0[1424:1424], sram_blwl_1424_configbus1[1424:1424] , sram_blwl_1424_configbus0_b[1424:1424] );
wire [1425:1425] sram_blwl_1425_configbus0;
wire [1425:1425] sram_blwl_1425_configbus1;
wire [1425:1425] sram_blwl_1425_configbus0_b;
assign sram_blwl_1425_configbus0[1425:1425] = sram_blwl_bl[1425:1425] ;
assign sram_blwl_1425_configbus1[1425:1425] = sram_blwl_wl[1425:1425] ;
assign sram_blwl_1425_configbus0_b[1425:1425] = sram_blwl_blb[1425:1425] ;
sram6T_blwl sram_blwl_1425_ (sram_blwl_out[1425], sram_blwl_out[1425], sram_blwl_outb[1425], sram_blwl_1425_configbus0[1425:1425], sram_blwl_1425_configbus1[1425:1425] , sram_blwl_1425_configbus0_b[1425:1425] );
wire [1426:1426] sram_blwl_1426_configbus0;
wire [1426:1426] sram_blwl_1426_configbus1;
wire [1426:1426] sram_blwl_1426_configbus0_b;
assign sram_blwl_1426_configbus0[1426:1426] = sram_blwl_bl[1426:1426] ;
assign sram_blwl_1426_configbus1[1426:1426] = sram_blwl_wl[1426:1426] ;
assign sram_blwl_1426_configbus0_b[1426:1426] = sram_blwl_blb[1426:1426] ;
sram6T_blwl sram_blwl_1426_ (sram_blwl_out[1426], sram_blwl_out[1426], sram_blwl_outb[1426], sram_blwl_1426_configbus0[1426:1426], sram_blwl_1426_configbus1[1426:1426] , sram_blwl_1426_configbus0_b[1426:1426] );
wire [1427:1427] sram_blwl_1427_configbus0;
wire [1427:1427] sram_blwl_1427_configbus1;
wire [1427:1427] sram_blwl_1427_configbus0_b;
assign sram_blwl_1427_configbus0[1427:1427] = sram_blwl_bl[1427:1427] ;
assign sram_blwl_1427_configbus1[1427:1427] = sram_blwl_wl[1427:1427] ;
assign sram_blwl_1427_configbus0_b[1427:1427] = sram_blwl_blb[1427:1427] ;
sram6T_blwl sram_blwl_1427_ (sram_blwl_out[1427], sram_blwl_out[1427], sram_blwl_outb[1427], sram_blwl_1427_configbus0[1427:1427], sram_blwl_1427_configbus1[1427:1427] , sram_blwl_1427_configbus0_b[1427:1427] );
wire [1428:1428] sram_blwl_1428_configbus0;
wire [1428:1428] sram_blwl_1428_configbus1;
wire [1428:1428] sram_blwl_1428_configbus0_b;
assign sram_blwl_1428_configbus0[1428:1428] = sram_blwl_bl[1428:1428] ;
assign sram_blwl_1428_configbus1[1428:1428] = sram_blwl_wl[1428:1428] ;
assign sram_blwl_1428_configbus0_b[1428:1428] = sram_blwl_blb[1428:1428] ;
sram6T_blwl sram_blwl_1428_ (sram_blwl_out[1428], sram_blwl_out[1428], sram_blwl_outb[1428], sram_blwl_1428_configbus0[1428:1428], sram_blwl_1428_configbus1[1428:1428] , sram_blwl_1428_configbus0_b[1428:1428] );
wire [1429:1429] sram_blwl_1429_configbus0;
wire [1429:1429] sram_blwl_1429_configbus1;
wire [1429:1429] sram_blwl_1429_configbus0_b;
assign sram_blwl_1429_configbus0[1429:1429] = sram_blwl_bl[1429:1429] ;
assign sram_blwl_1429_configbus1[1429:1429] = sram_blwl_wl[1429:1429] ;
assign sram_blwl_1429_configbus0_b[1429:1429] = sram_blwl_blb[1429:1429] ;
sram6T_blwl sram_blwl_1429_ (sram_blwl_out[1429], sram_blwl_out[1429], sram_blwl_outb[1429], sram_blwl_1429_configbus0[1429:1429], sram_blwl_1429_configbus1[1429:1429] , sram_blwl_1429_configbus0_b[1429:1429] );
wire [1430:1430] sram_blwl_1430_configbus0;
wire [1430:1430] sram_blwl_1430_configbus1;
wire [1430:1430] sram_blwl_1430_configbus0_b;
assign sram_blwl_1430_configbus0[1430:1430] = sram_blwl_bl[1430:1430] ;
assign sram_blwl_1430_configbus1[1430:1430] = sram_blwl_wl[1430:1430] ;
assign sram_blwl_1430_configbus0_b[1430:1430] = sram_blwl_blb[1430:1430] ;
sram6T_blwl sram_blwl_1430_ (sram_blwl_out[1430], sram_blwl_out[1430], sram_blwl_outb[1430], sram_blwl_1430_configbus0[1430:1430], sram_blwl_1430_configbus1[1430:1430] , sram_blwl_1430_configbus0_b[1430:1430] );
wire [1431:1431] sram_blwl_1431_configbus0;
wire [1431:1431] sram_blwl_1431_configbus1;
wire [1431:1431] sram_blwl_1431_configbus0_b;
assign sram_blwl_1431_configbus0[1431:1431] = sram_blwl_bl[1431:1431] ;
assign sram_blwl_1431_configbus1[1431:1431] = sram_blwl_wl[1431:1431] ;
assign sram_blwl_1431_configbus0_b[1431:1431] = sram_blwl_blb[1431:1431] ;
sram6T_blwl sram_blwl_1431_ (sram_blwl_out[1431], sram_blwl_out[1431], sram_blwl_outb[1431], sram_blwl_1431_configbus0[1431:1431], sram_blwl_1431_configbus1[1431:1431] , sram_blwl_1431_configbus0_b[1431:1431] );
wire [1432:1432] sram_blwl_1432_configbus0;
wire [1432:1432] sram_blwl_1432_configbus1;
wire [1432:1432] sram_blwl_1432_configbus0_b;
assign sram_blwl_1432_configbus0[1432:1432] = sram_blwl_bl[1432:1432] ;
assign sram_blwl_1432_configbus1[1432:1432] = sram_blwl_wl[1432:1432] ;
assign sram_blwl_1432_configbus0_b[1432:1432] = sram_blwl_blb[1432:1432] ;
sram6T_blwl sram_blwl_1432_ (sram_blwl_out[1432], sram_blwl_out[1432], sram_blwl_outb[1432], sram_blwl_1432_configbus0[1432:1432], sram_blwl_1432_configbus1[1432:1432] , sram_blwl_1432_configbus0_b[1432:1432] );
wire [1433:1433] sram_blwl_1433_configbus0;
wire [1433:1433] sram_blwl_1433_configbus1;
wire [1433:1433] sram_blwl_1433_configbus0_b;
assign sram_blwl_1433_configbus0[1433:1433] = sram_blwl_bl[1433:1433] ;
assign sram_blwl_1433_configbus1[1433:1433] = sram_blwl_wl[1433:1433] ;
assign sram_blwl_1433_configbus0_b[1433:1433] = sram_blwl_blb[1433:1433] ;
sram6T_blwl sram_blwl_1433_ (sram_blwl_out[1433], sram_blwl_out[1433], sram_blwl_outb[1433], sram_blwl_1433_configbus0[1433:1433], sram_blwl_1433_configbus1[1433:1433] , sram_blwl_1433_configbus0_b[1433:1433] );
wire [1434:1434] sram_blwl_1434_configbus0;
wire [1434:1434] sram_blwl_1434_configbus1;
wire [1434:1434] sram_blwl_1434_configbus0_b;
assign sram_blwl_1434_configbus0[1434:1434] = sram_blwl_bl[1434:1434] ;
assign sram_blwl_1434_configbus1[1434:1434] = sram_blwl_wl[1434:1434] ;
assign sram_blwl_1434_configbus0_b[1434:1434] = sram_blwl_blb[1434:1434] ;
sram6T_blwl sram_blwl_1434_ (sram_blwl_out[1434], sram_blwl_out[1434], sram_blwl_outb[1434], sram_blwl_1434_configbus0[1434:1434], sram_blwl_1434_configbus1[1434:1434] , sram_blwl_1434_configbus0_b[1434:1434] );
wire [1435:1435] sram_blwl_1435_configbus0;
wire [1435:1435] sram_blwl_1435_configbus1;
wire [1435:1435] sram_blwl_1435_configbus0_b;
assign sram_blwl_1435_configbus0[1435:1435] = sram_blwl_bl[1435:1435] ;
assign sram_blwl_1435_configbus1[1435:1435] = sram_blwl_wl[1435:1435] ;
assign sram_blwl_1435_configbus0_b[1435:1435] = sram_blwl_blb[1435:1435] ;
sram6T_blwl sram_blwl_1435_ (sram_blwl_out[1435], sram_blwl_out[1435], sram_blwl_outb[1435], sram_blwl_1435_configbus0[1435:1435], sram_blwl_1435_configbus1[1435:1435] , sram_blwl_1435_configbus0_b[1435:1435] );
wire [1436:1436] sram_blwl_1436_configbus0;
wire [1436:1436] sram_blwl_1436_configbus1;
wire [1436:1436] sram_blwl_1436_configbus0_b;
assign sram_blwl_1436_configbus0[1436:1436] = sram_blwl_bl[1436:1436] ;
assign sram_blwl_1436_configbus1[1436:1436] = sram_blwl_wl[1436:1436] ;
assign sram_blwl_1436_configbus0_b[1436:1436] = sram_blwl_blb[1436:1436] ;
sram6T_blwl sram_blwl_1436_ (sram_blwl_out[1436], sram_blwl_out[1436], sram_blwl_outb[1436], sram_blwl_1436_configbus0[1436:1436], sram_blwl_1436_configbus1[1436:1436] , sram_blwl_1436_configbus0_b[1436:1436] );
wire [1437:1437] sram_blwl_1437_configbus0;
wire [1437:1437] sram_blwl_1437_configbus1;
wire [1437:1437] sram_blwl_1437_configbus0_b;
assign sram_blwl_1437_configbus0[1437:1437] = sram_blwl_bl[1437:1437] ;
assign sram_blwl_1437_configbus1[1437:1437] = sram_blwl_wl[1437:1437] ;
assign sram_blwl_1437_configbus0_b[1437:1437] = sram_blwl_blb[1437:1437] ;
sram6T_blwl sram_blwl_1437_ (sram_blwl_out[1437], sram_blwl_out[1437], sram_blwl_outb[1437], sram_blwl_1437_configbus0[1437:1437], sram_blwl_1437_configbus1[1437:1437] , sram_blwl_1437_configbus0_b[1437:1437] );
wire [1438:1438] sram_blwl_1438_configbus0;
wire [1438:1438] sram_blwl_1438_configbus1;
wire [1438:1438] sram_blwl_1438_configbus0_b;
assign sram_blwl_1438_configbus0[1438:1438] = sram_blwl_bl[1438:1438] ;
assign sram_blwl_1438_configbus1[1438:1438] = sram_blwl_wl[1438:1438] ;
assign sram_blwl_1438_configbus0_b[1438:1438] = sram_blwl_blb[1438:1438] ;
sram6T_blwl sram_blwl_1438_ (sram_blwl_out[1438], sram_blwl_out[1438], sram_blwl_outb[1438], sram_blwl_1438_configbus0[1438:1438], sram_blwl_1438_configbus1[1438:1438] , sram_blwl_1438_configbus0_b[1438:1438] );
wire [1439:1439] sram_blwl_1439_configbus0;
wire [1439:1439] sram_blwl_1439_configbus1;
wire [1439:1439] sram_blwl_1439_configbus0_b;
assign sram_blwl_1439_configbus0[1439:1439] = sram_blwl_bl[1439:1439] ;
assign sram_blwl_1439_configbus1[1439:1439] = sram_blwl_wl[1439:1439] ;
assign sram_blwl_1439_configbus0_b[1439:1439] = sram_blwl_blb[1439:1439] ;
sram6T_blwl sram_blwl_1439_ (sram_blwl_out[1439], sram_blwl_out[1439], sram_blwl_outb[1439], sram_blwl_1439_configbus0[1439:1439], sram_blwl_1439_configbus1[1439:1439] , sram_blwl_1439_configbus0_b[1439:1439] );
wire [1440:1440] sram_blwl_1440_configbus0;
wire [1440:1440] sram_blwl_1440_configbus1;
wire [1440:1440] sram_blwl_1440_configbus0_b;
assign sram_blwl_1440_configbus0[1440:1440] = sram_blwl_bl[1440:1440] ;
assign sram_blwl_1440_configbus1[1440:1440] = sram_blwl_wl[1440:1440] ;
assign sram_blwl_1440_configbus0_b[1440:1440] = sram_blwl_blb[1440:1440] ;
sram6T_blwl sram_blwl_1440_ (sram_blwl_out[1440], sram_blwl_out[1440], sram_blwl_outb[1440], sram_blwl_1440_configbus0[1440:1440], sram_blwl_1440_configbus1[1440:1440] , sram_blwl_1440_configbus0_b[1440:1440] );
wire [1441:1441] sram_blwl_1441_configbus0;
wire [1441:1441] sram_blwl_1441_configbus1;
wire [1441:1441] sram_blwl_1441_configbus0_b;
assign sram_blwl_1441_configbus0[1441:1441] = sram_blwl_bl[1441:1441] ;
assign sram_blwl_1441_configbus1[1441:1441] = sram_blwl_wl[1441:1441] ;
assign sram_blwl_1441_configbus0_b[1441:1441] = sram_blwl_blb[1441:1441] ;
sram6T_blwl sram_blwl_1441_ (sram_blwl_out[1441], sram_blwl_out[1441], sram_blwl_outb[1441], sram_blwl_1441_configbus0[1441:1441], sram_blwl_1441_configbus1[1441:1441] , sram_blwl_1441_configbus0_b[1441:1441] );
wire [1442:1442] sram_blwl_1442_configbus0;
wire [1442:1442] sram_blwl_1442_configbus1;
wire [1442:1442] sram_blwl_1442_configbus0_b;
assign sram_blwl_1442_configbus0[1442:1442] = sram_blwl_bl[1442:1442] ;
assign sram_blwl_1442_configbus1[1442:1442] = sram_blwl_wl[1442:1442] ;
assign sram_blwl_1442_configbus0_b[1442:1442] = sram_blwl_blb[1442:1442] ;
sram6T_blwl sram_blwl_1442_ (sram_blwl_out[1442], sram_blwl_out[1442], sram_blwl_outb[1442], sram_blwl_1442_configbus0[1442:1442], sram_blwl_1442_configbus1[1442:1442] , sram_blwl_1442_configbus0_b[1442:1442] );
wire [1443:1443] sram_blwl_1443_configbus0;
wire [1443:1443] sram_blwl_1443_configbus1;
wire [1443:1443] sram_blwl_1443_configbus0_b;
assign sram_blwl_1443_configbus0[1443:1443] = sram_blwl_bl[1443:1443] ;
assign sram_blwl_1443_configbus1[1443:1443] = sram_blwl_wl[1443:1443] ;
assign sram_blwl_1443_configbus0_b[1443:1443] = sram_blwl_blb[1443:1443] ;
sram6T_blwl sram_blwl_1443_ (sram_blwl_out[1443], sram_blwl_out[1443], sram_blwl_outb[1443], sram_blwl_1443_configbus0[1443:1443], sram_blwl_1443_configbus1[1443:1443] , sram_blwl_1443_configbus0_b[1443:1443] );
wire [1444:1444] sram_blwl_1444_configbus0;
wire [1444:1444] sram_blwl_1444_configbus1;
wire [1444:1444] sram_blwl_1444_configbus0_b;
assign sram_blwl_1444_configbus0[1444:1444] = sram_blwl_bl[1444:1444] ;
assign sram_blwl_1444_configbus1[1444:1444] = sram_blwl_wl[1444:1444] ;
assign sram_blwl_1444_configbus0_b[1444:1444] = sram_blwl_blb[1444:1444] ;
sram6T_blwl sram_blwl_1444_ (sram_blwl_out[1444], sram_blwl_out[1444], sram_blwl_outb[1444], sram_blwl_1444_configbus0[1444:1444], sram_blwl_1444_configbus1[1444:1444] , sram_blwl_1444_configbus0_b[1444:1444] );
wire [1445:1445] sram_blwl_1445_configbus0;
wire [1445:1445] sram_blwl_1445_configbus1;
wire [1445:1445] sram_blwl_1445_configbus0_b;
assign sram_blwl_1445_configbus0[1445:1445] = sram_blwl_bl[1445:1445] ;
assign sram_blwl_1445_configbus1[1445:1445] = sram_blwl_wl[1445:1445] ;
assign sram_blwl_1445_configbus0_b[1445:1445] = sram_blwl_blb[1445:1445] ;
sram6T_blwl sram_blwl_1445_ (sram_blwl_out[1445], sram_blwl_out[1445], sram_blwl_outb[1445], sram_blwl_1445_configbus0[1445:1445], sram_blwl_1445_configbus1[1445:1445] , sram_blwl_1445_configbus0_b[1445:1445] );
wire [1446:1446] sram_blwl_1446_configbus0;
wire [1446:1446] sram_blwl_1446_configbus1;
wire [1446:1446] sram_blwl_1446_configbus0_b;
assign sram_blwl_1446_configbus0[1446:1446] = sram_blwl_bl[1446:1446] ;
assign sram_blwl_1446_configbus1[1446:1446] = sram_blwl_wl[1446:1446] ;
assign sram_blwl_1446_configbus0_b[1446:1446] = sram_blwl_blb[1446:1446] ;
sram6T_blwl sram_blwl_1446_ (sram_blwl_out[1446], sram_blwl_out[1446], sram_blwl_outb[1446], sram_blwl_1446_configbus0[1446:1446], sram_blwl_1446_configbus1[1446:1446] , sram_blwl_1446_configbus0_b[1446:1446] );
wire [1447:1447] sram_blwl_1447_configbus0;
wire [1447:1447] sram_blwl_1447_configbus1;
wire [1447:1447] sram_blwl_1447_configbus0_b;
assign sram_blwl_1447_configbus0[1447:1447] = sram_blwl_bl[1447:1447] ;
assign sram_blwl_1447_configbus1[1447:1447] = sram_blwl_wl[1447:1447] ;
assign sram_blwl_1447_configbus0_b[1447:1447] = sram_blwl_blb[1447:1447] ;
sram6T_blwl sram_blwl_1447_ (sram_blwl_out[1447], sram_blwl_out[1447], sram_blwl_outb[1447], sram_blwl_1447_configbus0[1447:1447], sram_blwl_1447_configbus1[1447:1447] , sram_blwl_1447_configbus0_b[1447:1447] );
wire [1448:1448] sram_blwl_1448_configbus0;
wire [1448:1448] sram_blwl_1448_configbus1;
wire [1448:1448] sram_blwl_1448_configbus0_b;
assign sram_blwl_1448_configbus0[1448:1448] = sram_blwl_bl[1448:1448] ;
assign sram_blwl_1448_configbus1[1448:1448] = sram_blwl_wl[1448:1448] ;
assign sram_blwl_1448_configbus0_b[1448:1448] = sram_blwl_blb[1448:1448] ;
sram6T_blwl sram_blwl_1448_ (sram_blwl_out[1448], sram_blwl_out[1448], sram_blwl_outb[1448], sram_blwl_1448_configbus0[1448:1448], sram_blwl_1448_configbus1[1448:1448] , sram_blwl_1448_configbus0_b[1448:1448] );
wire [1449:1449] sram_blwl_1449_configbus0;
wire [1449:1449] sram_blwl_1449_configbus1;
wire [1449:1449] sram_blwl_1449_configbus0_b;
assign sram_blwl_1449_configbus0[1449:1449] = sram_blwl_bl[1449:1449] ;
assign sram_blwl_1449_configbus1[1449:1449] = sram_blwl_wl[1449:1449] ;
assign sram_blwl_1449_configbus0_b[1449:1449] = sram_blwl_blb[1449:1449] ;
sram6T_blwl sram_blwl_1449_ (sram_blwl_out[1449], sram_blwl_out[1449], sram_blwl_outb[1449], sram_blwl_1449_configbus0[1449:1449], sram_blwl_1449_configbus1[1449:1449] , sram_blwl_1449_configbus0_b[1449:1449] );
wire [1450:1450] sram_blwl_1450_configbus0;
wire [1450:1450] sram_blwl_1450_configbus1;
wire [1450:1450] sram_blwl_1450_configbus0_b;
assign sram_blwl_1450_configbus0[1450:1450] = sram_blwl_bl[1450:1450] ;
assign sram_blwl_1450_configbus1[1450:1450] = sram_blwl_wl[1450:1450] ;
assign sram_blwl_1450_configbus0_b[1450:1450] = sram_blwl_blb[1450:1450] ;
sram6T_blwl sram_blwl_1450_ (sram_blwl_out[1450], sram_blwl_out[1450], sram_blwl_outb[1450], sram_blwl_1450_configbus0[1450:1450], sram_blwl_1450_configbus1[1450:1450] , sram_blwl_1450_configbus0_b[1450:1450] );
wire [1451:1451] sram_blwl_1451_configbus0;
wire [1451:1451] sram_blwl_1451_configbus1;
wire [1451:1451] sram_blwl_1451_configbus0_b;
assign sram_blwl_1451_configbus0[1451:1451] = sram_blwl_bl[1451:1451] ;
assign sram_blwl_1451_configbus1[1451:1451] = sram_blwl_wl[1451:1451] ;
assign sram_blwl_1451_configbus0_b[1451:1451] = sram_blwl_blb[1451:1451] ;
sram6T_blwl sram_blwl_1451_ (sram_blwl_out[1451], sram_blwl_out[1451], sram_blwl_outb[1451], sram_blwl_1451_configbus0[1451:1451], sram_blwl_1451_configbus1[1451:1451] , sram_blwl_1451_configbus0_b[1451:1451] );
wire [1452:1452] sram_blwl_1452_configbus0;
wire [1452:1452] sram_blwl_1452_configbus1;
wire [1452:1452] sram_blwl_1452_configbus0_b;
assign sram_blwl_1452_configbus0[1452:1452] = sram_blwl_bl[1452:1452] ;
assign sram_blwl_1452_configbus1[1452:1452] = sram_blwl_wl[1452:1452] ;
assign sram_blwl_1452_configbus0_b[1452:1452] = sram_blwl_blb[1452:1452] ;
sram6T_blwl sram_blwl_1452_ (sram_blwl_out[1452], sram_blwl_out[1452], sram_blwl_outb[1452], sram_blwl_1452_configbus0[1452:1452], sram_blwl_1452_configbus1[1452:1452] , sram_blwl_1452_configbus0_b[1452:1452] );
wire [1453:1453] sram_blwl_1453_configbus0;
wire [1453:1453] sram_blwl_1453_configbus1;
wire [1453:1453] sram_blwl_1453_configbus0_b;
assign sram_blwl_1453_configbus0[1453:1453] = sram_blwl_bl[1453:1453] ;
assign sram_blwl_1453_configbus1[1453:1453] = sram_blwl_wl[1453:1453] ;
assign sram_blwl_1453_configbus0_b[1453:1453] = sram_blwl_blb[1453:1453] ;
sram6T_blwl sram_blwl_1453_ (sram_blwl_out[1453], sram_blwl_out[1453], sram_blwl_outb[1453], sram_blwl_1453_configbus0[1453:1453], sram_blwl_1453_configbus1[1453:1453] , sram_blwl_1453_configbus0_b[1453:1453] );
wire [1454:1454] sram_blwl_1454_configbus0;
wire [1454:1454] sram_blwl_1454_configbus1;
wire [1454:1454] sram_blwl_1454_configbus0_b;
assign sram_blwl_1454_configbus0[1454:1454] = sram_blwl_bl[1454:1454] ;
assign sram_blwl_1454_configbus1[1454:1454] = sram_blwl_wl[1454:1454] ;
assign sram_blwl_1454_configbus0_b[1454:1454] = sram_blwl_blb[1454:1454] ;
sram6T_blwl sram_blwl_1454_ (sram_blwl_out[1454], sram_blwl_out[1454], sram_blwl_outb[1454], sram_blwl_1454_configbus0[1454:1454], sram_blwl_1454_configbus1[1454:1454] , sram_blwl_1454_configbus0_b[1454:1454] );
wire [1455:1455] sram_blwl_1455_configbus0;
wire [1455:1455] sram_blwl_1455_configbus1;
wire [1455:1455] sram_blwl_1455_configbus0_b;
assign sram_blwl_1455_configbus0[1455:1455] = sram_blwl_bl[1455:1455] ;
assign sram_blwl_1455_configbus1[1455:1455] = sram_blwl_wl[1455:1455] ;
assign sram_blwl_1455_configbus0_b[1455:1455] = sram_blwl_blb[1455:1455] ;
sram6T_blwl sram_blwl_1455_ (sram_blwl_out[1455], sram_blwl_out[1455], sram_blwl_outb[1455], sram_blwl_1455_configbus0[1455:1455], sram_blwl_1455_configbus1[1455:1455] , sram_blwl_1455_configbus0_b[1455:1455] );
wire [1456:1456] sram_blwl_1456_configbus0;
wire [1456:1456] sram_blwl_1456_configbus1;
wire [1456:1456] sram_blwl_1456_configbus0_b;
assign sram_blwl_1456_configbus0[1456:1456] = sram_blwl_bl[1456:1456] ;
assign sram_blwl_1456_configbus1[1456:1456] = sram_blwl_wl[1456:1456] ;
assign sram_blwl_1456_configbus0_b[1456:1456] = sram_blwl_blb[1456:1456] ;
sram6T_blwl sram_blwl_1456_ (sram_blwl_out[1456], sram_blwl_out[1456], sram_blwl_outb[1456], sram_blwl_1456_configbus0[1456:1456], sram_blwl_1456_configbus1[1456:1456] , sram_blwl_1456_configbus0_b[1456:1456] );
wire [1457:1457] sram_blwl_1457_configbus0;
wire [1457:1457] sram_blwl_1457_configbus1;
wire [1457:1457] sram_blwl_1457_configbus0_b;
assign sram_blwl_1457_configbus0[1457:1457] = sram_blwl_bl[1457:1457] ;
assign sram_blwl_1457_configbus1[1457:1457] = sram_blwl_wl[1457:1457] ;
assign sram_blwl_1457_configbus0_b[1457:1457] = sram_blwl_blb[1457:1457] ;
sram6T_blwl sram_blwl_1457_ (sram_blwl_out[1457], sram_blwl_out[1457], sram_blwl_outb[1457], sram_blwl_1457_configbus0[1457:1457], sram_blwl_1457_configbus1[1457:1457] , sram_blwl_1457_configbus0_b[1457:1457] );
wire [1458:1458] sram_blwl_1458_configbus0;
wire [1458:1458] sram_blwl_1458_configbus1;
wire [1458:1458] sram_blwl_1458_configbus0_b;
assign sram_blwl_1458_configbus0[1458:1458] = sram_blwl_bl[1458:1458] ;
assign sram_blwl_1458_configbus1[1458:1458] = sram_blwl_wl[1458:1458] ;
assign sram_blwl_1458_configbus0_b[1458:1458] = sram_blwl_blb[1458:1458] ;
sram6T_blwl sram_blwl_1458_ (sram_blwl_out[1458], sram_blwl_out[1458], sram_blwl_outb[1458], sram_blwl_1458_configbus0[1458:1458], sram_blwl_1458_configbus1[1458:1458] , sram_blwl_1458_configbus0_b[1458:1458] );
wire [1459:1459] sram_blwl_1459_configbus0;
wire [1459:1459] sram_blwl_1459_configbus1;
wire [1459:1459] sram_blwl_1459_configbus0_b;
assign sram_blwl_1459_configbus0[1459:1459] = sram_blwl_bl[1459:1459] ;
assign sram_blwl_1459_configbus1[1459:1459] = sram_blwl_wl[1459:1459] ;
assign sram_blwl_1459_configbus0_b[1459:1459] = sram_blwl_blb[1459:1459] ;
sram6T_blwl sram_blwl_1459_ (sram_blwl_out[1459], sram_blwl_out[1459], sram_blwl_outb[1459], sram_blwl_1459_configbus0[1459:1459], sram_blwl_1459_configbus1[1459:1459] , sram_blwl_1459_configbus0_b[1459:1459] );
wire [1460:1460] sram_blwl_1460_configbus0;
wire [1460:1460] sram_blwl_1460_configbus1;
wire [1460:1460] sram_blwl_1460_configbus0_b;
assign sram_blwl_1460_configbus0[1460:1460] = sram_blwl_bl[1460:1460] ;
assign sram_blwl_1460_configbus1[1460:1460] = sram_blwl_wl[1460:1460] ;
assign sram_blwl_1460_configbus0_b[1460:1460] = sram_blwl_blb[1460:1460] ;
sram6T_blwl sram_blwl_1460_ (sram_blwl_out[1460], sram_blwl_out[1460], sram_blwl_outb[1460], sram_blwl_1460_configbus0[1460:1460], sram_blwl_1460_configbus1[1460:1460] , sram_blwl_1460_configbus0_b[1460:1460] );
wire [1461:1461] sram_blwl_1461_configbus0;
wire [1461:1461] sram_blwl_1461_configbus1;
wire [1461:1461] sram_blwl_1461_configbus0_b;
assign sram_blwl_1461_configbus0[1461:1461] = sram_blwl_bl[1461:1461] ;
assign sram_blwl_1461_configbus1[1461:1461] = sram_blwl_wl[1461:1461] ;
assign sram_blwl_1461_configbus0_b[1461:1461] = sram_blwl_blb[1461:1461] ;
sram6T_blwl sram_blwl_1461_ (sram_blwl_out[1461], sram_blwl_out[1461], sram_blwl_outb[1461], sram_blwl_1461_configbus0[1461:1461], sram_blwl_1461_configbus1[1461:1461] , sram_blwl_1461_configbus0_b[1461:1461] );
wire [1462:1462] sram_blwl_1462_configbus0;
wire [1462:1462] sram_blwl_1462_configbus1;
wire [1462:1462] sram_blwl_1462_configbus0_b;
assign sram_blwl_1462_configbus0[1462:1462] = sram_blwl_bl[1462:1462] ;
assign sram_blwl_1462_configbus1[1462:1462] = sram_blwl_wl[1462:1462] ;
assign sram_blwl_1462_configbus0_b[1462:1462] = sram_blwl_blb[1462:1462] ;
sram6T_blwl sram_blwl_1462_ (sram_blwl_out[1462], sram_blwl_out[1462], sram_blwl_outb[1462], sram_blwl_1462_configbus0[1462:1462], sram_blwl_1462_configbus1[1462:1462] , sram_blwl_1462_configbus0_b[1462:1462] );
wire [1463:1463] sram_blwl_1463_configbus0;
wire [1463:1463] sram_blwl_1463_configbus1;
wire [1463:1463] sram_blwl_1463_configbus0_b;
assign sram_blwl_1463_configbus0[1463:1463] = sram_blwl_bl[1463:1463] ;
assign sram_blwl_1463_configbus1[1463:1463] = sram_blwl_wl[1463:1463] ;
assign sram_blwl_1463_configbus0_b[1463:1463] = sram_blwl_blb[1463:1463] ;
sram6T_blwl sram_blwl_1463_ (sram_blwl_out[1463], sram_blwl_out[1463], sram_blwl_outb[1463], sram_blwl_1463_configbus0[1463:1463], sram_blwl_1463_configbus1[1463:1463] , sram_blwl_1463_configbus0_b[1463:1463] );
wire [1464:1464] sram_blwl_1464_configbus0;
wire [1464:1464] sram_blwl_1464_configbus1;
wire [1464:1464] sram_blwl_1464_configbus0_b;
assign sram_blwl_1464_configbus0[1464:1464] = sram_blwl_bl[1464:1464] ;
assign sram_blwl_1464_configbus1[1464:1464] = sram_blwl_wl[1464:1464] ;
assign sram_blwl_1464_configbus0_b[1464:1464] = sram_blwl_blb[1464:1464] ;
sram6T_blwl sram_blwl_1464_ (sram_blwl_out[1464], sram_blwl_out[1464], sram_blwl_outb[1464], sram_blwl_1464_configbus0[1464:1464], sram_blwl_1464_configbus1[1464:1464] , sram_blwl_1464_configbus0_b[1464:1464] );
wire [1465:1465] sram_blwl_1465_configbus0;
wire [1465:1465] sram_blwl_1465_configbus1;
wire [1465:1465] sram_blwl_1465_configbus0_b;
assign sram_blwl_1465_configbus0[1465:1465] = sram_blwl_bl[1465:1465] ;
assign sram_blwl_1465_configbus1[1465:1465] = sram_blwl_wl[1465:1465] ;
assign sram_blwl_1465_configbus0_b[1465:1465] = sram_blwl_blb[1465:1465] ;
sram6T_blwl sram_blwl_1465_ (sram_blwl_out[1465], sram_blwl_out[1465], sram_blwl_outb[1465], sram_blwl_1465_configbus0[1465:1465], sram_blwl_1465_configbus1[1465:1465] , sram_blwl_1465_configbus0_b[1465:1465] );
wire [1466:1466] sram_blwl_1466_configbus0;
wire [1466:1466] sram_blwl_1466_configbus1;
wire [1466:1466] sram_blwl_1466_configbus0_b;
assign sram_blwl_1466_configbus0[1466:1466] = sram_blwl_bl[1466:1466] ;
assign sram_blwl_1466_configbus1[1466:1466] = sram_blwl_wl[1466:1466] ;
assign sram_blwl_1466_configbus0_b[1466:1466] = sram_blwl_blb[1466:1466] ;
sram6T_blwl sram_blwl_1466_ (sram_blwl_out[1466], sram_blwl_out[1466], sram_blwl_outb[1466], sram_blwl_1466_configbus0[1466:1466], sram_blwl_1466_configbus1[1466:1466] , sram_blwl_1466_configbus0_b[1466:1466] );
wire [1467:1467] sram_blwl_1467_configbus0;
wire [1467:1467] sram_blwl_1467_configbus1;
wire [1467:1467] sram_blwl_1467_configbus0_b;
assign sram_blwl_1467_configbus0[1467:1467] = sram_blwl_bl[1467:1467] ;
assign sram_blwl_1467_configbus1[1467:1467] = sram_blwl_wl[1467:1467] ;
assign sram_blwl_1467_configbus0_b[1467:1467] = sram_blwl_blb[1467:1467] ;
sram6T_blwl sram_blwl_1467_ (sram_blwl_out[1467], sram_blwl_out[1467], sram_blwl_outb[1467], sram_blwl_1467_configbus0[1467:1467], sram_blwl_1467_configbus1[1467:1467] , sram_blwl_1467_configbus0_b[1467:1467] );
wire [1468:1468] sram_blwl_1468_configbus0;
wire [1468:1468] sram_blwl_1468_configbus1;
wire [1468:1468] sram_blwl_1468_configbus0_b;
assign sram_blwl_1468_configbus0[1468:1468] = sram_blwl_bl[1468:1468] ;
assign sram_blwl_1468_configbus1[1468:1468] = sram_blwl_wl[1468:1468] ;
assign sram_blwl_1468_configbus0_b[1468:1468] = sram_blwl_blb[1468:1468] ;
sram6T_blwl sram_blwl_1468_ (sram_blwl_out[1468], sram_blwl_out[1468], sram_blwl_outb[1468], sram_blwl_1468_configbus0[1468:1468], sram_blwl_1468_configbus1[1468:1468] , sram_blwl_1468_configbus0_b[1468:1468] );
wire [1469:1469] sram_blwl_1469_configbus0;
wire [1469:1469] sram_blwl_1469_configbus1;
wire [1469:1469] sram_blwl_1469_configbus0_b;
assign sram_blwl_1469_configbus0[1469:1469] = sram_blwl_bl[1469:1469] ;
assign sram_blwl_1469_configbus1[1469:1469] = sram_blwl_wl[1469:1469] ;
assign sram_blwl_1469_configbus0_b[1469:1469] = sram_blwl_blb[1469:1469] ;
sram6T_blwl sram_blwl_1469_ (sram_blwl_out[1469], sram_blwl_out[1469], sram_blwl_outb[1469], sram_blwl_1469_configbus0[1469:1469], sram_blwl_1469_configbus1[1469:1469] , sram_blwl_1469_configbus0_b[1469:1469] );
endmodule
//----- END LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----

//----- Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----
module grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
input [0:0] Set,
input [0:0] Reset,
input [0:0] clk
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
input wire ff_0___D_0_,
output wire ff_0___Q_0_);
static_dff dff_6_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
endmodule
//----- END Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6_ -----
module grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_ble6___in_0_,
input wire mode_ble6___in_1_,
input wire mode_ble6___in_2_,
input wire mode_ble6___in_3_,
input wire mode_ble6___in_4_,
input wire mode_ble6___in_5_,
output wire mode_ble6___out_0_,
input wire mode_ble6___clk_0_,
input [1406:1470] sram_blwl_bl ,
input [1406:1470] sram_blwl_wl ,
input [1406:1470] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ lut6_0_ (
 lut6_0___in_0_,  lut6_0___in_1_,  lut6_0___in_2_,  lut6_0___in_3_,  lut6_0___in_4_,  lut6_0___in_5_,  lut6_0___out_0_,
sram_blwl_bl[1406:1469] ,
sram_blwl_wl[1406:1469] ,
sram_blwl_blb[1406:1469] );
grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ ff_0_ (
//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
wire [0:1] in_bus_mux_1level_tapbuf_size2_406_ ;
assign in_bus_mux_1level_tapbuf_size2_406_[0] = ff_0___Q_0_ ; 
assign in_bus_mux_1level_tapbuf_size2_406_[1] = lut6_0___out_0_ ; 
wire [1470:1470] mux_1level_tapbuf_size2_406_configbus0;
wire [1470:1470] mux_1level_tapbuf_size2_406_configbus1;
wire [1470:1470] mux_1level_tapbuf_size2_406_sram_blwl_out ;
wire [1470:1470] mux_1level_tapbuf_size2_406_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_406_configbus0[1470:1470] = sram_blwl_bl[1470:1470] ;
assign mux_1level_tapbuf_size2_406_configbus1[1470:1470] = sram_blwl_wl[1470:1470] ;
wire [1470:1470] mux_1level_tapbuf_size2_406_configbus0_b;
assign mux_1level_tapbuf_size2_406_configbus0_b[1470:1470] = sram_blwl_blb[1470:1470] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_406_ (in_bus_mux_1level_tapbuf_size2_406_, mode_ble6___out_0_, mux_1level_tapbuf_size2_406_sram_blwl_out[1470:1470] ,
mux_1level_tapbuf_size2_406_sram_blwl_outb[1470:1470] );
//----- SRAM bits for MUX[406], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_1470_ (mux_1level_tapbuf_size2_406_sram_blwl_out[1470:1470] ,mux_1level_tapbuf_size2_406_sram_blwl_out[1470:1470] ,mux_1level_tapbuf_size2_406_sram_blwl_outb[1470:1470] ,mux_1level_tapbuf_size2_406_configbus0[1470:1470], mux_1level_tapbuf_size2_406_configbus1[1470:1470] , mux_1level_tapbuf_size2_406_configbus0_b[1470:1470] );
direct_interc direct_interc_96_ (mode_ble6___in_0_, lut6_0___in_0_ );
direct_interc direct_interc_97_ (mode_ble6___in_1_, lut6_0___in_1_ );
direct_interc direct_interc_98_ (mode_ble6___in_2_, lut6_0___in_2_ );
direct_interc direct_interc_99_ (mode_ble6___in_3_, lut6_0___in_3_ );
direct_interc direct_interc_100_ (mode_ble6___in_4_, lut6_0___in_4_ );
direct_interc direct_interc_101_ (mode_ble6___in_5_, lut6_0___in_5_ );
direct_interc direct_interc_102_ (lut6_0___out_0_, ff_0___D_0_ );
direct_interc direct_interc_103_ (mode_ble6___clk_0_, ff_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6_ -----
module grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_n1_lut6___in_0_,
input wire mode_n1_lut6___in_1_,
input wire mode_n1_lut6___in_2_,
input wire mode_n1_lut6___in_3_,
input wire mode_n1_lut6___in_4_,
input wire mode_n1_lut6___in_5_,
output wire mode_n1_lut6___out_0_,
input wire mode_n1_lut6___clk_0_,
input [1406:1470] sram_blwl_bl ,
input [1406:1470] sram_blwl_wl ,
input [1406:1470] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6__ble6_0__mode_ble6_ ble6_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 ble6_0___in_0_,  ble6_0___in_1_,  ble6_0___in_2_,  ble6_0___in_3_,  ble6_0___in_4_,  ble6_0___in_5_,  ble6_0___out_0_,  ble6_0___clk_0_,
sram_blwl_bl[1406:1470] ,
sram_blwl_wl[1406:1470] ,
sram_blwl_blb[1406:1470] );
direct_interc direct_interc_104_ (ble6_0___out_0_, mode_n1_lut6___out_0_ );
direct_interc direct_interc_105_ (mode_n1_lut6___in_0_, ble6_0___in_0_ );
direct_interc direct_interc_106_ (mode_n1_lut6___in_1_, ble6_0___in_1_ );
direct_interc direct_interc_107_ (mode_n1_lut6___in_2_, ble6_0___in_2_ );
direct_interc direct_interc_108_ (mode_n1_lut6___in_3_, ble6_0___in_3_ );
direct_interc direct_interc_109_ (mode_n1_lut6___in_4_, ble6_0___in_4_ );
direct_interc direct_interc_110_ (mode_n1_lut6___in_5_, ble6_0___in_5_ );
direct_interc direct_interc_111_ (mode_n1_lut6___clk_0_, ble6_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6_ -----

//----- LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----
module grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ (
input wire lut6_0___in_0_,
input wire lut6_0___in_1_,
input wire lut6_0___in_2_,
input wire lut6_0___in_3_,
input wire lut6_0___in_4_,
input wire lut6_0___in_5_,
output wire lut6_0___out_0_,
input [1471:1534] sram_blwl_bl ,
input [1471:1534] sram_blwl_wl ,
input [1471:1534] sram_blwl_blb );
wire [0:5] lut6_0___in;
assign lut6_0___in[0] = lut6_0___in_0_;
assign lut6_0___in[1] = lut6_0___in_1_;
assign lut6_0___in[2] = lut6_0___in_2_;
assign lut6_0___in[3] = lut6_0___in_3_;
assign lut6_0___in[4] = lut6_0___in_4_;
assign lut6_0___in[5] = lut6_0___in_5_;
wire [0:0] lut6_0___out;
assign lut6_0___out_0_ = lut6_0___out[0];
wire [1471:1534] sram_blwl_out ;
wire [1471:1534] sram_blwl_outb ;
lut6 lut6_7_ (
//----- Input and output ports -----
 lut6_0___in[0:5] ,  lut6_0___out[0:0],//----- SRAM ports -----
sram_blwl_out[1471:1534] , sram_blwl_outb[1471:1534] );
//----- Truth Table for LUT[7], size=6. -----
//----- SRAM bits for LUT[7], size=6, num_sram=64. -----
//-----0000000000000000000000000000000000000000000000000000000000000000-----
wire [1471:1471] sram_blwl_1471_configbus0;
wire [1471:1471] sram_blwl_1471_configbus1;
wire [1471:1471] sram_blwl_1471_configbus0_b;
assign sram_blwl_1471_configbus0[1471:1471] = sram_blwl_bl[1471:1471] ;
assign sram_blwl_1471_configbus1[1471:1471] = sram_blwl_wl[1471:1471] ;
assign sram_blwl_1471_configbus0_b[1471:1471] = sram_blwl_blb[1471:1471] ;
sram6T_blwl sram_blwl_1471_ (sram_blwl_out[1471], sram_blwl_out[1471], sram_blwl_outb[1471], sram_blwl_1471_configbus0[1471:1471], sram_blwl_1471_configbus1[1471:1471] , sram_blwl_1471_configbus0_b[1471:1471] );
wire [1472:1472] sram_blwl_1472_configbus0;
wire [1472:1472] sram_blwl_1472_configbus1;
wire [1472:1472] sram_blwl_1472_configbus0_b;
assign sram_blwl_1472_configbus0[1472:1472] = sram_blwl_bl[1472:1472] ;
assign sram_blwl_1472_configbus1[1472:1472] = sram_blwl_wl[1472:1472] ;
assign sram_blwl_1472_configbus0_b[1472:1472] = sram_blwl_blb[1472:1472] ;
sram6T_blwl sram_blwl_1472_ (sram_blwl_out[1472], sram_blwl_out[1472], sram_blwl_outb[1472], sram_blwl_1472_configbus0[1472:1472], sram_blwl_1472_configbus1[1472:1472] , sram_blwl_1472_configbus0_b[1472:1472] );
wire [1473:1473] sram_blwl_1473_configbus0;
wire [1473:1473] sram_blwl_1473_configbus1;
wire [1473:1473] sram_blwl_1473_configbus0_b;
assign sram_blwl_1473_configbus0[1473:1473] = sram_blwl_bl[1473:1473] ;
assign sram_blwl_1473_configbus1[1473:1473] = sram_blwl_wl[1473:1473] ;
assign sram_blwl_1473_configbus0_b[1473:1473] = sram_blwl_blb[1473:1473] ;
sram6T_blwl sram_blwl_1473_ (sram_blwl_out[1473], sram_blwl_out[1473], sram_blwl_outb[1473], sram_blwl_1473_configbus0[1473:1473], sram_blwl_1473_configbus1[1473:1473] , sram_blwl_1473_configbus0_b[1473:1473] );
wire [1474:1474] sram_blwl_1474_configbus0;
wire [1474:1474] sram_blwl_1474_configbus1;
wire [1474:1474] sram_blwl_1474_configbus0_b;
assign sram_blwl_1474_configbus0[1474:1474] = sram_blwl_bl[1474:1474] ;
assign sram_blwl_1474_configbus1[1474:1474] = sram_blwl_wl[1474:1474] ;
assign sram_blwl_1474_configbus0_b[1474:1474] = sram_blwl_blb[1474:1474] ;
sram6T_blwl sram_blwl_1474_ (sram_blwl_out[1474], sram_blwl_out[1474], sram_blwl_outb[1474], sram_blwl_1474_configbus0[1474:1474], sram_blwl_1474_configbus1[1474:1474] , sram_blwl_1474_configbus0_b[1474:1474] );
wire [1475:1475] sram_blwl_1475_configbus0;
wire [1475:1475] sram_blwl_1475_configbus1;
wire [1475:1475] sram_blwl_1475_configbus0_b;
assign sram_blwl_1475_configbus0[1475:1475] = sram_blwl_bl[1475:1475] ;
assign sram_blwl_1475_configbus1[1475:1475] = sram_blwl_wl[1475:1475] ;
assign sram_blwl_1475_configbus0_b[1475:1475] = sram_blwl_blb[1475:1475] ;
sram6T_blwl sram_blwl_1475_ (sram_blwl_out[1475], sram_blwl_out[1475], sram_blwl_outb[1475], sram_blwl_1475_configbus0[1475:1475], sram_blwl_1475_configbus1[1475:1475] , sram_blwl_1475_configbus0_b[1475:1475] );
wire [1476:1476] sram_blwl_1476_configbus0;
wire [1476:1476] sram_blwl_1476_configbus1;
wire [1476:1476] sram_blwl_1476_configbus0_b;
assign sram_blwl_1476_configbus0[1476:1476] = sram_blwl_bl[1476:1476] ;
assign sram_blwl_1476_configbus1[1476:1476] = sram_blwl_wl[1476:1476] ;
assign sram_blwl_1476_configbus0_b[1476:1476] = sram_blwl_blb[1476:1476] ;
sram6T_blwl sram_blwl_1476_ (sram_blwl_out[1476], sram_blwl_out[1476], sram_blwl_outb[1476], sram_blwl_1476_configbus0[1476:1476], sram_blwl_1476_configbus1[1476:1476] , sram_blwl_1476_configbus0_b[1476:1476] );
wire [1477:1477] sram_blwl_1477_configbus0;
wire [1477:1477] sram_blwl_1477_configbus1;
wire [1477:1477] sram_blwl_1477_configbus0_b;
assign sram_blwl_1477_configbus0[1477:1477] = sram_blwl_bl[1477:1477] ;
assign sram_blwl_1477_configbus1[1477:1477] = sram_blwl_wl[1477:1477] ;
assign sram_blwl_1477_configbus0_b[1477:1477] = sram_blwl_blb[1477:1477] ;
sram6T_blwl sram_blwl_1477_ (sram_blwl_out[1477], sram_blwl_out[1477], sram_blwl_outb[1477], sram_blwl_1477_configbus0[1477:1477], sram_blwl_1477_configbus1[1477:1477] , sram_blwl_1477_configbus0_b[1477:1477] );
wire [1478:1478] sram_blwl_1478_configbus0;
wire [1478:1478] sram_blwl_1478_configbus1;
wire [1478:1478] sram_blwl_1478_configbus0_b;
assign sram_blwl_1478_configbus0[1478:1478] = sram_blwl_bl[1478:1478] ;
assign sram_blwl_1478_configbus1[1478:1478] = sram_blwl_wl[1478:1478] ;
assign sram_blwl_1478_configbus0_b[1478:1478] = sram_blwl_blb[1478:1478] ;
sram6T_blwl sram_blwl_1478_ (sram_blwl_out[1478], sram_blwl_out[1478], sram_blwl_outb[1478], sram_blwl_1478_configbus0[1478:1478], sram_blwl_1478_configbus1[1478:1478] , sram_blwl_1478_configbus0_b[1478:1478] );
wire [1479:1479] sram_blwl_1479_configbus0;
wire [1479:1479] sram_blwl_1479_configbus1;
wire [1479:1479] sram_blwl_1479_configbus0_b;
assign sram_blwl_1479_configbus0[1479:1479] = sram_blwl_bl[1479:1479] ;
assign sram_blwl_1479_configbus1[1479:1479] = sram_blwl_wl[1479:1479] ;
assign sram_blwl_1479_configbus0_b[1479:1479] = sram_blwl_blb[1479:1479] ;
sram6T_blwl sram_blwl_1479_ (sram_blwl_out[1479], sram_blwl_out[1479], sram_blwl_outb[1479], sram_blwl_1479_configbus0[1479:1479], sram_blwl_1479_configbus1[1479:1479] , sram_blwl_1479_configbus0_b[1479:1479] );
wire [1480:1480] sram_blwl_1480_configbus0;
wire [1480:1480] sram_blwl_1480_configbus1;
wire [1480:1480] sram_blwl_1480_configbus0_b;
assign sram_blwl_1480_configbus0[1480:1480] = sram_blwl_bl[1480:1480] ;
assign sram_blwl_1480_configbus1[1480:1480] = sram_blwl_wl[1480:1480] ;
assign sram_blwl_1480_configbus0_b[1480:1480] = sram_blwl_blb[1480:1480] ;
sram6T_blwl sram_blwl_1480_ (sram_blwl_out[1480], sram_blwl_out[1480], sram_blwl_outb[1480], sram_blwl_1480_configbus0[1480:1480], sram_blwl_1480_configbus1[1480:1480] , sram_blwl_1480_configbus0_b[1480:1480] );
wire [1481:1481] sram_blwl_1481_configbus0;
wire [1481:1481] sram_blwl_1481_configbus1;
wire [1481:1481] sram_blwl_1481_configbus0_b;
assign sram_blwl_1481_configbus0[1481:1481] = sram_blwl_bl[1481:1481] ;
assign sram_blwl_1481_configbus1[1481:1481] = sram_blwl_wl[1481:1481] ;
assign sram_blwl_1481_configbus0_b[1481:1481] = sram_blwl_blb[1481:1481] ;
sram6T_blwl sram_blwl_1481_ (sram_blwl_out[1481], sram_blwl_out[1481], sram_blwl_outb[1481], sram_blwl_1481_configbus0[1481:1481], sram_blwl_1481_configbus1[1481:1481] , sram_blwl_1481_configbus0_b[1481:1481] );
wire [1482:1482] sram_blwl_1482_configbus0;
wire [1482:1482] sram_blwl_1482_configbus1;
wire [1482:1482] sram_blwl_1482_configbus0_b;
assign sram_blwl_1482_configbus0[1482:1482] = sram_blwl_bl[1482:1482] ;
assign sram_blwl_1482_configbus1[1482:1482] = sram_blwl_wl[1482:1482] ;
assign sram_blwl_1482_configbus0_b[1482:1482] = sram_blwl_blb[1482:1482] ;
sram6T_blwl sram_blwl_1482_ (sram_blwl_out[1482], sram_blwl_out[1482], sram_blwl_outb[1482], sram_blwl_1482_configbus0[1482:1482], sram_blwl_1482_configbus1[1482:1482] , sram_blwl_1482_configbus0_b[1482:1482] );
wire [1483:1483] sram_blwl_1483_configbus0;
wire [1483:1483] sram_blwl_1483_configbus1;
wire [1483:1483] sram_blwl_1483_configbus0_b;
assign sram_blwl_1483_configbus0[1483:1483] = sram_blwl_bl[1483:1483] ;
assign sram_blwl_1483_configbus1[1483:1483] = sram_blwl_wl[1483:1483] ;
assign sram_blwl_1483_configbus0_b[1483:1483] = sram_blwl_blb[1483:1483] ;
sram6T_blwl sram_blwl_1483_ (sram_blwl_out[1483], sram_blwl_out[1483], sram_blwl_outb[1483], sram_blwl_1483_configbus0[1483:1483], sram_blwl_1483_configbus1[1483:1483] , sram_blwl_1483_configbus0_b[1483:1483] );
wire [1484:1484] sram_blwl_1484_configbus0;
wire [1484:1484] sram_blwl_1484_configbus1;
wire [1484:1484] sram_blwl_1484_configbus0_b;
assign sram_blwl_1484_configbus0[1484:1484] = sram_blwl_bl[1484:1484] ;
assign sram_blwl_1484_configbus1[1484:1484] = sram_blwl_wl[1484:1484] ;
assign sram_blwl_1484_configbus0_b[1484:1484] = sram_blwl_blb[1484:1484] ;
sram6T_blwl sram_blwl_1484_ (sram_blwl_out[1484], sram_blwl_out[1484], sram_blwl_outb[1484], sram_blwl_1484_configbus0[1484:1484], sram_blwl_1484_configbus1[1484:1484] , sram_blwl_1484_configbus0_b[1484:1484] );
wire [1485:1485] sram_blwl_1485_configbus0;
wire [1485:1485] sram_blwl_1485_configbus1;
wire [1485:1485] sram_blwl_1485_configbus0_b;
assign sram_blwl_1485_configbus0[1485:1485] = sram_blwl_bl[1485:1485] ;
assign sram_blwl_1485_configbus1[1485:1485] = sram_blwl_wl[1485:1485] ;
assign sram_blwl_1485_configbus0_b[1485:1485] = sram_blwl_blb[1485:1485] ;
sram6T_blwl sram_blwl_1485_ (sram_blwl_out[1485], sram_blwl_out[1485], sram_blwl_outb[1485], sram_blwl_1485_configbus0[1485:1485], sram_blwl_1485_configbus1[1485:1485] , sram_blwl_1485_configbus0_b[1485:1485] );
wire [1486:1486] sram_blwl_1486_configbus0;
wire [1486:1486] sram_blwl_1486_configbus1;
wire [1486:1486] sram_blwl_1486_configbus0_b;
assign sram_blwl_1486_configbus0[1486:1486] = sram_blwl_bl[1486:1486] ;
assign sram_blwl_1486_configbus1[1486:1486] = sram_blwl_wl[1486:1486] ;
assign sram_blwl_1486_configbus0_b[1486:1486] = sram_blwl_blb[1486:1486] ;
sram6T_blwl sram_blwl_1486_ (sram_blwl_out[1486], sram_blwl_out[1486], sram_blwl_outb[1486], sram_blwl_1486_configbus0[1486:1486], sram_blwl_1486_configbus1[1486:1486] , sram_blwl_1486_configbus0_b[1486:1486] );
wire [1487:1487] sram_blwl_1487_configbus0;
wire [1487:1487] sram_blwl_1487_configbus1;
wire [1487:1487] sram_blwl_1487_configbus0_b;
assign sram_blwl_1487_configbus0[1487:1487] = sram_blwl_bl[1487:1487] ;
assign sram_blwl_1487_configbus1[1487:1487] = sram_blwl_wl[1487:1487] ;
assign sram_blwl_1487_configbus0_b[1487:1487] = sram_blwl_blb[1487:1487] ;
sram6T_blwl sram_blwl_1487_ (sram_blwl_out[1487], sram_blwl_out[1487], sram_blwl_outb[1487], sram_blwl_1487_configbus0[1487:1487], sram_blwl_1487_configbus1[1487:1487] , sram_blwl_1487_configbus0_b[1487:1487] );
wire [1488:1488] sram_blwl_1488_configbus0;
wire [1488:1488] sram_blwl_1488_configbus1;
wire [1488:1488] sram_blwl_1488_configbus0_b;
assign sram_blwl_1488_configbus0[1488:1488] = sram_blwl_bl[1488:1488] ;
assign sram_blwl_1488_configbus1[1488:1488] = sram_blwl_wl[1488:1488] ;
assign sram_blwl_1488_configbus0_b[1488:1488] = sram_blwl_blb[1488:1488] ;
sram6T_blwl sram_blwl_1488_ (sram_blwl_out[1488], sram_blwl_out[1488], sram_blwl_outb[1488], sram_blwl_1488_configbus0[1488:1488], sram_blwl_1488_configbus1[1488:1488] , sram_blwl_1488_configbus0_b[1488:1488] );
wire [1489:1489] sram_blwl_1489_configbus0;
wire [1489:1489] sram_blwl_1489_configbus1;
wire [1489:1489] sram_blwl_1489_configbus0_b;
assign sram_blwl_1489_configbus0[1489:1489] = sram_blwl_bl[1489:1489] ;
assign sram_blwl_1489_configbus1[1489:1489] = sram_blwl_wl[1489:1489] ;
assign sram_blwl_1489_configbus0_b[1489:1489] = sram_blwl_blb[1489:1489] ;
sram6T_blwl sram_blwl_1489_ (sram_blwl_out[1489], sram_blwl_out[1489], sram_blwl_outb[1489], sram_blwl_1489_configbus0[1489:1489], sram_blwl_1489_configbus1[1489:1489] , sram_blwl_1489_configbus0_b[1489:1489] );
wire [1490:1490] sram_blwl_1490_configbus0;
wire [1490:1490] sram_blwl_1490_configbus1;
wire [1490:1490] sram_blwl_1490_configbus0_b;
assign sram_blwl_1490_configbus0[1490:1490] = sram_blwl_bl[1490:1490] ;
assign sram_blwl_1490_configbus1[1490:1490] = sram_blwl_wl[1490:1490] ;
assign sram_blwl_1490_configbus0_b[1490:1490] = sram_blwl_blb[1490:1490] ;
sram6T_blwl sram_blwl_1490_ (sram_blwl_out[1490], sram_blwl_out[1490], sram_blwl_outb[1490], sram_blwl_1490_configbus0[1490:1490], sram_blwl_1490_configbus1[1490:1490] , sram_blwl_1490_configbus0_b[1490:1490] );
wire [1491:1491] sram_blwl_1491_configbus0;
wire [1491:1491] sram_blwl_1491_configbus1;
wire [1491:1491] sram_blwl_1491_configbus0_b;
assign sram_blwl_1491_configbus0[1491:1491] = sram_blwl_bl[1491:1491] ;
assign sram_blwl_1491_configbus1[1491:1491] = sram_blwl_wl[1491:1491] ;
assign sram_blwl_1491_configbus0_b[1491:1491] = sram_blwl_blb[1491:1491] ;
sram6T_blwl sram_blwl_1491_ (sram_blwl_out[1491], sram_blwl_out[1491], sram_blwl_outb[1491], sram_blwl_1491_configbus0[1491:1491], sram_blwl_1491_configbus1[1491:1491] , sram_blwl_1491_configbus0_b[1491:1491] );
wire [1492:1492] sram_blwl_1492_configbus0;
wire [1492:1492] sram_blwl_1492_configbus1;
wire [1492:1492] sram_blwl_1492_configbus0_b;
assign sram_blwl_1492_configbus0[1492:1492] = sram_blwl_bl[1492:1492] ;
assign sram_blwl_1492_configbus1[1492:1492] = sram_blwl_wl[1492:1492] ;
assign sram_blwl_1492_configbus0_b[1492:1492] = sram_blwl_blb[1492:1492] ;
sram6T_blwl sram_blwl_1492_ (sram_blwl_out[1492], sram_blwl_out[1492], sram_blwl_outb[1492], sram_blwl_1492_configbus0[1492:1492], sram_blwl_1492_configbus1[1492:1492] , sram_blwl_1492_configbus0_b[1492:1492] );
wire [1493:1493] sram_blwl_1493_configbus0;
wire [1493:1493] sram_blwl_1493_configbus1;
wire [1493:1493] sram_blwl_1493_configbus0_b;
assign sram_blwl_1493_configbus0[1493:1493] = sram_blwl_bl[1493:1493] ;
assign sram_blwl_1493_configbus1[1493:1493] = sram_blwl_wl[1493:1493] ;
assign sram_blwl_1493_configbus0_b[1493:1493] = sram_blwl_blb[1493:1493] ;
sram6T_blwl sram_blwl_1493_ (sram_blwl_out[1493], sram_blwl_out[1493], sram_blwl_outb[1493], sram_blwl_1493_configbus0[1493:1493], sram_blwl_1493_configbus1[1493:1493] , sram_blwl_1493_configbus0_b[1493:1493] );
wire [1494:1494] sram_blwl_1494_configbus0;
wire [1494:1494] sram_blwl_1494_configbus1;
wire [1494:1494] sram_blwl_1494_configbus0_b;
assign sram_blwl_1494_configbus0[1494:1494] = sram_blwl_bl[1494:1494] ;
assign sram_blwl_1494_configbus1[1494:1494] = sram_blwl_wl[1494:1494] ;
assign sram_blwl_1494_configbus0_b[1494:1494] = sram_blwl_blb[1494:1494] ;
sram6T_blwl sram_blwl_1494_ (sram_blwl_out[1494], sram_blwl_out[1494], sram_blwl_outb[1494], sram_blwl_1494_configbus0[1494:1494], sram_blwl_1494_configbus1[1494:1494] , sram_blwl_1494_configbus0_b[1494:1494] );
wire [1495:1495] sram_blwl_1495_configbus0;
wire [1495:1495] sram_blwl_1495_configbus1;
wire [1495:1495] sram_blwl_1495_configbus0_b;
assign sram_blwl_1495_configbus0[1495:1495] = sram_blwl_bl[1495:1495] ;
assign sram_blwl_1495_configbus1[1495:1495] = sram_blwl_wl[1495:1495] ;
assign sram_blwl_1495_configbus0_b[1495:1495] = sram_blwl_blb[1495:1495] ;
sram6T_blwl sram_blwl_1495_ (sram_blwl_out[1495], sram_blwl_out[1495], sram_blwl_outb[1495], sram_blwl_1495_configbus0[1495:1495], sram_blwl_1495_configbus1[1495:1495] , sram_blwl_1495_configbus0_b[1495:1495] );
wire [1496:1496] sram_blwl_1496_configbus0;
wire [1496:1496] sram_blwl_1496_configbus1;
wire [1496:1496] sram_blwl_1496_configbus0_b;
assign sram_blwl_1496_configbus0[1496:1496] = sram_blwl_bl[1496:1496] ;
assign sram_blwl_1496_configbus1[1496:1496] = sram_blwl_wl[1496:1496] ;
assign sram_blwl_1496_configbus0_b[1496:1496] = sram_blwl_blb[1496:1496] ;
sram6T_blwl sram_blwl_1496_ (sram_blwl_out[1496], sram_blwl_out[1496], sram_blwl_outb[1496], sram_blwl_1496_configbus0[1496:1496], sram_blwl_1496_configbus1[1496:1496] , sram_blwl_1496_configbus0_b[1496:1496] );
wire [1497:1497] sram_blwl_1497_configbus0;
wire [1497:1497] sram_blwl_1497_configbus1;
wire [1497:1497] sram_blwl_1497_configbus0_b;
assign sram_blwl_1497_configbus0[1497:1497] = sram_blwl_bl[1497:1497] ;
assign sram_blwl_1497_configbus1[1497:1497] = sram_blwl_wl[1497:1497] ;
assign sram_blwl_1497_configbus0_b[1497:1497] = sram_blwl_blb[1497:1497] ;
sram6T_blwl sram_blwl_1497_ (sram_blwl_out[1497], sram_blwl_out[1497], sram_blwl_outb[1497], sram_blwl_1497_configbus0[1497:1497], sram_blwl_1497_configbus1[1497:1497] , sram_blwl_1497_configbus0_b[1497:1497] );
wire [1498:1498] sram_blwl_1498_configbus0;
wire [1498:1498] sram_blwl_1498_configbus1;
wire [1498:1498] sram_blwl_1498_configbus0_b;
assign sram_blwl_1498_configbus0[1498:1498] = sram_blwl_bl[1498:1498] ;
assign sram_blwl_1498_configbus1[1498:1498] = sram_blwl_wl[1498:1498] ;
assign sram_blwl_1498_configbus0_b[1498:1498] = sram_blwl_blb[1498:1498] ;
sram6T_blwl sram_blwl_1498_ (sram_blwl_out[1498], sram_blwl_out[1498], sram_blwl_outb[1498], sram_blwl_1498_configbus0[1498:1498], sram_blwl_1498_configbus1[1498:1498] , sram_blwl_1498_configbus0_b[1498:1498] );
wire [1499:1499] sram_blwl_1499_configbus0;
wire [1499:1499] sram_blwl_1499_configbus1;
wire [1499:1499] sram_blwl_1499_configbus0_b;
assign sram_blwl_1499_configbus0[1499:1499] = sram_blwl_bl[1499:1499] ;
assign sram_blwl_1499_configbus1[1499:1499] = sram_blwl_wl[1499:1499] ;
assign sram_blwl_1499_configbus0_b[1499:1499] = sram_blwl_blb[1499:1499] ;
sram6T_blwl sram_blwl_1499_ (sram_blwl_out[1499], sram_blwl_out[1499], sram_blwl_outb[1499], sram_blwl_1499_configbus0[1499:1499], sram_blwl_1499_configbus1[1499:1499] , sram_blwl_1499_configbus0_b[1499:1499] );
wire [1500:1500] sram_blwl_1500_configbus0;
wire [1500:1500] sram_blwl_1500_configbus1;
wire [1500:1500] sram_blwl_1500_configbus0_b;
assign sram_blwl_1500_configbus0[1500:1500] = sram_blwl_bl[1500:1500] ;
assign sram_blwl_1500_configbus1[1500:1500] = sram_blwl_wl[1500:1500] ;
assign sram_blwl_1500_configbus0_b[1500:1500] = sram_blwl_blb[1500:1500] ;
sram6T_blwl sram_blwl_1500_ (sram_blwl_out[1500], sram_blwl_out[1500], sram_blwl_outb[1500], sram_blwl_1500_configbus0[1500:1500], sram_blwl_1500_configbus1[1500:1500] , sram_blwl_1500_configbus0_b[1500:1500] );
wire [1501:1501] sram_blwl_1501_configbus0;
wire [1501:1501] sram_blwl_1501_configbus1;
wire [1501:1501] sram_blwl_1501_configbus0_b;
assign sram_blwl_1501_configbus0[1501:1501] = sram_blwl_bl[1501:1501] ;
assign sram_blwl_1501_configbus1[1501:1501] = sram_blwl_wl[1501:1501] ;
assign sram_blwl_1501_configbus0_b[1501:1501] = sram_blwl_blb[1501:1501] ;
sram6T_blwl sram_blwl_1501_ (sram_blwl_out[1501], sram_blwl_out[1501], sram_blwl_outb[1501], sram_blwl_1501_configbus0[1501:1501], sram_blwl_1501_configbus1[1501:1501] , sram_blwl_1501_configbus0_b[1501:1501] );
wire [1502:1502] sram_blwl_1502_configbus0;
wire [1502:1502] sram_blwl_1502_configbus1;
wire [1502:1502] sram_blwl_1502_configbus0_b;
assign sram_blwl_1502_configbus0[1502:1502] = sram_blwl_bl[1502:1502] ;
assign sram_blwl_1502_configbus1[1502:1502] = sram_blwl_wl[1502:1502] ;
assign sram_blwl_1502_configbus0_b[1502:1502] = sram_blwl_blb[1502:1502] ;
sram6T_blwl sram_blwl_1502_ (sram_blwl_out[1502], sram_blwl_out[1502], sram_blwl_outb[1502], sram_blwl_1502_configbus0[1502:1502], sram_blwl_1502_configbus1[1502:1502] , sram_blwl_1502_configbus0_b[1502:1502] );
wire [1503:1503] sram_blwl_1503_configbus0;
wire [1503:1503] sram_blwl_1503_configbus1;
wire [1503:1503] sram_blwl_1503_configbus0_b;
assign sram_blwl_1503_configbus0[1503:1503] = sram_blwl_bl[1503:1503] ;
assign sram_blwl_1503_configbus1[1503:1503] = sram_blwl_wl[1503:1503] ;
assign sram_blwl_1503_configbus0_b[1503:1503] = sram_blwl_blb[1503:1503] ;
sram6T_blwl sram_blwl_1503_ (sram_blwl_out[1503], sram_blwl_out[1503], sram_blwl_outb[1503], sram_blwl_1503_configbus0[1503:1503], sram_blwl_1503_configbus1[1503:1503] , sram_blwl_1503_configbus0_b[1503:1503] );
wire [1504:1504] sram_blwl_1504_configbus0;
wire [1504:1504] sram_blwl_1504_configbus1;
wire [1504:1504] sram_blwl_1504_configbus0_b;
assign sram_blwl_1504_configbus0[1504:1504] = sram_blwl_bl[1504:1504] ;
assign sram_blwl_1504_configbus1[1504:1504] = sram_blwl_wl[1504:1504] ;
assign sram_blwl_1504_configbus0_b[1504:1504] = sram_blwl_blb[1504:1504] ;
sram6T_blwl sram_blwl_1504_ (sram_blwl_out[1504], sram_blwl_out[1504], sram_blwl_outb[1504], sram_blwl_1504_configbus0[1504:1504], sram_blwl_1504_configbus1[1504:1504] , sram_blwl_1504_configbus0_b[1504:1504] );
wire [1505:1505] sram_blwl_1505_configbus0;
wire [1505:1505] sram_blwl_1505_configbus1;
wire [1505:1505] sram_blwl_1505_configbus0_b;
assign sram_blwl_1505_configbus0[1505:1505] = sram_blwl_bl[1505:1505] ;
assign sram_blwl_1505_configbus1[1505:1505] = sram_blwl_wl[1505:1505] ;
assign sram_blwl_1505_configbus0_b[1505:1505] = sram_blwl_blb[1505:1505] ;
sram6T_blwl sram_blwl_1505_ (sram_blwl_out[1505], sram_blwl_out[1505], sram_blwl_outb[1505], sram_blwl_1505_configbus0[1505:1505], sram_blwl_1505_configbus1[1505:1505] , sram_blwl_1505_configbus0_b[1505:1505] );
wire [1506:1506] sram_blwl_1506_configbus0;
wire [1506:1506] sram_blwl_1506_configbus1;
wire [1506:1506] sram_blwl_1506_configbus0_b;
assign sram_blwl_1506_configbus0[1506:1506] = sram_blwl_bl[1506:1506] ;
assign sram_blwl_1506_configbus1[1506:1506] = sram_blwl_wl[1506:1506] ;
assign sram_blwl_1506_configbus0_b[1506:1506] = sram_blwl_blb[1506:1506] ;
sram6T_blwl sram_blwl_1506_ (sram_blwl_out[1506], sram_blwl_out[1506], sram_blwl_outb[1506], sram_blwl_1506_configbus0[1506:1506], sram_blwl_1506_configbus1[1506:1506] , sram_blwl_1506_configbus0_b[1506:1506] );
wire [1507:1507] sram_blwl_1507_configbus0;
wire [1507:1507] sram_blwl_1507_configbus1;
wire [1507:1507] sram_blwl_1507_configbus0_b;
assign sram_blwl_1507_configbus0[1507:1507] = sram_blwl_bl[1507:1507] ;
assign sram_blwl_1507_configbus1[1507:1507] = sram_blwl_wl[1507:1507] ;
assign sram_blwl_1507_configbus0_b[1507:1507] = sram_blwl_blb[1507:1507] ;
sram6T_blwl sram_blwl_1507_ (sram_blwl_out[1507], sram_blwl_out[1507], sram_blwl_outb[1507], sram_blwl_1507_configbus0[1507:1507], sram_blwl_1507_configbus1[1507:1507] , sram_blwl_1507_configbus0_b[1507:1507] );
wire [1508:1508] sram_blwl_1508_configbus0;
wire [1508:1508] sram_blwl_1508_configbus1;
wire [1508:1508] sram_blwl_1508_configbus0_b;
assign sram_blwl_1508_configbus0[1508:1508] = sram_blwl_bl[1508:1508] ;
assign sram_blwl_1508_configbus1[1508:1508] = sram_blwl_wl[1508:1508] ;
assign sram_blwl_1508_configbus0_b[1508:1508] = sram_blwl_blb[1508:1508] ;
sram6T_blwl sram_blwl_1508_ (sram_blwl_out[1508], sram_blwl_out[1508], sram_blwl_outb[1508], sram_blwl_1508_configbus0[1508:1508], sram_blwl_1508_configbus1[1508:1508] , sram_blwl_1508_configbus0_b[1508:1508] );
wire [1509:1509] sram_blwl_1509_configbus0;
wire [1509:1509] sram_blwl_1509_configbus1;
wire [1509:1509] sram_blwl_1509_configbus0_b;
assign sram_blwl_1509_configbus0[1509:1509] = sram_blwl_bl[1509:1509] ;
assign sram_blwl_1509_configbus1[1509:1509] = sram_blwl_wl[1509:1509] ;
assign sram_blwl_1509_configbus0_b[1509:1509] = sram_blwl_blb[1509:1509] ;
sram6T_blwl sram_blwl_1509_ (sram_blwl_out[1509], sram_blwl_out[1509], sram_blwl_outb[1509], sram_blwl_1509_configbus0[1509:1509], sram_blwl_1509_configbus1[1509:1509] , sram_blwl_1509_configbus0_b[1509:1509] );
wire [1510:1510] sram_blwl_1510_configbus0;
wire [1510:1510] sram_blwl_1510_configbus1;
wire [1510:1510] sram_blwl_1510_configbus0_b;
assign sram_blwl_1510_configbus0[1510:1510] = sram_blwl_bl[1510:1510] ;
assign sram_blwl_1510_configbus1[1510:1510] = sram_blwl_wl[1510:1510] ;
assign sram_blwl_1510_configbus0_b[1510:1510] = sram_blwl_blb[1510:1510] ;
sram6T_blwl sram_blwl_1510_ (sram_blwl_out[1510], sram_blwl_out[1510], sram_blwl_outb[1510], sram_blwl_1510_configbus0[1510:1510], sram_blwl_1510_configbus1[1510:1510] , sram_blwl_1510_configbus0_b[1510:1510] );
wire [1511:1511] sram_blwl_1511_configbus0;
wire [1511:1511] sram_blwl_1511_configbus1;
wire [1511:1511] sram_blwl_1511_configbus0_b;
assign sram_blwl_1511_configbus0[1511:1511] = sram_blwl_bl[1511:1511] ;
assign sram_blwl_1511_configbus1[1511:1511] = sram_blwl_wl[1511:1511] ;
assign sram_blwl_1511_configbus0_b[1511:1511] = sram_blwl_blb[1511:1511] ;
sram6T_blwl sram_blwl_1511_ (sram_blwl_out[1511], sram_blwl_out[1511], sram_blwl_outb[1511], sram_blwl_1511_configbus0[1511:1511], sram_blwl_1511_configbus1[1511:1511] , sram_blwl_1511_configbus0_b[1511:1511] );
wire [1512:1512] sram_blwl_1512_configbus0;
wire [1512:1512] sram_blwl_1512_configbus1;
wire [1512:1512] sram_blwl_1512_configbus0_b;
assign sram_blwl_1512_configbus0[1512:1512] = sram_blwl_bl[1512:1512] ;
assign sram_blwl_1512_configbus1[1512:1512] = sram_blwl_wl[1512:1512] ;
assign sram_blwl_1512_configbus0_b[1512:1512] = sram_blwl_blb[1512:1512] ;
sram6T_blwl sram_blwl_1512_ (sram_blwl_out[1512], sram_blwl_out[1512], sram_blwl_outb[1512], sram_blwl_1512_configbus0[1512:1512], sram_blwl_1512_configbus1[1512:1512] , sram_blwl_1512_configbus0_b[1512:1512] );
wire [1513:1513] sram_blwl_1513_configbus0;
wire [1513:1513] sram_blwl_1513_configbus1;
wire [1513:1513] sram_blwl_1513_configbus0_b;
assign sram_blwl_1513_configbus0[1513:1513] = sram_blwl_bl[1513:1513] ;
assign sram_blwl_1513_configbus1[1513:1513] = sram_blwl_wl[1513:1513] ;
assign sram_blwl_1513_configbus0_b[1513:1513] = sram_blwl_blb[1513:1513] ;
sram6T_blwl sram_blwl_1513_ (sram_blwl_out[1513], sram_blwl_out[1513], sram_blwl_outb[1513], sram_blwl_1513_configbus0[1513:1513], sram_blwl_1513_configbus1[1513:1513] , sram_blwl_1513_configbus0_b[1513:1513] );
wire [1514:1514] sram_blwl_1514_configbus0;
wire [1514:1514] sram_blwl_1514_configbus1;
wire [1514:1514] sram_blwl_1514_configbus0_b;
assign sram_blwl_1514_configbus0[1514:1514] = sram_blwl_bl[1514:1514] ;
assign sram_blwl_1514_configbus1[1514:1514] = sram_blwl_wl[1514:1514] ;
assign sram_blwl_1514_configbus0_b[1514:1514] = sram_blwl_blb[1514:1514] ;
sram6T_blwl sram_blwl_1514_ (sram_blwl_out[1514], sram_blwl_out[1514], sram_blwl_outb[1514], sram_blwl_1514_configbus0[1514:1514], sram_blwl_1514_configbus1[1514:1514] , sram_blwl_1514_configbus0_b[1514:1514] );
wire [1515:1515] sram_blwl_1515_configbus0;
wire [1515:1515] sram_blwl_1515_configbus1;
wire [1515:1515] sram_blwl_1515_configbus0_b;
assign sram_blwl_1515_configbus0[1515:1515] = sram_blwl_bl[1515:1515] ;
assign sram_blwl_1515_configbus1[1515:1515] = sram_blwl_wl[1515:1515] ;
assign sram_blwl_1515_configbus0_b[1515:1515] = sram_blwl_blb[1515:1515] ;
sram6T_blwl sram_blwl_1515_ (sram_blwl_out[1515], sram_blwl_out[1515], sram_blwl_outb[1515], sram_blwl_1515_configbus0[1515:1515], sram_blwl_1515_configbus1[1515:1515] , sram_blwl_1515_configbus0_b[1515:1515] );
wire [1516:1516] sram_blwl_1516_configbus0;
wire [1516:1516] sram_blwl_1516_configbus1;
wire [1516:1516] sram_blwl_1516_configbus0_b;
assign sram_blwl_1516_configbus0[1516:1516] = sram_blwl_bl[1516:1516] ;
assign sram_blwl_1516_configbus1[1516:1516] = sram_blwl_wl[1516:1516] ;
assign sram_blwl_1516_configbus0_b[1516:1516] = sram_blwl_blb[1516:1516] ;
sram6T_blwl sram_blwl_1516_ (sram_blwl_out[1516], sram_blwl_out[1516], sram_blwl_outb[1516], sram_blwl_1516_configbus0[1516:1516], sram_blwl_1516_configbus1[1516:1516] , sram_blwl_1516_configbus0_b[1516:1516] );
wire [1517:1517] sram_blwl_1517_configbus0;
wire [1517:1517] sram_blwl_1517_configbus1;
wire [1517:1517] sram_blwl_1517_configbus0_b;
assign sram_blwl_1517_configbus0[1517:1517] = sram_blwl_bl[1517:1517] ;
assign sram_blwl_1517_configbus1[1517:1517] = sram_blwl_wl[1517:1517] ;
assign sram_blwl_1517_configbus0_b[1517:1517] = sram_blwl_blb[1517:1517] ;
sram6T_blwl sram_blwl_1517_ (sram_blwl_out[1517], sram_blwl_out[1517], sram_blwl_outb[1517], sram_blwl_1517_configbus0[1517:1517], sram_blwl_1517_configbus1[1517:1517] , sram_blwl_1517_configbus0_b[1517:1517] );
wire [1518:1518] sram_blwl_1518_configbus0;
wire [1518:1518] sram_blwl_1518_configbus1;
wire [1518:1518] sram_blwl_1518_configbus0_b;
assign sram_blwl_1518_configbus0[1518:1518] = sram_blwl_bl[1518:1518] ;
assign sram_blwl_1518_configbus1[1518:1518] = sram_blwl_wl[1518:1518] ;
assign sram_blwl_1518_configbus0_b[1518:1518] = sram_blwl_blb[1518:1518] ;
sram6T_blwl sram_blwl_1518_ (sram_blwl_out[1518], sram_blwl_out[1518], sram_blwl_outb[1518], sram_blwl_1518_configbus0[1518:1518], sram_blwl_1518_configbus1[1518:1518] , sram_blwl_1518_configbus0_b[1518:1518] );
wire [1519:1519] sram_blwl_1519_configbus0;
wire [1519:1519] sram_blwl_1519_configbus1;
wire [1519:1519] sram_blwl_1519_configbus0_b;
assign sram_blwl_1519_configbus0[1519:1519] = sram_blwl_bl[1519:1519] ;
assign sram_blwl_1519_configbus1[1519:1519] = sram_blwl_wl[1519:1519] ;
assign sram_blwl_1519_configbus0_b[1519:1519] = sram_blwl_blb[1519:1519] ;
sram6T_blwl sram_blwl_1519_ (sram_blwl_out[1519], sram_blwl_out[1519], sram_blwl_outb[1519], sram_blwl_1519_configbus0[1519:1519], sram_blwl_1519_configbus1[1519:1519] , sram_blwl_1519_configbus0_b[1519:1519] );
wire [1520:1520] sram_blwl_1520_configbus0;
wire [1520:1520] sram_blwl_1520_configbus1;
wire [1520:1520] sram_blwl_1520_configbus0_b;
assign sram_blwl_1520_configbus0[1520:1520] = sram_blwl_bl[1520:1520] ;
assign sram_blwl_1520_configbus1[1520:1520] = sram_blwl_wl[1520:1520] ;
assign sram_blwl_1520_configbus0_b[1520:1520] = sram_blwl_blb[1520:1520] ;
sram6T_blwl sram_blwl_1520_ (sram_blwl_out[1520], sram_blwl_out[1520], sram_blwl_outb[1520], sram_blwl_1520_configbus0[1520:1520], sram_blwl_1520_configbus1[1520:1520] , sram_blwl_1520_configbus0_b[1520:1520] );
wire [1521:1521] sram_blwl_1521_configbus0;
wire [1521:1521] sram_blwl_1521_configbus1;
wire [1521:1521] sram_blwl_1521_configbus0_b;
assign sram_blwl_1521_configbus0[1521:1521] = sram_blwl_bl[1521:1521] ;
assign sram_blwl_1521_configbus1[1521:1521] = sram_blwl_wl[1521:1521] ;
assign sram_blwl_1521_configbus0_b[1521:1521] = sram_blwl_blb[1521:1521] ;
sram6T_blwl sram_blwl_1521_ (sram_blwl_out[1521], sram_blwl_out[1521], sram_blwl_outb[1521], sram_blwl_1521_configbus0[1521:1521], sram_blwl_1521_configbus1[1521:1521] , sram_blwl_1521_configbus0_b[1521:1521] );
wire [1522:1522] sram_blwl_1522_configbus0;
wire [1522:1522] sram_blwl_1522_configbus1;
wire [1522:1522] sram_blwl_1522_configbus0_b;
assign sram_blwl_1522_configbus0[1522:1522] = sram_blwl_bl[1522:1522] ;
assign sram_blwl_1522_configbus1[1522:1522] = sram_blwl_wl[1522:1522] ;
assign sram_blwl_1522_configbus0_b[1522:1522] = sram_blwl_blb[1522:1522] ;
sram6T_blwl sram_blwl_1522_ (sram_blwl_out[1522], sram_blwl_out[1522], sram_blwl_outb[1522], sram_blwl_1522_configbus0[1522:1522], sram_blwl_1522_configbus1[1522:1522] , sram_blwl_1522_configbus0_b[1522:1522] );
wire [1523:1523] sram_blwl_1523_configbus0;
wire [1523:1523] sram_blwl_1523_configbus1;
wire [1523:1523] sram_blwl_1523_configbus0_b;
assign sram_blwl_1523_configbus0[1523:1523] = sram_blwl_bl[1523:1523] ;
assign sram_blwl_1523_configbus1[1523:1523] = sram_blwl_wl[1523:1523] ;
assign sram_blwl_1523_configbus0_b[1523:1523] = sram_blwl_blb[1523:1523] ;
sram6T_blwl sram_blwl_1523_ (sram_blwl_out[1523], sram_blwl_out[1523], sram_blwl_outb[1523], sram_blwl_1523_configbus0[1523:1523], sram_blwl_1523_configbus1[1523:1523] , sram_blwl_1523_configbus0_b[1523:1523] );
wire [1524:1524] sram_blwl_1524_configbus0;
wire [1524:1524] sram_blwl_1524_configbus1;
wire [1524:1524] sram_blwl_1524_configbus0_b;
assign sram_blwl_1524_configbus0[1524:1524] = sram_blwl_bl[1524:1524] ;
assign sram_blwl_1524_configbus1[1524:1524] = sram_blwl_wl[1524:1524] ;
assign sram_blwl_1524_configbus0_b[1524:1524] = sram_blwl_blb[1524:1524] ;
sram6T_blwl sram_blwl_1524_ (sram_blwl_out[1524], sram_blwl_out[1524], sram_blwl_outb[1524], sram_blwl_1524_configbus0[1524:1524], sram_blwl_1524_configbus1[1524:1524] , sram_blwl_1524_configbus0_b[1524:1524] );
wire [1525:1525] sram_blwl_1525_configbus0;
wire [1525:1525] sram_blwl_1525_configbus1;
wire [1525:1525] sram_blwl_1525_configbus0_b;
assign sram_blwl_1525_configbus0[1525:1525] = sram_blwl_bl[1525:1525] ;
assign sram_blwl_1525_configbus1[1525:1525] = sram_blwl_wl[1525:1525] ;
assign sram_blwl_1525_configbus0_b[1525:1525] = sram_blwl_blb[1525:1525] ;
sram6T_blwl sram_blwl_1525_ (sram_blwl_out[1525], sram_blwl_out[1525], sram_blwl_outb[1525], sram_blwl_1525_configbus0[1525:1525], sram_blwl_1525_configbus1[1525:1525] , sram_blwl_1525_configbus0_b[1525:1525] );
wire [1526:1526] sram_blwl_1526_configbus0;
wire [1526:1526] sram_blwl_1526_configbus1;
wire [1526:1526] sram_blwl_1526_configbus0_b;
assign sram_blwl_1526_configbus0[1526:1526] = sram_blwl_bl[1526:1526] ;
assign sram_blwl_1526_configbus1[1526:1526] = sram_blwl_wl[1526:1526] ;
assign sram_blwl_1526_configbus0_b[1526:1526] = sram_blwl_blb[1526:1526] ;
sram6T_blwl sram_blwl_1526_ (sram_blwl_out[1526], sram_blwl_out[1526], sram_blwl_outb[1526], sram_blwl_1526_configbus0[1526:1526], sram_blwl_1526_configbus1[1526:1526] , sram_blwl_1526_configbus0_b[1526:1526] );
wire [1527:1527] sram_blwl_1527_configbus0;
wire [1527:1527] sram_blwl_1527_configbus1;
wire [1527:1527] sram_blwl_1527_configbus0_b;
assign sram_blwl_1527_configbus0[1527:1527] = sram_blwl_bl[1527:1527] ;
assign sram_blwl_1527_configbus1[1527:1527] = sram_blwl_wl[1527:1527] ;
assign sram_blwl_1527_configbus0_b[1527:1527] = sram_blwl_blb[1527:1527] ;
sram6T_blwl sram_blwl_1527_ (sram_blwl_out[1527], sram_blwl_out[1527], sram_blwl_outb[1527], sram_blwl_1527_configbus0[1527:1527], sram_blwl_1527_configbus1[1527:1527] , sram_blwl_1527_configbus0_b[1527:1527] );
wire [1528:1528] sram_blwl_1528_configbus0;
wire [1528:1528] sram_blwl_1528_configbus1;
wire [1528:1528] sram_blwl_1528_configbus0_b;
assign sram_blwl_1528_configbus0[1528:1528] = sram_blwl_bl[1528:1528] ;
assign sram_blwl_1528_configbus1[1528:1528] = sram_blwl_wl[1528:1528] ;
assign sram_blwl_1528_configbus0_b[1528:1528] = sram_blwl_blb[1528:1528] ;
sram6T_blwl sram_blwl_1528_ (sram_blwl_out[1528], sram_blwl_out[1528], sram_blwl_outb[1528], sram_blwl_1528_configbus0[1528:1528], sram_blwl_1528_configbus1[1528:1528] , sram_blwl_1528_configbus0_b[1528:1528] );
wire [1529:1529] sram_blwl_1529_configbus0;
wire [1529:1529] sram_blwl_1529_configbus1;
wire [1529:1529] sram_blwl_1529_configbus0_b;
assign sram_blwl_1529_configbus0[1529:1529] = sram_blwl_bl[1529:1529] ;
assign sram_blwl_1529_configbus1[1529:1529] = sram_blwl_wl[1529:1529] ;
assign sram_blwl_1529_configbus0_b[1529:1529] = sram_blwl_blb[1529:1529] ;
sram6T_blwl sram_blwl_1529_ (sram_blwl_out[1529], sram_blwl_out[1529], sram_blwl_outb[1529], sram_blwl_1529_configbus0[1529:1529], sram_blwl_1529_configbus1[1529:1529] , sram_blwl_1529_configbus0_b[1529:1529] );
wire [1530:1530] sram_blwl_1530_configbus0;
wire [1530:1530] sram_blwl_1530_configbus1;
wire [1530:1530] sram_blwl_1530_configbus0_b;
assign sram_blwl_1530_configbus0[1530:1530] = sram_blwl_bl[1530:1530] ;
assign sram_blwl_1530_configbus1[1530:1530] = sram_blwl_wl[1530:1530] ;
assign sram_blwl_1530_configbus0_b[1530:1530] = sram_blwl_blb[1530:1530] ;
sram6T_blwl sram_blwl_1530_ (sram_blwl_out[1530], sram_blwl_out[1530], sram_blwl_outb[1530], sram_blwl_1530_configbus0[1530:1530], sram_blwl_1530_configbus1[1530:1530] , sram_blwl_1530_configbus0_b[1530:1530] );
wire [1531:1531] sram_blwl_1531_configbus0;
wire [1531:1531] sram_blwl_1531_configbus1;
wire [1531:1531] sram_blwl_1531_configbus0_b;
assign sram_blwl_1531_configbus0[1531:1531] = sram_blwl_bl[1531:1531] ;
assign sram_blwl_1531_configbus1[1531:1531] = sram_blwl_wl[1531:1531] ;
assign sram_blwl_1531_configbus0_b[1531:1531] = sram_blwl_blb[1531:1531] ;
sram6T_blwl sram_blwl_1531_ (sram_blwl_out[1531], sram_blwl_out[1531], sram_blwl_outb[1531], sram_blwl_1531_configbus0[1531:1531], sram_blwl_1531_configbus1[1531:1531] , sram_blwl_1531_configbus0_b[1531:1531] );
wire [1532:1532] sram_blwl_1532_configbus0;
wire [1532:1532] sram_blwl_1532_configbus1;
wire [1532:1532] sram_blwl_1532_configbus0_b;
assign sram_blwl_1532_configbus0[1532:1532] = sram_blwl_bl[1532:1532] ;
assign sram_blwl_1532_configbus1[1532:1532] = sram_blwl_wl[1532:1532] ;
assign sram_blwl_1532_configbus0_b[1532:1532] = sram_blwl_blb[1532:1532] ;
sram6T_blwl sram_blwl_1532_ (sram_blwl_out[1532], sram_blwl_out[1532], sram_blwl_outb[1532], sram_blwl_1532_configbus0[1532:1532], sram_blwl_1532_configbus1[1532:1532] , sram_blwl_1532_configbus0_b[1532:1532] );
wire [1533:1533] sram_blwl_1533_configbus0;
wire [1533:1533] sram_blwl_1533_configbus1;
wire [1533:1533] sram_blwl_1533_configbus0_b;
assign sram_blwl_1533_configbus0[1533:1533] = sram_blwl_bl[1533:1533] ;
assign sram_blwl_1533_configbus1[1533:1533] = sram_blwl_wl[1533:1533] ;
assign sram_blwl_1533_configbus0_b[1533:1533] = sram_blwl_blb[1533:1533] ;
sram6T_blwl sram_blwl_1533_ (sram_blwl_out[1533], sram_blwl_out[1533], sram_blwl_outb[1533], sram_blwl_1533_configbus0[1533:1533], sram_blwl_1533_configbus1[1533:1533] , sram_blwl_1533_configbus0_b[1533:1533] );
wire [1534:1534] sram_blwl_1534_configbus0;
wire [1534:1534] sram_blwl_1534_configbus1;
wire [1534:1534] sram_blwl_1534_configbus0_b;
assign sram_blwl_1534_configbus0[1534:1534] = sram_blwl_bl[1534:1534] ;
assign sram_blwl_1534_configbus1[1534:1534] = sram_blwl_wl[1534:1534] ;
assign sram_blwl_1534_configbus0_b[1534:1534] = sram_blwl_blb[1534:1534] ;
sram6T_blwl sram_blwl_1534_ (sram_blwl_out[1534], sram_blwl_out[1534], sram_blwl_outb[1534], sram_blwl_1534_configbus0[1534:1534], sram_blwl_1534_configbus1[1534:1534] , sram_blwl_1534_configbus0_b[1534:1534] );
endmodule
//----- END LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----

//----- Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----
module grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
input [0:0] Set,
input [0:0] Reset,
input [0:0] clk
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
input wire ff_0___D_0_,
output wire ff_0___Q_0_);
static_dff dff_7_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
endmodule
//----- END Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6_ -----
module grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_ble6___in_0_,
input wire mode_ble6___in_1_,
input wire mode_ble6___in_2_,
input wire mode_ble6___in_3_,
input wire mode_ble6___in_4_,
input wire mode_ble6___in_5_,
output wire mode_ble6___out_0_,
input wire mode_ble6___clk_0_,
input [1471:1535] sram_blwl_bl ,
input [1471:1535] sram_blwl_wl ,
input [1471:1535] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ lut6_0_ (
 lut6_0___in_0_,  lut6_0___in_1_,  lut6_0___in_2_,  lut6_0___in_3_,  lut6_0___in_4_,  lut6_0___in_5_,  lut6_0___out_0_,
sram_blwl_bl[1471:1534] ,
sram_blwl_wl[1471:1534] ,
sram_blwl_blb[1471:1534] );
grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ ff_0_ (
//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
wire [0:1] in_bus_mux_1level_tapbuf_size2_407_ ;
assign in_bus_mux_1level_tapbuf_size2_407_[0] = ff_0___Q_0_ ; 
assign in_bus_mux_1level_tapbuf_size2_407_[1] = lut6_0___out_0_ ; 
wire [1535:1535] mux_1level_tapbuf_size2_407_configbus0;
wire [1535:1535] mux_1level_tapbuf_size2_407_configbus1;
wire [1535:1535] mux_1level_tapbuf_size2_407_sram_blwl_out ;
wire [1535:1535] mux_1level_tapbuf_size2_407_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_407_configbus0[1535:1535] = sram_blwl_bl[1535:1535] ;
assign mux_1level_tapbuf_size2_407_configbus1[1535:1535] = sram_blwl_wl[1535:1535] ;
wire [1535:1535] mux_1level_tapbuf_size2_407_configbus0_b;
assign mux_1level_tapbuf_size2_407_configbus0_b[1535:1535] = sram_blwl_blb[1535:1535] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_407_ (in_bus_mux_1level_tapbuf_size2_407_, mode_ble6___out_0_, mux_1level_tapbuf_size2_407_sram_blwl_out[1535:1535] ,
mux_1level_tapbuf_size2_407_sram_blwl_outb[1535:1535] );
//----- SRAM bits for MUX[407], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_1535_ (mux_1level_tapbuf_size2_407_sram_blwl_out[1535:1535] ,mux_1level_tapbuf_size2_407_sram_blwl_out[1535:1535] ,mux_1level_tapbuf_size2_407_sram_blwl_outb[1535:1535] ,mux_1level_tapbuf_size2_407_configbus0[1535:1535], mux_1level_tapbuf_size2_407_configbus1[1535:1535] , mux_1level_tapbuf_size2_407_configbus0_b[1535:1535] );
direct_interc direct_interc_112_ (mode_ble6___in_0_, lut6_0___in_0_ );
direct_interc direct_interc_113_ (mode_ble6___in_1_, lut6_0___in_1_ );
direct_interc direct_interc_114_ (mode_ble6___in_2_, lut6_0___in_2_ );
direct_interc direct_interc_115_ (mode_ble6___in_3_, lut6_0___in_3_ );
direct_interc direct_interc_116_ (mode_ble6___in_4_, lut6_0___in_4_ );
direct_interc direct_interc_117_ (mode_ble6___in_5_, lut6_0___in_5_ );
direct_interc direct_interc_118_ (lut6_0___out_0_, ff_0___D_0_ );
direct_interc direct_interc_119_ (mode_ble6___clk_0_, ff_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6_ -----
module grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_n1_lut6___in_0_,
input wire mode_n1_lut6___in_1_,
input wire mode_n1_lut6___in_2_,
input wire mode_n1_lut6___in_3_,
input wire mode_n1_lut6___in_4_,
input wire mode_n1_lut6___in_5_,
output wire mode_n1_lut6___out_0_,
input wire mode_n1_lut6___clk_0_,
input [1471:1535] sram_blwl_bl ,
input [1471:1535] sram_blwl_wl ,
input [1471:1535] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6__ble6_0__mode_ble6_ ble6_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 ble6_0___in_0_,  ble6_0___in_1_,  ble6_0___in_2_,  ble6_0___in_3_,  ble6_0___in_4_,  ble6_0___in_5_,  ble6_0___out_0_,  ble6_0___clk_0_,
sram_blwl_bl[1471:1535] ,
sram_blwl_wl[1471:1535] ,
sram_blwl_blb[1471:1535] );
direct_interc direct_interc_120_ (ble6_0___out_0_, mode_n1_lut6___out_0_ );
direct_interc direct_interc_121_ (mode_n1_lut6___in_0_, ble6_0___in_0_ );
direct_interc direct_interc_122_ (mode_n1_lut6___in_1_, ble6_0___in_1_ );
direct_interc direct_interc_123_ (mode_n1_lut6___in_2_, ble6_0___in_2_ );
direct_interc direct_interc_124_ (mode_n1_lut6___in_3_, ble6_0___in_3_ );
direct_interc direct_interc_125_ (mode_n1_lut6___in_4_, ble6_0___in_4_ );
direct_interc direct_interc_126_ (mode_n1_lut6___in_5_, ble6_0___in_5_ );
direct_interc direct_interc_127_ (mode_n1_lut6___clk_0_, ble6_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6_ -----

//----- LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----
module grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ (
input wire lut6_0___in_0_,
input wire lut6_0___in_1_,
input wire lut6_0___in_2_,
input wire lut6_0___in_3_,
input wire lut6_0___in_4_,
input wire lut6_0___in_5_,
output wire lut6_0___out_0_,
input [1536:1599] sram_blwl_bl ,
input [1536:1599] sram_blwl_wl ,
input [1536:1599] sram_blwl_blb );
wire [0:5] lut6_0___in;
assign lut6_0___in[0] = lut6_0___in_0_;
assign lut6_0___in[1] = lut6_0___in_1_;
assign lut6_0___in[2] = lut6_0___in_2_;
assign lut6_0___in[3] = lut6_0___in_3_;
assign lut6_0___in[4] = lut6_0___in_4_;
assign lut6_0___in[5] = lut6_0___in_5_;
wire [0:0] lut6_0___out;
assign lut6_0___out_0_ = lut6_0___out[0];
wire [1536:1599] sram_blwl_out ;
wire [1536:1599] sram_blwl_outb ;
lut6 lut6_8_ (
//----- Input and output ports -----
 lut6_0___in[0:5] ,  lut6_0___out[0:0],//----- SRAM ports -----
sram_blwl_out[1536:1599] , sram_blwl_outb[1536:1599] );
//----- Truth Table for LUT[8], size=6. -----
//----- SRAM bits for LUT[8], size=6, num_sram=64. -----
//-----0000000000000000000000000000000000000000000000000000000000000000-----
wire [1536:1536] sram_blwl_1536_configbus0;
wire [1536:1536] sram_blwl_1536_configbus1;
wire [1536:1536] sram_blwl_1536_configbus0_b;
assign sram_blwl_1536_configbus0[1536:1536] = sram_blwl_bl[1536:1536] ;
assign sram_blwl_1536_configbus1[1536:1536] = sram_blwl_wl[1536:1536] ;
assign sram_blwl_1536_configbus0_b[1536:1536] = sram_blwl_blb[1536:1536] ;
sram6T_blwl sram_blwl_1536_ (sram_blwl_out[1536], sram_blwl_out[1536], sram_blwl_outb[1536], sram_blwl_1536_configbus0[1536:1536], sram_blwl_1536_configbus1[1536:1536] , sram_blwl_1536_configbus0_b[1536:1536] );
wire [1537:1537] sram_blwl_1537_configbus0;
wire [1537:1537] sram_blwl_1537_configbus1;
wire [1537:1537] sram_blwl_1537_configbus0_b;
assign sram_blwl_1537_configbus0[1537:1537] = sram_blwl_bl[1537:1537] ;
assign sram_blwl_1537_configbus1[1537:1537] = sram_blwl_wl[1537:1537] ;
assign sram_blwl_1537_configbus0_b[1537:1537] = sram_blwl_blb[1537:1537] ;
sram6T_blwl sram_blwl_1537_ (sram_blwl_out[1537], sram_blwl_out[1537], sram_blwl_outb[1537], sram_blwl_1537_configbus0[1537:1537], sram_blwl_1537_configbus1[1537:1537] , sram_blwl_1537_configbus0_b[1537:1537] );
wire [1538:1538] sram_blwl_1538_configbus0;
wire [1538:1538] sram_blwl_1538_configbus1;
wire [1538:1538] sram_blwl_1538_configbus0_b;
assign sram_blwl_1538_configbus0[1538:1538] = sram_blwl_bl[1538:1538] ;
assign sram_blwl_1538_configbus1[1538:1538] = sram_blwl_wl[1538:1538] ;
assign sram_blwl_1538_configbus0_b[1538:1538] = sram_blwl_blb[1538:1538] ;
sram6T_blwl sram_blwl_1538_ (sram_blwl_out[1538], sram_blwl_out[1538], sram_blwl_outb[1538], sram_blwl_1538_configbus0[1538:1538], sram_blwl_1538_configbus1[1538:1538] , sram_blwl_1538_configbus0_b[1538:1538] );
wire [1539:1539] sram_blwl_1539_configbus0;
wire [1539:1539] sram_blwl_1539_configbus1;
wire [1539:1539] sram_blwl_1539_configbus0_b;
assign sram_blwl_1539_configbus0[1539:1539] = sram_blwl_bl[1539:1539] ;
assign sram_blwl_1539_configbus1[1539:1539] = sram_blwl_wl[1539:1539] ;
assign sram_blwl_1539_configbus0_b[1539:1539] = sram_blwl_blb[1539:1539] ;
sram6T_blwl sram_blwl_1539_ (sram_blwl_out[1539], sram_blwl_out[1539], sram_blwl_outb[1539], sram_blwl_1539_configbus0[1539:1539], sram_blwl_1539_configbus1[1539:1539] , sram_blwl_1539_configbus0_b[1539:1539] );
wire [1540:1540] sram_blwl_1540_configbus0;
wire [1540:1540] sram_blwl_1540_configbus1;
wire [1540:1540] sram_blwl_1540_configbus0_b;
assign sram_blwl_1540_configbus0[1540:1540] = sram_blwl_bl[1540:1540] ;
assign sram_blwl_1540_configbus1[1540:1540] = sram_blwl_wl[1540:1540] ;
assign sram_blwl_1540_configbus0_b[1540:1540] = sram_blwl_blb[1540:1540] ;
sram6T_blwl sram_blwl_1540_ (sram_blwl_out[1540], sram_blwl_out[1540], sram_blwl_outb[1540], sram_blwl_1540_configbus0[1540:1540], sram_blwl_1540_configbus1[1540:1540] , sram_blwl_1540_configbus0_b[1540:1540] );
wire [1541:1541] sram_blwl_1541_configbus0;
wire [1541:1541] sram_blwl_1541_configbus1;
wire [1541:1541] sram_blwl_1541_configbus0_b;
assign sram_blwl_1541_configbus0[1541:1541] = sram_blwl_bl[1541:1541] ;
assign sram_blwl_1541_configbus1[1541:1541] = sram_blwl_wl[1541:1541] ;
assign sram_blwl_1541_configbus0_b[1541:1541] = sram_blwl_blb[1541:1541] ;
sram6T_blwl sram_blwl_1541_ (sram_blwl_out[1541], sram_blwl_out[1541], sram_blwl_outb[1541], sram_blwl_1541_configbus0[1541:1541], sram_blwl_1541_configbus1[1541:1541] , sram_blwl_1541_configbus0_b[1541:1541] );
wire [1542:1542] sram_blwl_1542_configbus0;
wire [1542:1542] sram_blwl_1542_configbus1;
wire [1542:1542] sram_blwl_1542_configbus0_b;
assign sram_blwl_1542_configbus0[1542:1542] = sram_blwl_bl[1542:1542] ;
assign sram_blwl_1542_configbus1[1542:1542] = sram_blwl_wl[1542:1542] ;
assign sram_blwl_1542_configbus0_b[1542:1542] = sram_blwl_blb[1542:1542] ;
sram6T_blwl sram_blwl_1542_ (sram_blwl_out[1542], sram_blwl_out[1542], sram_blwl_outb[1542], sram_blwl_1542_configbus0[1542:1542], sram_blwl_1542_configbus1[1542:1542] , sram_blwl_1542_configbus0_b[1542:1542] );
wire [1543:1543] sram_blwl_1543_configbus0;
wire [1543:1543] sram_blwl_1543_configbus1;
wire [1543:1543] sram_blwl_1543_configbus0_b;
assign sram_blwl_1543_configbus0[1543:1543] = sram_blwl_bl[1543:1543] ;
assign sram_blwl_1543_configbus1[1543:1543] = sram_blwl_wl[1543:1543] ;
assign sram_blwl_1543_configbus0_b[1543:1543] = sram_blwl_blb[1543:1543] ;
sram6T_blwl sram_blwl_1543_ (sram_blwl_out[1543], sram_blwl_out[1543], sram_blwl_outb[1543], sram_blwl_1543_configbus0[1543:1543], sram_blwl_1543_configbus1[1543:1543] , sram_blwl_1543_configbus0_b[1543:1543] );
wire [1544:1544] sram_blwl_1544_configbus0;
wire [1544:1544] sram_blwl_1544_configbus1;
wire [1544:1544] sram_blwl_1544_configbus0_b;
assign sram_blwl_1544_configbus0[1544:1544] = sram_blwl_bl[1544:1544] ;
assign sram_blwl_1544_configbus1[1544:1544] = sram_blwl_wl[1544:1544] ;
assign sram_blwl_1544_configbus0_b[1544:1544] = sram_blwl_blb[1544:1544] ;
sram6T_blwl sram_blwl_1544_ (sram_blwl_out[1544], sram_blwl_out[1544], sram_blwl_outb[1544], sram_blwl_1544_configbus0[1544:1544], sram_blwl_1544_configbus1[1544:1544] , sram_blwl_1544_configbus0_b[1544:1544] );
wire [1545:1545] sram_blwl_1545_configbus0;
wire [1545:1545] sram_blwl_1545_configbus1;
wire [1545:1545] sram_blwl_1545_configbus0_b;
assign sram_blwl_1545_configbus0[1545:1545] = sram_blwl_bl[1545:1545] ;
assign sram_blwl_1545_configbus1[1545:1545] = sram_blwl_wl[1545:1545] ;
assign sram_blwl_1545_configbus0_b[1545:1545] = sram_blwl_blb[1545:1545] ;
sram6T_blwl sram_blwl_1545_ (sram_blwl_out[1545], sram_blwl_out[1545], sram_blwl_outb[1545], sram_blwl_1545_configbus0[1545:1545], sram_blwl_1545_configbus1[1545:1545] , sram_blwl_1545_configbus0_b[1545:1545] );
wire [1546:1546] sram_blwl_1546_configbus0;
wire [1546:1546] sram_blwl_1546_configbus1;
wire [1546:1546] sram_blwl_1546_configbus0_b;
assign sram_blwl_1546_configbus0[1546:1546] = sram_blwl_bl[1546:1546] ;
assign sram_blwl_1546_configbus1[1546:1546] = sram_blwl_wl[1546:1546] ;
assign sram_blwl_1546_configbus0_b[1546:1546] = sram_blwl_blb[1546:1546] ;
sram6T_blwl sram_blwl_1546_ (sram_blwl_out[1546], sram_blwl_out[1546], sram_blwl_outb[1546], sram_blwl_1546_configbus0[1546:1546], sram_blwl_1546_configbus1[1546:1546] , sram_blwl_1546_configbus0_b[1546:1546] );
wire [1547:1547] sram_blwl_1547_configbus0;
wire [1547:1547] sram_blwl_1547_configbus1;
wire [1547:1547] sram_blwl_1547_configbus0_b;
assign sram_blwl_1547_configbus0[1547:1547] = sram_blwl_bl[1547:1547] ;
assign sram_blwl_1547_configbus1[1547:1547] = sram_blwl_wl[1547:1547] ;
assign sram_blwl_1547_configbus0_b[1547:1547] = sram_blwl_blb[1547:1547] ;
sram6T_blwl sram_blwl_1547_ (sram_blwl_out[1547], sram_blwl_out[1547], sram_blwl_outb[1547], sram_blwl_1547_configbus0[1547:1547], sram_blwl_1547_configbus1[1547:1547] , sram_blwl_1547_configbus0_b[1547:1547] );
wire [1548:1548] sram_blwl_1548_configbus0;
wire [1548:1548] sram_blwl_1548_configbus1;
wire [1548:1548] sram_blwl_1548_configbus0_b;
assign sram_blwl_1548_configbus0[1548:1548] = sram_blwl_bl[1548:1548] ;
assign sram_blwl_1548_configbus1[1548:1548] = sram_blwl_wl[1548:1548] ;
assign sram_blwl_1548_configbus0_b[1548:1548] = sram_blwl_blb[1548:1548] ;
sram6T_blwl sram_blwl_1548_ (sram_blwl_out[1548], sram_blwl_out[1548], sram_blwl_outb[1548], sram_blwl_1548_configbus0[1548:1548], sram_blwl_1548_configbus1[1548:1548] , sram_blwl_1548_configbus0_b[1548:1548] );
wire [1549:1549] sram_blwl_1549_configbus0;
wire [1549:1549] sram_blwl_1549_configbus1;
wire [1549:1549] sram_blwl_1549_configbus0_b;
assign sram_blwl_1549_configbus0[1549:1549] = sram_blwl_bl[1549:1549] ;
assign sram_blwl_1549_configbus1[1549:1549] = sram_blwl_wl[1549:1549] ;
assign sram_blwl_1549_configbus0_b[1549:1549] = sram_blwl_blb[1549:1549] ;
sram6T_blwl sram_blwl_1549_ (sram_blwl_out[1549], sram_blwl_out[1549], sram_blwl_outb[1549], sram_blwl_1549_configbus0[1549:1549], sram_blwl_1549_configbus1[1549:1549] , sram_blwl_1549_configbus0_b[1549:1549] );
wire [1550:1550] sram_blwl_1550_configbus0;
wire [1550:1550] sram_blwl_1550_configbus1;
wire [1550:1550] sram_blwl_1550_configbus0_b;
assign sram_blwl_1550_configbus0[1550:1550] = sram_blwl_bl[1550:1550] ;
assign sram_blwl_1550_configbus1[1550:1550] = sram_blwl_wl[1550:1550] ;
assign sram_blwl_1550_configbus0_b[1550:1550] = sram_blwl_blb[1550:1550] ;
sram6T_blwl sram_blwl_1550_ (sram_blwl_out[1550], sram_blwl_out[1550], sram_blwl_outb[1550], sram_blwl_1550_configbus0[1550:1550], sram_blwl_1550_configbus1[1550:1550] , sram_blwl_1550_configbus0_b[1550:1550] );
wire [1551:1551] sram_blwl_1551_configbus0;
wire [1551:1551] sram_blwl_1551_configbus1;
wire [1551:1551] sram_blwl_1551_configbus0_b;
assign sram_blwl_1551_configbus0[1551:1551] = sram_blwl_bl[1551:1551] ;
assign sram_blwl_1551_configbus1[1551:1551] = sram_blwl_wl[1551:1551] ;
assign sram_blwl_1551_configbus0_b[1551:1551] = sram_blwl_blb[1551:1551] ;
sram6T_blwl sram_blwl_1551_ (sram_blwl_out[1551], sram_blwl_out[1551], sram_blwl_outb[1551], sram_blwl_1551_configbus0[1551:1551], sram_blwl_1551_configbus1[1551:1551] , sram_blwl_1551_configbus0_b[1551:1551] );
wire [1552:1552] sram_blwl_1552_configbus0;
wire [1552:1552] sram_blwl_1552_configbus1;
wire [1552:1552] sram_blwl_1552_configbus0_b;
assign sram_blwl_1552_configbus0[1552:1552] = sram_blwl_bl[1552:1552] ;
assign sram_blwl_1552_configbus1[1552:1552] = sram_blwl_wl[1552:1552] ;
assign sram_blwl_1552_configbus0_b[1552:1552] = sram_blwl_blb[1552:1552] ;
sram6T_blwl sram_blwl_1552_ (sram_blwl_out[1552], sram_blwl_out[1552], sram_blwl_outb[1552], sram_blwl_1552_configbus0[1552:1552], sram_blwl_1552_configbus1[1552:1552] , sram_blwl_1552_configbus0_b[1552:1552] );
wire [1553:1553] sram_blwl_1553_configbus0;
wire [1553:1553] sram_blwl_1553_configbus1;
wire [1553:1553] sram_blwl_1553_configbus0_b;
assign sram_blwl_1553_configbus0[1553:1553] = sram_blwl_bl[1553:1553] ;
assign sram_blwl_1553_configbus1[1553:1553] = sram_blwl_wl[1553:1553] ;
assign sram_blwl_1553_configbus0_b[1553:1553] = sram_blwl_blb[1553:1553] ;
sram6T_blwl sram_blwl_1553_ (sram_blwl_out[1553], sram_blwl_out[1553], sram_blwl_outb[1553], sram_blwl_1553_configbus0[1553:1553], sram_blwl_1553_configbus1[1553:1553] , sram_blwl_1553_configbus0_b[1553:1553] );
wire [1554:1554] sram_blwl_1554_configbus0;
wire [1554:1554] sram_blwl_1554_configbus1;
wire [1554:1554] sram_blwl_1554_configbus0_b;
assign sram_blwl_1554_configbus0[1554:1554] = sram_blwl_bl[1554:1554] ;
assign sram_blwl_1554_configbus1[1554:1554] = sram_blwl_wl[1554:1554] ;
assign sram_blwl_1554_configbus0_b[1554:1554] = sram_blwl_blb[1554:1554] ;
sram6T_blwl sram_blwl_1554_ (sram_blwl_out[1554], sram_blwl_out[1554], sram_blwl_outb[1554], sram_blwl_1554_configbus0[1554:1554], sram_blwl_1554_configbus1[1554:1554] , sram_blwl_1554_configbus0_b[1554:1554] );
wire [1555:1555] sram_blwl_1555_configbus0;
wire [1555:1555] sram_blwl_1555_configbus1;
wire [1555:1555] sram_blwl_1555_configbus0_b;
assign sram_blwl_1555_configbus0[1555:1555] = sram_blwl_bl[1555:1555] ;
assign sram_blwl_1555_configbus1[1555:1555] = sram_blwl_wl[1555:1555] ;
assign sram_blwl_1555_configbus0_b[1555:1555] = sram_blwl_blb[1555:1555] ;
sram6T_blwl sram_blwl_1555_ (sram_blwl_out[1555], sram_blwl_out[1555], sram_blwl_outb[1555], sram_blwl_1555_configbus0[1555:1555], sram_blwl_1555_configbus1[1555:1555] , sram_blwl_1555_configbus0_b[1555:1555] );
wire [1556:1556] sram_blwl_1556_configbus0;
wire [1556:1556] sram_blwl_1556_configbus1;
wire [1556:1556] sram_blwl_1556_configbus0_b;
assign sram_blwl_1556_configbus0[1556:1556] = sram_blwl_bl[1556:1556] ;
assign sram_blwl_1556_configbus1[1556:1556] = sram_blwl_wl[1556:1556] ;
assign sram_blwl_1556_configbus0_b[1556:1556] = sram_blwl_blb[1556:1556] ;
sram6T_blwl sram_blwl_1556_ (sram_blwl_out[1556], sram_blwl_out[1556], sram_blwl_outb[1556], sram_blwl_1556_configbus0[1556:1556], sram_blwl_1556_configbus1[1556:1556] , sram_blwl_1556_configbus0_b[1556:1556] );
wire [1557:1557] sram_blwl_1557_configbus0;
wire [1557:1557] sram_blwl_1557_configbus1;
wire [1557:1557] sram_blwl_1557_configbus0_b;
assign sram_blwl_1557_configbus0[1557:1557] = sram_blwl_bl[1557:1557] ;
assign sram_blwl_1557_configbus1[1557:1557] = sram_blwl_wl[1557:1557] ;
assign sram_blwl_1557_configbus0_b[1557:1557] = sram_blwl_blb[1557:1557] ;
sram6T_blwl sram_blwl_1557_ (sram_blwl_out[1557], sram_blwl_out[1557], sram_blwl_outb[1557], sram_blwl_1557_configbus0[1557:1557], sram_blwl_1557_configbus1[1557:1557] , sram_blwl_1557_configbus0_b[1557:1557] );
wire [1558:1558] sram_blwl_1558_configbus0;
wire [1558:1558] sram_blwl_1558_configbus1;
wire [1558:1558] sram_blwl_1558_configbus0_b;
assign sram_blwl_1558_configbus0[1558:1558] = sram_blwl_bl[1558:1558] ;
assign sram_blwl_1558_configbus1[1558:1558] = sram_blwl_wl[1558:1558] ;
assign sram_blwl_1558_configbus0_b[1558:1558] = sram_blwl_blb[1558:1558] ;
sram6T_blwl sram_blwl_1558_ (sram_blwl_out[1558], sram_blwl_out[1558], sram_blwl_outb[1558], sram_blwl_1558_configbus0[1558:1558], sram_blwl_1558_configbus1[1558:1558] , sram_blwl_1558_configbus0_b[1558:1558] );
wire [1559:1559] sram_blwl_1559_configbus0;
wire [1559:1559] sram_blwl_1559_configbus1;
wire [1559:1559] sram_blwl_1559_configbus0_b;
assign sram_blwl_1559_configbus0[1559:1559] = sram_blwl_bl[1559:1559] ;
assign sram_blwl_1559_configbus1[1559:1559] = sram_blwl_wl[1559:1559] ;
assign sram_blwl_1559_configbus0_b[1559:1559] = sram_blwl_blb[1559:1559] ;
sram6T_blwl sram_blwl_1559_ (sram_blwl_out[1559], sram_blwl_out[1559], sram_blwl_outb[1559], sram_blwl_1559_configbus0[1559:1559], sram_blwl_1559_configbus1[1559:1559] , sram_blwl_1559_configbus0_b[1559:1559] );
wire [1560:1560] sram_blwl_1560_configbus0;
wire [1560:1560] sram_blwl_1560_configbus1;
wire [1560:1560] sram_blwl_1560_configbus0_b;
assign sram_blwl_1560_configbus0[1560:1560] = sram_blwl_bl[1560:1560] ;
assign sram_blwl_1560_configbus1[1560:1560] = sram_blwl_wl[1560:1560] ;
assign sram_blwl_1560_configbus0_b[1560:1560] = sram_blwl_blb[1560:1560] ;
sram6T_blwl sram_blwl_1560_ (sram_blwl_out[1560], sram_blwl_out[1560], sram_blwl_outb[1560], sram_blwl_1560_configbus0[1560:1560], sram_blwl_1560_configbus1[1560:1560] , sram_blwl_1560_configbus0_b[1560:1560] );
wire [1561:1561] sram_blwl_1561_configbus0;
wire [1561:1561] sram_blwl_1561_configbus1;
wire [1561:1561] sram_blwl_1561_configbus0_b;
assign sram_blwl_1561_configbus0[1561:1561] = sram_blwl_bl[1561:1561] ;
assign sram_blwl_1561_configbus1[1561:1561] = sram_blwl_wl[1561:1561] ;
assign sram_blwl_1561_configbus0_b[1561:1561] = sram_blwl_blb[1561:1561] ;
sram6T_blwl sram_blwl_1561_ (sram_blwl_out[1561], sram_blwl_out[1561], sram_blwl_outb[1561], sram_blwl_1561_configbus0[1561:1561], sram_blwl_1561_configbus1[1561:1561] , sram_blwl_1561_configbus0_b[1561:1561] );
wire [1562:1562] sram_blwl_1562_configbus0;
wire [1562:1562] sram_blwl_1562_configbus1;
wire [1562:1562] sram_blwl_1562_configbus0_b;
assign sram_blwl_1562_configbus0[1562:1562] = sram_blwl_bl[1562:1562] ;
assign sram_blwl_1562_configbus1[1562:1562] = sram_blwl_wl[1562:1562] ;
assign sram_blwl_1562_configbus0_b[1562:1562] = sram_blwl_blb[1562:1562] ;
sram6T_blwl sram_blwl_1562_ (sram_blwl_out[1562], sram_blwl_out[1562], sram_blwl_outb[1562], sram_blwl_1562_configbus0[1562:1562], sram_blwl_1562_configbus1[1562:1562] , sram_blwl_1562_configbus0_b[1562:1562] );
wire [1563:1563] sram_blwl_1563_configbus0;
wire [1563:1563] sram_blwl_1563_configbus1;
wire [1563:1563] sram_blwl_1563_configbus0_b;
assign sram_blwl_1563_configbus0[1563:1563] = sram_blwl_bl[1563:1563] ;
assign sram_blwl_1563_configbus1[1563:1563] = sram_blwl_wl[1563:1563] ;
assign sram_blwl_1563_configbus0_b[1563:1563] = sram_blwl_blb[1563:1563] ;
sram6T_blwl sram_blwl_1563_ (sram_blwl_out[1563], sram_blwl_out[1563], sram_blwl_outb[1563], sram_blwl_1563_configbus0[1563:1563], sram_blwl_1563_configbus1[1563:1563] , sram_blwl_1563_configbus0_b[1563:1563] );
wire [1564:1564] sram_blwl_1564_configbus0;
wire [1564:1564] sram_blwl_1564_configbus1;
wire [1564:1564] sram_blwl_1564_configbus0_b;
assign sram_blwl_1564_configbus0[1564:1564] = sram_blwl_bl[1564:1564] ;
assign sram_blwl_1564_configbus1[1564:1564] = sram_blwl_wl[1564:1564] ;
assign sram_blwl_1564_configbus0_b[1564:1564] = sram_blwl_blb[1564:1564] ;
sram6T_blwl sram_blwl_1564_ (sram_blwl_out[1564], sram_blwl_out[1564], sram_blwl_outb[1564], sram_blwl_1564_configbus0[1564:1564], sram_blwl_1564_configbus1[1564:1564] , sram_blwl_1564_configbus0_b[1564:1564] );
wire [1565:1565] sram_blwl_1565_configbus0;
wire [1565:1565] sram_blwl_1565_configbus1;
wire [1565:1565] sram_blwl_1565_configbus0_b;
assign sram_blwl_1565_configbus0[1565:1565] = sram_blwl_bl[1565:1565] ;
assign sram_blwl_1565_configbus1[1565:1565] = sram_blwl_wl[1565:1565] ;
assign sram_blwl_1565_configbus0_b[1565:1565] = sram_blwl_blb[1565:1565] ;
sram6T_blwl sram_blwl_1565_ (sram_blwl_out[1565], sram_blwl_out[1565], sram_blwl_outb[1565], sram_blwl_1565_configbus0[1565:1565], sram_blwl_1565_configbus1[1565:1565] , sram_blwl_1565_configbus0_b[1565:1565] );
wire [1566:1566] sram_blwl_1566_configbus0;
wire [1566:1566] sram_blwl_1566_configbus1;
wire [1566:1566] sram_blwl_1566_configbus0_b;
assign sram_blwl_1566_configbus0[1566:1566] = sram_blwl_bl[1566:1566] ;
assign sram_blwl_1566_configbus1[1566:1566] = sram_blwl_wl[1566:1566] ;
assign sram_blwl_1566_configbus0_b[1566:1566] = sram_blwl_blb[1566:1566] ;
sram6T_blwl sram_blwl_1566_ (sram_blwl_out[1566], sram_blwl_out[1566], sram_blwl_outb[1566], sram_blwl_1566_configbus0[1566:1566], sram_blwl_1566_configbus1[1566:1566] , sram_blwl_1566_configbus0_b[1566:1566] );
wire [1567:1567] sram_blwl_1567_configbus0;
wire [1567:1567] sram_blwl_1567_configbus1;
wire [1567:1567] sram_blwl_1567_configbus0_b;
assign sram_blwl_1567_configbus0[1567:1567] = sram_blwl_bl[1567:1567] ;
assign sram_blwl_1567_configbus1[1567:1567] = sram_blwl_wl[1567:1567] ;
assign sram_blwl_1567_configbus0_b[1567:1567] = sram_blwl_blb[1567:1567] ;
sram6T_blwl sram_blwl_1567_ (sram_blwl_out[1567], sram_blwl_out[1567], sram_blwl_outb[1567], sram_blwl_1567_configbus0[1567:1567], sram_blwl_1567_configbus1[1567:1567] , sram_blwl_1567_configbus0_b[1567:1567] );
wire [1568:1568] sram_blwl_1568_configbus0;
wire [1568:1568] sram_blwl_1568_configbus1;
wire [1568:1568] sram_blwl_1568_configbus0_b;
assign sram_blwl_1568_configbus0[1568:1568] = sram_blwl_bl[1568:1568] ;
assign sram_blwl_1568_configbus1[1568:1568] = sram_blwl_wl[1568:1568] ;
assign sram_blwl_1568_configbus0_b[1568:1568] = sram_blwl_blb[1568:1568] ;
sram6T_blwl sram_blwl_1568_ (sram_blwl_out[1568], sram_blwl_out[1568], sram_blwl_outb[1568], sram_blwl_1568_configbus0[1568:1568], sram_blwl_1568_configbus1[1568:1568] , sram_blwl_1568_configbus0_b[1568:1568] );
wire [1569:1569] sram_blwl_1569_configbus0;
wire [1569:1569] sram_blwl_1569_configbus1;
wire [1569:1569] sram_blwl_1569_configbus0_b;
assign sram_blwl_1569_configbus0[1569:1569] = sram_blwl_bl[1569:1569] ;
assign sram_blwl_1569_configbus1[1569:1569] = sram_blwl_wl[1569:1569] ;
assign sram_blwl_1569_configbus0_b[1569:1569] = sram_blwl_blb[1569:1569] ;
sram6T_blwl sram_blwl_1569_ (sram_blwl_out[1569], sram_blwl_out[1569], sram_blwl_outb[1569], sram_blwl_1569_configbus0[1569:1569], sram_blwl_1569_configbus1[1569:1569] , sram_blwl_1569_configbus0_b[1569:1569] );
wire [1570:1570] sram_blwl_1570_configbus0;
wire [1570:1570] sram_blwl_1570_configbus1;
wire [1570:1570] sram_blwl_1570_configbus0_b;
assign sram_blwl_1570_configbus0[1570:1570] = sram_blwl_bl[1570:1570] ;
assign sram_blwl_1570_configbus1[1570:1570] = sram_blwl_wl[1570:1570] ;
assign sram_blwl_1570_configbus0_b[1570:1570] = sram_blwl_blb[1570:1570] ;
sram6T_blwl sram_blwl_1570_ (sram_blwl_out[1570], sram_blwl_out[1570], sram_blwl_outb[1570], sram_blwl_1570_configbus0[1570:1570], sram_blwl_1570_configbus1[1570:1570] , sram_blwl_1570_configbus0_b[1570:1570] );
wire [1571:1571] sram_blwl_1571_configbus0;
wire [1571:1571] sram_blwl_1571_configbus1;
wire [1571:1571] sram_blwl_1571_configbus0_b;
assign sram_blwl_1571_configbus0[1571:1571] = sram_blwl_bl[1571:1571] ;
assign sram_blwl_1571_configbus1[1571:1571] = sram_blwl_wl[1571:1571] ;
assign sram_blwl_1571_configbus0_b[1571:1571] = sram_blwl_blb[1571:1571] ;
sram6T_blwl sram_blwl_1571_ (sram_blwl_out[1571], sram_blwl_out[1571], sram_blwl_outb[1571], sram_blwl_1571_configbus0[1571:1571], sram_blwl_1571_configbus1[1571:1571] , sram_blwl_1571_configbus0_b[1571:1571] );
wire [1572:1572] sram_blwl_1572_configbus0;
wire [1572:1572] sram_blwl_1572_configbus1;
wire [1572:1572] sram_blwl_1572_configbus0_b;
assign sram_blwl_1572_configbus0[1572:1572] = sram_blwl_bl[1572:1572] ;
assign sram_blwl_1572_configbus1[1572:1572] = sram_blwl_wl[1572:1572] ;
assign sram_blwl_1572_configbus0_b[1572:1572] = sram_blwl_blb[1572:1572] ;
sram6T_blwl sram_blwl_1572_ (sram_blwl_out[1572], sram_blwl_out[1572], sram_blwl_outb[1572], sram_blwl_1572_configbus0[1572:1572], sram_blwl_1572_configbus1[1572:1572] , sram_blwl_1572_configbus0_b[1572:1572] );
wire [1573:1573] sram_blwl_1573_configbus0;
wire [1573:1573] sram_blwl_1573_configbus1;
wire [1573:1573] sram_blwl_1573_configbus0_b;
assign sram_blwl_1573_configbus0[1573:1573] = sram_blwl_bl[1573:1573] ;
assign sram_blwl_1573_configbus1[1573:1573] = sram_blwl_wl[1573:1573] ;
assign sram_blwl_1573_configbus0_b[1573:1573] = sram_blwl_blb[1573:1573] ;
sram6T_blwl sram_blwl_1573_ (sram_blwl_out[1573], sram_blwl_out[1573], sram_blwl_outb[1573], sram_blwl_1573_configbus0[1573:1573], sram_blwl_1573_configbus1[1573:1573] , sram_blwl_1573_configbus0_b[1573:1573] );
wire [1574:1574] sram_blwl_1574_configbus0;
wire [1574:1574] sram_blwl_1574_configbus1;
wire [1574:1574] sram_blwl_1574_configbus0_b;
assign sram_blwl_1574_configbus0[1574:1574] = sram_blwl_bl[1574:1574] ;
assign sram_blwl_1574_configbus1[1574:1574] = sram_blwl_wl[1574:1574] ;
assign sram_blwl_1574_configbus0_b[1574:1574] = sram_blwl_blb[1574:1574] ;
sram6T_blwl sram_blwl_1574_ (sram_blwl_out[1574], sram_blwl_out[1574], sram_blwl_outb[1574], sram_blwl_1574_configbus0[1574:1574], sram_blwl_1574_configbus1[1574:1574] , sram_blwl_1574_configbus0_b[1574:1574] );
wire [1575:1575] sram_blwl_1575_configbus0;
wire [1575:1575] sram_blwl_1575_configbus1;
wire [1575:1575] sram_blwl_1575_configbus0_b;
assign sram_blwl_1575_configbus0[1575:1575] = sram_blwl_bl[1575:1575] ;
assign sram_blwl_1575_configbus1[1575:1575] = sram_blwl_wl[1575:1575] ;
assign sram_blwl_1575_configbus0_b[1575:1575] = sram_blwl_blb[1575:1575] ;
sram6T_blwl sram_blwl_1575_ (sram_blwl_out[1575], sram_blwl_out[1575], sram_blwl_outb[1575], sram_blwl_1575_configbus0[1575:1575], sram_blwl_1575_configbus1[1575:1575] , sram_blwl_1575_configbus0_b[1575:1575] );
wire [1576:1576] sram_blwl_1576_configbus0;
wire [1576:1576] sram_blwl_1576_configbus1;
wire [1576:1576] sram_blwl_1576_configbus0_b;
assign sram_blwl_1576_configbus0[1576:1576] = sram_blwl_bl[1576:1576] ;
assign sram_blwl_1576_configbus1[1576:1576] = sram_blwl_wl[1576:1576] ;
assign sram_blwl_1576_configbus0_b[1576:1576] = sram_blwl_blb[1576:1576] ;
sram6T_blwl sram_blwl_1576_ (sram_blwl_out[1576], sram_blwl_out[1576], sram_blwl_outb[1576], sram_blwl_1576_configbus0[1576:1576], sram_blwl_1576_configbus1[1576:1576] , sram_blwl_1576_configbus0_b[1576:1576] );
wire [1577:1577] sram_blwl_1577_configbus0;
wire [1577:1577] sram_blwl_1577_configbus1;
wire [1577:1577] sram_blwl_1577_configbus0_b;
assign sram_blwl_1577_configbus0[1577:1577] = sram_blwl_bl[1577:1577] ;
assign sram_blwl_1577_configbus1[1577:1577] = sram_blwl_wl[1577:1577] ;
assign sram_blwl_1577_configbus0_b[1577:1577] = sram_blwl_blb[1577:1577] ;
sram6T_blwl sram_blwl_1577_ (sram_blwl_out[1577], sram_blwl_out[1577], sram_blwl_outb[1577], sram_blwl_1577_configbus0[1577:1577], sram_blwl_1577_configbus1[1577:1577] , sram_blwl_1577_configbus0_b[1577:1577] );
wire [1578:1578] sram_blwl_1578_configbus0;
wire [1578:1578] sram_blwl_1578_configbus1;
wire [1578:1578] sram_blwl_1578_configbus0_b;
assign sram_blwl_1578_configbus0[1578:1578] = sram_blwl_bl[1578:1578] ;
assign sram_blwl_1578_configbus1[1578:1578] = sram_blwl_wl[1578:1578] ;
assign sram_blwl_1578_configbus0_b[1578:1578] = sram_blwl_blb[1578:1578] ;
sram6T_blwl sram_blwl_1578_ (sram_blwl_out[1578], sram_blwl_out[1578], sram_blwl_outb[1578], sram_blwl_1578_configbus0[1578:1578], sram_blwl_1578_configbus1[1578:1578] , sram_blwl_1578_configbus0_b[1578:1578] );
wire [1579:1579] sram_blwl_1579_configbus0;
wire [1579:1579] sram_blwl_1579_configbus1;
wire [1579:1579] sram_blwl_1579_configbus0_b;
assign sram_blwl_1579_configbus0[1579:1579] = sram_blwl_bl[1579:1579] ;
assign sram_blwl_1579_configbus1[1579:1579] = sram_blwl_wl[1579:1579] ;
assign sram_blwl_1579_configbus0_b[1579:1579] = sram_blwl_blb[1579:1579] ;
sram6T_blwl sram_blwl_1579_ (sram_blwl_out[1579], sram_blwl_out[1579], sram_blwl_outb[1579], sram_blwl_1579_configbus0[1579:1579], sram_blwl_1579_configbus1[1579:1579] , sram_blwl_1579_configbus0_b[1579:1579] );
wire [1580:1580] sram_blwl_1580_configbus0;
wire [1580:1580] sram_blwl_1580_configbus1;
wire [1580:1580] sram_blwl_1580_configbus0_b;
assign sram_blwl_1580_configbus0[1580:1580] = sram_blwl_bl[1580:1580] ;
assign sram_blwl_1580_configbus1[1580:1580] = sram_blwl_wl[1580:1580] ;
assign sram_blwl_1580_configbus0_b[1580:1580] = sram_blwl_blb[1580:1580] ;
sram6T_blwl sram_blwl_1580_ (sram_blwl_out[1580], sram_blwl_out[1580], sram_blwl_outb[1580], sram_blwl_1580_configbus0[1580:1580], sram_blwl_1580_configbus1[1580:1580] , sram_blwl_1580_configbus0_b[1580:1580] );
wire [1581:1581] sram_blwl_1581_configbus0;
wire [1581:1581] sram_blwl_1581_configbus1;
wire [1581:1581] sram_blwl_1581_configbus0_b;
assign sram_blwl_1581_configbus0[1581:1581] = sram_blwl_bl[1581:1581] ;
assign sram_blwl_1581_configbus1[1581:1581] = sram_blwl_wl[1581:1581] ;
assign sram_blwl_1581_configbus0_b[1581:1581] = sram_blwl_blb[1581:1581] ;
sram6T_blwl sram_blwl_1581_ (sram_blwl_out[1581], sram_blwl_out[1581], sram_blwl_outb[1581], sram_blwl_1581_configbus0[1581:1581], sram_blwl_1581_configbus1[1581:1581] , sram_blwl_1581_configbus0_b[1581:1581] );
wire [1582:1582] sram_blwl_1582_configbus0;
wire [1582:1582] sram_blwl_1582_configbus1;
wire [1582:1582] sram_blwl_1582_configbus0_b;
assign sram_blwl_1582_configbus0[1582:1582] = sram_blwl_bl[1582:1582] ;
assign sram_blwl_1582_configbus1[1582:1582] = sram_blwl_wl[1582:1582] ;
assign sram_blwl_1582_configbus0_b[1582:1582] = sram_blwl_blb[1582:1582] ;
sram6T_blwl sram_blwl_1582_ (sram_blwl_out[1582], sram_blwl_out[1582], sram_blwl_outb[1582], sram_blwl_1582_configbus0[1582:1582], sram_blwl_1582_configbus1[1582:1582] , sram_blwl_1582_configbus0_b[1582:1582] );
wire [1583:1583] sram_blwl_1583_configbus0;
wire [1583:1583] sram_blwl_1583_configbus1;
wire [1583:1583] sram_blwl_1583_configbus0_b;
assign sram_blwl_1583_configbus0[1583:1583] = sram_blwl_bl[1583:1583] ;
assign sram_blwl_1583_configbus1[1583:1583] = sram_blwl_wl[1583:1583] ;
assign sram_blwl_1583_configbus0_b[1583:1583] = sram_blwl_blb[1583:1583] ;
sram6T_blwl sram_blwl_1583_ (sram_blwl_out[1583], sram_blwl_out[1583], sram_blwl_outb[1583], sram_blwl_1583_configbus0[1583:1583], sram_blwl_1583_configbus1[1583:1583] , sram_blwl_1583_configbus0_b[1583:1583] );
wire [1584:1584] sram_blwl_1584_configbus0;
wire [1584:1584] sram_blwl_1584_configbus1;
wire [1584:1584] sram_blwl_1584_configbus0_b;
assign sram_blwl_1584_configbus0[1584:1584] = sram_blwl_bl[1584:1584] ;
assign sram_blwl_1584_configbus1[1584:1584] = sram_blwl_wl[1584:1584] ;
assign sram_blwl_1584_configbus0_b[1584:1584] = sram_blwl_blb[1584:1584] ;
sram6T_blwl sram_blwl_1584_ (sram_blwl_out[1584], sram_blwl_out[1584], sram_blwl_outb[1584], sram_blwl_1584_configbus0[1584:1584], sram_blwl_1584_configbus1[1584:1584] , sram_blwl_1584_configbus0_b[1584:1584] );
wire [1585:1585] sram_blwl_1585_configbus0;
wire [1585:1585] sram_blwl_1585_configbus1;
wire [1585:1585] sram_blwl_1585_configbus0_b;
assign sram_blwl_1585_configbus0[1585:1585] = sram_blwl_bl[1585:1585] ;
assign sram_blwl_1585_configbus1[1585:1585] = sram_blwl_wl[1585:1585] ;
assign sram_blwl_1585_configbus0_b[1585:1585] = sram_blwl_blb[1585:1585] ;
sram6T_blwl sram_blwl_1585_ (sram_blwl_out[1585], sram_blwl_out[1585], sram_blwl_outb[1585], sram_blwl_1585_configbus0[1585:1585], sram_blwl_1585_configbus1[1585:1585] , sram_blwl_1585_configbus0_b[1585:1585] );
wire [1586:1586] sram_blwl_1586_configbus0;
wire [1586:1586] sram_blwl_1586_configbus1;
wire [1586:1586] sram_blwl_1586_configbus0_b;
assign sram_blwl_1586_configbus0[1586:1586] = sram_blwl_bl[1586:1586] ;
assign sram_blwl_1586_configbus1[1586:1586] = sram_blwl_wl[1586:1586] ;
assign sram_blwl_1586_configbus0_b[1586:1586] = sram_blwl_blb[1586:1586] ;
sram6T_blwl sram_blwl_1586_ (sram_blwl_out[1586], sram_blwl_out[1586], sram_blwl_outb[1586], sram_blwl_1586_configbus0[1586:1586], sram_blwl_1586_configbus1[1586:1586] , sram_blwl_1586_configbus0_b[1586:1586] );
wire [1587:1587] sram_blwl_1587_configbus0;
wire [1587:1587] sram_blwl_1587_configbus1;
wire [1587:1587] sram_blwl_1587_configbus0_b;
assign sram_blwl_1587_configbus0[1587:1587] = sram_blwl_bl[1587:1587] ;
assign sram_blwl_1587_configbus1[1587:1587] = sram_blwl_wl[1587:1587] ;
assign sram_blwl_1587_configbus0_b[1587:1587] = sram_blwl_blb[1587:1587] ;
sram6T_blwl sram_blwl_1587_ (sram_blwl_out[1587], sram_blwl_out[1587], sram_blwl_outb[1587], sram_blwl_1587_configbus0[1587:1587], sram_blwl_1587_configbus1[1587:1587] , sram_blwl_1587_configbus0_b[1587:1587] );
wire [1588:1588] sram_blwl_1588_configbus0;
wire [1588:1588] sram_blwl_1588_configbus1;
wire [1588:1588] sram_blwl_1588_configbus0_b;
assign sram_blwl_1588_configbus0[1588:1588] = sram_blwl_bl[1588:1588] ;
assign sram_blwl_1588_configbus1[1588:1588] = sram_blwl_wl[1588:1588] ;
assign sram_blwl_1588_configbus0_b[1588:1588] = sram_blwl_blb[1588:1588] ;
sram6T_blwl sram_blwl_1588_ (sram_blwl_out[1588], sram_blwl_out[1588], sram_blwl_outb[1588], sram_blwl_1588_configbus0[1588:1588], sram_blwl_1588_configbus1[1588:1588] , sram_blwl_1588_configbus0_b[1588:1588] );
wire [1589:1589] sram_blwl_1589_configbus0;
wire [1589:1589] sram_blwl_1589_configbus1;
wire [1589:1589] sram_blwl_1589_configbus0_b;
assign sram_blwl_1589_configbus0[1589:1589] = sram_blwl_bl[1589:1589] ;
assign sram_blwl_1589_configbus1[1589:1589] = sram_blwl_wl[1589:1589] ;
assign sram_blwl_1589_configbus0_b[1589:1589] = sram_blwl_blb[1589:1589] ;
sram6T_blwl sram_blwl_1589_ (sram_blwl_out[1589], sram_blwl_out[1589], sram_blwl_outb[1589], sram_blwl_1589_configbus0[1589:1589], sram_blwl_1589_configbus1[1589:1589] , sram_blwl_1589_configbus0_b[1589:1589] );
wire [1590:1590] sram_blwl_1590_configbus0;
wire [1590:1590] sram_blwl_1590_configbus1;
wire [1590:1590] sram_blwl_1590_configbus0_b;
assign sram_blwl_1590_configbus0[1590:1590] = sram_blwl_bl[1590:1590] ;
assign sram_blwl_1590_configbus1[1590:1590] = sram_blwl_wl[1590:1590] ;
assign sram_blwl_1590_configbus0_b[1590:1590] = sram_blwl_blb[1590:1590] ;
sram6T_blwl sram_blwl_1590_ (sram_blwl_out[1590], sram_blwl_out[1590], sram_blwl_outb[1590], sram_blwl_1590_configbus0[1590:1590], sram_blwl_1590_configbus1[1590:1590] , sram_blwl_1590_configbus0_b[1590:1590] );
wire [1591:1591] sram_blwl_1591_configbus0;
wire [1591:1591] sram_blwl_1591_configbus1;
wire [1591:1591] sram_blwl_1591_configbus0_b;
assign sram_blwl_1591_configbus0[1591:1591] = sram_blwl_bl[1591:1591] ;
assign sram_blwl_1591_configbus1[1591:1591] = sram_blwl_wl[1591:1591] ;
assign sram_blwl_1591_configbus0_b[1591:1591] = sram_blwl_blb[1591:1591] ;
sram6T_blwl sram_blwl_1591_ (sram_blwl_out[1591], sram_blwl_out[1591], sram_blwl_outb[1591], sram_blwl_1591_configbus0[1591:1591], sram_blwl_1591_configbus1[1591:1591] , sram_blwl_1591_configbus0_b[1591:1591] );
wire [1592:1592] sram_blwl_1592_configbus0;
wire [1592:1592] sram_blwl_1592_configbus1;
wire [1592:1592] sram_blwl_1592_configbus0_b;
assign sram_blwl_1592_configbus0[1592:1592] = sram_blwl_bl[1592:1592] ;
assign sram_blwl_1592_configbus1[1592:1592] = sram_blwl_wl[1592:1592] ;
assign sram_blwl_1592_configbus0_b[1592:1592] = sram_blwl_blb[1592:1592] ;
sram6T_blwl sram_blwl_1592_ (sram_blwl_out[1592], sram_blwl_out[1592], sram_blwl_outb[1592], sram_blwl_1592_configbus0[1592:1592], sram_blwl_1592_configbus1[1592:1592] , sram_blwl_1592_configbus0_b[1592:1592] );
wire [1593:1593] sram_blwl_1593_configbus0;
wire [1593:1593] sram_blwl_1593_configbus1;
wire [1593:1593] sram_blwl_1593_configbus0_b;
assign sram_blwl_1593_configbus0[1593:1593] = sram_blwl_bl[1593:1593] ;
assign sram_blwl_1593_configbus1[1593:1593] = sram_blwl_wl[1593:1593] ;
assign sram_blwl_1593_configbus0_b[1593:1593] = sram_blwl_blb[1593:1593] ;
sram6T_blwl sram_blwl_1593_ (sram_blwl_out[1593], sram_blwl_out[1593], sram_blwl_outb[1593], sram_blwl_1593_configbus0[1593:1593], sram_blwl_1593_configbus1[1593:1593] , sram_blwl_1593_configbus0_b[1593:1593] );
wire [1594:1594] sram_blwl_1594_configbus0;
wire [1594:1594] sram_blwl_1594_configbus1;
wire [1594:1594] sram_blwl_1594_configbus0_b;
assign sram_blwl_1594_configbus0[1594:1594] = sram_blwl_bl[1594:1594] ;
assign sram_blwl_1594_configbus1[1594:1594] = sram_blwl_wl[1594:1594] ;
assign sram_blwl_1594_configbus0_b[1594:1594] = sram_blwl_blb[1594:1594] ;
sram6T_blwl sram_blwl_1594_ (sram_blwl_out[1594], sram_blwl_out[1594], sram_blwl_outb[1594], sram_blwl_1594_configbus0[1594:1594], sram_blwl_1594_configbus1[1594:1594] , sram_blwl_1594_configbus0_b[1594:1594] );
wire [1595:1595] sram_blwl_1595_configbus0;
wire [1595:1595] sram_blwl_1595_configbus1;
wire [1595:1595] sram_blwl_1595_configbus0_b;
assign sram_blwl_1595_configbus0[1595:1595] = sram_blwl_bl[1595:1595] ;
assign sram_blwl_1595_configbus1[1595:1595] = sram_blwl_wl[1595:1595] ;
assign sram_blwl_1595_configbus0_b[1595:1595] = sram_blwl_blb[1595:1595] ;
sram6T_blwl sram_blwl_1595_ (sram_blwl_out[1595], sram_blwl_out[1595], sram_blwl_outb[1595], sram_blwl_1595_configbus0[1595:1595], sram_blwl_1595_configbus1[1595:1595] , sram_blwl_1595_configbus0_b[1595:1595] );
wire [1596:1596] sram_blwl_1596_configbus0;
wire [1596:1596] sram_blwl_1596_configbus1;
wire [1596:1596] sram_blwl_1596_configbus0_b;
assign sram_blwl_1596_configbus0[1596:1596] = sram_blwl_bl[1596:1596] ;
assign sram_blwl_1596_configbus1[1596:1596] = sram_blwl_wl[1596:1596] ;
assign sram_blwl_1596_configbus0_b[1596:1596] = sram_blwl_blb[1596:1596] ;
sram6T_blwl sram_blwl_1596_ (sram_blwl_out[1596], sram_blwl_out[1596], sram_blwl_outb[1596], sram_blwl_1596_configbus0[1596:1596], sram_blwl_1596_configbus1[1596:1596] , sram_blwl_1596_configbus0_b[1596:1596] );
wire [1597:1597] sram_blwl_1597_configbus0;
wire [1597:1597] sram_blwl_1597_configbus1;
wire [1597:1597] sram_blwl_1597_configbus0_b;
assign sram_blwl_1597_configbus0[1597:1597] = sram_blwl_bl[1597:1597] ;
assign sram_blwl_1597_configbus1[1597:1597] = sram_blwl_wl[1597:1597] ;
assign sram_blwl_1597_configbus0_b[1597:1597] = sram_blwl_blb[1597:1597] ;
sram6T_blwl sram_blwl_1597_ (sram_blwl_out[1597], sram_blwl_out[1597], sram_blwl_outb[1597], sram_blwl_1597_configbus0[1597:1597], sram_blwl_1597_configbus1[1597:1597] , sram_blwl_1597_configbus0_b[1597:1597] );
wire [1598:1598] sram_blwl_1598_configbus0;
wire [1598:1598] sram_blwl_1598_configbus1;
wire [1598:1598] sram_blwl_1598_configbus0_b;
assign sram_blwl_1598_configbus0[1598:1598] = sram_blwl_bl[1598:1598] ;
assign sram_blwl_1598_configbus1[1598:1598] = sram_blwl_wl[1598:1598] ;
assign sram_blwl_1598_configbus0_b[1598:1598] = sram_blwl_blb[1598:1598] ;
sram6T_blwl sram_blwl_1598_ (sram_blwl_out[1598], sram_blwl_out[1598], sram_blwl_outb[1598], sram_blwl_1598_configbus0[1598:1598], sram_blwl_1598_configbus1[1598:1598] , sram_blwl_1598_configbus0_b[1598:1598] );
wire [1599:1599] sram_blwl_1599_configbus0;
wire [1599:1599] sram_blwl_1599_configbus1;
wire [1599:1599] sram_blwl_1599_configbus0_b;
assign sram_blwl_1599_configbus0[1599:1599] = sram_blwl_bl[1599:1599] ;
assign sram_blwl_1599_configbus1[1599:1599] = sram_blwl_wl[1599:1599] ;
assign sram_blwl_1599_configbus0_b[1599:1599] = sram_blwl_blb[1599:1599] ;
sram6T_blwl sram_blwl_1599_ (sram_blwl_out[1599], sram_blwl_out[1599], sram_blwl_outb[1599], sram_blwl_1599_configbus0[1599:1599], sram_blwl_1599_configbus1[1599:1599] , sram_blwl_1599_configbus0_b[1599:1599] );
endmodule
//----- END LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----

//----- Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----
module grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
input [0:0] Set,
input [0:0] Reset,
input [0:0] clk
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
input wire ff_0___D_0_,
output wire ff_0___Q_0_);
static_dff dff_8_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
endmodule
//----- END Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6_ -----
module grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_ble6___in_0_,
input wire mode_ble6___in_1_,
input wire mode_ble6___in_2_,
input wire mode_ble6___in_3_,
input wire mode_ble6___in_4_,
input wire mode_ble6___in_5_,
output wire mode_ble6___out_0_,
input wire mode_ble6___clk_0_,
input [1536:1600] sram_blwl_bl ,
input [1536:1600] sram_blwl_wl ,
input [1536:1600] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ lut6_0_ (
 lut6_0___in_0_,  lut6_0___in_1_,  lut6_0___in_2_,  lut6_0___in_3_,  lut6_0___in_4_,  lut6_0___in_5_,  lut6_0___out_0_,
sram_blwl_bl[1536:1599] ,
sram_blwl_wl[1536:1599] ,
sram_blwl_blb[1536:1599] );
grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ ff_0_ (
//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
wire [0:1] in_bus_mux_1level_tapbuf_size2_408_ ;
assign in_bus_mux_1level_tapbuf_size2_408_[0] = ff_0___Q_0_ ; 
assign in_bus_mux_1level_tapbuf_size2_408_[1] = lut6_0___out_0_ ; 
wire [1600:1600] mux_1level_tapbuf_size2_408_configbus0;
wire [1600:1600] mux_1level_tapbuf_size2_408_configbus1;
wire [1600:1600] mux_1level_tapbuf_size2_408_sram_blwl_out ;
wire [1600:1600] mux_1level_tapbuf_size2_408_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_408_configbus0[1600:1600] = sram_blwl_bl[1600:1600] ;
assign mux_1level_tapbuf_size2_408_configbus1[1600:1600] = sram_blwl_wl[1600:1600] ;
wire [1600:1600] mux_1level_tapbuf_size2_408_configbus0_b;
assign mux_1level_tapbuf_size2_408_configbus0_b[1600:1600] = sram_blwl_blb[1600:1600] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_408_ (in_bus_mux_1level_tapbuf_size2_408_, mode_ble6___out_0_, mux_1level_tapbuf_size2_408_sram_blwl_out[1600:1600] ,
mux_1level_tapbuf_size2_408_sram_blwl_outb[1600:1600] );
//----- SRAM bits for MUX[408], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_1600_ (mux_1level_tapbuf_size2_408_sram_blwl_out[1600:1600] ,mux_1level_tapbuf_size2_408_sram_blwl_out[1600:1600] ,mux_1level_tapbuf_size2_408_sram_blwl_outb[1600:1600] ,mux_1level_tapbuf_size2_408_configbus0[1600:1600], mux_1level_tapbuf_size2_408_configbus1[1600:1600] , mux_1level_tapbuf_size2_408_configbus0_b[1600:1600] );
direct_interc direct_interc_128_ (mode_ble6___in_0_, lut6_0___in_0_ );
direct_interc direct_interc_129_ (mode_ble6___in_1_, lut6_0___in_1_ );
direct_interc direct_interc_130_ (mode_ble6___in_2_, lut6_0___in_2_ );
direct_interc direct_interc_131_ (mode_ble6___in_3_, lut6_0___in_3_ );
direct_interc direct_interc_132_ (mode_ble6___in_4_, lut6_0___in_4_ );
direct_interc direct_interc_133_ (mode_ble6___in_5_, lut6_0___in_5_ );
direct_interc direct_interc_134_ (lut6_0___out_0_, ff_0___D_0_ );
direct_interc direct_interc_135_ (mode_ble6___clk_0_, ff_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6_ -----

//----- Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6_ -----
module grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_n1_lut6___in_0_,
input wire mode_n1_lut6___in_1_,
input wire mode_n1_lut6___in_2_,
input wire mode_n1_lut6___in_3_,
input wire mode_n1_lut6___in_4_,
input wire mode_n1_lut6___in_5_,
output wire mode_n1_lut6___out_0_,
input wire mode_n1_lut6___clk_0_,
input [1536:1600] sram_blwl_bl ,
input [1536:1600] sram_blwl_wl ,
input [1536:1600] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6__ble6_0__mode_ble6_ ble6_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 ble6_0___in_0_,  ble6_0___in_1_,  ble6_0___in_2_,  ble6_0___in_3_,  ble6_0___in_4_,  ble6_0___in_5_,  ble6_0___out_0_,  ble6_0___clk_0_,
sram_blwl_bl[1536:1600] ,
sram_blwl_wl[1536:1600] ,
sram_blwl_blb[1536:1600] );
direct_interc direct_interc_136_ (ble6_0___out_0_, mode_n1_lut6___out_0_ );
direct_interc direct_interc_137_ (mode_n1_lut6___in_0_, ble6_0___in_0_ );
direct_interc direct_interc_138_ (mode_n1_lut6___in_1_, ble6_0___in_1_ );
direct_interc direct_interc_139_ (mode_n1_lut6___in_2_, ble6_0___in_2_ );
direct_interc direct_interc_140_ (mode_n1_lut6___in_3_, ble6_0___in_3_ );
direct_interc direct_interc_141_ (mode_n1_lut6___in_4_, ble6_0___in_4_ );
direct_interc direct_interc_142_ (mode_n1_lut6___in_5_, ble6_0___in_5_ );
direct_interc direct_interc_143_ (mode_n1_lut6___clk_0_, ble6_0___clk_0_ );
endmodule
//----- END Idle programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6_ -----

//----- LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----
module grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ (
input wire lut6_0___in_0_,
input wire lut6_0___in_1_,
input wire lut6_0___in_2_,
input wire lut6_0___in_3_,
input wire lut6_0___in_4_,
input wire lut6_0___in_5_,
output wire lut6_0___out_0_,
input [1601:1664] sram_blwl_bl ,
input [1601:1664] sram_blwl_wl ,
input [1601:1664] sram_blwl_blb );
wire [0:5] lut6_0___in;
assign lut6_0___in[0] = lut6_0___in_0_;
assign lut6_0___in[1] = lut6_0___in_1_;
assign lut6_0___in[2] = lut6_0___in_2_;
assign lut6_0___in[3] = lut6_0___in_3_;
assign lut6_0___in[4] = lut6_0___in_4_;
assign lut6_0___in[5] = lut6_0___in_5_;
wire [0:0] lut6_0___out;
assign lut6_0___out_0_ = lut6_0___out[0];
wire [1601:1664] sram_blwl_out ;
wire [1601:1664] sram_blwl_outb ;
lut6 lut6_9_ (
//----- Input and output ports -----
 lut6_0___in[0:5] ,  lut6_0___out[0:0],//----- SRAM ports -----
sram_blwl_out[1601:1664] , sram_blwl_outb[1601:1664] );
//----- Truth Table for LUT node (n7). -----
//----- Truth Table for LUT[9], size=6. -----
//  0----- 1 
//----- SRAM bits for LUT[9], size=6, num_sram=64. -----
//-----0101010101010101010101010101010101010101010101010101010101010101-----
wire [1601:1601] sram_blwl_1601_configbus0;
wire [1601:1601] sram_blwl_1601_configbus1;
wire [1601:1601] sram_blwl_1601_configbus0_b;
assign sram_blwl_1601_configbus0[1601:1601] = sram_blwl_bl[1601:1601] ;
assign sram_blwl_1601_configbus1[1601:1601] = sram_blwl_wl[1601:1601] ;
assign sram_blwl_1601_configbus0_b[1601:1601] = sram_blwl_blb[1601:1601] ;
sram6T_blwl sram_blwl_1601_ (sram_blwl_out[1601], sram_blwl_out[1601], sram_blwl_outb[1601], sram_blwl_1601_configbus0[1601:1601], sram_blwl_1601_configbus1[1601:1601] , sram_blwl_1601_configbus0_b[1601:1601] );
wire [1602:1602] sram_blwl_1602_configbus0;
wire [1602:1602] sram_blwl_1602_configbus1;
wire [1602:1602] sram_blwl_1602_configbus0_b;
assign sram_blwl_1602_configbus0[1602:1602] = sram_blwl_bl[1602:1602] ;
assign sram_blwl_1602_configbus1[1602:1602] = sram_blwl_wl[1602:1602] ;
assign sram_blwl_1602_configbus0_b[1602:1602] = sram_blwl_blb[1602:1602] ;
sram6T_blwl sram_blwl_1602_ (sram_blwl_out[1602], sram_blwl_out[1602], sram_blwl_outb[1602], sram_blwl_1602_configbus0[1602:1602], sram_blwl_1602_configbus1[1602:1602] , sram_blwl_1602_configbus0_b[1602:1602] );
wire [1603:1603] sram_blwl_1603_configbus0;
wire [1603:1603] sram_blwl_1603_configbus1;
wire [1603:1603] sram_blwl_1603_configbus0_b;
assign sram_blwl_1603_configbus0[1603:1603] = sram_blwl_bl[1603:1603] ;
assign sram_blwl_1603_configbus1[1603:1603] = sram_blwl_wl[1603:1603] ;
assign sram_blwl_1603_configbus0_b[1603:1603] = sram_blwl_blb[1603:1603] ;
sram6T_blwl sram_blwl_1603_ (sram_blwl_out[1603], sram_blwl_out[1603], sram_blwl_outb[1603], sram_blwl_1603_configbus0[1603:1603], sram_blwl_1603_configbus1[1603:1603] , sram_blwl_1603_configbus0_b[1603:1603] );
wire [1604:1604] sram_blwl_1604_configbus0;
wire [1604:1604] sram_blwl_1604_configbus1;
wire [1604:1604] sram_blwl_1604_configbus0_b;
assign sram_blwl_1604_configbus0[1604:1604] = sram_blwl_bl[1604:1604] ;
assign sram_blwl_1604_configbus1[1604:1604] = sram_blwl_wl[1604:1604] ;
assign sram_blwl_1604_configbus0_b[1604:1604] = sram_blwl_blb[1604:1604] ;
sram6T_blwl sram_blwl_1604_ (sram_blwl_out[1604], sram_blwl_out[1604], sram_blwl_outb[1604], sram_blwl_1604_configbus0[1604:1604], sram_blwl_1604_configbus1[1604:1604] , sram_blwl_1604_configbus0_b[1604:1604] );
wire [1605:1605] sram_blwl_1605_configbus0;
wire [1605:1605] sram_blwl_1605_configbus1;
wire [1605:1605] sram_blwl_1605_configbus0_b;
assign sram_blwl_1605_configbus0[1605:1605] = sram_blwl_bl[1605:1605] ;
assign sram_blwl_1605_configbus1[1605:1605] = sram_blwl_wl[1605:1605] ;
assign sram_blwl_1605_configbus0_b[1605:1605] = sram_blwl_blb[1605:1605] ;
sram6T_blwl sram_blwl_1605_ (sram_blwl_out[1605], sram_blwl_out[1605], sram_blwl_outb[1605], sram_blwl_1605_configbus0[1605:1605], sram_blwl_1605_configbus1[1605:1605] , sram_blwl_1605_configbus0_b[1605:1605] );
wire [1606:1606] sram_blwl_1606_configbus0;
wire [1606:1606] sram_blwl_1606_configbus1;
wire [1606:1606] sram_blwl_1606_configbus0_b;
assign sram_blwl_1606_configbus0[1606:1606] = sram_blwl_bl[1606:1606] ;
assign sram_blwl_1606_configbus1[1606:1606] = sram_blwl_wl[1606:1606] ;
assign sram_blwl_1606_configbus0_b[1606:1606] = sram_blwl_blb[1606:1606] ;
sram6T_blwl sram_blwl_1606_ (sram_blwl_out[1606], sram_blwl_out[1606], sram_blwl_outb[1606], sram_blwl_1606_configbus0[1606:1606], sram_blwl_1606_configbus1[1606:1606] , sram_blwl_1606_configbus0_b[1606:1606] );
wire [1607:1607] sram_blwl_1607_configbus0;
wire [1607:1607] sram_blwl_1607_configbus1;
wire [1607:1607] sram_blwl_1607_configbus0_b;
assign sram_blwl_1607_configbus0[1607:1607] = sram_blwl_bl[1607:1607] ;
assign sram_blwl_1607_configbus1[1607:1607] = sram_blwl_wl[1607:1607] ;
assign sram_blwl_1607_configbus0_b[1607:1607] = sram_blwl_blb[1607:1607] ;
sram6T_blwl sram_blwl_1607_ (sram_blwl_out[1607], sram_blwl_out[1607], sram_blwl_outb[1607], sram_blwl_1607_configbus0[1607:1607], sram_blwl_1607_configbus1[1607:1607] , sram_blwl_1607_configbus0_b[1607:1607] );
wire [1608:1608] sram_blwl_1608_configbus0;
wire [1608:1608] sram_blwl_1608_configbus1;
wire [1608:1608] sram_blwl_1608_configbus0_b;
assign sram_blwl_1608_configbus0[1608:1608] = sram_blwl_bl[1608:1608] ;
assign sram_blwl_1608_configbus1[1608:1608] = sram_blwl_wl[1608:1608] ;
assign sram_blwl_1608_configbus0_b[1608:1608] = sram_blwl_blb[1608:1608] ;
sram6T_blwl sram_blwl_1608_ (sram_blwl_out[1608], sram_blwl_out[1608], sram_blwl_outb[1608], sram_blwl_1608_configbus0[1608:1608], sram_blwl_1608_configbus1[1608:1608] , sram_blwl_1608_configbus0_b[1608:1608] );
wire [1609:1609] sram_blwl_1609_configbus0;
wire [1609:1609] sram_blwl_1609_configbus1;
wire [1609:1609] sram_blwl_1609_configbus0_b;
assign sram_blwl_1609_configbus0[1609:1609] = sram_blwl_bl[1609:1609] ;
assign sram_blwl_1609_configbus1[1609:1609] = sram_blwl_wl[1609:1609] ;
assign sram_blwl_1609_configbus0_b[1609:1609] = sram_blwl_blb[1609:1609] ;
sram6T_blwl sram_blwl_1609_ (sram_blwl_out[1609], sram_blwl_out[1609], sram_blwl_outb[1609], sram_blwl_1609_configbus0[1609:1609], sram_blwl_1609_configbus1[1609:1609] , sram_blwl_1609_configbus0_b[1609:1609] );
wire [1610:1610] sram_blwl_1610_configbus0;
wire [1610:1610] sram_blwl_1610_configbus1;
wire [1610:1610] sram_blwl_1610_configbus0_b;
assign sram_blwl_1610_configbus0[1610:1610] = sram_blwl_bl[1610:1610] ;
assign sram_blwl_1610_configbus1[1610:1610] = sram_blwl_wl[1610:1610] ;
assign sram_blwl_1610_configbus0_b[1610:1610] = sram_blwl_blb[1610:1610] ;
sram6T_blwl sram_blwl_1610_ (sram_blwl_out[1610], sram_blwl_out[1610], sram_blwl_outb[1610], sram_blwl_1610_configbus0[1610:1610], sram_blwl_1610_configbus1[1610:1610] , sram_blwl_1610_configbus0_b[1610:1610] );
wire [1611:1611] sram_blwl_1611_configbus0;
wire [1611:1611] sram_blwl_1611_configbus1;
wire [1611:1611] sram_blwl_1611_configbus0_b;
assign sram_blwl_1611_configbus0[1611:1611] = sram_blwl_bl[1611:1611] ;
assign sram_blwl_1611_configbus1[1611:1611] = sram_blwl_wl[1611:1611] ;
assign sram_blwl_1611_configbus0_b[1611:1611] = sram_blwl_blb[1611:1611] ;
sram6T_blwl sram_blwl_1611_ (sram_blwl_out[1611], sram_blwl_out[1611], sram_blwl_outb[1611], sram_blwl_1611_configbus0[1611:1611], sram_blwl_1611_configbus1[1611:1611] , sram_blwl_1611_configbus0_b[1611:1611] );
wire [1612:1612] sram_blwl_1612_configbus0;
wire [1612:1612] sram_blwl_1612_configbus1;
wire [1612:1612] sram_blwl_1612_configbus0_b;
assign sram_blwl_1612_configbus0[1612:1612] = sram_blwl_bl[1612:1612] ;
assign sram_blwl_1612_configbus1[1612:1612] = sram_blwl_wl[1612:1612] ;
assign sram_blwl_1612_configbus0_b[1612:1612] = sram_blwl_blb[1612:1612] ;
sram6T_blwl sram_blwl_1612_ (sram_blwl_out[1612], sram_blwl_out[1612], sram_blwl_outb[1612], sram_blwl_1612_configbus0[1612:1612], sram_blwl_1612_configbus1[1612:1612] , sram_blwl_1612_configbus0_b[1612:1612] );
wire [1613:1613] sram_blwl_1613_configbus0;
wire [1613:1613] sram_blwl_1613_configbus1;
wire [1613:1613] sram_blwl_1613_configbus0_b;
assign sram_blwl_1613_configbus0[1613:1613] = sram_blwl_bl[1613:1613] ;
assign sram_blwl_1613_configbus1[1613:1613] = sram_blwl_wl[1613:1613] ;
assign sram_blwl_1613_configbus0_b[1613:1613] = sram_blwl_blb[1613:1613] ;
sram6T_blwl sram_blwl_1613_ (sram_blwl_out[1613], sram_blwl_out[1613], sram_blwl_outb[1613], sram_blwl_1613_configbus0[1613:1613], sram_blwl_1613_configbus1[1613:1613] , sram_blwl_1613_configbus0_b[1613:1613] );
wire [1614:1614] sram_blwl_1614_configbus0;
wire [1614:1614] sram_blwl_1614_configbus1;
wire [1614:1614] sram_blwl_1614_configbus0_b;
assign sram_blwl_1614_configbus0[1614:1614] = sram_blwl_bl[1614:1614] ;
assign sram_blwl_1614_configbus1[1614:1614] = sram_blwl_wl[1614:1614] ;
assign sram_blwl_1614_configbus0_b[1614:1614] = sram_blwl_blb[1614:1614] ;
sram6T_blwl sram_blwl_1614_ (sram_blwl_out[1614], sram_blwl_out[1614], sram_blwl_outb[1614], sram_blwl_1614_configbus0[1614:1614], sram_blwl_1614_configbus1[1614:1614] , sram_blwl_1614_configbus0_b[1614:1614] );
wire [1615:1615] sram_blwl_1615_configbus0;
wire [1615:1615] sram_blwl_1615_configbus1;
wire [1615:1615] sram_blwl_1615_configbus0_b;
assign sram_blwl_1615_configbus0[1615:1615] = sram_blwl_bl[1615:1615] ;
assign sram_blwl_1615_configbus1[1615:1615] = sram_blwl_wl[1615:1615] ;
assign sram_blwl_1615_configbus0_b[1615:1615] = sram_blwl_blb[1615:1615] ;
sram6T_blwl sram_blwl_1615_ (sram_blwl_out[1615], sram_blwl_out[1615], sram_blwl_outb[1615], sram_blwl_1615_configbus0[1615:1615], sram_blwl_1615_configbus1[1615:1615] , sram_blwl_1615_configbus0_b[1615:1615] );
wire [1616:1616] sram_blwl_1616_configbus0;
wire [1616:1616] sram_blwl_1616_configbus1;
wire [1616:1616] sram_blwl_1616_configbus0_b;
assign sram_blwl_1616_configbus0[1616:1616] = sram_blwl_bl[1616:1616] ;
assign sram_blwl_1616_configbus1[1616:1616] = sram_blwl_wl[1616:1616] ;
assign sram_blwl_1616_configbus0_b[1616:1616] = sram_blwl_blb[1616:1616] ;
sram6T_blwl sram_blwl_1616_ (sram_blwl_out[1616], sram_blwl_out[1616], sram_blwl_outb[1616], sram_blwl_1616_configbus0[1616:1616], sram_blwl_1616_configbus1[1616:1616] , sram_blwl_1616_configbus0_b[1616:1616] );
wire [1617:1617] sram_blwl_1617_configbus0;
wire [1617:1617] sram_blwl_1617_configbus1;
wire [1617:1617] sram_blwl_1617_configbus0_b;
assign sram_blwl_1617_configbus0[1617:1617] = sram_blwl_bl[1617:1617] ;
assign sram_blwl_1617_configbus1[1617:1617] = sram_blwl_wl[1617:1617] ;
assign sram_blwl_1617_configbus0_b[1617:1617] = sram_blwl_blb[1617:1617] ;
sram6T_blwl sram_blwl_1617_ (sram_blwl_out[1617], sram_blwl_out[1617], sram_blwl_outb[1617], sram_blwl_1617_configbus0[1617:1617], sram_blwl_1617_configbus1[1617:1617] , sram_blwl_1617_configbus0_b[1617:1617] );
wire [1618:1618] sram_blwl_1618_configbus0;
wire [1618:1618] sram_blwl_1618_configbus1;
wire [1618:1618] sram_blwl_1618_configbus0_b;
assign sram_blwl_1618_configbus0[1618:1618] = sram_blwl_bl[1618:1618] ;
assign sram_blwl_1618_configbus1[1618:1618] = sram_blwl_wl[1618:1618] ;
assign sram_blwl_1618_configbus0_b[1618:1618] = sram_blwl_blb[1618:1618] ;
sram6T_blwl sram_blwl_1618_ (sram_blwl_out[1618], sram_blwl_out[1618], sram_blwl_outb[1618], sram_blwl_1618_configbus0[1618:1618], sram_blwl_1618_configbus1[1618:1618] , sram_blwl_1618_configbus0_b[1618:1618] );
wire [1619:1619] sram_blwl_1619_configbus0;
wire [1619:1619] sram_blwl_1619_configbus1;
wire [1619:1619] sram_blwl_1619_configbus0_b;
assign sram_blwl_1619_configbus0[1619:1619] = sram_blwl_bl[1619:1619] ;
assign sram_blwl_1619_configbus1[1619:1619] = sram_blwl_wl[1619:1619] ;
assign sram_blwl_1619_configbus0_b[1619:1619] = sram_blwl_blb[1619:1619] ;
sram6T_blwl sram_blwl_1619_ (sram_blwl_out[1619], sram_blwl_out[1619], sram_blwl_outb[1619], sram_blwl_1619_configbus0[1619:1619], sram_blwl_1619_configbus1[1619:1619] , sram_blwl_1619_configbus0_b[1619:1619] );
wire [1620:1620] sram_blwl_1620_configbus0;
wire [1620:1620] sram_blwl_1620_configbus1;
wire [1620:1620] sram_blwl_1620_configbus0_b;
assign sram_blwl_1620_configbus0[1620:1620] = sram_blwl_bl[1620:1620] ;
assign sram_blwl_1620_configbus1[1620:1620] = sram_blwl_wl[1620:1620] ;
assign sram_blwl_1620_configbus0_b[1620:1620] = sram_blwl_blb[1620:1620] ;
sram6T_blwl sram_blwl_1620_ (sram_blwl_out[1620], sram_blwl_out[1620], sram_blwl_outb[1620], sram_blwl_1620_configbus0[1620:1620], sram_blwl_1620_configbus1[1620:1620] , sram_blwl_1620_configbus0_b[1620:1620] );
wire [1621:1621] sram_blwl_1621_configbus0;
wire [1621:1621] sram_blwl_1621_configbus1;
wire [1621:1621] sram_blwl_1621_configbus0_b;
assign sram_blwl_1621_configbus0[1621:1621] = sram_blwl_bl[1621:1621] ;
assign sram_blwl_1621_configbus1[1621:1621] = sram_blwl_wl[1621:1621] ;
assign sram_blwl_1621_configbus0_b[1621:1621] = sram_blwl_blb[1621:1621] ;
sram6T_blwl sram_blwl_1621_ (sram_blwl_out[1621], sram_blwl_out[1621], sram_blwl_outb[1621], sram_blwl_1621_configbus0[1621:1621], sram_blwl_1621_configbus1[1621:1621] , sram_blwl_1621_configbus0_b[1621:1621] );
wire [1622:1622] sram_blwl_1622_configbus0;
wire [1622:1622] sram_blwl_1622_configbus1;
wire [1622:1622] sram_blwl_1622_configbus0_b;
assign sram_blwl_1622_configbus0[1622:1622] = sram_blwl_bl[1622:1622] ;
assign sram_blwl_1622_configbus1[1622:1622] = sram_blwl_wl[1622:1622] ;
assign sram_blwl_1622_configbus0_b[1622:1622] = sram_blwl_blb[1622:1622] ;
sram6T_blwl sram_blwl_1622_ (sram_blwl_out[1622], sram_blwl_out[1622], sram_blwl_outb[1622], sram_blwl_1622_configbus0[1622:1622], sram_blwl_1622_configbus1[1622:1622] , sram_blwl_1622_configbus0_b[1622:1622] );
wire [1623:1623] sram_blwl_1623_configbus0;
wire [1623:1623] sram_blwl_1623_configbus1;
wire [1623:1623] sram_blwl_1623_configbus0_b;
assign sram_blwl_1623_configbus0[1623:1623] = sram_blwl_bl[1623:1623] ;
assign sram_blwl_1623_configbus1[1623:1623] = sram_blwl_wl[1623:1623] ;
assign sram_blwl_1623_configbus0_b[1623:1623] = sram_blwl_blb[1623:1623] ;
sram6T_blwl sram_blwl_1623_ (sram_blwl_out[1623], sram_blwl_out[1623], sram_blwl_outb[1623], sram_blwl_1623_configbus0[1623:1623], sram_blwl_1623_configbus1[1623:1623] , sram_blwl_1623_configbus0_b[1623:1623] );
wire [1624:1624] sram_blwl_1624_configbus0;
wire [1624:1624] sram_blwl_1624_configbus1;
wire [1624:1624] sram_blwl_1624_configbus0_b;
assign sram_blwl_1624_configbus0[1624:1624] = sram_blwl_bl[1624:1624] ;
assign sram_blwl_1624_configbus1[1624:1624] = sram_blwl_wl[1624:1624] ;
assign sram_blwl_1624_configbus0_b[1624:1624] = sram_blwl_blb[1624:1624] ;
sram6T_blwl sram_blwl_1624_ (sram_blwl_out[1624], sram_blwl_out[1624], sram_blwl_outb[1624], sram_blwl_1624_configbus0[1624:1624], sram_blwl_1624_configbus1[1624:1624] , sram_blwl_1624_configbus0_b[1624:1624] );
wire [1625:1625] sram_blwl_1625_configbus0;
wire [1625:1625] sram_blwl_1625_configbus1;
wire [1625:1625] sram_blwl_1625_configbus0_b;
assign sram_blwl_1625_configbus0[1625:1625] = sram_blwl_bl[1625:1625] ;
assign sram_blwl_1625_configbus1[1625:1625] = sram_blwl_wl[1625:1625] ;
assign sram_blwl_1625_configbus0_b[1625:1625] = sram_blwl_blb[1625:1625] ;
sram6T_blwl sram_blwl_1625_ (sram_blwl_out[1625], sram_blwl_out[1625], sram_blwl_outb[1625], sram_blwl_1625_configbus0[1625:1625], sram_blwl_1625_configbus1[1625:1625] , sram_blwl_1625_configbus0_b[1625:1625] );
wire [1626:1626] sram_blwl_1626_configbus0;
wire [1626:1626] sram_blwl_1626_configbus1;
wire [1626:1626] sram_blwl_1626_configbus0_b;
assign sram_blwl_1626_configbus0[1626:1626] = sram_blwl_bl[1626:1626] ;
assign sram_blwl_1626_configbus1[1626:1626] = sram_blwl_wl[1626:1626] ;
assign sram_blwl_1626_configbus0_b[1626:1626] = sram_blwl_blb[1626:1626] ;
sram6T_blwl sram_blwl_1626_ (sram_blwl_out[1626], sram_blwl_out[1626], sram_blwl_outb[1626], sram_blwl_1626_configbus0[1626:1626], sram_blwl_1626_configbus1[1626:1626] , sram_blwl_1626_configbus0_b[1626:1626] );
wire [1627:1627] sram_blwl_1627_configbus0;
wire [1627:1627] sram_blwl_1627_configbus1;
wire [1627:1627] sram_blwl_1627_configbus0_b;
assign sram_blwl_1627_configbus0[1627:1627] = sram_blwl_bl[1627:1627] ;
assign sram_blwl_1627_configbus1[1627:1627] = sram_blwl_wl[1627:1627] ;
assign sram_blwl_1627_configbus0_b[1627:1627] = sram_blwl_blb[1627:1627] ;
sram6T_blwl sram_blwl_1627_ (sram_blwl_out[1627], sram_blwl_out[1627], sram_blwl_outb[1627], sram_blwl_1627_configbus0[1627:1627], sram_blwl_1627_configbus1[1627:1627] , sram_blwl_1627_configbus0_b[1627:1627] );
wire [1628:1628] sram_blwl_1628_configbus0;
wire [1628:1628] sram_blwl_1628_configbus1;
wire [1628:1628] sram_blwl_1628_configbus0_b;
assign sram_blwl_1628_configbus0[1628:1628] = sram_blwl_bl[1628:1628] ;
assign sram_blwl_1628_configbus1[1628:1628] = sram_blwl_wl[1628:1628] ;
assign sram_blwl_1628_configbus0_b[1628:1628] = sram_blwl_blb[1628:1628] ;
sram6T_blwl sram_blwl_1628_ (sram_blwl_out[1628], sram_blwl_out[1628], sram_blwl_outb[1628], sram_blwl_1628_configbus0[1628:1628], sram_blwl_1628_configbus1[1628:1628] , sram_blwl_1628_configbus0_b[1628:1628] );
wire [1629:1629] sram_blwl_1629_configbus0;
wire [1629:1629] sram_blwl_1629_configbus1;
wire [1629:1629] sram_blwl_1629_configbus0_b;
assign sram_blwl_1629_configbus0[1629:1629] = sram_blwl_bl[1629:1629] ;
assign sram_blwl_1629_configbus1[1629:1629] = sram_blwl_wl[1629:1629] ;
assign sram_blwl_1629_configbus0_b[1629:1629] = sram_blwl_blb[1629:1629] ;
sram6T_blwl sram_blwl_1629_ (sram_blwl_out[1629], sram_blwl_out[1629], sram_blwl_outb[1629], sram_blwl_1629_configbus0[1629:1629], sram_blwl_1629_configbus1[1629:1629] , sram_blwl_1629_configbus0_b[1629:1629] );
wire [1630:1630] sram_blwl_1630_configbus0;
wire [1630:1630] sram_blwl_1630_configbus1;
wire [1630:1630] sram_blwl_1630_configbus0_b;
assign sram_blwl_1630_configbus0[1630:1630] = sram_blwl_bl[1630:1630] ;
assign sram_blwl_1630_configbus1[1630:1630] = sram_blwl_wl[1630:1630] ;
assign sram_blwl_1630_configbus0_b[1630:1630] = sram_blwl_blb[1630:1630] ;
sram6T_blwl sram_blwl_1630_ (sram_blwl_out[1630], sram_blwl_out[1630], sram_blwl_outb[1630], sram_blwl_1630_configbus0[1630:1630], sram_blwl_1630_configbus1[1630:1630] , sram_blwl_1630_configbus0_b[1630:1630] );
wire [1631:1631] sram_blwl_1631_configbus0;
wire [1631:1631] sram_blwl_1631_configbus1;
wire [1631:1631] sram_blwl_1631_configbus0_b;
assign sram_blwl_1631_configbus0[1631:1631] = sram_blwl_bl[1631:1631] ;
assign sram_blwl_1631_configbus1[1631:1631] = sram_blwl_wl[1631:1631] ;
assign sram_blwl_1631_configbus0_b[1631:1631] = sram_blwl_blb[1631:1631] ;
sram6T_blwl sram_blwl_1631_ (sram_blwl_out[1631], sram_blwl_out[1631], sram_blwl_outb[1631], sram_blwl_1631_configbus0[1631:1631], sram_blwl_1631_configbus1[1631:1631] , sram_blwl_1631_configbus0_b[1631:1631] );
wire [1632:1632] sram_blwl_1632_configbus0;
wire [1632:1632] sram_blwl_1632_configbus1;
wire [1632:1632] sram_blwl_1632_configbus0_b;
assign sram_blwl_1632_configbus0[1632:1632] = sram_blwl_bl[1632:1632] ;
assign sram_blwl_1632_configbus1[1632:1632] = sram_blwl_wl[1632:1632] ;
assign sram_blwl_1632_configbus0_b[1632:1632] = sram_blwl_blb[1632:1632] ;
sram6T_blwl sram_blwl_1632_ (sram_blwl_out[1632], sram_blwl_out[1632], sram_blwl_outb[1632], sram_blwl_1632_configbus0[1632:1632], sram_blwl_1632_configbus1[1632:1632] , sram_blwl_1632_configbus0_b[1632:1632] );
wire [1633:1633] sram_blwl_1633_configbus0;
wire [1633:1633] sram_blwl_1633_configbus1;
wire [1633:1633] sram_blwl_1633_configbus0_b;
assign sram_blwl_1633_configbus0[1633:1633] = sram_blwl_bl[1633:1633] ;
assign sram_blwl_1633_configbus1[1633:1633] = sram_blwl_wl[1633:1633] ;
assign sram_blwl_1633_configbus0_b[1633:1633] = sram_blwl_blb[1633:1633] ;
sram6T_blwl sram_blwl_1633_ (sram_blwl_out[1633], sram_blwl_out[1633], sram_blwl_outb[1633], sram_blwl_1633_configbus0[1633:1633], sram_blwl_1633_configbus1[1633:1633] , sram_blwl_1633_configbus0_b[1633:1633] );
wire [1634:1634] sram_blwl_1634_configbus0;
wire [1634:1634] sram_blwl_1634_configbus1;
wire [1634:1634] sram_blwl_1634_configbus0_b;
assign sram_blwl_1634_configbus0[1634:1634] = sram_blwl_bl[1634:1634] ;
assign sram_blwl_1634_configbus1[1634:1634] = sram_blwl_wl[1634:1634] ;
assign sram_blwl_1634_configbus0_b[1634:1634] = sram_blwl_blb[1634:1634] ;
sram6T_blwl sram_blwl_1634_ (sram_blwl_out[1634], sram_blwl_out[1634], sram_blwl_outb[1634], sram_blwl_1634_configbus0[1634:1634], sram_blwl_1634_configbus1[1634:1634] , sram_blwl_1634_configbus0_b[1634:1634] );
wire [1635:1635] sram_blwl_1635_configbus0;
wire [1635:1635] sram_blwl_1635_configbus1;
wire [1635:1635] sram_blwl_1635_configbus0_b;
assign sram_blwl_1635_configbus0[1635:1635] = sram_blwl_bl[1635:1635] ;
assign sram_blwl_1635_configbus1[1635:1635] = sram_blwl_wl[1635:1635] ;
assign sram_blwl_1635_configbus0_b[1635:1635] = sram_blwl_blb[1635:1635] ;
sram6T_blwl sram_blwl_1635_ (sram_blwl_out[1635], sram_blwl_out[1635], sram_blwl_outb[1635], sram_blwl_1635_configbus0[1635:1635], sram_blwl_1635_configbus1[1635:1635] , sram_blwl_1635_configbus0_b[1635:1635] );
wire [1636:1636] sram_blwl_1636_configbus0;
wire [1636:1636] sram_blwl_1636_configbus1;
wire [1636:1636] sram_blwl_1636_configbus0_b;
assign sram_blwl_1636_configbus0[1636:1636] = sram_blwl_bl[1636:1636] ;
assign sram_blwl_1636_configbus1[1636:1636] = sram_blwl_wl[1636:1636] ;
assign sram_blwl_1636_configbus0_b[1636:1636] = sram_blwl_blb[1636:1636] ;
sram6T_blwl sram_blwl_1636_ (sram_blwl_out[1636], sram_blwl_out[1636], sram_blwl_outb[1636], sram_blwl_1636_configbus0[1636:1636], sram_blwl_1636_configbus1[1636:1636] , sram_blwl_1636_configbus0_b[1636:1636] );
wire [1637:1637] sram_blwl_1637_configbus0;
wire [1637:1637] sram_blwl_1637_configbus1;
wire [1637:1637] sram_blwl_1637_configbus0_b;
assign sram_blwl_1637_configbus0[1637:1637] = sram_blwl_bl[1637:1637] ;
assign sram_blwl_1637_configbus1[1637:1637] = sram_blwl_wl[1637:1637] ;
assign sram_blwl_1637_configbus0_b[1637:1637] = sram_blwl_blb[1637:1637] ;
sram6T_blwl sram_blwl_1637_ (sram_blwl_out[1637], sram_blwl_out[1637], sram_blwl_outb[1637], sram_blwl_1637_configbus0[1637:1637], sram_blwl_1637_configbus1[1637:1637] , sram_blwl_1637_configbus0_b[1637:1637] );
wire [1638:1638] sram_blwl_1638_configbus0;
wire [1638:1638] sram_blwl_1638_configbus1;
wire [1638:1638] sram_blwl_1638_configbus0_b;
assign sram_blwl_1638_configbus0[1638:1638] = sram_blwl_bl[1638:1638] ;
assign sram_blwl_1638_configbus1[1638:1638] = sram_blwl_wl[1638:1638] ;
assign sram_blwl_1638_configbus0_b[1638:1638] = sram_blwl_blb[1638:1638] ;
sram6T_blwl sram_blwl_1638_ (sram_blwl_out[1638], sram_blwl_out[1638], sram_blwl_outb[1638], sram_blwl_1638_configbus0[1638:1638], sram_blwl_1638_configbus1[1638:1638] , sram_blwl_1638_configbus0_b[1638:1638] );
wire [1639:1639] sram_blwl_1639_configbus0;
wire [1639:1639] sram_blwl_1639_configbus1;
wire [1639:1639] sram_blwl_1639_configbus0_b;
assign sram_blwl_1639_configbus0[1639:1639] = sram_blwl_bl[1639:1639] ;
assign sram_blwl_1639_configbus1[1639:1639] = sram_blwl_wl[1639:1639] ;
assign sram_blwl_1639_configbus0_b[1639:1639] = sram_blwl_blb[1639:1639] ;
sram6T_blwl sram_blwl_1639_ (sram_blwl_out[1639], sram_blwl_out[1639], sram_blwl_outb[1639], sram_blwl_1639_configbus0[1639:1639], sram_blwl_1639_configbus1[1639:1639] , sram_blwl_1639_configbus0_b[1639:1639] );
wire [1640:1640] sram_blwl_1640_configbus0;
wire [1640:1640] sram_blwl_1640_configbus1;
wire [1640:1640] sram_blwl_1640_configbus0_b;
assign sram_blwl_1640_configbus0[1640:1640] = sram_blwl_bl[1640:1640] ;
assign sram_blwl_1640_configbus1[1640:1640] = sram_blwl_wl[1640:1640] ;
assign sram_blwl_1640_configbus0_b[1640:1640] = sram_blwl_blb[1640:1640] ;
sram6T_blwl sram_blwl_1640_ (sram_blwl_out[1640], sram_blwl_out[1640], sram_blwl_outb[1640], sram_blwl_1640_configbus0[1640:1640], sram_blwl_1640_configbus1[1640:1640] , sram_blwl_1640_configbus0_b[1640:1640] );
wire [1641:1641] sram_blwl_1641_configbus0;
wire [1641:1641] sram_blwl_1641_configbus1;
wire [1641:1641] sram_blwl_1641_configbus0_b;
assign sram_blwl_1641_configbus0[1641:1641] = sram_blwl_bl[1641:1641] ;
assign sram_blwl_1641_configbus1[1641:1641] = sram_blwl_wl[1641:1641] ;
assign sram_blwl_1641_configbus0_b[1641:1641] = sram_blwl_blb[1641:1641] ;
sram6T_blwl sram_blwl_1641_ (sram_blwl_out[1641], sram_blwl_out[1641], sram_blwl_outb[1641], sram_blwl_1641_configbus0[1641:1641], sram_blwl_1641_configbus1[1641:1641] , sram_blwl_1641_configbus0_b[1641:1641] );
wire [1642:1642] sram_blwl_1642_configbus0;
wire [1642:1642] sram_blwl_1642_configbus1;
wire [1642:1642] sram_blwl_1642_configbus0_b;
assign sram_blwl_1642_configbus0[1642:1642] = sram_blwl_bl[1642:1642] ;
assign sram_blwl_1642_configbus1[1642:1642] = sram_blwl_wl[1642:1642] ;
assign sram_blwl_1642_configbus0_b[1642:1642] = sram_blwl_blb[1642:1642] ;
sram6T_blwl sram_blwl_1642_ (sram_blwl_out[1642], sram_blwl_out[1642], sram_blwl_outb[1642], sram_blwl_1642_configbus0[1642:1642], sram_blwl_1642_configbus1[1642:1642] , sram_blwl_1642_configbus0_b[1642:1642] );
wire [1643:1643] sram_blwl_1643_configbus0;
wire [1643:1643] sram_blwl_1643_configbus1;
wire [1643:1643] sram_blwl_1643_configbus0_b;
assign sram_blwl_1643_configbus0[1643:1643] = sram_blwl_bl[1643:1643] ;
assign sram_blwl_1643_configbus1[1643:1643] = sram_blwl_wl[1643:1643] ;
assign sram_blwl_1643_configbus0_b[1643:1643] = sram_blwl_blb[1643:1643] ;
sram6T_blwl sram_blwl_1643_ (sram_blwl_out[1643], sram_blwl_out[1643], sram_blwl_outb[1643], sram_blwl_1643_configbus0[1643:1643], sram_blwl_1643_configbus1[1643:1643] , sram_blwl_1643_configbus0_b[1643:1643] );
wire [1644:1644] sram_blwl_1644_configbus0;
wire [1644:1644] sram_blwl_1644_configbus1;
wire [1644:1644] sram_blwl_1644_configbus0_b;
assign sram_blwl_1644_configbus0[1644:1644] = sram_blwl_bl[1644:1644] ;
assign sram_blwl_1644_configbus1[1644:1644] = sram_blwl_wl[1644:1644] ;
assign sram_blwl_1644_configbus0_b[1644:1644] = sram_blwl_blb[1644:1644] ;
sram6T_blwl sram_blwl_1644_ (sram_blwl_out[1644], sram_blwl_out[1644], sram_blwl_outb[1644], sram_blwl_1644_configbus0[1644:1644], sram_blwl_1644_configbus1[1644:1644] , sram_blwl_1644_configbus0_b[1644:1644] );
wire [1645:1645] sram_blwl_1645_configbus0;
wire [1645:1645] sram_blwl_1645_configbus1;
wire [1645:1645] sram_blwl_1645_configbus0_b;
assign sram_blwl_1645_configbus0[1645:1645] = sram_blwl_bl[1645:1645] ;
assign sram_blwl_1645_configbus1[1645:1645] = sram_blwl_wl[1645:1645] ;
assign sram_blwl_1645_configbus0_b[1645:1645] = sram_blwl_blb[1645:1645] ;
sram6T_blwl sram_blwl_1645_ (sram_blwl_out[1645], sram_blwl_out[1645], sram_blwl_outb[1645], sram_blwl_1645_configbus0[1645:1645], sram_blwl_1645_configbus1[1645:1645] , sram_blwl_1645_configbus0_b[1645:1645] );
wire [1646:1646] sram_blwl_1646_configbus0;
wire [1646:1646] sram_blwl_1646_configbus1;
wire [1646:1646] sram_blwl_1646_configbus0_b;
assign sram_blwl_1646_configbus0[1646:1646] = sram_blwl_bl[1646:1646] ;
assign sram_blwl_1646_configbus1[1646:1646] = sram_blwl_wl[1646:1646] ;
assign sram_blwl_1646_configbus0_b[1646:1646] = sram_blwl_blb[1646:1646] ;
sram6T_blwl sram_blwl_1646_ (sram_blwl_out[1646], sram_blwl_out[1646], sram_blwl_outb[1646], sram_blwl_1646_configbus0[1646:1646], sram_blwl_1646_configbus1[1646:1646] , sram_blwl_1646_configbus0_b[1646:1646] );
wire [1647:1647] sram_blwl_1647_configbus0;
wire [1647:1647] sram_blwl_1647_configbus1;
wire [1647:1647] sram_blwl_1647_configbus0_b;
assign sram_blwl_1647_configbus0[1647:1647] = sram_blwl_bl[1647:1647] ;
assign sram_blwl_1647_configbus1[1647:1647] = sram_blwl_wl[1647:1647] ;
assign sram_blwl_1647_configbus0_b[1647:1647] = sram_blwl_blb[1647:1647] ;
sram6T_blwl sram_blwl_1647_ (sram_blwl_out[1647], sram_blwl_out[1647], sram_blwl_outb[1647], sram_blwl_1647_configbus0[1647:1647], sram_blwl_1647_configbus1[1647:1647] , sram_blwl_1647_configbus0_b[1647:1647] );
wire [1648:1648] sram_blwl_1648_configbus0;
wire [1648:1648] sram_blwl_1648_configbus1;
wire [1648:1648] sram_blwl_1648_configbus0_b;
assign sram_blwl_1648_configbus0[1648:1648] = sram_blwl_bl[1648:1648] ;
assign sram_blwl_1648_configbus1[1648:1648] = sram_blwl_wl[1648:1648] ;
assign sram_blwl_1648_configbus0_b[1648:1648] = sram_blwl_blb[1648:1648] ;
sram6T_blwl sram_blwl_1648_ (sram_blwl_out[1648], sram_blwl_out[1648], sram_blwl_outb[1648], sram_blwl_1648_configbus0[1648:1648], sram_blwl_1648_configbus1[1648:1648] , sram_blwl_1648_configbus0_b[1648:1648] );
wire [1649:1649] sram_blwl_1649_configbus0;
wire [1649:1649] sram_blwl_1649_configbus1;
wire [1649:1649] sram_blwl_1649_configbus0_b;
assign sram_blwl_1649_configbus0[1649:1649] = sram_blwl_bl[1649:1649] ;
assign sram_blwl_1649_configbus1[1649:1649] = sram_blwl_wl[1649:1649] ;
assign sram_blwl_1649_configbus0_b[1649:1649] = sram_blwl_blb[1649:1649] ;
sram6T_blwl sram_blwl_1649_ (sram_blwl_out[1649], sram_blwl_out[1649], sram_blwl_outb[1649], sram_blwl_1649_configbus0[1649:1649], sram_blwl_1649_configbus1[1649:1649] , sram_blwl_1649_configbus0_b[1649:1649] );
wire [1650:1650] sram_blwl_1650_configbus0;
wire [1650:1650] sram_blwl_1650_configbus1;
wire [1650:1650] sram_blwl_1650_configbus0_b;
assign sram_blwl_1650_configbus0[1650:1650] = sram_blwl_bl[1650:1650] ;
assign sram_blwl_1650_configbus1[1650:1650] = sram_blwl_wl[1650:1650] ;
assign sram_blwl_1650_configbus0_b[1650:1650] = sram_blwl_blb[1650:1650] ;
sram6T_blwl sram_blwl_1650_ (sram_blwl_out[1650], sram_blwl_out[1650], sram_blwl_outb[1650], sram_blwl_1650_configbus0[1650:1650], sram_blwl_1650_configbus1[1650:1650] , sram_blwl_1650_configbus0_b[1650:1650] );
wire [1651:1651] sram_blwl_1651_configbus0;
wire [1651:1651] sram_blwl_1651_configbus1;
wire [1651:1651] sram_blwl_1651_configbus0_b;
assign sram_blwl_1651_configbus0[1651:1651] = sram_blwl_bl[1651:1651] ;
assign sram_blwl_1651_configbus1[1651:1651] = sram_blwl_wl[1651:1651] ;
assign sram_blwl_1651_configbus0_b[1651:1651] = sram_blwl_blb[1651:1651] ;
sram6T_blwl sram_blwl_1651_ (sram_blwl_out[1651], sram_blwl_out[1651], sram_blwl_outb[1651], sram_blwl_1651_configbus0[1651:1651], sram_blwl_1651_configbus1[1651:1651] , sram_blwl_1651_configbus0_b[1651:1651] );
wire [1652:1652] sram_blwl_1652_configbus0;
wire [1652:1652] sram_blwl_1652_configbus1;
wire [1652:1652] sram_blwl_1652_configbus0_b;
assign sram_blwl_1652_configbus0[1652:1652] = sram_blwl_bl[1652:1652] ;
assign sram_blwl_1652_configbus1[1652:1652] = sram_blwl_wl[1652:1652] ;
assign sram_blwl_1652_configbus0_b[1652:1652] = sram_blwl_blb[1652:1652] ;
sram6T_blwl sram_blwl_1652_ (sram_blwl_out[1652], sram_blwl_out[1652], sram_blwl_outb[1652], sram_blwl_1652_configbus0[1652:1652], sram_blwl_1652_configbus1[1652:1652] , sram_blwl_1652_configbus0_b[1652:1652] );
wire [1653:1653] sram_blwl_1653_configbus0;
wire [1653:1653] sram_blwl_1653_configbus1;
wire [1653:1653] sram_blwl_1653_configbus0_b;
assign sram_blwl_1653_configbus0[1653:1653] = sram_blwl_bl[1653:1653] ;
assign sram_blwl_1653_configbus1[1653:1653] = sram_blwl_wl[1653:1653] ;
assign sram_blwl_1653_configbus0_b[1653:1653] = sram_blwl_blb[1653:1653] ;
sram6T_blwl sram_blwl_1653_ (sram_blwl_out[1653], sram_blwl_out[1653], sram_blwl_outb[1653], sram_blwl_1653_configbus0[1653:1653], sram_blwl_1653_configbus1[1653:1653] , sram_blwl_1653_configbus0_b[1653:1653] );
wire [1654:1654] sram_blwl_1654_configbus0;
wire [1654:1654] sram_blwl_1654_configbus1;
wire [1654:1654] sram_blwl_1654_configbus0_b;
assign sram_blwl_1654_configbus0[1654:1654] = sram_blwl_bl[1654:1654] ;
assign sram_blwl_1654_configbus1[1654:1654] = sram_blwl_wl[1654:1654] ;
assign sram_blwl_1654_configbus0_b[1654:1654] = sram_blwl_blb[1654:1654] ;
sram6T_blwl sram_blwl_1654_ (sram_blwl_out[1654], sram_blwl_out[1654], sram_blwl_outb[1654], sram_blwl_1654_configbus0[1654:1654], sram_blwl_1654_configbus1[1654:1654] , sram_blwl_1654_configbus0_b[1654:1654] );
wire [1655:1655] sram_blwl_1655_configbus0;
wire [1655:1655] sram_blwl_1655_configbus1;
wire [1655:1655] sram_blwl_1655_configbus0_b;
assign sram_blwl_1655_configbus0[1655:1655] = sram_blwl_bl[1655:1655] ;
assign sram_blwl_1655_configbus1[1655:1655] = sram_blwl_wl[1655:1655] ;
assign sram_blwl_1655_configbus0_b[1655:1655] = sram_blwl_blb[1655:1655] ;
sram6T_blwl sram_blwl_1655_ (sram_blwl_out[1655], sram_blwl_out[1655], sram_blwl_outb[1655], sram_blwl_1655_configbus0[1655:1655], sram_blwl_1655_configbus1[1655:1655] , sram_blwl_1655_configbus0_b[1655:1655] );
wire [1656:1656] sram_blwl_1656_configbus0;
wire [1656:1656] sram_blwl_1656_configbus1;
wire [1656:1656] sram_blwl_1656_configbus0_b;
assign sram_blwl_1656_configbus0[1656:1656] = sram_blwl_bl[1656:1656] ;
assign sram_blwl_1656_configbus1[1656:1656] = sram_blwl_wl[1656:1656] ;
assign sram_blwl_1656_configbus0_b[1656:1656] = sram_blwl_blb[1656:1656] ;
sram6T_blwl sram_blwl_1656_ (sram_blwl_out[1656], sram_blwl_out[1656], sram_blwl_outb[1656], sram_blwl_1656_configbus0[1656:1656], sram_blwl_1656_configbus1[1656:1656] , sram_blwl_1656_configbus0_b[1656:1656] );
wire [1657:1657] sram_blwl_1657_configbus0;
wire [1657:1657] sram_blwl_1657_configbus1;
wire [1657:1657] sram_blwl_1657_configbus0_b;
assign sram_blwl_1657_configbus0[1657:1657] = sram_blwl_bl[1657:1657] ;
assign sram_blwl_1657_configbus1[1657:1657] = sram_blwl_wl[1657:1657] ;
assign sram_blwl_1657_configbus0_b[1657:1657] = sram_blwl_blb[1657:1657] ;
sram6T_blwl sram_blwl_1657_ (sram_blwl_out[1657], sram_blwl_out[1657], sram_blwl_outb[1657], sram_blwl_1657_configbus0[1657:1657], sram_blwl_1657_configbus1[1657:1657] , sram_blwl_1657_configbus0_b[1657:1657] );
wire [1658:1658] sram_blwl_1658_configbus0;
wire [1658:1658] sram_blwl_1658_configbus1;
wire [1658:1658] sram_blwl_1658_configbus0_b;
assign sram_blwl_1658_configbus0[1658:1658] = sram_blwl_bl[1658:1658] ;
assign sram_blwl_1658_configbus1[1658:1658] = sram_blwl_wl[1658:1658] ;
assign sram_blwl_1658_configbus0_b[1658:1658] = sram_blwl_blb[1658:1658] ;
sram6T_blwl sram_blwl_1658_ (sram_blwl_out[1658], sram_blwl_out[1658], sram_blwl_outb[1658], sram_blwl_1658_configbus0[1658:1658], sram_blwl_1658_configbus1[1658:1658] , sram_blwl_1658_configbus0_b[1658:1658] );
wire [1659:1659] sram_blwl_1659_configbus0;
wire [1659:1659] sram_blwl_1659_configbus1;
wire [1659:1659] sram_blwl_1659_configbus0_b;
assign sram_blwl_1659_configbus0[1659:1659] = sram_blwl_bl[1659:1659] ;
assign sram_blwl_1659_configbus1[1659:1659] = sram_blwl_wl[1659:1659] ;
assign sram_blwl_1659_configbus0_b[1659:1659] = sram_blwl_blb[1659:1659] ;
sram6T_blwl sram_blwl_1659_ (sram_blwl_out[1659], sram_blwl_out[1659], sram_blwl_outb[1659], sram_blwl_1659_configbus0[1659:1659], sram_blwl_1659_configbus1[1659:1659] , sram_blwl_1659_configbus0_b[1659:1659] );
wire [1660:1660] sram_blwl_1660_configbus0;
wire [1660:1660] sram_blwl_1660_configbus1;
wire [1660:1660] sram_blwl_1660_configbus0_b;
assign sram_blwl_1660_configbus0[1660:1660] = sram_blwl_bl[1660:1660] ;
assign sram_blwl_1660_configbus1[1660:1660] = sram_blwl_wl[1660:1660] ;
assign sram_blwl_1660_configbus0_b[1660:1660] = sram_blwl_blb[1660:1660] ;
sram6T_blwl sram_blwl_1660_ (sram_blwl_out[1660], sram_blwl_out[1660], sram_blwl_outb[1660], sram_blwl_1660_configbus0[1660:1660], sram_blwl_1660_configbus1[1660:1660] , sram_blwl_1660_configbus0_b[1660:1660] );
wire [1661:1661] sram_blwl_1661_configbus0;
wire [1661:1661] sram_blwl_1661_configbus1;
wire [1661:1661] sram_blwl_1661_configbus0_b;
assign sram_blwl_1661_configbus0[1661:1661] = sram_blwl_bl[1661:1661] ;
assign sram_blwl_1661_configbus1[1661:1661] = sram_blwl_wl[1661:1661] ;
assign sram_blwl_1661_configbus0_b[1661:1661] = sram_blwl_blb[1661:1661] ;
sram6T_blwl sram_blwl_1661_ (sram_blwl_out[1661], sram_blwl_out[1661], sram_blwl_outb[1661], sram_blwl_1661_configbus0[1661:1661], sram_blwl_1661_configbus1[1661:1661] , sram_blwl_1661_configbus0_b[1661:1661] );
wire [1662:1662] sram_blwl_1662_configbus0;
wire [1662:1662] sram_blwl_1662_configbus1;
wire [1662:1662] sram_blwl_1662_configbus0_b;
assign sram_blwl_1662_configbus0[1662:1662] = sram_blwl_bl[1662:1662] ;
assign sram_blwl_1662_configbus1[1662:1662] = sram_blwl_wl[1662:1662] ;
assign sram_blwl_1662_configbus0_b[1662:1662] = sram_blwl_blb[1662:1662] ;
sram6T_blwl sram_blwl_1662_ (sram_blwl_out[1662], sram_blwl_out[1662], sram_blwl_outb[1662], sram_blwl_1662_configbus0[1662:1662], sram_blwl_1662_configbus1[1662:1662] , sram_blwl_1662_configbus0_b[1662:1662] );
wire [1663:1663] sram_blwl_1663_configbus0;
wire [1663:1663] sram_blwl_1663_configbus1;
wire [1663:1663] sram_blwl_1663_configbus0_b;
assign sram_blwl_1663_configbus0[1663:1663] = sram_blwl_bl[1663:1663] ;
assign sram_blwl_1663_configbus1[1663:1663] = sram_blwl_wl[1663:1663] ;
assign sram_blwl_1663_configbus0_b[1663:1663] = sram_blwl_blb[1663:1663] ;
sram6T_blwl sram_blwl_1663_ (sram_blwl_out[1663], sram_blwl_out[1663], sram_blwl_outb[1663], sram_blwl_1663_configbus0[1663:1663], sram_blwl_1663_configbus1[1663:1663] , sram_blwl_1663_configbus0_b[1663:1663] );
wire [1664:1664] sram_blwl_1664_configbus0;
wire [1664:1664] sram_blwl_1664_configbus1;
wire [1664:1664] sram_blwl_1664_configbus0_b;
assign sram_blwl_1664_configbus0[1664:1664] = sram_blwl_bl[1664:1664] ;
assign sram_blwl_1664_configbus1[1664:1664] = sram_blwl_wl[1664:1664] ;
assign sram_blwl_1664_configbus0_b[1664:1664] = sram_blwl_blb[1664:1664] ;
sram6T_blwl sram_blwl_1664_ (sram_blwl_out[1664], sram_blwl_out[1664], sram_blwl_outb[1664], sram_blwl_1664_configbus0[1664:1664], sram_blwl_1664_configbus1[1664:1664] , sram_blwl_1664_configbus0_b[1664:1664] );
endmodule
//----- END LUT Verilog module: grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ -----

//----- Flip-flop Verilog module: Q0 -----
//----- Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----
module grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
input [0:0] Set,
input [0:0] Reset,
input [0:0] clk
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
input wire ff_0___D_0_,
output wire ff_0___Q_0_);
static_dff dff_9_ (//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_);
endmodule
//----- END Flip-flop Verilog module: grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ -----

//----- Programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6_ -----
module grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_ble6___in_0_,
input wire mode_ble6___in_1_,
input wire mode_ble6___in_2_,
input wire mode_ble6___in_3_,
input wire mode_ble6___in_4_,
input wire mode_ble6___in_5_,
output wire mode_ble6___out_0_,
input wire mode_ble6___clk_0_,
input [1601:1665] sram_blwl_bl ,
input [1601:1665] sram_blwl_wl ,
input [1601:1665] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6__lut6_0_ lut6_0_ (
 lut6_0___in_0_,  lut6_0___in_1_,  lut6_0___in_2_,  lut6_0___in_3_,  lut6_0___in_4_,  lut6_0___in_5_,  lut6_0___out_0_,
sram_blwl_bl[1601:1664] ,
sram_blwl_wl[1601:1664] ,
sram_blwl_blb[1601:1664] ); 
grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6__ff_0_ ff_0_ (
//----- BEGIN Global ports of SPICE_MODEL(static_dff) -----
Set[0:0],
Reset[0:0],
clk[0:0]
//----- END Global ports of SPICE_MODEL(static_dff)-----
,
 ff_0___D_0_,  ff_0___Q_0_); 
wire [0:1] in_bus_mux_1level_tapbuf_size2_409_ ;
assign in_bus_mux_1level_tapbuf_size2_409_[0] = ff_0___Q_0_ ; 
assign in_bus_mux_1level_tapbuf_size2_409_[1] = lut6_0___out_0_ ; 
wire [1665:1665] mux_1level_tapbuf_size2_409_configbus0;
wire [1665:1665] mux_1level_tapbuf_size2_409_configbus1;
wire [1665:1665] mux_1level_tapbuf_size2_409_sram_blwl_out ;
wire [1665:1665] mux_1level_tapbuf_size2_409_sram_blwl_outb ;
assign mux_1level_tapbuf_size2_409_configbus0[1665:1665] = sram_blwl_bl[1665:1665] ;
assign mux_1level_tapbuf_size2_409_configbus1[1665:1665] = sram_blwl_wl[1665:1665] ;
wire [1665:1665] mux_1level_tapbuf_size2_409_configbus0_b;
assign mux_1level_tapbuf_size2_409_configbus0_b[1665:1665] = sram_blwl_blb[1665:1665] ;
mux_1level_tapbuf_size2 mux_1level_tapbuf_size2_409_ (in_bus_mux_1level_tapbuf_size2_409_, mode_ble6___out_0_, mux_1level_tapbuf_size2_409_sram_blwl_out[1665:1665] ,
mux_1level_tapbuf_size2_409_sram_blwl_outb[1665:1665] );
//----- SRAM bits for MUX[409], level=1, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1-----
sram6T_blwl sram_blwl_1665_ (mux_1level_tapbuf_size2_409_sram_blwl_out[1665:1665] ,mux_1level_tapbuf_size2_409_sram_blwl_out[1665:1665] ,mux_1level_tapbuf_size2_409_sram_blwl_outb[1665:1665] ,mux_1level_tapbuf_size2_409_configbus0[1665:1665], mux_1level_tapbuf_size2_409_configbus1[1665:1665] , mux_1level_tapbuf_size2_409_configbus0_b[1665:1665] );
direct_interc direct_interc_144_ (mode_ble6___in_0_, lut6_0___in_0_ );
direct_interc direct_interc_145_ (mode_ble6___in_1_, lut6_0___in_1_ );
direct_interc direct_interc_146_ (mode_ble6___in_2_, lut6_0___in_2_ );
direct_interc direct_interc_147_ (mode_ble6___in_3_, lut6_0___in_3_ );
direct_interc direct_interc_148_ (mode_ble6___in_4_, lut6_0___in_4_ );
direct_interc direct_interc_149_ (mode_ble6___in_5_, lut6_0___in_5_ );
direct_interc direct_interc_150_ (lut6_0___out_0_, ff_0___D_0_ );
direct_interc direct_interc_151_ (mode_ble6___clk_0_, ff_0___clk_0_ );
endmodule
//----- END Programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6_ -----

//----- Programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6_ -----
module grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6_ (
//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
input wire mode_n1_lut6___in_0_,
input wire mode_n1_lut6___in_1_,
input wire mode_n1_lut6___in_2_,
input wire mode_n1_lut6___in_3_,
input wire mode_n1_lut6___in_4_,
input wire mode_n1_lut6___in_5_,
output wire mode_n1_lut6___out_0_,
input wire mode_n1_lut6___clk_0_,
input [1601:1665] sram_blwl_bl ,
input [1601:1665] sram_blwl_wl ,
input [1601:1665] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6__ble6_0__mode_ble6_ ble6_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 ble6_0___in_0_,  ble6_0___in_1_,  ble6_0___in_2_,  ble6_0___in_3_,  ble6_0___in_4_,  ble6_0___in_5_,  ble6_0___out_0_,  ble6_0___clk_0_,
sram_blwl_bl[1601:1665] ,
sram_blwl_wl[1601:1665] ,
sram_blwl_blb[1601:1665] ); 
direct_interc direct_interc_152_ (ble6_0___out_0_, mode_n1_lut6___out_0_ );
direct_interc direct_interc_153_ (mode_n1_lut6___in_0_, ble6_0___in_0_ );
direct_interc direct_interc_154_ (mode_n1_lut6___in_1_, ble6_0___in_1_ );
direct_interc direct_interc_155_ (mode_n1_lut6___in_2_, ble6_0___in_2_ );
direct_interc direct_interc_156_ (mode_n1_lut6___in_3_, ble6_0___in_3_ );
direct_interc direct_interc_157_ (mode_n1_lut6___in_4_, ble6_0___in_4_ );
direct_interc direct_interc_158_ (mode_n1_lut6___in_5_, ble6_0___in_5_ );
direct_interc direct_interc_159_ (mode_n1_lut6___clk_0_, ble6_0___clk_0_ );
endmodule
//----- END Programmable logic block Verilog module grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6_ -----

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
input wire mode_clb___I_4_,
input wire mode_clb___I_5_,
input wire mode_clb___I_6_,
input wire mode_clb___I_7_,
input wire mode_clb___I_8_,
input wire mode_clb___I_9_,
input wire mode_clb___I_10_,
input wire mode_clb___I_11_,
input wire mode_clb___I_12_,
input wire mode_clb___I_13_,
input wire mode_clb___I_14_,
input wire mode_clb___I_15_,
input wire mode_clb___I_16_,
input wire mode_clb___I_17_,
input wire mode_clb___I_18_,
input wire mode_clb___I_19_,
input wire mode_clb___I_20_,
input wire mode_clb___I_21_,
input wire mode_clb___I_22_,
input wire mode_clb___I_23_,
input wire mode_clb___I_24_,
input wire mode_clb___I_25_,
input wire mode_clb___I_26_,
input wire mode_clb___I_27_,
input wire mode_clb___I_28_,
input wire mode_clb___I_29_,
input wire mode_clb___I_30_,
input wire mode_clb___I_31_,
input wire mode_clb___I_32_,
input wire mode_clb___I_33_,
input wire mode_clb___I_34_,
input wire mode_clb___I_35_,
input wire mode_clb___I_36_,
input wire mode_clb___I_37_,
input wire mode_clb___I_38_,
input wire mode_clb___I_39_,
output wire mode_clb___O_0_,
output wire mode_clb___O_1_,
output wire mode_clb___O_2_,
output wire mode_clb___O_3_,
output wire mode_clb___O_4_,
output wire mode_clb___O_5_,
output wire mode_clb___O_6_,
output wire mode_clb___O_7_,
output wire mode_clb___O_8_,
output wire mode_clb___O_9_,
input wire mode_clb___clk_0_,
input [1016:2625] sram_blwl_bl ,
input [1016:2625] sram_blwl_wl ,
input [1016:2625] sram_blwl_blb );
grid_1__1__clb_0__mode_clb__fle_0__mode_n1_lut6_ fle_0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 fle_0___in_0_,  fle_0___in_1_,  fle_0___in_2_,  fle_0___in_3_,  fle_0___in_4_,  fle_0___in_5_,  fle_0___out_0_,  fle_0___clk_0_,
sram_blwl_bl[1016:1080] ,
sram_blwl_wl[1016:1080] ,
sram_blwl_blb[1016:1080] ); 
grid_1__1__clb_0__mode_clb__fle_1__mode_n1_lut6_ fle_1_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 fle_1___in_0_,  fle_1___in_1_,  fle_1___in_2_,  fle_1___in_3_,  fle_1___in_4_,  fle_1___in_5_,  fle_1___out_0_,  fle_1___clk_0_,
sram_blwl_bl[1081:1145] ,
sram_blwl_wl[1081:1145] ,
sram_blwl_blb[1081:1145] ); 
grid_1__1__clb_0__mode_clb__fle_2__mode_n1_lut6_ fle_2_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 fle_2___in_0_,  fle_2___in_1_,  fle_2___in_2_,  fle_2___in_3_,  fle_2___in_4_,  fle_2___in_5_,  fle_2___out_0_,  fle_2___clk_0_,
sram_blwl_bl[1146:1210] ,
sram_blwl_wl[1146:1210] ,
sram_blwl_blb[1146:1210] ); 
grid_1__1__clb_0__mode_clb__fle_3__mode_n1_lut6_ fle_3_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 fle_3___in_0_,  fle_3___in_1_,  fle_3___in_2_,  fle_3___in_3_,  fle_3___in_4_,  fle_3___in_5_,  fle_3___out_0_,  fle_3___clk_0_,
sram_blwl_bl[1211:1275] ,
sram_blwl_wl[1211:1275] ,
sram_blwl_blb[1211:1275] ); 
grid_1__1__clb_0__mode_clb__fle_4__mode_n1_lut6_ fle_4_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 fle_4___in_0_,  fle_4___in_1_,  fle_4___in_2_,  fle_4___in_3_,  fle_4___in_4_,  fle_4___in_5_,  fle_4___out_0_,  fle_4___clk_0_,
sram_blwl_bl[1276:1340] ,
sram_blwl_wl[1276:1340] ,
sram_blwl_blb[1276:1340] ); 
grid_1__1__clb_0__mode_clb__fle_5__mode_n1_lut6_ fle_5_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 fle_5___in_0_,  fle_5___in_1_,  fle_5___in_2_,  fle_5___in_3_,  fle_5___in_4_,  fle_5___in_5_,  fle_5___out_0_,  fle_5___clk_0_,
sram_blwl_bl[1341:1405] ,
sram_blwl_wl[1341:1405] ,
sram_blwl_blb[1341:1405] ); 
grid_1__1__clb_0__mode_clb__fle_6__mode_n1_lut6_ fle_6_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 fle_6___in_0_,  fle_6___in_1_,  fle_6___in_2_,  fle_6___in_3_,  fle_6___in_4_,  fle_6___in_5_,  fle_6___out_0_,  fle_6___clk_0_,
sram_blwl_bl[1406:1470] ,
sram_blwl_wl[1406:1470] ,
sram_blwl_blb[1406:1470] ); 
grid_1__1__clb_0__mode_clb__fle_7__mode_n1_lut6_ fle_7_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 fle_7___in_0_,  fle_7___in_1_,  fle_7___in_2_,  fle_7___in_3_,  fle_7___in_4_,  fle_7___in_5_,  fle_7___out_0_,  fle_7___clk_0_,
sram_blwl_bl[1471:1535] ,
sram_blwl_wl[1471:1535] ,
sram_blwl_blb[1471:1535] ); 
grid_1__1__clb_0__mode_clb__fle_8__mode_n1_lut6_ fle_8_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 fle_8___in_0_,  fle_8___in_1_,  fle_8___in_2_,  fle_8___in_3_,  fle_8___in_4_,  fle_8___in_5_,  fle_8___out_0_,  fle_8___clk_0_,
sram_blwl_bl[1536:1600] ,
sram_blwl_wl[1536:1600] ,
sram_blwl_blb[1536:1600] ); 
grid_1__1__clb_0__mode_clb__fle_9__mode_n1_lut6_ fle_9_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 fle_9___in_0_,  fle_9___in_1_,  fle_9___in_2_,  fle_9___in_3_,  fle_9___in_4_,  fle_9___in_5_,  fle_9___out_0_,  fle_9___clk_0_,
sram_blwl_bl[1601:1665] ,
sram_blwl_wl[1601:1665] ,
sram_blwl_blb[1601:1665] ); 
direct_interc direct_interc_160_ (fle_0___out_0_, mode_clb___O_0_ );
direct_interc direct_interc_161_ (fle_1___out_0_, mode_clb___O_1_ );
direct_interc direct_interc_162_ (fle_2___out_0_, mode_clb___O_2_ );
direct_interc direct_interc_163_ (fle_3___out_0_, mode_clb___O_3_ );
direct_interc direct_interc_164_ (fle_4___out_0_, mode_clb___O_4_ );
direct_interc direct_interc_165_ (fle_5___out_0_, mode_clb___O_5_ );
direct_interc direct_interc_166_ (fle_6___out_0_, mode_clb___O_6_ );
direct_interc direct_interc_167_ (fle_7___out_0_, mode_clb___O_7_ );
direct_interc direct_interc_168_ (fle_8___out_0_, mode_clb___O_8_ );
direct_interc direct_interc_169_ (fle_9___out_0_, mode_clb___O_9_ );
wire [0:49] in_bus_mux_2level_size50_0_ ;
assign in_bus_mux_2level_size50_0_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_0_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_0_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_0_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_0_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_0_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_0_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_0_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_0_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_0_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_0_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_0_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_0_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_0_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_0_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_0_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_0_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_0_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_0_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_0_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_0_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_0_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_0_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_0_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_0_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_0_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_0_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_0_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_0_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_0_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_0_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_0_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_0_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_0_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_0_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_0_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_0_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_0_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_0_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_0_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_0_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_0_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_0_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_0_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_0_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_0_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_0_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_0_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_0_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_0_[49] = fle_9___out_0_ ; 
wire [1666:1681] mux_2level_size50_0_configbus0;
wire [1666:1681] mux_2level_size50_0_configbus1;
wire [1666:1681] mux_2level_size50_0_sram_blwl_out ;
wire [1666:1681] mux_2level_size50_0_sram_blwl_outb ;
assign mux_2level_size50_0_configbus0[1666:1681] = sram_blwl_bl[1666:1681] ;
assign mux_2level_size50_0_configbus1[1666:1681] = sram_blwl_wl[1666:1681] ;
wire [1666:1681] mux_2level_size50_0_configbus0_b;
assign mux_2level_size50_0_configbus0_b[1666:1681] = sram_blwl_blb[1666:1681] ;
mux_2level_size50 mux_2level_size50_0_ (in_bus_mux_2level_size50_0_, fle_0___in_0_, mux_2level_size50_0_sram_blwl_out[1666:1681] ,
mux_2level_size50_0_sram_blwl_outb[1666:1681] );
//----- SRAM bits for MUX[0], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1666_ (mux_2level_size50_0_sram_blwl_out[1666:1666] ,mux_2level_size50_0_sram_blwl_out[1666:1666] ,mux_2level_size50_0_sram_blwl_outb[1666:1666] ,mux_2level_size50_0_configbus0[1666:1666], mux_2level_size50_0_configbus1[1666:1666] , mux_2level_size50_0_configbus0_b[1666:1666] );
sram6T_blwl sram_blwl_1667_ (mux_2level_size50_0_sram_blwl_out[1667:1667] ,mux_2level_size50_0_sram_blwl_out[1667:1667] ,mux_2level_size50_0_sram_blwl_outb[1667:1667] ,mux_2level_size50_0_configbus0[1667:1667], mux_2level_size50_0_configbus1[1667:1667] , mux_2level_size50_0_configbus0_b[1667:1667] );
sram6T_blwl sram_blwl_1668_ (mux_2level_size50_0_sram_blwl_out[1668:1668] ,mux_2level_size50_0_sram_blwl_out[1668:1668] ,mux_2level_size50_0_sram_blwl_outb[1668:1668] ,mux_2level_size50_0_configbus0[1668:1668], mux_2level_size50_0_configbus1[1668:1668] , mux_2level_size50_0_configbus0_b[1668:1668] );
sram6T_blwl sram_blwl_1669_ (mux_2level_size50_0_sram_blwl_out[1669:1669] ,mux_2level_size50_0_sram_blwl_out[1669:1669] ,mux_2level_size50_0_sram_blwl_outb[1669:1669] ,mux_2level_size50_0_configbus0[1669:1669], mux_2level_size50_0_configbus1[1669:1669] , mux_2level_size50_0_configbus0_b[1669:1669] );
sram6T_blwl sram_blwl_1670_ (mux_2level_size50_0_sram_blwl_out[1670:1670] ,mux_2level_size50_0_sram_blwl_out[1670:1670] ,mux_2level_size50_0_sram_blwl_outb[1670:1670] ,mux_2level_size50_0_configbus0[1670:1670], mux_2level_size50_0_configbus1[1670:1670] , mux_2level_size50_0_configbus0_b[1670:1670] );
sram6T_blwl sram_blwl_1671_ (mux_2level_size50_0_sram_blwl_out[1671:1671] ,mux_2level_size50_0_sram_blwl_out[1671:1671] ,mux_2level_size50_0_sram_blwl_outb[1671:1671] ,mux_2level_size50_0_configbus0[1671:1671], mux_2level_size50_0_configbus1[1671:1671] , mux_2level_size50_0_configbus0_b[1671:1671] );
sram6T_blwl sram_blwl_1672_ (mux_2level_size50_0_sram_blwl_out[1672:1672] ,mux_2level_size50_0_sram_blwl_out[1672:1672] ,mux_2level_size50_0_sram_blwl_outb[1672:1672] ,mux_2level_size50_0_configbus0[1672:1672], mux_2level_size50_0_configbus1[1672:1672] , mux_2level_size50_0_configbus0_b[1672:1672] );
sram6T_blwl sram_blwl_1673_ (mux_2level_size50_0_sram_blwl_out[1673:1673] ,mux_2level_size50_0_sram_blwl_out[1673:1673] ,mux_2level_size50_0_sram_blwl_outb[1673:1673] ,mux_2level_size50_0_configbus0[1673:1673], mux_2level_size50_0_configbus1[1673:1673] , mux_2level_size50_0_configbus0_b[1673:1673] );
sram6T_blwl sram_blwl_1674_ (mux_2level_size50_0_sram_blwl_out[1674:1674] ,mux_2level_size50_0_sram_blwl_out[1674:1674] ,mux_2level_size50_0_sram_blwl_outb[1674:1674] ,mux_2level_size50_0_configbus0[1674:1674], mux_2level_size50_0_configbus1[1674:1674] , mux_2level_size50_0_configbus0_b[1674:1674] );
sram6T_blwl sram_blwl_1675_ (mux_2level_size50_0_sram_blwl_out[1675:1675] ,mux_2level_size50_0_sram_blwl_out[1675:1675] ,mux_2level_size50_0_sram_blwl_outb[1675:1675] ,mux_2level_size50_0_configbus0[1675:1675], mux_2level_size50_0_configbus1[1675:1675] , mux_2level_size50_0_configbus0_b[1675:1675] );
sram6T_blwl sram_blwl_1676_ (mux_2level_size50_0_sram_blwl_out[1676:1676] ,mux_2level_size50_0_sram_blwl_out[1676:1676] ,mux_2level_size50_0_sram_blwl_outb[1676:1676] ,mux_2level_size50_0_configbus0[1676:1676], mux_2level_size50_0_configbus1[1676:1676] , mux_2level_size50_0_configbus0_b[1676:1676] );
sram6T_blwl sram_blwl_1677_ (mux_2level_size50_0_sram_blwl_out[1677:1677] ,mux_2level_size50_0_sram_blwl_out[1677:1677] ,mux_2level_size50_0_sram_blwl_outb[1677:1677] ,mux_2level_size50_0_configbus0[1677:1677], mux_2level_size50_0_configbus1[1677:1677] , mux_2level_size50_0_configbus0_b[1677:1677] );
sram6T_blwl sram_blwl_1678_ (mux_2level_size50_0_sram_blwl_out[1678:1678] ,mux_2level_size50_0_sram_blwl_out[1678:1678] ,mux_2level_size50_0_sram_blwl_outb[1678:1678] ,mux_2level_size50_0_configbus0[1678:1678], mux_2level_size50_0_configbus1[1678:1678] , mux_2level_size50_0_configbus0_b[1678:1678] );
sram6T_blwl sram_blwl_1679_ (mux_2level_size50_0_sram_blwl_out[1679:1679] ,mux_2level_size50_0_sram_blwl_out[1679:1679] ,mux_2level_size50_0_sram_blwl_outb[1679:1679] ,mux_2level_size50_0_configbus0[1679:1679], mux_2level_size50_0_configbus1[1679:1679] , mux_2level_size50_0_configbus0_b[1679:1679] );
sram6T_blwl sram_blwl_1680_ (mux_2level_size50_0_sram_blwl_out[1680:1680] ,mux_2level_size50_0_sram_blwl_out[1680:1680] ,mux_2level_size50_0_sram_blwl_outb[1680:1680] ,mux_2level_size50_0_configbus0[1680:1680], mux_2level_size50_0_configbus1[1680:1680] , mux_2level_size50_0_configbus0_b[1680:1680] );
sram6T_blwl sram_blwl_1681_ (mux_2level_size50_0_sram_blwl_out[1681:1681] ,mux_2level_size50_0_sram_blwl_out[1681:1681] ,mux_2level_size50_0_sram_blwl_outb[1681:1681] ,mux_2level_size50_0_configbus0[1681:1681], mux_2level_size50_0_configbus1[1681:1681] , mux_2level_size50_0_configbus0_b[1681:1681] );
wire [0:49] in_bus_mux_2level_size50_1_ ;
assign in_bus_mux_2level_size50_1_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_1_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_1_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_1_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_1_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_1_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_1_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_1_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_1_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_1_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_1_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_1_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_1_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_1_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_1_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_1_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_1_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_1_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_1_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_1_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_1_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_1_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_1_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_1_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_1_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_1_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_1_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_1_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_1_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_1_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_1_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_1_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_1_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_1_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_1_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_1_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_1_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_1_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_1_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_1_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_1_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_1_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_1_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_1_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_1_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_1_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_1_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_1_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_1_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_1_[49] = fle_9___out_0_ ; 
wire [1682:1697] mux_2level_size50_1_configbus0;
wire [1682:1697] mux_2level_size50_1_configbus1;
wire [1682:1697] mux_2level_size50_1_sram_blwl_out ;
wire [1682:1697] mux_2level_size50_1_sram_blwl_outb ;
assign mux_2level_size50_1_configbus0[1682:1697] = sram_blwl_bl[1682:1697] ;
assign mux_2level_size50_1_configbus1[1682:1697] = sram_blwl_wl[1682:1697] ;
wire [1682:1697] mux_2level_size50_1_configbus0_b;
assign mux_2level_size50_1_configbus0_b[1682:1697] = sram_blwl_blb[1682:1697] ;
mux_2level_size50 mux_2level_size50_1_ (in_bus_mux_2level_size50_1_, fle_0___in_1_, mux_2level_size50_1_sram_blwl_out[1682:1697] ,
mux_2level_size50_1_sram_blwl_outb[1682:1697] );
//----- SRAM bits for MUX[1], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1682_ (mux_2level_size50_1_sram_blwl_out[1682:1682] ,mux_2level_size50_1_sram_blwl_out[1682:1682] ,mux_2level_size50_1_sram_blwl_outb[1682:1682] ,mux_2level_size50_1_configbus0[1682:1682], mux_2level_size50_1_configbus1[1682:1682] , mux_2level_size50_1_configbus0_b[1682:1682] );
sram6T_blwl sram_blwl_1683_ (mux_2level_size50_1_sram_blwl_out[1683:1683] ,mux_2level_size50_1_sram_blwl_out[1683:1683] ,mux_2level_size50_1_sram_blwl_outb[1683:1683] ,mux_2level_size50_1_configbus0[1683:1683], mux_2level_size50_1_configbus1[1683:1683] , mux_2level_size50_1_configbus0_b[1683:1683] );
sram6T_blwl sram_blwl_1684_ (mux_2level_size50_1_sram_blwl_out[1684:1684] ,mux_2level_size50_1_sram_blwl_out[1684:1684] ,mux_2level_size50_1_sram_blwl_outb[1684:1684] ,mux_2level_size50_1_configbus0[1684:1684], mux_2level_size50_1_configbus1[1684:1684] , mux_2level_size50_1_configbus0_b[1684:1684] );
sram6T_blwl sram_blwl_1685_ (mux_2level_size50_1_sram_blwl_out[1685:1685] ,mux_2level_size50_1_sram_blwl_out[1685:1685] ,mux_2level_size50_1_sram_blwl_outb[1685:1685] ,mux_2level_size50_1_configbus0[1685:1685], mux_2level_size50_1_configbus1[1685:1685] , mux_2level_size50_1_configbus0_b[1685:1685] );
sram6T_blwl sram_blwl_1686_ (mux_2level_size50_1_sram_blwl_out[1686:1686] ,mux_2level_size50_1_sram_blwl_out[1686:1686] ,mux_2level_size50_1_sram_blwl_outb[1686:1686] ,mux_2level_size50_1_configbus0[1686:1686], mux_2level_size50_1_configbus1[1686:1686] , mux_2level_size50_1_configbus0_b[1686:1686] );
sram6T_blwl sram_blwl_1687_ (mux_2level_size50_1_sram_blwl_out[1687:1687] ,mux_2level_size50_1_sram_blwl_out[1687:1687] ,mux_2level_size50_1_sram_blwl_outb[1687:1687] ,mux_2level_size50_1_configbus0[1687:1687], mux_2level_size50_1_configbus1[1687:1687] , mux_2level_size50_1_configbus0_b[1687:1687] );
sram6T_blwl sram_blwl_1688_ (mux_2level_size50_1_sram_blwl_out[1688:1688] ,mux_2level_size50_1_sram_blwl_out[1688:1688] ,mux_2level_size50_1_sram_blwl_outb[1688:1688] ,mux_2level_size50_1_configbus0[1688:1688], mux_2level_size50_1_configbus1[1688:1688] , mux_2level_size50_1_configbus0_b[1688:1688] );
sram6T_blwl sram_blwl_1689_ (mux_2level_size50_1_sram_blwl_out[1689:1689] ,mux_2level_size50_1_sram_blwl_out[1689:1689] ,mux_2level_size50_1_sram_blwl_outb[1689:1689] ,mux_2level_size50_1_configbus0[1689:1689], mux_2level_size50_1_configbus1[1689:1689] , mux_2level_size50_1_configbus0_b[1689:1689] );
sram6T_blwl sram_blwl_1690_ (mux_2level_size50_1_sram_blwl_out[1690:1690] ,mux_2level_size50_1_sram_blwl_out[1690:1690] ,mux_2level_size50_1_sram_blwl_outb[1690:1690] ,mux_2level_size50_1_configbus0[1690:1690], mux_2level_size50_1_configbus1[1690:1690] , mux_2level_size50_1_configbus0_b[1690:1690] );
sram6T_blwl sram_blwl_1691_ (mux_2level_size50_1_sram_blwl_out[1691:1691] ,mux_2level_size50_1_sram_blwl_out[1691:1691] ,mux_2level_size50_1_sram_blwl_outb[1691:1691] ,mux_2level_size50_1_configbus0[1691:1691], mux_2level_size50_1_configbus1[1691:1691] , mux_2level_size50_1_configbus0_b[1691:1691] );
sram6T_blwl sram_blwl_1692_ (mux_2level_size50_1_sram_blwl_out[1692:1692] ,mux_2level_size50_1_sram_blwl_out[1692:1692] ,mux_2level_size50_1_sram_blwl_outb[1692:1692] ,mux_2level_size50_1_configbus0[1692:1692], mux_2level_size50_1_configbus1[1692:1692] , mux_2level_size50_1_configbus0_b[1692:1692] );
sram6T_blwl sram_blwl_1693_ (mux_2level_size50_1_sram_blwl_out[1693:1693] ,mux_2level_size50_1_sram_blwl_out[1693:1693] ,mux_2level_size50_1_sram_blwl_outb[1693:1693] ,mux_2level_size50_1_configbus0[1693:1693], mux_2level_size50_1_configbus1[1693:1693] , mux_2level_size50_1_configbus0_b[1693:1693] );
sram6T_blwl sram_blwl_1694_ (mux_2level_size50_1_sram_blwl_out[1694:1694] ,mux_2level_size50_1_sram_blwl_out[1694:1694] ,mux_2level_size50_1_sram_blwl_outb[1694:1694] ,mux_2level_size50_1_configbus0[1694:1694], mux_2level_size50_1_configbus1[1694:1694] , mux_2level_size50_1_configbus0_b[1694:1694] );
sram6T_blwl sram_blwl_1695_ (mux_2level_size50_1_sram_blwl_out[1695:1695] ,mux_2level_size50_1_sram_blwl_out[1695:1695] ,mux_2level_size50_1_sram_blwl_outb[1695:1695] ,mux_2level_size50_1_configbus0[1695:1695], mux_2level_size50_1_configbus1[1695:1695] , mux_2level_size50_1_configbus0_b[1695:1695] );
sram6T_blwl sram_blwl_1696_ (mux_2level_size50_1_sram_blwl_out[1696:1696] ,mux_2level_size50_1_sram_blwl_out[1696:1696] ,mux_2level_size50_1_sram_blwl_outb[1696:1696] ,mux_2level_size50_1_configbus0[1696:1696], mux_2level_size50_1_configbus1[1696:1696] , mux_2level_size50_1_configbus0_b[1696:1696] );
sram6T_blwl sram_blwl_1697_ (mux_2level_size50_1_sram_blwl_out[1697:1697] ,mux_2level_size50_1_sram_blwl_out[1697:1697] ,mux_2level_size50_1_sram_blwl_outb[1697:1697] ,mux_2level_size50_1_configbus0[1697:1697], mux_2level_size50_1_configbus1[1697:1697] , mux_2level_size50_1_configbus0_b[1697:1697] );
wire [0:49] in_bus_mux_2level_size50_2_ ;
assign in_bus_mux_2level_size50_2_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_2_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_2_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_2_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_2_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_2_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_2_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_2_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_2_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_2_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_2_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_2_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_2_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_2_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_2_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_2_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_2_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_2_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_2_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_2_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_2_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_2_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_2_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_2_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_2_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_2_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_2_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_2_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_2_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_2_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_2_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_2_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_2_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_2_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_2_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_2_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_2_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_2_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_2_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_2_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_2_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_2_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_2_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_2_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_2_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_2_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_2_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_2_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_2_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_2_[49] = fle_9___out_0_ ; 
wire [1698:1713] mux_2level_size50_2_configbus0;
wire [1698:1713] mux_2level_size50_2_configbus1;
wire [1698:1713] mux_2level_size50_2_sram_blwl_out ;
wire [1698:1713] mux_2level_size50_2_sram_blwl_outb ;
assign mux_2level_size50_2_configbus0[1698:1713] = sram_blwl_bl[1698:1713] ;
assign mux_2level_size50_2_configbus1[1698:1713] = sram_blwl_wl[1698:1713] ;
wire [1698:1713] mux_2level_size50_2_configbus0_b;
assign mux_2level_size50_2_configbus0_b[1698:1713] = sram_blwl_blb[1698:1713] ;
mux_2level_size50 mux_2level_size50_2_ (in_bus_mux_2level_size50_2_, fle_0___in_2_, mux_2level_size50_2_sram_blwl_out[1698:1713] ,
mux_2level_size50_2_sram_blwl_outb[1698:1713] );
//----- SRAM bits for MUX[2], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1698_ (mux_2level_size50_2_sram_blwl_out[1698:1698] ,mux_2level_size50_2_sram_blwl_out[1698:1698] ,mux_2level_size50_2_sram_blwl_outb[1698:1698] ,mux_2level_size50_2_configbus0[1698:1698], mux_2level_size50_2_configbus1[1698:1698] , mux_2level_size50_2_configbus0_b[1698:1698] );
sram6T_blwl sram_blwl_1699_ (mux_2level_size50_2_sram_blwl_out[1699:1699] ,mux_2level_size50_2_sram_blwl_out[1699:1699] ,mux_2level_size50_2_sram_blwl_outb[1699:1699] ,mux_2level_size50_2_configbus0[1699:1699], mux_2level_size50_2_configbus1[1699:1699] , mux_2level_size50_2_configbus0_b[1699:1699] );
sram6T_blwl sram_blwl_1700_ (mux_2level_size50_2_sram_blwl_out[1700:1700] ,mux_2level_size50_2_sram_blwl_out[1700:1700] ,mux_2level_size50_2_sram_blwl_outb[1700:1700] ,mux_2level_size50_2_configbus0[1700:1700], mux_2level_size50_2_configbus1[1700:1700] , mux_2level_size50_2_configbus0_b[1700:1700] );
sram6T_blwl sram_blwl_1701_ (mux_2level_size50_2_sram_blwl_out[1701:1701] ,mux_2level_size50_2_sram_blwl_out[1701:1701] ,mux_2level_size50_2_sram_blwl_outb[1701:1701] ,mux_2level_size50_2_configbus0[1701:1701], mux_2level_size50_2_configbus1[1701:1701] , mux_2level_size50_2_configbus0_b[1701:1701] );
sram6T_blwl sram_blwl_1702_ (mux_2level_size50_2_sram_blwl_out[1702:1702] ,mux_2level_size50_2_sram_blwl_out[1702:1702] ,mux_2level_size50_2_sram_blwl_outb[1702:1702] ,mux_2level_size50_2_configbus0[1702:1702], mux_2level_size50_2_configbus1[1702:1702] , mux_2level_size50_2_configbus0_b[1702:1702] );
sram6T_blwl sram_blwl_1703_ (mux_2level_size50_2_sram_blwl_out[1703:1703] ,mux_2level_size50_2_sram_blwl_out[1703:1703] ,mux_2level_size50_2_sram_blwl_outb[1703:1703] ,mux_2level_size50_2_configbus0[1703:1703], mux_2level_size50_2_configbus1[1703:1703] , mux_2level_size50_2_configbus0_b[1703:1703] );
sram6T_blwl sram_blwl_1704_ (mux_2level_size50_2_sram_blwl_out[1704:1704] ,mux_2level_size50_2_sram_blwl_out[1704:1704] ,mux_2level_size50_2_sram_blwl_outb[1704:1704] ,mux_2level_size50_2_configbus0[1704:1704], mux_2level_size50_2_configbus1[1704:1704] , mux_2level_size50_2_configbus0_b[1704:1704] );
sram6T_blwl sram_blwl_1705_ (mux_2level_size50_2_sram_blwl_out[1705:1705] ,mux_2level_size50_2_sram_blwl_out[1705:1705] ,mux_2level_size50_2_sram_blwl_outb[1705:1705] ,mux_2level_size50_2_configbus0[1705:1705], mux_2level_size50_2_configbus1[1705:1705] , mux_2level_size50_2_configbus0_b[1705:1705] );
sram6T_blwl sram_blwl_1706_ (mux_2level_size50_2_sram_blwl_out[1706:1706] ,mux_2level_size50_2_sram_blwl_out[1706:1706] ,mux_2level_size50_2_sram_blwl_outb[1706:1706] ,mux_2level_size50_2_configbus0[1706:1706], mux_2level_size50_2_configbus1[1706:1706] , mux_2level_size50_2_configbus0_b[1706:1706] );
sram6T_blwl sram_blwl_1707_ (mux_2level_size50_2_sram_blwl_out[1707:1707] ,mux_2level_size50_2_sram_blwl_out[1707:1707] ,mux_2level_size50_2_sram_blwl_outb[1707:1707] ,mux_2level_size50_2_configbus0[1707:1707], mux_2level_size50_2_configbus1[1707:1707] , mux_2level_size50_2_configbus0_b[1707:1707] );
sram6T_blwl sram_blwl_1708_ (mux_2level_size50_2_sram_blwl_out[1708:1708] ,mux_2level_size50_2_sram_blwl_out[1708:1708] ,mux_2level_size50_2_sram_blwl_outb[1708:1708] ,mux_2level_size50_2_configbus0[1708:1708], mux_2level_size50_2_configbus1[1708:1708] , mux_2level_size50_2_configbus0_b[1708:1708] );
sram6T_blwl sram_blwl_1709_ (mux_2level_size50_2_sram_blwl_out[1709:1709] ,mux_2level_size50_2_sram_blwl_out[1709:1709] ,mux_2level_size50_2_sram_blwl_outb[1709:1709] ,mux_2level_size50_2_configbus0[1709:1709], mux_2level_size50_2_configbus1[1709:1709] , mux_2level_size50_2_configbus0_b[1709:1709] );
sram6T_blwl sram_blwl_1710_ (mux_2level_size50_2_sram_blwl_out[1710:1710] ,mux_2level_size50_2_sram_blwl_out[1710:1710] ,mux_2level_size50_2_sram_blwl_outb[1710:1710] ,mux_2level_size50_2_configbus0[1710:1710], mux_2level_size50_2_configbus1[1710:1710] , mux_2level_size50_2_configbus0_b[1710:1710] );
sram6T_blwl sram_blwl_1711_ (mux_2level_size50_2_sram_blwl_out[1711:1711] ,mux_2level_size50_2_sram_blwl_out[1711:1711] ,mux_2level_size50_2_sram_blwl_outb[1711:1711] ,mux_2level_size50_2_configbus0[1711:1711], mux_2level_size50_2_configbus1[1711:1711] , mux_2level_size50_2_configbus0_b[1711:1711] );
sram6T_blwl sram_blwl_1712_ (mux_2level_size50_2_sram_blwl_out[1712:1712] ,mux_2level_size50_2_sram_blwl_out[1712:1712] ,mux_2level_size50_2_sram_blwl_outb[1712:1712] ,mux_2level_size50_2_configbus0[1712:1712], mux_2level_size50_2_configbus1[1712:1712] , mux_2level_size50_2_configbus0_b[1712:1712] );
sram6T_blwl sram_blwl_1713_ (mux_2level_size50_2_sram_blwl_out[1713:1713] ,mux_2level_size50_2_sram_blwl_out[1713:1713] ,mux_2level_size50_2_sram_blwl_outb[1713:1713] ,mux_2level_size50_2_configbus0[1713:1713], mux_2level_size50_2_configbus1[1713:1713] , mux_2level_size50_2_configbus0_b[1713:1713] );
wire [0:49] in_bus_mux_2level_size50_3_ ;
assign in_bus_mux_2level_size50_3_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_3_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_3_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_3_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_3_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_3_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_3_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_3_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_3_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_3_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_3_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_3_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_3_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_3_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_3_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_3_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_3_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_3_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_3_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_3_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_3_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_3_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_3_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_3_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_3_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_3_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_3_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_3_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_3_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_3_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_3_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_3_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_3_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_3_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_3_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_3_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_3_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_3_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_3_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_3_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_3_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_3_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_3_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_3_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_3_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_3_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_3_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_3_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_3_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_3_[49] = fle_9___out_0_ ; 
wire [1714:1729] mux_2level_size50_3_configbus0;
wire [1714:1729] mux_2level_size50_3_configbus1;
wire [1714:1729] mux_2level_size50_3_sram_blwl_out ;
wire [1714:1729] mux_2level_size50_3_sram_blwl_outb ;
assign mux_2level_size50_3_configbus0[1714:1729] = sram_blwl_bl[1714:1729] ;
assign mux_2level_size50_3_configbus1[1714:1729] = sram_blwl_wl[1714:1729] ;
wire [1714:1729] mux_2level_size50_3_configbus0_b;
assign mux_2level_size50_3_configbus0_b[1714:1729] = sram_blwl_blb[1714:1729] ;
mux_2level_size50 mux_2level_size50_3_ (in_bus_mux_2level_size50_3_, fle_0___in_3_, mux_2level_size50_3_sram_blwl_out[1714:1729] ,
mux_2level_size50_3_sram_blwl_outb[1714:1729] );
//----- SRAM bits for MUX[3], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1714_ (mux_2level_size50_3_sram_blwl_out[1714:1714] ,mux_2level_size50_3_sram_blwl_out[1714:1714] ,mux_2level_size50_3_sram_blwl_outb[1714:1714] ,mux_2level_size50_3_configbus0[1714:1714], mux_2level_size50_3_configbus1[1714:1714] , mux_2level_size50_3_configbus0_b[1714:1714] );
sram6T_blwl sram_blwl_1715_ (mux_2level_size50_3_sram_blwl_out[1715:1715] ,mux_2level_size50_3_sram_blwl_out[1715:1715] ,mux_2level_size50_3_sram_blwl_outb[1715:1715] ,mux_2level_size50_3_configbus0[1715:1715], mux_2level_size50_3_configbus1[1715:1715] , mux_2level_size50_3_configbus0_b[1715:1715] );
sram6T_blwl sram_blwl_1716_ (mux_2level_size50_3_sram_blwl_out[1716:1716] ,mux_2level_size50_3_sram_blwl_out[1716:1716] ,mux_2level_size50_3_sram_blwl_outb[1716:1716] ,mux_2level_size50_3_configbus0[1716:1716], mux_2level_size50_3_configbus1[1716:1716] , mux_2level_size50_3_configbus0_b[1716:1716] );
sram6T_blwl sram_blwl_1717_ (mux_2level_size50_3_sram_blwl_out[1717:1717] ,mux_2level_size50_3_sram_blwl_out[1717:1717] ,mux_2level_size50_3_sram_blwl_outb[1717:1717] ,mux_2level_size50_3_configbus0[1717:1717], mux_2level_size50_3_configbus1[1717:1717] , mux_2level_size50_3_configbus0_b[1717:1717] );
sram6T_blwl sram_blwl_1718_ (mux_2level_size50_3_sram_blwl_out[1718:1718] ,mux_2level_size50_3_sram_blwl_out[1718:1718] ,mux_2level_size50_3_sram_blwl_outb[1718:1718] ,mux_2level_size50_3_configbus0[1718:1718], mux_2level_size50_3_configbus1[1718:1718] , mux_2level_size50_3_configbus0_b[1718:1718] );
sram6T_blwl sram_blwl_1719_ (mux_2level_size50_3_sram_blwl_out[1719:1719] ,mux_2level_size50_3_sram_blwl_out[1719:1719] ,mux_2level_size50_3_sram_blwl_outb[1719:1719] ,mux_2level_size50_3_configbus0[1719:1719], mux_2level_size50_3_configbus1[1719:1719] , mux_2level_size50_3_configbus0_b[1719:1719] );
sram6T_blwl sram_blwl_1720_ (mux_2level_size50_3_sram_blwl_out[1720:1720] ,mux_2level_size50_3_sram_blwl_out[1720:1720] ,mux_2level_size50_3_sram_blwl_outb[1720:1720] ,mux_2level_size50_3_configbus0[1720:1720], mux_2level_size50_3_configbus1[1720:1720] , mux_2level_size50_3_configbus0_b[1720:1720] );
sram6T_blwl sram_blwl_1721_ (mux_2level_size50_3_sram_blwl_out[1721:1721] ,mux_2level_size50_3_sram_blwl_out[1721:1721] ,mux_2level_size50_3_sram_blwl_outb[1721:1721] ,mux_2level_size50_3_configbus0[1721:1721], mux_2level_size50_3_configbus1[1721:1721] , mux_2level_size50_3_configbus0_b[1721:1721] );
sram6T_blwl sram_blwl_1722_ (mux_2level_size50_3_sram_blwl_out[1722:1722] ,mux_2level_size50_3_sram_blwl_out[1722:1722] ,mux_2level_size50_3_sram_blwl_outb[1722:1722] ,mux_2level_size50_3_configbus0[1722:1722], mux_2level_size50_3_configbus1[1722:1722] , mux_2level_size50_3_configbus0_b[1722:1722] );
sram6T_blwl sram_blwl_1723_ (mux_2level_size50_3_sram_blwl_out[1723:1723] ,mux_2level_size50_3_sram_blwl_out[1723:1723] ,mux_2level_size50_3_sram_blwl_outb[1723:1723] ,mux_2level_size50_3_configbus0[1723:1723], mux_2level_size50_3_configbus1[1723:1723] , mux_2level_size50_3_configbus0_b[1723:1723] );
sram6T_blwl sram_blwl_1724_ (mux_2level_size50_3_sram_blwl_out[1724:1724] ,mux_2level_size50_3_sram_blwl_out[1724:1724] ,mux_2level_size50_3_sram_blwl_outb[1724:1724] ,mux_2level_size50_3_configbus0[1724:1724], mux_2level_size50_3_configbus1[1724:1724] , mux_2level_size50_3_configbus0_b[1724:1724] );
sram6T_blwl sram_blwl_1725_ (mux_2level_size50_3_sram_blwl_out[1725:1725] ,mux_2level_size50_3_sram_blwl_out[1725:1725] ,mux_2level_size50_3_sram_blwl_outb[1725:1725] ,mux_2level_size50_3_configbus0[1725:1725], mux_2level_size50_3_configbus1[1725:1725] , mux_2level_size50_3_configbus0_b[1725:1725] );
sram6T_blwl sram_blwl_1726_ (mux_2level_size50_3_sram_blwl_out[1726:1726] ,mux_2level_size50_3_sram_blwl_out[1726:1726] ,mux_2level_size50_3_sram_blwl_outb[1726:1726] ,mux_2level_size50_3_configbus0[1726:1726], mux_2level_size50_3_configbus1[1726:1726] , mux_2level_size50_3_configbus0_b[1726:1726] );
sram6T_blwl sram_blwl_1727_ (mux_2level_size50_3_sram_blwl_out[1727:1727] ,mux_2level_size50_3_sram_blwl_out[1727:1727] ,mux_2level_size50_3_sram_blwl_outb[1727:1727] ,mux_2level_size50_3_configbus0[1727:1727], mux_2level_size50_3_configbus1[1727:1727] , mux_2level_size50_3_configbus0_b[1727:1727] );
sram6T_blwl sram_blwl_1728_ (mux_2level_size50_3_sram_blwl_out[1728:1728] ,mux_2level_size50_3_sram_blwl_out[1728:1728] ,mux_2level_size50_3_sram_blwl_outb[1728:1728] ,mux_2level_size50_3_configbus0[1728:1728], mux_2level_size50_3_configbus1[1728:1728] , mux_2level_size50_3_configbus0_b[1728:1728] );
sram6T_blwl sram_blwl_1729_ (mux_2level_size50_3_sram_blwl_out[1729:1729] ,mux_2level_size50_3_sram_blwl_out[1729:1729] ,mux_2level_size50_3_sram_blwl_outb[1729:1729] ,mux_2level_size50_3_configbus0[1729:1729], mux_2level_size50_3_configbus1[1729:1729] , mux_2level_size50_3_configbus0_b[1729:1729] );
wire [0:49] in_bus_mux_2level_size50_4_ ;
assign in_bus_mux_2level_size50_4_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_4_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_4_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_4_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_4_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_4_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_4_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_4_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_4_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_4_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_4_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_4_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_4_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_4_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_4_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_4_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_4_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_4_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_4_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_4_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_4_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_4_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_4_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_4_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_4_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_4_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_4_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_4_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_4_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_4_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_4_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_4_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_4_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_4_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_4_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_4_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_4_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_4_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_4_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_4_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_4_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_4_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_4_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_4_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_4_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_4_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_4_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_4_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_4_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_4_[49] = fle_9___out_0_ ; 
wire [1730:1745] mux_2level_size50_4_configbus0;
wire [1730:1745] mux_2level_size50_4_configbus1;
wire [1730:1745] mux_2level_size50_4_sram_blwl_out ;
wire [1730:1745] mux_2level_size50_4_sram_blwl_outb ;
assign mux_2level_size50_4_configbus0[1730:1745] = sram_blwl_bl[1730:1745] ;
assign mux_2level_size50_4_configbus1[1730:1745] = sram_blwl_wl[1730:1745] ;
wire [1730:1745] mux_2level_size50_4_configbus0_b;
assign mux_2level_size50_4_configbus0_b[1730:1745] = sram_blwl_blb[1730:1745] ;
mux_2level_size50 mux_2level_size50_4_ (in_bus_mux_2level_size50_4_, fle_0___in_4_, mux_2level_size50_4_sram_blwl_out[1730:1745] ,
mux_2level_size50_4_sram_blwl_outb[1730:1745] );
//----- SRAM bits for MUX[4], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1730_ (mux_2level_size50_4_sram_blwl_out[1730:1730] ,mux_2level_size50_4_sram_blwl_out[1730:1730] ,mux_2level_size50_4_sram_blwl_outb[1730:1730] ,mux_2level_size50_4_configbus0[1730:1730], mux_2level_size50_4_configbus1[1730:1730] , mux_2level_size50_4_configbus0_b[1730:1730] );
sram6T_blwl sram_blwl_1731_ (mux_2level_size50_4_sram_blwl_out[1731:1731] ,mux_2level_size50_4_sram_blwl_out[1731:1731] ,mux_2level_size50_4_sram_blwl_outb[1731:1731] ,mux_2level_size50_4_configbus0[1731:1731], mux_2level_size50_4_configbus1[1731:1731] , mux_2level_size50_4_configbus0_b[1731:1731] );
sram6T_blwl sram_blwl_1732_ (mux_2level_size50_4_sram_blwl_out[1732:1732] ,mux_2level_size50_4_sram_blwl_out[1732:1732] ,mux_2level_size50_4_sram_blwl_outb[1732:1732] ,mux_2level_size50_4_configbus0[1732:1732], mux_2level_size50_4_configbus1[1732:1732] , mux_2level_size50_4_configbus0_b[1732:1732] );
sram6T_blwl sram_blwl_1733_ (mux_2level_size50_4_sram_blwl_out[1733:1733] ,mux_2level_size50_4_sram_blwl_out[1733:1733] ,mux_2level_size50_4_sram_blwl_outb[1733:1733] ,mux_2level_size50_4_configbus0[1733:1733], mux_2level_size50_4_configbus1[1733:1733] , mux_2level_size50_4_configbus0_b[1733:1733] );
sram6T_blwl sram_blwl_1734_ (mux_2level_size50_4_sram_blwl_out[1734:1734] ,mux_2level_size50_4_sram_blwl_out[1734:1734] ,mux_2level_size50_4_sram_blwl_outb[1734:1734] ,mux_2level_size50_4_configbus0[1734:1734], mux_2level_size50_4_configbus1[1734:1734] , mux_2level_size50_4_configbus0_b[1734:1734] );
sram6T_blwl sram_blwl_1735_ (mux_2level_size50_4_sram_blwl_out[1735:1735] ,mux_2level_size50_4_sram_blwl_out[1735:1735] ,mux_2level_size50_4_sram_blwl_outb[1735:1735] ,mux_2level_size50_4_configbus0[1735:1735], mux_2level_size50_4_configbus1[1735:1735] , mux_2level_size50_4_configbus0_b[1735:1735] );
sram6T_blwl sram_blwl_1736_ (mux_2level_size50_4_sram_blwl_out[1736:1736] ,mux_2level_size50_4_sram_blwl_out[1736:1736] ,mux_2level_size50_4_sram_blwl_outb[1736:1736] ,mux_2level_size50_4_configbus0[1736:1736], mux_2level_size50_4_configbus1[1736:1736] , mux_2level_size50_4_configbus0_b[1736:1736] );
sram6T_blwl sram_blwl_1737_ (mux_2level_size50_4_sram_blwl_out[1737:1737] ,mux_2level_size50_4_sram_blwl_out[1737:1737] ,mux_2level_size50_4_sram_blwl_outb[1737:1737] ,mux_2level_size50_4_configbus0[1737:1737], mux_2level_size50_4_configbus1[1737:1737] , mux_2level_size50_4_configbus0_b[1737:1737] );
sram6T_blwl sram_blwl_1738_ (mux_2level_size50_4_sram_blwl_out[1738:1738] ,mux_2level_size50_4_sram_blwl_out[1738:1738] ,mux_2level_size50_4_sram_blwl_outb[1738:1738] ,mux_2level_size50_4_configbus0[1738:1738], mux_2level_size50_4_configbus1[1738:1738] , mux_2level_size50_4_configbus0_b[1738:1738] );
sram6T_blwl sram_blwl_1739_ (mux_2level_size50_4_sram_blwl_out[1739:1739] ,mux_2level_size50_4_sram_blwl_out[1739:1739] ,mux_2level_size50_4_sram_blwl_outb[1739:1739] ,mux_2level_size50_4_configbus0[1739:1739], mux_2level_size50_4_configbus1[1739:1739] , mux_2level_size50_4_configbus0_b[1739:1739] );
sram6T_blwl sram_blwl_1740_ (mux_2level_size50_4_sram_blwl_out[1740:1740] ,mux_2level_size50_4_sram_blwl_out[1740:1740] ,mux_2level_size50_4_sram_blwl_outb[1740:1740] ,mux_2level_size50_4_configbus0[1740:1740], mux_2level_size50_4_configbus1[1740:1740] , mux_2level_size50_4_configbus0_b[1740:1740] );
sram6T_blwl sram_blwl_1741_ (mux_2level_size50_4_sram_blwl_out[1741:1741] ,mux_2level_size50_4_sram_blwl_out[1741:1741] ,mux_2level_size50_4_sram_blwl_outb[1741:1741] ,mux_2level_size50_4_configbus0[1741:1741], mux_2level_size50_4_configbus1[1741:1741] , mux_2level_size50_4_configbus0_b[1741:1741] );
sram6T_blwl sram_blwl_1742_ (mux_2level_size50_4_sram_blwl_out[1742:1742] ,mux_2level_size50_4_sram_blwl_out[1742:1742] ,mux_2level_size50_4_sram_blwl_outb[1742:1742] ,mux_2level_size50_4_configbus0[1742:1742], mux_2level_size50_4_configbus1[1742:1742] , mux_2level_size50_4_configbus0_b[1742:1742] );
sram6T_blwl sram_blwl_1743_ (mux_2level_size50_4_sram_blwl_out[1743:1743] ,mux_2level_size50_4_sram_blwl_out[1743:1743] ,mux_2level_size50_4_sram_blwl_outb[1743:1743] ,mux_2level_size50_4_configbus0[1743:1743], mux_2level_size50_4_configbus1[1743:1743] , mux_2level_size50_4_configbus0_b[1743:1743] );
sram6T_blwl sram_blwl_1744_ (mux_2level_size50_4_sram_blwl_out[1744:1744] ,mux_2level_size50_4_sram_blwl_out[1744:1744] ,mux_2level_size50_4_sram_blwl_outb[1744:1744] ,mux_2level_size50_4_configbus0[1744:1744], mux_2level_size50_4_configbus1[1744:1744] , mux_2level_size50_4_configbus0_b[1744:1744] );
sram6T_blwl sram_blwl_1745_ (mux_2level_size50_4_sram_blwl_out[1745:1745] ,mux_2level_size50_4_sram_blwl_out[1745:1745] ,mux_2level_size50_4_sram_blwl_outb[1745:1745] ,mux_2level_size50_4_configbus0[1745:1745], mux_2level_size50_4_configbus1[1745:1745] , mux_2level_size50_4_configbus0_b[1745:1745] );
wire [0:49] in_bus_mux_2level_size50_5_ ;
assign in_bus_mux_2level_size50_5_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_5_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_5_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_5_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_5_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_5_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_5_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_5_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_5_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_5_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_5_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_5_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_5_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_5_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_5_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_5_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_5_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_5_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_5_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_5_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_5_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_5_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_5_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_5_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_5_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_5_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_5_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_5_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_5_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_5_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_5_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_5_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_5_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_5_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_5_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_5_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_5_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_5_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_5_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_5_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_5_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_5_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_5_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_5_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_5_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_5_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_5_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_5_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_5_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_5_[49] = fle_9___out_0_ ; 
wire [1746:1761] mux_2level_size50_5_configbus0;
wire [1746:1761] mux_2level_size50_5_configbus1;
wire [1746:1761] mux_2level_size50_5_sram_blwl_out ;
wire [1746:1761] mux_2level_size50_5_sram_blwl_outb ;
assign mux_2level_size50_5_configbus0[1746:1761] = sram_blwl_bl[1746:1761] ;
assign mux_2level_size50_5_configbus1[1746:1761] = sram_blwl_wl[1746:1761] ;
wire [1746:1761] mux_2level_size50_5_configbus0_b;
assign mux_2level_size50_5_configbus0_b[1746:1761] = sram_blwl_blb[1746:1761] ;
mux_2level_size50 mux_2level_size50_5_ (in_bus_mux_2level_size50_5_, fle_0___in_5_, mux_2level_size50_5_sram_blwl_out[1746:1761] ,
mux_2level_size50_5_sram_blwl_outb[1746:1761] );
//----- SRAM bits for MUX[5], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1746_ (mux_2level_size50_5_sram_blwl_out[1746:1746] ,mux_2level_size50_5_sram_blwl_out[1746:1746] ,mux_2level_size50_5_sram_blwl_outb[1746:1746] ,mux_2level_size50_5_configbus0[1746:1746], mux_2level_size50_5_configbus1[1746:1746] , mux_2level_size50_5_configbus0_b[1746:1746] );
sram6T_blwl sram_blwl_1747_ (mux_2level_size50_5_sram_blwl_out[1747:1747] ,mux_2level_size50_5_sram_blwl_out[1747:1747] ,mux_2level_size50_5_sram_blwl_outb[1747:1747] ,mux_2level_size50_5_configbus0[1747:1747], mux_2level_size50_5_configbus1[1747:1747] , mux_2level_size50_5_configbus0_b[1747:1747] );
sram6T_blwl sram_blwl_1748_ (mux_2level_size50_5_sram_blwl_out[1748:1748] ,mux_2level_size50_5_sram_blwl_out[1748:1748] ,mux_2level_size50_5_sram_blwl_outb[1748:1748] ,mux_2level_size50_5_configbus0[1748:1748], mux_2level_size50_5_configbus1[1748:1748] , mux_2level_size50_5_configbus0_b[1748:1748] );
sram6T_blwl sram_blwl_1749_ (mux_2level_size50_5_sram_blwl_out[1749:1749] ,mux_2level_size50_5_sram_blwl_out[1749:1749] ,mux_2level_size50_5_sram_blwl_outb[1749:1749] ,mux_2level_size50_5_configbus0[1749:1749], mux_2level_size50_5_configbus1[1749:1749] , mux_2level_size50_5_configbus0_b[1749:1749] );
sram6T_blwl sram_blwl_1750_ (mux_2level_size50_5_sram_blwl_out[1750:1750] ,mux_2level_size50_5_sram_blwl_out[1750:1750] ,mux_2level_size50_5_sram_blwl_outb[1750:1750] ,mux_2level_size50_5_configbus0[1750:1750], mux_2level_size50_5_configbus1[1750:1750] , mux_2level_size50_5_configbus0_b[1750:1750] );
sram6T_blwl sram_blwl_1751_ (mux_2level_size50_5_sram_blwl_out[1751:1751] ,mux_2level_size50_5_sram_blwl_out[1751:1751] ,mux_2level_size50_5_sram_blwl_outb[1751:1751] ,mux_2level_size50_5_configbus0[1751:1751], mux_2level_size50_5_configbus1[1751:1751] , mux_2level_size50_5_configbus0_b[1751:1751] );
sram6T_blwl sram_blwl_1752_ (mux_2level_size50_5_sram_blwl_out[1752:1752] ,mux_2level_size50_5_sram_blwl_out[1752:1752] ,mux_2level_size50_5_sram_blwl_outb[1752:1752] ,mux_2level_size50_5_configbus0[1752:1752], mux_2level_size50_5_configbus1[1752:1752] , mux_2level_size50_5_configbus0_b[1752:1752] );
sram6T_blwl sram_blwl_1753_ (mux_2level_size50_5_sram_blwl_out[1753:1753] ,mux_2level_size50_5_sram_blwl_out[1753:1753] ,mux_2level_size50_5_sram_blwl_outb[1753:1753] ,mux_2level_size50_5_configbus0[1753:1753], mux_2level_size50_5_configbus1[1753:1753] , mux_2level_size50_5_configbus0_b[1753:1753] );
sram6T_blwl sram_blwl_1754_ (mux_2level_size50_5_sram_blwl_out[1754:1754] ,mux_2level_size50_5_sram_blwl_out[1754:1754] ,mux_2level_size50_5_sram_blwl_outb[1754:1754] ,mux_2level_size50_5_configbus0[1754:1754], mux_2level_size50_5_configbus1[1754:1754] , mux_2level_size50_5_configbus0_b[1754:1754] );
sram6T_blwl sram_blwl_1755_ (mux_2level_size50_5_sram_blwl_out[1755:1755] ,mux_2level_size50_5_sram_blwl_out[1755:1755] ,mux_2level_size50_5_sram_blwl_outb[1755:1755] ,mux_2level_size50_5_configbus0[1755:1755], mux_2level_size50_5_configbus1[1755:1755] , mux_2level_size50_5_configbus0_b[1755:1755] );
sram6T_blwl sram_blwl_1756_ (mux_2level_size50_5_sram_blwl_out[1756:1756] ,mux_2level_size50_5_sram_blwl_out[1756:1756] ,mux_2level_size50_5_sram_blwl_outb[1756:1756] ,mux_2level_size50_5_configbus0[1756:1756], mux_2level_size50_5_configbus1[1756:1756] , mux_2level_size50_5_configbus0_b[1756:1756] );
sram6T_blwl sram_blwl_1757_ (mux_2level_size50_5_sram_blwl_out[1757:1757] ,mux_2level_size50_5_sram_blwl_out[1757:1757] ,mux_2level_size50_5_sram_blwl_outb[1757:1757] ,mux_2level_size50_5_configbus0[1757:1757], mux_2level_size50_5_configbus1[1757:1757] , mux_2level_size50_5_configbus0_b[1757:1757] );
sram6T_blwl sram_blwl_1758_ (mux_2level_size50_5_sram_blwl_out[1758:1758] ,mux_2level_size50_5_sram_blwl_out[1758:1758] ,mux_2level_size50_5_sram_blwl_outb[1758:1758] ,mux_2level_size50_5_configbus0[1758:1758], mux_2level_size50_5_configbus1[1758:1758] , mux_2level_size50_5_configbus0_b[1758:1758] );
sram6T_blwl sram_blwl_1759_ (mux_2level_size50_5_sram_blwl_out[1759:1759] ,mux_2level_size50_5_sram_blwl_out[1759:1759] ,mux_2level_size50_5_sram_blwl_outb[1759:1759] ,mux_2level_size50_5_configbus0[1759:1759], mux_2level_size50_5_configbus1[1759:1759] , mux_2level_size50_5_configbus0_b[1759:1759] );
sram6T_blwl sram_blwl_1760_ (mux_2level_size50_5_sram_blwl_out[1760:1760] ,mux_2level_size50_5_sram_blwl_out[1760:1760] ,mux_2level_size50_5_sram_blwl_outb[1760:1760] ,mux_2level_size50_5_configbus0[1760:1760], mux_2level_size50_5_configbus1[1760:1760] , mux_2level_size50_5_configbus0_b[1760:1760] );
sram6T_blwl sram_blwl_1761_ (mux_2level_size50_5_sram_blwl_out[1761:1761] ,mux_2level_size50_5_sram_blwl_out[1761:1761] ,mux_2level_size50_5_sram_blwl_outb[1761:1761] ,mux_2level_size50_5_configbus0[1761:1761], mux_2level_size50_5_configbus1[1761:1761] , mux_2level_size50_5_configbus0_b[1761:1761] );
direct_interc direct_interc_170_ (mode_clb___clk_0_, fle_0___clk_0_ );
wire [0:49] in_bus_mux_2level_size50_6_ ;
assign in_bus_mux_2level_size50_6_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_6_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_6_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_6_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_6_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_6_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_6_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_6_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_6_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_6_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_6_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_6_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_6_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_6_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_6_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_6_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_6_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_6_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_6_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_6_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_6_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_6_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_6_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_6_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_6_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_6_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_6_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_6_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_6_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_6_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_6_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_6_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_6_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_6_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_6_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_6_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_6_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_6_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_6_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_6_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_6_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_6_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_6_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_6_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_6_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_6_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_6_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_6_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_6_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_6_[49] = fle_9___out_0_ ; 
wire [1762:1777] mux_2level_size50_6_configbus0;
wire [1762:1777] mux_2level_size50_6_configbus1;
wire [1762:1777] mux_2level_size50_6_sram_blwl_out ;
wire [1762:1777] mux_2level_size50_6_sram_blwl_outb ;
assign mux_2level_size50_6_configbus0[1762:1777] = sram_blwl_bl[1762:1777] ;
assign mux_2level_size50_6_configbus1[1762:1777] = sram_blwl_wl[1762:1777] ;
wire [1762:1777] mux_2level_size50_6_configbus0_b;
assign mux_2level_size50_6_configbus0_b[1762:1777] = sram_blwl_blb[1762:1777] ;
mux_2level_size50 mux_2level_size50_6_ (in_bus_mux_2level_size50_6_, fle_1___in_0_, mux_2level_size50_6_sram_blwl_out[1762:1777] ,
mux_2level_size50_6_sram_blwl_outb[1762:1777] );
//----- SRAM bits for MUX[6], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1762_ (mux_2level_size50_6_sram_blwl_out[1762:1762] ,mux_2level_size50_6_sram_blwl_out[1762:1762] ,mux_2level_size50_6_sram_blwl_outb[1762:1762] ,mux_2level_size50_6_configbus0[1762:1762], mux_2level_size50_6_configbus1[1762:1762] , mux_2level_size50_6_configbus0_b[1762:1762] );
sram6T_blwl sram_blwl_1763_ (mux_2level_size50_6_sram_blwl_out[1763:1763] ,mux_2level_size50_6_sram_blwl_out[1763:1763] ,mux_2level_size50_6_sram_blwl_outb[1763:1763] ,mux_2level_size50_6_configbus0[1763:1763], mux_2level_size50_6_configbus1[1763:1763] , mux_2level_size50_6_configbus0_b[1763:1763] );
sram6T_blwl sram_blwl_1764_ (mux_2level_size50_6_sram_blwl_out[1764:1764] ,mux_2level_size50_6_sram_blwl_out[1764:1764] ,mux_2level_size50_6_sram_blwl_outb[1764:1764] ,mux_2level_size50_6_configbus0[1764:1764], mux_2level_size50_6_configbus1[1764:1764] , mux_2level_size50_6_configbus0_b[1764:1764] );
sram6T_blwl sram_blwl_1765_ (mux_2level_size50_6_sram_blwl_out[1765:1765] ,mux_2level_size50_6_sram_blwl_out[1765:1765] ,mux_2level_size50_6_sram_blwl_outb[1765:1765] ,mux_2level_size50_6_configbus0[1765:1765], mux_2level_size50_6_configbus1[1765:1765] , mux_2level_size50_6_configbus0_b[1765:1765] );
sram6T_blwl sram_blwl_1766_ (mux_2level_size50_6_sram_blwl_out[1766:1766] ,mux_2level_size50_6_sram_blwl_out[1766:1766] ,mux_2level_size50_6_sram_blwl_outb[1766:1766] ,mux_2level_size50_6_configbus0[1766:1766], mux_2level_size50_6_configbus1[1766:1766] , mux_2level_size50_6_configbus0_b[1766:1766] );
sram6T_blwl sram_blwl_1767_ (mux_2level_size50_6_sram_blwl_out[1767:1767] ,mux_2level_size50_6_sram_blwl_out[1767:1767] ,mux_2level_size50_6_sram_blwl_outb[1767:1767] ,mux_2level_size50_6_configbus0[1767:1767], mux_2level_size50_6_configbus1[1767:1767] , mux_2level_size50_6_configbus0_b[1767:1767] );
sram6T_blwl sram_blwl_1768_ (mux_2level_size50_6_sram_blwl_out[1768:1768] ,mux_2level_size50_6_sram_blwl_out[1768:1768] ,mux_2level_size50_6_sram_blwl_outb[1768:1768] ,mux_2level_size50_6_configbus0[1768:1768], mux_2level_size50_6_configbus1[1768:1768] , mux_2level_size50_6_configbus0_b[1768:1768] );
sram6T_blwl sram_blwl_1769_ (mux_2level_size50_6_sram_blwl_out[1769:1769] ,mux_2level_size50_6_sram_blwl_out[1769:1769] ,mux_2level_size50_6_sram_blwl_outb[1769:1769] ,mux_2level_size50_6_configbus0[1769:1769], mux_2level_size50_6_configbus1[1769:1769] , mux_2level_size50_6_configbus0_b[1769:1769] );
sram6T_blwl sram_blwl_1770_ (mux_2level_size50_6_sram_blwl_out[1770:1770] ,mux_2level_size50_6_sram_blwl_out[1770:1770] ,mux_2level_size50_6_sram_blwl_outb[1770:1770] ,mux_2level_size50_6_configbus0[1770:1770], mux_2level_size50_6_configbus1[1770:1770] , mux_2level_size50_6_configbus0_b[1770:1770] );
sram6T_blwl sram_blwl_1771_ (mux_2level_size50_6_sram_blwl_out[1771:1771] ,mux_2level_size50_6_sram_blwl_out[1771:1771] ,mux_2level_size50_6_sram_blwl_outb[1771:1771] ,mux_2level_size50_6_configbus0[1771:1771], mux_2level_size50_6_configbus1[1771:1771] , mux_2level_size50_6_configbus0_b[1771:1771] );
sram6T_blwl sram_blwl_1772_ (mux_2level_size50_6_sram_blwl_out[1772:1772] ,mux_2level_size50_6_sram_blwl_out[1772:1772] ,mux_2level_size50_6_sram_blwl_outb[1772:1772] ,mux_2level_size50_6_configbus0[1772:1772], mux_2level_size50_6_configbus1[1772:1772] , mux_2level_size50_6_configbus0_b[1772:1772] );
sram6T_blwl sram_blwl_1773_ (mux_2level_size50_6_sram_blwl_out[1773:1773] ,mux_2level_size50_6_sram_blwl_out[1773:1773] ,mux_2level_size50_6_sram_blwl_outb[1773:1773] ,mux_2level_size50_6_configbus0[1773:1773], mux_2level_size50_6_configbus1[1773:1773] , mux_2level_size50_6_configbus0_b[1773:1773] );
sram6T_blwl sram_blwl_1774_ (mux_2level_size50_6_sram_blwl_out[1774:1774] ,mux_2level_size50_6_sram_blwl_out[1774:1774] ,mux_2level_size50_6_sram_blwl_outb[1774:1774] ,mux_2level_size50_6_configbus0[1774:1774], mux_2level_size50_6_configbus1[1774:1774] , mux_2level_size50_6_configbus0_b[1774:1774] );
sram6T_blwl sram_blwl_1775_ (mux_2level_size50_6_sram_blwl_out[1775:1775] ,mux_2level_size50_6_sram_blwl_out[1775:1775] ,mux_2level_size50_6_sram_blwl_outb[1775:1775] ,mux_2level_size50_6_configbus0[1775:1775], mux_2level_size50_6_configbus1[1775:1775] , mux_2level_size50_6_configbus0_b[1775:1775] );
sram6T_blwl sram_blwl_1776_ (mux_2level_size50_6_sram_blwl_out[1776:1776] ,mux_2level_size50_6_sram_blwl_out[1776:1776] ,mux_2level_size50_6_sram_blwl_outb[1776:1776] ,mux_2level_size50_6_configbus0[1776:1776], mux_2level_size50_6_configbus1[1776:1776] , mux_2level_size50_6_configbus0_b[1776:1776] );
sram6T_blwl sram_blwl_1777_ (mux_2level_size50_6_sram_blwl_out[1777:1777] ,mux_2level_size50_6_sram_blwl_out[1777:1777] ,mux_2level_size50_6_sram_blwl_outb[1777:1777] ,mux_2level_size50_6_configbus0[1777:1777], mux_2level_size50_6_configbus1[1777:1777] , mux_2level_size50_6_configbus0_b[1777:1777] );
wire [0:49] in_bus_mux_2level_size50_7_ ;
assign in_bus_mux_2level_size50_7_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_7_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_7_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_7_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_7_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_7_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_7_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_7_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_7_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_7_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_7_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_7_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_7_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_7_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_7_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_7_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_7_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_7_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_7_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_7_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_7_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_7_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_7_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_7_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_7_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_7_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_7_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_7_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_7_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_7_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_7_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_7_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_7_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_7_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_7_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_7_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_7_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_7_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_7_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_7_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_7_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_7_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_7_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_7_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_7_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_7_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_7_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_7_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_7_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_7_[49] = fle_9___out_0_ ; 
wire [1778:1793] mux_2level_size50_7_configbus0;
wire [1778:1793] mux_2level_size50_7_configbus1;
wire [1778:1793] mux_2level_size50_7_sram_blwl_out ;
wire [1778:1793] mux_2level_size50_7_sram_blwl_outb ;
assign mux_2level_size50_7_configbus0[1778:1793] = sram_blwl_bl[1778:1793] ;
assign mux_2level_size50_7_configbus1[1778:1793] = sram_blwl_wl[1778:1793] ;
wire [1778:1793] mux_2level_size50_7_configbus0_b;
assign mux_2level_size50_7_configbus0_b[1778:1793] = sram_blwl_blb[1778:1793] ;
mux_2level_size50 mux_2level_size50_7_ (in_bus_mux_2level_size50_7_, fle_1___in_1_, mux_2level_size50_7_sram_blwl_out[1778:1793] ,
mux_2level_size50_7_sram_blwl_outb[1778:1793] );
//----- SRAM bits for MUX[7], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1778_ (mux_2level_size50_7_sram_blwl_out[1778:1778] ,mux_2level_size50_7_sram_blwl_out[1778:1778] ,mux_2level_size50_7_sram_blwl_outb[1778:1778] ,mux_2level_size50_7_configbus0[1778:1778], mux_2level_size50_7_configbus1[1778:1778] , mux_2level_size50_7_configbus0_b[1778:1778] );
sram6T_blwl sram_blwl_1779_ (mux_2level_size50_7_sram_blwl_out[1779:1779] ,mux_2level_size50_7_sram_blwl_out[1779:1779] ,mux_2level_size50_7_sram_blwl_outb[1779:1779] ,mux_2level_size50_7_configbus0[1779:1779], mux_2level_size50_7_configbus1[1779:1779] , mux_2level_size50_7_configbus0_b[1779:1779] );
sram6T_blwl sram_blwl_1780_ (mux_2level_size50_7_sram_blwl_out[1780:1780] ,mux_2level_size50_7_sram_blwl_out[1780:1780] ,mux_2level_size50_7_sram_blwl_outb[1780:1780] ,mux_2level_size50_7_configbus0[1780:1780], mux_2level_size50_7_configbus1[1780:1780] , mux_2level_size50_7_configbus0_b[1780:1780] );
sram6T_blwl sram_blwl_1781_ (mux_2level_size50_7_sram_blwl_out[1781:1781] ,mux_2level_size50_7_sram_blwl_out[1781:1781] ,mux_2level_size50_7_sram_blwl_outb[1781:1781] ,mux_2level_size50_7_configbus0[1781:1781], mux_2level_size50_7_configbus1[1781:1781] , mux_2level_size50_7_configbus0_b[1781:1781] );
sram6T_blwl sram_blwl_1782_ (mux_2level_size50_7_sram_blwl_out[1782:1782] ,mux_2level_size50_7_sram_blwl_out[1782:1782] ,mux_2level_size50_7_sram_blwl_outb[1782:1782] ,mux_2level_size50_7_configbus0[1782:1782], mux_2level_size50_7_configbus1[1782:1782] , mux_2level_size50_7_configbus0_b[1782:1782] );
sram6T_blwl sram_blwl_1783_ (mux_2level_size50_7_sram_blwl_out[1783:1783] ,mux_2level_size50_7_sram_blwl_out[1783:1783] ,mux_2level_size50_7_sram_blwl_outb[1783:1783] ,mux_2level_size50_7_configbus0[1783:1783], mux_2level_size50_7_configbus1[1783:1783] , mux_2level_size50_7_configbus0_b[1783:1783] );
sram6T_blwl sram_blwl_1784_ (mux_2level_size50_7_sram_blwl_out[1784:1784] ,mux_2level_size50_7_sram_blwl_out[1784:1784] ,mux_2level_size50_7_sram_blwl_outb[1784:1784] ,mux_2level_size50_7_configbus0[1784:1784], mux_2level_size50_7_configbus1[1784:1784] , mux_2level_size50_7_configbus0_b[1784:1784] );
sram6T_blwl sram_blwl_1785_ (mux_2level_size50_7_sram_blwl_out[1785:1785] ,mux_2level_size50_7_sram_blwl_out[1785:1785] ,mux_2level_size50_7_sram_blwl_outb[1785:1785] ,mux_2level_size50_7_configbus0[1785:1785], mux_2level_size50_7_configbus1[1785:1785] , mux_2level_size50_7_configbus0_b[1785:1785] );
sram6T_blwl sram_blwl_1786_ (mux_2level_size50_7_sram_blwl_out[1786:1786] ,mux_2level_size50_7_sram_blwl_out[1786:1786] ,mux_2level_size50_7_sram_blwl_outb[1786:1786] ,mux_2level_size50_7_configbus0[1786:1786], mux_2level_size50_7_configbus1[1786:1786] , mux_2level_size50_7_configbus0_b[1786:1786] );
sram6T_blwl sram_blwl_1787_ (mux_2level_size50_7_sram_blwl_out[1787:1787] ,mux_2level_size50_7_sram_blwl_out[1787:1787] ,mux_2level_size50_7_sram_blwl_outb[1787:1787] ,mux_2level_size50_7_configbus0[1787:1787], mux_2level_size50_7_configbus1[1787:1787] , mux_2level_size50_7_configbus0_b[1787:1787] );
sram6T_blwl sram_blwl_1788_ (mux_2level_size50_7_sram_blwl_out[1788:1788] ,mux_2level_size50_7_sram_blwl_out[1788:1788] ,mux_2level_size50_7_sram_blwl_outb[1788:1788] ,mux_2level_size50_7_configbus0[1788:1788], mux_2level_size50_7_configbus1[1788:1788] , mux_2level_size50_7_configbus0_b[1788:1788] );
sram6T_blwl sram_blwl_1789_ (mux_2level_size50_7_sram_blwl_out[1789:1789] ,mux_2level_size50_7_sram_blwl_out[1789:1789] ,mux_2level_size50_7_sram_blwl_outb[1789:1789] ,mux_2level_size50_7_configbus0[1789:1789], mux_2level_size50_7_configbus1[1789:1789] , mux_2level_size50_7_configbus0_b[1789:1789] );
sram6T_blwl sram_blwl_1790_ (mux_2level_size50_7_sram_blwl_out[1790:1790] ,mux_2level_size50_7_sram_blwl_out[1790:1790] ,mux_2level_size50_7_sram_blwl_outb[1790:1790] ,mux_2level_size50_7_configbus0[1790:1790], mux_2level_size50_7_configbus1[1790:1790] , mux_2level_size50_7_configbus0_b[1790:1790] );
sram6T_blwl sram_blwl_1791_ (mux_2level_size50_7_sram_blwl_out[1791:1791] ,mux_2level_size50_7_sram_blwl_out[1791:1791] ,mux_2level_size50_7_sram_blwl_outb[1791:1791] ,mux_2level_size50_7_configbus0[1791:1791], mux_2level_size50_7_configbus1[1791:1791] , mux_2level_size50_7_configbus0_b[1791:1791] );
sram6T_blwl sram_blwl_1792_ (mux_2level_size50_7_sram_blwl_out[1792:1792] ,mux_2level_size50_7_sram_blwl_out[1792:1792] ,mux_2level_size50_7_sram_blwl_outb[1792:1792] ,mux_2level_size50_7_configbus0[1792:1792], mux_2level_size50_7_configbus1[1792:1792] , mux_2level_size50_7_configbus0_b[1792:1792] );
sram6T_blwl sram_blwl_1793_ (mux_2level_size50_7_sram_blwl_out[1793:1793] ,mux_2level_size50_7_sram_blwl_out[1793:1793] ,mux_2level_size50_7_sram_blwl_outb[1793:1793] ,mux_2level_size50_7_configbus0[1793:1793], mux_2level_size50_7_configbus1[1793:1793] , mux_2level_size50_7_configbus0_b[1793:1793] );
wire [0:49] in_bus_mux_2level_size50_8_ ;
assign in_bus_mux_2level_size50_8_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_8_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_8_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_8_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_8_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_8_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_8_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_8_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_8_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_8_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_8_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_8_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_8_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_8_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_8_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_8_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_8_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_8_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_8_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_8_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_8_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_8_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_8_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_8_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_8_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_8_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_8_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_8_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_8_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_8_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_8_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_8_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_8_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_8_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_8_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_8_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_8_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_8_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_8_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_8_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_8_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_8_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_8_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_8_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_8_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_8_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_8_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_8_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_8_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_8_[49] = fle_9___out_0_ ; 
wire [1794:1809] mux_2level_size50_8_configbus0;
wire [1794:1809] mux_2level_size50_8_configbus1;
wire [1794:1809] mux_2level_size50_8_sram_blwl_out ;
wire [1794:1809] mux_2level_size50_8_sram_blwl_outb ;
assign mux_2level_size50_8_configbus0[1794:1809] = sram_blwl_bl[1794:1809] ;
assign mux_2level_size50_8_configbus1[1794:1809] = sram_blwl_wl[1794:1809] ;
wire [1794:1809] mux_2level_size50_8_configbus0_b;
assign mux_2level_size50_8_configbus0_b[1794:1809] = sram_blwl_blb[1794:1809] ;
mux_2level_size50 mux_2level_size50_8_ (in_bus_mux_2level_size50_8_, fle_1___in_2_, mux_2level_size50_8_sram_blwl_out[1794:1809] ,
mux_2level_size50_8_sram_blwl_outb[1794:1809] );
//----- SRAM bits for MUX[8], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1794_ (mux_2level_size50_8_sram_blwl_out[1794:1794] ,mux_2level_size50_8_sram_blwl_out[1794:1794] ,mux_2level_size50_8_sram_blwl_outb[1794:1794] ,mux_2level_size50_8_configbus0[1794:1794], mux_2level_size50_8_configbus1[1794:1794] , mux_2level_size50_8_configbus0_b[1794:1794] );
sram6T_blwl sram_blwl_1795_ (mux_2level_size50_8_sram_blwl_out[1795:1795] ,mux_2level_size50_8_sram_blwl_out[1795:1795] ,mux_2level_size50_8_sram_blwl_outb[1795:1795] ,mux_2level_size50_8_configbus0[1795:1795], mux_2level_size50_8_configbus1[1795:1795] , mux_2level_size50_8_configbus0_b[1795:1795] );
sram6T_blwl sram_blwl_1796_ (mux_2level_size50_8_sram_blwl_out[1796:1796] ,mux_2level_size50_8_sram_blwl_out[1796:1796] ,mux_2level_size50_8_sram_blwl_outb[1796:1796] ,mux_2level_size50_8_configbus0[1796:1796], mux_2level_size50_8_configbus1[1796:1796] , mux_2level_size50_8_configbus0_b[1796:1796] );
sram6T_blwl sram_blwl_1797_ (mux_2level_size50_8_sram_blwl_out[1797:1797] ,mux_2level_size50_8_sram_blwl_out[1797:1797] ,mux_2level_size50_8_sram_blwl_outb[1797:1797] ,mux_2level_size50_8_configbus0[1797:1797], mux_2level_size50_8_configbus1[1797:1797] , mux_2level_size50_8_configbus0_b[1797:1797] );
sram6T_blwl sram_blwl_1798_ (mux_2level_size50_8_sram_blwl_out[1798:1798] ,mux_2level_size50_8_sram_blwl_out[1798:1798] ,mux_2level_size50_8_sram_blwl_outb[1798:1798] ,mux_2level_size50_8_configbus0[1798:1798], mux_2level_size50_8_configbus1[1798:1798] , mux_2level_size50_8_configbus0_b[1798:1798] );
sram6T_blwl sram_blwl_1799_ (mux_2level_size50_8_sram_blwl_out[1799:1799] ,mux_2level_size50_8_sram_blwl_out[1799:1799] ,mux_2level_size50_8_sram_blwl_outb[1799:1799] ,mux_2level_size50_8_configbus0[1799:1799], mux_2level_size50_8_configbus1[1799:1799] , mux_2level_size50_8_configbus0_b[1799:1799] );
sram6T_blwl sram_blwl_1800_ (mux_2level_size50_8_sram_blwl_out[1800:1800] ,mux_2level_size50_8_sram_blwl_out[1800:1800] ,mux_2level_size50_8_sram_blwl_outb[1800:1800] ,mux_2level_size50_8_configbus0[1800:1800], mux_2level_size50_8_configbus1[1800:1800] , mux_2level_size50_8_configbus0_b[1800:1800] );
sram6T_blwl sram_blwl_1801_ (mux_2level_size50_8_sram_blwl_out[1801:1801] ,mux_2level_size50_8_sram_blwl_out[1801:1801] ,mux_2level_size50_8_sram_blwl_outb[1801:1801] ,mux_2level_size50_8_configbus0[1801:1801], mux_2level_size50_8_configbus1[1801:1801] , mux_2level_size50_8_configbus0_b[1801:1801] );
sram6T_blwl sram_blwl_1802_ (mux_2level_size50_8_sram_blwl_out[1802:1802] ,mux_2level_size50_8_sram_blwl_out[1802:1802] ,mux_2level_size50_8_sram_blwl_outb[1802:1802] ,mux_2level_size50_8_configbus0[1802:1802], mux_2level_size50_8_configbus1[1802:1802] , mux_2level_size50_8_configbus0_b[1802:1802] );
sram6T_blwl sram_blwl_1803_ (mux_2level_size50_8_sram_blwl_out[1803:1803] ,mux_2level_size50_8_sram_blwl_out[1803:1803] ,mux_2level_size50_8_sram_blwl_outb[1803:1803] ,mux_2level_size50_8_configbus0[1803:1803], mux_2level_size50_8_configbus1[1803:1803] , mux_2level_size50_8_configbus0_b[1803:1803] );
sram6T_blwl sram_blwl_1804_ (mux_2level_size50_8_sram_blwl_out[1804:1804] ,mux_2level_size50_8_sram_blwl_out[1804:1804] ,mux_2level_size50_8_sram_blwl_outb[1804:1804] ,mux_2level_size50_8_configbus0[1804:1804], mux_2level_size50_8_configbus1[1804:1804] , mux_2level_size50_8_configbus0_b[1804:1804] );
sram6T_blwl sram_blwl_1805_ (mux_2level_size50_8_sram_blwl_out[1805:1805] ,mux_2level_size50_8_sram_blwl_out[1805:1805] ,mux_2level_size50_8_sram_blwl_outb[1805:1805] ,mux_2level_size50_8_configbus0[1805:1805], mux_2level_size50_8_configbus1[1805:1805] , mux_2level_size50_8_configbus0_b[1805:1805] );
sram6T_blwl sram_blwl_1806_ (mux_2level_size50_8_sram_blwl_out[1806:1806] ,mux_2level_size50_8_sram_blwl_out[1806:1806] ,mux_2level_size50_8_sram_blwl_outb[1806:1806] ,mux_2level_size50_8_configbus0[1806:1806], mux_2level_size50_8_configbus1[1806:1806] , mux_2level_size50_8_configbus0_b[1806:1806] );
sram6T_blwl sram_blwl_1807_ (mux_2level_size50_8_sram_blwl_out[1807:1807] ,mux_2level_size50_8_sram_blwl_out[1807:1807] ,mux_2level_size50_8_sram_blwl_outb[1807:1807] ,mux_2level_size50_8_configbus0[1807:1807], mux_2level_size50_8_configbus1[1807:1807] , mux_2level_size50_8_configbus0_b[1807:1807] );
sram6T_blwl sram_blwl_1808_ (mux_2level_size50_8_sram_blwl_out[1808:1808] ,mux_2level_size50_8_sram_blwl_out[1808:1808] ,mux_2level_size50_8_sram_blwl_outb[1808:1808] ,mux_2level_size50_8_configbus0[1808:1808], mux_2level_size50_8_configbus1[1808:1808] , mux_2level_size50_8_configbus0_b[1808:1808] );
sram6T_blwl sram_blwl_1809_ (mux_2level_size50_8_sram_blwl_out[1809:1809] ,mux_2level_size50_8_sram_blwl_out[1809:1809] ,mux_2level_size50_8_sram_blwl_outb[1809:1809] ,mux_2level_size50_8_configbus0[1809:1809], mux_2level_size50_8_configbus1[1809:1809] , mux_2level_size50_8_configbus0_b[1809:1809] );
wire [0:49] in_bus_mux_2level_size50_9_ ;
assign in_bus_mux_2level_size50_9_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_9_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_9_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_9_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_9_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_9_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_9_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_9_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_9_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_9_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_9_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_9_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_9_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_9_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_9_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_9_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_9_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_9_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_9_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_9_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_9_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_9_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_9_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_9_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_9_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_9_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_9_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_9_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_9_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_9_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_9_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_9_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_9_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_9_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_9_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_9_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_9_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_9_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_9_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_9_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_9_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_9_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_9_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_9_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_9_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_9_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_9_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_9_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_9_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_9_[49] = fle_9___out_0_ ; 
wire [1810:1825] mux_2level_size50_9_configbus0;
wire [1810:1825] mux_2level_size50_9_configbus1;
wire [1810:1825] mux_2level_size50_9_sram_blwl_out ;
wire [1810:1825] mux_2level_size50_9_sram_blwl_outb ;
assign mux_2level_size50_9_configbus0[1810:1825] = sram_blwl_bl[1810:1825] ;
assign mux_2level_size50_9_configbus1[1810:1825] = sram_blwl_wl[1810:1825] ;
wire [1810:1825] mux_2level_size50_9_configbus0_b;
assign mux_2level_size50_9_configbus0_b[1810:1825] = sram_blwl_blb[1810:1825] ;
mux_2level_size50 mux_2level_size50_9_ (in_bus_mux_2level_size50_9_, fle_1___in_3_, mux_2level_size50_9_sram_blwl_out[1810:1825] ,
mux_2level_size50_9_sram_blwl_outb[1810:1825] );
//----- SRAM bits for MUX[9], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1810_ (mux_2level_size50_9_sram_blwl_out[1810:1810] ,mux_2level_size50_9_sram_blwl_out[1810:1810] ,mux_2level_size50_9_sram_blwl_outb[1810:1810] ,mux_2level_size50_9_configbus0[1810:1810], mux_2level_size50_9_configbus1[1810:1810] , mux_2level_size50_9_configbus0_b[1810:1810] );
sram6T_blwl sram_blwl_1811_ (mux_2level_size50_9_sram_blwl_out[1811:1811] ,mux_2level_size50_9_sram_blwl_out[1811:1811] ,mux_2level_size50_9_sram_blwl_outb[1811:1811] ,mux_2level_size50_9_configbus0[1811:1811], mux_2level_size50_9_configbus1[1811:1811] , mux_2level_size50_9_configbus0_b[1811:1811] );
sram6T_blwl sram_blwl_1812_ (mux_2level_size50_9_sram_blwl_out[1812:1812] ,mux_2level_size50_9_sram_blwl_out[1812:1812] ,mux_2level_size50_9_sram_blwl_outb[1812:1812] ,mux_2level_size50_9_configbus0[1812:1812], mux_2level_size50_9_configbus1[1812:1812] , mux_2level_size50_9_configbus0_b[1812:1812] );
sram6T_blwl sram_blwl_1813_ (mux_2level_size50_9_sram_blwl_out[1813:1813] ,mux_2level_size50_9_sram_blwl_out[1813:1813] ,mux_2level_size50_9_sram_blwl_outb[1813:1813] ,mux_2level_size50_9_configbus0[1813:1813], mux_2level_size50_9_configbus1[1813:1813] , mux_2level_size50_9_configbus0_b[1813:1813] );
sram6T_blwl sram_blwl_1814_ (mux_2level_size50_9_sram_blwl_out[1814:1814] ,mux_2level_size50_9_sram_blwl_out[1814:1814] ,mux_2level_size50_9_sram_blwl_outb[1814:1814] ,mux_2level_size50_9_configbus0[1814:1814], mux_2level_size50_9_configbus1[1814:1814] , mux_2level_size50_9_configbus0_b[1814:1814] );
sram6T_blwl sram_blwl_1815_ (mux_2level_size50_9_sram_blwl_out[1815:1815] ,mux_2level_size50_9_sram_blwl_out[1815:1815] ,mux_2level_size50_9_sram_blwl_outb[1815:1815] ,mux_2level_size50_9_configbus0[1815:1815], mux_2level_size50_9_configbus1[1815:1815] , mux_2level_size50_9_configbus0_b[1815:1815] );
sram6T_blwl sram_blwl_1816_ (mux_2level_size50_9_sram_blwl_out[1816:1816] ,mux_2level_size50_9_sram_blwl_out[1816:1816] ,mux_2level_size50_9_sram_blwl_outb[1816:1816] ,mux_2level_size50_9_configbus0[1816:1816], mux_2level_size50_9_configbus1[1816:1816] , mux_2level_size50_9_configbus0_b[1816:1816] );
sram6T_blwl sram_blwl_1817_ (mux_2level_size50_9_sram_blwl_out[1817:1817] ,mux_2level_size50_9_sram_blwl_out[1817:1817] ,mux_2level_size50_9_sram_blwl_outb[1817:1817] ,mux_2level_size50_9_configbus0[1817:1817], mux_2level_size50_9_configbus1[1817:1817] , mux_2level_size50_9_configbus0_b[1817:1817] );
sram6T_blwl sram_blwl_1818_ (mux_2level_size50_9_sram_blwl_out[1818:1818] ,mux_2level_size50_9_sram_blwl_out[1818:1818] ,mux_2level_size50_9_sram_blwl_outb[1818:1818] ,mux_2level_size50_9_configbus0[1818:1818], mux_2level_size50_9_configbus1[1818:1818] , mux_2level_size50_9_configbus0_b[1818:1818] );
sram6T_blwl sram_blwl_1819_ (mux_2level_size50_9_sram_blwl_out[1819:1819] ,mux_2level_size50_9_sram_blwl_out[1819:1819] ,mux_2level_size50_9_sram_blwl_outb[1819:1819] ,mux_2level_size50_9_configbus0[1819:1819], mux_2level_size50_9_configbus1[1819:1819] , mux_2level_size50_9_configbus0_b[1819:1819] );
sram6T_blwl sram_blwl_1820_ (mux_2level_size50_9_sram_blwl_out[1820:1820] ,mux_2level_size50_9_sram_blwl_out[1820:1820] ,mux_2level_size50_9_sram_blwl_outb[1820:1820] ,mux_2level_size50_9_configbus0[1820:1820], mux_2level_size50_9_configbus1[1820:1820] , mux_2level_size50_9_configbus0_b[1820:1820] );
sram6T_blwl sram_blwl_1821_ (mux_2level_size50_9_sram_blwl_out[1821:1821] ,mux_2level_size50_9_sram_blwl_out[1821:1821] ,mux_2level_size50_9_sram_blwl_outb[1821:1821] ,mux_2level_size50_9_configbus0[1821:1821], mux_2level_size50_9_configbus1[1821:1821] , mux_2level_size50_9_configbus0_b[1821:1821] );
sram6T_blwl sram_blwl_1822_ (mux_2level_size50_9_sram_blwl_out[1822:1822] ,mux_2level_size50_9_sram_blwl_out[1822:1822] ,mux_2level_size50_9_sram_blwl_outb[1822:1822] ,mux_2level_size50_9_configbus0[1822:1822], mux_2level_size50_9_configbus1[1822:1822] , mux_2level_size50_9_configbus0_b[1822:1822] );
sram6T_blwl sram_blwl_1823_ (mux_2level_size50_9_sram_blwl_out[1823:1823] ,mux_2level_size50_9_sram_blwl_out[1823:1823] ,mux_2level_size50_9_sram_blwl_outb[1823:1823] ,mux_2level_size50_9_configbus0[1823:1823], mux_2level_size50_9_configbus1[1823:1823] , mux_2level_size50_9_configbus0_b[1823:1823] );
sram6T_blwl sram_blwl_1824_ (mux_2level_size50_9_sram_blwl_out[1824:1824] ,mux_2level_size50_9_sram_blwl_out[1824:1824] ,mux_2level_size50_9_sram_blwl_outb[1824:1824] ,mux_2level_size50_9_configbus0[1824:1824], mux_2level_size50_9_configbus1[1824:1824] , mux_2level_size50_9_configbus0_b[1824:1824] );
sram6T_blwl sram_blwl_1825_ (mux_2level_size50_9_sram_blwl_out[1825:1825] ,mux_2level_size50_9_sram_blwl_out[1825:1825] ,mux_2level_size50_9_sram_blwl_outb[1825:1825] ,mux_2level_size50_9_configbus0[1825:1825], mux_2level_size50_9_configbus1[1825:1825] , mux_2level_size50_9_configbus0_b[1825:1825] );
wire [0:49] in_bus_mux_2level_size50_10_ ;
assign in_bus_mux_2level_size50_10_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_10_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_10_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_10_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_10_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_10_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_10_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_10_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_10_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_10_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_10_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_10_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_10_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_10_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_10_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_10_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_10_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_10_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_10_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_10_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_10_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_10_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_10_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_10_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_10_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_10_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_10_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_10_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_10_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_10_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_10_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_10_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_10_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_10_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_10_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_10_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_10_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_10_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_10_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_10_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_10_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_10_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_10_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_10_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_10_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_10_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_10_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_10_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_10_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_10_[49] = fle_9___out_0_ ; 
wire [1826:1841] mux_2level_size50_10_configbus0;
wire [1826:1841] mux_2level_size50_10_configbus1;
wire [1826:1841] mux_2level_size50_10_sram_blwl_out ;
wire [1826:1841] mux_2level_size50_10_sram_blwl_outb ;
assign mux_2level_size50_10_configbus0[1826:1841] = sram_blwl_bl[1826:1841] ;
assign mux_2level_size50_10_configbus1[1826:1841] = sram_blwl_wl[1826:1841] ;
wire [1826:1841] mux_2level_size50_10_configbus0_b;
assign mux_2level_size50_10_configbus0_b[1826:1841] = sram_blwl_blb[1826:1841] ;
mux_2level_size50 mux_2level_size50_10_ (in_bus_mux_2level_size50_10_, fle_1___in_4_, mux_2level_size50_10_sram_blwl_out[1826:1841] ,
mux_2level_size50_10_sram_blwl_outb[1826:1841] );
//----- SRAM bits for MUX[10], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1826_ (mux_2level_size50_10_sram_blwl_out[1826:1826] ,mux_2level_size50_10_sram_blwl_out[1826:1826] ,mux_2level_size50_10_sram_blwl_outb[1826:1826] ,mux_2level_size50_10_configbus0[1826:1826], mux_2level_size50_10_configbus1[1826:1826] , mux_2level_size50_10_configbus0_b[1826:1826] );
sram6T_blwl sram_blwl_1827_ (mux_2level_size50_10_sram_blwl_out[1827:1827] ,mux_2level_size50_10_sram_blwl_out[1827:1827] ,mux_2level_size50_10_sram_blwl_outb[1827:1827] ,mux_2level_size50_10_configbus0[1827:1827], mux_2level_size50_10_configbus1[1827:1827] , mux_2level_size50_10_configbus0_b[1827:1827] );
sram6T_blwl sram_blwl_1828_ (mux_2level_size50_10_sram_blwl_out[1828:1828] ,mux_2level_size50_10_sram_blwl_out[1828:1828] ,mux_2level_size50_10_sram_blwl_outb[1828:1828] ,mux_2level_size50_10_configbus0[1828:1828], mux_2level_size50_10_configbus1[1828:1828] , mux_2level_size50_10_configbus0_b[1828:1828] );
sram6T_blwl sram_blwl_1829_ (mux_2level_size50_10_sram_blwl_out[1829:1829] ,mux_2level_size50_10_sram_blwl_out[1829:1829] ,mux_2level_size50_10_sram_blwl_outb[1829:1829] ,mux_2level_size50_10_configbus0[1829:1829], mux_2level_size50_10_configbus1[1829:1829] , mux_2level_size50_10_configbus0_b[1829:1829] );
sram6T_blwl sram_blwl_1830_ (mux_2level_size50_10_sram_blwl_out[1830:1830] ,mux_2level_size50_10_sram_blwl_out[1830:1830] ,mux_2level_size50_10_sram_blwl_outb[1830:1830] ,mux_2level_size50_10_configbus0[1830:1830], mux_2level_size50_10_configbus1[1830:1830] , mux_2level_size50_10_configbus0_b[1830:1830] );
sram6T_blwl sram_blwl_1831_ (mux_2level_size50_10_sram_blwl_out[1831:1831] ,mux_2level_size50_10_sram_blwl_out[1831:1831] ,mux_2level_size50_10_sram_blwl_outb[1831:1831] ,mux_2level_size50_10_configbus0[1831:1831], mux_2level_size50_10_configbus1[1831:1831] , mux_2level_size50_10_configbus0_b[1831:1831] );
sram6T_blwl sram_blwl_1832_ (mux_2level_size50_10_sram_blwl_out[1832:1832] ,mux_2level_size50_10_sram_blwl_out[1832:1832] ,mux_2level_size50_10_sram_blwl_outb[1832:1832] ,mux_2level_size50_10_configbus0[1832:1832], mux_2level_size50_10_configbus1[1832:1832] , mux_2level_size50_10_configbus0_b[1832:1832] );
sram6T_blwl sram_blwl_1833_ (mux_2level_size50_10_sram_blwl_out[1833:1833] ,mux_2level_size50_10_sram_blwl_out[1833:1833] ,mux_2level_size50_10_sram_blwl_outb[1833:1833] ,mux_2level_size50_10_configbus0[1833:1833], mux_2level_size50_10_configbus1[1833:1833] , mux_2level_size50_10_configbus0_b[1833:1833] );
sram6T_blwl sram_blwl_1834_ (mux_2level_size50_10_sram_blwl_out[1834:1834] ,mux_2level_size50_10_sram_blwl_out[1834:1834] ,mux_2level_size50_10_sram_blwl_outb[1834:1834] ,mux_2level_size50_10_configbus0[1834:1834], mux_2level_size50_10_configbus1[1834:1834] , mux_2level_size50_10_configbus0_b[1834:1834] );
sram6T_blwl sram_blwl_1835_ (mux_2level_size50_10_sram_blwl_out[1835:1835] ,mux_2level_size50_10_sram_blwl_out[1835:1835] ,mux_2level_size50_10_sram_blwl_outb[1835:1835] ,mux_2level_size50_10_configbus0[1835:1835], mux_2level_size50_10_configbus1[1835:1835] , mux_2level_size50_10_configbus0_b[1835:1835] );
sram6T_blwl sram_blwl_1836_ (mux_2level_size50_10_sram_blwl_out[1836:1836] ,mux_2level_size50_10_sram_blwl_out[1836:1836] ,mux_2level_size50_10_sram_blwl_outb[1836:1836] ,mux_2level_size50_10_configbus0[1836:1836], mux_2level_size50_10_configbus1[1836:1836] , mux_2level_size50_10_configbus0_b[1836:1836] );
sram6T_blwl sram_blwl_1837_ (mux_2level_size50_10_sram_blwl_out[1837:1837] ,mux_2level_size50_10_sram_blwl_out[1837:1837] ,mux_2level_size50_10_sram_blwl_outb[1837:1837] ,mux_2level_size50_10_configbus0[1837:1837], mux_2level_size50_10_configbus1[1837:1837] , mux_2level_size50_10_configbus0_b[1837:1837] );
sram6T_blwl sram_blwl_1838_ (mux_2level_size50_10_sram_blwl_out[1838:1838] ,mux_2level_size50_10_sram_blwl_out[1838:1838] ,mux_2level_size50_10_sram_blwl_outb[1838:1838] ,mux_2level_size50_10_configbus0[1838:1838], mux_2level_size50_10_configbus1[1838:1838] , mux_2level_size50_10_configbus0_b[1838:1838] );
sram6T_blwl sram_blwl_1839_ (mux_2level_size50_10_sram_blwl_out[1839:1839] ,mux_2level_size50_10_sram_blwl_out[1839:1839] ,mux_2level_size50_10_sram_blwl_outb[1839:1839] ,mux_2level_size50_10_configbus0[1839:1839], mux_2level_size50_10_configbus1[1839:1839] , mux_2level_size50_10_configbus0_b[1839:1839] );
sram6T_blwl sram_blwl_1840_ (mux_2level_size50_10_sram_blwl_out[1840:1840] ,mux_2level_size50_10_sram_blwl_out[1840:1840] ,mux_2level_size50_10_sram_blwl_outb[1840:1840] ,mux_2level_size50_10_configbus0[1840:1840], mux_2level_size50_10_configbus1[1840:1840] , mux_2level_size50_10_configbus0_b[1840:1840] );
sram6T_blwl sram_blwl_1841_ (mux_2level_size50_10_sram_blwl_out[1841:1841] ,mux_2level_size50_10_sram_blwl_out[1841:1841] ,mux_2level_size50_10_sram_blwl_outb[1841:1841] ,mux_2level_size50_10_configbus0[1841:1841], mux_2level_size50_10_configbus1[1841:1841] , mux_2level_size50_10_configbus0_b[1841:1841] );
wire [0:49] in_bus_mux_2level_size50_11_ ;
assign in_bus_mux_2level_size50_11_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_11_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_11_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_11_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_11_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_11_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_11_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_11_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_11_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_11_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_11_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_11_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_11_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_11_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_11_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_11_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_11_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_11_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_11_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_11_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_11_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_11_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_11_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_11_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_11_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_11_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_11_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_11_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_11_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_11_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_11_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_11_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_11_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_11_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_11_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_11_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_11_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_11_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_11_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_11_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_11_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_11_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_11_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_11_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_11_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_11_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_11_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_11_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_11_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_11_[49] = fle_9___out_0_ ; 
wire [1842:1857] mux_2level_size50_11_configbus0;
wire [1842:1857] mux_2level_size50_11_configbus1;
wire [1842:1857] mux_2level_size50_11_sram_blwl_out ;
wire [1842:1857] mux_2level_size50_11_sram_blwl_outb ;
assign mux_2level_size50_11_configbus0[1842:1857] = sram_blwl_bl[1842:1857] ;
assign mux_2level_size50_11_configbus1[1842:1857] = sram_blwl_wl[1842:1857] ;
wire [1842:1857] mux_2level_size50_11_configbus0_b;
assign mux_2level_size50_11_configbus0_b[1842:1857] = sram_blwl_blb[1842:1857] ;
mux_2level_size50 mux_2level_size50_11_ (in_bus_mux_2level_size50_11_, fle_1___in_5_, mux_2level_size50_11_sram_blwl_out[1842:1857] ,
mux_2level_size50_11_sram_blwl_outb[1842:1857] );
//----- SRAM bits for MUX[11], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1842_ (mux_2level_size50_11_sram_blwl_out[1842:1842] ,mux_2level_size50_11_sram_blwl_out[1842:1842] ,mux_2level_size50_11_sram_blwl_outb[1842:1842] ,mux_2level_size50_11_configbus0[1842:1842], mux_2level_size50_11_configbus1[1842:1842] , mux_2level_size50_11_configbus0_b[1842:1842] );
sram6T_blwl sram_blwl_1843_ (mux_2level_size50_11_sram_blwl_out[1843:1843] ,mux_2level_size50_11_sram_blwl_out[1843:1843] ,mux_2level_size50_11_sram_blwl_outb[1843:1843] ,mux_2level_size50_11_configbus0[1843:1843], mux_2level_size50_11_configbus1[1843:1843] , mux_2level_size50_11_configbus0_b[1843:1843] );
sram6T_blwl sram_blwl_1844_ (mux_2level_size50_11_sram_blwl_out[1844:1844] ,mux_2level_size50_11_sram_blwl_out[1844:1844] ,mux_2level_size50_11_sram_blwl_outb[1844:1844] ,mux_2level_size50_11_configbus0[1844:1844], mux_2level_size50_11_configbus1[1844:1844] , mux_2level_size50_11_configbus0_b[1844:1844] );
sram6T_blwl sram_blwl_1845_ (mux_2level_size50_11_sram_blwl_out[1845:1845] ,mux_2level_size50_11_sram_blwl_out[1845:1845] ,mux_2level_size50_11_sram_blwl_outb[1845:1845] ,mux_2level_size50_11_configbus0[1845:1845], mux_2level_size50_11_configbus1[1845:1845] , mux_2level_size50_11_configbus0_b[1845:1845] );
sram6T_blwl sram_blwl_1846_ (mux_2level_size50_11_sram_blwl_out[1846:1846] ,mux_2level_size50_11_sram_blwl_out[1846:1846] ,mux_2level_size50_11_sram_blwl_outb[1846:1846] ,mux_2level_size50_11_configbus0[1846:1846], mux_2level_size50_11_configbus1[1846:1846] , mux_2level_size50_11_configbus0_b[1846:1846] );
sram6T_blwl sram_blwl_1847_ (mux_2level_size50_11_sram_blwl_out[1847:1847] ,mux_2level_size50_11_sram_blwl_out[1847:1847] ,mux_2level_size50_11_sram_blwl_outb[1847:1847] ,mux_2level_size50_11_configbus0[1847:1847], mux_2level_size50_11_configbus1[1847:1847] , mux_2level_size50_11_configbus0_b[1847:1847] );
sram6T_blwl sram_blwl_1848_ (mux_2level_size50_11_sram_blwl_out[1848:1848] ,mux_2level_size50_11_sram_blwl_out[1848:1848] ,mux_2level_size50_11_sram_blwl_outb[1848:1848] ,mux_2level_size50_11_configbus0[1848:1848], mux_2level_size50_11_configbus1[1848:1848] , mux_2level_size50_11_configbus0_b[1848:1848] );
sram6T_blwl sram_blwl_1849_ (mux_2level_size50_11_sram_blwl_out[1849:1849] ,mux_2level_size50_11_sram_blwl_out[1849:1849] ,mux_2level_size50_11_sram_blwl_outb[1849:1849] ,mux_2level_size50_11_configbus0[1849:1849], mux_2level_size50_11_configbus1[1849:1849] , mux_2level_size50_11_configbus0_b[1849:1849] );
sram6T_blwl sram_blwl_1850_ (mux_2level_size50_11_sram_blwl_out[1850:1850] ,mux_2level_size50_11_sram_blwl_out[1850:1850] ,mux_2level_size50_11_sram_blwl_outb[1850:1850] ,mux_2level_size50_11_configbus0[1850:1850], mux_2level_size50_11_configbus1[1850:1850] , mux_2level_size50_11_configbus0_b[1850:1850] );
sram6T_blwl sram_blwl_1851_ (mux_2level_size50_11_sram_blwl_out[1851:1851] ,mux_2level_size50_11_sram_blwl_out[1851:1851] ,mux_2level_size50_11_sram_blwl_outb[1851:1851] ,mux_2level_size50_11_configbus0[1851:1851], mux_2level_size50_11_configbus1[1851:1851] , mux_2level_size50_11_configbus0_b[1851:1851] );
sram6T_blwl sram_blwl_1852_ (mux_2level_size50_11_sram_blwl_out[1852:1852] ,mux_2level_size50_11_sram_blwl_out[1852:1852] ,mux_2level_size50_11_sram_blwl_outb[1852:1852] ,mux_2level_size50_11_configbus0[1852:1852], mux_2level_size50_11_configbus1[1852:1852] , mux_2level_size50_11_configbus0_b[1852:1852] );
sram6T_blwl sram_blwl_1853_ (mux_2level_size50_11_sram_blwl_out[1853:1853] ,mux_2level_size50_11_sram_blwl_out[1853:1853] ,mux_2level_size50_11_sram_blwl_outb[1853:1853] ,mux_2level_size50_11_configbus0[1853:1853], mux_2level_size50_11_configbus1[1853:1853] , mux_2level_size50_11_configbus0_b[1853:1853] );
sram6T_blwl sram_blwl_1854_ (mux_2level_size50_11_sram_blwl_out[1854:1854] ,mux_2level_size50_11_sram_blwl_out[1854:1854] ,mux_2level_size50_11_sram_blwl_outb[1854:1854] ,mux_2level_size50_11_configbus0[1854:1854], mux_2level_size50_11_configbus1[1854:1854] , mux_2level_size50_11_configbus0_b[1854:1854] );
sram6T_blwl sram_blwl_1855_ (mux_2level_size50_11_sram_blwl_out[1855:1855] ,mux_2level_size50_11_sram_blwl_out[1855:1855] ,mux_2level_size50_11_sram_blwl_outb[1855:1855] ,mux_2level_size50_11_configbus0[1855:1855], mux_2level_size50_11_configbus1[1855:1855] , mux_2level_size50_11_configbus0_b[1855:1855] );
sram6T_blwl sram_blwl_1856_ (mux_2level_size50_11_sram_blwl_out[1856:1856] ,mux_2level_size50_11_sram_blwl_out[1856:1856] ,mux_2level_size50_11_sram_blwl_outb[1856:1856] ,mux_2level_size50_11_configbus0[1856:1856], mux_2level_size50_11_configbus1[1856:1856] , mux_2level_size50_11_configbus0_b[1856:1856] );
sram6T_blwl sram_blwl_1857_ (mux_2level_size50_11_sram_blwl_out[1857:1857] ,mux_2level_size50_11_sram_blwl_out[1857:1857] ,mux_2level_size50_11_sram_blwl_outb[1857:1857] ,mux_2level_size50_11_configbus0[1857:1857], mux_2level_size50_11_configbus1[1857:1857] , mux_2level_size50_11_configbus0_b[1857:1857] );
direct_interc direct_interc_171_ (mode_clb___clk_0_, fle_1___clk_0_ );
wire [0:49] in_bus_mux_2level_size50_12_ ;
assign in_bus_mux_2level_size50_12_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_12_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_12_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_12_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_12_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_12_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_12_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_12_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_12_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_12_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_12_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_12_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_12_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_12_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_12_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_12_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_12_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_12_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_12_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_12_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_12_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_12_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_12_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_12_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_12_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_12_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_12_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_12_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_12_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_12_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_12_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_12_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_12_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_12_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_12_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_12_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_12_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_12_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_12_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_12_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_12_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_12_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_12_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_12_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_12_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_12_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_12_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_12_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_12_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_12_[49] = fle_9___out_0_ ; 
wire [1858:1873] mux_2level_size50_12_configbus0;
wire [1858:1873] mux_2level_size50_12_configbus1;
wire [1858:1873] mux_2level_size50_12_sram_blwl_out ;
wire [1858:1873] mux_2level_size50_12_sram_blwl_outb ;
assign mux_2level_size50_12_configbus0[1858:1873] = sram_blwl_bl[1858:1873] ;
assign mux_2level_size50_12_configbus1[1858:1873] = sram_blwl_wl[1858:1873] ;
wire [1858:1873] mux_2level_size50_12_configbus0_b;
assign mux_2level_size50_12_configbus0_b[1858:1873] = sram_blwl_blb[1858:1873] ;
mux_2level_size50 mux_2level_size50_12_ (in_bus_mux_2level_size50_12_, fle_2___in_0_, mux_2level_size50_12_sram_blwl_out[1858:1873] ,
mux_2level_size50_12_sram_blwl_outb[1858:1873] );
//----- SRAM bits for MUX[12], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1858_ (mux_2level_size50_12_sram_blwl_out[1858:1858] ,mux_2level_size50_12_sram_blwl_out[1858:1858] ,mux_2level_size50_12_sram_blwl_outb[1858:1858] ,mux_2level_size50_12_configbus0[1858:1858], mux_2level_size50_12_configbus1[1858:1858] , mux_2level_size50_12_configbus0_b[1858:1858] );
sram6T_blwl sram_blwl_1859_ (mux_2level_size50_12_sram_blwl_out[1859:1859] ,mux_2level_size50_12_sram_blwl_out[1859:1859] ,mux_2level_size50_12_sram_blwl_outb[1859:1859] ,mux_2level_size50_12_configbus0[1859:1859], mux_2level_size50_12_configbus1[1859:1859] , mux_2level_size50_12_configbus0_b[1859:1859] );
sram6T_blwl sram_blwl_1860_ (mux_2level_size50_12_sram_blwl_out[1860:1860] ,mux_2level_size50_12_sram_blwl_out[1860:1860] ,mux_2level_size50_12_sram_blwl_outb[1860:1860] ,mux_2level_size50_12_configbus0[1860:1860], mux_2level_size50_12_configbus1[1860:1860] , mux_2level_size50_12_configbus0_b[1860:1860] );
sram6T_blwl sram_blwl_1861_ (mux_2level_size50_12_sram_blwl_out[1861:1861] ,mux_2level_size50_12_sram_blwl_out[1861:1861] ,mux_2level_size50_12_sram_blwl_outb[1861:1861] ,mux_2level_size50_12_configbus0[1861:1861], mux_2level_size50_12_configbus1[1861:1861] , mux_2level_size50_12_configbus0_b[1861:1861] );
sram6T_blwl sram_blwl_1862_ (mux_2level_size50_12_sram_blwl_out[1862:1862] ,mux_2level_size50_12_sram_blwl_out[1862:1862] ,mux_2level_size50_12_sram_blwl_outb[1862:1862] ,mux_2level_size50_12_configbus0[1862:1862], mux_2level_size50_12_configbus1[1862:1862] , mux_2level_size50_12_configbus0_b[1862:1862] );
sram6T_blwl sram_blwl_1863_ (mux_2level_size50_12_sram_blwl_out[1863:1863] ,mux_2level_size50_12_sram_blwl_out[1863:1863] ,mux_2level_size50_12_sram_blwl_outb[1863:1863] ,mux_2level_size50_12_configbus0[1863:1863], mux_2level_size50_12_configbus1[1863:1863] , mux_2level_size50_12_configbus0_b[1863:1863] );
sram6T_blwl sram_blwl_1864_ (mux_2level_size50_12_sram_blwl_out[1864:1864] ,mux_2level_size50_12_sram_blwl_out[1864:1864] ,mux_2level_size50_12_sram_blwl_outb[1864:1864] ,mux_2level_size50_12_configbus0[1864:1864], mux_2level_size50_12_configbus1[1864:1864] , mux_2level_size50_12_configbus0_b[1864:1864] );
sram6T_blwl sram_blwl_1865_ (mux_2level_size50_12_sram_blwl_out[1865:1865] ,mux_2level_size50_12_sram_blwl_out[1865:1865] ,mux_2level_size50_12_sram_blwl_outb[1865:1865] ,mux_2level_size50_12_configbus0[1865:1865], mux_2level_size50_12_configbus1[1865:1865] , mux_2level_size50_12_configbus0_b[1865:1865] );
sram6T_blwl sram_blwl_1866_ (mux_2level_size50_12_sram_blwl_out[1866:1866] ,mux_2level_size50_12_sram_blwl_out[1866:1866] ,mux_2level_size50_12_sram_blwl_outb[1866:1866] ,mux_2level_size50_12_configbus0[1866:1866], mux_2level_size50_12_configbus1[1866:1866] , mux_2level_size50_12_configbus0_b[1866:1866] );
sram6T_blwl sram_blwl_1867_ (mux_2level_size50_12_sram_blwl_out[1867:1867] ,mux_2level_size50_12_sram_blwl_out[1867:1867] ,mux_2level_size50_12_sram_blwl_outb[1867:1867] ,mux_2level_size50_12_configbus0[1867:1867], mux_2level_size50_12_configbus1[1867:1867] , mux_2level_size50_12_configbus0_b[1867:1867] );
sram6T_blwl sram_blwl_1868_ (mux_2level_size50_12_sram_blwl_out[1868:1868] ,mux_2level_size50_12_sram_blwl_out[1868:1868] ,mux_2level_size50_12_sram_blwl_outb[1868:1868] ,mux_2level_size50_12_configbus0[1868:1868], mux_2level_size50_12_configbus1[1868:1868] , mux_2level_size50_12_configbus0_b[1868:1868] );
sram6T_blwl sram_blwl_1869_ (mux_2level_size50_12_sram_blwl_out[1869:1869] ,mux_2level_size50_12_sram_blwl_out[1869:1869] ,mux_2level_size50_12_sram_blwl_outb[1869:1869] ,mux_2level_size50_12_configbus0[1869:1869], mux_2level_size50_12_configbus1[1869:1869] , mux_2level_size50_12_configbus0_b[1869:1869] );
sram6T_blwl sram_blwl_1870_ (mux_2level_size50_12_sram_blwl_out[1870:1870] ,mux_2level_size50_12_sram_blwl_out[1870:1870] ,mux_2level_size50_12_sram_blwl_outb[1870:1870] ,mux_2level_size50_12_configbus0[1870:1870], mux_2level_size50_12_configbus1[1870:1870] , mux_2level_size50_12_configbus0_b[1870:1870] );
sram6T_blwl sram_blwl_1871_ (mux_2level_size50_12_sram_blwl_out[1871:1871] ,mux_2level_size50_12_sram_blwl_out[1871:1871] ,mux_2level_size50_12_sram_blwl_outb[1871:1871] ,mux_2level_size50_12_configbus0[1871:1871], mux_2level_size50_12_configbus1[1871:1871] , mux_2level_size50_12_configbus0_b[1871:1871] );
sram6T_blwl sram_blwl_1872_ (mux_2level_size50_12_sram_blwl_out[1872:1872] ,mux_2level_size50_12_sram_blwl_out[1872:1872] ,mux_2level_size50_12_sram_blwl_outb[1872:1872] ,mux_2level_size50_12_configbus0[1872:1872], mux_2level_size50_12_configbus1[1872:1872] , mux_2level_size50_12_configbus0_b[1872:1872] );
sram6T_blwl sram_blwl_1873_ (mux_2level_size50_12_sram_blwl_out[1873:1873] ,mux_2level_size50_12_sram_blwl_out[1873:1873] ,mux_2level_size50_12_sram_blwl_outb[1873:1873] ,mux_2level_size50_12_configbus0[1873:1873], mux_2level_size50_12_configbus1[1873:1873] , mux_2level_size50_12_configbus0_b[1873:1873] );
wire [0:49] in_bus_mux_2level_size50_13_ ;
assign in_bus_mux_2level_size50_13_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_13_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_13_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_13_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_13_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_13_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_13_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_13_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_13_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_13_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_13_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_13_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_13_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_13_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_13_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_13_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_13_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_13_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_13_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_13_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_13_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_13_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_13_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_13_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_13_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_13_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_13_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_13_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_13_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_13_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_13_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_13_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_13_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_13_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_13_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_13_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_13_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_13_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_13_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_13_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_13_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_13_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_13_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_13_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_13_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_13_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_13_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_13_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_13_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_13_[49] = fle_9___out_0_ ; 
wire [1874:1889] mux_2level_size50_13_configbus0;
wire [1874:1889] mux_2level_size50_13_configbus1;
wire [1874:1889] mux_2level_size50_13_sram_blwl_out ;
wire [1874:1889] mux_2level_size50_13_sram_blwl_outb ;
assign mux_2level_size50_13_configbus0[1874:1889] = sram_blwl_bl[1874:1889] ;
assign mux_2level_size50_13_configbus1[1874:1889] = sram_blwl_wl[1874:1889] ;
wire [1874:1889] mux_2level_size50_13_configbus0_b;
assign mux_2level_size50_13_configbus0_b[1874:1889] = sram_blwl_blb[1874:1889] ;
mux_2level_size50 mux_2level_size50_13_ (in_bus_mux_2level_size50_13_, fle_2___in_1_, mux_2level_size50_13_sram_blwl_out[1874:1889] ,
mux_2level_size50_13_sram_blwl_outb[1874:1889] );
//----- SRAM bits for MUX[13], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1874_ (mux_2level_size50_13_sram_blwl_out[1874:1874] ,mux_2level_size50_13_sram_blwl_out[1874:1874] ,mux_2level_size50_13_sram_blwl_outb[1874:1874] ,mux_2level_size50_13_configbus0[1874:1874], mux_2level_size50_13_configbus1[1874:1874] , mux_2level_size50_13_configbus0_b[1874:1874] );
sram6T_blwl sram_blwl_1875_ (mux_2level_size50_13_sram_blwl_out[1875:1875] ,mux_2level_size50_13_sram_blwl_out[1875:1875] ,mux_2level_size50_13_sram_blwl_outb[1875:1875] ,mux_2level_size50_13_configbus0[1875:1875], mux_2level_size50_13_configbus1[1875:1875] , mux_2level_size50_13_configbus0_b[1875:1875] );
sram6T_blwl sram_blwl_1876_ (mux_2level_size50_13_sram_blwl_out[1876:1876] ,mux_2level_size50_13_sram_blwl_out[1876:1876] ,mux_2level_size50_13_sram_blwl_outb[1876:1876] ,mux_2level_size50_13_configbus0[1876:1876], mux_2level_size50_13_configbus1[1876:1876] , mux_2level_size50_13_configbus0_b[1876:1876] );
sram6T_blwl sram_blwl_1877_ (mux_2level_size50_13_sram_blwl_out[1877:1877] ,mux_2level_size50_13_sram_blwl_out[1877:1877] ,mux_2level_size50_13_sram_blwl_outb[1877:1877] ,mux_2level_size50_13_configbus0[1877:1877], mux_2level_size50_13_configbus1[1877:1877] , mux_2level_size50_13_configbus0_b[1877:1877] );
sram6T_blwl sram_blwl_1878_ (mux_2level_size50_13_sram_blwl_out[1878:1878] ,mux_2level_size50_13_sram_blwl_out[1878:1878] ,mux_2level_size50_13_sram_blwl_outb[1878:1878] ,mux_2level_size50_13_configbus0[1878:1878], mux_2level_size50_13_configbus1[1878:1878] , mux_2level_size50_13_configbus0_b[1878:1878] );
sram6T_blwl sram_blwl_1879_ (mux_2level_size50_13_sram_blwl_out[1879:1879] ,mux_2level_size50_13_sram_blwl_out[1879:1879] ,mux_2level_size50_13_sram_blwl_outb[1879:1879] ,mux_2level_size50_13_configbus0[1879:1879], mux_2level_size50_13_configbus1[1879:1879] , mux_2level_size50_13_configbus0_b[1879:1879] );
sram6T_blwl sram_blwl_1880_ (mux_2level_size50_13_sram_blwl_out[1880:1880] ,mux_2level_size50_13_sram_blwl_out[1880:1880] ,mux_2level_size50_13_sram_blwl_outb[1880:1880] ,mux_2level_size50_13_configbus0[1880:1880], mux_2level_size50_13_configbus1[1880:1880] , mux_2level_size50_13_configbus0_b[1880:1880] );
sram6T_blwl sram_blwl_1881_ (mux_2level_size50_13_sram_blwl_out[1881:1881] ,mux_2level_size50_13_sram_blwl_out[1881:1881] ,mux_2level_size50_13_sram_blwl_outb[1881:1881] ,mux_2level_size50_13_configbus0[1881:1881], mux_2level_size50_13_configbus1[1881:1881] , mux_2level_size50_13_configbus0_b[1881:1881] );
sram6T_blwl sram_blwl_1882_ (mux_2level_size50_13_sram_blwl_out[1882:1882] ,mux_2level_size50_13_sram_blwl_out[1882:1882] ,mux_2level_size50_13_sram_blwl_outb[1882:1882] ,mux_2level_size50_13_configbus0[1882:1882], mux_2level_size50_13_configbus1[1882:1882] , mux_2level_size50_13_configbus0_b[1882:1882] );
sram6T_blwl sram_blwl_1883_ (mux_2level_size50_13_sram_blwl_out[1883:1883] ,mux_2level_size50_13_sram_blwl_out[1883:1883] ,mux_2level_size50_13_sram_blwl_outb[1883:1883] ,mux_2level_size50_13_configbus0[1883:1883], mux_2level_size50_13_configbus1[1883:1883] , mux_2level_size50_13_configbus0_b[1883:1883] );
sram6T_blwl sram_blwl_1884_ (mux_2level_size50_13_sram_blwl_out[1884:1884] ,mux_2level_size50_13_sram_blwl_out[1884:1884] ,mux_2level_size50_13_sram_blwl_outb[1884:1884] ,mux_2level_size50_13_configbus0[1884:1884], mux_2level_size50_13_configbus1[1884:1884] , mux_2level_size50_13_configbus0_b[1884:1884] );
sram6T_blwl sram_blwl_1885_ (mux_2level_size50_13_sram_blwl_out[1885:1885] ,mux_2level_size50_13_sram_blwl_out[1885:1885] ,mux_2level_size50_13_sram_blwl_outb[1885:1885] ,mux_2level_size50_13_configbus0[1885:1885], mux_2level_size50_13_configbus1[1885:1885] , mux_2level_size50_13_configbus0_b[1885:1885] );
sram6T_blwl sram_blwl_1886_ (mux_2level_size50_13_sram_blwl_out[1886:1886] ,mux_2level_size50_13_sram_blwl_out[1886:1886] ,mux_2level_size50_13_sram_blwl_outb[1886:1886] ,mux_2level_size50_13_configbus0[1886:1886], mux_2level_size50_13_configbus1[1886:1886] , mux_2level_size50_13_configbus0_b[1886:1886] );
sram6T_blwl sram_blwl_1887_ (mux_2level_size50_13_sram_blwl_out[1887:1887] ,mux_2level_size50_13_sram_blwl_out[1887:1887] ,mux_2level_size50_13_sram_blwl_outb[1887:1887] ,mux_2level_size50_13_configbus0[1887:1887], mux_2level_size50_13_configbus1[1887:1887] , mux_2level_size50_13_configbus0_b[1887:1887] );
sram6T_blwl sram_blwl_1888_ (mux_2level_size50_13_sram_blwl_out[1888:1888] ,mux_2level_size50_13_sram_blwl_out[1888:1888] ,mux_2level_size50_13_sram_blwl_outb[1888:1888] ,mux_2level_size50_13_configbus0[1888:1888], mux_2level_size50_13_configbus1[1888:1888] , mux_2level_size50_13_configbus0_b[1888:1888] );
sram6T_blwl sram_blwl_1889_ (mux_2level_size50_13_sram_blwl_out[1889:1889] ,mux_2level_size50_13_sram_blwl_out[1889:1889] ,mux_2level_size50_13_sram_blwl_outb[1889:1889] ,mux_2level_size50_13_configbus0[1889:1889], mux_2level_size50_13_configbus1[1889:1889] , mux_2level_size50_13_configbus0_b[1889:1889] );
wire [0:49] in_bus_mux_2level_size50_14_ ;
assign in_bus_mux_2level_size50_14_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_14_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_14_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_14_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_14_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_14_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_14_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_14_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_14_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_14_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_14_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_14_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_14_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_14_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_14_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_14_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_14_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_14_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_14_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_14_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_14_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_14_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_14_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_14_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_14_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_14_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_14_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_14_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_14_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_14_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_14_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_14_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_14_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_14_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_14_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_14_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_14_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_14_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_14_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_14_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_14_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_14_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_14_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_14_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_14_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_14_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_14_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_14_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_14_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_14_[49] = fle_9___out_0_ ; 
wire [1890:1905] mux_2level_size50_14_configbus0;
wire [1890:1905] mux_2level_size50_14_configbus1;
wire [1890:1905] mux_2level_size50_14_sram_blwl_out ;
wire [1890:1905] mux_2level_size50_14_sram_blwl_outb ;
assign mux_2level_size50_14_configbus0[1890:1905] = sram_blwl_bl[1890:1905] ;
assign mux_2level_size50_14_configbus1[1890:1905] = sram_blwl_wl[1890:1905] ;
wire [1890:1905] mux_2level_size50_14_configbus0_b;
assign mux_2level_size50_14_configbus0_b[1890:1905] = sram_blwl_blb[1890:1905] ;
mux_2level_size50 mux_2level_size50_14_ (in_bus_mux_2level_size50_14_, fle_2___in_2_, mux_2level_size50_14_sram_blwl_out[1890:1905] ,
mux_2level_size50_14_sram_blwl_outb[1890:1905] );
//----- SRAM bits for MUX[14], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1890_ (mux_2level_size50_14_sram_blwl_out[1890:1890] ,mux_2level_size50_14_sram_blwl_out[1890:1890] ,mux_2level_size50_14_sram_blwl_outb[1890:1890] ,mux_2level_size50_14_configbus0[1890:1890], mux_2level_size50_14_configbus1[1890:1890] , mux_2level_size50_14_configbus0_b[1890:1890] );
sram6T_blwl sram_blwl_1891_ (mux_2level_size50_14_sram_blwl_out[1891:1891] ,mux_2level_size50_14_sram_blwl_out[1891:1891] ,mux_2level_size50_14_sram_blwl_outb[1891:1891] ,mux_2level_size50_14_configbus0[1891:1891], mux_2level_size50_14_configbus1[1891:1891] , mux_2level_size50_14_configbus0_b[1891:1891] );
sram6T_blwl sram_blwl_1892_ (mux_2level_size50_14_sram_blwl_out[1892:1892] ,mux_2level_size50_14_sram_blwl_out[1892:1892] ,mux_2level_size50_14_sram_blwl_outb[1892:1892] ,mux_2level_size50_14_configbus0[1892:1892], mux_2level_size50_14_configbus1[1892:1892] , mux_2level_size50_14_configbus0_b[1892:1892] );
sram6T_blwl sram_blwl_1893_ (mux_2level_size50_14_sram_blwl_out[1893:1893] ,mux_2level_size50_14_sram_blwl_out[1893:1893] ,mux_2level_size50_14_sram_blwl_outb[1893:1893] ,mux_2level_size50_14_configbus0[1893:1893], mux_2level_size50_14_configbus1[1893:1893] , mux_2level_size50_14_configbus0_b[1893:1893] );
sram6T_blwl sram_blwl_1894_ (mux_2level_size50_14_sram_blwl_out[1894:1894] ,mux_2level_size50_14_sram_blwl_out[1894:1894] ,mux_2level_size50_14_sram_blwl_outb[1894:1894] ,mux_2level_size50_14_configbus0[1894:1894], mux_2level_size50_14_configbus1[1894:1894] , mux_2level_size50_14_configbus0_b[1894:1894] );
sram6T_blwl sram_blwl_1895_ (mux_2level_size50_14_sram_blwl_out[1895:1895] ,mux_2level_size50_14_sram_blwl_out[1895:1895] ,mux_2level_size50_14_sram_blwl_outb[1895:1895] ,mux_2level_size50_14_configbus0[1895:1895], mux_2level_size50_14_configbus1[1895:1895] , mux_2level_size50_14_configbus0_b[1895:1895] );
sram6T_blwl sram_blwl_1896_ (mux_2level_size50_14_sram_blwl_out[1896:1896] ,mux_2level_size50_14_sram_blwl_out[1896:1896] ,mux_2level_size50_14_sram_blwl_outb[1896:1896] ,mux_2level_size50_14_configbus0[1896:1896], mux_2level_size50_14_configbus1[1896:1896] , mux_2level_size50_14_configbus0_b[1896:1896] );
sram6T_blwl sram_blwl_1897_ (mux_2level_size50_14_sram_blwl_out[1897:1897] ,mux_2level_size50_14_sram_blwl_out[1897:1897] ,mux_2level_size50_14_sram_blwl_outb[1897:1897] ,mux_2level_size50_14_configbus0[1897:1897], mux_2level_size50_14_configbus1[1897:1897] , mux_2level_size50_14_configbus0_b[1897:1897] );
sram6T_blwl sram_blwl_1898_ (mux_2level_size50_14_sram_blwl_out[1898:1898] ,mux_2level_size50_14_sram_blwl_out[1898:1898] ,mux_2level_size50_14_sram_blwl_outb[1898:1898] ,mux_2level_size50_14_configbus0[1898:1898], mux_2level_size50_14_configbus1[1898:1898] , mux_2level_size50_14_configbus0_b[1898:1898] );
sram6T_blwl sram_blwl_1899_ (mux_2level_size50_14_sram_blwl_out[1899:1899] ,mux_2level_size50_14_sram_blwl_out[1899:1899] ,mux_2level_size50_14_sram_blwl_outb[1899:1899] ,mux_2level_size50_14_configbus0[1899:1899], mux_2level_size50_14_configbus1[1899:1899] , mux_2level_size50_14_configbus0_b[1899:1899] );
sram6T_blwl sram_blwl_1900_ (mux_2level_size50_14_sram_blwl_out[1900:1900] ,mux_2level_size50_14_sram_blwl_out[1900:1900] ,mux_2level_size50_14_sram_blwl_outb[1900:1900] ,mux_2level_size50_14_configbus0[1900:1900], mux_2level_size50_14_configbus1[1900:1900] , mux_2level_size50_14_configbus0_b[1900:1900] );
sram6T_blwl sram_blwl_1901_ (mux_2level_size50_14_sram_blwl_out[1901:1901] ,mux_2level_size50_14_sram_blwl_out[1901:1901] ,mux_2level_size50_14_sram_blwl_outb[1901:1901] ,mux_2level_size50_14_configbus0[1901:1901], mux_2level_size50_14_configbus1[1901:1901] , mux_2level_size50_14_configbus0_b[1901:1901] );
sram6T_blwl sram_blwl_1902_ (mux_2level_size50_14_sram_blwl_out[1902:1902] ,mux_2level_size50_14_sram_blwl_out[1902:1902] ,mux_2level_size50_14_sram_blwl_outb[1902:1902] ,mux_2level_size50_14_configbus0[1902:1902], mux_2level_size50_14_configbus1[1902:1902] , mux_2level_size50_14_configbus0_b[1902:1902] );
sram6T_blwl sram_blwl_1903_ (mux_2level_size50_14_sram_blwl_out[1903:1903] ,mux_2level_size50_14_sram_blwl_out[1903:1903] ,mux_2level_size50_14_sram_blwl_outb[1903:1903] ,mux_2level_size50_14_configbus0[1903:1903], mux_2level_size50_14_configbus1[1903:1903] , mux_2level_size50_14_configbus0_b[1903:1903] );
sram6T_blwl sram_blwl_1904_ (mux_2level_size50_14_sram_blwl_out[1904:1904] ,mux_2level_size50_14_sram_blwl_out[1904:1904] ,mux_2level_size50_14_sram_blwl_outb[1904:1904] ,mux_2level_size50_14_configbus0[1904:1904], mux_2level_size50_14_configbus1[1904:1904] , mux_2level_size50_14_configbus0_b[1904:1904] );
sram6T_blwl sram_blwl_1905_ (mux_2level_size50_14_sram_blwl_out[1905:1905] ,mux_2level_size50_14_sram_blwl_out[1905:1905] ,mux_2level_size50_14_sram_blwl_outb[1905:1905] ,mux_2level_size50_14_configbus0[1905:1905], mux_2level_size50_14_configbus1[1905:1905] , mux_2level_size50_14_configbus0_b[1905:1905] );
wire [0:49] in_bus_mux_2level_size50_15_ ;
assign in_bus_mux_2level_size50_15_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_15_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_15_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_15_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_15_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_15_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_15_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_15_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_15_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_15_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_15_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_15_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_15_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_15_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_15_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_15_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_15_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_15_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_15_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_15_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_15_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_15_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_15_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_15_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_15_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_15_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_15_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_15_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_15_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_15_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_15_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_15_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_15_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_15_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_15_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_15_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_15_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_15_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_15_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_15_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_15_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_15_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_15_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_15_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_15_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_15_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_15_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_15_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_15_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_15_[49] = fle_9___out_0_ ; 
wire [1906:1921] mux_2level_size50_15_configbus0;
wire [1906:1921] mux_2level_size50_15_configbus1;
wire [1906:1921] mux_2level_size50_15_sram_blwl_out ;
wire [1906:1921] mux_2level_size50_15_sram_blwl_outb ;
assign mux_2level_size50_15_configbus0[1906:1921] = sram_blwl_bl[1906:1921] ;
assign mux_2level_size50_15_configbus1[1906:1921] = sram_blwl_wl[1906:1921] ;
wire [1906:1921] mux_2level_size50_15_configbus0_b;
assign mux_2level_size50_15_configbus0_b[1906:1921] = sram_blwl_blb[1906:1921] ;
mux_2level_size50 mux_2level_size50_15_ (in_bus_mux_2level_size50_15_, fle_2___in_3_, mux_2level_size50_15_sram_blwl_out[1906:1921] ,
mux_2level_size50_15_sram_blwl_outb[1906:1921] );
//----- SRAM bits for MUX[15], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1906_ (mux_2level_size50_15_sram_blwl_out[1906:1906] ,mux_2level_size50_15_sram_blwl_out[1906:1906] ,mux_2level_size50_15_sram_blwl_outb[1906:1906] ,mux_2level_size50_15_configbus0[1906:1906], mux_2level_size50_15_configbus1[1906:1906] , mux_2level_size50_15_configbus0_b[1906:1906] );
sram6T_blwl sram_blwl_1907_ (mux_2level_size50_15_sram_blwl_out[1907:1907] ,mux_2level_size50_15_sram_blwl_out[1907:1907] ,mux_2level_size50_15_sram_blwl_outb[1907:1907] ,mux_2level_size50_15_configbus0[1907:1907], mux_2level_size50_15_configbus1[1907:1907] , mux_2level_size50_15_configbus0_b[1907:1907] );
sram6T_blwl sram_blwl_1908_ (mux_2level_size50_15_sram_blwl_out[1908:1908] ,mux_2level_size50_15_sram_blwl_out[1908:1908] ,mux_2level_size50_15_sram_blwl_outb[1908:1908] ,mux_2level_size50_15_configbus0[1908:1908], mux_2level_size50_15_configbus1[1908:1908] , mux_2level_size50_15_configbus0_b[1908:1908] );
sram6T_blwl sram_blwl_1909_ (mux_2level_size50_15_sram_blwl_out[1909:1909] ,mux_2level_size50_15_sram_blwl_out[1909:1909] ,mux_2level_size50_15_sram_blwl_outb[1909:1909] ,mux_2level_size50_15_configbus0[1909:1909], mux_2level_size50_15_configbus1[1909:1909] , mux_2level_size50_15_configbus0_b[1909:1909] );
sram6T_blwl sram_blwl_1910_ (mux_2level_size50_15_sram_blwl_out[1910:1910] ,mux_2level_size50_15_sram_blwl_out[1910:1910] ,mux_2level_size50_15_sram_blwl_outb[1910:1910] ,mux_2level_size50_15_configbus0[1910:1910], mux_2level_size50_15_configbus1[1910:1910] , mux_2level_size50_15_configbus0_b[1910:1910] );
sram6T_blwl sram_blwl_1911_ (mux_2level_size50_15_sram_blwl_out[1911:1911] ,mux_2level_size50_15_sram_blwl_out[1911:1911] ,mux_2level_size50_15_sram_blwl_outb[1911:1911] ,mux_2level_size50_15_configbus0[1911:1911], mux_2level_size50_15_configbus1[1911:1911] , mux_2level_size50_15_configbus0_b[1911:1911] );
sram6T_blwl sram_blwl_1912_ (mux_2level_size50_15_sram_blwl_out[1912:1912] ,mux_2level_size50_15_sram_blwl_out[1912:1912] ,mux_2level_size50_15_sram_blwl_outb[1912:1912] ,mux_2level_size50_15_configbus0[1912:1912], mux_2level_size50_15_configbus1[1912:1912] , mux_2level_size50_15_configbus0_b[1912:1912] );
sram6T_blwl sram_blwl_1913_ (mux_2level_size50_15_sram_blwl_out[1913:1913] ,mux_2level_size50_15_sram_blwl_out[1913:1913] ,mux_2level_size50_15_sram_blwl_outb[1913:1913] ,mux_2level_size50_15_configbus0[1913:1913], mux_2level_size50_15_configbus1[1913:1913] , mux_2level_size50_15_configbus0_b[1913:1913] );
sram6T_blwl sram_blwl_1914_ (mux_2level_size50_15_sram_blwl_out[1914:1914] ,mux_2level_size50_15_sram_blwl_out[1914:1914] ,mux_2level_size50_15_sram_blwl_outb[1914:1914] ,mux_2level_size50_15_configbus0[1914:1914], mux_2level_size50_15_configbus1[1914:1914] , mux_2level_size50_15_configbus0_b[1914:1914] );
sram6T_blwl sram_blwl_1915_ (mux_2level_size50_15_sram_blwl_out[1915:1915] ,mux_2level_size50_15_sram_blwl_out[1915:1915] ,mux_2level_size50_15_sram_blwl_outb[1915:1915] ,mux_2level_size50_15_configbus0[1915:1915], mux_2level_size50_15_configbus1[1915:1915] , mux_2level_size50_15_configbus0_b[1915:1915] );
sram6T_blwl sram_blwl_1916_ (mux_2level_size50_15_sram_blwl_out[1916:1916] ,mux_2level_size50_15_sram_blwl_out[1916:1916] ,mux_2level_size50_15_sram_blwl_outb[1916:1916] ,mux_2level_size50_15_configbus0[1916:1916], mux_2level_size50_15_configbus1[1916:1916] , mux_2level_size50_15_configbus0_b[1916:1916] );
sram6T_blwl sram_blwl_1917_ (mux_2level_size50_15_sram_blwl_out[1917:1917] ,mux_2level_size50_15_sram_blwl_out[1917:1917] ,mux_2level_size50_15_sram_blwl_outb[1917:1917] ,mux_2level_size50_15_configbus0[1917:1917], mux_2level_size50_15_configbus1[1917:1917] , mux_2level_size50_15_configbus0_b[1917:1917] );
sram6T_blwl sram_blwl_1918_ (mux_2level_size50_15_sram_blwl_out[1918:1918] ,mux_2level_size50_15_sram_blwl_out[1918:1918] ,mux_2level_size50_15_sram_blwl_outb[1918:1918] ,mux_2level_size50_15_configbus0[1918:1918], mux_2level_size50_15_configbus1[1918:1918] , mux_2level_size50_15_configbus0_b[1918:1918] );
sram6T_blwl sram_blwl_1919_ (mux_2level_size50_15_sram_blwl_out[1919:1919] ,mux_2level_size50_15_sram_blwl_out[1919:1919] ,mux_2level_size50_15_sram_blwl_outb[1919:1919] ,mux_2level_size50_15_configbus0[1919:1919], mux_2level_size50_15_configbus1[1919:1919] , mux_2level_size50_15_configbus0_b[1919:1919] );
sram6T_blwl sram_blwl_1920_ (mux_2level_size50_15_sram_blwl_out[1920:1920] ,mux_2level_size50_15_sram_blwl_out[1920:1920] ,mux_2level_size50_15_sram_blwl_outb[1920:1920] ,mux_2level_size50_15_configbus0[1920:1920], mux_2level_size50_15_configbus1[1920:1920] , mux_2level_size50_15_configbus0_b[1920:1920] );
sram6T_blwl sram_blwl_1921_ (mux_2level_size50_15_sram_blwl_out[1921:1921] ,mux_2level_size50_15_sram_blwl_out[1921:1921] ,mux_2level_size50_15_sram_blwl_outb[1921:1921] ,mux_2level_size50_15_configbus0[1921:1921], mux_2level_size50_15_configbus1[1921:1921] , mux_2level_size50_15_configbus0_b[1921:1921] );
wire [0:49] in_bus_mux_2level_size50_16_ ;
assign in_bus_mux_2level_size50_16_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_16_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_16_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_16_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_16_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_16_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_16_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_16_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_16_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_16_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_16_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_16_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_16_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_16_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_16_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_16_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_16_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_16_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_16_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_16_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_16_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_16_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_16_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_16_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_16_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_16_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_16_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_16_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_16_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_16_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_16_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_16_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_16_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_16_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_16_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_16_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_16_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_16_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_16_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_16_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_16_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_16_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_16_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_16_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_16_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_16_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_16_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_16_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_16_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_16_[49] = fle_9___out_0_ ; 
wire [1922:1937] mux_2level_size50_16_configbus0;
wire [1922:1937] mux_2level_size50_16_configbus1;
wire [1922:1937] mux_2level_size50_16_sram_blwl_out ;
wire [1922:1937] mux_2level_size50_16_sram_blwl_outb ;
assign mux_2level_size50_16_configbus0[1922:1937] = sram_blwl_bl[1922:1937] ;
assign mux_2level_size50_16_configbus1[1922:1937] = sram_blwl_wl[1922:1937] ;
wire [1922:1937] mux_2level_size50_16_configbus0_b;
assign mux_2level_size50_16_configbus0_b[1922:1937] = sram_blwl_blb[1922:1937] ;
mux_2level_size50 mux_2level_size50_16_ (in_bus_mux_2level_size50_16_, fle_2___in_4_, mux_2level_size50_16_sram_blwl_out[1922:1937] ,
mux_2level_size50_16_sram_blwl_outb[1922:1937] );
//----- SRAM bits for MUX[16], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1922_ (mux_2level_size50_16_sram_blwl_out[1922:1922] ,mux_2level_size50_16_sram_blwl_out[1922:1922] ,mux_2level_size50_16_sram_blwl_outb[1922:1922] ,mux_2level_size50_16_configbus0[1922:1922], mux_2level_size50_16_configbus1[1922:1922] , mux_2level_size50_16_configbus0_b[1922:1922] );
sram6T_blwl sram_blwl_1923_ (mux_2level_size50_16_sram_blwl_out[1923:1923] ,mux_2level_size50_16_sram_blwl_out[1923:1923] ,mux_2level_size50_16_sram_blwl_outb[1923:1923] ,mux_2level_size50_16_configbus0[1923:1923], mux_2level_size50_16_configbus1[1923:1923] , mux_2level_size50_16_configbus0_b[1923:1923] );
sram6T_blwl sram_blwl_1924_ (mux_2level_size50_16_sram_blwl_out[1924:1924] ,mux_2level_size50_16_sram_blwl_out[1924:1924] ,mux_2level_size50_16_sram_blwl_outb[1924:1924] ,mux_2level_size50_16_configbus0[1924:1924], mux_2level_size50_16_configbus1[1924:1924] , mux_2level_size50_16_configbus0_b[1924:1924] );
sram6T_blwl sram_blwl_1925_ (mux_2level_size50_16_sram_blwl_out[1925:1925] ,mux_2level_size50_16_sram_blwl_out[1925:1925] ,mux_2level_size50_16_sram_blwl_outb[1925:1925] ,mux_2level_size50_16_configbus0[1925:1925], mux_2level_size50_16_configbus1[1925:1925] , mux_2level_size50_16_configbus0_b[1925:1925] );
sram6T_blwl sram_blwl_1926_ (mux_2level_size50_16_sram_blwl_out[1926:1926] ,mux_2level_size50_16_sram_blwl_out[1926:1926] ,mux_2level_size50_16_sram_blwl_outb[1926:1926] ,mux_2level_size50_16_configbus0[1926:1926], mux_2level_size50_16_configbus1[1926:1926] , mux_2level_size50_16_configbus0_b[1926:1926] );
sram6T_blwl sram_blwl_1927_ (mux_2level_size50_16_sram_blwl_out[1927:1927] ,mux_2level_size50_16_sram_blwl_out[1927:1927] ,mux_2level_size50_16_sram_blwl_outb[1927:1927] ,mux_2level_size50_16_configbus0[1927:1927], mux_2level_size50_16_configbus1[1927:1927] , mux_2level_size50_16_configbus0_b[1927:1927] );
sram6T_blwl sram_blwl_1928_ (mux_2level_size50_16_sram_blwl_out[1928:1928] ,mux_2level_size50_16_sram_blwl_out[1928:1928] ,mux_2level_size50_16_sram_blwl_outb[1928:1928] ,mux_2level_size50_16_configbus0[1928:1928], mux_2level_size50_16_configbus1[1928:1928] , mux_2level_size50_16_configbus0_b[1928:1928] );
sram6T_blwl sram_blwl_1929_ (mux_2level_size50_16_sram_blwl_out[1929:1929] ,mux_2level_size50_16_sram_blwl_out[1929:1929] ,mux_2level_size50_16_sram_blwl_outb[1929:1929] ,mux_2level_size50_16_configbus0[1929:1929], mux_2level_size50_16_configbus1[1929:1929] , mux_2level_size50_16_configbus0_b[1929:1929] );
sram6T_blwl sram_blwl_1930_ (mux_2level_size50_16_sram_blwl_out[1930:1930] ,mux_2level_size50_16_sram_blwl_out[1930:1930] ,mux_2level_size50_16_sram_blwl_outb[1930:1930] ,mux_2level_size50_16_configbus0[1930:1930], mux_2level_size50_16_configbus1[1930:1930] , mux_2level_size50_16_configbus0_b[1930:1930] );
sram6T_blwl sram_blwl_1931_ (mux_2level_size50_16_sram_blwl_out[1931:1931] ,mux_2level_size50_16_sram_blwl_out[1931:1931] ,mux_2level_size50_16_sram_blwl_outb[1931:1931] ,mux_2level_size50_16_configbus0[1931:1931], mux_2level_size50_16_configbus1[1931:1931] , mux_2level_size50_16_configbus0_b[1931:1931] );
sram6T_blwl sram_blwl_1932_ (mux_2level_size50_16_sram_blwl_out[1932:1932] ,mux_2level_size50_16_sram_blwl_out[1932:1932] ,mux_2level_size50_16_sram_blwl_outb[1932:1932] ,mux_2level_size50_16_configbus0[1932:1932], mux_2level_size50_16_configbus1[1932:1932] , mux_2level_size50_16_configbus0_b[1932:1932] );
sram6T_blwl sram_blwl_1933_ (mux_2level_size50_16_sram_blwl_out[1933:1933] ,mux_2level_size50_16_sram_blwl_out[1933:1933] ,mux_2level_size50_16_sram_blwl_outb[1933:1933] ,mux_2level_size50_16_configbus0[1933:1933], mux_2level_size50_16_configbus1[1933:1933] , mux_2level_size50_16_configbus0_b[1933:1933] );
sram6T_blwl sram_blwl_1934_ (mux_2level_size50_16_sram_blwl_out[1934:1934] ,mux_2level_size50_16_sram_blwl_out[1934:1934] ,mux_2level_size50_16_sram_blwl_outb[1934:1934] ,mux_2level_size50_16_configbus0[1934:1934], mux_2level_size50_16_configbus1[1934:1934] , mux_2level_size50_16_configbus0_b[1934:1934] );
sram6T_blwl sram_blwl_1935_ (mux_2level_size50_16_sram_blwl_out[1935:1935] ,mux_2level_size50_16_sram_blwl_out[1935:1935] ,mux_2level_size50_16_sram_blwl_outb[1935:1935] ,mux_2level_size50_16_configbus0[1935:1935], mux_2level_size50_16_configbus1[1935:1935] , mux_2level_size50_16_configbus0_b[1935:1935] );
sram6T_blwl sram_blwl_1936_ (mux_2level_size50_16_sram_blwl_out[1936:1936] ,mux_2level_size50_16_sram_blwl_out[1936:1936] ,mux_2level_size50_16_sram_blwl_outb[1936:1936] ,mux_2level_size50_16_configbus0[1936:1936], mux_2level_size50_16_configbus1[1936:1936] , mux_2level_size50_16_configbus0_b[1936:1936] );
sram6T_blwl sram_blwl_1937_ (mux_2level_size50_16_sram_blwl_out[1937:1937] ,mux_2level_size50_16_sram_blwl_out[1937:1937] ,mux_2level_size50_16_sram_blwl_outb[1937:1937] ,mux_2level_size50_16_configbus0[1937:1937], mux_2level_size50_16_configbus1[1937:1937] , mux_2level_size50_16_configbus0_b[1937:1937] );
wire [0:49] in_bus_mux_2level_size50_17_ ;
assign in_bus_mux_2level_size50_17_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_17_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_17_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_17_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_17_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_17_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_17_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_17_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_17_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_17_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_17_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_17_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_17_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_17_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_17_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_17_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_17_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_17_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_17_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_17_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_17_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_17_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_17_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_17_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_17_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_17_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_17_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_17_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_17_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_17_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_17_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_17_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_17_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_17_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_17_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_17_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_17_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_17_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_17_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_17_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_17_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_17_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_17_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_17_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_17_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_17_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_17_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_17_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_17_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_17_[49] = fle_9___out_0_ ; 
wire [1938:1953] mux_2level_size50_17_configbus0;
wire [1938:1953] mux_2level_size50_17_configbus1;
wire [1938:1953] mux_2level_size50_17_sram_blwl_out ;
wire [1938:1953] mux_2level_size50_17_sram_blwl_outb ;
assign mux_2level_size50_17_configbus0[1938:1953] = sram_blwl_bl[1938:1953] ;
assign mux_2level_size50_17_configbus1[1938:1953] = sram_blwl_wl[1938:1953] ;
wire [1938:1953] mux_2level_size50_17_configbus0_b;
assign mux_2level_size50_17_configbus0_b[1938:1953] = sram_blwl_blb[1938:1953] ;
mux_2level_size50 mux_2level_size50_17_ (in_bus_mux_2level_size50_17_, fle_2___in_5_, mux_2level_size50_17_sram_blwl_out[1938:1953] ,
mux_2level_size50_17_sram_blwl_outb[1938:1953] );
//----- SRAM bits for MUX[17], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1938_ (mux_2level_size50_17_sram_blwl_out[1938:1938] ,mux_2level_size50_17_sram_blwl_out[1938:1938] ,mux_2level_size50_17_sram_blwl_outb[1938:1938] ,mux_2level_size50_17_configbus0[1938:1938], mux_2level_size50_17_configbus1[1938:1938] , mux_2level_size50_17_configbus0_b[1938:1938] );
sram6T_blwl sram_blwl_1939_ (mux_2level_size50_17_sram_blwl_out[1939:1939] ,mux_2level_size50_17_sram_blwl_out[1939:1939] ,mux_2level_size50_17_sram_blwl_outb[1939:1939] ,mux_2level_size50_17_configbus0[1939:1939], mux_2level_size50_17_configbus1[1939:1939] , mux_2level_size50_17_configbus0_b[1939:1939] );
sram6T_blwl sram_blwl_1940_ (mux_2level_size50_17_sram_blwl_out[1940:1940] ,mux_2level_size50_17_sram_blwl_out[1940:1940] ,mux_2level_size50_17_sram_blwl_outb[1940:1940] ,mux_2level_size50_17_configbus0[1940:1940], mux_2level_size50_17_configbus1[1940:1940] , mux_2level_size50_17_configbus0_b[1940:1940] );
sram6T_blwl sram_blwl_1941_ (mux_2level_size50_17_sram_blwl_out[1941:1941] ,mux_2level_size50_17_sram_blwl_out[1941:1941] ,mux_2level_size50_17_sram_blwl_outb[1941:1941] ,mux_2level_size50_17_configbus0[1941:1941], mux_2level_size50_17_configbus1[1941:1941] , mux_2level_size50_17_configbus0_b[1941:1941] );
sram6T_blwl sram_blwl_1942_ (mux_2level_size50_17_sram_blwl_out[1942:1942] ,mux_2level_size50_17_sram_blwl_out[1942:1942] ,mux_2level_size50_17_sram_blwl_outb[1942:1942] ,mux_2level_size50_17_configbus0[1942:1942], mux_2level_size50_17_configbus1[1942:1942] , mux_2level_size50_17_configbus0_b[1942:1942] );
sram6T_blwl sram_blwl_1943_ (mux_2level_size50_17_sram_blwl_out[1943:1943] ,mux_2level_size50_17_sram_blwl_out[1943:1943] ,mux_2level_size50_17_sram_blwl_outb[1943:1943] ,mux_2level_size50_17_configbus0[1943:1943], mux_2level_size50_17_configbus1[1943:1943] , mux_2level_size50_17_configbus0_b[1943:1943] );
sram6T_blwl sram_blwl_1944_ (mux_2level_size50_17_sram_blwl_out[1944:1944] ,mux_2level_size50_17_sram_blwl_out[1944:1944] ,mux_2level_size50_17_sram_blwl_outb[1944:1944] ,mux_2level_size50_17_configbus0[1944:1944], mux_2level_size50_17_configbus1[1944:1944] , mux_2level_size50_17_configbus0_b[1944:1944] );
sram6T_blwl sram_blwl_1945_ (mux_2level_size50_17_sram_blwl_out[1945:1945] ,mux_2level_size50_17_sram_blwl_out[1945:1945] ,mux_2level_size50_17_sram_blwl_outb[1945:1945] ,mux_2level_size50_17_configbus0[1945:1945], mux_2level_size50_17_configbus1[1945:1945] , mux_2level_size50_17_configbus0_b[1945:1945] );
sram6T_blwl sram_blwl_1946_ (mux_2level_size50_17_sram_blwl_out[1946:1946] ,mux_2level_size50_17_sram_blwl_out[1946:1946] ,mux_2level_size50_17_sram_blwl_outb[1946:1946] ,mux_2level_size50_17_configbus0[1946:1946], mux_2level_size50_17_configbus1[1946:1946] , mux_2level_size50_17_configbus0_b[1946:1946] );
sram6T_blwl sram_blwl_1947_ (mux_2level_size50_17_sram_blwl_out[1947:1947] ,mux_2level_size50_17_sram_blwl_out[1947:1947] ,mux_2level_size50_17_sram_blwl_outb[1947:1947] ,mux_2level_size50_17_configbus0[1947:1947], mux_2level_size50_17_configbus1[1947:1947] , mux_2level_size50_17_configbus0_b[1947:1947] );
sram6T_blwl sram_blwl_1948_ (mux_2level_size50_17_sram_blwl_out[1948:1948] ,mux_2level_size50_17_sram_blwl_out[1948:1948] ,mux_2level_size50_17_sram_blwl_outb[1948:1948] ,mux_2level_size50_17_configbus0[1948:1948], mux_2level_size50_17_configbus1[1948:1948] , mux_2level_size50_17_configbus0_b[1948:1948] );
sram6T_blwl sram_blwl_1949_ (mux_2level_size50_17_sram_blwl_out[1949:1949] ,mux_2level_size50_17_sram_blwl_out[1949:1949] ,mux_2level_size50_17_sram_blwl_outb[1949:1949] ,mux_2level_size50_17_configbus0[1949:1949], mux_2level_size50_17_configbus1[1949:1949] , mux_2level_size50_17_configbus0_b[1949:1949] );
sram6T_blwl sram_blwl_1950_ (mux_2level_size50_17_sram_blwl_out[1950:1950] ,mux_2level_size50_17_sram_blwl_out[1950:1950] ,mux_2level_size50_17_sram_blwl_outb[1950:1950] ,mux_2level_size50_17_configbus0[1950:1950], mux_2level_size50_17_configbus1[1950:1950] , mux_2level_size50_17_configbus0_b[1950:1950] );
sram6T_blwl sram_blwl_1951_ (mux_2level_size50_17_sram_blwl_out[1951:1951] ,mux_2level_size50_17_sram_blwl_out[1951:1951] ,mux_2level_size50_17_sram_blwl_outb[1951:1951] ,mux_2level_size50_17_configbus0[1951:1951], mux_2level_size50_17_configbus1[1951:1951] , mux_2level_size50_17_configbus0_b[1951:1951] );
sram6T_blwl sram_blwl_1952_ (mux_2level_size50_17_sram_blwl_out[1952:1952] ,mux_2level_size50_17_sram_blwl_out[1952:1952] ,mux_2level_size50_17_sram_blwl_outb[1952:1952] ,mux_2level_size50_17_configbus0[1952:1952], mux_2level_size50_17_configbus1[1952:1952] , mux_2level_size50_17_configbus0_b[1952:1952] );
sram6T_blwl sram_blwl_1953_ (mux_2level_size50_17_sram_blwl_out[1953:1953] ,mux_2level_size50_17_sram_blwl_out[1953:1953] ,mux_2level_size50_17_sram_blwl_outb[1953:1953] ,mux_2level_size50_17_configbus0[1953:1953], mux_2level_size50_17_configbus1[1953:1953] , mux_2level_size50_17_configbus0_b[1953:1953] );
direct_interc direct_interc_172_ (mode_clb___clk_0_, fle_2___clk_0_ );
wire [0:49] in_bus_mux_2level_size50_18_ ;
assign in_bus_mux_2level_size50_18_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_18_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_18_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_18_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_18_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_18_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_18_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_18_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_18_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_18_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_18_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_18_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_18_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_18_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_18_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_18_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_18_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_18_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_18_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_18_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_18_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_18_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_18_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_18_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_18_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_18_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_18_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_18_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_18_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_18_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_18_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_18_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_18_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_18_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_18_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_18_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_18_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_18_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_18_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_18_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_18_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_18_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_18_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_18_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_18_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_18_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_18_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_18_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_18_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_18_[49] = fle_9___out_0_ ; 
wire [1954:1969] mux_2level_size50_18_configbus0;
wire [1954:1969] mux_2level_size50_18_configbus1;
wire [1954:1969] mux_2level_size50_18_sram_blwl_out ;
wire [1954:1969] mux_2level_size50_18_sram_blwl_outb ;
assign mux_2level_size50_18_configbus0[1954:1969] = sram_blwl_bl[1954:1969] ;
assign mux_2level_size50_18_configbus1[1954:1969] = sram_blwl_wl[1954:1969] ;
wire [1954:1969] mux_2level_size50_18_configbus0_b;
assign mux_2level_size50_18_configbus0_b[1954:1969] = sram_blwl_blb[1954:1969] ;
mux_2level_size50 mux_2level_size50_18_ (in_bus_mux_2level_size50_18_, fle_3___in_0_, mux_2level_size50_18_sram_blwl_out[1954:1969] ,
mux_2level_size50_18_sram_blwl_outb[1954:1969] );
//----- SRAM bits for MUX[18], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1954_ (mux_2level_size50_18_sram_blwl_out[1954:1954] ,mux_2level_size50_18_sram_blwl_out[1954:1954] ,mux_2level_size50_18_sram_blwl_outb[1954:1954] ,mux_2level_size50_18_configbus0[1954:1954], mux_2level_size50_18_configbus1[1954:1954] , mux_2level_size50_18_configbus0_b[1954:1954] );
sram6T_blwl sram_blwl_1955_ (mux_2level_size50_18_sram_blwl_out[1955:1955] ,mux_2level_size50_18_sram_blwl_out[1955:1955] ,mux_2level_size50_18_sram_blwl_outb[1955:1955] ,mux_2level_size50_18_configbus0[1955:1955], mux_2level_size50_18_configbus1[1955:1955] , mux_2level_size50_18_configbus0_b[1955:1955] );
sram6T_blwl sram_blwl_1956_ (mux_2level_size50_18_sram_blwl_out[1956:1956] ,mux_2level_size50_18_sram_blwl_out[1956:1956] ,mux_2level_size50_18_sram_blwl_outb[1956:1956] ,mux_2level_size50_18_configbus0[1956:1956], mux_2level_size50_18_configbus1[1956:1956] , mux_2level_size50_18_configbus0_b[1956:1956] );
sram6T_blwl sram_blwl_1957_ (mux_2level_size50_18_sram_blwl_out[1957:1957] ,mux_2level_size50_18_sram_blwl_out[1957:1957] ,mux_2level_size50_18_sram_blwl_outb[1957:1957] ,mux_2level_size50_18_configbus0[1957:1957], mux_2level_size50_18_configbus1[1957:1957] , mux_2level_size50_18_configbus0_b[1957:1957] );
sram6T_blwl sram_blwl_1958_ (mux_2level_size50_18_sram_blwl_out[1958:1958] ,mux_2level_size50_18_sram_blwl_out[1958:1958] ,mux_2level_size50_18_sram_blwl_outb[1958:1958] ,mux_2level_size50_18_configbus0[1958:1958], mux_2level_size50_18_configbus1[1958:1958] , mux_2level_size50_18_configbus0_b[1958:1958] );
sram6T_blwl sram_blwl_1959_ (mux_2level_size50_18_sram_blwl_out[1959:1959] ,mux_2level_size50_18_sram_blwl_out[1959:1959] ,mux_2level_size50_18_sram_blwl_outb[1959:1959] ,mux_2level_size50_18_configbus0[1959:1959], mux_2level_size50_18_configbus1[1959:1959] , mux_2level_size50_18_configbus0_b[1959:1959] );
sram6T_blwl sram_blwl_1960_ (mux_2level_size50_18_sram_blwl_out[1960:1960] ,mux_2level_size50_18_sram_blwl_out[1960:1960] ,mux_2level_size50_18_sram_blwl_outb[1960:1960] ,mux_2level_size50_18_configbus0[1960:1960], mux_2level_size50_18_configbus1[1960:1960] , mux_2level_size50_18_configbus0_b[1960:1960] );
sram6T_blwl sram_blwl_1961_ (mux_2level_size50_18_sram_blwl_out[1961:1961] ,mux_2level_size50_18_sram_blwl_out[1961:1961] ,mux_2level_size50_18_sram_blwl_outb[1961:1961] ,mux_2level_size50_18_configbus0[1961:1961], mux_2level_size50_18_configbus1[1961:1961] , mux_2level_size50_18_configbus0_b[1961:1961] );
sram6T_blwl sram_blwl_1962_ (mux_2level_size50_18_sram_blwl_out[1962:1962] ,mux_2level_size50_18_sram_blwl_out[1962:1962] ,mux_2level_size50_18_sram_blwl_outb[1962:1962] ,mux_2level_size50_18_configbus0[1962:1962], mux_2level_size50_18_configbus1[1962:1962] , mux_2level_size50_18_configbus0_b[1962:1962] );
sram6T_blwl sram_blwl_1963_ (mux_2level_size50_18_sram_blwl_out[1963:1963] ,mux_2level_size50_18_sram_blwl_out[1963:1963] ,mux_2level_size50_18_sram_blwl_outb[1963:1963] ,mux_2level_size50_18_configbus0[1963:1963], mux_2level_size50_18_configbus1[1963:1963] , mux_2level_size50_18_configbus0_b[1963:1963] );
sram6T_blwl sram_blwl_1964_ (mux_2level_size50_18_sram_blwl_out[1964:1964] ,mux_2level_size50_18_sram_blwl_out[1964:1964] ,mux_2level_size50_18_sram_blwl_outb[1964:1964] ,mux_2level_size50_18_configbus0[1964:1964], mux_2level_size50_18_configbus1[1964:1964] , mux_2level_size50_18_configbus0_b[1964:1964] );
sram6T_blwl sram_blwl_1965_ (mux_2level_size50_18_sram_blwl_out[1965:1965] ,mux_2level_size50_18_sram_blwl_out[1965:1965] ,mux_2level_size50_18_sram_blwl_outb[1965:1965] ,mux_2level_size50_18_configbus0[1965:1965], mux_2level_size50_18_configbus1[1965:1965] , mux_2level_size50_18_configbus0_b[1965:1965] );
sram6T_blwl sram_blwl_1966_ (mux_2level_size50_18_sram_blwl_out[1966:1966] ,mux_2level_size50_18_sram_blwl_out[1966:1966] ,mux_2level_size50_18_sram_blwl_outb[1966:1966] ,mux_2level_size50_18_configbus0[1966:1966], mux_2level_size50_18_configbus1[1966:1966] , mux_2level_size50_18_configbus0_b[1966:1966] );
sram6T_blwl sram_blwl_1967_ (mux_2level_size50_18_sram_blwl_out[1967:1967] ,mux_2level_size50_18_sram_blwl_out[1967:1967] ,mux_2level_size50_18_sram_blwl_outb[1967:1967] ,mux_2level_size50_18_configbus0[1967:1967], mux_2level_size50_18_configbus1[1967:1967] , mux_2level_size50_18_configbus0_b[1967:1967] );
sram6T_blwl sram_blwl_1968_ (mux_2level_size50_18_sram_blwl_out[1968:1968] ,mux_2level_size50_18_sram_blwl_out[1968:1968] ,mux_2level_size50_18_sram_blwl_outb[1968:1968] ,mux_2level_size50_18_configbus0[1968:1968], mux_2level_size50_18_configbus1[1968:1968] , mux_2level_size50_18_configbus0_b[1968:1968] );
sram6T_blwl sram_blwl_1969_ (mux_2level_size50_18_sram_blwl_out[1969:1969] ,mux_2level_size50_18_sram_blwl_out[1969:1969] ,mux_2level_size50_18_sram_blwl_outb[1969:1969] ,mux_2level_size50_18_configbus0[1969:1969], mux_2level_size50_18_configbus1[1969:1969] , mux_2level_size50_18_configbus0_b[1969:1969] );
wire [0:49] in_bus_mux_2level_size50_19_ ;
assign in_bus_mux_2level_size50_19_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_19_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_19_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_19_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_19_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_19_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_19_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_19_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_19_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_19_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_19_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_19_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_19_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_19_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_19_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_19_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_19_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_19_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_19_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_19_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_19_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_19_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_19_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_19_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_19_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_19_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_19_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_19_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_19_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_19_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_19_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_19_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_19_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_19_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_19_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_19_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_19_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_19_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_19_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_19_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_19_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_19_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_19_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_19_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_19_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_19_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_19_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_19_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_19_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_19_[49] = fle_9___out_0_ ; 
wire [1970:1985] mux_2level_size50_19_configbus0;
wire [1970:1985] mux_2level_size50_19_configbus1;
wire [1970:1985] mux_2level_size50_19_sram_blwl_out ;
wire [1970:1985] mux_2level_size50_19_sram_blwl_outb ;
assign mux_2level_size50_19_configbus0[1970:1985] = sram_blwl_bl[1970:1985] ;
assign mux_2level_size50_19_configbus1[1970:1985] = sram_blwl_wl[1970:1985] ;
wire [1970:1985] mux_2level_size50_19_configbus0_b;
assign mux_2level_size50_19_configbus0_b[1970:1985] = sram_blwl_blb[1970:1985] ;
mux_2level_size50 mux_2level_size50_19_ (in_bus_mux_2level_size50_19_, fle_3___in_1_, mux_2level_size50_19_sram_blwl_out[1970:1985] ,
mux_2level_size50_19_sram_blwl_outb[1970:1985] );
//----- SRAM bits for MUX[19], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1970_ (mux_2level_size50_19_sram_blwl_out[1970:1970] ,mux_2level_size50_19_sram_blwl_out[1970:1970] ,mux_2level_size50_19_sram_blwl_outb[1970:1970] ,mux_2level_size50_19_configbus0[1970:1970], mux_2level_size50_19_configbus1[1970:1970] , mux_2level_size50_19_configbus0_b[1970:1970] );
sram6T_blwl sram_blwl_1971_ (mux_2level_size50_19_sram_blwl_out[1971:1971] ,mux_2level_size50_19_sram_blwl_out[1971:1971] ,mux_2level_size50_19_sram_blwl_outb[1971:1971] ,mux_2level_size50_19_configbus0[1971:1971], mux_2level_size50_19_configbus1[1971:1971] , mux_2level_size50_19_configbus0_b[1971:1971] );
sram6T_blwl sram_blwl_1972_ (mux_2level_size50_19_sram_blwl_out[1972:1972] ,mux_2level_size50_19_sram_blwl_out[1972:1972] ,mux_2level_size50_19_sram_blwl_outb[1972:1972] ,mux_2level_size50_19_configbus0[1972:1972], mux_2level_size50_19_configbus1[1972:1972] , mux_2level_size50_19_configbus0_b[1972:1972] );
sram6T_blwl sram_blwl_1973_ (mux_2level_size50_19_sram_blwl_out[1973:1973] ,mux_2level_size50_19_sram_blwl_out[1973:1973] ,mux_2level_size50_19_sram_blwl_outb[1973:1973] ,mux_2level_size50_19_configbus0[1973:1973], mux_2level_size50_19_configbus1[1973:1973] , mux_2level_size50_19_configbus0_b[1973:1973] );
sram6T_blwl sram_blwl_1974_ (mux_2level_size50_19_sram_blwl_out[1974:1974] ,mux_2level_size50_19_sram_blwl_out[1974:1974] ,mux_2level_size50_19_sram_blwl_outb[1974:1974] ,mux_2level_size50_19_configbus0[1974:1974], mux_2level_size50_19_configbus1[1974:1974] , mux_2level_size50_19_configbus0_b[1974:1974] );
sram6T_blwl sram_blwl_1975_ (mux_2level_size50_19_sram_blwl_out[1975:1975] ,mux_2level_size50_19_sram_blwl_out[1975:1975] ,mux_2level_size50_19_sram_blwl_outb[1975:1975] ,mux_2level_size50_19_configbus0[1975:1975], mux_2level_size50_19_configbus1[1975:1975] , mux_2level_size50_19_configbus0_b[1975:1975] );
sram6T_blwl sram_blwl_1976_ (mux_2level_size50_19_sram_blwl_out[1976:1976] ,mux_2level_size50_19_sram_blwl_out[1976:1976] ,mux_2level_size50_19_sram_blwl_outb[1976:1976] ,mux_2level_size50_19_configbus0[1976:1976], mux_2level_size50_19_configbus1[1976:1976] , mux_2level_size50_19_configbus0_b[1976:1976] );
sram6T_blwl sram_blwl_1977_ (mux_2level_size50_19_sram_blwl_out[1977:1977] ,mux_2level_size50_19_sram_blwl_out[1977:1977] ,mux_2level_size50_19_sram_blwl_outb[1977:1977] ,mux_2level_size50_19_configbus0[1977:1977], mux_2level_size50_19_configbus1[1977:1977] , mux_2level_size50_19_configbus0_b[1977:1977] );
sram6T_blwl sram_blwl_1978_ (mux_2level_size50_19_sram_blwl_out[1978:1978] ,mux_2level_size50_19_sram_blwl_out[1978:1978] ,mux_2level_size50_19_sram_blwl_outb[1978:1978] ,mux_2level_size50_19_configbus0[1978:1978], mux_2level_size50_19_configbus1[1978:1978] , mux_2level_size50_19_configbus0_b[1978:1978] );
sram6T_blwl sram_blwl_1979_ (mux_2level_size50_19_sram_blwl_out[1979:1979] ,mux_2level_size50_19_sram_blwl_out[1979:1979] ,mux_2level_size50_19_sram_blwl_outb[1979:1979] ,mux_2level_size50_19_configbus0[1979:1979], mux_2level_size50_19_configbus1[1979:1979] , mux_2level_size50_19_configbus0_b[1979:1979] );
sram6T_blwl sram_blwl_1980_ (mux_2level_size50_19_sram_blwl_out[1980:1980] ,mux_2level_size50_19_sram_blwl_out[1980:1980] ,mux_2level_size50_19_sram_blwl_outb[1980:1980] ,mux_2level_size50_19_configbus0[1980:1980], mux_2level_size50_19_configbus1[1980:1980] , mux_2level_size50_19_configbus0_b[1980:1980] );
sram6T_blwl sram_blwl_1981_ (mux_2level_size50_19_sram_blwl_out[1981:1981] ,mux_2level_size50_19_sram_blwl_out[1981:1981] ,mux_2level_size50_19_sram_blwl_outb[1981:1981] ,mux_2level_size50_19_configbus0[1981:1981], mux_2level_size50_19_configbus1[1981:1981] , mux_2level_size50_19_configbus0_b[1981:1981] );
sram6T_blwl sram_blwl_1982_ (mux_2level_size50_19_sram_blwl_out[1982:1982] ,mux_2level_size50_19_sram_blwl_out[1982:1982] ,mux_2level_size50_19_sram_blwl_outb[1982:1982] ,mux_2level_size50_19_configbus0[1982:1982], mux_2level_size50_19_configbus1[1982:1982] , mux_2level_size50_19_configbus0_b[1982:1982] );
sram6T_blwl sram_blwl_1983_ (mux_2level_size50_19_sram_blwl_out[1983:1983] ,mux_2level_size50_19_sram_blwl_out[1983:1983] ,mux_2level_size50_19_sram_blwl_outb[1983:1983] ,mux_2level_size50_19_configbus0[1983:1983], mux_2level_size50_19_configbus1[1983:1983] , mux_2level_size50_19_configbus0_b[1983:1983] );
sram6T_blwl sram_blwl_1984_ (mux_2level_size50_19_sram_blwl_out[1984:1984] ,mux_2level_size50_19_sram_blwl_out[1984:1984] ,mux_2level_size50_19_sram_blwl_outb[1984:1984] ,mux_2level_size50_19_configbus0[1984:1984], mux_2level_size50_19_configbus1[1984:1984] , mux_2level_size50_19_configbus0_b[1984:1984] );
sram6T_blwl sram_blwl_1985_ (mux_2level_size50_19_sram_blwl_out[1985:1985] ,mux_2level_size50_19_sram_blwl_out[1985:1985] ,mux_2level_size50_19_sram_blwl_outb[1985:1985] ,mux_2level_size50_19_configbus0[1985:1985], mux_2level_size50_19_configbus1[1985:1985] , mux_2level_size50_19_configbus0_b[1985:1985] );
wire [0:49] in_bus_mux_2level_size50_20_ ;
assign in_bus_mux_2level_size50_20_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_20_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_20_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_20_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_20_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_20_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_20_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_20_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_20_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_20_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_20_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_20_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_20_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_20_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_20_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_20_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_20_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_20_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_20_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_20_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_20_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_20_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_20_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_20_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_20_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_20_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_20_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_20_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_20_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_20_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_20_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_20_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_20_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_20_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_20_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_20_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_20_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_20_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_20_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_20_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_20_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_20_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_20_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_20_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_20_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_20_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_20_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_20_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_20_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_20_[49] = fle_9___out_0_ ; 
wire [1986:2001] mux_2level_size50_20_configbus0;
wire [1986:2001] mux_2level_size50_20_configbus1;
wire [1986:2001] mux_2level_size50_20_sram_blwl_out ;
wire [1986:2001] mux_2level_size50_20_sram_blwl_outb ;
assign mux_2level_size50_20_configbus0[1986:2001] = sram_blwl_bl[1986:2001] ;
assign mux_2level_size50_20_configbus1[1986:2001] = sram_blwl_wl[1986:2001] ;
wire [1986:2001] mux_2level_size50_20_configbus0_b;
assign mux_2level_size50_20_configbus0_b[1986:2001] = sram_blwl_blb[1986:2001] ;
mux_2level_size50 mux_2level_size50_20_ (in_bus_mux_2level_size50_20_, fle_3___in_2_, mux_2level_size50_20_sram_blwl_out[1986:2001] ,
mux_2level_size50_20_sram_blwl_outb[1986:2001] );
//----- SRAM bits for MUX[20], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_1986_ (mux_2level_size50_20_sram_blwl_out[1986:1986] ,mux_2level_size50_20_sram_blwl_out[1986:1986] ,mux_2level_size50_20_sram_blwl_outb[1986:1986] ,mux_2level_size50_20_configbus0[1986:1986], mux_2level_size50_20_configbus1[1986:1986] , mux_2level_size50_20_configbus0_b[1986:1986] );
sram6T_blwl sram_blwl_1987_ (mux_2level_size50_20_sram_blwl_out[1987:1987] ,mux_2level_size50_20_sram_blwl_out[1987:1987] ,mux_2level_size50_20_sram_blwl_outb[1987:1987] ,mux_2level_size50_20_configbus0[1987:1987], mux_2level_size50_20_configbus1[1987:1987] , mux_2level_size50_20_configbus0_b[1987:1987] );
sram6T_blwl sram_blwl_1988_ (mux_2level_size50_20_sram_blwl_out[1988:1988] ,mux_2level_size50_20_sram_blwl_out[1988:1988] ,mux_2level_size50_20_sram_blwl_outb[1988:1988] ,mux_2level_size50_20_configbus0[1988:1988], mux_2level_size50_20_configbus1[1988:1988] , mux_2level_size50_20_configbus0_b[1988:1988] );
sram6T_blwl sram_blwl_1989_ (mux_2level_size50_20_sram_blwl_out[1989:1989] ,mux_2level_size50_20_sram_blwl_out[1989:1989] ,mux_2level_size50_20_sram_blwl_outb[1989:1989] ,mux_2level_size50_20_configbus0[1989:1989], mux_2level_size50_20_configbus1[1989:1989] , mux_2level_size50_20_configbus0_b[1989:1989] );
sram6T_blwl sram_blwl_1990_ (mux_2level_size50_20_sram_blwl_out[1990:1990] ,mux_2level_size50_20_sram_blwl_out[1990:1990] ,mux_2level_size50_20_sram_blwl_outb[1990:1990] ,mux_2level_size50_20_configbus0[1990:1990], mux_2level_size50_20_configbus1[1990:1990] , mux_2level_size50_20_configbus0_b[1990:1990] );
sram6T_blwl sram_blwl_1991_ (mux_2level_size50_20_sram_blwl_out[1991:1991] ,mux_2level_size50_20_sram_blwl_out[1991:1991] ,mux_2level_size50_20_sram_blwl_outb[1991:1991] ,mux_2level_size50_20_configbus0[1991:1991], mux_2level_size50_20_configbus1[1991:1991] , mux_2level_size50_20_configbus0_b[1991:1991] );
sram6T_blwl sram_blwl_1992_ (mux_2level_size50_20_sram_blwl_out[1992:1992] ,mux_2level_size50_20_sram_blwl_out[1992:1992] ,mux_2level_size50_20_sram_blwl_outb[1992:1992] ,mux_2level_size50_20_configbus0[1992:1992], mux_2level_size50_20_configbus1[1992:1992] , mux_2level_size50_20_configbus0_b[1992:1992] );
sram6T_blwl sram_blwl_1993_ (mux_2level_size50_20_sram_blwl_out[1993:1993] ,mux_2level_size50_20_sram_blwl_out[1993:1993] ,mux_2level_size50_20_sram_blwl_outb[1993:1993] ,mux_2level_size50_20_configbus0[1993:1993], mux_2level_size50_20_configbus1[1993:1993] , mux_2level_size50_20_configbus0_b[1993:1993] );
sram6T_blwl sram_blwl_1994_ (mux_2level_size50_20_sram_blwl_out[1994:1994] ,mux_2level_size50_20_sram_blwl_out[1994:1994] ,mux_2level_size50_20_sram_blwl_outb[1994:1994] ,mux_2level_size50_20_configbus0[1994:1994], mux_2level_size50_20_configbus1[1994:1994] , mux_2level_size50_20_configbus0_b[1994:1994] );
sram6T_blwl sram_blwl_1995_ (mux_2level_size50_20_sram_blwl_out[1995:1995] ,mux_2level_size50_20_sram_blwl_out[1995:1995] ,mux_2level_size50_20_sram_blwl_outb[1995:1995] ,mux_2level_size50_20_configbus0[1995:1995], mux_2level_size50_20_configbus1[1995:1995] , mux_2level_size50_20_configbus0_b[1995:1995] );
sram6T_blwl sram_blwl_1996_ (mux_2level_size50_20_sram_blwl_out[1996:1996] ,mux_2level_size50_20_sram_blwl_out[1996:1996] ,mux_2level_size50_20_sram_blwl_outb[1996:1996] ,mux_2level_size50_20_configbus0[1996:1996], mux_2level_size50_20_configbus1[1996:1996] , mux_2level_size50_20_configbus0_b[1996:1996] );
sram6T_blwl sram_blwl_1997_ (mux_2level_size50_20_sram_blwl_out[1997:1997] ,mux_2level_size50_20_sram_blwl_out[1997:1997] ,mux_2level_size50_20_sram_blwl_outb[1997:1997] ,mux_2level_size50_20_configbus0[1997:1997], mux_2level_size50_20_configbus1[1997:1997] , mux_2level_size50_20_configbus0_b[1997:1997] );
sram6T_blwl sram_blwl_1998_ (mux_2level_size50_20_sram_blwl_out[1998:1998] ,mux_2level_size50_20_sram_blwl_out[1998:1998] ,mux_2level_size50_20_sram_blwl_outb[1998:1998] ,mux_2level_size50_20_configbus0[1998:1998], mux_2level_size50_20_configbus1[1998:1998] , mux_2level_size50_20_configbus0_b[1998:1998] );
sram6T_blwl sram_blwl_1999_ (mux_2level_size50_20_sram_blwl_out[1999:1999] ,mux_2level_size50_20_sram_blwl_out[1999:1999] ,mux_2level_size50_20_sram_blwl_outb[1999:1999] ,mux_2level_size50_20_configbus0[1999:1999], mux_2level_size50_20_configbus1[1999:1999] , mux_2level_size50_20_configbus0_b[1999:1999] );
sram6T_blwl sram_blwl_2000_ (mux_2level_size50_20_sram_blwl_out[2000:2000] ,mux_2level_size50_20_sram_blwl_out[2000:2000] ,mux_2level_size50_20_sram_blwl_outb[2000:2000] ,mux_2level_size50_20_configbus0[2000:2000], mux_2level_size50_20_configbus1[2000:2000] , mux_2level_size50_20_configbus0_b[2000:2000] );
sram6T_blwl sram_blwl_2001_ (mux_2level_size50_20_sram_blwl_out[2001:2001] ,mux_2level_size50_20_sram_blwl_out[2001:2001] ,mux_2level_size50_20_sram_blwl_outb[2001:2001] ,mux_2level_size50_20_configbus0[2001:2001], mux_2level_size50_20_configbus1[2001:2001] , mux_2level_size50_20_configbus0_b[2001:2001] );
wire [0:49] in_bus_mux_2level_size50_21_ ;
assign in_bus_mux_2level_size50_21_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_21_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_21_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_21_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_21_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_21_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_21_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_21_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_21_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_21_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_21_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_21_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_21_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_21_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_21_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_21_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_21_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_21_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_21_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_21_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_21_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_21_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_21_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_21_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_21_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_21_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_21_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_21_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_21_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_21_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_21_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_21_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_21_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_21_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_21_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_21_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_21_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_21_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_21_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_21_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_21_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_21_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_21_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_21_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_21_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_21_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_21_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_21_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_21_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_21_[49] = fle_9___out_0_ ; 
wire [2002:2017] mux_2level_size50_21_configbus0;
wire [2002:2017] mux_2level_size50_21_configbus1;
wire [2002:2017] mux_2level_size50_21_sram_blwl_out ;
wire [2002:2017] mux_2level_size50_21_sram_blwl_outb ;
assign mux_2level_size50_21_configbus0[2002:2017] = sram_blwl_bl[2002:2017] ;
assign mux_2level_size50_21_configbus1[2002:2017] = sram_blwl_wl[2002:2017] ;
wire [2002:2017] mux_2level_size50_21_configbus0_b;
assign mux_2level_size50_21_configbus0_b[2002:2017] = sram_blwl_blb[2002:2017] ;
mux_2level_size50 mux_2level_size50_21_ (in_bus_mux_2level_size50_21_, fle_3___in_3_, mux_2level_size50_21_sram_blwl_out[2002:2017] ,
mux_2level_size50_21_sram_blwl_outb[2002:2017] );
//----- SRAM bits for MUX[21], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2002_ (mux_2level_size50_21_sram_blwl_out[2002:2002] ,mux_2level_size50_21_sram_blwl_out[2002:2002] ,mux_2level_size50_21_sram_blwl_outb[2002:2002] ,mux_2level_size50_21_configbus0[2002:2002], mux_2level_size50_21_configbus1[2002:2002] , mux_2level_size50_21_configbus0_b[2002:2002] );
sram6T_blwl sram_blwl_2003_ (mux_2level_size50_21_sram_blwl_out[2003:2003] ,mux_2level_size50_21_sram_blwl_out[2003:2003] ,mux_2level_size50_21_sram_blwl_outb[2003:2003] ,mux_2level_size50_21_configbus0[2003:2003], mux_2level_size50_21_configbus1[2003:2003] , mux_2level_size50_21_configbus0_b[2003:2003] );
sram6T_blwl sram_blwl_2004_ (mux_2level_size50_21_sram_blwl_out[2004:2004] ,mux_2level_size50_21_sram_blwl_out[2004:2004] ,mux_2level_size50_21_sram_blwl_outb[2004:2004] ,mux_2level_size50_21_configbus0[2004:2004], mux_2level_size50_21_configbus1[2004:2004] , mux_2level_size50_21_configbus0_b[2004:2004] );
sram6T_blwl sram_blwl_2005_ (mux_2level_size50_21_sram_blwl_out[2005:2005] ,mux_2level_size50_21_sram_blwl_out[2005:2005] ,mux_2level_size50_21_sram_blwl_outb[2005:2005] ,mux_2level_size50_21_configbus0[2005:2005], mux_2level_size50_21_configbus1[2005:2005] , mux_2level_size50_21_configbus0_b[2005:2005] );
sram6T_blwl sram_blwl_2006_ (mux_2level_size50_21_sram_blwl_out[2006:2006] ,mux_2level_size50_21_sram_blwl_out[2006:2006] ,mux_2level_size50_21_sram_blwl_outb[2006:2006] ,mux_2level_size50_21_configbus0[2006:2006], mux_2level_size50_21_configbus1[2006:2006] , mux_2level_size50_21_configbus0_b[2006:2006] );
sram6T_blwl sram_blwl_2007_ (mux_2level_size50_21_sram_blwl_out[2007:2007] ,mux_2level_size50_21_sram_blwl_out[2007:2007] ,mux_2level_size50_21_sram_blwl_outb[2007:2007] ,mux_2level_size50_21_configbus0[2007:2007], mux_2level_size50_21_configbus1[2007:2007] , mux_2level_size50_21_configbus0_b[2007:2007] );
sram6T_blwl sram_blwl_2008_ (mux_2level_size50_21_sram_blwl_out[2008:2008] ,mux_2level_size50_21_sram_blwl_out[2008:2008] ,mux_2level_size50_21_sram_blwl_outb[2008:2008] ,mux_2level_size50_21_configbus0[2008:2008], mux_2level_size50_21_configbus1[2008:2008] , mux_2level_size50_21_configbus0_b[2008:2008] );
sram6T_blwl sram_blwl_2009_ (mux_2level_size50_21_sram_blwl_out[2009:2009] ,mux_2level_size50_21_sram_blwl_out[2009:2009] ,mux_2level_size50_21_sram_blwl_outb[2009:2009] ,mux_2level_size50_21_configbus0[2009:2009], mux_2level_size50_21_configbus1[2009:2009] , mux_2level_size50_21_configbus0_b[2009:2009] );
sram6T_blwl sram_blwl_2010_ (mux_2level_size50_21_sram_blwl_out[2010:2010] ,mux_2level_size50_21_sram_blwl_out[2010:2010] ,mux_2level_size50_21_sram_blwl_outb[2010:2010] ,mux_2level_size50_21_configbus0[2010:2010], mux_2level_size50_21_configbus1[2010:2010] , mux_2level_size50_21_configbus0_b[2010:2010] );
sram6T_blwl sram_blwl_2011_ (mux_2level_size50_21_sram_blwl_out[2011:2011] ,mux_2level_size50_21_sram_blwl_out[2011:2011] ,mux_2level_size50_21_sram_blwl_outb[2011:2011] ,mux_2level_size50_21_configbus0[2011:2011], mux_2level_size50_21_configbus1[2011:2011] , mux_2level_size50_21_configbus0_b[2011:2011] );
sram6T_blwl sram_blwl_2012_ (mux_2level_size50_21_sram_blwl_out[2012:2012] ,mux_2level_size50_21_sram_blwl_out[2012:2012] ,mux_2level_size50_21_sram_blwl_outb[2012:2012] ,mux_2level_size50_21_configbus0[2012:2012], mux_2level_size50_21_configbus1[2012:2012] , mux_2level_size50_21_configbus0_b[2012:2012] );
sram6T_blwl sram_blwl_2013_ (mux_2level_size50_21_sram_blwl_out[2013:2013] ,mux_2level_size50_21_sram_blwl_out[2013:2013] ,mux_2level_size50_21_sram_blwl_outb[2013:2013] ,mux_2level_size50_21_configbus0[2013:2013], mux_2level_size50_21_configbus1[2013:2013] , mux_2level_size50_21_configbus0_b[2013:2013] );
sram6T_blwl sram_blwl_2014_ (mux_2level_size50_21_sram_blwl_out[2014:2014] ,mux_2level_size50_21_sram_blwl_out[2014:2014] ,mux_2level_size50_21_sram_blwl_outb[2014:2014] ,mux_2level_size50_21_configbus0[2014:2014], mux_2level_size50_21_configbus1[2014:2014] , mux_2level_size50_21_configbus0_b[2014:2014] );
sram6T_blwl sram_blwl_2015_ (mux_2level_size50_21_sram_blwl_out[2015:2015] ,mux_2level_size50_21_sram_blwl_out[2015:2015] ,mux_2level_size50_21_sram_blwl_outb[2015:2015] ,mux_2level_size50_21_configbus0[2015:2015], mux_2level_size50_21_configbus1[2015:2015] , mux_2level_size50_21_configbus0_b[2015:2015] );
sram6T_blwl sram_blwl_2016_ (mux_2level_size50_21_sram_blwl_out[2016:2016] ,mux_2level_size50_21_sram_blwl_out[2016:2016] ,mux_2level_size50_21_sram_blwl_outb[2016:2016] ,mux_2level_size50_21_configbus0[2016:2016], mux_2level_size50_21_configbus1[2016:2016] , mux_2level_size50_21_configbus0_b[2016:2016] );
sram6T_blwl sram_blwl_2017_ (mux_2level_size50_21_sram_blwl_out[2017:2017] ,mux_2level_size50_21_sram_blwl_out[2017:2017] ,mux_2level_size50_21_sram_blwl_outb[2017:2017] ,mux_2level_size50_21_configbus0[2017:2017], mux_2level_size50_21_configbus1[2017:2017] , mux_2level_size50_21_configbus0_b[2017:2017] );
wire [0:49] in_bus_mux_2level_size50_22_ ;
assign in_bus_mux_2level_size50_22_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_22_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_22_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_22_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_22_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_22_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_22_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_22_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_22_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_22_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_22_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_22_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_22_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_22_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_22_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_22_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_22_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_22_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_22_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_22_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_22_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_22_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_22_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_22_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_22_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_22_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_22_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_22_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_22_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_22_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_22_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_22_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_22_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_22_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_22_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_22_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_22_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_22_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_22_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_22_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_22_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_22_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_22_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_22_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_22_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_22_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_22_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_22_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_22_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_22_[49] = fle_9___out_0_ ; 
wire [2018:2033] mux_2level_size50_22_configbus0;
wire [2018:2033] mux_2level_size50_22_configbus1;
wire [2018:2033] mux_2level_size50_22_sram_blwl_out ;
wire [2018:2033] mux_2level_size50_22_sram_blwl_outb ;
assign mux_2level_size50_22_configbus0[2018:2033] = sram_blwl_bl[2018:2033] ;
assign mux_2level_size50_22_configbus1[2018:2033] = sram_blwl_wl[2018:2033] ;
wire [2018:2033] mux_2level_size50_22_configbus0_b;
assign mux_2level_size50_22_configbus0_b[2018:2033] = sram_blwl_blb[2018:2033] ;
mux_2level_size50 mux_2level_size50_22_ (in_bus_mux_2level_size50_22_, fle_3___in_4_, mux_2level_size50_22_sram_blwl_out[2018:2033] ,
mux_2level_size50_22_sram_blwl_outb[2018:2033] );
//----- SRAM bits for MUX[22], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2018_ (mux_2level_size50_22_sram_blwl_out[2018:2018] ,mux_2level_size50_22_sram_blwl_out[2018:2018] ,mux_2level_size50_22_sram_blwl_outb[2018:2018] ,mux_2level_size50_22_configbus0[2018:2018], mux_2level_size50_22_configbus1[2018:2018] , mux_2level_size50_22_configbus0_b[2018:2018] );
sram6T_blwl sram_blwl_2019_ (mux_2level_size50_22_sram_blwl_out[2019:2019] ,mux_2level_size50_22_sram_blwl_out[2019:2019] ,mux_2level_size50_22_sram_blwl_outb[2019:2019] ,mux_2level_size50_22_configbus0[2019:2019], mux_2level_size50_22_configbus1[2019:2019] , mux_2level_size50_22_configbus0_b[2019:2019] );
sram6T_blwl sram_blwl_2020_ (mux_2level_size50_22_sram_blwl_out[2020:2020] ,mux_2level_size50_22_sram_blwl_out[2020:2020] ,mux_2level_size50_22_sram_blwl_outb[2020:2020] ,mux_2level_size50_22_configbus0[2020:2020], mux_2level_size50_22_configbus1[2020:2020] , mux_2level_size50_22_configbus0_b[2020:2020] );
sram6T_blwl sram_blwl_2021_ (mux_2level_size50_22_sram_blwl_out[2021:2021] ,mux_2level_size50_22_sram_blwl_out[2021:2021] ,mux_2level_size50_22_sram_blwl_outb[2021:2021] ,mux_2level_size50_22_configbus0[2021:2021], mux_2level_size50_22_configbus1[2021:2021] , mux_2level_size50_22_configbus0_b[2021:2021] );
sram6T_blwl sram_blwl_2022_ (mux_2level_size50_22_sram_blwl_out[2022:2022] ,mux_2level_size50_22_sram_blwl_out[2022:2022] ,mux_2level_size50_22_sram_blwl_outb[2022:2022] ,mux_2level_size50_22_configbus0[2022:2022], mux_2level_size50_22_configbus1[2022:2022] , mux_2level_size50_22_configbus0_b[2022:2022] );
sram6T_blwl sram_blwl_2023_ (mux_2level_size50_22_sram_blwl_out[2023:2023] ,mux_2level_size50_22_sram_blwl_out[2023:2023] ,mux_2level_size50_22_sram_blwl_outb[2023:2023] ,mux_2level_size50_22_configbus0[2023:2023], mux_2level_size50_22_configbus1[2023:2023] , mux_2level_size50_22_configbus0_b[2023:2023] );
sram6T_blwl sram_blwl_2024_ (mux_2level_size50_22_sram_blwl_out[2024:2024] ,mux_2level_size50_22_sram_blwl_out[2024:2024] ,mux_2level_size50_22_sram_blwl_outb[2024:2024] ,mux_2level_size50_22_configbus0[2024:2024], mux_2level_size50_22_configbus1[2024:2024] , mux_2level_size50_22_configbus0_b[2024:2024] );
sram6T_blwl sram_blwl_2025_ (mux_2level_size50_22_sram_blwl_out[2025:2025] ,mux_2level_size50_22_sram_blwl_out[2025:2025] ,mux_2level_size50_22_sram_blwl_outb[2025:2025] ,mux_2level_size50_22_configbus0[2025:2025], mux_2level_size50_22_configbus1[2025:2025] , mux_2level_size50_22_configbus0_b[2025:2025] );
sram6T_blwl sram_blwl_2026_ (mux_2level_size50_22_sram_blwl_out[2026:2026] ,mux_2level_size50_22_sram_blwl_out[2026:2026] ,mux_2level_size50_22_sram_blwl_outb[2026:2026] ,mux_2level_size50_22_configbus0[2026:2026], mux_2level_size50_22_configbus1[2026:2026] , mux_2level_size50_22_configbus0_b[2026:2026] );
sram6T_blwl sram_blwl_2027_ (mux_2level_size50_22_sram_blwl_out[2027:2027] ,mux_2level_size50_22_sram_blwl_out[2027:2027] ,mux_2level_size50_22_sram_blwl_outb[2027:2027] ,mux_2level_size50_22_configbus0[2027:2027], mux_2level_size50_22_configbus1[2027:2027] , mux_2level_size50_22_configbus0_b[2027:2027] );
sram6T_blwl sram_blwl_2028_ (mux_2level_size50_22_sram_blwl_out[2028:2028] ,mux_2level_size50_22_sram_blwl_out[2028:2028] ,mux_2level_size50_22_sram_blwl_outb[2028:2028] ,mux_2level_size50_22_configbus0[2028:2028], mux_2level_size50_22_configbus1[2028:2028] , mux_2level_size50_22_configbus0_b[2028:2028] );
sram6T_blwl sram_blwl_2029_ (mux_2level_size50_22_sram_blwl_out[2029:2029] ,mux_2level_size50_22_sram_blwl_out[2029:2029] ,mux_2level_size50_22_sram_blwl_outb[2029:2029] ,mux_2level_size50_22_configbus0[2029:2029], mux_2level_size50_22_configbus1[2029:2029] , mux_2level_size50_22_configbus0_b[2029:2029] );
sram6T_blwl sram_blwl_2030_ (mux_2level_size50_22_sram_blwl_out[2030:2030] ,mux_2level_size50_22_sram_blwl_out[2030:2030] ,mux_2level_size50_22_sram_blwl_outb[2030:2030] ,mux_2level_size50_22_configbus0[2030:2030], mux_2level_size50_22_configbus1[2030:2030] , mux_2level_size50_22_configbus0_b[2030:2030] );
sram6T_blwl sram_blwl_2031_ (mux_2level_size50_22_sram_blwl_out[2031:2031] ,mux_2level_size50_22_sram_blwl_out[2031:2031] ,mux_2level_size50_22_sram_blwl_outb[2031:2031] ,mux_2level_size50_22_configbus0[2031:2031], mux_2level_size50_22_configbus1[2031:2031] , mux_2level_size50_22_configbus0_b[2031:2031] );
sram6T_blwl sram_blwl_2032_ (mux_2level_size50_22_sram_blwl_out[2032:2032] ,mux_2level_size50_22_sram_blwl_out[2032:2032] ,mux_2level_size50_22_sram_blwl_outb[2032:2032] ,mux_2level_size50_22_configbus0[2032:2032], mux_2level_size50_22_configbus1[2032:2032] , mux_2level_size50_22_configbus0_b[2032:2032] );
sram6T_blwl sram_blwl_2033_ (mux_2level_size50_22_sram_blwl_out[2033:2033] ,mux_2level_size50_22_sram_blwl_out[2033:2033] ,mux_2level_size50_22_sram_blwl_outb[2033:2033] ,mux_2level_size50_22_configbus0[2033:2033], mux_2level_size50_22_configbus1[2033:2033] , mux_2level_size50_22_configbus0_b[2033:2033] );
wire [0:49] in_bus_mux_2level_size50_23_ ;
assign in_bus_mux_2level_size50_23_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_23_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_23_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_23_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_23_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_23_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_23_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_23_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_23_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_23_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_23_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_23_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_23_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_23_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_23_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_23_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_23_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_23_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_23_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_23_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_23_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_23_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_23_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_23_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_23_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_23_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_23_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_23_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_23_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_23_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_23_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_23_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_23_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_23_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_23_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_23_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_23_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_23_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_23_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_23_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_23_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_23_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_23_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_23_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_23_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_23_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_23_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_23_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_23_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_23_[49] = fle_9___out_0_ ; 
wire [2034:2049] mux_2level_size50_23_configbus0;
wire [2034:2049] mux_2level_size50_23_configbus1;
wire [2034:2049] mux_2level_size50_23_sram_blwl_out ;
wire [2034:2049] mux_2level_size50_23_sram_blwl_outb ;
assign mux_2level_size50_23_configbus0[2034:2049] = sram_blwl_bl[2034:2049] ;
assign mux_2level_size50_23_configbus1[2034:2049] = sram_blwl_wl[2034:2049] ;
wire [2034:2049] mux_2level_size50_23_configbus0_b;
assign mux_2level_size50_23_configbus0_b[2034:2049] = sram_blwl_blb[2034:2049] ;
mux_2level_size50 mux_2level_size50_23_ (in_bus_mux_2level_size50_23_, fle_3___in_5_, mux_2level_size50_23_sram_blwl_out[2034:2049] ,
mux_2level_size50_23_sram_blwl_outb[2034:2049] );
//----- SRAM bits for MUX[23], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2034_ (mux_2level_size50_23_sram_blwl_out[2034:2034] ,mux_2level_size50_23_sram_blwl_out[2034:2034] ,mux_2level_size50_23_sram_blwl_outb[2034:2034] ,mux_2level_size50_23_configbus0[2034:2034], mux_2level_size50_23_configbus1[2034:2034] , mux_2level_size50_23_configbus0_b[2034:2034] );
sram6T_blwl sram_blwl_2035_ (mux_2level_size50_23_sram_blwl_out[2035:2035] ,mux_2level_size50_23_sram_blwl_out[2035:2035] ,mux_2level_size50_23_sram_blwl_outb[2035:2035] ,mux_2level_size50_23_configbus0[2035:2035], mux_2level_size50_23_configbus1[2035:2035] , mux_2level_size50_23_configbus0_b[2035:2035] );
sram6T_blwl sram_blwl_2036_ (mux_2level_size50_23_sram_blwl_out[2036:2036] ,mux_2level_size50_23_sram_blwl_out[2036:2036] ,mux_2level_size50_23_sram_blwl_outb[2036:2036] ,mux_2level_size50_23_configbus0[2036:2036], mux_2level_size50_23_configbus1[2036:2036] , mux_2level_size50_23_configbus0_b[2036:2036] );
sram6T_blwl sram_blwl_2037_ (mux_2level_size50_23_sram_blwl_out[2037:2037] ,mux_2level_size50_23_sram_blwl_out[2037:2037] ,mux_2level_size50_23_sram_blwl_outb[2037:2037] ,mux_2level_size50_23_configbus0[2037:2037], mux_2level_size50_23_configbus1[2037:2037] , mux_2level_size50_23_configbus0_b[2037:2037] );
sram6T_blwl sram_blwl_2038_ (mux_2level_size50_23_sram_blwl_out[2038:2038] ,mux_2level_size50_23_sram_blwl_out[2038:2038] ,mux_2level_size50_23_sram_blwl_outb[2038:2038] ,mux_2level_size50_23_configbus0[2038:2038], mux_2level_size50_23_configbus1[2038:2038] , mux_2level_size50_23_configbus0_b[2038:2038] );
sram6T_blwl sram_blwl_2039_ (mux_2level_size50_23_sram_blwl_out[2039:2039] ,mux_2level_size50_23_sram_blwl_out[2039:2039] ,mux_2level_size50_23_sram_blwl_outb[2039:2039] ,mux_2level_size50_23_configbus0[2039:2039], mux_2level_size50_23_configbus1[2039:2039] , mux_2level_size50_23_configbus0_b[2039:2039] );
sram6T_blwl sram_blwl_2040_ (mux_2level_size50_23_sram_blwl_out[2040:2040] ,mux_2level_size50_23_sram_blwl_out[2040:2040] ,mux_2level_size50_23_sram_blwl_outb[2040:2040] ,mux_2level_size50_23_configbus0[2040:2040], mux_2level_size50_23_configbus1[2040:2040] , mux_2level_size50_23_configbus0_b[2040:2040] );
sram6T_blwl sram_blwl_2041_ (mux_2level_size50_23_sram_blwl_out[2041:2041] ,mux_2level_size50_23_sram_blwl_out[2041:2041] ,mux_2level_size50_23_sram_blwl_outb[2041:2041] ,mux_2level_size50_23_configbus0[2041:2041], mux_2level_size50_23_configbus1[2041:2041] , mux_2level_size50_23_configbus0_b[2041:2041] );
sram6T_blwl sram_blwl_2042_ (mux_2level_size50_23_sram_blwl_out[2042:2042] ,mux_2level_size50_23_sram_blwl_out[2042:2042] ,mux_2level_size50_23_sram_blwl_outb[2042:2042] ,mux_2level_size50_23_configbus0[2042:2042], mux_2level_size50_23_configbus1[2042:2042] , mux_2level_size50_23_configbus0_b[2042:2042] );
sram6T_blwl sram_blwl_2043_ (mux_2level_size50_23_sram_blwl_out[2043:2043] ,mux_2level_size50_23_sram_blwl_out[2043:2043] ,mux_2level_size50_23_sram_blwl_outb[2043:2043] ,mux_2level_size50_23_configbus0[2043:2043], mux_2level_size50_23_configbus1[2043:2043] , mux_2level_size50_23_configbus0_b[2043:2043] );
sram6T_blwl sram_blwl_2044_ (mux_2level_size50_23_sram_blwl_out[2044:2044] ,mux_2level_size50_23_sram_blwl_out[2044:2044] ,mux_2level_size50_23_sram_blwl_outb[2044:2044] ,mux_2level_size50_23_configbus0[2044:2044], mux_2level_size50_23_configbus1[2044:2044] , mux_2level_size50_23_configbus0_b[2044:2044] );
sram6T_blwl sram_blwl_2045_ (mux_2level_size50_23_sram_blwl_out[2045:2045] ,mux_2level_size50_23_sram_blwl_out[2045:2045] ,mux_2level_size50_23_sram_blwl_outb[2045:2045] ,mux_2level_size50_23_configbus0[2045:2045], mux_2level_size50_23_configbus1[2045:2045] , mux_2level_size50_23_configbus0_b[2045:2045] );
sram6T_blwl sram_blwl_2046_ (mux_2level_size50_23_sram_blwl_out[2046:2046] ,mux_2level_size50_23_sram_blwl_out[2046:2046] ,mux_2level_size50_23_sram_blwl_outb[2046:2046] ,mux_2level_size50_23_configbus0[2046:2046], mux_2level_size50_23_configbus1[2046:2046] , mux_2level_size50_23_configbus0_b[2046:2046] );
sram6T_blwl sram_blwl_2047_ (mux_2level_size50_23_sram_blwl_out[2047:2047] ,mux_2level_size50_23_sram_blwl_out[2047:2047] ,mux_2level_size50_23_sram_blwl_outb[2047:2047] ,mux_2level_size50_23_configbus0[2047:2047], mux_2level_size50_23_configbus1[2047:2047] , mux_2level_size50_23_configbus0_b[2047:2047] );
sram6T_blwl sram_blwl_2048_ (mux_2level_size50_23_sram_blwl_out[2048:2048] ,mux_2level_size50_23_sram_blwl_out[2048:2048] ,mux_2level_size50_23_sram_blwl_outb[2048:2048] ,mux_2level_size50_23_configbus0[2048:2048], mux_2level_size50_23_configbus1[2048:2048] , mux_2level_size50_23_configbus0_b[2048:2048] );
sram6T_blwl sram_blwl_2049_ (mux_2level_size50_23_sram_blwl_out[2049:2049] ,mux_2level_size50_23_sram_blwl_out[2049:2049] ,mux_2level_size50_23_sram_blwl_outb[2049:2049] ,mux_2level_size50_23_configbus0[2049:2049], mux_2level_size50_23_configbus1[2049:2049] , mux_2level_size50_23_configbus0_b[2049:2049] );
direct_interc direct_interc_173_ (mode_clb___clk_0_, fle_3___clk_0_ );
wire [0:49] in_bus_mux_2level_size50_24_ ;
assign in_bus_mux_2level_size50_24_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_24_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_24_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_24_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_24_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_24_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_24_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_24_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_24_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_24_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_24_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_24_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_24_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_24_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_24_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_24_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_24_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_24_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_24_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_24_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_24_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_24_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_24_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_24_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_24_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_24_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_24_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_24_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_24_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_24_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_24_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_24_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_24_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_24_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_24_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_24_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_24_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_24_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_24_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_24_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_24_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_24_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_24_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_24_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_24_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_24_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_24_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_24_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_24_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_24_[49] = fle_9___out_0_ ; 
wire [2050:2065] mux_2level_size50_24_configbus0;
wire [2050:2065] mux_2level_size50_24_configbus1;
wire [2050:2065] mux_2level_size50_24_sram_blwl_out ;
wire [2050:2065] mux_2level_size50_24_sram_blwl_outb ;
assign mux_2level_size50_24_configbus0[2050:2065] = sram_blwl_bl[2050:2065] ;
assign mux_2level_size50_24_configbus1[2050:2065] = sram_blwl_wl[2050:2065] ;
wire [2050:2065] mux_2level_size50_24_configbus0_b;
assign mux_2level_size50_24_configbus0_b[2050:2065] = sram_blwl_blb[2050:2065] ;
mux_2level_size50 mux_2level_size50_24_ (in_bus_mux_2level_size50_24_, fle_4___in_0_, mux_2level_size50_24_sram_blwl_out[2050:2065] ,
mux_2level_size50_24_sram_blwl_outb[2050:2065] );
//----- SRAM bits for MUX[24], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2050_ (mux_2level_size50_24_sram_blwl_out[2050:2050] ,mux_2level_size50_24_sram_blwl_out[2050:2050] ,mux_2level_size50_24_sram_blwl_outb[2050:2050] ,mux_2level_size50_24_configbus0[2050:2050], mux_2level_size50_24_configbus1[2050:2050] , mux_2level_size50_24_configbus0_b[2050:2050] );
sram6T_blwl sram_blwl_2051_ (mux_2level_size50_24_sram_blwl_out[2051:2051] ,mux_2level_size50_24_sram_blwl_out[2051:2051] ,mux_2level_size50_24_sram_blwl_outb[2051:2051] ,mux_2level_size50_24_configbus0[2051:2051], mux_2level_size50_24_configbus1[2051:2051] , mux_2level_size50_24_configbus0_b[2051:2051] );
sram6T_blwl sram_blwl_2052_ (mux_2level_size50_24_sram_blwl_out[2052:2052] ,mux_2level_size50_24_sram_blwl_out[2052:2052] ,mux_2level_size50_24_sram_blwl_outb[2052:2052] ,mux_2level_size50_24_configbus0[2052:2052], mux_2level_size50_24_configbus1[2052:2052] , mux_2level_size50_24_configbus0_b[2052:2052] );
sram6T_blwl sram_blwl_2053_ (mux_2level_size50_24_sram_blwl_out[2053:2053] ,mux_2level_size50_24_sram_blwl_out[2053:2053] ,mux_2level_size50_24_sram_blwl_outb[2053:2053] ,mux_2level_size50_24_configbus0[2053:2053], mux_2level_size50_24_configbus1[2053:2053] , mux_2level_size50_24_configbus0_b[2053:2053] );
sram6T_blwl sram_blwl_2054_ (mux_2level_size50_24_sram_blwl_out[2054:2054] ,mux_2level_size50_24_sram_blwl_out[2054:2054] ,mux_2level_size50_24_sram_blwl_outb[2054:2054] ,mux_2level_size50_24_configbus0[2054:2054], mux_2level_size50_24_configbus1[2054:2054] , mux_2level_size50_24_configbus0_b[2054:2054] );
sram6T_blwl sram_blwl_2055_ (mux_2level_size50_24_sram_blwl_out[2055:2055] ,mux_2level_size50_24_sram_blwl_out[2055:2055] ,mux_2level_size50_24_sram_blwl_outb[2055:2055] ,mux_2level_size50_24_configbus0[2055:2055], mux_2level_size50_24_configbus1[2055:2055] , mux_2level_size50_24_configbus0_b[2055:2055] );
sram6T_blwl sram_blwl_2056_ (mux_2level_size50_24_sram_blwl_out[2056:2056] ,mux_2level_size50_24_sram_blwl_out[2056:2056] ,mux_2level_size50_24_sram_blwl_outb[2056:2056] ,mux_2level_size50_24_configbus0[2056:2056], mux_2level_size50_24_configbus1[2056:2056] , mux_2level_size50_24_configbus0_b[2056:2056] );
sram6T_blwl sram_blwl_2057_ (mux_2level_size50_24_sram_blwl_out[2057:2057] ,mux_2level_size50_24_sram_blwl_out[2057:2057] ,mux_2level_size50_24_sram_blwl_outb[2057:2057] ,mux_2level_size50_24_configbus0[2057:2057], mux_2level_size50_24_configbus1[2057:2057] , mux_2level_size50_24_configbus0_b[2057:2057] );
sram6T_blwl sram_blwl_2058_ (mux_2level_size50_24_sram_blwl_out[2058:2058] ,mux_2level_size50_24_sram_blwl_out[2058:2058] ,mux_2level_size50_24_sram_blwl_outb[2058:2058] ,mux_2level_size50_24_configbus0[2058:2058], mux_2level_size50_24_configbus1[2058:2058] , mux_2level_size50_24_configbus0_b[2058:2058] );
sram6T_blwl sram_blwl_2059_ (mux_2level_size50_24_sram_blwl_out[2059:2059] ,mux_2level_size50_24_sram_blwl_out[2059:2059] ,mux_2level_size50_24_sram_blwl_outb[2059:2059] ,mux_2level_size50_24_configbus0[2059:2059], mux_2level_size50_24_configbus1[2059:2059] , mux_2level_size50_24_configbus0_b[2059:2059] );
sram6T_blwl sram_blwl_2060_ (mux_2level_size50_24_sram_blwl_out[2060:2060] ,mux_2level_size50_24_sram_blwl_out[2060:2060] ,mux_2level_size50_24_sram_blwl_outb[2060:2060] ,mux_2level_size50_24_configbus0[2060:2060], mux_2level_size50_24_configbus1[2060:2060] , mux_2level_size50_24_configbus0_b[2060:2060] );
sram6T_blwl sram_blwl_2061_ (mux_2level_size50_24_sram_blwl_out[2061:2061] ,mux_2level_size50_24_sram_blwl_out[2061:2061] ,mux_2level_size50_24_sram_blwl_outb[2061:2061] ,mux_2level_size50_24_configbus0[2061:2061], mux_2level_size50_24_configbus1[2061:2061] , mux_2level_size50_24_configbus0_b[2061:2061] );
sram6T_blwl sram_blwl_2062_ (mux_2level_size50_24_sram_blwl_out[2062:2062] ,mux_2level_size50_24_sram_blwl_out[2062:2062] ,mux_2level_size50_24_sram_blwl_outb[2062:2062] ,mux_2level_size50_24_configbus0[2062:2062], mux_2level_size50_24_configbus1[2062:2062] , mux_2level_size50_24_configbus0_b[2062:2062] );
sram6T_blwl sram_blwl_2063_ (mux_2level_size50_24_sram_blwl_out[2063:2063] ,mux_2level_size50_24_sram_blwl_out[2063:2063] ,mux_2level_size50_24_sram_blwl_outb[2063:2063] ,mux_2level_size50_24_configbus0[2063:2063], mux_2level_size50_24_configbus1[2063:2063] , mux_2level_size50_24_configbus0_b[2063:2063] );
sram6T_blwl sram_blwl_2064_ (mux_2level_size50_24_sram_blwl_out[2064:2064] ,mux_2level_size50_24_sram_blwl_out[2064:2064] ,mux_2level_size50_24_sram_blwl_outb[2064:2064] ,mux_2level_size50_24_configbus0[2064:2064], mux_2level_size50_24_configbus1[2064:2064] , mux_2level_size50_24_configbus0_b[2064:2064] );
sram6T_blwl sram_blwl_2065_ (mux_2level_size50_24_sram_blwl_out[2065:2065] ,mux_2level_size50_24_sram_blwl_out[2065:2065] ,mux_2level_size50_24_sram_blwl_outb[2065:2065] ,mux_2level_size50_24_configbus0[2065:2065], mux_2level_size50_24_configbus1[2065:2065] , mux_2level_size50_24_configbus0_b[2065:2065] );
wire [0:49] in_bus_mux_2level_size50_25_ ;
assign in_bus_mux_2level_size50_25_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_25_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_25_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_25_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_25_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_25_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_25_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_25_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_25_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_25_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_25_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_25_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_25_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_25_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_25_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_25_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_25_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_25_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_25_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_25_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_25_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_25_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_25_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_25_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_25_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_25_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_25_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_25_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_25_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_25_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_25_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_25_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_25_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_25_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_25_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_25_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_25_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_25_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_25_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_25_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_25_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_25_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_25_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_25_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_25_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_25_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_25_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_25_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_25_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_25_[49] = fle_9___out_0_ ; 
wire [2066:2081] mux_2level_size50_25_configbus0;
wire [2066:2081] mux_2level_size50_25_configbus1;
wire [2066:2081] mux_2level_size50_25_sram_blwl_out ;
wire [2066:2081] mux_2level_size50_25_sram_blwl_outb ;
assign mux_2level_size50_25_configbus0[2066:2081] = sram_blwl_bl[2066:2081] ;
assign mux_2level_size50_25_configbus1[2066:2081] = sram_blwl_wl[2066:2081] ;
wire [2066:2081] mux_2level_size50_25_configbus0_b;
assign mux_2level_size50_25_configbus0_b[2066:2081] = sram_blwl_blb[2066:2081] ;
mux_2level_size50 mux_2level_size50_25_ (in_bus_mux_2level_size50_25_, fle_4___in_1_, mux_2level_size50_25_sram_blwl_out[2066:2081] ,
mux_2level_size50_25_sram_blwl_outb[2066:2081] );
//----- SRAM bits for MUX[25], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2066_ (mux_2level_size50_25_sram_blwl_out[2066:2066] ,mux_2level_size50_25_sram_blwl_out[2066:2066] ,mux_2level_size50_25_sram_blwl_outb[2066:2066] ,mux_2level_size50_25_configbus0[2066:2066], mux_2level_size50_25_configbus1[2066:2066] , mux_2level_size50_25_configbus0_b[2066:2066] );
sram6T_blwl sram_blwl_2067_ (mux_2level_size50_25_sram_blwl_out[2067:2067] ,mux_2level_size50_25_sram_blwl_out[2067:2067] ,mux_2level_size50_25_sram_blwl_outb[2067:2067] ,mux_2level_size50_25_configbus0[2067:2067], mux_2level_size50_25_configbus1[2067:2067] , mux_2level_size50_25_configbus0_b[2067:2067] );
sram6T_blwl sram_blwl_2068_ (mux_2level_size50_25_sram_blwl_out[2068:2068] ,mux_2level_size50_25_sram_blwl_out[2068:2068] ,mux_2level_size50_25_sram_blwl_outb[2068:2068] ,mux_2level_size50_25_configbus0[2068:2068], mux_2level_size50_25_configbus1[2068:2068] , mux_2level_size50_25_configbus0_b[2068:2068] );
sram6T_blwl sram_blwl_2069_ (mux_2level_size50_25_sram_blwl_out[2069:2069] ,mux_2level_size50_25_sram_blwl_out[2069:2069] ,mux_2level_size50_25_sram_blwl_outb[2069:2069] ,mux_2level_size50_25_configbus0[2069:2069], mux_2level_size50_25_configbus1[2069:2069] , mux_2level_size50_25_configbus0_b[2069:2069] );
sram6T_blwl sram_blwl_2070_ (mux_2level_size50_25_sram_blwl_out[2070:2070] ,mux_2level_size50_25_sram_blwl_out[2070:2070] ,mux_2level_size50_25_sram_blwl_outb[2070:2070] ,mux_2level_size50_25_configbus0[2070:2070], mux_2level_size50_25_configbus1[2070:2070] , mux_2level_size50_25_configbus0_b[2070:2070] );
sram6T_blwl sram_blwl_2071_ (mux_2level_size50_25_sram_blwl_out[2071:2071] ,mux_2level_size50_25_sram_blwl_out[2071:2071] ,mux_2level_size50_25_sram_blwl_outb[2071:2071] ,mux_2level_size50_25_configbus0[2071:2071], mux_2level_size50_25_configbus1[2071:2071] , mux_2level_size50_25_configbus0_b[2071:2071] );
sram6T_blwl sram_blwl_2072_ (mux_2level_size50_25_sram_blwl_out[2072:2072] ,mux_2level_size50_25_sram_blwl_out[2072:2072] ,mux_2level_size50_25_sram_blwl_outb[2072:2072] ,mux_2level_size50_25_configbus0[2072:2072], mux_2level_size50_25_configbus1[2072:2072] , mux_2level_size50_25_configbus0_b[2072:2072] );
sram6T_blwl sram_blwl_2073_ (mux_2level_size50_25_sram_blwl_out[2073:2073] ,mux_2level_size50_25_sram_blwl_out[2073:2073] ,mux_2level_size50_25_sram_blwl_outb[2073:2073] ,mux_2level_size50_25_configbus0[2073:2073], mux_2level_size50_25_configbus1[2073:2073] , mux_2level_size50_25_configbus0_b[2073:2073] );
sram6T_blwl sram_blwl_2074_ (mux_2level_size50_25_sram_blwl_out[2074:2074] ,mux_2level_size50_25_sram_blwl_out[2074:2074] ,mux_2level_size50_25_sram_blwl_outb[2074:2074] ,mux_2level_size50_25_configbus0[2074:2074], mux_2level_size50_25_configbus1[2074:2074] , mux_2level_size50_25_configbus0_b[2074:2074] );
sram6T_blwl sram_blwl_2075_ (mux_2level_size50_25_sram_blwl_out[2075:2075] ,mux_2level_size50_25_sram_blwl_out[2075:2075] ,mux_2level_size50_25_sram_blwl_outb[2075:2075] ,mux_2level_size50_25_configbus0[2075:2075], mux_2level_size50_25_configbus1[2075:2075] , mux_2level_size50_25_configbus0_b[2075:2075] );
sram6T_blwl sram_blwl_2076_ (mux_2level_size50_25_sram_blwl_out[2076:2076] ,mux_2level_size50_25_sram_blwl_out[2076:2076] ,mux_2level_size50_25_sram_blwl_outb[2076:2076] ,mux_2level_size50_25_configbus0[2076:2076], mux_2level_size50_25_configbus1[2076:2076] , mux_2level_size50_25_configbus0_b[2076:2076] );
sram6T_blwl sram_blwl_2077_ (mux_2level_size50_25_sram_blwl_out[2077:2077] ,mux_2level_size50_25_sram_blwl_out[2077:2077] ,mux_2level_size50_25_sram_blwl_outb[2077:2077] ,mux_2level_size50_25_configbus0[2077:2077], mux_2level_size50_25_configbus1[2077:2077] , mux_2level_size50_25_configbus0_b[2077:2077] );
sram6T_blwl sram_blwl_2078_ (mux_2level_size50_25_sram_blwl_out[2078:2078] ,mux_2level_size50_25_sram_blwl_out[2078:2078] ,mux_2level_size50_25_sram_blwl_outb[2078:2078] ,mux_2level_size50_25_configbus0[2078:2078], mux_2level_size50_25_configbus1[2078:2078] , mux_2level_size50_25_configbus0_b[2078:2078] );
sram6T_blwl sram_blwl_2079_ (mux_2level_size50_25_sram_blwl_out[2079:2079] ,mux_2level_size50_25_sram_blwl_out[2079:2079] ,mux_2level_size50_25_sram_blwl_outb[2079:2079] ,mux_2level_size50_25_configbus0[2079:2079], mux_2level_size50_25_configbus1[2079:2079] , mux_2level_size50_25_configbus0_b[2079:2079] );
sram6T_blwl sram_blwl_2080_ (mux_2level_size50_25_sram_blwl_out[2080:2080] ,mux_2level_size50_25_sram_blwl_out[2080:2080] ,mux_2level_size50_25_sram_blwl_outb[2080:2080] ,mux_2level_size50_25_configbus0[2080:2080], mux_2level_size50_25_configbus1[2080:2080] , mux_2level_size50_25_configbus0_b[2080:2080] );
sram6T_blwl sram_blwl_2081_ (mux_2level_size50_25_sram_blwl_out[2081:2081] ,mux_2level_size50_25_sram_blwl_out[2081:2081] ,mux_2level_size50_25_sram_blwl_outb[2081:2081] ,mux_2level_size50_25_configbus0[2081:2081], mux_2level_size50_25_configbus1[2081:2081] , mux_2level_size50_25_configbus0_b[2081:2081] );
wire [0:49] in_bus_mux_2level_size50_26_ ;
assign in_bus_mux_2level_size50_26_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_26_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_26_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_26_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_26_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_26_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_26_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_26_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_26_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_26_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_26_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_26_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_26_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_26_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_26_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_26_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_26_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_26_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_26_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_26_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_26_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_26_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_26_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_26_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_26_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_26_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_26_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_26_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_26_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_26_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_26_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_26_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_26_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_26_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_26_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_26_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_26_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_26_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_26_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_26_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_26_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_26_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_26_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_26_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_26_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_26_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_26_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_26_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_26_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_26_[49] = fle_9___out_0_ ; 
wire [2082:2097] mux_2level_size50_26_configbus0;
wire [2082:2097] mux_2level_size50_26_configbus1;
wire [2082:2097] mux_2level_size50_26_sram_blwl_out ;
wire [2082:2097] mux_2level_size50_26_sram_blwl_outb ;
assign mux_2level_size50_26_configbus0[2082:2097] = sram_blwl_bl[2082:2097] ;
assign mux_2level_size50_26_configbus1[2082:2097] = sram_blwl_wl[2082:2097] ;
wire [2082:2097] mux_2level_size50_26_configbus0_b;
assign mux_2level_size50_26_configbus0_b[2082:2097] = sram_blwl_blb[2082:2097] ;
mux_2level_size50 mux_2level_size50_26_ (in_bus_mux_2level_size50_26_, fle_4___in_2_, mux_2level_size50_26_sram_blwl_out[2082:2097] ,
mux_2level_size50_26_sram_blwl_outb[2082:2097] );
//----- SRAM bits for MUX[26], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2082_ (mux_2level_size50_26_sram_blwl_out[2082:2082] ,mux_2level_size50_26_sram_blwl_out[2082:2082] ,mux_2level_size50_26_sram_blwl_outb[2082:2082] ,mux_2level_size50_26_configbus0[2082:2082], mux_2level_size50_26_configbus1[2082:2082] , mux_2level_size50_26_configbus0_b[2082:2082] );
sram6T_blwl sram_blwl_2083_ (mux_2level_size50_26_sram_blwl_out[2083:2083] ,mux_2level_size50_26_sram_blwl_out[2083:2083] ,mux_2level_size50_26_sram_blwl_outb[2083:2083] ,mux_2level_size50_26_configbus0[2083:2083], mux_2level_size50_26_configbus1[2083:2083] , mux_2level_size50_26_configbus0_b[2083:2083] );
sram6T_blwl sram_blwl_2084_ (mux_2level_size50_26_sram_blwl_out[2084:2084] ,mux_2level_size50_26_sram_blwl_out[2084:2084] ,mux_2level_size50_26_sram_blwl_outb[2084:2084] ,mux_2level_size50_26_configbus0[2084:2084], mux_2level_size50_26_configbus1[2084:2084] , mux_2level_size50_26_configbus0_b[2084:2084] );
sram6T_blwl sram_blwl_2085_ (mux_2level_size50_26_sram_blwl_out[2085:2085] ,mux_2level_size50_26_sram_blwl_out[2085:2085] ,mux_2level_size50_26_sram_blwl_outb[2085:2085] ,mux_2level_size50_26_configbus0[2085:2085], mux_2level_size50_26_configbus1[2085:2085] , mux_2level_size50_26_configbus0_b[2085:2085] );
sram6T_blwl sram_blwl_2086_ (mux_2level_size50_26_sram_blwl_out[2086:2086] ,mux_2level_size50_26_sram_blwl_out[2086:2086] ,mux_2level_size50_26_sram_blwl_outb[2086:2086] ,mux_2level_size50_26_configbus0[2086:2086], mux_2level_size50_26_configbus1[2086:2086] , mux_2level_size50_26_configbus0_b[2086:2086] );
sram6T_blwl sram_blwl_2087_ (mux_2level_size50_26_sram_blwl_out[2087:2087] ,mux_2level_size50_26_sram_blwl_out[2087:2087] ,mux_2level_size50_26_sram_blwl_outb[2087:2087] ,mux_2level_size50_26_configbus0[2087:2087], mux_2level_size50_26_configbus1[2087:2087] , mux_2level_size50_26_configbus0_b[2087:2087] );
sram6T_blwl sram_blwl_2088_ (mux_2level_size50_26_sram_blwl_out[2088:2088] ,mux_2level_size50_26_sram_blwl_out[2088:2088] ,mux_2level_size50_26_sram_blwl_outb[2088:2088] ,mux_2level_size50_26_configbus0[2088:2088], mux_2level_size50_26_configbus1[2088:2088] , mux_2level_size50_26_configbus0_b[2088:2088] );
sram6T_blwl sram_blwl_2089_ (mux_2level_size50_26_sram_blwl_out[2089:2089] ,mux_2level_size50_26_sram_blwl_out[2089:2089] ,mux_2level_size50_26_sram_blwl_outb[2089:2089] ,mux_2level_size50_26_configbus0[2089:2089], mux_2level_size50_26_configbus1[2089:2089] , mux_2level_size50_26_configbus0_b[2089:2089] );
sram6T_blwl sram_blwl_2090_ (mux_2level_size50_26_sram_blwl_out[2090:2090] ,mux_2level_size50_26_sram_blwl_out[2090:2090] ,mux_2level_size50_26_sram_blwl_outb[2090:2090] ,mux_2level_size50_26_configbus0[2090:2090], mux_2level_size50_26_configbus1[2090:2090] , mux_2level_size50_26_configbus0_b[2090:2090] );
sram6T_blwl sram_blwl_2091_ (mux_2level_size50_26_sram_blwl_out[2091:2091] ,mux_2level_size50_26_sram_blwl_out[2091:2091] ,mux_2level_size50_26_sram_blwl_outb[2091:2091] ,mux_2level_size50_26_configbus0[2091:2091], mux_2level_size50_26_configbus1[2091:2091] , mux_2level_size50_26_configbus0_b[2091:2091] );
sram6T_blwl sram_blwl_2092_ (mux_2level_size50_26_sram_blwl_out[2092:2092] ,mux_2level_size50_26_sram_blwl_out[2092:2092] ,mux_2level_size50_26_sram_blwl_outb[2092:2092] ,mux_2level_size50_26_configbus0[2092:2092], mux_2level_size50_26_configbus1[2092:2092] , mux_2level_size50_26_configbus0_b[2092:2092] );
sram6T_blwl sram_blwl_2093_ (mux_2level_size50_26_sram_blwl_out[2093:2093] ,mux_2level_size50_26_sram_blwl_out[2093:2093] ,mux_2level_size50_26_sram_blwl_outb[2093:2093] ,mux_2level_size50_26_configbus0[2093:2093], mux_2level_size50_26_configbus1[2093:2093] , mux_2level_size50_26_configbus0_b[2093:2093] );
sram6T_blwl sram_blwl_2094_ (mux_2level_size50_26_sram_blwl_out[2094:2094] ,mux_2level_size50_26_sram_blwl_out[2094:2094] ,mux_2level_size50_26_sram_blwl_outb[2094:2094] ,mux_2level_size50_26_configbus0[2094:2094], mux_2level_size50_26_configbus1[2094:2094] , mux_2level_size50_26_configbus0_b[2094:2094] );
sram6T_blwl sram_blwl_2095_ (mux_2level_size50_26_sram_blwl_out[2095:2095] ,mux_2level_size50_26_sram_blwl_out[2095:2095] ,mux_2level_size50_26_sram_blwl_outb[2095:2095] ,mux_2level_size50_26_configbus0[2095:2095], mux_2level_size50_26_configbus1[2095:2095] , mux_2level_size50_26_configbus0_b[2095:2095] );
sram6T_blwl sram_blwl_2096_ (mux_2level_size50_26_sram_blwl_out[2096:2096] ,mux_2level_size50_26_sram_blwl_out[2096:2096] ,mux_2level_size50_26_sram_blwl_outb[2096:2096] ,mux_2level_size50_26_configbus0[2096:2096], mux_2level_size50_26_configbus1[2096:2096] , mux_2level_size50_26_configbus0_b[2096:2096] );
sram6T_blwl sram_blwl_2097_ (mux_2level_size50_26_sram_blwl_out[2097:2097] ,mux_2level_size50_26_sram_blwl_out[2097:2097] ,mux_2level_size50_26_sram_blwl_outb[2097:2097] ,mux_2level_size50_26_configbus0[2097:2097], mux_2level_size50_26_configbus1[2097:2097] , mux_2level_size50_26_configbus0_b[2097:2097] );
wire [0:49] in_bus_mux_2level_size50_27_ ;
assign in_bus_mux_2level_size50_27_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_27_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_27_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_27_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_27_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_27_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_27_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_27_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_27_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_27_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_27_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_27_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_27_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_27_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_27_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_27_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_27_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_27_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_27_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_27_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_27_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_27_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_27_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_27_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_27_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_27_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_27_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_27_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_27_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_27_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_27_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_27_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_27_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_27_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_27_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_27_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_27_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_27_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_27_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_27_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_27_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_27_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_27_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_27_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_27_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_27_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_27_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_27_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_27_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_27_[49] = fle_9___out_0_ ; 
wire [2098:2113] mux_2level_size50_27_configbus0;
wire [2098:2113] mux_2level_size50_27_configbus1;
wire [2098:2113] mux_2level_size50_27_sram_blwl_out ;
wire [2098:2113] mux_2level_size50_27_sram_blwl_outb ;
assign mux_2level_size50_27_configbus0[2098:2113] = sram_blwl_bl[2098:2113] ;
assign mux_2level_size50_27_configbus1[2098:2113] = sram_blwl_wl[2098:2113] ;
wire [2098:2113] mux_2level_size50_27_configbus0_b;
assign mux_2level_size50_27_configbus0_b[2098:2113] = sram_blwl_blb[2098:2113] ;
mux_2level_size50 mux_2level_size50_27_ (in_bus_mux_2level_size50_27_, fle_4___in_3_, mux_2level_size50_27_sram_blwl_out[2098:2113] ,
mux_2level_size50_27_sram_blwl_outb[2098:2113] );
//----- SRAM bits for MUX[27], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2098_ (mux_2level_size50_27_sram_blwl_out[2098:2098] ,mux_2level_size50_27_sram_blwl_out[2098:2098] ,mux_2level_size50_27_sram_blwl_outb[2098:2098] ,mux_2level_size50_27_configbus0[2098:2098], mux_2level_size50_27_configbus1[2098:2098] , mux_2level_size50_27_configbus0_b[2098:2098] );
sram6T_blwl sram_blwl_2099_ (mux_2level_size50_27_sram_blwl_out[2099:2099] ,mux_2level_size50_27_sram_blwl_out[2099:2099] ,mux_2level_size50_27_sram_blwl_outb[2099:2099] ,mux_2level_size50_27_configbus0[2099:2099], mux_2level_size50_27_configbus1[2099:2099] , mux_2level_size50_27_configbus0_b[2099:2099] );
sram6T_blwl sram_blwl_2100_ (mux_2level_size50_27_sram_blwl_out[2100:2100] ,mux_2level_size50_27_sram_blwl_out[2100:2100] ,mux_2level_size50_27_sram_blwl_outb[2100:2100] ,mux_2level_size50_27_configbus0[2100:2100], mux_2level_size50_27_configbus1[2100:2100] , mux_2level_size50_27_configbus0_b[2100:2100] );
sram6T_blwl sram_blwl_2101_ (mux_2level_size50_27_sram_blwl_out[2101:2101] ,mux_2level_size50_27_sram_blwl_out[2101:2101] ,mux_2level_size50_27_sram_blwl_outb[2101:2101] ,mux_2level_size50_27_configbus0[2101:2101], mux_2level_size50_27_configbus1[2101:2101] , mux_2level_size50_27_configbus0_b[2101:2101] );
sram6T_blwl sram_blwl_2102_ (mux_2level_size50_27_sram_blwl_out[2102:2102] ,mux_2level_size50_27_sram_blwl_out[2102:2102] ,mux_2level_size50_27_sram_blwl_outb[2102:2102] ,mux_2level_size50_27_configbus0[2102:2102], mux_2level_size50_27_configbus1[2102:2102] , mux_2level_size50_27_configbus0_b[2102:2102] );
sram6T_blwl sram_blwl_2103_ (mux_2level_size50_27_sram_blwl_out[2103:2103] ,mux_2level_size50_27_sram_blwl_out[2103:2103] ,mux_2level_size50_27_sram_blwl_outb[2103:2103] ,mux_2level_size50_27_configbus0[2103:2103], mux_2level_size50_27_configbus1[2103:2103] , mux_2level_size50_27_configbus0_b[2103:2103] );
sram6T_blwl sram_blwl_2104_ (mux_2level_size50_27_sram_blwl_out[2104:2104] ,mux_2level_size50_27_sram_blwl_out[2104:2104] ,mux_2level_size50_27_sram_blwl_outb[2104:2104] ,mux_2level_size50_27_configbus0[2104:2104], mux_2level_size50_27_configbus1[2104:2104] , mux_2level_size50_27_configbus0_b[2104:2104] );
sram6T_blwl sram_blwl_2105_ (mux_2level_size50_27_sram_blwl_out[2105:2105] ,mux_2level_size50_27_sram_blwl_out[2105:2105] ,mux_2level_size50_27_sram_blwl_outb[2105:2105] ,mux_2level_size50_27_configbus0[2105:2105], mux_2level_size50_27_configbus1[2105:2105] , mux_2level_size50_27_configbus0_b[2105:2105] );
sram6T_blwl sram_blwl_2106_ (mux_2level_size50_27_sram_blwl_out[2106:2106] ,mux_2level_size50_27_sram_blwl_out[2106:2106] ,mux_2level_size50_27_sram_blwl_outb[2106:2106] ,mux_2level_size50_27_configbus0[2106:2106], mux_2level_size50_27_configbus1[2106:2106] , mux_2level_size50_27_configbus0_b[2106:2106] );
sram6T_blwl sram_blwl_2107_ (mux_2level_size50_27_sram_blwl_out[2107:2107] ,mux_2level_size50_27_sram_blwl_out[2107:2107] ,mux_2level_size50_27_sram_blwl_outb[2107:2107] ,mux_2level_size50_27_configbus0[2107:2107], mux_2level_size50_27_configbus1[2107:2107] , mux_2level_size50_27_configbus0_b[2107:2107] );
sram6T_blwl sram_blwl_2108_ (mux_2level_size50_27_sram_blwl_out[2108:2108] ,mux_2level_size50_27_sram_blwl_out[2108:2108] ,mux_2level_size50_27_sram_blwl_outb[2108:2108] ,mux_2level_size50_27_configbus0[2108:2108], mux_2level_size50_27_configbus1[2108:2108] , mux_2level_size50_27_configbus0_b[2108:2108] );
sram6T_blwl sram_blwl_2109_ (mux_2level_size50_27_sram_blwl_out[2109:2109] ,mux_2level_size50_27_sram_blwl_out[2109:2109] ,mux_2level_size50_27_sram_blwl_outb[2109:2109] ,mux_2level_size50_27_configbus0[2109:2109], mux_2level_size50_27_configbus1[2109:2109] , mux_2level_size50_27_configbus0_b[2109:2109] );
sram6T_blwl sram_blwl_2110_ (mux_2level_size50_27_sram_blwl_out[2110:2110] ,mux_2level_size50_27_sram_blwl_out[2110:2110] ,mux_2level_size50_27_sram_blwl_outb[2110:2110] ,mux_2level_size50_27_configbus0[2110:2110], mux_2level_size50_27_configbus1[2110:2110] , mux_2level_size50_27_configbus0_b[2110:2110] );
sram6T_blwl sram_blwl_2111_ (mux_2level_size50_27_sram_blwl_out[2111:2111] ,mux_2level_size50_27_sram_blwl_out[2111:2111] ,mux_2level_size50_27_sram_blwl_outb[2111:2111] ,mux_2level_size50_27_configbus0[2111:2111], mux_2level_size50_27_configbus1[2111:2111] , mux_2level_size50_27_configbus0_b[2111:2111] );
sram6T_blwl sram_blwl_2112_ (mux_2level_size50_27_sram_blwl_out[2112:2112] ,mux_2level_size50_27_sram_blwl_out[2112:2112] ,mux_2level_size50_27_sram_blwl_outb[2112:2112] ,mux_2level_size50_27_configbus0[2112:2112], mux_2level_size50_27_configbus1[2112:2112] , mux_2level_size50_27_configbus0_b[2112:2112] );
sram6T_blwl sram_blwl_2113_ (mux_2level_size50_27_sram_blwl_out[2113:2113] ,mux_2level_size50_27_sram_blwl_out[2113:2113] ,mux_2level_size50_27_sram_blwl_outb[2113:2113] ,mux_2level_size50_27_configbus0[2113:2113], mux_2level_size50_27_configbus1[2113:2113] , mux_2level_size50_27_configbus0_b[2113:2113] );
wire [0:49] in_bus_mux_2level_size50_28_ ;
assign in_bus_mux_2level_size50_28_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_28_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_28_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_28_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_28_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_28_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_28_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_28_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_28_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_28_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_28_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_28_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_28_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_28_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_28_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_28_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_28_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_28_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_28_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_28_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_28_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_28_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_28_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_28_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_28_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_28_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_28_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_28_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_28_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_28_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_28_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_28_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_28_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_28_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_28_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_28_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_28_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_28_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_28_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_28_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_28_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_28_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_28_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_28_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_28_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_28_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_28_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_28_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_28_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_28_[49] = fle_9___out_0_ ; 
wire [2114:2129] mux_2level_size50_28_configbus0;
wire [2114:2129] mux_2level_size50_28_configbus1;
wire [2114:2129] mux_2level_size50_28_sram_blwl_out ;
wire [2114:2129] mux_2level_size50_28_sram_blwl_outb ;
assign mux_2level_size50_28_configbus0[2114:2129] = sram_blwl_bl[2114:2129] ;
assign mux_2level_size50_28_configbus1[2114:2129] = sram_blwl_wl[2114:2129] ;
wire [2114:2129] mux_2level_size50_28_configbus0_b;
assign mux_2level_size50_28_configbus0_b[2114:2129] = sram_blwl_blb[2114:2129] ;
mux_2level_size50 mux_2level_size50_28_ (in_bus_mux_2level_size50_28_, fle_4___in_4_, mux_2level_size50_28_sram_blwl_out[2114:2129] ,
mux_2level_size50_28_sram_blwl_outb[2114:2129] );
//----- SRAM bits for MUX[28], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2114_ (mux_2level_size50_28_sram_blwl_out[2114:2114] ,mux_2level_size50_28_sram_blwl_out[2114:2114] ,mux_2level_size50_28_sram_blwl_outb[2114:2114] ,mux_2level_size50_28_configbus0[2114:2114], mux_2level_size50_28_configbus1[2114:2114] , mux_2level_size50_28_configbus0_b[2114:2114] );
sram6T_blwl sram_blwl_2115_ (mux_2level_size50_28_sram_blwl_out[2115:2115] ,mux_2level_size50_28_sram_blwl_out[2115:2115] ,mux_2level_size50_28_sram_blwl_outb[2115:2115] ,mux_2level_size50_28_configbus0[2115:2115], mux_2level_size50_28_configbus1[2115:2115] , mux_2level_size50_28_configbus0_b[2115:2115] );
sram6T_blwl sram_blwl_2116_ (mux_2level_size50_28_sram_blwl_out[2116:2116] ,mux_2level_size50_28_sram_blwl_out[2116:2116] ,mux_2level_size50_28_sram_blwl_outb[2116:2116] ,mux_2level_size50_28_configbus0[2116:2116], mux_2level_size50_28_configbus1[2116:2116] , mux_2level_size50_28_configbus0_b[2116:2116] );
sram6T_blwl sram_blwl_2117_ (mux_2level_size50_28_sram_blwl_out[2117:2117] ,mux_2level_size50_28_sram_blwl_out[2117:2117] ,mux_2level_size50_28_sram_blwl_outb[2117:2117] ,mux_2level_size50_28_configbus0[2117:2117], mux_2level_size50_28_configbus1[2117:2117] , mux_2level_size50_28_configbus0_b[2117:2117] );
sram6T_blwl sram_blwl_2118_ (mux_2level_size50_28_sram_blwl_out[2118:2118] ,mux_2level_size50_28_sram_blwl_out[2118:2118] ,mux_2level_size50_28_sram_blwl_outb[2118:2118] ,mux_2level_size50_28_configbus0[2118:2118], mux_2level_size50_28_configbus1[2118:2118] , mux_2level_size50_28_configbus0_b[2118:2118] );
sram6T_blwl sram_blwl_2119_ (mux_2level_size50_28_sram_blwl_out[2119:2119] ,mux_2level_size50_28_sram_blwl_out[2119:2119] ,mux_2level_size50_28_sram_blwl_outb[2119:2119] ,mux_2level_size50_28_configbus0[2119:2119], mux_2level_size50_28_configbus1[2119:2119] , mux_2level_size50_28_configbus0_b[2119:2119] );
sram6T_blwl sram_blwl_2120_ (mux_2level_size50_28_sram_blwl_out[2120:2120] ,mux_2level_size50_28_sram_blwl_out[2120:2120] ,mux_2level_size50_28_sram_blwl_outb[2120:2120] ,mux_2level_size50_28_configbus0[2120:2120], mux_2level_size50_28_configbus1[2120:2120] , mux_2level_size50_28_configbus0_b[2120:2120] );
sram6T_blwl sram_blwl_2121_ (mux_2level_size50_28_sram_blwl_out[2121:2121] ,mux_2level_size50_28_sram_blwl_out[2121:2121] ,mux_2level_size50_28_sram_blwl_outb[2121:2121] ,mux_2level_size50_28_configbus0[2121:2121], mux_2level_size50_28_configbus1[2121:2121] , mux_2level_size50_28_configbus0_b[2121:2121] );
sram6T_blwl sram_blwl_2122_ (mux_2level_size50_28_sram_blwl_out[2122:2122] ,mux_2level_size50_28_sram_blwl_out[2122:2122] ,mux_2level_size50_28_sram_blwl_outb[2122:2122] ,mux_2level_size50_28_configbus0[2122:2122], mux_2level_size50_28_configbus1[2122:2122] , mux_2level_size50_28_configbus0_b[2122:2122] );
sram6T_blwl sram_blwl_2123_ (mux_2level_size50_28_sram_blwl_out[2123:2123] ,mux_2level_size50_28_sram_blwl_out[2123:2123] ,mux_2level_size50_28_sram_blwl_outb[2123:2123] ,mux_2level_size50_28_configbus0[2123:2123], mux_2level_size50_28_configbus1[2123:2123] , mux_2level_size50_28_configbus0_b[2123:2123] );
sram6T_blwl sram_blwl_2124_ (mux_2level_size50_28_sram_blwl_out[2124:2124] ,mux_2level_size50_28_sram_blwl_out[2124:2124] ,mux_2level_size50_28_sram_blwl_outb[2124:2124] ,mux_2level_size50_28_configbus0[2124:2124], mux_2level_size50_28_configbus1[2124:2124] , mux_2level_size50_28_configbus0_b[2124:2124] );
sram6T_blwl sram_blwl_2125_ (mux_2level_size50_28_sram_blwl_out[2125:2125] ,mux_2level_size50_28_sram_blwl_out[2125:2125] ,mux_2level_size50_28_sram_blwl_outb[2125:2125] ,mux_2level_size50_28_configbus0[2125:2125], mux_2level_size50_28_configbus1[2125:2125] , mux_2level_size50_28_configbus0_b[2125:2125] );
sram6T_blwl sram_blwl_2126_ (mux_2level_size50_28_sram_blwl_out[2126:2126] ,mux_2level_size50_28_sram_blwl_out[2126:2126] ,mux_2level_size50_28_sram_blwl_outb[2126:2126] ,mux_2level_size50_28_configbus0[2126:2126], mux_2level_size50_28_configbus1[2126:2126] , mux_2level_size50_28_configbus0_b[2126:2126] );
sram6T_blwl sram_blwl_2127_ (mux_2level_size50_28_sram_blwl_out[2127:2127] ,mux_2level_size50_28_sram_blwl_out[2127:2127] ,mux_2level_size50_28_sram_blwl_outb[2127:2127] ,mux_2level_size50_28_configbus0[2127:2127], mux_2level_size50_28_configbus1[2127:2127] , mux_2level_size50_28_configbus0_b[2127:2127] );
sram6T_blwl sram_blwl_2128_ (mux_2level_size50_28_sram_blwl_out[2128:2128] ,mux_2level_size50_28_sram_blwl_out[2128:2128] ,mux_2level_size50_28_sram_blwl_outb[2128:2128] ,mux_2level_size50_28_configbus0[2128:2128], mux_2level_size50_28_configbus1[2128:2128] , mux_2level_size50_28_configbus0_b[2128:2128] );
sram6T_blwl sram_blwl_2129_ (mux_2level_size50_28_sram_blwl_out[2129:2129] ,mux_2level_size50_28_sram_blwl_out[2129:2129] ,mux_2level_size50_28_sram_blwl_outb[2129:2129] ,mux_2level_size50_28_configbus0[2129:2129], mux_2level_size50_28_configbus1[2129:2129] , mux_2level_size50_28_configbus0_b[2129:2129] );
wire [0:49] in_bus_mux_2level_size50_29_ ;
assign in_bus_mux_2level_size50_29_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_29_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_29_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_29_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_29_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_29_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_29_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_29_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_29_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_29_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_29_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_29_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_29_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_29_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_29_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_29_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_29_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_29_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_29_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_29_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_29_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_29_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_29_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_29_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_29_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_29_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_29_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_29_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_29_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_29_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_29_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_29_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_29_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_29_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_29_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_29_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_29_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_29_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_29_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_29_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_29_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_29_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_29_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_29_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_29_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_29_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_29_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_29_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_29_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_29_[49] = fle_9___out_0_ ; 
wire [2130:2145] mux_2level_size50_29_configbus0;
wire [2130:2145] mux_2level_size50_29_configbus1;
wire [2130:2145] mux_2level_size50_29_sram_blwl_out ;
wire [2130:2145] mux_2level_size50_29_sram_blwl_outb ;
assign mux_2level_size50_29_configbus0[2130:2145] = sram_blwl_bl[2130:2145] ;
assign mux_2level_size50_29_configbus1[2130:2145] = sram_blwl_wl[2130:2145] ;
wire [2130:2145] mux_2level_size50_29_configbus0_b;
assign mux_2level_size50_29_configbus0_b[2130:2145] = sram_blwl_blb[2130:2145] ;
mux_2level_size50 mux_2level_size50_29_ (in_bus_mux_2level_size50_29_, fle_4___in_5_, mux_2level_size50_29_sram_blwl_out[2130:2145] ,
mux_2level_size50_29_sram_blwl_outb[2130:2145] );
//----- SRAM bits for MUX[29], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2130_ (mux_2level_size50_29_sram_blwl_out[2130:2130] ,mux_2level_size50_29_sram_blwl_out[2130:2130] ,mux_2level_size50_29_sram_blwl_outb[2130:2130] ,mux_2level_size50_29_configbus0[2130:2130], mux_2level_size50_29_configbus1[2130:2130] , mux_2level_size50_29_configbus0_b[2130:2130] );
sram6T_blwl sram_blwl_2131_ (mux_2level_size50_29_sram_blwl_out[2131:2131] ,mux_2level_size50_29_sram_blwl_out[2131:2131] ,mux_2level_size50_29_sram_blwl_outb[2131:2131] ,mux_2level_size50_29_configbus0[2131:2131], mux_2level_size50_29_configbus1[2131:2131] , mux_2level_size50_29_configbus0_b[2131:2131] );
sram6T_blwl sram_blwl_2132_ (mux_2level_size50_29_sram_blwl_out[2132:2132] ,mux_2level_size50_29_sram_blwl_out[2132:2132] ,mux_2level_size50_29_sram_blwl_outb[2132:2132] ,mux_2level_size50_29_configbus0[2132:2132], mux_2level_size50_29_configbus1[2132:2132] , mux_2level_size50_29_configbus0_b[2132:2132] );
sram6T_blwl sram_blwl_2133_ (mux_2level_size50_29_sram_blwl_out[2133:2133] ,mux_2level_size50_29_sram_blwl_out[2133:2133] ,mux_2level_size50_29_sram_blwl_outb[2133:2133] ,mux_2level_size50_29_configbus0[2133:2133], mux_2level_size50_29_configbus1[2133:2133] , mux_2level_size50_29_configbus0_b[2133:2133] );
sram6T_blwl sram_blwl_2134_ (mux_2level_size50_29_sram_blwl_out[2134:2134] ,mux_2level_size50_29_sram_blwl_out[2134:2134] ,mux_2level_size50_29_sram_blwl_outb[2134:2134] ,mux_2level_size50_29_configbus0[2134:2134], mux_2level_size50_29_configbus1[2134:2134] , mux_2level_size50_29_configbus0_b[2134:2134] );
sram6T_blwl sram_blwl_2135_ (mux_2level_size50_29_sram_blwl_out[2135:2135] ,mux_2level_size50_29_sram_blwl_out[2135:2135] ,mux_2level_size50_29_sram_blwl_outb[2135:2135] ,mux_2level_size50_29_configbus0[2135:2135], mux_2level_size50_29_configbus1[2135:2135] , mux_2level_size50_29_configbus0_b[2135:2135] );
sram6T_blwl sram_blwl_2136_ (mux_2level_size50_29_sram_blwl_out[2136:2136] ,mux_2level_size50_29_sram_blwl_out[2136:2136] ,mux_2level_size50_29_sram_blwl_outb[2136:2136] ,mux_2level_size50_29_configbus0[2136:2136], mux_2level_size50_29_configbus1[2136:2136] , mux_2level_size50_29_configbus0_b[2136:2136] );
sram6T_blwl sram_blwl_2137_ (mux_2level_size50_29_sram_blwl_out[2137:2137] ,mux_2level_size50_29_sram_blwl_out[2137:2137] ,mux_2level_size50_29_sram_blwl_outb[2137:2137] ,mux_2level_size50_29_configbus0[2137:2137], mux_2level_size50_29_configbus1[2137:2137] , mux_2level_size50_29_configbus0_b[2137:2137] );
sram6T_blwl sram_blwl_2138_ (mux_2level_size50_29_sram_blwl_out[2138:2138] ,mux_2level_size50_29_sram_blwl_out[2138:2138] ,mux_2level_size50_29_sram_blwl_outb[2138:2138] ,mux_2level_size50_29_configbus0[2138:2138], mux_2level_size50_29_configbus1[2138:2138] , mux_2level_size50_29_configbus0_b[2138:2138] );
sram6T_blwl sram_blwl_2139_ (mux_2level_size50_29_sram_blwl_out[2139:2139] ,mux_2level_size50_29_sram_blwl_out[2139:2139] ,mux_2level_size50_29_sram_blwl_outb[2139:2139] ,mux_2level_size50_29_configbus0[2139:2139], mux_2level_size50_29_configbus1[2139:2139] , mux_2level_size50_29_configbus0_b[2139:2139] );
sram6T_blwl sram_blwl_2140_ (mux_2level_size50_29_sram_blwl_out[2140:2140] ,mux_2level_size50_29_sram_blwl_out[2140:2140] ,mux_2level_size50_29_sram_blwl_outb[2140:2140] ,mux_2level_size50_29_configbus0[2140:2140], mux_2level_size50_29_configbus1[2140:2140] , mux_2level_size50_29_configbus0_b[2140:2140] );
sram6T_blwl sram_blwl_2141_ (mux_2level_size50_29_sram_blwl_out[2141:2141] ,mux_2level_size50_29_sram_blwl_out[2141:2141] ,mux_2level_size50_29_sram_blwl_outb[2141:2141] ,mux_2level_size50_29_configbus0[2141:2141], mux_2level_size50_29_configbus1[2141:2141] , mux_2level_size50_29_configbus0_b[2141:2141] );
sram6T_blwl sram_blwl_2142_ (mux_2level_size50_29_sram_blwl_out[2142:2142] ,mux_2level_size50_29_sram_blwl_out[2142:2142] ,mux_2level_size50_29_sram_blwl_outb[2142:2142] ,mux_2level_size50_29_configbus0[2142:2142], mux_2level_size50_29_configbus1[2142:2142] , mux_2level_size50_29_configbus0_b[2142:2142] );
sram6T_blwl sram_blwl_2143_ (mux_2level_size50_29_sram_blwl_out[2143:2143] ,mux_2level_size50_29_sram_blwl_out[2143:2143] ,mux_2level_size50_29_sram_blwl_outb[2143:2143] ,mux_2level_size50_29_configbus0[2143:2143], mux_2level_size50_29_configbus1[2143:2143] , mux_2level_size50_29_configbus0_b[2143:2143] );
sram6T_blwl sram_blwl_2144_ (mux_2level_size50_29_sram_blwl_out[2144:2144] ,mux_2level_size50_29_sram_blwl_out[2144:2144] ,mux_2level_size50_29_sram_blwl_outb[2144:2144] ,mux_2level_size50_29_configbus0[2144:2144], mux_2level_size50_29_configbus1[2144:2144] , mux_2level_size50_29_configbus0_b[2144:2144] );
sram6T_blwl sram_blwl_2145_ (mux_2level_size50_29_sram_blwl_out[2145:2145] ,mux_2level_size50_29_sram_blwl_out[2145:2145] ,mux_2level_size50_29_sram_blwl_outb[2145:2145] ,mux_2level_size50_29_configbus0[2145:2145], mux_2level_size50_29_configbus1[2145:2145] , mux_2level_size50_29_configbus0_b[2145:2145] );
direct_interc direct_interc_174_ (mode_clb___clk_0_, fle_4___clk_0_ );
wire [0:49] in_bus_mux_2level_size50_30_ ;
assign in_bus_mux_2level_size50_30_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_30_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_30_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_30_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_30_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_30_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_30_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_30_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_30_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_30_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_30_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_30_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_30_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_30_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_30_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_30_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_30_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_30_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_30_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_30_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_30_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_30_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_30_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_30_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_30_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_30_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_30_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_30_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_30_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_30_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_30_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_30_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_30_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_30_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_30_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_30_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_30_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_30_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_30_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_30_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_30_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_30_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_30_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_30_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_30_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_30_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_30_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_30_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_30_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_30_[49] = fle_9___out_0_ ; 
wire [2146:2161] mux_2level_size50_30_configbus0;
wire [2146:2161] mux_2level_size50_30_configbus1;
wire [2146:2161] mux_2level_size50_30_sram_blwl_out ;
wire [2146:2161] mux_2level_size50_30_sram_blwl_outb ;
assign mux_2level_size50_30_configbus0[2146:2161] = sram_blwl_bl[2146:2161] ;
assign mux_2level_size50_30_configbus1[2146:2161] = sram_blwl_wl[2146:2161] ;
wire [2146:2161] mux_2level_size50_30_configbus0_b;
assign mux_2level_size50_30_configbus0_b[2146:2161] = sram_blwl_blb[2146:2161] ;
mux_2level_size50 mux_2level_size50_30_ (in_bus_mux_2level_size50_30_, fle_5___in_0_, mux_2level_size50_30_sram_blwl_out[2146:2161] ,
mux_2level_size50_30_sram_blwl_outb[2146:2161] );
//----- SRAM bits for MUX[30], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2146_ (mux_2level_size50_30_sram_blwl_out[2146:2146] ,mux_2level_size50_30_sram_blwl_out[2146:2146] ,mux_2level_size50_30_sram_blwl_outb[2146:2146] ,mux_2level_size50_30_configbus0[2146:2146], mux_2level_size50_30_configbus1[2146:2146] , mux_2level_size50_30_configbus0_b[2146:2146] );
sram6T_blwl sram_blwl_2147_ (mux_2level_size50_30_sram_blwl_out[2147:2147] ,mux_2level_size50_30_sram_blwl_out[2147:2147] ,mux_2level_size50_30_sram_blwl_outb[2147:2147] ,mux_2level_size50_30_configbus0[2147:2147], mux_2level_size50_30_configbus1[2147:2147] , mux_2level_size50_30_configbus0_b[2147:2147] );
sram6T_blwl sram_blwl_2148_ (mux_2level_size50_30_sram_blwl_out[2148:2148] ,mux_2level_size50_30_sram_blwl_out[2148:2148] ,mux_2level_size50_30_sram_blwl_outb[2148:2148] ,mux_2level_size50_30_configbus0[2148:2148], mux_2level_size50_30_configbus1[2148:2148] , mux_2level_size50_30_configbus0_b[2148:2148] );
sram6T_blwl sram_blwl_2149_ (mux_2level_size50_30_sram_blwl_out[2149:2149] ,mux_2level_size50_30_sram_blwl_out[2149:2149] ,mux_2level_size50_30_sram_blwl_outb[2149:2149] ,mux_2level_size50_30_configbus0[2149:2149], mux_2level_size50_30_configbus1[2149:2149] , mux_2level_size50_30_configbus0_b[2149:2149] );
sram6T_blwl sram_blwl_2150_ (mux_2level_size50_30_sram_blwl_out[2150:2150] ,mux_2level_size50_30_sram_blwl_out[2150:2150] ,mux_2level_size50_30_sram_blwl_outb[2150:2150] ,mux_2level_size50_30_configbus0[2150:2150], mux_2level_size50_30_configbus1[2150:2150] , mux_2level_size50_30_configbus0_b[2150:2150] );
sram6T_blwl sram_blwl_2151_ (mux_2level_size50_30_sram_blwl_out[2151:2151] ,mux_2level_size50_30_sram_blwl_out[2151:2151] ,mux_2level_size50_30_sram_blwl_outb[2151:2151] ,mux_2level_size50_30_configbus0[2151:2151], mux_2level_size50_30_configbus1[2151:2151] , mux_2level_size50_30_configbus0_b[2151:2151] );
sram6T_blwl sram_blwl_2152_ (mux_2level_size50_30_sram_blwl_out[2152:2152] ,mux_2level_size50_30_sram_blwl_out[2152:2152] ,mux_2level_size50_30_sram_blwl_outb[2152:2152] ,mux_2level_size50_30_configbus0[2152:2152], mux_2level_size50_30_configbus1[2152:2152] , mux_2level_size50_30_configbus0_b[2152:2152] );
sram6T_blwl sram_blwl_2153_ (mux_2level_size50_30_sram_blwl_out[2153:2153] ,mux_2level_size50_30_sram_blwl_out[2153:2153] ,mux_2level_size50_30_sram_blwl_outb[2153:2153] ,mux_2level_size50_30_configbus0[2153:2153], mux_2level_size50_30_configbus1[2153:2153] , mux_2level_size50_30_configbus0_b[2153:2153] );
sram6T_blwl sram_blwl_2154_ (mux_2level_size50_30_sram_blwl_out[2154:2154] ,mux_2level_size50_30_sram_blwl_out[2154:2154] ,mux_2level_size50_30_sram_blwl_outb[2154:2154] ,mux_2level_size50_30_configbus0[2154:2154], mux_2level_size50_30_configbus1[2154:2154] , mux_2level_size50_30_configbus0_b[2154:2154] );
sram6T_blwl sram_blwl_2155_ (mux_2level_size50_30_sram_blwl_out[2155:2155] ,mux_2level_size50_30_sram_blwl_out[2155:2155] ,mux_2level_size50_30_sram_blwl_outb[2155:2155] ,mux_2level_size50_30_configbus0[2155:2155], mux_2level_size50_30_configbus1[2155:2155] , mux_2level_size50_30_configbus0_b[2155:2155] );
sram6T_blwl sram_blwl_2156_ (mux_2level_size50_30_sram_blwl_out[2156:2156] ,mux_2level_size50_30_sram_blwl_out[2156:2156] ,mux_2level_size50_30_sram_blwl_outb[2156:2156] ,mux_2level_size50_30_configbus0[2156:2156], mux_2level_size50_30_configbus1[2156:2156] , mux_2level_size50_30_configbus0_b[2156:2156] );
sram6T_blwl sram_blwl_2157_ (mux_2level_size50_30_sram_blwl_out[2157:2157] ,mux_2level_size50_30_sram_blwl_out[2157:2157] ,mux_2level_size50_30_sram_blwl_outb[2157:2157] ,mux_2level_size50_30_configbus0[2157:2157], mux_2level_size50_30_configbus1[2157:2157] , mux_2level_size50_30_configbus0_b[2157:2157] );
sram6T_blwl sram_blwl_2158_ (mux_2level_size50_30_sram_blwl_out[2158:2158] ,mux_2level_size50_30_sram_blwl_out[2158:2158] ,mux_2level_size50_30_sram_blwl_outb[2158:2158] ,mux_2level_size50_30_configbus0[2158:2158], mux_2level_size50_30_configbus1[2158:2158] , mux_2level_size50_30_configbus0_b[2158:2158] );
sram6T_blwl sram_blwl_2159_ (mux_2level_size50_30_sram_blwl_out[2159:2159] ,mux_2level_size50_30_sram_blwl_out[2159:2159] ,mux_2level_size50_30_sram_blwl_outb[2159:2159] ,mux_2level_size50_30_configbus0[2159:2159], mux_2level_size50_30_configbus1[2159:2159] , mux_2level_size50_30_configbus0_b[2159:2159] );
sram6T_blwl sram_blwl_2160_ (mux_2level_size50_30_sram_blwl_out[2160:2160] ,mux_2level_size50_30_sram_blwl_out[2160:2160] ,mux_2level_size50_30_sram_blwl_outb[2160:2160] ,mux_2level_size50_30_configbus0[2160:2160], mux_2level_size50_30_configbus1[2160:2160] , mux_2level_size50_30_configbus0_b[2160:2160] );
sram6T_blwl sram_blwl_2161_ (mux_2level_size50_30_sram_blwl_out[2161:2161] ,mux_2level_size50_30_sram_blwl_out[2161:2161] ,mux_2level_size50_30_sram_blwl_outb[2161:2161] ,mux_2level_size50_30_configbus0[2161:2161], mux_2level_size50_30_configbus1[2161:2161] , mux_2level_size50_30_configbus0_b[2161:2161] );
wire [0:49] in_bus_mux_2level_size50_31_ ;
assign in_bus_mux_2level_size50_31_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_31_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_31_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_31_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_31_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_31_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_31_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_31_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_31_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_31_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_31_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_31_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_31_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_31_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_31_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_31_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_31_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_31_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_31_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_31_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_31_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_31_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_31_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_31_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_31_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_31_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_31_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_31_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_31_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_31_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_31_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_31_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_31_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_31_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_31_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_31_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_31_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_31_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_31_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_31_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_31_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_31_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_31_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_31_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_31_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_31_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_31_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_31_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_31_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_31_[49] = fle_9___out_0_ ; 
wire [2162:2177] mux_2level_size50_31_configbus0;
wire [2162:2177] mux_2level_size50_31_configbus1;
wire [2162:2177] mux_2level_size50_31_sram_blwl_out ;
wire [2162:2177] mux_2level_size50_31_sram_blwl_outb ;
assign mux_2level_size50_31_configbus0[2162:2177] = sram_blwl_bl[2162:2177] ;
assign mux_2level_size50_31_configbus1[2162:2177] = sram_blwl_wl[2162:2177] ;
wire [2162:2177] mux_2level_size50_31_configbus0_b;
assign mux_2level_size50_31_configbus0_b[2162:2177] = sram_blwl_blb[2162:2177] ;
mux_2level_size50 mux_2level_size50_31_ (in_bus_mux_2level_size50_31_, fle_5___in_1_, mux_2level_size50_31_sram_blwl_out[2162:2177] ,
mux_2level_size50_31_sram_blwl_outb[2162:2177] );
//----- SRAM bits for MUX[31], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2162_ (mux_2level_size50_31_sram_blwl_out[2162:2162] ,mux_2level_size50_31_sram_blwl_out[2162:2162] ,mux_2level_size50_31_sram_blwl_outb[2162:2162] ,mux_2level_size50_31_configbus0[2162:2162], mux_2level_size50_31_configbus1[2162:2162] , mux_2level_size50_31_configbus0_b[2162:2162] );
sram6T_blwl sram_blwl_2163_ (mux_2level_size50_31_sram_blwl_out[2163:2163] ,mux_2level_size50_31_sram_blwl_out[2163:2163] ,mux_2level_size50_31_sram_blwl_outb[2163:2163] ,mux_2level_size50_31_configbus0[2163:2163], mux_2level_size50_31_configbus1[2163:2163] , mux_2level_size50_31_configbus0_b[2163:2163] );
sram6T_blwl sram_blwl_2164_ (mux_2level_size50_31_sram_blwl_out[2164:2164] ,mux_2level_size50_31_sram_blwl_out[2164:2164] ,mux_2level_size50_31_sram_blwl_outb[2164:2164] ,mux_2level_size50_31_configbus0[2164:2164], mux_2level_size50_31_configbus1[2164:2164] , mux_2level_size50_31_configbus0_b[2164:2164] );
sram6T_blwl sram_blwl_2165_ (mux_2level_size50_31_sram_blwl_out[2165:2165] ,mux_2level_size50_31_sram_blwl_out[2165:2165] ,mux_2level_size50_31_sram_blwl_outb[2165:2165] ,mux_2level_size50_31_configbus0[2165:2165], mux_2level_size50_31_configbus1[2165:2165] , mux_2level_size50_31_configbus0_b[2165:2165] );
sram6T_blwl sram_blwl_2166_ (mux_2level_size50_31_sram_blwl_out[2166:2166] ,mux_2level_size50_31_sram_blwl_out[2166:2166] ,mux_2level_size50_31_sram_blwl_outb[2166:2166] ,mux_2level_size50_31_configbus0[2166:2166], mux_2level_size50_31_configbus1[2166:2166] , mux_2level_size50_31_configbus0_b[2166:2166] );
sram6T_blwl sram_blwl_2167_ (mux_2level_size50_31_sram_blwl_out[2167:2167] ,mux_2level_size50_31_sram_blwl_out[2167:2167] ,mux_2level_size50_31_sram_blwl_outb[2167:2167] ,mux_2level_size50_31_configbus0[2167:2167], mux_2level_size50_31_configbus1[2167:2167] , mux_2level_size50_31_configbus0_b[2167:2167] );
sram6T_blwl sram_blwl_2168_ (mux_2level_size50_31_sram_blwl_out[2168:2168] ,mux_2level_size50_31_sram_blwl_out[2168:2168] ,mux_2level_size50_31_sram_blwl_outb[2168:2168] ,mux_2level_size50_31_configbus0[2168:2168], mux_2level_size50_31_configbus1[2168:2168] , mux_2level_size50_31_configbus0_b[2168:2168] );
sram6T_blwl sram_blwl_2169_ (mux_2level_size50_31_sram_blwl_out[2169:2169] ,mux_2level_size50_31_sram_blwl_out[2169:2169] ,mux_2level_size50_31_sram_blwl_outb[2169:2169] ,mux_2level_size50_31_configbus0[2169:2169], mux_2level_size50_31_configbus1[2169:2169] , mux_2level_size50_31_configbus0_b[2169:2169] );
sram6T_blwl sram_blwl_2170_ (mux_2level_size50_31_sram_blwl_out[2170:2170] ,mux_2level_size50_31_sram_blwl_out[2170:2170] ,mux_2level_size50_31_sram_blwl_outb[2170:2170] ,mux_2level_size50_31_configbus0[2170:2170], mux_2level_size50_31_configbus1[2170:2170] , mux_2level_size50_31_configbus0_b[2170:2170] );
sram6T_blwl sram_blwl_2171_ (mux_2level_size50_31_sram_blwl_out[2171:2171] ,mux_2level_size50_31_sram_blwl_out[2171:2171] ,mux_2level_size50_31_sram_blwl_outb[2171:2171] ,mux_2level_size50_31_configbus0[2171:2171], mux_2level_size50_31_configbus1[2171:2171] , mux_2level_size50_31_configbus0_b[2171:2171] );
sram6T_blwl sram_blwl_2172_ (mux_2level_size50_31_sram_blwl_out[2172:2172] ,mux_2level_size50_31_sram_blwl_out[2172:2172] ,mux_2level_size50_31_sram_blwl_outb[2172:2172] ,mux_2level_size50_31_configbus0[2172:2172], mux_2level_size50_31_configbus1[2172:2172] , mux_2level_size50_31_configbus0_b[2172:2172] );
sram6T_blwl sram_blwl_2173_ (mux_2level_size50_31_sram_blwl_out[2173:2173] ,mux_2level_size50_31_sram_blwl_out[2173:2173] ,mux_2level_size50_31_sram_blwl_outb[2173:2173] ,mux_2level_size50_31_configbus0[2173:2173], mux_2level_size50_31_configbus1[2173:2173] , mux_2level_size50_31_configbus0_b[2173:2173] );
sram6T_blwl sram_blwl_2174_ (mux_2level_size50_31_sram_blwl_out[2174:2174] ,mux_2level_size50_31_sram_blwl_out[2174:2174] ,mux_2level_size50_31_sram_blwl_outb[2174:2174] ,mux_2level_size50_31_configbus0[2174:2174], mux_2level_size50_31_configbus1[2174:2174] , mux_2level_size50_31_configbus0_b[2174:2174] );
sram6T_blwl sram_blwl_2175_ (mux_2level_size50_31_sram_blwl_out[2175:2175] ,mux_2level_size50_31_sram_blwl_out[2175:2175] ,mux_2level_size50_31_sram_blwl_outb[2175:2175] ,mux_2level_size50_31_configbus0[2175:2175], mux_2level_size50_31_configbus1[2175:2175] , mux_2level_size50_31_configbus0_b[2175:2175] );
sram6T_blwl sram_blwl_2176_ (mux_2level_size50_31_sram_blwl_out[2176:2176] ,mux_2level_size50_31_sram_blwl_out[2176:2176] ,mux_2level_size50_31_sram_blwl_outb[2176:2176] ,mux_2level_size50_31_configbus0[2176:2176], mux_2level_size50_31_configbus1[2176:2176] , mux_2level_size50_31_configbus0_b[2176:2176] );
sram6T_blwl sram_blwl_2177_ (mux_2level_size50_31_sram_blwl_out[2177:2177] ,mux_2level_size50_31_sram_blwl_out[2177:2177] ,mux_2level_size50_31_sram_blwl_outb[2177:2177] ,mux_2level_size50_31_configbus0[2177:2177], mux_2level_size50_31_configbus1[2177:2177] , mux_2level_size50_31_configbus0_b[2177:2177] );
wire [0:49] in_bus_mux_2level_size50_32_ ;
assign in_bus_mux_2level_size50_32_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_32_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_32_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_32_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_32_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_32_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_32_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_32_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_32_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_32_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_32_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_32_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_32_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_32_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_32_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_32_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_32_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_32_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_32_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_32_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_32_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_32_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_32_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_32_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_32_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_32_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_32_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_32_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_32_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_32_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_32_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_32_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_32_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_32_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_32_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_32_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_32_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_32_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_32_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_32_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_32_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_32_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_32_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_32_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_32_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_32_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_32_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_32_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_32_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_32_[49] = fle_9___out_0_ ; 
wire [2178:2193] mux_2level_size50_32_configbus0;
wire [2178:2193] mux_2level_size50_32_configbus1;
wire [2178:2193] mux_2level_size50_32_sram_blwl_out ;
wire [2178:2193] mux_2level_size50_32_sram_blwl_outb ;
assign mux_2level_size50_32_configbus0[2178:2193] = sram_blwl_bl[2178:2193] ;
assign mux_2level_size50_32_configbus1[2178:2193] = sram_blwl_wl[2178:2193] ;
wire [2178:2193] mux_2level_size50_32_configbus0_b;
assign mux_2level_size50_32_configbus0_b[2178:2193] = sram_blwl_blb[2178:2193] ;
mux_2level_size50 mux_2level_size50_32_ (in_bus_mux_2level_size50_32_, fle_5___in_2_, mux_2level_size50_32_sram_blwl_out[2178:2193] ,
mux_2level_size50_32_sram_blwl_outb[2178:2193] );
//----- SRAM bits for MUX[32], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2178_ (mux_2level_size50_32_sram_blwl_out[2178:2178] ,mux_2level_size50_32_sram_blwl_out[2178:2178] ,mux_2level_size50_32_sram_blwl_outb[2178:2178] ,mux_2level_size50_32_configbus0[2178:2178], mux_2level_size50_32_configbus1[2178:2178] , mux_2level_size50_32_configbus0_b[2178:2178] );
sram6T_blwl sram_blwl_2179_ (mux_2level_size50_32_sram_blwl_out[2179:2179] ,mux_2level_size50_32_sram_blwl_out[2179:2179] ,mux_2level_size50_32_sram_blwl_outb[2179:2179] ,mux_2level_size50_32_configbus0[2179:2179], mux_2level_size50_32_configbus1[2179:2179] , mux_2level_size50_32_configbus0_b[2179:2179] );
sram6T_blwl sram_blwl_2180_ (mux_2level_size50_32_sram_blwl_out[2180:2180] ,mux_2level_size50_32_sram_blwl_out[2180:2180] ,mux_2level_size50_32_sram_blwl_outb[2180:2180] ,mux_2level_size50_32_configbus0[2180:2180], mux_2level_size50_32_configbus1[2180:2180] , mux_2level_size50_32_configbus0_b[2180:2180] );
sram6T_blwl sram_blwl_2181_ (mux_2level_size50_32_sram_blwl_out[2181:2181] ,mux_2level_size50_32_sram_blwl_out[2181:2181] ,mux_2level_size50_32_sram_blwl_outb[2181:2181] ,mux_2level_size50_32_configbus0[2181:2181], mux_2level_size50_32_configbus1[2181:2181] , mux_2level_size50_32_configbus0_b[2181:2181] );
sram6T_blwl sram_blwl_2182_ (mux_2level_size50_32_sram_blwl_out[2182:2182] ,mux_2level_size50_32_sram_blwl_out[2182:2182] ,mux_2level_size50_32_sram_blwl_outb[2182:2182] ,mux_2level_size50_32_configbus0[2182:2182], mux_2level_size50_32_configbus1[2182:2182] , mux_2level_size50_32_configbus0_b[2182:2182] );
sram6T_blwl sram_blwl_2183_ (mux_2level_size50_32_sram_blwl_out[2183:2183] ,mux_2level_size50_32_sram_blwl_out[2183:2183] ,mux_2level_size50_32_sram_blwl_outb[2183:2183] ,mux_2level_size50_32_configbus0[2183:2183], mux_2level_size50_32_configbus1[2183:2183] , mux_2level_size50_32_configbus0_b[2183:2183] );
sram6T_blwl sram_blwl_2184_ (mux_2level_size50_32_sram_blwl_out[2184:2184] ,mux_2level_size50_32_sram_blwl_out[2184:2184] ,mux_2level_size50_32_sram_blwl_outb[2184:2184] ,mux_2level_size50_32_configbus0[2184:2184], mux_2level_size50_32_configbus1[2184:2184] , mux_2level_size50_32_configbus0_b[2184:2184] );
sram6T_blwl sram_blwl_2185_ (mux_2level_size50_32_sram_blwl_out[2185:2185] ,mux_2level_size50_32_sram_blwl_out[2185:2185] ,mux_2level_size50_32_sram_blwl_outb[2185:2185] ,mux_2level_size50_32_configbus0[2185:2185], mux_2level_size50_32_configbus1[2185:2185] , mux_2level_size50_32_configbus0_b[2185:2185] );
sram6T_blwl sram_blwl_2186_ (mux_2level_size50_32_sram_blwl_out[2186:2186] ,mux_2level_size50_32_sram_blwl_out[2186:2186] ,mux_2level_size50_32_sram_blwl_outb[2186:2186] ,mux_2level_size50_32_configbus0[2186:2186], mux_2level_size50_32_configbus1[2186:2186] , mux_2level_size50_32_configbus0_b[2186:2186] );
sram6T_blwl sram_blwl_2187_ (mux_2level_size50_32_sram_blwl_out[2187:2187] ,mux_2level_size50_32_sram_blwl_out[2187:2187] ,mux_2level_size50_32_sram_blwl_outb[2187:2187] ,mux_2level_size50_32_configbus0[2187:2187], mux_2level_size50_32_configbus1[2187:2187] , mux_2level_size50_32_configbus0_b[2187:2187] );
sram6T_blwl sram_blwl_2188_ (mux_2level_size50_32_sram_blwl_out[2188:2188] ,mux_2level_size50_32_sram_blwl_out[2188:2188] ,mux_2level_size50_32_sram_blwl_outb[2188:2188] ,mux_2level_size50_32_configbus0[2188:2188], mux_2level_size50_32_configbus1[2188:2188] , mux_2level_size50_32_configbus0_b[2188:2188] );
sram6T_blwl sram_blwl_2189_ (mux_2level_size50_32_sram_blwl_out[2189:2189] ,mux_2level_size50_32_sram_blwl_out[2189:2189] ,mux_2level_size50_32_sram_blwl_outb[2189:2189] ,mux_2level_size50_32_configbus0[2189:2189], mux_2level_size50_32_configbus1[2189:2189] , mux_2level_size50_32_configbus0_b[2189:2189] );
sram6T_blwl sram_blwl_2190_ (mux_2level_size50_32_sram_blwl_out[2190:2190] ,mux_2level_size50_32_sram_blwl_out[2190:2190] ,mux_2level_size50_32_sram_blwl_outb[2190:2190] ,mux_2level_size50_32_configbus0[2190:2190], mux_2level_size50_32_configbus1[2190:2190] , mux_2level_size50_32_configbus0_b[2190:2190] );
sram6T_blwl sram_blwl_2191_ (mux_2level_size50_32_sram_blwl_out[2191:2191] ,mux_2level_size50_32_sram_blwl_out[2191:2191] ,mux_2level_size50_32_sram_blwl_outb[2191:2191] ,mux_2level_size50_32_configbus0[2191:2191], mux_2level_size50_32_configbus1[2191:2191] , mux_2level_size50_32_configbus0_b[2191:2191] );
sram6T_blwl sram_blwl_2192_ (mux_2level_size50_32_sram_blwl_out[2192:2192] ,mux_2level_size50_32_sram_blwl_out[2192:2192] ,mux_2level_size50_32_sram_blwl_outb[2192:2192] ,mux_2level_size50_32_configbus0[2192:2192], mux_2level_size50_32_configbus1[2192:2192] , mux_2level_size50_32_configbus0_b[2192:2192] );
sram6T_blwl sram_blwl_2193_ (mux_2level_size50_32_sram_blwl_out[2193:2193] ,mux_2level_size50_32_sram_blwl_out[2193:2193] ,mux_2level_size50_32_sram_blwl_outb[2193:2193] ,mux_2level_size50_32_configbus0[2193:2193], mux_2level_size50_32_configbus1[2193:2193] , mux_2level_size50_32_configbus0_b[2193:2193] );
wire [0:49] in_bus_mux_2level_size50_33_ ;
assign in_bus_mux_2level_size50_33_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_33_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_33_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_33_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_33_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_33_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_33_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_33_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_33_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_33_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_33_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_33_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_33_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_33_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_33_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_33_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_33_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_33_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_33_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_33_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_33_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_33_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_33_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_33_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_33_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_33_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_33_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_33_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_33_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_33_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_33_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_33_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_33_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_33_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_33_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_33_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_33_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_33_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_33_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_33_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_33_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_33_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_33_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_33_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_33_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_33_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_33_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_33_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_33_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_33_[49] = fle_9___out_0_ ; 
wire [2194:2209] mux_2level_size50_33_configbus0;
wire [2194:2209] mux_2level_size50_33_configbus1;
wire [2194:2209] mux_2level_size50_33_sram_blwl_out ;
wire [2194:2209] mux_2level_size50_33_sram_blwl_outb ;
assign mux_2level_size50_33_configbus0[2194:2209] = sram_blwl_bl[2194:2209] ;
assign mux_2level_size50_33_configbus1[2194:2209] = sram_blwl_wl[2194:2209] ;
wire [2194:2209] mux_2level_size50_33_configbus0_b;
assign mux_2level_size50_33_configbus0_b[2194:2209] = sram_blwl_blb[2194:2209] ;
mux_2level_size50 mux_2level_size50_33_ (in_bus_mux_2level_size50_33_, fle_5___in_3_, mux_2level_size50_33_sram_blwl_out[2194:2209] ,
mux_2level_size50_33_sram_blwl_outb[2194:2209] );
//----- SRAM bits for MUX[33], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2194_ (mux_2level_size50_33_sram_blwl_out[2194:2194] ,mux_2level_size50_33_sram_blwl_out[2194:2194] ,mux_2level_size50_33_sram_blwl_outb[2194:2194] ,mux_2level_size50_33_configbus0[2194:2194], mux_2level_size50_33_configbus1[2194:2194] , mux_2level_size50_33_configbus0_b[2194:2194] );
sram6T_blwl sram_blwl_2195_ (mux_2level_size50_33_sram_blwl_out[2195:2195] ,mux_2level_size50_33_sram_blwl_out[2195:2195] ,mux_2level_size50_33_sram_blwl_outb[2195:2195] ,mux_2level_size50_33_configbus0[2195:2195], mux_2level_size50_33_configbus1[2195:2195] , mux_2level_size50_33_configbus0_b[2195:2195] );
sram6T_blwl sram_blwl_2196_ (mux_2level_size50_33_sram_blwl_out[2196:2196] ,mux_2level_size50_33_sram_blwl_out[2196:2196] ,mux_2level_size50_33_sram_blwl_outb[2196:2196] ,mux_2level_size50_33_configbus0[2196:2196], mux_2level_size50_33_configbus1[2196:2196] , mux_2level_size50_33_configbus0_b[2196:2196] );
sram6T_blwl sram_blwl_2197_ (mux_2level_size50_33_sram_blwl_out[2197:2197] ,mux_2level_size50_33_sram_blwl_out[2197:2197] ,mux_2level_size50_33_sram_blwl_outb[2197:2197] ,mux_2level_size50_33_configbus0[2197:2197], mux_2level_size50_33_configbus1[2197:2197] , mux_2level_size50_33_configbus0_b[2197:2197] );
sram6T_blwl sram_blwl_2198_ (mux_2level_size50_33_sram_blwl_out[2198:2198] ,mux_2level_size50_33_sram_blwl_out[2198:2198] ,mux_2level_size50_33_sram_blwl_outb[2198:2198] ,mux_2level_size50_33_configbus0[2198:2198], mux_2level_size50_33_configbus1[2198:2198] , mux_2level_size50_33_configbus0_b[2198:2198] );
sram6T_blwl sram_blwl_2199_ (mux_2level_size50_33_sram_blwl_out[2199:2199] ,mux_2level_size50_33_sram_blwl_out[2199:2199] ,mux_2level_size50_33_sram_blwl_outb[2199:2199] ,mux_2level_size50_33_configbus0[2199:2199], mux_2level_size50_33_configbus1[2199:2199] , mux_2level_size50_33_configbus0_b[2199:2199] );
sram6T_blwl sram_blwl_2200_ (mux_2level_size50_33_sram_blwl_out[2200:2200] ,mux_2level_size50_33_sram_blwl_out[2200:2200] ,mux_2level_size50_33_sram_blwl_outb[2200:2200] ,mux_2level_size50_33_configbus0[2200:2200], mux_2level_size50_33_configbus1[2200:2200] , mux_2level_size50_33_configbus0_b[2200:2200] );
sram6T_blwl sram_blwl_2201_ (mux_2level_size50_33_sram_blwl_out[2201:2201] ,mux_2level_size50_33_sram_blwl_out[2201:2201] ,mux_2level_size50_33_sram_blwl_outb[2201:2201] ,mux_2level_size50_33_configbus0[2201:2201], mux_2level_size50_33_configbus1[2201:2201] , mux_2level_size50_33_configbus0_b[2201:2201] );
sram6T_blwl sram_blwl_2202_ (mux_2level_size50_33_sram_blwl_out[2202:2202] ,mux_2level_size50_33_sram_blwl_out[2202:2202] ,mux_2level_size50_33_sram_blwl_outb[2202:2202] ,mux_2level_size50_33_configbus0[2202:2202], mux_2level_size50_33_configbus1[2202:2202] , mux_2level_size50_33_configbus0_b[2202:2202] );
sram6T_blwl sram_blwl_2203_ (mux_2level_size50_33_sram_blwl_out[2203:2203] ,mux_2level_size50_33_sram_blwl_out[2203:2203] ,mux_2level_size50_33_sram_blwl_outb[2203:2203] ,mux_2level_size50_33_configbus0[2203:2203], mux_2level_size50_33_configbus1[2203:2203] , mux_2level_size50_33_configbus0_b[2203:2203] );
sram6T_blwl sram_blwl_2204_ (mux_2level_size50_33_sram_blwl_out[2204:2204] ,mux_2level_size50_33_sram_blwl_out[2204:2204] ,mux_2level_size50_33_sram_blwl_outb[2204:2204] ,mux_2level_size50_33_configbus0[2204:2204], mux_2level_size50_33_configbus1[2204:2204] , mux_2level_size50_33_configbus0_b[2204:2204] );
sram6T_blwl sram_blwl_2205_ (mux_2level_size50_33_sram_blwl_out[2205:2205] ,mux_2level_size50_33_sram_blwl_out[2205:2205] ,mux_2level_size50_33_sram_blwl_outb[2205:2205] ,mux_2level_size50_33_configbus0[2205:2205], mux_2level_size50_33_configbus1[2205:2205] , mux_2level_size50_33_configbus0_b[2205:2205] );
sram6T_blwl sram_blwl_2206_ (mux_2level_size50_33_sram_blwl_out[2206:2206] ,mux_2level_size50_33_sram_blwl_out[2206:2206] ,mux_2level_size50_33_sram_blwl_outb[2206:2206] ,mux_2level_size50_33_configbus0[2206:2206], mux_2level_size50_33_configbus1[2206:2206] , mux_2level_size50_33_configbus0_b[2206:2206] );
sram6T_blwl sram_blwl_2207_ (mux_2level_size50_33_sram_blwl_out[2207:2207] ,mux_2level_size50_33_sram_blwl_out[2207:2207] ,mux_2level_size50_33_sram_blwl_outb[2207:2207] ,mux_2level_size50_33_configbus0[2207:2207], mux_2level_size50_33_configbus1[2207:2207] , mux_2level_size50_33_configbus0_b[2207:2207] );
sram6T_blwl sram_blwl_2208_ (mux_2level_size50_33_sram_blwl_out[2208:2208] ,mux_2level_size50_33_sram_blwl_out[2208:2208] ,mux_2level_size50_33_sram_blwl_outb[2208:2208] ,mux_2level_size50_33_configbus0[2208:2208], mux_2level_size50_33_configbus1[2208:2208] , mux_2level_size50_33_configbus0_b[2208:2208] );
sram6T_blwl sram_blwl_2209_ (mux_2level_size50_33_sram_blwl_out[2209:2209] ,mux_2level_size50_33_sram_blwl_out[2209:2209] ,mux_2level_size50_33_sram_blwl_outb[2209:2209] ,mux_2level_size50_33_configbus0[2209:2209], mux_2level_size50_33_configbus1[2209:2209] , mux_2level_size50_33_configbus0_b[2209:2209] );
wire [0:49] in_bus_mux_2level_size50_34_ ;
assign in_bus_mux_2level_size50_34_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_34_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_34_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_34_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_34_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_34_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_34_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_34_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_34_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_34_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_34_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_34_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_34_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_34_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_34_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_34_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_34_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_34_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_34_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_34_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_34_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_34_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_34_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_34_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_34_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_34_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_34_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_34_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_34_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_34_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_34_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_34_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_34_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_34_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_34_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_34_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_34_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_34_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_34_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_34_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_34_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_34_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_34_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_34_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_34_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_34_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_34_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_34_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_34_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_34_[49] = fle_9___out_0_ ; 
wire [2210:2225] mux_2level_size50_34_configbus0;
wire [2210:2225] mux_2level_size50_34_configbus1;
wire [2210:2225] mux_2level_size50_34_sram_blwl_out ;
wire [2210:2225] mux_2level_size50_34_sram_blwl_outb ;
assign mux_2level_size50_34_configbus0[2210:2225] = sram_blwl_bl[2210:2225] ;
assign mux_2level_size50_34_configbus1[2210:2225] = sram_blwl_wl[2210:2225] ;
wire [2210:2225] mux_2level_size50_34_configbus0_b;
assign mux_2level_size50_34_configbus0_b[2210:2225] = sram_blwl_blb[2210:2225] ;
mux_2level_size50 mux_2level_size50_34_ (in_bus_mux_2level_size50_34_, fle_5___in_4_, mux_2level_size50_34_sram_blwl_out[2210:2225] ,
mux_2level_size50_34_sram_blwl_outb[2210:2225] );
//----- SRAM bits for MUX[34], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2210_ (mux_2level_size50_34_sram_blwl_out[2210:2210] ,mux_2level_size50_34_sram_blwl_out[2210:2210] ,mux_2level_size50_34_sram_blwl_outb[2210:2210] ,mux_2level_size50_34_configbus0[2210:2210], mux_2level_size50_34_configbus1[2210:2210] , mux_2level_size50_34_configbus0_b[2210:2210] );
sram6T_blwl sram_blwl_2211_ (mux_2level_size50_34_sram_blwl_out[2211:2211] ,mux_2level_size50_34_sram_blwl_out[2211:2211] ,mux_2level_size50_34_sram_blwl_outb[2211:2211] ,mux_2level_size50_34_configbus0[2211:2211], mux_2level_size50_34_configbus1[2211:2211] , mux_2level_size50_34_configbus0_b[2211:2211] );
sram6T_blwl sram_blwl_2212_ (mux_2level_size50_34_sram_blwl_out[2212:2212] ,mux_2level_size50_34_sram_blwl_out[2212:2212] ,mux_2level_size50_34_sram_blwl_outb[2212:2212] ,mux_2level_size50_34_configbus0[2212:2212], mux_2level_size50_34_configbus1[2212:2212] , mux_2level_size50_34_configbus0_b[2212:2212] );
sram6T_blwl sram_blwl_2213_ (mux_2level_size50_34_sram_blwl_out[2213:2213] ,mux_2level_size50_34_sram_blwl_out[2213:2213] ,mux_2level_size50_34_sram_blwl_outb[2213:2213] ,mux_2level_size50_34_configbus0[2213:2213], mux_2level_size50_34_configbus1[2213:2213] , mux_2level_size50_34_configbus0_b[2213:2213] );
sram6T_blwl sram_blwl_2214_ (mux_2level_size50_34_sram_blwl_out[2214:2214] ,mux_2level_size50_34_sram_blwl_out[2214:2214] ,mux_2level_size50_34_sram_blwl_outb[2214:2214] ,mux_2level_size50_34_configbus0[2214:2214], mux_2level_size50_34_configbus1[2214:2214] , mux_2level_size50_34_configbus0_b[2214:2214] );
sram6T_blwl sram_blwl_2215_ (mux_2level_size50_34_sram_blwl_out[2215:2215] ,mux_2level_size50_34_sram_blwl_out[2215:2215] ,mux_2level_size50_34_sram_blwl_outb[2215:2215] ,mux_2level_size50_34_configbus0[2215:2215], mux_2level_size50_34_configbus1[2215:2215] , mux_2level_size50_34_configbus0_b[2215:2215] );
sram6T_blwl sram_blwl_2216_ (mux_2level_size50_34_sram_blwl_out[2216:2216] ,mux_2level_size50_34_sram_blwl_out[2216:2216] ,mux_2level_size50_34_sram_blwl_outb[2216:2216] ,mux_2level_size50_34_configbus0[2216:2216], mux_2level_size50_34_configbus1[2216:2216] , mux_2level_size50_34_configbus0_b[2216:2216] );
sram6T_blwl sram_blwl_2217_ (mux_2level_size50_34_sram_blwl_out[2217:2217] ,mux_2level_size50_34_sram_blwl_out[2217:2217] ,mux_2level_size50_34_sram_blwl_outb[2217:2217] ,mux_2level_size50_34_configbus0[2217:2217], mux_2level_size50_34_configbus1[2217:2217] , mux_2level_size50_34_configbus0_b[2217:2217] );
sram6T_blwl sram_blwl_2218_ (mux_2level_size50_34_sram_blwl_out[2218:2218] ,mux_2level_size50_34_sram_blwl_out[2218:2218] ,mux_2level_size50_34_sram_blwl_outb[2218:2218] ,mux_2level_size50_34_configbus0[2218:2218], mux_2level_size50_34_configbus1[2218:2218] , mux_2level_size50_34_configbus0_b[2218:2218] );
sram6T_blwl sram_blwl_2219_ (mux_2level_size50_34_sram_blwl_out[2219:2219] ,mux_2level_size50_34_sram_blwl_out[2219:2219] ,mux_2level_size50_34_sram_blwl_outb[2219:2219] ,mux_2level_size50_34_configbus0[2219:2219], mux_2level_size50_34_configbus1[2219:2219] , mux_2level_size50_34_configbus0_b[2219:2219] );
sram6T_blwl sram_blwl_2220_ (mux_2level_size50_34_sram_blwl_out[2220:2220] ,mux_2level_size50_34_sram_blwl_out[2220:2220] ,mux_2level_size50_34_sram_blwl_outb[2220:2220] ,mux_2level_size50_34_configbus0[2220:2220], mux_2level_size50_34_configbus1[2220:2220] , mux_2level_size50_34_configbus0_b[2220:2220] );
sram6T_blwl sram_blwl_2221_ (mux_2level_size50_34_sram_blwl_out[2221:2221] ,mux_2level_size50_34_sram_blwl_out[2221:2221] ,mux_2level_size50_34_sram_blwl_outb[2221:2221] ,mux_2level_size50_34_configbus0[2221:2221], mux_2level_size50_34_configbus1[2221:2221] , mux_2level_size50_34_configbus0_b[2221:2221] );
sram6T_blwl sram_blwl_2222_ (mux_2level_size50_34_sram_blwl_out[2222:2222] ,mux_2level_size50_34_sram_blwl_out[2222:2222] ,mux_2level_size50_34_sram_blwl_outb[2222:2222] ,mux_2level_size50_34_configbus0[2222:2222], mux_2level_size50_34_configbus1[2222:2222] , mux_2level_size50_34_configbus0_b[2222:2222] );
sram6T_blwl sram_blwl_2223_ (mux_2level_size50_34_sram_blwl_out[2223:2223] ,mux_2level_size50_34_sram_blwl_out[2223:2223] ,mux_2level_size50_34_sram_blwl_outb[2223:2223] ,mux_2level_size50_34_configbus0[2223:2223], mux_2level_size50_34_configbus1[2223:2223] , mux_2level_size50_34_configbus0_b[2223:2223] );
sram6T_blwl sram_blwl_2224_ (mux_2level_size50_34_sram_blwl_out[2224:2224] ,mux_2level_size50_34_sram_blwl_out[2224:2224] ,mux_2level_size50_34_sram_blwl_outb[2224:2224] ,mux_2level_size50_34_configbus0[2224:2224], mux_2level_size50_34_configbus1[2224:2224] , mux_2level_size50_34_configbus0_b[2224:2224] );
sram6T_blwl sram_blwl_2225_ (mux_2level_size50_34_sram_blwl_out[2225:2225] ,mux_2level_size50_34_sram_blwl_out[2225:2225] ,mux_2level_size50_34_sram_blwl_outb[2225:2225] ,mux_2level_size50_34_configbus0[2225:2225], mux_2level_size50_34_configbus1[2225:2225] , mux_2level_size50_34_configbus0_b[2225:2225] );
wire [0:49] in_bus_mux_2level_size50_35_ ;
assign in_bus_mux_2level_size50_35_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_35_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_35_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_35_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_35_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_35_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_35_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_35_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_35_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_35_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_35_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_35_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_35_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_35_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_35_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_35_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_35_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_35_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_35_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_35_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_35_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_35_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_35_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_35_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_35_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_35_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_35_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_35_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_35_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_35_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_35_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_35_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_35_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_35_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_35_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_35_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_35_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_35_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_35_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_35_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_35_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_35_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_35_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_35_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_35_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_35_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_35_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_35_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_35_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_35_[49] = fle_9___out_0_ ; 
wire [2226:2241] mux_2level_size50_35_configbus0;
wire [2226:2241] mux_2level_size50_35_configbus1;
wire [2226:2241] mux_2level_size50_35_sram_blwl_out ;
wire [2226:2241] mux_2level_size50_35_sram_blwl_outb ;
assign mux_2level_size50_35_configbus0[2226:2241] = sram_blwl_bl[2226:2241] ;
assign mux_2level_size50_35_configbus1[2226:2241] = sram_blwl_wl[2226:2241] ;
wire [2226:2241] mux_2level_size50_35_configbus0_b;
assign mux_2level_size50_35_configbus0_b[2226:2241] = sram_blwl_blb[2226:2241] ;
mux_2level_size50 mux_2level_size50_35_ (in_bus_mux_2level_size50_35_, fle_5___in_5_, mux_2level_size50_35_sram_blwl_out[2226:2241] ,
mux_2level_size50_35_sram_blwl_outb[2226:2241] );
//----- SRAM bits for MUX[35], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2226_ (mux_2level_size50_35_sram_blwl_out[2226:2226] ,mux_2level_size50_35_sram_blwl_out[2226:2226] ,mux_2level_size50_35_sram_blwl_outb[2226:2226] ,mux_2level_size50_35_configbus0[2226:2226], mux_2level_size50_35_configbus1[2226:2226] , mux_2level_size50_35_configbus0_b[2226:2226] );
sram6T_blwl sram_blwl_2227_ (mux_2level_size50_35_sram_blwl_out[2227:2227] ,mux_2level_size50_35_sram_blwl_out[2227:2227] ,mux_2level_size50_35_sram_blwl_outb[2227:2227] ,mux_2level_size50_35_configbus0[2227:2227], mux_2level_size50_35_configbus1[2227:2227] , mux_2level_size50_35_configbus0_b[2227:2227] );
sram6T_blwl sram_blwl_2228_ (mux_2level_size50_35_sram_blwl_out[2228:2228] ,mux_2level_size50_35_sram_blwl_out[2228:2228] ,mux_2level_size50_35_sram_blwl_outb[2228:2228] ,mux_2level_size50_35_configbus0[2228:2228], mux_2level_size50_35_configbus1[2228:2228] , mux_2level_size50_35_configbus0_b[2228:2228] );
sram6T_blwl sram_blwl_2229_ (mux_2level_size50_35_sram_blwl_out[2229:2229] ,mux_2level_size50_35_sram_blwl_out[2229:2229] ,mux_2level_size50_35_sram_blwl_outb[2229:2229] ,mux_2level_size50_35_configbus0[2229:2229], mux_2level_size50_35_configbus1[2229:2229] , mux_2level_size50_35_configbus0_b[2229:2229] );
sram6T_blwl sram_blwl_2230_ (mux_2level_size50_35_sram_blwl_out[2230:2230] ,mux_2level_size50_35_sram_blwl_out[2230:2230] ,mux_2level_size50_35_sram_blwl_outb[2230:2230] ,mux_2level_size50_35_configbus0[2230:2230], mux_2level_size50_35_configbus1[2230:2230] , mux_2level_size50_35_configbus0_b[2230:2230] );
sram6T_blwl sram_blwl_2231_ (mux_2level_size50_35_sram_blwl_out[2231:2231] ,mux_2level_size50_35_sram_blwl_out[2231:2231] ,mux_2level_size50_35_sram_blwl_outb[2231:2231] ,mux_2level_size50_35_configbus0[2231:2231], mux_2level_size50_35_configbus1[2231:2231] , mux_2level_size50_35_configbus0_b[2231:2231] );
sram6T_blwl sram_blwl_2232_ (mux_2level_size50_35_sram_blwl_out[2232:2232] ,mux_2level_size50_35_sram_blwl_out[2232:2232] ,mux_2level_size50_35_sram_blwl_outb[2232:2232] ,mux_2level_size50_35_configbus0[2232:2232], mux_2level_size50_35_configbus1[2232:2232] , mux_2level_size50_35_configbus0_b[2232:2232] );
sram6T_blwl sram_blwl_2233_ (mux_2level_size50_35_sram_blwl_out[2233:2233] ,mux_2level_size50_35_sram_blwl_out[2233:2233] ,mux_2level_size50_35_sram_blwl_outb[2233:2233] ,mux_2level_size50_35_configbus0[2233:2233], mux_2level_size50_35_configbus1[2233:2233] , mux_2level_size50_35_configbus0_b[2233:2233] );
sram6T_blwl sram_blwl_2234_ (mux_2level_size50_35_sram_blwl_out[2234:2234] ,mux_2level_size50_35_sram_blwl_out[2234:2234] ,mux_2level_size50_35_sram_blwl_outb[2234:2234] ,mux_2level_size50_35_configbus0[2234:2234], mux_2level_size50_35_configbus1[2234:2234] , mux_2level_size50_35_configbus0_b[2234:2234] );
sram6T_blwl sram_blwl_2235_ (mux_2level_size50_35_sram_blwl_out[2235:2235] ,mux_2level_size50_35_sram_blwl_out[2235:2235] ,mux_2level_size50_35_sram_blwl_outb[2235:2235] ,mux_2level_size50_35_configbus0[2235:2235], mux_2level_size50_35_configbus1[2235:2235] , mux_2level_size50_35_configbus0_b[2235:2235] );
sram6T_blwl sram_blwl_2236_ (mux_2level_size50_35_sram_blwl_out[2236:2236] ,mux_2level_size50_35_sram_blwl_out[2236:2236] ,mux_2level_size50_35_sram_blwl_outb[2236:2236] ,mux_2level_size50_35_configbus0[2236:2236], mux_2level_size50_35_configbus1[2236:2236] , mux_2level_size50_35_configbus0_b[2236:2236] );
sram6T_blwl sram_blwl_2237_ (mux_2level_size50_35_sram_blwl_out[2237:2237] ,mux_2level_size50_35_sram_blwl_out[2237:2237] ,mux_2level_size50_35_sram_blwl_outb[2237:2237] ,mux_2level_size50_35_configbus0[2237:2237], mux_2level_size50_35_configbus1[2237:2237] , mux_2level_size50_35_configbus0_b[2237:2237] );
sram6T_blwl sram_blwl_2238_ (mux_2level_size50_35_sram_blwl_out[2238:2238] ,mux_2level_size50_35_sram_blwl_out[2238:2238] ,mux_2level_size50_35_sram_blwl_outb[2238:2238] ,mux_2level_size50_35_configbus0[2238:2238], mux_2level_size50_35_configbus1[2238:2238] , mux_2level_size50_35_configbus0_b[2238:2238] );
sram6T_blwl sram_blwl_2239_ (mux_2level_size50_35_sram_blwl_out[2239:2239] ,mux_2level_size50_35_sram_blwl_out[2239:2239] ,mux_2level_size50_35_sram_blwl_outb[2239:2239] ,mux_2level_size50_35_configbus0[2239:2239], mux_2level_size50_35_configbus1[2239:2239] , mux_2level_size50_35_configbus0_b[2239:2239] );
sram6T_blwl sram_blwl_2240_ (mux_2level_size50_35_sram_blwl_out[2240:2240] ,mux_2level_size50_35_sram_blwl_out[2240:2240] ,mux_2level_size50_35_sram_blwl_outb[2240:2240] ,mux_2level_size50_35_configbus0[2240:2240], mux_2level_size50_35_configbus1[2240:2240] , mux_2level_size50_35_configbus0_b[2240:2240] );
sram6T_blwl sram_blwl_2241_ (mux_2level_size50_35_sram_blwl_out[2241:2241] ,mux_2level_size50_35_sram_blwl_out[2241:2241] ,mux_2level_size50_35_sram_blwl_outb[2241:2241] ,mux_2level_size50_35_configbus0[2241:2241], mux_2level_size50_35_configbus1[2241:2241] , mux_2level_size50_35_configbus0_b[2241:2241] );
direct_interc direct_interc_175_ (mode_clb___clk_0_, fle_5___clk_0_ );
wire [0:49] in_bus_mux_2level_size50_36_ ;
assign in_bus_mux_2level_size50_36_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_36_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_36_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_36_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_36_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_36_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_36_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_36_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_36_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_36_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_36_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_36_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_36_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_36_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_36_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_36_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_36_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_36_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_36_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_36_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_36_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_36_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_36_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_36_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_36_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_36_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_36_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_36_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_36_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_36_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_36_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_36_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_36_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_36_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_36_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_36_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_36_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_36_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_36_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_36_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_36_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_36_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_36_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_36_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_36_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_36_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_36_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_36_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_36_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_36_[49] = fle_9___out_0_ ; 
wire [2242:2257] mux_2level_size50_36_configbus0;
wire [2242:2257] mux_2level_size50_36_configbus1;
wire [2242:2257] mux_2level_size50_36_sram_blwl_out ;
wire [2242:2257] mux_2level_size50_36_sram_blwl_outb ;
assign mux_2level_size50_36_configbus0[2242:2257] = sram_blwl_bl[2242:2257] ;
assign mux_2level_size50_36_configbus1[2242:2257] = sram_blwl_wl[2242:2257] ;
wire [2242:2257] mux_2level_size50_36_configbus0_b;
assign mux_2level_size50_36_configbus0_b[2242:2257] = sram_blwl_blb[2242:2257] ;
mux_2level_size50 mux_2level_size50_36_ (in_bus_mux_2level_size50_36_, fle_6___in_0_, mux_2level_size50_36_sram_blwl_out[2242:2257] ,
mux_2level_size50_36_sram_blwl_outb[2242:2257] );
//----- SRAM bits for MUX[36], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2242_ (mux_2level_size50_36_sram_blwl_out[2242:2242] ,mux_2level_size50_36_sram_blwl_out[2242:2242] ,mux_2level_size50_36_sram_blwl_outb[2242:2242] ,mux_2level_size50_36_configbus0[2242:2242], mux_2level_size50_36_configbus1[2242:2242] , mux_2level_size50_36_configbus0_b[2242:2242] );
sram6T_blwl sram_blwl_2243_ (mux_2level_size50_36_sram_blwl_out[2243:2243] ,mux_2level_size50_36_sram_blwl_out[2243:2243] ,mux_2level_size50_36_sram_blwl_outb[2243:2243] ,mux_2level_size50_36_configbus0[2243:2243], mux_2level_size50_36_configbus1[2243:2243] , mux_2level_size50_36_configbus0_b[2243:2243] );
sram6T_blwl sram_blwl_2244_ (mux_2level_size50_36_sram_blwl_out[2244:2244] ,mux_2level_size50_36_sram_blwl_out[2244:2244] ,mux_2level_size50_36_sram_blwl_outb[2244:2244] ,mux_2level_size50_36_configbus0[2244:2244], mux_2level_size50_36_configbus1[2244:2244] , mux_2level_size50_36_configbus0_b[2244:2244] );
sram6T_blwl sram_blwl_2245_ (mux_2level_size50_36_sram_blwl_out[2245:2245] ,mux_2level_size50_36_sram_blwl_out[2245:2245] ,mux_2level_size50_36_sram_blwl_outb[2245:2245] ,mux_2level_size50_36_configbus0[2245:2245], mux_2level_size50_36_configbus1[2245:2245] , mux_2level_size50_36_configbus0_b[2245:2245] );
sram6T_blwl sram_blwl_2246_ (mux_2level_size50_36_sram_blwl_out[2246:2246] ,mux_2level_size50_36_sram_blwl_out[2246:2246] ,mux_2level_size50_36_sram_blwl_outb[2246:2246] ,mux_2level_size50_36_configbus0[2246:2246], mux_2level_size50_36_configbus1[2246:2246] , mux_2level_size50_36_configbus0_b[2246:2246] );
sram6T_blwl sram_blwl_2247_ (mux_2level_size50_36_sram_blwl_out[2247:2247] ,mux_2level_size50_36_sram_blwl_out[2247:2247] ,mux_2level_size50_36_sram_blwl_outb[2247:2247] ,mux_2level_size50_36_configbus0[2247:2247], mux_2level_size50_36_configbus1[2247:2247] , mux_2level_size50_36_configbus0_b[2247:2247] );
sram6T_blwl sram_blwl_2248_ (mux_2level_size50_36_sram_blwl_out[2248:2248] ,mux_2level_size50_36_sram_blwl_out[2248:2248] ,mux_2level_size50_36_sram_blwl_outb[2248:2248] ,mux_2level_size50_36_configbus0[2248:2248], mux_2level_size50_36_configbus1[2248:2248] , mux_2level_size50_36_configbus0_b[2248:2248] );
sram6T_blwl sram_blwl_2249_ (mux_2level_size50_36_sram_blwl_out[2249:2249] ,mux_2level_size50_36_sram_blwl_out[2249:2249] ,mux_2level_size50_36_sram_blwl_outb[2249:2249] ,mux_2level_size50_36_configbus0[2249:2249], mux_2level_size50_36_configbus1[2249:2249] , mux_2level_size50_36_configbus0_b[2249:2249] );
sram6T_blwl sram_blwl_2250_ (mux_2level_size50_36_sram_blwl_out[2250:2250] ,mux_2level_size50_36_sram_blwl_out[2250:2250] ,mux_2level_size50_36_sram_blwl_outb[2250:2250] ,mux_2level_size50_36_configbus0[2250:2250], mux_2level_size50_36_configbus1[2250:2250] , mux_2level_size50_36_configbus0_b[2250:2250] );
sram6T_blwl sram_blwl_2251_ (mux_2level_size50_36_sram_blwl_out[2251:2251] ,mux_2level_size50_36_sram_blwl_out[2251:2251] ,mux_2level_size50_36_sram_blwl_outb[2251:2251] ,mux_2level_size50_36_configbus0[2251:2251], mux_2level_size50_36_configbus1[2251:2251] , mux_2level_size50_36_configbus0_b[2251:2251] );
sram6T_blwl sram_blwl_2252_ (mux_2level_size50_36_sram_blwl_out[2252:2252] ,mux_2level_size50_36_sram_blwl_out[2252:2252] ,mux_2level_size50_36_sram_blwl_outb[2252:2252] ,mux_2level_size50_36_configbus0[2252:2252], mux_2level_size50_36_configbus1[2252:2252] , mux_2level_size50_36_configbus0_b[2252:2252] );
sram6T_blwl sram_blwl_2253_ (mux_2level_size50_36_sram_blwl_out[2253:2253] ,mux_2level_size50_36_sram_blwl_out[2253:2253] ,mux_2level_size50_36_sram_blwl_outb[2253:2253] ,mux_2level_size50_36_configbus0[2253:2253], mux_2level_size50_36_configbus1[2253:2253] , mux_2level_size50_36_configbus0_b[2253:2253] );
sram6T_blwl sram_blwl_2254_ (mux_2level_size50_36_sram_blwl_out[2254:2254] ,mux_2level_size50_36_sram_blwl_out[2254:2254] ,mux_2level_size50_36_sram_blwl_outb[2254:2254] ,mux_2level_size50_36_configbus0[2254:2254], mux_2level_size50_36_configbus1[2254:2254] , mux_2level_size50_36_configbus0_b[2254:2254] );
sram6T_blwl sram_blwl_2255_ (mux_2level_size50_36_sram_blwl_out[2255:2255] ,mux_2level_size50_36_sram_blwl_out[2255:2255] ,mux_2level_size50_36_sram_blwl_outb[2255:2255] ,mux_2level_size50_36_configbus0[2255:2255], mux_2level_size50_36_configbus1[2255:2255] , mux_2level_size50_36_configbus0_b[2255:2255] );
sram6T_blwl sram_blwl_2256_ (mux_2level_size50_36_sram_blwl_out[2256:2256] ,mux_2level_size50_36_sram_blwl_out[2256:2256] ,mux_2level_size50_36_sram_blwl_outb[2256:2256] ,mux_2level_size50_36_configbus0[2256:2256], mux_2level_size50_36_configbus1[2256:2256] , mux_2level_size50_36_configbus0_b[2256:2256] );
sram6T_blwl sram_blwl_2257_ (mux_2level_size50_36_sram_blwl_out[2257:2257] ,mux_2level_size50_36_sram_blwl_out[2257:2257] ,mux_2level_size50_36_sram_blwl_outb[2257:2257] ,mux_2level_size50_36_configbus0[2257:2257], mux_2level_size50_36_configbus1[2257:2257] , mux_2level_size50_36_configbus0_b[2257:2257] );
wire [0:49] in_bus_mux_2level_size50_37_ ;
assign in_bus_mux_2level_size50_37_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_37_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_37_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_37_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_37_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_37_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_37_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_37_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_37_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_37_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_37_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_37_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_37_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_37_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_37_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_37_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_37_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_37_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_37_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_37_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_37_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_37_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_37_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_37_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_37_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_37_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_37_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_37_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_37_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_37_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_37_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_37_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_37_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_37_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_37_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_37_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_37_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_37_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_37_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_37_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_37_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_37_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_37_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_37_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_37_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_37_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_37_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_37_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_37_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_37_[49] = fle_9___out_0_ ; 
wire [2258:2273] mux_2level_size50_37_configbus0;
wire [2258:2273] mux_2level_size50_37_configbus1;
wire [2258:2273] mux_2level_size50_37_sram_blwl_out ;
wire [2258:2273] mux_2level_size50_37_sram_blwl_outb ;
assign mux_2level_size50_37_configbus0[2258:2273] = sram_blwl_bl[2258:2273] ;
assign mux_2level_size50_37_configbus1[2258:2273] = sram_blwl_wl[2258:2273] ;
wire [2258:2273] mux_2level_size50_37_configbus0_b;
assign mux_2level_size50_37_configbus0_b[2258:2273] = sram_blwl_blb[2258:2273] ;
mux_2level_size50 mux_2level_size50_37_ (in_bus_mux_2level_size50_37_, fle_6___in_1_, mux_2level_size50_37_sram_blwl_out[2258:2273] ,
mux_2level_size50_37_sram_blwl_outb[2258:2273] );
//----- SRAM bits for MUX[37], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2258_ (mux_2level_size50_37_sram_blwl_out[2258:2258] ,mux_2level_size50_37_sram_blwl_out[2258:2258] ,mux_2level_size50_37_sram_blwl_outb[2258:2258] ,mux_2level_size50_37_configbus0[2258:2258], mux_2level_size50_37_configbus1[2258:2258] , mux_2level_size50_37_configbus0_b[2258:2258] );
sram6T_blwl sram_blwl_2259_ (mux_2level_size50_37_sram_blwl_out[2259:2259] ,mux_2level_size50_37_sram_blwl_out[2259:2259] ,mux_2level_size50_37_sram_blwl_outb[2259:2259] ,mux_2level_size50_37_configbus0[2259:2259], mux_2level_size50_37_configbus1[2259:2259] , mux_2level_size50_37_configbus0_b[2259:2259] );
sram6T_blwl sram_blwl_2260_ (mux_2level_size50_37_sram_blwl_out[2260:2260] ,mux_2level_size50_37_sram_blwl_out[2260:2260] ,mux_2level_size50_37_sram_blwl_outb[2260:2260] ,mux_2level_size50_37_configbus0[2260:2260], mux_2level_size50_37_configbus1[2260:2260] , mux_2level_size50_37_configbus0_b[2260:2260] );
sram6T_blwl sram_blwl_2261_ (mux_2level_size50_37_sram_blwl_out[2261:2261] ,mux_2level_size50_37_sram_blwl_out[2261:2261] ,mux_2level_size50_37_sram_blwl_outb[2261:2261] ,mux_2level_size50_37_configbus0[2261:2261], mux_2level_size50_37_configbus1[2261:2261] , mux_2level_size50_37_configbus0_b[2261:2261] );
sram6T_blwl sram_blwl_2262_ (mux_2level_size50_37_sram_blwl_out[2262:2262] ,mux_2level_size50_37_sram_blwl_out[2262:2262] ,mux_2level_size50_37_sram_blwl_outb[2262:2262] ,mux_2level_size50_37_configbus0[2262:2262], mux_2level_size50_37_configbus1[2262:2262] , mux_2level_size50_37_configbus0_b[2262:2262] );
sram6T_blwl sram_blwl_2263_ (mux_2level_size50_37_sram_blwl_out[2263:2263] ,mux_2level_size50_37_sram_blwl_out[2263:2263] ,mux_2level_size50_37_sram_blwl_outb[2263:2263] ,mux_2level_size50_37_configbus0[2263:2263], mux_2level_size50_37_configbus1[2263:2263] , mux_2level_size50_37_configbus0_b[2263:2263] );
sram6T_blwl sram_blwl_2264_ (mux_2level_size50_37_sram_blwl_out[2264:2264] ,mux_2level_size50_37_sram_blwl_out[2264:2264] ,mux_2level_size50_37_sram_blwl_outb[2264:2264] ,mux_2level_size50_37_configbus0[2264:2264], mux_2level_size50_37_configbus1[2264:2264] , mux_2level_size50_37_configbus0_b[2264:2264] );
sram6T_blwl sram_blwl_2265_ (mux_2level_size50_37_sram_blwl_out[2265:2265] ,mux_2level_size50_37_sram_blwl_out[2265:2265] ,mux_2level_size50_37_sram_blwl_outb[2265:2265] ,mux_2level_size50_37_configbus0[2265:2265], mux_2level_size50_37_configbus1[2265:2265] , mux_2level_size50_37_configbus0_b[2265:2265] );
sram6T_blwl sram_blwl_2266_ (mux_2level_size50_37_sram_blwl_out[2266:2266] ,mux_2level_size50_37_sram_blwl_out[2266:2266] ,mux_2level_size50_37_sram_blwl_outb[2266:2266] ,mux_2level_size50_37_configbus0[2266:2266], mux_2level_size50_37_configbus1[2266:2266] , mux_2level_size50_37_configbus0_b[2266:2266] );
sram6T_blwl sram_blwl_2267_ (mux_2level_size50_37_sram_blwl_out[2267:2267] ,mux_2level_size50_37_sram_blwl_out[2267:2267] ,mux_2level_size50_37_sram_blwl_outb[2267:2267] ,mux_2level_size50_37_configbus0[2267:2267], mux_2level_size50_37_configbus1[2267:2267] , mux_2level_size50_37_configbus0_b[2267:2267] );
sram6T_blwl sram_blwl_2268_ (mux_2level_size50_37_sram_blwl_out[2268:2268] ,mux_2level_size50_37_sram_blwl_out[2268:2268] ,mux_2level_size50_37_sram_blwl_outb[2268:2268] ,mux_2level_size50_37_configbus0[2268:2268], mux_2level_size50_37_configbus1[2268:2268] , mux_2level_size50_37_configbus0_b[2268:2268] );
sram6T_blwl sram_blwl_2269_ (mux_2level_size50_37_sram_blwl_out[2269:2269] ,mux_2level_size50_37_sram_blwl_out[2269:2269] ,mux_2level_size50_37_sram_blwl_outb[2269:2269] ,mux_2level_size50_37_configbus0[2269:2269], mux_2level_size50_37_configbus1[2269:2269] , mux_2level_size50_37_configbus0_b[2269:2269] );
sram6T_blwl sram_blwl_2270_ (mux_2level_size50_37_sram_blwl_out[2270:2270] ,mux_2level_size50_37_sram_blwl_out[2270:2270] ,mux_2level_size50_37_sram_blwl_outb[2270:2270] ,mux_2level_size50_37_configbus0[2270:2270], mux_2level_size50_37_configbus1[2270:2270] , mux_2level_size50_37_configbus0_b[2270:2270] );
sram6T_blwl sram_blwl_2271_ (mux_2level_size50_37_sram_blwl_out[2271:2271] ,mux_2level_size50_37_sram_blwl_out[2271:2271] ,mux_2level_size50_37_sram_blwl_outb[2271:2271] ,mux_2level_size50_37_configbus0[2271:2271], mux_2level_size50_37_configbus1[2271:2271] , mux_2level_size50_37_configbus0_b[2271:2271] );
sram6T_blwl sram_blwl_2272_ (mux_2level_size50_37_sram_blwl_out[2272:2272] ,mux_2level_size50_37_sram_blwl_out[2272:2272] ,mux_2level_size50_37_sram_blwl_outb[2272:2272] ,mux_2level_size50_37_configbus0[2272:2272], mux_2level_size50_37_configbus1[2272:2272] , mux_2level_size50_37_configbus0_b[2272:2272] );
sram6T_blwl sram_blwl_2273_ (mux_2level_size50_37_sram_blwl_out[2273:2273] ,mux_2level_size50_37_sram_blwl_out[2273:2273] ,mux_2level_size50_37_sram_blwl_outb[2273:2273] ,mux_2level_size50_37_configbus0[2273:2273], mux_2level_size50_37_configbus1[2273:2273] , mux_2level_size50_37_configbus0_b[2273:2273] );
wire [0:49] in_bus_mux_2level_size50_38_ ;
assign in_bus_mux_2level_size50_38_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_38_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_38_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_38_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_38_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_38_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_38_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_38_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_38_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_38_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_38_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_38_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_38_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_38_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_38_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_38_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_38_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_38_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_38_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_38_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_38_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_38_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_38_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_38_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_38_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_38_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_38_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_38_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_38_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_38_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_38_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_38_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_38_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_38_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_38_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_38_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_38_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_38_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_38_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_38_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_38_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_38_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_38_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_38_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_38_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_38_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_38_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_38_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_38_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_38_[49] = fle_9___out_0_ ; 
wire [2274:2289] mux_2level_size50_38_configbus0;
wire [2274:2289] mux_2level_size50_38_configbus1;
wire [2274:2289] mux_2level_size50_38_sram_blwl_out ;
wire [2274:2289] mux_2level_size50_38_sram_blwl_outb ;
assign mux_2level_size50_38_configbus0[2274:2289] = sram_blwl_bl[2274:2289] ;
assign mux_2level_size50_38_configbus1[2274:2289] = sram_blwl_wl[2274:2289] ;
wire [2274:2289] mux_2level_size50_38_configbus0_b;
assign mux_2level_size50_38_configbus0_b[2274:2289] = sram_blwl_blb[2274:2289] ;
mux_2level_size50 mux_2level_size50_38_ (in_bus_mux_2level_size50_38_, fle_6___in_2_, mux_2level_size50_38_sram_blwl_out[2274:2289] ,
mux_2level_size50_38_sram_blwl_outb[2274:2289] );
//----- SRAM bits for MUX[38], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2274_ (mux_2level_size50_38_sram_blwl_out[2274:2274] ,mux_2level_size50_38_sram_blwl_out[2274:2274] ,mux_2level_size50_38_sram_blwl_outb[2274:2274] ,mux_2level_size50_38_configbus0[2274:2274], mux_2level_size50_38_configbus1[2274:2274] , mux_2level_size50_38_configbus0_b[2274:2274] );
sram6T_blwl sram_blwl_2275_ (mux_2level_size50_38_sram_blwl_out[2275:2275] ,mux_2level_size50_38_sram_blwl_out[2275:2275] ,mux_2level_size50_38_sram_blwl_outb[2275:2275] ,mux_2level_size50_38_configbus0[2275:2275], mux_2level_size50_38_configbus1[2275:2275] , mux_2level_size50_38_configbus0_b[2275:2275] );
sram6T_blwl sram_blwl_2276_ (mux_2level_size50_38_sram_blwl_out[2276:2276] ,mux_2level_size50_38_sram_blwl_out[2276:2276] ,mux_2level_size50_38_sram_blwl_outb[2276:2276] ,mux_2level_size50_38_configbus0[2276:2276], mux_2level_size50_38_configbus1[2276:2276] , mux_2level_size50_38_configbus0_b[2276:2276] );
sram6T_blwl sram_blwl_2277_ (mux_2level_size50_38_sram_blwl_out[2277:2277] ,mux_2level_size50_38_sram_blwl_out[2277:2277] ,mux_2level_size50_38_sram_blwl_outb[2277:2277] ,mux_2level_size50_38_configbus0[2277:2277], mux_2level_size50_38_configbus1[2277:2277] , mux_2level_size50_38_configbus0_b[2277:2277] );
sram6T_blwl sram_blwl_2278_ (mux_2level_size50_38_sram_blwl_out[2278:2278] ,mux_2level_size50_38_sram_blwl_out[2278:2278] ,mux_2level_size50_38_sram_blwl_outb[2278:2278] ,mux_2level_size50_38_configbus0[2278:2278], mux_2level_size50_38_configbus1[2278:2278] , mux_2level_size50_38_configbus0_b[2278:2278] );
sram6T_blwl sram_blwl_2279_ (mux_2level_size50_38_sram_blwl_out[2279:2279] ,mux_2level_size50_38_sram_blwl_out[2279:2279] ,mux_2level_size50_38_sram_blwl_outb[2279:2279] ,mux_2level_size50_38_configbus0[2279:2279], mux_2level_size50_38_configbus1[2279:2279] , mux_2level_size50_38_configbus0_b[2279:2279] );
sram6T_blwl sram_blwl_2280_ (mux_2level_size50_38_sram_blwl_out[2280:2280] ,mux_2level_size50_38_sram_blwl_out[2280:2280] ,mux_2level_size50_38_sram_blwl_outb[2280:2280] ,mux_2level_size50_38_configbus0[2280:2280], mux_2level_size50_38_configbus1[2280:2280] , mux_2level_size50_38_configbus0_b[2280:2280] );
sram6T_blwl sram_blwl_2281_ (mux_2level_size50_38_sram_blwl_out[2281:2281] ,mux_2level_size50_38_sram_blwl_out[2281:2281] ,mux_2level_size50_38_sram_blwl_outb[2281:2281] ,mux_2level_size50_38_configbus0[2281:2281], mux_2level_size50_38_configbus1[2281:2281] , mux_2level_size50_38_configbus0_b[2281:2281] );
sram6T_blwl sram_blwl_2282_ (mux_2level_size50_38_sram_blwl_out[2282:2282] ,mux_2level_size50_38_sram_blwl_out[2282:2282] ,mux_2level_size50_38_sram_blwl_outb[2282:2282] ,mux_2level_size50_38_configbus0[2282:2282], mux_2level_size50_38_configbus1[2282:2282] , mux_2level_size50_38_configbus0_b[2282:2282] );
sram6T_blwl sram_blwl_2283_ (mux_2level_size50_38_sram_blwl_out[2283:2283] ,mux_2level_size50_38_sram_blwl_out[2283:2283] ,mux_2level_size50_38_sram_blwl_outb[2283:2283] ,mux_2level_size50_38_configbus0[2283:2283], mux_2level_size50_38_configbus1[2283:2283] , mux_2level_size50_38_configbus0_b[2283:2283] );
sram6T_blwl sram_blwl_2284_ (mux_2level_size50_38_sram_blwl_out[2284:2284] ,mux_2level_size50_38_sram_blwl_out[2284:2284] ,mux_2level_size50_38_sram_blwl_outb[2284:2284] ,mux_2level_size50_38_configbus0[2284:2284], mux_2level_size50_38_configbus1[2284:2284] , mux_2level_size50_38_configbus0_b[2284:2284] );
sram6T_blwl sram_blwl_2285_ (mux_2level_size50_38_sram_blwl_out[2285:2285] ,mux_2level_size50_38_sram_blwl_out[2285:2285] ,mux_2level_size50_38_sram_blwl_outb[2285:2285] ,mux_2level_size50_38_configbus0[2285:2285], mux_2level_size50_38_configbus1[2285:2285] , mux_2level_size50_38_configbus0_b[2285:2285] );
sram6T_blwl sram_blwl_2286_ (mux_2level_size50_38_sram_blwl_out[2286:2286] ,mux_2level_size50_38_sram_blwl_out[2286:2286] ,mux_2level_size50_38_sram_blwl_outb[2286:2286] ,mux_2level_size50_38_configbus0[2286:2286], mux_2level_size50_38_configbus1[2286:2286] , mux_2level_size50_38_configbus0_b[2286:2286] );
sram6T_blwl sram_blwl_2287_ (mux_2level_size50_38_sram_blwl_out[2287:2287] ,mux_2level_size50_38_sram_blwl_out[2287:2287] ,mux_2level_size50_38_sram_blwl_outb[2287:2287] ,mux_2level_size50_38_configbus0[2287:2287], mux_2level_size50_38_configbus1[2287:2287] , mux_2level_size50_38_configbus0_b[2287:2287] );
sram6T_blwl sram_blwl_2288_ (mux_2level_size50_38_sram_blwl_out[2288:2288] ,mux_2level_size50_38_sram_blwl_out[2288:2288] ,mux_2level_size50_38_sram_blwl_outb[2288:2288] ,mux_2level_size50_38_configbus0[2288:2288], mux_2level_size50_38_configbus1[2288:2288] , mux_2level_size50_38_configbus0_b[2288:2288] );
sram6T_blwl sram_blwl_2289_ (mux_2level_size50_38_sram_blwl_out[2289:2289] ,mux_2level_size50_38_sram_blwl_out[2289:2289] ,mux_2level_size50_38_sram_blwl_outb[2289:2289] ,mux_2level_size50_38_configbus0[2289:2289], mux_2level_size50_38_configbus1[2289:2289] , mux_2level_size50_38_configbus0_b[2289:2289] );
wire [0:49] in_bus_mux_2level_size50_39_ ;
assign in_bus_mux_2level_size50_39_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_39_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_39_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_39_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_39_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_39_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_39_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_39_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_39_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_39_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_39_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_39_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_39_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_39_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_39_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_39_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_39_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_39_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_39_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_39_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_39_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_39_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_39_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_39_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_39_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_39_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_39_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_39_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_39_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_39_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_39_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_39_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_39_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_39_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_39_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_39_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_39_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_39_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_39_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_39_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_39_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_39_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_39_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_39_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_39_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_39_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_39_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_39_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_39_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_39_[49] = fle_9___out_0_ ; 
wire [2290:2305] mux_2level_size50_39_configbus0;
wire [2290:2305] mux_2level_size50_39_configbus1;
wire [2290:2305] mux_2level_size50_39_sram_blwl_out ;
wire [2290:2305] mux_2level_size50_39_sram_blwl_outb ;
assign mux_2level_size50_39_configbus0[2290:2305] = sram_blwl_bl[2290:2305] ;
assign mux_2level_size50_39_configbus1[2290:2305] = sram_blwl_wl[2290:2305] ;
wire [2290:2305] mux_2level_size50_39_configbus0_b;
assign mux_2level_size50_39_configbus0_b[2290:2305] = sram_blwl_blb[2290:2305] ;
mux_2level_size50 mux_2level_size50_39_ (in_bus_mux_2level_size50_39_, fle_6___in_3_, mux_2level_size50_39_sram_blwl_out[2290:2305] ,
mux_2level_size50_39_sram_blwl_outb[2290:2305] );
//----- SRAM bits for MUX[39], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2290_ (mux_2level_size50_39_sram_blwl_out[2290:2290] ,mux_2level_size50_39_sram_blwl_out[2290:2290] ,mux_2level_size50_39_sram_blwl_outb[2290:2290] ,mux_2level_size50_39_configbus0[2290:2290], mux_2level_size50_39_configbus1[2290:2290] , mux_2level_size50_39_configbus0_b[2290:2290] );
sram6T_blwl sram_blwl_2291_ (mux_2level_size50_39_sram_blwl_out[2291:2291] ,mux_2level_size50_39_sram_blwl_out[2291:2291] ,mux_2level_size50_39_sram_blwl_outb[2291:2291] ,mux_2level_size50_39_configbus0[2291:2291], mux_2level_size50_39_configbus1[2291:2291] , mux_2level_size50_39_configbus0_b[2291:2291] );
sram6T_blwl sram_blwl_2292_ (mux_2level_size50_39_sram_blwl_out[2292:2292] ,mux_2level_size50_39_sram_blwl_out[2292:2292] ,mux_2level_size50_39_sram_blwl_outb[2292:2292] ,mux_2level_size50_39_configbus0[2292:2292], mux_2level_size50_39_configbus1[2292:2292] , mux_2level_size50_39_configbus0_b[2292:2292] );
sram6T_blwl sram_blwl_2293_ (mux_2level_size50_39_sram_blwl_out[2293:2293] ,mux_2level_size50_39_sram_blwl_out[2293:2293] ,mux_2level_size50_39_sram_blwl_outb[2293:2293] ,mux_2level_size50_39_configbus0[2293:2293], mux_2level_size50_39_configbus1[2293:2293] , mux_2level_size50_39_configbus0_b[2293:2293] );
sram6T_blwl sram_blwl_2294_ (mux_2level_size50_39_sram_blwl_out[2294:2294] ,mux_2level_size50_39_sram_blwl_out[2294:2294] ,mux_2level_size50_39_sram_blwl_outb[2294:2294] ,mux_2level_size50_39_configbus0[2294:2294], mux_2level_size50_39_configbus1[2294:2294] , mux_2level_size50_39_configbus0_b[2294:2294] );
sram6T_blwl sram_blwl_2295_ (mux_2level_size50_39_sram_blwl_out[2295:2295] ,mux_2level_size50_39_sram_blwl_out[2295:2295] ,mux_2level_size50_39_sram_blwl_outb[2295:2295] ,mux_2level_size50_39_configbus0[2295:2295], mux_2level_size50_39_configbus1[2295:2295] , mux_2level_size50_39_configbus0_b[2295:2295] );
sram6T_blwl sram_blwl_2296_ (mux_2level_size50_39_sram_blwl_out[2296:2296] ,mux_2level_size50_39_sram_blwl_out[2296:2296] ,mux_2level_size50_39_sram_blwl_outb[2296:2296] ,mux_2level_size50_39_configbus0[2296:2296], mux_2level_size50_39_configbus1[2296:2296] , mux_2level_size50_39_configbus0_b[2296:2296] );
sram6T_blwl sram_blwl_2297_ (mux_2level_size50_39_sram_blwl_out[2297:2297] ,mux_2level_size50_39_sram_blwl_out[2297:2297] ,mux_2level_size50_39_sram_blwl_outb[2297:2297] ,mux_2level_size50_39_configbus0[2297:2297], mux_2level_size50_39_configbus1[2297:2297] , mux_2level_size50_39_configbus0_b[2297:2297] );
sram6T_blwl sram_blwl_2298_ (mux_2level_size50_39_sram_blwl_out[2298:2298] ,mux_2level_size50_39_sram_blwl_out[2298:2298] ,mux_2level_size50_39_sram_blwl_outb[2298:2298] ,mux_2level_size50_39_configbus0[2298:2298], mux_2level_size50_39_configbus1[2298:2298] , mux_2level_size50_39_configbus0_b[2298:2298] );
sram6T_blwl sram_blwl_2299_ (mux_2level_size50_39_sram_blwl_out[2299:2299] ,mux_2level_size50_39_sram_blwl_out[2299:2299] ,mux_2level_size50_39_sram_blwl_outb[2299:2299] ,mux_2level_size50_39_configbus0[2299:2299], mux_2level_size50_39_configbus1[2299:2299] , mux_2level_size50_39_configbus0_b[2299:2299] );
sram6T_blwl sram_blwl_2300_ (mux_2level_size50_39_sram_blwl_out[2300:2300] ,mux_2level_size50_39_sram_blwl_out[2300:2300] ,mux_2level_size50_39_sram_blwl_outb[2300:2300] ,mux_2level_size50_39_configbus0[2300:2300], mux_2level_size50_39_configbus1[2300:2300] , mux_2level_size50_39_configbus0_b[2300:2300] );
sram6T_blwl sram_blwl_2301_ (mux_2level_size50_39_sram_blwl_out[2301:2301] ,mux_2level_size50_39_sram_blwl_out[2301:2301] ,mux_2level_size50_39_sram_blwl_outb[2301:2301] ,mux_2level_size50_39_configbus0[2301:2301], mux_2level_size50_39_configbus1[2301:2301] , mux_2level_size50_39_configbus0_b[2301:2301] );
sram6T_blwl sram_blwl_2302_ (mux_2level_size50_39_sram_blwl_out[2302:2302] ,mux_2level_size50_39_sram_blwl_out[2302:2302] ,mux_2level_size50_39_sram_blwl_outb[2302:2302] ,mux_2level_size50_39_configbus0[2302:2302], mux_2level_size50_39_configbus1[2302:2302] , mux_2level_size50_39_configbus0_b[2302:2302] );
sram6T_blwl sram_blwl_2303_ (mux_2level_size50_39_sram_blwl_out[2303:2303] ,mux_2level_size50_39_sram_blwl_out[2303:2303] ,mux_2level_size50_39_sram_blwl_outb[2303:2303] ,mux_2level_size50_39_configbus0[2303:2303], mux_2level_size50_39_configbus1[2303:2303] , mux_2level_size50_39_configbus0_b[2303:2303] );
sram6T_blwl sram_blwl_2304_ (mux_2level_size50_39_sram_blwl_out[2304:2304] ,mux_2level_size50_39_sram_blwl_out[2304:2304] ,mux_2level_size50_39_sram_blwl_outb[2304:2304] ,mux_2level_size50_39_configbus0[2304:2304], mux_2level_size50_39_configbus1[2304:2304] , mux_2level_size50_39_configbus0_b[2304:2304] );
sram6T_blwl sram_blwl_2305_ (mux_2level_size50_39_sram_blwl_out[2305:2305] ,mux_2level_size50_39_sram_blwl_out[2305:2305] ,mux_2level_size50_39_sram_blwl_outb[2305:2305] ,mux_2level_size50_39_configbus0[2305:2305], mux_2level_size50_39_configbus1[2305:2305] , mux_2level_size50_39_configbus0_b[2305:2305] );
wire [0:49] in_bus_mux_2level_size50_40_ ;
assign in_bus_mux_2level_size50_40_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_40_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_40_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_40_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_40_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_40_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_40_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_40_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_40_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_40_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_40_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_40_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_40_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_40_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_40_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_40_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_40_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_40_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_40_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_40_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_40_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_40_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_40_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_40_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_40_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_40_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_40_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_40_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_40_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_40_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_40_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_40_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_40_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_40_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_40_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_40_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_40_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_40_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_40_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_40_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_40_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_40_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_40_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_40_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_40_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_40_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_40_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_40_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_40_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_40_[49] = fle_9___out_0_ ; 
wire [2306:2321] mux_2level_size50_40_configbus0;
wire [2306:2321] mux_2level_size50_40_configbus1;
wire [2306:2321] mux_2level_size50_40_sram_blwl_out ;
wire [2306:2321] mux_2level_size50_40_sram_blwl_outb ;
assign mux_2level_size50_40_configbus0[2306:2321] = sram_blwl_bl[2306:2321] ;
assign mux_2level_size50_40_configbus1[2306:2321] = sram_blwl_wl[2306:2321] ;
wire [2306:2321] mux_2level_size50_40_configbus0_b;
assign mux_2level_size50_40_configbus0_b[2306:2321] = sram_blwl_blb[2306:2321] ;
mux_2level_size50 mux_2level_size50_40_ (in_bus_mux_2level_size50_40_, fle_6___in_4_, mux_2level_size50_40_sram_blwl_out[2306:2321] ,
mux_2level_size50_40_sram_blwl_outb[2306:2321] );
//----- SRAM bits for MUX[40], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2306_ (mux_2level_size50_40_sram_blwl_out[2306:2306] ,mux_2level_size50_40_sram_blwl_out[2306:2306] ,mux_2level_size50_40_sram_blwl_outb[2306:2306] ,mux_2level_size50_40_configbus0[2306:2306], mux_2level_size50_40_configbus1[2306:2306] , mux_2level_size50_40_configbus0_b[2306:2306] );
sram6T_blwl sram_blwl_2307_ (mux_2level_size50_40_sram_blwl_out[2307:2307] ,mux_2level_size50_40_sram_blwl_out[2307:2307] ,mux_2level_size50_40_sram_blwl_outb[2307:2307] ,mux_2level_size50_40_configbus0[2307:2307], mux_2level_size50_40_configbus1[2307:2307] , mux_2level_size50_40_configbus0_b[2307:2307] );
sram6T_blwl sram_blwl_2308_ (mux_2level_size50_40_sram_blwl_out[2308:2308] ,mux_2level_size50_40_sram_blwl_out[2308:2308] ,mux_2level_size50_40_sram_blwl_outb[2308:2308] ,mux_2level_size50_40_configbus0[2308:2308], mux_2level_size50_40_configbus1[2308:2308] , mux_2level_size50_40_configbus0_b[2308:2308] );
sram6T_blwl sram_blwl_2309_ (mux_2level_size50_40_sram_blwl_out[2309:2309] ,mux_2level_size50_40_sram_blwl_out[2309:2309] ,mux_2level_size50_40_sram_blwl_outb[2309:2309] ,mux_2level_size50_40_configbus0[2309:2309], mux_2level_size50_40_configbus1[2309:2309] , mux_2level_size50_40_configbus0_b[2309:2309] );
sram6T_blwl sram_blwl_2310_ (mux_2level_size50_40_sram_blwl_out[2310:2310] ,mux_2level_size50_40_sram_blwl_out[2310:2310] ,mux_2level_size50_40_sram_blwl_outb[2310:2310] ,mux_2level_size50_40_configbus0[2310:2310], mux_2level_size50_40_configbus1[2310:2310] , mux_2level_size50_40_configbus0_b[2310:2310] );
sram6T_blwl sram_blwl_2311_ (mux_2level_size50_40_sram_blwl_out[2311:2311] ,mux_2level_size50_40_sram_blwl_out[2311:2311] ,mux_2level_size50_40_sram_blwl_outb[2311:2311] ,mux_2level_size50_40_configbus0[2311:2311], mux_2level_size50_40_configbus1[2311:2311] , mux_2level_size50_40_configbus0_b[2311:2311] );
sram6T_blwl sram_blwl_2312_ (mux_2level_size50_40_sram_blwl_out[2312:2312] ,mux_2level_size50_40_sram_blwl_out[2312:2312] ,mux_2level_size50_40_sram_blwl_outb[2312:2312] ,mux_2level_size50_40_configbus0[2312:2312], mux_2level_size50_40_configbus1[2312:2312] , mux_2level_size50_40_configbus0_b[2312:2312] );
sram6T_blwl sram_blwl_2313_ (mux_2level_size50_40_sram_blwl_out[2313:2313] ,mux_2level_size50_40_sram_blwl_out[2313:2313] ,mux_2level_size50_40_sram_blwl_outb[2313:2313] ,mux_2level_size50_40_configbus0[2313:2313], mux_2level_size50_40_configbus1[2313:2313] , mux_2level_size50_40_configbus0_b[2313:2313] );
sram6T_blwl sram_blwl_2314_ (mux_2level_size50_40_sram_blwl_out[2314:2314] ,mux_2level_size50_40_sram_blwl_out[2314:2314] ,mux_2level_size50_40_sram_blwl_outb[2314:2314] ,mux_2level_size50_40_configbus0[2314:2314], mux_2level_size50_40_configbus1[2314:2314] , mux_2level_size50_40_configbus0_b[2314:2314] );
sram6T_blwl sram_blwl_2315_ (mux_2level_size50_40_sram_blwl_out[2315:2315] ,mux_2level_size50_40_sram_blwl_out[2315:2315] ,mux_2level_size50_40_sram_blwl_outb[2315:2315] ,mux_2level_size50_40_configbus0[2315:2315], mux_2level_size50_40_configbus1[2315:2315] , mux_2level_size50_40_configbus0_b[2315:2315] );
sram6T_blwl sram_blwl_2316_ (mux_2level_size50_40_sram_blwl_out[2316:2316] ,mux_2level_size50_40_sram_blwl_out[2316:2316] ,mux_2level_size50_40_sram_blwl_outb[2316:2316] ,mux_2level_size50_40_configbus0[2316:2316], mux_2level_size50_40_configbus1[2316:2316] , mux_2level_size50_40_configbus0_b[2316:2316] );
sram6T_blwl sram_blwl_2317_ (mux_2level_size50_40_sram_blwl_out[2317:2317] ,mux_2level_size50_40_sram_blwl_out[2317:2317] ,mux_2level_size50_40_sram_blwl_outb[2317:2317] ,mux_2level_size50_40_configbus0[2317:2317], mux_2level_size50_40_configbus1[2317:2317] , mux_2level_size50_40_configbus0_b[2317:2317] );
sram6T_blwl sram_blwl_2318_ (mux_2level_size50_40_sram_blwl_out[2318:2318] ,mux_2level_size50_40_sram_blwl_out[2318:2318] ,mux_2level_size50_40_sram_blwl_outb[2318:2318] ,mux_2level_size50_40_configbus0[2318:2318], mux_2level_size50_40_configbus1[2318:2318] , mux_2level_size50_40_configbus0_b[2318:2318] );
sram6T_blwl sram_blwl_2319_ (mux_2level_size50_40_sram_blwl_out[2319:2319] ,mux_2level_size50_40_sram_blwl_out[2319:2319] ,mux_2level_size50_40_sram_blwl_outb[2319:2319] ,mux_2level_size50_40_configbus0[2319:2319], mux_2level_size50_40_configbus1[2319:2319] , mux_2level_size50_40_configbus0_b[2319:2319] );
sram6T_blwl sram_blwl_2320_ (mux_2level_size50_40_sram_blwl_out[2320:2320] ,mux_2level_size50_40_sram_blwl_out[2320:2320] ,mux_2level_size50_40_sram_blwl_outb[2320:2320] ,mux_2level_size50_40_configbus0[2320:2320], mux_2level_size50_40_configbus1[2320:2320] , mux_2level_size50_40_configbus0_b[2320:2320] );
sram6T_blwl sram_blwl_2321_ (mux_2level_size50_40_sram_blwl_out[2321:2321] ,mux_2level_size50_40_sram_blwl_out[2321:2321] ,mux_2level_size50_40_sram_blwl_outb[2321:2321] ,mux_2level_size50_40_configbus0[2321:2321], mux_2level_size50_40_configbus1[2321:2321] , mux_2level_size50_40_configbus0_b[2321:2321] );
wire [0:49] in_bus_mux_2level_size50_41_ ;
assign in_bus_mux_2level_size50_41_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_41_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_41_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_41_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_41_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_41_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_41_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_41_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_41_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_41_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_41_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_41_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_41_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_41_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_41_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_41_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_41_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_41_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_41_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_41_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_41_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_41_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_41_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_41_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_41_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_41_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_41_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_41_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_41_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_41_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_41_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_41_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_41_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_41_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_41_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_41_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_41_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_41_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_41_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_41_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_41_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_41_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_41_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_41_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_41_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_41_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_41_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_41_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_41_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_41_[49] = fle_9___out_0_ ; 
wire [2322:2337] mux_2level_size50_41_configbus0;
wire [2322:2337] mux_2level_size50_41_configbus1;
wire [2322:2337] mux_2level_size50_41_sram_blwl_out ;
wire [2322:2337] mux_2level_size50_41_sram_blwl_outb ;
assign mux_2level_size50_41_configbus0[2322:2337] = sram_blwl_bl[2322:2337] ;
assign mux_2level_size50_41_configbus1[2322:2337] = sram_blwl_wl[2322:2337] ;
wire [2322:2337] mux_2level_size50_41_configbus0_b;
assign mux_2level_size50_41_configbus0_b[2322:2337] = sram_blwl_blb[2322:2337] ;
mux_2level_size50 mux_2level_size50_41_ (in_bus_mux_2level_size50_41_, fle_6___in_5_, mux_2level_size50_41_sram_blwl_out[2322:2337] ,
mux_2level_size50_41_sram_blwl_outb[2322:2337] );
//----- SRAM bits for MUX[41], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2322_ (mux_2level_size50_41_sram_blwl_out[2322:2322] ,mux_2level_size50_41_sram_blwl_out[2322:2322] ,mux_2level_size50_41_sram_blwl_outb[2322:2322] ,mux_2level_size50_41_configbus0[2322:2322], mux_2level_size50_41_configbus1[2322:2322] , mux_2level_size50_41_configbus0_b[2322:2322] );
sram6T_blwl sram_blwl_2323_ (mux_2level_size50_41_sram_blwl_out[2323:2323] ,mux_2level_size50_41_sram_blwl_out[2323:2323] ,mux_2level_size50_41_sram_blwl_outb[2323:2323] ,mux_2level_size50_41_configbus0[2323:2323], mux_2level_size50_41_configbus1[2323:2323] , mux_2level_size50_41_configbus0_b[2323:2323] );
sram6T_blwl sram_blwl_2324_ (mux_2level_size50_41_sram_blwl_out[2324:2324] ,mux_2level_size50_41_sram_blwl_out[2324:2324] ,mux_2level_size50_41_sram_blwl_outb[2324:2324] ,mux_2level_size50_41_configbus0[2324:2324], mux_2level_size50_41_configbus1[2324:2324] , mux_2level_size50_41_configbus0_b[2324:2324] );
sram6T_blwl sram_blwl_2325_ (mux_2level_size50_41_sram_blwl_out[2325:2325] ,mux_2level_size50_41_sram_blwl_out[2325:2325] ,mux_2level_size50_41_sram_blwl_outb[2325:2325] ,mux_2level_size50_41_configbus0[2325:2325], mux_2level_size50_41_configbus1[2325:2325] , mux_2level_size50_41_configbus0_b[2325:2325] );
sram6T_blwl sram_blwl_2326_ (mux_2level_size50_41_sram_blwl_out[2326:2326] ,mux_2level_size50_41_sram_blwl_out[2326:2326] ,mux_2level_size50_41_sram_blwl_outb[2326:2326] ,mux_2level_size50_41_configbus0[2326:2326], mux_2level_size50_41_configbus1[2326:2326] , mux_2level_size50_41_configbus0_b[2326:2326] );
sram6T_blwl sram_blwl_2327_ (mux_2level_size50_41_sram_blwl_out[2327:2327] ,mux_2level_size50_41_sram_blwl_out[2327:2327] ,mux_2level_size50_41_sram_blwl_outb[2327:2327] ,mux_2level_size50_41_configbus0[2327:2327], mux_2level_size50_41_configbus1[2327:2327] , mux_2level_size50_41_configbus0_b[2327:2327] );
sram6T_blwl sram_blwl_2328_ (mux_2level_size50_41_sram_blwl_out[2328:2328] ,mux_2level_size50_41_sram_blwl_out[2328:2328] ,mux_2level_size50_41_sram_blwl_outb[2328:2328] ,mux_2level_size50_41_configbus0[2328:2328], mux_2level_size50_41_configbus1[2328:2328] , mux_2level_size50_41_configbus0_b[2328:2328] );
sram6T_blwl sram_blwl_2329_ (mux_2level_size50_41_sram_blwl_out[2329:2329] ,mux_2level_size50_41_sram_blwl_out[2329:2329] ,mux_2level_size50_41_sram_blwl_outb[2329:2329] ,mux_2level_size50_41_configbus0[2329:2329], mux_2level_size50_41_configbus1[2329:2329] , mux_2level_size50_41_configbus0_b[2329:2329] );
sram6T_blwl sram_blwl_2330_ (mux_2level_size50_41_sram_blwl_out[2330:2330] ,mux_2level_size50_41_sram_blwl_out[2330:2330] ,mux_2level_size50_41_sram_blwl_outb[2330:2330] ,mux_2level_size50_41_configbus0[2330:2330], mux_2level_size50_41_configbus1[2330:2330] , mux_2level_size50_41_configbus0_b[2330:2330] );
sram6T_blwl sram_blwl_2331_ (mux_2level_size50_41_sram_blwl_out[2331:2331] ,mux_2level_size50_41_sram_blwl_out[2331:2331] ,mux_2level_size50_41_sram_blwl_outb[2331:2331] ,mux_2level_size50_41_configbus0[2331:2331], mux_2level_size50_41_configbus1[2331:2331] , mux_2level_size50_41_configbus0_b[2331:2331] );
sram6T_blwl sram_blwl_2332_ (mux_2level_size50_41_sram_blwl_out[2332:2332] ,mux_2level_size50_41_sram_blwl_out[2332:2332] ,mux_2level_size50_41_sram_blwl_outb[2332:2332] ,mux_2level_size50_41_configbus0[2332:2332], mux_2level_size50_41_configbus1[2332:2332] , mux_2level_size50_41_configbus0_b[2332:2332] );
sram6T_blwl sram_blwl_2333_ (mux_2level_size50_41_sram_blwl_out[2333:2333] ,mux_2level_size50_41_sram_blwl_out[2333:2333] ,mux_2level_size50_41_sram_blwl_outb[2333:2333] ,mux_2level_size50_41_configbus0[2333:2333], mux_2level_size50_41_configbus1[2333:2333] , mux_2level_size50_41_configbus0_b[2333:2333] );
sram6T_blwl sram_blwl_2334_ (mux_2level_size50_41_sram_blwl_out[2334:2334] ,mux_2level_size50_41_sram_blwl_out[2334:2334] ,mux_2level_size50_41_sram_blwl_outb[2334:2334] ,mux_2level_size50_41_configbus0[2334:2334], mux_2level_size50_41_configbus1[2334:2334] , mux_2level_size50_41_configbus0_b[2334:2334] );
sram6T_blwl sram_blwl_2335_ (mux_2level_size50_41_sram_blwl_out[2335:2335] ,mux_2level_size50_41_sram_blwl_out[2335:2335] ,mux_2level_size50_41_sram_blwl_outb[2335:2335] ,mux_2level_size50_41_configbus0[2335:2335], mux_2level_size50_41_configbus1[2335:2335] , mux_2level_size50_41_configbus0_b[2335:2335] );
sram6T_blwl sram_blwl_2336_ (mux_2level_size50_41_sram_blwl_out[2336:2336] ,mux_2level_size50_41_sram_blwl_out[2336:2336] ,mux_2level_size50_41_sram_blwl_outb[2336:2336] ,mux_2level_size50_41_configbus0[2336:2336], mux_2level_size50_41_configbus1[2336:2336] , mux_2level_size50_41_configbus0_b[2336:2336] );
sram6T_blwl sram_blwl_2337_ (mux_2level_size50_41_sram_blwl_out[2337:2337] ,mux_2level_size50_41_sram_blwl_out[2337:2337] ,mux_2level_size50_41_sram_blwl_outb[2337:2337] ,mux_2level_size50_41_configbus0[2337:2337], mux_2level_size50_41_configbus1[2337:2337] , mux_2level_size50_41_configbus0_b[2337:2337] );
direct_interc direct_interc_176_ (mode_clb___clk_0_, fle_6___clk_0_ );
wire [0:49] in_bus_mux_2level_size50_42_ ;
assign in_bus_mux_2level_size50_42_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_42_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_42_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_42_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_42_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_42_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_42_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_42_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_42_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_42_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_42_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_42_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_42_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_42_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_42_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_42_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_42_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_42_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_42_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_42_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_42_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_42_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_42_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_42_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_42_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_42_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_42_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_42_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_42_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_42_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_42_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_42_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_42_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_42_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_42_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_42_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_42_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_42_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_42_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_42_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_42_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_42_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_42_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_42_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_42_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_42_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_42_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_42_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_42_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_42_[49] = fle_9___out_0_ ; 
wire [2338:2353] mux_2level_size50_42_configbus0;
wire [2338:2353] mux_2level_size50_42_configbus1;
wire [2338:2353] mux_2level_size50_42_sram_blwl_out ;
wire [2338:2353] mux_2level_size50_42_sram_blwl_outb ;
assign mux_2level_size50_42_configbus0[2338:2353] = sram_blwl_bl[2338:2353] ;
assign mux_2level_size50_42_configbus1[2338:2353] = sram_blwl_wl[2338:2353] ;
wire [2338:2353] mux_2level_size50_42_configbus0_b;
assign mux_2level_size50_42_configbus0_b[2338:2353] = sram_blwl_blb[2338:2353] ;
mux_2level_size50 mux_2level_size50_42_ (in_bus_mux_2level_size50_42_, fle_7___in_0_, mux_2level_size50_42_sram_blwl_out[2338:2353] ,
mux_2level_size50_42_sram_blwl_outb[2338:2353] );
//----- SRAM bits for MUX[42], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2338_ (mux_2level_size50_42_sram_blwl_out[2338:2338] ,mux_2level_size50_42_sram_blwl_out[2338:2338] ,mux_2level_size50_42_sram_blwl_outb[2338:2338] ,mux_2level_size50_42_configbus0[2338:2338], mux_2level_size50_42_configbus1[2338:2338] , mux_2level_size50_42_configbus0_b[2338:2338] );
sram6T_blwl sram_blwl_2339_ (mux_2level_size50_42_sram_blwl_out[2339:2339] ,mux_2level_size50_42_sram_blwl_out[2339:2339] ,mux_2level_size50_42_sram_blwl_outb[2339:2339] ,mux_2level_size50_42_configbus0[2339:2339], mux_2level_size50_42_configbus1[2339:2339] , mux_2level_size50_42_configbus0_b[2339:2339] );
sram6T_blwl sram_blwl_2340_ (mux_2level_size50_42_sram_blwl_out[2340:2340] ,mux_2level_size50_42_sram_blwl_out[2340:2340] ,mux_2level_size50_42_sram_blwl_outb[2340:2340] ,mux_2level_size50_42_configbus0[2340:2340], mux_2level_size50_42_configbus1[2340:2340] , mux_2level_size50_42_configbus0_b[2340:2340] );
sram6T_blwl sram_blwl_2341_ (mux_2level_size50_42_sram_blwl_out[2341:2341] ,mux_2level_size50_42_sram_blwl_out[2341:2341] ,mux_2level_size50_42_sram_blwl_outb[2341:2341] ,mux_2level_size50_42_configbus0[2341:2341], mux_2level_size50_42_configbus1[2341:2341] , mux_2level_size50_42_configbus0_b[2341:2341] );
sram6T_blwl sram_blwl_2342_ (mux_2level_size50_42_sram_blwl_out[2342:2342] ,mux_2level_size50_42_sram_blwl_out[2342:2342] ,mux_2level_size50_42_sram_blwl_outb[2342:2342] ,mux_2level_size50_42_configbus0[2342:2342], mux_2level_size50_42_configbus1[2342:2342] , mux_2level_size50_42_configbus0_b[2342:2342] );
sram6T_blwl sram_blwl_2343_ (mux_2level_size50_42_sram_blwl_out[2343:2343] ,mux_2level_size50_42_sram_blwl_out[2343:2343] ,mux_2level_size50_42_sram_blwl_outb[2343:2343] ,mux_2level_size50_42_configbus0[2343:2343], mux_2level_size50_42_configbus1[2343:2343] , mux_2level_size50_42_configbus0_b[2343:2343] );
sram6T_blwl sram_blwl_2344_ (mux_2level_size50_42_sram_blwl_out[2344:2344] ,mux_2level_size50_42_sram_blwl_out[2344:2344] ,mux_2level_size50_42_sram_blwl_outb[2344:2344] ,mux_2level_size50_42_configbus0[2344:2344], mux_2level_size50_42_configbus1[2344:2344] , mux_2level_size50_42_configbus0_b[2344:2344] );
sram6T_blwl sram_blwl_2345_ (mux_2level_size50_42_sram_blwl_out[2345:2345] ,mux_2level_size50_42_sram_blwl_out[2345:2345] ,mux_2level_size50_42_sram_blwl_outb[2345:2345] ,mux_2level_size50_42_configbus0[2345:2345], mux_2level_size50_42_configbus1[2345:2345] , mux_2level_size50_42_configbus0_b[2345:2345] );
sram6T_blwl sram_blwl_2346_ (mux_2level_size50_42_sram_blwl_out[2346:2346] ,mux_2level_size50_42_sram_blwl_out[2346:2346] ,mux_2level_size50_42_sram_blwl_outb[2346:2346] ,mux_2level_size50_42_configbus0[2346:2346], mux_2level_size50_42_configbus1[2346:2346] , mux_2level_size50_42_configbus0_b[2346:2346] );
sram6T_blwl sram_blwl_2347_ (mux_2level_size50_42_sram_blwl_out[2347:2347] ,mux_2level_size50_42_sram_blwl_out[2347:2347] ,mux_2level_size50_42_sram_blwl_outb[2347:2347] ,mux_2level_size50_42_configbus0[2347:2347], mux_2level_size50_42_configbus1[2347:2347] , mux_2level_size50_42_configbus0_b[2347:2347] );
sram6T_blwl sram_blwl_2348_ (mux_2level_size50_42_sram_blwl_out[2348:2348] ,mux_2level_size50_42_sram_blwl_out[2348:2348] ,mux_2level_size50_42_sram_blwl_outb[2348:2348] ,mux_2level_size50_42_configbus0[2348:2348], mux_2level_size50_42_configbus1[2348:2348] , mux_2level_size50_42_configbus0_b[2348:2348] );
sram6T_blwl sram_blwl_2349_ (mux_2level_size50_42_sram_blwl_out[2349:2349] ,mux_2level_size50_42_sram_blwl_out[2349:2349] ,mux_2level_size50_42_sram_blwl_outb[2349:2349] ,mux_2level_size50_42_configbus0[2349:2349], mux_2level_size50_42_configbus1[2349:2349] , mux_2level_size50_42_configbus0_b[2349:2349] );
sram6T_blwl sram_blwl_2350_ (mux_2level_size50_42_sram_blwl_out[2350:2350] ,mux_2level_size50_42_sram_blwl_out[2350:2350] ,mux_2level_size50_42_sram_blwl_outb[2350:2350] ,mux_2level_size50_42_configbus0[2350:2350], mux_2level_size50_42_configbus1[2350:2350] , mux_2level_size50_42_configbus0_b[2350:2350] );
sram6T_blwl sram_blwl_2351_ (mux_2level_size50_42_sram_blwl_out[2351:2351] ,mux_2level_size50_42_sram_blwl_out[2351:2351] ,mux_2level_size50_42_sram_blwl_outb[2351:2351] ,mux_2level_size50_42_configbus0[2351:2351], mux_2level_size50_42_configbus1[2351:2351] , mux_2level_size50_42_configbus0_b[2351:2351] );
sram6T_blwl sram_blwl_2352_ (mux_2level_size50_42_sram_blwl_out[2352:2352] ,mux_2level_size50_42_sram_blwl_out[2352:2352] ,mux_2level_size50_42_sram_blwl_outb[2352:2352] ,mux_2level_size50_42_configbus0[2352:2352], mux_2level_size50_42_configbus1[2352:2352] , mux_2level_size50_42_configbus0_b[2352:2352] );
sram6T_blwl sram_blwl_2353_ (mux_2level_size50_42_sram_blwl_out[2353:2353] ,mux_2level_size50_42_sram_blwl_out[2353:2353] ,mux_2level_size50_42_sram_blwl_outb[2353:2353] ,mux_2level_size50_42_configbus0[2353:2353], mux_2level_size50_42_configbus1[2353:2353] , mux_2level_size50_42_configbus0_b[2353:2353] );
wire [0:49] in_bus_mux_2level_size50_43_ ;
assign in_bus_mux_2level_size50_43_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_43_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_43_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_43_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_43_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_43_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_43_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_43_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_43_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_43_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_43_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_43_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_43_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_43_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_43_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_43_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_43_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_43_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_43_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_43_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_43_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_43_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_43_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_43_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_43_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_43_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_43_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_43_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_43_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_43_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_43_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_43_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_43_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_43_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_43_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_43_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_43_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_43_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_43_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_43_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_43_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_43_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_43_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_43_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_43_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_43_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_43_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_43_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_43_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_43_[49] = fle_9___out_0_ ; 
wire [2354:2369] mux_2level_size50_43_configbus0;
wire [2354:2369] mux_2level_size50_43_configbus1;
wire [2354:2369] mux_2level_size50_43_sram_blwl_out ;
wire [2354:2369] mux_2level_size50_43_sram_blwl_outb ;
assign mux_2level_size50_43_configbus0[2354:2369] = sram_blwl_bl[2354:2369] ;
assign mux_2level_size50_43_configbus1[2354:2369] = sram_blwl_wl[2354:2369] ;
wire [2354:2369] mux_2level_size50_43_configbus0_b;
assign mux_2level_size50_43_configbus0_b[2354:2369] = sram_blwl_blb[2354:2369] ;
mux_2level_size50 mux_2level_size50_43_ (in_bus_mux_2level_size50_43_, fle_7___in_1_, mux_2level_size50_43_sram_blwl_out[2354:2369] ,
mux_2level_size50_43_sram_blwl_outb[2354:2369] );
//----- SRAM bits for MUX[43], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2354_ (mux_2level_size50_43_sram_blwl_out[2354:2354] ,mux_2level_size50_43_sram_blwl_out[2354:2354] ,mux_2level_size50_43_sram_blwl_outb[2354:2354] ,mux_2level_size50_43_configbus0[2354:2354], mux_2level_size50_43_configbus1[2354:2354] , mux_2level_size50_43_configbus0_b[2354:2354] );
sram6T_blwl sram_blwl_2355_ (mux_2level_size50_43_sram_blwl_out[2355:2355] ,mux_2level_size50_43_sram_blwl_out[2355:2355] ,mux_2level_size50_43_sram_blwl_outb[2355:2355] ,mux_2level_size50_43_configbus0[2355:2355], mux_2level_size50_43_configbus1[2355:2355] , mux_2level_size50_43_configbus0_b[2355:2355] );
sram6T_blwl sram_blwl_2356_ (mux_2level_size50_43_sram_blwl_out[2356:2356] ,mux_2level_size50_43_sram_blwl_out[2356:2356] ,mux_2level_size50_43_sram_blwl_outb[2356:2356] ,mux_2level_size50_43_configbus0[2356:2356], mux_2level_size50_43_configbus1[2356:2356] , mux_2level_size50_43_configbus0_b[2356:2356] );
sram6T_blwl sram_blwl_2357_ (mux_2level_size50_43_sram_blwl_out[2357:2357] ,mux_2level_size50_43_sram_blwl_out[2357:2357] ,mux_2level_size50_43_sram_blwl_outb[2357:2357] ,mux_2level_size50_43_configbus0[2357:2357], mux_2level_size50_43_configbus1[2357:2357] , mux_2level_size50_43_configbus0_b[2357:2357] );
sram6T_blwl sram_blwl_2358_ (mux_2level_size50_43_sram_blwl_out[2358:2358] ,mux_2level_size50_43_sram_blwl_out[2358:2358] ,mux_2level_size50_43_sram_blwl_outb[2358:2358] ,mux_2level_size50_43_configbus0[2358:2358], mux_2level_size50_43_configbus1[2358:2358] , mux_2level_size50_43_configbus0_b[2358:2358] );
sram6T_blwl sram_blwl_2359_ (mux_2level_size50_43_sram_blwl_out[2359:2359] ,mux_2level_size50_43_sram_blwl_out[2359:2359] ,mux_2level_size50_43_sram_blwl_outb[2359:2359] ,mux_2level_size50_43_configbus0[2359:2359], mux_2level_size50_43_configbus1[2359:2359] , mux_2level_size50_43_configbus0_b[2359:2359] );
sram6T_blwl sram_blwl_2360_ (mux_2level_size50_43_sram_blwl_out[2360:2360] ,mux_2level_size50_43_sram_blwl_out[2360:2360] ,mux_2level_size50_43_sram_blwl_outb[2360:2360] ,mux_2level_size50_43_configbus0[2360:2360], mux_2level_size50_43_configbus1[2360:2360] , mux_2level_size50_43_configbus0_b[2360:2360] );
sram6T_blwl sram_blwl_2361_ (mux_2level_size50_43_sram_blwl_out[2361:2361] ,mux_2level_size50_43_sram_blwl_out[2361:2361] ,mux_2level_size50_43_sram_blwl_outb[2361:2361] ,mux_2level_size50_43_configbus0[2361:2361], mux_2level_size50_43_configbus1[2361:2361] , mux_2level_size50_43_configbus0_b[2361:2361] );
sram6T_blwl sram_blwl_2362_ (mux_2level_size50_43_sram_blwl_out[2362:2362] ,mux_2level_size50_43_sram_blwl_out[2362:2362] ,mux_2level_size50_43_sram_blwl_outb[2362:2362] ,mux_2level_size50_43_configbus0[2362:2362], mux_2level_size50_43_configbus1[2362:2362] , mux_2level_size50_43_configbus0_b[2362:2362] );
sram6T_blwl sram_blwl_2363_ (mux_2level_size50_43_sram_blwl_out[2363:2363] ,mux_2level_size50_43_sram_blwl_out[2363:2363] ,mux_2level_size50_43_sram_blwl_outb[2363:2363] ,mux_2level_size50_43_configbus0[2363:2363], mux_2level_size50_43_configbus1[2363:2363] , mux_2level_size50_43_configbus0_b[2363:2363] );
sram6T_blwl sram_blwl_2364_ (mux_2level_size50_43_sram_blwl_out[2364:2364] ,mux_2level_size50_43_sram_blwl_out[2364:2364] ,mux_2level_size50_43_sram_blwl_outb[2364:2364] ,mux_2level_size50_43_configbus0[2364:2364], mux_2level_size50_43_configbus1[2364:2364] , mux_2level_size50_43_configbus0_b[2364:2364] );
sram6T_blwl sram_blwl_2365_ (mux_2level_size50_43_sram_blwl_out[2365:2365] ,mux_2level_size50_43_sram_blwl_out[2365:2365] ,mux_2level_size50_43_sram_blwl_outb[2365:2365] ,mux_2level_size50_43_configbus0[2365:2365], mux_2level_size50_43_configbus1[2365:2365] , mux_2level_size50_43_configbus0_b[2365:2365] );
sram6T_blwl sram_blwl_2366_ (mux_2level_size50_43_sram_blwl_out[2366:2366] ,mux_2level_size50_43_sram_blwl_out[2366:2366] ,mux_2level_size50_43_sram_blwl_outb[2366:2366] ,mux_2level_size50_43_configbus0[2366:2366], mux_2level_size50_43_configbus1[2366:2366] , mux_2level_size50_43_configbus0_b[2366:2366] );
sram6T_blwl sram_blwl_2367_ (mux_2level_size50_43_sram_blwl_out[2367:2367] ,mux_2level_size50_43_sram_blwl_out[2367:2367] ,mux_2level_size50_43_sram_blwl_outb[2367:2367] ,mux_2level_size50_43_configbus0[2367:2367], mux_2level_size50_43_configbus1[2367:2367] , mux_2level_size50_43_configbus0_b[2367:2367] );
sram6T_blwl sram_blwl_2368_ (mux_2level_size50_43_sram_blwl_out[2368:2368] ,mux_2level_size50_43_sram_blwl_out[2368:2368] ,mux_2level_size50_43_sram_blwl_outb[2368:2368] ,mux_2level_size50_43_configbus0[2368:2368], mux_2level_size50_43_configbus1[2368:2368] , mux_2level_size50_43_configbus0_b[2368:2368] );
sram6T_blwl sram_blwl_2369_ (mux_2level_size50_43_sram_blwl_out[2369:2369] ,mux_2level_size50_43_sram_blwl_out[2369:2369] ,mux_2level_size50_43_sram_blwl_outb[2369:2369] ,mux_2level_size50_43_configbus0[2369:2369], mux_2level_size50_43_configbus1[2369:2369] , mux_2level_size50_43_configbus0_b[2369:2369] );
wire [0:49] in_bus_mux_2level_size50_44_ ;
assign in_bus_mux_2level_size50_44_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_44_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_44_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_44_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_44_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_44_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_44_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_44_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_44_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_44_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_44_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_44_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_44_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_44_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_44_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_44_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_44_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_44_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_44_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_44_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_44_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_44_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_44_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_44_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_44_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_44_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_44_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_44_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_44_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_44_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_44_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_44_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_44_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_44_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_44_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_44_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_44_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_44_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_44_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_44_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_44_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_44_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_44_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_44_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_44_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_44_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_44_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_44_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_44_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_44_[49] = fle_9___out_0_ ; 
wire [2370:2385] mux_2level_size50_44_configbus0;
wire [2370:2385] mux_2level_size50_44_configbus1;
wire [2370:2385] mux_2level_size50_44_sram_blwl_out ;
wire [2370:2385] mux_2level_size50_44_sram_blwl_outb ;
assign mux_2level_size50_44_configbus0[2370:2385] = sram_blwl_bl[2370:2385] ;
assign mux_2level_size50_44_configbus1[2370:2385] = sram_blwl_wl[2370:2385] ;
wire [2370:2385] mux_2level_size50_44_configbus0_b;
assign mux_2level_size50_44_configbus0_b[2370:2385] = sram_blwl_blb[2370:2385] ;
mux_2level_size50 mux_2level_size50_44_ (in_bus_mux_2level_size50_44_, fle_7___in_2_, mux_2level_size50_44_sram_blwl_out[2370:2385] ,
mux_2level_size50_44_sram_blwl_outb[2370:2385] );
//----- SRAM bits for MUX[44], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2370_ (mux_2level_size50_44_sram_blwl_out[2370:2370] ,mux_2level_size50_44_sram_blwl_out[2370:2370] ,mux_2level_size50_44_sram_blwl_outb[2370:2370] ,mux_2level_size50_44_configbus0[2370:2370], mux_2level_size50_44_configbus1[2370:2370] , mux_2level_size50_44_configbus0_b[2370:2370] );
sram6T_blwl sram_blwl_2371_ (mux_2level_size50_44_sram_blwl_out[2371:2371] ,mux_2level_size50_44_sram_blwl_out[2371:2371] ,mux_2level_size50_44_sram_blwl_outb[2371:2371] ,mux_2level_size50_44_configbus0[2371:2371], mux_2level_size50_44_configbus1[2371:2371] , mux_2level_size50_44_configbus0_b[2371:2371] );
sram6T_blwl sram_blwl_2372_ (mux_2level_size50_44_sram_blwl_out[2372:2372] ,mux_2level_size50_44_sram_blwl_out[2372:2372] ,mux_2level_size50_44_sram_blwl_outb[2372:2372] ,mux_2level_size50_44_configbus0[2372:2372], mux_2level_size50_44_configbus1[2372:2372] , mux_2level_size50_44_configbus0_b[2372:2372] );
sram6T_blwl sram_blwl_2373_ (mux_2level_size50_44_sram_blwl_out[2373:2373] ,mux_2level_size50_44_sram_blwl_out[2373:2373] ,mux_2level_size50_44_sram_blwl_outb[2373:2373] ,mux_2level_size50_44_configbus0[2373:2373], mux_2level_size50_44_configbus1[2373:2373] , mux_2level_size50_44_configbus0_b[2373:2373] );
sram6T_blwl sram_blwl_2374_ (mux_2level_size50_44_sram_blwl_out[2374:2374] ,mux_2level_size50_44_sram_blwl_out[2374:2374] ,mux_2level_size50_44_sram_blwl_outb[2374:2374] ,mux_2level_size50_44_configbus0[2374:2374], mux_2level_size50_44_configbus1[2374:2374] , mux_2level_size50_44_configbus0_b[2374:2374] );
sram6T_blwl sram_blwl_2375_ (mux_2level_size50_44_sram_blwl_out[2375:2375] ,mux_2level_size50_44_sram_blwl_out[2375:2375] ,mux_2level_size50_44_sram_blwl_outb[2375:2375] ,mux_2level_size50_44_configbus0[2375:2375], mux_2level_size50_44_configbus1[2375:2375] , mux_2level_size50_44_configbus0_b[2375:2375] );
sram6T_blwl sram_blwl_2376_ (mux_2level_size50_44_sram_blwl_out[2376:2376] ,mux_2level_size50_44_sram_blwl_out[2376:2376] ,mux_2level_size50_44_sram_blwl_outb[2376:2376] ,mux_2level_size50_44_configbus0[2376:2376], mux_2level_size50_44_configbus1[2376:2376] , mux_2level_size50_44_configbus0_b[2376:2376] );
sram6T_blwl sram_blwl_2377_ (mux_2level_size50_44_sram_blwl_out[2377:2377] ,mux_2level_size50_44_sram_blwl_out[2377:2377] ,mux_2level_size50_44_sram_blwl_outb[2377:2377] ,mux_2level_size50_44_configbus0[2377:2377], mux_2level_size50_44_configbus1[2377:2377] , mux_2level_size50_44_configbus0_b[2377:2377] );
sram6T_blwl sram_blwl_2378_ (mux_2level_size50_44_sram_blwl_out[2378:2378] ,mux_2level_size50_44_sram_blwl_out[2378:2378] ,mux_2level_size50_44_sram_blwl_outb[2378:2378] ,mux_2level_size50_44_configbus0[2378:2378], mux_2level_size50_44_configbus1[2378:2378] , mux_2level_size50_44_configbus0_b[2378:2378] );
sram6T_blwl sram_blwl_2379_ (mux_2level_size50_44_sram_blwl_out[2379:2379] ,mux_2level_size50_44_sram_blwl_out[2379:2379] ,mux_2level_size50_44_sram_blwl_outb[2379:2379] ,mux_2level_size50_44_configbus0[2379:2379], mux_2level_size50_44_configbus1[2379:2379] , mux_2level_size50_44_configbus0_b[2379:2379] );
sram6T_blwl sram_blwl_2380_ (mux_2level_size50_44_sram_blwl_out[2380:2380] ,mux_2level_size50_44_sram_blwl_out[2380:2380] ,mux_2level_size50_44_sram_blwl_outb[2380:2380] ,mux_2level_size50_44_configbus0[2380:2380], mux_2level_size50_44_configbus1[2380:2380] , mux_2level_size50_44_configbus0_b[2380:2380] );
sram6T_blwl sram_blwl_2381_ (mux_2level_size50_44_sram_blwl_out[2381:2381] ,mux_2level_size50_44_sram_blwl_out[2381:2381] ,mux_2level_size50_44_sram_blwl_outb[2381:2381] ,mux_2level_size50_44_configbus0[2381:2381], mux_2level_size50_44_configbus1[2381:2381] , mux_2level_size50_44_configbus0_b[2381:2381] );
sram6T_blwl sram_blwl_2382_ (mux_2level_size50_44_sram_blwl_out[2382:2382] ,mux_2level_size50_44_sram_blwl_out[2382:2382] ,mux_2level_size50_44_sram_blwl_outb[2382:2382] ,mux_2level_size50_44_configbus0[2382:2382], mux_2level_size50_44_configbus1[2382:2382] , mux_2level_size50_44_configbus0_b[2382:2382] );
sram6T_blwl sram_blwl_2383_ (mux_2level_size50_44_sram_blwl_out[2383:2383] ,mux_2level_size50_44_sram_blwl_out[2383:2383] ,mux_2level_size50_44_sram_blwl_outb[2383:2383] ,mux_2level_size50_44_configbus0[2383:2383], mux_2level_size50_44_configbus1[2383:2383] , mux_2level_size50_44_configbus0_b[2383:2383] );
sram6T_blwl sram_blwl_2384_ (mux_2level_size50_44_sram_blwl_out[2384:2384] ,mux_2level_size50_44_sram_blwl_out[2384:2384] ,mux_2level_size50_44_sram_blwl_outb[2384:2384] ,mux_2level_size50_44_configbus0[2384:2384], mux_2level_size50_44_configbus1[2384:2384] , mux_2level_size50_44_configbus0_b[2384:2384] );
sram6T_blwl sram_blwl_2385_ (mux_2level_size50_44_sram_blwl_out[2385:2385] ,mux_2level_size50_44_sram_blwl_out[2385:2385] ,mux_2level_size50_44_sram_blwl_outb[2385:2385] ,mux_2level_size50_44_configbus0[2385:2385], mux_2level_size50_44_configbus1[2385:2385] , mux_2level_size50_44_configbus0_b[2385:2385] );
wire [0:49] in_bus_mux_2level_size50_45_ ;
assign in_bus_mux_2level_size50_45_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_45_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_45_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_45_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_45_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_45_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_45_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_45_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_45_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_45_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_45_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_45_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_45_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_45_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_45_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_45_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_45_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_45_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_45_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_45_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_45_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_45_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_45_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_45_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_45_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_45_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_45_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_45_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_45_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_45_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_45_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_45_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_45_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_45_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_45_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_45_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_45_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_45_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_45_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_45_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_45_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_45_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_45_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_45_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_45_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_45_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_45_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_45_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_45_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_45_[49] = fle_9___out_0_ ; 
wire [2386:2401] mux_2level_size50_45_configbus0;
wire [2386:2401] mux_2level_size50_45_configbus1;
wire [2386:2401] mux_2level_size50_45_sram_blwl_out ;
wire [2386:2401] mux_2level_size50_45_sram_blwl_outb ;
assign mux_2level_size50_45_configbus0[2386:2401] = sram_blwl_bl[2386:2401] ;
assign mux_2level_size50_45_configbus1[2386:2401] = sram_blwl_wl[2386:2401] ;
wire [2386:2401] mux_2level_size50_45_configbus0_b;
assign mux_2level_size50_45_configbus0_b[2386:2401] = sram_blwl_blb[2386:2401] ;
mux_2level_size50 mux_2level_size50_45_ (in_bus_mux_2level_size50_45_, fle_7___in_3_, mux_2level_size50_45_sram_blwl_out[2386:2401] ,
mux_2level_size50_45_sram_blwl_outb[2386:2401] );
//----- SRAM bits for MUX[45], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2386_ (mux_2level_size50_45_sram_blwl_out[2386:2386] ,mux_2level_size50_45_sram_blwl_out[2386:2386] ,mux_2level_size50_45_sram_blwl_outb[2386:2386] ,mux_2level_size50_45_configbus0[2386:2386], mux_2level_size50_45_configbus1[2386:2386] , mux_2level_size50_45_configbus0_b[2386:2386] );
sram6T_blwl sram_blwl_2387_ (mux_2level_size50_45_sram_blwl_out[2387:2387] ,mux_2level_size50_45_sram_blwl_out[2387:2387] ,mux_2level_size50_45_sram_blwl_outb[2387:2387] ,mux_2level_size50_45_configbus0[2387:2387], mux_2level_size50_45_configbus1[2387:2387] , mux_2level_size50_45_configbus0_b[2387:2387] );
sram6T_blwl sram_blwl_2388_ (mux_2level_size50_45_sram_blwl_out[2388:2388] ,mux_2level_size50_45_sram_blwl_out[2388:2388] ,mux_2level_size50_45_sram_blwl_outb[2388:2388] ,mux_2level_size50_45_configbus0[2388:2388], mux_2level_size50_45_configbus1[2388:2388] , mux_2level_size50_45_configbus0_b[2388:2388] );
sram6T_blwl sram_blwl_2389_ (mux_2level_size50_45_sram_blwl_out[2389:2389] ,mux_2level_size50_45_sram_blwl_out[2389:2389] ,mux_2level_size50_45_sram_blwl_outb[2389:2389] ,mux_2level_size50_45_configbus0[2389:2389], mux_2level_size50_45_configbus1[2389:2389] , mux_2level_size50_45_configbus0_b[2389:2389] );
sram6T_blwl sram_blwl_2390_ (mux_2level_size50_45_sram_blwl_out[2390:2390] ,mux_2level_size50_45_sram_blwl_out[2390:2390] ,mux_2level_size50_45_sram_blwl_outb[2390:2390] ,mux_2level_size50_45_configbus0[2390:2390], mux_2level_size50_45_configbus1[2390:2390] , mux_2level_size50_45_configbus0_b[2390:2390] );
sram6T_blwl sram_blwl_2391_ (mux_2level_size50_45_sram_blwl_out[2391:2391] ,mux_2level_size50_45_sram_blwl_out[2391:2391] ,mux_2level_size50_45_sram_blwl_outb[2391:2391] ,mux_2level_size50_45_configbus0[2391:2391], mux_2level_size50_45_configbus1[2391:2391] , mux_2level_size50_45_configbus0_b[2391:2391] );
sram6T_blwl sram_blwl_2392_ (mux_2level_size50_45_sram_blwl_out[2392:2392] ,mux_2level_size50_45_sram_blwl_out[2392:2392] ,mux_2level_size50_45_sram_blwl_outb[2392:2392] ,mux_2level_size50_45_configbus0[2392:2392], mux_2level_size50_45_configbus1[2392:2392] , mux_2level_size50_45_configbus0_b[2392:2392] );
sram6T_blwl sram_blwl_2393_ (mux_2level_size50_45_sram_blwl_out[2393:2393] ,mux_2level_size50_45_sram_blwl_out[2393:2393] ,mux_2level_size50_45_sram_blwl_outb[2393:2393] ,mux_2level_size50_45_configbus0[2393:2393], mux_2level_size50_45_configbus1[2393:2393] , mux_2level_size50_45_configbus0_b[2393:2393] );
sram6T_blwl sram_blwl_2394_ (mux_2level_size50_45_sram_blwl_out[2394:2394] ,mux_2level_size50_45_sram_blwl_out[2394:2394] ,mux_2level_size50_45_sram_blwl_outb[2394:2394] ,mux_2level_size50_45_configbus0[2394:2394], mux_2level_size50_45_configbus1[2394:2394] , mux_2level_size50_45_configbus0_b[2394:2394] );
sram6T_blwl sram_blwl_2395_ (mux_2level_size50_45_sram_blwl_out[2395:2395] ,mux_2level_size50_45_sram_blwl_out[2395:2395] ,mux_2level_size50_45_sram_blwl_outb[2395:2395] ,mux_2level_size50_45_configbus0[2395:2395], mux_2level_size50_45_configbus1[2395:2395] , mux_2level_size50_45_configbus0_b[2395:2395] );
sram6T_blwl sram_blwl_2396_ (mux_2level_size50_45_sram_blwl_out[2396:2396] ,mux_2level_size50_45_sram_blwl_out[2396:2396] ,mux_2level_size50_45_sram_blwl_outb[2396:2396] ,mux_2level_size50_45_configbus0[2396:2396], mux_2level_size50_45_configbus1[2396:2396] , mux_2level_size50_45_configbus0_b[2396:2396] );
sram6T_blwl sram_blwl_2397_ (mux_2level_size50_45_sram_blwl_out[2397:2397] ,mux_2level_size50_45_sram_blwl_out[2397:2397] ,mux_2level_size50_45_sram_blwl_outb[2397:2397] ,mux_2level_size50_45_configbus0[2397:2397], mux_2level_size50_45_configbus1[2397:2397] , mux_2level_size50_45_configbus0_b[2397:2397] );
sram6T_blwl sram_blwl_2398_ (mux_2level_size50_45_sram_blwl_out[2398:2398] ,mux_2level_size50_45_sram_blwl_out[2398:2398] ,mux_2level_size50_45_sram_blwl_outb[2398:2398] ,mux_2level_size50_45_configbus0[2398:2398], mux_2level_size50_45_configbus1[2398:2398] , mux_2level_size50_45_configbus0_b[2398:2398] );
sram6T_blwl sram_blwl_2399_ (mux_2level_size50_45_sram_blwl_out[2399:2399] ,mux_2level_size50_45_sram_blwl_out[2399:2399] ,mux_2level_size50_45_sram_blwl_outb[2399:2399] ,mux_2level_size50_45_configbus0[2399:2399], mux_2level_size50_45_configbus1[2399:2399] , mux_2level_size50_45_configbus0_b[2399:2399] );
sram6T_blwl sram_blwl_2400_ (mux_2level_size50_45_sram_blwl_out[2400:2400] ,mux_2level_size50_45_sram_blwl_out[2400:2400] ,mux_2level_size50_45_sram_blwl_outb[2400:2400] ,mux_2level_size50_45_configbus0[2400:2400], mux_2level_size50_45_configbus1[2400:2400] , mux_2level_size50_45_configbus0_b[2400:2400] );
sram6T_blwl sram_blwl_2401_ (mux_2level_size50_45_sram_blwl_out[2401:2401] ,mux_2level_size50_45_sram_blwl_out[2401:2401] ,mux_2level_size50_45_sram_blwl_outb[2401:2401] ,mux_2level_size50_45_configbus0[2401:2401], mux_2level_size50_45_configbus1[2401:2401] , mux_2level_size50_45_configbus0_b[2401:2401] );
wire [0:49] in_bus_mux_2level_size50_46_ ;
assign in_bus_mux_2level_size50_46_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_46_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_46_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_46_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_46_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_46_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_46_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_46_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_46_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_46_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_46_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_46_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_46_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_46_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_46_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_46_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_46_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_46_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_46_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_46_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_46_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_46_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_46_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_46_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_46_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_46_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_46_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_46_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_46_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_46_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_46_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_46_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_46_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_46_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_46_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_46_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_46_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_46_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_46_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_46_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_46_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_46_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_46_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_46_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_46_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_46_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_46_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_46_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_46_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_46_[49] = fle_9___out_0_ ; 
wire [2402:2417] mux_2level_size50_46_configbus0;
wire [2402:2417] mux_2level_size50_46_configbus1;
wire [2402:2417] mux_2level_size50_46_sram_blwl_out ;
wire [2402:2417] mux_2level_size50_46_sram_blwl_outb ;
assign mux_2level_size50_46_configbus0[2402:2417] = sram_blwl_bl[2402:2417] ;
assign mux_2level_size50_46_configbus1[2402:2417] = sram_blwl_wl[2402:2417] ;
wire [2402:2417] mux_2level_size50_46_configbus0_b;
assign mux_2level_size50_46_configbus0_b[2402:2417] = sram_blwl_blb[2402:2417] ;
mux_2level_size50 mux_2level_size50_46_ (in_bus_mux_2level_size50_46_, fle_7___in_4_, mux_2level_size50_46_sram_blwl_out[2402:2417] ,
mux_2level_size50_46_sram_blwl_outb[2402:2417] );
//----- SRAM bits for MUX[46], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2402_ (mux_2level_size50_46_sram_blwl_out[2402:2402] ,mux_2level_size50_46_sram_blwl_out[2402:2402] ,mux_2level_size50_46_sram_blwl_outb[2402:2402] ,mux_2level_size50_46_configbus0[2402:2402], mux_2level_size50_46_configbus1[2402:2402] , mux_2level_size50_46_configbus0_b[2402:2402] );
sram6T_blwl sram_blwl_2403_ (mux_2level_size50_46_sram_blwl_out[2403:2403] ,mux_2level_size50_46_sram_blwl_out[2403:2403] ,mux_2level_size50_46_sram_blwl_outb[2403:2403] ,mux_2level_size50_46_configbus0[2403:2403], mux_2level_size50_46_configbus1[2403:2403] , mux_2level_size50_46_configbus0_b[2403:2403] );
sram6T_blwl sram_blwl_2404_ (mux_2level_size50_46_sram_blwl_out[2404:2404] ,mux_2level_size50_46_sram_blwl_out[2404:2404] ,mux_2level_size50_46_sram_blwl_outb[2404:2404] ,mux_2level_size50_46_configbus0[2404:2404], mux_2level_size50_46_configbus1[2404:2404] , mux_2level_size50_46_configbus0_b[2404:2404] );
sram6T_blwl sram_blwl_2405_ (mux_2level_size50_46_sram_blwl_out[2405:2405] ,mux_2level_size50_46_sram_blwl_out[2405:2405] ,mux_2level_size50_46_sram_blwl_outb[2405:2405] ,mux_2level_size50_46_configbus0[2405:2405], mux_2level_size50_46_configbus1[2405:2405] , mux_2level_size50_46_configbus0_b[2405:2405] );
sram6T_blwl sram_blwl_2406_ (mux_2level_size50_46_sram_blwl_out[2406:2406] ,mux_2level_size50_46_sram_blwl_out[2406:2406] ,mux_2level_size50_46_sram_blwl_outb[2406:2406] ,mux_2level_size50_46_configbus0[2406:2406], mux_2level_size50_46_configbus1[2406:2406] , mux_2level_size50_46_configbus0_b[2406:2406] );
sram6T_blwl sram_blwl_2407_ (mux_2level_size50_46_sram_blwl_out[2407:2407] ,mux_2level_size50_46_sram_blwl_out[2407:2407] ,mux_2level_size50_46_sram_blwl_outb[2407:2407] ,mux_2level_size50_46_configbus0[2407:2407], mux_2level_size50_46_configbus1[2407:2407] , mux_2level_size50_46_configbus0_b[2407:2407] );
sram6T_blwl sram_blwl_2408_ (mux_2level_size50_46_sram_blwl_out[2408:2408] ,mux_2level_size50_46_sram_blwl_out[2408:2408] ,mux_2level_size50_46_sram_blwl_outb[2408:2408] ,mux_2level_size50_46_configbus0[2408:2408], mux_2level_size50_46_configbus1[2408:2408] , mux_2level_size50_46_configbus0_b[2408:2408] );
sram6T_blwl sram_blwl_2409_ (mux_2level_size50_46_sram_blwl_out[2409:2409] ,mux_2level_size50_46_sram_blwl_out[2409:2409] ,mux_2level_size50_46_sram_blwl_outb[2409:2409] ,mux_2level_size50_46_configbus0[2409:2409], mux_2level_size50_46_configbus1[2409:2409] , mux_2level_size50_46_configbus0_b[2409:2409] );
sram6T_blwl sram_blwl_2410_ (mux_2level_size50_46_sram_blwl_out[2410:2410] ,mux_2level_size50_46_sram_blwl_out[2410:2410] ,mux_2level_size50_46_sram_blwl_outb[2410:2410] ,mux_2level_size50_46_configbus0[2410:2410], mux_2level_size50_46_configbus1[2410:2410] , mux_2level_size50_46_configbus0_b[2410:2410] );
sram6T_blwl sram_blwl_2411_ (mux_2level_size50_46_sram_blwl_out[2411:2411] ,mux_2level_size50_46_sram_blwl_out[2411:2411] ,mux_2level_size50_46_sram_blwl_outb[2411:2411] ,mux_2level_size50_46_configbus0[2411:2411], mux_2level_size50_46_configbus1[2411:2411] , mux_2level_size50_46_configbus0_b[2411:2411] );
sram6T_blwl sram_blwl_2412_ (mux_2level_size50_46_sram_blwl_out[2412:2412] ,mux_2level_size50_46_sram_blwl_out[2412:2412] ,mux_2level_size50_46_sram_blwl_outb[2412:2412] ,mux_2level_size50_46_configbus0[2412:2412], mux_2level_size50_46_configbus1[2412:2412] , mux_2level_size50_46_configbus0_b[2412:2412] );
sram6T_blwl sram_blwl_2413_ (mux_2level_size50_46_sram_blwl_out[2413:2413] ,mux_2level_size50_46_sram_blwl_out[2413:2413] ,mux_2level_size50_46_sram_blwl_outb[2413:2413] ,mux_2level_size50_46_configbus0[2413:2413], mux_2level_size50_46_configbus1[2413:2413] , mux_2level_size50_46_configbus0_b[2413:2413] );
sram6T_blwl sram_blwl_2414_ (mux_2level_size50_46_sram_blwl_out[2414:2414] ,mux_2level_size50_46_sram_blwl_out[2414:2414] ,mux_2level_size50_46_sram_blwl_outb[2414:2414] ,mux_2level_size50_46_configbus0[2414:2414], mux_2level_size50_46_configbus1[2414:2414] , mux_2level_size50_46_configbus0_b[2414:2414] );
sram6T_blwl sram_blwl_2415_ (mux_2level_size50_46_sram_blwl_out[2415:2415] ,mux_2level_size50_46_sram_blwl_out[2415:2415] ,mux_2level_size50_46_sram_blwl_outb[2415:2415] ,mux_2level_size50_46_configbus0[2415:2415], mux_2level_size50_46_configbus1[2415:2415] , mux_2level_size50_46_configbus0_b[2415:2415] );
sram6T_blwl sram_blwl_2416_ (mux_2level_size50_46_sram_blwl_out[2416:2416] ,mux_2level_size50_46_sram_blwl_out[2416:2416] ,mux_2level_size50_46_sram_blwl_outb[2416:2416] ,mux_2level_size50_46_configbus0[2416:2416], mux_2level_size50_46_configbus1[2416:2416] , mux_2level_size50_46_configbus0_b[2416:2416] );
sram6T_blwl sram_blwl_2417_ (mux_2level_size50_46_sram_blwl_out[2417:2417] ,mux_2level_size50_46_sram_blwl_out[2417:2417] ,mux_2level_size50_46_sram_blwl_outb[2417:2417] ,mux_2level_size50_46_configbus0[2417:2417], mux_2level_size50_46_configbus1[2417:2417] , mux_2level_size50_46_configbus0_b[2417:2417] );
wire [0:49] in_bus_mux_2level_size50_47_ ;
assign in_bus_mux_2level_size50_47_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_47_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_47_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_47_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_47_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_47_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_47_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_47_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_47_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_47_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_47_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_47_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_47_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_47_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_47_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_47_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_47_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_47_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_47_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_47_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_47_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_47_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_47_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_47_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_47_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_47_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_47_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_47_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_47_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_47_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_47_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_47_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_47_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_47_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_47_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_47_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_47_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_47_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_47_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_47_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_47_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_47_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_47_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_47_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_47_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_47_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_47_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_47_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_47_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_47_[49] = fle_9___out_0_ ; 
wire [2418:2433] mux_2level_size50_47_configbus0;
wire [2418:2433] mux_2level_size50_47_configbus1;
wire [2418:2433] mux_2level_size50_47_sram_blwl_out ;
wire [2418:2433] mux_2level_size50_47_sram_blwl_outb ;
assign mux_2level_size50_47_configbus0[2418:2433] = sram_blwl_bl[2418:2433] ;
assign mux_2level_size50_47_configbus1[2418:2433] = sram_blwl_wl[2418:2433] ;
wire [2418:2433] mux_2level_size50_47_configbus0_b;
assign mux_2level_size50_47_configbus0_b[2418:2433] = sram_blwl_blb[2418:2433] ;
mux_2level_size50 mux_2level_size50_47_ (in_bus_mux_2level_size50_47_, fle_7___in_5_, mux_2level_size50_47_sram_blwl_out[2418:2433] ,
mux_2level_size50_47_sram_blwl_outb[2418:2433] );
//----- SRAM bits for MUX[47], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2418_ (mux_2level_size50_47_sram_blwl_out[2418:2418] ,mux_2level_size50_47_sram_blwl_out[2418:2418] ,mux_2level_size50_47_sram_blwl_outb[2418:2418] ,mux_2level_size50_47_configbus0[2418:2418], mux_2level_size50_47_configbus1[2418:2418] , mux_2level_size50_47_configbus0_b[2418:2418] );
sram6T_blwl sram_blwl_2419_ (mux_2level_size50_47_sram_blwl_out[2419:2419] ,mux_2level_size50_47_sram_blwl_out[2419:2419] ,mux_2level_size50_47_sram_blwl_outb[2419:2419] ,mux_2level_size50_47_configbus0[2419:2419], mux_2level_size50_47_configbus1[2419:2419] , mux_2level_size50_47_configbus0_b[2419:2419] );
sram6T_blwl sram_blwl_2420_ (mux_2level_size50_47_sram_blwl_out[2420:2420] ,mux_2level_size50_47_sram_blwl_out[2420:2420] ,mux_2level_size50_47_sram_blwl_outb[2420:2420] ,mux_2level_size50_47_configbus0[2420:2420], mux_2level_size50_47_configbus1[2420:2420] , mux_2level_size50_47_configbus0_b[2420:2420] );
sram6T_blwl sram_blwl_2421_ (mux_2level_size50_47_sram_blwl_out[2421:2421] ,mux_2level_size50_47_sram_blwl_out[2421:2421] ,mux_2level_size50_47_sram_blwl_outb[2421:2421] ,mux_2level_size50_47_configbus0[2421:2421], mux_2level_size50_47_configbus1[2421:2421] , mux_2level_size50_47_configbus0_b[2421:2421] );
sram6T_blwl sram_blwl_2422_ (mux_2level_size50_47_sram_blwl_out[2422:2422] ,mux_2level_size50_47_sram_blwl_out[2422:2422] ,mux_2level_size50_47_sram_blwl_outb[2422:2422] ,mux_2level_size50_47_configbus0[2422:2422], mux_2level_size50_47_configbus1[2422:2422] , mux_2level_size50_47_configbus0_b[2422:2422] );
sram6T_blwl sram_blwl_2423_ (mux_2level_size50_47_sram_blwl_out[2423:2423] ,mux_2level_size50_47_sram_blwl_out[2423:2423] ,mux_2level_size50_47_sram_blwl_outb[2423:2423] ,mux_2level_size50_47_configbus0[2423:2423], mux_2level_size50_47_configbus1[2423:2423] , mux_2level_size50_47_configbus0_b[2423:2423] );
sram6T_blwl sram_blwl_2424_ (mux_2level_size50_47_sram_blwl_out[2424:2424] ,mux_2level_size50_47_sram_blwl_out[2424:2424] ,mux_2level_size50_47_sram_blwl_outb[2424:2424] ,mux_2level_size50_47_configbus0[2424:2424], mux_2level_size50_47_configbus1[2424:2424] , mux_2level_size50_47_configbus0_b[2424:2424] );
sram6T_blwl sram_blwl_2425_ (mux_2level_size50_47_sram_blwl_out[2425:2425] ,mux_2level_size50_47_sram_blwl_out[2425:2425] ,mux_2level_size50_47_sram_blwl_outb[2425:2425] ,mux_2level_size50_47_configbus0[2425:2425], mux_2level_size50_47_configbus1[2425:2425] , mux_2level_size50_47_configbus0_b[2425:2425] );
sram6T_blwl sram_blwl_2426_ (mux_2level_size50_47_sram_blwl_out[2426:2426] ,mux_2level_size50_47_sram_blwl_out[2426:2426] ,mux_2level_size50_47_sram_blwl_outb[2426:2426] ,mux_2level_size50_47_configbus0[2426:2426], mux_2level_size50_47_configbus1[2426:2426] , mux_2level_size50_47_configbus0_b[2426:2426] );
sram6T_blwl sram_blwl_2427_ (mux_2level_size50_47_sram_blwl_out[2427:2427] ,mux_2level_size50_47_sram_blwl_out[2427:2427] ,mux_2level_size50_47_sram_blwl_outb[2427:2427] ,mux_2level_size50_47_configbus0[2427:2427], mux_2level_size50_47_configbus1[2427:2427] , mux_2level_size50_47_configbus0_b[2427:2427] );
sram6T_blwl sram_blwl_2428_ (mux_2level_size50_47_sram_blwl_out[2428:2428] ,mux_2level_size50_47_sram_blwl_out[2428:2428] ,mux_2level_size50_47_sram_blwl_outb[2428:2428] ,mux_2level_size50_47_configbus0[2428:2428], mux_2level_size50_47_configbus1[2428:2428] , mux_2level_size50_47_configbus0_b[2428:2428] );
sram6T_blwl sram_blwl_2429_ (mux_2level_size50_47_sram_blwl_out[2429:2429] ,mux_2level_size50_47_sram_blwl_out[2429:2429] ,mux_2level_size50_47_sram_blwl_outb[2429:2429] ,mux_2level_size50_47_configbus0[2429:2429], mux_2level_size50_47_configbus1[2429:2429] , mux_2level_size50_47_configbus0_b[2429:2429] );
sram6T_blwl sram_blwl_2430_ (mux_2level_size50_47_sram_blwl_out[2430:2430] ,mux_2level_size50_47_sram_blwl_out[2430:2430] ,mux_2level_size50_47_sram_blwl_outb[2430:2430] ,mux_2level_size50_47_configbus0[2430:2430], mux_2level_size50_47_configbus1[2430:2430] , mux_2level_size50_47_configbus0_b[2430:2430] );
sram6T_blwl sram_blwl_2431_ (mux_2level_size50_47_sram_blwl_out[2431:2431] ,mux_2level_size50_47_sram_blwl_out[2431:2431] ,mux_2level_size50_47_sram_blwl_outb[2431:2431] ,mux_2level_size50_47_configbus0[2431:2431], mux_2level_size50_47_configbus1[2431:2431] , mux_2level_size50_47_configbus0_b[2431:2431] );
sram6T_blwl sram_blwl_2432_ (mux_2level_size50_47_sram_blwl_out[2432:2432] ,mux_2level_size50_47_sram_blwl_out[2432:2432] ,mux_2level_size50_47_sram_blwl_outb[2432:2432] ,mux_2level_size50_47_configbus0[2432:2432], mux_2level_size50_47_configbus1[2432:2432] , mux_2level_size50_47_configbus0_b[2432:2432] );
sram6T_blwl sram_blwl_2433_ (mux_2level_size50_47_sram_blwl_out[2433:2433] ,mux_2level_size50_47_sram_blwl_out[2433:2433] ,mux_2level_size50_47_sram_blwl_outb[2433:2433] ,mux_2level_size50_47_configbus0[2433:2433], mux_2level_size50_47_configbus1[2433:2433] , mux_2level_size50_47_configbus0_b[2433:2433] );
direct_interc direct_interc_177_ (mode_clb___clk_0_, fle_7___clk_0_ );
wire [0:49] in_bus_mux_2level_size50_48_ ;
assign in_bus_mux_2level_size50_48_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_48_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_48_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_48_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_48_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_48_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_48_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_48_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_48_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_48_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_48_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_48_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_48_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_48_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_48_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_48_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_48_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_48_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_48_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_48_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_48_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_48_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_48_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_48_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_48_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_48_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_48_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_48_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_48_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_48_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_48_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_48_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_48_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_48_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_48_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_48_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_48_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_48_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_48_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_48_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_48_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_48_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_48_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_48_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_48_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_48_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_48_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_48_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_48_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_48_[49] = fle_9___out_0_ ; 
wire [2434:2449] mux_2level_size50_48_configbus0;
wire [2434:2449] mux_2level_size50_48_configbus1;
wire [2434:2449] mux_2level_size50_48_sram_blwl_out ;
wire [2434:2449] mux_2level_size50_48_sram_blwl_outb ;
assign mux_2level_size50_48_configbus0[2434:2449] = sram_blwl_bl[2434:2449] ;
assign mux_2level_size50_48_configbus1[2434:2449] = sram_blwl_wl[2434:2449] ;
wire [2434:2449] mux_2level_size50_48_configbus0_b;
assign mux_2level_size50_48_configbus0_b[2434:2449] = sram_blwl_blb[2434:2449] ;
mux_2level_size50 mux_2level_size50_48_ (in_bus_mux_2level_size50_48_, fle_8___in_0_, mux_2level_size50_48_sram_blwl_out[2434:2449] ,
mux_2level_size50_48_sram_blwl_outb[2434:2449] );
//----- SRAM bits for MUX[48], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2434_ (mux_2level_size50_48_sram_blwl_out[2434:2434] ,mux_2level_size50_48_sram_blwl_out[2434:2434] ,mux_2level_size50_48_sram_blwl_outb[2434:2434] ,mux_2level_size50_48_configbus0[2434:2434], mux_2level_size50_48_configbus1[2434:2434] , mux_2level_size50_48_configbus0_b[2434:2434] );
sram6T_blwl sram_blwl_2435_ (mux_2level_size50_48_sram_blwl_out[2435:2435] ,mux_2level_size50_48_sram_blwl_out[2435:2435] ,mux_2level_size50_48_sram_blwl_outb[2435:2435] ,mux_2level_size50_48_configbus0[2435:2435], mux_2level_size50_48_configbus1[2435:2435] , mux_2level_size50_48_configbus0_b[2435:2435] );
sram6T_blwl sram_blwl_2436_ (mux_2level_size50_48_sram_blwl_out[2436:2436] ,mux_2level_size50_48_sram_blwl_out[2436:2436] ,mux_2level_size50_48_sram_blwl_outb[2436:2436] ,mux_2level_size50_48_configbus0[2436:2436], mux_2level_size50_48_configbus1[2436:2436] , mux_2level_size50_48_configbus0_b[2436:2436] );
sram6T_blwl sram_blwl_2437_ (mux_2level_size50_48_sram_blwl_out[2437:2437] ,mux_2level_size50_48_sram_blwl_out[2437:2437] ,mux_2level_size50_48_sram_blwl_outb[2437:2437] ,mux_2level_size50_48_configbus0[2437:2437], mux_2level_size50_48_configbus1[2437:2437] , mux_2level_size50_48_configbus0_b[2437:2437] );
sram6T_blwl sram_blwl_2438_ (mux_2level_size50_48_sram_blwl_out[2438:2438] ,mux_2level_size50_48_sram_blwl_out[2438:2438] ,mux_2level_size50_48_sram_blwl_outb[2438:2438] ,mux_2level_size50_48_configbus0[2438:2438], mux_2level_size50_48_configbus1[2438:2438] , mux_2level_size50_48_configbus0_b[2438:2438] );
sram6T_blwl sram_blwl_2439_ (mux_2level_size50_48_sram_blwl_out[2439:2439] ,mux_2level_size50_48_sram_blwl_out[2439:2439] ,mux_2level_size50_48_sram_blwl_outb[2439:2439] ,mux_2level_size50_48_configbus0[2439:2439], mux_2level_size50_48_configbus1[2439:2439] , mux_2level_size50_48_configbus0_b[2439:2439] );
sram6T_blwl sram_blwl_2440_ (mux_2level_size50_48_sram_blwl_out[2440:2440] ,mux_2level_size50_48_sram_blwl_out[2440:2440] ,mux_2level_size50_48_sram_blwl_outb[2440:2440] ,mux_2level_size50_48_configbus0[2440:2440], mux_2level_size50_48_configbus1[2440:2440] , mux_2level_size50_48_configbus0_b[2440:2440] );
sram6T_blwl sram_blwl_2441_ (mux_2level_size50_48_sram_blwl_out[2441:2441] ,mux_2level_size50_48_sram_blwl_out[2441:2441] ,mux_2level_size50_48_sram_blwl_outb[2441:2441] ,mux_2level_size50_48_configbus0[2441:2441], mux_2level_size50_48_configbus1[2441:2441] , mux_2level_size50_48_configbus0_b[2441:2441] );
sram6T_blwl sram_blwl_2442_ (mux_2level_size50_48_sram_blwl_out[2442:2442] ,mux_2level_size50_48_sram_blwl_out[2442:2442] ,mux_2level_size50_48_sram_blwl_outb[2442:2442] ,mux_2level_size50_48_configbus0[2442:2442], mux_2level_size50_48_configbus1[2442:2442] , mux_2level_size50_48_configbus0_b[2442:2442] );
sram6T_blwl sram_blwl_2443_ (mux_2level_size50_48_sram_blwl_out[2443:2443] ,mux_2level_size50_48_sram_blwl_out[2443:2443] ,mux_2level_size50_48_sram_blwl_outb[2443:2443] ,mux_2level_size50_48_configbus0[2443:2443], mux_2level_size50_48_configbus1[2443:2443] , mux_2level_size50_48_configbus0_b[2443:2443] );
sram6T_blwl sram_blwl_2444_ (mux_2level_size50_48_sram_blwl_out[2444:2444] ,mux_2level_size50_48_sram_blwl_out[2444:2444] ,mux_2level_size50_48_sram_blwl_outb[2444:2444] ,mux_2level_size50_48_configbus0[2444:2444], mux_2level_size50_48_configbus1[2444:2444] , mux_2level_size50_48_configbus0_b[2444:2444] );
sram6T_blwl sram_blwl_2445_ (mux_2level_size50_48_sram_blwl_out[2445:2445] ,mux_2level_size50_48_sram_blwl_out[2445:2445] ,mux_2level_size50_48_sram_blwl_outb[2445:2445] ,mux_2level_size50_48_configbus0[2445:2445], mux_2level_size50_48_configbus1[2445:2445] , mux_2level_size50_48_configbus0_b[2445:2445] );
sram6T_blwl sram_blwl_2446_ (mux_2level_size50_48_sram_blwl_out[2446:2446] ,mux_2level_size50_48_sram_blwl_out[2446:2446] ,mux_2level_size50_48_sram_blwl_outb[2446:2446] ,mux_2level_size50_48_configbus0[2446:2446], mux_2level_size50_48_configbus1[2446:2446] , mux_2level_size50_48_configbus0_b[2446:2446] );
sram6T_blwl sram_blwl_2447_ (mux_2level_size50_48_sram_blwl_out[2447:2447] ,mux_2level_size50_48_sram_blwl_out[2447:2447] ,mux_2level_size50_48_sram_blwl_outb[2447:2447] ,mux_2level_size50_48_configbus0[2447:2447], mux_2level_size50_48_configbus1[2447:2447] , mux_2level_size50_48_configbus0_b[2447:2447] );
sram6T_blwl sram_blwl_2448_ (mux_2level_size50_48_sram_blwl_out[2448:2448] ,mux_2level_size50_48_sram_blwl_out[2448:2448] ,mux_2level_size50_48_sram_blwl_outb[2448:2448] ,mux_2level_size50_48_configbus0[2448:2448], mux_2level_size50_48_configbus1[2448:2448] , mux_2level_size50_48_configbus0_b[2448:2448] );
sram6T_blwl sram_blwl_2449_ (mux_2level_size50_48_sram_blwl_out[2449:2449] ,mux_2level_size50_48_sram_blwl_out[2449:2449] ,mux_2level_size50_48_sram_blwl_outb[2449:2449] ,mux_2level_size50_48_configbus0[2449:2449], mux_2level_size50_48_configbus1[2449:2449] , mux_2level_size50_48_configbus0_b[2449:2449] );
wire [0:49] in_bus_mux_2level_size50_49_ ;
assign in_bus_mux_2level_size50_49_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_49_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_49_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_49_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_49_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_49_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_49_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_49_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_49_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_49_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_49_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_49_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_49_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_49_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_49_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_49_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_49_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_49_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_49_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_49_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_49_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_49_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_49_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_49_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_49_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_49_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_49_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_49_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_49_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_49_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_49_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_49_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_49_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_49_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_49_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_49_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_49_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_49_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_49_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_49_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_49_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_49_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_49_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_49_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_49_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_49_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_49_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_49_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_49_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_49_[49] = fle_9___out_0_ ; 
wire [2450:2465] mux_2level_size50_49_configbus0;
wire [2450:2465] mux_2level_size50_49_configbus1;
wire [2450:2465] mux_2level_size50_49_sram_blwl_out ;
wire [2450:2465] mux_2level_size50_49_sram_blwl_outb ;
assign mux_2level_size50_49_configbus0[2450:2465] = sram_blwl_bl[2450:2465] ;
assign mux_2level_size50_49_configbus1[2450:2465] = sram_blwl_wl[2450:2465] ;
wire [2450:2465] mux_2level_size50_49_configbus0_b;
assign mux_2level_size50_49_configbus0_b[2450:2465] = sram_blwl_blb[2450:2465] ;
mux_2level_size50 mux_2level_size50_49_ (in_bus_mux_2level_size50_49_, fle_8___in_1_, mux_2level_size50_49_sram_blwl_out[2450:2465] ,
mux_2level_size50_49_sram_blwl_outb[2450:2465] );
//----- SRAM bits for MUX[49], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2450_ (mux_2level_size50_49_sram_blwl_out[2450:2450] ,mux_2level_size50_49_sram_blwl_out[2450:2450] ,mux_2level_size50_49_sram_blwl_outb[2450:2450] ,mux_2level_size50_49_configbus0[2450:2450], mux_2level_size50_49_configbus1[2450:2450] , mux_2level_size50_49_configbus0_b[2450:2450] );
sram6T_blwl sram_blwl_2451_ (mux_2level_size50_49_sram_blwl_out[2451:2451] ,mux_2level_size50_49_sram_blwl_out[2451:2451] ,mux_2level_size50_49_sram_blwl_outb[2451:2451] ,mux_2level_size50_49_configbus0[2451:2451], mux_2level_size50_49_configbus1[2451:2451] , mux_2level_size50_49_configbus0_b[2451:2451] );
sram6T_blwl sram_blwl_2452_ (mux_2level_size50_49_sram_blwl_out[2452:2452] ,mux_2level_size50_49_sram_blwl_out[2452:2452] ,mux_2level_size50_49_sram_blwl_outb[2452:2452] ,mux_2level_size50_49_configbus0[2452:2452], mux_2level_size50_49_configbus1[2452:2452] , mux_2level_size50_49_configbus0_b[2452:2452] );
sram6T_blwl sram_blwl_2453_ (mux_2level_size50_49_sram_blwl_out[2453:2453] ,mux_2level_size50_49_sram_blwl_out[2453:2453] ,mux_2level_size50_49_sram_blwl_outb[2453:2453] ,mux_2level_size50_49_configbus0[2453:2453], mux_2level_size50_49_configbus1[2453:2453] , mux_2level_size50_49_configbus0_b[2453:2453] );
sram6T_blwl sram_blwl_2454_ (mux_2level_size50_49_sram_blwl_out[2454:2454] ,mux_2level_size50_49_sram_blwl_out[2454:2454] ,mux_2level_size50_49_sram_blwl_outb[2454:2454] ,mux_2level_size50_49_configbus0[2454:2454], mux_2level_size50_49_configbus1[2454:2454] , mux_2level_size50_49_configbus0_b[2454:2454] );
sram6T_blwl sram_blwl_2455_ (mux_2level_size50_49_sram_blwl_out[2455:2455] ,mux_2level_size50_49_sram_blwl_out[2455:2455] ,mux_2level_size50_49_sram_blwl_outb[2455:2455] ,mux_2level_size50_49_configbus0[2455:2455], mux_2level_size50_49_configbus1[2455:2455] , mux_2level_size50_49_configbus0_b[2455:2455] );
sram6T_blwl sram_blwl_2456_ (mux_2level_size50_49_sram_blwl_out[2456:2456] ,mux_2level_size50_49_sram_blwl_out[2456:2456] ,mux_2level_size50_49_sram_blwl_outb[2456:2456] ,mux_2level_size50_49_configbus0[2456:2456], mux_2level_size50_49_configbus1[2456:2456] , mux_2level_size50_49_configbus0_b[2456:2456] );
sram6T_blwl sram_blwl_2457_ (mux_2level_size50_49_sram_blwl_out[2457:2457] ,mux_2level_size50_49_sram_blwl_out[2457:2457] ,mux_2level_size50_49_sram_blwl_outb[2457:2457] ,mux_2level_size50_49_configbus0[2457:2457], mux_2level_size50_49_configbus1[2457:2457] , mux_2level_size50_49_configbus0_b[2457:2457] );
sram6T_blwl sram_blwl_2458_ (mux_2level_size50_49_sram_blwl_out[2458:2458] ,mux_2level_size50_49_sram_blwl_out[2458:2458] ,mux_2level_size50_49_sram_blwl_outb[2458:2458] ,mux_2level_size50_49_configbus0[2458:2458], mux_2level_size50_49_configbus1[2458:2458] , mux_2level_size50_49_configbus0_b[2458:2458] );
sram6T_blwl sram_blwl_2459_ (mux_2level_size50_49_sram_blwl_out[2459:2459] ,mux_2level_size50_49_sram_blwl_out[2459:2459] ,mux_2level_size50_49_sram_blwl_outb[2459:2459] ,mux_2level_size50_49_configbus0[2459:2459], mux_2level_size50_49_configbus1[2459:2459] , mux_2level_size50_49_configbus0_b[2459:2459] );
sram6T_blwl sram_blwl_2460_ (mux_2level_size50_49_sram_blwl_out[2460:2460] ,mux_2level_size50_49_sram_blwl_out[2460:2460] ,mux_2level_size50_49_sram_blwl_outb[2460:2460] ,mux_2level_size50_49_configbus0[2460:2460], mux_2level_size50_49_configbus1[2460:2460] , mux_2level_size50_49_configbus0_b[2460:2460] );
sram6T_blwl sram_blwl_2461_ (mux_2level_size50_49_sram_blwl_out[2461:2461] ,mux_2level_size50_49_sram_blwl_out[2461:2461] ,mux_2level_size50_49_sram_blwl_outb[2461:2461] ,mux_2level_size50_49_configbus0[2461:2461], mux_2level_size50_49_configbus1[2461:2461] , mux_2level_size50_49_configbus0_b[2461:2461] );
sram6T_blwl sram_blwl_2462_ (mux_2level_size50_49_sram_blwl_out[2462:2462] ,mux_2level_size50_49_sram_blwl_out[2462:2462] ,mux_2level_size50_49_sram_blwl_outb[2462:2462] ,mux_2level_size50_49_configbus0[2462:2462], mux_2level_size50_49_configbus1[2462:2462] , mux_2level_size50_49_configbus0_b[2462:2462] );
sram6T_blwl sram_blwl_2463_ (mux_2level_size50_49_sram_blwl_out[2463:2463] ,mux_2level_size50_49_sram_blwl_out[2463:2463] ,mux_2level_size50_49_sram_blwl_outb[2463:2463] ,mux_2level_size50_49_configbus0[2463:2463], mux_2level_size50_49_configbus1[2463:2463] , mux_2level_size50_49_configbus0_b[2463:2463] );
sram6T_blwl sram_blwl_2464_ (mux_2level_size50_49_sram_blwl_out[2464:2464] ,mux_2level_size50_49_sram_blwl_out[2464:2464] ,mux_2level_size50_49_sram_blwl_outb[2464:2464] ,mux_2level_size50_49_configbus0[2464:2464], mux_2level_size50_49_configbus1[2464:2464] , mux_2level_size50_49_configbus0_b[2464:2464] );
sram6T_blwl sram_blwl_2465_ (mux_2level_size50_49_sram_blwl_out[2465:2465] ,mux_2level_size50_49_sram_blwl_out[2465:2465] ,mux_2level_size50_49_sram_blwl_outb[2465:2465] ,mux_2level_size50_49_configbus0[2465:2465], mux_2level_size50_49_configbus1[2465:2465] , mux_2level_size50_49_configbus0_b[2465:2465] );
wire [0:49] in_bus_mux_2level_size50_50_ ;
assign in_bus_mux_2level_size50_50_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_50_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_50_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_50_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_50_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_50_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_50_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_50_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_50_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_50_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_50_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_50_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_50_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_50_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_50_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_50_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_50_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_50_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_50_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_50_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_50_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_50_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_50_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_50_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_50_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_50_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_50_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_50_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_50_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_50_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_50_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_50_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_50_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_50_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_50_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_50_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_50_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_50_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_50_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_50_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_50_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_50_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_50_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_50_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_50_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_50_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_50_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_50_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_50_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_50_[49] = fle_9___out_0_ ; 
wire [2466:2481] mux_2level_size50_50_configbus0;
wire [2466:2481] mux_2level_size50_50_configbus1;
wire [2466:2481] mux_2level_size50_50_sram_blwl_out ;
wire [2466:2481] mux_2level_size50_50_sram_blwl_outb ;
assign mux_2level_size50_50_configbus0[2466:2481] = sram_blwl_bl[2466:2481] ;
assign mux_2level_size50_50_configbus1[2466:2481] = sram_blwl_wl[2466:2481] ;
wire [2466:2481] mux_2level_size50_50_configbus0_b;
assign mux_2level_size50_50_configbus0_b[2466:2481] = sram_blwl_blb[2466:2481] ;
mux_2level_size50 mux_2level_size50_50_ (in_bus_mux_2level_size50_50_, fle_8___in_2_, mux_2level_size50_50_sram_blwl_out[2466:2481] ,
mux_2level_size50_50_sram_blwl_outb[2466:2481] );
//----- SRAM bits for MUX[50], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2466_ (mux_2level_size50_50_sram_blwl_out[2466:2466] ,mux_2level_size50_50_sram_blwl_out[2466:2466] ,mux_2level_size50_50_sram_blwl_outb[2466:2466] ,mux_2level_size50_50_configbus0[2466:2466], mux_2level_size50_50_configbus1[2466:2466] , mux_2level_size50_50_configbus0_b[2466:2466] );
sram6T_blwl sram_blwl_2467_ (mux_2level_size50_50_sram_blwl_out[2467:2467] ,mux_2level_size50_50_sram_blwl_out[2467:2467] ,mux_2level_size50_50_sram_blwl_outb[2467:2467] ,mux_2level_size50_50_configbus0[2467:2467], mux_2level_size50_50_configbus1[2467:2467] , mux_2level_size50_50_configbus0_b[2467:2467] );
sram6T_blwl sram_blwl_2468_ (mux_2level_size50_50_sram_blwl_out[2468:2468] ,mux_2level_size50_50_sram_blwl_out[2468:2468] ,mux_2level_size50_50_sram_blwl_outb[2468:2468] ,mux_2level_size50_50_configbus0[2468:2468], mux_2level_size50_50_configbus1[2468:2468] , mux_2level_size50_50_configbus0_b[2468:2468] );
sram6T_blwl sram_blwl_2469_ (mux_2level_size50_50_sram_blwl_out[2469:2469] ,mux_2level_size50_50_sram_blwl_out[2469:2469] ,mux_2level_size50_50_sram_blwl_outb[2469:2469] ,mux_2level_size50_50_configbus0[2469:2469], mux_2level_size50_50_configbus1[2469:2469] , mux_2level_size50_50_configbus0_b[2469:2469] );
sram6T_blwl sram_blwl_2470_ (mux_2level_size50_50_sram_blwl_out[2470:2470] ,mux_2level_size50_50_sram_blwl_out[2470:2470] ,mux_2level_size50_50_sram_blwl_outb[2470:2470] ,mux_2level_size50_50_configbus0[2470:2470], mux_2level_size50_50_configbus1[2470:2470] , mux_2level_size50_50_configbus0_b[2470:2470] );
sram6T_blwl sram_blwl_2471_ (mux_2level_size50_50_sram_blwl_out[2471:2471] ,mux_2level_size50_50_sram_blwl_out[2471:2471] ,mux_2level_size50_50_sram_blwl_outb[2471:2471] ,mux_2level_size50_50_configbus0[2471:2471], mux_2level_size50_50_configbus1[2471:2471] , mux_2level_size50_50_configbus0_b[2471:2471] );
sram6T_blwl sram_blwl_2472_ (mux_2level_size50_50_sram_blwl_out[2472:2472] ,mux_2level_size50_50_sram_blwl_out[2472:2472] ,mux_2level_size50_50_sram_blwl_outb[2472:2472] ,mux_2level_size50_50_configbus0[2472:2472], mux_2level_size50_50_configbus1[2472:2472] , mux_2level_size50_50_configbus0_b[2472:2472] );
sram6T_blwl sram_blwl_2473_ (mux_2level_size50_50_sram_blwl_out[2473:2473] ,mux_2level_size50_50_sram_blwl_out[2473:2473] ,mux_2level_size50_50_sram_blwl_outb[2473:2473] ,mux_2level_size50_50_configbus0[2473:2473], mux_2level_size50_50_configbus1[2473:2473] , mux_2level_size50_50_configbus0_b[2473:2473] );
sram6T_blwl sram_blwl_2474_ (mux_2level_size50_50_sram_blwl_out[2474:2474] ,mux_2level_size50_50_sram_blwl_out[2474:2474] ,mux_2level_size50_50_sram_blwl_outb[2474:2474] ,mux_2level_size50_50_configbus0[2474:2474], mux_2level_size50_50_configbus1[2474:2474] , mux_2level_size50_50_configbus0_b[2474:2474] );
sram6T_blwl sram_blwl_2475_ (mux_2level_size50_50_sram_blwl_out[2475:2475] ,mux_2level_size50_50_sram_blwl_out[2475:2475] ,mux_2level_size50_50_sram_blwl_outb[2475:2475] ,mux_2level_size50_50_configbus0[2475:2475], mux_2level_size50_50_configbus1[2475:2475] , mux_2level_size50_50_configbus0_b[2475:2475] );
sram6T_blwl sram_blwl_2476_ (mux_2level_size50_50_sram_blwl_out[2476:2476] ,mux_2level_size50_50_sram_blwl_out[2476:2476] ,mux_2level_size50_50_sram_blwl_outb[2476:2476] ,mux_2level_size50_50_configbus0[2476:2476], mux_2level_size50_50_configbus1[2476:2476] , mux_2level_size50_50_configbus0_b[2476:2476] );
sram6T_blwl sram_blwl_2477_ (mux_2level_size50_50_sram_blwl_out[2477:2477] ,mux_2level_size50_50_sram_blwl_out[2477:2477] ,mux_2level_size50_50_sram_blwl_outb[2477:2477] ,mux_2level_size50_50_configbus0[2477:2477], mux_2level_size50_50_configbus1[2477:2477] , mux_2level_size50_50_configbus0_b[2477:2477] );
sram6T_blwl sram_blwl_2478_ (mux_2level_size50_50_sram_blwl_out[2478:2478] ,mux_2level_size50_50_sram_blwl_out[2478:2478] ,mux_2level_size50_50_sram_blwl_outb[2478:2478] ,mux_2level_size50_50_configbus0[2478:2478], mux_2level_size50_50_configbus1[2478:2478] , mux_2level_size50_50_configbus0_b[2478:2478] );
sram6T_blwl sram_blwl_2479_ (mux_2level_size50_50_sram_blwl_out[2479:2479] ,mux_2level_size50_50_sram_blwl_out[2479:2479] ,mux_2level_size50_50_sram_blwl_outb[2479:2479] ,mux_2level_size50_50_configbus0[2479:2479], mux_2level_size50_50_configbus1[2479:2479] , mux_2level_size50_50_configbus0_b[2479:2479] );
sram6T_blwl sram_blwl_2480_ (mux_2level_size50_50_sram_blwl_out[2480:2480] ,mux_2level_size50_50_sram_blwl_out[2480:2480] ,mux_2level_size50_50_sram_blwl_outb[2480:2480] ,mux_2level_size50_50_configbus0[2480:2480], mux_2level_size50_50_configbus1[2480:2480] , mux_2level_size50_50_configbus0_b[2480:2480] );
sram6T_blwl sram_blwl_2481_ (mux_2level_size50_50_sram_blwl_out[2481:2481] ,mux_2level_size50_50_sram_blwl_out[2481:2481] ,mux_2level_size50_50_sram_blwl_outb[2481:2481] ,mux_2level_size50_50_configbus0[2481:2481], mux_2level_size50_50_configbus1[2481:2481] , mux_2level_size50_50_configbus0_b[2481:2481] );
wire [0:49] in_bus_mux_2level_size50_51_ ;
assign in_bus_mux_2level_size50_51_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_51_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_51_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_51_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_51_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_51_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_51_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_51_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_51_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_51_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_51_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_51_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_51_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_51_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_51_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_51_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_51_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_51_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_51_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_51_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_51_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_51_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_51_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_51_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_51_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_51_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_51_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_51_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_51_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_51_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_51_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_51_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_51_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_51_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_51_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_51_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_51_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_51_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_51_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_51_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_51_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_51_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_51_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_51_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_51_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_51_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_51_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_51_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_51_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_51_[49] = fle_9___out_0_ ; 
wire [2482:2497] mux_2level_size50_51_configbus0;
wire [2482:2497] mux_2level_size50_51_configbus1;
wire [2482:2497] mux_2level_size50_51_sram_blwl_out ;
wire [2482:2497] mux_2level_size50_51_sram_blwl_outb ;
assign mux_2level_size50_51_configbus0[2482:2497] = sram_blwl_bl[2482:2497] ;
assign mux_2level_size50_51_configbus1[2482:2497] = sram_blwl_wl[2482:2497] ;
wire [2482:2497] mux_2level_size50_51_configbus0_b;
assign mux_2level_size50_51_configbus0_b[2482:2497] = sram_blwl_blb[2482:2497] ;
mux_2level_size50 mux_2level_size50_51_ (in_bus_mux_2level_size50_51_, fle_8___in_3_, mux_2level_size50_51_sram_blwl_out[2482:2497] ,
mux_2level_size50_51_sram_blwl_outb[2482:2497] );
//----- SRAM bits for MUX[51], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2482_ (mux_2level_size50_51_sram_blwl_out[2482:2482] ,mux_2level_size50_51_sram_blwl_out[2482:2482] ,mux_2level_size50_51_sram_blwl_outb[2482:2482] ,mux_2level_size50_51_configbus0[2482:2482], mux_2level_size50_51_configbus1[2482:2482] , mux_2level_size50_51_configbus0_b[2482:2482] );
sram6T_blwl sram_blwl_2483_ (mux_2level_size50_51_sram_blwl_out[2483:2483] ,mux_2level_size50_51_sram_blwl_out[2483:2483] ,mux_2level_size50_51_sram_blwl_outb[2483:2483] ,mux_2level_size50_51_configbus0[2483:2483], mux_2level_size50_51_configbus1[2483:2483] , mux_2level_size50_51_configbus0_b[2483:2483] );
sram6T_blwl sram_blwl_2484_ (mux_2level_size50_51_sram_blwl_out[2484:2484] ,mux_2level_size50_51_sram_blwl_out[2484:2484] ,mux_2level_size50_51_sram_blwl_outb[2484:2484] ,mux_2level_size50_51_configbus0[2484:2484], mux_2level_size50_51_configbus1[2484:2484] , mux_2level_size50_51_configbus0_b[2484:2484] );
sram6T_blwl sram_blwl_2485_ (mux_2level_size50_51_sram_blwl_out[2485:2485] ,mux_2level_size50_51_sram_blwl_out[2485:2485] ,mux_2level_size50_51_sram_blwl_outb[2485:2485] ,mux_2level_size50_51_configbus0[2485:2485], mux_2level_size50_51_configbus1[2485:2485] , mux_2level_size50_51_configbus0_b[2485:2485] );
sram6T_blwl sram_blwl_2486_ (mux_2level_size50_51_sram_blwl_out[2486:2486] ,mux_2level_size50_51_sram_blwl_out[2486:2486] ,mux_2level_size50_51_sram_blwl_outb[2486:2486] ,mux_2level_size50_51_configbus0[2486:2486], mux_2level_size50_51_configbus1[2486:2486] , mux_2level_size50_51_configbus0_b[2486:2486] );
sram6T_blwl sram_blwl_2487_ (mux_2level_size50_51_sram_blwl_out[2487:2487] ,mux_2level_size50_51_sram_blwl_out[2487:2487] ,mux_2level_size50_51_sram_blwl_outb[2487:2487] ,mux_2level_size50_51_configbus0[2487:2487], mux_2level_size50_51_configbus1[2487:2487] , mux_2level_size50_51_configbus0_b[2487:2487] );
sram6T_blwl sram_blwl_2488_ (mux_2level_size50_51_sram_blwl_out[2488:2488] ,mux_2level_size50_51_sram_blwl_out[2488:2488] ,mux_2level_size50_51_sram_blwl_outb[2488:2488] ,mux_2level_size50_51_configbus0[2488:2488], mux_2level_size50_51_configbus1[2488:2488] , mux_2level_size50_51_configbus0_b[2488:2488] );
sram6T_blwl sram_blwl_2489_ (mux_2level_size50_51_sram_blwl_out[2489:2489] ,mux_2level_size50_51_sram_blwl_out[2489:2489] ,mux_2level_size50_51_sram_blwl_outb[2489:2489] ,mux_2level_size50_51_configbus0[2489:2489], mux_2level_size50_51_configbus1[2489:2489] , mux_2level_size50_51_configbus0_b[2489:2489] );
sram6T_blwl sram_blwl_2490_ (mux_2level_size50_51_sram_blwl_out[2490:2490] ,mux_2level_size50_51_sram_blwl_out[2490:2490] ,mux_2level_size50_51_sram_blwl_outb[2490:2490] ,mux_2level_size50_51_configbus0[2490:2490], mux_2level_size50_51_configbus1[2490:2490] , mux_2level_size50_51_configbus0_b[2490:2490] );
sram6T_blwl sram_blwl_2491_ (mux_2level_size50_51_sram_blwl_out[2491:2491] ,mux_2level_size50_51_sram_blwl_out[2491:2491] ,mux_2level_size50_51_sram_blwl_outb[2491:2491] ,mux_2level_size50_51_configbus0[2491:2491], mux_2level_size50_51_configbus1[2491:2491] , mux_2level_size50_51_configbus0_b[2491:2491] );
sram6T_blwl sram_blwl_2492_ (mux_2level_size50_51_sram_blwl_out[2492:2492] ,mux_2level_size50_51_sram_blwl_out[2492:2492] ,mux_2level_size50_51_sram_blwl_outb[2492:2492] ,mux_2level_size50_51_configbus0[2492:2492], mux_2level_size50_51_configbus1[2492:2492] , mux_2level_size50_51_configbus0_b[2492:2492] );
sram6T_blwl sram_blwl_2493_ (mux_2level_size50_51_sram_blwl_out[2493:2493] ,mux_2level_size50_51_sram_blwl_out[2493:2493] ,mux_2level_size50_51_sram_blwl_outb[2493:2493] ,mux_2level_size50_51_configbus0[2493:2493], mux_2level_size50_51_configbus1[2493:2493] , mux_2level_size50_51_configbus0_b[2493:2493] );
sram6T_blwl sram_blwl_2494_ (mux_2level_size50_51_sram_blwl_out[2494:2494] ,mux_2level_size50_51_sram_blwl_out[2494:2494] ,mux_2level_size50_51_sram_blwl_outb[2494:2494] ,mux_2level_size50_51_configbus0[2494:2494], mux_2level_size50_51_configbus1[2494:2494] , mux_2level_size50_51_configbus0_b[2494:2494] );
sram6T_blwl sram_blwl_2495_ (mux_2level_size50_51_sram_blwl_out[2495:2495] ,mux_2level_size50_51_sram_blwl_out[2495:2495] ,mux_2level_size50_51_sram_blwl_outb[2495:2495] ,mux_2level_size50_51_configbus0[2495:2495], mux_2level_size50_51_configbus1[2495:2495] , mux_2level_size50_51_configbus0_b[2495:2495] );
sram6T_blwl sram_blwl_2496_ (mux_2level_size50_51_sram_blwl_out[2496:2496] ,mux_2level_size50_51_sram_blwl_out[2496:2496] ,mux_2level_size50_51_sram_blwl_outb[2496:2496] ,mux_2level_size50_51_configbus0[2496:2496], mux_2level_size50_51_configbus1[2496:2496] , mux_2level_size50_51_configbus0_b[2496:2496] );
sram6T_blwl sram_blwl_2497_ (mux_2level_size50_51_sram_blwl_out[2497:2497] ,mux_2level_size50_51_sram_blwl_out[2497:2497] ,mux_2level_size50_51_sram_blwl_outb[2497:2497] ,mux_2level_size50_51_configbus0[2497:2497], mux_2level_size50_51_configbus1[2497:2497] , mux_2level_size50_51_configbus0_b[2497:2497] );
wire [0:49] in_bus_mux_2level_size50_52_ ;
assign in_bus_mux_2level_size50_52_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_52_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_52_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_52_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_52_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_52_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_52_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_52_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_52_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_52_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_52_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_52_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_52_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_52_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_52_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_52_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_52_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_52_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_52_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_52_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_52_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_52_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_52_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_52_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_52_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_52_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_52_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_52_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_52_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_52_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_52_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_52_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_52_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_52_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_52_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_52_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_52_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_52_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_52_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_52_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_52_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_52_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_52_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_52_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_52_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_52_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_52_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_52_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_52_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_52_[49] = fle_9___out_0_ ; 
wire [2498:2513] mux_2level_size50_52_configbus0;
wire [2498:2513] mux_2level_size50_52_configbus1;
wire [2498:2513] mux_2level_size50_52_sram_blwl_out ;
wire [2498:2513] mux_2level_size50_52_sram_blwl_outb ;
assign mux_2level_size50_52_configbus0[2498:2513] = sram_blwl_bl[2498:2513] ;
assign mux_2level_size50_52_configbus1[2498:2513] = sram_blwl_wl[2498:2513] ;
wire [2498:2513] mux_2level_size50_52_configbus0_b;
assign mux_2level_size50_52_configbus0_b[2498:2513] = sram_blwl_blb[2498:2513] ;
mux_2level_size50 mux_2level_size50_52_ (in_bus_mux_2level_size50_52_, fle_8___in_4_, mux_2level_size50_52_sram_blwl_out[2498:2513] ,
mux_2level_size50_52_sram_blwl_outb[2498:2513] );
//----- SRAM bits for MUX[52], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2498_ (mux_2level_size50_52_sram_blwl_out[2498:2498] ,mux_2level_size50_52_sram_blwl_out[2498:2498] ,mux_2level_size50_52_sram_blwl_outb[2498:2498] ,mux_2level_size50_52_configbus0[2498:2498], mux_2level_size50_52_configbus1[2498:2498] , mux_2level_size50_52_configbus0_b[2498:2498] );
sram6T_blwl sram_blwl_2499_ (mux_2level_size50_52_sram_blwl_out[2499:2499] ,mux_2level_size50_52_sram_blwl_out[2499:2499] ,mux_2level_size50_52_sram_blwl_outb[2499:2499] ,mux_2level_size50_52_configbus0[2499:2499], mux_2level_size50_52_configbus1[2499:2499] , mux_2level_size50_52_configbus0_b[2499:2499] );
sram6T_blwl sram_blwl_2500_ (mux_2level_size50_52_sram_blwl_out[2500:2500] ,mux_2level_size50_52_sram_blwl_out[2500:2500] ,mux_2level_size50_52_sram_blwl_outb[2500:2500] ,mux_2level_size50_52_configbus0[2500:2500], mux_2level_size50_52_configbus1[2500:2500] , mux_2level_size50_52_configbus0_b[2500:2500] );
sram6T_blwl sram_blwl_2501_ (mux_2level_size50_52_sram_blwl_out[2501:2501] ,mux_2level_size50_52_sram_blwl_out[2501:2501] ,mux_2level_size50_52_sram_blwl_outb[2501:2501] ,mux_2level_size50_52_configbus0[2501:2501], mux_2level_size50_52_configbus1[2501:2501] , mux_2level_size50_52_configbus0_b[2501:2501] );
sram6T_blwl sram_blwl_2502_ (mux_2level_size50_52_sram_blwl_out[2502:2502] ,mux_2level_size50_52_sram_blwl_out[2502:2502] ,mux_2level_size50_52_sram_blwl_outb[2502:2502] ,mux_2level_size50_52_configbus0[2502:2502], mux_2level_size50_52_configbus1[2502:2502] , mux_2level_size50_52_configbus0_b[2502:2502] );
sram6T_blwl sram_blwl_2503_ (mux_2level_size50_52_sram_blwl_out[2503:2503] ,mux_2level_size50_52_sram_blwl_out[2503:2503] ,mux_2level_size50_52_sram_blwl_outb[2503:2503] ,mux_2level_size50_52_configbus0[2503:2503], mux_2level_size50_52_configbus1[2503:2503] , mux_2level_size50_52_configbus0_b[2503:2503] );
sram6T_blwl sram_blwl_2504_ (mux_2level_size50_52_sram_blwl_out[2504:2504] ,mux_2level_size50_52_sram_blwl_out[2504:2504] ,mux_2level_size50_52_sram_blwl_outb[2504:2504] ,mux_2level_size50_52_configbus0[2504:2504], mux_2level_size50_52_configbus1[2504:2504] , mux_2level_size50_52_configbus0_b[2504:2504] );
sram6T_blwl sram_blwl_2505_ (mux_2level_size50_52_sram_blwl_out[2505:2505] ,mux_2level_size50_52_sram_blwl_out[2505:2505] ,mux_2level_size50_52_sram_blwl_outb[2505:2505] ,mux_2level_size50_52_configbus0[2505:2505], mux_2level_size50_52_configbus1[2505:2505] , mux_2level_size50_52_configbus0_b[2505:2505] );
sram6T_blwl sram_blwl_2506_ (mux_2level_size50_52_sram_blwl_out[2506:2506] ,mux_2level_size50_52_sram_blwl_out[2506:2506] ,mux_2level_size50_52_sram_blwl_outb[2506:2506] ,mux_2level_size50_52_configbus0[2506:2506], mux_2level_size50_52_configbus1[2506:2506] , mux_2level_size50_52_configbus0_b[2506:2506] );
sram6T_blwl sram_blwl_2507_ (mux_2level_size50_52_sram_blwl_out[2507:2507] ,mux_2level_size50_52_sram_blwl_out[2507:2507] ,mux_2level_size50_52_sram_blwl_outb[2507:2507] ,mux_2level_size50_52_configbus0[2507:2507], mux_2level_size50_52_configbus1[2507:2507] , mux_2level_size50_52_configbus0_b[2507:2507] );
sram6T_blwl sram_blwl_2508_ (mux_2level_size50_52_sram_blwl_out[2508:2508] ,mux_2level_size50_52_sram_blwl_out[2508:2508] ,mux_2level_size50_52_sram_blwl_outb[2508:2508] ,mux_2level_size50_52_configbus0[2508:2508], mux_2level_size50_52_configbus1[2508:2508] , mux_2level_size50_52_configbus0_b[2508:2508] );
sram6T_blwl sram_blwl_2509_ (mux_2level_size50_52_sram_blwl_out[2509:2509] ,mux_2level_size50_52_sram_blwl_out[2509:2509] ,mux_2level_size50_52_sram_blwl_outb[2509:2509] ,mux_2level_size50_52_configbus0[2509:2509], mux_2level_size50_52_configbus1[2509:2509] , mux_2level_size50_52_configbus0_b[2509:2509] );
sram6T_blwl sram_blwl_2510_ (mux_2level_size50_52_sram_blwl_out[2510:2510] ,mux_2level_size50_52_sram_blwl_out[2510:2510] ,mux_2level_size50_52_sram_blwl_outb[2510:2510] ,mux_2level_size50_52_configbus0[2510:2510], mux_2level_size50_52_configbus1[2510:2510] , mux_2level_size50_52_configbus0_b[2510:2510] );
sram6T_blwl sram_blwl_2511_ (mux_2level_size50_52_sram_blwl_out[2511:2511] ,mux_2level_size50_52_sram_blwl_out[2511:2511] ,mux_2level_size50_52_sram_blwl_outb[2511:2511] ,mux_2level_size50_52_configbus0[2511:2511], mux_2level_size50_52_configbus1[2511:2511] , mux_2level_size50_52_configbus0_b[2511:2511] );
sram6T_blwl sram_blwl_2512_ (mux_2level_size50_52_sram_blwl_out[2512:2512] ,mux_2level_size50_52_sram_blwl_out[2512:2512] ,mux_2level_size50_52_sram_blwl_outb[2512:2512] ,mux_2level_size50_52_configbus0[2512:2512], mux_2level_size50_52_configbus1[2512:2512] , mux_2level_size50_52_configbus0_b[2512:2512] );
sram6T_blwl sram_blwl_2513_ (mux_2level_size50_52_sram_blwl_out[2513:2513] ,mux_2level_size50_52_sram_blwl_out[2513:2513] ,mux_2level_size50_52_sram_blwl_outb[2513:2513] ,mux_2level_size50_52_configbus0[2513:2513], mux_2level_size50_52_configbus1[2513:2513] , mux_2level_size50_52_configbus0_b[2513:2513] );
wire [0:49] in_bus_mux_2level_size50_53_ ;
assign in_bus_mux_2level_size50_53_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_53_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_53_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_53_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_53_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_53_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_53_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_53_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_53_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_53_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_53_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_53_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_53_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_53_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_53_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_53_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_53_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_53_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_53_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_53_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_53_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_53_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_53_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_53_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_53_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_53_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_53_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_53_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_53_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_53_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_53_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_53_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_53_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_53_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_53_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_53_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_53_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_53_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_53_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_53_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_53_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_53_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_53_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_53_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_53_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_53_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_53_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_53_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_53_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_53_[49] = fle_9___out_0_ ; 
wire [2514:2529] mux_2level_size50_53_configbus0;
wire [2514:2529] mux_2level_size50_53_configbus1;
wire [2514:2529] mux_2level_size50_53_sram_blwl_out ;
wire [2514:2529] mux_2level_size50_53_sram_blwl_outb ;
assign mux_2level_size50_53_configbus0[2514:2529] = sram_blwl_bl[2514:2529] ;
assign mux_2level_size50_53_configbus1[2514:2529] = sram_blwl_wl[2514:2529] ;
wire [2514:2529] mux_2level_size50_53_configbus0_b;
assign mux_2level_size50_53_configbus0_b[2514:2529] = sram_blwl_blb[2514:2529] ;
mux_2level_size50 mux_2level_size50_53_ (in_bus_mux_2level_size50_53_, fle_8___in_5_, mux_2level_size50_53_sram_blwl_out[2514:2529] ,
mux_2level_size50_53_sram_blwl_outb[2514:2529] );
//----- SRAM bits for MUX[53], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2514_ (mux_2level_size50_53_sram_blwl_out[2514:2514] ,mux_2level_size50_53_sram_blwl_out[2514:2514] ,mux_2level_size50_53_sram_blwl_outb[2514:2514] ,mux_2level_size50_53_configbus0[2514:2514], mux_2level_size50_53_configbus1[2514:2514] , mux_2level_size50_53_configbus0_b[2514:2514] );
sram6T_blwl sram_blwl_2515_ (mux_2level_size50_53_sram_blwl_out[2515:2515] ,mux_2level_size50_53_sram_blwl_out[2515:2515] ,mux_2level_size50_53_sram_blwl_outb[2515:2515] ,mux_2level_size50_53_configbus0[2515:2515], mux_2level_size50_53_configbus1[2515:2515] , mux_2level_size50_53_configbus0_b[2515:2515] );
sram6T_blwl sram_blwl_2516_ (mux_2level_size50_53_sram_blwl_out[2516:2516] ,mux_2level_size50_53_sram_blwl_out[2516:2516] ,mux_2level_size50_53_sram_blwl_outb[2516:2516] ,mux_2level_size50_53_configbus0[2516:2516], mux_2level_size50_53_configbus1[2516:2516] , mux_2level_size50_53_configbus0_b[2516:2516] );
sram6T_blwl sram_blwl_2517_ (mux_2level_size50_53_sram_blwl_out[2517:2517] ,mux_2level_size50_53_sram_blwl_out[2517:2517] ,mux_2level_size50_53_sram_blwl_outb[2517:2517] ,mux_2level_size50_53_configbus0[2517:2517], mux_2level_size50_53_configbus1[2517:2517] , mux_2level_size50_53_configbus0_b[2517:2517] );
sram6T_blwl sram_blwl_2518_ (mux_2level_size50_53_sram_blwl_out[2518:2518] ,mux_2level_size50_53_sram_blwl_out[2518:2518] ,mux_2level_size50_53_sram_blwl_outb[2518:2518] ,mux_2level_size50_53_configbus0[2518:2518], mux_2level_size50_53_configbus1[2518:2518] , mux_2level_size50_53_configbus0_b[2518:2518] );
sram6T_blwl sram_blwl_2519_ (mux_2level_size50_53_sram_blwl_out[2519:2519] ,mux_2level_size50_53_sram_blwl_out[2519:2519] ,mux_2level_size50_53_sram_blwl_outb[2519:2519] ,mux_2level_size50_53_configbus0[2519:2519], mux_2level_size50_53_configbus1[2519:2519] , mux_2level_size50_53_configbus0_b[2519:2519] );
sram6T_blwl sram_blwl_2520_ (mux_2level_size50_53_sram_blwl_out[2520:2520] ,mux_2level_size50_53_sram_blwl_out[2520:2520] ,mux_2level_size50_53_sram_blwl_outb[2520:2520] ,mux_2level_size50_53_configbus0[2520:2520], mux_2level_size50_53_configbus1[2520:2520] , mux_2level_size50_53_configbus0_b[2520:2520] );
sram6T_blwl sram_blwl_2521_ (mux_2level_size50_53_sram_blwl_out[2521:2521] ,mux_2level_size50_53_sram_blwl_out[2521:2521] ,mux_2level_size50_53_sram_blwl_outb[2521:2521] ,mux_2level_size50_53_configbus0[2521:2521], mux_2level_size50_53_configbus1[2521:2521] , mux_2level_size50_53_configbus0_b[2521:2521] );
sram6T_blwl sram_blwl_2522_ (mux_2level_size50_53_sram_blwl_out[2522:2522] ,mux_2level_size50_53_sram_blwl_out[2522:2522] ,mux_2level_size50_53_sram_blwl_outb[2522:2522] ,mux_2level_size50_53_configbus0[2522:2522], mux_2level_size50_53_configbus1[2522:2522] , mux_2level_size50_53_configbus0_b[2522:2522] );
sram6T_blwl sram_blwl_2523_ (mux_2level_size50_53_sram_blwl_out[2523:2523] ,mux_2level_size50_53_sram_blwl_out[2523:2523] ,mux_2level_size50_53_sram_blwl_outb[2523:2523] ,mux_2level_size50_53_configbus0[2523:2523], mux_2level_size50_53_configbus1[2523:2523] , mux_2level_size50_53_configbus0_b[2523:2523] );
sram6T_blwl sram_blwl_2524_ (mux_2level_size50_53_sram_blwl_out[2524:2524] ,mux_2level_size50_53_sram_blwl_out[2524:2524] ,mux_2level_size50_53_sram_blwl_outb[2524:2524] ,mux_2level_size50_53_configbus0[2524:2524], mux_2level_size50_53_configbus1[2524:2524] , mux_2level_size50_53_configbus0_b[2524:2524] );
sram6T_blwl sram_blwl_2525_ (mux_2level_size50_53_sram_blwl_out[2525:2525] ,mux_2level_size50_53_sram_blwl_out[2525:2525] ,mux_2level_size50_53_sram_blwl_outb[2525:2525] ,mux_2level_size50_53_configbus0[2525:2525], mux_2level_size50_53_configbus1[2525:2525] , mux_2level_size50_53_configbus0_b[2525:2525] );
sram6T_blwl sram_blwl_2526_ (mux_2level_size50_53_sram_blwl_out[2526:2526] ,mux_2level_size50_53_sram_blwl_out[2526:2526] ,mux_2level_size50_53_sram_blwl_outb[2526:2526] ,mux_2level_size50_53_configbus0[2526:2526], mux_2level_size50_53_configbus1[2526:2526] , mux_2level_size50_53_configbus0_b[2526:2526] );
sram6T_blwl sram_blwl_2527_ (mux_2level_size50_53_sram_blwl_out[2527:2527] ,mux_2level_size50_53_sram_blwl_out[2527:2527] ,mux_2level_size50_53_sram_blwl_outb[2527:2527] ,mux_2level_size50_53_configbus0[2527:2527], mux_2level_size50_53_configbus1[2527:2527] , mux_2level_size50_53_configbus0_b[2527:2527] );
sram6T_blwl sram_blwl_2528_ (mux_2level_size50_53_sram_blwl_out[2528:2528] ,mux_2level_size50_53_sram_blwl_out[2528:2528] ,mux_2level_size50_53_sram_blwl_outb[2528:2528] ,mux_2level_size50_53_configbus0[2528:2528], mux_2level_size50_53_configbus1[2528:2528] , mux_2level_size50_53_configbus0_b[2528:2528] );
sram6T_blwl sram_blwl_2529_ (mux_2level_size50_53_sram_blwl_out[2529:2529] ,mux_2level_size50_53_sram_blwl_out[2529:2529] ,mux_2level_size50_53_sram_blwl_outb[2529:2529] ,mux_2level_size50_53_configbus0[2529:2529], mux_2level_size50_53_configbus1[2529:2529] , mux_2level_size50_53_configbus0_b[2529:2529] );
direct_interc direct_interc_178_ (mode_clb___clk_0_, fle_8___clk_0_ );
wire [0:49] in_bus_mux_2level_size50_54_ ;
assign in_bus_mux_2level_size50_54_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_54_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_54_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_54_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_54_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_54_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_54_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_54_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_54_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_54_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_54_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_54_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_54_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_54_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_54_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_54_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_54_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_54_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_54_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_54_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_54_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_54_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_54_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_54_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_54_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_54_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_54_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_54_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_54_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_54_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_54_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_54_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_54_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_54_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_54_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_54_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_54_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_54_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_54_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_54_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_54_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_54_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_54_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_54_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_54_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_54_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_54_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_54_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_54_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_54_[49] = fle_9___out_0_ ; 
wire [2530:2545] mux_2level_size50_54_configbus0;
wire [2530:2545] mux_2level_size50_54_configbus1;
wire [2530:2545] mux_2level_size50_54_sram_blwl_out ;
wire [2530:2545] mux_2level_size50_54_sram_blwl_outb ;
assign mux_2level_size50_54_configbus0[2530:2545] = sram_blwl_bl[2530:2545] ;
assign mux_2level_size50_54_configbus1[2530:2545] = sram_blwl_wl[2530:2545] ;
wire [2530:2545] mux_2level_size50_54_configbus0_b;
assign mux_2level_size50_54_configbus0_b[2530:2545] = sram_blwl_blb[2530:2545] ;
mux_2level_size50 mux_2level_size50_54_ (in_bus_mux_2level_size50_54_, fle_9___in_0_, mux_2level_size50_54_sram_blwl_out[2530:2545] ,
mux_2level_size50_54_sram_blwl_outb[2530:2545] );
//----- SRAM bits for MUX[54], level=2, select_path_id=22. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----0010000000000010-----
sram6T_blwl sram_blwl_2530_ (mux_2level_size50_54_sram_blwl_out[2530:2530] ,mux_2level_size50_54_sram_blwl_out[2530:2530] ,mux_2level_size50_54_sram_blwl_outb[2530:2530] ,mux_2level_size50_54_configbus0[2530:2530], mux_2level_size50_54_configbus1[2530:2530] , mux_2level_size50_54_configbus0_b[2530:2530] );
sram6T_blwl sram_blwl_2531_ (mux_2level_size50_54_sram_blwl_out[2531:2531] ,mux_2level_size50_54_sram_blwl_out[2531:2531] ,mux_2level_size50_54_sram_blwl_outb[2531:2531] ,mux_2level_size50_54_configbus0[2531:2531], mux_2level_size50_54_configbus1[2531:2531] , mux_2level_size50_54_configbus0_b[2531:2531] );
sram6T_blwl sram_blwl_2532_ (mux_2level_size50_54_sram_blwl_out[2532:2532] ,mux_2level_size50_54_sram_blwl_out[2532:2532] ,mux_2level_size50_54_sram_blwl_outb[2532:2532] ,mux_2level_size50_54_configbus0[2532:2532], mux_2level_size50_54_configbus1[2532:2532] , mux_2level_size50_54_configbus0_b[2532:2532] );
sram6T_blwl sram_blwl_2533_ (mux_2level_size50_54_sram_blwl_out[2533:2533] ,mux_2level_size50_54_sram_blwl_out[2533:2533] ,mux_2level_size50_54_sram_blwl_outb[2533:2533] ,mux_2level_size50_54_configbus0[2533:2533], mux_2level_size50_54_configbus1[2533:2533] , mux_2level_size50_54_configbus0_b[2533:2533] );
sram6T_blwl sram_blwl_2534_ (mux_2level_size50_54_sram_blwl_out[2534:2534] ,mux_2level_size50_54_sram_blwl_out[2534:2534] ,mux_2level_size50_54_sram_blwl_outb[2534:2534] ,mux_2level_size50_54_configbus0[2534:2534], mux_2level_size50_54_configbus1[2534:2534] , mux_2level_size50_54_configbus0_b[2534:2534] );
sram6T_blwl sram_blwl_2535_ (mux_2level_size50_54_sram_blwl_out[2535:2535] ,mux_2level_size50_54_sram_blwl_out[2535:2535] ,mux_2level_size50_54_sram_blwl_outb[2535:2535] ,mux_2level_size50_54_configbus0[2535:2535], mux_2level_size50_54_configbus1[2535:2535] , mux_2level_size50_54_configbus0_b[2535:2535] );
sram6T_blwl sram_blwl_2536_ (mux_2level_size50_54_sram_blwl_out[2536:2536] ,mux_2level_size50_54_sram_blwl_out[2536:2536] ,mux_2level_size50_54_sram_blwl_outb[2536:2536] ,mux_2level_size50_54_configbus0[2536:2536], mux_2level_size50_54_configbus1[2536:2536] , mux_2level_size50_54_configbus0_b[2536:2536] );
sram6T_blwl sram_blwl_2537_ (mux_2level_size50_54_sram_blwl_out[2537:2537] ,mux_2level_size50_54_sram_blwl_out[2537:2537] ,mux_2level_size50_54_sram_blwl_outb[2537:2537] ,mux_2level_size50_54_configbus0[2537:2537], mux_2level_size50_54_configbus1[2537:2537] , mux_2level_size50_54_configbus0_b[2537:2537] );
sram6T_blwl sram_blwl_2538_ (mux_2level_size50_54_sram_blwl_out[2538:2538] ,mux_2level_size50_54_sram_blwl_out[2538:2538] ,mux_2level_size50_54_sram_blwl_outb[2538:2538] ,mux_2level_size50_54_configbus0[2538:2538], mux_2level_size50_54_configbus1[2538:2538] , mux_2level_size50_54_configbus0_b[2538:2538] );
sram6T_blwl sram_blwl_2539_ (mux_2level_size50_54_sram_blwl_out[2539:2539] ,mux_2level_size50_54_sram_blwl_out[2539:2539] ,mux_2level_size50_54_sram_blwl_outb[2539:2539] ,mux_2level_size50_54_configbus0[2539:2539], mux_2level_size50_54_configbus1[2539:2539] , mux_2level_size50_54_configbus0_b[2539:2539] );
sram6T_blwl sram_blwl_2540_ (mux_2level_size50_54_sram_blwl_out[2540:2540] ,mux_2level_size50_54_sram_blwl_out[2540:2540] ,mux_2level_size50_54_sram_blwl_outb[2540:2540] ,mux_2level_size50_54_configbus0[2540:2540], mux_2level_size50_54_configbus1[2540:2540] , mux_2level_size50_54_configbus0_b[2540:2540] );
sram6T_blwl sram_blwl_2541_ (mux_2level_size50_54_sram_blwl_out[2541:2541] ,mux_2level_size50_54_sram_blwl_out[2541:2541] ,mux_2level_size50_54_sram_blwl_outb[2541:2541] ,mux_2level_size50_54_configbus0[2541:2541], mux_2level_size50_54_configbus1[2541:2541] , mux_2level_size50_54_configbus0_b[2541:2541] );
sram6T_blwl sram_blwl_2542_ (mux_2level_size50_54_sram_blwl_out[2542:2542] ,mux_2level_size50_54_sram_blwl_out[2542:2542] ,mux_2level_size50_54_sram_blwl_outb[2542:2542] ,mux_2level_size50_54_configbus0[2542:2542], mux_2level_size50_54_configbus1[2542:2542] , mux_2level_size50_54_configbus0_b[2542:2542] );
sram6T_blwl sram_blwl_2543_ (mux_2level_size50_54_sram_blwl_out[2543:2543] ,mux_2level_size50_54_sram_blwl_out[2543:2543] ,mux_2level_size50_54_sram_blwl_outb[2543:2543] ,mux_2level_size50_54_configbus0[2543:2543], mux_2level_size50_54_configbus1[2543:2543] , mux_2level_size50_54_configbus0_b[2543:2543] );
sram6T_blwl sram_blwl_2544_ (mux_2level_size50_54_sram_blwl_out[2544:2544] ,mux_2level_size50_54_sram_blwl_out[2544:2544] ,mux_2level_size50_54_sram_blwl_outb[2544:2544] ,mux_2level_size50_54_configbus0[2544:2544], mux_2level_size50_54_configbus1[2544:2544] , mux_2level_size50_54_configbus0_b[2544:2544] );
sram6T_blwl sram_blwl_2545_ (mux_2level_size50_54_sram_blwl_out[2545:2545] ,mux_2level_size50_54_sram_blwl_out[2545:2545] ,mux_2level_size50_54_sram_blwl_outb[2545:2545] ,mux_2level_size50_54_configbus0[2545:2545], mux_2level_size50_54_configbus1[2545:2545] , mux_2level_size50_54_configbus0_b[2545:2545] );
wire [0:49] in_bus_mux_2level_size50_55_ ;
assign in_bus_mux_2level_size50_55_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_55_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_55_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_55_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_55_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_55_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_55_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_55_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_55_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_55_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_55_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_55_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_55_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_55_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_55_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_55_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_55_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_55_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_55_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_55_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_55_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_55_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_55_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_55_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_55_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_55_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_55_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_55_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_55_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_55_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_55_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_55_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_55_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_55_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_55_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_55_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_55_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_55_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_55_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_55_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_55_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_55_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_55_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_55_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_55_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_55_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_55_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_55_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_55_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_55_[49] = fle_9___out_0_ ; 
wire [2546:2561] mux_2level_size50_55_configbus0;
wire [2546:2561] mux_2level_size50_55_configbus1;
wire [2546:2561] mux_2level_size50_55_sram_blwl_out ;
wire [2546:2561] mux_2level_size50_55_sram_blwl_outb ;
assign mux_2level_size50_55_configbus0[2546:2561] = sram_blwl_bl[2546:2561] ;
assign mux_2level_size50_55_configbus1[2546:2561] = sram_blwl_wl[2546:2561] ;
wire [2546:2561] mux_2level_size50_55_configbus0_b;
assign mux_2level_size50_55_configbus0_b[2546:2561] = sram_blwl_blb[2546:2561] ;
mux_2level_size50 mux_2level_size50_55_ (in_bus_mux_2level_size50_55_, fle_9___in_1_, mux_2level_size50_55_sram_blwl_out[2546:2561] ,
mux_2level_size50_55_sram_blwl_outb[2546:2561] );
//----- SRAM bits for MUX[55], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2546_ (mux_2level_size50_55_sram_blwl_out[2546:2546] ,mux_2level_size50_55_sram_blwl_out[2546:2546] ,mux_2level_size50_55_sram_blwl_outb[2546:2546] ,mux_2level_size50_55_configbus0[2546:2546], mux_2level_size50_55_configbus1[2546:2546] , mux_2level_size50_55_configbus0_b[2546:2546] );
sram6T_blwl sram_blwl_2547_ (mux_2level_size50_55_sram_blwl_out[2547:2547] ,mux_2level_size50_55_sram_blwl_out[2547:2547] ,mux_2level_size50_55_sram_blwl_outb[2547:2547] ,mux_2level_size50_55_configbus0[2547:2547], mux_2level_size50_55_configbus1[2547:2547] , mux_2level_size50_55_configbus0_b[2547:2547] );
sram6T_blwl sram_blwl_2548_ (mux_2level_size50_55_sram_blwl_out[2548:2548] ,mux_2level_size50_55_sram_blwl_out[2548:2548] ,mux_2level_size50_55_sram_blwl_outb[2548:2548] ,mux_2level_size50_55_configbus0[2548:2548], mux_2level_size50_55_configbus1[2548:2548] , mux_2level_size50_55_configbus0_b[2548:2548] );
sram6T_blwl sram_blwl_2549_ (mux_2level_size50_55_sram_blwl_out[2549:2549] ,mux_2level_size50_55_sram_blwl_out[2549:2549] ,mux_2level_size50_55_sram_blwl_outb[2549:2549] ,mux_2level_size50_55_configbus0[2549:2549], mux_2level_size50_55_configbus1[2549:2549] , mux_2level_size50_55_configbus0_b[2549:2549] );
sram6T_blwl sram_blwl_2550_ (mux_2level_size50_55_sram_blwl_out[2550:2550] ,mux_2level_size50_55_sram_blwl_out[2550:2550] ,mux_2level_size50_55_sram_blwl_outb[2550:2550] ,mux_2level_size50_55_configbus0[2550:2550], mux_2level_size50_55_configbus1[2550:2550] , mux_2level_size50_55_configbus0_b[2550:2550] );
sram6T_blwl sram_blwl_2551_ (mux_2level_size50_55_sram_blwl_out[2551:2551] ,mux_2level_size50_55_sram_blwl_out[2551:2551] ,mux_2level_size50_55_sram_blwl_outb[2551:2551] ,mux_2level_size50_55_configbus0[2551:2551], mux_2level_size50_55_configbus1[2551:2551] , mux_2level_size50_55_configbus0_b[2551:2551] );
sram6T_blwl sram_blwl_2552_ (mux_2level_size50_55_sram_blwl_out[2552:2552] ,mux_2level_size50_55_sram_blwl_out[2552:2552] ,mux_2level_size50_55_sram_blwl_outb[2552:2552] ,mux_2level_size50_55_configbus0[2552:2552], mux_2level_size50_55_configbus1[2552:2552] , mux_2level_size50_55_configbus0_b[2552:2552] );
sram6T_blwl sram_blwl_2553_ (mux_2level_size50_55_sram_blwl_out[2553:2553] ,mux_2level_size50_55_sram_blwl_out[2553:2553] ,mux_2level_size50_55_sram_blwl_outb[2553:2553] ,mux_2level_size50_55_configbus0[2553:2553], mux_2level_size50_55_configbus1[2553:2553] , mux_2level_size50_55_configbus0_b[2553:2553] );
sram6T_blwl sram_blwl_2554_ (mux_2level_size50_55_sram_blwl_out[2554:2554] ,mux_2level_size50_55_sram_blwl_out[2554:2554] ,mux_2level_size50_55_sram_blwl_outb[2554:2554] ,mux_2level_size50_55_configbus0[2554:2554], mux_2level_size50_55_configbus1[2554:2554] , mux_2level_size50_55_configbus0_b[2554:2554] );
sram6T_blwl sram_blwl_2555_ (mux_2level_size50_55_sram_blwl_out[2555:2555] ,mux_2level_size50_55_sram_blwl_out[2555:2555] ,mux_2level_size50_55_sram_blwl_outb[2555:2555] ,mux_2level_size50_55_configbus0[2555:2555], mux_2level_size50_55_configbus1[2555:2555] , mux_2level_size50_55_configbus0_b[2555:2555] );
sram6T_blwl sram_blwl_2556_ (mux_2level_size50_55_sram_blwl_out[2556:2556] ,mux_2level_size50_55_sram_blwl_out[2556:2556] ,mux_2level_size50_55_sram_blwl_outb[2556:2556] ,mux_2level_size50_55_configbus0[2556:2556], mux_2level_size50_55_configbus1[2556:2556] , mux_2level_size50_55_configbus0_b[2556:2556] );
sram6T_blwl sram_blwl_2557_ (mux_2level_size50_55_sram_blwl_out[2557:2557] ,mux_2level_size50_55_sram_blwl_out[2557:2557] ,mux_2level_size50_55_sram_blwl_outb[2557:2557] ,mux_2level_size50_55_configbus0[2557:2557], mux_2level_size50_55_configbus1[2557:2557] , mux_2level_size50_55_configbus0_b[2557:2557] );
sram6T_blwl sram_blwl_2558_ (mux_2level_size50_55_sram_blwl_out[2558:2558] ,mux_2level_size50_55_sram_blwl_out[2558:2558] ,mux_2level_size50_55_sram_blwl_outb[2558:2558] ,mux_2level_size50_55_configbus0[2558:2558], mux_2level_size50_55_configbus1[2558:2558] , mux_2level_size50_55_configbus0_b[2558:2558] );
sram6T_blwl sram_blwl_2559_ (mux_2level_size50_55_sram_blwl_out[2559:2559] ,mux_2level_size50_55_sram_blwl_out[2559:2559] ,mux_2level_size50_55_sram_blwl_outb[2559:2559] ,mux_2level_size50_55_configbus0[2559:2559], mux_2level_size50_55_configbus1[2559:2559] , mux_2level_size50_55_configbus0_b[2559:2559] );
sram6T_blwl sram_blwl_2560_ (mux_2level_size50_55_sram_blwl_out[2560:2560] ,mux_2level_size50_55_sram_blwl_out[2560:2560] ,mux_2level_size50_55_sram_blwl_outb[2560:2560] ,mux_2level_size50_55_configbus0[2560:2560], mux_2level_size50_55_configbus1[2560:2560] , mux_2level_size50_55_configbus0_b[2560:2560] );
sram6T_blwl sram_blwl_2561_ (mux_2level_size50_55_sram_blwl_out[2561:2561] ,mux_2level_size50_55_sram_blwl_out[2561:2561] ,mux_2level_size50_55_sram_blwl_outb[2561:2561] ,mux_2level_size50_55_configbus0[2561:2561], mux_2level_size50_55_configbus1[2561:2561] , mux_2level_size50_55_configbus0_b[2561:2561] );
wire [0:49] in_bus_mux_2level_size50_56_ ;
assign in_bus_mux_2level_size50_56_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_56_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_56_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_56_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_56_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_56_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_56_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_56_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_56_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_56_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_56_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_56_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_56_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_56_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_56_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_56_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_56_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_56_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_56_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_56_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_56_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_56_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_56_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_56_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_56_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_56_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_56_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_56_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_56_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_56_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_56_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_56_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_56_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_56_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_56_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_56_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_56_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_56_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_56_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_56_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_56_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_56_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_56_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_56_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_56_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_56_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_56_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_56_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_56_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_56_[49] = fle_9___out_0_ ; 
wire [2562:2577] mux_2level_size50_56_configbus0;
wire [2562:2577] mux_2level_size50_56_configbus1;
wire [2562:2577] mux_2level_size50_56_sram_blwl_out ;
wire [2562:2577] mux_2level_size50_56_sram_blwl_outb ;
assign mux_2level_size50_56_configbus0[2562:2577] = sram_blwl_bl[2562:2577] ;
assign mux_2level_size50_56_configbus1[2562:2577] = sram_blwl_wl[2562:2577] ;
wire [2562:2577] mux_2level_size50_56_configbus0_b;
assign mux_2level_size50_56_configbus0_b[2562:2577] = sram_blwl_blb[2562:2577] ;
mux_2level_size50 mux_2level_size50_56_ (in_bus_mux_2level_size50_56_, fle_9___in_2_, mux_2level_size50_56_sram_blwl_out[2562:2577] ,
mux_2level_size50_56_sram_blwl_outb[2562:2577] );
//----- SRAM bits for MUX[56], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2562_ (mux_2level_size50_56_sram_blwl_out[2562:2562] ,mux_2level_size50_56_sram_blwl_out[2562:2562] ,mux_2level_size50_56_sram_blwl_outb[2562:2562] ,mux_2level_size50_56_configbus0[2562:2562], mux_2level_size50_56_configbus1[2562:2562] , mux_2level_size50_56_configbus0_b[2562:2562] );
sram6T_blwl sram_blwl_2563_ (mux_2level_size50_56_sram_blwl_out[2563:2563] ,mux_2level_size50_56_sram_blwl_out[2563:2563] ,mux_2level_size50_56_sram_blwl_outb[2563:2563] ,mux_2level_size50_56_configbus0[2563:2563], mux_2level_size50_56_configbus1[2563:2563] , mux_2level_size50_56_configbus0_b[2563:2563] );
sram6T_blwl sram_blwl_2564_ (mux_2level_size50_56_sram_blwl_out[2564:2564] ,mux_2level_size50_56_sram_blwl_out[2564:2564] ,mux_2level_size50_56_sram_blwl_outb[2564:2564] ,mux_2level_size50_56_configbus0[2564:2564], mux_2level_size50_56_configbus1[2564:2564] , mux_2level_size50_56_configbus0_b[2564:2564] );
sram6T_blwl sram_blwl_2565_ (mux_2level_size50_56_sram_blwl_out[2565:2565] ,mux_2level_size50_56_sram_blwl_out[2565:2565] ,mux_2level_size50_56_sram_blwl_outb[2565:2565] ,mux_2level_size50_56_configbus0[2565:2565], mux_2level_size50_56_configbus1[2565:2565] , mux_2level_size50_56_configbus0_b[2565:2565] );
sram6T_blwl sram_blwl_2566_ (mux_2level_size50_56_sram_blwl_out[2566:2566] ,mux_2level_size50_56_sram_blwl_out[2566:2566] ,mux_2level_size50_56_sram_blwl_outb[2566:2566] ,mux_2level_size50_56_configbus0[2566:2566], mux_2level_size50_56_configbus1[2566:2566] , mux_2level_size50_56_configbus0_b[2566:2566] );
sram6T_blwl sram_blwl_2567_ (mux_2level_size50_56_sram_blwl_out[2567:2567] ,mux_2level_size50_56_sram_blwl_out[2567:2567] ,mux_2level_size50_56_sram_blwl_outb[2567:2567] ,mux_2level_size50_56_configbus0[2567:2567], mux_2level_size50_56_configbus1[2567:2567] , mux_2level_size50_56_configbus0_b[2567:2567] );
sram6T_blwl sram_blwl_2568_ (mux_2level_size50_56_sram_blwl_out[2568:2568] ,mux_2level_size50_56_sram_blwl_out[2568:2568] ,mux_2level_size50_56_sram_blwl_outb[2568:2568] ,mux_2level_size50_56_configbus0[2568:2568], mux_2level_size50_56_configbus1[2568:2568] , mux_2level_size50_56_configbus0_b[2568:2568] );
sram6T_blwl sram_blwl_2569_ (mux_2level_size50_56_sram_blwl_out[2569:2569] ,mux_2level_size50_56_sram_blwl_out[2569:2569] ,mux_2level_size50_56_sram_blwl_outb[2569:2569] ,mux_2level_size50_56_configbus0[2569:2569], mux_2level_size50_56_configbus1[2569:2569] , mux_2level_size50_56_configbus0_b[2569:2569] );
sram6T_blwl sram_blwl_2570_ (mux_2level_size50_56_sram_blwl_out[2570:2570] ,mux_2level_size50_56_sram_blwl_out[2570:2570] ,mux_2level_size50_56_sram_blwl_outb[2570:2570] ,mux_2level_size50_56_configbus0[2570:2570], mux_2level_size50_56_configbus1[2570:2570] , mux_2level_size50_56_configbus0_b[2570:2570] );
sram6T_blwl sram_blwl_2571_ (mux_2level_size50_56_sram_blwl_out[2571:2571] ,mux_2level_size50_56_sram_blwl_out[2571:2571] ,mux_2level_size50_56_sram_blwl_outb[2571:2571] ,mux_2level_size50_56_configbus0[2571:2571], mux_2level_size50_56_configbus1[2571:2571] , mux_2level_size50_56_configbus0_b[2571:2571] );
sram6T_blwl sram_blwl_2572_ (mux_2level_size50_56_sram_blwl_out[2572:2572] ,mux_2level_size50_56_sram_blwl_out[2572:2572] ,mux_2level_size50_56_sram_blwl_outb[2572:2572] ,mux_2level_size50_56_configbus0[2572:2572], mux_2level_size50_56_configbus1[2572:2572] , mux_2level_size50_56_configbus0_b[2572:2572] );
sram6T_blwl sram_blwl_2573_ (mux_2level_size50_56_sram_blwl_out[2573:2573] ,mux_2level_size50_56_sram_blwl_out[2573:2573] ,mux_2level_size50_56_sram_blwl_outb[2573:2573] ,mux_2level_size50_56_configbus0[2573:2573], mux_2level_size50_56_configbus1[2573:2573] , mux_2level_size50_56_configbus0_b[2573:2573] );
sram6T_blwl sram_blwl_2574_ (mux_2level_size50_56_sram_blwl_out[2574:2574] ,mux_2level_size50_56_sram_blwl_out[2574:2574] ,mux_2level_size50_56_sram_blwl_outb[2574:2574] ,mux_2level_size50_56_configbus0[2574:2574], mux_2level_size50_56_configbus1[2574:2574] , mux_2level_size50_56_configbus0_b[2574:2574] );
sram6T_blwl sram_blwl_2575_ (mux_2level_size50_56_sram_blwl_out[2575:2575] ,mux_2level_size50_56_sram_blwl_out[2575:2575] ,mux_2level_size50_56_sram_blwl_outb[2575:2575] ,mux_2level_size50_56_configbus0[2575:2575], mux_2level_size50_56_configbus1[2575:2575] , mux_2level_size50_56_configbus0_b[2575:2575] );
sram6T_blwl sram_blwl_2576_ (mux_2level_size50_56_sram_blwl_out[2576:2576] ,mux_2level_size50_56_sram_blwl_out[2576:2576] ,mux_2level_size50_56_sram_blwl_outb[2576:2576] ,mux_2level_size50_56_configbus0[2576:2576], mux_2level_size50_56_configbus1[2576:2576] , mux_2level_size50_56_configbus0_b[2576:2576] );
sram6T_blwl sram_blwl_2577_ (mux_2level_size50_56_sram_blwl_out[2577:2577] ,mux_2level_size50_56_sram_blwl_out[2577:2577] ,mux_2level_size50_56_sram_blwl_outb[2577:2577] ,mux_2level_size50_56_configbus0[2577:2577], mux_2level_size50_56_configbus1[2577:2577] , mux_2level_size50_56_configbus0_b[2577:2577] );
wire [0:49] in_bus_mux_2level_size50_57_ ;
assign in_bus_mux_2level_size50_57_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_57_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_57_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_57_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_57_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_57_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_57_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_57_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_57_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_57_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_57_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_57_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_57_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_57_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_57_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_57_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_57_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_57_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_57_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_57_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_57_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_57_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_57_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_57_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_57_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_57_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_57_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_57_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_57_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_57_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_57_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_57_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_57_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_57_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_57_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_57_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_57_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_57_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_57_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_57_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_57_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_57_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_57_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_57_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_57_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_57_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_57_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_57_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_57_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_57_[49] = fle_9___out_0_ ; 
wire [2578:2593] mux_2level_size50_57_configbus0;
wire [2578:2593] mux_2level_size50_57_configbus1;
wire [2578:2593] mux_2level_size50_57_sram_blwl_out ;
wire [2578:2593] mux_2level_size50_57_sram_blwl_outb ;
assign mux_2level_size50_57_configbus0[2578:2593] = sram_blwl_bl[2578:2593] ;
assign mux_2level_size50_57_configbus1[2578:2593] = sram_blwl_wl[2578:2593] ;
wire [2578:2593] mux_2level_size50_57_configbus0_b;
assign mux_2level_size50_57_configbus0_b[2578:2593] = sram_blwl_blb[2578:2593] ;
mux_2level_size50 mux_2level_size50_57_ (in_bus_mux_2level_size50_57_, fle_9___in_3_, mux_2level_size50_57_sram_blwl_out[2578:2593] ,
mux_2level_size50_57_sram_blwl_outb[2578:2593] );
//----- SRAM bits for MUX[57], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2578_ (mux_2level_size50_57_sram_blwl_out[2578:2578] ,mux_2level_size50_57_sram_blwl_out[2578:2578] ,mux_2level_size50_57_sram_blwl_outb[2578:2578] ,mux_2level_size50_57_configbus0[2578:2578], mux_2level_size50_57_configbus1[2578:2578] , mux_2level_size50_57_configbus0_b[2578:2578] );
sram6T_blwl sram_blwl_2579_ (mux_2level_size50_57_sram_blwl_out[2579:2579] ,mux_2level_size50_57_sram_blwl_out[2579:2579] ,mux_2level_size50_57_sram_blwl_outb[2579:2579] ,mux_2level_size50_57_configbus0[2579:2579], mux_2level_size50_57_configbus1[2579:2579] , mux_2level_size50_57_configbus0_b[2579:2579] );
sram6T_blwl sram_blwl_2580_ (mux_2level_size50_57_sram_blwl_out[2580:2580] ,mux_2level_size50_57_sram_blwl_out[2580:2580] ,mux_2level_size50_57_sram_blwl_outb[2580:2580] ,mux_2level_size50_57_configbus0[2580:2580], mux_2level_size50_57_configbus1[2580:2580] , mux_2level_size50_57_configbus0_b[2580:2580] );
sram6T_blwl sram_blwl_2581_ (mux_2level_size50_57_sram_blwl_out[2581:2581] ,mux_2level_size50_57_sram_blwl_out[2581:2581] ,mux_2level_size50_57_sram_blwl_outb[2581:2581] ,mux_2level_size50_57_configbus0[2581:2581], mux_2level_size50_57_configbus1[2581:2581] , mux_2level_size50_57_configbus0_b[2581:2581] );
sram6T_blwl sram_blwl_2582_ (mux_2level_size50_57_sram_blwl_out[2582:2582] ,mux_2level_size50_57_sram_blwl_out[2582:2582] ,mux_2level_size50_57_sram_blwl_outb[2582:2582] ,mux_2level_size50_57_configbus0[2582:2582], mux_2level_size50_57_configbus1[2582:2582] , mux_2level_size50_57_configbus0_b[2582:2582] );
sram6T_blwl sram_blwl_2583_ (mux_2level_size50_57_sram_blwl_out[2583:2583] ,mux_2level_size50_57_sram_blwl_out[2583:2583] ,mux_2level_size50_57_sram_blwl_outb[2583:2583] ,mux_2level_size50_57_configbus0[2583:2583], mux_2level_size50_57_configbus1[2583:2583] , mux_2level_size50_57_configbus0_b[2583:2583] );
sram6T_blwl sram_blwl_2584_ (mux_2level_size50_57_sram_blwl_out[2584:2584] ,mux_2level_size50_57_sram_blwl_out[2584:2584] ,mux_2level_size50_57_sram_blwl_outb[2584:2584] ,mux_2level_size50_57_configbus0[2584:2584], mux_2level_size50_57_configbus1[2584:2584] , mux_2level_size50_57_configbus0_b[2584:2584] );
sram6T_blwl sram_blwl_2585_ (mux_2level_size50_57_sram_blwl_out[2585:2585] ,mux_2level_size50_57_sram_blwl_out[2585:2585] ,mux_2level_size50_57_sram_blwl_outb[2585:2585] ,mux_2level_size50_57_configbus0[2585:2585], mux_2level_size50_57_configbus1[2585:2585] , mux_2level_size50_57_configbus0_b[2585:2585] );
sram6T_blwl sram_blwl_2586_ (mux_2level_size50_57_sram_blwl_out[2586:2586] ,mux_2level_size50_57_sram_blwl_out[2586:2586] ,mux_2level_size50_57_sram_blwl_outb[2586:2586] ,mux_2level_size50_57_configbus0[2586:2586], mux_2level_size50_57_configbus1[2586:2586] , mux_2level_size50_57_configbus0_b[2586:2586] );
sram6T_blwl sram_blwl_2587_ (mux_2level_size50_57_sram_blwl_out[2587:2587] ,mux_2level_size50_57_sram_blwl_out[2587:2587] ,mux_2level_size50_57_sram_blwl_outb[2587:2587] ,mux_2level_size50_57_configbus0[2587:2587], mux_2level_size50_57_configbus1[2587:2587] , mux_2level_size50_57_configbus0_b[2587:2587] );
sram6T_blwl sram_blwl_2588_ (mux_2level_size50_57_sram_blwl_out[2588:2588] ,mux_2level_size50_57_sram_blwl_out[2588:2588] ,mux_2level_size50_57_sram_blwl_outb[2588:2588] ,mux_2level_size50_57_configbus0[2588:2588], mux_2level_size50_57_configbus1[2588:2588] , mux_2level_size50_57_configbus0_b[2588:2588] );
sram6T_blwl sram_blwl_2589_ (mux_2level_size50_57_sram_blwl_out[2589:2589] ,mux_2level_size50_57_sram_blwl_out[2589:2589] ,mux_2level_size50_57_sram_blwl_outb[2589:2589] ,mux_2level_size50_57_configbus0[2589:2589], mux_2level_size50_57_configbus1[2589:2589] , mux_2level_size50_57_configbus0_b[2589:2589] );
sram6T_blwl sram_blwl_2590_ (mux_2level_size50_57_sram_blwl_out[2590:2590] ,mux_2level_size50_57_sram_blwl_out[2590:2590] ,mux_2level_size50_57_sram_blwl_outb[2590:2590] ,mux_2level_size50_57_configbus0[2590:2590], mux_2level_size50_57_configbus1[2590:2590] , mux_2level_size50_57_configbus0_b[2590:2590] );
sram6T_blwl sram_blwl_2591_ (mux_2level_size50_57_sram_blwl_out[2591:2591] ,mux_2level_size50_57_sram_blwl_out[2591:2591] ,mux_2level_size50_57_sram_blwl_outb[2591:2591] ,mux_2level_size50_57_configbus0[2591:2591], mux_2level_size50_57_configbus1[2591:2591] , mux_2level_size50_57_configbus0_b[2591:2591] );
sram6T_blwl sram_blwl_2592_ (mux_2level_size50_57_sram_blwl_out[2592:2592] ,mux_2level_size50_57_sram_blwl_out[2592:2592] ,mux_2level_size50_57_sram_blwl_outb[2592:2592] ,mux_2level_size50_57_configbus0[2592:2592], mux_2level_size50_57_configbus1[2592:2592] , mux_2level_size50_57_configbus0_b[2592:2592] );
sram6T_blwl sram_blwl_2593_ (mux_2level_size50_57_sram_blwl_out[2593:2593] ,mux_2level_size50_57_sram_blwl_out[2593:2593] ,mux_2level_size50_57_sram_blwl_outb[2593:2593] ,mux_2level_size50_57_configbus0[2593:2593], mux_2level_size50_57_configbus1[2593:2593] , mux_2level_size50_57_configbus0_b[2593:2593] );
wire [0:49] in_bus_mux_2level_size50_58_ ;
assign in_bus_mux_2level_size50_58_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_58_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_58_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_58_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_58_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_58_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_58_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_58_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_58_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_58_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_58_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_58_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_58_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_58_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_58_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_58_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_58_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_58_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_58_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_58_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_58_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_58_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_58_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_58_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_58_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_58_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_58_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_58_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_58_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_58_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_58_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_58_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_58_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_58_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_58_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_58_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_58_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_58_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_58_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_58_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_58_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_58_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_58_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_58_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_58_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_58_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_58_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_58_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_58_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_58_[49] = fle_9___out_0_ ; 
wire [2594:2609] mux_2level_size50_58_configbus0;
wire [2594:2609] mux_2level_size50_58_configbus1;
wire [2594:2609] mux_2level_size50_58_sram_blwl_out ;
wire [2594:2609] mux_2level_size50_58_sram_blwl_outb ;
assign mux_2level_size50_58_configbus0[2594:2609] = sram_blwl_bl[2594:2609] ;
assign mux_2level_size50_58_configbus1[2594:2609] = sram_blwl_wl[2594:2609] ;
wire [2594:2609] mux_2level_size50_58_configbus0_b;
assign mux_2level_size50_58_configbus0_b[2594:2609] = sram_blwl_blb[2594:2609] ;
mux_2level_size50 mux_2level_size50_58_ (in_bus_mux_2level_size50_58_, fle_9___in_4_, mux_2level_size50_58_sram_blwl_out[2594:2609] ,
mux_2level_size50_58_sram_blwl_outb[2594:2609] );
//----- SRAM bits for MUX[58], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2594_ (mux_2level_size50_58_sram_blwl_out[2594:2594] ,mux_2level_size50_58_sram_blwl_out[2594:2594] ,mux_2level_size50_58_sram_blwl_outb[2594:2594] ,mux_2level_size50_58_configbus0[2594:2594], mux_2level_size50_58_configbus1[2594:2594] , mux_2level_size50_58_configbus0_b[2594:2594] );
sram6T_blwl sram_blwl_2595_ (mux_2level_size50_58_sram_blwl_out[2595:2595] ,mux_2level_size50_58_sram_blwl_out[2595:2595] ,mux_2level_size50_58_sram_blwl_outb[2595:2595] ,mux_2level_size50_58_configbus0[2595:2595], mux_2level_size50_58_configbus1[2595:2595] , mux_2level_size50_58_configbus0_b[2595:2595] );
sram6T_blwl sram_blwl_2596_ (mux_2level_size50_58_sram_blwl_out[2596:2596] ,mux_2level_size50_58_sram_blwl_out[2596:2596] ,mux_2level_size50_58_sram_blwl_outb[2596:2596] ,mux_2level_size50_58_configbus0[2596:2596], mux_2level_size50_58_configbus1[2596:2596] , mux_2level_size50_58_configbus0_b[2596:2596] );
sram6T_blwl sram_blwl_2597_ (mux_2level_size50_58_sram_blwl_out[2597:2597] ,mux_2level_size50_58_sram_blwl_out[2597:2597] ,mux_2level_size50_58_sram_blwl_outb[2597:2597] ,mux_2level_size50_58_configbus0[2597:2597], mux_2level_size50_58_configbus1[2597:2597] , mux_2level_size50_58_configbus0_b[2597:2597] );
sram6T_blwl sram_blwl_2598_ (mux_2level_size50_58_sram_blwl_out[2598:2598] ,mux_2level_size50_58_sram_blwl_out[2598:2598] ,mux_2level_size50_58_sram_blwl_outb[2598:2598] ,mux_2level_size50_58_configbus0[2598:2598], mux_2level_size50_58_configbus1[2598:2598] , mux_2level_size50_58_configbus0_b[2598:2598] );
sram6T_blwl sram_blwl_2599_ (mux_2level_size50_58_sram_blwl_out[2599:2599] ,mux_2level_size50_58_sram_blwl_out[2599:2599] ,mux_2level_size50_58_sram_blwl_outb[2599:2599] ,mux_2level_size50_58_configbus0[2599:2599], mux_2level_size50_58_configbus1[2599:2599] , mux_2level_size50_58_configbus0_b[2599:2599] );
sram6T_blwl sram_blwl_2600_ (mux_2level_size50_58_sram_blwl_out[2600:2600] ,mux_2level_size50_58_sram_blwl_out[2600:2600] ,mux_2level_size50_58_sram_blwl_outb[2600:2600] ,mux_2level_size50_58_configbus0[2600:2600], mux_2level_size50_58_configbus1[2600:2600] , mux_2level_size50_58_configbus0_b[2600:2600] );
sram6T_blwl sram_blwl_2601_ (mux_2level_size50_58_sram_blwl_out[2601:2601] ,mux_2level_size50_58_sram_blwl_out[2601:2601] ,mux_2level_size50_58_sram_blwl_outb[2601:2601] ,mux_2level_size50_58_configbus0[2601:2601], mux_2level_size50_58_configbus1[2601:2601] , mux_2level_size50_58_configbus0_b[2601:2601] );
sram6T_blwl sram_blwl_2602_ (mux_2level_size50_58_sram_blwl_out[2602:2602] ,mux_2level_size50_58_sram_blwl_out[2602:2602] ,mux_2level_size50_58_sram_blwl_outb[2602:2602] ,mux_2level_size50_58_configbus0[2602:2602], mux_2level_size50_58_configbus1[2602:2602] , mux_2level_size50_58_configbus0_b[2602:2602] );
sram6T_blwl sram_blwl_2603_ (mux_2level_size50_58_sram_blwl_out[2603:2603] ,mux_2level_size50_58_sram_blwl_out[2603:2603] ,mux_2level_size50_58_sram_blwl_outb[2603:2603] ,mux_2level_size50_58_configbus0[2603:2603], mux_2level_size50_58_configbus1[2603:2603] , mux_2level_size50_58_configbus0_b[2603:2603] );
sram6T_blwl sram_blwl_2604_ (mux_2level_size50_58_sram_blwl_out[2604:2604] ,mux_2level_size50_58_sram_blwl_out[2604:2604] ,mux_2level_size50_58_sram_blwl_outb[2604:2604] ,mux_2level_size50_58_configbus0[2604:2604], mux_2level_size50_58_configbus1[2604:2604] , mux_2level_size50_58_configbus0_b[2604:2604] );
sram6T_blwl sram_blwl_2605_ (mux_2level_size50_58_sram_blwl_out[2605:2605] ,mux_2level_size50_58_sram_blwl_out[2605:2605] ,mux_2level_size50_58_sram_blwl_outb[2605:2605] ,mux_2level_size50_58_configbus0[2605:2605], mux_2level_size50_58_configbus1[2605:2605] , mux_2level_size50_58_configbus0_b[2605:2605] );
sram6T_blwl sram_blwl_2606_ (mux_2level_size50_58_sram_blwl_out[2606:2606] ,mux_2level_size50_58_sram_blwl_out[2606:2606] ,mux_2level_size50_58_sram_blwl_outb[2606:2606] ,mux_2level_size50_58_configbus0[2606:2606], mux_2level_size50_58_configbus1[2606:2606] , mux_2level_size50_58_configbus0_b[2606:2606] );
sram6T_blwl sram_blwl_2607_ (mux_2level_size50_58_sram_blwl_out[2607:2607] ,mux_2level_size50_58_sram_blwl_out[2607:2607] ,mux_2level_size50_58_sram_blwl_outb[2607:2607] ,mux_2level_size50_58_configbus0[2607:2607], mux_2level_size50_58_configbus1[2607:2607] , mux_2level_size50_58_configbus0_b[2607:2607] );
sram6T_blwl sram_blwl_2608_ (mux_2level_size50_58_sram_blwl_out[2608:2608] ,mux_2level_size50_58_sram_blwl_out[2608:2608] ,mux_2level_size50_58_sram_blwl_outb[2608:2608] ,mux_2level_size50_58_configbus0[2608:2608], mux_2level_size50_58_configbus1[2608:2608] , mux_2level_size50_58_configbus0_b[2608:2608] );
sram6T_blwl sram_blwl_2609_ (mux_2level_size50_58_sram_blwl_out[2609:2609] ,mux_2level_size50_58_sram_blwl_out[2609:2609] ,mux_2level_size50_58_sram_blwl_outb[2609:2609] ,mux_2level_size50_58_configbus0[2609:2609], mux_2level_size50_58_configbus1[2609:2609] , mux_2level_size50_58_configbus0_b[2609:2609] );
wire [0:49] in_bus_mux_2level_size50_59_ ;
assign in_bus_mux_2level_size50_59_[0] = mode_clb___I_0_ ; 
assign in_bus_mux_2level_size50_59_[1] = mode_clb___I_1_ ; 
assign in_bus_mux_2level_size50_59_[2] = mode_clb___I_2_ ; 
assign in_bus_mux_2level_size50_59_[3] = mode_clb___I_3_ ; 
assign in_bus_mux_2level_size50_59_[4] = mode_clb___I_4_ ; 
assign in_bus_mux_2level_size50_59_[5] = mode_clb___I_5_ ; 
assign in_bus_mux_2level_size50_59_[6] = mode_clb___I_6_ ; 
assign in_bus_mux_2level_size50_59_[7] = mode_clb___I_7_ ; 
assign in_bus_mux_2level_size50_59_[8] = mode_clb___I_8_ ; 
assign in_bus_mux_2level_size50_59_[9] = mode_clb___I_9_ ; 
assign in_bus_mux_2level_size50_59_[10] = mode_clb___I_10_ ; 
assign in_bus_mux_2level_size50_59_[11] = mode_clb___I_11_ ; 
assign in_bus_mux_2level_size50_59_[12] = mode_clb___I_12_ ; 
assign in_bus_mux_2level_size50_59_[13] = mode_clb___I_13_ ; 
assign in_bus_mux_2level_size50_59_[14] = mode_clb___I_14_ ; 
assign in_bus_mux_2level_size50_59_[15] = mode_clb___I_15_ ; 
assign in_bus_mux_2level_size50_59_[16] = mode_clb___I_16_ ; 
assign in_bus_mux_2level_size50_59_[17] = mode_clb___I_17_ ; 
assign in_bus_mux_2level_size50_59_[18] = mode_clb___I_18_ ; 
assign in_bus_mux_2level_size50_59_[19] = mode_clb___I_19_ ; 
assign in_bus_mux_2level_size50_59_[20] = mode_clb___I_20_ ; 
assign in_bus_mux_2level_size50_59_[21] = mode_clb___I_21_ ; 
assign in_bus_mux_2level_size50_59_[22] = mode_clb___I_22_ ; 
assign in_bus_mux_2level_size50_59_[23] = mode_clb___I_23_ ; 
assign in_bus_mux_2level_size50_59_[24] = mode_clb___I_24_ ; 
assign in_bus_mux_2level_size50_59_[25] = mode_clb___I_25_ ; 
assign in_bus_mux_2level_size50_59_[26] = mode_clb___I_26_ ; 
assign in_bus_mux_2level_size50_59_[27] = mode_clb___I_27_ ; 
assign in_bus_mux_2level_size50_59_[28] = mode_clb___I_28_ ; 
assign in_bus_mux_2level_size50_59_[29] = mode_clb___I_29_ ; 
assign in_bus_mux_2level_size50_59_[30] = mode_clb___I_30_ ; 
assign in_bus_mux_2level_size50_59_[31] = mode_clb___I_31_ ; 
assign in_bus_mux_2level_size50_59_[32] = mode_clb___I_32_ ; 
assign in_bus_mux_2level_size50_59_[33] = mode_clb___I_33_ ; 
assign in_bus_mux_2level_size50_59_[34] = mode_clb___I_34_ ; 
assign in_bus_mux_2level_size50_59_[35] = mode_clb___I_35_ ; 
assign in_bus_mux_2level_size50_59_[36] = mode_clb___I_36_ ; 
assign in_bus_mux_2level_size50_59_[37] = mode_clb___I_37_ ; 
assign in_bus_mux_2level_size50_59_[38] = mode_clb___I_38_ ; 
assign in_bus_mux_2level_size50_59_[39] = mode_clb___I_39_ ; 
assign in_bus_mux_2level_size50_59_[40] = fle_0___out_0_ ; 
assign in_bus_mux_2level_size50_59_[41] = fle_1___out_0_ ; 
assign in_bus_mux_2level_size50_59_[42] = fle_2___out_0_ ; 
assign in_bus_mux_2level_size50_59_[43] = fle_3___out_0_ ; 
assign in_bus_mux_2level_size50_59_[44] = fle_4___out_0_ ; 
assign in_bus_mux_2level_size50_59_[45] = fle_5___out_0_ ; 
assign in_bus_mux_2level_size50_59_[46] = fle_6___out_0_ ; 
assign in_bus_mux_2level_size50_59_[47] = fle_7___out_0_ ; 
assign in_bus_mux_2level_size50_59_[48] = fle_8___out_0_ ; 
assign in_bus_mux_2level_size50_59_[49] = fle_9___out_0_ ; 
wire [2610:2625] mux_2level_size50_59_configbus0;
wire [2610:2625] mux_2level_size50_59_configbus1;
wire [2610:2625] mux_2level_size50_59_sram_blwl_out ;
wire [2610:2625] mux_2level_size50_59_sram_blwl_outb ;
assign mux_2level_size50_59_configbus0[2610:2625] = sram_blwl_bl[2610:2625] ;
assign mux_2level_size50_59_configbus1[2610:2625] = sram_blwl_wl[2610:2625] ;
wire [2610:2625] mux_2level_size50_59_configbus0_b;
assign mux_2level_size50_59_configbus0_b[2610:2625] = sram_blwl_blb[2610:2625] ;
mux_2level_size50 mux_2level_size50_59_ (in_bus_mux_2level_size50_59_, fle_9___in_5_, mux_2level_size50_59_sram_blwl_out[2610:2625] ,
mux_2level_size50_59_sram_blwl_outb[2610:2625] );
//----- SRAM bits for MUX[59], level=2, select_path_id=0. -----
//----- From LSB(LEFT) TO MSB (RIGHT) -----
//-----1000000010000000-----
sram6T_blwl sram_blwl_2610_ (mux_2level_size50_59_sram_blwl_out[2610:2610] ,mux_2level_size50_59_sram_blwl_out[2610:2610] ,mux_2level_size50_59_sram_blwl_outb[2610:2610] ,mux_2level_size50_59_configbus0[2610:2610], mux_2level_size50_59_configbus1[2610:2610] , mux_2level_size50_59_configbus0_b[2610:2610] );
sram6T_blwl sram_blwl_2611_ (mux_2level_size50_59_sram_blwl_out[2611:2611] ,mux_2level_size50_59_sram_blwl_out[2611:2611] ,mux_2level_size50_59_sram_blwl_outb[2611:2611] ,mux_2level_size50_59_configbus0[2611:2611], mux_2level_size50_59_configbus1[2611:2611] , mux_2level_size50_59_configbus0_b[2611:2611] );
sram6T_blwl sram_blwl_2612_ (mux_2level_size50_59_sram_blwl_out[2612:2612] ,mux_2level_size50_59_sram_blwl_out[2612:2612] ,mux_2level_size50_59_sram_blwl_outb[2612:2612] ,mux_2level_size50_59_configbus0[2612:2612], mux_2level_size50_59_configbus1[2612:2612] , mux_2level_size50_59_configbus0_b[2612:2612] );
sram6T_blwl sram_blwl_2613_ (mux_2level_size50_59_sram_blwl_out[2613:2613] ,mux_2level_size50_59_sram_blwl_out[2613:2613] ,mux_2level_size50_59_sram_blwl_outb[2613:2613] ,mux_2level_size50_59_configbus0[2613:2613], mux_2level_size50_59_configbus1[2613:2613] , mux_2level_size50_59_configbus0_b[2613:2613] );
sram6T_blwl sram_blwl_2614_ (mux_2level_size50_59_sram_blwl_out[2614:2614] ,mux_2level_size50_59_sram_blwl_out[2614:2614] ,mux_2level_size50_59_sram_blwl_outb[2614:2614] ,mux_2level_size50_59_configbus0[2614:2614], mux_2level_size50_59_configbus1[2614:2614] , mux_2level_size50_59_configbus0_b[2614:2614] );
sram6T_blwl sram_blwl_2615_ (mux_2level_size50_59_sram_blwl_out[2615:2615] ,mux_2level_size50_59_sram_blwl_out[2615:2615] ,mux_2level_size50_59_sram_blwl_outb[2615:2615] ,mux_2level_size50_59_configbus0[2615:2615], mux_2level_size50_59_configbus1[2615:2615] , mux_2level_size50_59_configbus0_b[2615:2615] );
sram6T_blwl sram_blwl_2616_ (mux_2level_size50_59_sram_blwl_out[2616:2616] ,mux_2level_size50_59_sram_blwl_out[2616:2616] ,mux_2level_size50_59_sram_blwl_outb[2616:2616] ,mux_2level_size50_59_configbus0[2616:2616], mux_2level_size50_59_configbus1[2616:2616] , mux_2level_size50_59_configbus0_b[2616:2616] );
sram6T_blwl sram_blwl_2617_ (mux_2level_size50_59_sram_blwl_out[2617:2617] ,mux_2level_size50_59_sram_blwl_out[2617:2617] ,mux_2level_size50_59_sram_blwl_outb[2617:2617] ,mux_2level_size50_59_configbus0[2617:2617], mux_2level_size50_59_configbus1[2617:2617] , mux_2level_size50_59_configbus0_b[2617:2617] );
sram6T_blwl sram_blwl_2618_ (mux_2level_size50_59_sram_blwl_out[2618:2618] ,mux_2level_size50_59_sram_blwl_out[2618:2618] ,mux_2level_size50_59_sram_blwl_outb[2618:2618] ,mux_2level_size50_59_configbus0[2618:2618], mux_2level_size50_59_configbus1[2618:2618] , mux_2level_size50_59_configbus0_b[2618:2618] );
sram6T_blwl sram_blwl_2619_ (mux_2level_size50_59_sram_blwl_out[2619:2619] ,mux_2level_size50_59_sram_blwl_out[2619:2619] ,mux_2level_size50_59_sram_blwl_outb[2619:2619] ,mux_2level_size50_59_configbus0[2619:2619], mux_2level_size50_59_configbus1[2619:2619] , mux_2level_size50_59_configbus0_b[2619:2619] );
sram6T_blwl sram_blwl_2620_ (mux_2level_size50_59_sram_blwl_out[2620:2620] ,mux_2level_size50_59_sram_blwl_out[2620:2620] ,mux_2level_size50_59_sram_blwl_outb[2620:2620] ,mux_2level_size50_59_configbus0[2620:2620], mux_2level_size50_59_configbus1[2620:2620] , mux_2level_size50_59_configbus0_b[2620:2620] );
sram6T_blwl sram_blwl_2621_ (mux_2level_size50_59_sram_blwl_out[2621:2621] ,mux_2level_size50_59_sram_blwl_out[2621:2621] ,mux_2level_size50_59_sram_blwl_outb[2621:2621] ,mux_2level_size50_59_configbus0[2621:2621], mux_2level_size50_59_configbus1[2621:2621] , mux_2level_size50_59_configbus0_b[2621:2621] );
sram6T_blwl sram_blwl_2622_ (mux_2level_size50_59_sram_blwl_out[2622:2622] ,mux_2level_size50_59_sram_blwl_out[2622:2622] ,mux_2level_size50_59_sram_blwl_outb[2622:2622] ,mux_2level_size50_59_configbus0[2622:2622], mux_2level_size50_59_configbus1[2622:2622] , mux_2level_size50_59_configbus0_b[2622:2622] );
sram6T_blwl sram_blwl_2623_ (mux_2level_size50_59_sram_blwl_out[2623:2623] ,mux_2level_size50_59_sram_blwl_out[2623:2623] ,mux_2level_size50_59_sram_blwl_outb[2623:2623] ,mux_2level_size50_59_configbus0[2623:2623], mux_2level_size50_59_configbus1[2623:2623] , mux_2level_size50_59_configbus0_b[2623:2623] );
sram6T_blwl sram_blwl_2624_ (mux_2level_size50_59_sram_blwl_out[2624:2624] ,mux_2level_size50_59_sram_blwl_out[2624:2624] ,mux_2level_size50_59_sram_blwl_outb[2624:2624] ,mux_2level_size50_59_configbus0[2624:2624], mux_2level_size50_59_configbus1[2624:2624] , mux_2level_size50_59_configbus0_b[2624:2624] );
sram6T_blwl sram_blwl_2625_ (mux_2level_size50_59_sram_blwl_out[2625:2625] ,mux_2level_size50_59_sram_blwl_out[2625:2625] ,mux_2level_size50_59_sram_blwl_outb[2625:2625] ,mux_2level_size50_59_configbus0[2625:2625], mux_2level_size50_59_configbus1[2625:2625] , mux_2level_size50_59_configbus0_b[2625:2625] );
direct_interc direct_interc_179_ (mode_clb___clk_0_, fle_9___clk_0_ );
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
input  top_height_0__pin_4_,
input  top_height_0__pin_8_,
input  top_height_0__pin_12_,
input  top_height_0__pin_16_,
input  top_height_0__pin_20_,
input  top_height_0__pin_24_,
input  top_height_0__pin_28_,
input  top_height_0__pin_32_,
input  top_height_0__pin_36_,
output  top_height_0__pin_40_,
output  top_height_0__pin_44_,
output  top_height_0__pin_48_,
input  right_height_0__pin_1_,
input  right_height_0__pin_5_,
input  right_height_0__pin_9_,
input  right_height_0__pin_13_,
input  right_height_0__pin_17_,
input  right_height_0__pin_21_,
input  right_height_0__pin_25_,
input  right_height_0__pin_29_,
input  right_height_0__pin_33_,
input  right_height_0__pin_37_,
output  right_height_0__pin_41_,
output  right_height_0__pin_45_,
output  right_height_0__pin_49_,
input  bottom_height_0__pin_2_,
input  bottom_height_0__pin_6_,
input  bottom_height_0__pin_10_,
input  bottom_height_0__pin_14_,
input  bottom_height_0__pin_18_,
input  bottom_height_0__pin_22_,
input  bottom_height_0__pin_26_,
input  bottom_height_0__pin_30_,
input  bottom_height_0__pin_34_,
input  bottom_height_0__pin_38_,
output  bottom_height_0__pin_42_,
output  bottom_height_0__pin_46_,
input  bottom_height_0__pin_50_,
input  left_height_0__pin_3_,
input  left_height_0__pin_7_,
input  left_height_0__pin_11_,
input  left_height_0__pin_15_,
input  left_height_0__pin_19_,
input  left_height_0__pin_23_,
input  left_height_0__pin_27_,
input  left_height_0__pin_31_,
input  left_height_0__pin_35_,
input  left_height_0__pin_39_,
output  left_height_0__pin_43_,
output  left_height_0__pin_47_,
input [1016:2625] sram_blwl_bl ,
input [1016:2625] sram_blwl_wl ,
input [1016:2625] sram_blwl_blb );
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
right_height_0__pin_5_ ,
bottom_height_0__pin_6_ ,
left_height_0__pin_7_ ,
top_height_0__pin_8_ ,
right_height_0__pin_9_ ,
bottom_height_0__pin_10_ ,
left_height_0__pin_11_ ,
top_height_0__pin_12_ ,
right_height_0__pin_13_ ,
bottom_height_0__pin_14_ ,
left_height_0__pin_15_ ,
top_height_0__pin_16_ ,
right_height_0__pin_17_ ,
bottom_height_0__pin_18_ ,
left_height_0__pin_19_ ,
top_height_0__pin_20_ ,
right_height_0__pin_21_ ,
bottom_height_0__pin_22_ ,
left_height_0__pin_23_ ,
top_height_0__pin_24_ ,
right_height_0__pin_25_ ,
bottom_height_0__pin_26_ ,
left_height_0__pin_27_ ,
top_height_0__pin_28_ ,
right_height_0__pin_29_ ,
bottom_height_0__pin_30_ ,
left_height_0__pin_31_ ,
top_height_0__pin_32_ ,
right_height_0__pin_33_ ,
bottom_height_0__pin_34_ ,
left_height_0__pin_35_ ,
top_height_0__pin_36_ ,
right_height_0__pin_37_ ,
bottom_height_0__pin_38_ ,
left_height_0__pin_39_ ,
top_height_0__pin_40_ ,
right_height_0__pin_41_ ,
bottom_height_0__pin_42_ ,
left_height_0__pin_43_ ,
top_height_0__pin_44_ ,
right_height_0__pin_45_ ,
bottom_height_0__pin_46_ ,
left_height_0__pin_47_ ,
top_height_0__pin_48_ ,
right_height_0__pin_49_ ,
bottom_height_0__pin_50_ 
//---- IOPAD ----
,
//---- SRAM ----
sram_blwl_bl[1016:2625] ,
sram_blwl_wl[1016:2625] ,
sram_blwl_blb[1016:2625] );
endmodule
//----- END Top Protocol -----
//----- END Grid[1][1], Capactity: 1 -----

