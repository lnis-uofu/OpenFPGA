//-----------------------------------------------------
// Design Name : frac_mem_256
// File Name   : frac_mem_256.v
// Function    : A fracturable BRAM which can operate in 3 modes
//               - Dual port RAM 16x16 bit
//               - Dual port RAM 8x32 bit 
//               - Dual port RAM 32x8 bit 
//               Inspired by the dual port ram Verilog from
//               https://www.intel.com/content/www/us/en/programmable/support/support-resources/design-examples/design-software/vhdl/vhd-true-dual-port-ram-sclk.html
// Coder       : Xifan Tang
//-----------------------------------------------------

module frac_mem_256 (
  input [0:4] waddr,
  input [0:4] raddr,
  input [0:31] d_in,
  input wen,
  input ren,
  output [0:31] d_out,
  input wclk,
  input rclk,
  input [0:1] mode);

  reg [0:15] ram [0:3];
  reg [0:31] internal;
  assign d_out = internal;

  always @(posedge wclk) begin
    // Operating mode: dual port RAM 16 x 16
    if (2'b00 == mode) begin 
      if (wen) begin
        ram[waddr[0:3]][0:15] <= d_in[0:15];
      end
      if (ren) begin
        internal[0:15] = ram[raddr[0:3]][0:15];
        internal[16:31] = 16'b0;
      end
    // Operating mode: dual port RAM 8x32
    end else if (2'b01 == mode) begin
      if (wen) begin
        ram[{waddr[0:2], 1'b0}][0:15] <= d_in[0:15];
        ram[{waddr[0:2], 1'b1}][0:15] <= d_in[16:31];
      end
      if (ren) begin
        internal[0:15] = ram[{raddr[0:2], 1'b0}][0:15];
        internal[16:31] = ram[{raddr[0:2], 1'b0}][0:15];
     // Operating mode: dual port RAM 32x8
    end else if (2'b10 == mode) begin
      if (wen) begin
        if (waddr[4] == 1'b0) begin
          ram[waddr[0:3] >> 2][0:7] <= d_in[0:7];
        end else begin
          ram[waddr[0:3] >> 2][8:15] <= d_in[0:7];
        end
      end
      if (ren) begin
        if (raddr[4] == 1'b0) begin
          internal[0:7] = ram[raddr[0:3]][0:7];
        end else begin
          internal[0:7] = ram[raddr[0:3]][8:15];
        end
        internal[8:31] = 24'b0;
      end
     end
    end
  end
endmodule
