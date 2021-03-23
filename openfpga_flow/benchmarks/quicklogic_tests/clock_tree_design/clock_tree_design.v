// clock tree design 1
//


module clock_tree_design(clk, t, clr_n, sel, mux_out);

input [5:0] clk;
input [1:0] sel;
input t, clr_n;


output reg [19:0]mux_out;
wire [4:0] SQ0;
wire [4:0] SQ1;
wire [4:0] SQ2;
wire [4:0] SQ3;
wire [4:0] SQ4;
wire [4:0] SQ5;
wire [4:0] SQ6;
wire [4:0] SQ7;
wire [4:0] SQ8;
wire [4:0] SQ9;
wire [4:0] SQ10;
wire [4:0] SQ11;
wire [4:0] SQ12;
wire [4:0] SQ13;
wire [4:0] SQ14;
wire [4:0] SQ15;


T_ff I0(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ0[0]));
T_ff I1(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ0[1]));
T_ff I2(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ0[2]));
T_ff I3(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ0[3]));
T_ff I4(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ0[4]));
T_ff I5(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ1[0]));
T_ff I6(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ1[1]));
T_ff I7(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ1[2]));
T_ff I8(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ1[3]));
T_ff I9(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ1[4]));
T_ff I10(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ2[0]));
T_ff I11(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ2[1]));
T_ff I12(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ2[2]));
T_ff I13(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ2[3]));
T_ff I14(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ2[4]));
T_ff I15(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ3[0]));
T_ff I16(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ3[1]));
T_ff I17(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ3[2]));
T_ff I18(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ3[3]));
T_ff I19(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ3[4]));
T_ff I20(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ4[0]));
T_ff I21(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ4[1]));
T_ff I22(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ4[2]));
T_ff I23(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ4[3]));
T_ff I24(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ4[4]));
T_ff I25(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ5[0]));
T_ff I26(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ5[1]));
T_ff I27(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ5[2]));
T_ff I28(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ5[3]));
T_ff I29(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ5[4]));
T_ff I30(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ6[0]));
T_ff I31(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ6[1]));
T_ff I32(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ6[2]));
T_ff I33(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ6[3]));
T_ff I34(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ6[4]));
T_ff I35(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ7[0]));
T_ff I36(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ7[1]));
T_ff I37(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ7[2]));
T_ff I38(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ7[3]));
T_ff I39(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ7[4]));
T_ff I40(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ8[0]));
T_ff I41(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ8[1]));
T_ff I42(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ8[2]));
T_ff I43(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ8[3]));
T_ff I44(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ8[4]));
T_ff I45(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ9[0]));
T_ff I46(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ9[1]));
T_ff I47(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ9[2]));
T_ff I48(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ9[3]));
T_ff I49(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ9[4]));
T_ff I50(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ10[0]));
T_ff I51(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ10[1]));
T_ff I52(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ10[2]));
T_ff I53(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ10[3]));
T_ff I54(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ10[4]));
T_ff I55(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ11[0]));
T_ff I56(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ11[1]));
T_ff I57(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ11[2]));
T_ff I58(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ11[3]));
T_ff I59(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ11[4]));
T_ff I60(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ12[0]));
T_ff I61(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ12[1]));
T_ff I62(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ12[2]));
T_ff I63(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ12[3]));
T_ff I64(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ12[4]));
T_ff I65(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ13[0]));
T_ff I66(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ13[1]));
T_ff I67(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ13[2]));
T_ff I68(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ13[3]));
T_ff I69(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ13[4]));
T_ff I70(.CLK(clk[0]), .CLR(clr_n), .T(t), .Q(SQ14[0]));
T_ff I71(.CLK(clk[1]), .CLR(clr_n), .T(t), .Q(SQ14[1]));
T_ff I72(.CLK(clk[2]), .CLR(clr_n), .T(t), .Q(SQ14[2]));
T_ff I73(.CLK(clk[3]), .CLR(clr_n), .T(t), .Q(SQ14[3]));
T_ff I74(.CLK(clk[4]), .CLR(clr_n), .T(t), .Q(SQ14[4]));
T_ff I75(.CLK(clk[5]), .CLR(clr_n), .T(t), .Q(SQ15[0]));
T_ff I76(.CLK(clk[5]), .CLR(clr_n), .T(t), .Q(SQ15[1]));
T_ff I77(.CLK(clk[5]), .CLR(clr_n), .T(t), .Q(SQ15[2]));
T_ff I78(.CLK(clk[5]), .CLR(clr_n), .T(t), .Q(SQ15[3]));
T_ff I79(.CLK(clk[5]), .CLR(clr_n), .T(t), .Q(SQ15[4]));

  always @ (SQ0[0] or SQ1[0] or SQ2[0] or SQ3[0] or sel) begin
      case (sel)
         2'b00 : mux_out[0] <= SQ0[0] ^ SQ1[0] ^ SQ2[0] ^ SQ3[0] ;
         2'b01 : mux_out[0] <= SQ0[0] & SQ1[0] & SQ2[0] & SQ3[0] ;
         2'b10 : mux_out[0] <= SQ0[0] | SQ1[0] |  SQ2[0] | SQ3[0] ;
         2'b11 : mux_out[0] <= 1'b0;
      endcase
   end

   always @ (SQ5[0] or SQ6[0] or SQ7[0] or SQ4[0] or sel) begin
      case (sel)
         2'b00 : mux_out[1] <= SQ5[0] ^ SQ6[0] ^ SQ7[0] ^ SQ4[0] ;
         2'b01 : mux_out[1] <= SQ5[0] & SQ6[0] & SQ7[0] & SQ4[0] ;
         2'b10 : mux_out[1] <= SQ5[0] | SQ6[0] |  SQ7[0] | SQ4[0] ;
         2'b11 : mux_out[1] <= 1'b0;
      endcase
   end

      always @ (SQ10[0] or SQ11[0] or SQ9[0] or SQ8[0] or sel) begin
      case (sel)
         2'b00 : mux_out[2] <= SQ10[0] ^ SQ11[0] ^ SQ9[0] ^ SQ8[0] ;
         2'b01 : mux_out[2] <= SQ10[0] & SQ11[0] & SQ9[0] & SQ8[0] ;
         2'b10 : mux_out[2] <= SQ10[0] | SQ11[0] |  SQ9[0] | SQ8[0] ;
         2'b11 : mux_out[2] <= 1'b0;
      endcase
   end

   always @ (SQ14[0] or SQ15[0] or SQ12[0] or SQ13[0] or sel) begin
      case (sel)
         2'b00 : mux_out[3] <= SQ14[0] ^ SQ15[0] ^ SQ12[0] ^ SQ13[0] ;
         2'b01 : mux_out[3] <= SQ14[0] & SQ15[0] & SQ12[0] & SQ13[0] ;
         2'b10 : mux_out[3] <= SQ14[0] | SQ15[0] |  SQ12[0] | SQ13[0] ;
         2'b11 : mux_out[3] <= 1'b0;
      endcase
   end

  always @ (SQ0[1] or SQ1[1] or SQ2[1] or SQ3[1] or sel) begin
      case (sel)
         2'b00 : mux_out[4] <= SQ0[1] ^ SQ1[1] ^ SQ2[1] ^ SQ3[1] ;
         2'b01 : mux_out[4] <= SQ0[1] & SQ1[1] & SQ2[1] & SQ3[1] ;
         2'b10 : mux_out[4] <= SQ0[1] | SQ1[1] |  SQ2[1] | SQ3[1] ;
         2'b11 : mux_out[4] <= 1'b0;
      endcase
   end

     always @ (SQ5[1] or SQ6[1] or SQ7[1] or SQ4[1] or sel) begin
      case (sel)
         2'b00 : mux_out[5] <= SQ5[1] ^ SQ6[1] ^ SQ7[1] ^ SQ4[1] ;
         2'b01 : mux_out[5] <= SQ5[1] & SQ6[1] & SQ7[1] & SQ4[1] ;
         2'b10 : mux_out[5] <= SQ5[1] | SQ6[1] |  SQ7[1] | SQ4[1] ;
         2'b11 : mux_out[5] <= 1'b0;
      endcase
   end

     always @ (SQ10[1] or SQ11[1] or SQ9[1] or SQ8[1] or sel) begin
      case (sel)
         2'b00 : mux_out[6] <= SQ10[1] ^ SQ11[1] ^ SQ9[1] ^ SQ8[1] ;
         2'b01 : mux_out[6] <= SQ10[1] & SQ11[1] & SQ9[1] & SQ8[1] ;
         2'b10 : mux_out[6] <= SQ10[1] | SQ11[1] |  SQ9[1] | SQ8[1] ;
         2'b11 : mux_out[6] <= 1'b0;
      endcase
   end

     always @ (SQ14[1] or SQ15[1] or SQ12[1] or SQ13[1] or sel) begin
      case (sel)
         2'b00 : mux_out[7] <= SQ14[1] ^ SQ15[1] ^ SQ12[1] ^ SQ13[1] ;
         2'b01 : mux_out[7] <= SQ14[1] & SQ15[1] & SQ12[1] & SQ13[1] ;
         2'b10 : mux_out[7] <= SQ14[1] | SQ15[1] |  SQ12[1] | SQ13[1] ;
         2'b11 : mux_out[7] <= 1'b0;
      endcase
   end


   always @ (SQ0[2] or SQ1[2] or SQ2[2] or SQ3[2] or sel) begin
      case (sel)
         2'b00 : mux_out[8] <= SQ0[2] ^ SQ1[2] ^ SQ2[2] ^ SQ3[2] ;
         2'b01 : mux_out[8] <= SQ0[2] & SQ1[2] & SQ2[2] & SQ3[2] ;
         2'b10 : mux_out[8] <= SQ0[2] | SQ1[2] |  SQ2[2] | SQ3[2] ;
         2'b11 : mux_out[8] <= 1'b0;
      endcase
   end

      always @ (SQ5[2] or SQ6[2] or SQ7[2] or SQ4[2] or sel) begin
      case (sel)
         2'b00 : mux_out[9] <= SQ5[2] ^ SQ6[2] ^ SQ7[2] ^ SQ4[2] ;
         2'b01 : mux_out[9] <= SQ5[2] & SQ6[2] & SQ7[2] & SQ4[2] ;
         2'b10 : mux_out[9] <= SQ5[2] | SQ6[2] |  SQ7[2] | SQ4[2] ;
         2'b11 : mux_out[9] <= 1'b0;
      endcase
   end

      always @ (SQ10[2] or SQ11[2] or SQ9[2] or SQ8[2] or sel) begin
      case (sel)
         2'b00 : mux_out[10] <= SQ10[2] ^ SQ11[2] ^ SQ9[2] ^ SQ8[2] ;
         2'b01 : mux_out[10] <= SQ10[2] & SQ11[2] & SQ9[2] & SQ8[2] ;
         2'b10 : mux_out[10] <= SQ10[2] | SQ11[2] |  SQ9[2] | SQ8[2] ;
         2'b11 : mux_out[10] <= 1'b0;
      endcase
   end

      always @ (SQ14[2] or SQ15[2] or SQ12[2] or SQ13[2] or sel) begin
      case (sel)
         2'b00 : mux_out[11] <= SQ14[2] ^ SQ15[2] ^ SQ12[2] ^ SQ13[2] ;
         2'b01 : mux_out[11] <= SQ14[2] & SQ15[2] & SQ12[2] & SQ13[2] ;
         2'b10 : mux_out[11] <= SQ14[2] | SQ15[2] |  SQ12[2] | SQ13[2] ;
         2'b11 : mux_out[11] <= 1'b0;
      endcase
   end

   always @ (SQ0[3] or SQ1[3] or SQ2[3] or SQ3[3] or sel) begin
      case (sel)
         2'b00 : mux_out[12] <= SQ0[3] ^ SQ1[3] ^ SQ2[3] ^ SQ3[3] ;
         2'b01 : mux_out[12] <= SQ0[3] & SQ1[3] & SQ2[3] & SQ3[3] ;
         2'b10 : mux_out[12] <= SQ0[3] | SQ1[3] |  SQ2[3] | SQ3[3] ;
         2'b11 : mux_out[12] <= 1'b0;
      endcase
   end

      always @ (SQ5[3] or SQ6[3] or SQ7[3] or SQ4[3] or sel) begin
      case (sel)
         2'b00 : mux_out[13] <= SQ5[3] ^ SQ6[3] ^ SQ7[3] ^ SQ4[3] ;
         2'b01 : mux_out[13] <= SQ5[3] & SQ6[3] & SQ7[3] & SQ4[3] ;
         2'b10 : mux_out[13] <= SQ5[3] | SQ6[3] |  SQ7[3] | SQ4[3] ;
         2'b11 : mux_out[13] <= 1'b0;
      endcase
   end

   always @ (SQ10[3] or SQ11[3] or SQ9[3] or SQ8[3] or sel) begin
      case (sel)
         2'b00 : mux_out[14] <= SQ10[3] ^ SQ11[3] ^ SQ9[3] ^ SQ8[3] ;
         2'b01 : mux_out[14] <= SQ10[3] & SQ11[3] & SQ9[3] & SQ8[3] ;
         2'b10 : mux_out[14] <= SQ10[3] | SQ11[3] |  SQ9[3] | SQ8[3] ;
         2'b11 : mux_out[14] <= 1'b0;
      endcase
   end

      always @ (SQ14[3] or SQ15[3] or SQ12[3] or SQ13[3] or sel) begin
      case (sel)
         2'b00 : mux_out[15] <= SQ14[3] ^ SQ15[3] ^ SQ12[3] ^ SQ13[3] ;
         2'b01 : mux_out[15] <= SQ14[3] & SQ15[3] & SQ12[3] & SQ13[3] ;
         2'b10 : mux_out[15] <= SQ14[3] | SQ15[3] |  SQ12[3] | SQ13[3] ;
         2'b11 : mux_out[15] <= 1'b0;
      endcase
   end

always @ (SQ0[4] or SQ1[4] or SQ2[4] or SQ3[4] or sel) begin
      case (sel)
         2'b00 : mux_out[16] <= SQ0[4] ^ SQ1[4] ^ SQ2[4] ^ SQ3[4] ;
         2'b01 : mux_out[16] <= SQ0[4] & SQ1[4] & SQ2[4] & SQ3[4] ;
         2'b10 : mux_out[16] <= SQ0[4] | SQ1[4] |  SQ2[4] | SQ3[4] ;
         2'b11 : mux_out[16] <= 1'b0;
      endcase
   end

     always @ (SQ5[4] or SQ6[4] or SQ7[4] or SQ4[4] or sel) begin
      case (sel)
         2'b00 : mux_out[17] <= SQ5[4] ^ SQ6[4] ^ SQ7[4] ^ SQ4[4] ;
         2'b01 : mux_out[17] <= SQ5[4] & SQ6[4] & SQ7[4] & SQ4[4] ;
         2'b10 : mux_out[17] <= SQ5[4] | SQ6[4] |  SQ7[4] | SQ4[4] ;
         2'b11 : mux_out[17] <= 1'b0;
      endcase
   end

     always @ (SQ10[4] or SQ11[4] or SQ9[4] or SQ8[4] or sel) begin
      case (sel)
         2'b00 : mux_out[18] <= SQ10[4] ^ SQ11[4] ^ SQ9[4] ^ SQ8[4] ;
         2'b01 : mux_out[18] <= SQ10[4] & SQ11[4] & SQ9[4] & SQ8[4] ;
         2'b10 : mux_out[18] <= SQ10[4] | SQ11[4] |  SQ9[4] | SQ8[4] ;
         2'b11 : mux_out[18] <= 1'b0;
      endcase
   end


     always @ (SQ14[4] or SQ15[4] or SQ12[4] or SQ13[4] or sel) begin
      case (sel)
         2'b00 : mux_out[19] <= SQ14[4] ^ SQ15[4] ^ SQ12[4] ^ SQ13[4] ;
         2'b01 : mux_out[19] <= SQ14[4] & SQ15[4] & SQ12[4] & SQ13[4] ;
         2'b10 : mux_out[19] <= SQ14[4] | SQ15[4] |  SQ12[4] | SQ13[4] ;
         2'b11 : mux_out[19] <= 1'b0;
      endcase
   end
endmodule

//-----------------------------------------------------
// Design Name : T_ff_async_reset
// File Name   : T_ff_async_reset.v
// Function    : T flip-flop async reset
//-----------------------------------------------------
module T_ff (
CLK    , // Clock Input
CLR , // Reset input 
Q,
T         // Q output
) ;
//-----------Input Ports---------------
input  T, CLK, CLR ; 

//-----------Output Ports---------------
output Q;

//------------Internal Variables--------
wire D_wire;
reg Q;


//-------------Code Starts Here---------
always @ ( posedge CLK or negedge CLR)
if (~CLR) begin
  Q <= 1'b0;
end  else begin
  Q <= D_wire;
end

assign D_wire = T ^ Q;

endmodule //

