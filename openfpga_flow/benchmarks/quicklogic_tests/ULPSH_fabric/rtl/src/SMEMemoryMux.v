// -----------------------------------------------------------------------------
// title          : Sensor Manager Memory Module
// project        : ULP Sensor Hub
// -----------------------------------------------------------------------------
// file           : SMEMemoryMux.v
// author         : OCTO
// company        : QuickLogic Corp
// created        : 2012/??/??	
// last update    : 2014/05/20
// platform       : PolarPro III
// standard       : Verilog 2001
// -----------------------------------------------------------------------------
// description: The Sensor Manger Memory Mux selects between several sources
//              for passing both read and write information. 
// -----------------------------------------------------------------------------
// copyright (c) 2013
// -----------------------------------------------------------------------------
// revisions  :
// date            version    author         description
// 2014/05/20      1.0        Glen Gomes     Updated -> Added support for a second 
//                                                      memory block by adding
//                                                      address bits.
// -----------------------------------------------------------------------------
// Comments: This solution is specifically for use with the QuickLogic
//           PolarPro III S2 device. 
// -----------------------------------------------------------------------------

`timescale 1ns / 10ps

module SMEMemoryMux (
	input		Select,
	
	input [9:0]		ReadAddressIn0,    // Expanded for Rel 0 on 6/18
	input [9:0]		ReadAddressIn1,    // Expanded for Rel 0 on 6/18
	output[9:0]		ReadAddressOut,    // Expanded for Rel 0 on 6/18
	
	input [9:0]		WriteAddressIn0,
	input [9:0]		WriteAddressIn1,
	output[9:0]		WriteAddressOut,
	
	input [8:0]		DataToMemoryIn0,
	input [8:0]		DataToMemoryIn1,
	output[8:0]		DataToMemoryOut,
	
	input [17:0]	DataFromMemoryIn0,
	input [17:0]	DataFromMemoryIn1,
	output[17:0]	DataFromMemoryOut,
	
	input			ReadEnableIn0,
	input			ReadEnableIn1,
	output			ReadEnableOut,
	
	input			WriteEnableIn0,
	input			WriteEnableIn1,
	output			WriteEnableOut,
	
	input			ReadClockIn0,
	input			ReadClockIn1,
	output			ReadClockOut	
	);
	
	assign ReadAddressOut = (Select) ? ReadAddressIn1 : ReadAddressIn0;
	assign WriteAddressOut = (Select) ? WriteAddressIn1 : WriteAddressIn0;
	assign DataToMemoryOut = (Select) ? DataToMemoryIn1 : DataToMemoryIn0;
	assign DataFromMemoryOut = (Select) ? DataFromMemoryIn1 : DataFromMemoryIn0;
	assign ReadEnableOut = (Select) ? ReadEnableIn1 : ReadEnableIn0;
	assign WriteEnableOut = (Select) ? WriteEnableIn1 : WriteEnableIn0;
	assign ReadClockOut = (Select) ? ReadClockIn1 : ReadClockIn0;

endmodule
