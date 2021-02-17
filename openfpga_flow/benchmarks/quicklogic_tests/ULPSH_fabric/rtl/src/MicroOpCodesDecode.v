`timescale 1ns / 10ps
// algorithm file = 'ulpsh_s2_main_JB4_BMI160_AK09911_PD.alg'
// UlpshType = S2_1KDM

module decodeMicroOpCode (MicroOpCode, Signals);

	input	[8:0]	MicroOpCode;
	output	[45:0]	Signals;
	wire	[45:0]	Signals;

	assign Signals[0] = (MicroOpCode == 9'h00e) || 0;
	assign Signals[1] = (MicroOpCode == 9'h012) || (MicroOpCode == 9'h013) || 0;
	assign Signals[2] = 0;
	assign Signals[3] = 0;
	assign Signals[4] = (MicroOpCode == 9'h038) || 0;
	assign Signals[5] = (MicroOpCode == 9'h016) || (MicroOpCode == 9'h017) || (MicroOpCode == 9'h021) || (MicroOpCode == 9'h034) || (MicroOpCode == 9'h035) || (MicroOpCode == 9'h036) || (MicroOpCode == 9'h038) || (MicroOpCode == 9'h040) || (MicroOpCode == 9'h041) || (MicroOpCode == 9'h042) || (MicroOpCode == 9'h045) || (MicroOpCode == 9'h04a) || (MicroOpCode == 9'h04e) || (MicroOpCode == 9'h04f) || 0;
	assign Signals[6] = (MicroOpCode == 9'h00a) || (MicroOpCode == 9'h014) || (MicroOpCode == 9'h020) || (MicroOpCode == 9'h024) || (MicroOpCode == 9'h026) || (MicroOpCode == 9'h027) || (MicroOpCode == 9'h028) || (MicroOpCode == 9'h02d) || (MicroOpCode == 9'h033) || (MicroOpCode == 9'h037) || (MicroOpCode == 9'h038) || (MicroOpCode == 9'h039) || (MicroOpCode == 9'h03e) || (MicroOpCode == 9'h03f) || (MicroOpCode == 9'h044) || (MicroOpCode == 9'h047) || (MicroOpCode == 9'h049) || (MicroOpCode == 9'h04b) || (MicroOpCode == 9'h04c) || (MicroOpCode == 9'h050) || 0;
	assign Signals[7] = (MicroOpCode == 9'h00a) || (MicroOpCode == 9'h014) || (MicroOpCode == 9'h020) || (MicroOpCode == 9'h024) || (MicroOpCode == 9'h026) || (MicroOpCode == 9'h027) || (MicroOpCode == 9'h028) || (MicroOpCode == 9'h02d) || (MicroOpCode == 9'h033) || (MicroOpCode == 9'h035) || (MicroOpCode == 9'h036) || (MicroOpCode == 9'h037) || (MicroOpCode == 9'h038) || (MicroOpCode == 9'h039) || (MicroOpCode == 9'h03e) || (MicroOpCode == 9'h03f) || (MicroOpCode == 9'h044) || (MicroOpCode == 9'h049) || (MicroOpCode == 9'h04a) || (MicroOpCode == 9'h050) || 0;
	assign Signals[8] = (MicroOpCode == 9'h00a) || (MicroOpCode == 9'h024) || (MicroOpCode == 9'h026) || (MicroOpCode == 9'h033) || (MicroOpCode == 9'h037) || (MicroOpCode == 9'h038) || (MicroOpCode == 9'h03b) || (MicroOpCode == 9'h03f) || (MicroOpCode == 9'h043) || (MicroOpCode == 9'h047) || (MicroOpCode == 9'h049) || (MicroOpCode == 9'h04a) || (MicroOpCode == 9'h04b) || (MicroOpCode == 9'h04c) || 0;
	assign Signals[9] = (MicroOpCode == 9'h027) || (MicroOpCode == 9'h028) || (MicroOpCode == 9'h039) || (MicroOpCode == 9'h03b) || (MicroOpCode == 9'h03e) || (MicroOpCode == 9'h043) || (MicroOpCode == 9'h044) || (MicroOpCode == 9'h047) || (MicroOpCode == 9'h04b) || (MicroOpCode == 9'h04c) || 0;
	assign Signals[10] = (MicroOpCode == 9'h007) || (MicroOpCode == 9'h008) || (MicroOpCode == 9'h009) || (MicroOpCode == 9'h00a) || (MicroOpCode == 9'h00d) || (MicroOpCode == 9'h00e) || (MicroOpCode == 9'h010) || (MicroOpCode == 9'h012) || (MicroOpCode == 9'h013) || (MicroOpCode == 9'h014) || (MicroOpCode == 9'h020) || (MicroOpCode == 9'h022) || (MicroOpCode == 9'h024) || (MicroOpCode == 9'h025) || (MicroOpCode == 9'h026) || (MicroOpCode == 9'h036) || (MicroOpCode == 9'h038) || (MicroOpCode == 9'h03a) || (MicroOpCode == 9'h03c) || (MicroOpCode == 9'h03d) || (MicroOpCode == 9'h03f) || (MicroOpCode == 9'h040) || (MicroOpCode == 9'h041) || (MicroOpCode == 9'h042) || (MicroOpCode == 9'h046) || (MicroOpCode == 9'h047) || (MicroOpCode == 9'h049) || (MicroOpCode == 9'h04a) || (MicroOpCode == 9'h04e) || (MicroOpCode == 9'h04f) || (MicroOpCode == 9'h050) || 0;
	assign Signals[11] = (MicroOpCode == 9'h037) || (MicroOpCode == 9'h049) || 0;
	assign Signals[12] = (MicroOpCode == 9'h03b) || (MicroOpCode == 9'h043) || (MicroOpCode == 9'h047) || (MicroOpCode == 9'h04b) || (MicroOpCode == 9'h04c) || 0;
	assign Signals[13] = (MicroOpCode == 9'h027) || (MicroOpCode == 9'h028) || (MicroOpCode == 9'h039) || (MicroOpCode == 9'h03b) || (MicroOpCode == 9'h03e) || (MicroOpCode == 9'h043) || (MicroOpCode == 9'h044) || (MicroOpCode == 9'h047) || (MicroOpCode == 9'h04b) || (MicroOpCode == 9'h04c) || 0;
	assign Signals[14] = 0;
	assign Signals[15] = (MicroOpCode == 9'h038) || (MicroOpCode == 9'h039) || (MicroOpCode == 9'h04a) || 0;
	assign Signals[16] = (MicroOpCode == 9'h034) || (MicroOpCode == 9'h035) || (MicroOpCode == 9'h048) || 0;
	assign Signals[17] = (MicroOpCode == 9'h00f) || (MicroOpCode == 9'h011) || (MicroOpCode == 9'h015) || (MicroOpCode == 9'h01c) || (MicroOpCode == 9'h029) || 0;
	assign Signals[18] = (MicroOpCode == 9'h00b) || (MicroOpCode == 9'h00f) || (MicroOpCode == 9'h01f) || (MicroOpCode == 9'h029) || 0;
	assign Signals[19] = (MicroOpCode == 9'h015) || (MicroOpCode == 9'h01f) || (MicroOpCode == 9'h029) || (MicroOpCode == 9'h031) || 0;
	assign Signals[20] = (MicroOpCode == 9'h001) || (MicroOpCode == 9'h002) || (MicroOpCode == 9'h009) || (MicroOpCode == 9'h00c) || (MicroOpCode == 9'h016) || (MicroOpCode == 9'h020) || (MicroOpCode == 9'h025) || (MicroOpCode == 9'h026) || (MicroOpCode == 9'h02c) || (MicroOpCode == 9'h02d) || (MicroOpCode == 9'h032) || (MicroOpCode == 9'h033) || (MicroOpCode == 9'h034) || (MicroOpCode == 9'h035) || (MicroOpCode == 9'h037) || (MicroOpCode == 9'h038) || (MicroOpCode == 9'h03c) || (MicroOpCode == 9'h03f) || (MicroOpCode == 9'h042) || (MicroOpCode == 9'h043) || (MicroOpCode == 9'h044) || (MicroOpCode == 9'h047) || (MicroOpCode == 9'h048) || (MicroOpCode == 9'h049) || (MicroOpCode == 9'h04a) || (MicroOpCode == 9'h04b) || (MicroOpCode == 9'h04d) || (MicroOpCode == 9'h04f) || 0;
	assign Signals[21] = (MicroOpCode == 9'h004) || (MicroOpCode == 9'h008) || (MicroOpCode == 9'h009) || (MicroOpCode == 9'h00c) || (MicroOpCode == 9'h010) || (MicroOpCode == 9'h013) || (MicroOpCode == 9'h016) || (MicroOpCode == 9'h019) || (MicroOpCode == 9'h01b) || (MicroOpCode == 9'h020) || (MicroOpCode == 9'h023) || (MicroOpCode == 9'h026) || (MicroOpCode == 9'h028) || (MicroOpCode == 9'h02a) || (MicroOpCode == 9'h02c) || (MicroOpCode == 9'h02d) || (MicroOpCode == 9'h02e) || (MicroOpCode == 9'h030) || (MicroOpCode == 9'h032) || (MicroOpCode == 9'h033) || (MicroOpCode == 9'h034) || (MicroOpCode == 9'h035) || (MicroOpCode == 9'h037) || (MicroOpCode == 9'h038) || (MicroOpCode == 9'h03a) || (MicroOpCode == 9'h03c) || (MicroOpCode == 9'h03d) || (MicroOpCode == 9'h03f) || (MicroOpCode == 9'h041) || (MicroOpCode == 9'h042) || (MicroOpCode == 9'h043) || (MicroOpCode == 9'h044) || (MicroOpCode == 9'h045) || (MicroOpCode == 9'h047) || (MicroOpCode == 9'h048) || (MicroOpCode == 9'h049) || (MicroOpCode == 9'h04a) || (MicroOpCode == 9'h04b) || (MicroOpCode == 9'h04d) || (MicroOpCode == 9'h04f) || (MicroOpCode == 9'h050) || 0;
	assign Signals[22] = (MicroOpCode == 9'h002) || (MicroOpCode == 9'h003) || (MicroOpCode == 9'h007) || (MicroOpCode == 9'h008) || (MicroOpCode == 9'h009) || (MicroOpCode == 9'h00d) || (MicroOpCode == 9'h00e) || (MicroOpCode == 9'h010) || (MicroOpCode == 9'h012) || (MicroOpCode == 9'h013) || (MicroOpCode == 9'h014) || (MicroOpCode == 9'h016) || (MicroOpCode == 9'h017) || (MicroOpCode == 9'h020) || (MicroOpCode == 9'h022) || (MicroOpCode == 9'h024) || (MicroOpCode == 9'h025) || (MicroOpCode == 9'h027) || (MicroOpCode == 9'h028) || (MicroOpCode == 9'h02a) || (MicroOpCode == 9'h02b) || (MicroOpCode == 9'h02c) || (MicroOpCode == 9'h02d) || (MicroOpCode == 9'h02e) || (MicroOpCode == 9'h02f) || (MicroOpCode == 9'h030) || (MicroOpCode == 9'h032) || (MicroOpCode == 9'h033) || (MicroOpCode == 9'h036) || (MicroOpCode == 9'h037) || (MicroOpCode == 9'h039) || (MicroOpCode == 9'h03a) || (MicroOpCode == 9'h03b) || (MicroOpCode == 9'h03c) || (MicroOpCode == 9'h03d) || (MicroOpCode == 9'h03e) || (MicroOpCode == 9'h03f) || (MicroOpCode == 9'h040) || (MicroOpCode == 9'h041) || (MicroOpCode == 9'h042) || (MicroOpCode == 9'h043) || (MicroOpCode == 9'h044) || (MicroOpCode == 9'h045) || (MicroOpCode == 9'h047) || (MicroOpCode == 9'h049) || (MicroOpCode == 9'h04b) || (MicroOpCode == 9'h04c) || (MicroOpCode == 9'h04e) || (MicroOpCode == 9'h04f) || (MicroOpCode == 9'h050) || 0;
	assign Signals[23] = 0;
	assign Signals[24] = 0;
	assign Signals[25] = (MicroOpCode == 9'h021) || 0;
	assign Signals[26] = (MicroOpCode == 9'h021) || 0;
	assign Signals[27] = (MicroOpCode == 9'h002) || (MicroOpCode == 9'h003) || 0;
	assign Signals[28] = (MicroOpCode == 9'h04d) || 0;
	assign Signals[29] = (MicroOpCode == 9'h023) || (MicroOpCode == 9'h02e) || (MicroOpCode == 9'h03d) || (MicroOpCode == 9'h04d) || 0;
	assign Signals[30] = (MicroOpCode == 9'h018) || (MicroOpCode == 9'h019) || (MicroOpCode == 9'h01a) || (MicroOpCode == 9'h01b) || 0;
	assign Signals[31] = (MicroOpCode == 9'h006) || 0;
	assign Signals[32] = (MicroOpCode == 9'h01e) || 0;
	assign Signals[33] = (MicroOpCode == 9'h039) || (MicroOpCode == 9'h03e) || (MicroOpCode == 9'h044) || (MicroOpCode == 9'h04b) || (MicroOpCode == 9'h04c) || 0;
	assign Signals[34] = (MicroOpCode == 9'h022) || (MicroOpCode == 9'h02a) || (MicroOpCode == 9'h02b) || (MicroOpCode == 9'h02c) || (MicroOpCode == 9'h03a) || (MicroOpCode == 9'h03c) || (MicroOpCode == 9'h03d) || (MicroOpCode == 9'h04e) || (MicroOpCode == 9'h04f) || 0;
	assign Signals[35] = (MicroOpCode == 9'h027) || (MicroOpCode == 9'h028) || 0;
	assign Signals[36] = (MicroOpCode == 9'h01d) || 0;
	assign Signals[37] = (MicroOpCode == 9'h018) || (MicroOpCode == 9'h019) || 0;
	assign Signals[38] = (MicroOpCode == 9'h00d) || (MicroOpCode == 9'h00e) || (MicroOpCode == 9'h010) || 0;
	assign Signals[39] = 0;
	assign Signals[40] = (MicroOpCode == 9'h005) || 0;
	assign Signals[41] = 0;
	assign Signals[42] = 0;
	assign Signals[43] = 0;
	assign Signals[44] = (MicroOpCode == 9'h01c) || 0;
	assign Signals[45] = 0;

endmodule

