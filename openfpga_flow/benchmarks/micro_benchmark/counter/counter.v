module counter_original(clk_counter, q_counter, rst_counter);

    input clk_counter;
    input rst_counter;
    output [7:0] q_counter;
    reg [7:0] q_counter;

    always @ (posedge clk_counter)
    begin
        if(rst_counter)
		q_counter <= 8'b00000000;
	else
		q_counter <= q_counter + 1;
    end

endmodule
