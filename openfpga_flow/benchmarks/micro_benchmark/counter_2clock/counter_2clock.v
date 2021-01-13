module counter_2clock(clk0, q0, rst0, clk1, q1, rst1);

    input clk0;
    input rst0;
    output [7:0] q0;
    reg [7:0] q0;

    input clk1;
    input rst1;
    output [7:0] q1;
    reg [7:0] q1;

    always @ (posedge clk0)
    begin
        if(rst0)
		q0 <= 8'b00000000;
	else
		q0 <= q0 + 1;
    end

    always @ (posedge clk1)
    begin
        if(rst1)
		q1 <= 8'b00000000;
	else
		q1 <= q1 + 1;
    end

endmodule
