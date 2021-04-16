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

module tv80_core (/*AUTOARG*/
  // Outputs
  m1_n, iorq, no_read, write, rfsh_n, halt_n, busak_n, A, do, mc, ts, 
  intcycle_n, IntE, stop, 
  // Inputs
  reset_n, clk, cen, wait_n, int_n, nmi_n, busrq_n, dinst, di
  );
  // Beginning of automatic inputs (from unused autoinst inputs)
  // End of automatics
  
  parameter Mode = 1;	// 0 => Z80, 1 => Fast Z80, 2 => 8080, 3 => GB
  parameter IOWait = 1;	// 1 => Single cycle I/O, 1 => Std I/O cycle
  parameter Flag_C = 0;
  parameter Flag_N = 1;
  parameter Flag_P = 2;
  parameter Flag_X = 3;
  parameter Flag_H = 4;
  parameter Flag_Y = 5;
  parameter Flag_Z = 6;
  parameter Flag_S = 7;

  input     reset_n;		
  input     clk;		
  input     cen;		
  input     wait_n;		
  input     int_n;		
  input     nmi_n;		
  input     busrq_n;		
  output    m1_n;		
  output    iorq;		
  output    no_read;		
  output    write;		
  output    rfsh_n;		
  output    halt_n;		
  output    busak_n;		
  output [15:0] A; 
  input [7:0]   dinst;	
  input [7:0]   di;	
  output [7:0]  do;	
  output [2:0]  mc;	
  output [2:0]  ts;	
  output	intcycle_n;	
  output	IntE;		
  output	stop;		

  reg    m1_n;		
  reg    iorq;		
  reg    rfsh_n;		
  reg    halt_n;		
  reg    busak_n;		
  reg [15:0] A; 
  reg [7:0]  do;	
  reg [2:0]  mc;	
  reg [2:0]  ts;	
  reg	intcycle_n;	
  reg	IntE;		
  reg	stop;		

  parameter     aNone	 = 3'b111;
  parameter     aBC	 = 3'b000;
  parameter     aDE	 = 3'b001;
  parameter     aXY	 = 3'b010;
  parameter     aIOA	 = 3'b100;
  parameter     aSP	 = 3'b101;
  parameter     aZI	 = 3'b110;

  // Registers
  reg [7:0]     ACC, F;
  reg [7:0]     Ap, Fp;
  reg [7:0]     I;
  reg [7:0]     R;
  reg [15:0]    SP, PC;
  reg [7:0]     RegDIH;
  reg [7:0]     RegDIL;
  wire [15:0]   RegBusA;
  wire [15:0]   RegBusB;
  wire [15:0]   RegBusC;
  reg [2:0]     RegAddrA_r;
  reg [2:0]     RegAddrA;
  reg [2:0]     RegAddrB_r;
  reg [2:0]     RegAddrB;
  reg [2:0]     RegAddrC;
  reg           RegWEH;
  reg           RegWEL;
  reg           Alternate;

  // Help Registers
  reg [15:0]    TmpAddr;	// Temporary address register
  reg [7:0]     IR;		// Instruction register
  reg [1:0]     ISet;		// Instruction set selector
  reg [15:0]    RegBusA_r;

  reg [15:0]    ID16;
  reg [7:0]     Save_Mux;

  reg [2:0]     tstate;
  reg [2:0]     mcycle;
  reg           IntE_FF1;
  reg           IntE_FF2;
  reg           Halt_FF;
  reg           BusReq_s;
  reg           BusAck;
  reg           ClkEn;
  reg           NMI_s;
  reg           INT_s;
  reg [1:0]     IStatus;

  reg [7:0]     DI_Reg;
  reg           T_Res;
  reg [1:0]     XY_State;
  reg [2:0]     Pre_XY_F_M;
  reg           NextIs_XY_Fetch;
  reg           XY_Ind;
  reg           No_BTR;
  reg           BTR_r;
  reg           Auto_Wait;
  reg           Auto_Wait_t1;
  reg           Auto_Wait_t2;
  reg           IncDecZ;

  // ALU signals
  reg [7:0]     BusB;
  reg [7:0]     BusA;
  wire [7:0]    ALU_Q;
  wire [7:0]    F_Out;

  // Registered micro code outputs
  reg [4:0]     Read_To_Reg_r;
  reg           Arith16_r;
  reg           Z16_r;
  reg [3:0]     ALU_Op_r;
  reg           Save_ALU_r;
  reg           PreserveC_r;
  reg [2:0]     mcycles;

  // Micro code outputs
  wire [2:0]    mcycles_d;
  wire [2:0]    tstates;
  reg           IntCycle;
  reg           NMICycle;
  wire          Inc_PC;
  wire          Inc_WZ;
  wire [3:0]    IncDec_16;
  wire [1:0]    Prefix;
  wire          Read_To_Acc;
  wire          Read_To_Reg;
  wire [3:0]     Set_BusB_To;
  wire [3:0]     Set_BusA_To;
  wire [3:0]     ALU_Op;
  wire           Save_ALU;
  wire           PreserveC;
  wire           Arith16;
  wire [2:0]     Set_Addr_To;
  wire           Jump;
  wire           JumpE;
  wire           JumpXY;
  wire           Call;
  wire           RstP;
  wire           LDZ;
  wire           LDW;
  wire           LDSPHL;
  wire           iorq_i;
  wire [2:0]     Special_LD;
  wire           ExchangeDH;
  wire           ExchangeRp;
  wire           ExchangeAF;
  wire           ExchangeRS;
  wire           I_DJNZ;
  wire           I_CPL;
  wire           I_CCF;
  wire           I_SCF;
  wire           I_RETN;
  wire           I_BT;
  wire           I_BC;
  wire           I_BTR;
  wire           I_RLD;
  wire           I_RRD;
  wire           I_INRC;
  wire           SetDI;
  wire           SetEI;
  wire [1:0]     IMode;
  wire           Halt;

  reg [15:0]     PC16;
  reg [15:0]     PC16_B;
  reg [15:0]     SP16, SP16_A, SP16_B;
  reg [15:0]     ID16_B;
  reg            Oldnmi_n;
  
  tv80_mcode #(Mode, Flag_C, Flag_N, Flag_P, Flag_X, Flag_H, Flag_Y, Flag_Z, Flag_S) i_mcode
    (
     .IR                   (IR),
     .ISet                 (ISet),
     .MCycle               (mcycle),
     .F                    (F),
     .NMICycle             (NMICycle),
     .IntCycle             (IntCycle),
     .MCycles              (mcycles_d),
     .TStates              (tstates),
     .Prefix               (Prefix),
     .Inc_PC               (Inc_PC),
     .Inc_WZ               (Inc_WZ),
     .IncDec_16            (IncDec_16),
     .Read_To_Acc          (Read_To_Acc),
     .Read_To_Reg          (Read_To_Reg),
     .Set_BusB_To          (Set_BusB_To),
     .Set_BusA_To          (Set_BusA_To),
     .ALU_Op               (ALU_Op),
     .Save_ALU             (Save_ALU),
     .PreserveC            (PreserveC),
     .Arith16              (Arith16),
     .Set_Addr_To          (Set_Addr_To),
     .IORQ                 (iorq_i),
     .Jump                 (Jump),
     .JumpE                (JumpE),
     .JumpXY               (JumpXY),
     .Call                 (Call),
     .RstP                 (RstP),
     .LDZ                  (LDZ),
     .LDW                  (LDW),
     .LDSPHL               (LDSPHL),
     .Special_LD           (Special_LD),
     .ExchangeDH           (ExchangeDH),
     .ExchangeRp           (ExchangeRp),
     .ExchangeAF           (ExchangeAF),
     .ExchangeRS           (ExchangeRS),
     .I_DJNZ               (I_DJNZ),
     .I_CPL                (I_CPL),
     .I_CCF                (I_CCF),
     .I_SCF                (I_SCF),
     .I_RETN               (I_RETN),
     .I_BT                 (I_BT),
     .I_BC                 (I_BC),
     .I_BTR                (I_BTR),
     .I_RLD                (I_RLD),
     .I_RRD                (I_RRD),
     .I_INRC               (I_INRC),
     .SetDI                (SetDI),
     .SetEI                (SetEI),
     .IMode                (IMode),
     .Halt                 (Halt),
     .NoRead               (no_read),
     .Write                (write)
     );

  tv80_alu #(Mode, Flag_C, Flag_N, Flag_P, Flag_X, Flag_H, Flag_Y, Flag_Z, Flag_S) i_alu
    (
     .Arith16              (Arith16_r),
     .Z16                  (Z16_r),
     .ALU_Op               (ALU_Op_r),
     .IR                   (IR[5:0]),
     .ISet                 (ISet),
     .BusA                 (BusA),
     .BusB                 (BusB),
     .F_In                 (F),
     .Q                    (ALU_Q),
     .F_Out                (F_Out)
     );

  always @(/*AUTOSENSE*/ALU_Q or BusAck or BusB or DI_Reg
	   or ExchangeRp or IR or Save_ALU_r or Set_Addr_To or XY_Ind
	   or XY_State or cen or mcycle or tstate or tstates)
    begin
      ClkEn = cen && ~ BusAck;

      if (tstate == tstates)
        T_Res = 1'b1;
      else T_Res = 1'b0;
      
      if (XY_State != 2'b00 && XY_Ind == 1'b0 &&
	  ((Set_Addr_To == aXY) ||
	   (mcycle == 3'b001 && IR == 8'b11001011) ||
	   (mcycle == 3'b001 && IR == 8'b00110110)))
        NextIs_XY_Fetch = 1'b1;
      else 
        NextIs_XY_Fetch = 1'b0;

      if (ExchangeRp)
        Save_Mux = BusB;
      else if (!Save_ALU_r)
        Save_Mux = DI_Reg;
      else
        Save_Mux = ALU_Q;
    end // always @ *
  
  always @ (posedge clk)
    begin
      if (reset_n == 1'b0 ) 
        begin
	  PC <= #1 0;  // Program Counter
	  A <= #1 0;
	  TmpAddr <= #1 0;
	  IR <= #1 8'b00000000;
	  ISet <= #1 2'b00;
	  XY_State <= #1 2'b00;
	  IStatus <= #1 2'b00;
	  mcycles <= #1 3'b000;
	  do <= #1 8'b00000000;

	  ACC <= #1 8'hFF;
	  F <= #1 8'hFF;
	  Ap <= #1 8'hFF;
	  Fp <= #1 8'hFF;
	  I <= #1 0;
	  R <= #1 0;
	  SP <= #1 16'hFFFF;
	  Alternate <= #1 1'b0;

	  Read_To_Reg_r <= #1 5'b00000;
	  Arith16_r <= #1 1'b0;
	  BTR_r <= #1 1'b0;
	  Z16_r <= #1 1'b0;
	  ALU_Op_r <= #1 4'b0000;
	  Save_ALU_r <= #1 1'b0;
	  PreserveC_r <= #1 1'b0;
	  XY_Ind <= #1 1'b0;
	end 
      else 
        begin

	  if (ClkEn == 1'b1 ) 
            begin

	      ALU_Op_r <= #1 4'b0000;
	      Save_ALU_r <= #1 1'b0;
	      Read_To_Reg_r <= #1 5'b00000;

	      mcycles <= #1 mcycles_d;

	      if (IMode != 2'b11 ) 
                begin
		  IStatus <= #1 IMode;
		end

	      Arith16_r <= #1 Arith16;
	      PreserveC_r <= #1 PreserveC;
	      if (ISet == 2'b10 && ALU_Op[2] == 1'b0 && ALU_Op[0] == 1'b1 && mcycle == 3'b011 ) 
                begin
		  Z16_r <= #1 1'b1;
		end 
              else 
                begin
		  Z16_r <= #1 1'b0;
		end

	      if (mcycle  == 3'b001 && tstate[2] == 1'b0 ) 
                begin
		  // mcycle == 1 && tstate == 1, 2, || 3

		  if (tstate == 2 && wait_n == 1'b1 ) 
                    begin
		      if (Mode < 2 ) 
                        begin
			  A[7:0] <= #1 R;
			  A[15:8] <= #1 I;
			  R[6:0] <= #1 R[6:0] + 1;
			end

		      if (Jump == 1'b0 && Call == 1'b0 && NMICycle == 1'b0 && IntCycle == 1'b0 && ~ (Halt_FF == 1'b1 || Halt == 1'b1) ) 
                        begin
			  PC <= #1 PC16;
			end

		      if (IntCycle == 1'b1 && IStatus == 2'b01 ) 
                        begin
			  IR <= #1 8'b11111111;
			end 
                      else if (Halt_FF == 1'b1 || (IntCycle == 1'b1 && IStatus == 2'b10) || NMICycle == 1'b1 ) 
                        begin
			  IR <= #1 8'b00000000;
		        end 
                      else 
                        begin
			  IR <= #1 dinst;
			end

		      ISet <= #1 2'b00;
		      if (Prefix != 2'b00 ) 
                        begin
			  if (Prefix == 2'b11 ) 
                            begin
			      if (IR[5] == 1'b1 ) 
                                begin
				  XY_State <= #1 2'b10;
				end 
                              else 
                                begin
				  XY_State <= #1 2'b01;
				end
			    end 
                          else 
                            begin
			      if (Prefix == 2'b10 ) 
                                begin
				  XY_State <= #1 2'b00;
				  XY_Ind <= #1 1'b0;
				end
			      ISet <= #1 Prefix;
			    end
			end 
                      else 
                        begin
			  XY_State <= #1 2'b00;
			  XY_Ind <= #1 1'b0;
			end
		    end // if (tstate == 2 && wait_n == 1'b1 )
                  

		end 
              else 
                begin
		  // either (mcycle > 1) OR (mcycle == 1 AND tstate > 3)

		  if (mcycle == 3'b110 ) 
                    begin
		      XY_Ind <= #1 1'b1;
		      if (Prefix == 2'b01 ) 
                        begin
			  ISet <= #1 2'b01;
			end
		    end
                  
		  if (T_Res == 1'b1 ) 
                    begin
		      BTR_r <= #1 (I_BT || I_BC || I_BTR) && ~ No_BTR;
		      if (Jump == 1'b1 ) 
                        begin
			  A[15:8] <= #1 DI_Reg;
			  A[7:0] <= #1 TmpAddr[7:0];
			  PC[15:8] <= #1 DI_Reg;
			  PC[7:0] <= #1 TmpAddr[7:0];
			end 
                      else if (JumpXY == 1'b1 ) 
                        begin
			  A <= #1 RegBusC;
			  PC <= #1 RegBusC;
			end else if (Call == 1'b1 || RstP == 1'b1 ) 
                          begin
			    A <= #1 TmpAddr;
			    PC <= #1 TmpAddr;
			  end 
                        else if (mcycle == mcycles && NMICycle == 1'b1 ) 
                          begin
			    A <= #1 16'b0000000001100110;
			    PC <= #1 16'b0000000001100110;
			  end 
                        else if (mcycle == 3'b011 && IntCycle == 1'b1 && IStatus == 2'b10 ) 
                          begin
			    A[15:8] <= #1 I;
			    A[7:0] <= #1 TmpAddr[7:0];
			    PC[15:8] <= #1 I;
			    PC[7:0] <= #1 TmpAddr[7:0];
			  end 
                        else 
                          begin
			    case (Set_Addr_To)
			      aXY :
                                begin
				  if (XY_State == 2'b00 ) 
                                    begin
				      A <= #1 RegBusC;
				    end 
                                  else 
                                    begin
				      if (NextIs_XY_Fetch == 1'b1 )
                                        begin
					  A <= #1 PC;
					end 
                                      else 
                                        begin
					  A <= #1 TmpAddr;
					end
				    end // else: !if(XY_State == 2'b00 )
                                end // case: aXY
                              
			      aIOA :
                                begin
				  if (Mode == 3 ) 
                                    begin
				      // Memory map I/O on GBZ80
				      A[15:8] <= #1 8'hFF;
				    end 
                                  else if (Mode == 2 ) 
                                    begin
				      // Duplicate I/O address on 8080
				      A[15:8] <= #1 DI_Reg;
				    end 
                                  else 
                                    begin
				      A[15:8] <= #1 ACC;
				    end
				  A[7:0] <= #1 DI_Reg;
                                end // case: aIOA

                              
			      aSP :
                                begin
				  A <= #1 SP;
                                end
                              
			      aBC :
                                begin
				  if (Mode == 3 && iorq_i == 1'b1 ) 
                                    begin
				      // Memory map I/O on GBZ80
				      A[15:8] <= #1 8'hFF;
				      A[7:0] <= #1 RegBusC[7:0];
				    end 
                                  else 
                                    begin
				      A <= #1 RegBusC;
				    end
                                end // case: aBC
                              
			      aDE :
                                begin
				  A <= #1 RegBusC;
                                end
                              
			      aZI :
                                begin                                  
				  if (Inc_WZ == 1'b1 ) 
                                    begin
				      A <= #1 TmpAddr + 1;
				    end 
                                  else 
                                    begin
				      A[15:8] <= #1 DI_Reg;
				      A[7:0] <= #1 TmpAddr[7:0];
				    end
                                end // case: aZI
                              
			      default	:
                                begin                                    
				  A <= #1 PC;
                                end
			    endcase // case(Set_Addr_To)
                            
			  end // else: !if(mcycle == 3'b011 && IntCycle == 1'b1 && IStatus == 2'b10 )
                      

		      Save_ALU_r <= #1 Save_ALU;
		      ALU_Op_r <= #1 ALU_Op;
                      
		      if (I_CPL == 1'b1 ) 
                        begin
			  // CPL
			  ACC <= #1 ~ ACC;
			  F[Flag_Y] <= #1 ~ ACC[5];
			  F[Flag_H] <= #1 1'b1;
			  F[Flag_X] <= #1 ~ ACC[3];
			  F[Flag_N] <= #1 1'b1;
			end
		      if (I_CCF == 1'b1 ) 
                        begin
			  // CCF
			  F[Flag_C] <= #1 ~ F[Flag_C];
			  F[Flag_Y] <= #1 ACC[5];
			  F[Flag_H] <= #1 F[Flag_C];
			  F[Flag_X] <= #1 ACC[3];
			  F[Flag_N] <= #1 1'b0;
			end
		      if (I_SCF == 1'b1 ) 
                        begin
			  // SCF
			  F[Flag_C] <= #1 1'b1;
			  F[Flag_Y] <= #1 ACC[5];
			  F[Flag_H] <= #1 1'b0;
			  F[Flag_X] <= #1 ACC[3];
			  F[Flag_N] <= #1 1'b0;
			end
		    end // if (T_Res == 1'b1 )
                  

		  if (tstate == 2 && wait_n == 1'b1 ) 
                    begin
		      if (ISet == 2'b01 && mcycle == 3'b111 ) 
                        begin
			  IR <= #1 dinst;
			end
		      if (JumpE == 1'b1 ) 
                        begin
                          PC <= #1 PC16;
			end 
                      else if (Inc_PC == 1'b1 ) 
                        begin
			  //PC <= #1 PC + 1;
                          PC <= #1 PC16;
			end
		      if (BTR_r == 1'b1 ) 
                        begin
			  //PC <= #1 PC - 2;
                          PC <= #1 PC16;
			end
		      if (RstP == 1'b1 ) 
                        begin
                          TmpAddr <= #1 { 10'h0, IR[5:3], 3'h0 };
			  //TmpAddr <= #1 (others =>1'b0);
			  //TmpAddr[5:3] <= #1 IR[5:3];
			end
		    end
		  if (tstate == 3 && mcycle == 3'b110 ) 
                    begin
                      TmpAddr <= #1 SP16;
		    end

		  if ((tstate == 2 && wait_n == 1'b1) || (tstate == 4 && mcycle == 3'b001) ) 
                    begin
		      if (IncDec_16[2:0] == 3'b111 ) 
                        begin
                          SP <= #1 SP16;
			end
		    end

		  if (LDSPHL == 1'b1 ) 
                    begin
		      SP <= #1 RegBusC;
		    end
		  if (ExchangeAF == 1'b1 ) 
                    begin
		      Ap <= #1 ACC;
		      ACC <= #1 Ap;
		      Fp <= #1 F;
		      F <= #1 Fp;
		    end
		  if (ExchangeRS == 1'b1 ) 
                    begin
		      Alternate <= #1 ~ Alternate;
		    end
		end // else: !if(mcycle  == 3'b001 && tstate(2) == 1'b0 )
              

	      if (tstate == 3 ) 
                begin
		  if (LDZ == 1'b1 ) 
                    begin
		      TmpAddr[7:0] <= #1 DI_Reg;
		    end
		  if (LDW == 1'b1 ) 
                    begin
		      TmpAddr[15:8] <= #1 DI_Reg;
		    end

		  if (Special_LD[2] == 1'b1 ) 
                    begin
		      case (Special_LD[1:0])
			2'b00 :
                          begin
			    ACC <= #1 I;
			    F[Flag_P] <= #1 IntE_FF2;
                          end
                        
			2'b01 :
                          begin
			    ACC <= #1 R;
			    F[Flag_P] <= #1 IntE_FF2;
                          end
                        
			2'b10 :
			  I <= #1 ACC;
                        
			default :
			  R <= #1 ACC;
		      endcase
		    end
		end // if (tstate == 3 )
              

	      if ((I_DJNZ == 1'b0 && Save_ALU_r == 1'b1) || ALU_Op_r == 4'b1001 ) 
                begin
		  if (Mode == 3 ) 
                    begin
		      F[6] <= #1 F_Out[6];
		      F[5] <= #1 F_Out[5];
		      F[7] <= #1 F_Out[7];
		      if (PreserveC_r == 1'b0 ) 
                        begin
			  F[4] <= #1 F_Out[4];
			end
		    end 
                  else 
                    begin
		      F[7:1] <= #1 F_Out[7:1];
		      if (PreserveC_r == 1'b0 ) 
                        begin
			  F[Flag_C] <= #1 F_Out[0];
			end
		    end
		end // if ((I_DJNZ == 1'b0 && Save_ALU_r == 1'b1) || ALU_Op_r == 4'b1001 )
              
	      if (T_Res == 1'b1 && I_INRC == 1'b1 ) 
                begin
		  F[Flag_H] <= #1 1'b0;
		  F[Flag_N] <= #1 1'b0;
		  if (DI_Reg[7:0] == 8'b00000000 ) 
                    begin
		      F[Flag_Z] <= #1 1'b1;
		    end 
                  else 
                    begin
		      F[Flag_Z] <= #1 1'b0;
		    end
		  F[Flag_S] <= #1 DI_Reg[7];
		  F[Flag_P] <= #1 ~ (^DI_Reg[7:0]);
		end // if (T_Res == 1'b1 && I_INRC == 1'b1 )
              

	      if (tstate == 1 && Auto_Wait_t1 == 1'b0 ) 
                begin
		  do <= #1 BusB;
		  if (I_RLD == 1'b1 ) 
                    begin
		      do[3:0] <= #1 BusA[3:0];
		      do[7:4] <= #1 BusB[3:0];
		    end
		  if (I_RRD == 1'b1 ) 
                    begin
		      do[3:0] <= #1 BusB[7:4];
		      do[7:4] <= #1 BusA[3:0];
		    end
		end

	      if (T_Res == 1'b1 ) 
                begin
		  Read_To_Reg_r[3:0] <= #1 Set_BusA_To;
		  Read_To_Reg_r[4] <= #1 Read_To_Reg;
		  if (Read_To_Acc == 1'b1 ) 
                    begin
		      Read_To_Reg_r[3:0] <= #1 4'b0111;
		      Read_To_Reg_r[4] <= #1 1'b1;
		    end
		end

	      if (tstate == 1 && I_BT == 1'b1 ) 
                begin
		  F[Flag_X] <= #1 ALU_Q[3];
		  F[Flag_Y] <= #1 ALU_Q[1];
		  F[Flag_H] <= #1 1'b0;
		  F[Flag_N] <= #1 1'b0;
		end
	      if (I_BC == 1'b1 || I_BT == 1'b1 ) 
                begin
		  F[Flag_P] <= #1 IncDecZ;
		end

	      if ((tstate == 1 && Save_ALU_r == 1'b0 && Auto_Wait_t1 == 1'b0) ||
		  (Save_ALU_r == 1'b1 && ALU_Op_r != 4'b0111) ) 
                begin
		  case (Read_To_Reg_r)
		    5'b10111 :
		      ACC <= #1 Save_Mux;
		    5'b10110 :
		      do <= #1 Save_Mux;
		    5'b11000 :
		      SP[7:0] <= #1 Save_Mux;
		    5'b11001 :
		      SP[15:8] <= #1 Save_Mux;
		    5'b11011 :
		      F <= #1 Save_Mux;
		  endcase
		end // if ((tstate == 1 && Save_ALU_r == 1'b0 && Auto_Wait_t1 == 1'b0) ||...              
	    end // if (ClkEn == 1'b1 )         
	end // else: !if(reset_n == 1'b0 )
    end
  

  //-------------------------------------------------------------------------
  //
  // BC('), DE('), HL('), IX && IY
  //
  //-------------------------------------------------------------------------
  always @ (posedge clk)
    begin
      if (ClkEn == 1'b1 ) 
        begin
	  // Bus A / Write
	  RegAddrA_r <= #1  { Alternate, Set_BusA_To[2:1] };
	  if (XY_Ind == 1'b0 && XY_State != 2'b00 && Set_BusA_To[2:1] == 2'b10 ) 
            begin
	      RegAddrA_r <= #1 { XY_State[1],  2'b11 };
	    end

	  // Bus B
	  RegAddrB_r <= #1 { Alternate, Set_BusB_To[2:1] };
	  if (XY_Ind == 1'b0 && XY_State != 2'b00 && Set_BusB_To[2:1] == 2'b10 ) 
            begin
	      RegAddrB_r <= #1 { XY_State[1],  2'b11 };
	    end

	  // Address from register
	  RegAddrC <= #1 { Alternate,  Set_Addr_To[1:0] };
	  // Jump (HL), LD SP,HL
	  if ((JumpXY == 1'b1 || LDSPHL == 1'b1) ) 
            begin
	      RegAddrC <= #1 { Alternate, 2'b10 };
	    end
	  if (((JumpXY == 1'b1 || LDSPHL == 1'b1) && XY_State != 2'b00) || (mcycle == 3'b110) ) 
            begin
	      RegAddrC <= #1 { XY_State[1],  2'b11 };
	    end

	  if (I_DJNZ == 1'b1 && Save_ALU_r == 1'b1 && Mode < 2 ) 
            begin
	      IncDecZ <= #1 F_Out[Flag_Z];
	    end
	  if ((tstate == 2 || (tstate == 3 && mcycle == 3'b001)) && IncDec_16[2:0] == 3'b100 ) 
            begin
	      if (ID16 == 0 ) 
                begin
		  IncDecZ <= #1 1'b0;
		end 
              else 
                begin
		  IncDecZ <= #1 1'b1;
		end
	    end
          
	  RegBusA_r <= #1 RegBusA;
	end
      
    end // always @ (posedge clk)
  

  always @(/*AUTOSENSE*/Alternate or ExchangeDH or IncDec_16
	   or RegAddrA_r or RegAddrB_r or XY_State or mcycle or tstate)
    begin
      if ((tstate == 2 || (tstate == 3 && mcycle == 3'b001 && IncDec_16[2] == 1'b1)) && XY_State == 2'b00)
        RegAddrA = { Alternate, IncDec_16[1:0] };
      else if ((tstate == 2 || (tstate == 3 && mcycle == 3'b001 && IncDec_16[2] == 1'b1)) && IncDec_16[1:0] == 2'b10)
        RegAddrA = { XY_State[1], 2'b11 };
      else if (ExchangeDH == 1'b1 && tstate == 3)
        RegAddrA = { Alternate, 2'b10 };
      else if (ExchangeDH == 1'b1 && tstate == 4)
	RegAddrA = { Alternate, 2'b01 };
      else
        RegAddrA = RegAddrA_r;
      
      if (ExchangeDH == 1'b1 && tstate == 3)
        RegAddrB = { Alternate, 2'b01 };
      else
        RegAddrB = RegAddrB_r;
    end // always @ *
  

  always @(/*AUTOSENSE*/ALU_Op_r or Auto_Wait_t1 or ExchangeDH
	   or IncDec_16 or Read_To_Reg_r or Save_ALU_r or mcycle
	   or tstate or wait_n)
    begin
      RegWEH = 1'b0;
      RegWEL = 1'b0;
      if ((tstate == 1 && Save_ALU_r == 1'b0 && Auto_Wait_t1 == 1'b0) ||
	  (Save_ALU_r == 1'b1 && ALU_Op_r != 4'b0111) ) 
        begin
	  case (Read_To_Reg_r)
	    5'b10000 , 5'b10001 , 5'b10010 , 5'b10011 , 5'b10100 , 5'b10101 :
              begin
	        RegWEH = ~ Read_To_Reg_r[0];
		RegWEL = Read_To_Reg_r[0];
              end
          endcase // case(Read_To_Reg_r)
          
	end // if ((tstate == 1 && Save_ALU_r == 1'b0 && Auto_Wait_t1 == 1'b0) ||...
      

      if (ExchangeDH == 1'b1 && (tstate == 3 || tstate == 4) ) 
        begin
	  RegWEH = 1'b1;
	  RegWEL = 1'b1;
	end

      if (IncDec_16[2] == 1'b1 && ((tstate == 2 && wait_n == 1'b1 && mcycle != 3'b001) || (tstate == 3 && mcycle == 3'b001)) ) 
        begin
	  case (IncDec_16[1:0])
	    2'b00 , 2'b01 , 2'b10 :
              begin
		RegWEH = 1'b1;
		RegWEL = 1'b1;
              end
	  endcase
	end
    end // always @ *
  

  always @(/*AUTOSENSE*/ExchangeDH or ID16 or IncDec_16 or RegBusA_r
	   or RegBusB or Save_Mux or mcycle or tstate)
    begin
      RegDIH = Save_Mux;
      RegDIL = Save_Mux;

      if (ExchangeDH == 1'b1 && tstate == 3 ) 
        begin
	  RegDIH = RegBusB[15:8];
	  RegDIL = RegBusB[7:0];
	end
      else if (ExchangeDH == 1'b1 && tstate == 4 ) 
        begin
	  RegDIH = RegBusA_r[15:8];
	  RegDIL = RegBusA_r[7:0];
	end
      else if (IncDec_16[2] == 1'b1 && ((tstate == 2 && mcycle != 3'b001) || (tstate == 3 && mcycle == 3'b001)) ) 
        begin
	  RegDIH = ID16[15:8];
	  RegDIL = ID16[7:0];
	end
    end

  tv80_reg i_reg
    (
     .clk                  (clk),
     .CEN                  (ClkEn),
     .WEH                  (RegWEH),
     .WEL                  (RegWEL),
     .AddrA                (RegAddrA),
     .AddrB                (RegAddrB),
     .AddrC                (RegAddrC),
     .DIH                  (RegDIH),
     .DIL                  (RegDIL),
     .DOAH                 (RegBusA[15:8]),
     .DOAL                 (RegBusA[7:0]),
     .DOBH                 (RegBusB[15:8]),
     .DOBL                 (RegBusB[7:0]),
     .DOCH                 (RegBusC[15:8]),
     .DOCL                 (RegBusC[7:0])
     );

  //-------------------------------------------------------------------------
  //
  // Buses
  //
  //-------------------------------------------------------------------------

  always @ (posedge clk)
    begin
      if (ClkEn == 1'b1 ) 
        begin
	  case (Set_BusB_To)
	    4'b0111 :
	      BusB <= #1 ACC;
	    4'b0000 , 4'b0001 , 4'b0010 , 4'b0011 , 4'b0100 , 4'b0101 :
              begin
		if (Set_BusB_To[0] == 1'b1 ) 
                  begin
		    BusB <= #1 RegBusB[7:0];
		  end 
                else 
                  begin
		    BusB <= #1 RegBusB[15:8];
		  end
              end
	    4'b0110 :
	      BusB <= #1 DI_Reg;
	    4'b1000 :
	      BusB <= #1 SP[7:0];
	    4'b1001 :
	      BusB <= #1 SP[15:8];
	    4'b1010 :
	      BusB <= #1 8'b00000001;
	    4'b1011 :
	      BusB <= #1 F;
	    4'b1100 :
	      BusB <= #1 PC[7:0];
	    4'b1101 :
	      BusB <= #1 PC[15:8];
	    4'b1110 :
	      BusB <= #1 8'b00000000;
	    default :
	      BusB <= #1 8'hxx;
	  endcase

	  case (Set_BusA_To)
	    4'b0111 :
	      BusA <= #1 ACC;
	    4'b0000 , 4'b0001 , 4'b0010 , 4'b0011 , 4'b0100 , 4'b0101 :
              begin
		if (Set_BusA_To[0] == 1'b1 )
                  begin
		    BusA <= #1 RegBusA[7:0];
		  end 
                else 
                  begin
		    BusA <= #1 RegBusA[15:8];
		  end
              end
	    4'b0110 :
	      BusA <= #1 DI_Reg;
	    4'b1000 :
	      BusA <= #1 SP[7:0];
	    4'b1001 :
	      BusA <= #1 SP[15:8];
	    4'b1010 :
	      BusA <= #1 8'b00000000;
	    default :
	      BusB <= #1  8'hxx;
	  endcase
	end
    end

  //-------------------------------------------------------------------------
  //
  // Generate external control signals
  //
  //-------------------------------------------------------------------------
  always @ (posedge clk)
    begin
      if (reset_n == 1'b0 ) 
        begin
	  rfsh_n <= #1 1'b1;
	end 
      else
        begin
	  if (cen == 1'b1 ) 
            begin
	      if (mcycle == 3'b001 && ((tstate == 2  && wait_n == 1'b1) || tstate == 3) ) 
                begin
		  rfsh_n <= #1 1'b0;
		end 
              else 
                begin
		  rfsh_n <= #1 1'b1;
		end
	    end
	end
    end
  

  always @(/*AUTOSENSE*/BusAck or Halt_FF or I_DJNZ or IntCycle
	   or IntE_FF1 or di or iorq_i or mcycle or tstate)
    begin
      mc = mcycle;
      ts = tstate;
      DI_Reg = di;
      halt_n = ~ Halt_FF;
      busak_n = ~ BusAck;
      intcycle_n = ~ IntCycle;
      IntE = IntE_FF1;
      iorq = iorq_i;
      stop = I_DJNZ;
    end

  //-----------------------------------------------------------------------
  //
  // Syncronise inputs
  //
  //-----------------------------------------------------------------------

  always @ (posedge clk)
    begin : sync_inputs

      if (reset_n == 1'b0 ) 
        begin
	  BusReq_s <= #1 1'b0;
	  INT_s <= #1 1'b0;
	  NMI_s <= #1 1'b0;
	  Oldnmi_n <= #1 1'b0;
	end 
      else
        begin
	  if (cen == 1'b1 ) 
            begin
	      BusReq_s <= #1 ~ busrq_n;
	      INT_s <= #1 ~ int_n;
	      if (NMICycle == 1'b1 ) 
                begin
		  NMI_s <= #1 1'b0;
		end 
              else if (nmi_n == 1'b0 && Oldnmi_n == 1'b1 ) 
                begin
		  NMI_s <= #1 1'b1;
		end
	      Oldnmi_n <= #1 nmi_n;
	    end
	end
    end

  //-----------------------------------------------------------------------
  //
  // Main state machine
  //
  //-----------------------------------------------------------------------

  always @ (posedge clk)
    begin
      if (reset_n == 1'b0 ) 
        begin
	  mcycle <= #1 3'b001;
	  tstate <= #1 3'b000;
	  Pre_XY_F_M <= #1 3'b000;
	  Halt_FF <= #1 1'b0;
	  BusAck <= #1 1'b0;
	  NMICycle <= #1 1'b0;
	  IntCycle <= #1 1'b0;
	  IntE_FF1 <= #1 1'b0;
	  IntE_FF2 <= #1 1'b0;
	  No_BTR <= #1 1'b0;
	  Auto_Wait_t1 <= #1 1'b0;
	  Auto_Wait_t2 <= #1 1'b0;
	  m1_n <= #1 1'b1;
	end 
      else
        begin
	  if (cen == 1'b1 ) 
            begin
	      if (T_Res == 1'b1 ) 
                begin
		  Auto_Wait_t1 <= #1 1'b0;
		end 
              else 
                begin
		  Auto_Wait_t1 <= #1 Auto_Wait || iorq_i;
		end
	      Auto_Wait_t2 <= #1 Auto_Wait_t1;
	      No_BTR <= #1 (I_BT && (~ IR[4] || ~ F[Flag_P])) ||
			(I_BC && (~ IR[4] || F[Flag_Z] || ~ F[Flag_P])) ||
			(I_BTR && (~ IR[4] || F[Flag_Z]));
	      if (tstate == 2 ) 
                begin
		  if (SetEI == 1'b1 ) 
                    begin
		      IntE_FF1 <= #1 1'b1;
		      IntE_FF2 <= #1 1'b1;
		    end
		  if (I_RETN == 1'b1 ) 
                    begin
		      IntE_FF1 <= #1 IntE_FF2;
		    end
		end
	      if (tstate == 3 ) 
                begin
		  if (SetDI == 1'b1 ) 
                    begin
		      IntE_FF1 <= #1 1'b0;
		      IntE_FF2 <= #1 1'b0;
		    end
		end
	      if (IntCycle == 1'b1 || NMICycle == 1'b1 ) 
                begin
		  Halt_FF <= #1 1'b0;
		end
	      if (mcycle == 3'b001 && tstate == 2 && wait_n == 1'b1 ) 
                begin
		  m1_n <= #1 1'b1;
		end
	      if (BusReq_s == 1'b1 && BusAck == 1'b1 ) 
                begin
                end 
              else 
                begin
		  BusAck <= #1 1'b0;
		  if (tstate == 2 && wait_n == 1'b0 ) 
                    begin
		    end 
                  else if (T_Res == 1'b1 ) 
                    begin
		      if (Halt == 1'b1 ) 
                        begin
			  Halt_FF <= #1 1'b1;
			end
		      if (BusReq_s == 1'b1 ) 
                        begin
			  BusAck <= #1 1'b1;
			end 
                      else 
                        begin
			  tstate <= #1 3'b001;
			  if (NextIs_XY_Fetch == 1'b1 ) 
                            begin
			      mcycle <= #1 3'b110;
			      Pre_XY_F_M <= #1 mcycle;
			      if (IR == 8'b00110110 && Mode == 0 ) 
                                begin
				  Pre_XY_F_M <= #1 3'b010;
				end
			    end 
                          else if ((mcycle == 3'b111) || (mcycle == 3'b110 && Mode == 1 && ISet != 2'b01) ) 
                            begin
			      mcycle <= #1 Pre_XY_F_M + 1;
			    end 
                          else if ((mcycle == mcycles) ||
				   No_BTR == 1'b1 ||
				   (mcycle == 3'b010 && I_DJNZ == 1'b1 && IncDecZ == 1'b1) ) 
                            begin
			      m1_n <= #1 1'b0;
			      mcycle <= #1 3'b001;
			      IntCycle <= #1 1'b0;
			      NMICycle <= #1 1'b0;
			      if (NMI_s == 1'b1 && Prefix == 2'b00 ) 
                                begin
				  NMICycle <= #1 1'b1;
				  IntE_FF1 <= #1 1'b0;
				end 
                              else if ((IntE_FF1 == 1'b1 && INT_s == 1'b1) && Prefix == 2'b00 && SetEI == 1'b0 ) 
                                begin
				  IntCycle <= #1 1'b1;
				  IntE_FF1 <= #1 1'b0;
				  IntE_FF2 <= #1 1'b0;
				end
			    end 
                          else 
                            begin
			      mcycle <= #1 mcycle + 1;
			    end
			end
		    end 
                  else 
                    begin   // verilog has no "nor" operator
		      if ( ~(Auto_Wait == 1'b1 && Auto_Wait_t2 == 1'b0) &&
			   ~(IOWait == 1 && iorq_i == 1'b1 && Auto_Wait_t1 == 1'b0) ) 
                        begin
			  tstate <= #1 tstate + 1;
			end
		    end
		end
	      if (tstate == 0 ) 
                begin
		  m1_n <= #1 1'b0;
		end
	    end
	end
    end

  always @(/*AUTOSENSE*/BTR_r or DI_Reg or IncDec_16 or JumpE or PC
	   or RegBusA or RegBusC or SP or tstate)
    begin
      if (JumpE == 1'b1 ) 
        begin
          PC16_B = { {8{DI_Reg[7]}}, DI_Reg };
	end 
      else if (BTR_r == 1'b1 ) 
        begin
          PC16_B = -2;
	end
      else
        begin
          PC16_B = 1;
	end

      if (tstate == 3)
        begin
          SP16_A = RegBusC;
          SP16_B = { {8{DI_Reg[7]}}, DI_Reg };
        end
      else
        begin
          // suspect that ID16 and SP16 could be shared
          SP16_A = SP;
          
          if (IncDec_16[3] == 1'b1)
            SP16_B = -1;
          else
            SP16_B = 1;
        end

      if (IncDec_16[3])  
        ID16_B = -1;
      else
        ID16_B = 1;

      ID16 = RegBusA + ID16_B;
      PC16 = PC + PC16_B;
      SP16 = SP16_A + SP16_B;
    end // always @ *
  

  always @(/*AUTOSENSE*/IntCycle or NMICycle or mcycle)
    begin
      Auto_Wait = 1'b0;
      if (IntCycle == 1'b1 || NMICycle == 1'b1 ) 
        begin
	  if (mcycle == 3'b001 ) 
            begin
	      Auto_Wait = 1'b1;
	    end
	end
    end // always @ *
  
// synopsys dc_script_begin
// set_attribute current_design "revision" "$Id: tv80_core.v,v 1.1 2004/05/16 17:39:57 ghutchis Exp $" -type string -quiet
// synopsys dc_script_end
endmodule // T80

