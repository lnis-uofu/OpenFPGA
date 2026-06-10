//-----------------------------------------------------
// Design Name : dpram_8x16_preload
// File Name   : dpram128preload.v
// Function    : Dual port RAM 8x16 with preload blocks for initialization 
// Coder       : Xifan Tang
//-----------------------------------------------------

module dpram_8x16_preload (
    // Preload Controller Interface to system bus
    input wire         preload_busy,  
    input wire         preload_wen,   
    input wire  [0:2]  preload_waddr, 
    input wire  [0:15] preload_d_in,  
    // Regular interface to FPGA routing
	input clk,
	input wen,
	input ren,
	input[0:2] waddr,
	input[0:2] raddr,
	input[0:15] d_in,
	output[0:15] d_out
);

// Preload block for initialization
// 2. Structural Multiplexer Unit
    dpram_8x16_preload_ctrl mux_bridge_inst (
        // System Inputs
        .sys_wen       (sys_wen),
        .sys_ren       (sys_ren),
        .sys_waddr     (sys_waddr),
        .sys_raddr     (sys_raddr),
        .sys_d_in      (sys_d_in),
        
        // Controller Inputs
        .preload_busy  (system_busy),
        .preload_wen   (ctrl_wen),
        .preload_waddr (ctrl_addr),
        .preload_d_in  (ctrl_wdata),
        
        // Output Interconnects out to the standalone RAM Core
        .core_wen      (to_core_wen),
        .core_ren      (to_core_ren),
        .core_waddr    (to_core_waddr),
        .core_raddr    (to_core_raddr),
        .core_data_in  (to_core_data_in)
    );

// Core memory block 
		dpram_8x16_core memory_0 (
			.wclk		(clk),
			.wen		(wen),
			.waddr		(waddr),
			.data_in	(d_in),
			.rclk		(clk),
			.ren		(ren),
			.raddr		(raddr),
			.d_out		(d_out) );

endmodule

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

module dpram_8x16_core (
	input wclk,
	input wen,
	input[0:2] waddr,
	input[0:15] data_in,
	input rclk,
	input ren,
	input[0:2] raddr,
	output[0:15] d_out );

	reg[0:15] ram[0:2];
	reg[0:15] internal;

	assign d_out = internal;

	always @(posedge wclk) begin
		if(wen) begin
			ram[waddr] <= data_in;
		end
	end

	always @(posedge rclk) begin
		if(ren) begin
			internal <= ram[raddr];
		end
	end

endmodule
