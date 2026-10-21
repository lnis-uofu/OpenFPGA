module square_32(x, out);
input [31:0] x;
output [63:0] out;
assign out = x * x;
endmodule
