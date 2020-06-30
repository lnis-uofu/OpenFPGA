//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Memories used in FPGA
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:39 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for mux_2level_tapbuf_size6_mem -----
module mux_2level_tapbuf_size6_mem(pReset,
                                   prog_clk,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
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
		.enable(enable[0]),
		.address(address[0:2]),
		.data_out(decoder3to6_0_data_out[0:5]));

	config_latch config_latch_0_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[0]),
		.Q(mem_out[0]),
		.Qb(mem_outb[0]));

	config_latch config_latch_1_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[1]),
		.Q(mem_out[1]),
		.Qb(mem_outb[1]));

	config_latch config_latch_2_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[2]),
		.Q(mem_out[2]),
		.Qb(mem_outb[2]));

	config_latch config_latch_3_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[3]),
		.Q(mem_out[3]),
		.Qb(mem_outb[3]));

	config_latch config_latch_4_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[4]),
		.Q(mem_out[4]),
		.Qb(mem_outb[4]));

	config_latch config_latch_5_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[5]),
		.Q(mem_out[5]),
		.Qb(mem_outb[5]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size6_mem -----



// ----- Verilog module for mux_2level_tapbuf_size2_mem -----
module mux_2level_tapbuf_size2_mem(pReset,
                                   prog_clk,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
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
		.enable(enable[0]),
		.address(address[0]),
		.data_out(decoder1to2_0_data_out[0:1]));

	config_latch config_latch_0_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder1to2_0_data_out[0]),
		.Q(mem_out[0]),
		.Qb(mem_outb[0]));

	config_latch config_latch_1_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder1to2_0_data_out[1]),
		.Q(mem_out[1]),
		.Qb(mem_outb[1]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size2_mem -----



// ----- Verilog module for mux_2level_tapbuf_size8_mem -----
module mux_2level_tapbuf_size8_mem(pReset,
                                   prog_clk,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
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
		.enable(enable[0]),
		.address(address[0:2]),
		.data_out(decoder3to6_0_data_out[0:5]));

	config_latch config_latch_0_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[0]),
		.Q(mem_out[0]),
		.Qb(mem_outb[0]));

	config_latch config_latch_1_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[1]),
		.Q(mem_out[1]),
		.Qb(mem_outb[1]));

	config_latch config_latch_2_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[2]),
		.Q(mem_out[2]),
		.Qb(mem_outb[2]));

	config_latch config_latch_3_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[3]),
		.Q(mem_out[3]),
		.Qb(mem_outb[3]));

	config_latch config_latch_4_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[4]),
		.Q(mem_out[4]),
		.Qb(mem_outb[4]));

	config_latch config_latch_5_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[5]),
		.Q(mem_out[5]),
		.Qb(mem_outb[5]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size8_mem -----



// ----- Verilog module for mux_2level_tapbuf_size9_mem -----
module mux_2level_tapbuf_size9_mem(pReset,
                                   prog_clk,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
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
		.enable(enable[0]),
		.address(address[0:2]),
		.data_out(decoder3to8_0_data_out[0:7]));

	config_latch config_latch_0_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[0]),
		.Q(mem_out[0]),
		.Qb(mem_outb[0]));

	config_latch config_latch_1_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[1]),
		.Q(mem_out[1]),
		.Qb(mem_outb[1]));

	config_latch config_latch_2_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[2]),
		.Q(mem_out[2]),
		.Qb(mem_outb[2]));

	config_latch config_latch_3_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[3]),
		.Q(mem_out[3]),
		.Qb(mem_outb[3]));

	config_latch config_latch_4_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[4]),
		.Q(mem_out[4]),
		.Qb(mem_outb[4]));

	config_latch config_latch_5_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[5]),
		.Q(mem_out[5]),
		.Qb(mem_outb[5]));

	config_latch config_latch_6_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[6]),
		.Q(mem_out[6]),
		.Qb(mem_outb[6]));

	config_latch config_latch_7_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[7]),
		.Q(mem_out[7]),
		.Qb(mem_outb[7]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size9_mem -----



// ----- Verilog module for mux_2level_tapbuf_size3_mem -----
module mux_2level_tapbuf_size3_mem(pReset,
                                   prog_clk,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
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
		.enable(enable[0]),
		.address(address[0]),
		.data_out(decoder1to2_0_data_out[0:1]));

	config_latch config_latch_0_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder1to2_0_data_out[0]),
		.Q(mem_out[0]),
		.Qb(mem_outb[0]));

	config_latch config_latch_1_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder1to2_0_data_out[1]),
		.Q(mem_out[1]),
		.Qb(mem_outb[1]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size3_mem -----



// ----- Verilog module for mux_2level_tapbuf_size5_mem -----
module mux_2level_tapbuf_size5_mem(pReset,
                                   prog_clk,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
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
		.enable(enable[0]),
		.address(address[0:2]),
		.data_out(decoder3to6_0_data_out[0:5]));

	config_latch config_latch_0_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[0]),
		.Q(mem_out[0]),
		.Qb(mem_outb[0]));

	config_latch config_latch_1_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[1]),
		.Q(mem_out[1]),
		.Qb(mem_outb[1]));

	config_latch config_latch_2_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[2]),
		.Q(mem_out[2]),
		.Qb(mem_outb[2]));

	config_latch config_latch_3_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[3]),
		.Q(mem_out[3]),
		.Qb(mem_outb[3]));

	config_latch config_latch_4_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[4]),
		.Q(mem_out[4]),
		.Qb(mem_outb[4]));

	config_latch config_latch_5_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[5]),
		.Q(mem_out[5]),
		.Qb(mem_outb[5]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size5_mem -----



// ----- Verilog module for mux_2level_tapbuf_size4_mem -----
module mux_2level_tapbuf_size4_mem(pReset,
                                   prog_clk,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
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
		.enable(enable[0]),
		.address(address[0:2]),
		.data_out(decoder3to6_0_data_out[0:5]));

	config_latch config_latch_0_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[0]),
		.Q(mem_out[0]),
		.Qb(mem_outb[0]));

	config_latch config_latch_1_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[1]),
		.Q(mem_out[1]),
		.Qb(mem_outb[1]));

	config_latch config_latch_2_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[2]),
		.Q(mem_out[2]),
		.Qb(mem_outb[2]));

	config_latch config_latch_3_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[3]),
		.Q(mem_out[3]),
		.Qb(mem_outb[3]));

	config_latch config_latch_4_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[4]),
		.Q(mem_out[4]),
		.Qb(mem_outb[4]));

	config_latch config_latch_5_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to6_0_data_out[5]),
		.Q(mem_out[5]),
		.Qb(mem_outb[5]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size4_mem -----



// ----- Verilog module for mux_2level_size14_mem -----
module mux_2level_size14_mem(pReset,
                             prog_clk,
                             enable,
                             address,
                             data_in,
                             mem_out,
                             mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
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
		.enable(enable[0]),
		.address(address[0:2]),
		.data_out(decoder3to8_0_data_out[0:7]));

	config_latch config_latch_0_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[0]),
		.Q(mem_out[0]),
		.Qb(mem_outb[0]));

	config_latch config_latch_1_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[1]),
		.Q(mem_out[1]),
		.Qb(mem_outb[1]));

	config_latch config_latch_2_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[2]),
		.Q(mem_out[2]),
		.Qb(mem_outb[2]));

	config_latch config_latch_3_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[3]),
		.Q(mem_out[3]),
		.Qb(mem_outb[3]));

	config_latch config_latch_4_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[4]),
		.Q(mem_out[4]),
		.Qb(mem_outb[4]));

	config_latch config_latch_5_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[5]),
		.Q(mem_out[5]),
		.Qb(mem_outb[5]));

	config_latch config_latch_6_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[6]),
		.Q(mem_out[6]),
		.Qb(mem_outb[6]));

	config_latch config_latch_7_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder3to8_0_data_out[7]),
		.Q(mem_out[7]),
		.Qb(mem_outb[7]));

endmodule
// ----- END Verilog module for mux_2level_size14_mem -----



// ----- Verilog module for mux_1level_tapbuf_size2_mem -----
module mux_1level_tapbuf_size2_mem(pReset,
                                   prog_clk,
                                   enable,
                                   address,
                                   data_in,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
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
		.enable(enable[0]),
		.address(address[0:1]),
		.data_out(decoder2to3_0_data_out[0:2]));

	config_latch config_latch_0_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder2to3_0_data_out[0]),
		.Q(mem_out[0]),
		.Qb(mem_outb[0]));

	config_latch config_latch_1_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder2to3_0_data_out[1]),
		.Q(mem_out[1]),
		.Qb(mem_outb[1]));

	config_latch config_latch_2_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder2to3_0_data_out[2]),
		.Q(mem_out[2]),
		.Qb(mem_outb[2]));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_size2_mem -----



// ----- Verilog module for lut4_config_latch_mem -----
module lut4_config_latch_mem(pReset,
                             prog_clk,
                             enable,
                             address,
                             data_in,
                             mem_out,
                             mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:3] address;
//----- INPUT PORTS -----
input [0:0] data_in;
//----- OUTPUT PORTS -----
output [0:15] mem_out;
//----- OUTPUT PORTS -----
output [0:15] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:15] decoder4to16_0_data_out;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	decoder4to16 decoder4to16_0_ (
		.enable(enable[0]),
		.address(address[0:3]),
		.data_out(decoder4to16_0_data_out[0:15]));

	config_latch config_latch_0_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[0]),
		.Q(mem_out[0]),
		.Qb(mem_outb[0]));

	config_latch config_latch_1_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[1]),
		.Q(mem_out[1]),
		.Qb(mem_outb[1]));

	config_latch config_latch_2_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[2]),
		.Q(mem_out[2]),
		.Qb(mem_outb[2]));

	config_latch config_latch_3_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[3]),
		.Q(mem_out[3]),
		.Qb(mem_outb[3]));

	config_latch config_latch_4_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[4]),
		.Q(mem_out[4]),
		.Qb(mem_outb[4]));

	config_latch config_latch_5_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[5]),
		.Q(mem_out[5]),
		.Qb(mem_outb[5]));

	config_latch config_latch_6_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[6]),
		.Q(mem_out[6]),
		.Qb(mem_outb[6]));

	config_latch config_latch_7_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[7]),
		.Q(mem_out[7]),
		.Qb(mem_outb[7]));

	config_latch config_latch_8_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[8]),
		.Q(mem_out[8]),
		.Qb(mem_outb[8]));

	config_latch config_latch_9_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[9]),
		.Q(mem_out[9]),
		.Qb(mem_outb[9]));

	config_latch config_latch_10_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[10]),
		.Q(mem_out[10]),
		.Qb(mem_outb[10]));

	config_latch config_latch_11_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[11]),
		.Q(mem_out[11]),
		.Qb(mem_outb[11]));

	config_latch config_latch_12_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[12]),
		.Q(mem_out[12]),
		.Qb(mem_outb[12]));

	config_latch config_latch_13_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[13]),
		.Q(mem_out[13]),
		.Qb(mem_outb[13]));

	config_latch config_latch_14_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[14]),
		.Q(mem_out[14]),
		.Qb(mem_outb[14]));

	config_latch config_latch_15_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder4to16_0_data_out[15]),
		.Q(mem_out[15]),
		.Qb(mem_outb[15]));

endmodule
// ----- END Verilog module for lut4_config_latch_mem -----



// ----- Verilog module for iopad_config_latch_mem -----
module iopad_config_latch_mem(pReset,
                              prog_clk,
                              enable,
                              address,
                              data_in,
                              mem_out,
                              mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
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
		.enable(enable[0]),
		.address(address[0]),
		.data_out(decoder1to1_0_data_out[0]));

	config_latch config_latch_0_ (
		.reset(pReset[0]),
		.clk(prog_clk[0]),
		.bl(data_in[0]),
		.wl(decoder1to1_0_data_out[0]),
		.Q(mem_out[0]),
		.Qb(mem_outb[0]));

endmodule
// ----- END Verilog module for iopad_config_latch_mem -----



