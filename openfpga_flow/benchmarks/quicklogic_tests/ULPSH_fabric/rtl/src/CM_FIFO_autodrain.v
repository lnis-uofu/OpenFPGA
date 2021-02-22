/*-----------------------------------------------------------------------------
CM_FIFO_autodrain
	This module will auto-drain (read from) the CM FIFO when RingBufferMode
	is enabled. It will stop on packet boundaries (designated by bit 8 of
	the FIFO data).
-----------------------------------------------------------------------------*/


`timescale 1ns / 10ps


module CM_FIFO_autodrain (
	input			rst,
	input			FFE_CLK_gclk,
	
	input			RingBufferMode,
	input	[3:0]	CM_FIFO_PushFlags,
	input			CM_FIFO_Empty,
	input			CM_FIFO_PopFromTLC,
	input	[8:0]	CM_FIFO_ReadData,
	
	output			CM_FIFO_Pop,
	output			busy,
	output			TP1,
	output			TP2
);


// state definitions
localparam ST_IDLE		= 3'b000;
localparam ST_SETBUSY1	= 3'b001;
localparam ST_SETBUSY2	= 3'b010;
localparam ST_WAIT		= 3'b011;
localparam ST_READ		= 3'b100;


wire			SOP_Marker;
wire			FIFO_AutoRead_Threshold;
reg				RingBufferMode_r1;
reg				RingBufferMode_r2;
reg		[2:0]	state;
reg				busy_reg;
reg				CM_FIFO_PopAutoDrain;


assign SOP_Marker = CM_FIFO_ReadData[8];


/*	PUSH_FLAG:
	0x0: full
	0x1: empty
	0x2: room for 1/2 to (full - 1)
	0x3: room for 1/4 to (1/2 -1)
	0x4: room for 64 to (1/4 - 1)
	0xA: room for 32 to 63
	0xB: room for 16 to 31
	0xC: room for 8 to 15
	0xD: room for 4 to 7
	0xE: room for 2 to 3
	0xF: room for 1
	others: reserved					*/

assign FIFO_AutoRead_Threshold = ((CM_FIFO_PushFlags == 4'h0) || (CM_FIFO_PushFlags[3]));	// full or (63 or less) 16-bit words


// sync RingBufferMode to the FFE clk
always @(posedge rst or posedge FFE_CLK_gclk)
	if (rst) begin
		RingBufferMode_r1 <= 0;
		RingBufferMode_r2 <= 0;
	end
		else begin
			RingBufferMode_r1 <= RingBufferMode;
			RingBufferMode_r2 <= RingBufferMode_r1;
		end


// state machine
always @(posedge rst or posedge FFE_CLK_gclk)
	if (rst)
		state <= ST_IDLE;
	else
		case (state)
			ST_IDLE:		if (RingBufferMode_r2)
								state <= ST_SETBUSY1;
							else
								state <= ST_IDLE;
			
			ST_SETBUSY1:	state <= ST_SETBUSY2;					// allow time for the FIFO read clock to switch safely
			
			ST_SETBUSY2:	state <= ST_WAIT;

			ST_WAIT:		if (!RingBufferMode_r2)
								state <= ST_IDLE;
							else
								state <= ST_READ;
			
			ST_READ:		if (SOP_Marker && !RingBufferMode_r2)
								state <= ST_SETBUSY1;				// goto ST_SETBUSY1 to allow time to switch to SPI_SCLK
							else
								state <= ST_READ;
		endcase


// busy
wire	busy_reg_reset;
assign busy_reg_reset = rst || !RingBufferMode;
always @(posedge busy_reg_reset or posedge FFE_CLK_gclk)
	if (busy_reg_reset)
		busy_reg <= 0;
	else
		case (busy_reg)
			1'b0:	if ((state == ST_IDLE) && (RingBufferMode_r2))
						busy_reg <= 1;
					else	
						busy_reg <= 0;
			1'b1:	if (((state == ST_SETBUSY1) && !RingBufferMode_r2) || (state == ST_IDLE))
						busy_reg <= 0;
					else	
						busy_reg <= 1;
		endcase



// FIFO Read control
always @(*)
	if (state == ST_READ)											// pop only allowed in ST_READ state...
		if (!CM_FIFO_Empty)											// ...and FIFO not empty
			if (!SOP_Marker)												// if not on SOP marker, keep reading
				CM_FIFO_PopAutoDrain <= 1;
			else	// (SOP_Marker)
				if (FIFO_AutoRead_Threshold && RingBufferMode_r2)			// if SOP marker, read next packet if FIFO is at or past threshold and RingBufferMode still on
					CM_FIFO_PopAutoDrain <= 1;
				else
					CM_FIFO_PopAutoDrain <= 0;						// else pop=0
		else	// (CM_FIFO_Empty)
			CM_FIFO_PopAutoDrain <= 0;
	else	// (state != ST_READ)
		CM_FIFO_PopAutoDrain <= 0;


assign CM_FIFO_Pop = busy_reg ? CM_FIFO_PopAutoDrain : CM_FIFO_PopFromTLC;

assign busy = busy_reg;


assign TP1 = FIFO_AutoRead_Threshold;
assign TP2 = 0;


endmodule

