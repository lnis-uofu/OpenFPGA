//-------------------------------------------
//	FPGA Synthesizable Verilog Netlist
//	Description: Decoders for fabric configuration protocol 
//	Author: Xifan TANG
//	Organization: University of Utah
//	Date: Wed Jun 10 20:32:39 2020
//-------------------------------------------
//----- Time scale -----
`timescale 1ns / 1ps

// ----- Verilog module for decoder3to6 -----
module decoder3to6(enable,
                   address,
                   data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:2] address;
//----- OUTPUT PORTS -----
output [0:5] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:5] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 3-bit addr to 6-bit data -----
always@(address[0:2] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:2])
			{3{1'b0}} : data_out[0:5] = 6'b100000;
			3'b100 : data_out[0:5] = 6'b010000;
			3'b010 : data_out[0:5] = 6'b001000;
			3'b110 : data_out[0:5] = 6'b000100;
			3'b001 : data_out[0:5] = 6'b000010;
			3'b101 : data_out[0:5] = 6'b000001;
			default : data_out[0:5] = {6{1'b0}};
		endcase
	end
	else begin
		data_out[0:5] = {6{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 3-bit addr to 6-bit data -----
endmodule
// ----- END Verilog module for decoder3to6 -----

// ----- Verilog module for decoder1to2 -----
module decoder1to2(enable,
                   address,
                   data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:0] address;
//----- OUTPUT PORTS -----
output [0:1] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:1] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 1-bit addr to 2-bit data -----
always@(address[0] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0])
			{1{1'b0}} : data_out[0:1] = 2'b10;
			{1{1'b1}} : data_out[0:1] = 2'b01;
			default : data_out[0:1] = {2{1'b0}};
		endcase
	end
	else begin
		data_out[0:1] = {2{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 1-bit addr to 2-bit data -----
endmodule
// ----- END Verilog module for decoder1to2 -----

// ----- Verilog module for decoder3to8 -----
module decoder3to8(enable,
                   address,
                   data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:2] address;
//----- OUTPUT PORTS -----
output [0:7] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:7] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 3-bit addr to 8-bit data -----
always@(address[0:2] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:2])
			{3{1'b0}} : data_out[0:7] = 8'b10000000;
			3'b100 : data_out[0:7] = 8'b01000000;
			3'b010 : data_out[0:7] = 8'b00100000;
			3'b110 : data_out[0:7] = 8'b00010000;
			3'b001 : data_out[0:7] = 8'b00001000;
			3'b101 : data_out[0:7] = 8'b00000100;
			3'b011 : data_out[0:7] = 8'b00000010;
			{3{1'b1}} : data_out[0:7] = 8'b00000001;
			default : data_out[0:7] = {8{1'b0}};
		endcase
	end
	else begin
		data_out[0:7] = {8{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 3-bit addr to 8-bit data -----
endmodule
// ----- END Verilog module for decoder3to8 -----

// ----- Verilog module for decoder2to3 -----
module decoder2to3(enable,
                   address,
                   data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:1] address;
//----- OUTPUT PORTS -----
output [0:2] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:2] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 2-bit addr to 3-bit data -----
always@(address[0:1] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:1])
			{2{1'b0}} : data_out[0:2] = 3'b100;
			2'b10 : data_out[0:2] = 3'b010;
			2'b01 : data_out[0:2] = 3'b001;
			default : data_out[0:2] = {3{1'b0}};
		endcase
	end
	else begin
		data_out[0:2] = {3{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 2-bit addr to 3-bit data -----
endmodule
// ----- END Verilog module for decoder2to3 -----

// ----- Verilog module for decoder4to16 -----
module decoder4to16(enable,
                    address,
                    data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:3] address;
//----- OUTPUT PORTS -----
output [0:15] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:15] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 4-bit addr to 16-bit data -----
always@(address[0:3] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:3])
			{4{1'b0}} : data_out[0:15] = 16'b1000000000000000;
			4'b1000 : data_out[0:15] = 16'b0100000000000000;
			4'b0100 : data_out[0:15] = 16'b0010000000000000;
			4'b1100 : data_out[0:15] = 16'b0001000000000000;
			4'b0010 : data_out[0:15] = 16'b0000100000000000;
			4'b1010 : data_out[0:15] = 16'b0000010000000000;
			4'b0110 : data_out[0:15] = 16'b0000001000000000;
			4'b1110 : data_out[0:15] = 16'b0000000100000000;
			4'b0001 : data_out[0:15] = 16'b0000000010000000;
			4'b1001 : data_out[0:15] = 16'b0000000001000000;
			4'b0101 : data_out[0:15] = 16'b0000000000100000;
			4'b1101 : data_out[0:15] = 16'b0000000000010000;
			4'b0011 : data_out[0:15] = 16'b0000000000001000;
			4'b1011 : data_out[0:15] = 16'b0000000000000100;
			4'b0111 : data_out[0:15] = 16'b0000000000000010;
			{4{1'b1}} : data_out[0:15] = 16'b0000000000000001;
			default : data_out[0:15] = {16{1'b0}};
		endcase
	end
	else begin
		data_out[0:15] = {16{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 4-bit addr to 16-bit data -----
endmodule
// ----- END Verilog module for decoder4to16 -----

// ----- Verilog module for decoder1to1 -----
module decoder1to1(enable,
                   address,
                   data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:0] address;
//----- OUTPUT PORTS -----
output [0:0] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:0] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 1-bit addr to 1-bit data -----
always@(address[0] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		data_out[0] = {1{1'b1}};
	end else begin
		data_out[0] = {1{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 1-bit addr to 1-bit data -----
endmodule
// ----- END Verilog module for decoder1to1 -----

// ----- Verilog module for decoder5to20 -----
module decoder5to20(enable,
                    address,
                    data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:4] address;
//----- OUTPUT PORTS -----
output [0:19] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:19] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 5-bit addr to 20-bit data -----
always@(address[0:4] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:4])
			{5{1'b0}} : data_out[0:19] = 20'b10000000000000000000;
			5'b10000 : data_out[0:19] = 20'b01000000000000000000;
			5'b01000 : data_out[0:19] = 20'b00100000000000000000;
			5'b11000 : data_out[0:19] = 20'b00010000000000000000;
			5'b00100 : data_out[0:19] = 20'b00001000000000000000;
			5'b10100 : data_out[0:19] = 20'b00000100000000000000;
			5'b01100 : data_out[0:19] = 20'b00000010000000000000;
			5'b11100 : data_out[0:19] = 20'b00000001000000000000;
			5'b00010 : data_out[0:19] = 20'b00000000100000000000;
			5'b10010 : data_out[0:19] = 20'b00000000010000000000;
			5'b01010 : data_out[0:19] = 20'b00000000001000000000;
			5'b11010 : data_out[0:19] = 20'b00000000000100000000;
			5'b00110 : data_out[0:19] = 20'b00000000000010000000;
			5'b10110 : data_out[0:19] = 20'b00000000000001000000;
			5'b01110 : data_out[0:19] = 20'b00000000000000100000;
			5'b11110 : data_out[0:19] = 20'b00000000000000010000;
			5'b00001 : data_out[0:19] = 20'b00000000000000001000;
			5'b10001 : data_out[0:19] = 20'b00000000000000000100;
			5'b01001 : data_out[0:19] = 20'b00000000000000000010;
			5'b11001 : data_out[0:19] = 20'b00000000000000000001;
			default : data_out[0:19] = {20{1'b0}};
		endcase
	end
	else begin
		data_out[0:19] = {20{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 5-bit addr to 20-bit data -----
endmodule
// ----- END Verilog module for decoder5to20 -----

// ----- Verilog module for decoder5to18 -----
module decoder5to18(enable,
                    address,
                    data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:4] address;
//----- OUTPUT PORTS -----
output [0:17] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:17] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 5-bit addr to 18-bit data -----
always@(address[0:4] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:4])
			{5{1'b0}} : data_out[0:17] = 18'b100000000000000000;
			5'b10000 : data_out[0:17] = 18'b010000000000000000;
			5'b01000 : data_out[0:17] = 18'b001000000000000000;
			5'b11000 : data_out[0:17] = 18'b000100000000000000;
			5'b00100 : data_out[0:17] = 18'b000010000000000000;
			5'b10100 : data_out[0:17] = 18'b000001000000000000;
			5'b01100 : data_out[0:17] = 18'b000000100000000000;
			5'b11100 : data_out[0:17] = 18'b000000010000000000;
			5'b00010 : data_out[0:17] = 18'b000000001000000000;
			5'b10010 : data_out[0:17] = 18'b000000000100000000;
			5'b01010 : data_out[0:17] = 18'b000000000010000000;
			5'b11010 : data_out[0:17] = 18'b000000000001000000;
			5'b00110 : data_out[0:17] = 18'b000000000000100000;
			5'b10110 : data_out[0:17] = 18'b000000000000010000;
			5'b01110 : data_out[0:17] = 18'b000000000000001000;
			5'b11110 : data_out[0:17] = 18'b000000000000000100;
			5'b00001 : data_out[0:17] = 18'b000000000000000010;
			5'b10001 : data_out[0:17] = 18'b000000000000000001;
			default : data_out[0:17] = {18{1'b0}};
		endcase
	end
	else begin
		data_out[0:17] = {18{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 5-bit addr to 18-bit data -----
endmodule
// ----- END Verilog module for decoder5to18 -----

// ----- Verilog module for decoder4to13 -----
module decoder4to13(enable,
                    address,
                    data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:3] address;
//----- OUTPUT PORTS -----
output [0:12] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:12] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 4-bit addr to 13-bit data -----
always@(address[0:3] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:3])
			{4{1'b0}} : data_out[0:12] = 13'b1000000000000;
			4'b1000 : data_out[0:12] = 13'b0100000000000;
			4'b0100 : data_out[0:12] = 13'b0010000000000;
			4'b1100 : data_out[0:12] = 13'b0001000000000;
			4'b0010 : data_out[0:12] = 13'b0000100000000;
			4'b1010 : data_out[0:12] = 13'b0000010000000;
			4'b0110 : data_out[0:12] = 13'b0000001000000;
			4'b1110 : data_out[0:12] = 13'b0000000100000;
			4'b0001 : data_out[0:12] = 13'b0000000010000;
			4'b1001 : data_out[0:12] = 13'b0000000001000;
			4'b0101 : data_out[0:12] = 13'b0000000000100;
			4'b1101 : data_out[0:12] = 13'b0000000000010;
			4'b0011 : data_out[0:12] = 13'b0000000000001;
			default : data_out[0:12] = {13{1'b0}};
		endcase
	end
	else begin
		data_out[0:12] = {13{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 4-bit addr to 13-bit data -----
endmodule
// ----- END Verilog module for decoder4to13 -----

// ----- Verilog module for decoder4to11 -----
module decoder4to11(enable,
                    address,
                    data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:3] address;
//----- OUTPUT PORTS -----
output [0:10] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:10] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 4-bit addr to 11-bit data -----
always@(address[0:3] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:3])
			{4{1'b0}} : data_out[0:10] = 11'b10000000000;
			4'b1000 : data_out[0:10] = 11'b01000000000;
			4'b0100 : data_out[0:10] = 11'b00100000000;
			4'b1100 : data_out[0:10] = 11'b00010000000;
			4'b0010 : data_out[0:10] = 11'b00001000000;
			4'b1010 : data_out[0:10] = 11'b00000100000;
			4'b0110 : data_out[0:10] = 11'b00000010000;
			4'b1110 : data_out[0:10] = 11'b00000001000;
			4'b0001 : data_out[0:10] = 11'b00000000100;
			4'b1001 : data_out[0:10] = 11'b00000000010;
			4'b0101 : data_out[0:10] = 11'b00000000001;
			default : data_out[0:10] = {11{1'b0}};
		endcase
	end
	else begin
		data_out[0:10] = {11{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 4-bit addr to 11-bit data -----
endmodule
// ----- END Verilog module for decoder4to11 -----

// ----- Verilog module for decoder4to12 -----
module decoder4to12(enable,
                    address,
                    data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:3] address;
//----- OUTPUT PORTS -----
output [0:11] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:11] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 4-bit addr to 12-bit data -----
always@(address[0:3] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:3])
			{4{1'b0}} : data_out[0:11] = 12'b100000000000;
			4'b1000 : data_out[0:11] = 12'b010000000000;
			4'b0100 : data_out[0:11] = 12'b001000000000;
			4'b1100 : data_out[0:11] = 12'b000100000000;
			4'b0010 : data_out[0:11] = 12'b000010000000;
			4'b1010 : data_out[0:11] = 12'b000001000000;
			4'b0110 : data_out[0:11] = 12'b000000100000;
			4'b1110 : data_out[0:11] = 12'b000000010000;
			4'b0001 : data_out[0:11] = 12'b000000001000;
			4'b1001 : data_out[0:11] = 12'b000000000100;
			4'b0101 : data_out[0:11] = 12'b000000000010;
			4'b1101 : data_out[0:11] = 12'b000000000001;
			default : data_out[0:11] = {12{1'b0}};
		endcase
	end
	else begin
		data_out[0:11] = {12{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 4-bit addr to 12-bit data -----
endmodule
// ----- END Verilog module for decoder4to12 -----

// ----- Verilog module for decoder4to15 -----
module decoder4to15(enable,
                    address,
                    data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:3] address;
//----- OUTPUT PORTS -----
output [0:14] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:14] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 4-bit addr to 15-bit data -----
always@(address[0:3] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:3])
			{4{1'b0}} : data_out[0:14] = 15'b100000000000000;
			4'b1000 : data_out[0:14] = 15'b010000000000000;
			4'b0100 : data_out[0:14] = 15'b001000000000000;
			4'b1100 : data_out[0:14] = 15'b000100000000000;
			4'b0010 : data_out[0:14] = 15'b000010000000000;
			4'b1010 : data_out[0:14] = 15'b000001000000000;
			4'b0110 : data_out[0:14] = 15'b000000100000000;
			4'b1110 : data_out[0:14] = 15'b000000010000000;
			4'b0001 : data_out[0:14] = 15'b000000001000000;
			4'b1001 : data_out[0:14] = 15'b000000000100000;
			4'b0101 : data_out[0:14] = 15'b000000000010000;
			4'b1101 : data_out[0:14] = 15'b000000000001000;
			4'b0011 : data_out[0:14] = 15'b000000000000100;
			4'b1011 : data_out[0:14] = 15'b000000000000010;
			4'b0111 : data_out[0:14] = 15'b000000000000001;
			default : data_out[0:14] = {15{1'b0}};
		endcase
	end
	else begin
		data_out[0:14] = {15{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 4-bit addr to 15-bit data -----
endmodule
// ----- END Verilog module for decoder4to15 -----

// ----- Verilog module for decoder4to10 -----
module decoder4to10(enable,
                    address,
                    data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:3] address;
//----- OUTPUT PORTS -----
output [0:9] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:9] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 4-bit addr to 10-bit data -----
always@(address[0:3] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:3])
			{4{1'b0}} : data_out[0:9] = 10'b1000000000;
			4'b1000 : data_out[0:9] = 10'b0100000000;
			4'b0100 : data_out[0:9] = 10'b0010000000;
			4'b1100 : data_out[0:9] = 10'b0001000000;
			4'b0010 : data_out[0:9] = 10'b0000100000;
			4'b1010 : data_out[0:9] = 10'b0000010000;
			4'b0110 : data_out[0:9] = 10'b0000001000;
			4'b1110 : data_out[0:9] = 10'b0000000100;
			4'b0001 : data_out[0:9] = 10'b0000000010;
			4'b1001 : data_out[0:9] = 10'b0000000001;
			default : data_out[0:9] = {10{1'b0}};
		endcase
	end
	else begin
		data_out[0:9] = {10{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 4-bit addr to 10-bit data -----
endmodule
// ----- END Verilog module for decoder4to10 -----

// ----- Verilog module for decoder3to5 -----
module decoder3to5(enable,
                   address,
                   data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:2] address;
//----- OUTPUT PORTS -----
output [0:4] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:4] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 3-bit addr to 5-bit data -----
always@(address[0:2] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:2])
			{3{1'b0}} : data_out[0:4] = 5'b10000;
			3'b100 : data_out[0:4] = 5'b01000;
			3'b010 : data_out[0:4] = 5'b00100;
			3'b110 : data_out[0:4] = 5'b00010;
			3'b001 : data_out[0:4] = 5'b00001;
			default : data_out[0:4] = {5{1'b0}};
		endcase
	end
	else begin
		data_out[0:4] = {5{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 3-bit addr to 5-bit data -----
endmodule
// ----- END Verilog module for decoder3to5 -----

// ----- Verilog module for decoder6to33 -----
module decoder6to33(enable,
                    address,
                    data_out);
//----- INPUT PORTS -----
input [0:0] enable;
//----- INPUT PORTS -----
input [0:5] address;
//----- OUTPUT PORTS -----
output [0:32] data_out;

//----- BEGIN wire-connection ports -----
//----- END wire-connection ports -----


//----- BEGIN Registered ports -----
reg [0:32] data_out;
//----- END Registered ports -----

// ----- BEGIN Verilog codes for Decoder convert 6-bit addr to 33-bit data -----
always@(address[0:5] or enable[0]) begin
	if (enable[0] == 1'b1) begin
		case (address[0:5])
			{6{1'b0}} : data_out[0:32] = 33'b100000000000000000000000000000000;
			6'b100000 : data_out[0:32] = 33'b010000000000000000000000000000000;
			6'b010000 : data_out[0:32] = 33'b001000000000000000000000000000000;
			6'b110000 : data_out[0:32] = 33'b000100000000000000000000000000000;
			6'b001000 : data_out[0:32] = 33'b000010000000000000000000000000000;
			6'b101000 : data_out[0:32] = 33'b000001000000000000000000000000000;
			6'b011000 : data_out[0:32] = 33'b000000100000000000000000000000000;
			6'b111000 : data_out[0:32] = 33'b000000010000000000000000000000000;
			6'b000100 : data_out[0:32] = 33'b000000001000000000000000000000000;
			6'b100100 : data_out[0:32] = 33'b000000000100000000000000000000000;
			6'b010100 : data_out[0:32] = 33'b000000000010000000000000000000000;
			6'b110100 : data_out[0:32] = 33'b000000000001000000000000000000000;
			6'b001100 : data_out[0:32] = 33'b000000000000100000000000000000000;
			6'b101100 : data_out[0:32] = 33'b000000000000010000000000000000000;
			6'b011100 : data_out[0:32] = 33'b000000000000001000000000000000000;
			6'b111100 : data_out[0:32] = 33'b000000000000000100000000000000000;
			6'b000010 : data_out[0:32] = 33'b000000000000000010000000000000000;
			6'b100010 : data_out[0:32] = 33'b000000000000000001000000000000000;
			6'b010010 : data_out[0:32] = 33'b000000000000000000100000000000000;
			6'b110010 : data_out[0:32] = 33'b000000000000000000010000000000000;
			6'b001010 : data_out[0:32] = 33'b000000000000000000001000000000000;
			6'b101010 : data_out[0:32] = 33'b000000000000000000000100000000000;
			6'b011010 : data_out[0:32] = 33'b000000000000000000000010000000000;
			6'b111010 : data_out[0:32] = 33'b000000000000000000000001000000000;
			6'b000110 : data_out[0:32] = 33'b000000000000000000000000100000000;
			6'b100110 : data_out[0:32] = 33'b000000000000000000000000010000000;
			6'b010110 : data_out[0:32] = 33'b000000000000000000000000001000000;
			6'b110110 : data_out[0:32] = 33'b000000000000000000000000000100000;
			6'b001110 : data_out[0:32] = 33'b000000000000000000000000000010000;
			6'b101110 : data_out[0:32] = 33'b000000000000000000000000000001000;
			6'b011110 : data_out[0:32] = 33'b000000000000000000000000000000100;
			6'b111110 : data_out[0:32] = 33'b000000000000000000000000000000010;
			6'b000001 : data_out[0:32] = 33'b000000000000000000000000000000001;
			default : data_out[0:32] = {33{1'b0}};
		endcase
	end
	else begin
		data_out[0:32] = {33{1'b0}};
	end
end
// ----- END Verilog codes for Decoder convert 6-bit addr to 33-bit data -----
endmodule
// ----- END Verilog module for decoder6to33 -----

