//-----------------------------------------------------//
// Design Name : Shift_reg
// File Name   : Shift_reg_8192.v
// Function    : Shift register
//------------------------------------------------------//


module shift_reg_8192 #( parameter size = 8191 ) (shift_in, clk, shift_out);

   // Port Declaration
   input   shift_in;
   input   clk;
   output  shift_out;
   
   reg [ size:0 ] shift; // shift register  
   
    always @ (posedge clk)
     begin
	  shift = { shift[size-1:0] , shift_in } ;	
     end
   
   assign shift_out = shift[size];   
   
endmodule 
