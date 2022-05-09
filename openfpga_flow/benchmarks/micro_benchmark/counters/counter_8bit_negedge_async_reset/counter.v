///////////////////////////////////////////
//  Functionality: Counter triggered at negative edge with asynchronous reset
//  Author:        Xifan Tang
////////////////////////////////////////

module counter (
	clkn,
	reset,
	result
);

	input clkn;
	input reset;
	output [7:0] result;

	reg [7:0] result;

    initial begin
      result <= 0;
    end

	always @(negedge clkn or posedge reset)
	begin
		if (reset) 
			result = 0;		
		else 
			result = result + 1;
	end
endmodule		
