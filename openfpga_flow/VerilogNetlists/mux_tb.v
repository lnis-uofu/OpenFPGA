//-----------------------------------------------------
// Design Name : testbench for 2-level SRAM MUX
// File Name   : mux_tb.v
// Function    : SRAM-based 2-level MUXes
// Coder       : Xifan TANG
//-----------------------------------------------------
//----- Time scale: simulation time step and accuracy -----
`timescale 1ns / 1ps

module cmos_mux2level_tb;
// Parameters
parameter SIZE_OF_MUX = 50; //---- MUX input size 
parameter SIZE_OF_SRAM = 16; //---- MUX input size 
parameter op_clk_period = 1; // [ns] half clock period
parameter operating_period =  SIZE_OF_MUX * 2 * op_clk_period; // [ns] One full clock period

// voltage sources
wire [0:SIZE_OF_MUX-1] in;
wire out;
wire [0:SIZE_OF_SRAM-1] sram;
wire [0:SIZE_OF_SRAM-1] sram_inv; 
// clocks
wire op_clock;
// registered ports 
reg op_clock_reg;
reg [0:SIZE_OF_MUX-1] in_reg;
reg [0:SIZE_OF_SRAM-1] sram_reg;
reg [0:SIZE_OF_SRAM-1] sram_inv_reg;
// Config done signal;
reg config_done;
// Temp register for rotating shift
reg temp;

// Unit Under Test
mux_2level_size50 U0 (in, out, sram, sram_inv);

// Task: assign inputs
task op_mux_input;
  begin
    @(posedge op_clock);
    temp = in_reg[SIZE_OF_MUX-1];
    in_reg[1:SIZE_OF_MUX-1] = in_reg[0:SIZE_OF_MUX-2];
    in_reg[0] = temp;
  end 
endtask

// Configuration done signal
initial
begin
  config_done = 1'b1;
end

// Operating clocks
initial 
begin
  op_clock_reg = 1'b0;
end
always
begin
  #op_clk_period op_clock_reg = ~op_clock_reg; 
end

// Programming and Operating clocks 
assign op_clock = op_clock_reg & config_done;

// Operating Phase
initial
begin
  in_reg = {SIZE_OF_MUX {1'b0}}; 
  in_reg[0] = 1'b1; // Last bit is 1 initially
end
always wait (config_done) // Only invoked when config_done is 1
begin
  /* Update inputs */
  op_mux_input;
end

// Wire ports
assign in = in_reg;
assign sram[0:7] = 8'b00010000;
assign sram[8:15] = 8'b00010000;
assign sram_inv = ~sram;

endmodule

