//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description:  Verilog Decoders 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:04 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- BL Decoder convert 5 bits to binary 19 bits -----
module bl_decoder5to19 (
input wire enable,
input wire [4:0] addr_in,
input wire data_in,
output reg [0:18] addr_out
);
always@(addr_out,addr_in,enable, data_in)
begin
	addr_out = 19'bz;
	if (1'b1 == enable) begin
		addr_out[addr_in] = data_in;
	end
end
endmodule
//----- WL Decoder convert 5 bits to binary 19 bits -----
module wl_decoder5to19 (
input wire enable,
input wire [4:0] addr_in,
output reg [0:18] addr_out
);
always@(addr_out,addr_in,enable)
begin
	addr_out = 19'b0;
	if (1'b1 == enable) begin
		addr_out[addr_in] = 1'b1;
	end
end
endmodule
