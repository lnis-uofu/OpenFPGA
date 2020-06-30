// Reference model class to check the 
// inverter function of OpenFPGA

`ifndef REFERENCE_MODEL
`define REFERENCE_MODEL

class reference_model extends uvm_component;

   // Objects
   openfpga_env_cfg    	m_openfpga_env_cfg;
   openfpga_env_cntxt  	m_openfpga_env_cntxt;

   // test_case
   string test_name;

   uvm_analysis_imp	#(stimuli_seq_item,reference_model) port;
   uvm_analysis_port	#(stimuli_seq_item) ap;

   stimuli_seq_item    	pkt_qu[$];

   `uvm_component_utils_begin(reference_model)
      `uvm_field_object(m_openfpga_env_cfg  , UVM_DEFAULT)
      `uvm_field_object(m_openfpga_env_cntxt, UVM_DEFAULT)
   `uvm_component_utils_end

   /**
    * Default constructor.
    */
   extern function new(string name="reference_model", uvm_component parent=null);
   /**
    * Builds ports and cfg/cntxt objects
    */
   extern virtual function void build_phase(uvm_phase phase);
   /**
    * TODO : Implement TLM
    */
   extern virtual function void connect_phase(uvm_phase phase);
  /**
    * Receive driver Pkt
    */
   extern virtual function void write(stimuli_seq_item trans_collected);
   /**
    * Pass the transaction item to the reference model
    * Switch the reference model according to test_case
    */
   extern virtual task run_phase(uvm_phase phase);

endclass : reference_model


function reference_model::new(string name="reference_model", uvm_component parent=null);

	super.new(name,parent);

endfunction : new



function void reference_model::build_phase(uvm_phase phase);
   
   super.build_phase(phase);
   
   void'(uvm_config_db#(openfpga_env_cfg)::get(this, "", "cfg", m_openfpga_env_cfg));
   if (!m_openfpga_env_cfg) begin
      `uvm_fatal("CFG", "Configuration handle is null")
   end
   else begin
      `uvm_info("CFG", $sformatf("Found configuration handle:\n%s", m_openfpga_env_cfg.sprint()), UVM_DEBUG)
   end
   
   void'(uvm_config_db#(openfpga_env_cntxt)::get(this, "", "cntxt", m_openfpga_env_cntxt));
      if (!m_openfpga_env_cntxt) begin
         `uvm_fatal("CNTXT", "Context handle is null")
      end
    if (uvm_config_db #(string) :: get (null, "uvm_test_top", "test_name", test_name))
      `uvm_info ("ENV", $sformatf ("Found %s", test_name), UVM_MEDIUM)
   
      port = new("port", this);
      ap = new("ap", this);
endfunction : build_phase

function void reference_model::connect_phase(uvm_phase phase);

   super.connect_phase(phase);
   

endfunction : connect_phase

function void reference_model::write(stimuli_seq_item trans_collected); // Print the top item in the queue

    `uvm_info(get_type_name(),$sformatf(" Value of sequence item in reference model \n"),UVM_LOW)
    trans_collected.print();
    pkt_qu.push_back(trans_collected);

endfunction:write

task reference_model::run_phase(uvm_phase phase);




   stimuli_seq_item new_tr;
   super.run_phase(phase);

	case (test_name)
	"andgate":     	begin  
				`uvm_info ("ENV", $sformatf ("Found %s", test_name), UVM_MEDIUM)
			end
	"benchmark":     begin
				`uvm_info ("ENV", $sformatf ("Found %s", test_name), UVM_MEDIUM)
			end
        endcase
 

   forever begin
     wait(pkt_qu.size()>0); 



      new_tr=pkt_qu.pop_front();

   case (test_name)
/*	"INV":
		begin

 		new_tr.gfpga_pad_GPIO_IN_drv[2] = inv_pkg::inv(new_tr.gfpga_pad_GPIO_IN_drv[7]); // Pinout to be adapted

		end*/
	"andgate":begin
                new_tr.gfpga_pad_GPIO_IN_drv[46] = andgate_pkg::andgate(new_tr.gfpga_pad_GPIO_IN_drv[17],new_tr.gfpga_pad_GPIO_IN_drv[40]);
      			 
		end

// To be adapted to benchmark in case you want to use scoreboard
// The following case is valid for the andgate benchmark

	"benchmark":begin
                new_tr.gfpga_pad_GPIO_IN_drv[46] = andgate_pkg::andgate(new_tr.gfpga_pad_GPIO_IN_drv[17],new_tr.gfpga_pad_GPIO_IN_drv[40]);
      			 
		end
// End

  endcase
      `uvm_info("my_model", $sformatf("get one transaction, copy and print it: "), UVM_LOW)	// Debug purpose for andgate
      `uvm_info(get_type_name(),$sformatf(" A = %0d", new_tr.gfpga_pad_GPIO_IN_drv[17]),UVM_LOW)
      `uvm_info(get_type_name(),$sformatf(" B = %0d", new_tr.gfpga_pad_GPIO_IN_drv[40]),UVM_LOW)
      `uvm_info(get_type_name(),$sformatf(" C = %0d", new_tr.gfpga_pad_GPIO_IN_drv[46]),UVM_LOW)
      new_tr.print();
      ap.write(new_tr);      
   end
endtask : run_phase



`endif // REFERENCE_MODEL
