`ifndef RESET_AT_START
`define RESET_AT_START

class reset_at_start extends clknrst_base_seq;
   
  `uvm_object_utils(reset_at_start)

   /**
    * Default constructor.
    */
   extern function new(string name="reset_at_start");
   /**
    * Reset to be send to DUT
    */
   extern virtual task body(); 
  
endclass : reset_at_start


function reset_at_start::new(string name = "reset_at_start");

    super.new(name);

endfunction
   
task reset_at_start::body();

     begin
	`uvm_do_with(req,{req.pReset == 1'b1;
			  req.Reset  == 1'b1;
	})

	#(2*clknrst_bitstream_period)

	`uvm_do_with(req,{req.pReset == 1'b0;
			  req.Reset  == 1'b1; 
	})
     end
 
endtask // body

`endif // RESET_AT_START
