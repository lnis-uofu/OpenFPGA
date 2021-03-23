///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2007, Licensed customers of QuickLogic may copy or modify
// this file for use in designing QuickLogic devices only.
//
// Module Name:  dat_control
// File Name:    dat_control.v
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ns

module dat_control(           
                       reset,    // global reset    
                       spi_reset,
					   io_abort,
                       clk,      
					   clkn,                      

                       dat,                           

					   //high_speed,	            
					   dat_width_4bit,    
					       
					   interrupt,
					   interrupt_sd4bit_en,
	                   rsp_end,
	                   intr_period_cmd_end,
	                   func_num_err,
	                   cmd_index,
					   cmd53_52_received,

					   // command 53 flag bits to determine read, write, modes, address incr
                       // all of these are registered except for byte_block_count
                       cmd52_53_rw_flag,  
                       cmd53_block_mode,    
                       cmd53_OP_code, 
                       cmd53_byte_block_count_nt_registered,
					   cmd52_53_reg_addr,
					   cmd52_53_func_num,
					   cmd53_received,
					   cmd53_wr_rd_executing,
					   block_mode_byte_size,
					   // end of registered signals

                       //fifo signal flags
                       data_txfifo_empty,
                       data_txfifo_rdy,  // this is a pulse only signal!
                       data_rxfifo_full,
                       data_rxfifo_empty,

                       current_cmd53_byte_block_count,

                       func0_block_size,
					   func1_block_size,
					   func2_block_size,
					   //func3_block_size,
					   //func4_block_size,
					   //func5_block_size,
					   //func6_block_size,
					   //func7_block_size,
					   spi_crc_check_en_bit,
                     
                       // output register write, read signals going to txfifo.v and rxfifo.v
					   // to properly execute CMD 53 byte or block mode reads.
					   cmd53_wr,
					   cmd53_rd,
					   cmd53_write_data,
					   cmd53_read_data,
					   cmd53_read_data_internal_reg,

                       // MISC
					   dat_lines_being_used,
					   //cmd52_reset,
		               //cmd52_abort, 
			           //func_suspend, 
			           exec_complete,
			           //func_resume,
					   cmd52_rd_after_wr,

					   //DATA transmission state
	                   state_DAT_IDLE,
					   state_BUSY,

                       cmd_txrx_cmd_idle,
			           // CMD BUS STATES
					   //cmd_bus_idle_state,    
	                   //cmd_bus_init_state,
	                   //cmd_bus_stby_state,    
                       //cmd_bus_cmd_state,      
                       //cmd_bus_txfr_state,     
	                   //cmd_bus_inactive_state,
					   
   					   //SPI mode
					   spi_csn,                    //input
					   spi_mode_on,                //input
					   spi_sdata,                  //output
					   sdcmd,                      //input
					   cmd_txrx_spicard_resp,      //output
					   all_blocks_written_out,     //input
	                   spicard_wr2host_bit,         //output
                       spicard_rd_frm_host_bit,     //output
					   clear_spicard_rd_frm_host_bit,  
					   clear_spicard_wr2host_bit,
					   spi_stop_tran_cmd_rcvd,
					   state_spicard_rd_busy,
					   spi_cmdrsp_sdata            //input

					   //// FOR testing purposes only
					   //dat_state,
                       //all_bytes_received,
                       //byte_end,
                       //i_cmd53_byte_block_count,
					   //dat_state_cnt,
                       //sdata,
                       //dout_reg,
                       //oe_reg
					   //// FOR testing purposes only
                   );

    input         reset;           
	input         spi_reset;   
	input         io_abort;                                        
    input         clk;                            
    input         clkn;
	inout   [3:0] dat;

    //input         high_speed;
	input         dat_width_4bit;

	input         interrupt;
	input         interrupt_sd4bit_en;
	input         rsp_end; 
	input         intr_period_cmd_end; 
	input         func_num_err; 
	input   [5:0] cmd_index; 
	input         cmd53_52_received;

    // command 53 flag bits to determine read, write, modes, address incr
    input         cmd52_53_rw_flag;  
    input         cmd53_block_mode;    
    input         cmd53_OP_code; 
    input   [9:0] cmd53_byte_block_count_nt_registered;
    input         cmd53_received;
	output        cmd53_wr_rd_executing;
	output  [9:0] block_mode_byte_size;

    input   [9:0] func0_block_size;
	input   [9:0] func1_block_size;
	input   [9:0] func2_block_size;
	//input   [9:0] func3_block_size;
	//input   [9:0] func4_block_size;
	//input   [9:0] func5_block_size;
	//input   [9:0] func6_block_size;
	//input   [9:0] func7_block_size;
	input         spi_crc_check_en_bit;
 
    // address bits coming from CMD53
    input  [16:0] cmd52_53_reg_addr;
    input   [2:0] cmd52_53_func_num;
                       
    // output register addr, write, read signals going to data fifo
    // to properly execute CMD 53 byte or block mode reads.
    output        cmd53_wr;
    output        cmd53_rd;
    output  [7:0] cmd53_write_data;
    input   [7:0] cmd53_read_data;
	input   [7:0] cmd53_read_data_internal_reg;

    // MISC
    output        dat_lines_being_used;
    output        exec_complete;
	//input         cmd52_reset;
    //input         cmd52_abort; 
    //input         func_suspend; 
    //input         func_resume;
	input         cmd52_rd_after_wr;

    //DATA transmission state
	output        state_DAT_IDLE;
	output        state_BUSY;

    input         cmd_txrx_cmd_idle;
    // CMD BUS STATES
//    input         cmd_bus_idle_state;    
//    input         cmd_bus_init_state;
//    input         cmd_bus_stby_state;    
//    input         cmd_bus_cmd_state;      
//    input         cmd_bus_txfr_state;     
//    input         cmd_bus_inactive_state;

    input         data_txfifo_empty;
    input         data_txfifo_rdy;           // this is a pulse only signal!
    input         data_rxfifo_full;
    input         data_rxfifo_empty;
    output  [9:0] current_cmd53_byte_block_count;

   	//SPI mode
    input         spi_csn;                    //input
    input         spi_mode_on;                //input
    output        spi_sdata;                  //output
	input         sdcmd;                      //input
    input         cmd_txrx_spicard_resp;      //output
    output        all_blocks_written_out;     //input
    input         spicard_wr2host_bit;        //output
    input         spicard_rd_frm_host_bit;    //output
	output        clear_spicard_rd_frm_host_bit;
	output        clear_spicard_wr2host_bit;
	input         spi_stop_tran_cmd_rcvd;
	output        state_spicard_rd_busy;      //output
	input         spi_cmdrsp_sdata;

					   //// FOR testing purposes only
    //output      [3:0] dat_state;
    //output            all_bytes_received;
    //output            byte_end;
    //output      [9:0] i_cmd53_byte_block_count;
	//output     [12:0] dat_state_cnt;
    //output            sdata;
    //output            dout_reg;
    //output            oe_reg;
					   //// FOR testing purposes only

    //// FOR testing purposes only
    wire            sdata;
    wire            dout_reg;
    wire            oe_reg;
    //// FOR testing purposes only

//--- Declaration of wires and regs
    wire          byte_almost_end;
	wire          cmd53_wr;
    wire          cmd53_rd;
	reg           icmd53_wr;
    reg           icmd53_rd;

  parameter	sDAT_IDLE	 	= 4'b0001;
  parameter sWR_START_WAIT	= 4'b0010;
  parameter	sWR_START	 	= 4'b0011;
  parameter	sWR_DATA	 	= 4'b0100;
  parameter	sWR_CRC	   	    = 4'b0101;
  parameter	sWR_END	   	    = 4'b0110; 

  parameter	sRD_START	 	= 4'b0111; 
  parameter	sRD_DATA	 	= 4'b1000;
  parameter	sRD_CRC	   	    = 4'b1001;
  parameter	sRD_END	   	    = 4'b1010;
  parameter	sCRC_STATUS	    = 4'b1011; 
  parameter	sBUSY	        = 4'b1100;

  parameter INTR_ON           = 2'b00; 
  parameter INTR_OFF          = 2'b01;
  parameter INTR_BLK_GAP_CLK1 = 2'b10;
  parameter INTR_BLK_GAP_CLK2 = 2'b11;

    wire          irst;
	wire          rst;
	wire    [3:0] dat;
	wire		  state_DAT_IDLE;  
	wire		  state_WR_START;  
	wire		  state_WR_DATA;   
	wire		  state_WR_CRC;    
	wire		  state_WR_END;    
	wire		  state_CRC_STATUS;
	wire		  state_BUSY;
	wire		  state_RD_START;  
	wire		  state_RD_DATA;   
	wire		  state_RD_CRC;    
	wire		  state_RD_END;

    reg     [3:0] dat_state;
    reg    [12:0] dat_state_cnt;

    reg     [9:0] i_cmd53_byte_block_count;

    wire          last_crc16_bit;
    reg           cmd53_wr_rd_executing;
    wire          byte_end;
	reg     [7:0] cmd_data;
    reg     [7:0] rcv_data;
	wire          crc_error;
    wire    [7:0] rcv0_data;
	wire          all_bytes_received;
	wire          last_crc_status_bit;
	reg     [9:0] block_mode_byte_size;
	wire    [1:0] rcv1_data;
    wire    [1:0] rcv2_data;
    wire    [1:0] rcv3_data;
	reg     [7:0] xmit0_data;
	reg     [1:0] xmit1_data;
	reg     [1:0] xmit2_data;
	reg     [1:0] xmit3_data;
	wire          crc0_error;
	wire          crc1_error;
	wire          crc2_error;
	wire          crc3_error;
	reg     [1:0] dat_phase;
	wire    [7:0] xmit_data;
	wire    [2:0] data_sel;
    wire          dat_lines_being_used;
    wire          exec_complete;
	wire          all_bytes_transmitted;
	wire    [7:0] cmd53_data;
	wire          clear_spicard_rd_frm_host_bit;
	wire          clear_spicard_wr2host_bit;

	wire    [7:0] rxfifo_spidatain_byte;
	wire          spi_dat_state_cnt_clr;
	reg           dat_state_cnt_clr;
	wire          spi_byte_almost_end;
	wire          mask_dataout_line;

	wire          state_spicard_idle;               
	wire          state_spicard_rd_start_byte;
	wire          state_spicard_rd_datline0;      
	wire          state_spicard_rd_crc;             
	wire          state_spicard_rd_dat_resp;       
	wire          state_spicard_rd_busy;         
	wire          state_spicard_wr_daterr_tkn;       
	wire          state_spicard_wr_start_tkn;      
	wire          state_spicard_wr_datline0;        
	wire          state_spicard_wr_crc;            
	wire          state_spicard_wr_chk_blk_rdy;     

	reg           io_abort_flopped;
	reg           state_WR_END_d1; 
	reg           state_WR_END_d2;
	wire          interrupt_out;
	reg     [1:0] intr_period_state;
	reg           interrupt_4bit;
	reg           interrupt_disable;
	reg           clr_io_abort_flopped;


assign dat_lines_being_used = ~state_DAT_IDLE;
assign exec_complete        = ~cmd53_wr_rd_executing & ~cmd53_received;

//--------------------------------------------------
//--------------------------------------------------
// -- START of dat control file code
//--------------------------------------------------
//--------------------------------------------------
assign irst = reset | spi_reset | io_abort;
assign rst  = reset | spi_reset;

// to keep track of byte_block counter that decrements each time a block is sent / received in block mode
// or each time a byte is sent / received for byte mode
  always@( posedge clk or posedge irst )
	begin
		if( irst )
		begin
          i_cmd53_byte_block_count   <= 10'b0;
		end
		else
		begin
          if( cmd53_received )
            i_cmd53_byte_block_count <= cmd53_byte_block_count_nt_registered;
   		  else if( (icmd53_rd | icmd53_wr) & ~cmd53_block_mode )  // if using byte mode then dec by byte read or written
		    i_cmd53_byte_block_count <= i_cmd53_byte_block_count - 1'b1;
          else if ( cmd53_byte_block_count_nt_registered[9] & cmd53_block_mode )  //infinite block transfer mode
            i_cmd53_byte_block_count <= cmd53_byte_block_count_nt_registered;
		  else if( ((state_WR_START & ~cmd52_53_rw_flag) | (state_RD_CRC & last_crc16_bit & cmd52_53_rw_flag))
		           & cmd53_block_mode )  // if using block mode then dec by block read or written
		    i_cmd53_byte_block_count <= i_cmd53_byte_block_count - 1'b1;
		end
    end

assign current_cmd53_byte_block_count = i_cmd53_byte_block_count;

//--------------------------------------------------
// -- CMD53 being executed flag
//--------------------------------------------------
  always@( posedge clk or posedge irst )
	begin
		if( irst )
		begin
          cmd53_wr_rd_executing      <= 1'b0;  
		end
		else
		begin
          if( cmd53_received )
		  begin
            cmd53_wr_rd_executing    <= 1'b1;   // if cmd 53 is received then start running       
		  end
		  else if( ((|i_cmd53_byte_block_count) & state_DAT_IDLE) |   // block or byte count becomes 0 and idle state
		           (cmd53_52_received & cmd53_wr_rd_executing) )       //to do the cmd52 io abort while in cmd53
		  begin
		    cmd53_wr_rd_executing    <= 1'b0;
		  end 
		end
    end

//--------------------------------------------------
// -- CMD53 reading and writing to registers in
// -- bytes
//--------------------------------------------------
assign cmd53_wr = icmd53_wr;
assign cmd53_rd = icmd53_rd;

  // create the write pulse to write to the data_rxfifo
  always@( posedge clk or posedge irst )
	begin
		if( irst )
		begin
          icmd53_wr   <= 1'b0;
		end
		else
		begin
          if( byte_end & state_RD_DATA & cmd53_wr_rd_executing & ~icmd53_wr 
		      & (~data_rxfifo_full | ~|cmd52_53_func_num) )
            icmd53_wr   <= 1'b1;		    
          else
		    icmd53_wr   <= 1'b0;	
		end
    end

  // create the read pulse to read from the data_txfifo
  always@( posedge clk or posedge irst )
	begin
		if( irst )
		begin
          icmd53_rd   <= 1'b0;
		end
		else
		begin
          if( ( ((data_txfifo_rdy | (~|cmd52_53_func_num & ~cmd52_53_rw_flag) ) & (|i_cmd53_byte_block_count) & state_DAT_IDLE & ~spi_mode_on ) 
		        | (byte_almost_end & state_WR_DATA & all_bytes_transmitted)) & cmd53_wr_rd_executing & ~icmd53_rd 
				& (~data_txfifo_empty | ~|cmd52_53_func_num) )
            icmd53_rd   <= 1'b1;		    
          else
		    icmd53_rd   <= 1'b0;	
		end
    end

  // create the register to store the data being received before writing to rxfifo
  // or to store the data to be transmitted from the txfifo
  assign cmd53_data = |cmd52_53_func_num ? cmd53_read_data : cmd53_read_data_internal_reg;

  always@( posedge clk or posedge irst )
	begin
		if( irst )
		begin
          cmd_data   <= 8'b0;
		end
		else
		begin
          if( data_txfifo_rdy | icmd53_rd | (~|cmd52_53_func_num & cmd52_rd_after_wr) )
		    cmd_data <= cmd53_data;               // reading from txfifo to transmit the data
		  else if( byte_end & state_RD_DATA & cmd53_wr_rd_executing & ~icmd53_wr 
		           & (~data_rxfifo_full | ~|cmd52_53_func_num) )
		    cmd_data <= rcv_data;                      // storing the data received to write to rxfifo
          else if( state_RD_END )  // transmit the crc_status back if good or bad data received
		  begin
            if( crc_error ) //bad data received transmit bad crc status
			  cmd_data <= {1'b0, 3'b101, 1'b1, 3'b0};
            else            //good data received transmit good crc status
			  cmd_data <= {1'b0, 3'b010, 1'b1, 3'b0}; 
		  end
		end
    end

assign cmd53_write_data = spi_mode_on ? rxfifo_spidatain_byte : cmd_data;   // storing the data to write to rxfifo

//--------------------------------------------------
// -- DAT Line write / read state machines
//--------------------------------------------------

assign datstate_cnt_clr = spi_mode_on ? spi_dat_state_cnt_clr : dat_state_cnt_clr;

always@( posedge clk or posedge irst)
begin
  if( irst )
    dat_state_cnt        <= 13'b0;
  else
    if( datstate_cnt_clr )
      dat_state_cnt      <= 13'b0;
	else
      dat_state_cnt      <= dat_state_cnt + 1'b1;
end

    assign	state_DAT_IDLE  	= ((dat_state == sDAT_IDLE) & ~spi_mode_on)    | (state_spicard_idle & spi_mode_on);
	assign	state_WR_START  	= ( dat_state == sWR_START );
	assign	state_WR_DATA   	= ( dat_state == sWR_DATA )     | ((state_spicard_wr_start_tkn | state_spicard_wr_datline0) & spi_mode_on);
	assign	state_WR_CRC    	= ( dat_state == sWR_CRC )      | (state_spicard_wr_crc & spi_mode_on);
	assign	state_WR_END    	= ( dat_state == sWR_END );
	assign	state_CRC_STATUS	= ( dat_state == sCRC_STATUS );
	assign	state_BUSY	        = ( dat_state == sBUSY );
	assign	state_RD_START  	= ( dat_state == sRD_START );
	assign	state_RD_DATA   	= ( dat_state == sRD_DATA )     | (state_spicard_rd_datline0 & spi_mode_on);
	assign	state_RD_CRC    	= ( dat_state == sRD_CRC )      | (state_spicard_rd_crc & spi_mode_on);
	assign	state_RD_END    	= ( dat_state == sRD_END );
	
	always@( posedge clk or posedge irst )
	begin
		if( irst )
		begin
			dat_state					<= sDAT_IDLE;	  
			dat_state_cnt_clr           <= 1'b1;
		end
		else
		begin
			case( dat_state )

				sDAT_IDLE:
				begin
			      dat_state_cnt_clr     <= 1'b1;
				  if( cmd53_wr_rd_executing & ~spi_mode_on & rcv0_data[0] )
				  begin
				    if( (data_txfifo_rdy | ~|cmd52_53_func_num) & ~cmd52_53_rw_flag 
					    & (|i_cmd53_byte_block_count) )
                    begin
				      dat_state				  <= sWR_START_WAIT;
					  dat_state_cnt_clr       <= 1'b1;
                    end
				    else if ( cmd52_53_rw_flag & (data_rxfifo_empty | ~|cmd52_53_func_num)
					          & (|i_cmd53_byte_block_count) )	
				    begin  
					  dat_state				  <= sRD_START;	  
					  dat_state_cnt_clr       <= 1'b1;
                    end
             	  end
			      //else
				  //    dat_state				  <= sDAT_IDLE;	  
				end

                sWR_START_WAIT:   // delay by 32 clocks before sending data out
				begin
			 	  dat_state_cnt_clr           <= 1'b0; 
                  if(dat_state_cnt[5] )
				  begin
				     dat_state_cnt_clr        <= 1'b1;
				     dat_state				  <= sWR_START;
                  end
				  //else
				  //   dat_state				  <= sWR_START_WAIT;
				end

                // WR_start is when the card is writing out to the dat line sending data to the host
				sWR_START:   // has to be only 1 clock cycle wide, state that sends the start bit on dat line
				begin
                  dat_state_cnt_clr           <= 1'b0;
				  dat_state				      <= sWR_DATA;
				end

				sWR_DATA:
				begin
	              dat_state_cnt_clr           <= 1'b0;
                  //if( (data_txfifo_empty & all_bytes_transmitted) & byte_end )
                  if( all_bytes_transmitted & byte_end )
				  begin
				    dat_state_cnt_clr         <= 1'b1;
					dat_state				  <= sWR_CRC;
                  end
				  //else
				  //  dat_state				  <= sWR_DATA;
				end

				sWR_CRC:
				begin
                  dat_state_cnt_clr           <= 1'b0;
                  if( last_crc16_bit & ~dat_state_cnt_clr )
				    dat_state				  <= sWR_END;
                  //else
				  //  dat_state				  <= sWR_CRC;
				end

				sWR_END:     // has to be only 1 clock cycle wide, state that sends the end bit
				begin
				  dat_state_cnt_clr           <= 1'b1;
				  dat_state				      <= sDAT_IDLE;
				end

                // RD_start is when the card is reading from the dat line and writing to the sram interface
				sRD_START:  
				begin
				  dat_state_cnt_clr           <= 1'b1;
                  if( ~rcv0_data[0] )
				  begin
				    dat_state				  <= sRD_DATA;
                    dat_state_cnt_clr         <= 1'b0;
				  end
				  //else
				  //  dat_state				  <= sRD_START;
				end				
						
				sRD_DATA:
				begin
                  dat_state_cnt_clr           <= 1'b0;
                  if( all_bytes_received & byte_end )
				  begin
				    dat_state_cnt_clr         <= 1'b1;
				    dat_state				  <= sRD_CRC;
				  end
                  //else
				  //  dat_state				  <= sRD_DATA;
				end

				sRD_CRC:
				begin
                  dat_state_cnt_clr           <= 1'b0;
                  if( last_crc16_bit & ~dat_state_cnt_clr )
				  begin
				    dat_state_cnt_clr         <= 1'b1;
					dat_state				  <= sRD_END;
                  end
                  //else
				  //  dat_state				  <= sRD_CRC;
				end

				sRD_END:      
				begin
                  //dat_state_cnt               <= dat_state_cnt + 1'b1;
				  //if( dat_state_cnt[0] )
                  begin
				    dat_state_cnt_clr         <= 1'b0;
  				    dat_state				  <= sCRC_STATUS;
                  end
				end

				sCRC_STATUS:  //send out the crc status bits NOTE: NEED TO ADD CRC CHECK HERE SO IF ERROR OCCURED THEN GO STRAIGHT TO IDLE AND CLEAR THE RXFIFO!!!!!!
				begin
	              dat_state_cnt_clr           <= 1'b0;
                  if( last_crc_status_bit )
				    dat_state				  <= sBUSY;
                  //else
				  //  dat_state				  <= sCRC_STATUS;
				end

				sBUSY:    // hold the line low for at least 1 cycle then if rxfifo empty then tristate dat0line, else if rxfifo not empty, hold this line low
				begin
				  dat_state_cnt_clr           <= 1'b1;
                  if( data_rxfifo_empty )
				    dat_state				  <= sDAT_IDLE;
                  //else
				  //  dat_state				  <= sBUSY;
				end

				default:
				begin
				  dat_state_cnt_clr           <= 1'b1;
				  dat_state				      <= sDAT_IDLE;
				end
			endcase
		end
	end

// to determine last byte in BLOCK mode
assign last_cmd53_func_byte  = (dat_width_4bit & ~spi_mode_on) 
                                          ? (block_mode_byte_size[9:0] == (dat_state_cnt[10:1] + 1'b1)) : 
                                            (block_mode_byte_size[9:0] == (dat_state_cnt[12:3] + 1'b1)) ; 
assign last_cmd53_byte_block = (i_cmd53_byte_block_count == 10'h1);   // to determine last byte in BYTE mode
assign byte_end              = (dat_width_4bit & spi_mode_on) ? dat_state_cnt[0] : (dat_state_cnt[2:0] == 3'b111);
assign byte_almost_end       = (dat_width_4bit & spi_mode_on) ? ~dat_state_cnt[0] : (dat_state_cnt[2:0] == 3'b110);
assign spi_byte_almost_end   = (dat_state_cnt[2:0] == 3'b110);
assign byte_start            = (dat_state_cnt[2:0] == 3'b000);
assign last_crc16_bit        = (dat_state_cnt[3:0] == 4'b1110);
assign last_crc_status_bit   = (dat_state_cnt[2:0] == 3'b100);
assign all_bytes_received    = cmd53_block_mode ?   
                               // block mode
                               //( dat_width_4bit ? (last_cmd53_func_byte & dat_state_cnt[0]) :
                                   (last_cmd53_func_byte)  : //(last_cmd53_func_byte & byte_end)  :                                
							   // byte mode
							   //( dat_width_4bit ? (last_cmd53_byte_block & dat_state_cnt[0]) :
                                   (last_cmd53_byte_block)  ;  //(last_cmd53_byte_block & byte_end)  ;  

assign all_bytes_transmitted = cmd53_block_mode ? (last_cmd53_func_byte)  :  //(last_cmd53_func_byte & byte_end)  : 
                                                  (i_cmd53_byte_block_count == 10'h0); //(i_cmd53_byte_block_count == 9'h0) & byte_end;

// to determine which block_size to use in receiving data bytes in BLOCK mode
always @ ( func0_block_size or func1_block_size or func2_block_size or cmd52_53_func_num )
           //or 
           //func3_block_size or func4_block_size or func5_block_size or 
		   //func6_block_size or func7_block_size or cmd52_53_func_num )
  begin
    case ( cmd52_53_func_num )
      3'b000  : block_mode_byte_size <= func0_block_size;
	  3'b001  : block_mode_byte_size <= func1_block_size;
	  3'b010  : block_mode_byte_size <= func2_block_size;
	  //3'b011  : block_mode_byte_size <= 10'h0;
	  //3'b100  : block_mode_byte_size <= 10'h0;
	  //3'b101  : block_mode_byte_size <= 10'h0;
	  //3'b110  : block_mode_byte_size <= 10'h0;
	  //3'b111  : block_mode_byte_size <= 10'h0;
	  default : block_mode_byte_size <= 10'h0;
	endcase  
  end

//--------------------------------------------------
// -- DAT Line shift register choices between 1 and 
// -- 4 bit modes
//--------------------------------------------------

// to transfer the data to the shift register on each of the dat lines
assign xmit_data = cmd_data;

	always@( dat_width_4bit or xmit_data or rcv0_data or rcv1_data or rcv2_data 
	         or rcv3_data or state_CRC_STATUS or state_BUSY )
	  begin
		if( ~dat_width_4bit | state_CRC_STATUS | state_BUSY )
		begin
		    rcv_data 	  	    <= rcv0_data;    //1 bit mode receiving from dat0 only
			xmit0_data       	<= xmit_data;    //1 bit mode transmiting from dat0 only
			xmit1_data	        <= 2'b0;
			xmit2_data	        <= 2'b0;
			xmit3_data	        <= 2'b0;
		end
		else
		begin
		    //4 bit mode receiving the data from all 4 dat lines
			rcv_data[7]			<= rcv3_data[1];
			rcv_data[6]			<= rcv2_data[1];
			rcv_data[5]			<= rcv1_data[1];
			rcv_data[4]			<= rcv0_data[1];
			rcv_data[3]			<= rcv3_data[0];
			rcv_data[2]			<= rcv2_data[0];
			rcv_data[1]			<= rcv1_data[0];
			rcv_data[0]			<= rcv0_data[0];
			//4 bit mode transmitting the data on all 4 dat lines
			xmit0_data[7:2]	    <= 6'b000000;
			xmit3_data[1]		<= xmit_data[7];
			xmit2_data[1]		<= xmit_data[6];
			xmit1_data[1]		<= xmit_data[5];
			xmit0_data[1]		<= xmit_data[4];
			xmit3_data[0]		<= xmit_data[3];
			xmit2_data[0]		<= xmit_data[2];
			xmit1_data[0]		<= xmit_data[1];
			xmit0_data[0]		<= xmit_data[0];
		end
	  end

	assign	dat_oe0			= state_WR_START | state_WR_DATA | state_WR_CRC | state_WR_END 
	                                         | state_CRC_STATUS | state_BUSY;
	assign	dat_oe1			= ( dat_width_4bit ) ? dat_oe0 & ~(state_CRC_STATUS | state_BUSY) : 1'b0;
	assign	dat_oe2			= ( dat_width_4bit ) ? dat_oe0 & ~(state_CRC_STATUS | state_BUSY) : 1'b0;
	assign	dat_oe3			= ( dat_width_4bit ) ? dat_oe0 & ~(state_CRC_STATUS | state_BUSY) : 1'b0;

    assign crc_error = crc0_error | crc1_error | crc2_error | crc3_error;

  always@( dat_state )
	begin
		case( dat_state )
		    // send out 0's on the dat lines
			sWR_START:
			begin
				dat_phase	<= 2'b00;
			end
			sBUSY:
			begin
				dat_phase	<= 2'b00;
			end

            // send out the actual data on the dat lines
			sCRC_STATUS:
			begin
				dat_phase	<= 2'b10;
			end
			sWR_DATA:
			begin
				dat_phase	<= 2'b10;
			end
			sRD_DATA:
			begin
				dat_phase	<= 2'b10;
			end

            // send out crc's on the dat lines
			sWR_CRC:
			begin
				dat_phase	<= 2'b11;
			end
			sRD_CRC:
			begin
				dat_phase	<= 2'b11;
			end

			// send out 1's on the dat lines
			default:
			begin
				dat_phase	<= 2'b01;
			end
		endcase
	end	

assign data_sel         = dat_state_cnt[2:0];
assign crc_rst          = state_DAT_IDLE;
assign crc_check_en     = state_RD_DATA | state_RD_CRC;
assign crc_check_en123	= ( dat_width_4bit ) ? crc_check_en : 1'b0;

	dat0_line iDAT0(
					.rst					( irst ),
					.clk					( clk ),
					.clkn 			     	( clkn ),
					//.high_speed 	        ( high_speed ),
					.dat_width_4bit 		( dat_width_4bit ),
					.oe 					( dat_oe0 ),
					.xmit_data 	  	        ( xmit0_data ),
					.rcv_data			    ( rcv0_data ),
					.crc_error		        ( crc0_error ),
					.dat_phase 		        ( dat_phase ),
                    .data_sel 		        ( data_sel ),
                    .dat					( dat[0] ),        // the actual inout dat lines
					.crc_rst 		   	    ( crc_rst ),
					.crc_check_en           ( crc_check_en ),
					.state_CRC_STATUS       ( state_CRC_STATUS ),
					
					//spi mode
					.spi_datline0_wr_en     ( spi_datline0_wr_en ),
                    .spi_datline0_data      ( spi_datline0_data ),
					.spi_mode_on            ( spi_mode_on ),
					.mask_dataout_line      ( mask_dataout_line )

					 //// FOR testing purposes only
                     //.sdata(sdata),
                     //.dout_reg(dout_reg),
                     //.oe_reg(oe_reg)
					 //// FOR testing purposes only
					);

//--------------------------------------------------
// -- Interrupt Period definition
//--------------------------------------------------
  always@( posedge clk or posedge rst )
	begin
		if( rst )
		begin
		    state_WR_END_d1      <= 1'b0;
			state_WR_END_d2      <= 1'b0;
			clr_io_abort_flopped <= 1'b0;
		end
		else
		begin
		    state_WR_END_d1      <= state_WR_END & cmd53_wr_rd_executing;
			state_WR_END_d2      <= state_WR_END_d1;
			clr_io_abort_flopped <= io_abort_flopped & cmd_txrx_cmd_idle;
		end
    end 

  always@( posedge clk or posedge rst )	 //08h
	begin
  	  if( rst )
 	    io_abort_flopped       <= 1'b0;
  	  else
	  begin
        if( clr_io_abort_flopped )
          io_abort_flopped     <= 1'b0;
		else if( io_abort )
		  io_abort_flopped     <= 1'b1;
      end
    end

always@( posedge clk or posedge rst )
	begin
		if( rst )
		begin
		    intr_period_state <= INTR_ON;
			interrupt_4bit    <= 1'b0;
			interrupt_disable <= 1'b0;
		end
		else
		begin
		  if( ~dat_width_4bit | spi_mode_on )
          begin
		    intr_period_state <= INTR_ON;
			interrupt_4bit    <= 1'b0;
			interrupt_disable <= 1'b0;
		  end
		  else
		  begin
		    case (intr_period_state)
               INTR_ON : 
               begin 
                   intr_period_state <= INTR_ON;
                   interrupt_4bit    <= interrupt;
				   interrupt_disable <= 1'b0;
				 if( ((intr_period_cmd_end | func_num_err) & (cmd_index == 6'b11_0101)) | io_abort )
				 begin
				   intr_period_state <= INTR_OFF;
				   interrupt_4bit    <= 1'b0;
				   interrupt_disable <= interrupt;
                 end
				 //else 
				 //begin
				 //  intr_period_state <= INTR_ON;
                 //  interrupt_4bit    <= interrupt;
				 //  interrupt_disable <= 1'b0;
				 //end
               end
               
               INTR_OFF  :
               begin
			       intr_period_state <= INTR_OFF;
                   interrupt_4bit    <= 1'b0; 
				   interrupt_disable <= 1'b0;
				 if( clr_io_abort_flopped | 
				     (( state_WR_END_d2 | (state_RD_END & cmd53_wr_rd_executing)) & (~|i_cmd53_byte_block_count)) )
                 begin  
				   intr_period_state <= INTR_ON;
                   interrupt_4bit    <= interrupt;
				   interrupt_disable <= 1'b0;
				 end
                 else if( (( state_WR_END_d2 | state_RD_END ) & |i_cmd53_byte_block_count & cmd53_block_mode & interrupt_sd4bit_en) )
                 begin
				   intr_period_state <= INTR_BLK_GAP_CLK1;
                   interrupt_4bit    <= interrupt;
				   interrupt_disable <= 1'b0;
				 end
                 //else 
				 //begin
				 //  intr_period_state <= INTR_OFF;
                 //  interrupt_4bit    <= 1'b0; 
				 //  interrupt_disable <= 1'b0;
				 //end
               end
               
               INTR_BLK_GAP_CLK1 :
               begin
                 intr_period_state <= INTR_BLK_GAP_CLK2;
				 interrupt_4bit    <= 1'b0;
				 interrupt_disable <= interrupt;
               end
               
               INTR_BLK_GAP_CLK2 :
               begin
                 intr_period_state <= INTR_OFF;
				 interrupt_4bit    <= 1'b0;
				 interrupt_disable <= 1'b0;
               end

               default    :
               begin
                  intr_period_state <= INTR_ON;
                  interrupt_4bit    <= 1'b0;
				  interrupt_disable <= 1'b0;
               end
            endcase	
		  end	
		end
	end

assign interrupt_out = (~spi_mode_on & dat_width_4bit) ? interrupt_4bit : interrupt;

    dat123_line iDAT1(
					.rst					( irst ),
					.clk					( clk ),
					.clkn					( clkn ),
					//.high_speed		        ( high_speed ),
					.oe						( dat_oe1 ),
	    			.xmit_data		        ( xmit1_data ),
					.rcv_data		  	    ( rcv1_data ),						
					.crc_error		        ( crc1_error ),
					.dat_phase		        ( dat_phase ),
					.data_sel		    	( data_sel[0] ),
					.dat					( dat[1] ),																
					.crc_rst		    	( crc_rst ),
					.crc_check_en	        ( crc_check_en123 ),

					.interrupt              ( interrupt_out ),
					.interrupt_disable      ( interrupt_disable )
					);

    dat123_line iDAT2(
					.rst					( irst ),
					.clk					( clk ),
					.clkn					( clkn ),
					//.high_speed		        ( high_speed ),
					.oe						( dat_oe2 ),
	    			.xmit_data		        ( xmit2_data ),
					.rcv_data		  	    ( rcv2_data ),						
					.crc_error		        ( crc2_error ),
					.dat_phase		        ( dat_phase ),
					.data_sel		    	( data_sel[0] ),
					.dat					( dat[2] ),																
					.crc_rst		    	( crc_rst ),
					.crc_check_en	        ( crc_check_en123 ),

					.interrupt              ( 1'b0 ),
					.interrupt_disable      ( 1'b0 )
					);

    dat123_line iDAT3(
					.rst					( irst ),
					.clk					( clk ),
					.clkn					( clkn ),
					//.high_speed		        ( high_speed ),
					.oe						( dat_oe3 ),
	    			.xmit_data		        ( xmit3_data ),
					.rcv_data		  	    ( rcv3_data ),						
					.crc_error		        ( crc3_error ),
					.dat_phase		        ( dat_phase ),
					.data_sel		    	( data_sel[0] ),
					.dat					( dat[3] ),																
					.crc_rst		    	( crc_rst ),
					.crc_check_en	        ( crc_check_en123 ),
					
					.interrupt              ( 1'b0 ),
					.interrupt_disable      ( 1'b0 )
					);

spi_dat_control spi_dat_control_inst(           
                       .reset                    ( reset ),    
					   .spi_reset                ( spi_reset ),
                       .clk                      ( clk ),      
					               
					   .cmd_in                   ( sdcmd ),		       //cmd line signal used for reading purposes   
//                       .rcv0_data0               ( rcv0_data0 ),                           
                       
					   // command 53 flag bits to determine read, write, modes, address incr
                       // all of these are registered except for byte_block_count
					   .cmd52_53_func_num        ( cmd52_53_func_num ),
                       .cmd52_53_rw_flag         ( cmd52_53_rw_flag ),  
                       .cmd53_block_mode         ( cmd53_block_mode ),    
//                       .cmd53_OP_code            ( cmd53_OP_code ), 
//                       .cmd53_byte_block_count_nt_registered  ( cmd53_byte_block_count_nt_registered ),
//					   .cmd53_received           ( cmd53_received ),
//					   .cmd53_wr_rd_executing    ( cmd53_wr_rd_executing ),
//					   .block_mode_byte_size     ( block_mode_byte_size ),
					   // end of registered signals

                       //fifo signal flags
                       .data_txfifo_empty        ( data_txfifo_empty ),
                       .data_txfifo_rdy          ( data_txfifo_rdy ),  // this is a pulse only signal!
//                       .data_rxfifo_full         ( data_rxfifo_full ),
                       .data_rxfifo_empty        ( data_rxfifo_empty ),
                    
                       // output register write, read signals going to txfifo.v and rxfifo.v
					   // to properly execute CMD 53 byte or block mode reads.
//					   .spi_cmd53_wr             ( spi_cmd53_wr ),
//					   .spi_cmd53_rd             ( spi_cmd53_rd ),

 					   //SPI mode
					   .spi_csn                           ( spi_csn ),                    //input
					   .spi_mode_on                       ( spi_mode_on ),                //input
//					   .spi_dat_sdata                     ( spi_dat_sdata ),              //output
					   .cmd_txrx_spicard_resp             ( cmd_txrx_spicard_resp ),      //input
					   .spi_cmdrsp_sdata                  ( spi_cmdrsp_sdata ),           //input
					   .all_blocks_written_out            ( all_blocks_written_out ),     //output
	                   .spicard_wr2host_bit               ( spicard_wr2host_bit ),        //input
                       .spicard_rd_frm_host_bit           ( spicard_rd_frm_host_bit ),    //input
					   .clear_spicard_rd_frm_host_bit     ( clear_spicard_rd_frm_host_bit ),  //output
					   .clear_spicard_wr2host_bit         ( clear_spicard_wr2host_bit ),  //output
//					   .spi_dat_err                       ( spi_dat_err ),                //output
					   .spi_stop_tran_cmd_rcvd            ( spi_stop_tran_cmd_rcvd ),     //input
//					   .flush_txfifo                      ( flush_txfifo ),               //output
					   .spi_dat_state_cnt_clr             ( spi_dat_state_cnt_clr ),      //output
					   .all_bytes_received                ( all_bytes_received ),         //input
					   .all_bytes_transmitted             ( all_bytes_transmitted ),      //input
					   .last_crc16_bit                    ( last_crc16_bit ),             //input
					   .i_cmd53_byte_block_count          ( i_cmd53_byte_block_count ),   //input
					   .byte_end                          ( byte_end ),                   //input
					   .spi_byte_almost_end               ( spi_byte_almost_end ),        //input
//					   .byte_start                        ( byte_start ),                 //input
					   .cmd53_data                        ( cmd53_data ),                 //input
//					   .spi_datrsp_sdata                  ( spi_datrsp_sdata ),           //output
                       .rxfifo_spidatain_byte             ( rxfifo_spidatain_byte ),      //output
					   .mask_dataout_line                 ( mask_dataout_line ),
					   .spi_crc_check_en_bit              ( spi_crc_check_en_bit ),
//					   .spi_dataout_wr_en                 ( spi_dataout_wr_en ),
					   .spi_datline0_wr_en                ( spi_datline0_wr_en ),
                       .spi_datline0_data                 ( spi_datline0_data ),

//					   .out_of_range                      ( out_of_range ),               //input from cmd_control
//					   .error                             ( error ),                      //input from cmd_control

					   .state_spicard_idle                ( state_spicard_idle ),  	        //output
    	               .state_spicard_rd_start_byte       ( state_spicard_rd_start_byte ),   //output
		               .state_spicard_rd_datline0         ( state_spicard_rd_datline0 ),     //output 
		               .state_spicard_rd_crc              ( state_spicard_rd_crc ),  	    //output  
		               .state_spicard_rd_dat_resp         ( state_spicard_rd_dat_resp ),     //output
		               .state_spicard_rd_busy             ( state_spicard_rd_busy ),         //output
		               .state_spicard_wr_daterr_tkn       ( state_spicard_wr_daterr_tkn ),   //output
		               .state_spicard_wr_start_tkn        ( state_spicard_wr_start_tkn ),    //output	
		               .state_spicard_wr_datline0         ( state_spicard_wr_datline0 ),     //output
		               .state_spicard_wr_crc              ( state_spicard_wr_crc ),          //output
		               .state_spicard_wr_chk_blk_rdy      ( state_spicard_wr_chk_blk_rdy )   //output
                   );

endmodule






















