///////////////////////////////////////////////////////////////////////////////
//
//
// Copyright (C) 2007, Licensed customers of QuickLogic may copy or modify
// this file for use in designing QuickLogic devices only.
//
// Module Name:  dat0_line
// File Name:    dat0_line.v
// 
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ns

module dat0_line(
									rst,
									clk,
									clkn,
							
									//high_speed,
									crc_rst,
									dat_width_4bit,
									oe,								
									data_sel,
									crc_check_en,											
									dat_phase,
									xmit_data,
									
									rcv_data,
									crc_error,
									dat,
									state_CRC_STATUS,

									//spi mode
					                spi_datline0_wr_en,
                                    spi_datline0_data, 
				                    spi_mode_on,
									mask_dataout_line
									
								    //// FOR testing purposes only
                                    //sdata,
                                    //dout_reg,
                                    //oe_reg
					                 //// FOR testing purposes only 
								);

	input				rst;
	input				clk;
	input				clkn;

	//input				high_speed;
	input				crc_rst;
	input				dat_width_4bit;
	input				oe;								
	input		[2:0]	data_sel;
	input				crc_check_en;											
	input		[1:0]	dat_phase;
	input		[7:0]	xmit_data;

	output	    [7:0]	rcv_data;
	output				crc_error;
	
	inout				dat;

	input               state_CRC_STATUS;

	//spi mode
	input               spi_datline0_wr_en;
	input               spi_datline0_data;
	input               spi_mode_on;
	input               mask_dataout_line;

    //// FOR testing purposes only
	//output				sdata;
	//output				dout_reg;
	//output				oe_reg;
    //// FOR testing purposes only 
	
	reg					dout_reg_hs;
	reg					oe_reg_hs;
	reg		    [7:0]	dat_dly;
	reg					dout_reg_ls;
	reg					oe_reg_ls;
	reg					dout;
	reg					sdata;
	reg					crc_error;
	
	wire				dout_reg;
	wire				oe_reg;
	wire				oe_reg_hs_e;
	
	wire				crc_din;
	wire				crc_dout;
	wire				gen_en;
	wire				out_en;
	wire                sdata_1bit;

	assign	rcv_data	= dat_dly;
	assign	dat 		= ( oe_reg )       ? dout_reg    : 1'bZ;  
	assign	dout_reg	= dout_reg_ls; //( high_speed & ~spi_mode_on )   ? dout_reg_hs : dout_reg_ls;
	assign	oe_reg		= oe_reg_ls;   //( high_speed & ~spi_mode_on )   ? oe_reg_hs_e : oe_reg_ls;	
	assign	oe_reg_hs_e	= oe_reg_hs;// | oe;
	assign	crc_din	    = ( crc_check_en ) ? dat_dly[0]  : dout;
	assign	gen_en	    = ( dat_phase == 2'b10 );
	assign	out_en	    = ( &dat_phase );

	always@( posedge clk or posedge rst )	 
	begin
		if( rst )
		begin
			dout_reg_hs		 <= 1'b0;
			oe_reg_hs		 <= 1'b0;
			dat_dly[7:0]	 <= 8'b0;
		end
		else
		begin
          if( spi_mode_on )
		  begin
			dout_reg_hs	     <= spi_datline0_data;
			oe_reg_hs		 <= spi_datline0_wr_en & ~mask_dataout_line; 
		  end
		  else
		  begin        
			dout_reg_hs	     <= dout;
			oe_reg_hs		 <= oe;	
			if( ~oe_reg_hs )
			begin
				dat_dly[7:1] <= dat_dly[6:0];	   
				dat_dly[0]	 <= dat;
			end
          end
		end	
	end	  

	always@( posedge clkn or posedge rst )
	begin
		if( rst )
		begin
			dout_reg_ls	    <= 1'b0;
			oe_reg_ls		<= 1'b0;
		end
		else
		begin
			dout_reg_ls	    <= dout_reg_hs;
			oe_reg_ls		<= oe_reg_hs_e;
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

	assign	sdata_1bit	     = ( data_sel[0] )     ? xmit_data[0] : xmit_data[1];
	
	always@( xmit_data or data_sel or dat_width_4bit or state_CRC_STATUS or data_sel[0] or sdata_1bit)
	begin
		if( dat_width_4bit & ~state_CRC_STATUS )
		begin
			sdata	<= sdata_1bit;
			//if( data_sel[0] ) 
			//begin
			//	 sdata	<= xmit_data[0];
			//end
			//else
			//begin
			//	 sdata	<= xmit_data[1];
			//end
		end
		else
		begin
			case( data_sel )
				3'b000:		sdata <= xmit_data[7];
				3'b001:		sdata <= xmit_data[6];
				3'b010:		sdata <= xmit_data[5];
				3'b011:		sdata <= xmit_data[4];
				3'b100:		sdata <= xmit_data[3];
				3'b101:		sdata <= xmit_data[2];
				3'b110:		sdata <= xmit_data[1];
				default:	sdata <= xmit_data[0];	
			endcase
		end
	end	
			
	always@( posedge clk or posedge rst )	  
	begin
		if( rst )
		begin
			crc_error	<= 1'b0;
		end
		else
		begin
			if( ~crc_check_en )
			begin
				crc_error	<= 1'b0;
			end
			else if( out_en & ( crc_dout != dat_dly[0] ) )
			begin
				crc_error <= 1'b1;
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