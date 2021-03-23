///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2007, Licensed customers of QuickLogic may copy or modify
// this file for use in designing QuickLogic devices only.
//
// Module Name:  spi_dat_control
// File Name:    spi_dat_control.v
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ns

module spi_dat_control(           
                       reset,    // global reset    
					   spi_reset,
                       clk,      
					               
					   cmd_in,		       //cmd line signal used for reading purposes   
//                       rcv0_data0,                           
                       
					   // command 53 flag bits to determine read, write, modes, address incr
                       // all of these are registered except for byte_block_count
					   cmd52_53_func_num,
                       cmd52_53_rw_flag,  
                       cmd53_block_mode,    
//                       cmd53_OP_code, 
//                       cmd53_byte_block_count_nt_registered,
//					   cmd53_received,
//					   cmd53_wr_rd_executing,
//					   block_mode_byte_size,
					   // end of registered signals

                       //fifo signal flags
                       data_txfifo_empty,
                       data_txfifo_rdy,  // this is a pulse only signal!
//                       data_rxfifo_full,
                       data_rxfifo_empty,
                    
                       // output register write, read signals going to txfifo.v and rxfifo.v
					   // to properly execute CMD 53 byte or block mode reads.
//					   spi_cmd53_wr,
//					   spi_cmd53_rd,

 					   //SPI mode
					   spi_csn,                    //input
					   spi_mode_on,                //input
//					   spi_dat_sdata,              //output
					   cmd_txrx_spicard_resp,      //input
					   spi_cmdrsp_sdata,           //input
					   all_blocks_written_out,     //output
	                   spicard_wr2host_bit,        //input
                       spicard_rd_frm_host_bit,    //input
					   clear_spicard_rd_frm_host_bit,  //output
					   clear_spicard_wr2host_bit,  //output
					   spi_dat_err,                //output
					   spi_stop_tran_cmd_rcvd,     //input
					   flush_txfifo,               //output
					   spi_dat_state_cnt_clr,       //output
					   all_bytes_received,         //input
					   all_bytes_transmitted,      //input
					   last_crc16_bit,             //input
					   i_cmd53_byte_block_count,   //input
					   byte_end,                   //input
					   spi_byte_almost_end,        //input
//					   byte_start,                 //input
					   cmd53_data,                 //input
					   spi_datrsp_sdata,           //output
					   rxfifo_spidatain_byte,      //output
					   mask_dataout_line,          //output
					   spi_crc_check_en_bit,       //input
					   spi_dataout_wr_en,          //output
					   spi_datline0_wr_en,         //output
                       spi_datline0_data,          //output

					   out_of_range,               //input from cmd_control
					   error,                      //input from cmd_control

					   state_spicard_idle,  	           //output
    	               state_spicard_rd_start_byte,         //output
		               state_spicard_rd_datline0,            //output 
		               state_spicard_rd_crc,  	            //output  
		               state_spicard_rd_dat_resp,          //output
		               state_spicard_rd_busy,              //output
		               state_spicard_wr_daterr_tkn,         //output
		               state_spicard_wr_start_tkn,         //output	
		               state_spicard_wr_datline0,          //output
		               state_spicard_wr_crc,            //output
		               state_spicard_wr_chk_blk_rdy         //output
                   );

	input			reset;    // global reset  
	input           spi_reset;  
	input			clk;      
					               
	input			cmd_in;		       //cmd line signal used for reading purposes   
//	input			rcv0_data0;                           
                       
    // command 53 flag bits to determine read, write, modes, address incr
	input   [2:0] cmd52_53_func_num;
    input         cmd52_53_rw_flag;  
    input         cmd53_block_mode;    
//    input         cmd53_OP_code; 
//    input   [9:0] cmd53_byte_block_count_nt_registered;
//    input         cmd53_received;
//	input         cmd53_wr_rd_executing;
//	input   [9:0] block_mode_byte_size;

    //fifo signal flags
	input         data_txfifo_empty;
	input         data_txfifo_rdy;  // this is a pulse only signal!
//	input         data_rxfifo_full;
	input         data_rxfifo_empty;
                    
    // output register write, read signals going to txfifo.v and rxfifo.v
    // to properly execute CMD 53 byte or block mode reads.
//	input         spi_cmd53_wr;
//	input         spi_cmd53_rd;

    //SPI mode
	input         spi_csn;                    //input
	input         spi_mode_on;                //input
//	output        spi_dat_sdata;              //output
	input         cmd_txrx_spicard_resp;      //input
	input         spi_cmdrsp_sdata;           //input
	output        all_blocks_written_out;     //output
	input         spicard_wr2host_bit;        //input
	input         spicard_rd_frm_host_bit;    //input
	output        clear_spicard_rd_frm_host_bit;  //output
	output        clear_spicard_wr2host_bit;  //output
	output        spi_dat_err;                //output
	input         spi_stop_tran_cmd_rcvd;     //input
	output        flush_txfifo;               //output
	output        spi_dat_state_cnt_clr;       //output
	input         all_bytes_received;         //input
	input         all_bytes_transmitted;      //input
	input   [9:0] i_cmd53_byte_block_count;   //input
	input         last_crc16_bit;             //input
	input         byte_end;                   //input
	input         spi_byte_almost_end;        //input
//	input         byte_start;                 //input
	input   [7:0] cmd53_data;                 //input
	output        spi_datrsp_sdata;           //output
	output  [7:0] rxfifo_spidatain_byte;      //output
	output        mask_dataout_line;          //output
	input         spi_crc_check_en_bit;       //input
	output        spi_dataout_wr_en;          //output 
	output        spi_datline0_wr_en;         //output
    output        spi_datline0_data;          //output

	input         out_of_range;               //input from cmd_control
	input         error;                      //input from cmd_control

	output        state_spicard_idle;  	           //output
	output        state_spicard_rd_start_byte;         //output
	output        state_spicard_rd_datline0;            //output 
	output        state_spicard_rd_crc;  	            //output  
	output        state_spicard_rd_dat_resp;          //output
	output        state_spicard_rd_busy;              //output
	output        state_spicard_wr_daterr_tkn;         //output
	output        state_spicard_wr_start_tkn;        //output	
	output        state_spicard_wr_datline0;          //output
	output        state_spicard_wr_crc;            //output
	output        state_spicard_wr_chk_blk_rdy;         //output

//--- Declaration of wires and regs

    reg     [7:0] data_byte;
	reg     [7:0] data_out_byte;
	reg     [3:0] spi_dat_state;
	reg           spi_dat_state_cnt_clr;
	wire          spi_datin;
	wire    [4:0] host_datawr_rsp_tkn;
	wire    [7:0] host_datard_err_tkn;
	wire    [7:0] host_datrd_start_tkn;
	wire          stop_trn_tkn_rcvd;
	wire          start_multi_blk_wr_rcvd;
	wire          start_blk_rcvd;
	wire    [7:0] data_in_byte;
    wire          mask_dataout_line;
	wire          spi_dataout_wr_en;

	wire          data_blk_rdy2send;
	wire          all_blocks_rcvd;
	wire          single_blk_wr2host;
	wire          single_blk_rd_frm_host;
	wire          cmd_datin_crc_error;
//	wire          spi_crc_check_en;           

	wire          state_spicard_idle;
	wire          state_spicard_rd_start_byte;
	wire          state_spicard_rd_datline0;
	wire          state_spicard_rd_crc;
	wire          state_spicard_rd_crc_chk;
	wire          state_spicard_rd_dat_resp;
	wire          state_spicard_rd_busy;
	wire          state_spicard_rd_wait;
	wire          state_spicard_wr_daterr_tkn;
	wire          state_spicard_wr_start_tkn;
	wire          state_spicard_wr_datline0;
	wire          state_spicard_wr_crc;
	wire          state_spicard_wr_chk_blk_rdy;

  parameter	spi_CARD_IDLE	   	     = 4'b0000;
  // for Host write mode where card reads data from the datline0
  parameter	spi_CARD_RD_START_BYTE   = 4'b0001;
  parameter	spi_CARD_RD_DATLINE0	 = 4'b0010;
  parameter	spi_CARD_RD_CRC  	     = 4'b0011;
  parameter	spi_CARD_RD_CRC_CHK	     = 4'b0100;
  parameter	spi_CARD_RD_DAT_RESP	 = 4'b0101; 
  parameter	spi_CARD_RD_BUSY  	     = 4'b0110;
  parameter spi_CARD_RD_WAIT         = 4'b0111;
  // for Host read mode where card writes to the datline0
  parameter	spi_CARD_WR_DATERR_TKN	 = 4'b1000;
  parameter	spi_CARD_WR_START_TKN    = 4'b1001;
  parameter	spi_CARD_WR_DATLINE0	 = 4'b1010;
  parameter	spi_CARD_WR_CRC 	   	 = 4'b1011;
  parameter	spi_CARD_WR_CHK_BLK_RDY  = 4'b1100; 


assign irst = reset  | ~spi_mode_on | spi_reset;

//-- Datain line shift reg in
always@( posedge clk or posedge irst )
begin
  if( irst )
    data_byte           <= 8'b0;
  else
    if( spi_dataout_wr_en | state_spicard_rd_crc_chk )
	begin
	  if( byte_end & ~state_spicard_rd_dat_resp )  //for normal wr_datline0 data in host read mode
        data_byte       <= data_out_byte;
      else
	  begin
	    data_byte[7:1]  <= data_byte[6:0];
        data_byte[0]    <= 1'b0;
	  end
	end
	//used for write dat resp in host write mode and to write the starttkn or dat err tkn in host read mode
	else if( (last_crc16_bit & state_spicard_rd_crc) |  (~spicard_rd_frm_host_bit & state_spicard_idle) )
      data_byte         <= data_out_byte;
	else 
	begin
	  data_byte[7:1]    <= data_byte[6:0];
	  data_byte[0]      <= spi_datin;
    end
end

assign data_in_byte     = data_byte;
assign datrsp_sdata     = data_byte[7];

  always@( spi_dat_state or host_datawr_rsp_tkn or host_datard_err_tkn or host_datrd_start_tkn or
           cmd53_data or spicard_wr2host_bit or spi_dat_err or spicard_rd_frm_host_bit)
	begin
		case( spi_dat_state )
  		    spi_CARD_RD_CRC:
			begin
			  data_out_byte <= {host_datawr_rsp_tkn, 3'b0};
			end

			spi_CARD_RD_CRC_CHK: 
			begin
			  data_out_byte <= {host_datawr_rsp_tkn, 3'b0};
			end

			spi_CARD_RD_DAT_RESP:
			begin
			  data_out_byte <= {host_datawr_rsp_tkn, 3'b0};
			end

            spi_CARD_IDLE:
            begin
              if( ~spicard_rd_frm_host_bit & ~spi_dat_err )
                data_out_byte <= host_datrd_start_tkn; 			      
              else if( ~spicard_rd_frm_host_bit & spi_dat_err )
                data_out_byte <= host_datard_err_tkn;
              end

			spi_CARD_WR_DATERR_TKN:
			begin
			    data_out_byte <= cmd53_data; //host_datard_err_tkn;
			end

			spi_CARD_WR_START_TKN:
			begin
			    data_out_byte <= cmd53_data; //host_datrd_start_tkn;
			end

			spi_CARD_WR_DATLINE0:
			begin
			  data_out_byte <= cmd53_data;
			end

			default:  // for SPI_CARD_BUSY ALSO to transmit all zeros
			begin
			  data_out_byte <= 8'b0;
			end
		endcase
  end

//--------------------------------------------------
// -- DAT Line write / read state machines
//--------------------------------------------------

    assign	state_spicard_idle  	        = ( spi_dat_state == spi_CARD_IDLE );
    assign	state_spicard_rd_start_byte  	= ( spi_dat_state == spi_CARD_RD_START_BYTE );
	assign	state_spicard_rd_datline0      	= ( spi_dat_state == spi_CARD_RD_DATLINE0 );
	assign	state_spicard_rd_crc  	        = ( spi_dat_state == spi_CARD_RD_CRC );
	assign	state_spicard_rd_crc_chk        = ( spi_dat_state == spi_CARD_RD_CRC_CHK);
	assign	state_spicard_rd_dat_resp   	= ( spi_dat_state == spi_CARD_RD_DAT_RESP );
	assign	state_spicard_rd_busy       	= ( spi_dat_state == spi_CARD_RD_BUSY );
	assign	state_spicard_rd_wait       	= ( spi_dat_state == spi_CARD_RD_WAIT );
	assign	state_spicard_wr_daterr_tkn  	= ( spi_dat_state == spi_CARD_WR_DATERR_TKN );
	assign	state_spicard_wr_start_tkn  	= ( spi_dat_state == spi_CARD_WR_START_TKN );
	assign	state_spicard_wr_datline0   	= ( spi_dat_state == spi_CARD_WR_DATLINE0 );
	assign	state_spicard_wr_crc        	= ( spi_dat_state == spi_CARD_WR_CRC );
	assign	state_spicard_wr_chk_blk_rdy  	= ( spi_dat_state == spi_CARD_WR_CHK_BLK_RDY );
										
	always@( posedge clk or posedge irst )
	begin
		if( irst )
		begin
			spi_dat_state					<= spi_CARD_IDLE;	  
			spi_dat_state_cnt_clr           <= 1'b1;
		end
		else
		begin
			case( spi_dat_state )
				spi_CARD_IDLE :
				begin
				  spi_dat_state_cnt_clr     <= 1'b1;
				  if( spicard_rd_frm_host_bit )
				  begin
				    spi_dat_state			<= spi_CARD_RD_START_BYTE;	  
				    spi_dat_state_cnt_clr   <= 1'b1;
				  end
                  else if( spicard_wr2host_bit & ~spi_dat_err & (data_txfifo_rdy | ~|cmd52_53_func_num))
 			      begin
				    spi_dat_state			<= spi_CARD_WR_START_TKN;
                    spi_dat_state_cnt_clr   <= 1'b0;
				  end
				  else if( spicard_wr2host_bit & spi_dat_err & (data_txfifo_rdy | ~|cmd52_53_func_num))
 			      begin
				    spi_dat_state			<= spi_CARD_WR_DATERR_TKN;
				    spi_dat_state_cnt_clr   <= 1'b0;
				  end
				  //else
                  //  spi_dat_state			<= spi_CARD_IDLE;
				end

                //-- for Host write mode where card reads data from the datline0
				spi_CARD_RD_START_BYTE :
				begin
                  spi_dat_state_cnt_clr     <= 1'b1;
				  if( stop_trn_tkn_rcvd )
				    spi_dat_state			<= spi_CARD_RD_BUSY;
				  else if( start_multi_blk_wr_rcvd | start_blk_rcvd )
				  begin
				    spi_dat_state			<= spi_CARD_RD_DATLINE0;
				    spi_dat_state_cnt_clr   <= 1'b0;
				  end
				  //else
				  //  spi_dat_state			<= spi_CARD_RD_START_BYTE;
				end

      	        spi_CARD_RD_DATLINE0 :	 
   				begin
				  spi_dat_state_cnt_clr     <= 1'b0;
				  if( all_bytes_received & byte_end )
                  begin
				    spi_dat_state			<= spi_CARD_RD_CRC;
				    spi_dat_state_cnt_clr   <= 1'b1;
				  end
				  //else
				  //  spi_dat_state			<= spi_CARD_RD_DATLINE0;	  
				end
	            
				spi_CARD_RD_CRC : 	    
   				begin
                  spi_dat_state_cnt_clr     <= 1'b0;
                  if( last_crc16_bit )
				  begin
				    spi_dat_state			<= spi_CARD_RD_CRC_CHK;
					spi_dat_state_cnt_clr   <= 1'b1;
				  end
                  //else
				  //  spi_dat_state			<= spi_CARD_RD_CRC;
				end  	            
				
                spi_CARD_RD_CRC_CHK :
   				begin
                  spi_dat_state_cnt_clr     <= 1'b0;
                  spi_dat_state			    <= spi_CARD_RD_DAT_RESP;
				end

				spi_CARD_RD_DAT_RESP :
   				begin
				  spi_dat_state_cnt_clr     <= 1'b0;
                  if( byte_end )
				  begin
				    spi_dat_state			<= spi_CARD_RD_BUSY;
					spi_dat_state_cnt_clr   <= 1'b1;
				  end
				end  	            
				
				spi_CARD_RD_BUSY : 	  
   				begin
				  if( data_rxfifo_empty )
				    spi_dat_state			<= spi_CARD_RD_WAIT;
                  //else
				  //  spi_dat_state			<= spi_CARD_RD_BUSY;
				end

				spi_CARD_RD_WAIT : 	   //wait for the sram to write all data to the sram lines
   				begin
				    spi_dat_state			<= spi_CARD_IDLE;
				end

                //-- for Host read mode where card writes to the datline0
  	            spi_CARD_WR_DATERR_TKN :	 
				begin
				  spi_dat_state_cnt_clr     <= 1'b0;
                  if( byte_end )
				    spi_dat_state			<= spi_CARD_IDLE;
				  //else
				  //  spi_dat_state			<= spi_CARD_WR_DATERR_TKN;
				end

                spi_CARD_WR_START_TKN :   
  	   			begin
				  if( spi_byte_almost_end )
				    spi_dat_state_cnt_clr   <= 1'b1;
                  else
				    spi_dat_state_cnt_clr   <= 1'b0;
				  
				  if( byte_end )
				  begin
				    spi_dat_state			<= spi_CARD_WR_DATLINE0;
				  end
				  else if( spi_stop_tran_cmd_rcvd )
				    spi_dat_state			<= spi_CARD_IDLE;
				  //else
				  //  spi_dat_state			<= spi_CARD_WR_START_TKN;
				end

	            spi_CARD_WR_DATLINE0 :	 
   				begin
				  spi_dat_state_cnt_clr     <= 1'b0;
                  if( all_bytes_transmitted & byte_end )
				  begin
				    spi_dat_state			<= spi_CARD_WR_CRC;
				    spi_dat_state_cnt_clr   <= 1'b1;
				  end
				  else if( spi_stop_tran_cmd_rcvd )
				    spi_dat_state			<= spi_CARD_IDLE;
				  //else
				  //  spi_dat_state			<= spi_CARD_WR_DATLINE0;
				end

             	spi_CARD_WR_CRC :	   	
   				begin
				  spi_dat_state_cnt_clr     <= 1'b0;
                  if( last_crc16_bit )
				  begin
				    spi_dat_state			<= spi_CARD_WR_CHK_BLK_RDY;
				  end
				  else if( spi_stop_tran_cmd_rcvd )
				    spi_dat_state			<= spi_CARD_IDLE;
				  //else
				  //  spi_dat_state			<= spi_CARD_WR_CRC;
				end

             	spi_CARD_WR_CHK_BLK_RDY :
   				begin
				  //spi_dat_state_cnt_clr     <= 1'b1;
                  //if( data_blk_rdy2send )
				  //begin
				    spi_dat_state			<= spi_CARD_IDLE;
					spi_dat_state_cnt_clr   <= 1'b1;
				  //end
				  //else if( spi_stop_tran_cmd_rcvd )
				  //  spi_dat_state			<= spi_CARD_IDLE;
				  //else
				  //  spi_dat_state			<= spi_CARD_WR_CHK_BLK_RDY;
				end

				default:
				begin
				  spi_dat_state				<= spi_CARD_IDLE;	  
			      spi_dat_state_cnt_clr     <= 1'b1;
				end
			endcase
		end
	end

assign mask_dataout_line = spicard_rd_frm_host_bit ? spi_csn : 1'b0;

assign clear_spicard_rd_frm_host_bit = stop_trn_tkn_rcvd | ((single_blk_rd_frm_host | all_blocks_rcvd) 
									   & data_rxfifo_empty & state_spicard_rd_wait);

assign flush_txfifo = spi_stop_tran_cmd_rcvd;  //used to ensure the txfifo is clean after a stop tran cmd

assign rxfifo_spidatain_byte    = data_in_byte;

assign spi_dat_err              = 1'b0;  //FOR NOW HARDWIRE THIS TO ZERO!

assign stop_trn_tkn_rcvd        = (data_in_byte[6:0] == 7'b1111_110) & spi_datin & state_spicard_rd_start_byte;
assign start_multi_blk_wr_rcvd  = (data_in_byte[6:0] == 7'b1111_110) & ~spi_datin & state_spicard_rd_start_byte 
                                  & cmd53_block_mode & cmd52_53_rw_flag;
assign start_blk_rcvd           = &data_in_byte[6:0] & ~spi_datin & state_spicard_rd_start_byte 
                                  & ~(cmd53_block_mode & cmd52_53_rw_flag);
assign all_blocks_rcvd          = ~(|i_cmd53_byte_block_count);
assign all_blocks_written_out   = ~(|i_cmd53_byte_block_count);

assign host_datawr_rsp_tkn      = cmd_datin_crc_error ? 5'b0_1011 : 5'b0_0101;  //NOTE need to add write error in here too??
assign host_datard_err_tkn      = {4'b0, out_of_range, 2'b0, error}; 

assign host_datrd_start_tkn     = 8'b1111_1110;

assign single_blk_rd_frm_host   = ~cmd53_block_mode & cmd52_53_rw_flag;

assign data_blk_rdy2send        = ~data_txfifo_empty | ~|cmd52_53_func_num;

assign spi_dataout_wr_en   = state_spicard_wr_datline0 | state_spicard_wr_daterr_tkn |
							 state_spicard_rd_dat_resp | state_spicard_wr_start_tkn |
							 state_spicard_wr_crc | state_spicard_rd_busy;

assign clear_spicard_wr2host_bit =   spi_stop_tran_cmd_rcvd | spi_dat_err  
                                   | ((all_blocks_written_out | single_blk_wr2host) 
								   & data_blk_rdy2send & state_spicard_wr_chk_blk_rdy); 

assign single_blk_wr2host = ~cmd53_block_mode & ~cmd52_53_rw_flag;

//to seperate if the data to be sent on datline0 is coming from cmd or spidat control module
assign spi_datline0_data  = cmd_txrx_spicard_resp ? spi_cmdrsp_sdata : (spi_datrsp_sdata & ~state_spicard_rd_busy);
assign spi_datline0_wr_en = spi_dataout_wr_en | cmd_txrx_spicard_resp;

spi_cmd_datin_line cmd_datin_line_inst(
									.rst               ( irst ),
									.clk               ( clk ),
                                    .spi_crc_check_en_bit ( spi_crc_check_en_bit ),

									.crc_rst           ( state_spicard_idle ),
//									.crc_check_en      ( spi_crc_check_en ),											
																			
									.crc_error         ( cmd_datin_crc_error ), 
									.cmd_datin_line    ( cmd_in ),
									.serial_dataout    ( spi_datrsp_sdata ),
									.start_multi_blk_wr_rcvd ( start_multi_blk_wr_rcvd ),
									.start_blk_rcvd ( start_blk_rcvd ),

									.state_spicard_idle  ( state_spicard_idle ),
									.spicard_wr2host_bit ( spicard_wr2host_bit ),
                                                             
									.xmit_data 	       ( datrsp_sdata ),	 
									.spi_dataout_wr_en          ( spi_dataout_wr_en ),	
									.state_spicard_rd_crc_chk    ( state_spicard_rd_crc_chk ),						
                                    .state_spicard_wr_crc         ( state_spicard_wr_crc ),
                                    .state_spicard_wr_datline0         ( state_spicard_wr_datline0 ),
                                    .state_spicard_wr_daterr_tkn         ( state_spicard_wr_daterr_tkn ),
                                    .state_spicard_wr_start_tkn         ( state_spicard_wr_start_tkn ),
                                    
									.rcvd_data         ( spi_datin ),
									.state_spicard_rd_start_byte  ( state_spicard_rd_start_byte ),
                                    .state_spicard_rd_datline0    ( state_spicard_rd_datline0 ),
                                    .state_spicard_rd_crc         ( state_spicard_rd_crc )
									);

endmodule
















