module Memory(Data_out,Address);

	output	[7:0]Data_out;
	input	[7:0]Address;
	
	reg	[7:0]mem[255:0];
	assign	Data_out=mem[Address];
	
	always@(Address)
	begin
		case(Address)
							//opcode_src_dest
		//NOP
		0:	mem[Address]=8'b0000_00_00;
		
		//rd 00 10		//Read MEM[130] to R2
		1:	mem[Address]=8'b0101_00_10; //Instruction 
		2:	mem[Address]=130;	//Address
		
		//rd 00 11		//Read MEM[131] to R3
		3:	mem[Address]=8'b0101_00_11; //Instruction 
		4:	mem[Address]=131; //Address
		
		//rd 00 01		//Read MEM[128] to R1
		5:	mem[Address]=8'b0101_00_01; //Instruction
		6:	mem[Address]=128; //Address

		//rd 00 00		//Read MEM[129] to R0		
		7:	mem[Address]=8'b0101_00_00; //Instruction
		8:	mem[Address]=129; //Address
		
		//Sub 00 01		//Sub R1-R0 to R1
		9:	mem[Address]=8'b0010_00_01; //Instruction
		
		//BRZ 00 00    
		10:	mem[Address]=8'b1000_00_00; //Instruction
		11:	mem[Address]=134; //Address
		
		//Add 10 11		//Add R2+R3 to R3
		12:	mem[Address]=8'b00011011;
		
		//BR
		13:	mem[Address]=8'b01110011; //Instruction
		14:	mem[Address]=140; //Address
		
		128:mem[Address]=6;
		129:mem[Address]=1;
		130:mem[Address]=2;
		131:mem[Address]=0;
		134:mem[Address]=139; //Address
		135:mem[Address]=0;
		//HAL
		139:mem[Address]=8'b1111_00_00; //Instruction
		140:mem[Address]=9; //Address
		default mem[Address]=8'bx;
    endcase
  end
	
endmodule
	
	