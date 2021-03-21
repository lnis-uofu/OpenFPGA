//-----------------------------
// Dual-port RAM 1024x8 bit (8Kbit)
// Core logic
//-----------------------------
module dpram_1024x8_core (
  input wclk,
  input wen,
  input [9:0] waddr,
  input [7:0] data_in,
  input rclk,
  input ren,
  input [9:0] raddr,
  output [7:0] data_out );

  reg [7:0] ram[1023:0];
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

//-----------------------------
// Dual-port RAM 1024x8 bit (8Kbit) wrapper
// where the read clock and write clock
// are combined to a unified clock
//-----------------------------
module dpram_1024x8 (
  input clk,
  input wen,
  input ren,
  input [9:0] waddr,
  input [9:0] raddr,
  input [7:0] data_in,
  output [7:0] data_out );

    dpram_1024x8_core memory_0 (
      .wclk    (clk),
      .wen    (wen),
      .waddr    (waddr),
      .data_in  (data_in),
      .rclk    (clk),
      .ren    (ren),
      .raddr    (raddr),
      .data_out    (data_out) );

endmodule

//-----------------------------
// 36-bit multiplier
//-----------------------------
module mult_36(
  input [35:0] A,
  input [35:0] B,
  output [71:0] Y
);

assign Y = A * B;

endmodule

