
/*------------------------------------------------------------------------------------------
Title:			TLC.v
Description:

--------------------------------------------------------------------------------------------
Revision History:

--------------------------------------------------------------------------------------------
To Do:


------------------------------------------------------------------------------------------*/

`timescale 1ns / 10ps

`include "SensorHubDefines.v"
`include "ulpsh_rtl_defines.v"

module TLC (
	// General interface
	input				SPI_SCLK,
	input				FFE_Clock,
	input				Clock32KIn,
	input				ResetIn,
	output				SW_Reset,

	output	reg			RingOsc_cal_en,
	output	reg	[2:0]	RingOsc_select,
	input		[15:0]	RingOsc_cal_value,

	input				I2C_Master_Error,
	input				FFEBusyIn,
	input				SMBusyIn,
	input				SMOverrunIn,
	output				StartFFEOut,
//	output 				InitSMOut,              // Removed for Rel 0 on 6/18
	output				StartSMOut,
	output reg	[15:0]	TimeStampOut,
	output reg	      	TimeStampOut_Tog,
	output				UseFastClockOut,

	input		[7:0]	InterruptMsgFromFFEIn,
	output				InterruptPinOut,
	input				SensorInterruptIn,

	output		[31:0]	FFE_Mailbox_Out,

	input		[9:0]	CtrlRunTimeAddressReg,  // Expanded for Rel 0 on 6/18
	output reg	[9:0]	CtrlRunTimeAddressOut,  // Expanded for Rel 0 on 6/18
	output reg			CtrlRunTimeAddressSM,

	// Interface to SPI Slave
	input 		[6:0]	RegAddrIn,
	input 		[7:0]	RegDataIn,
	output reg	[7:0] 	RegDataOut,
	input 				RegWriteEnableIn,
	input 				RegReadDataAckIn,

	// Interface to memories
	output				TLCDrivingFFEControlMem,
	output				TLCDrivingFFEDataMem1,
	output				TLCDrivingFFEDataMem2,
	output				TLCDrivingSMMem,
	output				TLCDrivingCMMem,
	output				MemorySelect_en,
	output		[2:0]	MemorySelect,
	output		[11:0]	MemoryAddressOut,
	output		[35:0]	MemoryDataOut,
	input 		[35:0]	MemoryDataIn,
	output reg			MemoryReadEnableOut,
	output reg			MemoryWriteEnableOut,
	output				MemoryClockOut,

	// Interface to Communication Manager FIFO
	output	reg			CM_FIFO_ReadEnable,
	input		[8:0]	CM_FIFO_ReadData,
	input		[3:0]	CM_FIFO_PopFlags,
	input		[3:0]	CM_FIFO_PushFlags,
	input				CM_FIFO_Overflow,
	output				CM_RingBufferMode,
	input				CM_AutoDrain_Busy,
	
	// test points
	output				TP1,
	output				TP2,
	
	// LEDs ON/OFF Control
	output reg	[2:0]	leds_off_o,
	
	// FFE CLock ENable
	output	reg			FFE_CLK_Enable_o,
	output	reg			ClockEnable_o,
	output	reg			clock_32KHz_Enable_o,
	output	reg	[2:0]	FFE_Clock_Control_o,
	output	reg	[2:0]	SM_Clock_Control_o,
	output	reg			ClkSourceSelect_o
);

	parameter		CYCLES_PER_MSEC = 33;		// number of 32.768KHz clock cycles per millisecond

	reg		[7:0]	CommandReg;
	reg		[7:0]	msecs_per_sample_reg;
	reg		[7:0]	MemSelect_reg;
	reg		[7:0]	MemAddrLow;
	reg		[7:0]	MemAddrHigh;
	reg		[7:0]	MemDataByte0;
	reg		[7:0]	MemDataByte1;
	reg		[7:0]	MemDataByte2;
	reg		[7:0]	MemDataByte3;
	reg		[3:0]	MemDataByte4;
	reg		[7:0]	CM_Control_reg;
	reg				WaitForMemRead;
	reg				WaitForMemWrite;
	reg				IncrementMemAddr;
	reg				StartFFE_32K, StartFFE_Clkin;
//	reg				InitSM_Clkin;              // Removed for Rel 0 on 6/18
	reg				StartSM_Clkin;

	reg		[7:0]	clock32K_count;		// this needs to be wide enough to accomodate the CYCLES_PER_MSEC constant
	reg		[7:0]	count_msecs;
	wire			pulse_1ms;
	wire			pulse_sample_period;
	reg				pulse_sample_period_reg;
	wire			FFE_Holdoff;		// Used to ensure a full count on the First FFE run
	reg				FFE_Holdoff_reset;
	reg				FFE_Holdoff_preset;

	wire			RunFFEContinuously;
	//wire			RunFFEOnce;
	//wire			RunSMOnce;
	//wire			RunSensorInit;

	wire			CM_FIFO_Overflow_reg;
	reg		[3:0]	CM_FIFO_PopFlags_r1;
	reg		[3:0]	CM_FIFO_PopFlags_r2;
	reg		[3:0]	CM_FIFO_PopFlags_sync;
	
	wire			I2C_Master_Error_reg;
	
	reg		[7:0]	InterruptCtrl_reg;
	reg		[7:0] 	InterruptFFEMsg_clear;
	wire			Interrupt_En_0;
	wire	[7:0]	InterruptFFEMsg_latched;
	wire			InterruptFFEMsg_ORed;

	reg				SW_Reset_Start;
	reg				SW_Reset_r1;
	reg				SW_Reset_r2;

	reg				RunFFEContinuously_r1;
	reg				RunFFEContinuously_r2;

	reg				FFEOverrun;
	reg		[31:0]	FFE_Mailbox_reg;
	
	wire			i_StartFFEOut;
	reg				s1_StartFFEOut, s2_StartFFEOut;
	reg				s3_FFEBusyIn, s2_FFEBusyIn, s1_FFEBusyIn;
	reg				d_FFE_CLK_Enable;
	reg				s1_SMBusyIn, s2_SMBusyIn, s3_SMBusyIn;
	wire			d_ClockEnable;
	wire 			smInit_enable;
//	reg				s1_InitSM_Clkin, s2_InitSM_Clkin, s3_InitSM_Clkin, s4_InitSM_Clkin, s5_InitSM_Clkin, s6_InitSM_Clkin;   // Removed for Rel 0 on 6/18
	reg				s1_StartSM_Clkin, s2_StartSM_Clkin, s3_StartSM_Clkin, s4_StartSM_Clkin, s5_StartSM_Clkin, s6_StartSM_Clkin;
	wire	[2:0]	FFE_SET, SM_SET;
	wire			clkSourceSelect;
	reg				SleepMode, IntInputLevel;
	reg				sensorInterrupt_s1, sensorInterrupt_s2, sensorInterrupt_s3;
	wire			sleepModeSet, sleepReset;



	assign FFE_Mailbox_Out			= {FFE_Mailbox_reg[15:0], FFE_Mailbox_reg[31:16]};
	assign MemoryClockOut			= SPI_SCLK;

	assign RunFFEContinuously		= CommandReg[0];
	//assign RunFFEOnce				= CommandReg[1];
	//assign RunSMOnce				= CommandReg[2];
	//assign RunSensorInit			= CommandReg[3];
	//assign SW_Reset				= CommandReg[6];
	assign UseFastClockOut			= CommandReg[7];

	assign Interrupt_En_0			= InterruptCtrl_reg[0];
	assign Interrupt_En_1			= InterruptCtrl_reg[1];

	assign TLCDrivingFFEControlMem	= MemSelect_reg[7] ? (MemSelect_reg[2:0] == 3'b000) : 1'b0;
	assign TLCDrivingFFEDataMem1	= MemSelect_reg[7] ? (MemSelect_reg[2:0] == 3'b001) : 1'b0;
	assign TLCDrivingFFEDataMem2	= MemSelect_reg[7] ? (MemSelect_reg[2:0] == 3'b010) : 1'b0;
	assign TLCDrivingSMMem			= MemSelect_reg[7] ? (MemSelect_reg[2:0] == 3'b011) : 1'b0;
	assign TLCDrivingCMMem			= 1'b0;		// should be removed, since the CMMem is now the CM FIFO
	assign MemorySelect_en			= MemSelect_reg[7];
	assign MemorySelect				= MemSelect_reg[2:0];

	assign MemoryAddressOut			= {MemAddrHigh[3:0],MemAddrLow};
	assign MemoryDataOut			= {MemDataByte4[3:0],MemDataByte3,MemDataByte2,MemDataByte1,MemDataByte0};

	assign CM_RingBufferMode		= CM_Control_reg[0];
	


	// requests to run FFE and Sensor Manager

	assign pulse_1ms			= (clock32K_count == (CYCLES_PER_MSEC - 1));		// 1-clock pulse each time 1ms has elapsed
	assign pulse_sample_period	= (pulse_1ms && (count_msecs == 1));	// 1-clock pulse @ each sample period

	assign StartFFEOut = s2_StartFFEOut;
	// Delay starting FFE
	always @(posedge Clock32KIn or posedge ResetIn)
	begin
		if (ResetIn)
		begin
			s1_StartFFEOut <= 1'b0;
			s2_StartFFEOut <= 1'b0;
		end
		else
		begin
			s1_StartFFEOut <= i_StartFFEOut;
			s2_StartFFEOut <= s1_StartFFEOut;
		end
	end
	
	// Synchronized FFE Busy input
	// & logic to generate FFE_CLK_Enable (active when timer starts and off when busy end)
	always @(posedge Clock32KIn or posedge ResetIn) 
	begin
		if (ResetIn) 
		begin
			s1_FFEBusyIn <= 0;
			s2_FFEBusyIn <= 0;
			s3_FFEBusyIn <= 0;
		end
		else 
		begin
			s1_FFEBusyIn <= FFEBusyIn;
			s2_FFEBusyIn <= s1_FFEBusyIn;
			s3_FFEBusyIn <= s2_FFEBusyIn;
		end	
	end

always @*
begin
	if (!FFE_CLK_Enable_o)
		d_FFE_CLK_Enable = s1_StartFFEOut ^ i_StartFFEOut;
	else
		if (s3_FFEBusyIn && !s2_FFEBusyIn)
			d_FFE_CLK_Enable = 1'b0;
		else
			d_FFE_CLK_Enable = FFE_CLK_Enable_o;
end

always @(posedge Clock32KIn or posedge ResetIn) 
	begin
		if (ResetIn) 
		begin
			FFE_CLK_Enable_o <= 0;
		end
		else 
		begin
			FFE_CLK_Enable_o <= d_FFE_CLK_Enable;
		end
	end	
	

	always @(posedge Clock32KIn or posedge ResetIn) begin
		if (ResetIn) begin
			clock32K_count			<= 0;
			count_msecs				<= 1;		// reset to 1 (sample period = 0 does not make sense, and should be invalid)
			pulse_sample_period_reg	<= 0;
			TimeStampOut			<= 0;
			TimeStampOut_Tog		<= 0;
			StartFFE_32K			<= 0;
			FFEOverrun				<= 0;
		end else begin

			pulse_sample_period_reg	<= pulse_sample_period;		// de-glitch the pulse_sample_period signal, since it's used to asynchronously reset FFE_Holdoff

			if (pulse_1ms) begin
				clock32K_count   <= 0;
				TimeStampOut     <= TimeStampOut + 1;				// the timestamp increments @ 1ms
				TimeStampOut_Tog <= ~TimeStampOut_Tog;				// the timestamp increments @ 1ms
			end else begin
				clock32K_count   <= clock32K_count + 1;
				TimeStampOut     <= TimeStampOut;
				TimeStampOut_Tog <= TimeStampOut_Tog;
			end

			if (pulse_sample_period)							// sample period boundary
				count_msecs <= msecs_per_sample_reg;				// reset the msec counter back to the register value
			else
				if (pulse_1ms)
					count_msecs <= count_msecs - 1;					// decrement by 1 @ the 1ms boundary
				else
					count_msecs <= count_msecs;


			//if ((clock32K_count == (CYCLES_PER_MSEC - 1)) && (count_msecs == 1)) begin		// msec counter about to be reset back to the register value
			if (pulse_sample_period && !SleepMode) begin								// msec counter about to be reset back to the register value
				if (RunFFEContinuously && !FFE_Holdoff) begin			// trigger a run only if FFE_Holdoff has been deactivated
					if (FFEBusyIn) begin
						FFEOverrun <= 1'b1;
					end else begin
						StartFFE_32K <= ~StartFFE_32K;
						//CMBufferBeingWrittenOut <= CMBufferBeingWrittenOut + 1;
						//if (!AlternateI2CIsActiveIn) begin // If Alternate I2C is active, then we are reading the buffer
						//	CMBufferBeingRead <= CMBufferBeingWrittenOut;
						//end
					end
				end
			end

		end
	end



	// software-controlled reset, 1-2 clock pulses wide @ the 32K clock

	// generate a one-clock pulse @ the SPI_SCLK, to be used to preset a flop running in the 32K clock domain
	always @(posedge SPI_SCLK)
		if (RegWriteEnableIn && (RegAddrIn == `CommandReg) && RegDataIn[6])		// SW Reset control bit is being written
			SW_Reset_Start <= 1;
		else
			SW_Reset_Start <= 0;

	// 32K clock domain, the r1 flop gets preset by the SPI clock pulse above, and only gets deactivated after another 32K clock period (using the r2 flop)
	always @(posedge Clock32KIn or posedge SW_Reset_Start)
		if (SW_Reset_Start)
			SW_Reset_r1 <= 1;
		else
			if (SW_Reset_r1 && !SW_Reset_r2)
				SW_Reset_r1 <= 1;
			else
				SW_Reset_r1 <= 0;

	// r2 flop, used to stretch the generated reset pulse
	always @(posedge Clock32KIn)
		SW_Reset_r2 <= SW_Reset_r1;


	assign SW_Reset = SW_Reset_r2;


	// When running the FFE continuously, this logic prevents the FFE from running until the start of a sample period

	always @(posedge SPI_SCLK or posedge ResetIn)
		if (ResetIn)
			FFE_Holdoff_preset <= 0;
		else
			if (RegWriteEnableIn && (RegAddrIn == `CommandReg) && RegDataIn[0])		// Run FFE Continuously control bit is being written...
				FFE_Holdoff_preset <= 1;											// ... assert FFE_Holdoff
			else
				FFE_Holdoff_preset <= 0;

	always @(posedge Clock32KIn or posedge ResetIn)
		if (ResetIn)
			FFE_Holdoff_reset <= 0;
		else
			if (pulse_sample_period_reg && RunFFEContinuously && FFE_Holdoff)		// reset FFE_Holdoff when the first timer expiration occurs, to ensure a full first run
				FFE_Holdoff_reset <= 1;
			else
				FFE_Holdoff_reset <= 0;

	dff_pre_clr dff_pre_clr_FFE_Holdoff ( .CLK(1'b0) , .CLR(FFE_Holdoff_reset), .D(1'b0), .PRE(FFE_Holdoff_preset), .Q(FFE_Holdoff) );


	
	// latch the I2C Master Error signal
	dff_pre_clr dff_pre_clr_I2C_Master_Error ( .CLK(1'b0) , .CLR(ResetIn), .D(1'b0), .PRE(I2C_Master_Error), .Q(I2C_Master_Error_reg) );



	// interrupt logic

	// note: InterruptMsgFromFFEIn should be de-glitched externally (currently de-glitched in FFE_Control.v)

	// logic to clear the FFE msg interrupts
	always @(posedge SPI_SCLK)
		if (RegWriteEnableIn && (RegAddrIn == `InterruptFFEMsg))
			InterruptFFEMsg_clear <= RegDataIn[7:0];
		else
			InterruptFFEMsg_clear <= 8'b0;

	// latch the interrupt msg from the FFE, clear when the InterruptFFEMsg register is being written
	dff_pre_clr dff_pre_clr_InterruptFFEMsg_0 ( .CLK(1'b0) , .CLR(InterruptFFEMsg_clear[0]), .D(1'b0), .PRE(InterruptMsgFromFFEIn[0]), .Q(InterruptFFEMsg_latched[0]) );
	dff_pre_clr dff_pre_clr_InterruptFFEMsg_1 ( .CLK(1'b0) , .CLR(InterruptFFEMsg_clear[1]), .D(1'b0), .PRE(InterruptMsgFromFFEIn[1]), .Q(InterruptFFEMsg_latched[1]) );
	dff_pre_clr dff_pre_clr_InterruptFFEMsg_2 ( .CLK(1'b0) , .CLR(InterruptFFEMsg_clear[2]), .D(1'b0), .PRE(InterruptMsgFromFFEIn[2]), .Q(InterruptFFEMsg_latched[2]) );
	dff_pre_clr dff_pre_clr_InterruptFFEMsg_3 ( .CLK(1'b0) , .CLR(InterruptFFEMsg_clear[3]), .D(1'b0), .PRE(InterruptMsgFromFFEIn[3]), .Q(InterruptFFEMsg_latched[3]) );
	dff_pre_clr dff_pre_clr_InterruptFFEMsg_4 ( .CLK(1'b0) , .CLR(InterruptFFEMsg_clear[4]), .D(1'b0), .PRE(InterruptMsgFromFFEIn[4]), .Q(InterruptFFEMsg_latched[4]) );
	dff_pre_clr dff_pre_clr_InterruptFFEMsg_5 ( .CLK(1'b0) , .CLR(InterruptFFEMsg_clear[5]), .D(1'b0), .PRE(InterruptMsgFromFFEIn[5]), .Q(InterruptFFEMsg_latched[5]) );
	dff_pre_clr dff_pre_clr_InterruptFFEMsg_6 ( .CLK(1'b0) , .CLR(InterruptFFEMsg_clear[6]), .D(1'b0), .PRE(InterruptMsgFromFFEIn[6]), .Q(InterruptFFEMsg_latched[6]) );
	dff_pre_clr dff_pre_clr_InterruptFFEMsg_7 ( .CLK(1'b0) , .CLR(InterruptFFEMsg_clear[7]), .D(1'b0), .PRE(InterruptMsgFromFFEIn[7]), .Q(InterruptFFEMsg_latched[7]) );
	


	assign InterruptFFEMsg_ORed = |InterruptFFEMsg_latched[7:0];

	// drive the interrupt output pin: active-high, level sensitive
	assign InterruptPinOut = (Interrupt_En_0 && InterruptFFEMsg_ORed) ||
	                         (Interrupt_En_1 && (I2C_Master_Error_reg || SMOverrunIn || FFEOverrun));

	
	
	// overflow detection bit
	
	dff_pre_clr dff_pre_clr_overflow ( .CLK(1'b0) , .CLR(ResetIn), .D(1'b0), .PRE(CM_FIFO_Overflow), .Q(CM_FIFO_Overflow_reg) );

	
	// sync the FIFO flags to the SPI clock domain
	always @(posedge SPI_SCLK) begin
		CM_FIFO_PopFlags_r1 <= CM_FIFO_PopFlags;
		CM_FIFO_PopFlags_r2 <= CM_FIFO_PopFlags_r1;
		if (CM_FIFO_PopFlags_r1 == CM_FIFO_PopFlags_r2)
			CM_FIFO_PopFlags_sync <= CM_FIFO_PopFlags_r2;
		else
			CM_FIFO_PopFlags_sync <= CM_FIFO_PopFlags_sync;
	end


	//Registers for controlling the FFE and memories
	always @(posedge SPI_SCLK or posedge ResetIn) begin
		if (ResetIn) begin
			RegDataOut				<= 0;
			MemoryReadEnableOut		<= 0;
			MemoryWriteEnableOut	<= 0;
			WaitForMemRead			<= 0;
			WaitForMemWrite			<= 0;
			IncrementMemAddr		<= 0;
			CommandReg				<= 0;
			msecs_per_sample_reg	<= 1;		// default value is 1 (sample period = 0 should be invalid)
			InterruptCtrl_reg		<= 0;
            MemSelect_reg			<= 0;
			MemAddrLow				<= 0;
			MemAddrHigh				<= 0;
			MemDataByte0			<= 0;
			MemDataByte1			<= 0;
			MemDataByte2			<= 0;
			MemDataByte3			<= 0;
			MemDataByte4			<= 0;
			StartFFE_Clkin			<= 0;
//			InitSM_Clkin			<= 0;       // Removed for Rel 0 on 6/18
			StartSM_Clkin			<= 0;
			CM_FIFO_ReadEnable		<= 0;
			CM_Control_reg			<= 0;
			FFE_Mailbox_reg			<= 0;
	        CtrlRunTimeAddressOut   <= 0;
			CtrlRunTimeAddressSM 	<= 0;
			leds_off_o				<= 3'b111;
			ClkSourceSelect_o		<= 1'b0;
			clock_32KHz_Enable_o	<= 1'b1;
			FFE_Clock_Control_o		<= FFE_SET;
			SM_Clock_Control_o		<= SM_SET;
			RingOsc_cal_en			<= 0;
			RingOsc_select			<= 3'h7;
		end else begin
			if (MemoryWriteEnableOut) begin
				if(WaitForMemWrite == 0) begin
					WaitForMemWrite <= 1;
				end else begin
					MemoryWriteEnableOut <= 0;
					WaitForMemWrite <= 0;
					IncrementMemAddr <= 1;
				end
			end // if (MemoryWriteEnableOut)

			if (IncrementMemAddr) begin
				IncrementMemAddr <= 0;
				{MemAddrHigh[3:0],MemAddrLow} <= {MemAddrHigh[3:0],MemAddrLow} + 1;
			end

			if (MemoryReadEnableOut) begin
				if (WaitForMemRead == 0) begin
					WaitForMemRead <= 1;
				end else begin
					MemoryReadEnableOut <= 0;
					WaitForMemRead <= 0;
					MemDataByte4 <= MemoryDataIn[35:32];
					MemDataByte3 <= MemoryDataIn[31:24];
					MemDataByte2 <= MemoryDataIn[23:16];
					MemDataByte1 <= MemoryDataIn[15:8];
					MemDataByte0 <= MemoryDataIn[7:0];
				end
			end

			// CM FIFO read control
			///// Old Code
			/*
			if (CM_FIFO_ReadEnable)
				CM_FIFO_ReadEnable <= 0;
			else
				if (RegAddrIn == `CM_FIFO_Data && RegReadDataAckIn)
					CM_FIFO_ReadEnable <= 1;
				else
					CM_FIFO_ReadEnable <= 0;
			*/
			//// New Code
			if (RegAddrIn == `CM_FIFO_Data && RegReadDataAckIn)
				CM_FIFO_ReadEnable <= !CM_FIFO_ReadEnable;
			else
				CM_FIFO_ReadEnable <= CM_FIFO_ReadEnable;

			if (RegWriteEnableIn) begin
				case (RegAddrIn)
					`CommandReg:			begin
												CommandReg 				<= RegDataIn;
												//FFE_Holdoff					<= 1;
											end
					`milSecSample:			msecs_per_sample_reg		<= RegDataIn;
					`InterruptCtrl:			InterruptCtrl_reg			<= RegDataIn;
					//`InterruptStat:														// currently writes to this register does nothing, adding more interrupts may change this.
					`MemSelect:				MemSelect_reg				<= RegDataIn;
					`MemAddrLow:			MemAddrLow					<= RegDataIn;
					`MemAddrHigh:			begin
												MemAddrHigh				<= RegDataIn;
												WaitForMemRead			<= 0;				// this is assigned in separate 'if' statements, the logic should be combined into 1.
												MemoryReadEnableOut		<= 1;
											end
					`MemDataByte0:			MemDataByte0				<= RegDataIn;
					`MemDataByte1:			MemDataByte1				<= RegDataIn;
					`MemDataByte2:			MemDataByte2				<= RegDataIn;
					`MemDataByte3:			MemDataByte3				<= RegDataIn;
					`MemDataByte4:			begin
												MemDataByte4			<= RegDataIn[3:0];
												MemoryWriteEnableOut	<= 1;
												WaitForMemWrite			<= 0;				// this is assigned in separate 'if' statements, the logic should be combined into 1.
											end
					`CM_Control:			CM_Control_reg				<= RegDataIn;
					`MailboxToFFE_0:		FFE_Mailbox_reg[7:0]		<= RegDataIn;
					`MailboxToFFE_1:		FFE_Mailbox_reg[15:8]		<= RegDataIn;
					`MailboxToFFE_2:		FFE_Mailbox_reg[23:16]		<= RegDataIn;
					`MailboxToFFE_3:		FFE_Mailbox_reg[31:24]		<= RegDataIn;
					`RunTimeAdrReg: 		begin
	                                            CtrlRunTimeAddressOut[7:0]   <= RegDataIn;      // Expanded for Rel 0 on 6/18
	                                            CtrlRunTimeAddressSM    <= ~CtrlRunTimeAddressSM;
	                                        end
					`DemoLedCtrlReg: 		begin
	                                            leds_off_o				<= RegDataIn[2:0];
	                                        end
					`ClocksControl: 		begin
	                                            FFE_Clock_Control_o		<= RegDataIn[2:0];
												SM_Clock_Control_o		<= RegDataIn[5:3];
												ClkSourceSelect_o		<= RegDataIn[6];
												clock_32KHz_Enable_o	<= RegDataIn[7];
	                                        end
					`RunTimeAdrReg_Upr: 	begin
	                                            CtrlRunTimeAddressOut[9:8]   <= RegDataIn[1:0]; // New for Rel 0 on 6/18
	                                        end
					`SleepControl:			begin
												RingOsc_cal_en			<= RegDataIn[7];
												RingOsc_select			<= RegDataIn[6:4];
											end
				endcase

				if ((RegAddrIn == `CommandReg) && RegDataIn[1])		// run FFE once
					StartFFE_Clkin <= ~StartFFE_Clkin;

				//the SM control signals come as a pair because only one should be toggled at a time if both bits are written to in CommandReg
				//Initialization takes precedense over Start
//				if ((RegAddrIn == `CommandReg) && RegDataIn[3])       // Removed for Rel 0 on 6/18
//					InitSM_Clkin <= ~InitSM_Clkin;                    // Removed for Rel 0 on 6/18
//				else if ((RegAddrIn == `CommandReg) && RegDataIn[2])  // Updated for Rel 0 on 6/18
				if ((RegAddrIn == `CommandReg) && RegDataIn[2])
					StartSM_Clkin <= ~StartSM_Clkin;

			end else begin
				case (RegAddrIn)

					`CommandReg:			RegDataOut <=	{	CommandReg[7],		// UseFastClk
																1'b0,				// SW_Reset, self-clearing
																1'b0,				// reserved
																1'b0,				// reserved
																1'b0,				// RunSensorInit, self-clearing
																1'b0,				// RunSMOnce, self-clearing
																1'b0,				// RunFFEOnce, self-clearing
																CommandReg[0]		// RunFFEContinuously
															};

					`StatusReg:				RegDataOut <=	{	3'b0,
																I2C_Master_Error_reg,
																SMOverrunIn,
																FFEOverrun,
																SMBusyIn,
																FFEBusyIn
															};	// Status Reg

					`milSecSample:			RegDataOut <= msecs_per_sample_reg;
					`InterruptCtrl:			RegDataOut <= {6'b0, Interrupt_En_1, Interrupt_En_0};
					`InterruptStat:			RegDataOut <= {7'b0, InterruptFFEMsg_ORed};
					`MemSelect:				RegDataOut <= {MemSelect_reg[7], 4'b0, MemSelect_reg[2:0]};
					`MemAddrLow:			RegDataOut <= MemAddrLow;
					`MemAddrHigh:			RegDataOut <= MemAddrHigh;
					`MemDataByte0:			RegDataOut <= MemoryDataIn[7:0];	// MemDataByte0;
					`MemDataByte1:			RegDataOut <= MemoryDataIn[15:8];	// MemDataByte1;
					`MemDataByte2:			RegDataOut <= MemoryDataIn[23:16];	// MemDataByte2;
					`MemDataByte3:			RegDataOut <= MemoryDataIn[31:24];	// MemDataByte3;
					`MemDataByte4:			RegDataOut <= MemoryDataIn[35:32];	// MemDataByte4;
					`CM_FIFO_Data:			RegDataOut <= CM_FIFO_ReadData[7:0];
					`CM_Control:			RegDataOut <= {7'b0, CM_RingBufferMode};
					`CM_Status:				RegDataOut <= {6'b0, CM_FIFO_Overflow_reg, CM_AutoDrain_Busy};
					`CM_FIFO_Flags_0:		RegDataOut <= {4'b0, CM_FIFO_PopFlags_sync};
					`MailboxToFFE_0:		RegDataOut <= FFE_Mailbox_reg[7:0];
					`MailboxToFFE_1:		RegDataOut <= FFE_Mailbox_reg[15:8];
					`MailboxToFFE_2:		RegDataOut <= FFE_Mailbox_reg[23:16];
					`MailboxToFFE_3:		RegDataOut <= FFE_Mailbox_reg[31:24];
					`InterruptFFEMsg:		RegDataOut <= InterruptFFEMsg_latched;
					`RunTimeAdrReg: 		RegDataOut <= CtrlRunTimeAddressReg[7:0];         // Expanded for Rel 0 on 6/18
					`DemoLedCtrlReg: 		RegDataOut <= {5'b0, leds_off_o};
					`ClocksControl: 		RegDataOut <= {clock_32KHz_Enable_o, ClkSourceSelect_o, SM_Clock_Control_o[2:0], FFE_Clock_Control_o[2:0]};
					`SleepControl:			RegDataOut <= {RingOsc_cal_en, RingOsc_select[2:0], 1'b0, sleepModeSet, IntInputLevel, SleepMode};
					`RunTimeAdrReg_Upr:		RegDataOut <= {6'h0, CtrlRunTimeAddressReg[9:8]}; // New for Rel 0 on 6/18
					`CalValueLow:			RegDataOut <= RingOsc_cal_value[7:0];
					`CalValueHi:			RegDataOut <= RingOsc_cal_value[15:8];
					default: 				RegDataOut <= 8'h21;
				endcase
			end
		end // if (ResetIn)
	end // Always


assign i_StartFFEOut	= StartFFE_32K ^ StartFFE_Clkin;
//assign InitSMOut	= s6_InitSM_Clkin;  // Removed for Rel 0 on 6/18
assign StartSMOut	= s6_StartSM_Clkin;

// test points
assign TP1 = FFE_Mailbox_reg[0];
assign TP2 = RegWriteEnableIn;

// Logic to drive RIng Osc Clock Enable / Disable

//assign smInit_enable = (s2_InitSM_Clkin ^ s3_InitSM_Clkin) || (s2_StartSM_Clkin ^ s3_StartSM_Clkin);  // Removed for Rel 0 on 6/18
assign smInit_enable = (s2_StartSM_Clkin ^ s3_StartSM_Clkin);                                           // Updated for Rel 0 on 6/18

assign d_ClockEnable = ClockEnable_o ? ((!s2_SMBusyIn && s3_SMBusyIn) ? 1'b0 : 1'b1) : (( d_FFE_CLK_Enable || smInit_enable ) ? 1'b1 : 1'b0);
always @(posedge Clock32KIn or posedge ResetIn) 
	begin
		if (ResetIn) 
		begin
			s1_SMBusyIn <= 0;
			s2_SMBusyIn <= 0;
			s3_SMBusyIn <= 0;
			ClockEnable_o <= 0;
//			s1_InitSM_Clkin <= 0;
//			s2_InitSM_Clkin <= 0;
//			s3_InitSM_Clkin <= 0;
//			s4_InitSM_Clkin <= 0;
//			s5_InitSM_Clkin <= 0;
//			s6_InitSM_Clkin <= 0;
			s1_StartSM_Clkin <= 0;
			s2_StartSM_Clkin <= 0;
			s3_StartSM_Clkin <= 0;
			s4_StartSM_Clkin <= 0;
			s5_StartSM_Clkin <= 0;
			s6_StartSM_Clkin <= 0;
		end
		else 
		begin
			s1_SMBusyIn <= (SMBusyIn || FFEBusyIn);
			s2_SMBusyIn <= s1_SMBusyIn;
			s3_SMBusyIn <= s2_SMBusyIn;
			ClockEnable_o <= d_ClockEnable;
//			s1_InitSM_Clkin <= InitSM_Clkin;
//			s2_InitSM_Clkin <= s1_InitSM_Clkin;
//			s3_InitSM_Clkin <= s2_InitSM_Clkin;
//			s4_InitSM_Clkin <= s3_InitSM_Clkin;
//			s5_InitSM_Clkin <= s4_InitSM_Clkin;
//			s6_InitSM_Clkin <= s5_InitSM_Clkin;
			s1_StartSM_Clkin <= StartSM_Clkin;
			s2_StartSM_Clkin <= s1_StartSM_Clkin;
			s3_StartSM_Clkin <= s2_StartSM_Clkin;
			s4_StartSM_Clkin <= s3_StartSM_Clkin;
			s5_StartSM_Clkin <= s4_StartSM_Clkin;
			s6_StartSM_Clkin <= s5_StartSM_Clkin;
		end
	end	
	
// Logic to select default reset values for the SM and FFE CLK selection
assign	FFE_SET[0] = (`FFE1CLK_FREQ_SLT == 8'b00000001) || (`FFE1CLK_FREQ_SLT == 8'b00000010) || (`FFE1CLK_FREQ_SLT == 8'b00000100) || (`FFE1CLK_FREQ_SLT == 8'b00001000) ||
                     (`FFE1CLK_FREQ_SLT == 8'b00010000) || (`FFE1CLK_FREQ_SLT == 8'b01000000);
assign	FFE_SET[1] = (`FFE1CLK_FREQ_SLT == 8'b00000001) || (`FFE1CLK_FREQ_SLT == 8'b00000010) || (`FFE1CLK_FREQ_SLT == 8'b00000100) || (`FFE1CLK_FREQ_SLT == 8'b00001000) ||
					 (`FFE1CLK_FREQ_SLT == 8'b00100000) || (`FFE1CLK_FREQ_SLT == 8'b01000000);
assign	FFE_SET[2] = (`FFE1CLK_FREQ_SLT == 8'b10000000);

assign	SM_SET[0] = (`SM1CLK_FREQ_SLT == 8'b00000010) || (`SM1CLK_FREQ_SLT == 8'b00001000) || (`SM1CLK_FREQ_SLT == 8'b00100000) || (`SM1CLK_FREQ_SLT == 8'b10000000);
assign	SM_SET[1] = (`SM1CLK_FREQ_SLT == 8'b00000100) || (`SM1CLK_FREQ_SLT == 8'b00001000) || (`SM1CLK_FREQ_SLT == 8'b01000000) || (`SM1CLK_FREQ_SLT == 8'b10000000);
assign	SM_SET[2] = (`SM1CLK_FREQ_SLT == 8'b00010000) || (`SM1CLK_FREQ_SLT == 8'b00100000) || (`SM1CLK_FREQ_SLT == 8'b01000000) || (`SM1CLK_FREQ_SLT == 8'b10000000);

// Logic for Sleep Mode
// Sensor interrupt input is double ring to check for transtion from low to high
// Once detected, the design will enable the sampling period
assign	sleepModeSet = sensorInterrupt_s2 && !sensorInterrupt_s3;
always @(posedge Clock32KIn or posedge ResetIn) 
	begin
		if (ResetIn) 
		begin
			sensorInterrupt_s1 <= 0;
			sensorInterrupt_s2 <= 0;
			sensorInterrupt_s3 <= 0;
		end
		else
		begin
			sensorInterrupt_s1 <= (SensorInterruptIn ~^ IntInputLevel) && SleepMode;
			sensorInterrupt_s2 <= sensorInterrupt_s1;
			sensorInterrupt_s3 <= sensorInterrupt_s2;
		end
	end

assign sleepReset = ResetIn || sleepModeSet;
always @(posedge SPI_SCLK or posedge sleepReset) begin
	if (sleepReset) 
	begin
		SleepMode <= 1'b0;
	end
	else if ((RegAddrIn == `SleepControl) && RegWriteEnableIn)
	begin
	    SleepMode <= RegDataIn[0];
	end
end
	

always @(posedge SPI_SCLK or posedge ResetIn) begin
	if (ResetIn) 
	begin
		IntInputLevel <= 1'b1;
	end
	else if ((RegAddrIn == `SleepControl) && RegWriteEnableIn)
	begin
		IntInputLevel <= RegDataIn[1];
	end
end

endmodule 
