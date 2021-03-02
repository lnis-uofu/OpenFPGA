/***************************************************
 Vendor          : QuickLogic Corp.
 Aurora Version  : AU 1.0.0 			
 File Name       : primtive_macros.v			
 Author          : Kishor Kumar 
 Description     : Verilog Netlist File (For Synthesis/Pre-Layout Simulation)									
*****************************************************/

/*-------------------------------------------------------------------------------
 MODULE NAME : ck_buff 
 DESCRIPTION : Clock tree buffer						
--------------------------------------------------------------------------------*/
`ifdef ck_buff
`else
`define ck_buff
module ck_buff ( A , Q )/* synthesis black_box black_box_pad_pin = "A" */;
input A/* synthesis syn_isclock=1 */;
output Q;
//pragma synthesis_off
//pragma synthesis_on
endmodule /* ck buff */
`endif

/*-------------------------------------------------------------------------------
 MODULE NAME : gclkbuff 
 DESCRIPTION : Global clock buffer						
--------------------------------------------------------------------------------*/
`ifdef GCLKBUFF
`else
`define GCLKBUFF
module GCLKBUFF ( A , Z )/* synthesis black_box */;
input A;
output Z;
//pragma synthesis_off
//pragma synthesis_on
endmodule /* gclk buff */
`endif

/*-------------------------------------------------------------------------------
 MODULE NAME : in_buff 
 DESCRIPTION : Input buffer					
--------------------------------------------------------------------------------*/
`ifdef in_buff
`else
`define in_buff
module in_buff ( A , Q )/* synthesis black_box */;
input A;
output Q;
//pragma synthesis_off
//pragma synthesis_on
endmodule /* in buff */
`endif

/*-------------------------------------------------------------------------------
 MODULE NAME : out_buff 
 DESCRIPTION : Output buffer					
--------------------------------------------------------------------------------*/
`ifdef out_buff
`else
`define out_buff
module out_buff ( A , Q )/* synthesis black_box */;
input A;
output Q;
//pragma synthesis_off
//pragma synthesis_on
endmodule /* out buff */
`endif

/*-------------------------------------------------------------------------------
 MODULE NAME : inv 
 DESCRIPTION : Inverter						
--------------------------------------------------------------------------------*/
`ifdef inv
`else
`define inv
module inv ( A , Q )/* synthesis black_box */;
input A;
output Q;
//pragma synthesis_off
//pragma synthesis_on
endmodule /*inverter*/
`endif

/*-------------------------------------------------------------------------------
 MODULE NAME : buff 
 DESCRIPTION : Buffer 					
--------------------------------------------------------------------------------*/
`ifdef buff
`else
`define buff
module buff ( A , Q )/* synthesis black_box */;
input A;
output Q;
//pragma synthesis_off
//pragma synthesis_on
endmodule /*buffer*/
`endif

/*-------------------------------------------------------------------------------
 MODULE NAME : mux2x0 
 DESCRIPTION : Basic Mux 2:1					
--------------------------------------------------------------------------------*/
`ifdef mux2x0
`else
`define mux2x0
module mux2x0 ( A , B, S, Q )/* synthesis black_box */;
input A, B, S;
output Q;
//pragma synthesis_off
//pragma synthesis_on
endmodule /*mux2x0*/
`endif

`ifdef LOGIC_Cell
`else
`define LOGIC_Cell
module LOGIC_Cell (T0I0, T0I1, T0I2, T0I3, B0I0, B0I1, B0I2, B0I3, 
            TB0S, Q0DI, CD0S, Q0EN, QST, QRT, QCK, QCKS, C0Z, Q0Z, B0Z)/* synthesis black_box */;
input T0I0, T0I1, T0I2, T0I3, B0I0, B0I1, B0I2, B0I3;
input TB0S, Q0DI, CD0S, Q0EN;				  
input QST, QRT, QCK, QCKS;
output C0Z, B0Z, Q0Z;
//pragma synthesis_off
//pragma synthesis_on
endmodule /*LOGIC_Cell*/
`endif
