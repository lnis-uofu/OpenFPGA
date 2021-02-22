
/*------------------------------------------------------------------------------
SPI_slave
	SPI slave interface, designed for the ULP Sensor Hub.
	This module is designed to be as small and simple as possible, while
	supporting the ULP Sensor Hub. Only supports SPI Mode 0 (CPOL=CPHA=0)...
	which means that input data is latched on the positive edge of SPI_SCLK,
	and driven out on the negative edge of SPI_SCLK, with the base value of
	SPI_SCLK being 0.


	SPI Protocol:
		Writes:		MOSI: A0 + D0 + D1 + ... + Dn
					MISO: xx + xx + xx + ... + xx

		Reads:		MOSI: A0 + xx + xx + ... + xx
					MISO: xx + xx + D0 + ... + Dn

		A0 = [1-bit R/W: 1=write, 0=read] + [7-bit address]
		Dn = valid data byte
		xx = don't-care data byte

		It is assumed that the MSb is transmitted first, and the LSb last.

		The address is latched, and auto-incremented to support burst reads/writes.
		The address, when 0d11, jumps back to 0d07, to support repeated (burst)
		reads to/from the memory data port. The logic to re-map addresses above
		0d11, that previously was in TLC.v, can now be removed. New registers
		above 0d11 may now be added if needed.

		This SPI slave requires extra edges on SPI_SCLK to complete any write
		operation. This may be accomplished in any one of several ways:
			1. A (non-destructive) read should be performed following the last
			   write operation.
			2. SPI_SCLK should be toggled after SPI_SS goes inactive
			   (a free-running SPI_SCLK would accomplish this).
			3. A few extra bits (totaling less than a full byte) should be
			   transmitted by the master. These extra bits will be ignored by
			   this core, but will provide the clocks needed to generated the
			   wr_data_valid pulse.
			4. A "null" transaction should be performed following the last
			   write transaction, during which the address byte is transmitted
			   by the SPI master followed by 0 bytes of read/write data.

------------------------------------------------------------------------------*/




`timescale 1ns / 1ns

`include "SensorHubDefines.v"


module SPI_slave (
	input			rst,			// system/global reset (active-high)

	// SPI interface
	input			SPI_SCLK,		// base value 0 (mode 0)
	input			SPI_MOSI,		// master out, slave in
	output			SPI_MISO,		// master in, slave out
	input			SPI_SS,			// slave select (active-low)

	// internal interface
	output	[6:0]	addr,
	output	[7:0]	wr_data,
	output			wr_data_valid,	// active high
	input	[7:0]	rd_data,
	output			rd_data_ack
);


parameter		WR = 1'b1;
parameter		RD = 1'b0;


wire			rst_int;

reg		[7:0]	shift_in;
reg		[7:0]	shift_out;

reg		[2:0]	bit_cnt, bit_cnt_neg;

reg				rcv_byte_valid;
reg				addr_has_been_latched;
reg				first_data_has_been_latched;
reg		[6:0]	addr_reg;
reg				write_readn;
reg		[7:0]	write_data_reg;
reg				wr_data_valid_reg;
reg				write_pending;
reg				rd_data_ack_reg;



// rst_int is active when the global rst occurs or when the SPI interface is idle.
//    Some logic needs to remain active after a SPI transaction occurs, so rst will be used in those cases.
assign rst_int = rst || SPI_SS;

// input shift register
always @(posedge rst_int or posedge SPI_SCLK)
	if (rst_int)
		shift_in <= 8'b0;
	else
		if (!SPI_SS)
			shift_in <= {shift_in[6:0], SPI_MOSI};
		else
			shift_in <= shift_in;


// bit counter
always @(posedge rst_int or posedge SPI_SCLK)
	if (rst_int)
		bit_cnt <= 3'b0;
	else
		if (!SPI_SS)
			bit_cnt <= bit_cnt + 1;
		else
			bit_cnt <= 3'b0;


// byte valid, active for 1 clk every time a full byte has been received from the master
always @(posedge rst_int or posedge SPI_SCLK)
	if (rst_int)
		rcv_byte_valid <= 1'b0;
	else
		if (rcv_byte_valid)				// added to guarantee that rcv_byte_valid is only active for 1 clock
			rcv_byte_valid <= 1'b0;
		else
			if (!SPI_SS && (bit_cnt == 3'b111))
				rcv_byte_valid <= 1'b1;
			else
				rcv_byte_valid <= 1'b0;


// flags for keeping track of the address byte and 1st data byte
always @(posedge rst_int or posedge SPI_SCLK)
	if (rst_int) begin
		addr_has_been_latched		<= 1'b0;	// flag that gets set after the addr has been received (and latched)
		first_data_has_been_latched	<= 1'b0;	// flag that gets set after the 1st data byte has been received (and latched)
	end
	else begin

//		if (SPI_SS)		// if SPI interface is idle
//			addr_has_been_latched <= 1'b0;
//		else
// the above is not necessary since the async rst includes SPI_SS
			if (rcv_byte_valid)
				addr_has_been_latched <= 1'b1;		// set flag after first byte (the address) is received, keep at 1 until transaction is over
			else
				addr_has_been_latched <= addr_has_been_latched;

//		if (SPI_SS)		// if SPI interface is idle
//			first_data_has_been_latched <= 1'b0;
//		else
// the above is not necessary since the async rst includes SPI_SS
			if (addr_has_been_latched && rcv_byte_valid)
				first_data_has_been_latched <= 1'b1;
			else
				first_data_has_been_latched <= first_data_has_been_latched;

	end


// address register, direction control flag
always @(posedge rst or posedge SPI_SCLK)		// don't use rst_int so these signals will remain active even after SPI_SS has gone inactive
	if (rst) begin
		addr_reg		<= 7'b0;
		write_readn		<= 1'b0;	// flag that signifies a write vs. read transaction
	end
	else begin

		if (!addr_has_been_latched && rcv_byte_valid)
			write_readn <= shift_in[7];				// the direction (r/w) flag is in the MSb of the address byte.
		else
			write_readn <= write_readn;

		if (!addr_has_been_latched)
			if (rcv_byte_valid)
				addr_reg <= shift_in[6:0];				// latch the new address, located in the lowest 7 bits of the address byte.
			else
				addr_reg <= addr_reg;
		else // addr_has_been_latched

//			if (((write_readn == WR) && wr_data_valid_reg) || ((write_readn == RD) && rcv_byte_valid))
//				addr_reg <= addr_reg + 1;
//			else
//				addr_reg <= addr_reg;

			// during writes, make addr_reg	wrap back to MemDataByte0 after MemDataByte4
			if ((write_readn == WR) && wr_data_valid_reg)
				if (addr_reg == `MemDataByte4)
					addr_reg <= `MemDataByte0;
				else
					addr_reg <= addr_reg + 1;
			else
				// during reads, do not increment addr_reg when accessing CM_FIFO_Data
				//if ((write_readn == RD) && rcv_byte_valid)
				if ((write_readn == RD) && rd_data_ack_reg)
					if (addr_reg == `CM_FIFO_Data)
						addr_reg <= addr_reg;
					else
						addr_reg <= addr_reg + 1;
				else
					addr_reg <= addr_reg;

	end


// write_pending flag, so writes eventually get sent to the internal interface when more SPI_SCLK edges occur
always @(posedge rst or posedge SPI_SCLK)		// don't use rst_int since this may need to stay active after SPI_SS goes inactive
	if (rst)
		write_pending <= 1'b0;
	else
		if (wr_data_valid_reg)
			write_pending <= 1'b0;
		else
			if ((write_readn == WR) && !SPI_SS && addr_has_been_latched && (bit_cnt == 3'b111))		// can't use rcv_byte_valid since there may not be extra clocks after this byte is being written
				write_pending <= 1'b1;
			else
				write_pending <= write_pending;


// write data valid signal
always @(posedge rst or posedge SPI_SCLK)		// don't use rst_int since this may need to be set after SPI_SS goes inactive
	if (rst)
		wr_data_valid_reg <= 1'b0;
	else
		if (wr_data_valid_reg)
			wr_data_valid_reg <= 1'b0;
		else
			if (write_pending)
				wr_data_valid_reg <= 1'b1;
			else
				wr_data_valid_reg <= wr_data_valid_reg;


always @(posedge rst or posedge SPI_SCLK)		// don't use rst_int since this needs to stay valid after SPI_SS goes inactive
	if (rst)
		write_data_reg <= 8'b0;
	else
		if (!SPI_SS && (bit_cnt == 3'b111))
			write_data_reg <= {shift_in[6:0], SPI_MOSI};
		else
			write_data_reg <= write_data_reg;


// output shift register
always @(posedge rst_int or negedge SPI_SCLK)
	if (rst_int) begin
		bit_cnt_neg <= 3'b0;
		shift_out <= 8'b0;
	end
	else begin
		if (!SPI_SS) begin
			bit_cnt_neg <= bit_cnt_neg + 1;

			if (addr_has_been_latched && (bit_cnt_neg == 7))
				shift_out <= rd_data;
			else
				shift_out <= {shift_out[6:0], 1'b0};
		end
		else begin
			bit_cnt_neg <= 3'b0;
			shift_out <= shift_out;
		end
	end


// read data acknowledge. this is required to pop data from the CM FIFO
always @(posedge rst_int or posedge SPI_SCLK)
	if (rst_int)
		rd_data_ack_reg <= 1'b0;
	else
		//if ( addr_has_been_latched && (write_readn == RD) && (bit_cnt == 3'b111) )
		if ( addr_has_been_latched && (write_readn == RD) && rcv_byte_valid)
			rd_data_ack_reg <= 1'b1;
		else
			rd_data_ack_reg <= 1'b0;



// assignments to the outputs
//assign SPI_MISO			= SPI_SS ? 1'bz : shift_out[7];
assign SPI_MISO			= shift_out[7];//AP2 doesn't support tristate
assign addr				= addr_reg;
assign wr_data			= write_data_reg;
assign wr_data_valid	= wr_data_valid_reg;
assign rd_data_ack		= rd_data_ack_reg;

endmodule

