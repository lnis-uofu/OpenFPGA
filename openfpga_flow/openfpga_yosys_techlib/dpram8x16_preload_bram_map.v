module $__MY_DPRAM_8x16 (
  output [0:15] B1DATA,
  input CLK1,
  input [0:2] B1ADDR,
  input [0:2] A1ADDR,
  input [0:15] A1DATA,
  input A1EN,
  input B1EN );

  generate
    dpram_8x16 #() _TECHMAP_REPLACE_ (
      .clk    (CLK1),
      .wen    (A1EN),
      .waddr    (A1ADDR),
      .data_in    (A1DATA),
      .ren    (B1EN),
      .raddr    (B1ADDR),
      .data_out    (B1DATA) );
  endgenerate

endmodule
