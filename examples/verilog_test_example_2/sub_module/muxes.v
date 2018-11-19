//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: MUXes used in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//---- Structural Verilog for CMOS MUX basis module: mux_2level_tapbuf_size16_basis -----
module mux_2level_tapbuf_size16_basis (
input [0:3] in,
output out,
input [0:3] mem,
input [0:3] mem_inv);
//---- Structure-level description -----
  TGATE TGATE_0 (in[0], mem[0], mem_inv[0], out);
  TGATE TGATE_1 (in[1], mem[1], mem_inv[1], out);
  TGATE TGATE_2 (in[2], mem[2], mem_inv[2], out);
  TGATE TGATE_3 (in[3], mem[3], mem_inv[3], out);
endmodule
//---- END Structural Verilog CMOS MUX basis module: mux_2level_tapbuf_size16_basis -----

//----- CMOS MUX info: spice_model_name=mux_2level_tapbuf, size=16, structure: multi-level -----
module mux_2level_tapbuf_size16 (input wire [0:15] in,
output wire out,
input wire [0:7] sram,
input wire [0:7] sram_inv
);
wire [0:15] mux2_l2_in; 
wire [0:3] mux2_l1_in; 
wire [0:0] mux2_l0_in; 
mux_2level_tapbuf_size16_basis mux_basis_no0 (mux2_l2_in[0:3], mux2_l1_in[0], sram[4:7], sram_inv[4:7] );

mux_2level_tapbuf_size16_basis mux_basis_no1 (mux2_l2_in[4:7], mux2_l1_in[1], sram[4:7], sram_inv[4:7] );

mux_2level_tapbuf_size16_basis mux_basis_no2 (mux2_l2_in[8:11], mux2_l1_in[2], sram[4:7], sram_inv[4:7] );

mux_2level_tapbuf_size16_basis mux_basis_no3 (mux2_l2_in[12:15], mux2_l1_in[3], sram[4:7], sram_inv[4:7] );

mux_2level_tapbuf_size16_basis mux_basis_no4 (mux2_l1_in[0:3], mux2_l0_in[0], sram[0:3], sram_inv[0:3] );

INVTX1 inv0 (in[0], mux2_l2_in[0]); 
INVTX1 inv1 (in[1], mux2_l2_in[1]); 
INVTX1 inv2 (in[2], mux2_l2_in[2]); 
INVTX1 inv3 (in[3], mux2_l2_in[3]); 
INVTX1 inv4 (in[4], mux2_l2_in[4]); 
INVTX1 inv5 (in[5], mux2_l2_in[5]); 
INVTX1 inv6 (in[6], mux2_l2_in[6]); 
INVTX1 inv7 (in[7], mux2_l2_in[7]); 
INVTX1 inv8 (in[8], mux2_l2_in[8]); 
INVTX1 inv9 (in[9], mux2_l2_in[9]); 
INVTX1 inv10 (in[10], mux2_l2_in[10]); 
INVTX1 inv11 (in[11], mux2_l2_in[11]); 
INVTX1 inv12 (in[12], mux2_l2_in[12]); 
INVTX1 inv13 (in[13], mux2_l2_in[13]); 
INVTX1 inv14 (in[14], mux2_l2_in[14]); 
INVTX1 inv15 (in[15], mux2_l2_in[15]); 
tap_buf4 buf_out (mux2_l0_in[0], out );
endmodule
//----- END CMOS MUX info: spice_model_name=mux_2level_tapbuf, size=16 -----


//---- Structural Verilog for CMOS MUX basis module: lut6_size64_basis -----
module lut6_size64_basis (
input [0:1] in,
output out,
input [0:0] mem,
input [0:0] mem_inv);
//---- Structure-level description -----
  TGATE TGATE_0 (in[0], mem[0], mem_inv[0], out);
  TGATE TGATE_1 (in[1], mem_inv[0], mem[0], out);
endmodule
//---- END Structural Verilog CMOS MUX basis module: lut6_size64_basis -----

//------ CMOS MUX info: spice_model_name= lut6_MUX, size=64 -----
module lut6_mux(
input wire [0:63] in,
output wire out,
input wire [0:5] sram,
input wire [0:5] sram_inv
);
wire [0:63] mux2_l6_in; 
wire [0:31] mux2_l5_in; 
wire [0:15] mux2_l4_in; 
wire [0:7] mux2_l3_in; 
wire [0:3] mux2_l2_in; 
wire [0:1] mux2_l1_in; 
wire [0:0] mux2_l0_in; 
lut6_size64_basis mux_basis_no0 (mux2_l6_in[0:1], mux2_l5_in[0], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no1 (mux2_l6_in[2:3], mux2_l5_in[1], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no2 (mux2_l6_in[4:5], mux2_l5_in[2], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no3 (mux2_l6_in[6:7], mux2_l5_in[3], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no4 (mux2_l6_in[8:9], mux2_l5_in[4], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no5 (mux2_l6_in[10:11], mux2_l5_in[5], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no6 (mux2_l6_in[12:13], mux2_l5_in[6], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no7 (mux2_l6_in[14:15], mux2_l5_in[7], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no8 (mux2_l6_in[16:17], mux2_l5_in[8], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no9 (mux2_l6_in[18:19], mux2_l5_in[9], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no10 (mux2_l6_in[20:21], mux2_l5_in[10], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no11 (mux2_l6_in[22:23], mux2_l5_in[11], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no12 (mux2_l6_in[24:25], mux2_l5_in[12], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no13 (mux2_l6_in[26:27], mux2_l5_in[13], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no14 (mux2_l6_in[28:29], mux2_l5_in[14], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no15 (mux2_l6_in[30:31], mux2_l5_in[15], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no16 (mux2_l6_in[32:33], mux2_l5_in[16], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no17 (mux2_l6_in[34:35], mux2_l5_in[17], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no18 (mux2_l6_in[36:37], mux2_l5_in[18], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no19 (mux2_l6_in[38:39], mux2_l5_in[19], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no20 (mux2_l6_in[40:41], mux2_l5_in[20], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no21 (mux2_l6_in[42:43], mux2_l5_in[21], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no22 (mux2_l6_in[44:45], mux2_l5_in[22], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no23 (mux2_l6_in[46:47], mux2_l5_in[23], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no24 (mux2_l6_in[48:49], mux2_l5_in[24], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no25 (mux2_l6_in[50:51], mux2_l5_in[25], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no26 (mux2_l6_in[52:53], mux2_l5_in[26], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no27 (mux2_l6_in[54:55], mux2_l5_in[27], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no28 (mux2_l6_in[56:57], mux2_l5_in[28], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no29 (mux2_l6_in[58:59], mux2_l5_in[29], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no30 (mux2_l6_in[60:61], mux2_l5_in[30], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no31 (mux2_l6_in[62:63], mux2_l5_in[31], sram[0], sram_inv[0]);
lut6_size64_basis mux_basis_no32 (mux2_l5_in[0:1], mux2_l4_in[0], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no33 (mux2_l5_in[2:3], mux2_l4_in[1], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no34 (mux2_l5_in[4:5], mux2_l4_in[2], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no35 (mux2_l5_in[6:7], mux2_l4_in[3], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no36 (mux2_l5_in[8:9], mux2_l4_in[4], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no37 (mux2_l5_in[10:11], mux2_l4_in[5], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no38 (mux2_l5_in[12:13], mux2_l4_in[6], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no39 (mux2_l5_in[14:15], mux2_l4_in[7], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no40 (mux2_l5_in[16:17], mux2_l4_in[8], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no41 (mux2_l5_in[18:19], mux2_l4_in[9], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no42 (mux2_l5_in[20:21], mux2_l4_in[10], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no43 (mux2_l5_in[22:23], mux2_l4_in[11], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no44 (mux2_l5_in[24:25], mux2_l4_in[12], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no45 (mux2_l5_in[26:27], mux2_l4_in[13], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no46 (mux2_l5_in[28:29], mux2_l4_in[14], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no47 (mux2_l5_in[30:31], mux2_l4_in[15], sram[1], sram_inv[1]);
lut6_size64_basis mux_basis_no48 (mux2_l4_in[0:1], mux2_l3_in[0], sram[2], sram_inv[2]);
lut6_size64_basis mux_basis_no49 (mux2_l4_in[2:3], mux2_l3_in[1], sram[2], sram_inv[2]);
lut6_size64_basis mux_basis_no50 (mux2_l4_in[4:5], mux2_l3_in[2], sram[2], sram_inv[2]);
lut6_size64_basis mux_basis_no51 (mux2_l4_in[6:7], mux2_l3_in[3], sram[2], sram_inv[2]);
lut6_size64_basis mux_basis_no52 (mux2_l4_in[8:9], mux2_l3_in[4], sram[2], sram_inv[2]);
lut6_size64_basis mux_basis_no53 (mux2_l4_in[10:11], mux2_l3_in[5], sram[2], sram_inv[2]);
lut6_size64_basis mux_basis_no54 (mux2_l4_in[12:13], mux2_l3_in[6], sram[2], sram_inv[2]);
lut6_size64_basis mux_basis_no55 (mux2_l4_in[14:15], mux2_l3_in[7], sram[2], sram_inv[2]);
lut6_size64_basis mux_basis_no56 (mux2_l3_in[0:1], mux2_l2_in[0], sram[3], sram_inv[3]);
lut6_size64_basis mux_basis_no57 (mux2_l3_in[2:3], mux2_l2_in[1], sram[3], sram_inv[3]);
lut6_size64_basis mux_basis_no58 (mux2_l3_in[4:5], mux2_l2_in[2], sram[3], sram_inv[3]);
lut6_size64_basis mux_basis_no59 (mux2_l3_in[6:7], mux2_l2_in[3], sram[3], sram_inv[3]);
lut6_size64_basis mux_basis_no60 (mux2_l2_in[0:1], mux2_l1_in[0], sram[4], sram_inv[4]);
lut6_size64_basis mux_basis_no61 (mux2_l2_in[2:3], mux2_l1_in[1], sram[4], sram_inv[4]);
lut6_size64_basis mux_basis_no62 (mux2_l1_in[0:1], mux2_l0_in[0], sram[5], sram_inv[5]);
INVTX1 inv0 (in[0], mux2_l6_in[0]); 
INVTX1 inv1 (in[1], mux2_l6_in[1]); 
INVTX1 inv2 (in[2], mux2_l6_in[2]); 
INVTX1 inv3 (in[3], mux2_l6_in[3]); 
INVTX1 inv4 (in[4], mux2_l6_in[4]); 
INVTX1 inv5 (in[5], mux2_l6_in[5]); 
INVTX1 inv6 (in[6], mux2_l6_in[6]); 
INVTX1 inv7 (in[7], mux2_l6_in[7]); 
INVTX1 inv8 (in[8], mux2_l6_in[8]); 
INVTX1 inv9 (in[9], mux2_l6_in[9]); 
INVTX1 inv10 (in[10], mux2_l6_in[10]); 
INVTX1 inv11 (in[11], mux2_l6_in[11]); 
INVTX1 inv12 (in[12], mux2_l6_in[12]); 
INVTX1 inv13 (in[13], mux2_l6_in[13]); 
INVTX1 inv14 (in[14], mux2_l6_in[14]); 
INVTX1 inv15 (in[15], mux2_l6_in[15]); 
INVTX1 inv16 (in[16], mux2_l6_in[16]); 
INVTX1 inv17 (in[17], mux2_l6_in[17]); 
INVTX1 inv18 (in[18], mux2_l6_in[18]); 
INVTX1 inv19 (in[19], mux2_l6_in[19]); 
INVTX1 inv20 (in[20], mux2_l6_in[20]); 
INVTX1 inv21 (in[21], mux2_l6_in[21]); 
INVTX1 inv22 (in[22], mux2_l6_in[22]); 
INVTX1 inv23 (in[23], mux2_l6_in[23]); 
INVTX1 inv24 (in[24], mux2_l6_in[24]); 
INVTX1 inv25 (in[25], mux2_l6_in[25]); 
INVTX1 inv26 (in[26], mux2_l6_in[26]); 
INVTX1 inv27 (in[27], mux2_l6_in[27]); 
INVTX1 inv28 (in[28], mux2_l6_in[28]); 
INVTX1 inv29 (in[29], mux2_l6_in[29]); 
INVTX1 inv30 (in[30], mux2_l6_in[30]); 
INVTX1 inv31 (in[31], mux2_l6_in[31]); 
INVTX1 inv32 (in[32], mux2_l6_in[32]); 
INVTX1 inv33 (in[33], mux2_l6_in[33]); 
INVTX1 inv34 (in[34], mux2_l6_in[34]); 
INVTX1 inv35 (in[35], mux2_l6_in[35]); 
INVTX1 inv36 (in[36], mux2_l6_in[36]); 
INVTX1 inv37 (in[37], mux2_l6_in[37]); 
INVTX1 inv38 (in[38], mux2_l6_in[38]); 
INVTX1 inv39 (in[39], mux2_l6_in[39]); 
INVTX1 inv40 (in[40], mux2_l6_in[40]); 
INVTX1 inv41 (in[41], mux2_l6_in[41]); 
INVTX1 inv42 (in[42], mux2_l6_in[42]); 
INVTX1 inv43 (in[43], mux2_l6_in[43]); 
INVTX1 inv44 (in[44], mux2_l6_in[44]); 
INVTX1 inv45 (in[45], mux2_l6_in[45]); 
INVTX1 inv46 (in[46], mux2_l6_in[46]); 
INVTX1 inv47 (in[47], mux2_l6_in[47]); 
INVTX1 inv48 (in[48], mux2_l6_in[48]); 
INVTX1 inv49 (in[49], mux2_l6_in[49]); 
INVTX1 inv50 (in[50], mux2_l6_in[50]); 
INVTX1 inv51 (in[51], mux2_l6_in[51]); 
INVTX1 inv52 (in[52], mux2_l6_in[52]); 
INVTX1 inv53 (in[53], mux2_l6_in[53]); 
INVTX1 inv54 (in[54], mux2_l6_in[54]); 
INVTX1 inv55 (in[55], mux2_l6_in[55]); 
INVTX1 inv56 (in[56], mux2_l6_in[56]); 
INVTX1 inv57 (in[57], mux2_l6_in[57]); 
INVTX1 inv58 (in[58], mux2_l6_in[58]); 
INVTX1 inv59 (in[59], mux2_l6_in[59]); 
INVTX1 inv60 (in[60], mux2_l6_in[60]); 
INVTX1 inv61 (in[61], mux2_l6_in[61]); 
INVTX1 inv62 (in[62], mux2_l6_in[62]); 
INVTX1 inv63 (in[63], mux2_l6_in[63]); 
INVTX1 inv_out (mux2_l0_in[0], out );
endmodule
//----- END CMOS MUX info: spice_model_name=lut6, size=64 -----


//---- Structural Verilog for CMOS MUX basis module: mux_2level_size50_basis -----
module mux_2level_size50_basis (
input [0:7] in,
output out,
input [0:7] mem,
input [0:7] mem_inv);
//---- Structure-level description -----
  TGATE TGATE_0 (in[0], mem[0], mem_inv[0], out);
  TGATE TGATE_1 (in[1], mem[1], mem_inv[1], out);
  TGATE TGATE_2 (in[2], mem[2], mem_inv[2], out);
  TGATE TGATE_3 (in[3], mem[3], mem_inv[3], out);
  TGATE TGATE_4 (in[4], mem[4], mem_inv[4], out);
  TGATE TGATE_5 (in[5], mem[5], mem_inv[5], out);
  TGATE TGATE_6 (in[6], mem[6], mem_inv[6], out);
  TGATE TGATE_7 (in[7], mem[7], mem_inv[7], out);
endmodule
//---- END Structural Verilog CMOS MUX basis module: mux_2level_size50_basis -----

//----- CMOS MUX info: spice_model_name=mux_2level, size=50, structure: multi-level -----
module mux_2level_size50 (input wire [0:49] in,
output wire out,
input wire [0:15] sram,
input wire [0:15] sram_inv
);
wire [0:47] mux2_l2_in; 
wire [0:7] mux2_l1_in; 
wire [0:0] mux2_l0_in; 
mux_2level_size50_basis mux_basis_no0 (mux2_l2_in[0:7], mux2_l1_in[0], sram[8:15], sram_inv[8:15] );

mux_2level_size50_basis mux_basis_no1 (mux2_l2_in[8:15], mux2_l1_in[1], sram[8:15], sram_inv[8:15] );

mux_2level_size50_basis mux_basis_no2 (mux2_l2_in[16:23], mux2_l1_in[2], sram[8:15], sram_inv[8:15] );

mux_2level_size50_basis mux_basis_no3 (mux2_l2_in[24:31], mux2_l1_in[3], sram[8:15], sram_inv[8:15] );

mux_2level_size50_basis mux_basis_no4 (mux2_l2_in[32:39], mux2_l1_in[4], sram[8:15], sram_inv[8:15] );

mux_2level_size50_basis mux_basis_no5 (mux2_l2_in[40:47], mux2_l1_in[5], sram[8:15], sram_inv[8:15] );

mux_2level_size50_basis mux_basis_no6 (mux2_l1_in[0:7], mux2_l0_in[0], sram[0:7], sram_inv[0:7] );

INVTX1 inv0 (in[0], mux2_l2_in[0]); 
INVTX1 inv1 (in[1], mux2_l2_in[1]); 
INVTX1 inv2 (in[2], mux2_l2_in[2]); 
INVTX1 inv3 (in[3], mux2_l2_in[3]); 
INVTX1 inv4 (in[4], mux2_l2_in[4]); 
INVTX1 inv5 (in[5], mux2_l2_in[5]); 
INVTX1 inv6 (in[6], mux2_l2_in[6]); 
INVTX1 inv7 (in[7], mux2_l2_in[7]); 
INVTX1 inv8 (in[8], mux2_l2_in[8]); 
INVTX1 inv9 (in[9], mux2_l2_in[9]); 
INVTX1 inv10 (in[10], mux2_l2_in[10]); 
INVTX1 inv11 (in[11], mux2_l2_in[11]); 
INVTX1 inv12 (in[12], mux2_l2_in[12]); 
INVTX1 inv13 (in[13], mux2_l2_in[13]); 
INVTX1 inv14 (in[14], mux2_l2_in[14]); 
INVTX1 inv15 (in[15], mux2_l2_in[15]); 
INVTX1 inv16 (in[16], mux2_l2_in[16]); 
INVTX1 inv17 (in[17], mux2_l2_in[17]); 
INVTX1 inv18 (in[18], mux2_l2_in[18]); 
INVTX1 inv19 (in[19], mux2_l2_in[19]); 
INVTX1 inv20 (in[20], mux2_l2_in[20]); 
INVTX1 inv21 (in[21], mux2_l2_in[21]); 
INVTX1 inv22 (in[22], mux2_l2_in[22]); 
INVTX1 inv23 (in[23], mux2_l2_in[23]); 
INVTX1 inv24 (in[24], mux2_l2_in[24]); 
INVTX1 inv25 (in[25], mux2_l2_in[25]); 
INVTX1 inv26 (in[26], mux2_l2_in[26]); 
INVTX1 inv27 (in[27], mux2_l2_in[27]); 
INVTX1 inv28 (in[28], mux2_l2_in[28]); 
INVTX1 inv29 (in[29], mux2_l2_in[29]); 
INVTX1 inv30 (in[30], mux2_l2_in[30]); 
INVTX1 inv31 (in[31], mux2_l2_in[31]); 
INVTX1 inv32 (in[32], mux2_l2_in[32]); 
INVTX1 inv33 (in[33], mux2_l2_in[33]); 
INVTX1 inv34 (in[34], mux2_l2_in[34]); 
INVTX1 inv35 (in[35], mux2_l2_in[35]); 
INVTX1 inv36 (in[36], mux2_l2_in[36]); 
INVTX1 inv37 (in[37], mux2_l2_in[37]); 
INVTX1 inv38 (in[38], mux2_l2_in[38]); 
INVTX1 inv39 (in[39], mux2_l2_in[39]); 
INVTX1 inv40 (in[40], mux2_l2_in[40]); 
INVTX1 inv41 (in[41], mux2_l2_in[41]); 
INVTX1 inv42 (in[42], mux2_l2_in[42]); 
INVTX1 inv43 (in[43], mux2_l2_in[43]); 
INVTX1 inv44 (in[44], mux2_l2_in[44]); 
INVTX1 inv45 (in[45], mux2_l2_in[45]); 
INVTX1 inv46 (in[46], mux2_l2_in[46]); 
INVTX1 inv47 (in[47], mux2_l2_in[47]); 
INVTX1 inv48 (in[48], mux2_l1_in[6]); 
INVTX1 inv49 (in[49], mux2_l1_in[7]); 
INVTX1 inv_out (mux2_l0_in[0], out );
endmodule
//----- END CMOS MUX info: spice_model_name=mux_2level, size=50 -----


//---- Structural Verilog for CMOS MUX basis module: mux_1level_tapbuf_size3_basis -----
module mux_1level_tapbuf_size3_basis (
input [0:2] in,
output out,
input [0:2] mem,
input [0:2] mem_inv);
//---- Structure-level description -----
  TGATE TGATE_0 (in[0], mem[0], mem_inv[0], out);
  TGATE TGATE_1 (in[1], mem[1], mem_inv[1], out);
  TGATE TGATE_2 (in[2], mem[2], mem_inv[2], out);
endmodule
//---- END Structural Verilog CMOS MUX basis module: mux_1level_tapbuf_size3_basis -----

//----- CMOS MUX info: spice_model_name=mux_1level_tapbuf, size=3, structure: one-level -----
module mux_1level_tapbuf_size3 (input wire [0:2] in,
output wire out,
input wire [0:2] sram,
input wire [0:2] sram_inv
);
wire [0:2] mux2_l1_in; 
wire [0:0] mux2_l0_in; 
mux_1level_tapbuf_size3_basis mux_basis (
//----- MUX inputs -----
mux2_l1_in[0:2], mux2_l0_in[0], 
//----- SRAM ports -----
sram[0:2], sram_inv[0:2] 
);
INVTX1 inv0 (in[0], mux2_l1_in[0]); 
INVTX1 inv1 (in[1], mux2_l1_in[1]); 
INVTX1 inv2 (in[2], mux2_l1_in[2]); 
tap_buf4 buf_out (mux2_l0_in[0], out );
endmodule
//----- END CMOS MUX info: spice_model_name=mux_1level_tapbuf, size=3 -----


//---- Structural Verilog for CMOS MUX basis module: mux_1level_tapbuf_size2_basis -----
module mux_1level_tapbuf_size2_basis (
input [0:1] in,
output out,
input [0:0] mem,
input [0:0] mem_inv);
//---- Structure-level description -----
  TGATE TGATE_0 (in[0], mem[0], mem_inv[0], out);
  TGATE TGATE_1 (in[1], mem_inv[0], mem[0], out);
endmodule
//---- END Structural Verilog CMOS MUX basis module: mux_1level_tapbuf_size2_basis -----

//----- CMOS MUX info: spice_model_name=mux_1level_tapbuf, size=2, structure: one-level -----
module mux_1level_tapbuf_size2 (input wire [0:1] in,
output wire out,
input wire [0:0] sram,
input wire [0:0] sram_inv
);
wire [0:1] mux2_l1_in; 
wire [0:0] mux2_l0_in; 
mux_1level_tapbuf_size2_basis mux_basis (
//----- MUX inputs -----
mux2_l1_in[0:1], mux2_l0_in[0], 
//----- SRAM ports -----
sram[0:0], sram_inv[0:0] 
);
INVTX1 inv0 (in[0], mux2_l1_in[0]); 
INVTX1 inv1 (in[1], mux2_l1_in[1]); 
tap_buf4 buf_out (mux2_l0_in[0], out );
endmodule
//----- END CMOS MUX info: spice_model_name=mux_1level_tapbuf, size=2 -----


