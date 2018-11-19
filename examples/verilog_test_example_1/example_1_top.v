//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: FPGA Verilog Netlist for Design: example_1 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Include User-defined netlists -----
// `include "/research/ece/lnis/USERS/tang/tangxifan-eda-tools/branches/vpr7_rram/vpr/VerilogNetlists/ff.v"
// `include "/research/ece/lnis/USERS/tang/tangxifan-eda-tools/branches/vpr7_rram/vpr/VerilogNetlists/sram.v"
// `include "/research/ece/lnis/USERS/tang/tangxifan-eda-tools/branches/vpr7_rram/vpr/VerilogNetlists/io.v"
//----- Include subckt netlists: Multiplexers -----
// `include "./verilog_test_example_1/routing/muxes.v"
//----- Include subckt netlists: Wires -----
// `include "./verilog_test_example_1/routing/wires.v"
//----- Include subckt netlists: Look-Up Tables (LUTs) -----
// `include "./verilog_test_example_1/routing/luts.v"
//------ Include subckt netlists: Logic Blocks -----
// `include "./verilog_test_example_1/routing/logic_blocks.v"
//----- Include subckt netlists: Routing structures (Switch Boxes, Channels, Connection Boxes) -----
// `include "./verilog_test_example_1/routing/routing.v"
//----- Include subckt netlists: Decoders (controller for memeory bank) -----
// `include "./verilog_test_example_1/routing/decoders.v"
//----- Top-level Verilog Module -----
module example_1_top (

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
input [4:0] addr_bl , //--- Address of bit lines 
input [4:0] addr_wl  //--- Address of word lines 
);
wire [0:18] bl_bus ; //--- Array Bit lines bus 
wire [0:18] wl_bus ; //--- Array Bit lines bus 
wire [0:18] blb_bus ; //--- Inverted Array Bit lines bus 

  wire [0:360] sram_blwl_bl; //---- Normal Bit lines 
  wire [0:360] sram_blwl_wl; //---- Normal Word lines 
  wire [0:360] sram_blwl_blb; //---- Inverted Normal Bit lines 
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
  assign sram_blwl_bl[0:18] = bl_bus[0:18];
  assign sram_blwl_blb[0:18] = blb_bus[0:18];
  assign sram_blwl_bl[19:37] = bl_bus[0:18];
  assign sram_blwl_blb[19:37] = blb_bus[0:18];
  assign sram_blwl_bl[38:56] = bl_bus[0:18];
  assign sram_blwl_blb[38:56] = blb_bus[0:18];
  assign sram_blwl_bl[57:75] = bl_bus[0:18];
  assign sram_blwl_blb[57:75] = blb_bus[0:18];
  assign sram_blwl_bl[76:94] = bl_bus[0:18];
  assign sram_blwl_blb[76:94] = blb_bus[0:18];
  assign sram_blwl_bl[95:113] = bl_bus[0:18];
  assign sram_blwl_blb[95:113] = blb_bus[0:18];
  assign sram_blwl_bl[114:132] = bl_bus[0:18];
  assign sram_blwl_blb[114:132] = blb_bus[0:18];
  assign sram_blwl_bl[133:151] = bl_bus[0:18];
  assign sram_blwl_blb[133:151] = blb_bus[0:18];
  assign sram_blwl_bl[152:170] = bl_bus[0:18];
  assign sram_blwl_blb[152:170] = blb_bus[0:18];
  assign sram_blwl_bl[171:189] = bl_bus[0:18];
  assign sram_blwl_blb[171:189] = blb_bus[0:18];
  assign sram_blwl_bl[190:208] = bl_bus[0:18];
  assign sram_blwl_blb[190:208] = blb_bus[0:18];
  assign sram_blwl_bl[209:227] = bl_bus[0:18];
  assign sram_blwl_blb[209:227] = blb_bus[0:18];
  assign sram_blwl_bl[228:246] = bl_bus[0:18];
  assign sram_blwl_blb[228:246] = blb_bus[0:18];
  assign sram_blwl_bl[247:265] = bl_bus[0:18];
  assign sram_blwl_blb[247:265] = blb_bus[0:18];
  assign sram_blwl_bl[266:284] = bl_bus[0:18];
  assign sram_blwl_blb[266:284] = blb_bus[0:18];
  assign sram_blwl_bl[285:303] = bl_bus[0:18];
  assign sram_blwl_blb[285:303] = blb_bus[0:18];
  assign sram_blwl_bl[304:322] = bl_bus[0:18];
  assign sram_blwl_blb[304:322] = blb_bus[0:18];
  assign sram_blwl_bl[323:341] = bl_bus[0:18];
  assign sram_blwl_blb[323:341] = blb_bus[0:18];
  assign sram_blwl_bl[342:360] = bl_bus[0:18];
  assign sram_blwl_blb[342:360] = blb_bus[0:18];
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
    assign sram_blwl_wl[19] = wl_bus[1];
    assign sram_blwl_wl[20] = wl_bus[1];
    assign sram_blwl_wl[21] = wl_bus[1];
    assign sram_blwl_wl[22] = wl_bus[1];
    assign sram_blwl_wl[23] = wl_bus[1];
    assign sram_blwl_wl[24] = wl_bus[1];
    assign sram_blwl_wl[25] = wl_bus[1];
    assign sram_blwl_wl[26] = wl_bus[1];
    assign sram_blwl_wl[27] = wl_bus[1];
    assign sram_blwl_wl[28] = wl_bus[1];
    assign sram_blwl_wl[29] = wl_bus[1];
    assign sram_blwl_wl[30] = wl_bus[1];
    assign sram_blwl_wl[31] = wl_bus[1];
    assign sram_blwl_wl[32] = wl_bus[1];
    assign sram_blwl_wl[33] = wl_bus[1];
    assign sram_blwl_wl[34] = wl_bus[1];
    assign sram_blwl_wl[35] = wl_bus[1];
    assign sram_blwl_wl[36] = wl_bus[1];
    assign sram_blwl_wl[37] = wl_bus[1];
    assign sram_blwl_wl[38] = wl_bus[2];
    assign sram_blwl_wl[39] = wl_bus[2];
    assign sram_blwl_wl[40] = wl_bus[2];
    assign sram_blwl_wl[41] = wl_bus[2];
    assign sram_blwl_wl[42] = wl_bus[2];
    assign sram_blwl_wl[43] = wl_bus[2];
    assign sram_blwl_wl[44] = wl_bus[2];
    assign sram_blwl_wl[45] = wl_bus[2];
    assign sram_blwl_wl[46] = wl_bus[2];
    assign sram_blwl_wl[47] = wl_bus[2];
    assign sram_blwl_wl[48] = wl_bus[2];
    assign sram_blwl_wl[49] = wl_bus[2];
    assign sram_blwl_wl[50] = wl_bus[2];
    assign sram_blwl_wl[51] = wl_bus[2];
    assign sram_blwl_wl[52] = wl_bus[2];
    assign sram_blwl_wl[53] = wl_bus[2];
    assign sram_blwl_wl[54] = wl_bus[2];
    assign sram_blwl_wl[55] = wl_bus[2];
    assign sram_blwl_wl[56] = wl_bus[2];
    assign sram_blwl_wl[57] = wl_bus[3];
    assign sram_blwl_wl[58] = wl_bus[3];
    assign sram_blwl_wl[59] = wl_bus[3];
    assign sram_blwl_wl[60] = wl_bus[3];
    assign sram_blwl_wl[61] = wl_bus[3];
    assign sram_blwl_wl[62] = wl_bus[3];
    assign sram_blwl_wl[63] = wl_bus[3];
    assign sram_blwl_wl[64] = wl_bus[3];
    assign sram_blwl_wl[65] = wl_bus[3];
    assign sram_blwl_wl[66] = wl_bus[3];
    assign sram_blwl_wl[67] = wl_bus[3];
    assign sram_blwl_wl[68] = wl_bus[3];
    assign sram_blwl_wl[69] = wl_bus[3];
    assign sram_blwl_wl[70] = wl_bus[3];
    assign sram_blwl_wl[71] = wl_bus[3];
    assign sram_blwl_wl[72] = wl_bus[3];
    assign sram_blwl_wl[73] = wl_bus[3];
    assign sram_blwl_wl[74] = wl_bus[3];
    assign sram_blwl_wl[75] = wl_bus[3];
    assign sram_blwl_wl[76] = wl_bus[4];
    assign sram_blwl_wl[77] = wl_bus[4];
    assign sram_blwl_wl[78] = wl_bus[4];
    assign sram_blwl_wl[79] = wl_bus[4];
    assign sram_blwl_wl[80] = wl_bus[4];
    assign sram_blwl_wl[81] = wl_bus[4];
    assign sram_blwl_wl[82] = wl_bus[4];
    assign sram_blwl_wl[83] = wl_bus[4];
    assign sram_blwl_wl[84] = wl_bus[4];
    assign sram_blwl_wl[85] = wl_bus[4];
    assign sram_blwl_wl[86] = wl_bus[4];
    assign sram_blwl_wl[87] = wl_bus[4];
    assign sram_blwl_wl[88] = wl_bus[4];
    assign sram_blwl_wl[89] = wl_bus[4];
    assign sram_blwl_wl[90] = wl_bus[4];
    assign sram_blwl_wl[91] = wl_bus[4];
    assign sram_blwl_wl[92] = wl_bus[4];
    assign sram_blwl_wl[93] = wl_bus[4];
    assign sram_blwl_wl[94] = wl_bus[4];
    assign sram_blwl_wl[95] = wl_bus[5];
    assign sram_blwl_wl[96] = wl_bus[5];
    assign sram_blwl_wl[97] = wl_bus[5];
    assign sram_blwl_wl[98] = wl_bus[5];
    assign sram_blwl_wl[99] = wl_bus[5];
    assign sram_blwl_wl[100] = wl_bus[5];
    assign sram_blwl_wl[101] = wl_bus[5];
    assign sram_blwl_wl[102] = wl_bus[5];
    assign sram_blwl_wl[103] = wl_bus[5];
    assign sram_blwl_wl[104] = wl_bus[5];
    assign sram_blwl_wl[105] = wl_bus[5];
    assign sram_blwl_wl[106] = wl_bus[5];
    assign sram_blwl_wl[107] = wl_bus[5];
    assign sram_blwl_wl[108] = wl_bus[5];
    assign sram_blwl_wl[109] = wl_bus[5];
    assign sram_blwl_wl[110] = wl_bus[5];
    assign sram_blwl_wl[111] = wl_bus[5];
    assign sram_blwl_wl[112] = wl_bus[5];
    assign sram_blwl_wl[113] = wl_bus[5];
    assign sram_blwl_wl[114] = wl_bus[6];
    assign sram_blwl_wl[115] = wl_bus[6];
    assign sram_blwl_wl[116] = wl_bus[6];
    assign sram_blwl_wl[117] = wl_bus[6];
    assign sram_blwl_wl[118] = wl_bus[6];
    assign sram_blwl_wl[119] = wl_bus[6];
    assign sram_blwl_wl[120] = wl_bus[6];
    assign sram_blwl_wl[121] = wl_bus[6];
    assign sram_blwl_wl[122] = wl_bus[6];
    assign sram_blwl_wl[123] = wl_bus[6];
    assign sram_blwl_wl[124] = wl_bus[6];
    assign sram_blwl_wl[125] = wl_bus[6];
    assign sram_blwl_wl[126] = wl_bus[6];
    assign sram_blwl_wl[127] = wl_bus[6];
    assign sram_blwl_wl[128] = wl_bus[6];
    assign sram_blwl_wl[129] = wl_bus[6];
    assign sram_blwl_wl[130] = wl_bus[6];
    assign sram_blwl_wl[131] = wl_bus[6];
    assign sram_blwl_wl[132] = wl_bus[6];
    assign sram_blwl_wl[133] = wl_bus[7];
    assign sram_blwl_wl[134] = wl_bus[7];
    assign sram_blwl_wl[135] = wl_bus[7];
    assign sram_blwl_wl[136] = wl_bus[7];
    assign sram_blwl_wl[137] = wl_bus[7];
    assign sram_blwl_wl[138] = wl_bus[7];
    assign sram_blwl_wl[139] = wl_bus[7];
    assign sram_blwl_wl[140] = wl_bus[7];
    assign sram_blwl_wl[141] = wl_bus[7];
    assign sram_blwl_wl[142] = wl_bus[7];
    assign sram_blwl_wl[143] = wl_bus[7];
    assign sram_blwl_wl[144] = wl_bus[7];
    assign sram_blwl_wl[145] = wl_bus[7];
    assign sram_blwl_wl[146] = wl_bus[7];
    assign sram_blwl_wl[147] = wl_bus[7];
    assign sram_blwl_wl[148] = wl_bus[7];
    assign sram_blwl_wl[149] = wl_bus[7];
    assign sram_blwl_wl[150] = wl_bus[7];
    assign sram_blwl_wl[151] = wl_bus[7];
    assign sram_blwl_wl[152] = wl_bus[8];
    assign sram_blwl_wl[153] = wl_bus[8];
    assign sram_blwl_wl[154] = wl_bus[8];
    assign sram_blwl_wl[155] = wl_bus[8];
    assign sram_blwl_wl[156] = wl_bus[8];
    assign sram_blwl_wl[157] = wl_bus[8];
    assign sram_blwl_wl[158] = wl_bus[8];
    assign sram_blwl_wl[159] = wl_bus[8];
    assign sram_blwl_wl[160] = wl_bus[8];
    assign sram_blwl_wl[161] = wl_bus[8];
    assign sram_blwl_wl[162] = wl_bus[8];
    assign sram_blwl_wl[163] = wl_bus[8];
    assign sram_blwl_wl[164] = wl_bus[8];
    assign sram_blwl_wl[165] = wl_bus[8];
    assign sram_blwl_wl[166] = wl_bus[8];
    assign sram_blwl_wl[167] = wl_bus[8];
    assign sram_blwl_wl[168] = wl_bus[8];
    assign sram_blwl_wl[169] = wl_bus[8];
    assign sram_blwl_wl[170] = wl_bus[8];
    assign sram_blwl_wl[171] = wl_bus[9];
    assign sram_blwl_wl[172] = wl_bus[9];
    assign sram_blwl_wl[173] = wl_bus[9];
    assign sram_blwl_wl[174] = wl_bus[9];
    assign sram_blwl_wl[175] = wl_bus[9];
    assign sram_blwl_wl[176] = wl_bus[9];
    assign sram_blwl_wl[177] = wl_bus[9];
    assign sram_blwl_wl[178] = wl_bus[9];
    assign sram_blwl_wl[179] = wl_bus[9];
    assign sram_blwl_wl[180] = wl_bus[9];
    assign sram_blwl_wl[181] = wl_bus[9];
    assign sram_blwl_wl[182] = wl_bus[9];
    assign sram_blwl_wl[183] = wl_bus[9];
    assign sram_blwl_wl[184] = wl_bus[9];
    assign sram_blwl_wl[185] = wl_bus[9];
    assign sram_blwl_wl[186] = wl_bus[9];
    assign sram_blwl_wl[187] = wl_bus[9];
    assign sram_blwl_wl[188] = wl_bus[9];
    assign sram_blwl_wl[189] = wl_bus[9];
    assign sram_blwl_wl[190] = wl_bus[10];
    assign sram_blwl_wl[191] = wl_bus[10];
    assign sram_blwl_wl[192] = wl_bus[10];
    assign sram_blwl_wl[193] = wl_bus[10];
    assign sram_blwl_wl[194] = wl_bus[10];
    assign sram_blwl_wl[195] = wl_bus[10];
    assign sram_blwl_wl[196] = wl_bus[10];
    assign sram_blwl_wl[197] = wl_bus[10];
    assign sram_blwl_wl[198] = wl_bus[10];
    assign sram_blwl_wl[199] = wl_bus[10];
    assign sram_blwl_wl[200] = wl_bus[10];
    assign sram_blwl_wl[201] = wl_bus[10];
    assign sram_blwl_wl[202] = wl_bus[10];
    assign sram_blwl_wl[203] = wl_bus[10];
    assign sram_blwl_wl[204] = wl_bus[10];
    assign sram_blwl_wl[205] = wl_bus[10];
    assign sram_blwl_wl[206] = wl_bus[10];
    assign sram_blwl_wl[207] = wl_bus[10];
    assign sram_blwl_wl[208] = wl_bus[10];
    assign sram_blwl_wl[209] = wl_bus[11];
    assign sram_blwl_wl[210] = wl_bus[11];
    assign sram_blwl_wl[211] = wl_bus[11];
    assign sram_blwl_wl[212] = wl_bus[11];
    assign sram_blwl_wl[213] = wl_bus[11];
    assign sram_blwl_wl[214] = wl_bus[11];
    assign sram_blwl_wl[215] = wl_bus[11];
    assign sram_blwl_wl[216] = wl_bus[11];
    assign sram_blwl_wl[217] = wl_bus[11];
    assign sram_blwl_wl[218] = wl_bus[11];
    assign sram_blwl_wl[219] = wl_bus[11];
    assign sram_blwl_wl[220] = wl_bus[11];
    assign sram_blwl_wl[221] = wl_bus[11];
    assign sram_blwl_wl[222] = wl_bus[11];
    assign sram_blwl_wl[223] = wl_bus[11];
    assign sram_blwl_wl[224] = wl_bus[11];
    assign sram_blwl_wl[225] = wl_bus[11];
    assign sram_blwl_wl[226] = wl_bus[11];
    assign sram_blwl_wl[227] = wl_bus[11];
    assign sram_blwl_wl[228] = wl_bus[12];
    assign sram_blwl_wl[229] = wl_bus[12];
    assign sram_blwl_wl[230] = wl_bus[12];
    assign sram_blwl_wl[231] = wl_bus[12];
    assign sram_blwl_wl[232] = wl_bus[12];
    assign sram_blwl_wl[233] = wl_bus[12];
    assign sram_blwl_wl[234] = wl_bus[12];
    assign sram_blwl_wl[235] = wl_bus[12];
    assign sram_blwl_wl[236] = wl_bus[12];
    assign sram_blwl_wl[237] = wl_bus[12];
    assign sram_blwl_wl[238] = wl_bus[12];
    assign sram_blwl_wl[239] = wl_bus[12];
    assign sram_blwl_wl[240] = wl_bus[12];
    assign sram_blwl_wl[241] = wl_bus[12];
    assign sram_blwl_wl[242] = wl_bus[12];
    assign sram_blwl_wl[243] = wl_bus[12];
    assign sram_blwl_wl[244] = wl_bus[12];
    assign sram_blwl_wl[245] = wl_bus[12];
    assign sram_blwl_wl[246] = wl_bus[12];
    assign sram_blwl_wl[247] = wl_bus[13];
    assign sram_blwl_wl[248] = wl_bus[13];
    assign sram_blwl_wl[249] = wl_bus[13];
    assign sram_blwl_wl[250] = wl_bus[13];
    assign sram_blwl_wl[251] = wl_bus[13];
    assign sram_blwl_wl[252] = wl_bus[13];
    assign sram_blwl_wl[253] = wl_bus[13];
    assign sram_blwl_wl[254] = wl_bus[13];
    assign sram_blwl_wl[255] = wl_bus[13];
    assign sram_blwl_wl[256] = wl_bus[13];
    assign sram_blwl_wl[257] = wl_bus[13];
    assign sram_blwl_wl[258] = wl_bus[13];
    assign sram_blwl_wl[259] = wl_bus[13];
    assign sram_blwl_wl[260] = wl_bus[13];
    assign sram_blwl_wl[261] = wl_bus[13];
    assign sram_blwl_wl[262] = wl_bus[13];
    assign sram_blwl_wl[263] = wl_bus[13];
    assign sram_blwl_wl[264] = wl_bus[13];
    assign sram_blwl_wl[265] = wl_bus[13];
    assign sram_blwl_wl[266] = wl_bus[14];
    assign sram_blwl_wl[267] = wl_bus[14];
    assign sram_blwl_wl[268] = wl_bus[14];
    assign sram_blwl_wl[269] = wl_bus[14];
    assign sram_blwl_wl[270] = wl_bus[14];
    assign sram_blwl_wl[271] = wl_bus[14];
    assign sram_blwl_wl[272] = wl_bus[14];
    assign sram_blwl_wl[273] = wl_bus[14];
    assign sram_blwl_wl[274] = wl_bus[14];
    assign sram_blwl_wl[275] = wl_bus[14];
    assign sram_blwl_wl[276] = wl_bus[14];
    assign sram_blwl_wl[277] = wl_bus[14];
    assign sram_blwl_wl[278] = wl_bus[14];
    assign sram_blwl_wl[279] = wl_bus[14];
    assign sram_blwl_wl[280] = wl_bus[14];
    assign sram_blwl_wl[281] = wl_bus[14];
    assign sram_blwl_wl[282] = wl_bus[14];
    assign sram_blwl_wl[283] = wl_bus[14];
    assign sram_blwl_wl[284] = wl_bus[14];
    assign sram_blwl_wl[285] = wl_bus[15];
    assign sram_blwl_wl[286] = wl_bus[15];
    assign sram_blwl_wl[287] = wl_bus[15];
    assign sram_blwl_wl[288] = wl_bus[15];
    assign sram_blwl_wl[289] = wl_bus[15];
    assign sram_blwl_wl[290] = wl_bus[15];
    assign sram_blwl_wl[291] = wl_bus[15];
    assign sram_blwl_wl[292] = wl_bus[15];
    assign sram_blwl_wl[293] = wl_bus[15];
    assign sram_blwl_wl[294] = wl_bus[15];
    assign sram_blwl_wl[295] = wl_bus[15];
    assign sram_blwl_wl[296] = wl_bus[15];
    assign sram_blwl_wl[297] = wl_bus[15];
    assign sram_blwl_wl[298] = wl_bus[15];
    assign sram_blwl_wl[299] = wl_bus[15];
    assign sram_blwl_wl[300] = wl_bus[15];
    assign sram_blwl_wl[301] = wl_bus[15];
    assign sram_blwl_wl[302] = wl_bus[15];
    assign sram_blwl_wl[303] = wl_bus[15];
    assign sram_blwl_wl[304] = wl_bus[16];
    assign sram_blwl_wl[305] = wl_bus[16];
    assign sram_blwl_wl[306] = wl_bus[16];
    assign sram_blwl_wl[307] = wl_bus[16];
    assign sram_blwl_wl[308] = wl_bus[16];
    assign sram_blwl_wl[309] = wl_bus[16];
    assign sram_blwl_wl[310] = wl_bus[16];
    assign sram_blwl_wl[311] = wl_bus[16];
    assign sram_blwl_wl[312] = wl_bus[16];
    assign sram_blwl_wl[313] = wl_bus[16];
    assign sram_blwl_wl[314] = wl_bus[16];
    assign sram_blwl_wl[315] = wl_bus[16];
    assign sram_blwl_wl[316] = wl_bus[16];
    assign sram_blwl_wl[317] = wl_bus[16];
    assign sram_blwl_wl[318] = wl_bus[16];
    assign sram_blwl_wl[319] = wl_bus[16];
    assign sram_blwl_wl[320] = wl_bus[16];
    assign sram_blwl_wl[321] = wl_bus[16];
    assign sram_blwl_wl[322] = wl_bus[16];
    assign sram_blwl_wl[323] = wl_bus[17];
    assign sram_blwl_wl[324] = wl_bus[17];
    assign sram_blwl_wl[325] = wl_bus[17];
    assign sram_blwl_wl[326] = wl_bus[17];
    assign sram_blwl_wl[327] = wl_bus[17];
    assign sram_blwl_wl[328] = wl_bus[17];
    assign sram_blwl_wl[329] = wl_bus[17];
    assign sram_blwl_wl[330] = wl_bus[17];
    assign sram_blwl_wl[331] = wl_bus[17];
    assign sram_blwl_wl[332] = wl_bus[17];
    assign sram_blwl_wl[333] = wl_bus[17];
    assign sram_blwl_wl[334] = wl_bus[17];
    assign sram_blwl_wl[335] = wl_bus[17];
    assign sram_blwl_wl[336] = wl_bus[17];
    assign sram_blwl_wl[337] = wl_bus[17];
    assign sram_blwl_wl[338] = wl_bus[17];
    assign sram_blwl_wl[339] = wl_bus[17];
    assign sram_blwl_wl[340] = wl_bus[17];
    assign sram_blwl_wl[341] = wl_bus[17];
    assign sram_blwl_wl[342] = wl_bus[18];
    assign sram_blwl_wl[343] = wl_bus[18];
    assign sram_blwl_wl[344] = wl_bus[18];
    assign sram_blwl_wl[345] = wl_bus[18];
    assign sram_blwl_wl[346] = wl_bus[18];
    assign sram_blwl_wl[347] = wl_bus[18];
    assign sram_blwl_wl[348] = wl_bus[18];
    assign sram_blwl_wl[349] = wl_bus[18];
    assign sram_blwl_wl[350] = wl_bus[18];
    assign sram_blwl_wl[351] = wl_bus[18];
    assign sram_blwl_wl[352] = wl_bus[18];
    assign sram_blwl_wl[353] = wl_bus[18];
    assign sram_blwl_wl[354] = wl_bus[18];
    assign sram_blwl_wl[355] = wl_bus[18];
    assign sram_blwl_wl[356] = wl_bus[18];
    assign sram_blwl_wl[357] = wl_bus[18];
    assign sram_blwl_wl[358] = wl_bus[18];
    assign sram_blwl_wl[359] = wl_bus[18];
    assign sram_blwl_wl[360] = wl_bus[18];
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
 grid_1__1__pin_0__1__1_,
 grid_1__1__pin_0__1__5_,
 grid_1__1__pin_0__2__2_,
 grid_1__1__pin_0__3__3_,
sram_blwl_bl[288:328] ,
sram_blwl_wl[288:328] ,
sram_blwl_blb[288:328] );
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
sram_blwl_bl[329:336] ,
sram_blwl_wl[329:336] ,
sram_blwl_blb[329:336] );
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
sram_blwl_bl[337:344] ,
sram_blwl_wl[337:344] ,
sram_blwl_blb[337:344] );
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
sram_blwl_bl[345:352] ,
sram_blwl_wl[345:352] ,
sram_blwl_blb[345:352] );
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
sram_blwl_bl[353:360] ,
sram_blwl_wl[353:360] ,
sram_blwl_blb[353:360] );
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
chanx_1__0__midout_29_ 
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
chanx_1__1__midout_29_ 
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
chany_0__1__midout_29_ 
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
chany_1__1__midout_29_ 
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
//----- top side outputs: CLB input pins -----
 grid_1__1__pin_0__2__2_, 
//----- bottom side outputs: CLB input pins -----
 grid_1__0__pin_0__0__0_, 
 grid_1__0__pin_0__0__2_, 
 grid_1__0__pin_0__0__4_, 
 grid_1__0__pin_0__0__6_, 
 grid_1__0__pin_0__0__8_, 
 grid_1__0__pin_0__0__10_, 
 grid_1__0__pin_0__0__12_, 
 grid_1__0__pin_0__0__14_, 
sram_blwl_bl[144:179] ,
sram_blwl_wl[144:179] ,
sram_blwl_blb[144:179] );
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
sram_blwl_bl[180:215] ,
sram_blwl_wl[180:215] ,
sram_blwl_blb[180:215] );
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
//----- right side outputs: CLB input pins -----
 grid_1__1__pin_0__3__3_, 
//----- left side outputs: CLB input pins -----
 grid_0__1__pin_0__1__0_, 
 grid_0__1__pin_0__1__2_, 
 grid_0__1__pin_0__1__4_, 
 grid_0__1__pin_0__1__6_, 
 grid_0__1__pin_0__1__8_, 
 grid_0__1__pin_0__1__10_, 
 grid_0__1__pin_0__1__12_, 
 grid_0__1__pin_0__1__14_, 
sram_blwl_bl[216:251] ,
sram_blwl_wl[216:251] ,
sram_blwl_blb[216:251] );
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
sram_blwl_bl[252:287] ,
sram_blwl_wl[252:287] ,
sram_blwl_blb[252:287] );
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
chany_0__1__out_0_, chany_0__1__in_1_, chany_0__1__out_2_, chany_0__1__in_3_, chany_0__1__out_4_, chany_0__1__in_5_, chany_0__1__out_6_, chany_0__1__in_7_, chany_0__1__out_8_, chany_0__1__in_9_, chany_0__1__out_10_, chany_0__1__in_11_, chany_0__1__out_12_, chany_0__1__in_13_, chany_0__1__out_14_, chany_0__1__in_15_, chany_0__1__out_16_, chany_0__1__in_17_, chany_0__1__out_18_, chany_0__1__in_19_, chany_0__1__out_20_, chany_0__1__in_21_, chany_0__1__out_22_, chany_0__1__in_23_, chany_0__1__out_24_, chany_0__1__in_25_, chany_0__1__out_26_, chany_0__1__in_27_, chany_0__1__out_28_, chany_0__1__in_29_, 
//----- top side inputs: CLB output pins -----
 grid_0__1__pin_0__1__1_, grid_0__1__pin_0__1__3_, grid_0__1__pin_0__1__5_, grid_0__1__pin_0__1__7_, grid_0__1__pin_0__1__9_, grid_0__1__pin_0__1__11_, grid_0__1__pin_0__1__13_, grid_0__1__pin_0__1__15_,
//----- right side channel ports-----
chanx_1__0__out_0_, chanx_1__0__in_1_, chanx_1__0__out_2_, chanx_1__0__in_3_, chanx_1__0__out_4_, chanx_1__0__in_5_, chanx_1__0__out_6_, chanx_1__0__in_7_, chanx_1__0__out_8_, chanx_1__0__in_9_, chanx_1__0__out_10_, chanx_1__0__in_11_, chanx_1__0__out_12_, chanx_1__0__in_13_, chanx_1__0__out_14_, chanx_1__0__in_15_, chanx_1__0__out_16_, chanx_1__0__in_17_, chanx_1__0__out_18_, chanx_1__0__in_19_, chanx_1__0__out_20_, chanx_1__0__in_21_, chanx_1__0__out_22_, chanx_1__0__in_23_, chanx_1__0__out_24_, chanx_1__0__in_25_, chanx_1__0__out_26_, chanx_1__0__in_27_, chanx_1__0__out_28_, chanx_1__0__in_29_, 
//----- right side inputs: CLB output pins -----
 grid_1__0__pin_0__0__1_, grid_1__0__pin_0__0__3_, grid_1__0__pin_0__0__5_, grid_1__0__pin_0__0__7_, grid_1__0__pin_0__0__9_, grid_1__0__pin_0__0__11_, grid_1__0__pin_0__0__13_, grid_1__0__pin_0__0__15_,
//----- bottom side channel ports-----

//----- bottom side inputs: CLB output pins -----

//----- left side channel ports-----

//----- left side inputs: CLB output pins -----

sram_blwl_bl[0:33] ,
sram_blwl_wl[0:33] ,
sram_blwl_blb[0:33] );
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
chanx_1__1__out_0_, chanx_1__1__in_1_, chanx_1__1__out_2_, chanx_1__1__in_3_, chanx_1__1__out_4_, chanx_1__1__in_5_, chanx_1__1__out_6_, chanx_1__1__in_7_, chanx_1__1__out_8_, chanx_1__1__in_9_, chanx_1__1__out_10_, chanx_1__1__in_11_, chanx_1__1__out_12_, chanx_1__1__in_13_, chanx_1__1__out_14_, chanx_1__1__in_15_, chanx_1__1__out_16_, chanx_1__1__in_17_, chanx_1__1__out_18_, chanx_1__1__in_19_, chanx_1__1__out_20_, chanx_1__1__in_21_, chanx_1__1__out_22_, chanx_1__1__in_23_, chanx_1__1__out_24_, chanx_1__1__in_25_, chanx_1__1__out_26_, chanx_1__1__in_27_, chanx_1__1__out_28_, chanx_1__1__in_29_, 
//----- right side inputs: CLB output pins -----
 grid_1__2__pin_0__2__1_, grid_1__2__pin_0__2__3_, grid_1__2__pin_0__2__5_, grid_1__2__pin_0__2__7_, grid_1__2__pin_0__2__9_, grid_1__2__pin_0__2__11_, grid_1__2__pin_0__2__13_, grid_1__2__pin_0__2__15_, grid_1__1__pin_0__0__4_,
//----- bottom side channel ports-----
chany_0__1__in_0_, chany_0__1__out_1_, chany_0__1__in_2_, chany_0__1__out_3_, chany_0__1__in_4_, chany_0__1__out_5_, chany_0__1__in_6_, chany_0__1__out_7_, chany_0__1__in_8_, chany_0__1__out_9_, chany_0__1__in_10_, chany_0__1__out_11_, chany_0__1__in_12_, chany_0__1__out_13_, chany_0__1__in_14_, chany_0__1__out_15_, chany_0__1__in_16_, chany_0__1__out_17_, chany_0__1__in_18_, chany_0__1__out_19_, chany_0__1__in_20_, chany_0__1__out_21_, chany_0__1__in_22_, chany_0__1__out_23_, chany_0__1__in_24_, chany_0__1__out_25_, chany_0__1__in_26_, chany_0__1__out_27_, chany_0__1__in_28_, chany_0__1__out_29_, 
//----- bottom side inputs: CLB output pins -----
 grid_0__1__pin_0__1__1_, grid_0__1__pin_0__1__3_, grid_0__1__pin_0__1__5_, grid_0__1__pin_0__1__7_, grid_0__1__pin_0__1__9_, grid_0__1__pin_0__1__11_, grid_0__1__pin_0__1__13_, grid_0__1__pin_0__1__15_,
//----- left side channel ports-----

//----- left side inputs: CLB output pins -----

sram_blwl_bl[34:71] ,
sram_blwl_wl[34:71] ,
sram_blwl_blb[34:71] );
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
chany_1__1__out_0_, chany_1__1__in_1_, chany_1__1__out_2_, chany_1__1__in_3_, chany_1__1__out_4_, chany_1__1__in_5_, chany_1__1__out_6_, chany_1__1__in_7_, chany_1__1__out_8_, chany_1__1__in_9_, chany_1__1__out_10_, chany_1__1__in_11_, chany_1__1__out_12_, chany_1__1__in_13_, chany_1__1__out_14_, chany_1__1__in_15_, chany_1__1__out_16_, chany_1__1__in_17_, chany_1__1__out_18_, chany_1__1__in_19_, chany_1__1__out_20_, chany_1__1__in_21_, chany_1__1__out_22_, chany_1__1__in_23_, chany_1__1__out_24_, chany_1__1__in_25_, chany_1__1__out_26_, chany_1__1__in_27_, chany_1__1__out_28_, chany_1__1__in_29_, 
//----- top side inputs: CLB output pins -----
 grid_2__1__pin_0__3__1_, grid_2__1__pin_0__3__3_, grid_2__1__pin_0__3__5_, grid_2__1__pin_0__3__7_, grid_2__1__pin_0__3__9_, grid_2__1__pin_0__3__11_, grid_2__1__pin_0__3__13_, grid_2__1__pin_0__3__15_,
//----- right side channel ports-----

//----- right side inputs: CLB output pins -----

//----- bottom side channel ports-----

//----- bottom side inputs: CLB output pins -----

//----- left side channel ports-----
chanx_1__0__in_0_, chanx_1__0__out_1_, chanx_1__0__in_2_, chanx_1__0__out_3_, chanx_1__0__in_4_, chanx_1__0__out_5_, chanx_1__0__in_6_, chanx_1__0__out_7_, chanx_1__0__in_8_, chanx_1__0__out_9_, chanx_1__0__in_10_, chanx_1__0__out_11_, chanx_1__0__in_12_, chanx_1__0__out_13_, chanx_1__0__in_14_, chanx_1__0__out_15_, chanx_1__0__in_16_, chanx_1__0__out_17_, chanx_1__0__in_18_, chanx_1__0__out_19_, chanx_1__0__in_20_, chanx_1__0__out_21_, chanx_1__0__in_22_, chanx_1__0__out_23_, chanx_1__0__in_24_, chanx_1__0__out_25_, chanx_1__0__in_26_, chanx_1__0__out_27_, chanx_1__0__in_28_, chanx_1__0__out_29_, 
//----- left side inputs: CLB output pins -----
 grid_1__0__pin_0__0__1_, grid_1__0__pin_0__0__3_, grid_1__0__pin_0__0__5_, grid_1__0__pin_0__0__7_, grid_1__0__pin_0__0__9_, grid_1__0__pin_0__0__11_, grid_1__0__pin_0__0__13_, grid_1__0__pin_0__0__15_,
sram_blwl_bl[72:105] ,
sram_blwl_wl[72:105] ,
sram_blwl_blb[72:105] );
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
chany_1__1__in_0_, chany_1__1__out_1_, chany_1__1__in_2_, chany_1__1__out_3_, chany_1__1__in_4_, chany_1__1__out_5_, chany_1__1__in_6_, chany_1__1__out_7_, chany_1__1__in_8_, chany_1__1__out_9_, chany_1__1__in_10_, chany_1__1__out_11_, chany_1__1__in_12_, chany_1__1__out_13_, chany_1__1__in_14_, chany_1__1__out_15_, chany_1__1__in_16_, chany_1__1__out_17_, chany_1__1__in_18_, chany_1__1__out_19_, chany_1__1__in_20_, chany_1__1__out_21_, chany_1__1__in_22_, chany_1__1__out_23_, chany_1__1__in_24_, chany_1__1__out_25_, chany_1__1__in_26_, chany_1__1__out_27_, chany_1__1__in_28_, chany_1__1__out_29_, 
//----- bottom side inputs: CLB output pins -----
 grid_2__1__pin_0__3__1_, grid_2__1__pin_0__3__3_, grid_2__1__pin_0__3__5_, grid_2__1__pin_0__3__7_, grid_2__1__pin_0__3__9_, grid_2__1__pin_0__3__11_, grid_2__1__pin_0__3__13_, grid_2__1__pin_0__3__15_,
//----- left side channel ports-----
chanx_1__1__in_0_, chanx_1__1__out_1_, chanx_1__1__in_2_, chanx_1__1__out_3_, chanx_1__1__in_4_, chanx_1__1__out_5_, chanx_1__1__in_6_, chanx_1__1__out_7_, chanx_1__1__in_8_, chanx_1__1__out_9_, chanx_1__1__in_10_, chanx_1__1__out_11_, chanx_1__1__in_12_, chanx_1__1__out_13_, chanx_1__1__in_14_, chanx_1__1__out_15_, chanx_1__1__in_16_, chanx_1__1__out_17_, chanx_1__1__in_18_, chanx_1__1__out_19_, chanx_1__1__in_20_, chanx_1__1__out_21_, chanx_1__1__in_22_, chanx_1__1__out_23_, chanx_1__1__in_24_, chanx_1__1__out_25_, chanx_1__1__in_26_, chanx_1__1__out_27_, chanx_1__1__in_28_, chanx_1__1__out_29_, 
//----- left side inputs: CLB output pins -----
 grid_1__2__pin_0__2__1_, grid_1__2__pin_0__2__3_, grid_1__2__pin_0__2__5_, grid_1__2__pin_0__2__7_, grid_1__2__pin_0__2__9_, grid_1__2__pin_0__2__11_, grid_1__2__pin_0__2__13_, grid_1__2__pin_0__2__15_, grid_1__1__pin_0__0__4_,
sram_blwl_bl[106:143] ,
sram_blwl_wl[106:143] ,
sram_blwl_blb[106:143] );
//----- END call module Switch blocks [1][1] -----

//----- BEGIN CLB to CLB Direct Connections -----
//----- END CLB to CLB Direct Connections -----
//----- BEGIN call decoders for memory bank controller -----
bl_decoder5to19 mem_bank_bl_decoder (en_bl, addr_bl[4:0], data_in, bl_bus[0:18]);
wl_decoder5to19 mem_bank_wl_decoder (en_wl, addr_wl[4:0], wl_bus[0:18]);
//----- END call decoders for memory bank controller -----

endmodule
