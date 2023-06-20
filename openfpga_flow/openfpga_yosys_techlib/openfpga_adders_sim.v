//---------------------------------------
// 1-bit adder 
//---------------------------------------
(* abc9_box, lib_whitebox *)
module adder(
	input cin,
	input a,
	input b,
	output cout,
	output sumout );


	assign sumout = a ^ b ^ cin;
	assign cout = (a & b) | ((a | b) & cin);

endmodule
