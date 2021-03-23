
`timescale 1ns/1ns

module function1(
         cmd52_53_func_num,
		 io_en_func1,
		 sram_resetn1,
         sram_wen1,
	     sram_oen1,
         sram_addr1,
	     sram_data_in1,  
		 sram_data_out1, 
		 sram_csn1,
		 sram_onn      	     
             );

    input  [2:0] cmd52_53_func_num;
	input        io_en_func1;
    input        sram_wen1;
    input        sram_oen1;
    input [16:0] sram_addr1;
    input        sram_resetn1;
    input  [7:0] sram_data_in1;
    output [7:0] sram_data_out1;
	output       sram_csn1;
	input        sram_onn;

    wire         sram_csn1;
    wire         reset;
	wire   [7:0] sram_data_in;
	reg    [7:0] sram_data_out;        
	wire  [19:0] addr;
	
    assign    sram_csn1     = ~((cmd52_53_func_num == 3'b001) & io_en_func1) & ~sram_onn;

endmodule






					 


























