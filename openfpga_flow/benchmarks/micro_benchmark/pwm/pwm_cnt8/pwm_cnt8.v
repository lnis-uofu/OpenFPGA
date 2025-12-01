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
      .MATCH0_REF(00001111),
      .MATCH1_REF(11111111)
    ) CNT0 (
      .clk(clk),
      .rst(reset),
      .match0(m0_out),
      .match1(m1_out)
    ); 

endmodule		
