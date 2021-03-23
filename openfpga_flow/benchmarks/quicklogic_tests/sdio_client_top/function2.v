
`timescale 1ns/1ns

module function2(
         cmd52_53_func_num,
		 io_en_func2,
		 sram_resetn2,
         sram_wen2,
	     sram_oen2,
         sram_addr2,
	     sram_data_in2,  
		 sram_data_out2, 
		 sram_csn2,
		 sram_onn,
		 
		 sram_nor_flash_ry_byn      	     
             );

    input  [2:0] cmd52_53_func_num;
	input        io_en_func2;
    input        sram_wen2;
    input        sram_oen2;
    input [16:0] sram_addr2;
    input        sram_resetn2;
    input  [7:0] sram_data_in2;
    output [7:0] sram_data_out2;
	output       sram_csn2;
	input        sram_onn;
	input        sram_nor_flash_ry_byn;

    wire         sram_csn2;
    wire         reset;
	wire   [7:0] sram_data_in;
	reg    [7:0] sram_data_out;        
	wire  [19:0] addr;
	
    assign    sram_csn2      = ~((cmd52_53_func_num == 3'b010) & io_en_func2) & ~sram_onn;

    assign    sram_data_out2 = {7'b0, sram_nor_flash_ry_byn};

endmodule






					 


























