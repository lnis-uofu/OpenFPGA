module io_reg(clk, in, out);

    input clk;
    input in;
    output out;
    reg out;

    //reg temp;

    always @(posedge clk)
    begin
        out <= in;
    end
    
    /*always @(posedge clk)
    begin
        out <= temp ;
    end*/
    
endmodule


