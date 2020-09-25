//-----------------------------------------------------
// Design Name : testbench for static_dff
// File Name   : ff_tb.v
// Function    : D flip-flop with asyn reset and set
// Coder       : Xifan TANG
//-----------------------------------------------------
//----- Time scale: simulation time step and accuracy -----
`timescale 1ns / 1ps

module sram6T_rram_tb;
// voltage sources 
wire read;
wire nequalize;
wire din;
wire dout;
wire doutb;
reg [0:2] bl;
reg [0:2] wl;
reg prog_clock;

// Parameters 
parameter prog_clk_period = 2; // [ns] a full clock period

// Unit Under Test
sram6T_rram U0 (read, nequalize, din, dout, doutb, bl, wl);

// Voltage stimuli
// read : alway disabled 
assign read = 1'b0;

// nequalize: always disabled 
assign nequalize = 1'b1;

// din: always disabled 
assign din = 1'b0;

// Programming clock 
initial
begin 
  prog_clock = 1'b0;
end 
always
begin
  #prog_clk_period prog_clock = ~prog_clock;
end

// Task: assign BL and WL values 
task prog_blwl;
  input [0:2] bl_val;
  input [0:2] wl_val;
  begin
    @(posedge prog_clock);
    bl = bl_val;
    wl = wl_val;  
  end 
endtask

// Test two cases:
// 1. Program dout to 0 
// bl[0] = 1, wl[2] = 1  
// bl[2] = 1, wl[0] = 1  
// 2. Program dout to 1 
// bl[1] = 1, wl[2] = 1  
// bl[2] = 1, wl[1] = 1  
initial 
begin
  bl = 3'b000;
  wl = 3'b000;
// 1. Program dout to 0 
// bl[0] = 1, wl[2] = 1  
  prog_blwl(3'b100, 3'b001);
// bl[2] = 1, wl[0] = 1  
  prog_blwl(3'b001, 3'b100);
// 2. Program dout to 1 
// bl[1] = 1, wl[2] = 1  
  prog_blwl(3'b010, 3'b001);
// bl[2] = 1, wl[1] = 1  
  prog_blwl(3'b100, 3'b010);
// 3. Program dout to 0 
// bl[0] = 1, wl[2] = 1  
  prog_blwl(3'b100, 3'b001);
// bl[2] = 1, wl[0] = 1  
  prog_blwl(3'b001, 3'b100);
end

// Outputs are wired to dout and doutb

endmodule
