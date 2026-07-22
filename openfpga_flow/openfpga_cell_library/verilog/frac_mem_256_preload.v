//---------------------------------------------------------------------
// Design Name : frac_mem_256_preload_ctrl (Port-to-Port Routing Version)
// Description : Manages muxing logic externally. Outputs clean, resolved
//               signals ready to drive a standalone frac_mem_256_core.
//               Preload only needs to populate the memory in its default
//               16x16 (mode 2'b00) organization, so core_mode is forced
//               to 2'b00 whenever preload_busy is active: each preload
//               row (0-15) is written as one 16-bit word via the same
//               write path mode 2'b00 uses at runtime.
//---------------------------------------------------------------------
module frac_mem_256_preload_ctrl (
    // System Functional Interface Inputs
    input  wire        sys_wen,
    input  wire        sys_ren,
    input  wire [0:4]  sys_waddr,
    input  wire [0:4]  sys_raddr,
    input  wire [0:31] sys_d_in,
    input  wire [0:1]  sys_mode,
    // Preload Controller Interface Inputs
    input  wire        preload_busy,
    input  wire        preload_wen,
    input  wire [0:3]  preload_waddr,
    input  wire [0:15] preload_d_in,
    // Core Memory Block Driving Outputs
    output wire        core_wen,
    output wire        core_ren,
    output wire [0:4]  core_waddr,
    output wire [0:4]  core_raddr,
    output wire [0:31] core_data_in,
    output wire [0:1]  core_mode
);
    // Mode forced onto the core while preloading: default 16x16 mode,
    // where row = waddr[0:3] and the write data is a plain 16-bit word.
    localparam MODE_16X16 = 2'b00;

    // --- Integrated Multiplexer Logic ---
    // If preload_busy is active, route controller signals to the Core
    // outputs (address/data are zero-extended into the wider system
    // buses). Otherwise, pass through the native system inputs.
    assign core_wen      = (preload_busy) ? preload_wen               : sys_wen;
    assign core_waddr    = (preload_busy) ? {preload_waddr, 1'b0}     : sys_waddr;
    assign core_data_in  = (preload_busy) ? {preload_d_in, 16'b0}     : sys_d_in;
    assign core_mode     = (preload_busy) ? MODE_16X16                : sys_mode;
    // Safely disable memory reads while initialization cycles are occurring
    assign core_ren      = (preload_busy) ? 1'b0 : sys_ren;
    assign core_raddr    = sys_raddr;
endmodule

//-----------------------------------------------------
// Design Name : frac_mem_256_core
// Function    : Fixed/cleaned-up version of frac_mem_256. A fracturable
//               BRAM which can operate in 3 modes:
//               - 2'b00 : Dual port RAM 16 x 16 bit
//               - 2'b01 : Dual port RAM  8 x 32 bit
//               - 2'b10 : Dual port RAM 32 x  8 bit
//               Preload (see frac_mem_256_preload below) always writes
//               through the 2'b00 (16x16) path, one 16-bit row at a time.
//               Fixes applied relative to the original frac_mem_256.v:
//                 * ram[] sized to 16 rows (0:15) of 16 bits = 256 bits
//                   total storage, matching the module's name/address
//                   width, instead of the original 4-row array.
//                 * 8x32 mode: read logic now correctly pulls the
//                   upper 16 bits from the "odd" row instead of
//                   re-reading the "even" row twice, and the missing
//                   'end' keywords that left the mode 2'b10 branch
//                   nested inside mode 2'b01 have been fixed.
//                 * 32x8 mode: write path indexes ram[waddr[0:3]]
//                   directly (matching the read path) instead of the
//                   original's meaningless waddr[0:3] >> 2 shift.
//                 * Reads are now clocked on the dedicated rclk input
//                   (previously unused/dead) instead of piggy-backing
//                   on wclk.
//-----------------------------------------------------
module frac_mem_256_core (
    input  wire [0:4]  waddr,
    input  wire [0:4]  raddr,
    input  wire [0:31] d_in,
    input  wire        wen,
    input  wire        ren,
    output wire [0:31] d_out,
    input  wire        wclk,
    input  wire        rclk,
    input  wire [0:1]  mode
);

    reg [0:15] ram [0:15];
    reg [0:31] internal;
    assign d_out = internal;

    // ---------------- Write path ----------------
    always @(posedge wclk) begin
        case (mode)
            2'b00: begin // 16 x 16
                if (wen) begin
                    ram[waddr[0:3]] <= d_in[0:15];
                end
            end
            2'b01: begin // 8 x 32
                if (wen) begin
                    ram[{waddr[0:2], 1'b0}] <= d_in[0:15];
                    ram[{waddr[0:2], 1'b1}] <= d_in[16:31];
                end
            end
            2'b10: begin // 32 x 8
                if (wen) begin
                    if (waddr[4] == 1'b0) begin
                        ram[waddr[0:3]][0:7]  <= d_in[0:7];
                    end else begin
                        ram[waddr[0:3]][8:15] <= d_in[0:7];
                    end
                end
            end
        endcase
    end

    // ---------------- Read path ----------------
    always @(posedge rclk) begin
        case (mode)
            2'b00: begin // 16 x 16
                if (ren) begin
                    internal[0:15]  <= ram[raddr[0:3]];
                    internal[16:31] <= 16'b0;
                end
            end
            2'b01: begin // 8 x 32
                if (ren) begin
                    internal[0:15]  <= ram[{raddr[0:2], 1'b0}];
                    internal[16:31] <= ram[{raddr[0:2], 1'b1}];
                end
            end
            2'b10: begin // 32 x 8
                if (ren) begin
                    if (raddr[4] == 1'b0) begin
                        internal[0:7] <= ram[raddr[0:3]][0:7];
                    end else begin
                        internal[0:7] <= ram[raddr[0:3]][8:15];
                    end
                    internal[8:31] <= 24'b0;
                end
            end
        endcase
    end
endmodule

//---------------------------------------------------------------------
// Design Name : frac_mem_256_preload
// File Name   : frac_mem_256_preload.v
// Function    : Fracturable dual port RAM (frac_mem_256) with
//               preload/initialization support. Combines
//               frac_mem_256_core_preload (RAM + mux bridge) and
//               frac_mem_256_preload_initializer (FSM) into a single
//               module. A dedicated preload_clk drives the initializer
//               FSM and is muxed onto the RAM write clock only while
//               preload_busy is asserted, isolating initialization
//               traffic from the system clock domain. Preload always
//               loads the memory as 16 raw rows of 16 bits (256 bits
//               total), independent of which fracturable mode the
//               system interface is configured for afterwards.
// Coder       : Xifan Tang
//---------------------------------------------------------------------
module frac_mem_256_preload (
    input  wire        clk,          // System clock (drives wclk/rclk)
    input  wire        preload_clk,  // Dedicated clock for preload initializer;
                                      // muxed onto RAM wclk during initialization
    input  wire        rst_n,
    // Initialization control
    input  wire        init_start,
    output wire         init_done,
    // Initialization data source (raw 16-row indexed buses)
    output wire [0:3]  init_src_addr,
    input  wire [0:15] init_src_data,
    // System functional interface
    input  wire        wen,
    input  wire        ren,
    input  wire [0:4]  waddr,
    input  wire [0:4]  raddr,
    input  wire [0:31] d_in,
    input  wire [0:1]  mode,
    output wire [0:31] d_out
);

    // ----------------------------------------------------------------
    // Internal wires connecting the initializer FSM to the core+mux
    // ----------------------------------------------------------------
    wire        preload_busy;
    wire        preload_wen;
    wire [0:3]  preload_waddr;
    wire [0:15] preload_d_in;

    // ----------------------------------------------------------------
    // frac_mem_256_preload_initializer instance
    // Clocked by preload_clk so the FSM and its write sequencing run
    // in the preload clock domain, independent of the system clock.
    // ----------------------------------------------------------------
    frac_mem_256_preload_initializer initializer_inst (
        .clk            (preload_clk),
        .rst_n          (rst_n),
        .init_start     (init_start),
        .init_done      (init_done),
        .init_src_addr  (init_src_addr),
        .init_src_data  (init_src_data),
        .preload_busy   (preload_busy),
        .preload_wen    (preload_wen),
        .preload_waddr  (preload_waddr),
        .preload_d_in   (preload_d_in)
    );

    // ----------------------------------------------------------------
    // frac_mem_256_core_preload instance (RAM + mux bridge)
    // Receives both clocks; core_preload muxes wclk internally.
    // ----------------------------------------------------------------
    frac_mem_256_core_preload core_preload_inst (
        .preload_busy   (preload_busy),
        .preload_wen    (preload_wen),
        .preload_waddr  (preload_waddr),
        .preload_d_in   (preload_d_in),
        .clk            (clk),
        .preload_clk    (preload_clk),
        .wen            (wen),
        .ren            (ren),
        .waddr          (waddr),
        .raddr          (raddr),
        .d_in           (d_in),
        .mode           (mode),
        .d_out          (d_out)
    );

endmodule

//-----------------------------------------------------
// Design Name : frac_mem_256_core_preload
// Function    : Instantiates mux bridge + core RAM.
//               The RAM write clock is muxed between preload_clk
//               (during initialization) and the system clk (otherwise),
//               ensuring preload writes are clocked by preload_clk only
//               while preload_busy is asserted. Reads always use the
//               system clk, matching the dpram_8x16 reference pattern.
//-----------------------------------------------------
module frac_mem_256_core_preload (
    input  wire        preload_busy,
    input  wire        preload_wen,
    input  wire [0:3]  preload_waddr,
    input  wire [0:15] preload_d_in,
    input  wire        clk,          // System clock
    input  wire        preload_clk,  // Preload clock, active during initialization
    input  wire        wen,
    input  wire        ren,
    input  wire [0:4]  waddr,
    input  wire [0:4]  raddr,
    input  wire [0:31] d_in,
    input  wire [0:1]  mode,
    output wire [0:31] d_out
);
    wire        to_core_wen;
    wire        to_core_ren;
    wire [0:4]  to_core_waddr;
    wire [0:4]  to_core_raddr;
    wire [0:31] to_core_data_in;
    wire [0:1]  to_core_mode;

    // ----------------------------------------------------------------
    // Write-clock mux:
    //   preload_busy = 1 -> use preload_clk (gated: AND with preload_busy
    //                        so no stray edges leak through after DONE)
    //   preload_busy = 0 -> use system clk
    //
    // The AND gate creates a clock gate on preload_clk, preventing any
    // preload_clk edges from reaching the RAM once preload_busy deasserts.
    // ----------------------------------------------------------------
    wire gated_preload_clk;
    wire wclk_muxed;

    assign gated_preload_clk = preload_clk & preload_busy;
    assign wclk_muxed        = (preload_busy) ? gated_preload_clk : clk;

    // Bridge Multiplexer Block Routing
    frac_mem_256_preload_ctrl mux_bridge_inst (
        .sys_wen        (wen),
        .sys_ren        (ren),
        .sys_waddr      (waddr),
        .sys_raddr      (raddr),
        .sys_d_in       (d_in),
        .sys_mode       (mode),
        .preload_busy   (preload_busy),
        .preload_wen    (preload_wen),
        .preload_waddr  (preload_waddr),
        .preload_d_in   (preload_d_in),
        .core_wen       (to_core_wen),
        .core_ren       (to_core_ren),
        .core_waddr     (to_core_waddr),
        .core_raddr     (to_core_raddr),
        .core_data_in   (to_core_data_in),
        .core_mode      (to_core_mode)
    );

    // Core RAM: wclk is muxed between preload_clk and system clk
    frac_mem_256_core memory_0 (
        .wclk           (wclk_muxed),
        .wen            (to_core_wen),
        .waddr          (to_core_waddr),
        .d_in           (to_core_data_in),
        .mode           (to_core_mode),
        .rclk           (clk),
        .ren            (to_core_ren),
        .raddr          (to_core_raddr),
        .d_out          (d_out)
    );
endmodule

//---------------------------------------------------------------------
// Design Name : frac_mem_256_preload_initializer
// Description : Memory initializer controller for frac_mem_256_preload.
//               Sequentially populates the 16 raw rows (16-bit each,
//               256 bits total) of the underlying RAM. Clocked by
//               preload_clk (supplied externally) so its FSM and write
//               sequencing are in the preload clock domain.
//---------------------------------------------------------------------
module frac_mem_256_preload_initializer (
    input  wire        clk,        // Driven by preload_clk from the top level
    input  wire        rst_n,
    // Control interface to trigger initialization
    input  wire        init_start,
    output reg          init_done,
    // Interface to external initialization source (raw 16-row indexed buses)
    output reg  [0:3]  init_src_addr,
    input  wire [0:15] init_src_data,
    // Outbound connections to frac_mem_256_core_preload
    output reg          preload_busy,
    output reg          preload_wen,
    output reg  [0:3]  preload_waddr,
    output reg  [0:15] preload_d_in
);
    // State Encoding
    localparam IDLE       = 2'b00,
               PREPARE    = 2'b01,
               WRITE_DATA = 2'b10,
               DONE       = 2'b11;

    reg [0:1] state, next_state;
    reg [0:3] addr_counter;
    reg counter_en;
    reg counter_rst;

    // Address counter tracking the RAM rows (0 to 15)
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            addr_counter <= 4'b0000;
        end else if (counter_rst) begin
            addr_counter <= 4'b0000;
        end else if (counter_en) begin
            addr_counter <= addr_counter + 1'b1;
        end
    end

    // State Transitions
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state <= IDLE;
        end else begin
            state <= next_state;
        end
    end

    // Next State and Output FSM Logic
    always @(*) begin
        next_state    = state;
        preload_busy  = 1'b0;
        preload_wen   = 1'b0;
        counter_en    = 1'b0;
        counter_rst   = 1'b0;
        init_done     = 1'b0;
        // Match source address mapping to current counter step
        init_src_addr = addr_counter;
        preload_waddr = addr_counter;
        preload_d_in  = init_src_data;

        case (state)
            IDLE: begin
                counter_rst = 1'b1;
                if (init_start) begin
                    next_state = PREPARE;
                end
            end
            PREPARE: begin
                // Give external initialization source 1 cycle to settle data
                preload_busy = 1'b1;
                next_state   = WRITE_DATA;
            end
            WRITE_DATA: begin
                preload_busy = 1'b1;
                preload_wen  = 1'b1;  // Assert write enable to RAM
                counter_en   = 1'b1;  // Increment address for next cycle
                if (addr_counter == 4'b1111) begin
                    next_state = DONE;
                end else begin
                    next_state = WRITE_DATA;
                end
            end
            DONE: begin
                init_done = 1'b1;
                if (!init_start) begin
                    next_state = IDLE;
                end
            end
            default: next_state = IDLE;
        endcase
    end
endmodule
