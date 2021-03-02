/* -----------------------------------------------------------------------------
 title     : FlexFusionEngine Control
 project   : Jim-Bob Hardware Sensor Hub
 -----------------------------------------------------------------------------
 platform  : Alabama test chip
 standard  : Verilog 2001
 -----------------------------------------------------------------------------
 description: Module for controlling the FlexFusionEngine
 -----------------------------------------------------------------------------
 copyright (c) 2013, QuickLogic Corporation
 -----------------------------------------------------------------------------
 History  :
 Date         Version   Author       Description
 2013/03/21   1.0       Jason Lew    Created
 2013/05/02   1.1       Jason Lew    Migrated from FFEAT v21d
 2013/06/14   1.2       Randy O.     Corrected assignment to DataMemReadAddr's b/c it wasn't using IndexReg properly.
                                     Corrected assignment to MemReadData's b/c it shouldn't use IndexReg.
 2013/06/26   1.3       Randy O.     Made the Signals bus 64 bits wide instead of 32, since it needs to be at least as wide as the one in microopdecodes.v
 2013/07/01   1.4       Randy O.     Cosmetic changes to improve readability.
                                     Removed DataMem1WriteData_int & DataMem2WriteData_int since they were unused.
 2013/07/08   1.5       Randy O.     Added unit delays to aid in functional sim.
 2014/05/21   1.6       Glen  G.     Added ability to read/write expanded Sensor Manager Memory

 -----------------------------------------------------------------------------
 Comments: This solution is specifically for implementing into the Alabama
           test chip. Verification will be done using the Jim-Bob Sensor Board
------------------------------------------------------------------------------*/

`include "ulpsh_rtl_defines.v"


`timescale 1ns / 10ps

module FFE_Control ( // named RunFlexFusionEngine in C source
	input				ClockIn,
	input				Clock_x2In,
	input 				ResetIn,
	input 				StartIn,
	output				StartSMOut,
	input		[15:0]	TimeStampIn,
	input		[31:0]	MailboxIn,
	input		[3:0]	SM_InterruptIn,
	output		[11:0]	ControlMemAddressOut,
	output	reg			ControlMemReadEnableOut,
	output		[9:0]	SensorMemReadAddressOut,	// Expanded for Rel 0 on 6/18
	output				SensorMemReadEnableOut,
	output		[9:0]	SensorMemWriteAddressOut,	// New for Rel 0 on 6/18
	output				SensorMemWriteEnableOut,	// New for Rel 0 on 6/18
	input		[35:0]	ControlMemDataIn,
	input		[35:0]	Mem1ReadData,
	input		[35:0]	Mem2ReadData,
	input		[17:0]	SensorMemReadDataIn,
	input				SensorMemBusyIn,

	output		[8:0]	SensorMemWriteDataOut,		// New for Rel 0 on 6/18
	output 				BusyOut,
	output 				DataMem1ReadEnable,
	output 				DataMem2ReadEnable,
	output 				DataMem1WriteEnable,
	output 				DataMem2WriteEnable,
	output 		[9:0] 	DataMem1ReadAddressOut,
	output 		[9:0] 	DataMem1WriteAddressOut,
	output 		[35:0] 	DataMem1WriteDataOut,
	output 		[9:0] 	DataMem2ReadAddressOut,
	output		[9:0] 	DataMem2WriteAddressOut,
	output 		[35:0] 	DataMem2WriteDataOut,

	output	reg			FFE_clock_halfperiod,

	input				MultClockIn,
	input		[3:0]	MultStateIn,
		
	// Status data
	input				SMBusyIn,
	output reg			SMOverrunOut,
	
	// CM FIFO controls
	output		[17:0]	CMWriteDataOut,
	output				CMWriteEnableOut,
	
	output		[7:0]	InterruptMsgOut,

	// interface to ASSP multiplier
	output		[31:0]	mult_in1,
	output		[31:0]	mult_in2,
	output				mult_enable,
	input		[63:0]	mult_out,

	
	output				TP1,
	output				TP2,
	output				TP3
);


// compiler directives related to CM size:
// ENABLE_FFE_F0_CM_SIZE_2K
// ENABLE_FFE_F0_CM_SIZE_4K
// ENABLE_FFE_F0_CM_SIZE_3K	(future support if needed)


`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	// 4k CM
	reg		[11:0]	xPC;
	wire 	[11:0] 	xJumpAddress;
	reg		[11:0]	PC_BG;
`else
	// 2k CM (3k CM support may be added in the future)
	reg		[10:0]	xPC;
	wire 	[10:0] 	xJumpAddress;
	reg		[10:0]	PC_BG;
`endif


reg 			BusyOut_reg, BusyOut_r1;

reg 			Start_r1, Start_r2, Start_r3;
wire	[31:0] 	Mem1ReadDataToALU;
wire 	[31:0] 	Mem2ReadDataToALU;
wire 	[8:0] 	MicroOpCode;
wire	[63:0] 	Signals;		// the width of Signals is defined to be way larger than it needs to be (today), extra bits should get optimized out.
wire	[8:0] 	xIndexRegister;
wire 	[31:0] 	xWriteData;
wire 			xJumpFlag;
wire 	[35:0] 	Mem1ReadDataX;
wire 	[35:0] 	Mem2ReadDataX;

reg		[7:0]	InterruptMsg_reg;
reg				StartSM_reg;

reg		[15:0]	TimeStamp_r1, TimeStamp_r2, TimeStamp_synced;


reg				f5_BG;
reg				f2_BG;
reg				BGcontinue_pending;
reg				BGsave_pending;
reg				BGstop_pending;
reg				BG_active;
reg				Start_pending;
wire			Start_detected;
wire			Save_BG_registers;
wire			Restore_BG_registers;
wire			Clear_PC;
wire			Disable_DataMem_WrEn;

reg			[2:0]	Thread_switch_cnt;
parameter	[2:0]	THREAD_SWITCH_CNT_DONE = 3'b111;


// standard-depth DM addresses (9 bits)
wire 		[8:0] 	DataMem1ReadAddressOut_std;
wire 		[8:0] 	DataMem1WriteAddressOut_std;
wire 		[8:0] 	DataMem2ReadAddressOut_std;
wire		[8:0] 	DataMem2WriteAddressOut_std;

wire		[9:0]	DataMem1WriteAddressOut_trans;
wire		[9:0]	DataMem2WriteAddressOut_trans;
wire		[9:0]	DataMem1ReadAddressOut_trans;
wire		[9:0]	DataMem2ReadAddressOut_trans;
reg			[9:0]	DataMem2ReadAddressOut_trans_hold;
reg					DataMem1ReadAddressOut_trans_MSB_r1;

wire		[9:0]	DataMem1WriteAddressOut_mux;
wire		[9:0]	DataMem1ReadAddressOut_mux;

wire				DataMem1ReadEnable_mux;
wire				DataMem1WriteEnable_mux;

wire		[9:0]	DataMem1WriteAddressOut_split;
wire		[9:0]	DataMem2WriteAddressOut_split;
wire		[9:0]	DataMem1ReadAddressOut_split;
wire		[9:0]	DataMem2ReadAddressOut_split;

wire				DataMem1ReadEnable_split;
wire				DataMem1WriteEnable_split;
wire				DataMem2ReadEnable_split;
wire				DataMem2WriteEnable_split;

//reg				FFE_clock_halfperiod;

wire				DataMem1ReadEnable_std;
wire				DataMem2ReadEnable_std;
reg					DataMem2ReadEnable_std_hold;
wire				DataMem1WriteEnable_std;
wire				DataMem2WriteEnable_std;

reg			[31:0]	Mem1ReadData_latched;
reg			[31:0]	Mem2ReadData_latched;

wire				ClockIn_dly1;
wire				ClockIn_dly2;
wire				ClockIn_dly3;

`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	reg				SensorMemReadEnable_reg;
	reg				SensorMemWriteEnable_reg;
	reg	[9:0]		SensorMemReadAddress_reg;
	reg	[9:0]		SensorMemWriteAddress_reg;
	reg				CMWriteEnable_reg;
`endif

// compiler options, from rtl_defines.v
// ENABLE_FFE_F0_EXTENDED_DM
// ENABLE_FFE_F0_SINGLE_DM
// ENABLE_FFE_F0_PROGRAMMABLE_SEG0_OFFSET
// FFE_F0_SEG0_OFFSET  [value]

`ifdef FFE_F0_SEG0_OFFSET
	parameter	[8:0]	Segment0_offset = `FFE_F0_SEG0_OFFSET;
`endif

`ifdef ENABLE_FFE_F0_EXTENDED_DM
	reg			[9:0]	CurrentSegment_offset;
	reg					DataMem2ReadAddress_MSB_r1;
`endif

`ifdef ENABLE_FFE_F0_SINGLE_DM
	reg			[31:16]	WriteData_latched;
	reg			sig37_latched;
`endif


// sync the timestamp to this clock domain
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	always @(posedge Clock_x2In) begin
`else
	always @(posedge ClockIn) begin
`endif
		TimeStamp_r1 <= TimeStampIn;
		TimeStamp_r2 <= TimeStamp_r1;
		if (TimeStamp_r1 == TimeStamp_r2)
			TimeStamp_synced <= TimeStamp_r2;
		else
			TimeStamp_synced <= TimeStamp_synced;
	end


FFE_ALU u_FFE_ALU (
	.xReadData1				( Mem1ReadDataToALU[31:0]	),
	.xReadData2				( Mem2ReadDataToALU[31:0]	),
	.signals				( Signals					),
	.ClockIn				( ClockIn					),
	.Clock_x2In				( Clock_x2In				),
	.FFE_clock_halfperiod	( FFE_clock_halfperiod		),
	.MultClockIn			( MultClockIn				),
	.MultStateIn			( MultStateIn				),
	.TimeStampIn			( TimeStamp_synced			),
	.MailboxIn				( MailboxIn					),
	.SM_InterruptIn			( SM_InterruptIn			),
	.xIndexRegister			( xIndexRegister			),
	.xWriteData				( xWriteData				),
	.xJumpFlag				( xJumpFlag					),
	.Save_BG_registers		( Save_BG_registers			),
	.Restore_BG_registers	( Restore_BG_registers		),

	.mult_in1				( mult_in1					),
	.mult_in2				( mult_in2					),
	.mult_enable			( mult_enable				),
	.mult_out				( mult_out					)
 );

decodeMicroOpCode U_decodeMicroOpCode (
	.MicroOpCode		( MicroOpCode				),
	.Signals			( Signals					)
);

// Fetch Micro OpCode from Control Memory
//  then needs to be decoded for the various control signals to the ALU (these are called 'signals')
assign MicroOpCode = BusyOut_reg ? ControlMemDataIn[8:0] : 9'b0;   // xMicroOpCode (hold at zero if FFE is not running because of single port ASSP RAMs


`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	// 4k CM
	assign xJumpAddress = ControlMemDataIn[20:9];
`else
	// 2k CM
	assign xJumpAddress = ControlMemDataIn[19:9];
`endif


// standard (legacy) control/address signals for the DM's

assign DataMem1ReadEnable_std = Signals[20];
assign DataMem2ReadEnable_std = Signals[21];
assign DataMem1WriteEnable_std = Disable_DataMem_WrEn ? 1'b0 : (Signals[22] && BusyOut_reg);
assign DataMem2WriteEnable_std = Disable_DataMem_WrEn ? 1'b0 : (Signals[23] && BusyOut_reg);

assign DataMem1WriteAddressOut_std = Signals[34] ? (ControlMemDataIn[17:9] + xIndexRegister) : ControlMemDataIn[17:9];
assign DataMem2WriteAddressOut_std = Signals[34] ? (ControlMemDataIn[17:9] + xIndexRegister) : ControlMemDataIn[17:9];
assign DataMem1ReadAddressOut_std = Signals[28] ? (ControlMemDataIn[26:18] + xIndexRegister) : ControlMemDataIn[26:18];
assign DataMem2ReadAddressOut_std = Signals[29] ? (ControlMemDataIn[35:27] + xIndexRegister) : ControlMemDataIn[35:27];


// translate DM read/write addresses if an extended-length DM is specified
`ifdef ENABLE_FFE_F0_EXTENDED_DM
	// extended-length DM

	`ifdef ENABLE_FFE_F0_CM_SIZE_4K
		always @(posedge Clock_x2In or posedge ResetIn)
			if (ResetIn)
				CurrentSegment_offset <= 0;
			else
				if (Signals[44] && FFE_clock_halfperiod)	// seg_offset being written
					CurrentSegment_offset <= ControlMemDataIn[32:23];
	`else
		always @(posedge ClockIn or posedge ResetIn)
			if (ResetIn)
				CurrentSegment_offset <= 0;
			else
				if (Signals[44])	// seg_offset being written
					CurrentSegment_offset <= ControlMemDataIn[32:23];
	`endif

	// translate addresses to handle extended data memory(ies)
	assign DataMem1WriteAddressOut_trans = (DataMem1WriteAddressOut_std < Segment0_offset) ?
												{1'b0, DataMem1WriteAddressOut_std[8:0]} :
												({1'b0, DataMem1WriteAddressOut_std} + {1'b0, CurrentSegment_offset});
	assign DataMem2WriteAddressOut_trans = (DataMem2WriteAddressOut_std < Segment0_offset) ?
												{1'b0, DataMem2WriteAddressOut_std[8:0]} :
												({1'b0, DataMem2WriteAddressOut_std} + {1'b0, CurrentSegment_offset});
	assign DataMem1ReadAddressOut_trans = (DataMem1ReadAddressOut_std < Segment0_offset) ?
												{1'b0, DataMem1ReadAddressOut_std[8:0]} :
												({1'b0, DataMem1ReadAddressOut_std} + {1'b0, CurrentSegment_offset});
	assign DataMem2ReadAddressOut_trans = (DataMem2ReadAddressOut_std < Segment0_offset) ?
												{1'b0, DataMem2ReadAddressOut_std[8:0]} :
												({1'b0, DataMem2ReadAddressOut_std} + {1'b0, CurrentSegment_offset});

`else
	// standard-length DM (could be single or double)

	assign DataMem1WriteAddressOut_trans = {1'b0, DataMem1WriteAddressOut_std};
	assign DataMem2WriteAddressOut_trans = {1'b0, DataMem2WriteAddressOut_std};
	assign DataMem1ReadAddressOut_trans = {1'b0, DataMem1ReadAddressOut_std};
	assign DataMem2ReadAddressOut_trans = {1'b0, DataMem2ReadAddressOut_std};

`endif


// mux the DM1/DM2 addresses into a single logical DM1 address if a single-DM design is specified
`ifdef ENABLE_FFE_F0_SINGLE_DM
	// single DM

	// keep track of the half-period (0 or 1) within a single FFE clock, by using the 2x FFE clock.
	// FFE_clock_halfperiod should be 0 when the 1x clock is low, 1 when the 1x clock is high (it's registered here to help eliminate timing issues).
	buff buff_clockin_dly1 (.A(ClockIn), .Q(ClockIn_dly1));
	buff buff_clockin_dly2 (.A(ClockIn_dly1), .Q(ClockIn_dly2));
	buff buff_clockin_dly3 (.A(ClockIn_dly2), .Q(ClockIn_dly3));
	//pragma attribute buff_clockin_dly1 dont_touch true
	//pragma attribute buff_clockin_dly2 dont_touch true
	//pragma attribute buff_clockin_dly3 dont_touch true
	assign #3 ClockIn_dly4 = ClockIn_dly3;
	always @(posedge Clock_x2In or posedge ResetIn)
		if (ResetIn)
			FFE_clock_halfperiod <= 0;
		else
			FFE_clock_halfperiod <= #1 ClockIn_dly4;
			/*
			if (BusyOut_reg)
				FFE_clock_halfperiod <= !FFE_clock_halfperiod;
			else
				FFE_clock_halfperiod <= 0;
			*/

	always @(posedge Clock_x2In or posedge ResetIn)
		if (ResetIn) begin
			DataMem2ReadAddressOut_trans_hold <= 0;
			DataMem2ReadEnable_std_hold <= 0;
		end
		else begin
			if (!FFE_clock_halfperiod) begin
				DataMem2ReadAddressOut_trans_hold <= DataMem2ReadAddressOut_trans;
				DataMem2ReadEnable_std_hold <= DataMem2ReadEnable_std;
			end
		end

	// on half-period 0, drive the DM1 read address and read enable
	// on half-period 1, drive the DM2 read address and read enable
	// drive the write address on both half-periods, and the write enable on half-period 0
	assign DataMem1ReadAddressOut_mux = FFE_clock_halfperiod ? DataMem2ReadAddressOut_trans_hold : DataMem1ReadAddressOut_trans;
	assign DataMem1ReadEnable_mux = FFE_clock_halfperiod  ? DataMem2ReadEnable_std_hold : DataMem1ReadEnable_std;
	assign DataMem1WriteAddressOut_mux = DataMem1WriteAddressOut_trans;	// note: DM1 write address = DM2 write address
	assign DataMem1WriteEnable_mux = FFE_clock_halfperiod ? 0 : DataMem1WriteEnable_std;

`else
	// double DM

	// FFE_clock_halfperiod should never be used in this case. Assign it to 0.
	always @(*)
		FFE_clock_halfperiod <= 0;
`endif


// split the muxed RAM control signals across both physical memories if an extended-depth DM is specified.
// (if an extended-depth DM is specified, it must also be a single DM)
`ifdef ENABLE_FFE_F0_EXTENDED_DM
	// extended-length DM
	// note that the DM2 "_mux" signals are not defined, since it's assumed that an extended-length DM is also a single-DM

	assign DataMem1ReadAddressOut_split = DataMem1ReadAddressOut_mux;
	assign DataMem1ReadEnable_split = (DataMem1ReadAddressOut_mux[9] == 1'b0) ? DataMem1ReadEnable_mux : 1'b0;
	assign DataMem1WriteAddressOut_split = DataMem1WriteAddressOut_mux;
	// assign DataMem1WriteEnable_split = (DataMem1WriteAddressOut_mux[9] == 1'b0) ? DataMem1WriteEnable_split : 1'b0; // original
	assign DataMem1WriteEnable_split = (DataMem1WriteAddressOut_mux[9] == 1'b0) ? DataMem1WriteEnable_mux : 1'b0;  // Anthony Le 11-01-2014

	assign DataMem2ReadAddressOut_split = DataMem1ReadAddressOut_mux;
	assign DataMem2ReadEnable_split = (DataMem1ReadAddressOut_mux[9] == 1'b1) ? DataMem1ReadEnable_mux : 1'b0;
	assign DataMem2WriteAddressOut_split = DataMem1WriteAddressOut_mux;
	// assign DataMem2WriteEnable_split = (DataMem1WriteAddressOut_mux[9] == 1'b1) ? DataMem1WriteEnable_split : 1'b0; // original
	assign DataMem2WriteEnable_split = (DataMem1WriteAddressOut_mux[9] == 1'b1) ? DataMem1WriteEnable_mux : 1'b0; // Anthony Le 11-01-2014
`endif


// drive the outputs for the DM control/address
`ifdef ENABLE_FFE_F0_EXTENDED_DM
	// extended-length DM (must be single DM as well)

	// must use the translated then muxed then split signals...

	assign DataMem1ReadEnable = DataMem1ReadEnable_split;
	assign DataMem1WriteEnable = DataMem1WriteEnable_split;
	assign DataMem1WriteAddressOut = DataMem1WriteAddressOut_split;
	assign DataMem1ReadAddressOut = DataMem1ReadAddressOut_split;

	assign DataMem2ReadEnable = DataMem2ReadEnable_split;
	assign DataMem2WriteEnable = DataMem2WriteEnable_split;
	assign DataMem2WriteAddressOut = DataMem2WriteAddressOut_split;
	assign DataMem2ReadAddressOut = DataMem2ReadAddressOut_split;

`else
	// standard-length DM

	`ifdef ENABLE_FFE_F0_SINGLE_DM
		// standard-length single DM

		// must use the non-translated then muxed signals
		// note that physical DM2 is unused, so those outputs are grounded.

		assign DataMem1ReadEnable = DataMem1ReadEnable_mux;
		assign DataMem1WriteEnable = DataMem1WriteEnable_mux;
		assign DataMem1WriteAddressOut = DataMem1WriteAddressOut_mux;
		assign DataMem1ReadAddressOut = DataMem1ReadAddressOut_mux;

		assign DataMem2ReadEnable = 0;
		assign DataMem2WriteEnable = 0;
		assign DataMem2WriteAddressOut = 0;
		assign DataMem2ReadAddressOut = 0;

	`else
		// standard-length double DM (legacy)

		// use the standard signals

		assign DataMem1WriteAddressOut = {1'b0, DataMem1WriteAddressOut_std};
		assign DataMem2WriteAddressOut = {1'b0, DataMem2WriteAddressOut_std};
		assign DataMem1ReadAddressOut = {1'b0, DataMem1ReadAddressOut_std};
		assign DataMem2ReadAddressOut = {1'b0, DataMem2ReadAddressOut_std};

		assign DataMem1ReadEnable = DataMem1ReadEnable_std;
		assign DataMem2ReadEnable = DataMem2ReadEnable_std;
		assign DataMem1WriteEnable = DataMem1WriteEnable_std;
		assign DataMem2WriteEnable = DataMem2WriteEnable_std;

	`endif

`endif


`ifdef ENABLE_FFE_F0_SINGLE_DM
	// single DM, extended or standard length

	// hold the write data so it can be written to the CM FIFO or SM Mem correctly, when a single DM is used
	always @(posedge Clock_x2In or posedge ResetIn)
		if (ResetIn) begin
			WriteData_latched <= 0;
			sig37_latched <= 0;
		end
		else begin
			if (!FFE_clock_halfperiod) begin
				sig37_latched <= Signals[37];
				//if (!FFE_clock_halfperiod && (CMWriteEnableOut || SensorMemWriteEnableOut || Signals[36]))
				if (Signals[30] || Signals[40] || Signals[36])	// CM write or SM write or Interrupt msg write
					WriteData_latched <= xWriteData[31:16];
			end
		end

	//assign CMWriteDataOut = {1'b0, WriteData_latched[31:24], Signals[37], WriteData_latched[23:16]};
	assign CMWriteDataOut = {1'b0, WriteData_latched[31:24], sig37_latched, WriteData_latched[23:16]};
	assign SensorMemWriteDataOut = WriteData_latched[24:16];

`else
	// double DM (legacy)
	
	assign CMWriteDataOut = {1'b0, xWriteData[31:24], Signals[37], xWriteData[23:16]};
	assign SensorMemWriteDataOut = xWriteData[24:16];
`endif



`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	// 4k CM

	// this is done to make sure that all of these signals are stable when the 1x clock occurs.
	always @(posedge Clock_x2In or posedge ResetIn)
		if (ResetIn) begin
			SensorMemReadEnable_reg <= 0;
			SensorMemWriteEnable_reg <= 0;
			SensorMemReadAddress_reg <= 0;
			SensorMemWriteAddress_reg <= 0;
			CMWriteEnable_reg <= 0;
		end
		else begin
			if (!FFE_clock_halfperiod ) begin
				SensorMemReadEnable_reg <= DataMem1ReadEnable_std;
				SensorMemWriteEnable_reg <= Disable_DataMem_WrEn ? 1'b0 : (Signals[40] && BusyOut_reg);
				SensorMemReadAddress_reg <= Signals[28] ? (ControlMemDataIn[27:18] + xIndexRegister) : ControlMemDataIn[27:18];
				SensorMemWriteAddress_reg <= Signals[34] ? (ControlMemDataIn[18:9] + xIndexRegister) : ControlMemDataIn[18:9];
				CMWriteEnable_reg <= Disable_DataMem_WrEn ? 1'b0 : (Signals[30] && BusyOut_reg);	// Write enable to CM FIFO
			end
		end

	assign SensorMemReadEnableOut = SensorMemReadEnable_reg;
	assign SensorMemWriteEnableOut = SensorMemWriteEnable_reg;
	assign SensorMemReadAddressOut = SensorMemReadAddress_reg;
	assign SensorMemWriteAddressOut = SensorMemWriteAddress_reg;
	assign CMWriteEnableOut = CMWriteEnable_reg;

`else
	// 2k CM

	assign SensorMemReadEnableOut = DataMem1ReadEnable_std;
	assign SensorMemWriteEnableOut = Disable_DataMem_WrEn ? 1'b0 : (Signals[40] && BusyOut_reg);
	assign SensorMemReadAddressOut = (Signals[28] ? (ControlMemDataIn[27:18] + xIndexRegister) : ControlMemDataIn[27:18]);
	assign SensorMemWriteAddressOut = Signals[34] ? (ControlMemDataIn[18:9] + xIndexRegister) : ControlMemDataIn[18:9];

	assign CMWriteEnableOut = Disable_DataMem_WrEn ? 1'b0 : (Signals[30] && BusyOut_reg);	// Write enable to CM FIFO
`endif



`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	// 4k CM
	assign ControlMemAddressOut = xPC;
`else
	// 2k CM
	assign ControlMemAddressOut = {1'b0, xPC};
`endif

assign DataMem1WriteDataOut = {4'b0000,xWriteData};
assign DataMem2WriteDataOut = {4'b0000,xWriteData};



// latch the read data from the DM(s)
`ifdef ENABLE_FFE_F0_SINGLE_DM
	`ifdef ENABLE_FFE_F0_EXTENDED_DM
		// extended-length single-DM

		always @(posedge Clock_x2In or posedge ResetIn)
			if (ResetIn)
				DataMem1ReadAddressOut_trans_MSB_r1 <= 0;
			else
				if (!FFE_clock_halfperiod)
					DataMem1ReadAddressOut_trans_MSB_r1 <= DataMem1ReadAddressOut_trans[9];


		always @(posedge Clock_x2In or posedge ResetIn)
			if (ResetIn)
				Mem1ReadData_latched <= 0;
			else
				if (FFE_clock_halfperiod)
					Mem1ReadData_latched <= DataMem1ReadAddressOut_trans_MSB_r1 ? Mem2ReadData : Mem1ReadData;	// read from the correct physical DM

		// store the (logical) DM2 read address' MSB, so it can be used on the following clock to select the correct physical DM
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
		always @(posedge Clock_x2In or posedge ResetIn)
			if (ResetIn)
				DataMem2ReadAddress_MSB_r1 <= 0;
			else
				if (FFE_clock_halfperiod)
					DataMem2ReadAddress_MSB_r1 <= DataMem2ReadAddressOut_trans[9];
`else
		always @(posedge ClockIn or posedge ResetIn)
			if (ResetIn)
				DataMem2ReadAddress_MSB_r1 <= 0;
			else
				DataMem2ReadAddress_MSB_r1 <= DataMem2ReadAddressOut_trans[9];
`endif




		always @(*)
			Mem2ReadData_latched <= DataMem2ReadAddress_MSB_r1 ? Mem2ReadData : Mem1ReadData;			// read from the correct physical DM
		// note that Mem2ReadData_latched will only be valid on the first half-period
		// note that ReadData2 can be registered as well, to make FFE clock-to-clock timing easier (at the cost of more FFs).

	`else
		// standard-length single-DM, latch & hold at the appropriate half-periods
		always @(posedge Clock_x2In or posedge ResetIn)
			if (ResetIn)
				Mem1ReadData_latched <= 0;
			else
				if (FFE_clock_halfperiod)
					Mem1ReadData_latched <= Mem1ReadData;
				else
					Mem1ReadData_latched <= Mem1ReadData_latched;

		always @(*)
			Mem2ReadData_latched <= Mem1ReadData;
		// note that ReadData2 can be registered as well, to make FFE clock-to-clock timing easier (at the cost of more FFs).
	`endif

`else
	// standard-length double-DM, pass-thru
	always @(*) begin
		Mem1ReadData_latched <= Mem1ReadData;
		Mem2ReadData_latched <= Mem2ReadData;
	end
`endif


assign Mem1ReadDataX = Mem1ReadData_latched[31:0];
//a mux that switches between data read from FFEDM2 or SMSM
assign Mem2ReadDataX = Signals[27] ? {SensorMemReadDataIn[16:9], SensorMemReadDataIn[7:0], 16'b0} : Mem2ReadData_latched[31:0];


assign Mem1ReadDataToALU = Mem1ReadDataX;
assign Mem2ReadDataToALU = Mem2ReadDataX;




// toggle StartSM each time Signals[31] is active
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	always @(posedge Clock_x2In or posedge ResetIn)
		if (ResetIn)
			StartSM_reg <= 0;
		else
			if (FFE_clock_halfperiod && Signals[31])
				StartSM_reg <= !StartSM_reg;
			else
				StartSM_reg <= StartSM_reg;
				
`else
	always @(posedge ClockIn or posedge ResetIn)
		if (ResetIn)
			StartSM_reg <= 0;
		else
			if (Signals[31])
				StartSM_reg <= !StartSM_reg;
			else
				StartSM_reg <= StartSM_reg;
				
`endif

assign StartSMOut = StartSM_reg;


// de-glitch the interrupt msg signal since it comes out of the decoder and data mem
`ifdef ENABLE_FFE_F0_SINGLE_DM
	// single DM

	`ifdef ENABLE_FFE_F0_CM_SIZE_4K
		// 4k CM
		always @(posedge Clock_x2In)
			InterruptMsg_reg <= (FFE_clock_halfperiod && Signals[36]) ? WriteData_latched[23:16] : 8'b0;
	`else
		// 2k CM
		always @(posedge ClockIn)
			InterruptMsg_reg <= Signals[36] ? WriteData_latched[23:16] : 8'b0;
	`endif

`else
	// double DM, legacy behavior
	always @(posedge ClockIn)
		InterruptMsg_reg <= Signals[36] ? xWriteData[23:16] : 8'b0;
`endif

assign InterruptMsgOut = InterruptMsg_reg;


// sync to local clock
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	always @(posedge Clock_x2In) begin
`else
	always @(posedge ClockIn) begin
`endif
		Start_r1 <= StartIn;
		Start_r2 <= Start_r1;
		Start_r3 <= Start_r2;
	end

assign Start_detected = (Start_r1 != Start_r2) || (Start_r2 != Start_r3);


// Program Counter to step through the Control Memory
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	always @(posedge Clock_x2In or posedge ResetIn) begin
`else
	always @(posedge ClockIn or posedge ResetIn) begin
`endif

		if (ResetIn) begin
			xPC <= 0;
			BusyOut_reg <= 1'b0;
			BusyOut_r1 <= 1'b0;
			ControlMemReadEnableOut <= 1'b0;
			SMOverrunOut <= 1'b0;
		end else
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
			if (FFE_clock_halfperiod)
`endif
			begin
				BusyOut_r1 <= BusyOut_reg;
				if (BusyOut_reg && BusyOut_r1) begin		// make sure the 1st control word appears on the RAM outputs... requires one clock cycle after the read enable is turned on.
					if (!Signals[32]) begin					// !Signals[32] = continue running = !STOP
						if (Restore_BG_registers)
							xPC <= PC_BG;
						else
							if (Clear_PC)
								xPC <= 0;
							else
								// hold the PC while switching threads
								if (BG_active && 
										((Start_detected || BGsave_pending) ||
										(BGcontinue_pending && (Thread_switch_cnt != THREAD_SWITCH_CNT_DONE))))
									xPC <= xPC;
								else
									if (xJumpFlag)
										xPC <= xJumpAddress;
									else
										xPC <= xPC + 1;
					end else begin	// Signals[32] = STOP
						xPC <= 0;
						if (!BG_active) begin					// FG mode
							BusyOut_reg <= 1'b0;
							ControlMemReadEnableOut <= 1'b0;
						end else begin							// BG mode
							ControlMemReadEnableOut <= 1'b1;
							if (BGstop_pending && (Thread_switch_cnt == THREAD_SWITCH_CNT_DONE) && !Start_pending && !Start_detected)
								BusyOut_reg <= 1'b0;
							else
								BusyOut_reg <= 1'b1;
						end
					end
				end else		// new start condition not detected, keep running
					if (Start_detected) begin
						if (SMBusyIn) begin
							SMOverrunOut <= 1'b1;
						end else begin
							BusyOut_reg <= 1'b1;
							ControlMemReadEnableOut <= 1'b1;
						end
					end

			end
	end

assign BusyOut = BusyOut_reg;



// --- BG thread support

// Signals[42] = SetBGflag (instruction modifier)
// Signals[43] = BGcontinue (instruction)


// Start_pending, flag to latch the start event, in case it happens right as we're switching to the BG thread or while running the BG thread.
//		This needs to be latched so the FG thread can be started immediately once we've switched out of the BG thread.
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	always @(posedge Clock_x2In or posedge ResetIn)
`else
	always @(posedge ClockIn or posedge ResetIn)
`endif
		if (ResetIn)
			Start_pending <= 0;
		else
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
			if (FFE_clock_halfperiod)
`endif
				case (Start_pending)
					1'b0:	if (Start_detected &&			// start detected AND...
									(Signals[42] ||			//	...SetBGflag active (about to start or continue BG)...OR...
									BG_active))				//	...BG active (switching to BG, running BG, about to stop/end BG, stopping BG)
								Start_pending <= 1;
					1'b1:	if (!BG_active)					// clear this flag when BG_active goes away
								Start_pending <= 0;
				endcase


// BG_pending counter, used instead of individual state machines for each type of context switch
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	always @(posedge Clock_x2In or posedge ResetIn)
`else
	always @(posedge ClockIn or posedge ResetIn)
`endif
		if (ResetIn)
			Thread_switch_cnt <= 0;
		else
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
			if (FFE_clock_halfperiod)
`endif
				if (BGsave_pending || BGcontinue_pending || BGstop_pending)
					if (Thread_switch_cnt == THREAD_SWITCH_CNT_DONE)
						Thread_switch_cnt <= 0;
					else
						Thread_switch_cnt <= Thread_switch_cnt + 1;
				else
					Thread_switch_cnt <= 0;


// BGcontinue_pending, flag that goes active while resuming the BG thread (BGcontinue instruction)
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	always @(posedge Clock_x2In or posedge ResetIn)
`else
	always @(posedge ClockIn or posedge ResetIn)
`endif
		if (ResetIn)
			BGcontinue_pending <= 0;
		else
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
			if (FFE_clock_halfperiod)
`endif
				case (BGcontinue_pending)
					1'b0:	if (Signals[43])
								BGcontinue_pending <= 1;
					1'b1:	if (Thread_switch_cnt == THREAD_SWITCH_CNT_DONE)
								BGcontinue_pending <= 0;
				endcase


// BGsave_pending, flag that goes active while saving the state of the BG thread (BG_continue or BG thread interrupted by the sample timer)
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	always @(posedge Clock_x2In or posedge ResetIn)
`else
	always @(posedge ClockIn or posedge ResetIn)
`endif
		if (ResetIn)
			BGsave_pending <= 0;
		else
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
			if (FFE_clock_halfperiod)
`endif
				case (BGsave_pending)
					1'b0:	if (BG_active &&																					// in the BG thread...AND...
									(Start_detected ||																			// 		...started detected...OR...
									(BGcontinue_pending && (Thread_switch_cnt == THREAD_SWITCH_CNT_DONE) && Start_pending)))	// ...about to complete BGcontinue AND start was detected
								BGsave_pending <= 1;
					1'b1:	if (Thread_switch_cnt == THREAD_SWITCH_CNT_DONE)
								BGsave_pending <= 0;
				endcase


// BGstop_pending, flag that goes active while stopping the BG thread (normal completion)
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	always @(posedge Clock_x2In or posedge ResetIn)
`else
	always @(posedge ClockIn or posedge ResetIn)
`endif
		if (ResetIn)
			BGstop_pending <= 0;
		else
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
			if (FFE_clock_halfperiod)
`endif
				case (BGstop_pending)
					1'b0:	if (BG_active && Signals[32])
								BGstop_pending <= 1;
					1'b1:	if (Thread_switch_cnt == THREAD_SWITCH_CNT_DONE)
								BGstop_pending <= 0;
				endcase



// BG_active, flag that is active while switching to, in, or switching from, the BG thread
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	always @(posedge Clock_x2In or posedge ResetIn)
`else
	always @(posedge ClockIn or posedge ResetIn)
`endif
		if (ResetIn)
			BG_active <= 0;
		else
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
			if (FFE_clock_halfperiod)
`endif
				case (BG_active)
					1'b0:	if (Signals[42])									// SetBGactive (entering BG mode, either via start BG or continue BG)
								BG_active <= 1;
					1'b1:	if ((BGsave_pending || BGstop_pending) && (Thread_switch_cnt == THREAD_SWITCH_CNT_DONE))	// done switching out of BG mode
								BG_active <= 0;
				endcase


// Control signal used to save the BG copy of the PC and ALU flags
assign Save_BG_registers = (BGsave_pending && (Thread_switch_cnt == 1));
	// clock-by-clock sequence of events:
	// {BGsave_pending,Thread_switch_cnt}
	//  0,0 - Start detected, last BG instruction, hold PC
	//  1,0 - hold PC, disable DataMemWrEn
	//  1,1 - hold PC, save PC & flags (Save_BG_registers active), disable DataMemWrEn
	//  1,2 - clear PC, disable DataMemWrEn
	//  1,3 - hold PC (driving into CodeMem), disable DataMemWrEn
	//  1,4 - hold PC (CodeMem available, driving DataMem), disable DataMemWrEn
	//  1,5 - hold PC (DataMem available), disable DataMemWrEn
	//  1,6 - hold PC (extraneous), disable DataMemWrEn
	//  1,7 - hold PC (extraneous), disable DataMemWrEn
	//  0,0 - continue running normally (now in FG thread)

// Control signal used to restore the BG state of the PC and ALU flags
assign Restore_BG_registers = (BGcontinue_pending && (Thread_switch_cnt == 1));
	// clock-by-clock sequence of events:
	// {BGcontinue_pending,Thread_switch_cnt} - action(s)
	//  0,0 - BGcontinue();
	//  1,0 - NOP;, disable DataMemWrEn
	//  1,1 - load PC & flags (Restore_BG_registers active), disable DataMemWrEn
	//  1,2 - hold PC (driving into CodeMem), disable DataMemWrEn
	//  1,3 - hold PC (CodeMem available, driving DataMem), disable DataMemWrEn
	//  1,4 - hold PC (DataMem available), disable DataMemWrEn
	//  1,5 - hold PC (extraneous), disable DataMemWrEn
	//  1,6 - hold PC (extraneous), disable DataMemWrEn
	//  1,7 - increment PC, disable DataMemWrEn
	//  0,0 - continue running normally (now in BG thread)


// Control signal used to reset the PC (during BGstop or BGsave)
assign Clear_PC = ((BGsave_pending && (Thread_switch_cnt == 2)) || (BGstop_pending));

// Control signal used to disable the FFE DataMem write enable, while resuming the BG thread
assign Disable_DataMem_WrEn = (BGcontinue_pending);


// BG copy of the PC
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	always @(posedge Clock_x2In or posedge ResetIn)
`else
	always @(posedge ClockIn or posedge ResetIn)
`endif
	if (ResetIn)
		PC_BG <= 0;
	else
`ifdef ENABLE_FFE_F0_CM_SIZE_4K
			if (FFE_clock_halfperiod)
`endif
			if (Save_BG_registers)
				PC_BG <= xPC;
			else
				PC_BG <= PC_BG;



// test points
assign TP1 = 0;
assign TP2 = 0;
assign TP3 = 0;


endmodule




