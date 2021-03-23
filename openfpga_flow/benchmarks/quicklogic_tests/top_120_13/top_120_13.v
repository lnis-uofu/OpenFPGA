// 5 counter with 5 clock domain
// each counter has 130 bits
// each counter has 21 output

// test: placement , routing and performace for each clock domain

module top_120_13(clk1,clk2,clk3,out1x,out2x,out3x,reset);

input clk1,clk2,clk3,reset;

output [13:0] out1x;
output [13:0] out2x;
output [13:0] out3x;
//output [13:0] out4x;
//output [13:0] out5x;

reg [120:0] cnt1;
reg [120:0] cnt2;
reg [120:0] cnt3;
//reg [120:0] cnt4;
//reg [120:0] cnt5;


assign out1x = {cnt1[120:115],cnt1[7:0]};
assign out2x = {cnt2[120:115],cnt2[7:0]};
assign out3x = {cnt3[120:115],cnt3[7:0]};
//assign out4x = {cnt4[120:115],cnt4[7:0]};
//assign out5x = {cnt5[120:115],cnt5[7:0]};
                     

always  @(posedge clk1)

begin

    if (reset)

        cnt1 <=1'b0;

    else

        cnt1 <= cnt1+1;

end


always  @(posedge clk2)

begin

    if (reset)

        cnt2 <=1'b0;

    else

        cnt2 <= cnt2 +1;

end
always  @(posedge clk3)

begin

    if (reset)

        cnt3 <=1'b0;

    else

        cnt3 <= cnt3 +1;

end

/*
always  @(posedge clk4)

begin

    if (reset)

        cnt4 <=1'b0;

    else

        cnt4 <= cnt4 +1;

end

always  @(posedge clk5)

begin

    if (reset)

        cnt5 <=1'b0;

    else

        cnt5 <= cnt5 +1;

end

*/
endmodule

