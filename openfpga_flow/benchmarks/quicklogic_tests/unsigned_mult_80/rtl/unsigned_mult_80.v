module unsigned_mult_80 (out, a, b);
output [40:0] out;
wire [80:0] mult_wire;
	input  [40:0] a;
	input  [40:0] b;

	assign mult_wire = a * b;
    assign out = mult_wire[80:40] | mult_wire[39:0];

endmodule
