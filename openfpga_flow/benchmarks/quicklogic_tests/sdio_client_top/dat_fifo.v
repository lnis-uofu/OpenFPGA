///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2007, Licensed customers of QuickLogic may copy or modify
// this file for use in designing QuickLogic devices only.
//
// Module Name:  dat_fifo
// File Name:    dat_fifo.v
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ns

module dat_fifo(           
                       reset,    // global reset  
					   spi_reset,  
                       io_abort,
					   clk,
					   sram_clk,
                       //fifo signal flags
                       data_txfifo_empty,
                       data_txfifo_rdy,  // this is a pulse only signal!
                       data_rxfifo_full,
                       data_rxfifo_empty,

                       /////////////////////////////////
                       //register interface
					   /////////////////////////////////
                       //high_speed,	
					   dat_width_4bit,                    
                       resp4_max_num_func,
                       resp4_ocr_reg,
                       resp6_RCA,
					   //cmd52_reset,
		               //cmd52_abort, 
			           //func_suspend, 
			           //func_resume,
					   //func0_block_size,
					   //func1_block_size,
					   //func2_block_size,
					   //func3_block_size,
					   //func4_block_size,
					   //func5_block_size,
					   //func6_block_size,
					   //func7_block_size,

                       /////////////////////////////////
                       //dat control interface   
                       /////////////////////////////////
                       current_cmd53_byte_block_count,                
                       // output register write, read signals going to txfifo.v and rxfifo.v
					   // to properly execute CMD 53 byte or block mode reads.
					   cmd53_wr,
					   cmd53_rd,
					   cmd53_write_data,
					   cmd53_read_data,
                       // MISC
					   dat_lines_being_used,
					   exec_complete,
					   cmd53_wr_rd_executing,
					   block_mode_byte_size,
			           //DATA transmission state
	                   state_DAT_IDLE,           
					   state_BUSY,     
					   state_spicard_rd_busy,
				   
				   	   /////////////////////////////////
				       //cmd control interface          
					   /////////////////////////////////                               
					   cmd52_accessed_byte_register_sram,
                       cmd52_write_data,
                       cmd52_rd_after_wr,
					   cmd52_write,
                       // all of these are registered except for byte_block_count
					   // assume these signals do not change once written so save on sync registers
                       cmd52_53_rw_flag,  
                       cmd53_block_mode,    
                       cmd53_OP_code, 
                       cmd53_byte_block_count_nt_registered,
					   cmd52_53_reg_addr,
					   cmd52_53_func_num,
					   cmd53_received,
					   sramclksync_cmd53_52_received,
					   // end of registered signals
					   // CMD BUS STATES
					   cmd_bus_idle_state,    
	                   cmd_bus_init_state,
	                   cmd_bus_stby_state,    
                       cmd_bus_cmd_state,      
                       cmd_bus_txfr_state,     
	                   cmd_bus_inactive_state, 
				   
                       /////////////////////////////////				   
				       //sram interface
                       /////////////////////////////////
				       //sram_csn7,
					   //sram_csn6,
					   //sram_csn5,
					   //sram_csn4,
					   //sram_csn3,
					   //sram_csn2,
					   //sram_csn1,
					   sram_wen_out,
					   sram_oen_out,
                       sram_addr,
					   sram_data_out,     //data going out of the sram to the external world
                       sram_data_in,      //data going into the sram from the external world
					   sram_resetn,
					   sram_data_oe,
					   sram_onn,

                       /////////////////////////////////
					   // SRAM CLK DOMAIN SIGNALS
					   /////////////////////////////////
                       txfifo_byte_size_filled,
                       sramclksync_icmd53_wr_rd_executing,
					   sramclksync_txfifo_Almost_Full_pulse,
                       sramclksync_get_nxt_block_to_transmit,
					   sramclksync_cmd52_rd_after_wr,
					   sramclksync_cmd52_rd_after_wr_pulse,
					   sramclksync_cmd52_write_pulse,
					   sramclksync_cmd52_rd_after_wr_pulse_dly1,
					   sramclksync_cmd52_rd_after_wr_pulse_dly2,

					   /////////////////////////////////
					   // CLK DOMAIN SIGNALS
					   /////////////////////////////////
					   clksync_txfifo_byte_size_filled,
					   txfifo_Almost_Full_pulse,
					   get_nxt_block_to_transmit
					   );

    input         reset;      
	input         spi_reset;  
	input         io_abort;
    input         clk;
	input         sram_clk;
                       
	//fifo signal flags
    output        data_txfifo_empty;
    output        data_txfifo_rdy;        // this is a pulse only signal!
    output        data_rxfifo_full;
    output        data_rxfifo_empty;

    /////////////////////////////////
    //register interface
    /////////////////////////////////
    //input         high_speed;	
    input         dat_width_4bit;                    
	input   [2:0] resp4_max_num_func;
	input  [23:0] resp4_ocr_reg;
	input  [15:0] resp6_RCA;
    //input         cmd52_reset;
    //input         cmd52_abort; 
    //input         func_suspend; 
    //input         func_resume;
	//input   [9:0] func0_block_size;
	//input   [9:0] func1_block_size;
	//input   [9:0] func2_block_size;
	//input   [9:0] func3_block_size;
	//input   [9:0] func4_block_size;
	//input   [9:0] func5_block_size;
	//input   [9:0] func6_block_size;
	//input   [9:0] func7_block_size;

    /////////////////////////////////
    //dat control interface   
    /////////////////////////////////
    input   [9:0] current_cmd53_byte_block_count;                
    // output register write, read signals going to txfifo.v and rxfifo.v
    // to properly execute CMD 53 byte or block mode reads.
    input         cmd53_wr;
    input         cmd53_rd;
    input   [7:0] cmd53_write_data;
    output  [7:0] cmd53_read_data;
    // MISC
    input         dat_lines_being_used;
    input         exec_complete;
	input         cmd53_wr_rd_executing;
	input   [9:0] block_mode_byte_size;
    //DATA transmission state
    input         state_DAT_IDLE;      
	input         state_BUSY;          
	input         state_spicard_rd_busy;
				   
    /////////////////////////////////
    //cmd control interface          
    /////////////////////////////////                               
	output  [7:0] cmd52_accessed_byte_register_sram;
	input   [7:0] cmd52_write_data;
    input         cmd52_rd_after_wr;
    input         cmd52_write;
    // all of these are registered except for byte_block_count
    input         cmd52_53_rw_flag;  
    input         cmd53_block_mode;    
    input         cmd53_OP_code; 
    input   [9:0] cmd53_byte_block_count_nt_registered;
	input  [16:0] cmd52_53_reg_addr;
    input   [2:0] cmd52_53_func_num;
    input         cmd53_received;
	input         sramclksync_cmd53_52_received;
    // end of registered signals
    // CMD BUS STATES
    input         cmd_bus_idle_state;    
    input         cmd_bus_init_state;
    input         cmd_bus_stby_state;    
    input         cmd_bus_cmd_state;     
    input         cmd_bus_txfr_state;     
    input         cmd_bus_inactive_state;
				   
    /////////////////////////////////				   
    //sram interface
    /////////////////////////////////
    output        sram_resetn;
	//output        sram_csn7;
    //output        sram_csn6;
    //output        sram_csn5;
    //output        sram_csn4;
    //output        sram_csn3;
    //output        sram_csn2;
    //output        sram_csn1;
    output        sram_wen_out;
    output        sram_oen_out;
    output [16:0] sram_addr;
    output  [7:0] sram_data_out;     //data going out of the sram to the external world
    input   [7:0] sram_data_in;      //data going into the sram from the external world
	output        sram_data_oe;
	input         sram_onn;

    /////////////////////////////////
	// SRAM CLK DOMAIN SIGNALS
	/////////////////////////////////
    output        txfifo_byte_size_filled;
    input         sramclksync_icmd53_wr_rd_executing;
	input         sramclksync_txfifo_Almost_Full_pulse;
    input         sramclksync_get_nxt_block_to_transmit;
	input         sramclksync_cmd52_rd_after_wr;
	input         sramclksync_cmd52_rd_after_wr_pulse;
	input         sramclksync_cmd52_write_pulse;
	input         sramclksync_cmd52_rd_after_wr_pulse_dly1;
	input         sramclksync_cmd52_rd_after_wr_pulse_dly2;

	/////////////////////////////////
    // CLK DOMAIN SIGNALS
	/////////////////////////////////
	input         clksync_txfifo_byte_size_filled;
    //output        cmd53_wr_rd_executing;
	output        txfifo_Almost_Full_pulse;
	output        get_nxt_block_to_transmit;
	//output        cmd52_rd_after_wr;

    //DECLARATION OF WIRES / REGS
	//rxfifo signals
    wire          rxfifo_Fifo_Push_Flush;
    wire          rxfifo_Fifo_Pop_Flush;
    wire          rxfifo_Push_Clk;
    wire          rxfifo_Pop_Clk;
    wire          rxfifo_PUSH;
    reg           rxfifo_Pop;
    wire    [8:0] rxfifo_DIN;
    wire    [8:0] rxfifo_DOUT;
    wire    [3:0] rxfifo_PUSH_FLAG;
    wire    [3:0] rxfifo_POP_FLAG;
    wire          rxfifo_Almost_Full;
    wire          rxfifo_Almost_Empty;

    //txfifo signals
    wire          txfifo_Fifo_Push_Flush;
    wire          txfifo_Fifo_Pop_Flush;
    wire          txfifo_Push_Clk;
    wire          txfifo_Pop_Clk;
    reg           txfifo_PUSH;
    wire          txfifo_POP;
    reg     [8:0] txfifo_DIN;
    wire    [8:0] txfifo_DOUT;
    wire    [3:0] txfifo_PUSH_FLAG;
    wire    [3:0] txfifo_POP_FLAG;
    wire          txfifo_Almost_Full;
    wire          txfifo_Almost_Empty;

    wire          txfifo_now_rdy;      //means the txfifo is now filled with data to be transmitted
    reg           txfifo_Almost_Full_pulse;
    reg           txfifo_Almost_Full_f;
    wire          dat_width_4bit;
	wire          irst;
    reg     [9:0] cmd53_byte_block_count_nt;

	reg    [16:0] sram_addr;
	wire          sram_resetn;
	//wire          sram_csn;
    reg           sram_wen;
    reg           sram_oen;
	reg     [7:0] sram_data_out;
	wire          sram_data_oe;
    reg     [7:0] cmd52_accessed_byte_register_sram;

	wire          cmd52_sram_wen;
	wire          sram_wen_out;
	wire          sram_oen_out;

//--- CLOCK 
assign irst            = reset | spi_reset | io_abort | sram_onn;
assign txfifo_Pop_Clk  = clk;
assign txfifo_Push_Clk = sram_clk;
assign rxfifo_Push_Clk = clk;
assign rxfifo_Pop_Clk  = sram_clk;

//MISC FIFO stuff
assign data_txfifo_empty = ~|txfifo_POP_FLAG;  /////////txfifo_Almost_Empty;
assign data_txfifo_full  = ~|txfifo_PUSH_FLAG;  /////////txfifo_Almost_Full;
assign data_txfifo_rdy   = txfifo_Almost_Full_pulse;  // should be a pulse
assign data_rxfifo_empty = ~|rxfifo_POP_FLAG; ////////rxfifo_Almost_Empty;
assign data_rxfifo_full  = ~|rxfifo_PUSH_FLAG; ////////rxfifo_Almost_Full;

assign rxfifo_Fifo_Push_Flush   = irst;
assign rxfifo_Fifo_Pop_Flush    = irst;
assign txfifo_Fifo_Push_Flush   = irst;
assign txfifo_Fifo_Pop_Flush    = irst;

assign icmd53_wr_rd_executing   = cmd53_wr_rd_executing & |cmd52_53_func_num;
assign txfifo_POP               = cmd53_rd & |cmd52_53_func_num;
assign cmd53_read_data          = txfifo_DOUT;
assign rxfifo_PUSH              = cmd53_wr & |cmd52_53_func_num;
assign rxfifo_DIN               = cmd53_write_data;

//TXFIFO , gets data from the sram and transmits it on the dat lines
af512x9_512x9  txfifo_tb(
                      .DIN                ( txfifo_DIN ),
					  .Fifo_Push_Flush    ( txfifo_Fifo_Push_Flush ),
					  .Fifo_Pop_Flush     ( txfifo_Fifo_Pop_Flush ),
					  .PUSH               ( txfifo_PUSH ),
					  .POP                ( txfifo_POP ),
					  .Push_Clk           ( txfifo_Push_Clk ),
                      .Pop_Clk            ( txfifo_Pop_Clk ),
                      .Almost_Full        ( txfifo_Almost_Full ),
					  .Almost_Empty       ( txfifo_Almost_Empty ),
					  .PUSH_FLAG          ( txfifo_PUSH_FLAG ),
					  .POP_FLAG           ( txfifo_POP_FLAG ),
					  .DOUT               ( txfifo_DOUT )
					  );

//RXFIFO , receives data from the dat lines and writes it to the sram or registers
af512x9_512x9  rxfifo_tb(
                      .DIN                ( rxfifo_DIN ),
					  .Fifo_Push_Flush    ( rxfifo_Fifo_Push_Flush ),
					  .Fifo_Pop_Flush     ( rxfifo_Fifo_Pop_Flush ),
					  .PUSH               ( rxfifo_PUSH ),
					  .POP                ( rxfifo_Pop ),
					  .Push_Clk           ( rxfifo_Push_Clk ),
                      .Pop_Clk            ( rxfifo_Pop_Clk ),
                      .Almost_Full        ( rxfifo_Almost_Full ),
					  .Almost_Empty       ( rxfifo_Almost_Empty ),
					  .PUSH_FLAG          ( rxfifo_PUSH_FLAG ),
					  .POP_FLAG           ( rxfifo_POP_FLAG ),
					  .DOUT               ( rxfifo_DOUT )
					  );

/////////////////////////////////				   
//clk section logic
/////////////////////////////////
always@(posedge clk or posedge irst)
  begin
    if( irst )
	begin
      txfifo_Almost_Full_pulse <= 1'b0;
	  txfifo_Almost_Full_f     <= 1'b0;
    end
	else
	begin
	  if( icmd53_wr_rd_executing )
	  begin
	    txfifo_Almost_Full_pulse <= txfifo_now_rdy & state_DAT_IDLE & ~txfifo_Almost_Full_f;
        txfifo_Almost_Full_f     <= txfifo_now_rdy & state_DAT_IDLE;  
	  end
	end
  end

//signal to tell the dat control interface that the txfifo is filled with data
assign txfifo_now_rdy = ~cmd52_53_rw_flag & state_DAT_IDLE & clksync_txfifo_byte_size_filled;

//signal to tell the sram portion to fill the txfifo buffer with data
assign get_nxt_block_to_transmit = (~data_txfifo_full  & icmd53_wr_rd_executing &
                                    ~cmd52_53_rw_flag & (|current_cmd53_byte_block_count)); 
//assign get_nxt_block_to_transmit = cmd53_block_mode ?  (~data_txfifo_full & state_DAT_IDLE & icmd53_wr_rd_executing &
                                                      // ~cmd52_53_rw_flag & (|current_cmd53_byte_block_count)) 
													  // : (~data_txfifo_full & state_DAT_IDLE & icmd53_wr_rd_executing &
                                                      // ~cmd52_53_rw_flag);

/////////////////////////////////				   
//sram_clk section logic
/////////////////////////////////

assign sram_resetn   = ~reset & ~sram_onn;
//assign sram_csn7     = ~(cmd52_53_func_num == 3'b111);
//assign sram_csn6     = ~(cmd52_53_func_num == 3'b110);
//assign sram_csn5     = ~(cmd52_53_func_num == 3'b101);
//assign sram_csn4     = ~(cmd52_53_func_num == 3'b100);
//assign sram_csn3     = ~(cmd52_53_func_num == 3'b011);
//assign sram_csn2     = ~(cmd52_53_func_num == 3'b010);
//assign sram_csn1     = ~(cmd52_53_func_num == 3'b001);

//this signal is used to increment the counter to determine how much data is in the fifo
always@(posedge sram_clk or posedge irst)
  begin
    if( irst )
	begin
      cmd53_byte_block_count_nt <= 10'b0;
    end
	else
	  if( sramclksync_icmd53_wr_rd_executing & ~sram_oen & ~sramclksync_cmd52_rd_after_wr )
 	    cmd53_byte_block_count_nt <= cmd53_byte_block_count_nt + 1'b1;
      else if( sramclksync_txfifo_Almost_Full_pulse )
	    cmd53_byte_block_count_nt <= 10'b0;
  end

// to determine if the txfifo is filled correctly or not.
assign txfifo_byte_size_filled = cmd53_block_mode ? (block_mode_byte_size == cmd53_byte_block_count_nt) : 
                                                    (cmd53_byte_block_count_nt_registered == cmd53_byte_block_count_nt);

//this signal is used to increment the sram address lines if required
always@(posedge sram_clk or posedge irst)
  begin
    if( irst )
	begin
      sram_addr     <= 17'b0;
    end
	else
      //if( sram_onn )
	  //  sram_addr   <= 17'b0;
	  if ( ~sram_oen | sram_wen ) //else if ( ~sram_oen | ~sram_wen )
	  begin
        if( cmd53_OP_code )
          sram_addr <= sram_addr + 1'b1;
        else
		  sram_addr <= sram_addr;
	  end
	  else if( sramclksync_cmd53_52_received )
	    sram_addr   <= cmd52_53_reg_addr;
  end

//this signal is used to create the oen signals for sram interface
//data is latched on the rising edge of oen signal thus, txfifo_PUSH
//should be one sram_clk cycle after the sram_oen
always@(posedge sram_clk or posedge irst)
  begin
    if( irst )
	begin
      sram_oen      <= 1'b1;
	  txfifo_PUSH   <= 1'b0;
      txfifo_DIN    <= 8'b0;  
	end
	else
	begin
      txfifo_PUSH   <= ~sram_oen & ~sramclksync_cmd52_rd_after_wr_pulse_dly1;
	  txfifo_DIN    <= sram_data_in;
      if( (sramclksync_get_nxt_block_to_transmit & ~txfifo_byte_size_filled & sram_oen & ~sramclksync_cmd53_52_received) 
	      | (sramclksync_cmd52_rd_after_wr_pulse & |cmd52_53_func_num) )
	    sram_oen    <= 1'b0;
      else
	    sram_oen    <= 1'b1;
	end  
  end

//this signal is used to create the wen signals for sram interface
//data is output on the falling edge of wen signal thus, rxfifo_Pop
//should be one sram_clk cycle before the sram_wen
always@(posedge sram_clk or posedge irst)
  begin
    if( irst )
	begin
      sram_wen         <= 1'b1;
	  rxfifo_Pop       <= 1'b0;
      sram_data_out    <= 8'b0; 
	  //sram_data_oe     <= 1'b0; 
	end
	else
	begin
      sram_wen         <= ~rxfifo_Pop & cmd52_sram_wen;
	  //sram_data_oe     <= ~sram_wen | ~(~rxfifo_Pop & cmd52_sram_wen);

	  if( rxfifo_Pop )
	    sram_data_out  <= rxfifo_DOUT;
      else if( ~sramclksync_cmd52_write_pulse )
        sram_data_out  <= cmd52_write_data;
      
	  if( ~data_rxfifo_empty & (state_BUSY | state_spicard_rd_busy) & ~rxfifo_Pop )
	    rxfifo_Pop     <= 1'b1;
      else
	    rxfifo_Pop     <= 1'b0;
	end  
  end

assign sram_wen_out = sram_wen & ~sram_onn;
assign sram_oen_out = sram_oen & ~sram_onn;

assign sram_data_oe   = ~sram_wen & ~sram_onn;

assign cmd52_sram_wen = sramclksync_cmd52_write_pulse ? ~(|cmd52_53_func_num) : 1'b1;

//// FOR CMD52 ACCESSES ONLY
//this signal is used to create the oen signals for sram interface
//data is latched on the rising edge of oen signal thus, txfifo_PUSH
//should be one sram_clk cycle after the sram_oen
always@(posedge sram_clk or posedge irst)
  begin
    if( irst )
      cmd52_accessed_byte_register_sram <= 8'b0;  
	//else if( sramclksync_cmd52_rd_after_wr_pulse_dly2 )
	else if( sramclksync_cmd52_rd_after_wr_pulse_dly1 )
	  cmd52_accessed_byte_register_sram <= sram_data_in; 
  end

endmodule














