//-----------------------------
// 8-bit multiplier
//-----------------------------
module mult_8(
  input [0:7] A,
  input [0:7] B,
  output [0:15] Y
);

assign Y = A * B;

endmodule

//-----------------------------
// Dual-port RAM 512x32 bit (1Kbit)
// Core logic
//-----------------------------
module dpram_512x32_core (
  input wclk,
  input wen,
  input [0:8] waddr,
  input [0:31] d_in,
  input rclk,
  input ren,
  input [0:8] raddr,
  output [0:31] d_out );

  reg [0:31] ram[0:511];
  reg [0:31] internal;

  assign d_out = internal;

  always @(posedge wclk) begin
    if(wen) begin
      ram[waddr] <= d_in;
    end
  end

  always @(posedge rclk) begin
    if(ren) begin
      internal <= ram[raddr];
    end
  end

endmodule

//-----------------------------
// Dual-port RAM 512x32 bit (1Kbit) wrapper
// where the read clock and write clock
// are combined to a unified clock
//-----------------------------
module dpram_512x32 (
  input clk,
  input wen,
  input ren,
  input [0:8] waddr,
  input [0:8] raddr,
  input [0:31] d_in,
  output [0:31] d_out );

    dpram_512x32_core memory_0 (
      .wclk    (clk),
      .wen    (wen),
      .waddr    (waddr),
      .d_in  (d_in),
      .rclk    (clk),
      .ren    (ren),
      .raddr    (raddr),
      .d_out    (d_out) );

endmodule
