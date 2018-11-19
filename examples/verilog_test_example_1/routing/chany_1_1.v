//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Routing Channel - Y direction  [1][1] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Verilog Module Channel Y [1][1] -----
module chany_1__1_ ( 

//----- BEGIN Global ports -----
input [0:0] zin,
input [0:0] clk,
input [0:0] Reset,
input [0:0] Set
//----- END Global ports -----
,
  input in0, //--- track 0 input 
  output out1, //--- track 1 output 
  input in2, //--- track 2 input 
  output out3, //--- track 3 output 
  input in4, //--- track 4 input 
  output out5, //--- track 5 output 
  input in6, //--- track 6 input 
  output out7, //--- track 7 output 
  input in8, //--- track 8 input 
  output out9, //--- track 9 output 
  input in10, //--- track 10 input 
  output out11, //--- track 11 output 
  input in12, //--- track 12 input 
  output out13, //--- track 13 output 
  input in14, //--- track 14 input 
  output out15, //--- track 15 output 
  input in16, //--- track 16 input 
  output out17, //--- track 17 output 
  input in18, //--- track 18 input 
  output out19, //--- track 19 output 
  input in20, //--- track 20 input 
  output out21, //--- track 21 output 
  input in22, //--- track 22 input 
  output out23, //--- track 23 output 
  input in24, //--- track 24 input 
  output out25, //--- track 25 output 
  input in26, //--- track 26 input 
  output out27, //--- track 27 output 
  input in28, //--- track 28 input 
  output out29, //--- track 29 output 
  output out0, //--- track 0 output
  input in1, //--- track 1 input 
  output out2, //--- track 2 output
  input in3, //--- track 3 input 
  output out4, //--- track 4 output
  input in5, //--- track 5 input 
  output out6, //--- track 6 output
  input in7, //--- track 7 input 
  output out8, //--- track 8 output
  input in9, //--- track 9 input 
  output out10, //--- track 10 output
  input in11, //--- track 11 input 
  output out12, //--- track 12 output
  input in13, //--- track 13 input 
  output out14, //--- track 14 output
  input in15, //--- track 15 input 
  output out16, //--- track 16 output
  input in17, //--- track 17 input 
  output out18, //--- track 18 output
  input in19, //--- track 19 input 
  output out20, //--- track 20 output
  input in21, //--- track 21 input 
  output out22, //--- track 22 output
  input in23, //--- track 23 input 
  output out24, //--- track 24 output
  input in25, //--- track 25 input 
  output out26, //--- track 26 output
  input in27, //--- track 27 input 
  output out28, //--- track 28 output
  input in29, //--- track 29 input 
  output mid_out0, // Middle output 0 to logic blocks 
  output mid_out1, // Middle output 1 to logic blocks 
  output mid_out2, // Middle output 2 to logic blocks 
  output mid_out3, // Middle output 3 to logic blocks 
  output mid_out4, // Middle output 4 to logic blocks 
  output mid_out5, // Middle output 5 to logic blocks 
  output mid_out6, // Middle output 6 to logic blocks 
  output mid_out7, // Middle output 7 to logic blocks 
  output mid_out8, // Middle output 8 to logic blocks 
  output mid_out9, // Middle output 9 to logic blocks 
  output mid_out10, // Middle output 10 to logic blocks 
  output mid_out11, // Middle output 11 to logic blocks 
  output mid_out12, // Middle output 12 to logic blocks 
  output mid_out13, // Middle output 13 to logic blocks 
  output mid_out14, // Middle output 14 to logic blocks 
  output mid_out15, // Middle output 15 to logic blocks 
  output mid_out16, // Middle output 16 to logic blocks 
  output mid_out17, // Middle output 17 to logic blocks 
  output mid_out18, // Middle output 18 to logic blocks 
  output mid_out19, // Middle output 19 to logic blocks 
  output mid_out20, // Middle output 20 to logic blocks 
  output mid_out21, // Middle output 21 to logic blocks 
  output mid_out22, // Middle output 22 to logic blocks 
  output mid_out23, // Middle output 23 to logic blocks 
  output mid_out24, // Middle output 24 to logic blocks 
  output mid_out25, // Middle output 25 to logic blocks 
  output mid_out26, // Middle output 26 to logic blocks 
  output mid_out27, // Middle output 27 to logic blocks 
  output mid_out28, // Middle output 28 to logic blocks 
  output mid_out29 // Middle output 29 to logic blocks 
  );
assign out0 = in0; 
assign mid_out0 = in0; 
assign out1 = in1; 
assign mid_out1 = in1; 
assign out2 = in2; 
assign mid_out2 = in2; 
assign out3 = in3; 
assign mid_out3 = in3; 
assign out4 = in4; 
assign mid_out4 = in4; 
assign out5 = in5; 
assign mid_out5 = in5; 
assign out6 = in6; 
assign mid_out6 = in6; 
assign out7 = in7; 
assign mid_out7 = in7; 
assign out8 = in8; 
assign mid_out8 = in8; 
assign out9 = in9; 
assign mid_out9 = in9; 
assign out10 = in10; 
assign mid_out10 = in10; 
assign out11 = in11; 
assign mid_out11 = in11; 
assign out12 = in12; 
assign mid_out12 = in12; 
assign out13 = in13; 
assign mid_out13 = in13; 
assign out14 = in14; 
assign mid_out14 = in14; 
assign out15 = in15; 
assign mid_out15 = in15; 
assign out16 = in16; 
assign mid_out16 = in16; 
assign out17 = in17; 
assign mid_out17 = in17; 
assign out18 = in18; 
assign mid_out18 = in18; 
assign out19 = in19; 
assign mid_out19 = in19; 
assign out20 = in20; 
assign mid_out20 = in20; 
assign out21 = in21; 
assign mid_out21 = in21; 
assign out22 = in22; 
assign mid_out22 = in22; 
assign out23 = in23; 
assign mid_out23 = in23; 
assign out24 = in24; 
assign mid_out24 = in24; 
assign out25 = in25; 
assign mid_out25 = in25; 
assign out26 = in26; 
assign mid_out26 = in26; 
assign out27 = in27; 
assign mid_out27 = in27; 
assign out28 = in28; 
assign mid_out28 = in28; 
assign out29 = in29; 
assign mid_out29 = in29; 
endmodule
//----- END Verilog Module of Channel Y [1][1] -----

