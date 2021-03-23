///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2007, Licensed customers of QuickLogic may copy or modify
// this file for use in designing QuickLogic devices only.
//
// Module Name:  sync
// File Name:    sync.v
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ns

module sync(           
                       reset,    // global reset    
					   spi_reset,
					   io_abort,
                       clk,
					   sram_clk,
                       
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
                       sramclksync_cmd53_52_received,

					   /////////////////////////////////
					   // CLK DOMAIN SIGNALS
					   /////////////////////////////////
					   clksync_txfifo_byte_size_filled,
                       cmd53_wr_rd_executing,
					   txfifo_Almost_Full_pulse,
					   get_nxt_block_to_transmit,
					   cmd52_rd_after_wr,
					   cmd52_write,
					   cmd53_52_received,
					   cmd52_53_func_num,

                       clksync_clear_abort_reg,
					   abort_reg_written 
					   );

    input         reset;     
	input         spi_reset;   
	input         io_abort;
    input         clk;
	input         sram_clk;
                       
	/////////////////////////////////
	// SRAM CLK DOMAIN SIGNALS
	/////////////////////////////////
    input         txfifo_byte_size_filled;
    output        sramclksync_icmd53_wr_rd_executing;
	output        sramclksync_txfifo_Almost_Full_pulse;
    output        sramclksync_get_nxt_block_to_transmit;
	output        sramclksync_cmd52_rd_after_wr;
	output        sramclksync_cmd52_rd_after_wr_pulse;
	output        sramclksync_cmd52_write_pulse;
	output        sramclksync_cmd52_rd_after_wr_pulse_dly1;
	output        sramclksync_cmd52_rd_after_wr_pulse_dly2;
	output        sramclksync_cmd53_52_received;

	/////////////////////////////////
    // CLK DOMAIN SIGNALS
	/////////////////////////////////
	output        clksync_txfifo_byte_size_filled;
    input         cmd53_wr_rd_executing;
	input         txfifo_Almost_Full_pulse;
	input         get_nxt_block_to_transmit;
	input         cmd52_rd_after_wr;
	input         cmd52_write;
	input         cmd53_52_received;
	input   [2:0] cmd52_53_func_num;

    output        clksync_clear_abort_reg;
	input         abort_reg_written; 

    wire       irst;
	wire       rst;

	reg        clksync_txfifo_byte_size_filled_sync1;
    reg        clksync_txfifo_byte_size_filled;
	reg        clksync_clear_abort_reg_sync1;
	reg        clksync_clear_abort_reg;

    reg        sramclksync_cmd52_write_sync1;
    reg        sramclksync_icmd53_wr_rd_executing_sync1;
	reg        sramclksync_txfifo_Almost_Full_pulse_sync1;
	reg        sramclksync_get_nxt_block_to_transmit_sync1;
	reg        sramclksync_cmd52_rd_after_wr_sync1;
    reg        sramclksync_icmd53_wr_rd_executing;
	reg        sramclksync_txfifo_Almost_Full_pulse;
    reg        sramclksync_get_nxt_block_to_transmit;
	reg        sramclksync_cmd52_rd_after_wr;
	reg        sramclksync_cmd52_write_pulse;
    reg        sramclksync_abort_reg_written_sync1;
    reg        sramclksync_abort_reg_written;

	reg        itxfifo_Almost_Full_pulse; 
	reg        icmd52_write;                    
	reg        itxfifo_Almost_Full_pulse_dly1; 
	reg        icmd52_write_dly1;              
	reg        itxfifo_Almost_Full_pulse_dly2;
	reg        icmd52_write_dly2;        
	
	reg        icmd53_52_received;
	reg        icmd53_52_received_dly1;
	reg        icmd53_52_received_dly2;    
	reg        sramclksync_cmd53_52_received_sync1;
	reg        sramclksync_cmd53_52_received;
	
    reg        sramclksync_cmd52_write_dly1;         
	reg        sramclksync_cmd52_rd_after_wr_dly1;       
	wire       sramclksync_cmd52_rd_after_wr_pulse;      
	reg        sramclksync_cmd52_rd_after_wr_pulse_dly1; 
	reg        sramclksync_cmd52_rd_after_wr_pulse_dly2;   
	reg        sramclksync_cmd52_write;

assign irst = reset | spi_reset | io_abort;
assign rst  = reset | spi_reset;

/////////////////////////////////				   
//sram_clk section logic
/////////////////////////////////
    //sync the different signals to sram_clk
	always@( posedge irst or posedge sram_clk )
	begin
      if(irst)
      begin
        sramclksync_icmd53_wr_rd_executing_sync1       <= 1'b0;
     	sramclksync_txfifo_Almost_Full_pulse_sync1     <= 1'b0; 
        sramclksync_get_nxt_block_to_transmit_sync1    <= 1'b0; 
        sramclksync_cmd52_rd_after_wr_sync1            <= 1'b0;
		sramclksync_cmd52_write_sync1                  <= 1'b0;
		sramclksync_cmd53_52_received_sync1            <= 1'b0;   
		sramclksync_icmd53_wr_rd_executing             <= 1'b0;
     	sramclksync_txfifo_Almost_Full_pulse           <= 1'b0; 
        sramclksync_get_nxt_block_to_transmit          <= 1'b0; 
        sramclksync_cmd52_rd_after_wr                  <= 1'b0;  
		sramclksync_cmd52_write                        <= 1'b0;
		sramclksync_cmd53_52_received                  <= 1'b0;
      end
      else
      begin
        sramclksync_icmd53_wr_rd_executing_sync1       <= cmd53_wr_rd_executing & |cmd52_53_func_num;      //icmd53_wr_rd_executing is being held high when executing so no need to prolong this signal
     	sramclksync_txfifo_Almost_Full_pulse_sync1     <= itxfifo_Almost_Full_pulse;  
        sramclksync_get_nxt_block_to_transmit_sync1    <= get_nxt_block_to_transmit;  //iget_nxt_block_to_transmit is being held high when executing so no need to prolong this signal
        sramclksync_cmd52_rd_after_wr_sync1            <= cmd52_rd_after_wr & ~icmd52_write;          //icmd52_rd_after_wr is being held high when executing so no need to prolong this signal
		sramclksync_cmd52_write_sync1                  <= icmd52_write;
		sramclksync_cmd53_52_received_sync1            <= icmd53_52_received;
		sramclksync_icmd53_wr_rd_executing             <= sramclksync_icmd53_wr_rd_executing_sync1;
     	sramclksync_txfifo_Almost_Full_pulse           <= sramclksync_txfifo_Almost_Full_pulse_sync1; 
        sramclksync_get_nxt_block_to_transmit          <= sramclksync_get_nxt_block_to_transmit_sync1; 
        sramclksync_cmd52_rd_after_wr                  <= sramclksync_cmd52_rd_after_wr_sync1; 
		sramclksync_cmd52_write                        <= sramclksync_cmd52_write_sync1;   
		sramclksync_cmd53_52_received                  <= sramclksync_cmd53_52_received_sync1;
       end
	end

    //sync the different signals to sram_clk
	always@( posedge rst or posedge sram_clk )
	begin
      if( rst )
      begin
        sramclksync_abort_reg_written_sync1            <= 1'b0;
        sramclksync_abort_reg_written                  <= 1'b0;
      end
      else
      begin
	    sramclksync_abort_reg_written_sync1            <= abort_reg_written;
		sramclksync_abort_reg_written                  <= sramclksync_abort_reg_written_sync1;
       end
	end

    //create the pulse signals 
	always@( posedge irst or posedge sram_clk )
	begin
      if(irst)
      begin
        sramclksync_cmd52_write_pulse              <= 1'b0;
		sramclksync_cmd52_write_dly1               <= 1'b0;
		sramclksync_cmd52_rd_after_wr_dly1         <= 1'b0;
		//sramclksync_cmd52_rd_after_wr_pulse        <= 1'b0;
        sramclksync_cmd52_rd_after_wr_pulse_dly1   <= 1'b0;
		sramclksync_cmd52_rd_after_wr_pulse_dly2   <= 1'b0;
      end
      else
      begin
        sramclksync_cmd52_write_dly1               <= sramclksync_cmd52_write;
        sramclksync_cmd52_write_pulse              <= sramclksync_cmd52_write & ~sramclksync_cmd52_write_dly1;
        sramclksync_cmd52_rd_after_wr_dly1         <= sramclksync_cmd52_rd_after_wr;
		//sramclksync_cmd52_rd_after_wr_pulse        <= sramclksync_cmd52_rd_after_wr & ~sramclksync_cmd52_rd_after_wr_dly1;
        sramclksync_cmd52_rd_after_wr_pulse_dly1   <= sramclksync_cmd52_rd_after_wr_pulse;
		sramclksync_cmd52_rd_after_wr_pulse_dly2   <= sramclksync_cmd52_rd_after_wr_pulse_dly1;
      end
	end

assign sramclksync_cmd52_rd_after_wr_pulse = sramclksync_cmd52_rd_after_wr & ~sramclksync_cmd52_rd_after_wr_dly1;

/////////////////////////////////				   
//clk section logic
/////////////////////////////////
    //sync the different signals to clk
	always@( posedge irst or posedge clk )
	begin
      if(irst)
      begin
        clksync_txfifo_byte_size_filled_sync1        <= 1'b0;
        clksync_txfifo_byte_size_filled              <= 1'b0;
      end
      else
      begin
        clksync_txfifo_byte_size_filled_sync1        <= txfifo_byte_size_filled;
        clksync_txfifo_byte_size_filled              <= clksync_txfifo_byte_size_filled_sync1;
      end
	end

    //sync the different signals to clk
	always@( posedge rst or posedge clk )
	begin
      if( rst )
      begin
		clksync_clear_abort_reg                      <= 1'b0;
		clksync_clear_abort_reg_sync1                <= 1'b0;
      end
      else
      begin
		clksync_clear_abort_reg_sync1                <= sramclksync_abort_reg_written;
		clksync_clear_abort_reg                      <= clksync_clear_abort_reg_sync1;
      end
	end

    //used to extend some of the signal flags depending on how slow sram_clk is to clk
	always@( posedge irst or posedge clk )
	begin
      if(irst)
      begin
     	itxfifo_Almost_Full_pulse       <= 1'b0; 
		icmd52_write                    <= 1'b0;
		itxfifo_Almost_Full_pulse_dly1  <= 1'b0; 
		icmd52_write_dly1               <= 1'b0;
		itxfifo_Almost_Full_pulse_dly2  <= 1'b0; 
		icmd52_write_dly2               <= 1'b0;
		icmd53_52_received              <= 1'b0;
		icmd53_52_received_dly1         <= 1'b0;
		icmd53_52_received_dly2         <= 1'b0;
      end
      else
      begin
	    icmd53_52_received_dly1         <= cmd53_52_received;
	  	itxfifo_Almost_Full_pulse_dly1  <= txfifo_Almost_Full_pulse; 
		icmd52_write_dly1               <= cmd52_write;
		icmd53_52_received_dly2         <= icmd53_52_received_dly1;
	  	itxfifo_Almost_Full_pulse_dly2  <= itxfifo_Almost_Full_pulse_dly1; 
		icmd52_write_dly2               <= icmd52_write_dly1;
        icmd53_52_received              <= cmd53_52_received | icmd53_52_received_dly1 | icmd53_52_received_dly2;
     	itxfifo_Almost_Full_pulse       <= txfifo_Almost_Full_pulse | itxfifo_Almost_Full_pulse_dly1 | itxfifo_Almost_Full_pulse_dly2; 
		icmd52_write                    <= cmd52_write | icmd52_write_dly1 | icmd52_write_dly2;
       end
	end




endmodule














