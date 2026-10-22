module mult_32x32(a, b, out);
input [31:0] a;
input [31:0] b;
output [63:0] out;
assign out = a * b;
endmodule
