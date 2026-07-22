module $__MY_DPRAM_8x32 #(
  parameter INIT = 256'bx // Yosys passes the array initialization here
)(
  output [0:31] B1DATA,
  input CLK1,
  input [0:2] B1ADDR,
  input [0:2] A1ADDR,
  input [0:31] A1DATA,
  input A1EN,
  input B1EN );

  generate
    dpram_8x32 #(
      .INIT(INIT) // Pass it directly
    ) _TECHMAP_REPLACE_ (
      .clk    (CLK1),
      .wen    (A1EN),
      .waddr    (A1ADDR),
      .data_in    (A1DATA),
      .ren    (B1EN),
      .raddr    (B1ADDR),
      .data_out    (B1DATA) );
  endgenerate

endmodule
