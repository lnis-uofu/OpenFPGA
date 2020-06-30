package andgate_pkg;
	bit IN1,IN2;
	function bit andgate(bit IN1,IN2);
		andgate = IN1 && IN2;
	endfunction
endpackage : andgate_pkg
