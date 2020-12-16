`timescale 1ns/1ns

module RISC_testbench;
   
wire	[7:0]bus_1_out;
reg	clk,rst;
wire	[7:0]Reg_R0_out;
wire	[7:0]Reg_R1_out;
wire	[7:0]Reg_R2_out;
wire	[7:0]Reg_R3_out;

/* wire	[7:0]MEMAddress;
wire	[7:0]MEMdataout;
wire  	MEMwrite; */

/* assign MEMAddress = top.MEMAddress;
assign MEMdataout = top.MEMdataout;
assign MEMwrite   = top.MEMwrite; */

RISC_core_mem_top top(Reg_R0_out,Reg_R1_out,Reg_R2_out,Reg_R3_out,bus_1_out,clk,rst);

 always#20 clk=~clk;
   
 initial
    begin
      clk=0;rst=1;
      #30 rst=0;
      #6000 $stop;       
      end

/* //----------

	integer fp;
	initial
	begin
		fp = $fopen("RISC_xa.vec");
		
		$fdisplay(fp, "radix		1	  1	44	44	44	44	44	1	44	44");
		$fdisplay(fp, "vname	  clk	rst		Reg_R0_out[[7:0]]	Reg_R1_out[[7:0]]	Reg_R2_out[[7:0]]	Reg_R3_out[[7:0]]	bus_1_out[[7:0]]	MEMwrite	MEMAddress	MEMdataout");
		$fdisplay(fp, "   io		i	  i	oo	oo	oo	oo	oo	o	oo	ii");
		$fdisplay(fp, "slope		0.3");
		$fdisplay(fp, "  vih		3.3");
		$fdisplay(fp, "  vil		0");
		$fdisplay(fp, "tunit		ns");
	end
	always@(clk)
	begin
		$fdisplay(fp, "%t %b %b %h %h %h %h %h %b %h %h", $time, clk, rst, Reg_R0_out, Reg_R1_out, Reg_R2_out, Reg_R3_out, bus_1_out, MEMwrite, MEMAddress, MEMdataout);
	end

//---------- */
   
endmodule