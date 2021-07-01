`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/03/2021 03:37:44 PM
// Design Name: 
// Module Name: pulse_generator
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: A simple pulse generator with configurable initial values and waiting cycles
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

// Uncomment if using Vivado to synthesize the design. This will enable the initial block
// If using Yosys, initial blocks are not supported, and cannot be included.
// `define VIVADO_SYNTHESIS

module pulse_generator(
  input clk_in,
  input repeated, // Specify if the pulse should be generated repeatedly
  output reg pulse
  );
    
    
  parameter INITIAL_VALUE=0; // Define the initial value for the pulse, either 0 or 1; The pulse logic level will be a flip over the initial value
  parameter WAIT_CYCLES=0; // Define the number of clock cycles to wait before the pulse is applied
  parameter PULSE_WIDTH=1; // Define the length of the pulse width 
  parameter PULSE_COUNTER_SIZE=10; // Define the size of the pulse width counter
  
  reg [WAIT_CYCLES<=2 ? 2 : $clog2(WAIT_CYCLES) : 0] wait_cycle_counter; // Size of wait counter is determined by WAIT_CYCLES
  reg [PULSE_COUNTER_SIZE - 1 : 0] pulse_width_counter;
  reg pulse_start;
  reg pulse_end;

`ifdef VIVADO_SYNTHESIS
  initial begin
    pulse <= INITIAL_VALUE;
    pulse_start <= 1'b0;
    pulse_end <= 1'b0;
    wait_cycle_counter <= 0;
    pulse_width_counter <= 0;
  end
`endif
  
  // Wait a number of clock cycles, hold the initial value
  always @(posedge clk_in) begin
    if (wait_cycle_counter == WAIT_CYCLES) begin
      pulse_start <= 1'b1;
    end
    if (~pulse_start) begin
      wait_cycle_counter <= wait_cycle_counter + 1;
    end
  end
  
  // Wait a number of clock cycles, hold the initial value
  always @(posedge clk_in) begin
    pulse <= INITIAL_VALUE;
    if (pulse_start && ~pulse_end) begin
      // Reach the pulse width limit, stop counting
      if (pulse_width_counter < PULSE_WIDTH) begin
        pulse <= ~INITIAL_VALUE;
        if (~repeated) begin
          pulse_end = 1'b1;
        end
      end
      // When pulse ends, flip to initial value
      if (pulse_end) begin
        pulse <= INITIAL_VALUE;
      end
      pulse_width_counter <= pulse_width_counter + 1;
    end
  end
  
endmodule
