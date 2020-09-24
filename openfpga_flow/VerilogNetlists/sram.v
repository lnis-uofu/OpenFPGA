//-----------------------------------------------------
// Design Name : sram_blwl
// File Name   : sram.v
// Function    : A SRAM cell is is accessible
//               when wl is enabled
// Coder       : Xifan TANG
//-----------------------------------------------------
module sram_blwl(
input reset, // Word line control signal
input wl, // Word line control signal
input bl, // Bit line control signal
output out, // Data output
output outb // Data output
);

  //----- local variable need to be registered
  reg data;

  //----- when wl is enabled, we can read in data from bl
  always @(bl or wl) 
  begin
    if (1'b1 == reset) begin 
      data <= 1'b0;
    end else if ((1'b1 == bl)&&(1'b1 == wl)) begin
    //----- Cases to program internal memory bit 
    //----- case 1: bl = 1, wl = 1, a -> 0
      data <= 1'b1;
    end else if ((1'b0 == bl)&&(1'b1 == wl)) begin
    //----- case 2: bl = 0, wl = 1, a -> 0
     data <= 1'b0;
    end 
  end

`ifndef ENABLE_FORMAL_VERIFICATION
    // Wire q_reg to Q
    assign out = data;
    assign outb = ~data;
`else
    assign out = 1'bZ;
    assign outb = !out;
`endif

endmodule

//-----------------------------------------------------
// Function    : A SRAM cell with 
//               - an active-high set
//               - a write-enable
//-----------------------------------------------------
module SRAMS(
  input SET, // active-high set signal
  input WE, // Word line control signal as write enable
  input D, // Bit line control signal as data input
  output Q, // Data output
  output QN // Data output
);

  //----- local variable need to be registered
  reg data;

  //----- when wl is enabled, we can read in data from bl
  always @(D or WE) 
  begin
    if (1'b1 == SET) begin 
      data <= 1'b1;
    end else if ((1'b1 == D)&&(1'b1 == WE)) begin
    //----- Cases to program internal memory bit 
    //----- case 1: bl = 1, wl = 1, a -> 0
      data <= 1'b1;
    end else if ((1'b0 == D)&&(1'b1 == WE)) begin
    //----- case 2: bl = 0, wl = 1, a -> 0
     data <= 1'b0;
    end 
  end

`ifndef ENABLE_FORMAL_VERIFICATION
    // Wire q_reg to Q
    assign Q = data;
    assign QN = ~data;
`else
    assign Q = 1'bZ;
    assign QN = !out;
`endif

endmodule

//-----------------------------------------------------
// Function    : A SRAM cell with 
//               - an active-low set
//               - a write-enable
//-----------------------------------------------------
module SRAMSN(
  input SETN, // active-low set signal
  input WE, // Word line control signal as write enable
  input D, // Bit line control signal as data input
  output Q, // Data output
  output QN // Data output
);

  //----- local variable need to be registered
  reg data;

  //----- when wl is enabled, we can read in data from bl
  always @(D or WE) 
  begin
    if (1'b0 == SETN) begin 
      data <= 1'b1;
    end else if ((1'b1 == D)&&(1'b1 == WE)) begin
    //----- Cases to program internal memory bit 
    //----- case 1: bl = 1, wl = 1, a -> 0
      data <= 1'b1;
    end else if ((1'b0 == D)&&(1'b1 == WE)) begin
    //----- case 2: bl = 0, wl = 1, a -> 0
     data <= 1'b0;
    end 
  end

`ifndef ENABLE_FORMAL_VERIFICATION
    // Wire q_reg to Q
    assign Q = data;
    assign QN = ~data;
`else
    assign Q = 1'bZ;
    assign QN = !out;
`endif

endmodule

//-----------------------------------------------------
// Function    : A SRAM cell with 
//               - an active-high reset
//               - a write-enable
//-----------------------------------------------------
module SRAMR(
  input RST, // active-high reset signal
  input WE, // Word line control signal as write enable
  input D, // Bit line control signal as data input
  output Q, // Data output
  output QN // Data output
);

  //----- local variable need to be registered
  reg data;

  //----- when wl is enabled, we can read in data from bl
  always @(D or WE) 
  begin
    if (1'b1 == RST) begin 
      data <= 1'b0;
    end else if ((1'b1 == D)&&(1'b1 == WE)) begin
    //----- Cases to program internal memory bit 
    //----- case 1: bl = 1, wl = 1, a -> 0
      data <= 1'b1;
    end else if ((1'b0 == D)&&(1'b1 == WE)) begin
    //----- case 2: bl = 0, wl = 1, a -> 0
     data <= 1'b0;
    end 
  end

`ifndef ENABLE_FORMAL_VERIFICATION
    // Wire q_reg to Q
    assign Q = data;
    assign QN = ~data;
`else
    assign Q = 1'bZ;
    assign QN = !out;
`endif

endmodule

//-----------------------------------------------------
// Function    : A SRAM cell with 
//               - an active-low reset
//               - a write-enable
//-----------------------------------------------------
module SRAMRN(
  input RSTN, // active-low reset signal
  input WE, // Word line control signal as write enable
  input D, // Bit line control signal as data input
  output Q, // Data output
  output QN // Data output
);

  //----- local variable need to be registered
  reg data;

  //----- when wl is enabled, we can read in data from bl
  always @(D or WE) 
  begin
    if (1'b0 == RSTN) begin 
      data <= 1'b0;
    end else if ((1'b1 == D)&&(1'b1 == WE)) begin
    //----- Cases to program internal memory bit 
    //----- case 1: bl = 1, wl = 1, a -> 0
      data <= 1'b1;
    end else if ((1'b0 == D)&&(1'b1 == WE)) begin
    //----- case 2: bl = 0, wl = 1, a -> 0
     data <= 1'b0;
    end 
  end

`ifndef ENABLE_FORMAL_VERIFICATION
    // Wire q_reg to Q
    assign Q = data;
    assign QN = ~data;
`else
    assign Q = 1'bZ;
    assign QN = !out;
`endif

endmodule

//-----------------------------------------------------
// Function    : A SRAM cell with 
//               - an active-high reset
//               - an active-high set
//               - a write-enable
//-----------------------------------------------------
module SRAMSR(
  input RST, // active-high reset signal
  input SET, // active-high set signal
  input WE, // Word line control signal as write enable
  input D, // Bit line control signal as data input
  output Q, // Data output
  output QN // Data output
);

  //----- local variable need to be registered
  reg data;

  //----- when wl is enabled, we can read in data from bl
  always @(D or WE) 
  begin
    if (1'b1 == RST) begin 
      data <= 1'b0;
    end else if (1'b1 == SET) begin 
      data <= 1'b1;
    end else if ((1'b1 == D)&&(1'b1 == WE)) begin
    //----- Cases to program internal memory bit 
    //----- case 1: bl = 1, wl = 1, a -> 0
      data <= 1'b1;
    end else if ((1'b0 == D)&&(1'b1 == WE)) begin
    //----- case 2: bl = 0, wl = 1, a -> 0
     data <= 1'b0;
    end 
  end

`ifndef ENABLE_FORMAL_VERIFICATION
    // Wire q_reg to Q
    assign Q = data;
    assign QN = ~data;
`else
    assign Q = 1'bZ;
    assign QN = !out;
`endif

endmodule

//-----------------------------------------------------
// Function    : A SRAM cell with 
//               - an active-low reset
//               - an active-low set
//               - a write-enable
//-----------------------------------------------------
module SRAMSNRN(
  input RSTN, // active-low reset signal
  input SETN, // active-low set signal
  input WE, // Word line control signal as write enable
  input D, // Bit line control signal as data input
  output Q, // Data output
  output QN // Data output
);

  //----- local variable need to be registered
  reg data;

  //----- when wl is enabled, we can read in data from bl
  always @(D or WE) 
  begin
    if (1'b0 == RSTN) begin 
      data <= 1'b0;
    end else if (1'b0 == SETN) begin 
      data <= 1'b1;
    end else if ((1'b1 == D)&&(1'b1 == WE)) begin
    //----- Cases to program internal memory bit 
    //----- case 1: bl = 1, wl = 1, a -> 0
      data <= 1'b1;
    end else if ((1'b0 == D)&&(1'b1 == WE)) begin
    //----- case 2: bl = 0, wl = 1, a -> 0
     data <= 1'b0;
    end 
  end

`ifndef ENABLE_FORMAL_VERIFICATION
    // Wire q_reg to Q
    assign Q = data;
    assign QN = ~data;
`else
    assign Q = 1'bZ;
    assign QN = !out;
`endif

endmodule


module sram6T_rram(
input read,
input nequalize,
input din, // Data input
output dout, // Data output
output doutb, // Data output
// !!! Port bit position should start from LSB to MSB
// Follow this convention for BL/WLs in each module!
input [0:2] bl, // Bit line control signal
input [0:2] wl// Word line control signal
);
  //----- local variable need to be registered
  //----- Modeling two RRAMs 
  reg r0, r1;

  always @(bl[0], wl[2]) 
  begin
    //----- Cases to program r0 
    //----- case 1: bl[0] = 1, wl[2] = 1, r0 -> 0
    if ((1'b1 == bl[0])&&(1'b1 == wl[2])) begin
      r0 <= 0;
    end 
  end
  
  always @(bl[2], wl[0]) 
  begin
    //----- case 2: bl[2] = 1, wl[0] = 1, r0 -> 1
    if ((1'b1 == bl[2])&&(1'b1 == wl[0])) begin
      r0 <= 1;
    end 
  end

  always @(bl[1], wl[2]) 
  begin
    //----- Cases to program r1 
    //----- case 1: bl[1] = 1, wl[2] = 1, r0 -> 0
    if ((1'b1 == bl[1])&&(1'b1 == wl[2])) begin
      r1 <= 0;
    end 
  end

  always @( bl[2], wl[1]) 
  begin
    //----- case 2: bl[2] = 1, wl[1] = 1, r0 -> 1
    if ((1'b1 == bl[2])&&(1'b1 == wl[1])) begin
      r1 <= 1;
    end
  end

  // dout is r0 AND r1  
  assign dout = r0 | (~r1);

  //---- doutb is always opposite to dout 
  assign doutb = ~dout;

endmodule
