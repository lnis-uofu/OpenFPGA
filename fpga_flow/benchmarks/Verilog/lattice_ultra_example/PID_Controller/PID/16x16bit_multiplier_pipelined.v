/*16x16-bit multiplier
Author: Zhu Xu
Email: m99a1@yahoo.cn
*/

//Booth Encoder Array
module booth_array(
input	[15:0]multiplier,
output	[7:0]zero,
output	[7:0]double,
output	[7:0]negation
);

booth_radix4	booth_radix4_0(
{multiplier[1:0],1'b0},
zero[0],
double[0],
negation[0]
);

booth_radix4	booth_radix4_1(
multiplier[3:1],
zero[1],
double[1],
negation[1]
);

booth_radix4	booth_radix4_2(
multiplier[5:3],
zero[2],
double[2],
negation[2]
);
booth_radix4	booth_radix4_3(
multiplier[7:5],
zero[3],
double[3],
negation[3]
);

booth_radix4	booth_radix4_4(
multiplier[9:7],
zero[4],
double[4],
negation[4]
);

booth_radix4	booth_radix4_5(
multiplier[11:9],
zero[5],
double[5],
negation[5]
);
booth_radix4	booth_radix4_6(
multiplier[13:11],
zero[6],
double[6],
negation[6]
);

booth_radix4	booth_radix4_7(
multiplier[15:13],
zero[7],
double[7],
negation[7]
);

endmodule

/*partial product generator unit
generate one 17-bit partial with inversed MSB without correction bit for negation
*/
module partial_product_gen(
input	[15:0]md,		//multiplicand
input	zero,
input	double,
input	negation,
output	[16:0]pp 
);

wire	[15:0]nmd;
assign	nmd=negation?~md:md;

wire	[15:0]zmd;
assign	zmd=zero?0:nmd;

assign	pp=double?{~zmd[15],zmd[14:0],negation}:{~zmd[15],zmd[15:0]};

endmodule

module	half_adder(
input	A,
input	B,
output	S,
output	carry
);
assign	S=A^B;
assign	carry=A&B;
endmodule

module full_adder(
input	A,
input	B,
input	cin,
output	S,
output	cout
);
wire	AB;
assign	AB=A&B;
wire	AxorB;
assign	AxorB=A^B;
assign	S=AxorB^cin;
assign	cout=AB|(AxorB&cin);
endmodule

module	compressor42(
input	A,
input	B,
input	C,
input	D,
input	cin,
output	S,
output	carry,
output	cout
);
wire	AB;
assign	AB=A&B;
wire	AxorB;
assign	AxorB=A^B;
wire	CD;
assign	CD=C&D;
wire	CxorD;
assign	CxorD=C^D;

wire	AxBxCxD=AxorB^CxorD;

assign	cout=AB|CD;
assign	carry=(AB&CD)|(AxorB&CxorD)|((AxBxCxD)&cin);

assign	S=AxBxCxD^cin;

endmodule


module	multiplier_16x16bit_pipelined(
input	i_clk,
input	i_rst,
input	i_start,
input	[15:0]i_md,
input	[15:0]i_mr,
output	[31:0]o_product,
output	o_ready
);
/////////////////////////////////////////////////////////////stage 0///////////////////////////////////////////////////

reg	[15:0]md;
reg	[15:0]mr;
reg	stage_0_ready;

always @(posedge i_clk or negedge i_rst)begin
	if(!i_rst)begin
		md<=0;
		mr<=0;
		stage_0_ready<=0;
	end
	else begin
		if(i_start)begin
			md<=i_md;
			mr<=i_mr;
		end
		stage_0_ready<=i_start;
	end
	
end

wire	[7:0]zero;
wire	[7:0]double;
wire	[7:0]negation;

booth_array	booth_array_0(
mr,
zero,
double,
negation
);	


//layer 0
wire	layer_0_w0[1:0];
wire	layer_0_w1;
wire	layer_0_w2[2:0];
wire	layer_0_w3[1:0];
wire	layer_0_w4[3:0];
wire	layer_0_w5[2:0];
wire	layer_0_w6[4:0];
wire	layer_0_w7[3:0];
wire	layer_0_w8[5:0];
wire	layer_0_w9[4:0];
wire	layer_0_w10[6:0];
wire	layer_0_w11[5:0];
wire	layer_0_w12[7:0];
wire	layer_0_w13[6:0];
wire	layer_0_w14[8:0];
wire	layer_0_w15[7:0];
wire	layer_0_w16[8:0];
wire	layer_0_w17[7:0];
wire	layer_0_w18[6:0];
wire	layer_0_w19[6:0];
wire	layer_0_w20[5:0];
wire	layer_0_w21[5:0];
wire	layer_0_w22[4:0];
wire	layer_0_w23[4:0];
wire	layer_0_w24[3:0];
wire	layer_0_w25[3:0];
wire	layer_0_w26[2:0];
wire	layer_0_w27[2:0];
wire	layer_0_w28[1:0];
wire	layer_0_w29[1:0];
wire	layer_0_w30;
wire	layer_0_w31;
partial_product_gen	partial_product_gen_0(
md,
zero[0],
double[0],
negation[0],
{layer_0_w16[0],layer_0_w15[0],layer_0_w14[0],layer_0_w13[0],layer_0_w12[0],layer_0_w11[0],layer_0_w10[0],layer_0_w9[0],layer_0_w8[0],layer_0_w7[0],layer_0_w6[0],layer_0_w5[0],layer_0_w4[0],layer_0_w3[0],layer_0_w2[0],layer_0_w1,layer_0_w0[0]}
);
partial_product_gen	partial_product_gen_1(
md,
zero[1],
double[1],
negation[1],
{layer_0_w18[0],layer_0_w17[0],layer_0_w16[1],layer_0_w15[1],layer_0_w14[1],layer_0_w13[1],layer_0_w12[1],layer_0_w11[1],layer_0_w10[1],layer_0_w9[1],layer_0_w8[1],layer_0_w7[1],layer_0_w6[1],layer_0_w5[1],layer_0_w4[1],layer_0_w3[1],layer_0_w2[1]}
);
partial_product_gen	partial_product_gen_2(
md,
zero[2],
double[2],
negation[2],
{layer_0_w20[0],layer_0_w19[0],layer_0_w18[1],layer_0_w17[1],layer_0_w16[2],layer_0_w15[2],layer_0_w14[2],layer_0_w13[2],layer_0_w12[2],layer_0_w11[2],layer_0_w10[2],layer_0_w9[2],layer_0_w8[2],layer_0_w7[2],layer_0_w6[2],layer_0_w5[2],layer_0_w4[2]}
);
partial_product_gen	partial_product_gen_3(
md,
zero[3],
double[3],
negation[3],
{layer_0_w22[0],layer_0_w21[0],layer_0_w20[1],layer_0_w19[1],layer_0_w18[2],layer_0_w17[2],layer_0_w16[3],layer_0_w15[3],layer_0_w14[3],layer_0_w13[3],layer_0_w12[3],layer_0_w11[3],layer_0_w10[3],layer_0_w9[3],layer_0_w8[3],layer_0_w7[3],layer_0_w6[3]}
);
partial_product_gen	partial_product_gen_4(
md,
zero[4],
double[4],
negation[4],
{layer_0_w24[0],layer_0_w23[0],layer_0_w22[1],layer_0_w21[1],layer_0_w20[2],layer_0_w19[2],layer_0_w18[3],layer_0_w17[3],layer_0_w16[4],layer_0_w15[4],layer_0_w14[4],layer_0_w13[4],layer_0_w12[4],layer_0_w11[4],layer_0_w10[4],layer_0_w9[4],layer_0_w8[4]}
);
partial_product_gen	partial_product_gen_5(
md,
zero[5],
double[5],
negation[5],
{layer_0_w26[0],layer_0_w25[0],layer_0_w24[1],layer_0_w23[1],layer_0_w22[2],layer_0_w21[2],layer_0_w20[3],layer_0_w19[3],layer_0_w18[4],layer_0_w17[4],layer_0_w16[5],layer_0_w15[5],layer_0_w14[5],layer_0_w13[5],layer_0_w12[5],layer_0_w11[5],layer_0_w10[5]}
);
partial_product_gen	partial_product_gen_6(
md,
zero[6],
double[6],
negation[6],
{layer_0_w28[0],layer_0_w27[0],layer_0_w26[1],layer_0_w25[1],layer_0_w24[2],layer_0_w23[2],layer_0_w22[3],layer_0_w21[3],layer_0_w20[4],layer_0_w19[4],layer_0_w18[5],layer_0_w17[5],layer_0_w16[6],layer_0_w15[6],layer_0_w14[6],layer_0_w13[6],layer_0_w12[6]}
);
partial_product_gen	partial_product_gen_7(
md,
zero[7],
double[7],
negation[7],
{layer_0_w30,layer_0_w29[0],layer_0_w28[1],layer_0_w27[1],layer_0_w26[2],layer_0_w25[2],layer_0_w24[3],layer_0_w23[3],layer_0_w22[4],layer_0_w21[4],layer_0_w20[5],layer_0_w19[5],layer_0_w18[6],layer_0_w17[6],layer_0_w16[7],layer_0_w15[7],layer_0_w14[7]}
);
//correction for negation
assign	layer_0_w0[1]=negation[0];
//sign extension
assign	layer_0_w16[8]=1;
assign	layer_0_w17[7]=1;
//correction for negation
assign	layer_0_w2[2]=negation[1];
//sign extension
assign	layer_0_w19[6]=1;
//correction for negation
assign	layer_0_w4[3]=negation[2];
//sign extension
assign	layer_0_w21[5]=1;
//correction for negation
assign	layer_0_w6[4]=negation[3];
//sign extension
assign	layer_0_w23[4]=1;
//correction for negation
assign	layer_0_w8[5]=negation[4];
//sign extension
assign	layer_0_w25[3]=1;
//correction for negation
assign	layer_0_w10[6]=negation[5];
//sign extension
assign	layer_0_w27[2]=1;
//correction for negation
assign	layer_0_w12[7]=negation[6];
//sign extension
assign	layer_0_w29[1]=1;
//correction for negation
assign	layer_0_w14[8]=negation[7];
//sign extension
assign	layer_0_w31=1;

//layer 1
wire	layer_1_w0[1:0];
wire	layer_1_w1;
wire	layer_1_w2[2:0];
wire	layer_1_w3[1:0];
wire	layer_1_w4[1:0];
wire	layer_1_w5[1:0];
wire	layer_1_w6[1:0];
wire	layer_1_w7[3:0];
wire	layer_1_w8[2:0];
wire	layer_1_w9[2:0];
wire	layer_1_w10[4:0];
wire	layer_1_w11[3:0];
wire	layer_1_w12[3:0];
wire	layer_1_w13[5:0];
wire	layer_1_w14[4:0];
wire	layer_1_w15[4:0];
wire	layer_1_w16[5:0];
wire	layer_1_w17[4:0];
wire	layer_1_w18[5:0];
wire	layer_1_w19[4:0];
wire	layer_1_w20[3:0];
wire	layer_1_w21[3:0];
wire	layer_1_w22[2:0];
wire	layer_1_w23[2:0];
wire	layer_1_w24[3:0];
wire	layer_1_w25[2:0];
wire	layer_1_w26[1:0];
wire	layer_1_w27[1:0];
wire	layer_1_w28[2:0];
wire	layer_1_w29[1:0];
wire	layer_1_w30;
wire	layer_1_w31;
assign	layer_1_w0[0]=layer_0_w0[0];
assign	layer_1_w0[1]=layer_0_w0[1];
assign	layer_1_w1=layer_0_w1;
assign	layer_1_w2[0]=layer_0_w2[0];
assign	layer_1_w2[1]=layer_0_w2[1];
assign	layer_1_w2[2]=layer_0_w2[2];
assign	layer_1_w3[0]=layer_0_w3[0];
assign	layer_1_w3[1]=layer_0_w3[1];
full_adder	layer_1_full_adder_0(
layer_0_w4[0],
layer_0_w4[1],
layer_0_w4[2],
layer_1_w4[0],
layer_1_w5[0]
);
assign	layer_1_w4[1]=layer_0_w4[3];
full_adder	layer_1_full_adder_1(
layer_0_w5[0],
layer_0_w5[1],
layer_0_w5[2],
layer_1_w5[1],
layer_1_w6[0]
);
compressor42	layer_1_compressor42_0(
layer_0_w6[0],
layer_0_w6[1],
layer_0_w6[2],
layer_0_w6[3],
layer_0_w6[4],
layer_1_w6[1],
layer_1_w7[0],
layer_1_w7[1]
);
full_adder	layer_1_full_adder_2(
layer_0_w7[0],
layer_0_w7[1],
layer_0_w7[2],
layer_1_w7[2],
layer_1_w8[0]
);
assign	layer_1_w7[3]=layer_0_w7[3];
compressor42	layer_1_compressor42_1(
layer_0_w8[0],
layer_0_w8[1],
layer_0_w8[2],
layer_0_w8[3],
layer_0_w8[4],
layer_1_w8[1],
layer_1_w9[0],
layer_1_w9[1]
);
assign	layer_1_w8[2]=layer_0_w8[5];
compressor42	layer_1_compressor42_2(
layer_0_w9[0],
layer_0_w9[1],
layer_0_w9[2],
layer_0_w9[3],
layer_0_w9[4],
layer_1_w9[2],
layer_1_w10[0],
layer_1_w10[1]
);
compressor42	layer_1_compressor42_3(
layer_0_w10[0],
layer_0_w10[1],
layer_0_w10[2],
layer_0_w10[3],
layer_0_w10[4],
layer_1_w10[2],
layer_1_w11[0],
layer_1_w11[1]
);
assign	layer_1_w10[3]=layer_0_w10[5];
assign	layer_1_w10[4]=layer_0_w10[6];
compressor42	layer_1_compressor42_4(
layer_0_w11[0],
layer_0_w11[1],
layer_0_w11[2],
layer_0_w11[3],
layer_0_w11[4],
layer_1_w11[2],
layer_1_w12[0],
layer_1_w12[1]
);
assign	layer_1_w11[3]=layer_0_w11[5];
compressor42	layer_1_compressor42_5(
layer_0_w12[0],
layer_0_w12[1],
layer_0_w12[2],
layer_0_w12[3],
layer_0_w12[4],
layer_1_w12[2],
layer_1_w13[0],
layer_1_w13[1]
);
full_adder	layer_1_full_adder_3(
layer_0_w12[5],
layer_0_w12[6],
layer_0_w12[7],
layer_1_w12[3],
layer_1_w13[2]
);
compressor42	layer_1_compressor42_6(
layer_0_w13[0],
layer_0_w13[1],
layer_0_w13[2],
layer_0_w13[3],
layer_0_w13[4],
layer_1_w13[3],
layer_1_w14[0],
layer_1_w14[1]
);
assign	layer_1_w13[4]=layer_0_w13[5];
assign	layer_1_w13[5]=layer_0_w13[6];
compressor42	layer_1_compressor42_7(
layer_0_w14[0],
layer_0_w14[1],
layer_0_w14[2],
layer_0_w14[3],
layer_0_w14[4],
layer_1_w14[2],
layer_1_w15[0],
layer_1_w15[1]
);
full_adder	layer_1_full_adder_4(
layer_0_w14[5],
layer_0_w14[6],
layer_0_w14[7],
layer_1_w14[3],
layer_1_w15[2]
);
assign	layer_1_w14[4]=layer_0_w14[8];
compressor42	layer_1_compressor42_8(
layer_0_w15[0],
layer_0_w15[1],
layer_0_w15[2],
layer_0_w15[3],
layer_0_w15[4],
layer_1_w15[3],
layer_1_w16[0],
layer_1_w16[1]
);
full_adder	layer_1_full_adder_5(
layer_0_w15[5],
layer_0_w15[6],
layer_0_w15[7],
layer_1_w15[4],
layer_1_w16[2]
);
compressor42	layer_1_compressor42_9(
layer_0_w16[0],
layer_0_w16[1],
layer_0_w16[2],
layer_0_w16[3],
layer_0_w16[4],
layer_1_w16[3],
layer_1_w17[0],
layer_1_w17[1]
);
full_adder	layer_1_full_adder_6(
layer_0_w16[5],
layer_0_w16[6],
layer_0_w16[7],
layer_1_w16[4],
layer_1_w17[2]
);
assign	layer_1_w16[5]=layer_0_w16[8];
compressor42	layer_1_compressor42_10(
layer_0_w17[0],
layer_0_w17[1],
layer_0_w17[2],
layer_0_w17[3],
layer_0_w17[4],
layer_1_w17[3],
layer_1_w18[0],
layer_1_w18[1]
);
full_adder	layer_1_full_adder_7(
layer_0_w17[5],
layer_0_w17[6],
layer_0_w17[7],
layer_1_w17[4],
layer_1_w18[2]
);
compressor42	layer_1_compressor42_11(
layer_0_w18[0],
layer_0_w18[1],
layer_0_w18[2],
layer_0_w18[3],
layer_0_w18[4],
layer_1_w18[3],
layer_1_w19[0],
layer_1_w19[1]
);
assign	layer_1_w18[4]=layer_0_w18[5];
assign	layer_1_w18[5]=layer_0_w18[6];
compressor42	layer_1_compressor42_12(
layer_0_w19[0],
layer_0_w19[1],
layer_0_w19[2],
layer_0_w19[3],
layer_0_w19[4],
layer_1_w19[2],
layer_1_w20[0],
layer_1_w20[1]
);
assign	layer_1_w19[3]=layer_0_w19[5];
assign	layer_1_w19[4]=layer_0_w19[6];
compressor42	layer_1_compressor42_13(
layer_0_w20[0],
layer_0_w20[1],
layer_0_w20[2],
layer_0_w20[3],
layer_0_w20[4],
layer_1_w20[2],
layer_1_w21[0],
layer_1_w21[1]
);
assign	layer_1_w20[3]=layer_0_w20[5];
compressor42	layer_1_compressor42_14(
layer_0_w21[0],
layer_0_w21[1],
layer_0_w21[2],
layer_0_w21[3],
layer_0_w21[4],
layer_1_w21[2],
layer_1_w22[0],
layer_1_w22[1]
);
assign	layer_1_w21[3]=layer_0_w21[5];
compressor42	layer_1_compressor42_15(
layer_0_w22[0],
layer_0_w22[1],
layer_0_w22[2],
layer_0_w22[3],
layer_0_w22[4],
layer_1_w22[2],
layer_1_w23[0],
layer_1_w23[1]
);
compressor42	layer_1_compressor42_16(
layer_0_w23[0],
layer_0_w23[1],
layer_0_w23[2],
layer_0_w23[3],
layer_0_w23[4],
layer_1_w23[2],
layer_1_w24[0],
layer_1_w24[1]
);
full_adder	layer_1_full_adder_8(
layer_0_w24[0],
layer_0_w24[1],
layer_0_w24[2],
layer_1_w24[2],
layer_1_w25[0]
);
assign	layer_1_w24[3]=layer_0_w24[3];
full_adder	layer_1_full_adder_9(
layer_0_w25[0],
layer_0_w25[1],
layer_0_w25[2],
layer_1_w25[1],
layer_1_w26[0]
);
assign	layer_1_w25[2]=layer_0_w25[3];
full_adder	layer_1_full_adder_10(
layer_0_w26[0],
layer_0_w26[1],
layer_0_w26[2],
layer_1_w26[1],
layer_1_w27[0]
);
full_adder	layer_1_full_adder_11(
layer_0_w27[0],
layer_0_w27[1],
layer_0_w27[2],
layer_1_w27[1],
layer_1_w28[0]
);
assign	layer_1_w28[1]=layer_0_w28[0];
assign	layer_1_w28[2]=layer_0_w28[1];
assign	layer_1_w29[0]=layer_0_w29[0];
assign	layer_1_w29[1]=layer_0_w29[1];
assign	layer_1_w30=layer_0_w30;
assign	layer_1_w31=layer_0_w31;

//layer 2
wire	[1:0]layer_2_w0;
wire	layer_2_w1;
wire	[2:0]layer_2_w2;
wire	[1:0]layer_2_w3;
wire	[1:0]layer_2_w4;
wire	[1:0]layer_2_w5;
wire	[1:0]layer_2_w6;
wire	[1:0]layer_2_w7;
wire	[1:0]layer_2_w8;
wire	[1:0]layer_2_w9;
wire	[1:0]layer_2_w10;
wire	[3:0]layer_2_w11;
wire	[2:0]layer_2_w12;
wire	[2:0]layer_2_w13;
wire	[2:0]layer_2_w14;
wire	[2:0]layer_2_w15;
wire	[3:0]layer_2_w16;
wire	[2:0]layer_2_w17;
wire	[3:0]layer_2_w18;
wire	[2:0]layer_2_w19;
wire	[3:0]layer_2_w20;
wire	[2:0]layer_2_w21;
wire	[1:0]layer_2_w22;
wire	[1:0]layer_2_w23;
wire	[2:0]layer_2_w24;
wire	[1:0]layer_2_w25;
wire	[2:0]layer_2_w26;
wire	[1:0]layer_2_w27;
wire	layer_2_w28;
wire	[2:0]layer_2_w29;
wire	layer_2_w30;
wire	layer_2_w31;
assign	layer_2_w0[0]=layer_1_w0[0];
assign	layer_2_w0[1]=layer_1_w0[1];
assign	layer_2_w1=layer_1_w1;
assign	layer_2_w2[0]=layer_1_w2[0];
assign	layer_2_w2[1]=layer_1_w2[1];
assign	layer_2_w2[2]=layer_1_w2[2];
assign	layer_2_w3[0]=layer_1_w3[0];
assign	layer_2_w3[1]=layer_1_w3[1];
assign	layer_2_w4[0]=layer_1_w4[0];
assign	layer_2_w4[1]=layer_1_w4[1];
assign	layer_2_w5[0]=layer_1_w5[0];
assign	layer_2_w5[1]=layer_1_w5[1];
assign	layer_2_w6[0]=layer_1_w6[0];
assign	layer_2_w6[1]=layer_1_w6[1];
full_adder	layer_2_full_adder_0(
layer_1_w7[0],
layer_1_w7[1],
layer_1_w7[2],
layer_2_w7[0],
layer_2_w8[0]
);
assign	layer_2_w7[1]=layer_1_w7[3];
full_adder	layer_2_full_adder_1(
layer_1_w8[0],
layer_1_w8[1],
layer_1_w8[2],
layer_2_w8[1],
layer_2_w9[0]
);
full_adder	layer_2_full_adder_2(
layer_1_w9[0],
layer_1_w9[1],
layer_1_w9[2],
layer_2_w9[1],
layer_2_w10[0]
);
compressor42	layer_2_compressor42_0(
layer_1_w10[0],
layer_1_w10[1],
layer_1_w10[2],
layer_1_w10[3],
layer_1_w10[4],
layer_2_w10[1],
layer_2_w11[0],
layer_2_w11[1]
);
full_adder	layer_2_full_adder_3(
layer_1_w11[0],
layer_1_w11[1],
layer_1_w11[2],
layer_2_w11[2],
layer_2_w12[0]
);
assign	layer_2_w11[3]=layer_1_w11[3];
full_adder	layer_2_full_adder_4(
layer_1_w12[0],
layer_1_w12[1],
layer_1_w12[2],
layer_2_w12[1],
layer_2_w13[0]
);
assign	layer_2_w12[2]=layer_1_w12[3];
compressor42	layer_2_compressor42_1(
layer_1_w13[0],
layer_1_w13[1],
layer_1_w13[2],
layer_1_w13[3],
layer_1_w13[4],
layer_2_w13[1],
layer_2_w14[0],
layer_2_w14[1]
);
assign	layer_2_w13[2]=layer_1_w13[5];
compressor42	layer_2_compressor42_2(
layer_1_w14[0],
layer_1_w14[1],
layer_1_w14[2],
layer_1_w14[3],
layer_1_w14[4],
layer_2_w14[2],
layer_2_w15[0],
layer_2_w15[1]
);
compressor42	layer_2_compressor42_3(
layer_1_w15[0],
layer_1_w15[1],
layer_1_w15[2],
layer_1_w15[3],
layer_1_w15[4],
layer_2_w15[2],
layer_2_w16[0],
layer_2_w16[1]
);
compressor42	layer_2_compressor42_4(
layer_1_w16[0],
layer_1_w16[1],
layer_1_w16[2],
layer_1_w16[3],
layer_1_w16[4],
layer_2_w16[2],
layer_2_w17[0],
layer_2_w17[1]
);
assign	layer_2_w16[3]=layer_1_w16[5];
compressor42	layer_2_compressor42_5(
layer_1_w17[0],
layer_1_w17[1],
layer_1_w17[2],
layer_1_w17[3],
layer_1_w17[4],
layer_2_w17[2],
layer_2_w18[0],
layer_2_w18[1]
);
compressor42	layer_2_compressor42_6(
layer_1_w18[0],
layer_1_w18[1],
layer_1_w18[2],
layer_1_w18[3],
layer_1_w18[4],
layer_2_w18[2],
layer_2_w19[0],
layer_2_w19[1]
);
assign	layer_2_w18[3]=layer_1_w18[5];
compressor42	layer_2_compressor42_7(
layer_1_w19[0],
layer_1_w19[1],
layer_1_w19[2],
layer_1_w19[3],
layer_1_w19[4],
layer_2_w19[2],
layer_2_w20[0],
layer_2_w20[1]
);
full_adder	layer_2_full_adder_5(
layer_1_w20[0],
layer_1_w20[1],
layer_1_w20[2],
layer_2_w20[2],
layer_2_w21[0]
);
assign	layer_2_w20[3]=layer_1_w20[3];
full_adder	layer_2_full_adder_6(
layer_1_w21[0],
layer_1_w21[1],
layer_1_w21[2],
layer_2_w21[1],
layer_2_w22[0]
);
assign	layer_2_w21[2]=layer_1_w21[3];
full_adder	layer_2_full_adder_7(
layer_1_w22[0],
layer_1_w22[1],
layer_1_w22[2],
layer_2_w22[1],
layer_2_w23[0]
);
full_adder	layer_2_full_adder_8(
layer_1_w23[0],
layer_1_w23[1],
layer_1_w23[2],
layer_2_w23[1],
layer_2_w24[0]
);
full_adder	layer_2_full_adder_9(
layer_1_w24[0],
layer_1_w24[1],
layer_1_w24[2],
layer_2_w24[1],
layer_2_w25[0]
);
assign	layer_2_w24[2]=layer_1_w24[3];
full_adder	layer_2_full_adder_10(
layer_1_w25[0],
layer_1_w25[1],
layer_1_w25[2],
layer_2_w25[1],
layer_2_w26[0]
);
assign	layer_2_w26[1]=layer_1_w26[0];
assign	layer_2_w26[2]=layer_1_w26[1];
assign	layer_2_w27[0]=layer_1_w27[0];
assign	layer_2_w27[1]=layer_1_w27[1];
full_adder	layer_2_full_adder_11(
layer_1_w28[0],
layer_1_w28[1],
layer_1_w28[2],
layer_2_w28,
layer_2_w29[0]
);
assign	layer_2_w29[1]=layer_1_w29[0];
assign	layer_2_w29[2]=layer_1_w29[1];
assign	layer_2_w30=layer_1_w30;
assign	layer_2_w31=layer_1_w31;


///////////////////////////////////////////////////////stage 1///////////////////////////////////////////////////////
reg	[1:0]reg_layer_2_w0;
reg	reg_layer_2_w1;
reg	[2:0]reg_layer_2_w2;
reg	[1:0]reg_layer_2_w3;
reg	[1:0]reg_layer_2_w4;
reg	[1:0]reg_layer_2_w5;
reg	[1:0]reg_layer_2_w6;
reg	[1:0]reg_layer_2_w7;
reg	[1:0]reg_layer_2_w8;
reg	[1:0]reg_layer_2_w9;
reg	[1:0]reg_layer_2_w10;
reg	[3:0]reg_layer_2_w11;
reg	[2:0]reg_layer_2_w12;
reg	[2:0]reg_layer_2_w13;
reg	[2:0]reg_layer_2_w14;
reg	[2:0]reg_layer_2_w15;
reg	[3:0]reg_layer_2_w16;
reg	[2:0]reg_layer_2_w17;
reg	[3:0]reg_layer_2_w18;
reg	[2:0]reg_layer_2_w19;
reg	[3:0]reg_layer_2_w20;
reg	[2:0]reg_layer_2_w21;
reg	[1:0]reg_layer_2_w22;
reg	[1:0]reg_layer_2_w23;
reg	[2:0]reg_layer_2_w24;
reg	[1:0]reg_layer_2_w25;
reg	[2:0]reg_layer_2_w26;
reg	[1:0]reg_layer_2_w27;
reg	reg_layer_2_w28;
reg	[2:0]reg_layer_2_w29;
reg	reg_layer_2_w30;
reg	reg_layer_2_w31;
reg	stage_1_ready;
assign	o_ready=stage_1_ready;

always @(posedge i_clk or negedge i_rst)begin
	if(!i_rst)begin
		stage_1_ready<=0;
		reg_layer_2_w0<=0;
		reg_layer_2_w1<=0;
		reg_layer_2_w2<=0;
		reg_layer_2_w3<=0;
		reg_layer_2_w4<=0;
		reg_layer_2_w5<=0;
		reg_layer_2_w6<=0;
		reg_layer_2_w7<=0;
		reg_layer_2_w8<=0;
		reg_layer_2_w9<=0;
		reg_layer_2_w10<=0;
		reg_layer_2_w11<=0;
		reg_layer_2_w12<=0;
		reg_layer_2_w13<=0;
		reg_layer_2_w14<=0;
		reg_layer_2_w15<=0;
		reg_layer_2_w16<=0;
		reg_layer_2_w17<=0;
		reg_layer_2_w18<=0;
		reg_layer_2_w19<=0;
		reg_layer_2_w20<=0;
		reg_layer_2_w21<=0;
		reg_layer_2_w22<=0;
		reg_layer_2_w23<=0;
		reg_layer_2_w24<=0;
		reg_layer_2_w25<=0;
		reg_layer_2_w26<=0;
		reg_layer_2_w27<=0;
		reg_layer_2_w28<=0;
		reg_layer_2_w29<=0;
		reg_layer_2_w30<=0;
		reg_layer_2_w31<=0;
	end
	else begin
		if(stage_0_ready)begin
			reg_layer_2_w0<=layer_2_w0;
			reg_layer_2_w1<=layer_2_w1;
			reg_layer_2_w2<=layer_2_w2;
			reg_layer_2_w3<=layer_2_w3;
			reg_layer_2_w4<=layer_2_w4;
			reg_layer_2_w5<=layer_2_w5;
			reg_layer_2_w6<=layer_2_w6;
			reg_layer_2_w7<=layer_2_w7;
			reg_layer_2_w8<=layer_2_w8;
			reg_layer_2_w9<=layer_2_w9;
			reg_layer_2_w10<=layer_2_w10;
			reg_layer_2_w11<=layer_2_w11;
			reg_layer_2_w12<=layer_2_w12;
			reg_layer_2_w13<=layer_2_w13;
			reg_layer_2_w14<=layer_2_w14;
			reg_layer_2_w15<=layer_2_w15;
			reg_layer_2_w16<=layer_2_w16;
			reg_layer_2_w17<=layer_2_w17;
			reg_layer_2_w18<=layer_2_w18;
			reg_layer_2_w19<=layer_2_w19;
			reg_layer_2_w20<=layer_2_w20;
			reg_layer_2_w21<=layer_2_w21;
			reg_layer_2_w22<=layer_2_w22;
			reg_layer_2_w23<=layer_2_w23;
			reg_layer_2_w24<=layer_2_w24;
			reg_layer_2_w25<=layer_2_w25;
			reg_layer_2_w26<=layer_2_w26;
			reg_layer_2_w27<=layer_2_w27;
			reg_layer_2_w28<=layer_2_w28;
			reg_layer_2_w29<=layer_2_w29;
			reg_layer_2_w30<=layer_2_w30;
			reg_layer_2_w31<=layer_2_w31;
		end
		stage_1_ready<=stage_0_ready;
	end
end

//layer 3
wire	layer_3_w0[1:0];
wire	layer_3_w1;
wire	layer_3_w2[2:0];
wire	layer_3_w3[1:0];
wire	layer_3_w4[1:0];
wire	layer_3_w5[1:0];
wire	layer_3_w6[1:0];
wire	layer_3_w7[1:0];
wire	layer_3_w8[1:0];
wire	layer_3_w9[1:0];
wire	layer_3_w10[1:0];
wire	layer_3_w11[1:0];
wire	layer_3_w12[1:0];
wire	layer_3_w13[1:0];
wire	layer_3_w14[1:0];
wire	layer_3_w15[1:0];
wire	layer_3_w16[2:0];
wire	layer_3_w17[1:0];
wire	layer_3_w18[2:0];
wire	layer_3_w19[1:0];
wire	layer_3_w20[2:0];
wire	layer_3_w21[1:0];
wire	layer_3_w22[2:0];
wire	layer_3_w23[1:0];
wire	layer_3_w24;
wire	layer_3_w25[2:0];
wire	layer_3_w26;
wire	layer_3_w27[2:0];
wire	layer_3_w28;
wire	layer_3_w29;
wire	layer_3_w30[1:0];
wire	layer_3_w31;
assign	layer_3_w0[0]=reg_layer_2_w0[0];
assign	layer_3_w0[1]=reg_layer_2_w0[1];
assign	layer_3_w1=reg_layer_2_w1;
assign	layer_3_w2[0]=reg_layer_2_w2[0];
assign	layer_3_w2[1]=reg_layer_2_w2[1];
assign	layer_3_w2[2]=reg_layer_2_w2[2];
assign	layer_3_w3[0]=reg_layer_2_w3[0];
assign	layer_3_w3[1]=reg_layer_2_w3[1];
assign	layer_3_w4[0]=reg_layer_2_w4[0];
assign	layer_3_w4[1]=reg_layer_2_w4[1];
assign	layer_3_w5[0]=reg_layer_2_w5[0];
assign	layer_3_w5[1]=reg_layer_2_w5[1];
assign	layer_3_w6[0]=reg_layer_2_w6[0];
assign	layer_3_w6[1]=reg_layer_2_w6[1];
assign	layer_3_w7[0]=reg_layer_2_w7[0];
assign	layer_3_w7[1]=reg_layer_2_w7[1];
assign	layer_3_w8[0]=reg_layer_2_w8[0];
assign	layer_3_w8[1]=reg_layer_2_w8[1];
assign	layer_3_w9[0]=reg_layer_2_w9[0];
assign	layer_3_w9[1]=reg_layer_2_w9[1];
assign	layer_3_w10[0]=reg_layer_2_w10[0];
assign	layer_3_w10[1]=reg_layer_2_w10[1];
full_adder	layer_3_full_adder_0(
reg_layer_2_w11[0],
reg_layer_2_w11[1],
reg_layer_2_w11[2],
layer_3_w11[0],
layer_3_w12[0]
);
assign	layer_3_w11[1]=reg_layer_2_w11[3];
full_adder	layer_3_full_adder_1(
reg_layer_2_w12[0],
reg_layer_2_w12[1],
reg_layer_2_w12[2],
layer_3_w12[1],
layer_3_w13[0]
);
full_adder	layer_3_full_adder_2(
reg_layer_2_w13[0],
reg_layer_2_w13[1],
reg_layer_2_w13[2],
layer_3_w13[1],
layer_3_w14[0]
);
full_adder	layer_3_full_adder_3(
reg_layer_2_w14[0],
reg_layer_2_w14[1],
reg_layer_2_w14[2],
layer_3_w14[1],
layer_3_w15[0]
);
full_adder	layer_3_full_adder_4(
reg_layer_2_w15[0],
reg_layer_2_w15[1],
reg_layer_2_w15[2],
layer_3_w15[1],
layer_3_w16[0]
);
full_adder	layer_3_full_adder_5(
reg_layer_2_w16[0],
reg_layer_2_w16[1],
reg_layer_2_w16[2],
layer_3_w16[1],
layer_3_w17[0]
);
assign	layer_3_w16[2]=reg_layer_2_w16[3];
full_adder	layer_3_full_adder_6(
reg_layer_2_w17[0],
reg_layer_2_w17[1],
reg_layer_2_w17[2],
layer_3_w17[1],
layer_3_w18[0]
);
full_adder	layer_3_full_adder_7(
reg_layer_2_w18[0],
reg_layer_2_w18[1],
reg_layer_2_w18[2],
layer_3_w18[1],
layer_3_w19[0]
);
assign	layer_3_w18[2]=reg_layer_2_w18[3];
full_adder	layer_3_full_adder_8(
reg_layer_2_w19[0],
reg_layer_2_w19[1],
reg_layer_2_w19[2],
layer_3_w19[1],
layer_3_w20[0]
);
full_adder	layer_3_full_adder_9(
reg_layer_2_w20[0],
reg_layer_2_w20[1],
reg_layer_2_w20[2],
layer_3_w20[1],
layer_3_w21[0]
);
assign	layer_3_w20[2]=reg_layer_2_w20[3];
full_adder	layer_3_full_adder_10(
reg_layer_2_w21[0],
reg_layer_2_w21[1],
reg_layer_2_w21[2],
layer_3_w21[1],
layer_3_w22[0]
);
assign	layer_3_w22[1]=reg_layer_2_w22[0];
assign	layer_3_w22[2]=reg_layer_2_w22[1];
assign	layer_3_w23[0]=reg_layer_2_w23[0];
assign	layer_3_w23[1]=reg_layer_2_w23[1];
full_adder	layer_3_full_adder_11(
reg_layer_2_w24[0],
reg_layer_2_w24[1],
reg_layer_2_w24[2],
layer_3_w24,
layer_3_w25[0]
);
assign	layer_3_w25[1]=reg_layer_2_w25[0];
assign	layer_3_w25[2]=reg_layer_2_w25[1];
full_adder	layer_3_full_adder_12(
reg_layer_2_w26[0],
reg_layer_2_w26[1],
reg_layer_2_w26[2],
layer_3_w26,
layer_3_w27[0]
);
assign	layer_3_w27[1]=reg_layer_2_w27[0];
assign	layer_3_w27[2]=reg_layer_2_w27[1];
assign	layer_3_w28=reg_layer_2_w28;
full_adder	layer_3_full_adder_13(
reg_layer_2_w29[0],
reg_layer_2_w29[1],
reg_layer_2_w29[2],
layer_3_w29,
layer_3_w30[0]
);
assign	layer_3_w30[1]=reg_layer_2_w30;
assign	layer_3_w31=reg_layer_2_w31;

//layer 4
wire	layer_4_w0[1:0];
wire	layer_4_w1;
wire	layer_4_w2;
wire	layer_4_w3[1:0];
wire	layer_4_w4[1:0];
wire	layer_4_w5[1:0];
wire	layer_4_w6[1:0];
wire	layer_4_w7[1:0];
wire	layer_4_w8[1:0];
wire	layer_4_w9[1:0];
wire	layer_4_w10[1:0];
wire	layer_4_w11[1:0];
wire	layer_4_w12[1:0];
wire	layer_4_w13[1:0];
wire	layer_4_w14[1:0];
wire	layer_4_w15[1:0];
wire	layer_4_w16[1:0];
wire	layer_4_w17[1:0];
wire	layer_4_w18[1:0];
wire	layer_4_w19[1:0];
wire	layer_4_w20[1:0];
wire	layer_4_w21[1:0];
wire	layer_4_w22[1:0];
wire	layer_4_w23[1:0];
wire	layer_4_w24[1:0];
wire	layer_4_w25;
wire	layer_4_w26[1:0];
wire	layer_4_w27;
wire	layer_4_w28[1:0];
wire	layer_4_w29;
wire	layer_4_w30[1:0];
wire	layer_4_w31;
assign	layer_4_w0[0]=layer_3_w0[0];
assign	layer_4_w0[1]=layer_3_w0[1];
assign	layer_4_w1=layer_3_w1;
full_adder	layer_4_full_adder_0(
layer_3_w2[0],
layer_3_w2[1],
layer_3_w2[2],
layer_4_w2,
layer_4_w3[0]
);
half_adder	layer_4_half_adder_0(
layer_3_w3[0],
layer_3_w3[1],
layer_4_w3[1],
layer_4_w4[0]
);
half_adder	layer_4_half_adder_1(
layer_3_w4[0],
layer_3_w4[1],
layer_4_w4[1],
layer_4_w5[0]
);
half_adder	layer_4_half_adder_2(
layer_3_w5[0],
layer_3_w5[1],
layer_4_w5[1],
layer_4_w6[0]
);
half_adder	layer_4_half_adder_3(
layer_3_w6[0],
layer_3_w6[1],
layer_4_w6[1],
layer_4_w7[0]
);
half_adder	layer_4_half_adder_4(
layer_3_w7[0],
layer_3_w7[1],
layer_4_w7[1],
layer_4_w8[0]
);
half_adder	layer_4_half_adder_5(
layer_3_w8[0],
layer_3_w8[1],
layer_4_w8[1],
layer_4_w9[0]
);
half_adder	layer_4_half_adder_6(
layer_3_w9[0],
layer_3_w9[1],
layer_4_w9[1],
layer_4_w10[0]
);
half_adder	layer_4_half_adder_7(
layer_3_w10[0],
layer_3_w10[1],
layer_4_w10[1],
layer_4_w11[0]
);
half_adder	layer_4_half_adder_8(
layer_3_w11[0],
layer_3_w11[1],
layer_4_w11[1],
layer_4_w12[0]
);
half_adder	layer_4_half_adder_9(
layer_3_w12[0],
layer_3_w12[1],
layer_4_w12[1],
layer_4_w13[0]
);
half_adder	layer_4_half_adder_10(
layer_3_w13[0],
layer_3_w13[1],
layer_4_w13[1],
layer_4_w14[0]
);
half_adder	layer_4_half_adder_11(
layer_3_w14[0],
layer_3_w14[1],
layer_4_w14[1],
layer_4_w15[0]
);
half_adder	layer_4_half_adder_12(
layer_3_w15[0],
layer_3_w15[1],
layer_4_w15[1],
layer_4_w16[0]
);
full_adder	layer_4_full_adder_1(
layer_3_w16[0],
layer_3_w16[1],
layer_3_w16[2],
layer_4_w16[1],
layer_4_w17[0]
);
half_adder	layer_4_half_adder_13(
layer_3_w17[0],
layer_3_w17[1],
layer_4_w17[1],
layer_4_w18[0]
);
full_adder	layer_4_full_adder_2(
layer_3_w18[0],
layer_3_w18[1],
layer_3_w18[2],
layer_4_w18[1],
layer_4_w19[0]
);
half_adder	layer_4_half_adder_14(
layer_3_w19[0],
layer_3_w19[1],
layer_4_w19[1],
layer_4_w20[0]
);
full_adder	layer_4_full_adder_3(
layer_3_w20[0],
layer_3_w20[1],
layer_3_w20[2],
layer_4_w20[1],
layer_4_w21[0]
);
half_adder	layer_4_half_adder_15(
layer_3_w21[0],
layer_3_w21[1],
layer_4_w21[1],
layer_4_w22[0]
);
full_adder	layer_4_full_adder_4(
layer_3_w22[0],
layer_3_w22[1],
layer_3_w22[2],
layer_4_w22[1],
layer_4_w23[0]
);
half_adder	layer_4_half_adder_16(
layer_3_w23[0],
layer_3_w23[1],
layer_4_w23[1],
layer_4_w24[0]
);
assign	layer_4_w24[1]=layer_3_w24;
full_adder	layer_4_full_adder_5(
layer_3_w25[0],
layer_3_w25[1],
layer_3_w25[2],
layer_4_w25,
layer_4_w26[0]
);
assign	layer_4_w26[1]=layer_3_w26;
full_adder	layer_4_full_adder_6(
layer_3_w27[0],
layer_3_w27[1],
layer_3_w27[2],
layer_4_w27,
layer_4_w28[0]
);
assign	layer_4_w28[1]=layer_3_w28;
assign	layer_4_w29=layer_3_w29;
assign	layer_4_w30[0]=layer_3_w30[0];
assign	layer_4_w30[1]=layer_3_w30[1];
assign	layer_4_w31=layer_3_w31;

//group reduction results into 2 numbers
wire	[31:0]A,B;
assign	A[0]=layer_4_w0[0];
assign	B[0]=layer_4_w0[1];
assign	A[1]=layer_4_w1;
assign	B[1]=0;
assign	A[2]=layer_4_w2;
assign	B[2]=0;
assign	A[3]=layer_4_w3[0];
assign	B[3]=layer_4_w3[1];
assign	A[4]=layer_4_w4[0];
assign	B[4]=layer_4_w4[1];
assign	A[5]=layer_4_w5[0];
assign	B[5]=layer_4_w5[1];
assign	A[6]=layer_4_w6[0];
assign	B[6]=layer_4_w6[1];
assign	A[7]=layer_4_w7[0];
assign	B[7]=layer_4_w7[1];
assign	A[8]=layer_4_w8[0];
assign	B[8]=layer_4_w8[1];
assign	A[9]=layer_4_w9[0];
assign	B[9]=layer_4_w9[1];
assign	A[10]=layer_4_w10[0];
assign	B[10]=layer_4_w10[1];
assign	A[11]=layer_4_w11[0];
assign	B[11]=layer_4_w11[1];
assign	A[12]=layer_4_w12[0];
assign	B[12]=layer_4_w12[1];
assign	A[13]=layer_4_w13[0];
assign	B[13]=layer_4_w13[1];
assign	A[14]=layer_4_w14[0];
assign	B[14]=layer_4_w14[1];
assign	A[15]=layer_4_w15[0];
assign	B[15]=layer_4_w15[1];
assign	A[16]=layer_4_w16[0];
assign	B[16]=layer_4_w16[1];
assign	A[17]=layer_4_w17[0];
assign	B[17]=layer_4_w17[1];
assign	A[18]=layer_4_w18[0];
assign	B[18]=layer_4_w18[1];
assign	A[19]=layer_4_w19[0];
assign	B[19]=layer_4_w19[1];
assign	A[20]=layer_4_w20[0];
assign	B[20]=layer_4_w20[1];
assign	A[21]=layer_4_w21[0];
assign	B[21]=layer_4_w21[1];
assign	A[22]=layer_4_w22[0];
assign	B[22]=layer_4_w22[1];
assign	A[23]=layer_4_w23[0];
assign	B[23]=layer_4_w23[1];
assign	A[24]=layer_4_w24[0];
assign	B[24]=layer_4_w24[1];
assign	A[25]=layer_4_w25;
assign	B[25]=0;
assign	A[26]=layer_4_w26[0];
assign	B[26]=layer_4_w26[1];
assign	A[27]=layer_4_w27;
assign	B[27]=0;
assign	A[28]=layer_4_w28[0];
assign	B[28]=layer_4_w28[1];
assign	A[29]=layer_4_w29;
assign	B[29]=0;
assign	A[30]=layer_4_w30[0];
assign	B[30]=layer_4_w30[1];
assign	A[31]=layer_4_w31;
assign	B[31]=0;

wire	carry;
adder_32bit	adder_32bit(
A,
B,
1'b0,
o_product,
carry
);


endmodule
