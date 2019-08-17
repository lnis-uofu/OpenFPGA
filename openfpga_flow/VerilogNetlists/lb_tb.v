//-----------------------------------------------------
// Design Name : testbench for logic blocks
// File Name   : lb_tb.v
// Function    : Configurable logic block
// Coder       : Xifan TANG
//-----------------------------------------------------
//----- Time scale: simulation time step and accuracy -----
`timescale 1ns / 1ps

module lb_tb;
// Parameters
parameter SIZE_IN = 40; //---- MUX input size 
parameter SIZE_OUT = 10; //---- MUX input size 
parameter SIZE_RESERV_BLWL = 49 + 1; //---- MUX input size 
parameter SIZE_BLWL = 1019 - 310 + 1; //---- MUX input size 
parameter prog_clk_period = 1; // [ns] half clock period
parameter op_clk_period = 1; // [ns] half clock period
parameter config_period =  2 * prog_clk_period; // [ns] One full clock period
parameter operating_period =  SIZE_IN * 2 * op_clk_period; // [ns] One full clock period

// Ports
wire [0:SIZE_IN-1] lb_in;
wire [0:SIZE_IN-1] lb_out;
wire lb_clk;
wire [0:SIZE_RESERV_BLWL-1] reserv_bl;
wire [0:SIZE_RESERV_BLWL-1] reserv_wl;
wire [0:SIZE_BLWL-1] bl;
wire [0:SIZE_BLWL-1] wl;
wire prog_EN;
wire prog_ENb;
wire zin;
wire nequalize;
wire read;
wire clk;
wire Reset;
wire Set; 
// Clocks
wire prog_clock;
wire op_clock;

// Registered port
reg [0:SIZE_IN-1] lb_in_reg;
reg [0:SIZE_RESERV_BLWL-1] reserv_bl_reg;
reg [0:SIZE_RESERV_BLWL-1] reserv_wl_reg;
reg [0:SIZE_BLWL-1] bl_reg;
reg [0:SIZE_BLWL-1] wl_reg;
reg prog_clock_reg;
reg op_clock_reg;

// Config done signal;
reg config_done;
// Temp register for rotating shift
reg temp;

// Unit under test
grid_1__1_ U0 (
zin,
nequalize,
read,
clk,
Reset,
Set,
prog_ENb,
prog_EN,
// Top inputs
lb_in[0], lb_in[4], lb_in[8], lb_in[12], lb_in[16], 
lb_in[20], lb_in[24], lb_in[28], lb_in[32], lb_in[36], 
// Top outputs
lb_out[0], lb_out[4], lb_out[8], 
// Right inputs
lb_in[1], lb_in[5], lb_in[9], lb_in[13], lb_in[17], 
lb_in[21], lb_in[25], lb_in[29], lb_in[33], lb_in[37], 
// Right outputs
lb_out[1], lb_out[5], lb_out[9], 
// Bottom inputs
lb_in[2], lb_in[6], lb_in[10], lb_in[14], lb_in[18], 
lb_in[22], lb_in[26], lb_in[30], lb_in[34], lb_in[38], 
// Bottom outputs
lb_out[2], lb_out[6], 
// Bottom inputs
lb_clk, 
// left inputs
lb_in[3], lb_in[7], lb_in[11], lb_in[15], lb_in[19], 
lb_in[23], lb_in[27], lb_in[31], lb_in[35], lb_in[39], 
// left outputs
lb_out[3], lb_out[7],  
reserv_bl, reserv_wl,
bl, wl 
);

// Task: assign BL and WL values 
task prog_lb_blwl;
  begin
    @(posedge prog_clock);
    // Rotate left shift
    temp = reserv_bl_reg[SIZE_RESERV_BLWL-1];
    //bl_reg = bl_reg >> 1;
    reserv_bl_reg[1:SIZE_RESERV_BLWL-1] = reserv_bl_reg[0:SIZE_RESERV_BLWL-2]; 
    reserv_bl_reg[0] = temp;
  end 
endtask

// Task: assign inputs
task op_lb_in;
  begin
    @(posedge op_clock);
    temp = lb_in_reg[SIZE_IN-1];
    lb_in_reg[1:SIZE_IN-1] = lb_in_reg[0:SIZE_IN-2];
    lb_in_reg[0] = temp;
  end 
endtask

// Configuration done signal
initial
begin
  config_done = 1'b0;
end
// Enabled during config_period, Disabled during op_period 
always
begin
  #config_period config_done = ~config_done;
  #operating_period config_done = ~config_done;
end

// Programming clocks
initial 
begin
  prog_clock_reg = 1'b0;
end
always
begin
  #prog_clk_period prog_clock_reg = ~prog_clock_reg; 
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
assign prog_clock = prog_clock_reg & (~config_done);
assign op_clock = op_clock_reg & config_done;

// Programming Enable signals
assign prog_EN = prog_clock & (~config_done);
assign prog_ENb = ~prog_EN;

// Programming phase: BL/WL 
initial
begin
  // Initialize BL/WL registers
  reserv_bl_reg = {SIZE_RESERV_BLWL {1'b0}}; 
  reserv_bl_reg[0] = 1'b1;
  reserv_wl_reg = {SIZE_RESERV_BLWL {1'b0}}; 
  // Reserved BL/WL
  bl_reg = {SIZE_BLWL {1'b0}};
  wl_reg = {SIZE_BLWL {1'b1}};
  //wl_reg[SIZE_BLWL-1] = 1'b1;
end
always wait (~config_done) // Only invoked when config_done is 0
begin
  // Propagate input 1 to the output
  // BL[0] = 1, WL[4] = 1
  prog_lb_blwl;
end

// Operating Phase
initial
begin
  lb_in_reg = {SIZE_IN {1'b0}}; 
  lb_in_reg[0] = 1'b1; // Last bit is 1 initially
end
always wait (config_done) // Only invoked when config_done is 1
begin
  /* Update inputs */
  op_lb_in;
end

// Wire ports
assign lb_in = lb_in_reg;
assign reserv_bl = reserv_bl_reg;
assign reserv_wl = reserv_wl_reg;
assign bl = bl_reg;
assign wl = wl_reg;

// Constant ports
assign zin = 1'b0;
assign nequalize = 1'b1;
assign read = 1'b0;
assign clk = op_clock;
assign Reset = ~config_done;
assign Set = 1'b0; 

endmodule
