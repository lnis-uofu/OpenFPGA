// Contributed by narutozxp in https://github.com/lnis-uofu/OpenFPGA/issues/1958
module multi_driver(
    input clk,
    input rst_n,

    input ina1,
    input ina2,
    output [1:0] outa,

    input inb1,
    input inb2,
    output [1:0] outb1,
    output [1:0] outb2
);

reg ina1_reg;
reg ina2_reg;
reg inb1_reg;
reg inb2_reg;
reg [1:0] outa_reg;
reg [1:0] outb_reg;

assign outa = outa_reg;
assign outb1 = outb_reg;
assign outb2 = outb_reg;

always @(posedge clk, negedge rst_n) begin
    if(!rst_n) begin 
        ina1_reg <= 0;
        ina2_reg <= 0;
        inb1_reg <= 0;
        inb2_reg <= 0;
        outa_reg <= 0;
        outb_reg <= 0;
    end else begin
        ina1_reg <= ina1;
        ina2_reg <= ina2;
        inb1_reg <= inb1;
        inb2_reg <= inb2;
        outa_reg <= ina1_reg + ina2_reg;
        outb_reg <= inb1_reg + inb2_reg;
    end
end

endmodule
