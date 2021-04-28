//-----------------------------------------------------
// Design Name : dpram_2048x8
// File Name   : dpram_2048x8.v
// Function    : Dual port RAM 2048 x 8bit 
// Coder       : Xifan Tang
//-----------------------------------------------------

module dpram_2048x8 (
  input clk,
  input wen,
  input ren,
  input[0:10] waddr,
  input[0:10] raddr,
  input[0:7] data_in,
  output[0:7] data_out );

    dual_port_sram memory_0 (
      .wclk    (clk),
      .wen    (wen),
      .waddr    (waddr),
      .data_in  (data_in),
      .rclk    (clk),
      .ren    (ren),
      .raddr    (raddr),
      .data_out    (data_out) );

endmodule

//-----------------------------------------------------
// Design Name : dpram_2048x8_core
// File Name   : dpram_2048x8.v
// Function    : Core module of dual port RAM 2048 addresses x 8 bit
// Coder       : Xifan tang
//-----------------------------------------------------
module dual_port_sram (
  input wclk,
  input wen,
  input[0:10] waddr,
  input[0:7] data_in,
  input rclk,
  input ren,
  input[0:10] raddr,
  output[0:7] data_out );

  reg[0:7] ram[0:2047];
  reg[0:7] internal;

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
