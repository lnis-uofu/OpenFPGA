module Mux_31(Y,A0,A1,A2,sel);

output	[7:0]Y;
input	[7:0]A2,A1,A0;
input	[1:0]sel;

reg		[7:0]Y;

always@(*)
	begin
		case(sel)
			0:	Y=A0;
			1:	Y=A1;
			2:	Y=A2;
			default:Y=8'bz;
		endcase
	end

endmodule

