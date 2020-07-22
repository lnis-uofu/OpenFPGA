module Controller(
	output reg [11:0] control_signals,
	input [3:0] opcode,
	input clk,
	input clr_
	); 
	
	reg [3:0] ps, ns;
	
	always @(posedge clk)
	begin
		if(~clr_) ps <= 4'd0;
		else ps <= ns;
	end
	
	always @(*)
	begin
		case(ps)
			0:
			begin
				control_signals = 12'h3e3;
				ns = 4'd1;		
			end
			
			1: //T1
			begin
				control_signals = 12'h5e3;
				ns = 4'd2;	
			end
			
			2: //T2
			begin
//				control_signals = 12'hbe3;
				control_signals = 12'h863;
				ns = 4'd3;	
			end
			
			3: //T3
			begin
//				control_signals = 12'h263;
				control_signals = 12'h3e3;
				if(opcode == 4'd0) //LDA
					ns = 4'd4;
				else if(opcode == 4'd1) //ADD
					ns = 4'd6;
				else if(opcode == 4'd2) //SUB
					ns = 4'd9;
				else if(opcode == 4'd14) //OUT
					ns = 4'd12;
				else if(opcode == 4'd15) //HLT
					ns = 4'd13;
			end
			
			4: //LDA
			begin
				control_signals = 12'h1a3;
				ns = 4'd5;	
			end
			
			5: //LDA
			begin
				control_signals = 12'h2c3;
				ns = 4'd1;	
			end
			
			6: //ADD
			begin
				control_signals = 12'h1a3;
				ns = 4'd7;	
			end
			
			7: //ADD
			begin
				control_signals = 12'h2e1;
				ns = 4'd8;	
			end
			
			8: //ADD
			begin
				control_signals = 12'h3c7;
				ns = 4'd1;	
			end
			
			9: //SUB
			begin
				control_signals = 12'h1a3;
				ns = 4'd10;	
			end
			
			10: //SUB
			begin
				control_signals = 12'h2e1;
				ns = 4'd11;	
			end
			
			11: //SUB
			begin
				control_signals = 12'h3cf;
				ns = 4'd1;	
			end
			
			12: //OUT
			begin
				control_signals = 12'h3f2;
				ns = 4'd1;	
			end
			
			13: //HLT
				ns = 4'd13;	
			
			default: 
			begin
				ns = 4'd0;
				control_signals = 12'h3e3;
			end
		
		endcase
	end	
endmodule