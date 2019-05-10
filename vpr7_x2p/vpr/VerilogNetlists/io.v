//------ Module: iopad -----//
//------ Verilog file: io.v -----//
//------ Author: Xifan TANG -----//
module iopad(
//input zin, // Set output to be Z
input dout, // Data output
output din, // Data input
inout pad, // bi-directional pad
input direction // enable signal to control direction of iopad
//input direction_inv // enable signal to control direction of iopad
);
  //----- when direction enabled, the signal is propagated from pad to din
  assign din = direction ? pad : 1'bz;
  //----- when direction is disabled, the signal is propagated from dout to pad
  assign pad = direction ? 1'bz : dout;
endmodule
