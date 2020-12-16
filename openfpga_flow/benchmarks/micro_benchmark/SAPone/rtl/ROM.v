module ROM(
	output reg [7:0] rom_out,
	input [3:0] rom_in
	);
	
	always @(*)
	begin
		rom_out = 8'bx;
		case(rom_in)
			4'b0000: rom_out = 8'b0000_1001; //LDA
			4'b0001: rom_out = 8'b0001_1010; //ADD
			4'b0010: rom_out = 8'b0001_1011; //ADD
			4'b0011: rom_out = 8'b0010_1100; //SUB
			4'b0100: rom_out = 8'b1110_xxxx; //OUT
			4'b0101: rom_out = 8'b1111_xxxx; //HLT
			4'b0110: rom_out = 8'bxxxx_xxxx;
			4'b0111: rom_out = 8'bxxxx_xxxx;
			4'b1000: rom_out = 8'bxxxx_xxxx;
			4'b1001: rom_out = 8'b0001_0000;
			4'b1010: rom_out = 8'b0001_0100;
			4'b1011: rom_out = 8'b0001_1000;
			4'b1100: rom_out = 8'b0010_0000;	
		endcase
	end
	
endmodule
