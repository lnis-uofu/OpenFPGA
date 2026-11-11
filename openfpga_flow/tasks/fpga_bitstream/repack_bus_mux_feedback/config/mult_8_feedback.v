// Multiplier whose operand A is the low byte of its registered product,
// shifted right by one bit with sign extension
module mult_8_feedback (
  input clk,
  input [7:0] b,
  output reg [15:0] p
);

  wire [7:0] a = {p[7], p[7:1]};

  always @(posedge clk) begin
    p <= a * b;
  end

endmodule
