module counter(clk, q, rst);

    input clk;
    input rst;
    output [7:0] q;
    reg [7:0] q;

    always @ (posedge clk)
    begin
        if(rst)
			q <= 8'b00000000;
	else
		q <= q + 1;
    end

endmodule
