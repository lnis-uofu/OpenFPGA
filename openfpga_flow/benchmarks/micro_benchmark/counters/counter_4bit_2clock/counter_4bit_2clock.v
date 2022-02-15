module counter_4bit_2clock(clk0, rst0, clk1, rst1, q0, q1);

    input clk0;
    input rst0;
    output [3:0] q0;
    reg [3:0] q0;

    input clk1;
    input rst1;
    output [3:0] q1;
    reg [3:0] q1;

    initial begin
      q0 <= 0;
      q1 <= 0;
    end

    always @ (posedge clk0)
    begin
        if(rst0)
		q0 <= 4'b0000;
	else
		q0 <= q0 + 1;
    end

    always @ (posedge clk1)
    begin
        if(rst1)
		q1 <= 4'b0000;
	else
		q1 <= q1 + 1;
    end

endmodule
