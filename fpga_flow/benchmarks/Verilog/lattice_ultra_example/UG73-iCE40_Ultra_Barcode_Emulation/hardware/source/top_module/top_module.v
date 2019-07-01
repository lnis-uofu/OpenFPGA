//==================================================================
//   >>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<
//   ------------------------------------------------------------------
//   Copyright (c) 2014 by Lattice Semiconductor Corporation
// 					ALL RIGHTS RESERVED
//   ------------------------------------------------------------------
//
//   Permission:
//
//      Lattice SG Pte. Ltd. grants permission to use this code for use
//   in synthesis for any Lattice programmable logic product.  Other
//   use of this code, including the selling or duplication of any
//   portion is strictly prohibited.

//
//   Disclaimer:
//
//   This VHDL or Verilog source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Lattice provides no warranty
//   regarding the use or functionality of this code.
//
//   --------------------------------------------------------------------
//
//                  Lattice SG Pte. Ltd.
//                  101 Thomson Road, United Square #07-02
// 					Singapore 307591
//
//
//                  TEL: 1-800-Lattice (USA and Canada)
//	+65-6631-2000 (Singapore)
//                       +1-503-268-8001 (other locations)
//
//                  web: http://www.latticesemi.com/
//                  email: techsupport@latticesemi.com
//
//   --------------------------------------------------------------------
//

//`define SIM
`define i2c
//`define spi
//`define uart



module top_module
    // Outputs
 (   	o_led,
    // Inouts
	`ifdef i2c
	   	io_i2c_scl, io_i2c_sda,
	`endif
	//spi signals
	`ifdef spi
		i_sck, i_csn, i_mosi,o_miso,
	`endif
	`ifdef uart
		o_tx,i_rx,
	`endif
    //testpoints
	drive_on,
    // Inputs
   	 i_clk
    )/*synthesis syn_dspstyle=logic*/;

    //input i_rst;

    output wire o_led;
    output drive_on;
	input i_clk;
`ifdef i2c
	inout io_i2c_scl;
	inout io_i2c_sda;
`endif
`ifdef spi
	input  i_sck;
    input  i_csn;
    input  i_mosi;
    output o_miso;
`endif
`ifdef uart
	output o_tx;
	input i_rx;
`endif
    wire  sclin_i;
    wire  sdain_i;
    wire  sdaout_i;
    wire [7:0] datain_i;
    wire       write_en_i;
    wire [7:0] dataout_i;
    wire [7:0] regaddr_i;
    wire       read_en_i;
    reg [15:0] poweron_reset_count_i = 0; //initialized for simulation
    reg        poweron_reset_n_i = 0; // initialized for simulation
    wire       rst_i;

//mobeam control logic interface//
wire [15:0] o_ba_mem_data;
wire [7:0] i_ba_mem_addr;
wire i_ba_mem_rd_en;
wire i_ba_mem_rd_clk;
wire rd_revision_code;
wire rst_mobeam;
wire  [7:0] o_bsr_mem_data;
wire [8:0] i_bsr_mem_addr;
wire i_bsr_mem_rd_en;
wire i_bsr_mem_rd_clk;
wire o_new_data_rd;
wire o_bsr_data_strobe;
wire i_config_reg_done;
////////////////////////////////
wire clk_20mhz_g;

testpll_pll testpll_pll_inst(.REFERENCECLK(i_clk),
                             .PLLOUTCORE(),
                             .PLLOUTGLOBAL(clk_20mhz_g),
                             .RESET(~rst_i/*1'b1*/),
                             .LOCK());

//selection of SPI or I2C///
`ifdef spi
user_logic_control_reg_spidata_buf spi_mobeam_logic_interface
(
///spi Signal/////
        .i_sys_clk(i_clk),
	.rst(rst_i),
        .i_sck(i_sck),
        .i_csn(i_csn),
        .i_mosi(i_mosi),
        .o_miso(o_miso),
///Mobeam Control Signals///
	.rd_revision_code(rd_revision_code),
	.rst_mobeam(rst_mobeam),
	.led_polarity(led_polarity),
	.mobeam_start_stop(mobeam_start_stop),
	.o_ba_mem_data(o_ba_mem_data),
	.i_ba_mem_addr(i_ba_mem_addr),
	.i_ba_mem_rd_en(i_ba_mem_rd_en),
	.i_ba_mem_rd_clk(i_ba_mem_rd_clk),
	.o_bsr_mem_data(o_bsr_mem_data),
	.i_bsr_mem_addr(i_bsr_mem_addr),
	.i_bsr_mem_rd_en(i_bsr_mem_rd_en),
	.i_bsr_mem_rd_clk(i_bsr_mem_rd_clk)
//	.i_config_reg_done(i_config_reg_done),
//	.o_new_data_rd(o_new_data_rd),
//	.o_data_strobe(o_bsr_data_strobe)
);
`endif

`ifdef i2c
user_logic_control_reg_data_buf i2c_mobeam_logic_interface
(
///i2c Signal/////
	.clk(clk_20mhz_g),
	.rst(rst_i),
	.scl(sclin_i),
	.sdaout(sdaout_i),
	.sdaIn(sdain_i),
///Mobeam Control Signals///
	.rd_revision_code(rd_revision_code),
	.rst_mobeam(rst_mobeam),
	.led_polarity(led_polarity),
	.mobeam_start_stop(mobeam_start_stop),
	.o_ba_mem_data(o_ba_mem_data),
	.i_ba_mem_addr(i_ba_mem_addr),
	.i_ba_mem_rd_en(i_ba_mem_rd_en),
	.i_ba_mem_rd_clk(i_ba_mem_rd_clk),
	.o_bsr_mem_data(o_bsr_mem_data),
	.i_bsr_mem_addr(i_bsr_mem_addr),
	.i_bsr_mem_rd_en(i_bsr_mem_rd_en),
	.i_bsr_mem_rd_clk(i_bsr_mem_rd_clk)
//	.i_config_reg_done(i_config_reg_done),
//	.o_new_data_rd(o_new_data_rd),
//	.o_data_strobe(o_bsr_data_strobe)
);
`endif

//--------------------------------------------------------------
    //uart module instantiation
`ifdef uart
    uart_top u_inst(
                    .i_sys_clk              (clk_20mhz_g),
                    .i_sys_rst              (rst_i),
                    .i_rx                   (i_rx),
                    //outputs
                    .o_tx                   (o_tx),
                    .o_done                 (done_i),
                    ///Mobeam Control Signals
	            .rd_revision_code       (rd_revision_code),
	            .rst_mobeam             (rst_mobeam),
	            .led_polarity           (led_polarity),
	            .mobeam_start_stop      (mobeam_start_stop),
	            .o_ba_mem_data          (o_ba_mem_data),
	            .i_ba_mem_addr          (i_ba_mem_addr),
	            .i_ba_mem_rd_en         (i_ba_mem_rd_en),
	            .i_ba_mem_rd_clk        (i_ba_mem_rd_clk),
	            .o_bsr_mem_data         (o_bsr_mem_data),
	            .i_bsr_mem_addr         (i_bsr_mem_addr),
	            .i_bsr_mem_rd_en        (i_bsr_mem_rd_en),
	            .i_bsr_mem_rd_clk       (i_bsr_mem_rd_clk)
                    );
`endif
///////////////////////////////////////////////////////

////MobeamControlFSM//////
mobeam_control_fsm fsm_mobeam(
	 .sys_clk_i(clk_20mhz_g),
	 ///Mobeam Control Signals///
	 	.start_stop(mobeam_start_stop),
		 .rst_mobeam(rst_mobeam|rst_i),
	//bsr memory signals
		 .bsr_mem_data(o_bsr_mem_data),
		 .bsr_mem_clk(i_bsr_mem_rd_clk),
		 .bsr_mem_addr(i_bsr_mem_addr),
		 .bsr_mem_rd_en(i_bsr_mem_rd_en),

	//ba memory signals
		 .ba_mem_data(o_ba_mem_data),
		 .ba_mem_clk(i_ba_mem_rd_clk),
		 .ba_mem_addr(i_ba_mem_addr),
		 .ba_mem_rd_en(i_ba_mem_rd_en),

	//TO LED DRIVER
		.o_byte_data(barcode_array),
		.shift_done(byte_done),
		.bit_done(bit_done),
		.txn_start(txn_start),
	//	.bsr_load_done(),
		.bsr_bw(bar_width)

);

wire [7:0]  bar_width, barcode_array;
/////LED Driver//////
led_driver led_drive_inst (
		 .sys_clk(clk_20mhz_g),
		 .mobeam_start_stop(mobeam_start_stop),
		 .led_polarity(led_polarity),
		 .rst_n(~rst_i|rst_mobeam),
		 .txn_start(txn_start),
		 .bar_width(bar_width),
		 .barcode_array(barcode_array),
		 .byte_done(byte_done),
		 .bit_done(bit_done),
		 .drive_on(drive_on),
		 .oled(o_led)
		//output dynamic_clk);
   		 );
////////////////////////////////////////

////////////////////////////////////////////////
/////oled data verification/////////////////////
 `ifdef SIM

reg [7:0] capture_oled=0;
reg [7:0] oled_check=0;
integer i=8;
integer mon;


initial  begin
	mon = $fopen("monitor.txt","w");   //file to write
end

always @(posedge i_clk) begin
//	 $fwrite(mon,"%h \n",oled_check);
	if(txn_start && /*rise_reload_ba_reg_dtc*/fall_reload_ba_reg_dtc) begin
		//if(led_polarity)
			capture_oled[i]= o_led;
		//else
		//	capture_oled[i]= ~o_led;
	end
 end

 always @( negedge fall_reload_ba_reg_dtc) begin
	 if(i==0) begin
		 oled_check=capture_oled;

		 i<=7;
		 end
	else

			i<=i-1;
	end


always @(posedge i_clk) begin
	if(txn_start && byte_done)
	$fwrite(mon,"%h \n",oled_check);
	end


 	reg q_reload_ba_reg;
always @(posedge i_clk or posedge rst_i) begin
	if(rst_i)
		q_reload_ba_reg<=0;
	else
		q_reload_ba_reg<=led_drive_inst.reload_ba_reg;
	end
assign rise_reload_ba_reg_dtc=(~q_reload_ba_reg) && led_drive_inst.reload_ba_reg;

assign fall_reload_ba_reg_dtc=(q_reload_ba_reg) && (~led_drive_inst.reload_ba_reg);

`endif

//end
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
`ifdef i2c
    assign io_i2c_sda = (sdaout_i == 1'b0) ? 1'b0 : 1'bz;
    assign sdain_i = io_i2c_sda;
    assign sclin_i = io_i2c_scl;
`endif

    always @(posedge i_clk)begin
        if(poweron_reset_count_i == 256)begin
            poweron_reset_count_i <= 256;
        end else begin
            poweron_reset_count_i <= poweron_reset_count_i + 1;
        end
    end

    always @(posedge i_clk)begin
        if(poweron_reset_count_i == 256)begin
            poweron_reset_n_i <= 1;
        end else begin
            poweron_reset_n_i <= 0;
        end
    end
    assign rst_i = ~poweron_reset_n_i;
endmodule // i2c_slave