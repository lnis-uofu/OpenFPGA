// Creating a scaleable adder

module adder_16_clk #(
    parameter size = 2
)(
    output reg cout,
    output reg [size-1:0] sum, 	 // sum uses the size parameter
    input cin,
    input [size-1:0] a, b,  // 'a' and 'b' use the size parameter
    input clk_in,
    input rst
);


// reg [size-1:0] _sum;
// reg _cout;

always @(posedge clk_in) begin
    if (rst)
        {sum,cout} <= 0;
    else
        {sum,cout} <= a + b + cin;
end

// assign sum = _sum;
// assign cout = _cout;

// assign {cout, sum} = a + b + cin;

endmodule









