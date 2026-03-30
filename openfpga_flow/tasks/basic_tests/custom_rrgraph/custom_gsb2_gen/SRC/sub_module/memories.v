//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Memories used in FPGA
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size16_mem -----
module mux_2level_tapbuf_size16_mem(pReset,
                                    prog_clk,
                                    ccff_head,
                                    ccff_tail,
                                    mem_out,
                                    mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [9:0] mem_out;
//----- OUTPUT PORTS -----
output [9:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[9];
// ----- END Local output short connections -----

	DFFR DFFR_0_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFFR DFFR_1_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFFR DFFR_2_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	DFFR DFFR_3_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[2]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	DFFR DFFR_4_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[3]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	DFFR DFFR_5_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[4]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

	DFFR DFFR_6_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[5]),
		.Q(mem_out[6]),
		.QN(mem_outb[6]));

	DFFR DFFR_7_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[6]),
		.Q(mem_out[7]),
		.QN(mem_outb[7]));

	DFFR DFFR_8_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[7]),
		.Q(mem_out[8]),
		.QN(mem_outb[8]));

	DFFR DFFR_9_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[8]),
		.Q(mem_out[9]),
		.QN(mem_outb[9]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size16_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size16_feedthrough_mem -----
module mux_2level_tapbuf_size16_feedthrough_mem(feedthrough_mem_in,
                                                feedthrough_mem_inb,
                                                mem_out,
                                                mem_outb);
//----- INPUT PORTS -----
input [9:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [9:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [9:0] mem_out;
//----- OUTPUT PORTS -----
output [9:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[0] = feedthrough_mem_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[1] = feedthrough_mem_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[2] = feedthrough_mem_in[2];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[3] = feedthrough_mem_in[3];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[4] = feedthrough_mem_in[4];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[5] = feedthrough_mem_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[6] = feedthrough_mem_in[6];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[7] = feedthrough_mem_in[7];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[8] = feedthrough_mem_in[8];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[9] = feedthrough_mem_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[0] = feedthrough_mem_inb[0];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[1] = feedthrough_mem_inb[1];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[2] = feedthrough_mem_inb[2];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[3] = feedthrough_mem_inb[3];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[4] = feedthrough_mem_inb[4];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[5] = feedthrough_mem_inb[5];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[6] = feedthrough_mem_inb[6];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[7] = feedthrough_mem_inb[7];
// ----- Local connection due to Wire 18 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[8] = feedthrough_mem_inb[8];
// ----- Local connection due to Wire 19 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[9] = feedthrough_mem_inb[9];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size16_feedthrough_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size40_mem -----
module mux_2level_tapbuf_size40_mem(pReset,
                                    prog_clk,
                                    ccff_head,
                                    ccff_tail,
                                    mem_out,
                                    mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [13:0] mem_out;
//----- OUTPUT PORTS -----
output [13:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[13];
// ----- END Local output short connections -----

	DFFR DFFR_0_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFFR DFFR_1_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFFR DFFR_2_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	DFFR DFFR_3_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[2]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	DFFR DFFR_4_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[3]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	DFFR DFFR_5_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[4]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

	DFFR DFFR_6_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[5]),
		.Q(mem_out[6]),
		.QN(mem_outb[6]));

	DFFR DFFR_7_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[6]),
		.Q(mem_out[7]),
		.QN(mem_outb[7]));

	DFFR DFFR_8_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[7]),
		.Q(mem_out[8]),
		.QN(mem_outb[8]));

	DFFR DFFR_9_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[8]),
		.Q(mem_out[9]),
		.QN(mem_outb[9]));

	DFFR DFFR_10_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[9]),
		.Q(mem_out[10]),
		.QN(mem_outb[10]));

	DFFR DFFR_11_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[10]),
		.Q(mem_out[11]),
		.QN(mem_outb[11]));

	DFFR DFFR_12_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[11]),
		.Q(mem_out[12]),
		.QN(mem_outb[12]));

	DFFR DFFR_13_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[12]),
		.Q(mem_out[13]),
		.QN(mem_outb[13]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size40_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size40_feedthrough_mem -----
module mux_2level_tapbuf_size40_feedthrough_mem(feedthrough_mem_in,
                                                feedthrough_mem_inb,
                                                mem_out,
                                                mem_outb);
//----- INPUT PORTS -----
input [13:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [13:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [13:0] mem_out;
//----- OUTPUT PORTS -----
output [13:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[0] = feedthrough_mem_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[1] = feedthrough_mem_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[2] = feedthrough_mem_in[2];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[3] = feedthrough_mem_in[3];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[4] = feedthrough_mem_in[4];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[5] = feedthrough_mem_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[6] = feedthrough_mem_in[6];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[7] = feedthrough_mem_in[7];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[8] = feedthrough_mem_in[8];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[9] = feedthrough_mem_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[10] = feedthrough_mem_in[10];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[11] = feedthrough_mem_in[11];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[12] = feedthrough_mem_in[12];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[13] = feedthrough_mem_in[13];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[0] = feedthrough_mem_inb[0];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[1] = feedthrough_mem_inb[1];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[2] = feedthrough_mem_inb[2];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[3] = feedthrough_mem_inb[3];
// ----- Local connection due to Wire 18 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[4] = feedthrough_mem_inb[4];
// ----- Local connection due to Wire 19 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[5] = feedthrough_mem_inb[5];
// ----- Local connection due to Wire 20 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[6] = feedthrough_mem_inb[6];
// ----- Local connection due to Wire 21 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[7] = feedthrough_mem_inb[7];
// ----- Local connection due to Wire 22 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[8] = feedthrough_mem_inb[8];
// ----- Local connection due to Wire 23 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[9] = feedthrough_mem_inb[9];
// ----- Local connection due to Wire 24 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[10] = feedthrough_mem_inb[10];
// ----- Local connection due to Wire 25 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[11] = feedthrough_mem_inb[11];
// ----- Local connection due to Wire 26 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[12] = feedthrough_mem_inb[12];
// ----- Local connection due to Wire 27 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[13] = feedthrough_mem_inb[13];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size40_feedthrough_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size32_mem -----
module mux_2level_tapbuf_size32_mem(pReset,
                                    prog_clk,
                                    ccff_head,
                                    ccff_tail,
                                    mem_out,
                                    mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [11:0] mem_out;
//----- OUTPUT PORTS -----
output [11:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[11];
// ----- END Local output short connections -----

	DFFR DFFR_0_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFFR DFFR_1_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFFR DFFR_2_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	DFFR DFFR_3_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[2]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	DFFR DFFR_4_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[3]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	DFFR DFFR_5_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[4]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

	DFFR DFFR_6_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[5]),
		.Q(mem_out[6]),
		.QN(mem_outb[6]));

	DFFR DFFR_7_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[6]),
		.Q(mem_out[7]),
		.QN(mem_outb[7]));

	DFFR DFFR_8_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[7]),
		.Q(mem_out[8]),
		.QN(mem_outb[8]));

	DFFR DFFR_9_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[8]),
		.Q(mem_out[9]),
		.QN(mem_outb[9]));

	DFFR DFFR_10_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[9]),
		.Q(mem_out[10]),
		.QN(mem_outb[10]));

	DFFR DFFR_11_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[10]),
		.Q(mem_out[11]),
		.QN(mem_outb[11]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size32_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size32_feedthrough_mem -----
module mux_2level_tapbuf_size32_feedthrough_mem(feedthrough_mem_in,
                                                feedthrough_mem_inb,
                                                mem_out,
                                                mem_outb);
//----- INPUT PORTS -----
input [11:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [11:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [11:0] mem_out;
//----- OUTPUT PORTS -----
output [11:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[0] = feedthrough_mem_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[1] = feedthrough_mem_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[2] = feedthrough_mem_in[2];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[3] = feedthrough_mem_in[3];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[4] = feedthrough_mem_in[4];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[5] = feedthrough_mem_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[6] = feedthrough_mem_in[6];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[7] = feedthrough_mem_in[7];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[8] = feedthrough_mem_in[8];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[9] = feedthrough_mem_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[10] = feedthrough_mem_in[10];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[11] = feedthrough_mem_in[11];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[0] = feedthrough_mem_inb[0];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[1] = feedthrough_mem_inb[1];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[2] = feedthrough_mem_inb[2];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[3] = feedthrough_mem_inb[3];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[4] = feedthrough_mem_inb[4];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[5] = feedthrough_mem_inb[5];
// ----- Local connection due to Wire 18 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[6] = feedthrough_mem_inb[6];
// ----- Local connection due to Wire 19 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[7] = feedthrough_mem_inb[7];
// ----- Local connection due to Wire 20 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[8] = feedthrough_mem_inb[8];
// ----- Local connection due to Wire 21 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[9] = feedthrough_mem_inb[9];
// ----- Local connection due to Wire 22 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[10] = feedthrough_mem_inb[10];
// ----- Local connection due to Wire 23 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[11] = feedthrough_mem_inb[11];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size32_feedthrough_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size48_mem -----
module mux_2level_tapbuf_size48_mem(pReset,
                                    prog_clk,
                                    ccff_head,
                                    ccff_tail,
                                    mem_out,
                                    mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [13:0] mem_out;
//----- OUTPUT PORTS -----
output [13:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[13];
// ----- END Local output short connections -----

	DFFR DFFR_0_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFFR DFFR_1_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFFR DFFR_2_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	DFFR DFFR_3_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[2]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	DFFR DFFR_4_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[3]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	DFFR DFFR_5_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[4]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

	DFFR DFFR_6_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[5]),
		.Q(mem_out[6]),
		.QN(mem_outb[6]));

	DFFR DFFR_7_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[6]),
		.Q(mem_out[7]),
		.QN(mem_outb[7]));

	DFFR DFFR_8_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[7]),
		.Q(mem_out[8]),
		.QN(mem_outb[8]));

	DFFR DFFR_9_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[8]),
		.Q(mem_out[9]),
		.QN(mem_outb[9]));

	DFFR DFFR_10_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[9]),
		.Q(mem_out[10]),
		.QN(mem_outb[10]));

	DFFR DFFR_11_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[10]),
		.Q(mem_out[11]),
		.QN(mem_outb[11]));

	DFFR DFFR_12_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[11]),
		.Q(mem_out[12]),
		.QN(mem_outb[12]));

	DFFR DFFR_13_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[12]),
		.Q(mem_out[13]),
		.QN(mem_outb[13]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size48_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size48_feedthrough_mem -----
module mux_2level_tapbuf_size48_feedthrough_mem(feedthrough_mem_in,
                                                feedthrough_mem_inb,
                                                mem_out,
                                                mem_outb);
//----- INPUT PORTS -----
input [13:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [13:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [13:0] mem_out;
//----- OUTPUT PORTS -----
output [13:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[0] = feedthrough_mem_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[1] = feedthrough_mem_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[2] = feedthrough_mem_in[2];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[3] = feedthrough_mem_in[3];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[4] = feedthrough_mem_in[4];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[5] = feedthrough_mem_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[6] = feedthrough_mem_in[6];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[7] = feedthrough_mem_in[7];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[8] = feedthrough_mem_in[8];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[9] = feedthrough_mem_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[10] = feedthrough_mem_in[10];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[11] = feedthrough_mem_in[11];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[12] = feedthrough_mem_in[12];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[13] = feedthrough_mem_in[13];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[0] = feedthrough_mem_inb[0];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[1] = feedthrough_mem_inb[1];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[2] = feedthrough_mem_inb[2];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[3] = feedthrough_mem_inb[3];
// ----- Local connection due to Wire 18 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[4] = feedthrough_mem_inb[4];
// ----- Local connection due to Wire 19 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[5] = feedthrough_mem_inb[5];
// ----- Local connection due to Wire 20 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[6] = feedthrough_mem_inb[6];
// ----- Local connection due to Wire 21 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[7] = feedthrough_mem_inb[7];
// ----- Local connection due to Wire 22 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[8] = feedthrough_mem_inb[8];
// ----- Local connection due to Wire 23 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[9] = feedthrough_mem_inb[9];
// ----- Local connection due to Wire 24 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[10] = feedthrough_mem_inb[10];
// ----- Local connection due to Wire 25 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[11] = feedthrough_mem_inb[11];
// ----- Local connection due to Wire 26 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[12] = feedthrough_mem_inb[12];
// ----- Local connection due to Wire 27 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[13] = feedthrough_mem_inb[13];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size48_feedthrough_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size56_mem -----
module mux_2level_tapbuf_size56_mem(pReset,
                                    prog_clk,
                                    ccff_head,
                                    ccff_tail,
                                    mem_out,
                                    mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [15:0] mem_out;
//----- OUTPUT PORTS -----
output [15:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[15];
// ----- END Local output short connections -----

	DFFR DFFR_0_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFFR DFFR_1_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFFR DFFR_2_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	DFFR DFFR_3_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[2]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	DFFR DFFR_4_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[3]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	DFFR DFFR_5_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[4]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

	DFFR DFFR_6_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[5]),
		.Q(mem_out[6]),
		.QN(mem_outb[6]));

	DFFR DFFR_7_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[6]),
		.Q(mem_out[7]),
		.QN(mem_outb[7]));

	DFFR DFFR_8_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[7]),
		.Q(mem_out[8]),
		.QN(mem_outb[8]));

	DFFR DFFR_9_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[8]),
		.Q(mem_out[9]),
		.QN(mem_outb[9]));

	DFFR DFFR_10_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[9]),
		.Q(mem_out[10]),
		.QN(mem_outb[10]));

	DFFR DFFR_11_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[10]),
		.Q(mem_out[11]),
		.QN(mem_outb[11]));

	DFFR DFFR_12_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[11]),
		.Q(mem_out[12]),
		.QN(mem_outb[12]));

	DFFR DFFR_13_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[12]),
		.Q(mem_out[13]),
		.QN(mem_outb[13]));

	DFFR DFFR_14_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[13]),
		.Q(mem_out[14]),
		.QN(mem_outb[14]));

	DFFR DFFR_15_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[14]),
		.Q(mem_out[15]),
		.QN(mem_outb[15]));

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size56_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_tapbuf_size56_feedthrough_mem -----
module mux_2level_tapbuf_size56_feedthrough_mem(feedthrough_mem_in,
                                                feedthrough_mem_inb,
                                                mem_out,
                                                mem_outb);
//----- INPUT PORTS -----
input [15:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [15:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [15:0] mem_out;
//----- OUTPUT PORTS -----
output [15:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[0] = feedthrough_mem_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[1] = feedthrough_mem_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[2] = feedthrough_mem_in[2];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[3] = feedthrough_mem_in[3];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[4] = feedthrough_mem_in[4];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[5] = feedthrough_mem_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[6] = feedthrough_mem_in[6];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[7] = feedthrough_mem_in[7];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[8] = feedthrough_mem_in[8];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[9] = feedthrough_mem_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[10] = feedthrough_mem_in[10];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[11] = feedthrough_mem_in[11];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[12] = feedthrough_mem_in[12];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[13] = feedthrough_mem_in[13];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[14] = feedthrough_mem_in[14];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[15] = feedthrough_mem_in[15];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[0] = feedthrough_mem_inb[0];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[1] = feedthrough_mem_inb[1];
// ----- Local connection due to Wire 18 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[2] = feedthrough_mem_inb[2];
// ----- Local connection due to Wire 19 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[3] = feedthrough_mem_inb[3];
// ----- Local connection due to Wire 20 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[4] = feedthrough_mem_inb[4];
// ----- Local connection due to Wire 21 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[5] = feedthrough_mem_inb[5];
// ----- Local connection due to Wire 22 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[6] = feedthrough_mem_inb[6];
// ----- Local connection due to Wire 23 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[7] = feedthrough_mem_inb[7];
// ----- Local connection due to Wire 24 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[8] = feedthrough_mem_inb[8];
// ----- Local connection due to Wire 25 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[9] = feedthrough_mem_inb[9];
// ----- Local connection due to Wire 26 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[10] = feedthrough_mem_inb[10];
// ----- Local connection due to Wire 27 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[11] = feedthrough_mem_inb[11];
// ----- Local connection due to Wire 28 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[12] = feedthrough_mem_inb[12];
// ----- Local connection due to Wire 29 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[13] = feedthrough_mem_inb[13];
// ----- Local connection due to Wire 30 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[14] = feedthrough_mem_inb[14];
// ----- Local connection due to Wire 31 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[15] = feedthrough_mem_inb[15];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

endmodule
// ----- END Verilog module for mux_2level_tapbuf_size56_feedthrough_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_size20_mem -----
module mux_2level_size20_mem(pReset,
                             prog_clk,
                             ccff_head,
                             ccff_tail,
                             mem_out,
                             mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [9:0] mem_out;
//----- OUTPUT PORTS -----
output [9:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[9];
// ----- END Local output short connections -----

	DFFR DFFR_0_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFFR DFFR_1_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFFR DFFR_2_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	DFFR DFFR_3_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[2]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	DFFR DFFR_4_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[3]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	DFFR DFFR_5_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[4]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

	DFFR DFFR_6_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[5]),
		.Q(mem_out[6]),
		.QN(mem_outb[6]));

	DFFR DFFR_7_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[6]),
		.Q(mem_out[7]),
		.QN(mem_outb[7]));

	DFFR DFFR_8_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[7]),
		.Q(mem_out[8]),
		.QN(mem_outb[8]));

	DFFR DFFR_9_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[8]),
		.Q(mem_out[9]),
		.QN(mem_outb[9]));

endmodule
// ----- END Verilog module for mux_2level_size20_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_2level_size20_feedthrough_mem -----
module mux_2level_size20_feedthrough_mem(feedthrough_mem_in,
                                         feedthrough_mem_inb,
                                         mem_out,
                                         mem_outb);
//----- INPUT PORTS -----
input [9:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [9:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [9:0] mem_out;
//----- OUTPUT PORTS -----
output [9:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[0] = feedthrough_mem_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[1] = feedthrough_mem_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[2] = feedthrough_mem_in[2];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[3] = feedthrough_mem_in[3];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[4] = feedthrough_mem_in[4];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[5] = feedthrough_mem_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[6] = feedthrough_mem_in[6];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[7] = feedthrough_mem_in[7];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[8] = feedthrough_mem_in[8];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[9] = feedthrough_mem_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[0] = feedthrough_mem_inb[0];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[1] = feedthrough_mem_inb[1];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[2] = feedthrough_mem_inb[2];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[3] = feedthrough_mem_inb[3];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[4] = feedthrough_mem_inb[4];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[5] = feedthrough_mem_inb[5];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[6] = feedthrough_mem_inb[6];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[7] = feedthrough_mem_inb[7];
// ----- Local connection due to Wire 18 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[8] = feedthrough_mem_inb[8];
// ----- Local connection due to Wire 19 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[9] = feedthrough_mem_inb[9];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

endmodule
// ----- END Verilog module for mux_2level_size20_feedthrough_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_1level_tapbuf_size2_mem -----
module mux_1level_tapbuf_size2_mem(pReset,
                                   prog_clk,
                                   ccff_head,
                                   ccff_tail,
                                   mem_out,
                                   mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [2:0] mem_out;
//----- OUTPUT PORTS -----
output [2:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[2];
// ----- END Local output short connections -----

	DFFR DFFR_0_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFFR DFFR_1_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFFR DFFR_2_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

endmodule
// ----- END Verilog module for mux_1level_tapbuf_size2_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for mux_1level_tapbuf_size2_feedthrough_mem -----
module mux_1level_tapbuf_size2_feedthrough_mem(feedthrough_mem_in,
                                               feedthrough_mem_inb,
                                               mem_out,
                                               mem_outb);
//----- INPUT PORTS -----
input [2:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [2:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [2:0] mem_out;
//----- OUTPUT PORTS -----
output [2:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[0] = feedthrough_mem_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[1] = feedthrough_mem_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[2] = feedthrough_mem_in[2];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[0] = feedthrough_mem_inb[0];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[1] = feedthrough_mem_inb[1];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[2] = feedthrough_mem_inb[2];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

endmodule
// ----- END Verilog module for mux_1level_tapbuf_size2_feedthrough_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for frac_lut4_DFFR_mem -----
module frac_lut4_DFFR_mem(pReset,
                          prog_clk,
                          ccff_head,
                          ccff_tail,
                          mem_out,
                          mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;
//----- OUTPUT PORTS -----
output [16:0] mem_out;
//----- OUTPUT PORTS -----
output [16:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
	assign ccff_tail[0] = mem_out[16];
// ----- END Local output short connections -----

	DFFR DFFR_0_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out[0]),
		.QN(mem_outb[0]));

	DFFR DFFR_1_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[0]),
		.Q(mem_out[1]),
		.QN(mem_outb[1]));

	DFFR DFFR_2_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[1]),
		.Q(mem_out[2]),
		.QN(mem_outb[2]));

	DFFR DFFR_3_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[2]),
		.Q(mem_out[3]),
		.QN(mem_outb[3]));

	DFFR DFFR_4_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[3]),
		.Q(mem_out[4]),
		.QN(mem_outb[4]));

	DFFR DFFR_5_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[4]),
		.Q(mem_out[5]),
		.QN(mem_outb[5]));

	DFFR DFFR_6_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[5]),
		.Q(mem_out[6]),
		.QN(mem_outb[6]));

	DFFR DFFR_7_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[6]),
		.Q(mem_out[7]),
		.QN(mem_outb[7]));

	DFFR DFFR_8_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[7]),
		.Q(mem_out[8]),
		.QN(mem_outb[8]));

	DFFR DFFR_9_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[8]),
		.Q(mem_out[9]),
		.QN(mem_outb[9]));

	DFFR DFFR_10_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[9]),
		.Q(mem_out[10]),
		.QN(mem_outb[10]));

	DFFR DFFR_11_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[10]),
		.Q(mem_out[11]),
		.QN(mem_outb[11]));

	DFFR DFFR_12_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[11]),
		.Q(mem_out[12]),
		.QN(mem_outb[12]));

	DFFR DFFR_13_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[12]),
		.Q(mem_out[13]),
		.QN(mem_outb[13]));

	DFFR DFFR_14_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[13]),
		.Q(mem_out[14]),
		.QN(mem_outb[14]));

	DFFR DFFR_15_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[14]),
		.Q(mem_out[15]),
		.QN(mem_outb[15]));

	DFFR DFFR_16_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(mem_out[15]),
		.Q(mem_out[16]),
		.QN(mem_outb[16]));

endmodule
// ----- END Verilog module for frac_lut4_DFFR_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for frac_lut4_feedthrough_DFFR_mem -----
module frac_lut4_feedthrough_DFFR_mem(feedthrough_mem_in,
                                      feedthrough_mem_inb,
                                      mem_out,
                                      mem_outb);
//----- INPUT PORTS -----
input [16:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [16:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [16:0] mem_out;
//----- OUTPUT PORTS -----
output [16:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[0] = feedthrough_mem_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[1] = feedthrough_mem_in[1];
// ----- Local connection due to Wire 2 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[2] = feedthrough_mem_in[2];
// ----- Local connection due to Wire 3 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[3] = feedthrough_mem_in[3];
// ----- Local connection due to Wire 4 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[4] = feedthrough_mem_in[4];
// ----- Local connection due to Wire 5 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[5] = feedthrough_mem_in[5];
// ----- Local connection due to Wire 6 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[6] = feedthrough_mem_in[6];
// ----- Local connection due to Wire 7 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[7] = feedthrough_mem_in[7];
// ----- Local connection due to Wire 8 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[8] = feedthrough_mem_in[8];
// ----- Local connection due to Wire 9 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[9] = feedthrough_mem_in[9];
// ----- Local connection due to Wire 10 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[10] = feedthrough_mem_in[10];
// ----- Local connection due to Wire 11 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[11] = feedthrough_mem_in[11];
// ----- Local connection due to Wire 12 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[12] = feedthrough_mem_in[12];
// ----- Local connection due to Wire 13 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[13] = feedthrough_mem_in[13];
// ----- Local connection due to Wire 14 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[14] = feedthrough_mem_in[14];
// ----- Local connection due to Wire 15 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[15] = feedthrough_mem_in[15];
// ----- Local connection due to Wire 16 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[16] = feedthrough_mem_in[16];
// ----- Local connection due to Wire 17 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[0] = feedthrough_mem_inb[0];
// ----- Local connection due to Wire 18 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[1] = feedthrough_mem_inb[1];
// ----- Local connection due to Wire 19 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[2] = feedthrough_mem_inb[2];
// ----- Local connection due to Wire 20 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[3] = feedthrough_mem_inb[3];
// ----- Local connection due to Wire 21 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[4] = feedthrough_mem_inb[4];
// ----- Local connection due to Wire 22 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[5] = feedthrough_mem_inb[5];
// ----- Local connection due to Wire 23 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[6] = feedthrough_mem_inb[6];
// ----- Local connection due to Wire 24 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[7] = feedthrough_mem_inb[7];
// ----- Local connection due to Wire 25 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[8] = feedthrough_mem_inb[8];
// ----- Local connection due to Wire 26 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[9] = feedthrough_mem_inb[9];
// ----- Local connection due to Wire 27 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[10] = feedthrough_mem_inb[10];
// ----- Local connection due to Wire 28 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[11] = feedthrough_mem_inb[11];
// ----- Local connection due to Wire 29 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[12] = feedthrough_mem_inb[12];
// ----- Local connection due to Wire 30 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[13] = feedthrough_mem_inb[13];
// ----- Local connection due to Wire 31 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[14] = feedthrough_mem_inb[14];
// ----- Local connection due to Wire 32 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[15] = feedthrough_mem_inb[15];
// ----- Local connection due to Wire 33 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[16] = feedthrough_mem_inb[16];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

endmodule
// ----- END Verilog module for frac_lut4_feedthrough_DFFR_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for GPIO_DFFR_mem -----
module GPIO_DFFR_mem(pReset,
                     prog_clk,
                     ccff_head,
                     ccff_tail,
                     mem_out,
                     mem_outb);
//----- GLOBAL PORTS -----
input [0:0] pReset;
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

	DFFR DFFR_0_ (
		.RST(pReset),
		.CK(prog_clk),
		.D(ccff_head),
		.Q(mem_out),
		.QN(mem_outb));

endmodule
// ----- END Verilog module for GPIO_DFFR_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for GPIO_feedthrough_DFFR_mem -----
module GPIO_feedthrough_DFFR_mem(feedthrough_mem_in,
                                 feedthrough_mem_inb,
                                 mem_out,
                                 mem_outb);
//----- INPUT PORTS -----
input [0:0] feedthrough_mem_in;
//----- INPUT PORTS -----
input [0:0] feedthrough_mem_inb;
//----- OUTPUT PORTS -----
output [0:0] mem_out;
//----- OUTPUT PORTS -----
output [0:0] mem_outb;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- Local connection due to Wire 0 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_out[0] = feedthrough_mem_in[0];
// ----- Local connection due to Wire 1 -----
// ----- Net source id 0 -----
// ----- Net sink id 0 -----
	assign mem_outb[0] = feedthrough_mem_inb[0];
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

endmodule
// ----- END Verilog module for GPIO_feedthrough_DFFR_mem -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_clb_config_group_mem_size264 -----
module grid_clb_config_group_mem_size264(pReset,
                                         prog_clk,
                                         ccff_head,
                                         mem_out,
                                         mem_outb,
                                         ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [263:0] mem_out;
//----- OUTPUT PORTS -----
output [263:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] frac_lut4_DFFR_mem_0_ccff_tail;
wire [0:0] frac_lut4_DFFR_mem_1_ccff_tail;
wire [0:0] frac_lut4_DFFR_mem_2_ccff_tail;
wire [0:0] frac_lut4_DFFR_mem_3_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_0_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_10_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_11_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_1_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_2_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_3_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_4_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_5_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_6_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_7_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_8_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_9_ccff_tail;
wire [0:0] mux_2level_size20_mem_0_ccff_tail;
wire [0:0] mux_2level_size20_mem_10_ccff_tail;
wire [0:0] mux_2level_size20_mem_11_ccff_tail;
wire [0:0] mux_2level_size20_mem_12_ccff_tail;
wire [0:0] mux_2level_size20_mem_13_ccff_tail;
wire [0:0] mux_2level_size20_mem_14_ccff_tail;
wire [0:0] mux_2level_size20_mem_1_ccff_tail;
wire [0:0] mux_2level_size20_mem_2_ccff_tail;
wire [0:0] mux_2level_size20_mem_3_ccff_tail;
wire [0:0] mux_2level_size20_mem_4_ccff_tail;
wire [0:0] mux_2level_size20_mem_5_ccff_tail;
wire [0:0] mux_2level_size20_mem_6_ccff_tail;
wire [0:0] mux_2level_size20_mem_7_ccff_tail;
wire [0:0] mux_2level_size20_mem_8_ccff_tail;
wire [0:0] mux_2level_size20_mem_9_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	frac_lut4_DFFR_mem frac_lut4_DFFR_mem (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(frac_lut4_DFFR_mem_0_ccff_tail),
		.mem_out({mem_out[16], mem_out[15], mem_out[14], mem_out[13], mem_out[12], mem_out[11], mem_out[10], mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[16], mem_outb[15], mem_outb[14], mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10], mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	frac_lut4_DFFR_mem frac_lut4_DFFR_mem_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_2_ccff_tail),
		.ccff_tail(frac_lut4_DFFR_mem_1_ccff_tail),
		.mem_out({mem_out[42], mem_out[41], mem_out[40], mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30], mem_out[29], mem_out[28], mem_out[27], mem_out[26]}),
		.mem_outb({mem_outb[42], mem_outb[41], mem_outb[40], mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30], mem_outb[29], mem_outb[28], mem_outb[27], mem_outb[26]}));

	frac_lut4_DFFR_mem frac_lut4_DFFR_mem_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_5_ccff_tail),
		.ccff_tail(frac_lut4_DFFR_mem_2_ccff_tail),
		.mem_out({mem_out[68], mem_out[67], mem_out[66], mem_out[65], mem_out[64], mem_out[63], mem_out[62], mem_out[61], mem_out[60], mem_out[59], mem_out[58], mem_out[57], mem_out[56], mem_out[55], mem_out[54], mem_out[53], mem_out[52]}),
		.mem_outb({mem_outb[68], mem_outb[67], mem_outb[66], mem_outb[65], mem_outb[64], mem_outb[63], mem_outb[62], mem_outb[61], mem_outb[60], mem_outb[59], mem_outb[58], mem_outb[57], mem_outb[56], mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52]}));

	frac_lut4_DFFR_mem frac_lut4_DFFR_mem_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_8_ccff_tail),
		.ccff_tail(frac_lut4_DFFR_mem_3_ccff_tail),
		.mem_out({mem_out[94], mem_out[93], mem_out[92], mem_out[91], mem_out[90], mem_out[89], mem_out[88], mem_out[87], mem_out[86], mem_out[85], mem_out[84], mem_out[83], mem_out[82], mem_out[81], mem_out[80], mem_out[79], mem_out[78]}),
		.mem_outb({mem_outb[94], mem_outb[93], mem_outb[92], mem_outb[91], mem_outb[90], mem_outb[89], mem_outb[88], mem_outb[87], mem_outb[86], mem_outb[85], mem_outb[84], mem_outb[83], mem_outb[82], mem_outb[81], mem_outb[80], mem_outb[79], mem_outb[78]}));

	mux_1level_tapbuf_size2_mem mem_frac_logic_out_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(frac_lut4_DFFR_mem_0_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_0_ccff_tail),
		.mem_out({mem_out[19], mem_out[18], mem_out[17]}),
		.mem_outb({mem_outb[19], mem_outb[18], mem_outb[17]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_0_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_1_ccff_tail),
		.mem_out({mem_out[22], mem_out[21], mem_out[20]}),
		.mem_outb({mem_outb[22], mem_outb[21], mem_outb[20]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_1_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_2_ccff_tail),
		.mem_out({mem_out[25], mem_out[24], mem_out[23]}),
		.mem_outb({mem_outb[25], mem_outb[24], mem_outb[23]}));

	mux_1level_tapbuf_size2_mem mem_frac_logic_out_0_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(frac_lut4_DFFR_mem_1_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_3_ccff_tail),
		.mem_out({mem_out[45], mem_out[44], mem_out[43]}),
		.mem_outb({mem_outb[45], mem_outb[44], mem_outb[43]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_0_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_3_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_4_ccff_tail),
		.mem_out({mem_out[48], mem_out[47], mem_out[46]}),
		.mem_outb({mem_outb[48], mem_outb[47], mem_outb[46]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_1_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_4_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_5_ccff_tail),
		.mem_out({mem_out[51], mem_out[50], mem_out[49]}),
		.mem_outb({mem_outb[51], mem_outb[50], mem_outb[49]}));

	mux_1level_tapbuf_size2_mem mem_frac_logic_out_0_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(frac_lut4_DFFR_mem_2_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_6_ccff_tail),
		.mem_out({mem_out[71], mem_out[70], mem_out[69]}),
		.mem_outb({mem_outb[71], mem_outb[70], mem_outb[69]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_0_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_6_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_7_ccff_tail),
		.mem_out({mem_out[74], mem_out[73], mem_out[72]}),
		.mem_outb({mem_outb[74], mem_outb[73], mem_outb[72]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_1_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_7_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_8_ccff_tail),
		.mem_out({mem_out[77], mem_out[76], mem_out[75]}),
		.mem_outb({mem_outb[77], mem_outb[76], mem_outb[75]}));

	mux_1level_tapbuf_size2_mem mem_frac_logic_out_0_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(frac_lut4_DFFR_mem_3_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_9_ccff_tail),
		.mem_out({mem_out[97], mem_out[96], mem_out[95]}),
		.mem_outb({mem_outb[97], mem_outb[96], mem_outb[95]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_0_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_9_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_10_ccff_tail),
		.mem_out({mem_out[100], mem_out[99], mem_out[98]}),
		.mem_outb({mem_outb[100], mem_outb[99], mem_outb[98]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_1_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_10_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_11_ccff_tail),
		.mem_out({mem_out[103], mem_out[102], mem_out[101]}),
		.mem_outb({mem_outb[103], mem_outb[102], mem_outb[101]}));

	mux_2level_size20_mem mem_fle_0_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_11_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_0_ccff_tail),
		.mem_out({mem_out[113], mem_out[112], mem_out[111], mem_out[110], mem_out[109], mem_out[108], mem_out[107], mem_out[106], mem_out[105], mem_out[104]}),
		.mem_outb({mem_outb[113], mem_outb[112], mem_outb[111], mem_outb[110], mem_outb[109], mem_outb[108], mem_outb[107], mem_outb[106], mem_outb[105], mem_outb[104]}));

	mux_2level_size20_mem mem_fle_0_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_0_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_1_ccff_tail),
		.mem_out({mem_out[123], mem_out[122], mem_out[121], mem_out[120], mem_out[119], mem_out[118], mem_out[117], mem_out[116], mem_out[115], mem_out[114]}),
		.mem_outb({mem_outb[123], mem_outb[122], mem_outb[121], mem_outb[120], mem_outb[119], mem_outb[118], mem_outb[117], mem_outb[116], mem_outb[115], mem_outb[114]}));

	mux_2level_size20_mem mem_fle_0_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_1_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_2_ccff_tail),
		.mem_out({mem_out[133], mem_out[132], mem_out[131], mem_out[130], mem_out[129], mem_out[128], mem_out[127], mem_out[126], mem_out[125], mem_out[124]}),
		.mem_outb({mem_outb[133], mem_outb[132], mem_outb[131], mem_outb[130], mem_outb[129], mem_outb[128], mem_outb[127], mem_outb[126], mem_outb[125], mem_outb[124]}));

	mux_2level_size20_mem mem_fle_0_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_2_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_3_ccff_tail),
		.mem_out({mem_out[143], mem_out[142], mem_out[141], mem_out[140], mem_out[139], mem_out[138], mem_out[137], mem_out[136], mem_out[135], mem_out[134]}),
		.mem_outb({mem_outb[143], mem_outb[142], mem_outb[141], mem_outb[140], mem_outb[139], mem_outb[138], mem_outb[137], mem_outb[136], mem_outb[135], mem_outb[134]}));

	mux_2level_size20_mem mem_fle_1_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_3_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_4_ccff_tail),
		.mem_out({mem_out[153], mem_out[152], mem_out[151], mem_out[150], mem_out[149], mem_out[148], mem_out[147], mem_out[146], mem_out[145], mem_out[144]}),
		.mem_outb({mem_outb[153], mem_outb[152], mem_outb[151], mem_outb[150], mem_outb[149], mem_outb[148], mem_outb[147], mem_outb[146], mem_outb[145], mem_outb[144]}));

	mux_2level_size20_mem mem_fle_1_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_4_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_5_ccff_tail),
		.mem_out({mem_out[163], mem_out[162], mem_out[161], mem_out[160], mem_out[159], mem_out[158], mem_out[157], mem_out[156], mem_out[155], mem_out[154]}),
		.mem_outb({mem_outb[163], mem_outb[162], mem_outb[161], mem_outb[160], mem_outb[159], mem_outb[158], mem_outb[157], mem_outb[156], mem_outb[155], mem_outb[154]}));

	mux_2level_size20_mem mem_fle_1_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_5_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_6_ccff_tail),
		.mem_out({mem_out[173], mem_out[172], mem_out[171], mem_out[170], mem_out[169], mem_out[168], mem_out[167], mem_out[166], mem_out[165], mem_out[164]}),
		.mem_outb({mem_outb[173], mem_outb[172], mem_outb[171], mem_outb[170], mem_outb[169], mem_outb[168], mem_outb[167], mem_outb[166], mem_outb[165], mem_outb[164]}));

	mux_2level_size20_mem mem_fle_1_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_6_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_7_ccff_tail),
		.mem_out({mem_out[183], mem_out[182], mem_out[181], mem_out[180], mem_out[179], mem_out[178], mem_out[177], mem_out[176], mem_out[175], mem_out[174]}),
		.mem_outb({mem_outb[183], mem_outb[182], mem_outb[181], mem_outb[180], mem_outb[179], mem_outb[178], mem_outb[177], mem_outb[176], mem_outb[175], mem_outb[174]}));

	mux_2level_size20_mem mem_fle_2_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_7_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_8_ccff_tail),
		.mem_out({mem_out[193], mem_out[192], mem_out[191], mem_out[190], mem_out[189], mem_out[188], mem_out[187], mem_out[186], mem_out[185], mem_out[184]}),
		.mem_outb({mem_outb[193], mem_outb[192], mem_outb[191], mem_outb[190], mem_outb[189], mem_outb[188], mem_outb[187], mem_outb[186], mem_outb[185], mem_outb[184]}));

	mux_2level_size20_mem mem_fle_2_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_8_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_9_ccff_tail),
		.mem_out({mem_out[203], mem_out[202], mem_out[201], mem_out[200], mem_out[199], mem_out[198], mem_out[197], mem_out[196], mem_out[195], mem_out[194]}),
		.mem_outb({mem_outb[203], mem_outb[202], mem_outb[201], mem_outb[200], mem_outb[199], mem_outb[198], mem_outb[197], mem_outb[196], mem_outb[195], mem_outb[194]}));

	mux_2level_size20_mem mem_fle_2_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_9_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_10_ccff_tail),
		.mem_out({mem_out[213], mem_out[212], mem_out[211], mem_out[210], mem_out[209], mem_out[208], mem_out[207], mem_out[206], mem_out[205], mem_out[204]}),
		.mem_outb({mem_outb[213], mem_outb[212], mem_outb[211], mem_outb[210], mem_outb[209], mem_outb[208], mem_outb[207], mem_outb[206], mem_outb[205], mem_outb[204]}));

	mux_2level_size20_mem mem_fle_2_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_10_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_11_ccff_tail),
		.mem_out({mem_out[223], mem_out[222], mem_out[221], mem_out[220], mem_out[219], mem_out[218], mem_out[217], mem_out[216], mem_out[215], mem_out[214]}),
		.mem_outb({mem_outb[223], mem_outb[222], mem_outb[221], mem_outb[220], mem_outb[219], mem_outb[218], mem_outb[217], mem_outb[216], mem_outb[215], mem_outb[214]}));

	mux_2level_size20_mem mem_fle_3_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_11_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_12_ccff_tail),
		.mem_out({mem_out[233], mem_out[232], mem_out[231], mem_out[230], mem_out[229], mem_out[228], mem_out[227], mem_out[226], mem_out[225], mem_out[224]}),
		.mem_outb({mem_outb[233], mem_outb[232], mem_outb[231], mem_outb[230], mem_outb[229], mem_outb[228], mem_outb[227], mem_outb[226], mem_outb[225], mem_outb[224]}));

	mux_2level_size20_mem mem_fle_3_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_12_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_13_ccff_tail),
		.mem_out({mem_out[243], mem_out[242], mem_out[241], mem_out[240], mem_out[239], mem_out[238], mem_out[237], mem_out[236], mem_out[235], mem_out[234]}),
		.mem_outb({mem_outb[243], mem_outb[242], mem_outb[241], mem_outb[240], mem_outb[239], mem_outb[238], mem_outb[237], mem_outb[236], mem_outb[235], mem_outb[234]}));

	mux_2level_size20_mem mem_fle_3_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_13_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_14_ccff_tail),
		.mem_out({mem_out[253], mem_out[252], mem_out[251], mem_out[250], mem_out[249], mem_out[248], mem_out[247], mem_out[246], mem_out[245], mem_out[244]}),
		.mem_outb({mem_outb[253], mem_outb[252], mem_outb[251], mem_outb[250], mem_outb[249], mem_outb[248], mem_outb[247], mem_outb[246], mem_outb[245], mem_outb[244]}));

	mux_2level_size20_mem mem_fle_3_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_14_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[263], mem_out[262], mem_out[261], mem_out[260], mem_out[259], mem_out[258], mem_out[257], mem_out[256], mem_out[255], mem_out[254]}),
		.mem_outb({mem_outb[263], mem_outb[262], mem_outb[261], mem_outb[260], mem_outb[259], mem_outb[258], mem_outb[257], mem_outb[256], mem_outb[255], mem_outb[254]}));

endmodule
// ----- END Verilog module for grid_clb_config_group_mem_size264 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_clb_tile_config_group_mem_size264 -----
module grid_clb_tile_config_group_mem_size264(pReset,
                                              prog_clk,
                                              ccff_head,
                                              mem_out,
                                              mem_outb,
                                              ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [263:0] mem_out;
//----- OUTPUT PORTS -----
output [263:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] frac_lut4_DFFR_mem_0_ccff_tail;
wire [0:0] frac_lut4_DFFR_mem_1_ccff_tail;
wire [0:0] frac_lut4_DFFR_mem_2_ccff_tail;
wire [0:0] frac_lut4_DFFR_mem_3_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_0_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_10_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_11_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_1_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_2_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_3_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_4_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_5_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_6_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_7_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_8_ccff_tail;
wire [0:0] mux_1level_tapbuf_size2_mem_9_ccff_tail;
wire [0:0] mux_2level_size20_mem_0_ccff_tail;
wire [0:0] mux_2level_size20_mem_10_ccff_tail;
wire [0:0] mux_2level_size20_mem_11_ccff_tail;
wire [0:0] mux_2level_size20_mem_12_ccff_tail;
wire [0:0] mux_2level_size20_mem_13_ccff_tail;
wire [0:0] mux_2level_size20_mem_14_ccff_tail;
wire [0:0] mux_2level_size20_mem_1_ccff_tail;
wire [0:0] mux_2level_size20_mem_2_ccff_tail;
wire [0:0] mux_2level_size20_mem_3_ccff_tail;
wire [0:0] mux_2level_size20_mem_4_ccff_tail;
wire [0:0] mux_2level_size20_mem_5_ccff_tail;
wire [0:0] mux_2level_size20_mem_6_ccff_tail;
wire [0:0] mux_2level_size20_mem_7_ccff_tail;
wire [0:0] mux_2level_size20_mem_8_ccff_tail;
wire [0:0] mux_2level_size20_mem_9_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	frac_lut4_DFFR_mem frac_lut4_DFFR_mem (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(frac_lut4_DFFR_mem_0_ccff_tail),
		.mem_out({mem_out[16], mem_out[15], mem_out[14], mem_out[13], mem_out[12], mem_out[11], mem_out[10], mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[16], mem_outb[15], mem_outb[14], mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10], mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	frac_lut4_DFFR_mem frac_lut4_DFFR_mem_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_2_ccff_tail),
		.ccff_tail(frac_lut4_DFFR_mem_1_ccff_tail),
		.mem_out({mem_out[42], mem_out[41], mem_out[40], mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30], mem_out[29], mem_out[28], mem_out[27], mem_out[26]}),
		.mem_outb({mem_outb[42], mem_outb[41], mem_outb[40], mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30], mem_outb[29], mem_outb[28], mem_outb[27], mem_outb[26]}));

	frac_lut4_DFFR_mem frac_lut4_DFFR_mem_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_5_ccff_tail),
		.ccff_tail(frac_lut4_DFFR_mem_2_ccff_tail),
		.mem_out({mem_out[68], mem_out[67], mem_out[66], mem_out[65], mem_out[64], mem_out[63], mem_out[62], mem_out[61], mem_out[60], mem_out[59], mem_out[58], mem_out[57], mem_out[56], mem_out[55], mem_out[54], mem_out[53], mem_out[52]}),
		.mem_outb({mem_outb[68], mem_outb[67], mem_outb[66], mem_outb[65], mem_outb[64], mem_outb[63], mem_outb[62], mem_outb[61], mem_outb[60], mem_outb[59], mem_outb[58], mem_outb[57], mem_outb[56], mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52]}));

	frac_lut4_DFFR_mem frac_lut4_DFFR_mem_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_8_ccff_tail),
		.ccff_tail(frac_lut4_DFFR_mem_3_ccff_tail),
		.mem_out({mem_out[94], mem_out[93], mem_out[92], mem_out[91], mem_out[90], mem_out[89], mem_out[88], mem_out[87], mem_out[86], mem_out[85], mem_out[84], mem_out[83], mem_out[82], mem_out[81], mem_out[80], mem_out[79], mem_out[78]}),
		.mem_outb({mem_outb[94], mem_outb[93], mem_outb[92], mem_outb[91], mem_outb[90], mem_outb[89], mem_outb[88], mem_outb[87], mem_outb[86], mem_outb[85], mem_outb[84], mem_outb[83], mem_outb[82], mem_outb[81], mem_outb[80], mem_outb[79], mem_outb[78]}));

	mux_1level_tapbuf_size2_mem mem_frac_logic_out_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(frac_lut4_DFFR_mem_0_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_0_ccff_tail),
		.mem_out({mem_out[19], mem_out[18], mem_out[17]}),
		.mem_outb({mem_outb[19], mem_outb[18], mem_outb[17]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_0_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_1_ccff_tail),
		.mem_out({mem_out[22], mem_out[21], mem_out[20]}),
		.mem_outb({mem_outb[22], mem_outb[21], mem_outb[20]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_1_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_2_ccff_tail),
		.mem_out({mem_out[25], mem_out[24], mem_out[23]}),
		.mem_outb({mem_outb[25], mem_outb[24], mem_outb[23]}));

	mux_1level_tapbuf_size2_mem mem_frac_logic_out_0_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(frac_lut4_DFFR_mem_1_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_3_ccff_tail),
		.mem_out({mem_out[45], mem_out[44], mem_out[43]}),
		.mem_outb({mem_outb[45], mem_outb[44], mem_outb[43]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_0_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_3_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_4_ccff_tail),
		.mem_out({mem_out[48], mem_out[47], mem_out[46]}),
		.mem_outb({mem_outb[48], mem_outb[47], mem_outb[46]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_1_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_4_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_5_ccff_tail),
		.mem_out({mem_out[51], mem_out[50], mem_out[49]}),
		.mem_outb({mem_outb[51], mem_outb[50], mem_outb[49]}));

	mux_1level_tapbuf_size2_mem mem_frac_logic_out_0_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(frac_lut4_DFFR_mem_2_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_6_ccff_tail),
		.mem_out({mem_out[71], mem_out[70], mem_out[69]}),
		.mem_outb({mem_outb[71], mem_outb[70], mem_outb[69]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_0_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_6_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_7_ccff_tail),
		.mem_out({mem_out[74], mem_out[73], mem_out[72]}),
		.mem_outb({mem_outb[74], mem_outb[73], mem_outb[72]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_1_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_7_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_8_ccff_tail),
		.mem_out({mem_out[77], mem_out[76], mem_out[75]}),
		.mem_outb({mem_outb[77], mem_outb[76], mem_outb[75]}));

	mux_1level_tapbuf_size2_mem mem_frac_logic_out_0_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(frac_lut4_DFFR_mem_3_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_9_ccff_tail),
		.mem_out({mem_out[97], mem_out[96], mem_out[95]}),
		.mem_outb({mem_outb[97], mem_outb[96], mem_outb[95]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_0_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_9_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_10_ccff_tail),
		.mem_out({mem_out[100], mem_out[99], mem_out[98]}),
		.mem_outb({mem_outb[100], mem_outb[99], mem_outb[98]}));

	mux_1level_tapbuf_size2_mem mem_fabric_out_1_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_10_ccff_tail),
		.ccff_tail(mux_1level_tapbuf_size2_mem_11_ccff_tail),
		.mem_out({mem_out[103], mem_out[102], mem_out[101]}),
		.mem_outb({mem_outb[103], mem_outb[102], mem_outb[101]}));

	mux_2level_size20_mem mem_fle_0_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_1level_tapbuf_size2_mem_11_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_0_ccff_tail),
		.mem_out({mem_out[113], mem_out[112], mem_out[111], mem_out[110], mem_out[109], mem_out[108], mem_out[107], mem_out[106], mem_out[105], mem_out[104]}),
		.mem_outb({mem_outb[113], mem_outb[112], mem_outb[111], mem_outb[110], mem_outb[109], mem_outb[108], mem_outb[107], mem_outb[106], mem_outb[105], mem_outb[104]}));

	mux_2level_size20_mem mem_fle_0_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_0_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_1_ccff_tail),
		.mem_out({mem_out[123], mem_out[122], mem_out[121], mem_out[120], mem_out[119], mem_out[118], mem_out[117], mem_out[116], mem_out[115], mem_out[114]}),
		.mem_outb({mem_outb[123], mem_outb[122], mem_outb[121], mem_outb[120], mem_outb[119], mem_outb[118], mem_outb[117], mem_outb[116], mem_outb[115], mem_outb[114]}));

	mux_2level_size20_mem mem_fle_0_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_1_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_2_ccff_tail),
		.mem_out({mem_out[133], mem_out[132], mem_out[131], mem_out[130], mem_out[129], mem_out[128], mem_out[127], mem_out[126], mem_out[125], mem_out[124]}),
		.mem_outb({mem_outb[133], mem_outb[132], mem_outb[131], mem_outb[130], mem_outb[129], mem_outb[128], mem_outb[127], mem_outb[126], mem_outb[125], mem_outb[124]}));

	mux_2level_size20_mem mem_fle_0_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_2_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_3_ccff_tail),
		.mem_out({mem_out[143], mem_out[142], mem_out[141], mem_out[140], mem_out[139], mem_out[138], mem_out[137], mem_out[136], mem_out[135], mem_out[134]}),
		.mem_outb({mem_outb[143], mem_outb[142], mem_outb[141], mem_outb[140], mem_outb[139], mem_outb[138], mem_outb[137], mem_outb[136], mem_outb[135], mem_outb[134]}));

	mux_2level_size20_mem mem_fle_1_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_3_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_4_ccff_tail),
		.mem_out({mem_out[153], mem_out[152], mem_out[151], mem_out[150], mem_out[149], mem_out[148], mem_out[147], mem_out[146], mem_out[145], mem_out[144]}),
		.mem_outb({mem_outb[153], mem_outb[152], mem_outb[151], mem_outb[150], mem_outb[149], mem_outb[148], mem_outb[147], mem_outb[146], mem_outb[145], mem_outb[144]}));

	mux_2level_size20_mem mem_fle_1_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_4_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_5_ccff_tail),
		.mem_out({mem_out[163], mem_out[162], mem_out[161], mem_out[160], mem_out[159], mem_out[158], mem_out[157], mem_out[156], mem_out[155], mem_out[154]}),
		.mem_outb({mem_outb[163], mem_outb[162], mem_outb[161], mem_outb[160], mem_outb[159], mem_outb[158], mem_outb[157], mem_outb[156], mem_outb[155], mem_outb[154]}));

	mux_2level_size20_mem mem_fle_1_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_5_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_6_ccff_tail),
		.mem_out({mem_out[173], mem_out[172], mem_out[171], mem_out[170], mem_out[169], mem_out[168], mem_out[167], mem_out[166], mem_out[165], mem_out[164]}),
		.mem_outb({mem_outb[173], mem_outb[172], mem_outb[171], mem_outb[170], mem_outb[169], mem_outb[168], mem_outb[167], mem_outb[166], mem_outb[165], mem_outb[164]}));

	mux_2level_size20_mem mem_fle_1_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_6_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_7_ccff_tail),
		.mem_out({mem_out[183], mem_out[182], mem_out[181], mem_out[180], mem_out[179], mem_out[178], mem_out[177], mem_out[176], mem_out[175], mem_out[174]}),
		.mem_outb({mem_outb[183], mem_outb[182], mem_outb[181], mem_outb[180], mem_outb[179], mem_outb[178], mem_outb[177], mem_outb[176], mem_outb[175], mem_outb[174]}));

	mux_2level_size20_mem mem_fle_2_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_7_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_8_ccff_tail),
		.mem_out({mem_out[193], mem_out[192], mem_out[191], mem_out[190], mem_out[189], mem_out[188], mem_out[187], mem_out[186], mem_out[185], mem_out[184]}),
		.mem_outb({mem_outb[193], mem_outb[192], mem_outb[191], mem_outb[190], mem_outb[189], mem_outb[188], mem_outb[187], mem_outb[186], mem_outb[185], mem_outb[184]}));

	mux_2level_size20_mem mem_fle_2_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_8_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_9_ccff_tail),
		.mem_out({mem_out[203], mem_out[202], mem_out[201], mem_out[200], mem_out[199], mem_out[198], mem_out[197], mem_out[196], mem_out[195], mem_out[194]}),
		.mem_outb({mem_outb[203], mem_outb[202], mem_outb[201], mem_outb[200], mem_outb[199], mem_outb[198], mem_outb[197], mem_outb[196], mem_outb[195], mem_outb[194]}));

	mux_2level_size20_mem mem_fle_2_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_9_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_10_ccff_tail),
		.mem_out({mem_out[213], mem_out[212], mem_out[211], mem_out[210], mem_out[209], mem_out[208], mem_out[207], mem_out[206], mem_out[205], mem_out[204]}),
		.mem_outb({mem_outb[213], mem_outb[212], mem_outb[211], mem_outb[210], mem_outb[209], mem_outb[208], mem_outb[207], mem_outb[206], mem_outb[205], mem_outb[204]}));

	mux_2level_size20_mem mem_fle_2_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_10_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_11_ccff_tail),
		.mem_out({mem_out[223], mem_out[222], mem_out[221], mem_out[220], mem_out[219], mem_out[218], mem_out[217], mem_out[216], mem_out[215], mem_out[214]}),
		.mem_outb({mem_outb[223], mem_outb[222], mem_outb[221], mem_outb[220], mem_outb[219], mem_outb[218], mem_outb[217], mem_outb[216], mem_outb[215], mem_outb[214]}));

	mux_2level_size20_mem mem_fle_3_in_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_11_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_12_ccff_tail),
		.mem_out({mem_out[233], mem_out[232], mem_out[231], mem_out[230], mem_out[229], mem_out[228], mem_out[227], mem_out[226], mem_out[225], mem_out[224]}),
		.mem_outb({mem_outb[233], mem_outb[232], mem_outb[231], mem_outb[230], mem_outb[229], mem_outb[228], mem_outb[227], mem_outb[226], mem_outb[225], mem_outb[224]}));

	mux_2level_size20_mem mem_fle_3_in_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_12_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_13_ccff_tail),
		.mem_out({mem_out[243], mem_out[242], mem_out[241], mem_out[240], mem_out[239], mem_out[238], mem_out[237], mem_out[236], mem_out[235], mem_out[234]}),
		.mem_outb({mem_outb[243], mem_outb[242], mem_outb[241], mem_outb[240], mem_outb[239], mem_outb[238], mem_outb[237], mem_outb[236], mem_outb[235], mem_outb[234]}));

	mux_2level_size20_mem mem_fle_3_in_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_13_ccff_tail),
		.ccff_tail(mux_2level_size20_mem_14_ccff_tail),
		.mem_out({mem_out[253], mem_out[252], mem_out[251], mem_out[250], mem_out[249], mem_out[248], mem_out[247], mem_out[246], mem_out[245], mem_out[244]}),
		.mem_outb({mem_outb[253], mem_outb[252], mem_outb[251], mem_outb[250], mem_outb[249], mem_outb[248], mem_outb[247], mem_outb[246], mem_outb[245], mem_outb[244]}));

	mux_2level_size20_mem mem_fle_3_in_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_size20_mem_14_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[263], mem_out[262], mem_out[261], mem_out[260], mem_out[259], mem_out[258], mem_out[257], mem_out[256], mem_out[255], mem_out[254]}),
		.mem_outb({mem_outb[263], mem_outb[262], mem_outb[261], mem_outb[260], mem_outb[259], mem_outb[258], mem_outb[257], mem_outb[256], mem_outb[255], mem_outb[254]}));

endmodule
// ----- END Verilog module for grid_clb_tile_config_group_mem_size264 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_io_left_tile_config_group_mem_size8 -----
module grid_io_left_tile_config_group_mem_size8(pReset,
                                                prog_clk,
                                                ccff_head,
                                                mem_out,
                                                mem_outb,
                                                ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [7:0] mem_out;
//----- OUTPUT PORTS -----
output [7:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] GPIO_DFFR_mem_0_ccff_tail;
wire [0:0] GPIO_DFFR_mem_1_ccff_tail;
wire [0:0] GPIO_DFFR_mem_2_ccff_tail;
wire [0:0] GPIO_DFFR_mem_3_ccff_tail;
wire [0:0] GPIO_DFFR_mem_4_ccff_tail;
wire [0:0] GPIO_DFFR_mem_5_ccff_tail;
wire [0:0] GPIO_DFFR_mem_6_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	GPIO_DFFR_mem GPIO_DFFR_mem (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(GPIO_DFFR_mem_0_ccff_tail),
		.mem_out(mem_out[0]),
		.mem_outb(mem_outb[0]));

	GPIO_DFFR_mem GPIO_DFFR_mem_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_0_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_1_ccff_tail),
		.mem_out(mem_out[1]),
		.mem_outb(mem_outb[1]));

	GPIO_DFFR_mem GPIO_DFFR_mem_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_1_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_2_ccff_tail),
		.mem_out(mem_out[2]),
		.mem_outb(mem_outb[2]));

	GPIO_DFFR_mem GPIO_DFFR_mem_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_2_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_3_ccff_tail),
		.mem_out(mem_out[3]),
		.mem_outb(mem_outb[3]));

	GPIO_DFFR_mem GPIO_DFFR_mem_4_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_3_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_4_ccff_tail),
		.mem_out(mem_out[4]),
		.mem_outb(mem_outb[4]));

	GPIO_DFFR_mem GPIO_DFFR_mem_5_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_4_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_5_ccff_tail),
		.mem_out(mem_out[5]),
		.mem_outb(mem_outb[5]));

	GPIO_DFFR_mem GPIO_DFFR_mem_6_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_5_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_6_ccff_tail),
		.mem_out(mem_out[6]),
		.mem_outb(mem_outb[6]));

	GPIO_DFFR_mem GPIO_DFFR_mem_7_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_6_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out(mem_out[7]),
		.mem_outb(mem_outb[7]));

endmodule
// ----- END Verilog module for grid_io_left_tile_config_group_mem_size8 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_io_right_tile_config_group_mem_size8 -----
module grid_io_right_tile_config_group_mem_size8(pReset,
                                                 prog_clk,
                                                 ccff_head,
                                                 mem_out,
                                                 mem_outb,
                                                 ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [7:0] mem_out;
//----- OUTPUT PORTS -----
output [7:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] GPIO_DFFR_mem_0_ccff_tail;
wire [0:0] GPIO_DFFR_mem_1_ccff_tail;
wire [0:0] GPIO_DFFR_mem_2_ccff_tail;
wire [0:0] GPIO_DFFR_mem_3_ccff_tail;
wire [0:0] GPIO_DFFR_mem_4_ccff_tail;
wire [0:0] GPIO_DFFR_mem_5_ccff_tail;
wire [0:0] GPIO_DFFR_mem_6_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	GPIO_DFFR_mem GPIO_DFFR_mem (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(GPIO_DFFR_mem_0_ccff_tail),
		.mem_out(mem_out[0]),
		.mem_outb(mem_outb[0]));

	GPIO_DFFR_mem GPIO_DFFR_mem_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_0_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_1_ccff_tail),
		.mem_out(mem_out[1]),
		.mem_outb(mem_outb[1]));

	GPIO_DFFR_mem GPIO_DFFR_mem_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_1_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_2_ccff_tail),
		.mem_out(mem_out[2]),
		.mem_outb(mem_outb[2]));

	GPIO_DFFR_mem GPIO_DFFR_mem_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_2_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_3_ccff_tail),
		.mem_out(mem_out[3]),
		.mem_outb(mem_outb[3]));

	GPIO_DFFR_mem GPIO_DFFR_mem_4_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_3_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_4_ccff_tail),
		.mem_out(mem_out[4]),
		.mem_outb(mem_outb[4]));

	GPIO_DFFR_mem GPIO_DFFR_mem_5_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_4_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_5_ccff_tail),
		.mem_out(mem_out[5]),
		.mem_outb(mem_outb[5]));

	GPIO_DFFR_mem GPIO_DFFR_mem_6_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_5_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_6_ccff_tail),
		.mem_out(mem_out[6]),
		.mem_outb(mem_outb[6]));

	GPIO_DFFR_mem GPIO_DFFR_mem_7_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_6_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out(mem_out[7]),
		.mem_outb(mem_outb[7]));

endmodule
// ----- END Verilog module for grid_io_right_tile_config_group_mem_size8 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_io_top_tile_config_group_mem_size8 -----
module grid_io_top_tile_config_group_mem_size8(pReset,
                                               prog_clk,
                                               ccff_head,
                                               mem_out,
                                               mem_outb,
                                               ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [7:0] mem_out;
//----- OUTPUT PORTS -----
output [7:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] GPIO_DFFR_mem_0_ccff_tail;
wire [0:0] GPIO_DFFR_mem_1_ccff_tail;
wire [0:0] GPIO_DFFR_mem_2_ccff_tail;
wire [0:0] GPIO_DFFR_mem_3_ccff_tail;
wire [0:0] GPIO_DFFR_mem_4_ccff_tail;
wire [0:0] GPIO_DFFR_mem_5_ccff_tail;
wire [0:0] GPIO_DFFR_mem_6_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	GPIO_DFFR_mem GPIO_DFFR_mem (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(GPIO_DFFR_mem_0_ccff_tail),
		.mem_out(mem_out[0]),
		.mem_outb(mem_outb[0]));

	GPIO_DFFR_mem GPIO_DFFR_mem_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_0_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_1_ccff_tail),
		.mem_out(mem_out[1]),
		.mem_outb(mem_outb[1]));

	GPIO_DFFR_mem GPIO_DFFR_mem_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_1_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_2_ccff_tail),
		.mem_out(mem_out[2]),
		.mem_outb(mem_outb[2]));

	GPIO_DFFR_mem GPIO_DFFR_mem_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_2_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_3_ccff_tail),
		.mem_out(mem_out[3]),
		.mem_outb(mem_outb[3]));

	GPIO_DFFR_mem GPIO_DFFR_mem_4_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_3_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_4_ccff_tail),
		.mem_out(mem_out[4]),
		.mem_outb(mem_outb[4]));

	GPIO_DFFR_mem GPIO_DFFR_mem_5_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_4_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_5_ccff_tail),
		.mem_out(mem_out[5]),
		.mem_outb(mem_outb[5]));

	GPIO_DFFR_mem GPIO_DFFR_mem_6_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_5_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_6_ccff_tail),
		.mem_out(mem_out[6]),
		.mem_outb(mem_outb[6]));

	GPIO_DFFR_mem GPIO_DFFR_mem_7_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_6_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out(mem_out[7]),
		.mem_outb(mem_outb[7]));

endmodule
// ----- END Verilog module for grid_io_top_tile_config_group_mem_size8 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for grid_io_bottom_tile_config_group_mem_size8 -----
module grid_io_bottom_tile_config_group_mem_size8(pReset,
                                                  prog_clk,
                                                  ccff_head,
                                                  mem_out,
                                                  mem_outb,
                                                  ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [7:0] mem_out;
//----- OUTPUT PORTS -----
output [7:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] GPIO_DFFR_mem_0_ccff_tail;
wire [0:0] GPIO_DFFR_mem_1_ccff_tail;
wire [0:0] GPIO_DFFR_mem_2_ccff_tail;
wire [0:0] GPIO_DFFR_mem_3_ccff_tail;
wire [0:0] GPIO_DFFR_mem_4_ccff_tail;
wire [0:0] GPIO_DFFR_mem_5_ccff_tail;
wire [0:0] GPIO_DFFR_mem_6_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	GPIO_DFFR_mem GPIO_DFFR_mem (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(GPIO_DFFR_mem_0_ccff_tail),
		.mem_out(mem_out[0]),
		.mem_outb(mem_outb[0]));

	GPIO_DFFR_mem GPIO_DFFR_mem_1_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_0_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_1_ccff_tail),
		.mem_out(mem_out[1]),
		.mem_outb(mem_outb[1]));

	GPIO_DFFR_mem GPIO_DFFR_mem_2_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_1_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_2_ccff_tail),
		.mem_out(mem_out[2]),
		.mem_outb(mem_outb[2]));

	GPIO_DFFR_mem GPIO_DFFR_mem_3_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_2_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_3_ccff_tail),
		.mem_out(mem_out[3]),
		.mem_outb(mem_outb[3]));

	GPIO_DFFR_mem GPIO_DFFR_mem_4_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_3_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_4_ccff_tail),
		.mem_out(mem_out[4]),
		.mem_outb(mem_outb[4]));

	GPIO_DFFR_mem GPIO_DFFR_mem_5_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_4_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_5_ccff_tail),
		.mem_out(mem_out[5]),
		.mem_outb(mem_outb[5]));

	GPIO_DFFR_mem GPIO_DFFR_mem_6_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_5_ccff_tail),
		.ccff_tail(GPIO_DFFR_mem_6_ccff_tail),
		.mem_out(mem_out[6]),
		.mem_outb(mem_outb[6]));

	GPIO_DFFR_mem GPIO_DFFR_mem_7_ (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(GPIO_DFFR_mem_6_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out(mem_out[7]),
		.mem_outb(mem_outb[7]));

endmodule
// ----- END Verilog module for grid_io_bottom_tile_config_group_mem_size8 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_5__config_group_mem_size56 -----
module sb_5__config_group_mem_size56(pReset,
                                     prog_clk,
                                     ccff_head,
                                     mem_out,
                                     mem_outb,
                                     ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [55:0] mem_out;
//----- OUTPUT PORTS -----
output [55:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size40_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_2_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size40_mem mem_bottom_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.mem_out({mem_out[13], mem_out[12], mem_out[11], mem_out[10], mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10], mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.mem_out({mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20], mem_out[19], mem_out[18], mem_out[17], mem_out[16], mem_out[15], mem_out[14]}),
		.mem_outb({mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20], mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16], mem_outb[15], mem_outb[14]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.mem_out({mem_out[41], mem_out[40], mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30], mem_out[29], mem_out[28]}),
		.mem_outb({mem_outb[41], mem_outb[40], mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30], mem_outb[29], mem_outb[28]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50], mem_out[49], mem_out[48], mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42]}),
		.mem_outb({mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50], mem_outb[49], mem_outb[48], mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42]}));

endmodule
// ----- END Verilog module for sb_5__config_group_mem_size56 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_6__config_group_mem_size56 -----
module sb_6__config_group_mem_size56(pReset,
                                     prog_clk,
                                     ccff_head,
                                     mem_out,
                                     mem_outb,
                                     ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [55:0] mem_out;
//----- OUTPUT PORTS -----
output [55:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size40_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_2_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size40_mem mem_bottom_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.mem_out({mem_out[13], mem_out[12], mem_out[11], mem_out[10], mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10], mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.mem_out({mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20], mem_out[19], mem_out[18], mem_out[17], mem_out[16], mem_out[15], mem_out[14]}),
		.mem_outb({mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20], mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16], mem_outb[15], mem_outb[14]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.mem_out({mem_out[41], mem_out[40], mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30], mem_out[29], mem_out[28]}),
		.mem_outb({mem_outb[41], mem_outb[40], mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30], mem_outb[29], mem_outb[28]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50], mem_out[49], mem_out[48], mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42]}),
		.mem_outb({mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50], mem_outb[49], mem_outb[48], mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42]}));

endmodule
// ----- END Verilog module for sb_6__config_group_mem_size56 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_8__config_group_mem_size56 -----
module sb_8__config_group_mem_size56(pReset,
                                     prog_clk,
                                     ccff_head,
                                     mem_out,
                                     mem_outb,
                                     ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [55:0] mem_out;
//----- OUTPUT PORTS -----
output [55:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size40_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_2_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size40_mem mem_left_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.mem_out({mem_out[13], mem_out[12], mem_out[11], mem_out[10], mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10], mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.mem_out({mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20], mem_out[19], mem_out[18], mem_out[17], mem_out[16], mem_out[15], mem_out[14]}),
		.mem_outb({mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20], mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16], mem_outb[15], mem_outb[14]}));

	mux_2level_tapbuf_size40_mem mem_left_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.mem_out({mem_out[41], mem_out[40], mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30], mem_out[29], mem_out[28]}),
		.mem_outb({mem_outb[41], mem_outb[40], mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30], mem_outb[29], mem_outb[28]}));

	mux_2level_tapbuf_size40_mem mem_left_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50], mem_out[49], mem_out[48], mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42]}),
		.mem_outb({mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50], mem_outb[49], mem_outb[48], mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42]}));

endmodule
// ----- END Verilog module for sb_8__config_group_mem_size56 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_9__config_group_mem_size120 -----
module sb_9__config_group_mem_size120(pReset,
                                      prog_clk,
                                      ccff_head,
                                      mem_out,
                                      mem_outb,
                                      ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [119:0] mem_out;
//----- OUTPUT PORTS -----
output [119:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size48_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size48_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size48_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size56_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size56_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size56_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size56_mem_3_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size56_mem mem_bottom_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size56_mem_0_ccff_tail),
		.mem_out({mem_out[15], mem_out[14], mem_out[13], mem_out[12], mem_out[11], mem_out[10], mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[15], mem_outb[14], mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10], mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size56_mem mem_bottom_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size56_mem_1_ccff_tail),
		.mem_out({mem_out[31], mem_out[30], mem_out[29], mem_out[28], mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20], mem_out[19], mem_out[18], mem_out[17], mem_out[16]}),
		.mem_outb({mem_outb[31], mem_outb[30], mem_outb[29], mem_outb[28], mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20], mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16]}));

	mux_2level_tapbuf_size56_mem mem_bottom_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size56_mem_2_ccff_tail),
		.mem_out({mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42], mem_out[41], mem_out[40], mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32]}),
		.mem_outb({mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42], mem_outb[41], mem_outb[40], mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32]}));

	mux_2level_tapbuf_size56_mem mem_bottom_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size56_mem_3_ccff_tail),
		.mem_out({mem_out[63], mem_out[62], mem_out[61], mem_out[60], mem_out[59], mem_out[58], mem_out[57], mem_out[56], mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50], mem_out[49], mem_out[48]}),
		.mem_outb({mem_outb[63], mem_outb[62], mem_outb[61], mem_outb[60], mem_outb[59], mem_outb[58], mem_outb[57], mem_outb[56], mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50], mem_outb[49], mem_outb[48]}));

	mux_2level_tapbuf_size48_mem mem_left_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_0_ccff_tail),
		.mem_out({mem_out[77], mem_out[76], mem_out[75], mem_out[74], mem_out[73], mem_out[72], mem_out[71], mem_out[70], mem_out[69], mem_out[68], mem_out[67], mem_out[66], mem_out[65], mem_out[64]}),
		.mem_outb({mem_outb[77], mem_outb[76], mem_outb[75], mem_outb[74], mem_outb[73], mem_outb[72], mem_outb[71], mem_outb[70], mem_outb[69], mem_outb[68], mem_outb[67], mem_outb[66], mem_outb[65], mem_outb[64]}));

	mux_2level_tapbuf_size48_mem mem_left_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_1_ccff_tail),
		.mem_out({mem_out[91], mem_out[90], mem_out[89], mem_out[88], mem_out[87], mem_out[86], mem_out[85], mem_out[84], mem_out[83], mem_out[82], mem_out[81], mem_out[80], mem_out[79], mem_out[78]}),
		.mem_outb({mem_outb[91], mem_outb[90], mem_outb[89], mem_outb[88], mem_outb[87], mem_outb[86], mem_outb[85], mem_outb[84], mem_outb[83], mem_outb[82], mem_outb[81], mem_outb[80], mem_outb[79], mem_outb[78]}));

	mux_2level_tapbuf_size48_mem mem_left_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_2_ccff_tail),
		.mem_out({mem_out[105], mem_out[104], mem_out[103], mem_out[102], mem_out[101], mem_out[100], mem_out[99], mem_out[98], mem_out[97], mem_out[96], mem_out[95], mem_out[94], mem_out[93], mem_out[92]}),
		.mem_outb({mem_outb[105], mem_outb[104], mem_outb[103], mem_outb[102], mem_outb[101], mem_outb[100], mem_outb[99], mem_outb[98], mem_outb[97], mem_outb[96], mem_outb[95], mem_outb[94], mem_outb[93], mem_outb[92]}));

	mux_2level_tapbuf_size48_mem mem_left_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_2_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[119], mem_out[118], mem_out[117], mem_out[116], mem_out[115], mem_out[114], mem_out[113], mem_out[112], mem_out[111], mem_out[110], mem_out[109], mem_out[108], mem_out[107], mem_out[106]}),
		.mem_outb({mem_outb[119], mem_outb[118], mem_outb[117], mem_outb[116], mem_outb[115], mem_outb[114], mem_outb[113], mem_outb[112], mem_outb[111], mem_outb[110], mem_outb[109], mem_outb[108], mem_outb[107], mem_outb[106]}));

endmodule
// ----- END Verilog module for sb_9__config_group_mem_size120 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_10__config_group_mem_size272 -----
module sb_10__config_group_mem_size272(pReset,
                                       prog_clk,
                                       ccff_head,
                                       mem_out,
                                       mem_outb,
                                       ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [271:0] mem_out;
//----- OUTPUT PORTS -----
output [271:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size32_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_10_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_11_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_12_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_13_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_14_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_15_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_3_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_4_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_5_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_6_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_7_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_8_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_9_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size40_mem mem_bottom_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.mem_out({mem_out[13], mem_out[12], mem_out[11], mem_out[10], mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10], mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.mem_out({mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20], mem_out[19], mem_out[18], mem_out[17], mem_out[16], mem_out[15], mem_out[14]}),
		.mem_outb({mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20], mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16], mem_outb[15], mem_outb[14]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_5 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.mem_out({mem_out[41], mem_out[40], mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30], mem_out[29], mem_out[28]}),
		.mem_outb({mem_outb[41], mem_outb[40], mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30], mem_outb[29], mem_outb[28]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_7 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_3_ccff_tail),
		.mem_out({mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50], mem_out[49], mem_out[48], mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42]}),
		.mem_outb({mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50], mem_outb[49], mem_outb[48], mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_4_ccff_tail),
		.mem_out({mem_out[69], mem_out[68], mem_out[67], mem_out[66], mem_out[65], mem_out[64], mem_out[63], mem_out[62], mem_out[61], mem_out[60], mem_out[59], mem_out[58], mem_out[57], mem_out[56]}),
		.mem_outb({mem_outb[69], mem_outb[68], mem_outb[67], mem_outb[66], mem_outb[65], mem_outb[64], mem_outb[63], mem_outb[62], mem_outb[61], mem_outb[60], mem_outb[59], mem_outb[58], mem_outb[57], mem_outb[56]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_11 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_5_ccff_tail),
		.mem_out({mem_out[83], mem_out[82], mem_out[81], mem_out[80], mem_out[79], mem_out[78], mem_out[77], mem_out[76], mem_out[75], mem_out[74], mem_out[73], mem_out[72], mem_out[71], mem_out[70]}),
		.mem_outb({mem_outb[83], mem_outb[82], mem_outb[81], mem_outb[80], mem_outb[79], mem_outb[78], mem_outb[77], mem_outb[76], mem_outb[75], mem_outb[74], mem_outb[73], mem_outb[72], mem_outb[71], mem_outb[70]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_13 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_6_ccff_tail),
		.mem_out({mem_out[97], mem_out[96], mem_out[95], mem_out[94], mem_out[93], mem_out[92], mem_out[91], mem_out[90], mem_out[89], mem_out[88], mem_out[87], mem_out[86], mem_out[85], mem_out[84]}),
		.mem_outb({mem_outb[97], mem_outb[96], mem_outb[95], mem_outb[94], mem_outb[93], mem_outb[92], mem_outb[91], mem_outb[90], mem_outb[89], mem_outb[88], mem_outb[87], mem_outb[86], mem_outb[85], mem_outb[84]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_15 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_6_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_7_ccff_tail),
		.mem_out({mem_out[111], mem_out[110], mem_out[109], mem_out[108], mem_out[107], mem_out[106], mem_out[105], mem_out[104], mem_out[103], mem_out[102], mem_out[101], mem_out[100], mem_out[99], mem_out[98]}),
		.mem_outb({mem_outb[111], mem_outb[110], mem_outb[109], mem_outb[108], mem_outb[107], mem_outb[106], mem_outb[105], mem_outb[104], mem_outb[103], mem_outb[102], mem_outb[101], mem_outb[100], mem_outb[99], mem_outb[98]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_7_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_8_ccff_tail),
		.mem_out({mem_out[125], mem_out[124], mem_out[123], mem_out[122], mem_out[121], mem_out[120], mem_out[119], mem_out[118], mem_out[117], mem_out[116], mem_out[115], mem_out[114], mem_out[113], mem_out[112]}),
		.mem_outb({mem_outb[125], mem_outb[124], mem_outb[123], mem_outb[122], mem_outb[121], mem_outb[120], mem_outb[119], mem_outb[118], mem_outb[117], mem_outb[116], mem_outb[115], mem_outb[114], mem_outb[113], mem_outb[112]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_19 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_8_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_9_ccff_tail),
		.mem_out({mem_out[139], mem_out[138], mem_out[137], mem_out[136], mem_out[135], mem_out[134], mem_out[133], mem_out[132], mem_out[131], mem_out[130], mem_out[129], mem_out[128], mem_out[127], mem_out[126]}),
		.mem_outb({mem_outb[139], mem_outb[138], mem_outb[137], mem_outb[136], mem_outb[135], mem_outb[134], mem_outb[133], mem_outb[132], mem_outb[131], mem_outb[130], mem_outb[129], mem_outb[128], mem_outb[127], mem_outb[126]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_21 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_9_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_10_ccff_tail),
		.mem_out({mem_out[153], mem_out[152], mem_out[151], mem_out[150], mem_out[149], mem_out[148], mem_out[147], mem_out[146], mem_out[145], mem_out[144], mem_out[143], mem_out[142], mem_out[141], mem_out[140]}),
		.mem_outb({mem_outb[153], mem_outb[152], mem_outb[151], mem_outb[150], mem_outb[149], mem_outb[148], mem_outb[147], mem_outb[146], mem_outb[145], mem_outb[144], mem_outb[143], mem_outb[142], mem_outb[141], mem_outb[140]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_23 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_10_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_11_ccff_tail),
		.mem_out({mem_out[167], mem_out[166], mem_out[165], mem_out[164], mem_out[163], mem_out[162], mem_out[161], mem_out[160], mem_out[159], mem_out[158], mem_out[157], mem_out[156], mem_out[155], mem_out[154]}),
		.mem_outb({mem_outb[167], mem_outb[166], mem_outb[165], mem_outb[164], mem_outb[163], mem_outb[162], mem_outb[161], mem_outb[160], mem_outb[159], mem_outb[158], mem_outb[157], mem_outb[156], mem_outb[155], mem_outb[154]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_11_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_12_ccff_tail),
		.mem_out({mem_out[181], mem_out[180], mem_out[179], mem_out[178], mem_out[177], mem_out[176], mem_out[175], mem_out[174], mem_out[173], mem_out[172], mem_out[171], mem_out[170], mem_out[169], mem_out[168]}),
		.mem_outb({mem_outb[181], mem_outb[180], mem_outb[179], mem_outb[178], mem_outb[177], mem_outb[176], mem_outb[175], mem_outb[174], mem_outb[173], mem_outb[172], mem_outb[171], mem_outb[170], mem_outb[169], mem_outb[168]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_27 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_12_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_13_ccff_tail),
		.mem_out({mem_out[195], mem_out[194], mem_out[193], mem_out[192], mem_out[191], mem_out[190], mem_out[189], mem_out[188], mem_out[187], mem_out[186], mem_out[185], mem_out[184], mem_out[183], mem_out[182]}),
		.mem_outb({mem_outb[195], mem_outb[194], mem_outb[193], mem_outb[192], mem_outb[191], mem_outb[190], mem_outb[189], mem_outb[188], mem_outb[187], mem_outb[186], mem_outb[185], mem_outb[184], mem_outb[183], mem_outb[182]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_29 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_13_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_14_ccff_tail),
		.mem_out({mem_out[209], mem_out[208], mem_out[207], mem_out[206], mem_out[205], mem_out[204], mem_out[203], mem_out[202], mem_out[201], mem_out[200], mem_out[199], mem_out[198], mem_out[197], mem_out[196]}),
		.mem_outb({mem_outb[209], mem_outb[208], mem_outb[207], mem_outb[206], mem_outb[205], mem_outb[204], mem_outb[203], mem_outb[202], mem_outb[201], mem_outb[200], mem_outb[199], mem_outb[198], mem_outb[197], mem_outb[196]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_31 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_14_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_15_ccff_tail),
		.mem_out({mem_out[223], mem_out[222], mem_out[221], mem_out[220], mem_out[219], mem_out[218], mem_out[217], mem_out[216], mem_out[215], mem_out[214], mem_out[213], mem_out[212], mem_out[211], mem_out[210]}),
		.mem_outb({mem_outb[223], mem_outb[222], mem_outb[221], mem_outb[220], mem_outb[219], mem_outb[218], mem_outb[217], mem_outb[216], mem_outb[215], mem_outb[214], mem_outb[213], mem_outb[212], mem_outb[211], mem_outb[210]}));

	mux_2level_tapbuf_size32_mem mem_left_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_15_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_0_ccff_tail),
		.mem_out({mem_out[235], mem_out[234], mem_out[233], mem_out[232], mem_out[231], mem_out[230], mem_out[229], mem_out[228], mem_out[227], mem_out[226], mem_out[225], mem_out[224]}),
		.mem_outb({mem_outb[235], mem_outb[234], mem_outb[233], mem_outb[232], mem_outb[231], mem_outb[230], mem_outb[229], mem_outb[228], mem_outb[227], mem_outb[226], mem_outb[225], mem_outb[224]}));

	mux_2level_tapbuf_size32_mem mem_left_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_1_ccff_tail),
		.mem_out({mem_out[247], mem_out[246], mem_out[245], mem_out[244], mem_out[243], mem_out[242], mem_out[241], mem_out[240], mem_out[239], mem_out[238], mem_out[237], mem_out[236]}),
		.mem_outb({mem_outb[247], mem_outb[246], mem_outb[245], mem_outb[244], mem_outb[243], mem_outb[242], mem_outb[241], mem_outb[240], mem_outb[239], mem_outb[238], mem_outb[237], mem_outb[236]}));

	mux_2level_tapbuf_size32_mem mem_left_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_2_ccff_tail),
		.mem_out({mem_out[259], mem_out[258], mem_out[257], mem_out[256], mem_out[255], mem_out[254], mem_out[253], mem_out[252], mem_out[251], mem_out[250], mem_out[249], mem_out[248]}),
		.mem_outb({mem_outb[259], mem_outb[258], mem_outb[257], mem_outb[256], mem_outb[255], mem_outb[254], mem_outb[253], mem_outb[252], mem_outb[251], mem_outb[250], mem_outb[249], mem_outb[248]}));

	mux_2level_tapbuf_size32_mem mem_left_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_2_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[271], mem_out[270], mem_out[269], mem_out[268], mem_out[267], mem_out[266], mem_out[265], mem_out[264], mem_out[263], mem_out[262], mem_out[261], mem_out[260]}),
		.mem_outb({mem_outb[271], mem_outb[270], mem_outb[269], mem_outb[268], mem_outb[267], mem_outb[266], mem_outb[265], mem_outb[264], mem_outb[263], mem_outb[262], mem_outb[261], mem_outb[260]}));

endmodule
// ----- END Verilog module for sb_10__config_group_mem_size272 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_11__config_group_mem_size56 -----
module sb_11__config_group_mem_size56(pReset,
                                      prog_clk,
                                      ccff_head,
                                      mem_out,
                                      mem_outb,
                                      ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [55:0] mem_out;
//----- OUTPUT PORTS -----
output [55:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size40_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_2_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size40_mem mem_left_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.mem_out({mem_out[13], mem_out[12], mem_out[11], mem_out[10], mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10], mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size40_mem mem_left_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.mem_out({mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20], mem_out[19], mem_out[18], mem_out[17], mem_out[16], mem_out[15], mem_out[14]}),
		.mem_outb({mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20], mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16], mem_outb[15], mem_outb[14]}));

	mux_2level_tapbuf_size40_mem mem_left_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.mem_out({mem_out[41], mem_out[40], mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30], mem_out[29], mem_out[28]}),
		.mem_outb({mem_outb[41], mem_outb[40], mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30], mem_outb[29], mem_outb[28]}));

	mux_2level_tapbuf_size40_mem mem_left_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50], mem_out[49], mem_out[48], mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42]}),
		.mem_outb({mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50], mem_outb[49], mem_outb[48], mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42]}));

endmodule
// ----- END Verilog module for sb_11__config_group_mem_size56 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_12__config_group_mem_size120 -----
module sb_12__config_group_mem_size120(pReset,
                                       prog_clk,
                                       ccff_head,
                                       mem_out,
                                       mem_outb,
                                       ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [119:0] mem_out;
//----- OUTPUT PORTS -----
output [119:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size48_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size48_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size48_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size56_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size56_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size56_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size56_mem_3_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size56_mem mem_bottom_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size56_mem_0_ccff_tail),
		.mem_out({mem_out[15], mem_out[14], mem_out[13], mem_out[12], mem_out[11], mem_out[10], mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[15], mem_outb[14], mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10], mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size56_mem mem_bottom_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size56_mem_1_ccff_tail),
		.mem_out({mem_out[31], mem_out[30], mem_out[29], mem_out[28], mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20], mem_out[19], mem_out[18], mem_out[17], mem_out[16]}),
		.mem_outb({mem_outb[31], mem_outb[30], mem_outb[29], mem_outb[28], mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20], mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16]}));

	mux_2level_tapbuf_size56_mem mem_bottom_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size56_mem_2_ccff_tail),
		.mem_out({mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42], mem_out[41], mem_out[40], mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32]}),
		.mem_outb({mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42], mem_outb[41], mem_outb[40], mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32]}));

	mux_2level_tapbuf_size56_mem mem_bottom_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size56_mem_3_ccff_tail),
		.mem_out({mem_out[63], mem_out[62], mem_out[61], mem_out[60], mem_out[59], mem_out[58], mem_out[57], mem_out[56], mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50], mem_out[49], mem_out[48]}),
		.mem_outb({mem_outb[63], mem_outb[62], mem_outb[61], mem_outb[60], mem_outb[59], mem_outb[58], mem_outb[57], mem_outb[56], mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50], mem_outb[49], mem_outb[48]}));

	mux_2level_tapbuf_size48_mem mem_left_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size56_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_0_ccff_tail),
		.mem_out({mem_out[77], mem_out[76], mem_out[75], mem_out[74], mem_out[73], mem_out[72], mem_out[71], mem_out[70], mem_out[69], mem_out[68], mem_out[67], mem_out[66], mem_out[65], mem_out[64]}),
		.mem_outb({mem_outb[77], mem_outb[76], mem_outb[75], mem_outb[74], mem_outb[73], mem_outb[72], mem_outb[71], mem_outb[70], mem_outb[69], mem_outb[68], mem_outb[67], mem_outb[66], mem_outb[65], mem_outb[64]}));

	mux_2level_tapbuf_size48_mem mem_left_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_1_ccff_tail),
		.mem_out({mem_out[91], mem_out[90], mem_out[89], mem_out[88], mem_out[87], mem_out[86], mem_out[85], mem_out[84], mem_out[83], mem_out[82], mem_out[81], mem_out[80], mem_out[79], mem_out[78]}),
		.mem_outb({mem_outb[91], mem_outb[90], mem_outb[89], mem_outb[88], mem_outb[87], mem_outb[86], mem_outb[85], mem_outb[84], mem_outb[83], mem_outb[82], mem_outb[81], mem_outb[80], mem_outb[79], mem_outb[78]}));

	mux_2level_tapbuf_size48_mem mem_left_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size48_mem_2_ccff_tail),
		.mem_out({mem_out[105], mem_out[104], mem_out[103], mem_out[102], mem_out[101], mem_out[100], mem_out[99], mem_out[98], mem_out[97], mem_out[96], mem_out[95], mem_out[94], mem_out[93], mem_out[92]}),
		.mem_outb({mem_outb[105], mem_outb[104], mem_outb[103], mem_outb[102], mem_outb[101], mem_outb[100], mem_outb[99], mem_outb[98], mem_outb[97], mem_outb[96], mem_outb[95], mem_outb[94], mem_outb[93], mem_outb[92]}));

	mux_2level_tapbuf_size48_mem mem_left_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size48_mem_2_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[119], mem_out[118], mem_out[117], mem_out[116], mem_out[115], mem_out[114], mem_out[113], mem_out[112], mem_out[111], mem_out[110], mem_out[109], mem_out[108], mem_out[107], mem_out[106]}),
		.mem_outb({mem_outb[119], mem_outb[118], mem_outb[117], mem_outb[116], mem_outb[115], mem_outb[114], mem_outb[113], mem_outb[112], mem_outb[111], mem_outb[110], mem_outb[109], mem_outb[108], mem_outb[107], mem_outb[106]}));

endmodule
// ----- END Verilog module for sb_12__config_group_mem_size120 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_15__config_group_mem_size248 -----
module sb_15__config_group_mem_size248(pReset,
                                       prog_clk,
                                       ccff_head,
                                       mem_out,
                                       mem_outb,
                                       ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [247:0] mem_out;
//----- OUTPUT PORTS -----
output [247:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size32_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_10_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_11_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_12_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_13_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_14_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_3_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_4_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_5_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_6_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_7_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_8_ccff_tail;
wire [0:0] mux_2level_tapbuf_size32_mem_9_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size40_mem_3_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size40_mem mem_bottom_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.mem_out({mem_out[13], mem_out[12], mem_out[11], mem_out[10], mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10], mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.mem_out({mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20], mem_out[19], mem_out[18], mem_out[17], mem_out[16], mem_out[15], mem_out[14]}),
		.mem_outb({mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20], mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16], mem_outb[15], mem_outb[14]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.mem_out({mem_out[41], mem_out[40], mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30], mem_out[29], mem_out[28]}),
		.mem_outb({mem_outb[41], mem_outb[40], mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30], mem_outb[29], mem_outb[28]}));

	mux_2level_tapbuf_size40_mem mem_bottom_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size40_mem_3_ccff_tail),
		.mem_out({mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50], mem_out[49], mem_out[48], mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42]}),
		.mem_outb({mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50], mem_outb[49], mem_outb[48], mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42]}));

	mux_2level_tapbuf_size32_mem mem_left_track_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size40_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_0_ccff_tail),
		.mem_out({mem_out[67], mem_out[66], mem_out[65], mem_out[64], mem_out[63], mem_out[62], mem_out[61], mem_out[60], mem_out[59], mem_out[58], mem_out[57], mem_out[56]}),
		.mem_outb({mem_outb[67], mem_outb[66], mem_outb[65], mem_outb[64], mem_outb[63], mem_outb[62], mem_outb[61], mem_outb[60], mem_outb[59], mem_outb[58], mem_outb[57], mem_outb[56]}));

	mux_2level_tapbuf_size32_mem mem_left_track_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_1_ccff_tail),
		.mem_out({mem_out[79], mem_out[78], mem_out[77], mem_out[76], mem_out[75], mem_out[74], mem_out[73], mem_out[72], mem_out[71], mem_out[70], mem_out[69], mem_out[68]}),
		.mem_outb({mem_outb[79], mem_outb[78], mem_outb[77], mem_outb[76], mem_outb[75], mem_outb[74], mem_outb[73], mem_outb[72], mem_outb[71], mem_outb[70], mem_outb[69], mem_outb[68]}));

	mux_2level_tapbuf_size32_mem mem_left_track_5 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_2_ccff_tail),
		.mem_out({mem_out[91], mem_out[90], mem_out[89], mem_out[88], mem_out[87], mem_out[86], mem_out[85], mem_out[84], mem_out[83], mem_out[82], mem_out[81], mem_out[80]}),
		.mem_outb({mem_outb[91], mem_outb[90], mem_outb[89], mem_outb[88], mem_outb[87], mem_outb[86], mem_outb[85], mem_outb[84], mem_outb[83], mem_outb[82], mem_outb[81], mem_outb[80]}));

	mux_2level_tapbuf_size32_mem mem_left_track_7 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_3_ccff_tail),
		.mem_out({mem_out[103], mem_out[102], mem_out[101], mem_out[100], mem_out[99], mem_out[98], mem_out[97], mem_out[96], mem_out[95], mem_out[94], mem_out[93], mem_out[92]}),
		.mem_outb({mem_outb[103], mem_outb[102], mem_outb[101], mem_outb[100], mem_outb[99], mem_outb[98], mem_outb[97], mem_outb[96], mem_outb[95], mem_outb[94], mem_outb[93], mem_outb[92]}));

	mux_2level_tapbuf_size32_mem mem_left_track_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_4_ccff_tail),
		.mem_out({mem_out[115], mem_out[114], mem_out[113], mem_out[112], mem_out[111], mem_out[110], mem_out[109], mem_out[108], mem_out[107], mem_out[106], mem_out[105], mem_out[104]}),
		.mem_outb({mem_outb[115], mem_outb[114], mem_outb[113], mem_outb[112], mem_outb[111], mem_outb[110], mem_outb[109], mem_outb[108], mem_outb[107], mem_outb[106], mem_outb[105], mem_outb[104]}));

	mux_2level_tapbuf_size32_mem mem_left_track_11 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_5_ccff_tail),
		.mem_out({mem_out[127], mem_out[126], mem_out[125], mem_out[124], mem_out[123], mem_out[122], mem_out[121], mem_out[120], mem_out[119], mem_out[118], mem_out[117], mem_out[116]}),
		.mem_outb({mem_outb[127], mem_outb[126], mem_outb[125], mem_outb[124], mem_outb[123], mem_outb[122], mem_outb[121], mem_outb[120], mem_outb[119], mem_outb[118], mem_outb[117], mem_outb[116]}));

	mux_2level_tapbuf_size32_mem mem_left_track_13 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_6_ccff_tail),
		.mem_out({mem_out[139], mem_out[138], mem_out[137], mem_out[136], mem_out[135], mem_out[134], mem_out[133], mem_out[132], mem_out[131], mem_out[130], mem_out[129], mem_out[128]}),
		.mem_outb({mem_outb[139], mem_outb[138], mem_outb[137], mem_outb[136], mem_outb[135], mem_outb[134], mem_outb[133], mem_outb[132], mem_outb[131], mem_outb[130], mem_outb[129], mem_outb[128]}));

	mux_2level_tapbuf_size32_mem mem_left_track_15 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_6_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_7_ccff_tail),
		.mem_out({mem_out[151], mem_out[150], mem_out[149], mem_out[148], mem_out[147], mem_out[146], mem_out[145], mem_out[144], mem_out[143], mem_out[142], mem_out[141], mem_out[140]}),
		.mem_outb({mem_outb[151], mem_outb[150], mem_outb[149], mem_outb[148], mem_outb[147], mem_outb[146], mem_outb[145], mem_outb[144], mem_outb[143], mem_outb[142], mem_outb[141], mem_outb[140]}));

	mux_2level_tapbuf_size32_mem mem_left_track_17 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_7_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_8_ccff_tail),
		.mem_out({mem_out[163], mem_out[162], mem_out[161], mem_out[160], mem_out[159], mem_out[158], mem_out[157], mem_out[156], mem_out[155], mem_out[154], mem_out[153], mem_out[152]}),
		.mem_outb({mem_outb[163], mem_outb[162], mem_outb[161], mem_outb[160], mem_outb[159], mem_outb[158], mem_outb[157], mem_outb[156], mem_outb[155], mem_outb[154], mem_outb[153], mem_outb[152]}));

	mux_2level_tapbuf_size32_mem mem_left_track_19 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_8_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_9_ccff_tail),
		.mem_out({mem_out[175], mem_out[174], mem_out[173], mem_out[172], mem_out[171], mem_out[170], mem_out[169], mem_out[168], mem_out[167], mem_out[166], mem_out[165], mem_out[164]}),
		.mem_outb({mem_outb[175], mem_outb[174], mem_outb[173], mem_outb[172], mem_outb[171], mem_outb[170], mem_outb[169], mem_outb[168], mem_outb[167], mem_outb[166], mem_outb[165], mem_outb[164]}));

	mux_2level_tapbuf_size32_mem mem_left_track_21 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_9_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_10_ccff_tail),
		.mem_out({mem_out[187], mem_out[186], mem_out[185], mem_out[184], mem_out[183], mem_out[182], mem_out[181], mem_out[180], mem_out[179], mem_out[178], mem_out[177], mem_out[176]}),
		.mem_outb({mem_outb[187], mem_outb[186], mem_outb[185], mem_outb[184], mem_outb[183], mem_outb[182], mem_outb[181], mem_outb[180], mem_outb[179], mem_outb[178], mem_outb[177], mem_outb[176]}));

	mux_2level_tapbuf_size32_mem mem_left_track_23 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_10_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_11_ccff_tail),
		.mem_out({mem_out[199], mem_out[198], mem_out[197], mem_out[196], mem_out[195], mem_out[194], mem_out[193], mem_out[192], mem_out[191], mem_out[190], mem_out[189], mem_out[188]}),
		.mem_outb({mem_outb[199], mem_outb[198], mem_outb[197], mem_outb[196], mem_outb[195], mem_outb[194], mem_outb[193], mem_outb[192], mem_outb[191], mem_outb[190], mem_outb[189], mem_outb[188]}));

	mux_2level_tapbuf_size32_mem mem_left_track_25 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_11_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_12_ccff_tail),
		.mem_out({mem_out[211], mem_out[210], mem_out[209], mem_out[208], mem_out[207], mem_out[206], mem_out[205], mem_out[204], mem_out[203], mem_out[202], mem_out[201], mem_out[200]}),
		.mem_outb({mem_outb[211], mem_outb[210], mem_outb[209], mem_outb[208], mem_outb[207], mem_outb[206], mem_outb[205], mem_outb[204], mem_outb[203], mem_outb[202], mem_outb[201], mem_outb[200]}));

	mux_2level_tapbuf_size32_mem mem_left_track_27 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_12_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_13_ccff_tail),
		.mem_out({mem_out[223], mem_out[222], mem_out[221], mem_out[220], mem_out[219], mem_out[218], mem_out[217], mem_out[216], mem_out[215], mem_out[214], mem_out[213], mem_out[212]}),
		.mem_outb({mem_outb[223], mem_outb[222], mem_outb[221], mem_outb[220], mem_outb[219], mem_outb[218], mem_outb[217], mem_outb[216], mem_outb[215], mem_outb[214], mem_outb[213], mem_outb[212]}));

	mux_2level_tapbuf_size32_mem mem_left_track_29 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_13_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size32_mem_14_ccff_tail),
		.mem_out({mem_out[235], mem_out[234], mem_out[233], mem_out[232], mem_out[231], mem_out[230], mem_out[229], mem_out[228], mem_out[227], mem_out[226], mem_out[225], mem_out[224]}),
		.mem_outb({mem_outb[235], mem_outb[234], mem_outb[233], mem_outb[232], mem_outb[231], mem_outb[230], mem_outb[229], mem_outb[228], mem_outb[227], mem_outb[226], mem_outb[225], mem_outb[224]}));

	mux_2level_tapbuf_size32_mem mem_left_track_31 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size32_mem_14_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[247], mem_out[246], mem_out[245], mem_out[244], mem_out[243], mem_out[242], mem_out[241], mem_out[240], mem_out[239], mem_out[238], mem_out[237], mem_out[236]}),
		.mem_outb({mem_outb[247], mem_outb[246], mem_outb[245], mem_outb[244], mem_outb[243], mem_outb[242], mem_outb[241], mem_outb[240], mem_outb[239], mem_outb[238], mem_outb[237], mem_outb[236]}));

endmodule
// ----- END Verilog module for sb_15__config_group_mem_size248 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for cbx_1__config_group_mem_size80 -----
module cbx_1__config_group_mem_size80(pReset,
                                      prog_clk,
                                      ccff_head,
                                      mem_out,
                                      mem_outb,
                                      ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [79:0] mem_out;
//----- OUTPUT PORTS -----
output [79:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size16_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_3_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_4_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_5_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_6_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size16_mem mem_top_ipin_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size16_mem_0_ccff_tail),
		.mem_out({mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_1_ccff_tail),
		.mem_out({mem_out[19], mem_out[18], mem_out[17], mem_out[16], mem_out[15], mem_out[14], mem_out[13], mem_out[12], mem_out[11], mem_out[10]}),
		.mem_outb({mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16], mem_outb[15], mem_outb[14], mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_2_ccff_tail),
		.mem_out({mem_out[29], mem_out[28], mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20]}),
		.mem_outb({mem_outb[29], mem_outb[28], mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_3_ccff_tail),
		.mem_out({mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30]}),
		.mem_outb({mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_4 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_4_ccff_tail),
		.mem_out({mem_out[49], mem_out[48], mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42], mem_out[41], mem_out[40]}),
		.mem_outb({mem_outb[49], mem_outb[48], mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42], mem_outb[41], mem_outb[40]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_5 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_5_ccff_tail),
		.mem_out({mem_out[59], mem_out[58], mem_out[57], mem_out[56], mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50]}),
		.mem_outb({mem_outb[59], mem_outb[58], mem_outb[57], mem_outb[56], mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_6 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_6_ccff_tail),
		.mem_out({mem_out[69], mem_out[68], mem_out[67], mem_out[66], mem_out[65], mem_out[64], mem_out[63], mem_out[62], mem_out[61], mem_out[60]}),
		.mem_outb({mem_outb[69], mem_outb[68], mem_outb[67], mem_outb[66], mem_outb[65], mem_outb[64], mem_outb[63], mem_outb[62], mem_outb[61], mem_outb[60]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_7 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_6_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[79], mem_out[78], mem_out[77], mem_out[76], mem_out[75], mem_out[74], mem_out[73], mem_out[72], mem_out[71], mem_out[70]}),
		.mem_outb({mem_outb[79], mem_outb[78], mem_outb[77], mem_outb[76], mem_outb[75], mem_outb[74], mem_outb[73], mem_outb[72], mem_outb[71], mem_outb[70]}));

endmodule
// ----- END Verilog module for cbx_1__config_group_mem_size80 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for cbx_2__config_group_mem_size120 -----
module cbx_2__config_group_mem_size120(pReset,
                                       prog_clk,
                                       ccff_head,
                                       mem_out,
                                       mem_outb,
                                       ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [119:0] mem_out;
//----- OUTPUT PORTS -----
output [119:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size16_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_10_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_3_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_4_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_5_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_6_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_7_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_8_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_9_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size16_mem mem_top_ipin_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size16_mem_0_ccff_tail),
		.mem_out({mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_1_ccff_tail),
		.mem_out({mem_out[19], mem_out[18], mem_out[17], mem_out[16], mem_out[15], mem_out[14], mem_out[13], mem_out[12], mem_out[11], mem_out[10]}),
		.mem_outb({mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16], mem_outb[15], mem_outb[14], mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_2_ccff_tail),
		.mem_out({mem_out[29], mem_out[28], mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20]}),
		.mem_outb({mem_outb[29], mem_outb[28], mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_3_ccff_tail),
		.mem_out({mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30]}),
		.mem_outb({mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_4 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_4_ccff_tail),
		.mem_out({mem_out[49], mem_out[48], mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42], mem_out[41], mem_out[40]}),
		.mem_outb({mem_outb[49], mem_outb[48], mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42], mem_outb[41], mem_outb[40]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_5 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_5_ccff_tail),
		.mem_out({mem_out[59], mem_out[58], mem_out[57], mem_out[56], mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50]}),
		.mem_outb({mem_outb[59], mem_outb[58], mem_outb[57], mem_outb[56], mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_6 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_6_ccff_tail),
		.mem_out({mem_out[69], mem_out[68], mem_out[67], mem_out[66], mem_out[65], mem_out[64], mem_out[63], mem_out[62], mem_out[61], mem_out[60]}),
		.mem_outb({mem_outb[69], mem_outb[68], mem_outb[67], mem_outb[66], mem_outb[65], mem_outb[64], mem_outb[63], mem_outb[62], mem_outb[61], mem_outb[60]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_7 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_6_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_7_ccff_tail),
		.mem_out({mem_out[79], mem_out[78], mem_out[77], mem_out[76], mem_out[75], mem_out[74], mem_out[73], mem_out[72], mem_out[71], mem_out[70]}),
		.mem_outb({mem_outb[79], mem_outb[78], mem_outb[77], mem_outb[76], mem_outb[75], mem_outb[74], mem_outb[73], mem_outb[72], mem_outb[71], mem_outb[70]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_8 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_7_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_8_ccff_tail),
		.mem_out({mem_out[89], mem_out[88], mem_out[87], mem_out[86], mem_out[85], mem_out[84], mem_out[83], mem_out[82], mem_out[81], mem_out[80]}),
		.mem_outb({mem_outb[89], mem_outb[88], mem_outb[87], mem_outb[86], mem_outb[85], mem_outb[84], mem_outb[83], mem_outb[82], mem_outb[81], mem_outb[80]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_9 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_8_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_9_ccff_tail),
		.mem_out({mem_out[99], mem_out[98], mem_out[97], mem_out[96], mem_out[95], mem_out[94], mem_out[93], mem_out[92], mem_out[91], mem_out[90]}),
		.mem_outb({mem_outb[99], mem_outb[98], mem_outb[97], mem_outb[96], mem_outb[95], mem_outb[94], mem_outb[93], mem_outb[92], mem_outb[91], mem_outb[90]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_10 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_9_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_10_ccff_tail),
		.mem_out({mem_out[109], mem_out[108], mem_out[107], mem_out[106], mem_out[105], mem_out[104], mem_out[103], mem_out[102], mem_out[101], mem_out[100]}),
		.mem_outb({mem_outb[109], mem_outb[108], mem_outb[107], mem_outb[106], mem_outb[105], mem_outb[104], mem_outb[103], mem_outb[102], mem_outb[101], mem_outb[100]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_11 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_10_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[119], mem_out[118], mem_out[117], mem_out[116], mem_out[115], mem_out[114], mem_out[113], mem_out[112], mem_out[111], mem_out[110]}),
		.mem_outb({mem_outb[119], mem_outb[118], mem_outb[117], mem_outb[116], mem_outb[115], mem_outb[114], mem_outb[113], mem_outb[112], mem_outb[111], mem_outb[110]}));

endmodule
// ----- END Verilog module for cbx_2__config_group_mem_size120 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for cbx_3__config_group_mem_size80 -----
module cbx_3__config_group_mem_size80(pReset,
                                      prog_clk,
                                      ccff_head,
                                      mem_out,
                                      mem_outb,
                                      ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [79:0] mem_out;
//----- OUTPUT PORTS -----
output [79:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size16_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_3_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_4_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_5_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_6_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size16_mem mem_top_ipin_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size16_mem_0_ccff_tail),
		.mem_out({mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_1_ccff_tail),
		.mem_out({mem_out[19], mem_out[18], mem_out[17], mem_out[16], mem_out[15], mem_out[14], mem_out[13], mem_out[12], mem_out[11], mem_out[10]}),
		.mem_outb({mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16], mem_outb[15], mem_outb[14], mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_2_ccff_tail),
		.mem_out({mem_out[29], mem_out[28], mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20]}),
		.mem_outb({mem_outb[29], mem_outb[28], mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_3_ccff_tail),
		.mem_out({mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30]}),
		.mem_outb({mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_4 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_4_ccff_tail),
		.mem_out({mem_out[49], mem_out[48], mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42], mem_out[41], mem_out[40]}),
		.mem_outb({mem_outb[49], mem_outb[48], mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42], mem_outb[41], mem_outb[40]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_5 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_5_ccff_tail),
		.mem_out({mem_out[59], mem_out[58], mem_out[57], mem_out[56], mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50]}),
		.mem_outb({mem_outb[59], mem_outb[58], mem_outb[57], mem_outb[56], mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_6 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_6_ccff_tail),
		.mem_out({mem_out[69], mem_out[68], mem_out[67], mem_out[66], mem_out[65], mem_out[64], mem_out[63], mem_out[62], mem_out[61], mem_out[60]}),
		.mem_outb({mem_outb[69], mem_outb[68], mem_outb[67], mem_outb[66], mem_outb[65], mem_outb[64], mem_outb[63], mem_outb[62], mem_outb[61], mem_outb[60]}));

	mux_2level_tapbuf_size16_mem mem_top_ipin_7 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_6_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[79], mem_out[78], mem_out[77], mem_out[76], mem_out[75], mem_out[74], mem_out[73], mem_out[72], mem_out[71], mem_out[70]}),
		.mem_outb({mem_outb[79], mem_outb[78], mem_outb[77], mem_outb[76], mem_outb[75], mem_outb[74], mem_outb[73], mem_outb[72], mem_outb[71], mem_outb[70]}));

endmodule
// ----- END Verilog module for cbx_3__config_group_mem_size80 -----

//----- Default net type -----
`default_nettype wire




//----- Default net type -----
`default_nettype none

// ----- Verilog module for cby_1__config_group_mem_size80 -----
module cby_1__config_group_mem_size80(pReset,
                                      prog_clk,
                                      ccff_head,
                                      mem_out,
                                      mem_outb,
                                      ccff_tail);
//----- GLOBAL PORTS -----
input [0:0] pReset;
//----- GLOBAL PORTS -----
input [0:0] prog_clk;
//----- INPUT PORTS -----
input [0:0] ccff_head;
//----- OUTPUT PORTS -----
output [79:0] mem_out;
//----- OUTPUT PORTS -----
output [79:0] mem_outb;
//----- OUTPUT PORTS -----
output [0:0] ccff_tail;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----


wire [0:0] mux_2level_tapbuf_size16_mem_0_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_1_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_2_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_3_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_4_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_5_ccff_tail;
wire [0:0] mux_2level_tapbuf_size16_mem_6_ccff_tail;

// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	mux_2level_tapbuf_size16_mem mem_right_ipin_0 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(ccff_head),
		.ccff_tail(mux_2level_tapbuf_size16_mem_0_ccff_tail),
		.mem_out({mem_out[9], mem_out[8], mem_out[7], mem_out[6], mem_out[5], mem_out[4], mem_out[3], mem_out[2], mem_out[1], mem_out[0]}),
		.mem_outb({mem_outb[9], mem_outb[8], mem_outb[7], mem_outb[6], mem_outb[5], mem_outb[4], mem_outb[3], mem_outb[2], mem_outb[1], mem_outb[0]}));

	mux_2level_tapbuf_size16_mem mem_right_ipin_1 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_0_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_1_ccff_tail),
		.mem_out({mem_out[19], mem_out[18], mem_out[17], mem_out[16], mem_out[15], mem_out[14], mem_out[13], mem_out[12], mem_out[11], mem_out[10]}),
		.mem_outb({mem_outb[19], mem_outb[18], mem_outb[17], mem_outb[16], mem_outb[15], mem_outb[14], mem_outb[13], mem_outb[12], mem_outb[11], mem_outb[10]}));

	mux_2level_tapbuf_size16_mem mem_right_ipin_2 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_1_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_2_ccff_tail),
		.mem_out({mem_out[29], mem_out[28], mem_out[27], mem_out[26], mem_out[25], mem_out[24], mem_out[23], mem_out[22], mem_out[21], mem_out[20]}),
		.mem_outb({mem_outb[29], mem_outb[28], mem_outb[27], mem_outb[26], mem_outb[25], mem_outb[24], mem_outb[23], mem_outb[22], mem_outb[21], mem_outb[20]}));

	mux_2level_tapbuf_size16_mem mem_right_ipin_3 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_2_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_3_ccff_tail),
		.mem_out({mem_out[39], mem_out[38], mem_out[37], mem_out[36], mem_out[35], mem_out[34], mem_out[33], mem_out[32], mem_out[31], mem_out[30]}),
		.mem_outb({mem_outb[39], mem_outb[38], mem_outb[37], mem_outb[36], mem_outb[35], mem_outb[34], mem_outb[33], mem_outb[32], mem_outb[31], mem_outb[30]}));

	mux_2level_tapbuf_size16_mem mem_right_ipin_4 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_3_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_4_ccff_tail),
		.mem_out({mem_out[49], mem_out[48], mem_out[47], mem_out[46], mem_out[45], mem_out[44], mem_out[43], mem_out[42], mem_out[41], mem_out[40]}),
		.mem_outb({mem_outb[49], mem_outb[48], mem_outb[47], mem_outb[46], mem_outb[45], mem_outb[44], mem_outb[43], mem_outb[42], mem_outb[41], mem_outb[40]}));

	mux_2level_tapbuf_size16_mem mem_right_ipin_5 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_4_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_5_ccff_tail),
		.mem_out({mem_out[59], mem_out[58], mem_out[57], mem_out[56], mem_out[55], mem_out[54], mem_out[53], mem_out[52], mem_out[51], mem_out[50]}),
		.mem_outb({mem_outb[59], mem_outb[58], mem_outb[57], mem_outb[56], mem_outb[55], mem_outb[54], mem_outb[53], mem_outb[52], mem_outb[51], mem_outb[50]}));

	mux_2level_tapbuf_size16_mem mem_right_ipin_6 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_5_ccff_tail),
		.ccff_tail(mux_2level_tapbuf_size16_mem_6_ccff_tail),
		.mem_out({mem_out[69], mem_out[68], mem_out[67], mem_out[66], mem_out[65], mem_out[64], mem_out[63], mem_out[62], mem_out[61], mem_out[60]}),
		.mem_outb({mem_outb[69], mem_outb[68], mem_outb[67], mem_outb[66], mem_outb[65], mem_outb[64], mem_outb[63], mem_outb[62], mem_outb[61], mem_outb[60]}));

	mux_2level_tapbuf_size16_mem mem_right_ipin_7 (
		.pReset(pReset),
		.prog_clk(prog_clk),
		.ccff_head(mux_2level_tapbuf_size16_mem_6_ccff_tail),
		.ccff_tail(ccff_tail),
		.mem_out({mem_out[79], mem_out[78], mem_out[77], mem_out[76], mem_out[75], mem_out[74], mem_out[73], mem_out[72], mem_out[71], mem_out[70]}),
		.mem_outb({mem_outb[79], mem_outb[78], mem_outb[77], mem_outb[76], mem_outb[75], mem_outb[74], mem_outb[73], mem_outb[72], mem_outb[71], mem_outb[70]}));

endmodule
// ----- END Verilog module for cby_1__config_group_mem_size80 -----

//----- Default net type -----
`default_nettype wire




