//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: MUXes used in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//---- Structural Verilog for CMOS MUX basis module: mux_2level_tapbuf_size4_basis -----
module mux_2level_tapbuf_size4_basis (
input [0:1] in,
output out,
input [0:1] mem,
input [0:1] mem_inv);
//---- Structure-level description -----
  TGATE TGATE_0 (in[0], mem[0], mem_inv[0], out);
  TGATE TGATE_1 (in[1], mem[1], mem_inv[1], out);
endmodule
//---- END Structural Verilog CMOS MUX basis module: mux_2level_tapbuf_size4_basis -----

//----- CMOS MUX info: spice_model_name=mux_2level_tapbuf, size=4, structure: multi-level -----
module mux_2level_tapbuf_size4 (input wire [0:3] in,
output wire out,
input wire [0:3] sram,
input wire [0:3] sram_inv
);
wire [0:3] mux2_l2_in; 
wire [0:1] mux2_l1_in; 
wire [0:0] mux2_l0_in; 
mux_2level_tapbuf_size4_basis mux_basis_no0 (mux2_l2_in[0:1], mux2_l1_in[0], sram[2:3], sram_inv[2:3] );

mux_2level_tapbuf_size4_basis mux_basis_no1 (mux2_l2_in[2:3], mux2_l1_in[1], sram[2:3], sram_inv[2:3] );

mux_2level_tapbuf_size4_basis mux_basis_no2 (mux2_l1_in[0:1], mux2_l0_in[0], sram[0:1], sram_inv[0:1] );

INVTX1 inv0 (in[0], mux2_l2_in[0]); 
INVTX1 inv1 (in[1], mux2_l2_in[1]); 
INVTX1 inv2 (in[2], mux2_l2_in[2]); 
INVTX1 inv3 (in[3], mux2_l2_in[3]); 
tap_buf4 buf_out (mux2_l0_in[0], out );
endmodule
//----- END CMOS MUX info: spice_model_name=mux_2level_tapbuf, size=4 -----


//---- Structural Verilog for CMOS MUX basis module: lut4_size16_basis -----
module lut4_size16_basis (
input [0:1] in,
output out,
input [0:0] mem,
input [0:0] mem_inv);
//---- Structure-level description -----
  TGATE TGATE_0 (in[0], mem[0], mem_inv[0], out);
  TGATE TGATE_1 (in[1], mem_inv[0], mem[0], out);
endmodule
//---- END Structural Verilog CMOS MUX basis module: lut4_size16_basis -----

//------ CMOS MUX info: spice_model_name= lut4_MUX, size=16 -----
module lut4_mux(
input wire [0:15] in,
output wire out,
input wire [0:3] sram,
input wire [0:3] sram_inv
);
wire [0:15] mux2_l4_in; 
wire [0:7] mux2_l3_in; 
wire [0:3] mux2_l2_in; 
wire [0:1] mux2_l1_in; 
wire [0:0] mux2_l0_in; 
lut4_size16_basis mux_basis_no0 (mux2_l4_in[0:1], mux2_l3_in[0], sram[0], sram_inv[0]);
lut4_size16_basis mux_basis_no1 (mux2_l4_in[2:3], mux2_l3_in[1], sram[0], sram_inv[0]);
lut4_size16_basis mux_basis_no2 (mux2_l4_in[4:5], mux2_l3_in[2], sram[0], sram_inv[0]);
lut4_size16_basis mux_basis_no3 (mux2_l4_in[6:7], mux2_l3_in[3], sram[0], sram_inv[0]);
lut4_size16_basis mux_basis_no4 (mux2_l4_in[8:9], mux2_l3_in[4], sram[0], sram_inv[0]);
lut4_size16_basis mux_basis_no5 (mux2_l4_in[10:11], mux2_l3_in[5], sram[0], sram_inv[0]);
lut4_size16_basis mux_basis_no6 (mux2_l4_in[12:13], mux2_l3_in[6], sram[0], sram_inv[0]);
lut4_size16_basis mux_basis_no7 (mux2_l4_in[14:15], mux2_l3_in[7], sram[0], sram_inv[0]);
lut4_size16_basis mux_basis_no8 (mux2_l3_in[0:1], mux2_l2_in[0], sram[1], sram_inv[1]);
lut4_size16_basis mux_basis_no9 (mux2_l3_in[2:3], mux2_l2_in[1], sram[1], sram_inv[1]);
lut4_size16_basis mux_basis_no10 (mux2_l3_in[4:5], mux2_l2_in[2], sram[1], sram_inv[1]);
lut4_size16_basis mux_basis_no11 (mux2_l3_in[6:7], mux2_l2_in[3], sram[1], sram_inv[1]);
lut4_size16_basis mux_basis_no12 (mux2_l2_in[0:1], mux2_l1_in[0], sram[2], sram_inv[2]);
lut4_size16_basis mux_basis_no13 (mux2_l2_in[2:3], mux2_l1_in[1], sram[2], sram_inv[2]);
lut4_size16_basis mux_basis_no14 (mux2_l1_in[0:1], mux2_l0_in[0], sram[3], sram_inv[3]);
INVTX1 inv0 (in[0], mux2_l4_in[0]); 
INVTX1 inv1 (in[1], mux2_l4_in[1]); 
INVTX1 inv2 (in[2], mux2_l4_in[2]); 
INVTX1 inv3 (in[3], mux2_l4_in[3]); 
INVTX1 inv4 (in[4], mux2_l4_in[4]); 
INVTX1 inv5 (in[5], mux2_l4_in[5]); 
INVTX1 inv6 (in[6], mux2_l4_in[6]); 
INVTX1 inv7 (in[7], mux2_l4_in[7]); 
INVTX1 inv8 (in[8], mux2_l4_in[8]); 
INVTX1 inv9 (in[9], mux2_l4_in[9]); 
INVTX1 inv10 (in[10], mux2_l4_in[10]); 
INVTX1 inv11 (in[11], mux2_l4_in[11]); 
INVTX1 inv12 (in[12], mux2_l4_in[12]); 
INVTX1 inv13 (in[13], mux2_l4_in[13]); 
INVTX1 inv14 (in[14], mux2_l4_in[14]); 
INVTX1 inv15 (in[15], mux2_l4_in[15]); 
INVTX1 inv_out (mux2_l0_in[0], out );
endmodule
//----- END CMOS MUX info: spice_model_name=lut4, size=16 -----


//---- Structural Verilog for CMOS MUX basis module: mux_2level_size5_basis -----
module mux_2level_size5_basis (
input [0:2] in,
output out,
input [0:2] mem,
input [0:2] mem_inv);
//---- Structure-level description -----
  TGATE TGATE_0 (in[0], mem[0], mem_inv[0], out);
  TGATE TGATE_1 (in[1], mem[1], mem_inv[1], out);
  TGATE TGATE_2 (in[2], mem[2], mem_inv[2], out);
endmodule
//---- END Structural Verilog CMOS MUX basis module: mux_2level_size5_basis -----

//----- CMOS MUX info: spice_model_name=mux_2level, size=5, structure: multi-level -----
module mux_2level_size5 (input wire [0:4] in,
output wire out,
input wire [0:5] sram,
input wire [0:5] sram_inv
);
wire [0:2] mux2_l2_in; 
wire [0:2] mux2_l1_in; 
wire [0:0] mux2_l0_in; 
mux_2level_size5_basis mux_basis_no0 (mux2_l2_in[0:2], mux2_l1_in[0], sram[3:5], sram_inv[3:5] );

mux_2level_size5_basis mux_basis_no1 (mux2_l1_in[0:2], mux2_l0_in[0], sram[0:2], sram_inv[0:2] );

INVTX1 inv0 (in[0], mux2_l2_in[0]); 
INVTX1 inv1 (in[1], mux2_l2_in[1]); 
INVTX1 inv2 (in[2], mux2_l2_in[2]); 
INVTX1 inv3 (in[3], mux2_l1_in[1]); 
INVTX1 inv4 (in[4], mux2_l1_in[2]); 
INVTX1 inv_out (mux2_l0_in[0], out );
endmodule
//----- END CMOS MUX info: spice_model_name=mux_2level, size=5 -----


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


