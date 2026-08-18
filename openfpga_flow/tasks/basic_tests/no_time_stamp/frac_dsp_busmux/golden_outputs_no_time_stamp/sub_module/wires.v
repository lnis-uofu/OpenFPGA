//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Wires
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- BEGIN Verilog modules for regular wires -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for direct_interc -----
module direct_interc(in,
                     out);
//----- INPUT PORTS -----
input [0:0] in;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

wire [0:0] in;
wire [0:0] out;
	assign out[0] = in[0];
endmodule
// ----- END Verilog module for direct_interc -----

//----- Default net type -----
`default_nettype wire


// ----- END Verilog modules for regular wires -----
