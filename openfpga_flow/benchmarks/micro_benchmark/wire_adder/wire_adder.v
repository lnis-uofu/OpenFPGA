/////////////////////////////////////////
//  Functionality: A wire but implemented by an adder
//  Author:        Xifan Tang
////////////////////////////////////////
`timescale 1ns / 1ps

module wire_adder(a, b);

input wire a;
output wire b;

assign b = a;

endmodule
