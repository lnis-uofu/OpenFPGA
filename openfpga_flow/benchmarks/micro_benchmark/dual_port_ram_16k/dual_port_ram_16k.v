//-----------------------------------------------------
// Design Name : dual_port_ram_16k
// File Name   : dual_port_ram_16k.v
// Function    : Dual port RAM 2048x8bit
// Coder       : Xifan Tang
//-----------------------------------------------------

module dual_port_ram_16k (
  input clk,
  input wen,
  input ren,
  input [10:0] waddr,
  input [10:0] raddr,
  input [7:0] din,
  output [7:0] dout
);

    dual_port_sram_16kb memory_0 (
      .wclk    (clk),
      .wen    (wen),
      .waddr    (waddr),
      .data_in  (din),
      .rclk    (clk),
      .ren    (ren),
      .raddr    (raddr),
      .data_out    (dout) );

endmodule

module dual_port_sram_16kb (
  input wclk,
  input wen,
  input [10:0] waddr,
  input [7:0] data_in,
  input rclk,
  input ren,
  input [10:0] raddr,
  output [7:0] data_out
);

  reg [7:0] ram[2047:0];
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
