module io_reg_tb;
    
    reg clk_gen, in_gen;
    wire out;

    io_reg inst(.clk(clk_gen), .in(in_gen), .out(out));

    initial begin
        #0 in_gen = 1'b1; clk_gen = 1'b0;
        #100 in_gen = 1'b0;
    end

    always begin
        #10 clk_gen = ~clk_gen;
    end

    initial begin
        #5000 $stop;
    end

endmodule
