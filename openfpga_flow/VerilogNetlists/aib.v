//-----------------------------------------------------
// Design Name : AIB interface
// File Name   : aib.v
// Function    : A wrapper for AIB interface
// Coder       : Xifan Tang
//-----------------------------------------------------

module AIB (
	input TX_CLK,
	input RX_CLK,
	inout[0:79] PAD,
	input[0:79] TX_DATA,
	output[0:79] RX_DATA);

// May add the logic function of a real AIB
// Refer to the offical AIB github
//   https://github.com/intel/aib-phy-hardware

endmodule
