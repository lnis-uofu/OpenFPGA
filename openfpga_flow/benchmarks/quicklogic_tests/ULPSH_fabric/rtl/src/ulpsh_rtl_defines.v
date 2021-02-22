// -----------------------------------------------------------------------------
// title          : ulpsh_rtl_defines.v
// project        : ULP Sensor Hub
// description    : RTL defines
// -----------------------------------------------------------------------------
// copyright (c) 2014, QuickLogic Corporation
// -----------------------------------------------------------------------------

// Clock Circuit control defines
`define 	OP_CLK_DIV		8'b01000000
`define 	FFE1CLK_FREQ_SLT	8'b01000000
`define 	SM1CLK_FREQ_SLT		8'b00100000

`ifdef SIMULATION
	`define 	OSC_SELECTION		1'b0
`else
	`define 	OSC_SELECTION		1'b1
`endif

`define 	ENABLE_FFE_F0_SINGLE_DM

`define 	ENABLE_FFE_F0_EXTENDED_DM

`define 	FFE_F0_SEG0_OFFSET  			9'h095

`define 	ENABLE_FFE_F0_CM_SIZE_4K
