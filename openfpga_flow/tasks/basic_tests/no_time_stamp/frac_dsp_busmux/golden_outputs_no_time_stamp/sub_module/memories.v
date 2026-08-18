//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Memories used in FPGA
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size4_mem -----
module mux_2level_tapbuf_size4_mem(pReset,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:2] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:5] mem_out;
//----- OUTPUT PORTS -----
output [0:5] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:5] decoder3to6_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder3to6 decoder3to6_0_ (
		.enable(enable),
		.address(address[0:2]),
		.data_out(decoder3to6_0_data_out[0:5]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	LATCHR LATCHR_2_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[2]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	LATCHR LATCHR_3_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[3]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	LATCHR LATCHR_4_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[4]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	LATCHR LATCHR_5_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[5]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size4_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size2_mem -----
module mux_2level_tapbuf_size2_mem(pReset,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:0] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:1] mem_out;
//----- OUTPUT PORTS -----
output [0:1] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:1] decoder1to2_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder1to2 decoder1to2_0_ (
		.enable(enable),
		.address(address),
		.data_out(decoder1to2_0_data_out[0:1]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder1to2_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder1to2_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size2_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size3_mem -----
module mux_2level_tapbuf_size3_mem(pReset,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:0] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:1] mem_out;
//----- OUTPUT PORTS -----
output [0:1] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:1] decoder1to2_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder1to2 decoder1to2_0_ (
		.enable(enable),
		.address(address),
		.data_out(decoder1to2_0_data_out[0:1]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder1to2_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder1to2_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size3_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size8_mem -----
module mux_2level_tapbuf_size8_mem(pReset,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:2] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:5] mem_out;
//----- OUTPUT PORTS -----
output [0:5] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:5] decoder3to6_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder3to6 decoder3to6_0_ (
		.enable(enable),
		.address(address[0:2]),
		.data_out(decoder3to6_0_data_out[0:5]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	LATCHR LATCHR_2_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[2]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	LATCHR LATCHR_3_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[3]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	LATCHR LATCHR_4_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[4]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	LATCHR LATCHR_5_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[5]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size8_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size7_mem -----
module mux_2level_tapbuf_size7_mem(pReset,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:2] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:5] mem_out;
//----- OUTPUT PORTS -----
output [0:5] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:5] decoder3to6_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder3to6 decoder3to6_0_ (
		.enable(enable),
		.address(address[0:2]),
		.data_out(decoder3to6_0_data_out[0:5]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	LATCHR LATCHR_2_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[2]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	LATCHR LATCHR_3_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[3]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	LATCHR LATCHR_4_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[4]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	LATCHR LATCHR_5_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[5]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size7_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size6_mem -----
module mux_2level_tapbuf_size6_mem(pReset,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:2] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:5] mem_out;
//----- OUTPUT PORTS -----
output [0:5] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:5] decoder3to6_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder3to6 decoder3to6_0_ (
		.enable(enable),
		.address(address[0:2]),
		.data_out(decoder3to6_0_data_out[0:5]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	LATCHR LATCHR_2_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[2]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	LATCHR LATCHR_3_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[3]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	LATCHR LATCHR_4_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[4]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	LATCHR LATCHR_5_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[5]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size6_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size9_mem -----
module mux_2level_tapbuf_size9_mem(pReset,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:2] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:7] mem_out;
//----- OUTPUT PORTS -----
output [0:7] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:7] decoder3to8_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder3to8 decoder3to8_0_ (
		.enable(enable),
		.address(address[0:2]),
		.data_out(decoder3to8_0_data_out[0:7]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to8_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to8_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	LATCHR LATCHR_2_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to8_0_data_out[2]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	LATCHR LATCHR_3_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to8_0_data_out[3]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	LATCHR LATCHR_4_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to8_0_data_out[4]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	LATCHR LATCHR_5_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to8_0_data_out[5]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

	LATCHR LATCHR_6_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to8_0_data_out[6]),
		.Q(mem_out[6]),
		.QN(mem_outb[6]));

	LATCHR LATCHR_7_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to8_0_data_out[7]),
		.Q(mem_out[7]),
		.QN(mem_outb[7]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size9_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size5_mem -----
module mux_2level_tapbuf_size5_mem(pReset,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:2] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:5] mem_out;
//----- OUTPUT PORTS -----
output [0:5] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:5] decoder3to6_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder3to6 decoder3to6_0_ (
		.enable(enable),
		.address(address[0:2]),
		.data_out(decoder3to6_0_data_out[0:5]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	LATCHR LATCHR_2_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[2]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	LATCHR LATCHR_3_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[3]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	LATCHR LATCHR_4_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[4]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	LATCHR LATCHR_5_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder3to6_0_data_out[5]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size5_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_size20_mem -----
module mux_2level_size20_mem(pReset,
                             enable,
                             address,
                             data_in,
                             mem_out,
                             mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:3] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:9] mem_out;
//----- OUTPUT PORTS -----
output [0:9] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:9] decoder4to10_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder4to10 decoder4to10_0_ (
		.enable(enable),
		.address(address[0:3]),
		.data_out(decoder4to10_0_data_out[0:9]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder4to10_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder4to10_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	LATCHR LATCHR_2_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder4to10_0_data_out[2]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	LATCHR LATCHR_3_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder4to10_0_data_out[3]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	LATCHR LATCHR_4_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder4to10_0_data_out[4]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	LATCHR LATCHR_5_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder4to10_0_data_out[5]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

	LATCHR LATCHR_6_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder4to10_0_data_out[6]),
		.Q(mem_out[6]),
		.QN(mem_outb[6]));

	LATCHR LATCHR_7_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder4to10_0_data_out[7]),
		.Q(mem_out[7]),
		.QN(mem_outb[7]));

	LATCHR LATCHR_8_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder4to10_0_data_out[8]),
		.Q(mem_out[8]),
		.QN(mem_outb[8]));

	LATCHR LATCHR_9_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder4to10_0_data_out[9]),
		.Q(mem_out[9]),
		.QN(mem_outb[9]));

endmodule
// ----- END Verilog module for mux_2level_size20_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_1level_tapbuf_size3_mem -----
module mux_1level_tapbuf_size3_mem(pReset,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:1] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:3] mem_out;
//----- OUTPUT PORTS -----
output [0:3] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:3] decoder2to4_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder2to4 decoder2to4_0_ (
		.enable(enable),
		.address(address[0:1]),
		.data_out(decoder2to4_0_data_out[0:3]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder2to4_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder2to4_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	LATCHR LATCHR_2_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder2to4_0_data_out[2]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	LATCHR LATCHR_3_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder2to4_0_data_out[3]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_size3_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_1level_tapbuf_size2_mem -----
module mux_1level_tapbuf_size2_mem(pReset,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:1] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:2] mem_out;
//----- OUTPUT PORTS -----
output [0:2] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:2] decoder2to3_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder2to3 decoder2to3_0_ (
		.enable(enable),
		.address(address[0:1]),
		.data_out(decoder2to3_0_data_out[0:2]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder2to3_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder2to3_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	LATCHR LATCHR_2_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder2to3_0_data_out[2]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_size2_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for frac_lut4_LATCHR_mem -----
module frac_lut4_LATCHR_mem(pReset,
                            enable,
                            address,
                            data_in,
                            mem_out,
                            mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:4] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:16] mem_out;
//----- OUTPUT PORTS -----
output [0:16] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:16] decoder5to17_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder5to17 decoder5to17_0_ (
		.enable(enable),
		.address(address[0:4]),
		.data_out(decoder5to17_0_data_out[0:16]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	LATCHR LATCHR_2_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[2]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	LATCHR LATCHR_3_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[3]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	LATCHR LATCHR_4_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[4]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	LATCHR LATCHR_5_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[5]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

	LATCHR LATCHR_6_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[6]),
		.Q(mem_out[6]),
		.QN(mem_outb[6]));

	LATCHR LATCHR_7_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[7]),
		.Q(mem_out[7]),
		.QN(mem_outb[7]));

	LATCHR LATCHR_8_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[8]),
		.Q(mem_out[8]),
		.QN(mem_outb[8]));

	LATCHR LATCHR_9_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[9]),
		.Q(mem_out[9]),
		.QN(mem_outb[9]));

	LATCHR LATCHR_10_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[10]),
		.Q(mem_out[10]),
		.QN(mem_outb[10]));

	LATCHR LATCHR_11_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[11]),
		.Q(mem_out[11]),
		.QN(mem_outb[11]));

	LATCHR LATCHR_12_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[12]),
		.Q(mem_out[12]),
		.QN(mem_outb[12]));

	LATCHR LATCHR_13_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[13]),
		.Q(mem_out[13]),
		.QN(mem_outb[13]));

	LATCHR LATCHR_14_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[14]),
		.Q(mem_out[14]),
		.QN(mem_outb[14]));

	LATCHR LATCHR_15_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[15]),
		.Q(mem_out[15]),
		.QN(mem_outb[15]));

	LATCHR LATCHR_16_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder5to17_0_data_out[16]),
		.Q(mem_out[16]),
		.QN(mem_outb[16]));

endmodule
// ----- END Verilog module for frac_lut4_LATCHR_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for GPIO_LATCHR_mem -----
module GPIO_LATCHR_mem(pReset,
                       enable,
                       address,
                       data_in,
                       mem_out,
                       mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:0] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:0] mem_out;
//----- OUTPUT PORTS -----
output [0:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] decoder1to1_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder1to1 decoder1to1_0_ (
		.enable(enable),
		.address(address),
		.data_out(decoder1to1_0_data_out));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder1to1_0_data_out),
		.Q(mem_out),
		.QN(mem_outb));

endmodule
// ----- END Verilog module for GPIO_LATCHR_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mult_32x32_LATCHR_mem -----
module mult_32x32_LATCHR_mem(pReset,
                             enable,
                             address,
                             data_in,
                             mem_out,
                             mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:0] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:1] mem_out;
//----- OUTPUT PORTS -----
output [0:1] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:1] decoder1to2_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder1to2 decoder1to2_0_ (
		.enable(enable),
		.address(address),
		.data_out(decoder1to2_0_data_out[0:1]));

	LATCHR LATCHR_0_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder1to2_0_data_out[0]),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	LATCHR LATCHR_1_ (
		.RST(pReset),
		.D(data_in),
		.WE(decoder1to2_0_data_out[1]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

endmodule
// ----- END Verilog module for mult_32x32_LATCHR_mem -----

//----- Default net type -----
`default_nettype wire




