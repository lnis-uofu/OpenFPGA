//////////////////////////////////////////////////////////////////////
////                                                              ////
////  DES Round                                                   ////
////                                                              ////
////  This file is part of the SystemC DES                        ////
////                                                              ////
////  Description:                                                ////
////  Performs a round of DES algorithm                           ////
////                                                              ////
////  Generated automatically using SystemC to Verilog translator ////
////                                                              ////
////  To Do:                                                      ////
////   - done                                                     ////
////                                                              ////
////  Author(s):                                                  ////
////      - Javier Castillo, jcastilo@opencores.org               ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Authors and OPENCORES.ORG                 ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//
// CVS Revision History
//
// $Log: desround.v,v $
// Revision 1.1.1.1  2004/07/05 17:31:16  jcastillo
// First import
//


module desround(clk,reset,iteration_i,decrypt_i,R_i,L_i,Key_i,R_o,L_o,Key_o,s1_o,s2_o,s3_o,s4_o,s5_o,s6_o,s7_o,s8_o,s1_i,s2_i,s3_i,s4_i,s5_i,s6_i,s7_i,s8_i);

input clk;
input reset;
input [3:0] iteration_i;
input decrypt_i;
input [31:0] R_i;
input [31:0] L_i;
input [55:0] Key_i;
output [31:0] R_o;
output [31:0] L_o;
output [55:0] Key_o;
output [5:0] s1_o;
output [5:0] s2_o;
output [5:0] s3_o;
output [5:0] s4_o;
output [5:0] s5_o;
output [5:0] s6_o;
output [5:0] s7_o;
output [5:0] s8_o;
input [3:0] s1_i;
input [3:0] s2_i;
input [3:0] s3_i;
input [3:0] s4_i;
input [3:0] s5_i;
input [3:0] s6_i;
input [3:0] s7_i;
input [3:0] s8_i;

reg [31:0] R_o;
reg [31:0] L_o;
reg [55:0] Key_o;
reg [5:0] s1_o;
reg [5:0] s2_o;
reg [5:0] s3_o;
reg [5:0] s4_o;
reg [5:0] s5_o;
reg [5:0] s6_o;
reg [5:0] s7_o;
reg [5:0] s8_o;



reg  [55:0] previous_key;

reg [3:0] iteration;

reg decrypt;



wire [55:0] non_perm_key;



wire [47:0] new_key;

reg [31:0] next_R;

reg [31:0] expanRSig;

  reg[47:0]  expandedR;
  reg[47:0]  round_key;
  reg[47:0]  KER;
  reg[31:0]  R_i_var;
	
  reg[31:0]  Soutput;
  reg[31:0]  f;	


key_gen kg1 (.previous_key(previous_key), .iteration(iteration), .decrypt(decrypt), .new_key(new_key), .non_perm_key(non_perm_key));

always @(posedge clk or negedge reset)

begin

	
	if(!reset)
    begin

		L_o = (0);
		R_o = (0);
		Key_o = (0);
	
    end
    else
    begin

        L_o = (R_i);
		R_o = (next_R);
	    Key_o = (non_perm_key);
	
    end

end

always @(  R_i or   L_i or   Key_i or   iteration_i or   decrypt_i or   new_key or   s1_i or   s2_i or   s3_i or   s4_i or   s5_i or   s6_i or   s7_i or   s8_i)

begin

  R_i_var=R_i;	
	

  expandedR[47]=R_i_var[0]; expandedR[46]=R_i_var[31]; expandedR[45]=R_i_var[30]; expandedR[44]=R_i_var[29];
  expandedR[43]=R_i_var[28]; expandedR[42]=R_i_var[27]; expandedR[41]=R_i_var[28]; expandedR[40]=R_i_var[27];
	
  expandedR[39]=R_i_var[26]; expandedR[38]=R_i_var[25]; expandedR[37]=R_i_var[24]; expandedR[36]=R_i_var[23];
  expandedR[35]=R_i_var[24]; expandedR[34]=R_i_var[23]; expandedR[33]=R_i_var[22]; expandedR[32]=R_i_var[21];
  
  expandedR[31]=R_i_var[20]; expandedR[30]=R_i_var[19]; expandedR[29]=R_i_var[20]; expandedR[28]=R_i_var[19];
  expandedR[27]=R_i_var[18]; expandedR[26]=R_i_var[17]; expandedR[25]=R_i_var[16]; expandedR[24]=R_i_var[15];
	
  expandedR[23]=R_i_var[16]; expandedR[22]=R_i_var[15]; expandedR[21]=R_i_var[14]; expandedR[20]=R_i_var[13];
  expandedR[19]=R_i_var[12]; expandedR[18]=R_i_var[11]; expandedR[17]=R_i_var[12]; expandedR[16]=R_i_var[11]; 

  expandedR[15]=R_i_var[10]; expandedR[14]=R_i_var[9]; expandedR[13]=R_i_var[8]; expandedR[12]=R_i_var[7]; 
  expandedR[11]=R_i_var[8]; expandedR[10]=R_i_var[7]; expandedR[9]=R_i_var[6]; expandedR[8]=R_i_var[5]; 
	
  expandedR[7]=R_i_var[4]; expandedR[6]=R_i_var[3]; expandedR[5]=R_i_var[4]; expandedR[4]=R_i_var[3]; 
  expandedR[3]=R_i_var[2]; expandedR[2]=R_i_var[1]; expandedR[1]=R_i_var[0]; expandedR[0]=R_i_var[31]; 
	  
  
  previous_key = (Key_i);
  iteration = (iteration_i);
  decrypt = (decrypt_i);
  
  round_key=new_key;
  
  KER=expandedR^round_key;
    
  
  s1_o = (KER[47:42]);
  s2_o = (KER[41:36]);
  s3_o = (KER[35:30]);
  s4_o = (KER[29:24]);
  s5_o = (KER[23:18]);
  s6_o = (KER[17:12]);
  s7_o = (KER[11:6]);
  s8_o = (KER[5:0]);

  Soutput[31:28]=s1_i;
  Soutput[27:24]=s2_i;
  Soutput[23:20]=s3_i;
  Soutput[19:16]=s4_i;
  Soutput[15:12]=s5_i;
  Soutput[11:8]=s6_i;
  Soutput[7:4]=s7_i;
  Soutput[3:0]=s8_i;
      

  
  f[31]=Soutput[16]; f[30]=Soutput[25]; f[29]=Soutput[12]; f[28]=Soutput[11]; 
  f[27]=Soutput[3]; f[26]=Soutput[20]; f[25]=Soutput[4]; f[24]=Soutput[15]; 
  
  f[23]=Soutput[31]; f[22]=Soutput[17]; f[21]=Soutput[9]; f[20]=Soutput[6]; 
  f[19]=Soutput[27]; f[18]=Soutput[14]; f[17]=Soutput[1]; f[16]=Soutput[22]; 
  
  f[15]=Soutput[30]; f[14]=Soutput[24]; f[13]=Soutput[8]; f[12]=Soutput[18]; 
  f[11]=Soutput[0]; f[10]=Soutput[5]; f[9]=Soutput[29]; f[8]=Soutput[23]; 
  
  f[7]=Soutput[13]; f[6]=Soutput[19]; f[5]=Soutput[2]; f[4]=Soutput[26]; 
  f[3]=Soutput[10]; f[2]=Soutput[21]; f[1]=Soutput[28]; f[0]=Soutput[7]; 
  
  next_R = (L_i^f);
  
  expanRSig = (L_i^f);
  
 
end

endmodule
