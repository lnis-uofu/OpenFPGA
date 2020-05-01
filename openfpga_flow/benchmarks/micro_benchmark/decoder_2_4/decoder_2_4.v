
module decoder_2_4(in0,in1,out0,out1,out2,out3);

input in0,in1;
output out0,out1,out2,out3;
assign out0 = ~in0 & ~in1;
assign out1 = in0 & ~in1;
assign out2 = ~in0 & in1;
assign out3 = in0 & in1;

endmodule
