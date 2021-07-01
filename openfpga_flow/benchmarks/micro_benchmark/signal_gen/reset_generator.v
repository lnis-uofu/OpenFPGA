`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/03/2021 04:52:18 PM
// Design Name: 
// Module Name: reset_generator
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
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

module reset_generator(
    input clk,
    output reg pulse
    );
    
	parameter INITIAL_VALUE=0; // Define the initial value for the pulse, either 0 or 1; The pulse logic level will be a flip over the initial value
	parameter ACTIVE_CYCLES=0; // Define the number of clock cycles to wait before the pulse is applied

	reg [ACTIVE_CYCLES<=2 ? 2 : $clog2(ACTIVE_CYCLES) - 1 : 0] active_cycle_counter;
    
`ifdef VIVADO_SYNTHESIS
    initial begin
      clkdiv_counter <= 0;
	  active_cycle_counter <= 0;
      pulse <= INITIAL_VALUE;
    end
`endif

	// Wait a number of clock cycles, hold the initial value
	always @(posedge clk) begin
		if (active_cycle_counter == ACTIVE_CYCLES) begin
			pulse <= ~pulse;
		end else begin 
			active_cycle_counter <= active_cycle_counter + 1;
		end
	end
    
endmodule
