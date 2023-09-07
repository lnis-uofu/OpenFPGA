//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Template for user-defined Verilog modules
//	Author: Xifan TANG
//	Organization: University of Utah
//-------------------------------------------
// ----- Template Verilog module for DFFSRQ -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for DFFSRQ -----
module DFFSRQ(SET,
              RST,
              CK,
              D,
              Q);
//----- GLOBAL PORTS -----
input [0:0] SET;
//----- GLOBAL PORTS -----
input [0:0] RST;
//----- GLOBAL PORTS -----
input [0:0] CK;
//----- INPUT PORTS -----
input [0:0] D;
//----- OUTPUT PORTS -----
output [0:0] Q;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

// ----- Internal logic should start here -----


// ----- Internal logic should end here -----
endmodule
// ----- END Verilog module for DFFSRQ -----

//----- Default net type -----
`default_nettype wire


// ----- Template Verilog module for DFFR -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for DFFR -----
module DFFR(RST,
            CK,
            D,
            Q,
            QN);
//----- GLOBAL PORTS -----
input [0:0] RST;
//----- GLOBAL PORTS -----
input [0:0] CK;
//----- INPUT PORTS -----
input [0:0] D;
//----- OUTPUT PORTS -----
output [0:0] Q;
//----- OUTPUT PORTS -----
output [0:0] QN;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

// ----- Internal logic should start here -----


// ----- Internal logic should end here -----
endmodule
// ----- END Verilog module for DFFR -----

//----- Default net type -----
`default_nettype wire


// ----- Template Verilog module for GPIO -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for GPIO -----
module GPIO(PAD,
            A,
            DIR,
            Y);
//----- GPIO PORTS -----
inout [0:0] PAD;
//----- INPUT PORTS -----
input [0:0] A;
//----- INPUT PORTS -----
input [0:0] DIR;
//----- OUTPUT PORTS -----
output [0:0] Y;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

// ----- Internal logic should start here -----


// ----- Internal logic should end here -----
endmodule
// ----- END Verilog module for GPIO -----

//----- Default net type -----
`default_nettype wire


// ----- Template Verilog module for ADDF -----
//----- Default net type -----
`default_nettype none

// ----- Verilog module for ADDF -----
module ADDF(A,
            B,
            CI,
            SUM,
            CO);
//----- INPUT PORTS -----
input [0:0] A;
//----- INPUT PORTS -----
input [0:0] B;
//----- INPUT PORTS -----
input [0:0] CI;
//----- OUTPUT PORTS -----
output [0:0] SUM;
//----- OUTPUT PORTS -----
output [0:0] CO;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
//----- END Registered ports -----

// ----- Internal logic should start here -----


// ----- Internal logic should end here -----
endmodule
// ----- END Verilog module for ADDF -----

//----- Default net type -----
`default_nettype wire


