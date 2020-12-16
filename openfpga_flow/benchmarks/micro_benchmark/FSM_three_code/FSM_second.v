module FSM_second(
	input wire rst,
	input wire clk,
	input wire [5:0] sec_in,
	input wire sec_in_load,
	output reg [5:0] sec_out);
	
	reg  [2:0] ps, ns;
	wire [5:0] sec_data_add;
	reg  [5:0] sec_data;
	reg  [5:0] sec_ps, sec_ns;
	reg  [1:0] sec_sel;
	
	always@(posedge clk)
	begin
		if(rst) ps <= 3'd0;
		else ps <= ns;
	end
	
	always@(posedge clk)
	begin
		if(rst) sec_ps <= 6'd0;
		else sec_ps <= sec_ns;
	end
	
	always@(*)
	begin
		sec_sel = 2'd0;
		case(ps)
			3'd0: begin
				ns = 3'd1;
			end
			3'd1: begin
				if(sec_in_load) begin
					sec_sel = 2'd1;
					sec_out = sec_data;
					ns = 3'd2;
					sec_ns = sec_data_add;
				end
				else ns = 3'd1;
			end
			3'd2: begin
				if(sec_data == 6'd59) begin
					sec_out = sec_data;
					ns = 3'd2;
					sec_ns = 6'd0;
				end
				else begin
					sec_out = sec_data;
					ns = 3'd2;
					sec_ns = sec_data_add;
				end
			end
			default: begin
				ns = 3'd0;
			end
		endcase
	end
	
	assign sec_data_add = sec_data + 1;
	
	always@(*)
	begin
		case(sec_sel)
			2'd0: sec_data = sec_ps;
			2'd1: sec_data = sec_in;
		endcase
	end

endmodule