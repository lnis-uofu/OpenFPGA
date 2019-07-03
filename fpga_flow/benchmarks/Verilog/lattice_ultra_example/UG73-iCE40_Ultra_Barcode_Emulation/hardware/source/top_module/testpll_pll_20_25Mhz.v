//==================================================================
//   >>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<
//   ------------------------------------------------------------------
//   Copyright (c) 2014 by Lattice Semiconductor Corporation
// 					ALL RIGHTS RESERVED 
//   ------------------------------------------------------------------
//
//   Permission:
//
//      Lattice SG Pte. Ltd. grants permission to use this code for use
//   in synthesis for any Lattice programmable logic product.  Other
//   use of this code, including the selling or duplication of any
//   portion is strictly prohibited.
  
//
//   Disclaimer:
//
//   This VHDL or Verilog source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Lattice provides no warranty
//   regarding the use or functionality of this code.
//
//   --------------------------------------------------------------------
//
//                  Lattice SG Pte. Ltd.
//                  101 Thomson Road, United Square #07-02 
// 					Singapore 307591
//	
//
//                  TEL: 1-800-Lattice (USA and Canada)
//	+65-6631-2000 (Singapore)
//                       +1-503-268-8001 (other locations)
//
//                  web: http://www.latticesemi.com/
//                  email: techsupport@latticesemi.com
//
//   --------------------------------------------------------------------
//


module testpll_pll(REFERENCECLK,
                   PLLOUTCORE,
                   PLLOUTGLOBAL,
                   RESET,
                   LOCK);

input REFERENCECLK;
input RESET;    /* To initialize the simulation properly, the RESET signal (Active Low) must be asserted at the beginning of the simulation */ 
output PLLOUTCORE;
output PLLOUTGLOBAL;
output LOCK;

SB_PLL40_CORE testpll_pll_inst(.REFERENCECLK(REFERENCECLK),
                               .PLLOUTCORE(PLLOUTCORE),
                               .PLLOUTGLOBAL(PLLOUTGLOBAL),
                               .EXTFEEDBACK(),
                               .DYNAMICDELAY(),
                               .RESETB(RESET),
                               .BYPASS(1'b0),
                               .LATCHINPUTVALUE(),
                               .LOCK(LOCK),
                               .SDI(),
                               .SDO(),
                               .SCLK());

//\\ Fin=27, Fout=20.05;
defparam testpll_pll_inst.DIVR = 4'b0000;
defparam testpll_pll_inst.DIVF = 7'b0010111;
defparam testpll_pll_inst.DIVQ = 3'b101;
defparam testpll_pll_inst.FILTER_RANGE = 3'b011;
defparam testpll_pll_inst.FEEDBACK_PATH = "SIMPLE";
defparam testpll_pll_inst.DELAY_ADJUSTMENT_MODE_FEEDBACK = "FIXED";
defparam testpll_pll_inst.FDA_FEEDBACK = 4'b0000;
defparam testpll_pll_inst.DELAY_ADJUSTMENT_MODE_RELATIVE = "FIXED";
defparam testpll_pll_inst.FDA_RELATIVE = 4'b0000;
defparam testpll_pll_inst.SHIFTREG_DIV_MODE = 2'b00;
defparam testpll_pll_inst.PLLOUT_SELECT = "GENCLK";
defparam testpll_pll_inst.ENABLE_ICEGATE = 1'b0;

endmodule
