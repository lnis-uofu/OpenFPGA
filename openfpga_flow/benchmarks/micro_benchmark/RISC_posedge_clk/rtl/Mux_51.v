module Mux_51(Y,A0,A1,A2,A3,A4,sel);

output	[7:0]Y;
input	[7:0]A4,A3,A2,A1,A0;
input	[2:0]sel;

reg		[7:0]Y;

always@(*)
	begin
		case(sel)
			0:	Y=A0;
			1:	Y=A1;
			2:	Y=A2;
			3:	Y=A3;
			4:	Y=A4;
			default:Y=8'bx;
		endcase
	end

endmodule

