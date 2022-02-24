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

//-----------------------------------------------------
// Function    : A minimum general purpose I/O with config_done signal
//               which can block signals during configuration phase
//-----------------------------------------------------
module GPIO_CFGD (
  input CONFIG_DONE, // Control signal to block signals
  input A, // Data output
  output Y, // Data input
  inout PAD, // bi-directional pad
  input DIR // direction control
);
  //----- when direction enabled, the signal is propagated from PAD to data input
  assign Y = CONFIG_DONE ? (DIR ? PAD : 1'bz) : 1'bz;
  //----- when direction is disabled, the signal is propagated from data out to pad
  assign PAD = CONFIG_DONE ? (DIR ? 1'bz : A) : 1'bz;
endmodule


//-----------------------------------------------------
// Function    : A minimum input pad
//-----------------------------------------------------
module GPIN (
  inout A, // External PAD signal
  output Y // Data input
);
  assign Y = A;
endmodule

//-----------------------------------------------------
// Function    : A minimum output pad
//-----------------------------------------------------
module GPOUT (
  inout Y, // External PAD signal
  input A // Data output
);
  assign Y = A;
endmodule

//-----------------------------------------------------
// Function    : A minimum embedded I/O
//               just an overlay to interface other components
//-----------------------------------------------------
module EMBEDDED_IO (
  input SOC_IN, // Input to drive the inpad signal
  output SOC_OUT, // Output the outpad signal
  output SOC_DIR, // Output the directionality
  output FPGA_IN, // Input data to FPGA
  input FPGA_OUT, // Output data from FPGA
  input FPGA_DIR // direction control 
);

  assign FPGA_IN = SOC_IN;
  assign SOC_OUT = FPGA_OUT;
  assign SOC_DIR = FPGA_DIR;
endmodule

//-----------------------------------------------------
// Function    : An embedded I/O with an protection circuit
//               which can force the I/O in input mode
//               The enable signal IO_ISOL_N is active-low 
//-----------------------------------------------------
module EMBEDDED_IO_ISOLN (
  input SOC_IN, // Input to drive the inpad signal
  output SOC_OUT, // Output the outpad signal
  output SOC_DIR, // Output the directionality
  output FPGA_IN, // Input data to FPGA
  input FPGA_OUT, // Output data from FPGA
  input FPGA_DIR, // direction control 
  input IO_ISOL_N // Active-low signal to set the I/O in input mode
);

  assign FPGA_IN = IO_ISOL_N ? SOC_IN : 1'bz;
  assign SOC_OUT = IO_ISOL_N ? FPGA_OUT : 1'bz;
  // Direction signal is set to logic '0' when in input mode 
  assign SOC_DIR = IO_ISOL_N ? FPGA_DIR : 1'b0;
endmodule
