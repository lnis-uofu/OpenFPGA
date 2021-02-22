`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Team: Virginia Tech Secure Embedded Systems (SES) Lab 
// Implementer: Ege Gulcan 
// 
// Create Date:    17:21:26 11/13/2013 
// Design Name: 
// Module Name:    simon_datapath_shiftreg 
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
module simon_datapath_shiftreg(clk,data_in,data_rdy,key_in,cipher_out,round_counter,bit_counter);

input clk,data_in,key_in;
input [1:0] data_rdy;
input round_counter;
output cipher_out;
output [5:0] bit_counter;

reg [55:0] shifter1;
reg [63:0] shifter2;
reg shift_in1,shift_in2;
wire shift_out1,shift_out2;
reg shifter_enable1,shifter_enable2;

reg fifo_ff63,fifo_ff62,fifo_ff61,fifo_ff60,fifo_ff59,fifo_ff58,fifo_ff57,fifo_ff56;
reg lut_ff63,lut_ff62,lut_ff61,lut_ff60,lut_ff59,lut_ff58,lut_ff57,lut_ff56;

reg lut_ff_input,fifo_ff_input;
reg lut_rol1,lut_rol2,lut_rol8;
reg s1,s4,s5,s6,s7;
reg [1:0] s3;
reg [5:0] bit_counter;
wire lut_out;



// Shift Register1 FIFO 56x1 Begin
// 56x1 Shift register to store the upper word
always @(posedge clk)
begin
	if(shifter_enable1)
	begin
		shifter1 <= {shift_in1, shifter1[55:1]};
	end
end

assign shift_out1 = shifter1[0];
// Shift Register1 End

// Shift Register2 FIFO 64x1 Begin
// 64x1 Shift register to store the lower word
always @(posedge clk)
begin
	if(shifter_enable2)
	begin
		shifter2 <= {shift_in2, shifter2[63:1]};
	end
end

assign shift_out2 = shifter2[0];
// Shift Register2 End


// 8 Flip-Flops to store the most significant 8 bits of the upper word at even rounds
// Denoted as Shift Register Up (SRU) in Figure 5
always@(posedge clk)
begin
	if(shifter_enable1)
	begin
		fifo_ff63 <= fifo_ff_input;
		fifo_ff62 <= fifo_ff63;
		fifo_ff61 <= fifo_ff62;
		fifo_ff60 <= fifo_ff61;
		fifo_ff59 <= fifo_ff60;
		fifo_ff58 <= fifo_ff59;
		fifo_ff57 <= fifo_ff58;
		fifo_ff56 <= fifo_ff57;
	end
end

// 8 Flip-Flops to store the most significant 8 bits of the upper word at odd rounds
// Denoted as Shift Register Down (SRD) in Figure 5
always@(posedge clk)
begin
	lut_ff63 <= lut_ff_input;
	lut_ff62 <= lut_ff63;
	lut_ff61 <= lut_ff62;
	lut_ff60 <= lut_ff61;
	lut_ff59 <= lut_ff60;
	lut_ff58 <= lut_ff59;
	lut_ff57 <= lut_ff58;
	lut_ff56 <= lut_ff57;
end

// FIFO 64x1 Input MUX
// Input of the lower FIFO is always the output of the upper FIFO
always@(*)
begin
		shift_in2 = shift_out1;
end

// FIFO 56x1 Input MUX
// Input of the upper FIFO depends on the select line S1
always@(*)
begin
	if(s1==0)
		shift_in1 = lut_ff56;
	else
		shift_in1 = fifo_ff56;
end

// FIFO FF Input MUX
// The input of FIFO_FF can be the input plaintext, output of 56x1 FIFO or the output of LUT
always@(*)
begin
	if(s3==0)
		fifo_ff_input = data_in;
	else if(s3==1)
		fifo_ff_input = shift_out1;
	else if(s3==2)
		fifo_ff_input = lut_out;
	else
		fifo_ff_input = 1'bx; // Debugging
end

// LUT FF Input MUX
// The input of the LUT_FF is either the output of 56x1 FIFO or the output of LUT
always@(*)
begin
	if(s5==0)
		lut_ff_input = shift_out1;
	else
		lut_ff_input = lut_out;
end

// LUT Input MUX
always@(*)
begin
	if(s7==0)
		lut_rol1 = fifo_ff63;
	else
		lut_rol1 = lut_ff63;
		
	if(s4==0)
		lut_rol2 = fifo_ff62;
	else
		lut_rol2 = lut_ff62;
		
	if(s6==0)
		lut_rol8 = fifo_ff56;
	else
		lut_rol8 = lut_ff56;
end

//Selection MUX
always@(*)
begin
	// For the first 8 bits of each even round OR for all the bits after the first 8 bits in odd rounds OR loading the plaintext  
	if((round_counter==0 && bit_counter<8)||(round_counter==1 && bit_counter>7)||(data_rdy==1))
		s1 = 1;
	else 
		s1 = 0;
		
	if(data_rdy==1) // Loading plaintext
		s3 = 0;
	else if(round_counter==0) // Even rounds
		s3 = 1;
	else if(round_counter==1) // Odd rounds
		s3 = 2;
	else 
		s3 = 1'bx; // For debugging
		
	if(round_counter==0) // Even rounds
		s6 = 0;
	else
		s6 = 1;
	
	s4 = s6;
	s7 = s6;
	s5 = ~s6;
end

// SHIFTER ENABLES
// Two shift registers are enabled when the plaintext is being loaded (1) or when the block cipher is running (3)
always@(*)
begin
	if(data_rdy==1 || data_rdy==3)
	begin
		shifter_enable1 = 1;
		shifter_enable2 = 1;
	end
	else
	begin
		shifter_enable1 = 0;
		shifter_enable2 = 0;
	end
end

// The bit_counter value is incremented in each clock cycle when the block cipher is running
always@(posedge clk)
begin
	if(data_rdy==0)
		bit_counter <= 0;
	else if(data_rdy==3)
		bit_counter <= bit_counter + 1;
	else 
		bit_counter <= bit_counter;
end

// The new computed value
assign lut_out = (lut_rol1 & lut_rol8) ^ shift_out2 ^ lut_rol2 ^ key_in;

// The global output that gives the ciphertext value
assign cipher_out = lut_out;
endmodule
