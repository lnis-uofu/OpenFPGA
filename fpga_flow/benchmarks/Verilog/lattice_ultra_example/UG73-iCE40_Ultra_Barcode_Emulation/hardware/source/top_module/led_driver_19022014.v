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


module led_driver (
		input  sys_clk,
		input  rst_n,	 
		input  txn_start,
		input mobeam_start_stop,
		input led_polarity,
		input [7:0] bar_width,
		input [7:0] barcode_array,
		output drive_on,
		output byte_done,
		output bit_done,
		output oled	
		//output dynamic_clk);
   		 );
   
   //reg [15:0] 	       clk_count;
   
  /* 
   always @(posedge sys_clk or negedge rst_n) 
     begin
	if (~rst_n)
	  clk_count <= 16'b0;
	else
	  if (clk_count==16'd4)
	    clk_count<= 0;
	  else
	    clk_count <= clk_count + 1'b1;
	
     end */// always @ (posedge sys_clk or negedge rst_n)

 //  assign dynamic_clk = (clk_count==16'd4);
   

   reg oled,drive_on;
   reg oled_int;
   reg [15:0] bw_count;
   reg [7:0]  ba_reg = 8'b10011001;
   reg 	      reload_ba_reg;
   reg 	      driver_busy;
   
   wire [7:0] BW;
   reg [15:0] BWx10  /* synthesis syn_multstyle = logic */;
   reg [2:0]  bit_cnt;
   reg 	      byte_done_int;
   reg 		txn_start_d; 
 /////////////////TXN_Start edge detect/////////////////
    
   always   @(posedge sys_clk or negedge rst_n)
     begin
	if (~rst_n  || !mobeam_start_stop) begin
		txn_start_d<= 1'b0;
	end
	else
		txn_start_d<=txn_start;
		
	end	  
	assign txn_start_pos = txn_start & (~txn_start_d);
	

		
   
assign BW =bar_width;
//assign byte_done_mod= (&bit_cnt) & reload_ba_reg;	  
//assign byte_done_mod= (&bit_cnt) && (bw_count==1'b1);
assign byte_done_mod= (BWx10==16'd4)?(&bit_cnt) && (bw_count==1'b1):byte_done_int;
assign byte_done = byte_done_mod;
assign bit_done = reload_ba_reg;
  // assign BWx10 = BW * 10 - 1'b1;
   
   always   @(posedge sys_clk or negedge rst_n)
     begin
	if (~rst_n || !mobeam_start_stop) begin
	   ba_reg <= 8'd0;
	   BWx10<=16'd4;
	   byte_done_int<= 1'b0;
	   bit_cnt<= 3'd0;
	end
	else begin
	   if (BW==0)
	     BWx10<= 4;
	   else	  begin
	     BWx10 <= BW * 10 - 1'b1;
	//	 BWX5  <= BW * 5 - 1'b1; 		//for 25% duty cycle
	  //   BWX15 <= 
		end
		
		//if (txn_start_pos) 	begin 
		if (txn_start && bit_cnt==3'd0) 	begin
	   		ba_reg <=barcode_array;
		end
		if (reload_ba_reg) begin
	     //ba_reg <= {ba_reg[0],ba_reg[7:1]};	   //lsb first
		 ba_reg <= {ba_reg[6:0],ba_reg[7]};		//msb first
		 if (bit_cnt==3'd7)
			 byte_done_int<=1'b1;
		else
			byte_done_int<=1'b0;
		 
	     	if (&bit_cnt) begin
				//byte_done_int<=1'b1;
	       		bit_cnt<= 3'd0;
		 	end
	      else begin
		// byte_done_int<= 1'b0;
		 bit_cnt<= bit_cnt + 1'b1;
	      end 
		  
	   end 
	   else
		    byte_done_int<= 1'b0;
	end
     end
   
   always   @(posedge sys_clk or negedge rst_n)
     begin
	if (~rst_n  || !mobeam_start_stop)
	  begin
	     oled_int <= 1'b0;
	     bw_count <= 16'b0;
	     reload_ba_reg <= 1'b0;
	     driver_busy<= 1'b0;
	     
	  end      
	else
	  if (txn_start) begin
	  //   driver_busy<= 1'b1;
	     if (bw_count == BWx10)
	       begin
		  //oled<=ba_reg[0]; //lsb_first
		  oled_int<=ba_reg[7]; //msb first
		  bw_count<=16'b0;
		  reload_ba_reg <= 1'b1;
	       end
	     
	     else begin
		oled_int<= oled_int;
		bw_count<= bw_count + 1'b1;
		reload_ba_reg <= 1'b0;
	     end 
	 end
		else
		reload_ba_reg <= 1'b0;      
     end // always   @ (posedge sys_clk or negedge rst_n)
   
  always @(posedge sys_clk ) begin  
	if (~rst_n  || !mobeam_start_stop) begin
		oled<=0;
	 end	else begin
		if(led_polarity)begin
			oled  <= oled_int;
			drive_on<=oled_int;
		end else begin
			oled <= ~oled_int;
			drive_on<=~oled_int;
		end
	  end	
		  
	end
	  
endmodule // clk_gen

