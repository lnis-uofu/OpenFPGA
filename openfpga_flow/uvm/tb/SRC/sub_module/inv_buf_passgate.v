//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Essential gates
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:39 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for const0 -----
module const0(const0);
//----- OUTPUT PORTS -----
output [0:0] const0;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

	assign const0[0] = {1{1'b0}};
endmodule
// ----- END Verilog module for const0 -----

// ----- Verilog module for const1 -----
module const1(const1);
//----- OUTPUT PORTS -----
output [0:0] const1;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

	assign const1[0] = {1{1'b1}};
endmodule
// ----- END Verilog module for const1 -----

// ----- Verilog module for INVTX1 -----
module INVTX1(in,
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
	assign out = (in === 1'bz)? $random : ~in;

`ifdef ENABLE_TIMING
// ------ BEGIN Pin-to-pin Timing constraints -----
	specify
		(in[0] => out[0]) = (0.01, 0.01);
	endspecify
// ------ END Pin-to-pin Timing constraints -----
`endif

`ifdef ENABLE_SIGNAL_INITIALIZATION
// ------ BEGIN driver initialization -----
	initial begin
	`ifdef ENABLE_FORMAL_VERIFICATION
		$deposit(in[0], 1'b0);
	`else
		$deposit(in[0], $random);
	`endif

	end
// ------ END driver initialization -----
`endif
endmodule
// ----- END Verilog module for INVTX1 -----

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
	assign out = (in === 1'bz)? $random : in;

`ifdef ENABLE_TIMING
// ------ BEGIN Pin-to-pin Timing constraints -----
	specify
		(in[0] => out[0]) = (0.01, 0.01);
	endspecify
// ------ END Pin-to-pin Timing constraints -----
`endif

`ifdef ENABLE_SIGNAL_INITIALIZATION
// ------ BEGIN driver initialization -----
	initial begin
	`ifdef ENABLE_FORMAL_VERIFICATION
		$deposit(in[0], 1'b0);
	`else
		$deposit(in[0], $random);
	`endif

	end
// ------ END driver initialization -----
`endif
endmodule
// ----- END Verilog module for buf4 -----

// ----- Verilog module for tap_buf4 -----
module tap_buf4(in,
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
	assign out = (in === 1'bz)? $random : ~in;

`ifdef ENABLE_TIMING
// ------ BEGIN Pin-to-pin Timing constraints -----
	specify
		(in[0] => out[0]) = (0.01, 0.01);
	endspecify
// ------ END Pin-to-pin Timing constraints -----
`endif

`ifdef ENABLE_SIGNAL_INITIALIZATION
// ------ BEGIN driver initialization -----
	initial begin
	`ifdef ENABLE_FORMAL_VERIFICATION
		$deposit(in[0], 1'b0);
	`else
		$deposit(in[0], $random);
	`endif

	end
// ------ END driver initialization -----
`endif
endmodule
// ----- END Verilog module for tap_buf4 -----

// ----- Verilog module for TGATE -----
module TGATE(in,
             sel,
             selb,
             out);
//----- INPUT PORTS -----
input [0:0] in;
//----- INPUT PORTS -----
input [0:0] sel;
//----- INPUT PORTS -----
input [0:0] selb;
//----- OUTPUT PORTS -----
output [0:0] out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

	assign out = sel ? in : 1'bz;

`ifdef ENABLE_TIMING
// ------ BEGIN Pin-to-pin Timing constraints -----
	specify
		(in[0] => out[0]) = (0.01, 0.01);
		(sel[0] => out[0]) = (0.005, 0.005);
		(selb[0] => out[0]) = (0.005, 0.005);
	endspecify
// ------ END Pin-to-pin Timing constraints -----
`endif

`ifdef ENABLE_SIGNAL_INITIALIZATION
// ------ BEGIN driver initialization -----
	initial begin
	`ifdef ENABLE_FORMAL_VERIFICATION
		$deposit(in[0], 1'b0);
		$deposit(sel[0], 1'b0);
		$deposit(selb[0], 1'b0);
	`else
		$deposit(in[0], $random);
		$deposit(sel[0], $random);
		$deposit(selb[0], $random);
	`endif

	end
// ------ END driver initialization -----
`endif
endmodule
// ----- END Verilog module for TGATE -----

