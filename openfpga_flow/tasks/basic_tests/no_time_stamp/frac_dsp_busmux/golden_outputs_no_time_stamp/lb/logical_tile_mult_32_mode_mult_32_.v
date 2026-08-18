//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: mult_32
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Physical programmable logic block Verilog module: mult_32 -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_mult_32_mode_mult_32_ -----
module logical_tile_mult_32_mode_mult_32_(pReset,
                                          mult_32_a,
                                          mult_32_b,
                                          enable,
                                          address,
                                          data_in,
                                          mult_32_out);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:31] mult_32_a;
//----- INPUT PORTS -----
input [0:31] mult_32_b;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:2] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:63] mult_32_out;

//----- BEGIN wire-connection ports -----
wire [0:31] mult_32_a;
wire [0:31] mult_32_b;
wire [0:63] mult_32_out;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] direct_interc_100_out;
wire [0:0] direct_interc_101_out;
wire [0:0] direct_interc_102_out;
wire [0:0] direct_interc_103_out;
wire [0:0] direct_interc_104_out;
wire [0:0] direct_interc_105_out;
wire [0:0] direct_interc_106_out;
wire [0:0] direct_interc_107_out;
wire [0:0] direct_interc_108_out;
wire [0:0] direct_interc_109_out;
wire [0:0] direct_interc_110_out;
wire [0:0] direct_interc_111_out;
wire [0:0] direct_interc_112_out;
wire [0:0] direct_interc_113_out;
wire [0:0] direct_interc_114_out;
wire [0:0] direct_interc_115_out;
wire [0:0] direct_interc_116_out;
wire [0:0] direct_interc_117_out;
wire [0:0] direct_interc_118_out;
wire [0:0] direct_interc_119_out;
wire [0:0] direct_interc_120_out;
wire [0:0] direct_interc_121_out;
wire [0:0] direct_interc_122_out;
wire [0:0] direct_interc_123_out;
wire [0:0] direct_interc_124_out;
wire [0:0] direct_interc_125_out;
wire [0:0] direct_interc_126_out;
wire [0:0] direct_interc_127_out;
wire [0:0] direct_interc_64_out;
wire [0:0] direct_interc_65_out;
wire [0:0] direct_interc_66_out;
wire [0:0] direct_interc_67_out;
wire [0:0] direct_interc_68_out;
wire [0:0] direct_interc_69_out;
wire [0:0] direct_interc_70_out;
wire [0:0] direct_interc_71_out;
wire [0:0] direct_interc_72_out;
wire [0:0] direct_interc_73_out;
wire [0:0] direct_interc_74_out;
wire [0:0] direct_interc_75_out;
wire [0:0] direct_interc_76_out;
wire [0:0] direct_interc_77_out;
wire [0:0] direct_interc_78_out;
wire [0:0] direct_interc_79_out;
wire [0:0] direct_interc_80_out;
wire [0:0] direct_interc_81_out;
wire [0:0] direct_interc_82_out;
wire [0:0] direct_interc_83_out;
wire [0:0] direct_interc_84_out;
wire [0:0] direct_interc_85_out;
wire [0:0] direct_interc_86_out;
wire [0:0] direct_interc_87_out;
wire [0:0] direct_interc_88_out;
wire [0:0] direct_interc_89_out;
wire [0:0] direct_interc_90_out;
wire [0:0] direct_interc_91_out;
wire [0:0] direct_interc_92_out;
wire [0:0] direct_interc_93_out;
wire [0:0] direct_interc_94_out;
wire [0:0] direct_interc_95_out;
wire [0:0] direct_interc_96_out;
wire [0:0] direct_interc_97_out;
wire [0:0] direct_interc_98_out;
wire [0:0] direct_interc_99_out;
wire [0:63] logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0 (
		.pReset(pReset),
		.mult_32x32_slice_A_cfg({direct_interc_64_out, direct_interc_65_out, direct_interc_66_out, direct_interc_67_out, direct_interc_68_out, direct_interc_69_out, direct_interc_70_out, direct_interc_71_out, direct_interc_72_out, direct_interc_73_out, direct_interc_74_out, direct_interc_75_out, direct_interc_76_out, direct_interc_77_out, direct_interc_78_out, direct_interc_79_out, direct_interc_80_out, direct_interc_81_out, direct_interc_82_out, direct_interc_83_out, direct_interc_84_out, direct_interc_85_out, direct_interc_86_out, direct_interc_87_out, direct_interc_88_out, direct_interc_89_out, direct_interc_90_out, direct_interc_91_out, direct_interc_92_out, direct_interc_93_out, direct_interc_94_out, direct_interc_95_out}),
		.mult_32x32_slice_B_cfg({direct_interc_96_out, direct_interc_97_out, direct_interc_98_out, direct_interc_99_out, direct_interc_100_out, direct_interc_101_out, direct_interc_102_out, direct_interc_103_out, direct_interc_104_out, direct_interc_105_out, direct_interc_106_out, direct_interc_107_out, direct_interc_108_out, direct_interc_109_out, direct_interc_110_out, direct_interc_111_out, direct_interc_112_out, direct_interc_113_out, direct_interc_114_out, direct_interc_115_out, direct_interc_116_out, direct_interc_117_out, direct_interc_118_out, direct_interc_119_out, direct_interc_120_out, direct_interc_121_out, direct_interc_122_out, direct_interc_123_out, direct_interc_124_out, direct_interc_125_out, direct_interc_126_out, direct_interc_127_out}),
		.enable(enable),
		.address(address[0:2]),
		.data_in(data_in),
		.mult_32x32_slice_OUT_cfg(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[0:63]));

	direct_interc direct_interc_0_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[0]),
		.out(mult_32_out[0]));

	direct_interc direct_interc_1_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[1]),
		.out(mult_32_out[1]));

	direct_interc direct_interc_2_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[2]),
		.out(mult_32_out[2]));

	direct_interc direct_interc_3_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[3]),
		.out(mult_32_out[3]));

	direct_interc direct_interc_4_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[4]),
		.out(mult_32_out[4]));

	direct_interc direct_interc_5_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[5]),
		.out(mult_32_out[5]));

	direct_interc direct_interc_6_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[6]),
		.out(mult_32_out[6]));

	direct_interc direct_interc_7_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[7]),
		.out(mult_32_out[7]));

	direct_interc direct_interc_8_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[8]),
		.out(mult_32_out[8]));

	direct_interc direct_interc_9_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[9]),
		.out(mult_32_out[9]));

	direct_interc direct_interc_10_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[10]),
		.out(mult_32_out[10]));

	direct_interc direct_interc_11_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[11]),
		.out(mult_32_out[11]));

	direct_interc direct_interc_12_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[12]),
		.out(mult_32_out[12]));

	direct_interc direct_interc_13_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[13]),
		.out(mult_32_out[13]));

	direct_interc direct_interc_14_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[14]),
		.out(mult_32_out[14]));

	direct_interc direct_interc_15_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[15]),
		.out(mult_32_out[15]));

	direct_interc direct_interc_16_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[16]),
		.out(mult_32_out[16]));

	direct_interc direct_interc_17_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[17]),
		.out(mult_32_out[17]));

	direct_interc direct_interc_18_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[18]),
		.out(mult_32_out[18]));

	direct_interc direct_interc_19_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[19]),
		.out(mult_32_out[19]));

	direct_interc direct_interc_20_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[20]),
		.out(mult_32_out[20]));

	direct_interc direct_interc_21_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[21]),
		.out(mult_32_out[21]));

	direct_interc direct_interc_22_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[22]),
		.out(mult_32_out[22]));

	direct_interc direct_interc_23_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[23]),
		.out(mult_32_out[23]));

	direct_interc direct_interc_24_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[24]),
		.out(mult_32_out[24]));

	direct_interc direct_interc_25_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[25]),
		.out(mult_32_out[25]));

	direct_interc direct_interc_26_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[26]),
		.out(mult_32_out[26]));

	direct_interc direct_interc_27_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[27]),
		.out(mult_32_out[27]));

	direct_interc direct_interc_28_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[28]),
		.out(mult_32_out[28]));

	direct_interc direct_interc_29_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[29]),
		.out(mult_32_out[29]));

	direct_interc direct_interc_30_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[30]),
		.out(mult_32_out[30]));

	direct_interc direct_interc_31_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[31]),
		.out(mult_32_out[31]));

	direct_interc direct_interc_32_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[32]),
		.out(mult_32_out[32]));

	direct_interc direct_interc_33_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[33]),
		.out(mult_32_out[33]));

	direct_interc direct_interc_34_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[34]),
		.out(mult_32_out[34]));

	direct_interc direct_interc_35_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[35]),
		.out(mult_32_out[35]));

	direct_interc direct_interc_36_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[36]),
		.out(mult_32_out[36]));

	direct_interc direct_interc_37_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[37]),
		.out(mult_32_out[37]));

	direct_interc direct_interc_38_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[38]),
		.out(mult_32_out[38]));

	direct_interc direct_interc_39_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[39]),
		.out(mult_32_out[39]));

	direct_interc direct_interc_40_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[40]),
		.out(mult_32_out[40]));

	direct_interc direct_interc_41_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[41]),
		.out(mult_32_out[41]));

	direct_interc direct_interc_42_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[42]),
		.out(mult_32_out[42]));

	direct_interc direct_interc_43_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[43]),
		.out(mult_32_out[43]));

	direct_interc direct_interc_44_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[44]),
		.out(mult_32_out[44]));

	direct_interc direct_interc_45_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[45]),
		.out(mult_32_out[45]));

	direct_interc direct_interc_46_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[46]),
		.out(mult_32_out[46]));

	direct_interc direct_interc_47_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[47]),
		.out(mult_32_out[47]));

	direct_interc direct_interc_48_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[48]),
		.out(mult_32_out[48]));

	direct_interc direct_interc_49_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[49]),
		.out(mult_32_out[49]));

	direct_interc direct_interc_50_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[50]),
		.out(mult_32_out[50]));

	direct_interc direct_interc_51_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[51]),
		.out(mult_32_out[51]));

	direct_interc direct_interc_52_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[52]),
		.out(mult_32_out[52]));

	direct_interc direct_interc_53_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[53]),
		.out(mult_32_out[53]));

	direct_interc direct_interc_54_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[54]),
		.out(mult_32_out[54]));

	direct_interc direct_interc_55_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[55]),
		.out(mult_32_out[55]));

	direct_interc direct_interc_56_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[56]),
		.out(mult_32_out[56]));

	direct_interc direct_interc_57_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[57]),
		.out(mult_32_out[57]));

	direct_interc direct_interc_58_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[58]),
		.out(mult_32_out[58]));

	direct_interc direct_interc_59_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[59]),
		.out(mult_32_out[59]));

	direct_interc direct_interc_60_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[60]),
		.out(mult_32_out[60]));

	direct_interc direct_interc_61_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[61]),
		.out(mult_32_out[61]));

	direct_interc direct_interc_62_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[62]),
		.out(mult_32_out[62]));

	direct_interc direct_interc_63_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_0_mult_32x32_slice_OUT_cfg[63]),
		.out(mult_32_out[63]));

	direct_interc direct_interc_64_ (
		.in(mult_32_a[0]),
		.out(direct_interc_64_out));

	direct_interc direct_interc_65_ (
		.in(mult_32_a[1]),
		.out(direct_interc_65_out));

	direct_interc direct_interc_66_ (
		.in(mult_32_a[2]),
		.out(direct_interc_66_out));

	direct_interc direct_interc_67_ (
		.in(mult_32_a[3]),
		.out(direct_interc_67_out));

	direct_interc direct_interc_68_ (
		.in(mult_32_a[4]),
		.out(direct_interc_68_out));

	direct_interc direct_interc_69_ (
		.in(mult_32_a[5]),
		.out(direct_interc_69_out));

	direct_interc direct_interc_70_ (
		.in(mult_32_a[6]),
		.out(direct_interc_70_out));

	direct_interc direct_interc_71_ (
		.in(mult_32_a[7]),
		.out(direct_interc_71_out));

	direct_interc direct_interc_72_ (
		.in(mult_32_a[8]),
		.out(direct_interc_72_out));

	direct_interc direct_interc_73_ (
		.in(mult_32_a[9]),
		.out(direct_interc_73_out));

	direct_interc direct_interc_74_ (
		.in(mult_32_a[10]),
		.out(direct_interc_74_out));

	direct_interc direct_interc_75_ (
		.in(mult_32_a[11]),
		.out(direct_interc_75_out));

	direct_interc direct_interc_76_ (
		.in(mult_32_a[12]),
		.out(direct_interc_76_out));

	direct_interc direct_interc_77_ (
		.in(mult_32_a[13]),
		.out(direct_interc_77_out));

	direct_interc direct_interc_78_ (
		.in(mult_32_a[14]),
		.out(direct_interc_78_out));

	direct_interc direct_interc_79_ (
		.in(mult_32_a[15]),
		.out(direct_interc_79_out));

	direct_interc direct_interc_80_ (
		.in(mult_32_a[16]),
		.out(direct_interc_80_out));

	direct_interc direct_interc_81_ (
		.in(mult_32_a[17]),
		.out(direct_interc_81_out));

	direct_interc direct_interc_82_ (
		.in(mult_32_a[18]),
		.out(direct_interc_82_out));

	direct_interc direct_interc_83_ (
		.in(mult_32_a[19]),
		.out(direct_interc_83_out));

	direct_interc direct_interc_84_ (
		.in(mult_32_a[20]),
		.out(direct_interc_84_out));

	direct_interc direct_interc_85_ (
		.in(mult_32_a[21]),
		.out(direct_interc_85_out));

	direct_interc direct_interc_86_ (
		.in(mult_32_a[22]),
		.out(direct_interc_86_out));

	direct_interc direct_interc_87_ (
		.in(mult_32_a[23]),
		.out(direct_interc_87_out));

	direct_interc direct_interc_88_ (
		.in(mult_32_a[24]),
		.out(direct_interc_88_out));

	direct_interc direct_interc_89_ (
		.in(mult_32_a[25]),
		.out(direct_interc_89_out));

	direct_interc direct_interc_90_ (
		.in(mult_32_a[26]),
		.out(direct_interc_90_out));

	direct_interc direct_interc_91_ (
		.in(mult_32_a[27]),
		.out(direct_interc_91_out));

	direct_interc direct_interc_92_ (
		.in(mult_32_a[28]),
		.out(direct_interc_92_out));

	direct_interc direct_interc_93_ (
		.in(mult_32_a[29]),
		.out(direct_interc_93_out));

	direct_interc direct_interc_94_ (
		.in(mult_32_a[30]),
		.out(direct_interc_94_out));

	direct_interc direct_interc_95_ (
		.in(mult_32_a[31]),
		.out(direct_interc_95_out));

	direct_interc direct_interc_96_ (
		.in(mult_32_b[0]),
		.out(direct_interc_96_out));

	direct_interc direct_interc_97_ (
		.in(mult_32_b[1]),
		.out(direct_interc_97_out));

	direct_interc direct_interc_98_ (
		.in(mult_32_b[2]),
		.out(direct_interc_98_out));

	direct_interc direct_interc_99_ (
		.in(mult_32_b[3]),
		.out(direct_interc_99_out));

	direct_interc direct_interc_100_ (
		.in(mult_32_b[4]),
		.out(direct_interc_100_out));

	direct_interc direct_interc_101_ (
		.in(mult_32_b[5]),
		.out(direct_interc_101_out));

	direct_interc direct_interc_102_ (
		.in(mult_32_b[6]),
		.out(direct_interc_102_out));

	direct_interc direct_interc_103_ (
		.in(mult_32_b[7]),
		.out(direct_interc_103_out));

	direct_interc direct_interc_104_ (
		.in(mult_32_b[8]),
		.out(direct_interc_104_out));

	direct_interc direct_interc_105_ (
		.in(mult_32_b[9]),
		.out(direct_interc_105_out));

	direct_interc direct_interc_106_ (
		.in(mult_32_b[10]),
		.out(direct_interc_106_out));

	direct_interc direct_interc_107_ (
		.in(mult_32_b[11]),
		.out(direct_interc_107_out));

	direct_interc direct_interc_108_ (
		.in(mult_32_b[12]),
		.out(direct_interc_108_out));

	direct_interc direct_interc_109_ (
		.in(mult_32_b[13]),
		.out(direct_interc_109_out));

	direct_interc direct_interc_110_ (
		.in(mult_32_b[14]),
		.out(direct_interc_110_out));

	direct_interc direct_interc_111_ (
		.in(mult_32_b[15]),
		.out(direct_interc_111_out));

	direct_interc direct_interc_112_ (
		.in(mult_32_b[16]),
		.out(direct_interc_112_out));

	direct_interc direct_interc_113_ (
		.in(mult_32_b[17]),
		.out(direct_interc_113_out));

	direct_interc direct_interc_114_ (
		.in(mult_32_b[18]),
		.out(direct_interc_114_out));

	direct_interc direct_interc_115_ (
		.in(mult_32_b[19]),
		.out(direct_interc_115_out));

	direct_interc direct_interc_116_ (
		.in(mult_32_b[20]),
		.out(direct_interc_116_out));

	direct_interc direct_interc_117_ (
		.in(mult_32_b[21]),
		.out(direct_interc_117_out));

	direct_interc direct_interc_118_ (
		.in(mult_32_b[22]),
		.out(direct_interc_118_out));

	direct_interc direct_interc_119_ (
		.in(mult_32_b[23]),
		.out(direct_interc_119_out));

	direct_interc direct_interc_120_ (
		.in(mult_32_b[24]),
		.out(direct_interc_120_out));

	direct_interc direct_interc_121_ (
		.in(mult_32_b[25]),
		.out(direct_interc_121_out));

	direct_interc direct_interc_122_ (
		.in(mult_32_b[26]),
		.out(direct_interc_122_out));

	direct_interc direct_interc_123_ (
		.in(mult_32_b[27]),
		.out(direct_interc_123_out));

	direct_interc direct_interc_124_ (
		.in(mult_32_b[28]),
		.out(direct_interc_124_out));

	direct_interc direct_interc_125_ (
		.in(mult_32_b[29]),
		.out(direct_interc_125_out));

	direct_interc direct_interc_126_ (
		.in(mult_32_b[30]),
		.out(direct_interc_126_out));

	direct_interc direct_interc_127_ (
		.in(mult_32_b[31]),
		.out(direct_interc_127_out));

endmodule
// ----- END Verilog module for logical_tile_mult_32_mode_mult_32_ -----

//----- Default net type -----
`default_nettype wire



// ----- END Physical programmable logic block Verilog module: mult_32 -----
