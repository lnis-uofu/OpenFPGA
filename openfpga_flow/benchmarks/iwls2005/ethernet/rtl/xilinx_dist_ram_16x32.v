module xilinx_dist_ram_16x32
(
    data_out,
    we,
    data_in,
    read_address,
    write_address,
    wclk
);
    output [31:0] data_out;
    input we, wclk;
    input [31:0] data_in;
    input [3:0] write_address, read_address;

    wire [3:0] waddr = write_address ;
    wire [3:0] raddr = read_address ;

    RAM16X1D ram00 (.DPO(data_out[0]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[0]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram01 (.DPO(data_out[1]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[1]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram02 (.DPO(data_out[2]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[2]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram03 (.DPO(data_out[3]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[3]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram04 (.DPO(data_out[4]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[4]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram05 (.DPO(data_out[5]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[5]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram06 (.DPO(data_out[6]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[6]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram07 (.DPO(data_out[7]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[7]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram08 (.DPO(data_out[8]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[8]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram09 (.DPO(data_out[9]),  .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[9]),  .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram10 (.DPO(data_out[10]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[10]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram11 (.DPO(data_out[11]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[11]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram12 (.DPO(data_out[12]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[12]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram13 (.DPO(data_out[13]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[13]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram14 (.DPO(data_out[14]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[14]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram15 (.DPO(data_out[15]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[15]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram16 (.DPO(data_out[16]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[16]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram17 (.DPO(data_out[17]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[17]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram18 (.DPO(data_out[18]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[18]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram19 (.DPO(data_out[19]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[19]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram20 (.DPO(data_out[20]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[20]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram21 (.DPO(data_out[21]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[21]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram22 (.DPO(data_out[22]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[22]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram23 (.DPO(data_out[23]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[23]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram24 (.DPO(data_out[24]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[24]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram25 (.DPO(data_out[25]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[25]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram26 (.DPO(data_out[26]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[26]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram27 (.DPO(data_out[27]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[27]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram28 (.DPO(data_out[28]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[28]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram29 (.DPO(data_out[29]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[29]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram30 (.DPO(data_out[30]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[30]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
    RAM16X1D ram31 (.DPO(data_out[31]), .SPO(), .A0(waddr[0]), .A1(waddr[1]), .A2(waddr[2]), .A3(waddr[3]), .D(data_in[31]), .DPRA0(raddr[0]), .DPRA1(raddr[1]), .DPRA2(raddr[2]), .DPRA3(raddr[3]), .WCLK(wclk), .WE(we));
endmodule
