//////////////////////////////////////
//                                  //
//    2x2 Test-modes Low density    //
//                                  //
//////////////////////////////////////


module test_mode_low (
	a,
	b,
	clk,
	reset,
	out );

	input wire a;
	input wire b;
	input wire clk;
	input wire reset;
	output wire[3:0] out;

	reg[1:0] pipe_a;
	reg[1:0] pipe_b;
	reg[3:0] pipe_sum;
	wire[7:0] sum;

	assign sum[1:0] = pipe_a[1] + pipe_b[1] + pipe_sum[3];
	assign sum[3:2] = pipe_sum[0] + sum[1] + pipe_sum[2];
	assign sum[5:4] = pipe_sum[1] + sum[3] + pipe_sum[3];
	assign sum[7:6] = pipe_sum[2] + sum[5] + pipe_sum[0];
	assign out = pipe_sum;

	initial begin
		pipe_a <= 2'b00;
		pipe_b <= 2'b00;
		pipe_sum <= 4'b0000;
	end

	always @(posedge clk or posedge reset) begin
		if(reset) begin
			pipe_a <= 2'b00;
			pipe_b <= 2'b00;
			pipe_sum <= 4'b0000;
		end else begin
			pipe_a[0] <= a;
			pipe_a[1] <= pipe_a[0];
			pipe_b[0] <= b;
			pipe_b[1] <= pipe_b[0];
			pipe_sum <= {sum[6], sum[4], sum[2], sum[0]};
		end	
	end

endmodule
