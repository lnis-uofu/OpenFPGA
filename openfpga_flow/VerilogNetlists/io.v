//------ Module: iopad -----//
//------ Verilog file: io.v -----//
//------ Author: Xifan TANG -----//
module iopad(
//input zin, // Set output to be Z
input outpad, // Data output
output inpad, // Data input
inout pad, // bi-directional pad
input en // enable signal to control direction of iopad
//input direction_inv // enable signal to control direction of iopad
);
  //----- when direction enabled, the signal is propagated from pad to din
  assign inpad = en ? pad : 1'bz;
  //----- when direction is disabled, the signal is propagated from dout to pad
  assign pad = en ? 1'bz : outpad;
endmodule
