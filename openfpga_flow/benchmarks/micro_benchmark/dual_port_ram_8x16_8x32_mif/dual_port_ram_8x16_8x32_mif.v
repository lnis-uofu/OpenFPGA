//-----------------------------------------------------
// Design Name : dual_port_ram_8x32
// File Name   : dual_port_ram_8x32.v
// Function    : Dual port RAM 8x32bit with memory initialization file (MIF)
// Coder       : Xifan Tang
//-----------------------------------------------------

module dual_port_ram_8x16_8x32 #(
  parameter MEM8x16_FILE = "init_8x16.hex", // provide a name of MIF file in HEX format
  parameter MEM8x32_FILE = "init_8x32.hex" // provide a name of MIF file in HEX format
)(
  input clk,
  input wen,
  input ren,
  input [2:0] waddr8x16,
  input [2:0] raddr8x16,
  input [15:0] din8x16,
  output [15:0] dout8x16,
  input [2:0] waddr8x32,
  input [2:0] raddr8x32,
  input [31:0] din8x32,
  output [31:0] dout8x32

);

    dual_port_sram_8x16_core #(
      .MEM_FILE(MEM8x16_FILE)
    ) memory_0 (
      .wclk    (clk),
      .wen    (wen),
      .waddr    (waddr8x16),
      .data_in  (din8x16),
      .rclk    (clk),
      .ren    (ren),
      .raddr    (raddr8x16),
      .data_out    (dout8x16) );


    dual_port_sram_8x32_core #(
      .MEM_FILE(MEM8x32_FILE)
    ) memory_1 (
      .wclk    (clk),
      .wen    (wen),
      .waddr    (waddr8x32),
      .data_in  (din8x32),
      .rclk    (clk),
      .ren    (ren),
      .raddr    (raddr8x32),
      .data_out    (dout8x32) );

endmodule

module dual_port_sram_8x32_core #(
  parameter MEM_FILE = "init_8x32.hex" // provide a name of MIF file in HEX format
)(
  input wclk,
  input wen,
  input [2:0] waddr,
  input [31:0] data_in,
  input rclk,
  input ren,
  input [2:0] raddr,
  output [31:0] data_out
);

  reg [31:0] ram[2:0];
  reg [31:0] internal;

  initial $readmemh(MEM_FILE, ram);

  assign data_out = internal;

  always @(posedge wclk) begin
    if(wen) begin
      ram[waddr] <= data_in;
    end
  end

  always @(posedge rclk) begin
    if(ren) begin
      internal <= ram[raddr];
    end
  end

endmodule

module dual_port_sram_8x16_core #(
  parameter MEM_FILE = "init_8x16.hex" // provide a name of MIF file in HEX format
)(
  input wclk,
  input wen,
  input [2:0] waddr,
  input [15:0] data_in,
  input rclk,
  input ren,
  input [2:0] raddr,
  output [15:0] data_out
);

  reg [15:0] ram[7:0];
  reg [15:0] internal;

  initial $readmemh(MEM_FILE, ram);

  assign data_out = internal;

  always @(posedge wclk) begin
    if(wen) begin
      ram[waddr] <= data_in;
    end
  end

  always @(posedge rclk) begin
    if(ren) begin
      internal <= ram[raddr];
    end
  end

endmodule
