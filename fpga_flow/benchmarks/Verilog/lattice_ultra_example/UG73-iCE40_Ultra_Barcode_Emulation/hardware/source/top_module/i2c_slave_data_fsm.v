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

`include "i2c_defines.v"
`include "i2c_new_reg.v"
`timescale 1 ns / 1 ns


module serialInterface (/*AUTOARG*/
    // Outputs
    dataOut, regAddr, sdaOut, writeEn, readEn, i2c_start,
    // Inputs
    clk, dataIn, rst, scl, sdaIn
    );
    input   clk;
    input [7:0] dataIn;
    input       rst;
    input       scl;
    input       sdaIn;
    output [7:0] dataOut;
    output [7:0] regAddr;
    output       sdaOut;
    output       writeEn;
    output       readEn;
    output       i2c_start;
    
	// I2C_SLAVE_INIT_ADDR: Upper Bits <9:2> can be changed. Lower bits <1:0> are fixed.
	// For I2C Hard IP located in Upper Left <1:0> must be set to "01".
	// For I2C Hard IP located in Upper Right <1:0> must be set to "10".
      parameter I2C_SLAVE_INIT_ADDR = "0b1111100001"; //Upper Left
	//parameter I2C_SLAVE_INIT_ADDR = "0b1111100010"; //Upper Right
	
	// BUS_ADDR74: Fixed value. SBADRI [7:4] bits also should match with this value to 
	// activate the IP.
	// For I2C Hard IP located in Upper Left [7:4] must be set to "0001".
	// For I2C Hard IP located in Upper Right [7:4] must be set to "0011".	
    parameter BUS_ADDR74_STRING = "0b0001"; //Upper Left
    //parameter BUS_ADDR74_STRING = "0b0011"; //Upper Right

	// These are for the "OR" function with "wb_adr_i". Note that bits [7:4] are copies
	// of BUS_ADDR74_STRING.
    parameter BUS_ADDR74 = 8'b0001_0000; //Upper Left
    //parameter BUS_ADDR74 = 8'b0011_0000; //Upper Right
    
    reg [7:0]    regAddr;
    reg          writeEn;
    wire [7:0]   dataOut;
    reg          i2c_start;
    
    /*
     * System bus interface signals
     */
    reg [7:0]                         wb_dat_i;
    reg                               wb_stb_i;
    reg [7:0]                         wb_adr_i;
    reg                               wb_we_i;
    wire [7:0]                        wb_dat_o;
    wire                              wb_ack_o;
    /*
     * Data Read and Write Register
     */
    reg [7:0]                         temp0;
    reg [7:0]                         temp1;
    reg [7:0]                         temp2;
    reg [7:0]                         temp3;
    reg [7:0]                         n_temp0;
    reg [7:0]                         n_temp1;
    reg [7:0]                         n_temp2;
    reg [7:0]                         n_temp3;
    reg                               readEn;
    

    /*
     * i2c Module Instanitiation
     */


    i2c UUT1  (
               .wb_clk_i   (clk),
               .wb_dat_i   (wb_dat_i),
               .wb_stb_i   (wb_stb_i),
               .wb_adr_i   (wb_adr_i | BUS_ADDR74),
               .wb_we_i    (wb_we_i),
               .wb_dat_o   (wb_dat_o),
               .wb_ack_o   (wb_ack_o),
               .i2c_irqo   ( ),
               .i2c_scl    (scl),
               .i2c_sda    (sdaOut),
               .i2c_sda_in (sdaIn),
	     .rst_i(rst)
	     );
    
    defparam UUT1.I2C_SLAVE_INIT_ADDR = I2C_SLAVE_INIT_ADDR;
    defparam UUT1.BUS_ADDR74_STRING = BUS_ADDR74_STRING;
    
    /*
     * Signal & wire Declartion
     */
    reg                               efb_flag;
    reg                               n_efb_flag;
    reg [7:0]                         n_wb_dat_i;
    reg                               n_wb_stb_i;
    reg [7:0]                         n_wb_adr_i;
    reg                               n_wb_we_i;
    reg [7:0]                         c_state;
    reg [7:0]                         n_state;
    reg                               n_count_en;
    reg                               count_en;
    wire                              invalid_command = 0;

    /*
     * Output generation
     */
    
    assign dataOut = temp3;

    always @(posedge clk or posedge rst)begin
        if (rst)begin
            writeEn <= 0;
            readEn <= 0;
        end else begin
            if(c_state == `state14)begin
                writeEn <= 1'b1;
            end else begin
                writeEn <= 1'b0;
            end
            
            if(c_state == `state15)begin
                readEn <= 1'b1;
            end else if (c_state == `state13) begin              //**
                if (n_temp2 & (`MICO_EFB_I2C_SR_SRW)) begin
                    readEn <= 1'b1;
                end else begin
                    readEn <= 1'b0;
                end
            end else begin
                readEn <= 1'b0;
            end
        end
    end
    
    always @(posedge clk or posedge rst)begin
        if (rst)begin
            regAddr <= 0;
        end else begin
            if(c_state == `state2)begin
                regAddr <= 8'd0;
            end else if(c_state == `state9)begin
                regAddr <= temp1;
            //end else if(writeEn || readEn)begin
            //    regAddr <= regAddr + 1;
            end
        end
    end

    //slave start detect 
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            i2c_start <= 0;
        end else begin
            if (c_state == `state12) begin
                i2c_start <= 0;
            end else if (c_state == `state9) begin
                i2c_start <= 1;
            end
        end
    end
    
    /*
     * Main state machine
     */
    always @ (posedge clk or posedge rst) begin
        if(rst) begin
            wb_dat_i <= 8'h00;
            wb_stb_i <= 1'b0 ;
            wb_adr_i <= 8'h00;
            wb_we_i  <= 1'b0;
        end else begin
            wb_dat_i <= #1 n_wb_dat_i;
            wb_stb_i <= #1 n_wb_stb_i;
            wb_adr_i <= #1 n_wb_adr_i;
            wb_we_i  <= #1 n_wb_we_i ;
        end
    end

    always @ (posedge clk or posedge rst) begin
        if(rst) begin
            c_state  <= 10'h000;
            efb_flag <= 1'b0 ;
            count_en <= 1'b0;
        end else begin
            c_state  <= n_state   ;
            efb_flag <= n_efb_flag;
            count_en <= n_count_en;
        end
    end

    always @ (posedge clk or posedge rst) begin
        if(rst) begin
            temp0 <= 8'h00 ;
            temp1 <= 8'h00 ;
            temp2 <= 8'h00 ;
	    temp3 <= 8'h00 ;
        end else begin
            temp0 <= n_temp0 ;
            temp1 <= n_temp1 ;
            temp2 <= n_temp2 ;
	    temp3 <= n_temp3 ;
        end
    end

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            n_temp2 <= 0;
        end else begin
            n_temp2 <= wb_dat_o; //**
        end
    end
    
    /*
     * FSM combinational block
     */
    always @ ( * ) begin
        n_efb_flag   =  1'b0 ;
        n_state      = c_state ;
        n_wb_dat_i  = 8'h00;
        n_wb_stb_i  = 1'b0 ;
        n_wb_adr_i  = 8'h00;
        n_wb_we_i   = 1'b0;
        n_count_en  = 1'b0;
        n_temp0     = temp0;
        n_temp1     = temp1;
        n_temp3     = temp3;

        case(c_state)
            `state0: begin
                n_wb_dat_i =  8'h00;
                n_wb_stb_i =  1'b0 ;
                n_wb_adr_i =  8'h00;
                n_wb_we_i  =  1'b0;
                n_wb_stb_i =  1'b0 ;
                n_state =  `state1 ;
            end

            `state1: begin // Enable I2C Interface
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    n_state = `state2;
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `WRITE;
                    n_wb_adr_i = `MICO_EFB_I2C_CR;
                    n_wb_dat_i = 8'h80;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state2: begin // Clock Disable
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    n_state = `state3;
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `WRITE;
                    n_wb_adr_i = `MICO_EFB_I2C_CMDR;
                    n_wb_dat_i = 8'h04;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state3: begin // Wait for not BUSY
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    if(wb_dat_o & (`MICO_EFB_I2C_SR_BUSY))begin
                        n_state = `state4;
                    end else begin
                        n_state = c_state;
                    end
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `READ_STATUS;
                    n_wb_adr_i = `MICO_EFB_I2C_SR;
                    n_wb_dat_i =  0 ;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state4: begin // Discard data 1
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    n_temp0 = wb_dat_o;
                    n_state = `state5;
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `READ_DATA;
                    n_wb_adr_i = `MICO_EFB_I2C_RXDR;
                    n_wb_dat_i =  0 ;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state5: begin // Discard data 2
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    n_temp0 = wb_dat_o;
                    n_state = `state6;
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `READ_DATA;
                    n_wb_adr_i = `MICO_EFB_I2C_RXDR;
                    n_wb_dat_i =  0 ;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state6: begin // Clock Enable
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    n_state = `state7;
                end else begin
                    n_efb_flag = `HIGH;
                    n_wb_we_i =  `WRITE;
                    n_wb_adr_i = `MICO_EFB_I2C_CMDR;
                    n_wb_dat_i = 8'h00;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state7: begin // wait for data to come
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
	            n_temp1 = 8'h00;
                    if((wb_dat_o & (`MICO_EFB_I2C_SR_TRRDY)))begin
	                n_state = `state8; // Slave acknowledged
                    end else if (~wb_dat_o[6])begin
                        n_state = `state2; // Disable clock
                    end else begin
                        n_state = c_state;
                    end
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `READ_STATUS;
                    n_wb_adr_i = `MICO_EFB_I2C_SR;
                    n_wb_dat_i =  0 ;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end
            

            `state8: begin // Store i2C Command Information
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    n_temp1 = wb_dat_o;
                    n_state = `state9;
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `READ_DATA;
                    n_wb_adr_i = `MICO_EFB_I2C_RXDR;
                    n_wb_dat_i =  0 ;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state9: begin // Send ACK or NACK Based upon Command Receive & Wait for Stop `state 17
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
	            if(invalid_command)begin // This is tied to '0' at present
	                n_state = `state17;
                    end else begin
	                n_state = `state12;
                    end
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `WRITE;
                    n_wb_adr_i = `MICO_EFB_I2C_CMDR;
                    n_wb_dat_i = {4'h0,invalid_command,3'b000};
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end
            

            `state12: begin // Wait for TRRDY Bit
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    if(wb_dat_o & (`MICO_EFB_I2C_SR_TRRDY))begin
                        n_state = `state13;
                    end else if (~wb_dat_o[6])begin
                        n_state = `state2;
                    end else begin
                        n_state = c_state;
                    end
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `READ_STATUS;
                    n_wb_adr_i = `MICO_EFB_I2C_SR;
                    n_wb_dat_i =  0 ;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state13: begin // Check for read or write operation
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    if(wb_dat_o & (`MICO_EFB_I2C_SR_SRW))begin
                        n_state = `state15; //Read from slave
                    end else begin
                        n_state = `state14; //Write to slave
                    end
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `READ_STATUS;
                    n_wb_adr_i = `MICO_EFB_I2C_SR;
                    n_wb_dat_i =  0 ;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state14: begin // Write data
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    n_temp3 = wb_dat_o;
                    n_state = `state19;
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `READ_DATA;
                    n_wb_adr_i = `MICO_EFB_I2C_RXDR;
                    n_wb_dat_i =  0 ;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state15: begin // Send Data to Master
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    n_state = `state18;
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `WRITE;
                    n_wb_adr_i = `MICO_EFB_I2C_TXDR;
                    n_wb_dat_i = dataIn;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state17: begin // Wait till Stop is Send
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    if(~wb_dat_o[6])begin
                        n_state = `state2;
                    end else begin
                        n_state = c_state;
                    end
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `READ_STATUS;
                    n_wb_adr_i = `MICO_EFB_I2C_SR;
                    n_wb_dat_i =  0 ;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end

            `state18: begin // Wait for TxRDY flag and send data again if required
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    if(wb_dat_o & (`MICO_EFB_I2C_SR_TRRDY))begin
     	                n_state = `state15;                   // Send Data
                    end else if (~wb_dat_o[6]) begin// If Stop go to beginning
                        n_state = `state2;
                    end else begin
                        n_state = c_state;
                    end
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `READ_STATUS;
                    n_wb_adr_i = `MICO_EFB_I2C_SR;
                    n_wb_dat_i =  0 ;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end


            `state19: begin // Wait for TRRDY bit
                if (wb_ack_o && efb_flag) begin
                    n_wb_dat_i = `ALL_ZERO ;
                    n_wb_adr_i = `ALL_ZERO ;
                    n_wb_we_i =  `LOW ;
                    n_wb_stb_i = `LOW ;
                    n_efb_flag = `LOW ;
                    n_count_en = `LOW ;
                    if(wb_dat_o & (`MICO_EFB_I2C_SR_TRRDY)) begin
                        n_state = `state14;
                    end else if (~wb_dat_o[6])begin
                        n_state = `state2;
                    end else begin
                        n_state = c_state;
                    end
                end else begin
                    n_efb_flag = `HIGH ;
                    n_wb_we_i =  `READ_STATUS;
                    n_wb_adr_i = `MICO_EFB_I2C_SR;
                    n_wb_dat_i =  0 ;
                    n_wb_stb_i = `HIGH ;
                    n_state = c_state;
                end
            end
        endcase
    end


endmodule
