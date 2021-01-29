// SPRAM 4x1 for implementation in LUT4-RAM
// Asynchronous reading

module spram_4x1 (
	input	 	clk,
	input[1:0]	addr,
	input		d_in,
	input		wr_en,
	output		d_out );

	reg[3:0]	mem;

	assign d_out = mem[addr];

	always @(posedge clk) begin
		if(wr_en) begin
			mem[addr] <= d_in;
		end
	end

endmodule
