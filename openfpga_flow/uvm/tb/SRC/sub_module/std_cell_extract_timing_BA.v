`timescale 1ns/1ps

//`include "/research/ece/lnis/USERS/DARPA_ERI/GF14nm_chip_2019/Chip_02_20_V2/FPGA_core_32x32/SRC/fpga_defines.v"
//----- Defines

`define ARM_PROP_DELAY 		0.08
`define ARM_INVALID_DELAY 	1.0

`define ARM_WIDTH 			1.0
`define ARM_PERIOD 			2*`ARM_WIDTH+0.5
`define ARM_SETUP_TIME 		0.02
`define ARM_HOLD_TIME 		0.008
`define ARM_RECOVERY_TIME 	1.0
`define ARM_REMOVAL_TIME 	0.5
`define ARM_WIDTH_THD 		0.0
`define ARM_OUTPUT_LATCH_DURATION 5.0
`define ARM_INIT_CORRUPTION_TIME  0.0
`define ARM_DONT_WARN_AT_CORRUPTION 0
`define ARM_ALLOW_SET_RESET_DURING_RESTORE 0

`define ARM_PROP_IODELAY 	 0.3		//Delay definitions; Default is 1ns
`define PULL				 3000.000	//Pull delay; Default is 3000ns
`define ARM_IO_INVALID_DELAY 100.000

//----- Fixed Values
`celldefine
module TIEHI_X1N_A10P5PP84TR_C14 (Y);
output Y;

  buf I0(Y, 1'b1);


specify

endspecify
endmodule // TIEHI_X1N_A10P5PP84TR_C14
`endcelldefine



`celldefine
module TIELO_X1N_A10P5PP84TR_C14 (Y);
output Y;

  buf I0(Y, 1'b0);


specify

endspecify
endmodule // TIELO_X1N_A10P5PP84TR_C14
`endcelldefine

//----- Inverters
`celldefine
module INV_X1N_A10P5PP84TR_C14 (Y, A);
output Y;
input A;

  not I0(Y, A);

specify
(A => Y) = (0.004,0.004);

endspecify
endmodule // INV_X1N_A10P5PP84TR_C14
`endcelldefine



`celldefine
module INV_X4N_A10P5PP84TR_C14 (Y, A);
output Y;
input A;

  not I0(Y, A);

specify
(A => Y) = (0.004,0.004);

endspecify
endmodule // INV_X4N_A10P5PP84TR_C14
`endcelldefine



`celldefine
module INV_X3N_A10P5PP84TR_C14 (Y, A);
output Y;
input A;

  not I0(Y, A);

specify
(A => Y) = (0.004,0.004);

endspecify
endmodule // INV_X3N_A10P5PP84TR_C14
`endcelldefine



`celldefine
module INV_X2N_A10P5PP84TR_C14 (Y, A);
output Y;
input A;

  not I0(Y, A);

specify
(A => Y) = (0.004,0.004);

endspecify
endmodule // INV_X2N_A10P5PP84TR_C14
`endcelldefine



//----- Buffers
module BUF_X4N_A10P5PP84TR_C14 (Y, A);
output Y;
input A;

  buf I0(Y, A);



specify
(A => Y) = (0.017,0.017);

endspecify
endmodule // BUF_X4N_A10P5PP84TR_C14
`endcelldefine



`celldefine
module BUF_X3N_A10P5PP84TR_C14 (Y, A);
output Y;
input A;

  buf I0(Y, A);

specify
(A => Y) = (0.017,0.017);

endspecify
endmodule // BUF_X3N_A10P5PP84TR_C14
`endcelldefine



`celldefine
module BUF_X2N_A10P5PP84TR_C14 (Y, A);
output Y;
input A;

  buf I0(Y, A);

specify
(A => Y) = (0.018,0.018);

endspecify
endmodule // BUF_X2N_A10P5PP84TR_C14
`endcelldefine



`celldefine
module BUF_X1N_A10P5PP84TR_C14 (Y, A);
output Y;
input A;

  buf I0(Y, A);

specify
(A => Y) = (0.014,0.014);

endspecify
endmodule // BUF_X1N_A10P5PP84TR_C14
`endcelldefine



//----- Logic gates
`celldefine
module MX2_X1N_A10P5PP84TR_C14 (Y, A, B, S0);
output Y;
input A, B, S0;

  udp_mux2_sc9mcpp84_14lppxl_base_rvt_c14 u0(Y, A, B, S0);



specify
if (B==1'b1)
(A => Y) = (0.017, 0.017);
if (B==1'b0)
(A => Y) = (0.012, 0.012);
if (A==1'b1)
(B => Y) = (0.021, 0.021);
if (A==1'b0)
(B => Y) = (0.016, 0.016);
(posedge S0 => (Y:1'bx)) = (0.028, 0.028);
(negedge S0 => (Y:1'bx)) = (0.028, 0.028);

endspecify
endmodule // MX2_X1N_A10P5PP84TR_C14
`endcelldefine



`celldefine
module OR2_X1N_A10P5PP84TR_C14 (Y, A, B);
output Y;
input A, B;

  or (Y, A, B);


specify
(A => Y) = (0.021,0.021);
(B => Y) = (0.021,0.021);

endspecify
endmodule // OR2_X1N_A10P5PP84TR_C14
`endcelldefine



//----- FFs
`celldefine
module DFFRPQ_X1N_A10P5PP84TR_C14 (Q, CK, D, R);
output Q;
input  D, CK, R;
reg NOTIFIER;
supply1 xSN;

  not   XX0 (xRN, R);
  buf     IC (clk, CK);
  udp_dff_PWR_sc9mcpp84_14lppxl_base_rvt_c14 I0 (n0, D, clk, xRN, xSN, 1'b1, 1'b0, NOTIFIER);
  buf     I1 (Q, n0);

wire ENABLE_D_AND_NOT_R ;
wire ENABLE_NOT_D_AND_NOT_R ;
wire ENABLE_NOT_R ;
wire ENABLE_D ;
wire ENABLE_CK_AND_D ;
wire ENABLE_CK_AND_NOT_D ;
wire ENABLE_NOT_CK_AND_D ;
wire ENABLE_NOT_CK_AND_NOT_D ;
assign ENABLE_D_AND_NOT_R = (D&!R) ? 1'b1:1'b0;
assign ENABLE_NOT_D_AND_NOT_R = (!D&!R) ? 1'b1:1'b0;
assign ENABLE_NOT_R = (!R) ? 1'b1:1'b0;
assign ENABLE_D = (D) ? 1'b1:1'b0;
assign ENABLE_CK_AND_D = (CK&D) ? 1'b1:1'b0;
assign ENABLE_CK_AND_NOT_D = (CK&!D) ? 1'b1:1'b0;
assign ENABLE_NOT_CK_AND_D = (!CK&D) ? 1'b1:1'b0;
assign ENABLE_NOT_CK_AND_NOT_D = (!CK&!D) ? 1'b1:1'b0;

specify
$width(posedge CK &&& (ENABLE_D_AND_NOT_R == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(negedge CK &&& (ENABLE_D_AND_NOT_R == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge CK &&& (ENABLE_NOT_D_AND_NOT_R == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(negedge CK &&& (ENABLE_NOT_D_AND_NOT_R == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$setuphold(posedge CK &&& (ENABLE_NOT_R == 1'b1), posedge D, 0.020, 0.005, NOTIFIER);
$setuphold(posedge CK &&& (ENABLE_NOT_R == 1'b1), negedge D, 0.020, 0.005, NOTIFIER);
$recrem(negedge R, posedge CK &&& (ENABLE_D == 1'b1), `ARM_RECOVERY_TIME, `ARM_REMOVAL_TIME, NOTIFIER);
$width(posedge R &&& (ENABLE_CK_AND_D == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_CK_AND_NOT_D == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_NOT_CK_AND_D == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_NOT_CK_AND_NOT_D == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
(posedge CK => (Q:1'bx)) = (`ARM_PROP_DELAY,`ARM_PROP_DELAY);
if (CK==1'b1 && D==1'b1)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, 0.003);
if (CK==1'b1 && D==1'b0)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, 0.003);
if (CK==1'b0 && D==1'b1)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, 0.002);
if (CK==1'b0 && D==1'b0)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, 0.002);

endspecify
endmodule // DFFRPQ_X1N_A10P5PP84TR_C14
`endcelldefine



`celldefine
module SDFFRPQ_X1N_A10P5PP84TR_C14 (Q, CK, D, R, SE, SI);
output Q;
input D, SI, SE, CK, R;
reg NOTIFIER;
supply1 xSN;
supply1 dSN;

  not   XX0 (xRN, dR);
  buf     IC (clk, dCK);
  udp_dff_PWR_sc9mcpp84_14lppxl_base_rvt_c14 I0 (n0, n1, clk, xRN, xSN, 1'b1, 1'b0, NOTIFIER);
  udp_mux2_sc9mcpp84_14lppxl_base_rvt_c14 I1 (n1, dD, dSI, dSE);
  buf     I2 (Q, n0);

wire ENABLE_D_AND_NOT_R_AND_SE_AND_SI ;
wire ENABLE_D_AND_NOT_R_AND_SE_AND_NOT_SI ;
wire ENABLE_D_AND_NOT_R_AND_NOT_SE_AND_SI ;
wire ENABLE_D_AND_NOT_R_AND_NOT_SE_AND_NOT_SI ;
wire ENABLE_NOT_D_AND_NOT_R_AND_SE_AND_SI ;
wire ENABLE_NOT_D_AND_NOT_R_AND_SE_AND_NOT_SI ;
wire ENABLE_NOT_D_AND_NOT_R_AND_NOT_SE_AND_SI ;
wire ENABLE_NOT_D_AND_NOT_R_AND_NOT_SE_AND_NOT_SI ;
wire ENABLE_NOT_R_AND_NOT_SE_AND_SI ;
wire ENABLE_NOT_R_AND_NOT_SE_AND_NOT_SI ;
wire ENABLE_D_AND_SE_AND_SI ;
wire ENABLE_D_AND_NOT_SE_AND_SI ;
wire ENABLE_D_AND_NOT_SE_AND_NOT_SI ;
wire ENABLE_NOT_D_AND_SE_AND_SI ;
wire ENABLE_CK_AND_D_AND_SE_AND_SI ;
wire ENABLE_CK_AND_D_AND_SE_AND_NOT_SI ;
wire ENABLE_CK_AND_D_AND_NOT_SE_AND_SI ;
wire ENABLE_CK_AND_D_AND_NOT_SE_AND_NOT_SI ;
wire ENABLE_CK_AND_NOT_D_AND_SE_AND_SI ;
wire ENABLE_CK_AND_NOT_D_AND_SE_AND_NOT_SI ;
wire ENABLE_CK_AND_NOT_D_AND_NOT_SE_AND_SI ;
wire ENABLE_CK_AND_NOT_D_AND_NOT_SE_AND_NOT_SI ;
wire ENABLE_NOT_CK_AND_D_AND_SE_AND_SI ;
wire ENABLE_NOT_CK_AND_D_AND_SE_AND_NOT_SI ;
wire ENABLE_NOT_CK_AND_D_AND_NOT_SE_AND_SI ;
wire ENABLE_NOT_CK_AND_D_AND_NOT_SE_AND_NOT_SI ;
wire ENABLE_NOT_CK_AND_NOT_D_AND_SE_AND_SI ;
wire ENABLE_NOT_CK_AND_NOT_D_AND_SE_AND_NOT_SI ;
wire ENABLE_NOT_CK_AND_NOT_D_AND_NOT_SE_AND_SI ;
wire ENABLE_NOT_CK_AND_NOT_D_AND_NOT_SE_AND_NOT_SI ;
wire ENABLE_D_AND_NOT_R_AND_NOT_SI ;
wire ENABLE_NOT_D_AND_NOT_R_AND_SI ;
wire ENABLE_D_AND_NOT_R_AND_SE ;
wire ENABLE_NOT_D_AND_NOT_R_AND_SE ;
assign ENABLE_D_AND_NOT_R_AND_SE_AND_SI = (D&!R&SE&SI) ? 1'b1:1'b0;
assign ENABLE_D_AND_NOT_R_AND_SE_AND_NOT_SI = (D&!R&SE&!SI) ? 1'b1:1'b0;
assign ENABLE_D_AND_NOT_R_AND_NOT_SE_AND_SI = (D&!R&!SE&SI) ? 1'b1:1'b0;
assign ENABLE_D_AND_NOT_R_AND_NOT_SE_AND_NOT_SI = (D&!R&!SE&!SI) ? 1'b1:1'b0;
assign ENABLE_NOT_D_AND_NOT_R_AND_SE_AND_SI = (!D&!R&SE&SI) ? 1'b1:1'b0;
assign ENABLE_NOT_D_AND_NOT_R_AND_SE_AND_NOT_SI = (!D&!R&SE&!SI) ? 1'b1:1'b0;
assign ENABLE_NOT_D_AND_NOT_R_AND_NOT_SE_AND_SI = (!D&!R&!SE&SI) ? 1'b1:1'b0;
assign ENABLE_NOT_D_AND_NOT_R_AND_NOT_SE_AND_NOT_SI = (!D&!R&!SE&!SI) ? 1'b1:1'b0;
assign ENABLE_NOT_R_AND_NOT_SE_AND_SI = (!R&!SE&SI) ? 1'b1:1'b0;
assign ENABLE_NOT_R_AND_NOT_SE_AND_NOT_SI = (!R&!SE&!SI) ? 1'b1:1'b0;
assign ENABLE_D_AND_SE_AND_SI = (D&SE&SI) ? 1'b1:1'b0;
assign ENABLE_D_AND_NOT_SE_AND_SI = (D&!SE&SI) ? 1'b1:1'b0;
assign ENABLE_D_AND_NOT_SE_AND_NOT_SI = (D&!SE&!SI) ? 1'b1:1'b0;
assign ENABLE_NOT_D_AND_SE_AND_SI = (!D&SE&SI) ? 1'b1:1'b0;
assign ENABLE_CK_AND_D_AND_SE_AND_SI = (CK&D&SE&SI) ? 1'b1:1'b0;
assign ENABLE_CK_AND_D_AND_SE_AND_NOT_SI = (CK&D&SE&!SI) ? 1'b1:1'b0;
assign ENABLE_CK_AND_D_AND_NOT_SE_AND_SI = (CK&D&!SE&SI) ? 1'b1:1'b0;
assign ENABLE_CK_AND_D_AND_NOT_SE_AND_NOT_SI = (CK&D&!SE&!SI) ? 1'b1:1'b0;
assign ENABLE_CK_AND_NOT_D_AND_SE_AND_SI = (CK&!D&SE&SI) ? 1'b1:1'b0;
assign ENABLE_CK_AND_NOT_D_AND_SE_AND_NOT_SI = (CK&!D&SE&!SI) ? 1'b1:1'b0;
assign ENABLE_CK_AND_NOT_D_AND_NOT_SE_AND_SI = (CK&!D&!SE&SI) ? 1'b1:1'b0;
assign ENABLE_CK_AND_NOT_D_AND_NOT_SE_AND_NOT_SI = (CK&!D&!SE&!SI) ? 1'b1:1'b0;
assign ENABLE_NOT_CK_AND_D_AND_SE_AND_SI = (!CK&D&SE&SI) ? 1'b1:1'b0;
assign ENABLE_NOT_CK_AND_D_AND_SE_AND_NOT_SI = (!CK&D&SE&!SI) ? 1'b1:1'b0;
assign ENABLE_NOT_CK_AND_D_AND_NOT_SE_AND_SI = (!CK&D&!SE&SI) ? 1'b1:1'b0;
assign ENABLE_NOT_CK_AND_D_AND_NOT_SE_AND_NOT_SI = (!CK&D&!SE&!SI) ? 1'b1:1'b0;
assign ENABLE_NOT_CK_AND_NOT_D_AND_SE_AND_SI = (!CK&!D&SE&SI) ? 1'b1:1'b0;
assign ENABLE_NOT_CK_AND_NOT_D_AND_SE_AND_NOT_SI = (!CK&!D&SE&!SI) ? 1'b1:1'b0;
assign ENABLE_NOT_CK_AND_NOT_D_AND_NOT_SE_AND_SI = (!CK&!D&!SE&SI) ? 1'b1:1'b0;
assign ENABLE_NOT_CK_AND_NOT_D_AND_NOT_SE_AND_NOT_SI = (!CK&!D&!SE&!SI) ? 1'b1:1'b0;
assign ENABLE_D_AND_NOT_R_AND_NOT_SI = (D&!R&!SI) ? 1'b1:1'b0;
assign ENABLE_NOT_D_AND_NOT_R_AND_SI = (!D&!R&SI) ? 1'b1:1'b0;
assign ENABLE_D_AND_NOT_R_AND_SE = (D&!R&SE) ? 1'b1:1'b0;
assign ENABLE_NOT_D_AND_NOT_R_AND_SE = (!D&!R&SE) ? 1'b1:1'b0;

specify
$width(posedge CK &&& (ENABLE_D_AND_NOT_R_AND_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(negedge CK &&& (ENABLE_D_AND_NOT_R_AND_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge CK &&& (ENABLE_D_AND_NOT_R_AND_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(negedge CK &&& (ENABLE_D_AND_NOT_R_AND_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge CK &&& (ENABLE_D_AND_NOT_R_AND_NOT_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(negedge CK &&& (ENABLE_D_AND_NOT_R_AND_NOT_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge CK &&& (ENABLE_D_AND_NOT_R_AND_NOT_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(negedge CK &&& (ENABLE_D_AND_NOT_R_AND_NOT_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(negedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(negedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_NOT_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(negedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_NOT_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_NOT_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(negedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_NOT_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$setuphold(posedge CK &&& (ENABLE_NOT_R_AND_NOT_SE_AND_SI == 1'b1), posedge D, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dD);
$setuphold(posedge CK &&& (ENABLE_NOT_R_AND_NOT_SE_AND_SI == 1'b1), negedge D, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dD);
$setuphold(posedge CK &&& (ENABLE_NOT_R_AND_NOT_SE_AND_NOT_SI == 1'b1), posedge D, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dD);
$setuphold(posedge CK &&& (ENABLE_NOT_R_AND_NOT_SE_AND_NOT_SI == 1'b1), negedge D, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dD);
$recrem(negedge R, posedge CK &&& (ENABLE_D_AND_SE_AND_SI == 1'b1), `ARM_RECOVERY_TIME, `ARM_REMOVAL_TIME, NOTIFIER, , ,dR,dCK);
$recrem(negedge R, posedge CK &&& (ENABLE_D_AND_NOT_SE_AND_SI == 1'b1), `ARM_RECOVERY_TIME, `ARM_REMOVAL_TIME, NOTIFIER, , ,dR,dCK);
$recrem(negedge R, posedge CK &&& (ENABLE_D_AND_NOT_SE_AND_NOT_SI == 1'b1), `ARM_RECOVERY_TIME, `ARM_REMOVAL_TIME, NOTIFIER, , ,dR,dCK);
$recrem(negedge R, posedge CK &&& (ENABLE_NOT_D_AND_SE_AND_SI == 1'b1), `ARM_RECOVERY_TIME, `ARM_REMOVAL_TIME, NOTIFIER, , ,dR,dCK);
$width(posedge R &&& (ENABLE_CK_AND_D_AND_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_CK_AND_D_AND_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_CK_AND_D_AND_NOT_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_CK_AND_D_AND_NOT_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_CK_AND_NOT_D_AND_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_CK_AND_NOT_D_AND_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_CK_AND_NOT_D_AND_NOT_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_CK_AND_NOT_D_AND_NOT_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_NOT_CK_AND_D_AND_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_NOT_CK_AND_D_AND_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_NOT_CK_AND_D_AND_NOT_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_NOT_CK_AND_D_AND_NOT_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_NOT_CK_AND_NOT_D_AND_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_NOT_CK_AND_NOT_D_AND_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_NOT_CK_AND_NOT_D_AND_NOT_SE_AND_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$width(posedge R &&& (ENABLE_NOT_CK_AND_NOT_D_AND_NOT_SE_AND_NOT_SI == 1'b1), `ARM_WIDTH,`ARM_WIDTH_THD,NOTIFIER);
$setuphold(posedge CK &&& (ENABLE_D_AND_NOT_R_AND_NOT_SI == 1'b1), posedge SE, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dSE);
$setuphold(posedge CK &&& (ENABLE_D_AND_NOT_R_AND_NOT_SI == 1'b1), negedge SE, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dSE);
$setuphold(posedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_SI == 1'b1), posedge SE, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dSE);
$setuphold(posedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_SI == 1'b1), negedge SE, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dSE);
$setuphold(posedge CK &&& (ENABLE_D_AND_NOT_R_AND_SE == 1'b1), posedge SI, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dSI);
$setuphold(posedge CK &&& (ENABLE_D_AND_NOT_R_AND_SE == 1'b1), negedge SI, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dSI);
$setuphold(posedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_SE == 1'b1), posedge SI, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dSI);
$setuphold(posedge CK &&& (ENABLE_NOT_D_AND_NOT_R_AND_SE == 1'b1), negedge SI, `ARM_SETUP_TIME, `ARM_HOLD_TIME, NOTIFIER, , ,dCK,dSI);
if (D==1'b1 && SE==1'b1)
(posedge CK => (Q:1'bx)) = (`ARM_PROP_DELAY,`ARM_PROP_DELAY);
if (D==1'b1 && SE==1'b0 && SI==1'b1 || D==1'b0 && SE==1'b1 && SI==1'b0)
(posedge CK => (Q:1'bx)) = (`ARM_PROP_DELAY,`ARM_PROP_DELAY);
if (SE==1'b0 && SI==1'b0)
(posedge CK => (Q:1'bx)) = (`ARM_PROP_DELAY,`ARM_PROP_DELAY);
if (D==1'b0 && SI==1'b1)
(posedge CK => (Q:1'bx)) = (`ARM_PROP_DELAY,`ARM_PROP_DELAY);
if (CK==1'b1 && D==1'b1 && SE==1'b1 && SI==1'b1)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b1 && D==1'b1 && SE==1'b1 && SI==1'b0)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b1 && D==1'b1 && SE==1'b0 && SI==1'b1)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b1 && D==1'b1 && SE==1'b0 && SI==1'b0)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b1 && D==1'b0 && SE==1'b1 && SI==1'b1)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b1 && D==1'b0 && SE==1'b1 && SI==1'b0)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b1 && D==1'b0 && SE==1'b0 && SI==1'b1)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b1 && D==1'b0 && SE==1'b0 && SI==1'b0)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b0 && D==1'b1 && SE==1'b1 && SI==1'b1)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b0 && D==1'b1 && SE==1'b1 && SI==1'b0)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b0 && D==1'b1 && SE==1'b0 && SI==1'b1)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b0 && D==1'b1 && SE==1'b0 && SI==1'b0)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b0 && D==1'b0 && SE==1'b1 && SI==1'b1)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b0 && D==1'b0 && SE==1'b1 && SI==1'b0)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b0 && D==1'b0 && SE==1'b0 && SI==1'b1)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);
if (CK==1'b0 && D==1'b0 && SE==1'b0 && SI==1'b0)
(posedge R *> (Q +: 1'b0))=(`ARM_INVALID_DELAY, `ARM_PROP_DELAY);

endspecify
endmodule // SDFFRPQ_X1N_A10P5PP84TR_C14
`endcelldefine



//----- Adder
`celldefine
module ADDF_X1N_A10P5PP84TR_C14 (CO, S, A, B, CI);
output S, CO;
input A, B, CI;
  xor I0(S, A, B, CI);
  and I1(a_and_b, A, B);
  and I2(a_and_ci, A, CI);
  and I3(b_and_ci, B, CI);
  or I4(CO, a_and_b, a_and_ci, b_and_ci);


specify
if (B==1'b1 && CI==1'b0)
(A => CO) = (0.034,0.034);
if (B==1'b0 && CI==1'b1)
(A => CO) = (0.031,0.031);
if (A==1'b1 && CI==1'b0)
(B => CO) = (0.035,0.035);
if (A==1'b0 && CI==1'b1)
(B => CO) = (0.033,0.033);
if (A==1'b1 && B==1'b0)
(CI => CO) = (0.031,0.031);
if (A==1'b0 && B==1'b1)
(CI => CO) = (0.031,0.031);
if (B==1'b1 && CI==1'b1)
(A => S) = (0.032,0.032);
if (B==1'b0 && CI==1'b0)
(A => S) = (0.031,0.031);
if (B==1'b1 && CI==1'b0)
(A => S) = (0.053,0.053);
if (B==1'b0 && CI==1'b1)
(A => S) = (0.051,0.051);
if (A==1'b1 && CI==1'b1)
(B => S) = (0.034,0.034);
if (A==1'b0 && CI==1'b0)
(B => S) = (0.032,0.032);
if (A==1'b1 && CI==1'b0)
(B => S) = (0.052,0.052);
if (A==1'b0 && CI==1'b1)
(B => S) = (0.051,0.051);
if (A==1'b1 && B==1'b1)
(CI => S) = (0.033,0.033);
if (A==1'b0 && B==1'b0)
(CI => S) = (0.031,0.031);
if (A==1'b1 && B==1'b0)
(CI => S) = (0.048,0.048);
if (A==1'b0 && B==1'b1)
(CI => S) = (0.050,0.050);

endspecify
endmodule // ADDF_X1N_A10P5PP84TR_C14
`endcelldefine



//----- IOs
`celldefine
module PBIDIR_18_18_NT_DR_V (
		PO,
		Y,
		PAD,
		A,
		DS0,
		DS1,
		IE,
		IS,
		OE,
		PE,
		POE,
		PS,
		RTO,
		SNS,
		SR
 );

   inout PAD;
   output Y;
   output PO;
   input A;
   input OE;
   input DS0;
   input DS1;
   input SR;
   input PS;
   input PE;
   input IE;
   input IS;
   input POE;
   input RTO;
   input SNS;

   tri PAD;
   tri P_i;
   wire PU;
   wire PD;
   wire A_LAT;
   wire PS_LAT;
   wire PE_LAT;
   wire OE_LAT;
   wire IE_LAT;
   wire PAD_reg;
   wire PO_reg;
   wire Y_reg;
   wire DVDD_wire;
   wire PAD_out;
   wire PO_out;
   wire EN_pwr;
   wire Y_out;
   reg NOTIFIER;
   reg PU_EN=0;
   reg PD_EN=0;

   supply0 su0;
   supply1 su1;

   assign DVDD_wire = 1'b1;

   //latches for A, DS0, DS1,SR, OE & IE
   udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr ua(A_LAT, DVDD_wire, A, RTO, NOTIFIER);
   udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr up(PS_LAT, DVDD_wire, PS, RTO, NOTIFIER);
   udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr ue(PE_LAT, DVDD_wire, PE, RTO, NOTIFIER);
   udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr uo(OE_LAT, DVDD_wire, OE, RTO, NOTIFIER);
   udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr ui(IE_LAT, DVDD_wire, IE, RTO, NOTIFIER);
   //Pull UP/DOWN logic
   and u0 (DRV_EN, OE_LAT, SNS);
   not u1 (OEN_LAT, OE_LAT);
   not u2 (PSN_LAT, PS_LAT);
   and I2 (PU, PE_LAT, PS_LAT,  OEN_LAT, SNS);
   and I3 (PD, PE_LAT, PSN_LAT, OEN_LAT, SNS);
   rnmos  u4(P_i, su1, PU_EN);
   rnmos  u5(P_i, su0, PD_EN);
   bufif1 u6(P_i, A_LAT, DRV_EN);

   //logic for driver & receiver
   nmos    u7(PAD_reg, P_i, 1'b1);
   and     u8(Y_reg, PAD, IE_LAT, SNS);
   nand    u9(PO_reg, POE, Y_out, RTO);
   assign EN_pwr  = DVDD_wire && !(RTO === 1'bx || RTO === 1'bz)
                         && !(SNS === 1'bx || SNS === 1'bz);
   //assign PAD_out = (EN_pwr) ? PAD_reg : 1'bx;
   nmos u11(PAD_out, PAD_reg, EN_pwr);
   nmos u12(PAD_out, 1'bx, !EN_pwr);
   assign PO_out  = (EN_pwr && SNS) ? PO_reg : 1'bx;
   assign Y_out   = (EN_pwr) ? Y_reg : 1'bx;
   nmos ux(PAD, PAD_out, 1'b1);
   nmos uy(PO, PO_out, 1'b1);
   nmos uz(Y, Y_out, 1'b1);

   always @(PU)
   begin
	   if (PU===1 && PAD!==1 && SNS===1 && RTO===1)
		   PU_EN <= #(`PULL) PU;
	   else if (PU===0 && DRV_EN===0 && SNS===1 && RTO===1)
		   PU_EN <= #(`PULL) PU;
	   else
		   PU_EN <= PU;
   end

   always @(PD)
   begin
	   if (PD===1 && PAD!==0 && SNS===1 && RTO===1)
		   PD_EN <= #(`PULL) PD;
	   else if (PD===0 && DRV_EN===0 && SNS===1 && RTO===1)
		   PD_EN <= #(`PULL) PD;
	   else
		   PD_EN <= PD;
   end



specify
if (IS == 1'b1 && OE == 1'b0 && PAD == 1'b1)
(IE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b0 && OE == 1'b0 && PAD == 1'b1)
(IE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1 && OE == 1'b0 && POE == 1'b1)
(PAD => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0 && OE == 1'b0 && POE == 1'b1)
(PAD => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1 && OE == 1'b0 && PAD == 1'b1)
(POE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0 && OE == 1'b0 && PAD == 1'b1)
(POE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b1 && OE == 1'b0 && PAD == 1'b1)
(IE => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b0 && OE == 1'b0 && PAD == 1'b1)
(IE => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1 && OE == 1'b0)
(PAD => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0 && OE == 1'b0)
(PAD => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b1 && SR == 1'b1)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b1 && SR == 1'b0)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b0 && SR == 1'b1)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b0 && SR == 1'b0)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b1 && SR == 1'b1)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b1 && SR == 1'b0)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b0 && SR == 1'b1)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b0 && SR == 1'b0)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b1 && SR == 1'b1)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b1 && SR == 1'b0)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b0 && SR == 1'b1)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b0 && SR == 1'b0)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b1 && SR == 1'b1)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b1 && SR == 1'b0)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b0 && SR == 1'b1)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b0 && SR == 1'b0)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);

endspecify
endmodule // PBIDIR_18_18_NT_DR_V
`endcelldefine

`celldefine
module PBIDIR_18_18_NT_DR_H (
		PO,
		Y,
		PAD,
		A,
		DS0,
		DS1,
		IE,
		IS,
		OE,
		PE,
		POE,
		PS,
		RTO,
		SNS,
		SR
 );

   inout PAD;
   output Y;
   output PO;
   input A;
   input OE;
   input DS0;
   input DS1;
   input SR;
   input PS;
   input PE;
   input IE;
   input IS;
   input POE;
   input RTO;
   input SNS;

   tri PAD;
   tri P_i;
   wire PU;
   wire PD;
   wire A_LAT;
   wire PS_LAT;
   wire PE_LAT;
   wire OE_LAT;
   wire IE_LAT;
   wire PAD_reg;
   wire PO_reg;
   wire Y_reg;
   wire DVDD_wire;
   wire PAD_out;
   wire PO_out;
   wire EN_pwr;
   wire Y_out;
   reg NOTIFIER;
   reg PU_EN=0;
   reg PD_EN=0;

   supply0 su0;
   supply1 su1;

   assign DVDD_wire = 1'b1;

   //latches for A, DS0, DS1,SR, OE & IE
   udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr ua(A_LAT, DVDD_wire, A, RTO, NOTIFIER);
   udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr up(PS_LAT, DVDD_wire, PS, RTO, NOTIFIER);
   udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr ue(PE_LAT, DVDD_wire, PE, RTO, NOTIFIER);
   udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr uo(OE_LAT, DVDD_wire, OE, RTO, NOTIFIER);
   udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr ui(IE_LAT, DVDD_wire, IE, RTO, NOTIFIER);
   //Pull UP/DOWN logic
   and u0 (DRV_EN, OE_LAT, SNS);
   not u1 (OEN_LAT, OE_LAT);
   not u2 (PSN_LAT, PS_LAT);
   and I2 (PU, PE_LAT, PS_LAT,  OEN_LAT, SNS);
   and I3 (PD, PE_LAT, PSN_LAT, OEN_LAT, SNS);
   rnmos  u4(P_i, su1, PU_EN);
   rnmos  u5(P_i, su0, PD_EN);
   bufif1 u6(P_i, A_LAT, DRV_EN);

   //logic for driver & receiver
   nmos    u7(PAD_reg, P_i, 1'b1);
   and     u8(Y_reg, PAD, IE_LAT, SNS);
   nand    u9(PO_reg, POE, Y_out, RTO);
   assign EN_pwr  = DVDD_wire && !(RTO === 1'bx || RTO === 1'bz)
                         && !(SNS === 1'bx || SNS === 1'bz);
   //assign PAD_out = (EN_pwr) ? PAD_reg : 1'bx;
   nmos u11(PAD_out, PAD_reg, EN_pwr);
   nmos u12(PAD_out, 1'bx, !EN_pwr);
   assign PO_out  = (EN_pwr && SNS) ? PO_reg : 1'bx;
   assign Y_out   = (EN_pwr) ? Y_reg : 1'bx;
   nmos ux(PAD, PAD_out, 1'b1);
   nmos uy(PO, PO_out, 1'b1);
   nmos uz(Y, Y_out, 1'b1);

   always @(PU)
   begin
	   if (PU===1 && PAD!==1 && SNS===1 && RTO===1)
		   PU_EN <= #(`PULL) PU;
	   else if (PU===0 && DRV_EN===0 && SNS===1 && RTO===1)
		   PU_EN <= #(`PULL) PU;
	   else
		   PU_EN <= PU;
   end

   always @(PD)
   begin
	   if (PD===1 && PAD!==0 && SNS===1 && RTO===1)
		   PD_EN <= #(`PULL) PD;
	   else if (PD===0 && DRV_EN===0 && SNS===1 && RTO===1)
		   PD_EN <= #(`PULL) PD;
	   else
		   PD_EN <= PD;
   end



specify
if (IS == 1'b1 && OE == 1'b0 && PAD == 1'b1)
(IE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b0 && OE == 1'b0 && PAD == 1'b1)
(IE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1 && OE == 1'b0 && POE == 1'b1)
(PAD => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0 && OE == 1'b0 && POE == 1'b1)
(PAD => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1 && OE == 1'b0 && PAD == 1'b1)
(POE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0 && OE == 1'b0 && PAD == 1'b1)
(POE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b1 && OE == 1'b0 && PAD == 1'b1)
(IE => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b0 && OE == 1'b0 && PAD == 1'b1)
(IE => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1 && OE == 1'b0)
(PAD => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0 && OE == 1'b0)
(PAD => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b1 && SR == 1'b1)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b1 && SR == 1'b0)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b0 && SR == 1'b1)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b0 && SR == 1'b0)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b1 && SR == 1'b1)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b1 && SR == 1'b0)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b0 && SR == 1'b1)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b0 && SR == 1'b0)
(A => PAD) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b1 && SR == 1'b1)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b1 && SR == 1'b0)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b0 && SR == 1'b1)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b1 && DS1 == 1'b0 && SR == 1'b0)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b1 && SR == 1'b1)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b1 && SR == 1'b0)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b0 && SR == 1'b1)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (DS0 == 1'b0 && DS1 == 1'b0 && SR == 1'b0)
( OE => PAD ) = (`ARM_PROP_IODELAY, `ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);

endspecify
endmodule // PBIDIR_18_18_NT_DR_H
`endcelldefine


`celldefine
`ifdef POWER_PINS
module PCORNER_18_18_NT_DR (
		DVDD,
		DVSS,
		VDD,
		VSS,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PCORNER_18_18_NT_DR (
		RTO,
		SNS
 );

`endif
   input SNS;
   input RTO;

specify

endspecify
endmodule // PCORNER_18_18_NT_DR
`endcelldefine



`celldefine
`ifdef POWER_PINS
module PDVDDTIE_18_18_NT_DR_H (
		RTO,
		SNS,
		DVDD,
		DVSS,
		VDD,
		VSS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PDVDDTIE_18_18_NT_DR_H (
		RTO,
		SNS
 );

`endif

    output RTO;
    output SNS;
`ifdef POWER_PINS
    assign SNS = (DVDD == 1'b1) ? 1'b1 : 1'b0;
    assign RTO = (DVDD == 1'b1) ? 1'b1: 1'b0;
`else
    // Both sns and rto are tied high
    buf (SNS, 1'b1);
    buf (RTO, 1'b1);
`endif


specify

endspecify
endmodule // PDVDDTIE_18_18_NT_DR_H
`endcelldefine

`celldefine
`ifdef POWER_PINS
module PDVDDTIE_18_18_NT_DR_V (
		RTO,
		SNS,
		DVDD,
		DVSS,
		VDD,
		VSS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PDVDDTIE_18_18_NT_DR_V (
		RTO,
		SNS
 );

`endif

    output RTO;
    output SNS;
`ifdef POWER_PINS
    assign SNS = (DVDD == 1'b1) ? 1'b1 : 1'b0;
    assign RTO = (DVDD == 1'b1) ? 1'b1: 1'b0;
`else
    // Both sns and rto are tied high
    buf (SNS, 1'b1);
    buf (RTO, 1'b1);
`endif


specify

endspecify
endmodule // PDVDDTIE_18_18_NT_DR_V
`endcelldefine


`celldefine
`ifdef POWER_PINS
module PDVDD_18_18_NT_DR_H (
		DVDD,
		DVSS,
		VDD,
		VSS,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PDVDD_18_18_NT_DR_H (
		RTO,
		SNS
 );

`endif

   input SNS;
   input RTO;

specify

endspecify
endmodule // PDVDD_18_18_NT_DR_H
`endcelldefine



`celldefine
`ifdef POWER_PINS
module PDVSS_18_18_NT_DR_H (
		DVDD,
		DVSS,
		VDD,
		VSS,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PDVSS_18_18_NT_DR_H (
		RTO,
		SNS
 );

`endif

   input SNS;
   input RTO;

specify

endspecify
endmodule // PDVSS_18_18_NT_DR_H
`endcelldefine



`celldefine
`ifdef POWER_PINS
module PDVDD_18_18_NT_DR_V (
		DVDD,
		DVSS,
		VDD,
		VSS,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PDVDD_18_18_NT_DR_V (
		RTO,
		SNS
 );

`endif

   input SNS;
   input RTO;

specify

endspecify
endmodule // PDVDD_18_18_NT_DR_V
`endcelldefine



`celldefine
`ifdef POWER_PINS
module PDVSS_18_18_NT_DR_V (
		DVDD,
		DVSS,
		VDD,
		VSS,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PDVSS_18_18_NT_DR_V (
		RTO,
		SNS
 );

`endif

   input SNS;
   input RTO;

specify

endspecify
endmodule // PDVSS_18_18_NT_DR_V
`endcelldefine



`celldefine
`ifdef POWER_PINS
module PDCAP10_18_18_NT_DR_H (
		DVDD,
		DVSS,
		VDD,
		VSS,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PDCAP10_18_18_NT_DR_H (
		RTO,
		SNS
 );

`endif
   input SNS;
   input RTO;

specify

endspecify
endmodule // PDCAP10_18_18_NT_DR_H
`endcelldefine
`celldefine
`ifdef POWER_PINS
module PDCAP10_18_18_NT_DR_V (
		DVDD,
		DVSS,
		VDD,
		VSS,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PDCAP10_18_18_NT_DR_V (
		RTO,
		SNS
 );

`endif
   input SNS;
   input RTO;

specify

endspecify
endmodule // PDCAP10_18_18_NT_DR_V
`endcelldefine



`celldefine
`ifdef POWER_PINS
module PINCNP_18_18_NT_DR_H (
		PO,
		Y,
		DVDD,
		DVSS,
		VDD,
		VSS,
		IE,
		IS,
		PAD,
		POE,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PINCNP_18_18_NT_DR_H (
		PO,
		Y,
		IE,
		IS,
		PAD,
		POE,
		RTO,
		SNS
 );

`endif

   output Y;
   output PO;
   input PAD;
   input IE;
   input IS;
   input POE;
   input RTO;
   input SNS;

   wire PO_int;
   wire Y_int;
   wire DVDD_wire;
   wire EN_pwr;
   wire PO_out;
   wire Y_out;

   //primitive for output enable
   `ifdef POWER_PINS
      assign DVDD_wire = DVDD;
   `else
      assign DVDD_wire = 1'b1;
   `endif

   assign EN_pwr  = DVDD_wire && !(SNS === 1'bx || SNS === 1'bz);
   assign PO_out  = (EN_pwr && SNS) ? PO_int : 1'bx;
   assign Y_out   = (EN_pwr) ? Y_int : 1'bx;
   //logic for  receiver
   and     u1(Y_int, PAD, IE, SNS);
   nand    u2(PO_int, POE, Y_out);
   nmos    u3(PO, PO_out, 1'b1);
   nmos    u4(Y, Y_out, 1'b1);



specify
if (IS == 1'b1 && PAD == 1'b1)
(IE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b0 && PAD == 1'b1)
(IE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1 && POE == 1'b1)
(PAD => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0 && POE == 1'b1)
(PAD => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1 && PAD == 1'b1)
(POE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0 && PAD == 1'b1)
(POE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b1 && PAD == 1'b1)
(IE => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b0 && PAD == 1'b1)
(IE => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1)
(PAD => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0)
(PAD => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);

endspecify
endmodule // PINCNP_18_18_NT_DR_H
`endcelldefine
`celldefine
`ifdef POWER_PINS
module PINCNP_18_18_NT_DR_V (
		PO,
		Y,
		DVDD,
		DVSS,
		VDD,
		VSS,
		IE,
		IS,
		PAD,
		POE,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PINCNP_18_18_NT_DR_V (
		PO,
		Y,
		IE,
		IS,
		PAD,
		POE,
		RTO,
		SNS
 );

`endif

   output Y;
   output PO;
   input PAD;
   input IE;
   input IS;
   input POE;
   input RTO;
   input SNS;

   wire PO_int;
   wire Y_int;
   wire DVDD_wire;
   wire EN_pwr;
   wire PO_out;
   wire Y_out;

   //primitive for output enable
   `ifdef POWER_PINS
      assign DVDD_wire = DVDD;
   `else
      assign DVDD_wire = 1'b1;
   `endif

   assign EN_pwr  = DVDD_wire && !(SNS === 1'bx || SNS === 1'bz);
   assign PO_out  = (EN_pwr && SNS) ? PO_int : 1'bx;
   assign Y_out   = (EN_pwr) ? Y_int : 1'bx;
   //logic for  receiver
   and     u1(Y_int, PAD, IE, SNS);
   nand    u2(PO_int, POE, Y_out);
   nmos    u3(PO, PO_out, 1'b1);
   nmos    u4(Y, Y_out, 1'b1);



specify
if (IS == 1'b1 && PAD == 1'b1)
(IE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b0 && PAD == 1'b1)
(IE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1 && POE == 1'b1)
(PAD => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0 && POE == 1'b1)
(PAD => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1 && PAD == 1'b1)
(POE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0 && PAD == 1'b1)
(POE => PO) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b1 && PAD == 1'b1)
(IE => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IS == 1'b0 && PAD == 1'b1)
(IE => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b1)
(PAD => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);
if (IE == 1'b1 && IS == 1'b0)
(PAD => Y) = (`ARM_PROP_IODELAY,`ARM_PROP_IODELAY);

endspecify
endmodule // PINCNP_18_18_NT_DR_V
`endcelldefine



`celldefine
`ifdef POWER_PINS
module PVDD_08_08_NT_DR_H (
		DVDD,
		DVSS,
		VDD,
		VSS,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PVDD_08_08_NT_DR_H (
		RTO,
		SNS
 );

`endif

   input SNS;
   input RTO;

specify

endspecify
endmodule // PVDD_08_08_NT_DR_H
`endcelldefine
`celldefine
`ifdef POWER_PINS
module PVDD_08_08_NT_DR_V (
		DVDD,
		DVSS,
		VDD,
		VSS,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PVDD_08_08_NT_DR_V (
		RTO,
		SNS
 );

`endif

   input SNS;
   input RTO;

specify

endspecify
endmodule // PVDD_08_08_NT_DR_V
`endcelldefine



`celldefine
`ifdef POWER_PINS
module PVSS_08_08_NT_DR_H (
		DVDD,
		DVSS,
		VDD,
		VSS,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PVSS_08_08_NT_DR_H (
		RTO,
		SNS
 );

`endif

   input SNS;
   input RTO;

specify

endspecify
endmodule // PVSS_08_08_NT_DR_H
`endcelldefine
`celldefine
`ifdef POWER_PINS
module PVSS_08_08_NT_DR_V (
		DVDD,
		DVSS,
		VDD,
		VSS,
		RTO,
		SNS
 );

inout DVDD, VDD;
inout DVSS, VSS;
`else
module PVSS_08_08_NT_DR_V (
		RTO,
		SNS
 );

`endif

   input SNS;
   input RTO;

specify

endspecify
endmodule // PVSS_08_08_NT_DR_V
`endcelldefine



`celldefine
module PBP50_18_18_NT_DR (
		PAD
 );

   inout PAD;

specify

endspecify
endmodule // PBP50_18_18_NT_DR
`endcelldefine



//----- Primitives
primitive udp_mux2_sc9mcpp84_14lppxl_base_rvt_c14 (out, in0, in1, sel);
   output out;
   input  in0, in1, sel;

   table

// in0 in1  sel :  out
//
   1  ?   0  :  1 ;
   0  ?   0  :  0 ;
   ?  1   1  :  1 ;
   ?  0   1  :  0 ;
   0  0   x  :  0 ;
   1  1   x  :  1 ;

   endtable
endprimitive // udp_mux2_sc9mcpp84_14lppxl_base_rvt_c14



primitive udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr (out, dvdd, in, hold, NOTIFIER);
   output out;
   input  dvdd, in, hold, NOTIFIER;
   reg    out;

   table

// dvdd in hold NOT : Qt : Qt+1
//
   1  1  1   ?    : ?  :  1  ; //
   1  0  1   ?    : ?  :  0  ; //
   1  ?  0   ?    : ?  :  -  ; // with pessimism
   0  ?  ?   ?    : ?  :  x  ; // no power
   x  ?  ?   ?    : ?  :  x  ; // unknown power
   ?  ?  ?   *    : ?  :  x  ; // any notifier changed

   endtable
endprimitive // udp_lat_rto_io_gppr_14lppxl_t18_mv10_mv18_fs18_rvt_dr



primitive udp_dff_PWR_sc9mcpp84_14lppxl_base_rvt_c14 (out, in, clk, clr_, set_, VDD, VSS, NOTIFIER);
   output out;
   input  in, clk, clr_, set_, VDD, VSS, NOTIFIER;
   reg    out;

   table

// in  clk  clr_   set_  VDD  VSS  NOTIFIER  : Qt : Qt+1
//
   0  r   ?   1   1  0  ?  : ?  :  0  ; // clock in 0
   1  r   1   ?   1  0  ?  : ?  :  1  ; // clock in 1
   1  *   1   ?   1  0  ?  : 1  :  1  ; // reduce pessimism
   0  *   ?   1   1  0  ?  : 0  :  0  ; // reduce pessimism
   ?  f   ?   ?   1  0  ?  : ?  :  -  ; // no changes on negedge clk
   *  b   ?   ?   1  0  ?  : ?  :  -  ; // no changes when in switches
   ?  ?   ?   0   1  0  ?  : ?  :  1  ; // set output
   ?  b   1   *   1  0  ?  : 1  :  1  ; // cover all transistions on set_
   1  x   1   *   1  0  ?  : 1  :  1  ; // cover all transistions on set_
   ?  ?   0   1   1  0  ?  : ?  :  0  ; // reset output
   ?  b   *   1   1  0  ?  : 0  :  0  ; // cover all transistions on clr_
   0  x   *   1   1  0  ?  : 0  :  0  ; // cover all transistions on clr_
   ?  ?   ?   ?   ?  ?  *  : ?  :  x  ; // any notifier changed

   endtable
endprimitive // udp_dff_sc9mcpp84_14lppxl_base_rvt_c14
