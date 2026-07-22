`timescale 1ns/1ps
module frac_mem_256_preload_tb;
    reg clk = 0, preload_clk = 0, rst_n = 0;
    reg init_start = 0;
    wire init_done;
    wire [0:3] init_src_addr;
    reg [0:15] init_src_data;

    reg wen = 0, ren = 0;
    reg [0:4] waddr = 0, raddr = 0;
    reg [0:31] d_in = 0;
    reg [0:1] mode = 0;
    wire [0:31] d_out;

    integer errors = 0;

    frac_mem_256_preload dut (
        .clk(clk), .preload_clk(preload_clk), .rst_n(rst_n),
        .init_start(init_start), .init_done(init_done),
        .init_src_addr(init_src_addr), .init_src_data(init_src_data),
        .wen(wen), .ren(ren), .waddr(waddr), .raddr(raddr),
        .d_in(d_in), .mode(mode), .d_out(d_out)
    );

    always #5  clk = ~clk;
    always #7  preload_clk = ~preload_clk; // async, different period on purpose

    // simple ROM: row i -> 16'hA000 + i
    always @(*) init_src_data = 16'hA000 + init_src_addr;

    task automatic do_write(input [0:1] m, input [0:4] a, input [0:31] data);
        begin
            @(negedge clk);
            mode = m; waddr = a; d_in = data; wen = 1;
            @(negedge clk);
            wen = 0;
        end
    endtask

    task automatic do_read(input [0:1] m, input [0:4] a, output [0:31] data);
        begin
            @(negedge clk);
            mode = m; raddr = a; ren = 1;
            @(negedge clk);
            ren = 0;
            data = d_out;
        end
    endtask

    reg [0:31] rd;
    integer i;

    initial begin
        rst_n = 0;
        #20 rst_n = 1;

        // Kick off preload
        @(negedge preload_clk);
        init_start = 1;
        wait (init_done == 1);
        $display("Preload finished at time %0t", $time);
        @(negedge preload_clk);
        init_start = 0;
        wait (init_done == 0);

        // Let clock domains settle
        repeat (4) @(negedge clk);

        // ---- Check via raw-equivalent mode 00 (16x16): row i should read back A000+i in low 16 bits
        for (i = 0; i < 16; i = i + 1) begin
            do_read(2'b00, {i[3:0], 1'b0}, rd);
            if (rd[0:15] !== (16'hA000 + i)) begin
                $display("FAIL mode00 row %0d: got %h expected %h", i, rd[0:15], 16'hA000+i);
                errors = errors + 1;
            end
        end
        $display("Preload readback via mode00 done, errors=%0d", errors);

        // ---- Test 16x16 write/read
        do_write(2'b00, 5'd3, {16'h1234, 16'h0000});
        do_read (2'b00, 5'd3, rd);
        if (rd[0:15] !== 16'h1234) begin
            $display("FAIL 16x16 rw: got %h", rd[0:15]); errors = errors + 1;
        end else $display("PASS 16x16 read/write");

        // ---- Test 8x32 write/read (addr 2 -> rows 4,5)
        do_write(2'b01, 5'd2, {16'hDEAD, 16'hBEEF});
        do_read (2'b01, 5'd2, rd);
        if (rd !== {16'hDEAD, 16'hBEEF}) begin
            $display("FAIL 8x32 rw: got %h", rd); errors = errors + 1;
        end else $display("PASS 8x32 read/write");

        // ---- Test 32x8 write/read (addr 0..31 -> row=addr[0:3], byte=addr[4])
        do_write(2'b10, {4'd7, 1'b0}, {8'h55, 24'h0}); // low byte of row 7
        do_write(2'b10, {4'd7, 1'b1}, {8'h66, 24'h0}); // high byte of row 7
        do_read (2'b10, {4'd7, 1'b0}, rd);
        if (rd[0:7] !== 8'h55) begin
            $display("FAIL 32x8 low byte: got %h", rd[0:7]); errors = errors + 1;
        end else $display("PASS 32x8 low byte");
        do_read (2'b10, {4'd7, 1'b1}, rd);
        if (rd[0:7] !== 8'h66) begin
            $display("FAIL 32x8 high byte: got %h", rd[0:7]); errors = errors + 1;
        end else $display("PASS 32x8 high byte");

        if (errors == 0)
            $display("ALL TESTS PASSED");
        else
            $display("%0d TEST(S) FAILED", errors);

        $finish;
    end

    initial begin
      $dumpfile("frac_mem_256_preload_tb.vcd");
      $dumpvars(0, frac_mem_256_preload_tb);
    end
endmodule
