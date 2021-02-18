// ----- Verilog module for OR2 -----
module OR2(a,
           b,
           out);
//----- INPUT PORTS -----
input [0:0] a;
//----- INPUT PORTS -----
input [0:0] b;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

// ----- Verilog codes of a 2-input 1-output AND gate -----
	assign out[0] = a[0] | b[0];

`ifdef ENABLE_TIMING
// ------ BEGIN Pin-to-pin Timing constraints -----
	specify
		(a[0] => out[0]) = (0.01, 0.01);
		(b[0] => out[0]) = (0.005, 0.005);
	endspecify
// ------ END Pin-to-pin Timing constraints -----
`endif
endmodule
// ----- END Verilog module for OR2 -----
