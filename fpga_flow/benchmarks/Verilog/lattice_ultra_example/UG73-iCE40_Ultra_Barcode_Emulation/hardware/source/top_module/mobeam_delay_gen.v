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


module mobeam_delay_gen(
	input clk,
	input rst_n,
	input [15:0] isd_val,
	input [15:0] ipd_val,
	input config_reg_done,
	input symbol_shift_done,
	input packet_shift_done,
	output reg isd_delay_en,
	output reg ipd_delay_en
);

reg [15:0] isd_val_temp;
reg [15:0] ipd_val_temp;
reg [15:0] isd_count; 
reg [15:0] ipd_count;
wire [15:0] max_count,max_count_pkt;
reg clk_count,clk_count_pkt;
reg count_done,count_done_pkt;

parameter const=10;
assign max_count=isd_val_temp * const;
assign max_count_pkt=ipd_val_temp * const;

wire count_enable,count_enable_pkt;
//wire config_done;
//assign config_done= !rst_n ? 0 : config_reg_done ? 1 : config_done;
assign count_enable= !rst_n ? 0 : count_done ? 0 : symbol_shift_done ? 1 : count_enable;
assign count_enable_pkt= !rst_n ? 0 : count_done_pkt ? 0 : packet_shift_done ? 1 : count_enable_pkt;

//assign isd_delay_en=symbol_shift_done ? 1 : (isd_count==max_count-1) :
  
///////////////////////symbol////////////////////////////////////
always @(posedge clk or negedge rst_n) begin
	if(!rst_n) begin
	isd_count<=0;
	count_done<=0;
	end
	else begin
	count_done<=0;
		if(count_enable) begin		
			if(isd_count==max_count-1) begin
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

always @(posedge clk or negedge rst_n) begin
	if(!rst_n) begin
	isd_val_temp<=0;
	clk_count<=0;
	end
	else begin
		if(clk_count)
		isd_val_temp<=isd_val_temp;
		else if(config_reg_done) begin
		clk_count<=clk_count+1;
		isd_val_temp<=isd_val;
		end
		else 
		isd_val_temp<=isd_val_temp;
	end
end
///////////////packet//////////////////////
always @(posedge clk or negedge rst_n) begin
	if(!rst_n) begin
	ipd_count<=0;
	count_done_pkt<=0;
	end
	else begin
	count_done_pkt<=0;
		if(count_enable_pkt) begin		
			if(ipd_count==max_count_pkt-1) begin
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

always @(posedge clk or negedge rst_n) begin
	if(!rst_n) begin
	ipd_val_temp<=0;
	clk_count_pkt<=0;
	end
	else begin
		if(clk_count_pkt)
		ipd_val_temp<=ipd_val_temp;
		else if(config_reg_done) begin
		clk_count_pkt<=clk_count_pkt+1;
		ipd_val_temp<=ipd_val;
		end
		else 
		ipd_val_temp<=ipd_val_temp;
	end
end

endmodule
