`timescale 1ns / 1ps

module tb_dual_port_ram_128;

    // Testbench parameters
    parameter MEM_FILE = "init.hex";

    // Inputs to the DUT (reg for driving stimulus)
    reg clk;
    reg wen;
    reg ren;
    reg [2:0] waddr;
    reg [2:0] raddr;
    reg [15:0] din;

    // Outputs from the DUT
    wire [15:0] dout;
    
    // File descriptor variable declared at the module level (Pure Verilog requirement)
    integer f;

    // Instantiate the Device Under Test (DUT)
    dual_port_ram_128 #(
        .MEM_FILE(MEM_FILE)
    ) uut (
        .clk(clk),
        .wen(wen),
        .ren(ren),
        .waddr(waddr),
        .raddr(raddr),
        .din(din),
        .dout(dout)
    );

    // Clock generation (50MHz clock -> 20ns period)
    always begin
        #10 clk = ~clk;
    end

    // Pure Verilog task using module-level registers
    task read_ram;
        input [2:0] addr;
        begin
            @(posedge clk);
            raddr = addr;
            ren = 1'b1;
            @(posedge clk);
            #1; // Small post-edge delay for output settling
            ren = 1'b0;
        end
    endtask

    // Main Test Stimulus
    initial begin
        // Step 1: Create the initialization file dynamically[cite: 2]
        f = $fopen(MEM_FILE, "w");
        $fwrite(f, "0 138F\n");
        $fwrite(f, "1 0020\n");
        $fwrite(f, "2 37EA\n");
        $fwrite(f, "@7 42FB\n");
        $fclose(f);

        // Initialize signals
        clk = 0;
        wen = 0;
        ren = 0;
        waddr = 0;
        raddr = 0;
        din = 0;

        // Wait for two clock edges to settle
        @(posedge clk);
        @(posedge clk);

        // -----------------------------------------------------------------
        // VERIFICATION: Check RAM Initialization Results (Readback)[cite: 2]
        // -----------------------------------------------------------------
        $display("--- Starting Readback Check of Initialized Values ---");

        // Check index 0 (Expected: 16'h138F)[cite: 2]
        read_ram(3'd0);
        $display("[READ] Addr: 0 | Expected: 16'h138F | Got: 16'h%h", dout);
        if (dout !== 16'h138F) $display("ERROR: Initialization mismatch at Addr 0!");

        // Check index 1 (Expected: 16'h0020)[cite: 2]
        read_ram(3'd1);
        $display("[READ] Addr: 1 | Expected: 16'h0020 | Got: 16'h%h", dout);
        if (dout !== 16'h0020) $display("ERROR: Initialization mismatch at Addr 1!");

        // Check index 2 (Expected: 16'h37EA)[cite: 2]
        read_ram(3'd2);
        $display("[READ] Addr: 2 | Expected: 16'h37EA | Got: 16'h%h", dout);
        if (dout !== 16'h37EA) $display("ERROR: Initialization mismatch at Addr 2!");

        // Check index 7 (Expected: 16'h42FB)[cite: 2]
        read_ram(3'd7);
        $display("[READ] Addr: 7 | Expected: 16'h42FB | Got: 16'h%h", dout);
        if (dout !== 16'h42FB) $display("ERROR: Initialization mismatch at Addr 7!");

        // -----------------------------------------------------------------
        // VERIFICATION: Write new data and read it back
        // -----------------------------------------------------------------
        $display("\n--- Starting Write-Then-Readback Check ---");

        // Write 16'hABCD to Address 3
        @(posedge clk);
        waddr = 3'd3;
        din   = 16'hABCD;
        wen   = 1'b1;
        @(posedge clk);
        wen   = 1'b0; // Deassert write enable
        
        // Readback Address 3 to confirm modification
        read_ram(3'd3);
        $display("[READ] Addr: 3 | Expected: 16'hABCD | Got: 16'h%h", dout);
        if (dout !== 16'hABCD) $display("ERROR: Write-Readback mismatch at Addr 3!");

        // Finish simulation
        #40;
        $display("\nSimulation Complete.");
        $finish;
    end

endmodule
