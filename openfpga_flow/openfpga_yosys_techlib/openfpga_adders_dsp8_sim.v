//---------------------------------------
// 1-bit adder 
//---------------------------------------
(* abc9_box, lib_whitebox *)
module adder(
    output sumout,
    output cout,
    input a,
    input b,
    input cin
);
    assign sumout = a ^ b ^ cin;
    assign cout = (a & b) | ((a | b) & cin);

endmodule

//-----------------------------
// 8-bit multiplier
//-----------------------------
module mult_8(
  input [0:7] A,
  input [0:7] B,
  output [0:15] Y
);

assign Y = A * B;

endmodule
