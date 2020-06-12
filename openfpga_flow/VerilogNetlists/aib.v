//-----------------------------------------------------
// Design Name : AIB interface
// File Name   : aib.v
// Function    : A wrapper for AIB interface
// Coder       : Xifan Tang
//-----------------------------------------------------

module aib (
	input tx_clk,
	input rx_clk,
	inout[0:79] pad,
	input[0:79] tx_data,
	output[0:79] rx_data);

// May add the logic function of a real AIB
// Refer to the offical AIB github
//   https://github.com/intel/aib-phy-hardware

endmodule
