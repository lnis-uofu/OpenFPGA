// Benchmark "s298.bench" written by ABC on Tue Mar  5 10:03:54 2019

module s298 ( clk, 
    G0, G1, G2,
    G117, G132, G66, G118, G133, G67  );
  input  G0, G1, G2, clk;
  output G117, G132, G66, G118, G133, G67;
  reg G10, G11, G12, G13, G14, G15, G16, G17, G18, G19, G20, G21, G22, G23;
  wire n57, n59, n64, n66, n21_1, n26_1, n31_1, n36_1, n41_1, n46_1, n51_1,
    n56_1, n61_1, n66_2, n71_1, n76_1, n81_1, n86_1;
  assign n21_1 = ~G0 & ~G10;
  assign n26_1 = ~G0 & (G10 ? (~G11 & (G12 | ~G13)) : G11);
  assign n31_1 = ~G0 & ((G12 & (~G10 | ~G11)) | (G10 & G11 & ~G12));
  assign n36_1 = ~G0 & ((G11 & ((~G12 & G13) | (G10 & G12 & ~G13))) | (G13 & (~G10 | (~G11 & G12))));
  assign n41_1 = ~G0 & (G14 ^ (G23 | (G10 & G13 & n57)));
  assign n57 = ~G11 & ~G12;
  assign n46_1 = ~G0 & ~n59;
  assign n59 = (G11 & (~G15 | (~G12 & G13 & ~G14 & ~G22))) | (~G15 & (G12 | ~G13 | G14 | ~G22));
  assign n51_1 = n59 & ((G13 & (~G14 | G16)) | (G12 & G14 & G16));
  assign n56_1 = n59 & ((~G13 & (G11 ? ~G12 : ~G14)) | (G14 & G17 & (G12 | G13)));
  assign n61_1 = n59 & ((G14 & G18 & (G12 | G13)) | (~G13 & (~G14 | (G11 & ~G12))));
  assign n66_2 = n59 ? n64 : ~G10;
  assign n64 = (G13 & (~G14 | G19)) | (G14 & ((~G11 & ~G12 & ~G13) | (G12 & G19)));
  assign n71_1 = n59 ? (n66 & (G20 | (~G12 & ~G13))) : ~G10;
  assign n66 = G14 & (~G11 | G12 | G13);
  assign n76_1 = n59 & ((G12 & ((G11 & ~G13 & ~G14) | (G14 & G21))) | (G13 & G14 & G21));
  assign n81_1 = ~G0 & (G2 ^ G22);
  assign n86_1 = ~G0 & (G1 ^ G23);
  assign G117 = G18;
  assign G132 = G20;
  assign G66 = G16;
  assign G118 = G19;
  assign G133 = G21;
  assign G67 = G17;
  always @ (posedge clk) begin
    G10 <= n21_1;
    G11 <= n26_1;
    G12 <= n31_1;
    G13 <= n36_1;
    G14 <= n41_1;
    G15 <= n46_1;
    G16 <= n51_1;
    G17 <= n56_1;
    G18 <= n61_1;
    G19 <= n66_2;
    G20 <= n71_1;
    G21 <= n76_1;
    G22 <= n81_1;
    G23 <= n86_1;
  end
  initial begin
    G10 <= 1'b0;
    G11 <= 1'b0;
    G12 <= 1'b0;
    G13 <= 1'b0;
    G14 <= 1'b0;
    G15 <= 1'b0;
    G16 <= 1'b0;
    G17 <= 1'b0;
    G18 <= 1'b0;
    G19 <= 1'b0;
    G20 <= 1'b0;
    G21 <= 1'b0;
    G22 <= 1'b0;
    G23 <= 1'b0;
  end
endmodule


