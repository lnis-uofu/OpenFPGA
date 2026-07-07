`timescale 1ns/1ps

//---------------------------------------------------------------------
// Testbench : dpram_8x16_preload_tb
// DUT       : dpram_8x16_preload
// Notes     : Adapted to the grouped dpram_8x16_preload top module.
//             A dedicated preload_clk (25 MHz) is generated separately
//             from the system clk (50 MHz) to exercise the clock mux
//             added in the improved design.
//             The former inter-module wires (preload_busy, preload_wen,
//             preload_waddr, preload_d_in) are now internal to the DUT
//             and are no longer needed in the testbench.
//---------------------------------------------------------------------
module dpram_8x16_preload_tb;

    // ----------------------------------------------------------------
    // Clocks and Reset
    // ----------------------------------------------------------------
    reg clk;          // System clock  – 50 MHz  (period = 20 ns)
    reg preload_clk;  // Preload clock – 25 MHz  (period = 40 ns)
    reg rst_n;

    // ----------------------------------------------------------------
    // Initializer control
    // ----------------------------------------------------------------
    reg  init_start;
    wire init_done;

    // ----------------------------------------------------------------
    // Mock Data ROM interconnects
    // (init_src_addr is driven by the DUT; init_src_data is supplied
    //  by the testbench ROM below)
    // ----------------------------------------------------------------
    wire [0:2]  init_src_addr;
    reg  [0:15] init_src_data;

    // ----------------------------------------------------------------
    // System functional interface
    // ----------------------------------------------------------------
    reg        sys_wen;
    reg        sys_ren;
    reg  [0:2] sys_waddr;
    reg  [0:2] sys_raddr;
    reg  [0:15] sys_d_in;
    wire [0:15] sys_d_out;

    // ----------------------------------------------------------------
    // 1. Mock Initialization Memory Source (Look-up Table Content)
    // ----------------------------------------------------------------
    always @(*) begin
        case (init_src_addr)
            3'b000: init_src_data = 16'hA5A5;
            3'b001: init_src_data = 16'h5A5A;
            3'b010: init_src_data = 16'h1111;
            3'b011: init_src_data = 16'h2222;
            3'b100: init_src_data = 16'h3333;
            3'b101: init_src_data = 16'h4444;
            3'b110: init_src_data = 16'h5555;
            3'b111: init_src_data = 16'h7777;
            default: init_src_data = 16'h0000;
        endcase
    end

    // ----------------------------------------------------------------
    // 2. DUT instantiation
    //    preload_clk is now a first-class port; all preload_busy /
    //    preload_wen / preload_waddr / preload_d_in signals are
    //    internal to the DUT and no longer appear here.
    // ----------------------------------------------------------------
    dpram_8x16_preload dut (
        .clk            (clk),
        .preload_clk    (preload_clk),
        .rst_n          (rst_n),
        .init_start     (init_start),
        .init_done      (init_done),
        .init_src_addr  (init_src_addr),
        .init_src_data  (init_src_data),
        .wen            (sys_wen),
        .ren            (sys_ren),
        .waddr          (sys_waddr),
        .raddr          (sys_raddr),
        .d_in           (sys_d_in),
        .d_out          (sys_d_out)
    );

    // ----------------------------------------------------------------
    // 3. Clock Generation
    //    System clk   : 50 MHz  – toggle every 10 ns
    //    Preload clk  : 25 MHz  – toggle every 20 ns
    //    The two clocks start together so phase relationships are
    //    deterministic in simulation.
    // ----------------------------------------------------------------
    always #10 clk        = ~clk;
    always #20 preload_clk = ~preload_clk;

    // ----------------------------------------------------------------
    // 4. Stimulus
    // ----------------------------------------------------------------
    initial begin
        // Initialise signals
        clk         = 0;
        preload_clk = 0;
        rst_n       = 0;
        init_start  = 0;
        sys_wen     = 0;
        sys_ren     = 0;
        sys_waddr   = 3'b000;
        sys_raddr   = 3'b000;
        sys_d_in    = 16'h0000;

        // Hold reset for 2 system clock cycles
        #40;
        rst_n = 1;
        #20;

        // ---- Step 1: Trigger Preload Phase ----
        $display("[TB] Triggering preload memory initialization (preload_clk = 25 MHz)...");
        init_start = 1;

        // Wait until the initializer FSM (running on preload_clk) signals done
        @(posedge init_done);
        // Allow one extra system clock for the mux to settle back to sys clk
        @(posedge clk); #1;
        init_start = 0;
        $display("[TB] Preload memory initialization complete!");

        // Brief idle gap before readback
        #40;

        // ---- Step 2: Read back and verify all preloaded values ----
        $display("[TB] Reading back initialized values via system fabric...");
        sys_ren = 1;

        sys_raddr = 3'b000; @(posedge clk); #1;
        $display("Addr 0 | Expected: A5A5 | Got: %h", sys_d_out);

        sys_raddr = 3'b001; @(posedge clk); #1;
        $display("Addr 1 | Expected: 5A5A | Got: %h", sys_d_out);

        sys_raddr = 3'b010; @(posedge clk); #1;
        $display("Addr 2 | Expected: 1111 | Got: %h", sys_d_out);

        sys_raddr = 3'b011; @(posedge clk); #1;
        $display("Addr 3 | Expected: 2222 | Got: %h", sys_d_out);

        sys_raddr = 3'b100; @(posedge clk); #1;
        $display("Addr 4 | Expected: 3333 | Got: %h", sys_d_out);

        sys_raddr = 3'b101; @(posedge clk); #1;
        $display("Addr 5 | Expected: 4444 | Got: %h", sys_d_out);

        sys_raddr = 3'b110; @(posedge clk); #1;
        $display("Addr 6 | Expected: 5555 | Got: %h", sys_d_out);

        sys_raddr = 3'b111; @(posedge clk); #1;
        $display("Addr 7 | Expected: 7777 | Got: %h", sys_d_out);

        sys_ren = 0;
        #40;

        // ---- Step 3: Normal system write then readback ----
        $display("[TB] Testing standard system runtime write/read...");
        sys_wen   = 1;
        sys_waddr = 3'b010;
        sys_d_in  = 16'hBEEF;
        @(posedge clk); #1;
        sys_wen   = 0;

        sys_ren   = 1;
        sys_raddr = 3'b010;
        @(posedge clk); #1;
        $display("Addr 2 (post-write) | Expected: BEEF | Got: %h", sys_d_out);
        sys_ren = 0;

        #100;
        $display("[TB] All tests complete.");
        $finish;
    end

    // ----------------------------------------------------------------
    // 5. Waveform dump (Icarus Verilog)
    // ----------------------------------------------------------------
    initial begin
        $dumpfile("dpram_8x16_init_waveform.vcd");
        $dumpvars(0, dpram_8x16_preload_tb);
    end

endmodule
