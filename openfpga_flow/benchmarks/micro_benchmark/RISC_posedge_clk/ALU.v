module ALU(zero_flag_out,alu_out,Reg_Y_in,Bus_1_in,IR_code);

output	zero_flag_out;
output	reg	[7:0]alu_out;
input	[7:0]Reg_Y_in,Bus_1_in;
input	[7:0]IR_code;

wire	[3:0]opcode=IR_code[7:4];



always@(*)
	begin
		case(opcode)
		1:	alu_out=Reg_Y_in+Bus_1_in;
		2:	alu_out=Bus_1_in+~(Reg_Y_in)+1;
		3:	alu_out=Reg_Y_in&(Bus_1_in);
		4:  alu_out=~(Bus_1_in);
		default:alu_out=8'b0;
		endcase
	end

assign zero_flag_out=~|alu_out;

endmodule

	