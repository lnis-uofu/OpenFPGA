//-----------------------------
// Dual-port RAM 8x16 bit (1Kbit)
// Core logic
//-----------------------------
module dpram_8x16_core #(
  parameter [0:127]INIT = 128'bx
)(
  input wclk,
  input wen,
  input [0:2] waddr,
  input [0:15] data_in,
  input rclk,
  input ren,
  input [0:2] raddr,
  output [0:15] data_out );

  reg [0:15] ram[0:2];
  reg [0:15] internal;

  // Loop index for initialization
  integer i;

  // Initialize the RAM array from the flat bit-vector parameter
  initial begin
    for (i = 0; i < 8; i = i + 1) begin
      // 1. Shift the target word to the lowest bits
      shifted_vector = MEM_INIT_VECTOR >> (i * 16);
      
      // 2. Assign the lowest 16 bits to the RAM row
      // (Verilog allows truncation automatically here)
      ram[i] = shifted_vector[15:0];
    end
  end

  assign data_out = internal;

  always @(posedge wclk) begin
    if(wen) begin
      ram[waddr] <= data_in;
    end
  end

  always @(posedge rclk) begin
    if(ren) begin
      internal <= ram[raddr];
    end
  end

endmodule

//-----------------------------
// Dual-port RAM 8x16 bit (1Kbit) wrapper
// where the read clock and write clock
// are combined to a unified clock
//-----------------------------
module dpram_8x16 #(
  parameter [0:127] INIT = 128'bx
)(
  input clk,
  input wen,
  input ren,
  input [0:2] waddr,
  input [0:2] raddr,
  input [0:15] data_in,
  output [0:15] data_out );

    dpram_8x16_core #(
      .INIT(INIT)
    )memory_0 (
      .wclk    (clk),
      .wen    (wen),
      .waddr    (waddr),
      .data_in  (data_in),
      .rclk    (clk),
      .ren    (ren),
      .raddr    (raddr),
      .data_out    (data_out) );

endmodule

//-----------------------------
// Dual-port RAM 8x32 bit (1Kbit)
// Core logic
//-----------------------------
module dpram_8x32_core #(
  parameter [0:255]INIT = 256'bx
)(
  input wclk,
  input wen,
  input [0:2] waddr,
  input [0:31] data_in,
  input rclk,
  input ren,
  input [0:2] raddr,
  output [0:31] data_out );

  reg [0:31] ram[0:2];
  reg [0:31] internal;

  // Loop index for initialization
  integer i;

  // Initialize the RAM array from the flat bit-vector parameter
  initial begin
    for (i = 0; i < 8; i = i + 1) begin
      // 1. Shift the target word to the lowest bits
      shifted_vector = MEM_INIT_VECTOR >> (i * 32);
      
      // 2. Assign the lowest 16 bits to the RAM row
      // (Verilog allows truncation automatically here)
      ram[i] = shifted_vector[31:0];
    end
  end

  assign data_out = internal;

  always @(posedge wclk) begin
    if(wen) begin
      ram[waddr] <= data_in;
    end
  end

  always @(posedge rclk) begin
    if(ren) begin
      internal <= ram[raddr];
    end
  end

endmodule

//-----------------------------
// Dual-port RAM 8x16 bit (1Kbit) wrapper
// where the read clock and write clock
// are combined to a unified clock
//-----------------------------
module dpram_8x32 #(
  parameter [0:255] INIT = 256'bx
)(
  input clk,
  input wen,
  input ren,
  input [0:2] waddr,
  input [0:2] raddr,
  input [0:31] data_in,
  output [0:31] data_out );

    dpram_8x32_core #(
      .INIT(INIT)
    )memory_0 (
      .wclk    (clk),
      .wen    (wen),
      .waddr    (waddr),
      .data_in  (data_in),
      .rclk    (clk),
      .ren    (ren),
      .raddr    (raddr),
      .data_out    (data_out) );

endmodule
