//-------------------------------------------------------
//  Functionality: Two 8-bit multiply and A 32-bit dual port RAM
//  Author:        Tarachand Pagarani
//-------------------------------------------------------

module two_mult8_ram32(a0, b0, a1, b1, clk, waddr, raddr, out);
input [7:0] a0, b0;
input [7:0] a1, b1;
input clk;
input [8:0] waddr;
input [8:0] raddr;
output [31:0] out;

wire [15:0] mult_y0;
wire [15:0] mult_y1;
wire [31:0] mult_y;
wire wen;
wire ren;

assign wen = 1'b1;
assign ren = 1'b1;

assign mult_y[31:16] = mult_y1;
assign mult_y[15:0] = mult_y0;
assign mult_y0 = a0 * b0;
assign mult_y1 = a1 * b1;

dual_port_ram_16k RAM0 (
  .clk(clk),
  .wen(wen),
  .ren(ren),
  .waddr(waddr),
  .raddr(raddr),
  .din(mult_y),
  .dout(out)
);

endmodule
