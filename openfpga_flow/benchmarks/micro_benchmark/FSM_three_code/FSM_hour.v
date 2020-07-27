module FSM_hour(
	input wire rst,
	input wire clk,
	input wire [5:0] hour_in,
	input wire hour_in_load,
	input wire [5:0] min_count,
	input wire [5:0] sec_count,
	output reg [5:0] hour_out);
	
	reg  [2:0] ps, ns;
	wire [5:0] hour_data_add;
	reg  [5:0] hour_data;
	reg  [5:0] hour_ps, hour_ns;
	reg  [1:0] hour_sel;
	wire hour_count;
	
	always@(posedge clk)
	begin
		if(rst) ps <= 3'd0;
		else ps <= ns;
	end
	
	always@(posedge clk)
	begin
		if(rst) hour_ps <= 6'd0;
		else hour_ps <= hour_ns;
	end
	
	always@(*)
	begin
		hour_sel = 2'd0;
		case(ps)
			3'd0: begin
				ns = 3'd1;
			end
			3'd1: begin
				if(hour_in_load) begin
					hour_sel = 2'd1;
					hour_out = hour_data;
					ns = 3'd2;
					hour_ns = hour_data;
				end
				else ns = 3'd1;
			end
			3'd2: begin
				if(hour_count == 1'd1) begin
					if(hour_data == 6'd59) begin
						hour_out = hour_data;
						ns = 3'd2;
						hour_ns = 6'd0;
					end
					else begin
						hour_out = hour_data;
						ns = 3'd2;
						hour_ns = hour_data_add;
					end
				end
				else begin
					hour_out = hour_data;
					hour_ns = hour_data;
					ns = 3'd2;
				end
			end
			default: begin
				ns = 3'd0;
			end
		endcase
	end
	
	assign hour_data_add = hour_data + 1;
	assign hour_count = ((sec_count == 6'd59)&&(min_count == 6'd59)) ? 1'd1 : 1'd0;
	
	always@(*)
	begin
		case(hour_sel)
			2'd0: hour_data = hour_ps;
			2'd1: hour_data = hour_in;
		endcase
	end

endmodule