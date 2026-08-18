//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for primitive pb_type: adder
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder -----
module logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder(adder_a,
                                                                                    adder_b,
                                                                                    adder_cin,
                                                                                    adder_cout,
                                                                                    adder_sumout);
//----- INPUT PORTS -----
input [0:0] adder_a;
//----- INPUT PORTS -----
input [0:0] adder_b;
//----- INPUT PORTS -----
input [0:0] adder_cin;
//----- OUTPUT PORTS -----
output [0:0] adder_cout;
//----- OUTPUT PORTS -----
output [0:0] adder_sumout;

//----- BEGIN wire-connection ports -----
wire [0:0] adder_a;
wire [0:0] adder_b;
wire [0:0] adder_cin;
wire [0:0] adder_cout;
wire [0:0] adder_sumout;
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

	ADDF ADDF_0_ (
		.A(adder_a),
		.B(adder_b),
		.CI(adder_cin),
		.SUM(adder_sumout),
		.CO(adder_cout));

endmodule
// ----- END Verilog module for logical_tile_clb_mode_default__fle_mode_physical__fabric_mode_default__adder -----

//----- Default net type -----
`default_nettype wire



