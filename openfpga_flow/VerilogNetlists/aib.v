//-----------------------------------------------------
// Design Name : AIB interface
// File Name   : aib.v
// Function    : A wrapper for AIB interface
// Coder       : Xifan Tang
//-----------------------------------------------------

module AIB (
	input TXCLK,
	input RXCLK,
	inout[0:79] PAD,
	input[0:79] TXDATA,
	output[0:79] RXDATA);

// May add the logic function of a real AIB
// Refer to the offical AIB github
//   https://github.com/intel/aib-phy-hardware

endmodule
