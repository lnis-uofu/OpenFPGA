//
// TV80 8-Bit Microprocessor Core
// Based on the VHDL T80 core by Daniel Wallner (jesus@opencores.org)
//
// Copyright (c) 2004 Guy Hutchison (ghutchis@opencores.org)
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

module tv80_alu (/*AUTOARG*/
  // Outputs
  Q, F_Out, 
  // Inputs
  Arith16, Z16, ALU_Op, IR, ISet, BusA, BusB, F_In
  );

  parameter		Mode   = 0;
  parameter		Flag_C = 0;
  parameter		Flag_N = 1;
  parameter		Flag_P = 2;
  parameter		Flag_X = 3;
  parameter		Flag_H = 4;
  parameter		Flag_Y = 5;
  parameter		Flag_Z = 6;
  parameter		Flag_S = 7;

  input 		Arith16;
  input 		Z16;
  input [3:0]           ALU_Op ;
  input [5:0]           IR;
  input [1:0]           ISet;
  input [7:0]           BusA;
  input [7:0]           BusB;
  input [7:0]           F_In;
  output [7:0]          Q;
  output [7:0]          F_Out;
  reg [7:0]             Q;
  reg [7:0]             F_Out;

  function [4:0] AddSub4;
    input [3:0] A;
    input [3:0] B;
    input Sub;
    input Carry_In;
    begin
      AddSub4 = { 1'b0, A } + { 1'b0, (Sub)?~B:B } + Carry_In;
    end
  endfunction // AddSub4
  
  function [3:0] AddSub3;
    input [2:0] A;
    input [2:0] B;
    input Sub;
    input Carry_In;
    begin
      AddSub3 = { 1'b0, A } + { 1'b0, (Sub)?~B:B } + Carry_In;
    end
  endfunction // AddSub4

  function [1:0] AddSub1;
    input A;
    input B;
    input Sub;
    input Carry_In;
    begin
      AddSub1 = { 1'b0, A } + { 1'b0, (Sub)?~B:B } + Carry_In;
    end
  endfunction // AddSub4

  // AddSub variables (temporary signals)
  reg UseCarry;
  reg Carry7_v;
  reg OverFlow_v;
  reg HalfCarry_v;
  reg Carry_v;
  reg [7:0] Q_v;

  reg [7:0] BitMask;


  always @(/*AUTOSENSE*/ALU_Op or BusA or BusB or F_In or IR)
    begin
      case (IR[5:3])
        3'b000 : BitMask = 8'b00000001;
        3'b001 : BitMask = 8'b00000010;
        3'b010 : BitMask = 8'b00000100; 
        3'b011 : BitMask = 8'b00001000; 
        3'b100 : BitMask = 8'b00010000; 
        3'b101 : BitMask = 8'b00100000; 
        3'b110 : BitMask = 8'b01000000; 
        default: BitMask = 8'b10000000; 
      endcase // case(IR[5:3])
      
      UseCarry = ~ ALU_Op[2] && ALU_Op[0];
      { HalfCarry_v, Q_v[3:0] } = AddSub4(BusA[3:0], BusB[3:0], ALU_Op[1], ALU_Op[1] ^ (UseCarry && F_In[Flag_C]) );
      { Carry7_v, Q_v[6:4]  } = AddSub3(BusA[6:4], BusB[6:4], ALU_Op[1], HalfCarry_v);
      { Carry_v, Q_v[7] } = AddSub1(BusA[7], BusB[7], ALU_Op[1], Carry7_v);
      OverFlow_v = Carry_v ^ Carry7_v;
    end // always @ *
  
  reg [7:0] Q_t;
  reg [8:0] DAA_Q;
  
  always @ (/*AUTOSENSE*/ALU_Op or Arith16 or BitMask or BusA or BusB
	    or Carry_v or F_In or HalfCarry_v or IR or ISet
	    or OverFlow_v or Q_v or Z16)
    begin
      Q_t = 8'hxx;
      DAA_Q = {9{1'bx}};
      
      F_Out = F_In;
      case (ALU_Op)
	4'b0000, 4'b0001,  4'b0010, 4'b0011, 4'b0100, 4'b0101, 4'b0110, 4'b0111 :
          begin
	    F_Out[Flag_N] = 1'b0;
	    F_Out[Flag_C] = 1'b0;
            
	    case (ALU_Op[2:0])
              
	      3'b000, 3'b001 : // ADD, ADC
                begin
		  Q_t = Q_v;
		  F_Out[Flag_C] = Carry_v;
		  F_Out[Flag_H] = HalfCarry_v;
		  F_Out[Flag_P] = OverFlow_v;
                end
              
	      3'b010, 3'b011, 3'b111 : // SUB, SBC, CP
                begin
		  Q_t = Q_v;
		  F_Out[Flag_N] = 1'b1;
		  F_Out[Flag_C] = ~ Carry_v;
		  F_Out[Flag_H] = ~ HalfCarry_v;
		  F_Out[Flag_P] = OverFlow_v;
                end
              
	      3'b100 : // AND
                begin
		  Q_t[7:0] = BusA & BusB;
		  F_Out[Flag_H] = 1'b1;
                end
              
	      3'b101 : // XOR
                begin
		  Q_t[7:0] = BusA ^ BusB;
		  F_Out[Flag_H] = 1'b0;
                end
              
	      default : // OR 3'b110
                begin
		  Q_t[7:0] = BusA | BusB;
		  F_Out[Flag_H] = 1'b0;
                end
              
	    endcase // case(ALU_OP[2:0])
            
	    if (ALU_Op[2:0] == 3'b111 ) 
              begin // CP
		F_Out[Flag_X] = BusB[3];
		F_Out[Flag_Y] = BusB[5];
	      end 
            else 
              begin
		F_Out[Flag_X] = Q_t[3];
		F_Out[Flag_Y] = Q_t[5];
	      end
            
	    if (Q_t[7:0] == 8'b00000000 ) 
              begin
		F_Out[Flag_Z] = 1'b1;
		if (Z16 == 1'b1 ) 
                  begin
		    F_Out[Flag_Z] = F_In[Flag_Z];	// 16 bit ADC,SBC
		  end
	      end 
            else 
              begin
		F_Out[Flag_Z] = 1'b0;
	      end // else: !if(Q_t[7:0] == 8'b00000000 )
            
	    F_Out[Flag_S] = Q_t[7];
	    case (ALU_Op[2:0])
	      3'b000, 3'b001, 3'b010, 3'b011, 3'b111 : // ADD, ADC, SUB, SBC, CP
                ;
              
	      default :
	        F_Out[Flag_P] = ~(^Q_t);                    
	    endcase // case(ALU_Op[2:0])
            
	    if (Arith16 == 1'b1 ) 
              begin
		F_Out[Flag_S] = F_In[Flag_S];
		F_Out[Flag_Z] = F_In[Flag_Z];
		F_Out[Flag_P] = F_In[Flag_P];
	      end
          end // case: 4'b0000, 4'b0001,  4'b0010, 4'b0011, 4'b0100, 4'b0101, 4'b0110, 4'b0111
        
	4'b1100 :
          begin
	    // DAA
	    F_Out[Flag_H] = F_In[Flag_H];
	    F_Out[Flag_C] = F_In[Flag_C];
	    DAA_Q[7:0] = BusA;
	    DAA_Q[8] = 1'b0;
	    if (F_In[Flag_N] == 1'b0 ) 
              begin
		// After addition
		// Alow > 9 || H == 1
		if (DAA_Q[3:0] > 9 || F_In[Flag_H] == 1'b1 ) 
                  begin
		    if ((DAA_Q[3:0] > 9) ) 
                      begin
			F_Out[Flag_H] = 1'b1;
		      end 
                    else 
                      begin
			F_Out[Flag_H] = 1'b0;
		      end
		    DAA_Q = DAA_Q + 6;
		  end // if (DAA_Q[3:0] > 9 || F_In[Flag_H] == 1'b1 )
                
		// new Ahigh > 9 || C == 1
		if (DAA_Q[8:4] > 9 || F_In[Flag_C] == 1'b1 ) 
                  begin
		    DAA_Q = DAA_Q + 96; // 0x60
		  end
	      end 
            else 
              begin
		// After subtraction
		if (DAA_Q[3:0] > 9 || F_In[Flag_H] == 1'b1 ) 
                  begin
		    if (DAA_Q[3:0] > 5 ) 
                      begin
			F_Out[Flag_H] = 1'b0;
		      end
		    DAA_Q[7:0] = DAA_Q[7:0] - 6;
		  end
		if (BusA > 153 || F_In[Flag_C] == 1'b1 ) 
                  begin
		    DAA_Q = DAA_Q - 352; // 0x160
		  end
	      end // else: !if(F_In[Flag_N] == 1'b0 )
            
	    F_Out[Flag_X] = DAA_Q[3];
	    F_Out[Flag_Y] = DAA_Q[5];
	    F_Out[Flag_C] = F_In[Flag_C] || DAA_Q[8];
	    Q_t = DAA_Q[7:0];
            
	    if (DAA_Q[7:0] == 8'b00000000 ) 
              begin
		F_Out[Flag_Z] = 1'b1;
	      end 
            else 
              begin
		F_Out[Flag_Z] = 1'b0;
	      end
            
	    F_Out[Flag_S] = DAA_Q[7];
	    F_Out[Flag_P] = ~ (^DAA_Q);
          end // case: 4'b1100
        
	4'b1101, 4'b1110 :
          begin
	    // RLD, RRD
	    Q_t[7:4] = BusA[7:4];
	    if (ALU_Op[0] == 1'b1 ) 
              begin
		Q_t[3:0] = BusB[7:4];
	      end 
            else 
              begin
		Q_t[3:0] = BusB[3:0];
	      end
	    F_Out[Flag_H] = 1'b0;
	    F_Out[Flag_N] = 1'b0;
	    F_Out[Flag_X] = Q_t[3];
	    F_Out[Flag_Y] = Q_t[5];
	    if (Q_t[7:0] == 8'b00000000 ) 
              begin
		F_Out[Flag_Z] = 1'b1;
	      end 
            else 
              begin
		F_Out[Flag_Z] = 1'b0;
	      end
	    F_Out[Flag_S] = Q_t[7];
            F_Out[Flag_P] = ~(^Q_t);
          end // case: when 4'b1101, 4'b1110
        
	4'b1001 :
          begin
	    // BIT
	    Q_t[7:0] = BusB & BitMask;
	    F_Out[Flag_S] = Q_t[7];
	    if (Q_t[7:0] == 8'b00000000 ) 
              begin
		F_Out[Flag_Z] = 1'b1;
		F_Out[Flag_P] = 1'b1;
	      end 
            else 
              begin
		F_Out[Flag_Z] = 1'b0;
		F_Out[Flag_P] = 1'b0;
	      end
	    F_Out[Flag_H] = 1'b1;
	    F_Out[Flag_N] = 1'b0;
	    F_Out[Flag_X] = 1'b0;
	    F_Out[Flag_Y] = 1'b0;
	    if (IR[2:0] != 3'b110 ) 
              begin
		F_Out[Flag_X] = BusB[3];
		F_Out[Flag_Y] = BusB[5];
	      end
          end // case: when 4'b1001
        
	4'b1010 :
	  // SET
	  Q_t[7:0] = BusB | BitMask;
        
	4'b1011 :
	  // RES
	  Q_t[7:0] = BusB & ~ BitMask;
        
	4'b1000 :
          begin
	    // ROT
	    case (IR[5:3])
	      3'b000 : // RLC
                begin
		  Q_t[7:1] = BusA[6:0];
		  Q_t[0] = BusA[7];
		  F_Out[Flag_C] = BusA[7];
                end
              
	      3'b010 : // RL
                begin
		  Q_t[7:1] = BusA[6:0];
		  Q_t[0] = F_In[Flag_C];
		  F_Out[Flag_C] = BusA[7];
                end
              
	      3'b001 : // RRC
                begin
		  Q_t[6:0] = BusA[7:1];
		  Q_t[7] = BusA[0];
		  F_Out[Flag_C] = BusA[0];
                end
              
	      3'b011 : // RR
                begin                        
		  Q_t[6:0] = BusA[7:1];
		  Q_t[7] = F_In[Flag_C];
		  F_Out[Flag_C] = BusA[0];
                end
              
	      3'b100 : // SLA
                begin
		  Q_t[7:1] = BusA[6:0];
		  Q_t[0] = 1'b0;
		  F_Out[Flag_C] = BusA[7];
                end
              
	      3'b110 : // SLL (Undocumented) / SWAP
                begin
		  if (Mode == 3 ) 
                    begin
		      Q_t[7:4] = BusA[3:0];
		      Q_t[3:0] = BusA[7:4];
		      F_Out[Flag_C] = 1'b0;                            
		    end 
                  else 
                    begin
		      Q_t[7:1] = BusA[6:0];
		      Q_t[0] = 1'b1;
		      F_Out[Flag_C] = BusA[7];
		    end // else: !if(Mode == 3 )
                end // case: 3'b110
              
	      3'b101 : // SRA
                begin
		  Q_t[6:0] = BusA[7:1];
		  Q_t[7] = BusA[7];
		  F_Out[Flag_C] = BusA[0];
                end
              
	      default : // SRL
                begin
		  Q_t[6:0] = BusA[7:1];
		  Q_t[7] = 1'b0;
		  F_Out[Flag_C] = BusA[0];
                end
	    endcase // case(IR[5:3])
            
	    F_Out[Flag_H] = 1'b0;
	    F_Out[Flag_N] = 1'b0;
	    F_Out[Flag_X] = Q_t[3];
	    F_Out[Flag_Y] = Q_t[5];
	    F_Out[Flag_S] = Q_t[7];
	    if (Q_t[7:0] == 8'b00000000 ) 
              begin
		F_Out[Flag_Z] = 1'b1;
	      end 
            else 
              begin
		F_Out[Flag_Z] = 1'b0;
	      end
            F_Out[Flag_P] = ~(^Q_t);

	    if (ISet == 2'b00 ) 
              begin
		F_Out[Flag_P] = F_In[Flag_P];
		F_Out[Flag_S] = F_In[Flag_S];
		F_Out[Flag_Z] = F_In[Flag_Z];
	      end
          end // case: 4'b1000
        
        
	default :
	  ;
        
      endcase // case(ALU_Op)
      
      Q = Q_t;
    end // always @ (Arith16, ALU_OP, F_In, BusA, BusB, IR, Q_v, Carry_v, HalfCarry_v, OverFlow_v, BitMask, ISet, Z16)
  
endmodule // T80_ALU
