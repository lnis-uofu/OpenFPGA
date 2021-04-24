//-------------------------------------------------------
//  Functionality: A 16-bit multiply-acculumate circuit
//  Author:        Xifan Tang
//-------------------------------------------------------

module mac_16(a, b, c, out);
parameter DATA_WIDTH = 16;  /* declare a parameter. default required */
input [DATA_WIDTH - 1 : 0] a, b, c;
output [DATA_WIDTH - 1 : 0] out;

assign out = a * b + c;

endmodule









