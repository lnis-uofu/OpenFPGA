//-------------------------------------------------------
//  Functionality: A 8-bit multiply circuit using macro
//  Author:        Tarachand Pagarani
//-------------------------------------------------------

module mult8_ram8(a, b, clk, waddr, raddr, out);
input [7:0] a, b;
input clk;
input [6:0] waddr;
input [6:0] raddr;
output [15:0] out;

wire [15:0] mult_y;
wire wen;
wire ren;

assign wen = 1'b1;
assign ren = 1'b1;

assign mult_y = a * b;

dual_port_ram_1k RAM0 (
  .clk(clk),
  .wen(wen),
  .ren(ren),
  .waddr(waddr),
  .raddr(raddr),
  .din(mult_y[7:0]),
  .dout(out[7:0])
);

dual_port_ram_1k RAM1 (
  .clk(clk),
  .wen(wen),
  .ren(ren),
  .waddr(waddr),
  .raddr(raddr),
  .din(mult_y[15:8]),
  .dout(out[15:8])
);

endmodule
