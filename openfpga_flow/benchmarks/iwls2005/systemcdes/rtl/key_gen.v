//////////////////////////////////////////////////////////////////////
////                                                              ////
////  Key generator                                               ////
////                                                              ////
////  This file is part of the SystemC DES                        ////
////                                                              ////
////  Description:                                                ////
////  Generate the next key from the previous one                 ////
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
// $Log: key_gen.v,v $
// Revision 1.1.1.1  2004/07/05 17:31:17  jcastillo
// First import
//


module key_gen(previous_key,iteration,decrypt,non_perm_key,new_key);

input [55:0] previous_key;
input [3:0] iteration;
input decrypt;
output [55:0] non_perm_key;
output [47:0] new_key;

reg [55:0] non_perm_key;
reg [47:0] new_key;


reg  prev0,prev1;
reg[55:0]  prev_key_var,non_perm_key_var;
reg[47:0]  new_key_var;
reg[27:0]  semi_key;


always @(  previous_key or   iteration or   decrypt)

begin


	
	prev_key_var=previous_key;
	new_key_var=0;
	new_key = (0);
	non_perm_key_var=0;
	non_perm_key = (0);
	
	if(!decrypt)
      begin
	  case(iteration)

		  0, 1, 8, 15:
			begin
		      semi_key=prev_key_var[55:28];
		      prev0=semi_key[27];
		      semi_key=semi_key<<1;
		      semi_key[0]=prev0;
		      non_perm_key_var[55:28]=semi_key;
		      semi_key=prev_key_var[27:0];
		      prev0=semi_key[27];
		      semi_key=semi_key<<1;
		      semi_key[0]=prev0;
		      non_perm_key_var[27:0]=semi_key;
		     end
		  default:    
			begin
		      semi_key=prev_key_var[55:28];
		      prev0=semi_key[27];
		      prev1=semi_key[26];
		      semi_key=semi_key<<2;
		      semi_key[1]=prev0;
		      semi_key[0]=prev1;
		      non_perm_key_var[55:28]=semi_key;
		      semi_key=prev_key_var[27:0];
		      prev0=semi_key[27];
		      prev1=semi_key[26];
		      semi_key=semi_key<<2;
		      semi_key[1]=prev0;
		      semi_key[0]=prev1;
		      non_perm_key_var[27:0]=semi_key;
			end
      
      endcase
	  end
    else
    begin
	  case(iteration)

		  0:
		  begin
			  semi_key=prev_key_var[55:28];
		      non_perm_key_var[55:28]=semi_key;
		      semi_key=prev_key_var[27:0];
              non_perm_key_var[27:0]=semi_key;
		  end
		  1, 8, 15:
		  begin
		      semi_key=prev_key_var[55:28];
		      prev0=semi_key[0];
		      semi_key=semi_key>>1;
		      semi_key[27]=prev0;
		      non_perm_key_var[55:28]=semi_key;
		      semi_key=prev_key_var[27:0];
		      prev0=semi_key[0];
		      semi_key=semi_key>>1;
		      semi_key[27]=prev0;
		      non_perm_key_var[27:0]=semi_key;
		  end
		  default:    
			begin
		      semi_key=prev_key_var[55:28];
		      prev0=semi_key[0];
		      prev1=semi_key[1];
		      semi_key=semi_key>>2;
		      semi_key[26]=prev0;
		      semi_key[27]=prev1;
		      non_perm_key_var[55:28]=semi_key;
		      semi_key=prev_key_var[27:0];
		      prev0=semi_key[0];
		      prev1=semi_key[1];
		      semi_key=semi_key>>2;
		      semi_key[26]=prev0;
		      semi_key[27]=prev1;
		      non_perm_key_var[27:0]=semi_key;
		  end
	 
      endcase
	end

   
   non_perm_key = (non_perm_key_var);
      

   new_key_var[47]=non_perm_key_var[42]; new_key_var[46]=non_perm_key_var[39]; new_key_var[45]=non_perm_key_var[45]; new_key_var[44]=non_perm_key_var[32];
   new_key_var[43]=non_perm_key_var[55]; new_key_var[42]=non_perm_key_var[51]; new_key_var[41]=non_perm_key_var[53]; new_key_var[40]=non_perm_key_var[28];
	
   new_key_var[39]=non_perm_key_var[41]; new_key_var[38]=non_perm_key_var[50]; new_key_var[37]=non_perm_key_var[35]; new_key_var[36]=non_perm_key_var[46];
   new_key_var[35]=non_perm_key_var[33]; new_key_var[34]=non_perm_key_var[37]; new_key_var[33]=non_perm_key_var[44]; new_key_var[32]=non_perm_key_var[52];
  
   new_key_var[31]=non_perm_key_var[30]; new_key_var[30]=non_perm_key_var[48]; new_key_var[29]=non_perm_key_var[40]; new_key_var[28]=non_perm_key_var[49];
   new_key_var[27]=non_perm_key_var[29]; new_key_var[26]=non_perm_key_var[36]; new_key_var[25]=non_perm_key_var[43]; new_key_var[24]=non_perm_key_var[54];
	
   new_key_var[23]=non_perm_key_var[15]; new_key_var[22]=non_perm_key_var[4]; new_key_var[21]=non_perm_key_var[25]; new_key_var[20]=non_perm_key_var[19];
   new_key_var[19]=non_perm_key_var[9]; new_key_var[18]=non_perm_key_var[1]; new_key_var[17]=non_perm_key_var[26]; new_key_var[16]=non_perm_key_var[16]; 

   new_key_var[15]=non_perm_key_var[5]; new_key_var[14]=non_perm_key_var[11]; new_key_var[13]=non_perm_key_var[23]; new_key_var[12]=non_perm_key_var[8]; 
   new_key_var[11]=non_perm_key_var[12]; new_key_var[10]=non_perm_key_var[7]; new_key_var[9]=non_perm_key_var[17]; new_key_var[8]=non_perm_key_var[0]; 
	
   new_key_var[7]=non_perm_key_var[22]; new_key_var[6]=non_perm_key_var[3]; new_key_var[5]=non_perm_key_var[10]; new_key_var[4]=non_perm_key_var[14]; 
   new_key_var[3]=non_perm_key_var[6]; new_key_var[2]=non_perm_key_var[20]; new_key_var[1]=non_perm_key_var[27]; new_key_var[0]=non_perm_key_var[24]; 

   new_key = (new_key_var);
   

end

endmodule
