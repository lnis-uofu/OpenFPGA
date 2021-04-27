// FIFO buffer implemented with synchronous dual-port block ram
// Reference: 
//   https://embeddedthoughts.com/2016/07/13/fifo-buffer-using-block-ram-on-a-xilinx-spartan-3-fpga/
module fifo
  #( parameter ADDRESS_WIDTH = 12, // number of words in ram
                 DATA_WIDTH    =  8  // number of bits in word
   )
   
  // IO ports
  (
    input wire clk, reset,
    input wire read, write,
    input wire [DATA_WIDTH-1:0] write_data,
    output wire empty, full,
    output wire [DATA_WIDTH-1:0] read_data
  );
  
  // internal signal declarations
  reg [ADDRESS_WIDTH-1:0] write_address_reg, write_address_next, write_address_after;
  reg [ADDRESS_WIDTH-1:0] read_address_reg, read_address_next, read_address_after;
  reg full_reg, empty_reg, full_next, empty_next;
  wire write_en;
  
  // write enable is asserted when write input is asserted and FIFO isn't full
  assign write_en = write & ~full_reg;
  
  // instantiate synchronous block ram
  sync_dual_port_ram #(.ADDRESS_WIDTH(ADDRESS_WIDTH), .DATA_WIDTH(DATA_WIDTH)) ram
                       (.clk(clk), .write_en(write_en), .write_address(write_address_reg),
      .read_address(read_address_reg), .write_data_in(write_data),
      .write_data_out(), .read_data_out(read_data));
  
  // register for address pointers, full/empty status
  always @(posedge clk, posedge reset)
    if (reset)
        begin
                    write_address_reg <= 0;
                    read_address_reg  <= 0;
                    full_reg          <= 1'b0;
                    empty_reg         <= 1'b1;
        end
    else
        begin
                    write_address_reg <= write_address_next;
                    read_address_reg  <= read_address_next;
                    full_reg          <= full_next;
                    empty_reg         <= empty_next;
        end
      
  // next-state logic for address index values after read/write operations
  always @*
    begin
    write_address_after = write_address_reg + 1;
    read_address_after  = read_address_reg + 1;
    end
    
  // next-state logic for address pointers
  always @*
    begin
    // defaults
    write_address_next = write_address_reg;
    read_address_next  = read_address_reg;
    full_next          = full_reg;
    empty_next         = empty_reg;
    
    // if read input asserted and FIFO isn't empty
    if(read && ~empty_reg && ~write)
      begin
      read_address_next = read_address_after;       // read address moves forward
      full_next = 1'b0;                             // FIFO isn't full if a read occured
      
      if (read_address_after == write_address_reg)  // if read address caught up with write address,
        empty_next = 1'b1;                        // FIFO is empty
      end
    
    // if write input asserted and FIFO isn't full
    else if(write && ~full_reg && ~read)
      begin
      write_address_next = write_address_after;     // write address moves forward
      empty_next = 1'b0;                            // FIFO isn't empty if write occured
      
      if (write_address_after == read_address_reg)    // if write address caught up with read address
        full_next = 1'b1;                         // FIFO is full
                        end
    
    // if write and read are asserted
                else if(write && read)
      begin
      write_address_next = write_address_after;     // write address moves forward
      read_address_next  = read_address_after;      // read address moves forward
                        end
    end 
  
   // assign full/empty status to output ports
   assign full  = full_reg;
   assign empty = empty_reg;
endmodule