module counter_tb;

  reg clk, resetb;
  wire [127:0] result;

  counter DUT(
    .clk(clk), 
    .resetb(resetb), 
    .result(result)
  );
  
  initial begin
    #0 reset = 1'b0; clk = 1'b0;
    #100 reset = 1'b1;
  end

  always begin
    #10 clk = ~clk;
  end
  
  initial begin
    #5000 $stop;
  end

endmodule
