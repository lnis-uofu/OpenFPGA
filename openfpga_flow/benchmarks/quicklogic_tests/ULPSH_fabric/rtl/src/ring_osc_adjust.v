
/* ------------------------------------------------------------------
ring_osc_adjust.v

Control logic to keep the ring oscillator's frequency within the
desired range.

	divider values from ClockDivider_rev2.pdf:

		SEL		sysclk_x2	sysclk
		--------------------------------
		000		?			?
		001		2			4
		010		3			6
		011		4			8
		100		5			10
		101		6			12
		110		7			14
		111		8			16

------------------------------------------------------------------ */

`timescale 1ps/1ps

/*
module ring_osc_adjust (
	input			reset_i,				// async reset
	input			clk_ringosc_i,		// the ring oscillator output divided by 2 (this is not clk_main)
	input			clk_32khz_i,			// 32.768KHz reference clock
	input			enable_i,				// enable, can be tied off externally or driven occasionally to force re-calibration
	output	[2:0]	div_sel_o				// divider selection control for the ring oscillator divider circuit (in ASSP)
);

reg				clk32k_r1, clk32k_r2;
reg				clk32k_cycle_start;
reg				enable_32k_sync, enable_32k_sync_r1;
reg				enable_main_sync, enable_main_sync_r1, enable_main;
reg		[10:0]	clk_ringosc_div2_cnt;
reg		[2:0]	ring_osc_div, ring_osc_div_reg;

// divider SEL values
localparam	[2:0]	DIV2_SEL = 3'b001;
localparam	[2:0]	DIV3_SEL = 3'b010;
localparam	[2:0]	DIV4_SEL = 3'b011;
localparam	[2:0]	DIV5_SEL = 3'b100;
localparam	[2:0]	DIV6_SEL = 3'b101;
localparam	[2:0]	DIV7_SEL = 3'b110;
localparam	[2:0]	DIV8_SEL = 3'b111;

// threshold values for each divider value.
//		These are the count values where each divider value will be applied.
//		Example: if there are 395 counts on clk_ringosc_div2 within a 32KHz clock period, the ring osc is divided by 5.
//					A divider of 5 means that sysclk_x2 = ring_osc/5, and sysclk = ring_osc/10.
//		Nomenclature:
//			ring_osc		= ring oscillator raw clock (not accessible outside of ASSP)
//			ring_osc_div2	= ring oscillator divided by 2 (used for calibration)
//			sysclk_x2		= ring oscillator divided by SEL
//			sysclk			= ring oscillator divided by SEL*2 (used as system clock A.K.A. FFE clock)
//		Assumptions:
//			Ring osc range: 25.2MHz - 53.2MHz (39.7ns to 18.8ns period)
//			I2C master will divide clk_main by 9 to produce SCL.
//			SCL freq cannot exceed 400KHz.
//			Guardband of 10% is added to allow for temperature/voltage variation, in case calibration is only done once at startup.
//				A smaller guardband can be used if calibration is performed periodically.
localparam	[10:0]	DIV4_THRESHOLD = 11'd32;	// (the threshold of 32 is arbitrary... just needs to be somewhat larger than 0)
localparam	[10:0]	DIV5_THRESHOLD = 11'd395;
localparam	[10:0]	DIV6_THRESHOLD = 11'd494;
localparam	[10:0]	DIV7_THRESHOLD = 11'd595;
localparam	[10:0]	DIV8_THRESHOLD = 11'd693;


// synchronize the enable to clk32k (set this FF on the rising edge of enable_i,
//		clear it after one full 32KHz period has elapsed)
always @(posedge enable_i or posedge clk_32khz_i)
	if (enable_i)
		enable_32k_sync <= 1'b1;
	else
		if (enable_32k_sync_r1)
			enable_32k_sync <= 1'b0;
		else
			enable_32k_sync <= enable_32k_sync;

always @(posedge clk_32khz_i)
	enable_32k_sync_r1 <= enable_32k_sync;

assign enable_32k = enable_32k_sync_r1;


// detect rising edge on clk32khz
always @(posedge clk_ringosc_i) begin
	if (!enable_i) begin
		clk32k_r1 <= 1'b0;
		clk32k_r2 <= 1'b0;
		clk32k_cycle_start <= 1'b0;
	end
	else begin
		clk32k_r1 <= clk_32khz_i;
		clk32k_r2 <= clk32k_r1;
		clk32k_cycle_start <= clk32k_r1 && !clk32k_r2;
	end
end


// synchronize the stretched enable to the main clk domain,
//		turn this enable off when clk32k_cycle_start is active
always @(posedge clk_ringosc_i or posedge reset_i) begin
	if (reset_i) begin
		enable_main_sync <= 1'b0;
		enable_main_sync_r1 <= 1'b0;
		enable_main <= 1'b0;
	end
	else begin
		enable_main_sync <= enable_32k;
		enable_main_sync_r1 <= enable_main_sync;
		case (enable_main)
			1'b0:	if (clk32k_cycle_start && enable_main_sync_r1)
						enable_main <= 1'b1;
					else
						enable_main <= 1'b0;
			1'b1:	if (clk32k_cycle_start && !enable_32k)
						enable_main <= 1'b0;
					else
						enable_main <= 1'b1;
		endcase
	end
end


// count # of clk_ringosc_div2 cycles per 32khz clock period
always @(posedge clk_ringosc_i or posedge reset_i)
	if (reset_i)
		clk_ringosc_div2_cnt <= 0;
	else
		if (enable_main)
			if (clk32k_cycle_start)
				clk_ringosc_div2_cnt <= 0;
			else
				clk_ringosc_div2_cnt <= clk_ringosc_div2_cnt + 1;
		else
			clk_ringosc_div2_cnt <= 0;


// set the ring_osc clock divider value
//		_div holds the temporary divider SEL valud
//		_div_reg gets assigned after a full 32KHz clock period
always @(posedge clk_ringosc_i or posedge reset_i)
	if (reset_i) begin
		ring_osc_div <= 3'b111;		// use the largest divide value by default
		ring_osc_div_reg <= 3'b111;
	end
	else begin
		if (enable_main)
			case (clk_ringosc_div2_cnt)
				DIV4_THRESHOLD:		ring_osc_div <= DIV4_SEL;
				DIV5_THRESHOLD:		ring_osc_div <= DIV5_SEL;
				DIV6_THRESHOLD:		ring_osc_div <= DIV6_SEL;
				DIV7_THRESHOLD:		ring_osc_div <= DIV7_SEL;
				DIV8_THRESHOLD:		ring_osc_div <= DIV8_SEL;
				default:	ring_osc_div <= ring_osc_div;	// hold for all other values
			endcase
		else
			ring_osc_div <= ring_osc_div;	// need to retain the old value when enable is off

		if (clk32k_cycle_start)
			ring_osc_div_reg <= ring_osc_div;
		else
			ring_osc_div_reg <= ring_osc_div_reg;
	end

assign div_sel_o = ring_osc_div_reg;


//// New Logic to produce CNT to system
//// Detect transition of Calibration aneble from Low to Hi
always @(posedge clk_32khz_i or posedge reset_i)
begin
	if (reset_i) begin
		enable_r1 <= 1'b0;
		enable_r2 <= 1'b0;
		enable_r3 <= 1'b0;
	end
	else begin
		enable_r1 <= enable_i;
		enable_r2 <= enable_r1;
		enable_r3 <= enable_r2;
	end
end

// Generating enable for Clock Cnt circuit
// Default is is 2 32KHz CLK period
always @(posedge clk_32khz_i or posedge reset_i)
begin
	if (reset_i)
		downCnt <= 2'b0;
	else
		if (enable_r2 && !enable_r3)
			downCnt <= 2'b11;
		else if (downCnt[1] || downCnt[0])
			downCnt <= downCnt - 1'b1;
		else
			downCnt <= downCnt;
end

// Sync to ring osc clk
always @(posedge clk_ringosc_i or posedge reset_i)
	if (reset_i)
		downCnt1_r1 <= 1'b0;
		downCnt1_r2 <= 1'b0;
	else
		downCnt1_r1 <= downCnt[1];
		downCnt1_r2 <= downCnt1_r1;

assign ringosccnt_reset = reset_i && !enable_i;
// Counting # of ringosc cyces in two 32KHz clock
always @(posedge clk_ringosc_i or posedge ringosccnt_reset)
begin
	if (ringosccnt_reset)
		ringosc_2_cnt <= 16'b0;
	else if (downCnt1_r2)
		ringosc_2_cnt <= ringosc_2_cnt + 1'b0;
	else
		ringosc_2_cnt <= ringosc_2_cnt;
end


endmodule
*/

module ring_osc_adjust (
	input			reset_i,				// async reset
	input			clk_ringosc_i,		// the ring oscillator output divided by 2 (this is not clk_main)
	input			clk_32khz_i,			// 32.768KHz reference clock
	input			enable_i,				// enable, can be tied off externally or driven occasionally to force re-calibration
	output	[15:0]	cal_val_o				// divider selection control for the ring oscillator divider circuit (in ASSP)
);

reg			enable_r1, enable_r2, enable_r3;
reg	[2:0]	downCnt;
reg			downCnt1_r1, downCnt1_r2;
reg	[15:0]	ringosc_2_cnt;
wire		ringosccnt_reset;

//// New Logic to produce CNT to system
//// Detect transition of Calibration aneble from Low to Hi
always @(posedge clk_32khz_i or posedge reset_i)
begin
	if (reset_i) begin
		enable_r1 <= 1'b0;
		enable_r2 <= 1'b0;
		enable_r3 <= 1'b0;
	end
	else begin
		enable_r1 <= enable_i;
		enable_r2 <= enable_r1;
		enable_r3 <= enable_r2;
	end
end

// Generating enable for Clock Cnt circuit
// Default is is 2 32KHz CLK period
always @(posedge clk_32khz_i or posedge reset_i)
begin
	if (reset_i)
		downCnt <= 3'b0;
	else
		if (enable_r2 && !enable_r3)
			downCnt <= 3'b111;
		else if (downCnt != 3'b000)
			downCnt <= downCnt - 1'b1;
		else
			downCnt <= downCnt;
end

// Sync to ring osc clk
always @(posedge clk_ringosc_i or posedge reset_i)
	if (reset_i)
	begin
		downCnt1_r1 <= 1'b0;
		downCnt1_r2 <= 1'b0;
	end
	else
	begin
		downCnt1_r1 <= downCnt[2];
		downCnt1_r2 <= downCnt1_r1;
	end

assign ringosccnt_reset = reset_i || !enable_i;
// Counting # of ringosc cyces in two 32KHz clock
always @(posedge clk_ringosc_i or posedge ringosccnt_reset)
begin
	if (ringosccnt_reset)
		ringosc_2_cnt <= 16'b0;
	else if (downCnt1_r2)
		ringosc_2_cnt <= ringosc_2_cnt + 1'b1;
	else
		ringosc_2_cnt <= ringosc_2_cnt;
end

assign	cal_val_o = ringosc_2_cnt;

endmodule