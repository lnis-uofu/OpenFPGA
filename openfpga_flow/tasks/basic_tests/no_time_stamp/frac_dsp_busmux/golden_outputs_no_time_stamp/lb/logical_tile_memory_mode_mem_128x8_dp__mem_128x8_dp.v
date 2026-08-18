//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for primitive pb_type: mem_128x8_dp
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_memory_mode_mem_128x8_dp__mem_128x8_dp -----
module logical_tile_memory_mode_mem_128x8_dp__mem_128x8_dp(clk,
                                                           mem_128x8_dp_waddr,
                                                           mem_128x8_dp_raddr,
                                                           mem_128x8_dp_d_in,
                                                           mem_128x8_dp_wen,
                                                           mem_128x8_dp_ren,
                                                           mem_128x8_dp_d_out,
                                                           mem_128x8_dp_clk);
//----- GLOBAL PORTS -----
input [0:0] clk;
//----- INPUT PORTS -----
input [0:6] mem_128x8_dp_waddr;
//----- INPUT PORTS -----
input [0:6] mem_128x8_dp_raddr;
//----- INPUT PORTS -----
input [0:7] mem_128x8_dp_d_in;
//----- INPUT PORTS -----
input [0:0] mem_128x8_dp_wen;
//----- INPUT PORTS -----
input [0:0] mem_128x8_dp_ren;
//----- OUTPUT PORTS -----
output [0:7] mem_128x8_dp_d_out;
//----- CLOCK PORTS -----
input [0:0] mem_128x8_dp_clk;

//----- BEGIN wire-connection ports -----
wire [0:6] mem_128x8_dp_waddr;
wire [0:6] mem_128x8_dp_raddr;
wire [0:7] mem_128x8_dp_d_in;
wire [0:0] mem_128x8_dp_wen;
wire [0:0] mem_128x8_dp_ren;
wire [0:7] mem_128x8_dp_d_out;
wire [0:0] mem_128x8_dp_clk;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	dpram_128x8 dpram_128x8_0_ (
		.clk(clk),
		.waddr(mem_128x8_dp_waddr[0:6]),
		.raddr(mem_128x8_dp_raddr[0:6]),
		.d_in(mem_128x8_dp_d_in[0:7]),
		.wen(mem_128x8_dp_wen),
		.ren(mem_128x8_dp_ren),
		.d_out(mem_128x8_dp_d_out[0:7]));

endmodule
// ----- END Verilog module for logical_tile_memory_mode_mem_128x8_dp__mem_128x8_dp -----

//----- Default net type -----
`default_nettype wire



