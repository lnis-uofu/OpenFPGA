module mux_1024x1 (in,sel,out);
input [1023:0] in;
input [9:0]sel;
output out;
wire out0_w, out1_w;

mux_512x1 m1024_0(.in(in[511:0]),.sel(sel[8:0]),.out(out0_w));
mux_512x1 m1024_1(.in(in[1023:512]),.sel(sel[8:0]),.out(out1_w));
mux_2x1 m1024_2(.a(out0_w),.b(out1_w),.sel(sel[9]),.out(out));

endmodule


module mux_512x1 (in,sel,out);
input [511:0] in;
input [8:0]sel;
output out;
wire out0_w, out1_w;

mux_256x1 m512_0(.in(in[255:0]),.sel(sel[7:0]),.out(out0_w));
mux_256x1 m512_1(.in(in[511:256]),.sel(sel[7:0]),.out(out1_w));
mux_2x1 m512_2(.a(out0_w),.b(out1_w),.sel(sel[8]),.out(out));

endmodule

module mux_256x1 (in,sel,out);
input [255:0] in;
input [7:0]sel;
output out;
wire out0_w, out1_w;

mux_128x1 m256_0(.in(in[127:0]),.sel(sel[6:0]),.out(out0_w));
mux_128x1 m256_1(.in(in[255:128]),.sel(sel[6:0]),.out(out1_w));
mux_2x1 m256_2(.a(out0_w),.b(out1_w),.sel(sel[7]),.out(out));

endmodule

module mux_128x1 (in,sel,out);
input [127:0] in;
input [6:0]sel;
output out;
wire out0_w, out1_w;

mux_64x1 m128_0(.in(in[63:0]),.sel(sel[5:0]),.out(out0_w));
mux_64x1 m128_1(.in(in[127:64]),.sel(sel[5:0]),.out(out1_w));
mux_2x1 m128_2(.a(out0_w),.b(out1_w),.sel(sel[6]),.out(out));

endmodule

module mux_64x1 (in,sel,out);
input [63:0] in;
input [5:0]sel;
output out;
wire out0_w, out1_w;

mux_32x1 m64_0(.in(in[31:0]),.sel(sel[4:0]),.out(out0_w));
mux_32x1 m64_1(.in(in[63:32]),.sel(sel[4:0]),.out(out1_w));
mux_2x1 m64_2(.a(out0_w),.b(out1_w),.sel(sel[5]),.out(out));

endmodule

module mux_32x1 (in,sel,out);
input [31:0] in;
input [4:0]sel;
output out;
wire out0_w, out1_w;

mux_16x1 m32_0(.in(in[15:0]),.sel(sel[3:0]),.out(out0_w));
mux_16x1 m32_1(.in(in[31:16]),.sel(sel[3:0]),.out(out1_w));
mux_2x1 m32_2(.a(out0_w),.b(out1_w),.sel(sel[4]),.out(out));

endmodule

module mux_16x1 (in,sel,out);
input [15:0] in;
input [3:0]sel;
output out;
wire out0_w, out1_w;

mux_8x1 m16_0(.in(in[7:0]),.sel(sel[2:0]),.out(out0_w));
mux_8x1 m16_1(.in(in[15:8]),.sel(sel[2:0]),.out(out1_w));
mux_2x1 m16_2(.a(out0_w),.b(out1_w),.sel(sel[3]),.out(out));

endmodule

module mux_8x1 (in,sel,out);
input [7:0] in;
input [2:0]sel;
output out;
wire out0_w, out1_w;

mux_4x1 m8_0(.in(in[3:0]),.sel(sel[1:0]),.out(out0_w));
mux_4x1 m8_1(.in(in[7:4]),.sel(sel[1:0]),.out(out1_w));
mux_2x1 m8_2(.a(out0_w),.b(out1_w),.sel(sel[2]),.out(out));

endmodule

module mux_4x1 (in,sel,out);
input [3:0] in;
input [1:0]sel;
output out;
wire out0_w, out1_w;

mux_2x1 m4_0(.a(in[0]),.b(in[1]),.sel(sel[0]),.out(out0_w));
mux_2x1 m4_1(.a(in[2]),.b(in[3]),.sel(sel[0]),.out(out1_w));
mux_2x1 m4_2(.a(out0_w),.b(out1_w),.sel(sel[1]),.out(out));

endmodule 

module mux_2x1 (a,b,sel,out);
input a,b;
input sel;
output out;

assign out = sel ? b : a;

endmodule 
