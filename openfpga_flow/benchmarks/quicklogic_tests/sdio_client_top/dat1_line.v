///////////////////////////////////////////////////////////////////////////////
//
//
// Copyright (C) 2007, Licensed customers of QuickLogic may copy or modify
// this file for use in designing QuickLogic devices only.
//
// Module Name:  dat1_line
// File Name:    dat1_line.v
// 
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ns

module dat1_line(
										rst,
										clk,
										clkn,
								
										//high_speed,
										crc_rst,
										interrupt,
										interrupt_disable,
										oe,								
										data_sel,
										crc_check_en,											
										dat_phase,
										xmit_data,
										
										rcv_data,
										crc_error,
										dat
									);

	input			rst;
	input			clk;
	input			clkn;

	//input			high_speed;
	input			crc_rst;
	input			interrupt;
	input           interrupt_disable;
	input			oe;								
	input			data_sel;
	input			crc_check_en;											
	input	[1:0]	dat_phase;
	input	[1:0]	xmit_data;

	output	[1:0]	rcv_data;
	output			crc_error;
	
	inout			dat;

	reg				dout_reg_hs;
	reg				oe_reg_hs;
	reg		[1:0]	dat_dly;
	reg				dout_reg_ls;
	reg				oe_reg_ls;
	reg				dout;
	reg				crc_error;
	
	wire			dout_reg;
	wire			oe_reg;
	wire			oe_reg_hs_e;
	
	wire			crc_din;
	wire			crc_dout;
	wire			gen_en;
	wire			out_en;
	wire			sdata;
	
	assign	sdata	     = ( data_sel )     ? xmit_data[0] : xmit_data[1];
	assign	rcv_data	 = dat_dly;
	assign	dat 		 = ( oe_reg )       ? dout_reg     : 1'bZ;  
	assign	dout_reg	 = dout_reg_ls; //( high_speed )   ? dout_reg_hs  : dout_reg_ls;
	assign	oe_reg		 = oe_reg_ls;   //( high_speed )   ? oe_reg_hs_e  : oe_reg_ls;	
	assign	oe_reg_hs_e	 = oe_reg_hs; //| oe;
	assign	crc_din	     = ( crc_check_en ) ? dat_dly[0]   : dout;
	assign	gen_en	     = ( dat_phase == 2'b10 );
	assign	out_en	     = ( &dat_phase );

 	always@( posedge clk or posedge rst )	 
	begin
		if( rst )
		begin
			dout_reg_hs		<= 1'b0;
			oe_reg_hs		<= 1'b0;
			dat_dly[1:0]	<= 2'b0;
		end
		else
		begin
			dout_reg_hs	    <= (dout & ~interrupt) | interrupt_disable; //since interrupt is active low
			oe_reg_hs		<= oe | interrupt | interrupt_disable;	
			if( ~oe_reg_hs )
			begin
				dat_dly[1]	<= dat_dly[0];	   
				dat_dly[0]	<= dat;
			end
		end	
	end	  

	always@( posedge clkn or posedge rst )
	begin
		if( rst )
		begin
			dout_reg_ls	<= 1'b0;
			oe_reg_ls	<= 1'b0;
		end
		else
		begin
			dout_reg_ls	<= dout_reg_hs;
			oe_reg_ls   <= oe_reg_hs_e;
		end
	end	

	always@( dat_phase or sdata or crc_dout )
	begin
		case( dat_phase )
			2'b01:
			begin
				dout	<= 1'b1;
			end
			2'b10:
			begin
				dout	<= sdata;
			end
			2'b11:
			begin
				dout	<= crc_dout;
			end
			default:
			begin
				dout	<= 1'b0;
			end
		endcase
	end

	always@( posedge clk or posedge rst )	  
	begin
		if( rst )
		begin
			crc_error	    <= 1'b0;
		end
		else
		begin
			if( ~crc_check_en )
			begin
				crc_error	<= 1'b0;
			end
			else if( out_en & ( crc_dout != dat_dly[0] ) )
			begin
				crc_error   <= 1'b1;
			end
		end	
	end

	crc16 crc16_inst(
					.rst		( crc_rst ),
					.clk		( clk ),
					.gen_en     ( gen_en ),
					.out_en     ( out_en ),
					.din		( crc_din ),
					.dout		( crc_dout ),
					.crc_reg    (  )
					);	   

endmodule