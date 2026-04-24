//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Verilog modules for Unique Switch Blocks[0][6]
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
//----- Default net type -----
`default_nettype none

// ----- Verilog module for sb_0__6_ -----
module sb_0__6_(chanx_right_in,
                chany_bottom_in,
                chanx_right_out,
                chany_bottom_out);
//----- INPUT PORTS -----
input [15:0] chanx_right_in;
//----- INPUT PORTS -----
input [15:0] chany_bottom_in;
//----- OUTPUT PORTS -----
output [15:0] chanx_right_out;
//----- OUTPUT PORTS -----
output [15:0] chany_bottom_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----



// ----- BEGIN Local short connections -----
// ----- END Local short connections -----
// ----- BEGIN Local output short connections -----
// ----- END Local output short connections -----

endmodule
// ----- END Verilog module for sb_0__6_ -----

//----- Default net type -----
`default_nettype wire



