//-------------------------------------------
//    FPGA Synthesizable Verilog Netlist     
//    Description:  Verilog Decoders 
//           Author: Xifan TANG              
//        Organization: EPFL/IC/LSI          
//    Date: Thu Nov 15 14:26:09 2018
 
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

//----- BL Decoder convert 6 bits to binary 52 bits -----
module bl_decoder6to52 (
input wire enable,
input wire [5:0] addr_in,
input wire data_in,
output reg [0:51] addr_out
);
always@(addr_out,addr_in,enable, data_in)
begin
	addr_out = 52'bz;
	if (1'b1 == enable) begin
		addr_out[addr_in] = data_in;
	end
end
endmodule
//----- WL Decoder convert 6 bits to binary 52 bits -----
module wl_decoder6to52 (
input wire enable,
input wire [5:0] addr_in,
output reg [0:51] addr_out
);
always@(addr_out,addr_in,enable)
begin
	addr_out = 52'b0;
	if (1'b1 == enable) begin
		addr_out[addr_in] = 1'b1;
	end
end
endmodule
