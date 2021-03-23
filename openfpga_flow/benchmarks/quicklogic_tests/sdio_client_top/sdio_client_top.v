///////////////////////////////////////////////////////////////////////////////
//
//
// Copyright (C) 2007, Licensed customers of QuickLogic may copy or modify
// this file for use in designing QuickLogic devices only.
//
// Module Name:  client_top_lvl
// File Name:    client_top_lvl.v
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ns

module sdio_client_top(
                     resetn, 
					 
					 /////////////////////////////////				   
				     //sdio interface
                     /////////////////////////////////
					 sdclk,
					 sdcmd,
					 sddat,

                     /////////////////////////////////				   
				     //sram interface
                     /////////////////////////////////
				     sram_csn2,
					 sram_csn1,
					 sram_wen,
					 sram_oen,
                     sram_addr,
					 sram_data,         
					 sram_resetn,
					 sram_lbn,
					 sram_ubn,
					 sram_nor_flash_wppn,
					 sram_nor_flash_ry_byn,

					 /////////////////////////////////				   
				     //misc
                     /////////////////////////////////
                     sram_onn,     //tie low
                     power_gd,     //tie high
					 osc_st,       //tie high
                     sd_pwr_onn,   //tie high
                     led,         //tie low
//					 sw,
					 cd_disable,
					 
					 debug           
                     );
                   
    
    input         resetn; //pragma attribute resetn nopad true
					 
    /////////////////////////////////				   
    //sdio interface
    /////////////////////////////////
    input         sdclk;
    inout         sdcmd;
    inout   [3:0] sddat;

    /////////////////////////////////				   
    //sram interface
    /////////////////////////////////
	output        sram_csn2;
    output        sram_csn1;
    output        sram_wen;
    output        sram_oen;
    output [20:0] sram_addr;
    inout   [7:0] sram_data;         
    output        sram_resetn;
    output   	  sram_lbn;
    output   	  sram_ubn;
	output        sram_nor_flash_wppn;
	input         sram_nor_flash_ry_byn;
    /////////////////////////////////				   
    //misc
    /////////////////////////////////
    output        sram_onn;     //tie low
    output        power_gd;     //tie high
    output        osc_st;       //tie high
    output        sd_pwr_onn;   //tie high
    output  [3:0] led;         //tie low
//    input   [3:0] sw;
	output        cd_disable;

	output [31:0] debug;

    
    /////////////////////////////////
	// WIRE DECLARATIONS
	/////////////////////////////////                                                     
    wire         clk;                            
    wire         clkn;
	wire         sdcmd;    
    //wire         high_speed;
	wire  [2:0]  resp4_max_num_func;
	wire  [23:0] resp4_ocr_reg;
	wire  [15:0] resp6_RCA;
	wire   [7:0] cmd52_accessed_byte_register;
	wire   [7:0] cmd52_accessed_byte_register_sram;
	wire   [7:0] cmd52_write_data;
    wire         cmd52_rd_after_wr;
	wire	     cmd52_write;
	wire  [16:0] cmd52_53_reg_addr;
	wire   [2:0] cmd52_53_func_num;
	wire         cmd3_create_new_RCA;
	wire         cmd52_53_rw_flag;  
    wire         cmd53_block_mode;    
    wire         cmd53_OP_code; 
    wire   [9:0] cmd53_byte_block_count_nt_registered;
	wire         cmd53_received;
    wire         dat_lines_being_used;
	//wire         cmd52_reset;
	//wire	     cmd52_abort;
	//wire         func_suspend;
	wire         exec_complete;
	//wire         func_resume;
	wire         cmd_txrx_cmd_idle;
	wire		 cmd_bus_idle_state;    
	wire         cmd_bus_init_state;
	wire         cmd_bus_stby_state;    
    wire         cmd_bus_cmd_state;      
    wire         cmd_bus_txfr_state;     
	wire         cmd_bus_inactive_state;
    wire         cmd53_wr;
    wire         cmd53_rd;
    wire   [7:0] cmd53_write_data;
    wire   [7:0] cmd53_read_data;
	wire   [7:0] cmd53_read_data_internal_reg;
	wire         cmd53_wr_rd_executing;
	wire   [3:0] sddat;
    wire         dat_width_4bit;
    wire   [9:0] current_cmd53_byte_block_count;
	wire   [9:0] block_mode_byte_size;
    wire         data_txfifo_empty;
    wire         data_txfifo_rdy;           // this is a pulse only signal!
    wire         data_rxfifo_full;
    wire         data_rxfifo_empty;
    wire         state_DAT_IDLE;
	wire         state_BUSY;
	wire   [9:0] func0_block_size;
	wire   [9:0] func1_block_size;
	wire   [9:0] func2_block_size;
	//wire   [9:0] func3_block_size;
	//wire   [9:0] func4_block_size;
	//wire   [9:0] func5_block_size;
	//wire   [9:0] func6_block_size;
	//wire   [9:0] func7_block_size;
	wire         spi_crc_check_en_bit;
	wire         sram_csn2;
    wire         sram_csn1;
    wire         sram_wen;
    wire         sram_oen;
	wire  [20:0] sram_addr;
    wire   [7:0] sram_data_out;     
    wire   [7:0] sram_data_in;      
    wire         sram_resetn;
	wire         sram_data_oe;
    wire   	     sram_lbn;
    wire   	     sram_ubn;
    wire         txfifo_byte_size_filled;
    wire         sramclksync_icmd53_wr_rd_executing;
	wire         sramclksync_txfifo_Almost_Full_pulse;
    wire         sramclksync_get_nxt_block_to_transmit;
	wire         sramclksync_cmd52_rd_after_wr;
	wire         sramclksync_cmd52_rd_after_wr_pulse;
	wire         sramclksync_cmd52_write_pulse;
	wire         sramclksync_cmd52_rd_after_wr_pulse_dly1;
	wire         sramclksync_cmd52_rd_after_wr_pulse_dly2;
	wire         clksync_txfifo_byte_size_filled;
	wire         txfifo_Almost_Full_pulse;
	wire         get_nxt_block_to_transmit;
	wire   [3:0] led_reg;
	wire         cmd53_52_received;
    wire         sramclksync_cmd53_52_received;

    reg          reset;
	wire         sdclk_int;
	wire         sdclkn_int;
    wire         sram_clk_int;
	reg          sram_clk_reg;

    wire         spi_reset;
	wire         spi_mode_on;                    
	wire         spi_cmdrsp_sdata;               
	wire         cmd_txrx_spicard_resp;          
	wire         all_blocks_written_out;         
	wire         spicard_wr2host_bit;            
	wire         spicard_rd_frm_host_bit;        
	wire         clear_spicard_rd_frm_host_bit;  
	wire         clear_spicard_wr2host_bit;
//	wire         spi_dat_err;                    
	wire         spi_stop_tran_cmd_rcvd;         
//	wire         out_of_range;                   
//	wire         error;                          
//	wire         spi_sdata;           
	wire         state_spicard_rd_busy;          

//    wire         cmd_end;
	wire         intr_period_cmd_end;
	wire         rsp_end;
	wire         func_num_err;
	wire   [5:0] cmd_index;
	wire         io_abort;
	wire         interrupt;
	wire         spi_csn;
	wire         cd_disable;

	wire   [2:0] cmd52_wr_io_abort_reg_data;
	wire         cmd52_wr_io_abort_reg;
	wire   [7:0] sram_data_out2;

//	wire         io_ready_func1;
//	wire         io_ready_func2;
//	wire         io_ready_func3;
//	wire         io_ready_func4;
//	wire         io_ready_func5;
//	wire         io_ready_func6;
//	wire         io_ready_func7;
	wire         io_en_func1;
	wire         io_en_func2;
//	wire         io_en_func3;
//	wire         io_en_func4;
//	wire         io_en_func5;
//	wire         io_en_func6;
//	wire         io_en_func7;
//	wire         go_cmd_state_func1;
//	wire         go_cmd_state_func2;
	//wire         go_cmd_state_func3;
	//wire         go_cmd_state_func4;
	//wire         go_cmd_state_func5;
	//wire         go_cmd_state_func6;
	//wire         go_cmd_state_func7;
	wire         abort_reg_written;
	wire         clksync_clear_abort_reg;
	wire         interrupt_sd4bit_en;
	wire         cmd_start;
	wire         sram_onn;

//	wire  [31:0] debug;

					   //// FOR testing purposes only
    wire      [3:0] dat_state;
    wire            all_bytes_received;
    wire            byte_end;
    wire      [9:0] i_cmd53_byte_block_count;
	wire     [12:0] dat_state_cnt;
    wire            sdata;
    wire            dout_reg;
    wire            oe_reg;
					   //// FOR testing purposes only

//assign sram_onn             = sram_onn;
assign sram_lbn             = 1'b0;
assign sram_ubn             = ~sram_onn;
assign sram_addr[20:17]     = 4'b0;   
assign power_gd             = 1'b1;     //tie high
assign osc_st               = 1'b0;     //tie low
assign sd_pwr_onn           = 1'b1;     //tie high
assign led                  = led_reg[3:0];     
assign sram_nor_flash_wppn  = 1'b0;

assign sram_data_in = (~sram_csn2 & ~|sram_addr)   ? sram_data_out2 : sram_data;   // to allow reading of the ry/Byn status bit
assign sram_data    = sram_data_oe                 ? sram_data_out  : 8'bz;

//assign i_resetn = resetn;

assign i_resetn = resetn;

//  gclkbuff buff_resetn(
 //   .A (resetn), 
  //  .Z (i_resetn)
  //);

// reset and clocks generated
  always@( posedge clk or negedge i_resetn )
	begin
  	  if( ~i_resetn )
        reset   <= 1'b1;
  	  else
	    reset   <= ~i_resetn;
    end

  always@( posedge clk or posedge reset )
	begin
  	  if( reset )
        sram_clk_reg   <= 1'b1;
  	  else
	    sram_clk_reg   <= ~sram_clk_reg;
    end

//  gclkbuff sd_clk_int_gbuf (
//    .A (sdclk), 
//    .Z (sdclk_int)
//  );
assign sdclk_int = sdclk;
/*
  gclkbuff sram_clk_int_gbuf (
    .A (sram_clk_reg), 
    .Z (sram_clk_int)
  );

  gclkbuff sd_clkn_int_gbuf (
    .A (sdclkn_int), 
    .Z (clkn)
  ); */

  assign clkn = sdclkn_int;
  assign sram_clk_int = sram_clk_reg;

assign sdclkn_int = ~clk; 
assign clk        = sdclk_int;
assign sram_clk   = sram_clk_int;
assign spi_csn    = sddat[3];
assign spi_reset  = reset | (spi_mode_on & spi_csn & ~spicard_rd_frm_host_bit);

/////////////////////////////////				   
//--- Block instantiations
/////////////////////////////////
cmd_control cmd_control_blk(          
                       .reset                                ( reset ),  
					   .spi_reset                            ( spi_reset ),     
                       .clk                                  ( clk ),                            
                       .clkn                                 ( clkn ), 

					   .cmd                                  ( sdcmd ),

                       //.high_speed                           ( high_speed ),                       
                       .resp4_max_num_func                   ( resp4_max_num_func ),
                       .resp4_ocr_reg                        ( resp4_ocr_reg ),
                       .resp6_RCA                            ( resp6_RCA ),
                       
					   .cmd52_accessed_byte_register         ( cmd52_accessed_byte_register ),
					   .cmd52_accessed_byte_register_sram    ( cmd52_accessed_byte_register_sram ),
                       .cmd52_write_data                     ( cmd52_write_data ),
                       .cmd52_rd_after_wr                    ( cmd52_rd_after_wr ),
					   .cmd52_write                          ( cmd52_write ),
					   .cmd3_create_new_RCA                  ( cmd3_create_new_RCA ),
					   .cmd53_52_received                    ( cmd53_52_received ),

                       // all of these are registered except for byte_block_count
                       .cmd52_53_rw_flag                     ( cmd52_53_rw_flag ),  
                       .cmd53_block_mode                     ( cmd53_block_mode ),    
                       .cmd53_OP_code                        ( cmd53_OP_code ), 
                       .cmd53_byte_block_count_nt_registered ( cmd53_byte_block_count_nt_registered ),
					   .cmd52_53_reg_addr                    ( cmd52_53_reg_addr ),
					   .cmd52_53_func_num                    ( cmd52_53_func_num ),
					   .cmd53_received                       ( cmd53_received ),
					   // end of registered signals
					   
					   .cmd_start                            ( cmd_start ),
                       .cmd_end                              ( cmd_end ),	
					   .rsp_end                              ( rsp_end ),
					   .intr_period_cmd_end                  ( intr_period_cmd_end ),
					   .func_num_err                         ( func_num_err ),	
					   .cmd_index                            ( cmd_index ),

					   .cmd52_wr_io_abort_reg_data           ( cmd52_wr_io_abort_reg_data ),
                       .cmd52_wr_io_abort_reg                ( cmd52_wr_io_abort_reg ),

					   .dat_lines_being_used                 ( dat_lines_being_used ),
					   //.cmd52_reset                          ( cmd52_reset ),
		               .cmd52_abort                          ( io_abort ), 
			           //.func_suspend                         ( func_suspend ), 
			           .exec_complete                        ( exec_complete ),
			           //.func_resume                          ( func_resume ),

                       .cmd_txrx_cmd_idle                    ( cmd_txrx_cmd_idle ),
			           // CMD BUS STATES
					   .cmd_bus_idle_state                   ( cmd_bus_idle_state ),    
	                   .cmd_bus_init_state                   ( cmd_bus_init_state ),
	                   .cmd_bus_stby_state                   ( cmd_bus_stby_state ),    
                       .cmd_bus_cmd_state                    ( cmd_bus_cmd_state ),      
                       .cmd_bus_txfr_state                   ( cmd_bus_txfr_state ),     
	                   .cmd_bus_inactive_state               ( cmd_bus_inactive_state ),

					   // SPI MODE
					   .spi_crc_check_en_bit                 ( spi_crc_check_en_bit ),
					   .spi_csn                              ( spi_csn ),
					   .spi_mode_on                          ( spi_mode_on ),               
					   .spi_cmdrsp_sdata                     ( spi_cmdrsp_sdata ),           
					   .cmd_txrx_spicard_resp                ( cmd_txrx_spicard_resp ),      
					   .all_blocks_written_out               ( all_blocks_written_out ),    
	                   .spicard_wr2host_bit                  ( spicard_wr2host_bit ),        
                       .spicard_rd_frm_host_bit              ( spicard_rd_frm_host_bit ),  
					   .clear_spicard_rd_frm_host_bit        ( clear_spicard_rd_frm_host_bit ),  
					   .clear_spicard_wr2host_bit            ( clear_spicard_wr2host_bit ),
					   .spi_dat_err                          ( spi_dat_err ),        
					   .spi_stop_tran_cmd_rcvd               ( spi_stop_tran_cmd_rcvd ),  
					   .out_of_range                         ( out_of_range ),                
					   .error                                ( error )                      

                   );

registers registers_blk(          
                       .reset                              ( reset ),    
                       .clk                                ( clk ),                            
                           
					   //.high_speed                         ( high_speed ),	                    
                       .resp4_max_num_func                 ( resp4_max_num_func ),
                       .resp4_ocr_reg                      ( resp4_ocr_reg ),
                       .resp6_RCA                          ( resp6_RCA ),
                       
					   .cmd52_accessed_byte_register       ( cmd52_accessed_byte_register ),
                       .cmd52_write_data                   ( cmd52_write_data ),
                       .cmd52_rd_after_wr                  ( cmd52_rd_after_wr ),
					   .cmd52_write                        ( cmd52_write ),
					   .cmd52_53_reg_addr                  ( cmd52_53_reg_addr ),
					   .cmd52_53_func_num                  ( cmd52_53_func_num ),
                       .cmd3_create_new_RCA                ( cmd3_create_new_RCA ),
					   .cmd53_OP_code                      ( cmd53_OP_code ),
					   .cmd52_53_rw_flag                   ( cmd52_53_rw_flag ),
					   .cmd53_block_mode                   ( cmd53_block_mode ),
					   .cmd53_52_received                  ( cmd53_52_received ),
					   .cmd_start                          ( cmd_start ),

					   .cmd52_wr_io_abort_reg_data         ( cmd52_wr_io_abort_reg_data ),
                       .cmd52_wr_io_abort_reg              ( cmd52_wr_io_abort_reg ),

                       .interrupt                          ( interrupt ),
					   .interrupt_sd4bit_en                ( interrupt_sd4bit_en ),
                       .spi_mode_on                        ( spi_mode_on ),
					   .spi_csn                            ( spi_csn ),
					   .cd_disable                         ( cd_disable ),

					   // output register write, read signals going to txfifo.v and rxfifo.v
					   // to properly execute CMD 53 byte or block mode reads.
					   .cmd53_wr                           ( cmd53_wr ),
					   .cmd53_rd                           ( cmd53_rd ),
					   .cmd53_write_data                   ( cmd53_write_data ),
					   .cmd53_read_data_internal_reg       ( cmd53_read_data_internal_reg ),
					   .cmd53_wr_rd_executing              ( cmd53_wr_rd_executing ),

					   .dat_lines_being_used               ( dat_lines_being_used ),
					   .dat_width_4bit                     ( dat_width_4bit ),
					   //.cmd52_reset                        ( cmd52_reset ),
		               //.cmd52_abort                        ( cmd52_abort ), 
			           //.func_suspend                       ( func_suspend ), 
			           .exec_complete                      ( exec_complete ),
			           //.func_resume                        ( func_resume ),
 
                       .func0_block_size                   ( func0_block_size ),
					   .func1_block_size                   ( func1_block_size ),
					   .func2_block_size                   ( func2_block_size ),
					   //.func3_block_size                   ( func3_block_size ),
					   //.func4_block_size                   ( func4_block_size ),
					   //.func5_block_size                   ( func5_block_size ),
					   //.func6_block_size                   ( func6_block_size ),
					   //.func7_block_size                   ( func7_block_size ),
					   //.spi_crc_check_en_bit               ( spi_crc_check_en_bit ),

			           // CMD BUS STATES
					   .cmd_bus_idle_state                 ( cmd_bus_idle_state ),    
	                   .cmd_bus_init_state                 ( cmd_bus_init_state ),
	                   .cmd_bus_stby_state                 ( cmd_bus_stby_state ),    
                       .cmd_bus_cmd_state                  ( cmd_bus_cmd_state ),      
                       .cmd_bus_txfr_state                 ( cmd_bus_txfr_state ),     
	                   .cmd_bus_inactive_state             ( cmd_bus_inactive_state ),
					   
					   .led_reg                            ( led_reg ), 
					   .sram_onn                           ( sram_onn ),

					   	   //function inputs / outputs
                       .io_ready_func1                     ( io_en_func1 ),
                       .io_ready_func2                     ( io_en_func2 ),
                       //.io_ready_func3                     ( 1'b0 ),
                       //.io_ready_func4                     ( 1'b0 ),
                       //.io_ready_func5                     ( 1'b0 ),
                       //.io_ready_func6                     ( 1'b0 ),
                       //.io_ready_func7                     ( 1'b0 ),
					   ////int_pending_func1,
	                   ////int_pending_func2,
	                   ////int_pending_func3,
	                   ////int_pending_func4,
	                   ////int_pending_func5,
					   ////int_pending_func6,
	                   ////int_pending_func7,
                       .io_en_func1                        ( io_en_func1 ),
	                   .io_en_func2                        ( io_en_func2 ),
	                   //.io_en_func3                        ( io_en_func3 ),
	                   //.io_en_func4                        ( io_en_func4 ),
	                   //.io_en_func5                        ( io_en_func5 ),
	                   //.io_en_func6                        ( io_en_func6 ),
	                   //.io_en_func7                        ( io_en_func7 ),
					   .go_cmd_state_func1                 ( go_cmd_state_func1 ),
                       .go_cmd_state_func2                 ( go_cmd_state_func2 ),
                       //.go_cmd_state_func3                 ( go_cmd_state_func3 ),
                       //.go_cmd_state_func4                 ( go_cmd_state_func4 ),
                       //.go_cmd_state_func5                 ( go_cmd_state_func5 ),
                       //.go_cmd_state_func6                 ( go_cmd_state_func6 ),
	                   //.go_cmd_state_func7                 ( go_cmd_state_func7 ),
					   .io_abort                           ( io_abort ),
					   
					   .abort_reg_written                  ( abort_reg_written ),
					   .clksync_clear_abort_reg            ( clksync_clear_abort_reg )
                   );

      
dat_control dat_control_blk(    
                       .reset                                   ( reset ),   
					   .spi_reset                               ( spi_reset ),
					   .io_abort                                ( io_abort ),
                       .clk                                     ( clk ),      
					   .clkn                                    ( clkn ),                      

                       .dat                                     ( sddat ),                           

					   //.high_speed                              ( high_speed ),	            
					   .dat_width_4bit                          ( dat_width_4bit ),        
                       
  					   .interrupt                               ( interrupt ),
					   .interrupt_sd4bit_en                     ( interrupt_sd4bit_en ),
	                   .rsp_end                                 ( rsp_end ),
	                   .intr_period_cmd_end                     ( intr_period_cmd_end ),
	                   .func_num_err                            ( func_num_err ),
	                   .cmd_index                               ( cmd_index ),
					   .cmd53_52_received                       ( cmd53_52_received ),

					   // command 53 flag bits to determine read, write, modes, address incr
                       // all of these are registered except for byte_block_count
                       .cmd52_53_rw_flag                        ( cmd52_53_rw_flag ),  
                       .cmd53_block_mode                        ( cmd53_block_mode ),    
                       .cmd53_OP_code                           ( cmd53_OP_code ), 
                       .cmd53_byte_block_count_nt_registered    ( cmd53_byte_block_count_nt_registered ),
					   .cmd52_53_reg_addr                       ( cmd52_53_reg_addr ),
					   .cmd52_53_func_num                       ( cmd52_53_func_num ),
					   .cmd53_received                          ( cmd53_received ),
					   .cmd53_wr_rd_executing                   ( cmd53_wr_rd_executing ),
					   .block_mode_byte_size                    ( block_mode_byte_size ),
					   // end of registered signals

                       //fifo signal flags
                       .data_txfifo_empty                       ( data_txfifo_empty ),
                       .data_txfifo_rdy                         ( data_txfifo_rdy ),  
                       .data_rxfifo_full                        ( data_rxfifo_full ),
                       .data_rxfifo_empty                       ( data_rxfifo_empty ),

                       .current_cmd53_byte_block_count          ( current_cmd53_byte_block_count ),

                       .func0_block_size                        ( func0_block_size ),
					   .func1_block_size                        ( func1_block_size ),
					   .func2_block_size                        ( func2_block_size ),
					   //.func3_block_size                        ( func3_block_size ),
					   //.func4_block_size                        ( func4_block_size ),
					   //.func5_block_size                        ( func5_block_size ),
					   //.func6_block_size                        ( func6_block_size ),
					   //.func7_block_size                        ( func7_block_size ),
					   .spi_crc_check_en_bit                    ( spi_crc_check_en_bit ),
                     
                       // output register write, read signals going to txfifo.v and rxfifo.v
					   // to properly execute CMD 53 byte or block mode reads.
					   .cmd53_wr                                ( cmd53_wr ),
					   .cmd53_rd                                ( cmd53_rd ),
					   .cmd53_write_data                        ( cmd53_write_data ),
					   .cmd53_read_data                         ( cmd53_read_data ),
					   .cmd53_read_data_internal_reg            ( cmd53_read_data_internal_reg ),

                       // MISC
					   .dat_lines_being_used                    ( dat_lines_being_used ),
					   //.cmd52_reset                             ( cmd52_reset ),
		               //.cmd52_abort                             ( cmd52_abort ), 
			           //.func_suspend                            ( func_suspend ), 
			           .exec_complete                           ( exec_complete ),
			           //.func_resume                             ( func_resume ),
					   .cmd52_rd_after_wr                       ( cmd52_rd_after_wr ),

					   //DATA transmission state
	                   .state_DAT_IDLE                          ( state_DAT_IDLE ),
					   .state_BUSY                              ( state_BUSY ),

                       .cmd_txrx_cmd_idle                       ( cmd_txrx_cmd_idle ),
			           // CMD BUS STATES
					   //.cmd_bus_idle_state                      ( cmd_bus_idle_state ),    
	                   //.cmd_bus_init_state                      ( cmd_bus_init_state ),
	                   //.cmd_bus_stby_state                      ( cmd_bus_stby_state ),    
                       //.cmd_bus_cmd_state                       ( cmd_bus_cmd_state ),      
                       //.cmd_bus_txfr_state                      ( cmd_bus_txfr_state ),     
	                   //.cmd_bus_inactive_state                  ( cmd_bus_inactive_state ),
					   
					   //SPI mode
					   .spi_csn                                 ( spi_csn ),              
					   .spi_mode_on                             ( spi_mode_on ),        
					   .spi_sdata                               ( spi_sdata ),
					   .sdcmd                                   ( sdcmd ),            
					   .cmd_txrx_spicard_resp                   ( cmd_txrx_spicard_resp ),    
					   .all_blocks_written_out                  ( all_blocks_written_out ),    
	                   .spicard_wr2host_bit                     ( spicard_wr2host_bit ),   
                       .spicard_rd_frm_host_bit                 ( spicard_rd_frm_host_bit ),
					   .clear_spicard_rd_frm_host_bit           ( clear_spicard_rd_frm_host_bit ),
					   .clear_spicard_wr2host_bit               ( clear_spicard_wr2host_bit ),
					   .spi_stop_tran_cmd_rcvd                  ( spi_stop_tran_cmd_rcvd ),
					   .state_spicard_rd_busy                   ( state_spicard_rd_busy ),
					   .spi_cmdrsp_sdata                        ( spi_cmdrsp_sdata )

					   //// FOR testing purposes only
					   //.dat_state( dat_state ),
                       //.all_bytes_received( all_bytes_received ),
                       //.byte_end( byte_end ),
                       //.i_cmd53_byte_block_count( i_cmd53_byte_block_count ),
					   //.dat_state_cnt( dat_state_cnt ),
                       //.sdata(sdata),
                       //.dout_reg(dout_reg),
                       //.oe_reg(oe_reg)
					   //// FOR testing purposes only
                   );

dat_fifo dat_fifo_blk(          
                       .reset                                  ( reset ),   
					   .spi_reset                              ( spi_reset ),
					   .io_abort                               ( io_abort ),    
                       .clk                                    ( clk ),
					   .sram_clk                               ( sram_clk ),
                       //fifo signal flags
                       .data_txfifo_empty                      ( data_txfifo_empty ),
                       .data_txfifo_rdy                        ( data_txfifo_rdy ),  
                       .data_rxfifo_full                       ( data_rxfifo_full ),
                       .data_rxfifo_empty                      ( data_rxfifo_empty ),

                       /////////////////////////////////
                       //register interface
					   /////////////////////////////////
                       //.high_speed                              ( high_speed ),	
					   .dat_width_4bit                          ( dat_width_4bit ),                    
                       .resp4_max_num_func                      ( resp4_max_num_func ),
                       .resp4_ocr_reg                           ( resp4_ocr_reg ),
                       .resp6_RCA                               ( resp6_RCA ),
					   //.cmd52_reset                             ( cmd52_reset ),
		               //.cmd52_abort                             ( cmd52_abort ), 
			           //.func_suspend                            ( func_suspend ), 
			           //.func_resume                             ( func_resume ),
 
                       //.func0_block_size                        ( func0_block_size ),
					   //.func1_block_size                        ( func1_block_size ),
					   //.func2_block_size                        ( func2_block_size ),
					   //.func3_block_size                        ( func3_block_size ),
					   //.func4_block_size                        ( func4_block_size ),
					   //.func5_block_size                        ( func5_block_size ),
					   //.func6_block_size                        ( func6_block_size ),
					   //.func7_block_size                        ( func7_block_size ),

                       /////////////////////////////////
                       //dat control interface   
                       /////////////////////////////////
                       .current_cmd53_byte_block_count          ( current_cmd53_byte_block_count ),                
                       // output register write, read signals going to txfifo.v and rxfifo.v
					   // to properly execute CMD 53 byte or block mode reads.
					   .cmd53_wr                                ( cmd53_wr ),
					   .cmd53_rd                                ( cmd53_rd ),
					   .cmd53_write_data                        ( cmd53_write_data ),
					   .cmd53_read_data                         ( cmd53_read_data ),
                       // MISC
					   .dat_lines_being_used                    ( dat_lines_being_used ),
					   .exec_complete                           ( exec_complete ),
					   .cmd53_wr_rd_executing                   ( cmd53_wr_rd_executing ),
					   .block_mode_byte_size                    ( block_mode_byte_size ),
			           //DATA transmission state
	                   .state_DAT_IDLE                          ( state_DAT_IDLE ),     
					   .state_BUSY                              ( state_BUSY ), 
					   .state_spicard_rd_busy                   ( state_spicard_rd_busy ),          
				   
				   	   /////////////////////////////////
				       //cmd control interface          
					   /////////////////////////////////                               
					   .cmd52_accessed_byte_register_sram       ( cmd52_accessed_byte_register_sram ),
                       .cmd52_write_data                        ( cmd52_write_data ),
                       .cmd52_rd_after_wr                       ( cmd52_rd_after_wr ),
					   .cmd52_write                             ( cmd52_write ),
                       // all of these are registered except for byte_block_count
					   // assume these signals do not change once written so save on sync registers
                       .cmd52_53_rw_flag                        ( cmd52_53_rw_flag ),   
                       .cmd53_block_mode                        ( cmd53_block_mode ),    
                       .cmd53_OP_code                           ( cmd53_OP_code ), 
                       .cmd53_byte_block_count_nt_registered    ( cmd53_byte_block_count_nt_registered ),
					   .cmd52_53_reg_addr                       ( cmd52_53_reg_addr ),
					   .cmd52_53_func_num                       ( cmd52_53_func_num ),
					   .cmd53_received                          ( cmd53_received ),
					   // end of registered signals
					   // CMD BUS STATES
					   .cmd_bus_idle_state                      ( cmd_bus_idle_state ),    
	                   .cmd_bus_init_state                      ( cmd_bus_init_state ),
	                   .cmd_bus_stby_state                      ( cmd_bus_stby_state ),    
                       .cmd_bus_cmd_state                       ( cmd_bus_cmd_state ),      
                       .cmd_bus_txfr_state                      ( cmd_bus_txfr_state ),     
	                   .cmd_bus_inactive_state                  ( cmd_bus_inactive_state ), 
				   
                       /////////////////////////////////				   
				       //sram interface
                       /////////////////////////////////
				       //.sram_csn7                               ( sram_csn7 ),
					   //.sram_csn6                               ( sram_csn6 ),
					   //.sram_csn5                               ( sram_csn5 ),
					   //.sram_csn4                               ( sram_csn4 ),
					   //.sram_csn3                               ( sram_csn3 ),
					   //.sram_csn2                               ( sram_csn2 ),
					   //.sram_csn1                               ( sram_csn1 ),
					   .sram_wen_out                            ( sram_wen ),
					   .sram_oen_out                            ( sram_oen ),
                       .sram_addr                               ( sram_addr[16:0] ),
					   .sram_data_out                           ( sram_data_out ),     
                       .sram_data_in                            ( sram_data_in ),      
					   .sram_resetn                             ( sram_resetn ),
					   .sram_data_oe                            ( sram_data_oe ),
					   .sram_onn                                ( sram_onn ),

                       /////////////////////////////////
					   // SRAM CLK DOMAIN SIGNALS
					   /////////////////////////////////
                       .txfifo_byte_size_filled                 ( txfifo_byte_size_filled ),
                       .sramclksync_icmd53_wr_rd_executing      ( sramclksync_icmd53_wr_rd_executing ),
					   .sramclksync_txfifo_Almost_Full_pulse    ( sramclksync_txfifo_Almost_Full_pulse ),
                       .sramclksync_get_nxt_block_to_transmit   ( sramclksync_get_nxt_block_to_transmit ),
					   .sramclksync_cmd52_rd_after_wr           ( sramclksync_cmd52_rd_after_wr ),
					   .sramclksync_cmd52_rd_after_wr_pulse     ( sramclksync_cmd52_rd_after_wr_pulse ),
					   .sramclksync_cmd52_write_pulse           ( sramclksync_cmd52_write_pulse ),
					   .sramclksync_cmd52_rd_after_wr_pulse_dly1 ( sramclksync_cmd52_rd_after_wr_pulse_dly1 ),
					   .sramclksync_cmd52_rd_after_wr_pulse_dly2 ( sramclksync_cmd52_rd_after_wr_pulse_dly2 ),
                       .sramclksync_cmd53_52_received           ( sramclksync_cmd53_52_received ),
					   
					   /////////////////////////////////
					   // CLK DOMAIN SIGNALS
					   /////////////////////////////////
					   .clksync_txfifo_byte_size_filled         ( clksync_txfifo_byte_size_filled ),
					   .txfifo_Almost_Full_pulse                ( txfifo_Almost_Full_pulse ),
					   .get_nxt_block_to_transmit               ( get_nxt_block_to_transmit )
					   );

sync sync_blk(           
                       .reset                                       ( reset ),   
					   .spi_reset                                   ( spi_reset ),
					   .io_abort                                    ( io_abort ),
                       .clk                                         ( clk ),
					   .sram_clk                                    ( sram_clk ),
                       
                       /////////////////////////////////
					   // SRAM CLK DOMAIN SIGNALS
					   /////////////////////////////////
                       .txfifo_byte_size_filled                     ( txfifo_byte_size_filled ),
                       .sramclksync_icmd53_wr_rd_executing          ( sramclksync_icmd53_wr_rd_executing ),
					   .sramclksync_txfifo_Almost_Full_pulse        ( sramclksync_txfifo_Almost_Full_pulse ),
                       .sramclksync_get_nxt_block_to_transmit       ( sramclksync_get_nxt_block_to_transmit ),
					   .sramclksync_cmd52_rd_after_wr               ( sramclksync_cmd52_rd_after_wr ),
					   .sramclksync_cmd52_rd_after_wr_pulse         ( sramclksync_cmd52_rd_after_wr_pulse ),
					   .sramclksync_cmd52_write_pulse               ( sramclksync_cmd52_write_pulse ), 
					   .sramclksync_cmd52_rd_after_wr_pulse_dly1    ( sramclksync_cmd52_rd_after_wr_pulse_dly1 ),
					   .sramclksync_cmd52_rd_after_wr_pulse_dly2    ( sramclksync_cmd52_rd_after_wr_pulse_dly2 ),
                       .sramclksync_cmd53_52_received               ( sramclksync_cmd53_52_received ),
					   
					   /////////////////////////////////
					   // CLK DOMAIN SIGNALS
					   /////////////////////////////////
					   .clksync_txfifo_byte_size_filled             ( clksync_txfifo_byte_size_filled ),
                       .cmd53_wr_rd_executing                       ( cmd53_wr_rd_executing ),
					   .txfifo_Almost_Full_pulse                    ( txfifo_Almost_Full_pulse ),
					   .get_nxt_block_to_transmit                   ( get_nxt_block_to_transmit ),
					   .cmd52_rd_after_wr                           ( cmd52_rd_after_wr ),
					   .cmd52_write                                 ( cmd52_write ),
					   .cmd53_52_received                           ( cmd53_52_received ),
					   .cmd52_53_func_num                           ( cmd52_53_func_num ),

					   .clksync_clear_abort_reg                     ( clksync_clear_abort_reg ),
					   .abort_reg_written                           ( abort_reg_written )
					   );

function1 function1_blk(
                       .cmd52_53_func_num                           ( cmd52_53_func_num ),
		               .io_en_func1                                 ( io_en_func1 ),
		               .sram_resetn1                                ( sram_resetn ),
                       .sram_wen1                                   ( sram_wen ),
	                   .sram_oen1                                   ( sram_oen ),
                       .sram_addr1                                  ( sram_addr[16:0] ),
	                   .sram_data_in1                               ( sram_data_in ),  
		               .sram_data_out1                              (  ), 
		               .sram_csn1                                   ( sram_csn1 ),
					   .sram_onn      	                            ( sram_onn )
                       );

function2 function2_blk(
                       .cmd52_53_func_num                           ( cmd52_53_func_num ),
		               .io_en_func2                                 ( io_en_func2 ),
		               .sram_resetn2                                ( sram_resetn ),
                       .sram_wen2                                   ( sram_wen ),
	                   .sram_oen2                                   ( sram_oen ),
                       .sram_addr2                                  ( sram_addr[16:0] ),
	                   .sram_data_in2                               ( sram_data_in ),  
		               .sram_data_out2                              ( sram_data_out2 ), 
		               .sram_csn2                                   ( sram_csn2 ),
					   .sram_onn      	                            ( sram_onn ),

					   .sram_nor_flash_ry_byn                       ( sram_nor_flash_ry_byn )      	     
                       );

//// FOR testing purposes only
/*assign debug[24]   = sdata;
assign debug[23]   = oe_reg;
assign debug[22]   = dout_reg;
assign debug[21:9] = dat_state_cnt;
assign debug[8]    = |i_cmd53_byte_block_count;
assign debug[7]    = cmd53_wr_rd_executing;
assign debug[6]    = byte_end;
assign debug[5]    = all_bytes_received;
assign debug[4]    = data_rxfifo_empty;
assign debug[3:0]  = dat_state;
//// FOR testing purposes only
*/
endmodule





