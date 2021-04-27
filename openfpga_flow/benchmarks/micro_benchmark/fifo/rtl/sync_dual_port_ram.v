// Synchronous dual-port block ram
// Reference: 
//   https://embeddedthoughts.com/2016/07/13/fifo-buffer-using-block-ram-on-a-xilinx-spartan-3-fpga/
module sync_dual_port_ram
		#( parameter ADDRESS_WIDTH = 12, // number of words in ram
                     DATA_WIDTH    =  8  // number of bits in word
	 )
	
	// IO ports
	(
		input wire clk,                                             // clk for synchronous read/write 
		input wire write_en,                                        // signal to enable synchronous write
		input wire [ADDRESS_WIDTH-1:0] read_address, write_address, // inputs for dual port addresses
		input wire [DATA_WIDTH-1:0] write_data_in,                  // input for data to write to ram
		output wire [DATA_WIDTH-1:0] read_data_out, write_data_out  // outputs for dual data ports
	);
	
	// internal signal declarations
	reg [DATA_WIDTH-1:0] ram [2**ADDRESS_WIDTH-1:0];             // ADDRESS_WIDTH x DATA_WIDTH RAM declaration
	reg [ADDRESS_WIDTH-1:0] read_address_reg, write_address_reg; // dual port address declarations
	
	// synchronous write and address update
	always @(posedge clk)
		begin
		if (write_en)  							 // if write enabled
		   ram[write_address] <= write_data_in; // write data to ram and write_address 
		    
                read_address_reg  <= read_address;      // store read_address to reg
	        write_address_reg <= write_address;     // store write_address to reg
	        end
	
	// assignments for two data out ports
	assign read_data_out  = ram[read_address_reg];
	assign write_data_out = ram[write_address_reg];
endmodule