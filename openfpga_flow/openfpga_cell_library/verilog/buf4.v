// ----- Verilog module for const0 -----
module const0(const0);
output [0:0] const0;
assign const0[0] = 1'b0;
endmodule

// ----- Verilog module for buf4 -----
module buf4(in,
            out);
//----- INPUT PORTS -----
input [0:0] in;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

// ----- Verilog codes of a regular inverter -----
  //assign out = (in === 1'bz)? $random : in;
  assign out = in;

`ifdef ENABLE_TIMING
// ------ BEGIN Pin-to-pin Timing constraints -----
  specify
    (in[0] => out[0]) = (0.01, 0.01);
  endspecify
// ------ END Pin-to-pin Timing constraints -----
`endif
endmodule
// ----- END Verilog module for buf4 -----

