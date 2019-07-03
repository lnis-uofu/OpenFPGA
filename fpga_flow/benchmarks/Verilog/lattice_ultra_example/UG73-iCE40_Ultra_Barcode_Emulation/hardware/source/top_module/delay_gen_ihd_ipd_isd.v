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


module delay_gen_ihd_ipd_isd(
	input clk,
	input rst,
	input start_stop,
	input [15:0] isd_val,
	input [15:0] ipd_val,
	input config_reg_done,
	input symbol_shift_done,
	input packet_shift_done, 
	input hop_shift_done,
	output reg isd_delay_en,
	output reg ipd_delay_en
);

reg [15:0] isd_val_temp;
reg [15:0] ipd_val_temp;
reg [15:0] isd_count; 
reg [15:0] ipd_count;																		    
wire [20:0] max_count;
wire [20:0] max_count_pkt /* synthesis syn_multstyle = logic */;
reg clk_count,clk_count_pkt;
reg count_done,count_done_pkt;

parameter const_fact=20;
assign max_count=isd_val_temp * const_fact;
assign max_count_pkt=ipd_val_temp * const_fact;

//wire count_enable,count_enable_pkt;
reg  count_enable,count_enable_pkt;
//wire config_done;
//assign config_done= !rst_n ? 0 : config_reg_done ? 1 : config_done;	 //orignal
//assign count_enable= rst ? 0 : count_done ? 0 : symbol_shift_done ? 1 : count_enable;	//orignal
//assign count_enable_pkt= rst ? 0 : count_done_pkt ? 0 : (packet_shift_done || hop_shift_done) ? 1 : count_enable_pkt;

//assign isd_delay_en=symbol_shift_done ? 1 : (isd_count==max_count-1) :   

///////**********modified 11/feb/2014*************** /////////
  always @(posedge clk or posedge rst) begin
	if(rst || !start_stop) begin
	count_enable=0;	
	end
	else begin
		if(count_done) 
			count_enable=0;
		else if(symbol_shift_done)
			count_enable=1; 
		else 
			count_enable=count_enable;
			
	end
end	

always @(posedge clk or posedge rst) begin
	if(rst || !start_stop) begin
	count_enable_pkt=0;	
	end
	else begin
		if(count_done_pkt) 
			count_enable_pkt=0;
		else if(packet_shift_done || hop_shift_done)
			count_enable_pkt=1; 
		else 
			count_enable_pkt=count_enable_pkt;		
	end
end	
	
/////////////////******************************//////////



  
///////////////////////symbol////////////////////////////////////
always @(posedge clk or posedge rst) begin
	if(rst || !start_stop) begin
	isd_count<=0;
	count_done<=0;
	isd_delay_en<=0;
	end
	else begin
	count_done<=0;
		if(count_enable) begin		
			if(isd_count==max_count) begin
			isd_count<=0;
			count_done<=1;
			isd_delay_en<=0;
			end 
			else begin
			isd_delay_en<=1;
			isd_count<=isd_count+1;
			end
		end
		else begin
		isd_delay_en<=0;
		isd_count<=0;
		end	
	end
end

always @(posedge clk or posedge rst) begin
	if(rst || !start_stop) begin
	isd_val_temp<=0;
	clk_count<=0;
	end
	else begin
		if(!config_reg_done) begin
		isd_val_temp<=isd_val;		   
		clk_count<=0;
		end
		else if(config_reg_done) begin
		clk_count<=clk_count+1;
		isd_val_temp<=isd_val;
		end
		else begin
		isd_val_temp<=isd_val_temp;
		end
	end
end
///////////////packet//////////////////////
always @(posedge clk or posedge rst) begin
	if(rst || !start_stop) begin
	ipd_count<=0;
	count_done_pkt<=0;	
	ipd_delay_en<=0;
	end
	else begin
	count_done_pkt<=0;
		if(count_enable_pkt) begin		
			if(ipd_count==max_count_pkt) begin
			ipd_count<=0;
			count_done_pkt<=1;
			ipd_delay_en<=0;
			end 
			else begin
			ipd_delay_en<=1;
			ipd_count<=ipd_count+1;
			end
		end
		else begin
		ipd_delay_en<=0;
		ipd_count<=0;
		end	
	end
end

always @(posedge clk or posedge rst) begin
	if(rst || !start_stop) begin
	ipd_val_temp<=0;
	clk_count_pkt<=0;
	end
else begin	
		if(!config_reg_done)	begin
		ipd_val_temp<=ipd_val;			 
		clk_count_pkt<=0;
		end		
		else if(config_reg_done) begin
		clk_count_pkt<=clk_count_pkt+1;
		ipd_val_temp<=ipd_val;
		end
		else begin // if(!config_reg_done)	begin
		ipd_val_temp<=ipd_val_temp;
		end
	end
end

endmodule
