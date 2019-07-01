//==================================================================
//   >>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<
//   ------------------------------------------------------------------
//   Copyright (c) 2014 by Lattice Semiconductor Corporation
// 					ALL RIGHTS RESERVED 
//   ------------------------------------------------------------------
//
//   Permission:
//
//      Lattice SG Pte. Ltd. grants permission to use this code for use
//   in synthesis for any Lattice programmable logic product.  Other
//   use of this code, including the selling or duplication of any
//   portion is strictly prohibited.
  
//
//   Disclaimer:
//
//   This VHDL or Verilog source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Lattice provides no warranty
//   regarding the use or functionality of this code.
//
//   --------------------------------------------------------------------
//
//                  Lattice SG Pte. Ltd.
//                  101 Thomson Road, United Square #07-02 
// 					Singapore 307591
//	
//
//                  TEL: 1-800-Lattice (USA and Canada)
//	+65-6631-2000 (Singapore)
//                       +1-503-268-8001 (other locations)
//
//                  web: http://www.latticesemi.com/
//                  email: techsupport@latticesemi.com
//
//   --------------------------------------------------------------------
//


module user_logic_control_reg_data_buf
(	
///i2c signal/////
	clk,
	rst,
	scl,
	sdaout,
	sdaIn,
///mobeam control signals///
	rd_revision_code,
	rst_mobeam,	
	led_polarity,
	mobeam_start_stop,
	o_ba_mem_data,
	i_ba_mem_addr,
	i_ba_mem_rd_en,
	i_ba_mem_rd_clk,
	o_bsr_mem_data,
	i_bsr_mem_addr,
	i_bsr_mem_rd_en,
	i_bsr_mem_rd_clk
);
///////i2c signals interface///////
input clk;
input rst;
inout scl;
output sdaout;
input sdaIn;
//////////////////////////////////
//mobeam control logic interface//
output  [15:0] o_ba_mem_data;
input [7:0] i_ba_mem_addr;
input i_ba_mem_rd_en;
input i_ba_mem_rd_clk;
output  rd_revision_code;
output 	rst_mobeam;
output  mobeam_start_stop;
output  [7:0] o_bsr_mem_data;
input [8:0] i_bsr_mem_addr;
input i_bsr_mem_rd_en;
input i_bsr_mem_rd_clk;

output  led_polarity;
	
//////////////////////////////////
///////wires and reg declarations////
wire[7:0]dataOut;
wire[7:0]regAddr;
wire writeEn;
wire readEn;
wire i2c_start;
wire[7:0]dataIn;
///////////////////////////////////////
serialInterface i2cslavedatafsm(/*AUTOARG*/
  // Outputs
    .dataOut(dataOut), 
	.regAddr(regAddr), 
	.sdaOut(sdaout), 
	.writeEn(writeEn), 
	.readEn(readEn), 
	.i2c_start(i2c_start),
    // Inputs
    .clk(clk), 
	.dataIn(dataIn), 
	.rst(rst), 
	.scl(scl), 
	.sdaIn(sdaIn)
 );


 /*mobeam_reg_sets*/mobeam_i2c_reg_interface mobeam_registers(
		.rst_i(rst),
		.clk_i(clk),
		//i2c slave interface signals///	
		.i2c_master_to_slave_data_i(dataOut),
		.i2c_slave_to_master_data_o(dataIn),
		.i2c_slave_data_address_i(regAddr),
		.wr_en_i(writeEn),
		.rd_en_i(readEn),
		.i2c_start_i(i2c_start),
		//MoBeam control logic interface//
		.rd_revision_code(rd_revision_code),
		.rst_mobeam(rst_mobeam),
		.led_polarity(led_polarity),
		.mobeam_start_stop(mobeam_start_stop),
		.o_ba_mem_data(o_ba_mem_data),
		.i_ba_mem_addr(i_ba_mem_addr),
		.i_ba_mem_rd_en(i_ba_mem_rd_en),
		.i_ba_mem_rd_clk(i_ba_mem_rd_clk),
		.o_bsr_mem_data(o_bsr_mem_data),
		.i_bsr_mem_addr(i_bsr_mem_addr),
		.i_bsr_mem_rd_en(i_bsr_mem_rd_en),
		.i_bsr_mem_rd_clk(i_bsr_mem_rd_clk)
		);
		
endmodule