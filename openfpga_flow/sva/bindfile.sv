
//`ifndef SVA_PARAM
parameter enable_assertions = 1,
	  enable_one_fire_mode = 0,
	  enable_undefined_check = 1,
	  enable_break_on_error = 1

//`endif


bind CCFFX1_0 FF_checker #(.enable_assertions(enable_assertions),
			   .enable_one_fire_mode(enable_one_fire_mode),
			   .enable_undefined_check(1),
			   .enable_break_on_error(enable_break_on_error))
				sva_checker
				(.*);	


