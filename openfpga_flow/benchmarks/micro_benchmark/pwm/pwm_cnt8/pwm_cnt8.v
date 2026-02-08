///////////////////////////////////////////
//  Functionality: PWM based on a hard counter
//  Author:        Xifan Tang
////////////////////////////////////////

module pwm_cnt8 (
	clk,
	reset,
	m0_out,
    m1_out
);

	input clk;
	input reset;
	output m0_out;
	output m1_out;

    cnt_8 #(
      .MATCH0_REF(8'b11000000),
      .MATCH1_REF(8'b11110000)
    ) CNT0 (
      .clk(clk),
      .rst(reset),
      .match0(m0_out),
      .match1(m1_out)
    ); 

endmodule		
