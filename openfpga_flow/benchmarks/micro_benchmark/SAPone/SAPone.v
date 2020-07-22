module SAPone(
	output wire [7:0] SAP_out,
	output wire [11:0] con,
	output reg [7:0] bus,
	input clk,
	input clr_
	);
	
	wire cp, ep, lm_, ce_, li_, ei_, la_, ea, su, eu, lb_, lo_;
	wire [7:0] acc_out2, BRegister_out, OutputRegister_out;
	wire [3:0] IR_out, mar_out;
	wire [4:0] bus_sel;
	wire [3:0] pc_out, oprand;
	wire [7:0] rom_out, acc_out1, ADDSUB_out;
	
	assign {cp, ep, lm_, ce_, li_, ei_, la_, ea, su, eu, lb_, lo_} = con;
	assign bus_sel = {ep, ce_, ei_, ea, eu};
	
	always@(*)
	begin
		case(bus_sel)
			5'b11100: bus[3:0] = pc_out;
			5'b00100: bus[7:0] = rom_out;
			5'b01000: bus[3:0] = oprand;
			5'b01110: bus[7:0] = acc_out1;
			5'b01101: bus[7:0] = ADDSUB_out;
			default:  bus[7:0] = 8'bx;
		endcase
	end
	
	PC pc1(
		.pc_out(pc_out),
		.cp(cp),
		.clk(clk),
		.clr_(clr_)
		);
		
	MAR mar1(
		.mar_out(mar_out),
		.mar_in(bus[3:0]),
		.lm_(lm_),
		.clk(clk),
		.clr_(clr_)
		);
		
	ROM roml(
		.rom_out(rom_out),
		.rom_in(mar_out)
		);
	
	IR ir1(
		.opcode(IR_out),
		.oprand(oprand),
		.IR_in(bus[7:0]),
		.li_(li_),
		.clk(clk),
		.clr_(clr_)
		);
	
	Controller cont1(
		.control_signals(con),
		.opcode(IR_out),
		.clk(clk),
		.clr_(clr_)
		); 

	ACC acc1(
		.acc_out1(acc_out1),
		.acc_out2(acc_out2),
		.acc_in(bus[7:0]),
		.la_(la_),
		.clk(clk),
		.clr_(clr_)
		);
			
	ADDSUB addsub1(
		.ADDSUB_out(ADDSUB_out),
		.ADDSUB_in1(acc_out2),
		.ADDSUB_in2(BRegister_out),
		.su(su)
		);
		
	BRegister bregister1(
		.BRegister_out(BRegister_out),
		.BRegister_in(bus[7:0]),
		.lb_(lb_),
		.clk(clk),
		.clr_(clr_)
		);
		
	OutputRegister outputregister1(
		.OutputRegister_out(SAP_out),
		.OutputRegister_in(bus[7:0]),
		.lo_(lo_),
		.clk(clk),
		.clr_(clr_)
		);			


endmodule
