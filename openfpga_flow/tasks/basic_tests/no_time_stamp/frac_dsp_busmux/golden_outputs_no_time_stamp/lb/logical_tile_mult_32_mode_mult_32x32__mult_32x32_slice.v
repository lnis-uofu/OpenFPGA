//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for pb_type: mult_32x32_slice
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Physical programmable logic block Verilog module: mult_32x32_slice -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice -----
module logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice(pReset,
                                                              mult_32x32_slice_A_cfg,
                                                              mult_32x32_slice_B_cfg,
                                                              enable,
                                                              address,
                                                              data_in,
                                                              mult_32x32_slice_OUT_cfg);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:31] mult_32x32_slice_A_cfg;
//----- INPUT PORTS -----
input [0:31] mult_32x32_slice_B_cfg;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:2] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:63] mult_32x32_slice_OUT_cfg;

//----- BEGIN wire-connection ports -----
wire [0:31] mult_32x32_slice_A_cfg;
wire [0:31] mult_32x32_slice_B_cfg;
wire [0:63] mult_32x32_slice_OUT_cfg;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:1] decoder1to2_0_data_out;
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
wire [0:63] logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out;
wire [0:0] mux_1level_tapbuf_size2_0_out;
wire [0:0] mux_1level_tapbuf_size2_10_out;
wire [0:0] mux_1level_tapbuf_size2_11_out;
wire [0:0] mux_1level_tapbuf_size2_12_out;
wire [0:0] mux_1level_tapbuf_size2_13_out;
wire [0:0] mux_1level_tapbuf_size2_14_out;
wire [0:0] mux_1level_tapbuf_size2_15_out;
wire [0:0] mux_1level_tapbuf_size2_16_out;
wire [0:0] mux_1level_tapbuf_size2_17_out;
wire [0:0] mux_1level_tapbuf_size2_18_out;
wire [0:0] mux_1level_tapbuf_size2_19_out;
wire [0:0] mux_1level_tapbuf_size2_1_out;
wire [0:0] mux_1level_tapbuf_size2_20_out;
wire [0:0] mux_1level_tapbuf_size2_21_out;
wire [0:0] mux_1level_tapbuf_size2_22_out;
wire [0:0] mux_1level_tapbuf_size2_23_out;
wire [0:0] mux_1level_tapbuf_size2_24_out;
wire [0:0] mux_1level_tapbuf_size2_25_out;
wire [0:0] mux_1level_tapbuf_size2_26_out;
wire [0:0] mux_1level_tapbuf_size2_27_out;
wire [0:0] mux_1level_tapbuf_size2_28_out;
wire [0:0] mux_1level_tapbuf_size2_29_out;
wire [0:0] mux_1level_tapbuf_size2_2_out;
wire [0:0] mux_1level_tapbuf_size2_30_out;
wire [0:0] mux_1level_tapbuf_size2_31_out;
wire [0:0] mux_1level_tapbuf_size2_3_out;
wire [0:0] mux_1level_tapbuf_size2_4_out;
wire [0:0] mux_1level_tapbuf_size2_5_out;
wire [0:0] mux_1level_tapbuf_size2_6_out;
wire [0:0] mux_1level_tapbuf_size2_7_out;
wire [0:0] mux_1level_tapbuf_size2_8_out;
wire [0:0] mux_1level_tapbuf_size2_9_out;
wire [0:0] mux_1level_tapbuf_size2_mem_0_mem_out_0;
wire [1:1] mux_1level_tapbuf_size2_mem_0_mem_out_1;
wire [2:2] mux_1level_tapbuf_size2_mem_0_mem_out_2;
wire [0:0] mux_1level_tapbuf_size2_mem_0_mem_outb_0;
wire [1:1] mux_1level_tapbuf_size2_mem_0_mem_outb_1;
wire [2:2] mux_1level_tapbuf_size2_mem_0_mem_outb_2;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32 logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0 (
		.pReset(pReset),
		.mult_32x32_a({mux_1level_tapbuf_size2_0_out, mux_1level_tapbuf_size2_1_out, mux_1level_tapbuf_size2_2_out, mux_1level_tapbuf_size2_3_out, mux_1level_tapbuf_size2_4_out, mux_1level_tapbuf_size2_5_out, mux_1level_tapbuf_size2_6_out, mux_1level_tapbuf_size2_7_out, mux_1level_tapbuf_size2_8_out, mux_1level_tapbuf_size2_9_out, mux_1level_tapbuf_size2_10_out, mux_1level_tapbuf_size2_11_out, mux_1level_tapbuf_size2_12_out, mux_1level_tapbuf_size2_13_out, mux_1level_tapbuf_size2_14_out, mux_1level_tapbuf_size2_15_out, mux_1level_tapbuf_size2_16_out, mux_1level_tapbuf_size2_17_out, mux_1level_tapbuf_size2_18_out, mux_1level_tapbuf_size2_19_out, mux_1level_tapbuf_size2_20_out, mux_1level_tapbuf_size2_21_out, mux_1level_tapbuf_size2_22_out, mux_1level_tapbuf_size2_23_out, mux_1level_tapbuf_size2_24_out, mux_1level_tapbuf_size2_25_out, mux_1level_tapbuf_size2_26_out, mux_1level_tapbuf_size2_27_out, mux_1level_tapbuf_size2_28_out, mux_1level_tapbuf_size2_29_out, mux_1level_tapbuf_size2_30_out, mux_1level_tapbuf_size2_31_out}),
		.mult_32x32_b({direct_interc_64_out, direct_interc_65_out, direct_interc_66_out, direct_interc_67_out, direct_interc_68_out, direct_interc_69_out, direct_interc_70_out, direct_interc_71_out, direct_interc_72_out, direct_interc_73_out, direct_interc_74_out, direct_interc_75_out, direct_interc_76_out, direct_interc_77_out, direct_interc_78_out, direct_interc_79_out, direct_interc_80_out, direct_interc_81_out, direct_interc_82_out, direct_interc_83_out, direct_interc_84_out, direct_interc_85_out, direct_interc_86_out, direct_interc_87_out, direct_interc_88_out, direct_interc_89_out, direct_interc_90_out, direct_interc_91_out, direct_interc_92_out, direct_interc_93_out, direct_interc_94_out, direct_interc_95_out}),
		.enable(decoder1to2_0_data_out[0]),
		.address(address[0]),
		.data_in(data_in),
		.mult_32x32_out(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[0:63]));

	direct_interc direct_interc_0_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[0]),
		.out(mult_32x32_slice_OUT_cfg[0]));

	direct_interc direct_interc_1_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[1]),
		.out(mult_32x32_slice_OUT_cfg[1]));

	direct_interc direct_interc_2_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[2]),
		.out(mult_32x32_slice_OUT_cfg[2]));

	direct_interc direct_interc_3_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[3]),
		.out(mult_32x32_slice_OUT_cfg[3]));

	direct_interc direct_interc_4_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[4]),
		.out(mult_32x32_slice_OUT_cfg[4]));

	direct_interc direct_interc_5_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[5]),
		.out(mult_32x32_slice_OUT_cfg[5]));

	direct_interc direct_interc_6_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[6]),
		.out(mult_32x32_slice_OUT_cfg[6]));

	direct_interc direct_interc_7_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[7]),
		.out(mult_32x32_slice_OUT_cfg[7]));

	direct_interc direct_interc_8_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[8]),
		.out(mult_32x32_slice_OUT_cfg[8]));

	direct_interc direct_interc_9_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[9]),
		.out(mult_32x32_slice_OUT_cfg[9]));

	direct_interc direct_interc_10_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[10]),
		.out(mult_32x32_slice_OUT_cfg[10]));

	direct_interc direct_interc_11_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[11]),
		.out(mult_32x32_slice_OUT_cfg[11]));

	direct_interc direct_interc_12_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[12]),
		.out(mult_32x32_slice_OUT_cfg[12]));

	direct_interc direct_interc_13_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[13]),
		.out(mult_32x32_slice_OUT_cfg[13]));

	direct_interc direct_interc_14_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[14]),
		.out(mult_32x32_slice_OUT_cfg[14]));

	direct_interc direct_interc_15_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[15]),
		.out(mult_32x32_slice_OUT_cfg[15]));

	direct_interc direct_interc_16_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[16]),
		.out(mult_32x32_slice_OUT_cfg[16]));

	direct_interc direct_interc_17_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[17]),
		.out(mult_32x32_slice_OUT_cfg[17]));

	direct_interc direct_interc_18_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[18]),
		.out(mult_32x32_slice_OUT_cfg[18]));

	direct_interc direct_interc_19_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[19]),
		.out(mult_32x32_slice_OUT_cfg[19]));

	direct_interc direct_interc_20_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[20]),
		.out(mult_32x32_slice_OUT_cfg[20]));

	direct_interc direct_interc_21_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[21]),
		.out(mult_32x32_slice_OUT_cfg[21]));

	direct_interc direct_interc_22_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[22]),
		.out(mult_32x32_slice_OUT_cfg[22]));

	direct_interc direct_interc_23_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[23]),
		.out(mult_32x32_slice_OUT_cfg[23]));

	direct_interc direct_interc_24_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[24]),
		.out(mult_32x32_slice_OUT_cfg[24]));

	direct_interc direct_interc_25_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[25]),
		.out(mult_32x32_slice_OUT_cfg[25]));

	direct_interc direct_interc_26_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[26]),
		.out(mult_32x32_slice_OUT_cfg[26]));

	direct_interc direct_interc_27_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[27]),
		.out(mult_32x32_slice_OUT_cfg[27]));

	direct_interc direct_interc_28_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[28]),
		.out(mult_32x32_slice_OUT_cfg[28]));

	direct_interc direct_interc_29_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[29]),
		.out(mult_32x32_slice_OUT_cfg[29]));

	direct_interc direct_interc_30_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[30]),
		.out(mult_32x32_slice_OUT_cfg[30]));

	direct_interc direct_interc_31_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[31]),
		.out(mult_32x32_slice_OUT_cfg[31]));

	direct_interc direct_interc_32_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[32]),
		.out(mult_32x32_slice_OUT_cfg[32]));

	direct_interc direct_interc_33_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[33]),
		.out(mult_32x32_slice_OUT_cfg[33]));

	direct_interc direct_interc_34_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[34]),
		.out(mult_32x32_slice_OUT_cfg[34]));

	direct_interc direct_interc_35_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[35]),
		.out(mult_32x32_slice_OUT_cfg[35]));

	direct_interc direct_interc_36_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[36]),
		.out(mult_32x32_slice_OUT_cfg[36]));

	direct_interc direct_interc_37_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[37]),
		.out(mult_32x32_slice_OUT_cfg[37]));

	direct_interc direct_interc_38_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[38]),
		.out(mult_32x32_slice_OUT_cfg[38]));

	direct_interc direct_interc_39_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[39]),
		.out(mult_32x32_slice_OUT_cfg[39]));

	direct_interc direct_interc_40_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[40]),
		.out(mult_32x32_slice_OUT_cfg[40]));

	direct_interc direct_interc_41_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[41]),
		.out(mult_32x32_slice_OUT_cfg[41]));

	direct_interc direct_interc_42_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[42]),
		.out(mult_32x32_slice_OUT_cfg[42]));

	direct_interc direct_interc_43_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[43]),
		.out(mult_32x32_slice_OUT_cfg[43]));

	direct_interc direct_interc_44_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[44]),
		.out(mult_32x32_slice_OUT_cfg[44]));

	direct_interc direct_interc_45_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[45]),
		.out(mult_32x32_slice_OUT_cfg[45]));

	direct_interc direct_interc_46_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[46]),
		.out(mult_32x32_slice_OUT_cfg[46]));

	direct_interc direct_interc_47_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[47]),
		.out(mult_32x32_slice_OUT_cfg[47]));

	direct_interc direct_interc_48_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[48]),
		.out(mult_32x32_slice_OUT_cfg[48]));

	direct_interc direct_interc_49_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[49]),
		.out(mult_32x32_slice_OUT_cfg[49]));

	direct_interc direct_interc_50_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[50]),
		.out(mult_32x32_slice_OUT_cfg[50]));

	direct_interc direct_interc_51_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[51]),
		.out(mult_32x32_slice_OUT_cfg[51]));

	direct_interc direct_interc_52_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[52]),
		.out(mult_32x32_slice_OUT_cfg[52]));

	direct_interc direct_interc_53_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[53]),
		.out(mult_32x32_slice_OUT_cfg[53]));

	direct_interc direct_interc_54_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[54]),
		.out(mult_32x32_slice_OUT_cfg[54]));

	direct_interc direct_interc_55_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[55]),
		.out(mult_32x32_slice_OUT_cfg[55]));

	direct_interc direct_interc_56_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[56]),
		.out(mult_32x32_slice_OUT_cfg[56]));

	direct_interc direct_interc_57_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[57]),
		.out(mult_32x32_slice_OUT_cfg[57]));

	direct_interc direct_interc_58_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[58]),
		.out(mult_32x32_slice_OUT_cfg[58]));

	direct_interc direct_interc_59_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[59]),
		.out(mult_32x32_slice_OUT_cfg[59]));

	direct_interc direct_interc_60_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[60]),
		.out(mult_32x32_slice_OUT_cfg[60]));

	direct_interc direct_interc_61_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[61]),
		.out(mult_32x32_slice_OUT_cfg[61]));

	direct_interc direct_interc_62_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[62]),
		.out(mult_32x32_slice_OUT_cfg[62]));

	direct_interc direct_interc_63_ (
		.in(logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice_mode_default__mult_32x32_0_mult_32x32_out[63]),
		.out(mult_32x32_slice_OUT_cfg[63]));

	direct_interc direct_interc_64_ (
		.in(mult_32x32_slice_B_cfg[0]),
		.out(direct_interc_64_out));

	direct_interc direct_interc_65_ (
		.in(mult_32x32_slice_B_cfg[1]),
		.out(direct_interc_65_out));

	direct_interc direct_interc_66_ (
		.in(mult_32x32_slice_B_cfg[2]),
		.out(direct_interc_66_out));

	direct_interc direct_interc_67_ (
		.in(mult_32x32_slice_B_cfg[3]),
		.out(direct_interc_67_out));

	direct_interc direct_interc_68_ (
		.in(mult_32x32_slice_B_cfg[4]),
		.out(direct_interc_68_out));

	direct_interc direct_interc_69_ (
		.in(mult_32x32_slice_B_cfg[5]),
		.out(direct_interc_69_out));

	direct_interc direct_interc_70_ (
		.in(mult_32x32_slice_B_cfg[6]),
		.out(direct_interc_70_out));

	direct_interc direct_interc_71_ (
		.in(mult_32x32_slice_B_cfg[7]),
		.out(direct_interc_71_out));

	direct_interc direct_interc_72_ (
		.in(mult_32x32_slice_B_cfg[8]),
		.out(direct_interc_72_out));

	direct_interc direct_interc_73_ (
		.in(mult_32x32_slice_B_cfg[9]),
		.out(direct_interc_73_out));

	direct_interc direct_interc_74_ (
		.in(mult_32x32_slice_B_cfg[10]),
		.out(direct_interc_74_out));

	direct_interc direct_interc_75_ (
		.in(mult_32x32_slice_B_cfg[11]),
		.out(direct_interc_75_out));

	direct_interc direct_interc_76_ (
		.in(mult_32x32_slice_B_cfg[12]),
		.out(direct_interc_76_out));

	direct_interc direct_interc_77_ (
		.in(mult_32x32_slice_B_cfg[13]),
		.out(direct_interc_77_out));

	direct_interc direct_interc_78_ (
		.in(mult_32x32_slice_B_cfg[14]),
		.out(direct_interc_78_out));

	direct_interc direct_interc_79_ (
		.in(mult_32x32_slice_B_cfg[15]),
		.out(direct_interc_79_out));

	direct_interc direct_interc_80_ (
		.in(mult_32x32_slice_B_cfg[16]),
		.out(direct_interc_80_out));

	direct_interc direct_interc_81_ (
		.in(mult_32x32_slice_B_cfg[17]),
		.out(direct_interc_81_out));

	direct_interc direct_interc_82_ (
		.in(mult_32x32_slice_B_cfg[18]),
		.out(direct_interc_82_out));

	direct_interc direct_interc_83_ (
		.in(mult_32x32_slice_B_cfg[19]),
		.out(direct_interc_83_out));

	direct_interc direct_interc_84_ (
		.in(mult_32x32_slice_B_cfg[20]),
		.out(direct_interc_84_out));

	direct_interc direct_interc_85_ (
		.in(mult_32x32_slice_B_cfg[21]),
		.out(direct_interc_85_out));

	direct_interc direct_interc_86_ (
		.in(mult_32x32_slice_B_cfg[22]),
		.out(direct_interc_86_out));

	direct_interc direct_interc_87_ (
		.in(mult_32x32_slice_B_cfg[23]),
		.out(direct_interc_87_out));

	direct_interc direct_interc_88_ (
		.in(mult_32x32_slice_B_cfg[24]),
		.out(direct_interc_88_out));

	direct_interc direct_interc_89_ (
		.in(mult_32x32_slice_B_cfg[25]),
		.out(direct_interc_89_out));

	direct_interc direct_interc_90_ (
		.in(mult_32x32_slice_B_cfg[26]),
		.out(direct_interc_90_out));

	direct_interc direct_interc_91_ (
		.in(mult_32x32_slice_B_cfg[27]),
		.out(direct_interc_91_out));

	direct_interc direct_interc_92_ (
		.in(mult_32x32_slice_B_cfg[28]),
		.out(direct_interc_92_out));

	direct_interc direct_interc_93_ (
		.in(mult_32x32_slice_B_cfg[29]),
		.out(direct_interc_93_out));

	direct_interc direct_interc_94_ (
		.in(mult_32x32_slice_B_cfg[30]),
		.out(direct_interc_94_out));

	direct_interc direct_interc_95_ (
		.in(mult_32x32_slice_B_cfg[31]),
		.out(direct_interc_95_out));

	mux_1level_tapbuf_size2_mem mem_mult_32x32_0_a_0 (
		.pReset(pReset),
		.enable(decoder1to2_0_data_out[1]),
		.address(address[0:1]),
		.data_in(data_in),
		.mem_out({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.mem_outb({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_0 (
		.in({mult_32x32_slice_A_cfg[0], mult_32x32_slice_B_cfg[0]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_0_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_1 (
		.in({mult_32x32_slice_A_cfg[1], mult_32x32_slice_B_cfg[1]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_1_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_2 (
		.in({mult_32x32_slice_A_cfg[2], mult_32x32_slice_B_cfg[2]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_2_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_3 (
		.in({mult_32x32_slice_A_cfg[3], mult_32x32_slice_B_cfg[3]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_3_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_4 (
		.in({mult_32x32_slice_A_cfg[4], mult_32x32_slice_B_cfg[4]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_4_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_5 (
		.in({mult_32x32_slice_A_cfg[5], mult_32x32_slice_B_cfg[5]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_5_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_6 (
		.in({mult_32x32_slice_A_cfg[6], mult_32x32_slice_B_cfg[6]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_6_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_7 (
		.in({mult_32x32_slice_A_cfg[7], mult_32x32_slice_B_cfg[7]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_7_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_8 (
		.in({mult_32x32_slice_A_cfg[8], mult_32x32_slice_B_cfg[8]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_8_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_9 (
		.in({mult_32x32_slice_A_cfg[9], mult_32x32_slice_B_cfg[9]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_9_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_10 (
		.in({mult_32x32_slice_A_cfg[10], mult_32x32_slice_B_cfg[10]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_10_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_11 (
		.in({mult_32x32_slice_A_cfg[11], mult_32x32_slice_B_cfg[11]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_11_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_12 (
		.in({mult_32x32_slice_A_cfg[12], mult_32x32_slice_B_cfg[12]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_12_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_13 (
		.in({mult_32x32_slice_A_cfg[13], mult_32x32_slice_B_cfg[13]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_13_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_14 (
		.in({mult_32x32_slice_A_cfg[14], mult_32x32_slice_B_cfg[14]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_14_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_15 (
		.in({mult_32x32_slice_A_cfg[15], mult_32x32_slice_B_cfg[15]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_15_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_16 (
		.in({mult_32x32_slice_A_cfg[16], mult_32x32_slice_B_cfg[16]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_16_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_17 (
		.in({mult_32x32_slice_A_cfg[17], mult_32x32_slice_B_cfg[17]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_17_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_18 (
		.in({mult_32x32_slice_A_cfg[18], mult_32x32_slice_B_cfg[18]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_18_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_19 (
		.in({mult_32x32_slice_A_cfg[19], mult_32x32_slice_B_cfg[19]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_19_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_20 (
		.in({mult_32x32_slice_A_cfg[20], mult_32x32_slice_B_cfg[20]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_20_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_21 (
		.in({mult_32x32_slice_A_cfg[21], mult_32x32_slice_B_cfg[21]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_21_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_22 (
		.in({mult_32x32_slice_A_cfg[22], mult_32x32_slice_B_cfg[22]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_22_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_23 (
		.in({mult_32x32_slice_A_cfg[23], mult_32x32_slice_B_cfg[23]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_23_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_24 (
		.in({mult_32x32_slice_A_cfg[24], mult_32x32_slice_B_cfg[24]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_24_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_25 (
		.in({mult_32x32_slice_A_cfg[25], mult_32x32_slice_B_cfg[25]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_25_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_26 (
		.in({mult_32x32_slice_A_cfg[26], mult_32x32_slice_B_cfg[26]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_26_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_27 (
		.in({mult_32x32_slice_A_cfg[27], mult_32x32_slice_B_cfg[27]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_27_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_28 (
		.in({mult_32x32_slice_A_cfg[28], mult_32x32_slice_B_cfg[28]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_28_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_29 (
		.in({mult_32x32_slice_A_cfg[29], mult_32x32_slice_B_cfg[29]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_29_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_30 (
		.in({mult_32x32_slice_A_cfg[30], mult_32x32_slice_B_cfg[30]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_30_out));

	mux_1level_tapbuf_size2 mux_mult_32x32_0_a_31 (
		.in({mult_32x32_slice_A_cfg[31], mult_32x32_slice_B_cfg[31]}),
		.sram({mux_1level_tapbuf_size2_mem_0_mem_out_0[0], mux_1level_tapbuf_size2_mem_0_mem_out_1[1], mux_1level_tapbuf_size2_mem_0_mem_out_2[2]}),
		.sram_inv({mux_1level_tapbuf_size2_mem_0_mem_outb_0[0], mux_1level_tapbuf_size2_mem_0_mem_outb_1[1], mux_1level_tapbuf_size2_mem_0_mem_outb_2[2]}),
		.out(mux_1level_tapbuf_size2_31_out));

	decoder1to2 decoder1to2_0_ (
		.enable(enable),
		.address(address[2]),
		.data_out(decoder1to2_0_data_out[0:1]));

endmodule
// ----- END Verilog module for logical_tile_mult_32_mode_mult_32x32__mult_32x32_slice -----

//----- Default net type -----
`default_nettype wire



// ----- END Physical programmable logic block Verilog module: mult_32x32_slice -----
