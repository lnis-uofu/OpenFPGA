module Controller(L_R0,L_R1,L_R2,L_R3,L_PC,Inc_PC,
Sel_Bus1,L_IR,L_ADD_R,L_R_Y,L_R_Z,Sel_Bus2,write,
zero,instruction,nclk,rst);


//狀態
parameter	S_idle=0,S_fet1=1,S_fet2=2,S_dec=3,
			S_ex1=4,S_rd1=5,S_rd2=6,S_wr1=7,S_wr2=8,
			S_br1=9,S_br2=10,S_halt=11;
//指令
parameter	NOP=0,ADD=1,SUB=2,AND=3,NOT=4,
			RD=5,WR=6,BR=7,BRZ=8;


output reg	L_R0,L_R1,L_R2,L_R3,L_PC,Inc_PC,
			L_IR,L_ADD_R,L_R_Y,L_R_Z,write;
output	reg[2:0]Sel_Bus1;
output	reg [1:0]Sel_Bus2;

input	zero,nclk,rst;
input	[7:0]instruction;

reg	[15:0]Con_out;
reg	[3:0]PS,NS;
reg	err_flag;

wire [1:0]src=instruction[3:2];
wire [1:0]dest=instruction[1:0];
wire [3:0]opcode=instruction[7:4];

always@(posedge nclk)
	begin
		if(rst==1)PS<=0;
		else PS<=NS;
	end

always@(PS,opcode,src,dest,zero)
	begin
	L_R0=0;
	L_R1=0;
	L_R2=0;
	L_R3=0;
	L_PC=0;
	Inc_PC=0;
	Sel_Bus1=0;
	L_IR=0;
	L_ADD_R=0;
	L_R_Y=0;
	L_R_Z=0;
	Sel_Bus2=0;
	write=0;
	err_flag=0;
		case(PS)
			S_idle:		NS=S_fet1;
			
			S_fet1:		begin
						 NS=S_fet2;
						 Sel_Bus1=3'b100;//Sel_PC
						 Sel_Bus2=2'b01;//Sel_Bus1	
						 L_ADD_R=1;
						end
						
			S_fet2:		begin
						 NS=S_dec;
						 Sel_Bus2=2'b10;//Sel_Mem
						 L_IR=1;
						 Inc_PC=1;
						end
						
			S_dec:		begin
						case(opcode)
							NOP:NS=S_fet1;							
							ADD,SUB,AND:begin
								NS=S_ex1;
								Sel_Bus2=2'b01;//Sel_Bus1
								L_R_Y=1;
								case(src)
									0:		Sel_Bus1=3'b000;//R0
									1:		Sel_Bus1=3'b001;//R1
									2:		Sel_Bus1=3'b010;//R2
									3:		Sel_Bus1=3'b011;//R3
									default	err_flag=1;
								endcase
								end//ADD,SUB,AND
								
							NOT:begin
								NS=S_fet1;
								L_R_Z=1;
								Sel_Bus2=2'b00;//Sel_ALU
								case(src)
									0:		Sel_Bus1=3'b000;//R0
									1:		Sel_Bus1=3'b001;//R1
									2:		Sel_Bus1=3'b010;//R2
									3:		Sel_Bus1=3'b011;//R3
									default	err_flag=1;
								endcase
								case(dest)
									0:		L_R0=1;
									1:		L_R1=1;
									2:		L_R2=1;
									3:		L_R3=1;
									default	err_flag=1;
								endcase		
								end//NOT
								
							RD:	begin
								NS=S_rd1;
								Sel_Bus1=3'b100;//Sel_PC
								Sel_Bus2=3'b001;//Sel_Bus1
								L_ADD_R=1;
								end//RD
							
							WR:	begin
								NS=S_wr1;
								Sel_Bus1=3'b100;//Sel_PC
								Sel_Bus2=3'b001;//Sel_Bus1
								L_ADD_R=1;
								end//WR

							BR: begin
								NS=S_br1;
								Sel_Bus1=3'b100;//Sel_PC
								Sel_Bus2=3'b001;//Sel_Bus1
								L_ADD_R=1;
								end//BR
							
							BRZ:begin
								 if(zero==1)begin
									NS=S_br1;
									Sel_Bus1=3'b100;//Sel_PC
									Sel_Bus2=3'b001;//Sel_Bus1
									L_ADD_R=1;
								 end
								 else begin
									NS=S_fet1;
									Inc_PC=1;
								 end
								end//BRZ
								 
							default	NS=S_halt;
				 
						endcase//opcode
						end
				
			S_ex1:		begin
						 NS=S_fet1;
						 L_R_Z=1;
						 Sel_Bus2=2'b00;//Sel_ALU
						 case(dest)
							0:	begin Sel_Bus1=3'b000;L_R0=1;end
							1:	begin Sel_Bus1=3'b001;L_R1=1;end
							2:	begin Sel_Bus1=3'b010;L_R2=1;end
							3:	begin Sel_Bus1=3'b011;L_R3=1;end
							default	err_flag=1;
						 endcase		
						end	
			
			S_rd1:		begin
						 NS=S_rd2;						 						 
						 Inc_PC=1;
						 Sel_Bus2=2'b10;//Sel_Mem
						 L_ADD_R=1;
						end
						
			S_wr1:		begin
						 NS=S_wr2;
						 Inc_PC=1;
						 Sel_Bus2=2'b10;//Sel_Mem
						 L_ADD_R=1;
						end
						
			S_rd2:		begin
						 NS=S_fet1;
						 Sel_Bus2=2'b10;//Sel_Mem
						 case(dest)
							0:		L_R0=1;
							1:		L_R1=1;
							2:		L_R2=1;
							3:		L_R3=1;
							default	err_flag=1;
						 endcase
						end
						
			S_wr2:		begin
						 NS=S_fet1;
						 write=1;
						 case(src)
							0:		Sel_Bus1=3'b000;//R0
							1:		Sel_Bus1=3'b001;//R1
							2:		Sel_Bus1=3'b010;//R2
							3:		Sel_Bus1=3'b011;//R3
							default	err_flag=1;
						 endcase	
						end
			
			S_br1:		begin
						 NS=S_br2;
						 Sel_Bus2=2'b10;//Sel_Mem	
						 L_ADD_R=1;
						end
						
			S_br2:		begin	
						 NS=S_fet1;
						 Sel_Bus2=2'b10;//Sel_Mem 
						 L_PC=1;
						end
			S_halt:		NS=S_halt;
			default		NS=S_idle;
		endcase								 
	end
endmodule
