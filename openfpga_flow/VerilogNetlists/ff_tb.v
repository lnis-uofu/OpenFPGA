//-----------------------------------------------------
// Design Name : testbench for static_dff
// File Name   : ff_tb.v
// Function    : D flip-flop with asyn reset and set
// Coder       : Xifan TANG
//-----------------------------------------------------
//----- Time scale: simulation time step and accuracy -----
`timescale 1ns / 1ps

module static_dff_tb;
// voltage sources 
wire set;
reg reset;
reg clk;
reg D;
wire Q;
// Parameters 
parameter clk_period = 2; // [ns] a full clock period
parameter half_clk_period = clk_period / 2; // [ns] a half clock period
parameter d_period = 2 * clk_period; // [ns] two clock period
parameter reset_period = 8 * clk_period; // [ns] a full clock period

// Unit Under Test
static_dff U0 (set, reset, clk, D, Q);

// Voltage stimuli
// Reset : enable in the first clock cycle and then disabled 
initial 
begin
  reset = 1'b1;
  #clk_period reset = ~reset;
end
always 
begin
  #reset_period reset = ~reset;
end
 
// set : alway disabled 
assign set = 1'b0;

// clk: clock generator 
initial 
begin
  clk = 1'b0;
end
always 
begin
  #half_clk_period clk = ~clk;
end

// D: input, flip every two clock cycles
initial 
begin
  D = 1'b0;
end
always 
begin
  #d_period D = ~D;
end

// Q is an output 
//

endmodule
