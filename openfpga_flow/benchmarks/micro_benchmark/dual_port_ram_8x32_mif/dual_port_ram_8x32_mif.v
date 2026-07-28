//-----------------------------------------------------
// Design Name : dual_port_ram_8x32
// File Name   : dual_port_ram_8x32.v
// Function    : Dual port RAM 8x32bit with memory initialization file (MIF)
// Coder       : Xifan Tang
//-----------------------------------------------------

module dual_port_ram_8x32 #(
  parameter MEM_FILE = "init.hex" // provide a name of MIF file in HEX format
)(
  input clk,
  input wen,
  input ren,
  input [2:0] waddr,
  input [2:0] raddr,
  input [31:0] din,
  output [31:0] dout
);

    dual_port_sram_8x32_core #(
      .MEM_FILE(MEM_FILE)
    ) memory_0 (
      .wclk    (clk),
      .wen    (wen),
      .waddr    (waddr),
      .data_in  (din),
      .rclk    (clk),
      .ren    (ren),
      .raddr    (raddr),
      .data_out    (dout) );

endmodule

module dual_port_sram_8x32_core #(
  parameter MEM_FILE = "init.hex" // provide a name of MIF file in HEX format
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
