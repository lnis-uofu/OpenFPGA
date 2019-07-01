/*Carry look-ahead adder
Author: Zhu Xu
Email: m99a1@yahoo.cn
*/

module	operator_A(
input	A,
input	B,
output	P,
output	G
);

assign	P=A^B;
assign	G=A&B;

endmodule

module	operator_B(
input	P,G,P1,G1,
output	Po,Go
);

assign	Po=P&P1;
assign	Go=G|(P&G1);

endmodule

module	operator_C(
input	P,G,G1,
output	Go
);

assign	Go=G|(P&G1);

endmodule


/* 32-bit prefix-2 Han-Carlson adder
stage 0:	Number of Generation=32,	NP=32,	NOA=32,	NOB=0,	NOC=0.
stage 1:	NG=16,	NP=15,	NOA=0,	NOB=15,	NOC=1.	
stage 2:	NG=16,	NP=14,	NOA=0,	NOB=14,	NOC=1.	
stage 3:	NG=16,	NP=12,	NOA=0,	NOB=12,	NOC=2.	
stage 4:	NG=16,	NP=8,	NOA=0,	NOB=8,	NOC=4.	
stage 5:	NG=16,	NP=0,	NOA=0,	NOB=0,	NOC=8.	
stage 6;	NG=32,	NP=0,	NOA=0,	NOB=0,	NOC=15.
*/
module	adder_32bit(
input	[31:0]i_a,i_b,
input	i_c,
output	[31:0]o_s,
output	o_c
);

//stage 0
wire	[31:0]P0,G0;
operator_A	operator_A_0(i_a[0],i_b[0],P0[0],G0[0]);
operator_A	operator_A_1(i_a[1],i_b[1],P0[1],G0[1]);
operator_A	operator_A_2(i_a[2],i_b[2],P0[2],G0[2]);
operator_A	operator_A_3(i_a[3],i_b[3],P0[3],G0[3]);
operator_A	operator_A_4(i_a[4],i_b[4],P0[4],G0[4]);
operator_A	operator_A_5(i_a[5],i_b[5],P0[5],G0[5]);
operator_A	operator_A_6(i_a[6],i_b[6],P0[6],G0[6]);
operator_A	operator_A_7(i_a[7],i_b[7],P0[7],G0[7]);
operator_A	operator_A_8(i_a[8],i_b[8],P0[8],G0[8]);
operator_A	operator_A_9(i_a[9],i_b[9],P0[9],G0[9]);
operator_A	operator_A_10(i_a[10],i_b[10],P0[10],G0[10]);
operator_A	operator_A_11(i_a[11],i_b[11],P0[11],G0[11]);
operator_A	operator_A_12(i_a[12],i_b[12],P0[12],G0[12]);
operator_A	operator_A_13(i_a[13],i_b[13],P0[13],G0[13]);
operator_A	operator_A_14(i_a[14],i_b[14],P0[14],G0[14]);
operator_A	operator_A_15(i_a[15],i_b[15],P0[15],G0[15]);
operator_A	operator_A_16(i_a[16],i_b[16],P0[16],G0[16]);
operator_A	operator_A_17(i_a[17],i_b[17],P0[17],G0[17]);
operator_A	operator_A_18(i_a[18],i_b[18],P0[18],G0[18]);
operator_A	operator_A_19(i_a[19],i_b[19],P0[19],G0[19]);
operator_A	operator_A_20(i_a[20],i_b[20],P0[20],G0[20]);
operator_A	operator_A_21(i_a[21],i_b[21],P0[21],G0[21]);
operator_A	operator_A_22(i_a[22],i_b[22],P0[22],G0[22]);
operator_A	operator_A_23(i_a[23],i_b[23],P0[23],G0[23]);
operator_A	operator_A_24(i_a[24],i_b[24],P0[24],G0[24]);
operator_A	operator_A_25(i_a[25],i_b[25],P0[25],G0[25]);
operator_A	operator_A_26(i_a[26],i_b[26],P0[26],G0[26]);
operator_A	operator_A_27(i_a[27],i_b[27],P0[27],G0[27]);
operator_A	operator_A_28(i_a[28],i_b[28],P0[28],G0[28]);
operator_A	operator_A_29(i_a[29],i_b[29],P0[29],G0[29]);
operator_A	operator_A_30(i_a[30],i_b[30],P0[30],G0[30]);
operator_A	operator_A_31(i_a[31],i_b[31],P0[31],G0[31]);

//stage 1
wire	[15:0]G1;
wire	[15:1]P1;
operator_C	operator_C_stage_1_0(P0[0],G0[0],i_c,G1[0]);
operator_B	operator_B_stage_1_1(P0[2],G0[2],P0[1],G0[1],P1[1],G1[1]);
operator_B	operator_B_stage_1_2(P0[4],G0[4],P0[3],G0[3],P1[2],G1[2]);
operator_B	operator_B_stage_1_3(P0[6],G0[6],P0[5],G0[5],P1[3],G1[3]);
operator_B	operator_B_stage_1_4(P0[8],G0[8],P0[7],G0[7],P1[4],G1[4]);
operator_B	operator_B_stage_1_5(P0[10],G0[10],P0[9],G0[9],P1[5],G1[5]);
operator_B	operator_B_stage_1_6(P0[12],G0[12],P0[11],G0[11],P1[6],G1[6]);
operator_B	operator_B_stage_1_7(P0[14],G0[14],P0[13],G0[13],P1[7],G1[7]);
operator_B	operator_B_stage_1_8(P0[16],G0[16],P0[15],G0[15],P1[8],G1[8]);
operator_B	operator_B_stage_1_9(P0[18],G0[18],P0[17],G0[17],P1[9],G1[9]);
operator_B	operator_B_stage_1_10(P0[20],G0[20],P0[19],G0[19],P1[10],G1[10]);
operator_B	operator_B_stage_1_11(P0[22],G0[22],P0[21],G0[21],P1[11],G1[11]);
operator_B	operator_B_stage_1_12(P0[24],G0[24],P0[23],G0[23],P1[12],G1[12]);
operator_B	operator_B_stage_1_13(P0[26],G0[26],P0[25],G0[25],P1[13],G1[13]);
operator_B	operator_B_stage_1_14(P0[28],G0[28],P0[27],G0[27],P1[14],G1[14]);
operator_B	operator_B_stage_1_15(P0[30],G0[30],P0[29],G0[29],P1[15],G1[15]);



//stage 2
wire	[15:0]G2;
wire	[15:2]P2;
assign	G2[0]=G1[0];
operator_C	operator_C_stage_2_1(P1[1],G1[1],G1[0],G2[1]);
operator_B	operator_B_stage_2_2(P1[2], G1[2],P1[1],G1[1],P2[2],G2[2]);
operator_B	operator_B_stage_2_3(P1[3], G1[3],P1[2],G1[2],P2[3],G2[3]);
operator_B	operator_B_stage_2_4(P1[4], G1[4],P1[3],G1[3],P2[4],G2[4]);
operator_B	operator_B_stage_2_5(P1[5], G1[5],P1[4],G1[4],P2[5],G2[5]);
operator_B	operator_B_stage_2_6(P1[6], G1[6],P1[5],G1[5],P2[6],G2[6]);
operator_B	operator_B_stage_2_7(P1[7], G1[7],P1[6],G1[6],P2[7],G2[7]);
operator_B	operator_B_stage_2_8(P1[8], G1[8],P1[7],G1[7],P2[8],G2[8]);
operator_B	operator_B_stage_2_9(P1[9], G1[9],P1[8],G1[8],P2[9],G2[9]);
operator_B	operator_B_stage_2_10(P1[10], G1[10],P1[9],G1[9],P2[10],G2[10]);
operator_B	operator_B_stage_2_11(P1[11], G1[11],P1[10],G1[10],P2[11],G2[11]);
operator_B	operator_B_stage_2_12(P1[12], G1[12],P1[11],G1[11],P2[12],G2[12]);
operator_B	operator_B_stage_2_13(P1[13], G1[13],P1[12],G1[12],P2[13],G2[13]);
operator_B	operator_B_stage_2_14(P1[14], G1[14],P1[13],G1[13],P2[14],G2[14]);
operator_B	operator_B_stage_2_15(P1[15], G1[15],P1[14],G1[14],P2[15],G2[15]);

//stage 3
wire	[15:0]G3;
wire	[15:4]P3;
assign	G3[0]=G2[0];
assign	G3[1]=G2[1];
operator_C	operator_C_stage_3_2(P2[2],G2[2],G2[0],G3[2]);
operator_C	operator_C_stage_3_3(P2[3],G2[3],G2[1],G3[3]);
operator_B	operator_B_stage_3_4(P2[4], G2[4],P2[2],G2[2],P3[4],G3[4]);
operator_B	operator_B_stage_3_5(P2[5], G2[5],P2[3],G2[3],P3[5],G3[5]);
operator_B	operator_B_stage_3_6(P2[6], G2[6],P2[4],G2[4],P3[6],G3[6]);
operator_B	operator_B_stage_3_7(P2[7], G2[7],P2[5],G2[5],P3[7],G3[7]);
operator_B	operator_B_stage_3_8(P2[8], G2[8],P2[6],G2[6],P3[8],G3[8]);
operator_B	operator_B_stage_3_9(P2[9], G2[9],P2[7],G2[7],P3[9],G3[9]);
operator_B	operator_B_stage_3_10(P2[10], G2[10],P2[8],G2[8],P3[10],G3[10]);
operator_B	operator_B_stage_3_11(P2[11], G2[11],P2[9],G2[9],P3[11],G3[11]);
operator_B	operator_B_stage_3_12(P2[12], G2[12],P2[10],G2[10],P3[12],G3[12]);
operator_B	operator_B_stage_3_13(P2[13], G2[13],P2[11],G2[11],P3[13],G3[13]);
operator_B	operator_B_stage_3_14(P2[14], G2[14],P2[12],G2[12],P3[14],G3[14]);
operator_B	operator_B_stage_3_15(P2[15], G2[15],P2[13],G2[13],P3[15],G3[15]);

//stage 4
wire	[15:0]G4;
wire	[15:8]P4;
assign	G4[0]=G3[0];
assign	G4[1]=G3[1];
assign	G4[2]=G3[2];
assign	G4[3]=G3[3];
operator_C	operator_C_stage_4_4(P3[4],G3[4],G3[0],G4[4]);
operator_C	operator_C_stage_4_5(P3[5],G3[5],G3[1],G4[5]);
operator_C	operator_C_stage_4_6(P3[6],G3[6],G3[2],G4[6]);
operator_C	operator_C_stage_4_7(P3[7],G3[7],G3[3],G4[7]);
operator_B	operator_B_stage_4_8(P3[8], G3[8],P3[4],G3[4],P4[8],G4[8]);
operator_B	operator_B_stage_4_9(P3[9], G3[9],P3[5],G3[5],P4[9],G4[9]);
operator_B	operator_B_stage_4_10(P3[10], G3[10],P3[6],G3[6],P4[10],G4[10]);
operator_B	operator_B_stage_4_11(P3[11], G3[11],P3[7],G3[7],P4[11],G4[11]);
operator_B	operator_B_stage_4_12(P3[12], G3[12],P3[8],G3[8],P4[12],G4[12]);
operator_B	operator_B_stage_4_13(P3[13], G3[13],P3[9],G3[9],P4[13],G4[13]);
operator_B	operator_B_stage_4_14(P3[14], G3[14],P3[10],G3[10],P4[14],G4[14]);
operator_B	operator_B_stage_4_15(P3[15], G3[15],P3[11],G3[11],P4[15],G4[15]);

//stage 5
wire	[15:0]G5;
assign	G5[0]=G4[0];
assign	G5[1]=G4[1];
assign	G5[2]=G4[2];
assign	G5[3]=G4[3];
assign	G5[4]=G4[4];
assign	G5[5]=G4[5];
assign	G5[6]=G4[6];
assign	G5[7]=G4[7];
operator_C	operator_C_stage_5_8(P4[8],G4[8],G4[0],G5[8]);
operator_C	operator_C_stage_5_9(P4[9],G4[9],G4[1],G5[9]);
operator_C	operator_C_stage_5_10(P4[10],G4[10],G4[2],G5[10]);
operator_C	operator_C_stage_5_11(P4[11],G4[11],G4[3],G5[11]);
operator_C	operator_C_stage_5_12(P4[12],G4[12],G4[4],G5[12]);
operator_C	operator_C_stage_5_13(P4[13],G4[13],G4[5],G5[13]);
operator_C	operator_C_stage_5_14(P4[14],G4[14],G4[6],G5[14]);
operator_C	operator_C_stage_5_15(P4[15],G4[15],G4[7],G5[15]);

//stage 6
wire	[31:0]G6;
assign	G6[0]=G5[0];
assign	G6[2]=G5[1];
assign	G6[4]=G5[2];
assign	G6[6]=G5[3];
assign	G6[8]=G5[4];
assign	G6[10]=G5[5];
assign	G6[12]=G5[6];
assign	G6[14]=G5[7];
assign	G6[16]=G5[8];
assign	G6[18]=G5[9];
assign	G6[20]=G5[10];
assign	G6[22]=G5[11];
assign	G6[24]=G5[12];
assign	G6[26]=G5[13];
assign	G6[28]=G5[14];
assign	G6[30]=G5[15];
operator_C	operator_C_stage_6_0(P0[1],G0[1],G5[0],G6[1]);
operator_C	operator_C_stage_6_1(P0[3],G0[3],G5[1],G6[3]);
operator_C	operator_C_stage_6_2(P0[5],G0[5],G5[2],G6[5]);
operator_C	operator_C_stage_6_3(P0[7],G0[7],G5[3],G6[7]);
operator_C	operator_C_stage_6_4(P0[9],G0[9],G5[4],G6[9]);
operator_C	operator_C_stage_6_5(P0[11],G0[11],G5[5],G6[11]);
operator_C	operator_C_stage_6_6(P0[13],G0[13],G5[6],G6[13]);
operator_C	operator_C_stage_6_7(P0[15],G0[15],G5[7],G6[15]);
operator_C	operator_C_stage_6_8(P0[17],G0[17],G5[8],G6[17]);
operator_C	operator_C_stage_6_9(P0[19],G0[19],G5[9],G6[19]);
operator_C	operator_C_stage_6_10(P0[21],G0[21],G5[10],G6[21]);
operator_C	operator_C_stage_6_11(P0[23],G0[23],G5[11],G6[23]);
operator_C	operator_C_stage_6_12(P0[25],G0[25],G5[12],G6[25]);
operator_C	operator_C_stage_6_13(P0[27],G0[27],G5[13],G6[27]);
operator_C	operator_C_stage_6_14(P0[29],G0[29],G5[14],G6[29]);
operator_C	operator_C_stage_6_15(P0[31],G0[31],G5[15],G6[31]);

assign	o_s[0]=P0[0]^i_c;
assign	o_s[1]=P0[1]^G6[0];
assign	o_s[2]=P0[2]^G6[1];
assign	o_s[3]=P0[3]^G6[2];
assign	o_s[4]=P0[4]^G6[3];
assign	o_s[5]=P0[5]^G6[4];
assign	o_s[6]=P0[6]^G6[5];
assign	o_s[7]=P0[7]^G6[6];
assign	o_s[8]=P0[8]^G6[7];
assign	o_s[9]=P0[9]^G6[8];
assign	o_s[10]=P0[10]^G6[9];
assign	o_s[11]=P0[11]^G6[10];
assign	o_s[12]=P0[12]^G6[11];
assign	o_s[13]=P0[13]^G6[12];
assign	o_s[14]=P0[14]^G6[13];
assign	o_s[15]=P0[15]^G6[14];
assign	o_s[16]=P0[16]^G6[15];
assign	o_s[17]=P0[17]^G6[16];
assign	o_s[18]=P0[18]^G6[17];
assign	o_s[19]=P0[19]^G6[18];
assign	o_s[20]=P0[20]^G6[19];
assign	o_s[21]=P0[21]^G6[20];
assign	o_s[22]=P0[22]^G6[21];
assign	o_s[23]=P0[23]^G6[22];
assign	o_s[24]=P0[24]^G6[23];
assign	o_s[25]=P0[25]^G6[24];
assign	o_s[26]=P0[26]^G6[25];
assign	o_s[27]=P0[27]^G6[26];
assign	o_s[28]=P0[28]^G6[27];
assign	o_s[29]=P0[29]^G6[28];
assign	o_s[30]=P0[30]^G6[29];
assign	o_s[31]=P0[31]^G6[30];
assign	o_c=G6[31];

endmodule