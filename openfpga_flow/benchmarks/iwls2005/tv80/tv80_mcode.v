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

module tv80_mcode (/*AUTOARG*/
  // Outputs
  MCycles, TStates, Prefix, Inc_PC, Inc_WZ, IncDec_16, Read_To_Reg, 
  Read_To_Acc, Set_BusA_To, Set_BusB_To, ALU_Op, Save_ALU, PreserveC, 
  Arith16, Set_Addr_To, IORQ, Jump, JumpE, JumpXY, Call, RstP, LDZ, 
  LDW, LDSPHL, Special_LD, ExchangeDH, ExchangeRp, ExchangeAF, 
  ExchangeRS, I_DJNZ, I_CPL, I_CCF, I_SCF, I_RETN, I_BT, I_BC, I_BTR, 
  I_RLD, I_RRD, I_INRC, SetDI, SetEI, IMode, Halt, NoRead, Write, 
  // Inputs
  IR, ISet, MCycle, F, NMICycle, IntCycle
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

  input [7:0]           IR;
  input [1:0]           ISet			;
  input [2:0]           MCycle			;
  input [7:0]           F		       	;
  input 		NMICycle		;
  input 		IntCycle		;
  output [2:0]          MCycles			;
  output [2:0]          TStates			;
  output [1:0]          Prefix			; // None,BC,ED,DD/FD
  output		Inc_PC			;
  output		Inc_WZ			;
  output [3:0]          IncDec_16		; // BC,DE,HL,SP   0 is inc
  output		Read_To_Reg		;
  output		Read_To_Acc		;
  output [3:0]          Set_BusA_To	; // B,C,D,E,H,L,DI/DB,A,SP(L),SP(M),0,F
  output [3:0]          Set_BusB_To	; // B,C,D,E,H,L,DI,A,SP(L),SP(M),1,F,PC(L),PC(M),0
  output [3:0]          ALU_Op			;
  output		Save_ALU		;
  output		PreserveC		;
  output		Arith16			;
  output [2:0]          Set_Addr_To		; // aNone,aXY,aIOA,aSP,aBC,aDE,aZI
  output		IORQ			;
  output		Jump			;
  output		JumpE			;
  output		JumpXY			;
  output		Call			;
  output		RstP			;
  output		LDZ		       	;
  output		LDW		       	;
  output		LDSPHL			;
  output [2:0]          Special_LD		; // A,I;A,R;I,A;R,A;None
  output		ExchangeDH		;
  output		ExchangeRp		;
  output                ExchangeAF		;
  output                ExchangeRS		;
  output		I_DJNZ			;
  output		I_CPL			;
  output		I_CCF			;
  output		I_SCF			;
  output		I_RETN			;
  output		I_BT			;
  output		I_BC			;
  output		I_BTR			;
  output		I_RLD			;
  output		I_RRD			;
  output		I_INRC			;
  output		SetDI			;
  output		SetEI			;
  output [1:0]          IMode			;
  output		Halt			;
  output		NoRead			;
  output                Write	;		 

  // regs
  reg [2:0]             MCycles			;
  reg [2:0]             TStates			;
  reg [1:0]             Prefix			; // None,BC,ED,DD/FD
  reg                   Inc_PC			;
  reg                   Inc_WZ			;
  reg [3:0]             IncDec_16		; // BC,DE,HL,SP   0 is inc
  reg                   Read_To_Reg		;
  reg                   Read_To_Acc		;
  reg [3:0]             Set_BusA_To	; // B,C,D,E,H,L,DI/DB,A,SP(L),SP(M),0,F
  reg [3:0]             Set_BusB_To	; // B,C,D,E,H,L,DI,A,SP(L),SP(M),1,F,PC(L),PC(M),0
  reg [3:0]             ALU_Op			;
  reg                   Save_ALU		;
  reg                   PreserveC		;
  reg                   Arith16			;
  reg [2:0]             Set_Addr_To		; // aNone,aXY,aIOA,aSP,aBC,aDE,aZI
  reg                   IORQ			;
  reg                   Jump			;
  reg                   JumpE			;
  reg                   JumpXY			;
  reg                   Call			;
  reg                   RstP			;
  reg                   LDZ		       	;
  reg                   LDW		       	;
  reg                   LDSPHL			;
  reg [2:0]             Special_LD		; // A,I;A,R;I,A;R,A;None
  reg                   ExchangeDH		;
  reg                   ExchangeRp		;
  reg                   ExchangeAF		;
  reg                   ExchangeRS		;
  reg                   I_DJNZ			;
  reg                   I_CPL			;
  reg                   I_CCF			;
  reg                   I_SCF			;
  reg                   I_RETN			;
  reg                   I_BT			;
  reg                   I_BC			;
  reg                   I_BTR			;
  reg                   I_RLD			;
  reg                   I_RRD			;
  reg                   I_INRC			;
  reg                   SetDI			;
  reg                   SetEI			;
  reg [1:0]             IMode			;
  reg                   Halt			;
  reg                   NoRead			;
  reg                   Write	;		 

	parameter aNone	= 3'b111;
	parameter aBC	= 3'b000;
	parameter aDE	= 3'b001;
	parameter aXY	= 3'b010;
	parameter aIOA	= 3'b100;
	parameter aSP	= 3'b101;
	parameter aZI	= 3'b110;
//	constant aNone	: std_logic_vector[2:0] = 3'b000;
//	constant aXY	: std_logic_vector[2:0] = 3'b001;
//	constant aIOA	: std_logic_vector[2:0] = 3'b010;
//	constant aSP	: std_logic_vector[2:0] = 3'b011;
//	constant aBC	: std_logic_vector[2:0] = 3'b100;
//	constant aDE	: std_logic_vector[2:0] = 3'b101;
//	constant aZI	: std_logic_vector[2:0] = 3'b110;

  function is_cc_true;
    input [7:0] F;
    input [2:0] cc;
    begin
      if (Mode == 3 ) 
        begin
	  case (cc)
	    3'b000  : is_cc_true = F[7] == 1'b0; // NZ
	    3'b001  : is_cc_true = F[7] == 1'b1; // Z
	    3'b010  : is_cc_true = F[4] == 1'b0; // NC
	    3'b011  : is_cc_true = F[4] == 1'b1; // C
	    3'b100  : is_cc_true = 0;
	    3'b101  : is_cc_true = 0;
	    3'b110  : is_cc_true = 0;
	    3'b111  : is_cc_true = 0;
	  endcase
	end 
      else 
        begin
	  case (cc)
	    3'b000  : is_cc_true = F[6] == 1'b0; // NZ
	    3'b001  : is_cc_true = F[6] == 1'b1; // Z
	    3'b010  : is_cc_true = F[0] == 1'b0; // NC
	    3'b011  : is_cc_true = F[0] == 1'b1; // C
	    3'b100  : is_cc_true = F[2] == 1'b0; // PO
	    3'b101  : is_cc_true = F[2] == 1'b1; // PE
	    3'b110  : is_cc_true = F[7] == 1'b0; // P
	    3'b111  : is_cc_true = F[7] == 1'b1; // M
	  endcase
	end
    end
  endfunction // is_cc_true
  

  reg [2:0] DDD;
  reg [2:0] SSS;
  reg [1:0] DPAIR;
  reg [7:0] IRB;
  
  always @ (/*AUTOSENSE*/F or IR or ISet or IntCycle or MCycle
	    or NMICycle)
    begin
      DDD = IR[5:3];
      SSS = IR[2:0];
      DPAIR = IR[5:4];
      IRB = IR;

      MCycles = 3'b001;
      if (MCycle == 3'b001 ) 
        begin
	  TStates = 3'b100;
	end 
      else 
        begin
	  TStates = 3'b011;
	end
      Prefix = 2'b00;
      Inc_PC = 1'b0;
      Inc_WZ = 1'b0;
      IncDec_16 = 4'b0000;
      Read_To_Acc = 1'b0;
      Read_To_Reg = 1'b0;
      Set_BusB_To = 4'b0000;
      Set_BusA_To = 4'b0000;
      ALU_Op = { 1'b0, IR[5:3] };
      Save_ALU = 1'b0;
      PreserveC = 1'b0;
      Arith16 = 1'b0;
      IORQ = 1'b0;
      Set_Addr_To = aNone;
      Jump = 1'b0;
      JumpE = 1'b0;
      JumpXY = 1'b0;
      Call = 1'b0;
      RstP = 1'b0;
      LDZ = 1'b0;
      LDW = 1'b0;
      LDSPHL = 1'b0;
      Special_LD = 3'b000;
      ExchangeDH = 1'b0;
      ExchangeRp = 1'b0;
      ExchangeAF = 1'b0;
      ExchangeRS = 1'b0;
      I_DJNZ = 1'b0;
      I_CPL = 1'b0;
      I_CCF = 1'b0;
      I_SCF = 1'b0;
      I_RETN = 1'b0;
      I_BT = 1'b0;
      I_BC = 1'b0;
      I_BTR = 1'b0;
      I_RLD = 1'b0;
      I_RRD = 1'b0;
      I_INRC = 1'b0;
      SetDI = 1'b0;
      SetEI = 1'b0;
      IMode = 2'b11;
      Halt = 1'b0;
      NoRead = 1'b0;
      Write = 1'b0;
      
      case (ISet)
	2'b00  :
          begin

//----------------------------------------------------------------------------
//
//	Unprefixed instructions
//
//----------------------------------------------------------------------------

	    case (IRB)
// 8 BIT LOAD GROUP
	      8'b01000000,8'b01000001,8'b01000010,8'b01000011,8'b01000100,8'b01000101,8'b01000111,
	      8'b01001000,8'b01001001,8'b01001010,8'b01001011,8'b01001100,8'b01001101,8'b01001111,
	      8'b01010000,8'b01010001,8'b01010010,8'b01010011,8'b01010100,8'b01010101,8'b01010111,
	      8'b01011000,8'b01011001,8'b01011010,8'b01011011,8'b01011100,8'b01011101,8'b01011111,
	      8'b01100000,8'b01100001,8'b01100010,8'b01100011,8'b01100100,8'b01100101,8'b01100111,
	      8'b01101000,8'b01101001,8'b01101010,8'b01101011,8'b01101100,8'b01101101,8'b01101111,
	      8'b01111000,8'b01111001,8'b01111010,8'b01111011,8'b01111100,8'b01111101,8'b01111111 :
                begin
                            
		  // LD r,r'
		  Set_BusB_To[2:0] = SSS;
		  ExchangeRp = 1'b1;
		  Set_BusA_To[2:0] = DDD;
		  Read_To_Reg = 1'b1;
                end // case: 8'b01000000,8'b01000001,8'b01000010,8'b01000011,8'b01000100,8'b01000101,8'b01000111,...
              
	      8'b00000110,8'b00001110,8'b00010110,8'b00011110,8'b00100110,8'b00101110,8'b00111110  :
                begin
		  // LD r,n
		  MCycles = 3'b010;
		  case (MCycle)
			2  :
                          begin
				Inc_PC = 1'b1;
				Set_BusA_To[2:0] = DDD;
				Read_To_Reg = 1'b1;
                          end
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b00000110,8'b00001110,8'b00010110,8'b00011110,8'b00100110,8'b00101110,8'b00111110
              
	      8'b01000110,8'b01001110,8'b01010110,8'b01011110,8'b01100110,8'b01101110,8'b01111110  :
                  begin
		    // LD r,(HL)
		    MCycles = 3'b010;
		    case (MCycle)
		      1  :
				Set_Addr_To = aXY;
		      2  :
                        begin
			  Set_BusA_To[2:0] = DDD;
			  Read_To_Reg = 1'b1;
                        end
		      default :;
		    endcase // case(MCycle)
                  end // case: 8'b01000110,8'b01001110,8'b01010110,8'b01011110,8'b01100110,8'b01101110,8'b01111110
              
	      8'b01110000,8'b01110001,8'b01110010,8'b01110011,8'b01110100,8'b01110101,8'b01110111  :
                  begin
		    // LD (HL),r
		    MCycles = 3'b010;
		    case (MCycle)
		      1  :
                        begin
			  Set_Addr_To = aXY;
			  Set_BusB_To[2:0] = SSS;
			  Set_BusB_To[3] = 1'b0;
                        end
		      2  :
			Write = 1'b1;
		      default :;
		    endcase // case(MCycle)
                  end // case: 8'b01110000,8'b01110001,8'b01110010,8'b01110011,8'b01110100,8'b01110101,8'b01110111
              
	      8'b00110110  :
                  begin
		    // LD (HL),n
		    MCycles = 3'b011;
		    case (MCycle)
		      2  :
                        begin
			  Inc_PC = 1'b1;
			  Set_Addr_To = aXY;
			  Set_BusB_To[2:0] = SSS;
			  Set_BusB_To[3] = 1'b0;
                        end
		      3  :
			Write = 1'b1;
		      default :;
		    endcase // case(MCycle)
                  end // case: 8'b00110110
              
	      8'b00001010  :
                begin
		  // LD A,(BC)
		  MCycles = 3'b010;
		  case (MCycle)
		    1  :
		      Set_Addr_To = aBC;
		    2  :
		      Read_To_Acc = 1'b1;
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b00001010
              
	      8'b00011010  :
                begin
		  // LD A,(DE)
		  MCycles = 3'b010;
		  case (MCycle)
		    1  :
		      Set_Addr_To = aDE;
		    2  :
		      Read_To_Acc = 1'b1;
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b00011010
              
	      8'b00111010  :
                begin
		  if (Mode == 3 ) 
                    begin
		      // LDD A,(HL)
		      MCycles = 3'b010;
		      case (MCycle)
			1  :
			  Set_Addr_To = aXY;
			2  :
                          begin
			    Read_To_Acc = 1'b1;
			    IncDec_16 = 4'b1110;
                          end
			default :;
		      endcase
		    end 
                  else 
                    begin
		      // LD A,(nn)
		      MCycles = 3'b100;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    LDZ = 1'b1;
                          end
			3  :
                          begin
			    Set_Addr_To = aZI;
			    Inc_PC = 1'b1;
                          end
			4  :
                          begin
			    Read_To_Acc = 1'b1;
                          end
			default :;
		      endcase
		    end // else: !if(Mode == 3 )
                end // case: 8'b00111010
              
	      8'b00000010  :
                begin
		  // LD (BC),A
		  MCycles = 3'b010;
		  case (MCycle)
		    1  :
                      begin
			Set_Addr_To = aBC;
			Set_BusB_To = 4'b0111;
                      end
		    2  :
                      begin
		        Write = 1'b1;
                      end
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b00000010
              
	      8'b00010010  :
                begin
		  // LD (DE),A
		  MCycles = 3'b010;
		  case (MCycle)
		    1  :
                      begin
			Set_Addr_To = aDE;
			Set_BusB_To = 4'b0111;
                      end
		    2  :
		      Write = 1'b1;
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b00010010
              
	      8'b00110010  :
                begin
		  if (Mode == 3 ) 
                    begin
		      // LDD (HL),A
		      MCycles = 3'b010;
		      case (MCycle)
			1  :
                          begin
			    Set_Addr_To = aXY;
			    Set_BusB_To = 4'b0111;
                          end
			2  :
                          begin
			    Write = 1'b1;
			    IncDec_16 = 4'b1110;
                          end
			default :;
		      endcase // case(MCycle)
                      
		    end 
                  else 
                    begin
		      // LD (nn),A
		      MCycles = 3'b100;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    LDZ = 1'b1;
                          end
			3  :
                          begin
			    Set_Addr_To = aZI;
			    Inc_PC = 1'b1;
			    Set_BusB_To = 4'b0111;
                          end
			4  :
                          begin
			    Write = 1'b1;
                          end
			default :;
		      endcase
		    end // else: !if(Mode == 3 )
                end // case: 8'b00110010
              

// 16 BIT LOAD GROUP
	      8'b00000001,8'b00010001,8'b00100001,8'b00110001  :
                begin
		  // LD dd,nn
		  MCycles = 3'b011;
		  case (MCycle)
		    2  :
                      begin
			Inc_PC = 1'b1;
			Read_To_Reg = 1'b1;
			if (DPAIR == 2'b11 ) 
                          begin
			    Set_BusA_To[3:0] = 4'b1000;
			  end 
                        else 
                          begin
			    Set_BusA_To[2:1] = DPAIR;
			    Set_BusA_To[0] = 1'b1;
			  end
                      end // case: 2
                    
		    3  :
                      begin
			Inc_PC = 1'b1;
			Read_To_Reg = 1'b1;
			if (DPAIR == 2'b11 ) 
                          begin
			    Set_BusA_To[3:0] = 4'b1001;
			  end 
                        else 
                          begin
			    Set_BusA_To[2:1] = DPAIR;
			    Set_BusA_To[0] = 1'b0;
			  end
                      end // case: 3
                    
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b00000001,8'b00010001,8'b00100001,8'b00110001
              
	      8'b00101010  :
                begin
		  if (Mode == 3 ) 
                    begin
		      // LDI A,(HL)
		      MCycles = 3'b010;
		      case (MCycle)
			1  :
			  Set_Addr_To = aXY;
			2  :
                          begin
			    Read_To_Acc = 1'b1;
			    IncDec_16 = 4'b0110;
                          end
                        
			default :;
		      endcase
		    end 
                  else 
                    begin
		      // LD HL,(nn)
		      MCycles = 3'b101;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    LDZ = 1'b1;
                          end
			3  :
                          begin
			    Set_Addr_To = aZI;
			    Inc_PC = 1'b1;
			    LDW = 1'b1;
                          end
			4  :
                          begin
			    Set_BusA_To[2:0] = 3'b101; // L
			    Read_To_Reg = 1'b1;
			    Inc_WZ = 1'b1;
			    Set_Addr_To = aZI;
                          end
			5  :
                          begin
			    Set_BusA_To[2:0] = 3'b100; // H
			    Read_To_Reg = 1'b1;
                          end
			default :;
		      endcase
		    end // else: !if(Mode == 3 )
                end // case: 8'b00101010
              
	      8'b00100010  :
                begin
		  if (Mode == 3 ) 
                    begin
		      // LDI (HL),A
		      MCycles = 3'b010;
		      case (MCycle)
			1  :
                          begin
			    Set_Addr_To = aXY;
			    Set_BusB_To = 4'b0111;
                          end
			2  :
                          begin
			    Write = 1'b1;
			    IncDec_16 = 4'b0110;
                          end
			default :;
		      endcase
		    end 
                  else 
                    begin
		      // LD (nn),HL
		      MCycles = 3'b101;
		      case (MCycle)                        
			2  :
                          begin
			    Inc_PC = 1'b1;
			    LDZ = 1'b1;
                          end
                        
			3  :
                          begin
			    Set_Addr_To = aZI;
			    Inc_PC = 1'b1;
			    LDW = 1'b1;
			    Set_BusB_To = 4'b0101; // L
                          end
                        
			4  :
                          begin
			    Inc_WZ = 1'b1;
			    Set_Addr_To = aZI;
			    Write = 1'b1;
			    Set_BusB_To = 4'b0100; // H
                          end
			5  :                          
			  Write = 1'b1;
			default :;
		      endcase
		    end // else: !if(Mode == 3 )
                end // case: 8'b00100010
              
		8'b11111001  :
                  begin
		    // LD SP,HL
		    TStates = 3'b110;
		    LDSPHL = 1'b1;
                  end
              
	      8'b11000101,8'b11010101,8'b11100101,8'b11110101  :
                begin
		  // PUSH qq
		  MCycles = 3'b011;
		  case (MCycle)                    
		    1  :
                      begin
			TStates = 3'b101;
			IncDec_16 = 4'b1111;
			Set_Addr_To = aSP;
			if (DPAIR == 2'b11 ) 
                          begin
			    Set_BusB_To = 4'b0111;
			  end 
                        else
                          begin
			    Set_BusB_To[2:1] = DPAIR;
			    Set_BusB_To[0] = 1'b0;
			    Set_BusB_To[3] = 1'b0;
			  end
                      end // case: 1
                    
		    2  :
                      begin
			IncDec_16 = 4'b1111;
			Set_Addr_To = aSP;
			if (DPAIR == 2'b11 ) 
                          begin
			    Set_BusB_To = 4'b1011;
			  end 
                        else 
                          begin
			    Set_BusB_To[2:1] = DPAIR;
			    Set_BusB_To[0] = 1'b1;
			    Set_BusB_To[3] = 1'b0;
			  end
			Write = 1'b1;
                      end // case: 2
                    
		    3  :
		      Write = 1'b1;
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b11000101,8'b11010101,8'b11100101,8'b11110101
              
	      8'b11000001,8'b11010001,8'b11100001,8'b11110001  :
                begin
		  // POP qq
		  MCycles = 3'b011;
		  case (MCycle)
		    1  :
		      Set_Addr_To = aSP;
		    2  :
                      begin
			IncDec_16 = 4'b0111;
			Set_Addr_To = aSP;
			Read_To_Reg = 1'b1;
			if (DPAIR == 2'b11 ) 
                          begin
			    Set_BusA_To[3:0] = 4'b1011;
			  end 
                        else 
                          begin
			    Set_BusA_To[2:1] = DPAIR;
			    Set_BusA_To[0] = 1'b1;
			  end
                      end // case: 2
                    
		    3  :
                      begin
			IncDec_16 = 4'b0111;
			Read_To_Reg = 1'b1;
			if (DPAIR == 2'b11 ) 
                          begin
			    Set_BusA_To[3:0] = 4'b0111;
			  end 
                        else 
                          begin
			    Set_BusA_To[2:1] = DPAIR;
			    Set_BusA_To[0] = 1'b0;
			  end
                      end // case: 3
                    
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b11000001,8'b11010001,8'b11100001,8'b11110001
              

// EXCHANGE, BLOCK TRANSFER AND SEARCH GROUP
	      8'b11101011  :
                begin
		  if (Mode != 3 ) 
                    begin
		      // EX DE,HL
		      ExchangeDH = 1'b1;
		    end
                end
              
	      8'b00001000  :
                begin
		  if (Mode == 3 ) 
                    begin
		      // LD (nn),SP
		      MCycles = 3'b101;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    LDZ = 1'b1;
                          end
                        
			3  :
                          begin
			    Set_Addr_To = aZI;
			    Inc_PC = 1'b1;
			    LDW = 1'b1;
			    Set_BusB_To = 4'b1000;
                          end
                        
			4  :
                          begin
			    Inc_WZ = 1'b1;
			    Set_Addr_To = aZI;
			    Write = 1'b1;
			    Set_BusB_To = 4'b1001;
                          end
                        
			5  :
			  Write = 1'b1;
			default :;
		      endcase
		    end 
                  else if (Mode < 2 ) 
                    begin
		      // EX AF,AF'
		      ExchangeAF = 1'b1;
		    end
                end // case: 8'b00001000
              
	      8'b11011001  :
                begin
		  if (Mode == 3 ) 
                    begin
		      // RETI
		      MCycles = 3'b011;
		      case (MCycle)
			1  :
			  Set_Addr_To = aSP;
			2  :
                          begin
			    IncDec_16 = 4'b0111;
			    Set_Addr_To = aSP;
			    LDZ = 1'b1;
                          end
                        
			3  :
                          begin
			    Jump = 1'b1;
			    IncDec_16 = 4'b0111;
			    I_RETN = 1'b1;
			    SetEI = 1'b1;
                          end
			default :;
		      endcase
		    end 
                  else if (Mode < 2 ) 
                    begin
		      // EXX
		      ExchangeRS = 1'b1;
		    end
                end // case: 8'b11011001
              
	      8'b11100011  :
                begin
		  if (Mode != 3 ) 
                    begin
		      // EX (SP),HL
		      MCycles = 3'b101;
		      case (MCycle)
			1  :
			  Set_Addr_To = aSP;
			2  :
                          begin
			    Read_To_Reg = 1'b1;
			    Set_BusA_To = 4'b0101;
			    Set_BusB_To = 4'b0101;
			    Set_Addr_To = aSP;
                          end
			3  :
                          begin
			    IncDec_16 = 4'b0111;
			    Set_Addr_To = aSP;
			    TStates = 3'b100;
			    Write = 1'b1;
                          end
			4  :
                          begin
			    Read_To_Reg = 1'b1;
			    Set_BusA_To = 4'b0100;
			    Set_BusB_To = 4'b0100;
			    Set_Addr_To = aSP;
                          end
			5  :
                          begin
			    IncDec_16 = 4'b1111;
			    TStates = 3'b101;
			    Write = 1'b1;
                          end
                        
			default :;
		      endcase
		    end // if (Mode != 3 )
                end // case: 8'b11100011
              

// 8 BIT ARITHMETIC AND LOGICAL GROUP
	      8'b10000000,8'b10000001,8'b10000010,8'b10000011,8'b10000100,8'b10000101,8'b10000111,
	      8'b10001000,8'b10001001,8'b10001010,8'b10001011,8'b10001100,8'b10001101,8'b10001111,
	      8'b10010000,8'b10010001,8'b10010010,8'b10010011,8'b10010100,8'b10010101,8'b10010111,
	      8'b10011000,8'b10011001,8'b10011010,8'b10011011,8'b10011100,8'b10011101,8'b10011111,
	      8'b10100000,8'b10100001,8'b10100010,8'b10100011,8'b10100100,8'b10100101,8'b10100111,
	      8'b10101000,8'b10101001,8'b10101010,8'b10101011,8'b10101100,8'b10101101,8'b10101111,
	      8'b10110000,8'b10110001,8'b10110010,8'b10110011,8'b10110100,8'b10110101,8'b10110111,
	      8'b10111000,8'b10111001,8'b10111010,8'b10111011,8'b10111100,8'b10111101,8'b10111111 :
                begin
		  // ADD A,r
		  // ADC A,r
		  // SUB A,r
		  // SBC A,r
		  // AND A,r
		  // OR A,r
		  // XOR A,r
		  // CP A,r
		  Set_BusB_To[2:0] = SSS;
		  Set_BusA_To[2:0] = 3'b111;
		  Read_To_Reg = 1'b1;
		  Save_ALU = 1'b1;
                end // case: 8'b10000000,8'b10000001,8'b10000010,8'b10000011,8'b10000100,8'b10000101,8'b10000111,...
              
	      8'b10000110,8'b10001110,8'b10010110,8'b10011110,8'b10100110,8'b10101110,8'b10110110,8'b10111110  :
                begin
		  // ADD A,(HL)
		  // ADC A,(HL)
		  // SUB A,(HL)
		  // SBC A,(HL)
		  // AND A,(HL)
		  // OR A,(HL)
		  // XOR A,(HL)
		  // CP A,(HL)
		  MCycles = 3'b010;
		  case (MCycle)
		    1  :
		      Set_Addr_To = aXY;
		    2  :
                      begin
			Read_To_Reg = 1'b1;
			Save_ALU = 1'b1;
			Set_BusB_To[2:0] = SSS;
			Set_BusA_To[2:0] = 3'b111;
                      end
                    
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b10000110,8'b10001110,8'b10010110,8'b10011110,8'b10100110,8'b10101110,8'b10110110,8'b10111110
              
	      8'b11000110,8'b11001110,8'b11010110,8'b11011110,8'b11100110,8'b11101110,8'b11110110,8'b11111110  :
                begin
		  // ADD A,n
		  // ADC A,n
		  // SUB A,n
		  // SBC A,n
		  // AND A,n
		  // OR A,n
		  // XOR A,n
		  // CP A,n
		  MCycles = 3'b010;
		  if (MCycle == 3'b010 ) 
                    begin
		      Inc_PC = 1'b1;
		      Read_To_Reg = 1'b1;
		      Save_ALU = 1'b1;
		      Set_BusB_To[2:0] = SSS;
		      Set_BusA_To[2:0] = 3'b111;
		    end
                end // case: 8'b11000110,8'b11001110,8'b11010110,8'b11011110,8'b11100110,8'b11101110,8'b11110110,8'b11111110
              
	      8'b00000100,8'b00001100,8'b00010100,8'b00011100,8'b00100100,8'b00101100,8'b00111100  :
                begin
		  // INC r
		  Set_BusB_To = 4'b1010;
		  Set_BusA_To[2:0] = DDD;
		  Read_To_Reg = 1'b1;
		  Save_ALU = 1'b1;
		  PreserveC = 1'b1;
		  ALU_Op = 4'b0000;
                end
              
	      8'b00110100  :
                begin
		  // INC (HL)
		  MCycles = 3'b011;
		  case (MCycle)
		    1  :
		      Set_Addr_To = aXY;
		    2  :
                      begin
			TStates = 3'b100;
			Set_Addr_To = aXY;
			Read_To_Reg = 1'b1;
			Save_ALU = 1'b1;
			PreserveC = 1'b1;
			ALU_Op = 4'b0000;
			Set_BusB_To = 4'b1010;
			Set_BusA_To[2:0] = DDD;
                      end // case: 2
                    
		    3  :
		      Write = 1'b1;
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b00110100
              
	      8'b00000101,8'b00001101,8'b00010101,8'b00011101,8'b00100101,8'b00101101,8'b00111101  :
                begin               
		  // DEC r
		  Set_BusB_To = 4'b1010;
		  Set_BusA_To[2:0] = DDD;
		  Read_To_Reg = 1'b1;
		  Save_ALU = 1'b1;
		  PreserveC = 1'b1;
		  ALU_Op = 4'b0010;
                end
              
	      8'b00110101  :
                begin
		  // DEC (HL)
		  MCycles = 3'b011;
		  case (MCycle)
		    1  :
		      Set_Addr_To = aXY;
		    2  :
                      begin
			TStates = 3'b100;
			Set_Addr_To = aXY;
			ALU_Op = 4'b0010;
			Read_To_Reg = 1'b1;
			Save_ALU = 1'b1;
			PreserveC = 1'b1;
			Set_BusB_To = 4'b1010;
			Set_BusA_To[2:0] = DDD;
                      end // case: 2
                    
		    3  :
		      Write = 1'b1;
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b00110101              

// GENERAL PURPOSE ARITHMETIC AND CPU CONTROL GROUPS
		8'b00100111  :
                  begin
		    // DAA
		    Set_BusA_To[2:0] = 3'b111;
		    Read_To_Reg = 1'b1;
		    ALU_Op = 4'b1100;
		    Save_ALU = 1'b1;
                  end
              
		8'b00101111  :
		  // CPL
		  I_CPL = 1'b1;
              
		8'b00111111  :
		  // CCF
		  I_CCF = 1'b1;
              
		8'b00110111  :
		  // SCF
		  I_SCF = 1'b1;
              
		8'b00000000  :
                  begin
		    if (NMICycle == 1'b1 ) 
                      begin
			// NMI
			MCycles = 3'b011;
			case (MCycle)
			  1  :
                            begin
			      TStates = 3'b101;
			      IncDec_16 = 4'b1111;
			      Set_Addr_To = aSP;
			      Set_BusB_To = 4'b1101;
                            end
                          
			  2  :
                            begin
			      TStates = 3'b100;
			      Write = 1'b1;
			      IncDec_16 = 4'b1111;
			      Set_Addr_To = aSP;
			      Set_BusB_To = 4'b1100;
                            end
                          
			  3  :
                            begin
			      TStates = 3'b100;
			      Write = 1'b1;
                            end
                          
			  default :;
			endcase // case(MCycle)
                        
		      end 
                    else if (IntCycle == 1'b1 ) 
                      begin
			// INT (IM 2)
			MCycles = 3'b101;
			case (MCycle)
			  1  :
                            begin
			      LDZ = 1'b1;
			      TStates = 3'b101;
			      IncDec_16 = 4'b1111;
			      Set_Addr_To = aSP;
			      Set_BusB_To = 4'b1101;
                            end
                          
			  2  :
                            begin
			      TStates = 3'b100;
			      Write = 1'b1;
			      IncDec_16 = 4'b1111;
			      Set_Addr_To = aSP;
			      Set_BusB_To = 4'b1100;
                            end
                          
			  3  :
                            begin
			      TStates = 3'b100;
			      Write = 1'b1;
                            end
                          
			  4  :
                            begin
			      Inc_PC = 1'b1;
			      LDZ = 1'b1;
                            end
                          
			  5  :
			    Jump = 1'b1;
			  default :;
			endcase
		      end
		  end // case: 8'b00000000
              
	      8'b01110110  :
		// HALT
		Halt = 1'b1;
              
	      8'b11110011  :
		// DI
		SetDI = 1'b1;
              
	      8'b11111011  :
		// EI
		SetEI = 1'b1;

              // 16 BIT ARITHMETIC GROUP
	      8'b00001001,8'b00011001,8'b00101001,8'b00111001  :
                begin
		  // ADD HL,ss
		  MCycles = 3'b011;
		  case (MCycle)
		    2  :
                      begin
			NoRead = 1'b1;
			ALU_Op = 4'b0000;
			Read_To_Reg = 1'b1;
			Save_ALU = 1'b1;
			Set_BusA_To[2:0] = 3'b101;
			case (IR[5:4])
			  0,1,2  :
                            begin
			      Set_BusB_To[2:1] = IR[5:4];
			      Set_BusB_To[0] = 1'b1;
                            end
                          
			  default :
			    Set_BusB_To = 4'b1000;
			endcase // case(IR[5:4])
                        
			TStates = 3'b100;
			Arith16 = 1'b1;
                      end // case: 2
                    
		    3  :
                      begin
			NoRead = 1'b1;
			Read_To_Reg = 1'b1;
			Save_ALU = 1'b1;
			ALU_Op = 4'b0001;
			Set_BusA_To[2:0] = 3'b100;
			case (IR[5:4])
			  0,1,2  :
			    Set_BusB_To[2:1] = IR[5:4];
			  default :
			    Set_BusB_To = 4'b1001;
			endcase
			Arith16 = 1'b1;
                      end // case: 3
                    
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b00001001,8'b00011001,8'b00101001,8'b00111001              
                  
	      8'b00000011,8'b00010011,8'b00100011,8'b00110011  :
                begin
		  // INC ss
		  TStates = 3'b110;
		  IncDec_16[3:2] = 2'b01;
		  IncDec_16[1:0] = DPAIR;
                end
              
	      8'b00001011,8'b00011011,8'b00101011,8'b00111011  :
                begin
		  // DEC ss
		  TStates = 3'b110;
		  IncDec_16[3:2] = 2'b11;
		  IncDec_16[1:0] = DPAIR;
                end

// ROTATE AND SHIFT GROUP
	      8'b00000111,
		  // RLCA
		  8'b00010111,
		  // RLA
		  8'b00001111,
		  // RRCA
		  8'b00011111 :
			  // RRA
                begin
		  Set_BusA_To[2:0] = 3'b111;
		  ALU_Op = 4'b1000;
		  Read_To_Reg = 1'b1;
		  Save_ALU = 1'b1;
                end // case: 8'b00000111,...
              

// JUMP GROUP
	      8'b11000011  :
                begin
		  // JP nn
		  MCycles = 3'b011;
		  case (MCycle)
		    2  :
                      begin
			Inc_PC = 1'b1;
			LDZ = 1'b1;
                      end
                    
		    3  :
                      begin
			Inc_PC = 1'b1;
			Jump = 1'b1;
                      end
                    
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b11000011
              
	      8'b11000010,8'b11001010,8'b11010010,8'b11011010,8'b11100010,8'b11101010,8'b11110010,8'b11111010  :
                begin
		  if (IR[5] == 1'b1 && Mode == 3 ) 
                    begin
		      case (IRB[4:3])
			2'b00  :
                          begin
			    // LD ($FF00+C),A
			    MCycles = 3'b010;
			    case (MCycle)
			      1  :
                                begin
				  Set_Addr_To = aBC;
				  Set_BusB_To	= 4'b0111;
                                end
			      2  :
                                begin
				  Write = 1'b1;
				  IORQ = 1'b1;
                                end
                              
			      default :;
			    endcase // case(MCycle)
                          end // case: 2'b00
                        
			2'b01  :
                          begin
			    // LD (nn),A
			    MCycles = 3'b100;
			    case (MCycle)
			      2  :
                                begin
				  Inc_PC = 1'b1;
				  LDZ = 1'b1;
                                end
                              
			      3  :
                                begin
				  Set_Addr_To = aZI;
				  Inc_PC = 1'b1;
				  Set_BusB_To = 4'b0111;
                                end
                              
			      4  :
				Write = 1'b1;
			      default :;
			    endcase // case(MCycle)
                          end // case: default :...
                        
			2'b10  :
                          begin
			    // LD A,($FF00+C)
			    MCycles = 3'b010;
			    case (MCycle)
			      1  :
				Set_Addr_To = aBC;
			      2  :
                                begin
				  Read_To_Acc = 1'b1;
				  IORQ = 1'b1;
                                end
			      default :;
			    endcase // case(MCycle)
                          end // case: 2'b10
                        
			2'b11  :
                          begin
			    // LD A,(nn)
			    MCycles = 3'b100;
			    case (MCycle)
			      2  :
                                begin
				  Inc_PC = 1'b1;
				  LDZ = 1'b1;
                                end
			      3  :
                                begin
				  Set_Addr_To = aZI;
				  Inc_PC = 1'b1;
                                end
			      4  :
				Read_To_Acc = 1'b1;
			      default :;
			    endcase // case(MCycle)
                          end
		      endcase
		    end 
                  else 
                    begin
		      // JP cc,nn
		      MCycles = 3'b011;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    LDZ = 1'b1;
                          end
			3  :
                          begin
			    Inc_PC = 1'b1;
			    if (is_cc_true(F, IR[5:3]) ) 
                              begin
				Jump = 1'b1;
			      end
                          end
                        
			default :;
		      endcase
		    end // else: !if(DPAIR == 2'b11 )
                end // case: 8'b11000010,8'b11001010,8'b11010010,8'b11011010,8'b11100010,8'b11101010,8'b11110010,8'b11111010
              
	      8'b00011000  :
                begin
		  if (Mode != 2 ) 
                    begin
		      // JR e
		      MCycles = 3'b011;
		      case (MCycle)
			2  :
			  Inc_PC = 1'b1;
			3  :
                          begin
			    NoRead = 1'b1;
			    JumpE = 1'b1;
			    TStates = 3'b101;
                          end
			default :;
		      endcase
		    end // if (Mode != 2 )
                end // case: 8'b00011000
              
	      8'b00111000  :
                begin
		  if (Mode != 2 ) 
                    begin
		      // JR C,e
		      MCycles = 3'b011;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    if (F[Flag_C] == 1'b0 ) 
                              begin
			        MCycles = 3'b010;
			      end
                          end
                        
			3  :
                          begin
			    NoRead = 1'b1;
			    JumpE = 1'b1;
			    TStates = 3'b101;
                          end
			default :;
		      endcase
		    end // if (Mode != 2 )
                end // case: 8'b00111000
              
	      8'b00110000  :
                begin
		  if (Mode != 2 ) 
                    begin
		      // JR NC,e
		      MCycles = 3'b011;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    if (F[Flag_C] == 1'b1 ) 
                              begin
			        MCycles = 3'b010;
			      end
                          end
                        
			3  :
                          begin
			    NoRead = 1'b1;
			    JumpE = 1'b1;
			    TStates = 3'b101;
                          end
			default :;
		      endcase
		    end // if (Mode != 2 )
                end // case: 8'b00110000
              
	      8'b00101000  :
                begin
		  if (Mode != 2 ) 
                    begin
		      // JR Z,e
		      MCycles = 3'b011;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    if (F[Flag_Z] == 1'b0 ) 
                              begin
				MCycles = 3'b010;
			      end
                          end
                        
			3  :
                          begin
			    NoRead = 1'b1;
			    JumpE = 1'b1;
			    TStates = 3'b101;
                          end
                        
			default :;
		      endcase
		    end // if (Mode != 2 )
                end // case: 8'b00101000
              
	      8'b00100000  :
                begin
		  if (Mode != 2 ) 
                    begin
		      // JR NZ,e
		      MCycles = 3'b011;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    if (F[Flag_Z] == 1'b1 ) 
                              begin
			        MCycles = 3'b010;
			      end
                          end
			3  :
                          begin                            
			    NoRead = 1'b1;
			    JumpE = 1'b1;
			    TStates = 3'b101;
                          end
			default :;
		      endcase
		    end // if (Mode != 2 )
                end // case: 8'b00100000
              
	      8'b11101001  :
		// JP (HL)
		JumpXY = 1'b1;
              
	      8'b00010000  :
                begin
		  if (Mode == 3 ) 
                    begin
		      I_DJNZ = 1'b1;
		    end 
                  else if (Mode < 2 ) 
                    begin
		      // DJNZ,e
		      MCycles = 3'b011;
		      case (MCycle)
			1  :
                          begin
			    TStates = 3'b101;
			    I_DJNZ = 1'b1;
			    Set_BusB_To = 4'b1010;
			    Set_BusA_To[2:0] = 3'b000;
			    Read_To_Reg = 1'b1;
			    Save_ALU = 1'b1;
			    ALU_Op = 4'b0010;
                          end
			2  :
                          begin
			    I_DJNZ = 1'b1;
			    Inc_PC = 1'b1;
                          end
			3  :
                          begin
			    NoRead = 1'b1;
			    JumpE = 1'b1;
			    TStates = 3'b101;
                          end
			default :;
		      endcase
		    end // if (Mode < 2 )
                end // case: 8'b00010000
              

// CALL AND RETURN GROUP
	      8'b11001101  :
                begin
		  // CALL nn
		  MCycles = 3'b101;
		  case (MCycle)
		    2  :
                      begin
			Inc_PC = 1'b1;
			LDZ = 1'b1;
                      end
		    3  :
                      begin
			IncDec_16 = 4'b1111;
			Inc_PC = 1'b1;
			TStates = 3'b100;
			Set_Addr_To = aSP;
			LDW = 1'b1;
			Set_BusB_To = 4'b1101;
                      end
		    4  :
                      begin
			Write = 1'b1;
			IncDec_16 = 4'b1111;
			Set_Addr_To = aSP;
			Set_BusB_To = 4'b1100;
                      end
		    5  :
                      begin
			Write = 1'b1;
			Call = 1'b1;
                      end
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b11001101
              
	      8'b11000100,8'b11001100,8'b11010100,8'b11011100,8'b11100100,8'b11101100,8'b11110100,8'b11111100  :
                begin
		  if (IR[5] == 1'b0 || Mode != 3 ) 
                    begin
		      // CALL cc,nn
		      MCycles = 3'b101;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    LDZ = 1'b1;
                          end
			3  :
                          begin
			    Inc_PC = 1'b1;
			    LDW = 1'b1;
			    if (is_cc_true(F, IR[5:3]) ) 
                              begin
			        IncDec_16 = 4'b1111;
				Set_Addr_To = aSP;
				TStates = 3'b100;
				Set_BusB_To = 4'b1101;
			      end 
                            else 
                              begin
				MCycles = 3'b011;
			      end // else: !if(is_cc_true(F, IR[5:3]) )
                          end // case: 3
                        
			4  :
                          begin
			    Write = 1'b1;
			    IncDec_16 = 4'b1111;
			    Set_Addr_To = aSP;
			    Set_BusB_To = 4'b1100;
                          end
                        
			5  :
                          begin
			    Write = 1'b1;
			    Call = 1'b1;
                          end
                        
			default :;
		      endcase
		    end // if (IR[5] == 1'b0 || Mode != 3 )
                end // case: 8'b11000100,8'b11001100,8'b11010100,8'b11011100,8'b11100100,8'b11101100,8'b11110100,8'b11111100
              
	      8'b11001001  :
                begin
		  // RET
		  MCycles = 3'b011;
		  case (MCycle)
		    1  :
                      begin
			TStates = 3'b101;
			Set_Addr_To = aSP;
                      end
                    
		    2  :
                      begin
			IncDec_16 = 4'b0111;
			Set_Addr_To = aSP;
			LDZ = 1'b1;
                      end
                    
		    3  :
                      begin
			Jump = 1'b1;
			IncDec_16 = 4'b0111;
                      end
                    
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b11001001
              
	      8'b11000000,8'b11001000,8'b11010000,8'b11011000,8'b11100000,8'b11101000,8'b11110000,8'b11111000  :
                begin                  
		  if (IR[5] == 1'b1 && Mode == 3 ) 
                    begin
		      case (IRB[4:3])
			2'b00  :
                          begin
			    // LD ($FF00+nn),A
			    MCycles = 3'b011;
			    case (MCycle)
			      2  :
                                begin
				  Inc_PC = 1'b1;
				  Set_Addr_To = aIOA;
				  Set_BusB_To	= 4'b0111;
                                end
                              
			      3  :
				Write = 1'b1;
			      default :;
			    endcase // case(MCycle)
                          end // case: 2'b00
                        
			2'b01  :
                          begin
			    // ADD SP,n
			    MCycles = 3'b011;
			    case (MCycle)
			      2  :
                                begin
				  ALU_Op = 4'b0000;
				  Inc_PC = 1'b1;
				  Read_To_Reg = 1'b1;
				  Save_ALU = 1'b1;
				  Set_BusA_To = 4'b1000;
				  Set_BusB_To = 4'b0110;
                                end
                              
			      3  :
                                begin
				  NoRead = 1'b1;
				  Read_To_Reg = 1'b1;
				  Save_ALU = 1'b1;
				  ALU_Op = 4'b0001;
				  Set_BusA_To = 4'b1001;
				  Set_BusB_To = 4'b1110;	// Incorrect unsigned !!!!!!!!!!!!!!!!!!!!!
                                end
                              
			      default :;
			    endcase // case(MCycle)
                          end // case: 2'b01
                        
			2'b10  :
                          begin
			    // LD A,($FF00+nn)
			    MCycles = 3'b011;
			    case (MCycle)
			      2  :
                                begin
				  Inc_PC = 1'b1;
				  Set_Addr_To = aIOA;
                                end
                              
			      3  :
				Read_To_Acc = 1'b1;
			      default :;
			    endcase // case(MCycle)
                          end // case: 2'b10
                        
			2'b11  :
                          begin
			    // LD HL,SP+n	-- Not correct !!!!!!!!!!!!!!!!!!!
			    MCycles = 3'b101;
			    case (MCycle)
			      2  :
                                begin
				  Inc_PC = 1'b1;
				  LDZ = 1'b1;
                                end
                              
			      3  :
                                begin
				  Set_Addr_To = aZI;
				  Inc_PC = 1'b1;
				  LDW = 1'b1;
                                end
                              
			      4  :
                                begin
				  Set_BusA_To[2:0] = 3'b101; // L
				  Read_To_Reg = 1'b1;
				  Inc_WZ = 1'b1;
				  Set_Addr_To = aZI;
                                end
                              
			      5  :
                                begin
				  Set_BusA_To[2:0] = 3'b100; // H
				  Read_To_Reg = 1'b1;
                                end
                              
			      default :;
			    endcase // case(MCycle)
                          end // case: 2'b11
                        
		      endcase // case(IRB[4:3])
                      
		    end 
                  else 
                    begin
		      // RET cc
		      MCycles = 3'b011;
		      case (MCycle)
			1  :
                          begin
			    if (is_cc_true(F, IR[5:3]) )                              
                              begin
				Set_Addr_To = aSP;
			      end 
                            else 
                              begin
                                MCycles = 3'b001;
			      end
			    TStates = 3'b101;
                          end // case: 1
                        
			2  :
	                  begin
			    IncDec_16 = 4'b0111;
			    Set_Addr_To = aSP;
			    LDZ = 1'b1;
                          end
			3  :
	                  begin
			    Jump = 1'b1;
			    IncDec_16 = 4'b0111;
                          end
			default :;
		      endcase
		    end // else: !if(IR[5] == 1'b1 && Mode == 3 )
                end // case: 8'b11000000,8'b11001000,8'b11010000,8'b11011000,8'b11100000,8'b11101000,8'b11110000,8'b11111000
              
	      8'b11000111,8'b11001111,8'b11010111,8'b11011111,8'b11100111,8'b11101111,8'b11110111,8'b11111111  :
                begin
		  // RST p
		  MCycles = 3'b011;
		  case (MCycle)
		    1  :
                      begin
			TStates = 3'b101;
			IncDec_16 = 4'b1111;
			Set_Addr_To = aSP;
			Set_BusB_To = 4'b1101;
                      end
                    
		    2  :
                      begin
			Write = 1'b1;
			IncDec_16 = 4'b1111;
			Set_Addr_To = aSP;
			Set_BusB_To = 4'b1100;
                      end
                    
		    3  :
                      begin
			Write = 1'b1;
			RstP = 1'b1;
                      end
                    
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b11000111,8'b11001111,8'b11010111,8'b11011111,8'b11100111,8'b11101111,8'b11110111,8'b11111111
              
// INPUT AND OUTPUT GROUP
	      8'b11011011  :
                begin
		  if (Mode != 3 ) 
                    begin
		      // IN A,(n)
		      MCycles = 3'b011;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    Set_Addr_To = aIOA;
                          end
                        
			3  :
                          begin
			    Read_To_Acc = 1'b1;
			    IORQ = 1'b1;
                          end
                        
			default :;
		      endcase
		    end // if (Mode != 3 )
                end // case: 8'b11011011
              
	      8'b11010011  :
                begin
		  if (Mode != 3 ) 
                    begin
		      // OUT (n),A
		      MCycles = 3'b011;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    Set_Addr_To = aIOA;
			    Set_BusB_To	= 4'b0111;
                          end
                        
			3  :
                          begin
			    Write = 1'b1;
			    IORQ = 1'b1;
                          end
                        
			default :;
		      endcase
		    end // if (Mode != 3 )
                end // case: 8'b11010011
              

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// MULTIBYTE INSTRUCTIONS
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

	      8'b11001011  :
                begin
		  if (Mode != 2 ) 
                    begin
		      Prefix = 2'b01;
		    end
                end              

	      8'b11101101  :
                begin
		  if (Mode < 2 ) 
                    begin
		      Prefix = 2'b10;
		    end
                end

	      8'b11011101,8'b11111101  :
                begin
		  if (Mode < 2 ) 
                    begin
		      Prefix = 2'b11;
		    end
                end
              
	    endcase // case(IRB)
          end // case: 2'b00
        

	2'b01  :
          begin
            

            //----------------------------------------------------------------------------
            //
            //	CB prefixed instructions
            //
            //----------------------------------------------------------------------------
            
	    Set_BusA_To[2:0] = IR[2:0];
	    Set_BusB_To[2:0] = IR[2:0];
            
	    case (IRB)
	      8'b00000000,8'b00000001,8'b00000010,8'b00000011,8'b00000100,8'b00000101,8'b00000111,
	      8'b00010000,8'b00010001,8'b00010010,8'b00010011,8'b00010100,8'b00010101,8'b00010111,
	      8'b00001000,8'b00001001,8'b00001010,8'b00001011,8'b00001100,8'b00001101,8'b00001111,
	      8'b00011000,8'b00011001,8'b00011010,8'b00011011,8'b00011100,8'b00011101,8'b00011111,
	      8'b00100000,8'b00100001,8'b00100010,8'b00100011,8'b00100100,8'b00100101,8'b00100111,
	      8'b00101000,8'b00101001,8'b00101010,8'b00101011,8'b00101100,8'b00101101,8'b00101111,
	      8'b00110000,8'b00110001,8'b00110010,8'b00110011,8'b00110100,8'b00110101,8'b00110111,
	      8'b00111000,8'b00111001,8'b00111010,8'b00111011,8'b00111100,8'b00111101,8'b00111111 :
                begin
		  // RLC r
		  // RL r
		  // RRC r
		  // RR r
		  // SLA r
		  // SRA r
		  // SRL r
		  // SLL r (Undocumented) / SWAP r
		  if (MCycle == 3'b001 ) begin
		    ALU_Op = 4'b1000;
		    Read_To_Reg = 1'b1;
		    Save_ALU = 1'b1;
		  end
                end // case: 8'b00000000,8'b00000001,8'b00000010,8'b00000011,8'b00000100,8'b00000101,8'b00000111,...
              
	      8'b00000110,8'b00010110,8'b00001110,8'b00011110,8'b00101110,8'b00111110,8'b00100110,8'b00110110  :
                begin
		  // RLC (HL)
		  // RL (HL)
		  // RRC (HL)
		  // RR (HL)
		  // SRA (HL)
		  // SRL (HL)
		  // SLA (HL)
		  // SLL (HL) (Undocumented) / SWAP (HL)
		  MCycles = 3'b011;
		  case (MCycle)
		    1 , 7  :
		      Set_Addr_To = aXY;
		    2  :
                      begin
			ALU_Op = 4'b1000;
			Read_To_Reg = 1'b1;
			Save_ALU = 1'b1;
			Set_Addr_To = aXY;
			TStates = 3'b100;
                      end
                    
		    3  :
		      Write = 1'b1;
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b00000110,8'b00010110,8'b00001110,8'b00011110,8'b00101110,8'b00111110,8'b00100110,8'b00110110
              
	      8'b01000000,8'b01000001,8'b01000010,8'b01000011,8'b01000100,8'b01000101,8'b01000111,
		  8'b01001000,8'b01001001,8'b01001010,8'b01001011,8'b01001100,8'b01001101,8'b01001111,
		  8'b01010000,8'b01010001,8'b01010010,8'b01010011,8'b01010100,8'b01010101,8'b01010111,
		  8'b01011000,8'b01011001,8'b01011010,8'b01011011,8'b01011100,8'b01011101,8'b01011111,
		  8'b01100000,8'b01100001,8'b01100010,8'b01100011,8'b01100100,8'b01100101,8'b01100111,
		  8'b01101000,8'b01101001,8'b01101010,8'b01101011,8'b01101100,8'b01101101,8'b01101111,
		  8'b01110000,8'b01110001,8'b01110010,8'b01110011,8'b01110100,8'b01110101,8'b01110111,
		  8'b01111000,8'b01111001,8'b01111010,8'b01111011,8'b01111100,8'b01111101,8'b01111111 :
                    begin
		      // BIT b,r
		      if (MCycle == 3'b001 )
                        begin
                          Set_BusB_To[2:0] = IR[2:0];
			  ALU_Op = 4'b1001;
		        end
                    end // case: 8'b01000000,8'b01000001,8'b01000010,8'b01000011,8'b01000100,8'b01000101,8'b01000111,...
              
	      8'b01000110,8'b01001110,8'b01010110,8'b01011110,8'b01100110,8'b01101110,8'b01110110,8'b01111110  :
                begin
		  // BIT b,(HL)
		  MCycles = 3'b010;
		  case (MCycle)
		    1 , 7  :
		      Set_Addr_To = aXY;
		    2  :
                      begin
			ALU_Op = 4'b1001;
			TStates = 3'b100;
                      end
                    
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b01000110,8'b01001110,8'b01010110,8'b01011110,8'b01100110,8'b01101110,8'b01110110,8'b01111110
              
	      8'b11000000,8'b11000001,8'b11000010,8'b11000011,8'b11000100,8'b11000101,8'b11000111,
		  8'b11001000,8'b11001001,8'b11001010,8'b11001011,8'b11001100,8'b11001101,8'b11001111,
		  8'b11010000,8'b11010001,8'b11010010,8'b11010011,8'b11010100,8'b11010101,8'b11010111,
		  8'b11011000,8'b11011001,8'b11011010,8'b11011011,8'b11011100,8'b11011101,8'b11011111,
		  8'b11100000,8'b11100001,8'b11100010,8'b11100011,8'b11100100,8'b11100101,8'b11100111,
		  8'b11101000,8'b11101001,8'b11101010,8'b11101011,8'b11101100,8'b11101101,8'b11101111,
		  8'b11110000,8'b11110001,8'b11110010,8'b11110011,8'b11110100,8'b11110101,8'b11110111,
		  8'b11111000,8'b11111001,8'b11111010,8'b11111011,8'b11111100,8'b11111101,8'b11111111 :
                    begin
		      // SET b,r
		      if (MCycle == 3'b001 ) 
                        begin
			  ALU_Op = 4'b1010;
			  Read_To_Reg = 1'b1;
			  Save_ALU = 1'b1;
		        end
                    end // case: 8'b11000000,8'b11000001,8'b11000010,8'b11000011,8'b11000100,8'b11000101,8'b11000111,...
              
	      8'b11000110,8'b11001110,8'b11010110,8'b11011110,8'b11100110,8'b11101110,8'b11110110,8'b11111110  :
                begin
		  // SET b,(HL)
		  MCycles = 3'b011;
		  case (MCycle)
		    1 , 7  :
		      Set_Addr_To = aXY;
		    2  :
                      begin
			ALU_Op = 4'b1010;
			Read_To_Reg = 1'b1;
			Save_ALU = 1'b1;
			Set_Addr_To = aXY;
			TStates = 3'b100;
                      end
		    3  :
		      Write = 1'b1;
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b11000110,8'b11001110,8'b11010110,8'b11011110,8'b11100110,8'b11101110,8'b11110110,8'b11111110
              
	      8'b10000000,8'b10000001,8'b10000010,8'b10000011,8'b10000100,8'b10000101,8'b10000111,
		  8'b10001000,8'b10001001,8'b10001010,8'b10001011,8'b10001100,8'b10001101,8'b10001111,
		  8'b10010000,8'b10010001,8'b10010010,8'b10010011,8'b10010100,8'b10010101,8'b10010111,
		  8'b10011000,8'b10011001,8'b10011010,8'b10011011,8'b10011100,8'b10011101,8'b10011111,
		  8'b10100000,8'b10100001,8'b10100010,8'b10100011,8'b10100100,8'b10100101,8'b10100111,
		  8'b10101000,8'b10101001,8'b10101010,8'b10101011,8'b10101100,8'b10101101,8'b10101111,
		  8'b10110000,8'b10110001,8'b10110010,8'b10110011,8'b10110100,8'b10110101,8'b10110111,
		  8'b10111000,8'b10111001,8'b10111010,8'b10111011,8'b10111100,8'b10111101,8'b10111111 :
                    begin
		      // RES b,r
		      if (MCycle == 3'b001 ) 
                        begin
			  ALU_Op = 4'b1011;
			  Read_To_Reg = 1'b1;
			  Save_ALU = 1'b1;
		        end
                    end // case: 8'b10000000,8'b10000001,8'b10000010,8'b10000011,8'b10000100,8'b10000101,8'b10000111,...
              
	      8'b10000110,8'b10001110,8'b10010110,8'b10011110,8'b10100110,8'b10101110,8'b10110110,8'b10111110  :
                begin
		  // RES b,(HL)
		  MCycles = 3'b011;
		  case (MCycle)
		    1 , 7  :
		      Set_Addr_To = aXY;
		    2  :
                      begin
			ALU_Op = 4'b1011;
			Read_To_Reg = 1'b1;
			Save_ALU = 1'b1;
			Set_Addr_To = aXY;
			TStates = 3'b100;
                      end
                    
		    3  :
		      Write = 1'b1;
		    default :;
		  endcase // case(MCycle)
                end // case: 8'b10000110,8'b10001110,8'b10010110,8'b10011110,8'b10100110,8'b10101110,8'b10110110,8'b10111110
              
	    endcase // case(IRB)
          end // case: 2'b01
        
            
	default :
          begin : default_ed_block
                  
            //----------------------------------------------------------------------------
            //
            //	ED prefixed instructions
            //
            //----------------------------------------------------------------------------

	    case (IRB)
	      8'b00000000,8'b00000001,8'b00000010,8'b00000011,8'b00000100,8'b00000101,8'b00000110,8'b00000111
		,8'b00001000,8'b00001001,8'b00001010,8'b00001011,8'b00001100,8'b00001101,8'b00001110,8'b00001111
		  ,8'b00010000,8'b00010001,8'b00010010,8'b00010011,8'b00010100,8'b00010101,8'b00010110,8'b00010111
		    ,8'b00011000,8'b00011001,8'b00011010,8'b00011011,8'b00011100,8'b00011101,8'b00011110,8'b00011111
		      ,8'b00100000,8'b00100001,8'b00100010,8'b00100011,8'b00100100,8'b00100101,8'b00100110,8'b00100111
			,8'b00101000,8'b00101001,8'b00101010,8'b00101011,8'b00101100,8'b00101101,8'b00101110,8'b00101111
			  ,8'b00110000,8'b00110001,8'b00110010,8'b00110011,8'b00110100,8'b00110101,8'b00110110,8'b00110111
			    ,8'b00111000,8'b00111001,8'b00111010,8'b00111011,8'b00111100,8'b00111101,8'b00111110,8'b00111111
                              

				  ,8'b10000000,8'b10000001,8'b10000010,8'b10000011,8'b10000100,8'b10000101,8'b10000110,8'b10000111
				    ,8'b10001000,8'b10001001,8'b10001010,8'b10001011,8'b10001100,8'b10001101,8'b10001110,8'b10001111
				      ,8'b10010000,8'b10010001,8'b10010010,8'b10010011,8'b10010100,8'b10010101,8'b10010110,8'b10010111
				        ,8'b10011000,8'b10011001,8'b10011010,8'b10011011,8'b10011100,8'b10011101,8'b10011110,8'b10011111
				          ,                                            8'b10100100,8'b10100101,8'b10100110,8'b10100111
				            ,                                            8'b10101100,8'b10101101,8'b10101110,8'b10101111
				              ,                                            8'b10110100,8'b10110101,8'b10110110,8'b10110111
				                ,                                            8'b10111100,8'b10111101,8'b10111110,8'b10111111
				                  ,8'b11000000,8'b11000001,8'b11000010,8'b11000011,8'b11000100,8'b11000101,8'b11000110,8'b11000111
				                    ,8'b11001000,8'b11001001,8'b11001010,8'b11001011,8'b11001100,8'b11001101,8'b11001110,8'b11001111
				                      ,8'b11010000,8'b11010001,8'b11010010,8'b11010011,8'b11010100,8'b11010101,8'b11010110,8'b11010111
				                        ,8'b11011000,8'b11011001,8'b11011010,8'b11011011,8'b11011100,8'b11011101,8'b11011110,8'b11011111
				                          ,8'b11100000,8'b11100001,8'b11100010,8'b11100011,8'b11100100,8'b11100101,8'b11100110,8'b11100111
				                            ,8'b11101000,8'b11101001,8'b11101010,8'b11101011,8'b11101100,8'b11101101,8'b11101110,8'b11101111
				                              ,8'b11110000,8'b11110001,8'b11110010,8'b11110011,8'b11110100,8'b11110101,8'b11110110,8'b11110111
				                                ,8'b11111000,8'b11111001,8'b11111010,8'b11111011,8'b11111100,8'b11111101,8'b11111110,8'b11111111 :
				; // NOP, undocumented
                  
		  8'b01111110,8'b01111111  :
		    // NOP, undocumented
		;
                  // 8 BIT LOAD GROUP
		  8'b01010111  :
                    begin
		      // LD A,I
		      Special_LD = 3'b100;
		      TStates = 3'b101;
                    end
                  
		  8'b01011111  :
                    begin
		      // LD A,R
		      Special_LD = 3'b101;
		      TStates = 3'b101;
                    end
                  
		  8'b01000111  :
                    begin
		      // LD I,A
		      Special_LD = 3'b110;
		      TStates = 3'b101;
                    end
                  
		  8'b01001111  :
                    begin
		      // LD R,A
		      Special_LD = 3'b111;
		      TStates = 3'b101;
                    end
                  
                  // 16 BIT LOAD GROUP
		  8'b01001011,8'b01011011,8'b01101011,8'b01111011  :
                    begin
		      // LD dd,(nn)
		      MCycles = 3'b101;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    LDZ = 1'b1;
                          end
                        
			3  :
                          begin
			    Set_Addr_To = aZI;
			    Inc_PC = 1'b1;
			    LDW = 1'b1;
                          end
                        
			4  :
                          begin
			    Read_To_Reg = 1'b1;
			    if (IR[5:4] == 2'b11 ) 
                              begin
				Set_BusA_To = 4'b1000;
			      end 
                            else 
                              begin
				Set_BusA_To[2:1] = IR[5:4];
				Set_BusA_To[0] = 1'b1;
			      end
			    Inc_WZ = 1'b1;
			    Set_Addr_To = aZI;
                          end // case: 4
                        
			5  :
                          begin
			    Read_To_Reg = 1'b1;
			    if (IR[5:4] == 2'b11 ) 
                              begin
				Set_BusA_To = 4'b1001;
			      end 
                            else 
                              begin
				Set_BusA_To[2:1] = IR[5:4];
				Set_BusA_To[0] = 1'b0;
			      end
                          end // case: 5
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b01001011,8'b01011011,8'b01101011,8'b01111011
                  
                  
		  8'b01000011,8'b01010011,8'b01100011,8'b01110011  :
                    begin
		      // LD (nn),dd
		      MCycles = 3'b101;
		      case (MCycle)
			2  :
                          begin
			    Inc_PC = 1'b1;
			    LDZ = 1'b1;
                          end
                        
			3  :
                          begin
			    Set_Addr_To = aZI;
			    Inc_PC = 1'b1;
			    LDW = 1'b1;
			    if (IR[5:4] == 2'b11 ) 
                              begin
				Set_BusB_To = 4'b1000;
			      end 
                            else 
                              begin
				Set_BusB_To[2:1] = IR[5:4];
				Set_BusB_To[0] = 1'b1;
				Set_BusB_To[3] = 1'b0;
			      end
                          end // case: 3
                        
			4  :
                          begin
			    Inc_WZ = 1'b1;
			    Set_Addr_To = aZI;
			    Write = 1'b1;
			    if (IR[5:4] == 2'b11 ) 
                              begin
				Set_BusB_To = 4'b1001;
			      end 
                            else 
                              begin
				Set_BusB_To[2:1] = IR[5:4];
				Set_BusB_To[0] = 1'b0;
				Set_BusB_To[3] = 1'b0;
			      end
                          end // case: 4
                        
			5  :
                          begin
			    Write = 1'b1;
                          end
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b01000011,8'b01010011,8'b01100011,8'b01110011
                  
		  8'b10100000 , 8'b10101000 , 8'b10110000 , 8'b10111000  :
                    begin
		      // LDI, LDD, LDIR, LDDR
		      MCycles = 3'b100;
		      case (MCycle)
			1  :
                          begin
			    Set_Addr_To = aXY;
			    IncDec_16 = 4'b1100; // BC
                          end
                        
			2  :
                          begin
			    Set_BusB_To = 4'b0110;
			    Set_BusA_To[2:0] = 3'b111;
			    ALU_Op = 4'b0000;
			    Set_Addr_To = aDE;
			    if (IR[3] == 1'b0 ) 
                              begin
				IncDec_16 = 4'b0110; // IX
			      end 
                            else 
                              begin
				IncDec_16 = 4'b1110;
			      end
                          end // case: 2
                        
			3  :
                          begin
			    I_BT = 1'b1;
			    TStates = 3'b101;
			    Write = 1'b1;
			    if (IR[3] == 1'b0 ) 
                              begin
				IncDec_16 = 4'b0101; // DE
			      end 
                            else 
                              begin
				IncDec_16 = 4'b1101;
			      end
                          end // case: 3
                        
			4  :
                          begin
			    NoRead = 1'b1;
			    TStates = 3'b101;
                          end
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b10100000 , 8'b10101000 , 8'b10110000 , 8'b10111000
                  
		  8'b10100001 , 8'b10101001 , 8'b10110001 , 8'b10111001  :
                    begin
		      // CPI, CPD, CPIR, CPDR
		      MCycles = 3'b100;
		      case (MCycle)
			1  :
                          begin
			    Set_Addr_To = aXY;
			    IncDec_16 = 4'b1100; // BC
                          end
                        
			2  :
                          begin
			    Set_BusB_To = 4'b0110;
			    Set_BusA_To[2:0] = 3'b111;
			    ALU_Op = 4'b0111;
			    Save_ALU = 1'b1;
			    PreserveC = 1'b1;
			    if (IR[3] == 1'b0 ) 
                              begin
				IncDec_16 = 4'b0110;
			      end 
                            else 
                              begin
				IncDec_16 = 4'b1110;
			      end
                          end // case: 2
                        
			3  :
                          begin
			    NoRead = 1'b1;
			    I_BC = 1'b1;
			    TStates = 3'b101;
                          end
                        
			4  :
                          begin
			    NoRead = 1'b1;
			    TStates = 3'b101;
                          end
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b10100001 , 8'b10101001 , 8'b10110001 , 8'b10111001
                  
		  8'b01000100,8'b01001100,8'b01010100,8'b01011100,8'b01100100,8'b01101100,8'b01110100,8'b01111100  :
                    begin
		      // NEG
		      ALU_Op = 4'b0010;
		      Set_BusB_To = 4'b0111;
		      Set_BusA_To = 4'b1010;
		      Read_To_Acc = 1'b1;
		      Save_ALU = 1'b1;
                    end
                  
		  8'b01000110,8'b01001110,8'b01100110,8'b01101110  :
                    begin
		      // IM 0
		      IMode = 2'b00;
                    end
                  
		  8'b01010110,8'b01110110  :
		    // IM 1
		    IMode = 2'b01;
                  
		  8'b01011110,8'b01110111  :
		    // IM 2
		    IMode = 2'b10;
                  
                  // 16 bit arithmetic
		  8'b01001010,8'b01011010,8'b01101010,8'b01111010  :
                    begin
		      // ADC HL,ss
		      MCycles = 3'b011;
		      case (MCycle)
			2  :
                          begin
			    NoRead = 1'b1;
			    ALU_Op = 4'b0001;
			    Read_To_Reg = 1'b1;
			    Save_ALU = 1'b1;
			    Set_BusA_To[2:0] = 3'b101;
			    case (IR[5:4])
			      0,1,2  :
                                begin
				  Set_BusB_To[2:1] = IR[5:4];
				  Set_BusB_To[0] = 1'b1;
                                end
			      default :
				Set_BusB_To = 4'b1000;
			    endcase
			    TStates = 3'b100;
                          end // case: 2
                        
			3  :
                          begin
			    NoRead = 1'b1;
			    Read_To_Reg = 1'b1;
			    Save_ALU = 1'b1;
			    ALU_Op = 4'b0001;
			    Set_BusA_To[2:0] = 3'b100;
			    case (IR[5:4])
			      0,1,2  :
                                begin
				  Set_BusB_To[2:1] = IR[5:4];
				  Set_BusB_To[0] = 1'b0;
                                end
			      default :
				Set_BusB_To = 4'b1001;
			    endcase // case(IR[5:4])
                          end // case: 3
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b01001010,8'b01011010,8'b01101010,8'b01111010
                  
		  8'b01000010,8'b01010010,8'b01100010,8'b01110010  :
                    begin
		      // SBC HL,ss
		      MCycles = 3'b011;
		      case (MCycle)
			2  :
                          begin
			    NoRead = 1'b1;
			    ALU_Op = 4'b0011;
			    Read_To_Reg = 1'b1;
			    Save_ALU = 1'b1;
			    Set_BusA_To[2:0] = 3'b101;
			    case (IR[5:4])
			      0,1,2  :
                                begin
				  Set_BusB_To[2:1] = IR[5:4];
				  Set_BusB_To[0] = 1'b1;
                                end
			      default :
				Set_BusB_To = 4'b1000;
			    endcase
			    TStates = 3'b100;
                          end // case: 2
                        
			3  :
                          begin
			    NoRead = 1'b1;
			    ALU_Op = 4'b0011;
			    Read_To_Reg = 1'b1;
			    Save_ALU = 1'b1;
			    Set_BusA_To[2:0] = 3'b100;
			    case (IR[5:4])
			      0,1,2  :
				Set_BusB_To[2:1] = IR[5:4];
			      default :
				Set_BusB_To = 4'b1001;
			    endcase
                          end // case: 3
                        
			default :;
                        
		      endcase // case(MCycle)
                    end // case: 8'b01000010,8'b01010010,8'b01100010,8'b01110010
                  
		  8'b01101111  :
                    begin
		      // RLD
		      MCycles = 3'b100;
		      case (MCycle)
			2  :
                          begin
			    NoRead = 1'b1;
			    Set_Addr_To = aXY;
                          end
                        
			3  :
                          begin
			    Read_To_Reg = 1'b1;
			    Set_BusB_To[2:0] = 3'b110;
			    Set_BusA_To[2:0] = 3'b111;
			    ALU_Op = 4'b1101;
			    TStates = 3'b100;
			    Set_Addr_To = aXY;
			    Save_ALU = 1'b1;
                          end
                        
			4  :
                          begin
			    I_RLD = 1'b1;
			    Write = 1'b1;
                          end
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b01101111
                  
		  8'b01100111  :
                    begin
		      // RRD
		      MCycles = 3'b100;
		      case (MCycle)
			2  :
			  Set_Addr_To = aXY;
			3  :
                          begin
			    Read_To_Reg = 1'b1;
			    Set_BusB_To[2:0] = 3'b110;
			    Set_BusA_To[2:0] = 3'b111;
			    ALU_Op = 4'b1110;
			    TStates = 3'b100;
			    Set_Addr_To = aXY;
			    Save_ALU = 1'b1;
                          end
                        
			4  :
                          begin
			    I_RRD = 1'b1;
			    Write = 1'b1;
                          end
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b01100111
                  
		  8'b01000101,8'b01001101,8'b01010101,8'b01011101,8'b01100101,8'b01101101,8'b01110101,8'b01111101  :
                    begin
		      // RETI, RETN
		      MCycles = 3'b011;
		      case (MCycle)
			1  :
			  Set_Addr_To = aSP;
                        
			2  :
                          begin
			    IncDec_16 = 4'b0111;
			    Set_Addr_To = aSP;
			    LDZ = 1'b1;
                          end
                        
			3  :
                          begin
			    Jump = 1'b1;
			    IncDec_16 = 4'b0111;
			    I_RETN = 1'b1;
                          end
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b01000101,8'b01001101,8'b01010101,8'b01011101,8'b01100101,8'b01101101,8'b01110101,8'b01111101
                  
		  8'b01000000,8'b01001000,8'b01010000,8'b01011000,8'b01100000,8'b01101000,8'b01110000,8'b01111000  :
                    begin
		      // IN r,(C)
		      MCycles = 3'b010;
		      case (MCycle)
			1  :
			  Set_Addr_To = aBC;
                        
			2  :
                          begin
			    IORQ = 1'b1;
			    if (IR[5:3] != 3'b110 ) 
                              begin
				Read_To_Reg = 1'b1;
				Set_BusA_To[2:0] = IR[5:3];
			      end
			    I_INRC = 1'b1;
                          end
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b01000000,8'b01001000,8'b01010000,8'b01011000,8'b01100000,8'b01101000,8'b01110000,8'b01111000
                  
		  8'b01000001,8'b01001001,8'b01010001,8'b01011001,8'b01100001,8'b01101001,8'b01110001,8'b01111001  :
                    begin
		      // OUT (C),r
		      // OUT (C),0
		      MCycles = 3'b010;
		      case (MCycle)
			1  :
                          begin
			    Set_Addr_To = aBC;
			    Set_BusB_To[2:0]	= IR[5:3];
			    if (IR[5:3] == 3'b110 ) 
                              begin
				Set_BusB_To[3] = 1'b1;
			      end
                          end
                        
			2  :
                          begin
			    Write = 1'b1;
			    IORQ = 1'b1;
                          end
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b01000001,8'b01001001,8'b01010001,8'b01011001,8'b01100001,8'b01101001,8'b01110001,8'b01111001
                  
		  8'b10100010 , 8'b10101010 , 8'b10110010 , 8'b10111010  :
                    begin
		      // INI, IND, INIR, INDR
		      MCycles = 3'b100;
		      case (MCycle)
			1  :
                          begin
			    Set_Addr_To = aBC;
			    Set_BusB_To = 4'b1010;
			    Set_BusA_To = 4'b0000;
			    Read_To_Reg = 1'b1;
			    Save_ALU = 1'b1;
			    ALU_Op = 4'b0010;
                          end
                        
			2  :
                          begin
			    IORQ = 1'b1;
			    Set_BusB_To = 4'b0110;
			    Set_Addr_To = aXY;
                          end
                        
			3  :
                          begin
			    if (IR[3] == 1'b0 ) 
                              begin
				IncDec_16 = 4'b0010;
			      end 
                            else 
                              begin
				IncDec_16 = 4'b1010;
			      end
			    TStates = 3'b100;
			    Write = 1'b1;
			    I_BTR = 1'b1;
                          end // case: 3
                        
			4  :
                          begin
			    NoRead = 1'b1;
			    TStates = 3'b101;
                          end
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b10100010 , 8'b10101010 , 8'b10110010 , 8'b10111010
                  
		  8'b10100011 , 8'b10101011 , 8'b10110011 , 8'b10111011  :
                    begin
		      // OUTI, OUTD, OTIR, OTDR
		      MCycles = 3'b100;
		      case (MCycle)
			1  :
                          begin
			    TStates = 3'b101;
			    Set_Addr_To = aXY;
			    Set_BusB_To = 4'b1010;
			    Set_BusA_To = 4'b0000;
			    Read_To_Reg = 1'b1;
			    Save_ALU = 1'b1;
			    ALU_Op = 4'b0010;
                          end
                        
			2  :
                          begin
			    Set_BusB_To = 4'b0110;
			    Set_Addr_To = aBC;
                          end
                        
			3  :
                          begin
			    if (IR[3] == 1'b0 ) 
                              begin
				IncDec_16 = 4'b0010;
			      end 
                            else 
                              begin
				IncDec_16 = 4'b1010;
			      end
			    IORQ = 1'b1;
			    Write = 1'b1;
			    I_BTR = 1'b1;
                          end // case: 3
                        
			4  :
                          begin
			    NoRead = 1'b1;
			    TStates = 3'b101;
                          end
                        
			default :;
		      endcase // case(MCycle)
                    end // case: 8'b10100011 , 8'b10101011 , 8'b10110011 , 8'b10111011
                  
		endcase // case(IRB)                  
          end // block: default_ed_block        
      endcase // case(ISet)
         
      if (Mode == 1 ) 
        begin
	  if (MCycle == 3'b001 ) 
            begin
              //TStates = 3'b100;
	    end 
          else 
            begin
	      TStates = 3'b011;
	    end
	end

      if (Mode == 3 ) 
        begin
	  if (MCycle == 3'b001 ) 
            begin
	      //TStates = 3'b100;
	    end 
          else 
            begin
	      TStates = 3'b100;
	    end
	end

      if (Mode < 2 ) 
        begin
	  if (MCycle == 3'b110 ) 
            begin
	      Inc_PC = 1'b1;
	      if (Mode == 1 ) 
                begin
		  Set_Addr_To = aXY;
		  TStates = 3'b100;
		  Set_BusB_To[2:0] = SSS;
		  Set_BusB_To[3] = 1'b0;
		end
	      if (IRB == 8'b00110110 || IRB == 8'b11001011 ) 
                begin
		  Set_Addr_To = aNone;
		end
	    end
	  if (MCycle == 3'b111 ) 
            begin
	      if (Mode == 0 ) 
                begin
		  TStates = 3'b101;
		end
	      if (ISet != 2'b01 ) 
                begin
		  Set_Addr_To = aXY;
		end
	      Set_BusB_To[2:0] = SSS;
	      Set_BusB_To[3] = 1'b0;
	      if (IRB == 8'b00110110 || ISet == 2'b01 ) 
                begin
		  // LD (HL),n
		  Inc_PC = 1'b1;
		end 
              else 
                begin
		  NoRead = 1'b1;
		end
	    end
	end // if (Mode < 2 )      
      
    end // always @ (IR, ISet, MCycle, F, NMICycle, IntCycle)
  
// synopsys dc_script_begin
// set_attribute current_design "revision" "$Id: tv80_mcode.v,v 1.1 2004/05/16 17:39:57 ghutchis Exp $" -type string -quiet
// synopsys dc_script_end
endmodule // T80_MCode



