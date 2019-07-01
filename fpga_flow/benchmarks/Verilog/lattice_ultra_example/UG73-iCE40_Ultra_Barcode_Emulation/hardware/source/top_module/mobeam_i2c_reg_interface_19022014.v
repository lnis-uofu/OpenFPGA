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


module mobeam_i2c_reg_interface(
		rst_i,
		clk_i,
		//i2c slave interface signals///	
		i2c_master_to_slave_data_i,
		i2c_slave_to_master_data_o,
		i2c_slave_data_address_i,
		wr_en_i,
		rd_en_i,
		i2c_start_i,
		//MoBeam control logic interface//
		rd_revision_code,
		rst_mobeam,
		mobeam_start_stop,
		led_polarity,
		o_ba_mem_data,
		i_ba_mem_addr,
		i_ba_mem_rd_en,
		i_ba_mem_rd_clk,
		o_bsr_mem_data,
		i_bsr_mem_addr,
		i_bsr_mem_rd_en,
		i_bsr_mem_rd_clk
	//	i_config_reg_done,
	//	o_new_data_rd,
	//	o_data_strobe
		);
//i2c slave interface signals///		
input [7:0] i2c_master_to_slave_data_i;
output reg [7:0]i2c_slave_to_master_data_o;
input [7:0] i2c_slave_data_address_i;
input  wr_en_i;
input  rd_en_i;
input  i2c_start_i;
input  rst_i;
input  clk_i;
//mobeam control logic interface//
output  [15:0] o_ba_mem_data;
input [7:0] i_ba_mem_addr;
input i_ba_mem_rd_en;
input i_ba_mem_rd_clk;
output reg rd_revision_code=0;
output reg	rst_mobeam=0;
//input[7:0] i_revision_code_data;
//input rd_revision_code;
output  [7:0] o_bsr_mem_data;
//output reg [15:0] mem_data_buf16;
input [8:0] i_bsr_mem_addr;
input i_bsr_mem_rd_en;
input i_bsr_mem_rd_clk;
output reg mobeam_start_stop=1'b0;
output reg led_polarity=1'b0;
//output  reg o_new_data_rd=1'b0;
//output  reg o_data_strobe=1'b0;
//input i_config_reg_done;








//////////////wire & reg declarations//////////////////
wire wr_en;
wire rd_en;
reg  d1_wr_en_i;
reg  d2_wr_en_i;
reg  d1_rd_en_i;
reg  d2_rd_en_i;  

parameter revision_code=8'hF1;




/////////////////////BSR and CONFIG data memory//////////////

SB_RAM512x8 ram512x8_inst 
(
	.RDATA(o_bsr_mem_data),// EBR512x8_data
	.RADDR(i_bsr_mem_addr),// EBR512x8_addr
	.RCLK(i_bsr_mem_rd_clk),
	.RCLKE(i_bsr_mem_rd_en),
	.RE(i_bsr_mem_rd_en),// EBR512x8_re
	.WADDR({1'b0,bsr_mem_addr_in}),
	.WCLK(clk_i),
	.WCLKE(1'b1),///*bsr_wr_en/*wr_en*/),
	.WDATA(i2c_master_to_slave_data_i),
	.WE(bsr_wr_en)  ////*bsr_wr_en/*wr_en*/)
);

defparam ram512x8_inst.INIT_0 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_1 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_2 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_3 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_4 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_5 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
//ICE Technology Library 66
//Lattice Semiconductor Corporation Confidential
defparam ram512x8_inst.INIT_6 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_7 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_8 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_9 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_A = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_B = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_C = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_D = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_E = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram512x8_inst.INIT_F = 256'h0000000000000000000000000000000000000000000000000000000000000000;


///////////////////////BA i.e. beamable data memory//////////////
SB_RAM256x16 ram256x16_inst (
.RDATA(o_ba_mem_data),
.RADDR(i_ba_mem_addr),
.RCLK(i_ba_mem_rd_clk),
.RCLKE(i_ba_mem_rd_en),
.RE(i_ba_mem_rd_en),
.WADDR(ba_mem_addr_in_delayed_1),
.WCLK(clk_i),
.WCLKE(1'b1),///*ba_wr_en/*wr_en*/),
.WDATA(ba_mem_data_buffer),
.WE(ba_wr_en_delayed),///*wr_en*/),
.MASK(16'd0)
);
defparam ram256x16_inst.INIT_0 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_1 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_2 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_3 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_4 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_5 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_6 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_7 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_8 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_9 = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_A = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_B = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_C = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_D = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_E = 256'h0000000000000000000000000000000000000000000000000000000000000000;
defparam ram256x16_inst.INIT_F = 256'h0000000000000000000000000000000000000000000000000000000000000000;


// Write valid pulse
always @(posedge clk_i or posedge rst_i) begin
	if (rst_i) begin
		d1_wr_en_i <= 0;
		d2_wr_en_i <= 0;
	end else begin
		d1_wr_en_i <= wr_en_i;
		d2_wr_en_i <= d1_wr_en_i;
	end
end

assign wr_en = d2_wr_en_i && ~d1_wr_en_i;

// Read enable pulse
always @(posedge clk_i or posedge rst_i) begin
	if (rst_i) begin
		d1_rd_en_i <= 0;
		d2_rd_en_i <= 0;
	end else begin
		d1_rd_en_i <= rd_en_i;
		d2_rd_en_i <= d1_rd_en_i;
	end
end


assign rd_en = ~d2_rd_en_i && d1_rd_en_i;

reg  [7:0] bsr_mem_addr_in;

reg  [7:0] ba_mem_addr_in;
reg 		ba_wr_en;


always @(posedge clk_i or posedge rst_i) begin
	if (rst_i) begin
		bsr_mem_addr_in <= 9'd0;

	end
else  
	if (i2c_start_i)
		bsr_mem_addr_in <= i2c_slave_data_address_i;
		
	 else if (i2c_slave_data_address_i == 8'h80) begin  
		if (wr_en) begin
		bsr_mem_addr_in <=bsr_mem_addr_in + 1'b1;

		end

	end
end

assign bsr_wr_en = 	(i2c_slave_data_address_i == 8'h00)?1'b0:wr_en;

//MOBEAM START-STOP
always @(posedge clk_i or posedge rst_i) begin
	if (rst_i) begin
		mobeam_start_stop<=1'b0;

	end
else  
	if (i2c_slave_data_address_i == 8'hF0)	  //8'hEC
		if (wr_en & (i2c_master_to_slave_data_i==8'b00000001))begin
		mobeam_start_stop<=1'b1;  
		end 
		else if(wr_en & (i2c_master_to_slave_data_i==8'b00000000))begin
		mobeam_start_stop<=1'b0;
		end

end	  


//MOBEAM RESET
always @(posedge clk_i or posedge rst_i) begin
	if (rst_i) begin
		rst_mobeam<=1'b0;

	end
else  
	if (i2c_slave_data_address_i == 8'hF1)  
		if (wr_en & (i2c_master_to_slave_data_i==8'b00000001))begin
		rst_mobeam<=1'b1;  
		end 
		else if(wr_en & (i2c_master_to_slave_data_i==8'b00000000))begin
		rst_mobeam<=1'b0;
		end

end	 

//REVISION CODE
always @(posedge clk_i or posedge rst_i) begin
	if (rst_i) begin
		rd_revision_code<=1'b0;

	end
else  	
	if (rd_en)begin
			if (i2c_slave_data_address_i == 8'hF2) begin	 
			rd_revision_code<=1'b1;
			i2c_slave_to_master_data_o<=revision_code;
			end 
			else begin
			rd_revision_code<=1'b0;
			i2c_slave_to_master_data_o<=0;
			end	   
	end
	else 
	i2c_slave_to_master_data_o<=0;

end	  

//LED POLARITY
always @(posedge clk_i or posedge rst_i) begin
	if (rst_i) begin
		led_polarity<=1'b0;

	end else  begin
		//if (wr_en)begin
			//if (i2c_slave_data_address_i == 8'hEB)begin  
				//if (i2c_master_to_slave_data_i==8'b00000001)begin
					led_polarity<=1'b1;  
				//end else begin
					//led_polarity<=1'b0;
				//end
			//end
		//end 
	end
end

reg ba_wr_en_delayed;
reg wr_en_cnt;
reg [7:0] ba_mem_addr_in_delayed,ba_mem_addr_in_delayed_1;
reg [7:0] data_buffer;
reg [15:0] ba_mem_data_buffer; 


always @(posedge clk_i or posedge rst_i) begin
	if (rst_i) begin
	 ba_mem_addr_in_delayed<=8'd0; 
	 ba_mem_addr_in_delayed_1<=8'd0;
	 ba_wr_en_delayed<= 1'b0;
	 
	end
else begin
	 ba_mem_addr_in_delayed<= ba_mem_addr_in;
	 ba_mem_addr_in_delayed_1<= ba_mem_addr_in_delayed;
	 ba_wr_en_delayed<= ba_wr_en;
		
end
end





//assign ba_mem_data_buffer = { i2c_master_to_slave_data_i,data_buffer}; 
always @(posedge clk_i or posedge rst_i) begin
	if (rst_i) begin
		ba_mem_addr_in <= 9'd0;
		ba_wr_en<= 1'b0;
		wr_en_cnt<= 1'b0;
	end
else 

	 if (i2c_slave_data_address_i == 8'h00) begin
		  
		 if (wr_en_cnt)	begin
			ba_mem_data_buffer <= { i2c_master_to_slave_data_i,data_buffer};
		end
			
		
		if (wr_en) begin
			wr_en_cnt<= wr_en_cnt + 1'b1;
			ba_wr_en<= 1'b1;
				if (~wr_en_cnt)begin
				data_buffer <=  i2c_master_to_slave_data_i;
				end	 
				else
				ba_mem_addr_in <= ba_mem_addr_in + 1'b1;
		end
		else
			ba_wr_en<= 1'b0;
		
	end	
	else  begin
	ba_mem_addr_in<=0;
	ba_wr_en<= 1'b0;
	wr_en_cnt<=0;
	end
	
end		
endmodule