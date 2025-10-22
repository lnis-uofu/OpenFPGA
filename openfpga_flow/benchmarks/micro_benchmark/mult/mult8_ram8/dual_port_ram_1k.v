//-----------------------------------------------------
// Design Name : dual_port_ram_1k
// File Name   : dual_port_ram_1k.v
// Function    : Dual port RAM 128x8bit
// Coder       : Xifan Tang
//-----------------------------------------------------

module dual_port_ram_1k (
  input clk,
  input wen,
  input ren,
  input [6:0] waddr,
  input [6:0] raddr,
  input [7:0] din,
  output [7:0] dout
);

    dual_port_sram_1kb memory_0 (
      .wclk    (clk),
      .wen    (wen),
      .waddr    (waddr),
      .data_in  (din),
      .rclk    (clk),
      .ren    (ren),
      .raddr    (raddr),
      .data_out    (dout) );

endmodule

module dual_port_sram_1kb (
  input wclk,
  input wen,
  input [6:0] waddr,
  input [7:0] data_in,
  input rclk,
  input ren,
  input [6:0] raddr,
  output [7:0] data_out
);

  reg [7:0] ram[127:0];
  reg [7:0] internal;

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
