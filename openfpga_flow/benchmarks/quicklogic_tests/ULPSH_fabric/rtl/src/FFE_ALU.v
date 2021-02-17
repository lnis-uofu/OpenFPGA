// -----------------------------------------------------------------------------
// title          : FFE_ALU.v
// project        : ULP Sensor Hub
// description    : 32-bit ALU
// -----------------------------------------------------------------------------
// copyright (c) 2013, QuickLogic Corporation
// -----------------------------------------------------------------------------
// History        :
// date            version    author         description
//
// -----------------------------------------------------------------------------
// date            version    author         description
// 10-31-2013	   1.1        Anthony Le	 Add AND function to code
//	Details:
//		1. Add new bus signal xT30 as 32-bit for output of AND function
//		2. Assign a temporary 32bit bus to hold the concatenation of MailBox bit:
//     			Mailbox32bit = {8'b0, MailBox[7:0], 16'h0};
//	    3. Add code to perform AND function between Mailbox32bit and xT23
//		4. Add mux function between xT30 and MailBox in using signal[38]
//		5. Expected the Synthesis to remove all unused logics
//
// 11-29-2013	   1.2        Anthony Le	 Add additional comparison functions
//	Details:
//		1. Current system supports only three jump functions:
//         a. JMP: jump
//         b. JMPNEZ: jump when result is not equal to zero
//         c. JMPGEZ: jump when result is greater or equal to zero
//		2. Add four more compriason jump functions
//         a. JMPEQZ: jump when result is equal to zero
//         b. JMPLTZ: jump when result is less than zero
//         c. JMPLEZ: jump when result is less than or equal to zero
//         d. JMPGTZ: jump when result is greater than zero
//	    3. Re-use the three control signals[19:17]
//      4. Use an 8-to-1 mux for JUMP logic
//      5. Update xJumpFlag from wire to reg type
//
//
// -----------------------------------------------------------------------------

`timescale 1ns / 10ps

`include "ulpsh_rtl_defines.v"

module FFE_ALU 	(	
	input 		[31:0] 	xReadData1,
	input 		[31:0] 	xReadData2,
	input 		[63:0] 	signals,
	input				ClockIn,
	input				Clock_x2In,
	input				FFE_clock_halfperiod,
	input		[15:0]	TimeStampIn,
	input		[31:0]	MailboxIn,
	input		[3:0]	SM_InterruptIn,
	input				MultClockIn,
	input		[3:0]	MultStateIn,
	output 		[8:0] 	xIndexRegister,
	output 		[31:0] 	xWriteData,
	output 	reg			xJumpFlag,
	input 				Save_BG_registers,
	input 				Restore_BG_registers,

	output		[31:0]	mult_in1,
	output		[31:0]	mult_in2,
	output				mult_enable,
	input		[63:0]	mult_out
);
 

wire		[31:0]	xT0, xT1, xT2;
//wire		[31:0]	xT3;		// defined below, depending on the multiplier implementation
wire		[31:0]	xT4;
reg			[31:0]	xT5;
wire		[31:0]	xT6, xT7, xT8, xT9;
wire		[31:0]	xT10, xT11, xT12, xT13, xT14, xT15, xT16, xT17, xT18, xT19, xT20, xT21, xT22;
reg			[31:0]	xT23;
wire		[31:0]	xT24, xT25;
wire		[8:0]	xT26, xT27, xT28;
reg			[8:0]	xT29;
wire		[31:0]	xT30;
wire signed	[31:0]	xT12_signed;
wire				f0;
reg					f2, f5, f2_BG, f5_BG;
wire				f3, f6;
reg					f2_latched, f2_BG_latched;
reg			[31:0]	xT5_latched;
reg					f5_latched, f5_BG_latched;
reg			[8:0]	xT29_latched;

// compiler options, from rtl_defines.v
// ENABLE_FFE_F0_EXTENDED_DM
// ENABLE_FFE_F0_SINGLE_DM
// ENABLE_FFE_F0_PROGRAMMABLE_SEG0_OFFSET
// FFE_F0_SEG0_OFFSET  [value]

// compiler directives related to CM size:
// ENABLE_FFE_F0_CM_SIZE_2K
// ENABLE_FFE_F0_CM_SIZE_4K
// ENABLE_FFE_F0_CM_SIZE_3K	(future support if needed)



// select the multiplier implementation
`define ASSP_MULT

`ifdef FullSingleCycle
	wire	[63:0]	xT3;
	wire	[63:0]	xT1extended, xT2extended;

	assign xT1extended[63:0]	= { {32{xT1[31]}}, xT1[31:0] };
	assign xT2extended[63:0]	= { {32{xT2[31]}}, xT2[31:0] };
	assign xT3					= xT1extended * xT2extended;
	assign xT4					= (xT3 >> 16);

`elsif Handicapped
	wire	[31:0]	xT3;
	assign xT3	= xT1 + xT2;	// replace multiplier with adder
	assign xT4	= xT3;

`elsif MultiCycle
	wire	[31:0]	xT3;
	Multiplier Multiplier_1 (
		.ClockIn	( MultClockIn	),
		.StateIn	( MultStateIn	),
		.Arg1In		( xT1			),
		.Arg2In		( xT2			),
		.ResultOut	( xT3			)
	);
	assign xT4	= xT3;

`elsif ASSP_MULT
	wire	[63:0]	xT3;

	assign xT3 = mult_out;
	assign xT4 = (xT3 >> 16);
				
`else
	wire	[31:0]	xT3;

	assign xT3 = 0;
	assign xT4 = 0;

`endif


// drive the outputs for the ASSP multiplier
assign mult_in1 = xT1;
assign mult_in2 = xT2;
assign mult_enable = signals[5];



assign xT22			= signals[2] ? (xReadData1<<16) : xReadData1;

always @(*)
	case (signals[1:0])
		2'b00:	xT23 <= xReadData2;
		2'b01:	xT23 <= MailboxIn;
		2'b10:	xT23 <= {TimeStampIn[15:0], 16'b0};
		2'b11:	xT23 <= {7'b0, xT29[8:0], 16'b0};	// IndexReg
	endcase

assign xT24			= signals[3] ? {12'b0, SM_InterruptIn[3:0], 16'b0} : xT23;
assign xT30			= xReadData1 & xT24;
assign xT9			= signals[38] ? xT30 : xT24;
assign xT21			= xReadData1 | xT24;
assign xT25			= signals[39] ? xT21 : xT9;

assign xT0			= signals[4] ? xT25 : xT22;

assign xT12_signed	= xT12;
// remove these muxes to save space
//assign xT1	= signals[5] ? xT0 : xT1;		// this mux (latch) keeps the multiplier inputs stable, to save power
//assign xT2	= signals[5] ? xT25 : xT2;		// this mux (latch) keeps the multiplier inputs stable, to save power
assign xT1		= xT0;
assign xT2		= xT25;

assign xT7		= signals[5] ? xT4 : xT25;
assign xT10		= signals[8] ? ~xT7 : xT7;
assign xT11		= signals[8] ? 32'b1 : 32'b0;
assign xT13		= signals[9] ? xT7 : xT10;
assign xT6		= signals[6] ? xT22 : xT5;
assign xT8		= signals[7] ? xT6 : 32'b0;
assign xT12		= xT8 + xT10 + xT11;
assign xT14		= (xT12_signed >>> 1);
assign xT16		= signals[11] ? xT14 : xT12;
assign xT19		= signals[12] ? xT16 : xT6;
assign f0		= xT12[31];

// Sign bit
`ifndef ENABLE_FFE_F0_SINGLE_DM
	// double DM, default behavior
	
	`ifdef ENABLE_FFE_F0_CM_SIZE_4K
		// 4k CM
		always @(posedge Clock_x2In) begin
			if (FFE_clock_halfperiod) begin
				if (Restore_BG_registers)
					f2 <= f2_BG;
				else
					f2 <= signals[10] ? f0 : f2;

				if (Save_BG_registers)
					f2_BG <= f2;
				else
					f2_BG <= f2_BG;
			end
		end

	`else
		// 2k CM
		always @(posedge ClockIn) begin
			if (Restore_BG_registers)
				f2 <= f2_BG;
			else
				f2 <= signals[10] ? f0 : f2;

			if (Save_BG_registers)
				f2_BG <= f2;
			else
				f2_BG <= f2_BG;
		end

	`endif

`else
	// single DM

	always @(posedge Clock_x2In) begin
		if (!FFE_clock_halfperiod) begin
			if (Restore_BG_registers)
				f2_latched <= f2_BG;
			else
				f2_latched <= signals[10] ? f0 : f2;

			if (Save_BG_registers)
				f2_BG_latched <= f2;
			else
				f2_BG_latched <= f2_BG;
		end
	end

	`ifdef ENABLE_FFE_F0_CM_SIZE_4K
		always @(posedge Clock_x2In) begin
			if (FFE_clock_halfperiod) begin
				f2 <= f2_latched;
				f2_BG <= f2_BG_latched;
			end
		end
	`else
		always @(posedge ClockIn) begin
			f2 <= f2_latched;
			f2_BG <= f2_BG_latched;
		end
	`endif

`endif


assign f6		= signals[33] ? (f2) : (f0);
assign f3		= signals[35] ? !f5 : f6;

assign xT15		= (f3) ? xT13 : xT19;
assign xT17		= signals[13] ? xT15 : xT16;
assign xT18		= (xT17 << 1);
assign xT20		= signals[14] ? xT18 : xT17;

assign xWriteData	= xT20;

// accumulator
`ifndef ENABLE_FFE_F0_SINGLE_DM
	// double DM, default behavior
	`ifdef ENABLE_FFE_F0_CM_SIZE_4K
		always @(posedge Clock_x2In) begin
			if (FFE_clock_halfperiod)
				xT5 <= signals[16] ? xT20 : xT5;
		end
	`else
		always @(posedge ClockIn) begin
			xT5 <= signals[16] ? xT20 : xT5;
		end
	`endif
`else
	// single DM
	always @(posedge Clock_x2In) begin
		if (!FFE_clock_halfperiod)
			xT5_latched <= signals[16] ? xT20 : xT5;
	end

	`ifdef ENABLE_FFE_F0_CM_SIZE_4K
		always @(posedge Clock_x2In)
			if (FFE_clock_halfperiod)
				xT5 <= xT5_latched;
	`else
		always @(posedge ClockIn)
			xT5 <= xT5_latched;
	`endif
`endif


// NEZ flag
`ifndef ENABLE_FFE_F0_SINGLE_DM
	// double DM, default behavior
	`ifdef ENABLE_FFE_F0_CM_SIZE_4K
		always @(posedge Clock_x2In) begin
			if (FFE_clock_halfperiod) begin
				if (Restore_BG_registers)
					f5 <= f5_BG;
				else
					f5 <= signals[15] ? f5 : (xT20 != 32'b0);

				if (Save_BG_registers)
					f5_BG <= f5;
				else
					f5_BG <= f5_BG;
			end
		end
	`else
		always @(posedge ClockIn) begin
			if (Restore_BG_registers)
				f5 <= f5_BG;
			else
				f5 <= signals[15] ? f5 : (xT20 != 32'b0);

			if (Save_BG_registers)
				f5_BG <= f5;
			else
				f5_BG <= f5_BG;
		end
	`endif
`else
	// single DM
	always @(posedge Clock_x2In) begin
		if (!FFE_clock_halfperiod) begin
			if (Restore_BG_registers)
				f5_latched <= f5_BG;
				//f5_latched <= f5_BG_latched;
			else
				f5_latched <= signals[15] ? f5 : (xT20 != 32'b0);
				//f5_latched <= signals[15] ? f5_latched : (xT20 != 32'b0);

			if (Save_BG_registers)
				f5_BG_latched <= f5;
				//f5_BG_latched <= f5_latched;
			else
				f5_BG_latched <= f5_BG;
				//f5_BG_latched <= f5_BG_latched;
		end
	end

	`ifdef ENABLE_FFE_F0_CM_SIZE_4K
		always @(posedge Clock_x2In) begin
			if (FFE_clock_halfperiod) begin
				f5 <= f5_latched;
				f5_BG <= f5_BG_latched;
			end
		end
	`else
		always @(posedge ClockIn) begin
			f5 <= f5_latched;
			f5_BG <= f5_BG_latched;
		end
	`endif
`endif


always @(*)
begin
	case ({signals[19], signals[18], signals[17]})
		3'b000: xJumpFlag = 1'b0;			// no jump
		3'b001: xJumpFlag = 1'b1;			// JMP			(unconditional jump)
		3'b010: xJumpFlag = f5;				// JMPNEZ		(jump if NEZflag)
		3'b011: xJumpFlag = !f5;			// JMPEQZ		(jump if !NEZflag)
		3'b100: xJumpFlag = !f2;			// JMPGEZ		(jump if !SignBit)
		3'b101: xJumpFlag = f2;				// JMPLTZ		(jump if SignBit)
		3'b110: xJumpFlag = !(!f2 && f5);	// JMPLEZ		(jump if SignBit or !NEZflag)
		3'b111: xJumpFlag = !f2 && f5;		// JMPGTZ		(jump if !SignBit and NEZflag)
		default: xJumpFlag = 1'b0;
	endcase
end


// Index register code
assign xT26	= xT29 + 1;
assign xT27	= signals[24] ? xT26 : 9'b0;
assign xT28	= signals[26] ? xT20[24:16] : xT27;	// assign the integer portion of xT20, or xT27

// Index Register
`ifndef ENABLE_FFE_F0_SINGLE_DM
	// double DM, default behavior

	`ifdef ENABLE_FFE_F0_CM_SIZE_4K
		always @(posedge Clock_x2In) begin
			if (FFE_clock_halfperiod) begin
				if (signals[25])
					xT29 <= xT28;
				else
					xT29 <= xT29;
				end
		end
	`else
		always @(posedge ClockIn) begin
			if (signals[25])
				xT29 <= xT28;
			else
				xT29 <= xT29;
		end
	`endif
`else
	// single DM

	always @(posedge Clock_x2In) begin
		if (!FFE_clock_halfperiod) begin
			if (signals[25])
				xT29_latched <= xT28;
			else
				xT29_latched <= xT29;
		end
	end

	`ifdef ENABLE_FFE_F0_CM_SIZE_4K
		always @(posedge Clock_x2In)
			if (FFE_clock_halfperiod)
				xT29 <= xT29_latched;
	`else
		always @(posedge ClockIn)
			xT29 <= xT29_latched;
	`endif

`endif

assign xIndexRegister = xT29;


// prevent logic duplication
//pragma attribute xT0 preserve_driver true
//pragma attribute xT1 preserve_driver true
//pragma attribute xT2 preserve_driver true
//pragma attribute xT3 preserve_driver true
//pragma attribute xT4 preserve_driver true
//pragma attribute xT5 preserve_driver true
//pragma attribute xT6 preserve_driver true
//pragma attribute xT7 preserve_driver true
//pragma attribute xT8 preserve_driver true
//pragma attribute xT9 preserve_driver true
//pragma attribute xT10 preserve_driver true
//pragma attribute xT11 preserve_driver true
//pragma attribute xT12 preserve_driver true
//pragma attribute xT13 preserve_driver true
//pragma attribute xT14 preserve_driver true
//pragma attribute xT15 preserve_driver true
//pragma attribute xT16 preserve_driver true
//pragma attribute xT17 preserve_driver true
//pragma attribute xT18 preserve_driver true
//pragma attribute xT19 preserve_driver true
//pragma attribute xT20 preserve_driver true
//pragma attribute xT20 preserve_driver true
//pragma attribute xT21 preserve_driver true
//pragma attribute xT22 preserve_driver true
//pragma attribute xT23 preserve_driver true
//pragma attribute xT24 preserve_driver true
//pragma attribute xT25 preserve_driver true
//pragma attribute xT26 preserve_driver true
//pragma attribute xT27 preserve_driver true
//pragma attribute xT28 preserve_driver true
//pragma attribute xT29 preserve_driver true
//pragma attribute xT30 preserve_driver true
//pragma attribute xT12_signed preserve_driver true
//pragma attribute f0 preserve_driver true
//pragma attribute f2 preserve_driver true
//pragma attribute f5 preserve_driver true
//pragma attribute f3 preserve_driver true
//pragma attribute f6 preserve_driver true


endmodule

