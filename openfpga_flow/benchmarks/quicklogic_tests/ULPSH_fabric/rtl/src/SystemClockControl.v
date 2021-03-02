
`timescale 1ns / 10ps

module SystemClockControl (
	OperatingClockRef_i,
	Clock32KIn_i,
	SPIClock_i,
	ResetIn_i,
	
	FfeClkSelect_i,
	SmClkSelect_i,
	SmSpeedSelect_i,
	SpiClkSelect_i,
	ClkSourceSelect_i,
	Clk32KhzEnable_i,
	MainClkEnable_i,
	FfeClkEnable_i,
	CM_AutoDrain_Busy,
	
	SmClock_o,
	FfeClock_o,
	FfeClock_x2_o,
	clock_32KHz_o,
	multiplierClk_o,
	ClockGen_State_o,
	CM_FIFO_ReadClk,
	
	clk_ringosc_i,
	clk_ringosc_x2_i,
	enable_i,
	clk_cal_value_o,
	assp_ringosc_en_o
);

// IO Declaration
input			OperatingClockRef_i;
input			Clock32KIn_i;
input			SPIClock_i;
input			ResetIn_i;
	
input	[2:0]	FfeClkSelect_i;
input	[2:0]	SmClkSelect_i;
input			SmSpeedSelect_i;
input			SpiClkSelect_i;
input			ClkSourceSelect_i;
input			Clk32KhzEnable_i;
input			MainClkEnable_i;
input			FfeClkEnable_i;
input			CM_AutoDrain_Busy;

output			SmClock_o;
output			FfeClock_o;
output			FfeClock_x2_o;
output			clock_32KHz_o;
output			multiplierClk_o;
output	[3:0]	ClockGen_State_o;
output			CM_FIFO_ReadClk;

input			clk_ringosc_i;
input			clk_ringosc_x2_i;
input			enable_i;
output	[15:0]	clk_cal_value_o;
output			assp_ringosc_en_o;

reg				multiplierClk_o;
wire	[3:0]	ClockGen_State_o;
wire			CM_FIFO_ReadClk;
wire			assp_ringosc_en_o;



// Internal Signals Declaration
wire			highSpeedClock, highSpeedClock_buff, highSpeedClock_x2_buff;
reg		[6:0]	ClockDiv;
wire			FfeClock;
reg				SmClock;
wire			RingOscEnable;
wire			ring_osc_clk;
wire			OperatingClockRef;

	
// Operations

// Gating the enternal osc
// assign	OperatingClockRef = OperatingClockRef_i && MainClkEnable_i;
assign	OperatingClockRef = OperatingClockRef_i;

// CM FIFO AutoDrain Read Clock
// assign CM_FIFO_ReadClk = CM_AutoDrain_Busy ? (FfeClock && FfeClkEnable_i): SPIClock_i;
assign CM_FIFO_ReadClk = CM_AutoDrain_Busy ? highSpeedClock_buff : SPIClock_i;

// Ring Osclilator enable when the when weither FFE or SM is busy
// assign RingOscEnable = !ResetIn_i && MainClkEnable_i && ClkSourceSelect_i;

// Logic to gate 32KHz clock when the ULPSH goes to sleep
// Only static power consumption
assign	clock_32KHz_o = Clock32KIn_i && Clk32KhzEnable_i;

// Logic to select between the external high speed clock and the internal ring oscillation
// and main clock division
// assign	highSpeedClock = ClkSourceSelect_i ?  ring_osc_clk : OperatingClockRef;
buff buff_highSpeedClock (.A(clk_ringosc_i), .Q(highSpeedClock_buff));	// don't use a clock network for this
//pragma attribute buff_highSpeedClock dont_touch true

buff buff_highSpeedClock_x2 (.A(clk_ringosc_x2_i), .Q(highSpeedClock_x2_buff));	// don't use a clock network for this
//pragma attribute buff_highSpeedClock_x2 dont_touch true



// FFE CLK and SM CLK select and masking
// assign FfeClock_o = SpiClkSelect_i ? SPIClock_i : FfeClock && FfeClkEnable_i;
// assign SmClock_o = SmSpeedSelect_i ? SmClock : (SpiClkSelect_i ? SPIClock_i : FfeClock);
assign FfeClock_o = SpiClkSelect_i ? SPIClock_i : highSpeedClock_buff;
assign FfeClock_x2_o = SpiClkSelect_i ? SPIClock_i : highSpeedClock_x2_buff;
assign SmClock_o = SpiClkSelect_i ? SPIClock_i : highSpeedClock_buff;


ring_osc_adjust ring_osc_adjust_1 (
		.reset_i			( ResetIn_i				),
		.clk_ringosc_i		( clk_ringosc_i			),
		.clk_32khz_i		( Clock32KIn_i			),
		.enable_i			( enable_i				),
		.cal_val_o			( clk_cal_value_o		)
	);

assign assp_ringosc_en_o = ClkSourceSelect_i || MainClkEnable_i;
	
endmodule
