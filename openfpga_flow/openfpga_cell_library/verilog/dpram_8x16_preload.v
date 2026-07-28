//---------------------------------------------------------------------
// Design Name : dpram_8x16_preload_ctrl
//---------------------------------------------------------------------
module dpram_8x16_preload_ctrl (
    input wire sys_wen,
    input wire sys_ren,
    input wire [0:2] sys_waddr,
    input wire [0:2] sys_raddr,
    input wire [0:15] sys_d_in,
    input wire preload_busy,
    input wire preload_wen,
    input wire [0:2] preload_waddr,
    input wire [0:15] preload_d_in,
    output wire core_wen,
    output wire core_ren,
    output wire [0:2] core_waddr,
    output wire [0:2] core_raddr,
    output wire [0:15] core_data_in
);
    assign core_wen      = (preload_busy) ? preload_wen   : sys_wen;
    assign core_waddr    = (preload_busy) ? preload_waddr : sys_waddr;
    assign core_data_in  = (preload_busy) ? preload_d_in  : sys_d_in;
    assign core_ren      = (preload_busy) ? 1'b0 : sys_ren;
    assign core_raddr    = sys_raddr;
endmodule

module dpram_8x16_core (
    input wclk, input wen, input [0:2] waddr, input [0:15] data_in,
    input rclk, input ren, input [0:2] raddr, output [0:15] d_out
);
    reg [0:15] ram [0:7];
    reg [0:15] internal;
    assign d_out = internal;
    always @(posedge wclk) begin
        if(wen) ram[waddr] <= data_in;
    end
    always @(posedge rclk) begin
        if(ren) internal <= ram[raddr];
    end
endmodule

//---------------------------------------------------------------------
// Design Name : dpram_8x16_preload
// Change      : init_src_addr is now an INPUT. The external controller
//               (testbench / mem_init_addr counter) drives the address;
//               the initializer FSM uses it to fetch the correct data
//               word while its own addr_counter still sequences writes.
//---------------------------------------------------------------------
module dpram_8x16_preload (
    input wire clk,
    input wire preload_clk,
    input wire rst_n,
    input wire init_start,
    output wire init_done,
    // init_src_addr is now INPUT: driven by the external address counter
    input wire [0:2] init_src_addr,   // <-- changed from output to input
    input wire [0:15] init_src_data,
    input wire wen,
    input wire ren,
    input wire [0:2] waddr,
    input wire [0:2] raddr,
    input wire [0:15] d_in,
    output wire [0:15] d_out
);
    wire        preload_busy;
    wire        preload_wen;
    wire [0:2]  preload_waddr;
    wire [0:15] preload_d_in;

    dpram_8x16_preload_initializer initializer_inst (
        .clk            (preload_clk),
        .rst_n          (rst_n),
        .init_start     (init_start),
        .init_done      (init_done),
        .init_src_addr  (init_src_addr),  // pass-through: external → FSM
        .init_src_data  (init_src_data),
        .preload_busy   (preload_busy),
        .preload_wen    (preload_wen),
        .preload_waddr  (preload_waddr),
        .preload_d_in   (preload_d_in)
    );

    dpram_8x16_core_preload core_preload_inst (
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
        .d_out          (d_out)
    );
endmodule

//---------------------------------------------------------------------
// Design Name : dpram_8x16_core_preload  (unchanged)
//---------------------------------------------------------------------
module dpram_8x16_core_preload (
    input wire preload_busy,
    input wire preload_wen,
    input wire [0:2]  preload_waddr,
    input wire [0:15] preload_d_in,
    input wire clk,
    input wire preload_clk,
    input wire wen,
    input wire ren,
    input wire [0:2]  waddr,
    input wire [0:2]  raddr,
    input wire [0:15] d_in,
    output wire [0:15] d_out
);
    wire        to_core_wen;
    wire        to_core_ren;
    wire [0:2]  to_core_waddr;
    wire [0:2]  to_core_raddr;
    wire [0:15] to_core_data_in;

    wire gated_preload_clk;
    wire wclk_muxed;
    assign gated_preload_clk = preload_clk & preload_busy;
    assign wclk_muxed        = (preload_busy) ? gated_preload_clk : clk;

    dpram_8x16_preload_ctrl mux_bridge_inst (
        .sys_wen        (wen),
        .sys_ren        (ren),
        .sys_waddr      (waddr),
        .sys_raddr      (raddr),
        .sys_d_in       (d_in),
        .preload_busy   (preload_busy),
        .preload_wen    (preload_wen),
        .preload_waddr  (preload_waddr),
        .preload_d_in   (preload_d_in),
        .core_wen       (to_core_wen),
        .core_ren       (to_core_ren),
        .core_waddr     (to_core_waddr),
        .core_raddr     (to_core_raddr),
        .core_data_in   (to_core_data_in)
    );

    dpram_8x16_core memory_0 (
        .wclk           (wclk_muxed),
        .wen            (to_core_wen),
        .waddr          (to_core_waddr),
        .data_in        (to_core_data_in),
        .rclk           (clk),
        .ren            (to_core_ren),
        .raddr          (to_core_raddr),
        .d_out          (d_out)
    );
endmodule

//---------------------------------------------------------------------
// Design Name : dpram_8x16_preload_initializer
// Change      : init_src_addr is now an INPUT port. The FSM no longer
//               generates it; instead the external counter drives it so
//               the correct data word is presented on init_src_data.
//               The FSM's own addr_counter still increments to sequence
//               preload_waddr (which RAM cell to write), and must stay
//               in lock-step with the external address counter.
//---------------------------------------------------------------------
module dpram_8x16_preload_initializer (
    input wire clk,
    input wire rst_n,
    input wire init_start,
    output reg init_done,
    // init_src_addr is now INPUT -- driven by the external address counter
    input wire [0:2] init_src_addr,   // <-- changed from output reg to input
    input wire [0:15] init_src_data,
    output reg preload_busy,
    output reg preload_wen,
    output reg [0:2]  preload_waddr,
    output reg [0:15] preload_d_in
);
    localparam IDLE       = 2'b00,
               PREPARE    = 2'b01,
               WRITE_DATA = 2'b10,
               DONE       = 2'b11;

    reg [0:1] state, next_state;
    reg [0:2] addr_counter;
    reg counter_en;
    reg counter_rst;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            addr_counter <= 3'b000;
        end else if (counter_rst) begin
            addr_counter <= 3'b000;
        end else if (counter_en) begin
            addr_counter <= addr_counter + 1'b1;
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) state <= IDLE;
        else        state <= next_state;
    end

    always @(*) begin
        next_state   = state;
        preload_busy = 1'b0;
        preload_wen  = 1'b0;
        counter_en   = 1'b0;
        counter_rst  = 1'b0;
        init_done    = 1'b0;
        // init_src_addr is now an input -- removed: init_src_addr = addr_counter
        // preload_waddr still tracks addr_counter (which RAM word to write)
        preload_waddr = addr_counter;
        preload_d_in  = init_src_data;  // data fetched at the externally-driven address

        case (state)
            IDLE: begin
                counter_rst = 1'b1;
                if (init_start) next_state = PREPARE;
            end
            PREPARE: begin
                preload_busy = 1'b1;
                next_state   = WRITE_DATA;
            end
            WRITE_DATA: begin
                preload_busy = 1'b1;
                preload_wen  = 1'b1;
                counter_en   = 1'b1;
                if (addr_counter == 3'b111) next_state = DONE;
                else                        next_state = WRITE_DATA;
            end
            DONE: begin
                init_done = 1'b1;
                if (!init_start) next_state = IDLE;
            end
            default: next_state = IDLE;
        endcase
    end
endmodule