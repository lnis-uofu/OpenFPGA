///////////////////////////////////////////////////////////////////////////////
//
//
// Copyright (C) 2007, Licensed customers of QuickLogic may copy or modify
// this file for use in designing QuickLogic devices only.
//
// Module Name:  spi_cmd_datin_line
// File Name:    spi_cmd_datin_line.v
// 
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ns

module spi_cmd_datin_line(
										rst,
										clk,

										spi_crc_check_en_bit,

										crc_rst,
										//crc_check_en,											
																			
										crc_error,
										cmd_datin_line, 																											
									    serial_dataout,
										start_multi_blk_wr_rcvd,
										start_blk_rcvd,

										state_spicard_idle,
										spicard_wr2host_bit,
                                    
									    xmit_data, 	    
										spi_dataout_wr_en,  
										state_spicard_rd_crc_chk,				
                                        state_spicard_wr_crc,       
                                        state_spicard_wr_datline0,    
                                        state_spicard_wr_daterr_tkn,  
                                        state_spicard_wr_start_tkn,     

                                        rcvd_data,
										state_spicard_rd_start_byte,
                                        state_spicard_rd_datline0,
                                        state_spicard_rd_crc
									);

	input			rst;
	input			clk;
	
	input           spi_crc_check_en_bit;

	input			crc_rst;
//	input			crc_check_en;											

	output			crc_error;
	input			cmd_datin_line;
	output          serial_dataout;
	input 			start_multi_blk_wr_rcvd;
	input           start_blk_rcvd;

	input           state_spicard_idle;
    input           spicard_wr2host_bit;

	input			xmit_data; 	 
	input           spi_dataout_wr_en;    
	input           state_spicard_rd_crc_chk; 				
	input			state_spicard_wr_crc;       
	input			state_spicard_wr_datline0;    
	input			state_spicard_wr_daterr_tkn;  
	input			state_spicard_wr_start_tkn;   

	output	    	rcvd_data;
	input           state_spicard_rd_start_byte;
    input           state_spicard_rd_datline0;
    input           state_spicard_rd_crc;

	wire            load_start_tkn;
	wire            load_multi_blk_wr_tkn;
	
    reg             dat_dly;
	reg             crc_error;
	wire            crc_dout;
	wire            gen_en;
    wire            out_en;

	assign	rcvd_data	   = dat_dly;
	assign	crc_din	       = spi_dataout_wr_en ? xmit_data : dat_dly;

	assign	gen_en	       = state_spicard_rd_datline0 | state_spicard_wr_datline0 
	                         | state_spicard_wr_daterr_tkn ;

    assign load_start_tkn  = state_spicard_wr_start_tkn | (start_blk_rcvd & state_spicard_rd_start_byte);

    assign load_multi_blk_wr_tkn = (start_multi_blk_wr_rcvd & state_spicard_rd_start_byte);

	assign	out_en	       = state_spicard_rd_crc | state_spicard_wr_crc;

    assign  serial_dataout = state_spicard_wr_crc ? crc_dout : xmit_data; ////dat_dly;

 	always@( posedge clk or posedge rst )	 
	begin
		if( rst )
		  dat_dly       <= 1'b0;
		//else if ( spi_dataout_wr_en | state_spicard_rd_crc_chk | (spicard_wr2host_bit & state_spicard_idle) )
		//  dat_dly       <= xmit_data;
		else
		  dat_dly   	<= cmd_datin_line;	
	end	  

	always@( posedge clk or posedge rst )	  
	begin
		if( rst )
		begin
			crc_error	    <= 1'b0;
		end
		else
		begin
			if( ~spi_crc_check_en_bit )
			begin
				crc_error	<= 1'b0;
			end
			else 
			if( out_en & ( crc_dout != dat_dly ) )
			begin
				crc_error   <= 1'b1;
			end
		end	
	end

	spi_crc16 crc16_inst(
					.rst		( crc_rst ),
					.clk		( clk ),
					.gen_en     ( gen_en ),
					.out_en     ( out_en ),
					.din		( crc_din ),
					.dout		( crc_dout ),
					.load_start_tkn ( load_start_tkn ),
					.load_multi_blk_wr_tkn ( load_multi_blk_wr_tkn ),
					.crc_reg    (  )
					);	   

endmodule




									  