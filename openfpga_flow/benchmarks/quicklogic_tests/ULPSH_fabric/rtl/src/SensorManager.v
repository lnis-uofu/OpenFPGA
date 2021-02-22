// -----------------------------------------------------------------------------
// title          : Sensor Manager Main Routine
// project        : ULP Sensor Hub
// -----------------------------------------------------------------------------
// file           : SensorManager.v
// author         : OCTO
// company        : QuickLogic Corp
// created        : 2012/??/??	
// last update    : 2013/12/06
// platform       : PolarPro III
// standard       : Verilog 2001
// -----------------------------------------------------------------------------
// description: The Sensor Manger is responsible for controlling various
//              external sensors and storing the resulting data in Sensor
//              Manager Memory. These include performing transfers between 
//              Sensor Memory and various registers.
// -----------------------------------------------------------------------------
// copyright (c) 2013
// -----------------------------------------------------------------------------
// revisions  :
// date            version    author         description
// 2013/12/06      1.0        Glen Gomes     Updated -> Mostly re-written
// -----------------------------------------------------------------------------
// Comments: This solution is specifically for use with the QuickLogic
//           PolarPro III device. 
// -----------------------------------------------------------------------------

`timescale 1ns / 10ps

module SensorManager (

	                // General interface
	                ClockIn,
	                ResetIn,
                    StartFromFFE,
                    StartFromTLC,
	                BusyOut,
                    TimeStamp_Delta_i,
                    TimeStamp_Delta_Tog_i,
                    SensorInterrupt_0_i,
                    SensorInterrupt_1_i,
                    SensorInterrupt_2_i,
                    SensorInterrupt_3_i,
                    SensorInterrupt_0_o,
                    SensorInterrupt_1_o,
                    SensorInterrupt_2_o,
                    SensorInterrupt_3_o,
                    CtrlRunTimeAddressSM,
                    CtrlRunTimeAddressOut,
                    CtrlRunTimeAddressReg,
	                MemReadAddressOut,
	                MemReadEnableOut,
	                MemReadDataIn,
	                MemWriteAddressOut,
	                MemWriteEnableOut,
	                MemWriteDataOut,
	                MemClockOut,
	                I2C_wb_adr_o,
	                I2C_wb_dat_o,
	                I2C_wb_dat_i,
	                I2C_wb_we_o,
	                I2C_wb_stb_o,
	                I2C_wb_cyc_o,
	                I2C_wb_ack_i,
	                I2C_tip_i,
	                TP1,
	                TP2,
	                TP3,
					SmClockSelect_o
);	
	
//-----Port Signals--------------------
//

input               ClockIn;
input               ResetIn;
input           	StartFromFFE;
input               StartFromTLC;
output              BusyOut;
input       [15:0]  TimeStamp_Delta_i;
input               TimeStamp_Delta_Tog_i;
input               SensorInterrupt_0_i;
input               SensorInterrupt_1_i;
input               SensorInterrupt_2_i;
input               SensorInterrupt_3_i;
output              SensorInterrupt_0_o;
output              SensorInterrupt_1_o;
output              SensorInterrupt_2_o;
output              SensorInterrupt_3_o;
input               CtrlRunTimeAddressSM;
input        [9:0]  CtrlRunTimeAddressOut;
output       [9:0]  CtrlRunTimeAddressReg;
output       [9:0]  MemReadAddressOut;  // Expanded for Rel 0 on 6/18
output              MemReadEnableOut;
input       [17:0]  MemReadDataIn;
output       [9:0]  MemWriteAddressOut;
output              MemWriteEnableOut;
output       [8:0]  MemWriteDataOut;
output              MemClockOut;
output       [2:0]  I2C_wb_adr_o;
output       [7:0]  I2C_wb_dat_o;
input        [7:0]  I2C_wb_dat_i;
output              I2C_wb_we_o;
output              I2C_wb_stb_o;
output              I2C_wb_cyc_o;
input               I2C_wb_ack_i;
input               I2C_tip_i;
output              TP1;
output              TP2;
output              TP3;
output				SmClockSelect_o;


wire                ClockIn;
wire                ResetIn;
wire                StartFromFFE;
wire            	StartFromTLC;
wire                BusyOut;
wire        [15:0]  TimeStamp_Delta_i;
wire                TimeStamp_Delta_Tog_i;
wire                SensorInterrupt_0_i;
wire                SensorInterrupt_1_i;
wire                SensorInterrupt_2_i;
wire                SensorInterrupt_3_i;
reg                 SensorInterrupt_0_o;
reg                 SensorInterrupt_1_o;
reg                 SensorInterrupt_2_o;
reg                 SensorInterrupt_3_o;
wire                CtrlRunTimeAddressSM;
wire         [9:0]  CtrlRunTimeAddressOut;
reg          [9:0]  CtrlRunTimeAddressReg;
wire  		 [9:0]  MemReadAddressOut;  // Expanded for Rel 0 on 6/18
wire                MemReadEnableOut;
wire        [17:0]  MemReadDataIn;
wire         [9:0]  MemWriteAddressOut;
wire                MemWriteEnableOut;
reg          [8:0]  MemWriteDataOut;
wire         [2:0]  I2C_wb_adr_o;
wire         [7:0]  I2C_wb_dat_o;
wire         [7:0]  I2C_wb_dat_i;
wire                I2C_wb_we_o;
wire                I2C_wb_stb_o;
wire                I2C_wb_cyc_o;
wire                I2C_wb_ack_i;
wire                I2C_tip_i;
wire                MemClockOut;
wire                TP1;
wire                TP2;
wire                TP3;
reg					SmClockSelect_o;


//-----Internal Signals--------------------
//

wire		    	wb_cyc;

wire			    wb_we;
wire			    wb_stb;

wire                wb_ack;
reg                 wb_ack_sm;

wire                wb_busy_poll;

reg          [9:0]  CtrlReceiveAddressReg;

reg          [5:0]  CtrlMailBoxSegmentCtr;
reg                 CtrlMailBoxSegmentCtr_ce;

reg          [9:2]  CtrlMailBoxTablePtr;

reg          [9:0]  CtrlMailBoxJumpInstPtr;

reg                 CtrlMailBoxJumpInstCycle;
reg                 CtrlMailBoxJumpInstCycle_ce;
	
wire         [9:0]  StateMachineCtrlMemAddr;
wire         [7:0]  i2c_masterDataToMem;

wire                save_reg_2_mem;

wire                control_sensor_manager_reg_dcd;
wire                control_receive_reg_dcd;
wire                control_runtime_reg_dcd;
wire                control_jump_reg_dcd;
wire                control_mailbox_tbl_ptr_dcd;
wire                control_mailbox_jump_inst_ptr_dcd;

reg                 RunSM;
wire                BusySM;

wire                StartIn_stb;

reg                 StartFromFFE_1ff;
reg                 StartFromFFE_2ff;
reg                 StartFromFFE_3ff;
reg                 StartFromFFE_4ff;
reg                 StartFromFFE_5ff;
reg                 StartFromFFE_6ff;

reg                 StartFromTLC_1ff;
reg                 StartFromTLC_2ff;

reg                 CtrlRunTimeAddressSM_1ff;
reg                 CtrlRunTimeAddressSM_2ff;

reg					s1_BusyOut, s2_BusyOut;

// Define write enable controls for each TimeStamp register
//
wire                SensorInterrupt_event_reg_we_dcd;

wire                SensorInterrupt_event_mask_we_dcd;

reg                 SensorInterrupt_event_mask_0;
reg                 SensorInterrupt_event_mask_1;
reg                 SensorInterrupt_event_mask_2;
reg                 SensorInterrupt_event_mask_3;


// Delta Time Stamp registers for each sensor
//
reg          [7:0]  TimeStamp_Delta_sensor_0;
reg          [7:0]  TimeStamp_Delta_sensor_1;
reg          [7:0]  TimeStamp_Delta_sensor_2;
reg          [7:0]  TimeStamp_Delta_sensor_3;

reg         [15:0]  TimeStamp_Delta_capt;
reg         [15:0]  TimeStamp_Delta_readback;

reg                 TimeStamp_Delta_Tog_1ff;
reg                 TimeStamp_Delta_Tog_2ff;
reg                 TimeStamp_Delta_Tog_3ff;

// Meta-state registers for each sensor interrupt
//
reg                 SensorInterrupt_0_1ff;
reg                 SensorInterrupt_0_2ff;
reg                 SensorInterrupt_0_3ff;

reg                 SensorInterrupt_1_1ff;
reg                 SensorInterrupt_1_2ff;
reg                 SensorInterrupt_1_3ff;

reg                 SensorInterrupt_2_1ff;
reg                 SensorInterrupt_2_2ff;
reg                 SensorInterrupt_2_3ff;

reg                 SensorInterrupt_3_1ff;
reg                 SensorInterrupt_3_2ff;
reg                 SensorInterrupt_3_3ff;


//  Wait Instruction Registers
//
wire                control_wait_instr_reg_dcd;

reg         [12:0]  control_wait_instr_cntr;
reg         [12:0]  control_wait_instr_cntr_nxt;

reg                 control_wait_instr_cntr_tc;
reg                 control_wait_instr_cntr_tc_nxt;

reg                 control_wait_instr_busy;
wire                control_wait_instr_busy_nxt;



//------Define Parameters---------
//

//
// Define the various address spaces
//
// Note: These correspond to bits [7:3] of the register address field.
//
parameter SENSOR_MANAGER_ADR      = 5'h0;
parameter I2C_MASTER_ADR          = 5'h1;
parameter TIMESTAMP_DELTA_ADR     = 5'h2;



//
// Define the available registers in the Sensor Manager
//
// Note: These correspond to bits [2:0] of the register address field.
//
parameter CONTROL_SENSOR_MANAGER_ADDRESS  = 3'h0;
parameter CONTROL_RECEIVE_ADDRESS         = 3'h1;
parameter CONTROL_RUNTIME_ADDRESS         = 3'h2;
parameter CONTROL_WAIT_INSTR_ADDRESS      = 3'h4;
parameter CONTROL_MAILBOX_TABLE_PTR       = 3'h5;
parameter CONTROL_MAILBOX_JUMP_INST_PTR   = 3'h6;
parameter CONTROL_JUMP_ADDRESS            = 3'h7;


//
// Define the key registers in the I2C Master IP
//
// Note: These correspond to bits [2:0] of the register address field.
//
parameter I2C_MASTER_PRELO		  = 3'h0;
parameter I2C_MASTER_PREHI        = 3'h1;
parameter I2C_MASTER_CTR          = 3'h2;
parameter I2C_MASTER_TXR          = 3'h3;
parameter I2C_MASTER_CR           = 3'h4;


// Define the available registers in the TimeStamp Logic
//
// Note: These correspond to bits [2:0] of the register address field.
//
parameter TIMESTAMP_DELTA_SENSOR_0    = 3'h0;
parameter TIMESTAMP_DELTA_SENSOR_1    = 3'h1;
parameter TIMESTAMP_DELTA_SENSOR_2    = 3'h2;
parameter TIMESTAMP_DELTA_SENSOR_3    = 3'h3;
parameter TIMESTAMP_DELTA_GENERIC_LSB = 3'h4;
parameter TIMESTAMP_DELTA_GENERIC_MSB = 3'h5;
parameter TIMESTAMP_DELTA_INT_EVENT   = 3'h6;


//
// Define the default location of the Mail Box structure in SM Memory
//
parameter MAILBOX_SM_ADDRESS      = 8'hFF; // Note: This is "3FC" shifted by two to the right

//------Logic Operations----------
//

// I2C Interface to the Right Bank ASSP
//
assign  I2C_wb_dat_o                     = MemReadDataIn[15:8];
assign  i2c_masterDataToMem              = I2C_wb_dat_i;
assign  I2C_wb_we_o                      = wb_we;
assign  I2C_wb_stb_o                     = wb_stb;

// Decode the Wishbone bus address space(s)
//
assign I2C_wb_cyc_o                      = (MemReadDataIn[7:3] == I2C_MASTER_ADR) & wb_cyc & ~CtrlMailBoxJumpInstCycle;


// Define the write enables controls for each register
//
assign control_sensor_manager_reg_dcd  = (MemReadDataIn[7:0] == {SENSOR_MANAGER_ADR, CONTROL_SENSOR_MANAGER_ADDRESS}) & wb_cyc & wb_stb & wb_we & ~wb_ack_sm & ~CtrlMailBoxJumpInstCycle;
assign control_runtime_reg_dcd         = (MemReadDataIn[7:0] == {SENSOR_MANAGER_ADR, CONTROL_RUNTIME_ADDRESS})        & wb_cyc & wb_stb & wb_we & ~wb_ack_sm & ~CtrlMailBoxJumpInstCycle;

assign control_wait_instr_reg_dcd      = (MemReadDataIn[7:0] == {SENSOR_MANAGER_ADR, CONTROL_WAIT_INSTR_ADDRESS})     & wb_cyc & wb_stb & wb_we & ~wb_ack_sm & ~CtrlMailBoxJumpInstCycle;

assign control_receive_reg_dcd         = ((MemReadDataIn[7:0] == { SENSOR_MANAGER_ADR, CONTROL_RECEIVE_ADDRESS})
		                               |  (MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_SENSOR_0})
                                       |  (MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_SENSOR_1})
                                       |  (MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_SENSOR_2})
                                       |  (MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_SENSOR_3})
                                       |  (MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_GENERIC_MSB})
                                       |  (MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_GENERIC_LSB})) & wb_cyc & wb_stb & wb_we & ~wb_ack_sm & ~CtrlMailBoxJumpInstCycle;



// Define the write enable for the Interrupt event of the Time Stamp Logic
//
// Note: This write occurs after the write of interrupt data to SM Memory
//
assign SensorInterrupt_event_reg_we_dcd  = (MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_INT_EVENT})   & wb_cyc & wb_stb & ~wb_we &  wb_ack_sm & ~CtrlMailBoxJumpInstCycle;
assign SensorInterrupt_event_mask_we_dcd = (MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_INT_EVENT})   & wb_cyc & wb_stb &  wb_we & ~wb_ack_sm & ~CtrlMailBoxJumpInstCycle;
assign TimeStamp_Delta_lsb_reg_we_dcd    = (MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_GENERIC_LSB}) & wb_cyc & wb_stb &  wb_we & ~wb_ack_sm & ~CtrlMailBoxJumpInstCycle;


// Deterimine if the current cycle is a write to the instruction pointer.
//
// Note: This only happens during the "jump" instruction
//
//       This signals the Sensor Manager Statemachine that the current cycle
//       is a "jump" and to load the register data value into the instruction
//       pointer at the end of the current "register write" instruction.
//
assign control_jump_reg_dcd              = (MemReadDataIn[7:0] == {SENSOR_MANAGER_ADR, CONTROL_JUMP_ADDRESS})                                                 & ~CtrlMailBoxJumpInstCycle;


// Determine if the current cycle is the start of a Mail Box based Jump
// sequence.
//
// Note: The value of the bits in the Mail Box region of SM Memory determine
//       if the current jump address should be loaded into the instruction
//       pointer or if it should be ignored.
//
assign control_mailbox_tbl_ptr_dcd       = (MemReadDataIn[7:0] == {SENSOR_MANAGER_ADR, CONTROL_MAILBOX_TABLE_PTR})     & wb_cyc & wb_stb & wb_we & ~wb_ack_sm & ~CtrlMailBoxJumpInstCycle;
assign control_mailbox_jump_inst_ptr_dcd = (MemReadDataIn[7:0] == {SENSOR_MANAGER_ADR, CONTROL_MAILBOX_JUMP_INST_PTR}) & wb_cyc & wb_stb & wb_we &  wb_ack_sm & ~CtrlMailBoxJumpInstCycle;


// Determine if the current address should include a write to the Sensor Manager's Memory
//
// Note: There is currently only one address but this may expand with, for
//       example, a TimeStamp function.
//
assign save_reg_2_mem                  = ((MemReadDataIn[7:0] == {     I2C_MASTER_ADR, I2C_MASTER_CR}) &  MemReadDataIn[13] & ~CtrlMailBoxJumpInstCycle)
                                       | ((MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_SENSOR_0})           & ~CtrlMailBoxJumpInstCycle)
                                       | ((MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_SENSOR_1})           & ~CtrlMailBoxJumpInstCycle)
                                       | ((MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_SENSOR_2})           & ~CtrlMailBoxJumpInstCycle)
                                       | ((MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_SENSOR_3})           & ~CtrlMailBoxJumpInstCycle)
                                       | ((MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_GENERIC_LSB})        & ~CtrlMailBoxJumpInstCycle)
                                       | ((MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_GENERIC_MSB})        & ~CtrlMailBoxJumpInstCycle)
                                       | ((MemReadDataIn[7:0] == {TIMESTAMP_DELTA_ADR, TIMESTAMP_DELTA_INT_EVENT})          & ~CtrlMailBoxJumpInstCycle);


// Determine if the Wishbone device requires monitoring its busy signal
//
// Note: The only device currently supported is the I2C Master IP. This IP
//       will generate a I2C bus cycle when the "read" or "write"
//       bits are set in the control register.
//
assign wb_busy_poll                    = ((MemReadDataIn[7:0] == {I2C_MASTER_ADR,     I2C_MASTER_CR             }) & (MemReadDataIn[12] | MemReadDataIn[13]) & ~CtrlMailBoxJumpInstCycle)
                                       | ((MemReadDataIn[7:0] == {SENSOR_MANAGER_ADR, CONTROL_WAIT_INSTR_ADDRESS})                                           & ~CtrlMailBoxJumpInstCycle);


// Determine when to start the Sensor Manager Statemachine
//
// Note: Use double-rank synchronization to resolve meta-stability issues.
//
//       Simulation shows these signals toggle from TLC.v clock domain to
//       the Sensor Manager's.
//
assign StartIn_stb = (StartFromFFE_5ff ^ StartFromFFE_6ff)
                   | (StartFromTLC_1ff ^ StartFromTLC_2ff);


// Define the Sensor Manager Control Registers
//
always @(posedge ClockIn or posedge ResetIn) 
begin
    if (ResetIn)
    begin
		wb_ack_sm             <= 1'b0;

        StartFromFFE_1ff      <= 1'b0;
        StartFromFFE_2ff      <= 1'b0;
		StartFromFFE_3ff      <= 1'b0;
        StartFromFFE_4ff      <= 1'b0;
		StartFromFFE_5ff      <= 1'b0;
        StartFromFFE_6ff      <= 1'b0;

        StartFromTLC_1ff      <= 1'b0;
        StartFromTLC_2ff      <= 1'b0;

		RunSM                 <= 1'b0;

		CtrlReceiveAddressReg <= 10'h0;
		CtrlRunTimeAddressReg <= 10'h0;

        CtrlRunTimeAddressSM_1ff <=  1'b0;
        CtrlRunTimeAddressSM_2ff <=  1'b0;

        TimeStamp_Delta_sensor_0 <=  8'h0;
        TimeStamp_Delta_sensor_1 <=  8'h0;
        TimeStamp_Delta_sensor_2 <=  8'h0;
        TimeStamp_Delta_sensor_3 <=  8'h0;

        SensorInterrupt_0_1ff    <=  4'h0;
        SensorInterrupt_0_2ff    <=  4'h0;
        SensorInterrupt_0_3ff    <=  4'h0;

        SensorInterrupt_1_1ff    <=  4'h0;
        SensorInterrupt_1_2ff    <=  4'h0;
        SensorInterrupt_1_3ff    <=  4'h0;

        SensorInterrupt_2_1ff    <=  4'h0;
        SensorInterrupt_2_2ff    <=  4'h0;
        SensorInterrupt_2_3ff    <=  4'h0;

        SensorInterrupt_3_1ff    <=  4'h0;
        SensorInterrupt_3_2ff    <=  4'h0;
        SensorInterrupt_3_3ff    <=  4'h0;

        SensorInterrupt_0_o      <=  1'b0;
        SensorInterrupt_1_o      <=  1'b0;
        SensorInterrupt_2_o      <=  1'b0;
        SensorInterrupt_3_o      <=  1'b0;

        SensorInterrupt_event_mask_0 <= 1'b0;
        SensorInterrupt_event_mask_1 <= 1'b0;
        SensorInterrupt_event_mask_2 <= 1'b0;
        SensorInterrupt_event_mask_3 <= 1'b0;

        TimeStamp_Delta_capt     <= 16'h0;
        TimeStamp_Delta_readback <= 16'h0;

        TimeStamp_Delta_Tog_1ff  <=  1'b0;
        TimeStamp_Delta_Tog_2ff  <=  1'b0;
        TimeStamp_Delta_Tog_3ff  <=  1'b0;

        CtrlMailBoxSegmentCtr       <=  6'h0;
        CtrlMailBoxSegmentCtr_ce    <=  1'b0;

        CtrlMailBoxTablePtr         <= MAILBOX_SM_ADDRESS;
        CtrlMailBoxJumpInstPtr      <= 10'h0;

        CtrlMailBoxJumpInstCycle    <=  1'b0;
        CtrlMailBoxJumpInstCycle_ce <=  1'b0;

        control_wait_instr_cntr     <= 13'h0;
        control_wait_instr_cntr_tc  <=  1'b0;
        control_wait_instr_busy     <=  1'b0;
    end
    else 
    begin  
		wb_ack_sm             <= ((MemReadDataIn[7:3] == SENSOR_MANAGER_ADR )
		                       |  (MemReadDataIn[7:3] == TIMESTAMP_DELTA_ADR)
		                       |  (CtrlMailBoxJumpInstCycle))                 & wb_cyc & wb_stb & ~wb_ack_sm;

		// Double-rank synchronization between clock domains to avoid meta-state issues
		//

        StartFromFFE_1ff      <= StartFromFFE;
        StartFromFFE_2ff      <= StartFromFFE_1ff;
		StartFromFFE_3ff      <= StartFromFFE_2ff;
        StartFromFFE_4ff      <= StartFromFFE_3ff;
		StartFromFFE_5ff      <= StartFromFFE_4ff;
        StartFromFFE_6ff      <= StartFromFFE_5ff;

        StartFromTLC_1ff      <= StartFromTLC;
        StartFromTLC_2ff      <= StartFromTLC_1ff;

        CtrlRunTimeAddressSM_1ff <= CtrlRunTimeAddressSM;
        CtrlRunTimeAddressSM_2ff <= CtrlRunTimeAddressSM_1ff;

		// Define the Sensor Manager Control Register
        //
		// Note: A write of "0" to bit "0" of Sensor Manager Register "0" stops execution.
		//
		//       The remaining bits of the "control" register can be used for other purposes.
		//
		if (StartIn_stb)
            RunSM             <= 1'b1;
        else if (control_sensor_manager_reg_dcd)
            RunSM             <= MemReadDataIn[8];

        // Define the Sensor Manager Receive Address Register
        //
        if (control_receive_reg_dcd)
			CtrlReceiveAddressReg <= MemReadDataIn[17:8];

        // Define the Sensor Manager Run-time Address Register
        //
        if (control_runtime_reg_dcd)
		    CtrlRunTimeAddressReg <= MemReadDataIn[17:8];
	    else if ( CtrlRunTimeAddressSM_1ff ^ CtrlRunTimeAddressSM_2ff)
		    CtrlRunTimeAddressReg <= CtrlRunTimeAddressOut;


		// Synchronize the interrupt inputs to avoid meta-state issues
		//
        SensorInterrupt_0_1ff    <=  SensorInterrupt_0_i;
        SensorInterrupt_0_2ff    <=  SensorInterrupt_0_1ff;
        SensorInterrupt_0_3ff    <=  SensorInterrupt_0_2ff;

        SensorInterrupt_1_1ff    <=  SensorInterrupt_1_i;
        SensorInterrupt_1_2ff    <=  SensorInterrupt_1_1ff;
        SensorInterrupt_1_3ff    <=  SensorInterrupt_1_2ff;

        SensorInterrupt_2_1ff    <=  SensorInterrupt_2_i;
        SensorInterrupt_2_2ff    <=  SensorInterrupt_2_1ff;
        SensorInterrupt_2_3ff    <=  SensorInterrupt_2_2ff;

        SensorInterrupt_3_1ff    <=  SensorInterrupt_3_i;
        SensorInterrupt_3_2ff    <=  SensorInterrupt_3_1ff;
        SensorInterrupt_3_3ff    <=  SensorInterrupt_3_2ff;

        TimeStamp_Delta_Tog_1ff  <=  TimeStamp_Delta_Tog_i;
        TimeStamp_Delta_Tog_2ff  <=  TimeStamp_Delta_Tog_1ff;
        TimeStamp_Delta_Tog_3ff  <=  TimeStamp_Delta_Tog_2ff;


		// Capture the external TimeStamp from the Communication Manager.
		//
		// Note: The Communication Manager uses the 32KHz clock for the
		//       TimeStamp function. In the current application, this is not
		//       the same clock used for the Sensor Manager. However, the
		//       Sensor Manager's clock is currently significantly faster than
		//       the 32KHz clock and can capture the TimeStamp value reliably
		//       when is receives the TimeStamp toggle signal from the
		//       Communication Manager.
		//
		//       This scheme may need to be revisted if the clock assignments
		//       change on future designs.
		//
		if (TimeStamp_Delta_Tog_2ff ^ TimeStamp_Delta_Tog_3ff)
            TimeStamp_Delta_capt     <= TimeStamp_Delta_i;

	    // Capture the TimeStamp Value for a "generic" TimeStamp write to SM
		// Memory.
		//
		// Note: The entire TimeStamp value is captured when a write of the
		//       LSB value to SM Memory is triggered. This allows for the
		//       writting of the MSB bits without the danger of the TimeStamp
		//       value changing between writes of each TimeStamp byte to 
		//       SM Memory.
		//
        if (TimeStamp_Delta_lsb_reg_we_dcd)
            TimeStamp_Delta_readback <= TimeStamp_Delta_capt;


		// Capture the time stamp delta when an interrupt is detected.
		//
		// Note: See below for the definition of the bit operations.
		//
        if (SensorInterrupt_0_2ff && (!SensorInterrupt_0_3ff))
            TimeStamp_Delta_sensor_0 <=  TimeStamp_Delta_capt;


        if (SensorInterrupt_1_2ff && (!SensorInterrupt_1_3ff))
            TimeStamp_Delta_sensor_1 <=  TimeStamp_Delta_capt;


        if (SensorInterrupt_2_2ff && (!SensorInterrupt_2_3ff))
            TimeStamp_Delta_sensor_2 <=  TimeStamp_Delta_capt;


        if (SensorInterrupt_3_2ff && (!SensorInterrupt_3_3ff))
            TimeStamp_Delta_sensor_3 <=  TimeStamp_Delta_capt;

	    // Set the Interrupt Status Mask bits
		//
		// Note: These bits are used "ANDed" with the write signal to clear
		//       individual status bits. 
		//
		//       The alternate way is to write the interrupt status once
		//       at the end of a series of SM code segments. However, there
		//       may be a significant amount of time between TimeStamp value
		//       capture and a single status being written to memory. This 
		//       can allow the interrupt status to change after the TimeStamp 
		//       is written to memory. This could result in the assumption 
		//       of a good TimeStamp when, in fact, the TimeStamp is not 
		//       valid.
		//
        if (SensorInterrupt_event_mask_we_dcd)
		begin
            SensorInterrupt_event_mask_0 <= MemReadDataIn[8];
            SensorInterrupt_event_mask_1 <= MemReadDataIn[9];
            SensorInterrupt_event_mask_2 <= MemReadDataIn[10];
            SensorInterrupt_event_mask_3 <= MemReadDataIn[11];
		end

	    // Set the interrupt event bit for each sensor when an interrupt is
		// detected.
		//
		// Note: Without this "interrupt event bit" is may not be possible to
		//       know for certain if an interrupt happened. For example,
		//       a value of "0" may be correct given the right
		//       sampling period.
		//
		//       These status bits assume a positive (i.e. low-to-high)
		//       interrupt assertion.
		//
		//       All interrupts are cleared when this register is read.
		//
        if (SensorInterrupt_event_reg_we_dcd && SensorInterrupt_event_mask_0)
            SensorInterrupt_0_o       <=  1'h0;
        else if (SensorInterrupt_0_2ff && (!SensorInterrupt_0_3ff))
            SensorInterrupt_0_o       <=  1'h1;


        if (SensorInterrupt_event_reg_we_dcd && SensorInterrupt_event_mask_1)
            SensorInterrupt_1_o       <=  1'h0;
        else if (SensorInterrupt_1_2ff && (!SensorInterrupt_1_3ff))
            SensorInterrupt_1_o       <=  1'h1;


        if (SensorInterrupt_event_reg_we_dcd && SensorInterrupt_event_mask_2)
            SensorInterrupt_2_o       <=  1'h0;
        else if (SensorInterrupt_2_2ff && (!SensorInterrupt_2_3ff))
            SensorInterrupt_2_o       <=  1'h1;


        if (SensorInterrupt_event_reg_we_dcd && SensorInterrupt_event_mask_3)
            SensorInterrupt_3_o       <=  1'h0;
        else if (SensorInterrupt_3_2ff && (!SensorInterrupt_3_3ff))
            SensorInterrupt_3_o       <=  1'h1;


		//  Mail Box Bit Counter
		//
		//  Note: Reset Bit Counter between SM Sessions.
		//
		//        This counter selects the Mail Box bits corresponding to each
		//        SM code segment.
		//
        if (!BusySM)
            CtrlMailBoxSegmentCtr    <=  6'h0;
        else if (CtrlMailBoxSegmentCtr_ce)
            CtrlMailBoxSegmentCtr    <=  CtrlMailBoxSegmentCtr + 1'b1;

        CtrlMailBoxSegmentCtr_ce     <=  wb_cyc & wb_stb & wb_we & ~wb_ack_sm & CtrlMailBoxJumpInstCycle;

		// Mail Box Table Address Pointer 
		//
		// Note: This is the location in SM Memory where the Mail Box is
		//       located. Typically, the mail box will be in the last four
		//       18-bit words in SM Memory.
		//
		//       This value can be dynamically changed by instructions in SM
		//       memory.
		//
        if (control_mailbox_tbl_ptr_dcd)
            CtrlMailBoxTablePtr      <=  MemReadDataIn[17:10];

	    // Mail Box Jump Address
		//
		// Note: This address must be temporarily storged while the Mail Box
		//       bits are being read from SM Memory.
		//
		//       Based on the Mail Box bit for the current code segment, this
		//       jump address may or may not be used for a jump.
		//
        if (control_mailbox_jump_inst_ptr_dcd)
            CtrlMailBoxJumpInstPtr   <=  MemReadDataIn[17:8];

	    // Mail Box Jump Decode Cycle Flag
		//
		// Note: This flags that the current SM write cycle is a Mail Box Jump
		//       decode operation. 
		//
		//       The data from SM Memory consist of Mail Box bits and should
		//       not be decoded as a SM "write" instruction would.
		//
		//       The decode consists of selecting the correct bit from the 
		//       Mail Box for the current SM code segment. Based on the state 
		//       of this bit (i.e. 0 - No Jump; 1 - Jump), the SM instruction 
		//       pointer will either proceed with the next instruction address 
		//       or jump to a new code segment.
		//
        if (control_mailbox_jump_inst_ptr_dcd)
            CtrlMailBoxJumpInstCycle <= 1'b1;
        else if (CtrlMailBoxJumpInstCycle_ce)
            CtrlMailBoxJumpInstCycle <= 1'b0;

        CtrlMailBoxJumpInstCycle_ce  <= wb_cyc & wb_stb & wb_we &  wb_ack_sm & ~control_mailbox_jump_inst_ptr_dcd;


		// Wait Instruction Register
		//
        if (control_wait_instr_reg_dcd || control_wait_instr_busy)
		begin
            control_wait_instr_cntr      <= control_wait_instr_cntr_nxt;
            control_wait_instr_cntr_tc   <= control_wait_instr_cntr_tc_nxt;
		end

        control_wait_instr_busy          <= control_wait_instr_busy_nxt;
    end
end   


// Define the Wait Instruction Busy signal
//
// Note: This busy starts with the first write and ends when the counter is done.
//
//       This is an N-1 counter. Therefore, a value of "0" means an "N" of "1".
//       Therefore, there should be one cycle of busy even with a value of "0".
//
assign control_wait_instr_busy_nxt = (~control_wait_instr_busy &  control_wait_instr_reg_dcd)
                                   | ( control_wait_instr_busy & ~control_wait_instr_cntr_tc);
	

// Define the operation of the Wait Instruction Counter
//
always @(MemReadDataIn           or
         control_wait_instr_busy or
         control_wait_instr_cntr
        )
begin

    case({control_wait_instr_busy, MemReadDataIn[17]})
	2'b00:  // MSB == 0 then count  1-to-1
	begin
        control_wait_instr_cntr_nxt      <= {4'h0, MemReadDataIn[16:8]        };
        control_wait_instr_cntr_tc_nxt   <= (      MemReadDataIn[16:8] == 9'h0);
	end
	2'b01:  // MSB == 1 then count 16-to-1
	begin
        control_wait_instr_cntr_nxt      <= {      MemReadDataIn[16:8],   4'hF}; // Remember: N-1 means that "0" should be one wait period
        control_wait_instr_cntr_tc_nxt   <= 1'b0;
	end
	2'b10:  // Normal Count
	begin
        control_wait_instr_cntr_nxt      <=  control_wait_instr_cntr -  13'h1;
        control_wait_instr_cntr_tc_nxt   <= (control_wait_instr_cntr == 13'h1); 
	end
	2'b11:  // Normal Count - The value was shift << 4 so it is already 16x larger at loading time
	begin
        control_wait_instr_cntr_nxt      <=  control_wait_instr_cntr -  13'h1;
        control_wait_instr_cntr_tc_nxt   <= (control_wait_instr_cntr == 13'h1); 
	end
	endcase
end


// Use the "run" bit to signal when the statemachine is "busy" in addition to
// the statemachine busy bit.
//
assign BusyOut            = RunSM | BusySM;
	

// Define the Sensor Manager Memory's read address
//
// Note: StateMachine is allowed to read all of SensorManager Memory
//
//       The Sensor Manager Memory's "read" port is 10-bits (i.e. [9:0])
//
//       Select the Mail Box Address pointer during Mail Box Jump operations.
//       The location pointed to contains Mail Box Jump enable bits AND NOT 
//       SM instructions.
//
assign MemReadAddressOut  = CtrlMailBoxJumpInstCycle ? {CtrlMailBoxTablePtr, CtrlMailBoxSegmentCtr[5:4]} 
                                                     :  StateMachineCtrlMemAddr ;


// Limit the register write function to the upper half of the Sensor Manager's Memory space
//
// Note: The Sensor Manager Memory's "write" port address is 10-bits (i.e. [9:0])
//
assign MemWriteAddressOut = CtrlReceiveAddressReg;


// Define the Data to be written to Sensor Memory
//
// Note: The I2C Master IP only outputs byte wide values
//
//       For the current design, the following are read back:
//           - I2C Master IP is read back
//           - TimeStamp registers for four sensors
//           - TimeSTamp related interrupt event register
//
//      Only the I2C Master IP was supported in previous designs
//
always @(MemReadDataIn            or
		 MemWriteDataOut          or
         i2c_masterDataToMem      or
         TimeStamp_Delta_sensor_0 or
         TimeStamp_Delta_sensor_1 or
         TimeStamp_Delta_sensor_2 or
         TimeStamp_Delta_sensor_3 or
		 TimeStamp_Delta_readback or
         SensorInterrupt_0_o      or
         SensorInterrupt_1_o      or
         SensorInterrupt_2_o      or
         SensorInterrupt_3_o    
        )
begin
	case(MemReadDataIn[7:3])
    I2C_MASTER_ADR:                    MemWriteDataOut <= {1'b0, i2c_masterDataToMem};
	TIMESTAMP_DELTA_ADR:
	begin
		case(MemReadDataIn[2:0])
        TIMESTAMP_DELTA_SENSOR_0     : MemWriteDataOut <= {1'b0, TimeStamp_Delta_sensor_0};
        TIMESTAMP_DELTA_SENSOR_1     : MemWriteDataOut <= {1'b0, TimeStamp_Delta_sensor_1};
        TIMESTAMP_DELTA_SENSOR_2     : MemWriteDataOut <= {1'b0, TimeStamp_Delta_sensor_2};
        TIMESTAMP_DELTA_SENSOR_3     : MemWriteDataOut <= {1'b0, TimeStamp_Delta_sensor_3};
        TIMESTAMP_DELTA_GENERIC_LSB  : MemWriteDataOut <= {1'b0, TimeStamp_Delta_readback[7:0]};
        TIMESTAMP_DELTA_GENERIC_MSB  : MemWriteDataOut <= {1'b0, TimeStamp_Delta_readback[15:8]};
        TIMESTAMP_DELTA_INT_EVENT    : MemWriteDataOut <= {4'h0, SensorInterrupt_3_o,
				                                                 SensorInterrupt_2_o,
															     SensorInterrupt_1_o,
															     SensorInterrupt_0_o};
		default:                       MemWriteDataOut <= {1'b0, i2c_masterDataToMem};
        endcase
	end
	default:                           MemWriteDataOut <= {1'b0, i2c_masterDataToMem};
	endcase
end


// Define the Sensor Manager Memory's clock
//
// Note: This is currently a flow through but this may change in future designs.
//
assign MemClockOut        = ClockIn;


// Combine all Wishbone bus acknowledges
//
// Note: Only one acknowledge should happen at a time.
//
assign wb_ack             = wb_ack_sm | I2C_wb_ack_i;


// Multiplex the address to the I2C Master IP when performing an I2C read
//
// Note: The address must be switched from the I2C "Control" Register to the I2C "Transmit/Receive" data address.
//
//       This only affects the I2C Master IP and does not affect any other device on the Wishbone bus.
//
assign I2C_wb_adr_o       = ((MemReadDataIn[7:0] == {I2C_MASTER_ADR, I2C_MASTER_CR}) &  MemReadDataIn[13] & (~wb_we))   
                                                                   ? I2C_MASTER_TXR  :  MemReadDataIn[2:0];


//------Instantiate Modules----------------
//

// Instantiate the Sensor Manager Statemachine
//
StateMachine StateMachine_inst      (

        .CLK_IN                     ( ClockIn                               ),
        .RESET_IN                   ( ResetIn                               ),

		.RUNTIME_ADDRESS            ( CtrlRunTimeAddressReg                 ),
        .CONTROL_JUMP_REG_DCD       ( control_jump_reg_dcd                  ),
        .SAVE_REG_2_MEM             ( save_reg_2_mem                        ),

        .MAILBOX_JUMP_INST_CYCLE    (CtrlMailBoxJumpInstCycle               ),
        .MAILBOX_JUMP_INST_PTR      (CtrlMailBoxJumpInstPtr                 ),
        .MAILBOX_SEGMENT_CTR        (CtrlMailBoxSegmentCtr[3:0]             ),

        .WB_ACK_I                   ( wb_ack                                ),
        .WB_BUSY_I                  ( I2C_tip_i | control_wait_instr_busy   ),
		.WB_BUSY_POLL_I             ( wb_busy_poll                          ),

        .WB_WE_O                    ( wb_we                                 ),
        .WB_STB_O                   ( wb_stb                                ),
        .WB_CYC_O                   ( wb_cyc                                ),

        .SM_CNTL_REG_RUN            ( RunSM                                 ),
        .SM_READ_DATA               ( MemReadDataIn                         ), // Data    "Byte" is MemReadDataIn[17:8]

        .SM_INSTR_PTR               ( StateMachineCtrlMemAddr               ),
        .SM_READ_SELECT             ( MemReadEnableOut                      ),

        .SM_WRITE_SELECT            ( MemWriteEnableOut                     ),
        .SM_BUSY                    ( BusySM                                )

         );


// test points
//
assign TP1 = I2C_tip_i;
assign TP2 = BusyOut;
assign TP3 = 0;

// Logic to generate SmClockSelect_o
wire	d_SmClockSelect;

always @(posedge ClockIn or posedge ResetIn) 
begin
    if (ResetIn)
    begin
		s1_BusyOut  <= 1'b0;
		s2_BusyOut	<= 1'b0;
		SmClockSelect_o <= 1'b0;
	end
	else
	begin
		s1_BusyOut  <= BusyOut;
		s2_BusyOut	<= s1_BusyOut;
		SmClockSelect_o <= d_SmClockSelect;
	end
end

assign d_SmClockSelect = SmClockSelect_o ? ((!s1_BusyOut && s2_BusyOut) ? 1'b0 : 1'b1) : ((StartFromFFE_1ff ^ StartFromFFE_2ff) ? 1'b1: 1'b0);	
	
endmodule
