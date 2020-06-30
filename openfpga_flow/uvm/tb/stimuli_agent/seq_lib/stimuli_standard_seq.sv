`ifndef "STIMULI_STANDARD_SEQ"
`define "STIMULI_STANDARD_SEQ"

class stimuli_standard_seq extends stimuli_base_seq;

   `uvm_object_utils(stimuli_standard_seq)

   /**
    * Default constructor.
    */
   extern function new(string name="stimuli_standard_seq");
   /**
    * Sequence body
    */
   extern virtual task body();

endclass : stimuli_standard_seq


function stimuli_standard_seq::new(string name = "stimuli_standard_seq");

	super.new(name);

endfunction // body

task stimuli_standard_seq::body();

     `uvm_info("Stimuli", $sformatf("stimuli sequence started"), UVM_LOW);     
     repeat(100)
     begin
			`uvm_do(req);
     end

     `uvm_info("Stimuli", $sformatf("stimuli sequence stopped"), UVM_LOW);
 
endtask // body
   

`endif // STIMULI_STANDARD_SEQ
