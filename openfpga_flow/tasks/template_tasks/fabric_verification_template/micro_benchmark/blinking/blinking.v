// ------------------------------
// Design Name: Blinking
// Functionality: 1-bit blinking
// ------------------------------
module blinking(
  clk,
  out
);

input clk;
output out;

  always @(posedge clk) begin
    out = ~out;
  end

endmodule
