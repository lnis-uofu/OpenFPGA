module RISC_core_top(Reg_R0_out,Reg_R1_out,Reg_R2_out,Reg_R3_out,bus_1_out,clk,rst,MEMdataout,MEMAddress);

output	[7:0]bus_1_out,MEMAddress;
input	clk,rst;
input	[7:0]MEMdataout;
output	[7:0]Reg_R0_out;
output	[7:0]Reg_R1_out;
output	[7:0]Reg_R2_out;
output	[7:0]Reg_R3_out;

wire	[7:0]BUS_2,BUS_1,MEMAddress;
wire	[7:0]alu_out;
wire	[7:0]MEMdataout;
wire	[7:0]Reg_Y_out,Reg_R0_out,Reg_R1_out,Reg_R2_out,Reg_R3_out,PC_out;
wire	[7:0]IR_out;
wire	zero_flag_out;
wire	[2:0]Sel_Bus1;
wire	[1:0]Sel_Bus2;
wire	L_R0,L_R1,L_R2,L_R3,L_PC,Inc_PC,L_IR,L_ADD_R,L_R_Y,L_R_Z,MEMwrite,zero;

assign	bus_1_out=BUS_1;
assign	bus_2_out=BUS_2;

Controller CON(L_R0,L_R1,L_R2,L_R3,L_PC,Inc_PC,Sel_Bus1,L_IR,L_ADD_R,L_R_Y,L_R_Z,Sel_Bus2,MEMwrite,zero,IR_out,clk,rst);
//module PC(PC_out,PC_in,load,inc,clk,rst);
PC Program_Counter(PC_out,BUS_2,L_PC,Inc_PC,clk,rst);
//module ALU(zero_flag_out,alu_out,Reg_Y_in,Bus_1_in,IR_code);
ALU	 Arithmetic_Logic_Unit(zero_flag_out,alu_out,Reg_Y_out,BUS_1,IR_out);
//module Memory(Data_out,Data_in,MEMAddress,clk,MEMwrite);
//Memory MEM(MEMdataout,BUS_1,MEMAddress,clk,MEMwrite);
//module Mux_31(Y,A0,A1,A2,sel);
Mux_31	 Mux31(BUS_2,alu_out,BUS_1,MEMdataout,Sel_Bus2);
//module Reg_1bit(Q,D,load,clk,rst);
Reg_1bit Reg_Z(zero,zero_flag_out,L_R_Z,clk,rst);
//module Reg_8bit(Q,D,load,clk,rst);
Reg_8bit Reg_Y(Reg_Y_out,BUS_2,L_R_Y,clk,rst);
Reg_8bit Add_R(MEMAddress,BUS_2,L_ADD_R,clk,rst);
//R0~R3
Reg_8bit Reg_R0(Reg_R0_out,BUS_2,L_R0,clk,rst);
Reg_8bit Reg_R1(Reg_R1_out,BUS_2,L_R1,clk,rst);
Reg_8bit Reg_R2(Reg_R2_out,BUS_2,L_R2,clk,rst);
Reg_8bit Reg_R3(Reg_R3_out,BUS_2,L_R3,clk,rst);
//module Mux_51(Y,A0,A1,A2,A3,A4,sel);
Mux_51	Mux51(BUS_1,Reg_R0_out,Reg_R1_out,Reg_R2_out,Reg_R3_out,PC_out,Sel_Bus1);
//module IR(IR_out,IR_in,load,clk,rst);
IR	Instruction_Register(IR_out,BUS_2,L_IR,clk,rst);

endmodule