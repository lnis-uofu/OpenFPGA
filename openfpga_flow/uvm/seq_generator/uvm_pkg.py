// Include file for all UVM package & UVM testbench.

`timescale 1ns / 1ps

// Parameter for scalability

parameter  GPIO_WIDTH = ${gpio_width};
parameter  ADDR_WIDTH = ${addr_width};

`include "../tb/clknrst_agent/clknrst_pkg.sv"
`include "../tb/bs_agent/bs_pkg.sv"
`include "../tb/stimuli_agent/stimuli_pkg.sv"
`include "../tb/openfpga_env/openfpga_env_pkg.sv"
`include "../tb/openfpga_tb/openfpga_tb_pkg.sv"
`include "../tb/openfpga_tb/openfpga_tb.sv"
