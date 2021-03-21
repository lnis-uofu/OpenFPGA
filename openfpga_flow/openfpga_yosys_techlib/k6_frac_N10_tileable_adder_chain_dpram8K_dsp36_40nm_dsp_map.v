module mult_36x36 (
  input [35:0] A,
  input [35:0] B,
  output [71:0] Y
);
  parameter A_SIGNED = 0;
  parameter B_SIGNED = 0;
  parameter A_WIDTH = 0;
  parameter B_WIDTH = 0;
  parameter Y_WIDTH = 0;

  mult_36 #() _TECHMAP_REPLACE_ (
    .A    (A),
    .B    (B),
    .Y    (Y) );

endmodule
