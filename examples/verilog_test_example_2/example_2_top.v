//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: FPGA Verilog Netlist for Design: example_2 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Include User-defined netlists -----
// `include "/research/ece/lnis/USERS/tang/tangxifan-eda-tools/branches/vpr7_rram/vpr/VerilogNetlists/ff.v"
// `include "/research/ece/lnis/USERS/tang/tangxifan-eda-tools/branches/vpr7_rram/vpr/VerilogNetlists/sram.v"
// `include "/research/ece/lnis/USERS/tang/tangxifan-eda-tools/branches/vpr7_rram/vpr/VerilogNetlists/io.v"
//----- Include subckt netlists: Multiplexers -----
// `include "./verilog_test_example_2/routing/muxes.v"
//----- Include subckt netlists: Wires -----
// `include "./verilog_test_example_2/routing/wires.v"
//----- Include subckt netlists: Look-Up Tables (LUTs) -----
// `include "./verilog_test_example_2/routing/luts.v"
//------ Include subckt netlists: Logic Blocks -----
// `include "./verilog_test_example_2/routing/logic_blocks.v"
//----- Include subckt netlists: Routing structures (Switch Boxes, Channels, Connection Boxes) -----
// `include "./verilog_test_example_2/routing/routing.v"
//----- Include subckt netlists: Decoders (controller for memeory bank) -----
// `include "./verilog_test_example_2/routing/decoders.v"
//----- Top-level Verilog Module -----
module example_2_top (

//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
inout [31:0] gfpga_pad_iopad , //---FPGA inouts 
input [0:0] en_bl , //--- BL enable port 
input [0:0] en_wl , //--- WL enable port 
input [0:0] data_in , //--- BL data input port 
input [5:0] addr_bl , //--- Address of bit lines 
input [5:0] addr_wl  //--- Address of word lines 
);
wire [0:51] bl_bus ; //--- Array Bit lines bus 
wire [0:51] wl_bus ; //--- Array Bit lines bus 
wire [0:51] blb_bus ; //--- Inverted Array Bit lines bus 

  wire [0:2657] sram_blwl_bl; //---- Normal Bit lines 
  wire [0:2657] sram_blwl_wl; //---- Normal Word lines 
  wire [0:2657] sram_blwl_blb; //---- Inverted Normal Bit lines 
 INVTX1 INVTX1_blb_0 (bl_bus[0], blb_bus[0]);
 INVTX1 INVTX1_blb_1 (bl_bus[1], blb_bus[1]);
 INVTX1 INVTX1_blb_2 (bl_bus[2], blb_bus[2]);
 INVTX1 INVTX1_blb_3 (bl_bus[3], blb_bus[3]);
 INVTX1 INVTX1_blb_4 (bl_bus[4], blb_bus[4]);
 INVTX1 INVTX1_blb_5 (bl_bus[5], blb_bus[5]);
 INVTX1 INVTX1_blb_6 (bl_bus[6], blb_bus[6]);
 INVTX1 INVTX1_blb_7 (bl_bus[7], blb_bus[7]);
 INVTX1 INVTX1_blb_8 (bl_bus[8], blb_bus[8]);
 INVTX1 INVTX1_blb_9 (bl_bus[9], blb_bus[9]);
 INVTX1 INVTX1_blb_10 (bl_bus[10], blb_bus[10]);
 INVTX1 INVTX1_blb_11 (bl_bus[11], blb_bus[11]);
 INVTX1 INVTX1_blb_12 (bl_bus[12], blb_bus[12]);
 INVTX1 INVTX1_blb_13 (bl_bus[13], blb_bus[13]);
 INVTX1 INVTX1_blb_14 (bl_bus[14], blb_bus[14]);
 INVTX1 INVTX1_blb_15 (bl_bus[15], blb_bus[15]);
 INVTX1 INVTX1_blb_16 (bl_bus[16], blb_bus[16]);
 INVTX1 INVTX1_blb_17 (bl_bus[17], blb_bus[17]);
 INVTX1 INVTX1_blb_18 (bl_bus[18], blb_bus[18]);
 INVTX1 INVTX1_blb_19 (bl_bus[19], blb_bus[19]);
 INVTX1 INVTX1_blb_20 (bl_bus[20], blb_bus[20]);
 INVTX1 INVTX1_blb_21 (bl_bus[21], blb_bus[21]);
 INVTX1 INVTX1_blb_22 (bl_bus[22], blb_bus[22]);
 INVTX1 INVTX1_blb_23 (bl_bus[23], blb_bus[23]);
 INVTX1 INVTX1_blb_24 (bl_bus[24], blb_bus[24]);
 INVTX1 INVTX1_blb_25 (bl_bus[25], blb_bus[25]);
 INVTX1 INVTX1_blb_26 (bl_bus[26], blb_bus[26]);
 INVTX1 INVTX1_blb_27 (bl_bus[27], blb_bus[27]);
 INVTX1 INVTX1_blb_28 (bl_bus[28], blb_bus[28]);
 INVTX1 INVTX1_blb_29 (bl_bus[29], blb_bus[29]);
 INVTX1 INVTX1_blb_30 (bl_bus[30], blb_bus[30]);
 INVTX1 INVTX1_blb_31 (bl_bus[31], blb_bus[31]);
 INVTX1 INVTX1_blb_32 (bl_bus[32], blb_bus[32]);
 INVTX1 INVTX1_blb_33 (bl_bus[33], blb_bus[33]);
 INVTX1 INVTX1_blb_34 (bl_bus[34], blb_bus[34]);
 INVTX1 INVTX1_blb_35 (bl_bus[35], blb_bus[35]);
 INVTX1 INVTX1_blb_36 (bl_bus[36], blb_bus[36]);
 INVTX1 INVTX1_blb_37 (bl_bus[37], blb_bus[37]);
 INVTX1 INVTX1_blb_38 (bl_bus[38], blb_bus[38]);
 INVTX1 INVTX1_blb_39 (bl_bus[39], blb_bus[39]);
 INVTX1 INVTX1_blb_40 (bl_bus[40], blb_bus[40]);
 INVTX1 INVTX1_blb_41 (bl_bus[41], blb_bus[41]);
 INVTX1 INVTX1_blb_42 (bl_bus[42], blb_bus[42]);
 INVTX1 INVTX1_blb_43 (bl_bus[43], blb_bus[43]);
 INVTX1 INVTX1_blb_44 (bl_bus[44], blb_bus[44]);
 INVTX1 INVTX1_blb_45 (bl_bus[45], blb_bus[45]);
 INVTX1 INVTX1_blb_46 (bl_bus[46], blb_bus[46]);
 INVTX1 INVTX1_blb_47 (bl_bus[47], blb_bus[47]);
 INVTX1 INVTX1_blb_48 (bl_bus[48], blb_bus[48]);
 INVTX1 INVTX1_blb_49 (bl_bus[49], blb_bus[49]);
 INVTX1 INVTX1_blb_50 (bl_bus[50], blb_bus[50]);
  assign sram_blwl_bl[0:51] = bl_bus[0:51];
  assign sram_blwl_blb[0:51] = blb_bus[0:51];
  assign sram_blwl_bl[52:103] = bl_bus[0:51];
  assign sram_blwl_blb[52:103] = blb_bus[0:51];
  assign sram_blwl_bl[104:155] = bl_bus[0:51];
  assign sram_blwl_blb[104:155] = blb_bus[0:51];
  assign sram_blwl_bl[156:207] = bl_bus[0:51];
  assign sram_blwl_blb[156:207] = blb_bus[0:51];
  assign sram_blwl_bl[208:259] = bl_bus[0:51];
  assign sram_blwl_blb[208:259] = blb_bus[0:51];
  assign sram_blwl_bl[260:311] = bl_bus[0:51];
  assign sram_blwl_blb[260:311] = blb_bus[0:51];
  assign sram_blwl_bl[312:363] = bl_bus[0:51];
  assign sram_blwl_blb[312:363] = blb_bus[0:51];
  assign sram_blwl_bl[364:415] = bl_bus[0:51];
  assign sram_blwl_blb[364:415] = blb_bus[0:51];
  assign sram_blwl_bl[416:467] = bl_bus[0:51];
  assign sram_blwl_blb[416:467] = blb_bus[0:51];
  assign sram_blwl_bl[468:519] = bl_bus[0:51];
  assign sram_blwl_blb[468:519] = blb_bus[0:51];
  assign sram_blwl_bl[520:571] = bl_bus[0:51];
  assign sram_blwl_blb[520:571] = blb_bus[0:51];
  assign sram_blwl_bl[572:623] = bl_bus[0:51];
  assign sram_blwl_blb[572:623] = blb_bus[0:51];
  assign sram_blwl_bl[624:675] = bl_bus[0:51];
  assign sram_blwl_blb[624:675] = blb_bus[0:51];
  assign sram_blwl_bl[676:727] = bl_bus[0:51];
  assign sram_blwl_blb[676:727] = blb_bus[0:51];
  assign sram_blwl_bl[728:779] = bl_bus[0:51];
  assign sram_blwl_blb[728:779] = blb_bus[0:51];
  assign sram_blwl_bl[780:831] = bl_bus[0:51];
  assign sram_blwl_blb[780:831] = blb_bus[0:51];
  assign sram_blwl_bl[832:883] = bl_bus[0:51];
  assign sram_blwl_blb[832:883] = blb_bus[0:51];
  assign sram_blwl_bl[884:935] = bl_bus[0:51];
  assign sram_blwl_blb[884:935] = blb_bus[0:51];
  assign sram_blwl_bl[936:987] = bl_bus[0:51];
  assign sram_blwl_blb[936:987] = blb_bus[0:51];
  assign sram_blwl_bl[988:1039] = bl_bus[0:51];
  assign sram_blwl_blb[988:1039] = blb_bus[0:51];
  assign sram_blwl_bl[1040:1091] = bl_bus[0:51];
  assign sram_blwl_blb[1040:1091] = blb_bus[0:51];
  assign sram_blwl_bl[1092:1143] = bl_bus[0:51];
  assign sram_blwl_blb[1092:1143] = blb_bus[0:51];
  assign sram_blwl_bl[1144:1195] = bl_bus[0:51];
  assign sram_blwl_blb[1144:1195] = blb_bus[0:51];
  assign sram_blwl_bl[1196:1247] = bl_bus[0:51];
  assign sram_blwl_blb[1196:1247] = blb_bus[0:51];
  assign sram_blwl_bl[1248:1299] = bl_bus[0:51];
  assign sram_blwl_blb[1248:1299] = blb_bus[0:51];
  assign sram_blwl_bl[1300:1351] = bl_bus[0:51];
  assign sram_blwl_blb[1300:1351] = blb_bus[0:51];
  assign sram_blwl_bl[1352:1403] = bl_bus[0:51];
  assign sram_blwl_blb[1352:1403] = blb_bus[0:51];
  assign sram_blwl_bl[1404:1455] = bl_bus[0:51];
  assign sram_blwl_blb[1404:1455] = blb_bus[0:51];
  assign sram_blwl_bl[1456:1507] = bl_bus[0:51];
  assign sram_blwl_blb[1456:1507] = blb_bus[0:51];
  assign sram_blwl_bl[1508:1559] = bl_bus[0:51];
  assign sram_blwl_blb[1508:1559] = blb_bus[0:51];
  assign sram_blwl_bl[1560:1611] = bl_bus[0:51];
  assign sram_blwl_blb[1560:1611] = blb_bus[0:51];
  assign sram_blwl_bl[1612:1663] = bl_bus[0:51];
  assign sram_blwl_blb[1612:1663] = blb_bus[0:51];
  assign sram_blwl_bl[1664:1715] = bl_bus[0:51];
  assign sram_blwl_blb[1664:1715] = blb_bus[0:51];
  assign sram_blwl_bl[1716:1767] = bl_bus[0:51];
  assign sram_blwl_blb[1716:1767] = blb_bus[0:51];
  assign sram_blwl_bl[1768:1819] = bl_bus[0:51];
  assign sram_blwl_blb[1768:1819] = blb_bus[0:51];
  assign sram_blwl_bl[1820:1871] = bl_bus[0:51];
  assign sram_blwl_blb[1820:1871] = blb_bus[0:51];
  assign sram_blwl_bl[1872:1923] = bl_bus[0:51];
  assign sram_blwl_blb[1872:1923] = blb_bus[0:51];
  assign sram_blwl_bl[1924:1975] = bl_bus[0:51];
  assign sram_blwl_blb[1924:1975] = blb_bus[0:51];
  assign sram_blwl_bl[1976:2027] = bl_bus[0:51];
  assign sram_blwl_blb[1976:2027] = blb_bus[0:51];
  assign sram_blwl_bl[2028:2079] = bl_bus[0:51];
  assign sram_blwl_blb[2028:2079] = blb_bus[0:51];
  assign sram_blwl_bl[2080:2131] = bl_bus[0:51];
  assign sram_blwl_blb[2080:2131] = blb_bus[0:51];
  assign sram_blwl_bl[2132:2183] = bl_bus[0:51];
  assign sram_blwl_blb[2132:2183] = blb_bus[0:51];
  assign sram_blwl_bl[2184:2235] = bl_bus[0:51];
  assign sram_blwl_blb[2184:2235] = blb_bus[0:51];
  assign sram_blwl_bl[2236:2287] = bl_bus[0:51];
  assign sram_blwl_blb[2236:2287] = blb_bus[0:51];
  assign sram_blwl_bl[2288:2339] = bl_bus[0:51];
  assign sram_blwl_blb[2288:2339] = blb_bus[0:51];
  assign sram_blwl_bl[2340:2391] = bl_bus[0:51];
  assign sram_blwl_blb[2340:2391] = blb_bus[0:51];
  assign sram_blwl_bl[2392:2443] = bl_bus[0:51];
  assign sram_blwl_blb[2392:2443] = blb_bus[0:51];
  assign sram_blwl_bl[2444:2495] = bl_bus[0:51];
  assign sram_blwl_blb[2444:2495] = blb_bus[0:51];
  assign sram_blwl_bl[2496:2547] = bl_bus[0:51];
  assign sram_blwl_blb[2496:2547] = blb_bus[0:51];
  assign sram_blwl_bl[2548:2599] = bl_bus[0:51];
  assign sram_blwl_blb[2548:2599] = blb_bus[0:51];
  assign sram_blwl_bl[2600:2651] = bl_bus[0:51];
  assign sram_blwl_blb[2600:2651] = blb_bus[0:51];
  assign sram_blwl_bl[2652:2657] = bl_bus[0:5];
  assign sram_blwl_blb[2652:2657] = blb_bus[0:5];
    assign sram_blwl_wl[0] = wl_bus[0];
    assign sram_blwl_wl[1] = wl_bus[0];
    assign sram_blwl_wl[2] = wl_bus[0];
    assign sram_blwl_wl[3] = wl_bus[0];
    assign sram_blwl_wl[4] = wl_bus[0];
    assign sram_blwl_wl[5] = wl_bus[0];
    assign sram_blwl_wl[6] = wl_bus[0];
    assign sram_blwl_wl[7] = wl_bus[0];
    assign sram_blwl_wl[8] = wl_bus[0];
    assign sram_blwl_wl[9] = wl_bus[0];
    assign sram_blwl_wl[10] = wl_bus[0];
    assign sram_blwl_wl[11] = wl_bus[0];
    assign sram_blwl_wl[12] = wl_bus[0];
    assign sram_blwl_wl[13] = wl_bus[0];
    assign sram_blwl_wl[14] = wl_bus[0];
    assign sram_blwl_wl[15] = wl_bus[0];
    assign sram_blwl_wl[16] = wl_bus[0];
    assign sram_blwl_wl[17] = wl_bus[0];
    assign sram_blwl_wl[18] = wl_bus[0];
    assign sram_blwl_wl[19] = wl_bus[0];
    assign sram_blwl_wl[20] = wl_bus[0];
    assign sram_blwl_wl[21] = wl_bus[0];
    assign sram_blwl_wl[22] = wl_bus[0];
    assign sram_blwl_wl[23] = wl_bus[0];
    assign sram_blwl_wl[24] = wl_bus[0];
    assign sram_blwl_wl[25] = wl_bus[0];
    assign sram_blwl_wl[26] = wl_bus[0];
    assign sram_blwl_wl[27] = wl_bus[0];
    assign sram_blwl_wl[28] = wl_bus[0];
    assign sram_blwl_wl[29] = wl_bus[0];
    assign sram_blwl_wl[30] = wl_bus[0];
    assign sram_blwl_wl[31] = wl_bus[0];
    assign sram_blwl_wl[32] = wl_bus[0];
    assign sram_blwl_wl[33] = wl_bus[0];
    assign sram_blwl_wl[34] = wl_bus[0];
    assign sram_blwl_wl[35] = wl_bus[0];
    assign sram_blwl_wl[36] = wl_bus[0];
    assign sram_blwl_wl[37] = wl_bus[0];
    assign sram_blwl_wl[38] = wl_bus[0];
    assign sram_blwl_wl[39] = wl_bus[0];
    assign sram_blwl_wl[40] = wl_bus[0];
    assign sram_blwl_wl[41] = wl_bus[0];
    assign sram_blwl_wl[42] = wl_bus[0];
    assign sram_blwl_wl[43] = wl_bus[0];
    assign sram_blwl_wl[44] = wl_bus[0];
    assign sram_blwl_wl[45] = wl_bus[0];
    assign sram_blwl_wl[46] = wl_bus[0];
    assign sram_blwl_wl[47] = wl_bus[0];
    assign sram_blwl_wl[48] = wl_bus[0];
    assign sram_blwl_wl[49] = wl_bus[0];
    assign sram_blwl_wl[50] = wl_bus[0];
    assign sram_blwl_wl[51] = wl_bus[0];
    assign sram_blwl_wl[52] = wl_bus[1];
    assign sram_blwl_wl[53] = wl_bus[1];
    assign sram_blwl_wl[54] = wl_bus[1];
    assign sram_blwl_wl[55] = wl_bus[1];
    assign sram_blwl_wl[56] = wl_bus[1];
    assign sram_blwl_wl[57] = wl_bus[1];
    assign sram_blwl_wl[58] = wl_bus[1];
    assign sram_blwl_wl[59] = wl_bus[1];
    assign sram_blwl_wl[60] = wl_bus[1];
    assign sram_blwl_wl[61] = wl_bus[1];
    assign sram_blwl_wl[62] = wl_bus[1];
    assign sram_blwl_wl[63] = wl_bus[1];
    assign sram_blwl_wl[64] = wl_bus[1];
    assign sram_blwl_wl[65] = wl_bus[1];
    assign sram_blwl_wl[66] = wl_bus[1];
    assign sram_blwl_wl[67] = wl_bus[1];
    assign sram_blwl_wl[68] = wl_bus[1];
    assign sram_blwl_wl[69] = wl_bus[1];
    assign sram_blwl_wl[70] = wl_bus[1];
    assign sram_blwl_wl[71] = wl_bus[1];
    assign sram_blwl_wl[72] = wl_bus[1];
    assign sram_blwl_wl[73] = wl_bus[1];
    assign sram_blwl_wl[74] = wl_bus[1];
    assign sram_blwl_wl[75] = wl_bus[1];
    assign sram_blwl_wl[76] = wl_bus[1];
    assign sram_blwl_wl[77] = wl_bus[1];
    assign sram_blwl_wl[78] = wl_bus[1];
    assign sram_blwl_wl[79] = wl_bus[1];
    assign sram_blwl_wl[80] = wl_bus[1];
    assign sram_blwl_wl[81] = wl_bus[1];
    assign sram_blwl_wl[82] = wl_bus[1];
    assign sram_blwl_wl[83] = wl_bus[1];
    assign sram_blwl_wl[84] = wl_bus[1];
    assign sram_blwl_wl[85] = wl_bus[1];
    assign sram_blwl_wl[86] = wl_bus[1];
    assign sram_blwl_wl[87] = wl_bus[1];
    assign sram_blwl_wl[88] = wl_bus[1];
    assign sram_blwl_wl[89] = wl_bus[1];
    assign sram_blwl_wl[90] = wl_bus[1];
    assign sram_blwl_wl[91] = wl_bus[1];
    assign sram_blwl_wl[92] = wl_bus[1];
    assign sram_blwl_wl[93] = wl_bus[1];
    assign sram_blwl_wl[94] = wl_bus[1];
    assign sram_blwl_wl[95] = wl_bus[1];
    assign sram_blwl_wl[96] = wl_bus[1];
    assign sram_blwl_wl[97] = wl_bus[1];
    assign sram_blwl_wl[98] = wl_bus[1];
    assign sram_blwl_wl[99] = wl_bus[1];
    assign sram_blwl_wl[100] = wl_bus[1];
    assign sram_blwl_wl[101] = wl_bus[1];
    assign sram_blwl_wl[102] = wl_bus[1];
    assign sram_blwl_wl[103] = wl_bus[1];
    assign sram_blwl_wl[104] = wl_bus[2];
    assign sram_blwl_wl[105] = wl_bus[2];
    assign sram_blwl_wl[106] = wl_bus[2];
    assign sram_blwl_wl[107] = wl_bus[2];
    assign sram_blwl_wl[108] = wl_bus[2];
    assign sram_blwl_wl[109] = wl_bus[2];
    assign sram_blwl_wl[110] = wl_bus[2];
    assign sram_blwl_wl[111] = wl_bus[2];
    assign sram_blwl_wl[112] = wl_bus[2];
    assign sram_blwl_wl[113] = wl_bus[2];
    assign sram_blwl_wl[114] = wl_bus[2];
    assign sram_blwl_wl[115] = wl_bus[2];
    assign sram_blwl_wl[116] = wl_bus[2];
    assign sram_blwl_wl[117] = wl_bus[2];
    assign sram_blwl_wl[118] = wl_bus[2];
    assign sram_blwl_wl[119] = wl_bus[2];
    assign sram_blwl_wl[120] = wl_bus[2];
    assign sram_blwl_wl[121] = wl_bus[2];
    assign sram_blwl_wl[122] = wl_bus[2];
    assign sram_blwl_wl[123] = wl_bus[2];
    assign sram_blwl_wl[124] = wl_bus[2];
    assign sram_blwl_wl[125] = wl_bus[2];
    assign sram_blwl_wl[126] = wl_bus[2];
    assign sram_blwl_wl[127] = wl_bus[2];
    assign sram_blwl_wl[128] = wl_bus[2];
    assign sram_blwl_wl[129] = wl_bus[2];
    assign sram_blwl_wl[130] = wl_bus[2];
    assign sram_blwl_wl[131] = wl_bus[2];
    assign sram_blwl_wl[132] = wl_bus[2];
    assign sram_blwl_wl[133] = wl_bus[2];
    assign sram_blwl_wl[134] = wl_bus[2];
    assign sram_blwl_wl[135] = wl_bus[2];
    assign sram_blwl_wl[136] = wl_bus[2];
    assign sram_blwl_wl[137] = wl_bus[2];
    assign sram_blwl_wl[138] = wl_bus[2];
    assign sram_blwl_wl[139] = wl_bus[2];
    assign sram_blwl_wl[140] = wl_bus[2];
    assign sram_blwl_wl[141] = wl_bus[2];
    assign sram_blwl_wl[142] = wl_bus[2];
    assign sram_blwl_wl[143] = wl_bus[2];
    assign sram_blwl_wl[144] = wl_bus[2];
    assign sram_blwl_wl[145] = wl_bus[2];
    assign sram_blwl_wl[146] = wl_bus[2];
    assign sram_blwl_wl[147] = wl_bus[2];
    assign sram_blwl_wl[148] = wl_bus[2];
    assign sram_blwl_wl[149] = wl_bus[2];
    assign sram_blwl_wl[150] = wl_bus[2];
    assign sram_blwl_wl[151] = wl_bus[2];
    assign sram_blwl_wl[152] = wl_bus[2];
    assign sram_blwl_wl[153] = wl_bus[2];
    assign sram_blwl_wl[154] = wl_bus[2];
    assign sram_blwl_wl[155] = wl_bus[2];
    assign sram_blwl_wl[156] = wl_bus[3];
    assign sram_blwl_wl[157] = wl_bus[3];
    assign sram_blwl_wl[158] = wl_bus[3];
    assign sram_blwl_wl[159] = wl_bus[3];
    assign sram_blwl_wl[160] = wl_bus[3];
    assign sram_blwl_wl[161] = wl_bus[3];
    assign sram_blwl_wl[162] = wl_bus[3];
    assign sram_blwl_wl[163] = wl_bus[3];
    assign sram_blwl_wl[164] = wl_bus[3];
    assign sram_blwl_wl[165] = wl_bus[3];
    assign sram_blwl_wl[166] = wl_bus[3];
    assign sram_blwl_wl[167] = wl_bus[3];
    assign sram_blwl_wl[168] = wl_bus[3];
    assign sram_blwl_wl[169] = wl_bus[3];
    assign sram_blwl_wl[170] = wl_bus[3];
    assign sram_blwl_wl[171] = wl_bus[3];
    assign sram_blwl_wl[172] = wl_bus[3];
    assign sram_blwl_wl[173] = wl_bus[3];
    assign sram_blwl_wl[174] = wl_bus[3];
    assign sram_blwl_wl[175] = wl_bus[3];
    assign sram_blwl_wl[176] = wl_bus[3];
    assign sram_blwl_wl[177] = wl_bus[3];
    assign sram_blwl_wl[178] = wl_bus[3];
    assign sram_blwl_wl[179] = wl_bus[3];
    assign sram_blwl_wl[180] = wl_bus[3];
    assign sram_blwl_wl[181] = wl_bus[3];
    assign sram_blwl_wl[182] = wl_bus[3];
    assign sram_blwl_wl[183] = wl_bus[3];
    assign sram_blwl_wl[184] = wl_bus[3];
    assign sram_blwl_wl[185] = wl_bus[3];
    assign sram_blwl_wl[186] = wl_bus[3];
    assign sram_blwl_wl[187] = wl_bus[3];
    assign sram_blwl_wl[188] = wl_bus[3];
    assign sram_blwl_wl[189] = wl_bus[3];
    assign sram_blwl_wl[190] = wl_bus[3];
    assign sram_blwl_wl[191] = wl_bus[3];
    assign sram_blwl_wl[192] = wl_bus[3];
    assign sram_blwl_wl[193] = wl_bus[3];
    assign sram_blwl_wl[194] = wl_bus[3];
    assign sram_blwl_wl[195] = wl_bus[3];
    assign sram_blwl_wl[196] = wl_bus[3];
    assign sram_blwl_wl[197] = wl_bus[3];
    assign sram_blwl_wl[198] = wl_bus[3];
    assign sram_blwl_wl[199] = wl_bus[3];
    assign sram_blwl_wl[200] = wl_bus[3];
    assign sram_blwl_wl[201] = wl_bus[3];
    assign sram_blwl_wl[202] = wl_bus[3];
    assign sram_blwl_wl[203] = wl_bus[3];
    assign sram_blwl_wl[204] = wl_bus[3];
    assign sram_blwl_wl[205] = wl_bus[3];
    assign sram_blwl_wl[206] = wl_bus[3];
    assign sram_blwl_wl[207] = wl_bus[3];
    assign sram_blwl_wl[208] = wl_bus[4];
    assign sram_blwl_wl[209] = wl_bus[4];
    assign sram_blwl_wl[210] = wl_bus[4];
    assign sram_blwl_wl[211] = wl_bus[4];
    assign sram_blwl_wl[212] = wl_bus[4];
    assign sram_blwl_wl[213] = wl_bus[4];
    assign sram_blwl_wl[214] = wl_bus[4];
    assign sram_blwl_wl[215] = wl_bus[4];
    assign sram_blwl_wl[216] = wl_bus[4];
    assign sram_blwl_wl[217] = wl_bus[4];
    assign sram_blwl_wl[218] = wl_bus[4];
    assign sram_blwl_wl[219] = wl_bus[4];
    assign sram_blwl_wl[220] = wl_bus[4];
    assign sram_blwl_wl[221] = wl_bus[4];
    assign sram_blwl_wl[222] = wl_bus[4];
    assign sram_blwl_wl[223] = wl_bus[4];
    assign sram_blwl_wl[224] = wl_bus[4];
    assign sram_blwl_wl[225] = wl_bus[4];
    assign sram_blwl_wl[226] = wl_bus[4];
    assign sram_blwl_wl[227] = wl_bus[4];
    assign sram_blwl_wl[228] = wl_bus[4];
    assign sram_blwl_wl[229] = wl_bus[4];
    assign sram_blwl_wl[230] = wl_bus[4];
    assign sram_blwl_wl[231] = wl_bus[4];
    assign sram_blwl_wl[232] = wl_bus[4];
    assign sram_blwl_wl[233] = wl_bus[4];
    assign sram_blwl_wl[234] = wl_bus[4];
    assign sram_blwl_wl[235] = wl_bus[4];
    assign sram_blwl_wl[236] = wl_bus[4];
    assign sram_blwl_wl[237] = wl_bus[4];
    assign sram_blwl_wl[238] = wl_bus[4];
    assign sram_blwl_wl[239] = wl_bus[4];
    assign sram_blwl_wl[240] = wl_bus[4];
    assign sram_blwl_wl[241] = wl_bus[4];
    assign sram_blwl_wl[242] = wl_bus[4];
    assign sram_blwl_wl[243] = wl_bus[4];
    assign sram_blwl_wl[244] = wl_bus[4];
    assign sram_blwl_wl[245] = wl_bus[4];
    assign sram_blwl_wl[246] = wl_bus[4];
    assign sram_blwl_wl[247] = wl_bus[4];
    assign sram_blwl_wl[248] = wl_bus[4];
    assign sram_blwl_wl[249] = wl_bus[4];
    assign sram_blwl_wl[250] = wl_bus[4];
    assign sram_blwl_wl[251] = wl_bus[4];
    assign sram_blwl_wl[252] = wl_bus[4];
    assign sram_blwl_wl[253] = wl_bus[4];
    assign sram_blwl_wl[254] = wl_bus[4];
    assign sram_blwl_wl[255] = wl_bus[4];
    assign sram_blwl_wl[256] = wl_bus[4];
    assign sram_blwl_wl[257] = wl_bus[4];
    assign sram_blwl_wl[258] = wl_bus[4];
    assign sram_blwl_wl[259] = wl_bus[4];
    assign sram_blwl_wl[260] = wl_bus[5];
    assign sram_blwl_wl[261] = wl_bus[5];
    assign sram_blwl_wl[262] = wl_bus[5];
    assign sram_blwl_wl[263] = wl_bus[5];
    assign sram_blwl_wl[264] = wl_bus[5];
    assign sram_blwl_wl[265] = wl_bus[5];
    assign sram_blwl_wl[266] = wl_bus[5];
    assign sram_blwl_wl[267] = wl_bus[5];
    assign sram_blwl_wl[268] = wl_bus[5];
    assign sram_blwl_wl[269] = wl_bus[5];
    assign sram_blwl_wl[270] = wl_bus[5];
    assign sram_blwl_wl[271] = wl_bus[5];
    assign sram_blwl_wl[272] = wl_bus[5];
    assign sram_blwl_wl[273] = wl_bus[5];
    assign sram_blwl_wl[274] = wl_bus[5];
    assign sram_blwl_wl[275] = wl_bus[5];
    assign sram_blwl_wl[276] = wl_bus[5];
    assign sram_blwl_wl[277] = wl_bus[5];
    assign sram_blwl_wl[278] = wl_bus[5];
    assign sram_blwl_wl[279] = wl_bus[5];
    assign sram_blwl_wl[280] = wl_bus[5];
    assign sram_blwl_wl[281] = wl_bus[5];
    assign sram_blwl_wl[282] = wl_bus[5];
    assign sram_blwl_wl[283] = wl_bus[5];
    assign sram_blwl_wl[284] = wl_bus[5];
    assign sram_blwl_wl[285] = wl_bus[5];
    assign sram_blwl_wl[286] = wl_bus[5];
    assign sram_blwl_wl[287] = wl_bus[5];
    assign sram_blwl_wl[288] = wl_bus[5];
    assign sram_blwl_wl[289] = wl_bus[5];
    assign sram_blwl_wl[290] = wl_bus[5];
    assign sram_blwl_wl[291] = wl_bus[5];
    assign sram_blwl_wl[292] = wl_bus[5];
    assign sram_blwl_wl[293] = wl_bus[5];
    assign sram_blwl_wl[294] = wl_bus[5];
    assign sram_blwl_wl[295] = wl_bus[5];
    assign sram_blwl_wl[296] = wl_bus[5];
    assign sram_blwl_wl[297] = wl_bus[5];
    assign sram_blwl_wl[298] = wl_bus[5];
    assign sram_blwl_wl[299] = wl_bus[5];
    assign sram_blwl_wl[300] = wl_bus[5];
    assign sram_blwl_wl[301] = wl_bus[5];
    assign sram_blwl_wl[302] = wl_bus[5];
    assign sram_blwl_wl[303] = wl_bus[5];
    assign sram_blwl_wl[304] = wl_bus[5];
    assign sram_blwl_wl[305] = wl_bus[5];
    assign sram_blwl_wl[306] = wl_bus[5];
    assign sram_blwl_wl[307] = wl_bus[5];
    assign sram_blwl_wl[308] = wl_bus[5];
    assign sram_blwl_wl[309] = wl_bus[5];
    assign sram_blwl_wl[310] = wl_bus[5];
    assign sram_blwl_wl[311] = wl_bus[5];
    assign sram_blwl_wl[312] = wl_bus[6];
    assign sram_blwl_wl[313] = wl_bus[6];
    assign sram_blwl_wl[314] = wl_bus[6];
    assign sram_blwl_wl[315] = wl_bus[6];
    assign sram_blwl_wl[316] = wl_bus[6];
    assign sram_blwl_wl[317] = wl_bus[6];
    assign sram_blwl_wl[318] = wl_bus[6];
    assign sram_blwl_wl[319] = wl_bus[6];
    assign sram_blwl_wl[320] = wl_bus[6];
    assign sram_blwl_wl[321] = wl_bus[6];
    assign sram_blwl_wl[322] = wl_bus[6];
    assign sram_blwl_wl[323] = wl_bus[6];
    assign sram_blwl_wl[324] = wl_bus[6];
    assign sram_blwl_wl[325] = wl_bus[6];
    assign sram_blwl_wl[326] = wl_bus[6];
    assign sram_blwl_wl[327] = wl_bus[6];
    assign sram_blwl_wl[328] = wl_bus[6];
    assign sram_blwl_wl[329] = wl_bus[6];
    assign sram_blwl_wl[330] = wl_bus[6];
    assign sram_blwl_wl[331] = wl_bus[6];
    assign sram_blwl_wl[332] = wl_bus[6];
    assign sram_blwl_wl[333] = wl_bus[6];
    assign sram_blwl_wl[334] = wl_bus[6];
    assign sram_blwl_wl[335] = wl_bus[6];
    assign sram_blwl_wl[336] = wl_bus[6];
    assign sram_blwl_wl[337] = wl_bus[6];
    assign sram_blwl_wl[338] = wl_bus[6];
    assign sram_blwl_wl[339] = wl_bus[6];
    assign sram_blwl_wl[340] = wl_bus[6];
    assign sram_blwl_wl[341] = wl_bus[6];
    assign sram_blwl_wl[342] = wl_bus[6];
    assign sram_blwl_wl[343] = wl_bus[6];
    assign sram_blwl_wl[344] = wl_bus[6];
    assign sram_blwl_wl[345] = wl_bus[6];
    assign sram_blwl_wl[346] = wl_bus[6];
    assign sram_blwl_wl[347] = wl_bus[6];
    assign sram_blwl_wl[348] = wl_bus[6];
    assign sram_blwl_wl[349] = wl_bus[6];
    assign sram_blwl_wl[350] = wl_bus[6];
    assign sram_blwl_wl[351] = wl_bus[6];
    assign sram_blwl_wl[352] = wl_bus[6];
    assign sram_blwl_wl[353] = wl_bus[6];
    assign sram_blwl_wl[354] = wl_bus[6];
    assign sram_blwl_wl[355] = wl_bus[6];
    assign sram_blwl_wl[356] = wl_bus[6];
    assign sram_blwl_wl[357] = wl_bus[6];
    assign sram_blwl_wl[358] = wl_bus[6];
    assign sram_blwl_wl[359] = wl_bus[6];
    assign sram_blwl_wl[360] = wl_bus[6];
    assign sram_blwl_wl[361] = wl_bus[6];
    assign sram_blwl_wl[362] = wl_bus[6];
    assign sram_blwl_wl[363] = wl_bus[6];
    assign sram_blwl_wl[364] = wl_bus[7];
    assign sram_blwl_wl[365] = wl_bus[7];
    assign sram_blwl_wl[366] = wl_bus[7];
    assign sram_blwl_wl[367] = wl_bus[7];
    assign sram_blwl_wl[368] = wl_bus[7];
    assign sram_blwl_wl[369] = wl_bus[7];
    assign sram_blwl_wl[370] = wl_bus[7];
    assign sram_blwl_wl[371] = wl_bus[7];
    assign sram_blwl_wl[372] = wl_bus[7];
    assign sram_blwl_wl[373] = wl_bus[7];
    assign sram_blwl_wl[374] = wl_bus[7];
    assign sram_blwl_wl[375] = wl_bus[7];
    assign sram_blwl_wl[376] = wl_bus[7];
    assign sram_blwl_wl[377] = wl_bus[7];
    assign sram_blwl_wl[378] = wl_bus[7];
    assign sram_blwl_wl[379] = wl_bus[7];
    assign sram_blwl_wl[380] = wl_bus[7];
    assign sram_blwl_wl[381] = wl_bus[7];
    assign sram_blwl_wl[382] = wl_bus[7];
    assign sram_blwl_wl[383] = wl_bus[7];
    assign sram_blwl_wl[384] = wl_bus[7];
    assign sram_blwl_wl[385] = wl_bus[7];
    assign sram_blwl_wl[386] = wl_bus[7];
    assign sram_blwl_wl[387] = wl_bus[7];
    assign sram_blwl_wl[388] = wl_bus[7];
    assign sram_blwl_wl[389] = wl_bus[7];
    assign sram_blwl_wl[390] = wl_bus[7];
    assign sram_blwl_wl[391] = wl_bus[7];
    assign sram_blwl_wl[392] = wl_bus[7];
    assign sram_blwl_wl[393] = wl_bus[7];
    assign sram_blwl_wl[394] = wl_bus[7];
    assign sram_blwl_wl[395] = wl_bus[7];
    assign sram_blwl_wl[396] = wl_bus[7];
    assign sram_blwl_wl[397] = wl_bus[7];
    assign sram_blwl_wl[398] = wl_bus[7];
    assign sram_blwl_wl[399] = wl_bus[7];
    assign sram_blwl_wl[400] = wl_bus[7];
    assign sram_blwl_wl[401] = wl_bus[7];
    assign sram_blwl_wl[402] = wl_bus[7];
    assign sram_blwl_wl[403] = wl_bus[7];
    assign sram_blwl_wl[404] = wl_bus[7];
    assign sram_blwl_wl[405] = wl_bus[7];
    assign sram_blwl_wl[406] = wl_bus[7];
    assign sram_blwl_wl[407] = wl_bus[7];
    assign sram_blwl_wl[408] = wl_bus[7];
    assign sram_blwl_wl[409] = wl_bus[7];
    assign sram_blwl_wl[410] = wl_bus[7];
    assign sram_blwl_wl[411] = wl_bus[7];
    assign sram_blwl_wl[412] = wl_bus[7];
    assign sram_blwl_wl[413] = wl_bus[7];
    assign sram_blwl_wl[414] = wl_bus[7];
    assign sram_blwl_wl[415] = wl_bus[7];
    assign sram_blwl_wl[416] = wl_bus[8];
    assign sram_blwl_wl[417] = wl_bus[8];
    assign sram_blwl_wl[418] = wl_bus[8];
    assign sram_blwl_wl[419] = wl_bus[8];
    assign sram_blwl_wl[420] = wl_bus[8];
    assign sram_blwl_wl[421] = wl_bus[8];
    assign sram_blwl_wl[422] = wl_bus[8];
    assign sram_blwl_wl[423] = wl_bus[8];
    assign sram_blwl_wl[424] = wl_bus[8];
    assign sram_blwl_wl[425] = wl_bus[8];
    assign sram_blwl_wl[426] = wl_bus[8];
    assign sram_blwl_wl[427] = wl_bus[8];
    assign sram_blwl_wl[428] = wl_bus[8];
    assign sram_blwl_wl[429] = wl_bus[8];
    assign sram_blwl_wl[430] = wl_bus[8];
    assign sram_blwl_wl[431] = wl_bus[8];
    assign sram_blwl_wl[432] = wl_bus[8];
    assign sram_blwl_wl[433] = wl_bus[8];
    assign sram_blwl_wl[434] = wl_bus[8];
    assign sram_blwl_wl[435] = wl_bus[8];
    assign sram_blwl_wl[436] = wl_bus[8];
    assign sram_blwl_wl[437] = wl_bus[8];
    assign sram_blwl_wl[438] = wl_bus[8];
    assign sram_blwl_wl[439] = wl_bus[8];
    assign sram_blwl_wl[440] = wl_bus[8];
    assign sram_blwl_wl[441] = wl_bus[8];
    assign sram_blwl_wl[442] = wl_bus[8];
    assign sram_blwl_wl[443] = wl_bus[8];
    assign sram_blwl_wl[444] = wl_bus[8];
    assign sram_blwl_wl[445] = wl_bus[8];
    assign sram_blwl_wl[446] = wl_bus[8];
    assign sram_blwl_wl[447] = wl_bus[8];
    assign sram_blwl_wl[448] = wl_bus[8];
    assign sram_blwl_wl[449] = wl_bus[8];
    assign sram_blwl_wl[450] = wl_bus[8];
    assign sram_blwl_wl[451] = wl_bus[8];
    assign sram_blwl_wl[452] = wl_bus[8];
    assign sram_blwl_wl[453] = wl_bus[8];
    assign sram_blwl_wl[454] = wl_bus[8];
    assign sram_blwl_wl[455] = wl_bus[8];
    assign sram_blwl_wl[456] = wl_bus[8];
    assign sram_blwl_wl[457] = wl_bus[8];
    assign sram_blwl_wl[458] = wl_bus[8];
    assign sram_blwl_wl[459] = wl_bus[8];
    assign sram_blwl_wl[460] = wl_bus[8];
    assign sram_blwl_wl[461] = wl_bus[8];
    assign sram_blwl_wl[462] = wl_bus[8];
    assign sram_blwl_wl[463] = wl_bus[8];
    assign sram_blwl_wl[464] = wl_bus[8];
    assign sram_blwl_wl[465] = wl_bus[8];
    assign sram_blwl_wl[466] = wl_bus[8];
    assign sram_blwl_wl[467] = wl_bus[8];
    assign sram_blwl_wl[468] = wl_bus[9];
    assign sram_blwl_wl[469] = wl_bus[9];
    assign sram_blwl_wl[470] = wl_bus[9];
    assign sram_blwl_wl[471] = wl_bus[9];
    assign sram_blwl_wl[472] = wl_bus[9];
    assign sram_blwl_wl[473] = wl_bus[9];
    assign sram_blwl_wl[474] = wl_bus[9];
    assign sram_blwl_wl[475] = wl_bus[9];
    assign sram_blwl_wl[476] = wl_bus[9];
    assign sram_blwl_wl[477] = wl_bus[9];
    assign sram_blwl_wl[478] = wl_bus[9];
    assign sram_blwl_wl[479] = wl_bus[9];
    assign sram_blwl_wl[480] = wl_bus[9];
    assign sram_blwl_wl[481] = wl_bus[9];
    assign sram_blwl_wl[482] = wl_bus[9];
    assign sram_blwl_wl[483] = wl_bus[9];
    assign sram_blwl_wl[484] = wl_bus[9];
    assign sram_blwl_wl[485] = wl_bus[9];
    assign sram_blwl_wl[486] = wl_bus[9];
    assign sram_blwl_wl[487] = wl_bus[9];
    assign sram_blwl_wl[488] = wl_bus[9];
    assign sram_blwl_wl[489] = wl_bus[9];
    assign sram_blwl_wl[490] = wl_bus[9];
    assign sram_blwl_wl[491] = wl_bus[9];
    assign sram_blwl_wl[492] = wl_bus[9];
    assign sram_blwl_wl[493] = wl_bus[9];
    assign sram_blwl_wl[494] = wl_bus[9];
    assign sram_blwl_wl[495] = wl_bus[9];
    assign sram_blwl_wl[496] = wl_bus[9];
    assign sram_blwl_wl[497] = wl_bus[9];
    assign sram_blwl_wl[498] = wl_bus[9];
    assign sram_blwl_wl[499] = wl_bus[9];
    assign sram_blwl_wl[500] = wl_bus[9];
    assign sram_blwl_wl[501] = wl_bus[9];
    assign sram_blwl_wl[502] = wl_bus[9];
    assign sram_blwl_wl[503] = wl_bus[9];
    assign sram_blwl_wl[504] = wl_bus[9];
    assign sram_blwl_wl[505] = wl_bus[9];
    assign sram_blwl_wl[506] = wl_bus[9];
    assign sram_blwl_wl[507] = wl_bus[9];
    assign sram_blwl_wl[508] = wl_bus[9];
    assign sram_blwl_wl[509] = wl_bus[9];
    assign sram_blwl_wl[510] = wl_bus[9];
    assign sram_blwl_wl[511] = wl_bus[9];
    assign sram_blwl_wl[512] = wl_bus[9];
    assign sram_blwl_wl[513] = wl_bus[9];
    assign sram_blwl_wl[514] = wl_bus[9];
    assign sram_blwl_wl[515] = wl_bus[9];
    assign sram_blwl_wl[516] = wl_bus[9];
    assign sram_blwl_wl[517] = wl_bus[9];
    assign sram_blwl_wl[518] = wl_bus[9];
    assign sram_blwl_wl[519] = wl_bus[9];
    assign sram_blwl_wl[520] = wl_bus[10];
    assign sram_blwl_wl[521] = wl_bus[10];
    assign sram_blwl_wl[522] = wl_bus[10];
    assign sram_blwl_wl[523] = wl_bus[10];
    assign sram_blwl_wl[524] = wl_bus[10];
    assign sram_blwl_wl[525] = wl_bus[10];
    assign sram_blwl_wl[526] = wl_bus[10];
    assign sram_blwl_wl[527] = wl_bus[10];
    assign sram_blwl_wl[528] = wl_bus[10];
    assign sram_blwl_wl[529] = wl_bus[10];
    assign sram_blwl_wl[530] = wl_bus[10];
    assign sram_blwl_wl[531] = wl_bus[10];
    assign sram_blwl_wl[532] = wl_bus[10];
    assign sram_blwl_wl[533] = wl_bus[10];
    assign sram_blwl_wl[534] = wl_bus[10];
    assign sram_blwl_wl[535] = wl_bus[10];
    assign sram_blwl_wl[536] = wl_bus[10];
    assign sram_blwl_wl[537] = wl_bus[10];
    assign sram_blwl_wl[538] = wl_bus[10];
    assign sram_blwl_wl[539] = wl_bus[10];
    assign sram_blwl_wl[540] = wl_bus[10];
    assign sram_blwl_wl[541] = wl_bus[10];
    assign sram_blwl_wl[542] = wl_bus[10];
    assign sram_blwl_wl[543] = wl_bus[10];
    assign sram_blwl_wl[544] = wl_bus[10];
    assign sram_blwl_wl[545] = wl_bus[10];
    assign sram_blwl_wl[546] = wl_bus[10];
    assign sram_blwl_wl[547] = wl_bus[10];
    assign sram_blwl_wl[548] = wl_bus[10];
    assign sram_blwl_wl[549] = wl_bus[10];
    assign sram_blwl_wl[550] = wl_bus[10];
    assign sram_blwl_wl[551] = wl_bus[10];
    assign sram_blwl_wl[552] = wl_bus[10];
    assign sram_blwl_wl[553] = wl_bus[10];
    assign sram_blwl_wl[554] = wl_bus[10];
    assign sram_blwl_wl[555] = wl_bus[10];
    assign sram_blwl_wl[556] = wl_bus[10];
    assign sram_blwl_wl[557] = wl_bus[10];
    assign sram_blwl_wl[558] = wl_bus[10];
    assign sram_blwl_wl[559] = wl_bus[10];
    assign sram_blwl_wl[560] = wl_bus[10];
    assign sram_blwl_wl[561] = wl_bus[10];
    assign sram_blwl_wl[562] = wl_bus[10];
    assign sram_blwl_wl[563] = wl_bus[10];
    assign sram_blwl_wl[564] = wl_bus[10];
    assign sram_blwl_wl[565] = wl_bus[10];
    assign sram_blwl_wl[566] = wl_bus[10];
    assign sram_blwl_wl[567] = wl_bus[10];
    assign sram_blwl_wl[568] = wl_bus[10];
    assign sram_blwl_wl[569] = wl_bus[10];
    assign sram_blwl_wl[570] = wl_bus[10];
    assign sram_blwl_wl[571] = wl_bus[10];
    assign sram_blwl_wl[572] = wl_bus[11];
    assign sram_blwl_wl[573] = wl_bus[11];
    assign sram_blwl_wl[574] = wl_bus[11];
    assign sram_blwl_wl[575] = wl_bus[11];
    assign sram_blwl_wl[576] = wl_bus[11];
    assign sram_blwl_wl[577] = wl_bus[11];
    assign sram_blwl_wl[578] = wl_bus[11];
    assign sram_blwl_wl[579] = wl_bus[11];
    assign sram_blwl_wl[580] = wl_bus[11];
    assign sram_blwl_wl[581] = wl_bus[11];
    assign sram_blwl_wl[582] = wl_bus[11];
    assign sram_blwl_wl[583] = wl_bus[11];
    assign sram_blwl_wl[584] = wl_bus[11];
    assign sram_blwl_wl[585] = wl_bus[11];
    assign sram_blwl_wl[586] = wl_bus[11];
    assign sram_blwl_wl[587] = wl_bus[11];
    assign sram_blwl_wl[588] = wl_bus[11];
    assign sram_blwl_wl[589] = wl_bus[11];
    assign sram_blwl_wl[590] = wl_bus[11];
    assign sram_blwl_wl[591] = wl_bus[11];
    assign sram_blwl_wl[592] = wl_bus[11];
    assign sram_blwl_wl[593] = wl_bus[11];
    assign sram_blwl_wl[594] = wl_bus[11];
    assign sram_blwl_wl[595] = wl_bus[11];
    assign sram_blwl_wl[596] = wl_bus[11];
    assign sram_blwl_wl[597] = wl_bus[11];
    assign sram_blwl_wl[598] = wl_bus[11];
    assign sram_blwl_wl[599] = wl_bus[11];
    assign sram_blwl_wl[600] = wl_bus[11];
    assign sram_blwl_wl[601] = wl_bus[11];
    assign sram_blwl_wl[602] = wl_bus[11];
    assign sram_blwl_wl[603] = wl_bus[11];
    assign sram_blwl_wl[604] = wl_bus[11];
    assign sram_blwl_wl[605] = wl_bus[11];
    assign sram_blwl_wl[606] = wl_bus[11];
    assign sram_blwl_wl[607] = wl_bus[11];
    assign sram_blwl_wl[608] = wl_bus[11];
    assign sram_blwl_wl[609] = wl_bus[11];
    assign sram_blwl_wl[610] = wl_bus[11];
    assign sram_blwl_wl[611] = wl_bus[11];
    assign sram_blwl_wl[612] = wl_bus[11];
    assign sram_blwl_wl[613] = wl_bus[11];
    assign sram_blwl_wl[614] = wl_bus[11];
    assign sram_blwl_wl[615] = wl_bus[11];
    assign sram_blwl_wl[616] = wl_bus[11];
    assign sram_blwl_wl[617] = wl_bus[11];
    assign sram_blwl_wl[618] = wl_bus[11];
    assign sram_blwl_wl[619] = wl_bus[11];
    assign sram_blwl_wl[620] = wl_bus[11];
    assign sram_blwl_wl[621] = wl_bus[11];
    assign sram_blwl_wl[622] = wl_bus[11];
    assign sram_blwl_wl[623] = wl_bus[11];
    assign sram_blwl_wl[624] = wl_bus[12];
    assign sram_blwl_wl[625] = wl_bus[12];
    assign sram_blwl_wl[626] = wl_bus[12];
    assign sram_blwl_wl[627] = wl_bus[12];
    assign sram_blwl_wl[628] = wl_bus[12];
    assign sram_blwl_wl[629] = wl_bus[12];
    assign sram_blwl_wl[630] = wl_bus[12];
    assign sram_blwl_wl[631] = wl_bus[12];
    assign sram_blwl_wl[632] = wl_bus[12];
    assign sram_blwl_wl[633] = wl_bus[12];
    assign sram_blwl_wl[634] = wl_bus[12];
    assign sram_blwl_wl[635] = wl_bus[12];
    assign sram_blwl_wl[636] = wl_bus[12];
    assign sram_blwl_wl[637] = wl_bus[12];
    assign sram_blwl_wl[638] = wl_bus[12];
    assign sram_blwl_wl[639] = wl_bus[12];
    assign sram_blwl_wl[640] = wl_bus[12];
    assign sram_blwl_wl[641] = wl_bus[12];
    assign sram_blwl_wl[642] = wl_bus[12];
    assign sram_blwl_wl[643] = wl_bus[12];
    assign sram_blwl_wl[644] = wl_bus[12];
    assign sram_blwl_wl[645] = wl_bus[12];
    assign sram_blwl_wl[646] = wl_bus[12];
    assign sram_blwl_wl[647] = wl_bus[12];
    assign sram_blwl_wl[648] = wl_bus[12];
    assign sram_blwl_wl[649] = wl_bus[12];
    assign sram_blwl_wl[650] = wl_bus[12];
    assign sram_blwl_wl[651] = wl_bus[12];
    assign sram_blwl_wl[652] = wl_bus[12];
    assign sram_blwl_wl[653] = wl_bus[12];
    assign sram_blwl_wl[654] = wl_bus[12];
    assign sram_blwl_wl[655] = wl_bus[12];
    assign sram_blwl_wl[656] = wl_bus[12];
    assign sram_blwl_wl[657] = wl_bus[12];
    assign sram_blwl_wl[658] = wl_bus[12];
    assign sram_blwl_wl[659] = wl_bus[12];
    assign sram_blwl_wl[660] = wl_bus[12];
    assign sram_blwl_wl[661] = wl_bus[12];
    assign sram_blwl_wl[662] = wl_bus[12];
    assign sram_blwl_wl[663] = wl_bus[12];
    assign sram_blwl_wl[664] = wl_bus[12];
    assign sram_blwl_wl[665] = wl_bus[12];
    assign sram_blwl_wl[666] = wl_bus[12];
    assign sram_blwl_wl[667] = wl_bus[12];
    assign sram_blwl_wl[668] = wl_bus[12];
    assign sram_blwl_wl[669] = wl_bus[12];
    assign sram_blwl_wl[670] = wl_bus[12];
    assign sram_blwl_wl[671] = wl_bus[12];
    assign sram_blwl_wl[672] = wl_bus[12];
    assign sram_blwl_wl[673] = wl_bus[12];
    assign sram_blwl_wl[674] = wl_bus[12];
    assign sram_blwl_wl[675] = wl_bus[12];
    assign sram_blwl_wl[676] = wl_bus[13];
    assign sram_blwl_wl[677] = wl_bus[13];
    assign sram_blwl_wl[678] = wl_bus[13];
    assign sram_blwl_wl[679] = wl_bus[13];
    assign sram_blwl_wl[680] = wl_bus[13];
    assign sram_blwl_wl[681] = wl_bus[13];
    assign sram_blwl_wl[682] = wl_bus[13];
    assign sram_blwl_wl[683] = wl_bus[13];
    assign sram_blwl_wl[684] = wl_bus[13];
    assign sram_blwl_wl[685] = wl_bus[13];
    assign sram_blwl_wl[686] = wl_bus[13];
    assign sram_blwl_wl[687] = wl_bus[13];
    assign sram_blwl_wl[688] = wl_bus[13];
    assign sram_blwl_wl[689] = wl_bus[13];
    assign sram_blwl_wl[690] = wl_bus[13];
    assign sram_blwl_wl[691] = wl_bus[13];
    assign sram_blwl_wl[692] = wl_bus[13];
    assign sram_blwl_wl[693] = wl_bus[13];
    assign sram_blwl_wl[694] = wl_bus[13];
    assign sram_blwl_wl[695] = wl_bus[13];
    assign sram_blwl_wl[696] = wl_bus[13];
    assign sram_blwl_wl[697] = wl_bus[13];
    assign sram_blwl_wl[698] = wl_bus[13];
    assign sram_blwl_wl[699] = wl_bus[13];
    assign sram_blwl_wl[700] = wl_bus[13];
    assign sram_blwl_wl[701] = wl_bus[13];
    assign sram_blwl_wl[702] = wl_bus[13];
    assign sram_blwl_wl[703] = wl_bus[13];
    assign sram_blwl_wl[704] = wl_bus[13];
    assign sram_blwl_wl[705] = wl_bus[13];
    assign sram_blwl_wl[706] = wl_bus[13];
    assign sram_blwl_wl[707] = wl_bus[13];
    assign sram_blwl_wl[708] = wl_bus[13];
    assign sram_blwl_wl[709] = wl_bus[13];
    assign sram_blwl_wl[710] = wl_bus[13];
    assign sram_blwl_wl[711] = wl_bus[13];
    assign sram_blwl_wl[712] = wl_bus[13];
    assign sram_blwl_wl[713] = wl_bus[13];
    assign sram_blwl_wl[714] = wl_bus[13];
    assign sram_blwl_wl[715] = wl_bus[13];
    assign sram_blwl_wl[716] = wl_bus[13];
    assign sram_blwl_wl[717] = wl_bus[13];
    assign sram_blwl_wl[718] = wl_bus[13];
    assign sram_blwl_wl[719] = wl_bus[13];
    assign sram_blwl_wl[720] = wl_bus[13];
    assign sram_blwl_wl[721] = wl_bus[13];
    assign sram_blwl_wl[722] = wl_bus[13];
    assign sram_blwl_wl[723] = wl_bus[13];
    assign sram_blwl_wl[724] = wl_bus[13];
    assign sram_blwl_wl[725] = wl_bus[13];
    assign sram_blwl_wl[726] = wl_bus[13];
    assign sram_blwl_wl[727] = wl_bus[13];
    assign sram_blwl_wl[728] = wl_bus[14];
    assign sram_blwl_wl[729] = wl_bus[14];
    assign sram_blwl_wl[730] = wl_bus[14];
    assign sram_blwl_wl[731] = wl_bus[14];
    assign sram_blwl_wl[732] = wl_bus[14];
    assign sram_blwl_wl[733] = wl_bus[14];
    assign sram_blwl_wl[734] = wl_bus[14];
    assign sram_blwl_wl[735] = wl_bus[14];
    assign sram_blwl_wl[736] = wl_bus[14];
    assign sram_blwl_wl[737] = wl_bus[14];
    assign sram_blwl_wl[738] = wl_bus[14];
    assign sram_blwl_wl[739] = wl_bus[14];
    assign sram_blwl_wl[740] = wl_bus[14];
    assign sram_blwl_wl[741] = wl_bus[14];
    assign sram_blwl_wl[742] = wl_bus[14];
    assign sram_blwl_wl[743] = wl_bus[14];
    assign sram_blwl_wl[744] = wl_bus[14];
    assign sram_blwl_wl[745] = wl_bus[14];
    assign sram_blwl_wl[746] = wl_bus[14];
    assign sram_blwl_wl[747] = wl_bus[14];
    assign sram_blwl_wl[748] = wl_bus[14];
    assign sram_blwl_wl[749] = wl_bus[14];
    assign sram_blwl_wl[750] = wl_bus[14];
    assign sram_blwl_wl[751] = wl_bus[14];
    assign sram_blwl_wl[752] = wl_bus[14];
    assign sram_blwl_wl[753] = wl_bus[14];
    assign sram_blwl_wl[754] = wl_bus[14];
    assign sram_blwl_wl[755] = wl_bus[14];
    assign sram_blwl_wl[756] = wl_bus[14];
    assign sram_blwl_wl[757] = wl_bus[14];
    assign sram_blwl_wl[758] = wl_bus[14];
    assign sram_blwl_wl[759] = wl_bus[14];
    assign sram_blwl_wl[760] = wl_bus[14];
    assign sram_blwl_wl[761] = wl_bus[14];
    assign sram_blwl_wl[762] = wl_bus[14];
    assign sram_blwl_wl[763] = wl_bus[14];
    assign sram_blwl_wl[764] = wl_bus[14];
    assign sram_blwl_wl[765] = wl_bus[14];
    assign sram_blwl_wl[766] = wl_bus[14];
    assign sram_blwl_wl[767] = wl_bus[14];
    assign sram_blwl_wl[768] = wl_bus[14];
    assign sram_blwl_wl[769] = wl_bus[14];
    assign sram_blwl_wl[770] = wl_bus[14];
    assign sram_blwl_wl[771] = wl_bus[14];
    assign sram_blwl_wl[772] = wl_bus[14];
    assign sram_blwl_wl[773] = wl_bus[14];
    assign sram_blwl_wl[774] = wl_bus[14];
    assign sram_blwl_wl[775] = wl_bus[14];
    assign sram_blwl_wl[776] = wl_bus[14];
    assign sram_blwl_wl[777] = wl_bus[14];
    assign sram_blwl_wl[778] = wl_bus[14];
    assign sram_blwl_wl[779] = wl_bus[14];
    assign sram_blwl_wl[780] = wl_bus[15];
    assign sram_blwl_wl[781] = wl_bus[15];
    assign sram_blwl_wl[782] = wl_bus[15];
    assign sram_blwl_wl[783] = wl_bus[15];
    assign sram_blwl_wl[784] = wl_bus[15];
    assign sram_blwl_wl[785] = wl_bus[15];
    assign sram_blwl_wl[786] = wl_bus[15];
    assign sram_blwl_wl[787] = wl_bus[15];
    assign sram_blwl_wl[788] = wl_bus[15];
    assign sram_blwl_wl[789] = wl_bus[15];
    assign sram_blwl_wl[790] = wl_bus[15];
    assign sram_blwl_wl[791] = wl_bus[15];
    assign sram_blwl_wl[792] = wl_bus[15];
    assign sram_blwl_wl[793] = wl_bus[15];
    assign sram_blwl_wl[794] = wl_bus[15];
    assign sram_blwl_wl[795] = wl_bus[15];
    assign sram_blwl_wl[796] = wl_bus[15];
    assign sram_blwl_wl[797] = wl_bus[15];
    assign sram_blwl_wl[798] = wl_bus[15];
    assign sram_blwl_wl[799] = wl_bus[15];
    assign sram_blwl_wl[800] = wl_bus[15];
    assign sram_blwl_wl[801] = wl_bus[15];
    assign sram_blwl_wl[802] = wl_bus[15];
    assign sram_blwl_wl[803] = wl_bus[15];
    assign sram_blwl_wl[804] = wl_bus[15];
    assign sram_blwl_wl[805] = wl_bus[15];
    assign sram_blwl_wl[806] = wl_bus[15];
    assign sram_blwl_wl[807] = wl_bus[15];
    assign sram_blwl_wl[808] = wl_bus[15];
    assign sram_blwl_wl[809] = wl_bus[15];
    assign sram_blwl_wl[810] = wl_bus[15];
    assign sram_blwl_wl[811] = wl_bus[15];
    assign sram_blwl_wl[812] = wl_bus[15];
    assign sram_blwl_wl[813] = wl_bus[15];
    assign sram_blwl_wl[814] = wl_bus[15];
    assign sram_blwl_wl[815] = wl_bus[15];
    assign sram_blwl_wl[816] = wl_bus[15];
    assign sram_blwl_wl[817] = wl_bus[15];
    assign sram_blwl_wl[818] = wl_bus[15];
    assign sram_blwl_wl[819] = wl_bus[15];
    assign sram_blwl_wl[820] = wl_bus[15];
    assign sram_blwl_wl[821] = wl_bus[15];
    assign sram_blwl_wl[822] = wl_bus[15];
    assign sram_blwl_wl[823] = wl_bus[15];
    assign sram_blwl_wl[824] = wl_bus[15];
    assign sram_blwl_wl[825] = wl_bus[15];
    assign sram_blwl_wl[826] = wl_bus[15];
    assign sram_blwl_wl[827] = wl_bus[15];
    assign sram_blwl_wl[828] = wl_bus[15];
    assign sram_blwl_wl[829] = wl_bus[15];
    assign sram_blwl_wl[830] = wl_bus[15];
    assign sram_blwl_wl[831] = wl_bus[15];
    assign sram_blwl_wl[832] = wl_bus[16];
    assign sram_blwl_wl[833] = wl_bus[16];
    assign sram_blwl_wl[834] = wl_bus[16];
    assign sram_blwl_wl[835] = wl_bus[16];
    assign sram_blwl_wl[836] = wl_bus[16];
    assign sram_blwl_wl[837] = wl_bus[16];
    assign sram_blwl_wl[838] = wl_bus[16];
    assign sram_blwl_wl[839] = wl_bus[16];
    assign sram_blwl_wl[840] = wl_bus[16];
    assign sram_blwl_wl[841] = wl_bus[16];
    assign sram_blwl_wl[842] = wl_bus[16];
    assign sram_blwl_wl[843] = wl_bus[16];
    assign sram_blwl_wl[844] = wl_bus[16];
    assign sram_blwl_wl[845] = wl_bus[16];
    assign sram_blwl_wl[846] = wl_bus[16];
    assign sram_blwl_wl[847] = wl_bus[16];
    assign sram_blwl_wl[848] = wl_bus[16];
    assign sram_blwl_wl[849] = wl_bus[16];
    assign sram_blwl_wl[850] = wl_bus[16];
    assign sram_blwl_wl[851] = wl_bus[16];
    assign sram_blwl_wl[852] = wl_bus[16];
    assign sram_blwl_wl[853] = wl_bus[16];
    assign sram_blwl_wl[854] = wl_bus[16];
    assign sram_blwl_wl[855] = wl_bus[16];
    assign sram_blwl_wl[856] = wl_bus[16];
    assign sram_blwl_wl[857] = wl_bus[16];
    assign sram_blwl_wl[858] = wl_bus[16];
    assign sram_blwl_wl[859] = wl_bus[16];
    assign sram_blwl_wl[860] = wl_bus[16];
    assign sram_blwl_wl[861] = wl_bus[16];
    assign sram_blwl_wl[862] = wl_bus[16];
    assign sram_blwl_wl[863] = wl_bus[16];
    assign sram_blwl_wl[864] = wl_bus[16];
    assign sram_blwl_wl[865] = wl_bus[16];
    assign sram_blwl_wl[866] = wl_bus[16];
    assign sram_blwl_wl[867] = wl_bus[16];
    assign sram_blwl_wl[868] = wl_bus[16];
    assign sram_blwl_wl[869] = wl_bus[16];
    assign sram_blwl_wl[870] = wl_bus[16];
    assign sram_blwl_wl[871] = wl_bus[16];
    assign sram_blwl_wl[872] = wl_bus[16];
    assign sram_blwl_wl[873] = wl_bus[16];
    assign sram_blwl_wl[874] = wl_bus[16];
    assign sram_blwl_wl[875] = wl_bus[16];
    assign sram_blwl_wl[876] = wl_bus[16];
    assign sram_blwl_wl[877] = wl_bus[16];
    assign sram_blwl_wl[878] = wl_bus[16];
    assign sram_blwl_wl[879] = wl_bus[16];
    assign sram_blwl_wl[880] = wl_bus[16];
    assign sram_blwl_wl[881] = wl_bus[16];
    assign sram_blwl_wl[882] = wl_bus[16];
    assign sram_blwl_wl[883] = wl_bus[16];
    assign sram_blwl_wl[884] = wl_bus[17];
    assign sram_blwl_wl[885] = wl_bus[17];
    assign sram_blwl_wl[886] = wl_bus[17];
    assign sram_blwl_wl[887] = wl_bus[17];
    assign sram_blwl_wl[888] = wl_bus[17];
    assign sram_blwl_wl[889] = wl_bus[17];
    assign sram_blwl_wl[890] = wl_bus[17];
    assign sram_blwl_wl[891] = wl_bus[17];
    assign sram_blwl_wl[892] = wl_bus[17];
    assign sram_blwl_wl[893] = wl_bus[17];
    assign sram_blwl_wl[894] = wl_bus[17];
    assign sram_blwl_wl[895] = wl_bus[17];
    assign sram_blwl_wl[896] = wl_bus[17];
    assign sram_blwl_wl[897] = wl_bus[17];
    assign sram_blwl_wl[898] = wl_bus[17];
    assign sram_blwl_wl[899] = wl_bus[17];
    assign sram_blwl_wl[900] = wl_bus[17];
    assign sram_blwl_wl[901] = wl_bus[17];
    assign sram_blwl_wl[902] = wl_bus[17];
    assign sram_blwl_wl[903] = wl_bus[17];
    assign sram_blwl_wl[904] = wl_bus[17];
    assign sram_blwl_wl[905] = wl_bus[17];
    assign sram_blwl_wl[906] = wl_bus[17];
    assign sram_blwl_wl[907] = wl_bus[17];
    assign sram_blwl_wl[908] = wl_bus[17];
    assign sram_blwl_wl[909] = wl_bus[17];
    assign sram_blwl_wl[910] = wl_bus[17];
    assign sram_blwl_wl[911] = wl_bus[17];
    assign sram_blwl_wl[912] = wl_bus[17];
    assign sram_blwl_wl[913] = wl_bus[17];
    assign sram_blwl_wl[914] = wl_bus[17];
    assign sram_blwl_wl[915] = wl_bus[17];
    assign sram_blwl_wl[916] = wl_bus[17];
    assign sram_blwl_wl[917] = wl_bus[17];
    assign sram_blwl_wl[918] = wl_bus[17];
    assign sram_blwl_wl[919] = wl_bus[17];
    assign sram_blwl_wl[920] = wl_bus[17];
    assign sram_blwl_wl[921] = wl_bus[17];
    assign sram_blwl_wl[922] = wl_bus[17];
    assign sram_blwl_wl[923] = wl_bus[17];
    assign sram_blwl_wl[924] = wl_bus[17];
    assign sram_blwl_wl[925] = wl_bus[17];
    assign sram_blwl_wl[926] = wl_bus[17];
    assign sram_blwl_wl[927] = wl_bus[17];
    assign sram_blwl_wl[928] = wl_bus[17];
    assign sram_blwl_wl[929] = wl_bus[17];
    assign sram_blwl_wl[930] = wl_bus[17];
    assign sram_blwl_wl[931] = wl_bus[17];
    assign sram_blwl_wl[932] = wl_bus[17];
    assign sram_blwl_wl[933] = wl_bus[17];
    assign sram_blwl_wl[934] = wl_bus[17];
    assign sram_blwl_wl[935] = wl_bus[17];
    assign sram_blwl_wl[936] = wl_bus[18];
    assign sram_blwl_wl[937] = wl_bus[18];
    assign sram_blwl_wl[938] = wl_bus[18];
    assign sram_blwl_wl[939] = wl_bus[18];
    assign sram_blwl_wl[940] = wl_bus[18];
    assign sram_blwl_wl[941] = wl_bus[18];
    assign sram_blwl_wl[942] = wl_bus[18];
    assign sram_blwl_wl[943] = wl_bus[18];
    assign sram_blwl_wl[944] = wl_bus[18];
    assign sram_blwl_wl[945] = wl_bus[18];
    assign sram_blwl_wl[946] = wl_bus[18];
    assign sram_blwl_wl[947] = wl_bus[18];
    assign sram_blwl_wl[948] = wl_bus[18];
    assign sram_blwl_wl[949] = wl_bus[18];
    assign sram_blwl_wl[950] = wl_bus[18];
    assign sram_blwl_wl[951] = wl_bus[18];
    assign sram_blwl_wl[952] = wl_bus[18];
    assign sram_blwl_wl[953] = wl_bus[18];
    assign sram_blwl_wl[954] = wl_bus[18];
    assign sram_blwl_wl[955] = wl_bus[18];
    assign sram_blwl_wl[956] = wl_bus[18];
    assign sram_blwl_wl[957] = wl_bus[18];
    assign sram_blwl_wl[958] = wl_bus[18];
    assign sram_blwl_wl[959] = wl_bus[18];
    assign sram_blwl_wl[960] = wl_bus[18];
    assign sram_blwl_wl[961] = wl_bus[18];
    assign sram_blwl_wl[962] = wl_bus[18];
    assign sram_blwl_wl[963] = wl_bus[18];
    assign sram_blwl_wl[964] = wl_bus[18];
    assign sram_blwl_wl[965] = wl_bus[18];
    assign sram_blwl_wl[966] = wl_bus[18];
    assign sram_blwl_wl[967] = wl_bus[18];
    assign sram_blwl_wl[968] = wl_bus[18];
    assign sram_blwl_wl[969] = wl_bus[18];
    assign sram_blwl_wl[970] = wl_bus[18];
    assign sram_blwl_wl[971] = wl_bus[18];
    assign sram_blwl_wl[972] = wl_bus[18];
    assign sram_blwl_wl[973] = wl_bus[18];
    assign sram_blwl_wl[974] = wl_bus[18];
    assign sram_blwl_wl[975] = wl_bus[18];
    assign sram_blwl_wl[976] = wl_bus[18];
    assign sram_blwl_wl[977] = wl_bus[18];
    assign sram_blwl_wl[978] = wl_bus[18];
    assign sram_blwl_wl[979] = wl_bus[18];
    assign sram_blwl_wl[980] = wl_bus[18];
    assign sram_blwl_wl[981] = wl_bus[18];
    assign sram_blwl_wl[982] = wl_bus[18];
    assign sram_blwl_wl[983] = wl_bus[18];
    assign sram_blwl_wl[984] = wl_bus[18];
    assign sram_blwl_wl[985] = wl_bus[18];
    assign sram_blwl_wl[986] = wl_bus[18];
    assign sram_blwl_wl[987] = wl_bus[18];
    assign sram_blwl_wl[988] = wl_bus[19];
    assign sram_blwl_wl[989] = wl_bus[19];
    assign sram_blwl_wl[990] = wl_bus[19];
    assign sram_blwl_wl[991] = wl_bus[19];
    assign sram_blwl_wl[992] = wl_bus[19];
    assign sram_blwl_wl[993] = wl_bus[19];
    assign sram_blwl_wl[994] = wl_bus[19];
    assign sram_blwl_wl[995] = wl_bus[19];
    assign sram_blwl_wl[996] = wl_bus[19];
    assign sram_blwl_wl[997] = wl_bus[19];
    assign sram_blwl_wl[998] = wl_bus[19];
    assign sram_blwl_wl[999] = wl_bus[19];
    assign sram_blwl_wl[1000] = wl_bus[19];
    assign sram_blwl_wl[1001] = wl_bus[19];
    assign sram_blwl_wl[1002] = wl_bus[19];
    assign sram_blwl_wl[1003] = wl_bus[19];
    assign sram_blwl_wl[1004] = wl_bus[19];
    assign sram_blwl_wl[1005] = wl_bus[19];
    assign sram_blwl_wl[1006] = wl_bus[19];
    assign sram_blwl_wl[1007] = wl_bus[19];
    assign sram_blwl_wl[1008] = wl_bus[19];
    assign sram_blwl_wl[1009] = wl_bus[19];
    assign sram_blwl_wl[1010] = wl_bus[19];
    assign sram_blwl_wl[1011] = wl_bus[19];
    assign sram_blwl_wl[1012] = wl_bus[19];
    assign sram_blwl_wl[1013] = wl_bus[19];
    assign sram_blwl_wl[1014] = wl_bus[19];
    assign sram_blwl_wl[1015] = wl_bus[19];
    assign sram_blwl_wl[1016] = wl_bus[19];
    assign sram_blwl_wl[1017] = wl_bus[19];
    assign sram_blwl_wl[1018] = wl_bus[19];
    assign sram_blwl_wl[1019] = wl_bus[19];
    assign sram_blwl_wl[1020] = wl_bus[19];
    assign sram_blwl_wl[1021] = wl_bus[19];
    assign sram_blwl_wl[1022] = wl_bus[19];
    assign sram_blwl_wl[1023] = wl_bus[19];
    assign sram_blwl_wl[1024] = wl_bus[19];
    assign sram_blwl_wl[1025] = wl_bus[19];
    assign sram_blwl_wl[1026] = wl_bus[19];
    assign sram_blwl_wl[1027] = wl_bus[19];
    assign sram_blwl_wl[1028] = wl_bus[19];
    assign sram_blwl_wl[1029] = wl_bus[19];
    assign sram_blwl_wl[1030] = wl_bus[19];
    assign sram_blwl_wl[1031] = wl_bus[19];
    assign sram_blwl_wl[1032] = wl_bus[19];
    assign sram_blwl_wl[1033] = wl_bus[19];
    assign sram_blwl_wl[1034] = wl_bus[19];
    assign sram_blwl_wl[1035] = wl_bus[19];
    assign sram_blwl_wl[1036] = wl_bus[19];
    assign sram_blwl_wl[1037] = wl_bus[19];
    assign sram_blwl_wl[1038] = wl_bus[19];
    assign sram_blwl_wl[1039] = wl_bus[19];
    assign sram_blwl_wl[1040] = wl_bus[20];
    assign sram_blwl_wl[1041] = wl_bus[20];
    assign sram_blwl_wl[1042] = wl_bus[20];
    assign sram_blwl_wl[1043] = wl_bus[20];
    assign sram_blwl_wl[1044] = wl_bus[20];
    assign sram_blwl_wl[1045] = wl_bus[20];
    assign sram_blwl_wl[1046] = wl_bus[20];
    assign sram_blwl_wl[1047] = wl_bus[20];
    assign sram_blwl_wl[1048] = wl_bus[20];
    assign sram_blwl_wl[1049] = wl_bus[20];
    assign sram_blwl_wl[1050] = wl_bus[20];
    assign sram_blwl_wl[1051] = wl_bus[20];
    assign sram_blwl_wl[1052] = wl_bus[20];
    assign sram_blwl_wl[1053] = wl_bus[20];
    assign sram_blwl_wl[1054] = wl_bus[20];
    assign sram_blwl_wl[1055] = wl_bus[20];
    assign sram_blwl_wl[1056] = wl_bus[20];
    assign sram_blwl_wl[1057] = wl_bus[20];
    assign sram_blwl_wl[1058] = wl_bus[20];
    assign sram_blwl_wl[1059] = wl_bus[20];
    assign sram_blwl_wl[1060] = wl_bus[20];
    assign sram_blwl_wl[1061] = wl_bus[20];
    assign sram_blwl_wl[1062] = wl_bus[20];
    assign sram_blwl_wl[1063] = wl_bus[20];
    assign sram_blwl_wl[1064] = wl_bus[20];
    assign sram_blwl_wl[1065] = wl_bus[20];
    assign sram_blwl_wl[1066] = wl_bus[20];
    assign sram_blwl_wl[1067] = wl_bus[20];
    assign sram_blwl_wl[1068] = wl_bus[20];
    assign sram_blwl_wl[1069] = wl_bus[20];
    assign sram_blwl_wl[1070] = wl_bus[20];
    assign sram_blwl_wl[1071] = wl_bus[20];
    assign sram_blwl_wl[1072] = wl_bus[20];
    assign sram_blwl_wl[1073] = wl_bus[20];
    assign sram_blwl_wl[1074] = wl_bus[20];
    assign sram_blwl_wl[1075] = wl_bus[20];
    assign sram_blwl_wl[1076] = wl_bus[20];
    assign sram_blwl_wl[1077] = wl_bus[20];
    assign sram_blwl_wl[1078] = wl_bus[20];
    assign sram_blwl_wl[1079] = wl_bus[20];
    assign sram_blwl_wl[1080] = wl_bus[20];
    assign sram_blwl_wl[1081] = wl_bus[20];
    assign sram_blwl_wl[1082] = wl_bus[20];
    assign sram_blwl_wl[1083] = wl_bus[20];
    assign sram_blwl_wl[1084] = wl_bus[20];
    assign sram_blwl_wl[1085] = wl_bus[20];
    assign sram_blwl_wl[1086] = wl_bus[20];
    assign sram_blwl_wl[1087] = wl_bus[20];
    assign sram_blwl_wl[1088] = wl_bus[20];
    assign sram_blwl_wl[1089] = wl_bus[20];
    assign sram_blwl_wl[1090] = wl_bus[20];
    assign sram_blwl_wl[1091] = wl_bus[20];
    assign sram_blwl_wl[1092] = wl_bus[21];
    assign sram_blwl_wl[1093] = wl_bus[21];
    assign sram_blwl_wl[1094] = wl_bus[21];
    assign sram_blwl_wl[1095] = wl_bus[21];
    assign sram_blwl_wl[1096] = wl_bus[21];
    assign sram_blwl_wl[1097] = wl_bus[21];
    assign sram_blwl_wl[1098] = wl_bus[21];
    assign sram_blwl_wl[1099] = wl_bus[21];
    assign sram_blwl_wl[1100] = wl_bus[21];
    assign sram_blwl_wl[1101] = wl_bus[21];
    assign sram_blwl_wl[1102] = wl_bus[21];
    assign sram_blwl_wl[1103] = wl_bus[21];
    assign sram_blwl_wl[1104] = wl_bus[21];
    assign sram_blwl_wl[1105] = wl_bus[21];
    assign sram_blwl_wl[1106] = wl_bus[21];
    assign sram_blwl_wl[1107] = wl_bus[21];
    assign sram_blwl_wl[1108] = wl_bus[21];
    assign sram_blwl_wl[1109] = wl_bus[21];
    assign sram_blwl_wl[1110] = wl_bus[21];
    assign sram_blwl_wl[1111] = wl_bus[21];
    assign sram_blwl_wl[1112] = wl_bus[21];
    assign sram_blwl_wl[1113] = wl_bus[21];
    assign sram_blwl_wl[1114] = wl_bus[21];
    assign sram_blwl_wl[1115] = wl_bus[21];
    assign sram_blwl_wl[1116] = wl_bus[21];
    assign sram_blwl_wl[1117] = wl_bus[21];
    assign sram_blwl_wl[1118] = wl_bus[21];
    assign sram_blwl_wl[1119] = wl_bus[21];
    assign sram_blwl_wl[1120] = wl_bus[21];
    assign sram_blwl_wl[1121] = wl_bus[21];
    assign sram_blwl_wl[1122] = wl_bus[21];
    assign sram_blwl_wl[1123] = wl_bus[21];
    assign sram_blwl_wl[1124] = wl_bus[21];
    assign sram_blwl_wl[1125] = wl_bus[21];
    assign sram_blwl_wl[1126] = wl_bus[21];
    assign sram_blwl_wl[1127] = wl_bus[21];
    assign sram_blwl_wl[1128] = wl_bus[21];
    assign sram_blwl_wl[1129] = wl_bus[21];
    assign sram_blwl_wl[1130] = wl_bus[21];
    assign sram_blwl_wl[1131] = wl_bus[21];
    assign sram_blwl_wl[1132] = wl_bus[21];
    assign sram_blwl_wl[1133] = wl_bus[21];
    assign sram_blwl_wl[1134] = wl_bus[21];
    assign sram_blwl_wl[1135] = wl_bus[21];
    assign sram_blwl_wl[1136] = wl_bus[21];
    assign sram_blwl_wl[1137] = wl_bus[21];
    assign sram_blwl_wl[1138] = wl_bus[21];
    assign sram_blwl_wl[1139] = wl_bus[21];
    assign sram_blwl_wl[1140] = wl_bus[21];
    assign sram_blwl_wl[1141] = wl_bus[21];
    assign sram_blwl_wl[1142] = wl_bus[21];
    assign sram_blwl_wl[1143] = wl_bus[21];
    assign sram_blwl_wl[1144] = wl_bus[22];
    assign sram_blwl_wl[1145] = wl_bus[22];
    assign sram_blwl_wl[1146] = wl_bus[22];
    assign sram_blwl_wl[1147] = wl_bus[22];
    assign sram_blwl_wl[1148] = wl_bus[22];
    assign sram_blwl_wl[1149] = wl_bus[22];
    assign sram_blwl_wl[1150] = wl_bus[22];
    assign sram_blwl_wl[1151] = wl_bus[22];
    assign sram_blwl_wl[1152] = wl_bus[22];
    assign sram_blwl_wl[1153] = wl_bus[22];
    assign sram_blwl_wl[1154] = wl_bus[22];
    assign sram_blwl_wl[1155] = wl_bus[22];
    assign sram_blwl_wl[1156] = wl_bus[22];
    assign sram_blwl_wl[1157] = wl_bus[22];
    assign sram_blwl_wl[1158] = wl_bus[22];
    assign sram_blwl_wl[1159] = wl_bus[22];
    assign sram_blwl_wl[1160] = wl_bus[22];
    assign sram_blwl_wl[1161] = wl_bus[22];
    assign sram_blwl_wl[1162] = wl_bus[22];
    assign sram_blwl_wl[1163] = wl_bus[22];
    assign sram_blwl_wl[1164] = wl_bus[22];
    assign sram_blwl_wl[1165] = wl_bus[22];
    assign sram_blwl_wl[1166] = wl_bus[22];
    assign sram_blwl_wl[1167] = wl_bus[22];
    assign sram_blwl_wl[1168] = wl_bus[22];
    assign sram_blwl_wl[1169] = wl_bus[22];
    assign sram_blwl_wl[1170] = wl_bus[22];
    assign sram_blwl_wl[1171] = wl_bus[22];
    assign sram_blwl_wl[1172] = wl_bus[22];
    assign sram_blwl_wl[1173] = wl_bus[22];
    assign sram_blwl_wl[1174] = wl_bus[22];
    assign sram_blwl_wl[1175] = wl_bus[22];
    assign sram_blwl_wl[1176] = wl_bus[22];
    assign sram_blwl_wl[1177] = wl_bus[22];
    assign sram_blwl_wl[1178] = wl_bus[22];
    assign sram_blwl_wl[1179] = wl_bus[22];
    assign sram_blwl_wl[1180] = wl_bus[22];
    assign sram_blwl_wl[1181] = wl_bus[22];
    assign sram_blwl_wl[1182] = wl_bus[22];
    assign sram_blwl_wl[1183] = wl_bus[22];
    assign sram_blwl_wl[1184] = wl_bus[22];
    assign sram_blwl_wl[1185] = wl_bus[22];
    assign sram_blwl_wl[1186] = wl_bus[22];
    assign sram_blwl_wl[1187] = wl_bus[22];
    assign sram_blwl_wl[1188] = wl_bus[22];
    assign sram_blwl_wl[1189] = wl_bus[22];
    assign sram_blwl_wl[1190] = wl_bus[22];
    assign sram_blwl_wl[1191] = wl_bus[22];
    assign sram_blwl_wl[1192] = wl_bus[22];
    assign sram_blwl_wl[1193] = wl_bus[22];
    assign sram_blwl_wl[1194] = wl_bus[22];
    assign sram_blwl_wl[1195] = wl_bus[22];
    assign sram_blwl_wl[1196] = wl_bus[23];
    assign sram_blwl_wl[1197] = wl_bus[23];
    assign sram_blwl_wl[1198] = wl_bus[23];
    assign sram_blwl_wl[1199] = wl_bus[23];
    assign sram_blwl_wl[1200] = wl_bus[23];
    assign sram_blwl_wl[1201] = wl_bus[23];
    assign sram_blwl_wl[1202] = wl_bus[23];
    assign sram_blwl_wl[1203] = wl_bus[23];
    assign sram_blwl_wl[1204] = wl_bus[23];
    assign sram_blwl_wl[1205] = wl_bus[23];
    assign sram_blwl_wl[1206] = wl_bus[23];
    assign sram_blwl_wl[1207] = wl_bus[23];
    assign sram_blwl_wl[1208] = wl_bus[23];
    assign sram_blwl_wl[1209] = wl_bus[23];
    assign sram_blwl_wl[1210] = wl_bus[23];
    assign sram_blwl_wl[1211] = wl_bus[23];
    assign sram_blwl_wl[1212] = wl_bus[23];
    assign sram_blwl_wl[1213] = wl_bus[23];
    assign sram_blwl_wl[1214] = wl_bus[23];
    assign sram_blwl_wl[1215] = wl_bus[23];
    assign sram_blwl_wl[1216] = wl_bus[23];
    assign sram_blwl_wl[1217] = wl_bus[23];
    assign sram_blwl_wl[1218] = wl_bus[23];
    assign sram_blwl_wl[1219] = wl_bus[23];
    assign sram_blwl_wl[1220] = wl_bus[23];
    assign sram_blwl_wl[1221] = wl_bus[23];
    assign sram_blwl_wl[1222] = wl_bus[23];
    assign sram_blwl_wl[1223] = wl_bus[23];
    assign sram_blwl_wl[1224] = wl_bus[23];
    assign sram_blwl_wl[1225] = wl_bus[23];
    assign sram_blwl_wl[1226] = wl_bus[23];
    assign sram_blwl_wl[1227] = wl_bus[23];
    assign sram_blwl_wl[1228] = wl_bus[23];
    assign sram_blwl_wl[1229] = wl_bus[23];
    assign sram_blwl_wl[1230] = wl_bus[23];
    assign sram_blwl_wl[1231] = wl_bus[23];
    assign sram_blwl_wl[1232] = wl_bus[23];
    assign sram_blwl_wl[1233] = wl_bus[23];
    assign sram_blwl_wl[1234] = wl_bus[23];
    assign sram_blwl_wl[1235] = wl_bus[23];
    assign sram_blwl_wl[1236] = wl_bus[23];
    assign sram_blwl_wl[1237] = wl_bus[23];
    assign sram_blwl_wl[1238] = wl_bus[23];
    assign sram_blwl_wl[1239] = wl_bus[23];
    assign sram_blwl_wl[1240] = wl_bus[23];
    assign sram_blwl_wl[1241] = wl_bus[23];
    assign sram_blwl_wl[1242] = wl_bus[23];
    assign sram_blwl_wl[1243] = wl_bus[23];
    assign sram_blwl_wl[1244] = wl_bus[23];
    assign sram_blwl_wl[1245] = wl_bus[23];
    assign sram_blwl_wl[1246] = wl_bus[23];
    assign sram_blwl_wl[1247] = wl_bus[23];
    assign sram_blwl_wl[1248] = wl_bus[24];
    assign sram_blwl_wl[1249] = wl_bus[24];
    assign sram_blwl_wl[1250] = wl_bus[24];
    assign sram_blwl_wl[1251] = wl_bus[24];
    assign sram_blwl_wl[1252] = wl_bus[24];
    assign sram_blwl_wl[1253] = wl_bus[24];
    assign sram_blwl_wl[1254] = wl_bus[24];
    assign sram_blwl_wl[1255] = wl_bus[24];
    assign sram_blwl_wl[1256] = wl_bus[24];
    assign sram_blwl_wl[1257] = wl_bus[24];
    assign sram_blwl_wl[1258] = wl_bus[24];
    assign sram_blwl_wl[1259] = wl_bus[24];
    assign sram_blwl_wl[1260] = wl_bus[24];
    assign sram_blwl_wl[1261] = wl_bus[24];
    assign sram_blwl_wl[1262] = wl_bus[24];
    assign sram_blwl_wl[1263] = wl_bus[24];
    assign sram_blwl_wl[1264] = wl_bus[24];
    assign sram_blwl_wl[1265] = wl_bus[24];
    assign sram_blwl_wl[1266] = wl_bus[24];
    assign sram_blwl_wl[1267] = wl_bus[24];
    assign sram_blwl_wl[1268] = wl_bus[24];
    assign sram_blwl_wl[1269] = wl_bus[24];
    assign sram_blwl_wl[1270] = wl_bus[24];
    assign sram_blwl_wl[1271] = wl_bus[24];
    assign sram_blwl_wl[1272] = wl_bus[24];
    assign sram_blwl_wl[1273] = wl_bus[24];
    assign sram_blwl_wl[1274] = wl_bus[24];
    assign sram_blwl_wl[1275] = wl_bus[24];
    assign sram_blwl_wl[1276] = wl_bus[24];
    assign sram_blwl_wl[1277] = wl_bus[24];
    assign sram_blwl_wl[1278] = wl_bus[24];
    assign sram_blwl_wl[1279] = wl_bus[24];
    assign sram_blwl_wl[1280] = wl_bus[24];
    assign sram_blwl_wl[1281] = wl_bus[24];
    assign sram_blwl_wl[1282] = wl_bus[24];
    assign sram_blwl_wl[1283] = wl_bus[24];
    assign sram_blwl_wl[1284] = wl_bus[24];
    assign sram_blwl_wl[1285] = wl_bus[24];
    assign sram_blwl_wl[1286] = wl_bus[24];
    assign sram_blwl_wl[1287] = wl_bus[24];
    assign sram_blwl_wl[1288] = wl_bus[24];
    assign sram_blwl_wl[1289] = wl_bus[24];
    assign sram_blwl_wl[1290] = wl_bus[24];
    assign sram_blwl_wl[1291] = wl_bus[24];
    assign sram_blwl_wl[1292] = wl_bus[24];
    assign sram_blwl_wl[1293] = wl_bus[24];
    assign sram_blwl_wl[1294] = wl_bus[24];
    assign sram_blwl_wl[1295] = wl_bus[24];
    assign sram_blwl_wl[1296] = wl_bus[24];
    assign sram_blwl_wl[1297] = wl_bus[24];
    assign sram_blwl_wl[1298] = wl_bus[24];
    assign sram_blwl_wl[1299] = wl_bus[24];
    assign sram_blwl_wl[1300] = wl_bus[25];
    assign sram_blwl_wl[1301] = wl_bus[25];
    assign sram_blwl_wl[1302] = wl_bus[25];
    assign sram_blwl_wl[1303] = wl_bus[25];
    assign sram_blwl_wl[1304] = wl_bus[25];
    assign sram_blwl_wl[1305] = wl_bus[25];
    assign sram_blwl_wl[1306] = wl_bus[25];
    assign sram_blwl_wl[1307] = wl_bus[25];
    assign sram_blwl_wl[1308] = wl_bus[25];
    assign sram_blwl_wl[1309] = wl_bus[25];
    assign sram_blwl_wl[1310] = wl_bus[25];
    assign sram_blwl_wl[1311] = wl_bus[25];
    assign sram_blwl_wl[1312] = wl_bus[25];
    assign sram_blwl_wl[1313] = wl_bus[25];
    assign sram_blwl_wl[1314] = wl_bus[25];
    assign sram_blwl_wl[1315] = wl_bus[25];
    assign sram_blwl_wl[1316] = wl_bus[25];
    assign sram_blwl_wl[1317] = wl_bus[25];
    assign sram_blwl_wl[1318] = wl_bus[25];
    assign sram_blwl_wl[1319] = wl_bus[25];
    assign sram_blwl_wl[1320] = wl_bus[25];
    assign sram_blwl_wl[1321] = wl_bus[25];
    assign sram_blwl_wl[1322] = wl_bus[25];
    assign sram_blwl_wl[1323] = wl_bus[25];
    assign sram_blwl_wl[1324] = wl_bus[25];
    assign sram_blwl_wl[1325] = wl_bus[25];
    assign sram_blwl_wl[1326] = wl_bus[25];
    assign sram_blwl_wl[1327] = wl_bus[25];
    assign sram_blwl_wl[1328] = wl_bus[25];
    assign sram_blwl_wl[1329] = wl_bus[25];
    assign sram_blwl_wl[1330] = wl_bus[25];
    assign sram_blwl_wl[1331] = wl_bus[25];
    assign sram_blwl_wl[1332] = wl_bus[25];
    assign sram_blwl_wl[1333] = wl_bus[25];
    assign sram_blwl_wl[1334] = wl_bus[25];
    assign sram_blwl_wl[1335] = wl_bus[25];
    assign sram_blwl_wl[1336] = wl_bus[25];
    assign sram_blwl_wl[1337] = wl_bus[25];
    assign sram_blwl_wl[1338] = wl_bus[25];
    assign sram_blwl_wl[1339] = wl_bus[25];
    assign sram_blwl_wl[1340] = wl_bus[25];
    assign sram_blwl_wl[1341] = wl_bus[25];
    assign sram_blwl_wl[1342] = wl_bus[25];
    assign sram_blwl_wl[1343] = wl_bus[25];
    assign sram_blwl_wl[1344] = wl_bus[25];
    assign sram_blwl_wl[1345] = wl_bus[25];
    assign sram_blwl_wl[1346] = wl_bus[25];
    assign sram_blwl_wl[1347] = wl_bus[25];
    assign sram_blwl_wl[1348] = wl_bus[25];
    assign sram_blwl_wl[1349] = wl_bus[25];
    assign sram_blwl_wl[1350] = wl_bus[25];
    assign sram_blwl_wl[1351] = wl_bus[25];
    assign sram_blwl_wl[1352] = wl_bus[26];
    assign sram_blwl_wl[1353] = wl_bus[26];
    assign sram_blwl_wl[1354] = wl_bus[26];
    assign sram_blwl_wl[1355] = wl_bus[26];
    assign sram_blwl_wl[1356] = wl_bus[26];
    assign sram_blwl_wl[1357] = wl_bus[26];
    assign sram_blwl_wl[1358] = wl_bus[26];
    assign sram_blwl_wl[1359] = wl_bus[26];
    assign sram_blwl_wl[1360] = wl_bus[26];
    assign sram_blwl_wl[1361] = wl_bus[26];
    assign sram_blwl_wl[1362] = wl_bus[26];
    assign sram_blwl_wl[1363] = wl_bus[26];
    assign sram_blwl_wl[1364] = wl_bus[26];
    assign sram_blwl_wl[1365] = wl_bus[26];
    assign sram_blwl_wl[1366] = wl_bus[26];
    assign sram_blwl_wl[1367] = wl_bus[26];
    assign sram_blwl_wl[1368] = wl_bus[26];
    assign sram_blwl_wl[1369] = wl_bus[26];
    assign sram_blwl_wl[1370] = wl_bus[26];
    assign sram_blwl_wl[1371] = wl_bus[26];
    assign sram_blwl_wl[1372] = wl_bus[26];
    assign sram_blwl_wl[1373] = wl_bus[26];
    assign sram_blwl_wl[1374] = wl_bus[26];
    assign sram_blwl_wl[1375] = wl_bus[26];
    assign sram_blwl_wl[1376] = wl_bus[26];
    assign sram_blwl_wl[1377] = wl_bus[26];
    assign sram_blwl_wl[1378] = wl_bus[26];
    assign sram_blwl_wl[1379] = wl_bus[26];
    assign sram_blwl_wl[1380] = wl_bus[26];
    assign sram_blwl_wl[1381] = wl_bus[26];
    assign sram_blwl_wl[1382] = wl_bus[26];
    assign sram_blwl_wl[1383] = wl_bus[26];
    assign sram_blwl_wl[1384] = wl_bus[26];
    assign sram_blwl_wl[1385] = wl_bus[26];
    assign sram_blwl_wl[1386] = wl_bus[26];
    assign sram_blwl_wl[1387] = wl_bus[26];
    assign sram_blwl_wl[1388] = wl_bus[26];
    assign sram_blwl_wl[1389] = wl_bus[26];
    assign sram_blwl_wl[1390] = wl_bus[26];
    assign sram_blwl_wl[1391] = wl_bus[26];
    assign sram_blwl_wl[1392] = wl_bus[26];
    assign sram_blwl_wl[1393] = wl_bus[26];
    assign sram_blwl_wl[1394] = wl_bus[26];
    assign sram_blwl_wl[1395] = wl_bus[26];
    assign sram_blwl_wl[1396] = wl_bus[26];
    assign sram_blwl_wl[1397] = wl_bus[26];
    assign sram_blwl_wl[1398] = wl_bus[26];
    assign sram_blwl_wl[1399] = wl_bus[26];
    assign sram_blwl_wl[1400] = wl_bus[26];
    assign sram_blwl_wl[1401] = wl_bus[26];
    assign sram_blwl_wl[1402] = wl_bus[26];
    assign sram_blwl_wl[1403] = wl_bus[26];
    assign sram_blwl_wl[1404] = wl_bus[27];
    assign sram_blwl_wl[1405] = wl_bus[27];
    assign sram_blwl_wl[1406] = wl_bus[27];
    assign sram_blwl_wl[1407] = wl_bus[27];
    assign sram_blwl_wl[1408] = wl_bus[27];
    assign sram_blwl_wl[1409] = wl_bus[27];
    assign sram_blwl_wl[1410] = wl_bus[27];
    assign sram_blwl_wl[1411] = wl_bus[27];
    assign sram_blwl_wl[1412] = wl_bus[27];
    assign sram_blwl_wl[1413] = wl_bus[27];
    assign sram_blwl_wl[1414] = wl_bus[27];
    assign sram_blwl_wl[1415] = wl_bus[27];
    assign sram_blwl_wl[1416] = wl_bus[27];
    assign sram_blwl_wl[1417] = wl_bus[27];
    assign sram_blwl_wl[1418] = wl_bus[27];
    assign sram_blwl_wl[1419] = wl_bus[27];
    assign sram_blwl_wl[1420] = wl_bus[27];
    assign sram_blwl_wl[1421] = wl_bus[27];
    assign sram_blwl_wl[1422] = wl_bus[27];
    assign sram_blwl_wl[1423] = wl_bus[27];
    assign sram_blwl_wl[1424] = wl_bus[27];
    assign sram_blwl_wl[1425] = wl_bus[27];
    assign sram_blwl_wl[1426] = wl_bus[27];
    assign sram_blwl_wl[1427] = wl_bus[27];
    assign sram_blwl_wl[1428] = wl_bus[27];
    assign sram_blwl_wl[1429] = wl_bus[27];
    assign sram_blwl_wl[1430] = wl_bus[27];
    assign sram_blwl_wl[1431] = wl_bus[27];
    assign sram_blwl_wl[1432] = wl_bus[27];
    assign sram_blwl_wl[1433] = wl_bus[27];
    assign sram_blwl_wl[1434] = wl_bus[27];
    assign sram_blwl_wl[1435] = wl_bus[27];
    assign sram_blwl_wl[1436] = wl_bus[27];
    assign sram_blwl_wl[1437] = wl_bus[27];
    assign sram_blwl_wl[1438] = wl_bus[27];
    assign sram_blwl_wl[1439] = wl_bus[27];
    assign sram_blwl_wl[1440] = wl_bus[27];
    assign sram_blwl_wl[1441] = wl_bus[27];
    assign sram_blwl_wl[1442] = wl_bus[27];
    assign sram_blwl_wl[1443] = wl_bus[27];
    assign sram_blwl_wl[1444] = wl_bus[27];
    assign sram_blwl_wl[1445] = wl_bus[27];
    assign sram_blwl_wl[1446] = wl_bus[27];
    assign sram_blwl_wl[1447] = wl_bus[27];
    assign sram_blwl_wl[1448] = wl_bus[27];
    assign sram_blwl_wl[1449] = wl_bus[27];
    assign sram_blwl_wl[1450] = wl_bus[27];
    assign sram_blwl_wl[1451] = wl_bus[27];
    assign sram_blwl_wl[1452] = wl_bus[27];
    assign sram_blwl_wl[1453] = wl_bus[27];
    assign sram_blwl_wl[1454] = wl_bus[27];
    assign sram_blwl_wl[1455] = wl_bus[27];
    assign sram_blwl_wl[1456] = wl_bus[28];
    assign sram_blwl_wl[1457] = wl_bus[28];
    assign sram_blwl_wl[1458] = wl_bus[28];
    assign sram_blwl_wl[1459] = wl_bus[28];
    assign sram_blwl_wl[1460] = wl_bus[28];
    assign sram_blwl_wl[1461] = wl_bus[28];
    assign sram_blwl_wl[1462] = wl_bus[28];
    assign sram_blwl_wl[1463] = wl_bus[28];
    assign sram_blwl_wl[1464] = wl_bus[28];
    assign sram_blwl_wl[1465] = wl_bus[28];
    assign sram_blwl_wl[1466] = wl_bus[28];
    assign sram_blwl_wl[1467] = wl_bus[28];
    assign sram_blwl_wl[1468] = wl_bus[28];
    assign sram_blwl_wl[1469] = wl_bus[28];
    assign sram_blwl_wl[1470] = wl_bus[28];
    assign sram_blwl_wl[1471] = wl_bus[28];
    assign sram_blwl_wl[1472] = wl_bus[28];
    assign sram_blwl_wl[1473] = wl_bus[28];
    assign sram_blwl_wl[1474] = wl_bus[28];
    assign sram_blwl_wl[1475] = wl_bus[28];
    assign sram_blwl_wl[1476] = wl_bus[28];
    assign sram_blwl_wl[1477] = wl_bus[28];
    assign sram_blwl_wl[1478] = wl_bus[28];
    assign sram_blwl_wl[1479] = wl_bus[28];
    assign sram_blwl_wl[1480] = wl_bus[28];
    assign sram_blwl_wl[1481] = wl_bus[28];
    assign sram_blwl_wl[1482] = wl_bus[28];
    assign sram_blwl_wl[1483] = wl_bus[28];
    assign sram_blwl_wl[1484] = wl_bus[28];
    assign sram_blwl_wl[1485] = wl_bus[28];
    assign sram_blwl_wl[1486] = wl_bus[28];
    assign sram_blwl_wl[1487] = wl_bus[28];
    assign sram_blwl_wl[1488] = wl_bus[28];
    assign sram_blwl_wl[1489] = wl_bus[28];
    assign sram_blwl_wl[1490] = wl_bus[28];
    assign sram_blwl_wl[1491] = wl_bus[28];
    assign sram_blwl_wl[1492] = wl_bus[28];
    assign sram_blwl_wl[1493] = wl_bus[28];
    assign sram_blwl_wl[1494] = wl_bus[28];
    assign sram_blwl_wl[1495] = wl_bus[28];
    assign sram_blwl_wl[1496] = wl_bus[28];
    assign sram_blwl_wl[1497] = wl_bus[28];
    assign sram_blwl_wl[1498] = wl_bus[28];
    assign sram_blwl_wl[1499] = wl_bus[28];
    assign sram_blwl_wl[1500] = wl_bus[28];
    assign sram_blwl_wl[1501] = wl_bus[28];
    assign sram_blwl_wl[1502] = wl_bus[28];
    assign sram_blwl_wl[1503] = wl_bus[28];
    assign sram_blwl_wl[1504] = wl_bus[28];
    assign sram_blwl_wl[1505] = wl_bus[28];
    assign sram_blwl_wl[1506] = wl_bus[28];
    assign sram_blwl_wl[1507] = wl_bus[28];
    assign sram_blwl_wl[1508] = wl_bus[29];
    assign sram_blwl_wl[1509] = wl_bus[29];
    assign sram_blwl_wl[1510] = wl_bus[29];
    assign sram_blwl_wl[1511] = wl_bus[29];
    assign sram_blwl_wl[1512] = wl_bus[29];
    assign sram_blwl_wl[1513] = wl_bus[29];
    assign sram_blwl_wl[1514] = wl_bus[29];
    assign sram_blwl_wl[1515] = wl_bus[29];
    assign sram_blwl_wl[1516] = wl_bus[29];
    assign sram_blwl_wl[1517] = wl_bus[29];
    assign sram_blwl_wl[1518] = wl_bus[29];
    assign sram_blwl_wl[1519] = wl_bus[29];
    assign sram_blwl_wl[1520] = wl_bus[29];
    assign sram_blwl_wl[1521] = wl_bus[29];
    assign sram_blwl_wl[1522] = wl_bus[29];
    assign sram_blwl_wl[1523] = wl_bus[29];
    assign sram_blwl_wl[1524] = wl_bus[29];
    assign sram_blwl_wl[1525] = wl_bus[29];
    assign sram_blwl_wl[1526] = wl_bus[29];
    assign sram_blwl_wl[1527] = wl_bus[29];
    assign sram_blwl_wl[1528] = wl_bus[29];
    assign sram_blwl_wl[1529] = wl_bus[29];
    assign sram_blwl_wl[1530] = wl_bus[29];
    assign sram_blwl_wl[1531] = wl_bus[29];
    assign sram_blwl_wl[1532] = wl_bus[29];
    assign sram_blwl_wl[1533] = wl_bus[29];
    assign sram_blwl_wl[1534] = wl_bus[29];
    assign sram_blwl_wl[1535] = wl_bus[29];
    assign sram_blwl_wl[1536] = wl_bus[29];
    assign sram_blwl_wl[1537] = wl_bus[29];
    assign sram_blwl_wl[1538] = wl_bus[29];
    assign sram_blwl_wl[1539] = wl_bus[29];
    assign sram_blwl_wl[1540] = wl_bus[29];
    assign sram_blwl_wl[1541] = wl_bus[29];
    assign sram_blwl_wl[1542] = wl_bus[29];
    assign sram_blwl_wl[1543] = wl_bus[29];
    assign sram_blwl_wl[1544] = wl_bus[29];
    assign sram_blwl_wl[1545] = wl_bus[29];
    assign sram_blwl_wl[1546] = wl_bus[29];
    assign sram_blwl_wl[1547] = wl_bus[29];
    assign sram_blwl_wl[1548] = wl_bus[29];
    assign sram_blwl_wl[1549] = wl_bus[29];
    assign sram_blwl_wl[1550] = wl_bus[29];
    assign sram_blwl_wl[1551] = wl_bus[29];
    assign sram_blwl_wl[1552] = wl_bus[29];
    assign sram_blwl_wl[1553] = wl_bus[29];
    assign sram_blwl_wl[1554] = wl_bus[29];
    assign sram_blwl_wl[1555] = wl_bus[29];
    assign sram_blwl_wl[1556] = wl_bus[29];
    assign sram_blwl_wl[1557] = wl_bus[29];
    assign sram_blwl_wl[1558] = wl_bus[29];
    assign sram_blwl_wl[1559] = wl_bus[29];
    assign sram_blwl_wl[1560] = wl_bus[30];
    assign sram_blwl_wl[1561] = wl_bus[30];
    assign sram_blwl_wl[1562] = wl_bus[30];
    assign sram_blwl_wl[1563] = wl_bus[30];
    assign sram_blwl_wl[1564] = wl_bus[30];
    assign sram_blwl_wl[1565] = wl_bus[30];
    assign sram_blwl_wl[1566] = wl_bus[30];
    assign sram_blwl_wl[1567] = wl_bus[30];
    assign sram_blwl_wl[1568] = wl_bus[30];
    assign sram_blwl_wl[1569] = wl_bus[30];
    assign sram_blwl_wl[1570] = wl_bus[30];
    assign sram_blwl_wl[1571] = wl_bus[30];
    assign sram_blwl_wl[1572] = wl_bus[30];
    assign sram_blwl_wl[1573] = wl_bus[30];
    assign sram_blwl_wl[1574] = wl_bus[30];
    assign sram_blwl_wl[1575] = wl_bus[30];
    assign sram_blwl_wl[1576] = wl_bus[30];
    assign sram_blwl_wl[1577] = wl_bus[30];
    assign sram_blwl_wl[1578] = wl_bus[30];
    assign sram_blwl_wl[1579] = wl_bus[30];
    assign sram_blwl_wl[1580] = wl_bus[30];
    assign sram_blwl_wl[1581] = wl_bus[30];
    assign sram_blwl_wl[1582] = wl_bus[30];
    assign sram_blwl_wl[1583] = wl_bus[30];
    assign sram_blwl_wl[1584] = wl_bus[30];
    assign sram_blwl_wl[1585] = wl_bus[30];
    assign sram_blwl_wl[1586] = wl_bus[30];
    assign sram_blwl_wl[1587] = wl_bus[30];
    assign sram_blwl_wl[1588] = wl_bus[30];
    assign sram_blwl_wl[1589] = wl_bus[30];
    assign sram_blwl_wl[1590] = wl_bus[30];
    assign sram_blwl_wl[1591] = wl_bus[30];
    assign sram_blwl_wl[1592] = wl_bus[30];
    assign sram_blwl_wl[1593] = wl_bus[30];
    assign sram_blwl_wl[1594] = wl_bus[30];
    assign sram_blwl_wl[1595] = wl_bus[30];
    assign sram_blwl_wl[1596] = wl_bus[30];
    assign sram_blwl_wl[1597] = wl_bus[30];
    assign sram_blwl_wl[1598] = wl_bus[30];
    assign sram_blwl_wl[1599] = wl_bus[30];
    assign sram_blwl_wl[1600] = wl_bus[30];
    assign sram_blwl_wl[1601] = wl_bus[30];
    assign sram_blwl_wl[1602] = wl_bus[30];
    assign sram_blwl_wl[1603] = wl_bus[30];
    assign sram_blwl_wl[1604] = wl_bus[30];
    assign sram_blwl_wl[1605] = wl_bus[30];
    assign sram_blwl_wl[1606] = wl_bus[30];
    assign sram_blwl_wl[1607] = wl_bus[30];
    assign sram_blwl_wl[1608] = wl_bus[30];
    assign sram_blwl_wl[1609] = wl_bus[30];
    assign sram_blwl_wl[1610] = wl_bus[30];
    assign sram_blwl_wl[1611] = wl_bus[30];
    assign sram_blwl_wl[1612] = wl_bus[31];
    assign sram_blwl_wl[1613] = wl_bus[31];
    assign sram_blwl_wl[1614] = wl_bus[31];
    assign sram_blwl_wl[1615] = wl_bus[31];
    assign sram_blwl_wl[1616] = wl_bus[31];
    assign sram_blwl_wl[1617] = wl_bus[31];
    assign sram_blwl_wl[1618] = wl_bus[31];
    assign sram_blwl_wl[1619] = wl_bus[31];
    assign sram_blwl_wl[1620] = wl_bus[31];
    assign sram_blwl_wl[1621] = wl_bus[31];
    assign sram_blwl_wl[1622] = wl_bus[31];
    assign sram_blwl_wl[1623] = wl_bus[31];
    assign sram_blwl_wl[1624] = wl_bus[31];
    assign sram_blwl_wl[1625] = wl_bus[31];
    assign sram_blwl_wl[1626] = wl_bus[31];
    assign sram_blwl_wl[1627] = wl_bus[31];
    assign sram_blwl_wl[1628] = wl_bus[31];
    assign sram_blwl_wl[1629] = wl_bus[31];
    assign sram_blwl_wl[1630] = wl_bus[31];
    assign sram_blwl_wl[1631] = wl_bus[31];
    assign sram_blwl_wl[1632] = wl_bus[31];
    assign sram_blwl_wl[1633] = wl_bus[31];
    assign sram_blwl_wl[1634] = wl_bus[31];
    assign sram_blwl_wl[1635] = wl_bus[31];
    assign sram_blwl_wl[1636] = wl_bus[31];
    assign sram_blwl_wl[1637] = wl_bus[31];
    assign sram_blwl_wl[1638] = wl_bus[31];
    assign sram_blwl_wl[1639] = wl_bus[31];
    assign sram_blwl_wl[1640] = wl_bus[31];
    assign sram_blwl_wl[1641] = wl_bus[31];
    assign sram_blwl_wl[1642] = wl_bus[31];
    assign sram_blwl_wl[1643] = wl_bus[31];
    assign sram_blwl_wl[1644] = wl_bus[31];
    assign sram_blwl_wl[1645] = wl_bus[31];
    assign sram_blwl_wl[1646] = wl_bus[31];
    assign sram_blwl_wl[1647] = wl_bus[31];
    assign sram_blwl_wl[1648] = wl_bus[31];
    assign sram_blwl_wl[1649] = wl_bus[31];
    assign sram_blwl_wl[1650] = wl_bus[31];
    assign sram_blwl_wl[1651] = wl_bus[31];
    assign sram_blwl_wl[1652] = wl_bus[31];
    assign sram_blwl_wl[1653] = wl_bus[31];
    assign sram_blwl_wl[1654] = wl_bus[31];
    assign sram_blwl_wl[1655] = wl_bus[31];
    assign sram_blwl_wl[1656] = wl_bus[31];
    assign sram_blwl_wl[1657] = wl_bus[31];
    assign sram_blwl_wl[1658] = wl_bus[31];
    assign sram_blwl_wl[1659] = wl_bus[31];
    assign sram_blwl_wl[1660] = wl_bus[31];
    assign sram_blwl_wl[1661] = wl_bus[31];
    assign sram_blwl_wl[1662] = wl_bus[31];
    assign sram_blwl_wl[1663] = wl_bus[31];
    assign sram_blwl_wl[1664] = wl_bus[32];
    assign sram_blwl_wl[1665] = wl_bus[32];
    assign sram_blwl_wl[1666] = wl_bus[32];
    assign sram_blwl_wl[1667] = wl_bus[32];
    assign sram_blwl_wl[1668] = wl_bus[32];
    assign sram_blwl_wl[1669] = wl_bus[32];
    assign sram_blwl_wl[1670] = wl_bus[32];
    assign sram_blwl_wl[1671] = wl_bus[32];
    assign sram_blwl_wl[1672] = wl_bus[32];
    assign sram_blwl_wl[1673] = wl_bus[32];
    assign sram_blwl_wl[1674] = wl_bus[32];
    assign sram_blwl_wl[1675] = wl_bus[32];
    assign sram_blwl_wl[1676] = wl_bus[32];
    assign sram_blwl_wl[1677] = wl_bus[32];
    assign sram_blwl_wl[1678] = wl_bus[32];
    assign sram_blwl_wl[1679] = wl_bus[32];
    assign sram_blwl_wl[1680] = wl_bus[32];
    assign sram_blwl_wl[1681] = wl_bus[32];
    assign sram_blwl_wl[1682] = wl_bus[32];
    assign sram_blwl_wl[1683] = wl_bus[32];
    assign sram_blwl_wl[1684] = wl_bus[32];
    assign sram_blwl_wl[1685] = wl_bus[32];
    assign sram_blwl_wl[1686] = wl_bus[32];
    assign sram_blwl_wl[1687] = wl_bus[32];
    assign sram_blwl_wl[1688] = wl_bus[32];
    assign sram_blwl_wl[1689] = wl_bus[32];
    assign sram_blwl_wl[1690] = wl_bus[32];
    assign sram_blwl_wl[1691] = wl_bus[32];
    assign sram_blwl_wl[1692] = wl_bus[32];
    assign sram_blwl_wl[1693] = wl_bus[32];
    assign sram_blwl_wl[1694] = wl_bus[32];
    assign sram_blwl_wl[1695] = wl_bus[32];
    assign sram_blwl_wl[1696] = wl_bus[32];
    assign sram_blwl_wl[1697] = wl_bus[32];
    assign sram_blwl_wl[1698] = wl_bus[32];
    assign sram_blwl_wl[1699] = wl_bus[32];
    assign sram_blwl_wl[1700] = wl_bus[32];
    assign sram_blwl_wl[1701] = wl_bus[32];
    assign sram_blwl_wl[1702] = wl_bus[32];
    assign sram_blwl_wl[1703] = wl_bus[32];
    assign sram_blwl_wl[1704] = wl_bus[32];
    assign sram_blwl_wl[1705] = wl_bus[32];
    assign sram_blwl_wl[1706] = wl_bus[32];
    assign sram_blwl_wl[1707] = wl_bus[32];
    assign sram_blwl_wl[1708] = wl_bus[32];
    assign sram_blwl_wl[1709] = wl_bus[32];
    assign sram_blwl_wl[1710] = wl_bus[32];
    assign sram_blwl_wl[1711] = wl_bus[32];
    assign sram_blwl_wl[1712] = wl_bus[32];
    assign sram_blwl_wl[1713] = wl_bus[32];
    assign sram_blwl_wl[1714] = wl_bus[32];
    assign sram_blwl_wl[1715] = wl_bus[32];
    assign sram_blwl_wl[1716] = wl_bus[33];
    assign sram_blwl_wl[1717] = wl_bus[33];
    assign sram_blwl_wl[1718] = wl_bus[33];
    assign sram_blwl_wl[1719] = wl_bus[33];
    assign sram_blwl_wl[1720] = wl_bus[33];
    assign sram_blwl_wl[1721] = wl_bus[33];
    assign sram_blwl_wl[1722] = wl_bus[33];
    assign sram_blwl_wl[1723] = wl_bus[33];
    assign sram_blwl_wl[1724] = wl_bus[33];
    assign sram_blwl_wl[1725] = wl_bus[33];
    assign sram_blwl_wl[1726] = wl_bus[33];
    assign sram_blwl_wl[1727] = wl_bus[33];
    assign sram_blwl_wl[1728] = wl_bus[33];
    assign sram_blwl_wl[1729] = wl_bus[33];
    assign sram_blwl_wl[1730] = wl_bus[33];
    assign sram_blwl_wl[1731] = wl_bus[33];
    assign sram_blwl_wl[1732] = wl_bus[33];
    assign sram_blwl_wl[1733] = wl_bus[33];
    assign sram_blwl_wl[1734] = wl_bus[33];
    assign sram_blwl_wl[1735] = wl_bus[33];
    assign sram_blwl_wl[1736] = wl_bus[33];
    assign sram_blwl_wl[1737] = wl_bus[33];
    assign sram_blwl_wl[1738] = wl_bus[33];
    assign sram_blwl_wl[1739] = wl_bus[33];
    assign sram_blwl_wl[1740] = wl_bus[33];
    assign sram_blwl_wl[1741] = wl_bus[33];
    assign sram_blwl_wl[1742] = wl_bus[33];
    assign sram_blwl_wl[1743] = wl_bus[33];
    assign sram_blwl_wl[1744] = wl_bus[33];
    assign sram_blwl_wl[1745] = wl_bus[33];
    assign sram_blwl_wl[1746] = wl_bus[33];
    assign sram_blwl_wl[1747] = wl_bus[33];
    assign sram_blwl_wl[1748] = wl_bus[33];
    assign sram_blwl_wl[1749] = wl_bus[33];
    assign sram_blwl_wl[1750] = wl_bus[33];
    assign sram_blwl_wl[1751] = wl_bus[33];
    assign sram_blwl_wl[1752] = wl_bus[33];
    assign sram_blwl_wl[1753] = wl_bus[33];
    assign sram_blwl_wl[1754] = wl_bus[33];
    assign sram_blwl_wl[1755] = wl_bus[33];
    assign sram_blwl_wl[1756] = wl_bus[33];
    assign sram_blwl_wl[1757] = wl_bus[33];
    assign sram_blwl_wl[1758] = wl_bus[33];
    assign sram_blwl_wl[1759] = wl_bus[33];
    assign sram_blwl_wl[1760] = wl_bus[33];
    assign sram_blwl_wl[1761] = wl_bus[33];
    assign sram_blwl_wl[1762] = wl_bus[33];
    assign sram_blwl_wl[1763] = wl_bus[33];
    assign sram_blwl_wl[1764] = wl_bus[33];
    assign sram_blwl_wl[1765] = wl_bus[33];
    assign sram_blwl_wl[1766] = wl_bus[33];
    assign sram_blwl_wl[1767] = wl_bus[33];
    assign sram_blwl_wl[1768] = wl_bus[34];
    assign sram_blwl_wl[1769] = wl_bus[34];
    assign sram_blwl_wl[1770] = wl_bus[34];
    assign sram_blwl_wl[1771] = wl_bus[34];
    assign sram_blwl_wl[1772] = wl_bus[34];
    assign sram_blwl_wl[1773] = wl_bus[34];
    assign sram_blwl_wl[1774] = wl_bus[34];
    assign sram_blwl_wl[1775] = wl_bus[34];
    assign sram_blwl_wl[1776] = wl_bus[34];
    assign sram_blwl_wl[1777] = wl_bus[34];
    assign sram_blwl_wl[1778] = wl_bus[34];
    assign sram_blwl_wl[1779] = wl_bus[34];
    assign sram_blwl_wl[1780] = wl_bus[34];
    assign sram_blwl_wl[1781] = wl_bus[34];
    assign sram_blwl_wl[1782] = wl_bus[34];
    assign sram_blwl_wl[1783] = wl_bus[34];
    assign sram_blwl_wl[1784] = wl_bus[34];
    assign sram_blwl_wl[1785] = wl_bus[34];
    assign sram_blwl_wl[1786] = wl_bus[34];
    assign sram_blwl_wl[1787] = wl_bus[34];
    assign sram_blwl_wl[1788] = wl_bus[34];
    assign sram_blwl_wl[1789] = wl_bus[34];
    assign sram_blwl_wl[1790] = wl_bus[34];
    assign sram_blwl_wl[1791] = wl_bus[34];
    assign sram_blwl_wl[1792] = wl_bus[34];
    assign sram_blwl_wl[1793] = wl_bus[34];
    assign sram_blwl_wl[1794] = wl_bus[34];
    assign sram_blwl_wl[1795] = wl_bus[34];
    assign sram_blwl_wl[1796] = wl_bus[34];
    assign sram_blwl_wl[1797] = wl_bus[34];
    assign sram_blwl_wl[1798] = wl_bus[34];
    assign sram_blwl_wl[1799] = wl_bus[34];
    assign sram_blwl_wl[1800] = wl_bus[34];
    assign sram_blwl_wl[1801] = wl_bus[34];
    assign sram_blwl_wl[1802] = wl_bus[34];
    assign sram_blwl_wl[1803] = wl_bus[34];
    assign sram_blwl_wl[1804] = wl_bus[34];
    assign sram_blwl_wl[1805] = wl_bus[34];
    assign sram_blwl_wl[1806] = wl_bus[34];
    assign sram_blwl_wl[1807] = wl_bus[34];
    assign sram_blwl_wl[1808] = wl_bus[34];
    assign sram_blwl_wl[1809] = wl_bus[34];
    assign sram_blwl_wl[1810] = wl_bus[34];
    assign sram_blwl_wl[1811] = wl_bus[34];
    assign sram_blwl_wl[1812] = wl_bus[34];
    assign sram_blwl_wl[1813] = wl_bus[34];
    assign sram_blwl_wl[1814] = wl_bus[34];
    assign sram_blwl_wl[1815] = wl_bus[34];
    assign sram_blwl_wl[1816] = wl_bus[34];
    assign sram_blwl_wl[1817] = wl_bus[34];
    assign sram_blwl_wl[1818] = wl_bus[34];
    assign sram_blwl_wl[1819] = wl_bus[34];
    assign sram_blwl_wl[1820] = wl_bus[35];
    assign sram_blwl_wl[1821] = wl_bus[35];
    assign sram_blwl_wl[1822] = wl_bus[35];
    assign sram_blwl_wl[1823] = wl_bus[35];
    assign sram_blwl_wl[1824] = wl_bus[35];
    assign sram_blwl_wl[1825] = wl_bus[35];
    assign sram_blwl_wl[1826] = wl_bus[35];
    assign sram_blwl_wl[1827] = wl_bus[35];
    assign sram_blwl_wl[1828] = wl_bus[35];
    assign sram_blwl_wl[1829] = wl_bus[35];
    assign sram_blwl_wl[1830] = wl_bus[35];
    assign sram_blwl_wl[1831] = wl_bus[35];
    assign sram_blwl_wl[1832] = wl_bus[35];
    assign sram_blwl_wl[1833] = wl_bus[35];
    assign sram_blwl_wl[1834] = wl_bus[35];
    assign sram_blwl_wl[1835] = wl_bus[35];
    assign sram_blwl_wl[1836] = wl_bus[35];
    assign sram_blwl_wl[1837] = wl_bus[35];
    assign sram_blwl_wl[1838] = wl_bus[35];
    assign sram_blwl_wl[1839] = wl_bus[35];
    assign sram_blwl_wl[1840] = wl_bus[35];
    assign sram_blwl_wl[1841] = wl_bus[35];
    assign sram_blwl_wl[1842] = wl_bus[35];
    assign sram_blwl_wl[1843] = wl_bus[35];
    assign sram_blwl_wl[1844] = wl_bus[35];
    assign sram_blwl_wl[1845] = wl_bus[35];
    assign sram_blwl_wl[1846] = wl_bus[35];
    assign sram_blwl_wl[1847] = wl_bus[35];
    assign sram_blwl_wl[1848] = wl_bus[35];
    assign sram_blwl_wl[1849] = wl_bus[35];
    assign sram_blwl_wl[1850] = wl_bus[35];
    assign sram_blwl_wl[1851] = wl_bus[35];
    assign sram_blwl_wl[1852] = wl_bus[35];
    assign sram_blwl_wl[1853] = wl_bus[35];
    assign sram_blwl_wl[1854] = wl_bus[35];
    assign sram_blwl_wl[1855] = wl_bus[35];
    assign sram_blwl_wl[1856] = wl_bus[35];
    assign sram_blwl_wl[1857] = wl_bus[35];
    assign sram_blwl_wl[1858] = wl_bus[35];
    assign sram_blwl_wl[1859] = wl_bus[35];
    assign sram_blwl_wl[1860] = wl_bus[35];
    assign sram_blwl_wl[1861] = wl_bus[35];
    assign sram_blwl_wl[1862] = wl_bus[35];
    assign sram_blwl_wl[1863] = wl_bus[35];
    assign sram_blwl_wl[1864] = wl_bus[35];
    assign sram_blwl_wl[1865] = wl_bus[35];
    assign sram_blwl_wl[1866] = wl_bus[35];
    assign sram_blwl_wl[1867] = wl_bus[35];
    assign sram_blwl_wl[1868] = wl_bus[35];
    assign sram_blwl_wl[1869] = wl_bus[35];
    assign sram_blwl_wl[1870] = wl_bus[35];
    assign sram_blwl_wl[1871] = wl_bus[35];
    assign sram_blwl_wl[1872] = wl_bus[36];
    assign sram_blwl_wl[1873] = wl_bus[36];
    assign sram_blwl_wl[1874] = wl_bus[36];
    assign sram_blwl_wl[1875] = wl_bus[36];
    assign sram_blwl_wl[1876] = wl_bus[36];
    assign sram_blwl_wl[1877] = wl_bus[36];
    assign sram_blwl_wl[1878] = wl_bus[36];
    assign sram_blwl_wl[1879] = wl_bus[36];
    assign sram_blwl_wl[1880] = wl_bus[36];
    assign sram_blwl_wl[1881] = wl_bus[36];
    assign sram_blwl_wl[1882] = wl_bus[36];
    assign sram_blwl_wl[1883] = wl_bus[36];
    assign sram_blwl_wl[1884] = wl_bus[36];
    assign sram_blwl_wl[1885] = wl_bus[36];
    assign sram_blwl_wl[1886] = wl_bus[36];
    assign sram_blwl_wl[1887] = wl_bus[36];
    assign sram_blwl_wl[1888] = wl_bus[36];
    assign sram_blwl_wl[1889] = wl_bus[36];
    assign sram_blwl_wl[1890] = wl_bus[36];
    assign sram_blwl_wl[1891] = wl_bus[36];
    assign sram_blwl_wl[1892] = wl_bus[36];
    assign sram_blwl_wl[1893] = wl_bus[36];
    assign sram_blwl_wl[1894] = wl_bus[36];
    assign sram_blwl_wl[1895] = wl_bus[36];
    assign sram_blwl_wl[1896] = wl_bus[36];
    assign sram_blwl_wl[1897] = wl_bus[36];
    assign sram_blwl_wl[1898] = wl_bus[36];
    assign sram_blwl_wl[1899] = wl_bus[36];
    assign sram_blwl_wl[1900] = wl_bus[36];
    assign sram_blwl_wl[1901] = wl_bus[36];
    assign sram_blwl_wl[1902] = wl_bus[36];
    assign sram_blwl_wl[1903] = wl_bus[36];
    assign sram_blwl_wl[1904] = wl_bus[36];
    assign sram_blwl_wl[1905] = wl_bus[36];
    assign sram_blwl_wl[1906] = wl_bus[36];
    assign sram_blwl_wl[1907] = wl_bus[36];
    assign sram_blwl_wl[1908] = wl_bus[36];
    assign sram_blwl_wl[1909] = wl_bus[36];
    assign sram_blwl_wl[1910] = wl_bus[36];
    assign sram_blwl_wl[1911] = wl_bus[36];
    assign sram_blwl_wl[1912] = wl_bus[36];
    assign sram_blwl_wl[1913] = wl_bus[36];
    assign sram_blwl_wl[1914] = wl_bus[36];
    assign sram_blwl_wl[1915] = wl_bus[36];
    assign sram_blwl_wl[1916] = wl_bus[36];
    assign sram_blwl_wl[1917] = wl_bus[36];
    assign sram_blwl_wl[1918] = wl_bus[36];
    assign sram_blwl_wl[1919] = wl_bus[36];
    assign sram_blwl_wl[1920] = wl_bus[36];
    assign sram_blwl_wl[1921] = wl_bus[36];
    assign sram_blwl_wl[1922] = wl_bus[36];
    assign sram_blwl_wl[1923] = wl_bus[36];
    assign sram_blwl_wl[1924] = wl_bus[37];
    assign sram_blwl_wl[1925] = wl_bus[37];
    assign sram_blwl_wl[1926] = wl_bus[37];
    assign sram_blwl_wl[1927] = wl_bus[37];
    assign sram_blwl_wl[1928] = wl_bus[37];
    assign sram_blwl_wl[1929] = wl_bus[37];
    assign sram_blwl_wl[1930] = wl_bus[37];
    assign sram_blwl_wl[1931] = wl_bus[37];
    assign sram_blwl_wl[1932] = wl_bus[37];
    assign sram_blwl_wl[1933] = wl_bus[37];
    assign sram_blwl_wl[1934] = wl_bus[37];
    assign sram_blwl_wl[1935] = wl_bus[37];
    assign sram_blwl_wl[1936] = wl_bus[37];
    assign sram_blwl_wl[1937] = wl_bus[37];
    assign sram_blwl_wl[1938] = wl_bus[37];
    assign sram_blwl_wl[1939] = wl_bus[37];
    assign sram_blwl_wl[1940] = wl_bus[37];
    assign sram_blwl_wl[1941] = wl_bus[37];
    assign sram_blwl_wl[1942] = wl_bus[37];
    assign sram_blwl_wl[1943] = wl_bus[37];
    assign sram_blwl_wl[1944] = wl_bus[37];
    assign sram_blwl_wl[1945] = wl_bus[37];
    assign sram_blwl_wl[1946] = wl_bus[37];
    assign sram_blwl_wl[1947] = wl_bus[37];
    assign sram_blwl_wl[1948] = wl_bus[37];
    assign sram_blwl_wl[1949] = wl_bus[37];
    assign sram_blwl_wl[1950] = wl_bus[37];
    assign sram_blwl_wl[1951] = wl_bus[37];
    assign sram_blwl_wl[1952] = wl_bus[37];
    assign sram_blwl_wl[1953] = wl_bus[37];
    assign sram_blwl_wl[1954] = wl_bus[37];
    assign sram_blwl_wl[1955] = wl_bus[37];
    assign sram_blwl_wl[1956] = wl_bus[37];
    assign sram_blwl_wl[1957] = wl_bus[37];
    assign sram_blwl_wl[1958] = wl_bus[37];
    assign sram_blwl_wl[1959] = wl_bus[37];
    assign sram_blwl_wl[1960] = wl_bus[37];
    assign sram_blwl_wl[1961] = wl_bus[37];
    assign sram_blwl_wl[1962] = wl_bus[37];
    assign sram_blwl_wl[1963] = wl_bus[37];
    assign sram_blwl_wl[1964] = wl_bus[37];
    assign sram_blwl_wl[1965] = wl_bus[37];
    assign sram_blwl_wl[1966] = wl_bus[37];
    assign sram_blwl_wl[1967] = wl_bus[37];
    assign sram_blwl_wl[1968] = wl_bus[37];
    assign sram_blwl_wl[1969] = wl_bus[37];
    assign sram_blwl_wl[1970] = wl_bus[37];
    assign sram_blwl_wl[1971] = wl_bus[37];
    assign sram_blwl_wl[1972] = wl_bus[37];
    assign sram_blwl_wl[1973] = wl_bus[37];
    assign sram_blwl_wl[1974] = wl_bus[37];
    assign sram_blwl_wl[1975] = wl_bus[37];
    assign sram_blwl_wl[1976] = wl_bus[38];
    assign sram_blwl_wl[1977] = wl_bus[38];
    assign sram_blwl_wl[1978] = wl_bus[38];
    assign sram_blwl_wl[1979] = wl_bus[38];
    assign sram_blwl_wl[1980] = wl_bus[38];
    assign sram_blwl_wl[1981] = wl_bus[38];
    assign sram_blwl_wl[1982] = wl_bus[38];
    assign sram_blwl_wl[1983] = wl_bus[38];
    assign sram_blwl_wl[1984] = wl_bus[38];
    assign sram_blwl_wl[1985] = wl_bus[38];
    assign sram_blwl_wl[1986] = wl_bus[38];
    assign sram_blwl_wl[1987] = wl_bus[38];
    assign sram_blwl_wl[1988] = wl_bus[38];
    assign sram_blwl_wl[1989] = wl_bus[38];
    assign sram_blwl_wl[1990] = wl_bus[38];
    assign sram_blwl_wl[1991] = wl_bus[38];
    assign sram_blwl_wl[1992] = wl_bus[38];
    assign sram_blwl_wl[1993] = wl_bus[38];
    assign sram_blwl_wl[1994] = wl_bus[38];
    assign sram_blwl_wl[1995] = wl_bus[38];
    assign sram_blwl_wl[1996] = wl_bus[38];
    assign sram_blwl_wl[1997] = wl_bus[38];
    assign sram_blwl_wl[1998] = wl_bus[38];
    assign sram_blwl_wl[1999] = wl_bus[38];
    assign sram_blwl_wl[2000] = wl_bus[38];
    assign sram_blwl_wl[2001] = wl_bus[38];
    assign sram_blwl_wl[2002] = wl_bus[38];
    assign sram_blwl_wl[2003] = wl_bus[38];
    assign sram_blwl_wl[2004] = wl_bus[38];
    assign sram_blwl_wl[2005] = wl_bus[38];
    assign sram_blwl_wl[2006] = wl_bus[38];
    assign sram_blwl_wl[2007] = wl_bus[38];
    assign sram_blwl_wl[2008] = wl_bus[38];
    assign sram_blwl_wl[2009] = wl_bus[38];
    assign sram_blwl_wl[2010] = wl_bus[38];
    assign sram_blwl_wl[2011] = wl_bus[38];
    assign sram_blwl_wl[2012] = wl_bus[38];
    assign sram_blwl_wl[2013] = wl_bus[38];
    assign sram_blwl_wl[2014] = wl_bus[38];
    assign sram_blwl_wl[2015] = wl_bus[38];
    assign sram_blwl_wl[2016] = wl_bus[38];
    assign sram_blwl_wl[2017] = wl_bus[38];
    assign sram_blwl_wl[2018] = wl_bus[38];
    assign sram_blwl_wl[2019] = wl_bus[38];
    assign sram_blwl_wl[2020] = wl_bus[38];
    assign sram_blwl_wl[2021] = wl_bus[38];
    assign sram_blwl_wl[2022] = wl_bus[38];
    assign sram_blwl_wl[2023] = wl_bus[38];
    assign sram_blwl_wl[2024] = wl_bus[38];
    assign sram_blwl_wl[2025] = wl_bus[38];
    assign sram_blwl_wl[2026] = wl_bus[38];
    assign sram_blwl_wl[2027] = wl_bus[38];
    assign sram_blwl_wl[2028] = wl_bus[39];
    assign sram_blwl_wl[2029] = wl_bus[39];
    assign sram_blwl_wl[2030] = wl_bus[39];
    assign sram_blwl_wl[2031] = wl_bus[39];
    assign sram_blwl_wl[2032] = wl_bus[39];
    assign sram_blwl_wl[2033] = wl_bus[39];
    assign sram_blwl_wl[2034] = wl_bus[39];
    assign sram_blwl_wl[2035] = wl_bus[39];
    assign sram_blwl_wl[2036] = wl_bus[39];
    assign sram_blwl_wl[2037] = wl_bus[39];
    assign sram_blwl_wl[2038] = wl_bus[39];
    assign sram_blwl_wl[2039] = wl_bus[39];
    assign sram_blwl_wl[2040] = wl_bus[39];
    assign sram_blwl_wl[2041] = wl_bus[39];
    assign sram_blwl_wl[2042] = wl_bus[39];
    assign sram_blwl_wl[2043] = wl_bus[39];
    assign sram_blwl_wl[2044] = wl_bus[39];
    assign sram_blwl_wl[2045] = wl_bus[39];
    assign sram_blwl_wl[2046] = wl_bus[39];
    assign sram_blwl_wl[2047] = wl_bus[39];
    assign sram_blwl_wl[2048] = wl_bus[39];
    assign sram_blwl_wl[2049] = wl_bus[39];
    assign sram_blwl_wl[2050] = wl_bus[39];
    assign sram_blwl_wl[2051] = wl_bus[39];
    assign sram_blwl_wl[2052] = wl_bus[39];
    assign sram_blwl_wl[2053] = wl_bus[39];
    assign sram_blwl_wl[2054] = wl_bus[39];
    assign sram_blwl_wl[2055] = wl_bus[39];
    assign sram_blwl_wl[2056] = wl_bus[39];
    assign sram_blwl_wl[2057] = wl_bus[39];
    assign sram_blwl_wl[2058] = wl_bus[39];
    assign sram_blwl_wl[2059] = wl_bus[39];
    assign sram_blwl_wl[2060] = wl_bus[39];
    assign sram_blwl_wl[2061] = wl_bus[39];
    assign sram_blwl_wl[2062] = wl_bus[39];
    assign sram_blwl_wl[2063] = wl_bus[39];
    assign sram_blwl_wl[2064] = wl_bus[39];
    assign sram_blwl_wl[2065] = wl_bus[39];
    assign sram_blwl_wl[2066] = wl_bus[39];
    assign sram_blwl_wl[2067] = wl_bus[39];
    assign sram_blwl_wl[2068] = wl_bus[39];
    assign sram_blwl_wl[2069] = wl_bus[39];
    assign sram_blwl_wl[2070] = wl_bus[39];
    assign sram_blwl_wl[2071] = wl_bus[39];
    assign sram_blwl_wl[2072] = wl_bus[39];
    assign sram_blwl_wl[2073] = wl_bus[39];
    assign sram_blwl_wl[2074] = wl_bus[39];
    assign sram_blwl_wl[2075] = wl_bus[39];
    assign sram_blwl_wl[2076] = wl_bus[39];
    assign sram_blwl_wl[2077] = wl_bus[39];
    assign sram_blwl_wl[2078] = wl_bus[39];
    assign sram_blwl_wl[2079] = wl_bus[39];
    assign sram_blwl_wl[2080] = wl_bus[40];
    assign sram_blwl_wl[2081] = wl_bus[40];
    assign sram_blwl_wl[2082] = wl_bus[40];
    assign sram_blwl_wl[2083] = wl_bus[40];
    assign sram_blwl_wl[2084] = wl_bus[40];
    assign sram_blwl_wl[2085] = wl_bus[40];
    assign sram_blwl_wl[2086] = wl_bus[40];
    assign sram_blwl_wl[2087] = wl_bus[40];
    assign sram_blwl_wl[2088] = wl_bus[40];
    assign sram_blwl_wl[2089] = wl_bus[40];
    assign sram_blwl_wl[2090] = wl_bus[40];
    assign sram_blwl_wl[2091] = wl_bus[40];
    assign sram_blwl_wl[2092] = wl_bus[40];
    assign sram_blwl_wl[2093] = wl_bus[40];
    assign sram_blwl_wl[2094] = wl_bus[40];
    assign sram_blwl_wl[2095] = wl_bus[40];
    assign sram_blwl_wl[2096] = wl_bus[40];
    assign sram_blwl_wl[2097] = wl_bus[40];
    assign sram_blwl_wl[2098] = wl_bus[40];
    assign sram_blwl_wl[2099] = wl_bus[40];
    assign sram_blwl_wl[2100] = wl_bus[40];
    assign sram_blwl_wl[2101] = wl_bus[40];
    assign sram_blwl_wl[2102] = wl_bus[40];
    assign sram_blwl_wl[2103] = wl_bus[40];
    assign sram_blwl_wl[2104] = wl_bus[40];
    assign sram_blwl_wl[2105] = wl_bus[40];
    assign sram_blwl_wl[2106] = wl_bus[40];
    assign sram_blwl_wl[2107] = wl_bus[40];
    assign sram_blwl_wl[2108] = wl_bus[40];
    assign sram_blwl_wl[2109] = wl_bus[40];
    assign sram_blwl_wl[2110] = wl_bus[40];
    assign sram_blwl_wl[2111] = wl_bus[40];
    assign sram_blwl_wl[2112] = wl_bus[40];
    assign sram_blwl_wl[2113] = wl_bus[40];
    assign sram_blwl_wl[2114] = wl_bus[40];
    assign sram_blwl_wl[2115] = wl_bus[40];
    assign sram_blwl_wl[2116] = wl_bus[40];
    assign sram_blwl_wl[2117] = wl_bus[40];
    assign sram_blwl_wl[2118] = wl_bus[40];
    assign sram_blwl_wl[2119] = wl_bus[40];
    assign sram_blwl_wl[2120] = wl_bus[40];
    assign sram_blwl_wl[2121] = wl_bus[40];
    assign sram_blwl_wl[2122] = wl_bus[40];
    assign sram_blwl_wl[2123] = wl_bus[40];
    assign sram_blwl_wl[2124] = wl_bus[40];
    assign sram_blwl_wl[2125] = wl_bus[40];
    assign sram_blwl_wl[2126] = wl_bus[40];
    assign sram_blwl_wl[2127] = wl_bus[40];
    assign sram_blwl_wl[2128] = wl_bus[40];
    assign sram_blwl_wl[2129] = wl_bus[40];
    assign sram_blwl_wl[2130] = wl_bus[40];
    assign sram_blwl_wl[2131] = wl_bus[40];
    assign sram_blwl_wl[2132] = wl_bus[41];
    assign sram_blwl_wl[2133] = wl_bus[41];
    assign sram_blwl_wl[2134] = wl_bus[41];
    assign sram_blwl_wl[2135] = wl_bus[41];
    assign sram_blwl_wl[2136] = wl_bus[41];
    assign sram_blwl_wl[2137] = wl_bus[41];
    assign sram_blwl_wl[2138] = wl_bus[41];
    assign sram_blwl_wl[2139] = wl_bus[41];
    assign sram_blwl_wl[2140] = wl_bus[41];
    assign sram_blwl_wl[2141] = wl_bus[41];
    assign sram_blwl_wl[2142] = wl_bus[41];
    assign sram_blwl_wl[2143] = wl_bus[41];
    assign sram_blwl_wl[2144] = wl_bus[41];
    assign sram_blwl_wl[2145] = wl_bus[41];
    assign sram_blwl_wl[2146] = wl_bus[41];
    assign sram_blwl_wl[2147] = wl_bus[41];
    assign sram_blwl_wl[2148] = wl_bus[41];
    assign sram_blwl_wl[2149] = wl_bus[41];
    assign sram_blwl_wl[2150] = wl_bus[41];
    assign sram_blwl_wl[2151] = wl_bus[41];
    assign sram_blwl_wl[2152] = wl_bus[41];
    assign sram_blwl_wl[2153] = wl_bus[41];
    assign sram_blwl_wl[2154] = wl_bus[41];
    assign sram_blwl_wl[2155] = wl_bus[41];
    assign sram_blwl_wl[2156] = wl_bus[41];
    assign sram_blwl_wl[2157] = wl_bus[41];
    assign sram_blwl_wl[2158] = wl_bus[41];
    assign sram_blwl_wl[2159] = wl_bus[41];
    assign sram_blwl_wl[2160] = wl_bus[41];
    assign sram_blwl_wl[2161] = wl_bus[41];
    assign sram_blwl_wl[2162] = wl_bus[41];
    assign sram_blwl_wl[2163] = wl_bus[41];
    assign sram_blwl_wl[2164] = wl_bus[41];
    assign sram_blwl_wl[2165] = wl_bus[41];
    assign sram_blwl_wl[2166] = wl_bus[41];
    assign sram_blwl_wl[2167] = wl_bus[41];
    assign sram_blwl_wl[2168] = wl_bus[41];
    assign sram_blwl_wl[2169] = wl_bus[41];
    assign sram_blwl_wl[2170] = wl_bus[41];
    assign sram_blwl_wl[2171] = wl_bus[41];
    assign sram_blwl_wl[2172] = wl_bus[41];
    assign sram_blwl_wl[2173] = wl_bus[41];
    assign sram_blwl_wl[2174] = wl_bus[41];
    assign sram_blwl_wl[2175] = wl_bus[41];
    assign sram_blwl_wl[2176] = wl_bus[41];
    assign sram_blwl_wl[2177] = wl_bus[41];
    assign sram_blwl_wl[2178] = wl_bus[41];
    assign sram_blwl_wl[2179] = wl_bus[41];
    assign sram_blwl_wl[2180] = wl_bus[41];
    assign sram_blwl_wl[2181] = wl_bus[41];
    assign sram_blwl_wl[2182] = wl_bus[41];
    assign sram_blwl_wl[2183] = wl_bus[41];
    assign sram_blwl_wl[2184] = wl_bus[42];
    assign sram_blwl_wl[2185] = wl_bus[42];
    assign sram_blwl_wl[2186] = wl_bus[42];
    assign sram_blwl_wl[2187] = wl_bus[42];
    assign sram_blwl_wl[2188] = wl_bus[42];
    assign sram_blwl_wl[2189] = wl_bus[42];
    assign sram_blwl_wl[2190] = wl_bus[42];
    assign sram_blwl_wl[2191] = wl_bus[42];
    assign sram_blwl_wl[2192] = wl_bus[42];
    assign sram_blwl_wl[2193] = wl_bus[42];
    assign sram_blwl_wl[2194] = wl_bus[42];
    assign sram_blwl_wl[2195] = wl_bus[42];
    assign sram_blwl_wl[2196] = wl_bus[42];
    assign sram_blwl_wl[2197] = wl_bus[42];
    assign sram_blwl_wl[2198] = wl_bus[42];
    assign sram_blwl_wl[2199] = wl_bus[42];
    assign sram_blwl_wl[2200] = wl_bus[42];
    assign sram_blwl_wl[2201] = wl_bus[42];
    assign sram_blwl_wl[2202] = wl_bus[42];
    assign sram_blwl_wl[2203] = wl_bus[42];
    assign sram_blwl_wl[2204] = wl_bus[42];
    assign sram_blwl_wl[2205] = wl_bus[42];
    assign sram_blwl_wl[2206] = wl_bus[42];
    assign sram_blwl_wl[2207] = wl_bus[42];
    assign sram_blwl_wl[2208] = wl_bus[42];
    assign sram_blwl_wl[2209] = wl_bus[42];
    assign sram_blwl_wl[2210] = wl_bus[42];
    assign sram_blwl_wl[2211] = wl_bus[42];
    assign sram_blwl_wl[2212] = wl_bus[42];
    assign sram_blwl_wl[2213] = wl_bus[42];
    assign sram_blwl_wl[2214] = wl_bus[42];
    assign sram_blwl_wl[2215] = wl_bus[42];
    assign sram_blwl_wl[2216] = wl_bus[42];
    assign sram_blwl_wl[2217] = wl_bus[42];
    assign sram_blwl_wl[2218] = wl_bus[42];
    assign sram_blwl_wl[2219] = wl_bus[42];
    assign sram_blwl_wl[2220] = wl_bus[42];
    assign sram_blwl_wl[2221] = wl_bus[42];
    assign sram_blwl_wl[2222] = wl_bus[42];
    assign sram_blwl_wl[2223] = wl_bus[42];
    assign sram_blwl_wl[2224] = wl_bus[42];
    assign sram_blwl_wl[2225] = wl_bus[42];
    assign sram_blwl_wl[2226] = wl_bus[42];
    assign sram_blwl_wl[2227] = wl_bus[42];
    assign sram_blwl_wl[2228] = wl_bus[42];
    assign sram_blwl_wl[2229] = wl_bus[42];
    assign sram_blwl_wl[2230] = wl_bus[42];
    assign sram_blwl_wl[2231] = wl_bus[42];
    assign sram_blwl_wl[2232] = wl_bus[42];
    assign sram_blwl_wl[2233] = wl_bus[42];
    assign sram_blwl_wl[2234] = wl_bus[42];
    assign sram_blwl_wl[2235] = wl_bus[42];
    assign sram_blwl_wl[2236] = wl_bus[43];
    assign sram_blwl_wl[2237] = wl_bus[43];
    assign sram_blwl_wl[2238] = wl_bus[43];
    assign sram_blwl_wl[2239] = wl_bus[43];
    assign sram_blwl_wl[2240] = wl_bus[43];
    assign sram_blwl_wl[2241] = wl_bus[43];
    assign sram_blwl_wl[2242] = wl_bus[43];
    assign sram_blwl_wl[2243] = wl_bus[43];
    assign sram_blwl_wl[2244] = wl_bus[43];
    assign sram_blwl_wl[2245] = wl_bus[43];
    assign sram_blwl_wl[2246] = wl_bus[43];
    assign sram_blwl_wl[2247] = wl_bus[43];
    assign sram_blwl_wl[2248] = wl_bus[43];
    assign sram_blwl_wl[2249] = wl_bus[43];
    assign sram_blwl_wl[2250] = wl_bus[43];
    assign sram_blwl_wl[2251] = wl_bus[43];
    assign sram_blwl_wl[2252] = wl_bus[43];
    assign sram_blwl_wl[2253] = wl_bus[43];
    assign sram_blwl_wl[2254] = wl_bus[43];
    assign sram_blwl_wl[2255] = wl_bus[43];
    assign sram_blwl_wl[2256] = wl_bus[43];
    assign sram_blwl_wl[2257] = wl_bus[43];
    assign sram_blwl_wl[2258] = wl_bus[43];
    assign sram_blwl_wl[2259] = wl_bus[43];
    assign sram_blwl_wl[2260] = wl_bus[43];
    assign sram_blwl_wl[2261] = wl_bus[43];
    assign sram_blwl_wl[2262] = wl_bus[43];
    assign sram_blwl_wl[2263] = wl_bus[43];
    assign sram_blwl_wl[2264] = wl_bus[43];
    assign sram_blwl_wl[2265] = wl_bus[43];
    assign sram_blwl_wl[2266] = wl_bus[43];
    assign sram_blwl_wl[2267] = wl_bus[43];
    assign sram_blwl_wl[2268] = wl_bus[43];
    assign sram_blwl_wl[2269] = wl_bus[43];
    assign sram_blwl_wl[2270] = wl_bus[43];
    assign sram_blwl_wl[2271] = wl_bus[43];
    assign sram_blwl_wl[2272] = wl_bus[43];
    assign sram_blwl_wl[2273] = wl_bus[43];
    assign sram_blwl_wl[2274] = wl_bus[43];
    assign sram_blwl_wl[2275] = wl_bus[43];
    assign sram_blwl_wl[2276] = wl_bus[43];
    assign sram_blwl_wl[2277] = wl_bus[43];
    assign sram_blwl_wl[2278] = wl_bus[43];
    assign sram_blwl_wl[2279] = wl_bus[43];
    assign sram_blwl_wl[2280] = wl_bus[43];
    assign sram_blwl_wl[2281] = wl_bus[43];
    assign sram_blwl_wl[2282] = wl_bus[43];
    assign sram_blwl_wl[2283] = wl_bus[43];
    assign sram_blwl_wl[2284] = wl_bus[43];
    assign sram_blwl_wl[2285] = wl_bus[43];
    assign sram_blwl_wl[2286] = wl_bus[43];
    assign sram_blwl_wl[2287] = wl_bus[43];
    assign sram_blwl_wl[2288] = wl_bus[44];
    assign sram_blwl_wl[2289] = wl_bus[44];
    assign sram_blwl_wl[2290] = wl_bus[44];
    assign sram_blwl_wl[2291] = wl_bus[44];
    assign sram_blwl_wl[2292] = wl_bus[44];
    assign sram_blwl_wl[2293] = wl_bus[44];
    assign sram_blwl_wl[2294] = wl_bus[44];
    assign sram_blwl_wl[2295] = wl_bus[44];
    assign sram_blwl_wl[2296] = wl_bus[44];
    assign sram_blwl_wl[2297] = wl_bus[44];
    assign sram_blwl_wl[2298] = wl_bus[44];
    assign sram_blwl_wl[2299] = wl_bus[44];
    assign sram_blwl_wl[2300] = wl_bus[44];
    assign sram_blwl_wl[2301] = wl_bus[44];
    assign sram_blwl_wl[2302] = wl_bus[44];
    assign sram_blwl_wl[2303] = wl_bus[44];
    assign sram_blwl_wl[2304] = wl_bus[44];
    assign sram_blwl_wl[2305] = wl_bus[44];
    assign sram_blwl_wl[2306] = wl_bus[44];
    assign sram_blwl_wl[2307] = wl_bus[44];
    assign sram_blwl_wl[2308] = wl_bus[44];
    assign sram_blwl_wl[2309] = wl_bus[44];
    assign sram_blwl_wl[2310] = wl_bus[44];
    assign sram_blwl_wl[2311] = wl_bus[44];
    assign sram_blwl_wl[2312] = wl_bus[44];
    assign sram_blwl_wl[2313] = wl_bus[44];
    assign sram_blwl_wl[2314] = wl_bus[44];
    assign sram_blwl_wl[2315] = wl_bus[44];
    assign sram_blwl_wl[2316] = wl_bus[44];
    assign sram_blwl_wl[2317] = wl_bus[44];
    assign sram_blwl_wl[2318] = wl_bus[44];
    assign sram_blwl_wl[2319] = wl_bus[44];
    assign sram_blwl_wl[2320] = wl_bus[44];
    assign sram_blwl_wl[2321] = wl_bus[44];
    assign sram_blwl_wl[2322] = wl_bus[44];
    assign sram_blwl_wl[2323] = wl_bus[44];
    assign sram_blwl_wl[2324] = wl_bus[44];
    assign sram_blwl_wl[2325] = wl_bus[44];
    assign sram_blwl_wl[2326] = wl_bus[44];
    assign sram_blwl_wl[2327] = wl_bus[44];
    assign sram_blwl_wl[2328] = wl_bus[44];
    assign sram_blwl_wl[2329] = wl_bus[44];
    assign sram_blwl_wl[2330] = wl_bus[44];
    assign sram_blwl_wl[2331] = wl_bus[44];
    assign sram_blwl_wl[2332] = wl_bus[44];
    assign sram_blwl_wl[2333] = wl_bus[44];
    assign sram_blwl_wl[2334] = wl_bus[44];
    assign sram_blwl_wl[2335] = wl_bus[44];
    assign sram_blwl_wl[2336] = wl_bus[44];
    assign sram_blwl_wl[2337] = wl_bus[44];
    assign sram_blwl_wl[2338] = wl_bus[44];
    assign sram_blwl_wl[2339] = wl_bus[44];
    assign sram_blwl_wl[2340] = wl_bus[45];
    assign sram_blwl_wl[2341] = wl_bus[45];
    assign sram_blwl_wl[2342] = wl_bus[45];
    assign sram_blwl_wl[2343] = wl_bus[45];
    assign sram_blwl_wl[2344] = wl_bus[45];
    assign sram_blwl_wl[2345] = wl_bus[45];
    assign sram_blwl_wl[2346] = wl_bus[45];
    assign sram_blwl_wl[2347] = wl_bus[45];
    assign sram_blwl_wl[2348] = wl_bus[45];
    assign sram_blwl_wl[2349] = wl_bus[45];
    assign sram_blwl_wl[2350] = wl_bus[45];
    assign sram_blwl_wl[2351] = wl_bus[45];
    assign sram_blwl_wl[2352] = wl_bus[45];
    assign sram_blwl_wl[2353] = wl_bus[45];
    assign sram_blwl_wl[2354] = wl_bus[45];
    assign sram_blwl_wl[2355] = wl_bus[45];
    assign sram_blwl_wl[2356] = wl_bus[45];
    assign sram_blwl_wl[2357] = wl_bus[45];
    assign sram_blwl_wl[2358] = wl_bus[45];
    assign sram_blwl_wl[2359] = wl_bus[45];
    assign sram_blwl_wl[2360] = wl_bus[45];
    assign sram_blwl_wl[2361] = wl_bus[45];
    assign sram_blwl_wl[2362] = wl_bus[45];
    assign sram_blwl_wl[2363] = wl_bus[45];
    assign sram_blwl_wl[2364] = wl_bus[45];
    assign sram_blwl_wl[2365] = wl_bus[45];
    assign sram_blwl_wl[2366] = wl_bus[45];
    assign sram_blwl_wl[2367] = wl_bus[45];
    assign sram_blwl_wl[2368] = wl_bus[45];
    assign sram_blwl_wl[2369] = wl_bus[45];
    assign sram_blwl_wl[2370] = wl_bus[45];
    assign sram_blwl_wl[2371] = wl_bus[45];
    assign sram_blwl_wl[2372] = wl_bus[45];
    assign sram_blwl_wl[2373] = wl_bus[45];
    assign sram_blwl_wl[2374] = wl_bus[45];
    assign sram_blwl_wl[2375] = wl_bus[45];
    assign sram_blwl_wl[2376] = wl_bus[45];
    assign sram_blwl_wl[2377] = wl_bus[45];
    assign sram_blwl_wl[2378] = wl_bus[45];
    assign sram_blwl_wl[2379] = wl_bus[45];
    assign sram_blwl_wl[2380] = wl_bus[45];
    assign sram_blwl_wl[2381] = wl_bus[45];
    assign sram_blwl_wl[2382] = wl_bus[45];
    assign sram_blwl_wl[2383] = wl_bus[45];
    assign sram_blwl_wl[2384] = wl_bus[45];
    assign sram_blwl_wl[2385] = wl_bus[45];
    assign sram_blwl_wl[2386] = wl_bus[45];
    assign sram_blwl_wl[2387] = wl_bus[45];
    assign sram_blwl_wl[2388] = wl_bus[45];
    assign sram_blwl_wl[2389] = wl_bus[45];
    assign sram_blwl_wl[2390] = wl_bus[45];
    assign sram_blwl_wl[2391] = wl_bus[45];
    assign sram_blwl_wl[2392] = wl_bus[46];
    assign sram_blwl_wl[2393] = wl_bus[46];
    assign sram_blwl_wl[2394] = wl_bus[46];
    assign sram_blwl_wl[2395] = wl_bus[46];
    assign sram_blwl_wl[2396] = wl_bus[46];
    assign sram_blwl_wl[2397] = wl_bus[46];
    assign sram_blwl_wl[2398] = wl_bus[46];
    assign sram_blwl_wl[2399] = wl_bus[46];
    assign sram_blwl_wl[2400] = wl_bus[46];
    assign sram_blwl_wl[2401] = wl_bus[46];
    assign sram_blwl_wl[2402] = wl_bus[46];
    assign sram_blwl_wl[2403] = wl_bus[46];
    assign sram_blwl_wl[2404] = wl_bus[46];
    assign sram_blwl_wl[2405] = wl_bus[46];
    assign sram_blwl_wl[2406] = wl_bus[46];
    assign sram_blwl_wl[2407] = wl_bus[46];
    assign sram_blwl_wl[2408] = wl_bus[46];
    assign sram_blwl_wl[2409] = wl_bus[46];
    assign sram_blwl_wl[2410] = wl_bus[46];
    assign sram_blwl_wl[2411] = wl_bus[46];
    assign sram_blwl_wl[2412] = wl_bus[46];
    assign sram_blwl_wl[2413] = wl_bus[46];
    assign sram_blwl_wl[2414] = wl_bus[46];
    assign sram_blwl_wl[2415] = wl_bus[46];
    assign sram_blwl_wl[2416] = wl_bus[46];
    assign sram_blwl_wl[2417] = wl_bus[46];
    assign sram_blwl_wl[2418] = wl_bus[46];
    assign sram_blwl_wl[2419] = wl_bus[46];
    assign sram_blwl_wl[2420] = wl_bus[46];
    assign sram_blwl_wl[2421] = wl_bus[46];
    assign sram_blwl_wl[2422] = wl_bus[46];
    assign sram_blwl_wl[2423] = wl_bus[46];
    assign sram_blwl_wl[2424] = wl_bus[46];
    assign sram_blwl_wl[2425] = wl_bus[46];
    assign sram_blwl_wl[2426] = wl_bus[46];
    assign sram_blwl_wl[2427] = wl_bus[46];
    assign sram_blwl_wl[2428] = wl_bus[46];
    assign sram_blwl_wl[2429] = wl_bus[46];
    assign sram_blwl_wl[2430] = wl_bus[46];
    assign sram_blwl_wl[2431] = wl_bus[46];
    assign sram_blwl_wl[2432] = wl_bus[46];
    assign sram_blwl_wl[2433] = wl_bus[46];
    assign sram_blwl_wl[2434] = wl_bus[46];
    assign sram_blwl_wl[2435] = wl_bus[46];
    assign sram_blwl_wl[2436] = wl_bus[46];
    assign sram_blwl_wl[2437] = wl_bus[46];
    assign sram_blwl_wl[2438] = wl_bus[46];
    assign sram_blwl_wl[2439] = wl_bus[46];
    assign sram_blwl_wl[2440] = wl_bus[46];
    assign sram_blwl_wl[2441] = wl_bus[46];
    assign sram_blwl_wl[2442] = wl_bus[46];
    assign sram_blwl_wl[2443] = wl_bus[46];
    assign sram_blwl_wl[2444] = wl_bus[47];
    assign sram_blwl_wl[2445] = wl_bus[47];
    assign sram_blwl_wl[2446] = wl_bus[47];
    assign sram_blwl_wl[2447] = wl_bus[47];
    assign sram_blwl_wl[2448] = wl_bus[47];
    assign sram_blwl_wl[2449] = wl_bus[47];
    assign sram_blwl_wl[2450] = wl_bus[47];
    assign sram_blwl_wl[2451] = wl_bus[47];
    assign sram_blwl_wl[2452] = wl_bus[47];
    assign sram_blwl_wl[2453] = wl_bus[47];
    assign sram_blwl_wl[2454] = wl_bus[47];
    assign sram_blwl_wl[2455] = wl_bus[47];
    assign sram_blwl_wl[2456] = wl_bus[47];
    assign sram_blwl_wl[2457] = wl_bus[47];
    assign sram_blwl_wl[2458] = wl_bus[47];
    assign sram_blwl_wl[2459] = wl_bus[47];
    assign sram_blwl_wl[2460] = wl_bus[47];
    assign sram_blwl_wl[2461] = wl_bus[47];
    assign sram_blwl_wl[2462] = wl_bus[47];
    assign sram_blwl_wl[2463] = wl_bus[47];
    assign sram_blwl_wl[2464] = wl_bus[47];
    assign sram_blwl_wl[2465] = wl_bus[47];
    assign sram_blwl_wl[2466] = wl_bus[47];
    assign sram_blwl_wl[2467] = wl_bus[47];
    assign sram_blwl_wl[2468] = wl_bus[47];
    assign sram_blwl_wl[2469] = wl_bus[47];
    assign sram_blwl_wl[2470] = wl_bus[47];
    assign sram_blwl_wl[2471] = wl_bus[47];
    assign sram_blwl_wl[2472] = wl_bus[47];
    assign sram_blwl_wl[2473] = wl_bus[47];
    assign sram_blwl_wl[2474] = wl_bus[47];
    assign sram_blwl_wl[2475] = wl_bus[47];
    assign sram_blwl_wl[2476] = wl_bus[47];
    assign sram_blwl_wl[2477] = wl_bus[47];
    assign sram_blwl_wl[2478] = wl_bus[47];
    assign sram_blwl_wl[2479] = wl_bus[47];
    assign sram_blwl_wl[2480] = wl_bus[47];
    assign sram_blwl_wl[2481] = wl_bus[47];
    assign sram_blwl_wl[2482] = wl_bus[47];
    assign sram_blwl_wl[2483] = wl_bus[47];
    assign sram_blwl_wl[2484] = wl_bus[47];
    assign sram_blwl_wl[2485] = wl_bus[47];
    assign sram_blwl_wl[2486] = wl_bus[47];
    assign sram_blwl_wl[2487] = wl_bus[47];
    assign sram_blwl_wl[2488] = wl_bus[47];
    assign sram_blwl_wl[2489] = wl_bus[47];
    assign sram_blwl_wl[2490] = wl_bus[47];
    assign sram_blwl_wl[2491] = wl_bus[47];
    assign sram_blwl_wl[2492] = wl_bus[47];
    assign sram_blwl_wl[2493] = wl_bus[47];
    assign sram_blwl_wl[2494] = wl_bus[47];
    assign sram_blwl_wl[2495] = wl_bus[47];
    assign sram_blwl_wl[2496] = wl_bus[48];
    assign sram_blwl_wl[2497] = wl_bus[48];
    assign sram_blwl_wl[2498] = wl_bus[48];
    assign sram_blwl_wl[2499] = wl_bus[48];
    assign sram_blwl_wl[2500] = wl_bus[48];
    assign sram_blwl_wl[2501] = wl_bus[48];
    assign sram_blwl_wl[2502] = wl_bus[48];
    assign sram_blwl_wl[2503] = wl_bus[48];
    assign sram_blwl_wl[2504] = wl_bus[48];
    assign sram_blwl_wl[2505] = wl_bus[48];
    assign sram_blwl_wl[2506] = wl_bus[48];
    assign sram_blwl_wl[2507] = wl_bus[48];
    assign sram_blwl_wl[2508] = wl_bus[48];
    assign sram_blwl_wl[2509] = wl_bus[48];
    assign sram_blwl_wl[2510] = wl_bus[48];
    assign sram_blwl_wl[2511] = wl_bus[48];
    assign sram_blwl_wl[2512] = wl_bus[48];
    assign sram_blwl_wl[2513] = wl_bus[48];
    assign sram_blwl_wl[2514] = wl_bus[48];
    assign sram_blwl_wl[2515] = wl_bus[48];
    assign sram_blwl_wl[2516] = wl_bus[48];
    assign sram_blwl_wl[2517] = wl_bus[48];
    assign sram_blwl_wl[2518] = wl_bus[48];
    assign sram_blwl_wl[2519] = wl_bus[48];
    assign sram_blwl_wl[2520] = wl_bus[48];
    assign sram_blwl_wl[2521] = wl_bus[48];
    assign sram_blwl_wl[2522] = wl_bus[48];
    assign sram_blwl_wl[2523] = wl_bus[48];
    assign sram_blwl_wl[2524] = wl_bus[48];
    assign sram_blwl_wl[2525] = wl_bus[48];
    assign sram_blwl_wl[2526] = wl_bus[48];
    assign sram_blwl_wl[2527] = wl_bus[48];
    assign sram_blwl_wl[2528] = wl_bus[48];
    assign sram_blwl_wl[2529] = wl_bus[48];
    assign sram_blwl_wl[2530] = wl_bus[48];
    assign sram_blwl_wl[2531] = wl_bus[48];
    assign sram_blwl_wl[2532] = wl_bus[48];
    assign sram_blwl_wl[2533] = wl_bus[48];
    assign sram_blwl_wl[2534] = wl_bus[48];
    assign sram_blwl_wl[2535] = wl_bus[48];
    assign sram_blwl_wl[2536] = wl_bus[48];
    assign sram_blwl_wl[2537] = wl_bus[48];
    assign sram_blwl_wl[2538] = wl_bus[48];
    assign sram_blwl_wl[2539] = wl_bus[48];
    assign sram_blwl_wl[2540] = wl_bus[48];
    assign sram_blwl_wl[2541] = wl_bus[48];
    assign sram_blwl_wl[2542] = wl_bus[48];
    assign sram_blwl_wl[2543] = wl_bus[48];
    assign sram_blwl_wl[2544] = wl_bus[48];
    assign sram_blwl_wl[2545] = wl_bus[48];
    assign sram_blwl_wl[2546] = wl_bus[48];
    assign sram_blwl_wl[2547] = wl_bus[48];
    assign sram_blwl_wl[2548] = wl_bus[49];
    assign sram_blwl_wl[2549] = wl_bus[49];
    assign sram_blwl_wl[2550] = wl_bus[49];
    assign sram_blwl_wl[2551] = wl_bus[49];
    assign sram_blwl_wl[2552] = wl_bus[49];
    assign sram_blwl_wl[2553] = wl_bus[49];
    assign sram_blwl_wl[2554] = wl_bus[49];
    assign sram_blwl_wl[2555] = wl_bus[49];
    assign sram_blwl_wl[2556] = wl_bus[49];
    assign sram_blwl_wl[2557] = wl_bus[49];
    assign sram_blwl_wl[2558] = wl_bus[49];
    assign sram_blwl_wl[2559] = wl_bus[49];
    assign sram_blwl_wl[2560] = wl_bus[49];
    assign sram_blwl_wl[2561] = wl_bus[49];
    assign sram_blwl_wl[2562] = wl_bus[49];
    assign sram_blwl_wl[2563] = wl_bus[49];
    assign sram_blwl_wl[2564] = wl_bus[49];
    assign sram_blwl_wl[2565] = wl_bus[49];
    assign sram_blwl_wl[2566] = wl_bus[49];
    assign sram_blwl_wl[2567] = wl_bus[49];
    assign sram_blwl_wl[2568] = wl_bus[49];
    assign sram_blwl_wl[2569] = wl_bus[49];
    assign sram_blwl_wl[2570] = wl_bus[49];
    assign sram_blwl_wl[2571] = wl_bus[49];
    assign sram_blwl_wl[2572] = wl_bus[49];
    assign sram_blwl_wl[2573] = wl_bus[49];
    assign sram_blwl_wl[2574] = wl_bus[49];
    assign sram_blwl_wl[2575] = wl_bus[49];
    assign sram_blwl_wl[2576] = wl_bus[49];
    assign sram_blwl_wl[2577] = wl_bus[49];
    assign sram_blwl_wl[2578] = wl_bus[49];
    assign sram_blwl_wl[2579] = wl_bus[49];
    assign sram_blwl_wl[2580] = wl_bus[49];
    assign sram_blwl_wl[2581] = wl_bus[49];
    assign sram_blwl_wl[2582] = wl_bus[49];
    assign sram_blwl_wl[2583] = wl_bus[49];
    assign sram_blwl_wl[2584] = wl_bus[49];
    assign sram_blwl_wl[2585] = wl_bus[49];
    assign sram_blwl_wl[2586] = wl_bus[49];
    assign sram_blwl_wl[2587] = wl_bus[49];
    assign sram_blwl_wl[2588] = wl_bus[49];
    assign sram_blwl_wl[2589] = wl_bus[49];
    assign sram_blwl_wl[2590] = wl_bus[49];
    assign sram_blwl_wl[2591] = wl_bus[49];
    assign sram_blwl_wl[2592] = wl_bus[49];
    assign sram_blwl_wl[2593] = wl_bus[49];
    assign sram_blwl_wl[2594] = wl_bus[49];
    assign sram_blwl_wl[2595] = wl_bus[49];
    assign sram_blwl_wl[2596] = wl_bus[49];
    assign sram_blwl_wl[2597] = wl_bus[49];
    assign sram_blwl_wl[2598] = wl_bus[49];
    assign sram_blwl_wl[2599] = wl_bus[49];
    assign sram_blwl_wl[2600] = wl_bus[50];
    assign sram_blwl_wl[2601] = wl_bus[50];
    assign sram_blwl_wl[2602] = wl_bus[50];
    assign sram_blwl_wl[2603] = wl_bus[50];
    assign sram_blwl_wl[2604] = wl_bus[50];
    assign sram_blwl_wl[2605] = wl_bus[50];
    assign sram_blwl_wl[2606] = wl_bus[50];
    assign sram_blwl_wl[2607] = wl_bus[50];
    assign sram_blwl_wl[2608] = wl_bus[50];
    assign sram_blwl_wl[2609] = wl_bus[50];
    assign sram_blwl_wl[2610] = wl_bus[50];
    assign sram_blwl_wl[2611] = wl_bus[50];
    assign sram_blwl_wl[2612] = wl_bus[50];
    assign sram_blwl_wl[2613] = wl_bus[50];
    assign sram_blwl_wl[2614] = wl_bus[50];
    assign sram_blwl_wl[2615] = wl_bus[50];
    assign sram_blwl_wl[2616] = wl_bus[50];
    assign sram_blwl_wl[2617] = wl_bus[50];
    assign sram_blwl_wl[2618] = wl_bus[50];
    assign sram_blwl_wl[2619] = wl_bus[50];
    assign sram_blwl_wl[2620] = wl_bus[50];
    assign sram_blwl_wl[2621] = wl_bus[50];
    assign sram_blwl_wl[2622] = wl_bus[50];
    assign sram_blwl_wl[2623] = wl_bus[50];
    assign sram_blwl_wl[2624] = wl_bus[50];
    assign sram_blwl_wl[2625] = wl_bus[50];
    assign sram_blwl_wl[2626] = wl_bus[50];
    assign sram_blwl_wl[2627] = wl_bus[50];
    assign sram_blwl_wl[2628] = wl_bus[50];
    assign sram_blwl_wl[2629] = wl_bus[50];
    assign sram_blwl_wl[2630] = wl_bus[50];
    assign sram_blwl_wl[2631] = wl_bus[50];
    assign sram_blwl_wl[2632] = wl_bus[50];
    assign sram_blwl_wl[2633] = wl_bus[50];
    assign sram_blwl_wl[2634] = wl_bus[50];
    assign sram_blwl_wl[2635] = wl_bus[50];
    assign sram_blwl_wl[2636] = wl_bus[50];
    assign sram_blwl_wl[2637] = wl_bus[50];
    assign sram_blwl_wl[2638] = wl_bus[50];
    assign sram_blwl_wl[2639] = wl_bus[50];
    assign sram_blwl_wl[2640] = wl_bus[50];
    assign sram_blwl_wl[2641] = wl_bus[50];
    assign sram_blwl_wl[2642] = wl_bus[50];
    assign sram_blwl_wl[2643] = wl_bus[50];
    assign sram_blwl_wl[2644] = wl_bus[50];
    assign sram_blwl_wl[2645] = wl_bus[50];
    assign sram_blwl_wl[2646] = wl_bus[50];
    assign sram_blwl_wl[2647] = wl_bus[50];
    assign sram_blwl_wl[2648] = wl_bus[50];
    assign sram_blwl_wl[2649] = wl_bus[50];
    assign sram_blwl_wl[2650] = wl_bus[50];
    assign sram_blwl_wl[2651] = wl_bus[50];
    assign sram_blwl_wl[2652] = wl_bus[51];
    assign sram_blwl_wl[2653] = wl_bus[51];
    assign sram_blwl_wl[2654] = wl_bus[51];
    assign sram_blwl_wl[2655] = wl_bus[51];
    assign sram_blwl_wl[2656] = wl_bus[51];
    assign sram_blwl_wl[2657] = wl_bus[51];
//----- BEGIN Call Grid[1][1] module -----
grid_1__1_  grid_1__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 grid_1__1__pin_0__0__0_,
 grid_1__1__pin_0__0__4_,
 grid_1__1__pin_0__0__8_,
 grid_1__1__pin_0__0__12_,
 grid_1__1__pin_0__0__16_,
 grid_1__1__pin_0__0__20_,
 grid_1__1__pin_0__0__24_,
 grid_1__1__pin_0__0__28_,
 grid_1__1__pin_0__0__32_,
 grid_1__1__pin_0__0__36_,
 grid_1__1__pin_0__0__40_,
 grid_1__1__pin_0__0__44_,
 grid_1__1__pin_0__0__48_,
 grid_1__1__pin_0__1__1_,
 grid_1__1__pin_0__1__5_,
 grid_1__1__pin_0__1__9_,
 grid_1__1__pin_0__1__13_,
 grid_1__1__pin_0__1__17_,
 grid_1__1__pin_0__1__21_,
 grid_1__1__pin_0__1__25_,
 grid_1__1__pin_0__1__29_,
 grid_1__1__pin_0__1__33_,
 grid_1__1__pin_0__1__37_,
 grid_1__1__pin_0__1__41_,
 grid_1__1__pin_0__1__45_,
 grid_1__1__pin_0__1__49_,
 grid_1__1__pin_0__2__2_,
 grid_1__1__pin_0__2__6_,
 grid_1__1__pin_0__2__10_,
 grid_1__1__pin_0__2__14_,
 grid_1__1__pin_0__2__18_,
 grid_1__1__pin_0__2__22_,
 grid_1__1__pin_0__2__26_,
 grid_1__1__pin_0__2__30_,
 grid_1__1__pin_0__2__34_,
 grid_1__1__pin_0__2__38_,
 grid_1__1__pin_0__2__42_,
 grid_1__1__pin_0__2__46_,
 grid_1__1__pin_0__2__50_,
 grid_1__1__pin_0__3__3_,
 grid_1__1__pin_0__3__7_,
 grid_1__1__pin_0__3__11_,
 grid_1__1__pin_0__3__15_,
 grid_1__1__pin_0__3__19_,
 grid_1__1__pin_0__3__23_,
 grid_1__1__pin_0__3__27_,
 grid_1__1__pin_0__3__31_,
 grid_1__1__pin_0__3__35_,
 grid_1__1__pin_0__3__39_,
 grid_1__1__pin_0__3__43_,
 grid_1__1__pin_0__3__47_,
sram_blwl_bl[1016:2625] ,
sram_blwl_wl[1016:2625] ,
sram_blwl_blb[1016:2625] );
//----- END call Grid[1][1] module -----

//----- BEGIN Call Grid[0][1] module -----
grid_0__1_  grid_0__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 grid_0__1__pin_0__1__0_,
 grid_0__1__pin_0__1__1_,
 grid_0__1__pin_0__1__2_,
 grid_0__1__pin_0__1__3_,
 grid_0__1__pin_0__1__4_,
 grid_0__1__pin_0__1__5_,
 grid_0__1__pin_0__1__6_,
 grid_0__1__pin_0__1__7_,
 grid_0__1__pin_0__1__8_,
 grid_0__1__pin_0__1__9_,
 grid_0__1__pin_0__1__10_,
 grid_0__1__pin_0__1__11_,
 grid_0__1__pin_0__1__12_,
 grid_0__1__pin_0__1__13_,
 grid_0__1__pin_0__1__14_,
 grid_0__1__pin_0__1__15_,
gfpga_pad_iopad[7:0] ,
sram_blwl_bl[2626:2633] ,
sram_blwl_wl[2626:2633] ,
sram_blwl_blb[2626:2633] );
//----- END call Grid[0][1] module -----

//----- BEGIN Call Grid[2][1] module -----
grid_2__1_  grid_2__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 grid_2__1__pin_0__3__0_,
 grid_2__1__pin_0__3__1_,
 grid_2__1__pin_0__3__2_,
 grid_2__1__pin_0__3__3_,
 grid_2__1__pin_0__3__4_,
 grid_2__1__pin_0__3__5_,
 grid_2__1__pin_0__3__6_,
 grid_2__1__pin_0__3__7_,
 grid_2__1__pin_0__3__8_,
 grid_2__1__pin_0__3__9_,
 grid_2__1__pin_0__3__10_,
 grid_2__1__pin_0__3__11_,
 grid_2__1__pin_0__3__12_,
 grid_2__1__pin_0__3__13_,
 grid_2__1__pin_0__3__14_,
 grid_2__1__pin_0__3__15_,
gfpga_pad_iopad[15:8] ,
sram_blwl_bl[2634:2641] ,
sram_blwl_wl[2634:2641] ,
sram_blwl_blb[2634:2641] );
//----- END call Grid[2][1] module -----

//----- BEGIN Call Grid[1][0] module -----
grid_1__0_  grid_1__0__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 grid_1__0__pin_0__0__0_,
 grid_1__0__pin_0__0__1_,
 grid_1__0__pin_0__0__2_,
 grid_1__0__pin_0__0__3_,
 grid_1__0__pin_0__0__4_,
 grid_1__0__pin_0__0__5_,
 grid_1__0__pin_0__0__6_,
 grid_1__0__pin_0__0__7_,
 grid_1__0__pin_0__0__8_,
 grid_1__0__pin_0__0__9_,
 grid_1__0__pin_0__0__10_,
 grid_1__0__pin_0__0__11_,
 grid_1__0__pin_0__0__12_,
 grid_1__0__pin_0__0__13_,
 grid_1__0__pin_0__0__14_,
 grid_1__0__pin_0__0__15_,
gfpga_pad_iopad[23:16] ,
sram_blwl_bl[2642:2649] ,
sram_blwl_wl[2642:2649] ,
sram_blwl_blb[2642:2649] );
//----- END call Grid[1][0] module -----

//----- BEGIN Call Grid[1][2] module -----
grid_1__2_  grid_1__2__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
 grid_1__2__pin_0__2__0_,
 grid_1__2__pin_0__2__1_,
 grid_1__2__pin_0__2__2_,
 grid_1__2__pin_0__2__3_,
 grid_1__2__pin_0__2__4_,
 grid_1__2__pin_0__2__5_,
 grid_1__2__pin_0__2__6_,
 grid_1__2__pin_0__2__7_,
 grid_1__2__pin_0__2__8_,
 grid_1__2__pin_0__2__9_,
 grid_1__2__pin_0__2__10_,
 grid_1__2__pin_0__2__11_,
 grid_1__2__pin_0__2__12_,
 grid_1__2__pin_0__2__13_,
 grid_1__2__pin_0__2__14_,
 grid_1__2__pin_0__2__15_,
gfpga_pad_iopad[31:24] ,
sram_blwl_bl[2650:2657] ,
sram_blwl_wl[2650:2657] ,
sram_blwl_blb[2650:2657] );
//----- END call Grid[1][2] module -----

//----- BEGIN Call Channel-X [1][0] module -----
chanx_1__0_ chanx_1__0__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
chanx_1__0__out_0_, 
chanx_1__0__in_1_, 
chanx_1__0__out_2_, 
chanx_1__0__in_3_, 
chanx_1__0__out_4_, 
chanx_1__0__in_5_, 
chanx_1__0__out_6_, 
chanx_1__0__in_7_, 
chanx_1__0__out_8_, 
chanx_1__0__in_9_, 
chanx_1__0__out_10_, 
chanx_1__0__in_11_, 
chanx_1__0__out_12_, 
chanx_1__0__in_13_, 
chanx_1__0__out_14_, 
chanx_1__0__in_15_, 
chanx_1__0__out_16_, 
chanx_1__0__in_17_, 
chanx_1__0__out_18_, 
chanx_1__0__in_19_, 
chanx_1__0__out_20_, 
chanx_1__0__in_21_, 
chanx_1__0__out_22_, 
chanx_1__0__in_23_, 
chanx_1__0__out_24_, 
chanx_1__0__in_25_, 
chanx_1__0__out_26_, 
chanx_1__0__in_27_, 
chanx_1__0__out_28_, 
chanx_1__0__in_29_, 
chanx_1__0__out_30_, 
chanx_1__0__in_31_, 
chanx_1__0__out_32_, 
chanx_1__0__in_33_, 
chanx_1__0__out_34_, 
chanx_1__0__in_35_, 
chanx_1__0__out_36_, 
chanx_1__0__in_37_, 
chanx_1__0__out_38_, 
chanx_1__0__in_39_, 
chanx_1__0__out_40_, 
chanx_1__0__in_41_, 
chanx_1__0__out_42_, 
chanx_1__0__in_43_, 
chanx_1__0__out_44_, 
chanx_1__0__in_45_, 
chanx_1__0__out_46_, 
chanx_1__0__in_47_, 
chanx_1__0__out_48_, 
chanx_1__0__in_49_, 
chanx_1__0__out_50_, 
chanx_1__0__in_51_, 
chanx_1__0__out_52_, 
chanx_1__0__in_53_, 
chanx_1__0__out_54_, 
chanx_1__0__in_55_, 
chanx_1__0__out_56_, 
chanx_1__0__in_57_, 
chanx_1__0__out_58_, 
chanx_1__0__in_59_, 
chanx_1__0__out_60_, 
chanx_1__0__in_61_, 
chanx_1__0__out_62_, 
chanx_1__0__in_63_, 
chanx_1__0__out_64_, 
chanx_1__0__in_65_, 
chanx_1__0__out_66_, 
chanx_1__0__in_67_, 
chanx_1__0__out_68_, 
chanx_1__0__in_69_, 
chanx_1__0__out_70_, 
chanx_1__0__in_71_, 
chanx_1__0__out_72_, 
chanx_1__0__in_73_, 
chanx_1__0__out_74_, 
chanx_1__0__in_75_, 
chanx_1__0__out_76_, 
chanx_1__0__in_77_, 
chanx_1__0__out_78_, 
chanx_1__0__in_79_, 
chanx_1__0__out_80_, 
chanx_1__0__in_81_, 
chanx_1__0__out_82_, 
chanx_1__0__in_83_, 
chanx_1__0__out_84_, 
chanx_1__0__in_85_, 
chanx_1__0__out_86_, 
chanx_1__0__in_87_, 
chanx_1__0__out_88_, 
chanx_1__0__in_89_, 
chanx_1__0__out_90_, 
chanx_1__0__in_91_, 
chanx_1__0__out_92_, 
chanx_1__0__in_93_, 
chanx_1__0__out_94_, 
chanx_1__0__in_95_, 
chanx_1__0__out_96_, 
chanx_1__0__in_97_, 
chanx_1__0__out_98_, 
chanx_1__0__in_99_, 
chanx_1__0__in_0_, 
chanx_1__0__out_1_, 
chanx_1__0__in_2_, 
chanx_1__0__out_3_, 
chanx_1__0__in_4_, 
chanx_1__0__out_5_, 
chanx_1__0__in_6_, 
chanx_1__0__out_7_, 
chanx_1__0__in_8_, 
chanx_1__0__out_9_, 
chanx_1__0__in_10_, 
chanx_1__0__out_11_, 
chanx_1__0__in_12_, 
chanx_1__0__out_13_, 
chanx_1__0__in_14_, 
chanx_1__0__out_15_, 
chanx_1__0__in_16_, 
chanx_1__0__out_17_, 
chanx_1__0__in_18_, 
chanx_1__0__out_19_, 
chanx_1__0__in_20_, 
chanx_1__0__out_21_, 
chanx_1__0__in_22_, 
chanx_1__0__out_23_, 
chanx_1__0__in_24_, 
chanx_1__0__out_25_, 
chanx_1__0__in_26_, 
chanx_1__0__out_27_, 
chanx_1__0__in_28_, 
chanx_1__0__out_29_, 
chanx_1__0__in_30_, 
chanx_1__0__out_31_, 
chanx_1__0__in_32_, 
chanx_1__0__out_33_, 
chanx_1__0__in_34_, 
chanx_1__0__out_35_, 
chanx_1__0__in_36_, 
chanx_1__0__out_37_, 
chanx_1__0__in_38_, 
chanx_1__0__out_39_, 
chanx_1__0__in_40_, 
chanx_1__0__out_41_, 
chanx_1__0__in_42_, 
chanx_1__0__out_43_, 
chanx_1__0__in_44_, 
chanx_1__0__out_45_, 
chanx_1__0__in_46_, 
chanx_1__0__out_47_, 
chanx_1__0__in_48_, 
chanx_1__0__out_49_, 
chanx_1__0__in_50_, 
chanx_1__0__out_51_, 
chanx_1__0__in_52_, 
chanx_1__0__out_53_, 
chanx_1__0__in_54_, 
chanx_1__0__out_55_, 
chanx_1__0__in_56_, 
chanx_1__0__out_57_, 
chanx_1__0__in_58_, 
chanx_1__0__out_59_, 
chanx_1__0__in_60_, 
chanx_1__0__out_61_, 
chanx_1__0__in_62_, 
chanx_1__0__out_63_, 
chanx_1__0__in_64_, 
chanx_1__0__out_65_, 
chanx_1__0__in_66_, 
chanx_1__0__out_67_, 
chanx_1__0__in_68_, 
chanx_1__0__out_69_, 
chanx_1__0__in_70_, 
chanx_1__0__out_71_, 
chanx_1__0__in_72_, 
chanx_1__0__out_73_, 
chanx_1__0__in_74_, 
chanx_1__0__out_75_, 
chanx_1__0__in_76_, 
chanx_1__0__out_77_, 
chanx_1__0__in_78_, 
chanx_1__0__out_79_, 
chanx_1__0__in_80_, 
chanx_1__0__out_81_, 
chanx_1__0__in_82_, 
chanx_1__0__out_83_, 
chanx_1__0__in_84_, 
chanx_1__0__out_85_, 
chanx_1__0__in_86_, 
chanx_1__0__out_87_, 
chanx_1__0__in_88_, 
chanx_1__0__out_89_, 
chanx_1__0__in_90_, 
chanx_1__0__out_91_, 
chanx_1__0__in_92_, 
chanx_1__0__out_93_, 
chanx_1__0__in_94_, 
chanx_1__0__out_95_, 
chanx_1__0__in_96_, 
chanx_1__0__out_97_, 
chanx_1__0__in_98_, 
chanx_1__0__out_99_, 
chanx_1__0__midout_0_ ,
chanx_1__0__midout_1_ ,
chanx_1__0__midout_2_ ,
chanx_1__0__midout_3_ ,
chanx_1__0__midout_4_ ,
chanx_1__0__midout_5_ ,
chanx_1__0__midout_6_ ,
chanx_1__0__midout_7_ ,
chanx_1__0__midout_8_ ,
chanx_1__0__midout_9_ ,
chanx_1__0__midout_10_ ,
chanx_1__0__midout_11_ ,
chanx_1__0__midout_12_ ,
chanx_1__0__midout_13_ ,
chanx_1__0__midout_14_ ,
chanx_1__0__midout_15_ ,
chanx_1__0__midout_16_ ,
chanx_1__0__midout_17_ ,
chanx_1__0__midout_18_ ,
chanx_1__0__midout_19_ ,
chanx_1__0__midout_20_ ,
chanx_1__0__midout_21_ ,
chanx_1__0__midout_22_ ,
chanx_1__0__midout_23_ ,
chanx_1__0__midout_24_ ,
chanx_1__0__midout_25_ ,
chanx_1__0__midout_26_ ,
chanx_1__0__midout_27_ ,
chanx_1__0__midout_28_ ,
chanx_1__0__midout_29_ ,
chanx_1__0__midout_30_ ,
chanx_1__0__midout_31_ ,
chanx_1__0__midout_32_ ,
chanx_1__0__midout_33_ ,
chanx_1__0__midout_34_ ,
chanx_1__0__midout_35_ ,
chanx_1__0__midout_36_ ,
chanx_1__0__midout_37_ ,
chanx_1__0__midout_38_ ,
chanx_1__0__midout_39_ ,
chanx_1__0__midout_40_ ,
chanx_1__0__midout_41_ ,
chanx_1__0__midout_42_ ,
chanx_1__0__midout_43_ ,
chanx_1__0__midout_44_ ,
chanx_1__0__midout_45_ ,
chanx_1__0__midout_46_ ,
chanx_1__0__midout_47_ ,
chanx_1__0__midout_48_ ,
chanx_1__0__midout_49_ ,
chanx_1__0__midout_50_ ,
chanx_1__0__midout_51_ ,
chanx_1__0__midout_52_ ,
chanx_1__0__midout_53_ ,
chanx_1__0__midout_54_ ,
chanx_1__0__midout_55_ ,
chanx_1__0__midout_56_ ,
chanx_1__0__midout_57_ ,
chanx_1__0__midout_58_ ,
chanx_1__0__midout_59_ ,
chanx_1__0__midout_60_ ,
chanx_1__0__midout_61_ ,
chanx_1__0__midout_62_ ,
chanx_1__0__midout_63_ ,
chanx_1__0__midout_64_ ,
chanx_1__0__midout_65_ ,
chanx_1__0__midout_66_ ,
chanx_1__0__midout_67_ ,
chanx_1__0__midout_68_ ,
chanx_1__0__midout_69_ ,
chanx_1__0__midout_70_ ,
chanx_1__0__midout_71_ ,
chanx_1__0__midout_72_ ,
chanx_1__0__midout_73_ ,
chanx_1__0__midout_74_ ,
chanx_1__0__midout_75_ ,
chanx_1__0__midout_76_ ,
chanx_1__0__midout_77_ ,
chanx_1__0__midout_78_ ,
chanx_1__0__midout_79_ ,
chanx_1__0__midout_80_ ,
chanx_1__0__midout_81_ ,
chanx_1__0__midout_82_ ,
chanx_1__0__midout_83_ ,
chanx_1__0__midout_84_ ,
chanx_1__0__midout_85_ ,
chanx_1__0__midout_86_ ,
chanx_1__0__midout_87_ ,
chanx_1__0__midout_88_ ,
chanx_1__0__midout_89_ ,
chanx_1__0__midout_90_ ,
chanx_1__0__midout_91_ ,
chanx_1__0__midout_92_ ,
chanx_1__0__midout_93_ ,
chanx_1__0__midout_94_ ,
chanx_1__0__midout_95_ ,
chanx_1__0__midout_96_ ,
chanx_1__0__midout_97_ ,
chanx_1__0__midout_98_ ,
chanx_1__0__midout_99_ 
);
//----- END Call Channel-X [1][0] module -----
//----- BEGIN Call Channel-X [1][1] module -----
chanx_1__1_ chanx_1__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
chanx_1__1__out_0_, 
chanx_1__1__in_1_, 
chanx_1__1__out_2_, 
chanx_1__1__in_3_, 
chanx_1__1__out_4_, 
chanx_1__1__in_5_, 
chanx_1__1__out_6_, 
chanx_1__1__in_7_, 
chanx_1__1__out_8_, 
chanx_1__1__in_9_, 
chanx_1__1__out_10_, 
chanx_1__1__in_11_, 
chanx_1__1__out_12_, 
chanx_1__1__in_13_, 
chanx_1__1__out_14_, 
chanx_1__1__in_15_, 
chanx_1__1__out_16_, 
chanx_1__1__in_17_, 
chanx_1__1__out_18_, 
chanx_1__1__in_19_, 
chanx_1__1__out_20_, 
chanx_1__1__in_21_, 
chanx_1__1__out_22_, 
chanx_1__1__in_23_, 
chanx_1__1__out_24_, 
chanx_1__1__in_25_, 
chanx_1__1__out_26_, 
chanx_1__1__in_27_, 
chanx_1__1__out_28_, 
chanx_1__1__in_29_, 
chanx_1__1__out_30_, 
chanx_1__1__in_31_, 
chanx_1__1__out_32_, 
chanx_1__1__in_33_, 
chanx_1__1__out_34_, 
chanx_1__1__in_35_, 
chanx_1__1__out_36_, 
chanx_1__1__in_37_, 
chanx_1__1__out_38_, 
chanx_1__1__in_39_, 
chanx_1__1__out_40_, 
chanx_1__1__in_41_, 
chanx_1__1__out_42_, 
chanx_1__1__in_43_, 
chanx_1__1__out_44_, 
chanx_1__1__in_45_, 
chanx_1__1__out_46_, 
chanx_1__1__in_47_, 
chanx_1__1__out_48_, 
chanx_1__1__in_49_, 
chanx_1__1__out_50_, 
chanx_1__1__in_51_, 
chanx_1__1__out_52_, 
chanx_1__1__in_53_, 
chanx_1__1__out_54_, 
chanx_1__1__in_55_, 
chanx_1__1__out_56_, 
chanx_1__1__in_57_, 
chanx_1__1__out_58_, 
chanx_1__1__in_59_, 
chanx_1__1__out_60_, 
chanx_1__1__in_61_, 
chanx_1__1__out_62_, 
chanx_1__1__in_63_, 
chanx_1__1__out_64_, 
chanx_1__1__in_65_, 
chanx_1__1__out_66_, 
chanx_1__1__in_67_, 
chanx_1__1__out_68_, 
chanx_1__1__in_69_, 
chanx_1__1__out_70_, 
chanx_1__1__in_71_, 
chanx_1__1__out_72_, 
chanx_1__1__in_73_, 
chanx_1__1__out_74_, 
chanx_1__1__in_75_, 
chanx_1__1__out_76_, 
chanx_1__1__in_77_, 
chanx_1__1__out_78_, 
chanx_1__1__in_79_, 
chanx_1__1__out_80_, 
chanx_1__1__in_81_, 
chanx_1__1__out_82_, 
chanx_1__1__in_83_, 
chanx_1__1__out_84_, 
chanx_1__1__in_85_, 
chanx_1__1__out_86_, 
chanx_1__1__in_87_, 
chanx_1__1__out_88_, 
chanx_1__1__in_89_, 
chanx_1__1__out_90_, 
chanx_1__1__in_91_, 
chanx_1__1__out_92_, 
chanx_1__1__in_93_, 
chanx_1__1__out_94_, 
chanx_1__1__in_95_, 
chanx_1__1__out_96_, 
chanx_1__1__in_97_, 
chanx_1__1__out_98_, 
chanx_1__1__in_99_, 
chanx_1__1__in_0_, 
chanx_1__1__out_1_, 
chanx_1__1__in_2_, 
chanx_1__1__out_3_, 
chanx_1__1__in_4_, 
chanx_1__1__out_5_, 
chanx_1__1__in_6_, 
chanx_1__1__out_7_, 
chanx_1__1__in_8_, 
chanx_1__1__out_9_, 
chanx_1__1__in_10_, 
chanx_1__1__out_11_, 
chanx_1__1__in_12_, 
chanx_1__1__out_13_, 
chanx_1__1__in_14_, 
chanx_1__1__out_15_, 
chanx_1__1__in_16_, 
chanx_1__1__out_17_, 
chanx_1__1__in_18_, 
chanx_1__1__out_19_, 
chanx_1__1__in_20_, 
chanx_1__1__out_21_, 
chanx_1__1__in_22_, 
chanx_1__1__out_23_, 
chanx_1__1__in_24_, 
chanx_1__1__out_25_, 
chanx_1__1__in_26_, 
chanx_1__1__out_27_, 
chanx_1__1__in_28_, 
chanx_1__1__out_29_, 
chanx_1__1__in_30_, 
chanx_1__1__out_31_, 
chanx_1__1__in_32_, 
chanx_1__1__out_33_, 
chanx_1__1__in_34_, 
chanx_1__1__out_35_, 
chanx_1__1__in_36_, 
chanx_1__1__out_37_, 
chanx_1__1__in_38_, 
chanx_1__1__out_39_, 
chanx_1__1__in_40_, 
chanx_1__1__out_41_, 
chanx_1__1__in_42_, 
chanx_1__1__out_43_, 
chanx_1__1__in_44_, 
chanx_1__1__out_45_, 
chanx_1__1__in_46_, 
chanx_1__1__out_47_, 
chanx_1__1__in_48_, 
chanx_1__1__out_49_, 
chanx_1__1__in_50_, 
chanx_1__1__out_51_, 
chanx_1__1__in_52_, 
chanx_1__1__out_53_, 
chanx_1__1__in_54_, 
chanx_1__1__out_55_, 
chanx_1__1__in_56_, 
chanx_1__1__out_57_, 
chanx_1__1__in_58_, 
chanx_1__1__out_59_, 
chanx_1__1__in_60_, 
chanx_1__1__out_61_, 
chanx_1__1__in_62_, 
chanx_1__1__out_63_, 
chanx_1__1__in_64_, 
chanx_1__1__out_65_, 
chanx_1__1__in_66_, 
chanx_1__1__out_67_, 
chanx_1__1__in_68_, 
chanx_1__1__out_69_, 
chanx_1__1__in_70_, 
chanx_1__1__out_71_, 
chanx_1__1__in_72_, 
chanx_1__1__out_73_, 
chanx_1__1__in_74_, 
chanx_1__1__out_75_, 
chanx_1__1__in_76_, 
chanx_1__1__out_77_, 
chanx_1__1__in_78_, 
chanx_1__1__out_79_, 
chanx_1__1__in_80_, 
chanx_1__1__out_81_, 
chanx_1__1__in_82_, 
chanx_1__1__out_83_, 
chanx_1__1__in_84_, 
chanx_1__1__out_85_, 
chanx_1__1__in_86_, 
chanx_1__1__out_87_, 
chanx_1__1__in_88_, 
chanx_1__1__out_89_, 
chanx_1__1__in_90_, 
chanx_1__1__out_91_, 
chanx_1__1__in_92_, 
chanx_1__1__out_93_, 
chanx_1__1__in_94_, 
chanx_1__1__out_95_, 
chanx_1__1__in_96_, 
chanx_1__1__out_97_, 
chanx_1__1__in_98_, 
chanx_1__1__out_99_, 
chanx_1__1__midout_0_ ,
chanx_1__1__midout_1_ ,
chanx_1__1__midout_2_ ,
chanx_1__1__midout_3_ ,
chanx_1__1__midout_4_ ,
chanx_1__1__midout_5_ ,
chanx_1__1__midout_6_ ,
chanx_1__1__midout_7_ ,
chanx_1__1__midout_8_ ,
chanx_1__1__midout_9_ ,
chanx_1__1__midout_10_ ,
chanx_1__1__midout_11_ ,
chanx_1__1__midout_12_ ,
chanx_1__1__midout_13_ ,
chanx_1__1__midout_14_ ,
chanx_1__1__midout_15_ ,
chanx_1__1__midout_16_ ,
chanx_1__1__midout_17_ ,
chanx_1__1__midout_18_ ,
chanx_1__1__midout_19_ ,
chanx_1__1__midout_20_ ,
chanx_1__1__midout_21_ ,
chanx_1__1__midout_22_ ,
chanx_1__1__midout_23_ ,
chanx_1__1__midout_24_ ,
chanx_1__1__midout_25_ ,
chanx_1__1__midout_26_ ,
chanx_1__1__midout_27_ ,
chanx_1__1__midout_28_ ,
chanx_1__1__midout_29_ ,
chanx_1__1__midout_30_ ,
chanx_1__1__midout_31_ ,
chanx_1__1__midout_32_ ,
chanx_1__1__midout_33_ ,
chanx_1__1__midout_34_ ,
chanx_1__1__midout_35_ ,
chanx_1__1__midout_36_ ,
chanx_1__1__midout_37_ ,
chanx_1__1__midout_38_ ,
chanx_1__1__midout_39_ ,
chanx_1__1__midout_40_ ,
chanx_1__1__midout_41_ ,
chanx_1__1__midout_42_ ,
chanx_1__1__midout_43_ ,
chanx_1__1__midout_44_ ,
chanx_1__1__midout_45_ ,
chanx_1__1__midout_46_ ,
chanx_1__1__midout_47_ ,
chanx_1__1__midout_48_ ,
chanx_1__1__midout_49_ ,
chanx_1__1__midout_50_ ,
chanx_1__1__midout_51_ ,
chanx_1__1__midout_52_ ,
chanx_1__1__midout_53_ ,
chanx_1__1__midout_54_ ,
chanx_1__1__midout_55_ ,
chanx_1__1__midout_56_ ,
chanx_1__1__midout_57_ ,
chanx_1__1__midout_58_ ,
chanx_1__1__midout_59_ ,
chanx_1__1__midout_60_ ,
chanx_1__1__midout_61_ ,
chanx_1__1__midout_62_ ,
chanx_1__1__midout_63_ ,
chanx_1__1__midout_64_ ,
chanx_1__1__midout_65_ ,
chanx_1__1__midout_66_ ,
chanx_1__1__midout_67_ ,
chanx_1__1__midout_68_ ,
chanx_1__1__midout_69_ ,
chanx_1__1__midout_70_ ,
chanx_1__1__midout_71_ ,
chanx_1__1__midout_72_ ,
chanx_1__1__midout_73_ ,
chanx_1__1__midout_74_ ,
chanx_1__1__midout_75_ ,
chanx_1__1__midout_76_ ,
chanx_1__1__midout_77_ ,
chanx_1__1__midout_78_ ,
chanx_1__1__midout_79_ ,
chanx_1__1__midout_80_ ,
chanx_1__1__midout_81_ ,
chanx_1__1__midout_82_ ,
chanx_1__1__midout_83_ ,
chanx_1__1__midout_84_ ,
chanx_1__1__midout_85_ ,
chanx_1__1__midout_86_ ,
chanx_1__1__midout_87_ ,
chanx_1__1__midout_88_ ,
chanx_1__1__midout_89_ ,
chanx_1__1__midout_90_ ,
chanx_1__1__midout_91_ ,
chanx_1__1__midout_92_ ,
chanx_1__1__midout_93_ ,
chanx_1__1__midout_94_ ,
chanx_1__1__midout_95_ ,
chanx_1__1__midout_96_ ,
chanx_1__1__midout_97_ ,
chanx_1__1__midout_98_ ,
chanx_1__1__midout_99_ 
);
//----- END Call Channel-X [1][1] module -----
//----- BEGIN call Channel-Y [0][1] module -----

chany_0__1_ chany_0__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
chany_0__1__out_0_, 
chany_0__1__in_1_, 
chany_0__1__out_2_, 
chany_0__1__in_3_, 
chany_0__1__out_4_, 
chany_0__1__in_5_, 
chany_0__1__out_6_, 
chany_0__1__in_7_, 
chany_0__1__out_8_, 
chany_0__1__in_9_, 
chany_0__1__out_10_, 
chany_0__1__in_11_, 
chany_0__1__out_12_, 
chany_0__1__in_13_, 
chany_0__1__out_14_, 
chany_0__1__in_15_, 
chany_0__1__out_16_, 
chany_0__1__in_17_, 
chany_0__1__out_18_, 
chany_0__1__in_19_, 
chany_0__1__out_20_, 
chany_0__1__in_21_, 
chany_0__1__out_22_, 
chany_0__1__in_23_, 
chany_0__1__out_24_, 
chany_0__1__in_25_, 
chany_0__1__out_26_, 
chany_0__1__in_27_, 
chany_0__1__out_28_, 
chany_0__1__in_29_, 
chany_0__1__out_30_, 
chany_0__1__in_31_, 
chany_0__1__out_32_, 
chany_0__1__in_33_, 
chany_0__1__out_34_, 
chany_0__1__in_35_, 
chany_0__1__out_36_, 
chany_0__1__in_37_, 
chany_0__1__out_38_, 
chany_0__1__in_39_, 
chany_0__1__out_40_, 
chany_0__1__in_41_, 
chany_0__1__out_42_, 
chany_0__1__in_43_, 
chany_0__1__out_44_, 
chany_0__1__in_45_, 
chany_0__1__out_46_, 
chany_0__1__in_47_, 
chany_0__1__out_48_, 
chany_0__1__in_49_, 
chany_0__1__out_50_, 
chany_0__1__in_51_, 
chany_0__1__out_52_, 
chany_0__1__in_53_, 
chany_0__1__out_54_, 
chany_0__1__in_55_, 
chany_0__1__out_56_, 
chany_0__1__in_57_, 
chany_0__1__out_58_, 
chany_0__1__in_59_, 
chany_0__1__out_60_, 
chany_0__1__in_61_, 
chany_0__1__out_62_, 
chany_0__1__in_63_, 
chany_0__1__out_64_, 
chany_0__1__in_65_, 
chany_0__1__out_66_, 
chany_0__1__in_67_, 
chany_0__1__out_68_, 
chany_0__1__in_69_, 
chany_0__1__out_70_, 
chany_0__1__in_71_, 
chany_0__1__out_72_, 
chany_0__1__in_73_, 
chany_0__1__out_74_, 
chany_0__1__in_75_, 
chany_0__1__out_76_, 
chany_0__1__in_77_, 
chany_0__1__out_78_, 
chany_0__1__in_79_, 
chany_0__1__out_80_, 
chany_0__1__in_81_, 
chany_0__1__out_82_, 
chany_0__1__in_83_, 
chany_0__1__out_84_, 
chany_0__1__in_85_, 
chany_0__1__out_86_, 
chany_0__1__in_87_, 
chany_0__1__out_88_, 
chany_0__1__in_89_, 
chany_0__1__out_90_, 
chany_0__1__in_91_, 
chany_0__1__out_92_, 
chany_0__1__in_93_, 
chany_0__1__out_94_, 
chany_0__1__in_95_, 
chany_0__1__out_96_, 
chany_0__1__in_97_, 
chany_0__1__out_98_, 
chany_0__1__in_99_, 
chany_0__1__in_0_, 
chany_0__1__out_1_, 
chany_0__1__in_2_, 
chany_0__1__out_3_, 
chany_0__1__in_4_, 
chany_0__1__out_5_, 
chany_0__1__in_6_, 
chany_0__1__out_7_, 
chany_0__1__in_8_, 
chany_0__1__out_9_, 
chany_0__1__in_10_, 
chany_0__1__out_11_, 
chany_0__1__in_12_, 
chany_0__1__out_13_, 
chany_0__1__in_14_, 
chany_0__1__out_15_, 
chany_0__1__in_16_, 
chany_0__1__out_17_, 
chany_0__1__in_18_, 
chany_0__1__out_19_, 
chany_0__1__in_20_, 
chany_0__1__out_21_, 
chany_0__1__in_22_, 
chany_0__1__out_23_, 
chany_0__1__in_24_, 
chany_0__1__out_25_, 
chany_0__1__in_26_, 
chany_0__1__out_27_, 
chany_0__1__in_28_, 
chany_0__1__out_29_, 
chany_0__1__in_30_, 
chany_0__1__out_31_, 
chany_0__1__in_32_, 
chany_0__1__out_33_, 
chany_0__1__in_34_, 
chany_0__1__out_35_, 
chany_0__1__in_36_, 
chany_0__1__out_37_, 
chany_0__1__in_38_, 
chany_0__1__out_39_, 
chany_0__1__in_40_, 
chany_0__1__out_41_, 
chany_0__1__in_42_, 
chany_0__1__out_43_, 
chany_0__1__in_44_, 
chany_0__1__out_45_, 
chany_0__1__in_46_, 
chany_0__1__out_47_, 
chany_0__1__in_48_, 
chany_0__1__out_49_, 
chany_0__1__in_50_, 
chany_0__1__out_51_, 
chany_0__1__in_52_, 
chany_0__1__out_53_, 
chany_0__1__in_54_, 
chany_0__1__out_55_, 
chany_0__1__in_56_, 
chany_0__1__out_57_, 
chany_0__1__in_58_, 
chany_0__1__out_59_, 
chany_0__1__in_60_, 
chany_0__1__out_61_, 
chany_0__1__in_62_, 
chany_0__1__out_63_, 
chany_0__1__in_64_, 
chany_0__1__out_65_, 
chany_0__1__in_66_, 
chany_0__1__out_67_, 
chany_0__1__in_68_, 
chany_0__1__out_69_, 
chany_0__1__in_70_, 
chany_0__1__out_71_, 
chany_0__1__in_72_, 
chany_0__1__out_73_, 
chany_0__1__in_74_, 
chany_0__1__out_75_, 
chany_0__1__in_76_, 
chany_0__1__out_77_, 
chany_0__1__in_78_, 
chany_0__1__out_79_, 
chany_0__1__in_80_, 
chany_0__1__out_81_, 
chany_0__1__in_82_, 
chany_0__1__out_83_, 
chany_0__1__in_84_, 
chany_0__1__out_85_, 
chany_0__1__in_86_, 
chany_0__1__out_87_, 
chany_0__1__in_88_, 
chany_0__1__out_89_, 
chany_0__1__in_90_, 
chany_0__1__out_91_, 
chany_0__1__in_92_, 
chany_0__1__out_93_, 
chany_0__1__in_94_, 
chany_0__1__out_95_, 
chany_0__1__in_96_, 
chany_0__1__out_97_, 
chany_0__1__in_98_, 
chany_0__1__out_99_, 
chany_0__1__midout_0_ ,
chany_0__1__midout_1_ ,
chany_0__1__midout_2_ ,
chany_0__1__midout_3_ ,
chany_0__1__midout_4_ ,
chany_0__1__midout_5_ ,
chany_0__1__midout_6_ ,
chany_0__1__midout_7_ ,
chany_0__1__midout_8_ ,
chany_0__1__midout_9_ ,
chany_0__1__midout_10_ ,
chany_0__1__midout_11_ ,
chany_0__1__midout_12_ ,
chany_0__1__midout_13_ ,
chany_0__1__midout_14_ ,
chany_0__1__midout_15_ ,
chany_0__1__midout_16_ ,
chany_0__1__midout_17_ ,
chany_0__1__midout_18_ ,
chany_0__1__midout_19_ ,
chany_0__1__midout_20_ ,
chany_0__1__midout_21_ ,
chany_0__1__midout_22_ ,
chany_0__1__midout_23_ ,
chany_0__1__midout_24_ ,
chany_0__1__midout_25_ ,
chany_0__1__midout_26_ ,
chany_0__1__midout_27_ ,
chany_0__1__midout_28_ ,
chany_0__1__midout_29_ ,
chany_0__1__midout_30_ ,
chany_0__1__midout_31_ ,
chany_0__1__midout_32_ ,
chany_0__1__midout_33_ ,
chany_0__1__midout_34_ ,
chany_0__1__midout_35_ ,
chany_0__1__midout_36_ ,
chany_0__1__midout_37_ ,
chany_0__1__midout_38_ ,
chany_0__1__midout_39_ ,
chany_0__1__midout_40_ ,
chany_0__1__midout_41_ ,
chany_0__1__midout_42_ ,
chany_0__1__midout_43_ ,
chany_0__1__midout_44_ ,
chany_0__1__midout_45_ ,
chany_0__1__midout_46_ ,
chany_0__1__midout_47_ ,
chany_0__1__midout_48_ ,
chany_0__1__midout_49_ ,
chany_0__1__midout_50_ ,
chany_0__1__midout_51_ ,
chany_0__1__midout_52_ ,
chany_0__1__midout_53_ ,
chany_0__1__midout_54_ ,
chany_0__1__midout_55_ ,
chany_0__1__midout_56_ ,
chany_0__1__midout_57_ ,
chany_0__1__midout_58_ ,
chany_0__1__midout_59_ ,
chany_0__1__midout_60_ ,
chany_0__1__midout_61_ ,
chany_0__1__midout_62_ ,
chany_0__1__midout_63_ ,
chany_0__1__midout_64_ ,
chany_0__1__midout_65_ ,
chany_0__1__midout_66_ ,
chany_0__1__midout_67_ ,
chany_0__1__midout_68_ ,
chany_0__1__midout_69_ ,
chany_0__1__midout_70_ ,
chany_0__1__midout_71_ ,
chany_0__1__midout_72_ ,
chany_0__1__midout_73_ ,
chany_0__1__midout_74_ ,
chany_0__1__midout_75_ ,
chany_0__1__midout_76_ ,
chany_0__1__midout_77_ ,
chany_0__1__midout_78_ ,
chany_0__1__midout_79_ ,
chany_0__1__midout_80_ ,
chany_0__1__midout_81_ ,
chany_0__1__midout_82_ ,
chany_0__1__midout_83_ ,
chany_0__1__midout_84_ ,
chany_0__1__midout_85_ ,
chany_0__1__midout_86_ ,
chany_0__1__midout_87_ ,
chany_0__1__midout_88_ ,
chany_0__1__midout_89_ ,
chany_0__1__midout_90_ ,
chany_0__1__midout_91_ ,
chany_0__1__midout_92_ ,
chany_0__1__midout_93_ ,
chany_0__1__midout_94_ ,
chany_0__1__midout_95_ ,
chany_0__1__midout_96_ ,
chany_0__1__midout_97_ ,
chany_0__1__midout_98_ ,
chany_0__1__midout_99_ 
);
//----- END call Channel-Y [0][1] module -----

//----- BEGIN call Channel-Y [1][1] module -----

chany_1__1_ chany_1__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
chany_1__1__out_0_, 
chany_1__1__in_1_, 
chany_1__1__out_2_, 
chany_1__1__in_3_, 
chany_1__1__out_4_, 
chany_1__1__in_5_, 
chany_1__1__out_6_, 
chany_1__1__in_7_, 
chany_1__1__out_8_, 
chany_1__1__in_9_, 
chany_1__1__out_10_, 
chany_1__1__in_11_, 
chany_1__1__out_12_, 
chany_1__1__in_13_, 
chany_1__1__out_14_, 
chany_1__1__in_15_, 
chany_1__1__out_16_, 
chany_1__1__in_17_, 
chany_1__1__out_18_, 
chany_1__1__in_19_, 
chany_1__1__out_20_, 
chany_1__1__in_21_, 
chany_1__1__out_22_, 
chany_1__1__in_23_, 
chany_1__1__out_24_, 
chany_1__1__in_25_, 
chany_1__1__out_26_, 
chany_1__1__in_27_, 
chany_1__1__out_28_, 
chany_1__1__in_29_, 
chany_1__1__out_30_, 
chany_1__1__in_31_, 
chany_1__1__out_32_, 
chany_1__1__in_33_, 
chany_1__1__out_34_, 
chany_1__1__in_35_, 
chany_1__1__out_36_, 
chany_1__1__in_37_, 
chany_1__1__out_38_, 
chany_1__1__in_39_, 
chany_1__1__out_40_, 
chany_1__1__in_41_, 
chany_1__1__out_42_, 
chany_1__1__in_43_, 
chany_1__1__out_44_, 
chany_1__1__in_45_, 
chany_1__1__out_46_, 
chany_1__1__in_47_, 
chany_1__1__out_48_, 
chany_1__1__in_49_, 
chany_1__1__out_50_, 
chany_1__1__in_51_, 
chany_1__1__out_52_, 
chany_1__1__in_53_, 
chany_1__1__out_54_, 
chany_1__1__in_55_, 
chany_1__1__out_56_, 
chany_1__1__in_57_, 
chany_1__1__out_58_, 
chany_1__1__in_59_, 
chany_1__1__out_60_, 
chany_1__1__in_61_, 
chany_1__1__out_62_, 
chany_1__1__in_63_, 
chany_1__1__out_64_, 
chany_1__1__in_65_, 
chany_1__1__out_66_, 
chany_1__1__in_67_, 
chany_1__1__out_68_, 
chany_1__1__in_69_, 
chany_1__1__out_70_, 
chany_1__1__in_71_, 
chany_1__1__out_72_, 
chany_1__1__in_73_, 
chany_1__1__out_74_, 
chany_1__1__in_75_, 
chany_1__1__out_76_, 
chany_1__1__in_77_, 
chany_1__1__out_78_, 
chany_1__1__in_79_, 
chany_1__1__out_80_, 
chany_1__1__in_81_, 
chany_1__1__out_82_, 
chany_1__1__in_83_, 
chany_1__1__out_84_, 
chany_1__1__in_85_, 
chany_1__1__out_86_, 
chany_1__1__in_87_, 
chany_1__1__out_88_, 
chany_1__1__in_89_, 
chany_1__1__out_90_, 
chany_1__1__in_91_, 
chany_1__1__out_92_, 
chany_1__1__in_93_, 
chany_1__1__out_94_, 
chany_1__1__in_95_, 
chany_1__1__out_96_, 
chany_1__1__in_97_, 
chany_1__1__out_98_, 
chany_1__1__in_99_, 
chany_1__1__in_0_, 
chany_1__1__out_1_, 
chany_1__1__in_2_, 
chany_1__1__out_3_, 
chany_1__1__in_4_, 
chany_1__1__out_5_, 
chany_1__1__in_6_, 
chany_1__1__out_7_, 
chany_1__1__in_8_, 
chany_1__1__out_9_, 
chany_1__1__in_10_, 
chany_1__1__out_11_, 
chany_1__1__in_12_, 
chany_1__1__out_13_, 
chany_1__1__in_14_, 
chany_1__1__out_15_, 
chany_1__1__in_16_, 
chany_1__1__out_17_, 
chany_1__1__in_18_, 
chany_1__1__out_19_, 
chany_1__1__in_20_, 
chany_1__1__out_21_, 
chany_1__1__in_22_, 
chany_1__1__out_23_, 
chany_1__1__in_24_, 
chany_1__1__out_25_, 
chany_1__1__in_26_, 
chany_1__1__out_27_, 
chany_1__1__in_28_, 
chany_1__1__out_29_, 
chany_1__1__in_30_, 
chany_1__1__out_31_, 
chany_1__1__in_32_, 
chany_1__1__out_33_, 
chany_1__1__in_34_, 
chany_1__1__out_35_, 
chany_1__1__in_36_, 
chany_1__1__out_37_, 
chany_1__1__in_38_, 
chany_1__1__out_39_, 
chany_1__1__in_40_, 
chany_1__1__out_41_, 
chany_1__1__in_42_, 
chany_1__1__out_43_, 
chany_1__1__in_44_, 
chany_1__1__out_45_, 
chany_1__1__in_46_, 
chany_1__1__out_47_, 
chany_1__1__in_48_, 
chany_1__1__out_49_, 
chany_1__1__in_50_, 
chany_1__1__out_51_, 
chany_1__1__in_52_, 
chany_1__1__out_53_, 
chany_1__1__in_54_, 
chany_1__1__out_55_, 
chany_1__1__in_56_, 
chany_1__1__out_57_, 
chany_1__1__in_58_, 
chany_1__1__out_59_, 
chany_1__1__in_60_, 
chany_1__1__out_61_, 
chany_1__1__in_62_, 
chany_1__1__out_63_, 
chany_1__1__in_64_, 
chany_1__1__out_65_, 
chany_1__1__in_66_, 
chany_1__1__out_67_, 
chany_1__1__in_68_, 
chany_1__1__out_69_, 
chany_1__1__in_70_, 
chany_1__1__out_71_, 
chany_1__1__in_72_, 
chany_1__1__out_73_, 
chany_1__1__in_74_, 
chany_1__1__out_75_, 
chany_1__1__in_76_, 
chany_1__1__out_77_, 
chany_1__1__in_78_, 
chany_1__1__out_79_, 
chany_1__1__in_80_, 
chany_1__1__out_81_, 
chany_1__1__in_82_, 
chany_1__1__out_83_, 
chany_1__1__in_84_, 
chany_1__1__out_85_, 
chany_1__1__in_86_, 
chany_1__1__out_87_, 
chany_1__1__in_88_, 
chany_1__1__out_89_, 
chany_1__1__in_90_, 
chany_1__1__out_91_, 
chany_1__1__in_92_, 
chany_1__1__out_93_, 
chany_1__1__in_94_, 
chany_1__1__out_95_, 
chany_1__1__in_96_, 
chany_1__1__out_97_, 
chany_1__1__in_98_, 
chany_1__1__out_99_, 
chany_1__1__midout_0_ ,
chany_1__1__midout_1_ ,
chany_1__1__midout_2_ ,
chany_1__1__midout_3_ ,
chany_1__1__midout_4_ ,
chany_1__1__midout_5_ ,
chany_1__1__midout_6_ ,
chany_1__1__midout_7_ ,
chany_1__1__midout_8_ ,
chany_1__1__midout_9_ ,
chany_1__1__midout_10_ ,
chany_1__1__midout_11_ ,
chany_1__1__midout_12_ ,
chany_1__1__midout_13_ ,
chany_1__1__midout_14_ ,
chany_1__1__midout_15_ ,
chany_1__1__midout_16_ ,
chany_1__1__midout_17_ ,
chany_1__1__midout_18_ ,
chany_1__1__midout_19_ ,
chany_1__1__midout_20_ ,
chany_1__1__midout_21_ ,
chany_1__1__midout_22_ ,
chany_1__1__midout_23_ ,
chany_1__1__midout_24_ ,
chany_1__1__midout_25_ ,
chany_1__1__midout_26_ ,
chany_1__1__midout_27_ ,
chany_1__1__midout_28_ ,
chany_1__1__midout_29_ ,
chany_1__1__midout_30_ ,
chany_1__1__midout_31_ ,
chany_1__1__midout_32_ ,
chany_1__1__midout_33_ ,
chany_1__1__midout_34_ ,
chany_1__1__midout_35_ ,
chany_1__1__midout_36_ ,
chany_1__1__midout_37_ ,
chany_1__1__midout_38_ ,
chany_1__1__midout_39_ ,
chany_1__1__midout_40_ ,
chany_1__1__midout_41_ ,
chany_1__1__midout_42_ ,
chany_1__1__midout_43_ ,
chany_1__1__midout_44_ ,
chany_1__1__midout_45_ ,
chany_1__1__midout_46_ ,
chany_1__1__midout_47_ ,
chany_1__1__midout_48_ ,
chany_1__1__midout_49_ ,
chany_1__1__midout_50_ ,
chany_1__1__midout_51_ ,
chany_1__1__midout_52_ ,
chany_1__1__midout_53_ ,
chany_1__1__midout_54_ ,
chany_1__1__midout_55_ ,
chany_1__1__midout_56_ ,
chany_1__1__midout_57_ ,
chany_1__1__midout_58_ ,
chany_1__1__midout_59_ ,
chany_1__1__midout_60_ ,
chany_1__1__midout_61_ ,
chany_1__1__midout_62_ ,
chany_1__1__midout_63_ ,
chany_1__1__midout_64_ ,
chany_1__1__midout_65_ ,
chany_1__1__midout_66_ ,
chany_1__1__midout_67_ ,
chany_1__1__midout_68_ ,
chany_1__1__midout_69_ ,
chany_1__1__midout_70_ ,
chany_1__1__midout_71_ ,
chany_1__1__midout_72_ ,
chany_1__1__midout_73_ ,
chany_1__1__midout_74_ ,
chany_1__1__midout_75_ ,
chany_1__1__midout_76_ ,
chany_1__1__midout_77_ ,
chany_1__1__midout_78_ ,
chany_1__1__midout_79_ ,
chany_1__1__midout_80_ ,
chany_1__1__midout_81_ ,
chany_1__1__midout_82_ ,
chany_1__1__midout_83_ ,
chany_1__1__midout_84_ ,
chany_1__1__midout_85_ ,
chany_1__1__midout_86_ ,
chany_1__1__midout_87_ ,
chany_1__1__midout_88_ ,
chany_1__1__midout_89_ ,
chany_1__1__midout_90_ ,
chany_1__1__midout_91_ ,
chany_1__1__midout_92_ ,
chany_1__1__midout_93_ ,
chany_1__1__midout_94_ ,
chany_1__1__midout_95_ ,
chany_1__1__midout_96_ ,
chany_1__1__midout_97_ ,
chany_1__1__midout_98_ ,
chany_1__1__midout_99_ 
);
//----- END call Channel-Y [1][1] module -----

//----- BEGIN Call Connection Box-X direction [1][0] module -----
cbx_1__0_ cbx_1__0__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
//----- right side inputs: channel track middle outputs -----
chanx_1__0__midout_0_, 
chanx_1__0__midout_1_, 
chanx_1__0__midout_2_, 
chanx_1__0__midout_3_, 
chanx_1__0__midout_4_, 
chanx_1__0__midout_5_, 
chanx_1__0__midout_6_, 
chanx_1__0__midout_7_, 
chanx_1__0__midout_8_, 
chanx_1__0__midout_9_, 
chanx_1__0__midout_10_, 
chanx_1__0__midout_11_, 
chanx_1__0__midout_12_, 
chanx_1__0__midout_13_, 
chanx_1__0__midout_14_, 
chanx_1__0__midout_15_, 
chanx_1__0__midout_16_, 
chanx_1__0__midout_17_, 
chanx_1__0__midout_18_, 
chanx_1__0__midout_19_, 
chanx_1__0__midout_20_, 
chanx_1__0__midout_21_, 
chanx_1__0__midout_22_, 
chanx_1__0__midout_23_, 
chanx_1__0__midout_24_, 
chanx_1__0__midout_25_, 
chanx_1__0__midout_26_, 
chanx_1__0__midout_27_, 
chanx_1__0__midout_28_, 
chanx_1__0__midout_29_, 
chanx_1__0__midout_30_, 
chanx_1__0__midout_31_, 
chanx_1__0__midout_32_, 
chanx_1__0__midout_33_, 
chanx_1__0__midout_34_, 
chanx_1__0__midout_35_, 
chanx_1__0__midout_36_, 
chanx_1__0__midout_37_, 
chanx_1__0__midout_38_, 
chanx_1__0__midout_39_, 
chanx_1__0__midout_40_, 
chanx_1__0__midout_41_, 
chanx_1__0__midout_42_, 
chanx_1__0__midout_43_, 
chanx_1__0__midout_44_, 
chanx_1__0__midout_45_, 
chanx_1__0__midout_46_, 
chanx_1__0__midout_47_, 
chanx_1__0__midout_48_, 
chanx_1__0__midout_49_, 
chanx_1__0__midout_50_, 
chanx_1__0__midout_51_, 
chanx_1__0__midout_52_, 
chanx_1__0__midout_53_, 
chanx_1__0__midout_54_, 
chanx_1__0__midout_55_, 
chanx_1__0__midout_56_, 
chanx_1__0__midout_57_, 
chanx_1__0__midout_58_, 
chanx_1__0__midout_59_, 
chanx_1__0__midout_60_, 
chanx_1__0__midout_61_, 
chanx_1__0__midout_62_, 
chanx_1__0__midout_63_, 
chanx_1__0__midout_64_, 
chanx_1__0__midout_65_, 
chanx_1__0__midout_66_, 
chanx_1__0__midout_67_, 
chanx_1__0__midout_68_, 
chanx_1__0__midout_69_, 
chanx_1__0__midout_70_, 
chanx_1__0__midout_71_, 
chanx_1__0__midout_72_, 
chanx_1__0__midout_73_, 
chanx_1__0__midout_74_, 
chanx_1__0__midout_75_, 
chanx_1__0__midout_76_, 
chanx_1__0__midout_77_, 
chanx_1__0__midout_78_, 
chanx_1__0__midout_79_, 
chanx_1__0__midout_80_, 
chanx_1__0__midout_81_, 
chanx_1__0__midout_82_, 
chanx_1__0__midout_83_, 
chanx_1__0__midout_84_, 
chanx_1__0__midout_85_, 
chanx_1__0__midout_86_, 
chanx_1__0__midout_87_, 
chanx_1__0__midout_88_, 
chanx_1__0__midout_89_, 
chanx_1__0__midout_90_, 
chanx_1__0__midout_91_, 
chanx_1__0__midout_92_, 
chanx_1__0__midout_93_, 
chanx_1__0__midout_94_, 
chanx_1__0__midout_95_, 
chanx_1__0__midout_96_, 
chanx_1__0__midout_97_, 
chanx_1__0__midout_98_, 
chanx_1__0__midout_99_, 
//----- top side outputs: CLB input pins -----
 grid_1__1__pin_0__2__2_, 
 grid_1__1__pin_0__2__6_, 
 grid_1__1__pin_0__2__10_, 
 grid_1__1__pin_0__2__14_, 
 grid_1__1__pin_0__2__18_, 
 grid_1__1__pin_0__2__22_, 
 grid_1__1__pin_0__2__26_, 
 grid_1__1__pin_0__2__30_, 
 grid_1__1__pin_0__2__34_, 
 grid_1__1__pin_0__2__38_, 
 grid_1__1__pin_0__2__50_, 
//----- bottom side outputs: CLB input pins -----
 grid_1__0__pin_0__0__0_, 
 grid_1__0__pin_0__0__2_, 
 grid_1__0__pin_0__0__4_, 
 grid_1__0__pin_0__0__6_, 
 grid_1__0__pin_0__0__8_, 
 grid_1__0__pin_0__0__10_, 
 grid_1__0__pin_0__0__12_, 
 grid_1__0__pin_0__0__14_, 
sram_blwl_bl[440:583] ,
sram_blwl_wl[440:583] ,
sram_blwl_blb[440:583] );
//----- END call Connection Box-X direction [1][0] module -----

//----- BEGIN Call Connection Box-X direction [1][1] module -----
cbx_1__1_ cbx_1__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
//----- right side inputs: channel track middle outputs -----
chanx_1__1__midout_0_, 
chanx_1__1__midout_1_, 
chanx_1__1__midout_2_, 
chanx_1__1__midout_3_, 
chanx_1__1__midout_4_, 
chanx_1__1__midout_5_, 
chanx_1__1__midout_6_, 
chanx_1__1__midout_7_, 
chanx_1__1__midout_8_, 
chanx_1__1__midout_9_, 
chanx_1__1__midout_10_, 
chanx_1__1__midout_11_, 
chanx_1__1__midout_12_, 
chanx_1__1__midout_13_, 
chanx_1__1__midout_14_, 
chanx_1__1__midout_15_, 
chanx_1__1__midout_16_, 
chanx_1__1__midout_17_, 
chanx_1__1__midout_18_, 
chanx_1__1__midout_19_, 
chanx_1__1__midout_20_, 
chanx_1__1__midout_21_, 
chanx_1__1__midout_22_, 
chanx_1__1__midout_23_, 
chanx_1__1__midout_24_, 
chanx_1__1__midout_25_, 
chanx_1__1__midout_26_, 
chanx_1__1__midout_27_, 
chanx_1__1__midout_28_, 
chanx_1__1__midout_29_, 
chanx_1__1__midout_30_, 
chanx_1__1__midout_31_, 
chanx_1__1__midout_32_, 
chanx_1__1__midout_33_, 
chanx_1__1__midout_34_, 
chanx_1__1__midout_35_, 
chanx_1__1__midout_36_, 
chanx_1__1__midout_37_, 
chanx_1__1__midout_38_, 
chanx_1__1__midout_39_, 
chanx_1__1__midout_40_, 
chanx_1__1__midout_41_, 
chanx_1__1__midout_42_, 
chanx_1__1__midout_43_, 
chanx_1__1__midout_44_, 
chanx_1__1__midout_45_, 
chanx_1__1__midout_46_, 
chanx_1__1__midout_47_, 
chanx_1__1__midout_48_, 
chanx_1__1__midout_49_, 
chanx_1__1__midout_50_, 
chanx_1__1__midout_51_, 
chanx_1__1__midout_52_, 
chanx_1__1__midout_53_, 
chanx_1__1__midout_54_, 
chanx_1__1__midout_55_, 
chanx_1__1__midout_56_, 
chanx_1__1__midout_57_, 
chanx_1__1__midout_58_, 
chanx_1__1__midout_59_, 
chanx_1__1__midout_60_, 
chanx_1__1__midout_61_, 
chanx_1__1__midout_62_, 
chanx_1__1__midout_63_, 
chanx_1__1__midout_64_, 
chanx_1__1__midout_65_, 
chanx_1__1__midout_66_, 
chanx_1__1__midout_67_, 
chanx_1__1__midout_68_, 
chanx_1__1__midout_69_, 
chanx_1__1__midout_70_, 
chanx_1__1__midout_71_, 
chanx_1__1__midout_72_, 
chanx_1__1__midout_73_, 
chanx_1__1__midout_74_, 
chanx_1__1__midout_75_, 
chanx_1__1__midout_76_, 
chanx_1__1__midout_77_, 
chanx_1__1__midout_78_, 
chanx_1__1__midout_79_, 
chanx_1__1__midout_80_, 
chanx_1__1__midout_81_, 
chanx_1__1__midout_82_, 
chanx_1__1__midout_83_, 
chanx_1__1__midout_84_, 
chanx_1__1__midout_85_, 
chanx_1__1__midout_86_, 
chanx_1__1__midout_87_, 
chanx_1__1__midout_88_, 
chanx_1__1__midout_89_, 
chanx_1__1__midout_90_, 
chanx_1__1__midout_91_, 
chanx_1__1__midout_92_, 
chanx_1__1__midout_93_, 
chanx_1__1__midout_94_, 
chanx_1__1__midout_95_, 
chanx_1__1__midout_96_, 
chanx_1__1__midout_97_, 
chanx_1__1__midout_98_, 
chanx_1__1__midout_99_, 
//----- top side outputs: CLB input pins -----
 grid_1__2__pin_0__2__0_, 
 grid_1__2__pin_0__2__2_, 
 grid_1__2__pin_0__2__4_, 
 grid_1__2__pin_0__2__6_, 
 grid_1__2__pin_0__2__8_, 
 grid_1__2__pin_0__2__10_, 
 grid_1__2__pin_0__2__12_, 
 grid_1__2__pin_0__2__14_, 
//----- bottom side outputs: CLB input pins -----
 grid_1__1__pin_0__0__0_, 
 grid_1__1__pin_0__0__4_, 
 grid_1__1__pin_0__0__8_, 
 grid_1__1__pin_0__0__12_, 
 grid_1__1__pin_0__0__16_, 
 grid_1__1__pin_0__0__20_, 
 grid_1__1__pin_0__0__24_, 
 grid_1__1__pin_0__0__28_, 
 grid_1__1__pin_0__0__32_, 
 grid_1__1__pin_0__0__36_, 
sram_blwl_bl[584:727] ,
sram_blwl_wl[584:727] ,
sram_blwl_blb[584:727] );
//----- END call Connection Box-X direction [1][1] module -----

//----- BEGIN Call Connection Box-Y direction [0][1] module -----
cby_0__1_ cby_0__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
//----- top side inputs: channel track middle outputs -----
chany_0__1__midout_0_, 
chany_0__1__midout_1_, 
chany_0__1__midout_2_, 
chany_0__1__midout_3_, 
chany_0__1__midout_4_, 
chany_0__1__midout_5_, 
chany_0__1__midout_6_, 
chany_0__1__midout_7_, 
chany_0__1__midout_8_, 
chany_0__1__midout_9_, 
chany_0__1__midout_10_, 
chany_0__1__midout_11_, 
chany_0__1__midout_12_, 
chany_0__1__midout_13_, 
chany_0__1__midout_14_, 
chany_0__1__midout_15_, 
chany_0__1__midout_16_, 
chany_0__1__midout_17_, 
chany_0__1__midout_18_, 
chany_0__1__midout_19_, 
chany_0__1__midout_20_, 
chany_0__1__midout_21_, 
chany_0__1__midout_22_, 
chany_0__1__midout_23_, 
chany_0__1__midout_24_, 
chany_0__1__midout_25_, 
chany_0__1__midout_26_, 
chany_0__1__midout_27_, 
chany_0__1__midout_28_, 
chany_0__1__midout_29_, 
chany_0__1__midout_30_, 
chany_0__1__midout_31_, 
chany_0__1__midout_32_, 
chany_0__1__midout_33_, 
chany_0__1__midout_34_, 
chany_0__1__midout_35_, 
chany_0__1__midout_36_, 
chany_0__1__midout_37_, 
chany_0__1__midout_38_, 
chany_0__1__midout_39_, 
chany_0__1__midout_40_, 
chany_0__1__midout_41_, 
chany_0__1__midout_42_, 
chany_0__1__midout_43_, 
chany_0__1__midout_44_, 
chany_0__1__midout_45_, 
chany_0__1__midout_46_, 
chany_0__1__midout_47_, 
chany_0__1__midout_48_, 
chany_0__1__midout_49_, 
chany_0__1__midout_50_, 
chany_0__1__midout_51_, 
chany_0__1__midout_52_, 
chany_0__1__midout_53_, 
chany_0__1__midout_54_, 
chany_0__1__midout_55_, 
chany_0__1__midout_56_, 
chany_0__1__midout_57_, 
chany_0__1__midout_58_, 
chany_0__1__midout_59_, 
chany_0__1__midout_60_, 
chany_0__1__midout_61_, 
chany_0__1__midout_62_, 
chany_0__1__midout_63_, 
chany_0__1__midout_64_, 
chany_0__1__midout_65_, 
chany_0__1__midout_66_, 
chany_0__1__midout_67_, 
chany_0__1__midout_68_, 
chany_0__1__midout_69_, 
chany_0__1__midout_70_, 
chany_0__1__midout_71_, 
chany_0__1__midout_72_, 
chany_0__1__midout_73_, 
chany_0__1__midout_74_, 
chany_0__1__midout_75_, 
chany_0__1__midout_76_, 
chany_0__1__midout_77_, 
chany_0__1__midout_78_, 
chany_0__1__midout_79_, 
chany_0__1__midout_80_, 
chany_0__1__midout_81_, 
chany_0__1__midout_82_, 
chany_0__1__midout_83_, 
chany_0__1__midout_84_, 
chany_0__1__midout_85_, 
chany_0__1__midout_86_, 
chany_0__1__midout_87_, 
chany_0__1__midout_88_, 
chany_0__1__midout_89_, 
chany_0__1__midout_90_, 
chany_0__1__midout_91_, 
chany_0__1__midout_92_, 
chany_0__1__midout_93_, 
chany_0__1__midout_94_, 
chany_0__1__midout_95_, 
chany_0__1__midout_96_, 
chany_0__1__midout_97_, 
chany_0__1__midout_98_, 
chany_0__1__midout_99_, 
//----- right side outputs: CLB input pins -----
 grid_1__1__pin_0__3__3_, 
 grid_1__1__pin_0__3__7_, 
 grid_1__1__pin_0__3__11_, 
 grid_1__1__pin_0__3__15_, 
 grid_1__1__pin_0__3__19_, 
 grid_1__1__pin_0__3__23_, 
 grid_1__1__pin_0__3__27_, 
 grid_1__1__pin_0__3__31_, 
 grid_1__1__pin_0__3__35_, 
 grid_1__1__pin_0__3__39_, 
//----- left side outputs: CLB input pins -----
 grid_0__1__pin_0__1__0_, 
 grid_0__1__pin_0__1__2_, 
 grid_0__1__pin_0__1__4_, 
 grid_0__1__pin_0__1__6_, 
 grid_0__1__pin_0__1__8_, 
 grid_0__1__pin_0__1__10_, 
 grid_0__1__pin_0__1__12_, 
 grid_0__1__pin_0__1__14_, 
sram_blwl_bl[728:871] ,
sram_blwl_wl[728:871] ,
sram_blwl_blb[728:871] );
//----- END call Connection Box-Y direction [0][1] module -----

//----- BEGIN Call Connection Box-Y direction [1][1] module -----
cby_1__1_ cby_1__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
//----- top side inputs: channel track middle outputs -----
chany_1__1__midout_0_, 
chany_1__1__midout_1_, 
chany_1__1__midout_2_, 
chany_1__1__midout_3_, 
chany_1__1__midout_4_, 
chany_1__1__midout_5_, 
chany_1__1__midout_6_, 
chany_1__1__midout_7_, 
chany_1__1__midout_8_, 
chany_1__1__midout_9_, 
chany_1__1__midout_10_, 
chany_1__1__midout_11_, 
chany_1__1__midout_12_, 
chany_1__1__midout_13_, 
chany_1__1__midout_14_, 
chany_1__1__midout_15_, 
chany_1__1__midout_16_, 
chany_1__1__midout_17_, 
chany_1__1__midout_18_, 
chany_1__1__midout_19_, 
chany_1__1__midout_20_, 
chany_1__1__midout_21_, 
chany_1__1__midout_22_, 
chany_1__1__midout_23_, 
chany_1__1__midout_24_, 
chany_1__1__midout_25_, 
chany_1__1__midout_26_, 
chany_1__1__midout_27_, 
chany_1__1__midout_28_, 
chany_1__1__midout_29_, 
chany_1__1__midout_30_, 
chany_1__1__midout_31_, 
chany_1__1__midout_32_, 
chany_1__1__midout_33_, 
chany_1__1__midout_34_, 
chany_1__1__midout_35_, 
chany_1__1__midout_36_, 
chany_1__1__midout_37_, 
chany_1__1__midout_38_, 
chany_1__1__midout_39_, 
chany_1__1__midout_40_, 
chany_1__1__midout_41_, 
chany_1__1__midout_42_, 
chany_1__1__midout_43_, 
chany_1__1__midout_44_, 
chany_1__1__midout_45_, 
chany_1__1__midout_46_, 
chany_1__1__midout_47_, 
chany_1__1__midout_48_, 
chany_1__1__midout_49_, 
chany_1__1__midout_50_, 
chany_1__1__midout_51_, 
chany_1__1__midout_52_, 
chany_1__1__midout_53_, 
chany_1__1__midout_54_, 
chany_1__1__midout_55_, 
chany_1__1__midout_56_, 
chany_1__1__midout_57_, 
chany_1__1__midout_58_, 
chany_1__1__midout_59_, 
chany_1__1__midout_60_, 
chany_1__1__midout_61_, 
chany_1__1__midout_62_, 
chany_1__1__midout_63_, 
chany_1__1__midout_64_, 
chany_1__1__midout_65_, 
chany_1__1__midout_66_, 
chany_1__1__midout_67_, 
chany_1__1__midout_68_, 
chany_1__1__midout_69_, 
chany_1__1__midout_70_, 
chany_1__1__midout_71_, 
chany_1__1__midout_72_, 
chany_1__1__midout_73_, 
chany_1__1__midout_74_, 
chany_1__1__midout_75_, 
chany_1__1__midout_76_, 
chany_1__1__midout_77_, 
chany_1__1__midout_78_, 
chany_1__1__midout_79_, 
chany_1__1__midout_80_, 
chany_1__1__midout_81_, 
chany_1__1__midout_82_, 
chany_1__1__midout_83_, 
chany_1__1__midout_84_, 
chany_1__1__midout_85_, 
chany_1__1__midout_86_, 
chany_1__1__midout_87_, 
chany_1__1__midout_88_, 
chany_1__1__midout_89_, 
chany_1__1__midout_90_, 
chany_1__1__midout_91_, 
chany_1__1__midout_92_, 
chany_1__1__midout_93_, 
chany_1__1__midout_94_, 
chany_1__1__midout_95_, 
chany_1__1__midout_96_, 
chany_1__1__midout_97_, 
chany_1__1__midout_98_, 
chany_1__1__midout_99_, 
//----- right side outputs: CLB input pins -----
 grid_2__1__pin_0__3__0_, 
 grid_2__1__pin_0__3__2_, 
 grid_2__1__pin_0__3__4_, 
 grid_2__1__pin_0__3__6_, 
 grid_2__1__pin_0__3__8_, 
 grid_2__1__pin_0__3__10_, 
 grid_2__1__pin_0__3__12_, 
 grid_2__1__pin_0__3__14_, 
//----- left side outputs: CLB input pins -----
 grid_1__1__pin_0__1__1_, 
 grid_1__1__pin_0__1__5_, 
 grid_1__1__pin_0__1__9_, 
 grid_1__1__pin_0__1__13_, 
 grid_1__1__pin_0__1__17_, 
 grid_1__1__pin_0__1__21_, 
 grid_1__1__pin_0__1__25_, 
 grid_1__1__pin_0__1__29_, 
 grid_1__1__pin_0__1__33_, 
 grid_1__1__pin_0__1__37_, 
sram_blwl_bl[872:1015] ,
sram_blwl_wl[872:1015] ,
sram_blwl_blb[872:1015] );
//----- END call Connection Box-Y direction [1][1] module -----

//----- BEGIN call module Switch blocks [0][0] -----
sb_0__0_ sb_0__0__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
//----- top side channel ports-----
chany_0__1__out_0_, chany_0__1__in_1_, chany_0__1__out_2_, chany_0__1__in_3_, chany_0__1__out_4_, chany_0__1__in_5_, chany_0__1__out_6_, chany_0__1__in_7_, chany_0__1__out_8_, chany_0__1__in_9_, chany_0__1__out_10_, chany_0__1__in_11_, chany_0__1__out_12_, chany_0__1__in_13_, chany_0__1__out_14_, chany_0__1__in_15_, chany_0__1__out_16_, chany_0__1__in_17_, chany_0__1__out_18_, chany_0__1__in_19_, chany_0__1__out_20_, chany_0__1__in_21_, chany_0__1__out_22_, chany_0__1__in_23_, chany_0__1__out_24_, chany_0__1__in_25_, chany_0__1__out_26_, chany_0__1__in_27_, chany_0__1__out_28_, chany_0__1__in_29_, chany_0__1__out_30_, chany_0__1__in_31_, chany_0__1__out_32_, chany_0__1__in_33_, chany_0__1__out_34_, chany_0__1__in_35_, chany_0__1__out_36_, chany_0__1__in_37_, chany_0__1__out_38_, chany_0__1__in_39_, chany_0__1__out_40_, chany_0__1__in_41_, chany_0__1__out_42_, chany_0__1__in_43_, chany_0__1__out_44_, chany_0__1__in_45_, chany_0__1__out_46_, chany_0__1__in_47_, chany_0__1__out_48_, chany_0__1__in_49_, chany_0__1__out_50_, chany_0__1__in_51_, chany_0__1__out_52_, chany_0__1__in_53_, chany_0__1__out_54_, chany_0__1__in_55_, chany_0__1__out_56_, chany_0__1__in_57_, chany_0__1__out_58_, chany_0__1__in_59_, chany_0__1__out_60_, chany_0__1__in_61_, chany_0__1__out_62_, chany_0__1__in_63_, chany_0__1__out_64_, chany_0__1__in_65_, chany_0__1__out_66_, chany_0__1__in_67_, chany_0__1__out_68_, chany_0__1__in_69_, chany_0__1__out_70_, chany_0__1__in_71_, chany_0__1__out_72_, chany_0__1__in_73_, chany_0__1__out_74_, chany_0__1__in_75_, chany_0__1__out_76_, chany_0__1__in_77_, chany_0__1__out_78_, chany_0__1__in_79_, chany_0__1__out_80_, chany_0__1__in_81_, chany_0__1__out_82_, chany_0__1__in_83_, chany_0__1__out_84_, chany_0__1__in_85_, chany_0__1__out_86_, chany_0__1__in_87_, chany_0__1__out_88_, chany_0__1__in_89_, chany_0__1__out_90_, chany_0__1__in_91_, chany_0__1__out_92_, chany_0__1__in_93_, chany_0__1__out_94_, chany_0__1__in_95_, chany_0__1__out_96_, chany_0__1__in_97_, chany_0__1__out_98_, chany_0__1__in_99_, 
//----- top side inputs: CLB output pins -----
 grid_0__1__pin_0__1__1_, grid_0__1__pin_0__1__3_, grid_0__1__pin_0__1__5_, grid_0__1__pin_0__1__7_, grid_0__1__pin_0__1__9_, grid_0__1__pin_0__1__11_, grid_0__1__pin_0__1__13_, grid_0__1__pin_0__1__15_, grid_1__1__pin_0__3__43_, grid_1__1__pin_0__3__47_,
//----- right side channel ports-----
chanx_1__0__out_0_, chanx_1__0__in_1_, chanx_1__0__out_2_, chanx_1__0__in_3_, chanx_1__0__out_4_, chanx_1__0__in_5_, chanx_1__0__out_6_, chanx_1__0__in_7_, chanx_1__0__out_8_, chanx_1__0__in_9_, chanx_1__0__out_10_, chanx_1__0__in_11_, chanx_1__0__out_12_, chanx_1__0__in_13_, chanx_1__0__out_14_, chanx_1__0__in_15_, chanx_1__0__out_16_, chanx_1__0__in_17_, chanx_1__0__out_18_, chanx_1__0__in_19_, chanx_1__0__out_20_, chanx_1__0__in_21_, chanx_1__0__out_22_, chanx_1__0__in_23_, chanx_1__0__out_24_, chanx_1__0__in_25_, chanx_1__0__out_26_, chanx_1__0__in_27_, chanx_1__0__out_28_, chanx_1__0__in_29_, chanx_1__0__out_30_, chanx_1__0__in_31_, chanx_1__0__out_32_, chanx_1__0__in_33_, chanx_1__0__out_34_, chanx_1__0__in_35_, chanx_1__0__out_36_, chanx_1__0__in_37_, chanx_1__0__out_38_, chanx_1__0__in_39_, chanx_1__0__out_40_, chanx_1__0__in_41_, chanx_1__0__out_42_, chanx_1__0__in_43_, chanx_1__0__out_44_, chanx_1__0__in_45_, chanx_1__0__out_46_, chanx_1__0__in_47_, chanx_1__0__out_48_, chanx_1__0__in_49_, chanx_1__0__out_50_, chanx_1__0__in_51_, chanx_1__0__out_52_, chanx_1__0__in_53_, chanx_1__0__out_54_, chanx_1__0__in_55_, chanx_1__0__out_56_, chanx_1__0__in_57_, chanx_1__0__out_58_, chanx_1__0__in_59_, chanx_1__0__out_60_, chanx_1__0__in_61_, chanx_1__0__out_62_, chanx_1__0__in_63_, chanx_1__0__out_64_, chanx_1__0__in_65_, chanx_1__0__out_66_, chanx_1__0__in_67_, chanx_1__0__out_68_, chanx_1__0__in_69_, chanx_1__0__out_70_, chanx_1__0__in_71_, chanx_1__0__out_72_, chanx_1__0__in_73_, chanx_1__0__out_74_, chanx_1__0__in_75_, chanx_1__0__out_76_, chanx_1__0__in_77_, chanx_1__0__out_78_, chanx_1__0__in_79_, chanx_1__0__out_80_, chanx_1__0__in_81_, chanx_1__0__out_82_, chanx_1__0__in_83_, chanx_1__0__out_84_, chanx_1__0__in_85_, chanx_1__0__out_86_, chanx_1__0__in_87_, chanx_1__0__out_88_, chanx_1__0__in_89_, chanx_1__0__out_90_, chanx_1__0__in_91_, chanx_1__0__out_92_, chanx_1__0__in_93_, chanx_1__0__out_94_, chanx_1__0__in_95_, chanx_1__0__out_96_, chanx_1__0__in_97_, chanx_1__0__out_98_, chanx_1__0__in_99_, 
//----- right side inputs: CLB output pins -----
 grid_1__1__pin_0__2__42_, grid_1__1__pin_0__2__46_, grid_1__0__pin_0__0__1_, grid_1__0__pin_0__0__3_, grid_1__0__pin_0__0__5_, grid_1__0__pin_0__0__7_, grid_1__0__pin_0__0__9_, grid_1__0__pin_0__0__11_, grid_1__0__pin_0__0__13_, grid_1__0__pin_0__0__15_,
//----- bottom side channel ports-----

//----- bottom side inputs: CLB output pins -----

//----- left side channel ports-----

//----- left side inputs: CLB output pins -----

sram_blwl_bl[0:99] ,
sram_blwl_wl[0:99] ,
sram_blwl_blb[0:99] );
//----- END call module Switch blocks [0][0] -----

//----- BEGIN call module Switch blocks [0][1] -----
sb_0__1_ sb_0__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
//----- top side channel ports-----

//----- top side inputs: CLB output pins -----

//----- right side channel ports-----
chanx_1__1__out_0_, chanx_1__1__in_1_, chanx_1__1__out_2_, chanx_1__1__in_3_, chanx_1__1__out_4_, chanx_1__1__in_5_, chanx_1__1__out_6_, chanx_1__1__in_7_, chanx_1__1__out_8_, chanx_1__1__in_9_, chanx_1__1__out_10_, chanx_1__1__in_11_, chanx_1__1__out_12_, chanx_1__1__in_13_, chanx_1__1__out_14_, chanx_1__1__in_15_, chanx_1__1__out_16_, chanx_1__1__in_17_, chanx_1__1__out_18_, chanx_1__1__in_19_, chanx_1__1__out_20_, chanx_1__1__in_21_, chanx_1__1__out_22_, chanx_1__1__in_23_, chanx_1__1__out_24_, chanx_1__1__in_25_, chanx_1__1__out_26_, chanx_1__1__in_27_, chanx_1__1__out_28_, chanx_1__1__in_29_, chanx_1__1__out_30_, chanx_1__1__in_31_, chanx_1__1__out_32_, chanx_1__1__in_33_, chanx_1__1__out_34_, chanx_1__1__in_35_, chanx_1__1__out_36_, chanx_1__1__in_37_, chanx_1__1__out_38_, chanx_1__1__in_39_, chanx_1__1__out_40_, chanx_1__1__in_41_, chanx_1__1__out_42_, chanx_1__1__in_43_, chanx_1__1__out_44_, chanx_1__1__in_45_, chanx_1__1__out_46_, chanx_1__1__in_47_, chanx_1__1__out_48_, chanx_1__1__in_49_, chanx_1__1__out_50_, chanx_1__1__in_51_, chanx_1__1__out_52_, chanx_1__1__in_53_, chanx_1__1__out_54_, chanx_1__1__in_55_, chanx_1__1__out_56_, chanx_1__1__in_57_, chanx_1__1__out_58_, chanx_1__1__in_59_, chanx_1__1__out_60_, chanx_1__1__in_61_, chanx_1__1__out_62_, chanx_1__1__in_63_, chanx_1__1__out_64_, chanx_1__1__in_65_, chanx_1__1__out_66_, chanx_1__1__in_67_, chanx_1__1__out_68_, chanx_1__1__in_69_, chanx_1__1__out_70_, chanx_1__1__in_71_, chanx_1__1__out_72_, chanx_1__1__in_73_, chanx_1__1__out_74_, chanx_1__1__in_75_, chanx_1__1__out_76_, chanx_1__1__in_77_, chanx_1__1__out_78_, chanx_1__1__in_79_, chanx_1__1__out_80_, chanx_1__1__in_81_, chanx_1__1__out_82_, chanx_1__1__in_83_, chanx_1__1__out_84_, chanx_1__1__in_85_, chanx_1__1__out_86_, chanx_1__1__in_87_, chanx_1__1__out_88_, chanx_1__1__in_89_, chanx_1__1__out_90_, chanx_1__1__in_91_, chanx_1__1__out_92_, chanx_1__1__in_93_, chanx_1__1__out_94_, chanx_1__1__in_95_, chanx_1__1__out_96_, chanx_1__1__in_97_, chanx_1__1__out_98_, chanx_1__1__in_99_, 
//----- right side inputs: CLB output pins -----
 grid_1__2__pin_0__2__1_, grid_1__2__pin_0__2__3_, grid_1__2__pin_0__2__5_, grid_1__2__pin_0__2__7_, grid_1__2__pin_0__2__9_, grid_1__2__pin_0__2__11_, grid_1__2__pin_0__2__13_, grid_1__2__pin_0__2__15_, grid_1__1__pin_0__0__40_, grid_1__1__pin_0__0__44_, grid_1__1__pin_0__0__48_,
//----- bottom side channel ports-----
chany_0__1__in_0_, chany_0__1__out_1_, chany_0__1__in_2_, chany_0__1__out_3_, chany_0__1__in_4_, chany_0__1__out_5_, chany_0__1__in_6_, chany_0__1__out_7_, chany_0__1__in_8_, chany_0__1__out_9_, chany_0__1__in_10_, chany_0__1__out_11_, chany_0__1__in_12_, chany_0__1__out_13_, chany_0__1__in_14_, chany_0__1__out_15_, chany_0__1__in_16_, chany_0__1__out_17_, chany_0__1__in_18_, chany_0__1__out_19_, chany_0__1__in_20_, chany_0__1__out_21_, chany_0__1__in_22_, chany_0__1__out_23_, chany_0__1__in_24_, chany_0__1__out_25_, chany_0__1__in_26_, chany_0__1__out_27_, chany_0__1__in_28_, chany_0__1__out_29_, chany_0__1__in_30_, chany_0__1__out_31_, chany_0__1__in_32_, chany_0__1__out_33_, chany_0__1__in_34_, chany_0__1__out_35_, chany_0__1__in_36_, chany_0__1__out_37_, chany_0__1__in_38_, chany_0__1__out_39_, chany_0__1__in_40_, chany_0__1__out_41_, chany_0__1__in_42_, chany_0__1__out_43_, chany_0__1__in_44_, chany_0__1__out_45_, chany_0__1__in_46_, chany_0__1__out_47_, chany_0__1__in_48_, chany_0__1__out_49_, chany_0__1__in_50_, chany_0__1__out_51_, chany_0__1__in_52_, chany_0__1__out_53_, chany_0__1__in_54_, chany_0__1__out_55_, chany_0__1__in_56_, chany_0__1__out_57_, chany_0__1__in_58_, chany_0__1__out_59_, chany_0__1__in_60_, chany_0__1__out_61_, chany_0__1__in_62_, chany_0__1__out_63_, chany_0__1__in_64_, chany_0__1__out_65_, chany_0__1__in_66_, chany_0__1__out_67_, chany_0__1__in_68_, chany_0__1__out_69_, chany_0__1__in_70_, chany_0__1__out_71_, chany_0__1__in_72_, chany_0__1__out_73_, chany_0__1__in_74_, chany_0__1__out_75_, chany_0__1__in_76_, chany_0__1__out_77_, chany_0__1__in_78_, chany_0__1__out_79_, chany_0__1__in_80_, chany_0__1__out_81_, chany_0__1__in_82_, chany_0__1__out_83_, chany_0__1__in_84_, chany_0__1__out_85_, chany_0__1__in_86_, chany_0__1__out_87_, chany_0__1__in_88_, chany_0__1__out_89_, chany_0__1__in_90_, chany_0__1__out_91_, chany_0__1__in_92_, chany_0__1__out_93_, chany_0__1__in_94_, chany_0__1__out_95_, chany_0__1__in_96_, chany_0__1__out_97_, chany_0__1__in_98_, chany_0__1__out_99_, 
//----- bottom side inputs: CLB output pins -----
 grid_1__1__pin_0__3__43_, grid_1__1__pin_0__3__47_, grid_0__1__pin_0__1__1_, grid_0__1__pin_0__1__3_, grid_0__1__pin_0__1__5_, grid_0__1__pin_0__1__7_, grid_0__1__pin_0__1__9_, grid_0__1__pin_0__1__11_, grid_0__1__pin_0__1__13_, grid_0__1__pin_0__1__15_,
//----- left side channel ports-----

//----- left side inputs: CLB output pins -----

sram_blwl_bl[100:209] ,
sram_blwl_wl[100:209] ,
sram_blwl_blb[100:209] );
//----- END call module Switch blocks [0][1] -----

//----- BEGIN call module Switch blocks [1][0] -----
sb_1__0_ sb_1__0__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
//----- top side channel ports-----
chany_1__1__out_0_, chany_1__1__in_1_, chany_1__1__out_2_, chany_1__1__in_3_, chany_1__1__out_4_, chany_1__1__in_5_, chany_1__1__out_6_, chany_1__1__in_7_, chany_1__1__out_8_, chany_1__1__in_9_, chany_1__1__out_10_, chany_1__1__in_11_, chany_1__1__out_12_, chany_1__1__in_13_, chany_1__1__out_14_, chany_1__1__in_15_, chany_1__1__out_16_, chany_1__1__in_17_, chany_1__1__out_18_, chany_1__1__in_19_, chany_1__1__out_20_, chany_1__1__in_21_, chany_1__1__out_22_, chany_1__1__in_23_, chany_1__1__out_24_, chany_1__1__in_25_, chany_1__1__out_26_, chany_1__1__in_27_, chany_1__1__out_28_, chany_1__1__in_29_, chany_1__1__out_30_, chany_1__1__in_31_, chany_1__1__out_32_, chany_1__1__in_33_, chany_1__1__out_34_, chany_1__1__in_35_, chany_1__1__out_36_, chany_1__1__in_37_, chany_1__1__out_38_, chany_1__1__in_39_, chany_1__1__out_40_, chany_1__1__in_41_, chany_1__1__out_42_, chany_1__1__in_43_, chany_1__1__out_44_, chany_1__1__in_45_, chany_1__1__out_46_, chany_1__1__in_47_, chany_1__1__out_48_, chany_1__1__in_49_, chany_1__1__out_50_, chany_1__1__in_51_, chany_1__1__out_52_, chany_1__1__in_53_, chany_1__1__out_54_, chany_1__1__in_55_, chany_1__1__out_56_, chany_1__1__in_57_, chany_1__1__out_58_, chany_1__1__in_59_, chany_1__1__out_60_, chany_1__1__in_61_, chany_1__1__out_62_, chany_1__1__in_63_, chany_1__1__out_64_, chany_1__1__in_65_, chany_1__1__out_66_, chany_1__1__in_67_, chany_1__1__out_68_, chany_1__1__in_69_, chany_1__1__out_70_, chany_1__1__in_71_, chany_1__1__out_72_, chany_1__1__in_73_, chany_1__1__out_74_, chany_1__1__in_75_, chany_1__1__out_76_, chany_1__1__in_77_, chany_1__1__out_78_, chany_1__1__in_79_, chany_1__1__out_80_, chany_1__1__in_81_, chany_1__1__out_82_, chany_1__1__in_83_, chany_1__1__out_84_, chany_1__1__in_85_, chany_1__1__out_86_, chany_1__1__in_87_, chany_1__1__out_88_, chany_1__1__in_89_, chany_1__1__out_90_, chany_1__1__in_91_, chany_1__1__out_92_, chany_1__1__in_93_, chany_1__1__out_94_, chany_1__1__in_95_, chany_1__1__out_96_, chany_1__1__in_97_, chany_1__1__out_98_, chany_1__1__in_99_, 
//----- top side inputs: CLB output pins -----
 grid_1__1__pin_0__1__41_, grid_1__1__pin_0__1__45_, grid_1__1__pin_0__1__49_, grid_2__1__pin_0__3__1_, grid_2__1__pin_0__3__3_, grid_2__1__pin_0__3__5_, grid_2__1__pin_0__3__7_, grid_2__1__pin_0__3__9_, grid_2__1__pin_0__3__11_, grid_2__1__pin_0__3__13_, grid_2__1__pin_0__3__15_,
//----- right side channel ports-----

//----- right side inputs: CLB output pins -----

//----- bottom side channel ports-----

//----- bottom side inputs: CLB output pins -----

//----- left side channel ports-----
chanx_1__0__in_0_, chanx_1__0__out_1_, chanx_1__0__in_2_, chanx_1__0__out_3_, chanx_1__0__in_4_, chanx_1__0__out_5_, chanx_1__0__in_6_, chanx_1__0__out_7_, chanx_1__0__in_8_, chanx_1__0__out_9_, chanx_1__0__in_10_, chanx_1__0__out_11_, chanx_1__0__in_12_, chanx_1__0__out_13_, chanx_1__0__in_14_, chanx_1__0__out_15_, chanx_1__0__in_16_, chanx_1__0__out_17_, chanx_1__0__in_18_, chanx_1__0__out_19_, chanx_1__0__in_20_, chanx_1__0__out_21_, chanx_1__0__in_22_, chanx_1__0__out_23_, chanx_1__0__in_24_, chanx_1__0__out_25_, chanx_1__0__in_26_, chanx_1__0__out_27_, chanx_1__0__in_28_, chanx_1__0__out_29_, chanx_1__0__in_30_, chanx_1__0__out_31_, chanx_1__0__in_32_, chanx_1__0__out_33_, chanx_1__0__in_34_, chanx_1__0__out_35_, chanx_1__0__in_36_, chanx_1__0__out_37_, chanx_1__0__in_38_, chanx_1__0__out_39_, chanx_1__0__in_40_, chanx_1__0__out_41_, chanx_1__0__in_42_, chanx_1__0__out_43_, chanx_1__0__in_44_, chanx_1__0__out_45_, chanx_1__0__in_46_, chanx_1__0__out_47_, chanx_1__0__in_48_, chanx_1__0__out_49_, chanx_1__0__in_50_, chanx_1__0__out_51_, chanx_1__0__in_52_, chanx_1__0__out_53_, chanx_1__0__in_54_, chanx_1__0__out_55_, chanx_1__0__in_56_, chanx_1__0__out_57_, chanx_1__0__in_58_, chanx_1__0__out_59_, chanx_1__0__in_60_, chanx_1__0__out_61_, chanx_1__0__in_62_, chanx_1__0__out_63_, chanx_1__0__in_64_, chanx_1__0__out_65_, chanx_1__0__in_66_, chanx_1__0__out_67_, chanx_1__0__in_68_, chanx_1__0__out_69_, chanx_1__0__in_70_, chanx_1__0__out_71_, chanx_1__0__in_72_, chanx_1__0__out_73_, chanx_1__0__in_74_, chanx_1__0__out_75_, chanx_1__0__in_76_, chanx_1__0__out_77_, chanx_1__0__in_78_, chanx_1__0__out_79_, chanx_1__0__in_80_, chanx_1__0__out_81_, chanx_1__0__in_82_, chanx_1__0__out_83_, chanx_1__0__in_84_, chanx_1__0__out_85_, chanx_1__0__in_86_, chanx_1__0__out_87_, chanx_1__0__in_88_, chanx_1__0__out_89_, chanx_1__0__in_90_, chanx_1__0__out_91_, chanx_1__0__in_92_, chanx_1__0__out_93_, chanx_1__0__in_94_, chanx_1__0__out_95_, chanx_1__0__in_96_, chanx_1__0__out_97_, chanx_1__0__in_98_, chanx_1__0__out_99_, 
//----- left side inputs: CLB output pins -----
 grid_1__1__pin_0__2__42_, grid_1__1__pin_0__2__46_, grid_1__0__pin_0__0__1_, grid_1__0__pin_0__0__3_, grid_1__0__pin_0__0__5_, grid_1__0__pin_0__0__7_, grid_1__0__pin_0__0__9_, grid_1__0__pin_0__0__11_, grid_1__0__pin_0__0__13_, grid_1__0__pin_0__0__15_,
sram_blwl_bl[210:319] ,
sram_blwl_wl[210:319] ,
sram_blwl_blb[210:319] );
//----- END call module Switch blocks [1][0] -----

//----- BEGIN call module Switch blocks [1][1] -----
sb_1__1_ sb_1__1__0_ (
//----- BEGIN Global ports -----
zin[0:0],
clk[0:0],
Reset[0:0],
Set[0:0]
//----- END Global ports -----
,
//----- top side channel ports-----

//----- top side inputs: CLB output pins -----

//----- right side channel ports-----

//----- right side inputs: CLB output pins -----

//----- bottom side channel ports-----
chany_1__1__in_0_, chany_1__1__out_1_, chany_1__1__in_2_, chany_1__1__out_3_, chany_1__1__in_4_, chany_1__1__out_5_, chany_1__1__in_6_, chany_1__1__out_7_, chany_1__1__in_8_, chany_1__1__out_9_, chany_1__1__in_10_, chany_1__1__out_11_, chany_1__1__in_12_, chany_1__1__out_13_, chany_1__1__in_14_, chany_1__1__out_15_, chany_1__1__in_16_, chany_1__1__out_17_, chany_1__1__in_18_, chany_1__1__out_19_, chany_1__1__in_20_, chany_1__1__out_21_, chany_1__1__in_22_, chany_1__1__out_23_, chany_1__1__in_24_, chany_1__1__out_25_, chany_1__1__in_26_, chany_1__1__out_27_, chany_1__1__in_28_, chany_1__1__out_29_, chany_1__1__in_30_, chany_1__1__out_31_, chany_1__1__in_32_, chany_1__1__out_33_, chany_1__1__in_34_, chany_1__1__out_35_, chany_1__1__in_36_, chany_1__1__out_37_, chany_1__1__in_38_, chany_1__1__out_39_, chany_1__1__in_40_, chany_1__1__out_41_, chany_1__1__in_42_, chany_1__1__out_43_, chany_1__1__in_44_, chany_1__1__out_45_, chany_1__1__in_46_, chany_1__1__out_47_, chany_1__1__in_48_, chany_1__1__out_49_, chany_1__1__in_50_, chany_1__1__out_51_, chany_1__1__in_52_, chany_1__1__out_53_, chany_1__1__in_54_, chany_1__1__out_55_, chany_1__1__in_56_, chany_1__1__out_57_, chany_1__1__in_58_, chany_1__1__out_59_, chany_1__1__in_60_, chany_1__1__out_61_, chany_1__1__in_62_, chany_1__1__out_63_, chany_1__1__in_64_, chany_1__1__out_65_, chany_1__1__in_66_, chany_1__1__out_67_, chany_1__1__in_68_, chany_1__1__out_69_, chany_1__1__in_70_, chany_1__1__out_71_, chany_1__1__in_72_, chany_1__1__out_73_, chany_1__1__in_74_, chany_1__1__out_75_, chany_1__1__in_76_, chany_1__1__out_77_, chany_1__1__in_78_, chany_1__1__out_79_, chany_1__1__in_80_, chany_1__1__out_81_, chany_1__1__in_82_, chany_1__1__out_83_, chany_1__1__in_84_, chany_1__1__out_85_, chany_1__1__in_86_, chany_1__1__out_87_, chany_1__1__in_88_, chany_1__1__out_89_, chany_1__1__in_90_, chany_1__1__out_91_, chany_1__1__in_92_, chany_1__1__out_93_, chany_1__1__in_94_, chany_1__1__out_95_, chany_1__1__in_96_, chany_1__1__out_97_, chany_1__1__in_98_, chany_1__1__out_99_, 
//----- bottom side inputs: CLB output pins -----
 grid_2__1__pin_0__3__1_, grid_2__1__pin_0__3__3_, grid_2__1__pin_0__3__5_, grid_2__1__pin_0__3__7_, grid_2__1__pin_0__3__9_, grid_2__1__pin_0__3__11_, grid_2__1__pin_0__3__13_, grid_2__1__pin_0__3__15_, grid_1__1__pin_0__1__41_, grid_1__1__pin_0__1__45_, grid_1__1__pin_0__1__49_,
//----- left side channel ports-----
chanx_1__1__in_0_, chanx_1__1__out_1_, chanx_1__1__in_2_, chanx_1__1__out_3_, chanx_1__1__in_4_, chanx_1__1__out_5_, chanx_1__1__in_6_, chanx_1__1__out_7_, chanx_1__1__in_8_, chanx_1__1__out_9_, chanx_1__1__in_10_, chanx_1__1__out_11_, chanx_1__1__in_12_, chanx_1__1__out_13_, chanx_1__1__in_14_, chanx_1__1__out_15_, chanx_1__1__in_16_, chanx_1__1__out_17_, chanx_1__1__in_18_, chanx_1__1__out_19_, chanx_1__1__in_20_, chanx_1__1__out_21_, chanx_1__1__in_22_, chanx_1__1__out_23_, chanx_1__1__in_24_, chanx_1__1__out_25_, chanx_1__1__in_26_, chanx_1__1__out_27_, chanx_1__1__in_28_, chanx_1__1__out_29_, chanx_1__1__in_30_, chanx_1__1__out_31_, chanx_1__1__in_32_, chanx_1__1__out_33_, chanx_1__1__in_34_, chanx_1__1__out_35_, chanx_1__1__in_36_, chanx_1__1__out_37_, chanx_1__1__in_38_, chanx_1__1__out_39_, chanx_1__1__in_40_, chanx_1__1__out_41_, chanx_1__1__in_42_, chanx_1__1__out_43_, chanx_1__1__in_44_, chanx_1__1__out_45_, chanx_1__1__in_46_, chanx_1__1__out_47_, chanx_1__1__in_48_, chanx_1__1__out_49_, chanx_1__1__in_50_, chanx_1__1__out_51_, chanx_1__1__in_52_, chanx_1__1__out_53_, chanx_1__1__in_54_, chanx_1__1__out_55_, chanx_1__1__in_56_, chanx_1__1__out_57_, chanx_1__1__in_58_, chanx_1__1__out_59_, chanx_1__1__in_60_, chanx_1__1__out_61_, chanx_1__1__in_62_, chanx_1__1__out_63_, chanx_1__1__in_64_, chanx_1__1__out_65_, chanx_1__1__in_66_, chanx_1__1__out_67_, chanx_1__1__in_68_, chanx_1__1__out_69_, chanx_1__1__in_70_, chanx_1__1__out_71_, chanx_1__1__in_72_, chanx_1__1__out_73_, chanx_1__1__in_74_, chanx_1__1__out_75_, chanx_1__1__in_76_, chanx_1__1__out_77_, chanx_1__1__in_78_, chanx_1__1__out_79_, chanx_1__1__in_80_, chanx_1__1__out_81_, chanx_1__1__in_82_, chanx_1__1__out_83_, chanx_1__1__in_84_, chanx_1__1__out_85_, chanx_1__1__in_86_, chanx_1__1__out_87_, chanx_1__1__in_88_, chanx_1__1__out_89_, chanx_1__1__in_90_, chanx_1__1__out_91_, chanx_1__1__in_92_, chanx_1__1__out_93_, chanx_1__1__in_94_, chanx_1__1__out_95_, chanx_1__1__in_96_, chanx_1__1__out_97_, chanx_1__1__in_98_, chanx_1__1__out_99_, 
//----- left side inputs: CLB output pins -----
 grid_1__2__pin_0__2__1_, grid_1__2__pin_0__2__3_, grid_1__2__pin_0__2__5_, grid_1__2__pin_0__2__7_, grid_1__2__pin_0__2__9_, grid_1__2__pin_0__2__11_, grid_1__2__pin_0__2__13_, grid_1__2__pin_0__2__15_, grid_1__1__pin_0__0__40_, grid_1__1__pin_0__0__44_, grid_1__1__pin_0__0__48_,
sram_blwl_bl[320:439] ,
sram_blwl_wl[320:439] ,
sram_blwl_blb[320:439] );
//----- END call module Switch blocks [1][1] -----

//----- BEGIN CLB to CLB Direct Connections -----
//----- END CLB to CLB Direct Connections -----
//----- BEGIN call decoders for memory bank controller -----
bl_decoder6to52 mem_bank_bl_decoder (en_bl, addr_bl[5:0], data_in, bl_bus[0:51]);
wl_decoder6to52 mem_bank_wl_decoder (en_wl, addr_wl[5:0], wl_bus[0:51]);
//----- END call decoders for memory bank controller -----

endmodule
