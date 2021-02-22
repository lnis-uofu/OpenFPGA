`timescale 1ns / 10ps

module FFEDataMemoryMux (
	input		Select,
	
	input [9:0]		ReadAddressIn0,
	input [9:0]		ReadAddressIn1,
	output[9:0]		ReadAddressOut,
	
	input [9:0]		WriteAddressIn0,
	input [9:0]		WriteAddressIn1,
	output[9:0]		WriteAddressOut,
	
	input [35:0]	DataToMemoryIn0,
	input [35:0]	DataToMemoryIn1,
	output[35:0]	DataToMemoryOut,
	
	input [35:0]	DataFromMemoryIn0,
	input [35:0]	DataFromMemoryIn1,
	output[35:0]	DataFromMemoryOut,
	
	input			ReadEnable0,
	input			ReadEnable1,
	output			ReadEnable,
	
	input			WriteEnable0,
	input			WriteEnable1,
	output			WriteEnable
	);
	
	assign ReadAddressOut = (Select) ? ReadAddressIn1 : ReadAddressIn0;
	assign WriteAddressOut = (Select) ? WriteAddressIn1 : WriteAddressIn0;
	assign DataToMemoryOut = (Select) ? DataToMemoryIn1 : DataToMemoryIn0;
	assign DataFromMemoryOut = (Select) ? DataFromMemoryIn1 : DataFromMemoryIn0;
	assign ReadEnable = (Select) ? ReadEnable1 : ReadEnable0;
	assign WriteEnable = (Select) ? WriteEnable1 : WriteEnable0;

endmodule
