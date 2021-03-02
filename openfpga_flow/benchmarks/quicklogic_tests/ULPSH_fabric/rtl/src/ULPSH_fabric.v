
`timescale 1ns / 10ps

`include "ulpsh_rtl_defines.v"


module ULPSH_fabric (
	input			OperatingClockRef,
	input			Clock32KIn,
	input			ResetInN,
	
	input			SPI_SCLK,
	input			SPI_SS,
	input			SPI_MOSI,
	output			SPI_MISO,
	
	output			Interrupt,
	input	[4:0] 	SensorInterrupt,

	// ASSP ring oscillator interface
	output			ASSP_ringosc_en_o,
	output	[2:0]	ASSP_ringosc_sel_o,
	input			ASSP_ringosc_sysclk_i,			// divided ring osc clock (use this for system clock)
	input			ASSP_ringosc_sysclk_x2_i,		// divided ring osc clock times 2
	input			ASSP_ringosc_clk_i,				// raw ring osc clock div 2

	// FFE Ctrl Mem, RAM0 --> ASSP RAM interface - left bank
	output			FCM0_CLK_o,
	output	[8:0]	FCM0_ADDR_o,
	output	[35:0]	FCM0_WR_DATA_o,
	input	[35:0]	FCM0_RD_DATA_i,
	output			FCM0_WR_EN_o,
	output			FCM0_RD_EN_o,
	output	[3:0]	FCM0_WR_BE_o,

	// FFE Ctrl Mem, RAM1 --> ASSP RAM interface - right bank
	output			FCM1_CLK_o,
	output	[8:0]	FCM1_ADDR_o,
	output	[35:0]	FCM1_WR_DATA_o,
	input	[35:0]	FCM1_RD_DATA_i,
	output			FCM1_WR_EN_o,
	output			FCM1_RD_EN_o,
	output	[3:0]	FCM1_WR_BE_o,

`ifdef ENABLE_FFE_F0_CM_SIZE_4K
	// ASSP RAM interface - 8k - left bank
	output			FCM8K_CLK_o,
    output	[11:0]	FCM8K_ADDR_o,
    output	[16:0]	FCM8K_WR_DATA_o,
    input	[16:0]	FCM8K_RD_DATA_i,
    output			FCM8K_WR_EN_o,
    output			FCM8K_RD_EN_o,
    output	[1:0]	FCM8K_WR_BE_o,
`endif

	// FFE Data Mem1 --> ASSP RAM interface - right bank
	output			DMM1_WR_CLK_o,
	output	[9:0]	DMM1_WR_ADDR_o,
	output	[31:0]	DMM1_WR_DATA_o,
	output			DMM1_WR_EN_o,
	output	[3:0]	DMM1_WR_BE_o,
	output			DMM1_RD_CLK_o,
	output	[9:0]	DMM1_RD_ADDR_o,
	input	[31:0]	DMM1_RD_DATA_i,
	output			DMM1_RD_EN_o,

	// FFE Data Mem2 --> ASSP RAM interface - right bank
	output			DMM2_WR_CLK_o,
	output	[9:0]	DMM2_WR_ADDR_o,
	output	[31:0]	DMM2_WR_DATA_o,
	output			DMM2_WR_EN_o,
	output	[3:0]	DMM2_WR_BE_o,
	output			DMM2_RD_CLK_o,
	output	[9:0]	DMM2_RD_ADDR_o,
	input	[31:0]	DMM2_RD_DATA_i,
	output			DMM2_RD_EN_o,

	// ASSP multiplier interface
	output	[31:0]	Amult_o,
	output	[31:0]	Bmult_o,
	output			Valid_mult_o,
	input	[63:0]	Cmult_i,
	// ASSP RIGHT BANK: I2C Wishbone interface
	output          I2C_wb_clk_o,
	output          I2C_arst_o,
	output  [2:0]   I2C_wb_adr_o,
	output  [7:0]   I2C_wb_dat_o,
	input   [7:0]   I2C_wb_dat_i,
	output          I2C_wb_we_o,
	output          I2C_wb_stb_o,
	output          I2C_wb_cyc_o,
	input           I2C_wb_ack_i,
	input           I2C_tip_i,
	input           I2C_Master_Error_i,

    //inout           SensorScl_io,
    output          SensorScl_io,
    input           SensorScl_oen_i,
	input           SensorScl_i,

    //inout           SensorSda_io,
    output          SensorSda_io,
    input           SensorSda_oen_i,
	input           SensorSda_i,


	output	LED1,

	output	TP1,
	output	TP2,
	output	TP3,
	
	//AP2
	output 	[17:0]	CM_FIFO_1x_din_o			,
	output 			CM_FIFO_1x_push_int_o		,
	output 			CM_FIFO_1x_pop_int_o		,
	output 			CM_FIFO_1x_push_clk_o		,
	output 			CM_FIFO_1x_pop_clk_o		,
	output 			CM_FIFO_1x_rst_o			,
	                                            
	input 			CM_FIFO_1x_almost_full_i	,
	input 			CM_FIFO_1x_almost_empty_i	,
	input 	[3:0]	CM_FIFO_1x_push_flag_i		,
	input 	[3:0]	CM_FIFO_1x_pop_flag_i		,
	input 	[8:0]	CM_FIFO_1x_dout_i			,
	
    output 	[9:0]	SMMemory_WriteAddressIn_TLC_o,
	output 	[8:0]	SMMemory_ReadAddressIn_o,
	output 			SMMemory_WriteSelectIn_TLC_o,
	output 			SMMemory_ReadSelect_RAM0_o,
	output 			SMMemory_WriteClockIn_o,
	output 			SMMemory_ReadClockIn_o,
	output  [8:0]	SMMemory_WriteDataIn_TLC_o,
	input  	[17:0]	SMMemory_ReadDataOut_SRAM_i,
	output 	[9:0]	SMMemory_WriteAddressIn_o,
	output 		    SMMemory_WriteSelectIn_o,
	output 			SMMemory_ReadSelect_RAM1_o,
	output 			SMMemory_WriteDataIn_o,
	input  	[17:0]	SMMemory_ReadDataOut_SRAM1_i,

	
	output  [8:0]   FFEControlMemory_4k_Address_TLC_o      ,		
	output  [8:0]  	FFEControlMemory_4k_ReadAddress_muxed_o ,
	output    		FFEControlMemory_4k_ram5_wr_en_o,  			
	output    		FFEControlMemory_4k_ram5_rd_en_o,			
	output    		FFEControlMemory_4k_SPI_clk_o, 				
	output    		FFEControlMemory_4k_TLC_FFE_clk2x_muxed_o, 	
	output  [35:0]  FFEControlMemory_4k_WriteData_TLC_o ,
	input   [35:0]  FFEControlMemory_4k_ram5_rd_data_i	,
	output    		FFEControlMemory_4k_ram4_wr_en_o	,	
	output    		FFEControlMemory_4k_ram4_rd_en_o,
	input  [35:0]  FFEControlMemory_4k_ram4_rd_data_i,
	output [9:0]    FFEControlMemory_4k_fabric_ram1Kx9_addr_o,
	output 			FFEControlMemory_4k_ram1_wr_en_o  ,		
	output 			FFEControlMemory_4k_ram1_rd_en_o ,		
	input  [8:0]    FFEControlMemory_4k_ram1_rd_data_i
	

);

wire			SPI_SCLK_gclk;

wire	[6:0]	RegAddr;
wire	[7:0]	RegDataToTLC;
wire	[7:0]	RegDataFromTLC;
wire			RegWriteEnableToTLC;
wire			RegReadDataAck;

wire			TlcFfeCMMuxSelect;
wire			TlcFfeDM1MuxSelect;
wire			TlcFfeDM2MuxSelect;
wire			TLC_SMMuxSelect;
wire			TLC_CMMuxSelect;
wire			TLC_MemorySelect_en;
wire	[2:0]	TLC_MemorySelect;
wire	[11:0]	TLC_MemoryAddress;
wire	[35:0]	TLC_MemoryDataToMemory;
wire	[35:0]	TlcMemoryDataFromMemory;
wire			TLC_WriteEnable;
wire			TLC_ReadEnable;
wire			TLC_MemoryClock;

wire			StartFFE;
wire	[15:0]	TimeStamp;
wire			UseFastClock;
wire	[31:0]	MailboxToFFE;

// Signals associated with Communication Manager function of TLC
wire			CM_ReadEnableFromTLC;
wire	[9:0]	CM_ReadAddressFromTLC;
wire			CM_ReadClockFromTLC;
wire	[3:0]	CMBufferBeingWritten;

// Signals associated with FFE Control Memory
wire	[11:0]	FFECM_WriteAddress;
wire	[11:0]	FFECM_ReadAddress;
wire			FFECM_WriteSelect;
wire			FFECM_ReadSelect;
wire	[35:0]	FFECM_WriteData;
wire	[35:0]	FFECM_ReadData;
wire	[35:0]	MemoryDataFromDM1Mux;

// Signals associated with FFE Data Memory 1
wire	[9:0]	FFEDM1_WriteAddress;
wire	[9:0]	FFEDM1_ReadAddress;
wire			FFEDM1_WriteSelect;
wire			FFEDM1_ReadSelect;
wire	[35:0]	FFEDM1_WriteData;
wire	[35:0]	FFEDM1_ReadData;
wire	[35:0]	MemoryDataFromDM2Mux;

// Signals associated with FFE Data Memory 2
wire	[9:0]	FFEDM2_WriteAddress;
wire	[9:0]	FFEDM2_ReadAddress;
wire			FFEDM2_WriteSelect;
wire			FFEDM2_ReadSelect;
wire	[35:0]	FFEDM2_WriteData;
wire	[35:0]	FFEDM2_ReadData;
wire	[17:0]	MemoryDataFromSMMux;

// Signals associated with Sensor Manager Memory
wire	[9:0]	SM_WriteAddress;
wire	[9:0]	SM_ReadAddress;                        // Expanded for Rel 0 on 6/18
wire			SM_WriteSelect;
wire			SM_ReadSelect;
wire	[8:0]	SM_WriteData;
wire	[17:0]	SM_ReadData;
wire	[8:0]	MemoryDataFromCMMux;

// Signals associated with Communication Manager Memory
wire			ASSP_CMFIFO_wr_clk;
wire	[16:0]	ASSP_CMFIFO_wr_data;
wire			ASSP_CMFIFO_wr_en;
wire			ASSP_CMFIFO_rd_clk;
wire	[16:0]	ASSP_CMFIFO_rd_data;
wire			ASSP_CMFIFO_rd_en;
wire			ASSP_CMFIFO_empty;
wire			ASSP_CMFIFO_full;
wire	[3:0]	ASSP_CMFIFO_rd_flags;
wire	[3:0]	ASSP_CMFIFO_wr_flags;
wire	[8:0]	CM_WriteAddress;
wire	[9:0]	CM_ReadAddress;
wire			CM_WriteSelect;
wire			CM_ReadSelect;
wire	[17:0]	CM_WriteData;
wire	[8:0]	CM_ReadData;

// Signals associated with FFE
wire	[11:0]	FFECM_ReadAddressFromFFE;
wire			FFECM_ReadEnableFromFFE;
wire	[9:0]	FFEDM1_ReadAddressFromFFE;
wire	[9:0]	FFEDM1_WriteAddressFromFFE;
wire	[35:0]	FFEDM1_WriteDataFromFFE;
wire	[9:0]	FFEDM2_ReadAddressFromFFE;
wire	[9:0]	FFEDM2_WriteAddressFromFFE;
wire	[35:0]	FFEDM2_WriteDataFromFFE;
wire	[9:0]	SMSM_ReadAddressFromFFE;                // Expanded for Rel 0 on 6/18
wire			SMSM_ReadEnableFromFFE;
wire	[17:0]	CM_WriteDataFromFFE;
wire			CM_WriteEnableFromFFE;
wire			SMOverrun;								// Error bit when SM still running and FFE trying to start

wire    [9:0]   SMSM_WriteAddressFromFFE;               // New      for Rel 0 on 6/18
wire            SMSM_WriteEnableFromFFE;                // New      for Rel 0 on 6/18
wire	[8:0]	SMSM_WriteDataFromFFE;                  // New      for Rel 0 on 6/18

// Signals associated with Sensor Manager
wire	[9:0]	SMSM_WriteAddressFromSensorManager; 
wire			SMSM_WriteEnableFromSensorManager;
wire	[8:0]	SMSM_WriteDataFromSensorManager;
wire	[9:0]	SMSM_ReadAddressFromSensorManager;      // Expanded for Rel 0 on 6/18
wire			SMSM_ReadEnableFromSensorManager;
wire			SMSM_ReadClockFromSensorManager;
//	wire			InitSMFromTLC;                          // Removed  for Rel 0 on 6/18
wire			StartSMFromTLC;
wire			StartSMFromFFE;

wire			CM_FIFO_ReadEnable;
wire	[8:0]	CM_FIFO_ReadData;
wire	[3:0]	CM_FIFO_Pop_Flags;
wire	[3:0]	CM_FIFO_Push_Flags;

//wire			CM_FIFO_WriteEnable;
//wire	[17:0]	CM_FIFO_WriteData;
wire			CM_FIFO_full;
wire			CM_FIFO_empty;
wire			CM_FIFO_Overflow;
wire			CM_RingBufferMode;
wire			CM_AutoDrain_Busy;
wire			CM_FIFO_ReadClk;
wire			CM_FIFO_PopFromTLC;

wire			FFE_Control_Clock_gclk;
wire			FFE_Control_Clock_x2_gclk;
wire			OperatingClock;

wire			ResetIn;
wire			SW_Reset;

// FFE&SM Clock
wire			FFE_Control_Clock;
wire			FFE_Control_Clock_x2;
wire			FFEandSM_Clock;
wire			FFEBusy;
wire			SMBusy;

wire			FFEandSM_Clock_p;
wire	[9:0]	ReadAddressFromSMorFFE;                  // Expanded for Rel 0 on 6/18
wire			ReadEnableFromSMorFFE;
	
wire    [9:0]   WriteAddressFromSMorFFE;                 // New      for Rel 0 on 6/18
wire            WriteEnableFromSMorFFE;                  // New      for Rel 0 on 6/18
wire	[8:0]	WriteDataFromSMorFFE;                    // New      for Rel 0 on 6/18

wire			FFE_Control_TP_1;
wire			FFE_Control_TP_2;
wire			FFE_Control_TP_3;

wire			SM_ReadClock;
wire			SM_ReadClock_gclk;

wire			TP_SM_1, TP_SM_2, TP_SM_3;
wire			TP_TLC_1, TP_TLC_2;

wire	[7:0]	InterruptMsgFromFFE;

wire			FFE_clock_halfperiod;

reg		[11:0]	TLC_pop_cnt;
reg				TLC_pop_stretched;

wire			SM_Clock;
wire			SM_Clock_gclk;

wire 	[9:0]	CtrlRunTimeAddressReg;                   // Expanded for Rel 0 on 6/18
wire 	[9:0]	CtrlRunTimeAddressOut;                   // Expanded for Rel 0 on 6/18
wire 			CtrlRunTimeAddressSM;

// Sensor On/OFF Control
wire	[2:0]	LEDS_CTRL;

wire			ring_osc_clk;
wire			FFE_CLK_ENABLE;
wire			SmClockSelect;
wire			Main_Clock_Enable;
wire			clock_32KHz_Enable;
wire			Clock_32KHz;
wire			external_Clock;
wire	[2:0]	ffe_clock_select, sm_clock_select;
reg				smClock;
wire			clk_source_select;
wire			OperatingClockRef_buff;
wire 	[3:0] 	ClockGen_State;

wire 	        TimeStamp_Tog;
wire 	        SensorInterrupt_0_FFE;
wire 	        SensorInterrupt_1_FFE;
wire 	        SensorInterrupt_2_FFE;
wire 	        SensorInterrupt_3_FFE;
wire	[3:0]	SMInterruptToFFE;

wire			assp_lb_ram0_clk;
wire	[8:0]	assp_lb_ram0_addr;
wire	[35:0]	assp_lb_ram0_wr_data;
wire	[35:0]	assp_lb_ram0_rd_data;
wire			assp_lb_ram0_wr_en;
wire			assp_lb_ram0_rd_en;
wire	[3:0]	assp_lb_ram0_wr_be;

wire			assp_rb_ram1_clk;
wire	[8:0]	assp_rb_ram1_addr;
wire	[35:0]	assp_rb_ram1_wr_data;
wire	[35:0]	assp_rb_ram1_rd_data;
wire			assp_rb_ram1_wr_en;
wire			assp_rb_ram1_rd_en;
wire	[3:0]	assp_rb_ram1_wr_be;

wire			assp_lb_ram8k_clk;
wire	[11:0]	assp_lb_ram8k_addr;
wire	[16:0]	assp_lb_ram8k_wr_data;
wire	[16:0]	assp_lb_ram8k_rd_data;
wire			assp_lb_ram8k_wr_en;
wire			assp_lb_ram8k_rd_en;
wire	[1:0]	assp_lb_ram8k_wr_be;

wire			RingOsc_cal_en;
wire	[2:0]	RingOsc_select;
wire	[15:0]	clk_cal_value;

wire			FFEDM1_ReadClock;
wire			FFEDM1_WriteClock;
wire			FFEDM2_ReadClock;
wire			FFEDM2_WriteClock;

wire			FFE_Control_Clock_dly1;
wire			FFE_Control_Clock_dly2;
wire			FFE_Control_Clock_dly3;


`ifdef ENABLE_FFE_F0_EXTENDED_DM
	wire		extended_DM1_select;
	wire		extended_DM2_select;
`endif


//  Assign interrupt
assign	SensorInterrupt_0 = SensorInterrupt[0];
assign	SensorInterrupt_1 = SensorInterrupt[1];
assign	SensorInterrupt_2 = SensorInterrupt[2];
assign	SensorInterrupt_3 = SensorInterrupt[3];




// compiler options, from rtl_defines.v
// ENABLE_FFE_F0_EXTENDED_DM
// ENABLE_FFE_F0_PROGRAMMABLE_SEG0_OFFSET
// FFE_F0_SEG0_OFFSET  [value]
// ENABLE_FFE_F0_SINGLE_DM



assign ResetIn = ~ResetInN || SW_Reset;

/*
//// To be Remove
	ring_osc_adjust ring_osc_adjust_1 (
		.reset_i				( ResetIn					),
		.clk_ringosc_div2_i		( ASSP_ringosc_clk_i	),
		.clk_32khz_i			( Clock32KIn				),
		.enable_i				( RingOsc_cal_en			),
		.div_sel_o				( ASSP_ringosc_sel_o		)
	);
	assign ASSP_ringosc_en_o = 1'b1;
*/

	
SystemClockControl clockcontrol_1	(
	.OperatingClockRef_i		( OperatingClockRef			),
	.Clock32KIn_i				( Clock32KIn				),
	.SPIClock_i					( SPI_SCLK					),
	.ResetIn_i					( ResetIn					),
	
	.FfeClkSelect_i				( ffe_clock_select			),
	.SmClkSelect_i				( sm_clock_select			),
	.SmSpeedSelect_i			( SmClockSelect				),
	.SpiClkSelect_i				( UseFastClock				),
	.ClkSourceSelect_i			( clk_source_select			),
	.Clk32KhzEnable_i			( clock_32KHz_Enable		),
	.MainClkEnable_i			( Main_Clock_Enable			),
	.FfeClkEnable_i				( FFE_CLK_ENABLE			),
	.CM_AutoDrain_Busy			( CM_AutoDrain_Busy			),
	
	.SmClock_o					( SM_Clock					),
	.FfeClock_o					( FFE_Control_Clock			),
	.FfeClock_x2_o				( FFE_Control_Clock_x2		),
	.clock_32KHz_o				( Clock_32KHz				),
	.multiplierClk_o			( OperatingClock			),
	.ClockGen_State_o			( ClockGen_State			),
	.CM_FIFO_ReadClk			( CM_FIFO_ReadClk			),
	
	.clk_ringosc_i				( ASSP_ringosc_sysclk_i		),
	.clk_ringosc_x2_i			( ASSP_ringosc_sysclk_x2_i	),
	.enable_i					( RingOsc_cal_en			),
	.clk_cal_value_o			( clk_cal_value				),
	.assp_ringosc_en_o			( ASSP_ringosc_en_o			)
);

// The Dragon SPI version requires a gclkbuff instantiation b/c the SPI CLK is on a GPIO pin

GCLKBUFF clock_buffer_SPI_SCLK (.A(SPI_SCLK), .Z(SPI_SCLK_gclk));

GCLKBUFF clock_buffer_OperatingClock (.A(OperatingClock), .Z(OperatingClock_gclk));

// delay the 1x clock
buff buff_ffe_control_clock1 (.A(FFE_Control_Clock), .Q(FFE_Control_Clock_dly1));
buff buff_ffe_control_clock2 (.A(FFE_Control_Clock_dly1), .Q(FFE_Control_Clock_dly2));
buff buff_ffe_control_clock3 (.A(FFE_Control_Clock_dly2), .Q(FFE_Control_Clock_dly3));
//pragma attribute buff_ffe_control_clock1 dont_touch true
//pragma attribute buff_ffe_control_clock2 dont_touch true
//pragma attribute buff_ffe_control_clock3 dont_touch true

//clock_buffer clock_buffer_FFE_Control_Clock (.A(FFE_Control_Clock), .Z(FFE_Control_Clock_gclk));
GCLKBUFF clock_buffer_FFE_Control_Clock (.A(FFE_Control_Clock_dly3), .Z(FFE_Control_Clock_gclk));

GCLKBUFF clock_buffer_FFE_Control_Clock_x2 (.A(FFE_Control_Clock_x2), .Z(FFE_Control_Clock_x2_gclk));

GCLKBUFF clock_buffer_SM_Clock (.A(SM_Clock), .Z(SM_Clock_gclk));

GCLKBUFF clock_buffer_CM_FIFO_ReadClk (.A(CM_FIFO_ReadClk), .Z(CM_FIFO_ReadClk_gclk));


SPI_slave SPI_slave_1 (
	.rst			( ResetIn				),		// system/global reset (active-high)

	// SPI interface
	.SPI_SCLK		( SPI_SCLK_gclk			),		// base value 0 (mode 0)
	.SPI_MOSI		( SPI_MOSI				),		// master out, slave in
	.SPI_MISO		( SPI_MISO				),		// master in, slave out
	.SPI_SS			( SPI_SS				),		// slave select (active-low)

	// internal interface
	.addr			( RegAddr				),
	.wr_data		( RegDataToTLC			),
	.wr_data_valid	( RegWriteEnableToTLC	),	// active high
	.rd_data		( RegDataFromTLC		),
	.rd_data_ack	( RegReadDataAck		)
);


assign ASSP_ringosc_sel_o = RingOsc_select;
TLC u_TLC (
	// General interface
	.SPI_SCLK					( SPI_SCLK_gclk				),
	.Clock32KIn					( Clock_32KHz				),
	.FFE_Clock					( FFE_Control_Clock_gclk	),
	.ResetIn					( ResetIn					),
	.SW_Reset					( SW_Reset					),

	.RingOsc_cal_en				( RingOsc_cal_en			),
	.RingOsc_select				( RingOsc_select			),
	.RingOsc_cal_value			( clk_cal_value				),

	.I2C_Master_Error			( I2C_Master_Error_i		),
	.FFEBusyIn					( FFEBusy					),
	.SMBusyIn					( SMBusy					),
	.SMOverrunIn				( SMOverrun					),
	.StartFFEOut				( StartFFE					),
//		.InitSMOut					( InitSMFromTLC				),  // Removed  for Rel 0 on 6/18
	.StartSMOut					( StartSMFromTLC			),
	.TimeStampOut				( TimeStamp					),
	.TimeStampOut_Tog			( TimeStamp_Tog				),
	.UseFastClockOut			( UseFastClock				),

	.InterruptMsgFromFFEIn		( InterruptMsgFromFFE		),
	.InterruptPinOut			( Interrupt					),
	.SensorInterruptIn			( SensorInterrupt[4]		),
	
	.FFE_Mailbox_Out			( MailboxToFFE				),
	
	.CtrlRunTimeAddressReg		( CtrlRunTimeAddressReg		),
	.CtrlRunTimeAddressOut		( CtrlRunTimeAddressOut		),
	.CtrlRunTimeAddressSM 		( CtrlRunTimeAddressSM		),


	// Interface to SPI Slave
	.RegAddrIn					( RegAddr					),
	.RegDataIn					( RegDataToTLC				),
	.RegDataOut					( RegDataFromTLC			),
	.RegWriteEnableIn			( RegWriteEnableToTLC		),
	.RegReadDataAckIn			( RegReadDataAck			),

	// Interface to memories
	.TLCDrivingFFEControlMem	( TlcFfeCMMuxSelect			),
	.TLCDrivingFFEDataMem1		( TlcFfeDM1MuxSelect		),
	.TLCDrivingFFEDataMem2		( TlcFfeDM2MuxSelect		),
	.TLCDrivingSMMem			( TLC_SMMuxSelect			),
	.TLCDrivingCMMem			( TLC_CMMuxSelect			),
	.MemorySelect_en			( TLC_MemorySelect_en		),
	.MemorySelect				( TLC_MemorySelect			),
	.MemoryAddressOut			( TLC_MemoryAddress			),
	.MemoryDataOut				( TLC_MemoryDataToMemory	),
	.MemoryDataIn				( TlcMemoryDataFromMemory	),
	.MemoryReadEnableOut		( TLC_ReadEnable			),
	.MemoryWriteEnableOut		( TLC_WriteEnable			),
	.MemoryClockOut				( TLC_MemoryClock			),
	
	// Interface to Communication Manager FIFO
	.CM_FIFO_ReadEnable			( CM_FIFO_PopFromTLC		),
	.CM_FIFO_ReadData			( CM_FIFO_ReadData			),
	.CM_FIFO_PopFlags			( CM_FIFO_Pop_Flags			),
	.CM_FIFO_PushFlags			( CM_FIFO_Push_Flags		),
	.CM_FIFO_Overflow			( CM_FIFO_Overflow			),
	.CM_RingBufferMode			( CM_RingBufferMode			),
	.CM_AutoDrain_Busy			( CM_AutoDrain_Busy			),
	.TP1						( TP_TLC_1					),
	.TP2						( TP_TLC_2					),
	.leds_off_o					( LEDS_CTRL					),
	.FFE_CLK_Enable_o			( FFE_CLK_ENABLE			),
	.ClockEnable_o				( Main_Clock_Enable			),
	.clock_32KHz_Enable_o		( clock_32KHz_Enable		),
	.FFE_Clock_Control_o		( ffe_clock_select			),
	.SM_Clock_Control_o			( sm_clock_select			),
	.ClkSourceSelect_o			( clk_source_select			)
);


assign SMInterruptToFFE = {SensorInterrupt_3_FFE, SensorInterrupt_2_FFE, SensorInterrupt_1_FFE, SensorInterrupt_0_FFE};

FFE_Control u_FFE_Control ( // named RunFlexFusionEngine in C source
	// General interface
	.ClockIn					( FFE_Control_Clock_gclk		),
	.Clock_x2In					( FFE_Control_Clock_x2_gclk		),
	.ResetIn					( ResetIn						),
	.MultClockIn				( OperatingClock_gclk			),
	.MultStateIn				( ClockGen_State				),
	.StartIn					( StartFFE						), //Start from TLC
	.StartSMOut					( StartSMFromFFE				),
	.BusyOut					( FFEBusy						),
	.TimeStampIn				( TimeStamp						),
	.MailboxIn					( MailboxToFFE					),
	.SM_InterruptIn				( SMInterruptToFFE				),
	.InterruptMsgOut			( InterruptMsgFromFFE			),

	// FFE Memories
	.ControlMemAddressOut		( FFECM_ReadAddressFromFFE		),
	.ControlMemReadEnableOut	( FFECM_ReadEnableFromFFE		),
	.SensorMemReadAddressOut	( SMSM_ReadAddressFromFFE		),
	.SensorMemReadEnableOut		( SMSM_ReadEnableFromFFE		),
	.SensorMemWriteAddressOut	( SMSM_WriteAddressFromFFE		), // New for Rel 0 on 6/18 **** Place holder *****
	.SensorMemWriteEnableOut	( SMSM_WriteEnableFromFFE		), // New for Rel 0 on 6/18 **** Place holder *****
	.ControlMemDataIn			( FFECM_ReadData				),
	.Mem1ReadData				( FFEDM1_ReadData				),
	.Mem2ReadData				( FFEDM2_ReadData				),
	.SensorMemReadDataIn		( SM_ReadData					),
	.SensorMemBusyIn			( SMBusy						),


	.SensorMemWriteDataOut		( SMSM_WriteDataFromFFE			), // New for Rel 0 on 6/18 **** Place holder *****
	.DataMem1ReadEnable			( FFEToDM1Mux_ReadEnable		),
	.DataMem2ReadEnable			( FFEToDM2Mux_ReadEnable		),
	.DataMem1WriteEnable		( FFEToDM1Mux_WriteEnable		),
	.DataMem2WriteEnable		( FFEDM2_WriteEnable			),
	.DataMem1ReadAddressOut		( FFEDM1_ReadAddressFromFFE		),
	.DataMem1WriteAddressOut	( FFEDM1_WriteAddressFromFFE	),
	.DataMem1WriteDataOut		( FFEDM1_WriteDataFromFFE		),
	.DataMem2ReadAddressOut		( FFEDM2_ReadAddressFromFFE		),
	.DataMem2WriteAddressOut	( FFEDM2_WriteAddressFromFFE	),
	.DataMem2WriteDataOut		( FFEDM2_WriteDataFromFFE		),

	.FFE_clock_halfperiod		( FFE_clock_halfperiod			),

	.SMBusyIn					( SMBusy						),
	.SMOverrunOut				( SMOverrun						),
	
	.CMWriteDataOut				( CM_WriteDataFromFFE			),
	.CMWriteEnableOut			( CM_WriteEnableFromFFE			),

	.mult_in1					( Amult_o						),
	.mult_in2					( Bmult_o						),
	.mult_enable				( Valid_mult_o					),
	.mult_out					( Cmult_i						),
	
	.TP1						( FFE_Control_TP_1				),
	.TP2						( FFE_Control_TP_2				),
	.TP3						( FFE_Control_TP_3				)
);


assign I2C_wb_clk_o = SM_Clock_gclk;
assign I2C_arst_o   = ResetIn;

// Define the I2C bus control signals
//
/* assign SensorScl_io       = SensorScl_oen_i ? 1'bz : SensorScl_i;
assign SensorSclPUOut     = SensorScl_oen_i ? 1 : 1'bz;

assign SensorSda_io       = SensorSda_oen_i ? 1'bz : SensorSda_i;
assign SensorSdaPUOut     = SensorSda_oen_i ? 1 : 1'bz; */


assign SensorScl_io       = SensorScl_i;
assign SensorSclPUOut     = 1 ;

assign SensorSda_io       = SensorSda_i;
assign SensorSdaPUOut     = 1;
	

SensorManager u_SensorManager (
	.ClockIn			( SM_Clock_gclk							),
	.ResetIn			( ResetIn								),
//		.InitIn				( InitSMFromTLC							), // Removed  for Rel 0 on 6/18
	.StartFromFFE		( StartSMFromFFE						),
	.StartFromTLC		( StartSMFromTLC						),
	.BusyOut			( SMBusy								),
	.TimeStamp_Delta_i      ( TimeStamp[15:0]                   ), // Added    for Rel 0 on 6/18
	.TimeStamp_Delta_Tog_i  ( TimeStamp_Tog                     ), // Added    for Rel 0 on 6/18
	.SensorInterrupt_0_i    ( SensorInterrupt_0                 ), // Added    for Rel 0 on 6/18
	.SensorInterrupt_1_i    ( SensorInterrupt_1                 ), // Added    for Rel 0 on 6/18
	.SensorInterrupt_2_i    ( SensorInterrupt_2                 ), // Added    for Rel 0 on 6/18
	.SensorInterrupt_3_i    ( SensorInterrupt_3                 ), // Added    for Rel 0 on 6/18
	.SensorInterrupt_0_o    ( SensorInterrupt_0_FFE             ), // Added    for Rel 0 on 6/18; needs updated FFE
	.SensorInterrupt_1_o    ( SensorInterrupt_1_FFE             ), // Added    for Rel 0 on 6/18; needs updated FFE
	.SensorInterrupt_2_o    ( SensorInterrupt_2_FFE             ), // Added    for Rel 0 on 6/18; needs updated FFE
	.SensorInterrupt_3_o    ( SensorInterrupt_3_FFE             ), // Added    for Rel 0 on 6/18; needs updated FFE
	.CtrlRunTimeAddressReg	( CtrlRunTimeAddressReg				),
	.CtrlRunTimeAddressOut	( CtrlRunTimeAddressOut				),
	.CtrlRunTimeAddressSM 	( CtrlRunTimeAddressSM				),
	.MemReadAddressOut	( SMSM_ReadAddressFromSensorManager		),
	.MemReadEnableOut	( SMSM_ReadEnableFromSensorManager		),
	.MemReadDataIn		( SM_ReadData							),
	.MemWriteAddressOut	( SMSM_WriteAddressFromSensorManager	),
	.MemWriteEnableOut	( SMSM_WriteEnableFromSensorManager		),
	.MemWriteDataOut	( SMSM_WriteDataFromSensorManager		),
	.MemClockOut		( SMSM_ReadClockFromSensorManager		),
	.I2C_wb_adr_o       ( I2C_wb_adr_o                          ),
	.I2C_wb_dat_o       ( I2C_wb_dat_o                          ),
	.I2C_wb_dat_i       ( I2C_wb_dat_i                          ),
	.I2C_wb_we_o        ( I2C_wb_we_o                           ),
	.I2C_wb_stb_o       ( I2C_wb_stb_o                          ),
	.I2C_wb_cyc_o       ( I2C_wb_cyc_o                          ),
	.I2C_wb_ack_i       ( I2C_wb_ack_i                          ),
	.I2C_tip_i          ( I2C_tip_i                             ),
	.TP1				( TP_SM_1								),
	.TP2				( TP_SM_2								),
	.TP3				( TP_SM_3								),
	.SmClockSelect_o	( SmClockSelect							)
);



	// 4k CM
	FFEControlMemory_4k u_FFEControlMemory (
		// General Interface
		.ResetIn				( ResetIn					),
		.SPI_clk				( SPI_SCLK_gclk				),
		.TLC_FFE_clk2x_muxed	( FFE_Control_Clock_x2_gclk	),

		.MemSelect_en			( TLC_MemorySelect_en		),
		.MemSelect				( TLC_MemorySelect			),

		.FFE_clock_halfperiod	( FFE_clock_halfperiod		),

		.Address_TLC			( TLC_MemoryAddress			),

		.MemoryMux_in			( MemoryDataFromDM1Mux		),
		.MemoryMux_out			( TlcMemoryDataFromMemory	),

		//Read Interface
		.ReadAddress_FFE		( FFECM_ReadAddressFromFFE	),
		.ReadData				( FFECM_ReadData			),
		.ReadEnable_TLC			( TLC_ReadEnable			),
		.ReadEnable_FFE			( FFECM_ReadEnableFromFFE	),

		//Write Interface
		.WriteData_TLC			( TLC_MemoryDataToMemory	),
		.WriteEnable_TLC		( TLC_WriteEnable			),

		// ASSP RAM interface - left bank
		.assp_lb_ram0_clk		( assp_lb_ram0_clk			),
		.assp_lb_ram0_addr		( assp_lb_ram0_addr			),
		.assp_lb_ram0_wr_data	( assp_lb_ram0_wr_data		),
		.assp_lb_ram0_rd_data	( assp_lb_ram0_rd_data		),
		.assp_lb_ram0_wr_en		( assp_lb_ram0_wr_en		),
		.assp_lb_ram0_rd_en		( assp_lb_ram0_rd_en		),
		.assp_lb_ram0_wr_be		( assp_lb_ram0_wr_be		),

		// ASSP RAM interface - right bank
		.assp_rb_ram1_clk		( assp_rb_ram1_clk			),
		.assp_rb_ram1_addr		( assp_rb_ram1_addr			),
		.assp_rb_ram1_wr_data	( assp_rb_ram1_wr_data		),
		.assp_rb_ram1_rd_data	( assp_rb_ram1_rd_data		),
		.assp_rb_ram1_wr_en		( assp_rb_ram1_wr_en		),
		.assp_rb_ram1_rd_en		( assp_rb_ram1_rd_en		),
		.assp_rb_ram1_wr_be		( assp_rb_ram1_wr_be		),

		// ASSP RAM interface - 8k - left bank
		.assp_lb_ram8k_clk		( assp_lb_ram8k_clk			),
		.assp_lb_ram8k_addr		( assp_lb_ram8k_addr		),
		.assp_lb_ram8k_wr_data	( assp_lb_ram8k_wr_data		),
		.assp_lb_ram8k_rd_data	( assp_lb_ram8k_rd_data		),
		.assp_lb_ram8k_wr_en	( assp_lb_ram8k_wr_en		),
		.assp_lb_ram8k_rd_en	( assp_lb_ram8k_rd_en		),
		.assp_lb_ram8k_wr_be	( assp_lb_ram8k_wr_be		),
		
		.FFEControlMemory_4k_Address_TLC_o 				(   FFEControlMemory_4k_Address_TLC_o 			),	
		.FFEControlMemory_4k_ReadAddress_muxed_o        (   FFEControlMemory_4k_ReadAddress_muxed_o     ),
		.FFEControlMemory_4k_ram5_wr_en_o  			    (   FFEControlMemory_4k_ram5_wr_en_o  			), 
		.FFEControlMemory_4k_ram5_rd_en_o 			    (   FFEControlMemory_4k_ram5_rd_en_o 			), 
		.FFEControlMemory_4k_SPI_clk_o 				    (   FFEControlMemory_4k_SPI_clk_o 				), 
		.FFEControlMemory_4k_TLC_FFE_clk2x_muxed_o 	    (   FFEControlMemory_4k_TLC_FFE_clk2x_muxed_o 	), 
		.FFEControlMemory_4k_WriteData_TLC_o            (   FFEControlMemory_4k_WriteData_TLC_o         ),
		.FFEControlMemory_4k_ram5_rd_data_i 			(   FFEControlMemory_4k_ram5_rd_data_i 		    ),
		.FFEControlMemory_4k_ram4_wr_en_o               (   FFEControlMemory_4k_ram4_wr_en_o            ),
		.FFEControlMemory_4k_ram4_rd_en_o               (   FFEControlMemory_4k_ram4_rd_en_o            ),
		.FFEControlMemory_4k_ram4_rd_data_i             (   FFEControlMemory_4k_ram4_rd_data_i          ),
		.FFEControlMemory_4k_fabric_ram1Kx9_addr_o      (   FFEControlMemory_4k_fabric_ram1Kx9_addr_o   ),
		.FFEControlMemory_4k_ram1_wr_en_o               (   FFEControlMemory_4k_ram1_wr_en_o            ),
		.FFEControlMemory_4k_ram1_rd_en_o               (   FFEControlMemory_4k_ram1_rd_en_o            ),
		.FFEControlMemory_4k_ram1_rd_data_i             (   FFEControlMemory_4k_ram1_rd_data_i          )
		
	
		
	);




// connections to the ASSP RAM blocks
assign FCM0_CLK_o			= assp_lb_ram0_clk;
assign #2 FCM0_ADDR_o		= assp_lb_ram0_addr;
assign #2 FCM0_WR_DATA_o	= assp_lb_ram0_wr_data;
assign assp_lb_ram0_rd_data = FCM0_RD_DATA_i;
assign #2 FCM0_WR_EN_o		= assp_lb_ram0_wr_en;
assign #2 FCM0_RD_EN_o		= assp_lb_ram0_rd_en;
assign #2 FCM0_WR_BE_o		= assp_lb_ram0_wr_be;

assign FCM1_CLK_o			= assp_rb_ram1_clk;
assign #2 FCM1_ADDR_o		= assp_rb_ram1_addr;
assign #2 FCM1_WR_DATA_o	= assp_rb_ram1_wr_data;
assign assp_rb_ram1_rd_data	= FCM1_RD_DATA_i;
assign #2 FCM1_WR_EN_o		= assp_rb_ram1_wr_en;
assign #2 FCM1_RD_EN_o		= assp_rb_ram1_rd_en;
assign #2 FCM1_WR_BE_o		= assp_rb_ram1_wr_be;

	assign FCM8K_CLK_o				= assp_lb_ram8k_clk;
	assign #2 FCM8K_ADDR_o			= assp_lb_ram8k_addr;
	assign #2 FCM8K_WR_DATA_o		= assp_lb_ram8k_wr_data;
	assign assp_lb_ram8k_rd_data	= FCM8K_RD_DATA_i;
	assign #2 FCM8K_WR_EN_o			= assp_lb_ram8k_wr_en;
	assign #2 FCM8K_RD_EN_o			= assp_lb_ram8k_rd_en;
	assign #2 FCM8K_WR_BE_o			= assp_lb_ram8k_wr_be;


	// extended-length DM

	// If an extended-length DM is used, physical DM1 is the lower half of logical DM1.
	// Therefore, the TLC select for DM1 must be combined with the most-significant address bit.

	assign extended_DM1_select = TlcFfeDM1MuxSelect && !TLC_MemoryAddress[9];

	FFEDataMemoryMux u_FFEDataMemory1Mux (
		.Select				( extended_DM1_select			),

		.ReadAddressIn0		( FFEDM1_ReadAddressFromFFE		),
		.ReadAddressIn1		( TLC_MemoryAddress[9:0]		),
		.ReadAddressOut		( FFEDM1_ReadAddress			),

		.WriteAddressIn0	( FFEDM1_WriteAddressFromFFE	),
		.WriteAddressIn1	( TLC_MemoryAddress[9:0]		),
		.WriteAddressOut	( FFEDM1_WriteAddress			),

		.DataToMemoryIn0	( FFEDM1_WriteDataFromFFE		),
		.DataToMemoryIn1	( TLC_MemoryDataToMemory		),
		.DataToMemoryOut	( FFEDM1_WriteData				),

		.DataFromMemoryIn0	( MemoryDataFromDM2Mux			),
		.DataFromMemoryIn1	( FFEDM1_ReadData				),
		.DataFromMemoryOut	( MemoryDataFromDM1Mux			),

		.ReadEnable0		( FFEToDM1Mux_ReadEnable		),
		.ReadEnable1		( TLC_ReadEnable				),
		.ReadEnable			( FFEDM1_ReadSelect				),

		.WriteEnable0		( FFEToDM1Mux_WriteEnable		),
		.WriteEnable1		( TLC_WriteEnable				),
		.WriteEnable		( FFEDM1_WriteSelect			)
	);




// DM1 clock
	// single-DM: use 2x FFE clock
	assign FFEDM1_ReadClock	= FFE_Control_Clock_x2_gclk;
	assign FFEDM1_WriteClock = FFE_Control_Clock_x2_gclk;



assign DMM1_WR_CLK_o		= FFEDM1_WriteClock;
assign #2 DMM1_WR_ADDR_o	= FFEDM1_WriteAddress;
assign #2 DMM1_WR_DATA_o	= FFEDM1_WriteData[31:0];
assign #2 DMM1_WR_EN_o 		= FFEDM1_WriteSelect;
assign #2 DMM1_WR_BE_o 		= 4'b1111;
assign DMM1_RD_CLK_o		= FFEDM1_ReadClock;
assign #2 DMM1_RD_ADDR_o	= FFEDM1_ReadAddress;
assign FFEDM1_ReadData		= {4'b0, DMM1_RD_DATA_i};
assign #2 DMM1_RD_EN_o 		= FFEDM1_ReadSelect;


// extended-length DM

	// If an extended-length DM is used, physical DM2 is the upper half of logical DM1, according to s/w.
	// Therefore, the TLC select for DM1 must be combined with the most-significant address bit.

	assign extended_DM2_select = TlcFfeDM1MuxSelect && TLC_MemoryAddress[9];

	FFEDataMemoryMux u_FFEDataMemory2Mux (

		.Select				( extended_DM2_select				),

		.ReadAddressIn0		( FFEDM2_ReadAddressFromFFE			),
		.ReadAddressIn1		( TLC_MemoryAddress[9:0]			),
		.ReadAddressOut		( FFEDM2_ReadAddress				),

		.WriteAddressIn0	( FFEDM2_WriteAddressFromFFE		),
		.WriteAddressIn1	( TLC_MemoryAddress[9:0]			),
		.WriteAddressOut	( FFEDM2_WriteAddress				),

		.DataToMemoryIn0	( FFEDM2_WriteDataFromFFE			),
		.DataToMemoryIn1	( TLC_MemoryDataToMemory			),
		.DataToMemoryOut	( FFEDM2_WriteData					),

		.DataFromMemoryIn0	( {18'h00000,MemoryDataFromSMMux}	),
		.DataFromMemoryIn1	( FFEDM2_ReadData					),
		.DataFromMemoryOut	( MemoryDataFromDM2Mux				),

		.ReadEnable0		( FFEToDM2Mux_ReadEnable			),
		.ReadEnable1		( TLC_ReadEnable					),
		.ReadEnable			( FFEDM2_ReadSelect					),

		.WriteEnable0		( FFEDM2_WriteEnable				),
		.WriteEnable1		( TLC_WriteEnable					),
		.WriteEnable		( FFEDM2_WriteSelect				)
	);


// extended-length single-DM: DM2 driven by the 2x FFE clock
		assign FFEDM2_ReadClock = FFE_Control_Clock_x2_gclk;
		assign FFEDM2_WriteClock = FFE_Control_Clock_x2_gclk;



// DM2 signal connections
	// single-DM

		// extended-length single-DM

		// connect to DM2 as usual (note that the FFE_control module should drive these signals correctly,
		//		accounting for the double-clock rate and the extended length).
		assign DMM2_WR_CLK_o		= FFEDM2_WriteClock;
		assign #2 DMM2_WR_ADDR_o	= FFEDM2_WriteAddress;
		assign #2 DMM2_WR_DATA_o	= FFEDM2_WriteData[31:0];
		assign #2 DMM2_WR_EN_o		= FFEDM2_WriteSelect;
		assign #2 DMM2_WR_BE_o		= 4'b1111;
		assign DMM2_RD_CLK_o		= FFEDM2_ReadClock;
		assign #2 DMM2_RD_ADDR_o	= FFEDM2_ReadAddress;
		assign #2 FFEDM2_ReadData	= {4'b0, DMM2_RD_DATA_i};
		assign #2 DMM2_RD_EN_o		= FFEDM2_ReadSelect;



assign ReadAddressFromSMorFFE  = SMBusy ? SMSM_ReadAddressFromSensorManager  :        SMSM_ReadAddressFromFFE;   // Expanded for Rel 0 on 6/18
assign ReadEnableFromSMorFFE   = SMBusy ? SMSM_ReadEnableFromSensorManager   :        SMSM_ReadEnableFromFFE;

assign WriteAddressFromSMorFFE = SMBusy ? SMSM_WriteAddressFromSensorManager :        SMSM_WriteAddressFromFFE;  // New      for Rel 0 on 6/18
assign WriteEnableFromSMorFFE  = SMBusy ? SMSM_WriteEnableFromSensorManager  :        SMSM_WriteEnableFromFFE;   // New      for Rel 0 on 6/18
assign WriteDataFromSMorFFE    = SMBusy ? SMSM_WriteDataFromSensorManager    :        SMSM_WriteDataFromFFE;     // New      for Rel 0 on 6/18

SMEMemoryMux u_SMEMemoryMux (
	.Select				( TLC_SMMuxSelect									),

	.ReadAddressIn0		( ReadAddressFromSMorFFE							),
	.ReadAddressIn1		( TLC_MemoryAddress[9:0]	 						),      // Expanded for Rel 0 on 6/18
	.ReadAddressOut		( SM_ReadAddress									),

	.WriteAddressIn0	( WriteAddressFromSMorFFE							),      // Expanded for Rel 0 on 6/18
	.WriteAddressIn1	( TLC_MemoryAddress[9:0]							),
	.WriteAddressOut	( SM_WriteAddress									),

	.DataToMemoryIn0	( WriteDataFromSMorFFE								),      // New      for Rel 0 on 6/18
	.DataToMemoryIn1	( TLC_MemoryDataToMemory[8:0]						),
	.DataToMemoryOut	( SM_WriteData										),

	.DataFromMemoryIn0	( {9'h000, 9'h123}									),
	.DataFromMemoryIn1	( SM_ReadData										),
	.DataFromMemoryOut	( MemoryDataFromSMMux								),

	.ReadEnableIn0		( ReadEnableFromSMorFFE								),
	.ReadEnableIn1		( TLC_ReadEnable									),
	.ReadEnableOut		( SM_ReadSelect										),

	.WriteEnableIn0		( WriteEnableFromSMorFFE							),      // New      for Rel 0 on 6/18
	.WriteEnableIn1		( TLC_WriteEnable & ( TLC_MemoryAddress[10])		),      // Expanded for Rel 0 on 6/18
	.WriteEnableOut		( SM_WriteSelect									),

	.ReadClockIn0		( FFE_Control_Clock_gclk							),		// = ClockIn or Clock32K
	.ReadClockIn1		( TLC_MemoryClock									),		// = ClockIn
	.ReadClockOut		( 													)		// use external SM_ReadClock logic instead of this mux
);


SMMemory u_SMMemory (
	.ResetIn			( ResetIn			),
	.SMBusyIn			( SMBusy			),

	//Read Interface  - Both   512 x 18 Memory Blocks
	.ReadAddressIn		( SM_ReadAddress	),
	.ReadDataOut		( SM_ReadData		),
	.ReadSelectIn		( SM_ReadSelect		),
	.ReadClockIn		( SM_Clock_gclk		),

	//Write Interface - Lower 1024 x  9 Memory Block
	.WriteAddressIn_TLC	( TLC_MemoryAddress[9:0]				),                  // New      for Rel 0 on 6/18
	.WriteDataIn_TLC	( TLC_MemoryDataToMemory[8:0]			),                  // New      for Rel 0 on 6/18
	.WriteSelectIn_TLC	( TLC_WriteEnable & (~TLC_MemoryAddress[10])),              // New      for Rel 0 on 6/18

	//Write Interface - Upper 1024 x  9 Memory Block
	.WriteAddressIn		( SM_WriteAddress	),
	.WriteDataIn		( SM_WriteData		),
	.WriteSelectIn		( SM_WriteSelect	),
	.WriteClockIn		( SM_Clock_gclk		),
	
    .SMMemory_WriteAddressIn_TLC_o  ( SMMemory_WriteAddressIn_TLC_o ),
	.SMMemory_ReadAddressIn_o         ( SMMemory_ReadAddressIn_o        ),
	.SMMemory_WriteSelectIn_TLC_o   ( SMMemory_WriteSelectIn_TLC_o  ),
	.SMMemory_ReadSelect_RAM0_o     ( SMMemory_ReadSelect_RAM0_o    ),
	.SMMemory_WriteClockIn_o        ( SMMemory_WriteClockIn_o       ),
	.SMMemory_ReadClockIn_o         ( SMMemory_ReadClockIn_o        ),
	.SMMemory_WriteDataIn_TLC_o     ( SMMemory_WriteDataIn_TLC_o    ),
	.SMMemory_ReadDataOut_SRAM_i    ( SMMemory_ReadDataOut_SRAM_i   ),
	.SMMemory_WriteAddressIn_o	    (	SMMemory_WriteAddressIn_o	),
	.SMMemory_WriteSelectIn_o       (	SMMemory_WriteSelectIn_o 	),
	.SMMemory_ReadSelect_RAM1_o      (	SMMemory_ReadSelect_RAM1_o   	),
	.SMMemory_WriteDataIn_o         (	SMMemory_WriteDataIn_o   	),
	.SMMemory_ReadDataOut_SRAM1_i    (	SMMemory_ReadDataOut_SRAM1_i 	)
	
);


	// 4k CM
	CM_FIFO_1x CM_FIFO_1x_1 (
		.rst		( ResetIn					),

		.push_clk	( FFE_Control_Clock_gclk	),
		.push		( CM_WriteEnableFromFFE		),
		.din		( CM_WriteDataFromFFE		),
		.full		( CM_FIFO_full				),
		.push_flag	( CM_FIFO_Push_Flags		),
		.overflow	( CM_FIFO_Overflow			),

		.pop_clk	( CM_FIFO_ReadClk_gclk		),
		.pop		( CM_FIFO_ReadEnable		),
		.dout		( CM_FIFO_ReadData			),
		.empty		( CM_FIFO_empty				),
		.pop_flag	( CM_FIFO_Pop_Flags			),
		
		.CM_FIFO_1x_din_o				(	CM_FIFO_1x_din_o			),			
		.CM_FIFO_1x_push_int_o		    (	CM_FIFO_1x_push_int_o		),
		.CM_FIFO_1x_pop_int_o		    (	CM_FIFO_1x_pop_int_o		),
		.CM_FIFO_1x_push_clk_o		    (	CM_FIFO_1x_push_clk_o		),
		.CM_FIFO_1x_pop_clk_o		    (	CM_FIFO_1x_pop_clk_o		),
		.CM_FIFO_1x_rst_o			    (	CM_FIFO_1x_rst_o			),
		                                
		.CM_FIFO_1x_almost_full_i	    (	CM_FIFO_1x_almost_full_i	),
		.CM_FIFO_1x_almost_empty_i	    (	CM_FIFO_1x_almost_empty_i	),
		.CM_FIFO_1x_push_flag_i		    (	CM_FIFO_1x_push_flag_i		),
		.CM_FIFO_1x_pop_flag_i		    (	CM_FIFO_1x_pop_flag_i		),
		.CM_FIFO_1x_dout_i			    (	CM_FIFO_1x_dout_i			)
		
		
		
		
	);




// auto drain module, to support Ring Buffer Mode
CM_FIFO_autodrain CM_FIFO_autodrain_1 (
	.rst					( ResetIn					),
	.FFE_CLK_gclk			( CM_FIFO_ReadClk_gclk	),
	//.FFE_CLK_gclk			( FFE_Control_Clock_gclk	),

	.RingBufferMode			( CM_RingBufferMode			),
	.CM_FIFO_PushFlags		( CM_FIFO_Push_Flags		),
	.CM_FIFO_Empty			( CM_FIFO_empty				),
	.CM_FIFO_PopFromTLC		( CM_FIFO_PopFromTLC		),
	.CM_FIFO_ReadData		( CM_FIFO_ReadData			),

	.CM_FIFO_Pop			( CM_FIFO_ReadEnable		),
	.busy					( CM_AutoDrain_Busy			),
	.TP1					( CM_ad_TP1					),
	.TP2					( )
);


// LED
assign LED1	= !CM_RingBufferMode;

// Standard
/*
assign TP1 = 0;
assign TP2 = 0;
assign TP3 = 0;
*/


// For LEDs Demo Control

assign TP1 = LEDS_CTRL[0];
assign TP2 = LEDS_CTRL[1];
assign TP3 = LEDS_CTRL[2];

	
	

	
endmodule
