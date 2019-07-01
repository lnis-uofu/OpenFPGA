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

	
module mobeam_control_fsm(
	//`ifndef SIM
	input shift_done,
	input bit_done,
	// `endif
	input sys_clk_i,
	input rst_mobeam,
//	input new_data_rd,
//	input data_strobe,
	input start_stop,
	 
//	output reg config_reg_done,
//	output reg write_to_bsr_buffer,
	
	//bsr memory signals
	input  [7:0]  bsr_mem_data,
	output 		  bsr_mem_clk, 	
	output reg [8:0]  bsr_mem_addr, 
	output  reg      bsr_mem_rd_en,
	
	//ba memory signals
	input  [15:0] 	  ba_mem_data ,
	output 		  	  ba_mem_clk, 	
	output reg [7:0]  ba_mem_addr,
	output reg    	  ba_mem_rd_en,
	
	//TO LED DRIVER	
	output [7:0] 	o_byte_data,
	output			txn_start,
//	output	reg		bsr_load_done,
	output	reg [7:0]   bsr_bw	
);

`define 	IDLE_BSR 				0
`define 	LB_READ					1
`define 	LB_READ_LATENCY			2
`define 	LP_READ					3	
`define 	LP_READ_LATENCY			4	
`define		BL_READ					5
`define 	BL_READ_LATENCY			6
`define 	NH_READ 				7
`define 	NH_READ_LATENCY			8
`define 	NR_READ 				9
`define 	NR_READ_LATENCY			10

`define 	BSR_READ 				11
`define 	BSR_READ_LATENCY		12
`define		START_HOP				13 
//`define 	BEAM_START_STOP 		
//`define		BEAM_START_STOP_LATENCY 10
	
`define		IDLE_STATE				14
`define		READ_WORD				15
`define		READ_BYTE_LATENCY_1		16
`define		DELAY_STATE				17
`define		SHIFT_WORD				18



// ==============================================================================
// state assignments & defines
// ==============================================================================
parameter 	BSR_ADDR_WIDTH	=		3	;
parameter	BSR_MEM_DEPTH	=	1 << BSR_ADDR_WIDTH ;
///mobeam reg addresses////////////
//parameter revision_code_addr=8'hEE; //EE
parameter rst_mobeam_addr=8'hF1;	//ED
parameter lb_addr=8'hEA;			//F0
parameter lp_addr=8'hEB;			//EF
parameter bl_addr=8'hEC;			//E9
parameter nh_addr=8'hED;			//EA
parameter nr_addr=8'hEE;			//EB
parameter bsr_addr=8'h80;
parameter ba_addr=8'h00;	 
parameter beam_start_stop_addr=8'hF0; //EC



integer bsr_data_count;
reg rd_done;
reg i,i_2;	   

reg [7:0] o_lb_data;
reg [7:0] o_lp_data;
reg [7:0] o_nh_data;
reg [7:0] o_nr_data;
reg [7:0] o_bl_data;
reg [7:0] o_bsr_data; 
reg [4:0] ba_state;
reg [4:0] bsr_state; 
reg [7:0] bsr_buffer [0:6];
reg config_reg_done;
//reg write_to_bsr_buffer;   


//reg start_stop; //from mobeam reg set
//wire shift_done; //from led control logic 

	


//////////////////////////BA FSM////////////////////////////////////////////////	

	assign ba_mem_clk=sys_clk_i;
	reg [7:0] byte_data;
	wire bl_done; 
	reg [1:0]check_shift_done_count_d;

	always @(posedge sys_clk_i or posedge rst_mobeam) begin
			if(rst_mobeam || !start_stop) begin
				//check_clk_1=0;
				//check_clk_2=0;
				byte_data <= 8'd0;
				check_shift_done_count_d<=0;
			end	
	else begin
		check_shift_done_count_d<=check_shift_done_count;
		//	if(ba_state!=`IDLE_STATE) begin
		
			  if(check_shift_done_count_d==2'b01) begin 
					//if(check_clk_1==1) begin
					byte_data<=ba_mem_data[15:8];
				 	//check_clk_1=0;
					//end	
					//else
				 	//check_clk_1=1;	
				end
				else if(check_shift_done_count_d==2'b00) begin
					//if(check_clk_2==1) begin
					byte_data<=ba_mem_data[7:0];
				 	//check_clk_2=0;
					//end	
					//else
				 	//check_clk_2=1;			
				end		
				else 
				byte_data<=byte_data;			
			end
	end
	
	
	
	
	////*****count shift_done's***************////
	
	reg [1:0] check_shift_done_count;
	reg clk_count_2,clk_count_3;
	
			always @(posedge sys_clk_i or posedge rst_mobeam) begin
			if(rst_mobeam || !start_stop)  begin
			clk_count_2<=0;
			clk_count_3<=0;
			check_shift_done_count<=2'b00;
			end
			else begin
			if(/*tx_out_en_high_dtc*/symbol_shift_done)
			check_shift_done_count<=0;
			else if(shift_done) begin
			//	if(clk_count_2==1) begin
			//	clk_count_2<=0;
				check_shift_done_count<=check_shift_done_count + 1;
			//	end
			//	else begin
			//	check_shift_done_count<=check_shift_done_count;
			//	end
		//	clk_count_2<=clk_count_2 + 1;			
			end
			else if(check_shift_done_count==2'b10) begin
		//		if(clk_count_3==1) begin
				check_shift_done_count<=0;
		//		clk_count_3<=0;
		//		end
		//		else begin
		//		check_shift_done_count<=check_shift_done_count;
		//		end
		//	clk_count_3<=clk_count_3+1;
			end
			else
			check_shift_done_count<=check_shift_done_count;
			end			
		end	
		
		/*
		always @(posedge sys_clk_i or negedge rst_mobeam_n) begin
			if(!rst_mobeam_n)  begin
			clk_count_2<=0;
			clk_count_3<=0;
			check_shift_done_count<=2'b00;
			end
			else begin
			
			if(shift_done) begin
				if(clk_count_2==1) begin
				clk_count_2<=0;
				check_shift_done_count<=check_shift_done_count + 1;
				end
				else begin
				check_shift_done_count<=check_shift_done_count;
				end
			clk_count_2<=clk_count_2 + 1;			
			end
			else if(check_shift_done_count==2'b10) begin
				if(clk_count_3==1) begin
				check_shift_done_count<=0;
				clk_count_3<=0;
				end
				else begin
				check_shift_done_count<=check_shift_done_count;
				end
			clk_count_3<=clk_count_3+1;
			end
			else
			check_shift_done_count<=check_shift_done_count;
			end			
		end	
			*/
	//assign check_shift_done_count= (!rst_mobeam_n) ? 3'b000 : shift_done ? (check_shift_done_count + 1) : check_shift_done_count;
	
	wire incr_ba_mem_addr;
	reg q_tx_out_en_0,q_tx_out_en_1;
	assign incr_ba_mem_addr=(check_shift_done_count==2'b10) ? 1 : 0; 
	//assign bl_done=(bl_byte_count_3==o_bl_data + 1) ? 1 : 0;
	//assign o_byte_data=((bl_byte_count_3 >0) && !bl_done) ? byte_data : 8'bz;
	//assign o_byte_data= ba_mem_rd_en ? (/*(isd_delay_en || ipd_delay_en)*/ !tx_out_en ? 8'bz : byte_data) : 0;
	
	assign o_byte_data= byte_data;
	assign txn_start = q_tx_out_en_1;
	
	
	//assign txn_start = tx_out_en;		//orignal
	
	always @(posedge sys_clk_i or posedge rst_mobeam) begin
			if(rst_mobeam || !start_stop)  begin
		q_tx_out_en_0<=0;
		q_tx_out_en_1<=0;
		end
		else begin
		q_tx_out_en_0<=tx_out_en;
		q_tx_out_en_1<=q_tx_out_en_0;
		end
	end	
	wire tx_out_en_high_dtc;
	
	assign tx_out_en_high_dtc= ~q_tx_out_en_0 && tx_out_en;
		
//////////////BA FSM/////////////////////////////////////////////
	always @(posedge sys_clk_i or posedge rst_mobeam) begin
			if(rst_mobeam)  begin
			ba_mem_addr<=0;
			ba_mem_rd_en<=0;
			ba_state<=`IDLE_STATE;				
			end
		else begin
		case(ba_state)
				`IDLE_STATE: begin
				ba_mem_addr<=0;
				ba_mem_rd_en<=0;
				if(/*start_stop*/ config_reg_done)
					ba_state<=`READ_WORD;
				else
					ba_state<=`IDLE_STATE;					
				end
				
				`READ_WORD:begin
				ba_mem_rd_en<=1;
				if(!start_stop)
					ba_state<=`IDLE_STATE;		
				else if(symbol_shift_done)   //make address=0 when #shift_done's == o_bl_data
 					ba_mem_addr<=0;
				else if(!tx_out_en)
					ba_state<=`DELAY_STATE;
				else if(incr_ba_mem_addr) begin
					ba_mem_addr<=ba_mem_addr + 1;
					ba_state<=`READ_BYTE_LATENCY_1;
				end				
				else begin
					ba_mem_addr<=ba_mem_addr;
					ba_state<=`READ_WORD;
				end				
				end
				
				`READ_BYTE_LATENCY_1:begin
				if(!start_stop)
					ba_state<=`IDLE_STATE;
				else	
					ba_state<=`READ_WORD;					
				end
				
				`DELAY_STATE:begin
				if(!start_stop)
					ba_state<=`IDLE_STATE;
				else begin
					ba_mem_addr<=ba_mem_addr;
					if(tx_out_en)
					ba_state<=`READ_WORD;
					else 
					ba_state<=`DELAY_STATE;
				end		
				end
	
			endcase	 	
		end //else begin
	end //else always
/////////////////////////////////////////////////////////////////	
	reg start_stop_count;

 	always @(posedge sys_clk_i or posedge rst_mobeam) begin
		 if(rst_mobeam) 		  begin
			start_stop_count=0; 
		 end
	 	else begin
			if(start_stop)
				start_stop_count=1;
		 end
	 end
		 

//////////////////BSR FSM///////////////////////////////////////



assign bsr_mem_clk=sys_clk_i;
reg state_count;
reg init_addr_read_done;
reg [8:0] prev_mem_rd_addr;
reg [8:0] next_mem_rd_addr;
 
	always @(posedge sys_clk_i or posedge rst_mobeam) begin
			if(rst_mobeam) 		  begin
			bsr_state<=`IDLE_BSR;	   
				i<=0;
						i_2<=0;
						bsr_data_count<=0;						
					//	write_to_bsr_buffer<=0;
						state_count<=0;
						config_reg_done<=0;
						next_mem_rd_addr<=0;
						bsr_mem_addr<=0;  
						prev_mem_rd_addr<=0;
						bsr_mem_rd_en<=1'b1;
					//	bsr_load_done<=0;
						init_addr_read_done<=0;
						rd_done<=1'b0;
						o_lb_data<=0;
						o_lp_data<=0;
						o_nh_data<=0;
						o_nr_data<=0;
						o_bl_data<=0;
						o_bsr_data<=0; 
			end
		else begin
		
		case(bsr_state)
					`IDLE_BSR:begin
						i<=0;
						i_2<=0;
						bsr_data_count<=0;						
					//	write_to_bsr_buffer<=0;
						state_count<=0;
						config_reg_done<=0;
						next_mem_rd_addr<=0;
						bsr_mem_addr<=0;  
						prev_mem_rd_addr<=0;
						bsr_mem_rd_en<=1'b1;
					//	bsr_load_done<=0;
						init_addr_read_done<=0;
						rd_done<=1'b0;
						if (start_stop==1)
							bsr_state<=`LB_READ;
						else 
							bsr_state<=`IDLE_BSR;
					end
					
						
					`LB_READ:begin
						bsr_mem_addr<=lb_addr;
						bsr_mem_rd_en<=1'b1;
						bsr_state<=`LB_READ_LATENCY;
					end	
					
					`LB_READ_LATENCY:begin
						state_count<=state_count + 1;
						if(state_count==1) begin
						o_lb_data<=bsr_mem_data;
						state_count<=0;
						bsr_state<=`LP_READ;
						end
						else
						bsr_state<=`LB_READ_LATENCY;
					end
					
					`LP_READ:begin
						bsr_mem_addr<=lp_addr;
						bsr_mem_rd_en<=1'b1;
						bsr_state<=`LP_READ_LATENCY;
					end	
					
					`LP_READ_LATENCY:begin
						state_count<=state_count + 1;
						if(state_count==1) begin
						o_lp_data<=bsr_mem_data;
						state_count<=0;
						bsr_state<=`BL_READ;
						end
						else
						bsr_state<=`LP_READ_LATENCY;
					end
					
					`BL_READ:begin
						bsr_mem_addr<=bl_addr;
						bsr_mem_rd_en<=1'b1;
						bsr_state<=`BL_READ_LATENCY;
						
					end
					
					`BL_READ_LATENCY:begin
						state_count<=state_count + 1;
						if(state_count==1) begin
						o_bl_data<=bsr_mem_data;
						state_count<=0;
						bsr_state<=`NH_READ;
						end
						else
						bsr_state<=`BL_READ_LATENCY;						
					end
					
					`NH_READ:begin
						bsr_mem_addr<=nh_addr;
						bsr_mem_rd_en<=1'b1;
						bsr_state<=`NH_READ_LATENCY;
					end
					
					`NH_READ_LATENCY:begin
						state_count<=state_count + 1;
						if(state_count==1) begin
						o_nh_data<=bsr_mem_data;
						state_count<=0;
						bsr_state<=`NR_READ;
						end
						else
						bsr_state<=`NH_READ_LATENCY;
					end

					`NR_READ:begin
						bsr_mem_addr<=nr_addr;
						bsr_mem_rd_en<=1'b1;
						rd_done<=1'b1;
						bsr_state<=`NR_READ_LATENCY;										
					end
					
					`NR_READ_LATENCY:begin
						state_count<=state_count + 1;
						if(state_count==1) begin
						o_nr_data<=bsr_mem_data;
						state_count<=0;
						bsr_state<=`BSR_READ;
						end
						else
						bsr_state<=`NR_READ_LATENCY;						
					end
					
					
					`BSR_READ:begin	
						if (i==0) begin
							i<=1'b1;
							bsr_data_count<=0;
							
							if(init_addr_read_done==0 || nh_hop_shift_done_2)
							bsr_mem_addr<=bsr_addr;	
							else if(hop_shift_done_3) begin //if shift_count==np*ns then next 8 bytes else 
							//config_reg_done<=0;
							bsr_mem_addr<=next_mem_rd_addr+1;
							end
							else 
							bsr_mem_addr<=prev_mem_rd_addr;
						
						bsr_mem_rd_en<=1'b1;
						bsr_state<=`BSR_READ_LATENCY; 
						end
						else begin
						bsr_mem_addr<=bsr_mem_addr+1;
						bsr_data_count<=bsr_data_count+1;
						if (bsr_data_count==6) begin                 //read all 8 bytes of bsr data,increament hop_count
							//	bsr_data_count<=0;
								next_mem_rd_addr<=bsr_mem_addr;
							//	write_to_bsr_buffer<=0;
							//	bsr_load_done<=1;
								config_reg_done<=1;
								//bsr_state<=`BEAM_START_STOP;
								bsr_state<=`START_HOP;
						end	
						else begin
								config_reg_done<=config_reg_done;
								bsr_state<=`BSR_READ_LATENCY;
						end
						end					
					end		
					
					`BSR_READ_LATENCY:begin
							if(i_2==1'b0) begin           /*store first address in temp register*/
							i_2<=1'b1;
							prev_mem_rd_addr<=bsr_mem_addr;
							end							
							state_count<=state_count + 1;  
						//	write_to_bsr_buffer<=1;
							if(state_count==1) begin
							o_bsr_data<=bsr_mem_data;
							bsr_buffer[bsr_data_count]<=bsr_mem_data;
							state_count<=0;
							bsr_state<=`BSR_READ;
							end
							else   begin
							bsr_state<=`BSR_READ_LATENCY;
						//	write_to_bsr_buffer<=0;
							end
					end
					
					`START_HOP: begin
							i<=1'b0;
							i_2<=1'b0;
						//	bsr_load_done<=0;
							init_addr_read_done<=1;
							if(!start_stop && start_stop_count)
							bsr_state<=`IDLE_BSR;
							else if(hop_shift_done)
							bsr_state<=`BSR_READ;
							else
							bsr_state<=`START_HOP;
					end
					
				
	//				`BEAM_START_STOP: begin
	//							i<=1'b0;
	//							i_2<=1'b0;
	//							init_addr_read_done<=1;
	//							bsr_load_done<=0;
	//							rd_done<=1'b0;
	//							bsr_mem_addr<=beam_start_stop_addr;
	//							bsr_mem_rd_en<=1'b1;
	//							if(hop_shift_done/*symbol_shift_done*/)
	//							bsr_state<=`BSR_READ;
	//							else
	//							bsr_state<=`BEAM_START_STOP_LATENCY;
	//				end 
					
	//				`BEAM_START_STOP_LATENCY:begin
	//				state_count<=state_count + 1;
	//					if(state_count==1) begin
	//						state_count<=0;
	//							if (bsr_mem_data==8'b00000001) begin
	//								start_stop<=1'b1;
	//									if(hop_shift_done/*symbol_shift_done*/)
	//									bsr_state<=`BSR_READ;
	//									else
	//									bsr_state<=`BEAM_START_STOP;
	//							end
	//							else begin
	//								start_stop<=1'b0;
	//								bsr_state<=`IDLE_BSR;
	//							end	
	//						end	
	//					else
	//					bsr_state<=`BEAM_START_STOP_LATENCY;								
	//				end
						
			endcase

		end
	end
	
	/////*********edge detection for config_reg_done***********////////////
		reg  q_config_reg_done;
		always @(posedge sys_clk_i or posedge rst_mobeam) begin
			if(rst_mobeam) 
				q_config_reg_done<=0;
			else
				q_config_reg_done<=config_reg_done;
			end
			
			assign config_reg_done_pos_dtc= config_reg_done && (~q_config_reg_done);
		
	
////**************LOGIC TO FIND PACKET AND SYMBOL COUNT*************/////////
	reg [7:0] packet_count;
	reg [3:0] hop_count;
	reg [7:0] symbol_count;
	reg [7:0] shift_done_count;
	reg symbol_shift_done,symbol_shift_done_2,symbol_shift_done_3;
	reg packet_shift_done;
	reg hop_shift_done,hop_shift_done_2,hop_shift_done_3;
	reg clk_count,clk_count_4;
	reg nh_hop_shift_done,nh_hop_shift_done_2,nh_hop_shift_done_3;
	
	assign nh_hop_shift_done_led=nh_hop_shift_done_3;
													
	
	always @(posedge sys_clk_i or posedge rst_mobeam) begin
			if(rst_mobeam)  begin
			packet_count=0;
			hop_count=0;
			hop_shift_done=0;
			shift_done_count=0;
			symbol_count=0;
			clk_count<=0;
			clk_count_4<=0;
			hop_shift_done_2<=0;
			hop_shift_done_3<=0;
			nh_hop_shift_done<=0;
			nh_hop_shift_done_2<=0;
			nh_hop_shift_done_3<=0;
			symbol_shift_done<=0;
			symbol_shift_done_2<=0;
			symbol_shift_done_3<=0;
			packet_shift_done=0;
			end
		else begin
		if(start_stop) begin
		hop_shift_done=0;
		nh_hop_shift_done<=0;
		symbol_shift_done<=0;
		packet_shift_done=0;
			if(shift_done) begin
				//clk_count<=clk_count+1;
				//if(clk_count==1'b1)	begin				//orignal
				//clk_count<=0;
				if(shift_done_count==(o_bl_data-1)) begin	//bl check
					shift_done_count=0;	
					symbol_shift_done<=1;
					if(symbol_count==(bsr_ns-1)) begin			//symbol_check
						symbol_count=0;
						packet_shift_done=1;
						if(packet_count==(bsr_np-1)) begin          //packet check
						packet_count=0;	 
						hop_shift_done=1;
							if(hop_count==(o_nh_data-1)) begin			//hop_check
							hop_count=0;
							nh_hop_shift_done<=1;
							end
							else begin
							hop_count=hop_count + 1;					//hop_check	
							end	
						end
						else begin									//packet check				
						packet_count=packet_count+1; 
						hop_shift_done=0;
						end
					end
					else begin				
						symbol_count=symbol_count+1;					
					end			
				end	
				else
				shift_done_count=shift_done_count+1;	   //bl check			
				end	
			else
			//shift_done_count=shift_done_count+1;   //orignal
 			shift_done_count=shift_done_count;	
			//end	 									//orignal
			
				nh_hop_shift_done_2<=nh_hop_shift_done;
				nh_hop_shift_done_3<=nh_hop_shift_done_2;
				hop_shift_done_2<=hop_shift_done;
				hop_shift_done_3<=hop_shift_done_2;
				symbol_shift_done_2<=symbol_shift_done;
				symbol_shift_done_3<=symbol_shift_done_2;
			end
			else begin 
			packet_count=0;
			hop_count=0;
			hop_shift_done=0;
			shift_done_count=0;
			symbol_count=0;
			clk_count<=0;
			clk_count_4<=0;
			hop_shift_done_2<=0;
			hop_shift_done_3<=0;
			nh_hop_shift_done<=0;
			nh_hop_shift_done_2<=0;
			nh_hop_shift_done_3<=0;
			symbol_shift_done<=0;
			symbol_shift_done_2<=0;
			symbol_shift_done_3<=0;
			packet_shift_done=0;
			end	
		end	 
end	
	/*
	
	always @(posedge sys_clk_i or negedge rst_mobeam_n) begin
		if(!rst_mobeam_n)  begin
			packet_count=0;
			hop_count=0;
			hop_shift_done=0;
			shift_done_count=0;
			clk_count<=0;
			nh_hop_shift_done=0;
			end
		else begin
		if(start_stop) begin
		hop_shift_done=0;
		nh_hop_shift_done=0;
			if(shift_done) begin
				clk_count<=clk_count+1;
				if(clk_count==1'b1)	begin	
				clk_count<=0;
				if(shift_done_count==(bsr_ns-1)) begin
					shift_done_count=0;
					if(packet_count==(bsr_np-1)) begin
					packet_count=0;
						if(hop_count==(o_nh_data-1)) begin
						hop_count=0;
						nh_hop_shift_done=1;
					//	hop_shift_done=1;
						end
						else begin
						hop_shift_done=1;
						hop_count=hop_count + 1;
						end	
					end
					else begin
					packet_count=packet_count+1;
					hop_shift_done=0;
					end
				end
				else 
				shift_done_count=shift_done_count+1;
				end
			end	
			else
			shift_done_count=shift_done_count;	
		end		
		else begin
			shift_done_count=0;
			packet_count=0;
			hop_count=0;
			hop_shift_done=0;
		end	
		end	 
end	
*/
/////////*******BSR CONTROL REGSITER DATA******************////////
	  	
wire [7:0] bsr_bw_int;  			//bar width
wire [7:0] bsr_ns;				//number of symbols
wire [15:0] bsr_isd;			//inter symbol distance		
wire [7:0] bsr_np;				//number of packets
wire [15:0] bsr_ipd;			//inter packet distance
//wire [55:0] control_reg;

//assign bsr_bw = bsr_bw_int; //orignal

always @(posedge sys_clk_i or posedge rst_mobeam) begin
	if(rst_mobeam || !start_stop) begin
		bsr_bw<= 0;
	end
else 
	if (hop_after_txn_neg || config_reg_done_pos_dtc)   //added  "|| config_reg_done_pos_dtc" on 13/feb/2014
	bsr_bw <= 	bsr_bw_int; 
	//else
		//bsr_bw <= 	bsr_bw;
	end
	
		


//assign bsr_bw = (bsr_data_count==31'd7) ? (hop_after_txn_neg ? bsr_bw_int : bsr_bw) : 0 ;


assign bsr_bw_int=(bsr_data_count==31'd7) ? {bsr_buffer[0][7:0]} : 0; 
assign bsr_ns=(bsr_data_count==31'd7) ? bsr_buffer[1][7:0] : 0;
assign bsr_isd=(bsr_data_count==31'd7) ?{bsr_buffer[3][7:0],bsr_buffer[2][7:0]} : 0;
assign bsr_np=(bsr_data_count==31'd7) ? bsr_buffer[4][7:0] : 0;
assign bsr_ipd=(bsr_data_count==31'd7) ? {bsr_buffer[6][7:0],bsr_buffer[5][7:0]}: 0; 

//assign control_reg=!rst_mobeam ? 56'bz : bsr_load_done ? {bsr_ipd,bsr_np,bsr_isd,bsr_ns,bsr_bw}:control_reg;

//assign o_ba_mem_data=start_stop ? ba_mem_data : 16'bz;
//assign o_ba_mem_data=(ba_state==`SHIFT_WORD) ? ba_mem_data : 16'bz;

/************************************************************************/




/////***************FSM DECODE********************//////////////
    /*AUTOASCIIENUM("bsr_state" "state_ASCII")*/
    // Beginning of automatic ASCII enum decoding
    reg [111:0]         state_ASCII;            // Decode of bsr_state
    always @(bsr_state) begin
        case ({bsr_state})
            `IDLE_BSR		:     state_ASCII = "IDLE_BSR    ";
            `NH_READ		: 	  state_ASCII = "NH_READ";
            `NH_READ_LATENCY:     state_ASCII = "NH_READ_LATENCY ";
            `NR_READ		:     state_ASCII = "NR_READ ";
            `NR_READ_LATENCY:     state_ASCII = "NR_READ_LATENCY ";
            `BL_READ		:     state_ASCII = "BL_READ  ";
            `BL_READ_LATENCY:     state_ASCII = "BL_READ_LATENCY  ";
			`BSR_READ		:	  state_ASCII = "BSR_READ  ";
			`BSR_READ_LATENCY:	  state_ASCII = "BSR_READ_LATENCY  ";
			`START_HOP  	:		state_ASCII = "START_HOP  ";
			`LB_READ 		:		state_ASCII = "LB_READ  ";
			`LB_READ_LATENCY:		state_ASCII = "LB_READ_LATENCY  ";
			`LP_READ  		:		state_ASCII = "LP_READ  ";
			`LP_READ_LATENCY:		state_ASCII = "LP_READ_LATENCY  ";
			
			//`BEAM_START_STOP	: state_ASCII = "BEAM_START_STOP  ";
			//`BEAM_START_STOP_LATENCY: state_ASCII = "BEAM_START_STOP_LATENCY  ";
            default:        state_ASCII = "%Error        ";
        endcase
    end
    // End of automatics
	
	/*AUTOASCIIENUM("ba_state" "state_ASCII")*/
    // Beginning of automatic ASCII enum decoding
    reg [111:0]         ba_state_ASCII;            // Decode of ba_state
    always @(ba_state) begin
        case ({ba_state})
            `IDLE_STATE			:     ba_state_ASCII = "IDLE_STATE    ";
            `READ_WORD			: 	  ba_state_ASCII = "READ_WORD";
            `READ_BYTE_LATENCY_1:     ba_state_ASCII = "READ_BYTE_LATENCY_1 ";	 
			`DELAY_STATE		:     ba_state_ASCII = "DELAY_STATE ";

            `SHIFT_WORD			:     ba_state_ASCII = "SHIFT_WORD ";
            default:        ba_state_ASCII = "%Error        ";
        endcase
    end
    // End of automatics
////////////////********************************////////////////////
 /*
wire isd_delay_en;
wire ipd_delay_en;
wire low_isd_delay_en_dtc;
wire tx_out_en;


reg q1_isd_delay_en,q2_isd_delay_en;


always @(posedge sys_clk_i or posedge rst_mobeam) begin
			if(rst_mobeam)  begin
		q1_isd_delay_en<=0;
		q2_isd_delay_en<=0;
		end 
		else begin 
		q1_isd_delay_en<=isd_delay_en;
		q2_isd_delay_en<=q1_isd_delay_en;
		end
	end	
	*/
//	assign low_isd_delay_en_dtc= q1_isd_delay_en && ~isd_delay_en ;///*&& q2_isd_delay_en*/;  

wire isd_delay_en;
wire ipd_delay_en;
wire low_isd_delay_en_dtc;


//assign tx_out_en= ba_mem_rd_en ? ((isd_delay_en || ipd_delay_en) ? 0 : 1) : 0; 

//assign dummy_tx_out_en= ba_mem_rd_en ? ( (isd_delay_en && ipd_delay_en) ? (ipd_delay_en ? 1 : 0) : (isd_delay_en ? 1 : 0)) : 0 ;

//assign dummy_tx_out_en= ba_mem_rd_en ? (isd_delay_en_rise_dtc  ? ((isd_delay_en_rise_dtc && ipd_delay_en_rise_dtc) ? (ipd_delay_en ? 1 : 0) : ipd_delay_en) : (isd_delay_en ? 1 : 0)) : 0 ; //(isd_delay_en ? 1 : 0)) : 0 ;

reg  q1_isd_delay_en,q1_ipd_delay_en;

always @(posedge sys_clk_i or posedge rst_mobeam) begin
	if(rst_mobeam || !start_stop) begin
		q1_isd_delay_en<=0;
		q1_ipd_delay_en<=0;
	end
	else begin	 
		q1_isd_delay_en<=isd_delay_en;	
		q1_ipd_delay_en<=ipd_delay_en;
	end
end	

reg tx_out_en;


always @(posedge sys_clk_i or posedge rst_mobeam) begin
	if(rst_mobeam || !start_stop) begin
	tx_out_en<=0;
	end
else begin	  
	if(ba_mem_rd_en) begin				
		 if(isd_delay_en_rise_dtc && ipd_delay_en_rise_dtc) 
			tx_out_en<=0;
		else if(isd_delay_en_rise_dtc)	
			tx_out_en<=0;	 		   
		else if(isd_delay_en_fall_dtc && ipd_delay_en) //
			tx_out_en<=0;
		else if(ipd_delay_en_fall_dtc)
			tx_out_en<=1;	
		else if(!isd_delay_en && !ipd_delay_en) //
			tx_out_en<=1;		
		else 
			tx_out_en<=tx_out_en;
	end
	else  
		tx_out_en<=0;
	end
end

assign isd_delay_en_rise_dtc=~q1_isd_delay_en && isd_delay_en;
assign ipd_delay_en_rise_dtc=~q1_ipd_delay_en && ipd_delay_en; 	
assign isd_delay_en_fall_dtc=q1_isd_delay_en && ~isd_delay_en;
assign ipd_delay_en_fall_dtc=q1_ipd_delay_en && ~ipd_delay_en; 
assign low_isd_delay_en_dtc= q1_isd_delay_en && ~isd_delay_en ;///*&& q2_isd_delay_en*/;						
		

reg hop_after_txn,q_hop_after_txn;

always @(posedge sys_clk_i or posedge rst_mobeam) begin
	if(rst_mobeam || !start_stop) begin
		hop_after_txn<=0;	
	end
	else begin
		if(hop_shift_done)
		hop_after_txn<=1;
		else if(tx_out_en && bit_done)
		hop_after_txn<=0;
		else 
		hop_after_txn<=hop_after_txn;	
	end
end	 

always @(posedge sys_clk_i or posedge rst_mobeam) begin
	if(rst_mobeam || !start_stop) begin
		q_hop_after_txn<=0;	
	end
	else 
		q_hop_after_txn<=hop_after_txn;
end	 

   assign hop_after_txn_neg= (~hop_after_txn) & q_hop_after_txn;


delay_gen_ihd_ipd_isd mobeam_delay_gen_inst(
	.clk(sys_clk_i),
	.rst(rst_mobeam),
	.isd_val(bsr_isd),
	.ipd_val(bsr_ipd),
	.start_stop(start_stop),
	.config_reg_done(config_reg_done),
	.hop_shift_done(hop_shift_done),
	.packet_shift_done(packet_shift_done),
	.symbol_shift_done(symbol_shift_done),
	.isd_delay_en(isd_delay_en),
	.ipd_delay_en(ipd_delay_en)
	)  ;
	
	/*
	`ifdef SIM	   
	reg shift_done;
	initial begin
		shift_done=0;
		#160000
		repeat(1000) begin
		#40000
		shift_done=1;
		#50
		shift_done=0;
		end
		
	end
	`endif
	*/
endmodule