//-----------------------------------------------------
// Design Name : General Purpose I/Os
// File Name   : gpio.v
// Coder       : Xifan TANG
//-----------------------------------------------------

//-----------------------------------------------------
// Function    : A minimum general purpose I/O
//-----------------------------------------------------
module GPIO (
  input A, // Data output
  output Y, // Data input
  inout PAD, // bi-directional pad
  input DIR // direction control
);
  //----- when direction enabled, the signal is propagated from PAD to data input
  assign Y = DIR ? PAD : 1'bz;
  //----- when direction is disabled, the signal is propagated from data out to pad
  assign PAD = DIR ? 1'bz : A;
endmodule
