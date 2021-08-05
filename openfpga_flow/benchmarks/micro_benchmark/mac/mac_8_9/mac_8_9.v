//-------------------------------------------------------
//  Functionality: 
//    - A 8-bit multiply-acculumate circuit 
//    - A 9-bit multiply-acculumate circuit
//  Author:        Xifan Tang
//-------------------------------------------------------

module mac_8_9(a, b, c, out);
parameter DATA_WIDTH = 18;  /* declare a parameter. default required */
input [DATA_WIDTH - 1 : 0] a, b, c;
output [DATA_WIDTH - 1 : 0] out;

assign out[8:0] = a[8:0] * b[8:0] + c[8:0];
assign out[17:9] = a[17:9] * b[17:9] + c[17:9];

endmodule









