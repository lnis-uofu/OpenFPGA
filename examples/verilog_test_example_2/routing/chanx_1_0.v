//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description: Routing Channel - X direction  [1][0] in FPGA 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Verilog Module of Channel X [1][0] -----
module chanx_1__0_ ( 

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
  input in30, //--- track 30 input 
  output out31, //--- track 31 output 
  input in32, //--- track 32 input 
  output out33, //--- track 33 output 
  input in34, //--- track 34 input 
  output out35, //--- track 35 output 
  input in36, //--- track 36 input 
  output out37, //--- track 37 output 
  input in38, //--- track 38 input 
  output out39, //--- track 39 output 
  input in40, //--- track 40 input 
  output out41, //--- track 41 output 
  input in42, //--- track 42 input 
  output out43, //--- track 43 output 
  input in44, //--- track 44 input 
  output out45, //--- track 45 output 
  input in46, //--- track 46 input 
  output out47, //--- track 47 output 
  input in48, //--- track 48 input 
  output out49, //--- track 49 output 
  input in50, //--- track 50 input 
  output out51, //--- track 51 output 
  input in52, //--- track 52 input 
  output out53, //--- track 53 output 
  input in54, //--- track 54 input 
  output out55, //--- track 55 output 
  input in56, //--- track 56 input 
  output out57, //--- track 57 output 
  input in58, //--- track 58 input 
  output out59, //--- track 59 output 
  input in60, //--- track 60 input 
  output out61, //--- track 61 output 
  input in62, //--- track 62 input 
  output out63, //--- track 63 output 
  input in64, //--- track 64 input 
  output out65, //--- track 65 output 
  input in66, //--- track 66 input 
  output out67, //--- track 67 output 
  input in68, //--- track 68 input 
  output out69, //--- track 69 output 
  input in70, //--- track 70 input 
  output out71, //--- track 71 output 
  input in72, //--- track 72 input 
  output out73, //--- track 73 output 
  input in74, //--- track 74 input 
  output out75, //--- track 75 output 
  input in76, //--- track 76 input 
  output out77, //--- track 77 output 
  input in78, //--- track 78 input 
  output out79, //--- track 79 output 
  input in80, //--- track 80 input 
  output out81, //--- track 81 output 
  input in82, //--- track 82 input 
  output out83, //--- track 83 output 
  input in84, //--- track 84 input 
  output out85, //--- track 85 output 
  input in86, //--- track 86 input 
  output out87, //--- track 87 output 
  input in88, //--- track 88 input 
  output out89, //--- track 89 output 
  input in90, //--- track 90 input 
  output out91, //--- track 91 output 
  input in92, //--- track 92 input 
  output out93, //--- track 93 output 
  input in94, //--- track 94 input 
  output out95, //--- track 95 output 
  input in96, //--- track 96 input 
  output out97, //--- track 97 output 
  input in98, //--- track 98 input 
  output out99, //--- track 99 output 
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
  output out30, //--- track 30 output
  input in31, //--- track 31 input 
  output out32, //--- track 32 output
  input in33, //--- track 33 input 
  output out34, //--- track 34 output
  input in35, //--- track 35 input 
  output out36, //--- track 36 output
  input in37, //--- track 37 input 
  output out38, //--- track 38 output
  input in39, //--- track 39 input 
  output out40, //--- track 40 output
  input in41, //--- track 41 input 
  output out42, //--- track 42 output
  input in43, //--- track 43 input 
  output out44, //--- track 44 output
  input in45, //--- track 45 input 
  output out46, //--- track 46 output
  input in47, //--- track 47 input 
  output out48, //--- track 48 output
  input in49, //--- track 49 input 
  output out50, //--- track 50 output
  input in51, //--- track 51 input 
  output out52, //--- track 52 output
  input in53, //--- track 53 input 
  output out54, //--- track 54 output
  input in55, //--- track 55 input 
  output out56, //--- track 56 output
  input in57, //--- track 57 input 
  output out58, //--- track 58 output
  input in59, //--- track 59 input 
  output out60, //--- track 60 output
  input in61, //--- track 61 input 
  output out62, //--- track 62 output
  input in63, //--- track 63 input 
  output out64, //--- track 64 output
  input in65, //--- track 65 input 
  output out66, //--- track 66 output
  input in67, //--- track 67 input 
  output out68, //--- track 68 output
  input in69, //--- track 69 input 
  output out70, //--- track 70 output
  input in71, //--- track 71 input 
  output out72, //--- track 72 output
  input in73, //--- track 73 input 
  output out74, //--- track 74 output
  input in75, //--- track 75 input 
  output out76, //--- track 76 output
  input in77, //--- track 77 input 
  output out78, //--- track 78 output
  input in79, //--- track 79 input 
  output out80, //--- track 80 output
  input in81, //--- track 81 input 
  output out82, //--- track 82 output
  input in83, //--- track 83 input 
  output out84, //--- track 84 output
  input in85, //--- track 85 input 
  output out86, //--- track 86 output
  input in87, //--- track 87 input 
  output out88, //--- track 88 output
  input in89, //--- track 89 input 
  output out90, //--- track 90 output
  input in91, //--- track 91 input 
  output out92, //--- track 92 output
  input in93, //--- track 93 input 
  output out94, //--- track 94 output
  input in95, //--- track 95 input 
  output out96, //--- track 96 output
  input in97, //--- track 97 input 
  output out98, //--- track 98 output
  input in99, //--- track 99 input 
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
  output mid_out29, // Middle output 29 to logic blocks 
  output mid_out30, // Middle output 30 to logic blocks 
  output mid_out31, // Middle output 31 to logic blocks 
  output mid_out32, // Middle output 32 to logic blocks 
  output mid_out33, // Middle output 33 to logic blocks 
  output mid_out34, // Middle output 34 to logic blocks 
  output mid_out35, // Middle output 35 to logic blocks 
  output mid_out36, // Middle output 36 to logic blocks 
  output mid_out37, // Middle output 37 to logic blocks 
  output mid_out38, // Middle output 38 to logic blocks 
  output mid_out39, // Middle output 39 to logic blocks 
  output mid_out40, // Middle output 40 to logic blocks 
  output mid_out41, // Middle output 41 to logic blocks 
  output mid_out42, // Middle output 42 to logic blocks 
  output mid_out43, // Middle output 43 to logic blocks 
  output mid_out44, // Middle output 44 to logic blocks 
  output mid_out45, // Middle output 45 to logic blocks 
  output mid_out46, // Middle output 46 to logic blocks 
  output mid_out47, // Middle output 47 to logic blocks 
  output mid_out48, // Middle output 48 to logic blocks 
  output mid_out49, // Middle output 49 to logic blocks 
  output mid_out50, // Middle output 50 to logic blocks 
  output mid_out51, // Middle output 51 to logic blocks 
  output mid_out52, // Middle output 52 to logic blocks 
  output mid_out53, // Middle output 53 to logic blocks 
  output mid_out54, // Middle output 54 to logic blocks 
  output mid_out55, // Middle output 55 to logic blocks 
  output mid_out56, // Middle output 56 to logic blocks 
  output mid_out57, // Middle output 57 to logic blocks 
  output mid_out58, // Middle output 58 to logic blocks 
  output mid_out59, // Middle output 59 to logic blocks 
  output mid_out60, // Middle output 60 to logic blocks 
  output mid_out61, // Middle output 61 to logic blocks 
  output mid_out62, // Middle output 62 to logic blocks 
  output mid_out63, // Middle output 63 to logic blocks 
  output mid_out64, // Middle output 64 to logic blocks 
  output mid_out65, // Middle output 65 to logic blocks 
  output mid_out66, // Middle output 66 to logic blocks 
  output mid_out67, // Middle output 67 to logic blocks 
  output mid_out68, // Middle output 68 to logic blocks 
  output mid_out69, // Middle output 69 to logic blocks 
  output mid_out70, // Middle output 70 to logic blocks 
  output mid_out71, // Middle output 71 to logic blocks 
  output mid_out72, // Middle output 72 to logic blocks 
  output mid_out73, // Middle output 73 to logic blocks 
  output mid_out74, // Middle output 74 to logic blocks 
  output mid_out75, // Middle output 75 to logic blocks 
  output mid_out76, // Middle output 76 to logic blocks 
  output mid_out77, // Middle output 77 to logic blocks 
  output mid_out78, // Middle output 78 to logic blocks 
  output mid_out79, // Middle output 79 to logic blocks 
  output mid_out80, // Middle output 80 to logic blocks 
  output mid_out81, // Middle output 81 to logic blocks 
  output mid_out82, // Middle output 82 to logic blocks 
  output mid_out83, // Middle output 83 to logic blocks 
  output mid_out84, // Middle output 84 to logic blocks 
  output mid_out85, // Middle output 85 to logic blocks 
  output mid_out86, // Middle output 86 to logic blocks 
  output mid_out87, // Middle output 87 to logic blocks 
  output mid_out88, // Middle output 88 to logic blocks 
  output mid_out89, // Middle output 89 to logic blocks 
  output mid_out90, // Middle output 90 to logic blocks 
  output mid_out91, // Middle output 91 to logic blocks 
  output mid_out92, // Middle output 92 to logic blocks 
  output mid_out93, // Middle output 93 to logic blocks 
  output mid_out94, // Middle output 94 to logic blocks 
  output mid_out95, // Middle output 95 to logic blocks 
  output mid_out96, // Middle output 96 to logic blocks 
  output mid_out97, // Middle output 97 to logic blocks 
  output mid_out98, // Middle output 98 to logic blocks 
  output mid_out99 // Middle output 99 to logic blocks 
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
assign out30 = in30; 
assign mid_out30 = in30; 
assign out31 = in31; 
assign mid_out31 = in31; 
assign out32 = in32; 
assign mid_out32 = in32; 
assign out33 = in33; 
assign mid_out33 = in33; 
assign out34 = in34; 
assign mid_out34 = in34; 
assign out35 = in35; 
assign mid_out35 = in35; 
assign out36 = in36; 
assign mid_out36 = in36; 
assign out37 = in37; 
assign mid_out37 = in37; 
assign out38 = in38; 
assign mid_out38 = in38; 
assign out39 = in39; 
assign mid_out39 = in39; 
assign out40 = in40; 
assign mid_out40 = in40; 
assign out41 = in41; 
assign mid_out41 = in41; 
assign out42 = in42; 
assign mid_out42 = in42; 
assign out43 = in43; 
assign mid_out43 = in43; 
assign out44 = in44; 
assign mid_out44 = in44; 
assign out45 = in45; 
assign mid_out45 = in45; 
assign out46 = in46; 
assign mid_out46 = in46; 
assign out47 = in47; 
assign mid_out47 = in47; 
assign out48 = in48; 
assign mid_out48 = in48; 
assign out49 = in49; 
assign mid_out49 = in49; 
assign out50 = in50; 
assign mid_out50 = in50; 
assign out51 = in51; 
assign mid_out51 = in51; 
assign out52 = in52; 
assign mid_out52 = in52; 
assign out53 = in53; 
assign mid_out53 = in53; 
assign out54 = in54; 
assign mid_out54 = in54; 
assign out55 = in55; 
assign mid_out55 = in55; 
assign out56 = in56; 
assign mid_out56 = in56; 
assign out57 = in57; 
assign mid_out57 = in57; 
assign out58 = in58; 
assign mid_out58 = in58; 
assign out59 = in59; 
assign mid_out59 = in59; 
assign out60 = in60; 
assign mid_out60 = in60; 
assign out61 = in61; 
assign mid_out61 = in61; 
assign out62 = in62; 
assign mid_out62 = in62; 
assign out63 = in63; 
assign mid_out63 = in63; 
assign out64 = in64; 
assign mid_out64 = in64; 
assign out65 = in65; 
assign mid_out65 = in65; 
assign out66 = in66; 
assign mid_out66 = in66; 
assign out67 = in67; 
assign mid_out67 = in67; 
assign out68 = in68; 
assign mid_out68 = in68; 
assign out69 = in69; 
assign mid_out69 = in69; 
assign out70 = in70; 
assign mid_out70 = in70; 
assign out71 = in71; 
assign mid_out71 = in71; 
assign out72 = in72; 
assign mid_out72 = in72; 
assign out73 = in73; 
assign mid_out73 = in73; 
assign out74 = in74; 
assign mid_out74 = in74; 
assign out75 = in75; 
assign mid_out75 = in75; 
assign out76 = in76; 
assign mid_out76 = in76; 
assign out77 = in77; 
assign mid_out77 = in77; 
assign out78 = in78; 
assign mid_out78 = in78; 
assign out79 = in79; 
assign mid_out79 = in79; 
assign out80 = in80; 
assign mid_out80 = in80; 
assign out81 = in81; 
assign mid_out81 = in81; 
assign out82 = in82; 
assign mid_out82 = in82; 
assign out83 = in83; 
assign mid_out83 = in83; 
assign out84 = in84; 
assign mid_out84 = in84; 
assign out85 = in85; 
assign mid_out85 = in85; 
assign out86 = in86; 
assign mid_out86 = in86; 
assign out87 = in87; 
assign mid_out87 = in87; 
assign out88 = in88; 
assign mid_out88 = in88; 
assign out89 = in89; 
assign mid_out89 = in89; 
assign out90 = in90; 
assign mid_out90 = in90; 
assign out91 = in91; 
assign mid_out91 = in91; 
assign out92 = in92; 
assign mid_out92 = in92; 
assign out93 = in93; 
assign mid_out93 = in93; 
assign out94 = in94; 
assign mid_out94 = in94; 
assign out95 = in95; 
assign mid_out95 = in95; 
assign out96 = in96; 
assign mid_out96 = in96; 
assign out97 = in97; 
assign mid_out97 = in97; 
assign out98 = in98; 
assign mid_out98 = in98; 
assign out99 = in99; 
assign mid_out99 = in99; 
endmodule
//----- END Verilog Module of Channel X [1][0] -----

