// -----------------------------------------------------------------------------
// title          : Sensor Manager Memory Module
// project        : ULP Sensor Hub
// -----------------------------------------------------------------------------
// file           : SMMemory.v
// author         : OCTO
// company        : QuickLogic Corp
// created        : 2012/??/??	
// last update    : 2014/05/20
// platform       : PolarPro III
// standard       : Verilog 2001
// -----------------------------------------------------------------------------
// description: The Sensor Manger Memory performs several tasks. These include
//              storing Sensor Manager Instructions, Sensor Data, and FFE mail
//              box data. This memory consists of several physical memory
//              blocks.
// -----------------------------------------------------------------------------
// copyright (c) 2013
// -----------------------------------------------------------------------------
// revisions  :
// date            version    author         description
// 2014/05/20      1.0        Glen Gomes     Updated -> Added a second memory block
// -----------------------------------------------------------------------------
// Comments: This solution is specifically for use with the QuickLogic
//           PolarPro III S2 device. 
// -----------------------------------------------------------------------------

`timescale 1ns / 10ps

module SMMemory (
    // General Interface
    input         ResetIn,
	input		  SMBusyIn,
    
    //Read Interface
    input   [9:0] ReadAddressIn,
    output [17:0] ReadDataOut,
    input         ReadSelectIn,
    input         ReadClockIn,
    
    //Write Interface
    input   [9:0] WriteAddressIn,
    input   [8:0] WriteDataIn,
    input         WriteSelectIn,

    input   [9:0] WriteAddressIn_TLC,
    input   [8:0] WriteDataIn_TLC,
    input         WriteSelectIn_TLC,

    input         WriteClockIn,
	
	output 	[9:0]	SMMemory_WriteAddressIn_TLC_o,
	output 	[8:0]	SMMemory_ReadAddressIn_o,
	output 			SMMemory_WriteSelectIn_TLC_o,
	output 			SMMemory_ReadSelect_RAM0_o,
	output 			SMMemory_WriteClockIn_o,
	output 			SMMemory_ReadClockIn_o,
	output  [8:0]	SMMemory_WriteDataIn_TLC_o,
	input  	[17:0]	SMMemory_ReadDataOut_SRAM_i,
	output 	[9:0]	SMMemory_WriteAddressIn_o,
	output 		    SMMemory_WriteSelectIn_o,
	output 			SMMemory_ReadSelect_RAM1_o,
	output 			SMMemory_WriteDataIn_o,
	input  	[17:0]	SMMemory_ReadDataOut_SRAM1_i
	
	

	
    );

	// Define local variables
	//
	wire   [17:0] ReadDataOut_SRAM;
	wire   [17:0] ReadDataOut_SRAM_1;

	reg				ReadDataSel;
	wire			ReadSelect_RAM0;
	wire			ReadSelect_RAM1;
	wire			SMMemoryBankSelect;


	// generate individual read enables
	assign ReadSelect_RAM0 = ReadSelectIn && !ReadAddressIn[9];
	assign ReadSelect_RAM1 = ReadSelectIn && ReadAddressIn[9];


	// Mux the read data
	always @(posedge ReadClockIn)
		ReadDataSel <= ReadAddressIn[9];

	assign SMMemoryBankSelect = SMBusyIn ? ReadAddressIn[9] : ReadDataSel;
	assign ReadDataOut = SMMemoryBankSelect ? ReadDataOut_SRAM_1: ReadDataOut_SRAM;


	// Instantiate the Memory Blocks
	//
	assign SMMemory_WriteAddressIn_TLC_o = WriteAddressIn_TLC;
	assign SMMemory_ReadAddressIn_o[8:0] = ReadAddressIn[8:0];
	assign SMMemory_WriteSelectIn_TLC_o  = WriteSelectIn_TLC;
	assign SMMemory_ReadSelect_RAM0_o    = ReadSelect_RAM0;
	assign SMMemory_WriteClockIn_o       = WriteClockIn;
	assign SMMemory_ReadClockIn_o        = ReadClockIn;
	assign SMMemory_WriteDataIn_TLC_o    = WriteDataIn_TLC;
	assign ReadDataOut_SRAM   		     = SMMemory_ReadDataOut_SRAM_i;
	
	assign SMMemory_WriteAddressIn_o     = WriteAddressIn;
	assign SMMemory_WriteSelectIn_o      = WriteSelectIn;
	assign SMMemory_ReadSelect_RAM1_o    = ReadSelect_RAM1;
	assign SMMemory_WriteDataIn_o         = WriteDataIn;
	assign ReadDataOut_SRAM1   		     = SMMemory_ReadDataOut_SRAM1_i;

endmodule 
