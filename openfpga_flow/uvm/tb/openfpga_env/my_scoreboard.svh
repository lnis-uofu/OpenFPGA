`ifndef MY_SCOREBOARD
`define MY_SCOREBOARD

//Declaring two ports
`uvm_analysis_imp_decl(_dut)
`uvm_analysis_imp_decl(_ref)

class my_scoreboard extends uvm_scoreboard;

   `uvm_component_utils(my_scoreboard)
   uvm_analysis_imp_dut#(stimuli_seq_item,my_scoreboard)	dut_collected_export; // Port connected to DUT transaction
   uvm_analysis_imp_ref#(stimuli_seq_item,my_scoreboard)	ref_collected_export; // Port connected to Reference model transaction


   stimuli_seq_item       					pkt_dut_qu[$]; // Packet queue for transaction coming from DUT
   stimuli_seq_item						pkt_ref_qu[$]; // Packet queue for transaction coming from Reference model

   /**
    * Default constructor.
    */
   extern function new(string name="my_scoreboard", uvm_component parent=null);
   
   /**
    * Builds all components via create_<x>()
    */
   extern function void build_phase(uvm_phase phase);
  /**
    * Receive DUT Pkt
    */
   extern virtual function void write_dut(stimuli_seq_item trans_collected);
   /**
    * Receive Ref Pkt
    */
   extern virtual function void write_ref(stimuli_seq_item trans_collected);
   /**
    * Compare DUT Output to reference model
    */
   extern virtual task run_phase(uvm_phase phase);



endclass : my_scoreboard

function my_scoreboard::new(string name="my_scoreboard", uvm_component parent=null);

      super.new(name,parent);

endfunction : new

function void my_scoreboard::build_phase(uvm_phase phase);

      super.build_phase(phase);
      dut_collected_export = new("bs_collected_export",this);
      ref_collected_export = new("ref_export",this);
      
endfunction: build_phase


function void my_scoreboard::write_dut(stimuli_seq_item trans_collected); // Print the top item in the queue

    `uvm_info(get_type_name(),$sformatf(" Value of sequence item in SCOREBOARD from dut \n"),UVM_LOW)
   `uvm_info(get_type_name(),$sformatf(" GPIO = %0h", trans_collected.gfpga_pad_GPIO),UVM_LOW)
    trans_collected.print();
    pkt_dut_qu.push_back(trans_collected);

endfunction:write_dut
  

function void my_scoreboard::write_ref(stimuli_seq_item trans_collected); // Print the top item in the queue

    `uvm_info(get_type_name(),$sformatf(" Value of sequence item in SCOREBOARD from ref \n"),UVM_LOW)
    `uvm_info(get_type_name(),$sformatf(" GPIO = %0h", trans_collected.gfpga_pad_GPIO_IN_drv),UVM_LOW)
    trans_collected.print();
    pkt_ref_qu.push_back(trans_collected);
endfunction:write_ref


task my_scoreboard::run_phase (uvm_phase phase);

    // Handle to transactions
    stimuli_seq_item 	dut_seq;
    stimuli_seq_item	ref_seq;
    forever begin

      wait(pkt_dut_qu.size()>0 && pkt_ref_qu.size()>0);
 // If the queue isn't empty
      dut_seq=pkt_dut_qu.pop_front(); 			    // Pick the top transaction coming out of our DUT
      ref_seq=pkt_ref_qu.pop_front(); 		    // Pick the top transaction coming out of the reference model
	ref_seq.print();
	dut_seq.print();
      if(dut_seq.gfpga_pad_GPIO==ref_seq.gfpga_pad_GPIO_IN_drv)

	begin

	  `uvm_info("Pass",$sformatf("Test Passed"),UVM_LOW)
	end
      else
	begin
	  `uvm_info("Fail",$sformatf("Test Failed"),UVM_LOW)	
          `uvm_info(get_type_name(),$sformatf(" Expected Value = %0h", ref_seq.gfpga_pad_GPIO_IN_drv),UVM_LOW)
          `uvm_info(get_type_name(),$sformatf(" Actual Value   = %0h", dut_seq.gfpga_pad_GPIO),UVM_LOW)      
	end
    end	    
		    

endtask: run_phase


`endif // MY_SCOREBOARD
      
