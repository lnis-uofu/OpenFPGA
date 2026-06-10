`timescale 1ns/1ps

module tb_dpram_initializer;

    // Clock and Reset
    reg clk;
    reg rst_n;
    
    // Initializer Control
    reg init_start;
    wire init_done;
    
    // Big-Endian Interconnect wires between Initializer and Preload Core
    wire preload_busy;
    wire preload_wen;
    wire [0:2]  preload_waddr;
    wire [0:15] preload_d_in;
    
    // Mock Data ROM Interconnects
    wire [0:2]  init_src_addr;
    reg  [0:15] init_src_data;

    // Standard User Fabric System Interfaces (Big-Endian Indexed)
    reg         sys_wen;
    reg         sys_ren;
    reg  [0:2]  sys_waddr;
    reg  [0:2]  sys_raddr;
    reg  [0:15] sys_d_in;
    wire [0:15] sys_d_out;

    // 1. Mock Initialization Memory Source (Look-up Table Content)
    always @(*) begin
        case(init_src_addr)
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

    // 2. Instantiate the Big-Endian Memory Block Initializer
    dpram_preload_initializer u_initializer (
        .clk             (clk),
        .rst_n           (rst_n),
        .init_start      (init_start),
        .init_done       (init_done),
        .init_src_addr   (init_src_addr),
        .init_src_data   (init_src_data),
        .preload_busy    (preload_busy),
        .preload_wen     (preload_wen),
        .preload_waddr   (preload_waddr),
        .preload_d_in    (preload_d_in)
    );

    // 3. Instantiate the Patched Core Block mapping Big-Endian Ports
    dpram_8x16_preload u_ram_block (
        .preload_busy    (preload_busy),
        .preload_wen     (preload_wen),
        .preload_waddr   (preload_waddr),
        .preload_d_in    (preload_d_in),
        
        .clk             (clk),
        .wen             (sys_wen),
        .ren             (sys_ren),
        .waddr           (sys_waddr),
        .raddr           (sys_raddr),
        .d_in            (sys_d_in),
        .d_out           (sys_d_out)
    );

    // Clock Generation (50MHz)
    always #10 clk = ~clk;

    initial begin
        // Initialize Signals
        clk = 0;
        rst_n = 0;
        init_start = 0;
        sys_wen = 0;
        sys_ren = 0;
        sys_waddr = 3'b000;
        sys_raddr = 3'b000;
        sys_d_in = 16'h0000;

        #40;
        rst_n = 1;
        #20;

        // ---- Step 1: Trigger Preload Phase ----
        $display("[TB] Triggering Big-Endian Preload Memory Initialization...");
        init_start = 1;
        
        // Wait until initialization is complete
        @ (posedge init_done);
        #10;
        init_start = 0;
        $display("[TB] Preload Memory Initialization Complete!");
        
        #40;

        // ---- Step 2: Read Back and Verify RAM Contents ----
        $display("[TB] Reading back initialized values via normal system fabric routing...");
        sys_ren = 1;
        
        sys_raddr = 3'b000; #20;
        $display("Addr 0 | Expected: A5A5 | Got: %h", sys_d_out);
        
        sys_raddr = 3'b001; #20;
        $display("Addr 1 | Expected: 5A5A | Got: %h", sys_d_out);
        
        sys_raddr = 3'b010; #20;
        $display("Addr 2 | Expected: 1111 | Got: %h", sys_d_out);

        sys_raddr = 3'b111; #20;
        $display("Addr 7 | Expected: 7777 | Got: %h", sys_d_out);
        
        sys_ren = 0;
        #40;
        
        // ---- Step 3: Run Normal System Write/Read Cycles ----
        $display("[TB] Testing standard system runtime writes...");
        sys_wen = 1;
        sys_waddr = 3'b010;
        sys_d_in = 16'hBEEF;
        #20;
        sys_wen = 0;
        
        // Readback overridden address
        sys_ren = 1;
        sys_raddr = 3'b010;
        #20;
        $display("Addr 2 (Post-Write) | Expected: BEEF | Got: %h", sys_d_out);
        sys_ren = 0;

        #100;
        $finish;
    end

endmodule
