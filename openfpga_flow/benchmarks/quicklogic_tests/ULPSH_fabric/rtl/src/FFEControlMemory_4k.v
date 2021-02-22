
// 4k deep CM
// This module bypasses the FFEControlMememoryMux since the addressing of the individual RAM blocks
// is not the same when coming from the TLC vs. FFE, and dynamic code updates must be supported for
// the fabric RAM's.

// Note: in order for this to work correctly, RdClk and WrClk must be the same (tied together externally).


`timescale 1ns / 10ps

`include "ulpsh_rtl_defines.v"

module FFEControlMemory_4k (
	// General Interface
	input			ResetIn,
	input			SPI_clk,
	input			TLC_FFE_clk2x_muxed,		// already muxed based on UseFastClock from TLC

	input			MemSelect_en,				// MemorySelect and enable from TLC
	input	[2:0]	MemSelect,

	input			FFE_clock_halfperiod,

	input	[11:0]	Address_TLC,				// TLC address is used for both TLC reads and writes

	input	[35:0]	MemoryMux_in,
	output	[35:0]	MemoryMux_out,

	//Read Interface
	input	[11:0]	ReadAddress_FFE,
	output	[35:0]	ReadData,
	input			ReadEnable_TLC,
	input			ReadEnable_FFE,

	//Write Interface
	input	[35:0]	WriteData_TLC,
	input			WriteEnable_TLC,

	// ASSP RAM interface - left bank
	output			assp_lb_ram0_clk,
    output	[8:0]	assp_lb_ram0_addr,
    output	[35:0]	assp_lb_ram0_wr_data,
    input	[35:0]	assp_lb_ram0_rd_data,
    output			assp_lb_ram0_wr_en,
    output			assp_lb_ram0_rd_en,
    output	[3:0]	assp_lb_ram0_wr_be,

	// ASSP RAM interface - right bank
	output			assp_rb_ram1_clk,
    output	[8:0]	assp_rb_ram1_addr,
    output	[35:0]	assp_rb_ram1_wr_data,
    input	[35:0]	assp_rb_ram1_rd_data,
    output			assp_rb_ram1_wr_en,
    output			assp_rb_ram1_rd_en,
    output	[3:0]	assp_rb_ram1_wr_be,

	// ASSP RAM interface - 8k - left bank
	output			assp_lb_ram8k_clk,
    output	[11:0]	assp_lb_ram8k_addr,
    output	[16:0]	assp_lb_ram8k_wr_data,
    input	[16:0]	assp_lb_ram8k_rd_data,
    output			assp_lb_ram8k_wr_en,
    output			assp_lb_ram8k_rd_en,
    output	[1:0]	assp_lb_ram8k_wr_be,
	
	//AP2
	output  [8:0]   FFEControlMemory_4k_Address_TLC_o      ,		
	output  [8:0]  	FFEControlMemory_4k_ReadAddress_muxed_o ,
	output    		FFEControlMemory_4k_ram5_wr_en_o,  			
	output    		FFEControlMemory_4k_ram5_rd_en_o,			
	output    		FFEControlMemory_4k_SPI_clk_o, 				
	output    		FFEControlMemory_4k_TLC_FFE_clk2x_muxed_o, 	
	output  [35:0]  FFEControlMemory_4k_WriteData_TLC_o ,
	input   [35:0]  FFEControlMemory_4k_ram5_rd_data_i	,
	output    		FFEControlMemory_4k_ram4_wr_en_o	,	
	output    		FFEControlMemory_4k_ram4_rd_en_o,
	input  [35:0]  FFEControlMemory_4k_ram4_rd_data_i,
	output [9:0]    FFEControlMemory_4k_fabric_ram1Kx9_addr_o,
	output 			FFEControlMemory_4k_ram1_wr_en_o  ,		
	output 			FFEControlMemory_4k_ram1_rd_en_o ,		
	input  [8:0]    FFEControlMemory_4k_ram1_rd_data_i
	
	
	
);

wire	[11:0]	ReadAddress_muxed;

wire			Select_from_TLC;

wire			ram0_wr_en;
wire			ram1_wr_en;
wire			ram2_wr_en;
wire			ram3_wr_en;
wire			ram4_wr_en;
wire			ram5_wr_en;

wire			ram0_rd_en;
wire			ram1_rd_en;
wire			ram2_rd_en;
wire			ram3_rd_en;
wire			ram4_rd_en;
wire			ram5_rd_en;

wire	[11:0]	assp_ram8k_addr;
wire	[9:0]	fabric_ram1Kx9_addr;

wire	[16:0]	ram0_rd_data;
wire	[8:0]	ram1_rd_data;
wire	[35:0]	ram2_rd_data;
wire	[35:0]	ram3_rd_data;
wire	[35:0]	ram4_rd_data;
wire	[35:0]	ram5_rd_data;
reg		[35:0]	lower2k_rd_data;
reg		[16:0]	lower2k_rd_data_phase0, lower2k_rd_data_phase1;
reg				ReadAddress_muxed_bit0_r1;

reg		[2:0]	ram_rd_select;
reg		[35:0]	ram_rd_data;
wire	[8:0]	assp_ram_addr;


// RAM blocks are arranged as follows:
// RAM 0: 0-2k: 4Kx17 (double-clocked to create the lower 34 bits of each uInstruction)
// RAM 1: 0-2k: 1024x9 (one-half of the 1024x9 word is used for the remaining 2 bits of each uInstruction)
// RAM 2,3: 2k-3k: ASSP RAM's (formerly 0k-1k in the 2k CM)
// RAM 4,5: 3k-4k: fabric RAM's (formerly 1k-2k in the 2k CM)


assign Select_from_TLC = (MemSelect_en && (MemSelect == 3'h0 || MemSelect == 3'h4 || MemSelect == 3'h5));

// memory mux to pass data back to TLC
assign MemoryMux_out = Select_from_TLC ? ReadData : MemoryMux_in;


// mux between the TLC and FFE control signals
assign ReadAddress_muxed = Select_from_TLC ? Address_TLC : ReadAddress_FFE;
assign ReadEnable_muxed = Select_from_TLC ? ReadEnable_TLC : ReadEnable_FFE;


// generate the read address for the 4Kx17 ASSP RAM
assign assp_ram8k_addr = Select_from_TLC ? Address_TLC : {ReadAddress_FFE[10:0], FFE_clock_halfperiod};

/// generate the read address for the 1Kx9 fabric RAM
assign fabric_ram1Kx9_addr = Select_from_TLC ? Address_TLC : ReadAddress_FFE[10:1];

// write enables for each RAM block
//		note: fabric RAM's cannot use MemSelect_en since these RAM's may be updated during run-time.
assign ram0_wr_en = (MemSelect_en && MemSelect == 3'h4 && WriteEnable_TLC);
assign ram1_wr_en = (MemSelect_en && MemSelect == 3'h5 && WriteEnable_TLC);
assign ram2_wr_en = (MemSelect_en && MemSelect == 3'h0 && WriteEnable_TLC && Address_TLC[10:9] == 2'b00);
assign ram3_wr_en = (MemSelect_en && MemSelect == 3'h0 && WriteEnable_TLC && Address_TLC[10:9] == 2'b01);
assign ram4_wr_en = (                MemSelect == 3'h0 && WriteEnable_TLC && Address_TLC[10:9] == 2'b10);
assign ram5_wr_en = (                MemSelect == 3'h0 && WriteEnable_TLC && Address_TLC[10:9] == 2'b11);

// read enables for each RAM block
assign ram0_rd_en = (MemSelect_en && MemSelect == 3'h4) ? ReadEnable_TLC : (ReadEnable_FFE && ReadAddress_FFE[11] == 1'b0);
assign ram1_rd_en = (MemSelect_en && MemSelect == 3'h5) ? ReadEnable_TLC : (ReadEnable_FFE && ReadAddress_FFE[11] == 1'b0 && FFE_clock_halfperiod);
assign ram2_rd_en = (MemSelect_en && MemSelect == 3'h0) ? (ReadEnable_TLC && Address_TLC[10:9] == 2'b00) : (ReadEnable_FFE && FFE_clock_halfperiod && ReadAddress_FFE[11:9] == 3'b100 && FFE_clock_halfperiod);
assign ram3_rd_en = (MemSelect_en && MemSelect == 3'h0) ? (ReadEnable_TLC && Address_TLC[10:9] == 2'b01) : (ReadEnable_FFE && FFE_clock_halfperiod && ReadAddress_FFE[11:9] == 3'b101 && FFE_clock_halfperiod);
assign ram4_rd_en = (MemSelect_en && MemSelect == 3'h0) ? (ReadEnable_TLC && Address_TLC[10:9] == 2'b10) : (ReadEnable_FFE && FFE_clock_halfperiod && ReadAddress_FFE[11:9] == 3'b110 && FFE_clock_halfperiod);
assign ram5_rd_en = (MemSelect_en && MemSelect == 3'h0) ? (ReadEnable_TLC && Address_TLC[10:9] == 2'b11) : (ReadEnable_FFE && FFE_clock_halfperiod && ReadAddress_FFE[11:9] == 3'b111 && FFE_clock_halfperiod);


// RAM 5 (fabric)
assign FFEControlMemory_4k_Address_TLC_o[8:0] 			= Address_TLC[8:0];
assign FFEControlMemory_4k_ReadAddress_muxed_o[8:0] 	= ReadAddress_muxed[8:0];
assign FFEControlMemory_4k_ram5_wr_en_o  				= ram5_wr_en ;
assign FFEControlMemory_4k_ram5_rd_en_o 				= ram5_rd_en;
assign FFEControlMemory_4k_SPI_clk_o 					= SPI_clk;
assign FFEControlMemory_4k_TLC_FFE_clk2x_muxed_o 		= TLC_FFE_clk2x_muxed;
assign FFEControlMemory_4k_WriteData_TLC_o 				= WriteData_TLC;
assign ram5_rd_data 									= FFEControlMemory_4k_ram5_rd_data_i;


assign FFEControlMemory_4k_ram4_wr_en_o  				= ram4_wr_en ;
assign FFEControlMemory_4k_ram4_rd_en_o 				= ram4_rd_en;
assign ram4_rd_data 									= FFEControlMemory_4k_ram4_rd_data_i;


// mappings to the ASSP RAM's

assign assp_ram_addr = (MemSelect_en && MemSelect == 3'h0) ? Address_TLC[8:0] : ReadAddress_muxed[8:0];

// RAM 3 (ASSP right bank)
//		note: the port names are still called "ram1" to maintain compatibility with the 2k CM variant
assign assp_rb_ram1_clk		= TLC_FFE_clk2x_muxed;
assign assp_rb_ram1_addr	= assp_ram_addr;
assign assp_rb_ram1_wr_data	= WriteData_TLC;
assign ram3_rd_data			= assp_rb_ram1_rd_data;
assign assp_rb_ram1_wr_en	= ram3_wr_en;
assign assp_rb_ram1_rd_en	= ram3_rd_en;
assign assp_rb_ram1_wr_be	= 4'b1111;

// RAM 2 (ASSP left bank)
//		note: the port names are still called "ram0" to maintain compatibility with the 2k CM variant
assign assp_lb_ram0_clk		= TLC_FFE_clk2x_muxed;
assign assp_lb_ram0_addr	= assp_ram_addr;
assign assp_lb_ram0_wr_data	= WriteData_TLC;
assign ram2_rd_data			= assp_lb_ram0_rd_data;
assign assp_lb_ram0_wr_en	= ram2_wr_en;
assign assp_lb_ram0_rd_en	= ram2_rd_en;
assign assp_lb_ram0_wr_be	= 4'b1111;

assign FFEControlMemory_4k_fabric_ram1Kx9_addr_o		= fabric_ram1Kx9_addr;
assign FFEControlMemory_4k_ram1_wr_en_o  				= ram1_wr_en ;
assign FFEControlMemory_4k_ram1_rd_en_o 				= ram1_rd_en;
assign FFEControlMemory_4k_SPI_clk_o 					= SPI_clk;
assign ram1_rd_data 									= FFEControlMemory_4k_ram1_rd_data_i;


// RAM 0 (ASSP 8k left bank)
assign assp_lb_ram8k_clk		= TLC_FFE_clk2x_muxed;
assign assp_lb_ram8k_addr		= assp_ram8k_addr;
assign assp_lb_ram8k_wr_data	= WriteData_TLC;
assign ram0_rd_data				= assp_lb_ram8k_rd_data;
assign assp_lb_ram8k_wr_en		= ram0_wr_en;
assign assp_lb_ram8k_rd_en		= ram0_rd_en;
assign assp_lb_ram8k_wr_be		= 2'b11;


// latch the 4Kx17 read data
always @(posedge TLC_FFE_clk2x_muxed) begin
	if (FFE_clock_halfperiod)
		lower2k_rd_data_phase0 <= ram0_rd_data;
	if (!FFE_clock_halfperiod)
		lower2k_rd_data_phase1 <= ram0_rd_data;
	if (FFE_clock_halfperiod)
		ReadAddress_muxed_bit0_r1 <= ReadAddress_muxed[0];
end

// assemble the read data for the lower 2k (ram0/ram1)
always @(*)
	if (FFE_clock_halfperiod == 0)
		if (ReadAddress_muxed_bit0_r1 == 0)
			lower2k_rd_data <= {ram1_rd_data[1:0], ram0_rd_data[16:0], lower2k_rd_data_phase0[16:0]};
		else
			lower2k_rd_data <= {ram1_rd_data[3:2], ram0_rd_data[16:0], lower2k_rd_data_phase0[16:0]};
	else
		if (ReadAddress_muxed_bit0_r1 == 0)
			lower2k_rd_data <= {ram1_rd_data[1:0], lower2k_rd_data_phase1[16:0], lower2k_rd_data_phase0[16:0]};
		else
			lower2k_rd_data <= {ram1_rd_data[3:2], lower2k_rd_data_phase1[16:0], lower2k_rd_data_phase0[16:0]};



// mux the read data from each RAM block, for the FFE
always @(posedge TLC_FFE_clk2x_muxed)
	if (FFE_clock_halfperiod)
		ram_rd_select <= ReadAddress_muxed[11:9];

always @(*)
	if (MemSelect_en)
		// TLC is reading
		if (MemSelect == 3'h0)
			case (ram_rd_select[1:0])
				2'b00:	ram_rd_data <= ram2_rd_data;
				2'b01:	ram_rd_data <= ram3_rd_data;
				2'b10:	ram_rd_data <= ram4_rd_data;
				2'b11:	ram_rd_data <= ram5_rd_data;
			endcase
		else
			if (MemSelect == 3'h4)
				ram_rd_data <= ram0_rd_data;
			else
				// assume select=5 to reduce logic

				//if (MemSelect == 3'h5)
					ram_rd_data <= ram1_rd_data;
				//else
				//	ram_rd_data <= 0;
	else
		// FFE is reading
		if (ram_rd_select[2])
			// upper 2k
			case(ram_rd_select[1:0])
				2'b00:	ram_rd_data <= ram2_rd_data;
				2'b01:	ram_rd_data <= ram3_rd_data;
				2'b10:	ram_rd_data <= ram4_rd_data;
				2'b11:	ram_rd_data <= ram5_rd_data;
			endcase
		else
			// lower 2k
			ram_rd_data <= lower2k_rd_data;

assign ReadData = ram_rd_data;


endmodule 

