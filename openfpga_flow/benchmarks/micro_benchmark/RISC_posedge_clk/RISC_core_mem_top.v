module RISC_core_mem_top(Reg_R0_out,Reg_R1_out,Reg_R2_out,Reg_R3_out,bus_1_out,clk,rst);

output	[7:0]bus_1_out;
input	clk,rst;
output	[7:0]Reg_R0_out;
output	[7:0]Reg_R1_out;
output	[7:0]Reg_R2_out;
output	[7:0]Reg_R3_out;

wire	[7:0]bus_1_out,MEMAddress;
wire	clk,rst;
wire	[7:0]MEMdataout;
wire	[7:0]Reg_R0_out;
wire	[7:0]Reg_R1_out;
wire	[7:0]Reg_R2_out;
wire	[7:0]Reg_R3_out;


RISC_core_top core(Reg_R0_out,Reg_R1_out,Reg_R2_out,Reg_R3_out,bus_1_out,clk,rst,MEMdataout,MEMAddress);

Memory MEM(MEMdataout,MEMAddress);

endmodule