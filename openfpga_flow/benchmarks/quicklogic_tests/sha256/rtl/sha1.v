/////////////////////////////////////////////////////////////////////
////                                                             ////
////  SHA-160                                                    ////
////  Secure Hash Algorithm (SHA-160)                            ////
////                                                             ////
////  Author: marsgod                                            ////
////          marsgod@opencores.org                              ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/sha_core/  ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2002-2004 marsgod                             ////
////                         marsgod@opencores.org               ////
////                                                             ////
////                                                             ////
//// This source file may be used and distributed without        ////
//// restriction provided that this copyright statement is not   ////
//// removed from the file and that any derivative work contains ////
//// the original copyright notice and the associated disclaimer.////
////                                                             ////
////     THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY     ////
//// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   ////
//// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS   ////
//// FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHOR      ////
//// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,         ////
//// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    ////
//// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE   ////
//// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR        ////
//// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF  ////
//// LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT  ////
//// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  ////
//// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE         ////
//// POSSIBILITY OF SUCH DAMAGE.                                 ////
////                                                             ////
/////////////////////////////////////////////////////////////////////

`define SHA1_H0 32'h67452301
`define SHA1_H1 32'hefcdab89
`define SHA1_H2 32'h98badcfe
`define SHA1_H3 32'h10325476
`define SHA1_H4 32'hc3d2e1f0

`define SHA1_K0 32'h5a827999
`define SHA1_K1 32'h6ed9eba1
`define SHA1_K2 32'h8f1bbcdc
`define SHA1_K3 32'hca62c1d6

module sha1 (clk_i, rst_i, text_i, text_o, cmd_i, cmd_w_i, cmd_o);

	input		clk_i; 	// global clock input
	input		rst_i; 	// global reset input , active high
	
	input	[31:0]	text_i;	// text input 32bit
	output	[31:0]	text_o;	// text output 32bit
	
	input	[2:0]	cmd_i;	// command input
	input		cmd_w_i;// command input write enable
	output	[3:0]	cmd_o;	// command output(status)

	/*
		cmd
		Busy Round W R

		bit3 bit2  bit1 bit0
		Busy Round W    R
		
		Busy:
		0	idle
		1	busy
		
		Round:
		0	first round
		1	internal round
		
		W:
		0       No-op
		1	write data
		
		R:
		0	No-op
		1	read data
			
	*/
	

    	reg	[3:0]	cmd;
    	wire	[3:0]	cmd_o;
    	
    	reg	[31:0]	text_o;
    	
    	reg	[6:0]	round;
    	wire	[6:0]	round_plus_1;
    	
    	reg	[2:0]	read_counter;
    	
    	reg	[31:0]	H0,H1,H2,H3,H4;
    	reg	[31:0]	W0,W1,W2,W3,W4,W5,W6,W7,W8,W9,W10,W11,W12,W13,W14;
    	reg	[31:0]	Wt,Kt;
    	reg	[31:0]	A,B,C,D,E;

    	reg		busy;
    	
    	assign cmd_o = cmd;
    	always @ (posedge clk_i)
    	begin
    		if (rst_i)
    			cmd <= 'b0;
    		else
    		if (cmd_w_i)
    			cmd[2:0] <= cmd_i[2:0];		// busy bit can't write
    		else
    		begin
    			cmd[3] <= busy;			// update busy bit
    			if (~busy)
    				cmd[1:0] <= 2'b00;	// hardware auto clean R/W bits
    		end
    	end
    	
    	// Hash functions
	wire [31:0] SHA1_f1_BCD,SHA1_f2_BCD,SHA1_f3_BCD,SHA1_Wt_1;
	wire [31:0] SHA1_ft_BCD;
	wire [31:0] next_Wt,next_A,next_C;
	wire [159:0] SHA1_result;
	
	assign SHA1_f1_BCD = (B & C) ^ (~B & D);
	assign SHA1_f2_BCD = B ^ C ^ D;
	assign SHA1_f3_BCD = (B & C) ^ (C & D) ^ (B & D);
	
	assign SHA1_ft_BCD = (round < 'd21) ? SHA1_f1_BCD : (round < 'd41) ? SHA1_f2_BCD : (round < 'd61) ? SHA1_f3_BCD : SHA1_f2_BCD;
	
    	assign SHA1_Wt_1 = {W13 ^ W8 ^ W2 ^ W0};

	assign next_Wt = {SHA1_Wt_1[30:0],SHA1_Wt_1[31]};	// NSA fix added
	assign next_A = {A[26:0],A[31:27]} + SHA1_ft_BCD + E + Kt + Wt;
	assign next_C = {B[1:0],B[31:2]};
	
	assign SHA1_result   = {A,B,C,D,E};
	
	assign round_plus_1 = round + 1;
	
	//------------------------------------------------------------------	
	// SHA round
	//------------------------------------------------------------------
	always @(posedge clk_i)
	begin
		if (rst_i)
		begin
			round <= 'd0;
			busy <= 'b0;

			W0  <= 'b0;
			W1  <= 'b0;
			W2  <= 'b0;
			W3  <= 'b0;
			W4  <= 'b0;
			W5  <= 'b0;
			W6  <= 'b0;
			W7  <= 'b0;
			W8  <= 'b0;
			W9  <= 'b0;
			W10 <= 'b0;
			W11 <= 'b0;
			W12 <= 'b0;
			W13 <= 'b0;
			W14 <= 'b0;
			Wt  <= 'b0;
			
			A <= 'b0;
			B <= 'b0;
			C <= 'b0;
			D <= 'b0;
			E <= 'b0;
			
			H0 <= 'b0;
			H1 <= 'b0;
			H2 <= 'b0;
			H3 <= 'b0;
			H4 <= 'b0;

		end
		else
		begin
			case (round)
			
			'd0:
				begin
					if (cmd[1])
					begin
						W0 <= text_i;
						Wt <= text_i;
						busy <= 'b1;
						round <= round_plus_1;
                                               	
						case (cmd[2])
							1'b0:	// sha-1 first message
								begin
									A <= `SHA1_H0;
									B <= `SHA1_H1;
									C <= `SHA1_H2;
									D <= `SHA1_H3;
									E <= `SHA1_H4;
									
									H0 <= `SHA1_H0;
									H1 <= `SHA1_H1;
									H2 <= `SHA1_H2;
									H3 <= `SHA1_H3;
									H4 <= `SHA1_H4;
								end
							1'b1:	// sha-1 internal message
								begin
									H0 <= A;
									H1 <= B;
									H2 <= C;
									H3 <= D;
									H4 <= E;
								end
						endcase
					end
					else
					begin	// IDLE
						round <= 'd0;
					end
				end
			'd1:
				begin
					W1 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd2:
				begin
					W2 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd3:
				begin
					W3 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd4:
				begin
					W4 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd5:
				begin
					W5 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd6:
				begin
					W6 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd7:
				begin
					W7 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd8:
				begin
					W8 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd9:
				begin
					W9 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd10:
				begin
					W10 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd11:
				begin
					W11 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd12:
				begin
					W12 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd13:
				begin
					W13 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd14:
				begin
					W14 <= text_i;
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd15:
				begin
					Wt <= text_i;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd16,
			'd17,
			'd18,
			'd19,
			'd20,
			'd21,
			'd22,
			'd23,
			'd24,
			'd25,
			'd26,
			'd27,
			'd28,
			'd29,
			'd30,
			'd31,
			'd32,
			'd33,
			'd34,
			'd35,
			'd36,
			'd37,
			'd38,
			'd39,
			'd40,
			'd41,
			'd42,
			'd43,
			'd44,
			'd45,
			'd46,
			'd47,
			'd48,
			'd49,
			'd50,
			'd51,
			'd52,
			'd53,
			'd54,
			'd55,
			'd56,
			'd57,
			'd58,
			'd59,
			'd60,
			'd61,
			'd62,
			'd63,
			'd64,
			'd65,
			'd66,
			'd67,
			'd68,
			'd69,
			'd70,
			'd71,
			'd72,
			'd73,
			'd74,
			'd75,
			'd76,
			'd77,
			'd78,
			'd79:
				begin
					W0  <= W1;
					W1  <= W2;
					W2  <= W3;
					W3  <= W4;
					W4  <= W5;
					W5  <= W6;
					W6  <= W7;
					W7  <= W8;
					W8  <= W9;
					W9  <= W10;
					W10 <= W11;
					W11 <= W12;
					W12 <= W13;
					W13 <= W14;
					W14 <= Wt;
					Wt  <= next_Wt;
					
					E <= D;
					D <= C;
					C <= next_C;
					B <= A;
					A <= next_A;
						
					round <= round_plus_1;
				end
			'd80:
				begin
					A <= next_A + H0;
					B <= A + H1;
					C <= next_C + H2;
					D <= C + H3;
					E <= D + H4;
					round <= 'd0;
					busy <= 'b0;
				end
			default:
				begin
					round <= 'd0;
					busy <= 'b0;
				end
			endcase
		end	
	end 
	
	
	//------------------------------------------------------------------	
	// Kt generator
	//------------------------------------------------------------------	
	always @ (posedge clk_i)
	begin
		if (rst_i)
		begin
			Kt <= 'b0;
		end
		else
		begin
			if (round < 'd20)
				Kt <= `SHA1_K0;
			else
			if (round < 'd40)
				Kt <= `SHA1_K1;
			else
			if (round < 'd60)
				Kt <= `SHA1_K2;
			else
				Kt <= `SHA1_K3;
		end
	end

	//------------------------------------------------------------------	
	// read result 
	//------------------------------------------------------------------	
	always @ (posedge clk_i)
	begin
		if (rst_i)
		begin
			text_o <= 'b0;
			read_counter <= 'b0;
		end
		else
		begin
			if (cmd[0])
			begin
				read_counter <= 'd4;	// sha-1 	160/32=5
			end
			else
			begin
			if (~busy)
			begin
				case (read_counter)
					'd4:	text_o <= SHA1_result[5*32-1:4*32];
					'd3:	text_o <= SHA1_result[4*32-1:3*32];
					'd2:	text_o <= SHA1_result[3*32-1:2*32];
					'd1:	text_o <= SHA1_result[2*32-1:1*32];
					'd0:	text_o <= SHA1_result[1*32-1:0*32];
					default:text_o <= 'b0;
				endcase
				if (|read_counter)
					read_counter <= read_counter - 'd1;
			end
			else
			begin
				text_o <= 'b0;
			end
			end
		end
	end
	
endmodule
 