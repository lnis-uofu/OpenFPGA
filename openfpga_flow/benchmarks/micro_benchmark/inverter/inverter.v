
module inverter (
    input wire in,
    output wire out
);
    assign out = ~in;
endmodule