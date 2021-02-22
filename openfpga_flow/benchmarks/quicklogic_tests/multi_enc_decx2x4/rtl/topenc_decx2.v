
module top(clock,reset,datain,dataout,datain1,dataout1);


input clock,reset;

input [127:0] datain;
output [127:0] dataout;
                          
input [127:0] datain1;   
output [127:0] dataout1; 
                          
wire [6:0] enc_out;
reg  [127:0] data_encin;
reg  [6:0] data_encout;

wire [6:0] enc_out1;
reg  [127:0] data_encin1;
reg  [6:0] data_encout1;





encoder128 U01(.datain(data_encin),.dataout(enc_out));
decoder128 U02(.datain(data_encout),.dataout(dataout));

encoder128 U011(.datain(data_encin1),.dataout(enc_out1));
decoder128 U021(.datain(data_encout1),.dataout(dataout1));



	always @(posedge clock)
		
		begin
		
			if (reset)
			
				data_encin <= 127'h00000;
				
			else
			
				data_encin <= datain;
				
				
		end
		
		
	always @(posedge clock)
		
		begin
		
			if (reset)
			
				data_encout <= 7'h0;
				
			else
			
				data_encout<= enc_out;
				
				
		end
			
			
			
		always @(posedge clock)
		
		begin
		
			if (reset)
			
				data_encin1 <= 127'h00000;
				
			else
			
				data_encin1 <= datain1;
				
				
		end
		
		
	always @(posedge clock)
		
		begin
		
			if (reset)
			
				data_encout1 <= 7'h0;
				
			else
			
				data_encout1<= enc_out1;
				
				
		end		
			
			
			
			
			
			
			
endmodule

