//-------------------------------------------------------
//  Functionality: A 8-bit multiply circuit using macro
//  Author:        Tarachand Pagarani
//-------------------------------------------------------

module mult8(a, b, out);
parameter DATA_WIDTH = 8;  /* declare a parameter. default required */
input [DATA_WIDTH - 1 : 0] a, b;
output [2*DATA_WIDTH - 1 : 0] out;

(* keep *)
  mult_8 #(.MODE(1'b1)) DSP (
		.A(a),
		.B(b),
		.Y(out),
	);

endmodule









