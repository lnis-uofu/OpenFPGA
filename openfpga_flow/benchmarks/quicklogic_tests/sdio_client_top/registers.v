///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2007, Licensed customers of QuickLogic may copy or modify
// this file for use in designing QuickLogic devices only.
//
// Module Name:  registers
// File Name:    registers.v
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ns

module registers(           
                       reset,    // global reset    
                       clk,                            
                           
					   //high_speed,	                    
                       resp4_max_num_func,
                       resp4_ocr_reg,
                       resp6_RCA,
                       
					   cmd52_accessed_byte_register,
                       cmd52_write_data,
                       cmd52_rd_after_wr,
					   cmd52_write,
					   cmd52_53_reg_addr,
					   cmd52_53_func_num,
                       cmd3_create_new_RCA,
					   cmd53_OP_code,
					   cmd52_53_rw_flag,
					   cmd53_block_mode,
					   cmd53_52_received,
					   cmd_start,

	                   cmd52_wr_io_abort_reg_data,
	                   cmd52_wr_io_abort_reg,

					   interrupt,
					   interrupt_sd4bit_en,
					   spi_mode_on,
					   spi_csn,
					   cd_disable,
					   // output register write, read signals going to txfifo.v and rxfifo.v
					   // to properly execute CMD 53 byte or block mode reads.
					   cmd53_wr,
					   cmd53_rd,
					   cmd53_write_data,
					   cmd53_read_data_internal_reg,
					   cmd53_wr_rd_executing,

					   dat_lines_being_used,
					   dat_width_4bit,
					   //cmd52_reset,
		               //cmd52_abort, 
			           //func_suspend, 
			           exec_complete,
			           //func_resume,

					   func0_block_size,
					   func1_block_size,
					   func2_block_size,
					   //func3_block_size,
					   //func4_block_size,
					   //func5_block_size,
					   //func6_block_size,
					   //func7_block_size,
					   //spi_crc_check_en_bit,

			           // CMD BUS STATES
					   cmd_bus_idle_state,    
	                   cmd_bus_init_state,
	                   cmd_bus_stby_state,    
                       cmd_bus_cmd_state,      
                       cmd_bus_txfr_state,     
	                   cmd_bus_inactive_state,
					   
					   led_reg,
					   sram_onn,
					   
					   //function inputs / outputs
                       io_ready_func1,
                       io_ready_func2,
                       //io_ready_func3,
                       //io_ready_func4,
                       //io_ready_func5,
                       //io_ready_func6,
                       //io_ready_func7,
					   ////int_pending_func1,
	                   ////int_pending_func2,
	                   ////int_pending_func3,
	                   ////int_pending_func4,
	                   ////int_pending_func5,
					   ////int_pending_func6,
	                   ////int_pending_func7,
                       io_en_func1,
	                   io_en_func2,
	                   //io_en_func3,
	                   //io_en_func4,
	                   //io_en_func5,
	                   //io_en_func6,
	                   //io_en_func7,
					   go_cmd_state_func1,
                       go_cmd_state_func2,
                       //go_cmd_state_func3,
                       //go_cmd_state_func4,
                       //go_cmd_state_func5,
                       //go_cmd_state_func6,
	                   //go_cmd_state_func7,
					   io_abort,
					   
					   abort_reg_written,
					   clksync_clear_abort_reg  
                   );

    input         reset;                                                      
    input         clk;                            

    //output        high_speed;
	output  [2:0] resp4_max_num_func;
	output [23:0] resp4_ocr_reg;
	output [15:0] resp6_RCA;

	output  [7:0] cmd52_accessed_byte_register;
	input   [7:0] cmd52_write_data;
    input         cmd52_rd_after_wr;
	input	 	  cmd52_write;
	input  [16:0] cmd52_53_reg_addr;
	input   [2:0] cmd52_53_func_num;
    input         cmd3_create_new_RCA;
	input         cmd53_OP_code;
	input         cmd52_53_rw_flag;
	input         cmd53_block_mode;
	input         cmd53_52_received;
	input         cmd_start;

	input   [2:0] cmd52_wr_io_abort_reg_data;
	input         cmd52_wr_io_abort_reg;

    output        interrupt;
	output        interrupt_sd4bit_en;
	output        cd_disable;     //to control external cd resistor
	input         spi_mode_on;
	input         spi_csn;
	// output register addr, write, read signals going to data fifo
    // to properly execute CMD 53 byte or block mode reads.
    input         cmd53_wr;
    input         cmd53_rd;
    input   [7:0] cmd53_write_data;
    output  [7:0] cmd53_read_data_internal_reg;
	input         cmd53_wr_rd_executing;

    input         dat_lines_being_used;
	output        dat_width_4bit;
	//output        cmd52_reset;
	//output		  cmd52_abort;
	//output        func_suspend;
	input         exec_complete;
	//output        func_resume;

	output  [3:0] led_reg;
	output        sram_onn;

    output  [9:0] func0_block_size;
	output  [9:0] func1_block_size;
	output  [9:0] func2_block_size;
	//output  [9:0] func3_block_size;
	//output  [9:0] func4_block_size;
	//output  [9:0] func5_block_size;
	//output  [9:0] func6_block_size;
	//output  [9:0] func7_block_size;
//	output        spi_crc_check_en_bit;

	input		  cmd_bus_idle_state;    
	input         cmd_bus_init_state;
	input         cmd_bus_stby_state;    
    input         cmd_bus_cmd_state;      
    input         cmd_bus_txfr_state;     
	input         cmd_bus_inactive_state; 

//function inputs / outputs
    input         io_ready_func1;
    input         io_ready_func2;
    //input         io_ready_func3;
    //input         io_ready_func4;
    //input         io_ready_func5;
    //input         io_ready_func6;
    //input         io_ready_func7;
////	input         int_pending_func1;
////	input         int_pending_func2;
////	input         int_pending_func3;
////	input         int_pending_func4;
////	input         int_pending_func5;
////	input         int_pending_func6;
////	input         int_pending_func7;
    output        io_en_func1;
	output        io_en_func2;
	//output        io_en_func3;
	//output        io_en_func4;
	//output        io_en_func5;
	//output        io_en_func6;
	//output        io_en_func7;
	output        go_cmd_state_func1;
    output        go_cmd_state_func2;
    //output        go_cmd_state_func3;
    //output        go_cmd_state_func4;
    //output        go_cmd_state_func5;
    //output        go_cmd_state_func6;
	//output        go_cmd_state_func7;
	output        io_abort;

	output        abort_reg_written;
	input         clksync_clear_abort_reg;

//--- Declaration of wires and regs
    wire          irst;

    // read, write, address, data signals
    wire          iwr;
	wire          ird;
	reg           wr;
	reg           rd;
	reg    [16:0] addr;
    reg     [7:0] read_byte_register;
	wire    [7:0] iwrite_byte_register;
	reg     [7:0] write_byte_register;

    //registers
	reg     [6:0] iresp6_RCA;
	wire    [3:0] cccr_rev;           //hardwired
    wire    [3:0] sdio_rev;           //hardwired
    wire    [3:0] sd_format_ver_num;  //hardwired
	reg     [2:1] int_enable_func;
	reg           int_enable_master;
	reg     [3:0] led_reg;
	reg           sram_onn;
	reg           io_card_reset; 
	reg     [2:0] abort_func_num;
	reg           cd_disable;
	wire          continuous_spi_int_support;
	reg           continuous_spi_int_en;
	reg     [1:0] dat_bus_width;
	wire          low_speed_4bit_support;
	wire          low_speed_card;
	reg           interrupt_sd4bit_en;
	wire          interrupt_sd4bit_support;
	wire          suspend_resume_support;
	wire          read_wait_support; 
	wire          multi_block_support; 
	wire          direct_cmd_support; 
	wire          bus_status;
	wire          bus_release_status;
	wire          resume_dat_flag; 
	wire    [3:0] func_bit_sel;
	wire    [7:0] execution_flags;
	wire    [7:0] ready_flags;
	reg     [7:0] block_size_reg_lb;
	reg     [1:0] block_size_reg_hb;
	reg           master_pwr_contrl_en; 
	wire          master_pwr_control_support;
    reg           io_en_func1;
	reg           io_en_func2;
	//reg           io_en_func3;
	//reg           io_en_func4;
	//reg           io_en_func5;
	//reg           io_en_func6;
	//reg           io_en_func7;
    wire          internal_int;
	reg           abort_reg_written;

	reg     [2:1] interrupt_pending_test;
	reg     [2:0] prv_cmd52_53_func_num;

    wire    [2:1] func_int;
    wire    [9:0] func0_block_size;
	wire    [9:0] func1_block_size;
	wire    [9:0] func2_block_size;
	//wire    [9:0] func3_block_size;
	//wire    [9:0] func4_block_size;
	//wire    [9:0] func5_block_size;
	//wire    [9:0] func6_block_size;
	//wire    [9:0] func7_block_size;
    wire          go_cmd_state_func1;
    wire          go_cmd_state_func2;
    //wire          go_cmd_state_func3;
    //wire          go_cmd_state_func4;
    //wire          go_cmd_state_func5;
    //wire          go_cmd_state_func6;
	//wire          go_cmd_state_func7;

	wire          int_pending_func1;
	wire          int_pending_func2;
	//wire          int_pending_func3;
	//wire          int_pending_func4;
	//wire          int_pending_func5;
	//wire          int_pending_func6;
	//wire          int_pending_func7;

	wire    [3:0] func1_sdio_func_int_code;
	wire          func1_csa_support;
	wire          func1_csa_en;
	wire    [7:0] func1_extended_sdio_func_int_code;
	wire          func1_sps;
	wire          func1_eps;
	wire   [23:0] func1_cis;
	reg     [7:0] block_size_reg_lb_func1;
	reg     [1:0] block_size_reg_hb_func1;

	wire    [3:0] func2_sdio_func_int_code;
	wire          func2_csa_support;
	wire          func2_csa_en;
	wire    [7:0] func2_extended_sdio_func_int_code;
	wire          func2_sps;
	wire          func2_eps;
	wire   [23:0] func2_cis;
	reg     [7:0] block_size_reg_lb_func2;
	reg     [1:0] block_size_reg_hb_func2;

//--------------------------------------------------
// -- TBD // ?????
//--------------------------------------------------
assign        dat_width_4bit       = dat_bus_width[1] & ~dat_bus_width[0];
//assign        cmd52_reset          = 1'b0;
//assign		  cmd52_abort          = 1'b0;
//assign        func_suspend         = 1'b0;
//assign        func_resume          = 1'b0;

assign        func0_block_size     = {block_size_reg_hb[1:0], block_size_reg_lb};
assign        func1_block_size     = {block_size_reg_hb_func1[1:0], block_size_reg_lb_func1};
assign        func2_block_size     = {block_size_reg_hb_func2[1:0], block_size_reg_lb_func2};
//assign        func3_block_size     = 10'b0; //{block_size_reg_hb[1:0], block_size_reg_lb};
//assign        func4_block_size     = 10'b0; //{block_size_reg_hb[1:0], block_size_reg_lb};
//assign        func5_block_size     = 10'b0; //{block_size_reg_hb[1:0], block_size_reg_lb};
//assign        func6_block_size     = 10'b0; //{block_size_reg_hb[1:0], block_size_reg_lb};
//assign        func7_block_size     = 10'b0; //{block_size_reg_hb[1:0], block_size_reg_lb};

//assign        spi_crc_check_en_bit = 1'b1;

//--------------------------------------------------
//--------------------------------------------------
// -- START of register file code
//--------------------------------------------------
//--------------------------------------------------

assign irst                         = reset;
assign ird                          = (cmd52_rd_after_wr | cmd53_rd) & ~|cmd52_53_func_num;
assign iwr                          = (cmd52_write | cmd53_wr) & ~|cmd52_53_func_num;  
assign iwrite_byte_register         = (cmd53_wr_rd_executing & ~cmd53_52_received) ? cmd53_write_data : cmd52_write_data;
assign cmd52_accessed_byte_register = read_byte_register;
assign cmd53_read_data_internal_reg = read_byte_register;

  always@( posedge clk or posedge irst )	 
	begin
		if( irst )
		begin
		  addr                  <= 17'h0;
		  rd                    <= 1'b0;
		  wr                    <= 1'b0;
		  write_byte_register   <= 8'b0;
		  prv_cmd52_53_func_num <= 3'b0;
		end
		else 
		begin	
		  rd                    <= ird;
          wr                    <= iwr;
		  write_byte_register   <= iwrite_byte_register;
          
		  if(cmd_start)
		    prv_cmd52_53_func_num <= cmd52_53_func_num;

          if(cmd53_52_received)
            addr           <= cmd52_53_reg_addr;
 		  else if(cmd53_wr_rd_executing)
		  begin
		    if( cmd53_OP_code & (wr|rd) )
			  addr         <= addr + 1'b1;
		  end
		  else
		    addr           <= cmd52_53_reg_addr;
	    end
	end

//--------------------------------------------------
// -- HARDWIRED VALUES and 
// -- SDIO SPEC FIXED Registers
//--------------------------------------------------
assign resp4_max_num_func = 3'b010;
//assign high_speed         = 1'b0;
assign resp4_ocr_reg      = 24'h10_0000;
assign resp6_RCA          = {iresp6_RCA, iresp6_RCA, 1'b0};

  always@( posedge clk or posedge irst )	 
	begin
		if( irst )
		begin
		  iresp6_RCA        <= 7'h0;
		end
		else 
		begin
		  if( cmd3_create_new_RCA )
		  begin
            iresp6_RCA[6:4]   <= iresp6_RCA[5:3] ;
            iresp6_RCA[3]     <= cmd3_create_new_RCA ^ iresp6_RCA[6] ^ iresp6_RCA[2];
            iresp6_RCA[2:1]   <= iresp6_RCA[1:0] ;
            iresp6_RCA[0]     <= cmd3_create_new_RCA ^ iresp6_RCA[6];
		  end
	    end
	end
	
assign cccr_rev                   = 4'h1; 
assign sdio_rev                   = 4'h1;
assign sd_format_ver_num          = 4'h1;
assign continuous_spi_int_support = 1'b1;
assign direct_cmd_support         = 1'b0;
assign multi_block_support        = 1'b1;
assign read_wait_support          = 1'b0;
assign suspend_resume_support     = 1'b0;
assign interrupt_sd4bit_support   = 1'b1;
assign low_speed_card             = 1'b0;
assign low_speed_4bit_support     = 1'b0;
assign bus_status                 = 1'b0;
assign bus_release_status         = 1'b0;
assign func_bit_sel               = 4'b0;
assign resume_dat_flag            = 1'b0;
assign execution_flags            = 8'b0;
assign ready_flags                = 8'b0;
assign master_pwr_control_support = 1'b1;

assign func1_sdio_func_int_code          = 4'b0;
assign func1_csa_support                 = 1'b0;
assign func1_csa_en                      = 1'b0;
assign func1_extended_sdio_func_int_code = 8'b0;
assign func1_sps                         = 1'b0;
assign func1_eps                         = 1'b0;
assign func1_cis                         = 24'h01_1000;

assign func2_sdio_func_int_code          = 4'b0;
assign func2_csa_support                 = 1'b0;
assign func2_csa_en                      = 1'b0;
assign func2_extended_sdio_func_int_code = 8'b0;
assign func2_sps                         = 1'b0;
assign func2_eps                         = 1'b0;
assign func2_cis                         = 24'h01_2000;

//--------------------------------------------------
// -- REGISTER READS using only the rd lines no clk
//--------------------------------------------------
  always@( addr or irst or sdio_rev or cccr_rev or sd_format_ver_num or 
           //io_en_func7 or io_en_func6 or
           //io_en_func5 or io_en_func4 or io_en_func3 or 
		   io_en_func2 or io_en_func1 or 
		   //io_ready_func7 or
		   //io_ready_func6 or io_ready_func5 or io_ready_func4 or io_ready_func3 or 
		   io_ready_func2 or io_ready_func1 or int_enable_func or int_enable_master or 
		   //int_pending_func7 or int_pending_func6 or
		   //int_pending_func5 or int_pending_func4 or int_pending_func3 or 
		   int_pending_func2 or int_pending_func1 or
		   cd_disable or continuous_spi_int_support or continuous_spi_int_en or
           dat_bus_width or	low_speed_4bit_support or low_speed_card or interrupt_sd4bit_en or interrupt_sd4bit_support or   
		   suspend_resume_support or read_wait_support or multi_block_support or direct_cmd_support or bus_status or
           bus_release_status or resume_dat_flag or func_bit_sel or execution_flags or ready_flags or 
		   block_size_reg_lb or block_size_reg_hb or master_pwr_contrl_en or master_pwr_control_support or
           led_reg or interrupt_pending_test or func1_sdio_func_int_code or func1_csa_support or
		   func1_csa_en or func1_extended_sdio_func_int_code or func1_sps or func1_eps or func1_cis or 
		   func2_sdio_func_int_code or func2_csa_support or func2_csa_en or func2_extended_sdio_func_int_code or 
		   func2_sps or func2_eps or func2_cis or block_size_reg_hb_func1 or block_size_reg_lb_func1 or 
		   block_size_reg_hb_func2 or block_size_reg_lb_func2 )	 
	begin
	  if( irst )
	  begin
	    read_byte_register	  <= 8'b0;
	  end
	  else
	  begin    //NOTE: commented out registers prob means these are read as '0' anyway so remove it but keep it here for reference
        case( addr )
          17'h0_0000:   
            read_byte_register <= {sdio_rev, cccr_rev}; 
		  17'h0_0001:   
            read_byte_register <= {4'h0, sd_format_ver_num}; 
		  17'h0_0002:   
            read_byte_register <= {5'b0, io_en_func2, io_en_func1, 1'b0}; 
		  17'h0_0003:   
            read_byte_register <= {5'b0, io_ready_func2, io_ready_func1, 1'b0}; 
          17'h0_0004:   
            read_byte_register <= {5'b0, int_enable_func, int_enable_master}; 
		  17'h0_0005:   
            read_byte_register <= {5'b0, int_pending_func2, int_pending_func1, sram_onn}; 
		  //17'h0_0006:   
          //  read_byte_register <= 8'h0;   //{4'b0, io_card_reset, abort_func_num};  since write only
		  17'h0_0007:   
            read_byte_register <= {cd_disable, continuous_spi_int_support, continuous_spi_int_en, 3'b0, dat_bus_width};
          17'h0_0008:   
            read_byte_register <= {low_speed_4bit_support, low_speed_card, interrupt_sd4bit_en, interrupt_sd4bit_support, suspend_resume_support, read_wait_support, multi_block_support, direct_cmd_support}; 
		  //17'h0_0009:   
          //  read_byte_register <= 8'h00; 
		  17'h0_000A:   
            read_byte_register <= 8'h10; 
		  //17'h0_000B:   
          //  read_byte_register <= 8'h00; 
          //17'h0_000C:   
          //  read_byte_register <= {6'b0, bus_status, bus_release_status}; 
		  //17'h0_000D:   
          //  read_byte_register <= {resume_dat_flag, 3'b0, func_bit_sel}; 
		  //17'h0_000E:   
          //  read_byte_register <= execution_flags; 
		  //17'h0_000F:   
          //  read_byte_register <= ready_flags;
		  17'h0_0010:   
            read_byte_register <= block_size_reg_lb; 
		  17'h0_0011:   
            read_byte_register <= {6'b0, block_size_reg_hb}; 
		  17'h0_0012:   
            read_byte_register <= {6'b0, master_pwr_contrl_en, master_pwr_control_support}; 
          17'h0_00F0:
            read_byte_register <= {4'b0, led_reg};
		  17'h0_00F1:
            read_byte_register <= {5'b0, interrupt_pending_test, sram_onn};  // To be removed once interrupt debugging done, check where sram_onn should go to
		  
		  //function 1 FBR
		  //17'h0_0100:   
          //  read_byte_register <= { func1_csa_en, func1_csa_support, 2'b0, func1_sdio_func_int_code}; 
		  //17'h0_0101:   
		  //  read_byte_register <= func1_extended_sdio_func_int_code;
		  //17'h0_0102:   
          //  read_byte_register <= {6'b0, func1_eps, func1_sps}; 
		  //17'h0_0109:   
		  //  read_byte_register <= func1_cis[7:0];
		  17'h0_010A:   
            read_byte_register <= func1_cis[15:8]; 
		  17'h0_010B:   
		    read_byte_register <= {7'b0, func1_cis[16]};
		  17'h0_0110:   
            read_byte_register <= block_size_reg_lb_func1; 
		  17'h0_0111:   
		    read_byte_register <= {6'b0, block_size_reg_hb_func1};

          //function 2 FBR
		  //17'h0_0200:   
          //  read_byte_register <= {func2_csa_en, func2_csa_support, 2'b0, func2_sdio_func_int_code}; 
		  //17'h0_0201:   
		  //  read_byte_register <= func2_extended_sdio_func_int_code;
		  //17'h0_0202:   
          //  read_byte_register <= {6'b0, func2_eps, func2_sps}; 
		  //17'h0_0209:   
		  //  read_byte_register <= func2_cis[7:0];
		  17'h0_020A:   
            read_byte_register <= func2_cis[15:8]; 
		  17'h0_020B:   
		    read_byte_register <= {7'b0, func2_cis[16]};
		  17'h0_0210:   
            read_byte_register <= block_size_reg_lb_func2; 
		  17'h0_0211:   
		    read_byte_register <= {6'b0, block_size_reg_hb_func2};

		  default: 
		    read_byte_register <= 8'h0;
        endcase	
	  end
    end

//--------------------------------------------------
// -- Interrupt
//--------------------------------------------------
assign interrupt    = spi_mode_on ? (continuous_spi_int_en ? internal_int : (internal_int & ~spi_csn)) : 
                                    internal_int;
assign internal_int = int_enable_master  & (|func_int);   
//assign func_int[7]  = int_enable_func[7] & int_pending_func7;
//assign func_int[6]  = int_enable_func[6] & int_pending_func6;
//assign func_int[5]  = int_enable_func[5] & int_pending_func5;
//assign func_int[4]  = int_enable_func[4] & int_pending_func4;
//assign func_int[3]  = int_enable_func[3] & int_pending_func3;
assign func_int[2]  = int_enable_func[2] & int_pending_func2;
assign func_int[1]  = int_enable_func[1] & int_pending_func1;

//--------------------------------------------------
// -- CCCR REGISTER WRITES (FUNC 0)
// -- 0x000000 to 0x01FFFF
//--------------------------------------------------	
  always@( posedge clk or posedge irst )	 //02h
	begin
		if( irst )
		begin
		  io_en_func1  <= 1'b0;   // can be used to reset func1
		  io_en_func2  <= 1'b0;   // can be used to reset func2
		  //io_en_func3  <= 1'b0;   // can be used to reset func3
		  //io_en_func4  <= 1'b0;   // can be used to reset func4
		  //io_en_func5  <= 1'b0;   // can be used to reset func5
		  //io_en_func6  <= 1'b0;   // can be used to reset func6
		  //io_en_func7  <= 1'b0;   // can be used to reset func7
		end
		else 
		begin
          if( io_card_reset )
		  begin
		    io_en_func1  <= ~io_card_reset;
		    io_en_func2  <= ~io_card_reset;
		    //io_en_func3  <= ~io_card_reset;
		    //io_en_func4  <= ~io_card_reset;
		    //io_en_func5  <= ~io_card_reset;
		    //io_en_func6  <= ~io_card_reset;
		    //io_en_func7  <= ~io_card_reset;
          end
		  else if( ~master_pwr_contrl_en )
		  begin
		    io_en_func1  <= master_pwr_contrl_en;
		    io_en_func2  <= master_pwr_contrl_en;
		    //io_en_func3  <= master_pwr_contrl_en;
		    //io_en_func4  <= master_pwr_contrl_en;
		    //io_en_func5  <= master_pwr_contrl_en;
		    //io_en_func6  <= master_pwr_contrl_en;
		    //io_en_func7  <= master_pwr_contrl_en;
		  end
		  else if( wr & (~|addr[16:2]) & (addr[1:0] == 2'b10) )
		  begin
		    io_en_func1  <= write_byte_register[1];
		    io_en_func2  <= write_byte_register[2];
		    //io_en_func3  <= write_byte_register[3];
		    //io_en_func4  <= write_byte_register[4];
		    //io_en_func5  <= write_byte_register[5];
		    //io_en_func6  <= write_byte_register[6];
		    //io_en_func7  <= write_byte_register[7];
		  end
	    end
	end

  always@( posedge clk or posedge irst )	 //04h
	begin
		if( irst )
		begin
		  int_enable_func	  <= 2'b0;
		  int_enable_master   <= 1'b0;
		end
		else 
		begin
		  if( wr & (~|addr[16:3]) & (addr[2:0] == 3'b100) )
		  begin
		    int_enable_func	  <= write_byte_register[2:1];
            int_enable_master <= write_byte_register[0];
		  end
	    end
	end

  always@( posedge clk or posedge irst )	 //06h
	begin
		if( irst )
		begin
		  io_card_reset	      <= 1'b0;
		  //abort_func_num      <= 3'b0;
		end
		else 
		begin
		  if( wr & (~|addr[16:3]) & (addr[2:0] == 3'b110) )
		  begin
   		    io_card_reset	  <= write_byte_register[3];
            //abort_func_num    <= write_byte_register[2:0];
		  end
		  else if( clksync_clear_abort_reg )
		  begin
		    io_card_reset	  <= 1'b0;
		    //abort_func_num    <= 3'b0;
		  end
	    end
	end

 always@( posedge clk or posedge irst )	 //06h
	begin
		if( irst )
		  abort_func_num      <= 3'b0;
		else 
		begin
		  if( cmd52_wr_io_abort_reg )
            abort_func_num    <= cmd52_wr_io_abort_reg_data;
		  else if( clksync_clear_abort_reg )
		    abort_func_num    <= 3'b0;
	    end
	end

  always@( posedge clk or posedge irst )	 //used to determine if abort reg written, then sync this signal to be used to clear these registers 
	begin
		if( irst )
		  abort_reg_written <= 1'b0;
		else 
		begin
		  if( wr & (~|addr[16:3]) & (addr[2:0] == 3'b110) )
   		    abort_reg_written <= 1'b1;
		  else if( clksync_clear_abort_reg )
		    abort_reg_written <= 1'b0;
	    end
	end
	
assign io_abort           = (abort_func_num == prv_cmd52_53_func_num) & |abort_func_num;
assign go_cmd_state_func1 = (abort_func_num == 3'b001);
assign go_cmd_state_func2 = (abort_func_num == 3'b010);
//assign go_cmd_state_func3 = (abort_func_num == 3'b011);
//assign go_cmd_state_func4 = (abort_func_num == 3'b100);
//assign go_cmd_state_func5 = (abort_func_num == 3'b101);
//assign go_cmd_state_func6 = (abort_func_num == 3'b110);
//assign go_cmd_state_func7 = (abort_func_num == 3'b111);

  always@( posedge clk or posedge irst )	 //07h
	begin
		if( irst )
		begin
		  cd_disable	         <= 1'b0;
		  continuous_spi_int_en  <= 1'b0;
		  dat_bus_width          <= 2'b0;
		end
		else 
		begin
		  if( wr & (~|addr[16:3]) & (addr[2:0] == 3'b111) )
		  begin
   		    cd_disable     	      <= write_byte_register[7];
            continuous_spi_int_en <= write_byte_register[5];
            dat_bus_width         <= write_byte_register[1:0];
		  end
	    end
	end

  always@( posedge clk or posedge irst )	 //08h
	begin
		if( irst )
		  interrupt_sd4bit_en	  <= 1'b0;
		else 
		begin
		  if( wr & (~|addr[16:4]) & (addr[3:0] == 4'b1000) )
   		    interrupt_sd4bit_en   <= write_byte_register[5];
	    end
	end

  always@( posedge clk or posedge irst )	 //10h
	begin
		if( irst )
		begin
		  block_size_reg_lb	  <= 8'h4;
		end
		else 
		begin
		  if( wr & (~|addr[16:8]) & (addr[7:0] == 8'h10) )
		  begin
            block_size_reg_lb <= write_byte_register[7:0];
		  end
	    end
	end

  always@( posedge clk or posedge irst )	 //11h
	begin
		if( irst )
		begin
		  block_size_reg_hb   	  <= 2'h0;
		end
		else 
		begin
		  if( wr & (~|addr[16:8]) & (addr[7:0] == 8'h11) )
		  begin
            block_size_reg_hb    <= write_byte_register[1:0];
		  end
	    end
	end

  always@( posedge clk or posedge irst )	 //12h
	begin
		if( irst )
		begin
		  master_pwr_contrl_en   <= 1'b0;
		end
		else 
		begin
		  if( wr & (~|addr[16:8]) & (addr[7:0] == 8'h12) )
		  begin
            master_pwr_contrl_en <= write_byte_register[1];
		  end
	    end
	end

  always@( posedge clk or posedge irst )	 //F0h
	begin
		if( irst )
		begin
		  led_reg        	  <= 4'h0;
		end
		else 
		begin
		  if( wr & (~|addr[16:8]) & (addr[7:0] == 8'hF0) )
		  begin
            led_reg           <= write_byte_register[3:0];
		  end
	    end
	end

  always@( posedge clk or posedge irst )	 //F1h  // To be removed once interrupt debugging done, for testing purposes only
	begin
		if( irst )
		begin
		  interrupt_pending_test   <= 2'h0;
		  sram_onn                 <= 1'b1;
		end
		else 
		begin
		  if( (~|addr[16:8]) & (addr[7:0] == 8'hF1) )
		  begin
            if( wr )
			begin
			  interrupt_pending_test[2:1] <= write_byte_register[2:1];
			  sram_onn                    <= write_byte_register[0];
			end
		    else if( rd & ~cmd52_53_rw_flag )
			  interrupt_pending_test[2:1] <= 2'h0;
		  end
	    end
	end

assign int_pending_func1 = interrupt_pending_test[1];   // To be removed once interrupt debugging done
assign int_pending_func2 = interrupt_pending_test[2];   // To be removed once interrupt debugging done
//assign int_pending_func3 = interrupt_pending_test[3];   // To be removed once interrupt debugging done
//assign int_pending_func4 = interrupt_pending_test[4];   // To be removed once interrupt debugging done
//assign int_pending_func5 = interrupt_pending_test[5];   // To be removed once interrupt debugging done
//assign int_pending_func6 = interrupt_pending_test[6];   // To be removed once interrupt debugging done
//assign int_pending_func7 = interrupt_pending_test[7];   // To be removed once interrupt debugging done


  always@( posedge clk or posedge irst )	 //110h
	begin
		if( irst )
		begin
		  block_size_reg_lb_func1 <= 8'h4;
		end
		else 
		begin
		  if( wr & (~|addr[16:12]) & (addr[11:0] == 12'h110) )
		  begin
            block_size_reg_lb_func1 <= write_byte_register[7:0];
		  end
	    end
	end

  always@( posedge clk or posedge irst )	 //111h
	begin
		if( irst )
		begin
		  block_size_reg_hb_func1 <= 2'h0;
		end
		else 
		begin
		  if( wr & (~|addr[16:12]) & (addr[11:0] == 12'h111) )
		  begin
            block_size_reg_hb_func1    <= write_byte_register[1:0];
		  end
	    end
	end


  always@( posedge clk or posedge irst )	 //210h
	begin
		if( irst )
		begin
		  block_size_reg_lb_func2 <= 8'h4;
		end
		else 
		begin
		  if( wr & (~|addr[16:12]) & (addr[11:0] == 12'h210) )
		  begin
            block_size_reg_lb_func2 <= write_byte_register[7:0];
		  end
	    end
	end

  always@( posedge clk or posedge irst )	 //211h
	begin
		if( irst )
		begin
		  block_size_reg_hb_func2 <= 2'h0;
		end
		else 
		begin
		  if( wr & (~|addr[16:12]) & (addr[11:0] == 12'h211) )
		  begin
            block_size_reg_hb_func2    <= write_byte_register[1:0];
		  end
	    end
	end

endmodule






















