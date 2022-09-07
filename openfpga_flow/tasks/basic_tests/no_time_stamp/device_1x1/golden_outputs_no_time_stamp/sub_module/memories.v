//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Memories used in FPGA
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_tapbuf_size6_mem -----
module mux_tree_tapbuf_size6_mem(prog_clk,
                                 ccff_head,
                                 ccff_tail,
                                 mem_out,
                                 mem_outb);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [0:2] mem_out;
//----- OUTPUT PORTS -----
output [0:2] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[2];
// ----- END Local output short connections -----

	DFF DFF_0_ (
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFF DFF_1_ (
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFF DFF_2_ (
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

endmodule
// ----- END Verilog module for mux_tree_tapbuf_size6_mem -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_tapbuf_size4_mem -----
module mux_tree_tapbuf_size4_mem(prog_clk,
                                 ccff_head,
                                 ccff_tail,
                                 mem_out,
                                 mem_outb);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [0:2] mem_out;
//----- OUTPUT PORTS -----
output [0:2] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[2];
// ----- END Local output short connections -----

	DFF DFF_0_ (
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFF DFF_1_ (
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFF DFF_2_ (
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

endmodule
// ----- END Verilog module for mux_tree_tapbuf_size4_mem -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_tapbuf_size3_mem -----
module mux_tree_tapbuf_size3_mem(prog_clk,
                                 ccff_head,
                                 ccff_tail,
                                 mem_out,
                                 mem_outb);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [0:1] mem_out;
//----- OUTPUT PORTS -----
output [0:1] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[1];
// ----- END Local output short connections -----

	DFF DFF_0_ (
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFF DFF_1_ (
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

endmodule
// ----- END Verilog module for mux_tree_tapbuf_size3_mem -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_tapbuf_size2_mem -----
module mux_tree_tapbuf_size2_mem(prog_clk,
                                 ccff_head,
                                 ccff_tail,
                                 mem_out,
                                 mem_outb);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [0:1] mem_out;
//----- OUTPUT PORTS -----
output [0:1] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[1];
// ----- END Local output short connections -----

	DFF DFF_0_ (
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFF DFF_1_ (
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

endmodule
// ----- END Verilog module for mux_tree_tapbuf_size2_mem -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_tree_size14_mem -----
module mux_tree_size14_mem(prog_clk,
                           ccff_head,
                           ccff_tail,
                           mem_out,
                           mem_outb);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [0:3] mem_out;
//----- OUTPUT PORTS -----
output [0:3] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[3];
// ----- END Local output short connections -----

	DFF DFF_0_ (
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFF DFF_1_ (
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFF DFF_2_ (
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	DFF DFF_3_ (
		.CK(prog_clk),
		.D(mem_out[2]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

endmodule
// ----- END Verilog module for mux_tree_size14_mem -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for lut4_DFF_mem -----
module lut4_DFF_mem(prog_clk,
                    ccff_head,
                    ccff_tail,
                    mem_out,
                    mem_outb);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [0:15] mem_out;
//----- OUTPUT PORTS -----
output [0:15] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[15];
// ----- END Local output short connections -----

	DFF DFF_0_ (
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFF DFF_1_ (
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFF DFF_2_ (
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	DFF DFF_3_ (
		.CK(prog_clk),
		.D(mem_out[2]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	DFF DFF_4_ (
		.CK(prog_clk),
		.D(mem_out[3]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	DFF DFF_5_ (
		.CK(prog_clk),
		.D(mem_out[4]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

	DFF DFF_6_ (
		.CK(prog_clk),
		.D(mem_out[5]),
		.Q(mem_out[6]),
		.QN(mem_outb[6]));

	DFF DFF_7_ (
		.CK(prog_clk),
		.D(mem_out[6]),
		.Q(mem_out[7]),
		.QN(mem_outb[7]));

	DFF DFF_8_ (
		.CK(prog_clk),
		.D(mem_out[7]),
		.Q(mem_out[8]),
		.QN(mem_outb[8]));

	DFF DFF_9_ (
		.CK(prog_clk),
		.D(mem_out[8]),
		.Q(mem_out[9]),
		.QN(mem_outb[9]));

	DFF DFF_10_ (
		.CK(prog_clk),
		.D(mem_out[9]),
		.Q(mem_out[10]),
		.QN(mem_outb[10]));

	DFF DFF_11_ (
		.CK(prog_clk),
		.D(mem_out[10]),
		.Q(mem_out[11]),
		.QN(mem_outb[11]));

	DFF DFF_12_ (
		.CK(prog_clk),
		.D(mem_out[11]),
		.Q(mem_out[12]),
		.QN(mem_outb[12]));

	DFF DFF_13_ (
		.CK(prog_clk),
		.D(mem_out[12]),
		.Q(mem_out[13]),
		.QN(mem_outb[13]));

	DFF DFF_14_ (
		.CK(prog_clk),
		.D(mem_out[13]),
		.Q(mem_out[14]),
		.QN(mem_outb[14]));

	DFF DFF_15_ (
		.CK(prog_clk),
		.D(mem_out[14]),
		.Q(mem_out[15]),
		.QN(mem_outb[15]));

endmodule
// ----- END Verilog module for lut4_DFF_mem -----

//----- Default net type -----
`default_nettype none




//----- Default net type -----
`default_nettype none

// ----- Verilog module for GPIO_DFF_mem -----
module GPIO_DFF_mem(prog_clk,
                    ccff_head,
                    ccff_tail,
                    mem_out,
                    mem_outb);
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [0:0] mem_out;
//----- OUTPUT PORTS -----
output [0:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[0];
// ----- END Local output short connections -----

	DFF DFF_0_ (
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out),
		.QN(mem_outb));

endmodule
// ----- END Verilog module for GPIO_DFF_mem -----

//----- Default net type -----
`default_nettype none




