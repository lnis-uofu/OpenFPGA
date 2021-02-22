
`ifndef SensorHub_Definitions

	`define SensorHub_Definitions

	// Communication Manager register offsets
	`define CommandReg				7'h00
	`define StatusReg				7'h01
	`define milSecSample			7'h02
	`define InterruptCtrl			7'h03
	`define InterruptStat			7'h04
	`define CalValueLow				7'h05
	`define CalValueHi				7'h06
	`define Reserved_10				7'h07
	`define Reserved_11				7'h08
	`define Reserved_12				7'h09
	`define Reserved_13				7'h0A
	`define Reserved_14				7'h0B
	`define CM_FIFO_Data			7'h0C
	`define CM_Control				7'h0D
	`define CM_Status				7'h0E
	`define CM_FIFO_Flags_0			7'h0F
	`define Reserved_1				7'h10	/* reserved for CM_FIFO_Flags_23 */
	`define MailboxToFFE_0			7'h11
	`define MailboxToFFE_1			7'h12
	`define MailboxToFFE_2			7'h13
	`define MailboxToFFE_3			7'h14
	`define InterruptFFEMsg			7'h15
	`define Reserved_5				7'h16
	`define Reserved_6				7'h17
	`define Reserved_7				7'h18
	`define RunTimeAdrReg           7'h19
	`define DemoLedCtrlReg          7'h1A   /* Register to control demo LEDs */
	`define ClocksControl           7'h1B
	`define	SleepControl			7'h1C
	`define RunTimeAdrReg_Upr       7'h1D   /* New for Rel 0 on 6/18 */
	`define MemAddrLow				7'h20
	`define MemAddrHigh				7'h21
	`define MemSelect				7'h22
	`define MemDataByte0			7'h28
	`define MemDataByte1			7'h29
	`define MemDataByte2			7'h2A
	`define MemDataByte3			7'h2B
	`define MemDataByte4			7'h2C
	

	`define	CtrlSensorManager		8'h00
	`define CtrlReceiveAddress		8'h01
	`define CtrlRunTimeAddress		8'h02
	`define	MasterPreLo				8'h08
	`define	MasterPreHi				8'h09
	`define MasterCTR				8'h0A
	`define MasterTXR				8'h0B
	`define MasterCR				8'h0C


	`define CR_START		8'b1000_0000
	`define CR_STOP			8'b0100_0000
	`define CR_READ			8'b0010_0000
	`define CR_WRITE		8'b0001_0000
	`define CR_NAK			8'b0000_1000
	// `define CR_POLL			8'b0000_0100
	`define CR_IACK			8'b0000_0001

`endif
