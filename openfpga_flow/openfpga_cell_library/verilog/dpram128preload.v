//---------------------------------------------------------------------
// Design Name : dpram_8x16_preload_ctrl (Port-to-Port Routing Version)
// Description : Manages muxing logic externally. Outputs clean, resolved 
//               signals ready to drive a standalone dpram_8x16_core.
//---------------------------------------------------------------------
module dpram_8x16_preload_ctrl (
    // System Functional Interface Inputs
    input wire         sys_wen,
    input wire         sys_ren,
    input wire  [0:2]  sys_waddr,
    input wire  [0:2]  sys_raddr,
    input wire  [0:15] sys_d_in,
    
    // Preload Controller Interface Inputs
    input wire         preload_busy,  
    input wire         preload_wen,   
    input wire  [0:2]  preload_waddr, 
    input wire  [0:15] preload_d_in,  

    // Core Memory Block Driving Outputs
    output wire        core_wen,
    output wire        core_ren,
    output wire [0:2]  core_waddr,
    output wire [0:2]  core_raddr,
    output wire [0:15] core_data_in
);

    // --- Integrated Multiplexer Logic ---
    // If preload_busy is active, route controller signals to the Core outputs.
    // Otherwise, pass through the native system inputs.
    assign core_wen     = (preload_busy) ? preload_wen   : sys_wen;
    assign core_waddr   = (preload_busy) ? preload_waddr : sys_waddr;
    assign core_data_in = (preload_busy) ? preload_d_in  : sys_d_in;
    
    // Safely disable memory reads while initialization cycles are occurring
    assign core_ren     = (preload_busy) ? 1'b0           : sys_ren;
    assign core_raddr   = sys_raddr;

endmodule

//---------------------------------------------------------------------
// Design Name : dpram_preload_initializer (Big-Endian Version)
// Description : Memory initializer controller for OpenFPGA dpram_8x16_preload
//               Sequentially populates the 8 words (16-bit) of the RAM.
//---------------------------------------------------------------------
module dpram_preload_initializer (
    input wire clk,
    input wire rst_n,
    
    // Control interface to trigger initialization
    input wire init_start,
    output reg init_done,

    // Interface to external initialization source (Big-Endian indexed buses)
    output reg [0:2]  init_src_addr,
    input wire [0:15] init_src_data,

    // Outbound Connections to dpram_8x16_preload 
    output reg        preload_busy,
    output reg        preload_wen,
    output reg [0:2]  preload_waddr,
    output reg [0:15] preload_d_in
);

    // State Encoding
    localparam IDLE       = 2'b00,
               PREPARE    = 2'b01,
               WRITE_DATA = 2'b10,
               DONE       = 2'b11;

    reg [0:1] state, next_state;
    reg [0:2] addr_counter;
    reg       counter_en;
    reg       counter_rst;

    // Address counter tracking the RAM words (0 to 7)
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            addr_counter <= 3'b000;
        end else if (counter_rst) begin
            addr_counter <= 3'b000;
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
        
        // Match source address mapping to current Big-Endian counter step
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
                preload_wen  = 1'b1; // Assert write enable to RAM
                counter_en   = 1'b1; // Increment address for the next cycle
                
                if (addr_counter == 3'b111) begin
                    next_state = DONE;
                end else begin
                    next_state = WRITE_DATA;
                end
            end

            DONE: begin
                init_done  = 1'b1;
                if (!init_start) begin
                    next_state = IDLE;
                end
            end
            
            default: next_state = IDLE;
        endcase
    end

endmodule


module dpram_8x16_core (
    input wclk, input wen, input [0:2] waddr, input [0:15] data_in,
    input rclk, input ren, input [0:2] raddr, output [0:15] d_out
);
    // Explicit Big-Endian dimensions for memory registers array
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

//-----------------------------------------------------
// Design Name : dpram_8x16_preload
// File Name   : dpram128preload.v
// Function    : Dual port RAM 8x16 with preload blocks for initialization 
// Coder       : Xifan Tang
//-----------------------------------------------------
module dpram_8x16_preload (
    input wire         preload_busy,
    input wire         preload_wen,
    input wire  [0:2]  preload_waddr,
    input wire  [0:15] preload_d_in,
    input clk,
    input wen,
    input ren,
    input  [0:2]  waddr,
    input  [0:2]  raddr,
    input  [0:15] d_in,
    output [0:15] d_out
);

    wire        to_core_wen;
    wire        to_core_ren;
    wire [0:2]  to_core_waddr;
    wire [0:2]  to_core_raddr;
    wire [0:15] to_core_data_in;

    // Bridge Multiplexer Block Routing
    dpram_8x16_preload_ctrl mux_bridge_inst (
        .sys_wen       (wen),
        .sys_ren       (ren),
        .sys_waddr     (waddr),
        .sys_raddr     (raddr),
        .sys_d_in      (d_in),
        .preload_busy  (preload_busy),
        .preload_wen   (preload_wen),
        .preload_waddr (preload_waddr),
        .preload_d_in  (preload_d_in),
        .core_wen      (to_core_wen),
        .core_ren      (to_core_ren),
        .core_waddr    (to_core_waddr),
        .core_raddr    (to_core_raddr),
        .core_data_in  (to_core_data_in)
    );

    // Fixed internal Core RAM block utilizing clean big endian array bounds
    dpram_8x16_core memory_0 (
        .wclk    (clk),
        .wen     (to_core_wen),
        .waddr   (to_core_waddr),
        .data_in (to_core_data_in),
        .rclk    (clk),
        .ren     (to_core_ren),
        .raddr   (to_core_raddr),
        .d_out   (d_out)
    );
endmodule

