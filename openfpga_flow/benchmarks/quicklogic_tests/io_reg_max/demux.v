module demux_1x1024 (in,sel,out,clk);
input in;
input clk;

input [9:0] sel;
output [1023:0] out;
wire [1:0] out_w;

demux_1x2 d1024_0(.in(in),.sel(sel[9]),.out(out_w[1:0]));
demux_1x512 d1024_1(.in(out_w[0]),.sel(sel[8:0]),.out(out[511:0]),.clk(clk));
demux_1x512 d1024_2(.in(out_w[1]),.sel(sel[8:0]),.out(out[1023:512]),.clk(clk));

endmodule


module demux_1x512 (in,sel,out,clk);
input in;
input clk;

input [8:0] sel;
output [511:0] out;
wire [1:0] out_w;

demux_1x2 d512_0(.in(in),.sel(sel[8]),.out(out_w[1:0]));
demux_1x256 d512_1(.in(out_w[0]),.sel(sel[7:0]),.out(out[255:0]),.clk(clk));
demux_1x256 d512_2(.in(out_w[1]),.sel(sel[7:0]),.out(out[511:256]),.clk(clk));

endmodule

module demux_1x256 (in,sel,out,clk);
input in;
input clk;

input [7:0] sel;
output [255:0] out;
wire [1:0] out_w;

demux_1x2 d256_0(.in(in),.sel(sel[7]),.out(out_w[1:0]));
demux_1x128 d256_1(.in(out_w[0]),.sel(sel[6:0]),.out(out[127:0]),.clk(clk));
demux_1x128 d256_2(.in(out_w[1]),.sel(sel[6:0]),.out(out[255:128]),.clk(clk));

endmodule

module demux_1x128 (in,sel,out,clk);
input in;
input clk;

input [6:0] sel;
output [127:0] out;
wire [1:0] out_w;

demux_1x2 d128_0(.in(in),.sel(sel[6]),.out(out_w[1:0]));
demux_1x64 d128_1(.in(out_w[0]),.sel(sel[5:0]),.out(out[63:0]),.clk(clk));
demux_1x64 d128_2(.in(out_w[1]),.sel(sel[5:0]),.out(out[127:64]),.clk(clk));

endmodule

module demux_1x64 (in,sel,out,clk);
input in;
input clk;

input [5:0] sel;
output [63:0] out;
wire [1:0] out_w;

demux_1x2 d64_0(.in(in),.sel(sel[5]),.out(out_w[1:0]));
demux_1x32 d64_1(.in(out_w[0]),.sel(sel[4:0]),.out(out[31:0]),.clk(clk));
demux_1x32 d64_2(.in(out_w[1]),.sel(sel[4:0]),.out(out[63:32]),.clk(clk));

endmodule

module demux_1x32 (in,sel,out,clk);
input in;
input clk;

input [4:0] sel;
output [31:0] out;
wire [1:0] out_w;

demux_1x2 d32_0(.in(in),.sel(sel[4]),.out(out_w[1:0]));
demux_1x16 d32_1(.in(out_w[0]),.sel(sel[3:0]),.out(out[15:0]),.clk(clk));
demux_1x16 d32_2(.in(out_w[1]),.sel(sel[3:0]),.out(out[31:16]),.clk(clk));

endmodule


module demux_1x16 (in,sel,out,clk);
input in;
input clk;

input [3:0] sel;
output [15:0] out;
wire [1:0] out_w;

demux_1x2 d16_0(.in(in),.sel(sel[3]),.out(out_w[1:0]));
demux_1x8 d16_1(.in(out_w[0]),.sel(sel[2:0]),.out(out[7:0]),.clk(clk));
demux_1x8 d16_2(.in(out_w[1]),.sel(sel[2:0]),.out(out[15:8]),.clk(clk));

endmodule

module demux_1x8 (in,sel,out,clk);
input in;
input clk;

input [2:0] sel;
output [7:0] out;
wire [1:0] out_w;

demux_1x2 d8_0(.in(in),.sel(sel[2]),.out(out_w[1:0]));
demux_1x4 d8_1(.in(out_w[0]),.sel(sel[1:0]),.out(out[3:0]),.clk(clk));
demux_1x4 d8_2(.in(out_w[1]),.sel(sel[1:0]),.out(out[7:4]),.clk(clk));

endmodule 

module demux_1x4 (in,sel,out,clk);
input in;
input clk;

input [1:0] sel;
output [3:0] out;
wire [1:0]out_w;

demux_1x2 d4_0(.in(in),.sel(sel[1]),.out(out_w));
demux_1x2_reg d4_1(.in(out_w[0]),.sel(sel[0]),.out(out[1:0]),.clk(clk));
demux_1x2_reg d4_2(.in(out_w[1]),.sel(sel[0]),.out(out[3:2]),.clk(clk));


endmodule 

module demux_1x2 (in,sel,out);
input in,sel;
output [1:0] out;

assign out[0] = (sel==0) ? in :0;
assign out[1] = (sel==1) ? in :0;

endmodule

module demux_1x2_reg (in,sel,out,clk);
input in,sel;
output reg [1:0] out;
input clk;
wire [1:0]out_w;

assign out_w[0] = (sel==0) ? in :0;
assign out_w[1] = (sel==1) ? in :0;


always @(posedge clk)
	out <= out_w;

endmodule 

