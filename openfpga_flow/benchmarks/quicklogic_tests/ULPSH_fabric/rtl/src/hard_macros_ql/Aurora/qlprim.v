/***************************************************
 Vendor        : QuickLogic Corp.
 File Name     : qlprim.v
 Description   : Behavioral model of Logic cell
 Revisions     : Added specify blocks for Logic cell, GPIO and RAM
 Author        : Kishor
*****************************************************/

// logic cell ------------------------------------------------------------------
`timescale 1ns/10ps

////////////////   Implementation of the LUT   //////////////////////
module LUT (fragBitInfo, 
            I0, 
            I1, 
            I2, 
            I3, 
            LUTOutput,
			CarryOut 
			);

input [15:0] fragBitInfo; 
input I0, I1, I2, I3;
output LUTOutput; 
output CarryOut;

wire stage0_op0, stage0_op1, stage0_op2, stage0_op3, stage0_op4, stage0_op5, stage0_op6, stage0_op7; 
wire stage1_op0, stage1_op1, stage1_op2, stage1_op3;
wire stage2_op0, stage2_op1;

//wire carry_mux_op;

//assign carry_mux_op = CIS ? I2 : Cin;

assign stage0_op0 = I0 ? fragBitInfo[1] : fragBitInfo[0];
assign stage0_op1 = I0 ? fragBitInfo[3] : fragBitInfo[2];
assign stage0_op2 = I0 ? fragBitInfo[5] : fragBitInfo[4];
assign stage0_op3 = I0 ? fragBitInfo[7] : fragBitInfo[6];
assign stage0_op4 = I0 ? fragBitInfo[9] : fragBitInfo[8];
assign stage0_op5 = I0 ? fragBitInfo[11] : fragBitInfo[10];
assign stage0_op6 = I0 ? fragBitInfo[13] : fragBitInfo[12];
assign stage0_op7 = I0 ? fragBitInfo[15] : fragBitInfo[14];


assign stage1_op0 = I1 ? stage0_op1 : stage0_op0; 
assign stage1_op1 = I1 ? stage0_op3 : stage0_op2;
assign stage1_op2 = I1 ? stage0_op5 : stage0_op4;
assign stage1_op3 = I1 ? stage0_op7 : stage0_op6;


assign stage2_op0 = I2 ? stage1_op1 : stage1_op0;
assign stage2_op1 = I2 ? stage1_op3 : stage1_op2;


assign LUTOutput = I3 ? stage2_op1 : stage2_op0;
assign CarryOut = stage2_op1;

endmodule


module ONE_LOGIC_CELL (
		tFragBitInfo,
		bFragBitInfo,
        TI0,
        TI1,
        TI2, 
        TI3,
        BI0, 
        BI1,
        BI2,
        BI3,
        TBS, 
        QDI,
        CDS,
        QEN,
        QST,
        QRT,
        QCK,
        QCKS,
        CZ,
        QZ,
        BZ,
        BCO,
		TCO);

input [15:0] tFragBitInfo;
input [15:0] bFragBitInfo;
input TI0, TI1, TI2, TI3, BI0, BI1, BI2, BI3, TBS, QDI, CDS, QEN, QST, QRT, QCK, QCKS;

output  CZ, QZ, BZ, BCO, TCO;


wire tFragLUTOutput, bFragLUTOutput;

wire mux_tbs_op, mux_cds_op, mux_bqs_op; 

reg QZ_reg;


LUT tLUT (.fragBitInfo(tFragBitInfo), .I0(TI0), .I1(TI1), .I2(TI2), .I3(TI3), .LUTOutput(tFragLUTOutput), .CarryOut(TCO));
LUT bLUT (.fragBitInfo(bFragBitInfo), .I0(BI0), .I1(BI1), .I2(BI2), .I3(BI3), .LUTOutput(bFragLUTOutput), .CarryOut(BCO));


assign mux_tbs_op = TBS ? bFragLUTOutput : tFragLUTOutput;
assign mux_cds_op = CDS ? QDI : mux_tbs_op;


/* synopsys translate off */
always @ (posedge QCK)   
begin
	if(~QRT && ~QST )
		if(QEN)
        	QZ_reg = mux_cds_op;
end
	
always @(QRT or QST)
begin
	if(QRT)
		 QZ_reg = 1'b0;
	else if (QST)
		 QZ_reg = 1'b1;
end


assign CZ = mux_tbs_op; 
assign BZ = bFragLUTOutput;

assign QZ = QZ_reg;
//assign BQZ = mux_bqs_op;

endmodule 


module LOGIC (
		tFragBitInfo, 
		bFragBitInfo,
        T0I0, 
        T0I1,
        T0I2,
        T0I3,
        B0I0, 
        B0I1,
        B0I2,
        B0I3,
        TB0S, 
        Q0DI,
        CD0S,
        Q0EN,
        T1I0,
        T1I1,
        T1I2,
        T1I3,
        B1I0, 
        B1I1,
        B1I2,
        B1I3,
        TB1S, 
        Q1DI,
        CD1S,
        Q1EN,
        T2I0,
        T2I1,
        T2I2,
        T2I3,
        B2I0, 
        B2I1,
        B2I2,
        B2I3,
        TB2S, 
        Q2DI,
        CD2S,
        Q2EN,
        T3I0,
        T3I1,
        T3I2,
        T3I3,
        B3I0, 
        B3I1,
        B3I2,
        B3I3,
        TB3S, 
        Q3DI,
        CD3S,
        Q3EN,
        QST, 
        QRT,
        QCK,
        QCKS,
        C0Z,
        Q0Z,
        B0Z,
        C1Z,
        Q1Z,
        B1Z,
        C2Z,
        Q2Z,
        B2Z,
        C3Z,
        Q3Z,
        B3Z,
		T0CO, 
		B0CO, 
        T3CO, 
        B3CO, 
		T1CO, 
		B1CO, 
		T2CO, 
		B2CO);

input [63:0] tFragBitInfo;
input [63:0] bFragBitInfo;
input  T0I0; 
input  T0I1;
input  T0I2;
input  T0I3;
input  B0I0; 
input  B0I1;
input  B0I2;
input  B0I3;
input  TB0S; 
input  Q0DI;
input  CD0S;
input  Q0EN;
input  T1I0;
input  T1I1;
input  T1I2;
input  T1I3;
input  B1I0; 
input  B1I1;
input  B1I2;
input  B1I3;
input  TB1S; 
input  Q1DI;
input  CD1S;
input  Q1EN;
input  T2I0;
input  T2I1;
input  T2I2;
input  T2I3;
input  B2I0; 
input  B2I1;
input  B2I2;
input  B2I3;
input  TB2S; 
input  Q2DI;
input  CD2S;
input  Q2EN;
input  T3I0;
input  T3I1;
input  T3I2;
input  T3I3;
input  B3I0; 
input  B3I1;
input  B3I2;
input  B3I3;
input  TB3S; 
input  Q3DI;
input  CD3S;
input  Q3EN;
input  QST;
input  QRT;
input  QCK;
input  QCKS;
output  C0Z;
output  Q0Z;
output  B0Z;
output  C1Z;
output  Q1Z;
output  B1Z;
output  C2Z;
output  Q2Z;
output  B2Z;
output  C3Z;
output  Q3Z;
output  B3Z;
output  T3CO; 
output  B3CO;
output  T0CO; 
output  B0CO; 
output  T1CO; 
output  B1CO; 
output  T2CO; 	
output  B2CO;


ONE_LOGIC_CELL logic0(
		.tFragBitInfo (tFragBitInfo[15:0]),
		.bFragBitInfo (bFragBitInfo[15:0]),
        .TI0 (T0I0),
        .TI1 (T0I1),
        .TI2 (T0I2), 
        .TI3 (T0I3),
        .BI0 (B0I0), 
        .BI1 (B0I1),
        .BI2 (B0I2),
        .BI3 (B0I3),
        .TBS (TB0S), 
        .QDI (Q0DI),
        .CDS (CD0S),
        .QEN (Q0EN),
        .QST (QST),
        .QRT (QRT),
        .QCK (QCK),
        .QCKS (QCKS),
        .CZ (C0Z),
        .QZ (Q0Z),
        .BZ (B0Z),
        .TCO (T0CO), 
        .BCO (B0CO)); 

ONE_LOGIC_CELL logic1(
		.tFragBitInfo (tFragBitInfo[31:16]),
		.bFragBitInfo (bFragBitInfo[31:16]),
        .TI0 (T1I0),
        .TI1 (T1I1),
        .TI2 (T1I2), 
        .TI3 (T1I3),
        .BI0 (B1I0), 
        .BI1 (B1I1),
        .BI2 (B1I2),
        .BI3 (B1I3),
        .TBS (TB1S), 
        .QDI (Q1DI),
        .CDS (CD1S),
        .QEN (Q1EN),
        .QST (QST),
        .QRT (QRT),
        .QCK (QCK),
        .QCKS (QCKS),
        .CZ (C1Z),
        .QZ (Q1Z),
        .BZ (B1Z),
        .TCO (T1CO), 
        .BCO (B1CO)); 

ONE_LOGIC_CELL logic2(
		.tFragBitInfo (tFragBitInfo[47:32]),
		.bFragBitInfo (bFragBitInfo[47:32]),
        .TI0 (T2I0),
        .TI1 (T2I1),
        .TI2 (T2I2), 
        .TI3 (T2I3),
        .BI0 (B2I0), 
        .BI1 (B2I1),
        .BI2 (B2I2),
        .BI3 (B2I3),
        .TBS (TB2S), 
        .QDI (Q2DI),
        .CDS (CD2S),
        .QEN (Q2EN),
        .QST (QST),
        .QRT (QRT),
        .QCK (QCK),
        .QCKS (QCKS),
        .CZ (C2Z),
        .QZ (Q2Z),
        .BZ (B2Z),
        .TCO (T2CO),
        .BCO (B2CO));

ONE_LOGIC_CELL logic3(
		.tFragBitInfo (tFragBitInfo[63:48]),
		.bFragBitInfo (bFragBitInfo[63:48]),
        .TI0 (T3I0),
        .TI1 (T3I1),
        .TI2 (T3I2), 
        .TI3 (T3I3),
        .BI0 (B3I0), 
        .BI1 (B3I1),
        .BI2 (B3I2),
        .BI3 (B3I3),
        .TBS (TB3S), 
        .QDI (Q3DI),
        .CDS (CD3S),
        .QEN (Q3EN),
        .QST (QST),
        .QRT (QRT),
        .QCK (QCK),
        .QCKS (QCKS),
        .CZ (C3Z),
        .QZ (Q3Z),
        .BZ (B3Z),
        .TCO (T3CO),
        .BCO (B3CO)); 
specify

	(QCK => Q0Z)  = (0,0);
	(QCK => Q1Z)  = (0,0);
	(QCK => Q2Z)  = (0,0);
	(QCK => Q3Z)  = (0,0);

/*
	if ((B0CIS == 1'b0) && (T0CIS == 1'b0))
		(T0CI => B0Z)  = (0,0);
	if (B0CIS == 1'b0)
		(T0I0 => B0Z)  = (0,0);
	if (B0CIS == 1'b0)
		(T0I1 => B0Z)  = (0,0);
	if ((B0CIS == 1'b1) && (T0CIS == 1'b0))
		(T0I2 => B0Z)  = (0,0);
	if (B0CIS == 1'b0)
		(T0I3 => B0Z)  = (0,0);

	if ((B1CIS == 1'b0) && (T1CIS == 1'b0))
		(B0I0 => B1Z)  = (0,0);
	if ((B1CIS == 1'b0) && (T1CIS == 1'b0))
		(B0I1 => B1Z)  = (0,0);
	if ((B0CIS == 1'b1) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(B0I2 => B1Z)  = (0,0);
	if ((B1CIS == 1'b0) && (T1CIS == 1'b0))
		(B0I3 => B1Z)  = (0,0);

	if ((T0CIS == 1'b0) && (B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(T0CI => B1Z)  = (0,0);
	if ((B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(T0I0 => B1Z)  = (0,0);
	if ((B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(T0I1 => B1Z)  = (0,0);
	if ((T0CIS == 1'b1) && (B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(T0I2 => B1Z)  = (0,0);
	if ((B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(T0I3 => B1Z)  = (0,0);

	if ((B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(B0I0 => B2Z)  = (0,0);
	if ((B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(B0I1 => B2Z)  = (0,0);
	if ((B0CIS == 1'b1) && (B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(B0I2 => B2Z)  = (0,0);
	if ((B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(B0I3 => B2Z)  = (0,0);


	if ((T0CIS == 1'b0) && (B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(T0CI => B2Z)  = (0,0);
	if ((B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(T0I0 => B2Z)  = (0,0);
	if ((B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(T0I1 => B2Z)  = (0,0);
	if ((T0CIS == 1'b1) && (B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(T0I2 => B2Z)  = (0,0);
	if ((B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(T0I3 => B2Z)  = (0,0);

	if ((B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(B0I0 => B3Z)  = (0,0);
	if ((B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(B0I1 => B3Z)  = (0,0);
	if ((B1CIS == 1'b1) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(B0I2 => B3Z)  = (0,0);
	if ((B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(B0I3 => B3Z)  = (0,0);

	if ((T0CIS == 1'b0) && (B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(T0CI => B3Z)  = (0,0);
	if ((B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(T0I0 => B3Z)  = (0,0);
	if ((B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(T0I1 => B3Z)  = (0,0);
	if ((T0CIS == 1'b1) && (B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(T0I2 => B3Z)  = (0,0);
	if ((B0CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(T0I3 => B3Z)  = (0,0);

	if ((B1CIS == 1'b0)) 
		(T1I0 => B1Z)  = (0,0);
	if ((B1CIS == 1'b0)) 
		(T1I1 => B1Z)  = (0,0);
	if ( (T1CIS == 1'b1) && (B1CIS == 1'b0)) 
		(T1I2 => B1Z)  = (0,0);
	if ((B1CIS == 1'b0)) 
		(T1I3 => B1Z)  = (0,0);

	if ((B1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0)) 
		(T1I0 => B2Z)  = (0,0);
	if ((B1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0)) 
		(T1I1 => B2Z)  = (0,0);
	if ((T1CIS == 1'b1) && (B1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0)) 
		(T1I2 => B2Z)  = (0,0);
	if ((B1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0)) 
		(T1I3 => B2Z)  = (0,0);

	if (B2CIS == 1'b0)
		(T2I0 => B2Z)  = (0,0);
	if (B2CIS == 1'b0)
		(T2I1 => B2Z)  = (0,0);
	if ((T2CIS == 1'b1) && (B2CIS == 1'b0))
		(T2I2 => B2Z)  = (0,0);
	if (B2CIS == 1'b0)
		(T2I3 => B2Z)  = (0,0);


	if ((B1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0)) 
		(T1I0 => B3Z)  = (0,0);
	if ((B1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0)) 
		(T1I1 => B3Z)  = (0,0);
	if ((T1CIS == 1'b1) && (B1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0)) 
		(T1I2 => B3Z)  = (0,0);
	if ((B1CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0)) 
		(T1I3 => B3Z)  = (0,0);

	if (B2CIS == 1'b0) 
		(T2I0 => B3Z)  = (0,0);
	if (B2CIS == 1'b0) 
		(T2I1 => B3Z)  = (0,0);
	if ((T2CIS == 1'b1) && (B2CIS == 1'b0))
		(T2I2 => B3Z)  = (0,0);
	if (B2CIS == 1'b0) 
		(T2I3 => B3Z)  = (0,0);

	if (B3CIS == 1'b0) 
		(T3I0 => B3Z)  = (0,0);
	if (B3CIS == 1'b0) 
		(T3I1 => B3Z)  = (0,0);
	if  ((T3CIS == 1'b1)&& (B3CIS == 1'b0))
		(T3I2 => B3Z)  = (0,0);
	if (B3CIS == 1'b0) 
		(T3I3 => B3Z)  = (0,0);


	if ((T2CIS == 1'b0) && (B2CIS == 1'b0))
		(B1I0 => B2Z)  = (0,0);
	if ((T2CIS == 1'b0) && (B2CIS == 1'b0))
		(B1I1 => B2Z)  = (0,0);
	if ( (B1CIS == 1'b1) && (T2CIS == 1'b0) && (B2CIS == 1'b0))
		(B1I2 => B2Z)  = (0,0);
	if ((T2CIS == 1'b0) && (B2CIS == 1'b0))
		(B1I3 => B2Z)  = (0,0);

	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0))
		(B1I0 => B3Z)  = (0,0);
	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0))
		(B1I1 => B3Z)  = (0,0);
	if ( (B1CIS == 1'b1) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0))
		(B1I2 => B3Z)  = (0,0);
	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0))
		(B1I3 => B3Z)  = (0,0);

	if ((T3CIS == 1'b0) && (B3CIS == 1'b0))
		(B2I0 => B3Z)  = (0,0);
	if ((T3CIS == 1'b0) && (B3CIS == 1'b0))
		(B2I1 => B3Z)  = (0,0);
	if ((B2CIS == 1'b1) && (T3CIS == 1'b0) && (B3CIS == 1'b0))
		(B2I2 => B3Z)  = (0,0);
	if ((T3CIS == 1'b0) && (B3CIS == 1'b0))
		(B2I3 => B3Z)  = (0,0);


	if ((T0CIS == 1'b0) && (B0CIS == 1'b0) && (TB1S == 1'b0))
		(T0CI => C1Z)  = (0,0);
	if ((B0CIS == 1'b0) && (TB1S == 1'b0))
		(T0I0 => C1Z)  = (0,0);
	if ((B0CIS == 1'b0) && (TB1S == 1'b0))
		(T0I1 => C1Z)  = (0,0);
	if ((T0CIS == 1'b1) && (B0CIS == 1'b0) && (TB1S == 1'b0))
		(T0I2 => C1Z)  = (0,0);
	if ((B0CIS == 1'b0) && (TB1S == 1'b0))
		(T0I3 => C1Z)  = (0,0);

	if ((T0CIS == 1'b0) && (B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (TB2S == 1'b0))
		(T0CI => C2Z)  = (0,0);
	if ((B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (TB2S == 1'b0))
		(T0I0 => C2Z)  = (0,0);
	if ((B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (TB2S == 1'b0))
		(T0I1 => C2Z)  = (0,0);
	if ((T0CIS == 1'b1) && (B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (TB2S == 1'b0))
		(T0I2 => C2Z)  = (0,0);
	if ((B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (TB2S == 1'b0))
		(T0I3 => C2Z)  = (0,0);

	if ((B1CIS == 1'b0) &&  (T2CIS == 1'b0) && (TB2S == 1'b0))
		(T1I0 => C2Z)  = (0,0);
	if ((B1CIS == 1'b0) &&  (T2CIS == 1'b0) && (TB2S == 1'b0))
		(T1I1 => C2Z)  = (0,0);
	if ((T1CIS == 1'b1) && (B1CIS == 1'b0) &&  (T2CIS == 1'b0) && (TB2S == 1'b0))
		(T1I2 => C2Z)  = (0,0);
	if ((B1CIS == 1'b0) &&  (T2CIS == 1'b0) && (TB2S == 1'b0))
		(T1I3 => C2Z)  = (0,0);

	if ((T0CIS == 1'b0) && (B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T0CI => C3Z)  = (0,0);
	if ((B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T0I0 => C3Z)  = (0,0);
	if ((B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T0I1 => C3Z)  = (0,0);
	if ((T0CIS == 1'b1) && (B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T0I2 => C3Z)  = (0,0);
	if ((B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T0I3 => C3Z)  = (0,0);

	if ((B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T1I0 => C3Z)  = (0,0);
	if ((B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T1I1 => C3Z)  = (0,0);
	if ((T1CIS == 1'b1) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T1I2 => C3Z)  = (0,0);
	if ((B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T1I3 => C3Z)  = (0,0);

	if ((B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T2I0 => C3Z)  = (0,0);
	if ((B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T2I1 => C3Z)  = (0,0);
	if ((T2CIS == 1'b1) && (B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T2I2 => C3Z)  = (0,0);
	if ((B2CIS == 1'b0) && (T3CIS == 1'b0)  && (TB2S == 1'b0))
		(T2I3 => C3Z)  = (0,0);

	if ((T1CIS == 1'b0) && (TB1S == 1'b0))
		(B0I0 => C1Z)  = (0,0);
	if ((T1CIS == 1'b0) && (TB1S == 1'b0))
		(B0I1 => C1Z)  = (0,0);
	if ( (B0CIS == 1'b0) && (T1CIS == 1'b0) && (TB1S == 1'b0))
		(B0I2 => C1Z)  = (0,0);
	if ((T1CIS == 1'b0) && (TB1S == 1'b0))
		(B0I3 => C1Z)  = (0,0);

	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (TB1S == 1'b1))
		(B0I0 => C1Z)  = (0,0);
	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (TB1S == 1'b1))
		(B0I1 => C1Z)  = (0,0);
	if ((B0CIS == 1'b1) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (TB1S == 1'b1))
		(B0I2 => C1Z)  = (0,0);
	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (TB1S == 1'b1))
		(B0I3 => C1Z)  = (0,0);


	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (TB2S == 1'b0))
		(B0I0 => C2Z)  = (0,0);
	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (TB2S == 1'b0))
		(B0I1 => C2Z)  = (0,0);
	if ((B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (TB2S == 1'b0))
		(B0I2 => C2Z)  = (0,0);
	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (TB2S == 1'b0))
		(B0I3 => C2Z)  = (0,0);

	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (TB2S == 1'b1))
		(B0I0 => C2Z)  = (0,0);
	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (TB2S == 1'b1))
		(B0I1 => C2Z)  = (0,0);
	if ((B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (TB2S == 1'b1))
		(B0I2 => C2Z)  = (0,0);
	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (TB2S == 1'b1))
		(B0I3 => C2Z)  = (0,0);


	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (TB3S == 1'b0))
		(B0I0 => C3Z)  = (0,0);
	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (TB3S == 1'b0))
		(B0I1 => C3Z)  = (0,0);
	if ((B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (TB3S == 1'b0))
		(B0I2 => C3Z)  = (0,0);
	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (TB3S == 1'b0))
		(B0I3 => C3Z)  = (0,0);



	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B0I0 => C3Z)  = (0,0);
	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B0I1 => C3Z)  = (0,0);
	if ((B0CIS == 1'b0) && (T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B0I2 => C3Z)  = (0,0);
	if ((T1CIS == 1'b0) && (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B0I3 => C3Z)  = (0,0);


	if ((T2CIS == 1'b0) && (TB2S == 1'b0))
		(B1I0 => C2Z)  = (0,0);
	if ((T2CIS == 1'b0) && (TB2S == 1'b0))
		(B1I1 => C2Z)  = (0,0);
	if ((B1CIS == 1'b0) && (T2CIS == 1'b0) && (TB2S == 1'b0))
		(B1I2 => C2Z)  = (0,0);
	if ((T2CIS == 1'b0) && (TB2S == 1'b0))
		(B1I3 => C2Z)  = (0,0);

	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (TB2S == 1'b1))
		(B1I0 => C2Z)  = (0,0);
	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (TB2S == 1'b1))
		(B1I1 => C2Z)  = (0,0);
	if ((B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (TB2S == 1'b1))
		(B1I2 => C2Z)  = (0,0);
	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (TB2S == 1'b1))
		(B1I3 => C2Z)  = (0,0);

	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (TB3S == 1'b0))
		(B1I0 => C3Z)  = (0,0);
	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (TB3S == 1'b0))
		(B1I1 => C3Z)  = (0,0);
	if ( (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (TB3S == 1'b0))
		(B1I2 => C3Z)  = (0,0);
	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (TB3S == 1'b0))
		(B1I3 => C3Z)  = (0,0);



	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B1I0 => C3Z)  = (0,0);
	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B1I1 => C3Z)  = (0,0);
	if ( (B1CIS == 1'b0) && (T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B1I2 => C3Z)  = (0,0);
	if ((T2CIS == 1'b0) && (B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B1I3 => C3Z)  = (0,0);

	if ((T3CIS == 1'b0) && (TB3S == 1'b0))
		(B2I0 => C3Z)  = (0,0);
	if ((T3CIS == 1'b0) && (TB3S == 1'b0))
		(B2I1 => C3Z)  = (0,0);
	if ((B2CIS == 1'b0) && (T3CIS == 1'b0) && (TB3S == 1'b0))
		(B2I2 => C3Z)  = (0,0);
	if ((T3CIS == 1'b0) && (TB3S == 1'b0))
		(B2I3 => C3Z)  = (0,0);


	if ((T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B2I0 => C3Z)  = (0,0);
	if ((T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B2I1 => C3Z)  = (0,0);
	if ((B2CIS == 1'b0) && (T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B2I2 => C3Z)  = (0,0);
	if ((T3CIS == 1'b0) && (B3CIS == 1'b0) && (TB3S == 1'b1))
		(B2I3 => C3Z)  = (0,0);

*/

	(B3I0 => B3CO) = (0,0);
	(B3I1 => B3CO) = (0,0);
	(B3I3 => B3CO) = (0,0);
	(B3I2 => B3CO) = (0,0);

	(T3I0 => T3CO) = (0,0);
	(T3I1 => T3CO) = (0,0);
	(T3I3 => T3CO) = (0,0);
	(T3I2 => T3CO) = (0,0);

/*
	if (B3CIS == 1'b0)
		(T3I0 => B3CO) = (0,0);
	if (B3CIS == 1'b0)
		(T3I1 => B3CO) = (0,0);
	if (B3CIS == 1'b0)
		(T3I3 => B3CO) = (0,0);
	if ((T3CIS == 1'b1) && (B3CIS == 1'b0))
		(T3I2 => B3CO) = (0,0);


	if ((B3CIS == 1'b0) && (T3CIS == 1'b0))
		(B2I0 => B3CO) = (0,0);
	if ((B3CIS == 1'b0) && (T3CIS == 1'b0))
		(B2I1 => B3CO) = (0,0);
	if ((B3CIS == 1'b0) && (T3CIS == 1'b0))
		(B2I3 => B3CO) = (0,0);
	if ((B2CIS == 1'b1) && (B3CIS == 1'b0) && (T3CIS == 1'b0) )
		(B2I2 => B3CO) = (0,0);

	if ((B2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(T2I0 => B3CO) = (0,0);
	if ((B2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(T2I1 => B3CO) = (0,0);
	if ((B2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(T2I3 => B3CO) = (0,0);
	if ((T3CIS == 1'b1) && (B2CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0))
		(T2I2 => B3CO) = (0,0);

	if ((B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(B1I0 => B3CO) = (0,0);
	if ((B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(B1I1 => B3CO) = (0,0);
	if ((B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(B1I3 => B3CO) = (0,0);
	if ((B1CIS == 1'b1) && (B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) )
		(B1I2 => B3CO) = (0,0);

	if ((B1CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(T1I0 => B3CO) = (0,0);
	if ((B1CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(T1I1 => B3CO) = (0,0);
	if ((B1CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(T1I3 => B3CO) = (0,0);
	if ((T1CIS == 1'b1) && (B3CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0))
		(T1I2 => B3CO) = (0,0);


	if ((B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(B0I0 => B3CO) = (0,0);
	if ((B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) )
		(B0I1 => B3CO) = (0,0);
	if ((B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(B0I3 => B3CO) = (0,0);
	if ((B0CIS == 1'b1) && (B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0) )
		(B0I2 => B3CO) = (0,0);

	if ((B0CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(T0I0 => B3CO) = (0,0);
	if ((B0CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(T0I1 => B3CO) = (0,0);
	if ((B0CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(T0I3 => B3CO) = (0,0);
	if ((T0CIS == 1'b1) &&  (B0CIS == 1'b0) && (B3CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(T0I2 => B3CO) = (0,0);

	if ((T0CIS == 1'b0) &&  (B0CIS == 1'b0) && (B3CIS == 1'b0) && (B3CIS == 1'b0) && (T3CIS == 1'b0) && (B2CIS == 1'b0) && (T2CIS == 1'b0) && (B1CIS == 1'b0) && (T1CIS == 1'b0))
		(T0CI => B3CO) = (0,0);
*/

	(B0I0 => B0Z) = (0,0);
	(B0I1 => B0Z) = (0,0);
	(B0I3 => B0Z) = (0,0);
	(B0I2 => B0Z) = (0,0);
	//if (B0CIS == 1'b1)
	//	(B0I2 => B0Z) = (0,0);

	if (TB0S == 1'b1)
		(B0I0 => C0Z) = (0,0);
	if (TB0S == 1'b1)
		(B0I1 => C0Z) = (0,0);
	if (TB0S == 1'b1)
		(B0I3 => C0Z) = (0,0);
	//if ((B0CIS == 1'b1) && (TB0S == 1'b1)) 
	if (TB0S == 1'b1)
		(B0I2 => C0Z) = (0,0);

	if (TB0S == 1'b0)
		(T0I0 => C0Z) = (0,0);
	if (TB0S == 1'b0)
		(T0I1 => C0Z) = (0,0);
	if (TB0S == 1'b0)
		(T0I3 => C0Z) = (0,0);
	//if ((T0CIS == 1'b1) && (TB0S == 1'b0))
	if (TB0S == 1'b0)
		(T0I2 => C0Z) = (0,0);

	(B1I0 => B1Z) = (0,0);
	(B1I1 => B1Z) = (0,0);
	(B1I3 => B1Z) = (0,0);
	(B1I2 => B1Z) = (0,0);
	//if (B1CIS == 1'b1)
	//	(B1I2 => B1Z) = (0,0);

	if (TB1S == 1'b1)
		(B1I0 => C1Z) = (0,0);
	if (TB1S == 1'b1)
		(B1I1 => C1Z) = (0,0);
	if (TB1S == 1'b1)
		(B1I3 => C1Z) = (0,0);
	//if ((B1CIS == 1'b1) && (TB1S == 1'b1)) 
	if (TB1S == 1'b1) 
		(B1I2 => C1Z) = (0,0);

	if (TB1S == 1'b0)
		(T1I0 => C1Z) = (0,0);
	if (TB1S == 1'b0)
		(T1I1 => C1Z) = (0,0);
	if (TB1S == 1'b0)
		(T1I3 => C1Z) = (0,0);
	if (TB1S == 1'b0)
		(T1I2 => C1Z) = (0,0);

	(B2I0 => B2Z) = (0,0);
	(B2I1 => B2Z) = (0,0);
	(B2I3 => B2Z) = (0,0);
	(B2I2 => B2Z) = (0,0);
	//if (B2CIS == 1'b1)
	//	(B2I2 => B2Z) = (0,0);

	if (TB2S == 1'b1)
		(B2I0 => C2Z) = (0,0);
	if (TB2S == 1'b1)
		(B2I1 => C2Z) = (0,0);
	if (TB2S == 1'b1)
		(B2I3 => C2Z) = (0,0);
	//if ((B2CIS == 1'b1) && (TB2S == 1'b1)) 
	if (TB2S == 1'b1)
		(B2I2 => C2Z) = (0,0);

	if (TB2S == 1'b0)
		(T2I0 => C2Z) = (0,0);
	if (TB2S == 1'b0)
		(T2I1 => C2Z) = (0,0);
	if (TB2S == 1'b0)
		(T2I3 => C2Z) = (0,0);
	//if (T2CIS == 1'b1)
	if (TB2S == 1'b0)
		(T2I2 => C2Z) = (0,0);

	(B3I0 => B3Z) = (0,0);
	(B3I1 => B3Z) = (0,0);
	(B3I3 => B3Z) = (0,0);
	(B3I2 => B3Z) = (0,0);
	//if (B3CIS == 1'b1)
	//	(B3I2 => B3Z) = (0,0);

	if (TB3S == 1'b1)
		(B3I0 => C3Z) = (0,0);
	if (TB3S == 1'b1)
		(B3I1 => C3Z) = (0,0);
	if (TB3S == 1'b1)
		(B3I3 => C3Z) = (0,0);
	//if ((B3CIS == 1'b1) && (TB3S == 1'b1)) 
	if (TB3S == 1'b1) 
		(B3I2 => C3Z) = (0,0);

	if (TB3S == 1'b0)
		(T3I0 => C3Z) = (0,0);
	if (TB3S == 1'b0)
		(T3I1 => C3Z) = (0,0);
	if (TB3S == 1'b0)
		(T3I3 => C3Z) = (0,0);
	//if ((T3CIS == 1'b1) && (TB3S == 1'b0))
	if (TB3S == 1'b0)
		(T3I2 => C3Z) = (0,0);


$removal (posedge QRT,negedge QST, 0);
$removal (negedge QRT,negedge QST, 0);
$recovery (posedge QRT,negedge QST, 0);
$recovery (negedge QRT,negedge QST, 0);
$setup( posedge Q1EN, negedge QCK, 0);
$setup( negedge Q1EN, negedge QCK, 0);
$hold( negedge QCK, posedge Q1EN, 0);
$hold( negedge QCK, negedge Q1EN, 0);
$setup( posedge Q2EN, negedge QCK, 0);
$setup( negedge Q2EN, negedge QCK, 0);
$hold( negedge QCK, posedge Q2EN, 0);
$hold( negedge QCK, negedge Q2EN, 0);
$setup( posedge Q2EN, negedge QCK, 0);
$setup( negedge Q2EN, negedge QCK, 0);
$hold( negedge QCK, posedge Q2EN, 0);
$hold( negedge QCK, negedge Q2EN, 0);
$setup( posedge Q3EN, negedge QCK, 0);
$setup( negedge Q3EN, negedge QCK, 0);
$hold( negedge QCK, posedge Q3EN, 0);
$hold( negedge QCK, negedge Q3EN, 0);
endspecify


endmodule    


`timescale 1ns/10ps

module BIDIR (	
		ESEL,
		IE,
		OSEL,
		OQI,
		OQE,	
		FIXHOLD,
		IZ,
		IQZ,
		IQE,
		IQC,
		IQR,
		IN_EN,
		IP,
		CLK_EN,
		DS_0_,
		DS_1_,
		SR,
		ST,
		WP_0_,
		WP_1_
		);
				
input ESEL;
input IE;
input OSEL;
input OQI;
input OQE;
input FIXHOLD;
output IZ;
output IQZ;
input IQE;
input IQC;
input IN_EN;
input IQR;
inout IP;

input	CLK_EN;
input	DS_0_;
input	DS_1_;
input	SR;
input	ST;
input	WP_0_;
input	WP_1_;

reg EN_reg, OQ_reg, IQZ;
wire rstn, EN, OQ, AND_OUT, IQCP;

wire FIXHOLD_int;	
wire ESEL_int;
wire IE_int;
wire OSEL_int;
wire OQI_int;
wire IN_EN_int;
wire OQE_int;
wire IQE_int;
wire IQC_int;
wire IQR_int;

parameter IOwithOUTDriver = 0;        //  has to be set for IO with out Driver

buf FIXHOLD_buf (FIXHOLD_int,FIXHOLD);	
buf IN_EN_buf (INEN_int,INEN);
buf IQC_buf (IQC_int,IQC);
buf IQR_buf (IQR_int,IQR);
buf ESEL_buf (ESEL_int,ESEL);
buf IE_buf (IE_int,IE);
buf OSEL_buf (OSEL_int,OSEL);
buf OQI_buf (OQI_int,OQI);
buf OQE_buf (OQE_int,OQE);
buf IQE_buf (IQE_int,IQE);

assign rstn = ~IQR_int;
assign IQCP = IQC_int;
 if (IOwithOUTDriver)
 begin
	assign IZ = IP;
 end
 else
  begin
	//assign AND_OUT = IN_EN_int ? IP : 1'b0;
	// Changing IN_EN_int, as its functionality is changed now
	assign AND_OUT = ~IN_EN ? IP : 1'b0;

	assign IZ = AND_OUT;
 end
assign EN = ESEL_int ? IE_int : EN_reg ;

assign OQ = OSEL_int ? OQI_int : OQ_reg ;

assign IP = EN ? OQ : 1'bz;

initial
	begin		
		//Power on reset
		EN_reg	= 1'b0;
		OQ_reg= 1'b0;
		IQZ=1'b0;
	end
always @(posedge IQCP or negedge rstn)
	if (~rstn)
		EN_reg <= 1'b0;
	else
		EN_reg <= IE_int;

always @(posedge IQCP or negedge rstn)
	if (~rstn)
		OQ_reg <= 1'b0;
	else
		if (OQE_int)
			OQ_reg <= OQI_int;
			
			
always @(posedge IQCP or negedge rstn)		
	if (~rstn)
		IQZ <= 1'b0;
	else
		if (IQE_int)
			IQZ <= AND_OUT;
		
// orig value
//wire gpio_c18 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b1 && DS == 1'b1 && IQCS == 1'b1);
//wire gpio_c16 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b1 && DS == 1'b0 && IQCS == 1'b1);
//wire gpio_c14 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b0 && DS == 1'b1 && IQCS == 1'b1);
//wire gpio_c12 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b0 && DS == 1'b0 && IQCS == 1'b1);
//wire gpio_c10 = (OSEL == 1'b0  && OQE == 1'b1 && IQCS == 1'b1);
//wire gpio_c8 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b1 && DS == 1'b1 && IQCS == 1'b0);
//wire gpio_c6 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b1 && DS == 1'b0 && IQCS == 1'b0);
//wire gpio_c4 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b0 && DS == 1'b1 && IQCS == 1'b0);
//wire gpio_c2 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b0 && DS == 1'b0 && IQCS == 1'b0);
//wire gpio_c0 = (OSEL == 1'b0  && OQE == 1'b1 && IQCS == 1'b0);
//wire gpio_c30 = (IQE == 1'b1  && FIXHOLD == 1'b1 && IN_EN == 1'b1 && IQCS == 1'b1);
//wire gpio_c28 = (IQE == 1'b1  && FIXHOLD == 1'b0 && IN_EN == 1'b1 && IQCS == 1'b1);
//wire gpio_c22 = (IQE == 1'b1  && FIXHOLD == 1'b1 && IN_EN == 1'b1 && IQCS == 1'b0);
//wire gpio_c20 = (IQE == 1'b1  && FIXHOLD == 1'b0 && IN_EN == 1'b1 && IQCS == 1'b0);

// changed one
wire gpio_c18 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 );
wire gpio_c16 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 );
wire gpio_c14 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 );
wire gpio_c12 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 );
wire gpio_c10 = (OSEL == 1'b0  && OQE == 1'b1 );
wire gpio_c8 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1);
wire gpio_c6 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 );
wire gpio_c4 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1);
wire gpio_c2 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1);
wire gpio_c0 = (OSEL == 1'b0  && OQE == 1'b1);
wire gpio_c30 = (IQE == 1'b1  && IN_EN == 1'b0);
wire gpio_c28 = (IQE == 1'b1  && IN_EN == 1'b0);
wire gpio_c22 = (IQE == 1'b1  && IN_EN == 1'b0);
wire gpio_c20 = (IQE == 1'b1  && IN_EN == 1'b0);
specify
if ( IQE == 1'b1  )
(IQC => IQZ) = (0,0,0,0,0,0);
(IQR => IQZ) = (0,0);
if ( IE == 1'b1 && OQE == 1'b1  )
(IQC => IZ) = (0,0,0,0,0,0);
if ( IE == 1'b0 )
(IP => IZ) = (0,0);
if ( IE == 1'b0 && IN_EN == 1'b1  )
(IP => IZ) = (0,0);
$setup (posedge IE,negedge IQC, 0);
$setup (negedge IE,negedge IQC, 0);
$hold (negedge IQC,posedge IE, 0);
$hold (negedge IQC,negedge IE, 0);
$setup (posedge IE,posedge IQC, 0);
$setup (negedge IE,posedge IQC, 0);
$hold (posedge IQC,posedge IE, 0);
$hold (posedge IQC,negedge IE, 0);
$setup( posedge OQI, negedge IQC &&& gpio_c18, 0);
$setup( negedge OQI, negedge IQC &&& gpio_c18, 0);
$hold( negedge IQC, posedge OQI &&& gpio_c18, 0);
$hold( negedge IQC, negedge OQI &&& gpio_c18, 0);
$setup( posedge OQI, negedge IQC &&& gpio_c16, 0);
$setup( negedge OQI, negedge IQC &&& gpio_c16, 0);
$hold( negedge IQC, posedge OQI &&& gpio_c16, 0);
$hold( negedge IQC, negedge OQI &&& gpio_c16, 0);
$setup( posedge OQI, negedge IQC &&& gpio_c14, 0);
$setup( negedge OQI, negedge IQC &&& gpio_c14, 0);
$hold( negedge IQC, posedge OQI &&& gpio_c14, 0);
$hold( negedge IQC, negedge OQI &&& gpio_c14, 0);
$setup( posedge OQI, negedge IQC &&& gpio_c12, 0);
$setup( negedge OQI, negedge IQC &&& gpio_c12, 0);
$hold( negedge IQC, posedge OQI &&& gpio_c12, 0);
$hold( negedge IQC, negedge OQI &&& gpio_c12, 0);
$setup( posedge OQI, negedge IQC &&& gpio_c10, 0);
$setup( negedge OQI, negedge IQC &&& gpio_c10, 0);
$hold( negedge IQC, posedge OQI &&& gpio_c10, 0);
$hold( negedge IQC, negedge OQI &&& gpio_c10, 0);
$setup( posedge OQI, posedge IQC &&& gpio_c8, 0);
$setup( negedge OQI, posedge IQC &&& gpio_c8, 0);
$hold( posedge IQC, posedge OQI &&& gpio_c8, 0);
$hold( posedge IQC, negedge OQI &&& gpio_c8, 0);
$setup( posedge OQI, posedge IQC &&& gpio_c6, 0);
$setup( negedge OQI, posedge IQC &&& gpio_c6, 0);
$hold( posedge IQC, posedge OQI &&& gpio_c6, 0);
$hold( posedge IQC, negedge OQI &&& gpio_c6, 0);
$setup( posedge OQI, posedge IQC &&& gpio_c4, 0);
$setup( negedge OQI, posedge IQC &&& gpio_c4, 0);
$hold( posedge IQC, posedge OQI &&& gpio_c4, 0);
$hold( posedge IQC, negedge OQI &&& gpio_c4, 0);
$setup( posedge OQI, posedge IQC &&& gpio_c2, 0);
$setup( negedge OQI, posedge IQC &&& gpio_c2, 0);
$hold( posedge IQC, posedge OQI &&& gpio_c2, 0);
$hold( posedge IQC, negedge OQI &&& gpio_c2, 0);
$setup( posedge OQI, posedge IQC &&& gpio_c0, 0);
$setup( negedge OQI, posedge IQC &&& gpio_c0, 0);
$hold( posedge IQC, posedge OQI &&& gpio_c0, 0);
$hold( posedge IQC, negedge OQI &&& gpio_c0, 0);
$setup (posedge OQE,negedge IQC, 0);
$setup (negedge OQE,negedge IQC, 0);
$hold (negedge IQC,posedge OQE, 0);
$hold (negedge IQC,negedge OQE, 0);
$setup (posedge OQE,posedge IQC, 0);
$setup (negedge OQE,posedge IQC, 0);
$hold (posedge IQC,posedge OQE, 0);
$hold (posedge IQC,negedge OQE, 0);
$setup (posedge IQE,negedge IQC, 0);
$setup (negedge IQE,negedge IQC, 0);
$hold (negedge IQC,posedge IQE, 0);
$hold (negedge IQC,negedge IQE, 0);
$setup (posedge IQE,posedge IQC, 0);
$setup (negedge IQE,posedge IQC, 0);
$hold (posedge IQC,posedge IQE, 0);
$hold (posedge IQC,negedge IQE, 0);
$recovery (posedge IQR,negedge IQC, 0);
$recovery (negedge IQR,negedge IQC, 0);
$removal (posedge IQR,negedge IQC, 0);
$removal (negedge IQR,negedge IQC, 0);
$recovery (posedge IQR,posedge IQC, 0);
$recovery (negedge IQR,posedge IQC, 0);
$removal (posedge IQR,posedge IQC, 0);
$removal (negedge IQR,posedge IQC, 0);
$setup( posedge IP, negedge IQC &&& gpio_c30, 0);
$setup( negedge IP, negedge IQC &&& gpio_c30, 0);
$hold( negedge IQC, posedge IP &&& gpio_c30, 0);
$hold( negedge IQC, negedge IP &&& gpio_c30, 0);
$setup( posedge IP, negedge IQC &&& gpio_c28, 0);
$setup( negedge IP, negedge IQC &&& gpio_c28, 0);
$hold( negedge IQC, posedge IP &&& gpio_c28, 0);
$hold( negedge IQC, negedge IP &&& gpio_c28, 0);
$setup( posedge IP, posedge IQC &&& gpio_c22, 0);
$setup( negedge IP, posedge IQC &&& gpio_c22, 0);
$hold( posedge IQC, posedge IP &&& gpio_c22, 0);
$hold( posedge IQC, negedge IP &&& gpio_c22, 0);
$setup( posedge IP, posedge IQC &&& gpio_c20, 0);
$setup( negedge IP, posedge IQC &&& gpio_c20, 0);
$hold( posedge IQC, posedge IP &&& gpio_c20, 0);
$hold( posedge IQC, negedge IP &&& gpio_c20, 0);
(IE => IP) = (0,0,0,0,0,0);
if ( IE == 1'b1 )
(OQI => IP) = (0,0);
(IQC => IP) = (0,0,0,0,0,0);
if ( IE == 1'b1 && OQE == 1'b1  )
(IQC => IP) = (0,0,0,0,0,0);
if ( IE == 1'b0 )
(IQR => IP) = (0,0);
endspecify
endmodule

//pragma synthesis_off
module sw_mux (
	port_out,
	default_port,
	alt_port,
	switch
	);
	
	output port_out;
	input default_port;
	input alt_port;
	input switch;
	
	assign port_out = switch ? alt_port : default_port;
	
endmodule
//pragma synthesis_on

`timescale 1ns/10ps
module QMUX (GMUXIN, QHSCK, IS, IZ);
input GMUXIN, QHSCK, IS;
output IZ;

wire GMUXIN_int,  QHSCK_int, IS_int;
buf GMUXIN_buf (GMUXIN_int, GMUXIN) ;
buf QHSCK_buf (QHSCK_int, QHSCK) ;
buf IS_buf (IS_int, IS) ;

assign IZ = IS ? QHSCK_int : GMUXIN_int; 

specify
   (GMUXIN => IZ) = (0,0);
   (QHSCK => IZ) = (0,0);
   (IS => IZ) = (0,0);
endspecify

endmodule 

module QPMUX (QCLKIN, QHSCK, GMUXIN, IS0, IS1, IZ);
input QCLKIN, QHSCK, GMUXIN, IS0, IS1;
output IZ;

wire GMUXIN_int, QCLKIN_int, QHSCK_int, IS_int;
buf GMUXIN_buf (GMUXIN_int, GMUXIN) ;
buf QHSCK_buf (QHSCK_int, QHSCK) ;
buf QCLKIN_buf (QCLKIN_int, QCLKIN) ;
buf IS0_buf (IS0_int, IS0);
buf IS1_buf (IS1_int, IS1);

//assign IZ = IS0 ? (IS1 ? QHSCK_int : QCLKIN_int) : (IS1 ? QHSCK_int : GMUXIN_int); 
assign IZ = IS0_int ? (IS1_int ? QHSCK_int : GMUXIN_int) : (IS1_int ? QHSCK_int : QCLKIN_int); 

specify
   (QCLKIN => IZ) = (0,0);
   (QHSCK => IZ) = (0,0);
   (GMUXIN => IZ) = (0,0);
   (IS0 => IZ) = (0,0);
   (IS1 => IZ) = (0,0);
endspecify

endmodule 

module GMUX(GCLKIN, GHSCK, SSEL, BL_DEN, BL_DYNEN, BL_SEN, BL_VLP, BR_DEN, 
            BR_DYNEN, BR_SEN, 
            BR_VLP, TL_DEN, TL_DYNEN, TL_SEN, TL_VLP, TR_DEN, TR_DYNEN, TR_SEN, TR_VLP, IZ);
input GCLKIN, GHSCK, SSEL, BL_DYNEN, BL_VLP, BR_DEN, BR_DYNEN, BR_SEN, BL_DEN, BL_SEN, 
		 BR_VLP, TL_DEN, TL_DYNEN, TL_SEN, TL_VLP, TR_DEN, TR_DYNEN, TR_SEN, TR_VLP; 
output IZ; 
wire GCLKIN_int, GHSCK_int, SSEL_int;
wire wire_mux_op_0;


buf GCLKIN_buf (GCLKIN_int, GCLKIN) ;
buf GHSCK_buf (GHSCK_int, GHSCK) ;
buf SSEL_buf (SSEL_int, SSEL) ;
//buf SEN_buf (SEN_int, SEN) ;
//buf DYNEN_buf (DYNEN_int, DYNEN) ;
//buf DEN_buf (DEN_int, DEN) ;
//buf VLP_buf (VLP_int, VLP) ;

assign wire_mux_op_0 = SSEL_int ? GHSCK_int : GCLKIN_int;
//assign wire_mux_op_1 = SEN_int ? 1'b1 : 1'b0;
//assign wire_mux_op_2 = DEN_int ? DYNEN_int : wire_mux_op_1;
//assign wire_mux_op_3 = VLP_int ? 1'b0 : wire_mux_op_2;

assign IZ  = wire_mux_op_0;

specify
   (GCLKIN => IZ) = (0,0);
   (GHSCK => IZ) = (0,0);
   (BL_DEN => IZ) = (0,0);
   (BL_DYNEN => IZ) = (0,0);
	(BL_SEN => IZ) = (0,0);   
	(BL_VLP => IZ) = (0,0); 
	(BR_DEN => IZ) = (0,0);
	(BR_SEN => IZ) = (0,0); 
	(BL_DEN => IZ) = (0,0); 
	(BL_SEN => IZ) = (0,0); 
	(BL_VLP => IZ) = (0,0); 
	(BR_DEN => IZ) = (0,0); 
	(BR_SEN => IZ) = (0,0);
    (BR_VLP => IZ) = (0,0); 
	(TL_DEN => IZ) = (0,0); 
	(TL_SEN => IZ) = (0,0); 
	(TL_VLP => IZ) = (0,0); 
	(TR_DEN => IZ) = (0,0); 
	(TR_SEN => IZ) = (0,0); 
	(TR_VLP => IZ) = (0,0);
	(BL_DYNEN => IZ) = (0,0); 
    (BR_DYNEN => IZ) = (0,0); 
	(TR_DYNEN => IZ) = (0,0); 
	(TL_DYNEN => IZ) = (0,0); 
endspecify

endmodule 

module SQMUX(QMUXIN, SQHSCK, SELECT, IZ);
input QMUXIN, SQHSCK,SELECT;
output IZ;

wire QMUXIN_int, SQHSCK_int, SELECT_int;

buf QMUXIN_buf (QMUXIN_int, QMUXIN) ;
buf SQHSCK_buf (SQHSCK_int, SQHSCK) ;
buf SELECT_buf (SELECT_int, SELECT) ;

assign IZ = SELECT_int ?  SQHSCK_int : QMUXIN_int;
specify
   (QMUXIN => IZ) = (0,0);
   (SQHSCK => IZ) = (0,0);
   (SELECT => IZ) = (0,0);
endspecify

endmodule


`timescale 1ns/10ps
module SQEMUX(QMUXIN, SQHSCK, DYNEN, SEN, DEN, SELECT, IZ);
input QMUXIN, SQHSCK, DYNEN, SEN, DEN, SELECT;
output IZ;

wire QMUXIN_int, SQHSCK_int, DYNEN_int, SEN_int, DEN_int, SELECT_int;
buf QMUXIN_buf (QMUXIN_int, QMUXIN) ;
buf SQHSCK_buf (SQHSCK_int, SQHSCK) ;
buf DYNEN_buf (DYNEN_int, DYNEN) ;
buf SEN_buf (SEN_int, SEN) ;
buf DEN_buf (DEN_int, DEN) ;
buf SELECT_buf (SELECT_int, SELECT) ;


assign IZ = SELECT_int ?  SQHSCK_int : QMUXIN_int;
	
specify
   (QMUXIN => IZ) = (0,0);
   (SQHSCK => IZ) = (0,0);
   (DYNEN => IZ) = (0,0);
   (SEN => IZ) = (0,0);
   (DEN => IZ) = (0,0);
   (SELECT => IZ) = (0,0);
endspecify
endmodule



`timescale 1ns/10ps
module CAND(SEN, CLKIN, IZ);
input SEN, CLKIN;
output IZ;
wire SEN_int, CLKIN_int;
buf SEN_buf (SEN_int, SEN) ;
buf CLKIN_buf (CLKIN_int, CLKIN) ;
assign IZ = CLKIN_int & SEN_int; 

specify
   (CLKIN => IZ) = (0,0);
   (SEN => IZ) = (0,0);
endspecify
endmodule 

module CANDEN(CLKIN, DYNEN, SEN, DEN, IZ);
input CLKIN, DYNEN, SEN, DEN;
output IZ;
wire CLKIN_int, DYNEN_int, SEN_int, DEN_int; 
wire mux_op0, mux_op1;

buf SEN_buf (SEN_int, SEN) ;
buf CLKIN_buf (CLKIN_int, CLKIN) ;
buf DYNEN_buf (DYNEN_int, DYNEN) ;
buf DEN_buf (DEN_int, DEN) ;

assign mux_op0 = SEN_int ? 1'b1 : 1'b0;
assign mux_op1 = DEN_int ? DYNEN_int : mux_op0;

assign IZ = CLKIN_int & SEN_int; 

specify
   (CLKIN => IZ) = (0,0);
   (DYNEN => IZ) = (0,0);
   (SEN => IZ) = (0,0);
   (DEN => IZ) = (0,0);
endspecify

endmodule

`timescale 1ns/10ps
module CLOCK(IP, CEN, IC, OP);
input IP, CEN;
output IC, OP;
buf IP_buf (IP_int, IP) ;
buf CEN_buf (CEN_int, CEN) ;

buf (IC, IP_int);

specify
   (IP => IC) = (0,0);
endspecify


endmodule

// P_MUX3 cell -----------------------------------------------------------------
//

module P_MUX3( A, B, C, D, S, T, E, Z );
input A, B, C, D, S, E, T;
output Z;

udpmux3 QL2 ( Z, A, B, C, D, E, S, T );

specify
   (A => Z) = 0;
   (B => Z) = 0;
   (C => Z) = 0;
   (D => Z) = 0;
   (E => Z) = 0;
   (S => Z) = 0;
   (T => Z) = 0;
endspecify

endmodule

primitive udpmux3(Z, A, B, C, D, E, S, T);
   output Z;
   input A, B, C, D, E, S, T;
   table
   // A  B  C  D  E     S  T   :    Z
      1  ?  ?  ?  ?     0  0   :    1  ;
      0  ?  ?  ?  ?     0  0   :    0  ;
      ?  0  ?  ?  ?     0  1   :    0  ;
      ?  1  ?  ?  ?     0  1   :    1  ;
      ?  ?  0  ?  ?     1  0   :    0  ;
      ?  ?  1  ?  ?     1  0   :    1  ;
      ?  ?  ?  0  ?     1  1   :    0  ;
      ?  ?  ?  1  ?     1  1   :    1  ;
   endtable
endprimitive // udpmux3

// P_MUX2 cell -----------------------------------------------------------------


module P_MUX2( A, B, C, D, S, Z);
input A, B, C, D, S;
output Z;

udpmux2 QL1 ( Z, A, B, C, D, S );

specify
   (A => Z) = 0;
   (B => Z) = 0;
   (C => Z) = 0;
   (D => Z) = 0;
   (S => Z) = 0;
endspecify

endmodule // P_MUX2

// P_BUF cell -----------------------------------------------------------------

module P_BUF( A, Z);
input A;
output Z;

buf QL1 (Z, A);

specify
   (A => Z) = 0;
endspecify

endmodule

primitive udpmux2(Z, A, B, C, D, S);
   output Z;
   input A, B, C, D, S;
   table
      // A  B  C  D  S   :    Z
         1  0  ?  ?  0   :    1  ;
         0  ?  ?  ?  0   :    0  ;
         ?  1  ?  ?  0   :    0  ;
         ?  ?  1  0  1   :    1  ;
         ?  ?  0  ?  1   :    0  ;
         ?  ?  ?  1  1   :    0  ;
// Reduce pessimism
         1  0  1  0  ?   :    1  ;
         0  ?  0  ?  ?   :    0  ;
         0  ?  ?  1  ?   :    0  ; // new
         ?  1  ?  1  ?   :    0  ;
         ?  1  0  ?  ?   :    0  ; // new
   endtable
endprimitive // udpmux2

primitive udpand6(Z, A, B, C, D, E, F);
   output Z;
   input A, B, C, D, E, F;
   table
      // A  B  C  D  E  F  :  Z
         1  0  1  0  1  0  :  1  ;
         0  ?  ?  ?  ?  ?  :  0  ;
         ?  1  ?  ?  ?  ?  :  0  ;
         ?  ?  0  ?  ?  ?  :  0  ;
         ?  ?  ?  1  ?  ?  :  0  ;
         ?  ?  ?  ?  0  ?  :  0  ;
         ?  ?  ?  ?  ?  1  :  0  ;
   endtable
endprimitive // udpand6

module P_AND6( A, B, C, D, E, F, Z );
input A, B, C, D, E, F;
output Z;

udpand6 QL1 ( Z, A, B, C, D, E, F );

specify
   (A => Z) = 0;
   (B => Z) = 0;
   (C => Z) = 0;
   (D => Z) = 0;
   (E => Z) = 0;
   (F => Z) = 0;
endspecify

endmodule // P_AND6

`timescale 1ns/10ps
module SDIOMUX (SD_IP, SD_IZ, SD_OQI, SD_OE);

input  SD_OE, SD_OQI;
output SD_IZ;
inout SD_IP;

assign SD_IP = SD_OE ? SD_OQI : 1'bz;
assign SD_IZ = ~SD_OE ? SD_IP : 1'bz;

specify

if ( SD_OE == 1'b0 ) (SD_IP => SD_IZ) = (0,0);
if ( SD_OE == 1'b1 ) (SD_OQI => SD_IP) = (0,0);

(SD_IP => SD_IZ) = (0,0,0,0,0,0);
(SD_OE => SD_IP) = (0,0,0,0,0,0);

endspecify

endmodule

`timescale 1ns/10ps
module OBUF( IN_OBUF, OUT_OBUF);
input IN_OBUF;
output OUT_OBUF;

buf QL1 (OUT_OBUF, IN_OBUF);

specify
   (IN_OBUF => OUT_OBUF) = 0;
endspecify

endmodule

`timescale 1ns/10ps
module IBUF(IN_IBUF, OUT_IBUF);
input IN_IBUF;
output OUT_IBUF;

buf QL1 (OUT_IBUF, IN_IBUF);

specify
   (IN_IBUF => OUT_IBUF) = 0;
endspecify

endmodule

`timescale 1ns/10ps
module IO_REG (	
		ESEL,
		IE,
		OSEL,
		OQI,
		OQE,	
		FIXHOLD,
		IZ,
		IQZ,
		IQE,
		IQC,
		IQR,
		INEN,
                A2F_reg, 
                F2A_reg_0_, 
                F2A_reg_1_
		);
				
input ESEL;
input IE;
input OSEL;
input OQI;
input OQE;
input FIXHOLD;
output IZ;
output IQZ;
input IQE;
input IQC;
input INEN;
input IQR;
input A2F_reg;
output F2A_reg_0_;
output F2A_reg_1_;
//inout IP;

reg EN_reg, OQ_reg, IQZ;
wire rstn, EN, OQ, AND_OUT;

wire FIXHOLD_int;	
wire ESEL_int;
wire IE_int;
wire OSEL_int;
wire OQI_int;
wire INEN_int;
wire OQE_int;
wire IQE_int;
wire IQC_int;
wire IQR_int;
wire A2F_reg_int;

parameter IOwithOUTDriver = 0;        //  has to be set for IO with out Driver

buf FIXHOLD_buf (FIXHOLD_int,FIXHOLD);	
buf INEN_buf (INEN_int,INEN);
buf IQC_buf (IQC_int,IQC);
buf IQR_buf (IQR_int,IQR);
buf ESEL_buf (ESEL_int,ESEL);
buf IE_buf (IE_int,IE);
buf OSEL_buf (OSEL_int,OSEL);
buf OQI_buf (OQI_int,OQI);
buf OQE_buf (OQE_int,OQE);
buf IQE_buf (IQE_int,IQE);
buf A2F_reg_buf (A2F_reg_int, A2F_reg);

assign rstn = ~IQR_int;
 if (IOwithOUTDriver)
 begin
	//assign AND_OUT = IQIN_int;

	//assign IZ = IP;
	assign IZ = A2F_reg_int;
 end
 else
  begin
	//assign AND_OUT = INEN_int ? IP : 1'b0;
	// Changing INEN_int, as its functionality is changed now
	//assign AND_OUT = ~INEN_int ? IP : 1'b0;
	assign AND_OUT = ~INEN_int ? A2F_reg_int : 1'b0;

	assign IZ = AND_OUT;
 end
assign EN = ESEL_int ? IE_int : EN_reg ;

assign OQ = OSEL_int ? OQI_int : OQ_reg ;

assign F2A_reg_0_ = EN; 
assign F2A_reg_1_ = OQ; 
//output F2A_reg_0_;
//output F2A_reg_1_;
//assign IP = EN ? OQ : 1'bz;

//assign (highz1,pull0) IP = WPD ? 1'b0 : 1'b1;
initial
	begin		
		//Power on reset
		EN_reg	= 1'b0;
		OQ_reg= 1'b0;
		IQZ=1'b0;
	end
always @(posedge IQC_int or negedge rstn)
	if (~rstn)
		EN_reg <= 1'b0;
	else
		EN_reg <= IE_int;

always @(posedge IQC_int or negedge rstn)
	if (~rstn)
		OQ_reg <= 1'b0;
	else
		if (OQE_int)
			OQ_reg <= OQI_int;
			
			
always @(posedge IQC_int or negedge rstn)		
	if (~rstn)
		IQZ <= 1'b0;
	else
		if (IQE_int)
			IQZ <= AND_OUT;
		
// orig value
//wire gpio_c18 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b1 && DS == 1'b1 && IQCS == 1'b1);
//wire gpio_c16 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b1 && DS == 1'b0 && IQCS == 1'b1);
//wire gpio_c14 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b0 && DS == 1'b1 && IQCS == 1'b1);
//wire gpio_c12 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b0 && DS == 1'b0 && IQCS == 1'b1);
//wire gpio_c10 = (OSEL == 1'b0  && OQE == 1'b1 && IQCS == 1'b1);
//wire gpio_c8 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b1 && DS == 1'b1 && IQCS == 1'b0);
//wire gpio_c6 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b1 && DS == 1'b0 && IQCS == 1'b0);
//wire gpio_c4 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b0 && DS == 1'b1 && IQCS == 1'b0);
//wire gpio_c2 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1 && FIXHOLD == 1'b0 && DS == 1'b0 && IQCS == 1'b0);
//wire gpio_c0 = (OSEL == 1'b0  && OQE == 1'b1 && IQCS == 1'b0);
//wire gpio_c30 = (IQE == 1'b1  && FIXHOLD == 1'b1 && INEN == 1'b1 && IQCS == 1'b1);
//wire gpio_c28 = (IQE == 1'b1  && FIXHOLD == 1'b0 && INEN == 1'b1 && IQCS == 1'b1);
//wire gpio_c22 = (IQE == 1'b1  && FIXHOLD == 1'b1 && INEN == 1'b1 && IQCS == 1'b0);
//wire gpio_c20 = (IQE == 1'b1  && FIXHOLD == 1'b0 && INEN == 1'b1 && IQCS == 1'b0);

// changed one
wire gpio_c18 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1);
wire gpio_c16 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1);
wire gpio_c14 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1);
wire gpio_c12 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1);
wire gpio_c10 = (OSEL == 1'b0  && OQE == 1'b1);
wire gpio_c8 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1);
wire gpio_c6 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1);
wire gpio_c4 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1);
wire gpio_c2 = (OSEL == 1'b1  && IE == 1'b1 && IQE == 1'b1);
wire gpio_c0 = (OSEL == 1'b0  && OQE == 1'b1);
wire gpio_c30 = (IQE == 1'b1  && INEN == 1'b0);
wire gpio_c28 = (IQE == 1'b1  && INEN == 1'b0);
wire gpio_c22 = (IQE == 1'b1  && INEN == 1'b0);
wire gpio_c20 = (IQE == 1'b1  && INEN == 1'b0);
specify
if ( IQE == 1'b1  )
(IQC => IQZ) = (0,0,0,0,0,0);
(IQR => IQZ) = (0,0);
if ( IE == 1'b1 && OQE == 1'b1  )
(IQC => IZ) = (0,0,0,0,0,0);
if ( IE == 1'b0 )
(A2F_reg => IZ) = (0,0);
if ( IE == 1'b0 && INEN == 1'b1  )
(A2F_reg => IZ) = (0,0);
$setup (posedge IE,negedge IQC, 0);
$setup (negedge IE,negedge IQC, 0);
$hold (negedge IQC,posedge IE, 0);
$hold (negedge IQC,negedge IE, 0);
$setup (posedge IE,posedge IQC, 0);
$setup (negedge IE,posedge IQC, 0);
$hold (posedge IQC,posedge IE, 0);
$hold (posedge IQC,negedge IE, 0);
$setup( posedge OQI, negedge IQC &&& gpio_c18, 0);
$setup( negedge OQI, negedge IQC &&& gpio_c18, 0);
$hold( negedge IQC, posedge OQI &&& gpio_c18, 0);
$hold( negedge IQC, negedge OQI &&& gpio_c18, 0);
$setup( posedge OQI, negedge IQC &&& gpio_c16, 0);
$setup( negedge OQI, negedge IQC &&& gpio_c16, 0);
$hold( negedge IQC, posedge OQI &&& gpio_c16, 0);
$hold( negedge IQC, negedge OQI &&& gpio_c16, 0);
$setup( posedge OQI, negedge IQC &&& gpio_c14, 0);
$setup( negedge OQI, negedge IQC &&& gpio_c14, 0);
$hold( negedge IQC, posedge OQI &&& gpio_c14, 0);
$hold( negedge IQC, negedge OQI &&& gpio_c14, 0);
$setup( posedge OQI, negedge IQC &&& gpio_c12, 0);
$setup( negedge OQI, negedge IQC &&& gpio_c12, 0);
$hold( negedge IQC, posedge OQI &&& gpio_c12, 0);
$hold( negedge IQC, negedge OQI &&& gpio_c12, 0);
$setup( posedge OQI, negedge IQC &&& gpio_c10, 0);
$setup( negedge OQI, negedge IQC &&& gpio_c10, 0);
$hold( negedge IQC, posedge OQI &&& gpio_c10, 0);
$hold( negedge IQC, negedge OQI &&& gpio_c10, 0);
$setup( posedge OQI, posedge IQC &&& gpio_c8, 0);
$setup( negedge OQI, posedge IQC &&& gpio_c8, 0);
$hold( posedge IQC, posedge OQI &&& gpio_c8, 0);
$hold( posedge IQC, negedge OQI &&& gpio_c8, 0);
$setup( posedge OQI, posedge IQC &&& gpio_c6, 0);
$setup( negedge OQI, posedge IQC &&& gpio_c6, 0);
$hold( posedge IQC, posedge OQI &&& gpio_c6, 0);
$hold( posedge IQC, negedge OQI &&& gpio_c6, 0);
$setup( posedge OQI, posedge IQC &&& gpio_c4, 0);
$setup( negedge OQI, posedge IQC &&& gpio_c4, 0);
$hold( posedge IQC, posedge OQI &&& gpio_c4, 0);
$hold( posedge IQC, negedge OQI &&& gpio_c4, 0);
$setup( posedge OQI, posedge IQC &&& gpio_c2, 0);
$setup( negedge OQI, posedge IQC &&& gpio_c2, 0);
$hold( posedge IQC, posedge OQI &&& gpio_c2, 0);
$hold( posedge IQC, negedge OQI &&& gpio_c2, 0);
$setup( posedge OQI, posedge IQC &&& gpio_c0, 0);
$setup( negedge OQI, posedge IQC &&& gpio_c0, 0);
$hold( posedge IQC, posedge OQI &&& gpio_c0, 0);
$hold( posedge IQC, negedge OQI &&& gpio_c0, 0);
$setup (posedge OQE,negedge IQC, 0);
$setup (negedge OQE,negedge IQC, 0);
$hold (negedge IQC,posedge OQE, 0);
$hold (negedge IQC,negedge OQE, 0);
$setup (posedge OQE,posedge IQC, 0);
$setup (negedge OQE,posedge IQC, 0);
$hold (posedge IQC,posedge OQE, 0);
$hold (posedge IQC,negedge OQE, 0);
$setup (posedge IQE,negedge IQC, 0);
$setup (negedge IQE,negedge IQC, 0);
$hold (negedge IQC,posedge IQE, 0);
$hold (negedge IQC,negedge IQE, 0);
$setup (posedge IQE,posedge IQC, 0);
$setup (negedge IQE,posedge IQC, 0);
$hold (posedge IQC,posedge IQE, 0);
$hold (posedge IQC,negedge IQE, 0);
$recovery (posedge IQR,negedge IQC, 0);
$recovery (negedge IQR,negedge IQC, 0);
$removal (posedge IQR,negedge IQC, 0);
$removal (negedge IQR,negedge IQC, 0);
$recovery (posedge IQR,posedge IQC, 0);
$recovery (negedge IQR,posedge IQC, 0);
$removal (posedge IQR,posedge IQC, 0);
$removal (negedge IQR,posedge IQC, 0);
$setup( posedge A2F_reg, negedge IQC &&& gpio_c30, 0);
$setup( negedge A2F_reg, negedge IQC &&& gpio_c30, 0);
$hold( negedge IQC, posedge A2F_reg &&& gpio_c30, 0);
$hold( negedge IQC, negedge A2F_reg &&& gpio_c30, 0);
$setup( posedge A2F_reg, negedge IQC &&& gpio_c28, 0);
$setup( negedge A2F_reg, negedge IQC &&& gpio_c28, 0);
$hold( negedge IQC, posedge A2F_reg &&& gpio_c28, 0);
$hold( negedge IQC, negedge A2F_reg &&& gpio_c28, 0);
$setup( posedge A2F_reg, posedge IQC &&& gpio_c22, 0);
$setup( negedge A2F_reg, posedge IQC &&& gpio_c22, 0);
$hold( posedge IQC, posedge A2F_reg &&& gpio_c22, 0);
$hold( posedge IQC, negedge A2F_reg &&& gpio_c22, 0);
$setup( posedge A2F_reg, posedge IQC &&& gpio_c20, 0);
$setup( negedge A2F_reg, posedge IQC &&& gpio_c20, 0);
$hold( posedge IQC, posedge A2F_reg &&& gpio_c20, 0);
$hold( posedge IQC, negedge A2F_reg &&& gpio_c20, 0);
(IE => F2A_reg_0_) = (0,0,0,0,0,0);
if ( IE == 1'b1 )
(IQC => F2A_reg_0_) = (0,0,0,0,0,0);
if ( IE == 1'b1 && OQE == 1'b1  )
(OQI => F2A_reg_1_) = (0,0);
(IQC => F2A_reg_1_) = (0,0,0,0,0,0);
if ( IE == 1'b0 )
(IQR => F2A_reg_0_) = (0,0);
(IQR => F2A_reg_1_) = (0,0);
endspecify
endmodule
