`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Team: Virginia Tech Secure Embedded Systems (SES) Lab 
// Implementer: Ege Gulcan
// 
// Create Date:    16:55:06 11/12/2013 
// Design Name: 
// Module Name:    simon_key_expansion_shiftreg 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////

module simon_key_expansion_shiftreg(clk,data_in,key_out,data_rdy,bit_counter,round_counter_out);

input clk;
input data_in;
input [1:0] data_rdy;
input [5:0] bit_counter;
output key_out;
output round_counter_out;


reg [59:0] shifter1;
reg [63:0] shifter2;
reg shift_in1,shift_in2;
wire shift_out1,shift_out2;
reg shifter_enable1,shifter_enable2;

reg lut_ff_enable,fifo_ff_enable;
wire lut_out;
reg lut_in3;
reg s2,s3;
reg [1:0] s1;
reg [6:0] round_counter;
reg z_value;

reg fifo_ff0,fifo_ff1,fifo_ff2,fifo_ff3;

//(* shreg_extract = "no" *)
reg lut_ff0,lut_ff1,lut_ff2,lut_ff3;
//Constant value Z ROM
reg [0:67] Z = 68'b10101111011100000011010010011000101000010001111110010110110011101011;
reg c;


/////////////////////////////////////////
//// BEGIN CODE ////////////////////////
///////////////////////////////////////

// Least bit of the round counter is sent to the datapath to check if it is even or odd
assign round_counter_out = round_counter[0];

// Shift Register1 FIFO 60x1 Begin
// 60x1 shift register storing the 60 most significant bits of the upper word of the key
always @(posedge clk)
begin
	if(shifter_enable1)
	begin
		shifter1 <= {shift_in1, shifter1[59:1]};
	end
end

assign shift_out1 = shifter1[0];
// Shift Register1 End

// Shift Register2 FIFO 64x1 Begin
// 64x1 shift register storing the lower word of the key
always @(posedge clk)
begin
	if(shifter_enable2)
	begin
		shifter2 <= {shift_in2, shifter2[63:1]};
	end
end

assign shift_out2 = shifter2[0];
// Shift Register2 End

// 4 flip-flops storing the least significant 4 bits of the upper word in the first round
always @(posedge clk)
begin
	if(fifo_ff_enable)
	begin
		fifo_ff3 <= shift_out1;
		fifo_ff2 <= fifo_ff3;
		fifo_ff1 <= fifo_ff2;
		fifo_ff0 <= fifo_ff1;
	end
end

// 4 flip-flops storing the least significant 4 bits of the upper word after the first round
always@(posedge clk)
begin
	if(lut_ff_enable)
	begin
		lut_ff3 <= lut_out;
		lut_ff2 <= lut_ff3;
		lut_ff1 <= lut_ff2;
		lut_ff0 <= lut_ff1;
	end
end

//FIFO 64x1 Input MUX
always@(*)
begin
	if(data_rdy==2)
		shift_in2 = fifo_ff0;
	else if(data_rdy==3 && (round_counter<1 || bit_counter>3))
		shift_in2 = fifo_ff0;
	else if(data_rdy==3 && bit_counter<4 && round_counter>0) 
		shift_in2 = lut_ff0; 
	else
		shift_in2 = 1'bx;
end

//LUT >>3 Input MUX
always@(*)
begin
	if(s2==0)
		lut_in3 = fifo_ff3;
	else
		lut_in3 = lut_ff3;
end

//FIFO 60x1 Input MUX
always@(*)
begin
	if(s1==0)
		shift_in1 = fifo_ff0;
	else if(s1==1)
		shift_in1 = data_in;
	else if(s1==2)
		shift_in1 = lut_out;
	else if(s1==3)
		shift_in1 = lut_ff0;
	else
		shift_in1 = 1'bx;
end

//S2 MUX
always@(*)
begin
	if(bit_counter==0 && round_counter!=0)
		s2 = 1;
	else
		s2 = 0;
end

//S1 MUX
always@(*)
begin
	if(data_rdy==2)
		s1 = 1;
	else if(data_rdy==3 && bit_counter<4 && round_counter==0)
		s1 = 0;
	else if(data_rdy==3 && bit_counter<4 && round_counter>0)
		s1 = 3;
	else
		s1 = 2;
end

// LUT FF ENABLE MUX
// LUT FFs are used only at the first four clock cycles of each round
always@(*)
begin
	if(data_rdy==3 && bit_counter<4)
		lut_ff_enable = 1;
	else
		lut_ff_enable = 0;
end

//FIFO FF ENABLE MUX
always@(*)
begin
	if(data_rdy==2 || data_rdy==3)
		fifo_ff_enable = 1;
	else
		fifo_ff_enable = 0;
end

//SHIFTER ENABLES
// Shifters are enabled when the key is loaded or block cipher is running
always@(*)
begin
	if(data_rdy==2 || data_rdy==3)
		shifter_enable1 = 1;
	else
		shifter_enable1 = 0;
	
	if(data_rdy==2 || data_rdy==3)
		shifter_enable2 = 1;
	else
		shifter_enable2 = 0;
		
end

//Round Counter
always@(posedge clk)
begin
	if(data_rdy==3 && bit_counter==63)
		round_counter <= round_counter + 1;
	else if(data_rdy==0)
		round_counter <= 0;
	else
		round_counter <= round_counter;
end

// The necessary bit of the constant Z is selected by the round counter
always @(*)
begin
	if(bit_counter==0)
		z_value = Z[round_counter];
	else
		z_value = 0;
end

// The value of c is 1 at the first two cycles of each round only
always @(*)
begin
	if(bit_counter==0 || bit_counter==1)
		c = 0;
	else 
		c = 1;
end

// New computed key bit
assign lut_out = shift_out2 ^ lut_in3 ^ shift_out1 ^ z_value ^ c;

// Output key bit that is connected to the datapath	
assign key_out = shift_out2;
	
endmodule
