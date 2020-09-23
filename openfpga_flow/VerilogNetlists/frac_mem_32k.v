//-----------------------------------------------------
// Design Name : frac_mem_32k
// File Name   : frac_mem_32k.v
// Function    : A fracturable BRAM which can operate in 13 modes
//               - Single port RAM 512x64 bit 
//               - Single port RAM 1024x32 bit 
//               - Single port RAM 2048x16 bit 
//               - Single port RAM 4096x8 bit 
//               - Single port RAM 8192x4 bit 
//               - Single port RAM 16384x2 bit 
//               - Single port RAM 32768x1 bit 
//               - Dual port RAM 1024x32
//               - Dual port RAM 2048x16 bit 
//               - Dual port RAM 4096x8 bit 
//               - Dual port RAM 8192x4 bit 
//               - Dual port RAM 16384x2 bit 
//               - Dual port RAM 32768x1 bit 
//               Inspired by the dual port ram Verilog from
//               https://www.intel.com/content/www/us/en/programmable/support/support-resources/design-examples/design-software/vhdl/vhd-true-dual-port-ram-sclk.html
// Coder       : Xifan Tang
//-----------------------------------------------------

module frac_mem_32k (
  input [0:14] addr_a,
  input [0:14] addr_b,
  input [0:31] data_a,
  input [0:31] data_b,
  input we_a,
  input we_b,
  output reg [0:31] q_a,
  output reg [0:31] q_b,
  input clk,
  input [0:3] mode);

  reg [0:9] ram_a [0:31];
  reg [0:9] ram_b [0:31];

  always @(posedge clk) begin
    // Operating mode: single port RAM 512 x 64
    if (4'b0000 == mode) begin 
      if (we_a) begin
        ram_a[addr_a[0:8]] <= data_a;
        ram_b[addr_a[0:8]] <= data_b;
        q_a <= data_a;
        q_b <= data_b;
      end else begin
        q_a <= ram_a[addr_a[0:8]];
        q_b <= ram_b[addr_a[0:8]];
      end 
    // Operating mode: single port RAM 1024 x 32
    end else if (4'b0001 == mode) begin
      if (we_a) begin
        ram_a[addr_a[0:9]] <= data_a;
      end else begin
        q_a <= ram_a[addr_a[0:9]];
      end
    // Operating mode: single port RAM 2048 x 16
    end else if (4'b0010 == mode) begin
      if (we_a) begin
        case (addr_a[10:10])
          1'b0 : ram_a[addr_a[0:9]][0:15] <= data_a[0:15];
          1'b1 : ram_a[addr_a[0:9]][16:31] <= data_a[0:15];
        endcase
      end else begin
        case (addr_a[10:10])
          1'b0 : q_a <= ram_a[addr_a[0:9]][0:15];
          1'b1 : q_a <= ram_a[addr_a[0:9]][16:31];
        endcase
      end
    // Operating mode: single port RAM 4096 x 8
    end else if (4'b0011 == mode) begin
      if (we_a) begin
        case (addr_a[10:11])
          2'b00 : ram_a[addr_a[0:9]][0:7] <= data_a[0:7];
          2'b01 : ram_a[addr_a[0:9]][8:15] <= data_a[0:7];
          2'b10 : ram_a[addr_a[0:9]][16:23] <= data_a[0:7];
          2'b11 : ram_a[addr_a[0:9]][24:31] <= data_a[0:7];
        endcase
      end else begin
        case (addr_a[10:11])
          2'b00 : q_a <= ram_a[addr_a[0:9]][0:7];
          2'b01 : q_a <= ram_a[addr_a[0:9]][8:15];
          2'b10 : q_a <= ram_a[addr_a[0:9]][16:23];
          2'b11 : q_a <= ram_a[addr_a[0:9]][24:31];
        endcase
      end
    // Operating mode: single port RAM 8192 x 4
    end else if (4'b0100 == mode) begin
      if (we_a) begin
        case (addr_a[10:12])
          3'b000 : ram_a[addr_a[0:9]][0:3] <= data_a[0:3];
          3'b001 : ram_a[addr_a[0:9]][4:7] <= data_a[0:3];
          3'b010 : ram_a[addr_a[0:9]][8:11] <= data_a[0:3];
          3'b011 : ram_a[addr_a[0:9]][12:15] <= data_a[0:3];
          3'b100 : ram_a[addr_a[0:9]][16:19] <= data_a[0:3];
          3'b101 : ram_a[addr_a[0:9]][20:23] <= data_a[0:3];
          3'b110 : ram_a[addr_a[0:9]][24:27] <= data_a[0:3];
          3'b111 : ram_a[addr_a[0:9]][28:31] <= data_a[0:3];
        endcase
      end else begin
        case (addr_a[10:12])
          3'b000 : q_a <= ram_a[addr_a[0:9]][0:3];
          3'b001 : q_a <= ram_a[addr_a[0:9]][4:7];
          3'b010 : q_a <= ram_a[addr_a[0:9]][8:11];
          3'b011 : q_a <= ram_a[addr_a[0:9]][12:15];
          3'b100 : q_a <= ram_a[addr_a[0:9]][16:19];
          3'b101 : q_a <= ram_a[addr_a[0:9]][20:23];
          3'b110 : q_a <= ram_a[addr_a[0:9]][24:27];
          3'b111 : q_a <= ram_a[addr_a[0:9]][28:31];
        endcase
      end
    // Operating mode: single port RAM 16384 x 2
    end else if (4'b0101 == mode) begin
      if (we_a) begin
        case (addr_a[10:13])
          4'b0000 : ram_a[addr_a[0:9]][0:1] <= data_a[0:1];
          4'b0001 : ram_a[addr_a[0:9]][2:3] <= data_a[0:1];
          4'b0010 : ram_a[addr_a[0:9]][4:5] <= data_a[0:1];
          4'b0011 : ram_a[addr_a[0:9]][6:7] <= data_a[0:1];
          4'b0100 : ram_a[addr_a[0:9]][8:9] <= data_a[0:1];
          4'b0101 : ram_a[addr_a[0:9]][10:11] <= data_a[0:1];
          4'b0110 : ram_a[addr_a[0:9]][12:13] <= data_a[0:1];
          4'b0111 : ram_a[addr_a[0:9]][14:15] <= data_a[0:1];
          4'b1000 : ram_a[addr_a[0:9]][16:17] <= data_a[0:1];
          4'b1001 : ram_a[addr_a[0:9]][18:19] <= data_a[0:1];
          4'b1010 : ram_a[addr_a[0:9]][20:21] <= data_a[0:1];
          4'b1011 : ram_a[addr_a[0:9]][22:23] <= data_a[0:1];
          4'b1100 : ram_a[addr_a[0:9]][24:25] <= data_a[0:1];
          4'b1101 : ram_a[addr_a[0:9]][26:27] <= data_a[0:1];
          4'b1110 : ram_a[addr_a[0:9]][28:29] <= data_a[0:1];
          4'b1111 : ram_a[addr_a[0:9]][30:31] <= data_a[0:1];
        endcase
      end else begin
        case (addr_a[10:13])
          4'b0000 : q_a <= ram_a[addr_a[0:9]][0:1];
          4'b0001 : q_a <= ram_a[addr_a[0:9]][2:3];
          4'b0010 : q_a <= ram_a[addr_a[0:9]][4:5];
          4'b0011 : q_a <= ram_a[addr_a[0:9]][6:7];
          4'b0100 : q_a <= ram_a[addr_a[0:9]][8:9];
          4'b0101 : q_a <= ram_a[addr_a[0:9]][10:11];
          4'b0110 : q_a <= ram_a[addr_a[0:9]][12:13];
          4'b0111 : q_a <= ram_a[addr_a[0:9]][14:15];
          4'b1000 : q_a <= ram_a[addr_a[0:9]][16:17];
          4'b1001 : q_a <= ram_a[addr_a[0:9]][18:19];
          4'b1010 : q_a <= ram_a[addr_a[0:9]][20:21];
          4'b1011 : q_a <= ram_a[addr_a[0:9]][22:23];
          4'b1100 : q_a <= ram_a[addr_a[0:9]][24:25];
          4'b1101 : q_a <= ram_a[addr_a[0:9]][26:27];
          4'b1110 : q_a <= ram_a[addr_a[0:9]][28:29];
          4'b1111 : q_a <= ram_a[addr_a[0:9]][30:31];
        endcase
      end
    // Operating mode: single port RAM 32768 x 1
    end else if (4'b0110 == mode) begin
      if (we_a) begin
        ram_a[addr_a[0:9]][addr_a[10:14]] = data_a[0:0];
      end else begin
        q_a <= ram_a[addr_a[0:9]][addr_a[10:14]];
      end
    // Operating mode: dual port RAM 1024 x 32
    end else if (4'b0111 == mode) begin 
      if (we_a) begin
        ram_a[addr_a[0:9]] <= data_a;
        ram_b[addr_b[0:9]] <= data_b;
        q_a <= data_a;
        q_b <= data_b;
      end else begin
        q_a <= ram_a[addr_a[0:9]];
        q_b <= ram_b[addr_b[0:9]];
      end 
    // Operating mode: dual port RAM 2048 x 16
    end else if (4'b1000 == mode) begin
      if (we_a) begin
        case (addr_a[10:10])
          1'b0 : ram_a[addr_a[0:9]][0:15] <= data_a;
          1'b1 : ram_a[addr_a[0:9]][16:31] <= data_a;
        endcase
      end else begin
        case (addr_a[10:10])
          1'b0 : q_a <= ram_a[addr_a[0:9]][0:15];
          1'b1 : q_a <= ram_a[addr_a[0:9]][16:31];
        endcase
      end

      if (we_b) begin
        case (addr_b[10:10])
          1'b0 : ram_b[addr_b[0:9]][0:15] <= data_b;
          1'b1 : ram_b[addr_b[0:9]][16:31] <= data_b;
        endcase
      end else begin
        case (addr_b[10:10])
          1'b0 : q_b <= ram_b[addr_b[0:9]][0:15];
          1'b1 : q_b <= ram_b[addr_b[0:9]][16:31];
        endcase
      end
    // Operating mode: dual port RAM 4096 x 8
    end else if (4'b1001 == mode) begin
      if (we_a) begin
        case (addr_a[10:11])
          2'b00 : ram_a[addr_a[0:9]][0:7] <= data_a[0:7];
          2'b01 : ram_a[addr_a[0:9]][8:15] <= data_a[0:7];
          2'b10 : ram_a[addr_a[0:9]][16:23] <= data_a[0:7];
          2'b11 : ram_a[addr_a[0:9]][24:31] <= data_a[0:7];
        endcase
      end else begin
        case (addr_a[10:11])
          2'b00 : q_a <= ram_a[addr_a[0:9]][0:7];
          2'b01 : q_a <= ram_a[addr_a[0:9]][8:15];
          2'b10 : q_a <= ram_a[addr_a[0:9]][16:23];
          2'b11 : q_a <= ram_a[addr_a[0:9]][24:31];
        endcase
      end

      if (we_b) begin
        case (addr_b[10:11])
          2'b00 : ram_b[addr_b[0:9]][0:7] <= data_b[0:7];
          2'b01 : ram_b[addr_b[0:9]][8:15] <= data_b[0:7];
          2'b10 : ram_b[addr_b[0:9]][16:23] <= data_b[0:7];
          2'b11 : ram_b[addr_b[0:9]][24:31] <= data_b[0:7];
        endcase
      end else begin
        case (addr_b[10:11])
          2'b00 : q_b <= ram_b[addr_b[0:9]][0:7];
          2'b01 : q_b <= ram_b[addr_b[0:9]][8:15];
          2'b10 : q_b <= ram_b[addr_b[0:9]][16:23];
          2'b11 : q_b <= ram_b[addr_b[0:9]][24:31];
        endcase
      end
    // Operating mode: dual port RAM 8192 x 4
    end else if (4'b1011 == mode) begin
      if (we_a) begin
        case (addr_a[10:12])
          3'b000 : ram_a[addr_a[0:9]][0:3] <= data_a[0:3];
          3'b001 : ram_a[addr_a[0:9]][4:7] <= data_a[0:3];
          3'b010 : ram_a[addr_a[0:9]][8:11] <= data_a[0:3];
          3'b011 : ram_a[addr_a[0:9]][12:15] <= data_a[0:3];
          3'b100 : ram_a[addr_a[0:9]][16:19] <= data_a[0:3];
          3'b101 : ram_a[addr_a[0:9]][20:23] <= data_a[0:3];
          3'b110 : ram_a[addr_a[0:9]][24:27] <= data_a[0:3];
          3'b111 : ram_a[addr_a[0:9]][28:31] <= data_a[0:3];
        endcase
      end else begin
        case (addr_a[10:12])
          3'b000 : q_a <= ram_a[addr_a[0:9]][0:3];
          3'b001 : q_a <= ram_a[addr_a[0:9]][4:7];
          3'b010 : q_a <= ram_a[addr_a[0:9]][8:11];
          3'b011 : q_a <= ram_a[addr_a[0:9]][12:15];
          3'b100 : q_a <= ram_a[addr_a[0:9]][16:19];
          3'b101 : q_a <= ram_a[addr_a[0:9]][20:23];
          3'b110 : q_a <= ram_a[addr_a[0:9]][24:27];
          3'b111 : q_a <= ram_a[addr_a[0:9]][28:31];
        endcase
      end
      if (we_b) begin
        case (addr_b[10:12])
          3'b000 : ram_b[addr_b[0:9]][0:3] <= data_b[0:3];
          3'b001 : ram_b[addr_b[0:9]][4:7] <= data_b[0:3];
          3'b010 : ram_b[addr_b[0:9]][8:11] <= data_b[0:3];
          3'b011 : ram_b[addr_b[0:9]][12:15] <= data_b[0:3];
          3'b100 : ram_b[addr_b[0:9]][16:19] <= data_b[0:3];
          3'b101 : ram_b[addr_b[0:9]][20:23] <= data_b[0:3];
          3'b110 : ram_b[addr_b[0:9]][24:27] <= data_b[0:3];
          3'b111 : ram_b[addr_b[0:9]][28:31] <= data_b[0:3];
        endcase
      end else begin
        case (addr_b[10:12])
          3'b000 : q_b <= ram_b[addr_b[0:9]][0:3];
          3'b001 : q_b <= ram_b[addr_b[0:9]][4:7];
          3'b010 : q_b <= ram_b[addr_b[0:9]][8:11];
          3'b011 : q_b <= ram_b[addr_b[0:9]][12:15];
          3'b100 : q_b <= ram_b[addr_b[0:9]][16:19];
          3'b101 : q_b <= ram_b[addr_b[0:9]][20:23];
          3'b110 : q_b <= ram_b[addr_b[0:9]][24:27];
          3'b111 : q_b <= ram_b[addr_b[0:9]][28:31];
        endcase
      end
    // Operating mode: dual port RAM 16384 x 2
    end else if (4'b1100 == mode) begin
      if (we_a) begin
        case (addr_a[10:13])
          4'b0000 : ram_a[addr_a[0:9]][0:1] <= data_a[0:1];
          4'b0001 : ram_a[addr_a[0:9]][2:3] <= data_a[0:1];
          4'b0010 : ram_a[addr_a[0:9]][4:5] <= data_a[0:1];
          4'b0011 : ram_a[addr_a[0:9]][6:7] <= data_a[0:1];
          4'b0100 : ram_a[addr_a[0:9]][8:9] <= data_a[0:1];
          4'b0101 : ram_a[addr_a[0:9]][10:11] <= data_a[0:1];
          4'b0110 : ram_a[addr_a[0:9]][12:13] <= data_a[0:1];
          4'b0111 : ram_a[addr_a[0:9]][14:15] <= data_a[0:1];
          4'b1000 : ram_a[addr_a[0:9]][16:17] <= data_a[0:1];
          4'b1001 : ram_a[addr_a[0:9]][18:19] <= data_a[0:1];
          4'b1010 : ram_a[addr_a[0:9]][20:21] <= data_a[0:1];
          4'b1011 : ram_a[addr_a[0:9]][22:23] <= data_a[0:1];
          4'b1100 : ram_a[addr_a[0:9]][24:25] <= data_a[0:1];
          4'b1101 : ram_a[addr_a[0:9]][26:27] <= data_a[0:1];
          4'b1110 : ram_a[addr_a[0:9]][28:29] <= data_a[0:1];
          4'b1111 : ram_a[addr_a[0:9]][30:31] <= data_a[0:1];
        endcase
      end else begin
        case (addr_a[10:13])
          4'b0000 : q_a <= ram_a[addr_a[0:9]][0:1];
          4'b0001 : q_a <= ram_a[addr_a[0:9]][2:3];
          4'b0010 : q_a <= ram_a[addr_a[0:9]][4:5];
          4'b0011 : q_a <= ram_a[addr_a[0:9]][6:7];
          4'b0100 : q_a <= ram_a[addr_a[0:9]][8:9];
          4'b0101 : q_a <= ram_a[addr_a[0:9]][10:11];
          4'b0110 : q_a <= ram_a[addr_a[0:9]][12:13];
          4'b0111 : q_a <= ram_a[addr_a[0:9]][14:15];
          4'b1000 : q_a <= ram_a[addr_a[0:9]][16:17];
          4'b1001 : q_a <= ram_a[addr_a[0:9]][18:19];
          4'b1010 : q_a <= ram_a[addr_a[0:9]][20:21];
          4'b1011 : q_a <= ram_a[addr_a[0:9]][22:23];
          4'b1100 : q_a <= ram_a[addr_a[0:9]][24:25];
          4'b1101 : q_a <= ram_a[addr_a[0:9]][26:27];
          4'b1110 : q_a <= ram_a[addr_a[0:9]][28:29];
          4'b1111 : q_a <= ram_a[addr_a[0:9]][30:31];
        endcase
      end
      if (we_b) begin
        case (addr_b[10:13])
          4'b0000 : ram_b[addr_b[0:9]][0:1] <= data_b[0:1];
          4'b0001 : ram_b[addr_b[0:9]][2:3] <= data_b[0:1];
          4'b0010 : ram_b[addr_b[0:9]][4:5] <= data_b[0:1];
          4'b0011 : ram_b[addr_b[0:9]][6:7] <= data_b[0:1];
          4'b0100 : ram_b[addr_b[0:9]][8:9] <= data_b[0:1];
          4'b0101 : ram_b[addr_b[0:9]][10:11] <= data_b[0:1];
          4'b0110 : ram_b[addr_b[0:9]][12:13] <= data_b[0:1];
          4'b0111 : ram_b[addr_b[0:9]][14:15] <= data_b[0:1];
          4'b1000 : ram_b[addr_b[0:9]][16:17] <= data_b[0:1];
          4'b1001 : ram_b[addr_b[0:9]][18:19] <= data_b[0:1];
          4'b1010 : ram_b[addr_b[0:9]][20:21] <= data_b[0:1];
          4'b1011 : ram_b[addr_b[0:9]][22:23] <= data_b[0:1];
          4'b1100 : ram_b[addr_b[0:9]][24:25] <= data_b[0:1];
          4'b1101 : ram_b[addr_b[0:9]][26:27] <= data_b[0:1];
          4'b1110 : ram_b[addr_b[0:9]][28:29] <= data_b[0:1];
          4'b1111 : ram_b[addr_b[0:9]][30:31] <= data_b[0:1];
        endcase
      end else begin
        case (addr_b[10:13])
          4'b0000 : q_b <= ram_b[addr_b[0:9]][0:1];
          4'b0001 : q_b <= ram_b[addr_b[0:9]][2:3];
          4'b0010 : q_b <= ram_b[addr_b[0:9]][4:5];
          4'b0011 : q_b <= ram_b[addr_b[0:9]][6:7];
          4'b0100 : q_b <= ram_b[addr_b[0:9]][8:9];
          4'b0101 : q_b <= ram_b[addr_b[0:9]][10:11];
          4'b0110 : q_b <= ram_b[addr_b[0:9]][12:13];
          4'b0111 : q_b <= ram_b[addr_b[0:9]][14:15];
          4'b1000 : q_b <= ram_b[addr_b[0:9]][16:17];
          4'b1001 : q_b <= ram_b[addr_b[0:9]][18:19];
          4'b1010 : q_b <= ram_b[addr_b[0:9]][20:21];
          4'b1011 : q_b <= ram_b[addr_b[0:9]][22:23];
          4'b1100 : q_b <= ram_b[addr_b[0:9]][24:25];
          4'b1101 : q_b <= ram_b[addr_b[0:9]][26:27];
          4'b1110 : q_b <= ram_b[addr_b[0:9]][28:29];
          4'b1111 : q_b <= ram_b[addr_b[0:9]][30:31];
        endcase
      end
    // Operating mode: dual port RAM 32768 x 1
    end else if (4'b1101 == mode) begin
      if (we_a) begin
        ram_a[addr_a[0:9]][addr_a[10:14]] = data_a[0:0];
      end else begin
        q_a <= ram_a[addr_a[0:9]][addr_a[10:14]];
      end
      if (we_b) begin
        ram_b[addr_b[0:9]][addr_b[10:14]] = data_b[0:0];
      end else begin
        q_b <= ram_b[addr_b[0:9]][addr_b[10:14]];
      end
    end
  end
endmodule
