//------ Module: sram6T_blwl -----//
//------ Verilog file: sram.v -----//
//------ Author: Xifan TANG -----//
module sram6T_blwl(
//input read,
//input nequalize,
input din, // Data input
output dout, // Data output
output doutb, // Data output
input bl, // Bit line control signal
input wl, // Word line control signal
input blb // Inverted Bit line control signal
);
  //----- local variable need to be registered
  reg a;

  //----- when wl is enabled, we can read in data from bl
  always @(bl, wl) 
  begin
    //----- Cases to program internal memory bit 
    //----- case 1: bl = 1, wl = 1, a -> 0
    if ((1'b1 == bl)&&(1'b1 == wl)) begin
      a <= 1'b1;
    end 
    //----- case 2: bl = 0, wl = 1, a -> 0
    if ((1'b0 == bl)&&(1'b1 == wl)) begin
     a <= 1'b0;
    end 
  end

  // dout is short-wired to din 
  assign dout = a;
  //---- doutb is always opposite to dout 
  assign doutb = ~dout;
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
