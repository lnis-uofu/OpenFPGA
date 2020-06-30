`ifndef BS_SEQ_ITEM
`define BS_SEQ_ITEM

class bs_seq_item extends uvm_sequence_item;

   rand bit [0:20] address; // Oversizing address width, we deal with parameter at interface level
   rand bit [0:0]  data_in;
   rand bit [0:63] IE;    // Oversizing aswell, can be reduced manually to GPIO_WIDTH to enhance performances

  //Utility and Field macros
  `uvm_object_utils_begin(bs_seq_item)
   `uvm_field_int(address,UVM_ALL_ON)
   `uvm_field_int(data_in,UVM_ALL_ON)
   `uvm_field_int(IE,UVM_ALL_ON)
  `uvm_object_utils_end
   
   /**
    * Default constructor.
    */
   extern function new(string name="bs_seq_item"); 
 
// Constraint to enhance performances during programming phase
// IE isn't randomized and is set at the end of programming phase in the sequence
   constraint IE_cons {
	soft IE == 64'hFFFFFFFFFFFFFFFF;
   }
endclass : bs_seq_item



function bs_seq_item::new(string name = "bs_seq_item");

    super.new(name);

endfunction


`endif // BS_SEQ_ITEM
