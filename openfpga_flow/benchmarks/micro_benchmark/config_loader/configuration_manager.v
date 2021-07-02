`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/05/2021 10:29:55 AM
// Design Name: 
// Module Name: configuration_manager
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

`include "clock_divider.v"
`include "pulse_generator.v"

module configuration_manager(
    input clk_in,
    output prog_reset,
    output prog_clk,
    output ccff_head,
    output configuration_done
    );
    
    parameter START_CYCLE=3; // Start configuration on cycle 3 of prog_clk
    parameter CONFIGURATION_CLK_DIV_SIZE=12; // Divide clk_in (50MHz) by 4096 (2^12) times
    
    wire prog_clk_out; // prog_clk signal from clk_divider
    wire ccff_head_out;
    
    assign ccff_head = ccff_head_out & ~prog_reset;
    assign prog_clk = prog_clk_out & ~configuration_done; // prog_clk will stop when configuration done
    
    // PRESET
    // Programming reset will be enabled until START_CYCLE
    reset_generator #(
      .INITIAL_VALUE(1),
      .ACTIVE_CYCLES(START_CYCLE)
    ) prog_reset_generator(
      .clk(~prog_clk),
      .pulse(prog_reset)
    );
    
    
    // PROG_CLK
    // Divide pl_clk (50MHz) by 4096 (2^12) times
    clock_divider #(
      .CLK_DIVIDER_SIZE(CONFIGURATION_CLK_DIV_SIZE)
      ) prog_clk_divider (
        .clk_in(clk_in),
        .clk_out(prog_clk_out)
      ); 
      
      
    // Instantiate bitstream loader
    bitstream_loader loader (
      .prog_clk(prog_clk),
      .config_chain_head(ccff_head_out),
      .start(~prog_reset),
      .done(configuration_done)
    );
    
endmodule
