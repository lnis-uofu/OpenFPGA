///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2007, Licensed customers of QuickLogic may copy or modify
// this file for use in designing QuickLogic devices only.
//
// Module Name:  cmd_control
// File Name:    cmd_control.v
//
///////////////////////////////////////////////////////////////////////////////
`timescale 1ns/1ns

module cmd_control(           
                       reset,    // global reset   
					   spi_reset, 
                       clk,                            
                       clkn,

					   cmd,

                       //high_speed,                       
                       resp4_max_num_func,
                       resp4_ocr_reg,
                       resp6_RCA,
                       
					   cmd52_accessed_byte_register,
					   cmd52_accessed_byte_register_sram,
                       cmd52_write_data,
                       cmd52_rd_after_wr,
					   cmd52_write,
					   cmd3_create_new_RCA,
					   cmd53_52_received,

                       // all of these are registered except for byte_block_count
                       cmd52_53_rw_flag,  
                       cmd53_block_mode,    
                       cmd53_OP_code, 
                       cmd53_byte_block_count_nt_registered,
					   cmd52_53_reg_addr,
					   cmd52_53_func_num,
					   cmd53_received,
					   // end of registered signals

                       cmd_start,
                       cmd_end,	
					   rsp_end,
					   intr_period_cmd_end,
					   func_num_err,	
					   cmd_index,			   

	                   cmd52_wr_io_abort_reg_data,
	                   cmd52_wr_io_abort_reg,

					   dat_lines_being_used,
					   //cmd52_reset,
		               cmd52_abort, 
			           //func_suspend, 
			           exec_complete,
			           //func_resume,

                       cmd_txrx_cmd_idle,
			           // CMD BUS STATES
					   cmd_bus_idle_state,    
	                   cmd_bus_init_state,
	                   cmd_bus_stby_state,    
                       cmd_bus_cmd_state,      
                       cmd_bus_txfr_state,     
	                   cmd_bus_inactive_state,

					   //SPI mode
					   spi_crc_check_en_bit,       //input
					   spi_csn,                    //input
					   spi_mode_on,                //output
					   spi_cmdrsp_sdata,           //output
					   cmd_txrx_spicard_resp,      //output
					   all_blocks_written_out,     //input
	                   spicard_wr2host_bit,        //output
                       spicard_rd_frm_host_bit,    //output
					   clear_spicard_rd_frm_host_bit,  //input
					   clear_spicard_wr2host_bit,      //input
					   spi_dat_err,                //input
					   spi_stop_tran_cmd_rcvd,      //output
					   out_of_range,                 //output
					   error                        //output

                   );

    input         reset;          
	input         spi_reset;                                            
    input         clk;                            
    input         clkn;
 
	inout         cmd;
     
    //input         high_speed;
	input  [2:0]  resp4_max_num_func;
	input  [23:0] resp4_ocr_reg;
	input  [15:0] resp6_RCA;

	input   [7:0] cmd52_accessed_byte_register;
	input   [7:0] cmd52_accessed_byte_register_sram;
	output  [7:0] cmd52_write_data;
    output        cmd52_rd_after_wr;
	output	 	  cmd52_write;
	output [16:0] cmd52_53_reg_addr;
	output  [2:0] cmd52_53_func_num;
	output        cmd3_create_new_RCA;
    output        cmd53_52_received;

	output        cmd52_53_rw_flag;  
    output        cmd53_block_mode;    
    output        cmd53_OP_code; 
    output  [9:0] cmd53_byte_block_count_nt_registered;
	output        cmd53_received;

    output        cmd_start;
	output        cmd_end;
	output        rsp_end;
	output        intr_period_cmd_end;
	output        func_num_err;
	output  [5:0] cmd_index;

	output  [2:0] cmd52_wr_io_abort_reg_data;
	output        cmd52_wr_io_abort_reg;

    input         dat_lines_being_used;
	//input         cmd52_reset;
	input		  cmd52_abort;
	//input         func_suspend;
	input         exec_complete;
	//input         func_resume;

    output        cmd_txrx_cmd_idle;
	output		  cmd_bus_idle_state;    
	output        cmd_bus_init_state;
	output        cmd_bus_stby_state;    
    output        cmd_bus_cmd_state;      
    output        cmd_bus_txfr_state;     
	output        cmd_bus_inactive_state; 

    output        spi_crc_check_en_bit;           //output
	input         spi_csn;
	output        spi_mode_on;                    //output
	output        spi_cmdrsp_sdata;               //output
	output        cmd_txrx_spicard_resp;          //output
	input         all_blocks_written_out;         //input
	output        spicard_wr2host_bit;            //output
	output        spicard_rd_frm_host_bit;        //output
	input         clear_spicard_rd_frm_host_bit;  //input
	input         clear_spicard_wr2host_bit;      //input
	input         spi_dat_err;                    //input
	output        spi_stop_tran_cmd_rcvd;         //output
	output        out_of_range;                   //output
	output        error;                          //output


//--- Declaration of wires and regs
    reg         dout_reg_hs;
    reg         oe_reg_hs;
    reg         dout_reg_ls;
    reg         oe_reg_ls;
    reg  [39:0] iresp_data_reg;
    reg   [7:0] resp_reg;
	reg   [2:0] cmd_bus_state;
	reg   [2:0] cmd_txrx_state;
	reg   [5:0] cmd_txrx_cnt;                
    reg   [7:0] cmd_in_dly;          
    reg  [39:0] cmd_data;    
	reg         invalid_cmd;
	reg         com_crc_error;
	reg   [1:0] io_current_state;
	reg         cmd52_rd_after_wr;
	reg  	    cmd52_write;
	reg         cmd3_create_new_RCA;
	      
	wire		illegal_command; 
	wire		error; 
	wire		func_num_err; 
	wire        out_of_range;
    wire        ocr_mismatch;

    wire        icmd53_52_received;
	reg         cmd53_received;
    wire  [6:0] crc_reg;
    wire        dout_reg;
    wire        oe_reg;
	wire  [7:0] write_first_then_read_data; 
	wire  [7:0] read_data_no_write;
    wire        irst;     
    wire  [5:0] cmd_index;         
    wire        host2card;        
    wire [15:0] cmd7_15_RCA;        
    wire  [2:0] icmd52_53_func_num;      
	reg   [2:0] cmd52_53_func_num;
    wire        icmd52_53_rw_flag;      
	reg         cmd52_53_rw_flag; 
    wire        cmd52_raw_flag;          
    wire        icmd53_block_mode; 
	reg         cmd53_block_mode;       
    wire        icmd53_OP_code;           
	reg         cmd53_OP_code;
    wire [16:0] icmd52_53_reg_addr;       
	wire [16:0] cmd52_53_reg_addr;
    wire  [7:0] cmd52_write_data;       
    wire  [9:0] cmd53_byte_block_count_nt_registered; 
	wire [23:0] cmd5_ocr_register;
    wire        gen48;    
	wire        out48;    	
    wire        byte_end; 
	wire        cmd_byte_latch;
	wire        end48;    
	wire        resp_snd_end; 
    wire        cmd_txrx_rcv_cmd;  
	wire        cmd_txrx_wait;     
	wire        cmd_txrx_snd_resp; 
	wire        cmd_txrx_cmd_idle;
    wire        dout; 
	wire        sdata;
	wire        crc_din;
	wire        crc_rst; 
	wire        crc_dout;
	wire        card2host;               
	wire        iresp4_card_ready;         
	wire  [2:0] iresp4_max_num_func;   
	wire        iresp4_mem_present;    
	wire [23:0] iresp4_ocr_reg;       
	wire [15:0] iresp6_RCA;               
	wire [15:0] iresp6_card_status;
	wire  [5:0] iresp1b_5_6_cmd_index;  
	wire  [7:0] iresp5_resp_flags;     
	wire  [7:0] iresp5_rd_wr_data;     
	wire [31:0] iresp1b_card_status;
	wire        cmd53_52_received;

    reg         spicard_rd_frm_host_bit;
	wire [15:0] spicard_resp5;
	wire  [7:0] spicard_resp1;
	wire [39:0] spicard_resp4;
	wire        spicard_resp_end;
	wire        host_spi_rd_cmd;
	wire        host_spi_wr_cmd;
	wire        param_err;
	wire        clear_spicard_wr2host_bit;
	wire        single_blk_wr2host;
	reg         spi_mode_on;
	reg         spicard_wr2host_bit;
	reg         spi_crc_check_en_bit;

    wire        cmd0_index;
	wire        cmd3_index;
	wire        cmd5_index;
	wire        cmd7_index;
	wire        cmd15_index;
	wire        cmd52_index;
	wire        cmd53_index;
	wire        cmd59_index;

	wire        temp1;

	// Command BUS STATE MACHINE STATES
    parameter   sBUS_CMD_IDLE        = 3'b000;
	parameter   sBUS_CMD_INIT        = 3'b011;
    parameter   sBUS_STANBY_STATE    = 3'b100;
    parameter   sBUS_CMD_STATE       = 3'b101;
    parameter   sBUS_TRANSFER_STATE  = 3'b110;
    parameter   sBUS_INACTIVE_STATE  = 3'b111;

	// Command RECEPTION and Response TRANSMISSION STATE MACHINE STATES
    parameter   sTXRX_CMD_IDLE            = 3'b000;
    parameter   sTXRX_RCV_CMD             = 3'b001;
	parameter   sTXRX_WAIT                = 3'b010;
    parameter   sTXRX_SEND_RESP           = 3'b011;
    parameter   sTXRX_SPICARD_WAIT        = 3'b100;
	parameter   sTXRX_SPICARD_RESP        = 3'b101;
	parameter   sTXRX_SPICARD_WR2HOST     = 3'b110;
    parameter   sTXRX_SPICARD_RD_FRM_HOST = 3'b111;      

assign irst = reset;

assign cmd0_index  = ~|cmd_index;                                     //CMD0 
assign cmd3_index  = ~|cmd_index[5:2] & (&cmd_index[1:0]);            //CMD3
assign cmd5_index  = ~|cmd_index[5:3] & (cmd_index[2:0] == 3'b101);   //CMD5
assign cmd7_index  = ~|cmd_index[5:3] & (&cmd_index[2:0]);            //CMD7
assign cmd15_index = ~|cmd_index[5:4] & (&cmd_index[3:0]);            //CMD15
assign cmd52_index = &cmd_index[5:4]  & (cmd_index[3:0] == 4'b0100);  //CMD52
assign cmd53_index = &cmd_index[5:4]  & (cmd_index[3:0] == 4'b0101);  //CMD53
assign cmd59_index = &cmd_index[5:3]  & (cmd_index[2:0] == 3'b011);   //CMD59

//--------------------------------------------------
// -- CMD IO PAD
//--------------------------------------------------	
    assign	cmd			= ( oe_reg )     ? dout_reg    : 1'bz;  
    assign	dout_reg	= dout_reg_ls;  //( high_speed ) ? dout_reg_hs : dout_reg_ls;
    assign	oe_reg		= oe_reg_ls;    //( high_speed ) ? oe_reg_hs   : oe_reg_ls;	

//-- high speed output reference to clk positive edge	
	always@( posedge clk or posedge spi_reset )	 
	begin
		if( spi_reset )
		begin
		  dout_reg_hs	<= 1'b0;
		  oe_reg_hs		<= 1'b0;
		end
		else
		begin
		  dout_reg_hs	<= dout;
		  oe_reg_hs		<= cmd_txrx_snd_resp;	
		end
	end

//-- low speed output reference to clk negetive edge	
	always@( posedge clkn or posedge spi_reset )
	begin
		if( spi_reset )
		begin
		  dout_reg_ls	<= 1'b0;
		  oe_reg_ls		<= 1'b0;
		end
		else
		begin
		  dout_reg_ls	<= dout_reg_hs;
		  oe_reg_ls		<= oe_reg_hs;
		end
	end

//----------------------------------------------------
//-- Inbound Packet framing (latch on posedge) 
//----------------------------------------------------
  always@( posedge clk or posedge spi_reset )	 
	begin
	  if( spi_reset )
	  begin
	    cmd_in_dly      <= 8'hFF;
	  end
	  else
	  begin
	    cmd_in_dly      <= cmd_in_dly << 1;	
		cmd_in_dly[0]   <= cmd;
	  end
	end

//----------------------------------------------------
//-- Latch in the cmd index and the argument of the
//-- commands
//----------------------------------------------------
  always@( posedge clk or posedge spi_reset )	 
	begin
	  if( spi_reset )
	  begin
	    cmd_data         <= 40'h0;
	  end
	  else
	  begin
	    if( cmd_byte_latch & !(cmd_txrx_cnt[5] & cmd_txrx_cnt[3]) & cmd_txrx_rcv_cmd )
	    begin
		  cmd_data[39:32] <= cmd_data[31:24];
		  cmd_data[31:24] <= cmd_data[23:16];
		  cmd_data[23:16] <= cmd_data[15:8];
          cmd_data[15:8]  <= cmd_data[7:0];
	      cmd_data[7:0]   <= cmd_in_dly;	
	    end
      end
	end

assign cmd_index                            = cmd_data[37:32];    // For all commands
assign host2card                            = cmd_data[38];       // For all commands, should always be received as 1
assign cmd7_15_RCA                          = cmd_data[31:16];    // For commands cmd 7 and cmd 15 only
assign icmd52_53_func_num                   = cmd_data[30:28];    // For commands cmd 52 and cmd 53 only
assign icmd52_53_rw_flag                    = cmd_data[31];       // For commands cmd 52 and cmd 53 only
assign cmd52_raw_flag                       = cmd_data[27];       // For command 52 only
assign icmd53_block_mode                    = cmd_data[27];       // For command 53 only
assign icmd53_OP_code                       = cmd_data[26];       // For command 53 only
assign icmd52_53_reg_addr                   = cmd_data[25:9];     // For commands cmd 52 and cmd 53 only
assign cmd52_write_data                     = cmd_data[7:0];      // For command 52 only
assign cmd5_ocr_register                    = cmd_data[23:0];     // For command 5 only
assign cmd53_byte_block_count_nt_registered = (cmd_index[5:0] == 6'b11_0101) & (~|cmd_data[8:0]) ? 
                                              10'b10_0000_0000 : {1'b0, cmd_data[8:0]};      // For command 53 only

//----------------------------------------------------
//-- cmd52 io abort wr to register and data
//----------------------------------------------------
assign cmd52_wr_io_abort_reg      = cmd_end & icmd52_53_rw_flag & (~|icmd52_53_reg_addr[16:3]) & (icmd52_53_reg_addr[2:0] == 3'b110)
                                    & cmd52_index;
assign cmd52_wr_io_abort_reg_data = cmd52_write_data[2:0];

//----------------------------------------------------
//-- make sure the data is ready for output when
//-- the response send state starts
//----------------------------------------------------
assign card2host             = 1'b0;               // For all responses, should always be sent out as 0
assign iresp4_card_ready     = 1'b1;               // For response 4 only, CMD5 response cine card should always be ready
assign iresp4_max_num_func   = resp4_max_num_func; // For response 4 only, CMD5 response
assign iresp4_mem_present    = 1'b0;               // For response 4 only, no memory in design so 0 always 
assign iresp4_ocr_reg        = resp4_ocr_reg;      // For response 4 only, CMD5 response
assign iresp6_RCA            = resp6_RCA;          // For response 6 only, the newly written RCA of the card is shown
assign iresp6_card_status    = {com_crc_error, illegal_command, error, 13'b0};  // For response 6 only, card status bits
assign iresp1b_5_6_cmd_index = cmd_index;          // For response 1b and 5 only, used for CMD6, CMD7, CMD 52 and CMD 53
assign iresp5_resp_flags     = {com_crc_error, illegal_command, io_current_state, error, 1'b0, func_num_err, out_of_range};  // For response 5 only, used for CMD 52 and CMD 53
// For response 5 only, used in CMD 52 and CMD 53 and determines what goes into the read or write data field
assign iresp5_rd_wr_data     = cmd_index[0] ? 8'b0 : ( icmd52_53_rw_flag ? ( cmd52_raw_flag ? write_first_then_read_data : cmd52_write_data ) : read_data_no_write );
// For response 1b only, used in CMD 7. Note: resp1b allows for the sdio client to pull down datline0 until it has a free data receive buffer
assign iresp1b_card_status   = {out_of_range, 7'b0, com_crc_error, illegal_command, 2'b0, error, 6'b0, 4'hF, 9'b0};
assign write_first_then_read_data = |icmd52_53_func_num ? cmd52_accessed_byte_register_sram : cmd52_accessed_byte_register;
assign read_data_no_write         = |icmd52_53_func_num ? cmd52_accessed_byte_register_sram : cmd52_accessed_byte_register;

//----------------------------------------------------
//-- Outbound Response Packet framing
//----------------------------------------------------

// Full structure of the 38 bit response register since doesn't include the start and card2host bit as well as crc portion
always @ ( cmd_index or iresp1b_5_6_cmd_index or iresp6_RCA or iresp6_card_status or iresp4_card_ready or 
           iresp4_max_num_func or iresp4_mem_present or iresp4_ocr_reg or iresp1b_5_6_cmd_index or iresp1b_card_status
		   or iresp1b_5_6_cmd_index or iresp5_resp_flags or iresp5_rd_wr_data or spicard_resp1 or spicard_resp5 or
		   spicard_resp4 or spi_mode_on)
        begin
		  if( ~spi_mode_on )
		  begin 
		    case ( cmd_index )
		    // CMD0, CMD15 do not have response so ignored along with other incorrect commands 
		      6'b00_0011   :  iresp_data_reg <= {iresp1b_5_6_cmd_index, iresp6_RCA, iresp6_card_status, 2'b11};  // CMD3
		      6'b00_0101   :  iresp_data_reg <= {iresp4_card_ready, iresp4_max_num_func, iresp4_mem_present, 3'b0, iresp4_ocr_reg, 2'b11};  // CMD5
		      6'b00_0111   :  iresp_data_reg <= {iresp1b_5_6_cmd_index, iresp1b_card_status, 2'b11};  // CMD7
			  default      :  iresp_data_reg <= {iresp1b_5_6_cmd_index, 16'b0, iresp5_resp_flags, iresp5_rd_wr_data, 2'b11};  // CMD52 and CMD53
		    endcase
          end
		  else
		    case ( cmd_index )
		    // CMD0 and CMD59 (R1), CMD5 (R4), CMD52 and CMD53 (R5)  
		      6'b00_0000   :  iresp_data_reg <= {spicard_resp1, 32'hFFFF_FFFF};  // CMD0
		      6'b11_1011   :  iresp_data_reg <= {spicard_resp1, 32'hFFFF_FFFF};  // CMD59
			  6'b00_0101   :  iresp_data_reg <= spicard_resp4;                   // CMD5
			  default      :  iresp_data_reg <= {spicard_resp5, 24'hFF_FFFF};    // CMD52 and CMD53
		    endcase
        end 

// packetizing it into bytes for sending
always@( posedge clk or posedge spi_reset )
	begin  
	    if( spi_reset )
          resp_reg <= 8'h00;
		else
		begin 
	      if( ~spi_mode_on )
		  begin
		    if( cmd_txrx_wait )
		      resp_reg <= {1'b0, card2host, iresp_data_reg[39:34]};
            else
		      if( cmd_txrx_snd_resp )
	          case ( cmd_txrx_cnt[5:0] )
		        6'b00_0111   :  resp_reg <= iresp_data_reg[33:26];
		        6'b00_1111   :  resp_reg <= iresp_data_reg[25:18];
		        6'b01_0111   :  resp_reg <= iresp_data_reg[17:10];
		        6'b01_1111   :  resp_reg <= iresp_data_reg[9:2];
		        default      :  resp_reg <= resp_reg << 1;
			  endcase 
	      end
		  else
		  begin
		    if( cmd_txrx_wait )
		      resp_reg <= iresp_data_reg[39:32];
            else
		      if( cmd_txrx_spicard_resp )
	          case ( cmd_txrx_cnt[5:0] )
		        6'b00_0111   :  resp_reg <= iresp_data_reg[31:24];
		        6'b00_1111   :  resp_reg <= iresp_data_reg[23:16];
		        6'b01_0111   :  resp_reg <= iresp_data_reg[15:8];
		        6'b01_1111   :  resp_reg <= iresp_data_reg[7:0];
		        default      :  resp_reg <= resp_reg << 1;
			  endcase 
		  end
		end
	end

// ---End bit and CRC enable flags
    assign gen48          = !out48;
	assign out48          = (cmd_txrx_cnt[5:3] == 3'b101 ) ? 1'b1 : 1'b0;	
    assign cmd_byte_latch = (cmd_txrx_cnt[2:0] == 3'b110 ) ? 1'b1 : 1'b0;
	assign byte_end       = (cmd_txrx_cnt[2:0] == 3'b111 ) ? 1'b1 : 1'b0;
	assign end48          = byte_end & out48;
	assign resp_snd_end   = end48 & cmd_txrx_snd_resp;

// ---Outputting the data 
    assign dout  = out48 ? ( crc_dout | resp_snd_end ) : sdata;
	assign sdata = resp_reg[7];
	
//----------------------------------------------------
//-- CRC generation
//----------------------------------------------------
// CRC output phase is right after generation phase
    assign cmd_rcv_crc_err = out48 ? 
	                        (spi_mode_on ? (spi_crc_check_en_bit & (cmd_in_dly[1] ^ (crc_dout|end48))) : 
							(cmd_in_dly[1] ^ (crc_dout|end48))) : 1'b0;

	assign crc_din = (cmd_txrx_rcv_cmd) ? cmd_in_dly[1] : sdata;
	assign crc_rst = cmd_txrx_cmd_idle | cmd_txrx_wait;
	
 crc7 cmd_crc7(
               .rst     ( crc_rst ),
               .clk     ( clk ),
               .gen_en  ( gen48 ),
               .out_en  ( out48 ),
               .din     ( crc_din ),
               .dout    ( crc_dout ),
			   .crc_reg ( )
             );

//----------------------------------------------------
//-- CRC CHECKING and error flags
//----------------------------------------------------

	always@( posedge clk or posedge spi_reset )	 
	begin
	  if( spi_reset )
	  begin
        com_crc_error    <= 1'b0; 
	  end
	  else
	  begin
	    if( end48 )
		begin
		  if( cmd_txrx_rcv_cmd & (~&cmd_index[5:4]) )
	        com_crc_error  <= cmd_rcv_crc_err;
		  else
		    if( cmd_txrx_snd_resp & (&cmd_index[5:4]) )
              com_crc_error    <= 1'b0;
        end
	  end
	end

	assign	illegal_command  = invalid_cmd;
	assign  error            = 1'b0;    // ???
	assign  out_of_range     = 1'b0;    // ???

  always@( posedge clk or posedge spi_reset )	 
	begin
	  if( spi_reset )
	  begin
	    io_current_state <= 2'b0;       // ???
	  end
	  else
	  begin
	    if( cmd_bus_inactive_state | cmd_bus_init_state | cmd_bus_stby_state | cmd_bus_idle_state )
		  io_current_state <= 2'b0;
        else if( (cmd_bus_cmd_state | cmd_bus_txfr_state) & (~dat_lines_being_used) )
		  io_current_state <= 2'b01;
        else if( (cmd_bus_cmd_state | cmd_bus_txfr_state) & dat_lines_being_used )
		  io_current_state <= 2'b10;
	  end
	end	

assign func_num_err = intr_period_cmd_end ? (icmd52_53_func_num > resp4_max_num_func) : (~irst);

//----------------------------------------------------
//-- Command RECEPTION and Response TRANSMISSION STATE 
//-- MACHINE STATES 
//----------------------------------------------------
assign cmd_end             = cmd_txrx_rcv_cmd  & (cmd_txrx_cnt == 6'h2F);
assign cmd_start           = cmd_txrx_rcv_cmd  & ~|cmd_txrx_cnt;
assign rsp_end             = cmd_txrx_snd_resp & (cmd_txrx_cnt == 6'h2F);
assign intr_period_cmd_end = cmd_txrx_rcv_cmd  & (cmd_txrx_cnt == 6'h2B);

// ---CMD TXRX STATE Phases
    assign cmd_txrx_rcv_cmd             = (cmd_txrx_state == sTXRX_RCV_CMD)              ? 1'b1 : 1'b0;
	assign cmd_txrx_wait                = (cmd_txrx_state == sTXRX_WAIT)                 ? 1'b1 : 1'b0;
	assign cmd_txrx_snd_resp            = (cmd_txrx_state == sTXRX_SEND_RESP)            ? 1'b1 : 1'b0;
    assign cmd_txrx_cmd_idle            = (cmd_txrx_state == sTXRX_CMD_IDLE)             ? 1'b1 : 1'b0;
    assign cmd_txrx_spicard_wait        = (cmd_txrx_state == sTXRX_SPICARD_WAIT)         ? 1'b1 : 1'b0;
	assign cmd_txrx_spicard_resp        = (cmd_txrx_state == sTXRX_SPICARD_RESP)         ? 1'b1 : 1'b0;
    assign cmd_txrx_spicard_wr2host     = (cmd_txrx_state == sTXRX_SPICARD_WR2HOST)      ? 1'b1 : 1'b0;
    assign cmd_txrx_spicard_rd_frm_host = (cmd_txrx_state == sTXRX_SPICARD_RD_FRM_HOST)  ? 1'b1 : 1'b0;

    always@( posedge clk or posedge spi_reset )
	begin
	    if ( spi_reset )
		begin
		  cmd_txrx_cnt   <= 6'b0;
		  cmd_txrx_state <= sTXRX_CMD_IDLE;
		end
		else
		begin
		    case ( cmd_txrx_state )
			    sTXRX_CMD_IDLE :	
				begin
				  cmd_txrx_cnt     <= 6'b0;
                  if( !cmd_in_dly[0] & !oe_reg & spicard_rd_frm_host_bit )
                  begin
				    cmd_txrx_state <= sTXRX_RCV_CMD;
                  end
				end
				
				sTXRX_RCV_CMD :
				begin
                  cmd_txrx_cnt       <= cmd_txrx_cnt + 1'b1;
                  if( cmd_end )
				  begin
				    cmd_txrx_state   <= sTXRX_WAIT;
					cmd_txrx_cnt     <= 6'b0;
				  end
				end
				
                sTXRX_WAIT :
				begin
				  cmd_txrx_cnt         <= cmd_txrx_cnt + 1'b1;
                  if( spi_mode_on )                  
                  begin
				    cmd_txrx_state     <= sTXRX_SPICARD_WAIT; 
					cmd_txrx_cnt       <= 6'b0;
				  end
				  else if( ~(|cmd_index) | cmd_index[3] | invalid_cmd )  //if CMD0 or CMD15 since these CMDS have no response OR invalid command
				  begin
				    cmd_txrx_state     <= sTXRX_CMD_IDLE;
                  end
				  else if( cmd_txrx_cnt[0] )
				  begin
				    cmd_txrx_state     <= sTXRX_SEND_RESP;
				    cmd_txrx_cnt       <= 6'b0;
				  end
				end

				sTXRX_SEND_RESP :
				begin
                  cmd_txrx_cnt       <= cmd_txrx_cnt + 1'b1;
				  if( rsp_end )
				  begin
				    cmd_txrx_state   <= sTXRX_CMD_IDLE;
					cmd_txrx_cnt     <= 6'b0;
				  end
				end
					    
                sTXRX_SPICARD_WAIT :
				begin
                  cmd_txrx_cnt       <= cmd_txrx_cnt + 1'b1;
				  if( &cmd_txrx_cnt[1:0] )
				  begin
				    cmd_txrx_state   <= sTXRX_SPICARD_RESP;
					cmd_txrx_cnt     <= 6'b0;
				  end
				end

                sTXRX_SPICARD_RESP :
				begin
                  cmd_txrx_cnt         <= cmd_txrx_cnt + 1'b1;
				  if( spicard_resp_end )
				  begin
				    if( host_spi_rd_cmd )
				    begin
					  cmd_txrx_state   <= sTXRX_SPICARD_WR2HOST;
					  cmd_txrx_cnt     <= 6'b0;
				    end
					else if( host_spi_wr_cmd )
					begin
					  cmd_txrx_state   <= sTXRX_SPICARD_RD_FRM_HOST;
					  cmd_txrx_cnt     <= 6'b0;  
					end
					else
					  cmd_txrx_state   <= sTXRX_CMD_IDLE; 
				  end
				end

                sTXRX_SPICARD_WR2HOST :
				begin
				  cmd_txrx_state       <= sTXRX_CMD_IDLE; 
				end

                sTXRX_SPICARD_RD_FRM_HOST :
				begin
				  cmd_txrx_state       <= sTXRX_CMD_IDLE; 
				end

				default :
				begin
		          cmd_txrx_cnt       <= 6'b0;
		          cmd_txrx_state     <= sTXRX_CMD_IDLE;
				end
			endcase
		end
	end

//----------------------------------------------------
//-- Command BUS State Machine 
//----------------------------------------------------

// ---CMD BUS STATE Phases
    assign cmd_bus_idle_state     = (cmd_bus_state == sBUS_CMD_IDLE)       ? 1'b1 : 1'b0;
	assign cmd_bus_init_state     = (cmd_bus_state == sBUS_CMD_INIT)       ? 1'b1 : 1'b0;
	assign cmd_bus_stby_state     = (cmd_bus_state == sBUS_STANBY_STATE)   ? 1'b1 : 1'b0;
    assign cmd_bus_cmd_state      = (cmd_bus_state == sBUS_CMD_STATE)      ? 1'b1 : 1'b0;
    assign cmd_bus_txfr_state     = (cmd_bus_state == sBUS_TRANSFER_STATE) ? 1'b1 : 1'b0;
	assign cmd_bus_inactive_state = (cmd_bus_state == sBUS_INACTIVE_STATE) ? 1'b1 : 1'b0;

    always@( posedge clk or posedge spi_reset )
	begin
	    if (spi_reset)
		begin
		  cmd_bus_state <= sBUS_CMD_IDLE;
		end
		else
		begin
		  if( cmd_txrx_wait & cmd0_index ) //if CMD0
		    cmd_bus_state <= sBUS_CMD_IDLE;  
          else
		  begin
		    case (cmd_bus_state)
			    sBUS_CMD_IDLE :	
				begin
				  if( cmd_txrx_wait )
                  begin
				    if( cmd5_index )   //if CMD5 and no OCR mismatch since OCR mismatch only detected during CMD5
					  //if( ~ocr_mismatch )
					    cmd_bus_state <= sBUS_CMD_INIT;  
					  //else 
					  //  //if CMD5 and ocr mismatch then go inactive 
				      //  cmd_bus_state <= sBUS_INACTIVE_STATE;
				  end
				//  else
				 //   cmd_bus_state <= sBUS_CMD_IDLE;  
				end
				
				sBUS_CMD_INIT :
				begin
                  if( cmd_txrx_wait )
                  begin
                    if( cmd3_index )   //if CMD3
					  cmd_bus_state <= sBUS_STANBY_STATE;  
				    else 
					  if( cmd15_index )   //if CMD15 
				        cmd_bus_state <= sBUS_INACTIVE_STATE;
				  end
				 // else
				  //  cmd_bus_state <= sBUS_CMD_INIT;  
				end
				
				sBUS_STANBY_STATE :
				begin
                  if( cmd_txrx_wait )
                  begin
				    //if CMD7 with correct RCA
                    if( cmd7_index & (cmd7_15_RCA == resp6_RCA) )   
					  cmd_bus_state <= sBUS_CMD_STATE;  
					else 
					  if( cmd15_index )   //if CMD15 
				        cmd_bus_state <= sBUS_INACTIVE_STATE;
				  end
				  //else
				  //  cmd_bus_state <= sBUS_STANBY_STATE;  
				end
				
                sBUS_CMD_STATE :
				begin
                  //if( func_resume )
                  //  cmd_bus_state <= sBUS_TRANSFER_STATE;
                  //else
				    if( cmd_txrx_wait )
                    begin
				      //if CMD7 with incorrect RCA, NOTE: CMD52 reset has no need to be here since it will reset whole IP
                      if (cmd7_index & temp1) 
					    cmd_bus_state <= sBUS_STANBY_STATE;
					  //if CMD53 with valid function 
                      else if( cmd53_index & func_num_err )
                        cmd_bus_state <= sBUS_TRANSFER_STATE;
				      else 
					    if( cmd15_index )   //if CMD15
				          cmd_bus_state <= sBUS_INACTIVE_STATE;
				    end
				 //   else
				 //     cmd_bus_state <= sBUS_CMD_STATE;  
				end
					    
				sBUS_TRANSFER_STATE :
				begin
                  //if CMD52 abort OR execution complete
                  if( cmd52_abort | exec_complete )   
				    cmd_bus_state <= sBUS_CMD_STATE;  
				//  else
				//    cmd_bus_state <= sBUS_TRANSFER_STATE;
				end
					    
				sBUS_INACTIVE_STATE :
				begin
                  cmd_bus_state <= sBUS_INACTIVE_STATE;
				end

				default :
				begin
                  cmd_bus_state <= sBUS_CMD_IDLE;
				end
			endcase
		end
      end
	end					


assign temp1 = (cmd7_15_RCA == resp6_RCA) ;
//assign ocr_mismatch = ~(cmd5_ocr_register[20] & resp4_ocr_reg[20])  ;

//----------------------------------------------------
//-- Command BUS State Machine CHECKING OF VALID OR
//-- INVALID COMMANDS
//----------------------------------------------------

  always@( posedge clk or posedge spi_reset )	 
	begin
	  if( spi_reset )
	  begin
	    invalid_cmd  <= 1'b0;
	  end
	  else
	  begin
        if( spi_mode_on )
		  invalid_cmd  <= 1'b0;
        else if( cmd_txrx_rcv_cmd & (cmd_txrx_cnt == 6'h2F) )
        begin
	      case ( cmd_index )
		    6'b00_0011   :  invalid_cmd <= cmd_bus_idle_state | cmd_bus_cmd_state | 
			                               cmd_bus_txfr_state | cmd_bus_inactive_state; //CMD3
		    6'b00_0101   :  invalid_cmd <= cmd_bus_stby_state | cmd_bus_cmd_state |
			                               cmd_bus_txfr_state | cmd_bus_inactive_state; //CMD5
		    6'b00_0111   :  invalid_cmd <= cmd_bus_idle_state | cmd_bus_init_state |
			                               cmd_bus_txfr_state | cmd_bus_inactive_state; //CMD7
		    6'b00_1111   :  invalid_cmd <= cmd_bus_idle_state | cmd_bus_txfr_state |
			                               cmd_bus_inactive_state;                      //CMD15
		    6'b11_0100   :  invalid_cmd <= ~(cmd_bus_cmd_state | cmd_bus_txfr_state);   //CMD52
		    6'b11_0101   :  invalid_cmd <= ~cmd_bus_cmd_state;                          //CMD53
		    default      :  invalid_cmd <= 1'b0;                                        //CMD0 etc
		  endcase           
        end  
      end
	end

//----------------------------------------------------
//-- Command 52 byte read / write logic
//-- Command 3 create new RCA pulse 
//----------------------------------------------------

  always@( posedge clk or posedge spi_reset )	 
	begin
		if( spi_reset )
		begin
		  cmd52_rd_after_wr      <= 1'b0;
		  cmd52_write	         <= 1'b0;
		  cmd3_create_new_RCA    <= 1'b0;
		end
		else
		begin
          //write strobe during tx wait state
		  if( cmd_txrx_wait & cmd52_index & icmd52_53_rw_flag )  
		    cmd52_write	         <= 1'b1;
		  else
		    cmd52_write	         <= 1'b0;	 

          //read strobe during resp output state
		  if( (cmd_txrx_snd_resp | cmd_txrx_spicard_wait | cmd_txrx_spicard_resp) & cmd52_index )  
		    cmd52_rd_after_wr	 <= 1'b1;
		  else
		    cmd52_rd_after_wr    <= 1'b0;

          //create new RCA during tx wait state
		  if( cmd_txrx_wait & cmd3_index & ~cmd3_create_new_RCA )  
		    cmd3_create_new_RCA  <= 1'b1;
		  else
		    cmd3_create_new_RCA  <= 1'b0;

		end
	end

// Command 53 and 52 received pulse
assign icmd53_52_received = cmd_txrx_wait & (cmd52_index | cmd53_index)
                        & ~invalid_cmd & ~func_num_err;

assign cmd53_52_received = icmd53_52_received;

//latch all the cmd53 flags for data fifo and dat control use
  always@( posedge clk or posedge reset )	 
	begin
	  if( reset )
	  begin
        cmd52_53_rw_flag        <= 1'b0;   
        cmd53_block_mode        <= 1'b0;    
        cmd53_OP_code           <= 1'b0;  
        //cmd52_53_reg_addr       <= 17'b0;
        cmd52_53_func_num       <= 3'b0;
        cmd53_received          <= 1'b0;
	  end
	  else
	  begin
	    cmd53_received          <= icmd53_52_received & cmd53_index;
        if( icmd53_52_received )
		begin
	      cmd52_53_rw_flag      <= icmd52_53_rw_flag;   
          cmd53_block_mode      <= icmd53_block_mode;    
          cmd53_OP_code         <= icmd53_OP_code;  
          
          cmd52_53_func_num     <= icmd52_53_func_num;
        end
	  end
	end

assign cmd52_53_reg_addr     = icmd52_53_reg_addr;

//----------------------------------------------------
//-- SPI MODE LOGIC 
//----------------------------------------------------
assign icmd0_received = (cmd_txrx_cnt == 6'h2F) & cmd_txrx_rcv_cmd & cmd0_index & cmd_bus_idle_state;

  //used to enable spi_mode
  always@( posedge clk or posedge reset )	// NOTE: only way to reset the card is through a hard reset!!! 
	begin
	  if( reset )
	  begin
	    spi_mode_on     <= 1'b0;
	  end
	  else
	  begin
        if( icmd0_received & ~spi_csn & ~spi_mode_on )
		  spi_mode_on     <= 1'b1;
	  end
	end

  always@( posedge clk or posedge spi_reset )	// NOTE: to enable or disable the crc checking for spi mode
	begin
	  if( spi_reset )
	  begin
	    spi_crc_check_en_bit     <= 1'b0;
	  end
	  else
	  begin
        if( cmd59_index & cmd_txrx_wait & spi_mode_on )
		  spi_crc_check_en_bit   <= cmd_data[0];
	  end
	end

//-- send the spi responses on the dat0 line comes from this module on the spi_cmdrsp_sdata line
//-- the control signal is determined by the cmd_txrx_spicard_resp bit
assign spi_cmdrsp_sdata = cmd_txrx_spicard_resp ? sdata : 1'b1;

// spicard_resp1 is only 8 bits wide
assign spicard_resp1 = {1'b0, param_err, 1'b0, func_num_err, com_crc_error, illegal_command, 
                        1'b0, (cmd_bus_init_state | cmd_bus_idle_state) };
// spicard_resp4 is 40 bits wide
assign spicard_resp4 = {spicard_resp1, iresp4_card_ready, iresp4_max_num_func, iresp4_mem_present, 
                        3'b0, iresp4_ocr_reg};
// spicard_resp5 is 16 bits wide
assign spicard_resp5 = {spicard_resp1, iresp5_rd_wr_data};

// to determine how many bits is being sent out for the spi response portion
assign spicard_resp_end = 
     ((cmd0_index | cmd59_index)  & (cmd_txrx_cnt == 6'h8)) |  //CMD0 and CMD59 has R1 meaning 8 bit resp
     ((cmd52_index | cmd53_index) & (cmd_txrx_cnt == 6'hF)) |  //CMD52 and CMD 53 has R5 meaning 16bit resp
     (cmd_txrx_cnt == 6'h28);                                  //CMD5 has R4 meaning 40 bits resp   

assign host_spi_rd_cmd = cmd53_index & ~icmd52_53_rw_flag;
assign host_spi_wr_cmd = cmd53_index & icmd52_53_rw_flag;

  // used to determine if we are in write or read mode and when we finish
  // the writing or reading stages for spi
  always@( posedge clk or posedge spi_reset )	 
	begin
	  if( spi_reset )
	  begin
	    spicard_wr2host_bit     <= 1'b0;
        spicard_rd_frm_host_bit <= 1'b0;
	  end
	  else
	  begin
		if( cmd_txrx_spicard_wr2host )
	      spicard_wr2host_bit     <= 1'b1;
        else if( clear_spicard_wr2host_bit )
          spicard_wr2host_bit     <= 1'b0;
		  
        if( cmd_txrx_spicard_rd_frm_host )
		  spicard_rd_frm_host_bit <= 1'b1;
        else if( clear_spicard_rd_frm_host_bit )
          spicard_rd_frm_host_bit <= 1'b0;
	  end
	end
     
assign spi_stop_tran_cmd_rcvd = host_spi_wr_cmd & cmd_txrx_wait &     ////NOTE: this still needs to be changed to support stop tran for diff functions
                                (~|icmd52_53_reg_addr[16:3]) & (icmd52_53_reg_addr[2:0] == 3'b110) &
								~cmd52_write_data[3]; 

assign param_err = 1'b0; //ZERO FOR NOW!!!!

endmodule


















