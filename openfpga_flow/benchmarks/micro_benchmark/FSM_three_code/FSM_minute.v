module FSM_minute(
	input wire rst,
	input wire clk,
	input wire [5:0] min_in,
	input wire min_in_load,
	input wire [5:0] sec_count,
	output reg [5:0] min_out);
	
	reg  [2:0] ps, ns;
	wire [5:0] min_data_add;
	reg  [5:0] min_data;
	reg  [5:0] min_ps, min_ns;
	reg  [1:0] min_sel;
	wire min_count;
	
	always@(posedge clk)
	begin
		if(rst) ps <= 3'd0;
		else ps <= ns;
	end
	
	always@(posedge clk)
	begin
		if(rst) min_ps <= 6'd0;
		else min_ps <= min_ns;
	end
	
	always@(*)
	begin
		min_sel = 2'd0;
		case(ps)
			3'd0: begin
				ns = 3'd1;
			end
			3'd1: begin
				if(min_in_load) begin
					min_sel = 2'd1;
					min_out = min_data;
					ns = 3'd2;
					min_ns = min_data;
				end
				else ns = 3'd1;
			end
			3'd2: begin
				if(min_count == 1'd1) begin
					if(min_data == 6'd59) begin
						min_out = min_data;
						ns = 3'd2;
						min_ns = 6'd0;
					end
					else begin
						min_out = min_data;
						ns = 3'd2;
						min_ns = min_data_add;
					end
				end
				else begin
					min_out = min_data;
					min_ns = min_data;
					ns = 3'd2;
				end
			end
			default: begin
				ns = 3'd0;
			end
		endcase
	end
	
	assign min_data_add = min_data + 1;
	assign min_count = (sec_count == 6'd59) ? 1'd1 : 1'd0;
	
	always@(*)
	begin
		case(min_sel)
			2'd0: min_data = min_ps;
			2'd1: min_data = min_in;
		endcase
	end

endmodule